#include <QuickSDL/GameManager.h>
#include <headers/ScreenManager.h>

#include <mutex>
#include <socket/Client.hpp>
#include <socket/ThreadPool.hpp>
#include <sstream>
#include <thread>

using namespace TSS;
using namespace std;

void *auto_recv_message() {
  Client *client = Client::Instance();

  char buffer[1024];

  while (true) {
    memset(buffer, 0, sizeof(buffer));
    size_t bytes_received = recv(client->get_sock(), buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
      cout << "Connection closed on socket " << socket << endl;
      close(client->get_sock());
      break;
    }

    buffer[bytes_received] = '\0';

    cout << "Received message: " << buffer << endl;

    string bufferStr(buffer);
    istringstream iss(bufferStr);
    string event_name, data;

    getline(iss, event_name, '\n');
    getline(iss, data, '\n');

    if (event_name == "game:join_room:success") {
      cout << endl << "Join room success " << data << endl;
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Current room: " << client->get_current_room()->room_id << endl;

      continue;
    }

    if (event_name == "game:create_room:success") {
      cout << endl << "Create room success " << data << endl;
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Current room: " << client->get_current_room()->room_id << endl;

      continue;
    }
  }

  return NULL;
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <interface> <port>" << std::endl;
    exit(EXIT_FAILURE);
  }

  TSS::Client *client = TSS::Client::Instance(atoi(argv[2]), (char *)argv[1]);
  client->login();

  thread recv_thread(auto_recv_message);
  recv_thread.detach();

  QuickSDL::GameManager *game = QuickSDL::GameManager::Instance();
  game->Run();

  QuickSDL::GameManager::Release();
  game = NULL;

  TSS::Client::Release();
  client = NULL;

  return 0;
}
