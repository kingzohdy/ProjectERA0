
#include "EditorStream.h"
#include <windows.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace TE
{
	EditorStream::EditorStream(void)
	{
		m_pData = NULL;
		m_pSave = 0;
		m_nLength = 0;
		m_nCurPos = 0;
		m_strFileName[0] = 0;

		m_file = NULL ;


	}

	EditorStream::~EditorStream(void)
	{
		OGRE_DELETE_ARRAY(m_pSave);
		m_nLength = 0;
		m_nCurPos = 0;
		m_strFileName[0] = 0;

		if(m_file)
		{
			fclose(m_file);
		}
	}

	bool  EditorStream::ReadFile(const char* filename)
	{
		if(filename == NULL)
		{
			char logInfo[256];
			sprintf( logInfo , "%s 文件加载失败!" ,filename );
			MessageBox( NULL ,  logInfo , "提示" , MB_OK );
			return false;
		}

		FILE* fp  = fopen(filename,"rb");

		if(fp==NULL)
			return false;

		strncpy(m_strFileName,filename,NAME_LENGTH);

		struct stat statbuf;
		stat(filename, &statbuf);
		m_nLength = statbuf.st_size;

		//m_nLength = filelength (fileno (fp)); 

		m_pData = new char [m_nLength];
		if(m_pData == NULL)
			assert(0);

		m_pSave = m_pData;

		fread(m_pData,m_nLength,1,fp);
		fclose(fp);

		m_nCurPos = 0;

		return true;

	}; 
	bool  EditorStream::WriteFile(const char* filename)
	{	
		if(filename == NULL)
			return false;

		OGRE_DELETE_ARRAY(m_pSave);
		m_nCurPos = m_nLength = 0;

		strncpy(m_strFileName,filename,NAME_LENGTH);

		m_pSave = m_pData = new char [WRITE_FILE_MAX_LENGTH];
		if( m_pData == NULL )
		{
			return false;
		}

		m_file  = fopen(m_strFileName,"wb");
		if( m_file == NULL )
			return false;

		return true;

	};

	void EditorStream::Rewind()
	{
		m_pData -= m_nCurPos;
		m_nCurPos = 0;
	}

	bool  EditorStream::Read(void* data,int size) 
	{
		if(m_nCurPos + size > m_nLength)
			return false;

		memcpy(data,m_pData,size);
		m_pData += size;

		m_nCurPos += size;
		return true;
	};

	bool EditorStream::ReadFloat(float* data,int num )
	{
		if((m_nCurPos + sizeof(float)*num) > m_nLength)
			return false;   

		memcpy(data,m_pData,sizeof(float)*num);
		m_pData += sizeof(float)*num;

		m_nCurPos += sizeof(float)*num;
		return true;
	}
	bool EditorStream::ReadUnChar(unsigned char* data,int num )
	{
		if((m_nCurPos + sizeof(unsigned char)*num) > m_nLength)
			return false;   

		memcpy(data,m_pData,sizeof(unsigned char)*num);
		m_pData += num;

		m_nCurPos += sizeof(unsigned char)*num;
		return true;
	};

	bool EditorStream::ReadChar(char* data,int num )
	{
		if((m_nCurPos + sizeof(char)*num) > m_nLength)
			return false;   

		memcpy(data,m_pData,sizeof(char)*num);
		m_pData += num;

		m_nCurPos += sizeof(char)*num;
		return true;
	};

	bool EditorStream::ReadInt(int* data,int num )
	{
		if((m_nCurPos + sizeof(int)*num) > m_nLength)
			return false;   

		memcpy(data,m_pData,sizeof(int)*num);
		m_pData += num*sizeof(int);

		m_nCurPos += sizeof(int)*num;

		return true;
	};

	//增加支持写任意大小文件
	bool  EditorStream::Flush()
	{
		if ( fwrite(m_pSave,m_nLength,1,m_file) <= 0)
			return false;

		m_pData = m_pSave ;
		m_nCurPos = 0 ;
		m_nLength = 0 ;

		return true;
	}

	bool  EditorStream::Write(void* data,int size)
	{
		if((m_nCurPos + size) > WRITE_FILE_MAX_LENGTH)
		{
			if( !Flush())
			{
				assert(0);
				return false;
			}
		}

		memcpy(m_pData,data,size);

		m_pData += size;

		m_nCurPos += size;
		m_nLength = m_nCurPos;

		return true;

	};
	bool EditorStream::WriteFloat(float* data,int num )
	{
		if(m_nCurPos + sizeof(float)*num > WRITE_FILE_MAX_LENGTH)
		{
			if( !Flush() )
			{
				assert(0);
				return false;
			}
		}

		memcpy(m_pData,data,sizeof(float)*num);

		m_pData += sizeof(float)*num;

		m_nCurPos += sizeof(float)*num;
		m_nLength = m_nCurPos;
		return true;
	};

	bool EditorStream::WriteUnChar(unsigned char* data,int num)
	{
		if((m_nCurPos + sizeof(unsigned char)*num) > WRITE_FILE_MAX_LENGTH)
		{
			if( !Flush())
			{
				assert(0);
				return false;
			}
		}

		memcpy(m_pData,data,sizeof(unsigned char)*num);

		m_pData += sizeof(unsigned char)*num;

		m_nCurPos += sizeof(unsigned char)*num;
		m_nLength = m_nCurPos;

		return true;
	};	

	bool EditorStream::WriteChar(char* data,int num )
	{
		if((m_nCurPos + sizeof(char)*num) > WRITE_FILE_MAX_LENGTH)
		{
			if( !Flush() )
			{
				assert(0);
				return false;
			}
		}

		memcpy(m_pData,data,sizeof(char)*num);

		m_pData += sizeof(unsigned char)*num;

		m_nCurPos += sizeof(char)*num;
		m_nLength = m_nCurPos;

		return true;

	};

	bool EditorStream::WriteInt(int* data,int num)
	{
		if((m_nCurPos + sizeof(int)*num) > WRITE_FILE_MAX_LENGTH)
		{
			if( !Flush())
			{
				assert(0);
				return false;
			}
		}

		memcpy(m_pData,data,sizeof(int)*num);

		m_pData += sizeof(int)*num;

		m_nCurPos += sizeof(int)*num;
		m_nLength = m_nCurPos;

		return true;
	};

	bool  EditorStream::WriteEnd()
	{
		if(m_file==NULL)
			return false;

		fwrite(m_pSave,m_nLength,1,m_file);
		fclose(m_file);

		return true;
	}
}
