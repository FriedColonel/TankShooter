#ifndef _STARTSCREEN_H
#define _STARTSCREEN_H

#include <QuickSDL/AnimatedTexture.h>
#include <QuickSDL/InputManager.h>

using namespace QuickSDL;

class StartScreen : public GameEntity {
 private:
  Timer* mTimer;
  InputManager* mInput;

  // top bar
  GameEntity* mTopBar;
  Texture* mPlayerOne;
  Texture* mPlayerTwo;
  Texture* mHighScore;

  // play mode
  GameEntity* mPlayMode;
  Texture* mNewGameMode;
  Texture* mConnectGameMode;
  Texture* mCursor;
  Vector2 mCursorStartPos;
  Vector2 mCursorOffset;
  int mSelectedMode;

  // game title
  GameEntity* mGameTitle;
  Texture* mTitle;

 public:
  StartScreen();
  ~StartScreen();

  int SelectedMode();

  void ChangeMode(int mode);
  void ChangeScore(int score, int player);

  void Update();
  void Render();
};

#endif
