#ifdef _USE_ODL_COORD

#include ".\mcoordinate.h"
#include "EditorModel.h"
#include "EditorCamera.h"

#include "mterrainedit.h"

#include "OgreRay.h"
#include "OgreModel.h"
#include "OgreLines.h"

using namespace TE;
using namespace Ogre;

MCoordinate::MCoordinate() : m_CurCoordSelect(NO_AXES_SELECT), m_pCurBinObject(NULL), m_fScale(1.0f), m_nCurMode(0), m_fScaleObject(1.0f), m_vCurPos(0,0,0), m_vSaveObjectPos(0,0,0)
{
	memset( &m_RenderData , 0 , sizeof( MCoordinateRenderData ));
	memset( &m_BallRenderData , 0 , sizeof( MCoordinateRenderData ));
	memset( &m_ScaleRenderData , 0 , sizeof( MCoordinateRenderData ));	

	float  _pi = MAY_PI / 16.0f ; 
	for(int i = 0 ; i < 32 ; i ++ )
	{
		m_BallPosX[i] = 600*sin( i*_pi ); 
		m_BallPosY[i] = 600*cos( i*_pi );
	}
}

MCoordinate::~MCoordinate(void)
{
	Release();
}
//相交测试;参数( 射线方向,射线位置,模式( 为1表示改变坐标颜色 ) ) 返回 ( 坐标选中状态 )
COORDINATE_SELECT   MCoordinate::TestIntersectEx( MVec3  &dir , MVec3 &pos ,int mode)
{
	m_CurCoordSelect  = NO_AXES_SELECT ;
    static Ray  ray;
	ray.m_Dir = dir;
	ray.m_Origin = pos;

	if( ray.intersectBox(&m_xAxesBox))
	{
		m_CurCoordSelect = SELECT_X_AXES ;

		if ( mode == 1 )
		{	
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i].color = AXES_SELECT_COLOR;
				}
			
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+8].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+16].color = AXES_Z_COLOR;
				}
			}
		}
		return m_CurCoordSelect;
	}
	else if ( ray.intersectBox ( &m_yAxesBox ))
	{
		m_CurCoordSelect = SELECT_Y_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
	   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+8].color = AXES_SELECT_COLOR;
				}

				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+16].color = AXES_Z_COLOR;
				}
			}
		}
		return m_CurCoordSelect;
	}
	else if( ray.intersectBox ( &m_zAxesBox ))
	{
		m_CurCoordSelect = SELECT_Z_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+8].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+16].color = AXES_SELECT_COLOR ;
				}

    		}
		}
		return m_CurCoordSelect;
	}

	if ( mode == 1 )	
	{			
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+8].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 8 ; i ++)
				{
				    vdata[i+16].color = AXES_Z_COLOR ;
				}

			}
	}

	return m_CurCoordSelect;
}
//测试球体的相交;参数( 射线方向,射线方向,模式 ) , 返回 ( 坐标选中状态 )
COORDINATE_SELECT   MCoordinate::TestIntersectBall(MVec3 &dir , MVec3 &pos ,int mode)
{   
	if( m_pCurBinObject == NULL )
		return NO_AXES_SELECT ;

	m_CurCoordSelect  = NO_AXES_SELECT ;
	
	EditorModel*  pModel = (EditorModel*)m_pCurBinObject;

	MVec3 minPos,maxPos;
	minPos = pModel->GetShowModel()->getWorldPosition();

	MVec3  xFacePos ; 
	xFacePos.x = minPos.x ;
	float  dlength = minPos.x - pos.x ;

	xFacePos.y = pos.y + dlength*dir.y / dir.x;
	xFacePos.z = pos.z + dlength*dir.z / dir.x ;

	MVec3 _length = xFacePos - minPos ;

	LineVec_st* vdata = m_BallRenderData.vdata;
	
		for( int i = 0 ; i < 33 ; i ++)
		{
			if( i <= 15  || i  == 32)
			vdata[i].color = AXES_X_COLOR;
			else
				vdata[i].color = AXES_GRAY_COLOR;
		}
		for( int i = 0 ; i < 33 ; i ++)
		{
			vdata[i+33].color = AXES_Y_COLOR;
		}
		for( int i = 0 ; i < 33 ; i ++)
		{
			if( i <= 7 || ( i >= 24 && i <= 32 ) )
			vdata[i+66].color = AXES_Z_COLOR;
			else
				vdata[i+66].color = AXES_GRAY_COLOR;
		}

	//与X平面上的圆相交
		float _flength = _length.lengthSqr () ;

		float _flengthMax = m_fScaleObject*m_fScaleObject*360000.8f ;
		float _flengthMin = m_fScaleObject*m_fScaleObject*400000.6f ;
	if(  _flength <  _flengthMax && _flength > _flengthMin )
	{
		for( int i = 0 ; i < 33 ; i ++)
		{
           vdata[i].color = AXES_SELECT_COLOR ;

		}	
		m_CurCoordSelect  = SELECT_X_AXES ;
	}

	MVec3  yFacePos ; 
	yFacePos.y = minPos.y ;
	dlength = minPos.y - pos.y ;

	yFacePos.x = pos.x + dlength*dir.x / dir.y;
	yFacePos.z = pos.z + dlength*dir.z / dir.y ;

	_length = yFacePos - minPos ;	

	_flength = _length.lengthSqr () ;
	//与Y平面上的圆相交
	if( _flength < _flengthMax && _flength> _flengthMin )
	{
		for( int i = 0 ; i < 33 ; i ++)
		{
           vdata[i+33].color  = AXES_SELECT_COLOR ;
		}
		m_CurCoordSelect  = SELECT_Y_AXES ;
	}

	MVec3  zFacePos ; 
	zFacePos.z = minPos.z ;

	dlength = minPos.z - pos.z;
	zFacePos.x = pos.x + dlength*dir.x / dir.z;
	zFacePos.y = pos.y + dlength*dir.y / dir.z ;

	_length = zFacePos - minPos ;	
	_flength = _length.lengthSqr () ;
	//与Y平面上的圆相交
	if( _flength < _flengthMax && _flength> _flengthMin )
	{
		for( int i = 0 ; i < 33 ; i ++)
		{
           vdata[i+66].color  = AXES_SELECT_COLOR;
		}
		m_CurCoordSelect  = SELECT_Z_AXES ;
	}

	return  m_CurCoordSelect;
}
//测试XY面
MVec3  TestAndXYFace( MVec3 pos , MVec3 &dir , float dz )
{
	MVec3  _pos(0,0,0);

	float fk = dz - pos.z  ;
	fk = fk / dir.z ;

	_pos.x = pos.x + fk * dir.x ;
	_pos.y = pos.y + fk * dir.y ;
	_pos.z = dz ;

	return _pos ;
}
//测试XZ面
MVec3  TestAndXZFace( MVec3 pos ,MVec3 &dir , float dy )
{
	MVec3  _pos(0,0,0);

	float fk = dy - pos.y ;
	fk = fk / dir.y ;

	_pos.x = pos.x + fk * dir.x ;
	_pos.z = pos.z + fk * dir.z ;
	_pos.y = dy ;

	return _pos ;
}
//测试YZ面;
MVec3  TestAndYZFace(MVec3 pos , MVec3 &dir , float dx )
{
	MVec3  _pos(0,0,0);

	float fk = dx - pos.x  ;
	fk = fk / dir.x ;

	_pos.y = fk * dir.y ;
	_pos.z = fk * dir.z ;
	_pos.x = dx ;

	return _pos ;
}
//测试移动坐标
COORDINATE_SELECT   MCoordinate::TestIntersect( MVec3  &dir , MVec3 &pos ,int mode)
{
	if( m_nCurMode == 1 )
	{
		//旋转
        return TestIntersectBall(dir,pos,mode );
	}

	m_CurCoordSelect  = NO_AXES_SELECT ;
    static Ray  ray;
	ray.m_Dir = dir;
	ray.m_Origin = pos;

	if( ray.intersectBox ( &m_xAxesBox ))
	{
		m_CurCoordSelect = SELECT_X_AXES ;

		if ( mode == 1 )
		{	
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_SELECT_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_Z_COLOR ;
				}
			}
		}
		return m_CurCoordSelect;
	}
	else if ( ray.intersectBox ( &m_yAxesBox ))
	{
		m_CurCoordSelect = SELECT_Y_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_SELECT_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_Z_COLOR ;
				}

			}
		}
		return m_CurCoordSelect;
	}
	else if( ray.intersectBox ( &m_zAxesBox ))
	{
		m_CurCoordSelect = SELECT_Z_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_SELECT_COLOR ;
				}
			}
		}
		return m_CurCoordSelect;
	}
	else if ( ray.intersectBox( &m_xyAxesBox ) )
	{
		m_CurCoordSelect = SELECT_XY_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_SELECT_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_SELECT_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_Z_COLOR ;
				}
			}
			m_vCurPos = TestAndXYFace( pos, dir , m_xyAxesBox.getMinPos().z );
		}

		return m_CurCoordSelect;
	}
	else if ( ray.intersectBox( &m_xzAxesBox ) )
	{
		m_CurCoordSelect = SELECT_XZ_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_SELECT_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_SELECT_COLOR ;
				}
			}

			m_vCurPos = TestAndXZFace(pos, dir , m_xzAxesBox.getMinPos().y );
		}

		return m_CurCoordSelect;
	}
	else if ( ray.intersectBox( &m_yzAxesBox ) )
	{
		m_CurCoordSelect = SELECT_YZ_AXES ;

		if ( mode == 1 )
		{
			LineVec_st* vdata = m_RenderData.vdata;
		   
			if(vdata != NULL)
			{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_SELECT_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_SELECT_COLOR ;
				}

			}
			
			m_vCurPos = TestAndYZFace( pos,dir , m_yzAxesBox.getMinPos().x );
		}

		return m_CurCoordSelect;
	}


	if ( mode == 1 )	
	{			
		LineVec_st* vdata = m_RenderData.vdata;
				
		if(vdata != NULL)		
		{
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i].color = AXES_X_COLOR;
				}
			
				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+6].color = AXES_Y_COLOR;
				}

				for( int i = 0 ; i < 6 ; i ++)
				{
				    vdata[i+12].color = AXES_Z_COLOR ;
				}

		}
	}

	return m_CurCoordSelect;
};
//绑定物体	
void     MCoordinate::BindObject( EditorModel* model  )
{
	m_pCurBinObject = model;
};
//更新球体数据 ; 参数( 更新 )		
void    MCoordinate::UpdateBall(float dtime)
{
	if( m_pCurBinObject == NULL )
		return;

	EditorModel*  pModel = (EditorModel*)m_pCurBinObject;

	MVec3  curPos[24];

	MVec3 minPos,maxPos;
	minPos = pModel->GetShowModel()->getWorldPosition();

	//更新顶点数据
	if(1)
	{
		LineVec_st* vdata = m_BallRenderData.vdata;
	   
		if(vdata == NULL)
			return ;

		for( int i = 0 ; i < 32 ; i ++)
		{
           vdata[i].pos = minPos;
		   vdata[i].pos .y += m_fScaleObject*m_BallPosX[i];
		   vdata[i].pos .z += m_fScaleObject*m_BallPosY[i];
		}
        vdata[32].pos = minPos;
		vdata[32].pos .y += m_fScaleObject*m_BallPosX[0];
		vdata[32].pos .z += m_fScaleObject*m_BallPosY[0];

		for( int i = 0 ; i < 32 ; i ++)
		{
           vdata[i+33].pos = minPos;
		   vdata[i+33].pos.z += m_fScaleObject*m_BallPosX[i];
		   vdata[i+33].pos.x += m_fScaleObject*m_BallPosY[i];
		}
           vdata[65].pos = minPos;
		   vdata[65].pos.z += m_fScaleObject*m_BallPosX[0];
		   vdata[65].pos.x += m_fScaleObject*m_BallPosY[0];

		for( int i = 0 ; i < 32 ; i ++)
		{
           vdata[i+66].pos = minPos;
		   vdata[i+66].pos .x += m_fScaleObject*m_BallPosX[i];
		   vdata[i+66].pos .y += m_fScaleObject*m_BallPosY[i];
		}        
		vdata[98].pos = minPos;
		   vdata[98].pos .x += m_fScaleObject*m_BallPosX[0];
		   vdata[98].pos .y += m_fScaleObject*m_BallPosY[0];

	}

};
//移动位置坐标更新;
void    MCoordinate::UpdateEx (float dtime)
{
	if( m_pCurBinObject == NULL )
		return;


	EditorModel*  pModel = (EditorModel*)m_pCurBinObject;

	MVec3  curPos[24];

	MVec3 minPos,maxPos;
	minPos = pModel->GetShowModel()->getWorldPosition();

	MVec3  poslen = g_Camera.GetPosition () - minPos ;

	float lensqr = poslen.length () ;
	
	if( lensqr < 100 )
	{
	    m_fScale = 1.0f;
	}
	else
	{
		m_fScale = 1.0f + ( lensqr - 100 )/50.0f;
	}

	float axes_lenght = 4.0f*m_fScale ;
	float axes_size  = 0.2f*m_fScale;

	maxPos    = minPos ;
	maxPos.x  = minPos.x +  axes_lenght ;
	maxPos.y  = minPos.y + axes_size ;
	maxPos.z  = minPos.z + axes_size ;
	m_xAxesBox.setRange ( minPos , maxPos );
	//生成8个顶点
	curPos[0] = minPos;
	curPos[1].y = minPos.y ;curPos[1].x = maxPos.x;curPos[1].z = minPos.z ;
	curPos[3].y = minPos.y ;curPos[3].x = minPos.x;curPos[3].z = maxPos.z ;
	curPos[2].y = minPos.y ;curPos[2].x = maxPos.x;curPos[2].z = maxPos.z ;

	curPos[6] = maxPos;
	curPos[5].y = maxPos.y ;curPos[5].x = maxPos.x;curPos[5].z = minPos.z ;
	curPos[7].y = maxPos.y ;curPos[7].x = minPos.x;curPos[7].z = maxPos.z ;
	curPos[4].y = maxPos.y ;curPos[4].x = minPos.x;curPos[4].z = minPos.z ;

 	maxPos    = minPos ;
	maxPos.y  = minPos.y +  axes_lenght ;
	maxPos.x  = minPos.x +  axes_size ;
	maxPos.z  = minPos.z +  axes_size ;
	m_yAxesBox.setRange ( minPos , maxPos );
	//生成8个顶点
	curPos[0+8] = minPos;
	curPos[1+8].y = minPos.y ;curPos[1+8].x = maxPos.x;curPos[1+8].z = minPos.z ;
	curPos[3+8].y = minPos.y ;curPos[3+8].x = minPos.x;curPos[3+8].z = maxPos.z ;
	curPos[2+8].y = minPos.y ;curPos[2+8].x = maxPos.x;curPos[2+8].z = maxPos.z ;

	curPos[6+8] = maxPos;
	curPos[5+8].y = maxPos.y ;curPos[5+8].x = maxPos.x;curPos[5+8].z = minPos.z ;
	curPos[7+8].y = maxPos.y ;curPos[7+8].x = minPos.x;curPos[7+8].z = maxPos.z ;
	curPos[4+8].y = maxPos.y ;curPos[4+8].x = minPos.x;curPos[4+8].z = minPos.z ;

	maxPos    = minPos ;
	maxPos.z  = minPos.z +  axes_lenght ;
	maxPos.y  = minPos.y +  axes_size ;
	maxPos.x  = minPos.x +  axes_size ;
	m_zAxesBox.setRange ( minPos , maxPos );
	//生成8个顶点
	curPos[0+16] = minPos;
	curPos[1+16].y = minPos.y ;curPos[1+16].x = maxPos.x;curPos[1+16].z = minPos.z ;
	curPos[3+16].y = minPos.y ;curPos[3+16].x = minPos.x;curPos[3+16].z = maxPos.z ;
	curPos[2+16].y = minPos.y ;curPos[2+16].x = maxPos.x;curPos[2+16].z = maxPos.z ;

	curPos[6+16] = maxPos;
	curPos[5+16].y = maxPos.y ;curPos[5+16].x = maxPos.x;curPos[5+16].z = minPos.z ;
	curPos[7+16].y = maxPos.y ;curPos[7+16].x = minPos.x;curPos[7+16].z = maxPos.z ;
	curPos[4+16].y = maxPos.y ;curPos[4+16].x = minPos.x;curPos[4+16].z = minPos.z ;

	m_xyAxesBox.setRange( minPos ,MVec3( minPos.x + 50 , minPos.y + 50 , minPos.z + 5 ) );
	m_yzAxesBox.setRange( minPos ,MVec3( minPos.x + 5  , minPos.y + 50 , minPos.z + 50 ) );
	m_xzAxesBox.setRange( minPos ,MVec3( minPos.x + 50 , minPos.y + 5 , minPos.z + 50 ) );
	//更新顶点数据
	for( int i = 0 ; i < 24 ; i ++)
	{
        m_RenderData.vdata[i].pos = curPos[i];
	}
}

//坐标更新
void    MCoordinate::Update(float dtime)
{
	if( m_pCurBinObject == NULL )
		return;

	MVec3 minPos,maxPos;

	if( g_TerrainEdit.m_CurSelectModelSet.empty () )
	{
		EditorModel*  pModel = (EditorModel*)m_pCurBinObject;
		minPos = pModel->GetShowModel()->getWorldPosition();
	}
	else
	{
		EditorModel*  pModel = (EditorModel*)m_pCurBinObject;
		MVec3  allPos = pModel->GetShowModel()->getWorldPosition();

		for( unsigned int i = 0 ; i < g_TerrainEdit.m_CurSelectModelSet.size () ; i ++)
		{ 
			EditorModel*  pModel = (EditorModel*)g_TerrainEdit.m_CurSelectModelSet [i]->GetModelData ()->pModel ;
			MVec3 pos = pModel->GetShowModel()->getWorldPosition();

			allPos += pos;
		}

		minPos.x = allPos.x / ( 1 + g_TerrainEdit.m_CurSelectModelSet.size () );
		minPos.y = allPos.y / ( 1 + g_TerrainEdit.m_CurSelectModelSet.size () );
		minPos.z = allPos.z / ( 1 + g_TerrainEdit.m_CurSelectModelSet.size () );
	}

	MVec3  poslen = g_Camera.GetPosition() - minPos ;

	float lensqr = poslen.length () ;
	
	if( lensqr < 100 )
	{
	    m_fScaleObject = 1.0f;
	}
	else
	{
		m_fScaleObject = 1.0f ;//+ ( lensqr - 100 )/50.0f;
	}

	if( m_nCurMode == 1)
	{
		UpdateBall(dtime);
		return;
	}

	maxPos    = minPos ;
	maxPos.x  = minPos.x +  m_fScaleObject*AXES_LENGHT ;
	maxPos.y  = minPos.y +  AXES_SIZE;
	maxPos.z  = minPos.z +  AXES_SIZE;
	m_xAxesBox.setRange ( minPos , maxPos );

 	maxPos    = minPos ;
	maxPos.y  = minPos.y +  m_fScaleObject*AXES_LENGHT ;
	maxPos.x  = minPos.x +  AXES_SIZE;
	maxPos.z  = minPos.z +  AXES_SIZE;
	m_yAxesBox.setRange ( minPos , maxPos );

	maxPos    = minPos ;
	maxPos.z  = minPos.z +  m_fScaleObject*AXES_LENGHT ;
	maxPos.y  = minPos.y +  AXES_SIZE;
	maxPos.x  = minPos.x +  AXES_SIZE;
	m_zAxesBox.setRange ( minPos , maxPos );

	m_xyAxesBox.setRange( minPos ,MVec3( minPos.x + 50 , minPos.y + 50 , minPos.z + 10 ) );
	m_yzAxesBox.setRange( minPos ,MVec3( minPos.x + 10  , minPos.y + 50 , minPos.z + 50 ) );
	m_xzAxesBox.setRange( minPos ,MVec3( minPos.x + 50 , minPos.y + 10 , minPos.z + 50 ) );
	//更新顶点数据

		LineVec_st *vdata = m_RenderData.vdata;
         //更新六个顶点
		//x
		vdata[0].pos = minPos;

		vdata[1].pos = minPos;
		vdata[1].pos .x +=  m_fScaleObject*AXES_LENGHT;

		vdata[2].pos = vdata[1].pos ;
		vdata[3].pos = vdata[1].pos ;
		vdata[4].pos = vdata[1].pos ;
		vdata[5].pos = vdata[1].pos ;

		vdata[3].pos.x -= m_fScaleObject*20.0f;
		vdata[3].pos .z -= m_fScaleObject*20.0f;

		vdata[5].pos.x -=m_fScaleObject*20.0f;
		vdata[5].pos .z += m_fScaleObject*20.0f;
        //y
		vdata[6].pos = minPos;

		vdata[7].pos = minPos;
		vdata[7].pos .y +=  m_fScaleObject*AXES_LENGHT;

		vdata[8].pos = vdata[7].pos ;
		vdata[9].pos = vdata[7].pos ;
		vdata[10].pos = vdata[7].pos ;
		vdata[11].pos = vdata[7].pos ;

		vdata[9].pos .x -= m_fScaleObject*20.0f;
		vdata[9].pos.y -= m_fScaleObject*20.0f;

		vdata[11].pos .x += m_fScaleObject*20.0f;
		vdata[11].pos.y -= m_fScaleObject*20.0f;
		//z
		vdata[12].pos = minPos;

		vdata[13].pos = minPos;
		vdata[13].pos .z +=  m_fScaleObject*AXES_LENGHT;

		vdata[14].pos = vdata[13].pos ;
		vdata[15].pos = vdata[13].pos ;
		vdata[16].pos = vdata[13].pos ;
		vdata[17].pos = vdata[13].pos ;

		vdata[15].pos .x -= m_fScaleObject*20.0f;
		vdata[15].pos .z -= m_fScaleObject*20.0f;

		vdata[17].pos .x += m_fScaleObject*20.0f;
		vdata[17].pos .z -= m_fScaleObject*20.0f;
};
//坐标渲染
void   MCoordinate::RenderEx()
{
	g_TerrainEdit.m_pLinesRenderer->addTriangleList(&m_RenderData.vdata[0].pos, (const ColorQuad *)&m_RenderData.vdata[0].color, sizeof(LineVec_st), m_RenderData.idata, 108);
}

//球形坐标渲染
void   MCoordinate::RenderBall()
{
	Vector3 pt[33];
	for(int c=0; c<3; c++)
	{
		for(int i=0; i<33; i++)
		{
			pt[i] = m_BallRenderData.vdata[c*3+i].pos;
		}
		g_TerrainEdit.m_pLinesRenderer->addLineStrip(pt, 33, m_BallRenderData.vdata[c*3].color, true);
	}
}

//缩放坐标渲染
void   MCoordinate::Render()
{ 	
	if( m_pCurBinObject == NULL )
		return;

	if( m_nCurMode == 1 )
	{
		RenderBall();
		return;
	}

	Vector3 pt[18];
	for(int i=0; i<18; i++)
	{
		pt[i] = m_RenderData.vdata[i].pos;
	}
	g_TerrainEdit.m_pLinesRenderer->addLineList(pt, 18, m_RenderData.vdata[0].color);
};
//初始化扩展	
bool   MCoordinate::InitEx()
{
    CreateDateEx();
	CreateHLSLData();

	return true;
}
//初始化	
bool   MCoordinate::Init()
{
    CreateData();
	CreateBallData();

	CreateHLSLData();

	return true;
};
//释放数据	
void    MCoordinate::Release()
{
	m_pCurBinObject = NULL;
};
//创建数据扩展
bool  MCoordinate::CreateDateEx()
{
	// x , y , z 轴, 6个坐标点
	LineVec_st* vdata = m_RenderData.vdata;
					
	if(vdata != NULL)		
	{
		for( int i = 0 ; i < 3 ; i++)
		{
			switch( i )
			{
			case 0:
				for (int k = 0 ; k < 8 ; k++)
				{
					vdata[k].color = AXES_X_COLOR;		
				}
			break;
			case 1:
							
				for (int k = 0 ; k < 8 ; k++)
				{
					vdata[8+k].color = AXES_Y_COLOR;		
				}
			case 2:
								
				for (int k = 0 ; k < 8 ; k++)
				{
					vdata[16+k].color = AXES_Z_COLOR;		
				}
				break;
			}

		}
	}

	unsigned short*  idata = m_RenderData.idata;
	for( int i = 0 ; i < 3 ; i++)
	{
		//下面
		idata[0+i*36] = 1 + i*8;
		idata[1+i*36] = 0  + i*8;
		idata[2+i*36] = 2  + i*8;
		idata[3+i*36] = 0  + i*8;
		idata[4+i*36] = 2  + i*8;
		idata[5+i*36] = 3  + i*8;
		//上面
		idata[6+i*36] = 5  + i*8;
		idata[7+i*36] = 4  + i*8;
		idata[8+i*36] = 6  + i*8;
		idata[9+i*36] =  4   + i*8;
		idata[10+i*36] = 6   + i*8;
		idata[11+i*36] = 7   + i*8;
		//前面
		idata[12+i*36] = 1  + i*8;
		idata[13+i*36] = 0  + i*8;
		idata[14+i*36] = 5  + i*8;
		idata[15+i*36] = 0  + i*8;
		idata[16+i*36] = 5  + i*8;
		idata[17+i*36] = 4  + i*8;
		//后面
		idata[18+i*36] = 2  + i*8;
		idata[19+i*36] = 3  + i*8;
		idata[20+i*36] = 6  + i*8;
		idata[21+i*36] = 3  + i*8;
		idata[22+i*36] = 6  + i*8;
		idata[23+i*36] = 7  + i*8;
		//左面
		idata[24+i*36] = 0  + i*8;
		idata[25+i*36] = 3  + i*8;
		idata[26+i*36] = 4  + i*8;
		idata[27+i*36] = 3  + i*8;
		idata[28+i*36] = 4  + i*8;
		idata[29+i*36] = 7  + i*8;
		//右面
		idata[30+i*36] = 1  +  i*8;
		idata[31+i*36] = 2  +  i*8;
		idata[32+i*36] = 5  +  i*8;
		idata[33+i*36] = 2  +  i*8;
		idata[34+i*36] = 5  +  i*8;
		idata[35+i*36] = 6  +  i*8;
	}

	return true;
}
//创建球体渲染
bool  MCoordinate::CreateBallData()
{
	LineVec_st* vdata = m_BallRenderData.vdata;
					
	if(vdata != NULL)		
	{
		for( int i = 0 ; i < 33 ; i ++)
		{
			if( i <= 15  || i  == 32)
			vdata[i].color = AXES_X_COLOR;
			else
				vdata[i].color = AXES_GRAY_COLOR;

		}
		for( int i = 0 ; i < 33 ; i ++)
		{			
			vdata[i+33].color = AXES_Y_COLOR;
		}
		for( int i = 0 ; i < 33 ; i++)
		{		
			if( i <= 7 || ( i >= 24 && i <= 32 ) )
			vdata[i+66].color = AXES_Z_COLOR;
			else
				vdata[i+66].color = AXES_GRAY_COLOR;
		}
	}

	return true;
}
//创建缩放坐标数据	
bool  MCoordinate::CreateSacleData()
{
	LineVec_st* vdata = m_RenderData.vdata;
					
	if(vdata != NULL)		
	{
		for( int i = 0 ; i < 6 ; i ++)
		{	
			vdata[i].color = AXES_X_COLOR;		
		}
		for( int i = 0 ; i < 6 ; i ++)
		{			
			vdata[i+6].color = AXES_Y_COLOR;	
		}
		for( int i = 0 ; i < 6 ; i++)
		{		
			vdata[i+12].color = AXES_Z_COLOR;		
		}
	}

	return true;
};
//创建移动坐标数据
bool  MCoordinate::CreateData()
{
	LineVec_st* vdata = m_RenderData.vdata;
					
	if(vdata != NULL)		
	{
		for( int i = 0 ; i < 6 ; i ++)
		{	
			vdata[i].color = AXES_X_COLOR;		
		}
		for( int i = 0 ; i < 6 ; i ++)
		{			
			vdata[i+6].color = AXES_Y_COLOR;	
		}
		for( int i = 0 ; i < 6 ; i++)
		{		
			vdata[i+12].color = AXES_Z_COLOR;		
		}
	}

    return true;
};
//测试XY面 ; 参数( 射线位置,射线方向 )	; 返回 ( 距离 )	
MVec3   MCoordinate::GetRayIntersectXY( MVec3& pos , MVec3&dir )
{
	return TestAndXYFace( pos, dir , m_xyAxesBox.getMinPos().z );
};
//测试XZ面; 参数( 射线位置,射线方向 )	; 返回 ( 距离 )		
MVec3   MCoordinate::GetRayIntersectXZ( MVec3& pos , MVec3&dir )
{
	return TestAndXZFace( pos, dir , m_xzAxesBox.getMinPos().y );
};
//测试YZ面; 参数( 射线位置,射线方向 )	; 返回 ( 距离 )		
MVec3   MCoordinate::GetRayIntersectYZ( MVec3& pos , MVec3&dir )
{
	return TestAndYZFace( pos, dir , m_yzAxesBox.getMinPos().x);
};
//引导SHADER
bool  MCoordinate::CreateHLSLData()
{
	return true;
};

#endif