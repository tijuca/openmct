#!/bin/sh
for link in $(cat $3); do
	ln -s --force $2 $1$link
done
