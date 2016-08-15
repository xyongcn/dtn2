#!/bin/bash

push_run_intel_aodv(){
	echo "this is intel devices: $1"
}

push_run_panda_aodv(){
	echo "this is pandaboard devices: $1"
}

intel_devices=""
panda_devices=""
alldevices=""
#network num
network_num="192.168.5."
this_dir=`pwd`

funForDevice(){
#	echo "devices: $1 , substring: ${1:0:6}"
	if [ ${1:0:6} = "Medfie" ];then
		#intelDevice $1
		intel_devices="$intel_devices $1"
		alldevices="$alldevices $1"
	else
		#pandaDevice $1
		panda_devices="$panda_devices $1"
		alldevices="$alldevices $1"
	fi
} 

help(){
	echo "this bash's help:"
	echo "with no argument :"
	echo "./detach-device.sh : push aodv.ko,insmod aodv.ko,install app"
	echo "with 1 argument :"
	echo "./detach-device.sh -aodv-[aodv dir] : push aodv dir's aodv.ko,inmod aodv.ko"
	echo "./detach-device.sh -startApp : set the time with the track.txt,and start app"
	echo "./detach-device.sh -resetTime : reset the time with the track.txt's started time"
	echo "./detach-device.sh -clearAodv : clear the sh file and the aodv.ko file in the device"
	echo "./detach-device.sh -clearAPP : uninstall the installed app in the device"
	echo "./detach-device.sh -dmesgWatch : open another terminal to watch device's dmesg"
	echo "./detach-device.sh -reboot : reboot the all the devices"
	echo "./detach-device.sh -renameDevices : rename all the devices with -ip"
	echo "./detach-device.sh -getGeoDataLog : get the geoDtn data from devices"
	echo "./detach-device.sh -logcat : logcat the dtn-bytewalla log" 
	echo "./detach-device.sh -endExperiment : force-stop the dtn-bytewalla"
	echo "./detach-device.sh -startDtnExperiment : start the dtn experiment"
	echo "./detach-device.sh -prepareDtnExperiment : prepare for the dtn experiment,transmit the files"
	echo "with 2 arguments :"
	echo "./detach-device.sh -reinstallApp [app name] : reinstall the app with the app name"
	echo "./detach-device.sh -startApp [app name] : start the app with the app name"
	echo "./detach-device.sh -clearData [dataname:(geoDTN)] : excute dataname's sh to clear data"

}

#get deivces's name
devices_string=`sudo adb devices`
isDevices="0"
for str in $devices_string
do
	if [ $isDevices = "1" ];then
		isDevices="0"
		funForDevice $str
		continue
	fi

	if [ $str = "attached" ];then 
		isDevices="1";
	fi

	if [ $str = "device" ];then
		isDevices="1";
	fi

done

#no argument
if [ $# -eq 0 ];then

	#push and insmod aodv
	filename="simulator-aodv"
	#intel aodv
	cd ./$filename/intel-aodv
	for str in $intel_devices
	do
		echo ""
		echo "./$filename/intel-aodv/intel-aodv.sh $str"
		echo ""
		#./intel-aodv.sh $str $network_num$num
		./intel-aodv.sh $str
	done
	#pandaboard aodv
	cd $this_dir

	cd ./$filename/panda-aodv
	for str in $panda_devices
	do
		echo ""
		echo "./$filename/panda-aodv/panda-aodv.sh $str"
		echo ""
		./panda-aodv.sh $str
	done
	cd $this_dir

	#install simulator app
	cd ./install-realSImulation-App
	for str in $alldevices
	do
		echo ""
		./install-app.sh $str
	done
	cd $this_dir

elif [ $# -eq 1 ];then
	if [ $1 = "--help" ];then
		help
	elif [ $1 = "-help" ];then	
		help
	elif [ $1 = "-startApp"	];then
		#set time
		cd ./setDeviceTime
		./setTime.sh $alldevices
		cd $this_dir

		#start app
		cd ./install-realSImulation-App
		for str in $alldevices
		do
			./start-app.sh $str
		done

		cd $this_dir

	elif [ $1 = "-resetTime" ];then
		#set time
		cd ./setDeviceTime
		./setTime.sh $alldevices
		cd $this_dir

	elif [ $1 = "-clearAodv" ];then
		#rm the fbaodv.ko and the .sh
		for str in $alldevices
		do
			adb -s $str shell mount -o remount rw /system
			echo "clear device[$str]'s fbaodv.ko"
			adb -s $str shell rm /system/fbaodv_panda.ko
			adb -s $str shell rm /system/fbaodv_intel.ko
			echo "clear device[$str]'s adhoc-config.sh"
			adb -s $str shell rm /system/adhoc-config-intel.sh
			adb -s $str shell rm /system/adhoc-config-panda.sh
		done
	elif [ $1 = "-clearApp" ];then
		#uninstall the simulator app
		for str in $alldevices
		do
			echo "uninstall device[$str]'s the simulator app"
			runApp=`adb -s $str shell ps|grep "com.realsimulator.Main"`
			runApp=${runApp:0:3}
			if [ $runApp ];then
				adb -s $str shell am force-stop com.realsimulator.Main
			fi
			adb -s $str uninstall com.realsimulator.Main
		done
	elif [ $1 = "-dmesgWatch" ];then
		for str in $alldevices
		do
			echo "open another terminal to watch device[$str]"
			gnome-terminal -x bash -c "./device-dmesg-watch/watch-level4-log.sh $str"
		done
	elif [ $1 = "-reboot" ];then
		for str in $alldevices
		do
			echo "reboot device[$str]"
			adb -s $str shell reboot
		done
	elif [ $1 = "-renameDevices" ];then
		num=11
	
		#rename intel device
		for str in $intel_devices
		do
			rename="Medfie-$network_num$num"
			echo "rename device[$str] to device[$rename]"
			adb -s $str shell "echo $rename > /sys/class/android_usb/android0/iSerial"
			num=`expr $num + 1`
		done
		#rename pandaboard device
		num=`expr $num + 10`

		for str in $panda_devices
		do
			rename="Panda-$network_num$num"
			echo "rename device[$str] to device[$rename]"
			adb -s $str shell "echo $rename > /sys/class/android_usb/android0/iSerial"
			num=`expr $num + 1`
		done

		sudo adb kill-server
		sudo adb devices	
	elif [ $1 = "-getGeoDataLog" ];then
		cd ./geoDtnExperiment
			./getGeoData.sh $alldevices
		cd $this_dir

	elif [ $1 = "-logcat" ];then
		for str in $alldevices
		do
			gnome-terminal -x bash -c "./geoDtnExperiment/logcatSh/logcat.sh $str"
		done

	elif [ $1 = "-prepareDtnExperiment" ];then
		cd ./geoDtnExperiment/dtnExperimentFile/
			./prepareExperiment.sh $alldevices
		cd $this_dir

	elif [ $1 = "-startDtnExperiment" ];then
		cd ./geoDtnExperiment/
			./startExperiment.sh $alldevices
		cd $this_dir

	elif [ $1 = "-startVDtnExperiment" ];then
		cd ./geoDtnExperiment/
			./startVExperiment.sh $alldevices
		cd $this_dir

	elif [ $1 = "-endExperiment" ];then
		cd ./geoDtnExperiment/
			./endExperiment.sh $alldevices
		cd $this_dir

	#添加其他单个参数的地方	

	else
		#has -arugment
		argument=${1:0:5}
		if [ $argument = "-aodv" ];then
			filename=${1:6}
			if [ ! -d "./$filename" ];then
				echo "dir ./$filename don't exist"
				exit
			fi

			#push and insmod aodv
	
			#intel aodv
			cd ./$filename/intel-aodv
			for str in $intel_devices
			do
				echo ""
				echo "./$filename/intel-aodv/intel-aodv.sh $str"
				echo ""
				#./intel-aodv.sh $str $network_num$num
				./intel-aodv.sh $str
			done
			#pandaboard aodv
			cd $this_dir

			cd ./$filename/panda-aodv
			for str in $panda_devices
			do
				echo ""
				echo "./$filename/panda-aodv/panda-aodv.sh $str"
				echo ""
				./panda-aodv.sh $str
			done
			cd $this_dir

			#install simulator app
			cd ./install-realSImulation-App
			for str in $alldevices
			do
				echo ""
				./install-app.sh $str
			done
			cd $this_dir
		else
			echo "wrong arguments"
			help
		fi
	fi
elif [ $# -eq 2 ];then
	if [ $1 = "-reinstallApp" ];then
		cd ./App/
		for str in $alldevices
		do
			echo "./reinstall-app.sh $str $2"
			./reinstall-app.sh $str $2
		done
		cd $this_dir
	elif [ $1 = "-startApp" ];then
		cd ./App/
		for str in $alldevices
		do
			echo "./start-app.sh $str $2"
			./start-app.sh $str $2
		done
		cd $this_dir

	elif [ $1 = "-clearData" ];then
		cd ./clearData
		if [ $2 = "geoDTN" ];then
			./clearGeoDtnData.sh $alldevices
		else
			echo "./detach-device.sh -clearData $2 does't exist, please check the help"
		fi
		cd $this_dir

	#添加其他2个参数的地方

	else
		echo "error : wrong arguments"	
		#help
	fi
else

	help
fi

