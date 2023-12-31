#ifndef _LOBBYSCREEN_H
#define _LOBBYSCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/Graphics.h>
#include <QuickSDL/Texture.h>

#include <string>

using namespace QuickSDL;

class LobbyScreen : public GameEntity {
 private:
  GameEntity* mInputBox;
  Texture* mInputBoxTexture;

  std::string mInputText;

 public:
  void Update();
  void Render();

 private:
  LobbyScreen();
  ~LobbyScreen();
};

#endif
