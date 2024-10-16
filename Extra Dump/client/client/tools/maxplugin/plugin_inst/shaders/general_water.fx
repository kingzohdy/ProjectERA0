#include "header.fx"

float4		g_DeepWaterColor;
float4		g_ShallowWaterColor;
float		g_NumTexRepeat;
texture		g_WaterBaseTex;
texture		g_WaterDepthTex;
texture		g_Normaltexture;
texture		g_bumptexture;

texture		g_refelcttexture;
float4x4	g_refProj;

float4x4	g_WorldViewProj : WORLDVIEWPROJ;
float4		g_WavePara = float4(1.0f,8.0f,0.3f,0.4f);

sampler BaseSampler = sampler_state
{
    Texture = <g_WaterBaseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

sampler ReflectSampler = sampler_state
{
    Texture = <g_refelcttexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

sampler	DepthSampler = sampler_state
{
    Texture = <g_WaterDepthTex>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

sampler	NormalSampler = sampler_state
{
    Texture = <g_Normaltexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

sampler	BumpSampler = sampler_state
{
    Texture = <g_bumptexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

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


float4 PSMain(VS_OUTPUT input) : COLOR0
{
	float4 basecolor = tex2D(BaseSampler, g_NumTexRepeat*input.uv0);
	float t = tex2D(DepthSampler, input.uv0).a;
	float4 color = lerp(g_ShallowWaterColor, g_DeepWaterColor, t)*float4(basecolor.xyz, 1.0);//use lerp alpha

	float4 lighting = input.color0;

  /*  
#ifdef LIGHT_SPECULAR
	color.rgb = lighting.rgb*color.rgb + lighting.a*basecolor.a*g_SpecularColor;
#else
	color.rgb = lighting.rgb*color.rgb;
#endif
*/

#if FOG_DISTANCE>0
	color.rgb = lerp(g_DistFogColor, color.rgb, input.fogc.x);
#endif
#if FOG_HEIGHT > 0
	color.rgb = lerp(g_HeightFogColor, color.rgb, input.fogc.y);
#endif
	color.a += 0.05f;
	return color;
}


technique GeneralTech
<
    int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL;
    int lod = 0;
>
{
    pass P1
    {
#if ALPHABLEND==0
	AlphaBlendEnable = FALSE;
	ZWriteEnable = TRUE;
#else
	AlphaBlendEnable = TRUE;
	ZWriteEnable = FALSE;
#endif
	 CullMode = CW;
	 SrcBlend = SRCALPHA;
	 DestBlend = INVSRCALPHA;

        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMain();   
    }
}


technique GeneralTech
<
    int usage = RENDER_USAGE_REFLECT;
    int lod = 0;
>
{
    pass P1
    {    
        colorwriteenable = 0;
        zwriteenable	=	false;
        VertexShader = compile vs_2_0 VSMain();
        PixelShader = compile ps_2_0 PSMain();   
    }
}
