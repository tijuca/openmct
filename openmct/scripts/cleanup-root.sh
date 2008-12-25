#!/bin/sh

echo $1
DIR=$1
if [ "$1" != "" ]; then
	rm -rf $1/home
	rm -rf $1/mnt
	rm -rf $1/opt
	rm -rf $1/root
	cd $1 && ln -s /usr/root root 
	cd $1 && ln -s /usr/home home
	cd $1 && ln -s /tmp/media media
	rm -rf $1/usr
	rm -rf $1/var
	rm -rf $1/etc
#	rm $1/lib/libstdc++.so*
	mkdir $1/usr
	mkdir $1/var
fi
