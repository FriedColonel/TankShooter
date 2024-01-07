#ifndef Helper_hpp
#define Helper_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

namespace TSS {
// Enumerator
enum IN {
  // 13 is ASCII for carriage
  // return
  IN_BACK = 8,
  IN_RET = 13

};

int getch();

char *generate_random_string(size_t length);
std::string get_password();
}  // namespace TSS

#endif
