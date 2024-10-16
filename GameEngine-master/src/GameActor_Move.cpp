#define _USE_MATH_DEFINES
#include <math.h>
#include "GameActor_Move.h"
#include "GameMap.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "NetMessage.h"
#include "ServerTimer.h"
#include "OgreScriptLuaVM.h"
#include "OgreResourceManager.h"
#include "OgreAnimationPlayer.h"
#include "OgreMatrix4.h"
#include "OgreSkeleton.h"
#include <Windows.h>
#include "OgreLog.h"
#include "ogrebloom.h"

using namespace Ogre;

const float G = 98000;

SpecialMove::SpecialMove(void) : m_nMoveType(MOVETYPE_NOMAL), m_nFlyPointIndex(0), m_pFlyAnimMod(NULL), m_bInitCircleInfo(false),
									m_nStartJumpTime(0),m_nCurAnimIndex(0),m_bHideUI(false)
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
		mdata.Walk.Total			= (char)m_vectFlyPos.size();
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
			nFlyPointIndex = (int)m_vectFlyPos.size() -1;
		}
	}
	
	pActor->ChangeActState(GSACT_AUTOFLY);
	pActor->m_Rider.setRideOwner( pActor->GetID() );
	pActor->onHorse( 110000 );
	m_nMoveType = MOVETYPE_FLY;

	m_curSeg = 0;
}

// 根据动画路线飞行 
void SpecialMove::setAnimFly( GameActor* pActor, int indexId )
{
	char buf[256];
	if( indexId > FLY_ANIM_PATH_INDEX )
	{
		m_nCurAnimIndex = 0;
		sprintf( buf, "addAnimFlyPath(%d,%d)", pActor->GetID(), indexId );
		ActorManager::getSingleton().getScriptVM()->callString(buf);
		if( !m_vectorAnimPath.empty() )
		{
			sprintf( buf, "addEndPoint(%d,%d)", pActor->GetID(), m_vectorAnimPath[m_nCurAnimIndex++] );
			ActorManager::getSingleton().getScriptVM()->callString(buf);
		}
	}else{
		sprintf( buf, "addEndPoint(%d,%d)", pActor->GetID(), indexId );
		ActorManager::getSingleton().getScriptVM()->callString(buf);
	}
}

void SpecialMove::playAnimFly( int nOwnerId, const char* szAnimOmod, int modelId, int x, int y, int npcId, 
							  int delayTick, bool bHideUI )
{
	OGRE_RELEASE(m_pFlyAnimMod);

	Resource *pres = ResourceManager::getSingleton().blockLoad( szAnimOmod );
	GameActor*pActor = ActorManager::getSingleton().FindActor( nOwnerId );
	if( pres==NULL || !IS_KIND_OF(ModelData, pres) )
	{
		if( pActor->getType() == GAT_MAINPLAYER )
		{
			tagCSPkgBody msgbody;
			msgbody.AutoFlyClt.AutoFlyEnd = 1;
			msgbody.AutoFlyClt.EndPos.X = pActor->GetPosition().x ; 
			msgbody.AutoFlyClt.EndPos.Y = pActor->GetPosition().z ;
			SendGameServerMsg( AUTO_FLY_CLT, msgbody );	
		}
		return;
	}

	m_pFlyAnimMod  = new Model(static_cast<ModelData *>(pres));
	ModelData* pModelData =  m_pFlyAnimMod->getModelData();
	for (size_t i = 0 ; i < pModelData->getNumAnim();i++ )
	{
		if(pModelData->getAnimation(i)->hasSequence(0))
		{
			AnimationData* panimdata = pModelData->getAnimation(i);

			int seq_index = panimdata->getSequenceIndex(0);
			SequenceDesc *pseq = panimdata->getSequence(seq_index);
			pseq->loopmode = ANIM_MODE_ONCE_STOP;
		}
	}

	m_pFlyAnimMod->getAnimPlayer()->setAnimDelayInOut(0, 0);
	m_pFlyAnimMod->playAnim(0);
	m_nAnimPassTime	= m_pFlyAnimMod->getSeqDuration(0);
	
	pActor->ChangeActState( GSACT_AUTOFLY );
	pActor->m_Rider.setRideOwner( pActor->GetID() );
	m_nDelayBindTick = delayTick;
	if( delayTick == 0 )
	{
		pActor->onHorse( modelId );
	}else{
		pActor->onHorse( modelId,false );
	}
	m_nMoveType		= MOVETYPE_ANIMFLY;
	m_nStartTick	= timeGetTime();
	m_EndPos.x		= x;
	m_EndPos.y		= y;
	m_nNpcId		= npcId;
	m_nStartJumpTime = 0;
	m_uLastNotifyTime = timeGetTime();
	m_bHideUI = bHideUI;
	pres->release();
}

void SpecialMove::addFlyPath( int x, int y, int z )
{
	m_vectFlyPos.push_back( GameXYZ(x,y,z) );
}

void SpecialMove::doBack( GameActor* pActor, int mapid, int x, int z, int BackTime, int type, int nHeight, int speed )
{
	m_nMoveType = MOVETYPE_BACK;
	m_mapid		= mapid;
	m_InitPos = pActor->GetPosition();
	m_BackPos.x = x*10;
	m_BackPos.z = z*10;
	int y;
	bool b = pActor->getCurMap()->getTerrainH(x*10, z*10, y);
	assert(b);

	m_BackPos.y = y;
	m_uPreBackTime = m_uBackStartTime = timeGetTime();

	Ogre::Vector3	dir		= m_BackPos.toVector3() - m_InitPos.toVector3();
	Ogre::Vector3	dir2d	= dir;
	dir2d.y = 0;
	float			len		= dir.length();
	float			len2d	= dir2d.length();
	m_nMaxHeight			= nHeight; 
	m_BackType				= type;
	if (speed == 0)
	{
		m_BackTime = BackTime;
	}
	else
	{
		m_BackTime = Max(BackTime, int(len2d / speed * 1000));
	}
	m_BackAngle				= FaceAngleBetween( m_BackPos.x, m_BackPos.z, m_InitPos.x, m_InitPos.z );
	if (m_BackTime == 0)
	{
		m_nBackSpeed = INT_MAX;
	}
	else
	{
		m_nBackSpeed = (int)(len2d / m_BackTime * 1000);
	}

	if (m_BackType == BACK_PARABOLA)
	{
		if (m_BackPos.y - m_InitPos.y > nHeight)
		{
			nHeight += m_BackPos.y - m_InitPos.y;
		}
		m_paramA = (1 + sqrt(1 - (m_BackPos.y - m_InitPos.y) * 1.0 / nHeight)) * 2 * nHeight / m_BackTime;
		m_paramB = -m_paramA * m_paramA / (4 * nHeight);

		m_nMaxHeight = m_InitPos.y;
	}

	m_bInitCircleInfo		= false;
}

void SpecialMove::update( GameActor* pActor, unsigned int dtick )
{
	if( m_nMoveType == MOVETYPE_BACK )
	{
		if (m_mapid != pActor->getCurMap()->getMapId())
		{
			m_nMoveType = MOVETYPE_NOMAL;
		}
		else
		{
			// 直线飞行
			if( m_BackType == BACK_LINE )
			{
				int nCurTime = timeGetTime();
				Ogre::Vector3	dir		= m_BackPos.toVector3() - m_InitPos.toVector3();
				Ogre::Vector3	dir2d	= dir;
				dir2d.y = 0;
				float			len		= dir.length();
				float			len2d	= dir2d.length();

				Ogre::Normalize(dir);

				unsigned int passTime = nCurTime - m_uBackStartTime;
				if( passTime >= (uint)m_BackTime )
				{
					passTime = m_BackTime;
				}
				Ogre::WorldPos	currpos = m_InitPos;
				if (len2d == 0 || passTime == m_BackTime)
				{
					currpos = m_BackPos;
					m_nMoveType = MOVETYPE_NOMAL;
				}
				else
				{
					currpos += dir * (passTime / 1000.0f * m_nBackSpeed * len / len2d);
				}

				pActor->SetPosition( currpos.x/10, currpos.z/10 );
			}
			else if( m_BackType == BACK_SIN || m_BackType == BACK_CURPOST )
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

				uint passTime = timeGetTime() - m_uBackStartTime;

				if( passTime >= (uint)m_BackTime )
				{
					passTime = m_BackTime;
				}

				// sin形状飞行
				float angle  = ( (float)passTime/m_BackTime ) * 180;
				int   height = (int)(m_nMaxHeight * Sin( angle ));
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
			else if (m_BackType == BACK_CIRCLE)
			{
				// 绕A点旋转，落B点
				if (!m_bInitCircleInfo)
				{
					LOG_WARNING("circle move not be initialized");
				}
				else
				{
					int during = timeGetTime() - m_uBackStartTime;
					if (during < m_CircleInfo.time)
					{
						m_uPreBackTime = timeGetTime();
						float radius	= (m_CircleInfo.endRadius - m_CircleInfo.startRadius) /
							m_CircleInfo.time * during + m_CircleInfo.startRadius;
						float angle		= m_CircleInfo.angle / m_CircleInfo.time * during + m_CircleInfo.startAngle;
						int   x			= (int)((m_CircleInfo.x + radius * cos(angle)) / 10);
						int   y			= (int)((m_CircleInfo.y + radius * sin(angle)) / 10);
						int   height	= m_nMaxHeight * during / m_CircleInfo.time;

						pActor->SetPosition( x, y, height );
					}
					else
					{
						float during	= (timeGetTime() - m_uPreBackTime) / 1000.0f;
						int height	= (int)(m_nMaxHeight - during * during * G / 2);

						if (height  > 0)
						{
							pActor->SetPosition( m_BackPos.x / 10, m_BackPos.z / 10, height );
						}
						else
						{
							pActor->SetPosition( m_BackPos.x / 10, m_BackPos.z / 10);
							m_nMoveType = MOVETYPE_NOMAL;
						}
					}
				}
			}
			else if (m_BackType == BACK_PARABOLA)
			{
				int nCurTime = timeGetTime();
				unsigned int passTime = nCurTime - m_uBackStartTime;
				if( passTime >= (uint)m_BackTime )
				{
					passTime = m_BackTime;
				}

				if( passTime == m_BackTime )
				{
					m_nMoveType = MOVETYPE_NOMAL;
					pActor->SetPosition( m_BackPos.x/10, m_BackPos.z/10 );
				}
				else
				{
					Ogre::WorldPos	currpos		= pActor->GetPosition();
					Ogre::Vector3	vcurrpos	= currpos.toVector3();	
					Ogre::Vector3	dir   = m_BackPos.toVector3() - currpos.toVector3();

					float len = dir.length();
					Ogre::Normalize(dir);

					float framemove = ( nCurTime - m_uPreBackTime )*( m_nBackSpeed/1000.0f );
					m_uPreBackTime = nCurTime;
					vcurrpos += dir * (len > framemove ? framemove : len);
					currpos = vcurrpos;

					pActor->getCurMap()->getTerrainH(currpos.x, currpos.z, currpos.y);
					currpos.y = m_paramB * passTime * passTime + m_paramA * passTime + m_nMaxHeight - currpos.y;

					pActor->SetPosition( currpos.x/10, currpos.z/10, currpos.y );
				}
			}
		}
	}

	if( m_nMoveType == MOVETYPE_ANIMFLY )
	{
		if( m_pFlyAnimMod == NULL )
		{
			return;
		}

		m_pFlyAnimMod->update(dtick);

		int boneid = m_pFlyAnimMod->getSkeletonInstance()->findBoneID("Player01");
		if( boneid >= 0 )
		{
			Matrix4 tm = m_pFlyAnimMod->getSkeletonInstance()->getBoneTM(boneid);
			Ogre::Vector3 pos = tm.getTranslate();
			if( m_nDelayBindTick == 0 )
			{
				pActor->SetPositionTM( (int)(pos.x*10), (int)(pos.y*10), (int)(pos.z*10) );
			}else{
				if( int(timeGetTime() - m_nStartTick) < m_nDelayBindTick || timeGetTime() - m_nStartJumpTime < 180 )
				{
					if( pActor->m_Rider.m_pShowActor != NULL )
					{
						pActor->m_Rider.m_pShowActor->setPosition( int(pos.x * 10), int(pos.y * 10), int(pos.z * 10) );
					}
				}else
				{
					if( m_nStartJumpTime == 0 )
					{
						pActor->getShowActor()->stopCurAction();
						pActor->playAction( 10133, NULL, 0, 0 );
						pActor->m_Rider.m_pShowActor->setPosition( int(pos.x * 10), int(pos.y * 10), int(pos.z * 10) );
						m_nStartJumpTime = timeGetTime();
					}else if( timeGetTime() - m_nStartJumpTime >= 180 )
					{
						pActor->m_Rider.bindOwner();
						pActor->SetPositionTM( (int)(pos.x*10), (int)(pos.y*10), (int)(pos.z*10) );
					}
				}
			}
			Quaternion rot = m_pFlyAnimMod->getSkeletonInstance()->getBoneRotation(boneid);
			//Quaternion rot;
			//rot.setMatrix( tm );
			if( pActor->m_Rider.m_pShowActor != NULL )
			{
				pActor->m_Rider.m_pShowActor->getCurEntity()->setRotation(rot);
				if(m_nNpcId <= 0 && pActor->getType() == GAT_MAINPLAYER )
				{
					if( timeGetTime() - m_uLastNotifyTime > 500 )
					{
						if( pActor->getCurMap() != NULL )
						{
							GameXYZ pos = pActor->GetPosition();
							Point2D end( pos.x/10,pos.z/10 );
							if( pActor->getCurMap()->getMaskMap()->findPointWalkable( end, 1 ) )
							{
								m_uLastNotifyTime = timeGetTime();
								tagCSPkgBody msgbody;
								msgbody.AutoFlyClt.AutoFlyEnd = 0;
								msgbody.AutoFlyClt.EndPos.X = end.x ; 
								msgbody.AutoFlyClt.EndPos.Y = end.y ;
								SendGameServerMsg( AUTO_FLY_CLT, msgbody );
							}
						}
					}
				}
			}
		}

		if( ( timeGetTime() - m_nStartTick >=  m_nAnimPassTime ) 
			&& pActor == ActorManager::getSingleton().getMainPlayer()  )
		{
			OGRE_RELEASE( m_pFlyAnimMod );
			if( !m_vectorAnimPath.empty() ) 
			{
				if(m_nCurAnimIndex < m_vectorAnimPath.size() )
				{
					char buf[256];
					sprintf( buf, "continueAnimFly(%d,%d)", pActor->GetID(), m_vectorAnimPath[m_nCurAnimIndex++] );
					ActorManager::getSingleton().getScriptVM()->callString(buf);
					return;
				}else{
					m_vectorAnimPath.clear();
					m_nCurAnimIndex = 0;
				}

			}
			if (m_nNpcId > 0 )
			{
				tagCSPkgBody msgbody;
				msgbody.DlgC.Type		= 1;
				msgbody.DlgC.NpcID		= ActorManager::getSingleton().getActorIdByResId( m_nNpcId );
				msgbody.DlgC.Dlg.Input[0] = 0;
				msgbody.DlgC.RolePos.X	= pActor->GetPosition().x/10;
				msgbody.DlgC.RolePos.Y	= pActor->GetPosition().z/10;
				ServerTimer::getSingleton().getServerTime(msgbody.DlgC.SvrTime.BenchTimeSec, msgbody.DlgC.SvrTime.BenchTimeUSec);
				SendGameServerMsg( DLG_CLT, msgbody );
				m_nNpcId = -1;
			}else if (m_nMoveType != MOVETYPE_NOMAL && m_nNpcId == 0 )
			{
				pActor->ChangeActState( GSACT_STAND );
				m_nMoveType = MOVETYPE_NOMAL;
				if (m_EndPos.x !=0 && m_EndPos.y != 0 )
				{
					pActor->SetPosition(m_EndPos.x,m_EndPos.y);
					if( ((MainGamePlayer*)pActor)->m_bChangeSkillShortCut )
					{
						pActor->m_Equip.looseTmpArm();
					}
					tagCSPkgBody msgbody;
					msgbody.AutoFlyClt.AutoFlyEnd = 1;
					msgbody.AutoFlyClt.EndPos.X = m_EndPos.x; 
					msgbody.AutoFlyClt.EndPos.Y = m_EndPos.y;
					SendGameServerMsg( AUTO_FLY_CLT, msgbody );	
				}else{
					int x,z;
					pActor->m_pMapMover->GetCurPos(x,z);
					pActor->onStop(x,z);
				}
			}
		}
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
		if( nBackIndex >= (int)m_vectFlyPos.size() )
		{
			nBackIndex = (int)m_vectFlyPos.size() - 1;
		}

		int nLength			= (int)m_vectFlyPos[nFrontIndex].subtract(m_vectFlyPos[nBackIndex]).length();
		int nFrontLength	= (int)m_vectFlyPos[nFrontIndex].subtract(pos).length();
		double pre			= (double)nFrontLength/nLength;
		int nHeight			= (int)((double)( m_vectFlyPos[nBackIndex].z - m_vectFlyPos[nFrontIndex].z ) * pre + m_vectFlyPos[nFrontIndex].z);
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
	for( int i = 0; i < (int)m_vectFlyPos.size()-1; i++ )
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

void SpecialMove::setCircleInfo(GameActor* pActor, int x, int y, bool inverted /*= true*/)
{
	m_bInitCircleInfo		= true;
	m_CircleInfo.x			= x;
	m_CircleInfo.y			= y;

	double floorTime = sqrt(2 * m_nMaxHeight / G) * 1000;
	int rate = int(floorTime / m_BackTime);
	if (rate >= 1)
	{
		floorTime /= rate * 2;
		m_nMaxHeight /= rate * rate * 2 * 2;
	}
	m_CircleInfo.time = (int)(m_BackTime - floorTime);
	assert(m_CircleInfo.time > 0);

	int times = (int)ceil(m_CircleInfo.time / 500.0);
	

	GameXYZ selfPos = pActor->GetPosition();
	m_CircleInfo.startRadius	= sqrt((selfPos.x - x) * (selfPos.x - x) + (selfPos.z - y) * (selfPos.z - y) + 0.0f);
	m_CircleInfo.endRadius		= sqrt((m_BackPos.x - x) * (m_BackPos.x - x) + (m_BackPos.z - y) * (m_BackPos.z - y) + 0.0f);

	m_CircleInfo.startAngle = atan2(float(selfPos.z - y), (float)(selfPos.x - x));
	float diffangle = atan2(float(m_BackPos.z - y), (float)(m_BackPos.x - x)) - m_CircleInfo.startAngle;
	if (diffangle < 0)
	{
		diffangle += (float)M_PI;
	}

	if (inverted)
	{
		m_CircleInfo.angle = times * (float)M_PI + diffangle;
	}
	else
	{
		m_CircleInfo.angle = -times * (float)M_PI - ((float)M_PI - diffangle);
	}
}

void SpecialMove::InsertAnimPath(int animId)
{
	m_vectorAnimPath.push_back(animId);
}

void SpecialMove::continueAnimFly( int nOwnerId, const char* szAnimOmod,int modelId, int x, int y,int npcId )
{
	OGRE_RELEASE(m_pFlyAnimMod);
	Resource *pres = ResourceManager::getSingleton().blockLoad( szAnimOmod );
	if( pres==NULL || !IS_KIND_OF(ModelData, pres) )
	{
		return;
	}
	m_pFlyAnimMod  = new Model(static_cast<ModelData *>(pres));
	ModelData* pModelData =  m_pFlyAnimMod->getModelData();
	for (size_t i = 0 ; i < pModelData->getNumAnim();i++ )
	{
		if(pModelData->getAnimation(i)->hasSequence(0))
		{
			AnimationData* panimdata = pModelData->getAnimation(i);

			int seq_index = panimdata->getSequenceIndex(0);
			SequenceDesc *pseq = panimdata->getSequence(seq_index);
			pseq->loopmode = ANIM_MODE_ONCE_STOP;
		}
	}
	m_pFlyAnimMod->getAnimPlayer()->setAnimDelayInOut(0, 0);
	m_pFlyAnimMod->playAnim(0);
	m_nAnimPassTime	= m_pFlyAnimMod->getSeqDuration(0);
	m_nMoveType		= MOVETYPE_ANIMFLY;
	m_nStartTick	= timeGetTime();
	m_EndPos.x		= x;
	m_EndPos.y		= y;
	m_nNpcId		= npcId;
	m_nStartJumpTime = 0;

	pres->release();
}

bool SpecialMove::checkCancelFirstAnimFly()
{
	if( m_pFlyAnimMod == NULL || m_pFlyAnimMod->getModelData() == NULL
		|| m_pFlyAnimMod->getModelData()->getResPath() != "db\\fly\\a.omod" )
	{
		return false;
	}
	m_nAnimPassTime = 0;
	return true;
}

bool SpecialMove::canShowUI()
{
	if( m_pFlyAnimMod == NULL || m_pFlyAnimMod->getModelData() == NULL )
	{
		return true;
	}

	return !m_bHideUI;
}