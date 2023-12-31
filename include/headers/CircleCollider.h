#ifndef _CIRCLECOLLIDER_H_
#define _CIRCLECOLLIDER_H_

#include <headers/Collider.h>

class CircleCollider : public Collider {
 private:
  int mRadius;

 public:
  CircleCollider(int radius, bool broadPhase = false);
  ~CircleCollider();

  Vector2 GetFurthestPoint() override;

  float GetRadius();
};

#endif
