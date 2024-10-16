
#pragma once
#include "EffectUnit.h"
#include "InteractObject.h"
#include "BindInfo.h"
#include "EffectCommand.h"

#include "OgreBeamEmitterData.h"
#include "OgreKeyFrameArray.h"

class CBeamInteract : public CInteract
{
};

class CBeam :
	public CEffectUnit
{
public:
	struct BEA_FRAME
	{
		Ogre::Vector3 startrange;
		Ogre::Vector3 endrange;
		Ogre::Vector3 endpos;
		float sizevar;
		float life;
		float lifevar;
		float emitrate;

		int tick;

		BEA_FRAME()
		{
			startrange = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			endrange = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			endpos = Ogre::Vector3(100.0f, 0.0f, 0.0f);

			sizevar = 0;
			life = 2.0f;
			lifevar = 0;
			emitrate = 2.5f;
			tick = 0;
		}

	};

public:
	CBeam(void);
	~CBeam(void);

	virtual Ogre::SceneNode* GetNode();
	virtual void ReleaseNode();
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent);
	virtual Ogre::Resource* GetChunk();

	virtual Ogre::SceneNode* CreateNode();
	virtual void RefreshNode();
	virtual void UpdateSource();
	virtual float GetRange();
	virtual CString GetName();
	virtual void SetName(CString strName);
	virtual bool Create();
	virtual bool Destroy();
	//virtual bool IsVisible();
	//virtual void SetVisible(bool visible);

	virtual bool LoadEffect(CString path);
	virtual bool SaveEffect(CString path);

	virtual void DoFrame(Ogre::uint dtick) ;

	virtual bool LoadFromChunk(Ogre::Resource* pChunk);

	virtual CInteract*		GetInteract() { return &m_interact; };

	virtual BIND_INFO* GetBindInfo();
	virtual void SetBindInfo(BIND_INFO* bindinfo);
	virtual EFFECT_TYPE GetType();


	virtual bool IsNew() { return m_bIsNew;};
	virtual CEffectCommand* BeginRecordCommand();
	virtual void EndRecordCommand(CEffectCommand* pCmd);
	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual Ogre::Resource* GetSource() { return m_pSource;};
	virtual SCALE_TYPE		GetScaleType() { return ST_NO_SCALING ; }; 	

	bool ValidateDesc(Ogre::PBeamEmitterDesc& desc);

	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };

	void SetKfFromSource(Ogre::BeamEmitterData* pSource);
	void SetKfToSource(Ogre::BeamEmitterData* pSource);

	CDocInfo m_docInfo;

	bool m_bIsNew;

	BIND_INFO m_bindInfo;
	CString m_strName;
	CString m_strTexture;
	CString m_strMaskTexture;

	std::vector<BEA_FRAME>			m_KeyFrames;
	Ogre::PBeamEmitterDesc			m_desc;
	Ogre::KeyFrameArray<float>			m_WidthKey;
	Ogre::KeyFrameArray<Ogre::ColourValue>	m_ColorKey;
	Ogre::KeyFrameArray<float>			m_AlphaKey;

	Ogre::BeamEmitterData* m_pSource;
	Ogre::BeamEmitter* m_pNode;
	//bool m_bVisible;
	CBeamInteract m_interact;
	CString m_strLastTexture;
	CString m_strLastMaskTexture;
};

class CBeamCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CBeam* m_pTarget;

	Ogre::PBeamEmitterDesc m_oldDesc;
	Ogre::PBeamEmitterDesc m_newDesc;

	std::vector<CBeam::BEA_FRAME>		m_oldFrame;
	std::vector<CBeam::BEA_FRAME>		m_newFrame;

	CString m_oldTex;
	CString m_newTex;

	CString m_oldMaskTex;
	CString m_newMaskTex;
};

