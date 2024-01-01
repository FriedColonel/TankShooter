#include <headers/GameMap.h>

GameMap* GameMap::sInstance = NULL;

GameMap* GameMap::Instance() {
  if (sInstance == NULL) sInstance = new GameMap();

  return sInstance;
}

void GameMap::Release() {
  delete sInstance;
  sInstance = NULL;
}

GameMap::GameMap() {
  mGraphics = Graphics::Instance();

  for (int i = 0; i < mBricksPos.size(); i++) {
    Brick* brick = new Brick();
    brick->Parent(this);
    brick->Pos(Vector2(mBricksPos[i].second * mGraphics->MAP_CELL_SIZE +
                           mGraphics->MAP_CELL_SIZE / 2,
                       mBricksPos[i].first * mGraphics->MAP_CELL_SIZE +
                           mGraphics->MAP_CELL_SIZE / 2));
    mBricks.push_back(brick);
  }
}

GameMap::~GameMap() {
  mGraphics = NULL;

  for (int i = 0; i < mBricks.size(); i++) {
    delete mBricks[i];
    mBricks[i] = NULL;
  }
}

void GameMap::Update() {
  for (int i = 0; i < mBricks.size(); i++) {
    mBricks[i]->Update();
  }
}

void GameMap::Render() {
  for (int i = 0; i < mBricks.size(); i++) {
    mBricks[i]->Render();
  }
}
