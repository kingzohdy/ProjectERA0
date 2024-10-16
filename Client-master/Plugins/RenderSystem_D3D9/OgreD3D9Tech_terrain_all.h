
#ifndef __TECH_TERRAIN_ALL_H__
#define __TECH_TERRAIN_ALL_H__

#include "OgreD3D9Technique.h"

namespace Ogre
{
	class D3DTech_terrain_all_lod0 : public D3DTech
	{
	public:
		enum
		{
			MTLPARAM_TERRAIN_LAYERS = 1,
			MTLPARAM_USE_STATICLIGHT,
			MTLPARAM_USE_LIGHTMAP,
		};

		uchar m_TerrainLayers;
		uchar m_UseStaticLight;
		uchar m_UseLightMap;

	public:
		D3DTech *clone()
		{
			return new D3DTech_terrain_all_lod0;
		}

		void init(const ShaderEnvFlags &env, const MaterialMacro &macros)
		{
			m_TerrainLayers = 0;

			for(int i=0; i<MaterialMacro::MAX_MACRO; i++)
			{
				int indx = macros.indices[i];
				if(indx == 0) break;

				if (indx == MTLPARAM_TERRAIN_LAYERS) m_TerrainLayers = macros.value[i];
				if (indx == MTLPARAM_USE_STATICLIGHT) m_UseStaticLight = macros.value[i];
				if (indx == MTLPARAM_USE_LIGHTMAP) m_UseLightMap = macros.value[i];
			}
			if(m_TerrainLayers > 3 )m_TerrainLayers = 3;

			m_nPass = 1;
			m_AllPass[0].vs = m_pMtlTemplate->getCompiledVSPS(COMPILED_VS, "Main", env, macros);
			m_AllPass[0].ps = m_pMtlTemplate->getCompiledVSPS(COMPILED_PS, "Main", env, macros);

			//assert(m_TerrainLayers > 0);
			//if(m_TerrainLayers == 1) m_AllPass[0].ntex = 2;
			//else m_AllPass[0].ntex = 2+m_TerrainLayers;

			m_AllPass[0].ntex = 7;
			m_AllPass[0].texstage[0] = SPU_GENERAL_PARAM + 2;//g_BlendColorTex0
			m_AllPass[0].texstage[1] = SPU_GENERAL_PARAM + 6;//g_BlendAlphaTex
			m_AllPass[0].texstage[2] = SPU_GENERAL_PARAM + 3;//g_BlendColorTex1
			m_AllPass[0].texstage[3] = SPU_GENERAL_PARAM + 4;//g_BlendColorTex2
			m_AllPass[0].texstage[4] = SPU_GENERAL_PARAM + 5;//g_BlendColorTex3
			m_AllPass[0].texstage[5] = SPU_DEPTHMAP;
			m_AllPass[0].texstage[6] = SPU_GENERAL_PARAM + 7;

			initParams();
			m_UsedMacroMask = m_TerrainLayers;
		}

		virtual void beginPass( size_t ipass )
		{
			m_CurPass = ipass;

			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			SetBlendState(m_pDevice, BLEND_OPAQUE, 0);

			uint LOD_BIAS;
			*(float *)&LOD_BIAS = -0.7f;

			SamplerState_Wrap(m_pDevice, 0);
			m_pDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, LOD_BIAS);
			m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			SamplerState_DMap(m_pDevice, 5);

		//	if(m_TerrainLayers > 1)
			{
				SamplerState_Clamp(m_pDevice, 1);
				m_pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

				SamplerState_Wrap(m_pDevice, 2);
				m_pDevice->SetSamplerState(2, D3DSAMP_MIPMAPLODBIAS, LOD_BIAS);
				m_pDevice->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			}
		//	if(m_TerrainLayers > 2)
			{
				SamplerState_Wrap(m_pDevice, 3);
				m_pDevice->SetSamplerState(3, D3DSAMP_MIPMAPLODBIAS, LOD_BIAS);
				m_pDevice->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			}
			//if(m_TerrainLayers > 3)
			{
				SamplerState_Wrap(m_pDevice, 4);
				m_pDevice->SetSamplerState(4, D3DSAMP_MIPMAPLODBIAS, LOD_BIAS);
				m_pDevice->SetSamplerState(4, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			}

			SamplerState_Clamp(m_pDevice, 6);
			m_pDevice->SetSamplerState(6, D3DSAMP_MIPMAPLODBIAS, LOD_BIAS);
			m_pDevice->SetSamplerState(6, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			applyVSPS(m_CurPass);
		}
		virtual void endPass()
		{
			m_pDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, 0);
			if(m_TerrainLayers > 1)
			{
				m_pDevice->SetSamplerState(2, D3DSAMP_MIPMAPLODBIAS, 0);
			}
			if(m_TerrainLayers > 2)
			{
				m_pDevice->SetSamplerState(3, D3DSAMP_MIPMAPLODBIAS, 0);
			}
			if(m_TerrainLayers > 3)
			{
				m_pDevice->SetSamplerState(4, D3DSAMP_MIPMAPLODBIAS, 0);
			}

			m_pDevice->SetSamplerState(6, D3DSAMP_MIPMAPLODBIAS, 0);

			m_pDevice->SetTexture(0, 0); // 
			m_pDevice->SetTexture(1, 0); // 
			m_pDevice->SetTexture(2, 0); // 
			m_pDevice->SetTexture(3, 0); // 
			m_pDevice->SetTexture(4, 0); // 
			m_pDevice->SetTexture(5, 0); // 将深度纹清空
			m_pDevice->SetTexture(6, 0); // 
		}
	};

}

#endif