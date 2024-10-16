
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

		// 下面方法仅对3DSound有效
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

		// 在指定通道播放音乐
		// nChannel		: 播放音乐的通道（0-3）
		// szFileName	: 播放的音乐文件名，为NULL表示停止当前音乐
		// bLoop		: 是否循环播放
		// nFadeTime	: 淡入淡出时间，为0表示立即以最大音量播放
		virtual void playMusic(uint nChannel, const char* szFileName, bool bLoop, uint nFadeTime, float volume=1.0f);
		virtual void PlayMusicInternal(uint nChannel, FMOD::Sound* pSound, bool bLoop, uint nFadeTime, float volume=1.0f, const char *path="");

		// 设置音乐音量
		// nChannel		: 设定音量的通道（0-3）
		// fVolume		: 设定的音量值（0.0f-1.0f）
		virtual void setMusicVolume(uint nChannel, float fVolume);

		// 播放2D音效，播放后不管
		virtual void playSound2D(const char* szFileName, float fVolume);

		// 播放3D音效，播放后不管
		virtual void playSound3D(const char* szFileName, const SoundCreateInfo3D& createInfo);

		// 播放2D音效，通过返回的ISound接口操纵音效参数，最后需调用ISound接口的Release函数释放内存
		virtual ISound* playSound2DControl(const char* szFileName, float fVolume, bool bLoop, int ambient=0);

		// 播放3D音效，通过返回的ISound接口操纵音效参数，最后需调用ISound接口的Release函数释放内存
		virtual ISound* playSound3DControl(const char* szFileName, const SoundCreateInfo3D& createInfo);

		// 设置听者属性，不关心的参数可以设为NULL
		virtual void setListener(const Vector3* vPos, const Vector3* vVel, 
			const Vector3* vForward, const Vector3*  vUp);
		virtual Vector3 getListenerPos();

		// 设置系统级音乐音量
		virtual void setGlobalMusicVolume(float fVolume);
		// 设置系统级音效音量
		virtual void setGlobalSoundVolume(float fVolume);

		// 系统更新，需要每帧调用一次
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