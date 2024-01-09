#ifndef GameHandler_hpp
#define GameHandler_hpp

#include <stdio.h>
#include <string.h>

#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>

#include "Helper.hpp"
#include "tss-data-structure.hpp"

using json = nlohmann::json;
using std::to_string;

namespace TSS {
class GameHandler {
 private:
  LinkedList *rooms;

  static int compare(void *a, void *b);

 public:
  // contructor
  GameHandler();
  // create new room | return room_id
  std::string create_room(std::string username, int map, int req_player_socket);
  std::string join_room(std::string username, std::string room_id,
                        int req_player_socket);
  std::string find_room(std::string room_id);
  std::string leave_room(std::string username, std::string room_id);
  std::string ready(std::string username, std::string room_id);
  std::string unready(std::string username, std::string room_id);
  std::string start_game(std::string room_id);
  std::string pause_game(std::string room_id);
  std::string resume_game(std::string room_id);
  std::string player_die(std::string username, std::string room_id);

  // getters function
  std::string get_rooms_list();
  Room *get_room(std::string room_id);
};
}  // namespace TSS

#endif
