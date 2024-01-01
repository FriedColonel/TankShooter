#include <headers/PlayScreen.h>

PlayScreen::PlayScreen() {
  mTimer = Timer::Instance();

  mGameMap = GameMap::Instance();

  mPlayer = NULL;
}

PlayScreen::~PlayScreen() {
  mTimer = NULL;

  mGameMap = NULL;
  GameMap::Release();

  delete mPlayer;
  mPlayer = NULL;
}

void PlayScreen::StartNewGame() {
  delete mPlayer;
  mPlayer = NULL;

  mPlayer = new Player();
  mPlayer->Parent(this);
}

void PlayScreen::Update() {
  mGameMap->Update();
  if (mPlayer != NULL) mPlayer->Update();
  // if (mEnemy != NULL) mEnemy->Update();
}

void PlayScreen::Render() {
  mGameMap->Render();
  mPlayer->Render();

  // mEnemy->Render();
}
