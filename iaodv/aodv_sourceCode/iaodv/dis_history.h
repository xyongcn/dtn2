/***************************************************************************
 dis_history.h -  description
 -------------------
 begin                : Sat NOV 14
 copyright            : (C) 2015 by Cai Bingying
 ***************************************************************************/
#include "aodv.h"


bool is_dis_history_empty(void);
void init_dis_history(void);
dis_record* insert_dis_record(u_int32_t ip,u_int32_t dis);
void update_dis_record(u_int32_t ip,u_int32_t dis);
dis_record *find_dis_record(u_int32_t target_ip);
int del_dis_record(u_int32_t ip);
void cleanup_dis_history(void);
void del_dis_history(void);
int read_dis_history_proc(char *buffer, char **buffer_location, off_t offset,
		int buffer_length, int *eof, void *data);


