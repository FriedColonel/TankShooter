#ifndef _BOT_H
#define _BOT_H

#include <QuickSDL/Timer.h>
#include <headers/Tank.h>

class Bot : public Tank {
 private:
  Timer* mTimer;

  float mActionRandomSpeed;
  float mActionRandomTimer;

 public:
  Bot(BASE_POSITION basePos, COLOR color);
  ~Bot();

  void LateUpdate() override;
};

#endif
