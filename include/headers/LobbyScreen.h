#ifndef _LOBBYSCREEN_H
#define _LOBBYSCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Graphics.h>
#include <QuickSDL/Texture.h>
#include <headers/Tank.h>

#include <socket/Client.hpp>
#include <string>

using namespace QuickSDL;

class PlayerInfo : public GameEntity {
 public:
  std::string name;
  GameEntity::COLOR color;
  bool isThisPlayer;
  bool mIsReady;

 private:
  Texture* mPlayerName;
  AnimatedTexture* mBase;
  AnimatedTexture* mWeapon;
  Texture* mReady;
  Texture* mCrown;

  bool mIsLeader;

 public:
  PlayerInfo(std::string _name, GameEntity::COLOR _color, bool isThisPlayer,
             int index, bool isReady = false, bool isLeader = false)
      : name(_name), color(_color), isThisPlayer(isThisPlayer) {
    Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                250.0f + index * 40.0f));

    if (isThisPlayer) {
      mPlayerName = new Texture(name, "Font/ARCADE.TTF", 20, {150, 0, 0});
    } else {
      mPlayerName = new Texture(name, "Font/ARCADE.TTF", 20, {230, 230, 230});
    }
    mPlayerName->Parent(this);
    mPlayerName->Pos(Vector2(-75.0f, 0.0f));

    mIsReady = isReady;
    mIsLeader = isLeader;

    mBase = new AnimatedTexture(
        "Tank/" + colorMap.at(color) + "/Bodies/body_halftrack.png", 0, 0,
        Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 2,
        0.2f, AnimatedTexture::horizontal);
    mBase->Parent(this);
    mBase->Pos(Vector2(75.0f, 0.0f));
    mBase->Scale(Vector2(0.35f, 0.35f));

    mWeapon = new AnimatedTexture(
        "Tank/" + colorMap.at(color) + "/Weapons/turret_01_mk1.png", 0, 0,
        Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 8,
        0.7f, AnimatedTexture::horizontal);
    mWeapon->WrapMode(AnimatedTexture::once);
    mWeapon->Parent(this);
    mWeapon->Pos(Vector2(75.0f, 0.0f));
    mWeapon->Scale(Vector2(0.35f, 0.35f));

    mReady = new Texture("Check/check.jpg");
    mReady->Parent(this);
    mReady->Pos(Vector2(150.0f, 0.0f));
    mReady->Scale(Vector2(0.04f, 0.04f));

    mCrown = new Texture("Menu/queen-crown.png");
    mCrown->Parent(this);
    mCrown->Pos(Vector2(-150.0f, 0.0f));
    mCrown->Scale(Vector2(0.35f, 0.35f));
  }

  ~PlayerInfo() {
    delete mPlayerName;
    mPlayerName = NULL;

    delete mBase;
    mBase = NULL;

    delete mWeapon;
    mWeapon = NULL;
  }

  void ToggleReady() { mIsReady = !mIsReady; }

  void Render() {
    mPlayerName->Render();
    mBase->Render();
    mWeapon->Render();

    if (mIsReady) mReady->Render();

    if (mIsLeader) mCrown->Render();
  }
};

class LobbyScreen : public GameEntity {
 private:
  static LobbyScreen* sInstance;

  InputManager* mInput;

  Texture* mTitle;
  Texture* mInstruction;
  Texture* mRoomCode;

  TSS::Client* mClient;

  struct PlayerInfo* mPlayerInfo[4];

 public:
  static LobbyScreen* Instance();
  static void Release();
  void Update();
  void Render();

  void SetRoomCode(std::string code);

 public:
  LobbyScreen();
  ~LobbyScreen();
};

#endif
