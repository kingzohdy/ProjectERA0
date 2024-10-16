
#ifndef __OgreSoundData_H__
#define __OgreSoundData_H__

#include "OgreResource.h"
#include "OgreSoundSystem.h"

namespace Ogre
{
	enum SOUND_PLAY_MODE
	{
		PM_FREE,
		PM_LIMITED
	};

	class _OgreExport SoundData : public Resource
	{
		DECLARE_RTTI(SoundData)
	public:
		SoundData()
		{
			m_Ambient = 0;
		}

		~SoundData()
		{
		}

		FixedString m_strFileName;
		SoundCreateInfo3D m_CreateInfo;
		SOUND_PLAY_MODE m_PlayMode;
		int m_Ambient; // 0 表示非插棒子，1表示是插棒子的 不用加载和保存
		virtual void SoundData::_serialize(Archive &ar, int version);
	};
}

#endif