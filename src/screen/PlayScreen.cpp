#include <headers/PlayScreen.h>

PlayScreen* PlayScreen::sInstance = NULL;

PlayScreen* PlayScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new PlayScreen();
  }
  return sInstance;
}

void PlayScreen::Release() {
  delete sInstance;
  sInstance = NULL;
}

PlayScreen::PlayScreen() {
  mTimer = Timer::Instance();
  mClient = TSS::Client::Instance();
  mGameMap = GameMap::Instance();

  mGameOver = false;
  mYouWin = false;
  mGameOverSent = false;
  mIsTraining = false;

  mGameOverText1 = new Texture("GAME OVER", "Font/ARCADE.TTF", 50, {255, 0, 0});
  // set position of mGameOverText1 to bottom center of the screen
  mGameOverText1->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                              Graphics::Instance()->SCREEN_HEIGHT * 1.2f));

  mYouWinText = new Texture("YOU WIN", "Font/ARCADE.TTF", 50, {255, 0, 0});
  mYouWinText->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                           Graphics::Instance()->SCREEN_HEIGHT * 1.2f));

  mGameOverText2 = new Texture("PRESS ENTER TO CONTINUE", "Font/ARCADE.TTF", 32,
                               {150, 0, 0});
  mGameOverText2->Parent(mGameOverText1);
  mGameOverText2->Pos(Vector2(0.0f, 40.0f));

  for (int i = 0; i < 4; i++) {
    mPlayer[i] = NULL;
  }
}

PlayScreen::~PlayScreen() {
  mTimer = NULL;

  mGameMap = NULL;
  GameMap::Release();

  mClient = NULL;

  for (map<string, Player*>::iterator it = mPlayers.begin();
       it != mPlayers.end(); it++) {
    delete it->second;
    it->second = NULL;
  }

  mPlayers.clear();

  delete mGameOverText1;
  mGameOverText1 = NULL;

  delete mGameOverText2;
  mGameOverText2 = NULL;
}

void PlayScreen::StartNewGame(bool isTraining) {
  mIsTraining = isTraining;

  for (int i = 0; i < 4; i++) {
    delete mPlayer[i];
    mPlayer[i] = NULL;
  }

  mPlayers.clear();

  if (mIsTraining) {
    mPlayer[0] = new Player(true, BASE_POSITION::bottomLeft, COLOR::red);
    mPlayers[mClient->get_username()] = mPlayer[0];

    for (int i = 1; i < 4; i++)
      mPlayer[i] = new Player(false, static_cast<BASE_POSITION>(i),
                              static_cast<COLOR>(i + 1), true);

    mGameMap->SetMapChoice(1);
  } else {
    for (int i = 0; i < mClient->get_current_room()->players.size(); i++) {
      mPlayer[i] = new Player(
          mClient->get_current_room()->players[i].username ==
              mClient->get_username(),
          static_cast<BASE_POSITION>(i),
          static_cast<COLOR>(mClient->get_current_room()->players[i].tank));

      mPlayers[mClient->get_current_room()->players[i].username] = mPlayer[i];
    }

    mGameMap->SetMapChoice(mClient->get_current_room()->map);
  }

  mGameOver = false;
  mYouWin = false;
  mGameOverSent = false;

  mGameOverText1->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                              Graphics::Instance()->SCREEN_HEIGHT * 1.2f));
  mYouWinText->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                           Graphics::Instance()->SCREEN_HEIGHT * 1.2f));
}

void PlayScreen::SetPlayerPosition(std::string username, Vector2 pos,
                                   bool moving, GameEntity::DIRECTION dir) {
  mPlayers[username]->ChangePosition(pos, dir, moving);
}

void PlayScreen::Shoot(std::string username, Vector2 pos,
                       GameEntity::DIRECTION dir) {
  mPlayers[username]->Shoot(pos, dir);
}

void PlayScreen::PlayerDead(std::string username) {
  mPlayers[username]->Dead();
}

bool PlayScreen::GameOver() { return mGameOver; }

void PlayScreen::Update() {
  mGameMap->Update();

  for (int i = 0; i < 4; i++) {
    if (mPlayer[i] != NULL) {
      mPlayer[i]->Update();
    }
  }

  int alive = 0;
  int thisPlayer = 0;

  if (!mPlayers[mClient->get_username()]->Alive()) {
    mGameOver = true;
  }

  if (mClient->alive_players == 1 && !mGameOverSent) {
    mGameOver = true;
    if (mPlayers[mClient->get_username()]->Alive()) {
      mYouWin = true;
      mClient->game_end();
      mGameOverSent = true;
    }
  }

  if (mGameOver) {
    if (mYouWin) {
      if (mYouWinText->Pos(world).y >
          Graphics::Instance()->SCREEN_HEIGHT * 0.5f) {
        mYouWinText->Translate(VEC2_UP * 100 * mTimer->DeltaTime(), world);
        mGameOverText2->Parent(mYouWinText);
      }
    } else {
      if (mGameOverText1->Pos(world).y >
          Graphics::Instance()->SCREEN_HEIGHT * 0.5f) {
        // game over text scrolling up from bottom of the screen
        mGameOverText1->Translate(VEC2_UP * 100 * mTimer->DeltaTime(), world);
        mGameOverText2->Parent(mGameOverText1);
      }
    }
  }
}

void PlayScreen::LateUpdate() {
  for (int i = 0; i < 4; i++) {
    if (mPlayer[i] != NULL) {
      mPlayer[i]->LateUpdate();
    }
  }
}

void PlayScreen::Render() {
  mGameMap->Render();

  for (int i = 0; i < 4; i++) {
    if (mPlayer[i] != NULL) {
      mPlayer[i]->Render();
    }
  }

  if (mGameOver) {
    if (mYouWin)
      mYouWinText->Render();
    else
      mGameOverText1->Render();

    mGameOverText2->Render();
  }
}
