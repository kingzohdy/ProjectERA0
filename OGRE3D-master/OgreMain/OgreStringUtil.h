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
#ifndef _StringUtil_H__
#define _StringUtil_H__

#include "OgrePrerequisites.h"
#include <xhash>
#include <sstream>

namespace Ogre {

	/** Utility class for manipulating Strings.  */
	class _OgreExport StringUtil
	{
	public:
		typedef std::ostringstream StrStreamType;

		static void init();

		/** Removes any whitespace characters, be it standard space or
		TABs and so on.
		@remarks
		The user may specify wether they want to trim only the
		beginning or the end of the String ( the default action is
		to trim both).
		*/
		static void trim( String& str, bool left = true, bool right = true );

		/** Returns a StringVector that contains all the substrings delimited
		by the characters in the passed <code>delims</code> argument.
		@param
		delims A list of delimiter characters to split by
		@param
		maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
		parameters is > 0, the splitting process will stop after this many splits, left to right.
		*/
		static std::vector< String > split( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

		/** Upper-cases all the characters in the string.
		*/
		static void toLowerCase( String& str );

		/** Lower-cases all the characters in the string.
		*/
		static void toUpperCase( String& str );


		/** Returns whether the string begins with the pattern passed in.
		@param pattern The pattern to compare with.
		@param lowerCase If true, the end of the string will be lower cased before
		comparison, pattern should also be in lower case.
		*/
		static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

		/** Returns whether the string ends with the pattern passed in.
		@param pattern The pattern to compare with.
		@param lowerCase If true, the end of the string will be lower cased before
		comparison, pattern should also be in lower case.
		*/
		static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

		/** Method for standardising paths - use forward slashes only, end with slash.
		*/
		static String standardisePath( const String &init);

		/** Method for splitting a fully qualified filename into the base name
		and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFilename(const String& qualifiedName,
			String& outBasename, String& outPath);

		/** Method for splitting a fully qualified filename into the base name,
		extension and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFullFilename(const Ogre::String& qualifiedName, 
			Ogre::String& outBasename, Ogre::String& outExtention, 
			Ogre::String& outPath);

		/** Method for splitting a filename into the base name
		and extension.
		*/
		static void splitBaseFilename(const Ogre::String& fullName, 
			Ogre::String& outBasename, Ogre::String& outExtention);


		/** Simple pattern-matching routine allowing a wildcard pattern.
		@param str String to test
		@param pattern Pattern to match against; can include simple '*' wildcards
		@param caseSensitive Whether the match is case sensitive or not
		*/
		static bool match(const String& str, const String& pattern, bool caseSensitive = true);


		static unsigned int hash(const String &str, unsigned int type);

		//if len < 0, pstr is zero terminated
		static unsigned int hash(const char *pstr, unsigned int type, int len=-1);


		/// Constant blank string, useful for returning by ref where local does not exist
		static const String BLANK;

		static int UnicodeToAnsi( char *pdestbuf, int maxchar, const wchar_t *psrc, int nsrcchar=-1 );
		static int AnsiToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar=-1 );
		static int UnicodeToUTF8( char *pdestbuf, int maxchar, const wchar_t *psrc, int nsrcchar=-1 );
		static int UTF8ToUnicode( wchar_t *pdestbuf, int maxchar, const char *psrc, int nsrcchar=-1 );
		static bool IsDBCSLeadByte(char c);

		static const char    *UnicodeToAnsi( const wchar_t *psrc, int nsrcchar=-1 );
		static const wchar_t *AnsiToUnicode( const char *psrc, int nsrcchar=-1 );
		static const char    *UnicodeToUTF8( const wchar_t *psrc, int nsrcchar=-1 );
		static const wchar_t *UTF8ToUnicode( const char *psrc, int nsrcchar=-1 );

	};


#if !defined( _STLP_HASH_FUN_H )
	typedef stdext::hash_compare< String, std::less< String > > _StringHash;
#else
	typedef std::hash< String > _StringHash;
#endif

} // namespace Ogre

#endif // _StringUtil_H__
