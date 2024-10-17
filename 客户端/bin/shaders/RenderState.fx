#define BLEND_OPAQUE 0
#define BLEND_ALPHABLEND 1
#define BLEND_ALPHATEST 2
#define BLEND_ADD_MULCOLOR 3
#define BLEND_ADD_MULALPHA 4
#define BLEND_MODULATE 5
#define BLEND_MODULATE2X 6
#define BLEND_ALPHABLENDZW 7

stateblock FogState = stateblock_state 
{
#if FOG_DIST > 0
	FogEnable			 = TRUE;
	FogTableMode		 =  linear;
	FogColor			 = (g_dwFogColor);
	FogEnd				 = (g_FogParam.y);
	FogStart			 = (g_FogParam.x);
#else
	FogEnable			 = false;
#endif
 };


stateblock BlendModeState = stateblock_state
{
#if   BLEND_MODE == BLEND_OPAQUE
		AlphaBlendEnable = FALSE;
#elif BLEND_MODE == BLEND_ALPHABLEND
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		ZWriteEnable = FALSE;
#elif BLEND_MODE == BLEND_ALPHATEST
		AlphaBlendEnable = FALSE;
		AlphaTestEnable = TRUE;
		AlphaRef = 178;
		AlphaFunc = GREATEREQUAL;
#elif BLEND_MODE == BLEND_ADD_MULCOLOR
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCCOLOR;
		DestBlend = ONE;
		ZWriteEnable = FALSE;
#elif BLEND_MODE == BLEND_ADD_MULALPHA
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = ONE;
		ZWriteEnable = FALSE;
#elif BLEND_MODE == BLEND_MODULATE
		AlphaBlendEnable = TRUE;
		SrcBlend = ZERO;
		DestBlend = SRCCOLOR;
		ZWriteEnable = FALSE;
#elif BLEND_MODE == BLEND_MODULATE
		AlphaBlendEnable = TRUE;
		SrcBlend = DESTCOLOR;
		DestBlend = SRCCOLOR;
		ZWriteEnable = FALSE;
#elif BLEND_MODE == BLEND_ALPHABLENDZW
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		//ZWriteEnable = false;		
#else
		AlphaBlendEnable = FALSE;
#endif
		cullmode	=	none;
};



stateblock TransparentState = stateblock_state
{
	AlphaBlendEnable = TRUE;
	SrcBlend = SRCALPHA;
	DestBlend = INVSRCALPHA;
	ZWriteEnable = FALSE;
	cullmode	=	none;
	colorwriteenable	=	RED|GREEN|BLUE|ALPHA;
	
};