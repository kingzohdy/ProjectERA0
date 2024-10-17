

#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

float3	g_SelfPower;
float	g_SpecPower;

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
	float2 fogh: TEXCOORD5;
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
    normal = normalize(normal);
#endif

    output.pos = mul(float4(pos,1.0), g_WorldViewProj);

    output.color0 = DoLighting(pos, normal, 0);

#if USE_SELFILLUM_TEX==SELFILLUM_COLOR
    output.color0.rgb += g_SelfPower.rgb;
#endif

    output.color0.a *= g_SpecPower;

    output.uv0 = input.uv0;

#ifdef LIGHT_SHADOW
    output.color1 = DoLighting(pos, normal, 1);
    output.color1.a *= g_SpecPower;
   
	float4 wpos2 = mul(float4(pos,1.0), g_World);
	output.uv1 = CalShadowProjUV(wpos2);
#endif


#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float3 wpos = mul(float4(pos,1.0), g_World);
	output.fogc = DoFog(wpos);
#endif

#if USE_SELFILLUM_TEX==SELFILLUM_ALL
	output.uv2 = input.uv0;
#endif 

    return output;
}
