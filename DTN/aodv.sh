#!/bin/bash
usage(){
	echo "usage: `basename $0` ip-addrs"
}
IP=$1
if [ $# -ne 1 ]; then
        usage
        exit 1
fi


#fbaodv config
MESH_DEV="mesh_dev=adhoc0"

AODV_NET="network_ip=$IP/255.255.255.0"
GATEWAY="aodv_gateway=0"
#METRIC="routing_metric=ETT"
METRIC="routing_metric=HOPS"
RATE="nominal_rate=60"
COM_RADIUS="com_radius=300"
echo "1" > /proc/sys/net/ipv4/ip_forward
echo "Running AODV-MCC"
rmmod fbaodv_ubuntu
#echo "clean log!"
#echo > /system/log

insmod ./fbaodv_ubuntu.ko $MESH_DEV $AODV_NET $METRIC $RATE $GATEWAY $COM_RADIUS aodv_blacklist="$aodvblacklist" dtn_blacklist="$dtnblacklist"







