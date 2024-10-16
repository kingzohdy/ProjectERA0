/*
**  @file $RCSfile: tconnmgr.c,v $
**  general description of this module
**  $Id: tconnmgr.c,v 1.3 2010/08/24 09:20:21 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2010/08/24 09:20:21 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "pal/pal.h"
#include "tdr/tdr.h"
#include "tbus/tbus.h"
#include "tconnmgr.h"


#ifdef WIN32
	#pragma warning(disable:4996)
#endif

struct tagTDRSizeInfo
{
	TDROFF iNOff;   /**<网络传输信息偏移*/
	TDROFF iHOff;    /**<本地存储信息偏移*/
	int iUnitSize;	
	int idxSizeType;	/*保存sizeinfo 用简单类型打包方法时使用类型，在TDR_BUILD version 5时加入*/
};
typedef struct tagTDRSizeInfo TDRSIZEINFO;
typedef struct tagTDRSizeInfo *LPTDRSIZEINFO;

extern unsigned char g_szMetalib_proto_cs[];

extern int tdr_sizeinfo_to_off_i(LPTDRSIZEINFO a_pstRedirector, LPTDRMETA a_pstMeta, int a_iEntry, const char* a_pszName);


extern int tconnd_get_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread);
extern int tconnd_get_thttp_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread);
extern int tconnd_get_none_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread);
extern int tconnd_get_auth_pkglen(TCONNINST* pstInst, TCONNTHREAD* pstThread);

int tconnd_fini_tdrinstlist(TDRINSTLIST* pstTDRInstList)
{
	int i;

	for(i=0; i<pstTDRInstList->iCount; i++)
	{
		tdr_free_lib(&pstTDRInstList->astInsts[i].pstLib);
	}

	pstTDRInstList->iCount	=	0;

	return 0;
}

int tconnd_init_tdrinstlist(TDRINSTLIST* pstTDRInstList, TDRLIST* pstTDRList)
{
	TDR* pstTDR;
	TDRINST* pstTDRInst;
	int iIsFail	=	0;

	pstTDRInstList->iCount	=	0;

	while( pstTDRInstList->iCount < pstTDRList->Count )
	{
		if( pstTDRInstList->iCount >= (int) (sizeof(pstTDRInstList->astInsts)/sizeof(pstTDRInstList->astInsts[0])) )
		{
			iIsFail	=	1;
			break;
		}

		pstTDR		=	pstTDRList->TDRs + pstTDRInstList->iCount;
		pstTDRInst	=	pstTDRInstList->astInsts + pstTDRInstList->iCount;

		if( tdr_load_metalib(&pstTDRInst->pstLib, pstTDR->Path)<0 )
		{
			iIsFail	=	1;
			printf("tdr_load_metalib failed:path = %s\n",pstTDR->Path);
			break;
		}

		STRNCPY(pstTDRInst->szName, pstTDR->Name, sizeof(pstTDRInst->szName));

		pstTDRInstList->iCount++;
	}

	if( !iIsFail )
	{
		return 0;
	}

	tconnd_fini_tdrinstlist(pstTDRInstList);

	return -1;
}

int tconnd_find_lib(LPTDRINSTLIST pstTDRInstList, const char* pszName, LPTDRMETALIB* ppstLib)
{
	int i;
	int iIsFind = 0;
	LPTDRMETALIB pstFind = NULL;

	if( !pszName || !pszName[0] )
	{
		if( ppstLib )
		{
			*ppstLib	=	NULL;
		}

		return 0;
	}

	for(i=0; i<pstTDRInstList->iCount; i++)
	{
		if( !strcasecmp(pszName, pstTDRInstList->astInsts[i].szName) )
		{
			pstFind	=	pstTDRInstList->astInsts[i].pstLib;
			iIsFind	=	1;
			break;
		}
	}

	if( !iIsFind )
	{
		return -1;
	}

	if( ppstLib )
	{
		*ppstLib	=	pstFind;
	}

	return 0;
}

int tconnd_find_meta(TDRINSTLIST* pstTDRInstList, const char* pszName, LPTDRMETALIB pstPrefer, LPTDRMETA* ppstFind)
{
	LPTDRMETALIB pstLib;
	LPTDRMETA pstMeta;
	int iIsFind;
	int i;

	if( !pszName || !pszName[0] )
		return 0;

	if( pstPrefer )
	{
		pstMeta	=	tdr_get_meta_by_name(pstPrefer, pszName);

		if( !pstMeta )
		{
			return -1;
		}
		else
		{
			*ppstFind	=	pstMeta;
			return 0;
		}
	}

	iIsFind	=	0;

	for(i=0; i<pstTDRInstList->iCount; i++)
	{
		pstLib	=	pstTDRInstList->astInsts[i].pstLib;

		if( pstLib==pstPrefer )
			continue;

		pstMeta	=	tdr_get_meta_by_name(pstLib, pszName);

		if( pstMeta )
		{
			iIsFind	=	1;
			break;
		}
	}

	if( iIsFind )
	{
		*ppstFind	=	pstMeta;
		return 0;
	}
	else
	{
		return -1;
	}
}


int tconnd_init_pdulen_tdrparser(TDRINSTLIST* pstTDRInstList, PDUINST* pstPDUInst, PDU* pstPDU)
{
	LPTDRMETALIB pstPrefer=NULL;
	int iRet ;
	TDRSIZEINFO stSizeInfo;
	LPPDULENTDRPARSERINST pstTDRParserInst; 
	LPPDULENTDRPARSER pstTDRParser ;

	pstTDRParserInst = &pstPDUInst->stLenParser.stTDRParser;
	pstTDRParser = &pstPDU->LenParser.TDRParser;
	if( tconnd_find_lib(pstTDRInstList, pstTDRParser->TDR, &pstPrefer)<0 )
	{
        printf("tconnd_find_lib load Metalib failed!TDR Name = %s\n",pstTDRParser->TDR);
		return -1;
	}
	
	if( tconnd_find_meta(pstTDRInstList, pstTDRParser->Pkg, pstPrefer, &pstTDRParserInst->pstPkg)<0 )
	{
        printf("tconnd_find_meta find meta failded!Meta Name = %s\n",pstTDRParser->Pkg);
		return -1;
	}

	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstTDRParserInst->pstPkg, -1, pstTDRParser->PkgLen);
	if (TDR_SUCCESS == iRet)
	{
		pstTDRParserInst->iPkgLenNOff = stSizeInfo.iNOff;
		pstTDRParserInst->iPkgLenUnitSize = stSizeInfo.iUnitSize;
	}
	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstTDRParserInst->pstPkg, -1, pstTDRParser->HeadLen);
	if (TDR_SUCCESS == iRet)
	{
		pstTDRParserInst->iHeadLenNOff = stSizeInfo.iNOff;
		pstTDRParserInst->iHeadLenUnitSize = stSizeInfo.iUnitSize;
	}
	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstTDRParserInst->pstPkg, -1, pstTDRParser->BodyLen);
	if (TDR_SUCCESS == iRet)
	{
		pstTDRParserInst->iBodyLenNOff = stSizeInfo.iNOff;
		pstTDRParserInst->iBodyLenUnitSize = stSizeInfo.iUnitSize;
	}
	if ((0 >= pstTDRParserInst->iPkgLenUnitSize) && (
		(0 >= pstTDRParserInst->iHeadLenUnitSize) || (0 >= pstTDRParserInst->iBodyLenUnitSize)))
	{
		printf("failed to get the entry contained length of PDU<%s>", pstTDRParser->Pkg);
		return -1;
	}	
	
	pstPDUInst->iUpUnit   =     pstPDU->UpSize;
	pstPDUInst->iDownUnit =   pstPDU->DownSize;
	pstPDUInst->iUnit	=	tdr_get_meta_size(pstTDRParserInst->pstPkg);

	if( pstPDUInst->iUpUnit <= 0 )
	{
		pstPDUInst->iUpUnit	=	pstPDUInst->iUnit;
	}
	if( pstPDUInst->iDownUnit <= 0 )
	{
		pstPDUInst->iDownUnit	=	pstPDUInst->iUnit;
	}

    pstPDUInst->iUnit              =     (pstPDUInst->iDownUnit > pstPDUInst->iUpUnit)?(pstPDUInst->iDownUnit):(pstPDUInst->iUpUnit);
	pstPDUInst->iUnit	=	(pstPDUInst->iUnit + 0x400 - 1)/0x400*0x400;
	pstPDUInst->iUpUnit      	=	(pstPDUInst->iUpUnit + 0x400 - 1)/0x400*0x400;

	pstPDUInst->pfnGetPkgLen = tconnd_get_pkglen;
	return 0;
}

int tconnd_init_pdulen_thttpparser(PDUINST* pstPDUInst, PDU* pstPDU)
{
	pstPDUInst->iUpUnit   =     pstPDU->UpSize;
	pstPDUInst->iDownUnit =   pstPDU->DownSize;
	if( pstPDUInst->iUpUnit <= 0 )
	{
           pstPDUInst->iUpUnit = TCONND_DEFAULT_UP_PKG_LEN;
	}
	if( pstPDUInst->iDownUnit <= 0 )
	{
           pstPDUInst->iDownUnit = TCONND_DEFAULT_DOWN_PKG_LEN;
	}

	pstPDUInst->iUnit = ( pstPDUInst->iDownUnit > pstPDUInst->iUpUnit )?( pstPDUInst->iDownUnit ):( pstPDUInst->iUpUnit );
	pstPDUInst->iUnit	=	(pstPDUInst->iUnit + 0x400 - 1)/0x400*0x400;
	pstPDUInst->iUpUnit	=	(pstPDUInst->iUpUnit + 0x400 - 1)/0x400*0x400;

	pstPDUInst->pfnGetPkgLen = tconnd_get_thttp_pkglen;
	
	return 0;
}

int tconnd_init_pdulen_noneparser(PDUINST* pstPDUInst, PDU* pstPDU)
{
	pstPDUInst->iUpUnit   =     pstPDU->UpSize;
	pstPDUInst->iDownUnit =   pstPDU->DownSize;
	if( pstPDUInst->iUpUnit <= 0 )
	{
           pstPDUInst->iUpUnit = TCONND_DEFAULT_UP_PKG_LEN;
	}
	if( pstPDUInst->iDownUnit <= 0 )
	{
           pstPDUInst->iDownUnit = TCONND_DEFAULT_DOWN_PKG_LEN;
	}

	pstPDUInst->iUnit = ( pstPDUInst->iDownUnit > pstPDUInst->iUpUnit )?( pstPDUInst->iDownUnit ):( pstPDUInst->iUpUnit );
	pstPDUInst->iUnit	=	(pstPDUInst->iUnit + 0x400 - 1)/0x400*0x400;
	pstPDUInst->iUpUnit	=	(pstPDUInst->iUpUnit + 0x400 - 1)/0x400*0x400;

	pstPDUInst->pfnGetPkgLen = tconnd_get_none_pkglen;
	
	return 0;
}

int tconnd_init_pdulen_authparser(TDRINSTLIST* pstTDRInstList, PDUINST* pstPDUInst, PDU* pstPDU)
{
	LPTDRMETALIB pstPrefer=NULL;
	int iRet ;
	TDRSIZEINFO stSizeInfo;
	LPPDULENAUTHPARSERINST pstAuthParserInst; 
	LPPDULENAUTHPARSER pstAuthParser ;
	LPTDRMETA pstMeta;
	int iSize;

	pstAuthParserInst = &pstPDUInst->stLenParser.stAuthParser;
	pstAuthParser = &pstPDU->LenParser.AuthParser;

	pstPDUInst->iUpUnit          = pstPDU->UpSize;
	pstPDUInst->iDownUnit      = pstPDU->DownSize;
	
	if( pstAuthParser->TDR[0] )
	{
		if( tconnd_find_lib(pstTDRInstList, pstAuthParser->TDR, &pstPrefer)<0 )
		{
                     printf("Error:tconnd_find_lib failed! TDR Name =%s\n", pstAuthParser->TDR);
			return -1;
		}
	
		if( pstAuthParser->SendPkg[0] )
		{
			if( tconnd_find_meta(pstTDRInstList, pstAuthParser->SendPkg, pstPrefer, &pstMeta)<0 )
			{
                            printf("Error:tconnd_find_meta failed! Send Meta Name =%s\n",pstAuthParser->SendPkg);
				return -1;
			}

			iSize	=	tdr_get_meta_size(pstMeta);

			if( pstPDUInst->iUpUnit <= 0 )
			{
				pstPDUInst->iUpUnit	=	iSize + sizeof(PDUFRAME);
			}
			else if( pstPDUInst->iUpUnit < (int)(iSize+sizeof(PDUFRAME)) )
			{
                            printf("Warning:PDU MaxSize Exceeds Upsize,MaxSize = %d,Upsize = %d \n",(iSize+sizeof(PDUFRAME)),pstPDUInst->iUpUnit);
			}
		}
		else
		{
                    printf("Send Meta Name For AuthParser Incorrect\n");
		      return -1;
		}
		
		if( pstAuthParser->RecvPkg[0] )
		{
			if( tconnd_find_meta(pstTDRInstList, pstAuthParser->RecvPkg, pstPrefer, &pstMeta)<0 )
			{
                            printf("tconnd_find_meta failed!Recv Meta Name = %s\n",pstAuthParser->RecvPkg);
				return -1;
			}

			iSize	=	tdr_get_meta_size(pstMeta);

			if( pstPDUInst->iDownUnit <= 0 )
			{
				pstPDUInst->iDownUnit =	iSize + sizeof(PDUFRAME);
			}
			else if( pstPDUInst->iDownUnit < (int)(iSize+sizeof(PDUFRAME)) )
			{
                            printf("Warning:PDU MaxSize Exceeds Downsize,MaxSize = %d,Downsize = %d \n",(iSize+sizeof(PDUFRAME)),pstPDUInst->iDownUnit);
			}
		}
		else
		{
                    printf("Recv Meta Name For AuthParser Incorrect\n");
		      return -1;
		}
	}
	else
	{
           printf("TDR Name For AuthParser Incorrect\n");
	    return -1;
	}
       pstPDUInst->iUnit = ( pstPDUInst->iDownUnit > pstPDUInst->iUpUnit )?( pstPDUInst->iDownUnit ):( pstPDUInst->iUpUnit );
	pstPDUInst->iUnit	=	(pstPDUInst->iUnit + 0x400 - 1)/0x400*0x400;
	pstPDUInst->iUpUnit	=	(pstPDUInst->iUpUnit + 0x400 - 1)/0x400*0x400;

	pstAuthParserInst->pstMetaHead	=	tdr_get_meta_by_name(pstPrefer, "PDUHead");
	if (NULL == pstAuthParserInst->pstMetaHead)
	{
		printf("failed to get meta by name %s", "PDUHead");
		return -1;
	}
	pstAuthParserInst->pstMetaGameSig	=	tdr_get_meta_by_name(pstPrefer, "SigInfo");
	if (NULL == pstAuthParserInst->pstMetaGameSig)
	{
		printf("failed to get meta by name %s", "SigInfo");
		return -1;
	}
	
	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstAuthParserInst->pstMetaHead, -1, "Base.HeadLen");
	if (TDR_SUCCESS == iRet)
	{
		pstAuthParserInst->iHeadLenNOff 		= stSizeInfo.iNOff;
		pstAuthParserInst->iHeadLenUnitSize 	= stSizeInfo.iUnitSize;
	}
	else
	{
		return -1;
	}

	iRet = tdr_sizeinfo_to_off_i(&stSizeInfo, pstAuthParserInst->pstMetaHead, -1, "Base.BodyLen");
	if (TDR_SUCCESS == iRet)
	{
		pstAuthParserInst->iBodyLenNOff 		= stSizeInfo.iNOff;
		pstAuthParserInst->iBodyLenUnitSize 	= stSizeInfo.iUnitSize;
	}
	else
	{
		return -1;
	}


	pstPDUInst->pfnGetPkgLen = 	tconnd_get_auth_pkglen;

	return 0;
}


int tconnd_init_pduinstlist(TDRINSTLIST* pstTDRInstList, PDUINSTLIST* pstPDUInstList, PDULIST* pstPDUList)
{
	int iRet = 0;

	pstPDUInstList->iCount	=	0;
	while( pstPDUInstList->iCount<pstPDUList->Count)
	{
		PDUINST* pstPDUInst;
		PDU* pstPDU;

		if( pstPDUInstList->iCount >= (int) (sizeof(pstPDUInstList->astInsts)/sizeof(pstPDUInstList->astInsts[0]) )  )
		{
			iRet = -1;
			break;
		}
		
		pstPDU = pstPDUList->PDUs + pstPDUInstList->iCount;
		pstPDUInst = pstPDUInstList->astInsts + pstPDUInstList->iCount;
		memset(pstPDUInst, 0, sizeof(PDUINST));
		STRNCPY(pstPDUInst->szName, pstPDU->Name, sizeof(pstPDUInst->szName));

		pstPDUInst->pstPDU	=	pstPDU;

		pstPDUInst->iLenParsertype = pstPDU->LenParsertype;
		switch(pstPDUInst->iLenParsertype)
		{
		case PDULENPARSERID_BY_TDR:
			iRet = tconnd_init_pdulen_tdrparser(pstTDRInstList, pstPDUInst, pstPDU);
			break;
		case PDULENPARSERID_BY_NULL:
			iRet = tconnd_init_pdulen_thttpparser(pstPDUInst, pstPDU);
			break;
		case PDULENPARSERID_BY_NONE:
			iRet = tconnd_init_pdulen_noneparser(pstPDUInst, pstPDU);
			break;
		case PDULENPARSERID_BY_AUTH:
			iRet = tconnd_init_pdulen_authparser(pstTDRInstList, pstPDUInst, pstPDU);
			break;
		default:
			iRet = -1;
			break;
		}/*switch(pstPDUInst->iLenParsertype)*/

		if(0 != iRet)
		{
			break;
		}
		pstPDUInstList->iCount++;
	}/*while( pstPDUInstList->iCount<pstPDUList->iCount)*/

	return iRet;
}

int tconnd_init_lisinstlist(LISINSTLIST* pstLisInstList, LISTENERLIST* pstListenerList)
{
	LISINST* pstLisInst;
	LISTENER* pstListener;

	pstLisInstList->iCount	=	0;

	while( pstLisInstList->iCount < pstListenerList->Count )
	{
		pstLisInst	=	pstLisInstList->astInsts + pstLisInstList->iCount;
		pstListener	=	pstListenerList->Listeners + pstLisInstList->iCount;

		memcpy(pstLisInst->szName, pstListener->Name, sizeof(pstLisInst->szName));

		pstLisInst->iRef	=	0;

		pstLisInst->pstListener	=	pstListener;

		pstLisInstList->iCount++;
	}

	return 0;
}

int tconnd_fini_lisinstlist(LISINSTLIST* pstLisInstList)
{
	LISINST* pstLisInst;
	int i;

	for(i=0; i<pstLisInstList->iCount; i++)
	{
		pstLisInst	=	pstLisInstList->astInsts + i;
		pstLisInst->iRef	=	0;
	}

	return 0;
}

int tconnd_init_serinstlist(SERINSTLIST* pstSerInstList, SERIALIZERLIST* pstSerializerList)
{
	SERINST* pstSerInst;
	SERIALIZER* pstSerializer;

	pstSerInstList->iCount	=	0;

	while( pstSerInstList->iCount < pstSerializerList->Count )
	{
		pstSerInst	=	pstSerInstList->astInsts + pstSerInstList->iCount;
		pstSerializer=	pstSerializerList->Serializers + pstSerInstList->iCount;

		tbus_addr_aton(pstSerializer->Url, (TBUSADDR * )&pstSerInst->iDst);
		
		memcpy(pstSerInst->szName, pstSerializer->Name, sizeof(pstSerInst->szName));

		pstSerInst->iRef	=	0;
		pstSerInst->pstSerializer=	pstSerializer;

		pstSerInstList->iCount++;
	}

	return 0;
}

int tconnd_fini_serinstlist(SERINSTLIST* pstSerInstList)
{
	SERINST* pstSerInst;
	int i;

	for(i=0; i<pstSerInstList->iCount; i++)
	{
		pstSerInst	=	pstSerInstList->astInsts + i;

		pstSerInst->iDst	=	0;

		pstSerInst->iRef	=	0;
	}

	return 0;
}
int tconnd_init_transinstlist(TRANSINSTLIST* pstTransInstList, ZONE_CONNCONF* pstConnd, LISINSTLIST* pstLisInstList, SERINSTLIST* pstSerInstList)
{
	NETTRANSLIST* pstNetTransList;
	NETTRANS* pstNetTrans;
	TRANSINST* pstTransInst;
	LISTENERLIST* pstListenerList;
	SERIALIZERLIST* pstSerializerList;
	TDRLIST* pstTDRList;
	PDULIST* pstPDUList;
	int iIsFind;
	int i;
	int j;

	pstNetTransList		=	&pstConnd->NetTransList;
	pstTDRList			=	&pstConnd->TDRList;
	pstPDUList			=	&pstConnd->PDUList;
	pstListenerList		=	&pstConnd->ListenerList;
	pstSerializerList	=	&pstConnd->SerializerList;

	if( pstNetTransList->Count>(int)(sizeof(pstTransInstList->astInsts)/sizeof(pstTransInstList->astInsts[0])) )
	{
		return -1;
	}

	/* first, check the tdrs. */
	for(i=0; i<pstNetTransList->Count; i++)
	{
		pstNetTrans	=	pstNetTransList->NetTrans + i;
		pstTransInst=	pstTransInstList->astInsts + i;

		pstTransInst->iWaitQueueHead	=	-1;
		pstTransInst->iWaitQueueTail	=	-1;
		//pstTransInst->uiTokenAlloc	=	0;
		//pstTransInst->uiTokenPass	=	0;

		pstTransInst->iRecvCheckInterval=	TCONND_DEF_RECVCHECK_INTERVAL;


		pstTransInst->iPkgMaxSpeed	=	pstNetTrans->TransLimit.PkgSpeed;;
		pstTransInst->iPkgPermit	=	pstTransInst->iPkgMaxSpeed*pstTransInst->iRecvCheckInterval;
		pstTransInst->iPkgPermitLow	=	pstTransInst->iPkgPermit/2;
		pstTransInst->iPkgPermitHigh	=	pstTransInst->iPkgPermit;

		pstTransInst->iByteMaxSpeed	=	pstNetTrans->TransLimit.ByteSpeed;
		pstTransInst->iBytePermit	=	pstTransInst->iByteMaxSpeed*pstTransInst->iRecvCheckInterval;
		pstTransInst->iBytePermitLow	=	pstTransInst->iBytePermit/2;
		pstTransInst->iBytePermitHigh	=	pstTransInst->iBytePermit;

		iIsFind		=	0;

		for(j=0; j<pstPDUList->Count; j++)
		{
			if( !strcasecmp(pstNetTrans->PDU, pstPDUList->PDUs[j].Name) )
			{
				pstTransInst->iPDULoc	=	j;
				iIsFind	=	1;
				break;
			}
		}

		if( !iIsFind )
		{
			printf("Error: Find PDU \'%s\' failed.\n", pstNetTrans->PDU);
			return -1;
		}

		iIsFind	=	0;

		pstTransInst->iLisCount	=	0;

		for(j=0; j<pstListenerList->Count; j++)
		{
			if( !strcasecmp(pstNetTrans->Listener, pstListenerList->Listeners[j].Name) )
			{
				iIsFind	=	1;

				if( pstTransInst->iLisCount>=(int)(sizeof(pstTransInst->aiLisLoc)/sizeof(pstTransInst->aiLisLoc[0])) )
					break;

				if( pstLisInstList->astInsts[j].iRef>=1 )
				{
					printf("Error: Listener \'%s\' be used more than 1 times.\n", pstNetTrans->Listener);
					return -1;
				}

				pstLisInstList->astInsts[j].iRef++;

				pstTransInst->aiLisLoc[pstTransInst->iLisCount]   =	j;
				pstTransInst->iLisCount++;
			}
		}

		if( !iIsFind )
		{
			printf("Error: Find Listener \'%s\' failed.\n", pstNetTrans->Listener);
			return -1;
		}


		iIsFind	=	0;

		pstTransInst->iSerCount	=	0;

		for(j=0; j<pstSerializerList->Count; j++)
		{
			if( !strcasecmp(pstNetTrans->Serializer, pstSerializerList->Serializers[j].Name) )
			{
				iIsFind	=	1;

				if( pstTransInst->iSerCount>=(int)(sizeof(pstTransInst->aiSerLoc)/sizeof(pstTransInst->aiSerLoc[0])) )
				{
					break;
				}

				pstSerInstList->astInsts[j].iRef++;

				pstTransInst->aiSerLoc[pstTransInst->iSerCount]		=	j;
				pstTransInst->iSerCount++;
			}
		}

		if( !iIsFind )
		{
			printf("Error: Find Serializer \'%s\' failed.\n", pstNetTrans->Serializer);
			return -1;
		}

		
	}

	pstTransInstList->iCount	=	pstNetTransList->Count;

	return 0;

}

int tconnd_fini_confinst(CONFINST* pstConfInst)
{
	assert(pstConfInst);

	return 0;
}

int tconnd_init_confinst(CONFINST* pstConfInst, ZONE_CONNCONF* pstConnd)
{
	if( tconnd_init_tdrinstlist(&pstConfInst->stTDRInstList, &pstConnd->TDRList)<0 )
	{
        printf("tconnd_init_tdrinstlist failed\n");
		return -1;
	}
	if( tconnd_init_pduinstlist(&pstConfInst->stTDRInstList, &pstConfInst->stPDUInstList, &pstConnd->PDUList)<0 )
	{
        printf("tconnd_init_pduinstlist failed\n");
		return -1;
	}
	if( tconnd_init_lisinstlist(&pstConfInst->stLisInstList, &pstConnd->ListenerList)<0 )
	{
        printf("tconnd_init_lisinstlist failed\n");
		return -1;
	}
	if( tconnd_init_serinstlist(&pstConfInst->stSerInstList, &pstConnd->SerializerList)<0 )
	{     
	       printf("tconnd_init_serinstlist failed\n");
		return -1;
	} 
	if( tconnd_init_transinstlist(&pstConfInst->stTransInstList, pstConnd, &pstConfInst->stLisInstList, &pstConfInst->stSerInstList)<0 )
	{
        printf("tconnd_init_transinstlist failed\n");  
		return -1;
	}

	return 0;
}

int tconnd_init_tconndrun(ZONE_CONNRUN_STATUS   *pstRun, ZONE_CONNCONF* pstConnd)
{
	int i = 0;

	memset(pstRun, 0, sizeof(*pstRun));

	for(i=0; i<pstConnd->ListenerList.Count;i++)
	{
		strcpy(pstRun->ListenerRunList.Listeners[i].Name, pstConnd->ListenerList.Listeners[i].Name);
	}

	for(i=0; i<pstConnd->SerializerList.Count;i++)
	{
		strcpy(pstRun->SerializerRunList.Serializers[i].Name, pstConnd->SerializerList.Serializers[i].Name);
	}

	for(i=0; i<pstConnd->NetTransList.Count;i++)
	{
		strcpy(pstRun->NetTransRunList.NetTrans[i].Name, pstConnd->NetTransList.NetTrans[i].Name);
	}

	pstRun->NetTransRunList.Count	=	pstConnd->NetTransList.Count;
	pstRun->ListenerRunList.Count	=	pstConnd->ListenerList.Count;
	pstRun->SerializerRunList.Count	=	pstConnd->SerializerList.Count;

	return 0;
}
