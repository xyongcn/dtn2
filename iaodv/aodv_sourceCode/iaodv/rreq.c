/***************************************************************************
 Created by Miguel Catalan Cid - miguel.catcid@gmail.com - Version: Mon Jan 1 2010
 ***************************************************************************/

#include "rreq.h"

extern u_int8_t g_aodv_gateway;
extern u_int8_t g_routing_metric;
extern aodv_route * g_local_route;
extern u_int32_t g_mesh_ip;
extern u_int32_t g_null_ip;
extern u_int32_t g_default_gw;

void convert_rreq_to_host(rreq * tmp_rreq) {
	tmp_rreq->dst_seq = ntohl(tmp_rreq->dst_seq);
	tmp_rreq->path_metric = ntohl(tmp_rreq->path_metric);
}

void convert_rreq_to_network(rreq * tmp_rreq) {
	tmp_rreq->dst_seq = htonl(tmp_rreq->dst_seq);
	tmp_rreq->path_metric = htonl(tmp_rreq->path_metric);
}

int recv_rreq(task * tmp_packet) {
	int error;
	rreq *tmp_rreq = NULL;
	aodv_neigh *tmp_neigh;
	int iam_destination = 0;

	tmp_rreq = tmp_packet->data;
	convert_rreq_to_host(tmp_rreq);

#ifdef DEBUG
	//test:if we can get dev via task
	struct net_device *task_dev;
	task_dev = tmp_packet->dev;
	if(task_dev)
	printk("get dev %s from task in recv rreq\n",task_dev->name);
#endif

	//20141130
	//if (tmp_rreq->dst_ip == g_mesh_ip || tmp_rreq->gateway == g_mesh_ip)
	if(is_local_ip(tmp_rreq->dst_ip) || is_local_ip(tmp_rreq->gateway))	
		iam_destination = 1;

	if (tmp_packet->ttl <= 1 && !iam_destination) //If i'm destination, i can receive it
	{
#ifdef 	CaiDebug
		printk("AODV TTL for RREQ from: %s expired\n", inet_ntoa(tmp_rreq->src_ip));
#endif
		return -ETIMEDOUT;
	}

	tmp_neigh = find_aodv_neigh(tmp_packet->src_ip);

	if (tmp_neigh == NULL) {
#ifdef DEBUG
		printk("Ignoring RREQ received from unknown neighbor\n");
#endif
		return 1;
	}

#ifdef REALSIM
//add for realsim.
//check the neigh distance .if out of com_radius,discard it
	extern node_info *node;
	dis_record *tmp_record = find_dis_record(tmp_neigh->ip);
	if( (tmp_record != NULL) && (tmp_record->dis > node->com_radius) ){
#ifdef DEBUG2
		printk("Ignoring RREQ received from neighbor outside com_radius\n");
#endif
		return 1;
	}
#endif

	//Update neighbor timelife
	delete_timer(tmp_neigh->ip, tmp_neigh->ip, NO_TOS, TASK_NEIGHBOR);
	insert_timer_simple(TASK_NEIGHBOR, HELLO_INTERVAL
			* (1 + ALLOWED_HELLO_LOSS) + 100, tmp_neigh->ip);
	update_timer_queue();

//printk("iAODV in RS: flash neighbour out of time,rreq.c->recv_rreq\n");

	tmp_neigh->lifetime = HELLO_INTERVAL * (1 + ALLOWED_HELLO_LOSS) + 20
			+ getcurrtime();

	//20141130
	//if (g_mesh_ip == tmp_rreq->src_ip) {
	if(is_local_ip(tmp_rreq->src_ip)){
#ifdef DEBUG
		printk("I'm the generator of the incoming RREQ - Discarted\n");
#endif
		return 1;
	}

	if (g_aodv_gateway && tmp_rreq->dst_ip == g_null_ip && !is_local_ip(tmp_rreq->gateway)){//tmp_rreq->gateway != g_mesh_ip) {
#ifdef DEBUG
		printk("I'm not the gateway of this source - I cannot apply as an intermediate node to its default route\n");
#endif
		return 1;
	}

	tmp_rreq->num_hops++;
/*
	if (g_routing_metric == ETT) {
		error = ett_metric(tmp_neigh, tmp_rreq);
		if (error) {
#ifdef DEBUG
			printk("Metric too high - RREQ Discarted\n");
#endif
			return 1;
		}
	}

	else if (g_routing_metric == WCIM) {
		error = wcim_metric(tmp_neigh, tmp_rreq);
		if (error) {
#ifdef DEBUG
			printk("Metric too high - RREQ Discarted\n");
#endif
			return 1;
		}
	} else
*/
		//number of hops
		tmp_rreq->path_metric =tmp_rreq->num_hops;

	//20141223
	//if I'm the dst of the rreq and the rediscovery is set 1,compare the hop_nums in rreq and the one of route.And set the smaller as a better route.

#ifdef RRediscovery
	int l=strlen(tmp_rreq->path);
	//tmp_rreq->path[l+1]='\n';
	if(strcmp(tmp_packet->dev->name,"adhoc0")==0)//it's a wireless link,set path[l] to 0
		tmp_rreq->path[l]='0';
	else //it's a wired link,set path[l] to 1
		tmp_rreq->path[l]='1';
#ifdef DEBUG2
	printk("The path len from %s is %d,& the links seq of the path like %s in rreq\n",inet_ntoa(tmp_neigh->ip),l+1,tmp_rreq->path);
#endif

	if(iam_destination && tmp_rreq->rediscovery==1)
	{
#ifdef DEBUG2
		printk("====I'm dst & the rd is 1 in recv_rreq.====\n");

		aodv_route* t_route = first_aodv_route();
		char src[20];
		char dst[20];
		char next[20];
		//char last[20];
		while(t_route){
			strcpy(src,inet_ntoa(t_route->src_ip));
			strcpy(dst,inet_ntoa(t_route->dst_ip));
			strcpy(next,inet_ntoa(t_route->next_hop));
			printk("t_route src:%s   dst:%s    next:%s\n",src,dst,next);
			t_route = t_route->next;
		}
#endif
		aodv_route *tmp_route = find_aodv_route(tmp_rreq->dst_ip, tmp_rreq->src_ip,tmp_rreq->tos);

		if(tmp_route != NULL)
		{
#ifdef DEBUG2
			printk("-----the next hop of exiting route is %s----\n",inet_ntoa(tmp_route->next_hop));		
#endif
			if(tmp_rreq->num_hops >= tmp_route->num_hops)//to be replaced by new metric
			{
#ifdef DEBUG2
				printk("====It's not a better route,discard it in rrreq.====\n");
#endif
				return 0;
			}
#ifdef DEBUG2
			else
				printk("====It's  a better route with hopnum is %d.====\n",tmp_rreq->num_hops);
#endif	
		}
	
	}//if(iam_destination && tmp_rreq->rediscovery==1)
#endif

	if (rreq_aodv_route(tmp_rreq->dst_ip, tmp_rreq->src_ip, tmp_rreq->tos,
			tmp_neigh, tmp_rreq->num_hops, tmp_rreq->dst_seq, tmp_packet->dev,
			tmp_rreq->path_metric,tmp_rreq->rediscovery)) {
#ifdef RRediscovery
		
		aodv_route *tmp_route = find_aodv_route_by_id(tmp_rreq->src_ip, tmp_rreq->dst_seq);
		if(tmp_route!=NULL)
		{	
			strcpy(tmp_route->path,tmp_rreq->path);
#ifdef DEBUG2
			printk("The path like %s in tmp_route\n",tmp_route->path);
#endif
		

		}
		else
			printk("The route is NULL,find aodv_route by id failed.\n");
			
		if(tmp_rreq->rediscovery==1 && iam_destination)
		{
			
			aodv_route *using_route = find_aodv_route(tmp_rreq->dst_ip, tmp_rreq->src_ip,tmp_rreq->tos);
#ifdef DEBUG2
			printk("----Both the rediscovery and iam_dst are 1,and it's a better route,gen rrrep!----\n");
#endif
			if (!find_timer(tmp_rreq->src_ip, tmp_rreq->dst_ip, tmp_rreq->tos,
					TASK_GEN_RRREP)) {
				task *new_task;
				new_task = create_task(TASK_GEN_RRREP);
				new_task->src_ip = tmp_rreq->src_ip;
				new_task->dst_ip = tmp_rreq->dst_ip;
				new_task->tos = tmp_rreq->tos;
				insert_task_at_front(new_task);
				//insert_timer_directional(TASK_GEN_RRREP, RREP_TIMER, 0,
				//		tmp_rreq->src_ip, tmp_rreq->dst_ip, tmp_rreq->tos);
				update_timer_queue();
			}
			return 0;		
		}
#endif

		if (iam_destination) {

			//If there is a rrep to be sent, it would use the newest route
			//If not, wait a little (RREP_TIMER) for other possible incoming RREQ's (better path?) before sending the RREP
			if (!find_timer(tmp_rreq->src_ip, tmp_rreq->dst_ip, tmp_rreq->tos,
					TASK_SEND_RREP)) {
				insert_timer_directional(TASK_SEND_RREP, RREP_TIMER, 0,
						tmp_rreq->src_ip, tmp_rreq->dst_ip, tmp_rreq->tos);
				update_timer_queue();
			}
			return 0;
		}
		
#ifdef DTN_HELLO
		//manage dttl
		//if it's a rreq for looking for DTN neighbors
		//the dst would not be any ip of local net
		extern dtn_register;
		if(dtn_register){
			if(tmp_rreq->dttl>=1){
				u_int32_t para[4];
				para[0] = tmp_rreq->src_ip;
				//old dtn hello
				/*
				para[1] = (u_int32_t)tmp_rreq->tos;

		#ifdef WWTAO_DEBUG_GENRREP
			printk("testWu: rreq send2dtn tos(%d) \n",(int)tmp_rreq->tos);
		#endif

				para[2] = NULL;
				para[3] = NULL;
				*/

				

		#ifdef WWTAO_DEBUG_GENRREP
			printk("testWu: rreq send2dtn tos(%d) \n",(int)tmp_rreq->tos);
		#endif

				para[1] = NULL;
				para[2] = NULL;
				para[3] = (u_int32_t)tmp_rreq->tos;

				send2dtn((void*)para,DTN_LOCATION_PORT);			
				//gen_rrep(tmp_rreq->src_ip,tmp_rreq->dst_ip,tmp_rreq->tos);
				tmp_rreq->dttl = tmp_rreq->dttl - 1;
			}	
		}
#endif



		//else forwarding RREQ
		convert_rreq_to_network(tmp_rreq);

		local_broadcast(tmp_packet->ttl - 1, tmp_rreq, sizeof(rreq),tmp_packet->dev);

		return 0;
	}

	return 1;

}

int resend_rreq(task * tmp_packet) {
	rreq *out_rreq;
	u_int8_t out_ttl;

	if (tmp_packet->retries <= 0) {
		ipq_drop_ip(tmp_packet->src_ip, tmp_packet->dst_ip, tmp_packet->tos);
#ifdef DEBUG
	char src[16];
	char dst[16];
	strcpy(src, inet_ntoa(tmp_packet->src_ip));
	strcpy(dst, inet_ntoa(tmp_packet->dst_ip));
	printk("RREQ has achieved its retry limit - The route from %s to %s is not possible\n", src, dst);
#endif
		return 0;
	}

	if ((out_rreq = (rreq *) kmalloc(sizeof(rreq), GFP_ATOMIC)) == NULL) {
#ifdef DEBUG
		printk( "Can't allocate new rreq\n");
#endif
		return 0;
	}
	out_ttl = NET_DIAMETER;

	/* Get our own sequence number */
	g_local_route->dst_seq = g_local_route->dst_seq + 1;
	out_rreq->dst_seq = g_local_route->dst_seq;
	out_rreq->dst_ip = tmp_packet->dst_ip;
	if (out_rreq->dst_ip == g_null_ip)
			out_rreq->gateway = g_default_gw;

		else
			out_rreq->gateway = g_null_ip;

	out_rreq->src_ip = tmp_packet->src_ip;
	out_rreq->type = RREQ_MESSAGE;
	out_rreq->num_hops = 0;
	out_rreq->j = 0;
	out_rreq->r = 0;
	out_rreq->d = 1;
	out_rreq->u = 0;
	out_rreq->reserved = 0;
	out_rreq->g = 0;

	out_rreq->path_metric = 0;
	out_rreq->tos = tmp_packet->tos;

#ifdef RRediscovery
	out_rreq->rediscovery = 0;
	memset(out_rreq->path,0,sizeof(out_rreq->path));
	//out_rreq->path[0] = '\n';
#endif

#ifdef DTN_HELLO
		out_rreq->dttl = 0;
#endif 

/*
#ifdef DTN_HELLO
	extern u_int32_t dtn_hello_ip;
	if(tmp_packet->dst_ip == dtn_hello_ip)
		out_rreq->dttl = DTTL;
#endif 
*/

	convert_rreq_to_network(out_rreq);
	local_broadcast(out_ttl, out_rreq, sizeof(rreq),tmp_packet->dev);

		insert_timer_directional(TASK_RESEND_RREQ, 0, tmp_packet->retries - 1,
				out_rreq->src_ip, out_rreq->dst_ip, out_rreq->tos);

	update_timer_queue();

	kfree(out_rreq);

	return 0;
}

/****************************************************

 gen_rreq
 ----------------------------------------------------
 Generates a RREQ! wahhooo!
 ****************************************************/
//the new parameter r means that the rreq is a rediscovery rreq.
//when recevice a rreq whose rediscovery is set to be 1,don't return rrep spilt of having route
int gen_rreq(u_int32_t src_ip, u_int32_t dst_ip, unsigned char tos, u_int8_t rd) {
	rreq *out_rreq;

	u_int8_t out_ttl;
	flow_type *new_flow;

printk("-----------------gen rreq--------------\n");
//	new_flow = find_flow_type(tos);
//
//	if (new_flow == NULL) {
//		printk("new_flow is NULL\n");
//		return 0;
//	}dst_ip

//////
//printk("--------gen_rreq %s-------\n",inet_ntoa(dst_ip));	
/////
#ifdef DTN_HELLO
	extern u_int32_t dtn_hello_ip;
	u_int8_t dttl=0;
	if(dst_ip == dtn_hello_ip)

	{	
		dttl = DTTL;
		//printk("It's a DTN hello!\n");
	}
#endif

//	if (find_timer(src_ip, dst_ip, tos, TASK_RESEND_RREQ)) {
//			printk("don't flood the net with new rreq, wait...\n");
//			return 1;
//	}
#ifdef DEBUG
	else {
		char src[16];
		char dst[16];
		strcpy(src, inet_ntoa(src_ip));
		strcpy(dst, inet_ntoa(dst_ip));
		printk("gen_rreq: Generates a RREQ from %s to %s!\n", src, dst);
	}
#endif

	/* Allocate memory for the rreq message */
	if ((out_rreq = (rreq *) kmalloc(sizeof(rreq), GFP_ATOMIC)) == NULL) {
		printk("Can't allocate new rreq\n");
		return 1;
	}

	out_ttl = NET_DIAMETER;

	/* Get our own sequence number */
	g_local_route->dst_seq = g_local_route->dst_seq + 1;
	out_rreq->dst_seq = g_local_route->dst_seq;
	out_rreq->dst_ip = dst_ip;
	if (out_rreq->dst_ip == g_null_ip)
		out_rreq->gateway = g_default_gw;
	else
		out_rreq->gateway = g_null_ip;

//wujingbang
/*
	if (g_default_gw == g_null_ip){
#ifdef DEBUG
		printk("There isn't a default Gateway! - External traffic will be dropped until an active gateway is discovered\n");
#endif
		kfree(out_rreq);
		return 0;
	}
*/
	out_rreq->src_ip = src_ip;
	out_rreq->type = RREQ_MESSAGE;
	out_rreq->num_hops = 0;
	out_rreq->j = 0;
	out_rreq->r = 0;
	out_rreq->d = 1;
	out_rreq->u = 0;
	out_rreq->reserved = 0;
	out_rreq->g = 0;
	out_rreq->path_metric = 0;
	out_rreq->tos = tos;
#ifdef RRediscovery
	out_rreq->rediscovery = rd;
	memset(out_rreq->path,0,sizeof(out_rreq->path));
	//out_rreq->path[0]='\n';
	//printk("-----rd is %d in gen_rreq-----\n",rd);
#endif
#ifdef DTN_HELLO
	out_rreq->dttl = dttl;
#endif

	convert_rreq_to_network(out_rreq);
	local_broadcast(out_ttl, out_rreq, sizeof(rreq),NULL);
	if(out_rreq->rediscovery == 1)//if it's a rediscovery rreq,don't resend rreq
	{
		kfree(out_rreq);

		return 1;
	}
	insert_timer_directional(TASK_RESEND_RREQ, 0, RREQ_RETRIES, src_ip,
			dst_ip, tos);
	update_timer_queue();
	kfree(out_rreq);

	return 1;

}
