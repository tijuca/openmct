#!/bin/bash

package="OpenMCT Images"
#autoscan
#autoheader
#autoconf
echo "Generating configuration for $package ..."
autoreconf -i -f -s
#automake
echo "Done."
echo "Now you can run './configure --help' if you don't know thats options are to set!"
echo