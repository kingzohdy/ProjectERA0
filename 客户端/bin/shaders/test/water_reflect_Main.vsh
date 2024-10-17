#include "header.fx"

float4x4	g_WorldViewProj : WORLDVIEWPROJ;

struct VS_INPUT
{
    float4 pos        : POSITION;
    float2 uv0        : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos          : POSITION;
    float2 bumpuv       : TEXCOORD0;
    float4 reflectxy_refractyx : TEXCOORD1;
    float W : TEXCOORD2;
    float3 eyevec : TEXCOORD3;
    
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD4;
#endif
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 projpos = mul(input.pos, g_WorldViewProj);
	output.pos = projpos;

	//output.bumpuv = input.uv0*g_NumTexRepeat + float2(g_fSpeed*g_fTime, -g_fSpeed*g_fTime)*0.3;
	output.bumpuv = input.uv0;

	float2 reflect_pos = (float2(-projpos.x, -projpos.y) + projpos.w) * 0.5;
	float2 refract_pos = (float2(projpos.x, -projpos.y) + projpos.w) * 0.5;
	output.reflectxy_refractyx = float4( reflect_pos.x, reflect_pos.y, refract_pos.y, refract_pos.x);
	output.W = projpos.w;

	output.eyevec = normalize(g_EyePos.xyz - input.pos.xyz);
	
#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif

	return output;
}
