
float3		g_vEye			: EYE_POS;
float4x4	g_World			: WORLD;
float4x4	g_WorldView		: WORLDVIEW;
float4x4	g_WorldViewProj		: WORLDVIEWPROJ;

float		g_fLight		: LIGHT_MULTIPLY	= 1.0;
float		g_fAmbLight		: AMBLIGHT_MULTIPLY = 1.0;
float		g_fTime			:TIME;

float3 g_LightDir : DIRECTION <  
	string UIName = "Light"; 
	string Object = "TargetLight";
	int refID = 0;
> = {0, 0, 1.0};

// = {-0.577, -0.577, 0.577}

float4 g_LightCol : LIGHTCOLOR
<
	int LightRef = 0;
> = float4(0.0f, 0.0f, 0.0f, 0.0f);

float3 DoSpecularLighting(float3 pos, float3 normal, float shininess)
{
	float3 R = normalize(2*dot(normal, g_LightDir)*normal - g_LightDir); // reflection vector (view space)
	float3 V = normalize(pos);                             // view direction (view space)

	float spec = pow(max(dot(R, V),0), shininess);
	return spec * g_LightCol.xyz;
}


float3 DoDiffuseLighting(float3 pos, float3 normal)
{
	float3 diffuse = g_LightCol.xyz * max(0, dot(normal, g_LightDir));
	return diffuse;
} 
