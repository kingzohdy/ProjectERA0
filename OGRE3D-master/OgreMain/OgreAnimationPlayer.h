
#ifndef __AnimationPlayer_H__
#define __AnimationPlayer_H__

#include "OgrePrerequisites.h"
#include "OgreAnimationData.h"

namespace Ogre
{
	class AnimPlayTrack
	{
	public:
		enum PlayState
		{
			STATE_NULL = 0,
			STATE_IN,
			STATE_ACTIVE, //在这个layer中active
			STATE_OUT,    //在这个layer中decay
		};

		AnimPlayTrack() : m_State(STATE_NULL)
		{
		}

		void update(uint dtick);
		void resetUpdate(uint tick);

	public:
		AnimationType m_AnimType;
		BaseAnimationData *m_pAnimData;
		int m_iSeq;   //seq index
		int m_nSequence;
		PlayState m_State;
		float  m_Weight;
		uint m_CurTick;
		uint m_Start;
		uint m_End;
		AnimPlayMode m_PlayMode;
		int  m_nPriority;
		float  m_Speed;
		float  m_DelayIn;
		float  m_DelayOut;
	};

	struct AnimGroupPlayInfo
	{
		int	nseq;
		int	seq[4];
		int	priority[4];
		BaseAnimationData *panimdata[4];
	};

	class _OgreExport AnimationPlayer
	{
	public:
		AnimationPlayer(Model *pmodel);
		~AnimationPlayer();

		AnimPlayTrack *findPlayTrack(int seq);
		AnimPlayTrack *play(int seq, BaseAnimationData *panimdata, float weight=1.0f, float speed=1.0f);
		void play(const AnimGroupPlayInfo& playInfo);
		void stop(AnimPlayTrack * hanim);
		void stopAll();
		void pause(AnimPlayTrack * hanim);
		void resume(AnimPlayTrack * hanim);

		void update(uint dtick);
		void resetUpdate(uint tick);
		void setAnimDelayInOut(float delayin, float delayout);

	protected:
		std::vector<AnimPlayTrack *>m_PlayTracks;
		float m_AnimDelayIn;
		float m_AnimDelayOut;
		uint m_AccumTick;

		Model *m_pModel;

	private:
		void applySkeleton(AnimPlayTrack *ptracks[], size_t count, SkeletonInstance *pskeleton);
		void applyMorph(AnimPlayTrack *ptracks[], size_t count);
		void applyMaterial(AnimPlayTrack *ptracks[], size_t count, Material *pmtl);
		void applyAnimToModel(AnimPlayTrack *ptracks[], size_t count, Model *pmodel);
	};
}

#endif