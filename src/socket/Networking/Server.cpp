#include <socket/Server.hpp>

TSS::Server::Server(int domain, int service, int protocol, int port,
                    u_long interface, int backlog)
    : SocketServer(domain, service, protocol, port, interface, backlog) {
  auth_handler = new AuthHandler();
  game_handler = new GameHandler();

  // Initialize current set
  FD_ZERO(&current_sockets);
  FD_SET(get_sock(), &current_sockets);
  max_fd = get_sock();
}

int TSS::Server::accepter() {
  int addrlen = sizeof(address);
  int client_socket =
      accept(get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
  check_connection(client_socket);

  std::cout << "Got connection from " << inet_ntoa(address.sin_addr) << ":"
            << ntohs(address.sin_port) << std::endl;

  return client_socket;
}

void TSS::Server::broadcast(std::string msg) {
  for (int i = 0; i <= max_fd; i++) {
    if (i != get_sock()) {
      responder(i, msg);
    }
  }
}

// Return -1 if connection closed
int TSS::Server::handler(int client_socket) {
  char buffer[256] = {0};
  size_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

  if (bytes_received <= 0) {
    std::cout << "Connection closed on socket " << client_socket << std::endl;
    close(client_socket);
    return -1;
  }

  buffer[bytes_received] = '\0';

  std::cout << "Received message: " << buffer << std::endl;

  char *action = strtok(buffer, ":");

  if (strcmp(action, "auth") == 0) {
    handle_auth(client_socket);
  }

  if (strcmp(action, "game") == 0) {
    handle_game(client_socket);
  }

  return 1;
}

void TSS::Server::responder(int client_socket, std::string msg) {
  size_t bytes_sent = send(client_socket, msg.c_str(), msg.size(), 0);

  if (bytes_sent < 0) {
    perror("Error when sending message");
    close(client_socket);
    exit(EXIT_FAILURE);
  }
}

void TSS::Server::launch() {
  while (true) {
    std::cout << "Waiting for client..." << std::endl;

    ready_sockets = current_sockets;

    int activity = select(max_fd + 1, &ready_sockets, NULL, NULL, NULL);
    check_connection(activity);

    for (int i = 0; i <= max_fd; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == get_sock()) {
          // Accept new connection
          int client_socket = accepter();
          FD_SET(client_socket, &current_sockets);
          if (client_socket > max_fd) {
            max_fd = client_socket;
          }
        } else {
          // Handle connection event
          if (handler(i) == -1) {
            FD_CLR(i, &current_sockets);
          }
        }
      }
    }
  }
}

void TSS::Server::handle_auth(int client_socket) {
  char *action = strtok(NULL, ":");
  char *username = strtok(NULL, ":");
  char *password = strtok(NULL, ":");

  if (strcmp(action, "login") == 0) {
    int result = auth_handler->check_user(username, password);

    char response_msg[256] = {0};

    if (result == 1) {
      strcpy(response_msg, "auth:login:success");
    } else if (result == 0) {
      strcpy(response_msg, "auth:login:password_incorrect");
    } else if (result == 2) {
      strcpy(response_msg, "auth:login:already_login");
    } else {
      strcpy(response_msg, "auth:login:user_not_found");
    }

    responder(client_socket, response_msg);
  }
}

void TSS::Server::handle_game(int client_socket) {
  char *action = strtok(NULL, ":");

  if (strcmp(action, "create_room") == 0) {
    char *username = strtok(NULL, ":");
    char *map = strtok(NULL, ":");

    std::string room = game_handler->create_room(username, atoi(map));

    std::string response_msg = "game:create_room\n" + room;

    responder(client_socket, response_msg);
  }

  if (strcmp(action, "join_room") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    std::string joined_room = game_handler->join_room(username, room_id);

    if (joined_room == "") {
      joined_room = "game:join_room:failed";
    } else {
      joined_room = "game:join_room:success\n" + joined_room;
    }

    broadcast(joined_room);
  }

  if (strcmp(action, "find_room") == 0) {
    char *room_id = strtok(NULL, ":");

    std::string founded_room = game_handler->find_room(room_id);
    if (founded_room == "") {
      founded_room = "game:find_room:failed";
    } else {
      founded_room = "game:find_room:success:" + founded_room;
    }

    responder(client_socket, founded_room);
  }

  if (strcmp(action, "leave_room") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    char response_msg[256] = {0};
    if (game_handler->leave_room(username, room_id)) {
      strcpy(response_msg, "game:leave_room:success");
    } else {
      strcpy(response_msg, "game:leave_room:failed");
    }

    responder(client_socket, response_msg);
  }

  if (strcmp(action, "ready") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    char response_msg[256] = {0};
    if (game_handler->ready(username, room_id)) {
      strcpy(response_msg, "game:ready:success");
    } else {
      strcpy(response_msg, "game:ready:failed");
    }

    responder(client_socket, response_msg);
  }

  if (strcmp(action, "unready") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    char response_msg[256] = {0};
    if (game_handler->unready(username, room_id)) {
      strcpy(response_msg, "game:unready:success");
    } else {
      strcpy(response_msg, "game:unready:failed");
    }

    responder(client_socket, response_msg);
  }
}
