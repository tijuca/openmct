# TCP Wrapper

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://ftp.porcupine.org/pub/security/tcp_wrappers_7.6.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc REAL_DAEMON_DIR=/tmp linux

INSTALL=	true

include		$(OPENMCT_DEVEL_PREFIX)Rules
