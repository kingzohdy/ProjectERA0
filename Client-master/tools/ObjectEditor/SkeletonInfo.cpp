
#include "stdafx.h"
#include "SkeletonInfo.h"

#include "OgreModel.h"
#include "OgreModelData.h"
#include "OgreSkeletonData.h"
#include "OgreSkeletonAnimData.h"
#include "OgreResourceManager.h"

CSkeleton::CSkeleton(void) : m_pSkeleton(NULL), m_pModelData(NULL)
{
}

CSkeleton::~CSkeleton(void)
{
	ClearSkeleton();
}

void CSkeleton::SetCoreSkeleton(Ogre::ModelData* pmodeldata)
{
	ClearSkeleton();

	m_pModelData = pmodeldata;
	if(pmodeldata)
	{
		pmodeldata->addRef();
		m_pSkeleton = pmodeldata->getSkeletonData();

		RefreshSkeleton();
	}
}

void CSkeleton::RefreshSkeleton()
{
	char namebuf[256] = {0};
	m_nameId.clear();

	if(m_pSkeleton)
	{
		for(size_t i = 0; i < m_pSkeleton->getNumBone(); i++)
		{
			Ogre::BoneData* pBone = m_pSkeleton->getIthBone(i);
			sprintf(namebuf, "%s(%d)", (const char*)pBone->m_Name, pBone->m_ID);
			std::string name = namebuf;
			m_nameId.insert(std::pair<std::string, int>(name, pBone->m_ID));
		}
	}

	m_animOrder.clear();
	if(m_pModelData)
	{
		for(size_t ianim=0; ianim<m_pModelData->getNumAnim(); ianim++)
		{
			Ogre::AnimationData *panim = m_pModelData->getAnimation(ianim);

			for(size_t i=0; i<panim->m_Sequences.size(); i++)
			{
				ANIMINFO animInfo;
				animInfo.iSeqId = panim->m_Sequences[i].id;
				animInfo.iAnimGroup = ianim;
				animInfo.iSeqIndex = i;
				m_animOrder.push_back(animInfo);
			}
		}
	}
}

Ogre::AnimationData* CSkeleton::GetAnimResByIndex(int index)
{
	int i = 0;
	if(m_pSkeleton == NULL)
		return NULL;

	if(index>=0 && index<m_pModelData->getNumAnim()) return m_pModelData->getAnimation(index);
	return NULL;
}

Ogre::SkeletonData* CSkeleton::GetCoreSkeleton()
{
	return m_pSkeleton;
}

void CSkeleton::ClearSkeleton()
{
	OGRE_RELEASE(m_pModelData);

	m_nameId.clear();
	m_animChunks.clear();
	m_animOrder.clear();
	m_pSkeleton = NULL;

	for(int i = 0; i < (int) m_animFiles.size(); i++)
	{
		if(m_animFiles[i].m_pAnimRes != NULL)
		{
			m_animFiles[i].m_pAnimRes->release();
		}
	}
	m_animFiles.clear();

}

std::string CSkeleton::GetDisplayName(int id)
{
	std::map<std::string, int>::iterator i = m_nameId.begin();
	while(i != m_nameId.end())
	{
		if(i->second == id)
			return i->first;
		i++;
	}
	return "";
}

int CSkeleton::GetId(std::string displayName)
{
	return m_nameId[displayName];
}

void CSkeleton::GetAllDisplayName(std::vector<std::string>& outName)
{
	outName.clear();
	std::map<std::string, int>::iterator i = m_nameId.begin();
	while(i != m_nameId.end())
	{
		outName.push_back(i->first);
		i++;
	}

}

void CSkeleton::AttachAnims(std::string filename)
{

	if(m_pSkeleton == NULL)
	{
		return;
	}

	Ogre::Resource* pRes = Ogre::ResourceManager::getSingleton().blockLoad(filename.c_str());
	if(pRes == NULL)
	{
		return;
	}
	assert(IS_KIND_OF(Ogre::AnimationData, pRes));
	Ogre::AnimationData* pAnimRes = (Ogre::AnimationData *)pRes;

	m_pModelData->addAnimation(pAnimRes);


	AnimFileInfo info;
	info.m_filename = filename;
	info.m_pAnimRes = pAnimRes;

	m_animFiles.push_back(info);
	RefreshSkeleton();

}

void CSkeleton::RemoveAnims(std::string filename)
{
	if(m_pSkeleton == NULL)
	{
		return;
	}

	RemoveAnimsInner(filename);
	RefreshSkeleton();
}


void CSkeleton::RemoveImportedAnim()
{
	std::vector<std::string> names;
	for(int i = 0; i < (int)m_animFiles.size(); i++)
	{
		names.push_back(m_animFiles[i].m_filename);
	}

	for(int i = 0; i < (int)names.size(); i++)
	{
		RemoveAnimsInner(names[i]);
	}
	RefreshSkeleton();
}

void CSkeleton::RemoveAnimsInner(std::string filename)
{
	if(m_pSkeleton == NULL)
	{
		return;
	}

	for(int i = 0; i < (int)m_animFiles.size(); i++)
	{
		if(m_animFiles[i].m_filename == filename)
		{
			Ogre::AnimationData *panim = m_animFiles[i].m_pAnimRes;
			for(size_t ianim=0; ianim<m_pModelData->getNumAnim(); ianim++)
			{
				if(m_pModelData->getAnimation(ianim) == panim)
				{
					m_pModelData->removeAnimation(ianim);
					break;
				}
			}

			if(m_animFiles[i].m_pAnimRes != NULL)
			{
				m_animFiles[i].m_pAnimRes->release();
			}

			m_animFiles.erase(m_animFiles.begin() + i);

			break;
		}
	}
}

Ogre::SequenceDesc *CSkeleton::GetSequence(int iSeq)
{
	for(size_t i=0; i<m_pModelData->getNumAnim(); i++)
	{
		Ogre::AnimationData *panim = m_pModelData->getAnimation(i);

		int index = panim->getSequenceIndex(iSeq);
		if(index >= 0) return panim->getSequence(index);
	}

	assert(0);
	return NULL;
}
