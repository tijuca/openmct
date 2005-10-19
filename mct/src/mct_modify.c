/***********************************************************
 * mct_modify.c
 *
 * local firmware modify utility for mct nas products
 * (for example http://www.claxan.de)
 *
 * Author: Andi <andi@void.at>
 *
 * Note:
 * This coded is based on the source by Roy Franz
 * pack.c and unpack.c.
 * http://www.royfranz.com/Hacking_the_Tritton_NAS120.html
 *
 * Greetz to Greuff, philipp and the other hoagie-fellas :-)
 *
 * THIS FILE IS FOR STUDYING PURPOSES ONLY AND A PROOF-OF-
 * CONCEPT. THE AUTHOR CAN NOT BE HELD RESPONSIBLE FOR ANY
 * DAMAGE DONE USING THIS PROGRAM.
 *
 ************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* mct header for firmware. start at 0x20000 from flash base */
struct mct_h_t {
	int16_t 	magic;
	int16_t 	product;
	int16_t 	version;
	uint16_t 	version_b;
	uint32_t 	ramdisk_offset_from_base;
	uint32_t 	signature_offset_from_base;
	uint32_t 	ramdisk_end_offset_from_kernel;
	uint32_t 	ramdisk_start_offset_from_kernel;
	uint32_t	signature_offset_from_base_b;
	uint32_t 	zero_word;
};

/* defines */
#define MCT_DEFAULT_MAGIC		0x0503
#define MCT_DEFAULT_PRODUCT 		0x0004
#define MCT_DEFAULT_VERSION 		0x0148
#define MCT_DEFAULT_SUBVERSION 		0x0001
#define MCT_DEFAULT_RAMDISK		"ramdisk.gz"
#define MCT_DEFAULT_KERNEL		"vmkernel.bin.gz"
#define MCT_DEFAULT_FLASHBASE		(0xbfc00000ul)
#define MCT_DEFAULT_UPDATEOFFSET   	0x20000
#define MCT_DEFAULT_SIGNATURE 		(0xAA5555AAUL)
#define	MCT_DEFAULT_FIRMWARESIZE	0x400000
#define MCT_DEFAULT_ROOTFS		"root.squashfs"
#define MCT_DEFAULT_ETCFS		"etc.jffs2"
#define	MCT_DEFAULT_KERNELFLASHSIZE	1024
#define	MCT_DEFAULT_ROOTFLASHSIZE	2176
#define	MCT_DEFAULT_ETCFLASHSIZE	704 

enum 	MCT_MODI 			{ EXTRACT, CREATE, CREATEEXT } mode;

#define	MCT_FILL_SUFFIX			".fill"
#define	MCT_FILENAME_SIZE		256

/* function: 	mct_modify_help
 * parameter:	argc ... argument counter from main 
 * 		argv ... argument array from main
 *
 * display a help screen
 */
int mct_modify_help(int argc, char **argv) {
	printf("-x ... extract kernel and ramdisk from image\n");
	printf("-c ... create a new image\n");
	printf("-w ... create a new image (extended version with Squash and JFFS2)\n");
	printf("-i ... image filename\n");
	printf("-m ... magic number (default: 0x%04x)\n", MCT_DEFAULT_MAGIC);
	printf("-p ... product number (default: 0x%04x)\n", MCT_DEFAULT_MAGIC);
	printf("-v ... version (default: 0x%04x)\n", MCT_DEFAULT_VERSION);
	printf("-n ... subversion (default: 0x%04x)\n", MCT_DEFAULT_SUBVERSION);
	printf("-r ... ramdisk file (default: %s)\n", MCT_DEFAULT_RAMDISK);
	printf("-k ... vmkernel file (default: %s)\n", MCT_DEFAULT_KERNEL);
	printf("-f ... flash base (default: 0x%08x)\n", MCT_DEFAULT_FLASHBASE);
	printf("-u ... update offset (default: 0x%08x)\n", MCT_DEFAULT_UPDATEOFFSET);
	printf("-s ... signature (default: 0x%08x)\n", MCT_DEFAULT_SIGNATURE);
	printf("-q ... Root Filesystem File (Squash Filesystem) (default: %s)\n", MCT_DEFAULT_ROOTFS);
	printf("-j ... /etc Filesystem File (JFFS2 Filesystem) (default: %s)\n", MCT_DEFAULT_ETCFS);
	printf("-a ... Map Array: Kernel,Root,Etc (default: %d,%d,%d)\n", MCT_DEFAULT_KERNELFLASHSIZE, MCT_DEFAULT_ROOTFLASHSIZE, MCT_DEFAULT_ETCFLASHSIZE);

	printf("\nexamples:\n");
	printf("$ %s -x -i kfs_sa113_v152_040405.bin\n", argv[0]);
	printf("	create a vmkernel.bin.gz and a ramdisk.gz file from image kfs_sa113_v152_040405.bin\n\n");
	printf("$ %s -c -i image.bin -m 0x0503 -p 0x005 -v 0x148 -n 0x1 -k mykernel.gz -r myramdisk.gz\n", argv[0]);
	printf("	create an image from mykernel.gz and myramdisk.gz\n");
	printf("$ %s -w -i VOID-2005-09-07-00-07.bin -m 0x0503 -p 0x0005 -v 0x148 -n 0x1 -k mykernel.gz -q root.fs -j etc.fs -a 1024,2176,704\n");
}

/* function:	mct_modify_dump_header
 * parameter:	header ... pointer to header data
 *
 * display header data
 */
int mct_modify_dump_header(struct mct_h_t *header, int flashbase, int updateoffset) {
	int size_kernel = header->ramdisk_start_offset_from_kernel - sizeof(struct mct_h_t);
	int size_ramdisk = header->ramdisk_end_offset_from_kernel - header->ramdisk_start_offset_from_kernel;
	int sig_offset = header->signature_offset_from_base - flashbase - updateoffset;

    	printf("[*] magic:   				0x%04x\n", header->magic);
	printf("[*] product: 				0x%04x\n", header->product);
    	printf("[*] version:		 		0x%04x\n", header->version);
	printf("[*] subversion:				0x%04x\n", header->version_b);
 	printf("[*] ramdisk offset from base of flash: 	0x%08x\n", header->ramdisk_offset_from_base - flashbase);
 	printf("[*] ramdisk offset from kernel header:  0x%08x\n", header->ramdisk_start_offset_from_kernel);
    	printf("[*] ramdisk size: 			0x%08x\n", size_ramdisk);
 	printf("[*] kernel size: 			0x%08x\n", size_kernel);
 	printf("[*] signature offset raw: 		0x%08x\n", header->signature_offset_from_base);
	printf("[*] signature offset: 			0x%08x\n", sig_offset);
}

/* function:	mct_modify_set_header
 * parameter:	header ... pointer to header data
 * 		size_kernel ... size of kernel
 * 		size_ramdisk ... size of ramdisk
 *
 * update header information for writing image
 */
int mct_modify_set_header(struct mct_h_t *header, int size_kernel, int size_ramdisk, int flashbase, int updateoffset) {
	header->ramdisk_start_offset_from_kernel = size_kernel + sizeof(struct mct_h_t);
	header->ramdisk_offset_from_base = header->ramdisk_start_offset_from_kernel +
                                           flashbase + updateoffset;
	header->signature_offset_from_base = header->signature_offset_from_base_b =
					     size_kernel + size_ramdisk + sizeof(struct mct_h_t) +
					     updateoffset + flashbase;
	header->ramdisk_end_offset_from_kernel = header->ramdisk_start_offset_from_kernel +
						 size_ramdisk;
	header->zero_word = 0;
}

/* function:	mct_modify_write_kernel
 * parameter:	buf ... firmware image in memory
 *		fd_kernel ... file descriptor for kernel output file
 *
 * write kernel from firmeware image to a file
 */
int mct_modify_write_kernel(char *buf, int fd_kernel) {
        struct mct_h_t *header = (struct mct_h_t *) buf;
        int size_kernel = header->ramdisk_start_offset_from_kernel - sizeof(struct mct_h_t);

    	if (write(fd_kernel, buf + sizeof(struct mct_h_t), size_kernel) == -1) {
		printf("[*] write kernel failed\n");
	}
}

/* function:	mct_modify_write_ramdisk
 * parameter:	buf ... firmware image in memory
 *		fd_ramdisk ... file descriptor for ramdisk output file
 *
 * write ramdisk from firmeware image to a file
 */
int mct_modify_write_ramdisk(char *buf, int fd_ramdisk) {
        struct mct_h_t *header = (struct mct_h_t *) buf;
	int size_ramdisk = header->ramdisk_end_offset_from_kernel - header->ramdisk_start_offset_from_kernel;

 	if (write(fd_ramdisk, buf + header->ramdisk_start_offset_from_kernel, size_ramdisk) == -1) {
		printf("[*] write ramdisk failed\n");
	}
}

/* function:	mct_modify_extract
 * parameter:	firmware ... location of firmware image file (input)
 * 		kernel ... locatin of kernel file (output)
 * 		ramdisk ... locatino of ramdisk file  (output)
 *
 */
int mct_modify_extract(char *firmware, char *kernel, char *ramdisk, int flashbase, int updateoffset,
		       int signature, struct mct_h_t *header) {
	int		fd_firmware;
	int		fd_kernel;
	int		fd_ramdisk;
	char		buf[MCT_DEFAULT_FIRMWARESIZE];

	fd_firmware = open(firmware, O_RDONLY);
	if (fd_firmware != -1) {
		fd_kernel = open(kernel, O_CREAT | O_WRONLY, 0644);
		if (fd_kernel != -1) {
			fd_ramdisk = open(ramdisk, O_CREAT | O_WRONLY, 0644);
			if (fd_ramdisk != -1) {
				read(fd_firmware, buf, MCT_DEFAULT_FIRMWARESIZE);
				header = (struct mct_h_t *)buf;
				mct_modify_dump_header(header, flashbase, updateoffset);
				printf("[*] writing kernel to file '%s'...\n", kernel);
				mct_modify_write_kernel(buf, fd_kernel);
				printf("[*] writing ramdisk to file '%s'...\n", ramdisk);
				mct_modify_write_ramdisk(buf, fd_ramdisk);
				close(fd_firmware);
				close(fd_kernel);
				close(fd_ramdisk);
			} else {
				printf("[*] can't open ramdisk '%s' for writing\n", ramdisk);
				close(fd_firmware);
				close(fd_kernel);
			}
		} else {
			printf("[*] can't open kernel '%s' for writing\n", kernel);
			close(fd_firmware);
		}
	} else {
		printf("[*] can't open firmware '%s' for reading\n", firmware);
	}
}

/* function:	mct_modify_create
 * parameter:	firmware ... location of firmware image file (output)
 * 		kernel ... locatin of kernel file (input)
 * 		ramdisk ... locatino of ramdisk file  (input)
 *
 */
int mct_modify_create(char *firmware, char *kernel, char *ramdisk, int flashbase, int updateoffset, 
		      int signature, struct mct_h_t *header) {
	int		fd_firmware;
	int		fd_kernel;
	int		fd_ramdisk;
	char		buf_kernel[MCT_DEFAULT_FIRMWARESIZE];
	char		buf_ramdisk[MCT_DEFAULT_FIRMWARESIZE];
	int		size_kernel;
	int		size_ramdisk;

	fd_firmware = open(firmware, O_CREAT | O_WRONLY, 0644);
	if (fd_firmware != -1) {
		fd_kernel = open(kernel, O_RDONLY);
		if (fd_kernel != -1) {
			fd_ramdisk = open(ramdisk, O_RDONLY);
			if (fd_ramdisk != -1) {
				size_kernel = (read(fd_kernel, buf_kernel, MCT_DEFAULT_FIRMWARESIZE) + 16) & ~(16UL-1);
				size_ramdisk = (read(fd_ramdisk, buf_ramdisk, MCT_DEFAULT_FIRMWARESIZE) + 16) & ~(16UL-1);

				mct_modify_set_header(header, size_kernel, size_ramdisk, flashbase, updateoffset);

				mct_modify_dump_header(header, flashbase, updateoffset);
				printf("[*] writing header to file '%s'...\n", firmware);
				if (write(fd_firmware, header, sizeof(struct mct_h_t)) == -1) {
					printf("[*] write header failed\n");
				}
				printf("[*] writing kernel to file '%s'...\n", firmware);
				if (write(fd_firmware, buf_kernel, size_kernel) == -1) {
					printf("[*] write kernel failed\n");	
				}
				printf("[*] writing ramdisk to file '%s'...\n", firmware);
				if (write(fd_firmware, buf_ramdisk, size_ramdisk) == -1) {
					printf("[*] write ramdisk failed\n");	
				}
				if (write(fd_firmware, &signature, sizeof(signature)) == -1) {
					printf("[*] write signature failed\n");	
				}
				close(fd_firmware);
				close(fd_kernel);
				close(fd_ramdisk);
			} else {
				printf("[*] can't open ramdisk '%s' for writing\n", ramdisk);
				close(fd_firmware);
				close(fd_kernel);
			}
		} else {
			printf("[*] can't open kernel '%s' for reading\n", kernel);
			close(fd_firmware);
		}
	} else {
		printf("[*] can't open firmware '%s' for reading\n", firmware);
	}
}

/* function:	mct_modify_createramdisk
 * parameter:	ramdisk ... filename for output ramdisk file
 *		rootfs ... filename for input root filesystem
 *		rootflashsize ... size of root filesystem in flash memory
 *		etcfs ... filename for input etc filesystem
 *		etcflashsize ... size of etc filesystem in flash flash
 *
 * create an extended ramdisk
 */
int mct_modify_createramdisk(char *ramdisk, char *rootfs, int rootflashsize, char *etcfs, int etcflashsize) {
	int 		size_ramdisk = (rootflashsize + etcflashsize) * 1024;
	char*		buf_ramdisk = (char*)malloc(size_ramdisk);
	int 		fd_rootfs;
	int		fd_etcfs;
	int		fd_ramdisk;
	int		realsize_rootfs;
	int		realsize_etcfs;

	if (buf_ramdisk == NULL) {
		printf("[*] can't allocate buffer for ramdisk\n");
	} else {
		memset(buf_ramdisk, 0, rootflashsize * 1024);
		memset(buf_ramdisk + rootflashsize * 1024, 0xFF, etcflashsize * 1024);
		fd_rootfs = open(rootfs, O_RDONLY);
		if (fd_rootfs != -1) {
			fd_etcfs = open(etcfs, O_RDONLY);
			if (fd_etcfs != -1) {
				fd_ramdisk = open(ramdisk, O_CREAT | O_WRONLY, 0644);
				if (fd_ramdisk != -1) {
					realsize_rootfs = read(fd_rootfs, buf_ramdisk, rootflashsize * 1024);
					realsize_etcfs = read(fd_etcfs, buf_ramdisk + rootflashsize * 1024, etcflashsize * 1024);
					write(fd_ramdisk, buf_ramdisk, size_ramdisk);
					close(fd_ramdisk);
				} else {
					printf("[*] can't open ramdisk '%s' for writing\n", ramdisk);
				}
				close(fd_etcfs);
			} else {
				printf("[*] can't open etc filesystem '%s' for reading \n", etcfs);
			}
			close(fd_rootfs);
		} else {
			printf("[*] can't open root filesystem '%s' for reading\n", rootfs);
		}
		free(buf_ramdisk);
	}
}

/* function:    mct_modify_createkernel
 * parameter:   kernel ... filename for input kernel file
 *              kernelflashsize ... size of kernel in flash memory
 *
 * create an extended kernel
 */
int mct_modify_createkernel(char *kernel, int kernelflashsize, char *fill_kernel) {
	int		size_kernel = kernelflashsize * 1024 - 48;
	char*		buf_kernel = (char*) malloc(size_kernel);
	int 		fd_kernel;

	fd_kernel = open(kernel, O_RDONLY);
	if (fd_kernel) {
		memset(buf_kernel, 0, size_kernel);
		read(fd_kernel, buf_kernel, size_kernel);
		close(fd_kernel);
		fd_kernel = open(fill_kernel, O_CREAT | O_WRONLY, 0644);
		if (fd_kernel != -1) {
			write(fd_kernel, buf_kernel, size_kernel);
			close(fd_kernel);
		} else {
			printf("[*] can't open kernel '%s' for writing\n", fill_kernel);
		}
	} else {
		printf("[*] can't open kernel '%s' for reading\n", kernel);
	}
}

/* function: 	main
 * parameter:	argc ... argument counter from main 
 * 		argv ... argument array from main
 *
 * display a help screen
 */
int main(int argc, char **argv) {
	char 		opt = 0;
	char*		image = NULL;
	int 		magic = MCT_DEFAULT_MAGIC;
	int 		product = MCT_DEFAULT_PRODUCT;
	int		version = MCT_DEFAULT_VERSION;
	int		subversion = MCT_DEFAULT_SUBVERSION;
	char*		ramdisk = MCT_DEFAULT_RAMDISK;
	char*		kernel = MCT_DEFAULT_KERNEL;
	char*		rootfs = MCT_DEFAULT_ROOTFS;
	char*		etcfs = MCT_DEFAULT_ETCFS;
	int		flashbase = MCT_DEFAULT_FLASHBASE;
	int		updateoffset = MCT_DEFAULT_UPDATEOFFSET;
	int		signature = MCT_DEFAULT_SIGNATURE;
	int		kernelflashsize = MCT_DEFAULT_KERNELFLASHSIZE;
	int		rootflashsize = MCT_DEFAULT_ROOTFLASHSIZE;
	int		etcflashsize = MCT_DEFAULT_ETCFLASHSIZE;
	char		fill_kernel[MCT_FILENAME_SIZE];
	struct mct_h_t 	header;

	printf("%s v1.1\n", argv[0]);

	if (argc <= 1) {
		mct_modify_help(argc, argv);
	} else {
		while ( (opt = getopt(argc, argv, "xcwi:m:p:v:n:r:k:f:u:s:q:j:a:")) != -1) {
			switch(opt) {
				case 'x': mode = EXTRACT;
					  break;
				case 'c': mode = CREATE;
					  break;
				case 'w': mode = CREATEEXT;
					  break;
				case 'i': image = optarg;
					  break;
				case 'm': sscanf(optarg, "0x%04x", &magic);
					  break;
				case 'p': sscanf(optarg, "0x%04x", &product);
					  break;
				case 'v': sscanf(optarg, "0x%04x", &version);
					  break;
				case 'n': sscanf(optarg, "0x%04x", &subversion);
					  break;
				case 'r': ramdisk = optarg;
					  break;
				case 'k': kernel = optarg;
					  break;
				case 'f': sscanf(optarg, "0x%08x", &flashbase);
					  break;
				case 'u': sscanf(optarg, "0x%08x", &updateoffset);
					  break;
				case 's': sscanf(optarg, "0x%08x", &signature);
					  break;
				case 'q': rootfs = optarg;
					  break;
				case 'j': etcfs = optarg;
					  break;
				case 'a': sscanf(optarg, "%d,%d,%d", &kernelflashsize, &rootflashsize, &etcflashsize);
					  break;
				default:
					  printf("[*] unknown option: '%c'\n", opt);
			}					  
		}

		if (mode == EXTRACT) {
			if (image == NULL) {
				printf("[*] missing image firmware. please use option -i [imagefilename]\n");
			} else {
				mct_modify_extract(image, kernel, ramdisk, flashbase, updateoffset, signature, &header);
			}
		} else if (mode == CREATE || mode == CREATEEXT) {
			if (image == NULL) {
				printf("[*] missing image firmware. please use option -i [imagefilename]\n");
			} else {
				header.magic = magic;
				header.product = product;
				header.version = version;
				header.version_b = subversion;
                                if (mode == CREATEEXT) {
					printf("[*] flash map (%d,%d,%d)...\n", kernelflashsize, rootflashsize, 
						etcflashsize);
					printf("[*] create fake '%s' with '%s' and '%s'...\n", ramdisk, rootfs, etcfs);
                                        mct_modify_createramdisk(ramdisk, rootfs, rootflashsize, etcfs, etcflashsize);
					snprintf(fill_kernel, sizeof(fill_kernel), "%s%s", kernel, MCT_FILL_SUFFIX);
					printf("[*] fill up kernel '%s' to '%s'...\n", kernel, fill_kernel);
                                        mct_modify_createkernel(kernel, kernelflashsize, fill_kernel);
					mct_modify_create(image, fill_kernel, ramdisk, flashbase, updateoffset, signature, 
							  &header);
					unlink(ramdisk);
					unlink(fill_kernel);
                                } else {
					mct_modify_create(image, kernel, ramdisk, flashbase, updateoffset, signature, 
							  &header);
				}
			}
		} else {
			printf("[*] missing mode. please use option -x or -c\n");
		}
	}

	return 0;
}	
