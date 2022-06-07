#!/bin/bash

#testing if a file is empty

file_name=$HOME/sentinel

if [ -f $file_name ]
then
	if [ -s $file_name ]
	then
		echo "The $file_name file exists and has data in it."
		echo "will not remove the file"
	else
		echo "The $file_name file exists , but it is empty"
		echo "Deleting empty file..."
		rm $file_name
	fi
else
	echo "The $file_name file doesn't exist"
fi

