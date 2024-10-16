
#ifndef __OgreEntityMotionData_H__
#define __OgreEntityMotionData_H__

#include "OgreEntityMotionElement.h"
#include "OgreResource.h"

namespace Ogre
{
	enum MOTIONLOOP_T
	{
		ML_ONCE,
		ML_LOOP,
		ML_ONCE_STOP
	};

	enum MOTIONOBJLIFE_T
	{
		MOL_START_TO_END,
		MOL_START_WITHOUT_END,
		MOL_INIT_TO_END,
		MOL_ALL_TIME
	};

	struct EVENT_ITEM
	{
		float fEventTime;
		std::vector<FixedString>eventNames;	
	};

	struct EVENT_LIST
	{
		std::vector<EVENT_ITEM> events;
	};

	struct _OgreExport BINDOBJ_T
	{
		BINDOBJ_T();

		int anchorid;
		Vector3 offset; //not used when motion obj
		Quaternion rotate;
		Resource *presource;
		BIND_ANIM_MODE animmode;
		FixedString animname;
		Vector3 scale;

		FixedString name;
		MOTIONOBJTYPE_T mottype;
		int layer_index;
		bool iscommonframe;
		bool isvisible;
		bool isfolded;
		bool extern_res; //ʹ���ⲿres��Դ�� ֻ����·��

		int effect_type;
		int sort_index;
	};

	/*
	struct _OgreExport OBJMOTION_T
	{
		OBJMOTION_T();

		MOTIONOBJLIFE_T	lifetype;
		float start;
		float end;

		MOTIONOBJTYPE_T type;
		int anim;
		int priority;
		int anchorid;
		Vector3 offset;
		Quaternion rotate;
		BINDOBJ_T obj;
		POSTEFFECT_T posteffect;

		int ver;
		char reserved[92];

		KeyFrameArray<Vector4> fs_keyframe;
		KeyFrameArray<float> transparent_keyframe;

		std::vector<PostproPhaseType> customPE;

		KeyFrameArray<Vector3> pos_key;
		KeyFrameArray<Quaternion> rot_key;
		KeyFrameArray<Vector3> scale_key;

		//std::string submotion;
		BIND_ROT_DATA rotdatatype;
		KeyFrameArray<Vector3> rotangle_key;

		int parent;

		int stObjLodLevel; //����� Lod ˮƽ

		int eventlist; 
		EVENT_LIST* pevent;
	};*/

	class _OgreExport MotionElementData : public Resource
	{
		DECLARE_RTTI_VIRTUAL(MotionElementData)
	public:
		MotionElementData();
		virtual ~MotionElementData();

		MOTIONOBJTYPE_T m_type;
		MOTIONOBJLIFE_T	m_lifetype;
		float m_fStart;
		float m_fEnd;
		int m_iObjLodLevel; //����� Lod ˮƽ
		int m_iParent;
		FixedString m_Name;

		virtual void _serialize(Archive &ar, int version);
	};

	class _OgreExport MotionAnimElementData : public MotionElementData
	{
		DECLARE_RTTI(MotionAnimElementData)
	public:
		int anim;
		int priority;

		virtual void _serialize(Archive &ar, int version);
	};

	struct FRAME_DATA
	{
		Vector3	pos;
		Vector3 scale;
		Quaternion rot;
		float	trans;
	};
	class _OgreExport MotionBindElementData : public MotionElementData
	{
		DECLARE_RTTI(MotionBindElementData)
	public:
		int							anchorid;
		int							anim;
		BINDOBJ_T					obj;
		KeyFrameArray<Vector3>		pos_key;
		KeyFrameArray<Quaternion>	rot_key;
		KeyFrameArray<Vector3>		scale_key;
		BIND_ROT_DATA				rotdatatype;
		KeyFrameArray<Vector3>		rotangle_key;
		bool						stFade;
		KeyFrameArray<float>		transparent_keyframe;

		MotionBindElementData();
		virtual ~MotionBindElementData();
		virtual void _serialize(Archive &ar, int version);
	};

	class _OgreExport MotionPostElementData : public MotionElementData
	{
		DECLARE_RTTI(MotionPostElementData)
	public:
		POSTEFFECT_T posteffect;

		KeyFrameArray<Vector4> fs_keyframe; //���⡢ƫɫ����
		KeyFrameArray<float> transparent_keyframe; //͸������ͷ��������

		//��Ӱ
		int stIntervalTime;
		int stImages;

		//overlay material
		BlendMode m_OverlayBlendMode;
		FixedString m_OverlayTexPath;
		float m_OverlayAlpha;

		virtual void _serialize(Archive &ar, int version);
	};

	class _OgreExport MotionEventElementData : public MotionElementData
	{
		DECLARE_RTTI(MotionEventElementData)
	
	public:
		~MotionEventElementData();

	public:
		std::vector<EVENT_ITEM *>m_Events;

		virtual void _serialize(Archive &ar, int version);
	};

	class _OgreExport EntityMotionData : public Resource
	{
		DECLARE_RTTI(EntityMotionData)
	public:
		EntityMotionData();

		float m_fTime;
		FixedString m_strName;
		//std::vector<EVENT_LIST> m_EventList;
		MOTIONLOOP_T	m_loop;

		int m_iModLodLevel; //ModelMotion �� Lod ˮƽ
		std::vector<MotionElementData *>m_Elements;

	protected:
		virtual ~EntityMotionData();
		virtual void _serialize(Archive &ar, int version);
	};
}

#endif