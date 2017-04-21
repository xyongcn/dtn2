/***************************************************************************
 hello.c  -  description
 -------------------
 begin                : Wed Aug 13 2003
 copyright            : (C) 2003 by Luke Klein-Berndt
 email                : kleinb@nist.gov
 ***************************************************************************/
/***************************************************************************
 Created by Miguel Catalan Cid - miguel.catcid@gmail.com - Version: Mon Jan 1 2010
 ***************************************************************************/
#include "hello.h"

extern u_int32_t g_mesh_ip;
extern u_int32_t g_null_ip;
extern u_int8_t g_routing_metric;
extern aodv_neigh *aodv_neigh_list;
extern aodv_dev *g_mesh_dev;
extern aodv_route *g_local_route;
///////////brk_list////////
#ifdef RECOVERYPATH
extern brk_link *brk_list;
#endif

#ifdef BLACKLIST
	extern u_int32_t aodv_blacklist_ip[10];

	extern int aodv_blacksize;
#endif

extern node_info *node;

int count = 0;

void update_neigh_load(aodv_neigh *neigh, aodv_neigh_2h *neigh_2h,
		u_int8_t load, u_int16_t load_seq) {

	if (neigh) {
		if (load_seq > neigh->load_metric.load_seq) { //Load update
			neigh->load_metric.load = load;
			neigh->load_metric.load_seq = load_seq;
		} else if (load_seq < neigh->load_metric.load_seq
				&& neigh->load_metric.load_seq-load_seq > 64000) {
			//sequence number has been reinitialized
			neigh->load_metric.load = load;
			neigh->load_metric.load_seq = load_seq;
		}

	} else if (neigh_2h) {
		if (load_seq > neigh_2h->load_seq) { //Load update
			neigh_2h->load = load;
			neigh_2h->load_seq = load_seq;
		} else if (load_seq < neigh_2h->load_seq
				&& neigh_2h->load_seq-load_seq > 64000) {
			//sequence number has been reinitialized
			neigh_2h->load = load;
			neigh_2h->load_seq = load_seq;
		}
	}

}

void update_neightx(aodv_neigh *neigh, u_int32_t neightx) {

	int i=0;
	int first_empty = -1;
	u_int32_t position;

	for (i=0; i<NEIGH_TABLESIZE; i++) {
		position = neigh->load_metric.neigh_tx[i];
		if (position == neightx) {
			first_empty = i;
			break;
		}
		else if (position == 0 && first_empty == -1)
			first_empty = i;
	}
	neigh->load_metric.neigh_tx[first_empty]=neightx;

}

int send_hello(helloext_struct *total_ext, int num_neigh) {
	hello *tmp_hello;
	int hello_size;
	u_int64_t jitter;
	int rand;
	helloext_struct *tmp_helloext_struct = total_ext;
	helloext_struct *free_helloext = NULL;
	hello_extension *tmp_hello_extension = NULL;

	void *data;
	hello_size = sizeof(hello) + (sizeof(hello_extension) * num_neigh);
	if ((data = kmalloc(hello_size, GFP_ATOMIC)) == NULL) {
		printk("Error creating packet to send HELLO\n");
		return -ENOMEM;
	}


	tmp_hello = (hello *) data;
	tmp_hello->type = HELLO_MESSAGE;

	//extern u_int32_t g_node_name;
	//extern u_int8_t g_node_type;
	tmp_hello->node_name = node->g_node_name;
	tmp_hello->node_type = node->g_node_type;
	

	tmp_hello->reserved1 = 0;
	tmp_hello->num_hops = 0;
	tmp_hello->neigh_count = num_neigh;
	tmp_hello->load = g_mesh_dev->load;
	tmp_hello->load_seq = g_mesh_dev->load_seq;
	


	tmp_hello_extension = (hello_extension *) ((void *)data + sizeof(hello));
	while (tmp_helloext_struct) {
		tmp_hello_extension->ip = tmp_helloext_struct->ip;
		tmp_hello_extension->count = tmp_helloext_struct->count;
		tmp_hello_extension->load = tmp_helloext_struct->load;
		tmp_hello_extension->load_seq = tmp_helloext_struct->load_seq;
		free_helloext = tmp_helloext_struct;
		tmp_helloext_struct = tmp_helloext_struct->next;
		kfree(free_helloext);
		tmp_hello_extension = (void *)tmp_hello_extension
				+ sizeof(hello_extension);
	}

#ifdef REALSIM
	//add location info to hello packet
	tmp_hello->longitude = node->longitude;
	tmp_hello->latitude = node->latitude;
#ifdef DEBUG
	printk("location:%u %u\n....",tmp_hello->longitude,tmp_hello->latitude);
#endif
#endif

	local_broadcast(1, data, hello_size,NULL);
	kfree(data);


	//random jitter
	get_random_bytes(&rand, sizeof (rand));
	jitter = (u_int64_t)(rand%NODE_TRAVERSAL_TIME); //a number between 0 and 50usecs
	insert_timer_simple(TASK_HELLO, HELLO_INTERVAL+jitter, g_mesh_ip);
	update_timer_queue();

	count++;
	if (count == 10) {
		count = 0;
/*
		if (g_routing_metric != HOPS)
			compute_etx();
*/
	}

	return 1;
}

int gen_hello() {

#ifdef REALSIM
//query location at the beginning of gen_hello,and,set the fields in send_hello
	interdata *idata = (interdata *)kmalloc(sizeof(interdata),GFP_ATOMIC);	
	
	idata->src_ip = g_mesh_ip;
	idata->n_latitude= NULL;
	idata->n_longitude = NULL;
	idata->dis = 0;//1 means it's query for neigh,0 means it's for itself
	idata->dst_seq = 0;
	
	//query the location & get distance
	querylocation(idata,LOCATION_PORT);
	//send2dtn((void *)idata,LOCATION_PORT);

#endif

	helloext_struct *total_ext = NULL;
	helloext_struct *simple_ext = NULL;
	int neigh_count = 0;
	aodv_neigh *tmp_entry;
/*
	if (g_routing_metric != HOPS) {
		tmp_entry = aodv_neigh_list;
		while (tmp_entry != NULL) {
			if ((simple_ext = (helloext_struct *) kmalloc(
					sizeof(helloext_struct), GFP_ATOMIC)) == NULL) {
				printk("HELLO - Can't allocate new entry\n");
				return 0;
			}
			simple_ext->next = total_ext;
			total_ext = simple_ext;
			simple_ext->ip = tmp_entry->ip;
			simple_ext->count = tmp_entry->etx.recv_etx;
			simple_ext->load = tmp_entry->load_metric.load;
			simple_ext->load_seq = tmp_entry->load_metric.load_seq;
			neigh_count++;
			tmp_entry = tmp_entry->next;
		}
	}
*/

	send_hello(total_ext, neigh_count);
	
	kfree(idata);

	return 0;
}

#ifdef REALSIM
int recv_hello(task * tmp_packet) {
	aodv_neigh *hello_orig;
	hello * tmp_hello;
	hello_extension *tmp_hello_extension;
	aodv_route *tmp_route;
	int i;
	u_int8_t load = 0;
	u_int16_t load_seq = 0;

	tmp_hello= (hello *)tmp_packet->data;
	tmp_hello_extension = (hello_extension *) ((void*)tmp_packet->data
			+ sizeof(hello));

	//query the location of node & cal the distance between two nodes at app layer
	interdata *idata = (interdata *)kmalloc(sizeof(interdata), GFP_ATOMIC);
	
	idata->src_ip = g_mesh_ip;
	idata->n_latitude = tmp_hello->latitude;
	idata->n_longitude = tmp_hello->longitude;
	idata->dis = 0;//1 means it's query for neigh,0 means it's for itself
	
	//change determine filed 2015.11.15	
	//idata->dst_seq = g_local_route->dst_seq;
	node->app_request_id = (node->app_request_id + 1)%65535;//max int

	idata->dst_seq = node->app_request_id;
	
	//query the location & get distance
	querylocation(idata,LOCATION_PORT);
#ifdef WWTAO_DEBUG_GENRREP
	printk("iAODV in RS: get hello & query apps about location %s %u %u \n",inet_ntoa(tmp_packet->src_ip),tmp_hello->latitude,tmp_hello->longitude);
#endif

	//cache the TASK,push it to app_request_queue.
	//g_local_route->dst_seq ++;
	app_request_queue_add(node->app_request_id,tmp_packet);//change filed.11.15
#ifdef DEBUG
	printk("---add task to task : %d----\n",node->app_request_id);//change field.11.15
	print_queue();
#endif

	kfree(idata);

	return 1;
}

//Only when its within radius,it will run this function
int add_neigh(task *tmp_packet)
{
//printk("-------add neigh when REALSIM---------\n");
	aodv_neigh *hello_orig;
	hello * tmp_hello;
	
	aodv_route *tmp_route;
	int i;
	u_int8_t load = 0;
	u_int16_t load_seq = 0;

	if(tmp_packet->data == NULL )
	{
		printk("=========HAS NO DATA AT TMP_PACKET========\n");
		return 0;
	}
	tmp_hello= (hello *)tmp_packet->data;

#ifdef DEBUG
	printk("-------the neigh ip is %s-------\n",inet_ntoa(tmp_hello->node_name));
#endif
	hello_orig = find_aodv_neigh(tmp_packet->src_ip);

	if (hello_orig == NULL) {
		aodv_neigh_2h *tmp_neigh_2h = find_aodv_neigh_2h(tmp_packet->src_ip);

		if (tmp_neigh_2h) { //previous detected as a 2h_neighbor - delete it but conserve its load info

			load = tmp_neigh_2h->load;
			load_seq = tmp_neigh_2h->load_seq;

			delete_aodv_neigh_2h(tmp_neigh_2h->ip);
			printk("MOVING NEIGHBOR_2HOPS TO NEIGHBOUR_1HOP: %s\n",
					inet_ntoa(tmp_packet->src_ip));

		} else
		{


			char neigh_name[20];
			strcpy(neigh_name,inet_ntoa(tmp_hello->node_name));
			printk("\nNEW NEIGHBOR_1HOP DETECTED:%s : %s\n",
					neigh_name,inet_ntoa(tmp_packet->src_ip));

		}

		hello_orig = create_aodv_neigh(tmp_hello->node_name,tmp_hello->node_type,tmp_packet->src_ip,tmp_packet->dev);


		if (!hello_orig) {
#ifdef DEBUG2
			printk("Error creating neighbor: %s\n", inet_ntoa(tmp_packet->src_ip));

#endif
			return -1;
		}



#ifdef RECOVERYPATH
		//printk("Start to manage brk_list in hello.c!\n");
		brk_link *tmp_link;
		tmp_link = brk_list;
		int is_rcvp = 0;
		unsigned char tos;
		
		tos = 0x02;

		if(tmp_link==NULL){
#ifdef CaiDebug
		    printk("The brk list is empty!\n");
#endif
		}
		else{
		    while(tmp_link!=NULL){
			is_rcvp = 0;
		        if(tmp_link->state!=INVALID && !is_overlapped_with_route(tmp_link) ){//�ö�·δʧЧ
		            if(tmp_link->dst_ip == tmp_packet->src_ip){//the new neighor is the right dst
				//because gen_rcvp will remove the current tmp_link, so get next
				//link sould be manage in this segment,the same in else
				//tmp_link = tmp_link->next;
                        	gen_rcvp(tmp_packet->src_ip);
				//gen_rcvp will manage all link which has a same dst,so just break
				is_rcvp = 1;
				break;
#ifdef CaiDebug
    printk("the new neighbor is just the dst of brk_link,gen rcvp to it\n");
#endif
		            }
		            else{
				if(gen_rreq(g_mesh_ip, tmp_link->dst_ip,tos)){

#ifdef CaiDebug
		                char src[20];
		                char dst[20];
		                strcpy(src,inet_ntoa(tmp_link->src_ip));
		                strcpy(dst,inet_ntoa(tmp_link->dst_ip));
		                printk("Try recovery the link from %s to %s\n",src,dst);
#endif	
		            	}//gen rreq

			    }//else
		        }//not invalid
			if(!is_rcvp)
		        	tmp_link = tmp_link->next;

		    }//while
		}


		/*********************manage brk_list*************************/

		/*********************manage route redirection*********************/
		
		tmp_route = first_aodv_route();
		while(tmp_route && tmp_route->state != INVALID){
			if( (tmp_route->src_ip != tmp_route->dst_ip)
					&& !is_local_ip(tmp_route->src_ip) ){//not self route

				gen_rrdp(tmp_route->src_ip,tmp_route->dst_ip,
						tmp_route->last_hop,tmp_route->tos);
			}


#ifdef DTN

			extern int dtn_register;
			if( is_local_ip(tmp_route->src_ip) && dtn_register 
					&& (tmp_route->src_ip != tmp_route->dst_ip) ){//I'm the source,tell DTN

				u_int32_t para[4];
				para[0] = tmp_route->src_ip;
				para[1] = tmp_route->dst_ip;
				para[2] = NULL;
				para[3] = (u_int32_t)RRDP_MESSAGE;
				send2dtn((void*)para,DTNPORT);
#ifdef CaiDebug
				printk("------------send the rrdp to DTN-----------\n");
#endif
	
#endif
			}
			tmp_route = tmp_route->next;
		}

		
#endif

		
#ifdef	RRediscovery
//tcnp means Topology Change Notification Packet
		if(tmp_hello->node_type == WDN_NODE || tmp_hello->node_type==ICN_NODE){
			tmp_route = first_aodv_route();
			while(tmp_route && tmp_route->state != INVALID){
				//if( (tmp_route->src_ip != tmp_route->dst_ip)
						//&& !is_local_ip(tmp_route->src_ip) ){//not self route
				if(!(tmp_route->self_route || tmp_route->neigh_route)){
					if(is_local_ip(tmp_route->src_ip)){
						//Iam the source of route,try to rediscovery

#ifdef DEBUG2
						printk("I'm the source of route,insert a RRREQ task in recv hello\n");
#endif
						task *new_task;
						new_task = create_task(TASK_GEN_RRREQ);
						new_task->src_ip = tmp_route->src_ip;
						new_task->dst_ip = tmp_route->dst_ip;
						new_task->tos = tmp_route->tos;
						insert_task_at_front(new_task);
					}
					else{
#ifdef DEBUG2
						printk("Gen tcnp in recv hello\n");
						printk("----Last hop:%s in gen_tcnp\n",inet_ntoa(tmp_route->last_hop));
#endif
						if(tmp_route->last_hop!=g_null_ip)
							gen_tcnp(tmp_route->src_ip,tmp_route->dst_ip,tmp_route->last_hop,tmp_route->tos);
					}
					
				}

				tmp_route = tmp_route->next;
			}
		}

		

#endif



		hello_orig->load_metric.load = load;
		hello_orig->load_metric.load_seq = load_seq;

	}

//Update neighbor timelife
	delete_timer(hello_orig->ip, hello_orig->ip, NO_TOS, TASK_NEIGHBOR);
	insert_timer_simple(TASK_NEIGHBOR, HELLO_INTERVAL
			* (1 + ALLOWED_HELLO_LOSS) + 100, hello_orig->ip);
	update_timer_queue();

//printk("iAODV in RS: flash neighbour out of time,hello.c->add_neigh\n");

	hello_orig->lifetime = HELLO_INTERVAL * (1 + ALLOWED_HELLO_LOSS) + 20
			+ getcurrtime();
	if (g_routing_metric == HOPS) //it doesn't need metric computation
		return 0;

printk("======End add neigh========\n");

	return 0;
	

}

#else

int recv_hello(task * tmp_packet) {
	aodv_neigh *hello_orig;
	hello * tmp_hello;
	hello_extension *tmp_hello_extension;
	aodv_route *tmp_route;
	int i;
	u_int8_t load = 0;
	u_int16_t load_seq = 0;

	tmp_hello= (hello *)tmp_packet->data;
	tmp_hello_extension = (hello_extension *) ((void*)tmp_packet->data
			+ sizeof(hello));

#ifdef BLACKLIST
	//block the aodv blacklist
	int k = 0;
	for(k=0; k<aodv_blacksize; k++) {
		if (aodv_blacklist_ip[k] == tmp_packet->src_ip)
			return 0;
	}
#endif
	hello_orig = find_aodv_neigh(tmp_packet->src_ip);

	if (hello_orig == NULL) {
		aodv_neigh_2h *tmp_neigh_2h = find_aodv_neigh_2h(tmp_packet->src_ip);

		if (tmp_neigh_2h) { //previous detected as a 2h_neighbor - delete it but conserve its load info
			load = tmp_neigh_2h->load;
			load_seq = tmp_neigh_2h->load_seq;
			delete_aodv_neigh_2h(tmp_neigh_2h->ip);
			printk("MOVING NEIGHBOR_2HOPS TO NEIGHBOUR_1HOP: %s\n",
					inet_ntoa(tmp_packet->src_ip));

		} else
		{
			char neigh_name[20];
			strcpy(neigh_name,inet_ntoa(tmp_hello->node_name));
			printk("\nNEW NEIGHBOR_1HOP DETECTED:%s : %s\n",
					neigh_name,inet_ntoa(tmp_packet->src_ip));
		}

		hello_orig = create_aodv_neigh(tmp_hello->node_name,tmp_hello->node_type,tmp_packet->src_ip,tmp_packet->dev);

		if (!hello_orig) {
#ifdef DEBUG
			printk("Error creating neighbor: %s\n", inet_ntoa(tmp_packet->src_ip));
#endif
			return -1;
		}


#ifdef RECOVERYPATH
		/*********************************************
            �����ͨ·���?�֣��ټ�⵽���ھ�ʱ��
            ɨ���·�?������Ӧ��Ŀ����·�ɷ���
		**********************************************/
		//printk("Start to manage brk_list in hello.c!\n");
		brk_link *tmp_link;
		tmp_link = brk_list;
		int is_rcvp = 0;
		unsigned char tos;
		//Ĭ������Ϊ1Mbps���ͣ�������Ҫ���е���

		tos = 0x02;

		if(tmp_link==NULL){//�գ��������������
#ifdef CaiDebug
		    printk("The brk list is empty!\n");
#endif
		}
		else{
		    while(tmp_link!=NULL){//�����·�?����·�ɷ���
			is_rcvp = 0;
		        if(tmp_link->state!=INVALID && !is_overlapped_with_route(tmp_link) ){//�ö�·δʧЧ
		            if(tmp_link->dst_ip == tmp_packet->src_ip){//the new neighor is the right dst
				//because gen_rcvp will remove the current tmp_link, so get next
				//link sould be manage in this segment,the same in else
				//tmp_link = tmp_link->next;
                        	gen_rcvp(tmp_packet->src_ip);
				//gen_rcvp will manage all link which has a same dst,so just break
				is_rcvp = 1;
				break;
#ifdef CaiDebug
    printk("the new neighbor is just the dst of brk_link,gen rcvp to it\n");
#endif
		            }
		            else{
				if(gen_rreq(g_mesh_ip, tmp_link->dst_ip,tos)){

#ifdef CaiDebug
		                char src[20];
		                char dst[20];
		                strcpy(src,inet_ntoa(tmp_link->src_ip));
		                strcpy(dst,inet_ntoa(tmp_link->dst_ip));
		                printk("Try recovery the link from %s to %s\n",src,dst);
#endif	
		            	}//gen rreq

			    }//else
		        }//not invalid
			if(!is_rcvp)
		        	tmp_link = tmp_link->next;

		    }//while
		}


		/*********************manage brk_list*************************/

		/*********************manage route redirection*********************/
		
		tmp_route = first_aodv_route();
		while(tmp_route && tmp_route->state != INVALID){
			if( (tmp_route->src_ip != tmp_route->dst_ip)
					&& !is_local_ip(tmp_route->src_ip) ){//not self route

				gen_rrdp(tmp_route->src_ip,tmp_route->dst_ip,
						tmp_route->last_hop,tmp_route->tos);
			}


#ifdef DTN

			extern int dtn_register;
			if( is_local_ip(tmp_route->src_ip) && dtn_register 
					&& (tmp_route->src_ip != tmp_route->dst_ip) ){//I'm the source,tell DTN

				u_int32_t para[4];
				para[0] = tmp_route->src_ip;
				para[1] = tmp_route->dst_ip;
				para[2] = NULL;
				para[3] = (u_int32_t)RRDP_MESSAGE;
				send2dtn((void*)para,DTNPORT);
#ifdef CaiDebug
				printk("------------send the rrdp to DTN-----------\n");
#endif
	
#endif
			}
			tmp_route = tmp_route->next;
		}

		
#endif

		
#ifdef	RRediscovery
//tcnp means Topology Change Notification Packet
		if(tmp_hello->node_type == WDN_NODE || tmp_hello->node_type==ICN_NODE){
			tmp_route = first_aodv_route();
			while(tmp_route && tmp_route->state != INVALID){
				//if( (tmp_route->src_ip != tmp_route->dst_ip)
						//&& !is_local_ip(tmp_route->src_ip) ){//not self route
				if(!(tmp_route->self_route || tmp_route->neigh_route)){
					if(is_local_ip(tmp_route->src_ip)){
						//Iam the source of route,try to rediscovery
#ifdef DEBUG2
						printk("I'm the source of route,insert a RRREQ task in recv hello\n");
#endif
						task *new_task;
						new_task = create_task(TASK_GEN_RRREQ);
						new_task->src_ip = tmp_route->src_ip;
						new_task->dst_ip = tmp_route->dst_ip;
						new_task->tos = tmp_route->tos;
						insert_task_at_front(new_task);
					}
					else{
#ifdef DEBUG2
						printk("Gen tcnp in recv hello\n");
						printk("----Last hop:%s in gen_tcnp\n",inet_ntoa(tmp_route->last_hop));
#endif
						if(tmp_route->last_hop!=g_null_ip)
							gen_tcnp(tmp_route->src_ip,tmp_route->dst_ip,tmp_route->last_hop,tmp_route->tos);
					}
					
				}

				tmp_route = tmp_route->next;
			}
		}

		

#endif



		hello_orig->load_metric.load = load;
		hello_orig->load_metric.load_seq = load_seq;

	}

//Update neighbor timelife
	delete_timer(hello_orig->ip, hello_orig->ip, NO_TOS, TASK_NEIGHBOR);
	insert_timer_simple(TASK_NEIGHBOR, HELLO_INTERVAL
			* (1 + ALLOWED_HELLO_LOSS) + 100, hello_orig->ip);
	update_timer_queue();

//printk("iAODV in RS: flash neighbour out of time,hello.c->recv_hello\n");

	hello_orig->lifetime = HELLO_INTERVAL * (1 + ALLOWED_HELLO_LOSS) + 20
			+ getcurrtime();
	if (g_routing_metric == HOPS) //it doesn't need metric computation
		return 0;
//// these are not need by hops of metric
/*			hello_orig->etx.count++;
	if (g_routing_metric == WCIM)
		update_neigh_load(hello_orig, NULL, tmp_hello->load,
				tmp_hello->load_seq);

	for (i=0; i < tmp_hello->neigh_count; i++) {
		if (tmp_hello_extension->ip == g_mesh_ip) {
			hello_orig->etx.send_etx = tmp_hello_extension->count;
			if (g_routing_metric == ETT) //it only needs its etx count
			return 0;
		}

		else {
			if (g_routing_metric == WCIM) { //neighbors of my neighbors

				aodv_neigh *ext_neigh =
						find_aodv_neigh(tmp_hello_extension->ip);

				if (ext_neigh)
					update_neigh_load(ext_neigh, NULL, tmp_hello->load,
							tmp_hello->load_seq);

				else {
					aodv_neigh_2h *ext_neigh_2h =
							find_aodv_neigh_2h(tmp_hello_extension->ip);
					if (ext_neigh_2h == NULL)
						ext_neigh_2h = create_aodv_neigh_2h(tmp_hello_extension->ip);

					else {
						delete_timer(tmp_hello_extension->ip,
								tmp_hello_extension->ip, NO_TOS,
								TASK_NEIGHBOR_2H);
						insert_timer_simple(TASK_NEIGHBOR_2H,
								NEIGHBOR_2H_TIMEOUT + 100,
								tmp_hello_extension->ip);
						update_timer_queue();
						ext_neigh_2h->lifetime = getcurrtime()
								+ NEIGHBOR_2H_TIMEOUT;
					}
					update_neigh_load(NULL, ext_neigh_2h, tmp_hello->load,
							tmp_hello->load_seq);
				}

				update_neightx(hello_orig, tmp_hello_extension->ip);
			}
		}

		tmp_hello_extension = (hello_extension *) ((void*)tmp_hello_extension
				+ sizeof(hello_extension));
	}
*/
	return 0;
}

#endif

/*
void compute_etx() {
	aodv_neigh *tmp_neigh = aodv_neigh_list;
	while (tmp_neigh != NULL) {
		if (tmp_neigh->etx.count == 0)
			tmp_neigh = tmp_neigh->next;
		else {
			tmp_neigh->etx.recv_etx = tmp_neigh->etx.count;
			tmp_neigh->etx.count = 0;

		if (tmp_neigh->etx.recv_etx  > 10)
			tmp_neigh->etx.recv_etx = 10;
		if (tmp_neigh->etx.send_etx  > 10)
			tmp_neigh->etx.send_etx = 10;

			tmp_neigh->etx_metric = tmp_neigh->etx.recv_etx
					*tmp_neigh->etx.send_etx; //new etx


			tmp_neigh = tmp_neigh->next;
		}

	}
}
*/
