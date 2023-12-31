#ifndef _PHYSICMANAGER_H
#define _PHYSICMANAGER_H

#include <headers/PhysicEntity.h>

#include <bitset>
#include <vector>

class PhysicManager {
 public:
  enum class CollisionLayers {
    Friendly,
    FriendlyProjectiles,
    Hostile,
    HostileProjectiles,
    Terrain,
    // -----------------------------
    MaxPlayers,
  };

  enum class CollisionFlags {
    None = 0x00,
    Friendly = 0x01,
    FriendlyProjectiles = 0x02,
    Hostile = 0x04,
    HostileProjectiles = 0x08,
    Terrain = 0x10,
  };

 private:
  static PhysicManager* sInstance;

  std::vector<PhysicEntity*>
      mCollisionLayers[static_cast<unsigned int>(CollisionLayers::MaxPlayers)];
  std::bitset<static_cast<unsigned int>(CollisionLayers::MaxPlayers)>
      mLayerMasks[static_cast<unsigned int>(CollisionLayers::MaxPlayers)];

  unsigned long mLastId;

 public:
  static PhysicManager* Instance();
  static void Release();

  void SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flags);

  unsigned long RegisterEntity(PhysicEntity* entity, CollisionLayers layer);
  void UnregisterEntity(unsigned long id);

  void Update();

 private:
  PhysicManager();
  ~PhysicManager();
};

inline PhysicManager::CollisionFlags operator|(
    PhysicManager::CollisionFlags a, PhysicManager::CollisionFlags b) {
  return static_cast<PhysicManager::CollisionFlags>(
      static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline PhysicManager::CollisionFlags operator&(
    PhysicManager::CollisionFlags a, PhysicManager::CollisionFlags b) {
  return static_cast<PhysicManager::CollisionFlags>(
      static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

#endif
