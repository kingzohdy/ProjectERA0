
#include "stdafx.h"
#include "EffectUnit_Geometry.h"
#include "EffectManager.h"
#include "EffectUnitEditBar_Geometry.h"

#include "OgreResourceManager.h"
#include "OgreTexture.h"
#include "OgreEntity.h"


CGeometry::CGeometry(void)
{
	m_pNode = NULL;
	m_pSource = NULL;
}

CGeometry::~CGeometry(void)
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
}
//EFFECT_RIBBON


float CGeometry::GetRange()
{
	return 20.0f; //(m_KeyFrames[0].above + m_KeyFrames[0].below) / 4;
}

void CGeometry::DoFrame(Ogre::uint dtick) 
{
	m_interact.DoFrame(dtick);
}

void CGeometry::ReleaseNode()
{
	OGRE_RELEASE(m_pNode);
	m_pNode = NULL;


	m_interact.Clear();
}

Ogre::SceneNode* CGeometry::CreateNode()
{
	ReleaseNode();
	OGRE_RELEASE(m_pSource);
	m_interact.Create(this);

	m_pSource = (Ogre::ParamShapeData*)GetChunk();
	if(m_pSource == NULL) return NULL;

	Ogre::ParametricShape* pNode = new Ogre::ParametricShape(m_pSource);

	m_pNode = pNode;
	m_strLastTexture = m_strTexture;
	m_strLastMaskTexture = m_strTexture;
	m_pNode->setPosition(m_bindInfo.offset);
	m_pNode->setRotation(m_bindInfo.rotate);
	m_pNode->setScale(m_bindInfo.scale);

	return m_pNode;

}

void CGeometry::RefreshNode()
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

void CGeometry::UpdateSource()
{
	if (!m_pSource)
		return;

	Ogre::Resource* pOldTex = m_pSource->m_pTextureRes;
	SetKfToSource(m_pSource);
	m_pSource->m_iBlendMode = m_iBlendMode;
	m_pSource->m_GeoType = m_GeomType;
	if(m_iTexCol > 0)
		m_pSource->m_iTexCol = m_iTexCol;
	if(m_iTexRow > 0)
		m_pSource->m_iTexRow = m_iTexRow;
	m_pSource->m_fFrameTime = m_fFrameTime;

	m_pSource->m_bDistort = m_bDistort;

	//if(m_strLastTexture != m_strTexture)
	//{
	//	Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	//	if(pNewTex != NULL)
	//	{
	//		if(pOldTex != NULL)
	//			pOldTex->Release();
	//		m_pSource->m_pTextureRes = static_cast<Ogre::TextureData*>(pNewTex);
	//		m_strLastTexture = m_strTexture;
	//	}
	//}

	//if(m_strLastMaskTexture != m_strMaskTexture)
	//{
	//	Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
	//	if(pNewTex != NULL)
	//	{
	//		if(pOldTex != NULL)
	//			pOldTex->Release();
	//		m_pSource->m_pMaskTextureRes = static_cast<Ogre::TextureData*>(pNewTex);
	//	}
	//	m_strLastMaskTexture = m_strMaskTexture;
	//}

	if(m_strLastTexture != m_strTexture)
	{
		OGRE_RELEASE(m_pSource->m_pTextureRes);
		Ogre::Resource *pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			if(IS_KIND_OF(Ogre::TextureData, pNewTex)) m_pSource->m_pTextureRes = static_cast<Ogre::TextureData*>(pNewTex);
			else pNewTex->release();
		}
		m_strLastTexture = m_strTexture;
	}

	if(m_strLastMaskTexture != m_strMaskTexture)
	{
		OGRE_RELEASE(m_pSource->m_pMaskTextureRes);
		Ogre::Resource* pNewTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
		if(pNewTex != NULL)
		{
			if(IS_KIND_OF(Ogre::TextureData, pNewTex)) m_pSource->m_pMaskTextureRes = static_cast<Ogre::TextureData*>(pNewTex);
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

CBCGPDockingControlBar* CGeometry::CreateBar(CBCGPFrameWnd* pParent)
{
	CGeometryBar* pBar = new CGeometryBar();
	//assert(0 && "CGeometry's edit bar has not been finish yet");
	return pBar;
}

Ogre::Resource* CGeometry::GetChunk()
{
	Ogre::ParamShapeData* pChu = new Ogre::ParamShapeData();

	SetKfToSource(pChu);
	pChu->m_iBlendMode = m_iBlendMode;
	pChu->m_GeoType = m_GeomType;
	if(m_iTexCol > 0)
		pChu->m_iTexCol = m_iTexCol;
	if(m_iTexRow > 0)
		pChu->m_iTexRow = m_iTexRow;

	if(m_iMaskTexCols > 0)
		pChu->m_iMaskTexCol = m_iMaskTexCols;
	if(m_iMaskTexRows > 0)
		pChu->m_iMaskTexRow = m_iMaskTexRows;

	pChu->m_fFrameTime = m_fFrameTime;
	pChu->m_bDistort = m_bDistort;

	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	pChu->m_pTextureRes = static_cast<Ogre::TextureData*>(pTex);

	pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer());
	pChu->m_pMaskTextureRes = static_cast<Ogre::TextureData*>(pTex);

	return pChu;
}


CString CGeometry::GetName()
{
	return m_strName;
}


bool CGeometry::Create()
{
	m_iUVAddressMode = Ogre::UVADDR_WRAP;
	m_iBlendMode = 1;
	m_GeomType = Ogre::GEO_SPHERE;

	m_bindInfo.boneid = 0;
	m_bindInfo.bDynamic = false;

	m_strName.Format(_T("¼¸ºÎÌå"));

	m_strTexture = DEF_FX_TEXTURE;
	m_bVisible = true;
	m_bDistort = false;

	GEO_FRAME firstFrame;

	firstFrame.lTick = 0;

	m_iTexRow = 1;
	m_iTexCol = 1;
	m_fFrameTime = 0;

	m_nUSeg = 4;
	m_nVSeg = 4;

	m_iMaskUVAddressMode = Ogre::UVADDR_CLAMP;
	m_iMaskTexCols = 1;
	m_iMaskTexRows = 1;
	m_strMaskTexture = "";

	m_KeyFrames.clear();
	m_KeyFrames.push_back(firstFrame);

	m_docInfo.NewDoc();
	return true;

}

bool CGeometry::Destroy()
{
	return true;
}

//
//void CGeometry::SetVisible(bool visible)
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


bool CGeometry::LoadEffect(CString path)
{
	Ogre::Resource* pChunk = Ogre::ResourceManager::getSingleton().blockLoad(path.GetBuffer());
	if(pChunk == NULL)
		return false;
	if(!IS_KIND_OF(Ogre::ParamShapeData, pChunk))
	{
		pChunk->release();
		return false;
	}
	LoadFromChunk(pChunk);
	m_strName = path;
	pChunk->release();
	return true;
}

bool CGeometry::SaveEffect(CString path)
{
	Ogre::ParamShapeData* pChu = new Ogre::ParamShapeData();
	SetKfToSource(pChu);
	pChu->m_iBlendMode = m_iBlendMode;
	pChu->m_GeoType = m_GeomType;

	pChu->m_iTexCol = 1;
	pChu->m_iTexRow = 1;

	if(m_iTexCol > 0)
		pChu->m_iTexCol = m_iTexCol;
	if(m_iTexRow > 0)
		pChu->m_iTexRow = m_iTexRow;
	pChu->m_fFrameTime = m_fFrameTime;
	pChu->m_bDistort = m_bDistort;


	Ogre::Resource* pTex = Ogre::ResourceManager::getSingleton().blockLoad(m_strTexture.GetBuffer());
	if(pTex == NULL)
	{
		OGRE_RELEASE( pChu);
		return false;
	}
	pChu->m_pTextureRes = static_cast<Ogre::TextureData *>(pTex);

	if(m_strMaskTexture != "")
	{
		pChu->m_pMaskTextureRes = static_cast<Ogre::TextureData *>(Ogre::ResourceManager::getSingleton().blockLoad(m_strMaskTexture.GetBuffer()));
	}

	pChu->save(path.GetBuffer());
	m_strName = path;
	OGRE_RELEASE( pChu);
	m_docInfo.Save();
	return true;
}

bool CGeometry::LoadFromChunk(Ogre::Resource* pChunk)
{
	Ogre::ParamShapeData* pChu = static_cast<Ogre::ParamShapeData*>( pChunk);

	m_iBlendMode = pChu->m_iBlendMode;
	m_GeomType = pChu->m_GeoType;

	if(pChu->m_iTexCol > 0)
		m_iTexCol = pChu->m_iTexCol;
	if(pChu->m_iTexRow > 0)
		m_iTexRow = pChu->m_iTexRow;
	m_fFrameTime = pChu->m_fFrameTime;
	m_bDistort = pChu->m_bDistort;

	SetKfFromSource(pChu);

	if(pChu->m_pTextureRes)	m_strTexture = pChu->m_pTextureRes->getResPath();
	else m_strTexture = "";

	if(pChu->m_pMaskTextureRes) m_strMaskTexture = pChu->m_pMaskTextureRes->getResPath();
	else m_strMaskTexture = "";

	m_docInfo.Open();
	return true;
}

BIND_INFO* CGeometry::GetBindInfo()
{
	return &m_bindInfo;
}

void CGeometry::SetBindInfo(BIND_INFO* bindinfo)
{
	m_bindInfo = *bindinfo;
}

EFFECT_TYPE CGeometry::GetType()
{
	return EFFECT_GEOM;
}

void CGeometry::SetKfToSource(Ogre::ParamShapeData* pSource)
{

	pSource->m_iUVAddressMode = m_iUVAddressMode;
	pSource->m_nUSeg = m_nUSeg;
	pSource->m_nVSeg = m_nVSeg;
	if(m_nUSeg < 2)
		m_nUSeg = 2;
	if(m_nVSeg < 2)
		m_nVSeg = 2;

	pSource->m_iMaskUVAddressMode = m_iMaskUVAddressMode;
	pSource->m_iMaskTexCol = m_iMaskTexCols;
	pSource->m_iMaskTexRow = m_iMaskTexRows;

	pSource->m_bGridMode = m_bShowGrid;
	Ogre::KeyFrameArray<float> initkf;

	Ogre::KeyFrameArray<Ogre::ColourValue> initkfc;

	pSource->m_SphereR = initkf;
	pSource->m_TorsuLR = initkf;
	pSource->m_TorsuNR = initkf;
	pSource->m_TubeH = initkf;
	pSource->m_TubeLR = initkf;
	pSource->m_TubeNR = initkf;
	pSource->m_UOff = initkf;
	pSource->m_VOff = initkf;
	pSource->m_Round = initkf;
	pSource->m_SphereAngle = initkf;
	pSource->m_ColumnDR = initkf;
	pSource->m_ColumnUR = initkf;
	pSource->m_ColumnH = initkf;
	pSource->m_UScl = initkf;
	pSource->m_VScl = initkf;
	pSource->m_UVRot = initkf;
	pSource->m_Clr = initkfc;

	pSource->m_MaskUOff = initkf;
	pSource->m_MaskVOff = initkf;
	pSource->m_MaskUScl = initkf;
	pSource->m_MaskVScl = initkf;
	pSource->m_MaskUVRot = initkf;


	for(int i = 0; i < (int)m_KeyFrames.size(); i++)
	{
		Ogre::KeyFrameArray<float>::KEYFRAME_T kf;
		Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfc;

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fShpereR;
		pSource->m_SphereR.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fTurosNR;
		pSource->m_TorsuLR.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fTurosLR;
		pSource->m_TorsuNR.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fTubeH;
		pSource->m_TubeH.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fTubeLR;
		pSource->m_TubeLR.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fTubeNR;
		pSource->m_TubeNR.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fUOffset;
		pSource->m_UOff.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fVOffset;
		pSource->m_VOff.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fRound;
		pSource->m_Round.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fShpereAngle;
		pSource->m_SphereAngle.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fColumnDR;
		pSource->m_ColumnDR .m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fColumnUR;
		pSource->m_ColumnUR.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fColumnH;
		pSource->m_ColumnH.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fUScale ;
		pSource->m_UScl.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fVScale;
		pSource->m_VScl.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fUVRotate;
		pSource->m_UVRot.m_Array.push_back(kf);

		kfc.tick = m_KeyFrames[i].lTick;
		kfc.data = m_KeyFrames[i].color;
		kfc.data.a = m_KeyFrames[i].alpha;
		pSource->m_Clr.m_Array.push_back(kfc);



		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fMaskUScale ;
		pSource->m_MaskUScl.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fMaskVScale;
		pSource->m_MaskVScl.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fMaskUVRotate;
		pSource->m_MaskUVRot.m_Array.push_back(kf);

		kf.data = m_KeyFrames[i].fMaskUOffset;
		pSource->m_MaskUOff.m_Array.push_back(kf);

		kf.tick = m_KeyFrames[i].lTick;
		kf.data = m_KeyFrames[i].fMaskVOffset;
		pSource->m_MaskVOff.m_Array.push_back(kf);

	}

}

void CGeometry::SetKfFromSource(Ogre::ParamShapeData* pSource)
{
	m_iUVAddressMode = pSource->m_iUVAddressMode;
	m_nUSeg = pSource->m_nUSeg;
	m_nVSeg = pSource->m_nVSeg;
	m_KeyFrames.clear();
	m_bShowGrid = pSource->m_bGridMode;

	m_iMaskUVAddressMode = pSource->m_iMaskUVAddressMode;
	m_iMaskTexCols = pSource->m_iMaskTexCol;
	m_iMaskTexRows = pSource->m_iMaskTexRow;

	for(int i = 0; i < (int)pSource->m_SphereR.m_Array.size(); i++)
	{
		GEO_FRAME frame;
		frame.lTick			= pSource->m_SphereR.m_Array[i].tick;

		frame.fShpereR			= pSource->m_SphereR.m_Array[i].data;
		frame.fTurosNR			= pSource->m_TorsuLR.m_Array[i].data;
		frame.fTurosLR			= pSource->m_TorsuNR.m_Array[i].data;
		frame.fTubeH			= pSource->m_TubeH.m_Array[i].data;
		frame.fTubeLR			= pSource->m_TubeLR.m_Array[i].data;
		frame.fTubeNR			= pSource->m_TubeNR.m_Array[i].data;
		frame.fUOffset			= pSource->m_UOff.m_Array[i].data;
		frame.fVOffset			= pSource->m_VOff.m_Array[i].data;

		frame.fRound			= pSource->m_Round.m_Array[i].data;
		frame.fShpereAngle		= pSource->m_SphereAngle.m_Array[i].data;
		frame.fColumnDR			= pSource->m_ColumnDR.m_Array[i].data;
		frame.fColumnUR			= pSource->m_ColumnUR.m_Array[i].data;
		frame.fColumnH			= pSource->m_ColumnH.m_Array[i].data;

		frame.fUScale			= pSource->m_UScl.m_Array[i].data;
		frame.fVScale			= pSource->m_VScl.m_Array[i].data;
		frame.fUVRotate			= pSource->m_UVRot.m_Array[i].data;
		frame.color				= pSource->m_Clr.m_Array[i].data;
		frame.alpha				= frame.color.a;

		frame.fMaskUScale			= pSource->m_MaskUScl.m_Array[i].data;
		frame.fMaskVScale			= pSource->m_MaskVScl.m_Array[i].data;
		frame.fMaskUVRotate			= pSource->m_MaskUVRot.m_Array[i].data;
		frame.fMaskUOffset			= pSource->m_MaskUOff.m_Array[i].data;
		frame.fMaskVOffset			= pSource->m_MaskVOff.m_Array[i].data;

		m_KeyFrames.push_back(frame);
	}
	//if(m_KeyFrames.size() == 0)
	//	MessageBox(NULL, "0 frame when SetKFFromSource", NULL,0);
}

void CGeometry::ExecuteCommand(CEffectCommand* pCmd)
{
	CEffectMgr::GetInstance()->GetCommandMgr()->SetEnable(false);

	CGeometryCommand* pGeoCmd = static_cast<CGeometryCommand*>( pCmd);
	bool bRefresh = false;

	if(pGeoCmd->m_newBlendMode != m_iBlendMode ||
		pGeoCmd->m_newGeomType != m_GeomType ||
		pGeoCmd->m_newBind.boneid != m_bindInfo.boneid ||
		pGeoCmd->m_newTex != m_strTexture)
	{
		bRefresh = true;	
	}

	m_iBlendMode = pGeoCmd->m_newBlendMode;
	m_GeomType = pGeoCmd->m_newGeomType;
	m_strTexture = pGeoCmd->m_newTex;
	m_KeyFrames = pGeoCmd->m_newKf;
	m_bindInfo = pGeoCmd->m_newBind;

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
CEffectCommand* CGeometry::BeginRecordCommand()
{
	CGeometryCommand* pUndoCmd = new CGeometryCommand();

	pUndoCmd->m_newBind = m_bindInfo;
	pUndoCmd->m_newBlendMode = m_iBlendMode;
	pUndoCmd->m_newGeomType = m_GeomType;
	pUndoCmd->m_newKf = m_KeyFrames;
	pUndoCmd->m_newTex = m_strTexture;
	pUndoCmd->m_pTarget = this;

	return pUndoCmd; 
}
void CGeometry::EndRecordCommand(CEffectCommand* pcmd)
{
	CGeometryCommand* pUndoCmd = static_cast<CGeometryCommand*>(pcmd);
	pUndoCmd->m_oldBind = m_bindInfo;
	pUndoCmd->m_oldBlendMode = m_iBlendMode;
	pUndoCmd->m_oldGeomType = m_GeomType;
	pUndoCmd->m_oldKf = m_KeyFrames;
	pUndoCmd->m_oldTex = m_strTexture;

	CEffectMgr::GetInstance()->GetCommandMgr()->OnCommandDone(pUndoCmd);
}


// -------------------CGeomtryCommand----------------------------- [7/4/2008]
CEffectUnit* CGeometryCommand::GetTarget()
{
	return m_pTarget;
}

bool CGeometryCommand::NeedRefresh()
{
	return false;
}

CEffectCommand* CGeometryCommand::GetInversCommand()
{
	CGeometryCommand* pCmd = new CGeometryCommand();

	pCmd->m_pTarget = m_pTarget;

	pCmd->m_newBind = m_oldBind;
	pCmd->m_oldBind = m_newBind;

	pCmd->m_newBlendMode = m_oldBlendMode;
	pCmd->m_oldBlendMode = m_newBlendMode;

	pCmd->m_newGeomType = m_oldGeomType;
	pCmd->m_oldGeomType = m_newGeomType;

	pCmd->m_newKf = m_oldKf;
	pCmd->m_oldKf = m_newKf;

	pCmd->m_newTex = m_oldTex;
	pCmd->m_oldTex = m_newTex;

	return static_cast<CEffectCommand*>(pCmd);
}

bool CGeometryCommand::IsIdentical()
{
	if(m_oldBlendMode != m_newBlendMode)
		return false;

	if(m_oldGeomType != m_newGeomType)
		return false;

	if(m_oldTex != m_newTex)
		return false;

	if(m_oldKf.size() != m_newKf.size())
		return false;

	for(int i = 0; i < (int) m_oldKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &m_newKf[i], sizeof(CGeometry::GEO_FRAME)) != 0)
			return false;
	}

	return true;
}

bool CGeometryCommand::Merge(CEffectCommand* pCmd, bool bForceMerge)
{
	if(pCmd->GetTarget() != m_pTarget)
		return false;

	CGeometryCommand* pGeoCmd = (CGeometryCommand*)pCmd;

	if(m_oldKf.size() != pGeoCmd->m_oldKf.size())
		return false;
	for(int i = 0; i < (int) m_newKf.size(); i++)
	{
		if(memcmp(&m_oldKf[i], &(pGeoCmd->m_oldKf[i]), sizeof(CGeometry::GEO_FRAME)) != 0)
			return false;
	}
	if(m_oldTex != pGeoCmd->m_oldTex)
		return false;


	if(m_oldBlendMode != pGeoCmd->m_oldBlendMode) return false;
	if(m_oldGeomType != pGeoCmd->m_oldGeomType) return false;
	if(m_oldTex != pGeoCmd->m_oldTex) return false;

	if(m_oldBind.boneid != pGeoCmd->m_oldBind.boneid)
		return false;

	m_oldBind.offset = pGeoCmd->m_oldBind.offset;
	m_oldBind.rotate = pGeoCmd->m_oldBind.rotate;
	m_oldBind.scale = pGeoCmd->m_oldBind.scale;

	return true;
}