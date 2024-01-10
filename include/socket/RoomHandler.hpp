#ifndef RoomHandler_hpp
#define RoomHandler_hpp

#include <stdio.h>
#include <string.h>

#include <mutex>
#include <sstream>
#include <vector>

#include "Helper.hpp"
#include "tss-data-structure.hpp"

namespace TSS {
class RoomHandler {
 private:
  std::mutex *room_mutex;

  LinkedList *rooms;

  static int compare(void *a, void *b);

 public:
  // contructor
  RoomHandler(std::mutex *room_mutex, LinkedList *rooms);

  // create new room
  std::string create_room(std::string username, int map, int req_player_socket);
  // join room
  std::string join_room(std::string username, std::string room_id,
                        int req_player_socket);
  // find room
  std::string find_room(std::string room_id);
  // leave room
  std::string leave_room(std::string username, std::string room_id);

  // getters function
  std::string get_rooms_list();
  // get a room
  Room *get_room(std::string room_id);
};
}  // namespace TSS

#endif
