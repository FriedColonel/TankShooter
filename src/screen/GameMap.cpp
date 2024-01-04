#include <headers/GameMap.h>
#include <headers/MapChoice.h>

#include <iostream>

GameMap* GameMap::sInstance = NULL;

GameMap* GameMap::Instance() {
  if (sInstance == NULL) sInstance = new GameMap(1);

  return sInstance;
}

void GameMap::Release() {
  delete sInstance;
  sInstance = NULL;
}

GameMap::GameMap(int mapChoice) : GameEntity() {
  mGraphics = Graphics::Instance();

  mMapChoice = mapChoice;

  // for (int i = 0; i < 16; i++) {
  //   for (int j = 0; j < 31; j++) {
  //     if (*(getMap(mapChoice) + i * 31 + j) == '#') {
  //       Brick* brick = new Brick();
  //       brick->Parent(this);
  //       brick->Pos(Vector2(
  //           j * mGraphics->MAP_CELL_SIZE + mGraphics->MAP_CELL_SIZE / 2,
  //           i * mGraphics->MAP_CELL_SIZE + mGraphics->MAP_CELL_SIZE / 2));
  //       mBricks.push_back(brick);
  //     }
  //   }
  // }

  mBackground = new Texture("Terrain/background.png");
  mBackground->Pos(
      Vector2(mGraphics->SCREEN_WIDTH * 0.5f, mGraphics->SCREEN_HEIGHT * 0.5f));
}

GameMap::~GameMap() {
  mGraphics = NULL;

  for (int i = 0; i < mBricks.size(); i++) {
    delete mBricks[i];
    mBricks[i] = NULL;
  }

  delete mBackground;
  mBackground = NULL;
}

void GameMap::Update() {
  for (int i = 0; i < mBricks.size(); i++) {
    mBricks[i]->Update();
  }
}

void GameMap::Render() {
  mBackground->Render();

  for (int i = 0; i < mBricks.size(); i++) {
    mBricks[i]->Render();
  }
}

void GameMap::SetMapChoice(int mapChoice) {
  mMapChoice = mapChoice;

  Reset();

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 31; j++) {
      if (*(getMap(mapChoice) + i * 31 + j) == '#') {
        Brick* brick = new Brick();
        brick->Parent(this);
        brick->Pos(Vector2(
            j * mGraphics->MAP_CELL_SIZE + mGraphics->MAP_CELL_SIZE / 2,
            i * mGraphics->MAP_CELL_SIZE + mGraphics->MAP_CELL_SIZE / 2));
        mBricks.push_back(brick);
      }
    }
  }
}

void GameMap::Reset() { mBricks.clear(); }