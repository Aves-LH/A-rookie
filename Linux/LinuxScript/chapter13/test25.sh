#!/bin/bash

IFS=:

for folder in $PATH
do
	echo "$folder:"
	for file in $floder/*
	do
		if [ -x $file ]
		then
			echo "    $file"
		fi
	done
done
