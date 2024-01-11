#ifndef Client_hpp
#define Client_hpp

// #include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <map>
#include <nlohmann/json.hpp>

#include "Helper.hpp"
#include "Mmaker.hpp"
#include "tss-socket.hpp"

using json = nlohmann::json;
using std::to_string;

namespace TSS {
class Client : public SocketClient {
 private:
  static Client *sInstance;

  std::string username;
  char buffer[256] = {0};
  Room *currentRoom;

  void handler();
  void responder();

 private:
  bool login(char *username, char *password);
  bool register_user(char *username, char *password);

 public:
  LinkedList *rooms;
  LinkedList *leaderboard;

  bool is_training;
  int alive_players;

  static Client *Instance();
  static Client *Instance(int port, char *interface);
  static void Release();

  // Constructor
  Client(int domain, int service, int protocol, int port, char *interface);
  ~Client();

  void sender(std::string msg);
  std::string receiver();
  void auth_menu();

  // Event handler
  // message format: game:create_room:username:map
  void create_room(int map);
  // message format: game:join_room:username:room_id
  void join_room(std::string room_id);
  // message format: game:leave_room:username:room_id
  void leave_room();
  // message format: game:find_room:room_id
  void get_rooms();
  // message format: game:find_room:room_id
  void find_room(std::string room_id);

  // message format: game:ready:username:room_id
  void ready();
  // message format: game:unready:username:room_id
  void unready();
  // message format: game:start:username:room_id
  void start_game();
  // message format: game:pause:room_id
  void pause_game();
  // message format: game:resume:room_id
  void resume_game();
  // message format: game:player_dead:room_id
  void player_dead();

  // message format: game:shoot:room_id:username:pos_x:pos_y:direction
  void shot_bullet(float x, float y, int direction);
  // message format: game:move:start:room_id:username:pos_x:pos_y:direction
  void move_start(float x, float y, int direction);
  // message format: game:move:stop:room_id:username:pos_x:pos_y
  void move_stop(float x, float y);

  // message format: game:game_end:room_id
  void game_end();

  // message format: leaderboard:get
  void get_leaderboard();

  // Auth handler function
  void login();
  void register_user();
  void logout();

  void close_socket();

  // Getter function
  Room *get_current_room();
  std::string get_username() { return username; }

  // Setter function
  void set_current_room(Room *room);
};
}  // namespace TSS

#endif
