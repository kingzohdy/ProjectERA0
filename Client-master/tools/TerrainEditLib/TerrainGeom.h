
#pragma once

namespace TE
{
	extern int indexMapBuf(int x, int y);
	const int StripSize[2] = {158,318};

	//生成一级索引序列
	template <class V>
	void stripify(V *in, V *out)
	{
		for (int row=0; row<8; row++) 
		{
			V *thisrow = &in[indexMapBuf(0,row*2)];
			V *nextrow = &in[indexMapBuf(0,(row+1)*2)];

			if (row>0) *out++ = thisrow[0];
			for (int col=0; col<9; col++)
			{
				*out++ = thisrow[col];
				*out++ = nextrow[col];
			}
			if (row<7) *out++ = nextrow[8];
		}
	};

	template <class V>
	void stripify2(V *in, V *out)
	{
		for (int row=0; row<8; row++)
		{ 
			V *thisrow = &in[indexMapBuf(0,row*2)];
			V *nextrow = &in[indexMapBuf(0,row*2+1)];
			V *overrow = &in[indexMapBuf(0,(row+1)*2)];

			if (row>0) *out++ = thisrow[0];
			int col ;
			for (col=0; col<8; col++) 
			{
				*out++ = thisrow[col];
				*out++ = nextrow[col];
			}
			*out++ = thisrow[8];
			*out++ = overrow[8];
			*out++ = overrow[8];
			*out++ = thisrow[0];
			*out++ = thisrow[0];
			for (col=0; col<8; col++)
			{
				*out++ = overrow[col];
				*out++ = nextrow[col];
			}
			if (row<8) *out++ = overrow[8];
			if (row<7) *out++ = overrow[8];
		}
	};

}