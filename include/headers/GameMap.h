#ifndef _GAMEMAP_H
#define _GAMEMAP_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Texture.h>
#include <headers/Brick.h>

#include <socket/Client.hpp>
#include <utility>
#include <vector>

using namespace QuickSDL;

class GameMap : public GameEntity {
 private:
  static GameMap* sInstance;

  Graphics* mGraphics;

  int mMapChoice;

  Texture* mBackground;

  std::vector<Brick*> mBricks;

 public:
  static GameMap* Instance();
  static void Release();

  void Update();
  void Render();

  void SetMapChoice(int mapChoice);

  void Reset();

 private:
  GameMap(int mapChoice);
  ~GameMap();
};

#endif
