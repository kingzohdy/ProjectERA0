
#include "stdafx.h"
#include "EffectUnit_Billboard.h"
#include "EffectManager.h"
#include "EffectUnitEditBar_Billboard.h"

#include "OgreBillboard.h"
#include "OgreTexture.h"
#include "OgreResourceManager.h"
#include "OgreEntity.h"

// --------------------------------------------------------------------------------------- [11/22/2007]
CBillboard::CBillboard(void)
{
	m_bindInfo.boneid = 0;
	m_bVisible = true;
	m_pNode = NULL;
	m_pSource = NULL;
	m_strLastTexture = "";
	//m_interact.Clear();
	m_fHotTime = -1;

	m_pTempFrame = &m_HotTimeFrameBuff;
}

CBillboard::~CBillboard(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}



Ogre::SceneNode* CBillboard::GetNode()
{
	return m_pNode;
}

void CBillboard::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_pNode = NULL;

	//m_interact.Clear();
}

Ogre::SceneNode* CBillboard::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	//m_interact.Create(this);

	m_pSource = (Ogre::BillboardData*)GetChunk();
	if(m_pSource == NULL) return NULL;
	Ogre::Billboard* pNode = new Ogre::Billboard(m_pSource);

	m_pNode = pNode;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);
	m_strLastTexture = m_strTexture;
	m_strLastMaskTexture = m_strMaskTexture;

	return m_pNode;

}

void CBillboard::RefreshNode()
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

void CBillboard::UpdateSource()
{
	if (!m_pSource)
		return;
	Ogre::Resource* pOldTex = m_pSource->m_pTexture;
	m_pSource->m_Desc = m_desc;
	m_pSource->m_DescEx = m_descEx;
	m_pSource->m_Desc104 = m_descEx104;

	SetKFToSource(m_pSource);
	if(m_strLastTexture != m_strTexture)
	{
		OGRE_RELEASE(m_pSource->m_pTexture);
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			if(IS_KIND_OF(Ogre::TextureData, pNewTex)) m_pSource->m_pTexture = static_cast<Ogre::TextureData *>(pNewTex);
			else pNewTex->release();
		}
		m_strLastTexture = m_strTexture;
	}

	if(m_strLastMaskTexture != m_strMaskTexture)
	{
		OGRE_RELEASE(m_pSource->m_pMaskTexture);
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			if(IS_KIND_OF(Ogre::TextureData, pNewTex)) m_pSource->m_pMaskTexture = static_cast<Ogre::TextureData*>(pNewTex);
			else pNewTex->release();
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


#define ADD_KF(keyframearray, val, tk, tmp) {((tmp).tick=(tk)); ((tmp).data=(val)); (keyframearray).m_Array.push_back(tmp);};
bool CBillboard::SetKFToSource(Ogre::BillboardData* pSource)
{
	pSource->m_Desc = m_desc;
	pSource->m_DescEx = m_descEx;
	pSource->m_Desc104 = m_descEx104;

	Ogre::KeyFrameArray<float> initkf;
	Ogre::KeyFrameArray<Ogre::ColourValue> initkfc;
	//pSource->m_DescEx.m_FrameType = Ogre::SFT_FIXED_FRAME_TIME; 

	pSource->m_SizeKeyFrame = initkf;
	pSource->m_AspectKeyFrame = initkf;
	pSource->m_UVRotateKeyFrame = initkf;
	pSource->m_UScaleKeyFrame = initkf;
	pSource->m_VScaleKeyFrame = initkf;
	pSource->m_UOffsetKeyFrame = initkf;
	pSource->m_VOffsetKeyFrame = initkf;
	pSource->m_SeqFrameKeyFrame = initkf;
	pSource->m_ColorKeyFrame = initkfc;

	pSource->m_MaskUVRotateKeyFrame = initkf;
	pSource->m_MaskUScaleKeyFrame = initkf;
	pSource->m_MaskVScaleKeyFrame = initkf;
	pSource->m_MaskUOffsetKeyFrame = initkf;
	pSource->m_MaskVOffsetKeyFrame = initkf;
	pSource->m_MaskSeqFrameKeyFrame = initkf;

	// 104
	pSource->m_SizeXKeyFrame= initkf;
	pSource->m_SizeYKeyFrame = initkf;

	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc;

		int tick = m_KeyFrames[i].lTick;
		ADD_KF(pSource->m_SizeKeyFrame, m_KeyFrames[i].Size, tick, kf); 
		ADD_KF(pSource->m_AspectKeyFrame,m_KeyFrames[i].Aspect, tick, kf); 
		ADD_KF(pSource->m_UVRotateKeyFrame, m_KeyFrames[i].UVRotat, tick, kf); 
		ADD_KF(pSource->m_UScaleKeyFrame, m_KeyFrames[i].UScale, tick, kf); 
		ADD_KF(pSource->m_VScaleKeyFrame, m_KeyFrames[i].VScale, tick, kf); 
		ADD_KF(pSource->m_UOffsetKeyFrame, m_KeyFrames[i].UOffset, tick, kf); 
		ADD_KF(pSource->m_VOffsetKeyFrame, m_KeyFrames[i].VOffset, tick, kf); 

		kfc.tick = tick;
		kfc.data = m_KeyFrames[i].color;
		kfc.data.a = m_KeyFrames[i].alpha;
		pSource->m_ColorKeyFrame.m_Array.push_back(kfc);

		ADD_KF(pSource->m_SeqFrameKeyFrame, m_KeyFrames[i].SeqFrame, tick, kf); 

		ADD_KF(pSource->m_MaskUVRotateKeyFrame, m_KeyFrames[i].MaskUVRotat, tick, kf); 
		ADD_KF(pSource->m_MaskUOffsetKeyFrame, m_KeyFrames[i].MaskUOffset, tick, kf); 
		ADD_KF(pSource->m_MaskVOffsetKeyFrame, m_KeyFrames[i].MaskVOffset, tick, kf); 
		ADD_KF(pSource->m_MaskUScaleKeyFrame, m_KeyFrames[i].MaskUScale, tick, kf); 
		ADD_KF(pSource->m_MaskVScaleKeyFrame, m_KeyFrames[i].MaskVScale, tick, kf); 
		ADD_KF(pSource->m_MaskSeqFrameKeyFrame, m_KeyFrames[i].MaskSeqFrame, tick, kf); 

		// 104
		ADD_KF(pSource->m_SizeXKeyFrame, m_KeyFrames[i].SizeX, tick, kf);
		ADD_KF(pSource->m_SizeYKeyFrame, m_KeyFrames[i].SizeY, tick, kf);

	}
	return true;
}

bool CBillboard::SetKFFromSource(Ogre::BillboardData* pSource)
{
	m_KeyFrames.clear();
	for(int i = 0; i < (int)pSource->m_SizeKeyFrame.m_Array.size(); i++)
	{
		BIL_FRAME frame;
		frame.lTick = (long)pSource->m_SizeKeyFrame.m_Array[i].tick;

		frame.UVRotat = pSource->m_UVRotateKeyFrame.m_Array[i].data;
		frame.UScale = pSource->m_UScaleKeyFrame.m_Array[i].data;
		frame.VScale = pSource->m_VScaleKeyFrame.m_Array[i].data;
		frame.UOffset = pSource->m_UOffsetKeyFrame.m_Array[i].data;
		frame.VOffset = pSource->m_VOffsetKeyFrame.m_Array[i].data;
		frame.SeqFrame = pSource->m_SeqFrameKeyFrame.m_Array[i].data;
		frame.color = pSource->m_ColorKeyFrame.m_Array[i].data;
		frame.alpha = frame.color.a;
		frame.Size = pSource->m_SizeKeyFrame.m_Array[i].data;
		frame.Aspect = pSource->m_AspectKeyFrame.m_Array[i].data;

		frame.MaskUVRotat = pSource->m_MaskUVRotateKeyFrame.m_Array[i].data;
		frame.MaskUOffset = pSource->m_MaskUOffsetKeyFrame.m_Array[i].data;
		frame.MaskVOffset = pSource->m_MaskVOffsetKeyFrame.m_Array[i].data;
		frame.MaskUScale = pSource->m_MaskUScaleKeyFrame.m_Array[i].data;
		frame.MaskVScale = pSource->m_MaskVScaleKeyFrame.m_Array[i].data;
		frame.MaskSeqFrame = pSource->m_MaskSeqFrameKeyFrame.m_Array[i].data;

		if (m_descEx104.m_bSizeXYDepend)
		{
			frame.SizeX = pSource->m_SizeXKeyFrame.m_Array[i].data;
			frame.SizeY = pSource->m_SizeYKeyFrame.m_Array[i].data;
		}

		m_KeyFrames.push_back(frame);
	}
	return true;
}

CBCGPDockingControlBar* CBillboard::CreateBar(CBCGPFrameWnd* pParent)
{
	CBillboardBar* pBar = new CBillboardBar();
	return pBar;
}

Ogre::Resource* CBillboard::GetChunk()
{
	Ogre::BillboardData* pBillboardChunk = new Ogre::BillboardData();

	SetKFToSource(pBillboardChunk);
	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(
		m_strTexture.GetBuffer());
	pBillboardChunk->m_pTexture = static_cast<Ogre::TextureData *>(pTex);

	if (m_strMaskTexture != "") 
	{
		pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
		pBillboardChunk->m_pMaskTexture = static_cast<Ogre::TextureData *>(pTex);
	}
	else
	{
		pBillboardChunk->m_pMaskTexture = NULL;
	}

	return pBillboardChunk;
}

float CBillboard::GetRange()
{
	float ret = m_desc.m_SizeKeys[0];
	if(ret < m_desc.m_SizeKeys[1]) 
		ret = m_desc.m_SizeKeys[1];
	if(ret < m_desc.m_SizeKeys[2]) 
		ret = m_desc.m_SizeKeys[2];
	return ret;
}

CString CBillboard::GetName()
{
	return m_strName;
}

bool CBillboard::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;
	m_strName.Format(_T("¹«¸æ°å"));

	//m_iType = 0;
	//m_Center = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	//m_Extent = Ogre::Vector2(0.5f, 0.5f);
	m_desc.m_iType = Ogre::ALWAYS_FACE_CAMERA;
	m_desc.m_Center = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	m_desc.m_BlendType = 0;
	m_desc.m_MidPoint = 0.5f;
	m_desc.m_TexCols = 1;
	m_desc.m_TexRows = 1;
	m_desc.m_TimeSlot = 0.0f;
	m_desc.m_Pos = Ogre::Vector3(0.0f, 0.0f, 0.0f);

	m_desc.m_AspectRatioKeys[0] = 1.0f;
	m_desc.m_AspectRatioKeys[1] = 1.0f;
	m_desc.m_AspectRatioKeys[2] = 1.0f;

	m_desc.m_SizeKeys[0] = 100.0f;
	m_desc.m_SizeKeys[1] = 100.0f;
	m_desc.m_SizeKeys[2] = 100.0f;

	m_desc.m_ColorKeys[0] = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	m_desc.m_ColorKeys[1] = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	m_desc.m_ColorKeys[2] = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);

	m_descEx.m_ColorMid = 0.5f;
	m_descEx.m_SizeMid = 0.5f;
	m_descEx.m_AspectMid = 0.5f;
	m_descEx.m_UVRotate = 0.0f;
	m_descEx.m_UVScale = 1.0f;
	m_descEx.m_bDistort = false;
	m_descEx.m_iUVAddressMode = Ogre::UVADDR_WRAP;
	m_descEx.m_FrameType = Ogre::SFT_FIXED_FRAME_TIME;
	m_descEx.m_iMaskUVAddrMode = Ogre::UVADDR_CLAMP;
	m_descEx.m_MaskTexCols = 1;
	m_descEx.m_MaskTexRows = 1;
	m_descEx.m_CenterAbsOffset = false;
	m_descEx.m_CenterOffset = Ogre::Vector2(0,0);
	// 104
	m_descEx104.m_bSizeXYDepend = false;

	m_strTexture = DEF_FX_TEXTURE;
	m_strMaskTexture = "";

	BIL_FRAME frame;
	m_KeyFrames.push_back(frame);

	m_bVisible = true;
	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;
}


void* CBillboard::GetKeyFramePropertyAddrByPtr(CString strPropertyName, BIL_FRAME* pFrame)
{
	BEGIN_NAME_CHECK(strPropertyName)
		RETURN_PTR_IF(UVRotat, pFrame)
		RETURN_PTR_IF(UScale, pFrame)
		RETURN_PTR_IF(VScale, pFrame)
		RETURN_PTR_IF(UOffset, pFrame)
		RETURN_PTR_IF(VOffset, pFrame)
		RETURN_PTR_IF(SeqFrame, pFrame)
		RETURN_PTR_IF(Size, pFrame)
		RETURN_PTR_IF(Aspect, pFrame)
		RETURN_PTR_IF(color, pFrame)
		RETURN_PTR_IF(color.a, pFrame)
		END_NAME_CHECK()
}

void* CBillboard::GetInfoPropertyAddr(CString strPropertyName)
{
	BEGIN_NAME_CHECK(strPropertyName)
		RETURN_PTR_IF(m_iType, &m_desc)
		RETURN_PTR_IF(m_BlendType, &m_desc)
		RETURN_PTR_IF(m_MidPoint, &m_desc)
		RETURN_PTR_IF(m_TexCols, &m_desc)
		RETURN_PTR_IF(m_TexRows, &m_desc)
		RETURN_PTR_IF(m_TimeSlot, &m_desc)
		RETURN_PTR_IF(m_ColorMid, &m_descEx)
		RETURN_PTR_IF(m_SizeMid, &m_descEx)
		RETURN_PTR_IF(m_AspectMid, &m_descEx)
		RETURN_PTR_IF(m_UVScale, &m_descEx)
		RETURN_PTR_IF(m_UVRotate, &m_descEx)
		RETURN_PTR_IF(m_bDistort, &m_descEx)
		RETURN_PTR_IF(m_iUVAddressMode, &m_descEx)
		RETURN_PTR_IF(m_CenterAbsOffset, &m_descEx)
		RETURN_PTR_IF(m_CenterOffset, &m_descEx)

		// 104
		RETURN_PTR_IF(m_bSizeXYDepend, &m_descEx104)

		RETURN_PTR_IF(m_strTexture, this)
		END_NAME_CHECK()
}


void* CBillboard::GetKeyFramePropertyAddr(CString strPropertyName, int iKeyFrame)
{
	if(iKeyFrame >= 0 && iKeyFrame < (int)m_KeyFrames.size())
	{
		return GetKeyFramePropertyAddrByPtr(strPropertyName, &m_KeyFrames[iKeyFrame]);
	}
	return NULL;
}

void* CBillboard::GetKeyFramePropertyAddrAtHotTime(CString strPropertyName)
{
	if(m_fHotTime < 0.0f)
		return NULL;
	return GetKeyFramePropertyAddrByPtr(strPropertyName, &m_HotTimeFrameBuff);
}



int CBillboard::GetSelectedKeyFrame()
{
	for(int i = 0; i < (int) m_KeyFrames.size(); i++)
	{
		float frameTime = m_KeyFrames[i].lTick * 0.001f;
		frameTime -= m_fHotTime;
		if(frameTime > -0.016 && frameTime < 0.016)
		{
			return i;
		}
	}
	return -1;
}

int CBillboard::GetKeyFrameCount()
{
	return (int)m_KeyFrames.size();
}

CEffectKeyFrame* CBillboard::GetKeyFrame(int iKeyFrame)
{
	if(iKeyFrame >= 0 && iKeyFrame < (int)m_KeyFrames.size())
		return &m_KeyFrames[iKeyFrame];

	return NULL;
}

float CBillboard::GetKeyFrameTime(int iKeyFrame)
{
	if(iKeyFrame >= 0 && iKeyFrame < (int)m_KeyFrames.size())
	{
		return m_KeyFrames[iKeyFrame].lTick * 0.001f;
	}
	return -1.0f;
}

int CBillboard::InsertKeyFrame(CEffectKeyFrame* pFrame, int iBefore)
{
	BIL_FRAME* pMyFrame = static_cast<BIL_FRAME*>(pFrame);
	m_KeyFrames.insert(m_KeyFrames.begin() + iBefore, *pMyFrame);
	return iBefore;
}

void CBillboard::DeleteKeyFrame(int index)
{
	m_KeyFrames.erase(m_KeyFrames.begin() + index);
}




void CBillboard::DoFrame(Ogre::uint dtick) 
{
	//m_interact.DoFrame(dtick);
}

bool CBillboard::Destroy()
{
	return true;
}

//bool CBillboard::IsVisible()
//{
//	return m_bVisible;
//}
//
//void CBillboard::SetVisible(bool visible)
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

bool CBillboard::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::BillboardData, pChunk))
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

bool CBillboard::SaveEffect(CString path)
{
	Ogre::Resource* pBillboardChunk = GetChunk();
	if(pBillboardChunk == NULL)
		return false;

	pBillboardChunk->save(path.GetBuffer());
	m_strName = path;
	OGRE_RELEASE (pBillboardChunk);
	m_bIsNew = false;
	m_docInfo.Save();
	return true;
}

bool CBillboard::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::BillboardData* pBillboardChunk = (Ogre::BillboardData*) pChunk;
	//m_Center = pBillboardChunk->m_Desc.m_Center;
	//m_iType = pBillboardChunk->m_Desc.m_iType;

	m_desc = pBillboardChunk->m_Desc;
	m_descEx = pBillboardChunk->m_DescEx;
	m_descEx104 = pBillboardChunk->m_Desc104;
	//if(pBillboardChunk->m_pTexture != Ogre::INVALID_HANDLE)
	//{
	//	CEnginAgent::GetInstance()->m_pEngine->GetResLoader()->ReleaseResource(
	//		pBillboardChunk->m_pTexture);
	//}
	SetKFFromSource(pBillboardChunk);
	if(pBillboardChunk->m_pTexture) m_strTexture = pBillboardChunk->m_pTexture->getResPath();
	else m_strTexture = "";

	if(pBillboardChunk->m_pMaskTexture != NULL)	m_strMaskTexture = pBillboardChunk->m_pMaskTexture->getResPath();
	else m_strMaskTexture = "";

	m_bIsNew = false;
	//m_interact.Create(this);
	m_docInfo.Open();
	return true;
}

BIND_INFO* CBillboard::GetBindInfo()
{
	return &m_bindInfo;
}

void CBillboard::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CBillboard::GetType()
{
	return EFFECT_BILLBOARD;
}


void CBillboard::ExecuteCommand(CEffectCommand* pCmd)
{
	if(pCmd->GetTarget() != this)
		return;

	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CBillboardCommand* pBilCmd = (CBillboardCommand*) pCmd;
	bool bRefresh = false;

	if(pBilCmd->m_newDesc.m_BlendType != m_desc.m_BlendType ||
		pBilCmd->m_newDesc.m_ColorKeys[0] != m_desc.m_ColorKeys[0] ||
		pBilCmd->m_newDesc.m_ColorKeys[1] != m_desc.m_ColorKeys[1] ||
		pBilCmd->m_newDesc.m_ColorKeys[2] != m_desc.m_ColorKeys[2] ||
		pBilCmd->m_newTex != m_strTexture)
	{
		bRefresh = true;	
	}

	m_desc = pBilCmd->m_newDesc;
	m_strTexture = pBilCmd->m_newTex;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);

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

CEffectCommand* CBillboard::BeginRecordCommand()
{
	CBillboardCommand* pUndoCmd = new CBillboardCommand();

	pUndoCmd->m_newDesc = m_desc;
	pUndoCmd->m_newTex = m_strTexture;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd;
}

void CBillboard::EndRecordCommand(CEffectCommand* pcmd)
{
	CBillboardCommand* pUndoCmd = static_cast<CBillboardCommand*>(pcmd);
	pUndoCmd->m_oldDesc = m_desc;
	pUndoCmd->m_oldTex = m_strTexture;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}

// ------------------------------------------------------------------------------ [11/21/2007]

CEffectUnit* CBillboardCommand::GetTarget()
{
	return m_pTarget;
}

bool CBillboardCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CBillboardCommand::GetInversCommand()
{
	CBillboardCommand* pCmd = new CBillboardCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newDesc = m_oldDesc;
	pCmd->m_oldDesc = m_newDesc;

	pCmd->m_newTex = m_oldTex;
	pCmd->m_oldTex = m_newTex;

	return pCmd;
}

bool CBillboardCommand::IsIdentical()
{
	if(memcmp(&m_newDesc, &m_oldDesc, sizeof(Ogre::BillboardDesc)) != 0)
		return false;

	if(m_oldTex != m_newTex)
		return false;

	return true;
}



bool CBillboardCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	if(pCmd->GetTarget() != m_pTarget)
		return false;

	CBillboardCommand* pBilCmd = (CBillboardCommand*) pCmd;
	if(m_oldTex != pBilCmd->m_oldTex)
		return false;

	if(m_oldDesc.m_iType != pBilCmd->m_oldDesc.m_iType) return false;
	//if(m_oldDesc.m_Center != pBilCmd->m_oldDesc.m_Center) return false;
	if(m_oldDesc.m_TexRows != pBilCmd->m_oldDesc.m_TexRows) return false;
	if(m_oldDesc.m_TexCols != pBilCmd->m_oldDesc.m_TexCols) return false;
	if(m_oldDesc.m_TimeSlot != pBilCmd->m_oldDesc.m_TimeSlot) return false;
	if(m_oldDesc.m_BlendType != pBilCmd->m_oldDesc.m_BlendType) return false;
	if(m_oldDesc.m_MidPoint != pBilCmd->m_oldDesc.m_MidPoint) return false;


	if(m_oldDesc.m_ColorKeys[0] != pBilCmd->m_oldDesc.m_ColorKeys[0]) return false;
	if(m_oldDesc.m_ColorKeys[1] != pBilCmd->m_oldDesc.m_ColorKeys[1]) return false;
	if(m_oldDesc.m_ColorKeys[2] != pBilCmd->m_oldDesc.m_ColorKeys[2]) return false;
	if(m_oldDesc.m_SizeKeys[0] != pBilCmd->m_oldDesc.m_SizeKeys[0]) return false;
	if(m_oldDesc.m_SizeKeys[1] != pBilCmd->m_oldDesc.m_SizeKeys[1]) return false;
	if(m_oldDesc.m_SizeKeys[2] != pBilCmd->m_oldDesc.m_SizeKeys[2]) return false;
	if(m_oldDesc.m_AspectRatioKeys[0] != pBilCmd->m_oldDesc.m_AspectRatioKeys[0]) return false;
	if(m_oldDesc.m_AspectRatioKeys[1] != pBilCmd->m_oldDesc.m_AspectRatioKeys[1]) return false;
	if(m_oldDesc.m_AspectRatioKeys[2] != pBilCmd->m_oldDesc.m_AspectRatioKeys[2]) return false;

	m_oldDesc.m_Pos = pBilCmd->m_oldDesc.m_Pos;
	return true;
}