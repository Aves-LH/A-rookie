#!/bin/bash

#Shell wrapper for sed editor script
#to reverse text file lines.

sed -n '{1!G ; h ; $p }' $1
