#ifndef FILA_FIFO_H
#define FILA_FIFO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
  int value;
  int key;
  struct _node* next; 
} Node;

typedef struct {
  Node* first;
  Node* last;
  int length;
} Queue;

void initialize_queue(Queue **queue);
int enqueue(Queue **queue, int key, int value);
int get_and_delete_next_key(Queue **queue);
int look_next_key(Queue **queue);
int look_next_value(Queue **queue);
int queue_length(Queue **queue);
int look_key_by_position(Queue **queue, int position);
int look_value_by_position(Queue **queue, int position);


#endif /* FILA_FIFO_H */