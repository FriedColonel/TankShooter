#include <headers/PhysicEntity.h>
#include <headers/PhysicHelper.h>
#include <headers/PhysicManager.h>

#include <iostream>

PhysicEntity::PhysicEntity() {
  mBroadPhaseCollider = nullptr;
  mId = 0;
}

PhysicEntity::~PhysicEntity() {
  for (int i = 0; i < mColliders.size(); i++) {
    delete mColliders[i];
    mColliders[i] = nullptr;
  }

  mColliders.clear();

  if (mBroadPhaseCollider) {
    delete mBroadPhaseCollider;
    mBroadPhaseCollider = nullptr;
  }

  if (mId != 0) {
    PhysicManager::Instance()->UnregisterEntity(mId);
  }
}

unsigned long PhysicEntity::GetId() { return mId; }

bool PhysicEntity::CheckCollision(PhysicEntity* other) {
  bool narrowPhaseCheck = false;
  if (mBroadPhaseCollider && other->mBroadPhaseCollider) {
    narrowPhaseCheck =
        ColliderColliderCheck(mBroadPhaseCollider, other->mBroadPhaseCollider);
  } else {
    narrowPhaseCheck = true;
  }

  if (narrowPhaseCheck) {
    for (int i = 0; i < mColliders.size(); i++) {
      for (int j = 0; j < other->mColliders.size(); j++) {
        if (ColliderColliderCheck(mColliders[i], other->mColliders[j])) {
          return true;
        }
      }
    }
  }

  return false;
}

void PhysicEntity::AddCollider(Collider* collider,
                               Vector2 localPosition /*= VEC2_ZERO*/) {
  collider->Parent(this);
  collider->Pos(localPosition);
  mColliders.push_back(collider);

  if (mColliders.size() > 1 ||
      mColliders[0]->GetType() != Collider::ColliderType::CIRCLE) {
    float furthestDist = mColliders[0]->GetFurthestPoint().Magnitude();
    float dist = 0.0f;

    for (int i = 1; i < mColliders.size(); i++) {
      dist = mColliders[i]->GetFurthestPoint().Magnitude();

      if (dist > furthestDist) {
        furthestDist = dist;
      }
    }

    delete mBroadPhaseCollider;
    mBroadPhaseCollider = new CircleCollider(furthestDist, true);
    mBroadPhaseCollider->Parent(this);
    mBroadPhaseCollider->Pos(VEC2_ZERO);
  }
}

void PhysicEntity::Hit(PhysicEntity* other) {
  // std::cout << "Hit!" << std::endl;
}

void PhysicEntity::Render() {
  for (int i = 0; i < mColliders.size(); i++) {
    mColliders[i]->Render();
  }

  if (mBroadPhaseCollider) {
    mBroadPhaseCollider->Render();
  }
}
