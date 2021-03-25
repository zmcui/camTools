#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void print_usage(char* argv[])
{
    printf("Usage: %s -w <width> -h <height> -f <data format> -i <infile.raw>\n", argv[0]);
    printf("Options:\n");
    printf("  -i <file>        input file\n");
    printf("  -o <file>        output file\n");
    printf("  -w <widht>       specify width, required for YUV input\n");
    printf("  -h <height>      specify height, required for YUV input\n");
    printf("  -f <mipi format> input file mipi raw data format(10: raw10, 12: raw12)\n");
    /*
	 * printf("  -l <left>        bbox left coord\n");
	 * printf("  -t <top>         bbox top coord\n");
	 * printf("  -r <right>       bbox right coord\n");
	 * printf("  -b <bottom>      bbox bottom coord\n");
     */

    exit(1);
}

static const char* threadString = "w:h:i:o:f:l:t:r:b:?";
int main(int argc, char* argv[])
{
    int ret = 0, ch;
    int32_t width_in = 0, height_in = 0;
    int32_t width_ot = 0, height_ot = 0;
    int32_t top = 0, left = 0;
    int32_t bottom = 0, right = 0;
    char* inputFileName = NULL;
    int inputFd, outputFd;
    int status;
    struct stat s;
    size_t size;
    char outputFileName[64] = "";
    int mipi_raw_format = 0;

    while ((ch = getopt(argc, argv, threadString)) != -1) {
        switch (ch) {
        case 'w':
            width_in = atoi(optarg);
            break;
        case 'h':
            height_in = atoi(optarg);
            break;
        case 'i':
            inputFileName = optarg;
            break;
        case 'f':
            mipi_raw_format = atoi(optarg);
            break;
        case 'l':
            left = atoi(optarg);
            break;
        case 't':
            top = atoi(optarg);
            break;
        case 'r':
            right = atoi(optarg);
            break;
        case 'b':
            bottom = atoi(optarg);
            break;
        case '?':
            print_usage(argv);
            return -1;
        }
    }
    // optind is for the extra arguments which are not parsed
    for (int i = 0; optind < argc && i < 10; optind++, i++)
        printf("extra arguments : %s\n", argv[optind]);

    if (!right)
        right = width_in;
    if (!bottom)
        bottom = height_in;
    width_ot = right - left;
    height_ot = bottom - top;

    /* argument verify*/
    if (!inputFileName) {
        printf("error:no input image\n");
        print_usage(argv);
        exit(1);
    }
    if (mipi_raw_format != 10 && mipi_raw_format != 12) {
        printf("error:invalid mipi data format %d\n", mipi_raw_format);
        print_usage(argv);
        exit(1);
    }
    printf("image %s: [%dx%d] of mipi raw%d data format\n", inputFileName, width_in, height_in, mipi_raw_format);

    /* open the file of input image */
    inputFd = open(inputFileName, O_RDONLY);
    if (inputFd < 0) {
        printf("image %s: failed to open, %s\n", inputFileName, strerror(errno));
        exit(1);
    }

    /* get the size of input image */
    status = fstat(inputFd, &s);
    if (status < 0) {
        printf("stat %s failed: %s\n", inputFileName, strerror(errno));
        exit(1);
    }
    size = s.st_size;
    if (size < width_in * height_in * mipi_raw_format / 8) {
        printf("image %s: size 0x%lx less than 0x%x expected!\n",
            inputFileName, size, width_in * height_in * mipi_raw_format / 8);
        exit(1);
    }

    /* memory-map the input image */
    const uint8_t* mmaped = mmap(0, size, PROT_READ, MAP_PRIVATE, inputFd, 0);
    if (mmaped == MAP_FAILED) {
        printf("mmap %s failed: %s", inputFileName, strerror(errno));
        exit(1);
    }

    if (outputFileName[0] == '\0') {
        snprintf(outputFileName, sizeof(outputFileName), "%s16", basename(inputFileName));
    }
    outputFd = open(outputFileName, O_RDWR | O_CREAT, 00666);
    if (outputFd < 0) {
        printf("failed to open %s, %s\n", outputFileName, strerror(errno));
        exit(1);
    }

    if (mipi_raw_format == 10) {
        uint8_t buffer[8];

        for (int i = 0; i < width_in * height_in / 4; ++i) {
            buffer[0] = ((mmaped[0 + i * 5] << 2) + ((mmaped[4 + i * 5]) & 0x3)) & 0xff;
            buffer[1] = ((mmaped[0 + i * 5] >> 6)) & 0x3;

            buffer[2] = ((mmaped[1 + i * 5] << 2) + (((mmaped[4 + i * 5]) >> 2) & 0x3)) & 0xff;
            buffer[3] = ((mmaped[1 + i * 5] >> 6)) & 0x3;

            buffer[4] = ((mmaped[2 + i * 5] << 2) + (((mmaped[4 + i * 5]) >> 4) & 0x3)) & 0xff;
            buffer[5] = ((mmaped[2 + i * 5] >> 6)) & 0x3;

            buffer[6] = ((mmaped[3 + i * 5] << 2) + (((mmaped[4 + i * 5]) >> 6) & 0x3)) & 0xff;
            buffer[7] = ((mmaped[3 + i * 5] >> 6)) & 0x3;

            write(outputFd, buffer, sizeof(buffer));
        }
    }

    if (mipi_raw_format == 12) {
        unsigned char buffer[4];

        for (int i = 0; i < width_in * height_in / 4; ++i) {
            buffer[0] = ((mmaped[0 + i * 3] << 4) + ((mmaped[2 + i * 3]) & 0xf)) & 0xff;
            buffer[1] = ((mmaped[0 + i * 3] >> 4)) & 0xf;

            buffer[2] = ((mmaped[1 + i * 3] << 4) + (((mmaped[2 + i * 3]) >> 4) & 0xf)) & 0xff;
            buffer[3] = ((mmaped[1 + i * 3] >> 4)) & 0xf;

            write(outputFd, buffer, sizeof(buffer));
        }
    }

    printf("saving %s\n", outputFileName);
    close(outputFd);

    munmap((void*)mmaped, size);
    close(inputFd);

    return 0;
}
