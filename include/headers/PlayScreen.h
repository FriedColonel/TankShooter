#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Timer.h>
#include <headers/GameMap.h>
#include <headers/Player.h>

#include <socket/Client.hpp>

using namespace QuickSDL;

class PlayScreen : public GameEntity {
 private:
  static PlayScreen* sInstance;

  Timer* mTimer;
  TSS::Client* mClient;

  Player* mPlayer[4];
  std::map<std::string, Player*> mPlayers;

  GameMap* mGameMap;

  bool mGameOver;
  Texture* mGameOverText1;
  Texture* mGameOverText2;

 public:
  static PlayScreen* Instance();
  static void Release();
  void Update();
  void Render();

  void StartNewGame();

  void SetPlayerPosition(std::string username, Vector2 pos, bool moving,
                         GameEntity::DIRECTION dir);
  void Shoot(std::string username, Vector2 pos, GameEntity::DIRECTION dir);
  void PlayerDead(std::string username);

 private:
  PlayScreen();
  ~PlayScreen();
};

#endif
