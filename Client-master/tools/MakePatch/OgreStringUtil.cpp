/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include <xhash>
#include <algorithm>
#include "OgreStringUtil.h"
#include <Windows.h>

//-----------------------------------------------------------------------
const String StringUtil::BLANK;

#define LIBMPQ_TOOLS_BUFSIZE		0x500	/* buffer size for the decryption engine */
static unsigned int	mpq_buffer[LIBMPQ_TOOLS_BUFSIZE];

static void initMPQBuffer()
{
	unsigned int seed   = 0x00100001;
	unsigned int index1 = 0;
	unsigned int index2 = 0;
	int i;

	/* Initialize the decryption buffer. */
	for (index1 = 0; index1 < 0x100; index1++)
	{
		for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			unsigned int temp1, temp2;
			seed  = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;

			seed  = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);

			mpq_buffer[index2] = (temp1 | temp2);
		}
	}
}

void StringUtil::init()
{
	initMPQBuffer();
}


//-----------------------------------------------------------------------
void StringUtil::trim(String& str, bool left, bool right)
{
	/*
	size_t lspaces, rspaces, len = length(), i;

	lspaces = rspaces = 0;

	if( left )
	{
	// Find spaces / tabs on the left
	for( i = 0;
	i < len && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
	++lspaces, ++i );
	}

	if( right && lspaces < len )
	{
	// Find spaces / tabs on the right
	for( i = len - 1;
	i >= 0 && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
	rspaces++, i-- );
	}

	*this = substr(lspaces, len-lspaces-rspaces);
	*/
	static const String delims = " \t\r";
	if(right)
		str.erase(str.find_last_not_of(delims)+1); // trim right
	if(left)
		str.erase(0, str.find_first_not_of(delims)); // trim left
}

//-----------------------------------------------------------------------
std::vector<String> StringUtil::split( const String& str, const String& delims, unsigned int maxSplits)
{
	std::vector<String> ret;
	// Pre-allocate some space for performance
	ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

	unsigned int numSplits = 0;

	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != String::npos);



	return ret;
}

//-----------------------------------------------------------------------
void StringUtil::toLowerCase(String& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		tolower);
}

//-----------------------------------------------------------------------
void StringUtil::toUpperCase(String& str) 
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		toupper);
}
//-----------------------------------------------------------------------
bool StringUtil::startsWith(const String& str, const String& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	String startOfThis = str.substr(0, patternLen);
	if (lowerCase)
		StringUtil::toLowerCase(startOfThis);

	return (startOfThis == pattern);
}
//-----------------------------------------------------------------------
bool StringUtil::endsWith(const String& str, const String& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	String endOfThis = str.substr(thisLen - patternLen, patternLen);
	if (lowerCase)
		StringUtil::toLowerCase(endOfThis);

	return (endOfThis == pattern);
}
//-----------------------------------------------------------------------
String StringUtil::standardisePath(const String& init)
{
	String path = init;

	std::replace( path.begin(), path.end(), '\\', '/' );
	if( path[path.length() - 1] != '/' )
		path += '/';

	return path;
}
//-----------------------------------------------------------------------
void StringUtil::splitFilename(const String& qualifiedName, 
							   String& outBasename, String& outPath)
{
	String path = qualifiedName;
	// Replace \ with / first
	std::replace( path.begin(), path.end(), '\\', '/' );
	// split based on final /
	size_t i = path.find_last_of('/');

	if (i == String::npos)
	{
		outPath.clear();
		outBasename = qualifiedName;
	}
	else
	{
		outBasename = path.substr(i+1, path.size() - i - 1);
		outPath = path.substr(0, i+1);
	}

}
//-----------------------------------------------------------------------
void StringUtil::splitBaseFilename(const String& fullName, 
								   String& outBasename, String& outExtention)
{
	size_t i = fullName.find_last_of(".");
	if (i == String::npos)
	{
		outExtention.clear();
		outBasename = fullName;
	}
	else
	{
		outExtention = fullName.substr(i+1);
		outBasename = fullName.substr(0, i);
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------------------
void StringUtil::splitFullFilename(	const String& qualifiedName, 
								   String& outBasename, String& outExtention, String& outPath )
{
	String fullName;
	splitFilename( qualifiedName, fullName, outPath );
	splitBaseFilename( fullName, outBasename, outExtention );
}
//-----------------------------------------------------------------------
bool StringUtil::match(const String& str, const String& pattern, bool caseSensitive)
{
	String tmpStr = str;
	String tmpPattern = pattern;
	if (!caseSensitive)
	{
		StringUtil::toLowerCase(tmpStr);
		StringUtil::toLowerCase(tmpPattern);
	}

	String::const_iterator strIt = tmpStr.begin();
	String::const_iterator patIt = tmpPattern.begin();
	String::const_iterator lastWildCardIt = tmpPattern.end();
	while (strIt != tmpStr.end() && patIt != tmpPattern.end())
	{
		if (*patIt == '*')
		{
			lastWildCardIt = patIt;
			// Skip over looking for next character
			++patIt;
			if (patIt == tmpPattern.end())
			{
				// Skip right to the end since * matches the entire rest of the string
				strIt = tmpStr.end();
			}
			else
			{
				// scan until we find next pattern character
				while(strIt != tmpStr.end() && *strIt != *patIt)
					++strIt;
			}
		}
		else
		{
			if (*patIt != *strIt)
			{
				if (lastWildCardIt != tmpPattern.end())
				{
					// The last wildcard can match this incorrect sequence
					// rewind pattern to wildcard and keep searching
					patIt = lastWildCardIt;
					lastWildCardIt = tmpPattern.end();
				}
				else
				{
					// no wildwards left
					return false;
				}
			}
			else
			{
				++patIt;
				++strIt;
			}
		}

	}
	// If we reached the end of both the pattern and the string, we succeeded
	if (patIt == tmpPattern.end() && strIt == tmpStr.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int StringUtil::hash(const char *pstr, unsigned int type, int len)
{
	unsigned int seed1 = 0x7FED7FED;
	unsigned int seed2 = 0xEEEEEEEE;
	unsigned int ch;			/* One key character */

	/* Prepare seeds */
	if(len < 0)
	{
		while (*pstr != 0) {
			ch = toupper(*pstr++);
			seed1 = mpq_buffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
		}
	}
	else
	{
		while(len > 0) {
			ch = toupper(*pstr++);
			seed1 = mpq_buffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;

			len--;
		}
	}

	return seed1;
}

unsigned int StringUtil::hash(const String &str, unsigned int type)
{
	return hash(str.c_str(), type);
}

#ifdef _WIN32
int StringUtil::UnicodeToAnsi( char *pdestbuf, int maxchar, const wchar_t *psrc, int nsrcchar )
{
	int nchar;
#ifdef _DEBUG
	nchar = WideCharToMultiByte( CP_ACP, WC_SEPCHARS|WC_COMPOSITECHECK, psrc, nsrcchar, pdestbuf, 0, NULL, NULL );
	//assert( nchar < maxchar );
#endif

	nchar = WideCharToMultiByte(
		CP_ACP,
		WC_SEPCHARS | WC_COMPOSITECHECK,
		psrc,
		nsrcchar,
		pdestbuf,
		maxchar,
		NULL,
		NULL );
	//assert( nchar > 0 );
	return nchar;
}

int StringUtil::AnsiToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar )
{
	int nchar;
#ifdef _DEBUG
	nchar = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, psrc, nsrcchar, pdestbuf, 0 );
	//assert( nchar <= maxchar );
#endif

	nchar = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		psrc,
		nsrcchar,
		pdestbuf,
		maxchar );

	//assert( nchar > 0 );
	return nchar;
}

bool StringUtil::IsDBCSLeadByte(char c)
{
	return ::IsDBCSLeadByte((unsigned char)c)==TRUE;
}

#endif

int StringUtil::UnicodeToUTF8( char *pdestbuf, int maxchar, const wchar_t *psrc, int nsrcchar )
{
	unsigned char *pdst = (unsigned char *)pdestbuf;
	const wchar_t *pcur = psrc;
	int src_count = nsrcchar<0 ? MAX_INT : nsrcchar;

	while(*pcur && src_count>0 )
	{
		src_count--;
		wchar_t c = *pcur++;

		if( c < 0x80 )
		{
			*pdst++ = (unsigned char)c;
		}
		else if( c < 0x800 )
		{
			*pdst++ = (c>>6) | 0xC0;
			*pdst++ = (c&0x3F) | 0x80;
		}
		else
		{
			*pdst++ = (c>>12) | 0xE0;
			*pdst++ = ((c>>6)&0x3F) | 0x80;
			*pdst++ = (c&0x3F) | 0x80;
		}
	}
	*pdst++ = 0;

	int nchar = int((char *)pdst - (char *)pdestbuf - 1);
	//assert(nchar < maxchar);
	return nchar;
}

int StringUtil::UTF8ToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar )
{
	wchar_t *pdst = pdestbuf;
	const unsigned char *pcur = (unsigned char *)psrc;
	int src_count = nsrcchar<0 ? MAX_INT : nsrcchar;

	while( *pcur && src_count>0 )
	{
		src_count--;
		unsigned char head = *pcur++;
		unsigned char mask = 0x80;
		wchar_t  c = 0;

		int bitshift = 0;
		if( head&mask )
		{
			mask >>= 1;
			while( head&mask )
			{
				//assert( *pcur!=0 && src_count>0 );
				src_count--;

				mask >>= 1;
				c = (c<<6) | (*pcur++ & 0x3F);
				bitshift += 6;
			}
		}

		c |= (head&(mask-1)) << bitshift;
		*pdst++ = c;
	}
	*pdst++ = 0;

	int nchar = int((char *)pdst - (char *)pdestbuf - 1);
	//assert(nchar < maxchar);
	return nchar;
}

const size_t MAX_CONVERT = 4096*2;
static char s_DBCSBuffer[MAX_CONVERT];
static wchar_t s_UnicodeBuffer[MAX_CONVERT];

const char *StringUtil::UnicodeToAnsi( const wchar_t *psrc, int nsrcchar )
{
	StringUtil::UnicodeToAnsi(s_DBCSBuffer, MAX_CONVERT, psrc, nsrcchar);
	return s_DBCSBuffer;
}

const wchar_t *StringUtil::AnsiToUnicode( const char *psrc, int nsrcchar )
{
	StringUtil::AnsiToUnicode(s_UnicodeBuffer, MAX_CONVERT, psrc, nsrcchar);
	return s_UnicodeBuffer;
}

const char    *StringUtil::UnicodeToUTF8( const wchar_t *psrc, int nsrcchar )
{
	StringUtil::UnicodeToUTF8(s_DBCSBuffer, MAX_CONVERT, psrc, nsrcchar);
	return s_DBCSBuffer;
}

const wchar_t *StringUtil::UTF8ToUnicode( const char *psrc, int nsrcchar )
{
	StringUtil::UTF8ToUnicode(s_UnicodeBuffer, MAX_CONVERT, psrc, nsrcchar);
	return s_UnicodeBuffer;
}

/* This function tests, whether the ISO 10646/Unicode character code 
* ucs belongs into the East Asian Wide (W) or East Asian FullWidth 
* (F) category as defined in Unicode Technical Report #11. In this 
* case, the terminal emulator should represent the character using a 
* a glyph from a double-wide font that covers two normal (Latin) 
* character cells. */ 
int iswide(int ucs)
{
	if (ucs < 0x1100) return 0;
	else return 
		(ucs >= 0x1100 && ucs <= 0x115f) || /* Hangul Jamo */ 
		(ucs >= 0x2e80 && ucs <= 0xa4cf && (ucs & ~0x0011) != 0x300a && 
		ucs != 0x303f) ||                     /* CJK ... Yi */ 
		(ucs >= 0xac00 && ucs <= 0xd7a3) || /* Hangul Syllables */ 
		(ucs >= 0xf900 && ucs <= 0xfaff) || /* CJK Compatibility Ideographs */ 

		(ucs >= 0xfe30 && ucs <= 0xfe6f) || /* CJK Compatibility Forms */ 
		(ucs >= 0xff00 && ucs <= 0xff5f) || /* Fullwidth Forms */ 
		(ucs >= 0xffe0 && ucs <= 0xffe6); 
}
