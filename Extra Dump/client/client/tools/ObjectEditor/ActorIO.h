#pragma once
#include "stdafx.h"
#include "OgreVector3.h"

class CActorIO
{
public:
	CActorIO(void);
	~CActorIO(void);

	void SetSkeleton(Ogre::SkeletonData* pCoreSkeleton);
	void AddBindObj(Ogre::Resource* pObj, int type, int iBoneID, int iSrcID, unsigned int flags, bool bNeedClear);
	void AddAnchor(Ogre::Vector3 point, int iBoneID, int id);

	Ogre::ModelData* LoadDefaultActor();
	Ogre::ModelData* LoadActor(CString strObjName);
	void SaveToActor(CString& strObjName);
	Ogre::SkeletonData* CreateSkeletonChunk();
	Ogre::SkeletonAnimData* CreateSkeletonAnimChunk();
	void CompleteSkinChunk(Ogre::MeshData* pMeshChunk);
	void CompleteMeshChunk(Ogre::MeshData* pMeshChunk);
	struct IO_BIND_INFO
	{
		int bindtype;
		int boneid;
		int srcid;
		unsigned int flags;
		union
		{
			Ogre::Resource* pchunk;
			Ogre::MeshData *pmesh;
			Ogre::ParticleEmitterData *pemitter;
			Ogre::LightData *plight;
			Ogre::BillboardData* pbillboard;
			Ogre::RibbonEmitterData* pribbon;
			Ogre::BeamEmitterData* pbeam;
		};
		bool bNeedClear;
	};
	struct ANCHOR_INFO
	{
		//Ogre::Vector3 point;
		int iBoneID;
		int iID;
	};

	Ogre::BoxBound* m_pBounding;
	Ogre::BSPData* m_pBsp;
	Ogre::SkeletonData * m_pCoreSkeleton;
	std::vector<ANCHOR_INFO> m_AnchorInfo;

	std::vector<IO_BIND_INFO> m_BindInfo;
	Ogre::ModelData* m_pOutputChunk;



};
