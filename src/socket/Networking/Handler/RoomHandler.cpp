#include <socket/RoomHandler.hpp>

TSS::RoomHandler::RoomHandler(std::mutex *room_mutex, LinkedList *rooms) {
  this->room_mutex = room_mutex;
  this->rooms = rooms;
}

std::string TSS::RoomHandler::create_room(std::string username, int map,
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

std::string TSS::RoomHandler::join_room(std::string username,
                                        std::string room_id,
                                        int req_player_socket) {
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);

  if (room == NULL) {
    return "";
  }

  if (room->status != 1) {
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

std::string TSS::RoomHandler::find_room(std::string room_id) {
  Room *room = get_room(room_id);
  return json_to_string(*room_to_json(room));
}

std::string TSS::RoomHandler::leave_room(std::string username,
                                         std::string room_id) {
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  // remove room if no player left in room
  if (room->players.size() <= 1) {
    for (int i = 0; i < rooms->length; i++) {
      Room *r = static_cast<Room *>(rooms->retrieve(i));
      if (r->room_id == room_id) {
        rooms->remove(i);
        break;
      }
    }

    return "room_empty";
  }

  // remove player from room
  for (int i = 0; i < room->players.size(); i++) {
    if (room->players[i].username == username) {
      // if leader leave room, assign new leader
      if (room->players[i].is_leader) {
        room->players[i + 1].is_leader = true;
      }

      room->players.erase(room->players.begin() + i);

      break;
    }
  }

  // reassign tank color
  for (int i = 0; i < room->players.size(); i++) {
    room->players[i].tank = i + 1;
  }

  return json_to_string(*room_to_json(room));
}

int TSS::RoomHandler::compare(void *a, void *b) {
  struct Room *room = (struct Room *)a;
  char *room_id = (char *)b;

  return room->room_id == std::string(room_id);
}

// Getters function
std::string TSS::RoomHandler::get_rooms_list() {
  json room_list_json = json::array();

  for (int i = 0; i < rooms->length; i++) {
    Room *room = static_cast<Room *>(rooms->retrieve(i));
    if (room->status == 1) room_list_json.push_back(*room_to_json(room));
  }

  return json_to_string(room_list_json);
}

TSS::Room *TSS::RoomHandler::get_room(std::string room_id) {
  Room *room =
      static_cast<Room *>(rooms->search((char *)room_id.c_str(), compare));

  return room;
}
