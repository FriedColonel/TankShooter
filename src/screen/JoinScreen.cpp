#include <headers/JoinScreen.h>

#include <iostream>

JoinScreen::JoinScreen() {
  mInput = InputManager::Instance();

  // title bar
  mTitleBar =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.1f));
  mTitle =
      new Texture("ENTER ROOM CODE", "Font/ARCADE.TTF", 48, {255, 255, 255});
  mTitle->Parent(mTitleBar);
  mTitle->Pos(Vector2(0.0f, 0.0f));

  // input box
  mInputBox =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.3f));

  mInputText = " ";

  // input text
  mInputBoxTexture =
      new Texture(mInputText, "Font/ARCADE.TTF", 32, {255, 255, 255});
  mInputBoxTexture->Parent(mInputBox);
  mInputBoxTexture->Pos(Vector2(0.0f, 0.0f));
}

JoinScreen::~JoinScreen() {
  delete mTitleBar;
  mTitleBar = NULL;

  delete mTitle;
  mTitle = NULL;

  delete mInputBox;
  mInputBox = NULL;

  delete mInputBoxTexture;
  mInputBoxTexture = NULL;
}

void JoinScreen::Update() {
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
  }
}

std::string JoinScreen::InputText() { return mInputText; }

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

  mInputBoxTexture =
      new Texture(mInputText, "Font/ARCADE.TTF", 32, {255, 255, 255});
  mInputBoxTexture->Parent(mInputBox);
  mInputBoxTexture->Pos(Vector2(0.0f, 0.0f));
}

void JoinScreen::Render() {
  mTitleBar->Render();
  mTitle->Render();

  mInputBox->Render();
  mInputBoxTexture->Render();
}
