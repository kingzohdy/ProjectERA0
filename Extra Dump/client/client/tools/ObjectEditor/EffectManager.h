
#pragma once
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreEntityMotionElement.h"

class CBounding;
class CBsp;
class CAvatarShowInfo;

struct NEWEFFECT_INFO
{
	int anchor;
	Ogre::Vector3 offset;
	std::string	path; //now means "name"
	std::string	realpath;
	Ogre::Quaternion rotate;
	bool bDynamic;
	bool bShowPane;
	bool bIsPath;
	Ogre::Resource* pChunk;
	Ogre::Vector3 scale;
	bool bIntegrated;
	Ogre::BIND_ANIM_MODE animmode;
	std::string			animname;
	bool				bVisible;
	CEffectUnit*		pOwner;

	NEWEFFECT_INFO()
	{
		pOwner = NULL;
		bVisible = true;
		bDynamic = false;
		animmode = Ogre::BAM_SELF;
		bIntegrated = true;
		pChunk = NULL;
		bIsPath = false;
		bShowPane = true;
	};
};



#define REF_POS_RIGHT	1

class CEffectMgr
{
public:
	CEffectMgr(void);
	~CEffectMgr(void);


	static CEffectMgr*	ms_pInst;
	static CEffectMgr*	CreateInstance()	{ return (ms_pInst = new CEffectMgr()); };
	static CEffectMgr*	GetInstance()		{ return ms_pInst; };
	static void			ReleaseInstance()	{ if(ms_pInst) delete ms_pInst; ms_pInst = NULL; };

	//总体
	void			Initialze();
	void			Release();

	//特效管理
	void			AddEffect(CEffectUnit* pEffect);
	void			RemoveEffect(CEffectUnit* pEffect);
	void			ClearEffect();
	void			ClearModelPrivateEffect();

	CEffectUnit*	FindEffect(int srcid);
	CEffectUnit*	FindEffect(const char* name);
	int				GetNewCreatedEffect(std::vector<CEffectUnit*>& effects);
	void			ExecuteCommand(CEffectCommand* pCommand);
	CCommandMgr*	GetCommandMgr()								{ return m_commandMgr; } ;
	void			RefreshEffect(CEffectUnit* pEffect);
	void			ChangeEffectType(CEffectUnit* pEffect, bool bDynamic);
	void			GetEffectInPrivateMotion(std::vector<CEffectUnit*>& effects);
	void			GetEffectInCommonMotion(std::vector<CEffectUnit*>& effects);

	//模型管理
	void			LoadModelFromXml(CString path);
	bool			LoadModelFromAvatar(int* attrib);
	bool			LoadSkinActor(CString path);
	bool			LoadShowModel(CString path, bool bNotify = true, std::vector<Ogre::Resource *> *animres=NULL);
	void			CreateDefaultModel();
	void			SaveShowActor(CString path);
	bool			SaveAsShowModel(CString path, bool bChangeName = true);
	bool			SaveMotionAsShowModel(CString path, int iIncludeMotion);
	bool			SaveAsShowModelImpl(CString path, bool bChangeName = true, int iIncludeMotion = -1);

	void updateAnchors();

	void			DestroyModel();
	void			DestroyMainNode();
	Ogre::Entity*	GetModelActor()								{ return m_pModelActor; };
	CAnchorSet*		GetAnchorInfo()								{ return m_anchorInfo; };
	CSkeleton*		GetSkeleton()								{ return m_skeleton; };
	CModelMotionInfo* GetModelMotion()							{ return m_modelMotion; };
	BIND_INFO*		GetBindInfoByEffect(CEffectUnit* pEffect);
	Ogre::Entity* CreateCloneModel();

	//动画管理
	bool			IsPaused() { return m_bPause; };
	//void			Pause(bool bPause) { m_bPause = bPause; };
	bool			PlayAnim(int seq);
	void			StopAnim(int seq);
	void			PauseAnim(bool bPause);
	void			PlayMotion(int index);
	void			PlayMotion(const char* name);
	void			InjectMotion(const char* name);
	void			InjectMotion(int index);
	void			InjectMotion(const char* name, Ogre::Entity* pModel);
	void			InjectMotion(int index, Ogre::Entity* pModel);
	void			ClearModelMotion();
	void			StopMotion(int index);
	void			AttachAnim(const char* filename);

	//帧动作
	void			UpdateEffectPos(Ogre::uint dtick);

	//参考点
	CRefNodeMgr*	GetRefNodeMgr()			{ return m_refNodeMgr; };

	//UI控制
	bool			OnLBtnDown(float x, float y);
	bool			OnLBtnUp(float x, float y);
	bool			OnMouseMove(float x, float y);
	bool			OnDblClick(float x, float y);
	bool			SetActiveEffect(CEffectUnit* pUnit);

	//背景更换
	bool			ToBigTerrainRoom(Ogre::GameScene* pSimpleRoom, Ogre::GameScene* pBigRoom);
	bool			ToSimpleRoom(Ogre::GameScene* pBigRoom, Ogre::GameScene* pSimpleRoom);
	bool			BigToBig(Ogre::GameScene* pOldBig, Ogre::GameScene* pNewBig);
	void			SetModelActorPos(Ogre::Vector3& pos);
	void			SetModelTargetPos(Ogre::Vector3& pos);

	//主角是否显示
	bool			m_bShowMainActor;
	bool			GetShowState() { return m_bShowMainActor; }
	void			SetMainActorState(bool bShow);

	std::vector<CEffectUnit*>	m_Effects;

	Ogre::Entity*	m_pModelActor;
	Ogre::Entity* m_pModelTarget; //受击动作模型

	CAnchorSet		*m_anchorInfo;
	CSkeleton		*m_skeleton;
	CBounding		*m_BoundingInfo;
	CBsp			*m_BspInfo;
	CAvatarShowInfo *m_avatarShowInfo;
	CModelMotionInfo *m_modelMotion;
	CCommandMgr		*m_commandMgr;
	CRefNodeMgr		*m_refNodeMgr;

	CString			m_strModName;
	CString			m_strSmName;
	CString			m_strActorFileName;

	std::vector<int>	m_playingAnim;
	bool				m_bPause;

	int				m_iActive;

	void			AddFrameJob(CFrameJob* pJob);
	void			RemoveFrameJob(CFrameJob* pJob);
	std::list<CFrameJob*> m_FrameJobs;
private:
	Ogre::SceneNode* CreateNodeFromRes(Ogre::Resource* pRes);
	void UnbindAllUnit();
};
