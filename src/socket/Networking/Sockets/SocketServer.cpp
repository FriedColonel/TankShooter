#include <socket/SocketServer.hpp>

TSS::SocketServer::SocketServer(int domain, int service, int protocol, int port,
                                u_long interface, int backlog)
    : Socket(domain, service, protocol) {
  this->backlog = backlog;

  // Configure server address
  config_address(domain, service, protocol, port, interface);

  // Bind socket to network
  connect_to_network(get_sock(), get_address());
  check_connection(get_connection());

  // Start listening for connections
  start_listening();
  check_connection(listening);

  std::cout << "Server is listening on port " << port << std::endl;
}

// Bind socket to network in Sever
int TSS::SocketServer::connect_to_network(int sock,
                                          struct sockaddr_in address) {
  return bind(sock, (struct sockaddr *)&address, sizeof(address));
}

// Start listening for connections
void TSS::SocketServer::start_listening() {
  listening = listen(get_sock(), backlog);
}

// Configure server address
void TSS::SocketServer::config_address(int domain, int service, int protocol,
                                       int port, u_long interface) {
  bzero(&address, sizeof(address));
  address.sin_family = domain;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = htonl(interface);
}
