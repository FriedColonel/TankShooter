#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <nlohmann/json.hpp>

#include "tss-socket.hpp"

using json = nlohmann::json;
using std::to_string;

namespace TSS {
class Client : public SocketClient {
 private:
  static Client *sInstance;

  char buffer[256] = {0};

  void handler();
  void responder();

  std::string username;

  Room *currentRoom;

 private:
  bool login(char *username, char *password);

 public:
  static Client *Instance();
  static Client *Instance(int port, char *interface);
  static void Release();

  // Constructor
  Client(int domain, int service, int protocol, int port, char *interface);
  ~Client();

  void sender(char *msg, size_t msg_length);
  std::string receiver();

  // Event handler
  void create_room(int map);
  void join_room(char *username, char *room_id);

  void close_socket();

  std::string get_username() { return username; }

  void login();

  // Getter function
  Room *get_current_room();

  // Setter function
  void set_current_room(Room *room);
};
}  // namespace TSS

#endif
