#!/bin/bash

echo -e "Generating configuration for Open\033[1;35mM\033[1;33mC\033[1;32mT \033[0m OWI ..."

echo "Processing autoreconf ...."
autoreconf -f -i -s
aclocal

if [ $? -ne 0 ] 
then
    echo "Somthing went wrong while aclocal runs ..."
    exit
fi

automake --foreign --include-deps
if [ $? -ne 0 ] 
then
    echo "Somthing went wrong while automake runs ..."
    exit
fi

echo "Done."
echo "Now you can run './configure --help' if you don't know thats options are to set!"
echo
