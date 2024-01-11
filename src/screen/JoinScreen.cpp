#include <headers/JoinScreen.h>

#include <iostream>

JoinScreen* JoinScreen::sInstance = NULL;

JoinScreen* JoinScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new JoinScreen();
  }
  return sInstance;
}

void JoinScreen::Release() {
  delete sInstance;
  sInstance = NULL;
}

JoinScreen::JoinScreen() {
  mInput = InputManager::Instance();
  mClient = TSS::Client::Instance();

  mClient->get_rooms();

  rooms = mClient->rooms;

  // title bar
  mTitleBar =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.1f));
  mTitle = new Texture("ENTER ROOM CODE", "Font/ARCADE.TTF", 24, {150, 0, 0});
  mTitle->Parent(mTitleBar);
  mTitle->Pos(VEC2_ZERO);

  // input box
  mInputBox =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.3f));

  mInputText = " ";

  mInputBoxLabel =
      new Texture("ROOM CODE: ", "Font/ARCADE.TTF", 16, {255, 255, 255});
  mInputBoxLabel->Parent(mInputBox);
  mInputBoxLabel->Pos(Vector2(-100.0f, 0.0f));

  // input text
  mInputBoxTexture =
      new Texture(mInputText, "Font/ARCADE.TTF", 16, {255, 255, 255});
  mInputBoxTexture->Parent(mInputBox);
  mInputBoxTexture->Pos(Vector2(50.0f, 0.0f));

  for (int i = 0; i < 5; i++) {
    mRoomList[i] = NULL;
  }

  mCursor = new Texture("Cursor/cursor.png");
  mCursor->Parent(this);
  mCursor->Scale(Vector2(0.5f, 0.5f));

  mSelectedRoom = -1;

  mRoomCount = 0;
}

JoinScreen::~JoinScreen() {
  mInput = NULL;
  mClient = NULL;
  rooms = NULL;

  delete mTitle;
  mTitle = NULL;

  delete mInputBox;
  mInputBox = NULL;

  delete mInputBoxTexture;
  mInputBoxTexture = NULL;

  for (int i = 0; i < 5; i++) {
    delete mRoomList[i];
    mRoomList[i] = NULL;
  }

  delete mTitleBar;
  mTitleBar = NULL;
}

void JoinScreen::Update() {
  UpdateRoomList();

  // handle input number
  if (mInput->KeyPressed(SDL_SCANCODE_0)) {
    UpdateInputText('0');
  } else if (mInput->KeyPressed(SDL_SCANCODE_1)) {
    UpdateInputText('1');
  } else if (mInput->KeyPressed(SDL_SCANCODE_2)) {
    UpdateInputText('2');
  } else if (mInput->KeyPressed(SDL_SCANCODE_3)) {
    UpdateInputText('3');
  } else if (mInput->KeyPressed(SDL_SCANCODE_4)) {
    UpdateInputText('4');
  } else if (mInput->KeyPressed(SDL_SCANCODE_5)) {
    UpdateInputText('5');
  } else if (mInput->KeyPressed(SDL_SCANCODE_6)) {
    UpdateInputText('6');
  } else if (mInput->KeyPressed(SDL_SCANCODE_7)) {
    UpdateInputText('7');
  } else if (mInput->KeyPressed(SDL_SCANCODE_8)) {
    UpdateInputText('8');
  } else if (mInput->KeyPressed(SDL_SCANCODE_9)) {
    UpdateInputText('9');
  } else if (mInput->KeyPressed(SDL_SCANCODE_BACKSPACE)) {
    UpdateInputText(' ');
  } else if (mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
    ChangeRoom(1);
  } else if (mInput->KeyPressed(SDL_SCANCODE_UP)) {
    ChangeRoom(-1);
  } else if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
    if (mSelectedRoom != -1) {
      printf("Joining room: %s\n",
             mRoomList[mSelectedRoom]->RoomCode().c_str());
    } else {
      printf("Search room: %s\n", mInputText.c_str());
    }
  }

  if (mSelectedRoom != -1) mCursor->Parent(mRoomList[mSelectedRoom]);

  mCursor->Pos(Vector2(-200.0f, 0.0f));
}

void JoinScreen::UpdateRoomList() {
  rooms = mClient->rooms;

  if (rooms) {
    for (int i = 0; i < 5; i++) {
      if (mRoomList[i] != NULL) {
        // delete mRoomList[i];
        mRoomList[i] = NULL;
      }
    }

    int count = 0;

    // room list
    for (int i = 0; count < 5 && i < rooms->length; i++) {
      TSS::Room* room = (TSS::Room*)rooms->retrieve(i);

      if (mInputText != " ") {
        if (room->room_id.find(mInputText) == std::string::npos) continue;
      }

      RoomInfo::Status status;

      if (room->players.size() == 4) {
        status = RoomInfo::Status::FULL;
      } else if (room->status == 1) {
        status = RoomInfo::Status::WAITING;
      } else {
        status = RoomInfo::Status::STARTED;
      }

      delete mRoomList[count];

      mRoomList[count] =
          new RoomInfo(room->room_id, room->players.size(), status);
      mRoomList[count]->Parent(this);
      mRoomList[count]->Pos(Vector2(
          Graphics::Instance()->SCREEN_WIDTH * 0.5f,
          Graphics::Instance()->SCREEN_HEIGHT * 0.5f + (count * 50.0f)));

      count++;
    }

    mRoomCount = count;
  }
}

std::string JoinScreen::InputText() { return mInputText; }

void JoinScreen::ChangeRoom(int change) {
  mSelectedRoom += change;

  if (mSelectedRoom > mRoomCount - 1) {
    mSelectedRoom = mRoomCount - 1;
  } else if (mSelectedRoom < -1) {
    mSelectedRoom = -1;
  }
}

std::string JoinScreen::GetSelectedRoom() {
  if (mSelectedRoom == -1) {
    return "";
  } else {
    return mRoomList[mSelectedRoom]->RoomCode();
  }
}

void JoinScreen::UpdateInputText(char c) {
  if (c == ' ') {
    if (mInputText.length() > 0) {
      mInputText.pop_back();
    }

    if (mInputText.length() == 0) {
      mInputText = " ";
    }
  } else {
    // check if text have space or not
    if (mInputText == " ") {
      mInputText = "";
    }

    if (mInputText.length() < 5) {
      mInputText += c;
    }
  }

  if (mInputBoxTexture != NULL) {
    delete mInputBoxTexture;
    mInputBoxTexture = NULL;
  }

  mInputBoxTexture =
      new Texture(mInputText, "Font/ARCADE.TTF", 16, {255, 255, 255});
  mInputBoxTexture->Parent(mInputBox);
  mInputBoxTexture->Pos(Vector2(50.0f, 0.0f));
}

void JoinScreen::Render() {
  mTitleBar->Render();
  mTitle->Render();

  mInputBox->Render();
  mInputBoxLabel->Render();
  mInputBoxTexture->Render();

  for (int i = 0; i < 5; i++) {
    if (mRoomList[i] != NULL) {
      mRoomList[i]->Render();
    }
  }

  if (mSelectedRoom != -1) {
    mCursor->Render();
  }
}

// =================== ROOM INFO ===================
RoomInfo::RoomInfo(std::string code, int playerCount, Status status)
    : GameEntity() {
  mRoomCode = code;
  mCount = playerCount;
  mStatus = status;

  mCode = new Texture(mRoomCode, "Font/ARCADE.TTF", 16, {255, 255, 255});

  mPlayerCount = new Texture(std::to_string(playerCount) + "/4",
                             "Font/ARCADE.TTF", 16, {255, 255, 255});

  switch (status) {
    case RoomInfo::Status::WAITING:
      mStatusText = new Texture("Menu/waiting.png");
      mStatusTexture =
          new Texture("waiting", "Font/ARCADE.TTF", 16, {255, 255, 255});
      break;
    case RoomInfo::Status::FULL:
      mStatusText = new Texture("Menu/full.png");
      mStatusTexture =
          new Texture("full", "Font/ARCADE.TTF", 16, {255, 255, 255});
      break;
    case RoomInfo::Status::STARTED:
      mStatusText = new Texture("Menu/playing.png");
      mStatusTexture =
          new Texture("playing", "Font/ARCADE.TTF", 16, {255, 255, 255});
      break;
  }

  mBase = new AnimatedTexture("Tank/Red/Bodies/body_halftrack.png", 0, 0,
                              Graphics::Instance()->CELL_SIZE,
                              Graphics::Instance()->CELL_SIZE, 2, 0.2f,
                              AnimatedTexture::horizontal);
  mBase->Parent(this);
  mBase->Pos(Vector2(25.0f, 0.0f));
  mBase->Scale(Vector2(0.35f, 0.35f));

  mWeapon = new AnimatedTexture("Tank/Red/Weapons/turret_01_mk1.png", 0, 0,
                                Graphics::Instance()->CELL_SIZE,
                                Graphics::Instance()->CELL_SIZE, 8, 0.7f,
                                AnimatedTexture::horizontal);
  mWeapon->Parent(this);
  mWeapon->Pos(Vector2(25.0f, 0.0f));
  mWeapon->Scale(Vector2(0.35f, 0.35f));

  mCode->Parent(this);
  mCode->Pos(Vector2(-125.0f, 0.0f));

  mPlayerCount->Parent(this);
  mPlayerCount->Pos(Vector2(-25.0f, 0.0f));

  mStatusText->Parent(this);
  mStatusText->Scale(Vector2(0.125f, 0.125f));
  mStatusText->Pos(Vector2(50.0f, 0.0f));

  mStatusTexture->Parent(this);
  mStatusTexture->Pos(Vector2(125.0f, 0.0f));
}

RoomInfo::~RoomInfo() {
  delete mBox;
  mBox = NULL;

  delete mCode;
  mCode = NULL;

  delete mStatusText;
  mStatusText = NULL;

  delete mPlayerCount;
  mPlayerCount = NULL;

  delete mBase;
  mBase = NULL;

  delete mWeapon;
  mWeapon = NULL;

  delete mStatusTexture;
  mStatusTexture = NULL;
}

void RoomInfo::Render() {
  mCode->Render();
  mPlayerCount->Render();
  mStatusText->Render();

  mBase->Render();
  mWeapon->Render();
  mStatusTexture->Render();
}

void RoomInfo::Update() {}
