#ifndef AuthHandler_hpp
#define AuthHandler_hpp

#include <string.h>

#include <sstream>
#include <vector>

#include "File.hpp"
#include "tss-data-structure.hpp"

namespace TSS {
struct User {
  char *username;
  char *password;
  bool is_login;
};

class AuthHandler {
 private:
  LinkedList *users;
  char saved_account_path[256] = "../account.txt";

  static int compare(void *a, void *b);
  void load_users();
  void print_users();
  void update_auth_status(User *user, bool is_login);

 public:
  AuthHandler();
  int check_user(char *username, char *password);
};
}  // namespace TSS

#endif
