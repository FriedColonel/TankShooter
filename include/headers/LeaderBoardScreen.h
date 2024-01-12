#ifndef _LEADERBOARDSCREEN_H_
#define _LEADERBOARDSCREEN_H_

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/GameEntity.h>
#include <headers/ResultScreen.h>

#include <socket/Client.hpp>

using namespace QuickSDL;

class LeaderBoardScreen : public GameEntity {
 private:
  static LeaderBoardScreen* sInstance;

  TSS::Client* mClient;

  GameEntity* mRankingsContainer;
  PlayerRanking* mRankings[5];

  Texture* mTitle;
  Texture* mThreeDot;

  bool mIsPlayerTop5;

 public:
  static LeaderBoardScreen* Instance();
  static void Release();

  void Update();
  void Render();

  void UpdateRankings();

 private:
  LeaderBoardScreen();
  ~LeaderBoardScreen();
};

class LeaderBoardPlayer : public PlayerRanking {
 public:
  LeaderBoardPlayer(std::string name, int score, int rank,
                    bool isThisPlayer = false);
};

#endif
