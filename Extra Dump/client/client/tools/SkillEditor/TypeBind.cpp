#include "StdAfx.h"
#include "TypeBind.h"
#include "NotifyProperty.h"

void PropUpdate_float(CBCGPProp** ppProp, float* data, DATA_UPDATA_TYPE type, CString name, float ratio, float max, float min)
{
	if(type == CREATE_PROP)
	{
		CNotifiedProp* pProp = new CNotifiedProp(name, _variant_t(float(0)));
		pProp->SetFloatDigitRatio(ratio);
		pProp->SetDigitRange((float)max, (float)min);
		(*ppProp) = pProp;
	}
	else if(type == PROP_TO_DATA)
	{
		*data = (float)((*ppProp)->GetValue());
	}
	else if(type == DATA_TO_PROP)
	{
		(*ppProp)->SetValue(_variant_t((float)(*data)));
	}
	else
	{
		if(type == INC_DATA_RELATED)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
			{
				*data += ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
		else if(type == INC_DATA_ALL)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
			{
				*data += ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
		else if(type == DEC_DATA_RELATED)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
			{
				*data -= ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
		else if(type == DEC_DATA_ALL)
		{
			if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
			{
				*data -= ((CNotifiedProp*)(*ppProp))->m_fIncStep;
				(*ppProp)->SetValue(_variant_t((float)(*data)));
			}
		}
	}
}

void PropUpdate_int(CBCGPProp** ppProp, int* data, DATA_UPDATA_TYPE type, CString name, float ratio, int max, int min)
{
	if(type == CREATE_PROP)
	{
		_variant_t t;
		t.intVal = 0;
		t.vt = VT_I4;

		CNotifiedProp* pProp = new CNotifiedProp(name,t);
		pProp->SetFloatDigitRatio(ratio);
		pProp->SetDigitRange((float)max, (float)min);

		(*ppProp) = pProp;
	}
	else if(type == PROP_TO_DATA)
	{
		*data = (int)((*ppProp)->GetValue());
	}
	else if(type == DATA_TO_PROP)
	{
		_variant_t t;
		t.intVal = *data;
		t.vt = VT_I4;
		(*ppProp)->SetValue(t);
	}
	else if(type == INC_DATA_RELATED)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
		{
			*data += (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
	else if(type == INC_DATA_ALL)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
		{
			*data += (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
	else if(type == DEC_DATA_RELATED)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)) && ((CNotifiedProp*)(*ppProp))->m_bRelated == true)
		{
			*data -= (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
	else if(type == DEC_DATA_ALL)
	{
		if((*ppProp)->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
		{
			*data -= (int)((CNotifiedProp*)(*ppProp))->m_fIncStep;
			(*ppProp)->SetValue(_variant_t((int)(*data)));
		}
	}
}

void PropUpdate_flag(CBCGPProp** ppProp, unsigned int* data, DATA_UPDATA_TYPE type, CString name, int flag)
{
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
		(*ppProp)->AddOption("是");
		(*ppProp)->AddOption("否");
	}
	else if(type == PROP_TO_DATA)
	{
		CString str = OLE2T((_bstr_t)(*ppProp)->GetValue());
		if(str == "是")
			*data |= flag;
		else
			*data &= ~flag;
	}
	else if(type == DATA_TO_PROP)
	{
		CString str;
		if( (*data & flag) != 0)
			str = "是";
		else
			str = "否";
		(*ppProp)->SetValue(_variant_t(str.GetBuffer()));
	}
}

void PropUpdate_bool(CBCGPProp** ppProp, bool* data, DATA_UPDATA_TYPE type, CString name)
{	
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
		(*ppProp)->AddOption("是");
		(*ppProp)->AddOption("否");
	}
	else if(type == PROP_TO_DATA)
	{
		CString str = OLE2T((_bstr_t)(*ppProp)->GetValue());
		if(str == "是")
			*data = true;
		else
			*data = false;
	}
	else if(type == DATA_TO_PROP)
	{
		CString str;
		if( *data == true)
			str = "是";
		else
			str = "否";
		(*ppProp)->SetValue(_variant_t(str.GetBuffer()));
	}
}


void PropUpdate_cstring(CBCGPProp** ppProp, CString* data, DATA_UPDATA_TYPE type, CString name)
{
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA && *ppProp != NULL)
	{
		*data = OLE2T((_bstr_t)(*ppProp)->GetValue());
	}
	else if(type == DATA_TO_PROP && *ppProp != NULL)
	{
		(*ppProp)->SetValue(_variant_t(data->GetBuffer()));
	}
}

void PropUpdate_charptr(CBCGPProp** ppProp, char* data, DATA_UPDATA_TYPE type, CString name)
{
	USES_CONVERSION;
	if(type == CREATE_PROP)
	{
		(*ppProp) = new CNotifiedProp(name, _T(""));
	}
	else if(type == PROP_TO_DATA && *ppProp != NULL)
	{
		strcpy(data, OLE2T((_bstr_t)(*ppProp)->GetValue()));

	}
	else if(type == DATA_TO_PROP && *ppProp != NULL)
	{
		(*ppProp)->SetValue(_variant_t(data));
	}
}
