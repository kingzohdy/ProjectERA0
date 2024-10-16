
#ifndef __GamePathFinding_H__
#define __GamePathFinding_H__

#include <map>
#include <deque>

//#pragma warning (disable:4355)

#define CVector std::vector
#pragma   once
#define NullIndex -1
template< size_t vCount >
class CFixedMemCycIdxAssignList
{
	struct  element
	{
		int		index;  //buf index
		int		pre;
		int		next;
		bool	bused;
		void free( ){ bused	= false;}
		void used( ){ bused	= true; }
		void _init( )
		{
			index	= NullIndex;
			pre		= NullIndex;
			next	= NullIndex;
			bused	= false;
		}
		~element( ){ free( ); }
	};
	element		assignList[ vCount ];
	int	mFirst;
	int	mBack;

	element&	back( ){ return assignList[ mBack ]; }
	void		moveBackToFront( );
	void		erase( element& vSrc );
	void		insert_front( element& vSrc  );
	void		insert_back( element& vSrc  );
	void		toBack( element& vSrc );
public:
	CFixedMemCycIdxAssignList( ){ _init( ); }
	void		_init( );
	int			useAnElemIndex( );
	void		freeAnElemIndex( int vIndex );
};
//----------------------------CFixedMemAssignList::--------------------------------------
template< size_t vCount >
int CFixedMemCycIdxAssignList< vCount >::useAnElemIndex( )
{
	element& used	= back( );
	if( used.bused == true )return NullIndex;
	int ret	= used.index;
	moveBackToFront( );
	used.used();
	return ret;
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::freeAnElemIndex( int vIndex )
{
	element& el	= assignList[ vIndex ];
	el.free( );
	toBack( el );
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::_init( )
{
	for( int i=0;i< vCount ;i++ )
	{
		assignList[ i ]._init( );
		assignList[ i ].index	= i;
		assignList[ i ].pre		= ( ((i-1)>=0)? (i-1) : NullIndex );
		assignList[ i ].next	= ( ((i+1)<vCount)? (i+1) : NullIndex );
	}
	mFirst	= 0;
	mBack	= vCount-1;
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::moveBackToFront( )
{
	element& tBack	= back( );
	erase( tBack );
	insert_front( tBack );
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::erase( element& vSrc )
{
	if( vSrc.next != NullIndex )
	{
		assignList[vSrc.next].pre	= vSrc.pre;
	}
	if( vSrc.pre != NullIndex )
	{
		assignList[ vSrc.pre ].next	= vSrc.next;
	}
	if( vSrc.index	== mFirst )mFirst= assignList[vSrc.index].next;
	if( vSrc.index	== mBack )mBack	= assignList[vSrc.index].pre;
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::insert_front( element& vSrc  )
{
	if( mFirst == NullIndex )
	{ 
		mBack	= mFirst	= vSrc.index;
		vSrc.pre	= vSrc.next	= NullIndex;
		return;
	}
	assignList[mFirst].pre	= vSrc.index;
	assignList[vSrc.index].next	= mFirst;
	assignList[vSrc.index].pre	= NullIndex;
	mFirst	= vSrc.index;
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::insert_back( element& vSrc  )
{
	if( mFirst == NullIndex )
	{ 
		mBack	= mFirst	= vSrc.index;
		vSrc.pre	= vSrc.next	= NullIndex;
		return;
	}
	assignList[mBack].next	= vSrc.index;
	assignList[vSrc.index].pre	= mBack;
	assignList[vSrc.index].next	= NullIndex;
	mBack	= vSrc.index;
}
//----------------------------------------
template< size_t vCount >
void CFixedMemCycIdxAssignList< vCount >::toBack( element& vSrc )
{
	if( vSrc.index == mBack )return;
	erase( vSrc );
	insert_back( vSrc );
}

template< class _Map,int _NumX,int _NumY >
class CAPatherMap
{
	enum{ NODE_MEM_CAPACITY	= _NumX * _NumY,NUM_X=_NumX,NUM_Y=_NumY };
	//enum{ MAP_MAX_COST	= 1024*1024 };
	typedef _Map CExternMap;
public:
	//-------------------------------------------------------------
	template< class _Ty >
	class MultimapAlloc : public std::allocator< _Ty >
	{
	public:
		typedef std::_Allocator_base<_Ty> _Mybase;
		typedef typename _Mybase::value_type value_type;

		typedef value_type _FARQ *pointer;
		typedef value_type _FARQ& reference;
		typedef const value_type _FARQ *const_pointer;
		typedef const value_type _FARQ& const_reference;

		template<class _Other>
		struct rebind
		{	// convert an allocator<_Ty> to an allocator <_Other>
			typedef MultimapAlloc<_Other> other;
		};
		struct Chunk
		{
			char           mObj[sizeof(value_type)];
			unsigned short mVectorIdx;
			unsigned short mAssignIdx;
			Chunk( ){ }
		};
		struct MemoryChunk
		{
			enum{ CAPACITY	= 1024 };
			CFixedMemCycIdxAssignList< CAPACITY > mAssignList;
			Chunk	mMemory[ CAPACITY ];
			MemoryChunk( ){ }
		};
		CVector< MemoryChunk* > mMemoryPool;

		void ClearAllMemory( )
		{
			for( unsigned int  i=0; i<mMemoryPool.size(); i++ )
				delete mMemoryPool[i];
			mMemoryPool.clear( );
		}
		void deallocate(pointer _Ptr, size_type)
		{	// deallocate object at _Ptr, ignore size
			//operator delete(_Ptr);
			Chunk* tpChunk	= (Chunk*)_Ptr;
			mMemoryPool[ tpChunk->mVectorIdx ]->mAssignList.freeAnElemIndex( tpChunk->mAssignIdx );
		}

		pointer allocate(size_type _Count)
		{
			assert( _Count == 1 );
			for( size_t i=0; i<mMemoryPool.size(); i++ )
			{
				int idx=mMemoryPool[i]->mAssignList.useAnElemIndex( );
				if( idx == NullIndex )continue;
				mMemoryPool[i]->mMemory[ idx ].mAssignIdx	= idx;
				mMemoryPool[i]->mMemory[ idx ].mVectorIdx	= i;
				return (pointer)&mMemoryPool[i]->mMemory[ idx ].mObj;
			}
			MemoryChunk* newChunk	= new MemoryChunk;
			mMemoryPool.push_back( newChunk );

			int idx	= newChunk->mAssignList.useAnElemIndex( );
			newChunk->mMemory[idx].mAssignIdx	= idx;
			newChunk->mMemory[idx].mVectorIdx	= (unsigned short)mMemoryPool.size()-1;
			return  (pointer)&newChunk->mMemory[idx].mObj;
		}

		pointer allocate(size_type _Count, const void _FARQ *)
		{	// allocate array of _Count elements, ignore hint
			return (allocate(_Count));
		}

		void construct(pointer _Ptr, const _Ty& _Val)
		{	// construct object at _Ptr with value _Val
			_Construct(_Ptr, _Val);
		}

		void destroy(pointer _Ptr)
		{	// destroy object at _Ptr
			std::_Destroy(_Ptr);
		}
		MultimapAlloc()
		{	// construct default allocator (do nothing)
		}

		MultimapAlloc(const MultimapAlloc<_Ty>&)
		{	// construct by copying (do nothing)
		}
		template<class _Other>
			MultimapAlloc(const MultimapAlloc<_Other>&)
		{	// construct from a related allocator (do nothing)
		}

		template<class _Other>
			MultimapAlloc<_Ty>& operator=(const MultimapAlloc<_Other>&)
		{	// assign from a related allocator (do nothing)
			return (*this);
		}
	};

	struct Node
	{
		int				mToOrigCost;
		int				mToDesCost;
		int				mIdx;
		Node*			mPid;
		unsigned short	mBucket;
		unsigned char	mbInClosedList;
		unsigned char	mbInOpenList;
		Node( ){ Init( );}
		bool Empty( ){ return ( (mbInClosedList == 0)&&(mbInOpenList == 0) );}
		int GetCost( )
		{
			//assert( mToOrigCost + mToDesCost < MAP_MAX_COST );
			//return (mToOrigCost + mToDesCost) * MAP_MAX_COST + mIdx;
			return (mToOrigCost + mToDesCost) ;
		}
		bool InClosedList( ){ return (mbInClosedList == 1);		}
		bool InOpenList( ){ return (mbInOpenList == 1);			}
		void SetInClosedList( ){ mbInClosedList = 1;			}
		void SetInOpenList( ){ mbInOpenList = 1;				}
		int	ToNodeCost( Node* vpNode,CExternMap* Map ){ return Map->BetweenCost( mIdx,vpNode->mIdx ); }
		inline void CalcCost( Node* vPid,Node* vDesNode ,CExternMap* Map )
		{ 
			mToOrigCost	= vPid->mToOrigCost + vPid->ToNodeCost( this ,Map );
			mToDesCost	= ToNodeCost( vDesNode,Map );
			mPid		= vPid;
		}
		void Init( )
		{
			mIdx	= -1;
			mPid	= 0;
			mBucket	= 0;
			mbInOpenList	= 0;
			mbInClosedList	= 0;
			mToOrigCost		= 0;
			mToDesCost		= 0;
		}
	};
	class CNodeMemChunks
	{
	public:
		enum{ CAPACITY = 1024 };
		unsigned char mNodes[CAPACITY*sizeof(Node)];
		int  mUsedNum;
		CNodeMemChunks( ):mUsedNum(0){ }
		Node* UseAnNode( )
		{
			if( mUsedNum == CAPACITY )return 0;
			Node* ret = (Node*)(mNodes + sizeof( Node )*mUsedNum);
			++mUsedNum; 
			return ret;
		}
		Node* operator[]( int vIdx )
		{
			Node* ret = (Node*)(mNodes + sizeof( Node )*vIdx);
			return ret;
		}
	};
	typedef MultimapAlloc< std::pair<const int, Node*> >  CMultimapAllocator;
	typedef std::multimap< int,Node*,std::less<int>,CMultimapAllocator  > COpenListType;
	//typedef std::multimap< int,Node* > COpenListType;
	Node*			mpMemory[NODE_MEM_CAPACITY];
	Node*			mBackupEndNode;
	COpenListType	msOpenList;
	int				mNodeCount;
	int				mMemorySize;
	CVector< CNodeMemChunks* > mNodeMemChunks;
	CExternMap*		mpMap;
	MASKBITFILED	mBackupStopMask;
	int				mTestedNodeNum;
	void ResetMap( CExternMap* Newmap )
	{
		mpMap	= Newmap;
	}
	inline Node* Idx2Node( int vIdx )
	{
		if( vIdx < 0 )return 0;
		if( mpMemory[vIdx] == 0 )
		{
			Node* tNewNode	= mNodeMemChunks.back( )->UseAnNode( );
			if( tNewNode == 0 )
			{
				mNodeMemChunks.push_back( new CNodeMemChunks );
				tNewNode	= mNodeMemChunks.back( )->UseAnNode( );
			}
			tNewNode->Init( );
			tNewNode->mIdx	= vIdx;
			mpMemory[vIdx]	= tNewNode;
			mTestedNodeNum++;
		}
		return mpMemory[vIdx];
	}
	inline void AddToOpenList( Node* vpPid,Node* vpNode )
	{
		if( vpNode == 0 )return;
		if( vpNode->InClosedList() )return;
		if( vpNode->InOpenList( ) )
		{
			int tToNewPidCost	= vpPid->ToNodeCost( vpNode,mpMap );
			if( vpNode->mToOrigCost < vpPid->mToOrigCost + tToNewPidCost )
				return;
			//COpenListType::iterator it=msOpenList.find( vpNode->GetCost( ) );
			//assert( it != msOpenList.end() );
			//	msOpenList.erase( it );
			vpNode->mToOrigCost	= vpPid->mToOrigCost + tToNewPidCost;
			//msOpenList.insert( COpenListType::value_type(vpNode->GetCost( ),vpNode) );
			vpNode->mPid		= vpPid;
		}
		else//not in open list
		{
			vpNode->CalcCost( vpPid,mBackupEndNode,mpMap );
			msOpenList.insert( COpenListType::value_type( vpNode->GetCost( ),vpNode ) );
			vpNode->SetInOpenList( );
		}
	}
	inline void AddToClosedList( Node* vpNode )
	{
		if( vpNode->InClosedList( ) )return;
		int tPathNum	= mpMap->GetLinkThePointWaypointNum( vpNode->mIdx );
		int tIdx;
		for( int i=0; i < tPathNum; i++ )
		{
			tIdx	= mpMap->GetLinkThePointWaypoint( i,vpNode->mIdx );

			assert( tIdx != vpNode->mIdx );

			if( mpMap->GridHasMask( tIdx,mBackupStopMask ) == false )
				AddToOpenList( vpNode,Idx2Node( tIdx )	);
		}

		vpNode->SetInClosedList( );
	}
	void PrepareSearchPath( )
	{
		mTestedNodeNum	= 0;
		msOpenList.clear( );
		msOpenList.get_allocator().ClearAllMemory( );
		mNodeMemChunks.push_back( new CNodeMemChunks );
	}
	void SearchPathFinish( )
	{
		msOpenList.clear( );
		mNodeCount=0;
		mMemorySize	= msOpenList.get_allocator().mMemoryPool.size()  * sizeof( CMultimapAllocator::MemoryChunk );
		mMemorySize+= mNodeMemChunks.size() * sizeof( CNodeMemChunks );
		msOpenList.get_allocator().ClearAllMemory( ); 
		CNodeMemChunks* tpNodeMemChunk;
		for( unsigned int i=0; i<mNodeMemChunks.size(); i++ )
		{
			tpNodeMemChunk	= mNodeMemChunks[i];
			for( int n=0; n<tpNodeMemChunk->mUsedNum; n++ )
			{
				mpMemory[ (*tpNodeMemChunk)[n]->mIdx ] =0;
				mNodeCount++;
			}	
			delete tpNodeMemChunk;
		}
		mNodeMemChunks.clear( );
	}
public:
	struct FuncStackDump
	{
		CAPatherMap* mpPather;
		FuncStackDump( CAPatherMap* vpPather ):mpPather( vpPather )
		{
			mpPather->PrepareSearchPath( );
		}
		~FuncStackDump( )
		{
			mpPather->SearchPathFinish( );
		}
	};
	CAPatherMap( CExternMap* vMap )
	{
		memset( mpMemory,0,sizeof(mpMemory) );
		mpMap	= vMap;
	}

	bool CanGetto( int vBegin,int vEnd,MASKBITFILED Stopmask )
	{
		mBackupStopMask	= Stopmask;
		if( vBegin == vEnd )
			return true;
		if( vBegin < 0 || vEnd < 0 )
			false;

		FuncStackDump dump( this );

		mBackupEndNode	= Idx2Node( vEnd );
		if( mBackupEndNode == 0 )
			return false;
		Node* root	= Idx2Node(vBegin);
		root->mToDesCost	= root->ToNodeCost( mBackupEndNode,mpMap );
		root->SetInOpenList( );
		msOpenList.insert( COpenListType::value_type( root->GetCost(),root ) );

		COpenListType::iterator it	= msOpenList.begin( );
		Node* tNode;

		while( it != msOpenList.end( ) )
		{
			tNode	= it->second;
			msOpenList.erase( it );
			if( tNode->mIdx == vEnd )
			{//找到了
				return true;
			}
			AddToClosedList( tNode );
			it	= msOpenList.begin( );
		}
		return false;
	}
	inline void GetAnPath( std::deque< int >& vOutPath,int vBegin,int vEnd,MASKBITFILED Stopmask,bool EraseMidPoint	= true )
	{
		mBackupStopMask	= Stopmask;
		if( vBegin == vEnd )return;
		if( vBegin < 0 || vEnd < 0 )return;

		FuncStackDump dump( this );

		mBackupEndNode	= Idx2Node( vEnd );
		if( mBackupEndNode == 0 )return;
		Node* root	= Idx2Node(vBegin);
		root->mToDesCost	= root->ToNodeCost( mBackupEndNode,mpMap );
		root->SetInOpenList( );
		msOpenList.insert( COpenListType::value_type( root->GetCost(),root ) );

		COpenListType::iterator it	= msOpenList.begin( );
		Node* tNode;
		Node* tpidNode;
		int	  dx0,dy0,dx1,dy1;

		TWPOS Sx	;
		TWPOS Sy	;
		TWPOS Ex	;
		TWPOS Ey	;
		while( it != msOpenList.end( ) )
		{
			tNode	= it->second;
			msOpenList.erase( it );
			if( tNode->mIdx == vEnd )
			{//找到了
				vOutPath.push_front( tNode->mIdx );
				tNode	= tNode->mPid;
				while( tNode )
				{
					tpidNode= tNode->mPid;
					if( tpidNode && EraseMidPoint == true )
					{
						dx1	= (tpidNode->mIdx % NUM_X);
						dy1	= (tpidNode->mIdx / NUM_X);

						dx0	= (vOutPath.front( ) % NUM_X);
						dy0	= (vOutPath.front( ) / NUM_X);

						Sx	= dx0 * mpMap->GetGridW( ) + mpMap->GetGridW( ) / 2;
						Sy	= dy0 * mpMap->GetGridH( ) + mpMap->GetGridH( ) / 2 ;
						Ex	= dx1 * mpMap->GetGridW( ) + mpMap->GetGridW( ) / 2;
						Ey	= dy1 * mpMap->GetGridH( ) + mpMap->GetGridH( ) / 2;
						if( LineThroughInMaskGridMap( Sx,Sy,Ex,Ey ,*mpMap,Stopmask ) )
						{
							tNode	= tpidNode;
							continue;
						}
					}
					vOutPath.push_front( tNode->mIdx );
					tNode	= tpidNode;
				}
				break;
			}
			AddToClosedList( tNode );
			it	= msOpenList.begin( );

			/*if( mTestedNodeNum > 1024 * 10 )
			{
			vOutPath.resize(0,int());
			return;
			};*/
		}
		if( vOutPath.size()== 1 )
			vOutPath.resize(0,int());
	}

};

#endif