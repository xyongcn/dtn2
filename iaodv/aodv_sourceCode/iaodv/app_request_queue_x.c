#include "app_request_queue.h"

#ifdef REALSIM

/*
//test  queue's work,
typedef  struct  test_Task
{
	int value;
}Task;
*/

//interface
//head  node
queue_head  *head=NULL;


//create  queue  head
/*
int  app_request_queue_create()
{
	if(head==NULL)
	{
		head=(queue_head *)kmalloc(sizeof(queue_head));
		if(head ==NULL)
		{
			//error ,malloc  cann't  allocate  memory

			return 0;
		}
		else
		{
			head->first=NULL;
			head->last=NULL;
		}
	}
	return 1;
}
*/
int  init_app_request_queue()
{
	if(head==NULL)
	{
		head=(queue_head *)kmalloc(sizeof(queue_head), GFP_ATOMIC);
		if(head ==NULL)
		{
			//error ,malloc  cann't  allocate  memory

			return 0;
		}
		else
		{
			head->first=NULL;
			head->last=NULL;
		}
	}
	return 1;
}

bool is_app_request_queue_empty()
{
	if(head==NULL)
		return true;
	return false;
}

//interface
//add a new node to queue
int app_request_queue_add(int id, task *rqt_task)
{
	//task is  not  valid
	if(rqt_task==NULL)
		return  0;

	//if  head is null,then  create  queue  head
	if(head==NULL)
	{
		init_app_request_queue();
	}

	//drop duplicated task and old infomation
	if( is_dumplicated(id, rqt_task) )
	{
#ifdef 	DEBUG2
		printk("The task is duplicated or old\n"); 
#endif
		//free the task & data
		kfree(rqt_task->data);
		kfree(rqt_task);
		return 0;
	}

	//add  node
	queue_node *node=(queue_node *)kmalloc(sizeof(queue_node),GFP_ATOMIC);
	node->dst_seq=id;
	node->rqt_task=rqt_task;

	//add to queue
	if(head->first==NULL || head->last==NULL)
	{
		head->first=node;
		head->last=node;

		node->prior=node;
		node->next=node;

		return 1;
	}


	head->last->next=node;
	node->next=head->first;
	head->first->prior=node;
	node->prior=head->last;

	head->last=node;
	return 1;
}

//interface
//get  Task by  id
task *  app_request_queue_get(int id)
{

printk("in app_request_queue_get\n");
	if(head==NULL)
	{
		init_app_request_queue();
		return NULL;
	}
printk("in app_request_queue_get22222\n");
	queue_node  *node=head->first;
	if(head->first==NULL)
	{
		return NULL;
	}
printk("in app_request_queue_get3333\n");
	do
	{
		if(node->dst_seq==id)
		{
			task *tmp_task = node->rqt_task;
			app_request_queue_free_node(node);
printk("in app_request_queue_get44444\n");
			return tmp_task;
		}

		node = node->next;
	}while(node!=head->first);
printk("in app_request_queue_get5555\n");
	return  NULL;
}

//interface
//delete  Task by  id
int app_request_queue_delete(int id)
{
	if(head==NULL)
	{
		init_app_request_queue();
		return 0;
	}

	queue_node  *node=head->first;

	if(node==NULL)
		return 0;

	do
	{
		if(node->dst_seq==id)
		{
			app_request_queue_free_node(node);
			return 1;
		}

		node=node->next;
	}while(node!=head->first);

	return  0;
}

//delete  node ,and  solve  the  prior node and next node
int app_request_queue_free_node(queue_node *node)
{
	if(node==NULL)
		return 0;

	if(head==NULL)
	{
		init_app_request_queue();
		return 0;
	}

	if(head->first==NULL || head->last==NULL)
		return 0;

	//if  queue  has only one  node
	if(node->next==node  &&  node->prior==node)
	{
		head->first=NULL;
		head->last=NULL;
		kfree(node);
		return 1;
	}

	node->next->prior=node->prior;
	node->prior->next=node->next;
	kfree(node);
	return 1;
}

//free the hole queue
int app_request_queue_free_queue()
{
	if(head==NULL)
		return 1;

	queue_node *node=head->first;

	if(node==NULL)
	{
		kfree(head);
		head=NULL;
		return 1;
	}

	do
	{
		queue_node *temp=node;
		node=node->next;

		kfree(temp);
	}while(node!=head->first && node!=NULL);
	kfree(head);
	head=NULL;
	return 1;
}

bool is_dumplicated(int id, task *rqt_task)
{
	if(head==NULL)
	{
		printk("queue is empty\n");
		return false;
	}

	queue_node *node=head->first;

	if(node==NULL)
	{
		printk("queue is empty\n");
		return false;
	}
	
	if(head->last==NULL)
		return false;
	
	if( head->last->dst_seq > id )
	{
#ifdef DEBUG2
		printk("too old %d  %d",head->last->dst_seq,id);
#endif
		return true;
	}

	do
	{
		task *tmp_task = node->rqt_task;

		if( (node->dst_seq == id) && (tmp_task->src_ip == rqt_task->src_ip) && (tmp_task->dst_ip == rqt_task->dst_ip) && (tmp_task->dev->name == rqt_task->dev->name) )
			return true;

		node=node->next;
	}while(node!=head->first);

		return false;
}

void print_queue()
{
	if(head==NULL)
	{
		printk("queue is empty\n");
		return;
	}

	queue_node *node=head->first;

	if(node==NULL)
	{
		printk("queue is empty\n");
		return;
	}

	printk("queue node:");
	do
	{
		printk("node seq %d, ",node->dst_seq);

		node=node->next;
	}while(node!=head->first);
	printk("\n");
}

#endif
