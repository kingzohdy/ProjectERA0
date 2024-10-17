
#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

float g_TransU = 0.0f;
float g_TransV = 0.0f;
float g_ScaleU = 1.0f;
float g_ScaleV = 1.0f;
float g_RotUV = 0.0f;

struct VS_INPUT
{
    float4 pos        : POSITION;
    float3 normal     : NORMAL;
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 pos            : POSITION;
	float4 color0     : COLOR0;
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;

#ifdef LIGHT_SHADOW
    float4 color1        : COLOR1; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
    float4 uv2            : TEXCOORD2;
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD3;
#endif
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;
    
	output.pos = mul(input.pos, g_WorldViewProj);
	output.color0 = DoLighting(input.pos, input.normal, 0);

	float angle = radians(g_RotUV);
	float2 uv = (input.uv0 - float2(0.5,0.5))*float2(g_ScaleU, g_ScaleV);
	float u = uv.x*cos(angle) + uv.y*sin(angle);
	float v = -uv.x*sin(angle) + uv.y*cos(angle);

	output.uv0 = float2(u,v) + float2(0.5,0.5) + float2(g_TransU, g_TransV);
	output.uv1 = input.uv1;

#ifdef LIGHT_SHADOW
    output.color1 = DoLighting(input.pos, input.normal, 1);
    float4 wpos2 = mul(input.pos, g_World);
    output.uv2 = CalShadowProjUV(wpos2);
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float3 wpos = mul(input.pos, g_World);
	output.fogc = DoFog(wpos);
#endif
	return output;
}