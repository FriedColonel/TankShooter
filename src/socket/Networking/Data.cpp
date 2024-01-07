#include <socket/Data.hpp>

std::string TSS::json_to_string(json j) { return to_string(j); }

json *TSS::string_to_json(std::string str) {
  json *j = new json();

  *j = json::parse(str);

  return j;
}

json *TSS::room_to_json(Room *room) {
  json *j = new json();

  j->push_back(json::object_t::value_type("room_id", room->room_id));
  j->push_back(json::object_t::value_type("status", room->status));
  j->push_back(json::object_t::value_type("map", room->map));

  json *players = new json();
  for (int i = 0; i < room->players.size(); i++) {
    json *player = new json();
    player->push_back(
        json::object_t::value_type("username", room->players[i].username));
    player->push_back(
        json::object_t::value_type("tank", room->players[i].tank));
    player->push_back(
        json::object_t::value_type("is_leader", room->players[i].is_leader));
    player->push_back(
        json::object_t::value_type("status", room->players[i].status));

    players->push_back(*player);
  }

  j->push_back(json::object_t::value_type("players", *players));

  return j;
}

TSS::Room *TSS::json_to_room(json *j) {
  Room *room = new Room();

  room->room_id = (char *)malloc(6);
  room->room_id = j->at("room_id").get<std::string>();
  room->status = j->at("status").get<int>();
  room->map = j->at("map").get<int>();

  json players = j->at("players");
  for (int i = 0; i < players.size(); i++) {
    Player *player = new Player();

    player->username =
        (char *)malloc(players[i].at("username").get<std::string>().length());
    player->username = players[i].at("username").get<std::string>();
    player->tank = players[i].at("tank").get<int>();
    player->is_leader = players[i].at("is_leader").get<bool>();
    player->status = players[i].at("status").get<int>();

    room->players.push_back(*player);
  }

  return room;
}

std::string TSS::get_rooms_list(LinkedList *rooms) {
  json room_list_json = json::array();

  for (int i = 0; i < rooms->length; i++) {
    Room *room = static_cast<Room *>(rooms->retrieve(i));
    room_list_json.push_back(*room_to_json(room));
  }

  return json_to_string(room_list_json);
}

void TSS::json_to_rooms_list(std::string str, LinkedList *rooms) {
  json *j = string_to_json(str);

  for (int i = 0; i < j->size(); i++) {
    Room *room = json_to_room(&j->at(i));
    rooms->insert(0, room, sizeof(struct Room));
  }
}

json *TSS::user_to_json(User *user) {
  json *j = new json();

  j->push_back(json::object_t::value_type("username", user->username));
  j->push_back(json::object_t::value_type("password", user->password));
  j->push_back(json::object_t::value_type("is_login", user->is_login));

  return j;
}
