#!/bin/sh
APPCONFIG="/home/grui/2017newwork/dtn2_20170223/ion/cmd.json"
SOURCE_ADDR="dtn://192.168.11.122.gao.com"
DESTINATION_ADDR="dtn://192.168.11.122.gao.com"
AREA="121412"

#CMD_PORT="1"

#Validating CMD
jq -c '.' ${APPCONFIG} 

if [ $? -ne 0 ]; then
echo "CMD Error!"
exit 1
fi

#res=`jq -c '.' ${APPCONFIG}`
#echo ${res}

#Sending CMD to ION daemon of this node
#bpsource ${THIS_ADDR}.${CMD_PORT} `jq -c '.' ${APPCONFIG}`
echo "/home/grui/2017newwork/dtn2_20170223/DTN/DTN2/apps/dtnsend/dtnsend -s ${SOURCE_ADDR} -d ${DESTINATION_ADDR} -t m -p `jq -c '.' ${APPCONFIG}` -g ${AREA}"
/home/grui/2017newwork/dtn2_20170223/DTN/DTN2/apps/dtnsend/dtnsend -s ${SOURCE_ADDR} -d ${DESTINATION_ADDR} -t m -p `jq -c '.' ${APPCONFIG}` -g ${AREA}
