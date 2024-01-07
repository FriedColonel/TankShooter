#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>

#include <atomic>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "LinkedList.hpp"

using json = nlohmann::json;
using std::to_string;

namespace TSS {
struct User {
  std::string username;
  std::string password;
  bool is_login;
};

struct Player {
  int client_socket;
  std::string username;
  int tank;  // 1: red, 2: blue, 3:green, 4:white
  bool is_leader;
  int status;  // 0: not ready, 1: ready, 2: out
  int points;
};

struct Room {
  std::string room_id;
  std::vector<Player> players;
  int status;  // 1: waiting, 2: playing, 3: pause, 4: finished
  int map;
};

// room converter
json *room_to_json(Room *room);
Room *json_to_room(json *j);
std::string get_rooms_list(LinkedList *rooms);

// user converter
json *user_to_json(User *user);
User *json_to_user(json *j);

std::string json_to_string(json j);
json *string_to_json(std::string str);
}  // namespace TSS

#endif
