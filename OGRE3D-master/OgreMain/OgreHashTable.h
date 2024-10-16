
#ifndef __HashTable_H__
#define __HashTable_H__

#include "OgrePrerequisites.h"
#include "OgreFixedString.h"

namespace Ogre
{
	class UIntHashCoder
	{
	public:
		uint operator()( uint i ) const{ return i; }
	};

	class FixedStringHashCoder
	{
	public:
		uint operator()( const FixedString &str ) const
		{
			return (uint)(str.getHandle());
		}
	};

	template<typename TKey, class TValue, class THashCoder>
	class HashTable
	{
	public:
		struct Element
		{
			TKey key;
			uint hash;
			TValue value;
			Element *pnext;
		};

	public:
		HashTable( size_t maxtable ) : m_MaxSlot(maxtable)
		{
			assert( m_MaxSlot > 0 );
			m_Table = new Element *[m_MaxSlot];
			memset( m_Table, 0, m_MaxSlot*sizeof(Element *) );
		}
		~HashTable()
		{
			for( size_t i=0; i<m_MaxSlot; i++ )
			{
				Element *pcur = m_Table[i];
				while( pcur )
				{
					Element *ptmp = pcur;
					pcur = pcur->pnext;
					delete ptmp;
				}
			}
			delete[] m_Table;
		}

		Element *iterate(Element *pelement)
		{
			size_t islot;
			Element *pnext;
			if(pelement)
			{
				islot = pelement->hash%m_MaxSlot;
				pnext = pelement->pnext;
			}
			else
			{
				islot = 0;
				pnext = m_Table[0];
			}

			while(pnext == NULL)
			{
				islot++;
				if(islot == m_MaxSlot) return NULL;
				pnext = m_Table[islot];
			}

			return pnext;
		}

		void erase(Element *pelement)
		{
			assert(pelement != NULL);
			size_t islot = pelement->hash%m_MaxSlot;
			Element *pcur = m_Table[islot];
			if(pcur == pelement)
			{
				m_Table[islot] = pcur->pnext;
			}
			else
			{
				while(pcur->pnext!=pelement)
				{
					pcur = pcur->pnext;
					assert(pcur != NULL);
				}

				pcur->pnext = pelement->pnext;
			}

			delete pelement;
		}

		Element *find(const TKey &key) const
		{
			uint index = m_KeyCoderFunc(key)%m_MaxSlot;
			Element *ptmp = m_Table[index];
			if( ptmp == NULL ) return NULL;
			else
			{
				while( ptmp->key != key )
				{
					if( ptmp->pnext == NULL ) return NULL;
					else ptmp = ptmp->pnext;
				}
			}

			return ptmp;
		}

		Element *insert(const TKey &key)
		{
			uint hash = m_KeyCoderFunc(key);
			uint index = hash % m_MaxSlot;
			Element *ptmp = m_Table[index];
			if( ptmp == NULL )
			{
				m_Table[index] = ptmp = allocElement(key);
			}
			else
			{
				while( ptmp->key != key )
				{
					if( ptmp->pnext == NULL )
					{
						ptmp->pnext = allocElement(key);
						ptmp = ptmp->pnext;
						break;
					}
					else ptmp = ptmp->pnext;
				}
			}

			ptmp->hash = hash;
			return ptmp;
		}

		Element *insert(const TKey &key, const TValue &value)
		{
			Element *pelement = insert(key);
			pelement->value = value;
			return pelement;
		}

		TValue &operator[](const TKey &key)
		{
			Element *pelement = insert(key);
			return pelement->value;
		}

	private:
		THashCoder m_KeyCoderFunc;
		Element ** m_Table;
		size_t     m_MaxSlot;

		Element *allocElement( TKey key )
		{
			Element *p = new Element;
			p->key = key;
			p->pnext = NULL;
			return p;
		}
	};
}

#endif