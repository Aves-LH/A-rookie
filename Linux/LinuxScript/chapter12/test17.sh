#!/bin/bash

#testing file execution

if [ -x test16.sh ]
then
	echo "you can run the script"
	./test16.sh
else
	echo "Sorry, you can't run the script"
fi

