#include <headers/BoxCollider.h>
#include <headers/Bullet.h>
#include <headers/PhysicManager.h>

Bullet::Bullet(bool isFriendly) : PhysicEntity() {
  mTimer = Timer::Instance();

  mTexture = new AnimatedTexture("Bullet/bullet.png", 11 * 32, 0, 32, 32, 4,
                                 0.2f, AnimatedTexture::horizontal);
  mTexture->Parent(this);
  mTexture->Pos(VEC2_ZERO);
  mTexture->WrapMode(AnimatedTexture::loop);

  mSpeed = 500.0f;

  mDirection = up;

  Reload();

  AddCollider(new BoxCollider(mTexture->ScaledDimensions() - VEC2_ONE * 10));

  if (isFriendly) {
    mId = PhysicManager::Instance()->RegisterEntity(
        this, PhysicManager::CollisionLayers::FriendlyProjectiles);
  } else {
    mId = PhysicManager::Instance()->RegisterEntity(
        this, PhysicManager::CollisionLayers::HostileProjectiles);
  }
}

Bullet::~Bullet() {
  mTimer = NULL;

  delete mTexture;
  mTexture = NULL;
}

void Bullet::Fire(Vector2 pos, DIRECTION dir) {
  Pos(pos);
  Active(true);
  mDirection = dir;
}

void Bullet::Hit(PhysicEntity* other) { Reload(); }

void Bullet::Reload() { Active(false); }

void Bullet::Update() {
  if (Active()) {
    switch (mDirection) {
      case up:
        AbsoluteRotate(90.0f);
        Translate(VEC2_UP * mSpeed * mTimer->DeltaTime(), world);
        break;
      case down:
        AbsoluteRotate(-90.0f);
        Translate(-VEC2_UP * mSpeed * mTimer->DeltaTime(), world);
        break;
      case left:
        AbsoluteRotate(0.0f);
        Translate(-VEC2_RIGHT * mSpeed * mTimer->DeltaTime(), world);
        break;
      case right:
        AbsoluteRotate(180.0f);
        Translate(VEC2_RIGHT * mSpeed * mTimer->DeltaTime(), world);
        break;
    }

    if (Pos().x < -OFFSCREEN_BUFFER ||
        Pos().x > Graphics::Instance()->SCREEN_WIDTH + OFFSCREEN_BUFFER ||
        Pos().y < -OFFSCREEN_BUFFER ||
        Pos().y > Graphics::Instance()->SCREEN_HEIGHT + OFFSCREEN_BUFFER) {
      Reload();
    }

    mTexture->Update();
  }
}

void Bullet::Render() {
  if (Active()) {
    mTexture->Render();
    PhysicEntity::Render();
  }
}
