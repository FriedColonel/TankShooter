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
  leaderboard = NULL;

  is_training = false;
  is_game_paused = false;
}

TSS::Client::~Client() {
  logout();
  // sleep thread to wait for server response
  sleep(1);
  leave_room();
  close(get_sock());
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

void TSS::Client::logout() {
  if (username == "") return;

  std::string send_message = "auth:logout:" + username;

  sender(send_message);
}

void TSS::Client::create_room(int map) {
  printf("Username: %s\n", username.c_str());

  std::string send_message =
      "room:create_room:" + username + ":" + std::to_string(map);

  sender(send_message);
}

void TSS::Client::join_room(std::string room_id) {
  std::string send_message = "room:join_room:" + username + ":" + room_id;

  sender(send_message);
}

void TSS::Client::leave_room() {
  if (currentRoom == NULL) return;

  std::string send_message =
      "room:leave_room:" + username + ":" + currentRoom->room_id;

  this->currentRoom = NULL;

  sender(send_message);
}

void TSS::Client::get_rooms() {
  char send_message[256] = "room:get_rooms";

  sender(send_message);
}

void TSS::Client::find_room(std::string room_id) {
  std::string send_message = "room:find_room:" + room_id;

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
  if (is_training) return;

  std::string send_message = "game:shoot:" + currentRoom->room_id + ":" +
                             username + ":" + std::to_string(x) + ":" +
                             std::to_string(y) + ":" +
                             std::to_string(direction);

  sender(send_message);
}

void TSS::Client::move_start(float x, float y, int direction) {
  if (is_training) return;

  std::string send_message = "game:move:start:" + currentRoom->room_id + ":" +
                             username + ":" + std::to_string(x) + ":" +
                             std::to_string(y) + ":" +
                             std::to_string(direction);

  sender(send_message);
}

void TSS::Client::move_stop(float x, float y) {
  if (is_training) return;

  std::string send_message = "game:move:stop:" + currentRoom->room_id + ":" +
                             username + ":" + std::to_string(x) + ":" +
                             std::to_string(y);

  sender(send_message);
}

void TSS::Client::pause_game() {
  if (is_training) {
    is_game_paused = true;
    return;
  }

  std::string send_message = "game:pause:" + currentRoom->room_id;

  sender(send_message);
}

void TSS::Client::resume_game() {
  if (is_training) {
    is_game_paused = false;
    return;
  }

  std::string send_message = "game:resume:" + currentRoom->room_id;

  sender(send_message);
}

void TSS::Client::player_dead() {
  if (is_training) {
    alive_players--;
    return;
  };

  std::string send_message =
      "game:player_dead:" + currentRoom->room_id + ":" + username;

  sender(send_message);
}

void TSS::Client::game_end() {
  if (is_training) return;

  std::string send_message = "game:game_end:" + currentRoom->room_id;

  sender(send_message);
}

void TSS::Client::get_leaderboard() {
  std::string send_message = "leaderboard:get";

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
}

void TSS::Client::register_user() {
  char username[256];
  char password[256];
  char confirm_password[256];

  do {
    std::cout << "Username: ";
    std::cin.clear();
    std::cin.getline(username, 256);

    do {
      std::cout << "Password: ";
      strcpy(password, get_password().c_str());

      std::cout << "Confirm password: ";
      strcpy(confirm_password, get_password().c_str());

      if (strcmp(password, confirm_password) != 0)
        std::cout << "Password and confirm password not match" << std::endl;
    } while (strcmp(password, confirm_password) != 0);
  } while (!register_user(username, password));
}

void TSS::Client::auth_menu() {
  menu auth_menu;
  int choice;
  auth_menu.menu_head("Tank Shooter");
  auth_menu.add("Login", 1, "Login to your account");
  auth_menu.add("Register", 2, "Create a new account");
  auth_menu.add("Exit", 3, "Exit the game");

  while (true) {
    choice = auth_menu.display();
    switch (choice) {
      case 1:
        login();
        return;
        break;
      case 2:
        register_user();
        return;
        break;
      case 3:
        delete this;
        exit(0);
        break;
      default:
        break;
    }
  }
}

// Private function
bool TSS::Client::login(char *userName, char *password) {
  std::string send_msg =
      "auth:login:" + std::string(userName) + ":" + std::string(password);

  sender(send_msg);

  std::string recv_msg = receiver();

  if (recv_msg == "auth:login:success") {
    std::cout << "Login successfully! Enjoy game..." << std::endl;
    this->username = std::string(userName);
    return true;
  }

  std::cout << recv_msg << std::endl;

  return false;
}

bool TSS::Client::register_user(char *username, char *password) {
  std::string send_msg =
      "auth:register:" + std::string(username) + ":" + std::string(password);

  sender(send_msg);

  std::string recv_msg = receiver();

  if (recv_msg == "auth:register:success") {
    std::cout << "Register successfully! Enjoy game..." << std::endl;
    this->username = std::string(username);
    return true;
  }

  std::cout << recv_msg << std::endl;

  return false;
}

// Getter function
TSS::Room *TSS::Client::get_current_room() { return currentRoom; }

// Setter function
void TSS::Client::set_current_room(Room *room) { currentRoom = room; }
