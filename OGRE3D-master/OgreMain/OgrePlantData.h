
#ifndef __PlantData_H__
#define __PlantData_H__

#include "OgreResource.h"
#include "OgreVector3.h"
#include <map>
#include <vector>
#include <string>

namespace Ogre
{
	struct PlantInfo_T
	{
		float m_fHeight,m_fWidth;
	};

	//植物种类
	struct PlantHeaderInfo_T
	{
		int plantKind;
	};

	//对应信息
	struct PlantHeader_T
	{
		char  fileName[128];
		int   plantNum;
	};

	struct PlantVecInfo_T
	{
		std::vector <Vector3>   m_PosSet;
		std::vector <float>     m_ScaleSet;
		std::vector<uint> m_VertColorSet;
		TextureData *m_pTexture;
	};

	class _OgreExport PlantSource : public Resource
	{
		DECLARE_RTTI(PlantSource)

	public:
		PlantSource();

	protected:
		~PlantSource();
		virtual void _serialize(Archive &ar, int version);

	public:
		typedef    std::map<FixedString, PlantVecInfo_T>PLANTVECINFO_MAP;
		PLANTVECINFO_MAP m_PlantData;
	};
}

#endif