#include <headers/Base.h>
#include <headers/BoxCollider.h>
#include <headers/Bullet.h>

Base::Base(BASE_POSITION basePos, bool isThisPlayer, COLOR color) {
  mTimer = Timer::Instance();
  mGraphics = Graphics::Instance();
  mPhysicMgr = PhysicManager::Instance();

  mAnimating = false;
  mIsThisPlayer = isThisPlayer;

  mRespawnTimer = 0.0f;
  mRespawnDelay = 1.0f;

  mColor = color;

  mId =
      mPhysicMgr->RegisterEntity(this, PhysicManager::CollisionLayers::Terrain);

  mTank = new Tank(isThisPlayer, basePos, color);

  mTexture = new Texture("Base/eagle.png", 0, 0, 60, 60);
  mTexture->Parent(this);
  AddCollider(new BoxCollider(mTexture->ScaledDimensions()));

  mDeathAnimation =
      new AnimatedTexture("Explosion/explosion.png", 0, 0, 256, 256, 8, 0.68f,
                          AnimatedTexture::horizontal);
  mDeathAnimation->Parent(this);
  mDeathAnimation->Scale(Vector2(0.25f, 0.25f));
  mDeathAnimation->WrapMode(AnimatedTexture::once);

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
  mPhysicMgr = NULL;

  delete mTexture;
  mTexture = NULL;

  delete mTank;
  mTank = NULL;

  for (int i = 0; i < 3; i++) {
    delete mWalls[i];
    mWalls[i] = NULL;
  }

  delete mDeathAnimation;
  mDeathAnimation = NULL;

  mId = 0;
}

void Base::HandlePlayerDeath() {
  if (mTank->WasHit() && Active()) {
    if (mRespawnTimer == 0.0f) {
      mTank->Dead();
    }

    mRespawnTimer += mTimer->DeltaTime();

    if (mRespawnTimer >= mRespawnDelay) {
      mRespawnTimer = 0.0f;

      delete mTank;
      mTank = new Tank(mIsThisPlayer, mBasePosition, mColor);
    }
  }
}

bool Base::WasHit() { return mAnimating; }

void Base::Hit(PhysicEntity* other) {
  if (instanceof <Bullet>(other)) {
    mAnimating = true;
    Active(false);
    mTank->Dead();

    for (int i = 0; i < 3; i++) {
      mWalls[i]->Dead();
    }
  }
}

void Base::ChangePosition(Vector2 pos, GameEntity::DIRECTION direction,
                          bool moving) {
  mTank->ChangePosition(pos, direction, moving);
}

void Base::Update() {
  if (mAnimating) {
    mDeathAnimation->Update();
    mAnimating = !mDeathAnimation->IsAnimationDone();
  } else {
    HandlePlayerDeath();
  }

  mTank->Update();

  for (int i = 0; i < 3; i++) {
    mWalls[i]->Update();
  }
}

void Base::Render() {
  if (mAnimating) {
    mDeathAnimation->Render();
    mTank->Render();
  } else {
    if (Active()) {
      mTank->Render();
      mTexture->Render();
      for (int i = 0; i < 3; i++) {
        mWalls[i]->Render();
      }
      PhysicEntity::Render();
    }
  }
}
