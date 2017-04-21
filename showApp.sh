#!/bin/bash

this_dir=`pwd`

./dtn-control.sh -iaodv
./dtn-control.sh -killrealSimulator
./dtn-control.sh -realSimulator
./dtn-control.sh -killmapInterface
./dtn-control.sh -mapInterface
./dtn-control.sh -killdtn2
echo "open dtn2"
./dtn-control.sh -dtn2

cd ./ion
echo "sh json_cmd_listener.sh"
./json_cmd_listener.sh 


