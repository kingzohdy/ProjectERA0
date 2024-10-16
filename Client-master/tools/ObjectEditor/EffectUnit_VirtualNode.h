
#pragma once
#include "EffectUnit.h"
#include "EffectCommand.h"
#include "InteractObject.h"
#include "BindInfo.h"

#include "OgreDummyNodeData.h"

class CVirtualNode :
	public CEffectUnit
{
public:
	CVirtualNode(void);
	virtual ~CVirtualNode(void);


	virtual Ogre::SceneNode* GetNode();
	virtual void ReleaseNode();
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent);
	virtual Ogre::Resource* GetChunk();
	virtual Ogre::Resource* GetSource() { return m_pSource;};

	virtual Ogre::SceneNode* CreateNode(); 
	virtual void RefreshNode();

	virtual CString GetName();
	virtual void SetName(CString strName);
	virtual bool Create();
	virtual bool Destroy();
	//virtual bool IsVisible();
	//virtual void SetVisible(bool visible);

	virtual bool LoadEffect(CString path);
	virtual bool SaveEffect(CString path);

	float GetRange();

	virtual bool LoadFromChunk(Ogre::Resource* pChunk);
	virtual bool IsNew() { return m_bIsNew;};
	bool m_bIsNew;

	virtual BIND_INFO* GetBindInfo();
	virtual void SetBindInfo(BIND_INFO* bindinfo);
	virtual EFFECT_TYPE GetType();
	virtual CInteract* GetInteract() { return &m_interact; };
	virtual void DoFrame(Ogre::uint dtick) ;
	virtual void UpdateSource();
	void SetKfFromSource(Ogre::DummyNodeData* pSource);
	void SetKfToSource(Ogre::DummyNodeData* pSource);

	virtual SCALE_TYPE		GetScaleType() { return ST_GEOM_SCALING ; }; 	

	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand* BeginRecordCommand();
	virtual void EndRecordCommand(CEffectCommand* pCmd);
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };
	CDocInfo m_docInfo;

	struct VIR_FRAME
	{
		long lTick;
		VIR_FRAME()
		{
			lTick = 0;
		}
	};
	CInteract m_interact;
	std::vector<VIR_FRAME> m_KeyFrames;
	Ogre::DummyNodeDesc m_desc;
	BIND_INFO m_bindInfo;
	CString m_strName;

	bool m_bShowBox;

	//bool m_bVisible;

	Ogre::DummyNode* m_pNode;
	Ogre::DummyNodeData* m_pSource;


};


class CVirtualNodeCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CVirtualNode* m_pTarget;

	BIND_INFO m_oldBind;
	BIND_INFO m_newBind;

	Ogre::DummyNodeDesc m_oldDesc;
	Ogre::DummyNodeDesc m_newDesc;

	std::vector<CVirtualNode::VIR_FRAME> m_oldKf;
	std::vector<CVirtualNode::VIR_FRAME> m_newKf;

	CString m_oldTex;
	CString m_newTex;
};
