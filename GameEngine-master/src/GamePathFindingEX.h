
#ifndef __GamePathFindingEX_H__
#define __GamePathFindingEX_H__

#include "GamePathFinding.h"

#include <list>

#define NULL_AREA_INDEX ((std::numeric_limits<unsigned short>::max)( ))
template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col >
class CAreaPather
{
public:
	struct CloseArea
	{
		const static int CHUNK_ID = 1001;
		bool		mMarked;
		int			mSxChunk;
		int			mSyChunk;
		unsigned short		mIdx;
		int			mLinkNum;
		unsigned short*		mLinksIdx;
		void AddLink( unsigned short Idx  )
		{
			mLinksIdx	= (unsigned short*)::realloc( mLinksIdx,sizeof( unsigned short ) * (mLinkNum+1) );
			mLinksIdx[mLinkNum]	= Idx;
			++mLinkNum;
		}
		CloseArea( int SxChunk,int SyChunk,unsigned short Idx ):mMarked( false ),
			mLinkNum( 0 ),mLinksIdx( NULL ),mSxChunk( SxChunk ),mSyChunk( SyChunk ),mIdx( Idx )
		{}
		CloseArea( )
		{
			::memset( this,0,sizeof(CloseArea) );
		}
		~CloseArea( )
		{
			if( mLinksIdx )
				::free( mLinksIdx );
		}
	};
	struct MapChunk
	{//for pre process
		int mSx;
		int mSy;
		MapChunk( int Sx,int Sy ):mSx( Sx ),mSy( Sy )
		{}
		typedef std::list< std::pair<int,int> > TAreas;
		TAreas	mAreas;
	};
public:
	typedef void ( *TFuncptrDivideCompleteRateCallBack)( float CP );
	typedef CAPatherMap< CAreaPather,_NumX,_NumY > TAPather;
	TFuncptrDivideCompleteRateCallBack	mpDivedeCompletePercentage;
	std::vector<CloseArea*>	mAreas;
	unsigned short		mGrids[ _NumY ][ _NumX ];
	TAPather	mAPather;
	_Map*		mpMap;
	bool		mbSearchArea;
	unsigned int		mChunkH;
	unsigned int		mChunkW;
	MASKBITFILED	mWalkStopMark;
	MapChunk* mpCurDividingChunk;  //for pre process

	bool		mbInMarkedPathFindPath;
	bool		mbUsedFastA;
public:
	void GetAnPath( std::deque< int >& vOutPath,int vBegin,int vEnd,MASKBITFILED Stopmask );

	CAreaPather( _Map* Map ,MASKBITFILED WalkStop):mAPather( this ),mWalkStopMark( WalkStop ),mpMap( Map )
	{
		ClearAll();
	}

	~CAreaPather( )
	{
		ClearAll();
	}

	void ResetMap( _Map* Newmap )
	{
		mpMap	= Newmap;
	}

	CloseArea* IndexToArea( unsigned short Idx )
	{
		return mAreas[ Idx ];
	}

	void DivideArea( );

	bool SaveToFile( const char* Filename );
	bool SaveToFile( FILE* vFile );

	bool ReadFromFile( FILE* vFile );
	bool ReadFromFile( const char* Filename );

	void ClearAll( );

	//-----------for a* pather immplement -----------------------
	void Pos2GridIdx( const TWPOS& X,const TWPOS& Y,int& OutX,int& OutY )
	{
		mpMap->Pos2GridIdx( X,Y,OutX,OutY );
	}
	TWPOS GetGridW( )
	{
		return mpMap->GetGridW( );
	}
	TWPOS GetGridH( )
	{ 
		return mpMap->GetGridH( );
	}
	bool GridHasMask( int X,int Y,MASKBITFILED BIT )
	{
		return mpMap->GridHasMask( X,Y,BIT );
	}
	bool GridHasMask( int Idx,MASKBITFILED Stopmask );

	int BetweenCost( int vSrc,int vDes );

	int GetLinkThePointWaypointNum( int vIdx );

	int GetLinkThePointWaypoint( int vNum,int vIdx );

	inline bool InMap( int X,int Y )
	{
		return mpMap->InMap( X,Y );
	}

	static const char* GetFileExtendName()
	{
		return "aap";
	}

};

template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
bool CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::GridHasMask( int Idx,MASKBITFILED Stopmask )
{
	if( Idx < 0 )
		return true;
	if( mpCurDividingChunk != NULL )
	{
		unsigned int Sx = Idx % mChunkW;
		unsigned int Sy = Idx / mChunkW;
		if( Sx >= mChunkW || Sy >= mChunkH )
			return true;
		return mpMap->GridHasMask( Sx + mpCurDividingChunk->mSx,Sy + mpCurDividingChunk->mSy,Stopmask );
	}


	if( mbSearchArea == true )
	{
		if( Idx >= (int)mAreas.size() )
			return true;
		if( !(Stopmask & mWalkStopMark) )
			abort();
		else
			return false;
	}
	else
	{
		unsigned int X= Idx%_NumX;
		unsigned int Y= Idx/_NumX;
		if( mbInMarkedPathFindPath )
		{
			if( mGrids[Y][X] == NULL_AREA_INDEX )
				return true;
			if( mAreas[mGrids[Y][X]]->mMarked == true )
				return false;
			else
				return true;
		}
		else
			return mpMap->GridHasMask( X,Y,Stopmask );
	}
}




template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
int CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::BetweenCost( int vSrc,int vDes )
{
	if( mpCurDividingChunk != NULL )
	{
		unsigned int Srcx = vSrc % mChunkW + mpCurDividingChunk->mSx;
		unsigned int Srcy = vSrc / mChunkW + mpCurDividingChunk->mSy;

		unsigned int Desx = vDes % mChunkW + mpCurDividingChunk->mSx;
		unsigned int Desy = vDes / mChunkW + mpCurDividingChunk->mSy;

		return mpMap->BetweenCost( Srcx + Srcy*_NumX,Desx + Desy*_NumX );
	}
	if( mbSearchArea == true )
	{
		int dy	= mAreas[vSrc]->mSyChunk - mAreas[vDes]->mSyChunk;
		int dx	= mAreas[vSrc]->mSxChunk - mAreas[vDes]->mSxChunk;

		if( dy<0 )dy=-dy;
		if( dx<0 )dx=-dx;
		if( (dy==1) && (1==dx) )
			return 7;
		return ( ( dy + dx )*5 );
	}
	return mpMap->BetweenCost( vSrc ,vDes );
}



template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
int CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::GetLinkThePointWaypointNum( int vIdx )
{
	if( mpCurDividingChunk != NULL )
	{
		unsigned int Srcx = vIdx % mChunkW + mpCurDividingChunk->mSx;
		unsigned int Srcy = vIdx / mChunkW + mpCurDividingChunk->mSy;
		return mpMap->GetLinkThePointWaypointNum( Srcx + Srcy*_NumX );
	}
	if( mbSearchArea == true  )
		return mAreas[vIdx]->mLinkNum;
	return mpMap->GetLinkThePointWaypointNum( vIdx );
}


template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
int CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::GetLinkThePointWaypoint( int vNum,int vIdx )
{
	if( mpCurDividingChunk != NULL )
	{
		unsigned int Srcx = vIdx % mChunkW + mpCurDividingChunk->mSx;
		unsigned int Srcy = vIdx / mChunkW + mpCurDividingChunk->mSy;

		int tRet = mpMap->GetLinkThePointWaypoint( vNum,Srcx + Srcy*_NumX );

		int rSx = tRet % _NumX - mpCurDividingChunk->mSx;
		int rSy = tRet / _NumX - mpCurDividingChunk->mSy;
		if( rSx >= (int)mChunkW || rSy >= (int)mChunkH  || rSx < 0 || rSy<0 )
			return -1;
		return ( rSx + rSy*mChunkW );

	}

	if( mbSearchArea == true  )
		return mAreas[ mAreas[vIdx]->mLinksIdx[vNum] ]->mIdx;
	return mpMap->GetLinkThePointWaypoint( vNum,vIdx );
}


template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
void CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::GetAnPath( std::deque< int >& vOutPath,int vBegin,int vEnd,MASKBITFILED Stopmask )
{
	mbSearchArea			= false;
	mbInMarkedPathFindPath	= false;
	if( mbUsedFastA == false )
		return mAPather.GetAnPath( vOutPath,vBegin,vEnd,Stopmask );
	int B =  mGrids[ vBegin / _NumX ][ vBegin % _NumX ];
	int E =  mGrids[ vEnd / _NumX ][ vEnd % _NumX ];

	if( B == NULL_AREA_INDEX || E == NULL_AREA_INDEX )
		return;
	if( Stopmask == mWalkStopMark && B != E )
		mbSearchArea	= true;
	if( mbSearchArea )
	{

		std::deque< int > AreaPath;
		mAPather.GetAnPath( AreaPath,B,E,Stopmask,false );
		if( AreaPath.size() == 0 )
			return;

		mbInMarkedPathFindPath	= true;
		for(std::deque< int >::iterator it = AreaPath.begin(); it != AreaPath.end(); ++it)
		{
			mAreas[*it]->mMarked	= true;
		}
		mbSearchArea	= false;
		mAPather.GetAnPath( vOutPath,vBegin,vEnd,Stopmask );
		for(std::deque< int >::iterator it = AreaPath.begin(); it != AreaPath.end(); ++it)
		{
			mAreas[*it]->mMarked	= false;

			//mpMap->PaintClosedArea( *it );
		}
	}
	else
	{
		mbInMarkedPathFindPath	= false;
		mAPather.GetAnPath( vOutPath,vBegin,vEnd,Stopmask );
	}
}


template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
void CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::DivideArea( )
{
	if( (_NumY % _Row) != 0 || (_NumX % _Col) != 0 )
		return ;
	MapChunk* tpChunks[ _Row+1 ][ _Col+1 ];
	memset( tpChunks,0,sizeof( tpChunks ) );
	unsigned int tChunkH	= _NumY / _Row;
	unsigned int tChunkW	= _NumX / _Col;
	mChunkH	= tChunkH;
	mChunkW	= tChunkW;

	unsigned int tX,tY;
	int Count	= 0;
	for( unsigned int R = 0; R < _Row; R++ )
	{
		for( unsigned int C=0; C < _Col; C++ )
		{
			if( mpDivedeCompletePercentage )
				(*mpDivedeCompletePercentage)( Count*100.f / float(_Row*_Col) );
			++Count;
			tpChunks[R][C]	= new MapChunk( C*tChunkW,R*tChunkH );
			for( unsigned int Y=0; Y<tChunkH; Y++ )
			{
				for( unsigned int X=0; X<tChunkW; X++ )
				{
					tX = X+C*tChunkW;
					tY = Y+R*tChunkH;
					if( true == mpMap->GridHasMask( tX,tY,mWalkStopMark ) )
					{
						mGrids[tY][tX]	= NULL_AREA_INDEX;
						continue;
					}
					int tAreaIdx =-1;
					for( MapChunk::TAreas::iterator it=tpChunks[R][C]->mAreas.begin() ; it!=tpChunks[R][C]->mAreas.end(); ++it )
					{
						mpCurDividingChunk	= tpChunks[R][C];
						if( mAPather.CanGetto( X+Y*tChunkW,it->second,mWalkStopMark ) == true )
						{
							tAreaIdx	= it->first;
							break;
						}
					}
					if( tAreaIdx == -1 )
					{
						tAreaIdx	= (int)mAreas.size();

						if( tAreaIdx > NULL_AREA_INDEX -10 )
							abort();
						mAreas.push_back( new CloseArea( C,R,tAreaIdx ) );

						if( tAreaIdx >= (std::numeric_limits< unsigned short >::max)( ) )
							::abort();
						tpChunks[R][C]->mAreas.push_back( std::pair<int,int>( tAreaIdx,X+Y*tChunkW ) );
					}
					mGrids[tY][tX]	= (unsigned short)tAreaIdx;
				}
			}
		}
	}
	mpCurDividingChunk	= NULL;
	MapChunk* Chunk;
	MapChunk* Right;
	MapChunk* Down;

	MapChunk* RU	= NULL;
	MapChunk* RD	= NULL;

	mbInMarkedPathFindPath	= true;
	for( unsigned int R = 0; R < _Row; R++ )
	{
		for( unsigned int C=0; C < _Col; C++ )
		{
			Chunk	= tpChunks[R][C];
			Right	= tpChunks[R][C+1];
			Down	= tpChunks[R+1][C];

			if( R == 0 )
				RU = NULL;
			else
				RU = tpChunks[R-1][C+1];
			RD	= tpChunks[R+1][C+1];



			if( Right )
			{
				for( MapChunk::TAreas::iterator itChunk = Chunk->mAreas.begin(); itChunk!=Chunk->mAreas.end(); ++itChunk )
				{
					mAreas[itChunk->first]->mMarked	= true;
					int First = ( itChunk->second / tChunkW + Chunk->mSy ) * _NumX + itChunk->second % tChunkW + Chunk->mSx;
					for( MapChunk::TAreas::iterator itR = Right->mAreas.begin(); itR != Right->mAreas.end(); ++itR )
					{
						mAreas[itR->first]->mMarked	= true;
						int Second = ( itR->second / tChunkW + Right->mSy ) * _NumX + itR->second % tChunkW + Right->mSx;
						if( mAPather.CanGetto( First,Second,mWalkStopMark ) == true )
						{
							mAreas[itChunk->first]->AddLink( mAreas[itR->first]->mIdx );
							mAreas[itR->first]->AddLink( mAreas[itChunk->first]->mIdx );
						}

						mAreas[itR->first]->mMarked	= false;
					}
					mAreas[itChunk->first]->mMarked	= false;
				}
			}
			if( Down )
			{
				for( MapChunk::TAreas::iterator itChunk = Chunk->mAreas.begin(); itChunk!=Chunk->mAreas.end(); ++itChunk )
				{
					mAreas[itChunk->first]->mMarked	= true;
					int First = ( itChunk->second / tChunkW + Chunk->mSy ) * _NumX + itChunk->second % tChunkW + Chunk->mSx;
					for( MapChunk::TAreas::iterator itD = Down->mAreas.begin(); itD != Down->mAreas.end(); ++itD )
					{
						mAreas[itD->first]->mMarked	= true;
						int Second = ( itD->second / tChunkW + Down->mSy ) * _NumX + itD->second % tChunkW + Down->mSx;
						if( mAPather.CanGetto( First,Second,mWalkStopMark ) == true )
						{
							mAreas[itChunk->first]->AddLink( mAreas[itD->first]->mIdx );
							mAreas[itD->first]->AddLink( mAreas[itChunk->first]->mIdx );
						}

						mAreas[itD->first]->mMarked	= false;
					}
					mAreas[itChunk->first]->mMarked	= false;
				}
			}

			if( RU )
			{
				for( MapChunk::TAreas::iterator itChunk = Chunk->mAreas.begin(); itChunk!=Chunk->mAreas.end(); ++itChunk )
				{
					mAreas[itChunk->first]->mMarked	= true;
					int First = ( itChunk->second / tChunkW + Chunk->mSy ) * _NumX + itChunk->second % tChunkW + Chunk->mSx;
					for( MapChunk::TAreas::iterator itR = RU->mAreas.begin(); itR != RU->mAreas.end(); ++itR )
					{
						mAreas[itR->first]->mMarked	= true;
						int Second = ( itR->second / tChunkW + RU->mSy ) * _NumX + itR->second % tChunkW + RU->mSx;
						if( mAPather.CanGetto( First,Second,mWalkStopMark ) == true )
						{
							mAreas[itChunk->first]->AddLink( mAreas[itR->first]->mIdx );
							mAreas[itR->first]->AddLink( mAreas[itChunk->first]->mIdx );
						}

						mAreas[itR->first]->mMarked	= false;
					}
					mAreas[itChunk->first]->mMarked	= false;
				}
			}

			if( RD )
			{
				for( MapChunk::TAreas::iterator itChunk = Chunk->mAreas.begin(); itChunk!=Chunk->mAreas.end(); ++itChunk )
				{
					mAreas[itChunk->first]->mMarked	= true;
					int First = ( itChunk->second / tChunkW + Chunk->mSy ) * _NumX + itChunk->second % tChunkW + Chunk->mSx;
					for( MapChunk::TAreas::iterator itR = RD->mAreas.begin(); itR != RD->mAreas.end(); ++itR )
					{
						mAreas[itR->first]->mMarked	= true;
						int Second = ( itR->second / tChunkW + RD->mSy ) * _NumX + itR->second % tChunkW + RD->mSx;
						if( mAPather.CanGetto( First,Second,mWalkStopMark ) == true )
						{
							mAreas[itChunk->first]->AddLink( mAreas[itR->first]->mIdx );
							mAreas[itR->first]->AddLink( mAreas[itChunk->first]->mIdx );
						}

						mAreas[itR->first]->mMarked	= false;
					}
					mAreas[itChunk->first]->mMarked	= false;
				}
			}
		}
	}

	for( unsigned int R = 0; R < _Row; R++ )
	{
		for( unsigned int C=0; C < _Col; C++ )
		{
			delete tpChunks[R][C];
		}
	}
}



template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
bool CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::ReadFromFile( FILE* vFile )
{
	unsigned char TYPE[4]={ 0 };
	size_t tReaded	= fread( TYPE,sizeof( TYPE ),1,vFile );
	if( tReaded != 1 )
		return false;
	if( TYPE[0] != 'A' ||
		TYPE[1] != '*' ||
		TYPE[2] != '+' ||
		TYPE[3] != '+')
		return false;
	fread( &mChunkW,sizeof( mChunkW ),1,vFile );
	fread( &mChunkH,sizeof( mChunkH ),1,vFile );

	fread( mGrids,sizeof(unsigned short),_NumY*_NumX,vFile );

	size_t tSize;
	fread( &tSize,sizeof( tSize ),1,vFile );
	for( size_t i=0; i<tSize; i++ )
	{
		CloseArea* tpArea	= new CloseArea;
		fread( tpArea,1,sizeof( CloseArea ),vFile );
		if( tpArea->mLinkNum > 0 )
		{
			tpArea->mLinksIdx	= (unsigned short*)::malloc( sizeof(unsigned short)*tpArea->mLinkNum );
			fread( tpArea->mLinksIdx,tpArea->mLinkNum,sizeof( unsigned short ),vFile );
		}
		mAreas.push_back( tpArea );
	}
	return true;
}

template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
bool CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::SaveToFile( FILE* vFile )
{
	unsigned char TYPE[4]={ 'A','*','+','+' };
	if( 1 != fwrite( TYPE,sizeof( TYPE ),1,vFile ) )
		return false;
	fwrite( &mChunkW,sizeof( mChunkW ),1,vFile );
	fwrite( &mChunkH,sizeof( mChunkH ),1,vFile );

	fwrite( mGrids,sizeof(unsigned short),_NumY*_NumX,vFile );

	size_t tSize = mAreas.size();
	fwrite( &tSize,sizeof( tSize ),1,vFile );
	for( size_t i=0; i<mAreas.size(); i++ )
	{
		fwrite( mAreas[i],1,sizeof( CloseArea ),vFile );
		if( mAreas[i]->mLinkNum > 0 )
			fwrite( mAreas[i]->mLinksIdx,mAreas[i]->mLinkNum,sizeof( unsigned short ),vFile );
	}
	return true;
}
template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
bool CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::SaveToFile( const char* Filename )
{
	FILE* hFile	= fopen( Filename,"wb" );
	if( hFile == NULL )
		return false;
	bool Ret = SaveToFile( hFile );
	fclose( hFile );
	return Ret;
}
template< class _Map,unsigned int _NumX,unsigned int _NumY,unsigned int _Row,unsigned int _Col > inline
void CAreaPather< _Map,_NumX,_NumY,_Row,_Col >::ClearAll( )
{
	for( size_t i=0; i<mAreas.size(); i++ )
	{
		delete mAreas[i];
	}
	mAreas.clear();
	memset( mGrids,NULL_AREA_INDEX, sizeof(mGrids) );
	memset(mGrids,NULL_AREA_INDEX,sizeof(mGrids) );

	mbUsedFastA	= true;
	mbInMarkedPathFindPath	= false;
	mpDivedeCompletePercentage	= NULL;
	mbSearchArea	= false;
	mpCurDividingChunk	= NULL;
}

#endif
