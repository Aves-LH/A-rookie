#!/bin/bash

#check if a file is writable

item_name=$HOME/sentinel

echo 
echo "The item being checked : $item_name"
echo

if [ -e $item_name ]
then
	echo "The file $item_name exist"
	if [ -f $item_name ]
	then
		echo "Yes, $item_name is a file"
		echo "But is it writable?"
		echo 
		if [ -w $item_name ]
		then
			echo "Yes, it is writable"
			echo "Writing current time to $item_name"
			date +%H%M >> $item_name
		else
			echo "No, it isn't writable"
		fi
	else
		echo "It isn't a file"
	fi
else
	echo "The file $item_name doesn't exist"
fi
