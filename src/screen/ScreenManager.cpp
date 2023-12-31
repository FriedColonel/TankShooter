#include <headers/ScreenManager.h>

#include <iostream>

ScreenManager* ScreenManager::sInstance = NULL;

ScreenManager* ScreenManager::Instance() {
  if (sInstance == NULL) sInstance = new ScreenManager();

  return sInstance;
}

void ScreenManager::Release() {
  delete sInstance;
  sInstance = NULL;
}

ScreenManager::ScreenManager() {
  mInput = InputManager::Instance();

  mStartScreen = new StartScreen();
  mCurrentScreen = start;

  mJoinScreen = new JoinScreen();
  mPlayScreen = new PlayScreen();
}

ScreenManager::~ScreenManager() {
  mInput = NULL;

  delete mStartScreen;
  mStartScreen = NULL;

  delete mJoinScreen;
  mJoinScreen = NULL;
}

void ScreenManager::Update() {
  switch (mCurrentScreen) {
    case start:
      mStartScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        if (mStartScreen->SelectedMode() == lobby) {
          mCurrentScreen = lobby;
        } else if (mStartScreen->SelectedMode() == join) {
          mCurrentScreen = join;
        }
      }
      break;

    case join:
      mJoinScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
      } else if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        mCurrentScreen = play;
        mPlayScreen->StartNewGame();
      }
      break;

    case play:
      mPlayScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
      }
      break;

    default:
      break;
  }
}

void ScreenManager::Render() {
  switch (mCurrentScreen) {
    case start:
      mStartScreen->Render();
      break;

    case join:
      mJoinScreen->Render();
      break;

    case play:
      mPlayScreen->Render();
      break;

    default:
      break;
  }
}
