

#ifndef __TECH_OVERLAY_H__
#define __TECH_OVERLAY_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{

	class D3DTech_Overlay_lod0 : public D3DTech
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
			return new D3DTech_Overlay_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE)
					m_BlendMode = macros.value[i];
			}

			if(env.model_xparent)
			{
				m_BlendMode = 2;
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 1; //diffusetex

			initParams();
			m_UsedMacroMask = m_BlendMode;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, m_BlendMode, 0);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}


	};

}

#endif