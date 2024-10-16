
#include "EditUndo.h"
#include "TerrainEdit.h"
#include "TerrainTile.h"
#include "EditEvent_ObjectBrush.h"
#include "EditEventHandlerMgr.h"

#include "OgreLog.h"

namespace TE
{
	UndoManager g_UndoManager;
	const int MAX_UNDOREDO_RECORD = 6;

	UndoSaveData::UndoSaveData()
	{
		gx = gz = 0;
		saveEditEvent = UNKOWN_ACTION;
		memset(&saveChunkData, 0, sizeof(saveChunkData));
		pSaveModel = NULL;
		savePos = saveDir = Ogre::Vector3(0,0,0);
		saveScale = 1.0f;
	}

	UndoSaveDataGroup::UndoSaveDataGroup()
	{
		m_SaveSet.clear();
	}

	UndoSaveDataGroup::~UndoSaveDataGroup()
	{
		Release();
	}

	void UndoSaveDataGroup::Release()
	{
		Ogre::DeletePointerArray(m_SaveSet);
		m_SaveSet.clear();
	}

	void UndoSaveDataGroup::AddChunkData(UndoSaveData* pData)
	{
		m_SaveSet.push_back ( pData );
	}

	UndoManager::UndoManager()
	{
		m_nCurSaveDataIndex = 0;
	}

	UndoManager::~UndoManager()
	{
	}

	UndoSaveDataGroup *UndoManager::getCurSaveDataGroup()
	{
		if(m_SaveData.empty()) return NULL;
		if(m_nCurSaveDataIndex < 0) m_nCurSaveDataIndex = 0;
		else if(m_nCurSaveDataIndex >= m_SaveData.size()) m_nCurSaveDataIndex = m_SaveData.size()-1;

		return m_SaveData[m_nCurSaveDataIndex];
	}

	//增加保存Undo数据
	void  UndoManager::AddSaveUndoData()
	{
		UndoSaveDataGroup *newdata = new UndoSaveDataGroup;
		size_t nSize = m_SaveData.size ();
		if(m_nCurSaveDataIndex +1 < (int)m_SaveData.size ())
		{
			for( size_t i = 0 ; i < nSize - m_nCurSaveDataIndex -1 ; i++)
			{
				m_SaveData.pop_back ();
			}
		}

		m_SaveData.push_back(newdata);
		if(m_SaveData.size () >= MAX_UNDOREDO_RECORD)
		{
			m_SaveData.pop_front ();
		}

		m_nCurSaveDataIndex = int(m_SaveData.size ())-1;
	}

	//增加数据以便做redo操作
	void  UndoManager::AddSaveRedoData()
	{
		if(m_SaveData.empty()) return;

		UndoSaveDataGroup *newdata = new UndoSaveDataGroup;

		size_t nSize = m_SaveReDoData.size();
		if(m_nCurSaveDataIndex  < (int)m_SaveReDoData.size())
		{
			for(size_t i=0; i<nSize-m_nCurSaveDataIndex; i++)
			{
				m_SaveReDoData.pop_back();
			}
		}

		m_SaveReDoData.push_back(newdata);

		if(m_SaveReDoData.size() >= MAX_UNDOREDO_RECORD)
		{
			m_SaveReDoData.pop_front();
		}

		if(m_nCurSaveDataIndex >= (int)m_SaveReDoData.size()) return;

		for(size_t i=0; i<m_SaveData[m_nCurSaveDataIndex]->m_SaveSet.size () ; i++)
		{
			SaveCurData( m_SaveData[m_nCurSaveDataIndex]->m_SaveSet[i]->gx , m_SaveData[m_nCurSaveDataIndex]->m_SaveSet [i]->gz , 1);
		}
	}

	bool UndoManager::SaveCurData(int gx,int gz,int mode)
	{
		if(m_nCurSaveDataIndex < 0) m_nCurSaveDataIndex = 0;
		if(m_nCurSaveDataIndex < 0 || m_nCurSaveDataIndex >= (int)m_SaveData.size()) return false; 

		UndoSaveDataGroup *pgroup = m_SaveData[m_nCurSaveDataIndex];
		UndoSaveDataGroup *predogroup = NULL;
		if(mode == 0)
		{
			for(size_t i=0; i<pgroup->m_SaveSet.size(); i++)
			{
				if(pgroup->m_SaveSet[i]->gx==gx && pgroup->m_SaveSet[i]->gz==gz) return true;
			}
		}
		else
		{
			if(m_nCurSaveDataIndex >= (int)m_SaveReDoData.size ()) return false;
			predogroup = m_SaveReDoData[m_nCurSaveDataIndex];
			for(size_t i=0; i<predogroup->m_SaveSet.size(); i++)
			{
				if(predogroup->m_SaveSet[i]->gx==gx && predogroup->m_SaveSet[i]->gz==gz) return true;
			}
		}

		UndoSaveData *saveData = new UndoSaveData;
		saveData->gx = gx;
		saveData->gz = gz;
		saveData->saveEditEvent = g_TerrainEdit.GetCurEditEvent();

		switch(g_TerrainEdit.GetCurEditEvent())
		{	
		case PANIT_EVN:
			memcpy(&saveData->saveChunkData, g_TerrainEdit.GetEditData()->chunkSet[gz][gx].GetChunkData(), sizeof(TerrBlockData));	
			break;

		case EDITEVENT_OBJECTBRUSH:
			{
				OBJEDIT_OP_TYPE op = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH))->getEditOP();
				if(op==OBJEDIT_OP_MOVE || op==OBJEDIT_OP_ROTATE || op==OBJEDIT_OP_SCALE)
				{
					TerrModel *pmodel = g_TerrainEdit.getSelectModel();
					if(pmodel)
					{
						saveData->pSaveModel = pmodel;
						saveData->savePos = pmodel->GetModelData ()->pos ;
						saveData->saveDir = pmodel->GetModelData ()->dir ;
						saveData->saveScale = pmodel->GetModelData ()->scale ;

						for(size_t i=0; i<g_TerrainEdit.m_CurSelectModelSet.size (); i++)
						{
							UndoSaveModelData saveInfo;
							saveInfo.savePos = g_TerrainEdit.m_CurSelectModelSet[i]->GetModelData ()->pos ;
							saveInfo.saveDir = g_TerrainEdit.m_CurSelectModelSet[i]->GetModelData ()->dir ;
							saveInfo.saveScale = g_TerrainEdit.m_CurSelectModelSet[i]->GetModelData ()->scale ;

							saveData->SaveSelectModelSet.push_back(saveInfo);
						}
					}
					else
					{
						saveData->pSaveModel = NULL;
					}
				}
			}
			break;

		case CHANGE_HEIGHT_EVN:
			memcpy(&saveData->saveChunkData , g_TerrainEdit.GetEditData()->chunkSet [gz][gx].GetChunkData (),sizeof(TerrBlockData));	
			break;

		case ADD_MORE_OBJECT_EVN:
			break;

		case MODIFY_LIQUID:
			break;

		case 	MODIFY_PATH:
			break;

		default:
			LOG_WARNING("SaveCurData(%d, %d, %d) 错误", gx, gz, mode);
			break;
		}

		if(mode == 0)
		{
			pgroup->m_SaveSet.push_back(saveData);
		}
		else
		{
			predogroup->m_SaveSet.push_back(saveData);
		}

		return false;
	}
}