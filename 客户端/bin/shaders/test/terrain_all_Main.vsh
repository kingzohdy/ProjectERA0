#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

struct VS_INPUT
{
	float4 pos			: POSITION;
	float3 normal		: NORMAL;
	float2 uv0			: TEXCOORD0;
	float4 dif			: COLOR0;
	float  state		: BLENDWEIGHT;
};

struct VS_OUTPUTPS
{
	float4 pos      : POSITION;
	float4 color0        : TEXCOORD3; //color0.xyz=ambient lighting,  color0.w=specular lighting
	float2 uv0      : TEXCOORD0;
#ifdef LIGHT_SHADOW
	float4 color1        : TEXCOORD4; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
	float4 uv1            : TEXCOORD1;
#else	
#ifdef LIGHT_SHADOWCUBE
	float4 color1        : TEXCOORD4; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
	float3 uv1           : TEXCOORD1;
#endif	
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif
	float2 fogh: TEXCOORD5;
};

VS_OUTPUTPS VSMain(VS_INPUT input)
{
	VS_OUTPUTPS output;

	output.pos  = mul(float4(input.pos.xyz,1.0), g_WorldViewProj);
	output.color0 = DoLighting(input.pos, input.normal, 0) + float4(input.dif.rgb, 0);

	output.uv0 =  input.uv0;
#ifdef LIGHT_SHADOW
	output.color1 = DoLighting(input.pos, input.normal, 1);
	output.uv1  = CalShadowProjUV(input.pos);
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif

   	return output;
}
