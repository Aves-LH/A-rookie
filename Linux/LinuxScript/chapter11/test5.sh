#!/bin/bash

testing=$(date)

echo The time and date are : $testing

today=$(date +%y%m%d)
ls /usr/bin -al > log.$today

echo $BASH_SUBSHELL
