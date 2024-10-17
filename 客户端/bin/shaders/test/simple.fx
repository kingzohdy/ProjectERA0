
texture g_DiffuseTex;
sampler s_DiffuseSampler = sampler_state{
    Texture = <g_DiffuseTex>;
    MipFilter = POINT;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

technique GeneralTech
{
    pass P1
    {
        AlphaBlendEnable = FALSE;
	AlphaTestEnable = FALSE;
	ZWriteEnable = TRUE;
	ZEnable = TRUE;
	CullMode = CCW;

	Sampler[0] = (s_DiffuseSampler);
    }
}
