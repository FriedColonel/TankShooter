#ifndef AuthHandler_hpp
#define AuthHandler_hpp

#include <string.h>

#include <mutex>
#include <sstream>
#include <vector>

#include "File.hpp"
#include "tss-data-structure.hpp"

namespace TSS {
class AuthHandler {
 private:
  LinkedList *users;
  char saved_account_path[256] = "../account.txt";

  static int compare(void *a, void *b);
  void load_users();
  void print_users();
  void update_auth_status(std::string founed_username, bool is_login);

 public:
  AuthHandler();
  int check_user(std::string username, std::string password);
  void logout(std::string username);
};
}  // namespace TSS

#endif
