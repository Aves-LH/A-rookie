#!/bin/bash

#using pattern matching

if [[ $USER == a* ]]
then
	echo "Hello $USER"
else
	echo "Sorry, I don't know you"
fi

