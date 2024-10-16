#pragma once
#include <string>
#include <map>
#include "OgrePrerequisites.h"

struct ANIMINFO
{
	int iSeqId;
	int iAnimGroup;
	int iSeqIndex;
};

class AnimFileInfo
{
public:
	std::string			m_filename;
	Ogre::AnimationData*	m_pAnimRes;
};

class CSkeleton
{
public:
	CSkeleton(void);
	~CSkeleton(void);

	Ogre::ModelData *m_pModelData;
	Ogre::SkeletonData* m_pSkeleton;
	std::map<std::string, int> m_nameId;

	std::vector<ANIMINFO>	m_animOrder;
	std::vector<std::string> m_animChunks;

	std::vector<AnimFileInfo> m_animFiles;

	void SetCoreSkeleton(Ogre::ModelData* pmodeldata);
	Ogre::SkeletonData* GetCoreSkeleton();
	void ClearSkeleton();

	std::string GetDisplayName(int id);
	int GetId(std::string displayName);

	void GetAllDisplayName(std::vector<std::string>& outName);
	Ogre::AnimationData* GetAnimResByIndex(int index);

	void RefreshSkeleton();

	void AttachAnims(std::string filename);
	void RemoveAnims(std::string filename);
	void RemoveAnimsInner(std::string filename);
	void RemoveImportedAnim();

	Ogre::SequenceDesc* GetSequence(int iSeq);
	Ogre::SequenceDesc *getSequenceByID(int seqid);
};
