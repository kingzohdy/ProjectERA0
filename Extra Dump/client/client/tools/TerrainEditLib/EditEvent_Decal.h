#pragma once


#include "EditEventHandler.h"

namespace TE
{

	class DecalEditHandler : public BaseEditHandler
	{
	public:
		DecalEditHandler(void);
		~DecalEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		bool      AddDecalPos(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir , Ogre::Vector3 &pos  );

	};

};
