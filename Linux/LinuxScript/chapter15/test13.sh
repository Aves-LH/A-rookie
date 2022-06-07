#!/bin/bash

#using an alternative file desciptor

exec 3>test13out

echo "This should display on the monitor"
echo "and this shoud be stored in thr file" >&3
echo "Then this should be back on the monitor"
