#include <headers/BoxCollider.h>
#include <headers/CircleCollider.h>
#include <headers/PhysicManager.h>
#include <headers/Player.h>

#include <iostream>

Player::Player(bool isThisPlayer, BASE_POSITION basePos, COLOR color) {
  mTimer = Timer::Instance();
  mInput = InputManager::Instance();

  mAlive = true;
  mThisPlayer = isThisPlayer;

  mBase = new Base(basePos, isThisPlayer, color);

  Active(true);
}

Player::~Player() {
  mTimer = NULL;
  mInput = NULL;
}

void Player::Alive(bool alive) { mAlive = alive; }

void Player::ChangePosition(Vector2 pos, GameEntity::DIRECTION direction,
                            bool moving) {
  mBase->mTank->ChangePosition(pos, direction, moving);
}

void Player::Shoot(Vector2 pos, GameEntity::DIRECTION direction) {
  mBase->mTank->Shoot(pos, direction);
}

void Player::Update() {
  if (mBase->WasHit()) {
    mAlive = false;
  }

  mBase->Update();
}

void Player::Render() { mBase->Render(); }
