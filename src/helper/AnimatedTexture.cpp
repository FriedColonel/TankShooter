//-----------------------------------------------------------------//
// AnimatedTexture.cpp                                             //
// Extends the Texture class' functionality to handle animations   //
// Uses the Texture class' spritesheet constructor                 //
//                                                                 //
// By: Ather Omar                                                  //
//-----------------------------------------------------------------//
#include <QuickSDL/AnimatedTexture.h>
//---------------------------------------------------------------
// QuickSDL
//---------------------------------------------------------------
namespace QuickSDL {

AnimatedTexture::AnimatedTexture(std::string filename, int x, int y, int w,
                                 int h, int frameCount, float animationSpeed,
                                 ANIM_DIR animationDir)
    : Texture(filename, x, y, w, h) {
  mTimer = Timer::Instance();

  mStartX = x;
  mStartY = y;

  mFrameCount = frameCount;
  mAnimationSpeed = animationSpeed;
  mTimePerFrame = mAnimationSpeed / mFrameCount;
  mAnimationTimer = 0.0f;
  mCurrentFrame = 0;

  mAnimationDirection = animationDir;

  mAnimationDone = false;

  mWrapMode = loop;
}

AnimatedTexture::~AnimatedTexture() {}

void AnimatedTexture::WrapMode(WRAP_MODE mode) { mWrapMode = mode; }

void AnimatedTexture::Update() {
  if (!mAnimationDone) {
    mAnimationTimer += mTimer->DeltaTime();

    if (mAnimationTimer >= mAnimationSpeed) {
      // Only loop if the wrap mode is loop
      if (mWrapMode == loop) {
        mAnimationTimer -= mAnimationSpeed;

      } else {
        mAnimationDone = true;
        mAnimationTimer = mAnimationSpeed - mTimePerFrame;
      }
    }

    if (mAnimationDirection == horizontal) {
      mClipRect.x = mStartX + (int)(mAnimationTimer / mTimePerFrame) * mWidth;

    } else {
      mClipRect.y = mStartY + (int)(mAnimationTimer / mTimePerFrame) * mHeight;
    }
  }
}

void AnimatedTexture::IgnoreTimerUpdate() {
  if (!mAnimationDone) {
    if (mCurrentFrame == mFrameCount - 1) {
      mAnimationDone = true;
    }

    if (mAnimationDirection == horizontal) {
      mClipRect.x = mStartX + (++mCurrentFrame) * mWidth;
    } else {
      mClipRect.y = mStartY + (++mCurrentFrame) * mHeight;
    }
  }
}

bool AnimatedTexture::IsAnimationDone() { return mAnimationDone; }

void AnimatedTexture::ResetAnimation() {
  mAnimationDone = false;
  mAnimationTimer = 0.0f;
  mClipRect.x = mStartX;
  mClipRect.y = mStartY;
}
}  // namespace QuickSDL
