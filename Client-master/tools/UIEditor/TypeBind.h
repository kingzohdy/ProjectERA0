#pragma once
#include "OgrePrerequisites.h"
#include "OgreUIRenderTypes.h"
#include "OgreBillboardData.h"

enum DATA_UPDATA_TYPE
{
	DATA_TO_PROP,
	PROP_TO_DATA,
	CREATE_PROP,
	INC_DATA_RELATED,
	DEC_DATA_RELATED,
	INC_DATA_ALL,
	DEC_DATA_ALL
};

extern void PropUpdate_float(CBCGPProp** ppProp, float* data, DATA_UPDATA_TYPE type, CString name, float ratio = 1.0f, float max = 32767, float min = -32766);
extern void PropUpdate_int(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name, float ratio = 1.0f, int max = 32767, int min = -32766);
extern void PropUpdate_cstring(CBCGPProp** ppProp, CString* data, DATA_UPDATA_TYPE type, CString name);
extern void PropUpdate_charptr(CBCGPProp** ppProp, char* data, DATA_UPDATA_TYPE type, CString name);
extern void PropUpdate_flag(CBCGPProp** ppProp, unsigned int* data, DATA_UPDATA_TYPE type, CString name, int flag);
extern void PropUpdate_bool(CBCGPProp** ppProp, bool* data, DATA_UPDATA_TYPE type, CString name);
