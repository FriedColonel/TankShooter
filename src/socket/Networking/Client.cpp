#include <socket/Client.hpp>

TSS::Client *TSS::Client::sInstance = NULL;

TSS::Client *TSS::Client::Instance(int port, char *interface) {
  if (sInstance == NULL)
    sInstance = new Client(AF_INET, SOCK_STREAM, 0, port, interface);

  return sInstance;
}

void TSS::Client::Release() {
  delete sInstance;
  sInstance = NULL;
}

TSS::Client::Client(int domain, int service, int protocol, int port,
                    char *interface)
    : SocketClient(domain, service, protocol, port, interface) {
  currentRoom = "";
}

TSS::Client::~Client() {
  close(get_sock());
  delete sInstance;
  sInstance = NULL;
}

// Send message to server
void TSS::Client::sender(char *msg, size_t msg_length) {
  size_t bytes_sent = send(get_sock(), msg, msg_length, 0);

  if (bytes_sent < 0) {
    perror("Error when sending message");
    close(get_sock());
    exit(EXIT_FAILURE);
  }
}

void TSS::Client::handler() {
  std::cout << "Message from server: " << buffer << std::endl;
}

void TSS::Client::responder() {
  char buffer[256] = "Hello from client";
  write(get_sock(), buffer, strlen(buffer));
}

char *TSS::Client::receiver() {
  size_t bytes_received = recv(get_sock(), buffer, sizeof(buffer), 0);

  if (bytes_received <= 0) {
    std::cout << "Connection closed on socket " << get_sock() << std::endl;
    close(get_sock());
    exit(EXIT_FAILURE);
  }

  buffer[bytes_received] = '\0';

  return buffer;
}

void TSS::Client::close_socket() { close(get_sock()); }

// Event handler
bool TSS::Client::login(char *username, char *password) {
  char send_message[256];

  strcpy(send_message, "auth:login:");
  strcat(send_message, username);
  strcat(send_message, ":");
  strcat(send_message, password);

  sender(send_message, sizeof(send_message));

  if (strcmp("auth:login:success", receiver()) == 0) {
    return true;
  }

  std::cout << "Login failed" << std::endl;

  return false;
}

void TSS::Client::create_room(char *username, int map) {
  char send_message[256];

  strcpy(send_message, "game:create_room:");
  strcat(send_message, username);
  strcat(send_message, ":");
  strcat(send_message, std::to_string(map).c_str());

  sender(send_message, sizeof(send_message));

  char *response = receiver();

  std::cout << "Msg from server: " << receiver() << std::endl;
}

void TSS::Client::join_room(char *username, char *room_id) {
  char send_message[256];

  strcpy(send_message, "game:join_room:");
  strcat(send_message, username);
  strcat(send_message, ":");
  strcat(send_message, room_id);

  sender(send_message, sizeof(send_message));

  std::cout << "Msg from server: " << receiver() << std::endl;
}

void TSS::Client::login() {
  char username[256];
  char password[256];

  do {
    std::cout << "Username: ";
    std::cin.clear();
    std::cin.getline(username, 256);

    std::cout << "Password: ";
    std::cin.clear();
    std::cin.getline(password, 256);
  } while (!sInstance->login(username, password));

  std::cout << "Login success" << std::endl;
}
