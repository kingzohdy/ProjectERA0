
#include "stdafx.h"
#include "EffectUnit_VirtualNode.h"
#include "EffectManager.h"
#include "EffectUnitEditBar_DummyNode.h"

#include "OgreDummyNode.h"
#include "OgreEntity.h"
#include "OgreResourceManager.h"


CVirtualNode::CVirtualNode(void)
{
	m_pNode = NULL;
	m_pSource = NULL;
	m_bShowBox = true;
}

CVirtualNode::~CVirtualNode(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}


Ogre::MovableObject* CVirtualNode::GetNode()
{
	return m_pNode;
}

float CVirtualNode::GetRange()
{
	return 20.0f;
}

void CVirtualNode::DoFrame(Ogre::uint dtick) 
{
	m_interact.DoFrame(dtick);
}

void CVirtualNode::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_pNode = NULL;


	m_interact.Clear();
}

Ogre::MovableObject* CVirtualNode::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	m_interact.Create(this);

	m_pSource = static_cast<Ogre::DummyNodeData *>(GetChunk());
	if(m_pSource == NULL)return NULL;

	Ogre::DummyNode* pNode = new Ogre::DummyNode(m_pSource);

	m_pNode = pNode;

	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);
	return m_pNode;

}

void CVirtualNode::RefreshNode()
{
	if(!m_bVisible)
		return ;

	Ogre::MovableObject* poldnode = m_pNode;
	//CEnginAgent::GetInstance()->m_pShowEngine->GetMap()->GetRoom()->DetachNode(poldnode);

	//ReleaseNode();
	//Ogre::MovableObject* pnode = CreateNode();

	//CEnginAgent::GetInstance()->m_pShowEngine->GetMap()->GetRoom()->AttachNode(pnode);

	if(poldnode)
	{
		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(poldnode);
	}

	ReleaseNode();

	Ogre::MovableObject* pnode = CreateNode();

	CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);

}

void CVirtualNode::UpdateSource()
{
	if (!m_pSource)
		return;

	SetKfToSource(m_pSource);
	m_pSource->m_Desc = m_desc;

	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		GetNode()->setPosition(GetBindInfo()->offset);
		GetNode()->setRotation(GetBindInfo()->rotate);
		GetNode()->setScale(GetBindInfo()->scale);
	}
	m_docInfo.Modify();
}

CBCGPDockingControlBar* CVirtualNode::CreateBar(CBCGPFrameWnd* pParent)
{
	CVirtualNodeBar* pBar = new CVirtualNodeBar();

	return pBar;
}

Ogre::Resource* CVirtualNode::GetChunk()
{
	Ogre::DummyNodeData* pChu = new Ogre::DummyNodeData();
	pChu->m_Desc = m_desc;
	SetKfToSource(pChu);

	return pChu;
}


CString CVirtualNode::GetName()
{
	return m_strName;
}

void CVirtualNode::SetName(CString strName)
{
	m_strName = strName;
}

bool CVirtualNode::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;

	m_strName.Format(_T("ÐéÄâÌå"));
	m_bVisible = true;

	VIR_FRAME firstFrame;
	firstFrame.lTick = 0;

	m_KeyFrames.clear();
	m_KeyFrames.push_back(firstFrame);

	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;

}

bool CVirtualNode::Destroy()
{
	return true;
}

//bool CVirtualNode::IsVisible()
//{
//	return m_bVisible;
//}
//
//void CVirtualNode::SetVisible(bool visible)
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


bool CVirtualNode::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::DummyNodeData, pChunk))
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

bool CVirtualNode::SaveEffect(CString path)
{
	Ogre::DummyNodeData* pChu = new Ogre::DummyNodeData();

	pChu->m_Desc = m_desc;
	SetKfToSource(pChu);


	pChu->save(path.GetBuffer());
	pChu->release();

	m_strName = path;
	m_bIsNew = false;
	m_docInfo.Save();
	return true;
}

bool CVirtualNode::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::DummyNodeData* pChu = static_cast<Ogre::DummyNodeData*>(pChunk);
	m_desc = pChu->m_Desc;
	SetKfFromSource(pChu);

	m_bIsNew = false;
	m_docInfo.Open();
	return true;
}

BIND_INFO* CVirtualNode::GetBindInfo()
{
	return &m_bindInfo;
}

void CVirtualNode::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CVirtualNode::GetType()
{
	return EFFECT_VIRTUALNODE;
}

void CVirtualNode::SetKfToSource(Ogre::DummyNodeData* pSource)
{
	//pSource->m_Above.m_Array.clear();
	//pSource->m_Below.m_Array.clear();
	//pSource->m_Color.m_Array.clear();

	Ogre::KeyFrameArray<float> initkf;
	Ogre::KeyFrameArray<Ogre::ColourValue> initkfc;
	pSource->m_bDrawBox = m_bShowBox;

	//pSource->m_Above = initkf;
	//pSource->m_Below = initkf;
	//pSource->m_Color = initkfc;
	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc;

		//pSource->m_Above.m_bUsed = false;
		//pSource->m_Below.m_bUsed = false;
		//pSource->m_Color.m_bUsed = false;

		//kf.tick = m_KeyFrames[i].lTick;
		//kf.data = m_KeyFrames[i].above;
		//pSource->m_Above.m_Array.push_back(kf);

		//kf.tick = m_KeyFrames[i].lTick;
		//kf.data = m_KeyFrames[i].below;
		//pSource->m_Below.m_Array.push_back(kf);

		//kfc.tick = m_KeyFrames[i].lTick;
		//kfc.data = m_KeyFrames[i].color;
		//pSource->m_Color.m_Array.push_back(kfc);

	}
}

void CVirtualNode::SetKfFromSource(Ogre::DummyNodeData* pSource)
{
	//m_KeyFrames.clear();

	//for(int i = 0; i < (int)pSource->m_Above.m_Array.size(); i++)
	//{
	//	RIB_FRAME frame;
	//	frame.lTick			= pSource->m_Above.m_Array[i].tick;

	//	frame.above			= pSource->m_Above.m_Array[i].data;
	//	frame.below			= pSource->m_Below.m_Array[i].data;
	//	frame.color			= pSource->m_Color.m_Array[i].data;

	//	m_KeyFrames.push_back(frame);
	//}
	//if(m_KeyFrames.size() == 0)
	//	MessageBox(NULL, "0 frame when SetKFFromSource", NULL,0);
	m_bShowBox = pSource->m_bDrawBox;
}


void CVirtualNode::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CVirtualNodeCommand* pVirCmd = (CVirtualNodeCommand*) pCmd;
	bool bRefresh = false;

	//if(pVirCmd->m_newDesc.m_BlendMode != m_desc.m_BlendMode ||
	//	pVirCmd->m_newTex != m_strTexture)
	//{
	//	bRefresh = true;	
	//}

	m_desc = pVirCmd->m_newDesc;

	m_KeyFrames = pVirCmd->m_newKf;
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
CEffectCommand* CVirtualNode::BeginRecordCommand()
{
	CVirtualNodeCommand* pUndoCmd = new CVirtualNodeCommand();

	pUndoCmd->m_newBind = m_bindInfo;
	pUndoCmd->m_newDesc = m_desc;
	pUndoCmd->m_newKf = m_KeyFrames;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd;
}
void CVirtualNode::EndRecordCommand(CEffectCommand* pcmd)
{
	CVirtualNodeCommand* pUndoCmd = static_cast<CVirtualNodeCommand*>(pcmd);
	pUndoCmd->m_oldBind = m_bindInfo;
	pUndoCmd->m_oldDesc = m_desc;
	pUndoCmd->m_oldKf = m_KeyFrames;


	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}
// --------------------------------------------------------------------- [11/21/2007]

CEffectUnit* CVirtualNodeCommand::GetTarget()
{
	return m_pTarget;
}

bool CVirtualNodeCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CVirtualNodeCommand::GetInversCommand()
{
	CVirtualNodeCommand* pCmd = new CVirtualNodeCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newBind = m_oldBind;
	pCmd->m_oldBind = m_newBind;

	pCmd->m_newDesc = m_oldDesc;
	pCmd->m_oldDesc = m_newDesc;

	pCmd->m_newKf = m_oldKf;
	pCmd->m_oldKf = m_newKf;


	return pCmd;
}

bool CVirtualNodeCommand::IsIdentical()
{
	if(memcmp(&m_newDesc, &m_oldDesc, sizeof(Ogre::DummyNodeDesc)) != 0)
		return false;

	if(m_oldKf.size() != m_newKf.size())
		return false;

	for(int i = 0; i < (int) m_oldKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &m_newKf[i], sizeof(CVirtualNode::VIR_FRAME)) != 0)
			return false;
	}

	return true;
}

bool CVirtualNodeCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	return false;
}