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
struct OnlineUser {
  std::string username;
  int client_socket;
  bool is_join_room;
};

struct User {
  std::string username;
  std::string password;
  bool is_login;
};

struct TopUser {
  std::string username;
  std::string room_id;
  int points;
};

struct Player {
  int client_socket;
  std::string username;
  int tank;  // 1: red, 2: blue, 3:green, 4:white
  bool is_leader;
  int status;  // 0: not ready, 1: ready, 2: out, 3: die
  int points;  // 1: top1, 2: top2, 3: top3, 4: top4
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
void json_to_rooms_list(std::string str, LinkedList *rooms);

// user converter
json *user_to_json(User *user);
User *json_to_user(json *j);

// leaderboard converter
json *top_user_to_json(TopUser *top_user);
TopUser *json_to_top_user(json *j);
std::string get_leaderboard_string(LinkedList *leaderboard);
void json_to_leaderboard_list(std::string str, LinkedList *leaderboard);

std::string json_to_string(json j);
json *string_to_json(std::string str);
}  // namespace TSS

#endif
