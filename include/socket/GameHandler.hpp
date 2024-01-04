#ifndef GameHandler_hpp
#define GameHandler_hpp

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sstream>
#include <vector>

#include "tss-data-structure.hpp"

namespace TSS {
struct Player {
  char *username;
  int tank;  // 1: red, 2: blue, 3:green, 4:white
  bool is_leader;
  int status;  // 0: not ready, 1: ready, 2: out
};

struct Room {
  char *room_id;
  std::vector<Player> players;
  int status;  // 1: waiting, 2: playing, 3: pause, 4: finished
  int map;
};

class GameHandler {
 private:
  LinkedList *rooms;

  static int compare(void *a, void *b);
  char *generate_random_string(size_t length);

 public:
  // contructor
  GameHandler();
  // create new room | return room_id
  char *create_room(char *username, int map);
  bool join_room(char *username, char *room_id);
  Room *find_room(char *room_id);
  bool leave_room(char *username, char *room_id);
  bool ready(char *username, char *room_id);
  bool unready(char *username, char *room_id);
};
}  // namespace TSS

#endif
