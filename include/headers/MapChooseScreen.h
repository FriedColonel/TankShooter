#ifndef _MAPCHOOSESCREEN_H
#define _MAPCHOOSESCREEN_H

#include <QuickSDL/GameEntity.h>
#include <QuickSDL/InputManager.h>
#include <QuickSDL/Texture.h>

using namespace QuickSDL;

class MapChooseScreen : public GameEntity {
 private:
  static MapChooseScreen* sInstance;

  Texture* mTitle;

  InputManager* mInput;

  GameEntity* mChooseBox;
  Texture* mMap1Title;
  Texture* mMap2Title;
  Texture* mMap3Title;
  Texture* mMap1;
  Texture* mMap2;
  Texture* mMap3;
  Texture* mInstructions;

  int mSelectedMap;

  Texture* mCursor;
  int mCursorOffset;
  Vector2 mCursorStartPos;

 public:
  static MapChooseScreen* Instance();
  static void Release();
  MapChooseScreen();
  ~MapChooseScreen();

  void ChangeMap(int change);

  int GetSelectedMap();

  void Update();
  void Render();
};

#endif
