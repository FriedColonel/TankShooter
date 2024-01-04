#ifndef Socket_hpp
#define Socket_hpp

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>

#include <iostream>

namespace TSS {
class Socket {
 private:
  int sock;
  int connection;

 protected:
  struct sockaddr_in address;

 public:
  // Constructor
  Socket(int domain, int service, int protocol);
  // Virtual function to connect to the network
  virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
  // Exit if the connection fails
  void check_connection(int);
  // Getters functions
  struct sockaddr_in get_address();
  int get_sock();
  int get_connection();
  // Setters functions
  void set_connection(int connection);
};
}  // namespace TSS

#endif
