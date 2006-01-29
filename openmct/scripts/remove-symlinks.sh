#!/bin/sh
for link in $(cat $3); do
	rm $1$link
done
