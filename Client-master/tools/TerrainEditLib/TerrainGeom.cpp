
#include "TerrainGeom.h"

namespace TE
{
	int indexMapBuf(int x, int y)
	{
		return ((y+1)/2)*9 + (y/2)*8 + x;
	};
}