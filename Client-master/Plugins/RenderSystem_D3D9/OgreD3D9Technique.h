
#ifndef __D3D9TECHNIQUE_H__
#define __D3D9TECHNIQUE_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreShader.h"

namespace Ogre
{
	class CompiledShader;
	class D3DMaterialTemplate;

	class D3DTech : public ShaderTechnique
	{
	public:
		D3DTech();
		bool isEqual(D3DTech *ptech);

		virtual size_t begin()
		{
			return m_nPass;
		}

		virtual void end()
		{
		}

		virtual D3DTech *clone() = 0;
		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros) = 0;
		virtual void setTexture(ShaderParamUsage type, HardwareTexture *ptexture);
		virtual void setConstant( ShaderParamUsage type, const void *pdata, size_t nbytes);
		virtual size_t getRequiredParams(ShaderParamUsage *pbuf, size_t maxbuf);

		virtual void setConstant(const char *pname, const void *pdata, size_t nbytes)
		{
			assert(0);
		}
		virtual void setTexture( const char *pname, HardwareTexture *ptexture )
		{
			assert(0);
		}
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb, size_t *poffsetbytes=NULL);
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer *pvb, size_t offset=0);
		virtual void setStreamInput(VertexDeclHandle vert_decl, HardwareVertexBuffer **ppvb,int nNumInstance,size_t *poffsetbytes);
		virtual void commitChanges()
		{
			assert(0);
		}
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive);
		virtual void draw(PrimitiveType primtype, size_t start_vert, size_t nprimitive, HardwareIndexBuffer *pib, size_t refvert_start, size_t refvert_num);

	protected:
		void applyVSPS(size_t ipass);
		void initParams();
		bool isParamUsed(ShaderParamUsage usage);

	public:
		uint m_UsedMacroMask; //用来比较两个D3DTech是否一样， 传入不同的env, macros可能生成向相同的m_UsedMacroMask， 如果vs, ps也相同， 那就不用相等了

		D3DMaterialTemplate *m_pMtlTemplate;
		struct PassData
		{
			CompiledShader *vs;
			CompiledShader *ps;
			int ntex;
			int texstage[8];
		};
		PassData m_AllPass[4];
		size_t m_nPass;
		size_t m_CurPass;

		ShaderParamUsage m_UsedParams[SPU_ENV_PARAM_END];
		size_t m_NumUsedParams;

		static IDirect3DDevice9 *m_pDevice;
		static ID3DXEffect *m_pEffect;

		static IDirect3DVertexShader9 *ms_pCurVS;
		static IDirect3DPixelShader9 *ms_pCurPS;
	};

#define SELFILLUM_NONE 0  //无自发光
#define SELFILLUM_TEX  1  //只使用自发光贴图
#define SELFILLUM_COLOR 2 //只使用自发光颜色
#define SELFILLUM_ALPHA 3 //只使用diffuse贴图alpha通道作为自发光
#define SELFILLUM_ALL 4   //使用所有

	inline void SamplerState_Wrap(IDirect3DDevice9 *pdevice, int stage)
	{
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pdevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		pdevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pdevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	inline void SamplerState_Clamp(IDirect3DDevice9 *pdevice, int stage)
	{
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		pdevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		pdevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pdevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	inline void SamplerState_DMap(IDirect3DDevice9 *pdevice, int stage)
	{
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		pdevice->SetSamplerState(stage, D3DSAMP_BORDERCOLOR, 0x00ffffff);

		pdevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		pdevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pdevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	inline void SamplerState_Border(IDirect3DDevice9 *pdevice, int stage, uint bordercolor=0)
	{
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
		pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		pdevice->SetSamplerState(stage, D3DSAMP_BORDERCOLOR, bordercolor);

		pdevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		pdevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pdevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	inline void SetBlendState(IDirect3DDevice9 *pdevice, int blendmode, int alpharef=-1)//alphares<0， 用系统缺省的值
	{
		if(blendmode == BLEND_OPAQUE)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
		else if(blendmode == BLEND_ALPHATEST)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHAREF, alpharef<0?85:alpharef);
			pdevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		}
		else if(blendmode == BLEND_ALPHABLEND)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHAREF, alpharef<0?0:alpharef);
			pdevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		else if(blendmode == BLEND_ADDBLEND)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		else if(blendmode == BLEND_ADD)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else if(blendmode == BLEND_MODULATE)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		}
		else if(blendmode == BLEND_MODULATE2X)
		{
			pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		}
		else
		{
			assert(0);
		}
	}

	inline void SetDefaultState(IDirect3DDevice9 *pdevice)
	{
		pdevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pdevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
		pdevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pdevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pdevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pdevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pdevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pdevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pdevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pdevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		pdevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		pdevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
		pdevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);

		for(int stage=0; stage<8; stage++)
		{
			pdevice->SetTexture(stage, NULL);
			pdevice->SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE);
			pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pdevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			pdevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			pdevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			pdevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pdevice->SetSamplerState(stage, D3DSAMP_MIPMAPLODBIAS, 0);
		}
	}

	extern D3DTech *CreateD3DTech(const char *name);
}

#endif //__D3D9TECHNIQUE_H__