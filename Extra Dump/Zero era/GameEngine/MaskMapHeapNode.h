
#ifndef __MASKMAPHEAPNODE_H__
#define __MASKMAPHEAPNODE_H__

#include <vector>
#include <list>
#include <algorithm>

inline unsigned int MAKEINDEX( int x, int y )
{
	return (y<<16) | (x);
}

inline int INDEX_TO_X( unsigned int index )
{
	return index&0xffff;
}

inline int INDEX_TO_Y( unsigned int index )
{
	return index>>16;
}

struct PathHeapNode
{
	unsigned int from;
	unsigned int index;
	int hval;  //heuristics_cost
	int aval; //�ӿ�ʼ�㵽�ﵱǰ���ʵ��cost
};

inline bool operator <( const PathHeapNode &node1, const PathHeapNode &node2 )
{ 
	return (node1.hval+node1.aval < node2.hval+node2.aval);
}
inline bool operator >( const PathHeapNode &node1, const PathHeapNode &node2 )
{ 
	return (node1.hval+node1.aval > node2.hval+node2.aval); 
}
inline bool operator ==( const PathHeapNode &node1, const PathHeapNode &node2 )
{ 
	return (node1.hval+node1.aval == node2.hval+node2.aval);
}

class PathFindHeap
{
public:
	void Init()
	{
		m_AllNodes.resize(0);
	}
	void PushNode( const PathHeapNode &node )
	{
		m_AllNodes.push_back( node );
		std::push_heap( m_AllNodes.begin(), m_AllNodes.end(), comp );
	}
	void PopNode( PathHeapNode &node )
	{
		node = m_AllNodes.front();
		std::pop_heap( m_AllNodes.begin(), m_AllNodes.end(), comp );
		m_AllNodes.pop_back();
	}
	bool HasNode()
	{
		return m_AllNodes.size()>0;
	}
	void AdjustNode( std::vector<PathHeapNode>::iterator iter )
	{
		std::push_heap( m_AllNodes.begin(), iter+1, comp );
	}

public:
	std::vector<PathHeapNode> m_AllNodes;
	std::greater<PathHeapNode> comp;
};

#endif