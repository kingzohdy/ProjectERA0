
#ifndef __OgreEntityMotion_H__
#define __OgreEntityMotion_H__

#include "OgreBaseObject.h"
#include "OgreEntityMotionElement.h"
#include "OgreEntityMotionData.h"

namespace Ogre
{
	class _OgreExport ModelMotion : public BaseObject, public LoadWrap
	{
	private:
		class Player
		{
			Entity *m_pModel;
		public:
			Player() : m_pModel(NULL) {}
			virtual ~Player() {}
			void setModel(Entity *model);
			void play(ModelMotion *motion);
			virtual void onPlay(ModelMotion *, Entity *) = 0;
		};

		class MotionPlayer : public Player
		{
		public:
			virtual void onPlay(ModelMotion *, Entity *);
		};

		class FlashChainPlayer : public Player
		{
		private:
			Ogre::Vector3	target;
			int				anchorid;

		public:
			FlashChainPlayer(int ac, const Ogre::Vector3 &v);
			virtual void onPlay(ModelMotion *, Entity *);
		};

		class ForcePEPlayer : public Player
		{
		private:
			Ogre::Vector3 pos;
			float	range;

		public:
			ForcePEPlayer(const Ogre::Vector3 &v, float r);
			virtual void onPlay(ModelMotion *, Entity *);
		};
	public:
		ModelMotion() :
		  m_bPause(false),
			  m_iModLodLevel((int)MOTION_LOD_HIGH),
			  m_iMainSeq(0),
			  m_fEscapeTime(0),
			  m_MotionClass(0),
			  m_fTime(0.0f),
			  m_bPlaying(false),
			  m_pMotionData(NULL),
			  m_hMotionLoad(NULL),
			  m_pMotionPlayer(NULL)
		{
		}

		void			PlayMotion(Entity* pModel);
		void			PlayFlashChain(Entity* pModel,int anchorid,Ogre::Vector3 vecTarget);
		void			PlayForcePE(Entity *pModel, Ogre::Vector3 pos, float fRange);
		void			Update(float dtime, Entity* pModel);
		void			resetUpdate(bool bpause, float time, Entity *pentity);
		bool			IsPlaying() { return m_bPlaying || m_pMotionPlayer != NULL; };
		void			Stop(Entity* pModel);
		void			DelayStop (Entity* pModel, float second=5.0f);
		void			Pause(bool bPause, Entity* pModel);

		void			LoadFromSource(EntityMotionData* pSource);
		void			LoadFromName(const FixedString &name, bool block = false);
		void			ResourceLoaded(Resource *rec, ResourceHandle h);
		ObjectMotion*	CreateObjMotionFromSrc(MotionElementData *pelement);

		void			SetLodLevel(LOD_LEVEL& level) { m_iModLodLevel = (int)level; }
		void			SetLodLevel(int level) { m_iModLodLevel = level; }

		bool m_bPlaying;
		bool m_bPause;
		int m_iMainSeq;
		float m_fEscapeTime;
		std::vector<ObjectMotion*> m_objMotion;

		float m_fTime;
		FixedString m_strName;
		MOTIONLOOP_T m_loop;
		int m_iModLodLevel;

		EntityMotionData *m_pMotionData;
		ResourceHandle m_hMotionLoad;
		Player	*m_pMotionPlayer;

		int m_MotionClass;

	protected:
		~ModelMotion();
	};
}

#endif