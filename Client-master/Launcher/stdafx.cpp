// stdafx.cpp : source file that includes just the standard includes
// Launcher.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void PreMultiplyRGBChannels(BYTE* pBits, int pitch, int width, int height)
{
	for( int y=0; y<height; y++ )
	{
		BYTE*	pPixel= pBits;

		for( int x=0; x<width; x++ )
		{
			pPixel[0] = pPixel[0]*pPixel[3]/255;
			pPixel[1] = pPixel[1]*pPixel[3]/255;
			pPixel[2] = pPixel[2]*pPixel[3]/255;

			pPixel += 4;
		}

		pBits += pitch;
	}
}

