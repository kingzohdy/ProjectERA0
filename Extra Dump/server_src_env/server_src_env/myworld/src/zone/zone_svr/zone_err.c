/*
**  @file $RCSfile: zone_err.c,v $
**  general description of this module
**  $Id: zone_err.c,v 1.4 2011/08/23 10:33:02 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2011/08/23 10:33:02 $
**  @version $Revision: 1.4 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_err.h"
#include <unistd.h>

#define ERR_MAX_STR 4096
#define UNUSED( var ) ((void)var)


static TERRLIB s_stLib;

HERR err_init(const char* pszPath, const char* pszLang, int iFlags)
{
	if( s_stLib.pstTree ) /* already inited. */
		return 0;

	terrimpl_init(&s_stLib);

	return terrimpl_load_file(&s_stLib, pszPath, pszLang);
	UNUSED(iFlags);	//add by paraunused.pl
}

void err_release(void)
{
	terrimpl_unload(&s_stLib);
}

HERR err_set_time_format(const char* pszFormat)
{
	return terrimpl_set_time_format(&s_stLib, pszFormat);
}

HERR err_set_output_format(const char* pszFormat)
{
	return terrimpl_set_output_format(&s_stLib, pszFormat);
}

HERR err_tostr(HERR hErr, char* pszErr, int* piLen)
{
	return terrimpl_translate(&s_stLib, hErr, pszErr, piLen);
}

HERR err_error(int iFile, HERR hErr)
{
	char szErr[ERR_MAX_STR];
	int iLen;
	HERR hRet;

	iLen	=	ERR_MAX_STR;

	hRet	=	terrimpl_translate(&s_stLib, hErr, szErr, &iLen);

	if( TERR_IS_ERROR(hRet) )
		return hRet;

	write(iFile, szErr, iLen);
	write(iFile, "\n", 1);

	return 0;
}

HERR err_ferror(FILE* fp, HERR hErr)
{
	char szErr[ERR_MAX_STR];
	int iLen;
	HERR hRet;

	iLen	=	ERR_MAX_STR;

	hRet	=	terrimpl_translate(&s_stLib, hErr, szErr, &iLen);

	fprintf(fp, "%s\n", szErr);

	return 0;
}

void err_perror(HERR hErr)
{
	char szErr[ERR_MAX_STR];
	int iLen;
	HERR hRet;

	iLen	=	ERR_MAX_STR;

	hRet	=	terrimpl_translate(&s_stLib, hErr, szErr, &iLen);

	fprintf(stderr, "%s\n", szErr);
}

const char* err_get(HERR hErr)
{
	return terrimpl_err2str(&s_stLib, hErr);
}


