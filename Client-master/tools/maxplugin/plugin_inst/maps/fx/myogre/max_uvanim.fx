
#include "header.fx"

int BLEND_MODE<
	string UIName = "���ģʽ:����|�����|���|�ӻ�|��";
	string UIType = "IntSpinner";
	float UIMin = 0;
	float UIMax = 4;	
>  = 2;

uniform bool RECEIVE_LIGHTING<
	string UIName = "�Ƿ��ܹ�";
> = false;

bool RECEIVE_FOG<
	string UIName = "�Ƿ�����Ӱ��";
> = true;

uniform bool USE_MASK_TEX<
	string UIName = "�Ƿ�ʹ��Mask��ͼ";
> = true;

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

#if USE_MASK_TEX==true
texture g_MaskTex
<
	string UIName = "�ɰ���ͼ";
	int Texcoord = 1;
	int MapChannel = 2;
>;

sampler s_MaskSampler = sampler_state{
    Texture = <g_MaskTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};
#endif

float g_TransU
<
    string UIWidget = "g_TransU";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "U����ƫ��";
> = 0.0f;

float g_TransV
<
    string UIWidget = "g_TransV";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "V����ƫ��";
> = 0.0f;

float g_ScaleU
<
    string UIWidget = "g_ScaleU";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "U�������";
> = 1.0f;

float g_ScaleV
<
    string UIWidget = "g_ScaleV";
    float UIMin = -50;
    float UIMax = 50;
    float UIStep = 0.1;
    string UIName = "V�������";
> = 1.0f;

float g_RotUV
<
    string UIWidget = "g_RotUV";
    float UIMin = -720;
    float UIMax = 720;
    float UIStep = 1;
    string UIName = "������ת";
> = 0.0f;

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
    float2 uv0            : TEXCOORD0;
    float2 uv1            : TEXCOORD1;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
	VS_OUTPUT output;
    
	output.pos = mul(input.pos, g_WorldViewProj);

	if(RECEIVE_LIGHTING)
	{
		float3 pos = mul(input.pos, g_World);
		float3 normal = normalize(mul(input.normal, (float3x3)g_World));

		output.color0 = float4(DoDiffuseLighting(pos, normal) + float3(0.2,0.2,0.2), 1.0);
	}
	else output.color0 = float4(1.0, 1.0, 1.0, 1.0);

	float angle = radians(g_RotUV);
	float2 uv = (input.uv0 - float2(0.5,-0.5))*float2(g_ScaleU, g_ScaleV);
	float u = uv.x*cos(angle) + uv.y*sin(angle);
	float v = -uv.x*sin(angle) + uv.y*cos(angle);

	output.uv0 = float2(u,v) + float2(0.5,-0.5) + float2(g_TransU, g_TransV);
	output.uv1 = input.uv1;
	return output;
}

float4 PSMain(VS_OUTPUT input):COLOR0
{
	float4 color0  = tex2D(s_DiffuseSampler,input.uv0)*input.color0;

	if(USE_MASK_TEX) color0 *= tex2D(s_MaskSampler, input.uv1);

	return color0;
}

technique GeneralTech
{
    pass P1
    {    
	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
	ZWriteEnable = TRUE;
	ZEnable = FALSE;

	CullMode = None;
        VertexShader =    compile vs_2_0 VSMain();
        PixelShader  =    compile ps_2_0 PSMain();   
    }
}
