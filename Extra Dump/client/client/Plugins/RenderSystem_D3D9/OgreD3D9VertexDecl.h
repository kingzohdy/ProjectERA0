
#include "OgreD3D9Prerequisites.h"
#include "OgreVertexIndexData.h"

namespace Ogre
{
	class D3D9VertexDecl
	{
	public:
		D3D9VertexDecl(D3D9RenderSystem *prs, const VertexFormat &vertfmt);
		~D3D9VertexDecl();

		bool create();
		void onLostDevice();
		bool onResetDevice();

	public:
		D3D9RenderSystem *m_pRS;
		VertexFormat m_VertFmt;
		IDirect3DVertexDeclaration9 *m_pVertDecl;
	};
}