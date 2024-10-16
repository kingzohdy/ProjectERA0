#ifdef USE_OLDEDIT

#pragma once
#include "EditEventHandler.h"

namespace TE
{
	class ModelSelectEditHandler : public BaseEditHandler
	{
	public:
		ModelSelectEditHandler(void);
		virtual ~ModelSelectEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
	protected:
		bool    AddSelectScreenObject( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode  );
		bool    SelectRangeObject(  TerrainEditMgr* pEditor ,int dx, int dy , int mode = 0);

		bool    DelSelectScreenObject( TerrainEditMgr* pEditor , Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode );
		bool    SelectScreenObject( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode = 0);
		
	};


};

#endif