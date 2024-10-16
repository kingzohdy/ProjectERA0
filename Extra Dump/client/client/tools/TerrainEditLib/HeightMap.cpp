
#include ".\heightmap.h"

using namespace TE;

CHeightMap::CHeightMap(void)
{
	Destory();
}

CHeightMap::~CHeightMap(void)
{
	Destory();
}
//产生一个新的高度图;参数( 宽度,高度 , 开始坐标 , 每个格子大小 , 基础高度 , 对应缩放 )
void    CHeightMap::CreateMap(MPoint2D nblock, MPoint2D ngrid, MVec3 startPos , float fSize , float fBaseHeight,float fScale)
{
	Destory();

	m_nWidth  = nblock.x*ngrid.x + 1;
	m_nHeight = nblock.y*ngrid.y + 1;

	m_vStartPos = startPos ;

	m_fSize = fSize ;
	m_fScale = fScale ;

	m_vHeight.resize( m_nWidth*m_nHeight );
	m_vVec.resize(m_nWidth*m_nHeight);
	m_vNormal.resize(m_nWidth*m_nHeight);

	for( int i = 0 ; i <  m_nHeight ; i ++ )
	{
		for( int j = 0 ;j < m_nWidth ; j ++ )
		{
			m_vHeight[i*m_nWidth +j] = fBaseHeight;

			m_vVec[i*m_nWidth + j].x = startPos.x + j*fSize ;
			m_vVec[i*m_nWidth + j].z = startPos.z + i*fSize ;
			m_vVec[i*m_nWidth + j].y = fBaseHeight ;

			m_vNormal[i*m_nWidth + j].x = m_vNormal[i*m_nWidth +j].z ;
			m_vNormal[i*m_nWidth + j].y = 1.0f;
		}
	}

	assert(ngrid.x==ngrid.y && nblock.x==nblock.y);
	m_nGridNum = ngrid.x;
};
//清楚信息
void      CHeightMap::Destory()
{
	 m_fScale = 1.0f;
	 m_fSize  = 0.0f;

	 m_nHeight = 0;
	 m_nWidth = 0;
	 
	 m_vHeight.clear();
	 m_vVec.clear();
	 m_vNormal.clear ();
	 
	 m_vSelectInfo.clear();
};	
//更新法线;参数( 索引ID );
void     CHeightMap::UpdateNormal(int index)
{
      int gx  = index % m_nWidth ;
	  int gy  = index / m_nWidth ;

	  UpdateNormal( gx , gy );
}
//更新法线;参数( X位置 , Y 位置 )
void     CHeightMap::UpdateNormal(int indexX, int indexY)
{
    MVec3  pos[4];

	int    index[4];
	index[0] = indexX - 1;
	if( index[0] < 0 ) index[0] = 0 ;
	index[1] = indexX + 1;
	if( index[1] >= m_nWidth )index[1] = m_nWidth-1;
	index[2] = indexY - 1;
	if( index[2] < 0 ) index[2] = 0;
	index[3] = indexY + 1;
	if(index[3] >= m_nWidth ) index[3] = m_nWidth-1;

	int   lastIndex[4];
	lastIndex[0] = index[0] + indexY*m_nWidth;
	lastIndex[1] = indexX  +  index[3]*m_nWidth;
	lastIndex[2] = index[1] + indexY*m_nWidth;
	lastIndex[3] = indexX + index[2]*m_nWidth;

	MVec3 normal = m_vNormal[indexY*m_nWidth + indexX] ;

	m_vNormal[indexY*m_nWidth + indexX] = CountNormal( m_vVec[indexY*m_nWidth+indexX],
		m_vVec[lastIndex[0]] , m_vVec[lastIndex[3]],
		m_vVec[lastIndex[2]] ,m_vVec[lastIndex[1]] );
};
//选择地图信息;参数( 位置坐标 , 范围大小 , 选择区域的类型 ) ; 返回 ( 为真表示成功 )
bool     CHeightMap::SelectMapInfo(MVec3 pos ,float fsize , HEIGHT_AREA area)
{
	static MVec3 savePos(0,0,0);
	static float fsavesize = 0.0f;

	if( savePos == pos && fsavesize == fsize )
	{
		return false;
	}

	m_vSelectInfo.clear();

	MVec3  curPos[4];
	int    mx[4],my[4];
      
	curPos[0].x = pos.x - fsize ;
	curPos[0].z = pos.z - fsize ;
	curPos[0].y = pos.y ;

	curPos[1].x = pos.x + fsize ;
	curPos[1].z = pos.z - fsize ;
	curPos[1].y = pos.y ;

	curPos[2].x = pos.x - fsize ;
	curPos[2].z = pos.z + fsize ;
	curPos[2].y = pos.y ;

	curPos[3].x = pos.x + fsize ;
	curPos[3].z = pos.z + fsize ;
	curPos[3].y = pos.y ;

	for( int i = 0 ; i < 4; i++ )
	{
		mx[i] = (int) ( ( curPos[i].x - m_vStartPos.x ) / m_fSize );
		my[i] =  (int) ( ( curPos[i].z - m_vStartPos.z ) / m_fSize ) ;
	}

	if( area == PAINT_CIRCLE )//圆形区域
	{	
		mx[0] -= 1;my[0] -= 1;
		mx[1] += 1;my[1] -= 1;
		mx[2] -= 1;my[2] += 1;
		mx[3] += 1;my[3] += 1;
	}
	else if ( area == PAINT_QUAD )//使选中的格子小于实际的线框
	{
		mx[0] += 1;my[0] += 1;
		my[1] += 1;
		mx[2] += 1;
	}

	for( int i = 0 ; i < 4 ; i ++ )
	{
		if( mx[i] < 0 ) mx[i] = 0 ;
		if( my[i] < 0 ) my[i] = 0 ;

		if( mx[i] >= m_nWidth ) mx[i] = 0 ;
		if( my[i] >= m_nHeight ) my[i] = 0 ;
	}

	MVec2  xyPos,centerPos;
	MVec2  curLen;
	centerPos.x = pos .x ;
	centerPos.y = pos. y ;

	float flengthsqr = fsize*fsize ;

	for ( int i = mx[0] ; i < mx[1] ; i ++ )
	{
		for ( int j = my[0] ; j < my[2] ; j++ )
		{
			xyPos .x = m_vVec[i*m_nWidth + j].x ;
			xyPos .y = m_vVec[i*m_nWidth + j].z ;

			curLen = xyPos - centerPos ;

			float curflengthsqr = curLen.lengthSqr();

			if( area == PAINT_CIRCLE )//如果是圆形区域,则判断是否在圆形区域内
			{			
				if ( curflengthsqr <= flengthsqr )
				{
					Select_Info  info;
					info.length = curflengthsqr ;
					info.vecIndex = i*m_nWidth + j;
					m_vSelectInfo.push_back ( info );
				}
			}
			else if ( area == PAINT_QUAD )//方形区域
			{
				Select_Info  info;
				info.length = curflengthsqr ;
				info.vecIndex = i*m_nWidth + j;
				m_vSelectInfo.push_back ( info );
			}
		}
	}

	return true;
}
//计算高度图的法线
void     CHeightMap::CountNormal()
{
	if( m_vVec.empty() )
		return;

	for( int i = 0 ; i < m_nHeight ; i ++ )
	{
		for( int j = 0 ;j < m_nWidth ; j++ )
		{
			if( i == 0 || j == 0)
				continue;

			if( i == m_nHeight-1 || j == m_nWidth - 1)
				continue;

			m_vNormal[j+i*m_nWidth] = CountNormal( m_vVec[j+i*m_nWidth] 
			,m_vVec[j-1+m_nWidth*i]
			, m_vVec[j + m_nWidth*(i-1)] 
			, m_vVec[j + 1+ i*m_nWidth]
			, m_vVec[j+(i+1)*m_nWidth])  ;
		}
	}
}
//改变选中范围的高度曲线变化;参数( 修改高度 , 修改的模式 , 范围 )		
void     CHeightMap::ChangeSelectHeightRadio(float height , HEIGHT_CHANGE_MODE mode ,float fSize )
{
	float changeheight ;
	for( size_t i = 0 ; i < m_vSelectInfo.size () ; i ++ )
	{
		float  fLength = sqrt( m_vSelectInfo[i].length );

		changeheight = height* cos ( ( fLength / fSize ) *MAY_PI / 2.0f ) ;

		if( mode == ALPHA_DEC )
		{
			changeheight = -changeheight ;
		}

		m_vHeight[m_vSelectInfo[i].vecIndex] += changeheight;
		m_vVec[m_vSelectInfo[i].vecIndex].y  = m_vHeight[m_vSelectInfo[i].vecIndex];
	}

};
//改变选中范围的高度线性变化;参数( 修改高度 , 修改模式 ) 
void     CHeightMap:: ChangeSelectHeigthLine(float height , HEIGHT_CHANGE_MODE mode)
{
	float changeHeight ;

	if( mode == ALPHA_ADD )
	{
		changeHeight = height ;
	}
	else 
	{
		changeHeight = -height ;
	}
		
    for( size_t i = 0 ; i < m_vSelectInfo.size () ; i ++ )
	{
		m_vHeight[m_vSelectInfo[i].vecIndex] += changeHeight;
		m_vVec[m_vSelectInfo[i].vecIndex].y  = m_vHeight[m_vSelectInfo[i].vecIndex];
	}
};
//平整选中的高度;参数( 修改高度 )	
void     CHeightMap::LevelOffSelectHeight(float height)
{
	if( m_vSelectInfo.empty() )
		return;

	for ( size_t i = 0 ;i < m_vSelectInfo.size() ; i ++ )
	{
		int index = m_vSelectInfo[i].vecIndex ;

		int mx = index % m_nWidth ;
		int my = index / m_nWidth ;

		if ( mx == 0 || my == 0 )
			continue ;
		if( mx >= m_nWidth-1 || my == m_nHeight )
			continue;

		m_vHeight[index] = height;
	}

};
//平滑选中区域高度
void     CHeightMap::SmoothSelectHeightMapData()
{
	if ( m_vSelectInfo.empty() )
	{
		return ;
	}

	for ( size_t i = 0 ;i < m_vSelectInfo.size() ; i ++ )
	{
		int index = m_vSelectInfo[i].vecIndex ;

		int mx = index % m_nWidth ;
		int my = index / m_nWidth ;

		if ( mx == 0 || my == 0 )
		{
			continue ;
		}
		if( mx >= m_nWidth-1 || my == m_nHeight )
		{
			continue;
		}

		m_vHeight[index] = ( m_vHeight[index] 
		+ m_vHeight[index + 1] 
		+ m_vHeight[index - 1]
		+ m_vHeight[index + m_nWidth]
		+ m_vHeight[index - m_nWidth] ) / 5.0f;
	}
}
//平滑高度图数据
void     CHeightMap::SmoothHeightMapData()
{
	if( m_vHeight.empty() )
	{
		return;
	}

	for( int i = 0 ; i < m_nHeight ; i ++ )
	{
		for( int j = 0 ;j < m_nWidth ; j++ )
		{
			if( i == 0 || j == 0)
			{
				continue;
			}

			if( i == m_nHeight-1 || j == m_nWidth - 1)
			{
				continue;
			}

			m_vHeight[j+i*m_nWidth] = ( m_vHeight[j+i*m_nWidth] 
			+ m_vHeight[j+1+m_nWidth*i]
			+ m_vHeight[j-1 + m_nWidth*i] 
			+ m_vHeight[j + (i-1)*m_nWidth]
			+ m_vHeight[j+(i+1)*m_nWidth]) / 5.0f ;

			m_vVec[j+i*m_nWidth].y = m_vHeight[j+i*m_nWidth] ;
		}
	}
}
// 功能( 计算法线(输入五个顶点位置,返回计算过的法线) );
// 参数( 顶点0 ,  顶点1 , 顶点2 , 顶点3 ,顶点4 ) 
// 返回 ( 返回为计算过的法线 )
MVec3    CHeightMap::CountNormal( MVec3 &v0,MVec3 &v1,MVec3 &v2,MVec3 &v3,MVec3 &v4 )
{
	MVec3  nor1,nor2,nor3,nor4,nor;

	MVec3 v11,v22;
	v11 = (v2 - v0 ) ;
	v22 = ( v1 - v0) ;
	nor1 = CrossProduct( v11,v22 );
	v11 = (v3 - v0 ) ;
	v22 = ( v2 - v0) ;
	nor2 = CrossProduct( v11,v22 );
	v11 = (v4 - v0 );
	v22 = ( v3 - v0);
	nor3 = CrossProduct(v11 ,v22 );
	v11 = (v1 - v0 );
	v22 = ( v4 - v0);
	nor4 = CrossProduct(v11 ,v22 );


	Normalize(nor1);
	Normalize(nor2);
	Normalize(nor3);
	Normalize(nor4);

	nor = ( nor1 + nor2 + nor3 + nor4 ) / 4.0f;

	//可以优化采用下列方法
	/*
	MVec3   nor , nor1 ,nor2 ;
	nor1 = v1 - v3 ;
	nor2 = v2 - v4 ;
	nor = CrossProduct( nor1 , nor2 );*/

	Normalize(nor);
	return nor;
};
//通过X,Z坐标得到高度 ; 参数( X , Z , 返回高度 ) , 返回 ( 为真表示获得高度成功 )
bool     CHeightMap::GetHeight(float x, float z,float*  height)
{	
	(*height) = 0.0f;

	if( m_vHeight.empty() )
		return false;

	float  fx , fz ;
	int    gx , gz ;

	fx =  ( x - m_vStartPos.x ) / m_fSize ;
	fz =  ( z - m_vStartPos.z ) / m_fSize ;

	gx = (int)fx ;
	gz = (int)fz ;

	if( gx < 0 || gx >= m_nWidth )
		return false;

	if( gz < 0 || gz >= m_nHeight )
		return false;

	if( m_vVec[gx + m_nWidth*gz].x == x
		&& m_vVec[gx + m_nWidth*gz].z == z)
	{
		(*height) = m_vVec[gx + m_nWidth*gz].y ;
		return true;
	}

	float dx = fx - gx;
	float dz = fz - gz;
	int hpos[4];
	// (x,z) ---------- (x+1,z)
	//   |                    |
	// (x,z+1)  ----------- (x+1,z+1)

	hpos[0] = gx + m_nWidth*gz;
	hpos[1] = gx + 1 + m_nWidth*gz;

	if(hpos[1] >= (int)m_vHeight.size () )
		hpos[1] = hpos[0];

	hpos[2] = gx  + (gz+1)*m_nWidth ;
	if(hpos[2] > (int)m_vHeight.size () )
		hpos[2] = hpos[0];

	hpos[3] = gx + 1 +(gz+1)*m_nWidth;
	if(hpos[3] > (int)m_vHeight.size ())
		hpos[3] = hpos[0];

	float height1 = m_vVec[hpos[0]].y*(1-dx) + m_vVec[hpos[1]].y*(dx) ;
	float height2 =  m_vVec[hpos[2]].y*(1-dx) + m_vVec[hpos[3]].y*(dx) ;

	(*height) =  height1*(1-dz) + height2*dz;

	return true;
};
//设置对应范围的高度 ; 参数( 高度 ,宽度 , 输入的数据 )	
bool    CHeightMap::SetHeightData( int width ,int height , unsigned char *data)
{
     if( width == m_nWidth && height == m_nHeight )
		 return false;

	 if( m_vHeight.size() != width*height )
		 return false;

	 for ( int i = 0 ; i < width*height ; i ++ )
	 {
         m_vHeight[i] = m_fScale*data[i];
		 m_vVec[i].y = m_vHeight[i] ;
	 }

	 return true;
}
//获得对应格子的高度;参数( 格子坐标X,格子坐标Z , 返回高度 ) 返回 ( 为真表示获得高度成功 )
bool    CHeightMap::GetGridHeight(int x,int z , float* height )
{
	(*height) = 0.0f;

	if( m_vHeight.empty() )
		return false;

	if( x < 0 || x >= m_nWidth )
		return false;

	if( z < 0 || z >= m_nHeight )
		return false;

 
	(*height) = m_vHeight[x*z];

	return true;
};
//对高度图 ;参数( 文件名 ) 返回 ( 为真表示成功 )
bool CHeightMap::ReadBmp(const  char* filename)
{
	return false;
};
//功能: 处理高度图 
// 参数: ( 处理事件 , 改变高度模式 , 修改范围 , 中心位置 , 修改高度 , 修改大小 , 是否改变 )
// 返回: ( 返回为真表示成功 )
bool 
CHeightMap:: ProcessMap(HEIGHT_PROC_EVEN  even ,HEIGHT_CHANGE_MODE mode,HEIGHT_AREA area,  MVec3  centerPos , float height ,float fsize,bool bChange)
{
	if( m_vHeight.empty() )
		return false;

	//获得当前选取的点的队列
	SelectMapInfo(centerPos , fsize , area );

	//如果不需要修改,则只进行选择顶点队列
	if( !bChange )
		return true;

	switch( even )
	{
	case 	SOFT_MODE://平滑
		{
			ChangeSelectHeightRadio( height ,mode ,fsize );
		}
		break;
	case		HARD_MODE://线性
		{
			ChangeSelectHeigthLine( height , mode );
		}
		break;
	case		SMOOTH_MODE ://曲线
		{
			SmoothSelectHeightMapData();
		}
		break;

	case LEVEL_OFF: //平整
		{
			LevelOffSelectHeight( height );
		}
		break;

	case RAMP_MODE :
		{

		}
		break;

	case DITCH_MODE:
		{

		}
		break;
	case SMOOTH_ALL_MODE://平滑所有高度
		{
			SmoothHeightMapData();
		}
		break;
	}

	return true;

};