#ifndef _PLAYER_H
#define _PLAYER_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>
#include <QuickSDL/Timer.h>
#include <headers/Base.h>
#include <headers/Bullet.h>
#include <headers/PhysicEntity.h>

#include <socket/Client.hpp>

using namespace QuickSDL;

class Player : public GameEntity {
 private:
  Timer* mTimer;
  InputManager* mInput;
  TSS::Client* mClient;

  bool mAlive;
  bool mThisPlayer;
  bool mIsLeader;

  Base* mBase;

 public:
  Player(bool isThisPlayer, BASE_POSITION basePos, COLOR color,
         bool isBot = false, bool isLeader = false);
  ~Player();

  void Alive(bool alive);
  bool Alive();

  void ChangePosition(Vector2 pos, GameEntity::DIRECTION direction,
                      bool moving = true);
  void Shoot(Vector2 pos, GameEntity::DIRECTION direction);
  void Dead();

  bool IsThisPlayer();

  void Update();
  void LateUpdate();
  void Render();
};

#endif
