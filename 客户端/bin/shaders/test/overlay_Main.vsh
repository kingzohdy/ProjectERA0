

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