include 	$(OPENMCT_DEVEL_PREFIX)Rules

all:
	test -d $(NEWVARDIR) || mkdir -p $(NEWVARDIR)
	svn export --force etc/ $(NEWROOTDIR)/etc
	svn export --force sbin $(NEWROOTDIR)/sbin
	svn export --force var  $(NEWVARDIR)/var
