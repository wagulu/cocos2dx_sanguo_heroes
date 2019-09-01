#!/bin/bash
#遇到任何错误则终止脚本
set -e
set -o pipefail

pathIn=proto
pathOut=pb

fileList=`ls ${pathIn}`

allFiles=""
echo "-----------------------------"
for oneFile in $fileList
do
	# echo "--> ${pathIn}/${oneFile}"
	echo "--> ${oneFile}"
	oneFileName=${oneFile%.*}

	# echo "   /--- ${oneFileName}"
	# echo "protoc --proto_path=./${pathIn} --descriptor_set_out=./${pathOut}/${oneFileName}.pb ./proto/${oneFile}"
	# protoc --proto_path=./${pathIn} --descriptor_set_out=./${pathOut}/${oneFileName}.pb ./proto/${oneFile}

	# echo "--> ${allFiles}"
	allFiles=${allFiles}" "${pathIn}/${oneFile}
done
echo "== protoc --proto_path=./${pathIn} --descriptor_set_out=./pb/all.pb${allFiles}"
protoc --proto_path=./${pathIn} --descriptor_set_out=./pb/all.pb${allFiles} 
echo "    ---- Done ----\n"
