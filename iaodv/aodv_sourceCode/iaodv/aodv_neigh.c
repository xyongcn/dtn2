/***************************************************************************
 aodv_neigh.c  -  description
 -------------------
 begin                : Thu Jul 31 2003
 copyright            : (C) 2003 by Luke Klein-Berndt
 email                : kleinb@nist.gov
 ***************************************************************************/
/***************************************************************************
 Modified by Miguel Catalan Cid - miguel.catcid@gmail.com - Version: Mon Jan 1 2010
 ***************************************************************************/
#include "aodv_neigh.h"


extern u_int32_t g_mesh_ip;
extern u_int8_t g_routing_metric;
extern u_int16_t g_fixed_rate; 
extern aodv_neigh_2h *aodv_neigh_list_2h;
extern aodv_dev *g_mesh_dev;
extern aodv_route *g_local_route;

aodv_neigh *aodv_neigh_list;

//rwlock_t neigh_lock = RW_LOCK_UNLOCKED;
DEFINE_RWLOCK(neigh_lock);

void neigh_read_lock(void) {
	read_lock_bh(&neigh_lock);
}

void neigh_read_unlock(void) {
	read_unlock_bh(&neigh_lock);
}

void neigh_write_lock(void) {
	write_lock_bh(&neigh_lock);
}

void neigh_write_unlock(void) {
	write_unlock_bh(&neigh_lock);
}

void init_aodv_neigh_list(void) {
	aodv_neigh_list=NULL;
}

void delete_aodv_neigh_list(void) {
	aodv_neigh *tmp_entry = aodv_neigh_list;
	aodv_neigh *prev_entry = NULL;
	while (tmp_entry != NULL) {
		prev_entry = tmp_entry;
		tmp_entry = tmp_entry->next;
		kfree(prev_entry);
	}
}



aodv_neigh *find_aodv_neigh(u_int32_t target_ip) {
	aodv_neigh *tmp_neigh;
	neigh_read_lock(); //find_aodv_neigh is used in packet_out.c (uncontrolled interruption)
	tmp_neigh = aodv_neigh_list;

	while ((tmp_neigh != NULL) && (tmp_neigh->ip <= target_ip)) {
		if (tmp_neigh->ip == target_ip) {

			neigh_read_unlock();
			return tmp_neigh;
		}
		tmp_neigh = tmp_neigh->next;
	}

	neigh_read_unlock();

	return NULL;
}

int delete_aodv_neigh(u_int32_t ip) {
	aodv_neigh *tmp_neigh;
	aodv_route *tmp_route;
	aodv_neigh *prev_neigh = NULL;
	src_list_entry *tmp_src_entry;
	int index;

	neigh_write_lock(); //to avoid conflicts with read_neigh_proc and packet_out.c (uncontrolled interruption)
	tmp_neigh = aodv_neigh_list;

	while (tmp_neigh != NULL) {

		if (tmp_neigh->ip == ip) {
			index = tmp_neigh->dev->ifindex;

			if (prev_neigh != NULL) {
				prev_neigh->next = tmp_neigh->next;
			} else

			{
				aodv_neigh_list = tmp_neigh->next;
			}

			neigh_write_unlock();



/** Cai:Del the following codes because they remove the route entry in kernel before
  * managing the broken link. We should genarate rerr and send it out first,then just
  * expires the route entry.
**/
/*
			tmp_src_entry = find_src_list_entry(g_mesh_ip);
			if (tmp_src_entry != NULL) {

				int error = rpdb_route(RTM_DELROUTE, tmp_src_entry->rt_table,
						NO_TOS, g_mesh_ip, ip, ip, index, 1);
				if (error < 0) {
					printk("Error sending with rtnetlink - Delete Route - err no: %d\n",error);

				}
			}*/
			
			//tmp_route = find_aodv_route(g_mesh_ip, ip, 0);
			//		if (tmp_route)
			//			remove_aodv_route(tmp_route);      

			
//20141213 try to repair one hop link before gen_rerr
			onehop_repair(tmp_neigh->neigh_name,ip);
//printk("try one hop repair\n");
			kfree(tmp_neigh);
			//gen_rerr(ip);
			return 0;

		}
		prev_neigh = tmp_neigh;
		tmp_neigh = tmp_neigh->next;
	}

#ifdef DEBUG2
	
	tmp_neigh = aodv_neigh_list;
	while (tmp_neigh != NULL) {

		printk("neigh_name in delete neigh %s",inet_ntoa(tmp_neigh->neigh_name));
		tmp_neigh = tmp_neigh->next;
	}

#endif

	neigh_write_unlock();

	return -ENODATA;
}



void cleanup_neigh_routes() {

	aodv_neigh *tmp_neigh;
	aodv_route *tmp_route;
	int error;
	src_list_entry *tmp_src_entry;

	tmp_neigh = aodv_neigh_list;

	extern u_int32_t local_ip_list[10];
	int i = 1;
//change at 20141219 by cai
	for(i=1;i<=local_ip_list[0];i++)
	{

	tmp_src_entry = find_src_list_entry(local_ip_list[i]);
	if (tmp_src_entry == NULL)
		continue;//return;

	aodv_dev *tmp_dev = get_netdev_by_ip(local_ip_list[i]);
#ifdef DEBUG2
printk("%d	%s	%d\n",i,inet_ntoa(local_ip_list[i]),tmp_dev->index);
#endif
	tmp_neigh = aodv_neigh_list;

	while (tmp_neigh) {

		//error = rpdb_route(RTM_DELROUTE, tmp_src_entry->rt_table, NO_TOS,
		//		local_ip_list[i], tmp_neigh->ip, tmp_neigh->ip, tmp_neigh->dev->ifindex,
		//		1);
		error = rpdb_route(RTM_DELROUTE, tmp_src_entry->rt_table, NO_TOS,
				local_ip_list[i], tmp_neigh->ip, tmp_neigh->ip, tmp_dev->index,
				1);

		if (error < 0) {
			printk(
					"Error sending with rtnetlink - Delete Route - err no: %d\n",
					error);

		}
		
		tmp_route = find_aodv_route(local_ip_list[i], tmp_neigh->ip, 0);

		if (tmp_route!=NULL)
			remove_aodv_route(tmp_route);
		
		tmp_neigh = tmp_neigh->next;

	}
}
}

aodv_neigh *create_aodv_neigh(u_int32_t neigh_name,u_int8_t neigh_type,u_int32_t ip,struct net_device *in_dev) {
	aodv_neigh *new_neigh;
	aodv_neigh *prev_neigh = NULL;
	aodv_neigh *tmp_neigh = NULL;
	u_int64_t jitter;
	int rand;
	int i;
	
	if ((new_neigh = kmalloc(sizeof(aodv_neigh), GFP_ATOMIC)) == NULL) {
#ifdef DEBUG
		printk("NEIGHBOR_LIST: Can't allocate new entry\n");
#endif
		return NULL;
	}

	tmp_neigh = aodv_neigh_list;
	while ((tmp_neigh != NULL) && (tmp_neigh->ip < ip)) {
		prev_neigh = tmp_neigh;
		tmp_neigh = tmp_neigh->next;
	}

	if (tmp_neigh && (tmp_neigh->ip == ip)) {
#ifdef DEBUG
		printk("AODV_NEIGH: Creating a duplicate neighbor\n");
#endif
		kfree(new_neigh);
		return NULL;
	}
//delay the neigh route setting
/*	
	if (!route_aodv_neigh(ip,in_dev)){
#ifdef DEBUG
		printk("AODV_NEIGH: Error in kernel route creation\n");
#endif
		kfree(new_neigh);
		return NULL;
	}
*/
			
	neigh_write_lock(); //to avoid conflicts with read_neigh_proc and packet_out.c (uncontrolled interruption)
//#ifdef DEBUGC
	new_neigh->neigh_name = neigh_name;
	new_neigh->neigh_type = neigh_type;
//#endif
	new_neigh->ip = ip;
	new_neigh->lifetime = 0;
	new_neigh->etx_metric = new_neigh->recv_rate = 0; 
	new_neigh->send_rate = g_fixed_rate;
//add dev
	new_neigh->dev = in_dev;
#ifdef DEBUG2
	printk("The neigh's income dev is %s in create_aodv_neigh\n",in_dev->name);
#endif
//
	new_neigh->next = NULL;
	
/*
	//ETX metric initialization
	new_neigh->etx.count=0;
	new_neigh->etx.send_etx=0;
	new_neigh->etx.recv_etx=0;
*/
//there are something about metric del by cai at 20141023
	
	if (prev_neigh == NULL) {
		new_neigh->next = aodv_neigh_list;
		aodv_neigh_list = new_neigh;
	} else {
		new_neigh->next = prev_neigh->next;
		prev_neigh->next = new_neigh;
	}
	
	neigh_write_unlock();

	
	return new_neigh;
}

int route_aodv_neigh(u_int32_t neigh_ip,struct net_device *in_dev) {
	src_list_entry *tmp_src_entry;
	aodv_route *tmp_route;
	int error; 
	
	aodv_dev *tmp_dev = get_netdev_by_name(in_dev->name);
	if(tmp_dev==NULL) return 0;

	tmp_route = create_aodv_route(tmp_dev->ip, neigh_ip, 0, g_local_route->dst_seq,in_dev);

	if (tmp_route == NULL) 
		return 0;
	
	tmp_route->num_hops = 1;
	tmp_route->path_metric = 1;
	tmp_route->dev = tmp_dev;
#ifdef DEBUG2
	printk("The neigh's route->dev is %s in route_aodv_neigh\n",tmp_dev->name);
#endif
	tmp_route->state = ACTIVE;
	tmp_route->neigh_route = TRUE;
	tmp_route->self_route = TRUE;
	
	tmp_src_entry = find_src_list_entry(tmp_dev->ip);
		if (tmp_src_entry == NULL)
			return 0;	
	error = rpdb_route(RTM_NEWROUTE, tmp_src_entry->rt_table, 0, g_mesh_ip,
			neigh_ip, neigh_ip, tmp_dev->index, 1);

	if (error < 0) {
		printk(
				"Error sending with rtnetlink - Insert Route - err no: %d\n",
				error);
		kfree(tmp_route);
		return 0;

	}

	return 1;

}

aodv_neigh *first_aodv_neigh() {
	return aodv_neigh_list;
}


int read_neigh_proc(char *buffer, char **buffer_location, off_t offset,
		int buffer_length, int *eof, void *data) {
	char tmp_buffer[200];
	aodv_neigh *tmp_neigh;
	u_int64_t remainder, numerator;
	u_int64_t tmp_time;
	int len;
	u_int64_t currtime;
	aodv_neigh_2h *tmp_neigh_2h;

	currtime=getcurrtime();

	neigh_read_lock();

	tmp_neigh = aodv_neigh_list;
	tmp_neigh_2h = aodv_neigh_list_2h;

	sprintf(
			buffer,
			"\nAODV Neighbors\n------------------------------------------------------------------------------------------------\n");
	sprintf(
			tmp_buffer,
			"       IP       | Hops |     Lifetime     | Send Rate | Recv Rate | ETX |\n");

	strcat(buffer, tmp_buffer);
	sprintf(
			tmp_buffer,
			"------------------------------------------------------------------------------------------------\n");
	strcat(buffer, tmp_buffer);
	while (tmp_neigh!=NULL) {

		sprintf(tmp_buffer, "   %-16s", inet_ntoa(tmp_neigh->ip));
		strcat(buffer, tmp_buffer);

		strcat(buffer, " 1Hop ");

		tmp_time=tmp_neigh->lifetime-currtime;
		numerator = (tmp_time);
		remainder = do_div(numerator, 1000);
		if (time_before((unsigned long) tmp_neigh->lifetime,
				(unsigned long) currtime))
			sprintf(tmp_buffer, " Expired!");

		else
			sprintf(tmp_buffer, " sec/msec: %2lu/%3lu ",
					(unsigned long)numerator, (unsigned long)remainder );

		strcat(buffer, tmp_buffer);

		sprintf(tmp_buffer, "   %5d Kbps   %5d Kbps  %3d\n",
				 tmp_neigh->send_rate*100,
				tmp_neigh->recv_rate*100, tmp_neigh->etx_metric);
		strcat(buffer, tmp_buffer);

		tmp_neigh=tmp_neigh->next;

	}

	while (tmp_neigh_2h!=NULL) {
		sprintf(tmp_buffer, "   %-16s", inet_ntoa(tmp_neigh_2h->ip));
		strcat(buffer, tmp_buffer);

		strcat(buffer, " 2Hops ");

		tmp_time=tmp_neigh_2h->lifetime-currtime;
		numerator = (tmp_time);
		remainder = do_div(numerator, 1000);
		if (time_before((unsigned long) tmp_neigh_2h->lifetime,
				(unsigned long) currtime) ) {
			sprintf(tmp_buffer, " Expired!");
		} else {
			sprintf(tmp_buffer, " sec/msec: %lu/%lu ",
					(unsigned long)numerator, (unsigned long)remainder );
		}

		strcat(buffer, tmp_buffer);
		sprintf(tmp_buffer, "\n");
		strcat(buffer, tmp_buffer);
		tmp_neigh_2h=tmp_neigh_2h->next;

	}
	strcat(
			buffer,
			"------------------------------------------------------------------------------------------------\n\n");

	len = strlen(buffer);
	*buffer_location = buffer + offset;
	len -= offset;
	if (len > buffer_length)
		len = buffer_length;
	else if (len < 0)
		len = 0;

	neigh_read_unlock();

return len;
}

aodv_neigh *get_better_link(aodv_neigh *cur_neigh)
{
	u_int32_t neigh_name=cur_neigh->neigh_name;
	aodv_neigh *tmp_neigh = aodv_neigh_list;

	while(tmp_neigh)
	{	
		if( (tmp_neigh->neigh_name==neigh_name) && (strncmp(tmp_neigh->dev->name,"eth",3)==0) )
			return tmp_neigh;
		tmp_neigh = tmp_neigh->next;
	}
	return NULL;
}

aodv_neigh* get_other_link2neigh(u_int32_t neigh_name,u_int32_t neigh_ip)
{
	aodv_neigh *tmp_neigh = aodv_neigh_list;
	while(tmp_neigh)
	{	
		if( (tmp_neigh->neigh_name==neigh_name)&&(tmp_neigh->ip!=neigh_ip) )
			return tmp_neigh;
		tmp_neigh = tmp_neigh->next;
	}
	return NULL;
}

/*
aodv_neigh *find_neigh_by_name(u_int32_t name)
{
	aodv_neigh *tmp_neigh = aodv_neigh_list;
	while(tmp_neigh)
	{	
		if(tmp_neigh->neigh_name == name)
			return tmp_neigh;
		tmp_neigh = tmp_neigh->next;
	}
	return NULL;
}*/




/*
int read_rel_list_proc(char *buffer, char **buffer_location, off_t offset,
		int buffer_length, int *eof, void *data) {
	static char *my_buffer;
	char temp_buffer[400];
	char dst[16];
	aodv_neigh *tmp_neigh =aodv_neigh_list;

	int len, size;

	size = sizeof(l_probe)+28;


	my_buffer = buffer;

	neigh_read_lock();
	sprintf(my_buffer, "\n Reliability Table \n"
		"----------------------------------------------------------------\n");
	sprintf(temp_buffer,
			"      IP      | Count ETX | Previous RDR | FDR | TxPercentage |\n");
	strcat(my_buffer, temp_buffer);
	sprintf(temp_buffer,
			"----------------------------------------------------------------\n");

	strcat(my_buffer, temp_buffer);

	while (tmp_neigh != NULL) {
		strcpy(dst, inet_ntoa(tmp_neigh->ip));
		

		sprintf(temp_buffer,
				"  %-16s %2d             %2d        %2d        %d    \n", dst,
				tmp_neigh->etx.count, tmp_neigh->etx.recv_etx, tmp_neigh->etx.send_etx,
				tmp_neigh->etx_metric);

		strcat(my_buffer, temp_buffer);
		
		tmp_neigh = tmp_neigh->next;
	}



	strcat(my_buffer,
			"\n----------------------------------------------------------------\n\n");

	len = strlen(my_buffer);
	*buffer_location = my_buffer + offset;
	len -= offset;
	if (len > buffer_length)
		len = buffer_length;
	else if (len < 0)
		len = 0;
	neigh_read_unlock();
	return len;
}
*/

int read_node_load_proc(char *buffer, char **buffer_location, off_t offset,
		int buffer_length, int *eof, void *data) {
	char tmp_buffer[200];
	int len;

	aodv_neigh *tmp_neigh = aodv_neigh_list;
	aodv_neigh_2h *tmp_neigh_2h = aodv_neigh_list_2h;

	neigh_read_lock();
	sprintf(
			buffer,
			"\nAODV Neighbor Loads\n------------------------------------------------------------------------\n");
	sprintf(tmp_buffer,
			"        IP        |    SEQ    |  LOAD  |  Hops  |\n");
	strcat(buffer, tmp_buffer);
	sprintf(tmp_buffer,
			"------------------------------------------------------------------------\n");
	strcat(buffer, tmp_buffer);

	sprintf(tmp_buffer, "   %-16s     ", inet_ntoa(g_mesh_dev->ip));
	strcat(buffer, tmp_buffer);

	sprintf(tmp_buffer, "%5d      %3d      %1s\n",
			g_mesh_dev->load_seq, g_mesh_dev->load, "0");
	strcat(buffer, tmp_buffer);

	while (tmp_neigh != NULL) {

		
			sprintf(tmp_buffer, "   %-16s     ", inet_ntoa(tmp_neigh->ip));
			strcat(buffer, tmp_buffer);

			sprintf(tmp_buffer, "%5d      %3d      %1s\n",
					tmp_neigh->load_metric.load_seq, tmp_neigh->load_metric.load, "1");
			strcat(buffer, tmp_buffer);
		
			tmp_neigh=tmp_neigh->next;

	}
	
	while (tmp_neigh_2h != NULL) {

		
			sprintf(tmp_buffer, "   %-16s     ", inet_ntoa(tmp_neigh_2h->ip));
			strcat(buffer, tmp_buffer);

			sprintf(tmp_buffer, "%5d      %3d      %1s\n",
					tmp_neigh_2h->load_seq, tmp_neigh_2h->load, "2");
			strcat(buffer, tmp_buffer);
		
			tmp_neigh_2h=tmp_neigh_2h->next;

	}
	

	strcat(buffer,
			"------------------------------------------------------------------------\n\n");

	
	len = strlen(buffer);
	*buffer_location = buffer + offset;
	len -= offset;
	if (len > buffer_length)
		len = buffer_length;
	else if (len < 0)
		len = 0;

	neigh_read_unlock();
	return len;
}
