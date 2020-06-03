#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dwt.h"



int main(int argc, char **argv)
{
	int yl1_width = 0;
	int yl2_width = 0;
	int yl3_width = 0;
	int yl4_width = 0;
	int yl1_height = 0;
	int yl2_height = 0;
	int yl3_height = 0;
	int yl4_height = 0;

	int uvl1_width = 0;
	int uvl2_width = 0;
	int uvl3_width = 0;
	int uvl4_width = 0;
	int uvl1_height = 0;
	int uvl2_height = 0;
	int uvl3_height = 0;
	int uvl4_height = 0;

	dwt_main(argc, argv, 
		&yl1_width, &yl2_width, &yl3_width, &yl4_width, &yl1_height, &yl2_height, &yl3_height, &yl4_height,
		&uvl1_width, &uvl2_width, &uvl3_width, &uvl4_width, &uvl1_height, &uvl2_height, &uvl3_height, &uvl4_height);
	
	return TRUE;
}