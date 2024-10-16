
#ifndef __OgreSoundSystem_H__
#define __OgreSoundSystem_H__

#include "OgreVector3.h"
#include "OgreSingleton.h"

namespace Ogre
{

	class ISound
	{
	public:
		virtual ~ISound(){}
		virtual bool isPlaying() = 0;
		virtual void setVolume(float fVolume) = 0;
		virtual void setPaused(bool bPaused) = 0;
		virtual void stop() = 0;

		// ���淽������3DSound��Ч
		virtual void setPosition(const Vector3& vPos) = 0;
		virtual void setVelocity(const Vector3& vVec) = 0;
		virtual void setDistance(float mindist, float maxdist) = 0;

		void release(){ delete(this); }
	};

	struct SoundSystemInitInfo
	{
		int			maxchannels;

		float		dopplerscale;
		float		distancefactor;
		float		rolloffscale;
	};

	struct SoundCreateInfo3D
	{
		float		mindistance; 
		float		maxdistance;
		float		volume;
		Vector3		pos;
		Vector3		velocity;
		bool		isloop;

		// 102
		float time0;
		float time1;

		// 103
		float fSoundFullRange;
	};

	class _OgreExport SoundSystem : public Singleton<SoundSystem>
	{
	public:
		virtual ~SoundSystem()
		{
		}

		virtual void LoadSoundRes (const char *filename) = 0;
		virtual void LoadMusicRes (const char *filename) = 0;
		virtual void ReleaseRes () = 0;

		// ��ָ��ͨ����������
		// channel		: �������ֵ�ͨ����0-3��
		// filename	: ���ŵ������ļ�����ΪNULL��ʾֹͣ��ǰ����
		// loop		: �Ƿ�ѭ������
		// fadetick	: ���뵭��ʱ�䣬Ϊ0��ʾ�����������������
		virtual void playMusic(uint channel, const char*filename, bool loop, uint fadetick, float volume=1.0f) = 0;

		// ������������
		// nChannel		: �趨������ͨ����0-3��
		// fVolume		: �趨������ֵ��0.0f-1.0f��
		virtual void setMusicVolume(uint nChannel, float fVolume) = 0;

		virtual void playSound2D(const char* szFileName, float fVolume) = 0;
		virtual void playSound3D(const char* szFileName, const SoundCreateInfo3D& createInfo) = 0;

		//���ص�ISound�ӿ���Ҫ�ͷ�
		virtual ISound* playSound2DControl(const char* szFileName, float fVolume, bool bLoop, int ambient = 0) = 0;
		//���ص�ISound�ӿ���Ҫ�ͷ�
		virtual ISound* playSound3DControl(const char* szFileName, const SoundCreateInfo3D& createInfo) = 0;

		//�����ĵĲ���������ΪNULL
		virtual void setListener(const Vector3* vPos, const Vector3* vVel, const Vector3* vForward, const Vector3*  vUp) = 0;
		virtual Vector3 getListenerPos() = 0;

		virtual void setGlobalMusicVolume(float volume) = 0;
		virtual void setGlobalSoundVolume(float volume) = 0;

		virtual void update() = 0;

		virtual void setPause(bool bPause) = 0;

	protected:
		Vector3	m_listenerPos;
	};

	enum SOUND_SYSTEM_TYPE
	{
		SST_FMOD = 0,
		SST_DUMMY
	};

	_OgreExport SoundSystem *CreateSoundSystem(SOUND_SYSTEM_TYPE type, const SoundSystemInitInfo &info);
}

#endif