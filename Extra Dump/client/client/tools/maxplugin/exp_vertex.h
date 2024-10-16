
#ifndef __EXP_VERTEX_H__
#define __EXP_VERTEX_H__

#include "exp_types.h"

namespace DKEXP
{
	class InflBoneArray
	{
	public:
		InflBoneArray(int max_patchbone);
		~InflBoneArray();

		void Init(MeshConverter &mesh, bool bVertAnim);
		void AddFace(int iface, MeshConverter &mesh, bool bVertAnim);
		unsigned short AddVertex(const VertexData &vert, MeshConverter &mesh, bool bVertAnim);

	public:
		std::vector<Ogre::SkinPatch *>	m_Patches;
		int								m_MaxVertInfl;
		int								m_MaxPatchBone;
		std::vector<VertexData>			m_MaxVertex;
		std::vector<unsigned short>		m_Indices;

		Ogre::SkinPatch *m_pCurPatch;
	};
}

#endif
