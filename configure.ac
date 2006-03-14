#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(claxan, 0.0.1, doc@hangar18.org)

# may we must know later the build_cpu, build_vendor and build_os
AC_CANONICAL_BUILD
# defined the host_cpu, host_vendor and host_os
#AC_CANONICAL_HOST
# so we use different target_cpu we define even target_cpu, target_vendor and target_os
AC_CANONICAL_TARGET
target="mipsel-linux"
AC_SUBST(target)


AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([Makefile.in])
AC_CONFIG_HEADER([config.h])



# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_LN_S
AC_PROG_MAKE_SET


# Checks for libraries.

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_TIME

# Checks for library functions.
#AC_FUNC_SELECT_ARGTYPES
#AC_CHECK_FUNCS([select strerror strncasecmp])

AC_PREFIX_DEFAULT(mct-build)
if test "$prefix" = "NONE"; then
        prefix=$HOME/openmct-build
fi
AC_SUBST(prefix)

# Checks for user options.

AC_MSG_CHECKING([for prefix directory])
if test -d "$prefix" ; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_WARN([must create prefix directory!])
	prefix_create="yes"
	mkdir $prefix
fi
	

# from another file, will be removed next time
#def_timeout=-1
#AC_ARG_ENABLE([timeout],
#  AC_HELP_STRING([--enable-timeout=SEC],
#    [timeout in SEC seconds (default is 5)]),
#  [def_timeout=$enableval])
#if test "$def_timeout" = yes; then
#  def_timeout=5
#fi
#if test "$def_timeout" = no; then
#  def_timeout=-1
#fi
#AC_DEFINE_UNQUOTED(DEFAULT_TIMEOUT, $def_timeout,
#  [Default timeout for waiting on an event.])

# we have different MCT Devices
AC_ARG_WITH(device,
    [  --with-device=NAME        the name of MCT Device you want to build ],
	[device="$with_device"],[device="NONE"])
AC_SUBST(device)

dnl We have to check if the given device is in the list of possibile devices!

if test "$device" = "nas110" || \
   test "$device" = "sa110"  || \
   test "$device" = "sa113"  || \
   test "$device" = "nas200" || \
   test "$device" = "sa200" ; then 
   devicetest="ok"
else
    devicetest="fail"
fi


# test if the User have give us a Device for that the package should build
if test "$device" = "NONE" || test "$devicetest" = "fail" ; then
AC_MSG_ERROR([
*************************************************
*                  WARNING!                     *
* You did not specify a right MCT Device!       *
* Please rerun ./configure with the             *
* wanted Device                                 *
*                                               *
* for example for an Claxan NAS110:             *
* ./configure --with-device=nas110 ....         *
*                                               *
* ./configure --help will show all possible     *
*                    parameters                 *
*************************************************
])
fi

AC_MSG_RESULT([
configure results:
----------------------------------------
Your specified device:           $device
])


AC_CONFIG_FILES([
Makefile
progs/Makefile
tools/Makefile
])

AC_OUTPUT
