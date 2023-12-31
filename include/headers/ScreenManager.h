#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H

#include <QuickSDL/InputManager.h>
#include <headers/JoinScreen.h>
#include <headers/PlayScreen.h>
#include <headers/StartScreen.h>

class ScreenManager {
 private:
  enum SCREENS { start, join, lobby, play };

  static ScreenManager* sInstance;

  InputManager* mInput;

  StartScreen* mStartScreen;
  JoinScreen* mJoinScreen;
  PlayScreen* mPlayScreen;

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
