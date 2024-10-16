
#ifndef __OgreSoundSystemFMod_H__
#define __OgreSoundSystemFMod_H__

#include "OgreSoundSystem.h"
#include "fmod/inc/fmod.hpp"
#include <hash_map>
#include <list>

namespace Ogre
{
	class FmodSound : public ISound
	{
	public:
		FmodSound(FMOD::Channel* pChannel);
		virtual ~FmodSound();
		virtual bool isPlaying();
		virtual void setVolume(float fVolume);
		virtual void setPaused(bool bPaused);
		virtual void stop();

		// ���淽������3DSound��Ч
		virtual void setPosition(const Vector3& vPos);
		virtual void setVelocity(const Vector3& vVec);
		virtual void setDistance(float mindist, float maxdist);

	private:
		FMOD::Channel*			m_pChannel;
		float					m_fLastVolume;
	};

	struct FmodSoundResource
	{
		char				szName[256];
		FMOD::Sound*		pFmodSound;
		uint				nRefCount;
	};

	struct lessstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};

	typedef stdext::hash_map< const char*, FmodSoundResource*, 
		stdext::hash_compare<const char*, lessstr> > ResourceMap;

	class FmodSoundSystem : public SoundSystem
	{
		enum SoundType
		{
			ST_2D,
			ST_3D
		};

		struct MusicChannelInfo
		{
			FMOD::Channel*			pChannelMain;
			FMOD::Channel*			pChannelFadein;
			uint					nFadeinTimeLen;
			uint					nFadeinTimeLeft;
			float main_volume;
			float fadein_volume;
			float					fVolume;
			char main_path[256];
			char fadein_path[256];
		};

	public:
		FmodSoundSystem();

		virtual ~FmodSoundSystem();

		bool Init(const SoundSystemInitInfo& InitInfo);

		void Terminate();

		virtual void LoadSoundRes (const char *filename);
		virtual void LoadMusicRes (const char *filename);
		virtual void ReleaseRes ();

		// ��ָ��ͨ����������
		// nChannel		: �������ֵ�ͨ����0-3��
		// szFileName	: ���ŵ������ļ�����ΪNULL��ʾֹͣ��ǰ����
		// bLoop		: �Ƿ�ѭ������
		// nFadeTime	: ���뵭��ʱ�䣬Ϊ0��ʾ�����������������
		virtual void playMusic(uint nChannel, const char* szFileName, bool bLoop, uint nFadeTime, float volume=1.0f);
		virtual void PlayMusicInternal(uint nChannel, FMOD::Sound* pSound, bool bLoop, uint nFadeTime, float volume=1.0f, const char *path="");

		// ������������
		// nChannel		: �趨������ͨ����0-3��
		// fVolume		: �趨������ֵ��0.0f-1.0f��
		virtual void setMusicVolume(uint nChannel, float fVolume);

		// ����2D��Ч�����ź󲻹�
		virtual void playSound2D(const char* szFileName, float fVolume);

		// ����3D��Ч�����ź󲻹�
		virtual void playSound3D(const char* szFileName, const SoundCreateInfo3D& createInfo);

		// ����2D��Ч��ͨ�����ص�ISound�ӿڲ�����Ч��������������ISound�ӿڵ�Release�����ͷ��ڴ�
		virtual ISound* playSound2DControl(const char* szFileName, float fVolume, bool bLoop, int ambient=0);

		// ����3D��Ч��ͨ�����ص�ISound�ӿڲ�����Ч��������������ISound�ӿڵ�Release�����ͷ��ڴ�
		virtual ISound* playSound3DControl(const char* szFileName, const SoundCreateInfo3D& createInfo);

		// �����������ԣ������ĵĲ���������ΪNULL
		virtual void setListener(const Vector3* vPos, const Vector3* vVel, 
			const Vector3* vForward, const Vector3*  vUp);
		virtual Vector3 getListenerPos();

		// ����ϵͳ����������
		virtual void setGlobalMusicVolume(float fVolume);
		// ����ϵͳ����Ч����
		virtual void setGlobalSoundVolume(float fVolume);

		// ϵͳ���£���Ҫÿ֡����һ��
		virtual void update();

		virtual void setPause(bool bPause);

	private:
		void cleanMusicChannel(int channel, bool main, bool fadein);

	private:
		FMOD::System*				m_pFmodSystem;
		FMOD::ChannelGroup*			m_pChannelGroupMusic;
		FMOD::ChannelGroup*			m_pChannelGroupSound;
		ResourceMap					m_soundResourceMap;

		MusicChannelInfo			m_musicChannel[4];

		uint						m_tmLastUpdate;

		FMOD::Sound* GetMusicResource(const char* szFileName);
		FMOD::Sound* GetSoundResource(const char* szFileName, SoundType eType, bool block = true);
	};

	class FakeSoundSystem : public SoundSystem
	{
	public:
		virtual void LoadSoundRes (const char *filename) {}
		virtual void LoadMusicRes (const char *filename) {}
		virtual void ReleaseRes () {}
		virtual void playMusic(uint nChannel, const char* szFileName, bool bLoop, uint nFadeTime, float volume=1.0f){}
		virtual void PlayMusicInternal(uint nChannel, FMOD::Sound* pSound, bool bLoop, uint nFadeTime, float volume=1.0f){}
		virtual void setMusicVolume(uint nChannel, float fVolume){}
		virtual void playSound2D(const char* szFileName, float fVolume){}
		virtual void playSound3D(const char* szFileName, const SoundCreateInfo3D& createInfo){}
		virtual ISound* playSound2DControl(const char* szFileName, float fVolume, bool bLoop, int ambient=0){return NULL;}
		virtual ISound* playSound3DControl(const char* szFileName, const SoundCreateInfo3D& createInfo){return NULL;}
		virtual void setListener(const Vector3* vPos, const Vector3* vVel, 
			const Vector3* vForward, const Vector3*  vUp){}
		virtual Vector3 getListenerPos() {return m_listenerPos;}

			virtual void setGlobalMusicVolume(float fVolume){}
			virtual void setGlobalSoundVolume(float fVolume){}
			virtual void update(){}
			virtual void setPause(bool bPause){}
	};
}

#endif