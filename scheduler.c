#include "scheduler.h"

void initialize_scheduler(Scheduler* scheduler, int cashier_count, int time_per_oper, int call_order[5]) {
  int i;
  // Allocate memory for the scheduler.
  scheduler = (Scheduler*) malloc(sizeof(Scheduler));
  scheduler->cashier_count = cashier_count;
  scheduler->time_per_operation = time_per_oper;
  scheduler->iteration = 0;

  for (i = 0; i < 5; i++) {
    // Allocate memory for each queue.
    scheduler->queues[i] = (Queue*) malloc(sizeof(Queue));
    scheduler->call_order[i] = call_order[i];
  }
}