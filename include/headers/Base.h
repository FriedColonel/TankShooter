#ifndef _BASE_H
#define _BASE_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Texture.h>
#include <QuickSDL/Timer.h>
#include <headers/Brick.h>
#include <headers/PhysicEntity.h>
#include <headers/PhysicManager.h>
#include <headers/Tank.h>

using namespace QuickSDL;

class Base : public PhysicEntity {
 private:
  Graphics* mGraphics;
  Timer* mTimer;
  PhysicManager* mPhysicMgr;

  bool mAnimating;
  bool mIsThisPlayer;

  COLOR mColor;

  float mRespawnTimer;
  float mRespawnDelay;

  int mId;

  Texture* mTexture;
  AnimatedTexture* mDeathAnimation;
  Brick* mWalls[3];

  Tank* mTank;

  BASE_POSITION mBasePosition;

 private:
  void HandlePlayerDeath();

 public:
  Base(BASE_POSITION basePos, bool isThisPlayer = true,
       COLOR color = COLOR::red);
  ~Base();

  bool WasHit();

  void Hit(PhysicEntity* other) override;

  void Update();
  void Render();
};

#endif
