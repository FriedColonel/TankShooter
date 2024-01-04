#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <vector>

#include "tss-handler.hpp"
#include "tss-socket.hpp"

namespace TSS {
class Server : public SocketServer {
 private:
  char buffer[256] = {0};
  int max_fd;
  fd_set current_sockets, ready_sockets;

  AuthHandler *auth_handler;
  GameHandler *game_handler;

  int accepter();
  int handler(int client_socket);

 public:
  // Constructor
  Server(int domain, int service, int protocol, int port, u_long interface,
         int backlog);
  // Start server
  void launch();
  void responder(int clent_socket, char *msg, size_t msg_length);
  // Broadcast message to all clients
  void broadcast(char *msg, size_t msg_length);
  // Handle client events
  void handle_auth(int client_socket);
  // Handle game events
  void handle_game(int client_socket);
};
}  // namespace TSS

#endif
