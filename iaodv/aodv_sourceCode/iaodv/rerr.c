/***************************************************************************
 rerr.c  -  description
 -------------------
 begin                : Mon Aug 11 2003
 copyright            : (C) 2003 by Luke Klein-Berndt
 email                : kleinb@nist.gov
 ***************************************************************************/
/***************************************************************************
 Modified by Miguel Catalan Cid - miguel.catcid@gmail.com - Version: Mon Jan 1 2010
 ***************************************************************************/
#include "rerr.h"

extern u_int32_t g_mesh_ip;
extern u_int32_t g_null_ip;
extern u_int8_t g_aodv_gateway;
extern u_int8_t g_routing_metric;


int reply_to_rrer(u_int32_t source, u_int32_t destination) {


	if (source == g_mesh_ip || (source == g_null_ip && g_aodv_gateway))
		return 1;

	return 0;
}

int onehop_repair(u_int32_t neigh_name,u_int32_t neigh_ip)
{
	aodv_neigh *tmp_neigh = get_other_link2neigh(neigh_name,neigh_ip);


//printk("------neigh_name:%s-------\n",inet_ntoa(neigh_name));
	aodv_route *tmp_route;
	src_list_entry *tmp_src_entry;
	int error;
	if(tmp_neigh==NULL)
	{
#ifdef DEBUG2
		printk("No other link,gen rerr.\n");
#endif
		gen_rerr(neigh_ip);
	}
	else{
printk("------neigh_name:%s-------\n",inet_ntoa(neigh_name));
#ifdef DEBUG2
char nn[20];
char ni[20];
char nnh[20];
strcpy(nn,inet_ntoa(neigh_name));
strcpy(ni,inet_ntoa(neigh_ip));
strcpy(nnh,inet_ntoa(tmp_neigh->ip));
printk("------neigh_name:%s,neigh_ip:%s,new next_hop:%s-------\n",nn,ni,nnh);
#endif	
		tmp_route = first_aodv_route();

		while (tmp_route != NULL) {


			if ((tmp_route->next_hop == neigh_ip) && (!tmp_route->self_route)&& (!tmp_route->neigh_route))
			{
			
				tmp_src_entry = find_src_list_entry(tmp_route->src_ip);
	 			if (tmp_src_entry == NULL)
					return 0;
				//delete the old & broken route in kernel
				error = rpdb_route(RTM_DELROUTE, tmp_src_entry->rt_table,
			 				tmp_route->tos, tmp_route->src_ip, tmp_route->dst_ip,
			 				tmp_route->next_hop, tmp_route->dev->index,
			 				tmp_route->num_hops);
				if (error < 0) {
			 		printk ("Error sending with rtnetlink - Delete Route - err no: %d in rerr\n", error);
			 	return 0;
				}

				tmp_route->next_hop = tmp_neigh->ip;
				tmp_route->lifetime = getcurrtime() + DELETE_PERIOD;
				tmp_route->state = ACTIVE;
				tmp_route->dev = get_netdev_by_name(tmp_neigh->dev->name);
				//add a new route
				error = rpdb_route(RTM_NEWROUTE, tmp_src_entry->rt_table,
			 				tmp_route->tos, tmp_route->src_ip, tmp_route->dst_ip,
			 				tmp_route->next_hop, tmp_route->dev->index,
			 				tmp_route->num_hops);

				if (error < 0) {
			 		printk ("Error sending with rtnetlink - Add Route - err no: %d in rerr\n", error);
					gen_rerr(neigh_ip);
			 	return 0;
				}

			
			}
			tmp_route = tmp_route->next;
		}//end while
	}//end else
//printk("-------------end 1hop repair-----------\n");	
	return 1;

}
//brk_dst_ipΪ�ѶϿ���·��Ŀ��ip
int gen_rerr(u_int32_t brk_dst_ip) {
	aodv_route *tmp_route;
	rerr *tmp_rerr;
	int expired_routes = 0;

	//�ҵ���һ��aodv·��
	tmp_route = first_aodv_route();

//printk("  --gen_rerr--  \n");

	//go through list
	//����������һ��Ϊbrk_dst_ip��·����Ŀ�������ýڵ㷢��rerr����Ч����·��
	while (tmp_route != NULL) {

		//printk("tmp_route->next_hop is %s\n",inet_ntoa(tmp_route->next_hop));
		if ((tmp_route->next_hop == brk_dst_ip) && (!tmp_route->self_route)&& (!tmp_route->neigh_route)){

			if (!reply_to_rrer(tmp_route->src_ip, tmp_route->dst_ip)) { //i'm not the source of the route.if i'm, don't send the rerr
				if (tmp_route->state != INVALID) {

					if ((tmp_rerr = (rerr *) kmalloc(sizeof(rerr), GFP_ATOMIC))
							== NULL) {
#ifdef DEBUG
						printk("Can't allocate new rrer\n");
#endif
						return 0;
					}

					tmp_rerr->type = RERR_MESSAGE;
					tmp_rerr->dst_count = 0; //unused
					tmp_rerr->reserved = 0;
					tmp_rerr->n = 0;
					tmp_rerr->num_hops = 0;
					tmp_rerr->dst_ip = tmp_route->dst_ip;
					tmp_rerr->dst_seq = htonl(tmp_route->dst_seq);

#ifdef DTN
					//if there is DTN register,current node will be the
					//last DTN hop near the brk node
					extern int dtn_register;
					tmp_rerr->last_avail_ip = NULL;

					tmp_rerr->src_ip = tmp_route->src_ip;
					if(dtn_register==1)
					{
						tmp_rerr->last_avail_ip = g_mesh_ip;
			        //printk("last_avail_ip:%s\n",inet_ntoa(tmp_rerr->last_avail_ip));
						//notice DTN layer
#include <linux/sched.h>
//JL: Added kernel threads interface:
#include <linux/kthread.h>
						u_int32_t para[4];
						para[0] = tmp_rerr->src_ip;
						para[1] = tmp_rerr->dst_ip;
						para[2] = tmp_rerr->last_avail_ip;

						//���ʹ���
						para[3] = (u_int32_t)tmp_rerr->type;
						//
						send2dtn((void*)para,DTNPORT);

					}
					else tmp_rerr->last_avail_ip = NULL;
#endif

#ifdef CaiDebug
					//cai:test gen rerr
	char local[20];
 	strcpy(local,inet_ntoa(g_mesh_ip));
	char brk[20];
strcpy(brk,inet_ntoa(brk_dst_ip));
	char last[20];
strcpy(last,inet_ntoa(tmp_route->last_hop));

	printk("rerr src is %s,brk_dst_ip is %s,the last hop is %s\n",local,brk,last);
	
#endif
                    send_message(tmp_route->last_hop, NET_DIAMETER, tmp_rerr,sizeof(rerr),tmp_route->dev);
                     kfree(tmp_rerr);

#ifdef RECOVERYPATH
                    /****************************************************
                        ������·��������ÿ������һ���µ�rerr��ʱ��Ҳ
                        �½�һ��brk_list����Ŀ
                    *****************************************************/

                    brk_link *tmp_link;
                    u_int32_t lastavail=NULL;
#ifdef DTN
                    if(dtn_register==1)
                    {
                            lastavail = g_mesh_ip;
                    }
#endif
                    tmp_link = find_brk_link(tmp_route->src_ip,tmp_route->dst_ip);
                    if(tmp_link != NULL){//�Ѵ��ڣ������������ڣ������������ٳ���
                        //Update brk_link timelife������Ҫ��ʱ���Ĵ���
                        tmp_link->lifetime = getcurrtime() + BRK_LINK_TIMEOUT;
                        flush_brk_list();

                    }
                    else{
                        tmp_link = create_brk_link(tmp_route->src_ip,tmp_route->dst_ip,
                                               tmp_route->last_hop,lastavail);//����������
                        if(tmp_link==NULL) printk("Creat new brk link failed!\n");
#ifdef CaiDebug
                        char s[20];
                        char d[20];
                        char l[20];
                        char la[20];
                        strcpy(s,inet_ntoa(tmp_route->src_ip));
                        strcpy(d,inet_ntoa(tmp_route->dst_ip));
                        strcpy(l,inet_ntoa(tmp_route->last_hop));
                        strcpy(la,inet_ntoa(lastavail));
                        printk("new brk link %s:%s:%s:%s\n",s,d,l,la);
#endif
                        flush_brk_list();
                    }
                    /******************************************************************/
#endif


				}//route is not invalid


			}//not the source of the route
#ifdef RECOVERYPATH
			else{		    
				/****************************************************         					*****************************************************/
#ifdef DTN
					//if there is DTN register,current node will be the
					//last DTN hop near the brk node
					extern int dtn_register;
					u_int32_t last_avail;//put them out of #ifdef DTN or in is a Q 
					last_avail = NULL;
					if(dtn_register==1)
					{
						last_avail = g_mesh_ip;
						//notice DTN layer
#include <linux/sched.h>
//JL: Added kernel threads interface:
#include <linux/kthread.h>
						u_int32_t para[4];
						para[0] = tmp_route->src_ip;
						para[1] = tmp_route->dst_ip;
						para[2] = last_avail;

						//���ʹ���
						para[3] = (u_int32_t)RERR_MESSAGE;
						//
						send2dtn((void*)para,DTNPORT);

					}
#endif
				brk_link *tmp_link;
				tmp_link = find_brk_link(tmp_route->src_ip,tmp_route->dst_ip);
				if(tmp_link != NULL){
					tmp_link->lifetime = getcurrtime() + BRK_LINK_TIMEOUT;
					flush_brk_list();                    
				}
				else{
				tmp_link = create_brk_link(tmp_route->src_ip,tmp_route->dst_ip,
								NULL,last_avail);//notice the last hop is NULL
				if(tmp_link==NULL) printk("Creat new brk link failed!\n");
#ifdef CaiDebug
char s[20];
char d[20];
char l[20];
char la[20];
strcpy(s,inet_ntoa(tmp_route->src_ip));
strcpy(d,inet_ntoa(tmp_route->dst_ip));
strcpy(l,"0.0.0.0");
strcpy(la,inet_ntoa(last_avail));
printk("new brk link %s:%s:%s:%s\n",s,d,l,la);
#endif
				flush_brk_list();
				}                    
				/******************************************************************/
			}//the source
#endif

			if (tmp_route->state != INVALID){

				expire_aodv_route(tmp_route);
				expired_routes++;
#ifdef CaiDebug
	char local[20];
 	strcpy(local,inet_ntoa(g_mesh_ip));
	char last[20];
	strcpy(last,inet_ntoa(tmp_route->last_hop));
	printk("invalidate tmp_route %s to %s,expored_route is %d\n",local,last,expired_routes);
#endif
			}
		}
		//for neigh link break //add 20150912
		else if ((tmp_route->next_hop == brk_dst_ip) && (tmp_route->neigh_route)){
			if (reply_to_rrer(tmp_route->src_ip, tmp_route->dst_ip)) { //i'm the source of the route.if i'm, don't send the rerr
				if (tmp_route->state != INVALID) {
					expire_aodv_route(tmp_route);
					expired_routes++;
				}
				
			}
		}
		//move on to the next entry
		tmp_route = tmp_route->next;

	}

#ifdef DEBUG2
//////look at route table

	tmp_route = first_aodv_route();

	while (tmp_route != NULL) {

		printk("tmp_route->next_hop is %s\n",inet_ntoa(tmp_route->next_hop));
		
		//move on to the next entry
		tmp_route = tmp_route->next;

	}

#endif

/*
	if (g_routing_metric == WCIM && expired_routes != 0)

	update_my_load();
*/

	return 0;
}


int recv_rerr(task * tmp_packet) {
	aodv_route *tmp_route;
	rerr *tmp_rerr;
	int num_hops;
	int expired_routes = 0;

	tmp_rerr = (rerr *) tmp_packet->data;
	num_hops = tmp_rerr->num_hops+1;

#ifdef DEBUG2
	printk("	Recieved a route error from %s\n", inet_ntoa(tmp_packet->src_ip));
#endif

#ifdef DTN

	extern int dtn_register;
	int first_dtn=0;

#include <linux/sched.h>
//JL: Added kernel threads interface:
#include <linux/kthread.h>
	u_int32_t para[4];
	para[0] = tmp_rerr->src_ip;
	para[1] = tmp_rerr->dst_ip;
	para[2] = tmp_rerr->last_avail_ip;
	para[3] = (u_int32_t)tmp_rerr->type;

	if( dtn_register==1 ){
	    //����ע��DTN����lastavailΪ�ջ��߽��յ���tmp_rerr��СС�ڶ�����DTN��rerr
		if( para[2]==NULL )//current node is the nearest DTN node to brk link
		{
			para[2] = g_mesh_ip;
			first_dtn = 1;
		}
//		kthread_run(&send2dtn, (void*)para, "send2dtn");
		send2dtn((void*)para,DTNPORT);//tmp_rerr->dst_ip, tmp_rerr->last_avail_ip);
	}

#endif

    tmp_route
			= find_aodv_route_by_id(tmp_rerr->dst_ip, ntohl(tmp_rerr->dst_seq));

	if (tmp_route && tmp_route->next_hop == tmp_packet->src_ip) {
		//if there is any hop before me

		if (!reply_to_rrer(tmp_route->src_ip, tmp_route->dst_ip)) {

			if (tmp_route->state != INVALID) { //route with active traffic

				tmp_rerr->type = RERR_MESSAGE;
				tmp_rerr->dst_count = 0; //unused
				tmp_rerr->reserved = 0;
				tmp_rerr->n = 0;
				tmp_rerr->num_hops = num_hops;
				tmp_rerr->dst_ip = tmp_route->dst_ip;
				tmp_rerr->dst_seq = htonl(tmp_route->dst_seq);

#ifdef DTN
				tmp_rerr->src_ip = tmp_route->src_ip;

				if(dtn_register==1 && first_dtn==1)
				{
					tmp_rerr->last_avail_ip = g_mesh_ip;
				}
#endif


				send_message(tmp_route->last_hop, NET_DIAMETER, tmp_rerr,
						sizeof(rerr),tmp_route->dev);

#ifdef RECOVERYPATH
                    /****************************************************
                        ������·��������ÿ������һ���µ�rerr��ʱ��Ҳ
                        �½�һ��brk_list����Ŀ
                    *****************************************************/
                    brk_link *tmp_link;

                    tmp_link = find_brk_link(tmp_route->src_ip,tmp_route->dst_ip);
                    if(tmp_link != NULL){//�Ѵ��ڣ������������ڣ������������ٳ���
                        //Update brk_link timelife������Ҫ��ʱ���Ĵ���
                        tmp_link->lifetime = getcurrtime() + BRK_LINK_TIMEOUT;
                        flush_brk_list();

                    }
                    else{
                        tmp_link = create_brk_link(tmp_route->src_ip,tmp_route->dst_ip,
                                               tmp_route->last_hop,tmp_rerr->last_avail_ip);//����������,ע���˴���ast_avail_ipӦ���Զ�·��

                        if(tmp_link==NULL) printk("Creat new brk link failed!\n");
#ifdef CaiDebug
                        char s[20];
                        char d[20];
                        char l[20];
                        char la[20];
                        strcpy(s,inet_ntoa(tmp_route->src_ip));
                        strcpy(d,inet_ntoa(tmp_route->dst_ip));
                        strcpy(l,inet_ntoa(tmp_route->last_hop));
                        strcpy(la,inet_ntoa(tmp_rerr->last_avail_ip));
                        printk("new brk link %s:%s:%s:%s\n",s,d,l,la);
#endif
                        flush_brk_list();
                    }
                    /******************************************************************/
#endif




			}

		}//not the source
#ifdef RECOVERYPATH
		else{
		    /****************************************************
                        ������·��������ÿ������һ���µ�rerr��ʱ��Ҳ
                        �½�һ��brk_list����Ŀ

                    *****************************************************/
#ifdef DTN
		    
			u_int32_t last_avail;
			last_avail = NULL;
			if(dtn_register==1 && first_dtn==1)
			{	
				last_avail = g_mesh_ip;
			}
#endif
                    brk_link *tmp_link;

                    tmp_link = find_brk_link(tmp_route->src_ip,tmp_route->dst_ip);
                    if(tmp_link != NULL){//�Ѵ��ڣ������������ڣ������������ٳ���
                        //Update brk_link timelife������Ҫ��ʱ���Ĵ���
                        tmp_link->lifetime = getcurrtime() + BRK_LINK_TIMEOUT;
                        flush_brk_list();

                    }
                    else{
                        tmp_link = create_brk_link(tmp_route->src_ip,tmp_route->dst_ip,
                                               NULL,last_avail);//notice the last hop is NULL

                        if(tmp_link==NULL) printk("Creat new brk link failed!\n");
#ifdef CaiDebug
                        char s[20];
                        char d[20];
                        char l[20];
                        char la[20];
                        strcpy(s,inet_ntoa(tmp_route->src_ip));
                        strcpy(d,inet_ntoa(tmp_route->dst_ip));
                        strcpy(l,"0.0.0.0");
                        strcpy(la,inet_ntoa(last_avail));
                        printk("new brk link %s:%s:%s:%s\n",s,d,l,la);

#endif
                        flush_brk_list();
                    }
                    /******************************************************************/
		}//the source
#endif

		if (tmp_route->state != INVALID) {
			expire_aodv_route(tmp_route);
			expired_routes++;
		}

	}
/*
	if (g_routing_metric == WCIM && expired_routes != 0)
	update_my_load();
*/

	return 0;

}

