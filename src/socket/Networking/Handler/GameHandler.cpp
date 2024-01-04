#include <socket/GameHandler.hpp>

TSS::GameHandler::GameHandler() { rooms = new LinkedList(); }

char *TSS::GameHandler::create_room(char *username, int map) {
  // create new player
  Player *player = new Player();
  player->username = username;
  player->tank = 1;
  player->status = 1;
  player->is_leader = true;

  // create new room
  Room *room = new Room();
  room->room_id = generate_random_string(5);
  room->players.push_back(*player);
  room->status = 1;
  room->map = map;

  rooms->insert(0, room, sizeof(struct Room));

  return room->room_id;
}

bool TSS::GameHandler::join_room(char *username, char *room_id) {
  Room *room = static_cast<Room *>(rooms->search(room_id, compare));
  if (room == NULL) {
    return false;
  }

  Player *player = new Player();
  player->username = username;
  player->tank = room->players.size() + 1;
  player->status = 0;
  player->is_leader = false;

  room->players.push_back(*player);

  return true;
}

char *TSS::GameHandler::generate_random_string(size_t length) {
  char *buffer = (char *)malloc(length + 1);
  char charset[] = "0123456789";

  // Seed the random number generator
  srand(time(NULL));

  for (size_t i = 0; i < length; i++) {
    int random_index = rand() % (sizeof(charset) - 1);
    buffer[i] = charset[random_index];
  }

  buffer[length] = '\0';  // Null-terminate the string

  return buffer;
}

TSS::Room *TSS::GameHandler::find_room(char *room_id) {
  Room *room = static_cast<Room *>(rooms->search(room_id, compare));
  return room;
}

bool TSS::GameHandler::leave_room(char *username, char *room_id) {
  Room *room = static_cast<Room *>(rooms->search(room_id, compare));
  if (room == NULL) {
    return false;
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (strcmp(room->players[i].username, username) == 0) {
      room->players.erase(room->players.begin() + i);
      return true;
    }
  }

  return false;
}

bool TSS::GameHandler::ready(char *username, char *room_id) {
  Room *room = static_cast<Room *>(rooms->search(room_id, compare));
  if (room == NULL) {
    return false;
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (strcmp(room->players[i].username, username) == 0) {
      room->players[i].status = 1;
      return true;
    }
  }

  return false;
}

bool TSS::GameHandler::unready(char *username, char *room_id) {
  Room *room = static_cast<Room *>(rooms->search(room_id, compare));
  if (room == NULL) {
    return false;
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (strcmp(room->players[i].username, username) == 0) {
      room->players[i].status = 0;
      return true;
    }
  }

  return false;
}

int TSS::GameHandler::compare(void *a, void *b) {
  struct Room *room = (struct Room *)a;
  char *room_id = (char *)b;

  if (strcmp(room->room_id, room_id) == 0) {
    return 0;
  }

  if (strcmp(room->room_id, room_id) > 0) {
    return 1;
  }

  return -1;
}
