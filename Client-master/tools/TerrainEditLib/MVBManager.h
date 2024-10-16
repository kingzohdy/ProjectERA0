#pragma once
#include <vector>
#include <map>
#include <string>
#include "TerrainEditTypes.h"

namespace TE
{

class MVBManager
{
public:
	MVBManager(void);
	~MVBManager(void);

   virtual LMVB CreateVB(void* data,int size,int FVF,const char* name=NULL);
   virtual void Release();

   virtual bool DelVB(const char* info);
   virtual LMVB GetVB(const char* info);
   
private:
	struct VBINFO
	{
		LMVB   vb;
		int    num;

		VBINFO():vb(NULL),num(0){};

		void Add(){num++;}
		//bool Del(){num--;if(num==0){MSAFE_RELEASE(vb);return true;}else return false;}
	};

	bool  Find(const char* info,LMVB& vbInfo);
private:

	std::vector<LMVB>                m_UnkownVBset;

   typedef std::map<std::string,VBINFO*>  VBSET;

   VBSET                            m_vbSet;

};


};
extern TE::MVBManager g_VBManager;