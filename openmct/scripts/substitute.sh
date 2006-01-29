#!/bin/sh
sed -e $1 < $2 > $3
exit $?
