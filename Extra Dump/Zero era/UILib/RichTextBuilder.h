#ifndef __UI_RICHTEXT_BUILDER_H__
#define __UI_RICHTEXT_BUILDER_H__

#include <list>
#include <string>
#include "OgreColourValue.h"
#include "OgreRect.h"
#include "RichTextCommonType.h"

enum RichTextObjectType
{
	RTOT_TEXT,		// ���ֶ���
	RTOT_FACE,		// �������
	RTOT_PICTURE,	// ͼƬ����
};

class RichTextObject
{
public:
	virtual ~RichTextObject()
	{}

	RichTextObjectType		m_type;					// ��������
	Ogre::RectFloat			m_rect;					// ����ռ�ݵľ���������������������½�
};

// ���ֶ���
class RichTextText : public RichTextObject
{
public:
	RichTextText()
	{
		m_szText	= NULL; 
		m_type		= RTOT_TEXT;
	}

	~RichTextText()
	{
		if ( m_szText != NULL )
		{
			delete[] m_szText;
		}
	}

	Ogre::ColorQuad			m_color;				// ������ɫ
	bool					m_bBlink;				// �����Ƿ���˸
	char*					m_szText;				// �������������
	bool					m_bLinkItem;			// ����������
	std::string				m_strLinkItemHref;		// �����ӵ�����
};

// �������
class RichTextFace : public RichTextObject
{
public:
	RichTextFace()
	{ 
		m_type = RTOT_FACE; 
	}

	int						m_nIconStartIdx;		// �����Ŀ�ʼ����
	int						m_nIconNum;				// ����֡��
	unsigned int			m_dwFrameInterval;		// ����֡���������
	unsigned int			m_dwStartTime;			// ������ʼʱ��
	int						m_nCode;				// �������
};

// ͼƬ����
class RichTextPicture : public RichTextObject
{
public:
	RichTextPicture()
	{ 
		m_type = RTOT_PICTURE; 
	}

	int				m_nIconStartIdx;		// �����Ŀ�ʼ����
	int				m_nIconNum;				// ����֡��
	unsigned int	m_dwFrameInterval;		// ����֡���������
	unsigned int	m_dwStartTime;			// ������ʼʱ��
	int				m_nCode;				// �������
	std::string		m_strAnimText;
};

typedef std::list<RichTextObject*> RichTextObjectList;

class RichTextLine
{
public:
	~RichTextLine();
	Ogre::RectFloat			m_rect;
	RichTextObjectList		m_listObjects;			// ��һ���������Ķ����б�
};

class RichText;
class RFPoint;

// ��������RichText�Ķ��󣬽�һ�����ּ���RichText
class RRichTextBuilder
{
private:
	bool						m_bCenter;
	RFPoint						m_newObjStartPoint;
	RichText*					m_pRichText;
	const char*					m_szText;
	const char*					m_szPos;				// ��ǰ���ڴ�����ַ�ָ��
	Ogre::ColorQuad				m_colorText;			// ������ɫ

	Ogre::ColorQuad				m_colorCur;				// ��ǰ������ɫ
	bool						m_bBlink;				// ��ǰ�����Ƿ���˸
	bool						m_bLinkItem;			// ����������
	std::string					m_strLinkItemHref;		// �����ӵ�����
	bool						m_bLinkBegin;			// �����ӿ�ʼ��־
	std::vector<RichTextText *> m_LinkItemList;			// �������ӵ�����
	
	bool CanBuildNewOneLine();

	bool IsNewLineSymbol( char firstChar, char secondChar );
	bool OnParseNewLineChar( char& firstChar, char& secondChar );

	bool IsLinkSymbol( char firstChar, char secondChar );
	bool OnParseLinkChar();
	
	bool IsWidthEnoughToPutOneNewFace();
	bool IsFaceCharPosSymbol();
	bool OnParseFaceChar( RichTextLine* pOneLine, char& firstChar, char& secondChar );
	bool OnParsePictureChar( RichTextLine* pOneLine );
	bool OnParseSpecialFunctionChar( char& firstChar, char& secondChar );

	bool IsSelfDefineFormatSymbol( char firstChar, char secondChar );
	bool OnParseSelfDefineFormatText( RichTextLine* pOneLine, char& firstChar, char& secondChar );
	
	bool OnParseNormalText( RichTextLine* pOneLine );

	bool OnFirstCharNotZero( RichTextLine* pOneLine, char& firstChar, char& secondChar );
	// �����µ�һ��
	bool BuildNewOneLine( RichTextLine*& pOneLine );
	bool IsRichViewRectTooSmall( char firstChar, char secondChar, const RichTextObjectList& oneLineTextList );

	// �����µ�һ������,����ֵ��ʾҪ��Ҫ����
	bool			ParseTextObjectTwoNeighbourChar( int& nLen, char* szTwoChars, unsigned char* szCurTextObjContent );
	bool			ParseTextObjectTwoNeighbourChar( char* szTwoChars, std::string& strCurTextObjContent );
	bool			CalculateOneTextObjectViewRect( Ogre::RectFloat& textObjViewRect, const char* pszTwoChars );
	RichTextText*	BuildNewOneTextObject( const char* pszCurTextObjContent, Ogre::RectFloat& textObjViewRect );
	bool IsChinese( unsigned char chOneChar );
	bool IsEnglish( unsigned char chOneChar );
	bool IsOmitSymbol( unsigned char chOneChar );
	
	RichTextObject* NewTextObject();
	RichTextObject* NewFaceObject();
	RichTextObject* NewPictureObject( int& nOffset );

	// �����Զ�����ɫ
	void SetCustomColor();

	bool CanBuildText( const char *szText, RichText *pRichText );
	void RemoveRedundantLineBeforeBuildNewText();
	void Init( const char *szText, RichText* pRichText, const Ogre::ColorQuad& color );
	
	void UpdateAutoExtendRichAfterBuildOneLine( const RichTextLine* pOneLine );

	bool NeedScrollNotAutoExtendRichAfterBuildOneLine( const RichTextLine* pOneLine );
	void UpdateNotAutoExtendRichAfterBuildOneLine( const RichTextLine* pOneLine );


	void UpdateRichAfterBuildOneLine( const RichTextLine* pOneLine );

public:
	void BuildText( const char *szText, RichText* pRichText, const Ogre::ColorQuad& color );
};

inline bool RRichTextBuilder::IsNewLineSymbol( char firstChar, char secondChar )
{
	return firstChar == '\n' || firstChar == '\r' || ( firstChar == '#' && secondChar == 'r' );
}

inline bool RRichTextBuilder::IsLinkSymbol( char firstChar, char secondChar )
{
	return firstChar == '@' && secondChar == '@';
}

inline bool RRichTextBuilder::IsSelfDefineFormatSymbol( char firstChar, char secondChar )
{
	return firstChar == '#' && secondChar != '#';
}

inline bool RRichTextBuilder::IsFaceCharPosSymbol()
{
	return *( m_szPos + 1 ) == '9' && *( m_szPos + 2 ) == '9' && *( m_szPos + 3 ) == '9';
}

inline bool RRichTextBuilder::IsRichViewRectTooSmall( char firstChar, char secondChar, const RichTextObjectList& oneLineTextList )
{
	if ( IsNewLineSymbol( firstChar, secondChar ) )
	{
		return false;
	}

	return oneLineTextList.empty();
}

inline bool RRichTextBuilder::IsChinese( unsigned char chOneChar )
{
	return chOneChar >= 0x81;
}

inline bool RRichTextBuilder::IsEnglish( unsigned char chOneChar )
{
	return ( chOneChar >= 32 ) && ( chOneChar <= 127 );
}

inline bool RRichTextBuilder::IsOmitSymbol( unsigned char chOneChar )
{
	return chOneChar == '#';
}

#endif