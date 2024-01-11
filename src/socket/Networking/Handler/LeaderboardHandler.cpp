#include <socket/LeaderboardHandler.hpp>

TSS::LeaderboardHandler::LeaderboardHandler(std::mutex *user_mutex,
                                            LinkedList *users) {
  this->user_mutex = user_mutex;
  this->users = users;

  leaderboard = new LinkedList();
}

std::string TSS::LeaderboardHandler::get_leaderboard(LinkedList *users) {
  if (users->length == 0) {
    return "";
  }

  leaderboard = new LinkedList();

  for (int i = 0; i < users->length; i++) {
    TopUser *top_user = new TopUser();

    int total_point = 0;
    User *user = (User *)users->retrieve(i);

    std::string get_point_path =
        get_archivement_prefix + user->username + ".txt";

    std::vector<std::string> lines =
        TSS::read_file((char *)get_point_path.c_str());

    while (!lines.empty()) {
      std::string line = lines.back();
      lines.pop_back();

      std::istringstream iss(line);
      std::string room_id, point;
      iss >> room_id >> point;

      total_point += std::stoi(point);
    }

    top_user->username = user->username;
    top_user->points = total_point;
    top_user->room_id = "null";

    leaderboard->insert(0, top_user, sizeof(struct TopUser));
  }

  leaderboard->sort(compare);

  return get_leaderboard_string(leaderboard);
}

int TSS::LeaderboardHandler::compare(void *a, void *b) {
  TopUser *user_a = (TopUser *)a;
  TopUser *user_b = (TopUser *)b;

  if (user_a->points > user_b->points) {
    return 1;
  } else if (user_a->points < user_b->points) {
    return -1;
  } else {
    return 0;
  }
}
