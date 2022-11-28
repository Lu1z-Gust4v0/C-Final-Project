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

int enqueue_by_class(Scheduler* scheduler, int class_num, int account, int oper_count) {
  Queue **queue = &scheduler->queues[class_num - 1];

  enqueue(queue, account, oper_count);
}

int look_next_queue_to_be_called(Scheduler* scheduler) {
  int threshold_01 = scheduler->call_order[0]; 
  int threshold_02 = scheduler->call_order[1] + threshold_01;
  int threshold_03 = scheduler->call_order[2] + threshold_02;
  int threshold_04 = scheduler->call_order[3] + threshold_03; 
  int threshold_05 = scheduler->call_order[4] + threshold_04;

  int call_order = scheduler->iteration % threshold_05; 

  // Queue 01;
  if (call_order < threshold_01) {
    return 1;
  // Queue 02;
  } else if (call_order < threshold_02) {
    return 2;
  // Queue 03;
  } else if (call_order < threshold_03) {
    return 3;
  // Queue 04;
  } else if (call_order < threshold_04) {
    return 4;
  // Queue 05;
  } else {
    return 5;
  }
}

int look_next_account_number(Scheduler* scheduler) { 
  int queue_num;

  queue_num = look_next_queue_to_be_called(scheduler);

  return look_next_key(&scheduler->queues[queue_num - 1]);
}

int look_next_operations_count(Scheduler* scheduler) {
  int queue_num;

  queue_num = look_next_queue_to_be_called(scheduler);

  return look_next_value(&scheduler->queues[queue_num - 1]);
}

int look_next_client_time(Scheduler* scheduler) {
  int operations_count;

  operations_count = look_next_operations_count(scheduler);

  return operations_count * scheduler->time_per_operation;
}