#include <socket/Socket.hpp>

TSS::Socket::Socket(int domain, int service, int protocol) {
  // Create socket
  sock = socket(domain, service, protocol);
  check_connection(sock);
}

void TSS::Socket::check_connection(int check_item) {
  // Exit if the connection fails
  if (check_item < 0) {
    perror("Error connecting to network");
    exit(EXIT_FAILURE);
  }
}

struct sockaddr_in TSS::Socket::get_address() { return address; }

int TSS::Socket::get_sock() { return sock; }

int TSS::Socket::get_connection() { return connection; }

void TSS::Socket::set_connection(int connection) {
  this->connection = connection;
}
