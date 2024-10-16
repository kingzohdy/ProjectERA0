#ifdef USE_OLDEDIT

#pragma once


#include "EditEvent_ModelMove.h"


namespace TE
{
	class ModelRotateEditHandler : public ModelMoveEditHandler
	{
	public:
		ModelRotateEditHandler(void);
		virtual ~ModelRotateEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
	protected:
		void rotateSelectModel(TerrainEditMgr* pEditor, int dx, int dy, COORDINATE_SELECT axis);
		void  RoateYSelectModel ( TerrainEditMgr* pEditor ,int dz );
	};

};

#endif