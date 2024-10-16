
#pragma once
#include "TerrainEditTypes.h"

#include <stdio.h>

namespace TE
{
//��ʱ�����ļ�д����������Ϊ6M.
	//�޸ĺ�֧�������С��һ���ԣ�д�����ݲ����� 128*1024
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
	//����֧��д�����С�ļ�
	bool           Flush();
private:
	//�ļ�ָ��
	FILE*          m_file;
	//����ָ��
	char*          m_pSave;
	//����ָ��
	char*          m_pData;
	//����
	int            m_nLength;
	//��ǰλ��
	int            m_nCurPos;
	//�ļ���
	char           m_strFileName[NAME_LENGTH];
};

};
