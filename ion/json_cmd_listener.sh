#!/bin/bash
THIS_ADDR="dtn://192.168.11.122.gao.com"

dir=`pwd`
num=1
fileflag="bundle payload is file@#@&%~#%"
bundlelist=`ls /var/dtn/bundleDeliveried`
#echo "bundlelist: $bundlelist"

while [ 1 ]
do
	newfile=`ls -t /var/dtn/bundleDeliveried | head -5`
	newlist=""
	for str in $newfile
	do 
		newlist="$str $newlist"
	done
	
	for str in $newlist
	do
		
		if [[ $bundlelist =~ $str ]];then
			continue
		else
			fileroute="/var/dtn/bundleDeliveried/"
			fileroute="$fileroute$str"
			bundlelist="$bundlelist $str"
			cmd=`cat $fileroute`
			#neighbour bundle			
			if [[ "$cmd" == "22 serialization"* ]]; then
				continue
			#jpg file
			elif [[ "$cmd" == $fileflag* ]]; then
				#recover file
				sed -i "s/$fileflag//g" $fileroute 
				echo "receive a file $str"
				cp $fileroute /home/grui/2017newwork/dtn2_20170223/ion/testfile$num
				num=`expr $num + 1`
			#json flie
			else
				tmp=`echo ${cmd} | jq -c '.retrieval'`
				if [ "$tmp" != "null" ]; then
					##retrieval
					#If this node is the cmd sender, then 
					#	send this cmd to the destination
					#else if this node is the cmd executor, then
					#	excute the cmd and send replying
					echo ${cmd} | jq '.'
					SADDR=`echo ${cmd} | jq -c '.retrieval.saddr'`
					SADDR=`echo ${SADDR} | sed 's/\"//g'`
		
					DADDR=`echo ${cmd} | jq -c '.retrieval.daddr'`
					DADDR=`echo ${DADDR} | sed 's/\"//g'`
		
					REQUEST_FILE=`echo ${cmd} | jq -c '.retrieval.file'`
					REQUEST_FILE=`echo ${REQUEST_FILE} | sed 's/\"//g'`
		
					DAREA=`echo ${cmd} | jq -c '.retrieval.darea'`
					DAREA=`echo ${DAREA} | sed 's/\"//g'`

					SAREA=`echo ${cmd} | jq -c '.retrieval.sarea'`
					SAREA=`echo ${SAREA} | sed 's/\"//g'`

					if [ "$SADDR" = "$THIS_ADDR" ]; then
						echo "dtnsend -s ${THIS_ADDR} -d ${DADDR} -t m -p ${cmd} -g ${DAREA}"
						/home/grui/2017newwork/dtn2_20170223/DTN/DTN2/apps/dtnsend/dtnsend -s ${THIS_ADDR} -d ${DADDR} -t m -p ${cmd} -g ${DAREA}


					elif [ "$DADDR" = "$THIS_ADDR" ]; then
						#Checking the requested file
						FILE_VALID=1
						if [ -f $REQUEST_FILE ]; then
							echo "dtnsend -s ${THIS_ADDR} -d ${SADDR} -t f -p ${REQUEST_FILE} -g ${SAREA}"
							#handle REQUEST_FILE
							filecontent=`cat $REQUEST_FILE`
							filecontent="$fileflag$filecontent"
							echo "$filecontent" > $REQUEST_FILE
							/home/grui/2017newwork/dtn2_20170223/DTN/DTN2/apps/dtnsend/dtnsend -s ${THIS_ADDR} -d ${SADDR} -t f -p ${REQUEST_FILE} -g ${SAREA}
							FILE_VALID=1
							#recover REQUEST_FILE
							sed -i "s/$fileflag//g" $REQUEST_FILE
						else
							echo "File not exist!"
							FILE_VALID=-1
						fi
						#Sending replying
						echo "Replying!"
						echo "dtnsend -s ${THIS_ADDR} -d ${SADDR} -t m -p {\"replying\":{\"retrieval\":\"${FILE_VALID}\"\,\"port\":\"${RECVPORT}\"}} -g ${SAREA}"	
						/home/grui/2017newwork/dtn2_20170223/DTN/DTN2/apps/dtnsend/dtnsend -s ${THIS_ADDR} -d ${SADDR} -t m -p {\"replying\":{\"retrieval\":\"${FILE_VALID}\"\,\"port\":\"${RECVPORT}\"}} -g ${SAREA}
					fi
				fi


				##CMD--Replying
				tmp=`echo ${cmd} | jq -c '.replying'`
				if [ "$tmp" != "null" ]; then
					echo ${cmd} | jq '.'
					RECVPORT=`echo ${cmd} | jq -c '.replying.port'`
					RECVPORT=`echo ${RECVPORT} | sed 's/\"//g'`
					# bprecvfile ${THIS_ADDR}.${RECVPORT} 1
		
				fi
			fi
			 
		fi
	done
done
