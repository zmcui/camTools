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



enum {
	Layer0 = 0,
	Layer1,
	Layer2,
	Layer3,
};

int dwt_main(unsigned char *yaddr, unsigned char *caddr, int inWidth, int inHeight,
		unsigned char *yll1, unsigned char *uvll1,
		unsigned char *yll2, unsigned char *uvll2,
		unsigned char *yll3, unsigned char *uvll3,
		unsigned char *yll4, unsigned char *uvll4);
#endif // !__DWT_H
