#include <headers/PhysicManager.h>

PhysicManager* PhysicManager::sInstance = nullptr;

PhysicManager* PhysicManager::Instance() {
  if (sInstance == nullptr) {
    sInstance = new PhysicManager();
  }

  return sInstance;
}

void PhysicManager::Release() {
  delete sInstance;
  sInstance = nullptr;
}

void PhysicManager::SetLayerCollisionMask(CollisionLayers layer,
                                          CollisionFlags flags) {
  mLayerMasks[static_cast<unsigned int>(layer)] =
      std::bitset<static_cast<unsigned int>(CollisionLayers::MaxPlayers)>(
          static_cast<unsigned int>(flags));
}

unsigned long PhysicManager::RegisterEntity(PhysicEntity* entity,
                                            CollisionLayers layer) {
  mCollisionLayers[static_cast<unsigned int>(layer)].push_back(entity);

  mLastId++;

  return mLastId;
}

void PhysicManager::UnregisterEntity(unsigned long id) {
  bool found = false;

  for (int i = 0;
       i < static_cast<unsigned int>(CollisionLayers::MaxPlayers) && !found;
       i++) {
    for (int j = 0; j < mCollisionLayers[i].size(); j++) {
      if (mCollisionLayers[i][j]->GetId() == id) {
        mCollisionLayers[i].erase(mCollisionLayers[i].begin() + j);
        found = true;
      }
    }
  }
}

PhysicManager::PhysicManager() {
  mLastId = 0;

  for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxPlayers);
       i++) {
    mLayerMasks[i] =
        std::bitset<static_cast<unsigned int>(CollisionLayers::MaxPlayers)>(
            static_cast<unsigned int>(CollisionFlags::None));
  }
}

PhysicManager::~PhysicManager() {
  for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxPlayers);
       i++) {
    mCollisionLayers[i].clear();
  }
}

void PhysicManager::Update() {
  for (unsigned int i = 0;
       i < static_cast<unsigned int>(CollisionLayers::MaxPlayers); i++) {
    for (unsigned int j = 0;
         j < static_cast<unsigned int>(CollisionLayers::MaxPlayers); j++) {
      if (mLayerMasks[i].test(j) && i <= j) {
        for (unsigned int k = 0; k < mCollisionLayers[i].size(); k++) {
          for (unsigned int l = 0; l < mCollisionLayers[j].size(); l++) {
            if (mCollisionLayers[i][k]->Active() &&
                mCollisionLayers[j][l]->Active() &&
                mCollisionLayers[i][k]->GetId() !=
                    mCollisionLayers[j][l]->GetId() &&
                mCollisionLayers[i][k]->CheckCollision(
                    mCollisionLayers[j][l])) {
              mCollisionLayers[i][k]->mIsColliding = true;
              mCollisionLayers[i][k]->Hit(mCollisionLayers[j][l]);
              mCollisionLayers[j][l]->Hit(mCollisionLayers[i][k]);
            } else {
              mCollisionLayers[i][k]->mIsColliding = false;
            }
          }
        }
      }
    }
  }
}
