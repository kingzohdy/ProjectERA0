
#include "stdafx.h"
#include "EffectUnit_Particle.h"
#include "EffectUnitEditBar_Particle.h"
#include "EditorManager.h"
#include "Coord.h"
#include "EffectManager.h"

#include "OgreCamera.h"
#include "OgreLines.h"
#include "OgreParticleEmitter.h"
#include "OgreEntity.h"
#include "OgreTexture.h"

CParticleInteract::CParticleInteract()
{
	m_InteractObjType = PIT_NODE;
	m_iFaceIndex = -1;
}

CParticleInteract::~CParticleInteract()
{
	Clear();
}
void CParticleInteract::DrawParticleShape()
{
	CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
	Ogre::Matrix4 view = CEnginAgent::GetInstance()->m_pCamera->getViewMatrix();
	Ogre::Matrix4 viewInv;
	view.quickInverse(viewInv);
	Ogre::Vector3 vLookTo = view.getRow3(2);

	Ogre::Matrix4 rot;
	if (vLookTo.x > 0.0) 
		rot.makeRotateY(Ogre::Rad2Deg( atanf(vLookTo.z/vLookTo.x)) + 90);
	else
		rot.makeRotateY(Ogre::Rad2Deg( atanf(vLookTo.z/vLookTo.x)) - 90);

	if(pTarget->m_Desc.m_EmitterType == Ogre::EMITTER_SPHERE)
	{
		for(int i = 0; i < (int) pTarget->m_KeyFrames.size(); i++)
		{
			Ogre::Vector3 v0, v1, v2;
			float len = 100.0f;
			v0 = pTarget->GetNode()->getWorldMatrix().getRow3(3);
			v1.x = len * Ogre::Sin(pTarget->m_KeyFrames[i].data.spread);
			v1.y = len * Ogre::Cos(pTarget->m_KeyFrames[i].data.spread);
			v1.z = 0;

			v2 = v1;
			v2.x = -v2.x;

			//v1 = v0 + v1;
			//v2 = v0 + v2;
			v1 = v1 * rot + v0;
			v2 = v2 * rot + v0;

			Ogre::RenderLines* pDebugger = CEnginAgent::GetInstance()->m_pLineRenderer;

			pDebugger->addLine(v0, v1, Ogre::ColorQuad(255, 255, 255, 255));
			pDebugger->addLine(v0, v2, Ogre::ColorQuad(255, 255, 255, 255));

		} 
	}
	else if(pTarget->m_Desc.m_EmitterType == Ogre::EMITTER_PLANE )
	{
		for(int i = 0; i < (int) pTarget->m_KeyFrames.size(); i++)
		{
			Ogre::Vector3 area[4];
			CParticle::KeyFrameData& data = pTarget->m_KeyFrames[i].data;
			Ogre::RenderLines* pDebugger = CEnginAgent::GetInstance()->m_pLineRenderer;

			area[0] = Ogre::Vector3(data.areaw, 0, data.areal);
			area[1] = Ogre::Vector3(data.areaw, 0, -data.areal);
			area[2] = Ogre::Vector3(-data.areaw, 0, -data.areal);
			area[3] = Ogre::Vector3(-data.areaw, 0, data.areal);

			Ogre::Vector3 uparea[4];
			for(int i = 0; i < 4; i++)
			{
				Ogre::Vector3 nor = GetNormalize(area[i]);
				float tan = Ogre::Tan(data.spread);
				if(tan >= 0)
					uparea[i] = nor * tan + Ogre::Vector3(0.0f, 1.0f, 0.0f);
				else
					uparea[i] = -nor * tan + Ogre::Vector3(0.0f, -1.0f, 0.0f);
				Normalize(uparea[i]);
				uparea[i] = area[i] + uparea[i] * 100.0f;
				uparea[i] = uparea[i] * m_pTarget->GetNode()->getWorldMatrix();
				area[i] = area[i] * m_pTarget->GetNode()->getWorldMatrix();
			}

			for(int i = 0; i < 4; i++)
			{
				pDebugger->addLine(area[i], area[(i + 1) % 4],  Ogre::ColorQuad(255, 255, 255, 255));
				pDebugger->addLine(uparea[i], uparea[(i + 1) % 4],  Ogre::ColorQuad(255, 255, 255, 255));
				pDebugger->addLine(area[i], uparea[i],  Ogre::ColorQuad(255, 255, 255, 255));
			}

		}
	}
}

void CParticleInteract::DrawCollisionFace()
{/*
 Ogre::RenderLines* pDebugger = CEnginAgent::GetInstance()->m_pLineRenderer;
 CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
 Ogre::ParticleEmitter* pNode = static_cast<Ogre::ParticleEmitter*>(pTarget->GetNode());

 for(int i = 0; i < (int) pTarget->m_CollisionFaces.size(); i++)
 {
 Ogre::PECollisionFace& face = pTarget->m_CollisionFaces[i];
 Ogre::Vector3 vec[4] = {
 Ogre::Vector3( face.m_fWidth / 2.0f, 0.0f,  face.m_fLength / 2.0f),
 Ogre::Vector3( face.m_fWidth / 2.0f, 0.0f, -face.m_fLength / 2.0f),
 Ogre::Vector3(-face.m_fWidth / 2.0f, 0.0f,  face.m_fLength / 2.0f),
 Ogre::Vector3(-face.m_fWidth / 2.0f, 0.0f, -face.m_fLength / 2.0f)

 };

 Ogre::Matrix4 tm;
 pNode->GetCollisionFaceTrans(face, tm);

 for(int i = 0; i < 4; i++)
 {
 tm.TransformCoord(vec[i], vec[i]);
 }

 pDebugger->addLine(vec[0], vec[1], Ogre::ColorQuad(255, 255, 50));
 pDebugger->addLine(vec[1], vec[3], Ogre::ColorQuad(255, 255, 50));
 pDebugger->addLine(vec[3], vec[2], Ogre::ColorQuad(255, 255, 50));
 pDebugger->addLine(vec[2], vec[0], Ogre::ColorQuad(255, 255, 50));

 //pDebugger->RenderTriangle(vec[0], vec[1], vec[2], Ogre::ColorQuad(170, 170, 50));
 //pDebugger->RenderTriangle(vec[1], vec[2], vec[3], Ogre::ColorQuad(170, 170, 50));
 }
 */
}

void CParticleInteract::DoFrame(Ogre::uint dtick)
{
	float range= m_pTarget->GetRange();
	if(m_pTarget->GetBindInfo()->bDynamic == false)
	{
		if(m_bHovering || m_bCood)
		{
			CEnginAgent::GetInstance()->AddContainerBox(m_pTarget->GetNode()->getWorldPosition(), range, Ogre::ColorQuad(0, 255, 255, 255));
		}
		else if(ms_showState == IS_ALWAYS_SHOW)
		{
			CEnginAgent::GetInstance()->AddContainerBox(m_pTarget->GetNode()->getWorldPosition(), range, Ogre::ColorQuad(255, 255, 180, 255));
		}
	}
	if(m_InteractObjType == PIT_NODE)
	{
		m_pCood->SetFather(m_pTarget->GetNode() , 0);
		m_pCood->SetPosition( Ogre::Vector3(0.0f, 0.0f, 0.0f));
		m_pCood->SetRotation(Ogre::Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
		m_pCood->SetScale(1.0f);
	}
	else
	{
		CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
		Ogre::PECollisionFace& face =  pTarget->m_CollisionFaces[m_iFaceIndex];
		Ogre::SceneNode* pParentNode = GetFaceParent(face);
		m_pCood->SetFather(pParentNode, face.m_iAnchor);
		m_pCood->SetPosition(face.m_Offset);
		m_pCood->SetRotation(face.m_Rotation);
		m_pCood->SetScale(1.0f);
	}

	m_pCood->m_fSize = range;
	m_pCood->UpdateCoord(dtick / 1000.0f);

	if(m_pTarget == NULL)
		return;


	if(m_bCood)
	{
		DrawParticleShape();
		if(m_pTarget->GetNode())
		{
			DrawCollisionFace();
		}
	}
}

void CParticleInteract::ResetInteractObj()
{
	m_iFaceIndex = -1;
	m_InteractObjType = PIT_NODE;
}

void CParticleInteract::SetInteractObj(PARTICLE_INTERACT_TARGET type, int face)
{
	m_InteractObjType = type;
	CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
	if(type == PIT_COLLISION_FACE)
	{
		if(face >= 0 && face < (int)pTarget->m_CollisionFaces.size())
		{
			m_iFaceIndex = face;
		}
		else
		{
			m_InteractObjType = PIT_NODE;
			m_iFaceIndex = -1;
		}
	}
}

Ogre::SceneNode* CParticleInteract::GetFaceParent(Ogre::PECollisionFace& face)
{/*
 Ogre::SceneNode* pOffsetNode = m_pTarget->GetNode();

 if(face.m_bReverseUpLevel == false)
 {
 for(int iLv = 0; iLv < face.m_iUpLevel; iLv++)
 {
 if(pOffsetNode->GetFather() == NULL)
 break;
 pOffsetNode = pOffsetNode->GetFather();
 }
 }
 else if(face.m_bReverseUpLevel == true)
 {
 Ogre::SceneNode* pFront = pOffsetNode->GetFather();
 int iLv = 0;
 while(pFront != NULL)
 {
 pFront = pFront->GetFather();
 iLv++;

 if(iLv > face.m_iUpLevel)
 {
 pOffsetNode = pOffsetNode->GetFather();
 iLv--;
 }

 }
 }
 return pOffsetNode;
 */
	return NULL;
}

void CParticleInteract::OnActive()
{
	m_iFaceIndex = -1;
	m_InteractObjType = PIT_NODE;
}

void CParticleInteract::OnDeactive()
{
}

Ogre::Quaternion CParticleInteract::GetOldRotation()
{
	if(m_InteractObjType == PIT_NODE)
		return CInteract::GetOldRotation();

	CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
	if(m_InteractObjType == PIT_COLLISION_FACE)
	{
		Ogre::PECollisionFace& face =  pTarget->m_CollisionFaces[m_iFaceIndex];
		return face.m_Rotation;
	}
	Ogre::Quaternion idn;
	idn.identity();
	return idn;
}

Ogre::Vector3 CParticleInteract::GetOldPosition()
{
	if(m_InteractObjType == PIT_NODE)
		return CInteract::GetOldPosition();

	CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
	if(m_InteractObjType == PIT_COLLISION_FACE)
	{
		Ogre::PECollisionFace& face =  pTarget->m_CollisionFaces[m_iFaceIndex];
		return face.m_Offset;
	}

	return Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

void CParticleInteract::SetNewRototion(Ogre::Quaternion rot)
{
	if(m_InteractObjType == PIT_NODE)
	{
		CInteract::SetNewRototion(rot);
		return;
	}

	CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
	if(m_InteractObjType == PIT_COLLISION_FACE)
	{
		Ogre::PECollisionFace& face =  pTarget->m_CollisionFaces[m_iFaceIndex];
		face.m_Rotation = rot;
		pTarget->GetDocInfo()->Modify();
	}
}

void CParticleInteract::SetNewPosition(Ogre::Vector3 pos)
{
	if(m_InteractObjType == PIT_NODE)
	{
		CInteract::SetNewPosition(pos);
		return;
	}

	CParticle* pTarget = static_cast<CParticle*>(m_pTarget);
	if(m_InteractObjType == PIT_COLLISION_FACE)
	{
		Ogre::PECollisionFace& face =  pTarget->m_CollisionFaces[m_iFaceIndex];
		face.m_Offset = pos;
		pTarget->GetDocInfo()->Modify();
	}
}

// ----------------------------------------------------------------------------- [11/22/2007]

int CParticle::ms_iInstCount = 0;
CParticle::CParticle(int iBoneID, int iSrcID, unsigned int flags)
: m_pEmitterNode(NULL), m_pSource(NULL),
m_strTextureName("")
{
	m_bindInfo.boneid = iBoneID;
	//this->m_dockingDlg.m_editDlg.SetSource(this);
	m_strName.Format(_T("粒子 %d"), ++ms_iInstCount);
	//m_bHovering = false;
	//m_pCood = NULL;
	//m_bCood = false;
	//m_tran = COOD_NONE;
	//m_interact.Clear();
	m_pEmitterNode = NULL; 
	m_PreUpdateTime = 0;
}

CParticle::~CParticle(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}

bool CParticle::Validate()
{
	int iLastTick = -1;
	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		if(m_KeyFrames[i].data.tick <= iLastTick)
			return false;
		iLastTick = m_KeyFrames[i].data.tick;
	}
	return true;
}

Ogre::SceneNode* CParticle::GetNode()
{
	return m_pEmitterNode;
}

void CParticle::ReleaseNode()
{
	OGRE_RELEASE(m_pEmitterNode);
	m_pEmitterNode = NULL;

	//OGRE_RELEASE(m_pSource);

	//m_interact.Clear();
}

Ogre::SceneNode* CParticle::CreateNode()
{
	ReleaseNode();
	//m_interact.Create(this);
	OGRE_RELEASE(m_pSource);

	m_pSource = static_cast<Ogre::ParticleEmitterData *>(GetChunk());
	if(m_pSource == NULL)
	{
		return NULL;
	}
	m_pSource->m_Desc.m_Flags  &= ~Ogre::PEFLAG_CUSTOM_EMIT; //去除“程序控制发射”标识，否则编辑器里没法看

	m_pEmitterNode = new Ogre::ParticleEmitter(m_pSource);
	m_strLastTexture = m_strTextureName;

	m_pEmitterNode->setPosition(m_bindInfo.offset);
	m_pEmitterNode->setRotation(m_bindInfo.rotate);
	return m_pEmitterNode;
}

void CParticle::RefreshNode()
{
	Ogre::SceneNode* poldnode = m_pEmitterNode;

	if(!m_bVisible)
		return;

	if(poldnode)
	{
		CEffectMgr::GetInstance()->GetModelActor()->unbindObject(poldnode);
	}

	ReleaseNode();

	static int tmp = 1;
	if(tmp == 1)
	{
		Ogre::SceneNode* pnode = CreateNode();

		CEffectMgr::GetInstance()->GetModelActor()->bindObject(m_bindInfo.boneid, pnode);
	}

}

void CParticle::UpdateSource()
{
	if (!m_pSource)
		return;

	//CParticleCommand* pCmd = BeginRecordCommand();

	Ogre::Resource* pOldTex = m_pSource->m_pTexture;

	if(SetToEmitter(m_pSource) == false)
	{
		return;
	}

	if(m_strLastTexture != m_strTextureName)
	{
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTextureName.GetBuffer());
		if(pNewTex != NULL)
		{
			OGRE_RELEASE(m_pSource->m_pTexture);
			m_pSource->m_pTexture = static_cast<Ogre::TextureData *>(pNewTex);
			m_strLastTexture = m_strTextureName;
		}
	}

	if(m_strLastMaskTexture != m_strMaskTextureName)
	{
		if(m_strMaskTextureName == "")
		{
			OGRE_RELEASE(m_pSource->m_pTexture);
		}
		else
		{
			Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTextureName.GetBuffer());
			OGRE_RELEASE(m_pSource->m_pTexture);
			m_pSource->m_pMaskTexture = static_cast<Ogre::TextureData *>(pNewTex);
		}
		m_strLastMaskTexture = m_strMaskTextureName;
	}

	m_pSource->m_Desc.m_Flags &= ~Ogre::PEFLAG_CUSTOM_EMIT;

	if(GetNode() && m_bindInfo.bDynamic == false)
	{
		GetNode()->setPosition(GetBindInfo()->offset);
		GetNode()->setRotation(GetBindInfo()->rotate);
		GetNode()->setScale(GetBindInfo()->scale);
	}
	//EndRecordCommand(pCmd);
	m_docInfo.Modify();
}


CBCGPDockingControlBar* CParticle::CreateBar(CBCGPFrameWnd* pParent)
{
	CParticleBar* pBar = new CParticleBar();
	static int id = 0;
	//pBar->Create(NULL, pParent, CRect (0, 0,  250,600),
	//	TRUE, 0,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);
	//pBar->SetEditTarget(this);
	return pBar;
}

Ogre::Resource* CParticle::GetChunk()
{
	Ogre::ParticleEmitterData *pemitterdata = new Ogre::ParticleEmitterData;

	if(!SetToEmitter(pemitterdata))
	{
		pemitterdata->release();
		return NULL;
	}

	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTextureName.GetBuffer());

	if(pTex == NULL)
	{
		pTex = Ogre::ResourceManager::getSingleton().blockLoad(DEF_FX_TEXTURE);
	}
	pemitterdata->m_pTexture = static_cast<Ogre::TextureData *>(pTex);

	if(m_strMaskTextureName != "")
	{
		pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTextureName.GetBuffer());
		pemitterdata->m_pMaskTexture = static_cast<Ogre::TextureData *>(pTex);
	}
	else
	{
		pemitterdata->m_pMaskTexture = NULL;
	}

	if(m_replaceObjects.size() > 0)
	{
		for(int i = 0; i < (int) m_replaceObjects.size(); i++)
		{
			Ogre::Resource *pres = Ogre::ResourceManager::getSingleton().blockLoad(m_replaceObjects[i].c_str());
			pemitterdata->m_pReplaceObjects.push_back(pres);
		}
	}

	/*
	if(m_replaceEffect.size() > 0)
	{
	Ogre::ComplexChunk* pReplaceObjectRes = new Ogre::ComplexChunk(Ogre::CHUNK_COMPLEX, "ReplaceObjectRes");
	for(int i = 0; i < (int) m_replaceEffect.size(); i++)
	{
	pemitterdata->m_re
	Ogre::Resource* pEffectChunk = m_replaceEffect[i]->GetChunk();
	if(pEffectChunk != NULL)
	pReplaceObjectRes->AddAttrib(pEffectChunk);
	}
	pParticleChunk->AddAttrib(pReplaceObjectRes);
	}
	*/
	return pemitterdata;
}




float CParticle::GetRange()
{
	float maxw = 0.0f;
	float maxl = 0.0f;

	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		if(m_KeyFrames[i].data.areal > maxl) maxl = m_KeyFrames[i].data.areal;
		if(m_KeyFrames[i].data.areaw > maxw) maxw = m_KeyFrames[i].data.areaw;
	}
	return (maxl + maxw) / 2.0f + (m_Desc.m_SizeKeys[0] + m_Desc.m_SizeKeys[0] + m_Desc.m_SizeKeys[0]) / 3.0f;
}
CString CParticle::GetName()
{
	return m_strName;
}

bool CParticle::Refresh()
{

	return true;
}


void CParticle::DoFrame(Ogre::uint dtick) 
{
	//m_interact.DoFrame(dtick);
}

bool CParticle::Create()
{

	m_Desc.m_EmitterType = Ogre::EMITTER_PLANE;	
	m_Desc.m_ParticleType = Ogre::PARTICLE_NORMAL;	
	m_Desc.m_Flags = 0;	
	m_Desc.m_BlendType = 0;	

	m_Desc.m_Pos.x = 0.0f;	
	m_Desc.m_Pos.y = 0.0f;
	m_Desc.m_Pos.z = 0.0f;

	m_Desc.m_MaxParticles = 10;

	m_Desc.m_MidPoint = 0.5f;	
	m_Desc.m_Slowdown = 0.0f;	
	m_Desc.m_Rotation = 0.0f;	
	m_Desc.m_TexRows = 1;	
	m_Desc.m_TexCols = 1;	

	m_Desc.m_ColorKeys[0] = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);	
	m_Desc.m_ColorKeys[1] = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	m_Desc.m_ColorKeys[2] = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	//float m_SizeKeys[3];

	m_Desc.m_SizeKeys[0] = 10.0f;
	m_Desc.m_SizeKeys[1] = 10.0f;
	m_Desc.m_SizeKeys[2] = 10.0f;

	m_Desc.m_AspectRatioKeys[0] = 1.0f;
	m_Desc.m_AspectRatioKeys[1] = 1.0f;
	m_Desc.m_AspectRatioKeys[2] = 1.01f;
	m_Desc.m_TimeSlot = 0.0f;
	m_Desc.m_bFixColor = true;
	m_RotStyle = Ogre::PR_FACE_CAMERA;
	m_Desc.m_Flags |= Ogre::PEFLAG_EX_ROT;

	m_DescEx.m_Collision = Ogre::PC_NO_COLLISION;

	m_DescEx.m_AspectMid = 0.5f;
	m_DescEx.m_ColorMid = 0.5f;
	m_DescEx.m_SizeMid = 0.5f;
	m_DescEx.m_UVRotate = 0.0f;
	m_DescEx.m_UVScale = 1.0f;
	m_DescEx.m_bDistort = false;
	m_DescEx.m_iUVAddressMode = Ogre::UVADDR_WRAP;
	m_DescEx.m_fRandomRot = 0.0f;


	m_DescEx.m_iMaskUVAddrMode = Ogre::UVADDR_CLAMP;
	m_DescEx.m_MaskUVRotate = 0.0f;
	m_DescEx.m_MaskUVScale = 1.0f;
	m_DescEx.m_MaskTexRows = 1;
	m_DescEx.m_MaskTexCols = 1;

	m_DescEx.m_CenterAbsOffset = false;
	m_DescEx.m_CenterOffset = Ogre::Vector2(0,0);
	m_DescExx.m_CollideWithTerrain = false;
	m_PreUpdateTime = 0;

	Ogre::KeyFrameArray<float>::KEYFRAME_T kf1,kf2,kf3;
	Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc1,kfc2,kfc3;

	kf1.tick = 0;		kf2.tick = 50; 		kf3.tick = 100;
	kf1.data = 10.0f;	kf2.data = 10.0f;	kf3.data = 10.0f;
	m_ParticleSizeKey.m_Array.push_back(kf1);
	m_ParticleSizeKey.m_Array.push_back(kf2);
	m_ParticleSizeKey.m_Array.push_back(kf3);

	kf1.tick = 0;		kf2.tick = 50; 		kf3.tick = 100;
	kf1.data = 1.0f;		kf2.data = 1.0f;		kf3.data = 1.0f;
	m_ParticleAspectKey.m_Array.push_back(kf1);
	m_ParticleAspectKey.m_Array.push_back(kf2);
	m_ParticleAspectKey.m_Array.push_back(kf3);

	kf1.tick = 0;		kf2.tick = 50; 		kf3.tick = 100;
	kf1.data = 1.0f;		kf2.data = 1.0f;		kf3.data = 1.0f;
	m_ParticleAlphaKey.m_Array.push_back(kf1);
	m_ParticleAlphaKey.m_Array.push_back(kf2);
	m_ParticleAlphaKey.m_Array.push_back(kf3);

	Ogre::ColourValue defClr = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
	kfc1.tick = 0;		kfc2.tick = 50; 		kfc3.tick = 100;
	kfc1.data = defClr;	kfc2.data = defClr;		kfc3.data = defClr;
	m_ParticleColorKey.m_Array.push_back(kfc1);
	m_ParticleColorKey.m_Array.push_back(kfc2);
	m_ParticleColorKey.m_Array.push_back(kfc3);

	KFItem item;

	item.data.tick = 0;
	item.handle = NULL;
	m_KeyFrames.push_back(item);
	m_strTextureName = DEF_FX_TEXTURE;
	m_strMaskTextureName = "";

	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;
	m_bIsNew = true;
	m_docInfo.NewDoc();
	return true;
}

bool CParticle::Destroy()
{
	return true;
}
bool CParticle::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::ParticleEmitterData* pemitterdata = static_cast<Ogre::ParticleEmitterData *>(pChunk);

	if(pemitterdata->m_pTexture != NULL) m_strTextureName = (const char*)pemitterdata->m_pTexture->getResPath();
	else m_strTextureName = "";

	if(pemitterdata->m_pMaskTexture) m_strMaskTextureName = (const char *)pemitterdata->m_pMaskTexture->getResPath();
	else m_strMaskTextureName = "";

	SetFromEmitter(pemitterdata);


	std::vector<Ogre::Resource*> chunks;
	for(int i = 0; i < (int)pemitterdata->m_pReplaceObjects.size(); i++)
	{
		NEWEFFECT_INFO ni;
		ni.bDynamic = false;
		ni.bIsPath = false;
		ni.bShowPane = true;
		ni.bVisible = false;
		ni.pChunk = pemitterdata->m_pReplaceObjects[i];
		ni.pOwner = this;

		char tmp[256] = {0};
		sprintf(tmp, "Object %d", i);
		ni.path = tmp;
		CEffectUnit* pEffect = (CEffectUnit*)SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);
		if(pEffect != NULL)
			m_replaceEffect.push_back(pEffect);
	}


	m_bIsNew = false;
	m_docInfo.Open();
	return true;

}

bool CParticle::LoadEffect(CString name)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(name.GetBuffer());
	if(pChunk == NULL)
		return false;

	if(!IS_KIND_OF(Ogre::ParticleEmitterData, pChunk))
	{
		pChunk->release();
		return false;
	}
	//Ogre::ParticleEmitterData* pParticleChunk = (Ogre::ParticleEmitterData*)pChunk;
	//
	//CString strTex = (const char*)pParticleChunk->GetAttribByName("Texture")->asString();
	//
	//SetFromEmitter(pemitterdata);
	//m_strTextureName = strTex;
	//m_bIsNew = false;
	LoadFromChunk(pChunk);

	m_strName = name;
	pChunk->release();
	m_docInfo.Open();
	return true;
}

bool CParticle::SaveEffect(CString name)
{
	Ogre::ParticleEmitterData* pemitterdata = new Ogre::ParticleEmitterData();

	if(!SetToEmitter(pemitterdata))
	{
		OGRE_RELEASE( pemitterdata);
		return false;
	}

	pemitterdata->m_pTexture = static_cast<Ogre::TextureData *>(Ogre::ResourceManager::getSingleton().blockLoad(m_strTextureName.GetBuffer()));

	if(m_strMaskTextureName != "")
	{
		pemitterdata->m_pMaskTexture = static_cast<Ogre::TextureData *>(Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTextureName.GetBuffer()));
	}

	if(m_replaceObjects.size() > 0)
	{
		for(int i = 0; i < (int) m_replaceObjects.size(); i++)
		{
			Ogre::Resource *pres = Ogre::ResourceManager::getSingleton().blockLoad(m_replaceObjects[i].c_str());
			pemitterdata->m_pReplaceObjects.push_back(pres);
		}
	}

	/*
	if(m_replaceEffect.size() > 0)
	{
	Ogre::ComplexChunk* pReplaceObjectRes = new Ogre::ComplexChunk(Ogre::CHUNK_COMPLEX, "ReplaceObjectRes");
	for(int i = 0; i < (int) m_replaceEffect.size(); i++)
	{
	Ogre::Resource* pEffectChunk = m_replaceEffect[i]->GetChunk();
	if(pEffectChunk != NULL)
	pReplaceObjectRes->AddAttrib(pEffectChunk);
	}
	pemitterdata->AddAttrib(pReplaceObjectRes);
	}*/

	pemitterdata->save(name.GetBuffer());
	m_strName = name;
	OGRE_RELEASE( pemitterdata);
	m_bIsNew = false;
	m_docInfo.Save();

	return true;
}

void CParticle::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CParticleCommand* pParCmd = (CParticleCommand*) pCmd;
	bool bRefresh = false;

	if(pParCmd->m_newDesc.m_BlendType != m_Desc.m_BlendType ||
		pParCmd->m_newDesc.m_EmitterType != m_Desc.m_EmitterType ||
		pParCmd->m_newDesc.m_ColorKeys[0] != m_Desc.m_ColorKeys[0] ||
		pParCmd->m_newDesc.m_ColorKeys[1] != m_Desc.m_ColorKeys[1] ||
		pParCmd->m_newDesc.m_ColorKeys[2] != m_Desc.m_ColorKeys[2] ||
		pParCmd->m_newDesc.m_MaxParticles != m_Desc.m_MaxParticles ||
		pParCmd->m_newTex != m_strTextureName)
	{
		bRefresh = true;	
	}

	m_Desc = pParCmd->m_newDesc;
	m_strTextureName = pParCmd->m_newTex;
	m_KeyFrames = pParCmd->m_newKf;
	m_bindInfo = pParCmd->m_newBindInfo;
	m_pEmitterNode->setPosition(m_bindInfo.offset);
	m_pEmitterNode->setRotation(m_bindInfo.rotate);
	m_pEmitterNode->setScale(m_bindInfo.scale);
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
CEffectCommand* CParticle::BeginRecordCommand()
{
	CParticleCommand* pUndoCmd = new CParticleCommand();

	pUndoCmd->m_newDesc = m_Desc;
	pUndoCmd->m_newKf = m_KeyFrames;
	pUndoCmd->m_newTex = m_strTextureName;
	pUndoCmd->m_newBindInfo = m_bindInfo;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd;
}
void CParticle::EndRecordCommand(CEffectCommand* pcmd)
{
	CParticleCommand* pUndoCmd = static_cast<CParticleCommand*>(pcmd);
	pUndoCmd->m_oldDesc = m_Desc;
	pUndoCmd->m_oldKf = m_KeyFrames;
	pUndoCmd->m_oldTex = m_strTextureName;
	pUndoCmd->m_oldBindInfo = m_bindInfo;
	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}

//bool CParticle::IsVisible()
//{
//	return m_bVisible;
//}
//void CParticle::SetVisible(bool visible)
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
//
//}

BIND_INFO* CParticle::GetBindInfo()
{
	return &m_bindInfo;
}

void CParticle::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

bool CParticle::SetToEmitter(Ogre::ParticleEmitterData* pEmitter)
{
	if(m_Desc.m_TexCols <= 0 || m_Desc.m_TexRows <= 0)
		return false;
	if(m_DescEx.m_MaskTexCols <= 0 || m_DescEx.m_MaskTexCols <= 0)
		return false;

	if((int)m_KeyFrames.size() <= 0)
		return false;


	//set
	pEmitter->m_Desc = m_Desc;
	pEmitter->m_DescEx = m_DescEx;

	pEmitter->m_Desc.m_Flags |= Ogre::PEFLAG_MORE;

	pEmitter->m_DescExx = m_DescExx;

	pEmitter->m_PreTime = m_PreUpdateTime;

	pEmitter->m_ParticleAlphaKey = m_ParticleAlphaKey;
	pEmitter->m_ParticleColorKey = m_ParticleColorKey;
	pEmitter->m_ParticleSizeKey = m_ParticleSizeKey;
	pEmitter->m_ParticleAspectKey = m_ParticleAspectKey;

	int iLastTick = -1;
	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		if(m_KeyFrames[i].data.tick <= iLastTick)
			return false;
		if(m_KeyFrames[i].data.lifespan < -0.0001f)
			return false;

		iLastTick = m_KeyFrames[i].data.tick;
	}

	for(int i = 0; i < (int) m_CollisionFaces.size(); i++)
	{
		if(m_CollisionFaces[i].m_iUpLevel < 0)
			return false;
	}
	pEmitter->m_CollisionFaces = m_CollisionFaces;

	//pEmitter->m_pReplaceObjects.clear();
	//for(int i = 0; i < (int)m_replaceObjects.size(); i++)
	//{
	//	Ogre::Resource* pres = pResLoader->BlockLoad(m_replaceObjects[i].c_str());
	//	if(pres)
	//		pEmitter->m_pReplaceObjects.push_back(pres);
	//}

	for(int i = 0; i < (int)pEmitter->m_pReplaceObjects.size(); i++)
	{
		pEmitter->m_pReplaceObjects[i]->release();
	}
	pEmitter->m_pReplaceObjects.clear();

	for(int i = 0; i < (int)m_replaceEffect.size(); i++)
	{
		Ogre::Resource* pres = m_replaceEffect[i]->GetSource();
		if(pres)
		{
			pres->addRef();
			pEmitter->m_pReplaceObjects.push_back(static_cast<Ogre::Resource *>(pres));
		}
	}



	Ogre::KeyFrameArray<float> initkf;
	Ogre::KeyFrameArray<Ogre::Vector3> initkfv;
	pEmitter->m_Speed = initkf;
	pEmitter->m_SpeedVariation = initkf;
	pEmitter->m_Spread = initkf;
	pEmitter->m_Latitude = initkf;
	pEmitter->m_Acceleration = initkf;
	pEmitter->m_AccelerationDir = initkfv;
	pEmitter->m_Lifespan = initkf;
	pEmitter->m_Rate = initkf;
	pEmitter->m_AreaLength = initkf;
	pEmitter->m_AreaWidth = initkf;
	pEmitter->m_AreaDepth = initkf;
	pEmitter->m_Deacceleration = initkf;
	pEmitter->m_SpreadOffset = initkf;
	pEmitter->m_SizeVariation = initkf;
	pEmitter->m_LifeVariation = initkf;
	pEmitter->m_RandomSpread = initkf;


	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::Vector3>::KEYFRAME_T kfv;
		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.speed;
		pEmitter->m_Speed.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.speedvar / 100.0f;
		pEmitter->m_SpeedVariation.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.spread;
		pEmitter->m_Spread.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.latitude;
		pEmitter->m_Latitude.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.accelerate;
		pEmitter->m_Acceleration.m_Array.push_back(kf);

		kfv.tick = m_KeyFrames[i].data.tick;
		kfv.data = m_KeyFrames[i].data.acceleratedir;
		pEmitter->m_AccelerationDir.m_Array.push_back(kfv);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.lifespan;
		pEmitter->m_Lifespan.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.rate;
		pEmitter->m_Rate.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.areal;
		pEmitter->m_AreaLength.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.areaw;
		pEmitter->m_AreaWidth.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.aread;
		pEmitter->m_AreaDepth.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.deacceleration;
		pEmitter->m_Deacceleration.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.spreadoffset;
		pEmitter->m_SpreadOffset.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.sizevar / 100.0f;
		pEmitter->m_SizeVariation.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.lifevar / 100.0f;
		pEmitter->m_LifeVariation.m_Array.push_back(kf);


		kf.tick = m_KeyFrames[i].data.tick;
		kf.data = m_KeyFrames[i].data.randomspread;
		pEmitter->m_RandomSpread.m_Array.push_back(kf);
	}

	pEmitter->m_Desc.m_Flags |= Ogre::PEFLAG_EX_ROT;
	pEmitter->m_RotStyle = m_RotStyle;

	return true;
}

void CParticle::CopyData(Ogre::ParticleEmitterData* pEmitter)
{
}


bool CParticle::SetFromEmitter(Ogre::ParticleEmitterData* pEmitter)
{
	m_KeyFrames.clear();
	m_Desc = pEmitter->m_Desc;
	m_CollisionFaces = pEmitter->m_CollisionFaces;
	m_DescEx = pEmitter->m_DescEx;
	m_DescExx = pEmitter->m_DescExx;
	m_PreUpdateTime = pEmitter->m_PreTime;

	m_ParticleAlphaKey = pEmitter->m_ParticleAlphaKey;
	m_ParticleColorKey = pEmitter->m_ParticleColorKey;
	m_ParticleSizeKey = pEmitter->m_ParticleSizeKey;
	m_ParticleAspectKey = pEmitter->m_ParticleAspectKey;

	for(int i = 0; i < (int)pEmitter->m_Speed.m_Array.size(); i++)
	{
		KFItem item;
		item.data.tick		= pEmitter->m_Speed.m_Array[i].tick;
		item.data.speed		= pEmitter->m_Speed.m_Array[i].data;
		item.data.speedvar	= pEmitter->m_SpeedVariation.m_Array[i].data * 100;
		item.data.spread	= pEmitter->m_Spread.m_Array[i].data;
		item.data.latitude	= pEmitter->m_Latitude.m_Array[i].data;
		item.data.accelerate= pEmitter->m_Acceleration.m_Array[i].data;
		item.data.acceleratedir = pEmitter->m_AccelerationDir.m_Array[i].data;
		item.data.lifespan	= pEmitter->m_Lifespan.m_Array[i].data;
		item.data.rate		= pEmitter->m_Rate.m_Array[i].data;
		item.data.areal		= pEmitter->m_AreaLength.m_Array[i].data;
		item.data.areaw	= pEmitter->m_AreaWidth.m_Array[i].data;

		if (pEmitter->m_version >= 103)
			item.data.aread = pEmitter->m_AreaDepth.m_Array[i].data;
		else
			item.data.aread	= 0.0f;			

		item.data.deacceleration = pEmitter->m_Deacceleration.m_Array[i].data;
		item.data.spreadoffset = pEmitter->m_SpreadOffset.m_Array[i].data;

		item.data.sizevar	= pEmitter->m_SizeVariation.m_Array[i].data * 100;
		item.data.lifevar	= pEmitter->m_LifeVariation.m_Array[i].data * 100;

		item.data.randomspread = pEmitter->m_RandomSpread.m_Array[i].data;

		m_KeyFrames.push_back(item);
	}
	if((m_Desc.m_Flags & Ogre::PEFLAG_EX_ROT) != 0)
	{
		m_RotStyle = pEmitter->m_RotStyle;
	}
	else
	{
		m_RotStyle = Ogre::PR_FACE_CAMERA;
	}


	return true;
}

void CParticle::SelectCollisionFace(Ogre::PECollisionFace* pFace)
{
	//for(int i = 0; i < (int)m_CollisionFaces.size(); i++)
	//{
	//	if(pFace == &m_CollisionFaces[i])
	//	{
	//		m_interact.SetInteractObj(PIT_COLLISION_FACE, i);
	//		return;
	//	}
	//}

	//m_interact.SetInteractObj(PIT_NODE);
}

void CParticle::AddReplaceObject(CString strRes)
{
	NEWEFFECT_INFO ni;
	ni.animmode = Ogre::BAM_SELF;
	ni.bDynamic = false;
	ni.bIsPath = true;
	ni.bShowPane = true;
	ni.path = strRes.GetBuffer();
	ni.realpath = strRes.GetBuffer();
	ni.bVisible = false;
	ni.pOwner = this;
	CEffectUnit* pUnit = (CEffectUnit*)SendMainFrameMsg(EEM_REQ_LOAD_EFFECT, 0, (LPARAM)&ni);

	if(pUnit != NULL)
	{
		m_replaceEffect.push_back(pUnit);
	}
}

void CParticle::AddReplaceObject(CEffectUnit* pUnit)
{
	if(pUnit != NULL)
	{
		pUnit->SetOwnerEffect(this);
		m_replaceEffect.push_back(pUnit);
	}

	CEffectMgr::GetInstance()->RefreshEffect(this);
}

void CParticle::RemoveReplaceObject(CEffectUnit* pUnit)
{
	//主动删除
	for(int i = 0; i < (int)m_replaceEffect.size(); i++)
	{
		if(m_replaceEffect[i] == pUnit)
		{
			pUnit->SetOwnerEffect(NULL);
			m_replaceEffect.erase(m_replaceEffect.begin() + i);

			SendMainFrameMsg(EEM_DELETE_EFFECT, 0, (LPARAM)pUnit);

		}
	}
	CEffectMgr::GetInstance()->RefreshEffect(this);
}

void CParticle::OnBelongingRemoved(CEffectUnit* pUnit)
{
	//被动删除
	for(int i = 0; i < (int)m_replaceEffect.size(); i++)
	{
		if(m_replaceEffect[i] == pUnit)
		{
			m_replaceEffect.erase(m_replaceEffect.begin() + i);
		}
	}
	CEffectMgr::GetInstance()->RefreshEffect(this);
}

// ---------------------CParticleCommand----------------------- [5/15/2008]
CEffectUnit* CParticleCommand::GetTarget()
{
	return m_pTarget;
}

bool CParticleCommand::NeedRefresh()
{
	return false;
}



CEffectCommand* CParticleCommand::GetInversCommand()
{
	CParticleCommand* pCmd = new CParticleCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newDesc = m_oldDesc;
	pCmd->m_oldDesc = m_newDesc;

	pCmd->m_newKf = m_oldKf;
	pCmd->m_oldKf = m_newKf;

	pCmd->m_newTex = m_oldTex;
	pCmd->m_oldTex = m_newTex;

	pCmd->m_newBindInfo = m_oldBindInfo;
	pCmd->m_oldBindInfo = m_newBindInfo;
	return pCmd;
}

bool CParticleCommand::IsIdentical()
{
	if(memcmp(&m_newDesc, &m_oldDesc, sizeof(Ogre::PEmitterDesc)) != 0)
		return false;

	if(m_oldTex != m_newTex)
		return false;

	if(m_oldKf.size() != m_newKf.size())
		return false;

	for(int i = 0; i < (int) m_oldKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &m_newKf[i], sizeof(CParticle::KFItem)) != 0)
			return false;
	}

	if(m_oldBindInfo != m_newBindInfo)
		return false;

	return true;
}

bool CParticleCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	//TRACE("\nBegin Merge\n");
	if(pCmd->GetTarget() != m_pTarget)
		return false;

	CParticleCommand* pParCmd = (CParticleCommand*)pCmd;

	if(m_oldKf.size() != pParCmd->m_oldKf.size())
		return false;
	for(int i = 0; i < (int) m_newKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &(pParCmd->m_oldKf[i]), sizeof(CParticle::KFItem)) != 0)
			return false;
	}
	if(m_oldTex != pParCmd->m_oldTex)
		return false;


	if(m_oldDesc.m_EmitterType != pParCmd->m_oldDesc.m_EmitterType) return false;
	if(m_oldDesc.m_ParticleType != pParCmd->m_oldDesc.m_ParticleType) return false;
	if(m_oldDesc.m_Flags != pParCmd->m_oldDesc.m_Flags) return false;
	if(m_oldDesc.m_BlendType != pParCmd->m_oldDesc.m_BlendType) return false;
	//if(m_oldDesc.m_Pos != pParCmd->m_oldDesc.m_Pos) return false;
	if(m_oldDesc.m_MaxParticles != pParCmd->m_oldDesc.m_MaxParticles) return false;
	if(m_oldDesc.m_MidPoint != pParCmd->m_oldDesc.m_MidPoint) return false;
	if(m_oldDesc.m_Slowdown != pParCmd->m_oldDesc.m_Slowdown) return false;
	if(m_oldDesc.m_Rotation != pParCmd->m_oldDesc.m_Rotation) return false;
	if(m_oldDesc.m_TexRows != pParCmd->m_oldDesc.m_TexRows) return false;
	if(m_oldDesc.m_TexCols != pParCmd->m_oldDesc.m_TexCols) return false;
	if(m_oldDesc.m_TimeSlot != pParCmd->m_oldDesc.m_TimeSlot) return false;
	if(m_oldDesc.m_bFixColor != pParCmd->m_oldDesc.m_bFixColor) return false;
	if(m_oldDesc.m_ColorKeys[0] != pParCmd->m_oldDesc.m_ColorKeys[0]) return false;
	if(m_oldDesc.m_ColorKeys[1] != pParCmd->m_oldDesc.m_ColorKeys[1]) return false;
	if(m_oldDesc.m_ColorKeys[2] != pParCmd->m_oldDesc.m_ColorKeys[2]) return false;
	if(m_oldDesc.m_SizeKeys[0] != pParCmd->m_oldDesc.m_SizeKeys[0]) return false;
	if(m_oldDesc.m_SizeKeys[1] != pParCmd->m_oldDesc.m_SizeKeys[1]) return false;
	if(m_oldDesc.m_SizeKeys[2] != pParCmd->m_oldDesc.m_SizeKeys[2]) return false;
	if(m_oldDesc.m_AspectRatioKeys[0] != pParCmd->m_oldDesc.m_AspectRatioKeys[0]) return false;
	if(m_oldDesc.m_AspectRatioKeys[1] != pParCmd->m_oldDesc.m_AspectRatioKeys[1]) return false;
	if(m_oldDesc.m_AspectRatioKeys[2] != pParCmd->m_oldDesc.m_AspectRatioKeys[2]) return false;

	if(m_oldBindInfo.boneid != pParCmd->m_oldBindInfo.boneid)
		return false;

	m_oldBindInfo.offset = pParCmd->m_oldBindInfo.offset;
	//m_oldDesc.m_Pos = pParCmd->m_oldDesc.m_Pos;

	//TRACE("Finish Merge\n");
	return true;
}


