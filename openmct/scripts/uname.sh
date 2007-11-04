#!/bin/sh
grep "^#define UTS_RELEASE \"" $1/include/linux/version.h | awk -F\" '{ print $2; }'
