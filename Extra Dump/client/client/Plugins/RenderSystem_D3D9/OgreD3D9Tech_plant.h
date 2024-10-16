
#ifndef __TECH_PLANT_H__
#define __TECH_PLANT_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_plant_lod0 : public D3DTech
	{
	public:
		D3DTech *clone()
		{
			return new D3DTech_plant_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 0; //diffusetex
			m_AllPass[0].texstage[1] = SPU_DEPTHMAP; //

			initParams();
			m_UsedMacroMask = 0;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, BLEND_ALPHATEST);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			SamplerState_Clamp(m_pDevice, 0);
			SamplerState_DMap(m_pDevice, 1);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetTexture(1, 0); // 将深度纹清空
		}

	};
}

#endif
