#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "tss-socket.hpp"

namespace TSS {
class Client : public SocketClient {
 private:
  static Client *sInstance;

  char buffer[256] = {0};

  void handler();
  void responder();

  std::string username;

  std::string currentRoom;

 private:
  bool login(char *username, char *password);

 public:
  static Client *Instance(int port, char *interface);
  static void Release();

  // Constructor
  Client(int domain, int service, int protocol, int port, char *interface);
  ~Client();

  void sender(char *msg, size_t msg_length);
  char *receiver();

  // Event handler
  void create_room(char *username, int map);
  void join_room(char *username, char *room_id);

  void close_socket();

  void login();
};
}  // namespace TSS

#endif
