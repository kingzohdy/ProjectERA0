
#ifndef __MorphAnimData_H__
#define __MorphAnimData_H__

#include "OgreAnimationData.h"
#include "OgreVertexIndexData.h"
#include "OgreVector3.h"

namespace Ogre
{
	class _OgreExport MorphAnimData : public AnimationData
	{
		DECLARE_RTTI(MorphAnimData)

	public:
		enum
		{
			CHANNEL_POS=1,  //pos and normal.  Vector3[2]
			CHANNEL_UV0=2,  //Vector2[1]
		};

		struct AnimRange
		{
			int begin;
			int end;  //[begin, end]
		};

	public:
		MorphAnimData();
		~MorphAnimData();

		virtual AnimationType getType()
		{
			return ANIM_MORPH;
		}
		virtual void _serialize(Archive &ar, int version);

		const FixedString &getTargetName()
		{
			return m_MeshName;
		}

	public:
		FixedString m_MeshName;
		uint m_Channel;
		uint m_nFrame;
		uint m_nVertex;
		VertexData *m_pVertData;

		Vector3 m_MinValue[4];
		Vector3 m_MaxValue[4];
		std::vector<uint>m_FrameTicks;
		std::vector<AnimRange>m_Ranges;
	};
}

#endif