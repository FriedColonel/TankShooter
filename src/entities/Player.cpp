#include <headers/BoxCollider.h>
#include <headers/CircleCollider.h>
#include <headers/PhysicManager.h>
#include <headers/Player.h>

#include <iostream>

Player::Player(bool isThisPlayer, BASE_POSITION basePos, COLOR color,
               bool isBot, bool isLeader) {
  mTimer = Timer::Instance();
  mInput = InputManager::Instance();
  mClient = TSS::Client::Instance();

  mAlive = true;
  mThisPlayer = isThisPlayer;
  mIsLeader = isLeader;

  mBase = new Base(basePos, isThisPlayer, color, isBot);

  Active(true);
}

Player::~Player() {
  mTimer = NULL;
  mInput = NULL;
  mClient = NULL;

  delete mBase;
  mBase = NULL;
}

void Player::Alive(bool alive) { mAlive = alive; }

bool Player::Alive() { return mAlive; }

void Player::ChangePosition(Vector2 pos, GameEntity::DIRECTION direction,
                            bool moving) {
  mBase->mTank->ChangePosition(pos, direction, moving);
}

void Player::Shoot(Vector2 pos, GameEntity::DIRECTION direction) {
  mBase->mTank->Shoot(pos, direction);
}

void Player::Dead() { mBase->Dead(); }

bool Player::IsThisPlayer() { return mThisPlayer; }

void Player::Update() {
  if (mBase->WasHit()) {
    mAlive = false;
  }

  if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE))
    if (mIsLeader)
      if (!mClient->is_game_paused)
        mClient->pause_game();
      else
        mClient->resume_game();

  if (mClient->is_game_paused) return;

  mBase->Update();
}

void Player::LateUpdate() {
  if (mClient->is_game_paused) return;

  mBase->mTank->LateUpdate();
}

void Player::Render() { mBase->Render(); }
