
#pragma once
#include "EditEventHandler.h"

namespace TE
{
	class WaterBrushEditHandler : public BaseEditHandler
	{
	public:
		WaterBrushEditHandler(void);
		virtual ~WaterBrushEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		bool  AddLiquid( TerrainEditMgr* pEditor , Ogre::Vector3 &dir, Ogre::Vector3 &pos );
	};

};