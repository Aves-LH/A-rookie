#!/bin/bash

#check if either a directory or file exists

item_name=$HOME

echo
echo "The item being checked: $item_name"
echo

if [ -e $item_name ]
then
	echo "The item, $item_name, does exist"
	echo "But is it a file?"
	if [ -f $item_name ]
	then
		echo "Yes, it is a file"
	elif [ -d $item_name ]
	then
		echo "It is a directory"
	else
		echo "It isn't a file. It isn't also a directory."
	fi
else
	echo "The item, $item_name, doesn't exist"
	echo "Nothing to update"
fi
