
#ifndef __EXP_LIGHT_H__
#define __EXP_LIGHT_H__

#include "exp_types.h"
#include "OgreColourValue.h"
#include "OgreLightData.h"

namespace DKEXP
{
	class LightConverter
	{
	public:
		int Convert(Interface* pIntf, INode* pMaxNode, Object* pObj, const std::vector<INode* > &pBoneNodes);

		LightConverter(TimeValue animStart,TimeValue animEnd);
		~LightConverter();

	public:
		Ogre::LightType m_eType;
		Ogre::ColourValue m_cColor;
		float m_fAttenuation;
		float m_fRange;

		int m_iBoneID;

	private:
		TimeValue m_animStart;
		TimeValue m_animEnd;
	};
}

#endif