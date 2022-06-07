#!/bin/bash

#simple script menu

function menu
{
	clear
	echo -e "\t\tSys Admin Menu\n"
	echo -e "\t1. Display dosk space"
	echo -e "\t2. Display logged on users"
	echo -e "\t3. Display memory usage"
	echo -e "\t0. Exit program\n"
	echo -en "\t\tEnter option: "
	read -n 1 option
	echo
}

function diskspace
{
	clear df 
}

function whoseon
{
	clear
	who
}

function menusage
{
	clear 
	cat /proc/meminfo
}

while [ 1 ]
do
	menu
	case $option in
		0)
			break;;
		1)
			diskspace;;
		2)
			whoseon;;
		3)
			menusage;;
		*)
			echo
			echo "Input Error!"
	esac
	echo -en "\n\n\t\tPress any key to continue"
	read -n 1 line
done
clear

