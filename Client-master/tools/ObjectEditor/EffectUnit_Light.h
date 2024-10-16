
#pragma once
#include "InteractObject.h"
#include "EffectUnit.h"
#include "BindInfo.h"
#include "EffectCommand.h"

#include "OgreRenderTypes.h"
#include "OgreColourValue.h"
#include "OgreLightData.h"

class CLight;
class CLightInteract : public CInteract
{
public:
	CLightInteract();
	~CLightInteract();
	//virtual bool OnLBtnDown(float x, float y, bool bForceOn);
	//virtual bool OnLBtnUp(float x, float y);
	//virtual bool OnMouseMove(float x, float y);
	//virtual bool Pick(float x, float y) ;
	virtual void DoFrame(Ogre::uint dtick) ;

	void Clear();
	void Create(CEffectUnit* pTarget);

	void DrawRange();

	//CLight* m_pTarget;

	//bool m_bHovering;
	//bool m_bCood;

	//COOR_ARROW m_tran;
	//CCoord* m_pCood;
	Ogre::MovableObject* m_pIdn;
};
class CLightCommand;
class CLight :
	public CEffectUnit
{
public:
	CLight(void);
	virtual ~CLight(void);

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
	CEffectCommand* BeginRecordCommand();
	void EndRecordCommand(CEffectCommand* pCmd);

	virtual void UpdateSource();
	virtual	void DoFrame(Ogre::uint dtick);
	virtual bool IsNew() { return m_bIsNew;};

	virtual float GetRange() { return 20.0f; };
	virtual SCALE_TYPE		GetScaleType() { return ST_NO_SCALING ; }; 	
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };
	CDocInfo m_docInfo;
	bool m_bIsNew;

	BIND_INFO m_bindInfo;
	CString m_strName;
	Ogre::LightType m_Type;
	Ogre::ColourValue m_Color;
	float m_Attenuation;
	float m_Range;

	//bool m_bVisible;
	//Ogre::LightChunk* m_pTmpChunk;

	Ogre::Light* m_pNode;
	Ogre::LightData* m_pSource;

	CLightInteract m_interact;
};

class CLightCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CLight* m_pTarget;

	BIND_INFO		m_oldBindInfo;
	Ogre::LightType	m_oldType;
	Ogre::ColourValue	m_oldColor;
	float			m_oldAttenuation;
	float			m_oldRange;

	BIND_INFO		m_newBindInfo;
	Ogre::LightType	m_newType;
	Ogre::ColourValue	m_newColor;
	float			m_newAttenuation;
	float			m_newRange;

};
