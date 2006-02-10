#
# OpenMCT Firmware Builder
#

all:	do-it-all

include $(OPENMCT_DEVEL_PREFIX)Rules

ifeq ($(CONFIG),$(wildcard $(CONFIG)))
include $(CONFIG)
do-it-all:      $(IMAGEFILENAME)
else
do-it-all:      show
endif

config:
	@if [ ! -f $(CONFIG) ] ; then \
		if [ -f $(CONFIGDIR)/$(MCT) ]; then \
			$(ECHO) -e "\033[1;33mCONFIGURE \033[1;32mOpenMCT\033[0m" ; \
			$(CP) $(CONFIGDIR)/$(MCT) $(CONFIG) ; \
			$(MKDIR) $(OUTPUTDIR) ; \
		else \
			$(ECHO) -e "\033[1;33mCONFIGURE FAILED\033[0m" ; \
			$(ECHO) "$(CONFIGDIR)/$(MCT) does not exist" ; \
		fi \
	else \
		$(ECHO) -e "\033[1;33mALREADY CONFIGURED\033[0m" ; \
	fi

show:
	@$(ECHO) -e "\n\033[1;33mOpenMCT Firmware Builder\033[0m" ; \
	$(ECHO) -e "\033[1;32m************************\033[0m" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32mmake config MCT=[target]\033[0m" ; \
	$(ECHO) -e "\033[1;32mmake\033[0m" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "choose one of the following targets: " ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32msa\033[0m\tClaxan SA 110 / Claxan SA 113 / Intellinet SOHO SERVER /" ; \
	$(ECHO) -e "\tTritton TRI-ASA1120/200 / Tritton TRI-ASA2120/200 " ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32mnas\033[0m\tClaxan NAS 110 / Intellinet SOHO NAS /" ; \
	$(ECHO) -e "\tTritton TRI-NAS080/120/200 " ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32mcustom\033[0m\tIOGEAR BOSS " ; \
	$(ECHO) -e "" ; \

clean:	pkgclean
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-clean

distclean:	pkgdistclean
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-distclean
	@$(MAKE) -C packages/build/buildroot OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-distclean
	@$(shell if [ -d $(OUTPUTDIR) ]; then rm -rf $(OUTPUTDIR); fi)
	@$(shell if [ -f $(CONFIG) ]; then rm -rf $(CONFIG); fi)

pkg:
	@$(foreach package,$(IMAGEPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-all ; )

pkgclean:
	@$(foreach package,$(IMAGEPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-clean ; )

pkgdistclean:
	@$(foreach package,$(IMAGEPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-distclean ; )

packages/system/linux/$(SRCDIR)/$(CONFIG):
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ $(SRCDIR)
	@$(CP) packages/system/linux/configs/$(IMAGEKERNELCONFIG) packages/system/linux/configs/config
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ $(SRCDIR)/$(CONFIG)
	@$(ECHO) -e "\033[1;33mPATCH \033[1;32m$(KERNEL)\033[0m : FLASH KERNEL SIZE"
	@$(PERL) -pi -e 's/1088/$(IMAGEFLASHKERNELSIZE)/g' packages/system/linux/src/drivers/mtd/maps/mct.c
	@$(ECHO) -e "\033[1;33mPATCH \033[1;32m$(KERNEL)\033[0m : FLASH ROOT SIZE"
	@$(PERL) -pi -e 's/2112/$(IMAGEFLASHROOTSIZE)/g' packages/system/linux/src/drivers/mtd/maps/mct.c
	@$(ECHO) -e "\033[1;33mPATCH \033[1;32m$(KERNEL)\033[0m : FLASH ETC SIZE"
	@$(PERL) -pi -e 's/704/$(IMAGEFLASHETCSIZE)/g' packages/system/linux/src/drivers/mtd/maps/mct.c
	@if [ ! -s $(OUTPUTDIR)/linux ]; then \
		$(LN) -s $(ACTDIR)/packages/system/linux/$(SRCDIR) $(OUTPUTDIR)/linux ; \
	fi
	@if [ ! -f packages/system/linux/src/$(DEPEND) ]; then \
		$(MAKE) -C packages/system/linux/src dep ; \
	fi

$(OUTPUTDIR):
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(OUTPUTDIR)\033[0m"
	@$(MKDIR) $(OUTPUTDIR)

$(OUTPUTDIR)/$(KERNEL):	packages/system/linux/$(SRCDIR)/$(CONFIG) $(NEWROOTDIR)
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-build
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-install

$(OUTPUTDIR)/$(MCT_MODIFY): packages/build/mct_modify/src/mct_modify.c
	@$(MAKE) -C packages/build/mct_modify OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-clean
	@$(MAKE) -C packages/build/mct_modify OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-all

$(NEWROOTDIR):
	$(MAKE) -C packages/build/buildroot OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-all

$(OUTPUTDIR)/$(ROOTSQUASHFS):
	@$(ECHO) -e "\033[1;33mREMOVE .svn FROM $(NEWROOTDIR)" 
	@$(REMOVESVN) $(NEWROOTDIR)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(ROOTSQUASHFS)\033[0m"
	@$(MKSQUASHFS) $(NEWROOTDIR)/* $(OUTPUTDIR)/$(ROOTSQUASHFS) $(SQUASHFSOPTS)

$(OUTPUTDIR)/$(ETCJFFS2):
	@$(ECHO) -e "\033[1;33mCOPY base \033[1;32m/etc\033[0m" 
	@$(CP) -a $(ETCDIR) $(NEWETCDIR)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/$(IMAGERELEASEFILENAME)\033[0m"
	@$(ECHO) "OpenMCT $(IMAGEVERSION) $(IMAGESUBVERSION) $(IMAGEPRODCODE)" > $(NEWETCDIR)/$(IMAGERELEASEFILENAME)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/hostname\033[0m"
	@$(ECHO) $(IMAGEHOSTNAME) > $(NEWETCDIR)/hostname
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/issue\033[0m"
	@$(ECHO) "Welcome on OpenMCT $(IMAGEVERSION) - Kernel \r (\t)" > $(NEWETCDIR)/issue
	@$(ECHO) -e "\033[1;33mMODIFY \033[1;32m/etc/httpd.conf\033[0m (mode 640)"
	@$(CHMOD) 640 $(NEWETCDIR)/httpd.conf 
	@$(ECHO) -e "\033[1;33mMODIFY \033[1;32m/etc/init.d/*\033[0m   (mode 755)" 
	@$(CHMOD) 755 $(NEWETCDIR)/init.d/* 
	@$(CP) $(ETCDIR)/network/interfaces $(NEWETCDIR)/network/interfaces
ifdef ETH0IPADDR 
	@$(ECHO) -e "\033[1;33mCONFIGURE \033[1;32meth0\033[0m"
	@$(ECHO) "auto eth0" >> $(NEWETCDIR)/network/interfaces
	@$(ECHO) "iface eth0 inet static" >> $(NEWETCDIR)/network/interfaces
	@$(ECHO) -e "\taddress $(ETH0IPADDR)" >> $(NEWETCDIR)/network/interfaces
ifdef ETH0NETMASK
	@$(ECHO) -e "\tnetmask $(ETH0NETMASK)" >> $(NEWETCDIR)/network/interfaces
else
	@$(ECHO) -e "\tnetmask 255.255.255.0" >> $(NEWETCDIR)/network/interfaces
endif
	@$(ECHO) "" >> $(NEWETCDIR)/network/interfaces 
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/hosts\033[0m"
	@$(ECHO) $(ETH0IPADDR) $(IMAGEHOSTNAME) > $(NEWETCDIR)/hosts
endif
ifdef ETH1IPADDR 
	@$(ECHO) -e "\033[1;33mCONFIGURE \033[1;32meth1\033[0m"
	@$(ECHO) "auto eth1" >> $(NEWETCDIR)/network/interfaces
	@$(ECHO) "iface eth1 inet static" >> $(NEWETCDIR)/network/interfaces
	@$(ECHO) -e "\taddress $(ETH1IPADDR)" >> $(NEWETCDIR)/network/interfaces
ifdef ETH1NETMASK
	@$(ECHO) -e "\tnetmask $(ETH1NETMASK)" >> $(NEWETCDIR)/network/interfaces
else
	@$(ECHO) -e "\tnetmask 255.255.255.0" >> $(NEWETCDIR)/network/interfaces
endif
	@$(ECHO) "" >> $(NEWETCDIR)/network/interfaces 
endif
	@$(ECHO) -e "\033[1;33mREMOVE .svn FROM $(NEWETCDIR)"
	@$(REMOVESVN) $(NEWETCDIR)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(ETCJFFS2)\033[0m"
	@$(MKFSJFFS2) -l -q -d $(NEWETCDIR)/ -o $(OUTPUTDIR)/$(ETCJFFS2)

$(IMAGEFILENAME):	$(OUTPUTDIR)/$(MCT_MODIFY) $(OUTPUTDIR)/$(KERNEL) pkg $(OUTPUTDIR)/$(ROOTSQUASHFS) $(OUTPUTDIR)/$(ETCJFFS2)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(IMAGEFILENAME)\033[0m"
	@$(OUTPUTDIR)/$(MCT_MODIFY) -w -i $(IMAGEFILENAME) -m $(IMAGEMODE) -p $(IMAGEPRODCODE) \
		-v $(IMAGEVERSION) -n $(IMAGESUBVERSION) -k $(OUTPUTDIR)/$(KERNEL) \
                -l $(OUTPUTDIR)/$(ROOTSQUASHFS),$(OUTPUTDIR)/$(ETCJFFS2) \
                -a $(IMAGEFLASHKERNELSIZE),$(shell expr $(IMAGEFLASHKERNELSIZE) + $(IMAGEFLASHROOTSIZE)) \
		-z $(shell expr $(IMAGEFLASHKERNELSIZE) + $(IMAGEFLASHROOTSIZE))
