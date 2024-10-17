

#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal        : NORMAL;
    float4 color        :COLOR;
    
#if SKIN_MAXINFL > 0
    float4 blendweights : BLENDWEIGHT;
    float4 blendindices : BLENDINDICES;
#endif

    float2 uv0            : TEXCOORD0;
#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	float2 uv1 : TEXCOORD1;
#endif
};

struct VS_OUTPUT_ShadowGen
{
    float4 pos            : POSITION;
    float2 uv0            : TEXCOORD0;
};

VS_OUTPUT_ShadowGen VSMain(VS_INPUT input)
{
    VS_OUTPUT_ShadowGen output;
    
    float3 pos;
    float3 normal;

#if SKIN_MAXINFL > 0
	DoSkinVertex(input.blendindices, input.blendweights, input.pos, input.normal, pos, normal);
#else
    pos        = input.pos;
    normal    = input.normal;
#endif

#ifdef LOG_SHADOWMAP
    float4 wpos = mul(float4(pos,1.0), g_WorldViewProj);
    float2 diff = wpos.xy - g_ShadowCenter.xy;
    float t = length(diff);
    float nt = g_ShadowCenter.w * log(g_ShadowCenter.z*t + 1)/t;

    output.pos.xy = g_ShadowCenter.xy + diff*nt;
    output.pos.zw = wpos.zw;
#else
    output.pos = mul(float4(pos,1.0), g_WorldViewProj);
#endif

    output.uv0 = input.uv0;

    return output;
}
