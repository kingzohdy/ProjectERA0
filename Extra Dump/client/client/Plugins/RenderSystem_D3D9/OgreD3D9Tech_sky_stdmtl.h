
#ifndef __TECH_SKY_STDMTL_H__
#define __TECH_SKY_STDMTL_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_sky_stdmtl_lod0 : public D3DTech
	{
	public:
		D3DTech *clone()
		{
			return new D3DTech_sky_stdmtl_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
		}

	};

	class D3DTech_sky_stdmtl_shadowgen : public D3DTech
	{
	public:
		D3DTech *clone()
		{
			return new D3DTech_sky_stdmtl_shadowgen;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_OPAQUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
		}
	};
}

#endif
