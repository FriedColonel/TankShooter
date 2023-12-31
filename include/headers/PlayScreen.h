#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Timer.h>
#include <headers/Player.h>

using namespace QuickSDL;

class PlayScreen : public GameEntity {
 private:
  Timer* mTimer;

  Player* mPlayer;
  Player* mEnemy;

 public:
  void Update();
  void Render();

  void StartNewGame();

 public:
  PlayScreen();
  ~PlayScreen();
};

#endif
