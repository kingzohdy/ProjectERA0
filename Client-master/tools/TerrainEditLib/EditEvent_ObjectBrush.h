
#pragma once

#include "EditEventHandler.h"

namespace TE
{
	enum EDIT_OBJECT_TYPE
	{
		EDITOBJ_MODEL = 0,
		EDITOBJ_LIGHT,
		EDITOBJ_SOUND,
		EDITOBJ_NPC
	};

	enum OBJEDIT_OP_TYPE
	{
		OBJEDIT_OP_ADD = 0,
		OBJEDIT_OP_SELECT,
		OBJEDIT_OP_MOVE,
		OBJEDIT_OP_ROTATE,
		OBJEDIT_OP_SCALE
	};

	enum OBJEDIT_STATE
	{
		OBJEDIT_NONE = 0,
		OBJEDIT_ADDING, //按下鼠标左键， 增加物体还没松开
	};

	class ObjectBrushEditHandler : public BaseEditHandler
	{
	public:
		ObjectBrushEditHandler(void);
		virtual ~ObjectBrushEditHandler(void);
		virtual bool handleEditEvent(TerrainEditMgr* pEditor, ActionEvent *event);
		virtual void Undo( TerrainEditMgr *pEditor);

		void setEditObjectType(EDIT_OBJECT_TYPE objtype);
		void setEditOP(OBJEDIT_OP_TYPE op);
		void deleteCurObject();
		void copyCurObject();
		EDIT_OBJECT_TYPE getEditObjType(){return m_ObjectType;}
		OBJEDIT_OP_TYPE getEditOP(){return m_EditOP;}
		void addModel(TerrainEditMgr* pEditor, const char* filename, Ogre::Vector3 pos, Ogre::Vector3 dir, float fscale);
		void copyCurNPC(TerrainEditMgr* pEditor);

	protected:
		void addModel(TerrainEditMgr *peditor, Ogre::Vector3 &pos);
		void addLight(TerrainEditMgr *peditor, Ogre::Vector3 &pos);
		void addSound(TerrainEditMgr *peditor, Ogre::Vector3 &pos);
		void addNpc(TerrainEditMgr *peditor, Ogre::Vector3 &pos);
		void selectObject(TerrainEditMgr *peditor, Ogre::Vector3 &dir, Ogre::Vector3 &pos, OBJSELECT_MODE mode);

	protected:
		EDIT_OBJECT_TYPE m_ObjectType;
		OBJEDIT_OP_TYPE m_EditOP;
		OBJEDIT_STATE m_State;
	};


}