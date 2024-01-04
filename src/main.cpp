#include <QuickSDL/GameManager.h>

#include <socket/Client.hpp>

int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <interface> <port>" << std::endl;
    exit(EXIT_FAILURE);
  }

  TSS::Client* client = TSS::Client::Instance(atoi(argv[2]), (char*)argv[1]);

  client->login();

  QuickSDL::GameManager* game = QuickSDL::GameManager::Instance();

  game->Run();

  QuickSDL::GameManager::Release();
  game = NULL;

  TSS::Client::Release();
  client = NULL;

  return 0;
}
