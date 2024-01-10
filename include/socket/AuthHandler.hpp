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
  std::mutex *user_mutex;

  LinkedList *users;
  LinkedList *online_users;
  char saved_account_path[256] = "../.data/account.txt";

  static int compare(void *a, void *b);
  void load_users();
  void print_users();
  void update_auth_status(std::string founed_username, bool is_login);

 public:
  AuthHandler(std::mutex *user_mutex, LinkedList *users,
              LinkedList *online_users);
  // return
  // -1: User not found, 1: User found, 0: Password incorrect, 2: User is login
  int check_user(std::string username, std::string password, int client_socket);
  // return
  // 0: Username already in use, 1: Register success
  int register_user(std::string username, std::string password,
                    int client_socket);
  void logout(std::string username, int client_socket);

  // Add to online users
  void add_online_user(std::string username, int client_socket);
  // Remove user from online users list
  void remove_online_user(std::string username);
  // Update user status when join/leave room
  void update_online_user(std::string username, bool is_join_room);
};
}  // namespace TSS

#endif
