#pragma once

#include "OgreLog.h"
#include "TerrainEditTypes.h"
#include <windows.h>
#include <windef.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <errno.h>

#include <direct.h>

#define INDEX_NONE   -1
#define UBOOL        int
enum {MAXINT		= 0x7fffffff };

#define ARRAY_COUNT( array ) \
	( sizeof(array) / sizeof((array)[0]) )

namespace Ogre
{

class  MArchive
{
public:

	virtual ~MArchive()
	{}
	virtual void Serialize( void* V, INT Length )
	{}
	virtual void SerializeBits( void* V, INT LengthBits )
	{
		Serialize( V, (LengthBits+7)/8 );
		if( IsLoading() )
		{
			((BYTE*)V)[LengthBits/8] &= ((1<<(LengthBits&7))-1);
		}
	}
	virtual void SerializeInt( DWORD& Value, DWORD Max )
	{
		ByteOrderSerialize( &Value, sizeof(Value) );
	}

	virtual void CountBytes( SIZE_T InNum, SIZE_T InMax )
	{}



	virtual INT Tell()
	{
		return INDEX_NONE;
	}
	virtual INT TotalSize()
	{
		return INDEX_NONE;
	}
	virtual int AtEnd()
	{
		INT Pos = Tell();
		return Pos!=INDEX_NONE && Pos>=TotalSize();
	}
	virtual void Seek( INT InPos )
	{}


	virtual int Precache( INT PrecacheOffset, INT PrecacheSize )
	{
		return TRUE;
	}

	virtual void Flush()
	{}
	virtual int Close()
	{
		return !m_ubArIsError;
	}
	virtual int GetError()
	{
		return m_ubArIsError;
	}


	MArchive& ByteOrderSerialize( void* V, int Length )
	{
		UBOOL SwapBytes = m_ubArForceByteSwapping;

		if( SwapBytes )
		{
			for( INT i=Length-1; i>=0; i-- )
				Serialize( (BYTE*)V + i, 1 );
		}
		else
		{
			Serialize( V, Length );
		}
		return *this;
	}

	void StartSerializingDefaults() 
	{
		m_nArSerializingDefaults++;
	}

	void StopSerializingDefaults() 
	{
		m_nArSerializingDefaults--;
	}

	int AllowEliminatingReferences( bool Allow )
	{
		m_nArAllowEliminatingReferences += Allow ? 1 : -1;
		return IsAllowingReferenceElimination();
	}

	MArchive()
	{
		Reset();
	}

	FORCEINLINE INT Ver()								const	{return m_nArVer;}
	FORCEINLINE INT NetVer()							const	{return m_nArNetVer&0x7fffffff;}
	FORCEINLINE INT LicenseeVer()						const	{return m_nArLicenseeVer;}
	FORCEINLINE int IsLoading()						const	{return m_ubArIsLoading;}
	FORCEINLINE int IsSaving()						const	{return m_ubArIsSaving;}

	FORCEINLINE int IsNet()							const	{return (m_nArNetVer&0x80000000)!=0;}
	FORCEINLINE int IsPersistent()					const	{return m_ubArIsPersistent;}
	FORCEINLINE int IsError()							const	{return m_ubArIsError;}
	FORCEINLINE int IsCriticalError()					const	{return m_ubArIsCriticalError;}
	FORCEINLINE int ForEdit()							const	{return m_ubArForEdit;}
	FORCEINLINE int ForClient()						const	{return m_ubArForClient;}
	FORCEINLINE int ForServer()						const	{return m_ubArForServer;}
	FORCEINLINE int ContainsCode()					const	{return m_ubArContainsCode;}
	FORCEINLINE int ContainsMap()						const	{return m_ubArContainsMap;}
	FORCEINLINE int ForceByteSwapping()				const	{return m_ubArForceByteSwapping;}
	FORCEINLINE int IsSerializingDefaults()			const	{return m_nArSerializingDefaults > 0 ? TRUE : FALSE;}
	FORCEINLINE int IsIgnoringArchetypeRef()			const	{return m_ubArIgnoreArchetypeRef;}
	FORCEINLINE int IsIgnoringOuterRef()				const	{return m_ubArIgnoreOuterRef;}
	FORCEINLINE int IsIgnoringClassRef()				const	{return m_ubArIgnoreClassRef;}
	FORCEINLINE int IsAllowingReferenceElimination()	const	{return m_nArAllowEliminatingReferences > 0 ? TRUE : FALSE;}
	FORCEINLINE int IsAllowingLazyLoading()			const	{return m_ubArAllowLazyLoading;}
	FORCEINLINE int IsObjectReferenceCollector()		const	{return m_ubArIsObjectReferenceCollector;}
	FORCEINLINE int IsCountingMemory()				const	{return m_ubArIsCountingMemory;}
	FORCEINLINE int GetPortFlags()					const	{return m_dwArPortFlags;}
	FORCEINLINE int HasAnyPortFlags( DWORD Flags )	const	{return (m_dwArPortFlags&Flags) != 0;}
	FORCEINLINE int HasAllPortFlags( DWORD Flags )	const	{return (m_dwArPortFlags&Flags) == Flags;}
	FORCEINLINE int ShouldSkipBulkData()				const	{return m_ubArShouldSkipBulkData;}


	void SetVer(INT Ver)			{ m_nArVer = Ver; }

	void SetLicenseeVer(INT Ver)	{ m_nArLicenseeVer = Ver; }

	void SetByteSwapping( int Enabled )	{ m_ubArForceByteSwapping = Enabled; }

	void SetPortFlags( unsigned long InPortFlags )	{ m_dwArPortFlags = InPortFlags; }

	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, char& C )
	{
		Ar.Serialize( &C, 1 );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, unsigned char& B )
	{
		Ar.Serialize( &B, 1 );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, signed char& B )
	{
		Ar.Serialize( &B, 1 );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, unsigned short& W )
	{
		Ar.ByteOrderSerialize( &W, sizeof(W) );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, short& S )
	{
		Ar.ByteOrderSerialize( &S, sizeof(S) );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, unsigned int& D )
	{
		Ar.ByteOrderSerialize( &D, sizeof(D) );
		return Ar;
	}


	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, int& I )
	{
		Ar.ByteOrderSerialize( &I, sizeof(I) );
		return Ar;
	}

	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, float& F )
	{
		Ar.ByteOrderSerialize( &F, sizeof(F) );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, double& F )
	{
		Ar.ByteOrderSerialize( &F, sizeof(F) );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive &Ar, unsigned __int64& Q )
	{
		Ar.ByteOrderSerialize( &Q, sizeof(Q) );
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, __int64& S )
	{
		Ar.ByteOrderSerialize( &S, sizeof(S) );
		return Ar;
	}

	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, MVec3& v )
	{
		Ar << v.x << v.y << v.z ;
		return Ar;
	}
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, MVec2& v )
	{
		Ar << v.x << v.y ;
		return Ar;
	}
	
	FORCEINLINE friend MArchive& operator<<( MArchive& Ar, MVec4& v )
	{
		Ar << v.x << v.y << v.z << v.w ;
		return Ar;
	}
protected:
	INT m_nArVer;
	INT m_nArNetVer;
	INT m_nArLicenseeVer;
	UBOOL m_ubArIsLoading;
	UBOOL m_ubArIsSaving;
	UBOOL m_ubArIsTransacting;
	UBOOL m_ubArIsPersistent;
	UBOOL m_ubArForEdit;
	UBOOL m_ubArForClient;
	UBOOL m_ubArForServer;
	UBOOL m_ubArIsError;
	UBOOL m_ubArIsCriticalError;

	UBOOL m_ubArContainsCode;
	UBOOL m_ubArContainsMap;
	UBOOL m_ubArForceByteSwapping;

	INT m_nArSerializingDefaults;
	UBOOL m_ubArIgnoreArchetypeRef;
	UBOOL m_ubArIgnoreOuterRef;
	UBOOL m_ubArIgnoreClassRef;

	INT m_nArAllowEliminatingReferences;										
	UBOOL m_ubArAllowLazyLoading;
	UBOOL m_ubArIsObjectReferenceCollector;
	UBOOL m_ubArIsCountingMemory;

	INT m_nStopper;
	DWORD m_dwArPortFlags;
	UBOOL m_ubArShouldSkipBulkData;

	void Reset(void)
	{
		m_nArVer							= 1;
		m_nArNetVer						= 1;
		m_nArLicenseeVer					= 1;
		m_ubArIsLoading						= FALSE;
		m_ubArIsSaving						= FALSE;
		m_ubArIsTransacting					= FALSE;
		m_ubArIsPersistent					= FALSE;
		m_ubArIsError						= FALSE;
		m_ubArIsCriticalError				= FALSE;
		m_ubArForEdit						= TRUE;
		m_ubArForClient						= TRUE;
		m_ubArForServer						= TRUE;
		m_ubArContainsCode					= FALSE;
		m_ubArContainsMap					= FALSE;
		m_ubArForceByteSwapping				= FALSE;
		m_nArSerializingDefaults			= 0;
		m_ubArIgnoreArchetypeRef			= FALSE;
		m_ubArIgnoreOuterRef				= FALSE;
		m_ubArIgnoreClassRef				= FALSE;
		m_nArAllowEliminatingReferences	= 1;
		m_ubArAllowLazyLoading				= FALSE;
		m_ubArIsObjectReferenceCollector	= FALSE;
		m_ubArIsCountingMemory				= FALSE;
		m_nStopper							= INDEX_NONE;
		m_dwArPortFlags						= 0;
		m_ubArShouldSkipBulkData			= FALSE;
	}
};

// File manager.
class  MArchiveFileReader : public MArchive
{
public:
	MArchiveFileReader( FILE* InFile,  int InSize )
	:	m_pFile			( InFile )
	,	m_nSize			( InSize )
	,	m_nPos				( 0 )
	,	m_nBufferBase		( 0 )
	,	m_nBufferCount		( 0 )
	{
		fseek( m_pFile, 0, SEEK_SET );
		m_ubArIsLoading = m_ubArIsPersistent = 1;
	}
	~MArchiveFileReader()
	{
		if( m_pFile )
			Close();
	}

	void Seek( INT InPos )
	{
		if( fseek(m_pFile,InPos,SEEK_SET) )
		{
			m_ubArIsError = 1;

		}
		m_nPos         = InPos;
		m_nBufferBase  = m_nPos;
		m_nBufferCount = 0;
	}
	INT Tell()
	{
		return m_nPos;
	}
	INT TotalSize()
	{
		return m_nSize;
	}
	UBOOL Close()
	{
		if( m_pFile )
		{
			fclose( m_pFile );
		}
		m_pFile = NULL;
		return !m_ubArIsError;
	}
	void Serialize( void* V, INT Length )
	{
		while( Length>0 )
		{
			INT Copy = min( Length, m_nBufferBase+m_nBufferCount-m_nPos );
			if( Copy==0 )
			{
				if( Length >= ARRAY_COUNT(m_ucBuffer) )
				{
					if( fread( V, Length, 1, m_pFile )!=1 )
					{
						m_ubArIsError = 1;
						LOG_SEVERE( "fread failed: Length=%i " , Length  );
					}
					m_nPos += Length;
					m_nBufferBase += Length;
					return;
				}
				InternalPrecache( m_nPos, MAXINT );
				Copy = min( Length, m_nBufferBase+m_nBufferCount-m_nPos );
				if( Copy<=0 )
				{
					m_ubArIsError = 1;
				    LOG_SEVERE( "ReadFile beyond EOF %i+%i/%i" , m_nPos, Length, m_nSize );
				}
				if( m_ubArIsError )
				{
					return;
				}
			}
			memcpy( V, m_ucBuffer+m_nPos-m_nBufferBase, Copy );
			m_nPos       += Copy;
			Length    -= Copy;
			V          = (BYTE*)V + Copy;
		}
	}
protected:
	UBOOL InternalPrecache( INT PrecacheOffset, INT PrecacheSize )
	{
		if( m_nPos == PrecacheOffset && (!m_nBufferBase || !m_nBufferCount || m_nBufferBase != m_nPos) )
		{
			m_nBufferBase = m_nPos;
			m_nBufferCount = min( min( PrecacheSize, (INT)(ARRAY_COUNT(m_ucBuffer) - (m_nPos&(ARRAY_COUNT(m_ucBuffer)-1))) ), m_nSize-m_nPos );
			if( fread( m_ucBuffer, m_nBufferCount, 1, m_pFile )!=1 && PrecacheSize!=0 )
			{
				m_ubArIsError = 1;
				LOG_SEVERE( "fread failed: BufferCount=%i ", m_nBufferCount );
			}
		}
		return TRUE;
	}

	FILE*			m_pFile;
	INT				m_nSize;
	INT				m_nPos;
	INT				m_nBufferBase;
	INT				m_nBufferCount;
	BYTE			m_ucBuffer[1024];
};

class  MArchiveFileWriter : public MArchive
{
public:
	MArchiveFileWriter( FILE* InFile )
	:	m_pFile		(InFile)
	,	m_nPos			(0)
	,	m_nBufferCount	(0)
	{
		m_ubArIsSaving = m_ubArIsPersistent = 1;
	}
	~MArchiveFileWriter()
	{
		if( m_pFile )
		{
			Close();
		}
		m_pFile = NULL;
	}
	void Seek( INT InPos )
	{
		Flush();
		if( fseek(m_pFile,InPos,SEEK_SET) )
		{
			m_ubArIsError = 1;
			LOG_SEVERE( "File Seek error !" );
		}
		m_nPos = InPos;
	}
	INT Tell()
	{
		return m_nPos;
	}
	UBOOL Close()
	{
		Flush();
		if( m_pFile && fclose( m_pFile ) )
		{
			m_ubArIsError = 1;
			LOG_SEVERE( "File close error !" );
		}
		return !m_ubArIsError;
	}
	void Serialize( void* V, INT Length )
	{
		m_nPos += Length;
		INT Copy;
		while( Length > (Copy=ARRAY_COUNT(m_ucBuffer)-m_nBufferCount) )
		{
			memcpy( m_ucBuffer+m_nBufferCount, V, Copy );
			m_nBufferCount += Copy;
			Length      -= Copy;
			V            = (BYTE*)V + Copy;
			Flush();
		}
		if( Length )
		{
			memcpy( m_ucBuffer+m_nBufferCount, V, Length );
			m_nBufferCount += Length;
		}
	}
	void Flush()
	{
		if( m_nBufferCount && fwrite( m_ucBuffer, m_nBufferCount, 1, m_pFile )!=1 )
		{
			m_ubArIsError = 1;
			LOG_SEVERE( "file writer error £¡ count : %n " , m_nBufferCount );
		}
		m_nBufferCount=0;
	}
protected:
	FILE*			m_pFile;
	INT				m_nPos;
	INT				m_nBufferCount;
	BYTE			m_ucBuffer[4096];
};


enum  FILE_FLAGS
{
    FILEREAD = 0 ,
    FILEWRITE ,
	FILEWRITE_APPEND ,
};

class  MFileManagerAnsi 
{
public:

	MArchive* CreateFileReader( const char* Filename, DWORD Flags )
	{
		FILE* File = fopen( Filename , TEXT("rb") );
		if( !File )
		{
			LOG_SEVERE( "create File : %s Reader Failed !" , Filename );
			return NULL;
		}
		fseek( File, 0, SEEK_END );
		return new MArchiveFileReader(File,ftell(File));
	};

	MArchive* CreateFileWriter( const char* Filename, DWORD Flags )
	{
		FILE* File = fopen( Filename ,"wb" );
		if( !File )
		{
			LOG_SEVERE( "create File : %s Writer Failed !" , Filename );
			return NULL;
		}

		return new MArchiveFileWriter(File);
	};
};

};

extern Ogre::MFileManagerAnsi   g_FileManager;