#include "pal/pal.h"
#include "ptgenimg.h"

int main(int argc, char* argv[])
{
	struct timeval stStart;
	struct timeval stEnd;
	struct timeval stDiff;
	int iMs;
	int i;
	char szStr[128];
	char szData[4096];
	int iLen;
		
	int iLoopNum = 100000;
	
	if (argc > 1)
	{
		iLoopNum = atoi(argv[1]);
	}

	gettimeofday(&stStart, NULL);

	for (i=0 ; i<iLoopNum; i++)
	{
		ptg_gen_rand_code(szStr, 4);
		iLen = sizeof(szData);
		if (0 > ptg_gen_image(szStr, 4, szData, &iLen))
		{
			return -1;
		}
	}

	gettimeofday(&stEnd, NULL);

	TV_DIFF(stDiff, stEnd, stStart);
	TV_TO_MS(iMs, stDiff);

	printf("benchmark gd loop %d time %d ms\n", iLoopNum, iMs);
	
	return 0;
}


