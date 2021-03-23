#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "finch_buffer_mgr.h"

#ifndef ALIGN_N
#define ALIGN_N(x, n)                               (((x) + (n) - 1) & (~((n) - 1)))
#endif /* ALIGN_N */

#define PAGE_SIZE                   (4096)

typedef enum CAM_PIXEL_FORMAT {
    CAM_FORMAT_DWT1,
    CAM_FORMAT_DWT2,
    CAM_FORMAT_DWT3,
    CAM_FORMAT_DWT4,
} CAM_PIXEL_FORMAT_E;

static void getPlanes(ASR_SIZE_S size, int32_t format, CAM_BUFFER_PLANE_S *planes, uint32_t *numPlanes)
{
    switch(format) {
        /* case PIXEL_FORMAT_YUV_SEMIPLANAR_420: */
        /* case PIXEL_FORMAT_YVU_SEMIPLANAR_420: */
        /*     *numPlanes = 2; */
        /*     planes[0].width = size.width; */
        /*     planes[0].height = size.height; */
        /*     planes[0].stride = ALIGN_N(planes[0].width, 32); */
        /*     planes[0].length = planes[0].stride * planes[0].height; */
        /*     planes[0].phyAddr = 0; */
        /*     planes[0].virAddr = 0; */
        /*     planes[1].width = size.width; */
        /*     planes[1].height = size.height / 2; */
        /*     planes[1].stride = ALIGN_N(planes[1].width, 32); */
        /*     planes[1].length = planes[1].stride * planes[1].height; */
        /*     planes[1].phyAddr = 0; */
        /*     planes[1].virAddr = 0; */
        /*     break; */
        /* case PIXEL_FORMAT_FBC: */
        /*     *numPlanes = 2; */
        /*     planes[0].width = ((size.width + 31) / 32) * ((size.height + 3) / 4) * 8; */
        /*     planes[0].height = 1; */
        /*     planes[0].stride = planes[0].width; */
        /*     planes[0].length = planes[0].stride * planes[0].height; */
        /*     planes[0].phyAddr = 0; */
        /*     planes[0].virAddr = 0; */
        /*     planes[1].width = ((size.width + 31) / 32) * ((size.height + 3) / 4) * 192; */
        /*     planes[1].height = 1; */
        /*     planes[1].stride = planes[1].width; */
        /*     planes[1].length = planes[1].stride * planes[1].height; */
        /*     planes[1].phyAddr = 0; */
        /*     planes[1].virAddr = 0; */
        /*     break; */
        /* case CAM_FORMAT_HIST: */
        /*     *numPlanes = 1; */
        /*     planes[0].width = 1024; */
        /*     planes[0].height = 1; */
        /*     planes[0].stride = planes[0].width; */
        /*     planes[0].length = planes[0].stride * planes[0].height; */
        /*     planes[0].phyAddr = 0; */
        /*     planes[0].virAddr = 0; */
        /*     break; */
        case CAM_FORMAT_DWT1:
            *numPlanes = 2;
            planes[0].width = ((ALIGN_N(size.width, 64) / 2 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 2);
            planes[0].height = 1;
            planes[0].stride = planes[0].width;
            planes[0].length = ALIGN_N(planes[0].stride * planes[0].height, PAGE_SIZE);
            planes[0].phyAddr = 0;
            planes[0].virAddr = 0;
            planes[1].width = ((ALIGN_N(size.width, 64) / 2 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 2) / 2;
            planes[1].height = 1;
            planes[1].stride = planes[1].width;
            planes[1].length = ALIGN_N(planes[1].stride * planes[1].height, PAGE_SIZE);
            planes[1].phyAddr = 0;
            planes[1].virAddr = 0;
            break;
        case CAM_FORMAT_DWT2:
            *numPlanes = 2;
            planes[0].width = ((ALIGN_N(size.width, 64) / 4 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 4);
            planes[0].height = 1;
            planes[0].stride = planes[0].width;
            planes[0].length = ALIGN_N(planes[0].stride * planes[0].height, PAGE_SIZE);
            planes[0].phyAddr = 0;
            planes[0].virAddr = 0;
            planes[1].width = ((ALIGN_N(size.width, 64) / 4 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 4) / 2;
            planes[1].height = 1;
            planes[1].stride = planes[1].width;
            planes[1].length = ALIGN_N(planes[1].stride * planes[1].height, PAGE_SIZE);
            planes[1].phyAddr = 0;
            planes[1].virAddr = 0;
            break;
        case CAM_FORMAT_DWT3:
            *numPlanes = 2;
            planes[0].width = ((ALIGN_N(size.width, 64) / 8 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 8);
            planes[0].height = 1;
            planes[0].stride = planes[0].width;
            planes[0].length = ALIGN_N(planes[0].stride * planes[0].height, PAGE_SIZE);
            planes[0].phyAddr = 0;
            planes[0].virAddr = 0;
            planes[1].width = ((ALIGN_N(size.width, 64) / 8 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 8) / 2;
            planes[1].height = 1;
            planes[1].stride = planes[1].width;
            planes[1].length = ALIGN_N(planes[1].stride * planes[1].height, PAGE_SIZE);
            planes[1].phyAddr = 0;
            planes[1].virAddr = 0;
            break;
        case CAM_FORMAT_DWT4:
            *numPlanes = 2;
            planes[0].width = ((ALIGN_N(size.width, 64) / 16 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 16);
            planes[0].height = 1;
            planes[0].stride = planes[0].width;
            planes[0].length = ALIGN_N(planes[0].stride * planes[0].height, PAGE_SIZE);
            planes[0].phyAddr = 0;
            planes[0].virAddr = 0;
            planes[1].width = ((ALIGN_N(size.width, 64) / 16 * 10 + 7) / 8) * (ALIGN_N(size.height, 32) / 16) / 2;
            planes[1].height = 1;
            planes[1].stride = planes[1].width;
            planes[1].length = ALIGN_N(planes[1].stride * planes[1].height, PAGE_SIZE);
            planes[1].phyAddr = 0;
            planes[1].virAddr = 0;
            break;
        /* case CAM_FORMAT_FRAMEINFO: */
        /*     *numPlanes = 1; */
        /*     planes[0].width = sizeof(CAM_FRAMEINFO_S); */
        /*     planes[0].height = 1; */
        /*     planes[0].stride = planes[0].width; */
        /*     planes[0].length = planes[0].stride * planes[0].height; */
        /*     planes[0].phyAddr = 0; */
        /*     planes[0].virAddr = 0; */
        /*     break; */
        /* case PIXEL_FORMAT_RGB_BAYER_10BPP: */
        /*     *numPlanes = 1; */
        /*     planes[0].width = size.width; */
        /*     planes[0].height = size.height; */
        /*     planes[0].stride = (size.width / 12 + ((size.width % 12) ? 1 : 0)) << 4; */
        /*     planes[0].length = planes[0].stride * planes[0].height + VRF_TAIL_SIZE; */
        /*     break; */
        /* case PIXEL_FORMAT_RGB_BAYER_12BPP: */
        /*     *numPlanes = 1; */
        /*     planes[0].width = size.width; */
        /*     planes[0].height = size.height; */
        /*     planes[0].stride = (size.width / 10 + ((size.width % 10) ? 1 : 0)) << 4; */
	    /* planes[0].length = planes[0].stride * planes[0].height + VRF_TAIL_SIZE; */
	    /* break; */
        /* default: */
        /*     *numPlanes = 1; */
        /*     planes[0].width = size.width; */
        /*     planes[0].height = size.height; */
        /*     planes[0].stride = planes[0].width * 2; */
        /*     planes[0].length = ALIGN_N(planes[0].stride * planes[0].height, PAGE_SIZE); */
        /*     planes[0].phyAddr = 0; */
        /*     planes[0].virAddr = 0; */
        /*     break; */
    }
}

uint32_t getDwtSize(ASR_SIZE_S size, CAM_BUFFER_PLANE_S dwt1[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt2[DWT_MAX_PLANES],
                                CAM_BUFFER_PLANE_S dwt3[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt4[DWT_MAX_PLANES], uint32_t *numPlanes)
{
    uint32_t tmpNumPlanes = 0, bufferSize = 0;
    uint32_t i;

    getPlanes(size, CAM_FORMAT_DWT1, dwt1, &tmpNumPlanes);
    assert(tmpNumPlanes > 0 && tmpNumPlanes <= DWT_MAX_PLANES);
    for(i = 0; i < tmpNumPlanes; i++) {
        bufferSize += dwt1[i].length;
    }

    getPlanes(size, CAM_FORMAT_DWT2, dwt2, &tmpNumPlanes);
    assert(tmpNumPlanes > 0 && tmpNumPlanes <= DWT_MAX_PLANES);
    for(i = 0; i < tmpNumPlanes; i++) {
        bufferSize += dwt2[i].length;
    }

    getPlanes(size, CAM_FORMAT_DWT3, dwt3, &tmpNumPlanes);
    assert(tmpNumPlanes > 0 && tmpNumPlanes <= DWT_MAX_PLANES);
    for(i = 0; i < tmpNumPlanes; i++) {
        bufferSize += dwt3[i].length;
    }

    getPlanes(size, CAM_FORMAT_DWT4, dwt4, &tmpNumPlanes);
    assert(tmpNumPlanes > 0 && tmpNumPlanes <= DWT_MAX_PLANES);
    for(i = 0; i < tmpNumPlanes; i++) {
        bufferSize += dwt4[i].length;
    }

    if(numPlanes) {
        *numPlanes = tmpNumPlanes;
    }

    return bufferSize;
}

int32_t updateCamBufferDWTAddr(unsigned char *virAddr, CAM_BUFFER_PLANE_S dwt1[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt2[DWT_MAX_PLANES],
                                CAM_BUFFER_PLANE_S dwt3[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt4[DWT_MAX_PLANES])
{
    uint32_t i, offset;

    for(i = 0; i < DWT_MAX_PLANES; i++) {
        dwt1[i].virAddr = virAddr;
        offset = dwt1[i].length;
        virAddr += offset;
    }

    for(i = 0; i < DWT_MAX_PLANES; i++) {
        dwt2[i].virAddr = virAddr;
        offset = dwt2[i].length;
        virAddr += offset;
    }

    for(i = 0; i < DWT_MAX_PLANES; i++) {
        dwt3[i].virAddr = virAddr;
        offset = dwt3[i].length;
        virAddr += offset;
    }

    for(i = 0; i < DWT_MAX_PLANES; i++) {
        dwt4[i].virAddr = virAddr;
        offset = dwt4[i].length;
        virAddr += offset;
    }

    return 0;
}

