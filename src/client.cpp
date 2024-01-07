#include <stdio.h>
#include <string.h>

#include <iostream>
#include <socket/Client.hpp>
#include <socket/ThreadPool.hpp>
#include <sstream>

using namespace std;
using namespace TSS;

char username[256];
char password[256];
bool is_login = false;

void *auto_recv_message(void *arg) {
  Client *client = static_cast<Client *>(arg);

  char buffer[1024];

  while (true) {
    memset(buffer, 0, sizeof(buffer));
    size_t bytes_received = recv(client->get_sock(), buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
      cout << "Connection closed on socket " << socket << endl;
      close(client->get_sock());
      break;
    }

    buffer[bytes_received] = '\0';

    cout << "Received message: " << buffer << endl;

    string bufferStr(buffer);
    istringstream iss(bufferStr);
    string event_name, data;

    getline(iss, event_name, '\n');
    getline(iss, data, '\n');

    if (event_name == "game:join_room:success") {
      cout << endl << "Join room success " << data << endl;
      Room *room = json_to_room(string_to_json(data));
      client->set_current_room(room);

      cout << "Current room: " << client->get_current_room()->room_id << endl;

      continue;
    }

    if (event_name == "game:get_rooms:success") {
      cout << endl << "Rooms data" << data << endl;

      json_to_rooms_list(data, client->rooms);

      cout << "Rooms list: " << client->rooms->length << endl;

      continue;
    }
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  Client *client = new Client(AF_INET, SOCK_STREAM, 0, atoi(argv[2]), argv[1]);

  ThreadJob thread_job(&auto_recv_message, (void *)client);

  ThreadPool *thread_pool = new ThreadPool(1);
  thread_pool->add_work(thread_job);

  client->login();

  client->create_room(1);

  client->get_rooms();

  delete thread_pool;
  delete client;
  return 0;
}
