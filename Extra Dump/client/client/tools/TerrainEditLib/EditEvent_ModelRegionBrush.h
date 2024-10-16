
#pragma once

#include "EditEvent_ObjectBrush.h"

namespace TE
{

	class ModelRegionBrushEditHandler : public ObjectBrushEditHandler
	{
	public:
		ModelRegionBrushEditHandler(void);
		virtual  ~ModelRegionBrushEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		void  RefreshMoreModel( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos,int mode = 0);
	};

};