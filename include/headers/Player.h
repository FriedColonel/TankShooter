#ifndef _PLAYER_H
#define _PLAYER_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>
#include <QuickSDL/Timer.h>
#include <headers/Base.h>
#include <headers/Bullet.h>
#include <headers/PhysicEntity.h>
#include <headers/Tank.h>

using namespace QuickSDL;

class Player : public GameEntity {
 private:
  Timer* mTimer;
  InputManager* mInput;

  bool mAlive;
  bool mThisPlayer;

  Base* mBase;

 public:
  Player(bool isThisPlayer, BASE_POSITION basePos, COLOR color);
  ~Player();

  void Alive(bool alive);

  void Update();
  void Render();
};

#endif