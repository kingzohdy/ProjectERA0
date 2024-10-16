
#pragma once
#include "OgrePrerequisites.h"

enum INTERACT_SHOW
{
	IS_HIDE,
	IS_ALWAYS_SHOW
};

class CInteract
{
public:
	CInteract(void);
	~CInteract(void);

	virtual bool			OnLBtnDown(float x, float y, bool bForceOn);
	virtual bool			OnLBtnUp(float x, float y);
	virtual bool			OnMouseMove(float x, float y) ;
	virtual bool			Pick(float x, float y) ;
	virtual void			Clear();
	virtual void			Create(CEffectUnit* pTarget);

	virtual void			DoFrame(Ogre::uint dtick);

	virtual void			OnActive()		{};
	virtual void			OnDeactive()	{};
	virtual Ogre::Quaternion GetOldRotation();
	virtual Ogre::Vector3	GetOldPosition();
	virtual Ogre::Vector3	GetOldScale();


	virtual void			SetNewRototion(Ogre::Quaternion rot);
	virtual void			SetNewPosition(Ogre::Vector3 pos);
	virtual void			SetNewScale(Ogre::Vector3 scale);

	CEffectUnit* m_pTarget;

	bool m_bHovering;
	bool m_bCood;

	COOR_ARROW m_tran;
	CCoord* m_pCood;

	static INTERACT_SHOW GetShowState() { return ms_showState; };
	static void SetShowState(INTERACT_SHOW show) { ms_showState = show; };

	static INTERACT_SHOW ms_showState;
};
