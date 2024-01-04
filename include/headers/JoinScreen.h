#ifndef _JOINSCREEN_H
#define _JOINSCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>

#include <string>

using namespace QuickSDL;

class RoomInfo : GameEntity {
 private:
  std::string mRoomCode;
  int mCount;

  GameEntity* mBox;
  Texture* mCode;
  Texture* mPlayerCount;

 public:
  RoomInfo(std::string code, int playerCount);
  ~RoomInfo();

  void Render();
  void Update();
};

class JoinScreen : public GameEntity {
 private:
  InputManager* mInput;

  // title bar
  GameEntity* mTitleBar;
  Texture* mTitle;

  // input box
  GameEntity* mInputBox;
  Texture* mInputBoxTexture;

  std::string mInputText;
  void UpdateInputText(char c);

 public:
  JoinScreen();
  ~JoinScreen();

  std::string InputText();

  void Update();
  void Render();
};

#endif
