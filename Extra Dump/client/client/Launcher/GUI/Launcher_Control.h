#pragma once

class CClient_Control
{
public:
	POINT				m_Pos;
	HDC					m_hDestDC;
	int					m_nClientID;

public:
	CClient_Control();

	void				SetClientID( int nID )
	{
		m_nClientID = nID;
	}

	int					GetClientID()
	{
		return m_nClientID;
	}

	void				SetDestDC(HDC hDestDC);

	void				SetPos(int nX, int nY);
    void                SetPos(POINT pt) 
	{
		SetPos(pt.x, pt.y); 
	}

	int					X(void);
	int					Y(void);
};

class CClient_BitmapControl : public CClient_Control
{
protected:
	CClient_Image*		m_pImage;

public:
						CClient_BitmapControl();

	void				SetImage(CClient_Image* pImage);
	CClient_Image*		Image(void);

    virtual void        GetWindowRect(LPRECT rt)
    {
        rt->left = m_Pos.x; rt->top = m_Pos.y;
        rt->right = rt->left + m_pImage->Size()->cx;
        rt->bottom = rt->top + m_pImage->Size()->cy;
    }
};