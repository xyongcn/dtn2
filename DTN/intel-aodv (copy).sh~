#/bin/bash
if [ $# -eq 1 ];then

	echo "insmod aodv in $@"

	#echo "change devices name"
	#adb -s $1 shell "echo Medfie-$2 > /sys/class/android_usb/android0/iSerial"

	#get device ip
	ip=${1##*-}

	temp=${1%-*}
	if [ $ip = $temp ];then
		echo "device[$1]'s name don't has -ip"
		exit
	fi

	if [ ! ip ];then
		echo "error : the device[$1]'s name do not has -ip"		
		exit
	fi
	
	echo "mount /system"
	adb -s $1 shell mount -o remount rw /system

	temp=`adb -s $1 shell ls /system/fbaodv_intel.ko`
	#echo "check if fbaodv_intel.ko exists"
	temp=${temp##*: }
	temp=${temp%%' '*}
	echo "temp=$temp"
	if [ $temp = 'No' ];then
	
		echo "push fbaodv_intel.ko to $1"
		adb -s $1 push ./fbaodv_intel.ko /system/fbaodv_intel.ko
	fi

	temp=`adb -s $1 shell ls /system/adhoc-config-intel.sh`
	temp=${temp##*: }
	temp=${temp%%' '*}
	echo "temp=$temp"
	if [ $temp = 'No' ];then
		echo "push adhoc-config-intel.sh to $1"
		adb -s $1 push ./adhoc-config-intel3.sh /system/adhoc-config-intel.sh
	fi

	echo "adb -s $1 shell rm /sdcard/dtn/storage/*"
	adb -s $1 shell rm /sdcard/dtn/storage/*
	adb -s $1 shell rm /sdcard/dtn_test_data/*

	echo "adb -s $1 shell sh /system/adhoc-config-intel.sh $ip"
	adb -s $1 shell sh /system/adhoc-config-intel.sh $ip
	#./intel-blacklist-aodv.sh $1 $ip
else
	echo "wrong arguments,usage : ./intel-aodv.sh [device name]"
fi
