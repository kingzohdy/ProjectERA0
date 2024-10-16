
#pragma once
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreEntityMotionElement.h"

struct BIND_INFO
{
	int boneid;
	Ogre::Vector3 offset;
	Ogre::Quaternion rotate;
	Ogre::Vector3 scale;

	bool bDynamic;
	bool bCommKeyFrame;		// 是否和动作层用同一套关键帧
	bool bIntegrated;

	Ogre::BIND_ANIM_MODE animmode;
	CString				animname;


	BIND_INFO() : offset(0.0f, 0.0f, 0.0f), rotate(0.0f, 0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) 
	{
		rotate.identity();
		bCommKeyFrame = false;
		bDynamic = false;
		bIntegrated = true;
		animmode = Ogre::BAM_SELF;
		animname = "";

	};
	BIND_INFO& operator = (const BIND_INFO& op)
	{
		boneid = op.boneid;
		offset = op.offset;
		bDynamic = op.bDynamic;
		rotate = op.rotate;
		scale = op.scale;
		bCommKeyFrame = op.bCommKeyFrame;
		bIntegrated = op.bIntegrated;
		animmode = op.animmode;
		animname = op.animname;
		return *this;
	};

	bool operator == (const BIND_INFO& op)
	{
		if(boneid == op.boneid
			&& offset == op.offset 
			&& bDynamic == op.bDynamic
			&& rotate == op.rotate
			&& scale == op.scale
			&& bCommKeyFrame == op.bCommKeyFrame
			&& bIntegrated == op.bIntegrated
			&& animmode == op.animmode
			&& animname == op.animname )
		{
			return true;
		}
		return false;
	};

	bool operator != (const BIND_INFO& op)
	{
		if(boneid == op.boneid
			&& offset == op.offset 
			&& bDynamic == op.bDynamic
			&& rotate == op.rotate
			&& scale == op.scale
			&& bCommKeyFrame == op.bCommKeyFrame
			&& bIntegrated == op.bIntegrated
			&& animmode == op.animmode
			&& animname == op.animname )
		{
			return false;
		}
		return true;
	};
};


