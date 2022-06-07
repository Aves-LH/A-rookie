#!/bin/bash

#Removing a set trao

trap "echo 'Sorry... Ctrl-C is trapped'" SIGINT

count=1

while [ $count -le 5 ]
do
	echo "Loop #$count"
	sleep 1
	count=$[ $count + 1 ]
done

trap -- SIGINT

echo "I just remove the trap"

count=1

while [ $count -le 5 ]
do
	echo "Loop #$count"
	sleep 1
	count=$[ $count + 1 ]
done

trap - SIGINT

count=1

while [ $count -le 5 ]
do
	echo "Loop #$count"
	sleep 1
	count=$[ $count + 1 ]
done

echo "This is the end of script"
