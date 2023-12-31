#ifndef _BRICK_H
#define _BRICK_H

#include <QuickSDL/AnimatedTexture.h>
#include <headers/PhysicEntity.h>

using namespace QuickSDL;

class Brick : public PhysicEntity {
 private:
  static const int mMaxHealth = 3;
  int mHealth;

  bool mAnimating;

  AnimatedTexture* mTexture;
  AnimatedTexture* mDeathAnimation;

 public:
  Brick();
  ~Brick();

  void Hit(PhysicEntity* other) override;

  void Update();
  void Render();
};

#endif
