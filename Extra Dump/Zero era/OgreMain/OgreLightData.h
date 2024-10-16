
#ifndef __LightData_H__
#define __LightData_H__

#include "OgreBaseObject.h"
#include "OgreColourValue.h"
#include "OgreRenderTypes.h"
#include "OgreResource.h"

namespace Ogre
{
	class _OgreExport LightData : public Resource
	{
		DECLARE_RTTI(LightData)
	public:
		LightType m_Type;
		ColourValue m_Color;
		float m_Attenuation;
		float m_Range;

	protected:
		virtual void _serialize(Archive &ar, int version);
	};
}

#endif