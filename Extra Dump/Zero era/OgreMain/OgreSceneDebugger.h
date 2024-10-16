
#ifndef __SceneDebugger_H__
#define __SceneDebugger_H__

#include "OgreVector3.h"
#include "OgreColourValue.h"
#include "OgreSingleton.h"

namespace Ogre
{
	enum
	{
		DBG_R_SKELETON = 1,
		DBG_R_BOUNDING = 2,
	};

	class _OgreExport SceneDebugger : public Singleton<SceneDebugger>
	{
	public:
		SceneDebugger();
		~SceneDebugger();

		void renderObject(RenderableObject *pobject, unsigned int flags);
		void renderRay(WorldRay &ray);
		void renderLine(const WorldPos &pos1, const WorldPos &pos2, RGBA color);
		void renderLineScreen(const Vector3 &pos1, const Vector3 &pos2, RGBA color);
		void renderTriangle(const WorldPos &pos1, const WorldPos &pos2, const WorldPos &pos3, RGBA color);

		void flipBuffer();
		void drawScene(Camera *pcamera, float fWidth, float fHeight);

	private:
		Material *m_pLineMtl;
		VertexDeclHandle m_VertDecl;

		struct stLine
		{
			Vector3 pos1;
			Vector3 pos2;
			RGBA color;
		};

		struct stTriangle
		{
			Vector3 pos1;
			Vector3 pos2;
			Vector3 pos3;
			RGBA color;
		};

		int						m_nRenderBuffer;
		int						m_nWritingBuffer;

		std::vector<stLine>		m_Lines[2];
		std::vector<stLine>		m_ScreenLines[2];
		std::vector<stTriangle>	m_Triangles[2];

	private:
		void getBoxEdgeLines(std::vector<stLine>&lines, const Vector3 &minpos, const Vector3 &maxpos, RGBA color);
	};
}

#endif