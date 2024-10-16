
#pragma once
#include <vector>
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreKeyFrameArray.h"
#include "OgreEntityMotionElement.h"
#include "OgreColourValue.h"

enum IDNTYPE
{
	IT_DEFAULT,
	IT_HOVER,
	IT_SELECTED,

};

class CMotionEventList
{
public:
	CMotionEventList() : m_fTime(-1.0f)
	{};
	float	m_fTime;
	std::vector<CString> m_EventNames;
};

class CObjectLayerInfo
{
public:
	CObjectLayerInfo(void);
	virtual ~CObjectLayerInfo(void);


	//��Ҫ���������ȫ��д�ڻ����У������н���������д������һЩά��UI״̬������
	CEffectUnit*						m_pEffect;
	Ogre::InterpolType                  m_PosInterpolType;
	float								m_start; //��ʼʱ��
	float								m_end; //pointerָ���ʱ�䣬��Motion��ʱ�䲻ͬ
	bool								m_hasStart;
	bool								m_hasEnd;
	int									m_anchor;
	Ogre::Vector3						m_offset;
	Ogre::Quaternion						m_rotate;
	Ogre::Vector3						m_scale;
	Ogre::MOTIONOBJTYPE_T				m_type;
	int									m_anim;
	int									m_priority;
	Ogre::POSTEFFECT_T					m_posteffect;
	Ogre::ColourValue						m_fs_color;
	Ogre::KeyFrameArray<Ogre::Vector4>	m_fs_colorkeys;
	Ogre::KeyFrameArray<float>			m_transparent_keys;
	std::vector<Ogre::PostproPhaseType>	m_customPE;
	Ogre::BlendMode                     m_OverlayBlendMode;
	CString                             m_OverlayTexPath;
	float								m_fOverlayAlhpa;

	Ogre::KeyFrameArray<Ogre::Vector3>	m_posKey;
	Ogre::KeyFrameArray<Ogre::Quaternion>	m_rotKey;
	Ogre::KeyFrameArray<Ogre::Vector3>	m_scaleKey;
	Ogre::KeyFrameArray<Ogre::Vector3>	m_rotAngleKey;
	//CString								m_submotion;

	bool								m_bVisible;

	int									m_iObjLayerLodLevel; //������ Lod ˮƽ��100��200��300������
	int									m_iMonLod;	//��ǰ��Ч���Ӧ��Montion��Lodˮƽ

	EFFECT_TYPE							m_eEffectType; //��������۵��������ӵı���
	int									m_SortIndex;
	bool								m_bFolded;
	bool								m_bSelfXparent;//�Ƿ�ֻ���Լ���͸����

	std::vector<CMotionEventList*>		m_events;

	static CObjectLayerInfo* CreateObjectLayer(Ogre::MOTIONOBJTYPE_T type, Ogre::POSTEFFECT_T posteffect);
	static Ogre::MotionElementData *createMotionElementData(Ogre::MOTIONOBJTYPE_T type);

	bool loadFromElementData(Ogre::MotionElementData *pelementdata);
	void saveToElementData(Ogre::MotionElementData *pelementdata, int layer_inex, int parent_inex);

	virtual CString		GetDisplayName() { return "��֪��"; };
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer, CTimeAixes* pTimeAix);

	virtual void		OnActive(int iMyLayerId, int block,  CTimeAixes* pTimeAix) {};
	virtual void		OnPosChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnSizeChange(int iMyLayerId, int iArea, CTimeAixes* pTimeAix);
	virtual void		OnSelected(int iMyLayerId,  CTimeAixes* pTimeAix) {};
	virtual void		OnDeleteFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix) {};
	virtual void		OnSelectFrame(int iMyLayerId, int iArea, CTimeAixes* pTimeAix) {};
	virtual void		OnCtrlDrag(int iMyLayerId, int iSrcArea, int iNewBoxPos,  CTimeAixes* pTimeAix) {};
	virtual void		RepresentAtTime(float fTime) {};
	virtual void		RepresentAtTime(float fTime, int lod) {}; //���lod��
	virtual void		RefreshRepresent(float fTime) {};
	virtual int			GetChildLevel();
	virtual bool		ReceiveChild(CObjectLayerInfo* pChildLayer) { return false; };
	virtual bool		GetAnchorList(std::vector<int>& outAnchor) { outAnchor.push_back(0); return true; };

	virtual CObjectLayerInfo*		CreateClone();

	virtual void		DrawIdentifier(Ogre::RenderLines *plines, float m_fTime, IDNTYPE type = IT_DEFAULT){};
	//virtual bool		NeedRepaint() { return false; };
	virtual void		SetObjLayerLodLevel(int level = 300) { m_iObjLayerLodLevel = level; }

	CObjectLayerInfo *getParent();
	void setParent(CObjectLayerInfo *p);
	size_t getNumChild();
	CObjectLayerInfo *getIthChild(size_t i);

	void updatePosInterpolType();

private:
	CObjectLayerInfo*					m_pParent;
	std::vector<CObjectLayerInfo *>m_Children;
};

// ------------------ [6/11/2008]
class CAnimLayerInfo : public CObjectLayerInfo
{
public:
	CAnimLayerInfo()	{ m_iObjLayerLodLevel = 100; }
	virtual CString		GetDisplayName();
	virtual void		CreateTimeLayer(CTimeLayer* pTimeLayer , CTimeAixes* pTimeAix);

	virtual void		OnActive(int iMyLayerId, int block, CTimeAixes* pTimeAix);

	virtual void		RepresentAtTime(float fTime) ;
	virtual void		RefreshRepresent(float fTime);
	virtual void		RepresentAtTime(float fTime, int lod);

};

//-----------------------
