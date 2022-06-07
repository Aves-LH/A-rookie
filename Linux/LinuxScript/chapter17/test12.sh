#!/bin/bash

#returning an array value

function arraydblr
{
	local oriArray
	local newArray
	local elements
	local i

	oriArray=($(echo "$@"))
	newArray=($(echo "$@"))
	elements=$[ $# - 1 ]

	for((i = 0; i <= $elements; i++))
	{
		newArray[$i]=$[ ${oriArray[$i]} * 2 ]
	}

	echo ${newArray[*]}
}

myArray=(1 2 3 4 5)
echo "The original array is : ${myArray[*]}"

arg1=$(echo ${myArray[*]})
result=($(arraydblr $arg1))

echo "The new array is : ${result[*]}"
