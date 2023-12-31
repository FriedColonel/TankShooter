#include <QuickSDL/GameManager.h>

int main(int argc, char const* argv[]) {
  QuickSDL::GameManager* game = QuickSDL::GameManager::Instance();

  game->Run();

  QuickSDL::GameManager::Release();
  game = NULL;

  return 0;
}
