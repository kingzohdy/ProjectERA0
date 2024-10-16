
#ifndef __List_H__
#define __List_H__

#include "OgrePrerequisites.h"

namespace Ogre
{

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------class DoubleChainList---------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	class ChainListNode
	{
	public:
		ChainListNode *m_pNextNode;
		ChainListNode *m_pPrevNode;
	};

	template<class T>
	class ChainList
	{
	public:
		ChainList(){ Clear(); }

		//得到当前加入的node数目
		size_t GetNumNodes(){ return m_nNodes; }
		bool IsEmpty(){ return m_nNodes==0; }
		void Clear()
		{ 
			m_EndNode.m_pNextNode = m_EndNode.m_pPrevNode = &m_EndNode;
			m_nNodes = 0;
		}

		void PushFront( ChainListNode *pnode ){ InsertAfter(pnode, &m_EndNode); }
		void PushBack( ChainListNode *pnode ){ InsertBefore(pnode, &m_EndNode); }
		void PopFront()
		{
			assert( !IsEmpty() );
			Remove( m_EndNode.m_pNextNode );
		}
		void PopBack()
		{
			assert( !IsEmpty() );
			Remove( m_EndNode.m_pPrevNode );
		}

		void InsertBefore( ChainListNode *pnode, ChainListNode *pref )
		{
			assert( pnode && pref );

			//这四行的顺序非常重要
			pnode->m_pNextNode = pref;
			pnode->m_pPrevNode = pref->m_pPrevNode;

			pref->m_pPrevNode->m_pNextNode = pnode;
			pref->m_pPrevNode = pnode;

			m_nNodes++;
		}

		void InsertAfter( ChainListNode *pnode, ChainListNode *pref )
		{
			assert( pnode && pref );

			pnode->m_pPrevNode = pref;
			pnode->m_pNextNode = pref->m_pNextNode;

			pref->m_pNextNode->m_pPrevNode = pnode;
			pref->m_pNextNode = pnode;

			m_nNodes++;
		}

		T *Remove( ChainListNode *pnode )
		{
			assert( pnode && pnode!=&m_EndNode );

			ChainListNode *pnext = pnode->m_pNextNode;
			pnode->m_pNextNode->m_pPrevNode = pnode->m_pPrevNode;
			pnode->m_pPrevNode->m_pNextNode = pnode->m_pNextNode;

			pnode->m_pNextNode = pnode->m_pPrevNode = NULL;

			m_nNodes--;
			return pnext==&m_EndNode ? NULL : static_cast<T *>(pnext);
		}

		bool IsNodeInList( ChainListNode *pnode )
		{
			ChainListNode *pcur = m_EndNode.m_pNextNode;
			while( pcur != &m_EndNode )
			{
				if( pcur == pnode ) return true;
				pcur = pcur->m_pNextNode;
			}
			return false;
		}

		void DeleteAllNodes()
		{
			ChainListNode *pcur = m_EndNode.m_pNextNode;
			while( pcur != &m_EndNode )
			{
				ChainListNode *pnext = pcur->m_pNextNode;
				MAY_DELETE(static_cast<T *>(pcur));
				pcur = pnext;
			}
			Clear();
		}

		void ReleaseAllNodes()
		{
			ChainListNode *pcur = m_EndNode.m_pNextNode;
			while( pcur != &m_EndNode )
			{
				ChainListNode *pnext = pcur->m_pNextNode;
				MAY_RELEASE(static_cast<T *>(pcur));
				pcur = pnext;
			}
			Clear();
		}

		T *BeginIterate()
		{ 
			return m_EndNode.m_pNextNode==&m_EndNode ? NULL : static_cast<T *>(m_EndNode.m_pNextNode);
		}
		T *Iterate( ChainListNode *pnode )
		{ 
			return pnode->m_pNextNode==&m_EndNode ? NULL : static_cast<T *>(pnode->m_pNextNode);
		}

		T *Front()
		{
			return m_EndNode.m_pNextNode==&m_EndNode ? NULL : static_cast<T *>(m_EndNode.m_pNextNode);
		}

		T *Back()
		{
			return m_EndNode.m_pPrevNode==&m_EndNode ? NULL : static_cast<T *>(m_EndNode.m_pPrevNode);
		}

	private:
		ChainListNode m_EndNode;
		size_t m_nNodes;
	};


	//--------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------class SingleChainList----------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	class SingleChainNode
	{
	public:
		SingleChainNode *m_pNextNode;
	};

	template<class T>
	class SingleChainList
	{
	public:
		SingleChainList(){ Clear(); }

		bool IsEmpty(){ return m_pHead==NULL; }
		void Clear(){ m_pHead = NULL; }

		void AddNode( SingleChainNode *pnode )
		{
			pnode->m_pNextNode = m_pHead;
			m_pHead = pnode;
		}

		void RemoveNode( SingleChainNode *pnode )
		{
			if( m_pHead == pnode )
			{
				m_pHead = pnode->m_pNextNode;
				return;
			}

			SingleChainNode *pcur = m_pHead;
			while( pcur )
			{
				if( pcur->m_pNextNode == pnode )
				{
					pcur->m_pNextNode = pnode->m_pNextNode;
					return;
				}
				pcur = pcur->m_pNextNode;
			}
		}

		//得到当前加入的node数目
		size_t GetNumNodes()
		{
			size_t nnode = 0;
			SingleChainNode *pcur = m_pHead;
			while( pcur )
			{
				nnode++;
				pcur = pcur->m_pNextNode;
			}
			return nnode;
		}

		//把当前node的顺序逆转
		void ReverseList()
		{
			SingleChainNode *pcur = m_pHead;
			SingleChainNode *pprev = NULL;
			while( pcur )
			{
				SingleChainNode *pnext = pcur->m_pNextNode;
				pcur->m_pNextNode = pprev;

				pprev = pcur;
				pcur = pnext;
			}

			m_pHead = pprev;
		}

		bool IsNodeInList( SingleChainNode *pnode )
		{
			SingleChainNode *pcur = m_pHead;
			while( pcur )
			{
				if( pcur == pnode ) return true;
				pcur = pcur->m_pNextNode;
			}
			return false;
		}

		void DeleteAllNodes()
		{
			SingleChainNode *pcur = m_pHead;
			while( pcur )
			{
				SingleChainNode *pnext = pcur->m_pNextNode;
				MAY_DELETE(static_cast<T *>(pcur));

				pcur = pnext;
			}
			Clear();
		}

		void ReleaseAllNodes()
		{
			SingleChainNode *pcur = m_pHead;
			while( pcur )
			{
				SingleChainNode *pnext = pcur->m_pNextNode;
				static_cast<T *>(pcur)->Release();

				pcur = pnext;
			}
			Clear();
		}

		T *BeginIterate()
		{ 
			return static_cast<T *>(m_pHead);
		}
		T *Iterate( SingleChainNode *pnode )
		{
			return static_cast<T *>(pnode->m_pNextNode);
		}


	private:
		SingleChainNode *m_pHead;
	};

}

#endif