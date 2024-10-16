#pragma once

#include "EditEventHandler.h"

namespace TE
{

	enum BEACHEDIT_OP_TYPE
	{
		BEACH_OP_ADDPOINT,
		BEACH_OP_SELECT,
		BEACH_OP_TRANSLATE
	};

	class BeachEditHandler : public BaseEditHandler
	{
	public:
		BeachEditHandler ();
		~BeachEditHandler ();

		virtual bool handleEditEvent (TerrainEditMgr* pEditor , ActionEvent *event);
		virtual  void  Undo( TerrainEditMgr *pEditor );

		void SetEditOpType (BEACHEDIT_OP_TYPE type) { m_BeachEditOpType = type; }
		BEACHEDIT_OP_TYPE GetEditOpType () { return m_BeachEditOpType; }

	protected:
		bool AddPos (TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos);

		BEACHEDIT_OP_TYPE m_BeachEditOpType;
	};

}
