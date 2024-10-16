
#include "ptgenimg.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PI 3.1415926

#define STD_FONT_PATH	"./fonts/std.ttf"

int ptg_produce(char* code, int codelen, void* buf, int* len)
{
    ptg_gen_rand_code(code, codelen);

    return ptg_gen_image(code, codelen, buf, len);
}

int ptg_produce_std(char* code, int codelen, void* buf, int* len)
{
    char font[256]=STD_FONT_PATH;

    return ptg_gen_image_std(code, codelen, font, buf, len);
}

int ptg_fill_noise_line(gdImagePtr im, int iWidth, int iHeight, int iLines, int iColor)
{
	int i;
	int x, y;
	int cx, cy;

	for(i=0; i<iLines; i++)
	{
		x	=	(int)(((rand()+i)/(double)RAND_MAX)*iWidth);
		y	=	rand()%iWidth + 1;
		
		cx = rand()%iWidth/4 + iWidth/4;
		cy = rand()%iHeight/4 + iWidth/4;

		gdImageLine(im, x, y, cx, cy, iColor);  
	}

	return 0;
}

int ptg_fill_noise_pixel(gdImagePtr im, int iWidth, int iHeight, int iPixels, int iColor)
{
	int i;
	int x, y;
	int cx, cy;

	for (i=0; i<iPixels; i++)
	{
		x	=	(int)(((rand()+i)/(double)RAND_MAX)*iWidth);
		y	=	(int)(((rand()+i)/(double)RAND_MAX)*iHeight);

		gdImageSetPixel(im, x, y, iColor);
		gdImageSetPixel(im, x+1, y, iColor);
		gdImageSetPixel(im, x, y+1, iColor);
		gdImageSetPixel(im, x+1, y+1, iColor);
	}

	return 0;
}

int ptg_gen_char_image(char one_char, char* font, gdImagePtr* pImPtr, int* piw, int* pih, int fstd)
{
    int brect[8];
    double size = CHAR_FONT_SIZE;
    char str[2] = {'\0', '\0'};
    double angle = (rand() % 100) * (PI / 36) / 100 + PI / 12;
	char* err;
    int x, y;
    int white;
    int black;

	if( fstd )
	{
		angle	= 0;
    	size	= CHAR_FONT_SIZE_STD;
	}
	else
	{
    	angle *= rand() % 2 == 0 ? -1 : 1;
    	size	= CHAR_FONT_SIZE;
	}

    str[0] = one_char;

    err = gdImageStringFT(NULL, &brect[0], 0, font, size, angle, 0, 0, str);
    if (err)
    	return -1;

    if (angle < 0)
    {
        *piw = brect[4] - brect[0];
        *pih = brect[3] - brect[7];
        x = 0;
        y = brect[1] - brect[7];
    }
    else
    {
        *piw = brect[2] - brect[6];
        *pih = brect[1] - brect[5];
        x = brect[0] - brect[6];
        y = *pih;
    }

    *pImPtr	= gdImageCreate(*piw, *pih);

	if( !(*pImPtr) )
		return -1;

    white	= gdImageColorResolve(*pImPtr, 255, 255, 255);
    black	= gdImageColorResolve(*pImPtr, 0, 0, 0);

    gdImageStringFT(*pImPtr, brect, black, font, size, angle, x, y, str);

	if( !fstd )
	{
		ptg_fill_noise_pixel(*pImPtr, *piw, *pih, 2, black);
		ptg_fill_noise_line(*pImPtr, *piw, *pih, 1, black);
	}

	return 0;
}

int ptg_gen_image(char* code, int codelen, void* buf, int* len)
{
    char font[256];
    gdImagePtr im_char[IMAGE_CHAR_NUM];
    int w[IMAGE_CHAR_NUM];
    int h[IMAGE_CHAR_NUM];
    int width= 0;
    int x = 0;
    int wx = 0;
	int i,j;
    void* image_data;
	int iRet=0;
//    FILE* pngout;
    gdImagePtr im_image = gdImageCreate(IMAGE_WIDTH, IMAGE_HEIGHT);
    int white = gdImageColorResolve(im_image, 255, 255, 255);

	if( codelen>IMAGE_CHAR_NUM )
		codelen	=	IMAGE_CHAR_NUM;

    sprintf(font, "./fonts/%d.ttf", rand() % 16);

	memset(im_char, 0, sizeof(im_char));

    for (j = 0; j < codelen; j++)
    {
        ptg_gen_char_image(code[j], font, &im_char[j], &w[j], &h[j], 1);
        width += w[j];
    }

    for (i = 0; i < codelen; i++)
    {
		if( !im_char[i] )
		{
			iRet	=	-1;
			continue;
		}

        wx = ( (double)(w[i]) / (double)(width) * IMAGE_WIDTH) + 0.5;
        gdImageCopyResized(im_image, im_char[i],
            x, 0,
            0, 0,
            wx, IMAGE_HEIGHT,
            w[i] , h[i]);
        
        gdImageDestroy(im_char[i]);
        x += wx;
    }

	gdImageColorTransparent(im_image, white);
    
/*
    pngout = fopen("test.gif", "wb");

    gdImageGif(im_image, pngout);    

    fclose(pngout);
*/

    image_data = gdImageGifPtr(im_image, len);
    memcpy(buf, image_data, *len);
    gdFree(image_data);

    gdImageDestroy(im_image);

	return iRet;
}

int ptg_gen_image_std(char* code, int codelen, char* font, void* buf, int* len)
{
    gdImagePtr im_char[IMAGE_CHAR_NUM];
    int w[IMAGE_CHAR_NUM];
    int h[IMAGE_CHAR_NUM];
    int width= 0;
    int x = 0;
    int wx = 0;
	int i,j;
    void* image_data;
	int iRet=0;
//    FILE* pngout;
    gdImagePtr im_image = gdImageCreate(IMAGE_WIDTH_STD, IMAGE_HEIGHT_STD);
    int white = gdImageColorResolve(im_image, 255, 255, 255);

	if( codelen>IMAGE_CHAR_NUM )
		codelen	=	IMAGE_CHAR_NUM;

	memset(im_char, 0, sizeof(im_char));

    for (j = 0; j < codelen; j++)
    {
        ptg_gen_char_image(code[j], font, &im_char[j], &w[j], &h[j], 1);
        width += w[j];
    }

    for (i = 0; i < codelen; i++)
    {
		if( !im_char[i] )
		{
			iRet	=	-1;
			continue;
		}

        wx = ( (double)(w[i]) / (double)(width) * IMAGE_WIDTH_STD) + 0.5;
        gdImageCopyResized(im_image, im_char[i],
            x, 0,
            0, 0,
            wx, IMAGE_HEIGHT_STD,
            w[i], h[i]);
        
        gdImageDestroy(im_char[i]);
        x += wx;
    }

	gdImageColorTransparent(im_image, white);
    
/*
    pngout = fopen("test.gif", "wb");

    gdImageGif(im_image, pngout);    

    fclose(pngout);
*/

    image_data = gdImageGifPtr(im_image, len);
    memcpy(buf, image_data, *len);
    gdFree(image_data);

    gdImageDestroy(im_image);

	return iRet;
}

void ptg_gen_rand_code(char* code, int codelen)
{
	int m;
	char cBm[PTG_MAX_CHAR_NUM];
	char c;

	if( codelen>IMAGE_CHAR_NUM )
		codelen	=	IMAGE_CHAR_NUM;

	memset(cBm, 0, sizeof(cBm));

    for(m = 0; m < codelen; m ++)
    {
        c = rand() % PTG_MAX_CHAR_NUM;

		if( cBm[c] )
		{
			m--;
			continue;
		}

		cBm[c]	=	1;

        c = c < 10 ? c + 48 : c - 10 + 65;

        switch (c)
        {
        case '0':
        case 'O':
        case 'B':
        case '8':
        case 'C':
        case 'I':
        case '1':
        case 'Q':
        case '7':
        case 'J':
            m --;
            continue;
        default:
            break;
        }

        code[m] = c;
    }

	code[codelen]	=	'\0';
}

