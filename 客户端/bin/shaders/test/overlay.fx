

#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = POINT;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal        : NORMAL;
    
#if SKIN_MAXINFL > 0
    float4 blendweights : BLENDWEIGHT;
    float4 blendindices : BLENDINDICES;
#endif

    float2 uv0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float2 uv0            : TEXCOORD0;

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif
};

technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass P1
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
#ifdef MODEL_TRANSPARENT
#undef BLEND_MODE
#define BLEND_MODE 2
#endif

#if BLEND_MODE == 0
        AlphaBlendEnable = FALSE;
	AlphaTestEnable = FALSE;
#elif BLEND_MODE == 1
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 85;
	AlphaFunc = GREATEREQUAL;
#elif BLEND_MODE == 2
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;
	AlphaFunc = GREATER;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
#elif BLEND_MODE == 3
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = ONE;
	DestBlend = INVSRCALPHA;
#elif BLEND_MODE == 4
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = ONE;
	DestBlend = ONE;
#elif BLEND_MODE == 5
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = DESTCOLOR;
	DestBlend = ZERO;
#elif BLEND_MODE == 6
	AlphaBlendEnable = TRUE;
	AlphaTestEnable = FALSE;
	SrcBlend = DESTCOLOR;
	DestBlend = SRCCOLOR;
#endif


	ZWriteEnable = FALSE;
	ZEnable = TRUE;
	ZFunc = EQUAL;

	CullMode = NONE;

	Sampler[0] = (s_DiffuseSampler);
    }
}
