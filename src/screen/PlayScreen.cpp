#include <headers/PlayScreen.h>

PlayScreen* PlayScreen::sInstance = NULL;

PlayScreen* PlayScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new PlayScreen();
  }
  return sInstance;
}

PlayScreen::PlayScreen() {
  mTimer = Timer::Instance();
  mClient = TSS::Client::Instance();
  mGameMap = GameMap::Instance();

  for (int i = 0; i < 4; i++) {
    mPlayer[i] = NULL;
  }
}

PlayScreen::~PlayScreen() {
  mTimer = NULL;

  mGameMap = NULL;
  GameMap::Release();

  mClient = NULL;

  for (int i = 0; i < 4; i++) {
    delete mPlayer[i];
    mPlayer[i] = NULL;
  }
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

void PlayScreen::Update() {
  mGameMap->Update();

  for (int i = 0; i < 4; i++) {
    if (mPlayer[i] != NULL) {
      mPlayer[i]->Update();
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
}
