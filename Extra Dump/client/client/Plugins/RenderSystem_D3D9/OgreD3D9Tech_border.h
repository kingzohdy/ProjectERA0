
#ifndef __TECH_BORDER_H__
#define __TECH_BORDER_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{

	class D3DTech_Border_lod0 : public D3DTech
	{
	public:
		D3DTech *clone ()
		{
			return new D3DTech_Border_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM +0; //diffusetex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, (DWORD)0);

			SetBlendState(m_pDevice, BLEND_ALPHATEST, 0);

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			m_pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)1);
			m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
			m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
		}
	};

	class D3DTech_Border1_lod0 : public D3DTech
	{
	public:
		D3DTech *clone ()
		{
			return new D3DTech_Border1_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 2;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main1", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main1", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex\
			
			m_AllPass[1].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[1].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[1].ntex = 1;
			m_AllPass[1].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
			m_pDevice->SetRenderState(D3DRS_STENCILREF, (DWORD)1);
			m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
			m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, false);

			SamplerState_Wrap(m_pDevice, 0);

			SetBlendState(m_pDevice, BLEND_ALPHABLEND);
			m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);

			applyVSPS(m_CurPass);

		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
			SetBlendState(m_pDevice, BLEND_OPAQUE);
		}
	};

}

#endif
