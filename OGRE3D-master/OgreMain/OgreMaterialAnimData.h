
#ifndef __MaterialAnimData_H__
#define __MaterialAnimData_H__

#include "OgreAnimationData.h"
#include <list>

namespace Ogre
{
	class _OgreExport MaterialAnimData : public AnimationData
	{
		DECLARE_RTTI(MaterialAnimData)

	public:
		virtual AnimationType getType()
		{
			return ANIM_MATERIAL;
		}

		virtual void _serialize(Archive &ar, int version);

	protected:
		std::vector<MaterialParamTrack *>m_MeshTracks;
	};
}

#endif