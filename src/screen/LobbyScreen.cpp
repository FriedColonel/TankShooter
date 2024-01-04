#include <headers/LobbyScreen.h>

LobbyScreen::LobbyScreen() {
  Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, 0.0f));

  mClient = TSS::Client::Instance();

  mTitle = new Texture("LOBBY", "Font/ARCADE.TTF", 80, {150, 0, 0});
  mTitle->Parent(this);
  mTitle->Pos(Vector2(0.0f, 70.0f));

  mInstruction = new Texture("Press SPACE to toggle ready", "Font/ARCADE.TTF",
                             50, {150, 0, 0});
  mInstruction->Parent(mTitle);
  mInstruction->Pos(Vector2(0.0f, 100.0f));

  mRoomCode = NULL;

  TSS::Room* room = mClient->get_current_room();
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

void LobbyScreen::Update() {
  if (mClient->get_current_room()) {
    SetRoomCode(mClient->get_current_room()->room_id);

    for (int i = 0; i < mClient->get_current_room()->players.size(); i++) {
      printf("Test test\n");

      mPlayerInfo[i] = new PlayerInfo(
          mClient->get_current_room()->players[i].username,
          static_cast<COLOR>(mClient->get_current_room()->players[i].tank),
          mClient->get_current_room()->players[i].username ==
              mClient->get_username(),
          i);
    }
  }
}

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
