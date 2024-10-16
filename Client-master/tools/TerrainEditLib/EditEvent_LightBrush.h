#pragma once


#include "EditEvent_ModelMove.h"

namespace TE
{

	class LightEditHandler	: public ModelMoveEditHandler
	{
	public:
		LightEditHandler(void);
		virtual ~LightEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

		virtual void  Undo( TerrainEditMgr* pEditor );

	private:
		bool SelectScreenLight( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode );	
		void  AddLight( TerrainEditMgr* pEditor ,Ogre::Vector3 &pos , Ogre::Vector3 &dir);
	};


}