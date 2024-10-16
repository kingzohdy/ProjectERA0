
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

		int				nNumPoints;							// ����Ŀ
		int				nCellFront;							// �������ӵ�����
		int				nCellBack;							// �������ӵ�����
	};

	class Cell
	{
	public:
		char					m_szChunkFile[MAX_CELLPATHNAME_LENGTH];			// mesh��Ӧ��chunk�ļ���
		int						m_nOutdoorLighting;			// �Ƿ��ܿ�����պ��ӣ������������
		int						m_nIndoor;					// �Ƿ�����������
		std::list<int>			m_listPortals;				// �󶨵��������Portal
	};

	class _OgreExport BuildingData : public Resource
	{
		DECLARE_RTTI(BuildingData)

	public:
		BuildingData();

		BoxBound				m_bounding;

		int							m_nNumPortals;				// Portal��Ŀ
		int							m_nNumCells;				// Cell��Ŀ
		std::vector<Portal>			m_vectorPortals;			// �������е�Portal
		std::vector<Cell>			m_vectorCells;				// �������е�Cell

	protected:
		~BuildingData();
	};
}

#endif