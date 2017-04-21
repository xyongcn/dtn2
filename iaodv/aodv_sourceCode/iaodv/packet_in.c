/***************************************************************************
 packet_in.c  -  description
 -------------------
 begin                : Mon Jul 29 2002
 copyright            : (C) 2002 by Luke Klein-Berndt
 email                : kleinb@nist.gov
 ***************************************************************************/
/***************************************************************************
 Modified by Miguel Catalan Cid - miguel.catcid@gmail.com - Version: Mon Jan 1 2010
 ***************************************************************************/

#include "packet_in.h"

#ifdef DTN
	extern int dtn_register;
#endif
#ifdef BLACKLIST
	extern u_int32_t aodv_blacklist_ip[10];
	extern u_int32_t dtn_blacklist_ip[10];
	extern int aodv_blacksize;
	extern int dtn_blacksize;
#endif

extern node_info *node;

int valid_aodv_packet(int numbytes, int type, char *data, struct timeval tv) {

	rerr *tmp_rerr;

	rrep *tmp_rrep;
	hello *tmp_hello;
	rreq *tmp_rreq;
	rreq_st *tmp_strreq;

#ifdef RECOVERYPATH
	rcvp *tmp_rcvp;
	rrdp *tmp_rrdp;
#endif
/*
	s_probe *tmp_sprobe;
	l_probe *tmp_lprobe;
	ett_info *tmp_ett_info;
*/
	switch (type) {

	case ST_RREQ_MESSAGE:
		tmp_strreq = (rreq_st *) data;
		if (numbytes == sizeof(rreq_st)) {
			return 1;
		}
		break;

	case RREP_MESSAGE:

		tmp_rrep = (rrep *) data;
		if (numbytes == sizeof(rrep)) {
			return 1;
		}
		break;

	case RERR_MESSAGE:
		tmp_rerr = (rerr *) data;
		if (numbytes == sizeof(rerr)) {
			return 1;
		}
		break;

	case HELLO_MESSAGE:
		tmp_hello = (hello *) data;
		if (numbytes == (sizeof(hello) + (sizeof(hello_extension)
				* tmp_hello->neigh_count))) {
			return 1;
		}
		break;

    /****************�������RCVP_MESSAGE*******************/
	#ifdef RECOVERYPATH
    	case RCVP_MESSAGE:
        	tmp_rcvp = (rcvp *) data;
        	if(numbytes == sizeof(rcvp))
       			return 1;
        	break;
	case RRDP_MESSAGE:
		tmp_rrdp = (rrdp *) data;
        	if(numbytes == sizeof(rrdp))
       			return 1;
        	break;
	#endif
/*
	case ETT_S_MESSAGE: //ETT small message received
		tmp_sprobe = (s_probe *) data;
		if (numbytes == sizeof(s_probe)) {
			tmp_sprobe->sec = (u_int32_t) tv.tv_sec;
			tmp_sprobe->usec= (u_int32_t) tv.tv_usec;
			return 1;
		}
		break;

	case ETT_L_MESSAGE: //ETT large message received
		tmp_lprobe = (l_probe *) data;
		if (numbytes == sizeof(l_probe)) {
			tmp_lprobe->sec = (u_int32_t) tv.tv_sec;
			tmp_lprobe->usec= (u_int32_t) tv.tv_usec;
			return 1;
		}
		break;
*/
	case RREQ_MESSAGE:
		tmp_rreq = (rreq *) data;
		if (numbytes == sizeof(rreq))
			return 1;
		break;

/*
	case ETT_INFO_MSG:
		tmp_ett_info = (ett_info *) data;
		if (numbytes == sizeof(ett_info))
			return 1;
		break;
*/
	default:
		break;

	}
	return 0;
}

int packet_in(struct sk_buff *packet, struct timeval tv) {
	struct iphdr *ip;

	// Create aodv message types
	u_int8_t aodv_type;

	//The packets which come in still have their headers from the IP and UDP
	int start_point = sizeof(struct udphdr) + sizeof(struct iphdr);

	//get pointers to the important parts of the message
	ip = ip_hdr(packet);

	aodv_type = (int) packet->data[start_point];
#ifdef CaiDebug
    if( aodv_type == RCVP_MESSAGE)
        printk("It's a rcvp message\n");
#endif


#ifdef DEBUG
	if ( aodv_type != HELLO_MESSAGE )
		printk("packet_in: type: %d and of size %u from: %s\n", aodv_type, packet->len - start_point, inet_ntoa(ip->saddr));
#endif

	if (!valid_aodv_packet(packet->len - start_point, aodv_type, packet->data
			+ start_point, tv)) {

#ifdef DEBUG
		printk("Packet of type: %d and of size %u from: %s failed packet check!\n", aodv_type, packet->len - start_point, inet_ntoa(ip->saddr));
#endif
		return NF_DROP;

	}

	insert_task(aodv_type, packet);

	return NF_ACCEPT;
}

extern int initialized;
unsigned int input_handler(unsigned int hooknum, struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out, int (*okfn) (struct sk_buff *)) {
	struct timeval tv;

	u_int8_t aodv_type;
	int start_point = sizeof(struct udphdr) + sizeof(struct iphdr);
	aodv_type = (int) skb->data[start_point];

	struct iphdr *ip = ip_hdr(skb);
	struct in_device *tmp_indev = (struct in_device *) in->ip_ptr;

	void *p = (uint32_t *) ip + ip->ihl;
	struct udphdr *udp = (struct udphdr *) p;

	if (!initialized) { // this is required otherwise kernel calls this function without insmod completing the module loading process.
		return NF_ACCEPT;
	}


	char src[16];
	char dst[16];
	strcpy(src, inet_ntoa(ip->saddr));
	strcpy(dst, inet_ntoa(ip->daddr));

#ifdef REALSIM
/*
	if(udp->dest == htons(LOCATION_PORT))
	{
		extern node_info *node;
		interdata *loc_data;
		loc_data = &(skb->data[start_point]);
		u_int32_t src_ip = loc_data->para1;
		//u_int32_t tos = ((u_int32_t *)loc_data)[1];
		if(is_local_ip(src_ip))
		{
			u_int32_t y = loc_data->para2;
			u_int32_t x = loc_data->para3;
			
			node->longitude = ntohl(x);
			node->latitude = ntohl(y);
#ifdef DEBUG2
			printk("*************location:%d %d at %d************\n",node->latitude,node->longitude,getcurrtime());
#endif
			if(loc_data->para4)//it's a neigh location query & in com_radius,insert a task
					   //or drop it
			{
				task *tmp_data = loc_data->pdata;
				task *new_task;
				new_task = create_task(TASK_ADD_NEIGH);
				new_task->src_ip = tmp_data->src_ip;
				new_task->dst_ip = tmp_data->src_ip;//neigh_ip
				new_task->tos = tmp_data->tos;
				new_task->dev = tmp_data->dev;
				new_task->data = tmp_data->data;
				insert_task_at_front(new_task);
				//insert_timer_directional(TASK_GEN_RRREP, RREP_TIMER, 0,
				//		tmp_rreq->src_ip, tmp_rreq->dst_ip, tmp_rreq->tos);
				update_timer_queue();
				printk("New add neigh task,src:%s,dev:%s------\n",inet_ntoa(new_task->dst_ip),new_task->dev->name);
			}
			else//not within radius drop it
			{	
#ifdef DEBUG2
				return NF_DROP;
			
				printk("The neigh is out of radius\n");
#endif
			}
		}
	}
	if(udp->dest == htons(10004))//test
	{
		printk("----Can get data from RS-----\n");
	}
*/
#endif 

#ifdef DTN
	static int t = 1;
	//DTN register
	if (t && !dtn_register && udp->dest == htons(DTNREGISTER)){
		dtn_register = 1;
		t = 0;
		printk("*************DTN registered*************\n");
		#ifdef WWTAO_DEBUG_GENRREP
		printk("testWu: get dtn registered\n");
		#endif
	}
	
	extern u_int32_t g_mesh_ip;	
	if( dtn_register && udp->dest == htons(GET_DTN_HELLO) ){
		insert_timer_simple(TASK_DTN_HELLO, 0, g_mesh_ip);
		update_timer_queue();
		printk("*************GOT DTN HELLO TASK*************\n");
		#ifdef WWTAO_DEBUG_GENRREP
		printk("testWu: get dtn's hello request\n");
		#endif
	}
/*
	if( dtn_register && udp->dest == htons(AODV_LOCATION_PORT)){
		extern u_int32_t dtn_hello_ip;
		extern u_int32_t longitude;
		extern u_int32_t latitude;
		void *loc_data;
		loc_data = &(skb->data[start_point]);
		u_int32_t src_ip = ((u_int32_t *)loc_data)[0];
		u_int32_t tos = ((u_int32_t *)loc_data)[1];
		u_int32_t x = ((u_int32_t *)loc_data)[2];
		u_int32_t y = ((u_int32_t *)loc_data)[3];
		longitude = x;
		latitude = y;
#ifdef CaiDebug
		printk("*********GOT Location Info%s  %ld  %ld**********\n",inet_ntoa(src_ip),ntohl(x),ntohl(y));
#endif
		//gen_rrep(src_ip,dtn_hello_ip,(unsigned char)tos);
		insert_timer_directional(TASK_SEND_RREP, RREP_TIMER, 0,
						src_ip, dtn_hello_ip,(unsigned char)tos);
		update_timer_queue();
}
*/
#endif
	
#ifdef WWTAO_DEBUG_GENRREP
	if(udp->dest == htons(RE_DTN_HELLO) )
	{
		void *loc_data;
		
		loc_data = &(skb->data[start_point]);

		u_int32_t src_ip = ((u_int32_t *)loc_data)[0];
		u_int32_t temp_longitude = ((u_int32_t *)loc_data)[1];
		u_int32_t temp_latitude = ((u_int32_t *)loc_data)[2];
		printk("testWu:temp: get mesg sent to DTN ,ip(%u),longitude(%u),latitude(%u)\n",src_ip,temp_longitude,temp_latitude);
		printk("testWu:temp: (%x,%x,%x)\n",src_ip,temp_longitude,temp_latitude);
	}
#endif

	if(udp->dest == htons(REPLY_LOCATION_PORT) )
	{
		void *loc_data;
		
		loc_data = &(skb->data[start_point]);

		u_int32_t src_ip = ((u_int32_t *)loc_data)[0];
		
		bool local_ip = is_local_ip(src_ip);
#ifdef DTN		
		if( dtn_register ){

			if( local_ip == false){
				extern u_int32_t dtn_hello_ip;

				
				//old dtn Hello
				/*
				u_int32_t tos = ((u_int32_t *)loc_data)[1];	

		#ifdef WWTAO_DEBUG_GENRREP
			printk("testWu: rreq send2dtn tos(%d) \n",(int)tos);
		#endif

				u_int32_t x = ((u_int32_t *)loc_data)[2];

				u_int32_t y = ((u_int32_t *)loc_data)[3];
                                */

				



				u_int32_t y = ((u_int32_t *)loc_data)[1];

				u_int32_t x = ((u_int32_t *)loc_data)[2];
				
				u_int32_t tos = ((u_int32_t *)loc_data)[3];	

		#ifdef WWTAO_DEBUG_GENRREP
			printk("testWu: rreq send2dtn tos(%d) \n",(int)tos);
		#endif

				node->longitude = x;

				node->latitude = y;
			
				#ifdef WWTAO_DEBUG_GENRREP
				printk("testWu:temq: input handler1,change node's longitude and lattitude,node->longitude(%u),node->latitude(%u)\n",node->longitude,node->latitude);
				#endif

			
#ifdef DEBUG2

				printk("*********GOT Location Info%s  %u  %u**********\n",inet_ntoa(src_ip),x,y);
	
#endif
				#ifdef WWTAO_DEBUG
				printk("testWu: receive dtn location , prepare send rrep\n");
				#endif

				//gen_rrep(src_ip,dtn_hello_ip,(unsigned char)tos);

				insert_timer_directional(TASK_SEND_RREP, RREP_TIMER, 0,

						src_ip, dtn_hello_ip,(unsigned char)tos);

				update_timer_queue();
			}
#ifdef REALSIM
			else
			{

				interdata *i_data;
		
				i_data = &(skb->data[start_point]);
				
				node->longitude = i_data->n_longitude;

				node->latitude = i_data->n_latitude;

				#ifdef WWTAO_DEBUG_GENRREP
				printk("testWu:temq: input handler2,change node's longitude and lattitude,node->longitude(%u),node->latitude(%u)\n",node->longitude,node->latitude);
				#endif

#ifdef DEBUG2
				printk("*********GOT Location Info%s  %u  %u when dtn is registered**********\n",inet_ntoa(i_data->src_ip),node->latitude,node->longitude);
#endif

				manage_location(i_data);


			}
#endif

		}
#endif
#ifdef REALSIM
		else{//has no dtn

				interdata *i_data;
		
				i_data = &(skb->data[start_point]);
				
				node->longitude = i_data->n_longitude;

				node->latitude = i_data->n_latitude;

				#ifdef WWTAO_DEBUG_GENRREP
				printk("testWu:temq: input handler3,change node's longitude and lattitude,node->longitude(%u),node->latitude(%u)\n",node->longitude,node->latitude);
				#endif

#ifdef DEBUG
				printk("*********GOT Location Info%s  %u  %u**********\n",inet_ntoa(src_ip),node->latitude,node->longitude);
#endif
				if(local_ip)
				{
					manage_location(i_data);
					//printk("end manage location\n");
				}
		}
#endif
	}

#ifdef REALSIM
	
	if (udp->dest == htons(9556)) {
/*		printk("intercept broadcast: get the port(9556),dst(%s),source(%s), broadcast packet, start intercept\n",dst,inet_ntoa(ip->saddr));

		if(strcmp("192.168.5.255",dst)){
			printk("intercept broadcast: dst(%s) is 192.168.5.255\n",dst);
		}

		if(find_aodv_neigh(ip->saddr)==NULL){
			printk("intercept broadcast: source(%s) is in neighbour list\n",inet_ntoa(ip->saddr));
		}*/

		if(!strcmp("192.168.5.255",dst) && find_aodv_neigh(ip->saddr)==NULL){
			// printk("intercept broadcast: abandon packet\n");
			return NF_DROP;
		}
	}
#else
#ifdef BLACKLIST
	int k = 0;
	if (udp->dest == htons(9556)) {
		for(k=0; k<dtn_blacksize; k++) {
			if (!strcmp("192.168.1.255",dst) && dtn_blacklist_ip[k] == ip->saddr) {
				return NF_DROP;
			}
		}
	}
#endif
#endif

#ifdef DEBUG
char name[30];
unsigned char if_port;
int ifindex;
unsigned short type;
unsigned short dev_id;
struct net_device *dev = skb->dev;
if( (strcmp(dev->name,"lo")!=0) && (aodv_type != HELLO_MESSAGE)){

	strcpy(name,dev->name);
	if_port = dev->if_port;
	ifindex = dev->ifindex;
	type = dev->type;
	dev_id = dev->dev_id;
	printk("-----------------in_handler-------------------\ndev->name:%s\ndev->ifindex:%d\ndev->type:%d\ndev->dev_id:%d\n----------------------------------\n",name,ifindex,type,dev_id);
}
#endif

#ifdef DEBUG
	if (ip->protocol == IPPROTO_ICMP) {
		printk("input_handler: Recv a ICMP from %s to %s\n", src, dst);
	} else if (aodv_type != HELLO_MESSAGE) {
		printk("input_handler: Recv a %d Packet, from %s to %s\n",ip->protocol, src, dst);
	}
#endif
#ifdef DEBUG2
	if (aodv_type == RREQ_MESSAGE ) {
		printk("input_handler: Recv a RREQ, from %s to %s\n",ip->protocol, src, dst);
	} else if (aodv_type == RREP_MESSAGE) {
		printk("input_handler: Recv a RREP, from %s to %s\n",ip->protocol, src, dst);
	}
#endif


	if (udp != NULL && ip->protocol == IPPROTO_UDP && skb->protocol == htons(ETH_P_IP) && udp->dest == htons(AODVPORT) && tmp_indev->ifa_list->ifa_address != ip->saddr) {

		do_gettimeofday(&tv); //MCC - capture the time of arrival needed for ett_probes
		return packet_in((skb), tv);

	}

#ifdef DEBUG
	if (aodv_type != HELLO_MESSAGE)
		printk("input_handler: input without process.\n");
#endif

	return NF_ACCEPT;

}

#ifdef REALSIM
void manage_location(interdata *idata)
{
	u_int32_t src_ip = idata->src_ip;
	u_int32_t y = idata->n_latitude; 
	u_int32_t x = idata->n_longitude;
	u_int32_t dis = idata->dis;
	node->latitude = y;
	node->longitude = x;

	#ifdef WWTAO_DEBUG_GENRREP
	printk("testWu:temq: manage_location,change node's longitude and lattitude,node->longitude(%u),node->latitude(%u)\n",node->longitude,node->latitude);
	#endif

//printk("----in manage location %d----\n",idata->dst_seq);
	
	if(idata->dst_seq != 0)//0 means it's a local query,others means it's a neigh query
	{
/*
#ifdef DEBUG2
		printk("The distance is %dm,seq is %d\n",dis,idata->dst_seq);
#endif
*/

		task *tmp_task = app_request_queue_get(idata->dst_seq);
	
		if(tmp_task==NULL)
		{
#ifdef DEBUG2
			printk("No this query task\n");
#endif
			return ;

		}
	
		if(dis < node->com_radius)//within communication range
		{

			//change the task fields
#ifdef WWTAO_DEBUG_GENRREP
printk("in the communication range\n");
printk("iAODV in RS: IP: %s The distance is %dm,seq is %d\n",inet_ntoa(tmp_task->src_ip),dis,idata->dst_seq);
#endif

			tmp_task->type = TASK_ADD_NEIGH;

			insert_task_at_front(tmp_task);


			update_timer_queue();
		}
		else //out of range,release task data & task
		{
#ifdef WWTAO_DEBUG_GENRREP
printk("out of communication range\n");
printk("iAODV in RS: IP: %s The distance is %dm,seq is %d\n",inet_ntoa(tmp_task->src_ip),dis,idata->dst_seq);
#endif
			if( tmp_task->data != NULL )
				kfree(tmp_task->data);
			if( tmp_task != NULL )
				kfree(tmp_task);

		}
		
		//update the dis record 2015.11.14
		update_dis_record(tmp_task->src_ip,dis);
	}
}
#endif
