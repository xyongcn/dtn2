/***************************************************************************
aodv_config.h  -  description
-------------------
begin                : FRI NOV 14 2014
copyright            : (C) 2014 by Cai Bingying
email                :
 ***************************************************************************/

#ifndef AODV_CONFIG_H
#define AODV_CONFIG_H

#include <linux/netdevice.h> 
#include "aodv.h"

int get_node_type(void);
bool is_local_ip(u_int32_t ip);
int route_neigh(u_int32_t src_ip, u_int32_t dst_ip, unsigned char tos);

#endif
