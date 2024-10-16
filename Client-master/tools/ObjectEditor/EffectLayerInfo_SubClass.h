
#pragma once
#include "EffectLayerInfo.h"

//动态模糊、黑白、镜头抖动、全屏柔化、眩光
class CNormalPostLayerInfo : public CObjectLayerInfo
{
public:
	CNormalPostLayerInfo(void);
	virtual ~CNormalPostLayerInfo(void);

	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);
	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);
};

//镜头抖动 
class CCameraShakeLayerInfo : public CObjectLayerInfo
{
public:
	CCameraShakeLayerInfo();
	~CCameraShakeLayerInfo();

	virtual CString GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);
	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);
	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
};

//泛色
class CFSEffectLayerInfo : public CObjectLayerInfo
{
public:
	CFSEffectLayerInfo(void);
	virtual ~CFSEffectLayerInfo(void);

	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);
	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);

	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);

};

//焦距拉伸使用透明度的数组保存数据
class CZoomLayerInfo : public CObjectLayerInfo
{
public:
	CZoomLayerInfo(void);
	virtual ~CZoomLayerInfo(void);


	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);
	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);

	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
};

//角色透明
class CTransLayerInfo :
	public CObjectLayerInfo
{
public:
	CTransLayerInfo(void);
	virtual ~CTransLayerInfo(void);

	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);
	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);

	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
};

class CEventLayerInfo :	public CObjectLayerInfo
{
public:
	CEventLayerInfo();
	virtual ~CEventLayerInfo();

	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);

	virtual void		OnActive(int iMyLayerId, int block,  CTimeAixes* pTimeAix);
	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnSizeChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnSelected(int iMyLayerId,  CTimeAixes* pTimeAix) ;
	virtual void		OnDeleteFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix) ;
	virtual void		OnSelectFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix) ;
};

class CEffect2LayerInfo : public CObjectLayerInfo
{
public:
	CEffect2LayerInfo(void);
	virtual ~CEffect2LayerInfo(void);

	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);
	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);

	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnDeleteFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnSelectFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnSelected(int iMyLayerId,  CTimeAixes* pTimeAix);
	virtual void		OnCtrlDrag(int iMyLayerId, int iSrcArea, int iNewBoxPos,  CTimeAixes* pTimeAix);
	virtual CObjectLayerInfo*	CreateClone();
	virtual bool		ReceiveChild(CObjectLayerInfo* pChildLayer);
	virtual bool		GetAnchorList(std::vector<int>& outAnchor);

	int		InsertMonKeyFrameAtPos(Ogre::uint tick, int iCopyFrom = -1);
	bool	CanMerge();
	void	OverwriteMotionFrame();
	void	OverwriteEffectFrame();
	void	MergeFrame();
	bool	ApartKeyFrame();

	bool	CanShow(float fTime);
	Ogre::SceneNode*	GetParentNode();

	virtual void		RepresentAtTime(float fTime, int lod);
	virtual void		RepresentAtTime(float fTime);
	virtual void		RefreshRepresent(float fTime);

	virtual void		DrawIdentifier(Ogre::RenderLines* pDbg, float fTime, IDNTYPE type = IT_DEFAULT);
	void				DrawBox(Ogre::RenderLines* pDbg, Ogre::Matrix4 tm, float fSize, Ogre::ColorQuad color);
	void				DrawTrack(Ogre::RenderLines* pDbg, Ogre::ColorQuad color);

	void				RefreshObjInfo(float fTime);
	void				PlayUnknownObjAnim();
	void	GetSRTAtTime(float fTime, Ogre::Vector3& pos, Ogre::Quaternion& rot, Ogre::Vector3& scale,float& trans, Ogre::Vector3& rotangle);
	void	GetWorldSRTAtTime(float fTime, Ogre::Vector3& pos, Ogre::Quaternion& rotByAngle, Ogre::Vector3& scale);

	bool	m_bShowing;
};