#include <socket/Server.hpp>

TSS::Server::Server(int domain, int service, int protocol, int port,
                    u_long interface, int backlog)
    : SocketServer(domain, service, protocol, port, interface, backlog) {
  auth_handler = new AuthHandler();
  game_handler = new GameHandler();
  online_users = new LinkedList();

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
  for (int i = 0; i < online_users->length; i++) {
    OnlineUser *online_user = (OnlineUser *)online_users->retrieve(i);
    if (online_user->is_join_room) {
      continue;
    }

    responder(online_user->client_socket, msg);
  }
}

void TSS::Server::broadcast_to_room(int client_socket, std::string msg,
                                    std::string room_id,
                                    bool is_send_to_sender = true) {
  Room *room = game_handler->get_room(room_id);

  if (room == NULL) {
    responder(client_socket, "game:failed:room_not_found");
    return;
  }

  for (int i = 0; i < room->players.size(); i++) {
    if (!is_send_to_sender && room->players[i].client_socket == client_socket) {
      continue;
    }

    responder(room->players[i].client_socket, msg);
  }
}

void TSS::Server::handler(int client_socket) {
  while (true) {
    char buffer[256] = {0};
    size_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
      std::cout << "Connection closed on socket " << client_socket << std::endl;
      close(client_socket);
      break;
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
  }
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

    // Accept new connection
    int client_socket = accepter();

    // Create new thread to handle client event
    std::thread client_thread(&Server::handler, this, client_socket);

    // Detach thread
    client_thread.detach();
  }
}

void TSS::Server::handle_auth(int client_socket) {
  char *action = strtok(NULL, ":");

  if (strcmp(action, "login") == 0) {
    char *username = strtok(NULL, ":");
    char *password = strtok(NULL, ":");

    int result =
        auth_handler->check_user(std::string(username), std::string(password));

    std::string response_msg;

    if (result == 1) {
      add_online_user(std::string(username), client_socket);

      response_msg = "auth:login:success";
    } else if (result == 0) {
      response_msg = "Error: Password is incorrect";
    } else if (result == 2) {
      response_msg = "Error: User is already logged in";
    } else {
      response_msg = "Error: User not found";
    }

    responder(client_socket, response_msg);
  }

  if (strcmp(action, "register") == 0) {
    char *username = strtok(NULL, ":");
    char *password = strtok(NULL, ":");

    int result = auth_handler->register_user(std::string(username),
                                             std::string(password));

    std::string response_msg;

    if (result == 1) {
      add_online_user(std::string(username), client_socket);
      response_msg = "auth:register:success";
    } else if (result == 0) {
      response_msg = "Error: Username already in use";
    } else {
      response_msg = "Error: Unknown error";
    }

    responder(client_socket, response_msg);
  }

  if (strcmp(action, "logout") == 0) {
    char *username = strtok(NULL, ":");

    remove_online_user(std::string(username));

    auth_handler->logout(std::string(username));

    std::string response_msg = "auth:logout:success";

    responder(client_socket, response_msg);
  }
}

void TSS::Server::handle_game(int client_socket) {
  char *action = strtok(NULL, ":");

  if (strcmp(action, "create_room") == 0) {
    char *username = strtok(NULL, ":");
    char *map = strtok(NULL, ":");

    std::string room =
        game_handler->create_room(username, atoi(map), client_socket);

    update_online_user(std::string(username), true);

    std::string response_msg = "game:create_room:success\n" + room;

    // Broadcast to all clients that a new room has been created
    broadcast("game:get_rooms:success\n" + game_handler->get_rooms_list());

    responder(client_socket, response_msg);
  }

  if (strcmp(action, "join_room") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    std::string result = make_response(
        "game:join_room",
        game_handler->join_room(username, room_id, client_socket));

    update_online_user(std::string(username), true);

    broadcast_to_room(client_socket, result, std::string(room_id));
  }

  if (strcmp(action, "find_room") == 0) {
    char *room_id = strtok(NULL, ":");

    std::string result =
        make_response("game:find_room", game_handler->find_room(room_id));

    responder(client_socket, result);
  }

  if (strcmp(action, "get_rooms") == 0) {
    std::string result =
        make_response("game:get_rooms", game_handler->get_rooms_list());

    responder(client_socket, result);
  }

  if (strcmp(action, "leave_room") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    std::string result = game_handler->leave_room(username, room_id);

    if (result != "") update_online_user(std::string(username), false);

    if (result == "room_empty") {
      broadcast("game:get_rooms:success\n" + game_handler->get_rooms_list());
      return;
    }

    std::string response_msg = make_response("game:leave_room", result);

    broadcast_to_room(client_socket, result, std::string(room_id), false);
  }

  if (strcmp(action, "ready") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    std::string result =
        make_response("game:ready", game_handler->ready(username, room_id));

    broadcast_to_room(client_socket, result, std::string(room_id));
  }

  if (strcmp(action, "unready") == 0) {
    char *username = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");

    std::string result =
        make_response("game:unready", game_handler->unready(username, room_id));

    broadcast_to_room(client_socket, result, std::string(room_id));
  }

  if (strcmp(action, "start") == 0) {
    char *room_id = strtok(NULL, ":");

    std::string result =
        make_response("game:start", game_handler->start_game(room_id));

    broadcast_to_room(client_socket, result, std::string(room_id));
  }

  if (strcmp(action, "shoot") == 0) {
    char *room_id = strtok(NULL, ":");
    char *username = strtok(NULL, ":");
    char *pos_x = strtok(NULL, ":");
    char *pos_y = strtok(NULL, ":");
    char *direction = strtok(NULL, ":");

    std::string result = "game:shoot\n" + std::string(username) + ":" +
                         std::string(pos_x) + ":" + std::string(pos_y) + ":" +
                         std::string(direction);

    broadcast_to_room(client_socket, result, std::string(room_id), false);
  }

  if (strcmp(action, "move") == 0) {
    char *status = strtok(NULL, ":");
    char *room_id = strtok(NULL, ":");
    char *username = strtok(NULL, ":");
    char *pos_x = strtok(NULL, ":");
    char *pos_y = strtok(NULL, ":");
    std::string result = std::string(username) + ":" + std::string(pos_x) +
                         ":" + std::string(pos_y);

    if (strcmp(status, "start") == 0) {
      char *direction = strtok(NULL, ":");

      result = "game:move:start\n" + result + ":" + std::string(direction);
    }

    if (strcmp(status, "stop") == 0) {
      result = "game:move:stop\n" + result;
    }

    broadcast_to_room(client_socket, result, std::string(room_id), false);
  }
}

std::string TSS::Server::make_response(std::string event_name,
                                       std::string data) {
  if (data == "") {
    return event_name + ":failed";
  }

  return event_name + ":success\n" + data;
}

void TSS::Server::add_online_user(std::string username, int client_socket) {
  OnlineUser *new_online_user = new OnlineUser();
  new_online_user->username = std::string(username);
  new_online_user->client_socket = client_socket;
  new_online_user->is_join_room = false;
  online_users->insert(0, new_online_user, sizeof(struct OnlineUser));
}

void TSS::Server::remove_online_user(std::string username) {
  for (int i = 0; i < online_users->length; i++) {
    OnlineUser *online_user = (OnlineUser *)online_users->retrieve(i);
    if (online_user->username == username) {
      online_users->remove(i);
      break;
    }
  }
}

void TSS::Server::update_online_user(std::string username, bool is_join_room) {
  for (int i = 0; i < online_users->length; i++) {
    OnlineUser *online_user = (OnlineUser *)online_users->retrieve(i);
    if (online_user->username == username) {
      online_user->is_join_room = is_join_room;
      break;
    }
  }
}
