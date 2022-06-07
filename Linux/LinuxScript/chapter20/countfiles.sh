#!/bin/bash

mypath=$(echo $PATH | sed 's/:/ /g')
count=1

for directory in $mypath
do
	file=$(ls $directory)
	for item in $file
	do
			count=$[ $count +  1 ]
	done
	echo "$directory - $count"
	count=0
done

