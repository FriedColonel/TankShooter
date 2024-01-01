#include <headers/BoxCollider.h>
#include <headers/CircleCollider.h>
#include <headers/PhysicManager.h>
#include <headers/Player.h>

#include <iostream>

Player::Player(bool isThisPlayer) {
  mTimer = Timer::Instance();
  mInput = InputManager::Instance();

  mAlive = true;
  mAnimating = false;
  mWasHit = false;
  mThisPlayer = isThisPlayer;

  mBase = new AnimatedTexture(
      "Tank/Bodies/body_halftrack.png", 0, 0, Graphics::Instance()->CELL_SIZE,
      Graphics::Instance()->CELL_SIZE, 2, 0.2f, AnimatedTexture::horizontal);
  mBase->Parent(this);
  mBase->Pos(VEC2_ZERO);
  mBase->WrapMode(AnimatedTexture::loop);

  mWeapon = new AnimatedTexture(
      "Tank/Weapons/turret_01_mk1.png", 0, 0, Graphics::Instance()->CELL_SIZE,
      Graphics::Instance()->CELL_SIZE, 8, 0.5f, AnimatedTexture::horizontal);
  mWeapon->Parent(this);
  mWeapon->Pos(VEC2_ZERO);
  mWeapon->WrapMode(AnimatedTexture::once);

  mMoveSpeed = MOVE_SPEED;
  mMoveBoundsX = Vector2(50, Graphics::Instance()->SCREEN_WIDTH - 50);
  mMoveBoundsY = Vector2(50, Graphics::Instance()->SCREEN_HEIGHT - 50);

  mDeathAnimation =
      new AnimatedTexture("Explosion/explosion.png", 0, 0, 256, 256, 8, 0.68f,
                          AnimatedTexture::horizontal);
  mDeathAnimation->Scale(Vector2(0.5f, 0.5f));
  mDeathAnimation->Parent(this);
  mDeathAnimation->Pos(VEC2_ZERO);
  mDeathAnimation->WrapMode(AnimatedTexture::once);

  mDirection = up;
  mFireDirection = up;

  if (mThisPlayer) {
    mMainHouse = new Base(bottomLeft);
    Pos(Vector2(64.0f, 192.0f));
  } else {
    mMainHouse = new Base(topRight);
  }

  // AddCollider(new CircleCollider(50.0f));
  AddCollider(new BoxCollider(Vector2(64.0f, 90.0f)), Vector2(0.0f, -5.0f));

  if (mThisPlayer) {
    mId = PhysicManager::Instance()->RegisterEntity(
        this, PhysicManager::CollisionLayers::Friendly);
    for (int i = 0; i < MAX_BULLETS; i++) {
      mBullets[i] = new Bullet();
    }
  } else {
    mId = PhysicManager::Instance()->RegisterEntity(
        this, PhysicManager::CollisionLayers::Hostile);
    for (int i = 0; i < MAX_BULLETS; i++) {
      mBullets[i] = new Bullet(false);
    }
  }

  Active(true);
}

Player::~Player() {
  mTimer = NULL;
  mInput = NULL;

  delete mBase;
  mBase = NULL;

  delete mWeapon;
  mWeapon = NULL;

  delete mDeathAnimation;
  mDeathAnimation = NULL;

  for (int i = 0; i < MAX_BULLETS; i++) {
    delete mBullets[i];
    mBullets[i] = NULL;
  }

  delete mMainHouse;
  mMainHouse = NULL;
}

void Player::HandleMovement() {
  if (mInput->KeyDown(SDL_SCANCODE_A)) {
    Translate(-VEC2_RIGHT * mMoveSpeed * mTimer->DeltaTime(), world);
    mDirection = left;
    AbsoluteRotate(-90.0f);
    // mBase->AbsoluteRotate(-90.0f);
    mBase->Update();
  } else if (mInput->KeyDown(SDL_SCANCODE_D)) {
    Translate(VEC2_RIGHT * mMoveSpeed * mTimer->DeltaTime(), world);
    mDirection = right;
    AbsoluteRotate(90.0f);
    // mBase->AbsoluteRotate(90.0f);
    mBase->Update();
  } else if (mInput->KeyDown(SDL_SCANCODE_W)) {
    Translate(VEC2_UP * mMoveSpeed * mTimer->DeltaTime(), world);
    mDirection = up;
    AbsoluteRotate(0.0f);
    // mBase->AbsoluteRotate(0.0f);
    mBase->Update();
  } else if (mInput->KeyDown(SDL_SCANCODE_S)) {
    Translate(-VEC2_UP * mMoveSpeed * mTimer->DeltaTime(), world);
    mDirection = down;
    AbsoluteRotate(180.0f);
    // mBase->AbsoluteRotate(180.0f);
    mBase->Update();
  } else if (mInput->KeyDown(SDL_SCANCODE_J)) {
    Hit(NULL);
  }

  if (mInput->KeyDown(SDL_SCANCODE_J)) {
    WasHit();
  } else if (mInput->KeyDown(SDL_SCANCODE_UP)) {
    mFireDirection = up;
    mWeapon->RotateWorld(0.0f);
  } else if (mInput->KeyDown(SDL_SCANCODE_DOWN)) {
    mFireDirection = down;
    mWeapon->RotateWorld(180.0f);
  } else if (mInput->KeyDown(SDL_SCANCODE_LEFT)) {
    mFireDirection = left;
    mWeapon->RotateWorld(-90.0f);
  } else if (mInput->KeyDown(SDL_SCANCODE_RIGHT)) {
    mFireDirection = right;
    mWeapon->RotateWorld(90.0f);
  }

  Vector2 pos = Pos(local);
  if (pos.x < mMoveBoundsX.x) {
    pos.x = mMoveBoundsX.x;
  } else if (pos.x > mMoveBoundsX.y) {
    pos.x = mMoveBoundsX.y;
  }

  if (pos.y < mMoveBoundsY.x) {
    pos.y = mMoveBoundsY.x;
  } else if (pos.y > mMoveBoundsY.y) {
    pos.y = mMoveBoundsY.y;
  }

  Pos(pos);
}

void Player::HandleFiring() {
  if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
    Shoot();
  }
}

void Player::Alive(bool alive) { mAlive = alive; }

void Player::Hit(PhysicEntity* other) {
  if (instanceof <Brick>(other)) {
    switch (mDirection) {
      case up:
        Translate(-VEC2_UP * mMoveSpeed * mTimer->DeltaTime(), world);
        break;
      case down:
        Translate(VEC2_UP * mMoveSpeed * mTimer->DeltaTime(), world);
        break;
      case left:
        Translate(VEC2_RIGHT * mMoveSpeed * mTimer->DeltaTime(), world);
        break;
      case right:
        Translate(-VEC2_RIGHT * mMoveSpeed * mTimer->DeltaTime(), world);
        break;
    }
    return;
  }

  mAnimating = true;
  mWasHit = true;
  mAlive = false;
  mDeathAnimation->ResetAnimation();
  Active(false);
}

bool Player::IsAnimating() { return mAnimating; }

bool Player::WasHit() { return mWasHit; }

void Player::Shoot() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!mBullets[i]->Active()) {
      mBullets[i]->Fire(Pos(world), mFireDirection);
      break;
    }
  }

  mAnimating = true;
  mWeapon->ResetAnimation();
}

void Player::Update() {
  if (mAnimating) {
    if (mWasHit) mWasHit = false;

    if (mAlive) {
      mWeapon->Update();
      if (mWeapon->IsAnimationDone()) {
        mAnimating = false;
      }
    } else {
      mDeathAnimation->Update();
      if (mDeathAnimation->IsAnimationDone()) {
        mAnimating = false;
      }
    }
  } else {
    if (Active() && mThisPlayer) {
      HandleMovement();
      HandleFiring();
    }
  }

  for (int i = 0; i < MAX_BULLETS; i++) mBullets[i]->Update();
  mMainHouse->Update();
}

void Player::Render() {
  for (int i = 0; i < MAX_BULLETS; i++) mBullets[i]->Render();

  if (mAlive) {
    mBase->Render();
    mWeapon->Render();
    mMainHouse->Render();
    PhysicEntity::Render();
  } else {
    if (mAnimating) {
      mDeathAnimation->Render();
    }
  }
}
