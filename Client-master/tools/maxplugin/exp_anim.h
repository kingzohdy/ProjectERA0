
#ifndef __EXP_ANIM_H__
#define __EXP_ANIM_H__

//#include "exp_nodetree.h"
#include "exp_types.h"

namespace DKEXP
{
	class KeyFrameData
	{
	public:
		unsigned int m_tick;
		DecompMatrix m_TM;
	};

	class BoneData
	{
	public:
		unsigned int	m_boneID;
		unsigned int	m_FatherID;
		float			m_flipScale;

		Ogre::Matrix4	m_originTM;
		Ogre::String	m_userData;
		Ogre::String	m_Name;
	}; 

	class AnimationConverter
	{
	public:
		AnimationConverter(TimeValue animStart, TimeValue animEnd);
		~AnimationConverter();

		int Convert(INode *pMaxNode, int nodetype);
		void GetKeyFrames(KeyFrameData &keyFrame, TimeValue tick, INode *pMaxNode, INode *pParent, int nodetype);
		static void ConvertSelectBonesName(char *p, SEQUENCE_EXP_T &sequence);
		static void ConvertMaterialAnimationMeshes(char *p, SEQUENCE_MTLANI_T &sequence);

		static int BuildNoteTrack(INode* pMaxNode, std::vector<SEQUENCE_EXP_T> &sequenceArray, std::vector<Ogre::TriggerDesc> &triggerArray);

		//for convert
		int							m_itype;
		bool						m_bUsed;

		BoneData						m_bone;

		std::vector<KeyFrameData>	m_keyFrame;

	private:
		TimeValue m_animStart;
		TimeValue m_animEnd;
	};
}

#endif