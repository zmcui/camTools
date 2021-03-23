#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void gen_raw10(unsigned int* addr, int width, int height)
{
    printf("gen_raw10:%dx%d", width, height);
    unsigned int BGGR8[8][4] = {
        {0xEE, 0x00, 0x00, 0x00},
        {0x00, 0x44, 0x44, 0x00},
        {0x00, 0x88, 0x88, 0x00},
        {0x00, 0x00, 0x00, 0xEE},
        {0x00, 0x00, 0x00, 0x00},
        {0x88, 0x88, 0x88, 0x88},
        {0xEE, 0xEE, 0xEE, 0xEE},
        {0x88, 0x00, 0x00, 0x88},
    };

    unsigned short BGGR[8][4];

    int i, j, fd, ret;
    for ( i = 0; i < 8; i++ ) {
        for ( j = 0; j < 4; j++ ) {
            int tmp = ((int)BGGR8[i][j]) << 2;
            BGGR[i][j] = tmp;
        }
    }

    unsigned short *buf = (unsigned short *)addr;
    for ( i = 0; i < height; i++ ) {
        printf(".");
        for ( j = 0; j < width; j++ ) {
            buf[i*width+j] = BGGR8[i*8/height][((i%2)<<1) + (j%2)];
        }
    }
    printf("done\n");
}

int main(int argc,char *argv[])
{
	int w=320;
	int h=240;
	int size;
 	if(argc>=3)
    	{
        w = atoi(argv[1]);
        h = atoi(argv[2]);
    	}
	printf("gen %dx%d raw10 data\n",w,h);
	//unsigned int buf[w*h*2];
	size = w*h*2;
	unsigned int *buf = malloc(size);
	char filename[2048]={'\0'};
	gen_raw10(buf,w,h);
	sprintf(filename, "%dx%d.raw10",w,h);
	int fd_out = open(filename,O_CREAT|O_WRONLY|O_TRUNC);
	if(fd_out == -1)
        {
         printf("error open out file error\n");
        }
	write(fd_out,buf,w*h*2);

	free(buf);
	return 0;
}
