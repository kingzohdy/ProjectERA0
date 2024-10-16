
#ifndef __BeachData_H__
#define __BeachData_H__

#include "OgreResource.h"
#include "OgreVector3.h"
#include "OgreTexture.h"

namespace Ogre
{

	class _OgreExport BeachData : public Resource
	{
	public:
		DECLARE_RTTI(BeachData)
	public:
		BeachData();

	protected:
		~BeachData();
		virtual void _serialize(Archive &ar, int version);

	public:
		float m_Width;
		float m_Height;
		int m_iBlendMode;
		float m_uvXRepeat;
		float m_uvYRepeat;
		float m_fUVSpeed;
		float m_UVTrans;
		float m_fVRange;
		std::vector<Vector3> m_RoadPoints;
		TextureData *m_pTextureRes;
		TextureData *m_pMaskTexture;
	};

}

#endif