
#ifndef __D3D9SHADERMANAGER_H__
#define __D3D9SHADERMANAGER_H__

#include "OgreRenderTypes.h"
#include "OgreVector4.h"
#include "OgreMatrix4.h"
#include "OgreColourValue.h"
#include <vector>

#include "OgreD3DMaterialTemplate.h"

namespace Ogre
{
	struct PassDesc
	{
		char vs[16];
		char ps[16];
	};
	struct TechniqueDesc
	{
		D3DXHANDLE htech;
		uint lod;
		int renderusage;
		std::vector<PassDesc>m_Pass;
	};
	class CompiledShader
	{
	public:
		CompiledShader(D3DMaterialTemplate *pmtltemplate, COMPILED_TYPE type);
		~CompiledShader();

		void cacheTechnique();
		TechniqueDesc *getTechnique(uint lod, RenderUsage renderusage);
		void cacheParamHandle();
		void setConstant(const char *pname, const void *pdata, size_t datalen);
		void setConstant( ShaderParamUsage usage, const void *pdata, size_t datalen);
		bool hasConstant(ShaderParamUsage usage, D3DXHANDLE htech);
		D3DXHANDLE paramUsage2Handle(ShaderParamUsage usage);

		void onLostDevice();
		bool onResetDevice();

		bool create ();

		void addShaderKey (ShaderEnvKey key)
		{
			if (!hasShaderKey(key))
			{
				mShaderEnvKeys.push_back(key);
			}
		}

		bool hasShaderKey (ShaderEnvKey key)
		{
			bool bHas = false;

			for (int i=0; i<(int)mShaderEnvKeys.size(); i++)
			{
				if (key == mShaderEnvKeys[i])
					bHas = true;
			}

			return bHas;
		}

		int numShaderKey ()
		{
			return (int)mShaderEnvKeys.size();
		}

	public:
		D3DMaterialTemplate *m_pMtlTemplate;
		COMPILED_TYPE m_Type;
		IDirect3DDevice9 *m_pDevice;
		ID3DXBuffer *m_pBuffer;
		std::vector<char> m_CompiledBuf;
		uint m_BufferKey;

		ID3DXEffect *m_pEffect;
		IDirect3DVertexShader9 *m_pVS;
		IDirect3DPixelShader9 *m_pPS;
		ID3DXConstantTable *m_pConst;

		D3DXHANDLE m_EnvHandles[SPU_ENV_PARAM_END];
		D3DXHANDLE m_MtlHandles[16];

		std::vector<TechniqueDesc *>m_TechDesc;

		std::vector<ShaderEnvKey> mShaderEnvKeys;

		bool m_bCreated;
		bool m_bAllCreated;

		static int ms_MaxLightNum;
		static int ms_MaxSkinNum; 

		static int ms_EffectNum;
		static int ms_EffectMemorySize;
		static int ms_StdEffectNum;
		static int ms_BorderEffectNum;
		static int ms_EffectCompileTime;

		static int ms_VSNum;
		static int ms_VSMemorySize;
		static int ms_StdVSNum;
		static int ms_BorderVSNum;
		static int ms_VSCompileTime;

		static int ms_PSNum;
		static int ms_PSMemorySize;
		static int ms_StdPSNum;
		static int ms_BorderPSNum;
		static int ms_PSCompileTime;

		static int ms_ProcessParamsTime;

		static int ms_CacheTechniqueTime;
		static int ms_CacheParamHandleTime;
	};

	class CompiledShaderGroup
	{
	public:
		CompiledShaderGroup(COMPILED_TYPE type, D3DMaterialTemplate *pmtltemplate, const FixedString &path);
		~CompiledShaderGroup();

		void onLostDevice();
		bool onResetDevice();

		CompiledShader *getCompiledShader(const ShaderEnvFlags &env, const MaterialMacro &macros);
		CompiledShader *createCompiledShader (const ShaderEnvFlags &env, const MaterialMacro &macros);

		void compileAllShaders();

	public:
		COMPILED_TYPE m_Type; //
		D3DMaterialTemplate *m_pMtlTemplate;
		FixedString m_Path;
		std::vector<CompiledShader *>m_Shaders;
		std::map<ShaderEnvKey, CompiledShader *>m_MacroShaderMap;

		bool m_bCreatedAll;
	};

	class D3DShaderTechnique : public ShaderTechnique
	{
	public:
		D3DShaderTechnique(D3DMaterialTemplate *pmtltemplate, const ShaderEnvFlags &env, const MaterialMacro &macros, uint lod, RenderUsage usage);
		~D3DShaderTechnique();

		virtual size_t begin();
		virtual void end();
		virtual void beginPass( size_t ipass );
		virtual void endPass();

		virtual void setConstant(const char *pname, const void *pdata, size_t nbytes);
		virtual void setConstant( ShaderParamUsage type, const void *pdata, size_t nbytes);
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
		D3DMaterialTemplate *m_pMtlTemplate;
		CompiledShader *m_pEffect;
		D3DXHANDLE m_hTech;
		size_t m_CurPass;
		int m_Lod;
		int m_RenderUsage;
		ShaderParamUsage m_UsedParams[SPU_ENV_PARAM_END];
		size_t m_NumUsedParams;
		bool m_bDirty;
		IDirect3DDevice9 *m_pDevice;

		struct PassData
		{
			CompiledShader *vs;
			CompiledShader *ps;
			bool use_fx_vs;
			bool use_fx_ps;
		};

		std::vector<PassData>m_AllPass;
	};

}

#endif
