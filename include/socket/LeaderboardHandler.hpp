#ifndef LeaderboardHandler_hpp
#define LeaderboardHandler_hpp

#include <stdio.h>
#include <string.h>

#include <mutex>
#include <sstream>
#include <vector>

#include "File.hpp"
#include "Helper.hpp"
#include "tss-data-structure.hpp"

namespace TSS {
class LeaderboardHandler {
 private:
  std::mutex *user_mutex;

  LinkedList *users;
  LinkedList *leaderboard;

  std::string get_archivement_prefix = "../.data/";

  static int compare(void *a, void *b);

 public:
  // Constructor
  LeaderboardHandler(std::mutex *user_mutex, LinkedList *users);
  // Get leaderboard
  std::string get_leaderboard(LinkedList *users);

  // compare function for sorting leaderboard by points
};
}  // namespace TSS

#endif
