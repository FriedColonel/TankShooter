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
  mClient = TSS::Client::Instance();

  mCurrentScreen = start;

  mStartScreen = StartScreen::Instance();
  mJoinScreen = JoinScreen::Instance();
  mPlayScreen = PlayScreen::Instance();
  mLobbyScreen = LobbyScreen::Instance();
  mMapChooseScreen = MapChooseScreen::Instance();
  mGameMap = GameMap::Instance();
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
        if (mStartScreen->SelectedMode() == mapChoose) {
          mCurrentScreen = mapChoose;
        } else if (mStartScreen->SelectedMode() == join) {
          mJoinScreen->UpdateRoomList();
          mCurrentScreen = join;
        }
      }
      break;

    case mapChoose:
      mMapChooseScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
      } else if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        mCurrentScreen = lobby;
        mClient->create_room(mMapChooseScreen->GetSelectedMap());
      }
      break;

    case lobby:
      mLobbyScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
        mClient->leave_room();
      } else if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        mClient->start_game();
      }

      if (mClient->get_current_room()) {
        if (mClient->get_current_room()->status == 2) {
          mCurrentScreen = play;
          mPlayScreen->StartNewGame();
        }
      }
      break;

    case join:
      mJoinScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
      } else if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        if (mJoinScreen->GetSelectedRoom() != "") {
          mCurrentScreen = lobby;
          mClient->join_room(mJoinScreen->GetSelectedRoom());
        }
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

    case mapChoose:
      mMapChooseScreen->Render();
      break;

    case lobby:
      mLobbyScreen->Render();
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

void ScreenManager::SetCurrentScreen(SCREENS screen) {
  mCurrentScreen = screen;
}
