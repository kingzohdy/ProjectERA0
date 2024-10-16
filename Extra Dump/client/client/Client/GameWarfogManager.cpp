#include "GameWarfogManager.h"
#include "GameMaxMap.h"
#include "GamePlayManager.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "OgreScreenTexture.h"
#include "OgreVertexIndexData.h"
#include "OgreTerrainBlock.h"
#include "OgreTerrainData.h"
#include "OgreHardwareBuffer.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "OgreUIRenderer.h"
#include "OgreTexture.h"
#include "OgreSceneManager.h"
#include "gameteammanager.h"

#include "OgreGameTerrainScene.h"

#include "../UILib/ui_frame.h"

#include <memory.h>
#define	BLOCKLONG 32
#define MAPRADIUS 32
#define TERRAINRADIUS 500000
using namespace Ogre;

GameWarfogManager::GameWarfogManager()
{
	m_pCurTile		= NULL;
	m_pTexdata		= NULL;
	m_pTexsuface	= NULL;
	m_wardistance   = -1;
	m_postnode.clear();
	m_mapNpcnode.clear();
}

GameWarfogManager::~GameWarfogManager()
{
	clear();
}

void GameWarfogManager::setWarfog( int wardistance, float outchroma, float postchroma, float r, float g, float b )
{
	m_wardistance	= wardistance*wardistance;
	m_outchroma		= outchroma;
	m_postchroma	= postchroma;
	m_warcolor[0]	= r;
	m_warcolor[1]	= g;
	m_warcolor[2]	= b;
}


void GameWarfogManager::clear()
{
	m_pCurTile		= NULL;
	m_pTexdata		= NULL;
	m_pTexsuface	= NULL;
}

void GameWarfogManager::updateBlockstate( int indexX, int indexZ, int x, int z )
{	
	VertexData *po1				= m_pCurTile->getBlock(indexX, indexZ)->m_pTerrainBlock->m_pVB;
	HardwareVertexBuffer *pHVB	= po1->m_pVB;//D3D缓存指针
	BlockFogVertex *vdata		= (BlockFogVertex*)po1->lock();
	for ( int i = 0; i <= 33; i++ )
	{
		for ( int i1 = 0; i1 <= 32; i1++ )
		{
			float x1,z1;
			x1 = vdata[i*32+i1].pos.x;
			z1 = vdata[i*32+i1].pos.z;
			if ( (z-z1)*(z-z1) + (x-x1)*(x-x1) < m_wardistance && vdata[i*32+i1].state > 0 )
			{
				vdata[i*32+i1].state = -1;
				if ( pHVB )
				{
					pHVB->m_DataInvalid = true;
				}
			}
		}
	}
}

void GameWarfogManager::updateBlock( float x, float z )
{
	//获得玩家所在的block块，因为玩家的视野绝对小于两个block的长度。所以每次只需检测玩家所在的上下左右和斜方向的block块
	int indexX,indexZ;
	indexX = m_pCurTile->getBlockX(x) - 1;
	indexZ = m_pCurTile->getBlockZ(z) - 1;//指向玩家所在block块的左上方的Block
	for ( int i = 0; i < 3; i++ )
	{
		if ( indexZ + i >= 0 && indexZ + i < m_pCurTile->m_NumBlockZ )//判断当前block坐标有没出界
		{
			for (int j=0;j<3;j++)
			{
				if ( indexX + j >=0 && indexX + j < m_pCurTile->m_NumBlockX )
				{	
					updateBlockstate( indexX + j, indexZ + i, x, z );
				}
			}
		}
	}
}

void GameWarfogManager::redrawMap()
{
	UINT c=0x50000000;
	for(int i = 0; i< m_postnode.size(); i++)
	{
		m_pTexsuface->setPixel( &c,m_postnode[i].x,   m_postnode[i].y );
	}
	m_postnode.clear();
}

void GameWarfogManager::addMappoint(int x, int y)
{
	node temp;
	temp.x=x;temp.y=y;
	m_postnode.push_back(temp);
}
void GameWarfogManager::drawPoint( int x, int y, int xh, int yh)
{

	UINT c=0x00000000;
	m_pTexsuface->setPixel( &c, x+xh,   y+yh );
	addMappoint( x+xh,   y+yh );
	m_pTexsuface->setPixel( &c, yh+x,   xh+y );
	addMappoint( yh+x,   xh+y );
	m_pTexsuface->setPixel( &c, -xh+x,  yh+y );
	addMappoint( -xh+x,  yh+y );
	m_pTexsuface->setPixel( &c, -yh+x,  xh+y );
	addMappoint(  -yh+x,  xh+y );
	m_pTexsuface->setPixel( &c, -xh+x, -yh+y );
	addMappoint( -xh+x, -yh+y );
	m_pTexsuface->setPixel( &c, -yh+x, -xh+y );
	addMappoint(  -yh+x, -xh+y );
	m_pTexsuface->setPixel( &c, xh+x,  -yh+y );
	addMappoint( xh+x,  -yh+y );
	m_pTexsuface->setPixel( &c, yh+x,  -xh+y );
	addMappoint( yh+x,  -xh+y );
}

void GameWarfogManager::drawRound( int x, int y )
{
	int R = MAPRADIUS;
	while (R>0)
	{
		int x1 = R,y1 = 0,F = 0,n = R;
		while(n > 0)
		{
			drawPoint( x, y, x1, y1 );
			if (F <= 0)
			{
				F  += 2*y1+1;
				y1 += 1;
			}
			else
			{
				F  += 1-2*x1;
				x1 -= 1; 
			}
			n--;
		}
		if (x1 == y1)
			drawPoint( x, y, x1, y1 );
		R = R-1;
	}
}

void GameWarfogManager::updateShowMap( int x, int y )
{
	Pair Ptemp=GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getPointScreenCoord( x, y, "WorldMapFrameMapTexture", 10 );
	x = x * 1024 / 578;
	y = y * 1024 / 578;
	drawRound( x, y );
	for (int i = 0; i < m_mapNpcnode.size(); i++)
	{
		m_mapNpcnode[i]->DrawShow(false);
	}
	m_pTexdata->setHardwareTextureinvild();
}

void GameWarfogManager::update()
{
	if ( ActorManager::getSingleton().getMainPlayer() && ActorManager::getSingleton().getMainPlayer()->getCurMap() && m_wardistance > 0 )
	{
		if ( m_pCurTile == NULL )
		{
			init();
		}
		//战争迷雾的更新
		float x,z;//玩家的世界坐标
		x = ActorManager::getSingleton().getMainPlayer()->GetPosition().x / 10;
		z = ActorManager::getSingleton().getMainPlayer()->GetPosition().z / 10;
		float* temp = NULL;
		temp = SceneManager::getSingleton().m_referencepoint;
		temp[15] = 1;temp[0] = x;temp[1] = z;
		float ol[16];
		ol[15] = 0;
		GameTeamManager::getSingleton().getTeamMemberPostion(temp, ol);
		int p = temp[15];
		ActorManager::getSingleton().hideAllActors();
		redrawMap();
		for ( int i = 0; i < p; i++ )
		{
			updateBlock( temp[i*2], temp[i*2+1] );
			updateShowMap( temp[i*2]*10, temp[i*2+1]*10 );
			ActorManager::getSingleton().WarFogUpdate( m_wardistance, temp[i*2], temp[i*2+1] );
		}
		p = ol[15];
		for ( int i = 0; i < p; i++ )
		{
			updateShowMap( ol[i*2]*10, ol[i*2+1]*10 );
		}
		GameActor* tempTarget = ActorManager::getSingleton().FindActor(ActorManager::getSingleton().getMainPlayer()->GetTargetID());
		if ( tempTarget != NULL && !tempTarget->isShow())
		{
			ActorManager::getSingleton().getMainPlayer()->SetSelectTarget(0);
		}
		SceneManager::getSingleton().m_warfogparameter.setElement( m_wardistance, m_postchroma, m_outchroma );//将玩家坐标作为参考点传入shader
		SceneManager::getSingleton().m_warfogcolor.setElement( m_warcolor[0], m_warcolor[1], m_warcolor[2] );//将玩家坐标作为参考点传入shader
	}
}

void GameWarfogManager::reset()
{
	if ( m_wardistance > 0 )
	{
		init();
	}
}
void GameWarfogManager::init()
{
	m_pUiTexture->Show();
	m_pTexsuface = m_pTexdata->lockSurface(0, 0, false);
	m_pCurTile   = GamePlayManager::getSingleton().getCurMap()->getShowGameMap()->getGameScene()->getTerrainTile();
	int nums=GameManager::getSingleton().getGameMapShower()->getMapNpcCount(ActorManager::getSingleton().getMainPlayer()->getCurMap()->getMapId());
	nums = nums + GameManager::getSingleton().getGameMapShower()->getMapSceneAreaCount(ActorManager::getSingleton().getMainPlayer()->getCurMap()->getMapId());
	for (int i = 0; i < nums; i++)
	{
		std::string name = "WorldmapNpcPosBtn";
		char cg,cs;
		cg = '0' + 1;
		if ( i >= 9 )
		{
			cs = '0' + (i+1) / 10;
			name = name + cs;
		}
		name = name + cg;
		Frame *temp=(Frame *)g_pFrameMgr->FindLayoutFrame( name );
		m_mapNpcnode.push_back( temp );
	}
}

void GameWarfogManager::initTexture()
{
	m_pUiTexture	= (::Texture* )g_pFrameMgr->FindTexture( "WorldMapFrame_ShowMapBtn", "WorldMapFrameMapwarfogTexture" );
	m_pTexdata		= (TextureData*)UIRenderer::getSingleton().getScreentexture(m_pUiTexture->m_hTex)->GetTexture();
	m_pTexdata->setStatic(false);
}
bool GameWarfogManager::ismapbuttonshow(int x, int y)
{
	if ( ActorManager::getSingleton().getMainPlayer()->getCurMap() && m_wardistance > 0 )
	{
		x=x/10;
		y=y/10;
		GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getPointScreenCoord( x , y , "WorldMapFrameMapTexture", 10 );
		x = x * 1024 / 578;
		y = y * 1024 / 578;
		int x1 = ActorManager::getSingleton().getMainPlayer()->GetPosition().x / 10;
		int y1 = ActorManager::getSingleton().getMainPlayer()->GetPosition().z / 10;
		GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getPointScreenCoord( x1, y1, "WorldMapFrameMapTexture", 10 );
		x1 = x1 * 1024 / 578;
		y1 = y1 * 1024 / 578;
		if ((x-x1)*(x-x1)+(y-y1)*(y-y1) <= 1024)
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}