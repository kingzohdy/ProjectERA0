
#ifndef __GroundGrid_H__
#define __GroundGrid_H__

#include "OgreLines.h"

namespace Ogre
{
	class _OgreExport GroundGrid : public RenderLines
	{
		DECLARE_RTTI(GroundGrid)

	public:
		GroundGrid(bool ztest=false);
		~GroundGrid();

		void setGridGround(float width, float length, int xgrids, int ygrids);
		void setLineColor(RGBA color);
		void setBorderColor(RGBA color);
		void setStepColor(RGBA color, int step);//i*step的线用这个颜色

		virtual void update(uint dtick);

		virtual void BuildDecalMesh(const BoxBound& boxbound, Vector3* pVB, unsigned short* pIB, 
									int nVertexBase, int nFaceLimit, int& nVertexCount, int& nFaceCount);

	protected:
		void createGridGround(float width, float length, int xgrids, int ygrids);

		RGBA m_borderColor;
		RGBA m_lineColor;
		RGBA m_stepColor;
		int  m_step;
		float m_Width;
		float m_Length;
		int m_xGrids;
		int m_yGrids;
	};
}

#endif