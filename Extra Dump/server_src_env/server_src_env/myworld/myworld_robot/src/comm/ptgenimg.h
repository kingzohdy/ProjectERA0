
#ifndef PT_GENIMAGE_H_
#define PT_GENIMAGE_H_

#include "gd.h"

enum enImage
{
    IMAGE_WIDTH = 96,
    IMAGE_HEIGHT = 24,
    IMAGE_WIDTH_STD = 48,
    IMAGE_HEIGHT_STD = 16,
    IMAGE_CHAR_NUM = 16,
    CHAR_FONT_SIZE = 24,
    CHAR_FONT_SIZE_STD = 16,
};

#define PTG_MAX_CHAR_NUM	36

/* public */
int ptg_produce(char* code, int codelen, void* buf, int* len);
int ptg_produce_std(char* code, int codelen, void* buf, int* len);

/* internal */
void ptg_gen_rand_code(char* code, int codelen);
void ptg_fill_background(gdImagePtr im);
void ptg_set_style_color(gdImagePtr im);
int ptg_gen_image(char* code, int codelen, void* buf, int* len);
int ptg_gen_image_std(char* code, int codelen, char* fond, void* buf, int* len);
int ptg_gen_char_image(char one_char, char* font, gdImagePtr* pImPtr, int* piw, int* pih, int fstd);

#endif /* PT_GENIMAGE_H_ */
