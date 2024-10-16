#pragma once


#include "EditEventHandler.h"

namespace TE
{

	class PathEditHandler : public BaseEditHandler
	{
	public:
		PathEditHandler(void);
		virtual ~PathEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
		virtual void UpdateData( TerrainEditMgr* pEditor );

	protected:
		bool AddPathPos(  TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos  );
		bool MovePathPos( TerrainEditMgr* pEditor ,  Ogre::Vector3 &dir , Ogre::Vector3 &pos );

		bool TestPathModel( TerrainEditMgr* pEditor ,  Ogre::Vector3 &dir , Ogre::Vector3 &pos );
	};

};
