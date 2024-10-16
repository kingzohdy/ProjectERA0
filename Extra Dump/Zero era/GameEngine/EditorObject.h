
#ifndef __EditorObject_H__
#define __EditorObject_H__

#include <vector>

struct VarArgument
{
	enum
	{
		VA_INT,
		VA_Number,
		VA_STRING,
		VA_VECTOR3,
		VA_QUATERNION,
		VA_NONE
	};
	char	mVarname[32];
	union
	{//value
		char	mStr[64];
		int		mInt;
		//Vector3 mVector3;
		//Queration mQueration;
		double	mNumber;
	};
	int		mValueType;
	char	mVarDesc[64];

	VarArgument( const char* Varname,int Int ):mInt( Int ),mValueType( VA_INT )
	{
		_tcsncpy( mVarname,Varname,32 );
	}
	VarArgument( const char* Varname,FLOAT Number ):mNumber( Number ),mValueType( VA_Number )
	{
		_tcsncpy( mVarname,Varname,32 );
	}
	VarArgument( const char* Varname,double Number ):mNumber( Number ),mValueType( VA_Number )
	{
		_tcsncpy( mVarname,Varname,32 );
	}
	VarArgument( const char* Varname,const char* Str ):mValueType( VA_STRING )
	{
		_tcsncpy( mStr,Str,64 );
		_tcsncpy( mVarname,Varname,32 );
	}
	VarArgument( ):mValueType( VA_NONE )
	{}

	void Get( int& vInt )const
	{
		vInt	= mInt;
	}
	void Get( FLOAT& vFloat )const
	{
		vFloat	= (FLOAT)mNumber;
	}
	void Get( double& vDouble )
	{
		vDouble	= mNumber;
	}
	void Get( char* tBuf,size_t BufLen )const
	{
		_sntprintf( tBuf,BufLen,mStr );
	}
	const char* Getname( )const
	{
		return mVarname;
	}
	bool Is( const char* Valname )const
	{
		if( _tcscmp( Valname,mVarname ) == 0 )
			return TRUE;
		return FALSE;
	}
};

class IEditObject
{
public:
	virtual void GetVariableList( std::vector<VarArgument>& Varlist )	= 0;
	virtual void SetVariables( const std::vector<VarArgument>& Varlist)	= 0;
};


#endif