#ifndef _GAMEMAP_H
#define _GAMEMAP_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Texture.h>
#include <headers/Brick.h>

#include <utility>
#include <vector>

using namespace QuickSDL;

class GameMap : public GameEntity {
 private:
  static GameMap* sInstance;

  Graphics* mGraphics;

  std::vector<std::pair<int, int>> mBricksPos = {
      std::pair<int, int>(0, 13),  std::pair<int, int>(0, 14),
      std::pair<int, int>(0, 15),  std::pair<int, int>(0, 16),
      std::pair<int, int>(1, 5),   std::pair<int, int>(1, 6),
      std::pair<int, int>(1, 13),  std::pair<int, int>(1, 14),
      std::pair<int, int>(1, 15),  std::pair<int, int>(1, 16),
      std::pair<int, int>(1, 23),  std::pair<int, int>(1, 24),
      std::pair<int, int>(2, 5),   std::pair<int, int>(2, 6),
      std::pair<int, int>(2, 9),   std::pair<int, int>(2, 10),
      std::pair<int, int>(2, 13),  std::pair<int, int>(2, 14),
      std::pair<int, int>(2, 15),  std::pair<int, int>(2, 16),
      std::pair<int, int>(2, 19),  std::pair<int, int>(2, 20),
      std::pair<int, int>(2, 23),  std::pair<int, int>(2, 24),
      std::pair<int, int>(3, 1),   std::pair<int, int>(3, 2),
      std::pair<int, int>(3, 5),   std::pair<int, int>(3, 6),
      std::pair<int, int>(3, 23),  std::pair<int, int>(3, 24),
      std::pair<int, int>(3, 27),  std::pair<int, int>(3, 28),
      std::pair<int, int>(4, 1),   std::pair<int, int>(4, 2),
      std::pair<int, int>(4, 5),   std::pair<int, int>(4, 6),
      std::pair<int, int>(4, 23),  std::pair<int, int>(4, 24),
      std::pair<int, int>(4, 27),  std::pair<int, int>(4, 28),
      std::pair<int, int>(5, 1),   std::pair<int, int>(5, 2),
      std::pair<int, int>(5, 11),  std::pair<int, int>(5, 12),
      std::pair<int, int>(5, 13),  std::pair<int, int>(5, 16),
      std::pair<int, int>(5, 17),  std::pair<int, int>(5, 18),
      std::pair<int, int>(5, 27),  std::pair<int, int>(5, 28),
      std::pair<int, int>(6, 1),   std::pair<int, int>(6, 2),
      std::pair<int, int>(6, 11),  std::pair<int, int>(6, 12),
      std::pair<int, int>(6, 13),  std::pair<int, int>(6, 16),
      std::pair<int, int>(6, 17),  std::pair<int, int>(6, 18),
      std::pair<int, int>(6, 27),  std::pair<int, int>(6, 28),
      std::pair<int, int>(7, 5),   std::pair<int, int>(7, 6),
      std::pair<int, int>(7, 7),   std::pair<int, int>(7, 22),
      std::pair<int, int>(7, 23),  std::pair<int, int>(7, 24),
      std::pair<int, int>(8, 1),   std::pair<int, int>(8, 2),
      std::pair<int, int>(8, 5),   std::pair<int, int>(8, 6),
      std::pair<int, int>(8, 7),   std::pair<int, int>(8, 22),
      std::pair<int, int>(8, 23),  std::pair<int, int>(8, 24),
      std::pair<int, int>(8, 27),  std::pair<int, int>(8, 28),
      std::pair<int, int>(9, 1),   std::pair<int, int>(9, 2),
      std::pair<int, int>(9, 13),  std::pair<int, int>(9, 16),
      std::pair<int, int>(9, 27),  std::pair<int, int>(9, 28),
      std::pair<int, int>(10, 1),  std::pair<int, int>(10, 2),
      std::pair<int, int>(10, 13), std::pair<int, int>(10, 14),
      std::pair<int, int>(10, 15), std::pair<int, int>(10, 16),
      std::pair<int, int>(10, 27), std::pair<int, int>(10, 28),
      std::pair<int, int>(11, 1),  std::pair<int, int>(11, 2),
      std::pair<int, int>(11, 5),  std::pair<int, int>(11, 6),
      std::pair<int, int>(11, 9),  std::pair<int, int>(11, 10),
      std::pair<int, int>(11, 13), std::pair<int, int>(11, 14),
      std::pair<int, int>(11, 15), std::pair<int, int>(11, 16),
      std::pair<int, int>(11, 19), std::pair<int, int>(11, 20),
      std::pair<int, int>(11, 23), std::pair<int, int>(11, 24),
      std::pair<int, int>(11, 27), std::pair<int, int>(11, 28),
      std::pair<int, int>(12, 1),  std::pair<int, int>(12, 2),
      std::pair<int, int>(12, 5),  std::pair<int, int>(12, 6),
      std::pair<int, int>(12, 9),  std::pair<int, int>(12, 10),
      std::pair<int, int>(12, 13), std::pair<int, int>(12, 16),
      std::pair<int, int>(12, 19), std::pair<int, int>(12, 20),
      std::pair<int, int>(12, 23), std::pair<int, int>(12, 24),
      std::pair<int, int>(12, 27), std::pair<int, int>(12, 28),
      std::pair<int, int>(13, 5),  std::pair<int, int>(13, 6),
      std::pair<int, int>(13, 9),  std::pair<int, int>(13, 10),
      std::pair<int, int>(13, 19), std::pair<int, int>(13, 20),
      std::pair<int, int>(13, 23), std::pair<int, int>(13, 24),
      std::pair<int, int>(14, 5),  std::pair<int, int>(14, 6),
      std::pair<int, int>(14, 23), std::pair<int, int>(14, 24),
      std::pair<int, int>(15, 13), std::pair<int, int>(15, 14),
      std::pair<int, int>(15, 15), std::pair<int, int>(15, 16),
  };
  std::vector<GameEntity*> mBricks;

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
