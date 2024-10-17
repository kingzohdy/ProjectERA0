
#include "header.fx"

#define SELFILLUM_NONE 0  //���Է���
#define SELFILLUM_TEX  1  //ֻʹ���Է�����ͼ
#define SELFILLUM_COLOR 2 //ֻʹ���Է�����ɫ
#define SELFILLUM_ALPHA 3 //ֻʹ��diffuse��ͼalphaͨ����Ϊ�Է���
#define SELFILLUM_ALL 4   //ʹ������


int BLEND_MODE<
	string UIName = "���ģʽ:0.����|1.�����|2.���|3.�ӻ�|4.��";
	string UIType = "IntSpinner";
	float UIMin = 0;
	float UIMax = 4;	
>  = 0;

bool DOUBLE_SIDE<
	string UIName = "˫�����";
> = 0;

int USE_SELFILLUM_TEX
<
	string UIName = "�Է���ģʽ:0.��|1.��ͼ|2.��ɫ|3.ͨ��|4.����";
	string UIType = "IntSpinner";
	float UIMin = 0;
	float UIMax = 4;	
> = 0;

texture g_DiffuseTex : DIFFUSEMAP
<
	string UIName = "��������ͼ";
	int Texcoord = 0;
	int MapChannel = 1;
>;

sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

texture g_SpecSelfTex
<
	string UIName = "�Է���|�߹�ͨ����ͼ";
	int Texcoord = 1;
	int MapChannel = 2;
>;

sampler s_SelfIllumSampler = sampler_state{
    Texture = <g_SpecSelfTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

bool USE_CLOTHMASK
<
string UIName = "ʹ�÷���Mask";	
> = 0;

texture g_ClothMask
<
	string UIName = "�·�Mask";
	int Texcoord = 0;
	int MapChannel = 1;
>;

sampler s_ClothMaskSampler = sampler_state{
    Texture = <g_ClothMask>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

float g_SpecPower
<
	string UIWidget = "g_SpecPower";
	float UIMin = 0;
	float UIMax = 1.0;
	float UIStep = 0.01;
	string UIName = "�߹�ǿ��";
> = 1.0;

float g_SpecShiness
<
	string UIWidget = "g_SpecShiness";
	float UIMin = 0;
	float UIMax = 32.0;
	float UIStep = 1.0;
	string UIName = "�߹����̶�";
> = 4.0f;

float4 g_SelfPower
<
	string UIName = "�Է���ǿ�ȼ�ƫɫ"; 
	string UIType = "MaxSwatch";
> = float4(1.0, 1.0, 1.0, 1.0);

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
    float4 color0        : COLOR0;
    float4 color1	 : COLOR1;
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;
    
	float3 pos = mul(input.pos, g_World);
	float3 normal = normalize(mul(input.normal, (float3x3)g_World));

	output.pos = mul(input.pos, g_WorldViewProj);
	
	float3 diffuse = DoDiffuseLighting(pos, normal);
	if(USE_SELFILLUM_TEX==SELFILLUM_COLOR) diffuse += g_SelfPower.rgb;

	output.color0 = float4(diffuse, 1.0);
	output.color1 = float4(DoSpecularLighting(pos, normal, g_SpecShiness)*g_SpecPower, 1.0);
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 colorLast;
	float4 color0  = tex2D(s_DiffuseSampler,input.uv0);
	if(USE_SELFILLUM_TEX==SELFILLUM_ALPHA||USE_SELFILLUM_TEX==SELFILLUM_ALL) color0 *= float4(lerp(input.color0.rgb,1.2,color0.a), 1.0);
	else color0 *= input.color0;

	if(USE_SELFILLUM_TEX==SELFILLUM_TEX||USE_SELFILLUM_TEX==SELFILLUM_ALL)
	{
		float4 color1 = tex2D(s_SelfIllumSampler, input.uv1)*g_SelfPower;
		colorLast = float4(color0.rgb+input.color1*color1.a+color1.rgb, color0.a);
	}
	else
	{
		colorLast = float4(color0.rgb+input.color1, color0.a);
	}

	if (USE_CLOTHMASK == 1)
	{
		float  second     = g_fTime;
		float4 colorMask;
		colorMask.r = 1.0f;
		colorMask.g = 0.0f;
		colorMask.b = 0.0f;
		float4 colorMaskA = tex2D(s_ClothMaskSampler,input.uv0);
		colorLast.rgb += (colorMask.rgb * colorMaskA.a);
	}	

	return colorLast;
}

technique GeneralTech
{
    pass P1
    {    
	AlphaBlendEnable = FALSE;
	AlphaTestEnable = TRUE;
	AlphaRef = 0;//85;
	AlphaFunc = GREATER;

	ZWriteEnable = TRUE;
	ZEnable = TRUE;

	CullMode = None;
        VertexShader =    compile vs_3_0 VSMain();
        PixelShader  =    compile ps_3_0 PSMain();   
    }
}
