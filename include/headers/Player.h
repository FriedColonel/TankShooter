#ifndef _PLAYER_H
#define _PLAYER_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>
#include <QuickSDL/Timer.h>
#include <headers/Base.h>
#include <headers/Bullet.h>
#include <headers/PhysicEntity.h>

using namespace QuickSDL;

class Player : public PhysicEntity {
 private:
  static const int MOVE_SPEED = 100;
  Timer* mTimer;
  InputManager* mInput;

  bool mAlive;
  bool mAnimating;
  bool mWasHit;
  bool mThisPlayer;

  AnimatedTexture* mBase;
  AnimatedTexture* mWeapon;
  AnimatedTexture* mDeathAnimation;

  float mMoveSpeed;
  Vector2 mMoveBoundsX;
  Vector2 mMoveBoundsY;

  DIRECTION mDirection;
  DIRECTION mFireDirection;

  static const int MAX_BULLETS = 5;
  Bullet* mBullets[MAX_BULLETS];

  Base* mMainHouse;

 private:
  void HandleMovement();
  void HandleFiring();

 public:
  Player(bool isThisPlayer = true);
  ~Player();

  void Alive(bool alive);

  void Hit(PhysicEntity* other) override;
  bool WasHit();

  bool IsAnimating();

  void Shoot();

  void Update();
  void Render();
};

#endif
