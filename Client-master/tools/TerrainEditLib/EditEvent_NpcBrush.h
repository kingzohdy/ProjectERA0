#ifdef USE_OLDEDIT

#pragma once
#include "EditEvent_ModelMove.h"

namespace TE
{
	class NpcBrushEditHandler : public ModelMoveEditHandler
	{
	public:
		NpcBrushEditHandler(void);
		~NpcBrushEditHandler(void);

		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );

	protected:
		bool   SelectScreenNPC( TerrainEditMgr* pEditor , Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode);
		void   AddNpc( TerrainEditMgr* pEditor ,Ogre::Vector3 &pos , Ogre::Vector3 &dir);

	public:
		void    AddNPC( TerrainEditMgr* pEditor ,const char* npcName , const char* npcModelPath ,  unsigned int npcID,
			Ogre::Vector3 npcPos, Ogre::Vector3 npcDir ,const char* script );
	};

};
#endif