
#include ".\mboxobject.h"

#include "mstream.h"
#include "MTerrainEdit.h"

#include "OgreLines.h"

using namespace Ogre;
using namespace TE;

MBoxObject::MBoxObject(void)
{
	//memset( &m_Data, 0 , sizeof( MBoxData ));
    m_RenderData.vb = NULL;
    m_RenderData.m_pLayout = NULL;
    m_RenderData.m_pshader = NULL; //, 0 , sizeof( MBoxRenderData ));
    

	m_Data.color = 0xffaaaaaa;
	m_Data.tm.identity();
}

MBoxObject::~MBoxObject(void)
{
	Release();
}

bool  MBoxObject::UpdataData()
{
	MVec3  pos[8];
	pos[0].x  = m_Data.center.x + m_Data.extent .x ; pos[0].y = m_Data.center .y + m_Data.extent .y ; pos[0].z = m_Data.center .z - m_Data.extent .z;
	pos[1].x  = m_Data.center.x + m_Data.extent .x; pos[1].y = m_Data.center .y + m_Data.extent .y; pos[1].z = m_Data.center .z + m_Data.extent .z;
	pos[2].x  = m_Data.center.x - m_Data.extent .x; pos[2].y = m_Data.center .y + m_Data.extent .y; pos[2].z = m_Data.center .z + m_Data.extent .z;
	pos[3].x  = m_Data.center.x - m_Data.extent .x; pos[3].y = m_Data.center .y + m_Data.extent .y; pos[3].z = m_Data.center .z - m_Data.extent .z;
	
	pos[4].x  = m_Data.center.x + m_Data.extent .x; pos[4].y = m_Data.center .y - m_Data.extent .y; pos[4].z = m_Data.center .z - m_Data.extent .z;
	pos[5].x  = m_Data.center.x + m_Data.extent .x; pos[5].y = m_Data.center .y - m_Data.extent .y; pos[5].z = m_Data.center .z + m_Data.extent .z;
	pos[6].x  = m_Data.center.x - m_Data.extent .x; pos[6].y = m_Data.center .y - m_Data.extent .y; pos[6].z = m_Data.center .z + m_Data.extent .z;
	pos[7].x  = m_Data.center.x - m_Data.extent .x; pos[7].y = m_Data.center .y - m_Data.extent .y; pos[7].z = m_Data.center .z - m_Data.extent .z;

	
	for( int i = 0 ; i < 8 ; i++)
	{
       // pos[i] = pos[i]*m_Data.tm;
	    //m_Data.tm .TransformCoord ( pos[i] , pos[i] );
	}

	int index = 0 ;

	Vector3 vdata[24];
	for( int i = 0 ; i < 8 ; i ++)
	{
        vdata[index] = pos[i];

		if ( i == 3)
		{
           vdata[index+1] = pos[0];
		}
		else if(i == 7 )
		{
		   vdata[index+1] = pos[4];
		}
		else
		{
           vdata[index+1] = pos[i+1];
		}	

		index += 2;
	}
    for( int i = 0 ; i < 4 ; i++)
	{
		vdata[index] = pos[i];
		vdata[index+1] = pos[i+4];
		index += 2;
	}

	g_TerrainEdit.m_pLinesRenderer->addLineList(vdata, 24, m_Data.color);
    return true;
};

	
bool   MBoxObject::SetRange(const MVec3 &centerPos , const MVec3 &extent )
{
   m_Data.center = centerPos ;
   m_Data.extent = extent ;
	return true;
};

bool  MBoxObject::CreateRenderData()
{
    return true;
};

bool   MBoxObject::LoadHLSLData()
{
	return true;
};
	
void   MBoxObject::SetColor( unsigned int color )
{
    m_Data.color = color;
};

bool  MBoxObject::Init( MVec3 centerPos, MVec3 extent )
{
	m_Data.center = centerPos;
	m_Data.extent  = extent;

    CreateRenderData();
	LoadHLSLData();

	return true;
};
	
void  MBoxObject::Render()
{
	if(g_TerrainEdit.m_DrawOption.bDrawQuad)
	{
		UpdataData();
	}
};

void  MBoxObject::Release()
{
};
