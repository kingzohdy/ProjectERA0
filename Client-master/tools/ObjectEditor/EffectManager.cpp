
#include "stdafx.h"
#include "EffectManager.h"
#include "NPCManager.h"
#include "AvatarShowInfo.h"
#include "DBManager.h"
#include "RefNodeManager.h"
#include "EffectUnit.h"
#include "BindInfo.h"
#include "ModelMotionInfo.h"
#include "ModelMotionInfo.h"
#include "EffectManager.h"
#include "ActorIO.h"
#include "EffectLayerInfo.h"
#include "EditorManager.h"
#include "EffectCommand.h"
#include "SkeletonInfo.h"
#include "AnchorSet.h"
#include "ObjectBounding.h"
#include "BSPInfo.h"
#include "InteractObject.h"

#include "AvatarManager.h"
#include "AvatarEntity.h"
#include "OgreEntity.h"
#include "OgreResourceManager.h"
#include "OgreEntityMotion.h"
#include "OgreEntityMotionEvent.h"

CEffectMgr*	CEffectMgr::ms_pInst = NULL;

extern Ogre::SceneRenderer *g_MainSceneRenderer;

CEffectMgr::CEffectMgr(void) : m_pModelActor(NULL), m_bPause(false), m_pModelTarget(NULL)
{
	m_iActive = -1;
	m_bShowMainActor = true;

	m_anchorInfo = new CAnchorSet;
	m_skeleton = new CSkeleton;
	m_BoundingInfo = new CBounding;
	m_BspInfo = new CBsp;
	m_avatarShowInfo = new CAvatarShowInfo;
	m_modelMotion = new CModelMotionInfo;
	m_commandMgr = new CCommandMgr;
	m_refNodeMgr = new CRefNodeMgr;
}

CEffectMgr::~CEffectMgr(void)
{
	delete m_refNodeMgr;
	delete m_commandMgr;
	delete m_modelMotion;
	delete m_avatarShowInfo;
	delete m_BspInfo;
	delete m_BoundingInfo;
	delete m_skeleton;
	delete m_anchorInfo;
}


void CEffectMgr::Initialze()
{
	CNPCOperator::InitDB();

	m_avatarShowInfo->m_pAvatarShow = new Ogre::AvatarManager;
	m_avatarShowInfo->m_pAvatarShow->init();

	m_refNodeMgr->AddRefNode(Ogre::Vector3(0.0f, 0.0f, 1000.0f), REF_POS_RIGHT);
}

void CEffectMgr::Release()
{
	delete m_avatarShowInfo->m_pAvatarShow;

	DestroyModel();
	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		delete m_Effects[i];
	}
}

//特效管理
void CEffectMgr::AddEffect(CEffectUnit* pEffect)
{
	m_Effects.push_back(pEffect);

	Ogre::SceneNode* pnode = pEffect->CreateNode();
	if(pEffect->GetBindInfo()->bDynamic == false && pnode)
		pEffect->BindToModel(m_pModelActor, pEffect->GetBindInfo()->boneid);  //m_pModelActor->bindObject(pEffect->GetBindInfo()->boneid, pnode);

}

void CEffectMgr::RemoveEffect(CEffectUnit* pEffect)
{
	for(std::vector<CEffectUnit*>::iterator i = m_Effects.begin();
		i != m_Effects.end();
		i++)
	{
		if(pEffect == *i)
		{			
			CEffectUnit* pUnit = *i;
			if(pUnit->GetNode())
			{
				//CEnginAgent::GetInstance()->m_pRoom->DetachNode(pUnit->GetNode());
				pUnit->UnbindFromModel(m_pModelActor); //m_pModelActor->unbindObject(pUnit->GetNode() );
				pUnit->ReleaseNode();
			}
			if(pUnit->GetBindInfo()->bDynamic)
			{
				for(int i = 0; i < (int)m_modelMotion->m_motions.size(); i++)
				{
					CModelMotionInfo::MOTION& mo = *m_modelMotion->m_motions[i];
					for(int j = 0; j < (int) mo.objs.size(); j++)
					{
						if((mo.objs[j]->m_type == Ogre::MOT_MODEL || mo.objs[j]->m_type == Ogre::MOT_MODEL_2) && mo.objs[j]->m_pEffect == pUnit)
						{

							m_modelMotion->SetSelect(-1, -1);
							delete mo.objs[j];
							mo.objs.erase(mo.objs.begin() + j);

							break;
						}
					}
				}
			}

			m_Effects.erase(i);

			//clear belongings
			bool bClear = false;
			while(bClear == false)
			{
				bClear = true;
				for(int k = 0; k < (int)m_Effects.size(); k++)
				{
					if(m_Effects[k]->GetOwnerEffect() == pUnit)
					{
						m_Effects[k]->SetOwnerEffect(NULL);
						SendMainFrameMsg(EEM_DELETE_EFFECT, 0, (LPARAM)m_Effects[k]);
						bClear = false;
					}
				}
			}

			//Notify Owner
			if (pUnit->GetOwnerEffect())
			{
				pUnit->GetOwnerEffect()->OnBelongingRemoved(pUnit);
			}

			delete pUnit;
			break;
		}
	}

}

void CEffectMgr::ClearEffect()
{
	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		if (m_Effects[i]->GetNode())
		{
			m_Effects[i]->UnbindFromModel(m_pModelActor); // m_pModelActor->unbindObject(m_Effects[i]->GetNode() );
			m_Effects[i]->ReleaseNode();
		}
		delete m_Effects[i];
	}
	m_Effects.clear();
}

void CEffectMgr::ClearModelPrivateEffect()
{
	std::vector<CEffectUnit*> effects;
	GetEffectInPrivateMotion(effects);

	for(int i = 0; i < (int) effects.size(); i++)
	{
		CEffectUnit* pCurrUnit = effects[i];
		bool bFound = false;
		for(int k = 0; k < (int) m_Effects.size(); k++)
		{
			CEffectUnit* pEffect = m_Effects[k];

			if(pEffect == pCurrUnit)
			{
				if (pEffect->GetNode())
				{
					pEffect->UnbindFromModel(m_pModelActor); // m_pModelActor->unbindObject(m_Effects[i]->GetNode() );
					pEffect->ReleaseNode();
				}
				delete pEffect;
				m_Effects.erase(m_Effects.begin() + k);
				bFound = true;
				break;
			}
		}
		assert(bFound == true);
	}

}

CEffectUnit* CEffectMgr::FindEffect(int srcid)
{
	return NULL;
}

CEffectUnit* CEffectMgr::FindEffect(const char* name)
{
	CString strname = name;
	strname.MakeUpper();
	for(int i = 0; i < (int)m_Effects.size(); i++)
	{
		CString effname = m_Effects[i]->GetName();
		effname.MakeUpper();
		if(name == effname)
			return m_Effects[i];
	}
	return NULL;
}

int CEffectMgr::GetNewCreatedEffect(std::vector<CEffectUnit*>& effects)
{
	effects.clear();
	for(int i = 0; i < (int)m_Effects.size(); i++)
	{
		if(m_Effects[i]->IsNew())
			effects.push_back(m_Effects[i]);
	}
	return (int)effects.size();
}

void CEffectMgr::ExecuteCommand(CEffectCommand* pCommand)
{
	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		if(m_Effects[i] == pCommand->GetTarget())
		{
			m_Effects[i]->ExecuteCommand(pCommand);
			PostMainFrameMsg(EEM_UPDATA_LIST, UE_EFFECT_LIST);
			break;
		}
	}
}

void CEffectMgr::RefreshEffect(CEffectUnit* pEffect)
{
	if(pEffect->IsVisible())
	{
		BIND_INFO* pBind = pEffect->GetBindInfo();
		if(pBind->bDynamic == false)
		{
			Ogre::SceneNode* poldnode = pEffect->GetNode();
			if(poldnode)
			{
				pEffect->UnbindFromModel(m_pModelActor); //m_pModelActor->unbindObject(poldnode);

			}

			pEffect->ReleaseNode();
			Ogre::SceneNode* pnode = pEffect->CreateNode();
			if(pnode != NULL)
			{
				pEffect->BindToModel(m_pModelActor, pBind->boneid);  //m_pModelActor->bindObject(pBind->boneid, pnode);
			}
		}
		else
		{
			pEffect->UpdateSource();
			m_modelMotion->RefreshEffect(pEffect);
		}
	}
	else
	{
		pEffect->ReleaseNode();
		Ogre::SceneNode* pnode = pEffect->CreateNode();
	}

	if(pEffect->GetOwnerEffect() != NULL)
	{
		RefreshEffect(pEffect->GetOwnerEffect());
	}
} 

void CEffectMgr::ChangeEffectType(CEffectUnit* pEffect, bool bDynamic)
{
	if(pEffect == NULL)
		return;
	BIND_INFO* pBind = pEffect->GetBindInfo();
	if(pBind->bDynamic == bDynamic)
		return ;

	pBind->bDynamic = bDynamic;
	if(pBind->bDynamic == true)
	{
		Ogre::SceneNode* poldnode = pEffect->GetNode();
		if(poldnode)
		{
			pEffect->UnbindFromModel(m_pModelActor);

		}
		pEffect->ReleaseNode();
	}
	else
	{
		pEffect->ReleaseNode(); 
		Ogre::SceneNode* pnode = pEffect->CreateNode();
		if(pnode != NULL)
		{
			pEffect->BindToModel(m_pModelActor, pBind->boneid); 
		}
	}
}


void CEffectMgr::GetEffectInPrivateMotion(std::vector<CEffectUnit*>& effects)
{
	for(int i = 0; i < (int)m_modelMotion->m_motions.size(); i++)
	{
		CModelMotionInfo::MOTION* pmo = m_modelMotion->m_motions[i];
		if(pmo->bIsPublic == true)
			continue;

		for(int k = 0; k < (int) pmo->objs.size(); k++)
		{
			CObjectLayerInfo* pObj = pmo->objs[k];
			if(pObj->m_type == Ogre::MOT_MODEL_2 && pObj->m_pEffect != NULL)
			{
				effects.push_back(pObj->m_pEffect);
			}
		}
	}

	for(int i = 0; i <(int) m_Effects.size(); i++)
	{
		if(m_Effects[i]->GetBindInfo()->bDynamic == false)
		{
			effects.push_back(m_Effects[i]);
		}
	}
}

void CEffectMgr::GetEffectInCommonMotion(std::vector<CEffectUnit*>& effects)
{
	for(int i = 0; i < (int)m_modelMotion->m_motions.size(); i++)
	{
		CModelMotionInfo::MOTION* pmo = m_modelMotion->m_motions[i];
		if(pmo->bIsPublic == false)
			continue;

		for(int k = 0; k < (int) pmo->objs.size(); k++)
		{
			CObjectLayerInfo* pObj = pmo->objs[k];
			if(pObj->m_type == Ogre::MOT_MODEL_2 && pObj->m_pEffect != NULL)
			{
				effects.push_back(pObj->m_pEffect);
			}
		}
	}

}


//模型管理
void CEffectMgr::LoadModelFromXml(CString path)
{
	//ShowEngine* pShowEngine = CEnginAgent::GetInstance()->m_pShowEngine;
	//
	//DestroyModel();

	//Ogre::XMLData xmldata;
	//if(!xmldata.LoadXMLFile(path))
	//{
	//	return;
	//}

	//Ogre::XMLNode *prootnode = xmldata.GetRootNode();
	//Ogre::XMLNode *pnode = prootnode->GetChild("skinactor");
	//if(pnode == NULL)
	//{
	//	return;
	//}
	//
	//m_strModName = prootnode->GetChild("skinactor")->AttribToString("path");
	//Ogre::Model* pActor = (Ogre::Model*)Ogre::BuildNodeFromChunk(m_strModName.GetBuffer());
	////Ogre::ModelData* pSAChunk = (Ogre::ModelData*) Ogre::LoadChunkFromFile("chunkdata", m_strModName.GetBuffer());

	//m_pModelActor = new Ogre::Entity();
	//m_pModelActor->SetMainNode(pActor);
	//OGRE_RELEASE(pActor);


	//m_pModelActor->attachToScene (CEnginAgent::GetInstance()->m_pRoom);
	//
	//m_skeleton->SetCoreSkeleton(pActor->GetSkeleton()->m_pCoreSkeleton);
	//m_anchorInfo->SetAnchorInifo(pActor);

	//m_strActorFileName = path;
	//pnode = prootnode->GetChild("bindobjs");
	//if(pnode != NULL)
	//{
	//	for(size_t i=0; i<pnode->GetNumChild(); i++)
	//	{
	//		Ogre::XMLNode *pchild = pnode->GetIthChild(i);
	//		Ogre::Resource* pchunk = Ogre::LoadChunkFromFile("chunkdata", pchild->AttribToString("path"));
	//		if(CEffectUnit::CanEdit(pchunk->GetType()))
	//		{
	//			char* tmp = new char[256];
	//			strcpy(tmp, pchild->AttribToString("path"));
	//			NEWEFFECT_INFO ni;
	//			pchild->AttribToInt("anchor", ni.anchor);
	//			if(pchild->HasAttrib("x") && pchild->HasAttrib("x") && pchild->HasAttrib("x"))
	//			{
	//				ni.offset.x = pchild->AttribToFloat("x");
	//				ni.offset.y = pchild->AttribToFloat("y");
	//				ni.offset.z = pchild->AttribToFloat("z");
	//			}
	//			ni.path = tmp;

	//			SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);
	//		}
	//		else
	//		{
	//			//m_pModelActor->bindObject(pchild->AttribToInt("anchor"), pchild->AttribToString("path"));
	//		}
	//		delete(pchunk);
	//	}
	//}

	//m_avatarShowInfo->EnableAvatar(false);

	//PostMainFrameMsg(EEM_ACTOR_LOADED);
}

bool CEffectMgr::LoadModelFromAvatar(int* attrib)
{
	std::vector<Ogre::Resource *> animres;
	Ogre::FixedString modelpath = Ogre::AvatarManager::getSingleton().getModelPath(attrib, animres);
	CString path = modelpath;
	if(LoadShowModel(path, false, &animres) == false)
	{
		return false;
	}

	for(int i = 0; i < MAX_AVATAR_FIELD; i++)
		m_avatarShowInfo->m_curAttrib[i] = attrib[i];
	m_avatarShowInfo->EnableAvatar(true);
	m_avatarShowInfo->UpdateActor();

	PostMainFrameMsg(EEM_ACTOR_LOADED, 1000); //1000用于标记加载了showmodel

	return true;

}



bool CEffectMgr::LoadSkinActor(CString path)
{
	DestroyMainNode();
	m_pModelActor = new Ogre::Entity;
	m_pModelActor->load(path.GetBuffer(), true);
	m_pModelActor->setPostSceneRenderer(g_MainSceneRenderer);

	m_pModelActor->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
	m_strModName = path;
	if(m_pModelActor->getMainModel() && IS_KIND_OF(Ogre::Model, m_pModelActor->getMainModel()))
	{
		Ogre::Model* pActor = (Ogre::Model*)m_pModelActor->getMainModel();
		m_skeleton->RemoveImportedAnim();
		m_skeleton->SetCoreSkeleton(pActor->getModelData());
		m_anchorInfo->SetAnchorInifo(m_pModelActor);
	}
	else
	{
		m_skeleton->ClearSkeleton();
		m_anchorInfo->SetAnchorInifo(NULL);
	}

	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		if(m_Effects[i]->GetBindInfo()->bDynamic == false)
			m_Effects[i]->BindToModel(m_pModelActor, 0); // m_pModelActor->bindObject(0, m_Effects[i]->GetNode());
		m_Effects[i]->GetBindInfo()->boneid = 0;
	}

	m_avatarShowInfo->EnableAvatar(false);
	//m_modelMotion->CreateInteract();
	m_strModName = path;
	m_BoundingInfo->m_isValid = false;
	m_BoundingInfo->m_bShowAABB = false;

	PostMainFrameMsg(EEM_ACTOR_LOADED);

	return true;
}

bool CEffectMgr::LoadShowModel(CString path, bool bNotify, std::vector<Ogre::Resource *> *animres)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL || !IS_KIND_OF(Ogre::EntityData, pChunk))
	{
		CString error = CString("装载文件错误：") + path;
		//AfxMessageBox(error.GetBuffer(), MB_OK);
		return false;
	}
	ClearEffect();
	DestroyModel();

	Ogre::EntityData* pModelChunk = (Ogre::EntityData*) pChunk;
	if(pModelChunk->m_pMainResource)
	{
		m_pModelActor = new Ogre::Entity;
		m_pModelActor->load(pModelChunk->m_pMainResource);

		m_strModName = (const char*)pModelChunk->m_pMainResource->getResPath();
		m_BoundingInfo->m_isValid = false;
		m_BoundingInfo->m_Center = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		m_BoundingInfo->m_Extent = Ogre::Vector3(100.0f, 100.0f, 100.0f);
	}
	else
	{
		CActorIO actorio;
		Ogre::ModelData* pchunk = actorio.LoadDefaultActor();
		m_pModelActor = new Ogre::Entity();
		m_pModelActor->load(pchunk);

		m_BoundingInfo->SetBounding(&pchunk->m_Bounding);
		m_BoundingInfo->m_isValid = true;

		pchunk->release();
	}
	m_pModelActor->setPostSceneRenderer(g_MainSceneRenderer);

	assert (m_pModelActor->getMainModel() != NULL && "Main Node is NULL");
	if(m_pModelActor->getMainModel() == NULL)
	{
		return false;
	}

	if(animres)
	{
		for(size_t i=0; i<animres->size(); i++)
		{
			m_pModelActor->getMainModel()->AddAnimation((*animres)[i]);
		}
	}


	m_pModelActor->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
	if(IS_KIND_OF(Ogre::Model, m_pModelActor->getMainModel()))
	{
		Ogre::Model* pActor = (Ogre::Model*)m_pModelActor->getMainModel();
		m_skeleton->SetCoreSkeleton(pActor->getModelData());
		m_anchorInfo->SetAnchorInifo(m_pModelActor);

		//外部引用的动画，因为有的动画文件是单?娣诺?
		m_skeleton->SetCoreSkeleton(pActor->getModelData());
	}
	else
	{
		m_skeleton->ClearSkeleton();
		m_anchorInfo->SetAnchorInifo(NULL);
	}

	m_modelMotion->LoadFromChunk(pModelChunk);
	for(size_t i=0; i<pModelChunk->m_Motions.size(); i++)
	{
		m_modelMotion->m_motions[i]->bIsPublic = false;
	}

	m_modelMotion->CreateInteract();
	m_avatarShowInfo->EnableAvatar(false);



	OGRE_RELEASE(pModelChunk);
	m_strSmName = path;


	if(bNotify)
		PostMainFrameMsg(EEM_ACTOR_LOADED);

	return true;
}

void CEffectMgr::CreateDefaultModel()
{
	ClearEffect();
	DestroyModel();

	CActorIO actorio;
	Ogre::ModelData* pchunk = actorio.LoadDefaultActor();

	m_pModelActor = new Ogre::Entity();
	m_pModelActor->load(pchunk);
	m_pModelActor->setPostSceneRenderer(g_MainSceneRenderer);

	m_pModelActor->attachToScene(CEnginAgent::GetInstance()->m_pRoom);

	Ogre::Model* pActor = (Ogre::Model*)m_pModelActor->getMainModel();
	m_skeleton->ClearSkeleton();
	m_skeleton->SetCoreSkeleton(pActor->getModelData());
	m_anchorInfo->SetAnchorInifo(m_pModelActor);
	m_avatarShowInfo->EnableAvatar(false);
	m_modelMotion->CreateInteract();
	m_BoundingInfo->m_isValid = true;
	m_BoundingInfo->m_Center = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	m_BoundingInfo->m_Extent = Ogre::Vector3(100.0f, 100.0f, 100.0f);

	PostMainFrameMsg(EEM_ACTOR_LOADED);
	OGRE_RELEASE(pchunk);
}

void CEffectMgr::updateAnchors()
{
	if(m_pModelActor->getMainModel() && IS_KIND_OF(Ogre::Model, m_pModelActor->getMainModel()))
	{
		m_anchorInfo->SetAnchorInifo(m_pModelActor);
	}
	else
	{
		m_anchorInfo->SetAnchorInifo(NULL);
	}

	PostMainFrameMsg(EEM_ACTOR_LOADED);
}

void CEffectMgr::SaveShowActor(CString path)
{
	static char* strheader = "<showactor opaque=\"%f\" scale=\"%f\">\r\n";
	static char* stractor  = "    <skinactor path=\"%s\" />\r\n";
	static char* strbind   = "    <bindobjs>\r\n";
	static char* strobj    = "        <obj anchor=\"%d\" x=\"%f\"  y=\"%f\" z=\"%f\" path=\"%s\" />\r\n";
	static char* strbind_t = "    </bindobjs>\r\n";
	static char* strtail   = "</showactor>\r\n";

	FILE* pfile = fopen(path, "w");
	if (!pfile)
		return;

	fprintf(pfile, strheader, 1.0f, 1.0f);
	fprintf(pfile, stractor, m_strModName.GetBuffer());
	fprintf(pfile, strbind);
	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		fprintf(pfile, strobj, m_Effects[i]->GetBindInfo()->boneid
			,m_Effects[i]->GetBindInfo()->offset.x
			,m_Effects[i]->GetBindInfo()->offset.y
			,m_Effects[i]->GetBindInfo()->offset.z
			, m_Effects[i]->GetName().GetBuffer());
	}
	fprintf(pfile, strbind_t);
	fprintf(pfile, strtail);

	fclose(pfile);

}

bool CEffectMgr::SaveAsShowModel(CString path, bool bChangeName)
{
	return SaveAsShowModelImpl(path, bChangeName, -1);
}

bool CEffectMgr::SaveMotionAsShowModel(CString path, int iIncludeMotion)
{
	return SaveAsShowModelImpl(path, false, iIncludeMotion);
}

bool CEffectMgr::SaveAsShowModelImpl(CString path, bool bChangeName, int iIncludeMotion )
{
	Ogre::EntityData* pChunk = new Ogre::EntityData();
	Ogre::Resource* pMainActorChunk = NULL;
	if(m_strModName != "")
	{
		pMainActorChunk = Ogre::ResourceManager::getSingleton().blockLoad(m_strModName.GetBuffer());
	}
	else
	{
		CActorIO actorio;
		pMainActorChunk = actorio.LoadDefaultActor();

		//增加bounding box
		float fMaxRange = 5.0f;
		for(int i = 0; i < (int) m_Effects.size(); i++)
			if(m_Effects[i]->GetRange() > fMaxRange) fMaxRange = m_Effects[i]->GetRange();

		Ogre::Vector3 exten(fMaxRange, fMaxRange, fMaxRange);
		static_cast<Ogre::ModelData *>(pMainActorChunk)->m_Bounding.setCenterExtension(Ogre::Vector3(0,0,0), exten);

		if(m_BoundingInfo->IsValid())
		{
			static_cast<Ogre::ModelData *>(pMainActorChunk)->m_Bounding.setCenterExtension(m_BoundingInfo->m_Center, m_BoundingInfo->m_Extent);
		}
	}

	pChunk->m_pMainResource = pMainActorChunk;

	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		CEffectUnit* pEffect = m_Effects[i];
		if(pEffect->GetBindInfo()->bDynamic) //动态特效由motion负责管理
			continue;

		if(pEffect->GetOwnerEffect() != NULL) //具有owner的特效，由owner管保存
			continue;
	}

	if(iIncludeMotion >= 0)
	{
		m_modelMotion->CreateSingleMotionChunk(iIncludeMotion, pChunk);
	}
	else
	{
		m_modelMotion->CreateMotionChunk(pChunk);
	}

	pChunk->save(path.GetBuffer());
	pChunk->release();
	if(bChangeName)
	{
		m_strSmName = path;
		return true;
	}
	return true;
}

Ogre::Entity* CEffectMgr::CreateCloneModel()
{
	Ogre::Entity* pModel = NULL;
	if(m_avatarShowInfo->IsEnable() == true)
	{
		Ogre::AvatarEntity *pentity = new Ogre::AvatarEntity;
		pentity->updateAttrib(m_avatarShowInfo->m_curAttrib);

		pModel = pentity;
	}
	else
	{
		if(m_strModName == "")
			return NULL;

		pModel = new Ogre::Entity;
		pModel->load(m_strModName.GetBuffer(), true);
	}

	for(int i = 0; i < (int)m_Effects.size(); i++)
	{
		Ogre::MovableObject *pobj = createObjectFromResource(m_Effects[i]->GetSource());
		pModel->bindObject(m_Effects[i]->GetBindInfo()->boneid, pobj);
	}
	for(int i = 0; i < (int)m_modelMotion->m_motions.size(); i++)
	{
		InjectMotion(i, pModel);
	}

	return pModel;
}

void CEffectMgr::DestroyMainNode()
{
	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		m_Effects[i]->GetBindInfo()->boneid = 0;
		if(m_Effects[i]->GetNode())
		{
			m_Effects[i]->UnbindFromModel(m_pModelActor); //m_pModelActor->unbindObject(m_Effects[i]->GetNode());
		}
	}
	if(m_pModelActor)
	{
		m_pModelActor->detachFromScene();
		OGRE_RELEASE(m_pModelActor);
	}
	PostMainFrameMsg(EEM_UPDATA_LIST, UE_ALL);
	//m_modelMotion->DestroyInteract();
	m_strActorFileName = "";
	m_strModName = "";
}

void CEffectMgr::DestroyModel()
{
	m_modelMotion->DestroyInteract();
	//m_modelMotion->ClearModelPrivateMotion();
	m_modelMotion->Clear();

	for(int i = 0; i < (int) m_Effects.size(); i++)
	{
		m_Effects[i]->GetBindInfo()->boneid = 0;
		if(m_Effects[i]->GetNode())
		{
			m_Effects[i]->UnbindFromModel(m_pModelActor);
		}
	}

	if(m_pModelActor)
	{
		m_pModelActor->detachFromScene();
		OGRE_RELEASE(m_pModelActor);
	}
	//删除Target
	if(m_pModelTarget)
	{
		m_pModelTarget->detachFromScene();
		OGRE_RELEASE(m_pModelTarget);
	}

	PostMainFrameMsg(EEM_UPDATA_LIST, UE_ALL);
	m_strActorFileName = "";
	m_strModName = "";
	m_strSmName = "";

	m_skeleton->ClearSkeleton();


}

bool CEffectMgr::PlayAnim(int seq)
{
	if(m_pModelActor)
	{
		m_bPause = false;
		m_pModelActor->playAnim(seq);
	}

	for ( size_t i=0; i<m_pModelActor->m_BindObjs.size(); i++)
	{
		Ogre::MovableObject *pobj = m_pModelActor->m_BindObjs[i]->pobject;
		if(IS_KIND_OF(Ogre::Model, pobj))
		{
			static_cast<Ogre::Model*>(pobj)->playAnim(seq);
		}
		else if(IS_KIND_OF(Ogre::Entity, pobj))
		{
			static_cast<Ogre::Entity *>(pobj)->playAnim(seq);
		}
	}

	return true;
}

void CEffectMgr::StopAnim(int seq)
{
	if(m_pModelActor)
	{
		//((Ogre::Model*)(m_pModelActor->getMainModel()))->ResetPos();
		m_pModelActor->getMainModel()->stopAnim(seq);
	}
}

void CEffectMgr::PauseAnim(bool bPause)
{
	m_bPause = bPause;
	if(m_pModelActor)
	{
		m_pModelActor->resetUpdate(bPause);
	}
}

void CEffectMgr::InjectMotion(const char* name)
{
	InjectMotion(name, m_pModelActor);
}

void CEffectMgr::InjectMotion(int index)
{
	InjectMotion(index, m_pModelActor);
}

void CEffectMgr::InjectMotion(const char* name, Ogre::Entity* pModel)
{
	for(int i = 0; i < (int) m_modelMotion->m_motions.size(); i++)
	{
		if(m_modelMotion->m_motions[i]->name == name)
		{
			InjectMotion(i, pModel);
			break;
		}
	}
}

bool IsObjectLayerVisible(CObjectLayerInfo &src)
{
	if(src.m_bVisible == false) return false;
	if(src.getParent()) return IsObjectLayerVisible(*(src.getParent()));
	else return true;
}

void CEffectMgr::InjectMotion(int index, Ogre::Entity* pModel)
{
	Ogre::ModelMotion* pMotion = new Ogre::ModelMotion();
	CModelMotionInfo::MOTION* pMoSrc = m_modelMotion->m_motions[index];
	Ogre::EntityMotionData* pMotionSource = new Ogre::EntityMotionData;

	pMotionSource->m_fTime = pMoSrc->fTime;
	pMotionSource->m_strName = pMoSrc->name.c_str();
	pMotionSource->m_loop = pMoSrc->loop;
	pMotionSource->m_iModLodLevel = pMoSrc->stMotionLodLevel;

	if(m_modelMotion->IsLoopPlay() == true)
	{
		pMotionSource->m_loop = Ogre::ML_LOOP;
	}

	Ogre::ObjectMotion* pObjMotion = NULL;
	std::map<CObjectLayerInfo*, Ogre::ObjectMotion*> addressToPos;
	std::map<int, int> realPosToCurrPos;
	for(int i = 0; i < (int)pMoSrc->objs.size(); i++)
	{
		CObjectLayerInfo& src =*( pMoSrc->objs[i]);
		if(!IsObjectLayerVisible(src))
		{
			continue;
		}

		Ogre::MotionElementData *pelementdata = CObjectLayerInfo::createMotionElementData(src.m_type);
		int iParent = m_modelMotion->FindObjectIndex(pMoSrc, src.getParent());
		if(iParent >= 0)
		{
			assert(realPosToCurrPos.find(iParent) != realPosToCurrPos.end());
			iParent = realPosToCurrPos[iParent];
		}
		src.saveToElementData(pelementdata, i, iParent);

		realPosToCurrPos[i] = (int)pMotionSource->m_Elements.size();
		pMotionSource->m_Elements.push_back(pelementdata);
	}

	pMotion->LoadFromSource(pMotionSource);
	pModel->addMotion(pMotion);

	pMotionSource->release();
	pMotion->release();
}


void CEffectMgr::ClearModelMotion()
{
	m_pModelActor->stopMotion();
	m_pModelActor->stopAnim();
	m_pModelActor->clearMotions();
}

void CEffectMgr::PlayMotion(const char* name)
{
	for(int i = 0; i < (int) m_modelMotion->m_motions.size(); i++)
	{
		if(m_modelMotion->m_motions[i]->name == name)
		{
			PlayMotion(i);
			break;
		}
	}
}

void CEffectMgr::PlayMotion(int index)
{
	if(index < 0 || index >= (int)m_modelMotion->m_motions.size())
		return;

	//清除原挂上去的Motion
	ClearModelMotion();

	//只挂上要播放的Motion;
	InjectMotion(index);

	m_modelMotion->SetHotTime(-1);

	//m_pModelActor->SetEnablePostEffect(true);
	//m_pModelActor->SetGameFocus(true);
	m_pModelActor->playMotion(m_modelMotion->m_motions[index]->name.c_str());
}

//index is reserved
void CEffectMgr::StopMotion(int index)
{
	ClearModelMotion();
	/*
	for(int i = 0; i < (int)m_pModelActor->m_Motions.size(); i++)
	{
	if(m_pModelActor->m_Motions[i]->m_bPlaying == true)
	{
	m_pModelActor->stopMotion(m_pModelActor->m_Motions[i]->m_strName.c_str());
	}
	ClearModelMotion();
	}*/

	//static_cast<Ogre::Model*>(m_pModelActor->getMainModel())->ResetPos();
}

void CEffectMgr::AttachAnim(const char* filename)
{
	if(!m_pModelActor || !m_pModelActor->getMainModel())
		return;

	//if(m_pModelActor->getMainModel()->IsKindOf(&Ogre::Model::m_RTTI) == true)
	{
		//Ogre::Model* pActor = (Ogre::Model*)m_pModelActor->getMainModel();
		//pActor->AttachAnimation(filename);
		//m_skeleton->m_animChunks.push_back(filename);
		m_skeleton->AttachAnims(filename);
		//m_skeleton->SetCoreSkeleton(pActor->GetSkeleton()->m_pCoreSkeleton);
	}
}


void CEffectMgr::AddFrameJob(CFrameJob* pJob)
{
	m_FrameJobs.push_back(pJob);
}
void CEffectMgr::RemoveFrameJob(CFrameJob* pJob)
{
	std::list<CFrameJob*> ::iterator it = m_FrameJobs.begin();
	for(; it != m_FrameJobs.end(); it++)
	{
		if(*it == pJob)
		{
			m_FrameJobs.erase(it);
			break;
		}
	}
}

void CEffectMgr::UpdateEffectPos(Ogre::uint dtick)
{

	CEnginAgent* pEA = CEnginAgent::GetInstance();
	//m_skill->DoFrame(dtick / 1000.0f * pEA->GetSpeed() );
	m_modelMotion->DoFrame(dtick);
	//if(pEA->m_pRoom->IsKindOf(&Ogre::BigTerrainRoom::m_RTTI) == false)
	m_pModelActor->update((Ogre::uint)(dtick*pEA->GetSpeed()));

	if(m_iActive >= 0)
	{
		m_Effects[m_iActive]->GetInteract()->DoFrame(dtick);
	}

	//for(int i = 0; i < (int)m_Effects.size(); i++)
	//{
	//if(m_Effects[i]->GetBindInfo()->bDynamic == true)
	//	continue;
	//if(m_Effects[i]->IsVisible() == false)
	//	continue;

	//CEffectUnit* pEffect = m_Effects[i];
	//int iBone = pEffect->GetBindInfo()->boneid;
	//Ogre::Vector3	nodeOffset = pEffect->GetBindInfo()->offset;
	//Ogre::SceneNode* pNode = pEffect->GetNode();

	//if(pNode)
	//{
	//	pEffect->DoFrame(dtick);
	//}
	//}
	m_BoundingInfo->DoFrame(dtick);

	std::list<CFrameJob*> ::iterator it = m_FrameJobs.begin();
	for(; it != m_FrameJobs.end(); it++)
	{
		CFrameJob* pJob = *it;
		pJob->DoJobFrame((int)dtick);
	}

	//m_anchorInfo->DoFrame(dtick);
}

bool CEffectMgr::OnLBtnDown(float x, float y)
{
	bool ret = false;

	if(m_modelMotion->OnLButtonDown(x, y))
		return true;;
	//if(m_iActive >=0 && m_iActive < (int)m_Effects.size())
	//{
	//	if(m_Effects[m_iActive]->IsVisible())
	//	{
	//		CInteract* pInter = m_Effects[m_iActive]->GetInteract();
	//		if(pInter != NULL && pInter->OnLBtnDown(x, y, false))
	//			return true;
	//	}
	//}

	//for(int i = 0; i < (int)m_Effects.size(); i++)
	//{
	//	if(i == m_iActive)
	//		continue;
	//	if(!m_Effects[i]->IsVisible())
	//		continue;
	//	CInteract* pInter = m_Effects[i]->GetInteract();
	//	if(pInter != NULL)
	//	{
	//		if(pInter->OnLBtnDown(x, y, false) == true)
	//		{
	//			m_iActive = i;
	//			ret = true;
	//		}
	//	}
	//}
	return ret;
}

bool CEffectMgr::OnLBtnUp(float x, float y)
{
	bool ret = false;
	if(m_modelMotion->OnLButtonUp(x, y))
		return true;

	//if(m_iActive >=0 && m_iActive < (int)m_Effects.size())
	//{
	//	if(m_Effects[m_iActive]->IsVisible())
	//	{
	//		CInteract* pInter = m_Effects[m_iActive]->GetInteract();
	//		if(pInter != NULL && pInter->OnLBtnUp(x, y))
	//			return true;
	//	}
	//}

	//for(int i = 0; i < (int)m_Effects.size(); i++)
	//{
	//	if(i == m_iActive)
	//		continue;
	//	if(!m_Effects[i]->IsVisible())
	//		continue;
	//	CInteract* pInter = m_Effects[i]->GetInteract();
	//	if(pInter != NULL)
	//	{
	//		if(pInter->OnLBtnUp(x, y) == true)
	//			ret = true;
	//	}
	//}
	return ret;
}

bool CEffectMgr::OnDblClick(float x, float y)
{
	//if(m_iActive >= 0 && m_iActive < (int)m_Effects.size())
	//{
	//	CEffectUnit* pUnit = m_Effects[m_iActive];
	//	bool touched = C3DMath::CheckTouchBox(pUnit->GetNode()->GetWorldPosition().ToVector3(), x, y, pUnit->GetRange());
	//	if(touched)
	//	{
	//		PostMainFrameMsg(EEM_GOTO_PAGE, 0, (LPARAM)pUnit);
	//		return true;
	//	}
	//}
	return false;
}

bool CEffectMgr::OnMouseMove(float x, float y)
{
	bool ret = false;
	if(m_modelMotion->OnMouseMove(x, y))
		return true;

	//if(m_iActive >=0 && m_iActive < (int)m_Effects.size())
	//{
	//	if(m_Effects[m_iActive]->IsVisible())
	//	{
	//		CInteract* pInter = m_Effects[m_iActive]->GetInteract();
	//		if(pInter != NULL && pInter->OnMouseMove(x, y))
	//			return true;
	//	}
	//}

	//for(int i = 0; i < (int)m_Effects.size(); i++)
	//{
	//	if(i == m_iActive)
	//		continue;
	//	if(!m_Effects[i]->IsVisible())
	//		continue;

	//	CInteract* pInter = m_Effects[i]->GetInteract();
	//	if(pInter != NULL)
	//	{
	//		if(pInter->OnMouseMove(x, y) == true)
	//			ret = true;
	//	}
	//}
	return ret;
}

bool CEffectMgr::SetActiveEffect(CEffectUnit* pUnit)
{
	for(int i = 0; i <(int) m_Effects.size(); i++)
	{
		if(pUnit == m_Effects[i])
		{
			m_iActive = i;
			return true;
		}
	}
	return false;
}

bool CEffectMgr::ToBigTerrainRoom(Ogre::GameScene* pSimpleRoom, Ogre::GameScene* pBigRoom)
{
	m_pModelActor->detachFromScene();
	m_pModelActor->attachToScene(pBigRoom);

	return true;
}

bool CEffectMgr::ToSimpleRoom(Ogre::GameScene* pBigRoom, Ogre::GameScene* pSimpleRoom)
{
	m_pModelActor->detachFromScene();
	m_pModelActor->attachToScene(pSimpleRoom);
	return true;
}

bool CEffectMgr::BigToBig(Ogre::GameScene* pOldBig, Ogre::GameScene* pNewBig)
{
	m_pModelActor->detachFromScene();
	m_pModelActor->attachToScene(pNewBig);
	return true;
}

void CEffectMgr::SetModelActorPos(Ogre::Vector3& pos)
{
	m_pModelActor->setPosition(pos);
}

//此函数暂时无用
void CEffectMgr::SetModelTargetPos(Ogre::Vector3& pos)
{
	//	m_pModelTarget->SetPosition(pos);
}

void CEffectMgr::SetMainActorState(bool bShow)
{
	m_bShowMainActor = bShow;
	if(m_bShowMainActor)
		m_pModelActor->getMainModel()->setTransparent(1.0f);
	else
		m_pModelActor->getMainModel()->setTransparent(0.0f);
};
