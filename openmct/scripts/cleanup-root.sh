#!/bin/sh
if [ "$1" != "" ]; then
	rm -rf $1/home
	rm -rf $1/mnt
	rm -rf $1/opt
	rm -rf $1/root
	rm -rf $1/usr
	rm -rf $1/var
	rm -rf $1/etc
	rm -f $1/lib/libstdc++*
	mkdir $1/usr
	mkdir $1/usr/home
	mkdir $1/usr/var
	mkdir $1/usr/vservers
	ln -s usr/home $1/home
	ln -s usr/var $1/var
	ln -s usr/vservers $1/vservers
fi
