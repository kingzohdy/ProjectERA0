
#include "stdafx.h"
#include "exp_vertex.h"
#include "exp_mesh.h"

using namespace Ogre;

namespace DKEXP
{
	static unsigned char InsertBoneID(std::vector<unsigned short>&destRef, std::vector<Matrix4>&destTM, int boneid, MeshConverter &mesh)
	{
		size_t i;
		for(i=0; i<destRef.size(); i++)
		{
			if(destRef[i] == (unsigned short)boneid)
			{
				assert(i <= 255);
				return static_cast<unsigned char>(i);
			}
		}
		destRef.push_back((unsigned short)boneid);
		assert(destRef.size() <= 255);

		for(i=0; i<mesh.m_BoneParams.size(); i++)
		{
			if (boneid == mesh.m_BoneParams[i].boneid)
			{
				//insert boneBindTM
				destTM.push_back(mesh.m_BoneParams[i].offsettm);
				break;
			}
		}

		assert(i < mesh.m_BoneParams.size());
		assert(destTM.size() == destRef.size());
		return static_cast<unsigned char>(destRef.size()-1);
	}

	InflBoneArray::InflBoneArray(int max_patchbone): m_MaxVertInfl(0), m_MaxPatchBone(max_patchbone)
	{
		for(UINT i=0; i<m_Patches.size(); i++)
		{
			delete (m_Patches[i]);
		}

		m_MaxVertex.resize(0);
		m_Indices.resize(0);

		m_pCurPatch = NULL;
	}

	//---------------------------------------------------------------------------
	InflBoneArray::~InflBoneArray()
	{
		for(size_t i=0; i<m_Patches.size(); i++)
		{
			delete (m_Patches[i]);
		}
	}

	//---------------------------------------------------------------------------
	void InflBoneArray::Init(MeshConverter &mesh, bool bVertAnim)
	{
		int nface = mesh.m_MaxVertex.size() / 3;

		m_pCurPatch = new SkinPatch;
		m_pCurPatch->m_StartVert = static_cast<unsigned int>(m_MaxVertex.size());
		m_pCurPatch->m_StartIndex = static_cast<unsigned int>(m_Indices.size());
		m_pCurPatch->m_NumVert = 0;
		m_pCurPatch->m_NumIndex = 0;

		for(int iface=0; iface<nface; iface++)
		{
			AddFace(iface, mesh, bVertAnim);
		}

		m_Patches.push_back(m_pCurPatch);
	}

	//---------------------------------------------------------------------------
	void InflBoneArray::AddFace(int iface, MeshConverter &mesh, bool bVertAnim)
	{
		size_t old_num = m_pCurPatch->m_RefBones.size();

		int i;
		for(i=0; i<3; i++)
		{
			const VertexData &vert = mesh.m_MaxVertex[3*iface + i];
			for(int j=0; j<4; j++)
			{
				if(vert.m_pfWeight[j] > 0)
				{
					InsertBoneID(m_pCurPatch->m_RefBones, m_pCurPatch->m_BoneBindTM, vert.m_puiBoneIndex[j], mesh);
				}
			}
		}

		size_t new_num = m_pCurPatch->m_RefBones.size();
		m_pCurPatch->m_RefBones.resize(old_num);
		m_pCurPatch->m_BoneBindTM.resize(old_num);

		if(new_num > size_t(m_MaxPatchBone))
		{
			char info[256];
			sprintf(info, "实际影响皮肤的骨骼数量超过%d根， 切分patch", m_MaxPatchBone);
			MessageBox(NULL, info, "Error", MB_OK);

			m_Patches.push_back(m_pCurPatch);

			m_pCurPatch = new SkinPatch;
			m_pCurPatch->m_StartVert = static_cast<unsigned int>(m_MaxVertex.size());
			m_pCurPatch->m_StartIndex = static_cast<unsigned int>(m_Indices.size());
			m_pCurPatch->m_NumVert = 0;
			m_pCurPatch->m_NumIndex = 0;
		}

		for(i=0; i<3; i++)
		{
			const VertexData &vert = mesh.m_MaxVertex[3*iface + i];
			m_Indices.push_back(AddVertex(vert, mesh, bVertAnim));
			m_pCurPatch->m_NumIndex++;
		}
	}

	//---------------------------------------------------------------------------
	unsigned short InflBoneArray::AddVertex(const VertexData &vert, MeshConverter &mesh, bool bVertAnim)
	{
		VertexData srcvert;
		srcvert.pos = vert.pos;
		srcvert.normal = vert.normal;
		srcvert.vertColor = vert.vertColor;
		srcvert.tangent = vert.tangent;
		srcvert.binormal = vert.binormal;

		for (int i=0; i<MAX_UV_NUM; i++)
		{
			srcvert.uv[i] = vert.uv[i];
		}

		for(int j=0; j<4; j++)
		{
			if(vert.m_pfWeight[j] > 0)
			{
				srcvert.m_puiBoneIndex[j] = InsertBoneID(m_pCurPatch->m_RefBones, m_pCurPatch->m_BoneBindTM, vert.m_puiBoneIndex[j], mesh);
				if(m_MaxVertInfl < j+1) m_MaxVertInfl = j+1;
			}
			else srcvert.m_puiBoneIndex[j] = 0;

			srcvert.m_pfWeight[j] = vert.m_pfWeight[j];
		}

		for(int k=0; k<4; k++)
		{
			assert(srcvert.m_puiBoneIndex[k] < MAX_BONE_INFL);
		}

		if (!bVertAnim)
		{
			for(size_t i=m_pCurPatch->m_StartVert; i<m_MaxVertex.size(); i++)
			{
				if(memcmp(&m_MaxVertex[i], &srcvert, sizeof(VertexData)) == 0)
				{
					return unsigned short(i);
				}
			}
		}

		m_MaxVertex.push_back(srcvert);
		m_pCurPatch->m_NumVert++;
		assert(m_pCurPatch->m_NumVert+m_pCurPatch->m_StartVert == m_MaxVertex.size());
		return (unsigned short)(m_MaxVertex.size()-1);
	}
}
