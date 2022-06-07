#!/bin/bash

#testing string equality 

testuser=baduser

if [ $USER != $testuser ]
then
	echo "There is not $testuser"
else
	echo "Welcome $testuser"
fi

