
#ifndef __TECH_CLOTH_H__
#define __TECH_CLOTH_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_cloth_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_DOUBLE_SIDE,
			MTLPARAM_USE_SELFILLUM_TEX,
			MTLPARAM_USE_CLOTHMASK
		};

		uchar m_BlendMode;
		uchar m_DoubleSide;
		uchar m_UseSelfIllumTex;
		uchar m_UseMask;

	public:
		D3DTech *clone()
		{
			return new D3DTech_cloth_lod0;
		}

		void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_BlendMode = m_DoubleSide = m_UseSelfIllumTex = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_BLEND_MODE) m_BlendMode = macros.value[i];
				else if(indx == MTLPARAM_DOUBLE_SIDE) m_DoubleSide = macros.value[i];
				else if(indx == MTLPARAM_USE_SELFILLUM_TEX) m_UseSelfIllumTex = macros.value[i];
				else if(indx == MTLPARAM_USE_CLOTHMASK) m_UseMask = macros.value[i];
			}

			m_nPass = 1;
			if(env.model_xparent)
			{
				if(m_BlendMode <= 1)
				{
					m_BlendMode = 2;
					m_nPass = 2;
				}
			}

			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 4;
			m_AllPass[0].texstage[1] = SPU_DEPTHMAP;
			m_AllPass[0].ntex = 2;
			if(m_UseSelfIllumTex==SELFILLUM_TEX || m_UseSelfIllumTex==SELFILLUM_ALL)
			{
				m_AllPass[0].texstage[2] = SPU_GENERAL_PARAM + 5;
				m_AllPass[0].ntex = 3;
			}

			if (m_UseMask)
			{
				m_AllPass[0].texstage[3] = SPU_GENERAL_PARAM + 6;
				m_AllPass[0].texstage[4] = SPU_GENERAL_PARAM + 7;
				m_AllPass[0].ntex = 5;
			}

			if(m_nPass == 2)
			{
				m_AllPass[1] = m_AllPass[0];
			}

			initParams();
			m_UsedMacroMask = m_BlendMode|(m_DoubleSide<<8)|(m_UseSelfIllumTex<<16);
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, m_DoubleSide?D3DCULL_NONE:D3DCULL_CCW);

			if(m_nPass>1 && ipass==0)
			{
				SetBlendState(m_pDevice, BLEND_ALPHATEST, 0);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
			}
			else
			{
				if(m_BlendMode == BLEND_ALPHATEST)
				{
					if(m_UseSelfIllumTex==SELFILLUM_ALPHA || m_UseSelfIllumTex==SELFILLUM_ALL)
						SetBlendState(m_pDevice, BLEND_ALPHATEST, 0);
					else
						SetBlendState(m_pDevice, m_BlendMode, 85);
				}
				else SetBlendState(m_pDevice, m_BlendMode);

				//m_pDevice->SetRenderState(D3DRS_ZFUNC, m_nPass>1?D3DCMP_LESSEQUAL:D3DCMP_LESS);
				m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, m_BlendMode<=1?TRUE:FALSE);
			}

			SamplerState_Wrap(m_pDevice, 0);
			SamplerState_DMap(m_pDevice, 1);
			if(m_UseSelfIllumTex==SELFILLUM_TEX || m_UseSelfIllumTex==SELFILLUM_ALL)
			{
				SamplerState_Wrap(m_pDevice, 2);
			}

			if (m_UseMask)
			{
				SamplerState_Clamp(m_pDevice, 3);
				SamplerState_Wrap(m_pDevice, 4);
			}

			applyVSPS(m_CurPass);
		}
		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			//m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

			m_pDevice->SetTexture(1, 0); // 将深度纹清空

			if (m_UseMask)
			{
				m_pDevice->SetTexture(3, 0); // 流光图清空
				m_pDevice->SetTexture(4, 0); 
			}

		}
	};

	class D3DTech_cloth_shadowgen : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_BLEND_MODE = 1,
			MTLPARAM_DOUBLE_SIDE,
			MTLPARAM_USE_SELFILLUM_TEX,
			MTLPARAM_USE_CLOTHMASK
		};

		uchar m_DoubleSide;

	public:
		D3DTech *clone()
		{
			return new D3DTech_cloth_shadowgen;
		}

		void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_DoubleSide = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if(indx == MTLPARAM_DOUBLE_SIDE) m_DoubleSide = macros.value[i];
			}

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "ShadowGen", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "ShadowGen", env, macros);

			m_AllPass[0].ntex = 1;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 4;

			initParams();
			m_UsedMacroMask = m_DoubleSide;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			SetBlendState(m_pDevice, BLEND_ALPHATEST);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, m_DoubleSide?D3DCULL_NONE:D3DCULL_CCW);

			/*
			uint bias;
			*(float *)&bias = 0.0005f;
			m_pDevice->SetRenderState(D3DRS_DEPTHBIAS, bias);
			*/

			SamplerState_Wrap(m_pDevice, 0);

			applyVSPS(m_CurPass);
		}

		virtual void endPass()
		{
			m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
		}
	};
}

#endif