
#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_INPUT
{
	float4 pos			: POSITION;
	float2 uv0			: TEXCOORD0;
};

struct VS_OUTPUTPS
{
	float4 pos      : POSITION;
	float2 uv0      : TEXCOORD0;
};

VS_OUTPUTPS VSMain(VS_INPUT input)
{
	VS_OUTPUTPS output;

	output.pos  = mul(float4(input.pos.xyz,1.0), g_WorldViewProj);
	output.uv0 =  input.uv0;

   	return output;
}
