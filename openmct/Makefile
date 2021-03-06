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
			$(CP) $(CONFIGDIR)/$(MCT) $(CONFIG) && echo "BASECONFIG=$(MCT)" >> $(CONFIG); \
			if [ ! -d $(OUTPUTDIR) ]; then \
			   $(MKDIR) $(OUTPUTDIR) ; \
			fi ; \
			if [ "$(NET)" == "dhcp" ]; then \
				echo "NET=dhcp" >> $(CONFIG) ; \
			elif [ "$(NET)" != "dhcp" ] && [ "$(NET)" != "" ]; then \
				$(ECHO) -e "\033[1;33mCONFIGURE FAILED\033[0m" ; \
				$(ECHO) "Unknown option 'NET=$(NET)' !" ; \
				$(ECHO) "For DHCP use 'NET=dhcp' " ; \
				$(ECHO) -e "\033[1;33mRemoving $(CONFIG)\033[0m" && rm $(CONFIG) ; \
				exit; \
			fi; \
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
	$(ECHO) -e "\033[1;32m************************\033[0m\n" ; \
	$(ECHO) -e "First You have to configure the buildsystem like:" ; \
	$(ECHO) -e "\033[1;32mmake config MCT=[target]\033[0m\n" ; \
	$(ECHO) -e "or if want to use DHCP for the NIC(s):" ; \
	$(ECHO) -e "\033[1;32mmake config MCT=[target] NET=dhcp\033[0m" ; \
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
	@$(CP) packages/system/linux/configs/$(IMAGEKERNELCONFIG) packages/system/linux/configs/config
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

$(OUTPUTDIR)/$(KERNEL):	packages/system/linux/$(SRCDIR)/$(CONFIG) $(NEWROOTDIR)
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

$(OUTPUTDIR)/$(ROOT): etc depmod openmct-release interfaces exports hosts resolv.conf

etc:
#	@rm -rf $(NEWROOTDIR)/etc $(NEWROOTDIR)/var
	@svn export --force packages/build/buildetc/new $(NEWROOTDIR)

depmod:
# cleaning up .svn files
	@$(ECHO) -e "\033[1;33mREMOVE .svn FROM $(NEWROOTDIR)" 
	@$(REMOVESVN) $(NEWROOTDIR)
# depmod for the kernel
	@$(ECHO) -e "\033[1;33mCALCULATE DEPENDENCIES FOR KERNEL MODULES\033[0m"
	@$(DEPMOD) -b $(NEWROOTDIR)/lib/modules/`$(UNAME) $(PACKAGEDIR)/system/linux/src` \
	           -k $(PACKAGEDIR)/system/linux/src/vmlinux

openmct-release:
# creating /etc/openmct-release
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/$(IMAGERELEASEFILENAME)\033[0m"
	@$(ECHO) "OpenMCT $(IMAGETARGET) $(IMAGEVERSION) $(IMAGESUBVERSION) $(IMAGEPRODCODE) $(SOFTWAREVERSION)" > $(NEWROOTDIR)/etc/$(IMAGERELEASEFILENAME)
# creating hostname
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/hostname\033[0m"
	@$(ECHO) $(IMAGETARGET) > $(NEWROOTDIR)/var/etc/hostname
# creating issue
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/issue\033[0m"
	@$(ECHO) "Welcome on $(IMAGETARGET) with OpenMCT $(SOFTWAREVERSION) !" > $(NEWROOTDIR)/etc/issue
	@$(ECHO) "### visit http://www.openmct.org ###" >> $(NEWROOTDIR)/etc/issue
	@$(ECHO) "working Kernel is \r \m (\d \t)" >> $(NEWROOTDIR)/etc/issue

interfaces:
# building the interfaces files
# we have to proof if the values from $(CONFIG) are valid
# is there a .config file?
	@if [ ! -f $(CONFIG) ]; then \
		$(ECHO) -e "\033[1;33mCONFIGURE FAILED\033[0m" ; \
		$(ECHO) "$(CONFIG) does not exist" ; \
		$(ECHO) "You have to run 'make config ...'!"; \
		exit 99; \
	fi
# yes we have a .config file, starting with the loopback device 
	@$(ECHO) "## Loopback interface ##" > $(NEWROOTDIR)/var/etc/network/interfaces
	@$(ECHO) "auto lo" >> $(NEWROOTDIR)/var/etc/network/interfaces
	@$(ECHO) -e "\tiface lo inet loopback" >> $(NEWROOTDIR)/var/etc/network/interfaces
# proofing the values for eth0 and/or for dhcp
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/interfaces based on device \033[1;34m$(BASECONFIG)\033[0m"
	@if [ "$(NET)" != "dhcp" ]; then \
		if ([[ "$(ETH0IPADDR)" = "" ]] || [[ "$(ETH0NETWORK)" = "" ]] || [[ "$(ETH0BROADCAST)" = "" ]]); then \
			$(ECHO) -e "\033[1;33mCONFIGURE FAILED\033[0m" ; \
			$(ECHO) -e "something is wrong with your network config!\nmaybe some misspelling?" ; \
			$(ECHO) -e "ETH0IPADDR=$(ETH0IPADDR)\nETH0NETWORK=$(ETH0NETWORK)\nETH0BROADCAST=$(ETH0BROADCAST)"; \
			exit 99; \
		else \
			$(ECHO) -e "\n## LAN interface eth0  ##" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			$(ECHO) "auto eth0" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			$(ECHO) "iface eth0 inet static" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			$(ECHO) -e "\taddress $(ETH0IPADDR)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			if [ "$(ETH0NETMASK)" != "" ]; then \
				$(ECHO) -e "\tnetmask $(ETH0NETMASK)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			else \
				$(ECHO) -e "\tnetmask 255.255.255.0" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			fi; \
			$(ECHO) -e "\tbroadcast $(ETH0BROADCAST)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			if [ "$(ETH0DEFAULTGW)" != "" ]; then \
				$(ECHO) -e "\tgateway $(ETH0DEFAULTGW)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			else \
			$(ECHO) -e "\033[1;33mWARNING! \033[1;31mno gateway set for eth0!\033[0m Proof your .config!" ; \
			fi; \
			$(ECHO) -e "\033[1;33mCONFIGURE eth0 OK\033[0m" ; \
			if ([[ "$(BASECONFIG)" = "sa113" ]] || [[ "$(BASECONFIG)" = "sa200" ]]); then \
				$(ECHO) -e "\n## LAN interface eth1  ##" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				$(ECHO) "auto eth1" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				$(ECHO) "iface eth1 inet static" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				$(ECHO) -e "\taddress $(ETH1IPADDR)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				if [ "$(ETH1NETMASK)" != "" ]; then \
					$(ECHO) -e "\tnetmask $(ETH1NETMASK)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				else \
					$(ECHO) -e "\tnetmask 255.255.255.0" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				fi; \
				if [ "$(ETH1BROADCAST)" != "" ]; then \
					$(ECHO) -e "\tbroadcast $(ETH1BROADCAST)" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				else \
					$(ECHO) -e "\tbroadcast" $(shell $(SCRIPTDIR)/ip-shift.sh ${ETH1IPADDR}).255 >> $(NEWROOTDIR)/var/etc/network/interfaces; \
				fi; \
				$(ECHO) -e "\033[1;33mCONFIGURE eth1 OK\033[0m" ; \
			fi; \
		fi \
	else \
		$(ECHO) -e "\033[1;33mbuild dhcp config for eth0\033[0m" ; \
		$(ECHO) -e "\n## LAN interface eth0  ##" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
		$(ECHO) "auto eth0" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
		$(ECHO) -e "\tiface eth0 inet dhcp" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
		if ([[ "$(BASECONFIG)" = "sa113" ]] || [[ "$(BASECONFIG)" = "sa200" ]]); then \
			$(ECHO) -e "\033[1;33mbuild dhcp config for eth1\033[0m" ; \
			$(ECHO) -e "\n## LAN interface eth1  ##" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			$(ECHO) "auto eth1" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
			$(ECHO) -e "\tiface eth1 inet dhcp" >> $(NEWROOTDIR)/var/etc/network/interfaces; \
		fi; \
	fi
	@cp $(NEWROOTDIR)/var/etc/network/interfaces $(NEWROOTDIR)/etc/init.d/backup/interfaces.backup

exports:
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/exports\033[0m"
	@if [ ! -f $(CONFIG) ]; then \
		$(ECHO) -e "\033[1;33mCONFIGURE FAILED\033[0m" ; \
		$(ECHO) "$(CONFIG) does not exist" ; \
		$(ECHO) "You have to run 'make config ...'!"; \
		exit 99; \
	fi
	@if [ "$(NET)" != "dhcp" ]; then \
		$(ECHO) "# default nfs release for eth0" > $(NEWROOTDIR)/var/etc/exports; \
		$(ECHO) -e "/usr/data $(ETH0NETWORK)/255.255.255.0(rw,async,no_subtree_check)\n" >> $(NEWROOTDIR)/var/etc/exports; \
		if ([[ "$(BASECONFIG)" = "sa113" ]] || [[ "$(BASECONFIG)" = "sa200" ]]); then \
			$(ECHO) "# default nfs release for eth1" >> $(NEWROOTDIR)/var/etc/exports; \
			$(ECHO) "/usr/data $(shell $(SCRIPTDIR)/ip-shift.sh ${ETH1IPADDR}).0/255.255.255.0(rw,async,no_subtree_check)" >> $(NEWROOTDIR)/var/etc/exports; \
		fi; \
	else \
		$(ECHO) "# default nfs release for eth0" > $(NEWROOTDIR)/var/etc/exports; \
		$(ECHO) "# edit the following line with correct parameters, like the correct IP subnet" > $(NEWROOTDIR)/var/etc/exports; \
		$(ECHO) -e "#/usr/data 192.168.1.0/255.255.255.0(rw,async,no_subtree_check)\n" >> $(NEWROOTDIR)/var/etc/exports; \
		if ([[ "$(BASECONFIG)" = "sa113" ]] || [[ "$(BASECONFIG)" = "sa200" ]]); then \
			$(ECHO) "# default nfs release for eth1" >> $(NEWROOTDIR)/var/etc/exports; \
			$(ECHO) "# edit the following line with correct parameters, like the correct IP subnet" > $(NEWROOTDIR)/var/etc/exports; \
			$(ECHO) "#/usr/data 172.16.0.0/255.255.255.0(rw,async,no_subtree_check)" >> $(NEWROOTDIR)/var/etc/exports; \
		fi; \
	fi

hosts:
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/hosts\033[0m"
	@$(ECHO) "# /etc/hosts" > $(NEWROOTDIR)/var/etc/hosts
	@$(ECHO) -e "# created by Makefile\n# edit by your own if needed\n#\n127.0.0.1\tlocalhost\t$(IMAGETARGET)" >>  $(NEWROOTDIR)/var/etc/hosts
	@if [ "$(NET)" != "dhcp" ]; then \
		$(ECHO) $(ETH0IPADDR) $(IMAGETARGET) >> $(NEWROOTDIR)/var/etc/hosts; \
	fi

resolv.conf:
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m/etc/resolv.conf\033[0m"
	@if [ "$(NET)" = "dhcp" ]; then \
		$(ECHO) "nameserver 127.0.0.1" >> $(NEWROOTDIR)/var/etc/resolv.conf; \
	else \
		$(ECHO) "nameserver $(ETH0DEFAULTGW)" >> $(NEWROOTDIR)/var/etc/resolv.conf; \
	fi
	$(ECHO) "/bin/echo \"You are in! Do the right thing! :-)\"" >>  $(NEWROOTDIR)/var/etc/profile

squashfs:
	@if [ -f $(OUTPUTDIR)/$(ROOT) ]; then rm -rf $(OUTPUTDIR)/$(ROOT) ; fi
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(ROOT)\033[0m"
	$(OUTPUTDIR)/$(MKSQUASHFS) $(NEWROOTDIR) $(OUTPUTDIR)/$(ROOT) $(ROOTOPTS)

jffs2:
	@$(MKDIR) $(JFFS2DIR)
	rm -rf $(JFFS2DIR)/*
	mv $(NEWROOTDIR)/var/* $(JFFS2DIR)
	$(MKFSJFFS2) -d $(JFFS2DIR) -o $(OUTPUTDIR)/$(JFFS2)

$(IMAGEFILENAME): $(OUTPUTDIR)/$(MCT_MODIFY) $(OUTPUTDIR)/$(MKSQUASHFS) $(OUTPUTDIR)/$(KERNEL) etc pkg $(OUTPUTDIR)/$(ROOT) jffs2 squashfs
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(IMAGEFILENAME)\033[0m"
	$(OUTPUTDIR)/$(MCT_MODIFY) -w -i $(IMAGEFILENAME) -m $(IMAGEMODE) -p $(IMAGEPRODCODE) \
		-v $(IMAGEVERSION) -n $(IMAGESUBVERSION) -k $(OUTPUTDIR)/$(KERNEL) \
                -l output/root.bin,output/jffs2.bin \
                -a $(IMAGEFLASHKERNELSIZE),$(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHKERNELSIZE)) \
		-z $(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHETCSIZE))

root.bin:
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(IMAGEFILENAME)\033[0m"
	$(OUTPUTDIR)/$(MCT_MODIFY) -w -i $(IMAGEFILENAME) -m $(IMAGEMODE) -p $(IMAGEPRODCODE) \
		-v $(IMAGEVERSION) -n $(IMAGESUBVERSION) -k $(OUTPUTDIR)/$(KERNEL) \
                -l output/root.bin,output/jffs2.bin \
                -a $(IMAGEFLASHKERNELSIZE),$(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHKERNELSIZE)) \
		-z $(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHETCSIZE))

nfs: etc pkg $(OUTPUTDIR)/$(ROOT) jffs2
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32mdirectory for NFS export (instead of nfs boot for your nas)\033[0m"
	@mkdir -p $(OUTPUTDIR)/nfs
	@cp -au $(NEWROOTDIR)/* $(OUTPUTDIR)/nfs
	@cp -au $(JFFS2DIR)/* $(OUTPUTDIR)/nfs/var
	@cat $(OUTPUTDIR)/nfs/etc/fstab.in | sed -e "s#/dev/mtdblock/#\#/dev/mtdblock/#g" > $(OUTPUTDIR)/nfs/etc/fstab.in
	@$(ECHO) -e "\033[1;32mNOTE! \033[0myou need the following entry in your /etc/exports :"
	@$(ECHO) -e "\t$(OUTPUTDIR)/nfs	[given nas IP]/255.255.255.0(sync,no_subtree_check,rw,all_squash,anonuid=[YOUR uid],anongid=[YOUR gid])"
	@$(ECHO) -e "\n\tfor example: the IP given by the DHCP server is \033[1;34m192.168.1.120 \033[0mand the User ID is \033[1;34m1000\033[0m, your Group ID is \033[1;34m100\033[0m"
	@$(ECHO) -e "\t$(OUTPUTDIR)/nfs	\033[1;34m192.168.1.120\033[0m/255.255.255.0(sync,no_subtree_check,rw,all_squash,anonuid=\033[1;34m1000\033[0m,anongid=\033[1;34m100\033[0m)"
	@$(ECHO)

# do not use this rule! It's actually broken!
rebuild:
	rm -rf $(OUTPUTDIR)/$(JFFS2)
	$(MAKE) jffs2
	@rm -rf $(OUTPUTDIR)/$(ROOT)
	$(MAKE) $(OUTPUTDIR)/$(ROOT)
	@$(ECHO) -e "\033[1;33mCREATE \033[1;32m$(IMAGEFILENAME)\033[0m"
	@if [ -f $(OUTPUTDIR)/$(ROOT) ]; then rm -rf $(OUTPUTDIR)/$(ROOT) ; fi
	$(OUTPUTDIR)/$(MCT_MODIFY) -w -i $(IMAGEFILENAME) -m $(IMAGEMODE) -p $(IMAGEPRODCODE) \
		-v $(IMAGEVERSION) -n $(IMAGESUBVERSION) -k $(OUTPUTDIR)/$(KERNEL) \
                -l output/root.bin,output/jffs2.bin \
                -a $(IMAGEFLASHKERNELSIZE),$(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHKERNELSIZE)) \
		-z $(shell expr $(IMAGEFLASHROOTSIZE) + $(IMAGEFLASHETCSIZE))

