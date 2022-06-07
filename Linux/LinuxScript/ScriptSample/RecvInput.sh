#!/bin/bash

if read -t 5 -p "Input Name: " name
then
	echo "your name is $name"
else

	read -p "Input Password: " -s passwd
	echo "Password is $passwd"
fi
