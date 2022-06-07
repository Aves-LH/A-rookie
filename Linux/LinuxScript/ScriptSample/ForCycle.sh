#!/bin/bash

for i in "$*"
do
	echo "Parameter is : $i"
done

for j in "$@"
do
	echo "Paramter: $j"
done
