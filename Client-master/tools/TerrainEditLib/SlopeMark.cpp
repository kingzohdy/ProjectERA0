
//����б�±��;��Ҫ��ʾ�����ı��
#include "SlopeMark.h"
#include "TerrainEdit.h"

#include "OgreLines.h"

using namespace Ogre;

namespace TE
{
	SlopeMark::SlopeMark(void)
	{
	}

	SlopeMark::~SlopeMark(void)
	{
		Release();
	}
	//��ʼ��;����( Ϊ���ʾ�ɹ� )	
	bool  SlopeMark::Init()
	{
		return true;
	};
	//�ͷ�	
	void  SlopeMark::Release()
	{
	};

	//��Ⱦ	
	void  SlopeMark::Render()
	{
	};

	//����λ��;����( ԴĿ��λ�� , Ŀ��Ŀ��λ�� , ��С ) 	
	void  SlopeMark::SetPos(Ogre::Vector3  posSrc,Ogre::Vector3 posDest , float size)
	{
		if( posDest == posSrc )
			return;

		Ogre::Vector3   normal = posDest  - posSrc ;
		Ogre::Normalize(normal);

		Ogre::Vector3   _len ;
		_len.y = 0 ;
		_len.x = -size*normal.z ;
		_len.z = size*normal.x ;
		m_Pos[0] = posSrc + _len;
		_len.x = size*normal.z ;
		_len.z = -size*normal.x ;
		m_Pos[1] = posSrc + _len ;

		_len.y = 0 ;
		_len.x = -size*normal.z ;
		_len.z = size*normal.x ;
		m_Pos[2] = posDest + _len;
		_len.x = size*normal.z ;
		_len.z = -size*normal.x ;
		m_Pos[3] = posDest + _len ;

		g_TerrainEdit.m_pLinesRenderer->addLineStrip(m_Pos, 4, 0x770000ff, true);
	}
}
