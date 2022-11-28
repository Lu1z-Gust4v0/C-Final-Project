#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct {
  int time_per_operation;  
  int cashier_count;
  int call_order[5];
  int iteration;
  Queue* queues[5];
} Scheduler;

void initialize_scheduler(Scheduler* scheduler, int cashier_count, int time_per_oper, int call_order[5]);
int enqueue_by_class(Scheduler* scheduler, int class_num, int account, int oper_count);
int get_and_remove_next_account(Scheduler* scheduler);
int look_next_account_number(Scheduler* scheduler);
int look_next_operations_count(Scheduler* scheduler);
int look_next_queue_to_be_called(Scheduler* scheduler);
int look_clients_amount(Scheduler* scheduler);
int look_next_client_time(Scheduler* scheduler);
int config_scheduler(Scheduler* scheduler, char* config_file);
void run_scheduler(Scheduler* scheduler, char* input_file, char* output_file);

#endif /* SCHEDULER_H */