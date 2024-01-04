#include <socket/Client.hpp>

TSS::Client *TSS::Client::sInstance = NULL;

TSS::Client *TSS::Client::Instance() { return sInstance; }

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
  currentRoom = NULL;
}

TSS::Client::~Client() {
  close(get_sock());
  delete sInstance;
  sInstance = NULL;
}

// Send message to server
void TSS::Client::sender(std::string msg) {
  size_t bytes_sent = send(get_sock(), msg.c_str(), msg.size(), 0);

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

std::string TSS::Client::receiver() {
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
bool TSS::Client::login(char *userName, char *password) {
  char send_message[256];

  strcpy(send_message, "auth:login:");
  strcat(send_message, userName);
  strcat(send_message, ":");
  strcat(send_message, password);

  sender(send_message);

  if (receiver() == "auth:login:success") {
    this->username = std::string(userName);
    return true;
  }

  std::cout << "Login failed" << std::endl;

  return false;
}

void TSS::Client::create_room(int map) {
  printf("Username: %s\n", username.c_str());

  std::string send_message =
      "game:create_room:" + username + ":" + std::to_string(map);

  // printf("%s\n", send_message.c_str());

  sender(send_message);
}

void TSS::Client::join_room(std::string room_id) {
  std::string send_message = "game:join_room:" + username + ":" + room_id;

  sender(send_message);
}

void TSS::Client::get_rooms() {
  char send_message[256] = "game:get_rooms";

  sender(send_message);
}

void TSS::Client::find_room(std::string room_id) {
  std::string send_message = "game:find_room:" + room_id;

  sender(send_message);
}

void TSS::Client::ready() {
  std::string send_message = "game:ready:" + username;

  sender(send_message);
}

void TSS::Client::unready() {
  std::string send_message = "game:unready:" + username;

  sender(send_message);
}

void TSS::Client::start_game() {
  std::string send_message = "game:start";

  sender(send_message);
}

void TSS::Client::shot_bullet(float x, float y, int direction) {
  std::string send_message = "game:shot:" + username + std::to_string(x) + ":" +
                             std::to_string(y) + ":" +
                             std::to_string(direction);

  sender(send_message);
}

void TSS::Client::move_start(float x, float y, int direction) {
  std::string send_message = "game:move:start:" + username + std::to_string(x) +
                             ":" + std::to_string(y) + ":" +
                             std::to_string(direction);

  sender(send_message);
}

void TSS::Client::move_stop(float x, float y) {
  std::string send_message = "game:move:stop:" + username + std::to_string(x) +
                             ":" + std::to_string(y);

  sender(send_message);
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
  } while (!login(username, password));

  std::cout << "Login success" << std::endl;
}

// Getter function
TSS::Room *TSS::Client::get_current_room() { return currentRoom; }

// Setter function
void TSS::Client::set_current_room(Room *room) { currentRoom = room; }
