#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include "dwt.h"
#include "finch_buffer_mgr.h"


static void display_usage(char *p)
{
	printf("Usage: %s <test> <options>\n", p);
}


static const char *argString = "w:h:i:?";
int main(int argc, char **argv)
{
	int ret = 0, ch;
	int32_t width_in = 0 , height_in = 0;
	char *inFileName = NULL;
	char outFileName[64];
	int inputFd, outputFd;
	int status;
	struct stat s;
	size_t size;
	unsigned char *pInputImage, *pOutputImage;

	while ((ch = getopt(argc, argv, argString)) != -1) {
		switch (ch) {
		case 'w':
			printf("option-w:'%s'\n", optarg);
			width_in = atoi(optarg);
			break;
		case 'h':
			printf("option-h:'%s'\n", optarg);
			height_in = atoi(optarg);
			break;
		case 'i':
			printf("option-i:'%s'\n", optarg);
			inFileName = optarg;
			break;
		case '?':
			display_usage(argv[0]);
			return -1;
		}
	}
	// optind is for the extra arguments which are not parsed
	for (int i = 0; optind < argc && i < 10; optind++, i++)
		printf("extra arguments : %s\n", argv[optind]);

	if (!inFileName || !width_in || !height_in) {
		printf("error:no input image size[%dx%d]\n",
		       width_in, height_in);
		display_usage(argv[0]);
		exit(1);
	} else {
		printf("image %s\n", inFileName);
	}

	/* open the file of input image */
	inputFd = open(inFileName, O_RDONLY);
	if (inputFd < 0) {
		printf("failed to open %s, %s\n", inFileName, strerror(errno));
		exit(1);
	} 
	
	/* get the size of input image */
	status = fstat(inputFd, &s);
	if (status < 0) {
		printf("stat %s failed: %s\n", inFileName, strerror(errno));
		exit(1);
	}
	size = s.st_size;
	if (size < width_in * height_in * 3 / 2) {
		printf("%s size 0x%lx less than 0x%x\n", inFileName, size, width_in * height_in * 3 / 2);
		exit(1);
	}

	/* memory-map the input image */
	pInputImage = (unsigned char *)mmap(0, size, PROT_READ, MAP_PRIVATE, inputFd, 0);
	if (pInputImage == MAP_FAILED) {
		printf("mmap %s failed: %s", inFileName, strerror(errno));
		exit(1);
	}

	unsigned char *yAddr = pInputImage;
	unsigned char *cAddr = pInputImage + width_in * height_in;

	uint32_t outputSize = 0;
    	CAM_BUFFER_PLANE_S dwt1[DWT_MAX_PLANES], dwt2[DWT_MAX_PLANES], dwt3[DWT_MAX_PLANES], dwt4[DWT_MAX_PLANES];
	ASR_SIZE_S resolution;
	resolution.width = width_in;
	resolution.height = height_in;
        outputSize = getDwtSize(resolution, dwt1, dwt2, dwt3, dwt4, NULL);
	pOutputImage = (unsigned char *)malloc(outputSize);
	updateCamBufferDWTAddr(pOutputImage, dwt1, dwt2, dwt3, dwt4);

	dwt_main(yAddr, cAddr, width_in, height_in,
		(unsigned char *)dwt1[0].virAddr, (unsigned char *)dwt1[1].virAddr,
		(unsigned char *)dwt2[0].virAddr, (unsigned char *)dwt2[1].virAddr,
		(unsigned char *)dwt3[0].virAddr, (unsigned char *)dwt3[1].virAddr,
		(unsigned char *)dwt4[0].virAddr, (unsigned char *)dwt4[1].virAddr);

	snprintf(outFileName, sizeof(outFileName), "%s_dwt", inFileName);
	outputFd = open(outFileName, O_RDWR | O_CREAT, 00666);
	if (outputFd < 0) {
		printf("failed to open %s, %s\n", outFileName, strerror(errno));
		exit(1);
	} 
	write(outputFd, pOutputImage, outputSize);
	printf("saving %s\n", outFileName);
	close(outputFd);

	munmap((void *)pInputImage, size);
	close(inputFd);
	
	return TRUE;
}
