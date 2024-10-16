
#ifndef __OgreEntityMotionElement_H__
#define __OgreEntityMotionElement_H__

#include "OgreKeyFrameArray.h"
#include "OgreSceneRendererPostEffect.h"
#include "OgreVector4.h"
#include "OgreLoadWrap.h"
#include <vector>

// begin specific for FlashChain
#include "OgreBeamEmitter.h"
// end specific for FlashChain

// begin specific for ForcePE
#include "OgreParticleEmitter.h"
// end specific for ForcePE

namespace Ogre
{
	enum LOD_LEVEL
	{
		MOTION_LOD_NOHANDLE = 10000, //不处理状态
		MOTION_LOD_HIGH = 300,
		MOTION_LOD_MIDDLE = 200,
		MOTION_LOD_LOW = 100
	};

	enum MOTIONOBJTYPE_T
	{
		MOT_MODEL,
		MOT_ANIM,
		MOT_ANIM_ADD,
		MOT_SOUND,
		MOT_POSTEFFECT,
		MOT_MODEL_2,
		MOT_EVENTS
	};

	enum BIND_ROT_DATA
	{
		BRD_QUAT,
		BRD_EULAR
	};

	enum POSTEFFECT_T
	{
		PE_MOTION_BLUR,
		PE_BLOOM,
		PE_GLOW,
		PE_GRAY,
		PE_SHAKE,
		PE_FSEFFECT,
		PE_TRANSPARENT,
		PE_ZOOM,
		PE_CUSTOM,
		PE_FROZEN,				//冰冻
		PE_REMANETSHODOW,		//残影
		PE_OFFCOLOR,			//偏色
		PE_DEPTHOFFIELD,		//景深
		PE_FOCUSBLUR,			//焦点模糊
		PE_STONE				//石化效果，支持溶解消散
	};

	enum BIND_ANIM_MODE
	{
		BAM_SELF = 0,  //播放自己的动作
		BAM_PARENT     //播放父entity的动作
	};

	struct DynamicObj
	{
		Resource* pres;
		BIND_ANIM_MODE animmode;
		MovableObject *pobject;
		int sortindex;
	};

	struct DelayDeleteObject 
	{
		DelayDeleteObject ()
		{
			object = NULL;
			delayTimeLeft = 5.0f;
		}

		~DelayDeleteObject ()
		{

		}

		Ogre::MovableObject *object;
		float delayTimeLeft;
	};


	class ObjectMotion;
	class _OgreExport MotionObjLife
	{
	public:
		virtual void OnPlay(Entity* pModel);
		virtual void OnEnd(Entity* pModel);
		virtual void OnStop(Entity* pModel);
		virtual void OnDelayStop (Entity* pModel, float seconed = 5.0f);
		virtual void OnRestart(Entity* pModel, float escaped);
		virtual void Update(Entity* pModel, float fMotionTime);


		ObjectMotion* m_pObjMotion;
		float m_fStart;
		bool  m_bHasStart;

		float m_fEnd;
		bool m_bHasEnd;
	};

	class _OgreExport ObjectMotion
	{
	public:
		//必实现
		virtual void InitObject(Entity* pModel) { m_state = 0; };
		virtual void StartObject(Entity* pModel) = 0;
		virtual void EndObject(Entity* pModel) = 0;
		virtual void StopObject(Entity* pModel) = 0;
		virtual void DelayStopObject (Entity* pModel, float second) {}
		virtual void UpdateData(float fEscaped, Entity* pModel) = 0; // montionTime - startTime

		// Just exist for FlashChain
		void SetFlashChain(bool bFlashChain, Vector3 target)
		{
			m_bFlashChain = bFlashChain;
			m_chainTarget = target;
		}
		bool IsFlashChain ()
		{
			return m_bFlashChain;
		}
		Vector3 GetTarget ()
		{
			return m_chainTarget;
		}

		// Just exist for ForcedPositon ParticleEmitter
		void SetForcePE(bool bForcePE, Vector3 pos, float range)
		{
			m_bForcePE = bForcePE;
			m_ForcePEPos = pos;
			m_ForcePERange = range;
		}
		bool IsForcePE ()
		{
			return m_bForcePE;
		}
		Vector3 GetForcePEPos ()
		{
			return m_ForcePEPos;
		}
		float GetForcePERange ()
		{
			return m_ForcePERange;
		}

		//尽量实现
		virtual void OnPause(bool bPause, float fEscaped, Entity* pModel) {};
		virtual ObjectMotion* GetParent() { return NULL; };
		virtual void SetParent(ObjectMotion* pParent) { };
		virtual void OnRestartObject(Entity* pModel, float escaped) {};

		//选实现
		virtual void SetLodLevel(LOD_LEVEL level = MOTION_LOD_HIGH) { m_iObjLodLevel = (int)level; }
		virtual void SetLodLevel(int level) { m_iObjLodLevel = level; }

		virtual void OnChildStart(ObjectMotion* pChild, Entity* pModel) {};
		virtual void OnChildEnd(ObjectMotion* pChild, Entity* pModel) {};
		virtual void OnChildStop(ObjectMotion* pChild, Entity* pModel) {};
		virtual void OnChildUpdate(ObjectMotion* pChild, float fChildTime, Entity* pModel) {};

		ObjectMotion();
		virtual ~ObjectMotion();

		int m_state;
		MotionObjLife* m_pLifeCtrl;
		ModelMotion *m_pOwner;
		MotionElementData *m_pElementData;

		bool m_bFlashChain;
		Ogre::Vector3 m_chainTarget;

		bool m_bForcePE;
		Ogre::Vector3 m_ForcePEPos;
		float m_ForcePERange;

		virtual MotionObjLife* GetLifeCtrl() { return m_pLifeCtrl;};
		virtual int GetState() { return m_state;};

		int m_iObjLodLevel; //每个对象层的 Lod 水平
	};

	class _OgreExport BindOjbectMotion : public ObjectMotion
	{
	public:
		virtual void InitObject(Entity* pModel);
		virtual void StartObject(Entity* pModel);
		virtual void EndObject(Entity* pModel);
		virtual void StopObject(Entity* pModel){};
		virtual void DelayStopObject (Entity* pModel, float second) {}
		virtual void UpdateData(float fEscaped, Entity* pModel){};

		//virtual void RestartObject(Entity* pModel){};
		virtual ~BindOjbectMotion();
		int			m_iAnchorid;
		Vector3		m_vOffset;
		Quaternion  m_qRotate;
		DynamicObj* m_pObj;
		int			m_iSubanim;
	};

	struct FRAME_DATA;
	class _OgreExport BindOjbect2Motion : public ObjectMotion
	{
	public:
		virtual void InitObject(Entity* pModel);
		virtual void StartObject(Entity* pModel);
		virtual void EndObject(Entity* pModel);
		virtual void StopObject(Entity* pModel);
		virtual void DelayStopObject (Entity* pModel, float second);
		virtual void UpdateData(float fEscaped, Entity* pModel);
		virtual void OnPause(bool bPause, float fEscaped, Entity* pModel);
		virtual void SetNodePause(SceneNode* pNode, bool bPause, float ftime);
		virtual void OnRestartObject(Entity* pModel, float escaped);
		bool GetDataOnTime(float fTime, FRAME_DATA& data);

		BindOjbect2Motion();
		virtual ~BindOjbect2Motion();

		DynamicObj*					m_pObj;
		int							m_iSubanim;
		std::string					m_strSubMotion;
		BindOjbect2Motion*			m_pParent;

		virtual ObjectMotion* GetParent() { return m_pParent; };
		virtual void SetParent(ObjectMotion* pParent);
		virtual void OnChildStart(ObjectMotion* pChild, Entity* pModel);
		virtual void OnChildEnd(ObjectMotion* pChild, Entity* pModel);
		virtual void OnChildStop(ObjectMotion* pChild, Entity* pModel);
		virtual void OnChildUpdate(ObjectMotion* pChild, float fChildTime, Entity* pModel);
	};

	class _OgreExport AnimOjbectMotion : public ObjectMotion
	{
	public:
		virtual void InitObject(Entity* pModel);
		virtual void StartObject(Entity* pModel);
		virtual void EndObject(Entity* pModel);
		virtual void StopObject(Entity* pModel);
		virtual void DelayStopObject (Entity* pModel, float second);
		virtual void UpdateData(float fEscaped, Entity* pModel){};
		virtual void OnRestartObject(Entity* pModel, float escaped);
	};

	class _OgreExport PostObjMotion : public ObjectMotion, public LoadWrap
	{
	public:
		PostObjMotion() : m_ShakeChannel(-1), m_hOverlayTexLoad(NULL){};

		virtual void InitObject(Entity* pModel);
		virtual void StartObject(Entity* pModel);
		virtual void EndObject(Entity* pModel);
		virtual void StopObject(Entity* pModel);
		virtual void DelayStopObject (Entity* pModel, float second);
		virtual void UpdateData(float fEscaped, Entity* pModel);
		//virtual void RestartObject(Entity* pModel){};
		virtual ~PostObjMotion();

	private:
		void ResourceLoaded(Resource *rec, ResourceHandle h);

	private:
		int			m_ShakeChannel;
		Entity		*m_pModel;
		ResourceHandle	m_hOverlayTexLoad;
	};
}

#endif