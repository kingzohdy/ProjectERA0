/*
**  @file $RCSfile: tdirent.h,v $
**  general description of this module
**  $Id: tdirent.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
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
