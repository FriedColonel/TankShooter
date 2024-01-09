#include <QuickSDL/GameManager.h>
#include <headers/JoinScreen.h>
#include <headers/PlayScreen.h>

#include <mutex>
#include <socket/Client.hpp>
#include <socket/ThreadPool.hpp>
#include <sstream>
#include <thread>

using namespace TSS;
using namespace std;

void *auto_recv_message();

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <interface> <port>" << std::endl;
    exit(EXIT_FAILURE);
  }

  TSS::Client *client = TSS::Client::Instance(atoi(argv[2]), (char *)argv[1]);
  client->auth_menu();

  QuickSDL::GameManager *game = QuickSDL::GameManager::Instance();
  thread recv_thread(auto_recv_message);
  recv_thread.detach();
  game->Run();

  QuickSDL::GameManager::Release();
  game = NULL;

  TSS::Client::Release();
  client = NULL;

  return 0;
}

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

    if (event_name == "game:get_rooms:success") {
      cout << endl << "Rooms data" << data << endl;

      TSS::LinkedList *rooms = new TSS::LinkedList();

      json_to_rooms_list(data, rooms);
      client->rooms = rooms;

      cout << "Rooms list: " << client->rooms->length << endl;

      continue;
    }

    if (event_name == "game:find_room:success") {
      Room *room = json_to_room(string_to_json(data));

      cout << "founed room: " << room->room_id << endl;

      continue;
    }

    // found other user leave room
    if (event_name == "game:leave_room:success") {
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Other user outed: " << client->get_current_room()->room_id
           << endl;

      continue;
    }

    if (event_name == "game:ready:success") {
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Ready success: " << client->get_current_room()->room_id << endl;

      continue;
    }

    if (event_name == "game:unready:success") {
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Unready success: " << client->get_current_room()->room_id
           << endl;

      continue;
    }

    if (event_name == "game:start:success") {
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Start success: " << client->get_current_room()->room_id << endl;

      continue;
    }

    // found other user shoot bullet
    if (event_name == "game:shoot") {
      string dataStr(data);
      istringstream iss(dataStr);
      string username, pos_x, pos_y, direction;

      getline(iss, username, ':');
      getline(iss, pos_x, ':');
      getline(iss, pos_y, ':');
      getline(iss, direction, ':');

      PlayScreen *playScreen = PlayScreen::Instance();
      playScreen->Shoot(username, Vector2(stof(pos_x), stof(pos_y)),
                        static_cast<GameEntity::DIRECTION>(stoi(direction)));

      cout << "Bullet start shoot: " << username << " " << pos_x << endl;
      continue;
    }

    // found other user start move
    if (event_name == "game:move:start") {
      string dataStr(data);
      istringstream iss(dataStr);
      string username, pos_x, pos_y, direction;

      getline(iss, username, ':');
      getline(iss, pos_x, ':');
      getline(iss, pos_y, ':');
      getline(iss, direction, ':');

      PlayScreen *playScreen = PlayScreen::Instance();

      cout << "Move start" << username << " " << pos_x << endl;

      playScreen->SetPlayerPosition(
          username, Vector2(stof(pos_x), stof(pos_y)), true,
          static_cast<GameEntity::DIRECTION>(stoi(direction)));

      continue;
    }

    // found other user stop move
    if (event_name == "game:move:stop") {
      string dataStr(data);
      istringstream iss(dataStr);
      string username, pos_x, pos_y;

      getline(iss, username, ':');
      getline(iss, pos_x, ':');
      getline(iss, pos_y, ':');

      PlayScreen *playScreen = PlayScreen::Instance();

      playScreen->SetPlayerPosition(username, Vector2(stof(pos_x), stof(pos_y)),
                                    false, GameEntity::DIRECTION::none);

      cout << "Move stop" << username << " " << pos_x << endl;
      continue;
    }

    if (event_name == "auth:logout:success") {
      cout << "Logout success" << endl;
      break;
    }
  }

  return NULL;
}
