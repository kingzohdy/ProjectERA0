
#ifndef __OgreEntityMotionEvent_H__
#define __OgreEntityMotionEvent_H__

#include "OgreEntityMotionElement.h"
#include "OgreEntityMotionData.h"

namespace Ogre
{
	class MotionEvent
	{
	public:
		MotionEvent(void) : m_fTrigTime(-1.0f), m_bTrigged(false)
		{};

		void TriggerMe(Entity* pModel);
		void Reset();
		bool IsTrigged() { return m_bTrigged; };
		float GetTime() { return m_fTrigTime; };
		std::vector<FixedString>	m_eventNames;
		float						m_fTrigTime;
		bool						m_bTrigged;

	};

	class MotionEventParam
	{
	public:
		MotionEventParam() : m_pModel(NULL), m_uParam1(0), m_uParam2(0)
		{
		}

		Entity*		m_pModel;
		FixedString	m_strEventName;
		uint			m_uParam1;
		uint			m_uParam2;
	};

	class MotionEventHandler
	{
	public:
		virtual int	OnEvent(MotionEventParam* pParam) = 0;
	};

	class _OgreExport EventTriggerObjectMotion : public ObjectMotion
	{
	public:
		~EventTriggerObjectMotion();
		virtual void InitObject(Entity* pModel);
		virtual void StartObject(Entity* pModel);
		virtual void EndObject(Entity* pModel) ;
		virtual void StopObject(Entity* pModel) ;
		virtual void DelayStopObject (Entity* pModel, float second);
		virtual void UpdateData(float fEscaped, Entity* pModel); // montionTime - startTime
		virtual void OnRestartObject(Entity* pModel, float escaped);

		void	LoadFromEventList(MotionEventElementData *peventdata);
		void	Clear();
		std::vector<MotionEvent*>	m_eventList;
	};
}

#endif