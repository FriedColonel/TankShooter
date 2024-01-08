#ifndef Helper_hpp
#define Helper_hpp

// #include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace TSS {
// Enumerator
enum IN {
  // 13 is ASCII for carriage
  // return
  IN_BACK = 8,
  IN_RET = 13

};

int getch();

// Random string generator
char *generate_random_string(size_t length);
// Get password from console
std::string get_password();

// XOR Encryption
std::string xor_encrypt(std::string data, const char *key);
// XOR Decryption
std::string xor_decrypt(std::string message, const char *key);
std::string hex_to_string(std::string input);
}  // namespace TSS

#endif
