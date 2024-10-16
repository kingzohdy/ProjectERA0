
#include "stdafx.h"
#include "IDxMaterial.h"

#include "exp_attribs.h"

using namespace Ogre;

namespace DKEXP
{
	CustAttribConverter::CustAttribConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;

		m_iMapChannel = -1000;
	}

	CustAttribConverter::~CustAttribConverter()
	{
	}

	char* CustAttribConverter::GetShortName(char* longName)
	{
		const char* pEx = strrchr(longName,'\\');

		if (!pEx)
		{
			return longName;
		}
		pEx++;

		static char sNameShort[256];
		strcpy(sNameShort,pEx);
		return sNameShort;
	}

	void CustAttribConverter::GetFilePath (char *szFile,char *szPath, char separator)  //参数(全文件名,保存结果的缓冲区) 
	{ 
		char *p,*q; 
		strcpy (szPath,szFile); 
		p=szPath; 
		q=strchr(p,separator);    //得到p指向的缓冲区中第一个'/'的地址 
		while (q)      
		{ 
			p=++q;             //开始时p和q处在'/'的后一个字符 
			q=strchr(q,separator);   //q去寻找下一个'/',如是NULL,那p中的就是最后一个'/' 
		};                  //p中始终保存着最后被找到的'/'的后一个字符 
		*p='\0';            //在'/'后写'\0'截断字符串 
	}                    //所以缓冲区中的字符串改变了  

	bool CustAttribConverter::IsValidName(char* pcStr)
	{
		int iLen = strlen(pcStr);
		if ( iLen == 0) 
			return false;

		for (int ui = 0; ui < iLen; ui++)
		{
			if (pcStr[ui] >= 'a' && pcStr[ui] <= 'z')
				continue;
			else if (pcStr[ui] >= 'A' && pcStr[ui] <= 'Z')
				continue;
			else if (pcStr[ui] >= '1' && pcStr[ui] <= '9')
				continue;
			else if (pcStr[ui] == '0')
				continue;
			else if (pcStr[ui] == ' ')
				continue;
			else if (pcStr[ui] == '\t')
				continue;
			else if (pcStr[ui] == '_')
				continue;
			else if (pcStr[ui] == '-')
				continue;
			else if (pcStr[ui] == '/')
				continue;
			else if (pcStr[ui] == '\\')
				continue; 
			else
				return false;
		}
		return true;
	}

	static bool GetMaterialNameFromPath(char *mtlname, const char *fullpath)
	{
		const char *psep = strrchr(fullpath, '\\');
		if(psep == NULL) psep = fullpath;
		else psep++;

		if(strstr(psep, "max_") != psep)
		{
			char errorbuf[256];
			sprintf(errorbuf, "材质[%s]必须用max_*.fx这种effect文件", psep);
			MessageBox(NULL, errorbuf, "材质错误", MB_OK);
			return false;
		}

		psep += 4;
		int count = 0;
		while(*psep!=0 && *psep!='.')
		{
			mtlname[count++] = *psep++;
		}

		mtlname[count] = 0;
		return true;
	}

	int CustAttribConverter::Convert(INode *pMaxNode, Ogre::Material *pdestmtl, Animatable *obj, std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		IDxMaterial* dxMtl = (IDxMaterial*)obj->GetInterface(IDXMATERIAL_INTERFACE);

		char *string_value = dxMtl->GetEffectFilename();
		/*
		 MaxSDK::AssetManagement::AssetUser user = dxMtl->GetEffectFile();
		char *string_value = (char *)user.GetFileName().data();
		*/
		char mtlname[256];
		if(!GetMaterialNameFromPath(mtlname, string_value))
		{
			return W3D_STAT_FAILED;
		}

		//保存shader name
		pdestmtl->setTemplateName(mtlname);

		IParamBlock2* pParamBlock = obj->GetParamBlock(0);
		if (pParamBlock != NULL)
		{
			if (!ConvertParamBlock(pMaxNode, pdestmtl, obj, pParamBlock, MtlAnimDataArray))
			{
				return W3D_STAT_FAILED;
			}
		}

		return W3D_STAT_OK;
	}

	bool CustAttribConverter::ConvertParamBlock(INode *pMaxNode, Ogre::Material *pdestmtl, Animatable *obj, IParamBlock2* pkParamBlock,	std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		ParamBlockDesc2 *pd = pkParamBlock->GetDesc();

		int num_params = pkParamBlock->NumParams();

		int param_type;
		for (int i=0; i<num_params; i++)
		{
			param_type = pkParamBlock->GetParameterType(i);

			switch(param_type) 
			{
			case TYPE_INODE:
				break;
			case TYPE_STRING:
				if (!ConvertStringAttrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_FLOAT:
				if (!ConvertFloatAttrib(pMaxNode, pkParamBlock, i, pdestmtl, MtlAnimDataArray))
				{
					return false;
				}
				break;
			case TYPE_INT:
				if (!ConvertIntAttrib(pMaxNode, obj, pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_RGBA:
				if (!ConvertColorAttrib(pkParamBlock, i, pdestmtl, i))
				{
					return false;
				}
				break;
			case TYPE_POINT3:
				if (!ConvertPoint3Attrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_POINT4:
				if (!ConvertPoint4Attrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_BOOL:
				if (!ConvertBoolAttrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_FLOAT_TAB:
				if (!ConvertFloatTabAttrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_RGBA_TAB:
				break;
			case TYPE_INT_TAB:
				break;
			case TYPE_STRING_TAB:
				break;
			case TYPE_TEXMAP:
				if (!ConvertTexMapAttrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_INTLISTBOX:
				if (!ConvertIntListBoxAttrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_BITMAP:
				if (!ConvertBitMapAttrib(pkParamBlock, i, pdestmtl))
				{
					return false;
				}
				break;
			case TYPE_FRGBA:
				if (!ConvertFRGBAAttrib(pMaxNode, pkParamBlock, i, pdestmtl, MtlAnimDataArray))
				{
					return false;
				}
				break;
			case TYPE_BITMAP_TAB:
				assert(0 && "have no TYPE_BITMAP_TAB ");
				break;
			default:
				break;
			}
		}

		return true;
	}

	bool CustAttribConverter::ConvertStringAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		char* string_value;
		Interval valid_inverval;
		BOOL retrieved = false;

		retrieved = pkParamBlock->GetValue(iIndex, 0, string_value, valid_inverval);

		if (retrieved)
		{
			char* value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
			}

			assert(IsValidName(value_name));

			assert(0); //不支持string 类型
			pdestmtl->setParamValue(value_name, string_value);
		}

		return true;
	}

	bool HasAnimation(IParamBlock2 *pblock, int index)
	{
		Control *pcontrol = pblock->GetController(index);
		return pcontrol != NULL;
	}

	bool CustAttribConverter::ConvertFloatAttrib(INode *pMaxNode, IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl, std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if(!pkParamDesc) return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		float float_value;
		Interval valid_inverval;
		BOOL retrieved = pkParamBlock->GetValue(iIndex, 0, float_value, valid_inverval);
		if(!retrieved)
		{
			return false;
		}

		char* value_name = kParamDef.int_name;
		if (!IsValidName(value_name))
		{
			value_name = pkParamBlock->GetLocalName(iIndex);
			assert (value_name);
		}

		if(!HasAnimation(pkParamBlock, iIndex))
		{
			pdestmtl->setParamValue(value_name, &float_value);
			return true;
		}

		int nframe = (m_animEnd-m_animStart)/GetTicksPerFrame() + 1;
		float *pvalueframes = (float *)malloc(nframe*sizeof(float));
		unsigned int *pticks = new unsigned int[nframe];
		for(int i=0; i<nframe; i++)
		{
			TimeValue tick = m_animStart + i*GetTicksPerFrame();
			pticks[i] = tick*1000/TIME_TICKSPERSEC;
			pvalueframes[i] = 0;
			pkParamBlock->GetValue(iIndex, tick, pvalueframes[i], valid_inverval);
		}

		MtlAnimData animdata;
		animdata.meshname = pMaxNode->GetName();
		animdata.mtlname = (const char *)pdestmtl->getName();
		animdata.varname = value_name;
		animdata.pticks = pticks;
		animdata.pkeyframes = pvalueframes;
		animdata.stride = sizeof(float);
		animdata.nframe = nframe;
		animdata.type = TYPE_FLOAT;

		MtlAnimDataArray.push_back(animdata);
		return true;
	}

	bool CustAttribConverter::ConvertIntAttrib(INode *pMaxNode, Animatable *obj, IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = 
			pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		int int_value;
		Interval valid_inverval;
		BOOL retrieved = false;

		retrieved = pkParamBlock->GetValue(iIndex, 0, int_value, valid_inverval);

		if (retrieved)
		{
			char* value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}

			//如果是灯光，不导出数据
			IDxMaterial *idxm = (IDxMaterial *)obj->GetInterface(IDXMATERIAL_INTERFACE);
			int iNumLights = idxm->GetNumberOfLightParams();
			for (int i=0; i<iNumLights; i++)
			{
				if (!strcmp(value_name, idxm->GetLightParameterName(i)))
				{
					return true;
				}			 
			}

			char *pMapChannel = "mapChannel";
			if (strstr(value_name, pMapChannel))
			{
				//if(m_iMapChannel != -1000)
				//	texcoord.m_Texcoord[m_iMapChannel] = int_value;
				return true;
			}

			pdestmtl->setParamMacro(value_name, int_value);
		}
		return true;

	}

	bool CustAttribConverter::ConvertIntListBoxAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		int int_value;
		Interval valid_inverval;
		BOOL retrieved = false;

		retrieved = pkParamBlock->GetValue(iIndex, 0, int_value, valid_inverval);

		if (retrieved)
		{
			char* value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}

			//3ds Max中数组从1开始,这里的index要减1
			int_value = (int_value==-1)?-1:(int_value-1);
			pdestmtl->setParamValue(value_name, &int_value);
		}
		return true;

	}

	bool CustAttribConverter::ConvertColorAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl, int& iIncrement)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();
		int num_params = pkParamBlock->NumParams();

		if(!pkParamDesc) return false;
		ParamDef& kColorParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));
		ParamDef& kAlphaParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex+1));

		Color color_value;
		float alpha_value = 255.0;
		Interval valid_interval;
		BOOL retrievedColor = false;
		BOOL retrievedAlpha = false;

		int iAlphaIndex = iIndex + 1;

		retrievedColor = pkParamBlock->GetValue(iIndex, 0, color_value, valid_interval);

		if (iAlphaIndex < num_params)
		{
			int param_type = pkParamBlock->GetParameterType(iAlphaIndex);
			if (param_type == TYPE_FLOAT)
			{
				retrievedAlpha = pkParamBlock->GetValue(iAlphaIndex, 0, alpha_value, valid_interval);
			}
		}

		if (retrievedColor)
		{
			char* value_name = kColorParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
			}
			assert(IsValidName(value_name));

			Control * pkColorControl = pkParamBlock->GetController(iIndex, 0);

			bool bHasAlpha = false;
			Control* pkAlphaControl = NULL;
			if (retrievedAlpha)
			{
				TCHAR* value_alpha_name = kAlphaParamDef.int_name;
				if (!IsValidName(value_alpha_name))
					value_name = pkParamBlock->GetLocalName(iAlphaIndex);
				pkAlphaControl = pkParamBlock->GetController(iAlphaIndex, 0);
				assert(IsValidName(value_alpha_name));

				std::string strColorName = value_name;
				strColorName += "Alpha";
				if (!strcmp(strColorName.c_str(), value_alpha_name))
				{
					bHasAlpha = true;
					iIncrement++;
				}
				else
				{
					pkAlphaControl = NULL;
				}
			}

			if(bHasAlpha)
			{
				ColourValue mycolor(color_value.r, color_value.g, color_value.b, alpha_value);
				pdestmtl->setParamValue(value_name, &mycolor);
			}
			else
			{
				ColourValue mycolor(color_value.r, color_value.g, color_value.b, 1.0f);
				pdestmtl->setParamValue(value_name, &mycolor);
			}
		}

		return true;
	}

	bool CustAttribConverter::ConvertPoint3Attrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		Point3 vector_value;
		Interval valid_interval;
		BOOL retrieved = false;

		retrieved =  pkParamBlock->GetValue ( iIndex, 
			0 , vector_value, valid_interval );

		if (retrieved)
		{
			char *value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}

			Vector3 kVector3(vector_value.x, vector_value.y, vector_value.z);
			pdestmtl->setParamValue(value_name, &kVector3);
		}

		return true;
	}

	bool CustAttribConverter::ConvertPoint4Attrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = 
			pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		Point4 vector_value;
		Interval valid_interval;
		BOOL retrieved = false;

		retrieved =  pkParamBlock->GetValue ( iIndex, 
			0 , vector_value, valid_interval );

		if (retrieved)
		{
			char *value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}

			Vector4 kVector4(vector_value.x, vector_value.y, vector_value.z, vector_value.w);
			pdestmtl->setParamValue(value_name, &kVector4);
		}

		return true;
	}

	bool CustAttribConverter::ConvertBoolAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		int int_value;
		Interval valid_interval;
		BOOL retrieved = false;

		retrieved =  pkParamBlock->GetValue (iIndex, 0 , int_value, valid_interval );

		if (retrieved)
		{
			char *value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}

			//bool b = int_value ? true : false;
			pdestmtl->setParamMacro(value_name, int_value);
		}

		return true;
	}

	bool CustAttribConverter::ConvertFloatTabAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = 
			pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		Interval valid_interval;
		BOOL retrieved = true;

		int iCount = pkParamBlock->Count(pkParamDesc->IndextoID(iIndex));
		if (iCount == 0)
			return false;

		float* pkFloats = new float[iCount];
		IParamBlock2* pkParams = pkParamBlock;

		for (int i = 0; i < iCount; i++)
		{
			retrieved &= pkParams->GetValue(iIndex, 0 , pkFloats[i], 
				valid_interval, i);
		}

		if (retrieved && iCount > 0)
		{
			TCHAR* value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}

			pdestmtl->setParamValue(value_name, pkFloats);
		}

		return true;
	}

	bool CustAttribConverter::ConvertFRGBAAttrib(INode *pMaxNode, IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl, std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if(!pkParamDesc) return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		Point4 vector_value;
		Interval valid_interval;
		BOOL retrieved = false;	

		retrieved =  pkParamBlock->GetValue(iIndex, 0 , vector_value, valid_interval);
		if(retrieved)
		{
			TCHAR* value_name = kParamDef.int_name;
			if(!IsValidName(value_name)) value_name = pkParamBlock->GetLocalName( iIndex );
			assert(IsValidName(value_name));

			pdestmtl->setParamValue(value_name, &vector_value.x);

			if(HasAnimation(pkParamBlock, iIndex))
			{
				MtlAnimData animdata;
				animdata.meshname = pMaxNode->GetName();
				animdata.mtlname = (const char *)pdestmtl->getName();
				animdata.varname = value_name;
				animdata.nframe = (m_animEnd-m_animStart)/GetTicksPerFrame() + 1;
				animdata.type = TYPE_FRGBA;
				animdata.stride = sizeof(Vector3);

				Vector3 *pkeyframes = (Vector3 *)malloc(animdata.nframe*sizeof(Vector3));
				unsigned int *pticks = new unsigned int[animdata.nframe];
				for(int i=0; i<animdata.nframe; i++)
				{
					TimeValue tick = m_animStart + i*GetTicksPerFrame();
					pkParamBlock->GetValue(iIndex, tick, vector_value, valid_interval);

					pticks[i] = tick*1000/TIME_TICKSPERSEC;
					pkeyframes[i].x = vector_value.x;
					pkeyframes[i].y = vector_value.y;
					pkeyframes[i].z = vector_value.z;
				}

				animdata.pkeyframes = pkeyframes;
				animdata.pticks = pticks;

				MtlAnimDataArray.push_back(animdata);
			}
		}

		return true;
	}

	bool CustAttribConverter::ConvertBitMapAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));

		PBBitmap *pb;
		Interval valid_interval;
		char* value_name = NULL;
		BOOL retrieved = false;	

		retrieved = pkParamBlock->GetValue(iIndex, 0, pb, valid_interval);

		char strBitMapName[256];
		if(pb)
		{
			if (BMMGetFullFilename(&pb->bi) == TRUE)
			{
				strcpy(strBitMapName, pb->bi.Name());
			}else
			{
				return true;
			}
		}else
		{
			return true;
		}

		if (retrieved)
		{
			value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
				value_name = pkParamBlock->GetLocalName( iIndex );
			assert(IsValidName(value_name));
		}

		if (!SaveTextureChunk(strBitMapName, pdestmtl, value_name))
		{
			return false;
		}

		return true;
	}

	bool CustAttribConverter::ConvertTexMapAttrib(IParamBlock2* pkParamBlock, int iIndex, Ogre::Material *pdestmtl)
	{
		ParamBlockDesc2 *pkParamDesc = pkParamBlock->GetDesc();
		int num_params = pkParamBlock->NumParams();

		if (!pkParamDesc)
			return false;

		ParamDef& kParamDef = 
			pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex));
		ParamDef& kTexSlotParamDef = 
			pkParamDesc->GetParamDef(pkParamDesc->IndextoID(iIndex+1));

		Texmap* Texmap_value;
		int index_value = 0;
		Interval valid_interval;
		BOOL retrievedTexmap = false;
		BOOL retrievedTexSlot = false;
		char* value_name = NULL;
		int iIndexTexSlot = iIndex + 1;

		retrievedTexmap = pkParamBlock->GetValue(iIndex, 0, Texmap_value, valid_interval);

		if (!Texmap_value)
		{
			return false;
		}

		BitmapTex *pbmptex = (BitmapTex*)Texmap_value;
		char* strBitMapName;
		strBitMapName = pbmptex->GetMapName();

		if (retrievedTexmap)
		{
			value_name = kParamDef.int_name;
			if (!IsValidName(value_name))
			{
				value_name = pkParamBlock->GetLocalName(iIndex);
				assert (value_name);
			}
		}

		if (iIndexTexSlot < num_params)
		{
			int param_type = pkParamBlock->GetParameterType(iIndexTexSlot);
			if (param_type == TYPE_INT)
			{
				char* value_index_name = kTexSlotParamDef.int_name;
				if (!IsValidName(value_index_name))
				{
					value_name = pkParamBlock->GetLocalName(iIndexTexSlot);
					assert (IsValidName(value_index_name));
				}

				std::string strTexmapName = value_name;
				strTexmapName += "Index";
				if (!strcmp(strTexmapName.c_str(), value_index_name))
				{
					retrievedTexSlot = pkParamBlock->GetValue (iIndexTexSlot, 
						0 , index_value, valid_interval);
				}
			}
		}

		//获取uv通道iSource
		UVGen           * pkUVGen = Texmap_value->GetTheUVGen();
		StdUVGen        * pkStdUVGen = NULL;

		if (pkUVGen && pkUVGen->IsStdUVGen())
			pkStdUVGen = (StdUVGen*) pkUVGen;

		int iSource = Texmap_value->GetUVWSource();

		if (pkUVGen == NULL)
		{
			return false;
		}
		else if (iSource == UVWSRC_EXPLICIT)
		{
			iSource = Texmap_value->GetMapChannel();
			assert(iSource != 0);
		}
		else if (iSource == UVWSRC_EXPLICIT2)
		{
			// MAX code for "use vertex colors"
			iSource = 0;
			//m_bUseVCForUV = true;
		}
		else
		{
			// nothing really good to do if they use one of the 
			// other uv channels
			return false;
		}

		SaveTextureChunk(strBitMapName, pdestmtl, value_name);

		return true;

	}

	void CustAttribConverter::ChangeTextureNameStyle(const char* str, char * temp)
	{
		//char *temp = new char[200];
		int iCount = 0;
		for (int i=0; i<strlen(str); i++)
		{
			if (str[i] == ':')
				continue;
			if (str[i] == '\\')
			{
				temp[iCount] = '.';
				iCount ++;
				continue;
			}

			temp[iCount] = str[i];
			iCount++;
		}
		temp[iCount] = '\0';

		for (int i=strlen(temp); i>0; i--)
		{
			if (str[i] == '.')
			{
				temp[i-1] = '\0';
				break;
			}
		}

		//return temp;
	}

	bool CustAttribConverter::SaveTextureChunk(const char *texName, Ogre::Material *pdestmtl, const char *texParamName)
	{
		const char *ptmp = strstr(texName, g_ExpOption.src_rootdir);
		if(ptmp == NULL)
		{
			char error[1024];
			sprintf(error, "贴图需要放在贴图根目录里:%s", texName);
			MessageBox(NULL, error, "Error!", MB_OK);
			return false;
		}

		TextureData *ptex = new TextureData;
		ptex->loadFromImageFile(texName);


		char strTexName[256];
		ptmp += strlen(g_ExpOption.src_rootdir);
		if(*ptmp == '\\') ptmp++;
		strcpy(strTexName, ptmp);

		char *pdot = strrchr(strTexName, '.');
		if(pdot)
		{
			strcpy(pdot, ".otex");
		}

		char destpath[256];
		sprintf(destpath, "%s\\%s", g_ExpOption.dest_rootdir, strTexName);
		ptex->save(destpath);
		ptex->setResPath(strTexName);

		pdestmtl->setParamTexture(texParamName, ptex);
		ptex->release();
		return true;
	}
}
