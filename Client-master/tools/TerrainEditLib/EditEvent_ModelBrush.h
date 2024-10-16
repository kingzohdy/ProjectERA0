#ifdef USE_OLDEDIT

#pragma once


#include "EditEvent_ModelMove.h"

namespace TE
{

	class ModelBrushEditHandler : public ModelMoveEditHandler
	{
	public:
		ModelBrushEditHandler(void);
		virtual ~ModelBrushEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	public:
		void    AddObjectTitle( TerrainEditMgr* pEditor );

		void    AddPlantModelToTitle( TerrainEditMgr* pEditor,const char* filename,Ogre::Vector3 pos,Ogre::Vector3 dir,float fscale);
	};


};

#endif