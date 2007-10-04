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
			if [ ! -d $(OUTPUTDIR) ]; then \
			   $(MKDIR) $(OUTPUTDIR) ; \
			fi ; \
			$(ECHO) -e "\033[1;33mBASE PACKAGES:\033[0m" ; \
			for package in $(BASEPACKAGES); do \
			   $(ECHO) -e "\t"$$package ; \
			done ; \
			$(ECHO) -e "\033[1;33mADDITIONAL PACKAGES:\033[0m" ; \
			for package in $(ADDITIONALPACKAGES); do \
			   $(ECHO) -e "\t"$$package ; \
			done ; \
			$(ECHO) -e "\033[1;33mDON'T FORGET TO ADD ADDITIONAL PACKAGES IN $(CONFIG)\033[0m" ; \
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
	$(ECHO) -e "\033[1;32msa200\033[0m\tClaxan SA 200" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32msa113\033[0m\tClaxan SA 113" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32msa110\033[0m\tClaxan SA 110" ; \
	$(ECHO) -e "\tIntellinet SOHO SERVER : Server Appliance SA-130/110" ; \
	$(ECHO) -e "\tTritton Server Appliance : TRI-ASA1120/200 / TRI-ASA2120/200" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32mnas200\033[0m\tClaxan NAS 200" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32mnas110\033[0m\tClaxan NAS 110" ; \
	$(ECHO) -e "\tIntellinet SOHO NAS : Network Attached Storage NAS-130/110" ; \
	$(ECHO) -e "\tTritton Wired SOHO NAS : TRI-NAS080/120/200" ; \
	$(ECHO) -e "" ; \
	$(ECHO) -e "\033[1;32mboss\033[0m\tIOGEAR BOSS " ; \
	$(ECHO) -e "" ; \

clean:	pkgclean
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-clean

distclean:	pkgdistclean
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-distclean
	@$(RM) packages/system/linux/configs/config
ifdef OPENMCT_CROSS_BUILD
	@$(MAKE) -C packages/build/buildroot OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-distclean
endif	
	@$(shell if [ -d $(OUTPUTDIR) ]; then rm -rf $(OUTPUTDIR); fi)
	@$(shell if [ -f $(CONFIG) ]; then rm -rf $(CONFIG); fi)

pkg:
	@$(foreach package,$(BASEPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-all ; )
	@$(foreach package,$(ADDITIONALPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-all ; )

pkgclean:
	@$(foreach package,$(BASEPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-clean ; )
	@$(foreach package,$(ADDITIONALPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-clean ; )

pkgdistclean:
	@$(foreach package,$(BASEPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-distclean ; )
	@$(foreach package,$(ADDITIONALPACKAGES),$(MAKE) -C $(package) OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ do-distclean ; )

packages/system/linux/$(SRCDIR)/$(CONFIG):
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ $(SRCDIR)
	@$(CP) packages/system/linux/configs/$(IMAGETARGET) packages/system/linux/configs/config
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(OPENMCT_DEVEL_PREFIX)/ $(SRCDIR)/$(CONFIG)
	@if [ ! -s $(OUTPUTDIR)/linux ]; then \
		$(LN) -s $(ACTDIR)/packages/system/linux/$(SRCDIR) $(OUTPUTDIR)/linux ; \
	fi
	@if [ ! -f packages/system/linux/src/$(DEPEND) ]; then \
		$(MAKE) -C packages/system/linux/src dep ; \
	fi


$(OUTPUTDIR):
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(OUTPUTDIR)\033[0m"
	@$(MKDIR) $(OUTPUTDIR)

$(OUTPUTDIR)/$(KERNEL):	packages/system/linux/$(SRCDIR)/$(CONFIG) $(NEWROOTDIR) $(NEWVARDIR)
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-build
	@$(MAKE) -C packages/system/linux OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-install

$(OUTPUTDIR)/$(MCT_MODIFY): packages/build/mct_modify/src/mct_modify.c
	@$(MAKE) -C packages/build/mct_modify OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-clean
	@$(MAKE) -C packages/build/mct_modify OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-all

$(OUTPUTDIR)/$(MKSQUASHFS):
	@$(MAKE) -C packages/build/squashfs OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-clean
	@$(MAKE) -C packages/build/squashfs OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-all

$(NEWROOTDIR):
ifdef OPENMCT_CROSS_BUILD
	@$(MAKE) -C packages/build/buildroot OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-all
else
	@if [ ! -d $(NEWROOTDIR) ]; then \
		$(CP) -a $(OPENMCT_CROSS_BASE)/root $(NEWROOTDIR) ; \
		$(CLEANUPROOT) $(NEWROOTDIR) ; \
	fi
endif

$(NEWVARDIR):
	@$(MAKE) -C packages/build/buildetc OPENMCT_DEVEL_PREFIX=$(ACTDIR)/ do-all
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/$(IMAGERELEASEFILENAME)\033[0m"
	@$(ECHO) "OpenMCT $(IMAGEVERSION) $(IMAGESUBVERSION) $(IMAGEPRODCODE)" > $(NEWROOTDIR)/etc/$(IMAGERELEASEFILENAME)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/hostname\033[0m"
	@$(ECHO) $(IMAGETARGET) > $(NEWVARDIR)/etc/hostname
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/issue\033[0m"
	@$(ECHO) "Welcome on OpenMCT $(IMAGEVERSION) - Kernel \r (\t)" > $(NEWVARDIR)/etc/issue
ifdef ETH0IPADDR 
	@$(ECHO) -e "\033[1;33mCONFIGURE \033[1;32meth0\033[0m"
	@$(ECHO) "auto eth0" >> $(NEWVARDIR)/etc/network/interfaces
	@$(ECHO) "iface eth0 inet static" >> $(NEWVARDIR)/etc/network/interfaces
	@$(ECHO) -e "\taddress $(ETH0IPADDR)" >> $(NEWVARDIR)/etc/network/interfaces
ifdef ETH0NETMASK
	@$(ECHO) -e "\tnetmask $(ETH0NETMASK)" >> $(NEWVARDIR)/etc/network/interfaces
else
	@$(ECHO) -e "\tnetmask 255.255.255.0" >> $(NEWVARDIR)/etc/network/interfaces
endif
ifdef ETH0DEFAULTGW
	@$(ECHO) -e "\tgateway $(ETH0DEFAULTGW)" >> $(NEWVARDIR)/etc/network/interfaces
endif
	@$(ECHO) "" >> $(NEWVARDIR)/etc/network/interfaces 
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/hosts\033[0m"
	@$(ECHO) $(ETH0IPADDR) $(IMAGETARGET) > $(NEWVARDIR)/etc/hosts
endif
ifdef ETH1IPADDR 
	@$(ECHO) -e "\033[1;33mCONFIGURE \033[1;32meth1\033[0m"
	@$(ECHO) "auto eth1" >> $(NEWVARDIR)/etc/network/interfaces
	@$(ECHO) "iface eth1 inet static" >> $(NEWVARDIR)/etc/network/interfaces
	@$(ECHO) -e "\taddress $(ETH1IPADDR)" >> $(NEWVARDIR)/etc/network/interfaces
ifdef ETH1NETMASK
	@$(ECHO) -e "\tnetmask $(ETH1NETMASK)" >> $(NEWVARDIR)/etc/network/interfaces
else
	@$(ECHO) -e "\tnetmask 255.255.255.0" >> $(NEWVARDIR)/etc/network/interfaces
endif
ifdef ETH1DEFAULTGW
	@$(ECHO) -e "\tgateway $(ETH1DEFAULTGW)" >> $(NEWVARDIR)/etc/network/interfaces
endif
	@$(ECHO) "" >> $(NEWVARDIR)/etc/network/interfaces 
endif
	@$(CP) $(NEWVARDIR)/etc/network/interfaces $(NEWROOTDIR)/etc/default/interfaces

$(OUTPUTDIR)/$(ROOT):
	@$(ECHO) -e "\033[1;33mREMOVE .svn FROM $(NEWROOTDIR)" 
	@$(REMOVESVN) $(NEWROOTDIR)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(ROOT)\033[0m"
	$(OUTPUTDIR)/$(MKSQUASHFS) $(NEWROOTDIR) $(OUTPUTDIR)/$(ROOT) $(ROOTOPTS)

$(OUTPUTDIR)/$(VAR):	
	@$(ECHO) -e "\033[1;33mREMOVE .svn FROM $(NEWVARDIR)" 
	@$(REMOVESVN) $(NEWVARDIR)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(VAR)\033[0m"
	@$(MKFSJFFS2) -d $(NEWVARDIR) -o $(OUTPUTDIR)/$(VAR)

$(IMAGEFILENAME):	$(OUTPUTDIR)/$(MCT_MODIFY) $(OUTPUTDIR)/$(MKSQUASHFS) $(OUTPUTDIR)/$(KERNEL) pkg $(OUTPUTDIR)/$(ROOT) $(OUTPUTDIR)/$(VAR)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(IMAGEFILENAME)\033[0m"
	@$(OUTPUTDIR)/$(MCT_MODIFY) -w -i $(IMAGEFILENAME) -m $(IMAGEMODE) -p $(IMAGEPRODCODE) \
		-v $(IMAGEVERSION) -n $(IMAGESUBVERSION) -k $(OUTPUTDIR)/$(KERNEL) \
                -l $(OUTPUTDIR)/$(ROOT),$(OUTPUTDIR)/$(VAR) \
                -a $(IMAGEFLASHKERNELSIZE),$(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHKERNELSIZE)) \
		-z $(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHETCSIZE))
