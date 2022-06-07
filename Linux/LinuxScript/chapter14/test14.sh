#!/bin/bash

#demonstrating a multi-position shift

echo
echo "The original parameter: $*"

shift 2
echo "Here're the new first parameter: $1"
echo

