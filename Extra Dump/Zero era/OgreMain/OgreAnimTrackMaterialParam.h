
#ifndef __OGREANIMTRACKMATERIALPARAM_H__
#define __OGREANIMTRACKMATERIALPARAM_H__

#include "OgreResource.h"
#include "OgreKeyFrameArray.h"
#include "OgreRenderTypes.h"
#include <list>

namespace Ogre
{
	class _OgreExport MaterialParamTrack : public Resource
	{
		DECLARE_RTTI(MaterialParamTrack)

	public:
		MaterialParamTrack();
		~MaterialParamTrack();

		virtual void _serialize(Archive &ar, int version);

	public:
		FixedString m_MeshName;
		FixedString m_MtlName;
		FixedString m_ParamName;
		int m_KeyType; //ShadeParamType
		BaseKeyFrameArray *m_pKeyframes;
	};

}

#endif