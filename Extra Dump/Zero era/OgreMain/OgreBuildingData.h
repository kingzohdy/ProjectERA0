
#ifndef __BuildingData_H__
#define __BuildingData_H__

#include "OgreResource.h"
#include "OgreVector3.h"
#include "OgreBounding.h"
#include <list>

namespace Ogre
{
#define MAX_CELLPATHNAME_LENGTH			128

	class Portal
	{
	public:
		Portal()
		{
			nCellFront = -1;
			nCellBack = -1;
		}

		Vector3			vPos[10];
		Vector3			vNormal;

		int				nNumPoints;							// 点数目
		int				nCellFront;							// 正面连接的区域
		int				nCellBack;							// 反面连接的区域
	};

	class Cell
	{
	public:
		char					m_szChunkFile[MAX_CELLPATHNAME_LENGTH];			// mesh对应的chunk文件名
		int						m_nOutdoorLighting;			// 是否能看到天空盒子，采用室外光照
		int						m_nIndoor;					// 是否是室内区域
		std::list<int>			m_listPortals;				// 绑定到本区域的Portal
	};

	class _OgreExport BuildingData : public Resource
	{
		DECLARE_RTTI(BuildingData)

	public:
		BuildingData();

		BoxBound				m_bounding;

		int							m_nNumPortals;				// Portal数目
		int							m_nNumCells;				// Cell数目
		std::vector<Portal>			m_vectorPortals;			// 保存所有的Portal
		std::vector<Cell>			m_vectorCells;				// 保存所有的Cell

	protected:
		~BuildingData();
	};
}

#endif