
#include "EditEvent_ObjectBrush.h"
#include "MapNpc.h"
#include "Terrain.h"
#include "EditorCamera.h"
#include "Coordinate.h"
#include "MapLights.h"
#include "GameDBData.h"
#include "MapMark.h"
#include "EditorInput.h"
#include "EditorModel.h"
#include "EditorMessageQue.h"
#include "EditUndo.h"
#include "OgrePhysicsScene.h"

namespace TE
{
	extern  float g_fModelSignRad;

	ObjectBrushEditHandler::ObjectBrushEditHandler(void)
	{
		m_nProcessType = EDITEVENT_OBJECTBRUSH;
		m_ObjectType = EDITOBJ_MODEL;
		m_EditOP = OBJEDIT_OP_ADD;
	}

	ObjectBrushEditHandler::~ObjectBrushEditHandler(void)
	{
	}

	bool ObjectBrushEditHandler::handleEditEvent(TerrainEditMgr* peditor, ActionEvent *event)
	{
		if(peditor->m_bLockModelEdit[peditor->m_EditOption.mModelType])
		{
			return true;
		}

		Ogre::Vector3 pos;
		switch(event->mouseEven)
		{
		case MouseLDown:
			if(m_EditOP == OBJEDIT_OP_ADD)
			{
				if(g_Terrain.TestAndRay(event->dir, event->pos, pos))
				{
					float fdist = 0.0f;
					Ogre::PhysicsScene *physicsScene = 0;
					physicsScene = g_Terrain.getPhysicsScene();
					if (physicsScene)
					{
						if (physicsScene->pick(Ogre::Ray(event->pos,event->dir), &fdist))
						{
							Ogre::Vector3 t = event->pos + event->dir*fdist;
							if(t.y > pos.y)
								pos = t;
						}
					}

					if(m_ObjectType == EDITOBJ_MODEL) addModel(peditor, pos);
					else if(m_ObjectType == EDITOBJ_LIGHT) addLight(peditor, pos);
					else if(m_ObjectType == EDITOBJ_SOUND) addSound(peditor, pos);
					else if(m_ObjectType == EDITOBJ_NPC) addNpc(peditor, pos);
					else
					{
						assert(0);
					}
					m_State = OBJEDIT_ADDING;
				}
			}
			else if(m_EditOP==OBJEDIT_OP_SELECT || peditor->m_select_coordinate==NO_AXES_SELECT&&(m_EditOP==OBJEDIT_OP_MOVE || m_EditOP==OBJEDIT_OP_ROTATE || m_EditOP==OBJEDIT_OP_SCALE))
			{
				OBJSELECT_MODE mode = OBJSELECT_NORMAL;
				if(m_ObjectType == EDITOBJ_MODEL)
				{
					if(g_MInputProcess.GetUserInput()->bLCtrl) mode = OBJSELECT_ADD;
					else if(g_MInputProcess.GetUserInput()->bLAlt) mode = OBJSELECT_DEL;
				}

				selectObject(peditor, event->dir, event->pos, mode);
				if(peditor->getSelectModel())
				{
					if(m_ObjectType==EDITOBJ_LIGHT || m_ObjectType==EDITOBJ_SOUND)
					{
						int index = g_LightManager.getIndexByModel(peditor->getSelectModel());
						assert(index >= 0);
						g_EditorMsgQue.postMessage(EDMSG_SELECTLIGHT, index);
					}
					else if(m_ObjectType == EDITOBJ_NPC)
					{
						int index = peditor->getNpcManager()->findNpcByModel(peditor->getSelectModel());
						assert(index >= 0);
						g_EditorMsgQue.postMessage(EDMSG_SELECTNPC, index);
					}
				}
			}

			if(m_EditOP==OBJEDIT_OP_MOVE || m_EditOP==OBJEDIT_OP_ROTATE || m_EditOP==OBJEDIT_OP_SCALE)
			{
				g_UndoManager.AddSaveUndoData();
				g_UndoManager.SaveCurData(0, 0, 0);
			}

			peditor->saveCoordinatePos(event->mx, event->my);
			break;

		case MouseMove:
			{
				if(m_State == OBJEDIT_ADDING)
				{
					peditor->MoveSelectModel(event->dir, event->pos);
					if(m_ObjectType==EDITOBJ_LIGHT || m_ObjectType==EDITOBJ_SOUND) g_LightManager.onModelMove(peditor->getSelectModel());
				}
				else if(m_EditOP==OBJEDIT_OP_MOVE || m_EditOP==OBJEDIT_OP_ROTATE || m_EditOP==OBJEDIT_OP_SCALE)
				{
					if(peditor->getSelectModel() && g_MInputProcess.isKeyDown(VK_LBUTTON))
					{
						if( peditor->m_select_coordinate == NO_AXES_SELECT )
						{
							peditor->MoveSelectModel(event->dir, event->pos);
						}
						else
						{
							if(m_EditOP == OBJEDIT_OP_MOVE) peditor->MoveMoveAxes(peditor->m_select_coordinate, event->mx, event->my, event->pos, event->dir);
							else if(m_EditOP == OBJEDIT_OP_ROTATE) peditor->rotateModelAround(peditor->m_select_coordinate, event->mx, event->my);
							else if(m_EditOP == OBJEDIT_OP_SCALE) peditor->scaleModel(event->mx, event->my);
						}

						if(m_ObjectType==EDITOBJ_LIGHT || m_ObjectType==EDITOBJ_SOUND) g_LightManager.onModelMove(peditor->getSelectModel());
					}
					else
					{
						peditor->m_select_coordinate = peditor->GetCoordinate()->intersect(event->dir , event->pos , 0);
					}
				}
				peditor->MoveSign(event->dir , event->pos);
			}
			break;
		case MouseLUp:
			if(m_State == OBJEDIT_ADDING)
			{
				m_State = OBJEDIT_NONE;
			}
			break;

		case MouseRDown:
			peditor->m_bOpenModelInfoDlg = true;
			break;

		case MouseSewheel:
			{		
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);	
			}
			break;
		}
		return true;
	}

	void ObjectBrushEditHandler::Undo(TerrainEditMgr* peditor)
	{
		UndoSaveDataGroup *pgroup = g_UndoManager.getCurSaveDataGroup();
		if(pgroup == NULL) return;

		for(size_t i=0; i<pgroup->m_SaveSet.size(); i++)
		{
			UndoSaveData *psavedata = pgroup->m_SaveSet[i];
			for(size_t imod=0; imod<psavedata->SaveSelectModelSet .size () ; imod++ )	
			{
				peditor->m_CurSelectModelSet[imod]->GetModelData()->pos = psavedata->SaveSelectModelSet[imod].savePos;
				peditor->m_CurSelectModelSet[imod]->GetModelData()->dir = psavedata->SaveSelectModelSet[imod].saveDir;
				peditor->m_CurSelectModelSet[imod]->GetModelData()->scale = psavedata->SaveSelectModelSet[imod].saveScale;
				peditor->m_CurSelectModelSet[imod]->ResetMat();
			}
			psavedata->SaveSelectModelSet.clear();

			if(psavedata->pSaveModel)
			{
				peditor->setSelectModel(psavedata->pSaveModel);

				peditor->getSelectModel()->GetModelData()->pos = psavedata->savePos;
				peditor->getSelectModel()->GetModelData()->dir  = psavedata->saveDir;
				peditor->getSelectModel()->GetModelData()->scale  = psavedata->saveScale;

				peditor->getSelectModel()->ResetMat();
				psavedata->pSaveModel = NULL;
			}
		}

		g_UndoManager.m_nCurSaveDataIndex--;
	}

	void ObjectBrushEditHandler::setEditObjectType(EDIT_OBJECT_TYPE objtype)
	{
		m_ObjectType = objtype;
	}

	void ObjectBrushEditHandler::setEditOP(OBJEDIT_OP_TYPE op)
	{
		m_EditOP = op;
	}

	void ObjectBrushEditHandler::deleteCurObject()
	{
		TerrModel *pmodel = g_TerrainEdit.getSelectModel();
		g_TerrainEdit.clearSelect();

		if(m_ObjectType==EDITOBJ_LIGHT || m_ObjectType==EDITOBJ_SOUND)
		{
			int index = g_LightManager.getIndexByModel(pmodel);
			if(index >= 0)
			{
				g_LightManager.deleteLight(index);
				g_EditorMsgQue.postMessage(EDMSG_DELETELIGHT, index);
			}
		}
		else if(m_ObjectType==EDITOBJ_NPC)
		{
			int index = g_TerrainEdit.getNpcManager()->findNpcByModel(pmodel);
			if(index >= 0)
			{
				g_TerrainEdit.getNpcManager()->DeleteIthNpc(index);
				g_EditorMsgQue.postMessage(EDMSG_DELETENPC, index);
			}
		}
		else
		{
			g_TerrainEdit.deleteModel(pmodel);
		}
	}

	void ObjectBrushEditHandler::copyCurObject()
	{

	}

	void ObjectBrushEditHandler::copyCurNPC(TerrainEditMgr* pEditor)
	{
		TerrModel *pmodel = g_TerrainEdit.getSelectModel();
		addNpc(pEditor, pmodel->GetModelData()->pos);
	}

	void ObjectBrushEditHandler::addModel(TerrainEditMgr *peditor, Ogre::Vector3 &pos)
	{
		if(peditor->m_SelResData.seltype!=SELECT_RES_MODEL || peditor->m_SelResData.respath.empty())
		{
			return;
		}

		int index = peditor->AddModelName(peditor->m_SelResData.respath);
		TerrModel *model = new TerrModel;
		TerrModelData *pmodeldata = model->GetModelData();

		pmodeldata->cType = peditor->m_EditOption.mModelType;
		strcpy(pmodeldata->name ,peditor->m_SelResData.respath.c_str());
		pmodeldata->modelID = index;
		pmodeldata->pos = pos;

		model->Init();
		model->ResetMat();
		model->AttachRoom();
		peditor->GetEditData()->modelData.modelSet.push_back(model);

		peditor->setSelectModel(model);



	}

	void ObjectBrushEditHandler::addLight(TerrainEditMgr *peditor, Ogre::Vector3 &pos)
	{
		LightInfo  lightInfo;
		lightInfo.nType = 0;
		lightInfo.fRange = 500.0f;
		lightInfo.pos = pos;
		lightInfo.color = Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0f);
		lightInfo.volume = 1.0f;
		lightInfo.time0 = 0.0f;
		lightInfo.time1 = 0.0f;
		lightInfo.fSoundFullRange = 0.0f;
		lightInfo.iIsLoop = true;

		LightNodeInfo nodeinfo = g_LightManager.AddLight(lightInfo);
		peditor->setSelectModel(nodeinfo.pModel);

		g_EditorMsgQue.postMessage(EDMSG_UPDATELIGHTS);
	}

	void ObjectBrushEditHandler::addSound(TerrainEditMgr *peditor, Ogre::Vector3 &pos)
	{
		LightInfo  lightInfo;
		lightInfo.nType = 1;
		lightInfo.fRange = 5000.0f;
		lightInfo.pos = pos;
		lightInfo.color = Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0f);
		lightInfo.volume = 1.0f;
		lightInfo.respath = "sounds\\default.wav";
		lightInfo.time0 = 0.0f;
		lightInfo.time1 = 0.0f;
		lightInfo.fSoundFullRange = 0.0f;
		lightInfo.iIsLoop = true;

		LightNodeInfo nodeinfo = g_LightManager.AddLight(lightInfo);
		peditor->setSelectModel(nodeinfo.pModel);

		g_EditorMsgQue.postMessage(EDMSG_UPDATELIGHTS);
	}

	void ObjectBrushEditHandler::addNpc(TerrainEditMgr *peditor, Ogre::Vector3 &pos)
	{
		if(peditor->m_EditOption.nCurSelectNpcID <= 0)
		{
			return;
		}

		const NPCDISPLAYINFO *pinfo = &DBDataManager::getSingleton().GetNpcDisplayInfo(peditor->m_EditOption.nCurSelectNpcID);
		MapNpc *pnpc = new MapNpc(pinfo->name, pinfo->model, pinfo->id, pos, Ogre::Vector3(0,0,0), "");
		peditor->getNpcManager()->AddNpc(pnpc);

		peditor->setSelectModel(pnpc->GetModel());

		g_EditorMsgQue.postMessage(EDMSG_UPDATENPC);
	}

	void ObjectBrushEditHandler::selectObject(TerrainEditMgr *peditor, Ogre::Vector3 &dir, Ogre::Vector3 &pos, OBJSELECT_MODE mode)
	{	
		Ogre::Ray ray(pos, dir);
		size_t i;
		TerrModel *pselected = NULL;

		if(m_ObjectType == EDITOBJ_MODEL)
		{
			std::vector<TerrModel *>&modelset = peditor->GetEditData()->modelData.modelSet;
			for(i=0; i<modelset.size(); i++)
			{
				TerrModelData *pmodeldata = modelset[i]->GetModelData();
				EditorModel *peditmodel = pmodeldata->pModel;
				if(peditmodel == NULL) continue;
				if(!CanEditModelType(pmodeldata->cType)) continue;
				if(peditor->m_bLockModelEdit[pmodeldata->cType]) continue; //此类型物件被锁定

				if(peditmodel->GetShowModel()->intersectRay(Ogre::INTERSECT_PICK, ray, NULL))
				{
					pselected = modelset[i];
					break;
				}
			}
		}
		else if(m_ObjectType==EDITOBJ_LIGHT || m_ObjectType==EDITOBJ_SOUND)
		{
			pselected = g_LightManager.intersectRay(ray);
		}
		else if(m_ObjectType == EDITOBJ_NPC)
		{
			pselected = peditor->getNpcManager()->intersectRay(ray);
		}

		peditor->setSelectModel(pselected, mode);
	}

	void  ObjectBrushEditHandler::addModel(TerrainEditMgr* pEditor, const char* filename, Ogre::Vector3 pos, Ogre::Vector3 dir, float fscale)
	{
		if( pEditor->GetEditData() == NULL )
		{
			return;
		}

		if( pEditor->m_SelResData.pmodel == NULL)
		{
			return;
		}

		int   index = pEditor->AddModelName(pEditor->m_SelResData.respath);

		TerrModel*  model = new TerrModel;
		strcpy(model->GetModelData ()->name , pEditor->m_SelResData.respath.c_str());
		model->GetModelData ()->modelID = index;
		model->Init ();
		model->GetModelData ()->pos= pos ;

		dir.x = (float)( rand() % 360 );

		if( pEditor->m_EditOption.curMinScale == pEditor->m_EditOption.curMaxScale )
		{
			fscale = 1.0f;
		}
		else
		{
			fscale =  ( pEditor->m_EditOption.curMinScale + rand()%( pEditor->m_EditOption.curMaxScale - pEditor->m_EditOption.curMinScale ) ) / 100.0f;
		}

		model->GetModelData ()->dir = dir;
		model->GetModelData ()->cType = pEditor->m_EditOption.mModelType ;
		model->GetModelData ()->scale = fscale;
		model->ResetMat();
		model->AttachRoom();

		pEditor->GetEditData()->modelData .modelSet .push_back ( model );

	}
}