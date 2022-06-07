#!/bin/bash

#trying to access script parameters inside a function

function func7
{
	echo $[ $1 * $2 ]
}

if [ $# -eq 2 ]
then
	echo "The result is $(func7 $1 $2)"
else
	echo "Usage: ./test7.sh a b"
fi
