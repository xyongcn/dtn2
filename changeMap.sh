#!/bin/bash

this_dir=`pwd`

if [ $# -eq 0 ];then
	echo "1.test2-1  ./changeMap.sh 2 1"
	echo "2.real-test_12Area   ./changeMap.sh 12"
	echo "3.real-test_32-1Area   ./changeMap.sh 32 1"
	echo "3.real-test_32-2Area   ./changeMap.sh 32 2"

elif [ $# -eq 1 ];then
	if [ $1 = "12" ];then
		echo "delete ./RealSimulator/track.txt"
		rm -rf ./RealSimulator/track.txt
		echo "delete ./MapInterface/sumo.db"
		rm -rf ./MapInterface/sumo.db
		echo "copy track.txt from ./dtn-test/real-test_12Area/track.txt"
		cp ./dtn-test/real-test_12Area/track.txt ./RealSimulator/track.txt
		echo "copy sumo.db from ./dtn-test/real-test_12Area/sumo.db"
		cp ./dtn-test/real-test_12Area/sumo.db ./MapInterface/sumo.db
		echo "clear dtn2 log"
		./dtn-control.sh -clearData
		echo "changing node track is 1"
		./dtn-control.sh -modifyNodeTrack 1
	fi
elif [ $# -eq 2 ];then
	if [ $1 = "2" ];then
		if [ $2 = "1" ];then
			echo "delete ./RealSimulator/track.txt"
			rm -rf ./RealSimulator/track.txt
			echo "delete ./MapInterface/sumo.db"
			rm -rf ./MapInterface/sumo.db
			echo "copy track.txt from ./dtn-test/dtn-test2/track2-1.txt"
			cp ./dtn-test/dtn-test2/track2-1.txt ./RealSimulator/track.txt
			echo "copy sumo.db from ./dtn-test/dtn-test2/sumo.db"
			cp ./dtn-test/dtn-test2/sumo.db ./MapInterface/sumo.db
			echo "clear dtn2 log"
			./dtn-control.sh -clearData
			nodename=`./dtn-control.sh -dtnNodeName`
			if [ $nodename = "dtn://192.168.11.122.gao.com" ];then
				echo "changing node track is 2"
				./dtn-control.sh -modifyNodeTrack 2
				echo "copy dtn2 logDocuments2"
				rm -rf ./DTN/DTN2/logDocuments/*
				cp -r ./dtn-test/dtn-test2/dtnlog2-1/logDocuments2/* ./DTN/DTN2/logDocuments/
			elif [ $nodename = "dtn://192.168.11.124.gao.com" ];then
				echo "changing node track is 1"
				./dtn-control.sh -modifyNodeTrack 1
				echo "copy dtn2 logDocuments1"
				rm -rf ./DTN/DTN2/logDocuments/*
				cp -r ./dtn-test/dtn-test2/dtnlog2-1/logDocuments1/* ./DTN/DTN2/logDocuments/
			elif [ $nodename = "dtn://192.168.11.125.gao.com" ];then
				echo "changing node track is 3"
				./dtn-control.sh -modifyNodeTrack 3
				echo "copy dtn2 logDocuments3"
				rm -rf ./DTN/DTN2/logDocuments/*
				cp -r ./dtn-test/dtn-test2/dtnlog2-1/logDocuments3/* ./DTN/DTN2/logDocuments/
			fi
			echo "changing communication distance is 500"
			./dtn-control.sh -modifyCommDis 500
			
		fi
	elif [ $1 = "32" ];then
		if [ $2 = "1" ];then
			echo "delete ./RealSimulator/track.txt"
			rm -rf ./RealSimulator/track.txt
			echo "delete ./MapInterface/sumo.db"
			rm -rf ./MapInterface/sumo.db
			echo "copy track.txt from ./dtn-test/real-test_32-1Area/track.txt"
			cp ./dtn-test/real-test_32-1Area/track.txt ./RealSimulator/track.txt
			echo "copy sumo.db from ./dtn-test/real-test_32-1Area/sumo.db"
			cp ./dtn-test/real-test_32-1Area/sumo.db ./MapInterface/sumo.db
			echo "clear dtn2 log"
			./dtn-control.sh -clearData
			echo "changing node track is 1"
			./dtn-control.sh -modifyNodeTrack 1
		elif [ $2 = "2" ];then
			echo "delete ./RealSimulator/track.txt"
			rm -rf ./RealSimulator/track.txt
			echo "delete ./MapInterface/sumo.db"
			rm -rf ./MapInterface/sumo.db
			echo "copy track.txt from ./dtn-test/real-test_32-2Area/track.txt"
			cp ./dtn-test/real-test_32-2Area/track.txt ./RealSimulator/track.txt
			echo "copy sumo.db from ./dtn-test/real-test_32-2Area/sumo.db"
			cp ./dtn-test/real-test_32-2Area/sumo.db ./MapInterface/sumo.db
			echo "clear dtn2 log"
			./dtn-control.sh -clearData
			nodename=`./dtn-control.sh -dtnNodeName`
			if [ $nodename = "dtn://192.168.11.122.gao.com" ];then
				echo "changing node track is 2"
				./dtn-control.sh -modifyNodeTrack 2
				echo "copy dtn2 logDocuments2"
				rm -rf ./DTN/DTN2/logDocuments/*
				cp -r ./dtn-test/real-test_32-2Area/dtnlog/logDocuments2/* ./DTN/DTN2/logDocuments/
			elif [ $nodename = "dtn://192.168.11.124.gao.com" ];then
				echo "changing node track is 1"
				./dtn-control.sh -modifyNodeTrack 1
				echo "copy dtn2 logDocuments1"
				rm -rf ./DTN/DTN2/logDocuments/*
				cp -r ./dtn-test/real-test_32-2Area/dtnlog/logDocuments1/* ./DTN/DTN2/logDocuments/
			elif [ $nodename = "dtn://192.168.11.125.gao.com" ];then
				echo "changing node track is 3"
				./dtn-control.sh -modifyNodeTrack 3
				echo "copy dtn2 logDocuments3"
				rm -rf ./DTN/DTN2/logDocuments/*
				cp -r ./dtn-test/real-test_32-2Area/dtnlog/logDocuments3/* ./DTN/DTN2/logDocuments/
			fi
			echo "changing communication distance is 1000"
			./dtn-control.sh -modifyCommDis 1000
			
		fi
	fi

fi			
