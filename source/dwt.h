#pragma once
#ifndef __DWT_H


#define FALSE -1
#define TRUE 0

#define OutputLayerNum 4
#define UVMEMVALUE 0x200
#define UVMEMVALUE0 0x80

typedef struct tag_DWT
{
	int m_nInputWidth;
	int m_nInputHeight;
	int m_nYPitch[OutputLayerNum];
	int m_nUVPitch[OutputLayerNum];
	int y_width[OutputLayerNum];
	int y_height[OutputLayerNum];
	int uv_width[OutputLayerNum];
	int uv_height[OutputLayerNum];
	int Output_ExtH_En;
}stDWT;



typedef enum {
	Layer0 = 0,
	Layer1,
	Layer2,
	Layer3,
};

int DWT_YProcess(unsigned char *Input_Y, unsigned short *Output_Y[OutputLayerNum], unsigned short *Linebuff_Y[OutputLayerNum], stDWT *dwt_reg, int ExtW);
int DWT_UVProcess(unsigned char *Input_UV, unsigned short *Output_UV[OutputLayerNum], unsigned short *Linebuff_UV[OutputLayerNum], stDWT *dwt_reg, int ExtW);
int Size_Extender(unsigned char *pIn, unsigned char *pOut, int InW, int InH, int OutW, int OutH);
int dwt_main(int argc, char **argv, 
	int *yl1_width, int *yl2_width, int *yl3_width, int *yl4_width, int *yl1_height, int *yl2_height, int *yl3_height, int *yl4_height, 
	int *uvl1_width, int *uvl2_width, int *uvl3_width, int *uvl4_width, int *uvl1_height, int *uvl2_height, int *uvl3_height, int *uvl4_height);

#endif // !__DWT_H