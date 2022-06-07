#!/bin/bash

#testing string equality

testuser=aves

if [ $USER = $testuser ]
then
	echo "Welcome $testuser"
fi

