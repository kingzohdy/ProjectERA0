
#include "header.fx"

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Clamp;
    AddressV = Clamp;
};

float4  g_PosExcursion;
float3  g_Normal = float3(0,1,0);

struct VS_INPUT
{
	float4 pos			: POSITION;
	float4 color			: COLOR0;
	float2 uv0			: TEXCOORD0;
};

struct VS_OUTPUTPS
{
	float4 pos      : POSITION;
	float4 color0        : COLOR0; //color0.xyz=ambient lighting,  color0.w=specular lighting
	float2 uv0      : TEXCOORD0;
	float4 color1        : COLOR1; //color1.xyz=shadowed_lighting,  color1.w=specular lighting
#ifdef LIGHT_SHADOW
	float4 uv1            : TEXCOORD1;
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	float2 fogc	: TEXCOORD2;
#endif

};

VS_OUTPUTPS mainvs(VS_INPUT input)
{
	VS_OUTPUTPS output;

	output.uv0 =  input.uv0;

	float4 position = input.pos;
	float dsin = sin(g_PosExcursion.x+input.pos.y);
	float ddsinColor = abs(dsin);

	float v = 1.0 - input.uv0.y;
	position.x += 9*dsin*v;
	output.pos  = mul(position, g_WorldViewProj);

	float colordiff = 0.2*ddsinColor*v;

	output.color0 = DoLighting(input.pos, g_Normal, 0)-colordiff;
	output.color1 = DoLighting(input.pos, g_Normal, 1);


#ifdef LIGHT_SHADOW
	output.uv1  = CalShadowProjUV(input.pos);
#endif

#if FOG_HEIGHT>0 || FOG_DISTANCE>0
	output.fogc = DoFog(input.pos);
#endif

   return output;
}

float4 PSMain2_0(VS_OUTPUTPS input) : COLOR0
{
	float4 color = tex2D(s_DiffuseSampler, input.uv0);

    float4 lighting = input.color0;
#ifdef LIGHT_SHADOW
    float shadow = DoHardShadow(s_DepthMapSampler, input.uv1, input.uv1.z/input.uv1.w); 
	shadow = 1.0f - (1.0f - shadow)*g_shadowdensity;
	lighting += shadow*input.color1;
#else
	lighting += input.color1;	
#endif

//#ifdef LIGHT_SPECULAR
 //   color.rgb = lighting.rgb*color.rgb + lighting.a*g_SpecularColor;
//#else
    color.rgb = lighting.rgb*color.rgb;
//#endif

#if FOG_DISTANCE>0
	color.rgb = lerp(g_DistFogColor, color.rgb, input.fogc.x);
#endif
#if FOG_HEIGHT > 0
	color.rgb = lerp(g_HeightFogColor, color.rgb, input.fogc.y);
#endif

	return color;
}

technique tech
<
	int usage = RENDER_USAGE_GENERAL;
	int lod = 0;
>
{
    pass p0
    {
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = true;
		AlphaBlendEnable = false;
		AlphaTestEnable  = true;
		ALPHAREF = 85;
		ALPHAFUNC = GREATER;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;  
               
		VertexShader = compile vs_1_1 mainvs();
 		PixelShader = compile ps_2_0 PSMain2_0();
   } 
}