// stdafx.cpp : 只包括标准包含文件的源文件
// ObjectEditor.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


int g_nExten = 7;
char *g_FileExten[] =
{
	".otex",
	".omod",
	".ent",
	".emo",
	".dds",
	".wav",
	".mp3",
	".wma"
};

char* g_strPublicMotionAddr = "effect\\public\\";
char* g_chCharactorResPath = "character\\";
char* g_pszModelResPath = "character\\";
char* g_chSpecialTextureResPath = "effect\\texture\\";
char* g_chPublicEffectResPath = "effect\\public\\";
char* g_strMaterialMotionAddr = "effect\\material\\";
char* g_strWorkMotionAddr = "effect\\public\\";

char* g_strShowModelEffectAddr = "character\\";
char* g_strMaterialShowModelEffectAddr = "effect\\";
char* g_strWorkShowModelEffectAddr = "effect\\";