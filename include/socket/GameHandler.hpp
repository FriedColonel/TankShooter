#ifndef GameHandler_hpp
#define GameHandler_hpp

#include <stdio.h>
#include <string.h>

#include <mutex>
#include <sstream>
#include <vector>

#include "File.hpp"
#include "Helper.hpp"
#include "tss-data-structure.hpp"

namespace TSS {
class GameHandler {
 private:
  std::mutex *room_mutex;

  std::string saved_archivement_prefix = "../.data/";
  LinkedList *rooms;

  static int compare(void *a, void *b);

 public:
  // contructor
  GameHandler(std::mutex *room_mutex, LinkedList *rooms);

  std::string ready(std::string username, std::string room_id);
  std::string unready(std::string username, std::string room_id);
  std::string start_game(std::string room_id);
  std::string pause_game(std::string room_id);
  std::string resume_game(std::string room_id);
  std::string player_dead(std::string username, std::string room_id);

  std::string game_end(std::string room_id);

  // getters function
  Room *get_room(std::string room_id);
};
}  // namespace TSS

#endif
