#include <socket/GameHandler.hpp>

std::mutex room_mutex;

TSS::GameHandler::GameHandler() { rooms = new LinkedList(); }

std::string TSS::GameHandler::create_room(std::string username, int map,
                                          int req_player_socket) {
  // create new player
  Player *player = new Player();
  player->username = username;
  player->tank = 1;
  player->status = 0;
  player->points = 0;
  player->is_leader = true;
  player->client_socket = req_player_socket;

  // create new room
  Room *room = new Room();
  room->room_id = generate_random_string(5);
  room->players.push_back(*player);
  room->status = 1;
  room->map = map;

  rooms->insert(0, room, sizeof(struct Room));

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::join_room(std::string username,
                                        std::string room_id,
                                        int req_player_socket) {
  std::lock_guard<std::mutex> lock(room_mutex);
  Room *room = get_room(room_id);

  if (room == NULL) {
    return "";
  }

  Player *player = new Player();
  player->username = username;
  player->tank = room->players.size() + 1;
  player->status = 0;
  player->is_leader = false;
  player->client_socket = req_player_socket;

  room->players.push_back(*player);

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::find_room(std::string room_id) {
  Room *room = get_room(room_id);
  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::leave_room(std::string username,
                                         std::string room_id) {
  std::lock_guard<std::mutex> lock(room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (room->players[i].username == username) {
      room->players.erase(room->players.begin() + i);
      break;
    }
  }

  // remove room if no player in room
  if (room->players.size() == 0) {
    for (int i = 0; i < rooms->length; i++) {
      Room *r = static_cast<Room *>(rooms->retrieve(i));
      if (r->room_id == room_id) {
        rooms->remove(i);
        break;
      }
    }

    return "room_empty";
  }

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::ready(std::string username, std::string room_id) {
  std::lock_guard<std::mutex> lock(room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (room->players[i].username == username) {
      printf("Founded username: %s\n", room->players[i].username.c_str());
      room->players[i].status = 1;
      break;
    }
  }

  printf("Check find room\n");

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::unready(std::string username,
                                      std::string room_id) {
  std::lock_guard<std::mutex> lock(room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (room->players[i].username == username) {
      room->players[i].status = 0;
      break;
    }
  }

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::start_game(std::string room_id) {
  std::lock_guard<std::mutex> lock(room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  room->status = 2;

  return json_to_string(*room_to_json(room));
}

int TSS::GameHandler::compare(void *a, void *b) {
  struct Room *room = (struct Room *)a;
  char *room_id = (char *)b;

  return room->room_id == std::string(room_id);
}

// Getters function
std::string TSS::GameHandler::get_rooms_list() {
  json room_list_json = json::array();

  for (int i = 0; i < rooms->length; i++) {
    Room *room = static_cast<Room *>(rooms->retrieve(i));
    room_list_json.push_back(*room_to_json(room));
  }

  return json_to_string(room_list_json);
}

TSS::Room *TSS::GameHandler::get_room(std::string room_id) {
  Room *room =
      static_cast<Room *>(rooms->search((char *)room_id.c_str(), compare));

  return room;
}