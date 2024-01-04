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
  std::string name;
  GameEntity::COLOR color;
  bool isThisPlayer;

  Texture* mPlayerName;
  AnimatedTexture* mBase;
  AnimatedTexture* mWeapon;

  bool mIsReady;

 public:
  PlayerInfo(std::string _name, GameEntity::COLOR _color, bool isThisPlayer,
             int index)
      : name(_name), color(_color), isThisPlayer(isThisPlayer) {
    Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                500.0f + index * 80.0f));

    mPlayerName = new Texture(name, "Font/ARCADE.TTF", 40, {230, 230, 230});
    mPlayerName->Parent(this);
    mPlayerName->Pos(Vector2(-150.0f, 0.0f));

    mIsReady = false;

    mBase = new AnimatedTexture(
        "Tank/" + colorMap.at(color) + "/Bodies/body_halftrack.png", 0, 0,
        Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 2,
        0.2f, AnimatedTexture::horizontal);
    mBase->Parent(this);
    mBase->Pos(Vector2(150.0f, 0.0f));
    mBase->Scale(Vector2(0.7f, 0.7f));

    mWeapon = new AnimatedTexture(
        "Tank/" + colorMap.at(color) + "/Weapons/turret_01_mk1.png", 0, 0,
        Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 8,
        0.7f, AnimatedTexture::horizontal);
    mWeapon->WrapMode(AnimatedTexture::once);
    mWeapon->Parent(this);
    mWeapon->Pos(Vector2(150.0f, 0.0f));
    mWeapon->Scale(Vector2(0.7f, 0.7f));
  }

  ~PlayerInfo() {
    delete mPlayerName;
    mPlayerName = NULL;

    delete mBase;
    mBase = NULL;

    delete mWeapon;
    mWeapon = NULL;
  }

  void Render() {
    mPlayerName->Render();
    mBase->Render();
    mWeapon->Render();
  }
};

class LobbyScreen : public GameEntity {
 private:
  Texture* mTitle;
  Texture* mInstruction;
  Texture* mRoomCode;

  TSS::Client* mClient;

  struct PlayerInfo* mPlayerInfo[4];

 public:
  void Update();
  void Render();

  void SetRoomCode(std::string code);

 public:
  LobbyScreen();
  ~LobbyScreen();
};

#endif
