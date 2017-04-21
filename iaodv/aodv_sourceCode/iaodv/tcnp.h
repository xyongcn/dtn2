/***************************************************************************
 Created by Cai Bingying- Version: Sun Dec 21 2014
 ***************************************************************************/
#ifndef TCNP_H
#define TCNP_H

#include "aodv.h"

//gen route rediscovery packet
int gen_tcnp(u_int32_t src_ip,u_int32_t dst_ip,u_int32_t last_hop,unsigned char tos);
int recv_tcnp(task *tmp_packet);

#endif
