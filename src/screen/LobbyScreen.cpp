#include <headers/LobbyScreen.h>

LobbyScreen::LobbyScreen() {
  mInputBox =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.3f));
}

LobbyScreen::~LobbyScreen() {
  delete mInputBox;
  mInputBox = NULL;
}

void LobbyScreen::Update() {}

void LobbyScreen::Render() { mInputBox->Render(); }
