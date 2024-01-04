#include <headers/LobbyScreen.h>

LobbyScreen::LobbyScreen() {
  Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, 0.0f));

  mTitle = new Texture("LOBBY", "Font/ARCADE.TTF", 80, {150, 0, 0});
  mTitle->Parent(this);
  mTitle->Pos(Vector2(0.0f, 70.0f));

  mInstruction = new Texture("Press SPACE to toggle ready", "Font/ARCADE.TTF",
                             50, {150, 0, 0});
  mInstruction->Parent(mTitle);
  mInstruction->Pos(Vector2(0.0f, 100.0f));

  mRoomCode = NULL;

  for (int i = 0; i < 4; i++) {
    mPlayerInfo[i] =
        new PlayerInfo("Huy123", GameEntity::COLOR::blue, false, i);
  }
}

LobbyScreen::~LobbyScreen() {
  delete mTitle;
  mTitle = NULL;

  delete mRoomCode;
  mRoomCode = NULL;

  for (int i = 0; i < 4; i++) {
    delete mPlayerInfo[i];
    mPlayerInfo[i] = NULL;
  }
}

void LobbyScreen::Update() {}

void LobbyScreen::Render() {
  mTitle->Render();
  mInstruction->Render();

  if (mRoomCode != NULL) mRoomCode->Render();

  for (int i = 0; i < 4; i++) {
    if (mPlayerInfo[i] != NULL) mPlayerInfo[i]->Render();
  }
}

void LobbyScreen::SetRoomCode(std::string code) {
  if (mRoomCode != NULL) {
    delete mRoomCode;
    mRoomCode = NULL;
  }

  mRoomCode =
      new Texture("Room code: " + code, "Font/ARCADE.TTF", 40, {230, 230, 230});
  mRoomCode->Parent(this);
  mRoomCode->Pos(Vector2(0.0f, 300.0f));
}
