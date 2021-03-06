#!/bin/bash
all_parameter=$*
#all_parameter=${all_parameter#* }
this_dir=`pwd`
ip=""
parameter=""

for str in $all_parameter
do
	if [ $str = "-configEnvironment" ];then
		continue
	fi
	parameter="$parameter $str"
	
done
echo "$parameter"
cd ./ModifyIaodv.shMesh_dev
./ModifyIaodv.shMesh_dev $parameter
cd $this_dir


for str2 in $all_parameter
do
	if [ $str2 = "-configEnvironment" ];then
		continue
	fi
	ip_part=`ifconfig $str2`
	ip_part=${ip_part#*"inet addr:"}
	ip_part=${ip_part%"Bcast"*}
	ip="$ip$ip_part"
	
done
echo "$ip"
cd ./ModifyDTN2_DA
./ModifyDTN2_DA $ip
cd $this_dir
