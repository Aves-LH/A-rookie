#!/bin/bash

#iterate through all the files in a directory

for file in /home/aves/*
do
	if [ -f "$file" ] 
	then
		echo "$file is a file"
	elif [ -d "$file" ]
	then
		echo "$file is a directory"
	fi
done

