
#ifndef __ModelData_H__
#define __ModelData_H__

#include "OgreResource.h"
#include "OgreAnimationData.h"
#include "OgreMesh.h"
#include "OgreQuaternion.h"

namespace Ogre
{
	class _OgreExport BSPData : public Resource
	{
		DECLARE_RTTI(BSPData)
	public:
		enum MTL_TYPE
		{
			MTL_NORMAL = 0,
			MTL_WALK,
			MTL_COLLIDE,
		};
		struct SufaceMtl
		{
			MTL_TYPE type;
		};
	public:
		BSPData()
		{
		}

		BSPData(const BSPData &rhs);

		virtual void _serialize(Archive &ar, int version);

		std::vector<Vector3> m_Vertex;

		//Index值为每个面4个，前三个是面的三个顶点索引，第四个值为该面的材质索引值，目前未使用
		std::vector<unsigned short> m_Index;
		std::vector<SufaceMtl> m_SufaceMtl;
	};

	struct ModelAnchor
	{
		int boneid;
		int anchorid;
	};

	struct SequenceDesc;
	class _OgreExport ModelData : public Resource
	{
		DECLARE_RTTI(ModelData)
	public:
		ModelData();
		~ModelData();

		SkeletonData *getSkeletonData()
		{
			return m_pSkeleton;
		}
		
		size_t getNumMesh()
		{
			return m_Meshes.size();
		}

		const MeshData *getMeshByName(const FixedString &name)const;
		MeshData *getIthMesh(size_t i) 
		{
			assert(i < m_Meshes.size());
			return m_Meshes[i];
		}

		SubMeshData *getSubMesh(size_t imesh, size_t isubmesh)
		{
			assert(isubmesh < getIthMesh(imesh)->m_SubMeshes.size());
			return getIthMesh(imesh)->m_SubMeshes[isubmesh];
		}

		void addMeshData(MeshData *pmesh)
		{
			m_Meshes.push_back(pmesh);
		}

		void addAnimation(AnimationData *panim);

		size_t getNumAnim()
		{
			return m_Anims.size();
		}

		AnimationData *getAnimation(size_t i)
		{
			return m_Anims[i];
		}
		
		void removeAnimation(size_t i)
		{
			assert(i < m_Anims.size());
			m_Anims.erase(m_Anims.begin() + i);
		}

		void getAllSequence(std::vector<SequenceDesc>&seq_array);

	private:
		virtual void _serialize(Archive &ar, int version);

	public:
		std::vector<MeshData *>m_Meshes;
		SkeletonData *m_pSkeleton;
		std::vector<AnimationData *>m_Anims;
		BSPData *m_pBspData;
		BoxBound m_Bounding;
		std::vector<ModelAnchor>m_Anchors;
	};
}

#endif
