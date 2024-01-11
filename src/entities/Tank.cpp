#include <headers/Base.h>
#include <headers/BoxCollider.h>
#include <headers/Tank.h>

#include <iostream>

Tank::Tank(bool isThisPlayer, BASE_POSITION basePos, COLOR color)
    : PhysicEntity() {
  mTimer = Timer::Instance();
  mInput = InputManager::Instance();
  mPhysicMgr = PhysicManager::Instance();
  mGraphics = Graphics::Instance();
  mClient = TSS::Client::Instance();

  mIsThisPlayer = isThisPlayer;

  if (isThisPlayer) {
    mId = mPhysicMgr->RegisterEntity(this,
                                     PhysicManager::CollisionLayers::Hostile);
  } else {
    mId = mPhysicMgr->RegisterEntity(this,
                                     PhysicManager::CollisionLayers::Hostile);
  }

  mAlive = true;
  mAnimating = false;
  mWasHit = false;
  mIsMoving = false;
  mMoveSpeed = MOVE_SPEED;

  switch (basePos) {
    case topLeft:
      Pos(Vector2(mGraphics->MAP_CELL_SIZE + mGraphics->CELL_SIZE * 0.5f,
                  mGraphics->MAP_CELL_SIZE));
      break;
    case bottomLeft:
      Pos(Vector2(mGraphics->MAP_CELL_SIZE + mGraphics->CELL_SIZE * 0.5f,
                  mGraphics->SCREEN_HEIGHT - mGraphics->MAP_CELL_SIZE));
      break;
    case bottomRight:
      Pos(Vector2(mGraphics->SCREEN_WIDTH -
                      (mGraphics->MAP_CELL_SIZE + mGraphics->CELL_SIZE * 0.5f),
                  mGraphics->SCREEN_HEIGHT - mGraphics->MAP_CELL_SIZE));
      break;
    case topRight:
      Pos(Vector2(mGraphics->SCREEN_WIDTH -
                      (mGraphics->MAP_CELL_SIZE + mGraphics->CELL_SIZE * 0.5f),
                  mGraphics->MAP_CELL_SIZE));
      break;
  }

  mBase = new AnimatedTexture(
      "Tank/" + colorMap.at(color) + "/Bodies/body_halftrack.png", 0, 0,
      Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 2, 0.2f,
      AnimatedTexture::horizontal);
  mBase->Parent(this);
  mBase->Scale(Vector2(0.5f, 0.5f));
  mBase->Pos(VEC2_ZERO);

  mWeapon = new AnimatedTexture(
      "Tank/" + colorMap.at(color) + "/Weapons/turret_01_mk1.png", 0, 0,
      Graphics::Instance()->CELL_SIZE, Graphics::Instance()->CELL_SIZE, 8, 0.7f,
      AnimatedTexture::horizontal);
  mWeapon->WrapMode(AnimatedTexture::once);
  mWeapon->Parent(this);
  mWeapon->Scale(Vector2(0.5f, 0.5f));
  mWeapon->Pos(VEC2_ZERO);

  mDeathAnimation =
      new AnimatedTexture("Explosion/explosion.png", 0, 0, 256, 256, 8, 0.68f,
                          AnimatedTexture::horizontal);
  mDeathAnimation->Scale(Vector2(mGraphics->MAP_CELL_SIZE / 256.0f,
                                 mGraphics->MAP_CELL_SIZE / 256.0f));
  mDeathAnimation->Parent(this);
  mDeathAnimation->Pos(VEC2_ZERO);
  mDeathAnimation->WrapMode(AnimatedTexture::once);

  mMoveSpeed = 50.0f;
  mMoveBoundsX = Vector2(10, Graphics::Instance()->SCREEN_WIDTH - 10);
  mMoveBoundsY = Vector2(10, Graphics::Instance()->SCREEN_HEIGHT - 10);

  mDirection = DIRECTION::up;
  mFireDirection = DIRECTION::up;

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (isThisPlayer) {
      mBullets[i] = new Bullet(false);
    } else {
      mBullets[i] = new Bullet(false);
    }
  }

  AddCollider(new BoxCollider(Vector2(32.0f, 40.0f)));
}

Tank::~Tank() {
  mTimer = NULL;
  mInput = NULL;
  mPhysicMgr = NULL;
  mGraphics = NULL;

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
}

void Tank::HandleMovement() {
  if (mIsMoving && !mIsColliding) {
    switch (mDirection) {
      case up:
        Translate(VEC2_UP * mMoveSpeed * mTimer->DeltaTime(), world);
        AbsoluteRotate(0.0f);
        mFireDirection = up;
        mBase->Update();
        break;
      case down:
        Translate(-VEC2_UP * mMoveSpeed * mTimer->DeltaTime(), world);
        AbsoluteRotate(180.0f);
        mFireDirection = down;
        mBase->Update();
        break;
      case left:
        Translate(-VEC2_RIGHT * mMoveSpeed * mTimer->DeltaTime(), world);
        AbsoluteRotate(-90.0f);
        mFireDirection = left;
        mBase->Update();
        break;
      case right:
        Translate(VEC2_RIGHT * mMoveSpeed * mTimer->DeltaTime(), world);
        AbsoluteRotate(90.0f);
        mFireDirection = right;
        mBase->Update();
        break;
    }
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

void Tank::HandleFiring() {
  if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
    Shoot();
  }
}

void Tank::Shoot() {
  if (mAnimating) {
    return;
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!mBullets[i]->Active()) {
      mAnimating = true;
      switch (mFireDirection) {
        case up:
          mBullets[i]->Fire(
              Pos() - Vector2(0.0f, Graphics::Instance()->MAP_CELL_SIZE),
              mFireDirection);
          mClient->shot_bullet(Pos().x,
                               Pos().y - Graphics::Instance()->MAP_CELL_SIZE,
                               mFireDirection);
          break;
        case down:
          mBullets[i]->Fire(
              Pos() + Vector2(0.0f, Graphics::Instance()->MAP_CELL_SIZE),
              mFireDirection);
          mClient->shot_bullet(Pos().x,
                               Pos().y + Graphics::Instance()->MAP_CELL_SIZE,
                               mFireDirection);
          break;
        case left:
          mBullets[i]->Fire(
              Pos() - Vector2(Graphics::Instance()->MAP_CELL_SIZE, 0.0f),
              mFireDirection);
          mClient->shot_bullet(Pos().x - Graphics::Instance()->MAP_CELL_SIZE,
                               Pos().y, mFireDirection);
          break;
        case right:
          mBullets[i]->Fire(
              Pos() + Vector2(Graphics::Instance()->MAP_CELL_SIZE, 0.0f),
              mFireDirection);
          mClient->shot_bullet(Pos().x + Graphics::Instance()->MAP_CELL_SIZE,
                               Pos().y, mFireDirection);
          break;
      }
      break;
    }
  }
}

void Tank::Shoot(Vector2 pos, GameEntity::DIRECTION direction) {
  if (mAnimating) {
    return;
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!mBullets[i]->Active()) {
      mAnimating = true;
      mBullets[i]->Fire(pos, direction);
      // Pos(pos);
      break;
    }
  }
}

void Tank::StopMoving(Vector2 pos, GameEntity::DIRECTION direction) {
  mIsMoving = false;
  mDirection = direction;
  Pos(pos);
}

void Tank::ChangePosition(Vector2 pos, GameEntity::DIRECTION direction,
                          bool moving) {
  mIsMoving = moving;
  mDirection = direction;
  printf("Change direction: %d\n", direction);
  Pos(pos);
}

void Tank::Dead() {
  mAnimating = true;
  mAlive = false;
  mWasHit = true;
  Active(false);
}

bool Tank::WasHit() { return mWasHit; }

void Tank::Hit(PhysicEntity* other) {
  if (instanceof <Brick>(other) || instanceof <Base>(other)) {
    mIsColliding = true;
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

bool Tank::IsAnimating() { return mAnimating; }

void Tank::Update() {
  if (mAnimating) {
    if (!mAlive) {
      mDeathAnimation->Update();
      mAnimating = !mDeathAnimation->IsAnimationDone();
    } else {
      mWeapon->Update();
      if (mWeapon->IsAnimationDone()) {
        mAnimating = false;
        mWeapon->ResetAnimation();
      }
    }
  }

  if (mAlive) {
    HandleMovement();
    if (mIsThisPlayer) {
      HandleFiring();
    }
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    mBullets[i]->Update();
  }
}

void Tank::LateUpdate() {
  if (mIsThisPlayer && !mIsColliding) {
    if (!mInput->KeyDown(SDL_SCANCODE_A) && !mInput->KeyDown(SDL_SCANCODE_D) &&
        !mInput->KeyDown(SDL_SCANCODE_W) && !mInput->KeyDown(SDL_SCANCODE_S) &&
        mIsMoving) {
      mIsMoving = false;
      mClient->move_stop(Pos().x, Pos().y);
    }

    if (mInput->KeyPressed(SDL_SCANCODE_A)) {
      mIsMoving = true;
      mDirection = left;
      mClient->move_start(Pos().x, Pos().y, mDirection);
    } else if (mInput->KeyPressed(SDL_SCANCODE_D)) {
      mIsMoving = true;
      mDirection = right;
      mClient->move_start(Pos().x, Pos().y, mDirection);
    } else if (mInput->KeyPressed(SDL_SCANCODE_W)) {
      mIsMoving = true;
      mDirection = up;
      mClient->move_start(Pos().x, Pos().y, mDirection);
    } else if (mInput->KeyPressed(SDL_SCANCODE_S)) {
      mIsMoving = true;
      mDirection = down;
      mClient->move_start(Pos().x, Pos().y, mDirection);
    } else if (mInput->KeyPressed(SDL_SCANCODE_J)) {
      Dead();
    }
  }
}

void Tank::Render() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    mBullets[i]->Render();
  }

  if (mAnimating) {
    if (!mAlive) {
      mDeathAnimation->Render();
    } else {
      mBase->Render();
      mWeapon->Render();
    }
  } else {
    if (mAlive) {
      mBase->Render();
      mWeapon->Render();

      PhysicEntity::Render();
    }
  }
}
