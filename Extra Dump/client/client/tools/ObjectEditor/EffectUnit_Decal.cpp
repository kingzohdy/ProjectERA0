
#include "stdafx.h"
#include "EffectUnit_Decal.h"
#include "EffectManager.h"
#include "EditorManager.h"
#include "EffectUnitEditBar_Decal.h"

#include "OgreDecal.h"
#include "OgreBillboard.h"
#include "OgreEntity.h"
#include "OgreTexture.h"
#include "OgreResourceManager.h"

CDecalInteract::CDecalInteract()
{
	m_pIdn = NULL;
}
CDecalInteract::~CDecalInteract()
{
	Clear();
}

void CDecalInteract::Clear()
{
	CInteract::Clear();
	if(m_pIdn)
	{
		m_pIdn->detachFromScene();
		OGRE_RELEASE(m_pIdn);
	}
}

void CDecalInteract::Create(CEffectUnit* pTarget)
{
	CInteract::Create(pTarget);
	if(m_pIdn)
	{
		m_pIdn->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
	}
}



void CDecalInteract::DoFrame(Ogre::uint dtick)
{
	CInteract::DoFrame(dtick);
	if(m_pIdn && !m_pTarget->GetBindInfo()->bDynamic)
	{
		m_pIdn->setPosition(Ogre::WorldPos(m_pTarget->GetNode()->getWorldPosition()));
	}

}

bool CDecalInteract::Pick(float x, float y)
{
	return false;
}


// --------------------------------------------------------------- [4/17/2008]

CDecal::CDecal(void)
{
	m_bindInfo.boneid = 0;
	m_bVisible = true;
	m_pNode = NULL;
	m_pSource = NULL;
	m_strLastTexture = "";
	m_interact.Clear();
	m_desc.m_BlendMode = 1;
	m_bSeeMask = true;
}

CDecal::~CDecal(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}

Ogre::SceneNode* CDecal::GetNode()
{
	return m_pNode;
}

void CDecal::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_interact.Clear();
}

Ogre::SceneNode* CDecal::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	m_interact.Create(this);

	m_pSource = (Ogre::DecalData*)GetChunk();
	if(m_pSource == NULL) return NULL;

	Ogre::DecalNode* pNode = new Ogre::DecalNode(m_pSource);

	m_pNode = pNode;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setScale(m_bindInfo.scale);

	m_strLastTexture = m_strTexture;

	return m_pNode;

}

void CDecal::RefreshNode()
{
	if(!m_bVisible)
		return ;

	Ogre::SceneNode* poldnode = m_pNode;
	if(poldnode)
	{
		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(poldnode);
	}

	ReleaseNode();

	Ogre::SceneNode* pnode = CreateNode();

	CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);
}

void CDecal::UpdateSource()
{
	if (!m_pNode || !m_pSource)
		return;
	Ogre::Resource* pOldTex = m_pSource->m_pTextureRes;
	m_pSource->m_Desc = m_desc;

	SetKfToSource(m_pSource);

	if(m_strLastTexture != m_strTexture)
	{
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			OGRE_RELEASE(pOldTex);
			m_pSource->m_pTextureRes = static_cast<Ogre::TextureData*>(pNewTex);
			m_strLastTexture = m_strTexture;
		}
	}
	if( !m_strMaskTexture.IsEmpty() )
	{
		Ogre::Resource* pMaskTexRes = Ogre::ResourceManager::getSingleton().blockLoad( m_strMaskTexture.GetBuffer() );
		if( pMaskTexRes != NULL )
		{
			OGRE_RELEASE(m_pSource->m_pMaskTextureRes);
			m_pSource->m_pMaskTextureRes = static_cast<Ogre::TextureData*>( pMaskTexRes );
		}
	}
	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		GetNode()->setPosition(GetBindInfo()->offset);
		GetNode()->setScale(GetBindInfo()->scale);
	}
	m_docInfo.Modify();
}

CBCGPDockingControlBar* CDecal::CreateBar(CBCGPFrameWnd* pParent)
{
	CDecalBar* pBar = new CDecalBar();
	return pBar;
}

Ogre::Resource* CDecal::GetChunk()
{
	Ogre::DecalData* pDecalChunk = new Ogre::DecalData();
	pDecalChunk->m_Desc = m_desc;
	SetKfToSource(pDecalChunk);

	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	if(pTex == NULL)
	{
		OGRE_RELEASE( pDecalChunk);
		return NULL;
	}
	pDecalChunk->m_pTextureRes = static_cast<Ogre::TextureData*>(pTex);

	Ogre::Resource* pMaskTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
	if(pMaskTex != NULL)
	{
		pDecalChunk->m_pMaskTextureRes = static_cast<Ogre::TextureData*>(pMaskTex);
	}

	return pDecalChunk;
}

float CDecal::GetRange()
{
	return 30.0f;
}

CString CDecal::GetName()
{
	return m_strName;
}

bool CDecal::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;
	m_strName.Format(_T("µØÐÎÌù»¨"));

	m_desc.m_TexRows = 1;
	m_desc.m_TexCols = 1;
	m_desc.m_TimeSlot = 0.0f;

	DCL_FRAME df;
	df.lTick = 0;
	df.size = 20.0f;
	df.aspect = 1.0f;
	df.alpha = 1.0f;
	df.color = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	m_KeyFrames.push_back(df);

	m_strTexture = DEF_FX_TEXTURE;
	m_strMaskTexture = _T("");

	m_bVisible = true;
	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;
}


void CDecal::DoFrame(Ogre::uint dtick) 
{
	m_interact.DoFrame(dtick);
}

bool CDecal::Destroy()
{
	return true;
}

bool CDecal::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::DecalData, pChunk))
	{
		OGRE_RELEASE( pChunk);
		return false;
	}
	LoadFromChunk(pChunk);
	m_strName = path;
	OGRE_RELEASE( pChunk);	
	m_bIsNew = false;
	m_docInfo.Open();
	return true;
}

bool CDecal::SaveEffect(CString path)
{
	Ogre::DecalData* pDecalChunk = new Ogre::DecalData();

	pDecalChunk->m_Desc = m_desc;
	SetKfToSource(pDecalChunk);
	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	if(pTex == NULL)
	{
		OGRE_RELEASE( pDecalChunk);
		return false;
	}
	pDecalChunk->m_pTextureRes = static_cast<Ogre::TextureData *>(pTex);

	Ogre::Resource* pMaskTex = Ogre::ResourceManager::getSingleton().blockLoad( m_strMaskTexture.GetBuffer() );
	if( pMaskTex != NULL )
	{
		pDecalChunk->m_pMaskTextureRes = static_cast<Ogre::TextureData *>(pMaskTex);
	}

	pDecalChunk->save(path.GetBuffer());
	m_strName = path;
	OGRE_RELEASE (pDecalChunk);
	m_bIsNew = false;
	m_docInfo.Save();
	return true;
}

bool CDecal::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::DecalData* pDecalChunk = (Ogre::DecalData*) pChunk;
	m_desc = pDecalChunk->m_Desc;
	SetKfFromSource(pDecalChunk);

	m_strTexture = pDecalChunk->m_pTextureRes->getResPath();

	if(pDecalChunk->m_pMaskTextureRes) m_strMaskTexture = pDecalChunk->m_pMaskTextureRes->getResPath();
	else m_strMaskTexture = "";

	m_bIsNew = false;
	m_docInfo.Open();
	return true;
}

BIND_INFO* CDecal::GetBindInfo()
{
	return &m_bindInfo;
}

void CDecal::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CDecal::GetType()
{
	return EFFECT_DECAL;
}
void CDecal::SetKfToSource(Ogre::DecalData* pSource)
{
	Ogre::KeyFrameArray<float> initkf;
	Ogre::KeyFrameArray<Ogre::ColourValue> initkfc;

	pSource->m_SizeKeyFrame = initkf;
	pSource->m_AspectKeyFrame = initkf;
	pSource->m_ColorKeyFrame = initkfc;

	pSource->m_UScaleKeyFrame = initkf;
	pSource->m_VScaleKeyFrame = initkf;
	pSource->m_UOffKeyFrame = initkf;
	pSource->m_VOffKeyFrame = initkf;
	pSource->m_UVRotateKeyFrame = initkf;
	pSource->m_SeqFrameKeyFrame = initkf;

	pSource->m_MaskUScaleKeyFrame = initkf;
	pSource->m_MaskVScaleKeyFrame = initkf;
	pSource->m_MaskUOffKeyFrame = initkf;
	pSource->m_MaskVOffKeyFrame = initkf;
	pSource->m_MaskUVRotateKeyFrame = initkf;
	pSource->m_MaskSeqFrameKeyFrame = initkf;

	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc;

		kf.tick = m_KeyFrames[i].lTick;
		kfc.tick = m_KeyFrames[i].lTick;

		kf.data = m_KeyFrames[i].size;
		pSource->m_SizeKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].aspect;
		pSource->m_AspectKeyFrame.m_Array.push_back(kf);

		kfc.data = m_KeyFrames[i].color;
		kfc.data.a = m_KeyFrames[i].alpha;
		pSource->m_ColorKeyFrame.m_Array.push_back(kfc);

		kf.data = m_KeyFrames[i].fUScale;
		pSource->m_UScaleKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fVScale;
		pSource->m_VScaleKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fUOff;
		pSource->m_UOffKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fVOff;
		pSource->m_VOffKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fUVRotate;
		pSource->m_UVRotateKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].nSequenceFrame;
		pSource->m_SeqFrameKeyFrame.m_Array.push_back(kf);

		//ÃÉ°åÌùÍ¼
		kf.data = m_KeyFrames[i].fMaskUScale;
		pSource->m_MaskUScaleKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fMaskVScale;
		pSource->m_MaskVScaleKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fMaskUOff;
		pSource->m_MaskUOffKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fMaskVOff;
		pSource->m_MaskVOffKeyFrame.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fMaskUVRotate;
		pSource->m_MaskUVRotateKeyFrame.m_Array.push_back(kf);

		kf.data = 0;
		pSource->m_MaskSeqFrameKeyFrame.m_Array.push_back(kf);
	}
}

void CDecal::SetKfFromSource(Ogre::DecalData* pSource)
{
	m_KeyFrames.clear();

	for(size_t i=0; i<pSource->m_SizeKeyFrame.getNumKey(); i++)
	{
		DCL_FRAME frame;
		frame.lTick			= pSource->m_SizeKeyFrame.m_Array[i].tick;
		frame.size			= pSource->m_SizeKeyFrame.m_Array[i].data;
		frame.aspect        = pSource->m_AspectKeyFrame.m_Array[i].data;
		frame.color         = pSource->m_ColorKeyFrame.m_Array[i].data;
		frame.alpha         = pSource->m_ColorKeyFrame.m_Array[i].data.a;

		frame.fUScale		= pSource->m_UScaleKeyFrame.m_Array[i].data;
		frame.fVScale		= pSource->m_VScaleKeyFrame.m_Array[i].data;
		frame.fUOff			= pSource->m_UOffKeyFrame.m_Array[i].data;
		frame.fVOff			= pSource->m_VOffKeyFrame.m_Array[i].data;
		frame.fUVRotate		= pSource->m_UVRotateKeyFrame.m_Array[i].data;
		frame.nSequenceFrame	= pSource->m_SeqFrameKeyFrame.m_Array[i].data;

		//ÃÉ°åÌùÍ¼
		frame.fMaskUScale		= pSource->m_MaskUScaleKeyFrame.m_Array[i].data;
		frame.fMaskVScale		= pSource->m_MaskVScaleKeyFrame.m_Array[i].data;
		frame.fMaskUOff			= pSource->m_MaskUOffKeyFrame.m_Array[i].data;
		frame.fMaskVOff			= pSource->m_MaskVOffKeyFrame.m_Array[i].data;
		frame.fMaskUVRotate		= pSource->m_MaskUVRotateKeyFrame.m_Array[i].data;

		m_KeyFrames.push_back(frame);
	}

}


void CDecal::ExecuteCommand(CEffectCommand* pCmd)
{
	if(pCmd->GetTarget() != this)
		return;

	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CDecalCommand* pDclCmd = (CDecalCommand*) pCmd;
	bool bRefresh = false;

	if(	pDclCmd->m_newTex != m_strTexture)
	{
		bRefresh = true;	
	}

	m_desc = pDclCmd->m_newDesc;
	m_strTexture = pDclCmd->m_newTex;

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

CEffectCommand* CDecal::BeginRecordCommand()
{
	CDecalCommand* pUndoCmd = new CDecalCommand();

	pUndoCmd->m_newDesc = m_desc;
	pUndoCmd->m_newTex = m_strTexture;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd;
}

void CDecal::EndRecordCommand(CEffectCommand* pcmd)
{
	CDecalCommand* pUndoCmd = static_cast<CDecalCommand*>(pcmd);
	pUndoCmd->m_oldDesc = m_desc;
	pUndoCmd->m_oldTex = m_strTexture;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}

// ---------------------------------------------------- [4/17/2008]

CEffectUnit* CDecalCommand::GetTarget()
{
	return m_pTarget;
}

bool CDecalCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CDecalCommand::GetInversCommand()
{
	CDecalCommand* pCmd = new CDecalCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newDesc = m_oldDesc;
	pCmd->m_oldDesc = m_newDesc;

	pCmd->m_newTex = m_oldTex;
	pCmd->m_oldTex = m_newTex;

	pCmd->m_newKf = m_oldKf;
	pCmd->m_oldKf = m_newKf;


	pCmd->m_newBindInfo = m_oldBindInfo;
	pCmd->m_oldBindInfo = m_newBindInfo;
	return pCmd;
}

bool CDecalCommand::IsIdentical()
{
	if(memcmp(&m_newDesc, &m_oldDesc, sizeof(Ogre::DecalDesc)) != 0)
		return false;

	if(m_oldTex != m_newTex)
		return false;
	if(m_oldKf.size() != m_newKf.size())
		return false;

	for(int i = 0; i < (int) m_oldKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &m_newKf[i], sizeof(CDecal::DCL_FRAME)) != 0)
			return false;
	}

	return true;
}



bool CDecalCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	if(pCmd->GetTarget() != m_pTarget)
		return false;
	CDecalCommand* pMyCmd = (CDecalCommand*) pCmd;

	if(bForceMerge == true)
	{
		m_oldDesc = pMyCmd->m_oldDesc;
		m_oldKf = pMyCmd->m_oldKf;
		m_oldTex = pMyCmd->m_oldTex;


		return true;
	}
	return false;
}