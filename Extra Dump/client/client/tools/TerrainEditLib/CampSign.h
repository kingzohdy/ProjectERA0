#pragma once

#include "TerrainEditTypes.h"

namespace  TE
{
	class SlopeSign
	{
	public:
		SlopeSign(void);
		~SlopeSign(void);

		bool  Init();
		void  Release();

		void  Render();
		void  SetPos(MVec3  posSrc,MVec3 posDest , float size);

	public:
		MVec3                      m_Pos[4];
	private:

	};
}
