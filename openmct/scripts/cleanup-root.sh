#!/bin/sh
if [ "$1" != "" ]; then
	rm -rf $1/home
	rm -rf $1/mnt
	rm -rf $1/opt
	rm -rf $1/root
	rm -rf $1/usr
	rm -rf $1/var
	rm -rf $1/etc
	rm $1/lib/libstdc++.so*
	mkdir $1/usr
	mkdir $1/var
	mkdir $1/jffs2
	mkdir $1/rw
	mkdir $1/harddisk
fi
