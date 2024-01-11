#include <headers/StartScreen.h>

StartScreen* StartScreen::sInstance = NULL;

StartScreen* StartScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new StartScreen();
  }
  return sInstance;
}

void StartScreen::Release() {
  delete sInstance;
  sInstance = NULL;
}

StartScreen::StartScreen() {
  mTimer = Timer::Instance();
  mInput = InputManager::Instance();

  // Top bar entities
  mTopBar =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, 80.0f));
  mPlayerOne = new Texture("1UP", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mHighScore = new Texture("HIGH SCORE", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mPlayerTwo = new Texture("2UP", "Font/ARCADE.TTF", 32, {150, 0, 0});

  mPlayerOne->Parent(mTopBar);
  mHighScore->Parent(mTopBar);
  mPlayerTwo->Parent(mTopBar);

  mPlayerOne->Pos(Vector2(-Graphics::Instance()->SCREEN_WIDTH * 0.35f, 0.0f));
  mHighScore->Pos(Vector2(-32.0f, 0.0f));
  mPlayerTwo->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.2f, 0.0f));

  mTopBar->Parent(this);

  // play mode entities
  mPlayMode =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.55f));

  mTrainingMode =
      new Texture("TRAINING", "Font/ARCADE.TTF", 32, {230, 230, 230});
  mNewGameMode =
      new Texture("NEW ROOM", "Font/ARCADE.TTF", 32, {230, 230, 230});
  mConnectGameMode =
      new Texture("JOIN ROOM", "Font/ARCADE.TTF", 32, {230, 230, 230});
  mCursor = new Texture("Cursor/cursor.png");

  mTrainingMode->Parent(mPlayMode);
  mNewGameMode->Parent(mPlayMode);
  mConnectGameMode->Parent(mPlayMode);
  mCursor->Parent(mPlayMode);

  mTrainingMode->Pos(Vector2(0.0f, -64.0f));
  mNewGameMode->Pos(VEC2_ZERO);
  mConnectGameMode->Pos(Vector2(0.0f, 64.0f));
  mCursor->Pos(Vector2(-200.0f, -64.0f));

  mPlayMode->Parent(this);

  mCursorStartPos = mCursor->Pos(local);
  mCursorOffset = Vector2(0.0f, 64.0f);
  mSelectedMode = 0;

  // game title
  mGameTitle =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.3f));

  mTitle = new Texture("TANK SHOOTER", "Font/ARCADE.TTF", 64, {150, 0, 0});

  mTitle->Parent(mGameTitle);
  mTitle->Pos(VEC2_ZERO);

  mGameTitle->Parent(this);
}

StartScreen::~StartScreen() {
  // Free Top bar entities
  delete mTopBar;
  mTopBar = NULL;

  delete mPlayerOne;
  mPlayerOne = NULL;

  delete mHighScore;
  mHighScore = NULL;

  delete mPlayerTwo;
  mPlayerTwo = NULL;

  // Free play mode entities
  delete mPlayMode;
  mPlayMode = NULL;

  delete mTrainingMode;
  mTrainingMode = NULL;

  delete mNewGameMode;
  mNewGameMode = NULL;

  delete mConnectGameMode;
  mConnectGameMode = NULL;

  delete mCursor;
  mCursor = NULL;

  // Free game title
  delete mGameTitle;
  mGameTitle = NULL;
}

int StartScreen::SelectedMode() { return mSelectedMode; }

void StartScreen::Update() {
  if (mInput->KeyPressed(SDL_SCANCODE_W) ||
      mInput->KeyPressed(SDL_SCANCODE_UP)) {
    ChangeMode(-1);
  } else if (mInput->KeyPressed(SDL_SCANCODE_S) ||
             mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
    ChangeMode(1);
  }
}

void StartScreen::Render() {
  mPlayerOne->Render();
  mHighScore->Render();
  mPlayerTwo->Render();

  mTrainingMode->Render();
  mNewGameMode->Render();
  mConnectGameMode->Render();
  mCursor->Render();

  mTitle->Render();
}

void StartScreen::ChangeMode(int mode) {
  mSelectedMode += mode;

  if (mSelectedMode < 0) {
    mSelectedMode = 0;
  } else if (mSelectedMode > 2) {
    mSelectedMode = 2;
  }

  mCursor->Pos(mCursorStartPos + mCursorOffset * mSelectedMode);
}

void StartScreen::ChangeScore(int score, int player) {}
