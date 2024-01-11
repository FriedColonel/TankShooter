#include <headers/ResultScreen.h>

ResultScreen* ResultScreen::sInstance = NULL;

ResultScreen* ResultScreen::Instance() {
  if (sInstance == NULL) sInstance = new ResultScreen();

  return sInstance;
}

void ResultScreen::Release() {
  delete sInstance;
  sInstance = NULL;
}

ResultScreen::ResultScreen() : GameEntity() {
  mClient = TSS::Client::Instance();

  mTitle = new Texture("RESULT", "Font/ARCADE.TTF", 50, {255, 0, 0});
  mTitle->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                      Graphics::Instance()->SCREEN_HEIGHT * 0.2f));
  mTitle->Parent(this);

  mPlayerRankingContainer = new GameEntity();
  mPlayerRankingContainer->Parent(this);
  mPlayerRankingContainer->Pos(
      Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
              Graphics::Instance()->SCREEN_HEIGHT * 0.4f));

  for (int i = 0; i < 4; i++) {
    mPlayerRanking[i] = NULL;
  }
}

ResultScreen::~ResultScreen() {
  mClient = NULL;

  delete mTitle;
  mTitle = NULL;
}

void ResultScreen::Update() {}

void ResultScreen::Render() {
  mTitle->Render();

  for (int i = 0; i < 4; i++) {
    if (mPlayerRanking[i] != NULL) mPlayerRanking[i]->Render();
  }
}

void ResultScreen::UpdatePlayerRanking() {
  TSS::Room* room = mClient->get_current_room();

  if (!room) return;

  for (int i = 0; i < room->players.size(); i++) {
    if (mPlayerRanking[i] != NULL) {
      delete mPlayerRanking[i];
      mPlayerRanking[i] = NULL;
    }

    mPlayerRanking[i] =
        new PlayerRanking(room->players[i].username, room->players[i].points,
                          static_cast<COLOR>(room->players[i].tank));
    mPlayerRanking[i]->Parent(mPlayerRankingContainer);
    mPlayerRanking[i]->Pos(Vector2(0.0f, i * 100));
  }
}

// ============PlayerRanking================
PlayerRanking::PlayerRanking(std::string name, int score, COLOR color)
    : GameEntity() {
  mTank = new GameEntity();
  mTank->Parent(this);

  mBase = new AnimatedTexture(
      "Tank/" + colorMap.at(color) + "/Bodies/body_halftrack.png", 0, 0,
      Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 2, 0.2f,
      AnimatedTexture::horizontal);
  mBase->Parent(mTank);
  mBase->Pos(Vector2(-250.0f, 0.0f));
  mBase->Scale(Vector2(0.7f, 0.7f));

  mWeapon = new AnimatedTexture(
      "Tank/" + colorMap.at(color) + "/Weapons/turret_01_mk1.png", 0, 0,
      Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 8, 0.7f,
      AnimatedTexture::horizontal);
  mWeapon->WrapMode(AnimatedTexture::once);
  mWeapon->Parent(mTank);
  mWeapon->Pos(Vector2(-250.0f, 0.0f));
  mWeapon->Scale(Vector2(0.7f, 0.7f));

  mNameText = new Texture(name, "Font/ARCADE.TTF", 32, {255, 255, 255});
  mNameText->Parent(this);
  mNameText->Pos(Vector2(-50.0f, 0.0f));

  mScoreText = new Texture(std::to_string(score), "Font/ARCADE.TTF", 32,
                           {255, 255, 255});
  mScoreText->Parent(this);
  mScoreText->Pos(Vector2(100.0f, 0.0f));

  std::string rankText = score == 10  ? "1st"
                         : score == 5 ? "2nd"
                         : score == 1 ? "3rd"
                                      : "4th";

  mRankText = new Texture(rankText, "Font/ARCADE.TTF", 32, {255, 255, 255});
  mRankText->Parent(this);
  mRankText->Pos(Vector2(250.0f, 0.0f));
}

PlayerRanking::~PlayerRanking() {
  delete mBase;
  mBase = NULL;

  delete mWeapon;
  mWeapon = NULL;

  delete mTank;
  mTank = NULL;

  delete mNameText;
  mNameText = NULL;

  delete mScoreText;
  mScoreText = NULL;

  delete mRankText;
  mRankText = NULL;
}

void PlayerRanking::Update() {}

void PlayerRanking::Render() {
  mBase->Render();
  mWeapon->Render();
  mNameText->Render();
  mScoreText->Render();
  mRankText->Render();
}
