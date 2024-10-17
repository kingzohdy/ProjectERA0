#include "header.fx"

float4x4	g_WorldViewProj : WORLDVIEWPROJ;
float4x4	g_refProj;

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal        : NORMAL;
    float2 uv0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos          : POSITION;
    float4 color0		: COLOR0;
    float2 uv0          : TEXCOORD0;
    float4 uv1			: TEXCOORD1;
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif

};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(input.pos, g_WorldViewProj);
	output.color0 = DoLighting(input.pos, input.normal, 0);
	output.uv0	= input.uv0;
	
	output.uv1	= mul(input.pos, g_refProj);
	
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif


	return output;
}
