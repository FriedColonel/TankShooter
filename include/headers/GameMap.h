#ifndef _GAMEMAP_H
#define _GAMEMAP_H

#include <QuickSDL/GameEntity.h>

using namespace QuickSDL;

class GameMap : public GameEntity {
 private:
  static GameMap* sInstance;

 public:
  static GameMap* Instance();
  static void Release();

  void Update();
  void Render();

 private:
  GameMap();
  ~GameMap();
};

#endif
