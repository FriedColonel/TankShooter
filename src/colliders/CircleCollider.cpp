#include <headers/CircleCollider.h>

CircleCollider::CircleCollider(int radius, bool broadPhase /*= false*/)
    : Collider(ColliderType::CIRCLE) {
  mRadius = radius;

  if (DEBUG_COLLIDERS) {
    if (broadPhase)
      SetDebugTexture(new Texture("Collider/BroadPhaseCollider.png"));
    else
      SetDebugTexture(new Texture("Collider/CircleCollider.png"));

    mDebugTexture->Scale(VEC2_ONE * (radius * 2 / 100.0f));
  }
}

CircleCollider::~CircleCollider() {}

Vector2 CircleCollider::GetFurthestPoint() {
  return VEC2_RIGHT * (mRadius + Pos(GameEntity::local).Magnitude());
}

float CircleCollider::GetRadius() { return mRadius; }
