
#ifndef __D3D9Shader_H__
#define __D3D9Shader_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreShader.h"
#include <d3dx9effect.h>

namespace Ogre
{
	class D3D9Shader;

	class D3D9ShaderTechnique : public ShaderTechnique
	{
	public:
		D3D9ShaderTechnique(D3D9Shader *pshader, D3DXHANDLE h);

		virtual size_t begin();
		virtual void end();
		virtual void beginPass( size_t ipass );
		virtual void endPass();
		virtual void setConstant(const char *pname, const void *pdata);
		virtual void setConstant( ShaderParamUsage usage, const void *pdata);
		virtual void setRawConstant(ShaderParamUsage usage, const void *pdata, size_t offset, size_t nbytes);
		virtual void setRawConstant(const char *pname, const void *pdata, size_t offset, size_t nbytes);
		virtual void setTexture( const char *pname, HardwareTexture *ptexture );
		virtual void setTexture(ShaderParamUsage type, HardwareTexture *ptexture);
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb, size_t *poffsetbytes=NULL);
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer *pvb, size_t offset=0);
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb,int nNumInstance,size_t *poffsetbytes);
		virtual void commitChanges();
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive);
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive, HardwareIndexBuffer *pib, size_t refvert_start, size_t refvert_num);
		virtual size_t getRequiredParams(ShaderParamUsage *pbuf, size_t maxbuf);

	public:
		D3D9Shader *m_pShader;
		ID3DXEffect *m_pEffect;
		D3DXHANDLE m_hTech;
		int m_Lod;
		int m_RenderUsage;
		ShaderParamUsage m_UsedParams[SPU_ENV_PARAM_END];
		size_t m_NumUsedParams;
		bool m_bDirty;
	};

	class D3D9Shader
	{
	public:
		D3D9Shader(D3D9RenderSystem *prs, D3DMaterialTemplate *pmtl);
		~D3D9Shader();

		bool create(const char *pbuffer, size_t buflen, const D3DXMACRO *pmacros);
		HRESULT createFromCompiled(const char *pbuffer, size_t buflen);
		D3D9ShaderTechnique *getTechnique(uint lod, RenderUsage renderusage);

		ID3DXEffect *getEffect(){ return m_pEffect; }

		D3DXHANDLE paramUsage2Handle(ShaderParamUsage usage);

		void onLostDevice();
		bool onResetDevice();

		bool hasID (int id)
		{
			bool bIn = false;

			for (int i=0; i<(int)m_MapIDList.size(); i++)
			{
				if (m_MapIDList[i] == id)
					bIn = true;
			}

			return bIn;
		}

		int getMapIDNum ()
		{
			return (int)m_MapIDList.size();
		}

		int getMapID (int iIndex)
		{
			assert(0<=iIndex && iIndex<(int)m_MapIDList.size());

			return m_MapIDList[iIndex];
		}

		void addMapID (int id)
		{
			if (!hasID(id))
			{
				m_MapIDList.push_back(id);
			}
		}

	public:
		D3D9RenderSystem *m_pRS;
		std::vector<int> m_MapIDList;
		bool m_bCompiled;
		ShaderEnvFlags m_ShaderEnv;
		MaterialMacro m_MtlMacro;
		std::vector<char>m_CompiledBuf;

	protected:
		D3DMaterialTemplate *m_pMtl;
		ID3DXEffect *m_pEffect;
		D3DXHANDLE m_EnvHandles[SPU_ENV_PARAM_END];
		std::vector<D3DXHANDLE>m_LocalHandles;

		std::vector<D3D9ShaderTechnique *>m_Techniques;

	private:
		void cacheParamHandle();
		void cacheTechnique();
	};
}

#endif