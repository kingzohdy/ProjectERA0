#include "SpecialMove.h"
#include "GameMap.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "NetMessage.h"
#include "ServerTimer.h"
#include "OgreScriptLuaVM.h"
#include <Windows.h>

using namespace Ogre;

SpecialMove::SpecialMove(void) : m_nMoveType(MOVETYPE_NOMAL),m_nFlyPointIndex(0)
{
}

SpecialMove::~SpecialMove(void)
{
}

void SpecialMove::setType(int type)
{
	m_nMoveType = type;
}

int SpecialMove::getType()
{
	return m_nMoveType;
}

void SpecialMove::setAutoFly( GameActor* pActor, int indexId, bool bFristFly )
{
	m_vectFlyPos.clear();
	char buf[256];
	sprintf( buf, "addFlyPoint(%d,%d)", pActor->GetID(), indexId );
	ActorManager::getSingleton().getScriptVM()->callString(buf);
	if( m_vectFlyPos.empty() )
	{
		return;
	}
	
	if( pActor->m_Rider.isOnHorse() )
	{
		pActor->m_Rider.downHorse();
	}

	if( pActor->getType() == GAT_MAINPLAYER )
	{
		MainGamePlayer* pMainPlayer = (MainGamePlayer*)pActor;
		
		GameMaskPath path;
		tagCSPkgBody msgbody;
		msgbody.MoveReq.MoveType	= CS_MOVE_WALK;
		msgbody.MoveReq.MoveSeq		= pMainPlayer->m_CurMoveSeq++;
		msgbody.MoveReq.x			= pMainPlayer->GetPosition().x/10;
		msgbody.MoveReq.y			= pMainPlayer->GetPosition().z/10;
		CSMOVEDATA &mdata			= msgbody.MoveReq.Data;
		mdata.Walk.Total			= m_vectFlyPos.size();
		int nFlyPointIndex			= 1; 

		for(size_t i = 1; i < m_vectFlyPos.size()+1; i++)
		{
			mdata.Walk.Points[i-1].x = m_vectFlyPos[i-1].x;
			mdata.Walk.Points[i-1].y = m_vectFlyPos[i-1].y;
			path.addPathPoint( m_vectFlyPos[i-1].x, m_vectFlyPos[i-1].y);
		}
		
		pMainPlayer->m_pMapMover->StartOnPath(&path);
		ServerTimer::getSingleton().getServerTime(msgbody.MoveReq.SvrTime.BenchTimeSec, msgbody.MoveReq.SvrTime.BenchTimeUSec);

		SendGameServerMsg(MOVE_CLT, msgbody);
		
		pMainPlayer->m_LastCmdMoveTick = timeGetTime();
	}else{
		int nFlyPointIndex	= 1; 
		if( bFristFly )
		{
			m_nFlyPointIndex  = 1;
		}else{
			// 中途寻找飞行点
			nFlyPointIndex		= findPathIndex( pActor->GetPosition().x/10, pActor->GetPosition().z/10 );
			m_nFlyPointIndex	= nFlyPointIndex;
			assert(nFlyPointIndex != -1);
		}
	
		if( nFlyPointIndex == -1 )
		{
			// TODO::这边不可能出现
			nFlyPointIndex = m_vectFlyPos.size() -1;
		}
	}
	
	pActor->ChangeActState(GSACT_AUTOFLY);	
	m_nMoveType = MOVETYPE_FLY;

	m_curSeg = 0;
}

void SpecialMove::addFlyPath( int x, int y, int z )
{
	m_vectFlyPos.push_back( GameXYZ(x,y,z) );
}

void SpecialMove::doBack( GameActor* pActor, int x, int z, int BackTime, int type, int nHeight, int speed )
{
	m_nMoveType = MOVETYPE_BACK;
	m_BackPos.x = x*10;
	m_BackPos.z = z*10;
	int y;
	bool b = pActor->getCurMap()->getTerrainH(x*10, z*10, y);
	assert(b);

	m_BackPos.y = y;
	m_uPreBackTime = m_uBackStartTime = timeGetTime();

	Ogre::WorldPos	currpos	= pActor->getShowActor()->getCurEntity()->getPosition();
	Ogre::Vector3	dir		= m_BackPos.toVector3() - currpos.toVector3();
	m_nMaxHeight			= nHeight; 
	m_BackType				= type;
	m_BackTime				= BackTime;
	m_BackAngle				= FaceAngleBetween( m_BackPos.x, m_BackPos.z, currpos.x, currpos.z );
	m_nBackSpeed			= speed;
}

void SpecialMove::update( GameActor* pActor, unsigned int dtick )
{
	if( m_nMoveType == MOVETYPE_BACK )
	{
		// 直线飞行
		if( m_BackType == BACK_LINE )
		{
			Ogre::WorldPos	currpos		= pActor->getShowActor()->getCurEntity()->getPosition();
			Ogre::Vector3	vcurrpos	= currpos.toVector3();	
			Ogre::Vector3	dir   = m_BackPos.toVector3() - currpos.toVector3();

			float len = dir.length();
			Ogre::Normalize(dir);

			unsigned int passTime = timeGetTime() - m_uBackStartTime;
			if( passTime >= m_BackTime )
			{
				passTime = m_BackTime;
			}

			float framemove = ( timeGetTime() - m_uPreBackTime )*( (float)m_nBackSpeed/1000 );
			m_uPreBackTime = timeGetTime();
			vcurrpos += dir * (len > framemove ? framemove : len);
			currpos = vcurrpos;
			pActor->SetPosition( currpos.x/10, currpos.z/10 );

			if(len < 10 )
			{
				if( m_BackTime == 0 )
				{
					m_nMoveType = MOVETYPE_NOMAL;
					pActor->SetPosition( m_BackPos.x/10, m_BackPos.z/10 );
				}else{
					if( passTime == m_BackTime )
					{
						m_nMoveType = MOVETYPE_NOMAL;
						pActor->SetPosition( m_BackPos.x/10, m_BackPos.z/10 );
					}
				}
				pActor->SetPosition( m_BackPos.x/10, m_BackPos.z/10 );
			}
		}

		if( m_BackType == BACK_SIN || m_BackType == BACK_CURPOST )
		{
			// 抛物线飞行
			Ogre::WorldPos	currpos		= pActor->getShowActor()->getCurEntity()->getPosition();
			Ogre::Vector3	vcurrpos	= currpos.toVector3();	
			Ogre::Vector3	dir		    = m_BackPos.toVector3() - currpos.toVector3();

			float len  = dir.length();
			Ogre::Normalize(dir);

			float framemove = ( timeGetTime() - m_uPreBackTime ) * ( (float)m_nBackSpeed/1000 );
			m_uPreBackTime = timeGetTime();
			vcurrpos += dir * (len > framemove ? framemove : len);
			currpos = vcurrpos;

			unsigned int passTime = timeGetTime() - m_uBackStartTime;

			if( passTime >= m_BackTime )
			{
				passTime = m_BackTime;
			}

			// sin形状飞行
			float angle  = ( (float)passTime/m_BackTime ) * 180;
			int   height = m_nMaxHeight * Sin( angle );
			pActor->SetPosition( currpos.x/10, currpos.z/10, height );

			if( len < 10 && m_BackType == BACK_SIN )
			{
				m_nMoveType = MOVETYPE_NOMAL;
				pActor->SetPosition( m_BackPos.x/10, m_BackPos.z/10 );
			}else if( m_BackType == BACK_CURPOST ){
				if( passTime == m_BackTime )
				{
					m_nMoveType = MOVETYPE_NOMAL;
					pActor->SetPosition( m_BackPos.x/10, m_BackPos.z/10 );
				}
			}
		}
		// 绕A点旋转，落B点
	}
}



// 寻找起飞点坐标
int SpecialMove::getPointHeight( GameActor* pActor, GameXYZ& pos )
{
	if( pActor->m_pMapMover->m_CurSeg != -1 )
	{
		int offIndex = 0;
		if( m_nFlyPointIndex >= 1 )
		{
			offIndex = m_nFlyPointIndex - 1;
		}
		int nFrontIndex = pActor->m_pMapMover->m_CurSeg + offIndex;	
		int nBackIndex	= pActor->m_pMapMover->m_CurSeg + 1 + offIndex;
		if( nBackIndex >= m_vectFlyPos.size() )
		{
			nBackIndex = m_vectFlyPos.size() - 1;
		}

		int nLength			= m_vectFlyPos[nFrontIndex].subtract(m_vectFlyPos[nBackIndex]).length();
		int nFrontLength	= m_vectFlyPos[nFrontIndex].subtract(pos).length();
		double pre			= (double)nFrontLength/nLength;
		int nHeight			= (double)( m_vectFlyPos[nBackIndex].z - m_vectFlyPos[nFrontIndex].z ) * pre + m_vectFlyPos[nFrontIndex].z;
		char buf[256];
		sprintf(buf, "当前节点%d,当前高度:%d, 当前per:%f, 前一节点高:%d, 后一节点高:%d\n", pActor->m_pMapMover->m_CurSeg, nHeight,
			pre, m_vectFlyPos[nFrontIndex].z, m_vectFlyPos[nBackIndex].z );
		OutputDebugString(buf);
		
		if( m_curSeg != pActor->m_pMapMover->m_CurSeg ) 
		{
			m_curSeg = pActor->m_pMapMover->m_CurSeg;
			pActor->m_Rider.horsePlayAct( 10124, NULL, 0, 0 );
		}
		/*int nAngle = FaceAngleBetween(pos.x, pos.y, m_vectFlyPos[nBackIndex].x, m_vectFlyPos[nBackIndex].y);
		static int i = 0;
		if( nAngle != 0 && i <= 20 )
		{
			i++;
			nAngle = nAngle + i * nAngle / 20;
		}else{
			i = 0;
		}
		pActor->getShowActor()->getCurEntity()->setRotation( nAngle - 90.0f, 0, 0 );*/
		return nHeight;
	}
	return 1;
}

// 寻找中间起点
int SpecialMove::findPathIndex( int x, int y )
{
	for( int i = 0; i < m_vectFlyPos.size()-1; i++ )
	{
		if( !IsPointInSegment( x, y, m_vectFlyPos[i].x, m_vectFlyPos[i].y, m_vectFlyPos[i+1].x, m_vectFlyPos[i+1].y ) )
		{
			continue;
		}
		return i+1;
	}
	return -1;
}

bool SpecialMove::IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 )
{
	if( x1 <= x2 )
	{
		if( x<x1 || x>x2 ) return false;
	}
	else if( x<x2 || x>x1 ) return false;

	if( y1 <= y2 )
	{
		if( y<y1 || y>y2 ) return false;
	}
	else if( y<y2 || y>y1 ) return false;

	int r = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
	if( r == 0 ) return true;

	return true;
}	