#ifdef USE_OLDEDIT

#pragma once


#include "EditEvent_ModelSelect.h"


namespace TE
{

	class ModelMoveEditHandler : public ModelSelectEditHandler
	{
	public:
		ModelMoveEditHandler(void);
		virtual ~ModelMoveEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

		virtual void  Undo( TerrainEditMgr* pEditor );

	protected:
		bool   TestCurSelectModel( TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos  );
		void   MoveSelectModel(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos,int mode = 0);
	};

};
#endif