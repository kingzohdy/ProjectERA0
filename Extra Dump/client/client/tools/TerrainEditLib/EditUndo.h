
#pragma once
#include <vector>
#include <deque>
#include "TerrainEditTypes.h"
#include "TerrainBlock.h"

namespace TE
{
	struct UndoSaveModelData
	{
		Ogre::Vector3  savePos ;
		Ogre::Vector3  saveDir ;
		float  saveScale;
	};

	struct UndoSaveData
	{
		UndoSaveData();

		int           gx,gz;
		EDIT_EVENT    saveEditEvent;
		TerrBlockData    saveChunkData;
		TerrModel   *pSaveModel;
		Ogre::Vector3         savePos;
		Ogre::Vector3         saveDir;
		float         saveScale;
		std::vector<UndoSaveModelData>SaveSelectModelSet;
	};

	class UndoSaveDataGroup
	{
	public:
		UndoSaveDataGroup();
		~UndoSaveDataGroup();

		void Release();
		void AddChunkData(UndoSaveData* pData);

	public:
		std::vector<UndoSaveData *>m_SaveSet;
	};

	class UndoManager
	{
	public:
		UndoManager();
		~UndoManager();

		UndoSaveDataGroup *getCurSaveDataGroup();

		void AddSaveUndoData();
		void AddSaveRedoData();
		bool SaveCurData(int gx,int gz,int mode);

	public:
		int m_nCurSaveDataIndex;

	private:
		std::deque<UndoSaveDataGroup *>m_SaveData;
		std::deque<UndoSaveDataGroup *>m_SaveReDoData;
	};

	extern UndoManager g_UndoManager;
}
