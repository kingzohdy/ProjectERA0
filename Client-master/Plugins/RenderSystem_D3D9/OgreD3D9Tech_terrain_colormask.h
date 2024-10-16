
#ifndef __TECH_TERRAIN_COLORMASK_H__
#define __TECH_TERRAIN_COLORMASK_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_terrain_colormask_lod0 : public D3DTech
	{
	public:
		D3DTech *clone()
		{
			return new D3DTech_terrain_colormask_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //g_ColorMaskTex

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_ALPHABLEND);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

	};
}

#endif
