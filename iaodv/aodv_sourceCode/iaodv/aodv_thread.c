/***************************************************************************
 aodv_thread.c  -  description
 -------------------
 begin                : Tue Aug 12 2003
 copyright            : (C) 2003 by Luke Klein-Berndt
 email                : kleinb@nist.gov
 ***************************************************************************/
/***************************************************************************
 Modified by Miguel Catalan Cid - miguel.catcid@gmail.com - Version: Mon Jan 1 2010
 ***************************************************************************/


#include "aodv_thread.h"
#include <linux/sched.h>
//JL: Added kernel threads interface:
#include <linux/kthread.h>

//static int aodv_pid;
wait_queue_head_t aodv_wait;

static atomic_t kill_thread;
static atomic_t aodv_is_dead;
int initialized =0;

//extern u_int32_t g_aodv_gateway;
extern u_int32_t g_mesh_ip;

struct task_struct *aodv_task;

void kick_aodv() {
	//We are trying to wake up AODV!!!
	//AODV thread is an interupptible sleep state.... this interupts it
	wake_up_interruptible(&aodv_wait);
}

void kill_aodv() {

	wait_queue_head_t queue;
	init_waitqueue_head(&queue);
	//sets a flag letting the thread know it should die
	//wait for the thread to set flag saying it is dead
	//lower semaphore for the thread
	atomic_set(&kill_thread, 1);
	wake_up_interruptible(&aodv_wait);
	interruptible_sleep_on_timeout(&queue, HZ);
	//kthread_should_stop();
}

void aodv(void) {
	//The queue holding all the events to be dealt with
	task *tmp_task;

	//Initalize the variables
	init_waitqueue_head(&aodv_wait);
	atomic_set(&kill_thread, 0);
	atomic_set(&aodv_is_dead, 0);


	//Name our thread
	/*lock_kernel();
	 sprintk(current->comm, "aodv-mcc");
	 //exit_mm(current); --> equivale a: current->mm = NULL;
	 //we are in a kthread? aren't we?
	 unlock_kernel();*/

	//add_wait_queue_exclusive(event_socket->sk->sleep,&(aodv_wait));
	//	add_wait_queue(&(aodv_wait),event_socket->sk->sleep);


	//why would I ever want to stop ? :)
	for (;;) {
		//should the thread exit?
		if (atomic_read(&kill_thread)) {
			goto exit;
		}
		//goto sleep until we recieve an interupt
		interruptible_sleep_on(&aodv_wait);

		//should the thread exit?
		if (atomic_read(&kill_thread)) {
			goto exit;
		}
		//While the buffer is not empty
		while ((tmp_task = get_task()) != NULL) {
			
			u_int32_t dst;

			//takes a different action depending on what type of event is recieved
	
#ifdef REALSIM
			//specilly deal with RECV_HELLO when REALSIM is defined
			//donnot free the task data & the task itself
			if(tmp_task->type == TASK_RECV_HELLO)
			{
				recv_hello(tmp_task);

				continue;
			}
#endif 
			switch (tmp_task->type) {
#ifdef REALSIM
			case TASK_ADD_NEIGH:

				add_neigh(tmp_task);
				if(tmp_task->data != NULL)
				{
					kfree(tmp_task->data);
				}
				break;
#endif

#ifdef RRediscovery
			case TASK_RECV_TCNP:
				recv_tcnp(tmp_task);
				kfree(tmp_task->data);
				break;

			case TASK_GEN_RRREQ:
				gen_rreq(tmp_task->src_ip, tmp_task->dst_ip, tmp_task->tos,1);
				break;
			
			case TASK_GEN_RRREP:
//printk("Got TASK_GEN_RRREP\n");
				gen_rrep(tmp_task->src_ip, tmp_task->dst_ip, tmp_task->tos,1);
				break;
				
#endif
			//route neighbor
			case TASK_ROUTE_NEIGH:
				route_neigh(tmp_task->src_ip, tmp_task->dst_ip, tmp_task->tos);
				kfree(tmp_task->data);
				break;
			//remove following case when DTN hell test end
			case TASK_DTN_HELLO:
				inet_aton("127.127.127.127",&dst);
				//extern u_int32_t dtn_hello_ip;
				gen_rreq(g_mesh_ip,dst,tmp_task->tos,0);
#ifdef CaiDebug
				printk("-------DTN HELLO TASK---------\n");
#endif
				//insert_timer_simple(TASK_DTN_HELLO, 300*HELLO_INTERVAL, g_mesh_ip);
				//update_timer_queue();
				break;

			//RREP
			case TASK_RECV_RREP:
				recv_rrep(tmp_task);
				kfree(tmp_task->data);
				break;

				//RERR
			case TASK_RECV_RERR:
				//printk("-----------\nget RERR from %s----------\n",inet_ntoa(tmp_task->src_ip));
				recv_rerr(tmp_task);
				kfree(tmp_task->data);
				break;

			case TASK_RECV_HELLO:
				//printk("get HELLO from %s\n",inet_ntoa(tmp_task->src_ip));
				recv_hello(tmp_task);
				kfree(tmp_task->data);
				break;

           		 /****************��ӽ��յ�ͨ·�������***************/
			#ifdef RECOVERYPATH
            		case TASK_RECV_RCVP:
               			//printk("Receive a RCVP\n");
               			recv_rcvp(tmp_task);
                		kfree(tmp_task->data);
                		break;
			case TASK_RECV_RRDP:
               			//printk("Receive a RRDP\n");
               			recv_rrdp(tmp_task);
                		kfree(tmp_task->data);
                		break;
			#endif

				//Cleanup  the Route Table and Flood ID queue
			case TASK_CLEANUP:
				flush_aodv_route_table();
				break;

			case TASK_HELLO:
				//printk("gen HELLO\n");
				gen_hello();
				break;

			case TASK_ST:
				gen_st_rreq();
				break;

			case TASK_GW_CLEANUP:
				update_gw_lifetimes();
				insert_timer_simple(TASK_GW_CLEANUP, ACTIVE_GWROUTE_TIMEOUT,
						g_mesh_ip);
				update_timer_queue();
				break;

			case TASK_NEIGHBOR:
#ifdef DEBUG2
printk("get NEIGHBOR TASH,delete neigh %s\n",inet_ntoa(tmp_task->src_ip));
#endif
			delete_aodv_neigh(tmp_task->src_ip);

				break;

			case TASK_ROUTE_CLEANUP:
				flush_aodv_route_table();
				break;

			case TASK_NEIGHBOR_2H:
				delete_aodv_neigh_2h(tmp_task->src_ip);
				break;

			case TASK_RECV_RREQ:
				recv_rreq(tmp_task);
				kfree(tmp_task->data);
				break;


			case TASK_RESEND_RREQ:
				resend_rreq(tmp_task);
				break;

			case TASK_SEND_RREP:
				gen_rrep(tmp_task->src_ip, tmp_task->dst_ip, tmp_task->tos,0);
				break;

			case TASK_RECV_STRREQ:
				recv_rreq_st(tmp_task);
				kfree(tmp_task->data);
				break;

			case TASK_UPDATE_LOAD:
				update_my_load();
				break;

			case TASK_GEN_RREQ:
				gen_rreq(tmp_task->src_ip, tmp_task->dst_ip, tmp_task->tos,0);
				break;

			default:
				break;
			}

			kfree(tmp_task);

		}

	}

	exit:
	//Set the flag that shows you are dead
	atomic_set(&aodv_is_dead, 1);

}

void startup_aodv() {
	//aodv_pid = kernel_thread((void *) &aodv, NULL, 0);
	aodv_task = kthread_run((void *) &aodv, NULL, "fbaodv_protocol");
	initialized = 1;
}
