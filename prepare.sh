#!/bin/bash

echo -e "Generating configuration for Open\033[1;35mM\033[1;33mC\033[1;32mT \033[0m..."

autoreconf -f -i -s

echo "Done."
echo "Now you can run './configure --help' if you don't know thats options are to set!"
echo