#!/bin/bash

#Look before you leap

jump_directory=/home/aves

if [ -d $jump_directory ]
then
	echo "The $jump_directory directory exists"
	cd $jump_directory
	ls
else	
	echo "The $jump_directory directory exists"
fi
