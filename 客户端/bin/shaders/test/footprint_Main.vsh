#include "header.fx"

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
	AddressU = CLAMP;
    AddressV = CLAMP;
};

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_OUTPUT
{
    float4 pos      : POSITION;
    float4 color0   : COLOR0;
    float2 uv0      : TEXCOORD0;
};

VS_OUTPUT VSMain( float4 pos : POSITION,
                   float4 color0 : COLOR,
		   float2 uv0 : TEXCOORD0
                          )
{
	VS_OUTPUT output;
    
	output.pos = mul(pos, g_WorldViewProj);
	output.color0 = color0;
	output.uv0 = uv0;

	return output;
}