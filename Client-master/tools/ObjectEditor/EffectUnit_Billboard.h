
#pragma once
#include "InteractObject.h"
#include "EffectUnit.h"
#include "BindInfo.h"
#include "EffectCommand.h"

#include "OgreColourValue.h"
#include "OgreBillboardData.h"

class CBillboardInteract : public CInteract
{
public:
	//CBillboardInteract();
	//virtual ~CBillboardInteract();
	//virtual bool OnLBtnDown(float x, float y, bool bForceOn);
	//virtual bool OnLBtnUp(float x, float y);
	//virtual bool OnMouseMove(float x, float y);
	//virtual bool Pick(float x, float y);
	//virtual void DoFrame(Ogre::uint dtick) ;

	//void Clear();
	//void Create(CBillboard* pTarget);

	//CBillboard* m_pTarget;

	//bool m_bHovering;
	//bool m_bCood;

	//COOR_ARROW m_tran;
	//CCoord* m_pCood;
};

class CBillboardCommand;

class CBillboard :
	public CEffectUnit, public CKeyFramePropertyObject
{
public:
	CBillboard(void);
	~CBillboard(void);

	struct BIL_FRAME : public CEffectKeyFrame
	{
		float UVRotat;
		float UScale;
		float VScale;
		float UOffset;
		float VOffset;
		float SeqFrame;
		Ogre::ColourValue color;
		float alpha;
		float Size;
		float Aspect;

		float MaskUVRotat;
		float MaskUScale;
		float MaskVScale;
		float MaskUOffset;
		float MaskVOffset;
		float MaskSeqFrame;

		// 104
		float SizeX;
		float SizeY;


		int lTick;
		BIL_FRAME()
		{
			UVRotat = 0.0f;
			UScale = 1.0f;
			VScale = 1.0f;
			UOffset = 0.0f;
			VOffset = 0.0f;
			SeqFrame = 0;
			Size = 100;
			Aspect = 1.0f;

			// 104
			SizeX = 100.0f;
			SizeY = 100.0f;

			color = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f);	
			alpha = 1.0f;

			MaskUVRotat = 0.0f;
			MaskUScale = 1.0f;
			MaskVScale = 1.0f;
			MaskUOffset = 0.0f;
			MaskVOffset = 0.0f;
			MaskSeqFrame = 1.0f;

			lTick = 0;

		};
		virtual void GetLerpKeyFrame(CEffectKeyFrame* pNextFrame, CEffectKeyFrame* pOutFrame)
		{
			BIL_FRAME* pNext = static_cast<BIL_FRAME*>(pNextFrame);
			BIL_FRAME* pOut = static_cast<BIL_FRAME*>(pOutFrame);
			LINEAR_LERP(UVRotat, pNext->UVRotat, pOut->UVRotat);
			LINEAR_LERP(UScale, pNext->UScale, pOut->UScale);
			LINEAR_LERP(VScale, pNext->VScale, pOut->VScale);
			LINEAR_LERP(UOffset, pNext->UOffset, pOut->UOffset);
			LINEAR_LERP(VOffset, pNext->VOffset, pOut->VOffset);
			LINEAR_LERP(SeqFrame, pNext->SeqFrame, pOut->SeqFrame);
			LINEAR_LERP(Size, pNext->Size, pOut->Size);
			LINEAR_LERP(Aspect, pNext->Aspect, pOut->Aspect);
			LINEAR_LERP(color.r, pNext->color.r, pOut->color.r);
			LINEAR_LERP(color.g, pNext->color.g, pOut->color.g);
			LINEAR_LERP(color.b, pNext->color.b, pOut->color.b);
			LINEAR_LERP(color.a, pNext->color.a, pOut->color.a);
			LINEAR_LERP(alpha, pNext->alpha, pOut->alpha);
		};
		virtual float GetTime()
		{
			return lTick * 0.001f;
		}
		virtual void SetTime(float fTime)
		{
			lTick = (int)(fTime * 1000);
		}
	};


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
	bool SetKFToSource(Ogre::BillboardData* pSource);
	bool SetKFFromSource(Ogre::BillboardData* pSource);

	virtual bool IsNew() { return m_bIsNew;};

	virtual CInteract* GetInteract() { return &m_interact; };
	virtual void DoFrame(Ogre::uint dtick) ;

	virtual float GetRange();

	virtual void ExecuteCommand(CEffectCommand* pCmd);
	virtual CEffectCommand* BeginRecordCommand();
	void EndRecordCommand(CEffectCommand* pCmd);
	virtual SCALE_TYPE		GetScaleType() { return ST_RANDOM_SCALING ; }; 	
	virtual CDocInfo*		GetDocInfo() { return &m_docInfo; };

	CKeyFramePropertyObject*	GetPropertyObject() { return NULL; };//{ return static_cast<CKeyFramePropertyObject*>(this); };

	virtual void*		GetKeyFramePropertyAddrByPtr(CString strPropertyName, BIL_FRAME* pFrame);


	virtual void*		GetInfoPropertyAddr(CString strPropertyName);
	virtual void*		GetKeyFramePropertyAddr(CString strPropertyName, int iKeyFrame);
	virtual void*		GetKeyFramePropertyAddrAtHotTime(CString strPropertyName);

	virtual int			GetSelectedKeyFrame();
	virtual int			GetKeyFrameCount();
	virtual float		GetKeyFrameTime(int iKeyFrame);

	virtual int			InsertKeyFrame(CEffectKeyFrame* pFrame, int iBefore);
	virtual void		DeleteKeyFrame(int index);
	virtual CEffectKeyFrame*	GetKeyFrame(int iKeyFrame);
	virtual CEffectKeyFrame*	CreateKeyFrame() { return new BIL_FRAME();};

	CDocInfo m_docInfo;		// 記錄文件是否被修改

	bool m_bIsNew;

	BIND_INFO				m_bindInfo;
	CString					m_strName;

	CString					m_strTexture;
	CString					m_strMaskTexture;
	CString					m_strLastMaskTexture;

	Ogre::BillboardDesc		m_desc;
	Ogre::BillboardDescEx	m_descEx;
	// 104
	Ogre::BillboardDesc104 m_descEx104;
	//
	std::vector<BIL_FRAME>	m_KeyFrames;
	BIL_FRAME				m_HotTimeFrameBuff;


	//bool					m_bVisible;
	Ogre::Billboard*		m_pNode;
	Ogre::BillboardData*	m_pSource;
	CString					m_strLastTexture;

	CBillboardInteract		m_interact;
};

class CBillboardCommand : public CEffectCommand
{
public:
	virtual CEffectUnit*	GetTarget();
	virtual bool			NeedRefresh();
	virtual CEffectCommand* GetInversCommand();
	virtual bool			IsIdentical();
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge);

	CBillboard* m_pTarget;

	Ogre::BillboardDesc m_oldDesc;
	Ogre::BillboardDesc m_newDesc;


	CString m_oldTex;
	CString m_newTex;
};
