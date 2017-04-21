/***************************************************************************
aodv_config.h  -  description
-------------------
begin                : FRI NOV 14 2014
copyright            : (C) 2014 by Cai Bingying
email                :
***************************************************************************/
#include "aodv_config.h"

#define max_dev_num 10


extern aodv_dev *net_dev_list;
extern aodv_route *g_local_route;
extern u_int8_t g_routing_metric;
//extern u_int32_t g_node_name;
extern node_info *node;
u_int32_t local_ip_list[max_dev_num];

int get_node_type()
{
	int i_wireless=0;
	int i_wired=0;

	aodv_dev *tmp_dev = net_dev_list;
	node->g_node_name = tmp_dev->ip;	
	
	while(tmp_dev)
	{
		//record the ips of all netdevices
		//element 0 is the num of ip 
		local_ip_list[i_wireless+i_wired+1] = tmp_dev->ip;
		//set the name of node as the smallest ip
		if(node->g_node_name > tmp_dev->ip)
			node->g_node_name = tmp_dev->ip;
		if(strcmp(tmp_dev->name,"adhoc0")==0)
			i_wireless ++;
		else if(strncmp(tmp_dev->name,"eth",3)==0)
			i_wired ++;
#ifdef	DEBUG2
	printk("==========dev:%s	%s============\n",tmp_dev->name,inet_ntoa(tmp_dev->ip));
#endif
		tmp_dev = tmp_dev->next;
	}
	
	local_ip_list[0] = i_wireless + i_wired;
	node->g_ip_num = local_ip_list[0];	
	
	if(i_wireless>0 && i_wired==0)
		return MN_NODE;
	else if(i_wireless==0 && i_wired>0)
		return WDN_NODE;
	else if(i_wireless>0 && i_wired>0)
		return ICN_NODE;
	return 0;
}

bool is_local_ip(u_int32_t ip)
{
	int i = 1;
	while(i <= local_ip_list[0])
	{
		if( ip==local_ip_list[i] )
			return true;
		i++;
	}
	return false;

}

int route_neigh(u_int32_t src_ip, u_int32_t dst_ip, unsigned char tos)
{
	//printk("task route neigh\n");
	//extern u_int8_t g_node_type;
	aodv_neigh *tmp_neigh = find_aodv_neigh(dst_ip);
	
	aodv_route *tmp_route;
	if( (tmp_neigh->neigh_type != ICN_NODE) || (node->g_node_type != ICN_NODE) )
	{
		rreq_aodv_route(src_ip,dst_ip,tos,tmp_neigh,1,g_local_route->dst_seq,tmp_neigh->dev,g_routing_metric,0);
		
	}
	else
	{
		if( strncmp(tmp_neigh->dev->name,"eth",3)==0 )
		{			
			rreq_aodv_route(src_ip,dst_ip,tos,tmp_neigh,1,g_local_route->dst_seq,tmp_neigh->dev,g_routing_metric,0);
		}
		else
		{	
			
			aodv_neigh *better = get_better_link(tmp_neigh);
			if(better)	
			{
				if(rreq_aodv_route(src_ip,dst_ip,tos,better,1,g_local_route->dst_seq,better->dev,g_routing_metric,0))
				printk("---------better:%s--------\n",inet_ntoa(better->ip));
			}
			else
				rreq_aodv_route(src_ip,dst_ip,tos,tmp_neigh,1,g_local_route->dst_seq,tmp_neigh->dev,g_routing_metric,0);

		}	
	}
	

	tmp_route = find_aodv_route(src_ip,dst_ip,tos);
	if (tmp_route == NULL) 
		return 0;
	
	tmp_route->num_hops = 1;
	tmp_route->path_metric = 1;
	tmp_route->state = ACTIVE;
	tmp_route->neigh_route = TRUE;
	tmp_route->self_route = TRUE;
	
	src_list_entry *tmp_src_entry = find_src_list_entry(src_ip);
		if (tmp_src_entry == NULL)
			return 0;	
	int error = rpdb_route(RTM_NEWROUTE, tmp_src_entry->rt_table, 0, src_ip,
			tmp_route->next_hop, tmp_route->next_hop, tmp_route->dev->index, 1);

	if (error < 0) {
		printk(
				"Error sending with rtnetlink - Insert Route - err no: %d\n",
				error);
		kfree(tmp_route);
		return 0;

	}


	return 1;

}
/*
void set_config_params(u_int8_t node_type)
{
	int times;
	switch(node_type)
	{
		case MN_NODE: times=1;break;
		case WDN_NODE: times=2;break;
		case ICN_NODE: times=1;break;
		default: times=1;break;
	}
	CONFIG.ACTIVE_ROUTE_TIMEOUT;
	CONFIG.BRK_LINK_TIMEOUT;
	CONFIG.ALLOWED_HELLO_LOSS;//MCC - Changed to increment the possibility of having a bad ETX metric
	CONFIG.BLACKLIST_TIMEOUT;//RREQ_RETRIES * NET_TRAVERSAL_TIME
	CONFIG.DELETE_PERIOD;//ALLOWED_HELLO_LOSS * HELLO_INTERVAL
	CONFIG.HELLO_INTERVAL;//1000
	CONFIG.LOCAL_ADD_TTL;//2
	CONFIG.MAX_REPAIR_TTL;//0.3 * NET_DIAMETER
	CONFIG.MY_ROUTE_TIMEOUT;//ACTIVE_ROUTE_TIMEOUT
	CONFIG.NET_DIAMETER;//10
	CONFIG.NODE_TRAVERSAL_TIME;//40
	CONFIG.NET_TRAVERSAL_TIME;//2 * NODE_TRAVERSAL_TIME * NET_DIAMETER
	CONFIG.NEXT_HOP_WAIT;//NODE_TRAVERSAL_TIME + 10
	CONFIG.PATH_DISCOVERY_TIME;//2 * NET_TRAVERSAL_TIME
	CONFIG.RERR_RATELIMIT;//10
	CONFIG.RING_TRAVERSAL_TIME;//2 * NODE_TRAVERSAL_TIME * ( TTL_VALUE + TIMEOUT_BUFFER)
	CONFIG.RREQ_RETRIES;//2
	CONFIG.RREQ_RATELIMIT;//10
	CONFIG.TIMEOUT_BUFFER;//2
	CONFIG.TTL_START;//2
	CONFIG.TTL_INCREMENT;//2
	CONFIG.TTL_THRESHOLD;//7
	CONFIG.TTL_VALUE;//3
	CONFIG.DTTL;//15
	CONFIG.ST_INTERVAL;//10000
	CONFIG.ACTIVE_GWROUTE_TIMEOUT;//(ST_INTERVAL * 2) + 1000
	CONFIG.TIMEOUT_DELAY_ROUTE_UPDATE;//1000 // 1 second //MCC - Unused

}
*/


