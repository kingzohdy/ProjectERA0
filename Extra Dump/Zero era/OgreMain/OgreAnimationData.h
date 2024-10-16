
#ifndef __AnimationData_H__
#define __AnimationData_H__

#include "OgreResource.h"

namespace Ogre
{
	enum AnimationType
	{
		ANIM_SKELETON = 0,
		ANIM_MORPH,
		ANIM_MATERIAL,
		ANIM_ALL,
		MAX_ANIM_TYPE
	};

	enum AnimPlayMode
	{
		ANIM_MODE_LOOP = 0,//循环播放
		ANIM_MODE_ONCE, //播放一次
		ANIM_MODE_ONCE_STOP, //播放一次完毕，停在末尾
	};

	struct SequenceDesc
	{
		int id;
		uint time_start;
		uint time_end;
		AnimPlayMode loopmode;
	};

	struct TriggerDesc
	{
		uint tick;
		uint type;
	};

	class _OgreExport BaseAnimationData : public Resource
	{
		DECLARE_RTTI_VIRTUAL(BaseAnimationData)
	public:
		bool hasSequence(int id);
		SequenceDesc *getSequence(size_t i);
		int getSequenceIndex(int id);

		void addTrigger(const TriggerDesc &trigger);

		virtual AnimationType getType() = 0;

		virtual void _serialize(Archive &ar, int version);

	public:
		std::vector<TriggerDesc>m_TriggerArray;
		std::vector<SequenceDesc>m_Sequences;
	};

	class _OgreExport AnimationData : public BaseAnimationData
	{
		DECLARE_RTTI(AnimationData)
	public:
		AnimationData();
		~AnimationData();

		virtual AnimationType getType();
		virtual void _serialize(Archive &ar, int version);

		void addBoneTrack(BoneTrack *ptrack);
		void addMtlParamTrack(MaterialParamTrack *ptrack);

		size_t getNumBoneTrack();
		BoneTrack *getBoneTrack(size_t i);

		size_t getNumMtlParamTrack();
		MaterialParamTrack *getMtlParamTrack(size_t i);

	public:
		std::vector<BoneTrack *>m_BoneTracks;
		std::vector<MaterialParamTrack *>m_MtlParamTracks;
	};
}

#endif