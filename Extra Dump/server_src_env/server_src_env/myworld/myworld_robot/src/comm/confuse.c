/*
**  @file $RCSfile: confuse.c,v $
**  general description of this module
**  $Id: confuse.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned char sKey[]={\
    0xd6, 0x02, 0x08, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xd0, 0xca, 0x01, 0x00};

int confuse(unsigned char *pszLibC, int iLibSize)
{
	int i;

	for (i=0; i<iLibSize; i++)
	{
		*(pszLibC+i) ^= sKey[i%16];
	}

	return 0;
}

int main(int argc, char* argv[])
{
	char *pszInFile;
	char *pszOutFile;
	unsigned char *pszLibC;
	struct stat stStat;
	FILE *fp;

	if (argc != 3)
	{
		printf("arg err, should be: confuse infile_name outfilename\n");
		return -1;
	}
	
	pszInFile = argv[1];
	pszOutFile = argv[2];

	if (0 != stat(pszInFile, &stStat))
	{
		return -1;
	}

	fp = fopen(pszInFile, "rb");
	if (NULL == fp)
	{
		return -1;
	}

	pszLibC = malloc(stStat.st_size);
	if (NULL == pszLibC)
	{
		return -1;
	}

	fread(pszLibC, stStat.st_size, 1, fp);
	fclose(fp);

	if (0 > confuse(pszLibC, stStat.st_size))
	{
		return -1;
	}

	fp = fopen(pszOutFile, "w+");
	if (NULL == fp)
	{
		return -1;
	}
	
	fwrite(pszLibC, stStat.st_size, 1, fp);

	fclose(fp);
	free(pszLibC);	
	
	return 0;
}

