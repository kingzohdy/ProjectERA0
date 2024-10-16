
#include "stdafx.h"
#include "ActorIO.h"

#include "OgreKeyFrameArray.h"
#include "OgreSkeletonData.h"
#include "OgreModelData.h"
#include "OgreResourceManager.h"

CActorIO::CActorIO(void) : m_pOutputChunk(NULL), m_pBounding(NULL), m_pBsp(NULL)
{
}

CActorIO::~CActorIO(void)
{
}

void CActorIO::SetSkeleton(Ogre::SkeletonData* pCoreSkeleton)
{
	m_pCoreSkeleton = pCoreSkeleton;
}

void CActorIO::AddBindObj(Ogre::Resource* pObj, int type, int iBoneID, int iSrcID, unsigned int flags, bool bNeedClear)
{
	IO_BIND_INFO bi;
	bi.boneid = iBoneID;
	bi.srcid = iSrcID;
	bi.flags = flags;
	bi.bindtype = type;
	bi.bNeedClear = bNeedClear;
	bi.pchunk = pObj;


	m_BindInfo.push_back(bi);

}
Ogre::Resource* CreateChunk(Ogre::Resource* pRes)
{
	//std::string clsname = pObj->GetRTTI()->GetClassName();

	//if(clsname == "MeshSource")
	//{
	//	return new Ogre::MeshData();
	//}
	//else if(clsname == "ParticleEmitterData")
	//{
	//	bi.bindtype = Ogre::ACTOR_BIND_MESH;
	//	bi.pmesh = (Ogre::MeshSource*) pObj;
	//}
	//else if(clsname == "CameraSource")
	//{
	//	bi.bindtype= Ogre::ACTOR_BIND_MESH;
	//	bi.pmesh = (Ogre::MeshSource*) pObj;
	//}
	//else if(clsname == "LightSource")
	//{
	//	bi.bindtype = Ogre::ACTOR_BIND_MESH;
	//	bi.pmesh = (Ogre::MeshSource*) pObj;
	//}
	return NULL;

}

void CActorIO::AddAnchor(Ogre::Vector3 point, int iBoneID, int id)
{
	ANCHOR_INFO ai;
	//ai.point = point;
	ai.iBoneID = iBoneID;
	ai.iID = id;
	m_AnchorInfo.push_back(ai);
}
template<typename T>
void AddKeyFrame(Ogre::KeyFrameArray<T>& kfarr, long lTick, T data)
{
	Ogre::KeyFrameArray<T>::KEYFRAME_T kfTrans;
	kfTrans.tick = lTick;
	kfTrans.data = data;
	kfarr.m_Array.push_back(kfTrans);

}

template<typename T>
void AddSequence(Ogre::KeyFrameArray<T>& kfarr, int start, int end)
{
	Ogre::BaseKeyFrameArray::AnimRange range;
	range.begin = start;
	range.end = end;
	kfarr.m_Ranges.push_back(range);
}

Ogre::ModelData* CActorIO::LoadDefaultActor()
{
	Ogre::ModelData* pSource = new Ogre::ModelData;

	Ogre::SkeletonData *pskeleton = new Ogre::SkeletonData;
	Ogre::BoneData* pBone = new Ogre::BoneData;

	pBone->m_ID = 0;
	pBone->m_Name = "root";
	pBone->m_FatherID = -1;
	pBone->m_OriginTM.identity();

	pskeleton->addBone(pBone);
	pSource->m_pSkeleton = pskeleton;

	pSource->m_Bounding.setRange(Ogre::Vector3(-100.0f, -100.0f, -100.0f), Ogre::Vector3(100.0f, 100.0f, 100.0f));

	return pSource;
}

Ogre::ModelData* CActorIO::LoadActor(CString strObjName)
{
	Ogre::ModelData* pChunk = static_cast<Ogre::ModelData *>(Ogre::ResourceManager::getSingleton().blockLoad(strObjName.GetBuffer()));

	if(!pChunk) return NULL;

	m_BindInfo.clear();
	for(size_t i=0; i<pChunk->m_Meshes.size(); i++)
	{
		IO_BIND_INFO bi;
		Ogre::MeshData *pmesh = pChunk->m_Meshes[i];
		if(!pmesh->m_bSkin)
		{
			bi.bindtype = 0;
			bi.boneid = pmesh->m_BindBoneID;
			bi.flags = 0;
			bi.srcid = 0;
			bi.pchunk = pmesh;
			m_BindInfo.push_back(bi);
		}
	}

	m_AnchorInfo.clear();
	for(size_t i=0; i<pChunk->m_Anchors.size(); i++)
	{
		ANCHOR_INFO ai;
		Ogre::ModelAnchor &anchor = pChunk->m_Anchors[i];
		ai.iBoneID = anchor.boneid;
		//ai.point = anchor.offset;
		ai.iID = anchor.anchorid;

		m_AnchorInfo.push_back(ai);
	}

	return pChunk;
}

Ogre::SkeletonData* CActorIO::CreateSkeletonChunk()
{
	Ogre::SkeletonData* pRet = new Ogre::SkeletonData();
	//add bone
	for(size_t i=0; i<m_pCoreSkeleton->getNumBone(); i++)
	{
		Ogre::BoneData* pBoneChunk = new Ogre::BoneData();
		Ogre::BoneData* pSourceBone = m_pCoreSkeleton->getIthBone(i);
		pBoneChunk->m_ID			= pSourceBone->m_ID;
		pBoneChunk->m_FatherID		= pSourceBone->m_FatherID;
		pBoneChunk->m_OriginTM		= pSourceBone->m_OriginTM;
		pBoneChunk->m_Name			= pSourceBone->m_Name;
		pBoneChunk->m_UserData = pSourceBone->m_UserData;

		pRet->addBone(pBoneChunk);
	}

	//add anim
	return pRet;
}

Ogre::SkeletonAnimData* CActorIO::CreateSkeletonAnimChunk()
{/*
 Ogre::SkeletonAnimData* pRet = new Ogre::SkeletonAnimData();
 std::list<Ogre::SkeletonData::stAnimRes>::iterator it = m_pCoreSkeleton->m_BoneAnims.begin();
 Ogre::SkeletonAnim* pAnim = it->panim;
 pRet->m_pAnim->m_Sequences.resize(pAnim->m_Sequences.size());
 for(int i = 0; i < (int)it->panim->m_Sequences.size(); i++)
 {
 Ogre::SequenceDesc& desseq = pRet->m_pAnim->m_Sequences[i];
 Ogre::SequenceDesc& srcseq = it->panim->m_Sequences[i];
 desseq.id			= srcseq.id;
 desseq.loopmode		= srcseq.loopmode;
 desseq.time_start	= srcseq.time_start;
 desseq.time_end		= srcseq.time_end;
 }

 for(int i = 0; i < (int)pAnim->m_TrackArray.size() ; i++)
 {
 Ogre::BoneTrackChunk *pTrackChunk = new Ogre::BoneTrackChunk;
 Ogre::BoneTrack *pTrack = pTrackChunk->m_pTrack;
 Ogre::BoneTrack *pSrcTrack = pAnim->m_TrackArray[i];
 pTrack->m_BoneID			= pSrcTrack->m_BoneID;
 pTrack->m_TranslateKeys		= pSrcTrack->m_TranslateKeys;
 pTrack->m_RotateKeys		= pSrcTrack->m_RotateKeys;
 pTrack->m_ScaleKeys			= pSrcTrack->m_ScaleKeys;

 for(int i=0; i<(int)pTrack->m_RotateKeys.m_Array.size(); i++)
 {
 Ogre::Quaternion &quat = pTrack->m_RotateKeys.m_Array[i].data;
 quat.w = -quat.w;
 }
 pRet->AddAttrib(pTrackChunk);

 }

 return pRet;
 */
	return NULL;
}


void CActorIO::SaveToActor(CString& strObjName)
{/*
 Ogre::ModelData* pActorChunk = new Ogre::ModelData();

 ////Build skeleton chunk and bone
 Ogre::SkeletonData* pSk = CreateSkeletonChunk();
 pActorChunk->AddAttrib(pSk);

 //Build Animation
 Ogre::SkeletonAnimData *pAnimChunk = CreateSkeletonAnimChunk();;
 pActorChunk->AddAttrib(pAnimChunk);

 //Add Anchor Point
 for(int i = 0; i < (int)m_AnchorInfo.size(); i++)
 {
 Ogre::Model::ANCHOR_T anchor;
 anchor.anchorid = m_AnchorInfo[i].iID;
 anchor.boneid = m_AnchorInfo[i].iBoneID;
 anchor.offset = m_AnchorInfo[i].point;
 pActorChunk->m_pSkinActor->m_Anchors.push_back(anchor);
 }

 //Add Bind Objects
 int srcid = 0;
 Ogre::ComplexChunk *pSkinChunks = new Ogre::ComplexChunk(Ogre::CHUNK_COMPLEX, "Skins");
 Ogre::Model::BINDOBJ_T bindObj;
 std::vector<Ogre::Resource*> toReserve;
 for(int i = 0; i < (int)m_BindInfo.size(); i++)
 {
 Ogre::Resource* pBindChunk = NULL;
 switch(m_BindInfo[i].bindtype)
 {
 case Ogre::ACTOR_BIND_LIGHT:
 pBindChunk = m_BindInfo[i].plight;
 break;
 case Ogre::ACTOR_BIND_SKIN:
 pBindChunk = m_BindInfo[i].pmesh;
 CompleteSkinChunk(m_BindInfo[i].pmesh);
 case Ogre::ACTOR_BIND_MESH:
 pBindChunk = m_BindInfo[i].pmesh;
 CompleteMeshChunk(m_BindInfo[i].pmesh);
 break;
 case Ogre::ACTOR_BIND_CAMERA:
 pBindChunk =m_BindInfo[i].pcamera;
 break;
 case Ogre::ACTOR_BIND_PEMITTER:
 pBindChunk = m_BindInfo[i].pemitter;
 break;
 case Ogre::ACTOR_BIND_BILLBOARD:
 pBindChunk = m_BindInfo[i].pbillboard;
 break;
 case Ogre::ACTOR_BIND_RIBBON:
 pBindChunk = m_BindInfo[i].pribbon;
 break;
 case Ogre::ACTOR_BIND_BEAMEMITTER:
 pBindChunk = m_BindInfo[i].pbeam;
 break;
 default:
 break;
 }

 if(pBindChunk != NULL)
 {
 pSkinChunks->AddAttrib(pBindChunk);
 if(!m_BindInfo[i].bNeedClear)
 toReserve.push_back(pBindChunk);
 ZeroMemory(&bindObj, sizeof(bindObj));
 bindObj.bindtype = m_BindInfo[i].bindtype;
 bindObj.boneid = m_BindInfo[i].boneid;
 bindObj.srcid = srcid++;
 bindObj.flags = m_BindInfo[i].flags;
 //bindObj.hres = Ogre::INVALID_HANDLE;


 if(m_BindInfo[i].bindtype == Ogre::ACTOR_BIND_SKIN)
 bindObj.boneid = -1;

 pActorChunk->m_pSkinActor->m_Binds.push_back(bindObj);
 }

 }

 if(m_pBounding != NULL)
 {
 Ogre::BoundingChunk* pBoundingChunk = new Ogre::BoundingChunk();
 pBoundingChunk->m_pBV = m_pBounding;
 pActorChunk->AddAttrib(pBoundingChunk);
 }

 if(m_pBsp != NULL)
 {
 Ogre::BSPChunk* pBspChunk = new Ogre::BSPChunk();
 pBspChunk->m_pBSP = m_pBsp;
 pActorChunk->AddAttrib(pBspChunk);
 }

 pActorChunk->AddAttrib(pSkinChunks);
 Ogre::SaveChunkToFile("chunkdata", strObjName.GetBuffer(), pActorChunk);

 for(int i = 0; i < (int)toReserve.size(); i++)
 pSkinChunks->m_AttribList.RemoveNode(toReserve[i]);

 delete pActorChunk;
 m_pBounding = NULL;
 m_pBsp = NULL;
 */
}

void CActorIO::CompleteSkinChunk(Ogre::MeshData* pMeshChunk)
{/*
 Ogre::ComplexChunk* pSkinInfoChunk = (Ogre::ComplexChunk* )pMeshChunk->GetAttribByName("SkinInfo");
 Ogre::CHUNKTYPE_T type;
 if(pSkinInfoChunk)
 {
 type = pSkinInfoChunk->GetType();
 pMeshChunk->m_AttribList.RemoveNode(pSkinInfoChunk);
 SAFE_DELETE(pSkinInfoChunk);
 }


 pSkinInfoChunk = new Ogre::ComplexChunk(Ogre::CHUNK_COMPLEX, "SkinInfo");

 Ogre::IntegerChunk* pMaxVertChunk = new Ogre::IntegerChunk();
 pMaxVertChunk->m_Name = "MaxVertInfl";
 pMaxVertChunk->m_Data = pMeshChunk->m_pMesh->m_MaxVertInfl;
 pSkinInfoChunk->AddAttrib(pMaxVertChunk);

 for(int i = 0; i < (int)pMeshChunk->m_pMesh->m_SkinPatches.size(); i++)
 {
 Ogre::SkinPatch* pSkinPath = new Ogre::SkinPatch();
 pSkinPath->m_BoneBindTM		= pMeshChunk->m_pMesh->m_SkinPatches[i]->m_BoneBindTM;
 pSkinPath->m_NumIndex		= pMeshChunk->m_pMesh->m_SkinPatches[i]->m_NumIndex;
 pSkinPath->m_NumVert		= pMeshChunk->m_pMesh->m_SkinPatches[i]->m_NumVert;
 pSkinPath->m_RefBones		= pMeshChunk->m_pMesh->m_SkinPatches[i]->m_RefBones;
 pSkinPath->m_StartIndex		= pMeshChunk->m_pMesh->m_SkinPatches[i]->m_StartIndex;
 pSkinPath->m_StartVert		= pMeshChunk->m_pMesh->m_SkinPatches[i]->m_StartVert;

 Ogre::SkinPatchChunk* pSkinPathChunk = new Ogre::SkinPatchChunk(pSkinPath);
 pSkinInfoChunk->AddAttrib(pSkinPathChunk);
 }

 pMeshChunk->AddAttrib(pSkinInfoChunk);

 CompleteMeshChunk(pMeshChunk);
 */
}

void CActorIO::CompleteMeshChunk(Ogre::MeshData* pMeshChunk)
{/*
 Ogre::BoundingChunk* pBoundingChunk = (Ogre::BoundingChunk*)pMeshChunk->GetAttribByType(Ogre::CHUNK_BOUNDING);
 CBounding bounding;
 if(pBoundingChunk == NULL)
 {
 if(pMeshChunk->m_pMesh->m_pBoundingBox == NULL)
 return;
 else
 {
 pBoundingChunk = new Ogre::BoundingChunk();
 pMeshChunk->AddAttrib(pBoundingChunk);
 }
 }
 bounding.SetBounding(pMeshChunk->m_pMesh->m_pBoundingBox);

 pBoundingChunk->m_pBV = bounding.CreateBoundVolume();
 */
}