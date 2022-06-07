#!/bin/bash

s=0
t=`expr $1**$2`
t=$[ t * 3 ]
s=$[ s + t ]
t=$[ $1 ** 2 ]
t=$[ t * 4 ]
s=$[ s + t ]
t=`expr $2*5`
s=$[ s + t ]
s=$[ s + 6 ]
echo $s
