#pragma once

#include "EditEventHandler.h"

namespace TE
{

	class RegionEditHandler : public BaseEditHandler
	{
	public:
		RegionEditHandler(void);
		~RegionEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		bool  AddAreaPos(  Ogre::Vector3 &dir , Ogre::Vector3 &pos  );
		bool  MoveAreaPos( Ogre::Vector3 &dir , Ogre::Vector3 &pos );
		void  DelAreaCurPos();
	};

};