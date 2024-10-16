
#ifndef __OgreLines_H__
#define __OgreLines_H__

#include "OgreRenderableObject.h"
#include "OgreColourValue.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVertexFormat.h"

namespace Ogre
{
	class _OgreExport RenderLines : public RenderableObject
	{
		DECLARE_RTTI(RenderLines)
	public:
		RenderLines(bool ztest=false);
		~RenderLines();

		virtual void update(uint dtick);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);

		void reset(); //每帧开始需要调用

		void addLine(const Vector3 &p1, const Vector3 &p2, ColorQuad color);
		void addLineStrip(const Vector3 *points, size_t numpt, ColorQuad color, bool loop);
		void addLineList(const Vector3 *points, size_t numpt, ColorQuad color);

		void addTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, ColorQuad color);
		void addTriangleList(const Vector3 *points, const ColorQuad *colors, size_t stride, ushort *indices, size_t nindex);
		void addTriangleFan(const Vector3 *points, size_t numpt, ColorQuad color);

	protected:
#pragma pack(1) 
		struct LineVertex
		{
			Vector3 pos;
			ColorQuad color;
			Vector2 uv;
		};
#pragma pack()
		std::vector<LineVertex>m_LineVerts;
		std::vector<LineVertex>m_TriangleVerts;
		BoxBound m_BoxBound;
        
		Material *m_pMtl;
		VertexDeclHandle m_VertDecl;
		VertexFormat m_VertFormat;
	};
}

#endif