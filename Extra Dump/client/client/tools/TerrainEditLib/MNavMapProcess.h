#pragma once


#include "mterrainedit.h"

namespace TE
{

	class MNavigationMap;

	class MNavMapProcess
	{
	public:
		MNavMapProcess(void);
		~MNavMapProcess(void);

		void calNaviMapData( MTerrainEdit* pEditor, int nmask_block, float height);
		bool saveNaviDataToMaskMap(MNavigationMap &destmap, MTerrainEdit* pEditor, int nmask_block);
		bool isPointWalkable(MTerrainEdit* pEditor, float x, float z);
	private:
		unsigned char GetMapSign(float x,float z,float sx,float sz,float height);
		void          SetModelSign( Ogre::MovableObject *pobj ,float height , float basex,float basez, unsigned char *pdestdata, int ngridx, int ngridz, int gridscalex, int gridscalez);
	};

};

extern TE::MNavMapProcess   g_NavMapProcess;