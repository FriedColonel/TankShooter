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
  rooms = new LinkedList();

  currentRoom = NULL;
}

TSS::Client::~Client() {
  logout();
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
  std::string send_msg =
      "auth:login:" + std::string(userName) + ":" + std::string(password);

  sender(send_msg);

  std::string recv_msg = receiver();

  if (recv_msg == "auth:login:success") {
    this->username = std::string(userName);
    return true;
  }

  std::cout << recv_msg << std::endl;

  return false;
}

void TSS::Client::logout() {
  if (username == "") return;

  std::string send_message = "auth:logout:" + username;

  sender(send_message);
}

void TSS::Client::create_room(int map) {
  printf("Username: %s\n", username.c_str());

  std::string send_message =
      "game:create_room:" + username + ":" + std::to_string(map);

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
  std::string send_message =
      "game:ready:" + username + ":" + currentRoom->room_id;

  sender(send_message);
}

void TSS::Client::unready() {
  std::string send_message =
      "game:unready:" + username + ":" + currentRoom->room_id;

  sender(send_message);
}

void TSS::Client::start_game() {
  std::string send_message = "game:start:" + currentRoom->room_id;

  sender(send_message);
}

void TSS::Client::shot_bullet(float x, float y, int direction) {
  std::string send_message = "game:shoot:" + currentRoom->room_id + ":" +
                             username + ":" + std::to_string(x) + ":" +
                             std::to_string(y) + ":" +
                             std::to_string(direction);

  sender(send_message);
}

void TSS::Client::move_start(float x, float y, int direction) {
  std::string send_message = "game:move:start:" + currentRoom->room_id + ":" +
                             username + ":" + std::to_string(x) + ":" +
                             std::to_string(y) + ":" +
                             std::to_string(direction);

  sender(send_message);
}

void TSS::Client::move_stop(float x, float y) {
  std::string send_message = "game:move:stop:" + currentRoom->room_id + ":" +
                             username + ":" + std::to_string(x) + ":" +
                             std::to_string(y);

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
    strcpy(password, get_password().c_str());
  } while (!login(username, password));

  std::cout << "\nLogin success" << std::endl;
}

// void TSS::Client::set_player(Player *player, std::string usrName) {
//   players[usrName] = player;
// }

// Getter function
TSS::Room *TSS::Client::get_current_room() { return currentRoom; }
// Player *TSS::Client::get_player(std::string username) {
//   return players[username];
// }

// Setter function
void TSS::Client::set_current_room(Room *room) { currentRoom = room; }
