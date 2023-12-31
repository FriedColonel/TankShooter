#ifndef _PHYSICENTITY_H
#define _PHYSICENTITY_H

#include <headers/Collider.h>

#include <vector>

template <typename Base, typename T>
inline bool instanceof (const T* ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

class PhysicEntity : public GameEntity {
 protected:
  unsigned long mId;

  std::vector<Collider*> mColliders;

  Collider* mBroadPhaseCollider;

 public:
  PhysicEntity();
  ~PhysicEntity();

  unsigned long GetId();

  bool CheckCollision(PhysicEntity* other);

  virtual void Hit(PhysicEntity* other);

  virtual void Render();

 protected:
  void AddCollider(Collider* collider, Vector2 localPosition = VEC2_ZERO);
};

#endif
