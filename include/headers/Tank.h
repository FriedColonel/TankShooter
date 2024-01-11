#ifndef _TANK_H
#define _TANK_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Graphics.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>
#include <QuickSDL/Timer.h>
#include <headers/Brick.h>
#include <headers/Bullet.h>
#include <headers/PhysicEntity.h>
#include <headers/PhysicManager.h>

#include <socket/Client.hpp>

using namespace QuickSDL;

class Tank : public PhysicEntity {
 protected:
  static const int MOVE_SPEED = 100;
  static const int MAX_BULLETS = 5;

 protected:
  int mId;

  Timer* mTimer;
  InputManager* mInput;
  PhysicManager* mPhysicMgr;
  Graphics* mGraphics;
  TSS::Client* mClient;

  bool mAlive;
  bool mAnimating;
  bool mWasHit;
  bool mIsMoving;
  bool mIsThisPlayer;

  AnimatedTexture* mBase;
  AnimatedTexture* mWeapon;
  AnimatedTexture* mDeathAnimation;

  float mMoveSpeed;
  Vector2 mMoveBoundsX;
  Vector2 mMoveBoundsY;

  DIRECTION mDirection;
  DIRECTION mFireDirection;

  Bullet* mBullets[MAX_BULLETS];

 private:
  void HandleMovement();
  void HandleFiring();

 public:
  Tank(bool isThisPlayer, BASE_POSITION basePos, COLOR color);
  ~Tank();

  void Hit(PhysicEntity* other) override;
  void Dead();
  bool WasHit();

  bool IsAnimating();

  void Shoot();
  void Shoot(Vector2 pos, GameEntity::DIRECTION direction);

  void StopMoving(Vector2 pos, GameEntity::DIRECTION direction);
  void ChangePosition(Vector2 pos, GameEntity::DIRECTION direction,
                      bool moving = true);

  void Update();
  virtual void LateUpdate();
  void Render();
};

#endif
