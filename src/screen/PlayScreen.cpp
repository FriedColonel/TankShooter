#include <headers/PlayScreen.h>

PlayScreen::PlayScreen() {
  mTimer = Timer::Instance();

  mPlayer = NULL;
}

PlayScreen::~PlayScreen() {
  mTimer = NULL;

  delete mPlayer;
  mPlayer = NULL;
}

void PlayScreen::StartNewGame() {
  delete mPlayer;
  mPlayer = NULL;

  mPlayer = new Player();
  mPlayer->Parent(this);
  mPlayer->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                       Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

  mEnemy = new Player(false);
  mEnemy->Parent(this);
  mEnemy->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                      Graphics::Instance()->SCREEN_HEIGHT * 0.2f));
}

void PlayScreen::Update() {
  if (mPlayer != NULL) mPlayer->Update();
  if (mEnemy != NULL) mEnemy->Update();
}

void PlayScreen::Render() {
  mPlayer->Render();
  mEnemy->Render();
}
