#ifndef __UI_RICHTEXT_BUILDER_H__
#define __UI_RICHTEXT_BUILDER_H__

#include <list>
#include <string>
#include "OgreColourValue.h"
#include "OgreRect.h"
#include "RichTextCommonType.h"

enum RichTextObjectType
{
	RTOT_TEXT,		// 文字对象
	RTOT_FACE,		// 表情对象
	RTOT_PICTURE,	// 图片对象
};

class RichTextObject
{
public:
	virtual ~RichTextObject()
	{}

	RichTextObjectType		m_type;					// 对象类型
	Ogre::RectFloat			m_rect;					// 对象占据的矩形区域，相对于所在行左下角
};

// 文字对象
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

	Ogre::ColorQuad			m_color;				// 文字颜色
	bool					m_bBlink;				// 文字是否闪烁
	char*					m_szText;				// 对象包含的文字
	bool					m_bLinkItem;			// 超链接字体
	std::string				m_strLinkItemHref;		// 超链接的内容
};

// 表情对象
class RichTextFace : public RichTextObject
{
public:
	RichTextFace()
	{ 
		m_type = RTOT_FACE; 
	}

	int						m_nIconStartIdx;		// 动画的开始索引
	int						m_nIconNum;				// 动画帧数
	unsigned int			m_dwFrameInterval;		// 动画帧间隔，毫秒
	unsigned int			m_dwStartTime;			// 动画开始时间
	int						m_nCode;				// 表情代码
};

// 图片对象
class RichTextPicture : public RichTextObject
{
public:
	RichTextPicture()
	{ 
		m_type = RTOT_PICTURE; 
	}

	int				m_nIconStartIdx;		// 动画的开始索引
	int				m_nIconNum;				// 动画帧数
	unsigned int	m_dwFrameInterval;		// 动画帧间隔，毫秒
	unsigned int	m_dwStartTime;			// 动画开始时间
	int				m_nCode;				// 表情代码
	std::string		m_strAnimText;
};

typedef std::list<RichTextObject*> RichTextObjectList;

class RichTextLine
{
public:
	~RichTextLine();
	Ogre::RectFloat			m_rect;
	RichTextObjectList		m_listObjects;			// 这一行所包含的对象列表
};

class RichText;
class RFPoint;

// 帮助构建RichText的对象，将一段文字加入RichText
class RRichTextBuilder
{
private:
	bool						m_bCenter;
	RFPoint						m_newObjStartPoint;
	RichText*					m_pRichText;
	const char*					m_szText;
	const char*					m_szPos;				// 当前正在处理的字符指针
	Ogre::ColorQuad				m_colorText;			// 文字颜色

	Ogre::ColorQuad				m_colorCur;				// 当前文字颜色
	bool						m_bBlink;				// 当前文字是否闪烁
	bool						m_bLinkItem;			// 超链接字体
	std::string					m_strLinkItemHref;		// 超链接的内容
	bool						m_bLinkBegin;			// 超链接开始标志
	std::vector<RichTextText *> m_LinkItemList;			// 处理超链接的数组
	
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
	// 构建新的一行
	bool BuildNewOneLine( RichTextLine*& pOneLine );
	bool IsRichViewRectTooSmall( char firstChar, char secondChar, const RichTextObjectList& oneLineTextList );

	// 构建新的一个对象,返回值表示要不要换行
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

	// 解析自定意颜色
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