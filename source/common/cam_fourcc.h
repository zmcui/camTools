#ifndef CAM_FOURCC_H
#define CAM_FOURCC_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define fourcc_code(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | \
				 ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

#if defined(__cplusplus)
}
#endif

#define CAM_PIX_FMT_P010     fourcc_code('P', '0', '1', '0')

/* two planes -- one Y, one Cr + Cb interleaved  */
#define CAM_PIX_FMT_NV12     fourcc_code('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define CAM_PIX_FMT_NV21     fourcc_code('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */

#endif /* CAM_FOURCC_H */
