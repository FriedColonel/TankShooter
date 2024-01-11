#include <headers/BoxCollider.h>
#include <headers/Brick.h>
#include <headers/Bullet.h>
#include <headers/PhysicManager.h>

Brick::Brick() : PhysicEntity() {
  mId = PhysicManager::Instance()->RegisterEntity(
      this, PhysicManager::CollisionLayers::Terrain);

  mHealth = mMaxHealth;

  mAnimating = false;

  mTexture = new AnimatedTexture("Base/brick.png", 0, 0, 16, 16, 3, 1.0f,
                                 AnimatedTexture::horizontal);
  mTexture->Parent(this);
  mTexture->Scale(Vector2(4.0f, 4.0f));

  mDeathAnimation =
      new AnimatedTexture("Explosion/explosion.png", 0, 0, 256, 256, 8, 0.68f,
                          AnimatedTexture::horizontal);
  mDeathAnimation->Parent(this);
  mDeathAnimation->Scale(Vector2(0.25f, 0.25f));
  mDeathAnimation->WrapMode(AnimatedTexture::once);

  AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
}

Brick::~Brick() {
  delete mTexture;
  mTexture = NULL;

  delete mDeathAnimation;
  mDeathAnimation = NULL;
}

void Brick::Dead() {
  mAnimating = true;
  Active(false);
  mDeathAnimation->ResetAnimation();
}

void Brick::Hit(PhysicEntity* other) {
  // check if other is bullet or not
  if (instanceof <Bullet>(other)) {
    mHealth--;
    mTexture->IgnoreTimerUpdate();

    if (mHealth <= 0) Dead();
  }
}

void Brick::Update() {
  if (mAnimating) {
    mDeathAnimation->Update();
    mAnimating = !mDeathAnimation->IsAnimationDone();
  }
}

void Brick::Render() {
  if (mAnimating) {
    mDeathAnimation->Render();
  } else {
    if (Active()) {
      mTexture->Render();
      PhysicEntity::Render();
    }
  }
}
