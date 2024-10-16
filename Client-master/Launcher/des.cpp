#include <string>
#include <Windows.h>
#include "des.h"
#include "CIniFile.h"
#include "util/utils.h"
#include <iostream>
using namespace std;

using namespace std;
//enum{ ENCRYPT, DECRYPT};

const int MAX_CHECK_FOR_COMPLETE_FILE_NUM = 15;
const int MAX_CHECK_FOR_EXIST_FILE_NUM = 17;
//static MultiAreaString s_selfCompleteCheckFile[MAX_CHECK_FOR_COMPLETE_FILE_NUM] = 
//{
//	TEXT("data\\common.pkg"),
//	TEXT("Client.exe"),
//	TEXT("d3dx9_36.dll"),
//	TEXT("devil.dll"),
//	TEXT("fmodex.dll"),
//	TEXT("ilu.dll"),
//	TEXT("ilut.dll"),
//	TEXT("Launcher.exe"),
//	TEXT("libmysql.dll"),
//	TEXT("luadll.dll"),
//	TEXT("mfc80.dll"),
//	TEXT("msvcp80.dll"),
//	TEXT("msvcr80.dll"),
//	TEXT("OgreMain.dll"),
//	TEXT("RenderSystem_Direct3D9.dll"),
//	TEXT("zlib1.dll"),
//	TEXT("version.data"),
//	TEXT("serverlist.data")
//};

static const char* SELF_COMPLETE_CHECK_APP = "CompleteCheck"; 
static string s_selfCompleteCheckFile[MAX_CHECK_FOR_COMPLETE_FILE_NUM] = 
{
	/*"data\\common.pkg",*/
	"Client.exe",
	"d3dx9_36.dll",
	"devil.dll",
	"fmodex.dll",
	"ilu.dll",
	"ilut.dll",
	"Launcher.exe",
	"libmysql.dll",
	"luadll.dll",
	"mfc80.dll",
	"msvcp80.dll",
	"msvcr80.dll",
	"OgreMain.dll",
	"RenderSystem_Direct3D9.dll",
	"zlib1.dll",
	//"version.data",
	//"serverlist.data"
};

static string s_existCheckFile[MAX_CHECK_FOR_EXIST_FILE_NUM] = 
{
	"data\\common.pkg",
	"Client.exe",
	"d3dx9_36.dll",
	"devil.dll",
	"fmodex.dll",
	"ilu.dll",
	"ilut.dll",
	"Launcher.exe",
	"libmysql.dll",
	"luadll.dll",
	"mfc80.dll",
	"msvcp80.dll",
	"msvcr80.dll",
	"OgreMain.dll",
	"RenderSystem_Direct3D9.dll",
	"zlib1.dll",
	//"version.data",
	"serverlist.data"
};

// 是否缺少文件
bool isAnyFileNotExist()
{
	char szPath[MAX_PATH] = { '\0' };
	GetCurrentDirectory( MAX_PATH, szPath );
	string strPrePath = szPath;
	strPrePath += "\\";
	
	string strCompletePath;
	for ( int i = 0; i < MAX_CHECK_FOR_EXIST_FILE_NUM; ++i )
	{
		const string& strOnePostPath = s_existCheckFile[i];
		strCompletePath = /*strPrePath +*/ strOnePostPath;
		FILE* pFile = fopen( strCompletePath.c_str(), "r" );
		if ( pFile == NULL )
		{
			return true;
		}
		fclose( pFile );
	}

	return false;
}

// 检验是否有写入完整性信息，若没有，则写入
bool HasMD5Writed()
{
	// 遍历每个文件，并找出version.data对应字段的值
	char szPath[MAX_PATH] = { '\0' };
	GetCurrentDirectory( MAX_PATH, szPath );
	string strPrePath = szPath;
	strPrePath += "\\";
	string strDestWritePath	= /*strPrePath + */"version.data";
	
	cout << "HasMD5Writed--strDestWritePath = " << strDestWritePath << endl; 
	string strCompletePath;
	const int MAX_MD5_LENGH			= 33;
	char szMd5Buffer[MAX_MD5_LENGH] = { '\0' };

	char* pszDecryptBuffer	= getDecryptFileBuffer( strDestWritePath.c_str(), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return false;
	}
	CIniFile iniNewVer;
	iniNewVer.Prepare( pszDecryptBuffer );
	
	char buffer[256] = { 0 };
	iniNewVer.GetString( SELF_COMPLETE_CHECK_APP, s_selfCompleteCheckFile[1].c_str(), "", buffer, sizeof( buffer ) );
	
	return buffer[0] > 0;
}

// 更新完整性自检查信息
// 遍历指定的所有文件，并将其内容写入对应的内存中
// preCondition: opFile中的内容一定是已经解密的buffer
void UpdateSelfCompleteCheckInfo()
{
	char szPath[MAX_PATH] = { '\0' };
	GetCurrentDirectory( MAX_PATH, szPath );
	string strPrePath = szPath;
	strPrePath += "\\";
	string strDestWritePath	= /*strPrePath + */"version.data";
	char* pszDecryptBuffer	= getDecryptFileBuffer( strDestWritePath.c_str(), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return;
	}

	CIniFile iniNewVer;
	iniNewVer.Create( strDestWritePath.c_str() );
	iniNewVer.Prepare( pszDecryptBuffer );

	string strCompletePath;
	for ( int i = 0; i < MAX_CHECK_FOR_COMPLETE_FILE_NUM; ++i )
	{
		const string& strOnePostPath = s_selfCompleteCheckFile[i];
		strCompletePath = /*strPrePath + */strOnePostPath;
		const char* pszMd5 = file_checksum( strCompletePath.c_str() );
		iniNewVer.WriteString( SELF_COMPLETE_CHECK_APP, strOnePostPath.c_str(), pszMd5 );
	}

	iniNewVer.Save();
	iniNewVer.Close();
	delete[] pszDecryptBuffer;

	// 再将version.data文件中的内容进行加密
	DesDecryptFile( strDestWritePath.c_str(), strDestWritePath.c_str(), CLIENT_JIAMI );
}

// 进行完整性自检查
bool IsSelfComplete()
{
#ifdef PACKAGE_LAUNCHER
	if ( isAnyFileNotExist() )
	{
		return false;
	}
#endif
	
	if ( !HasMD5Writed() )
	{
		return true;
	}

	// 遍历每个文件，并找出version.data对应字段的值
	char szPath[MAX_PATH] = { '\0' };
	GetCurrentDirectory( MAX_PATH, szPath );
	string strPrePath = szPath;
	strPrePath += "\\";
	string strDestWritePath	= /*strPrePath + */"version.data";

	string strCompletePath;
	const int MAX_MD5_LENGH			= 33;
	char szMd5Buffer[MAX_MD5_LENGH] = { '\0' };

	char* pszDecryptBuffer	= getDecryptFileBuffer( strDestWritePath.c_str(), CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return false;
	}
	CIniFile iniNewVer;
	iniNewVer.Prepare( pszDecryptBuffer );

	for ( int i = 0; i < MAX_CHECK_FOR_COMPLETE_FILE_NUM; ++i )
	{
		const string& strOnePostPath = s_selfCompleteCheckFile[i];
		strCompletePath = /*strPrePath + */strOnePostPath;
		const char* pszCurFileMd5 = file_checksum( strCompletePath.c_str() );
		iniNewVer.GetString( SELF_COMPLETE_CHECK_APP , strOnePostPath.c_str() ,"" , 
			szMd5Buffer, MAX_MD5_LENGH );
		if ( strncmp( pszCurFileMd5, szMd5Buffer, MAX_MD5_LENGH - 1 ) != 0 )
		{
			iniNewVer.Close();
			delete[] pszDecryptBuffer;
			return false;
		}
	}

	iniNewVer.Close();
	delete[] pszDecryptBuffer;
	return true;
}

char* getDecryptFileBuffer( const char* pszSrcFile, EnCodeOp nCurEncodeOp )
{
	std::string strPath			= pszSrcFile;
	std::string strEncryptKey	= "@$#^!1345^&()";

	FILE* pFileHandle = fopen( pszSrcFile, "rb" );
	if ( pFileHandle == NULL )
	{
		return NULL;
	}

	fseek( pFileHandle, 0, SEEK_END );
	size_t nSrcFileSize = ftell( pFileHandle );
	fseek( pFileHandle, 0, SEEK_SET );
	size_t nAllocBuffSize = ( ( nSrcFileSize + 7 ) & 0xfffffff8 ) >> 3;
	nAllocBuffSize *= 8;
	nAllocBuffSize += 1;

	char* pszSrcBinaryBuffer	= new char[nAllocBuffSize];
	char* pEncodeBuffer			= new char[nAllocBuffSize];
	memset( pszSrcBinaryBuffer, 0, nAllocBuffSize );
	memset( pEncodeBuffer,		0, nAllocBuffSize );
	if ( 1 != fread( pszSrcBinaryBuffer, nSrcFileSize, 1, pFileHandle ) )
	{
		printf("源文件读取失败\n");
		fclose(pFileHandle);
		delete[] pszSrcBinaryBuffer;
		return NULL;
	}

	// 加密
	Key_Go( pEncodeBuffer, pszSrcBinaryBuffer, nSrcFileSize, strEncryptKey.c_str(), 
			strEncryptKey.length(), nCurEncodeOp );
	
	delete[] pszSrcBinaryBuffer;
	fclose(pFileHandle);

	return pEncodeBuffer;
}

void DesDecryptFileFromBuffer( const char* pszSrcContent, const char* pszDestFile, EnCodeOp eOp )
{
	std::string strEncryptKey	= "@$#^!1345^&()";
	EnCodeOp nCurEncodeOp		= eOp;

	size_t nSrcFileSize = strlen( pszSrcContent ) - 1;
	size_t nAllocBuffSize = ( (nSrcFileSize+7)&0xfffffff8 ) >> 3;
	char* pszSrcBinaryBuffer	= new char[nAllocBuffSize];
	char* pEncodeBuffer			= new char[nAllocBuffSize];
	memset( pszSrcBinaryBuffer, 0, nAllocBuffSize );
	memset( pEncodeBuffer,		0, nAllocBuffSize );
	strncpy_s( pszSrcBinaryBuffer, nAllocBuffSize, pszSrcContent, nSrcFileSize + 1 );

	Key_Go( pEncodeBuffer, pszSrcBinaryBuffer, nSrcFileSize, strEncryptKey.c_str(), 
			strEncryptKey.length(), nCurEncodeOp );
	delete[] pszSrcBinaryBuffer;

	FILE* fp = fopen( pszDestFile, "wb" );
	if( fp != NULL )
	{
		fwrite( pEncodeBuffer, nAllocBuffSize - 1, 1, fp );
		fclose( fp );
	}

	delete[] pEncodeBuffer;
	return;
}

void DesDecryptFile( const char* pszSrcFile, const char* pszDestFile, EnCodeOp eOp )
{
	std::string strPath				= pszSrcFile;
	std::string::size_type findPos	= strPath.find_last_of(".");
	std::string strEncryptKey		= "@$#^!1345^&()";
	std::string strFileType;
	// 目前不支持不带.文件的加密解密，以后可以考虑扩展
	if ( findPos == string::npos )
	{
		return;
	}

	strFileType				= strPath.substr( findPos + 1 );
	EnCodeOp nCurEncodeOp	= eOp;

	FILE* pFileHandle = fopen( pszSrcFile, "rb" );
	if ( pFileHandle == NULL )
	{
		return;
	}

	fseek( pFileHandle, 0, SEEK_END );
	size_t nSrcFileSize = ftell( pFileHandle );
	fseek( pFileHandle, 0, SEEK_SET );
	size_t nAllocBuffSize = ( (nSrcFileSize+7)&0xfffffff8 ) >> 3;
	nAllocBuffSize *= 8;
	nAllocBuffSize += 1;

	char* pszSrcBinaryBuffer	= new char[nAllocBuffSize];
	char* pEncodeBuffer			= new char[nAllocBuffSize];
	memset( pszSrcBinaryBuffer, 0, nAllocBuffSize );
	memset( pEncodeBuffer,		0, nAllocBuffSize );
	if ( 1 != fread( pszSrcBinaryBuffer, nSrcFileSize, 1, pFileHandle ) )
	{
		printf("源文件读取失败\n");
		fclose(pFileHandle);
		delete[] pszSrcBinaryBuffer;
		return;
	}

	// 加密
	Key_Go( pEncodeBuffer, pszSrcBinaryBuffer, nSrcFileSize, strEncryptKey.c_str(), 
			strEncryptKey.length(), nCurEncodeOp );

	fclose(pFileHandle);
	delete[] pszSrcBinaryBuffer;

	strPath = pszDestFile;
	FILE* fp = fopen( strPath.c_str(), "wb" );
	if( fp != NULL )
	{
		fwrite( pEncodeBuffer, nAllocBuffSize - 1, 1, fp );
		fclose( fp );
	}

	delete[] pEncodeBuffer;
	return;
}

// initial permutation IP
const static char IP_Table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
// final permutation IP^-1 
const static char IPR_Table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};
// expansion operation matrix
static const char E_Table[48] = {
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};
// 32-bit permutation function P used on the output of the S-boxes 
const static char P_Table[32] = {
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};
// permuted choice table (key) 
const static char PC1_Table[56] = {
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};
// permuted choice key (table) 
const static char PC2_Table[48] = {
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};
// number left rotations of pc1 
const static char LOOP_Table[16] = {
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
// The (in)famous S-boxes 
const static char S_Box[8][4][16] = {
	// S1 
	14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	// S2 
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	// S3 
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	// S4 
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	// S5 
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	// S6 
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	// S7 
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	// S8 
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

//////////////////////////////////////////////////////////////////////////

typedef bool    (*PSubKey)[16][48];

//////////////////////////////////////////////////////////////////////////

static void KEY(char Out[8], char In[8], const PSubKey pSubKey, bool Type);//标准KEY加/解密
static void SetKey(const char* Key, int len);// 设置密钥
static void SetSubKey(PSubKey pSubKey, const char Key[8]);// 设置子密钥
static void F_func(bool In[32], const bool Ki[48]);// f 函数
static void S_func(bool Out[32], const bool In[48]);// S 盒代替
static void Transform(bool *Out, bool *In, const char *Table, int len);// 变换
static void Xor(bool *InA, const bool *InB, int len);// 异或
static void RotateL(bool *In, int len, int loop);// 循环左移
static void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
static void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组

//////////////////////////////////////////////////////////////////////////

static bool SubKey[2][16][48];// 16圈子密钥
static bool Is3DES;// 3次KEY标志
static char Tmp[256], deskey[16];

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Code starts from Line 130
//////////////////////////////////////////////////////////////////////////
bool Key_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type)
{
	datalen=(datalen+7)&0xfffffff8;
    if( !( Out && In && Key && datalen ) ) 
		return false;
	SetKey(Key, keylen);
	if( !Is3DES ) {   // 1次KEY
		for(long i=0,j=datalen>>3; i<j; ++i,Out+=8,In+=8)
			KEY(Out, In, &SubKey[0], Type);
	} else{   // 3次KEY 加密:加(key0)-解(key1)-加(key0) 解密::解(key0)-加(key1)-解(key0)
		for(long i=0,j=datalen>>3; i<j; ++i,Out+=8,In+=8) {
			KEY(Out, In,  &SubKey[0], Type);
			KEY(Out, Out, &SubKey[1], !Type);
			KEY(Out, Out, &SubKey[0], Type);
		}
	}
	return true;
}
void SetKey(const char* Key, int len)
{
	memset(deskey, 0, 16);
	memcpy(deskey, Key, len>16?16:len);
	SetSubKey(&SubKey[0], &deskey[0]);
	Is3DES = len>8 ? (SetSubKey(&SubKey[1], &deskey[8]), true) : false;
}

void KEY(char Out[8], char In[8], const PSubKey pSubKey, bool Type)
{
    static bool M[64], tmp[32], *Li=&M[0], *Ri=&M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP_Table, 64);
    if( Type == CLIENT_JIEMI ){
        for(int i=0; i<16; ++i) {
            memcpy(tmp, Ri, 32);
            F_func(Ri, (*pSubKey)[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, tmp, 32);
        }
    }else{
        for(int i=15; i>=0; --i) {
            memcpy(tmp, Li, 32);
            F_func(Li, (*pSubKey)[i]);
            Xor(Li, Ri, 32);
            memcpy(Ri, tmp, 32);
        }
	}
    Transform(M, M, IPR_Table, 64);
    BitToByte(Out, M, 64);
}
void SetSubKey(PSubKey pSubKey, const char Key[8])
{
    static bool K[64], *KL=&K[0], *KR=&K[28];
    ByteToBit(K, Key, 64);
    Transform(K, K, PC1_Table, 56);
    for(int i=0; i<16; ++i) {
        RotateL(KL, 28, LOOP_Table[i]);
        RotateL(KR, 28, LOOP_Table[i]);
        Transform((*pSubKey)[i], K, PC2_Table, 48);
    }
}
void F_func(bool In[32], const bool Ki[48])
{
    static bool MR[48];
    Transform(MR, In, E_Table, 48);
    Xor(MR, Ki, 48);
    S_func(In, MR);
    Transform(In, In, P_Table, 32);
}
void S_func(bool Out[32], const bool In[48])
{
    for(char i=0,j,k; i<8; ++i,In+=6,Out+=4) {
        j = (In[0]<<1) + In[5];
        k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
		ByteToBit(Out, &S_Box[i][j][k], 4);
    }
}
void Transform(bool *Out, bool *In, const char *Table, int len)
{
    for(int i=0; i<len; ++i)
        Tmp[i] = In[ Table[i]-1 ];
    memcpy(Out, Tmp, len);
}
void Xor(bool *InA, const bool *InB, int len)
{
    for(int i=0; i<len; ++i)
        InA[i] ^= InB[i];
}
void RotateL(bool *In, int len, int loop)
{
    memcpy(Tmp, In, loop);
    memcpy(In, In+loop, len-loop);
    memcpy(In+len-loop, Tmp, loop);
}
void ByteToBit(bool *Out, const char *In, int bits)
{
    for(int i=0; i<bits; ++i)
        Out[i] = (In[i>>3]>>(i&7)) & 1;
}
void BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, bits>>3);
    for(int i=0; i<bits; ++i)
        Out[i>>3] |= In[i]<<(i&7);
}
