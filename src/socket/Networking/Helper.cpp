#include <socket/Helper.hpp>

int TSS::getch() {
  int ch;
  // struct to hold the terminal settings
  struct termios old_settings, new_settings;
  // take default setting in old_settings
  tcgetattr(STDIN_FILENO, &old_settings);
  // make of copy of it (Read my previous blog to know
  // more about how to copy struct)
  new_settings = old_settings;
  // change the settings for by disabling ECHO mode
  // read man page of termios.h for more settings info
  new_settings.c_lflag &= ~(ICANON | ECHO);
  // apply these new settings
  tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
  // now take the input in this mode
  ch = getchar();
  // reset back to default settings
  tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
  return ch;
}

char *TSS::generate_random_string(size_t length) {
  char *buffer = (char *)malloc(length + 1);
  char charset[] = "0123456789";

  // Seed the random number generator
  srand(time(NULL));

  for (size_t i = 0; i < length; i++) {
    int random_index = rand() % (sizeof(charset) - 1);
    buffer[i] = charset[random_index];
  }

  buffer[length] = '\0';  // Null-terminate the string

  return buffer;
}

std::string TSS::get_password() {
  char password[100];
  int i = 0;
  int ch;

  while ((ch = getch()) != '\n') {
    if (ch == 127 || ch == 8) {  // handle backspace
      if (i != 0) {
        i--;
        printf("\b \b");
      }
    } else {
      password[i++] = ch;
      // echo the '*'
      printf("*");
    }
  }
  password[i] = '\0';

  return std::string(password);
}
