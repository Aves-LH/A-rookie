#!/bin/bash

if [ -w $1 ]
then
	echo "writeable"
else
	echo "unwriteable"
fi

if [ -x $1 ]
then
	echo "executeable"
else
	echo "unexecuteable"
fi
