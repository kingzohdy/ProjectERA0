
#include "stdafx.h"
#include "EffectUnit_Beam.h"
#include "EffectManager.h"
#include "RefNodeManager.h"
#include "EffectUnitEditBar_Beam.h"

#include "OgreBeamEmitter.h"
#include "OgreEntity.h"
#include "OgreResourceManager.h"
#include "OgreTexture.h"

CBeam::CBeam(void)
{
	m_bindInfo.boneid = 0;
	m_bVisible = true;
	m_pNode = NULL;
	m_pSource = NULL;
	m_strLastTexture = "";
	m_interact.Clear();
}

CBeam::~CBeam(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}


Ogre::SceneNode* CBeam::GetNode()
{
	//return NULL;//!!!!!!!!!!
	return m_pNode;
}

void CBeam::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_pNode = NULL;

	m_interact.Clear();
}

Ogre::SceneNode* CBeam::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	m_interact.Create(this);

	m_pSource = (Ogre::BeamEmitterData*)GetChunk();
	if(m_pSource == NULL) return NULL;
	Ogre::BeamEmitter* pNode = new Ogre::BeamEmitter(m_pSource);

	m_pNode = pNode;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);
	m_strLastTexture = m_strTexture;

	m_strLastMaskTexture = m_strLastTexture;
	return m_pNode;
}

void CBeam::RefreshNode()
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

bool CBeam::ValidateDesc(Ogre::PBeamEmitterDesc& desc)
{
	return true;
}

void CBeam::UpdateSource()
{
	if (!m_pSource)
		return;
	Ogre::Resource* pOldTex = m_pSource->m_pTexture;
	SetKfToSource(m_pSource);

	m_pSource->m_Desc = m_desc;


	if(m_strLastTexture != m_strTexture)
	{
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			if(pOldTex != NULL)	pOldTex->release();
			m_pSource->m_pTexture = static_cast<Ogre::TextureData *>(pNewTex);
			m_strLastTexture = m_strTexture;
		}
	}

	if(m_strLastMaskTexture != m_strMaskTexture)
	{
		if(m_strMaskTexture == "")
		{
			OGRE_RELEASE(m_pSource->m_pMaskTexture);
		}
		else
		{
			Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
			OGRE_RELEASE(m_pSource->m_pMaskTexture);
			assert(IS_KIND_OF(Ogre::TextureData, pNewTex));
			m_pSource->m_pMaskTexture = static_cast<Ogre::TextureData *>(pNewTex);
		}
		m_strLastMaskTexture = m_strMaskTexture;
	}

	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		m_pNode->setPosition(GetBindInfo()->offset);
		m_pNode->setRotation(GetBindInfo()->rotate);
		m_pNode->setScale(GetBindInfo()->scale);
	}
	m_docInfo.Modify();
}

CBCGPDockingControlBar* CBeam::CreateBar(CBCGPFrameWnd* pParent)
{
	CBeamBar* pBar = new CBeamBar();
	return pBar;
}

Ogre::Resource* CBeam::GetChunk()
{
	if(ValidateDesc(m_desc) == false)
		return NULL;

	Ogre::BeamEmitterData* pBeamChunk = new Ogre::BeamEmitterData();
	SetKfToSource(pBeamChunk);

	if(m_strMaskTexture == "")
	{
		pBeamChunk->m_pMaskTexture = NULL;
	}
	else
	{
		Ogre::Resource *pres = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
		assert(IS_KIND_OF(Ogre::TextureData, pres));
		pBeamChunk->m_pMaskTexture = static_cast<Ogre::TextureData *>(pres);
	}

	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	if(pTex == NULL)
	{
		pTex = Ogre::ResourceManager::getSingleton().blockLoad(DEF_FX_TEXTURE);
	}
	pBeamChunk->m_pTexture = static_cast<Ogre::TextureData *>(pTex);

	return pBeamChunk;
}
float CBeam::GetRange()
{
	return 20.0f;
}

CString CBeam::GetName()
{
	return m_strName;
}

bool CBeam::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;
	m_strName.Format(_T("²¨´ø"));

	m_desc.m_BlendType = 0;
	m_desc.m_MaxParticles = 5;
	m_desc.m_HighFrequencyPoints = 5;
	m_desc.m_HighFrequencyNoiseRange = Ogre::Vector2(-5.0f, 5.0f);

	m_desc.m_LowFrequencyPoints = 3;
	m_desc.m_WaveTypeY = Ogre::BWT_RANDOM;
	m_desc.m_LFYParam1 = 1.0f;
	m_desc.m_LFYParam2 = 1.0f;
	m_desc.m_LFYParam3 = 1.0f;

	m_desc.m_WaveTypeZ = Ogre::BWT_RANDOM;
	m_desc.m_LFZParam1 = 1.0f;
	m_desc.m_LFZParam2 = 1.0f;
	m_desc.m_LFZParam3 = 1.0f;

	m_desc.m_bEmitOnce = false;
	m_desc.m_bDistort = false;
	m_desc.m_bRandomTile = true;
	m_desc.m_FrameTime = 0.0f;

	m_desc.m_iUVAddressMode = Ogre::UVADDR_WRAP;
	m_desc.m_UVRotate = 0.0f;
	m_desc.m_UVScale = 1.0f;
	m_desc.m_TexRows = 1;
	m_desc.m_TexCols = 1;
	m_desc.m_USpeed = 0.0f;
	m_desc.m_VSpeed = 0.0f;
	m_desc.m_UOgnOff = 0.0f;
	m_desc.m_VOgnOff = 0.0f;

	m_desc.m_iMaskUVAddressMode = Ogre::UVADDR_CLAMP;
	m_desc.m_MaskUVRotate = 0.0f;
	m_desc.m_MaskUVScale = 1.0f;
	m_desc.m_MaskTexRows = 1;
	m_desc.m_MaskTexCols = 1;
	m_desc.m_MaskUSpeed = 0.0f;
	m_desc.m_MaskVSpeed = 0.0f;
	m_desc.m_MaskUOgnOff = 0.0f;
	m_desc.m_MaskVOgnOff = 0.0f;

	Ogre::KeyFrameArray<float>::KEYFRAME_T kf1,kf2,kf3;
	Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc1,kfc2,kfc3;

	kf1.tick = 0;		kf2.tick = 50; 		kf3.tick = 100;
	kf1.data = 10.0f;	kf2.data = 10.0f;	kf3.data = 10.0f;
	m_WidthKey.m_Array.push_back(kf1);
	m_WidthKey.m_Array.push_back(kf2);
	m_WidthKey.m_Array.push_back(kf3);


	kf1.tick = 0;		kf2.tick = 50; 		kf3.tick = 100;
	kf1.data = 1.0f;		kf2.data = 1.0f;		kf3.data = 1.0f;
	m_AlphaKey.m_Array.push_back(kf1);
	m_AlphaKey.m_Array.push_back(kf2);
	m_AlphaKey.m_Array.push_back(kf3);

	Ogre::ColourValue defClr = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	kfc1.tick = 0;		kfc2.tick = 50; 		kfc3.tick = 100;
	kfc1.data = defClr;	kfc2.data = defClr;		kfc3.data = defClr;
	m_ColorKey.m_Array.push_back(kfc1);
	m_ColorKey.m_Array.push_back(kfc2);
	m_ColorKey.m_Array.push_back(kfc3);

	m_strTexture = _T(DEF_FX_TEXTURE);
	m_strMaskTexture = "";

	BEA_FRAME frame;
	m_KeyFrames.clear();
	m_KeyFrames.push_back(frame);


	m_bVisible = true;
	m_docInfo.NewDoc();
	return true;
}

void CBeam::DoFrame(Ogre::uint dtick) 
{
	m_interact.DoFrame(dtick);
}

bool CBeam::Destroy()
{
	return true;
}

bool CBeam::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::BeamEmitterData, pChunk))
	{
		OGRE_RELEASE(pChunk);
		return false;
	}
	LoadFromChunk(pChunk);
	m_strName = path;
	OGRE_RELEASE(pChunk);	
	m_bIsNew = false;
	return true;
}

bool CBeam::SaveEffect(CString path)
{
	Ogre::Resource *pres = GetChunk();
	if(pres == NULL) return false;

	pres->save(path.GetBuffer());

	m_bIsNew = false;
	m_docInfo.Save();
	return true;
}

bool CBeam::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::BeamEmitterData* pBeamChunk = (Ogre::BeamEmitterData*) pChunk;

	SetKfFromSource(pBeamChunk);

	if (pBeamChunk->m_pTexture)
		m_strTexture = pBeamChunk->m_pTexture->getResPath();
	else
		m_strTexture = "";

	if(pBeamChunk->m_pMaskTexture)
		m_strMaskTexture = pBeamChunk->m_pMaskTexture->getResPath();
	else m_strMaskTexture = "";

	m_bIsNew = false;
	m_docInfo.Open();
	return true;
}

void CBeam::SetKfFromSource(Ogre::BeamEmitterData* pSource)
{
	m_desc = pSource->m_Desc;
	m_WidthKey = pSource->m_WidthKey;
	m_AlphaKey = pSource->m_AlphaKey;
	m_ColorKey = pSource->m_ColorKey;

	m_KeyFrames.clear();

	for(int i = 0; i < (int)pSource->m_StartRange.m_Array.size(); i++)
	{
		BEA_FRAME frame;
		frame.tick			= pSource->m_StartRange.m_Array[i].tick;

		frame.startrange	= pSource->m_StartRange.m_Array[i].data;
		frame.endrange		= pSource->m_EndRange.m_Array[i].data;
		frame.endpos		= pSource->m_EndPos.m_Array[i].data;
		frame.sizevar		= pSource->m_SizeVar.m_Array[i].data * 100.0f;
		frame.life			= pSource->m_Life.m_Array[i].data;
		frame.lifevar		= pSource->m_LifeVar.m_Array[i].data * 100.0f;
		frame.emitrate		= pSource->m_EmitRate.m_Array[i].data;

		m_KeyFrames.push_back(frame);
	}
}

#define BEAM_ADD_KF(keyframearray, val, tk, tmp) {((tmp).tick=(tk)); ((tmp).data=(val)); (keyframearray).m_Array.push_back(tmp);};

void CBeam::SetKfToSource(Ogre::BeamEmitterData* pSource)
{
	if(m_desc.m_HighFrequencyPoints < 0)
		m_desc.m_HighFrequencyPoints = 0;
	if(m_desc.m_LowFrequencyPoints < 2)
		m_desc.m_LowFrequencyPoints = 2;
	if(m_desc.m_MaskTexCols < 1)
		m_desc.m_MaskTexCols = 1;
	if(m_desc.m_MaskTexRows < 1)
		m_desc.m_MaskTexRows = 1;
	if(m_desc.m_TexCols < 1)
		m_desc.m_TexCols = 1;
	if(m_desc.m_TexRows < 1)
		m_desc.m_TexRows = 1;

	pSource->m_Desc = m_desc;
	pSource->m_WidthKey = m_WidthKey;
	pSource->m_AlphaKey = m_AlphaKey;
	pSource->m_ColorKey = m_ColorKey;

	Ogre::KeyFrameArray<Ogre::Vector3> initkv;
	Ogre::KeyFrameArray<float> initkf;

	pSource->m_StartRange = initkv;
	pSource->m_EndRange = initkv;
	pSource->m_EndPos = initkv;
	pSource->m_SizeVar = initkf;
	pSource->m_Life = initkf;
	pSource->m_LifeVar = initkf;
	pSource->m_EmitRate = initkf;

	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T kfv;
		int tick = m_KeyFrames[i].tick;

		BEAM_ADD_KF(pSource->m_StartRange, m_KeyFrames[i].startrange, tick, kfv);
		BEAM_ADD_KF(pSource->m_EndRange, m_KeyFrames[i].endrange, tick, kfv);
		BEAM_ADD_KF(pSource->m_EndPos, m_KeyFrames[i].endpos, tick, kfv);
		BEAM_ADD_KF(pSource->m_SizeVar, m_KeyFrames[i].sizevar/100.0f, tick, kf);
		BEAM_ADD_KF(pSource->m_Life, m_KeyFrames[i].life, tick, kf);
		BEAM_ADD_KF(pSource->m_LifeVar, m_KeyFrames[i].lifevar/100.0f, tick, kf);
		BEAM_ADD_KF(pSource->m_EmitRate, m_KeyFrames[i].emitrate, tick, kf);
	}


}

void CBeam::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CBeam::GetType()
{
	return EFFECT_BEAM;
}

void CBeam::SetName(CString strName)
{
	m_strName = strName;
}

BIND_INFO* CBeam::GetBindInfo()
{
	return &m_bindInfo;
}


CEffectCommand* CBeam::BeginRecordCommand()
{
	CBeamCommand* pUndoCmd = new CBeamCommand();

	pUndoCmd->m_newDesc = m_desc;
	pUndoCmd->m_newTex = m_strTexture;
	pUndoCmd->m_newFrame = m_KeyFrames;
	pUndoCmd->m_newMaskTex = m_strMaskTexture;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd;
}

void CBeam::EndRecordCommand(CEffectCommand* pcmd)
{
	CBeamCommand* pUndoCmd = static_cast<CBeamCommand*>(pcmd);
	pUndoCmd->m_oldDesc = m_desc;
	pUndoCmd->m_oldTex = m_strTexture;
	pUndoCmd->m_oldFrame = m_KeyFrames;
	pUndoCmd->m_oldMaskTex = m_strMaskTexture;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}

void CBeam::ExecuteCommand(CEffectCommand* pCmd)
{
	if(pCmd->GetTarget() != this)
		return;

	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	bool bRefresh = false;
	CBeamCommand* pBeamCmd = (CBeamCommand*) pCmd;

	if(pBeamCmd->m_newTex != m_strTexture
		||	pBeamCmd->m_newMaskTex != m_strMaskTexture
		||	pBeamCmd->m_newDesc.m_bDistort != m_desc.m_bDistort
		||	pBeamCmd->m_newDesc.m_MaxParticles != m_desc.m_MaxParticles
		||	pBeamCmd->m_newDesc.m_BlendType != m_desc.m_BlendType
		||	pBeamCmd->m_newDesc.m_iUVAddressMode != m_desc.m_iUVAddressMode
		||	pBeamCmd->m_newDesc.m_iMaskUVAddressMode != m_desc.m_iMaskUVAddressMode
		||	pBeamCmd->m_newDesc.m_bEmitOnce != m_desc.m_bEmitOnce
		)
	{
		bRefresh = true;
	}

	m_desc = pBeamCmd->m_newDesc;
	m_KeyFrames = pBeamCmd->m_newFrame;
	m_strTexture = pBeamCmd->m_newTex;
	m_strMaskTexture = pBeamCmd->m_newMaskTex;

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

// ------------------------------------------- [4/22/2008]

CEffectUnit* CBeamCommand::GetTarget()
{
	return m_pTarget;
}

bool CBeamCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CBeamCommand::GetInversCommand()
{
	CBeamCommand* pCmd = new CBeamCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newDesc = m_oldDesc;
	pCmd->m_oldDesc = m_newDesc;

	pCmd->m_newTex = m_oldTex;
	pCmd->m_oldTex = m_newTex;

	pCmd->m_newMaskTex = m_oldMaskTex;
	pCmd->m_oldMaskTex = m_newMaskTex;

	pCmd->m_newFrame = m_oldFrame;
	pCmd->m_oldFrame = m_newFrame;

	return pCmd;
}

bool CBeamCommand::IsIdentical()
{
	if(memcmp(&m_newDesc, &m_oldDesc, sizeof(Ogre::PBeamEmitterDesc)) != 0)
		return false;

	if(m_oldTex != m_newTex)
		return false;

	if(m_oldMaskTex != m_newMaskTex)
		return false;

	if(m_oldFrame.size() != m_newFrame.size())
		return false;
	for(int i = 0; i < (int)m_oldFrame.size(); i++)
	{
		if(memcmp(&m_oldFrame[i], &m_newFrame[i], sizeof(CBeam::BEA_FRAME)) != 0)
			return false;
	}

	return true;
}

bool CBeamCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	if(pCmd->GetTarget() != m_pTarget)
		return false;
	CBeamCommand* pBeamCmd = (CBeamCommand*) pCmd;

	if(bForceMerge == true)
	{
		m_oldDesc = pBeamCmd->m_oldDesc;

		m_oldFrame = pBeamCmd->m_oldFrame;
		m_oldTex = pBeamCmd->m_oldTex;
		m_oldMaskTex = pBeamCmd->m_oldMaskTex;

		return true;
	}
	return false;
}