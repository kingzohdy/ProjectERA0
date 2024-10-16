
#ifndef __OgreDummyNodeData_H__
#define __OgreDummyNodeData_H__

#include "OgreResource.h"

namespace Ogre
{
	class DummyNodeDesc
	{
	public:
		int test;
	};

	class _OgreExport DummyNodeData: public Resource
	{
		DECLARE_RTTI(DummyNodeData)
	public:
		DummyNodeData();

	protected:
		~DummyNodeData();
		virtual void _serialize(Archive &ar, int version);

	public:
		DummyNodeDesc m_Desc;
		bool			m_bDrawBox;
	};
}

#endif