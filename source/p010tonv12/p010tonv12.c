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
#include "../common/version.h"
#include "../common/cam_fourcc.h"

static void removeSuffix(char** outNewFileName, const char* inFileName, const char* suffix)
{
    *outNewFileName = strdup(inFileName);
    (*outNewFileName)[strlen(inFileName) - strlen(suffix)] = '\0';
    printf("%s: %s\n", __func__, *outNewFileName);
}

static void print_usage(char* argv[])
{
    printf("Usage: %s -w <width> -h <height> -f <data format> -i <infile.raw>\n", argv[0]);
    printf("Options:\n");
    printf("  -i <file>        input file\n");
    printf("  -o <file>        output file\n");
    printf("  -w <widht>       specify width, required for YUV input\n");
    printf("  -h <height>      specify height, required for YUV input\n");
    printf("  -f <mipi format> input file image format(p010, p016)\n");
    printf("Generic Options:\n");
    printf("  -v               display the version of this program\n");

    exit(1);
}

int main(int argc, char* argv[])
{
    int ret = 0, ch;
    int32_t width_in = 0, height_in = 0;
    int32_t width_ot = 0, height_ot = 0;
    int32_t top = 0, left = 0;
    int32_t bottom = 0, right = 0;
    char* inputFileName = NULL, *inFmt = NULL;
    int inputFd, outputFd;
    int status;
    struct stat s;
    size_t size;
    char outputFileName[256] = "";
    uint32_t fourcc;

    static const char* threadString = "w:h:i:f:v?";
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
            inFmt = optarg;
            break;
        case 'v':
            list_version(argv[0]);
            exit(EXIT_SUCCESS);
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
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

    if (strcmp(inFmt, "p010")) {
        printf("error:invalid input format %s\n", inFmt);
        print_usage(argv);
        exit(1);
    } else {
        fourcc = CAM_PIX_FMT_P010;
    }
    printf("image %s: [%dx%d] of %s\n", inputFileName, width_in, height_in, inFmt);

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
    if (size < width_in * height_in * 3) {
        printf("image %s: size 0x%lx less than 0x%x expected!\n",
            inputFileName, size, width_in * height_in * 2);
        exit(1);
    }

    /* memory-map the input image */
    const uint8_t* mmaped = mmap(0, size, PROT_READ, MAP_PRIVATE, inputFd, 0);
    if (mmaped == MAP_FAILED) {
        printf("mmap %s failed: %s", inputFileName, strerror(errno));
        exit(1);
    }

    if (outputFileName[0] == '\0') {
        char *tmp;
        removeSuffix(&tmp, inputFileName, "p010");
        snprintf(outputFileName, sizeof(outputFileName), "%snv12", tmp);
    }
    outputFd = open(outputFileName, O_RDWR | O_CREAT, 00666);
    if (outputFd < 0) {
        printf("failed to open %s, %s\n", outputFileName, strerror(errno));
        exit(1);
    }

    if (fourcc == CAM_PIX_FMT_P010) {
        uint8_t buffer;

        for (int i = 0; i < width_in * height_in * 3 / 2; ++i) {
            write(outputFd, mmaped + i * 2 + 1, sizeof(buffer));
        }
    }

    printf("saving %s\n", outputFileName);
    close(outputFd);

    munmap((void*)mmaped, size);
    close(inputFd);

    return 0;
}
