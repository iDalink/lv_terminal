#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_IC_BACK
#define LV_ATTRIBUTE_IMG_IC_BACK
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IC_BACK uint8_t ic_back_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Alpha 8 bit, Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x2d, 0x25, 0xdf, 0x25, 0x4b, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x23, 0x25, 0xe6, 0x25, 0xff, 0x25, 0xf7, 0x25, 0x44,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x1a, 0x25, 0xdd, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0x96,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x12, 0x25, 0xd3, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xbe, 0x25, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x0c, 0x25, 0xc8, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xca, 0x25, 0x0d, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x07, 0x25, 0xbc, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xd5, 0x25, 0x14, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x04, 0x25, 0xae, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xdf, 0x25, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x25, 0x9f, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xe7, 0x25, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x8e, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xef, 0x25, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x25, 0x7d, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xf4, 0x25, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x25, 0x6f, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xf9, 0x25, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x25, 0x63, 0x25, 0xfe, 0x25, 0xff, 0x25, 0xff, 0x25, 0xfc, 0x25, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x25, 0x90, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xf2, 0x25, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x6d, 0x03, 0x25, 0xab, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xeb, 0x25, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x25, 0x06, 0x25, 0xb5, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xe6, 0x25, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x25, 0x09, 0x25, 0xbe, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xe0, 0x25, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x0c, 0x25, 0xc7, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xd9, 0x25, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x11, 0x25, 0xcf, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xd2, 0x25, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x16, 0x25, 0xd6, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xca, 0x25, 0x0e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x1b, 0x25, 0xdd, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xc2, 0x25, 0x0b, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x21, 0x25, 0xe3, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0xb9, 0x25, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x28, 0x25, 0xe9, 0x25, 0xff, 0x25, 0xff, 0x25, 0xff, 0x25, 0x95,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x2f, 0x25, 0xee, 0x25, 0xff, 0x25, 0xf8, 0x25, 0x47,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x37, 0x25, 0xe9, 0x25, 0x4f, 0x00, 0x00,
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x29, 0x2d, 0x27, 0x29, 0xdf, 0x27, 0x29, 0x4b, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x29, 0x23, 0x27, 0x29, 0xe6, 0x47, 0x29, 0xff, 0x47, 0x29, 0xf7, 0x27, 0x29, 0x44,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x29, 0x1a, 0x47, 0x29, 0xdd, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0x96,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x29, 0x12, 0x27, 0x29, 0xd3, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xbe, 0x08, 0x21, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x29, 0x0c, 0x47, 0x29, 0xc8, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xca, 0x47, 0x29, 0x0d, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x29, 0x07, 0x27, 0x29, 0xbc, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xd5, 0x46, 0x29, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x42, 0x04, 0x47, 0x29, 0xae, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xdf, 0x27, 0x29, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x47, 0x29, 0x9f, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xe7, 0x27, 0x29, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x29, 0x8e, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xef, 0x47, 0x29, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x29, 0x7d, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xf4, 0x47, 0x29, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x47, 0x29, 0x6f, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xf9, 0x47, 0x29, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x47, 0x29, 0x63, 0x47, 0x29, 0xfe, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xfc, 0x47, 0x29, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x27, 0x29, 0x90, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xf2, 0x47, 0x29, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xab, 0x5a, 0x03, 0x27, 0x29, 0xab, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xeb, 0x27, 0x29, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x65, 0x29, 0x06, 0x47, 0x29, 0xb5, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xe6, 0x27, 0x29, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0x20, 0x09, 0x47, 0x29, 0xbe, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xe0, 0x26, 0x29, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x29, 0x0c, 0x47, 0x29, 0xc7, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xd9, 0x67, 0x29, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x31, 0x11, 0x47, 0x29, 0xcf, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xd2, 0x47, 0x29, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x21, 0x16, 0x47, 0x29, 0xd6, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x27, 0x29, 0xca, 0x27, 0x29, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x29, 0x1b, 0x47, 0x29, 0xdd, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xc2, 0x86, 0x31, 0x0b, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x29, 0x21, 0x47, 0x29, 0xe3, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xb9, 0x25, 0x29, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x29, 0x28, 0x47, 0x29, 0xe9, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0xff, 0x47, 0x29, 0x95,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x29, 0x2f, 0x47, 0x29, 0xee, 0x47, 0x29, 0xff, 0x47, 0x29, 0xf8, 0x47, 0x29, 0x47,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x29, 0x37, 0x47, 0x29, 0xe9, 0x47, 0x29, 0x4f, 0x00, 0x00, 0x00,
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit  BUT the 2  color bytes are swapped*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x46, 0x2d, 0x29, 0x27, 0xdf, 0x29, 0x27, 0x4b, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x26, 0x23, 0x29, 0x27, 0xe6, 0x29, 0x47, 0xff, 0x29, 0x47, 0xf7, 0x29, 0x27, 0x44,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x46, 0x1a, 0x29, 0x47, 0xdd, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0x96,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x67, 0x12, 0x29, 0x27, 0xd3, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xbe, 0x21, 0x08, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x68, 0x0c, 0x29, 0x47, 0xc8, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xca, 0x29, 0x47, 0x0d, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x29, 0x07, 0x29, 0x27, 0xbc, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xd5, 0x29, 0x46, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x08, 0x04, 0x29, 0x47, 0xae, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xdf, 0x29, 0x27, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x29, 0x47, 0x9f, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xe7, 0x29, 0x27, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x47, 0x8e, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xef, 0x29, 0x47, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x47, 0x7d, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xf4, 0x29, 0x47, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x29, 0x47, 0x6f, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xf9, 0x29, 0x47, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x29, 0x47, 0x63, 0x29, 0x47, 0xfe, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xfc, 0x29, 0x47, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x29, 0x27, 0x90, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xf2, 0x29, 0x47, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x5a, 0xab, 0x03, 0x29, 0x27, 0xab, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xeb, 0x29, 0x27, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x29, 0x65, 0x06, 0x29, 0x47, 0xb5, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xe6, 0x29, 0x27, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xe7, 0x09, 0x29, 0x47, 0xbe, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xe0, 0x29, 0x26, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x68, 0x0c, 0x29, 0x47, 0xc7, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xd9, 0x29, 0x67, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x08, 0x11, 0x29, 0x47, 0xcf, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xd2, 0x29, 0x47, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x27, 0x16, 0x29, 0x47, 0xd6, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x27, 0xca, 0x29, 0x27, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x47, 0x1b, 0x29, 0x47, 0xdd, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xc2, 0x31, 0x86, 0x0b, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x47, 0x21, 0x29, 0x47, 0xe3, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xb9, 0x29, 0x25, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x46, 0x28, 0x29, 0x47, 0xe9, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0xff, 0x29, 0x47, 0x95,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x47, 0x2f, 0x29, 0x47, 0xee, 0x29, 0x47, 0xff, 0x29, 0x47, 0xf8, 0x29, 0x47, 0x47,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x27, 0x37, 0x29, 0x47, 0xe9, 0x29, 0x47, 0x4f, 0x00, 0x00, 0x00,
#endif
#if LV_COLOR_DEPTH == 32
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x28, 0x28, 0x2d, 0x35, 0x25, 0x26, 0xdf, 0x36, 0x25, 0x25, 0x4b, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x24, 0x24, 0x23, 0x36, 0x25, 0x26, 0xe6, 0x36, 0x26, 0x27, 0xff, 0x35, 0x26, 0x27, 0xf7, 0x34, 0x25, 0x25, 0x44,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x27, 0x27, 0x1a, 0x36, 0x26, 0x27, 0xdd, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x27, 0x96,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x2a, 0x2a, 0x12, 0x36, 0x25, 0x25, 0xd3, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x34, 0x26, 0x26, 0xbe, 0x40, 0x20, 0x20, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x2a, 0x2a, 0x0c, 0x36, 0x26, 0x28, 0xc8, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x27, 0xca, 0x3b, 0x27, 0x27, 0x0d, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x24, 0x24, 0x07, 0x36, 0x25, 0x27, 0xbc, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x35, 0x26, 0x28, 0xd5, 0x33, 0x26, 0x26, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x04, 0x36, 0x26, 0x28, 0xae, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x35, 0x25, 0x26, 0xdf, 0x37, 0x24, 0x24, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x37, 0x26, 0x26, 0x9f, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x26, 0xe7, 0x37, 0x22, 0x29, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x26, 0x28, 0x8e, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x26, 0xef, 0x36, 0x26, 0x26, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x27, 0x27, 0x7d, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x26, 0xf4, 0x34, 0x27, 0x27, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x35, 0x27, 0x27, 0x6f, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x26, 0xf9, 0x36, 0x28, 0x28, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x36, 0x27, 0x27, 0x63, 0x36, 0x26, 0x27, 0xfe, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x35, 0x26, 0x27, 0xfc, 0x36, 0x27, 0x27, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x35, 0x25, 0x25, 0x90, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x35, 0x25, 0x26, 0xf2, 0x37, 0x28, 0x28, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x55, 0x55, 0x55, 0x03, 0x36, 0x25, 0x25, 0xab, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x27, 0xeb, 0x35, 0x24, 0x2a, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x2a, 0x2a, 0x2a, 0x06, 0x36, 0x26, 0x27, 0xb5, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x26, 0xe6, 0x39, 0x23, 0x2a, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x1c, 0x1c, 0x09, 0x34, 0x26, 0x26, 0xbe, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x26, 0xe0, 0x33, 0x22, 0x2a, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x2a, 0x2a, 0x0c, 0x35, 0x26, 0x26, 0xc7, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x24, 0x26, 0xd9, 0x35, 0x2a, 0x2a, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1e, 0x2d, 0x11, 0x36, 0x26, 0x27, 0xcf, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x35, 0x24, 0x26, 0xd2, 0x36, 0x28, 0x28, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x23, 0x23, 0x16, 0x34, 0x26, 0x27, 0xd6, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x25, 0x27, 0xca, 0x37, 0x24, 0x24, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x26, 0x26, 0x1b, 0x36, 0x26, 0x27, 0xdd, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x35, 0x26, 0x27, 0xc2, 0x2e, 0x2e, 0x2e, 0x0b, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x27, 0x27, 0x21, 0x35, 0x26, 0x27, 0xe3, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x34, 0x27, 0x27, 0xb9, 0x24, 0x24, 0x24, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x26, 0x26, 0x28, 0x35, 0x26, 0x27, 0xe9, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xff, 0x37, 0x26, 0x27, 0x95,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x26, 0x26, 0x2f, 0x35, 0x26, 0x26, 0xee, 0x36, 0x26, 0x27, 0xff, 0x36, 0x26, 0x27, 0xf8, 0x36, 0x28, 0x28, 0x47,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x25, 0x25, 0x37, 0x35, 0x26, 0x27, 0xe9, 0x37, 0x27, 0x27, 0x4f, 0x00, 0x00, 0x00, 0x00,
#endif
};

const lv_img_dsc_t ic_back = {
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 14,
  .header.h = 24,
  .data_size = 336 * LV_IMG_PX_SIZE_ALPHA_BYTE,
  .data = ic_back_map,
};
