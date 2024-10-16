
#include "stdafx.h"
#include "exp_anim.h"
#include "exp_nodetree.h"
#include <sstream>
#include <algorithm>
#include <string>

using namespace Ogre;

namespace DKEXP
{
	static ::Matrix3 GetBoneTM(INode *pnode, TimeValue t)
	{
		::Matrix3 tm;
		if( pnode )
		{
			tm = pnode->GetNodeTM( t );
		}
		else tm.IdentityMatrix();

		return tm;
	}

	static ::Matrix3 GetRelativeTM(INode *pMaxNode, TimeValue tick)
	{
		INode *pParent = pMaxNode->GetParentNode();

		::Matrix3 tm = GetBoneTM(pMaxNode, tick);
		if(pParent != NULL)
		{
			tm = tm * Inverse(GetBoneTM(pParent, tick));
		}

		tm = Inverse(Max2EngineMatrix()) * tm * Max2EngineMatrix();

		return tm;
	}

	//---------------------------------------------------------------------------
	AnimationConverter::AnimationConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;
	}

	//---------------------------------------------------------------------------
	AnimationConverter::~AnimationConverter()
	{
	}

	//---------------------------------------------------------------------------
	void AnimationConverter::GetKeyFrames(KeyFrameData &keyFrame, TimeValue tick, INode *pMaxNode, INode *pParent, int nodetype)
	{
		assert( pMaxNode );

		keyFrame.m_tick = tick * 1000/TIME_TICKSPERSEC;
		::Matrix3 rel_tm = GetRelativeTM(pMaxNode, tick);
		if(nodetype == 1) rel_tm.NoScale();

		keyFrame.m_TM = rel_tm;
	}

	//---------------------------------------------------------------------------
	int AnimationConverter::Convert(INode *pMaxNode, int nodetype)
	{
		m_itype = 1;
		m_bUsed = true;

		if(pMaxNode->IsRootNode())
		{
			m_bone.m_boneID = reinterpret_cast<unsigned int>(pMaxNode);
			m_bone.m_FatherID = 0;
			m_bone.m_flipScale = 1.0f;
			m_bone.m_originTM = ConvertMatrix3(GetRelativeTM(pMaxNode, m_animStart));

			m_bone.m_userData = "";
			m_bone.m_Name = pMaxNode->GetName();

			m_keyFrame.resize(0);
			return W3D_STAT_OK;
		}

		::Matrix3 tm = pMaxNode->GetNodeTM(m_animStart);
		INode *pParent = pMaxNode->GetParentNode();
		::Matrix3 parent_tm = pParent->GetNodeTM(m_animStart);
		::Matrix3 rel_tm = GetRelativeTM(pMaxNode, m_animStart);
		if(nodetype == 1) rel_tm.NoScale();

		m_bone.m_boneID = reinterpret_cast<unsigned int>(pMaxNode);
		m_bone.m_FatherID = reinterpret_cast<unsigned int>(pMaxNode->GetParentNode());
		m_bone.m_flipScale = 1.0f;
		m_bone.m_originTM = ConvertMatrix3(rel_tm);
		m_bone.m_Name = pMaxNode->GetName();

		TSTR strUserData;
		pMaxNode->GetUserPropBuffer(strUserData);
		if (strUserData.Length() == 0)
		{
			m_bone.m_userData = "";
		}
		else
		{
			m_bone.m_userData = strUserData;
		}

		KeyFrameData keyFrame;

		int uiNumFrames = (m_animEnd - m_animStart)/GetTicksPerFrame();
		size_t beginframe = m_animStart/GetTicksPerFrame();
		size_t endframe = m_animEnd/GetTicksPerFrame();
		if (pParent)
		{
			for(size_t iframe=beginframe; iframe<=endframe; iframe++)
			{
				GetKeyFrames(keyFrame, (beginframe+iframe)*GetTicksPerFrame(), pMaxNode, pParent, nodetype);
				m_keyFrame.push_back(keyFrame);
			}
		}

		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	void AnimationConverter::ConvertSelectBonesName(char *p, SEQUENCE_EXP_T &sequence)
	{
		std::string strSrc  = p;

		bool bSelect = false;
		strSrc = strSrc.substr(strlen("-selectbonesname "));

		int pos = 0;
		pos = strSrc.find(" ", pos);
		std::string strTemp = strSrc.substr(0, pos);
		std::string strNodeName;

		std::stringstream ss(strTemp);

		while(getline(ss, strNodeName, '@'))
		{
			std::replace(strNodeName.begin(), strNodeName.end(), '#', ' ');

			if (!strNodeName.empty())
			{
				bSelect = true;
				sequence.bonesname.push_back(strNodeName.c_str());
			}
		}

		if(bSelect)
		{
			sequence.bSelectBones = true;
		}else
		{
			sequence.bSelectBones = false;
			sequence.bonesname.resize(0);
		}
	}

	//---------------------------------------------------------------------------
	void AnimationConverter::ConvertMaterialAnimationMeshes(char *p, SEQUENCE_MTLANI_T &sequence)
	{
		std::string strSrc  = p;

		strSrc = strSrc.substr(strlen("-materialmeshes "));

		int pos=0;
		pos = strSrc.find(" ", pos);
		std::string strTemp = strSrc.substr(0, pos);
		std::string strNodeName;

		std::stringstream ss(strTemp);

		while(getline(ss, strNodeName, '@'))
		{
			std::replace(strNodeName.begin(), strNodeName.end(), '#', ' ');
			sequence.animesh.push_back(strNodeName.c_str());
		}
	}

	//---------------------------------------------------------------------------
	int AnimationConverter::BuildNoteTrack(INode* pMaxNode, std::vector<SEQUENCE_EXP_T> &sequenceArray, std::vector<TriggerDesc> &triggerArray)
	{
		//std::vector<SEQUENCE_MTLANI_T> mtlaniArray;

		char *p = NULL;
		char *pStart = "start";
		char *pSeqStart = "OgreBegin";
		char *pSeqEnd = "OgreEnd";
		char *pTrigger = "OgreTrigger";
		char *pLoop = " -loop";
		char *pType = " -type";
		char *pSeqID = " -seqID";
		char *pSeqName = "-name";
		char *pSelBonesName = "-selectbonesname";
		char *pExponly = "-exponly";

		char *pMtlAniStart = "OgreMaterialBegin";
		char *pMtlAniEnd = "OgreMaterialEnd";
		char *pMtlMesh = "-materialmeshes";

		bool bStart = true;
		bool bMtlAniStart = true;
		SEQUENCE_EXP_T seq;
		SEQUENCE_MTLANI_T mtlseq;
		TriggerDesc trig;

		if(!pMaxNode->HasNoteTracks())
		{
			return W3D_STAT_OK;
		}
		
		unsigned int numtracks = (unsigned int) pMaxNode->NumNoteTracks();
		const char *pnodename = pMaxNode->GetName();
		for (unsigned int ui = 0; ui<numtracks; ui++)
		{
			DefNoteTrack * notetrack = (DefNoteTrack *)pMaxNode->GetNoteTrack(ui);
			if(notetrack == NULL)
			{
				continue;
			}

			for(int k = 0; k < notetrack->keys.Count();k++)
			{
				NoteKey * notekey = *(notetrack->keys.Addr(k));
				if(notekey)
				{
					TSTR str = notekey->note;

					//如果是Gamebryo的notetrack不做处理
					p = strstr(str, pStart);
					if (p)
						break;

					//Sequence NoteTrack
					if(bStart)
					{
						p = strstr(str, pSeqStart);
						if (p)
						{
							TimeValue tStart = notekey->time;

							seq.sequence.time_start = tStart;

							p = strstr(str, pSeqName);
							if (p)
							{
								std::string strSrc = p+6;
								int pos = 0;
								pos = strSrc.find(" ", pos);
								std::string strTemp = strSrc.substr(0, pos);

								seq.sequence.id = atoi(strTemp.c_str());
								if(seq.sequence.id==0 && strTemp[0]!='0')
								{
									char temp[256];
									sprintf(temp, "物体%s中的%s动作转成ID错误", pnodename, strTemp.c_str());
									MessageBox(NULL, temp, "Warning!", MB_OK);
									continue;
								}
							}
							else
							{
								seq.sequence.id = 0;
							}

							p = strstr(str, pLoop);
							if (p)
								seq.sequence.loopmode = (AnimPlayMode)atoi(p+6);
							else
								seq.sequence.loopmode = ANIM_MODE_LOOP;

							//如果只导出该桢的数据
							p = strstr(str, pExponly);
							if (p)
							{
								HierConverter::m_bExpOnly = true;
							}

							p = strstr(str, pSelBonesName);
							if (p)
							{
								ConvertSelectBonesName(p, seq);
							}
							else
							{
								seq.bSelectBones = false;
								seq.bonesname.resize(0);
							}

							bStart = false;
							continue;
						}
					}
					else
					{
						p = strstr(str, pSeqEnd);
						if (p)
						{
							TimeValue tEnd = notekey->time;
							seq.sequence.time_end = tEnd;

							//p = strstr(str, pSeqID);

							//if (p)
							//	seq.sequence.id = atoi(p+7);								
							//else
							//	seq.sequence.id = 0;

							//如果只导出该桢的数据
							if(HierConverter::m_bExpOnly)
							{
								sequenceArray.clear();
								sequenceArray.push_back(seq);

								seq.bonesname.clear();
								return W3D_STAT_OK;
							}

							sequenceArray.push_back(seq);
							seq.bonesname.clear();

							bStart = true;
							continue;
						}
						else
						{
							assert(0);
						}
					}

					//Material Animation Sequence NoteTrack
					/*
					if(bMtlAniStart)
					{
						p = strstr(str, pMtlAniStart);
						if (p)
						{
							TimeValue tStart = notekey->time;

							mtlseq.sequence.time_start = tStart;

							p = strstr(str, pSeqName);
							if (p)
							{
								std::string strSrc = p+6;
								int pos = 0;
								pos = strSrc.find(" ", pos);
								std::string strTemp = strSrc.substr(0, pos);

								mtlseq.sequence.id = m_AnimNameMap->getIDByName(strTemp.c_str());

								//if (mtlseq.sequence.id == -1)
								//{
								//	char temp[256];
								//	sprintf(temp, "材质动画名为:%s的sequence错误。", strTemp.c_str());
								//	MessageBox(NULL, temp, "Error!", MB_OK);
								//	return W3D_STAT_FAILED;
								//}
							}
							else
							{
								mtlseq.sequence.id = 0;
							}

							p = strstr(str, pLoop);
							if(p) mtlseq.sequence.loopmode = (AnimPlayMode)atoi(p+6);
							else mtlseq.sequence.loopmode = ANIM_MODE_LOOP;

							p = strstr(str, pMtlMesh);
							if(p) ConvertMaterialAnimationMeshes(p, mtlseq);
							else mtlseq.animesh.resize(0);

							bMtlAniStart = false;
							continue;
						}
					}
					else
					{
						p = strstr(str, pMtlAniEnd);
						if (p)
						{
							TimeValue tEnd = notekey->time;
							mtlseq.sequence.time_end = tEnd;

							//p = strstr(str, pSeqID);

							//if (p)
							//	mtlseq.sequence.id = atoi(p+7);								
							//else
							//	mtlseq.sequence.id = 0;

							bMtlAniStart = true;

							mtlaniArray.push_back(mtlseq);

							//清空mtlseq
							mtlseq.animesh.clear();
							int aaa = mtlseq.animesh.size();
							continue;
						}
						else
							assert(0);
					}
					*/

					//Trigger NoteTrack
					p = strstr(str, pTrigger);
					if (p)
					{
						TimeValue tStart = notekey->time;
						TSTR str = notekey->note;

						trig.tick = tStart;

						p = strstr(str, pType);
						if (p)
							trig.type = atoi(p+6);
						else
							trig.type = -1;

						if (trig.type == -1)
						{
							continue;
						}
						triggerArray.push_back(trig);
					}
					else
						break;
				}
			}
		}

		return W3D_STAT_OK;
	}
}
