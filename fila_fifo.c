#include "fila_fifo.h"

void initialize_queue(Queue **queue) {
  *queue = (Queue*) malloc(sizeof(Queue));  
  (*queue)->first = NULL;
  (*queue)->last = NULL;
  (*queue)->length = 0;
}

int queue_length(Queue **queue) {
  return (*queue)->length;
}

int is_duplicated_key(Queue *queue, int key) {
  Node* ptr;

  for (ptr = queue->first; ptr != NULL; ptr = ptr->next) {
    if (ptr->key == key) {
      return 1;
    }
  }
  return 0;
}

int enqueue(Queue **queue, int key, int value) {
  Node *new_node;
  // The queue was not initialized.
  if (*queue == NULL) return 0;
  // Allocate memory to a new node.
  new_node = (Node*) malloc(sizeof(Node));
  if (new_node == NULL) {
    printf("Failed to allocate memory for a new node.\n");
    return 0;
  }
  // Check if the provided key is duplicated
  if (is_duplicated_key(*queue, key)) return 0;

  new_node->key = key;
  new_node->value = value;
  new_node->next = NULL;

  // Case 1: There are no elements in the queue.
  if(queue_length(queue) == 0) {
    (*queue)->first = new_node;
    (*queue)->last = new_node;
  // Case 2: There are elements in the queue. 
  } else {
    (*queue)->last->next = new_node;
    (*queue)->last = new_node;
  }  
  (*queue)->length++;
  
  return 1;
}