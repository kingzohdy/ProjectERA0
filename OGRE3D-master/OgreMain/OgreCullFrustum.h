
#ifndef __CullFrustum_H__
#define __CullFrustum_H__

#include "OgrePlane.h"
#include "OgreBounding.h"

namespace Ogre
{
	enum CullType
	{
		CULL_VIEWFRUSTUM,			// 简单的视锥剪裁，多用于编辑模式
		CULL_FULL,					// 利用各种特殊数据结构，做最大限度的剪裁，多用于游戏模式
	};

	class _OgreExport CullFrustum
	{
	public:
		enum CULLRET_T
		{
			CULL_IN,
			CULL_OUT,
			CULL_CROSS
		};

	public:
		CullFrustum();
		~CullFrustum();

		void createFromMatrix(const Matrix4 &tm);
		void addCullPlane(const Plane &plane);
		CULLRET_T cull(const BoxSphereBound &bounding);
		CULLRET_T cull(const Vector3 &point);
		const BoxSphereBound &getBounding(){ return m_Bounding; }

	private:
		Plane		m_Planes[32];
		int			m_UsedNum;
		BoxSphereBound	m_Bounding;
	};
}

#endif