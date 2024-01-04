#ifndef _MAPCHOOSESCREEN_H
#define _MAPCHOOSESCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>

using namespace QuickSDL;

class MapChooseScreen : public GameEntity {
 private:
  Texture* mTitle;

  InputManager* mInput;

  GameEntity* mChooseBox;
  Texture* mMap1;
  Texture* mMap2;
  Texture* mMap3;
  Texture* mInstructions;

  int mSelectedMap;

  Texture* mCursor;
  int mCursorOffset;
  Vector2 mCursorStartPos;

 public:
  MapChooseScreen();
  ~MapChooseScreen();

  void ChangeMap(int change);

  int GetSelectedMap();

  void Update();
  void Render();
};

#endif
