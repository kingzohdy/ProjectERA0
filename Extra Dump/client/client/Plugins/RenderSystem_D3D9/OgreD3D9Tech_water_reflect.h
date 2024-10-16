
#ifndef __TECH_WATER_REFLECT_H__
#define __TECH_WATER_REFLECT_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_water_reflect_lod0 : public D3DTech
	{
	public:
		D3DTech *clone()
		{
			return new D3DTech_water_reflect_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 3;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 6; //g_WaterDepthTex
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 8;; //g_refelcttexture
			m_AllPass[0].texstage[2] = SPU_GENERAL_PARAM + 11; //g_Normaltexture

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_ALPHABLEND);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			SamplerState_Clamp(m_pDevice, 0);
			SamplerState_Clamp(m_pDevice, 1);
			SamplerState_Wrap(m_pDevice, 2);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

	};
}

#endif
