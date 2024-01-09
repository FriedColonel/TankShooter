#ifndef _JOINSCREEN_H
#define _JOINSCREEN_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/GameEntity.h>
#include <QuickSDL/InputManager.h>

#include <socket/Client.hpp>
#include <string>

using namespace QuickSDL;

class RoomInfo : public GameEntity {
 public:
  enum Status { WAITING, FULL, STARTED };

 private:
  std::string mRoomCode;
  int mCount;
  int mStatus;

  GameEntity* mBox;
  Texture* mCode;
  Texture* mPlayerCount;
  Texture* mStatusText;
  AnimatedTexture* mBase;
  AnimatedTexture* mWeapon;
  Texture* mStatusTexture;

 public:
  RoomInfo(std::string code, int playerCount, Status status);
  ~RoomInfo();

  std::string RoomCode() { return mRoomCode; }

  void Render();
  void Update();
};

class JoinScreen : public GameEntity {
 private:
  static JoinScreen* sInstance;

  InputManager* mInput;
  TSS::Client* mClient;
  TSS::LinkedList* rooms;

  // title bar
  GameEntity* mTitleBar;
  Texture* mTitle;

  // input box
  GameEntity* mInputBox;
  Texture* mInputBoxLabel;
  Texture* mInputBoxTexture;

  std::string mInputText;

  // room list
  RoomInfo* mRoomList[5];
  Texture* mCursor;

  int mSelectedRoom;
  int mRoomCount;

  void UpdateInputText(char c);

 public:
  static JoinScreen* Instance();
  static void Release();
  JoinScreen();
  ~JoinScreen();

  std::string InputText();

  void ChangeRoom(int change);
  std::string GetSelectedRoom();

  void Update();
  void UpdateRoomList();

  void Render();
};

#endif
