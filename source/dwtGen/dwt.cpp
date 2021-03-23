#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dwt.h"


int DWT_YL0(unsigned short *Output_Y, unsigned short *Linebuff_Y, unsigned char *pY, int width, int height, int evenline, int linecount)
{
	int k, w, evenwidth;

	if (Output_Y == NULL ||  Linebuff_Y == NULL ||  pY == NULL) {
		return FALSE;
	}

	if ((width%2) == 0){
		w = width >> 1;
		evenwidth = 1;
	} else {
		w = (width+1)>>1;
		evenwidth = 0;
	}


	if (evenline) {
		if (linecount == (height-1)){//last line    height is odd
			for (k = 0; k < (w - 1); k++) {
				Output_Y[k] = pY[2*k] + pY[2*k+1];
			}
			
			if(evenwidth){
				Output_Y[k] = pY[2*k] + pY[2*k+1];
			} else {
				Output_Y[k] = pY[2*k];
			}
		} else {
			for (k = 0; k < (w - 1); k++) {
				Linebuff_Y[k] = pY[2*k] + pY[2*k+1];
			}
			
			if(evenwidth){
				Linebuff_Y[k] = pY[2*k] + pY[2*k+1];
			} else {
				Linebuff_Y[k] = pY[2*k];
			}
		}
	
	}
	else {
		for (k = 0; k < (w - 1); k++) {
			Output_Y[k] = pY[2*k] + pY[2*k+1] + Linebuff_Y[k];
		}

		if(evenwidth){
			Output_Y[k] = pY[2*k] + pY[2*k+1] + Linebuff_Y[k];
		} else {
			Output_Y[k] = pY[2*k] + Linebuff_Y[k];
		}

	}


	return TRUE;
}

int DWT_UVL0(unsigned short *Output_UV, unsigned short *Linebuff_UV, unsigned char *pUV, int width, int height, int evenline, int linecount)
{
	int k, w, evenwidth;;

	if (Output_UV == NULL || Linebuff_UV == NULL || pUV == NULL) {
		return FALSE;
	}

	if ((width%4) == 0){
		w = width >> 1;
		evenwidth = 1;
	} else {
		w = ((width >> 2)<<1) + 2;
		evenwidth = 0;
	}


	if (evenline) {

		if (linecount == (height-1)){//last line    height is odd
			for (k = 0; k < (w - 2); k = k + 2) {
				Output_UV[k] = pUV[2 * k] + pUV[2 * k + 2]+UVMEMVALUE0+UVMEMVALUE0;
				Output_UV[k + 1] = pUV[2 * k + 1] + pUV[2 * k + 3]+UVMEMVALUE0+UVMEMVALUE0;
			}
			if(evenwidth){
				Output_UV[k] = pUV[2 * k] + pUV[2 * k + 2]+UVMEMVALUE0+UVMEMVALUE0;
				Output_UV[k + 1] = pUV[2 * k + 1] + pUV[2 * k + 3]+UVMEMVALUE0+UVMEMVALUE0;				
			}else {
				Output_UV[k] = pUV[2 * k]+UVMEMVALUE0+UVMEMVALUE0+UVMEMVALUE0;
				Output_UV[k + 1] = pUV[2 * k + 1]+UVMEMVALUE0+UVMEMVALUE0+UVMEMVALUE0;
			}

		} else {
			for (k = 0; k < (w - 2); k = k + 2) {
				Linebuff_UV[k] = pUV[2 * k] + pUV[2 * k + 2];
				Linebuff_UV[k + 1] = pUV[2 * k + 1] + pUV[2 * k + 3];
			}
			if(evenwidth){
				Linebuff_UV[k] = pUV[2 * k] + pUV[2 * k + 2];
				Linebuff_UV[k + 1] = pUV[2 * k + 1] + pUV[2 * k + 3];
			} else {
				Linebuff_UV[k] = pUV[2 * k]+UVMEMVALUE0;
				Linebuff_UV[k + 1] = pUV[2 * k + 1]+UVMEMVALUE0;
			}
		}
	}
	else {

		for (k = 0; k < (w - 2); k = k + 2) {
			Output_UV[k] = pUV[2 * k] + pUV[2 * k + 2] + Linebuff_UV[k];
			Output_UV[k + 1] = pUV[2 * k + 1] + pUV[2 * k + 3] + Linebuff_UV[k + 1];
		}

		if(evenwidth){
			Output_UV[k] = pUV[2 * k] + pUV[2 * k + 2] + Linebuff_UV[k];
			Output_UV[k + 1] = pUV[2 * k + 1] + pUV[2 * k + 3] + Linebuff_UV[k + 1];

		} else {
			Output_UV[k] = pUV[2 * k]+UVMEMVALUE0 + Linebuff_UV[k];
			Output_UV[k + 1] = pUV[2 * k + 1]+UVMEMVALUE0 + Linebuff_UV[k + 1];
		}
	}


	return TRUE;
}


int DWT_YL123(unsigned short *Output_Y, unsigned short *Linebuff_Y, unsigned short *pY, int width, int height, int evenline, int linecount)
{
	int k, w, evenwidth;

	if (Output_Y == NULL || Linebuff_Y == NULL || pY == NULL) {
		return FALSE;
	}

	if ((width%2) == 0){
		w = width >> 1;
		evenwidth = 1;
	} else {
		w = (width+1)>>1;
		evenwidth = 0;
	}


	if (evenline) {
		if (linecount == (height-1)){//last line    height is odd
			for (k = 0; k < (w - 1); k++) {
				Output_Y[k] = (((pY[2*k] + pY[2*k+1] + 1) >> 1)+1)>>1;
			}

			if(evenwidth){
				Output_Y[k] = (((pY[2*k] + pY[2*k+1] + 1) >> 1) + 1)>>1;
			} else {
				Output_Y[k] = (((pY[2*k]+1)>>1)+1)>>1;
			}
		} else {
			for (k = 0; k < (w - 1); k++) {
				Linebuff_Y[k] = (pY[2*k] + pY[2*k+1] + 1) >> 1;
			}

			if(evenwidth){
				Linebuff_Y[k] = (pY[2*k] + pY[2*k+1] + 1) >> 1;
			} else {
				Linebuff_Y[k] = (pY[2*k]+1)>>1;
			}
		}

	}
	else {
		for (k = 0; k < (w - 1); k++) {
			Output_Y[k] = (((pY[2*k] + pY[2*k+1] + 1) >> 1) + Linebuff_Y[k] + 1) >> 1;
		}

		if(evenwidth){
			Output_Y[k] = (((pY[2*k] + pY[2*k+1] + 1) >> 1) + Linebuff_Y[k] + 1) >> 1;
		} else {
			Output_Y[k] = (((pY[2*k]+1)>>1) + Linebuff_Y[k] + 1) >> 1;
		}
	}

	return TRUE;
}

int DWT_UVL123(unsigned short *Output_UV, unsigned short *Linebuff_UV, unsigned short *pUV, int width, int height, int evenline, int linecount)
{
	int k, w, evenwidth;

	if (Output_UV == NULL ||  Linebuff_UV == NULL || pUV == NULL) {
		return FALSE;
	}


	if ((width%4) == 0){
		w = width >> 1;
		evenwidth = 1;
	} else {
		w = ((width >> 2)<<1) + 2;
		evenwidth = 0;
	}


	if (evenline) {
		if (linecount == (height-1)){//last line    height is odd
			for (k = 0; k < (w - 2); k = k + 2) {
				Output_UV[k] = (((pUV[2 * k] + pUV[2 * k + 2] + 1) >> 1)+UVMEMVALUE+1)>>1;
				Output_UV[k + 1] = (((pUV[2 * k + 1] + pUV[2 * k + 3] + 1) >> 1)+UVMEMVALUE+1)>>1;
			}

			if(evenwidth){
				Output_UV[k] = (((pUV[2 * k] + pUV[2 * k + 2] + 1) >> 1)+UVMEMVALUE+1)>>1;
				Output_UV[k + 1] = (((pUV[2 * k + 1] + pUV[2 * k + 3] + 1) >> 1)+UVMEMVALUE+1)>>1;
			} else {
				Output_UV[k] = (((pUV[2 * k]+UVMEMVALUE+1)>>1)+UVMEMVALUE+1)>>1;
				Output_UV[k + 1] = (((pUV[2 * k + 1]+UVMEMVALUE+1)>>1)+UVMEMVALUE+1)>>1;
			}
		} else {
			for (k = 0; k < (w - 2); k = k + 2) {
				Linebuff_UV[k] = (pUV[2 * k] + pUV[2 * k + 2] + 1) >> 1;
				Linebuff_UV[k + 1] = (pUV[2 * k + 1] + pUV[2 * k + 3] + 1) >> 1;
			}

			if(evenwidth){
				Linebuff_UV[k] = (pUV[2 * k] + pUV[2 * k + 2] + 1) >> 1;
				Linebuff_UV[k + 1] = (pUV[2 * k + 1] + pUV[2 * k + 3] + 1) >> 1;
			} else {
				Linebuff_UV[k] = (pUV[2 * k]+UVMEMVALUE+1)>>1;
				Linebuff_UV[k + 1] = (pUV[2 * k + 1]+UVMEMVALUE+1)>>1;
			}
		}
	}
	else {

		for (k = 0; k <  (w - 2); k = k + 2) {
			Output_UV[k] = (((pUV[2 * k] + pUV[2 * k + 2] + 1) >> 1) + Linebuff_UV[k] + 1) >> 1;
			Output_UV[k + 1] = (((pUV[2 * k + 1] + pUV[2 * k + 3] + 1) >> 1) + Linebuff_UV[k + 1] + 1) >> 1;
		}

		if(evenwidth){
			Output_UV[k] = (((pUV[2 * k] + pUV[2 * k + 2] + 1) >> 1) + Linebuff_UV[k] + 1) >> 1;
			Output_UV[k + 1] = (((pUV[2 * k + 1] + pUV[2 * k + 3] + 1) >> 1) + Linebuff_UV[k + 1] + 1) >> 1;
		} else {
			Output_UV[k] = (((pUV[2 * k]+UVMEMVALUE+1)>>1) + Linebuff_UV[k] + 1) >> 1;
			Output_UV[k + 1] = (((pUV[2 * k + 1]+UVMEMVALUE+1)>>1) + Linebuff_UV[k + 1] + 1) >> 1;
		}
	}

	return TRUE;
}



int DWT_YProcess(unsigned char *Input_Y, unsigned short *Output_Y[OutputLayerNum], unsigned short *Linebuff_Y[OutputLayerNum], stDWT *dwt_reg, int ExtW)
{
	int evenline[OutputLayerNum];
	int k[OutputLayerNum] = {0};
	int curw, curh;

	evenline[Layer0] = ~0;
	evenline[Layer1] = ~0;
	evenline[Layer2] = ~0;
	evenline[Layer3] = ~0;

	for (k[Layer0] = 0; k[Layer0] < dwt_reg->m_nInputHeight;) {
		curw = dwt_reg->m_nInputWidth;
		curh = dwt_reg->m_nInputHeight;
		DWT_YL0((Output_Y[Layer0] + ((k[Layer0] >>1)*(ExtW>>1))), Linebuff_Y[Layer0], (Input_Y + k[Layer0] * curw), curw, curh, evenline[Layer0], k[Layer0]);
		
		if ((k[Layer0] % 2 != 0) || (k[Layer0] == curh -1)) {
			curw = dwt_reg->y_width[Layer0];
			curh = dwt_reg->y_height[Layer0];
			DWT_YL123((Output_Y[Layer1] + ((k[Layer1] >> 1)*(ExtW >> 2))), Linebuff_Y[Layer1], (Output_Y[Layer0] + ((k[Layer0] >> 1)*(ExtW >> 1))), curw, curh, evenline[Layer1], k[Layer1]);

			if ((k[Layer1] % 2 != 0) || (k[Layer1] == curh -1)) {
				curw = dwt_reg->y_width[Layer1];
				curh = dwt_reg->y_height[Layer1];
				DWT_YL123((Output_Y[Layer2] + ((k[Layer2] >> 1)*(ExtW >> 3))), Linebuff_Y[Layer2], (Output_Y[Layer1] + ((k[Layer1] >> 1)*(ExtW >> 2))), curw, curh, evenline[Layer2], k[Layer2]);

				if ((k[Layer2] % 2 != 0) || (k[Layer2] == curh -1)) {
					curw = dwt_reg->y_width[Layer2];
					curh = dwt_reg->y_height[Layer2];
					DWT_YL123((Output_Y[Layer3] + ((k[Layer3] >> 1)*(ExtW >> 4))), Linebuff_Y[Layer3], (Output_Y[Layer2] + ((k[Layer2] >> 1)*(ExtW >> 3))), curw, curh, evenline[Layer3], k[Layer3]);
					k[Layer3]++;
					evenline[Layer3] = ~evenline[Layer3];
				}
				k[Layer2]++;
				evenline[Layer2] = ~evenline[Layer2];
			}
			
			k[Layer1]++;
			evenline[Layer1] = ~evenline[Layer1];
		}

		k[Layer0]++;
		evenline[Layer0] = ~evenline[Layer0];
	
	}


	return TRUE;
}


int DWT_UVProcess(unsigned char *Input_UV, unsigned short *Output_UV[OutputLayerNum], unsigned short *Linebuff_UV[OutputLayerNum], stDWT *dwt_reg, int ExtW)
{
	int evenline[OutputLayerNum];
	int k[OutputLayerNum] = { 0 };
	int curw, curh;

	evenline[Layer0] = ~0;
	evenline[Layer1] = ~0;
	evenline[Layer2] = ~0;
	evenline[Layer3] = ~0;

	for (k[Layer0] = 0; k[Layer0] < (dwt_reg->m_nInputHeight >> 1);) {
		curw = dwt_reg->m_nInputWidth;
		curh = dwt_reg->m_nInputHeight >> 1;
		DWT_UVL0((Output_UV[Layer0] + ((k[Layer0] >> 1)*(ExtW >> 1))), Linebuff_UV[Layer0], (Input_UV + k[Layer0] * curw), curw, curh, evenline[Layer0], k[Layer0]);

		if ((k[Layer0] % 2 != 0) || (k[Layer0] == curh -1)) {
			curw = dwt_reg->uv_width[Layer0];
			curh = dwt_reg->uv_height[Layer0];
			DWT_UVL123((Output_UV[Layer1] + ((k[Layer1] >> 1)*(ExtW >> 2))), Linebuff_UV[Layer1], (Output_UV[Layer0] + ((k[Layer0] >> 1)*(ExtW >> 1))), curw, curh, evenline[Layer1], k[Layer1]);

			if ((k[Layer1] % 2 != 0) || (k[Layer1] == curh -1)) {
				curw = dwt_reg->uv_width[Layer1];
				curh = dwt_reg->uv_height[Layer1];
				DWT_UVL123((Output_UV[Layer2] + ((k[Layer2] >> 1)*(ExtW >> 3))), Linebuff_UV[Layer2], (Output_UV[Layer1] + ((k[Layer1] >> 1)*(ExtW >> 2))), curw, curh, evenline[Layer2], k[Layer2]);

				if ((k[Layer2] % 2 != 0) || (k[Layer2] == curh -1)) {
					curw = dwt_reg->uv_width[Layer2];
					curh = dwt_reg->uv_height[Layer2];
					DWT_UVL123((Output_UV[Layer3] + ((k[Layer3] >> 1)*(ExtW >> 4))), Linebuff_UV[Layer3], (Output_UV[Layer2] + ((k[Layer2] >> 1)*(ExtW >> 3))), curw, curh, evenline[Layer3], k[Layer3]);
					k[Layer3]++;
					evenline[Layer3] = ~evenline[Layer3];
				}
				k[Layer2]++;
				evenline[Layer2] = ~evenline[Layer2];
			}
			
			k[Layer1]++;
			evenline[Layer1] = ~evenline[Layer1];
		}

		k[Layer0]++;
		evenline[Layer0] = ~evenline[Layer0];


	}


	return TRUE;
}

int Size_Extender(unsigned char *pIn, unsigned char *pOut, int InW, int InH, int OutW, int OutH)
{
	int i,j;
	int ExtW, ExtH;
	int Size, ExtSize;

	ExtW = OutW - InW;
	ExtH = OutH - InH;
	ExtSize = OutW*OutH;
	Size = InW*InH;

	/*copy pIn  Y*/
	for (i=0;i<InH;i++)
	{
		for(j=0;j<InW;j++)
		{
			pOut[i*OutW+j] = pIn[i*InW+j];//Y
		}
	}


	/*copy pIn  UV*/
	for (i=0;i<InH/2;i++)
	{
		for(j=0;j<InW;j++)
		{
			pOut[i*OutW+j+ExtSize] = pIn[i*InW+j+Size];//UV
		}
	}


	/*copy Column  Y*/
	for (i=0;i<InH;i++)
	{
		for(j=InW;j<InW+ExtW;j++)
		{
			pOut[i*OutW+j] = 0;//pIn[i*InW+InW-1];//Y
		}
	}

	/*copy Column  UV*/
	for (i=0;i<InH/2;i++)
	{
		for(j=InW;j<InW+ExtW;j=j+2)
		{
			pOut[i*OutW+j+ExtSize] = 0;//pIn[i*InW+InW-2+Size];//U
			pOut[i*OutW+j+1+ExtSize] = 0;//pIn[i*InW+InW-1+Size];//V
		}
	}

	/*copy Row  Y*/
	for (i=InH;i<InH+ExtH;i++)
	{
		for(j=0;j<OutW;j++)
		{
			pOut[i*OutW+j] = 0;//pOut[(InH-1)*OutW+j];//Y
		}
	}


	/*copy Column  UV*/
	for (i=InH/2;i<(InH+ExtH)/2;i++)
	{
		for(j=0;j<OutW;j++)
		{
			pOut[i*OutW+j+ExtSize] = 0;//pOut[(InH/2-1)*OutW+j+Size];//UV
		}
	}

	return TRUE;
}


int write_packet_data(unsigned short *src, unsigned char *dst, int width, int height)
{
	int pixel[4];
	int i, j, k = 0;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j = j + 4) {
			pixel[0] = src[i*width + j];
			pixel[1] = src[i*width + j + 1];
			pixel[2] = src[i*width + j + 2];
			pixel[3] = src[i*width + j + 3];

			dst[k++] = pixel[0] & 0xff;
			dst[k++] = ((pixel[1] & 0x3f) << 2) | ((pixel[0] & 0x300) >> 8);
			dst[k++] = ((pixel[2] & 0xf) << 4) | ((pixel[1] & 0x3c0) >> 6);
			dst[k++] = ((pixel[3] & 0x3) << 6) | ((pixel[2] & 0x3f0) >> 4);
			dst[k++] = ((pixel[3] & 0x3fc) >> 2);
		}
	}

	return k;
}

// int write_packet_image(unsigned char *pPacketImage, int InW, int InH, int ExtW, int ExtH, unsigned char *pIn, unsigned short *Output_Y[OutputLayerNum], unsigned short *Output_UV[OutputLayerNum])
int write_packet_image(int InW, int InH, int ExtW, int ExtH, unsigned short *Output_Y[OutputLayerNum], unsigned short *Output_UV[OutputLayerNum],
		unsigned char *Packed_Y[OutputLayerNum], unsigned char *Packed_UV[OutputLayerNum])
{
	int i, j;
	int w = InW, h = InH;
	int wsize = 0, llsize = 0;

	/* CLOG_INFO("%s E", __func__); */
	printf("%s E\n", __func__);
	w = ExtW >> 1;
	h = ExtH >> 1;

	//layer1~4  y
	for (i = 0; i < 4; i++)
	{
		llsize = write_packet_data(Output_Y[i], Packed_Y[i], w, h);
		//trace_without_prefix("data.save.binary YLL%d_%dx%d.nv12 0x%08x++0x%x\r\n", i + 1, w, h, Output_Y[i], llsize);
		wsize += llsize;
		w = w >> 1;
		h = h >> 1;
	}


	w = ExtW >> 1;
	h = ExtH >> 2;

	//layer1~4  uv
	for (i = 0; i < 4; i++)
	{
		llsize = write_packet_data(Output_UV[i], Packed_UV[i], w, h);
		//trace_without_prefix("data.save.binary UVLL%d_%dx%d.nv12 0x%08x++0x%x\r\n", i + 1, w, h, Output_UV[i], llsize);
		wsize += llsize;
		w = w >> 1;
		h = h >> 1;
	}

	return wsize;
}


int save_file_packet_image_LL0(unsigned char *pIn, int width, int height)
{
	FILE *fid = NULL;
	int w = width, h = height;
	int size;
	int wsize = 0;

	fid = fopen("YLL0.raw", "wb");
	if (pIn && fid)
	{
		fwrite(pIn, sizeof(unsigned char), w*h, fid);
		fclose(fid);
	}
	wsize = w * h;
	
	w = width;
	h = height >> 1;
	size = w * h;
	fid = fopen("UVLL0.raw", "wb");
	if (pIn && fid)
	{
		fwrite((pIn + wsize), sizeof(unsigned char), size, fid);
		fclose(fid);
	}

	return wsize;
}


void save_packet_img(char *filename, unsigned char *pIn, int ByteW, int ByteH, int ExtByteW)
{
	int i,j;
	FILE *fid = NULL;
	unsigned char wdata;

	fid = fopen(filename, "wb");
	for(i=0;i<ByteH;i++)
	{
		for(j=0;j<ByteW;j++)
		{
			wdata = pIn[i*ExtByteW+j];
			fwrite(&wdata, sizeof(unsigned char), 1, fid);
		}	
	}
	fclose(fid);
	
}

int save_file_packet_image(unsigned char *pPacketImage, int InW, int InH, int ExtW, int ExtH, stDWT *dwt_reg)
{
	FILE *fid = NULL;
	int size;
	int wsize = 0;
	unsigned char *pIn;
	int w, h, pitch, output_h;
	int i;
	char YL_name[OutputLayerNum][256] = { "YLL1.raw" , "YLL2.raw" , "YLL3.raw" , "YLL4.raw" };
	char UVL_name[OutputLayerNum][256] = { "UVLL1.raw" , "UVLL2.raw" , "UVLL3.raw" , "UVLL4.raw" };
	

	/////Y//////
	wsize = InW * InH;//YLL0  8bit
	w = ExtW >> 1;
	h = ExtH >> 1;

	for (i=Layer0;i<=Layer3;i++) {
		pIn = pPacketImage + wsize;
		if (dwt_reg->Output_ExtH_En) {
			output_h = h;
		} else {
			output_h = dwt_reg->y_height[i];
		}
		save_packet_img(&YL_name[i][0], pIn, dwt_reg->m_nYPitch[i], output_h, (w * 5 / 4));
		size = w * h * 5 / 4;
		wsize += size;
		w = w >> 1;
		h = h >> 1;
	}


	/////UV//////
	size = InW * (InH >> 1);//UVLL0  8bit
	wsize += size;
	w = ExtW >> 1;
	h = ExtH >> 2;

	for (i=Layer0;i<=Layer3;i++) {
		pIn = pPacketImage + wsize;
		if (dwt_reg->Output_ExtH_En) {
			output_h = h;
		} else {
			output_h = dwt_reg->uv_height[i];
		}
		save_packet_img(&UVL_name[i][0], pIn, dwt_reg->m_nUVPitch[i], output_h, (w * 5 / 4));
		size = w * h * 5 / 4;
		wsize += size;
		w = w >> 1;
		h = h >> 1;
	}


	return wsize;
}


/*
 * int LoadRegFromFile(char *filename, stDWT *dwt_reg,
 *         int *yl1_width, int *yl2_width, int *yl3_width, int *yl4_width, int *yl1_height, int *yl2_height, int *yl3_height, int *yl4_height,
 *         int *uvl1_width, int *uvl2_width, int *uvl3_width, int *uvl4_width, int *uvl1_height, int *uvl2_height, int *uvl3_height, int *uvl4_height)
 */
int LoadRegFromFile(stDWT *dwt_reg)
{
	int yl1_width,  yl2_width,  yl3_width,  yl4_width,  yl1_height,  yl2_height,  yl3_height,  yl4_height, uvl1_width,  uvl2_width,  uvl3_width,  uvl4_width,  uvl1_height,  uvl2_height,  uvl3_height,  uvl4_height;
	int *dataptr;
	int i;
	int w,h;
	int ExtW;
	int ew;

	/*
	 * FILE *fid = fopen(filename, "r");
	 * if (fid)
	 * {
	 *         for (i = 0; i < sizeof(struct tag_DWT) / sizeof(int); i++) {
	 *                 dataptr = (int *)(dwt_reg)+i;
	 *                 fscanf(fid, "%08x\n", dataptr);
	 *         }
	 *         fclose(fid);
	 * }
	 * else {
	 *         return 0;
	 * }
	 */

	ExtW = ((dwt_reg->m_nInputWidth + 63)>>6)<<6;

	///////Y
	w = (dwt_reg->m_nInputWidth + 1)>>1;
	h = (dwt_reg->m_nInputHeight + 1)>>1;
	ew = ExtW >> 1;

	//dwt_reg->Output_ExtH_En = 1;

	for(i=0;i<OutputLayerNum;i++)
	{
		dwt_reg->y_width[i] = w;
		dwt_reg->y_height[i] = h;

		if (dwt_reg->Output_ExtH_En) {
			dwt_reg->m_nYPitch[i] = (ew * 10 + 7) / 8;
		} else {
			dwt_reg->m_nYPitch[i] = (w * 10 + 7) / 8;
		}
		
		w = (w+1)>>1;
		h = (h+1)>>1;
		ew = ew >> 1;
	}


	////////UV
	w = ((dwt_reg->m_nInputWidth + 3) >> 2) << 1;
	h = (dwt_reg->m_nInputHeight + 3) >> 2;
	ew = ExtW >> 1;
	
	for(i=0;i<OutputLayerNum;i++)
	{
		dwt_reg->uv_width[i] = w;
		dwt_reg->uv_height[i] = h;

		if (dwt_reg->Output_ExtH_En) {
			dwt_reg->m_nUVPitch[i] = (ew * 10 + 7) / 8;
		} else {
			dwt_reg->m_nUVPitch[i] = (w * 10 + 7) / 8;
		}
		
		w = ((w+3) >> 2) << 1;
		h = (h+1)>>1;
		ew = ew >> 1;
	}


	//////save register
    /*
     *     fid = fopen(filename, "wb");
     * if (NULL != fid) {
     *     for (i = 0; i < sizeof(struct tag_DWT) / sizeof(int); i++) {
     *                     dataptr = (int *)(dwt_reg)+i;
     *                     fprintf(fid, "%08x\n", (*dataptr));
     *             }
     *     fclose(fid);
     * }
     */


	yl1_width = (dwt_reg->y_width[Layer0] * 10 + 7) / 8;
	yl2_width = (dwt_reg->y_width[Layer1] * 10 + 7) / 8;
	yl3_width = (dwt_reg->y_width[Layer2] * 10 + 7) / 8;
	yl4_width = (dwt_reg->y_width[Layer3] * 10 + 7) / 8;
	yl1_height = dwt_reg->y_height[Layer0];
	yl2_height = dwt_reg->y_height[Layer1];
	yl3_height = dwt_reg->y_height[Layer2];
	yl4_height = dwt_reg->y_height[Layer3];
	uvl1_width = (dwt_reg->uv_width[Layer0] * 10 + 7) / 8;
	uvl2_width = (dwt_reg->uv_width[Layer1] * 10 + 7) / 8;
	uvl3_width = (dwt_reg->uv_width[Layer2] * 10 + 7) / 8;
	uvl4_width = (dwt_reg->uv_width[Layer3] * 10 + 7) / 8;
	uvl1_height = dwt_reg->uv_height[Layer0];
	uvl2_height = dwt_reg->uv_height[Layer1];
	uvl3_height = dwt_reg->uv_height[Layer2];
	uvl4_height = dwt_reg->uv_height[Layer3];

	printf(" yl1= %dx%d yl2= %dx%d yl3= %dx%d yl4= %dx%d\n""uvl1= %dx%d uvl2= %dx%d uvl3= %dx%d uvl4= %dx%d\n",
	yl1_width, yl1_height, yl2_width, yl2_height, yl3_width, yl3_height, yl4_width, yl4_height,
	uvl1_width, uvl1_height, uvl2_width, uvl2_height, uvl3_width, uvl3_height, uvl4_width, uvl4_height);

	return 1;
}

void save_unpacket_img(FILE *fid, unsigned short*pIn, int width, int height, int ExtW)
{
	int i,j;

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			fprintf(fid, "%x\n",pIn[i*ExtW+j]);
			//fwrite(&pIn[i*ExtW+j], sizeof(unsigned short),1,fid);
		}	
	}
	
}

/*
 * int dwt_main(int argc, char **argv,
 *         int *yl1_width, int *yl2_width, int *yl3_width, int *yl4_width, int *yl1_height, int *yl2_height, int *yl3_height, int *yl4_height,
 *         int *uvl1_width, int *uvl2_width, int *uvl3_width, int *uvl4_width, int *uvl1_height, int *uvl2_height, int *uvl3_height, int *uvl4_height)
 */
int dwt_main(unsigned char *yaddr, unsigned char *caddr, int inWidth, int inHeight,
		unsigned char *yll1, unsigned char *uvll1,
		unsigned char *yll2, unsigned char *uvll2,
		unsigned char *yll3, unsigned char *uvll3,
		unsigned char *yll4, unsigned char *uvll4)
{
	int InSize, ExtSize, PacketSize;
	int InW, InH, ExtW, ExtH;
	unsigned short *Output_Y[OutputLayerNum] = {NULL};
	unsigned short *Linebuff_Y[OutputLayerNum] = { NULL };
	unsigned short *Output_UV[OutputLayerNum] = { NULL };
	unsigned short *Linebuff_UV[OutputLayerNum] = { NULL };
	unsigned char *Packed_Y[OutputLayerNum] = {NULL};
	unsigned char *Packed_UV[OutputLayerNum] = {NULL};
	unsigned short *pUV;
	int j, size;
	int i, k;
	int w, h;
	/*
	 * char ww[256];
	 * char hh[256];
	 * char delims[] = ".";
	 * char YL_name[OutputLayerNum][256] = { "_unpacketimage_YLL1.txt" , "_unpacketimage_YLL2.txt" , "_unpacketimage_YLL3.txt" , "_unpacketimage_YLL4.txt" };
	 * char UVL_name[OutputLayerNum][256] = { "_unpacketimage_UVLL1.txt" , "_unpacketimage_UVLL2.txt" , "_unpacketimage_UVLL3.txt" , "_unpacketimage_UVLL4.txt" };
	 * char pstr[256];
	 */
	stDWT dwt;


	// get parameter from argument instead of load from file
	dwt.m_nInputWidth = inWidth;
	dwt.m_nInputHeight = inHeight;
	if (LoadRegFromFile(&dwt))
		// yl1_width, yl2_width, yl3_width, yl4_width, yl1_height, yl2_height, yl3_height, yl4_height,
		// uvl1_width, uvl2_width, uvl3_width, uvl4_width, uvl1_height, uvl2_height, uvl3_height, uvl4_height))
	{
		InW = dwt.m_nInputWidth;
		InH = dwt.m_nInputHeight;
		ExtW = ((dwt.m_nInputWidth + 63)>>6)<<6;
		ExtH = ((dwt.m_nInputHeight + 31)>>5)<<5;
		InSize = InW * InH;

		/*read pIn yuv image*/
		/*
		 * unsigned char *pIn = (unsigned char*)malloc(sizeof(unsigned char) * InSize * 3 / 2);
		 * FILE *fid = fopen(argv[1], "rb");
		 * if (pIn && fid)
		 * {
		 *         fread(pIn, sizeof(unsigned char), InSize * 3 / 2, fid);
		 *         fclose(fid);
		 * }
		 * else {
		 *         fclose(fid);
		 * }
		 */

		/*Size Extender*/
		ExtSize = ExtW * ExtH;

		/*DWT_YProcess*/
		Output_Y[Layer0] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 4);
		Output_Y[Layer1] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 16);
		Output_Y[Layer2] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 64);
		Output_Y[Layer3] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 256);
		memset(Output_Y[Layer0], 0, sizeof(unsigned short) * ExtSize / 4);
		memset(Output_Y[Layer1], 0, sizeof(unsigned short) * ExtSize / 16);
		memset(Output_Y[Layer2], 0, sizeof(unsigned short) * ExtSize / 64);
		memset(Output_Y[Layer3], 0, sizeof(unsigned short) * ExtSize / 256);

		Linebuff_Y[Layer0] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 2);
		Linebuff_Y[Layer1] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 4);
		Linebuff_Y[Layer2] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 8);
		Linebuff_Y[Layer3] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 16);


		DWT_YProcess(yaddr, Output_Y, Linebuff_Y, &dwt, ExtW);

		/*Output Y unpacket image 1~4*/
/*
 *                 for (i = Layer0, (w = ExtW >> 1); i <= Layer3; i++) {
 *                         sprintf(ww, "%d", dwt.y_width[i]);
 *                         sprintf(hh, "%d", dwt.y_height[i]);
 *                         strcpy(pstr, ww);
 *                         strcat(pstr, "x");
 *                         strcat(pstr, hh);
 *                         strcat(pstr, &YL_name[i][0]);
 *                         fid = fopen(pstr, "wb");
 *                         if (fid)
 *                         {
 *                                 save_unpacket_img(fid, Output_Y[i], dwt.y_width[i], dwt.y_height[i], w);
 *                                 fclose(fid);
 *                         }
 *                         else {
 *                                 fclose(fid);
 *                         }
 *
 *                         w = w >> 1;
 *                         memset(pstr, 0, sizeof(pstr));
 *                 }
 */


		/*DWT_UVProcess*/
		Output_UV[Layer0] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 8);
		Output_UV[Layer1] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 32);
		Output_UV[Layer2] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 128);
		Output_UV[Layer3] = (unsigned short*)malloc(sizeof(unsigned short) * ExtSize / 512);
		
		for (i=Layer0,size = ExtSize / 8;i<=Layer3;i++) {
			pUV = Output_UV[i];
			for (j=0;j<size;j++)
			{
				pUV[j] = UVMEMVALUE;
			}
			size = size /4;
		}


		Linebuff_UV[Layer0] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 2);
		Linebuff_UV[Layer1] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 4);
		Linebuff_UV[Layer2] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 8);
		Linebuff_UV[Layer3] = (unsigned short*)malloc(sizeof(unsigned short) * ExtW / 16);


		DWT_UVProcess(caddr, Output_UV, Linebuff_UV, &dwt, ExtW);

#if 0
		/*Output UV unpacket image 1~4*/
		for (i = Layer0, (w = ExtW >> 1); i <= Layer3; i++) {
			sprintf(ww, "%d", dwt.uv_width[i]);
			sprintf(hh, "%d", dwt.uv_height[i]);
			strcpy(pstr, ww);
			strcat(pstr, "x");
			strcat(pstr, hh);
			strcat(pstr, &UVL_name[i][0]);
			fid = fopen(pstr, "wb");
			if (fid)
			{
				save_unpacket_img(fid, Output_UV[i], dwt.uv_width[i], dwt.uv_height[i], w);
				fclose(fid);
			}
			else {
				fclose(fid);
			}

			w = w >> 1;
			memset(pstr, 0, sizeof(pstr));
		}


		/*Output YUV packet image 0~4*/
		PacketSize = ExtW * ExtH / 256 * (64 + 16 + 4 + 1);
		PacketSize = PacketSize * 5 / 4;
		PacketSize += InW * InH;
		unsigned char *pPacketImage = (unsigned char *)malloc(sizeof(unsigned char) * PacketSize * 3 / 2);
#endif

		Packed_Y[Layer0] = yll1;
		Packed_Y[Layer1] = yll2;
		Packed_Y[Layer2] = yll3;
		Packed_Y[Layer3] = yll4;
		Packed_UV[Layer0] = uvll1;
		Packed_UV[Layer1] = uvll2;
		Packed_UV[Layer2] = uvll3;
		Packed_UV[Layer3] = uvll4;
		write_packet_image(InW, InH, ExtW, ExtH, Output_Y, Output_UV, Packed_Y, Packed_UV);
#if 0
		save_file_packet_image_LL0(pIn, InW, InH);
		save_file_packet_image(pPacketImage, InW, InH, ExtW, ExtH, &dwt);


		/*free*/
		free(pPacketImage);
		free(pIn);
#endif
		for (i = Layer0; i <= Layer3; i++) {
			free(Output_Y[i]);
			free(Linebuff_Y[i]);
			free(Output_UV[i]);
			free(Linebuff_UV[i]);
		}
	}

	return TRUE;
}
