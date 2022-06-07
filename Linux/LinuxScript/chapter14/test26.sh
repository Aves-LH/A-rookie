#!/bin/bash

#getting just one character of input

read -n1 -p "Do you want to continue (Y/N)? " answer

case $answer in
	Y | y) echo
		echo "Fine, continue on ..." ;;
	N | n) echo
		echo "OK, Goodbye!"
esac
echo "This is the end of script"
