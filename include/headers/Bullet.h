#ifndef _BULLET_H
#define _BULLET_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Timer.h>
#include <headers/PhysicEntity.h>

using namespace QuickSDL;

class Bullet : public PhysicEntity {
 private:
  const int OFFSCREEN_BUFFER = 10;

  Timer* mTimer;

  float mSpeed;

  AnimatedTexture* mTexture;

  DIRECTION mDirection;

 public:
  Bullet(bool isFriendly = true);
  ~Bullet();

  void Fire(Vector2 pos, DIRECTION dir);
  void Reload();

  void Hit(PhysicEntity* other) override;

  void Update();
  void Render();
};

#endif
