#include <socket/AuthHandler.hpp>

TSS::AuthHandler::AuthHandler() { users = new LinkedList(); }

void TSS::AuthHandler::load_users() {
  delete users;
  users = new LinkedList();

  // Load users from file
  std::vector<std::string> lines = TSS::read_file(saved_account_path);

  while (!lines.empty()) {
    std::string line = lines.back();
    lines.pop_back();

    std::istringstream iss(line);
    std::string username, password, is_login;
    iss >> username >> password >> is_login;

    struct User *user = (struct User *)malloc(sizeof(struct User));
    user->username = strdup(username.c_str());
    user->password = strdup(password.c_str());
    user->is_login = (is_login == "1") ? true : false;

    users->insert(0, user, sizeof(struct User));
  }
}

void TSS::AuthHandler::update_auth_status(User *user, bool is_login) {
  load_users();

  // Save users to file
  while (users->length > 0) {
    struct User *remove_user = (struct User *)users->retrieve(0);

    bool save_is_login = (strcmp(remove_user->username, user->username) == 0)
                             ? is_login
                             : remove_user->is_login;

    std::string line = std::string(remove_user->username) + " " +
                       std::string(remove_user->password) + " " +
                       std::to_string(save_is_login) + "\n";
    TSS::append_file(saved_account_path, (void *)line.c_str(), line.length());

    users->remove(0);
  }
}

// return
// -1: User not found, 1: User found, 0: Password incorrect, 2: User is login
int TSS::AuthHandler::check_user(char *username, char *password) {
  load_users();

  User *found_user = static_cast<User *>(users->search(username, compare));

  if (found_user != NULL) {
    if (found_user->is_login) {
      return 2;
    }

    if (strcmp(found_user->password, password) == 0) {
      // update_auth_status(found_user, true);
      return 1;
    } else {
      return 0;
    }
  }

  return -1;
}

int TSS::AuthHandler::compare(void *a, void *b) {
  struct User *user = (struct User *)a;
  char *username = (char *)b;

  if (strcmp(user->username, username) == 0) {
    return 0;
  }

  if (strcmp(user->username, username) > 0) {
    return 1;
  }

  return -1;
}

// print user list store in linked list
void TSS::AuthHandler::print_users() {
  for (int i = 0; i < users->length; i++) {
    struct User *user = (struct User *)users->retrieve(i);
    std::cout << "User: " << user->username << " " << user->password << " "
              << user->is_login << std::endl;
  }
}
