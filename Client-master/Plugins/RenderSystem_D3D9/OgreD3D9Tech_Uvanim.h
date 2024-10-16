

#ifndef __TECH_UVANIM_H__
#define __TECH_UVANIM_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{

	class D3DTech_Uvanim_lod0 : public D3DTech
	{
	public:
		enum 
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_RECEIVE_LIGHTING,
			MTLPARAM_RECEIVE_FOG,
			MTLPARAM_MASK_TEX
		};

		int m_BlendMode;
		int m_ReceiveLighting;
		int m_ReceiveFog;
		int m_Mask;

	public:
		D3DTech *clone ()
		{
			return new D3DTech_Uvanim_lod0;
		}

		virtual void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = m_ReceiveLighting = m_ReceiveFog = m_Mask = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE)
					m_BlendMode = macros.value[i];
				else if (indx == MTLPARAM_RECEIVE_LIGHTING)
					m_ReceiveLighting = macros.value[i];
				else if (indx == MTLPARAM_RECEIVE_FOG)
					m_ReceiveFog = macros.value[i];
				else if (indx == MTLPARAM_MASK_TEX)
					m_Mask = macros.value[i];
			}

			if(env.model_xparent && m_BlendMode<2) m_BlendMode = 2;

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].ntex = 2;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 4; //diffusetex
			m_AllPass[0].texstage[1] = SPU_DEPTHMAP;

			if(m_Mask)
			{
				m_AllPass[0].ntex = 3;
				m_AllPass[0].texstage[2] = SPU_GENERAL_PARAM + 5; //masktexture
			}

			initParams();
			m_UsedMacroMask = m_BlendMode|(m_ReceiveLighting<<8)|(m_ReceiveFog<<12)|(m_Mask<<16)|(env.model_xparent<<20);
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			SetBlendState(m_pDevice, m_BlendMode);

			if (m_BlendMode == 0 || m_BlendMode == 1)
			{
				m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			}
			else
			{
				m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			}

			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			SamplerState_Wrap(m_pDevice, 0);
			SamplerState_DMap(m_pDevice, 1);
			if(m_Mask)
			{
				SamplerState_Wrap(m_pDevice, 2);
			}
			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			m_pDevice->SetTexture(1, 0); // 将深度纹清空
		}


	};

}

#endif