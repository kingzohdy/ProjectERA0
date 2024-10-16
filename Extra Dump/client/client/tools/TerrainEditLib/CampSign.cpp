
//����б�±��;��Ҫ��ʾ�����ı��
#include ".\campsign.h"
#include "MTerrainEdit.h"

#include "OgreLines.h"

using namespace TE;
using namespace Ogre;

namespace TE
{
	SlopeSign::SlopeSign(void)
	{
	}

	SlopeSign::~SlopeSign(void)
	{
		Release();
	}
	//��ʼ��;����( Ϊ���ʾ�ɹ� )	
	bool  SlopeSign::Init()
	{
		return true;
	};
	//�ͷ�	
	void  SlopeSign::Release()
	{
	};

	//��Ⱦ	
	void  SlopeSign::Render()
	{
	};

	//����λ��;����( ԴĿ��λ�� , Ŀ��Ŀ��λ�� , ��С ) 	
	void  SlopeSign::SetPos(MVec3  posSrc,MVec3 posDest , float size)
	{
		if( posDest == posSrc )
			return;

		MVec3   normal = posDest  - posSrc ;
		Ogre::Normalize(normal);

		MVec3   _len ;
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
