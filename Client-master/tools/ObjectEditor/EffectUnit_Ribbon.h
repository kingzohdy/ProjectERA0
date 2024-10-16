#pragma once
#include "EffectUnit.h"
#include "InteractObject.h"
#include "EffectCommand.h"
#include "BindInfo.h"

#include "OgreColourValue.h"
#include "OgreRibbonData.h"

class CRibbionInteract : public CInteract
{
public:

};

class CRibbon :
	public CEffectUnit
{
public:
	CRibbon(void);
	virtual ~CRibbon(void);


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
	void SetKfFromSource(Ogre::RibbonEmitterData* pSource);
	void SetKfToSource(Ogre::RibbonEmitterData* pSource);

	virtual SCALE_TYPE		GetScaleType() { return ST_GEOM_SCALING ; }; 	

	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand* BeginRecordCommand();
	virtual void EndRecordCommand(CEffectCommand* pCmd);
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };
	CDocInfo m_docInfo;

	struct RIB_FRAME
	{
		Ogre::ColourValue color;
		float above;
		float below;
		float uvrot;
		float uscale;
		float vscale;
		float uoffset;
		float voffset;

		float maskuvrot;
		float maskuscale;
		float maskvscale;
		float maskuoffset;
		float maskvoffset;


		long lTick;

		RIB_FRAME()
		{
			color = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
			above = 50.0f;
			below = 50.0f;

			uvrot = 0.0f;
			uscale = 1.0f;
			vscale = 1.0f;
			uoffset = 0.0f;
			voffset = 0.0f;

			maskuvrot = 0.0f;
			maskuscale = 1.0f;
			maskvscale = 1.0f;
			maskuoffset = 0.0f;
			maskvoffset = 0.0f;

			lTick = 0;
		}
	};
	CRibbionInteract m_interact;
	std::vector<RIB_FRAME> m_KeyFrames;
	Ogre::RibbonDesc m_desc;
	Ogre::RibbonDescEx m_descEx;

	BIND_INFO m_bindInfo;
	CString m_strName;
	CString m_strTexture;

	bool m_bSeeMask;
	CString m_strMaskTexture;
	CString	m_strLastMaskTexture;

	Ogre::RibbonEmitter* m_pNode;
	Ogre::RibbonEmitterData* m_pSource;
	CString m_strLastTexture;

	Ogre::RibbonSectionDesc m_Template;
};


class CRibbionCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CRibbon* m_pTarget;

	BIND_INFO m_oldBind;
	BIND_INFO m_newBind;

	Ogre::RibbonDesc m_oldDesc;
	Ogre::RibbonDesc m_newDesc;

	std::vector<CRibbon::RIB_FRAME> m_oldKf;
	std::vector<CRibbon::RIB_FRAME> m_newKf;

	CString m_oldTex;
	CString m_newTex;
};