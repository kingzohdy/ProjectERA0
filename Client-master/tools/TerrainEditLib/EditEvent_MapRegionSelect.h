
#pragma once


#include "EditEventHandler.h"


namespace TE
{

	class RegionSelectEditHandler : public BaseEditHandler
	{
	public:
		RegionSelectEditHandler(void);
		~RegionSelectEditHandler(void);


		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		bool    SelectArea( Ogre::Vector3 &dir , Ogre::Vector3 &pos);
	};

};