
#ifndef __GameCamera_H__
#define __GameCamera_H__

#include "OgreWorldPos.h"
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include <set>

class FollowCamera;


class IFollowCameraObserver
{
public:
	virtual bool process(FollowCamera *, unsigned int) = NULL;
};

class FollowCamera : public Ogre::Singleton<FollowCamera>
{
public:
	FollowCamera();
	FollowCamera(Ogre::Camera *camera);
	~FollowCamera();

	void Reset(const Ogre::WorldPos &target);
	void Update( unsigned int dtick);
	void updateCamera(const Ogre::WorldPos &target, unsigned int dtick);

	void Rotate(float angle);
	void SetAngle(float angle);
	void SetAngle(int nAngle);
	void PullInOut(float dist);
	void ChangeToward( float angle );
	void ResetCameraUD();
	float getAngleLR() 
	{
		return m_AngleLR; 
	}

	Ogre::Camera *GetCameraNode()
	{
		return m_pCameraNode; 
	}

	Ogre::Camera *GetCutActorCameraNode()
	{
		return m_pCutActorCameraNode != NULL ? m_pCutActorCameraNode : m_pCameraNode;
	}

	void setGround(Ogre::ShowGameMap *pmap)//�������������Ҫ�����ر�����
	{
		m_pGroundMap = pmap;
	}
	
	void setClamp(bool b) 
	{
		m_bClamp = b;
	}
	// ���¾�ͷ����
	void updateCameraAnim(unsigned int dtick);
	// ���ų�����ͷ����
	void playCameraAnim( const char* szOmodPath );
	// �Ƿ����ڲ����������
	bool isPlayCameraAnim();
	// ��ȡ������ĵ�
	Ogre::WorldPos getCameraCenter();
	// �ж��������
	void stopCameraAnim();
	/// ����۲���
	void insertObserver(IFollowCameraObserver *ob);

	class PtrLess
	{
	public:
		bool operator () (const void *a, const void *b)
		{
			return (intptr_t)a < (intptr_t)b;
		}
	};

private:
	Ogre::Camera *m_pCameraNode;
	Ogre::Camera *m_pCutActorCameraNode;
	Ogre::ShowGameMap *m_pGroundMap;

	float m_lastSpeed;
	unsigned int m_lastTick;
	bool m_isSkid;
	int m_lastDerTime;
	Ogre::WorldPos m_Target;
	Ogre::WorldPos m_TargetFinal;
	Ogre::WorldPos m_CameraCenter;
	float m_AngleLR;
	float m_AngleUD;
	float m_CurDist;
	float m_TargetDist;
	float m_DistMoveSpeed;

	float m_MinDist;
	float m_MaxDist;
	float m_MinAngleUD;
	float m_MaxAngleUD;
	float m_MidAngleUD;
	float m_MinFov;
	float m_MaxFov;

	float m_MaxDistMoveSpeed;
	float m_MinDistMoveSpeed;
	float m_MoveTotalTime;
	float m_CurMoveTime;

	Ogre::Model*		m_pAnimMod;						// ��������
	unsigned int		m_nStartTick;					// ��ʼʱ��
	unsigned int		m_nAnimPassTime;				// ����ʱ��
	bool				m_bPlayAnim;	
	std::set<IFollowCameraObserver *, PtrLess>	m_lstObserver;

	bool m_bClamp;

	void UpdateCameraNode(unsigned int dtick);
};

#endif