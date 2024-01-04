#include <headers/MapChooseScreen.h>

MapChooseScreen::MapChooseScreen() {
  mInput = InputManager::Instance();

  mTitle = new Texture("Choose Map", "Font/ARCADE.TTF", 60, {150, 0, 0});
  mTitle->Parent(this);
  mTitle->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                      Graphics::Instance()->SCREEN_HEIGHT * 0.1f));

  mChooseBox =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.5f));
  mChooseBox->Parent(this);

  mMap1 = new Texture("Map 1", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mMap1->Parent(mChooseBox);
  mMap1->Pos(Vector2(-256.0f, 0.0f));

  mMap2 = new Texture("Map 2", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mMap2->Parent(mChooseBox);
  mMap2->Pos(Vector2(0.0f, 0.0f));

  mMap3 = new Texture("Map 3", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mMap3->Parent(mChooseBox);
  mMap3->Pos(Vector2(256.0f, 0.0f));

  mSelectedMap = 1;

  mCursor = new Texture("Cursor/cursor.png");
  mCursor->Parent(mChooseBox);
  mCursor->Pos(Vector2(-256.0f, -90.0f));
  mCursor->Rotate(90.0f);
  mCursorStartPos = Vector2(-256.0f, -90.0f);
  mCursorOffset = 256;

  mInstructions = new Texture("Press Enter to select map", "Font/ARCADE.TTF",
                              32, {150, 0, 0});
  mInstructions->Parent(this);
  mInstructions->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.8f));
}

MapChooseScreen::~MapChooseScreen() {
  mInput = NULL;

  delete mTitle;
  mTitle = NULL;

  delete mChooseBox;
  mChooseBox = NULL;

  delete mMap1;
  mMap1 = NULL;

  delete mMap2;
  mMap2 = NULL;

  delete mMap3;
  mMap3 = NULL;
}

void MapChooseScreen::ChangeMap(int change) {
  mSelectedMap += change;

  if (mSelectedMap < 1) {
    mSelectedMap = 3;
  } else if (mSelectedMap > 3) {
    mSelectedMap = 1;
  }

  mCursor->Pos(mCursorStartPos + mCursorOffset * (mSelectedMap - 1));
}

int MapChooseScreen::GetSelectedMap() { return mSelectedMap; }

void MapChooseScreen::Update() {
  if (mInput->KeyPressed(SDL_SCANCODE_LEFT)) {
    ChangeMap(-1);
  } else if (mInput->KeyPressed(SDL_SCANCODE_RIGHT)) {
    ChangeMap(1);
  }
}

void MapChooseScreen::Render() {
  mTitle->Render();

  mMap1->Render();
  mMap2->Render();
  mMap3->Render();

  mCursor->Render();
  mInstructions->Render();
}
