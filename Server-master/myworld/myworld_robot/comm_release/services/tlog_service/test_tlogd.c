/*
**  @file $RCSfile: test_tlogd.c,v $
**  general description of this module
**  $Id: test_tlogd.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "tlog/tlog.h"
#include "tdr/tdr.h"

TLOGCONF g_stLogConf;
TLOGCTX  g_stLogCtx;
LPTDRMETA g_pstMeta;


TLOGCATEGORYINST* get_log_cat(char *pszLogConf, char *pszCatName)
{
	TDRDATA stHost;
		
	stHost.pszBuff = (char *)&g_stLogConf;
	stHost.iBuff = (int)sizeof(g_stLogConf);
	if (0 >	tdr_input_file(g_pstMeta, &stHost, pszLogConf, 0, 0))
	{
		printf("load %s fail\n", pszLogConf);
		return NULL;
	}

	if (0 > tlog_init(&g_stLogCtx, &g_stLogConf))
	{
		printf("log init fail\n");
		return NULL;
	}

	return tlog_get_category(&g_stLogCtx, pszCatName);
}

int main(int argc, char* argv[])
{
	TLOGCATEGORYINST* pstCat;

	g_pstMeta = tdr_get_meta_by_name((LPTDRMETALIB)(tlog_get_meta_data()), "TLOGConf");
	if (!g_pstMeta)
	{
		printf("get meta TLOGConf fail\n");
		return -1;
	}

	pstCat = get_log_cat("./test_file.xml", "test");
	if (!pstCat)
	{
		printf("get_log_cat from test_file.xml fail\n");
		return -1;
	}
	tlog_info(pstCat, 0, 0, "hello world test_file");
	tlog_fini(&g_stLogCtx);

	pstCat = get_log_cat("./test_net.xml", "test");
	if (!pstCat)
	{
		printf("get_log_cat from test_net.xml fail\n");
		return -1;
	}
	tlog_info(pstCat, 0, 0, "hello world test_net");
	tlog_fini(&g_stLogCtx);

	pstCat = get_log_cat("./test_vec.xml", "VecTest");
	if (!pstCat)
	{
		printf("get_log_cat from test_vec.xml fail\n");
		return -1;
	}
	tlog_info(pstCat, 1, 0, "hello world test_vec id=1");
	tlog_info(pstCat, 2, 0, "hello world test_vec id=2");
	tlog_info(pstCat, 101, 0, "hello world test_vec id=101");
	tlog_info(pstCat, 102, 0, "hello world test_vec id=102");
	tlog_fini(&g_stLogCtx);
	
	return 0;
}


