#!/bin/bash

count1=0
count2=0

cd $1
ls -l $1
for file in $(ls)
do
	if [ -f $file ]
	then
		count1=$[ $count1 + 1 ]
	elif [ -d $file ]
	then
		count2=$[ $count2 + 1 ]
	fi
done
echo "The number of file is $count1"
echo "The number of directory is $count2"
