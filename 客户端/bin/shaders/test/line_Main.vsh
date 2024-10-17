#include "header.fx"
float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_INPUT
{
	float4 pos			: POSITION;
	float4 color                    : COLOR0;
	float2 uv0			: TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float2 uv0      : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.pos = mul(input.pos, g_WorldViewProj);
    output.color0 = input.color;
    output.uv0 = input.uv0;
    return output;
}