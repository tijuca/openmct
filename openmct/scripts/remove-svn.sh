#!/bin/sh
if [ "$1" != "" ]; then
	(find $1 -name '.svn' -exec rm -rf {} \; 2>&1) > /dev/null
	(find $1 -name '.svnignore' -exec rm {} \; 2>&1) > /dev/null
fi
