#include <headers/Base.h>

Base::Base(BASE_POSITION basePos) {
  mTimer = Timer::Instance();
  mGraphics = Graphics::Instance();

  mTexture = new Texture("Base/eagle.png", 0, 0, 60, 60);
  mTexture->Parent(this);

  mBasePosition = basePos;

  switch (basePos) {
    case topLeft:
      Pos(Vector2(mGraphics->MAP_CELL_SIZE * 0.5f,
                  mGraphics->MAP_CELL_SIZE * 0.5f));
      break;
    case bottomLeft:
      Pos(Vector2(mGraphics->MAP_CELL_SIZE * 0.5f,
                  mGraphics->SCREEN_HEIGHT - mGraphics->MAP_CELL_SIZE * 0.5f));
      break;
    case bottomRight:
      Pos(Vector2(mGraphics->SCREEN_WIDTH - mGraphics->MAP_CELL_SIZE * 0.5f,
                  mGraphics->SCREEN_HEIGHT - mGraphics->MAP_CELL_SIZE * 0.5f));
      break;
    case topRight:
      Pos(Vector2(mGraphics->SCREEN_WIDTH - mGraphics->MAP_CELL_SIZE * 0.5f,
                  mGraphics->MAP_CELL_SIZE * 0.5f));
      break;
  }

  for (int i = 0; i < 3; i++) {
    mWalls[i] = new Brick();
    mWalls[i]->Parent(this);
  }

  switch (basePos) {
    case topLeft:
      mWalls[0]->Pos(Vector2(0.0f, mGraphics->MAP_CELL_SIZE));
      mWalls[1]->Pos(Vector2(mGraphics->MAP_CELL_SIZE, 0.0f));
      mWalls[2]->Pos(VEC2_ONE * mGraphics->MAP_CELL_SIZE);
      break;

    case topRight:
      mWalls[0]->Pos(Vector2(0.0f, mGraphics->MAP_CELL_SIZE));
      mWalls[1]->Pos(Vector2(-mGraphics->MAP_CELL_SIZE, 0.0f));
      mWalls[2]->Pos(Vector2(-1.0f, 1.0f) * mGraphics->MAP_CELL_SIZE);
      break;

    case bottomLeft:
      mWalls[0]->Pos(Vector2(0.0f, -mGraphics->MAP_CELL_SIZE));
      mWalls[1]->Pos(Vector2(mGraphics->MAP_CELL_SIZE, 0.0f));
      mWalls[2]->Pos(Vector2(1.0f, -1.0f) * mGraphics->MAP_CELL_SIZE);
      break;

    case bottomRight:
      mWalls[0]->Pos(Vector2(0.0f, -mGraphics->MAP_CELL_SIZE));
      mWalls[1]->Pos(Vector2(-mGraphics->MAP_CELL_SIZE, 0.0f));
      mWalls[2]->Pos(Vector2(-1.0f, -1.0f) * mGraphics->MAP_CELL_SIZE);
      break;
  }
}

Base::~Base() {
  mTimer = NULL;
  mGraphics = NULL;

  delete mTexture;
  mTexture = NULL;

  for (int i = 0; i < 3; i++) {
    delete mWalls[i];
    mWalls[i] = NULL;
  }
}

void Base::Update() {
  for (int i = 0; i < 3; i++) {
    mWalls[i]->Update();
  }
}

void Base::Render() {
  mTexture->Render();

  for (int i = 0; i < 3; i++) {
    mWalls[i]->Render();
  }
}
