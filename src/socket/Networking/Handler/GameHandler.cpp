#include <socket/GameHandler.hpp>

TSS::GameHandler::GameHandler(std::mutex *room_mutex, LinkedList *rooms) {
  this->room_mutex = room_mutex;
  this->rooms = rooms;
}

std::string TSS::GameHandler::ready(std::string username, std::string room_id) {
  std::lock_guard<std::mutex> lock(*room_mutex);
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
  std::lock_guard<std::mutex> lock(*room_mutex);
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
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  room->status = 2;

  for (int i = 0; i < room->players.size(); i++) {
    room->players[i].status = 1;
    room->players[i].points = 0;
  }

  return json_to_string(*room_to_json(room));
}

int TSS::GameHandler::compare(void *a, void *b) {
  struct Room *room = (struct Room *)a;
  char *room_id = (char *)b;

  return room->room_id == std::string(room_id);
}

TSS::Room *TSS::GameHandler::get_room(std::string room_id) {
  Room *room =
      static_cast<Room *>(rooms->search((char *)room_id.c_str(), compare));

  return room;
}

std::string TSS::GameHandler::pause_game(std::string room_id) {
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  room->status = 3;

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::resume_game(std::string room_id) {
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  room->status = 2;

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::player_dead(std::string username,
                                          std::string room_id) {
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (room->players[i].username == username) {
      room->players[i].status = 3;
      break;
    }
  }

  return json_to_string(*room_to_json(room));
}

std::string TSS::GameHandler::game_end(std::string room_id) {
  std::lock_guard<std::mutex> lock(*room_mutex);
  Room *room = get_room(room_id);
  if (room == NULL) {
    return "";
  }

  room->status = 4;

  for (int i = 0; i < room->players.size(); i++) {
    room->players[i].status = 0;
    std::string saved_point_path =
        saved_archivement_prefix + room->players[i].username + ".txt";

    std::string saved_point =
        room_id + " " + std::to_string(room->players[i].points) + "\n";

    TSS::append_file((char *)saved_point_path.c_str(),
                     (void *)saved_point.c_str(), saved_point.length());
  }

  return json_to_string(*room_to_json(room));
}
