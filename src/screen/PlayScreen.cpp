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

  mGameOverText1 = new Texture("GAME OVER", "Font/ARCADE.TTF", 50, {255, 0, 0});
  // set position of mGameOverText1 to bottom center of the screen
  mGameOverText1->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
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

void PlayScreen::StartNewGame() {
  for (int i = 0; i < 4; i++) {
    delete mPlayer[i];
    mPlayer[i] = NULL;
  }

  for (int i = 0; i < mClient->get_current_room()->players.size(); i++) {
    mPlayer[i] = new Player(
        mClient->get_current_room()->players[i].username ==
            mClient->get_username(),
        static_cast<BASE_POSITION>(i),
        static_cast<COLOR>(mClient->get_current_room()->players[i].tank));
    mPlayer[i]->Parent(this);
    mPlayer[i]->Pos(Vector2(100 + i * 100, 100));

    mPlayers[mClient->get_current_room()->players[i].username] = mPlayer[i];
  }

  mGameMap->SetMapChoice(mClient->get_current_room()->map);
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

void PlayScreen::Update() {
  mGameMap->Update();

  for (int i = 0; i < 4; i++) {
    if (mPlayer[i] != NULL) {
      mPlayer[i]->Update();
    }
  }

  int alive = 0;

  for (int i = 0; i < 4; i++) {
    if (mPlayer[i] != NULL) {
      if (mPlayer[i]->IsThisPlayer() && !mPlayer[i]->Alive()) {
        mGameOver = true;
        break;
      }

      if (mPlayer[i]->Alive()) alive++;
    }
  }

  if (alive == 1) mGameOver = true;

  if (mGameOver && mGameOverText1->Pos(world).y >
                       Graphics::Instance()->SCREEN_HEIGHT * 0.5f) {
    // game over text scrolling up from bottom of the screen
    mGameOverText1->Translate(VEC2_UP * 100 * mTimer->DeltaTime(), world);
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
    mGameOverText1->Render();
    mGameOverText2->Render();
  }
}
