#ifndef __UI_RICHTEXTCOMMONTYPE_H__
#define __UI_RICHTEXTCOMMONTYPE_H__
//点类型，像素单位
class RFPoint
{
public:
	float m_x;		//X坐标
	float m_y;		//Y坐标

	RFPoint() : m_x( 0 ), m_y( 0 )
	{}

	RFPoint( float x,float y )
	{
		m_x = x;
		m_y = y;
	}

	//	RFPoint(RFPoint& p){m_x = p.m_x,m_y = p.m_y;}
	RFPoint( const RFPoint& p )
	{
		m_x = p.m_x;
		m_y = p.m_y;
	}

	RFPoint( RFPoint* p )
	{
		m_x = p->m_x;
		m_y = p->m_y;
	}

	void SetPos( float x,float y )
	{
		m_x = x;
		m_y = y;
	}

	bool operator==( const RFPoint& pt )
	{
		return ( ( int )m_x == ( int )pt.m_x && ( int )m_y == ( int )pt.m_y );
	}

	RFPoint operator+( const RFPoint& pt )
	{
		RFPoint p;
		p.m_x = m_x + pt.m_x;
		p.m_y = m_y + pt.m_y;
		return p;
	}

	RFPoint operator+=( const RFPoint& pt )
	{
		m_x += pt.m_x;
		m_y += pt.m_y;
		return *this;
	}

	RFPoint operator-( const RFPoint& pt )
	{
		RFPoint p;
		p.m_x = m_x - pt.m_x;
		p.m_y = m_y - pt.m_y;
		return p;
	}

	RFPoint operator-=( const RFPoint& pt )
	{
		m_x -= pt.m_x;
		m_y -= pt.m_y;
		return *this;
	}

	RFPoint operator-()
	{
		RFPoint p;
		p.m_x = -m_x;
		p.m_y = -m_y;
		return p;
	}

	RFPoint operator*( float f )
	{
		RFPoint p;
		p.m_x = m_x * f;
		p.m_y = m_y * f;
		return p;
	}

	RFPoint operator*=( float f )
	{
		m_x *= f;
		m_y *= f;
		return *this;
	}
};

class RFSize
{
public:
	float m_x;		//X坐标
	float m_y;		//Y坐标

	RFSize() : m_x( 0 ), m_y( 0 )
	{}

	RFSize( float x,float y )
	{
		m_x = x;
		m_y = y;
	}
	
	void SetSize( float x,float y )
	{
		m_x = x;
		m_y = y;
	}
};
#endif