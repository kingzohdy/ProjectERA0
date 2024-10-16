
#include "stdafx.h"
#include "EffectUnit_Ribbon.h"
#include "EffectManager.h"
#include "EffectUnitEditBar_Ribbon.h"

#include "OgreRibbonEmitter.h"
#include "OgreEntity.h"
#include "OgreTexture.h"
#include "OgreResourceManager.h"

CRibbon::CRibbon(void)
{
	m_pNode = NULL;
	m_pSource = NULL;
}

CRibbon::~CRibbon(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}
//EFFECT_RIBBON

Ogre::SceneNode* CRibbon::GetNode()
{
	return m_pNode;
}

float CRibbon::GetRange()
{
	return (m_KeyFrames[0].above + m_KeyFrames[0].below) / 4;
}

void CRibbon::DoFrame(Ogre::uint dtick) 
{
	m_interact.DoFrame(dtick);
}

void CRibbon::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_pNode = NULL;


	m_interact.Clear();
}

Ogre::SceneNode* CRibbon::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	m_interact.Create(this);

	m_pSource = (Ogre::RibbonEmitterData*)GetChunk();
	if(m_pSource == NULL)return NULL;

	Ogre::RibbonEmitter* pNode = new Ogre::RibbonEmitter(m_pSource);

	m_pNode = pNode;
	m_strLastTexture = m_strTexture;
	m_strLastMaskTexture = m_strMaskTexture;

	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);
	return m_pNode;

}

void CRibbon::RefreshNode()
{
	if(!m_bVisible)
		return ;

	Ogre::SceneNode* poldnode = m_pNode;
	//CEnginAgent::GetInstance()->m_pShowEngine->GetMap()->GetRoom()->DetachNode(poldnode);

	//ReleaseNode();
	//Ogre::SceneNode* pnode = CreateNode();

	//CEnginAgent::GetInstance()->m_pShowEngine->GetMap()->GetRoom()->AttachNode(pnode);

	if(poldnode)
	{
		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(poldnode);
	}

	ReleaseNode();

	Ogre::SceneNode* pnode = CreateNode();

	CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);

}

void CRibbon::UpdateSource()
{
	if (!m_pSource)
		return;
	//	Ogre::HANDLE_T hOldTex = m_pSource->m_pTextureRes;
	Ogre::Resource* pOldTex = m_pSource->m_pTextureRes;
	SetKfToSource(m_pSource);
	m_pSource->m_Desc = m_desc;
	m_pSource->m_DescEx = m_descEx;

	if(m_strLastTexture != m_strTexture)
	{
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			OGRE_RELEASE(m_pSource->m_pTextureRes);
			m_pSource->m_pTextureRes = static_cast<Ogre::TextureData *>(pNewTex);
			m_strLastTexture = m_strTexture;
		}
	}

	if(m_strLastMaskTexture != m_strMaskTexture)
	{
		if(m_strMaskTexture == "")
		{
			OGRE_RELEASE(m_pSource->m_pTextureRes);
		}
		else
		{
			Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
			OGRE_RELEASE(m_pSource->m_pTextureRes);
			m_pSource->m_pMaskTextureRes = static_cast<Ogre::TextureData *>(pNewTex);
		}
		m_strLastMaskTexture = m_strMaskTexture;
	}

	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		GetNode()->setPosition(GetBindInfo()->offset);
		GetNode()->setRotation(GetBindInfo()->rotate);
		GetNode()->setScale(GetBindInfo()->scale);
	}
	m_docInfo.Modify();
}

CBCGPDockingControlBar* CRibbon::CreateBar(CBCGPFrameWnd* pParent)
{
	CRibbonBar* pBar = new CRibbonBar();
	//static int id = 0;
	//pBar->Create(NULL, pParent, CRect (0, 0,  250,600),
	//	TRUE, 0,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	//pBar->SetEditTarget(this);
	return pBar;
}

Ogre::Resource* CRibbon::GetChunk()
{
	Ogre::RibbonEmitterData* pRibChu = new Ogre::RibbonEmitterData();
	pRibChu->m_Desc = m_desc;
	pRibChu->m_DescEx = m_descEx;

	SetKfToSource(pRibChu);

	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(
		m_strTexture.GetBuffer());
	if(pTex == NULL)
	{
		pTex = Ogre::ResourceManager::getSingleton().blockLoad(DEF_FX_TEXTURE);
	}
	pRibChu->m_pTextureRes = static_cast<Ogre::TextureData *>(pTex);

	if(m_strMaskTexture == "")
	{
		pRibChu->m_pMaskTextureRes = NULL;
	}
	else
	{
		pRibChu->m_pMaskTextureRes = static_cast<Ogre::TextureData *>(Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer()));
	}

	return pRibChu;
}


CString CRibbon::GetName()
{
	return m_strName;
}

void CRibbon::SetName(CString strName)
{
	m_strName = strName;
}

bool CRibbon::Create()
{
	//std::vector<RIB_FRAME> m_KeyFrames;
	//Ogre::RibbonDesc m_desc;
	//BIND_INFO m_bindInfo;
	//CString m_strName;
	//CString m_strTexture;
	//bool m_bIsVisible;

	m_desc.m_BlendMode = 0;
	m_desc.m_EmitteRate = 100.0f;
	m_desc.m_LifeSpan = 0.7f;
	m_desc.m_MaxSegsOrMagic = RIBBON_MAGIC;

	m_descEx.m_bDistort = false;
	m_descEx.m_MaxSegsNew = 100;
	m_descEx.m_iUVAddressMode = Ogre::UVADDR_WRAP;

	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;

	m_strName.Format(_T("Ìõ´ø"));

	m_strTexture = DEF_FX_TEXTURE;
	m_bVisible = true;
	m_descEx.m_LerpSeg = 5;

	m_descEx.m_iMaskUVAddrMode = Ogre::UVADDR_CLAMP;

	m_strMaskTexture = "";

	RIB_FRAME firstFrame;
	firstFrame.lTick = 0;
	m_KeyFrames.clear();
	m_KeyFrames.push_back(firstFrame);

	m_Template.m_dots.clear();
	m_Template.m_lines.clear();
	m_Template.m_vdata.clear();

	m_Template.m_dots.push_back(Ogre::Vector2(50.0f, 0.0f));
	m_Template.m_dots.push_back(Ogre::Vector2(-50.0f, 0.0f));

	m_Template.m_lines.push_back(0);
	m_Template.m_lines.push_back(1);

	m_Template.m_vdata.push_back(0);
	m_Template.m_vdata.push_back(1);

	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;
}

bool CRibbon::Destroy()
{
	return true;
}

//bool CRibbon::IsVisible()
//{
//	return m_bVisible;
//}
//
//void CRibbon::SetVisible(bool visible)
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


bool CRibbon::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::RibbonEmitterData, pChunk))
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

bool CRibbon::SaveEffect(CString path)
{
	Ogre::RibbonEmitterData* pRibChu = new Ogre::RibbonEmitterData();

	pRibChu->m_Desc = m_desc;
	pRibChu->m_DescEx = m_descEx;
	SetKfToSource(pRibChu);
	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	if(pTex == NULL)
	{
		OGRE_RELEASE(pRibChu);
		return false;
	}
	pRibChu->m_pTextureRes = static_cast<Ogre::TextureData *>(pTex);

	if(m_strMaskTexture == "")
		pRibChu->m_pMaskTextureRes = NULL;
	else
		pRibChu->m_pMaskTextureRes = static_cast<Ogre::TextureData *>(Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer()));

	pRibChu->save(path.GetBuffer());
	m_strName = path;
	pRibChu->release();
	m_bIsNew = false;
	m_docInfo.Save();

	return true;
}

bool CRibbon::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::RibbonEmitterData* pRibChu = (Ogre::RibbonEmitterData*) pChunk;
	m_desc = pRibChu->m_Desc;
	m_descEx = pRibChu->m_DescEx;
	SetKfFromSource(pRibChu);

	m_strTexture = pRibChu->m_pTextureRes->getResPath();

	if(pRibChu->m_pMaskTextureRes) m_strMaskTexture = pRibChu->m_pMaskTextureRes->getResPath();
	else m_strMaskTexture = "";

	m_bIsNew = false;
	m_docInfo.Open();
	return true;
}

BIND_INFO* CRibbon::GetBindInfo()
{
	return &m_bindInfo;
}

void CRibbon::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CRibbon::GetType()
{
	return EFFECT_RIBBON;
}

void CRibbon::SetKfToSource(Ogre::RibbonEmitterData* pSource)
{
	pSource->m_Template = m_Template;

	Ogre::KeyFrameArray<float> initkf;
	Ogre::KeyFrameArray<Ogre::ColourValue> initkfc;
	pSource->m_Above = initkf;
	pSource->m_Below = initkf;
	pSource->m_Color = initkfc;

	pSource->m_UVRotateKeyFrame = initkf;
	pSource->m_UScaleKeyFrame = initkf;
	pSource->m_VScaleKeyFrame = initkf;
	pSource->m_UOffsetKeyFrame = initkf;
	pSource->m_VOffsetKeyFrame = initkf;

	pSource->m_MaskUVRotateKeyFrame = initkf;
	pSource->m_MaskUScaleKeyFrame = initkf;
	pSource->m_MaskVScaleKeyFrame = initkf;
	pSource->m_MaskUOffsetKeyFrame = initkf;
	pSource->m_MaskVOffsetKeyFrame = initkf;



	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc;

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].above;
		pSource->m_Above.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].below;
		pSource->m_Below.m_Array.push_back(kf);

		kfc.tick = m_KeyFrames[i].lTick;
		kfc.data = m_KeyFrames[i].color;
		pSource->m_Color.m_Array.push_back(kfc);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].uvrot;
		pSource->m_UVRotateKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].uscale;
		pSource->m_UScaleKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].vscale;
		pSource->m_VScaleKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].uoffset ;
		pSource->m_UOffsetKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].voffset;
		pSource->m_VOffsetKeyFrame.m_Array.push_back(kf);



		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].maskuvrot;
		pSource->m_MaskUVRotateKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].maskuscale;
		pSource->m_MaskUScaleKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].maskvscale;
		pSource->m_MaskVScaleKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].maskuoffset ;
		pSource->m_MaskUOffsetKeyFrame.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].maskvoffset;
		pSource->m_MaskVOffsetKeyFrame.m_Array.push_back(kf);

	}
}

void CRibbon::SetKfFromSource(Ogre::RibbonEmitterData* pSource)
{
	m_KeyFrames.clear();
	m_Template = pSource->m_Template;

	for(int i = 0; i < (int)pSource->m_Above.m_Array.size(); i++)
	{
		RIB_FRAME frame;
		frame.lTick			= pSource->m_Above.m_Array[i].tick;

		frame.above			= pSource->m_Above.m_Array[i].data;
		frame.below			= pSource->m_Below.m_Array[i].data;
		frame.color			= pSource->m_Color.m_Array[i].data;

		frame.uvrot			= pSource->m_UVRotateKeyFrame.m_Array[i].data;
		frame.uscale		= pSource->m_UScaleKeyFrame.m_Array[i].data;
		frame.vscale		= pSource->m_VScaleKeyFrame.m_Array[i].data;
		frame.uoffset		= pSource->m_UOffsetKeyFrame.m_Array[i].data;
		frame.voffset		= pSource->m_VOffsetKeyFrame.m_Array[i].data;


		frame.maskuvrot			= pSource->m_MaskUVRotateKeyFrame.m_Array[i].data;
		frame.maskuscale		= pSource->m_MaskUScaleKeyFrame.m_Array[i].data;
		frame.maskvscale		= pSource->m_MaskVScaleKeyFrame.m_Array[i].data;
		frame.maskuoffset		= pSource->m_MaskUOffsetKeyFrame.m_Array[i].data;
		frame.maskvoffset		= pSource->m_MaskVOffsetKeyFrame.m_Array[i].data;

		m_KeyFrames.push_back(frame);
	}
	if(m_KeyFrames.size() == 0)
		MessageBox(NULL, "0 frame when SetKFFromSource", NULL,0);

	if(m_descEx.m_LerpSeg <= 2)
		m_descEx.m_LerpSeg = 2;
	if(m_descEx.m_LerpSeg >= 100)
		m_descEx.m_LerpSeg = 100;
}


void CRibbon::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CRibbionCommand* pRbnCmd = (CRibbionCommand*) pCmd;
	bool bRefresh = false;

	if(pRbnCmd->m_newDesc.m_BlendMode != m_desc.m_BlendMode ||
		pRbnCmd->m_newTex != m_strTexture)
	{
		bRefresh = true;	
	}

	m_desc = pRbnCmd->m_newDesc;
	m_strTexture = pRbnCmd->m_newTex;
	m_KeyFrames = pRbnCmd->m_newKf;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);
	if(bRefresh)
	{
		CEffectMgr::GetInstance()->RefreshEffect(this);//RefreshNode();
	}
	else
	{
		UpdateSource();
	}

	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(true);

}
CEffectCommand* CRibbon::BeginRecordCommand()
{
	CRibbionCommand* pUndoCmd = new CRibbionCommand();

	pUndoCmd->m_newBind = m_bindInfo;
	pUndoCmd->m_newDesc = m_desc;
	pUndoCmd->m_newKf = m_KeyFrames;
	pUndoCmd->m_newTex = m_strTexture;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd;
}
void CRibbon::EndRecordCommand(CEffectCommand* pcmd)
{
	CRibbionCommand* pUndoCmd = static_cast<CRibbionCommand*>(pcmd);
	pUndoCmd->m_oldBind = m_bindInfo;
	pUndoCmd->m_oldDesc = m_desc;
	pUndoCmd->m_oldKf = m_KeyFrames;
	pUndoCmd->m_oldTex = m_strTexture;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}
// --------------------------------------------------------------------- [11/21/2007]

CEffectUnit* CRibbionCommand::GetTarget()
{
	return m_pTarget;
}

bool CRibbionCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CRibbionCommand::GetInversCommand()
{
	CRibbionCommand* pCmd = new CRibbionCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newBind = m_oldBind;
	pCmd->m_oldBind = m_newBind;

	pCmd->m_newDesc = m_oldDesc;
	pCmd->m_oldDesc = m_newDesc;

	pCmd->m_newKf = m_oldKf;
	pCmd->m_oldKf = m_newKf;

	pCmd->m_newTex = m_oldTex;
	pCmd->m_oldTex = m_newTex;

	return pCmd;
}

bool CRibbionCommand::IsIdentical()
{
	if(memcmp(&m_newDesc, &m_oldDesc, sizeof(Ogre::RibbonDesc)) != 0)
		return false;

	if(m_oldTex != m_newTex)
		return false;

	if(m_oldKf.size() != m_newKf.size())
		return false;

	for(int i = 0; i < (int) m_oldKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &m_newKf[i], sizeof(CRibbon::RIB_FRAME)) != 0)
			return false;
	}

	return true;
}

bool CRibbionCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	return false;
}