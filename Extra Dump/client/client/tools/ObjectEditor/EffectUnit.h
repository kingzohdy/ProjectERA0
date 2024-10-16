
#pragma once
#include "OgrePrerequisites.h"

class CEffectKeyFrame
{
public:
	virtual void GetLerpKeyFrame(CEffectKeyFrame* pNextFrame, CEffectKeyFrame* pOutFrame) = 0;

	virtual float GetTime() = 0;

	virtual void SetTime(float fTime) = 0;

	virtual void CopyFrameData(CEffectKeyFrame* pOutFrame)
	{
		GetLerpKeyFrame(this, pOutFrame);
	}
};

class CKeyFramePropertyObject
{
public:
	float	m_fHotTime;
	CEffectKeyFrame* m_pTempFrame;

	virtual void*		GetInfoPropertyAddr(CString strPropertyName)	=	0;
	virtual void*		GetKeyFramePropertyAddr(CString strPropertyName, int iKeyFrame)	 =	0;
	virtual void*		GetKeyFramePropertyAddrAtHotTime(CString strPropertyName) = 0;

	virtual int			GetSelectedKeyFrame() = 0;
	virtual int			GetKeyFrameCount() = 0;
	virtual float		GetKeyFrameTime(int iKeyFrame) = 0;
	virtual CEffectKeyFrame*	GetKeyFrame(int iKeyFrame) = 0;

	virtual int			InsertKeyFrame(CEffectKeyFrame* pFrame, int iBefore) = 0;
	virtual void		DeleteKeyFrame(int index) = 0;
	virtual CEffectKeyFrame*	CreateKeyFrame() = 0;

	virtual void 		SetKeyFramePos(int index, float fTime);
	virtual void		CopyKeyFrame(int from, int to);
	virtual int			AddKeyFrame(float fTime);
	virtual float		GetHotTime();
	virtual void		SetHotTime(float fTime);



};

#define	BEGIN_NAME_CHECK(name)	{CString strCmpName = name;
#define RETURN_PTR_IF(name, owner_ptr) if(strCmpName == #name) return (void*)(&((owner_ptr)->name));
#define END_NAME_CHECK()	return NULL;}

#define LINEAR_LERP(start, end, pos) ((start) + ((end) - (start)) * (pos))


class CEffectCommand;
class CEffectUnit
{
public:
	CEffectUnit(void);
	virtual ~CEffectUnit(void);

	bool m_bModified;
public:
	virtual Ogre::SceneNode*			GetNode() = 0;
	virtual void					ReleaseNode() = 0;
	virtual CBCGPDockingControlBar* CreateBar(CBCGPFrameWnd* pParent) = 0;
	virtual Ogre::Resource*			GetSource() { return NULL;};

	virtual Ogre::SceneNode*			CreateNode(){return NULL;}; 

	virtual void					UpdateSource() {};
	virtual CDocInfo*		GetDocInfo() = 0;


	virtual CString			GetName() = 0;
	virtual void			SetName(CString strName) = 0;
	virtual CString			GetResPath() { return ""; };
	virtual void			SetResPath(CString strPath) {};

	virtual bool			Create() = 0;
	virtual bool			Destroy() = 0;
	virtual bool			IsVisible();
	virtual void			SetVisible(bool visible);

	virtual bool			LoadEffect(CString path) = 0;
	virtual bool			SaveEffect(CString path) = 0;

	virtual bool			LoadFromChunk(Ogre::Resource* pChunk) = 0;

	virtual BIND_INFO*		GetBindInfo() = 0;
	virtual void			SetBindInfo(BIND_INFO* bindinfo) = 0;
	virtual EFFECT_TYPE		GetType() = 0;

	virtual CInteract*		GetInteract() { return NULL;};
	virtual void			DoFrame(Ogre::uint dtick) {};

	virtual bool			IsNew() { return false;};
	virtual bool			IsModified() { return m_bModified; };

	virtual void			ExecuteCommand(CEffectCommand* pCmd) {};
	virtual float			GetRange()	{ return 0.0f; };

	virtual CEffectCommand* BeginRecordCommand()  = 0;
	virtual void			EndRecordCommand(CEffectCommand* pCmd) = 0;

	virtual Ogre::Entity*	GetModel()	{ return NULL; };
	virtual bool			IsModel()	{ return false; };

	virtual void			BindToModel(Ogre::Entity* pModel, int anchor);
	virtual void			UnbindFromModel(Ogre::Entity* pModel);
	virtual SCALE_TYPE		GetScaleType() { return ST_NO_SCALING; };	
	virtual bool			HasKeyFrame() { return false; };

	virtual void			OnBelongingRemoved(CEffectUnit* pUnit) {};
	virtual CKeyFramePropertyObject*	GetPropertyObject() { return NULL; };

	CEffectUnit*			GetOwnerEffect() { return m_pOwnerEffect; };
	void					SetOwnerEffect(CEffectUnit* pEffect ) { m_pOwnerEffect = pEffect; };


	CEffectUnit*			m_pOwnerEffect;
	bool					m_bVisible;

	//virtual CInteractObject*	GetInteractObject();
	//virtual void				SetInteractObject(CInteractObject* pInteractObject);

	//CInteractObject*	m_pExternInteractObject;


	CString				m_strDisplayName;
	CString				m_strName;
	Ogre::SceneNode*		m_pNode;

	static CEffectUnit*			CreateEffect(EFFECT_TYPE type);
	static CEffectUnit*			LoadEffectFromFile(CString strPath);
	//static Ogre::Resource*		GetResourceFromChunk(Ogre::ACTOR_BIND_T bindtype, Ogre::Resource* pChu);
	//static Ogre::Resource*		GetResourceFromChunk(Ogre::Resource* pChu);
	static bool					CanEdit(Ogre::Resource *pres);
	static CEffectUnit*			LoadEffectFromChunk(Ogre::Resource* pChunk);



};

