#!/bin/sh

OLDPWD=`pwd`

cd `dirname $0`/../..

# Job 1
mkdir testbed
cp data/members.txt testbed
sort testbed/members.txt > testbed/sorted.txt
rm testbed/members.txt

# Job 2
echo PLEASE ENTER INVALID USERS > testbed/invalid.txt
vi testbed/invalid.txt # (user interaction goes here)

# Job 3
for name in `cat testbed/invalid.txt`; do
    grep $name testbed/sorted.txt >> testbed/found.txt
done
echo --- USER LIST ---
more testbed/sorted.txt # (user interaction goes here)
echo --- UNAUTHORIZED USER LIST ---
more testbed/found.txt # (user interaction goes here)
cp testbed/found.txt testbed/sorted.txt data
rm -rf testbed

cd $OLDPWD
