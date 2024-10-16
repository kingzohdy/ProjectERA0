
#pragma once
#include "TerrainEditTypes.h"

#include <stdio.h>

namespace TE
{
//暂时设置文件写入的最多数据为6M.
	//修改后，支持任意大小，一次性，写入数据不超过 128*1024
#define WRITE_FILE_MAX_LENGTH  12*1024*1024

class MStream
{
public:
	MStream(void);
	~MStream(void);

	virtual bool    ReadFile(const char* filename); 
	virtual bool    WriteFile(const char* filename);
	virtual bool    Read(void* data,int size) ;

	void Rewind();

	bool ReadFloat(float* data,int num = 1);
	bool ReadChar(char* data,int num = 1);
	bool ReadInt(int* data,int num = 1);
	bool ReadUnChar(unsigned char* data,int num = 1);

	virtual bool    Write(void* data,int size);
	virtual bool    WriteEnd();

	bool WriteFloat(float* data,int num = 1);
	bool WriteChar(char* data,int num = 1);
	bool WriteInt(int* data,int num = 1);
	bool WriteUnChar(unsigned char* data,int num=1);

    template<class T>
	bool Read( T &data )
	{		
		return Read( (void*)&data, sizeof(T) );
	}

	template<class T>
	bool Write( const T &data )
	{
		return Write( (void*)&data, sizeof(T) );
	}
private:
	//增加支持写任意大小文件
	bool           Flush();
private:
	//文件指针
	FILE*          m_file;
	//保存指针
	char*          m_pSave;
	//数据指针
	char*          m_pData;
	//长度
	int            m_nLength;
	//当前位置
	int            m_nCurPos;
	//文件名
	char           m_strFileName[NAME_LENGTH];
};

};
