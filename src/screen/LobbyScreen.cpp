#include <headers/LobbyScreen.h>

LobbyScreen* LobbyScreen::sInstance = NULL;

LobbyScreen* LobbyScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new LobbyScreen();
  }
  return sInstance;
}

void LobbyScreen::Release() {
  delete sInstance;
  sInstance = NULL;
}

LobbyScreen::LobbyScreen() {
  Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, 0.0f));
  mInput = InputManager::Instance();

  mClient = TSS::Client::Instance();

  mTitle = new Texture("LOBBY", "Font/ARCADE.TTF", 40, {150, 0, 0});
  mTitle->Parent(this);
  mTitle->Pos(Vector2(0.0f, 35.0f));

  mInstruction = new Texture("Press SPACE to toggle ready", "Font/ARCADE.TTF",
                             25, {150, 0, 0});
  mInstruction->Parent(mTitle);
  mInstruction->Pos(Vector2(0.0f, 50.0f));

  mRoomCode = NULL;

  for (int i = 0; i < 4; i++) {
    mPlayerInfo[i] = NULL;
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

void LobbyScreen::Update() {
  if (mClient->get_current_room()) {
    SetRoomCode(mClient->get_current_room()->room_id);

    for (int i = 0; i < 4; i++)
      if (mPlayerInfo[i] != NULL) {
        delete mPlayerInfo[i];
        mPlayerInfo[i] = NULL;
      }

    for (int i = 0; i < mClient->get_current_room()->players.size(); i++) {
      mPlayerInfo[i] = new PlayerInfo(
          mClient->get_current_room()->players[i].username,
          static_cast<COLOR>(mClient->get_current_room()->players[i].tank),
          mClient->get_current_room()->players[i].username ==
              mClient->get_username(),
          i, !!mClient->get_current_room()->players[i].status,
          mClient->get_current_room()->players[i].is_leader);
    }
  }

  if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
    for (int i = 0; i < 4; i++) {
      if (mPlayerInfo[i] != NULL && mPlayerInfo[i]->isThisPlayer) {
        if (mPlayerInfo[i]->mIsReady) {
          mClient->unready();
        } else {
          mClient->ready();
        }
      }
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
      new Texture("Room code: " + code, "Font/ARCADE.TTF", 20, {230, 230, 230});
  mRoomCode->Parent(this);
  mRoomCode->Pos(Vector2(0.0f, 150.0f));
}
