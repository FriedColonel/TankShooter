#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

#define PORT 5550
#define BACKLOG 2
#define MAX_CLIENTS 10
#define KEY_SIZE 20
#define BUFF_SIZE 32765
#define CODE_LENGTH 5

using namespace std;
using namespace nlohmann;

map<string, string> rooms;

string seedRoomCode() {
  string code;
  for (int i = 0; i < CODE_LENGTH; i++) {
    code += (rand() % 10) + '0';
  }

  return code;
}

int main(int argc, char const* argv[]) {
  srand(time(NULL));
  json j = json::parse("{\"happy\": true, \"pi\": 3.141}");
  cout << j.dump() << endl;
  return 0;
}
