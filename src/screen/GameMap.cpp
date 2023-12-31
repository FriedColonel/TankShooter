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

GameMap::GameMap() {}

GameMap::~GameMap() {}

void GameMap::Update() {}

void GameMap::Render() {}
