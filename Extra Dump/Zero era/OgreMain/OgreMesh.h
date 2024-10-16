
#ifndef __Mesh_H__
#define __Mesh_H__

#include "OgrePrerequisites.h"
#include "OgreResource.h"
#include "OgreMaterial.h"
#include "OgreVertexIndexData.h"
#include "OgreBounding.h"

namespace Ogre
{
	class _OgreExport SkinPatch : public Resource
	{
		DECLARE_RTTI(SkinPatch)

	public:
		virtual void _serialize(Archive &ar, int version);

	public:
		uint m_StartVert;
		uint m_NumVert;
		uint m_StartIndex;
		uint m_NumIndex;
		std::vector<ushort>m_RefBones;
		std::vector<Matrix4>m_BoneBindTM;
	};

	class _OgreExport SubMeshData : public Resource
	{
		DECLARE_RTTI(SubMeshData);
	public:
		SubMeshData();
		Material *getMaterial()
		{
			return m_pMtl;
		}
	protected:
		virtual ~SubMeshData();
		virtual void _serialize(Archive &ar, int version);

	public:
		PrimitiveType m_PrimType;
		uint m_NumPrimitive;
		uint m_MaxVertInfl;

		IndexData *m_pIndexData;
		VertexData *m_pVertexData;

		Material *m_pMtl;

		std::vector<SkinPatch *>m_Patches;
	};

	class _OgreExport MeshData: public Resource
	{
		DECLARE_RTTI(MeshData)
	public:
		MeshData();

		void addSubMesh(SubMeshData *psubmesh)
		{
			m_SubMeshes.push_back(psubmesh);
		}

		const SubMeshData *getSubMesh(size_t i)const
		{
			assert(i < m_SubMeshes.size());
			return m_SubMeshes[i];
		}

	protected:
		virtual ~MeshData();
		virtual void _serialize(Archive &ar, int version);

	public:
		FixedString m_Name;
		std::vector<SubMeshData *>m_SubMeshes;
		BoxSphereBound m_Bounding;
		int m_BindBoneID;
		bool m_bSkin;
	};
};

#endif