/**********************************************************************
*<
FILE: stdafx.h

DESCRIPTION:

CREATED BY: Blisswang

HISTORY: Created Tuesday, February 03, 2007

*>	Copyright (c) 2007, All Rights Reserved.
**********************************************************************/

#ifndef __MTLHDR__H
#define __MTLHDR__H

#define USE_STDMTL2_AS_STDMTL

#ifdef BLD_MTL
#define MtlExport __declspec( dllexport )
#else
#define MtlExport __declspec( dllimport )
#endif

#pragma once

//3ds Max 头文件
#include "Max.h"
#include "resource.h"
#include "utilapi.h"
#include "istdplug.h"
#include "modstack.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "stdmat.h"
#include "ISkin.h"
#include "decomp.h"
#include "bipexp.h"
#include "tab.h"
#include "Phyexp.h"
#include "notetrck.h"

#include "imtl.h"
#include "texutil.h"
#include "buildver.h"

//#include "shaders.h"
//#include "macrorec.h"
//#include "gport.h"
//#include "simpobj.h"
//#include "spline3d.h"
//#include "splshape.h"
//#include "bmmlib.h"
//#include "ikctrl.h"
//#include "strclass.h"
//#include "interpik.h"
//#include "notetrck.h"
//
//#include "iparamm2.h"
//#include "simpspl.h"



//MAY引擎头文件
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"
#include "OgreFixedString.h"
#include "OgreString.h"
#include "OgreMesh.h"
#include "OgreXMLData.h"
#include "OgreSkeletonAnimData.h"
#include "OgreMaterialAnimData.h"
#include "OgreMorphAnimData.h"
#include "OgreModelData.h"
#include "OgreKeyFrameArray.h"
#include "OgreEntity.h"
#include "OgreTexture.h"
#include "OgreSkeletonData.h"

#include <vector>
#include <map>

//---------------------------------------------------------------------------

// error codes the converter can return
#define W3D_STAT_OK					0x0000
#define W3D_STAT_FAILED				0x0001
#define W3D_STAT_NO_MEMORY			0x0002

#define W3D_STAT_MISSING_DEMO		0x0003
#define W3D_STAT_NO_RENDERER		0x0004
#define W3D_STAT_ANIMATION_FAILED	0x0005
#define W3D_STAT_NO_MATERIAL		0X0006
#define W3D_STAT_TANGENT_FAILED		0X0007

//---------------------------------------------------------------------------
#define MAY_MAX_PATH 260

extern ClassDesc* GetBMTexDesc();
extern ClassDesc* GetTexmapsDesc();
extern ClassDesc* GetTexmaps2Desc();
extern ClassDesc* GetOldTexmaps2Desc();

#ifndef NO_MTL_TOPBOTTOM
extern ClassDesc* GetCMtlDesc();
#endif // NO_MTL_TOPBOTTOM

// old shaders are here, mostly to guarantee the existance of the default shader
extern ClassDesc* GetConstantShaderCD();
#ifndef USE_LIMITED_STDMTL 
extern ClassDesc* GetPhongShaderCD();
extern ClassDesc* GetMetalShaderCD();
#endif

extern ClassDesc* GetBlinnShaderCD();
extern ClassDesc* GetOldBlinnShaderCD();

TCHAR *GetString(int id);

#define DIRECTX_9_SHADER_CLASS_ID Class_ID(0x0ed995e4 ,0x6133daf2)

#define OGRE_DEFAULT_TEXTURE "toolres\\default_texture.dds"

enum
{
	EXP_ANIM_MODEL = 0,
	EXP_STATIC_MODEL,
	EXP_LINK_MESH,
	EXP_PURE_ANIM
};

struct ExportOption
{
	int exptype;

	bool exp_normal;
	bool exp_vertcolor;
	bool exp_tangent;
	int exp_numuv;

	/*
	bool exp_boneanim;
	bool exp_vertanim;
	bool exp_uvanim;
	bool exp_mtlanim;
	*/

	int anim_start;
	int anim_end;
	float keydiff_translate;
	float keydiff_rotate;
	float keydiff_scale;

	char src_rootdir[256];
	char dest_rootdir[256];
	char savepath[256];
};

extern HINSTANCE g_hInstance;
extern ExportOption g_ExpOption;
extern Interface *g_Interface;

extern const char *GetMaxRootDir();
extern TCHAR *GetString(int id);

#endif
