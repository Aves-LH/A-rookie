#!/bin/bash

#using the case command

case $USER in
	aves | barbara)
		echo "Welcome, $USER"
		echo "Please enjoy your visit";;
	testing)
		echo "Special testing account";;
	jessica)
		echo "Don't forget to log off when you're done";;
	*)
		echo "Sorry, you are'not allowed here"
esac

