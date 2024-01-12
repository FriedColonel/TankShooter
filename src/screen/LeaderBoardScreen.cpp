#include <headers/LeaderBoardScreen.h>

LeaderBoardScreen* LeaderBoardScreen::sInstance = NULL;

LeaderBoardScreen* LeaderBoardScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new LeaderBoardScreen();
  }
  return sInstance;
}

void LeaderBoardScreen::Release() {
  delete sInstance;
  sInstance = NULL;
}

LeaderBoardScreen::LeaderBoardScreen() : GameEntity() {
  mClient = TSS::Client::Instance();

  mIsPlayerTop5 = false;

  mTitle = new Texture("LEADERBOARD", "Font/ARCADE.TTF", 40, {150, 0, 0});
  mTitle->Parent(this);
  mTitle->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                      Graphics::Instance()->SCREEN_HEIGHT * 0.2f));

  mRankingsContainer = new GameEntity();
  mRankingsContainer->Parent(this);
  mRankingsContainer->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                                  Graphics::Instance()->SCREEN_HEIGHT * 0.4f));

  mThreeDot = new Texture("...", "Font/ARCADE.TTF", 16, {150, 0, 0});
  mThreeDot->Parent(mRankingsContainer);
  mThreeDot->Pos(Vector2(0.0f, 4 * 50.0f));

  for (int i = 0; i < 5; i++) {
    mRankings[i] = NULL;
  }
}

LeaderBoardScreen::~LeaderBoardScreen() {
  mClient = NULL;

  delete mTitle;
  mTitle = NULL;

  for (int i = 0; i < 5; i++) {
    if (mRankings[i] != NULL) {
      delete mRankings[i];
      mRankings[i] = NULL;
    }
  }

  delete mRankingsContainer;
  mRankingsContainer = NULL;
}

void LeaderBoardScreen::Update() {}

void LeaderBoardScreen::UpdateRankings() {
  if (mClient->leaderboard) {
    TSS::LinkedList* leaderboard = mClient->leaderboard;
    for (int i = 0; i < 5; i++) {
      if (mRankings[i] != NULL) {
        delete mRankings[i];
        mRankings[i] = NULL;
      }
    }

    int count = 0;

    for (int i = 0; count < 5 && i < mClient->leaderboard->length; i++) {
      TSS::TopUser* player = (TSS::TopUser*)mClient->leaderboard->retrieve(i);

      if (player->username == mClient->get_username()) {
        if (i < 5) mIsPlayerTop5 = true;
      }

      if (i > 3 && !mIsPlayerTop5 &&
          player->username != mClient->get_username())
        continue;

      mRankings[count] =
          new LeaderBoardPlayer(player->username, player->points, i,
                                player->username == mClient->get_username());
      mRankings[count]->Parent(mRankingsContainer);
      mRankings[count]->Pos(Vector2(0.0f, count * 50.0f));

      count++;
    }

    if (!mIsPlayerTop5) {
      mRankings[4]->Pos(Vector2(0.0f, 5 * 50.0f));
    }
  }
}

void LeaderBoardScreen::Render() {
  mTitle->Render();

  for (int i = 0; i < 4; i++) {
    if (mRankings[i] != NULL) mRankings[i]->Render();
  }

  if (!mIsPlayerTop5) {
    mThreeDot->Render();
    if (mRankings[4] != NULL) {
      mRankings[4]->Render();
    };
  } else {
    if (mRankings[4] != NULL) mRankings[4]->Render();
  }
}

// =============LEADERBOARDPLAYER================
LeaderBoardPlayer::LeaderBoardPlayer(std::string name, int score, int rank,
                                     bool isThisPlayer)
    : PlayerRanking(name, score, COLOR::red, isThisPlayer) {
  std::string rank_str = rank == 0   ? "1ST"
                         : rank == 1 ? "2ND"
                         : rank == 2 ? "3RD"
                                     : std::to_string(rank + 1) + "TH";

  mRankText = new Texture(rank_str, "Font/ARCADE.TTF", 16, {150, 0, 0});
  mRankText->Parent(this);
  mRankText->Pos(Vector2(150.0f, 0.0f));

  mScoreText->Pos(Vector2(75.0f, 0.0f));

  mTank->Pos(Vector2(-150.0f, 0.0f));
}
