#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(hotplug.c)

PACKAGE=omct-hotplug
HOTPLUG_MAJOR_VERSION=0
HOTPLUG_MINOR_VERSION=2
HOTPLUG_MICRO_VERSION=8
HOTPLUG_VERSION=$HOTPLUG_MAJOR_VERSION.$HOTPLUG_MINOR_VERSION.$HOTPLUG_MICRO_VERSION

AC_SUBST(HOTPLUG_MAJOR_VERSION)
AC_SUBST(HOTPLUG_MINOR_VERSION)
AC_SUBST(HOTPLUG_MICRO_VERSION)
AC_SUBST(HOTPLUG_VERSION)

AC_DEFINE_UNQUOTED(HOTPLUG_VERSION,"$HOTPLUG_VERSION",[The hotplughandler version])
AC_DEFINE_UNQUOTED(PACKAGE_NAME, "$PACKAGE", [The package name])

AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wno-portability]$PACKAGE, $HOTPLUG_VERSION, devel@openmct.org)
AM_MAINTAINER_MODE

AC_CONFIG_SRCDIR([hotplug.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([memset strdup])

AC_ARG_ENABLE([debug],
    AS_HELP_STRING([--enable-debug],[Enable debugging code (default=no)]),
		   [enable_debug=$enableval],
		   [enable_debug=no])
		    if test "x$enable_debug" = xyes; then
			AC_DEFINE(DEBUG, 1, [Enable debugging code])
		    fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
