#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H

#include <QuickSDL/InputManager.h>
#include <headers/JoinScreen.h>
#include <headers/LobbyScreen.h>
#include <headers/MapChooseScreen.h>
#include <headers/PlayScreen.h>
#include <headers/StartScreen.h>

#include <socket/Client.hpp>

class ScreenManager {
 private:
  enum SCREENS { mapChoose, join, lobby, start, play };

  static ScreenManager* sInstance;
  TSS::Client* mClient;

  InputManager* mInput;

  StartScreen* mStartScreen;
  LobbyScreen* mLobbyScreen;
  JoinScreen* mJoinScreen;
  PlayScreen* mPlayScreen;
  MapChooseScreen* mMapChooseScreen;
  GameMap* mGameMap;

  SCREENS mCurrentScreen;

 public:
  static ScreenManager* Instance();
  static void Release();

  void Update();
  void Render();

 private:
  ScreenManager();
  ~ScreenManager();
};

#endif
