#include <socket/AuthHandler.hpp>

std::mutex user_mutex;

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

    User *user = new User();
    user->username = username;
    user->password = password;
    user->is_login = (is_login == "1") ? true : false;

    users->insert(0, user, sizeof(struct User));
  }
}

void TSS::AuthHandler::update_auth_status(std::string founed_username,
                                          bool is_login) {
  load_users();

  std::string saved_users;

  std::cout << "Update auth status: " << users->length << std::endl;

  // Save users to file
  while (users->length > 0) {
    struct User *remove_user = (struct User *)users->retrieve(0);

    bool save_is_login = (remove_user->username == founed_username)
                             ? is_login
                             : remove_user->is_login;

    saved_users = saved_users + std::string(remove_user->username) + " " +
                  std::string(remove_user->password) + " " +
                  std::to_string(save_is_login) + "\n";

    users->remove(0);
  }

  TSS::write_file(saved_account_path, (void *)saved_users.c_str(),
                  saved_users.length());
}

int TSS::AuthHandler::check_user(std::string username, std::string password) {
  std::lock_guard<std::mutex> lock(user_mutex);
  load_users();

  User *found_user =
      static_cast<User *>(users->search((char *)username.c_str(), compare));

  if (found_user != NULL) {
    if (found_user->is_login) {
      return 2;
    }

    if (xor_decrypt(found_user->password, SECRET_KEY) == password) {
      update_auth_status(found_user->username, true);
      return 1;
    } else {
      return 0;
    }
  }

  return -1;
}

int TSS::AuthHandler::register_user(std::string username,
                                    std::string password) {
  std::lock_guard<std::mutex> lock(user_mutex);
  load_users();

  User *found_user =
      static_cast<User *>(users->search((char *)username.c_str(), compare));

  if (found_user != NULL) {
    return 0;
  }

  User *new_user = new User();
  new_user->username = username;
  new_user->password = password;
  new_user->is_login = true;

  users->insert(0, new_user, sizeof(struct User));

  // Login after register & save users to file
  std::string saved_new_users = std::string(new_user->username) + " " +
                                xor_encrypt(password, SECRET_KEY) + " 1\n";

  std::cout << "Register user: " << saved_new_users << std::endl;

  TSS::append_file(saved_account_path, (void *)saved_new_users.c_str(),
                   saved_new_users.length());

  return 1;
}

void TSS::AuthHandler::logout(std::string username) {
  std::lock_guard<std::mutex> lock(user_mutex);
  update_auth_status(username, false);
}

int TSS::AuthHandler::compare(void *a, void *b) {
  struct User *user = (struct User *)a;
  char *username = (char *)b;

  return user->username == std::string(username);
}

// print user list store in linked list
void TSS::AuthHandler::print_users() {
  for (int i = 0; i < users->length; i++) {
    struct User *user = (struct User *)users->retrieve(i);
    std::cout << "User: " << user->username << " " << user->password << " "
              << user->is_login << std::endl;
  }
}
