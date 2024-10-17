

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

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 pos;
    float3 normal;

#if SKIN_MAXINFL > 0
	DoSkinVertex(input.blendindices, input.blendweights, input.pos, input.normal, pos, normal);
#else
    pos        = input.pos;
    normal    = input.normal;
#endif

	normal		= normalize(normal);

    output.pos = mul(float4(pos,1.0), g_WorldViewProj);
    output.uv0 = input.uv0;

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float3 wpos = mul(float4(pos,1.0), g_World);
	output.fogc = DoFog(wpos);
#endif

    return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 color  = tex2D(s_DiffuseSampler,input.uv0);

#ifdef MODEL_TRANSPARENT
	color.a *= g_ModelTransparent;
#endif
	
#if FOG_DISTANCE>0
	color.a *= input.fogc.x;
#endif
#if FOG_HEIGHT > 0
	color.a *= input.fogc.y;
#endif
    
	return color;
}

technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass P1
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

        VertexShader = compile vs_2_0 VSMain();
        PixelShader     = compile ps_2_0 PSMain();   
    }
}
