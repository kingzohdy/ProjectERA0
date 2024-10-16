
#ifndef __TECH_BACK_H__
#define __TECH_BACK_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{

	class D3DTech_Back0_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
		};

		uchar m_BlendMode;

	public:
		D3DTech *clone ()
		{
			return new D3DTech_Back0_lod0;
		}

		void init (const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = 0;
			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE) m_BlendMode = macros.value[i];
			}

			if(env.model_xparent)
			{
				if(m_BlendMode <= 1)
				{
					m_BlendMode = 2;
				}
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 1; //diffusetex

			initParams();
			m_UsedMacroMask = m_BlendMode|(env.model_xparent<<8);
		}

		virtual size_t begin ()
		{
			return 1;
		}

		virtual void end ()
		{

		}

		virtual void beginPass (size_t ipass)
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, (DWORD)0);

			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);

			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			m_pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)1);
			m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
			m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

			SetBlendState(m_pDevice, m_BlendMode);

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		}
	};

	class D3DTech_Back1_lod0 : public D3DTech
	{
	public:
		D3DTech *clone ()
		{
			return new D3DTech_Back1_lod0;
		}

		void init (const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex


			initParams();
			m_UsedMacroMask = 0;
		}

		virtual size_t begin ()
		{
			return 1;
		}

		virtual void end ()
		{

		}

		virtual void beginPass (size_t ipass)
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
			m_pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)1);
			m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
			m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
	};
}

#endif