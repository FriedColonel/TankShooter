#include <socket/Node.hpp>

TSS::Node::Node(void *data, unsigned long size)
    : data(nullptr), next(nullptr), previous(nullptr) {
  if (size < 1) {
    printf("Invalid data size for node...\n");
    exit(1);
  }

  this->data = malloc(size);
  memcpy(this->data, data, size);
  // Initialize the pointers.
  next = nullptr;
  previous = nullptr;
}

TSS::Node::~Node() {
  free(data);
  next = nullptr;
  previous = nullptr;
}
