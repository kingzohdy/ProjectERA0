#pragma once
#include "EffectUnit.h"
#include "InteractObject.h"
#include "EffectCommand.h"
#include "BindInfo.h"

#include "OgreDecalData.h"

class CDecal;
class CDecalInteract : public CInteract
{
public:
	CDecalInteract();
	~CDecalInteract();
	virtual bool Pick(float x, float y);
	virtual void DoFrame(Ogre::uint dtick) ;

	void Clear();
	void Create(CEffectUnit* pTarget);

	Ogre::Billboard* m_pIdn;
};

class CDecalCommand;


class CDecal :	public CEffectUnit
{

public:

	struct DCL_FRAME
	{
		float size;
		float aspect;
		Ogre::ColourValue color;
		float alpha;
		int lTick;

		float fUScale;
		float fVScale;
		float fUOff;
		float fVOff;
		float fUVRotate;
		int nSequenceFrame;

		float fMaskUScale;
		float fMaskVScale;
		float fMaskUOff;
		float fMaskVOff;
		float fMaskUVRotate;


		DCL_FRAME()
		{
			size = 20.0f;
			aspect = 1.0f;
			color = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);
			alpha = 1.0f;
			lTick = 0;

			fUScale = 1.0f;
			fVScale = 1.0f;
			fUOff = 0.0f;
			fVOff = 0.0f;
			fUVRotate = 0.0f;
			nSequenceFrame = 0;

			fMaskUScale = 1.0f;
			fMaskVScale = 1.0f;
			fMaskUOff = 0.0f;
			fMaskVOff = 0.0f;
			fMaskUVRotate = 0.0f;
		}
	};

	CDecal(void);
	virtual ~CDecal(void);

	virtual Ogre::SceneNode* GetNode();
	virtual void ReleaseNode();
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent);
	virtual Ogre::Resource* GetChunk();
	virtual Ogre::Resource* GetSource() { return m_pSource;};
	virtual Ogre::SceneNode* CreateNode(); 
	virtual void RefreshNode();


	virtual CString GetName();
	virtual void SetName(CString strName) { m_strName = strName;};

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

	virtual void UpdateSource();
	void SetKfFromSource(Ogre::DecalData* pSource);
	void SetKfToSource(Ogre::DecalData* pSource);

	virtual bool IsNew() { return m_bIsNew;};

	virtual CInteract* GetInteract() { return &m_interact; };
	virtual void DoFrame(Ogre::uint dtick) ;

	virtual SCALE_TYPE		GetScaleType() { return ST_GEOM_SCALING ; }; 	

	float GetRange();

	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand* BeginRecordCommand();
	virtual void EndRecordCommand(CEffectCommand* pCmd);
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };
	CDocInfo m_docInfo;

	bool m_bIsNew;

	BIND_INFO m_bindInfo;
	CString m_strName;

	CString m_strTexture;
	CString m_strMaskTexture;

	Ogre::DecalDesc m_desc;
	std::vector<DCL_FRAME> m_KeyFrames;

	//bool m_bVisible;

	Ogre::DecalNode* m_pNode;
	Ogre::DecalData* m_pSource;
	CString m_strLastTexture;

	CDecalInteract m_interact;

	bool m_bSeeMask;
};

class CDecalCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CDecal* m_pTarget;

	Ogre::DecalDesc m_oldDesc;
	Ogre::DecalDesc m_newDesc;

	std::vector<CDecal::DCL_FRAME> m_oldKf;
	std::vector<CDecal::DCL_FRAME> m_newKf;

	BIND_INFO m_oldBindInfo;
	BIND_INFO m_newBindInfo;

	CString m_oldTex;
	CString m_newTex;
};