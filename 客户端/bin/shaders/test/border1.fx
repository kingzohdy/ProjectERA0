

#include "header.fx"

#define SELFILLUM_NONE 0  //无自发光
#define SELFILLUM_TEX  1  //只使用自发光贴图
#define SELFILLUM_COLOR 2 //只使用自发光颜色
#define SELFILLUM_ALPHA 3 //只使用diffuse贴图alpha通道作为自发光
#define SELFILLUM_ALL 4   //使用所有

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

float3	g_SelfPower = float3(0.0f,0.0f,0.0f);
float	g_SpecPower;
float	g_SpecShiness;

texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = POINT;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

#if USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL
texture g_SpecSelfTex;
sampler s_SpecSelfSampler = sampler_state{
    Texture = <g_SpecSelfTex>;
    MipFilter = linear;
    MinFilter = linear;
    MagFilter = linear;
};

float g_TransU = 0.0f;
float g_TransV = 0.0f;
float g_ScaleU = 1.0f;
float g_ScaleV = 1.0f;
float g_RotUV = 0.0f;
#endif

texture   g_depthtexture	   : DEPTH_MAP;
sampler  s_DepthMapSampler = sampler_state{
    Texture = <g_depthtexture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = BORDER ;
    AddressV = BORDER ;
    BORDERCOLOR		= 0x00ffffff;
};

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

technique GeneralTech
<
	int usage = RENDER_USAGE_UI|RENDER_USAGE_GENERAL|RENDER_USAGE_REFLECT|RENDER_USAGE_REFRACT;
	int lod = 0;
>
{
    pass pBB
    <
	string vs = "Main";
	string ps = "Main";
    >
    {
	COLORWRITEENABLE = 0x0000000F;

	StencilEnable = TRUE;
	StencilFunc = NOTEQUAL;
	StencilRef = 1;
	StencilPass = Keep;
	StencilZFail = Zero;

	CullMode = NONE;
     /* ZEnable  = TRUE;
	ZWriteEnable = TRUE;
	ZFunc = LESS;	*/

	AlphaTestEnable = FALSE;
	ALPHAREF = 85;
	ALPHAFUNC = GREATER;

	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
	Sampler[0] = (s_DiffuseSampler);
    }
}

