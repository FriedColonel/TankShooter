#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace TSS {
class Node {
 private:
 public:
  void *data;
  struct Node *next;
  struct Node *previous;

  // Constructor
  Node(void *data, unsigned long size);
  // Destructor
  ~Node();
};
}  // namespace TSS

#endif
