
#include "stdafx.h"
#include "EffectUnit_Light.h"
#include "EffectUnitEditBar_Light.h"
#include "EditorManager.h"
#include "EffectManager.h"

#include "OgreLight.h"
#include "OgreCamera.h"
#include "OgreLines.h"
#include "OgreResourceManager.h"
#include "OgreEntity.h"

CLightInteract::CLightInteract()
{
	//m_bHovering = false;
	//m_pCood = NULL;
	//m_bCood = false;
	//m_tran = COOD_NONE;
	//m_pTarget = NULL;
	m_pIdn = NULL;
}
CLightInteract::~CLightInteract()
{
	Clear();
}

void CLightInteract::Clear()
{
	//if(m_bCood && m_pCood)
	//{
	//	m_pCood->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);

	//}

	//m_bHovering = false;
	//OGRE_RELEASE(m_pCood);
	//m_pCood = NULL; 
	//m_bCood = false;
	//m_tran = COOD_NONE;

	CInteract::Clear();
	if(m_pIdn)
	{
		m_pIdn->detachFromScene();
		OGRE_RELEASE(m_pIdn);
	}
}

void CLightInteract::Create(CEffectUnit* pTarget)
{
	//m_bHovering = false;
	//m_pCood = NULL;
	//m_bCood = false;
	//m_tran = COOD_NONE;
	//m_pTarget = pTarget;

	//m_pCood = new CCoord();
	//m_pCood->CreateVertex();
	CInteract::Create(pTarget);

	Ogre::Resource *pres = Ogre::ResourceManager::getSingleton().blockLoad("toolres\\fxeditor\\lighticon.omod");
	m_pIdn = Ogre::createObjectFromResource(pres);
	if(m_pIdn)
	{
		m_pIdn->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
	}
}

void CLightInteract::DoFrame(Ogre::uint dtick)
{
	//float range= m_pTarget->GetRange();
	//if(m_bHovering && !m_pTarget->GetBindInfo()->bDynamic)
	//{
	//	CEnginAgent::GetInstance()->AddContainerBox(m_pTarget->GetNode()->getWorldPosition(), range);
	//}
	//m_pCood->SetFather(m_pTarget->GetNode());
	//m_pCood->m_fSize = range;
	//m_pCood->SetTranslate(m_Desc.m_Pos);

	CInteract::DoFrame(dtick);
	if(m_pIdn && !m_pTarget->GetBindInfo()->bDynamic)
	{
		m_pIdn->setPosition(Ogre::WorldPos(m_pTarget->GetNode()->getWorldPosition()));
	}
	//m_pCood->SetPosition(Ogre::WorldPos(m_pTarget->GetNode()->getWorldPosition()));

	if(m_bCood)
		DrawRange();
}

void CLightInteract::DrawRange()
{
	CLight* pTarget = static_cast<CLight*> (m_pTarget);
	float r = pTarget->m_Range;

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


// -------------------CLight----------------------------- [12/14/2007]
CLight::CLight(void) //: m_pTmpChunk(NULL)
{
	m_pNode = NULL;
	m_pSource = NULL;
	m_interact.Clear();
}

CLight::~CLight(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}


Ogre::SceneNode* CLight::GetNode()
{
	return m_pNode;
}



CBCGPDockingControlBar*CLight::CreateBar(CBCGPFrameWnd* pParent)
{
	CLightBar* pBar = new CLightBar();
	//static int id = 0;
	//pBar->Create(NULL, pParent, CRect (0, 0,  250,600),
	//	TRUE, 0,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	//pBar->SetEditTarget(this);
	return pBar;
}

Ogre::Resource* CLight::GetChunk()
{
	Ogre::LightData* pLightChunk = new Ogre::LightData();
	pLightChunk->m_Type = m_Type;
	pLightChunk->m_Color = m_Color;
	pLightChunk->m_Attenuation = m_Attenuation;
	pLightChunk->m_Range = m_Range;
	return pLightChunk;
}

void CLight::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_interact.Clear();
}

Ogre::SceneNode* CLight::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	m_interact.Create(this);

	m_pSource = static_cast<Ogre::LightData *>(GetChunk());
	if(m_pSource == NULL) return NULL;

	Ogre::Light *plight = new Ogre::Light(m_pSource);
	m_pNode = plight;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);

	return m_pNode;
}

void CLight::RefreshNode()
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


	CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);

}

void CLight::UpdateSource()
{
	if (!m_pNode )
		return;

	m_pSource->m_Type = m_Type;
	m_pSource->m_Color = m_Color;
	m_pSource->m_Range = m_Range;
	m_pSource->m_Attenuation = m_Attenuation;

	m_pNode->m_Type = m_Type;
	m_pNode->setColor(m_Color);
	m_pNode->m_Range = m_Range;
	m_pNode->m_Attenuation = m_Attenuation;
	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		GetNode()->setPosition(GetBindInfo()->offset);
	}
	m_docInfo.Modify();
}

void CLight::DoFrame(Ogre::uint dtick)
{
	m_interact.DoFrame(dtick);
}

CString CLight::GetName()
{
	return m_strName;
}


bool CLight::Create()
{
	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;

	m_strName.Format(_T("光源"));

	m_Type = Ogre::LT_POINT;
	m_Color = Ogre::ColourValue(0.2f, 0.2f, 0.2f, 1.0f);

	m_Attenuation = 1.0f;
	m_Range = 100.0f;
	m_bVisible = true;
	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;
}

bool CLight::Destroy()
{
	return true;
}

//bool CLight::IsVisible()
//{
//	return m_bVisible;
//}
//
//void CLight::SetVisible(bool visible)
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

bool CLight::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::LightData, pChunk))
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

bool CLight::SaveEffect(CString path)
{
	Ogre::Resource *pres = GetChunk();
	if(pres == NULL) return false;

	pres->save(path.GetBuffer());

	m_bIsNew = false;
	m_docInfo.Save();
	return true;

}

bool CLight::LoadFromChunk(Ogre::Resource* pChunk)
{
	m_strName.Format(_T("光源"));
	Ogre::LightData* pLight = ((Ogre::LightData*)pChunk);
	m_Type = pLight->m_Type;
	m_Color = pLight->m_Color;
	m_Attenuation = pLight->m_Attenuation;
	m_Range = pLight->m_Range;
	m_bVisible = true;
	m_bIsNew = false;
	m_docInfo.Open();
	return true;
}

void CLight::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CLightCommand* pLightCmd = (CLightCommand*) pCmd;
	bool bRefresh = false;

	if(pLightCmd->m_newType != m_Type 
		|| pLightCmd->m_newColor != m_Color )
	{
		bRefresh = true;	
	}

	m_Type = pLightCmd->m_newType;
	m_Range = pLightCmd->m_newRange;
	m_Color = pLightCmd->m_newColor;
	m_Attenuation = pLightCmd->m_newAttenuation;
	m_bindInfo = pLightCmd->m_newBindInfo;
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

CEffectCommand* CLight::BeginRecordCommand()
{
	CLightCommand* pRet = new CLightCommand();
	pRet->m_pTarget = this;
	pRet->m_newType = m_Type;
	pRet->m_newRange = m_Range;
	pRet->m_newColor = m_Color;
	pRet->m_newBindInfo = m_bindInfo;
	pRet->m_newAttenuation = m_Attenuation;
	return pRet;
}

void CLight::EndRecordCommand(CEffectCommand* psrccmd)
{
	CLightCommand* pCmd = static_cast<CLightCommand*>(psrccmd);
	pCmd->m_oldType = m_Type;
	pCmd->m_oldRange = m_Range;
	pCmd->m_oldColor = m_Color;
	pCmd->m_oldBindInfo = m_bindInfo;
	pCmd->m_oldAttenuation = m_Attenuation;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pCmd);
}

BIND_INFO* CLight::GetBindInfo()
{
	return &m_bindInfo;
}

void CLight::SetBindInfo(BIND_INFO* bindinfo) 
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CLight::GetType()
{
	return EFFECT_LIGHT;
}

// -----------CLightCommand---------------------- [12/14/2007]

CEffectUnit* CLightCommand::GetTarget()
{
	return m_pTarget;
}

bool CLightCommand::NeedRefresh()
{
	if(m_oldColor != m_newColor || m_oldType != m_newType)
		return true;
	return false;
}

CEffectCommand* CLightCommand::GetInversCommand()
{
	CLightCommand* pRet = new CLightCommand();
	pRet->m_pTarget = m_pTarget;

	pRet->m_oldType		= m_newType;
	pRet->m_oldColor	= m_newColor;
	pRet->m_oldBindInfo = m_newBindInfo;
	pRet->m_oldRange	= m_newRange;
	pRet->m_oldAttenuation = m_newAttenuation;

	pRet->m_newType		= m_oldType;
	pRet->m_newColor	= m_oldColor;
	pRet->m_newBindInfo = m_oldBindInfo;
	pRet->m_newRange	= m_oldRange;
	pRet->m_newAttenuation = m_oldAttenuation;

	return pRet;
}

bool CLightCommand::IsIdentical()
{
	if(	   m_oldType		== m_newType 
		&& m_oldColor		== m_newColor
		&& m_oldBindInfo	== m_newBindInfo
		&& m_oldRange		== m_newRange
		&& m_oldAttenuation == m_newAttenuation )
	{
		return true;
	}
	return false;
}

bool CLightCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	CLightCommand* pLightCmd = (CLightCommand*) pCmd;
	if(	   m_oldType		== pLightCmd->m_oldType 
		&& m_oldColor		== pLightCmd->m_oldColor
		&& m_oldRange		== pLightCmd->m_oldRange
		&& m_oldAttenuation == pLightCmd->m_oldAttenuation
		&& m_oldBindInfo.boneid == pLightCmd->m_oldBindInfo.boneid)
	{
		m_oldBindInfo.offset = pLightCmd->m_oldBindInfo.offset;
		return true;
	}
	return false;
}

