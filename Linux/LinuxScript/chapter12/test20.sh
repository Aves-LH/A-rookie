#!/bin/bash

#testing file dates

if [ test19.sh -nt test18.sh ]
then
	echo "The test19 file is newer than test18.sh"
else
	echo "The test18 file is newer than test19.sh"
fi

