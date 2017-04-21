#ifndef APP_REQUEST_QUEUE_H
#define APP_REQUEST_QUEUE_H

#include "aodv.h"
/*
//test  queue's work,
typedef  struct  test_Task
{
	int value;
}Task;
*/

typedef  struct  app_request_queue_node
{
	struct  app_request_queue_node   *prior;
	struct  app_request_queue_node   *next;

	task *rqt_task;
	int dst_seq;
}queue_node;

typedef   struct  app_request_queue_head
{
	queue_node  *first;
	queue_node  *last;
	int queue_size;
	int max_size;
}queue_head;

//create  queue  head
int  init_app_request_queue();
//whether the queue is empty
bool is_app_request_queue_empty();
//add a new node to queue
int app_request_queue_add(int id,task *rqt_task);
//get  Task by  id
task *app_request_queue_get(int id);
//delete  Task by  id
int app_request_queue_delete(int id);
//delete  node ,and  solve  the  prior node and next node
int app_request_queue_free_node(queue_node *node);
//free the hole queue
int app_request_queue_free_queue();
void print_queue();
bool is_dumplicated(int id, task *rqt_task);

#endif
