// DesFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "des.h"
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

std::string gStrKey = "@$#^!1345^&()";

int _tmain(int argc, _TCHAR* argv[])
{
	system("Pause");
	if ( argc < 2 )
	{
		return 0;
	}
	
	std::string strPath		= argv[1];
	std::string strFileType;
	std::string::size_type findPos = strPath.find_last_of(".");
	// Ŀǰ��֧�ֲ���.�ļ��ļ��ܽ��ܣ��Ժ���Կ�����չ
	if ( findPos == std::string::npos )
	{
		return 0;
	}
	
	strFileType				= strPath.substr( findPos + 1 );
	EnCodeOp nCurEncodeOp	= CLIENT_JIAMI;
	if( strFileType == "data" )
	{
		nCurEncodeOp = CLIENT_JIEMI;
	}

	FILE* pFileHandle = fopen( argv[1], "rb" );
	if ( pFileHandle == NULL )
	{
		return 0;
	}

	fseek( pFileHandle, 0, SEEK_END );
	size_t size = ftell( pFileHandle );
	fseek( pFileHandle, 0, SEEK_SET );
	size_t nAllocBuffSize = ( (size+7)&0xfffffff8 ) >> 3;
	nAllocBuffSize *= 8;
	nAllocBuffSize += 1;
	
	char* pszSrcBinaryBuffer	= new char[nAllocBuffSize];
	char* pEncodeBuffer			= new char[nAllocBuffSize];
	memset( pszSrcBinaryBuffer, 0, nAllocBuffSize );
	memset( pEncodeBuffer,		0, nAllocBuffSize );
	if ( 1 != fread( pszSrcBinaryBuffer, size, 1, pFileHandle ) )
	{
		printf("Դ�ļ���ȡʧ��\n");
		fclose(pFileHandle);
		delete[] pszSrcBinaryBuffer;
		return 0;
	}

	// ����
	Key_Go( pEncodeBuffer, pszSrcBinaryBuffer, size, gStrKey.c_str(), gStrKey.length(), nCurEncodeOp );

	fclose(pFileHandle);
	delete[] pszSrcBinaryBuffer;

	// д�����ļ�
	strPath = strPath.substr( 0, strPath.find_last_of( "." ) );
	findPos = strPath.find_last_of("\\");
	if ( findPos != string::npos )
	{
		strPath = strPath.substr( findPos + 1 );
	}

	if( nCurEncodeOp == CLIENT_JIAMI )
	{
		strPath = strPath + ".data";
	}
	else
	{
		strPath = strPath + ".txt";
	}
	
	std::string strCurDir = argv[0];
	findPos = strCurDir.find_last_of("\\");
	if ( findPos != string::npos )
	{
		strCurDir = strCurDir.substr( 0, findPos + 1 );
	}
	
	// ��Ϊ�����ɹ�����������ļ�����·��
	strPath = strCurDir + strPath;
	FILE* fp = fopen( strPath.c_str(), "wb" );
	if( fp != NULL )
	{
		fwrite( pEncodeBuffer, nAllocBuffSize - 1, 1, fp );
		fclose( fp );
	}
	else
	{
		if( nCurEncodeOp == CLIENT_JIAMI )
		{
			printf("�����ļ�����ʧ��ʧ��\n");
		}
		else
		{
			printf("�����ļ�����ʧ��ʧ��\n");
		}
	}
	delete[] pEncodeBuffer;
	
	if( nCurEncodeOp == CLIENT_JIAMI )
	{
		printf("���ܳɹ������������\n");
		UpdateSelfCompleteCheckInfo();
	}
	else
	{
		printf("���ܳɹ������������\n");
	}
	getchar();
	return 0;
}

