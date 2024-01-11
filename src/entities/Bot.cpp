#include <headers/Bot.h>

Bot::Bot(BASE_POSITION basePos, COLOR color) : Tank(false, basePos, color) {
  mTimer = Timer::Instance();

  srand(time(NULL));

  mActionRandomSpeed = 0.3f;
  mActionRandomTimer = 0.0f;
}

Bot::~Bot() {}

void Bot::LateUpdate() {
  if (mAlive) {
    mActionRandomTimer += mTimer->DeltaTime();

    if (mActionRandomTimer >= mActionRandomSpeed) {
      mActionRandomTimer = 0.0f;

      int action = rand() % 2;
      int randOffset = rand() % 10 < 7 ? 2 : 0;

      switch (action) {
        case 0:
          mDirection = static_cast<DIRECTION>(rand() % 2 + randOffset + 1);
          mFireDirection = mDirection;
          mIsMoving = true;
          break;

        case 1:
          mIsMoving = false;
          Shoot();
          break;

        default:
          break;
      }
    }
  }
}
