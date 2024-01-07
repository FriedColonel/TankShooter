#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <thread>
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
  void handler(int client_socket);
  std::string make_response(std::string event_name, std::string data);

 public:
  // Constructor
  Server(int domain, int service, int protocol, int port, u_long interface,
         int backlog);
  // Start server
  void launch();
  void responder(int clent_socket, std::string msg);
  // Broadcast message to all clients
  void broadcast_to_room(int client_socket, std::string msg,
                         std::string room_id);
  // Broadcast message to everyone except the sender
  void to_everyone_else_in_room(int client_socket, std::string msg,
                                std::string room_id);
  // Handle client events
  void handle_auth(int client_socket);
  // Handle game events
  void handle_game(int client_socket);
};
}  // namespace TSS

#endif
