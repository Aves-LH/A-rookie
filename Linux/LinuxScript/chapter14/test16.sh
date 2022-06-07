#!/bin/bash

#extracting options and parameters

echo 
while [ -n "$1" ]
do
	case "$1" in
		-a) echo "Found the first option";;
		-b) echo "Found the second option";;
		-c) echo "Found the third option";;
		--) shift
			break ;;
		*) echo "$1 isn't a parameter";;
	esac
	shift
done

count=1

for param in $@
do
	echo "Parameter #$count: $param"
	count=$[ $count + 1 ]
done

