

#include "header.fx"

#define SELFILLUM_NONE 0  //无自发光
#define SELFILLUM_TEX  1  //只使用自发光贴图
#define SELFILLUM_COLOR 2 //只使用自发光颜色
#define SELFILLUM_ALPHA 3 //只使用diffuse贴图alpha通道作为自发光
#define SELFILLUM_ALL 4   //使用所有

float4x4 g_WorldViewProj : WORLDVIEWPROJ;

// for包边
float g_strength  = 1.20f;

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
    normal = normalize(normal);

    output.pos = mul(float4(pos+normal*g_strength*2.15f,1.0), g_WorldViewProj);
    output.uv0 = input.uv0;

    float4 normalWorldView = mul(float4(normal, 1.0f), g_WorldView);
    normalWorldView = normalize(normalWorldView);

    float3 viewdir = normalize(g_EyePosModel - pos);
    float value = dot(normal, viewdir);
    output.color0.x = value;

return output;
}

