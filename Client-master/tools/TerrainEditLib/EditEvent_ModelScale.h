#ifdef USE_OLDEDIT

#pragma once


#include "EditEvent_ModelMove.h"

namespace TE
{

	class ModelScaleEditHandler : public ModelMoveEditHandler
	{
	public:
		ModelScaleEditHandler(void);
		virtual ~ModelScaleEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		void  ZoomSelectModel( TerrainEditMgr* pEditor , int  dz);

	};

};

#endif