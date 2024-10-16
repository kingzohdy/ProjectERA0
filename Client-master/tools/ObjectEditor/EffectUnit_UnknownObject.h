#pragma once
#include "InteractObject.h"
#include "EffectUnit.h"
#include "BindInfo.h"
#include "EffectCommand.h"

class CUnknownObjectInteract : public CInteract
{
public:
};

class CUnknownObject : public CEffectUnit
{
public:
	CUnknownObject(void);
	virtual ~CUnknownObject(void);


	virtual Ogre::SceneNode* GetNode();
	virtual void ReleaseNode();
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent);
	virtual Ogre::Resource* GetChunk();
	virtual Ogre::Resource* GetSource() { return m_pSource;};
	virtual Ogre::SceneNode* CreateNode(); 
	virtual void RefreshNode();

	virtual CString GetName();
	virtual void SetName(CString strName) { m_strName = strName; };
	virtual CString			GetResPath() { return m_strResPath; };
	virtual void			SetResPath(CString strPath) { m_strOldResPath = m_strResPath = strPath; };

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

	virtual Ogre::Entity*	GetModel();
	virtual bool			IsModel();
	virtual SCALE_TYPE		GetScaleType() { return ST_RANDOM_SCALING ; }; 	
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };

	void ReloadSource();
	CDocInfo m_docInfo;
	bool m_bIsNew;

	BIND_INFO m_bindInfo;
	CString m_strName;
	CString m_strResPath;

	CString m_strOldResPath;

	//bool m_bVisible;

	Ogre::SceneNode* m_pNode;
	Ogre::Resource* m_pSource;
	Ogre::Entity* m_pModel;

	CUnknownObjectInteract m_interact;
};

class CUnknownObjectCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CUnknownObject* m_pTarget;

	BIND_INFO		m_oldBindInfo;
	BIND_INFO		m_newBindInfo;

};

