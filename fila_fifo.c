#include "fila_fifo.h"

void initialize_queue(Queue **queue) {
  *queue = (Queue*) malloc(sizeof(Queue));  
  (*queue)->first = NULL;
  (*queue)->last = NULL;
  (*queue)->length = 0;
}
