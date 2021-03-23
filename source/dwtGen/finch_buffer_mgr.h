#ifndef __FINCH_BUFFER_MGR__
#define __FINCH_BUFFER_MGR__
#include <stdint.h>

#define DWT_MAX_PLANES              2

typedef struct asrSIZE_S
{
    uint32_t width;
    uint32_t height;
} ASR_SIZE_S;

/**
 * struct CAM_BUFFER_PLANE - plane info for multi-planar buffers
 * @length:		size of this plane (NOT the payload) in bytes
 */
typedef struct CAM_BUFFER_PLANE {
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t length;
    uint64_t phyAddr;
    unsigned char *virAddr;
} CAM_BUFFER_PLANE_S;

uint32_t getDwtSize(ASR_SIZE_S size, CAM_BUFFER_PLANE_S dwt1[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt2[DWT_MAX_PLANES],
                                CAM_BUFFER_PLANE_S dwt3[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt4[DWT_MAX_PLANES], uint32_t *numPlanes);

int32_t updateCamBufferDWTAddr(unsigned char *virAddr, CAM_BUFFER_PLANE_S dwt1[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt2[DWT_MAX_PLANES],
                                CAM_BUFFER_PLANE_S dwt3[DWT_MAX_PLANES], CAM_BUFFER_PLANE_S dwt4[DWT_MAX_PLANES]);
#endif /* ifndef __FINCH_BUFFER_MGR__ */
