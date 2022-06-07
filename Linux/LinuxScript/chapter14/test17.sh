#!/bin/bash

#extracting command line options and values

echo

while [ -n "$1" ]
do
	case "$1" in
		-a) param="$2"
			echo "Found the -a option, with parameter value $param" 
			shift ;;
		-b) param="$2"
			echo "Found the -b option , with parameter value $param"
			shift ;;
		-c) param="$2"
			echo "Found the -c option, with parameter value $param"
			shift ;;
		--) shift
			break ;;
		*) echo "$1 isn't an option" ;;
	esac
	shift
done

count=1

for param in "$@"
do
	echo "Parameter #$count: $param"
	count=$[ $count + 1 ]
done
