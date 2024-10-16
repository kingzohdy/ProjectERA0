#include ".\mvbmanager.h"

#include "mrender.h"

using namespace TE;
/*
MVBManager g_VBManager;

MVBManager::MVBManager(void)
{
}

MVBManager::~MVBManager(void)
{
	Release();
}
   

LMVB MVBManager::CreateVB(LMDev pDev,void* data,int size,int FVF,const char* name)
{
//   if( g_Render.GetDev() == NULL)
	//	return NULL;

	if(name==NULL)
	{
		LMVB  vb =NULL;
		HRESULT hr = 0;
			
		hr = pDev->CreateVertexBuffer( //g_Render.GetDev()
				size,
				D3DUSAGE_WRITEONLY,
				FVF,
				D3DPOOL_MANAGED,///D3DPOOL_DEFAULT,/内存中创建
				&vb ,
				0);

			if(FAILED(hr))
			{
				//LogSave("setvb创建顶点失败！");
				//MSAFE_RELEASE(vbInfo.vb);
				MSAFE_RELEASE(vb);
				return NULL;
			}
		   
			if(data != NULL)
			{			
				float  *v;
				{
					hr = (vb->Lock( 0,0, (void**)&v, 0 ));
					if(FAILED(hr))
					{
	                     //log error
					}
					else
					{
				     	memcpy(v,data,size);
						vb->Unlock();
					}
				}
			}
	   m_UnkownVBset.push_back (vb);
		return vb;
	}
	
	LMVB  lpvb;

	if(Find(name,lpvb))
	{
		return lpvb ;
	}

	VBINFO* vbInfo = NULL;

    vbInfo = new VBINFO;

	HRESULT hr = 0;
	hr = g_Render.GetDev()->CreateVertexBuffer(
		size,
		D3DUSAGE_WRITEONLY,
		FVF,
		D3DPOOL_DEFAULT,//D3DPOOL_MANAGED,//内存中创建
		&vbInfo->vb ,
		0);

	if(FAILED(hr))
	{
		//LogSave("setvb创建顶点失败！");
		//MSAFE_RELEASE(vbInfo.vb);
		MSAFE_DELETE(vbInfo);
		return NULL;
	}
   
	if(data == NULL)
	{
		
		std::string infoName = name;
		vbInfo->Add ();
		m_vbSet[infoName] = vbInfo;

		return vbInfo->vb;
	}
	
	float  *v;
	{
		vbInfo->vb->Lock( 0,0, (void**)&v, 0 );
		memcpy(v,data,size);
		vbInfo->vb->Unlock();
	}
			
	std::string infoName = name;	
	vbInfo->Add ();
	m_vbSet[infoName] = vbInfo;

	return vbInfo->vb ;

};
	
bool  MVBManager::Find(const char* info,LMVB& vbInfo)
{
	std::string strInfo = info;
	VBSET::iterator it = m_vbSet.find (strInfo);

	if(it == m_vbSet.end ())
	{
		return false;
	}
	
	it->second->Add ();

	vbInfo = it->second->vb;

	return true;

};

void MVBManager::Release()
{
	VBSET::iterator it;

	for(it = m_vbSet.begin () ; it != m_vbSet.end () ; it++)
	{
		MSAFE_RELEASE(it->second ->vb );
		MSAFE_DELETE(it->second );
	}

	m_vbSet.clear ();

	for(int i = 0 ; i < m_UnkownVBset.size () ; i++)
	{
		MSAFE_RELEASE(m_UnkownVBset[i]);
	}
	m_UnkownVBset.clear ();


};

bool MVBManager::DelVB(const char* info)
{
	std::string strInfo = info;

	VBSET::iterator  it = m_vbSet.find (info);
	if(it == m_vbSet.end ())
		return false;

	if(it->second ->Del ())
	{
		MSAFE_DELETE(it->second );
		m_vbSet.erase (it);
		return true;
	}
	else
	{
		return false;
	}
};
LMVB MVBManager::GetVB(const char* info)
{
	//VBINFO* vbInfo = NULL;
     LMVB lpVB;
	if(Find(info,lpVB))
	{
		return lpVB ;
	}

	return NULL;

};*/