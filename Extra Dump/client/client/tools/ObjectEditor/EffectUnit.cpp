
#include "stdafx.h"
#include "EffectUnit.h"
#include "EffectManager.h"
#include "BindInfo.h"
#include "EffectUnit_Particle.h"
#include "EffectUnit_Light.h"
#include "EffectUnit_Billboard.h"
#include "EffectUnit_Beam.h"
#include "EffectUnit_VirtualNode.h"
#include "EffectUnit_Ribbon.h"
#include "EffectUnit_Sound.h"
#include "EffectUnit_Decal.h"
#include "EffectUnit_UnknownObject.h"
#include "EffectUnit_Geometry.h"
#include "ObjectTypeHelper.h"

#include "OgreEntity.h"
#include "OgreResourceManager.h"
#include "OgreParametricShapeData.h"

void CKeyFramePropertyObject::CopyKeyFrame(int from, int to)
{
	if(from < 0 || from > GetKeyFrameCount() 
		|| to < 0 || to > GetKeyFrameCount()  )
	{
		return;
	}

	CEffectKeyFrame* pFromKeyFrame = GetKeyFrame(from);
	CEffectKeyFrame* pToKeyFrame = GetKeyFrame(to);
	if(pFromKeyFrame != NULL && pToKeyFrame != NULL)
	{
		pFromKeyFrame->CopyFrameData(pToKeyFrame);
	}
}

int CKeyFramePropertyObject::AddKeyFrame(float fTime)
{
	CEffectKeyFrame* pTmpFrame = CreateKeyFrame();
	pTmpFrame->SetTime(fTime);

	int i = 0;
	for(i = 0; i < GetKeyFrameCount(); i++)
	{
		if(GetKeyFrameTime(i) > fTime)
			break;
	}

	if(i >= GetKeyFrameCount())
	{
		CEffectKeyFrame* pLastFrame = GetKeyFrame(GetKeyFrameCount() - 1);
		*pTmpFrame = *pLastFrame;
		InsertKeyFrame(pTmpFrame, GetKeyFrameCount());
	}
	else
	{
		CEffectKeyFrame* pNextFrame = GetKeyFrame(i);
		CEffectKeyFrame* pPrevFrame = GetKeyFrame(i);

		float pos = (float)(fTime - pPrevFrame->GetTime() ) / ( pNextFrame->GetTime() - pPrevFrame->GetTime());

		pPrevFrame->GetLerpKeyFrame(pNextFrame, pTmpFrame);
		InsertKeyFrame(pTmpFrame, i);
	}

	return i;

}



void CKeyFramePropertyObject::SetKeyFramePos(int index, float fTime)
{
	if(index >= 0 && index < GetKeyFrameCount())
	{
		CEffectKeyFrame* pFrame = GetKeyFrame(index);
		pFrame->SetTime(fTime);
	}
}


float CKeyFramePropertyObject::GetHotTime() 
{ 
	return m_fHotTime; 
}

void CKeyFramePropertyObject::SetHotTime(float fTime)
{
	if(GetKeyFrameCount() == 0)
		return;

	int i = 0;
	for(i = 0; i < GetKeyFrameCount(); i++)
	{
		float frameTime = GetKeyFrameTime(i);
		if(frameTime > fTime)
			break;
	}

	CEffectKeyFrame* pSrcFrame = GetKeyFrame(i);
	CEffectKeyFrame* pSrcNextFrame = GetKeyFrame(i + 1);
	if(pSrcNextFrame == NULL)
	{
		CEffectKeyFrame* pLastFrame = GetKeyFrame(GetKeyFrameCount() - 1);
		*m_pTempFrame = *pLastFrame;
	}
	else
		pSrcFrame->GetLerpKeyFrame(pSrcNextFrame, m_pTempFrame);
	m_fHotTime = fTime;
}


CEffectUnit::CEffectUnit(void)
{
	//m_pExternInteractObject = NULL;
	m_pOwnerEffect = NULL;
	m_bVisible = true;
}

CEffectUnit::~CEffectUnit(void)
{
}

void CEffectUnit::SetVisible(bool visible)
{
	if(m_bVisible == visible)
		return;

	m_bVisible = visible;

	if(GetNode())
	{
		if(m_bVisible)
		{
			CEffectMgr::GetInstance()->GetModelActor()->bindObject(GetBindInfo()->boneid, GetNode());
		}
		else
		{
			CEffectMgr::GetInstance()->GetModelActor()->unbindObject(GetNode());
		}
	}
}

bool CEffectUnit::IsVisible()
{
	return m_bVisible;
}

void CEffectUnit::BindToModel(Ogre::Entity* pModel, int anchor)
{
	if(pModel == NULL)
		return;

	if(IsModel())
	{
		pModel->bindObject(anchor, GetModel(), 0);
		GetModel()->setPosition(GetBindInfo()->offset);
		GetModel()->setRotation(GetBindInfo()->rotate);
		GetModel()->setScale(GetBindInfo()->scale);
	}
	else
	{
		pModel->bindObject(anchor, GetNode(), 0);
		GetNode()->setPosition(GetBindInfo()->offset);
		GetNode()->setRotation(GetBindInfo()->rotate);
		GetNode()->setScale(GetBindInfo()->scale);
	}
}

void CEffectUnit::UnbindFromModel(Ogre::Entity* pModel)
{
	if(pModel == NULL)
		return;

	if(IsModel())
		pModel->unbindObject(GetModel());
	else
		pModel->unbindObject(GetNode());
}


CString	CEffectUnit::GetName()
{
	return m_strName;
}

void CEffectUnit::SetName(CString strName)
{
	m_strName = strName;
}

Ogre::SceneNode* CEffectUnit::GetNode()
{
	return m_pNode;
}

void CEffectUnit::ReleaseNode()
{
}

CEffectUnit* CEffectUnit::CreateEffect(EFFECT_TYPE type)
{
	switch(type) 
	{
	case EFFECT_PEMITTER:		return new CParticle();
	case EFFECT_LIGHT:			return new CLight();
	case EFFECT_BILLBOARD:		return new CBillboard();
	case EFFECT_RIBBON:			return new CRibbon();
	case EFFECT_BEAM:			return new CBeam();
	case EFFECT_SOUND:			return new CSound();
	case EFFECT_DECAL:			return new CDecal();
	case EFFECT_UNKNOWN:		return new CUnknownObject();
	//case EFFECT_TEST:			return new CTestObj();
	case EFFECT_GEOM:			return new CGeometry();
	case EFFECT_VIRTUALNODE:	return new CVirtualNode();
	}
	assert(0);
	return NULL;
}

//CEffectUnit* CEffectUnit::CreateEffectByBindType(Ogre::ACTOR_BIND_T bindtype)
//{
//	switch(bindtype) 
//	{
//	case Ogre::ACTOR_BIND_PEMITTER:	return new CParticle();
//	case Ogre::ACTOR_BIND_LIGHT:		return new CLight();
//	case Ogre::ACTOR_BIND_BILLBOARD:	return new CBillboard();
//	case Ogre::ACTOR_BIND_RIBBON:	return new CRibbon();
//	case Ogre::ACTOR_BIND_BEAMEMITTER: return new CBeam();
//	}
//	return new CUnknownObject();
//}

CEffectUnit* CEffectUnit::LoadEffectFromFile(CString strPath)
{
	CCvtHelper cvt;
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(strPath.GetBuffer());
	if(!pChunk)
		return NULL;

	CEffectUnit* pNewEffect = CEffectUnit::CreateEffect(cvt.ChunkTypeToEffectType(pChunk));
	if(pNewEffect == NULL)
	{
		OGRE_RELEASE(pChunk);
		return NULL;
	}

	if(false == pNewEffect->LoadFromChunk(pChunk))
	{
		OGRE_RELEASE(pChunk);
		return NULL;
	}
	//pNewEffect->SetName(strPath);
	//pNewEffect->SetResPath(strPath);
	OGRE_RELEASE(pChunk);
	return pNewEffect;
}
CEffectUnit* CEffectUnit::LoadEffectFromChunk(Ogre::Resource* pChunk)
{
	CCvtHelper cvt;

	if(!pChunk)
		return NULL;

	CEffectUnit* pNewEffect = CEffectUnit::CreateEffect(cvt.ChunkTypeToEffectType(pChunk));
	if(pNewEffect == NULL)
		return NULL;

	if(false == pNewEffect->LoadFromChunk(pChunk))
		return NULL;

	return pNewEffect;
}
//
//Ogre::Resource* CEffectUnit::GetResourceFromBindInfo(Ogre::Model::BINDOBJ_T* pBind)
//{
//	switch(pBind->bindtype) 
//	{
//	case Ogre::ACTOR_BIND_LIGHT:			return pBind->plight;
//	case Ogre::ACTOR_BIND_SKIN:				
//	case Ogre::ACTOR_BIND_MESH:			return pBind->pmesh;
//	case Ogre::ACTOR_BIND_CAMERA:		return pBind->pcamera;
//	case Ogre::ACTOR_BIND_PEMITTER:		return pBind->pemitter;
//	case Ogre::ACTOR_BIND_BILLBOARD:		return pBind->pbillboard;
//	case Ogre::ACTOR_BIND_RIBBON:		return pBind->pribbon;
//	case Ogre::ACTOR_BIND_BEAMEMITTER:	return pBind->pbeamemitter;
//	}
//	return NULL;
//}

/*
Ogre::Resource* CEffectUnit::GetResourceFromChunk(Ogre::ACTOR_BIND_T bindtype, Ogre::Resource* pChu)
{
switch(bindtype) 
{
case Ogre::ACTOR_BIND_LIGHT:		return ((Ogre::LightChunk*) pChu)->m_pLight;
case Ogre::ACTOR_BIND_SKIN:		
case Ogre::ACTOR_BIND_MESH:		return((Ogre::MeshChunk*) pChu)->m_pMesh;
case Ogre::ACTOR_BIND_CAMERA:	return ((Ogre::CameraChunk*) pChu)->m_pCamera;
case Ogre::ACTOR_BIND_PEMITTER:	return ((Ogre::PEmitterChunk*) pChu)->m_pEmitter;
case Ogre::ACTOR_BIND_BILLBOARD:	return ((Ogre::BillboardChunk*) pChu)->m_pBillboard;
case Ogre::ACTOR_BIND_RIBBON:	return ((Ogre::RibbonChunk*) pChu)->m_pRibbon;
case Ogre::ACTOR_BIND_BEAMEMITTER:return ((Ogre::BeamEmitterChunk*) pChu)->m_pBeamEmitter;

}

return NULL;
}*/

/*
Ogre::Resource* CEffectUnit::GetResourceFromChunk(Ogre::Resource* pChu)
{
Ogre::CHUNKTYPE_T type = pChu->GetType();
switch(type) 
{
case Ogre::CHUNK_LIGHT:			return ((Ogre::LightChunk*) pChu)->m_pLight;
case Ogre::CHUNK_SKINACTOR:		return ((Ogre::SkinActorChunk*)pChu)->m_pSkinActor;
case Ogre::CHUNK_MESH:			return ((Ogre::MeshChunk*) pChu)->m_pMesh;
case Ogre::CHUNK_CAMERA:			return ((Ogre::CameraChunk*) pChu)->m_pCamera;
case Ogre::CHUNK_PEMITTER:		return ((Ogre::PEmitterChunk*) pChu)->m_pEmitter;
case Ogre::CHUNK_BILLBOARD:		return ((Ogre::BillboardChunk*) pChu)->m_pBillboard;
case Ogre::CHUNK_RIBBON:			return ((Ogre::RibbonChunk*) pChu)->m_pRibbon;
case Ogre::CHUNK_BEAMEMITTER:	return ((Ogre::BeamEmitterChunk*) pChu)->m_pBeamEmitter;
case Ogre::CHUNK_SOUND		:	return ((Ogre::SoundChunk*) pChu)->m_pSound;
case Ogre::CHUNK_DECAL:			return ((Ogre::DecalChunk*)pChu)->m_pSource;
case Ogre::CHUNK_MODEL:			return ((Ogre::ModelChunk*)pChu)->m_pModelSource;
case Ogre::CHUNK_GEOMETRY:		return ((Ogre::GeometryChunk*)pChu)->m_pSource;
case Ogre::CHUNK_MESHSET:		return (Ogre::Resource*)(((Ogre::MeshSetChunk*)pChu)->m_pMeshSet);
case Ogre::CHUNK_VIRTUAL:		return (Ogre::Resource*)(((Ogre::VirtualNodeChunk*)pChu)->m_pVirtualNode);
}
return NULL;
}*/

//void CEffectUnit::SetResByBindtype(Ogre::Model::BINDOBJ_T* pBind, Ogre::Resource* pRes)
//{
//	switch(pBind->bindtype) 
//	{
//	case Ogre::ACTOR_BIND_LIGHT:
//		pBind->plight = (Ogre::LightData*)pRes;
//		break;
//	case Ogre::ACTOR_BIND_SKIN:
//	case Ogre::ACTOR_BIND_MESH:
//		pBind->pmesh = (Ogre::MeshSource*)pRes;
//		break;
//	case Ogre::ACTOR_BIND_CAMERA:
//		pBind->pcamera = (Ogre::CameraSource*)pRes;
//		break;
//	case Ogre::ACTOR_BIND_PEMITTER:
//		pBind->pemitter = (Ogre::ParticleEmitterData*)pRes;
//		break;
//	case Ogre::ACTOR_BIND_BILLBOARD:
//		pBind->pbillboard = (Ogre::BillboardData*)pRes;
//		break;
//	case Ogre::ACTOR_BIND_RIBBON:
//		pBind->pribbon = (Ogre::RibbonEmitterData*)pRes;
//		break;
//	case Ogre::ACTOR_BIND_BEAMEMITTER:
//		pBind->pbeamemitter = (Ogre::BeamEmitterData*)pRes;
//		break;
//	default:
//		pBind->pcamera = NULL;
//		break;
//	}
//}

//bool CEffectUnit::CanEdit(Ogre::ACTOR_BIND_T restype)
//{
//	if(restype == Ogre::ACTOR_BIND_LIGHT ||
//		restype == Ogre::ACTOR_BIND_PEMITTER ||
//		restype == Ogre::ACTOR_BIND_BILLBOARD ||
//		restype == Ogre::ACTOR_BIND_RIBBON ||
//		restype == Ogre::ACTOR_BIND_BEAMEMITTER)
//	{
//		return true;
//	}
//	return false;
//}
bool CEffectUnit::CanEdit(Ogre::Resource *pres)
{
	if(IS_KIND_OF(Ogre::ParticleEmitterData, pres) ||
		IS_KIND_OF(Ogre::BillboardData, pres) ||
		IS_KIND_OF(Ogre::LightData, pres) ||
		IS_KIND_OF(Ogre::RibbonEmitterData, pres) ||
		IS_KIND_OF(Ogre::BeamEmitterData, pres) ||
		IS_KIND_OF(Ogre::DecalData, pres) ||
		IS_KIND_OF(Ogre::SoundData, pres) ||
		IS_KIND_OF(Ogre::ModelData, pres) ||
		IS_KIND_OF(Ogre::EntityData, pres) ||
		IS_KIND_OF(Ogre::ParamShapeData, pres) ||
		IS_KIND_OF(Ogre::DummyNodeData, pres) ||
		IS_KIND_OF(Ogre::LightData, pres)
		)
	{
		return true;
	}
	else return false;
}