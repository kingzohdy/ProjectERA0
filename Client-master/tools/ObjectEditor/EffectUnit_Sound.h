#pragma once
#include "EffectUnit.h"
#include "InteractObject.h"
#include "BindInfo.h"
#include "EffectCommand.h"

#include "OgreColourValue.h"
#include "OgreSoundData.h"

class CSoundCommand;
class CSound;
class CSoundInteract : public CInteract
{
public:
	CSoundInteract();
	~CSoundInteract();
	//virtual bool OnLBtnDown(float x, float y, bool bForceOn);
	//virtual bool OnLBtnUp(float x, float y);
	//virtual bool OnMouseMove(float x, float y);
	//virtual bool Pick(float x, float y);

	virtual void DoFrame(Ogre::uint dtick) ;

	void Clear();
	void Create(CEffectUnit* pTarget);
	void DrawRange(float r, Ogre::ColorQuad clr);

	//CSound* m_pTarget;

	//bool m_bHovering;
	//bool m_bCood;
	Ogre::Billboard* m_pIdn;
	//COOR_ARROW m_tran;
	//CCoord* m_pCood;
};


class CSound : public CEffectUnit
{
public:
	CSound(void);
	~CSound(void);

	virtual Ogre::SceneNode* GetNode();
	virtual void ReleaseNode();
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent);
	virtual Ogre::Resource* GetChunk();
	virtual Ogre::Resource* GetSource() { return m_pSource;};
	virtual Ogre::SceneNode* CreateNode(); 
	virtual void RefreshNode();

	virtual CString GetName();
	virtual void SetName(CString strName) { m_strName = strName; };

	virtual bool Create();
	virtual bool Destroy();
	//virtual bool IsVisible();
	//virtual void SetVisible(bool visible);

	virtual bool LoadEffect(CString path);
	virtual bool SaveEffect(CString path);

	virtual bool LoadFromChunk(Ogre::Resource* pChunk);

	virtual BIND_INFO* GetBindInfo();
	virtual void SetBindInfo(BIND_INFO* bindinfo);
	virtual EFFECT_TYPE GetType();

	virtual CInteract* GetInteract() {return &m_interact; };

	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand* BeginRecordCommand();
	virtual void EndRecordCommand(CEffectCommand* pCmd);

	virtual void UpdateSource();
	virtual	void DoFrame(Ogre::uint dtick);
	virtual bool IsNew() { return m_bIsNew;};

	virtual float GetRange() { return 20.0f; };
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };
	CDocInfo m_docInfo;

	virtual SCALE_TYPE		GetScaleType() { return ST_NO_SCALING ; }; 	

	bool m_bIsNew;

	BIND_INFO m_bindInfo;
	CString m_strName;
	//bool m_bVisible;
	int m_iVolumeInt;
	CString m_strSoundFile;
	Ogre::SoundCreateInfo3D m_createInfo;
	Ogre::SOUND_PLAY_MODE   m_PlayMode;

	Ogre::SoundNode* m_pNode;
	Ogre::SoundData* m_pSource;


	CSoundInteract m_interact;
};

class CSoundCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CSound* m_pTarget;

	BIND_INFO		m_oldBindInfo;
	CString			m_strOldSoundFile;
	Ogre::SoundCreateInfo3D m_oldCreateInfo;
	int				m_iOldVol;

	BIND_INFO		m_newBindInfo;
	CString			m_strNewSoundFile;
	Ogre::SoundCreateInfo3D m_newCreateInfo;
	int				m_iNewVol;
};

