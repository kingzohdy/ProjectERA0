#include ".\msign.h"


#include "mterrain.h"

using namespace TE;

unsigned int g_nColorSet[8] = 
{
	0xffff0000,
	0xff00ff00,
	0xff0000ff,
	0xffff7777,
	0xff77ff77,
	0xff7777ff,
	0xff000000,
	0xffffffff,
};

void MRound::SetDivideNum ( int num )
{
	if( num > ROUND_MAX_NUM )
		return;

	if( num == m_nCurPosNum )
		return;

	m_nCurPosNum = num;

	float ra = 2*MAY_PI / num;

	float ra_add = 0;

	MVec2  pos;
	for( int i = 0 ; i < num ; i ++)
	{
		pos.x = sin( i*ra );
		pos.y = cos( i*ra );
		m_RoundPosSet[i] = pos ;
	}

}


MSign::MSign(void)
{
	m_nColor = 0xff0000ff;
	m_pPath = new MPath;	
	m_pPath->GetPathData ()->data.resize (5);
}

MSign::~MSign(void)
{
	OGRE_DELETE( m_pPath );
}
		
void MSign::SetPos(MVec3 pos)
{
	 static  MVec3 _pos(0,0,0);

	 static int _color;
	 
	 if( _pos == pos)
		 return;


	 _color++;
	 if(_color>=255)
		 _color = 0;

     if( m_pPath )
	 {    
		 m_pPath->GetPathData ()->color = m_nColor;

		 _pos = pos;
		 _pos.y += 0.2f;
		 _pos.x += 0.2f;
		 _pos.z += 0.2f;
         m_pPath->GetPathData ()->data [0] = ( _pos ); 
		 _pos = pos;
		 _pos.y += 0.2f;
		 _pos.x += 0.2f;
		 _pos.z -= 0.2f;
         m_pPath->GetPathData ()->data [1] = ( _pos ); 
		 _pos = pos;
		 _pos.y += 0.2f;
		 _pos.x -= 0.2f;
		 _pos.z -= 0.2f;
         m_pPath->GetPathData ()->data [2] =  ( _pos ); 
		 _pos = pos;
		 _pos.y += 0.2f;
		 _pos.x -= 0.2f;
		 _pos.z += 0.2f;
         m_pPath->GetPathData ()->data[3] = ( _pos ); 
		 _pos = pos;
		 _pos.y += 0.2f;
		 _pos.x += 0.2f;
		 _pos.z += 0.2f;
         m_pPath->GetPathData ()->data[4] = ( _pos ); 
		 m_pPath->UpdataData ();
	 }
};

void MSign::Render()
{
	if(m_pPath)
		m_pPath->Render ();
}

	
MSignSet::MSignSet()
{
    m_pSign = new MSign;
	m_nColorIndex = 0;
};
	
MSignSet::~MSignSet()
{
     Release();
};

	
void  MSignSet::PushData(MVec3 pos)
{
      if( m_nCurPos >= m_nNumPos )
		  return;

	  m_PosSet[m_nCurPos] = pos;
	  m_nCurPos ++ ;
};

void  MSignSet::Release()
{
    OGRE_DELETE( m_pSign );
};
	
void  MSignSet::UpdateData(MVec3 pos,float rad,int numRound)
{
	m_Round.SetDivideNum ( numRound );

	if( m_pSign == NULL )
		return;

	m_pSign->m_pPath ->GetPathData ()->data .clear ();
	m_pSign->m_pPath->GetPathData()->data.resize( numRound+1 );

	MVec3 _pos;
	for( int i = 0 ; i < numRound ; i ++)
	{
		_pos = pos ;

		MVec2  curPos = m_Round.m_RoundPosSet [i];
		_pos.x += rad*m_Round.m_RoundPosSet [i].x ;
		_pos.z += rad*m_Round.m_RoundPosSet [i].y ;

		//if( !g_Terrain.GetHeight ( _pos.x , _pos.z , &_pos.y ) )
		//{
	//		return ;
	//	}

		m_pSign->m_pPath ->GetPathData ()->data[i] = _pos;
	}
		
   if( !m_pSign->m_pPath ->GetPathData ()->data .empty ())
   {
	   m_pSign->m_pPath ->GetPathData ()->data[numRound] =  m_pSign->m_pPath ->GetPathData ()->data[0] ;
   }

	m_pSign->m_pPath ->UpdataData ();
};

void  MSignSet::UpdateData()
{
	m_nColorIndex = ( rand()/255 ) % 8;
}

void  MSignSet::Render()
{
    if( m_pSign )
	{
		if( m_PosSet.empty ())
		{	
			m_pSign->SetColor ( 0xff00ffff );	
			m_pSign->Render ();
		}
		else
		{
			for( unsigned int i = 0 ; i <  m_PosSet.size ()  ; i++)
			{			
				m_pSign->SetColor( g_nColorSet[m_nColorIndex]);
				m_pSign->SetPos( m_PosSet[i] );		
				m_pSign->Render();
			}
		}
	}
};

