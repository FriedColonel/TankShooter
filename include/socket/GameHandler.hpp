#ifndef GameHandler_hpp
#define GameHandler_hpp

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>

#include "tss-data-structure.hpp"

using json = nlohmann::json;
using std::to_string;

namespace TSS {
class GameHandler {
 private:
  LinkedList *rooms;

  static int compare(void *a, void *b);
  char *generate_random_string(size_t length);

 public:
  // contructor
  GameHandler();
  // create new room | return room_id
  std::string create_room(std::string username, int map);
  std::string join_room(std::string username, char *room_id);
  std::string find_room(char *room_id);
  bool leave_room(std::string username, char *room_id);
  bool ready(std::string username, char *room_id);
  bool unready(std::string username, char *room_id);
};
}  // namespace TSS

#endif
