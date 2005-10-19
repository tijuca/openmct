#
# MCT Firmware Builder
#
# Andi <andi@void.at>

# Variables

include $(TOP)Rules

# Directories
KERNELDIR=$(ACTDIR)/kernel
ROOTDIR=$(ACTDIR)/root

# Image Information
IMAGEPRODCODE=0x04
IMAGEMODE=0x0503
IMAGEVERSION=0x107
IMAGESUBVERSION=0x01
IMAGEFLASHMAP=1024,2176,704
IMAGEFILENAME=Void_$(IMAGEVERSION)_$(IMAGESUBVERSION)_$(IMAGEPRODCODE).bin

# Main
all:	directories etc_jffs2 mct_modify kernel_bin root_squashfs image

directories:
	$(shell if [ ! -d $(OUTPUTDIR) ]; then mkdir $(OUTPUTDIR); fi)

image:	
	@if [ ! -f $(OUTPUTDIR)/$(IMAGEFILENAME) ]; then \
		$(SRCDIR)/$(MCT_MODIFY) -w -i $(OUTPUTDIR)/$(IMAGEFILENAME) -m $(IMAGEMODE) -p $(IMAGEPRODCODE) \
		-v $(IMAGEVERSION) -n $(IMAGESUBVERSION) -k $(KERNELDIR)/output/$(VMLINUXBIN).gz \
		-q $(ROOTDIR)/output/$(ROOTSQUASHFS) -j $(OUTPUTDIR)/$(ETCJFFS2) -a $(IMAGEFLASHMAP) ; \
	fi

kernel_bin:	
	$(MAKE) -C $(KERNELDIR) TOP=../$(TOP)

root_squashfs:	kernel
	$(MAKE) -C $(ROOTDIR) TOP=../$(TOP)

etc_jffs2:
	@if [ ! -f $(OUTPUTDIR)/$(BUILD) ]; then \
	 	$(MKFSJFFS2) -l -q -d etc/ -o $(OUTPUTDIR)/$(ETCJFFS2) ; \
		$(TOUCH) $(OUTPUTDIR)/$(BUILD) ; \
	fi

mct_modify:
	@if [ ! -f $(SRCDIR)/$(BUILD) ]; then \
		$(GCC) $(SRCDIR)/$(MCT_MODIFY).c -o $(SRCDIR)/$(MCT_MODIFY) ; \
		$(TOUCH) $(SRCDIR)/$(BUILD) ; \
	fi

clean:
	@if [ -f $(OUTPUTDIR)/$(IMAGEFILENAME) ]; then \
		rm $(OUTPUTDIR)/$(IMAGEFILENAME) ; \
	fi
	@if [ -f $(OUTPUTDIR)/$(ETCJFFS2) ]; then \
		rm $(OUTPUTDIR)/$(ETCJFFS2) ; \
	fi
	@if [ -f $(OUTPUTDIR)/$(BUILD) ]; then \
		rm $(OUTPUTDIR)/$(BUILD) ; \
	fi
	@if [ -f $(SRCDIR)/$(MCT_MODIFY) ]; then \
		rm $(SRCDIR)/$(MCT_MODIFY) ; \
	fi
	@if [ -f $(SRCDIR)/$(BUILD) ]; then \
		rm $(SRCDIR)/$(BUILD) ; \
	fi

distclean: clean
	$(shell if [ -d $(OUTPUTDIR) ]; then rm -rf $(OUTPUTDIR); fi)
	$(MAKE) -C $(KERNELDIR) distclean
	$(MAKE) -C $(ROOTDIR) distclean
