#ifndef SocketSever_hpp
#define SocketSever_hpp

#include <stdio.h>

#include "Socket.hpp"

namespace TSS {
class SocketServer : public Socket {
 private:
  int backlog;
  int listening;

 public:
  // Constructor
  SocketServer(int domain, int service, int protocol, int port,
               u_long interface, int backlog);
  void config_address(int domain, int service, int protocol, int port,
                      u_long interface);
  // Bind socket to network in Sever
  int connect_to_network(int sock, struct sockaddr_in address);
  void start_listening();
};
}  // namespace TSS

#endif
