#include <socket/SocketClient.hpp>

TSS::SocketClient::SocketClient(int domain, int service, int protocol, int port,
                                char *interface)
    : Socket(domain, service, protocol) {
  // Configure server address
  config_address(domain, service, protocol, port, interface);

  // Connect socket to network
  connect_to_network(get_sock(), get_address());
  check_connection(get_connection());

  std::cout << "Connected to server" << std::endl;
}

int TSS::SocketClient::connect_to_network(int sock,
                                          struct sockaddr_in address) {
  return connect(sock, (struct sockaddr *)&address, sizeof(address));
}

// Configure server address
void TSS::SocketClient::config_address(int domain, int service, int protocol,
                                       int port, char *interface) {
  bzero(&address, sizeof(address));
  address.sin_family = domain;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = inet_addr(interface);
}
