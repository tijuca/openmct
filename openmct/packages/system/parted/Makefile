# parted

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://ftp.gnu.org/gnu/parted/parted-1.8.8.tar.bz2

DEPENDSON=	packages/lib/e2fsprogs

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) --without-readline

include		$(OPENMCT_DEVEL_PREFIX)Rules
