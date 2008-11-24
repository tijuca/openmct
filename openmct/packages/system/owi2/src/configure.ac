#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(owi2.c)

PACKAGE=owi
OWI_MAJOR_VERSION=2
OWI_MINOR_VERSION=0
OWI_MICRO_VERSION=0
OWI_VERSION_PREFIX="-alpha"
OWI_VERSION=$OWI_MAJOR_VERSION.$OWI_MINOR_VERSION.$OWI_MICRO_VERSION$OWI_VERSION_PREFIX

AC_SUBST(OWI_MAJOR_VERSION)
AC_SUBST(OWI_MINOR_VERSION)
AC_SUBST(OWI_MICRO_VERSION)
AC_SUBST(OWI_VERSION_PREFIX)
AC_SUBST(OWI_VERSION)

AC_DEFINE_UNQUOTED(OWI_VERSION,"$OWI_VERSION",[The OpenMCT WebInterface version])
AC_DEFINE_UNQUOTED(PACKAGE_NAME, "$PACKAGE", [The package name])

AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wno-portability]$PACKAGE, $OWI_VERSION, devel@openmct.org)
AM_MAINTAINER_MODE

AC_CONFIG_SRCDIR([src/owi2.c])
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

# directorys
OWI_HTMLDIR=$prefix/newroot/var/www
OWI_CSSDIR=$prefix/newroot/var/www/css
OWI_IMAGEDIR=$prefix/newroot/var/www/img
NFS_HTMLDIR=$prefix/nfs/var/www
NFS_CSSDIR=$prefix/nfs/var/www/css
NFS_IMAGEDIR=$prefix/nfs/var/www/img

AC_SUBST(OWI_CSSDIR)
AC_SUBST(OWI_IMAGEDIR)
AC_SUBST(OWI_HTMLDIR)
AC_SUBST(NFS_CSSDIR)
AC_SUBST(NFS_IMAGEDIR)
AC_SUBST(NFS_HTMLDIR)

AC_CONFIG_FILES([Makefile
		 img/Makefile
		 css/Makefile
		 html/Makefile
		 src/Makefile])

AC_OUTPUT
