#include ".\mnavigationmap.h"


#include <memory.h>
#include "mstream.h"

#include <windows.h>

using namespace TE;

MNavigationMap g_NavMap;

MNavigationMap::MNavigationMap(void)
{
	memset( &m_NavData , 0 ,sizeof( MNavData ));
	memset( &m_Head, 0 , sizeof( MNavHead ));
}

MNavigationMap::~MNavigationMap(void)
{
     Release();
}
	
void    MNavigationMap::Release()
{
	OGRE_DELETE_ARRAY(m_NavData.pdata);
};

bool MNavigationMap::SaveBmpFile( const char* filename, int layer)
{
	FILE *fp = fopen(filename, "wb");
	if(fp == NULL)
	{
		return false;
	}
		
	BITMAPFILEHEADER header;	
	int num = m_Head.ngridx * m_Head.ngridz  ;
	header.bfSize = num + 1078;
	header.bfType = (WORD)19778;
	header.bfOffBits = (DWORD)1078;//灰度图，所以便宜为 256*4 ,
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;

	BITMAPINFOHEADER  headInfo;

	headInfo.biWidth = m_Head.ngridx ;
	headInfo.biHeight = m_Head.ngridz  ;
	headInfo.biBitCount = 8;
	headInfo.biSize = sizeof( BITMAPINFOHEADER );
    headInfo.biSizeImage = m_Head.ngridx * m_Head.ngridz ;
	headInfo.biClrUsed = 0;
	headInfo.biXPelsPerMeter = 0;
	headInfo.biYPelsPerMeter = 0;
	headInfo.biClrImportant = 0;
	headInfo.biCompression = 0;
	headInfo.biPlanes = 1;

	fwrite(&header, sizeof(header), 1, fp);
	fwrite(&headInfo, sizeof(headInfo), 1, fp);

	unsigned char data[4];
	for( int i = 0 ; i < 256 ; i++)
	{
		data[0] = data[1] = data[2] = (unsigned char)i;
		data[3] = 0xff;
		fwrite(data, 4, 1, fp);
	}

	unsigned char *pdata = new unsigned char[num];
	for(int i=0; i<num; i++)
	{
		if(m_NavData.pdata[i] & (1<<layer)) pdata[i] = 0xff;
		else pdata[i] = 0;
	}
	fwrite(pdata, num, 1, fp);
	delete[] pdata;

	fclose(fp);
	return true;
}

bool GetMaskWalkablePoint(unsigned char *pbits, int ngridx, int ngridz, int &ret_x, int &ret_z)
{
	int x, z;
	for(z=0; z<ngridz; z++)
	{
		for(x=0; x<ngridx; x++)
		{
			if((pbits[z*ngridx+x]&1) == 0)
			{
				ret_x = x;
				ret_z = z;
				return true;
			}
		}
	}
	return false;
}

void TestMaskPoint(unsigned char *pbits, unsigned char *pvisitbits, int index, std::vector<int>&myheap)
{
	if((pbits[index]&1)==0 && pvisitbits[index]==0)
	{
		myheap.push_back(index);
		pvisitbits[index] = 1;
	}
}
bool CheckMaskMapLinked(unsigned char *pbits, int ngridx, int ngridz, int &ret_x, int &ret_z)
{
	int x, z;
	if(!GetMaskWalkablePoint(pbits, ngridx, ngridz, x, z)) return true;

	unsigned char *pvisitbits = new unsigned char[ngridx*ngridz];
	memset(pvisitbits, 0, ngridx*ngridz);

	std::vector<int>myheap;
	myheap.push_back(z*ngridx+x);
	pvisitbits[z*ngridx+x] = 1;

	while(!myheap.empty())
	{
		int index = myheap.back();
		myheap.pop_back();

		z = index/ngridx;
		x = index-z*ngridx;
		if(x-1 >= 0) TestMaskPoint(pbits, pvisitbits, z*ngridx+x-1, myheap);
		if(z-1 >= 0) TestMaskPoint(pbits, pvisitbits, (z-1)*ngridx+x, myheap);
		if(x+1 < ngridx) TestMaskPoint(pbits, pvisitbits, z*ngridx+x+1, myheap);
		if(z+1 < ngridz) TestMaskPoint(pbits, pvisitbits, (z+1)*ngridx+x, myheap);
	}

	for(z=0; z<ngridz; z++)
	{
		for(x=0; x<ngridx; x++)
		{
			if((pbits[z*ngridx+x]&1)==0 && pvisitbits[z*ngridx+x]==0)//能走但没有访问的点
			{
				ret_x = x;
				ret_z = z;
				return false;
			}
		}
	}

	return true;
}

bool MNavigationMap::SaveNavFile (const char* filename)
{
	int ret_x, ret_z;
	if(!CheckMaskMapLinked(m_NavData.pdata, m_Head.ngridx, m_Head.ngridz, ret_x, ret_z))
	{
		char buffer[256];
		sprintf(buffer, "这个点和其他点不能连通，请检查：x=%d, z=%d", ret_x*m_Head.gridsizex, ret_z*m_Head.gridsizez);
		MessageBox(NULL, buffer, "行走层检测到可能的错误", MB_OK);
	}

	FILE *fp = fopen(filename, "wb");
	if(fp == NULL) return false;

	fwrite(&m_Head, sizeof(m_Head), 1, fp);

	int num = m_Head.ngridx * m_Head.ngridz  ;

	fwrite(m_NavData.pdata, num, 1, fp);

	fclose(fp);
	return true;
}
