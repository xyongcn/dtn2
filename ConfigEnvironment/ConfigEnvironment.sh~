#!/bin/bash
all_parameter=$*
#all_parameter=${all_parameter#* }

ip=""

for str in $all_parameter
do
	if [ $str = "-configEnvironment" ];then
		continue
	fi
	ip_part=`ifconfig $str`
	ip_part=${ip_part#*"inet addr:"}
	ip_part=${ip_part%"Bcast"*}
	ip="$ip$ip_part"
	
done
echo "$ip"
./ConfigEnvironment $ip
