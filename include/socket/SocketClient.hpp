#ifndef SocketClient_hpp
#define SocketClient_hpp

#include <stdio.h>

#include "Socket.hpp"

namespace TSS {
class SocketClient : public Socket {
 public:
  // Constructor
  SocketClient(int domain, int service, int protocol, int port,
               char *interface);
  // Config address
  void config_address(int domain, int service, int protocol, int port,
                      char *interface);
  // Connect socket to network
  int connect_to_network(int sock, struct sockaddr_in address);
};
}  // namespace TSS

#endif
