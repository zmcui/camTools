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

#include "common/common.h"

static void print_usage(void)
{
	printf("Usage: nv12crop -i <infile.nv12>\n");
	printf("Options:\n");
	printf("  -i <file>        input file (nv12 formats supported)\n");
	printf("  -o <file>        output file (nv12 formats supported)\n");
	printf("  -w <widht>       specify width, required for YUV input\n");
	printf("  -h <height>      specify height, required for YUV input\n");
	printf("  -l <left>        bbox left coord\n");
	printf("  -t <top>         bbox top coord\n");
	printf("  -r <right>       bbox right coord\n");
	printf("  -b <bottom>      bbox bottom coord\n");
    printf("Generic Options:\n");
    printf("  -v               display the version of this program\n");

	exit(1);
}

static const char *threadString = "w:h:i:l:t:r:b:v?";
int main(int argc, char *argv[])
{
	int ret = 0, ch;
	int32_t width_in = 0 , height_in = 0;
	int32_t width_ot = 0 , height_ot = 0;
	int32_t top = 0, left = 0;
	int32_t bottom = 0, right = 0;
	char *inputFileName = NULL;
	int inputFd, outputFd;
	int status;
	struct stat s;
	size_t size;
	const unsigned char *mmaped;
	char outputFileName[64];

	while ((ch = getopt(argc, argv, threadString)) != -1) {
		switch (ch) {
		case 'w':
			/* printf("option-width:'%s'\n", optarg); */
			width_in = atoi(optarg);
			break;
		case 'h':
			/* printf("option-height:'%s'\n", optarg); */
			height_in = atoi(optarg);
			break;
		case 'i':
			/* printf("option-inputimg:'%s'\n", optarg); */
			inputFileName = optarg;
			break;
		case 'l':
			/* printf("option-top left:'%s'\n", optarg); */
			left = atoi(optarg);
			break;
		case 't':
			/* printf("option-top top:'%s'\n", optarg); */
			top = atoi(optarg);
			break;
		case 'r':
			/* printf("option-top right:'%s'\n", optarg); */
			right = atoi(optarg);
			break;
		case 'b':
			/* printf("option-top bottom:'%s'\n", optarg); */
			bottom = atoi(optarg);
			break;
        case 'v':
            list_version(argv[0]);
            exit(EXIT_SUCCESS);
		case '?':
			print_usage();
			return -1;
		}
	}
	// optind is for the extra arguments which are not parsed
	for (int i = 0; optind < argc && i < 10; optind++, i++)
		printf("extra arguments : %s\n", argv[optind]);

	width_ot = right - left;
	height_ot = bottom - top;

	if (!inputFileName || !width_in || !height_in) {
		printf("error:no input image size[%dx%d]\n",
		       width_in, height_in);
		print_usage();
		exit(1);
	} else if(width_ot < 0 || width_ot > width_in ||
		height_ot < 0 || height_ot > height_in) {
		printf("error: input size[%dx%d] vs output size[%dx%d]\n",
		       width_in, height_in, width_ot, height_ot);
		print_usage();
		exit(1);
	} else {
		printf("image %s \n", inputFileName);
		printf("crop [%d,%d]--[%d,%d]\n",
		       left, top, right, bottom);
	}

	/* open the file of input image */
	inputFd = open(inputFileName, O_RDONLY);
	if (inputFd < 0) {
		printf("failed to open %s, %s\n", inputFileName, strerror(errno));
		exit(1);
	} 
	
	/* get the size of input image */
	status = fstat(inputFd, &s);
	if (status < 0) {
		printf("stat %s failed: %s\n", inputFileName, strerror(errno));
		exit(1);
	}
	size = s.st_size;
	if (size < width_in * height_in * 3 / 2) {
		printf("%s size 0x%lx less than 0x%x\n", inputFileName, size, width_in * height_in * 3 / 2);
		exit(1);
	}

	/* memory-map the input image */
	mmaped = mmap(0, size, PROT_READ, MAP_PRIVATE, inputFd, 0);
	if (mmaped == MAP_FAILED) {
		printf("mmap %s failed: %s", inputFileName, strerror(errno));
		exit(1);
	}

	snprintf(outputFileName, sizeof(outputFileName), "input_%dx%d.nv12", width_ot, height_ot);
	outputFd = open(outputFileName, O_RDWR | O_CREAT, 00666);
	if (outputFd < 0) {
		printf("failed to open %s, %s\n", outputFileName, strerror(errno));
		exit(1);
	} 
	const unsigned char *yAddr, *cAddr;

	yAddr = mmaped + left + (width_in * top);
	for (int i = 0; i < height_ot; ++i) {
		/* for (int j = 0; j < width_ot ; j++) { */
		write(outputFd, yAddr, width_ot);
		yAddr += width_in;
		/* } */
	}

	cAddr = mmaped + (width_in * height_in) + left + (width_in * top) / 2;
	for (int i = 0; i < height_ot / 2; ++i) {
		/* for (int j = 0; j < width_ot ; j++) { */
			write(outputFd, cAddr, width_ot);
			cAddr += width_in;
		/* } */
	}
	printf("saving %s\n", outputFileName);
	close(outputFd);
	
	munmap((void *)mmaped, size);
	close(inputFd);

	return 0;
}
