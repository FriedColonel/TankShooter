#include <headers/MapChooseScreen.h>

MapChooseScreen* MapChooseScreen::sInstance = NULL;

MapChooseScreen* MapChooseScreen::Instance() {
  if (sInstance == NULL) {
    sInstance = new MapChooseScreen();
  }
  return sInstance;
}

MapChooseScreen::MapChooseScreen() {
  mInput = InputManager::Instance();

  mTitle = new Texture("Choose Map", "Font/ARCADE.TTF", 60, {150, 0, 0});
  mTitle->Parent(this);
  mTitle->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                      Graphics::Instance()->SCREEN_HEIGHT * 0.1f));

  mChooseBox =
      new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f,
                             Graphics::Instance()->SCREEN_HEIGHT * 0.6f));
  mChooseBox->Parent(this);

  mMap1Title = new Texture("Map 1", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mMap1Title->Parent(mChooseBox);
  mMap1Title->Pos(Vector2(-512.0f, 0.0f));

  mMap1 = new Texture("Map/map1.jpg");
  mMap1->Parent(mMap1Title);
  mMap1->Scale(Vector2(0.2f, 0.2f));
  mMap1->Pos(Vector2(0.0f, -128.0f));

  mMap2Title = new Texture("Map 2", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mMap2Title->Parent(mChooseBox);
  mMap2Title->Pos(Vector2(0.0f, 0.0f));

  mMap2 = new Texture("Map/map2.jpg");
  mMap2->Parent(mMap2Title);
  mMap2->Scale(Vector2(0.2f, 0.2f));
  mMap2->Pos(Vector2(0.0f, -128.0f));

  mMap3Title = new Texture("Map 3", "Font/ARCADE.TTF", 32, {150, 0, 0});
  mMap3Title->Parent(mChooseBox);
  mMap3Title->Pos(Vector2(512.0f, 0.0f));

  mMap3 = new Texture("Map/map3.jpg");
  mMap3->Parent(mMap3Title);
  mMap3->Scale(Vector2(0.2f, 0.2f));
  mMap3->Pos(Vector2(0.0f, -128.0f));

  mSelectedMap = 1;

  mCursor = new Texture("Cursor/cursor.png");
  mCursor->Parent(mChooseBox);
  mCursor->Pos(Vector2(-512.0f, -300.0f));
  mCursor->Rotate(90.0f);
  mCursorStartPos = Vector2(-512.0f, -300.0f);
  mCursorOffset = 512;

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

  delete mMap1Title;
  mMap1Title = NULL;

  delete mMap2Title;
  mMap2Title = NULL;

  delete mMap3Title;
  mMap3Title = NULL;
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

  mMap1Title->Render();
  mMap2Title->Render();
  mMap3Title->Render();

  mMap1->Render();
  mMap2->Render();
  mMap3->Render();

  mCursor->Render();
  mInstructions->Render();
}
