#ifndef AuthHandler_hpp
#define AuthHandler_hpp

#include <string.h>

#include <mutex>
#include <sstream>
#include <vector>

#include "File.hpp"
#include "Helper.hpp"
#include "tss-data-structure.hpp"

#define SECRET_KEY "SECRETPASSWORD"

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
  // return
  // -1: User not found, 1: User found, 0: Password incorrect, 2: User is login
  int check_user(std::string username, std::string password);
  // return
  // 0: Username already in use, 1: Register success
  int register_user(std::string username, std::string password);
  void logout(std::string username);
};
}  // namespace TSS

#endif
