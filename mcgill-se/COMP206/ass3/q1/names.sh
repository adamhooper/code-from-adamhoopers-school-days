#!/bin/sh

# The assignment asks us to use "set"...
set `users`

rm names.txt

for name in $*; do
	./insert-name $name
done
