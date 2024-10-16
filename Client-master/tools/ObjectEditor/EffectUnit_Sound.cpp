
#include "stdafx.h"
#include "EffectUnit_Sound.h"
#include "EffectManager.h"
#include "EditorManager.h"
#include "EffectUnitEditBar_Sound.h"

#include "OgreSoundNode.h"
#include "OgreLines.h"
#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreResourceManager.h"

CSoundInteract::CSoundInteract()
{
	m_pIdn = NULL;
}
CSoundInteract::~CSoundInteract()
{
	Clear();
}

void CSoundInteract::Clear()
{
	//if(m_bCood && m_pCood)
	//{
	//	m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);

	//}

	//m_bHovering = false;
	//OGRE_RELEASE(m_pCood);
	//m_pCood = NULL; //!!!!!
	//m_bCood = false;
	//m_tran = COOD_NONE;

	CInteract::Clear();
	//if(m_pIdn)
	//{
	//	CEnginAgent::GetInstance()->m_pRoom->DetachNode(m_pIdn);
	//	OGRE_RELEASE(m_pIdn);
	//	m_pIdn = NULL; //!!!!!!!
	//}
}

void CSoundInteract::Create(CEffectUnit* pTarget)
{
	//m_bHovering = false;
	//m_pCood = NULL;
	//m_bCood = false;
	//m_tran = COOD_NONE;
	//m_pTarget = pTarget;

	//m_pCood = new CCoord();
	//m_pCood->CreateVertex();
	CInteract::Create(pTarget);
	//m_pIdn = (Ogre::Billboard*)Ogre::BuildNodeFromChunk("effecteditor.effectedit.lighticon");
	//if(m_pIdn)
	//{
	//	CEnginAgent::GetInstance()->m_pRoom->AttachNode(m_pIdn);
	//}

}

//bool CSoundInteract::OnLBtnDown(float x, float y, bool bForceOn)
//{
//	if(m_pTarget->GetNode() == NULL || m_pTarget->GetBindInfo()->bDynamic)
//		return false;
//
//	if(false) //if(m_pTarget->GetBindInfo()->boneid != 0)
//	{
//		if(m_bCood)
//			m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);
//		m_bCood = false;
//		m_tran = COOD_NONE;
//		return false;
//	}
//
//	if(m_bCood)
//	{
//		m_tran = m_pCood->SelectArrow(x, y);
//		CEffectMgr::GetInstance()->GetCommandMgr()->ForceNewCommand();
//		if(m_tran != COOD_NONE)
//			return true;
//	}
//
//	bool bSelected = false;
//	if(bForceOn == true)
//		bSelected = true;
//	else
//		bSelected = C3DMath::CheckTouchBox(m_pTarget->GetNode()->GetWorldPosition().ToVector3() , x, y, m_pTarget->GetRange());
//
//	if(bSelected)
//	{
//		if(m_bCood && !bForceOn) //坐标已存在，在不是强制选定的情况下，确认移动轴
//		{
//			m_tran = m_pCood->SelectArrow(x, y);
//			CEffectMgr::GetInstance()->GetCommandMgr()->ForceNewCommand();
//		}
//		if(!m_bCood) //坐标不存在，创建坐标
//			m_pCood->AttachCoordToRoom(CEnginAgent::GetInstance()->m_pRoom);
//		m_bCood = true;
//
//		return true;
//	}
//	else
//	{
//		if(m_bCood)
//			m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);
//		m_bCood = false;
//		m_tran = COOD_NONE;
//
//	}
//	return false;
//}

void CSoundInteract::DoFrame(Ogre::uint dtick)
{
	//float range= m_pTarget->GetRange();
	//if(m_bHovering && !m_pTarget->GetBindInfo()->bDynamic)
	//{
	//	CEnginAgent::GetInstance()->AddContainerBox(m_pTarget->GetNode()->GetWorldPosition().ToVector3(), range);
	//}
	//m_pCood->SetFather(pTarget->GetNode());
	//m_pCood->m_fSize = range;

	CInteract::DoFrame(dtick);
	//if(m_pIdn && !m_pTarget->GetBindInfo()->bDynamic)
	//{
	//	m_pIdn->SetPosition(Ogre::WorldPos(m_pTarget->GetNode()->GetWorldPosition().ToVector3()));
	//}
	//m_pCood->SetPosition(Ogre::WorldPos(m_pTarget->GetNode()->GetWorldPosition().ToVector3()));
	CSound* pTarget = static_cast<CSound*>(m_pTarget);

	if(m_bCood)
	{
		DrawRange(pTarget->m_createInfo.maxdistance, Ogre::ColorQuad(255, 255, 140));
		DrawRange(pTarget->m_createInfo.mindistance, Ogre::ColorQuad(140, 255, 140));
	}
}

void CSoundInteract::DrawRange(float r, Ogre::ColorQuad clr)
{
	//float r = m_pTarget->m_createInfo.fMaxDistance;

	Ogre::Vector3 center = m_pTarget->GetNode()->getWorldMatrix().getRow3(3);
	Ogre::Matrix4 view = CEnginAgent::GetInstance()->m_pCamera->getViewMatrix();
	Ogre::Matrix4 viewInv;
	view.inverse(viewInv);

	Ogre::Vector3 viewCenter = center * view;

	//Ogre::Vector3 vert[50];
	Ogre::Vector3 pt;
	Ogre::Vector3 lastPt(r + viewCenter.x, viewCenter.y, viewCenter.z);
	lastPt = lastPt * viewInv;

	Ogre::RenderLines* pDebugger = CEnginAgent::GetInstance()->m_pLineRenderer;

	//pt = Ogre::Vector3(0, 0, 0);
	//lastPt = pt + Ogre::Vector3(100, 100, 0);
	for(int i = 0; i < 51; i++)
	{
		float theta = 360.0f * i / 50.0f;

		pt.x = Ogre::Cos(theta) * r + viewCenter.x;
		pt.y = Ogre::Sin(theta) * r + viewCenter.y;
		pt.z = viewCenter.z;

		pt = pt * viewInv;

		pDebugger->addLine(lastPt, pt, Ogre::ColorQuad(0, 255, 0, 255));

		lastPt = pt;

	}	



}



// -------------------CSound----------------------------- [12/14/2007]
CSound::CSound(void) //: m_pTmpChunk(NULL)
{

	m_bindInfo.boneid = 0;
	m_bVisible = true;
	m_pNode = NULL;
	m_pSource = NULL;
	m_interact.Clear();

}

CSound::~CSound(void)
{
}


Ogre::SceneNode* CSound::GetNode()
{
	return m_pNode;
}



CBCGPDockingControlBar*CSound:: CreateBar(CBCGPFrameWnd* pParent)
{
	CSoundBar* pBar = new CSoundBar();
	//static int id = 0;
	//pBar->Create(NULL, pParent, CRect (0, 0,  250,600),
	//	TRUE, 0,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	//pBar->SetEditTarget(this);m
	return pBar;
}

Ogre::Resource* CSound::GetChunk()
{
	Ogre::SoundData *pdata = new Ogre::SoundData;
	m_createInfo.volume = m_iVolumeInt / 10.0f;
	m_createInfo.time0 = 0.0f;
	m_createInfo.time1 = 0.0f;
	m_createInfo.fSoundFullRange = 0.0f;
	pdata->m_CreateInfo = m_createInfo;
	pdata->m_PlayMode = m_PlayMode;
	pdata->m_strFileName = m_strSoundFile.GetBuffer();

	return pdata;
}

void CSound::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_interact.Clear();
}

Ogre::SceneNode* CSound::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);

	m_pSource = static_cast<Ogre::SoundData *>(GetChunk());
	if(m_pSource == NULL) return NULL;

	Ogre::SoundNode* pNode = new Ogre::SoundNode(m_pSource, false);

	m_pNode = pNode;
	m_pNode->setPosition(m_bindInfo.offset);

	m_pNode->stop();
	m_interact.Create(this);
	return m_pNode;

}

void CSound::RefreshNode()
{
	if(m_bVisible)
		return;

	Ogre::SceneNode* poldnode = m_pNode;

	if(poldnode)
	{
		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(poldnode);
	}

	ReleaseNode();

	Ogre::SceneNode* pnode = CreateNode();

	CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);

}

void CSound::UpdateSource()
{
	if(m_pSource)
	{
		m_createInfo.volume = m_iVolumeInt / 10.0f;
		m_pSource->m_CreateInfo = m_createInfo;
		m_pSource->m_strFileName = m_strSoundFile.GetBuffer();
		m_pSource->m_PlayMode = m_PlayMode;
	}

	if(m_pNode && m_bindInfo.bDynamic == false)
	{
		m_pNode->setVolume(m_createInfo.volume);
		m_pNode->setVelocity(m_createInfo.velocity);
		m_pNode->setPosition(GetBindInfo()->offset);
	}
	m_docInfo.Modify();
}

void CSound::DoFrame(Ogre::uint dtick)
{
	m_interact.DoFrame(dtick);
}

CString CSound::GetName()
{
	return m_strName;
}


bool CSound::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;


	m_iVolumeInt = 5;

	m_strName.Format(_T("声音"));
	m_createInfo.isloop = false;
	m_createInfo.maxdistance = 100.0f;
	m_createInfo.mindistance = 10.0f;
	m_createInfo.volume = m_iVolumeInt / 10.0f;
	m_createInfo.velocity = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	m_PlayMode = Ogre::PM_FREE;
	m_bVisible = true;
	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;
}

bool CSound::Destroy()
{
	return true;
}

//bool CSound::IsVisible()
//{
//	return m_bVisible;
//}
//
//void CSound::SetVisible(bool visible)
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

bool CSound::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::SoundData, pChunk))
	{
		pChunk->release();
		return false;
	}
	LoadFromChunk(pChunk);
	m_strName = path;
	pChunk->release();	
	m_bIsNew = false;
	return true;
}

bool CSound::SaveEffect(CString path)
{/*
 Ogre::SoundChunk* pChunk = new Ogre::SoundChunk();
 m_createInfo.fVolume = m_iVolumeInt / 10.0f;

 CString filename = "data\\sounds\\";
 filename.Append(m_strSoundFile);
 pdata->m_strFileName = filename.GetBuffer();
 pdata->m_CreateInfo = m_createInfo;
 pdata->m_PlayMode = m_PlayMode;

 Ogre::StringChunk* pfile = new Ogre::StringChunk("File", pdata->m_strFileName);
 pChunk->AddAttrib(pfile);

 bool ret = Ogre::SaveChunkToFile("chunkdata", path.GetBuffer(), pChunk);
 if(ret)
 {
 m_strName = path;
 }
 delete( pChunk);
 m_bIsNew = false;
 m_docInfo.Save();
 return ret;
 */
	return true;
}

bool CSound::LoadFromChunk(Ogre::Resource* pChunk)
{
 m_strName.Format(_T("声音"));
 Ogre::SoundData* pSound = static_cast<Ogre::SoundData *>(pChunk);

 float tmp = pSound->m_CreateInfo.volume * 10;
 m_iVolumeInt = (int)tmp;
 m_createInfo = pSound->m_CreateInfo;
 m_PlayMode = pSound->m_PlayMode;

 m_strSoundFile = (const char*)(pSound->m_strFileName);

 m_bVisible = true;
 m_bIsNew = false;
 m_docInfo.Open();
 
	return true;
}

void CSound::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CSoundCommand* pSoundCmd = (CSoundCommand*) pCmd;
	bool bRefresh = false;

	if(pSoundCmd->m_strNewSoundFile != m_strSoundFile
		|| pSoundCmd->m_newCreateInfo.isloop != m_createInfo.isloop
		|| pSoundCmd->m_newCreateInfo.maxdistance != m_createInfo.maxdistance
		|| pSoundCmd->m_newCreateInfo.mindistance != m_createInfo.mindistance)
	{
		bRefresh = true;	
	}

	m_strSoundFile = pSoundCmd->m_strNewSoundFile;
	m_createInfo = pSoundCmd->m_newCreateInfo;
	m_iVolumeInt = pSoundCmd->m_iNewVol;
	m_createInfo.volume = m_iVolumeInt / 10.0f;

	m_pNode->setPosition(m_bindInfo.offset);

	if(bRefresh)
	{
		CEffectMgr::GetInstance()->RefreshEffect(this); //RefreshNode();
	}
	else
	{
		UpdateSource();
	}

	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(true);
}



CEffectCommand* CSound::BeginRecordCommand()
{
	CSoundCommand* pRet = new CSoundCommand();
	pRet->m_pTarget = this;
	pRet->m_strNewSoundFile = m_strSoundFile;
	pRet->m_newCreateInfo = m_createInfo;
	pRet->m_iNewVol = m_iVolumeInt;
	return pRet;
}

void CSound::EndRecordCommand(CEffectCommand* psrccmd)
{
	CSoundCommand* pCmd = static_cast<CSoundCommand*>(psrccmd);
	pCmd->m_strOldSoundFile = m_strSoundFile;
	pCmd->m_oldCreateInfo = m_createInfo;
	pCmd->m_iOldVol = m_iVolumeInt;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pCmd);
}

BIND_INFO* CSound::GetBindInfo()
{
	return &m_bindInfo;
}

void CSound::SetBindInfo(BIND_INFO* bindinfo) 
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CSound::GetType()
{
	return EFFECT_SOUND;
}

// -----------CLightCommand---------------------- [12/14/2007]

CEffectUnit* CSoundCommand::GetTarget()
{
	return m_pTarget;
}

bool CSoundCommand::NeedRefresh()
{
	//if(m_oldColor != m_newColor || m_oldType != m_newType)
	//return true;
	return false;
}

CEffectCommand* CSoundCommand::GetInversCommand()
{
	CSoundCommand* pRet = new CSoundCommand();
	pRet->m_pTarget = m_pTarget;

	pRet->m_oldBindInfo = m_newBindInfo;
	pRet->m_oldCreateInfo = m_newCreateInfo;
	pRet->m_strOldSoundFile = m_strNewSoundFile;
	pRet->m_iOldVol = m_iNewVol;

	pRet->m_newBindInfo = m_oldBindInfo;
	pRet->m_newCreateInfo = m_oldCreateInfo ;
	pRet->m_strNewSoundFile = m_strOldSoundFile ;
	pRet->m_iNewVol = m_iOldVol;

	return pRet;
}

bool CSoundCommand::IsIdentical()
{
	if(	   m_strOldSoundFile		== m_strNewSoundFile 
		&& m_oldCreateInfo.isloop	== m_newCreateInfo.isloop
		&& m_oldCreateInfo.maxdistance	== m_newCreateInfo.maxdistance
		&& m_oldCreateInfo.mindistance	== m_newCreateInfo.mindistance
		&& m_oldCreateInfo.volume	== m_newCreateInfo.volume 
		&& m_oldCreateInfo.pos	== m_newCreateInfo.pos 
		&& m_oldCreateInfo.velocity	== m_newCreateInfo.velocity 
		&& m_oldBindInfo == m_newBindInfo
		&& m_iOldVol == m_iNewVol)
	{
		return true;
	}
	return false;
}

bool CSoundCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	CSoundCommand* pSoundCmd = (CSoundCommand*) pCmd;
	if(	   m_strOldSoundFile		== pSoundCmd->m_strNewSoundFile 
		&& m_oldCreateInfo.isloop	== pSoundCmd->m_oldCreateInfo.isloop
		&& m_oldCreateInfo.maxdistance	== pSoundCmd->m_oldCreateInfo.maxdistance
		&& m_oldCreateInfo.mindistance	== pSoundCmd->m_oldCreateInfo.mindistance
		&& m_oldCreateInfo.volume	== pSoundCmd->m_oldCreateInfo.volume 
		&& m_oldCreateInfo.pos	== pSoundCmd->m_oldCreateInfo.pos 
		&& m_oldCreateInfo.velocity	== pSoundCmd->m_oldCreateInfo.velocity 
		&& m_oldBindInfo.boneid == pSoundCmd->m_oldBindInfo.boneid
		&& m_iOldVol == pSoundCmd->m_iOldVol)
	{
		m_oldBindInfo.offset = pSoundCmd->m_oldBindInfo.offset;
		return true;
	}
	return false;
}

