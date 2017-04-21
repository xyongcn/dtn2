#!/bin/bash

this_dir=`pwd`

help(){
	echo "this bash's help:"
	echo "with no argument :"
	echo "./dtn-control.sh : help" 

	echo "with 1 argument :"
	echo "./dtn-control.sh -help: help"
	echo "./dtn-control.sh -iaodv: inmod iaodv.ko"
	echo "./dtn-control.sh -dtnNodeName: look for dtn node's name"
	echo "./dtn-control.sh -realSimulator: open realSimulator"
	echo "./dtn-control.sh -mapInterface: open mapInterface"
	echo "./dtn-control.sh -osm2Sqlite: open osm2Sqlite and produce sumo.db"
	echo "./dtn-control.sh -dtn2: open dtn2 in the background,log appears in ./DTN/DTN2/dtn2.log"
	echo "./dtn-control.sh -dtn: open dtn2"
	echo "./dtn-control.sh -killdtn2: kill dtn2"
	echo "./dtn-control.sh -killrealSimulator: kill realSimulator"
	echo "./dtn-control.sh -killmapInterface: kill mapInterface"
	echo "./dtn-control.sh -watch: detection information of distance between nodes and dtn2"
	#echo "./dtn-control.sh -closeWatch: close the information"
	echo "./dtn-control.sh -clearData: clear log and historyarea law and neighbour historyarea law,if you want to change 			frequency vector's type,you must carry out this first"
	echo "./dtn-control.sh -clearLog: delete dtn2.log,MapInLog.txt.RealLog.txt"
	echo "./dtn-control.sh -sendBundle : Then,knock command at the terminal,for example: ./dtn-control.sh -sendBundle \"./dtnsend -s dtn://192.168.5.1.gao.com 			-d dtn://192.168.5.2.gao.com -t m -p hello -g 121412\""
 	echo "./dtn-control.sh -configEnvironment: configure the environment,using all network cards to send and receive data"

	echo "with 2 or more arguments :"
	echo "./dtn-control.sh -renameDevices : rename the dtn node's name"
	echo "./dtn-control.sh -modifyNodeTrack : modify node's track by modifying realsimulator's nodeInfo.txt"
	echo "./dtn-control.sh -modifyCommDis : modify communication distance between nodes"
	echo "./dtn-control.sh -modifyVector : modify frequency vector's type,options [minute | hour | monafteve | week | month]
		for example: ./dtn-control.sh -modifyVector minute hour week"
	echo "./dtn-control.sh -configEnvironment: configure the environment,using specified network cards to send and receive data"

	}
#find node's name from /etc/dtn.conf
cd ./NodeName/FindDtnNodeName
node_name=`./FindDtnNodeName`

temp=${node_name##*//}
node_ip=${temp%.gao*}

cd $this_dir
#no argument
if [ $# -eq 0 ];then
	help

elif [ $# -eq 1 ];then
	if [ $1 = "-help" ];then
		help
	elif [ $1 = "-iaodv"	];then
		#inmod iaodv.ko
		cd ./iaodv
		
		numIao=`lsmod | grep "fbaodv_ubuntu"`
		if [ "$numIao"x != ""x ];then
			echo "rmmod fbaodv_ubuntu"
			rmmod fbaodv_ubuntu
		fi

		./aodv.sh $node_ip
		cd $this_dir

	elif [ $1 = "-dtnNodeName" ];then
		#look for dtn node's name
		echo $node_name

	elif [ $1 = "-realSimulator" ];then
		#open osm2Sqlite and produce sumo.db
		cd ./RealSimulator
		#gnome-terminal -x bash -c "./RealSimulator"
		echo "open RealSimulator"
		./RealSimulator > RealLog.txt &
		cd $this_dir

	elif [ $1 = "-mapInterface" ];then
		#open mapInterface
		cd ./MapInterface
		#gnome-terminal -x bash -c "./MapInterface"
		echo "open MapInterface"
		./MapInterface > MapInLog.txt &
		cd $this_dir

	elif [ $1 = "-osm2Sqlite" ];then
		#open osm2Sqlite and produce sumo.db
		cd ./Osm2Sqlite
		./Osm2Sqlite
		cd $this_dir

	elif [ $1 = "-dtn2" ];then
		#open dtn2 in the background
		cd ./DTN/DTN2/
		#gnome-terminal -x bash -c "./dtnd"
		./daemon/dtnd -d -o ./dtn2.log
		cd $this_dir

	elif [ $1 = "-dtn" ];then
		#open dtn2
		cd ./DTN/DTN2/
		#gnome-terminal -x bash -c "./dtnd"
		./daemon/dtnd
		cd $this_dir

	elif [ $1 = "-clearLog" ];then
		#clear Log
		cd ./DTN/DTN2/
		rm -rf dtn2.log
		echo "delete dtn2.log"
		cd $this_dir

		cd ./MapInterface
		rm -rf MapInLog.txt
		echo "delete MapInLog.txt"
		cd $this_dir

		cd ./RealSimulator
		rm -rf RealLog.txt
		echo "delete RealLog.txt"
		cd $this_dir
	
	elif [ $1 = "-clearData" ];then
		#clear Data
		cd ./DTN/DTN2/logDocuments
		rm -rf areamoving.txt
		echo "delete areamoving.txt"
		rm -rf bundleLog.txt
		echo "delete bundleLog.txt"
		rm -rf dataBundleLog.txt
		echo "delete dataBundleLog.txt"
		rm -rf historyarea.txt
		echo "delete historyarea.txt"
		rm -rf GeohistoryLog.txt
		echo "delete GeohistoryLog.txt"
		rm -rf geoHistory_dtn
		echo "delete historyNeighbour.txt,neighbour.txt and all of neighbour's historyarea"
		cd $this_dir

	elif [ $1 = "-killrealSimulator" ];then
		PROCESS=`ps -ef|grep "RealSimulator"|grep -v grep|grep -v PPID|awk '{ print $2}'`
		for i in $PROCESS
		do
 			echo "Kill the RealSimulator process [ $i ]"
 		 	kill -9 $i
		done

	elif [ $1 = "-killmapInterface" ];then
		PROCESS=`ps -ef|grep "MapInterface"|grep -v grep|grep -v PPID|awk '{ print $2}'`
		for i in $PROCESS
		do
 			echo "Kill the MapInterface process [ $i ]"
 		 	kill -9 $i
		done

	elif [ $1 = "-killdtn2" ];then
		PROCESS=`ps -ef|grep "dtnd"|grep -v grep|grep -v PPID|awk '{ print $2}'`
		for i in $PROCESS
		do
 			echo "Kill the dtn2 process [ $i ]"
 		 	kill -9 $i
		done

	elif [ $1 = "-watch" ];then
	#	tail -f ./DTN/DTN2/dtn2.log &
 		while true
        	do
 			clear
 			echo "************************************"
 			cat /proc/fbaodv/dis_history
 			sleep 3
 		done

	#elif [ $1 = "-closewatch" ];then
	#	PROCESS=`ps -ef|grep "tail -f ./DTN/DTN2/"|grep -v grep|grep -v PPID|awk '{ print $2}'`
	#	for i in $PROCESS
	#	do
 	#		echo "Kill the tarl -f ./DTN/DTN2/dtn2.log process [ $i ]"
 	#	 	kill -9 $i
	#	done

	elif [ $1 = "-configEnvironment" ];then
		cd ./ConfigEnvironment
		./ConfigEnvironment.sh $*
		cd $this_dir

	else
	     echo "error : wrong arguments"
	     help
	fi

elif [ $# -eq 2 ];then
	if [ $1 = "-renameDevices" ];then
		cd ./NodeName/ModifyDtnNodeName
		./ModifyDtnNodeName $2	
		cd $this_dir

	elif [ $1 = "-modifyNodeTrack" ];then
		cd ./NodeName/ModifyNodeTrack
		./ModifyNodeTrack $2	
		cd $this_dir

	elif [ $1 = "-modifyCommDis" ];then
		cd ./ConfigEnvironment/ModifyIaodv.shCom_radius
		./ModifyIaodv.shCom_radius $2	
		cd $this_dir

	elif [ $1 = "-sendBundle" ];then
		#send bundle
		cd ./DTN/DTN2/apps/dtnsend/
		temp=""
		temp=${2#* }
		echo $temp
		./dtnsend $temp
		cd $this_dir

	elif [ $1 = "-configEnvironment" ];then
		cd ./ConfigEnvironment
		./ConfigEnvironment.sh $*
		cd $this_dir

	else
		echo "error : wrong arguments"	
		help
	fi

elif [ $# -ge 2 ];then

	if [ $1 = "-modifyVector" ];then
		#modify frequency vector's type
		cd ./ModifyVector
		./ModifyVector $*
		cd $this_dir

	elif [ $1 = "-configEnvironment" ];then
		cd ./ConfigEnvironment
		./ConfigEnvironment.sh $*
		cd $this_dir

	else
		echo "error : wrong arguments"	
		help
	fi
else

	help
fi
