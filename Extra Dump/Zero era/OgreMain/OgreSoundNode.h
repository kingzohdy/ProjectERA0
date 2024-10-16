
#ifndef __OgreSoundNode_H__
#define __OgreSoundNode_H__

#include "OgreSoundData.h"
#include "OgreMovableObject.h"
#include "OgreRandomNumber.h"

namespace Ogre
{
	class _OgreExport SoundNode : public MovableObject
	{
		DECLARE_RTTI(SoundNode)

	public:
		SoundNode(){}
		SoundNode(const char* szFileName, const SoundCreateInfo3D& createInfo);
		SoundNode(SoundData* pSrc, bool playnow=true);
		~SoundNode();

		virtual void update(uint dtick);
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();

		void play();
		bool isPlaying();
		void setVolume(float fVolume);
		void pause(bool bPaused);
		void stop();
		void setVelocity(const Vector3& vVec);
		void setDistance(float mindist, float maxdist);
		void setSoundFile(const char *filename);

		void setRandomTime0 (float time0);
		void setRandomTime1 (float time1);

		void setSoundFullRange (float fullrange);

		void setIsLoop (bool loop);

		const char *getFileName() {return m_szFileName;}

	protected:
		SoundSystem*				m_pSoundSystem;
		char						m_szFileName[256];
		SoundCreateInfo3D			m_createInfo;
		ISound*						m_pSound;
		SOUND_PLAY_MODE				m_PlayMode;
		bool						m_bPlayed;
		int						    m_Ambient;

		float						m_Time;
		float						m_RandomTime;
		RandomGenerator				m_RandomGen;
		bool						m_bUseRandom;
	};
}

#endif