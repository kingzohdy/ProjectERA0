#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_INPUT
{
	float4 pos			: POSITION;
	float3 normal		: NORMAL;
	float2 uv0			: TEXCOORD0;
	float4 dif			: COLOR0;
};

struct VS_OUTPUTFIX
{
	float4 pos      : POSITION;
	float4 color0        : COLOR;
	float2 uv0      : TEXCOORD0;
};

VS_OUTPUTFIX VSMainFIX(VS_INPUT input, uniform int layer)
{
	VS_OUTPUTFIX output;

	output.pos  = mul(float4(input.pos.xyz,1.0), g_WorldViewProj);
	output.color0 = float4(input.dif.rgb, 0) + float4(1.0,1.0,1.0,0);

	if(layer == 0) output.uv0 = input.uv0*g_UVRepeat.x;
	else if(layer==1) output.uv0 = input.uv0*g_UVRepeat.y;
	else if(layer==2) output.uv0 = input.uv0*g_UVRepeat.z;
	else if(layer==3) output.uv0 = input.uv0*g_UVRepeat.w;

   	return output;
}
