#ifndef _PHYSICHELPER_H
#define _PHYSICHELPER_H

#include <QuickSDL/MathHelper.h>
#include <headers/BoxCollider.h>
#include <headers/CircleCollider.h>

inline bool PointInPolygon(Vector2 *verts, int vertCount,
                           const Vector2 &point) {
  bool retVal = false;

  for (int i = 0, j = vertCount - 1; i < vertCount; j = i++) {
    if ((verts[i].y >= point.y) != (verts[j].y >= point.y)) {
      Vector2 vec1 = (verts[i] - verts[j]).Normalized();
      Vector2 prj = verts[j] + vec1 * Dot(point - verts[j], vec1);

      if (prj.x >= point.x) {
        retVal = !retVal;
      }
    }
  }

  return retVal;
}

inline float PointToLineDistance(const Vector2 &lineStart,
                                 const Vector2 &lineEnd, const Vector2 &point) {
  Vector2 slope = lineEnd - lineStart;

  float param =
      Clamp(Dot(point - lineStart, slope) / slope.MagnitudeSqr(), 0.0f, 1.0f);

  Vector2 nearestPoint = lineStart + slope * param;

  return (point - nearestPoint).Magnitude();
}

inline bool CircleCircleCollision(CircleCollider *c1, CircleCollider *c2) {
  return (c1->Pos() - c2->Pos()).Magnitude() <
         (c1->GetRadius() + c2->GetRadius());
}

inline bool BoxCircleCollision(BoxCollider *box, CircleCollider *circle) {
  Vector2 quad[4];
  quad[0] = box->GetVertexPos(0);
  quad[1] = box->GetVertexPos(1);
  quad[2] = box->GetVertexPos(3);
  quad[3] = box->GetVertexPos(2);

  float radius = circle->GetRadius();
  Vector2 circlePos = circle->Pos();

  for (int i = 0; i < 4; i++) {
    if ((quad[i] - circlePos).Magnitude() < radius) {
      return true;
    }
  }

  if (PointToLineDistance(box->GetVertexPos(0), box->GetVertexPos(1),
                          circlePos) < radius ||
      PointToLineDistance(box->GetVertexPos(0), box->GetVertexPos(2),
                          circlePos) < radius ||
      PointToLineDistance(box->GetVertexPos(2), box->GetVertexPos(3),
                          circlePos) < radius ||
      PointToLineDistance(box->GetVertexPos(3), box->GetVertexPos(1),
                          circlePos) < radius) {
    return true;
  }

  if (PointInPolygon(quad, 4, circle->Pos())) {
    return true;
  }

  return false;
}

inline bool BoxBoxColliderCheck(BoxCollider *b1, BoxCollider *b2) {
  Vector2 projAxis[4];

  projAxis[0] = (b1->GetVertexPos(0) - b1->GetVertexPos(1)).Normalized();
  projAxis[1] = (b1->GetVertexPos(0) - b1->GetVertexPos(2)).Normalized();
  projAxis[2] = (b2->GetVertexPos(0) - b2->GetVertexPos(1)).Normalized();
  projAxis[3] = (b2->GetVertexPos(0) - b2->GetVertexPos(2)).Normalized();

  float b1Min = 0.0f, b1Max = 0.0f;
  float b2Min = 0.0f, b2Max = 0.0f;
  float proj1 = 0.0f, proj2 = 0.0f;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      proj1 = Dot(b1->GetVertexPos(j), projAxis[i]);
      proj2 = Dot(b2->GetVertexPos(j), projAxis[i]);

      if (j == 0) {
        b1Min = b1Max = proj1;
        b2Min = b2Max = proj2;
      } else {
        if (proj1 < b1Min)
          b1Min = proj1;
        else if (proj1 > b1Max)
          b1Max = proj1;

        if (proj2 < b2Min)
          b2Min = proj2;
        else if (proj2 > b2Max)
          b2Max = proj2;
      }
    }

    float halfDis1 = (b1Max - b1Min) * 0.5f;
    float halfDis2 = (b2Max - b2Min) * 0.5f;

    float midPoint1 = b1Min + halfDis1;
    float midPoint2 = b2Min + halfDis2;

    if (abs(midPoint1 - midPoint2) > halfDis1 + halfDis2) {
      return false;
    }
  }

  return true;
}

inline bool ColliderColliderCheck(Collider *c1, Collider *c2) {
  if (c1->GetType() == Collider::ColliderType::CIRCLE &&
      c2->GetType() == Collider::ColliderType::CIRCLE) {
    return CircleCircleCollision(static_cast<CircleCollider *>(c1),
                                 static_cast<CircleCollider *>(c2));
  } else if (c1->GetType() == Collider::ColliderType::BOX &&
             c2->GetType() == Collider::ColliderType::CIRCLE) {
    return BoxCircleCollision(static_cast<BoxCollider *>(c1),
                              static_cast<CircleCollider *>(c2));
  } else if (c2->GetType() == Collider::ColliderType::BOX &&
             c1->GetType() == Collider::ColliderType::CIRCLE) {
    return BoxCircleCollision(static_cast<BoxCollider *>(c2),
                              static_cast<CircleCollider *>(c1));
  } else if (c1->GetType() == Collider::ColliderType::BOX &&
             c2->GetType() == Collider::ColliderType::BOX) {
    return BoxBoxColliderCheck(static_cast<BoxCollider *>(c1),
                               static_cast<BoxCollider *>(c2));
  } else
    return false;
}

#endif
