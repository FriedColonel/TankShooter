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
  mLeaderBoardScreen = LeaderBoardScreen::Instance();
  mJoinScreen = JoinScreen::Instance();
  mPlayScreen = PlayScreen::Instance();
  mLobbyScreen = LobbyScreen::Instance();
  mMapChooseScreen = MapChooseScreen::Instance();
  mResultScreen = ResultScreen::Instance();

  mGameMap = GameMap::Instance();
}

ScreenManager::~ScreenManager() {
  mInput = NULL;

  StartScreen::Release();
  mStartScreen = NULL;

  LeaderBoardScreen::Release();
  mLeaderBoardScreen = NULL;

  JoinScreen::Release();
  mJoinScreen = NULL;

  PlayScreen::Release();
  mPlayScreen = NULL;

  LobbyScreen::Release();
  mLobbyScreen = NULL;

  MapChooseScreen::Release();
  mMapChooseScreen = NULL;

  ResultScreen::Release();
  mResultScreen = NULL;
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
        } else if (mStartScreen->SelectedMode() == training) {
          mClient->is_training = true;
          mPlayScreen->StartNewGame(true);
          mCurrentScreen = play;
        } else if (mStartScreen->SelectedMode() == leaderBoard) {
          mClient->get_leaderboard();
          while (mClient->leaderboard == NULL)
            ;
          mLeaderBoardScreen->UpdateRankings();
          mCurrentScreen = leaderBoard;
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
        // check if this player is leader
        if (mClient->get_current_room()) {
          if (mClient->get_current_room()->players.size() > 1)
            for (int i = 0; i < mClient->get_current_room()->players.size();
                 i++) {
              if (mClient->get_current_room()->players[i].status != 1) break;

              if (mClient->get_current_room()->players[i].username ==
                      mClient->get_username() &&
                  mClient->get_current_room()->players[i].is_leader)
                mClient->start_game();
            }
        }
      }

      if (mClient->get_current_room()) {
        if (mClient->get_current_room()->status == 2) {
          mClient->is_training = false;
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
      if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        if (mPlayScreen->GameOver()) {
          if (!mClient->is_training) {
            mCurrentScreen = result;
            mResultScreen->UpdatePlayerRanking();
          } else {
            mCurrentScreen = start;
            mClient->leave_room();
          }
        }
      }
      break;

    case result:
      mResultScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
        mClient->leave_room();
      }
      break;

    case leaderBoard:
      mLeaderBoardScreen->Update();
      if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
        mCurrentScreen = start;
      }
      break;

    default:
      break;
  }
}

void ScreenManager::LateUpdate() {
  switch (mCurrentScreen) {
    case play:
      mPlayScreen->LateUpdate();
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

    case result:
      mResultScreen->Render();
      break;

    case leaderBoard:
      mLeaderBoardScreen->Render();
      break;

    default:
      break;
  }
}

void ScreenManager::SetCurrentScreen(SCREENS screen) {
  mCurrentScreen = screen;
}
