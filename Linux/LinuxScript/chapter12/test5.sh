#!/bin/bash

#testing nested ifs

testuser=NoSuchUser

if grep $testuser /etc/passwd
then
	echo "The user $testuser exist on this system"
elif ls -d /home/$testuser
then
	echo "The user $testyser does't exist on this system"
	echo "However, the user $testuser has a directory"
else
	echo "The user $testuser doesn't exist on this system"
	echo "And, $testuser doesn't have a directory"
fi

