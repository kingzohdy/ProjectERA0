
#include "stdafx.h"
#include "EffectUnit_UnknownObject.h"
#include "UnknownObjectEditBar.h"
#include "ObjectTypeHelper.h"
#include "EffectManager.h"

#include "OgreEntity.h"
#include "OgreResourceManager.h"

// ---------------------CUnknownObject------------------------------------ [2/26/2008]

CUnknownObject::CUnknownObject(void)
{
	m_pNode = NULL;
	m_pSource = NULL;
	m_pModel = NULL;
	m_interact.Clear();
	m_docInfo.m_bNeedSave = false;
}

CUnknownObject::~CUnknownObject(void)
{
	OGRE_RELEASE(m_pModel);
	OGRE_RELEASE(m_pNode);
	OGRE_RELEASE(m_pSource);

}

Ogre::SceneNode* CUnknownObject::GetNode()
{
	if(m_pModel) return m_pModel;
	else return m_pNode;
}



CBCGPDockingControlBar* CUnknownObject:: CreateBar(CBCGPFrameWnd* pParent)
{
	CUnknownObjectBar* pBar = new CUnknownObjectBar();
	//static int id = 0;
	//pBar->Create(NULL, pParent, CRect (0, 0,  250,600),
	//	TRUE, 0,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	//pBar->SetEditTarget(this);
	return pBar;
}

Ogre::Resource* CUnknownObject::GetChunk()
{

	return NULL;;
}

void CUnknownObject::ReleaseNode()
{
	OGRE_RELEASE(m_pModel);
	OGRE_RELEASE(m_pNode);
	m_interact.Clear();
}

Ogre::SceneNode* CUnknownObject::CreateNode()
{
	ReleaseNode();
	m_interact.Create(this);
	//Ogre::Resource* pRes = Ogre::ResourceManager::getSingleton().blockLoad(m_strName.GetBuffer());
	if(m_pSource == NULL)
		return NULL;

	if(m_strResPath != m_strOldResPath)
	{
		ReloadSource();
		m_strOldResPath = m_strResPath;
	}

	m_pNode = createObjectFromResource(m_pSource);

	if(GetNode())
	{
		GetNode()->setPosition(m_bindInfo.offset);
		GetNode()->setRotation(m_bindInfo.rotate);
		GetNode()->setScale(m_bindInfo.scale);
	}

	return GetNode();
}

void CUnknownObject::ReloadSource()
{
	OGRE_RELEASE(m_pSource);

	CCvtHelper cvt;
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(m_strResPath.GetBuffer());
	if(!pChunk)
	{
		return;
	}

	EFFECT_TYPE type = cvt.ChunkTypeToEffectType(pChunk);

	if(type != EFFECT_UNKNOWN)
	{
		OGRE_RELEASE(pChunk);
		return ;
	}

	if(false == LoadFromChunk(pChunk))
	{
		OGRE_RELEASE(pChunk);
		return ;
	}

	OGRE_RELEASE(pChunk);
}

void CUnknownObject::RefreshNode()
{
	if(m_bVisible)
		return;

	Ogre::SceneNode* poldnode = m_pNode;
	//if(m_pNode)
	//	CEnginAgent::GetInstance()->m_pShowEngine->GetMap()->GetRoom()->DetachNode(m_pNode);

	//ReleaseNode();
	//Ogre::SceneNode* pnode = CreateNode();

	//CEnginAgent::GetInstance()->m_pShowEngine->GetMap()->GetRoom()->AttachNode(m_pNode);

	if(poldnode)
	{
		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(poldnode);
	}

	ReleaseNode();

	Ogre::SceneNode* pnode = CreateNode();

	if(pnode != NULL)
	{
		CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);
	}

}

void CUnknownObject::UpdateSource()
{
	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		GetNode()->setPosition(GetBindInfo()->offset);
		GetNode()->setRotation(GetBindInfo()->rotate);
		GetNode()->setScale(GetBindInfo()->scale);
	}
}

void CUnknownObject::DoFrame(Ogre::uint dtick)
{
	m_interact.DoFrame(dtick);
}

CString CUnknownObject::GetName()
{
	return m_strName;
}


bool CUnknownObject::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;

	m_bVisible = true;
	m_bIsNew = false;
	return true;
}

bool CUnknownObject::Destroy()
{
	return true;
}

//bool CUnknownObject::IsVisible()
//{
//	return m_bVisible;
//}
//
//void CUnknownObject::SetVisible(bool visible)
//{
//	if(m_bVisible == visible)
//		return;
//
//	m_bVisible = visible;
//
//	if(m_bVisible)
//	{
//		CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, GetNode());
//	}
//	else
//	{
//		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(GetNode());
//	}
//}

bool CUnknownObject::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());

	if(pChunk == NULL)
		return false;

	LoadFromChunk(pChunk);
	m_strName = path;
	OGRE_RELEASE(pChunk);	
	m_bIsNew = false;
	return true;
}

bool CUnknownObject::SaveEffect(CString path)
{
	//Ogre::LightChunk* pLightChunk = new Ogre::LightChunk();
	//pLightChunk->m_pLight->m_Type = m_Type;
	//pLightChunk->m_pLight->m_Color = m_Color;
	//pLightChunk->m_pLight->m_Attenuation = m_Attenuation;
	//pLightChunk->m_pLight->m_Range = m_Range;

	//bool ret = Ogre::SaveChunkToFile("chunkdata", path.GetBuffer(), pLightChunk);
	//if(ret)
	//{
	//	m_strName = path;
	//}
	//delete pLightChunk;
	//m_bIsNew = false;
	return true;

}

bool CUnknownObject::LoadFromChunk(Ogre::Resource* pChunk)
{
	m_strName.Format(_T(""));

	assert(IS_KIND_OF(Ogre::Resource, pChunk));
	m_pSource = static_cast<Ogre::Resource *>(pChunk);
	if(m_pSource == NULL) return false;

	m_pSource->addRef();
	m_bVisible = true;
	m_bIsNew = false;

	return true;
}

void CUnknownObject::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CUnknownObjectCommand* pLightCmd = (CUnknownObjectCommand*) pCmd;

	if(GetNode())
	{
		GetNode()->setPosition(m_bindInfo.offset);
		GetNode()->setRotation(m_bindInfo.rotate);
		GetNode()->setScale(m_bindInfo.scale);
	}

	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(true);
}

CEffectCommand* CUnknownObject::BeginRecordCommand()
{
	CUnknownObjectCommand* pRet = new CUnknownObjectCommand();
	pRet->m_pTarget = this;
	pRet->m_newBindInfo = m_bindInfo;
	return pRet;
}

void CUnknownObject::EndRecordCommand(CEffectCommand* psrccmd)
{
	CUnknownObjectCommand* pCmd = static_cast<CUnknownObjectCommand*>(psrccmd);
	pCmd->m_oldBindInfo = m_bindInfo;
	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pCmd);
}

BIND_INFO* CUnknownObject::GetBindInfo()
{
	return &m_bindInfo;
}

void CUnknownObject::SetBindInfo(BIND_INFO* bindinfo) 
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CUnknownObject::GetType()
{
	return EFFECT_UNKNOWN;
}

Ogre::Entity*	CUnknownObject::GetModel()
{
	if(IsModel()) return static_cast<Ogre::Entity *>(m_pNode);
	else return NULL;
}

bool CUnknownObject::IsModel()
{
	return IS_KIND_OF(Ogre::Entity, m_pNode);
}

// ---------------------CUnknownObjectCommand----------------------------------------- [2/26/2008]

CEffectUnit* CUnknownObjectCommand::GetTarget()
{
	return m_pTarget;
}

bool CUnknownObjectCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CUnknownObjectCommand::GetInversCommand()
{
	CUnknownObjectCommand* pRet = new CUnknownObjectCommand();
	pRet->m_pTarget = m_pTarget;
	pRet->m_oldBindInfo = m_newBindInfo;
	pRet->m_newBindInfo = m_oldBindInfo;

	return pRet;
}

bool CUnknownObjectCommand::IsIdentical()
{
	if(m_oldBindInfo	== m_newBindInfo)
	{
		return true;
	}
	return false;
}

bool CUnknownObjectCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	CUnknownObjectCommand* pLightCmd = (CUnknownObjectCommand*) pCmd;
	if(m_oldBindInfo.boneid == pLightCmd->m_oldBindInfo.boneid)
	{
		m_oldBindInfo.offset = pLightCmd->m_oldBindInfo.offset;
		return true;
	}
	return false;
}