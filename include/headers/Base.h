#ifndef _BASE_H
#define _BASE_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Texture.h>
#include <QuickSDL/Timer.h>
#include <headers/Brick.h>

using namespace QuickSDL;

class Base : public GameEntity {
 private:
  Graphics* mGraphics;
  Timer* mTimer;

  Texture* mTexture;
  Brick* mWalls[3];

  BASE_POSITION mBasePosition;

 public:
  Base(BASE_POSITION basePos);
  ~Base();

  void Update();
  void Render();
};

#endif
