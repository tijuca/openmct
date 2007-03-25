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
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
#define	MCT_DEFAULT_FIRMWARESIZE	0x800000
#define MCT_DEFAULT_FILELIST 		"root.squashfs,etc.jffs2"
#define MCT_DEFAULT_OFFSETLIST		"1088,2112"
#define MCT_DEFAULT_MAXRAMDISKSIZE	2816

enum 	MCT_MODI 			{ EXTRACT, CREATE, CREATEEXT } mode;

#define	MCT_FILL_SUFFIX			".fill"
#define	MCT_FILENAME_SIZE		256

/*
 * adjust endianness auf header 
 * to make mct_modify work on big-endian machines 
 *
 * source for Int32swap() and Int16Swap():
 * http://www.codeproject.com/cpp/endianness.asp
 */
int isbigendian() {
	union { 
		long l; 
		char c[sizeof (long)]; 
	} u;

	u.l = 1; 
        return (u.c[0] != 1);
}

uint32_t Int32swap (uint32_t nLongNumber) {
	return (((nLongNumber&0x000000FF)<<24) + 
	        ((nLongNumber&0x0000FF00)<<8)  +
	        ((nLongNumber&0x00FF0000)>>8)  +
	        ((nLongNumber&0xFF000000)>>24));
}

uint16_t Int16Swap (uint16_t nValue) {
	return (((nValue>> 8)) | (nValue << 8));
}

/* function:    mct_modify_adjust_endianness
 * parameter:   header ... header struct whos elements are to adjust
 *
 * changes elements in header-struct by swapping endianness for big-endian systems
 */
void mct_modify_adjust_endianness(struct mct_h_t *header) {
	if (isbigendian()) {
		header->magic = Int16Swap(header->magic);
		header->product = Int16Swap(header->product);
		header->version = Int16Swap(header->version);
		header->version_b = Int16Swap(header->version_b);
		header->ramdisk_offset_from_base = Int32swap(header->ramdisk_offset_from_base);
		header->signature_offset_from_base = Int32swap(header->signature_offset_from_base);
		header->ramdisk_end_offset_from_kernel = Int32swap(header->ramdisk_end_offset_from_kernel);
		header->ramdisk_start_offset_from_kernel = Int32swap(header->ramdisk_start_offset_from_kernel);
		header->signature_offset_from_base_b = Int32swap(header->signature_offset_from_base_b);
		header->zero_word = Int32swap(header->zero_word);
	}
}


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
	printf("-f ... flash base (default: 0x%08lx)\n", MCT_DEFAULT_FLASHBASE);
	printf("-u ... update offset (default: 0x%08x)\n", MCT_DEFAULT_UPDATEOFFSET);
	printf("-s ... signature (default: 0x%08lx)\n", MCT_DEFAULT_SIGNATURE);
	printf("-l ... File List (default: %s)\n", MCT_DEFAULT_FILELIST);
	printf("-a ... Offset List in Flash (default: %s)\n", MCT_DEFAULT_OFFSETLIST);
	printf("-z ... Maximal Ramdisk Size in 1k blocks (default: %d)\n", MCT_DEFAULT_MAXRAMDISKSIZE);

	printf("\nexamples:\n");
	printf("$ %s -x -i kfs_sa113_v152_040405.bin\n", argv[0]);
	printf("	create a vmkernel.bin.gz and a ramdisk.gz file from image kfs_sa113_v152_040405.bin\n\n");
	printf("$ %s -c -i image.bin -m 0x0503 -p 0x005 -v 0x148 -n 0x1 -k mykernel.gz -r myramdisk.gz\n", argv[0]);
	printf("	create an image from mykernel.gz and myramdisk.gz\n");
	printf("$ %s -w -i VOID-2005-09-07-00-07.bin -m 0x0503 -p 0x0005 -v 0x148 -n 0x1 -k mykernel.gz -l root.fs,etc.fs -a 1024,2176,704\n", argv[0]);

	return 0;
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

	printf("%d\n", sizeof(struct mct_h_t));

    	printf("[*] magic:   				0x%04x\n", header->magic);
	printf("[*] product: 				0x%04x\n", header->product);
    	printf("[*] version:		 		0x%04x\n", header->version);
	printf("[*] subversion:				0x%04x\n", header->version_b);
 	printf("[*] ramdisk offset from base of flash: 	0x%08x\n", header->ramdisk_offset_from_base - flashbase);
 	printf("[*] ramdisk offset from kernel header:  0x%08x\n", header->ramdisk_start_offset_from_kernel);
	printf("[*] ramdisk size: 			0x%08x	%d=%.2f\n", size_ramdisk, size_ramdisk, size_ramdisk/1024.0);
	printf("[*] kernel size: 			0x%08x	%d=%.2f\n", size_kernel, size_kernel, size_kernel/1024.0);
 	printf("[*] signature offset raw: 		0x%08x\n", header->signature_offset_from_base);
	printf("[*] signature offset: 			0x%08x\n", sig_offset);

	return 0;
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

	return 0;
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

	return 0;
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

	return 0;
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
	char*		buf = (char *)malloc(MCT_DEFAULT_FIRMWARESIZE);
	int		retcode = -1;

	if (buf == NULL) {
		printf("[*] can't allocate buffer for firmware\n");
	} else {
		fd_firmware = open(firmware, O_RDONLY);
		if (fd_firmware != -1) {
			fd_kernel = open(kernel, O_CREAT | O_WRONLY, 0644);
			if (fd_kernel != -1) {
				fd_ramdisk = open(ramdisk, O_CREAT | O_WRONLY, 0644);
				if (fd_ramdisk != -1) {
					if (read(fd_firmware, buf, MCT_DEFAULT_FIRMWARESIZE) != -1) {
						header = (struct mct_h_t *)buf;
						mct_modify_adjust_endianness(header);
						mct_modify_dump_header(header, flashbase, updateoffset);
						printf("[*] writing kernel to file '%s'...\n", kernel);
						mct_modify_write_kernel(buf, fd_kernel);
						printf("[*] writing ramdisk to file '%s'...\n", ramdisk);
						mct_modify_write_ramdisk(buf, fd_ramdisk);
						close(fd_firmware);
						close(fd_kernel);
						close(fd_ramdisk);
						retcode = 0;
					} else {
						printf("[*] can't read firmware\n");
					}
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
		free(buf);
	}

	return retcode;
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
	char*		buf_kernel = (char *)malloc(MCT_DEFAULT_FIRMWARESIZE);
	char*		buf_ramdisk = (char *)malloc(MCT_DEFAULT_FIRMWARESIZE);
	int		size_kernel;
	int		size_ramdisk;
	int		retcode = -1;

	if (buf_kernel == NULL || buf_ramdisk == NULL) {
		printf("[*] can't allocate buffer for kernel and/or ramdisk\n");
	} else {
		fd_firmware = open(firmware, O_CREAT | O_WRONLY, 0644);
		if (fd_firmware != -1) {
			fd_kernel = open(kernel, O_RDONLY);
			if (fd_kernel != -1) {
				fd_ramdisk = open(ramdisk, O_RDONLY);
				if (fd_ramdisk != -1) {
					size_kernel = (read(fd_kernel, buf_kernel, MCT_DEFAULT_FIRMWARESIZE) + 15) & ~(16UL-1);
					size_ramdisk = (read(fd_ramdisk, buf_ramdisk, MCT_DEFAULT_FIRMWARESIZE) + 15) & ~(16UL-1);
					mct_modify_set_header(header, size_kernel, size_ramdisk, flashbase, updateoffset);
					mct_modify_adjust_endianness(header);
					mct_modify_dump_header(header, flashbase, updateoffset);
					printf("[*] writing header to file '%s'...\n", firmware);
					if (write(fd_firmware, header, sizeof(struct mct_h_t)) == -1) {
						printf("[*] write header failed\n");
						retcode = -1;
					}
					printf("[*] writing kernel to file '%s'...\n", firmware);
					if (write(fd_firmware, buf_kernel, size_kernel) == -1) {
						printf("[*] write kernel failed\n");	
						retcode = -1;
					}
					printf("[*] writing ramdisk to file '%s'...\n", firmware);
					if (write(fd_firmware, buf_ramdisk, size_ramdisk) == -1) {
						printf("[*] write ramdisk failed\n");	
						retcode = -1;
					}
					if (write(fd_firmware, &signature, sizeof(signature)) == -1) {
						printf("[*] write signature failed\n");	
						retcode = -1;
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

	return retcode;
}

int mct_modify_comma_count(char *string) {
	int i;
	int cnt_comma;

	for (cnt_comma = 0, i = 0; i < strlen(string); i++) {
		if (string[i] == ',') {
			cnt_comma++;
		}
	}
	
	return cnt_comma;
}

char **mct_modify_array_fill(char *list, int *array_size) {
	int cnt_comma = mct_modify_comma_count(list);
	char *element;
	char **array;

	array = (char **)malloc(sizeof(char *) * cnt_comma);
	*array_size = 0;
	if (array) {
		element = strtok(list, ",");
		while (element) {
			array[(*array_size)] = (char*)malloc(strlen(element));
			strcpy( (char*)array[(*array_size)], element);
			(*array_size)++;
			element = strtok(NULL, ",");
		}
	}
	return array;
}

int mct_modify_array_free(char **array, int array_size) {
}

/* function:	mct_modify_createramdisk
 * parameter:	ramdisk ... filename for output ramdisk file
 *		files ... list of filenames that will be used (comma seperated)
 *		offsets ... list of offsets that will be used (comma seperated)
 *
 * create an extended ramdisk
 */
int mct_modify_createramdisk(char *ramdisk, int size_ramdisk, int size_kernel, char *files, char *offsets) {
	char*		buf_ramdisk = (char*)malloc(size_ramdisk * 1024);
	int		fd_ramdisk;
	int		fd_file;
	int		retcode = -1;
	int		i;
	int		cnt_files;
	int		cnt_offsets;
	char		**array_files = NULL;
	char		**array_offsets = NULL;
	int		offset = 0;

	if (buf_ramdisk == NULL) {
		printf("[*] can't allocate buffer for ramdisk\n");
	} else {
		array_files = mct_modify_array_fill(files, &cnt_files);
		array_offsets = mct_modify_array_fill(offsets, &cnt_offsets);

		if (array_files == NULL) {
			printf("[*] can't allocate memory for files\n");
		} else if (array_offsets == NULL) {
			printf("[*] can't allocate memory for offsets\n");
		} else if (cnt_files != cnt_offsets) {
			printf("[*] please specifiy the same count of files and offsets\n");
		} else {
			memset(buf_ramdisk, -1, size_ramdisk * 1024);
			for (i = 0; i < cnt_files; i++) {
				fd_file = open(array_files[i], O_RDONLY);
				if (fd_file != -1) {
					offset += atoi(array_offsets[i]) * 1024 - (size_kernel * 1024);
					printf("[*] read '%s' at position 0x%08x\n", array_files[i], offset);
					read(fd_file, buf_ramdisk + offset, (size_ramdisk * 1024) - offset);
					close(fd_file);
				} else {
					printf("[*] can't open '%s' for reading\n", array_files[i]);
				}
			}
		}

		fd_ramdisk = open(ramdisk, O_CREAT | O_WRONLY, 0644);
		if (fd_ramdisk != -1) {
			write(fd_ramdisk, buf_ramdisk, size_ramdisk * 1024);
			close(fd_ramdisk);
		} else {
			printf("[*] can't open '%s' for writing\n", ramdisk);
		}

		mct_modify_array_free(array_files, cnt_files);
		mct_modify_array_free(array_offsets, cnt_offsets);
		
		memset(buf_ramdisk, 0, size_ramdisk * 1024);
		free(buf_ramdisk);
	}
	
	return retcode;
}

/* function:    mct_modify_createkernel
 * parameter:   kernel ... filename for input kernel file
 *              kernelflashsize ... size of kernel in flash memory
 *
 * create an extended kernel
 */
int mct_modify_createkernel(char *kernel, int kernelflashsize, char *fill_kernel) {
	int		size_kernel = kernelflashsize * 1024 - sizeof(struct mct_h_t);
	int 		file_size_kernel;
	char*		buf_kernel = (char*) malloc(size_kernel);
	char*		buf_dummy = (char*) malloc(1);
	int 		fd_kernel;
	int		retcode = -1;

	if (buf_kernel == NULL) {
		printf("[*] can't allocate buffer for kernel\n");
	} else {
		fd_kernel = open(kernel, O_RDONLY);
		if (fd_kernel != -1) {
			memset(buf_kernel, 0, size_kernel);
			file_size_kernel = read(fd_kernel, buf_kernel, size_kernel);
			if (read(fd_kernel,buf_dummy,1) > 0) {
				printf ("[*] WARNING: !!! Kernel size too large for specified partition size !!!\n");
				printf ("***************************************************\n");
				printf ("* * *  I M A G E   W I L L   N O T   W O R K  * * *\n");
				printf ("***************************************************\n");
			} else {
				printf ("[*] Reading %d bytes (max. %d).\n",file_size_kernel,size_kernel);
			}
			close (fd_kernel);
			fd_kernel = open(fill_kernel, O_CREAT | O_WRONLY, 0644);
			if (fd_kernel != -1) {
				if (write(fd_kernel, buf_kernel, size_kernel) == size_kernel) {
					retcode = 0;
				} else {
					printf("[*] can't write data to kernel\n");
				}
				close(fd_kernel);
			} else {
				printf("[*] can't open kernel '%s' for writing\n", fill_kernel);
			}
		} else {
			printf("[*] can't open kernel '%s' for reading\n", kernel);
		}
	}

	return retcode;
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
	char*		filelist = MCT_DEFAULT_FILELIST;
	char*		offsetlist = MCT_DEFAULT_OFFSETLIST;
	int		flashbase = MCT_DEFAULT_FLASHBASE;
	int		updateoffset = MCT_DEFAULT_UPDATEOFFSET;
	int		signature = MCT_DEFAULT_SIGNATURE;
	int		maxramdisksize = MCT_DEFAULT_MAXRAMDISKSIZE;
	int		kernelsize = 0;
	char		fill_kernel[MCT_FILENAME_SIZE];
	struct mct_h_t 	header;

	printf("%s v1.1\n", argv[0]);

	if (argc <= 1) {
		mct_modify_help(argc, argv);
	} else {
		while ( (opt = getopt(argc, argv, "xcwi:m:p:v:n:r:k:f:u:s:l:a:z:")) != -1) {
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
				case 'l': filelist = optarg;
					  break;
				case 'a': offsetlist = optarg;
					  sscanf(offsetlist, "%d", &kernelsize);
					  break;
				case 'z': sscanf(optarg, "%d", &maxramdisksize);
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
					printf("[*] ramdisk/flash map (%s)...\n", offsetlist);
					printf("[*] create fake '%s' with '%s' and '%s'...\n", ramdisk, filelist, offsetlist);
                                        mct_modify_createramdisk(ramdisk, maxramdisksize, kernelsize, filelist, offsetlist);
					snprintf(fill_kernel, sizeof(fill_kernel), "%s%s", kernel, MCT_FILL_SUFFIX);
					printf("[*] fill up size (up: %d) kernel '%s' to '%s'...\n", kernelsize, kernel, fill_kernel);
                                        mct_modify_createkernel(kernel, kernelsize, fill_kernel);
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
