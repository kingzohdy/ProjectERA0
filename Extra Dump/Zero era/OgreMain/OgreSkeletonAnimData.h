
#ifndef __SkeletonAnimData_H__
#define __SkeletonAnimData_H__

#include "OgreResource.h"
#include "OgreKeyFrameArray.h"
#include "OgreAnimationData.h"

namespace Ogre
{
	class _OgreExport SkeletonAnimData : public BaseAnimationData
	{
		DECLARE_RTTI(SkeletonAnimData)

	public:
		SkeletonAnimData();
		void addBoneTrack( BoneTrack *ptrack );
		size_t getNumTracks(){ return m_TrackArray.size(); }
		BoneTrack *getTrack( size_t i ){ return m_TrackArray[i]; }

		virtual AnimationType getType()
		{
			return ANIM_SKELETON;
		}

		virtual void _serialize(Archive &ar, int version);

	public:
		std::vector<BoneTrack *>m_TrackArray;

	protected:
		~SkeletonAnimData();
		
	};
}

#endif 