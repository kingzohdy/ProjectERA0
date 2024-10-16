/*
**  @file $RCSfile: tdirent.h,v $
**  general description of this module
**  $Id: tdirent.h,v 1.3 2007/05/17 01:46:21 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2007/05/17 01:46:21 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TDIRENT_H

#define TDIRENT_H

#ifdef WIN32

#else /* WIN32 */

	#include <dirent.h>

#endif /* WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32

#ifdef UNICODE
	#define dirent _WIN32_FIND_DATAW
#else /* UNICODE */
	#define dirent _WIN32_FIND_DATAA
#endif /* UNICODE */

	#define d_name cFileName
	#define d_namlen dwReserved1
	/* dwReserved0 is internal flag */
	#define d_ino nFileSizeLow

	typedef struct
	{
		char *name;
		HANDLE handle;
		unsigned count;
	} DIR;

DIR *opendir(const char *dir);

struct dirent *readdir(DIR *dir);

int closedir(DIR *dir);

void rewinddir(DIR *dir);

long telldir(DIR *dir);

void seekdir(DIR *dir, long loc);

int dirfd(DIR *dir);


#endif /* WIN32 */

#ifdef __cplusplus
}
#endif

#endif /* TDIRENT_H */
