#include <stdio.h>

#include <iostream>
#include <socket/Server.hpp>

using namespace std;
using namespace TSS;

int main(int argc, char const *argv[]) {
  Server *server =
      new Server(AF_INET, SOCK_STREAM, 0, atoi(argv[1]), INADDR_ANY, 5);

  server->launch();

  return 0;
}
