

#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

float3	g_SelfPower = float3(0.0f,0.0f,0.0f);
float	g_SpecPower;
float	g_SpecShiness;

#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
float g_TransU = 0.0f;
float g_TransV = 0.0f;
float g_ScaleU = 1.0f;
float g_ScaleV = 1.0f;
float g_RotUV = 0.0f;
#endif

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

struct VS_OUTPUT
{
    float4 pos            : POSITION;
    float4 color0        : COLOR0; //color0.xyz=ambient lighting,  color0.w=specular lighting
    float2 uv0            : TEXCOORD0;

#ifdef LIGHT_SHADOW
    float4 color1        : COLOR1; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
    float4 uv1            : TEXCOORD1;    
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif

#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	float2 uv2 : TEXCOORD3;
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
    output.color0 = DoLighting(pos, normal, 0);
#if USE_SELFILLUM_TEX==SELFILLUM_COLOR
    output.color0.rgb += g_SelfPower.rgb;
#endif

//    output.color0.rgb += input.color.rgb;
    output.color0.a *= g_SpecPower;
    output.uv0 = input.uv0;

#ifdef LIGHT_SHADOW
    output.color1 = DoLighting(pos, normal, 1);
    output.color1.a *= g_SpecPower;
   
	float4 wpos2 = mul(float4(pos,1.0), g_World);
	output.uv1 = CalShadowProjUV(wpos2);
#endif

//    float3 viewdir = normalize(g_EyePosModel - pos);
//	output.color0.a += 1.0 - dot(normal, viewdir);

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float3 wpos = mul(float4(pos,1.0), g_World);
	output.fogc = DoFog(wpos);
#endif

#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	float angle = radians(g_RotUV);
	float2 uv = (input.uv1 - float2(0.5,0.5))*float2(g_ScaleU, g_ScaleV);
	float u = uv.x*cos(angle) + uv.y*sin(angle);
	float v = -uv.x*sin(angle) + uv.y*cos(angle);

	output.uv2 = float2(u,v) + float2(0.5,0.5) + float2(g_TransU, g_TransV);
#endif 
    return output;
}
