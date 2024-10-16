#pragma once

#include "TerrainEditTypes.h"

namespace  TE
{
	class SlopeMark
	{
	public:
		SlopeMark(void);
		~SlopeMark(void);

		bool  Init();
		void  Release();

		void  Render();
		void  SetPos(Ogre::Vector3  posSrc,Ogre::Vector3 posDest , float size);

	public:
		Ogre::Vector3                      m_Pos[4];
	private:

	};
}
