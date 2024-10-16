
#ifndef __OgreDummyNode_H__
#define __OgreDummyNode_H__

#include "OgreLines.h"

namespace Ogre
{
	class _OgreExport DummyNode : public RenderLines
	{
		DECLARE_RTTI(DummyNode)

	public:
		DummyNode(DummyNodeData *psource=NULL);
		virtual void update(uint dtick);

		virtual void deleteThis()
		{
			delete this;
		}


		void DrawBox(Matrix4 tm, float fSize, ColorQuad color);
		~DummyNode();
	public:
		DummyNodeData *m_pSource;
	};
}

#endif


