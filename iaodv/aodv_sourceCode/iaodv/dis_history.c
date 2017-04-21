/***************************************************************************
 dis_history.c  -  description
 -------------------
 begin                : Sat NOV 14
 copyright            : (C) 2015 by Cai Bingying
 ***************************************************************************/
#include "dis_history.h"
#ifdef REALSIM

DEFINE_RWLOCK(dis_history_lock);

dis_record *dis_history;

void dis_histoty_read_lock(void) {
	read_lock_bh(&dis_history_lock);
}

void dis_histoty_read_unlock(void) {
	read_unlock_bh(&dis_history_lock);
}

void dis_histoty_write_lock(void) {
	write_lock_bh(&dis_history_lock);
}

void dis_histoty_write_unlock(void) {
	write_unlock_bh(&dis_history_lock);
}

bool is_dis_history_empty(void){

	if(dis_history==NULL)
		return true;

	return false;
}

void init_dis_history(void) {
	dis_history = NULL;
}

void del_dis_history(void){

	dis_record *tmp_entry = dis_history;
	dis_record *prev_entry = NULL;
	while (tmp_entry != NULL) {
		prev_entry = tmp_entry;
		tmp_entry = tmp_entry->next;
		kfree(prev_entry);
	}
}

//insert new entry by ip incre order
dis_record* insert_dis_record(u_int32_t ip,u_int32_t dis){

	dis_record *new_record;
	dis_record *prev_record = NULL;
	dis_record *tmp_record = NULL;
	
	
	if ((new_record = kmalloc(sizeof(dis_record), GFP_ATOMIC)) == NULL) {
#ifdef DEBUG2
		printk("DIS_HISTORY: Can't allocate new entry\n");
#endif
		return NULL;
	}

	tmp_record = dis_history;

	while ((tmp_record != NULL) && (tmp_record->ip < ip)) {
		prev_record = tmp_record;
		tmp_record = tmp_record->next;
	}

	if (tmp_record && (tmp_record->ip == ip)) {
#ifdef DEBUG2
		printk("DIS_RECORD: Creating a duplicate record\n");
#endif
		//the record is existing,update it
		dis_histoty_write_lock(); 

		tmp_record->dis = dis;

		dis_histoty_write_unlock();
		
		kfree(new_record);
		return NULL;
	}

			
	dis_histoty_write_lock(); 

	new_record->ip = ip;
	new_record->dis = dis;

	new_record->next = NULL;
	
//insert at beginning
	if (prev_record == NULL) {
		new_record->next = dis_history;
		dis_history = new_record;
	} else {//among or at the end
		new_record->next = prev_record->next;
		prev_record->next = new_record;
	}
	
	dis_histoty_write_unlock();

	
	return new_record;
}

//update dis
void update_dis_record(u_int32_t ip,u_int32_t dis){
	

	dis_record *update_record = find_dis_record(ip);
//the record is not existing,insert it
	if(update_record==NULL){
		
#ifdef DEBUG2
		printk("DIS_HISTORY: No this record,insert a new one\n");
#endif
		update_record = insert_dis_record(ip,dis);
		if(update_record==NULL){
			printk("DIS_HISTORY: Insert new record fail\n");
		}

		return ;
	}
	
#ifdef WWTAO_DEBUG_HISTORYNEIDIS
char src_ip[20];
char re_ip[20];
strcpy(src_ip,inet_ntoa(ip));
strcpy(re_ip,inet_ntoa(update_record->ip));
printk("iAODV in RS: change history dis,IP: param:%s,%d	record:%s,%d\n",src_ip,dis,re_ip,update_record->dis);
#endif


	dis_histoty_write_lock();
	update_record->dis = dis;
	dis_histoty_write_unlock();
	return ;
}

dis_record *find_dis_record(u_int32_t target_ip){

	dis_record *tmp_record;

	dis_histoty_read_lock(); 

	tmp_record = dis_history;

	while ((tmp_record != NULL) && (tmp_record->ip <= target_ip)) {
		if (tmp_record->ip == target_ip) {

			dis_histoty_read_unlock();
			return tmp_record;
		}
		tmp_record = tmp_record->next;
	}

	dis_histoty_read_unlock();

	return NULL;
}


int del_dis_record(u_int32_t ip){

	dis_record *tmp_record;
	dis_record *prev_record = NULL;

	dis_histoty_write_lock(); 

	tmp_record = dis_history;

	while (tmp_record != NULL) {

		if (tmp_record->ip == ip) {

			if (prev_record != NULL) {
				prev_record->next = tmp_record->next;
			} else

			{
				dis_history = tmp_record->next;
			}

			dis_histoty_write_unlock();


			kfree(tmp_record);
			
			return 0;

		}

		prev_record = tmp_record;
		tmp_record = tmp_record->next;
	}


	dis_histoty_write_unlock();

	return -ENODATA;
}

void cleanup_dis_history(void){

	del_dis_history();
	dis_history = NULL;
}



int read_dis_history_proc(char *buffer, char **buffer_location, off_t offset,
		int buffer_length, int *eof, void *data) {

	char tmp_buffer[200];
	dis_record *tmp_record;
	
	int len;
	

	dis_histoty_read_lock();

	tmp_record = dis_history;

	sprintf(
			buffer,
			"\nMeet Node Distance History\n------------------------------------------------------------------------------------------------\n");
	sprintf(tmp_buffer,"       IP       | DIS 	|\n");

	strcat(buffer, tmp_buffer);
	sprintf(tmp_buffer,
			"------------------------------------------------------------------------------------------------\n");
	strcat(buffer, tmp_buffer);

	while (tmp_record!=NULL) {

		sprintf(tmp_buffer, "   %-16s", inet_ntoa(tmp_record->ip));
		strcat(buffer, tmp_buffer);

		sprintf(tmp_buffer, "   %-16d\n", tmp_record->dis);
		strcat(buffer, tmp_buffer);

		tmp_record = tmp_record->next;

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

	dis_histoty_read_unlock();

	return len;
}

#endif

