
#pragma once
#include "EditEventHandler.h"

namespace TE
{
	class TexModelBrushEditHandler	: public BaseEditHandler
	{
	public:
		TexModelBrushEditHandler(void);
		~TexModelBrushEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

		virtual void Undo( TerrainEditMgr* pEditor );

	protected:
		void  SetAutoLinkMeshSign(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir ,Ogre::Vector3 &pos);
	};


};
