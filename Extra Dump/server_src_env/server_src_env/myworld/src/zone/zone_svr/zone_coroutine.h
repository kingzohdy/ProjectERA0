/*
**  @file $RCSfile: zone_coroutine.h,v $
**  general description of this module
**  $Id: zone_coroutine.h,v 1.1 2013/08/13 08:29:55 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/08/13 08:29:55 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef ZONE_COROUTINE_H
#define ZONE_COROUTINE_H

#include "zone_svr.h"



#define CO_BEGIN switch (pstEnv->stCoroutineRPC.Step)\
	{\
	case 0:

#define CO_YIELD(a) player_rpc_call(pstEnv,pstPlayer);\
		return pstEnv->stCoroutineRPC.Step;\
		break;\
	case a:

#define CO_END return 0;\
		break;\
	default:\
		return -1;\
		break;\
	}

#define DB_CO_BEGIN switch (pstEnv->stDBCoroutineRPC.TormCmd)\
	{\
	case 0:

#define DB_CO_YIELD db_rpc_call(pstEnv,pstPlayer);\
		return pstEnv->stDBCoroutineRPC.TormCmd;

#define DB_CO_RESUME(a) break;\
	case a:		

#define DB_CO_END return 0;\
		break;\
	default:\
		return -1;\
		break;\
	}

int player_rpc_call(ZONESVRENV *pstEnv,Player *pstPlayer);
int z_ss_coroutine_rpc(ZONESVRENV * pstEnv,COROUTINERPC *pstCoroutineRpc);
int co_init(COROUTINERPC *pstRpc);

int db_co_init(ZONESVRENV* pstEnv);
int db_rpc_call(ZONESVRENV *pstEnv,Player *pstPlayer);
int z_db_coroutine_rpc(ZONESVRENV * pstEnv,DBCOROUTINERPC *pstDBCoroutineRpc,TORMPKG *pstPkg,Player * pstPlayer);

#endif /* ZONE_COROUTINE_H */
