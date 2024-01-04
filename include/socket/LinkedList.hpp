#ifndef LinkedList_hpp
#define LinkedList_hpp

#include <stdio.h>

#include <iostream>

#include "Node.hpp"

namespace TSS {
class LinkedList {
 public:
  Node* head;
  int length;

  LinkedList();

  ~LinkedList();

  Node* create_node(void* data, unsigned long size);

  void destroy_node(Node* node);

  struct Node* iterate(int index);

  // Insert adds new items to the chain at a specified location - this function
  // creates the new nodes.
  void insert(int index, void* data, unsigned long size);

  // Remove deletes an item from the chain and handles the deallocation of
  // memory.
  void remove(int index);

  // Retrieve allows data in the chain to be accessed
  void* retrieve(int index);

  // Sorting and searching the list (bubble sort).
  void sort(int (*compare)(void* a, void* b));

  // Binary search (requires the list be sorted).
  void* search(void* query, int (*compare)(void* a, void* b));
};
}  // namespace TSS

#endif
