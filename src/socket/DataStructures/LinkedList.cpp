#include <socket/LinkedList.hpp>

TSS::LinkedList::LinkedList() : head(nullptr), length(0) {}

TSS::LinkedList::~LinkedList() {
  // Iterate through the list and destroy each node.
  while (head != NULL) {
    struct Node *cursor = head;
    head = head->next;
    destroy_node(cursor);
  }
}

TSS::Node *TSS::LinkedList::create_node(void *data, unsigned long size) {
  return new Node(data, size);
}

void TSS::LinkedList::destroy_node(Node *node) { delete node; }

struct TSS::Node *TSS::LinkedList::iterate(int index) {
  // Check for invalid index.
  if (index < 0 || index >= length) {
    return NULL;
  }

  // Create a pointer to the head.
  Node *currentNode = head;

  // Iterate through the list until the specified index is reached.
  for (int i = 0; i < index; i++) {
    currentNode = currentNode->next;
  }

  return currentNode;
}

void TSS::LinkedList::insert(int index, void *data, unsigned long size) {
  // Create the new node.
  Node *new_node = create_node(data, size);

  if (index == 0) {
    new_node->next = head;
    head = new_node;
  } else {
    // Find the item in the list immediately before the desired index.
    struct Node *cursor = iterate(index - 1);
    // Update next cursor
    new_node->next = cursor->next;
    cursor->next = new_node;
  }

  length++;
}

void TSS::LinkedList::remove(int index) {
  // If the index is 0, remove the head.
  if (index == 0) {
    struct Node *node_to_remove = head;

    if (node_to_remove) {
      head = node_to_remove->next;
      destroy_node(node_to_remove);
    }
  } else {
    // Find the item in the list before the one that is going to be removed.
    struct Node *cursor = iterate(index - 1);
    // Use the cursor to define the node to be removed.
    struct Node *node_to_remove = cursor->next;
    // Update the cursor's next to skip the node to be removed.
    cursor->next = node_to_remove->next;

    // Remove the node.
    destroy_node(node_to_remove);
  }

  length--;
}

void *TSS::LinkedList::retrieve(int index) {
  // Find the desired node and return its data.
  struct Node *cursor = iterate(index);
  if (cursor) {
    return cursor->data;
  } else {
    return NULL;
  }
}

void TSS::LinkedList::sort(int (*compare)(void *a, void *b)) {
  Node *currentNode = head;

  // Iterate through the list until the end is reached.
  while (currentNode->next != NULL) {
    // Create a pointer to the next node.
    Node *nextNode = currentNode->next;

    // Iterate through the list until the end is reached.
    while (nextNode != NULL) {
      // Compare the data in the current node to the data in the next node.
      if (compare(currentNode->data, nextNode->data) > 0) {
        void *temp = currentNode->data;
        currentNode->data = nextNode->data;
        nextNode->data = temp;
      }

      nextNode = nextNode->next;
    }

    currentNode = currentNode->next;
  }
}

void *TSS::LinkedList::search(void *query, int (*compare)(void *a, void *b)) {
  int position = length / 2;
  int min_checked = 0;
  int max_checked = length;
  while (max_checked > min_checked) {
    void *data = retrieve(position);
    if (data == NULL) {
      return NULL;
    }

    if (compare(data, query) == 1) {
      max_checked = position;
      if (position != (min_checked + position) / 2) {
        position = (min_checked + position) / 2;
      } else {
        break;
      }
    } else if (compare(data, query) == -1) {
      min_checked = position;
      if (position != (max_checked + position) / 2) {
        position = (max_checked + position) / 2;
      } else {
        break;
      }
    } else {
      return data;
    }
  }

  return NULL;
}
