#!/bin/sh
# Yes/No prompt!
read -p "Are you sure? [y/n] " -n 1 -r
echo	# (optional) move to a new line
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
	exit 1
fi
