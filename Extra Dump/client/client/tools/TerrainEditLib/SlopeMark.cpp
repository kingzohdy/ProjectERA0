
//地形斜坡标记;主要显示操作的标记
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
	//初始化;返回( 为真表示成功 )	
	bool  SlopeMark::Init()
	{
		return true;
	};
	//释放	
	void  SlopeMark::Release()
	{
	};

	//渲染	
	void  SlopeMark::Render()
	{
	};

	//设置位置;参数( 源目标位置 , 目的目标位置 , 大小 ) 	
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
