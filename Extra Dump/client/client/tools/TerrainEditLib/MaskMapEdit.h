#pragma once


#include "TerrainEdit.h"

namespace TE
{

	class EditorMaskMap;

	class MaskMapEditMgr
	{
	public:
		MaskMapEditMgr(void);
		~MaskMapEditMgr(void);

		void copyWalkMaskToFly(TerrainEditMgr* pEditor, int nmask_block);
		void calNaviMapData( TerrainEditMgr* pEditor, int nmask_block, float height);
		bool saveNaviDataToMaskMap(EditorMaskMap &destmap, TerrainEditMgr* pEditor, int nmask_block);
		bool loadMaskBMPMapToNaviData(EditorMaskMap &destmap, TerrainEditMgr* pEditor,int nmask_block);
		bool isPointWalkable(TerrainEditMgr* pEditor, float x, float z);
	private:
		unsigned char GetMapSign(float x,float z,float sx,float sz,float height);
		void          SetModelSign( Ogre::MovableObject *pobj ,float height , float basex,float basez, unsigned char *pdestdata, int ngridx, int ngridz, int gridscalex, int gridscalez);
	};

	extern MaskMapEditMgr   g_NavMapProcess;
}