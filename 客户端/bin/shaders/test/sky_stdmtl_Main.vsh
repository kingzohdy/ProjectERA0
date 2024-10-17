

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
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float4 color0        : COLOR0; //color0.xyz=ambient lighting,  color0.w=specular lighting
    float2 uv0            : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float3 pos;
    float3 normal;
    
    pos        = input.pos;
    normal    = input.normal;

    output.pos = mul(float4(pos,1.0), g_WorldViewProj);
    output.color0 = DoLighting(pos, normal, 0);
    output.uv0 = input.uv0;


    return output;
}
