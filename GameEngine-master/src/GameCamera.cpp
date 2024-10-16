
#include "GameCamera.h"
#include "ShowGameMap.h"
#include "OgreCamera.h"
#include "OgreModelData.h"
#include "OgreModel.h"
#include "OgreResourceManager.h"
#include "OgreAnimationPlayer.h"
#include "OgreMatrix4.h"
#include "OgreSkeleton.h"
#include "OgreScreenTexture.h"
#include "OgreTexture.h"
#include "GameMap.h"

#include "OgreUIRenderer.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"

#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "OgreEntity.h"

#include "OgreBorderScene.h"
#include <Windows.h>
#include "OgreShadowmap.h"
#include "OgreRay.h"

#include "../UILib/ui_framemgr.h"

using namespace Ogre;

FollowCamera::FollowCamera() : m_pGroundMap(NULL), m_bPlayAnim(false), m_pAnimMod(NULL), m_bClamp(false)
{
	m_pCameraNode = new Camera;
	m_pCameraNode->setDepth(20.0f , 25000.0f);
	m_pCameraNode->setFov(30.0f);

	m_pCutActorCameraNode = new Camera;
	m_pCutActorCameraNode->setDepth(20.0f , 25000.0f);
	m_pCutActorCameraNode->setFov(30.0f);

	Reset(WorldPos(0, 0, 0));
	m_isSkid = true;
}

FollowCamera::FollowCamera(Ogre::Camera *camera) : m_pGroundMap(NULL), m_bPlayAnim(false), m_pAnimMod(NULL)
{
	m_pCameraNode = camera;
	m_pCutActorCameraNode = NULL;
	m_isSkid = true;
}

FollowCamera::~FollowCamera()
{
	if (m_pCutActorCameraNode != NULL)
	{
		OGRE_RELEASE( m_pCameraNode );
		OGRE_RELEASE( m_pCutActorCameraNode );
	}
	OGRE_RELEASE( m_pAnimMod );
}

void FollowCamera::Reset(const Ogre::WorldPos &target)
{
	m_Target = m_TargetFinal = target;
	m_lastSpeed = 0;
	m_lastTick = 0;
	m_lastDerTime = 0;
	m_isSkid = true;

	m_AngleLR = 0;
	m_AngleUD = 135.0f;
	m_MinFov = 45.0f;
	m_MaxFov = 45.0f;
	m_MaxDistMoveSpeed = 1200.0f;
	m_MinDistMoveSpeed = 1.0f;
	m_MoveTotalTime = 0.0f;
	m_CurMoveTime = 0.0f;

	m_MinDist = 400.0f;
	m_MaxDist = 1800.0f;
	m_MinAngleUD = 45.0f;
	m_MaxAngleUD = 60.0f;
	m_MidAngleUD = 65.0f;
	m_DistMoveSpeed = m_MaxDistMoveSpeed;
	m_TargetDist = m_CurDist = m_MaxDist/*(m_MaxDist + m_MinDist)*0.55f*/;
	setGround( NULL );
	UpdateCameraNode(0);
}

void FollowCamera::Update( unsigned int dtick )
{
	bool result = false;
	for (std::set<IFollowCameraObserver *, PtrLess>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		result = (*i)->process(this, dtick);
	}

	if (!result)
	{
		if( m_bPlayAnim )
		{
			m_TargetFinal = m_CameraCenter;
			m_TargetFinal.y -= 2 * Ogre::WorldPos::Flt2Fix(100.0f);
			m_Target = m_TargetFinal;
			updateCameraAnim(dtick);
		}else if( ActorManager::getSingleton().getMainPlayer() != NULL &&
			ActorManager::getSingleton().getMainPlayer()->getCurMap() != NULL )
		{
			GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
			Ogre::ShowActor* pShowActor = pActor->getShowActor();
			m_TargetFinal = ActorManager::getSingleton().getMainPlayer()->GetPosition();
			/*
			if( pActor->getActState().GetState() == GSACT_AUTOFLY )
			{
				m_TargetFinal = pActor->m_Rider.m_pShowActor->getCurEntity()->getPosition();
			}
			*/
			static bool bchange = false;
			MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			if ( pMainPlayer->getCurMap()->getMapId() == 30200 )  // 坦克地图
			{
				if ( !bchange )
				{
					m_CurDist = m_MaxDist = 3500.0f;
					m_MinDist = 1000.0f;
					m_MinAngleUD = 55.0f;
					bchange = true;
				}
			}
			else if( pMainPlayer->getCurMap()->getMapId() == 4404 )
			{
				if( !bchange )
				{
					m_CurDist = m_MaxDist = 2200.0f;
					m_MinDist = 400.0f;
					m_MinAngleUD = 45.0f;
					bchange = true;
				}
			}else{
				m_TargetFinal.y += Ogre::WorldPos::Flt2Fix(100.0f);
				if(m_bClamp)
					m_TargetFinal.y += Ogre::WorldPos::Flt2Fix(50.0f);
				if( bchange )
				{
					m_CurDist = m_MaxDist = 1800.0f;
					m_MinDist = 400.0f;
					m_MinAngleUD = 45.0f;
					bchange = false;
				}

			}
			
			

// 			const float accelerateTime = 0.2f;
// 			const unsigned int gentleTime = 1000;
// 			float during = TickToTime(dtick);
// 			Ogre::WorldPos dir = m_TargetFinal - m_Target;
// 			float length = dir.toVector3().length() * 10;
// 			if (length > 0)
// 			{
// 				float ratio = during / accelerateTime;
// 				if (ratio > 1)
// 				{
// 					ratio = 1;
// 				}
// 				float speed = length * ratio / during;
// 				if (speed > m_lastSpeed)
// 				{
// 					if (m_isSkid)
// 					{
// 						unsigned int now = timeGetTime();
// 						if (now >= m_lastDerTime + gentleTime)
// 						{
// 							m_lastDerTime = now;
// 							m_isSkid = false;
// 						}
// 						else
// 						{
// 							speed = m_lastSpeed;
// 						}
// 					}
// 					m_lastSpeed = min(m_lastSpeed + length / (accelerateTime * accelerateTime) * during, speed);
// 				}
// 				else
// 				{
// 					if (!m_isSkid)
// 					{
// 						m_lastDerTime = timeGetTime();
// 						m_isSkid = true;
// 					}
// 					m_lastSpeed = speed;
// 				}
// 				ratio = m_lastSpeed * during / length;
// 				if (m_lastTick && m_lastTick * 3 <= dtick)
// 				{
// 					ratio /= dtick / m_lastTick;
// 				}
// 				else
// 				{
// 					m_lastTick = dtick;
// 				}
// 				m_Target += dir * ratio;
// 			}

			if (true || pActor->getActState().GetState() == GSACT_AUTOFLY)
			{
				m_Target = m_TargetFinal;
			}
			else
			{
				float prop = 0;
				if (m_CurDist > 0)
				{
					prop = (m_CurDist - m_MinDist) / (m_MaxDist - m_MinDist);
				}
				Ogre::WorldPos dir = m_TargetFinal - m_Target;
				float ratio = 1;
				if (prop > 0)
				{
					const float maxDistance = 10000;
					float accelerateTime = 0.15f * prop;
					float lagTime = 0.5f * prop;
					float during = TickToTime(dtick);
					float length = dir.toVector3().length() * 10;
					if (m_lastSpeed * lagTime < length)
					{
						m_lastSpeed = min(m_lastSpeed + length / (accelerateTime * accelerateTime) * during, length / lagTime);
					}
					else
					{
						m_lastSpeed = min(m_lastSpeed, length / accelerateTime);
					}
					if (length > 0)
					{
						ratio = m_lastSpeed * during / length;
					}
					if (m_lastTick && m_lastTick * 2 <= dtick)
					{
						ratio /= dtick / m_lastTick;
						m_lastTick = (m_lastTick + dtick) / 2;
					}
					else
					{
						m_lastTick = dtick;
					}
					if (length > maxDistance)
					{
						ratio = 1;
					}
					else if (ratio > 1)
					{
						ratio = 1;
					}
				}
				m_Target += dir * ratio;
			}

			m_Target.y += ActorManager::getSingleton().getMainPlayer()->getOverHead();
			setGround( ActorManager::getSingleton().getMainPlayer()->getCurMap()->getShowGameMap() );
			UpdateCameraNode( dtick );
			m_Target.y -= ActorManager::getSingleton().getMainPlayer()->getOverHead();
			getGlobalPos() = m_Target;
		}
	}

	float fStrength = m_CurDist*0.0016f;

	if (fStrength >= 0.9f && fStrength <=  2.7f)
	{
		// 相机包边
		if (BorderGameScene::getSingletonPtr())
			BorderGameScene::getSingleton().setBorderStrength(fStrength);	
	}
}

void FollowCamera::updateCamera(const Ogre::WorldPos &target, unsigned int dtick)
{
	m_Target = m_TargetFinal = target;
	m_lastSpeed = 0;
	m_lastTick = 0;
	m_lastDerTime = 0;
	m_isSkid = true;

	UpdateCameraNode(dtick);
}

void FollowCamera::UpdateCameraNode(unsigned int dtick)
{
	if (dtick == 0)
	{
		return;
	}
	float dtime = TickToTime(dtick);
	m_CurMoveTime += dtime;
	if(m_CurMoveTime > m_MoveTotalTime) 
		m_CurMoveTime = m_MoveTotalTime;

	if(m_MoveTotalTime > 0)
		m_DistMoveSpeed = Lerp(m_MaxDistMoveSpeed, m_MinDistMoveSpeed, m_CurMoveTime/m_MoveTotalTime);
	else
		m_DistMoveSpeed = 0;

	if (m_CurMoveTime == m_MoveTotalTime)
		m_DistMoveSpeed = 0.0f;

	if(m_CurDist < m_TargetDist)
	{
		m_CurDist += m_DistMoveSpeed*dtime;
		if(m_CurDist > m_TargetDist) m_CurDist = m_TargetDist;
	}
	else
	{
		m_CurDist -= m_DistMoveSpeed*dtime;
		if(m_CurDist < m_TargetDist) m_CurDist = m_TargetDist;
	}

	float t = (m_CurDist-m_MinDist)/(m_MaxDist-m_MinDist);
	//float ud = m_MinAngleUD*t*t + 2.0f*m_MidAngleUD*t*(1-t) + m_MaxAngleUD*(1.0f-t)*(1.0f-t);
	float ud = m_MinAngleUD*t + m_MaxAngleUD*(1.0f-t);
	float fov = m_MaxFov*t + m_MinFov*(1.0f-t);
	
	Vector3 dir;
	if ( ActorManager::getSingleton().getGameOpt( GAME_OPT_CAMERA_3D ) )
	{
		dir.y = -Cos(m_AngleUD);
		dir.x = -Sin(m_AngleUD)*Sin(m_AngleLR);
		dir.z = -Sin(m_AngleUD)*Cos(m_AngleLR);
	} 
	else
	{
		ResetCameraUD();
		dir.y = Cos(ud);
		dir.x = -Sin(ud)*Sin(m_AngleLR);
		dir.z = -Sin(ud)*Cos(m_AngleLR);
	}

	WorldPos pos = m_Target;
	pos += dir*m_CurDist;

	if(m_pGroundMap)
	{
		WPOS_T gh = m_pGroundMap->getGroundPoint(pos.x, pos.z, NULL, NULL) + WorldPos::Flt2Fix(100.0f);
		//if(pos.y < gh) pos.y = gh;
		if( pos.y < gh )
		{
			WorldRay ray;
			float dist;
			ray.m_Origin = m_Target;
			ray.m_Dir = dir;
			m_pGroundMap->pickGround( ray, &dist );	
			if ( dist  < 10.f )
			{
				dist = m_CurDist;
			}
			WorldPos pickpoint = ray.m_Origin + ray.m_Dir*(abs(dist) > abs(m_CurDist) ? m_CurDist : dist);
			pos.y = pickpoint.y+ WorldPos::Flt2Fix(30.0f);
			pos.x = pickpoint.x;
			pos.z = pickpoint.z;
		}
	}

//	m_pCameraNode->setLookDirect(pos, -dir, Vector3(0,1.0f,0));
//	m_pCameraNode->setFov(fov);
//	m_pCameraNode->update(dtick);
	//pcamera->setLookDirect(pos, -dir, Vector3(0,1.0f,0));
	m_pCameraNode->setLookAt(pos, m_Target, Vector3(0,1.0f,0));
	m_pCameraNode->setFov(fov);
	m_pCameraNode->update(dtick);
	if (m_pCutActorCameraNode != NULL)
	{
		WorldPos cutpos = m_Target;
		float dist = m_CurDist + 400;
		if (dist < (m_MinDist + m_MaxDist) / 2)
		{
			dist = (m_MinDist + m_MaxDist) / 2;
		}
		cutpos += dir * dist;
		m_pCutActorCameraNode->setLookAt(cutpos, m_Target, Vector3(0,1.0f,0));
		m_pCutActorCameraNode->setFov(fov);
		m_pCutActorCameraNode->update(dtick);
	}
//	m_pCameraNode->setLookDirect(pos, -dir, Vector3(0,1.0f,0));
//	m_pCameraNode->setFov(fov);
//	m_pCameraNode->update(dtick);
	if( Shadowmap::getSingletonPtr() )
	{
		Shadowmap::getSingletonPtr()->m_fAccuracy = m_CurDist / m_MaxDist;
	}
}

void FollowCamera::Rotate(float angle)
{
	m_AngleLR += angle;
	if(m_AngleLR < 0) m_AngleLR += 360.0f;
	if(m_AngleLR >= 360.0f) m_AngleLR -= 360.0f;
}
void FollowCamera::SetAngle(float angle)
{
	m_AngleLR = angle;
	while(m_AngleLR < 0) m_AngleLR += 360.0f;
	while(m_AngleLR >= 360.0f) m_AngleLR -= 360.0f;
}
void FollowCamera::SetAngle(int angle)
{
	SetAngle(float(angle));
}

void FollowCamera::PullInOut(float dist)
{
	if(!m_bClamp)
		m_TargetDist = Ogre::Clamp(m_TargetDist+dist, m_MinDist, m_MaxDist);
	else
		m_TargetDist = Ogre::Clamp(m_TargetDist+dist, 100.0f, m_MaxDist);

	float d = Abs(m_TargetDist-m_CurDist);
	m_MoveTotalTime = d*2.0f/(m_MinDistMoveSpeed+m_MaxDistMoveSpeed);
	m_CurMoveTime = 0;
	m_DistMoveSpeed = m_MaxDistMoveSpeed;

}

void FollowCamera::ChangeToward( float angle )
{
	if(m_AngleUD >90.0f  )
	{
		angle /= 5;
	}
	m_AngleUD += angle;
	if(m_AngleUD < 20.0f) m_AngleUD = 20.0f;
	if(m_AngleUD > 160.0f) m_AngleUD = 160.0f;
}
void FollowCamera::ResetCameraUD()
{
	m_AngleUD = 135.0f;
}

// 更新镜头动画
void FollowCamera::updateCameraAnim(unsigned int dtick)
{
	m_pAnimMod->update(dtick);

	int boneid = m_pAnimMod->getSkeletonInstance()->findBoneID("Camera01");
	if(boneid >= 0)
	{
		Matrix4 tm = m_pAnimMod->getSkeletonInstance()->getBoneTM(boneid);
		tm *= m_pAnimMod->getWorldMatrix();
		m_CameraCenter = tm.getTranslate();

		Vector3 dir(0, -1.0, 0), up(0, 0, 1.0f);
		tm.transformNormal(dir, dir);
		tm.transformNormal(up, up);
		const Ogre::Vector3 &ts = tm.getTranslate();
		m_pCameraNode->setLookDirect(ts, dir, up);
		if (m_pCutActorCameraNode != NULL)
		{
			m_pCutActorCameraNode->setLookDirect(ts, dir, up);
		}
	}
	Shadowmap::getSingletonPtr()->m_fAccuracy = 2.0f;
	if( timeGetTime() - m_nStartTick >=  m_nAnimPassTime )
	{
		m_bPlayAnim = false;
	}
}

// 播放出场镜头动画
void FollowCamera::playCameraAnim( const char* szOmodPath )
{
	if( szOmodPath == NULL )
	{
		return;
	}
	OGRE_RELEASE( m_pAnimMod );

	Resource *pres = ResourceManager::getSingleton().blockLoad( szOmodPath );
	if( pres==NULL || !IS_KIND_OF(ModelData, pres) )
	{
		return;
	}

	m_pAnimMod  = new Model(static_cast<ModelData *>(pres));
	ModelData* pModelData =  m_pAnimMod->getModelData();
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
	m_pAnimMod->getAnimPlayer()->setAnimDelayInOut(0, 0);
	updateCameraAnim(0);
	m_pAnimMod->playAnim(0);
	m_nAnimPassTime	= m_pAnimMod->getSeqDuration(0);
	m_nStartTick	= timeGetTime();
	m_bPlayAnim = true;

	pres->release();
}

// 是否正在播放相机动画
bool FollowCamera::isPlayCameraAnim()
{
	return m_bPlayAnim;
}

// 获取相机中心点
Ogre::WorldPos FollowCamera::getCameraCenter()
{
	return m_Target;
}

// 中断相机动画
void FollowCamera::stopCameraAnim()
{
	OGRE_RELEASE( m_pAnimMod );
	m_bPlayAnim = false;
	updateCamera(ActorManager::getSingleton().getMainPlayer()->GetPosition(), 0);
}

void FollowCamera::insertObserver(IFollowCameraObserver *ob)
{
	m_lstObserver.insert(ob);
}

