#ifndef _RESULTSCREEN_H
#define _RESULTSCREEN_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/GameEntity.h>

#include <socket/Client.hpp>

using namespace QuickSDL;

class PlayerRanking : public GameEntity {
 protected:
  GameEntity* mTank;
  Texture *mNameText, *mScoreText, *mRankText;
  AnimatedTexture *mBase, *mWeapon;

 public:
  PlayerRanking(std::string name, int score, COLOR color);
  ~PlayerRanking();

  void Update();
  void Render();
};

class ResultScreen : public GameEntity {
 private:
  static ResultScreen* sInstance;

  TSS::Client* mClient;

  Texture* mTitle;

  GameEntity* mPlayerRankingContainer;
  PlayerRanking* mPlayerRanking[4];

 public:
  static ResultScreen* Instance();
  static void Release();

  void Update();
  void Render();

  void UpdatePlayerRanking();

 private:
  ResultScreen();
  ~ResultScreen();
};

#endif
