#!/bin/bash

i=$[ $1 % 2 ]
if test $i -eq 0
then
	echo oushu
else
	echo jishu
fi

