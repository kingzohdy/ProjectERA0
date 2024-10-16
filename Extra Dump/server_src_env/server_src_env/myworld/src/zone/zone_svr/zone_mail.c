/*
**  @file $RCSfile: zone_mail.c,v $
**  general description of this module
**  $Id: zone_mail.c,v 1.153 2014/05/02 03:59:57 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/05/02 03:59:57 $
**  @version $Revision: 1.153 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "libdirty.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_db.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_package.h"
#include "zone_mail.h"
#include "zone_oplog.h"
#include "zone_relation.h"
#include "comm_misc.h"
#include "zone_player.h"
#include "zone_safelock.h"
#include "zone_spansvr.h"
#include "zone_span.h"
#include "zone_err.h"

#define MAIL_ACTIVE_TIMER 10000
#define MAIL_WRITEBACK_TIMEOUT 2000
#define MAIL_POSTAGE_PERCENT 1

#define MAIL_MIN_POSTAGE 1U

//当系统邮件超过了限定数量 且 邮件内存池超过了这个百分比，就不允许再发邮件了。
#define MAIL_POOL_SOFT_LIMITED_PERCENT 85

#define MAILBOX_MAX_WASHING_PERCENT 85 // 超过这个百分比就开始回收邮箱
#define MAILBOX_MAX_WASHING_NUM \
	((MAIL_MAX_MAILBOX * MAILBOX_MAX_WASHING_PERCENT) / 100)

#define MAILBOX_MIN_WASHING_PERCENT 55 // 一直回收到这个百分比
#define MAILBOX_MIN_WASHING_NUM \
	((MAIL_MAX_MAILBOX * MAILBOX_MIN_WASHING_PERCENT) / 100)

#define MAIL_MAX_EXIST_TIME 60 * 24 * 3600 // 邮件的最大存活时间

// 待发邮件的最大存活时间,这个存活时间要设置得稍长，不能太短，否则可能会使
// ZoneMailSend和ZoneMail两个结构体混淆!
#define MAIL_MAX_SEND_EXIST_TIME 16


// 非邮寄物品邮件需要消耗的绑定银币
#define MAIL_FIXED_POSTAGE_BULL 10

int g_iZoneBus = 0;

char szWatchMail1[MAIL_MAX_NUM - (MAIL_MAX_SYSTEM_NUM + MAIL_MAX_NORMAL_NUM)];

// 待发邮件数据
// 1.为了减小复杂度,这个结构体从pstEnv->pstMailPool中分配;
// 2.为了避免过多占用ZoneMail的内存，应该让本结构体存活时间尽量小;
// 这个结构体要从pstMailPool中分配,所以他的大小必须<=ZoneMail

typedef struct tagNormalMailSendData
{
	char szSend[ROLE_NAME_LEN]; // 发件人
	CSMAILCLTSEND stSendMail; // 待发邮件信息
}NormalMailSendData;

typedef union tagZoneMailSendData
{
	NormalMailSendData stNormalMail;
	ZONECTLSENDMAILREQ stSystemMail;
}ZoneMailSendData;

typedef struct tagZoneMailSend
{
	ListEntry stEntry;
	//int fSystemMail;
	int iFlags;
	int iEndTime;
	ZoneMailSendData stData;
	char sRes[ZONE_MAIL_DATA - sizeof(ListEntry) - sizeof(int)*2 - sizeof(ZoneMailSendData)];
}ZoneMailSend;


typedef struct tagSelActive
{
	int iActive;
	int iIdx;
}SelActive;


// 哈希表用的两个辅助函数
static int MailHashCmp(const void *pv1 , const void *pv2);
static unsigned int MailHashCode(const void *pvData);

// 更新哈希表中的邮箱索引
// 在增加/减少邮箱时需要更新部分邮箱索引
static int mail_mailbox_reidx_hash(ZONESVRENV *pstEnv, size_t nBegin);

// 新建一个邮箱
static Mailbox *mail_new_mailbox(ZONESVRENV *pstEnv, const char *pszOwner);

// 从内存中销毁邮箱
static int mail_destroy_mailbox(ZONESVRENV *pstEnv, Mailbox *pstInbox);

// 邮箱的所有邮件是否all clean
// 邮箱邮件all clean返回非0,否则返回0
static int mail_is_mailbox_allclean(ZONESVRENV *pstEnv, const Mailbox *pstBox);

// 清洗长期无用的邮箱
static int mail_wash_mailbox(ZONESVRENV *pstEnv);

// 提交待删除的邮件
static void mail_delete_commit(ZONESVRENV *pstEnv);

// 从内存池中分配一封邮件
static ZoneMail * mail_alloc(ZONESVRENV *pstEnv);

// 将一封邮件压入邮箱之中
static int mail_push_inbox(ZONESVRENV *pstEnv, Mailbox *pstInbox,
                           ZoneMail *pstMail);
	
// 新建一封邮件
static ZoneMail *mail_new_in(ZONESVRENV *pstEnv, int iFlags, const char *pszTo,
                             const char *pszSend, const char *pszTitle);
	
// 新建一封待发送邮件并插入发送列表
// 成功返回待发邮件的cookie(pool idx)，失败返回-1
static ZoneMailSend *mail_new_send_list_entry(ZONESVRENV *pstEnv, int fSystem);

static int mail_push_send_list_entry(ZONESVRENV *pstEnv,
                                     ZoneMailSend *pstEntry);

static int mail_system_send_list_add(ZONESVRENV *pstEnv, 
                                     const ZONECTLSENDMAILREQ *pstReq,
                                     int iFlags);

static int mail_normal_send_list_add(ZONESVRENV *pstEnv, const char *pszSend,
	                                 const CSMAILCLTSEND *pstMailSend);
	
// 从发送列表中删除一封待发送邮件，并设置下一封邮件的指针
// 如果删除失败返回-1，否则返回0。
static int mail_send_list_del(ZONESVRENV *pstEnv, int iMailSendIdx);

// 请求删除一封邮件
static int mail_delete(ZONESVRENV *pstEnv, Mailbox *pstBox,
                       ZoneMail *pstZoneMail);
	
// 查找角色拥有的邮箱
static Mailbox *mail_find_mailbox(ZONESVRENV *pstEnv, const char *pszOwner);

// 初始化torm头
static void mail_init_torm_head(ZONESVRENV *pstEnv, const ZoneMail *pstZoneMail,
	                            TORMPKG *pstPkg, int iCmd, int iTormReqCode,
	                            int fAsyn );
	
// 请求更新MAIL数据
static int mail_update_req(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail);

// 向数据库发送删除一封邮件的请求
static int mail_delete_req(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail);

// 查询角色名字count，用以验证某个名字是否存在。
static int mail_role_count_req(ZONESVRENV *pstEnv, const char *pszOwner,
                               int iCookie);
	
// 查询某角色的所有邮件
static int mail_select_req(ZONESVRENV *pstEnv, const char *pszOwner,
                           int iCookie);

// 处理一个邮件updating tick
static int mail_update_tick(ZONESVRENV *pstEnv, Mailbox *pstBox, 
                            ZoneMail *pstZoneMail, int *piDeleted);

// 处理一个邮件deleting tick
static int mail_delete_tick(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail);

// 处理一个邮箱tick
// 在内部执行邮件扫描循环
static void mail_mailbox_tick(ZONESVRENV *pstEnv, Mailbox *pstInbox);

// 冲洗发送列表
static void mail_wash_send_list(ZONESVRENV * pstEnv);

// 全信箱查找邮件
static ZoneMail *mail_find(ZONESVRENV *pstEnv, Mailbox *pstInbox, 
                           tdr_ulonglong WID);

// 检查玩家身上的钱能否邮寄
static int mail_money_post_test(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                            const CSMAILCLTSEND *pstMailSend);

// 测试物品的可投递性
static int mail_item_postable_test(ZONESVRENV *pstEnv, Player *pstPlayer,
	                               const ROLEITEM *pstItem);
	
// 检查背包里的物品是否能邮寄
static int mail_items_post_test(ZONESVRENV *pstEnv, Player *pstPlayer,
	                            const CSMAILCLTSEND *pstMailSend);

// 邮寄金钱
static int mail_money_post(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail,
	                       Player *pstPlayer, const CSMAILCLTSEND *pstMailSend);
	
// 邮寄物品
static int mail_items_post(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail,
	                       Player *pstPlayer, const CSMAILCLTSEND *pstMailSend);

// 投递到指定的信箱测试
// 并生成context，用于投递到信箱的优化
static int mail_post_inbox_test(ZONESVRENV *pstEnv, Mailbox *pstInbox,
                                int iFlags);
	
// 投递到指定的信箱
static int mail_post_inbox(ZONESVRENV *pstEnv, Mailbox *pstInbox, 
                           ZoneMail *pstZoneMail);
	
// 通知玩家邮件发送失败
static int mail_operating_failed(ZONESVRENV *pstEnv, const char *pszRoleName, 
	                             Player *pstPlayer, int iSysType,
	                             int iErrno, ...);
	
// 接收到select返回结果后的回调函数,处理一些需要在收到select返回结果以后处理
// 的事情!
static int mail_select_callback(ZONESVRENV *pstEnv, Mailbox *pstInbox,
	                            int iMailSendIdx);

// 发送系统邮件
static int mail_sys_post_inbox(ZONESVRENV *pstEnv,
                               const ZONECTLSENDMAILREQ *pstReq,
                               Mailbox *pstInbox,
                               int iFlags);
	
// 接收到select count返回结果后的回调函数,处理一些需要在收到select count返回结果
// 以后处理的事情 : 发送select请求
static int mail_role_count_callback(ZONESVRENV *pstEnv, const char *pszTo, 
	                                int iMailSendIdx);
	
// 从数据库中获取邮件元数据
static int mail_fetch_meta(ZONESVRENV *pstEnv, int iResultNum, Mailbox *pstBox);

// 过滤收件箱中的无效邮件
static int mail_filter_inbox(ZONESVRENV *pstEnv, Mailbox *pstInbox);

// 通知有新邮件
static int mail_new_notify(ZONESVRENV *pstEnv, const Mailbox *pstInbox,
                           const MAILHEAD *pstHead, int iFlag);

// 返回邮件发送结果
static int mail_send_res(ZONESVRENV *pstEnv, const char *pszRoleName, 
                         Player *pstPlayer, int fSucceeded);
	
// 测试某玩家是否能发送某邮件
static int player_mail_send_test(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                             CSMAILCLTSEND *pstSendMail);
	
// 投递邮件到收件箱
// 在调用本函数之前一定要先调用player_mail_can_send，测试玩家是否能发送邮件。
static int player_mail_post_inbox(ZONESVRENV *pstEnv, Player *pstPlayer,
	                              CSMAILCLTSEND *pstSendMail,
	                              Mailbox *pstInbox);

// 测试是否能按清单中的内容获取物品
int player_mail_get_test(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const ZoneMail *pstZoneMail, int iHow,
                         tdr_ulonglong ullItemWID);

// 收取邮件中的物品和钱
static int player_mail_do_get(ZONESVRENV *pstEnv, Player *pstPlayer,
                              ZoneMail *pstZoneMail, int iHow,
                              tdr_ulonglong ullItemWID,
                              CSMAILSVRGOODSCHG *pstChgInfo);

// 删除全部邮件
static int player_mail_all_delete(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  int *piDeletedCount);

// 收取邮件中的物品和钱
static int player_mail_get(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                       const CSMAILGETTINGLIST *pstGettingList,
	                       int *piDeletedCount);

// 收取所有邮件中的所有物品和钱
static int player_mail_all_get(ZONESVRENV *pstEnv, Player *pstPlayer,
                               int *piDeletedCount);

// 发送角色剩余的邮件头
static int player_mail_remain(ZONESVRENV *pstEnv, Player *pstPlayer,
                              int iCount);

// 修改邮件数据标准过程
static inline MAIL *mail_begin_write(ZoneMail *pstZoneMail)
{
	if(!pstZoneMail)
	{
		return NULL;
	}
	
	return &pstZoneMail->stMail;
}

static inline void mail_commit_write(ZoneMail *pstZoneMail)
{
	pstZoneMail->stInfo.iVersion++;
	pstZoneMail->stInfo.iDirty = CACHE_FLAG_DIRTY;
}

// 如果只MAIL_BEGIN_WRITE而不MAIL_COMMIT_WRITE,编译器会报错
#define MAIL_BEGIN_WRITE(zone_mail, data) \
	if(((data) = mail_begin_write(zone_mail)) != NULL) {

#define MAIL_COMMIT_WRITE(zone_mail) \
	mail_commit_write(zone_mail); }

// 检查邮件是否为"空"
static inline int mail_empty(ZoneMail *pstZoneMail)
{
	return pstZoneMail->stMail.Detail.ItemNum == 0 &&
	       pstZoneMail->stMail.Detail.Gold == 0;
}

// 检查邮件是否需要"包裹"
static inline int mail_need_packed(const CSMAILCLTSEND *pstMailSend)
{
	return pstMailSend->Cost.WID != 0LL;
}

static inline int ss_mail_send_sys_res(ZONESVRENV *pstEnv, 
	                                   const ZONECONNCTX *pstExtra,
	                                   int fSucceeded)
{
	SSPKG stSSPkg;
	ZONECTLSENDMAILRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.CtlSendMailRes;

	pstRes->Core.MailID = 0ULL;
	pstRes->Core.Succeeded = fSucceeded;
	pstRes->Extra = *pstExtra;

	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_SEND_MAIL_RES;
	return ss_sendto_cs_svr(pstEnv, &stSSPkg);
}

//
// 配置数据
//
int g_iMailMaxExistSecs = MAIL_MAX_EXIST_TIME;
int g_iMailPostingNeedItemID = MAIL_PACKAGE_ID;
int g_iMailboxMaxWashingNum = MAILBOX_MAX_WASHING_NUM;
int g_iMailboxMinWashingNum = MAILBOX_MIN_WASHING_NUM;



// 哈希表用的两个辅助函数
static int MailHashCmp(const void *pv1 , const void *pv2)
{
	const MailboxHash *pstLhs = pv1, *pstRhs = pv2;
	return strcmp(pstLhs->szOwner, pstRhs->szOwner);
}

static unsigned int MailHashCode(const void* pvData)
{
	const MailboxHash *pstMailboxHash = pvData;
	return sht_get_code(pstMailboxHash->szOwner);
}

// 更新哈希表中的邮箱索引
// 在增加/减少邮箱时需要更新部分邮箱索引
static int mail_mailbox_reidx_hash(ZONESVRENV *pstEnv, size_t nBegin)
{
	size_t i;
	MailboxHash stKey, *pstHash;
	Mailbox *pstBox;

	for(i = nBegin; i < pstEnv->pstPostoffice->nMailboxNum; i++)
	{
		pstBox = pstEnv->pstPostoffice->astMailboxs + i;

		stKey.nIdx = i;
		STRNCPY(stKey.szOwner, pstBox->szOwner, sizeof(stKey.szOwner));
		pstHash = sht_find(pstEnv->pstShtMailbox, &stKey,
		                   MailHashCmp, MailHashCode);
		if (!pstHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "mailbox hash error");
			continue;
		}
		pstHash->nIdx = i;
	}
	return 0;
}


// 新建一个邮箱
static Mailbox *mail_new_mailbox(ZONESVRENV *pstEnv, const char *pszOwner)
{
	Mailbox stNewBox;
	MailboxHash stKey;
	MailboxHash *pstHash = NULL;
	size_t nPos;
	
	if (pstEnv->pstPostoffice->nMailboxNum >= MAIL_MAX_MAILBOX)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mailbox full %d", pstEnv->pstPostoffice->nMailboxNum);
		return NULL;
	}

	STRNCPY(stKey.szOwner, pszOwner, sizeof(stKey.szOwner));
	stKey.nIdx = pstEnv->pstPostoffice->nMailboxNum;
	
	pstHash = sht_insert_unique(pstEnv->pstShtMailbox, &stKey, MailHashCmp,
		                        MailHashCode );
	if (!pstHash)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mailbox hash error, hash num %d", 
					pstEnv->pstShtMailbox->iItem);
		return NULL;
	}

	bzero(&stNewBox, sizeof(stNewBox));
	stNewBox.iMailList = dlist_create(pstEnv->pstMailPool);
	if (stNewBox.iMailList < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mail pool not enough memory %d.", pstEnv->pstMailPool->iUsed);
		return NULL;
	}

	stNewBox.iActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	STRNCPY(stNewBox.szOwner, pszOwner, sizeof(stNewBox.szOwner));

	nPos = pstEnv->pstPostoffice->nMailboxNum++;
	pstEnv->pstPostoffice->astMailboxs[nPos] = stNewBox;

	pstHash->nIdx = nPos;
	STRNCPY(pstHash->szOwner, pszOwner, sizeof(pstHash->szOwner));

	return (pstEnv->pstPostoffice->astMailboxs + nPos);
}

// 从内存中销毁邮箱
static int mail_destroy_mailbox(ZONESVRENV *pstEnv, Mailbox *pstBox)
{
	int i;
	MailboxHash stKey;

	/*if (pstBox->iState & MAILBOX_STATE_CONNECTED || 
		!mail_is_mailbox_allclean(pstEnv, pstBox))
	{
		return -1;
	}*/

	stKey.nIdx = (size_t)(pstBox - pstEnv->pstPostoffice->astMailboxs);
	STRNCPY(stKey.szOwner, pstBox->szOwner, sizeof(stKey.szOwner));
	sht_remove(pstEnv->pstShtMailbox, &stKey, MailHashCmp, MailHashCode);

	// 清除邮箱中所有邮件
	i = dlist_head(pstEnv->pstMailPool, pstBox->iMailList);
	while (i != pstBox->iMailList)
	{
		int iNext = 0;
		ListEntry *pstNode = NULL;

		if ((i < 0) ||
			(pstNode = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstBox->szOwner);
			return -1;
		}
		if(!pstNode)
		{
			return -1;
		}
		
		iNext = pstNode->iNext;
		tmempool_free(pstEnv->pstMailPool, i);
		i = iNext;
	}
	tmempool_free(pstEnv->pstMailPool, pstBox->iMailList);

	if (stKey.nIdx != pstEnv->pstPostoffice->nMailboxNum - 1)
	{
		*pstBox 
			= pstEnv->pstPostoffice->astMailboxs[
				pstEnv->pstPostoffice->nMailboxNum - 1];
	}
	pstEnv->pstPostoffice->nMailboxNum--;

	// mail_mailbox_reidx_hash在外面做
	return 0;
}

// 邮箱的所有邮件是否all clean
// 邮箱邮件all clean返回非0,否则返回0
static int mail_is_mailbox_allclean(ZONESVRENV *pstEnv, const Mailbox *pstBox)
{
	int i;

	dlist_foreach(pstEnv->pstMailPool, i, pstBox->iMailList)
	{
		const ZoneMail *pstZoneMail = NULL;

		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstBox->szOwner);
			return 0;
		}

		if (pstZoneMail->stInfo.iDirty != CACHE_FLAG_CLEAN)
		{
			return 0;
		}
	}
	return 1;
}

static int SelActiveCompare(const void *pv1, const void *pv2)
{
	const SelActive *pstSelActive1 = pv1;
	const SelActive *pstSelActive2 = pv2;
	
	return pstSelActive1->iActive -pstSelActive2->iActive;
}

static int SelActiveIdxCompare(const void *pv1, const void *pv2)
{
	const SelActive *pstSelActive1 = pv1;
	const SelActive *pstSelActive2 = pv2;
	
	return pstSelActive1->iIdx -pstSelActive2->iIdx;
}

// 清洗长期无用的邮箱
static int mail_wash_mailbox(ZONESVRENV *pstEnv)
{
	int i;
	Mailbox *pstBox = NULL;
	int iBeforeNum;	
	SelActive astSelActive[MAIL_MAX_MAILBOX];
	int iSelActive = 0;

	if (pstEnv->pstPostoffice->nMailboxNum <= (size_t)g_iMailboxMaxWashingNum)
	{
		return 0;
	}

	iBeforeNum = pstEnv->pstPostoffice->nMailboxNum;

	for(i = 0; i < (int)pstEnv->pstPostoffice->nMailboxNum; i++)
	{
		pstBox = pstEnv->pstPostoffice->astMailboxs + i;

		if ((pstBox->iState & MAILBOX_STATE_CONNECTED) || 
			!mail_is_mailbox_allclean(pstEnv, pstBox))
		{
			continue;
		}

		astSelActive[iSelActive].iActive = pstBox->iActive;
		astSelActive[iSelActive].iIdx = i;
		iSelActive++;	
	}

	if (0 >= iSelActive)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "wash mail fail");
		return 0;
	}

	qsort(astSelActive, iSelActive, sizeof(astSelActive[0]), SelActiveCompare);

	if (iSelActive > (g_iMailboxMaxWashingNum - g_iMailboxMinWashingNum))
	{
		iSelActive = (g_iMailboxMaxWashingNum - g_iMailboxMinWashingNum);
	}
	qsort(astSelActive, iSelActive, sizeof(astSelActive[0]),
	      SelActiveIdxCompare);
	for (i = iSelActive - 1; i >= 0; i--)
	{
		pstBox = pstEnv->pstPostoffice->astMailboxs + astSelActive[i].iIdx;
		if (mail_destroy_mailbox(pstEnv, pstBox) < 0)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "inbox[%s] can not be destroy",
				      pstBox->szOwner);
			return -1;
		}
	}

	mail_mailbox_reidx_hash(pstEnv, astSelActive[0].iIdx);

	tlog_info(pstEnv->pstLogCat, 0, 0, "mail wash before num %d, after num %d",
		        iBeforeNum, pstEnv->pstPostoffice->nMailboxNum);
	return 0;
}


// 提交待删除的邮件
static void mail_delete_commit(ZONESVRENV * pstEnv)
{
	int i;
	PostofficeInfo *pstInfo = &pstEnv->pstPostoffice->stInfo;

	if (pstInfo->iDeleteNum == 0)
	{
		return;
	}

	dlist_foreach(pstEnv->pstMailPool, i, pstInfo->iDeleteList)
	{
		ZoneMail *pstZoneMail = NULL;

		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "deleting mail list error.");
			break;
		}

		mail_delete_tick(pstEnv, pstZoneMail);
	}
}

// 从内存池中分配一封邮件
static ZoneMail *mail_alloc(ZONESVRENV * pstEnv)
{
	int iIdx = tmempool_alloc(pstEnv->pstMailPool);
	ZoneMail *pstRet = NULL;

	if (iIdx < 0)
	{
		return NULL;
	}

	pstRet = tmempool_get(pstEnv->pstMailPool, iIdx);
	bzero(pstRet, sizeof(*pstRet));

	// 初始化
	dlist_init(pstEnv->pstMailPool, iIdx);
	pstRet->stInfo.iVersion = pstEnv->pstAppCtx->stCurr.tv_sec;

	return pstRet;
}

// 将一封邮件压入邮箱之中
static int mail_push_inbox(ZONESVRENV *pstEnv, Mailbox *pstMailbox,
                           ZoneMail *pstMail)
{
	UNUSED(pstEnv);

	if (pstMail->stMail.Detail.Gold > 0 || pstMail->stMail.Detail.ItemNum > 0)
	{
		pstMail->stMail.Head.Flags |= MAIL_FLAG_NONORM;
	}

	if (pstMail->stMail.Head.Flags & MAIL_FLAG_SYSTEM)
	{
		// 系统邮件超过了一定数量 and 邮件内存池占用超过了一定百分比，就不允许
		// 再插入邮件了
		if (pstMailbox->nSystemNum >= MAIL_MAX_SYSTEM_NUM &&
			pstEnv->pstMailPool->iUsed >=
			pstEnv->pstMailPool->iMax * MAIL_POOL_SOFT_LIMITED_PERCENT / 100)
		{
			return -1;
		}
		pstMailbox->nSystemNum++;
	}
	else
	{
		if (pstMailbox->nNormalNum >= MAIL_MAX_NORMAL_NUM)
		{
			return -1;
		}
		pstMailbox->nNormalNum++;
	}

	dlist_push_back(pstEnv->pstMailPool, pstMailbox->iMailList,
		            pstMail->stEntry.iIdx);
	return 0;
}

// 新建一封邮件
static ZoneMail *mail_new_in(ZONESVRENV *pstEnv, int iFlags, const char *pszTo, 
	                         const char *pszSend, const char *pszTitle )
{
	ZoneMail *pstNew = mail_alloc(pstEnv);
	MAIL *pstMail = NULL;

	if (!pstNew)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"not enough memory.");
		return NULL;
	}

	MAIL_BEGIN_WRITE(pstNew, pstMail)
		pstMail->Head.WID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
		pstMail->Head.Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstMail->Head.Flags = iFlags;
		STRNCPY(pstMail->Head.Recv, pszTo, CCH(pstMail->Head.Recv));
		STRNCPY(pstMail->Head.Send, pszSend, CCH(pstMail->Head.Send));
		STRNCPY(pstMail->Head.Title, pszTitle, CCH(pstMail->Head.Title));
	MAIL_COMMIT_WRITE(pstNew)
	
	return pstNew;
}

// 新建一封普通待发送邮件并插入发送列表
// 成功返回待发邮件的cookie(pool idx)，失败返回-1
static ZoneMailSend *mail_new_send_list_entry(ZONESVRENV *pstEnv, int fSystem)
{
	int iIdx = -1;
	ZoneMailSend *pstEntry = NULL;
	
	iIdx = tmempool_alloc(pstEnv->pstMailPool);
	if (iIdx < 0)
	{
		goto out;
	}

	pstEntry = tmempool_get(pstEnv->pstMailPool, iIdx);
	if (!pstEntry)
	{
		goto out;
	}

	pstEntry->iFlags = fSystem ? MAIL_FLAG_SYSTEM : 0;
	pstEntry->iEndTime 
		= pstEnv->pstAppCtx->stCurr.tv_sec + MAIL_MAX_SEND_EXIST_TIME;

	dlist_init(pstEnv->pstMailPool, iIdx);

out:
	return pstEntry;
}

static int mail_push_send_list_entry(ZONESVRENV *pstEnv, ZoneMailSend *pstEntry)
{
	PostofficeInfo *pstInfo = &pstEnv->pstPostoffice->stInfo;

	// Lazy creating and initializing.
	if (pstInfo->iSendNum == 0)
	{
		pstInfo->iSendList = tmempool_alloc(pstEnv->pstMailPool);
		if (pstInfo->iSendList < 0)
		{
			return -1;
		}
		dlist_init(pstEnv->pstMailPool, pstInfo->iSendList);
	}

	dlist_push_back(pstEnv->pstMailPool, pstInfo->iSendList, 
		            pstEntry->stEntry.iIdx);
	pstInfo->iSendNum++;
	return 0;
}

static int mail_system_send_list_add(ZONESVRENV *pstEnv, 
                                     const ZONECTLSENDMAILREQ *pstReq,
                                     int iFlags)
{
	ZoneMailSend *pstZoneMailSend = NULL;
	
	pstZoneMailSend = mail_new_send_list_entry(pstEnv, 1);
	if (!pstZoneMailSend)
	{
		goto error;
	}
	pstZoneMailSend->iFlags = iFlags;
	pstZoneMailSend->stData.stSystemMail = *pstReq;
	
	if (mail_push_send_list_entry(pstEnv, pstZoneMailSend) < 0)
	{
		goto error;
	}
	
	return pstZoneMailSend->stEntry.iIdx;
	
error:
	tlog_error(pstEnv->pstLogCat, 0, 0, "not enough memory.");
	if (pstZoneMailSend)
	{
		tmempool_free(pstEnv->pstMailPool, pstZoneMailSend->stEntry.iIdx);
	}
	return -1;
}

static int mail_normal_send_list_add(ZONESVRENV * pstEnv, const char *pszSend,
	                                 const CSMAILCLTSEND *pstMailSend)
{
	ZoneMailSend *pstZoneMailSend = NULL;

	pstZoneMailSend = mail_new_send_list_entry(pstEnv, 0);
	if (!pstZoneMailSend)
	{
		goto error;
	}

	STRNCPY(pstZoneMailSend->stData.stNormalMail.szSend, pszSend,
	        CCH(pstZoneMailSend->stData.stNormalMail.szSend));
	pstZoneMailSend->stData.stNormalMail.stSendMail = *pstMailSend;
	
	if (mail_push_send_list_entry(pstEnv, pstZoneMailSend) < 0)
	{
		goto error;
	}
	
	return pstZoneMailSend->stEntry.iIdx;
	
error:
	tlog_error(pstEnv->pstLogCat, 0, 0,	"not enough memory.");
	if (pstZoneMailSend)
	{
		tmempool_free(pstEnv->pstMailPool, pstZoneMailSend->stEntry.iIdx);
	}
	return -1;
}

// 从发送列表中删除一封待发送邮件，并设置下一封邮件的指针
// 如果删除失败返回-1，否则返回0。
static int mail_send_list_del(ZONESVRENV *pstEnv, int iMailSendIdx)
{
	PostofficeInfo *pstInfo = &pstEnv->pstPostoffice->stInfo;

	if (iMailSendIdx < 0 || !pstInfo->iSendNum)
	{
		return -1;
	}

	dlist_remove(pstEnv->pstMailPool, iMailSendIdx);
	tmempool_free(pstEnv->pstMailPool, iMailSendIdx);

	if (--(pstInfo->iSendNum) == 0)
	{
		tmempool_free(pstEnv->pstMailPool, pstInfo->iSendList);
		pstInfo->iSendList = 0;
	}
	
	return 0;
}

// 请求删除一封邮件
static int mail_delete(ZONESVRENV *pstEnv, Mailbox *pstBox,
                       ZoneMail *pstZoneMail)
{
	PostofficeInfo *pstInfo = &pstEnv->pstPostoffice->stInfo;

	// 从邮箱中移出被删除的邮件
	pstZoneMail->stInfo.iDirty = CACHE_FLAG_INEXIST;
	if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_SYSTEM)
	{
		pstBox->nSystemNum--;
	}
	else
	{
		pstBox->nNormalNum--;
	}

	// 把所有需要删除的邮件都放到Deleting List中,统一删除
	// Lazy initializing
	if (pstInfo->iDeleteNum == 0)
	{
		pstInfo->iDeleteList = tmempool_alloc(pstEnv->pstMailPool);
		if (pstInfo->iDeleteList < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"not enough memory.");
			return -1;
		}
		dlist_init(pstEnv->pstMailPool, pstInfo->iDeleteList);
	}

	dlist_remove(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);
	dlist_push_back(pstEnv->pstMailPool, pstInfo->iDeleteList, 
		            pstZoneMail->stEntry.iIdx);
	pstInfo->iDeleteNum++;
	return 0;
}

// 查找角色拥有的邮箱
static Mailbox *mail_find_mailbox(ZONESVRENV *pstEnv, const char *pszOwner)
{
	MailboxHash stKey;
	MailboxHash *pstHash = NULL;

	STRNCPY(stKey.szOwner, pszOwner, sizeof(stKey.szOwner));
	pstHash = sht_find(pstEnv->pstShtMailbox, &stKey, MailHashCmp,
	                   MailHashCode);

	if (!pstHash)
	{
		return NULL;
	}

	if (pstHash->nIdx < pstEnv->pstPostoffice->nMailboxNum)
	{
		return pstEnv->pstPostoffice->astMailboxs + pstHash->nIdx;
	}
	
	return NULL;
}

static void mail_init_torm_head(ZONESVRENV *pstEnv, const ZoneMail *pstZoneMail,
	                            TORMPKG *pstPkg, int iCmd, int iTormReqCode,
	                            int fAsyn)
{
	if (fAsyn)
	{
		DBAsyn stDBAsyn;

		stDBAsyn.iCmd = iCmd;
		stDBAsyn.AsynCmd.stActiveMailboxInfo.iMailIdx 
			= pstZoneMail->stEntry.iIdx;
		stDBAsyn.AsynCmd.stActiveMailboxInfo.iMailVersion 
			= pstZoneMail->stInfo.iVersion;
		STRNCPY(stDBAsyn.AsynCmd.stActiveMailboxInfo.szOwner,
			      pstZoneMail->stMail.Head.Recv,
			      sizeof(stDBAsyn.AsynCmd.stActiveMailboxInfo.szOwner));
		
		z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, iTormReqCode,
			               "MailEntry", -1, &stDBAsyn);
	}
	else
	{
		z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, iTormReqCode,
			               "MailEntry", -1, NULL);
	}
}

// 请求更新MAIL数据
static int mail_update_req(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	if (pstZoneMail->stInfo.iWriteBackTm + MAIL_WRITEBACK_TIMEOUT >=
		pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		// 还没超时,不要轻举妄动.
		return -1;
	}

	mail_init_torm_head(pstEnv, pstZoneMail, pstPkg, DB_MAIL_UPDATE, 
		                  TORM_UPDATE_REQ, 1);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneMail->stMail;
	stMetaData.iBuff = sizeof(pstZoneMail->stMail);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
		                        pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstZoneMail->stInfo.iWriteBackTm = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

// 向数据库发送删除一封邮件的请求
static int mail_delete_req(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail)
{
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;

	if (pstZoneMail->stInfo.iWriteBackTm + MAIL_WRITEBACK_TIMEOUT >=
		pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		// 还没超时,不要轻举妄动.
		return -1;
	}

	mail_init_torm_head(pstEnv, pstZoneMail, pstPkg, DB_MAIL_DELETE,
		                  TORM_DELETE_REQ, 1);
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDeleteReq->OperInfo.WhereDef[0] = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&pstZoneMail->stMail;
	stMetaData.iBuff = sizeof(pstZoneMail->stMail);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
		                       pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	pstZoneMail->stInfo.iWriteBackTm = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

// 查询角色名字count，用以验证某个名字是否存在。
static int mail_role_count_req(ZONESVRENV *pstEnv, const char *pszOwner,
	                           int iCookie)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ * pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	NAMEID stNameID;
	DBAsyn stDBAsyn;

	stDBAsyn.iCmd = DB_MAIL_ROLE_COUNT_SELECT;
	stDBAsyn.AsynCmd.stActiveMailboxInfo.iMailIdx = iCookie;
	STRNCPY(stDBAsyn.AsynCmd.stActiveMailboxInfo.szOwner, pszOwner,
		    sizeof(stDBAsyn.AsynCmd.stActiveMailboxInfo.szOwner));
		
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head,
		               TORM_SELECT_COUNT_REQ, "NameID", -1, &stDBAsyn);

	snprintf(pstSelectCountReq->WhereDef, CCH(pstSelectCountReq->WhereDef), 
		       "where BINARY Name = '%s' and NameType != 65", pszOwner);

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);

	bzero(&stNameID, sizeof(stNameID));
	stMetaData.pszBuff = (char *)&stNameID;
	stMetaData.iBuff = sizeof(stNameID);

	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                       pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	return 0;
}

// 查询某角色的所有邮件
static int mail_select_req(ZONESVRENV *pstEnv, const char *pszOwner,
                           int iCookie)
{
	MAIL stMail;
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;

	stDBAsyn.iCmd = DB_MAIL_SELECT;
	stDBAsyn.AsynCmd.stActiveMailboxInfo.iMailIdx = iCookie;
	STRNCPY(stDBAsyn.AsynCmd.stActiveMailboxInfo.szOwner, pszOwner, 
	        CCH(stDBAsyn.AsynCmd.stActiveMailboxInfo.szOwner));

	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, 
		             "MailEntry", -1, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,
		     sizeof(pstSelectReq->OperInfo.WhereDef),
		     "WHERE Recv = '%s' ORDER BY Time", // 按Time升序排序
		     pszOwner);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	bzero(&stMail, sizeof(stMail));

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stMail;
	stMetaData.iBuff = sizeof(stMail);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData,
		                       pstEnv->iMiscDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}

	return 0;
}

// 处理一个邮件updating tick
static int mail_update_tick(ZONESVRENV *pstEnv, Mailbox *pstBox,
                            ZoneMail *pstZoneMail, int *piDeleted)
{
	int iCurr = pstZoneMail->stEntry.iNext;

	switch(pstZoneMail->stInfo.iDirty)
	{
	case CACHE_FLAG_CLEAN: // 超时
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstZoneMail->stMail.Head.Time >
			g_iMailMaxExistSecs)
		{
			if (piDeleted)
			{
				*piDeleted = 1;
			}
			mail_delete(pstEnv, pstBox, pstZoneMail);
			z_rolemail_oplog(pstEnv, NULL, OP_MAIL_SYS_DEL, &pstZoneMail->stMail);
		}
		break;
		
	case CACHE_FLAG_DIRTY:
	case CACHE_FLAG_IMM:	
		// 处理回写超时
		if (pstZoneMail->stInfo.iWriteBackTm != 0 &&
			(pstEnv->pstAppCtx->stCurr.tv_sec -
			pstZoneMail->stInfo.iWriteBackTm > MAIL_WRITEBACK_TIMEOUT))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
				       "mail data write back to db timed out! try again.");
			pstZoneMail->stInfo.iWriteBackTm = 0;
		}
		mail_update_req(pstEnv, pstZoneMail);
		break;

	default:
		tlog_error(pstEnv->pstLogCat, 0, 0, "mail data error!");
		break;
	}

	return iCurr;
}

// 处理一个邮件deleting tick
static int mail_delete_tick(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail)
{
	// 处理回写超时
	if (pstZoneMail->stInfo.iWriteBackTm != 0 &&
		(pstEnv->pstAppCtx->stCurr.tv_sec -
		pstZoneMail->stInfo.iWriteBackTm > MAIL_WRITEBACK_TIMEOUT))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
			       "mail data write back to db timed out! try again.");
		pstZoneMail->stInfo.iWriteBackTm = 0;
	}

	if (pstZoneMail->stInfo.iDirty != CACHE_FLAG_INEXIST)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mail data error!");
		return -1;
	}

	return mail_delete_req(pstEnv, pstZoneMail);
}

// 处理一个邮箱tick
// 在内部执行邮件扫描循环
static void mail_mailbox_tick(ZONESVRENV *pstEnv, Mailbox *pstBox)
{
	int i, fDeleted = 0;
	CSPKG stPkg;
	CSMAILSVRCHG *pstChg = &stPkg.Body.MailSvr.MailSvrData.MailChgData;
	MAILCHGWIDS *pstDelInfo = &stPkg.Body.MailSvr.MailSvrData.MailChgData.ChgInfo.DeletedWIDs;

	if ((pstBox->nNormalNum + pstBox->nSystemNum) == 0)
	{
		return;
	}

	bzero(pstChg, sizeof(*pstChg));
	pstChg->What = MAIL_CHG_DEL;
	
	i = dlist_head(pstEnv->pstMailPool, pstBox->iMailList);
	while(i != pstBox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;

		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstBox->szOwner);
			break;
		}

		fDeleted = 0;
		i = mail_update_tick(pstEnv, pstBox, pstZoneMail, &fDeleted);
		if (fDeleted) // 记录被删除邮件WID
		{
			pstDelInfo->WIDs[pstDelInfo->Num++] = pstZoneMail->stMail.Head.WID;
		}
	}

	// 如果玩家在线,就发送通知
	if (pstDelInfo->Num > 0 && (pstBox->iState & MAILBOX_STATE_CONNECTED))
	{
		Player *pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                  pstBox->szOwner);

		if (pstPlayer) // 这里几乎不可能为NULL
		{
			stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
			Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			player_mail_remain(pstEnv, pstPlayer, pstDelInfo->Num);
		}
	}
}

static void mail_checked_delete_send(ZONESVRENV *pstEnv, ZoneMailSend *pstSend)
{
	Mailbox *pstInbox = NULL;
	ZONECTLSENDMAILREQ *pstReq = NULL;

	// 只检查系统邮件, 因为系统邮件中有确实存在的物品
	if (!(pstSend->iFlags & MAIL_FLAG_SYSTEM))
	{
		goto just_delete;
	}

	pstReq = &pstSend->stData.stSystemMail;
	pstInbox = mail_find_mailbox(pstEnv, pstReq->Core.To);
	if (pstInbox)
	{
		mail_sys_post_inbox(pstEnv, &pstSend->stData.stSystemMail, pstInbox,
		                    pstSend->iFlags);
	}
	else
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "big problem: can not find %s's mailbox; can not send system"
		           " mail! [sender:%s|title:%s|item-num:%d]",
		           pstReq->Core.To, pstReq->Core.Send, pstReq->Core.Title,
		           pstReq->Core.ItemNum);
	}

just_delete:
	dlist_remove(pstEnv->pstMailPool, pstSend->stEntry.iIdx);
	tmempool_free(pstEnv->pstMailPool, pstSend->stEntry.iIdx);
}

// 冲洗发送列表
static void mail_wash_send_list(ZONESVRENV *pstEnv)
{
	int i = -1;
	PostofficeInfo *pstInfo = &pstEnv->pstPostoffice->stInfo;

	if(!pstInfo->iSendNum)
	{
		return;
	}

	if(pstInfo->iSendList < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "SendList %d", pstInfo->iSendList);
		return;
	}

	i = dlist_head(pstEnv->pstMailPool, pstInfo->iSendList);
	while(i != pstInfo->iSendList)
	{
		ZoneMailSend *pstLast = NULL;
		
		if ((i < 0) || (pstLast = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "mail send list error." );
			return;
		}

		if (pstLast->iEndTime >= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			// 第一个没有超时，后面的就都没有超时
			// 因为链表中的结点是按时间从晚到早排列
			break;
		}

		i = pstLast->stEntry.iNext;
		mail_checked_delete_send(pstEnv, pstLast); // 删除本结点
		pstInfo->iSendNum--;
	}

	// Lazy cleanup
	if (!pstInfo->iSendNum)
	{
		tmempool_free(pstEnv->pstMailPool, pstInfo->iSendList);
		pstInfo->iSendList = 0;
	}
}

// 全信箱查找邮件
static ZoneMail *mail_find(ZONESVRENV *pstEnv, Mailbox *pstInbox,
                           tdr_ulonglong WID)
{
	int i;
	ZoneMail *pstRet = NULL;

	dlist_foreach(pstEnv->pstMailPool, i, pstInbox->iMailList)
	{
		ZoneMail * pstTmp = NULL;
		
		if ((i < 0) || (pstTmp = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			break;
		}

		if (pstTmp->stMail.Head.WID == WID)
		{
			pstRet = pstTmp;
			break;
		}			
	}
	
	return pstRet;
}

// 测试玩家身上的钱能否邮寄
static int mail_money_post_test(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                            const CSMAILCLTSEND *pstMailSend)
{
	// 邮寄的金钱是否足够
	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY,
	                          pstMailSend->Gold) < 0)
	{
		mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2,
		                      ZONEERR_MAIL101, MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	return 0;
}

// 测试物品的可投递性
static int mail_item_postable_test(ZONESVRENV *pstEnv, Player *pstPlayer,
	                               const ROLEITEM *pstItem)
{
	ITEMDEF *pstItemDef = z_find_itemdef(pstEnv, pstItem->DefID);
	if (!pstPlayer ||!pstItemDef)
	{
		return -1;
	}

	return package_current_item_check(pstEnv, pstPlayer, pstItemDef,
	                                  (ROLEITEM *)pstItem);
	/*
	int iRet = 0;
	ITEMDEF *pstItemDef = NULL;
	char szMsg[CLT_MSG_LEN];

	pstItemDef = z_find_itemdef(pstEnv, pstItem->DefID);
	if(!pstPlayer ||!pstItemDef)
	{
		iRet = -1;
		goto final;
	}

	if(pstItem->InstFlag & INST_ITEM_BIND)
	{
		snprintf(szMsg, sizeof(szMsg), "物品 %s 已绑定,不能邮寄",
		         pstItemDef->Name);
		goto can_not_post;
	}

	if(pstItemDef->NoDeal || pstItemDef->NoDrop)
	{
		snprintf(szMsg, sizeof(szMsg), "物品 %s 不能邮寄", pstItemDef->Name);
		goto can_not_post;
	}

	if(pstItemDef->ItemType == ITEM_TYPE_TASK)
	{
		snprintf(szMsg, sizeof(szMsg), "物品 %s 是任务物品,不能邮寄",
			     pstItemDef->Name);
		goto can_not_post;
	}
	
	if(pstItemDef ->ExchgLevel > 0)
	{
		snprintf(szMsg, sizeof(szMsg), "物品 %s 是委托物品,不能邮寄",
			     pstItemDef->Name);
		goto can_not_post;
	}

final:
	return iRet;

can_not_post:
	iRet = -1;
	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);
	goto final;
	return 0;
	*/
}

// 检查背包里的物品是否能邮寄
static int mail_items_post_test(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                            const CSMAILCLTSEND *pstMailSend)
{
	size_t i, j;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;

	if(mail_need_packed(pstMailSend) && pstMailSend->ItemNum == 0)
	{
		mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL102);
		return -1;
	}
	
	for(i = 0; i < pstMailSend->ItemNum; i++)
	{
		const MAILPOSTITEM *pstPostItem = pstMailSend->Items + i;
		int iPos = 0, iFoundRet = -1;

		for(j = i + 1; j < pstMailSend->ItemNum; j++)
		{
			if(pstMailSend->Items[j].WID == pstPostItem->WID)
			{
				return -1;
			}
		}

		switch(pstPostItem->ListType)
		{			
		case LIST_TYPE_STUFF:
			{
				const ROLEITEM *pstRoleItem = NULL;
				
				iPos = package_itemgrid_pos(pstPackage->StuffPackage.RoleItems,
					                        pstPackage->StuffPackage.Num,
					                        pstPostItem->GridIdx);
				if (iPos < 0)
				{
					return -1;
				}

				pstRoleItem = pstPackage->StuffPackage.RoleItems + iPos;

				if (pstPostItem->GridNum > 0 &&
					pstRoleItem->GridIdx == pstPostItem->GridIdx &&
					pstRoleItem->WID == pstPostItem->WID &&
					pstRoleItem->Num >= pstPostItem->GridNum)
				{
					iFoundRet = mail_item_postable_test(pstEnv, pstPlayer,
					                                    pstRoleItem);
				}
			}
			break;
			
		case LIST_TYPE_SUNDRIES:
			{
				const ROLEGRID *pstRoleGrid = NULL;

				iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids, 
					                    pstPackage->SundriesPackage.Num,
					                    pstPostItem->GridIdx);
				if (iPos < 0)
				{
					return -1;
				}

				pstRoleGrid = pstPackage->SundriesPackage.RoleGrids + iPos;

				if (pstPostItem->GridNum > 0  &&
					pstRoleGrid->GridData.RoleItem.GridIdx == 
					pstPostItem->GridIdx &&
					pstRoleGrid->GridData.RoleItem.WID == pstPostItem->WID &&
					pstRoleGrid->GridData.RoleItem.Num >= pstPostItem->GridNum)
				{
					if (pstRoleGrid->GridData.RoleItem.GridIdx ==
						pstMailSend->Cost.GridIdx)
					{
						// 邮寄的正好是"包裹"需要特殊检查处理
						if (pstRoleGrid->GridData.RoleItem.DefID != 
							MAIL_PACKAGE_ID &&
							pstRoleGrid->GridData.RoleItem.DefID !=
							MAIL_BINDED_PACKAGE_ID)
						{
							return -1;
						}

						if (pstRoleGrid->GridData.RoleItem.Num <
							pstPostItem->GridNum + 1)
						{
							mail_operating_failed(pstEnv, NULL, pstPlayer,
							                      SYS_MIDDLE2, ZONEERR_MAIL103);
							return -1;
						}
					}

					iFoundRet = mail_item_postable_test(pstEnv, pstPlayer,
					                           &pstRoleGrid->GridData.RoleItem);
				}
			}
			break;

		default:
			return -1;
			break;
		}

		if(iFoundRet < 0)
		{
			return -1;
		}
	}

	// 他妈的，邮寄"邮寄凭证"的时候要特殊处理
	if (pstMailSend->ItemNum > 0 ||pstMailSend->Gold > 0)
	{
		const ROLEGRID *pstRoleGrid = NULL;
		int iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids, 
			                        pstPackage->SundriesPackage.Num,
			                        pstMailSend->Cost.GridIdx);
		if (iPos < 0)
		{
 			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL104);
			return -1;
		}

		if (pstMailSend->Cost.ListType != LIST_TYPE_SUNDRIES)
		{
			return -1;
		}

		pstRoleGrid = pstPackage->SundriesPackage.RoleGrids + iPos;

		if (pstRoleGrid->GridData.RoleItem.DefID != MAIL_PACKAGE_ID &&
			pstRoleGrid->GridData.RoleItem.DefID != MAIL_BINDED_PACKAGE_ID)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2,  ZONEERR_MAIL105);
			return -1;
		}

		if (pstRoleGrid->GridData.RoleItem.Num < 1 ||
			pstRoleGrid->GridData.RoleItem.GridIdx !=
			pstMailSend->Cost.GridIdx ||
			pstRoleGrid->GridData.RoleItem.WID != pstMailSend->Cost.WID)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL106);
			return -1;
		}
	}

	return 0;
}

// 邮寄金钱
static int mail_money_post(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail,
	                       Player *pstPlayer, const CSMAILCLTSEND *pstMailSend)
{
	/*
	if (package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY,
	                      MAIL_FIXED_POSTAGE_BULL) < 0)
	{
		return -1;
	}
	*/

	// 邮寄的金钱
	if (package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY,
	                      pstMailSend->Gold) < 0)
	{
		return -1;
	}

	pstZoneMail->stMail.Detail.Gold = pstMailSend->Gold;
	return 0;
}

// 邮寄物品
// 一定要mail_items_post_test过后再邮寄物品，切记。
static int mail_items_post(ZONESVRENV *pstEnv, ZoneMail *pstZoneMail,
	                       Player *pstPlayer, const CSMAILCLTSEND *pstMailSend)
{
	size_t i;
	ROLEPACKAGE *pstPackage = &pstPlayer->stRoleData.Package;

	pstZoneMail->stMail.Detail.ItemNum = 0; // 安全第一
	for(i = 0; i < pstMailSend->ItemNum; i++)
	{
		int iPos = 0;
		const MAILPOSTITEM *pstPostItem = pstMailSend->Items + i;
		ROLEGRID stDummyGrid;

		switch(pstPostItem->ListType)
		{			
		case LIST_TYPE_STUFF:
			{
				const ROLEITEM *pstRoleItem = NULL;
				
				iPos = package_itemgrid_pos(pstPackage->StuffPackage.RoleItems,
					                        pstPackage->StuffPackage.Num,
					                        pstPostItem->GridIdx );
				if (iPos < 0)
				{
					return -1;
				}

				pstRoleItem = pstPackage->StuffPackage.RoleItems + iPos;

				stDummyGrid.Type = GRID_ITEM_ITEM;
				stDummyGrid.GridData.RoleItem = *pstRoleItem;
				stDummyGrid.GridData.RoleItem.Num = pstPostItem->GridNum;

				if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
				                pstRoleItem->DefID, pstPostItem->GridNum,
				                pstPostItem->ListType, &iPos,PACKAGE_DEC_NONE, 0) < 0)
				{
					return -1;
				}
			}
			break;
			
		case LIST_TYPE_SUNDRIES:
			{
				const ROLEGRID *pstRoleGrid = NULL;

				iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
					                    pstPackage->SundriesPackage.Num,
					                    pstPostItem->GridIdx);
				if(iPos < 0)
				{
					return -1;
				}

				pstRoleGrid = pstPackage->SundriesPackage.RoleGrids + iPos;

				stDummyGrid = *pstRoleGrid;
				stDummyGrid.GridData.RoleItem.Num = pstPostItem->GridNum;

				if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
					            pstRoleGrid->GridData.RoleItem.DefID,
					            pstPostItem->GridNum, pstPostItem->ListType,
				                &iPos,PACKAGE_DEC_NONE, 0) < 0)
				{
					return -1;
				}
			}
			break;

		default:
			return -1;
			break;
		}

		pstZoneMail->stMail.Detail.Items[pstZoneMail->stMail.Detail.ItemNum++]
			= stDummyGrid;
	}

	// 消耗消耗品
	if(pstMailSend->ItemNum > 0)
	{
		const ROLEGRID *pstRoleGrid = NULL;
		int iPos = package_grid_pos(pstPackage->SundriesPackage.RoleGrids,
			                        pstPackage->SundriesPackage.Num,
			                        pstMailSend->Cost.GridIdx);
		if(iPos < 0)
		{
			return -1;
		}

		pstRoleGrid = pstPackage->SundriesPackage.RoleGrids + iPos;
		if (pstRoleGrid->GridData.RoleItem.DefID != MAIL_PACKAGE_ID &&
			pstRoleGrid->GridData.RoleItem.DefID != MAIL_BINDED_PACKAGE_ID)
		{
			return -1;
		}

		if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			            pstRoleGrid->GridData.RoleItem.DefID,
			            1, pstMailSend->Cost.ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_MAIL) < 0)
		{
			return -1;
		}
	}
	
	return 0;
}



// 测试能否投递到邮箱
// 对方邮箱满了就直接返回失败
static int mail_post_inbox_test(ZONESVRENV *pstEnv, Mailbox *pstInbox,
                                int iFlags)
{
	UNUSED(pstEnv);
	
	// 判断是否允许往邮箱里投递邮件
	if (iFlags & MAIL_FLAG_SYSTEM)
	{
		// 系统邮件超过了一定数量 and 邮件内存池占用超过了一定百分比，就不允许
		// 再插入邮件了
		if (pstInbox->nSystemNum >= MAIL_MAX_SYSTEM_NUM &&
			pstEnv->pstMailPool->iUsed >=
			pstEnv->pstMailPool->iMax * MAIL_POOL_SOFT_LIMITED_PERCENT / 100)
		{
			return -1;
		}
	}
	else
	{
		if (pstInbox->nNormalNum >= MAIL_MAX_NORMAL_NUM)
		{
			return -1;
		}
	}

	return 0;
}

// 投递到邮箱
static int mail_post_inbox(ZONESVRENV *pstEnv, Mailbox *pstInbox,
                           ZoneMail *pstZoneMail)
{
	return mail_push_inbox(pstEnv, pstInbox, pstZoneMail);
}

// 通知玩家邮件发送失败
static int mail_operating_failed(ZONESVRENV *pstEnv, const char *pszRoleName, 
	                             Player *pstPlayer, int iSysType,
	                             int iErrno, ...)
{
	CSPKG stPkg;
	CSSYSMSG *pstSysMsg = &stPkg.Body.SysMsg;
	va_list valist;
	const char* pszErr;
	
	if (!pszRoleName && !pstPlayer)
	{
		return -1;
	}

	if (!pstPlayer)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
		if (!pstPlayer)
		{
			return -1; // 玩家不在线,无法通知.
		}
	}

	pstSysMsg->Type = iSysType;
	pstSysMsg->Times = 0;
	pstSysMsg->Interval = 0;
	pstSysMsg->LinkLen = 0;

	va_start(valist, iErrno);
	pszErr	=	ZONE_GET_ERR(iErrno, 0);

	if( !pszErr )
	{
		snprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), "SysNo : %d", iErrno );
	}
	else
	{
		vsnprintf(pstSysMsg->Msg, sizeof(pstSysMsg->Msg), pszErr, valist);
	}
	
	pstSysMsg->Msg[SYS_MSG_LEN -1] = 0;
	va_end(valist);

	Z_CSHEAD_INIT(&stPkg.Head, SYS_MSG);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// 接收到select返回结果后的回调函数,处理一些需要在收到select返回结果以后处理
// 的事情!
static int mail_select_callback(ZONESVRENV *pstEnv, Mailbox *pstInbox,
	                            int iMailSendIdx)
{
	Player *pstSend = NULL, *pstTo = NULL;
	ZoneMailSend *pstMailSend = NULL;
	int iRet = 0;

	// 更新收件箱的状态
	pstTo = z_name_player(pstEnv->pstAppCtx, pstEnv, pstInbox->szOwner);
	if (pstTo)
	{
		if ((iMailSendIdx < 0 &&
			(pstTo->stOnline.cMiscFlag & MISC_FLAG_MAIL_SNAP)))
		{
			player_mail_head_snapshot(pstEnv, pstTo);
		}
		pstInbox->iState |= MAILBOX_STATE_CONNECTED;
	}
	else
	{
		pstInbox->iState &= ~MAILBOX_STATE_CONNECTED;
	}

	// 只是创建角色的邮箱，不需要投递邮件
	if (iMailSendIdx < 0)
	{
		goto final;
	}

	// 投递邮件到收件箱
	pstMailSend = tmempool_get(pstEnv->pstMailPool, iMailSendIdx);
	if (!pstMailSend)
	{
		goto error;
	}

	if (pstMailSend->iFlags)
	{
		iRet = mail_sys_post_inbox(pstEnv, &pstMailSend->stData.stSystemMail, 
			                       pstInbox, pstMailSend->iFlags);
	}
	else
	{
		pstSend = z_name_player(pstEnv->pstAppCtx, pstEnv, 
		                        pstMailSend->stData.stNormalMail.szSend);
		if (!pstSend)
		{
			goto error;
		}
		iRet = player_mail_post_inbox(pstEnv, pstSend, 
		                           &pstMailSend->stData.stNormalMail.stSendMail,
		                           pstInbox);
	}
	
final:
	return iRet;

error:
	iRet = -1;
	goto final;
	return 0;
}

// 发送系统邮件
static int mail_sys_post_inbox(ZONESVRENV *pstEnv, 
                               const ZONECTLSENDMAILREQ *pstCtlReq, 
                               Mailbox *pstInbox,
                               int iFlags)
{
	int i = 0, iRet = 0;
	ZoneMail *pstZoneMail = NULL;
	const ZONESENDMAILREQ *pstReq = &pstCtlReq->Core;
	
	if (mail_post_inbox_test(pstEnv, pstInbox, iFlags) < 0)
	{
		goto error;
	}

	pstZoneMail = mail_new_in(pstEnv, iFlags, pstReq->To, pstReq->Send,
	                          pstReq->Title);
	if (!pstZoneMail)
	{
		goto error;
	}

	STRNCPY(pstZoneMail->stMail.Detail.Text, pstReq->Text, 
		    CCH(pstZoneMail->stMail.Detail.Text));

	pstZoneMail->stMail.Detail.Gold = pstReq->Money;
	pstZoneMail->stMail.Detail.ItemNum = pstReq->ItemNum;
	i = pstReq->ItemNum;
	while(i--)
	{
		pstZoneMail->stMail.Detail.Items[i] = pstReq->Items[i];
		
		// 重新生成当前服务器相关的WID
		pstZoneMail->stMail.Detail.Items[i].GridData.RoleItem.WID = 
			z_get_gid(pstEnv->pstAppCtx, pstEnv);
	}
	
	if (mail_post_inbox(pstEnv, pstInbox, pstZoneMail) < 0)
	{
		goto error;
	}

	//if (pstInbox->nNormalNum + pstInbox->nSystemNum <= MAIL_MAX_NUM)
	{
		mail_new_notify(pstEnv, pstInbox, &pstZoneMail->stMail.Head, 0);
	}
	
	pstInbox->iActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	z_rolemail_oplog(pstEnv, NULL, OP_MAIL_SYS_SEND, &pstZoneMail->stMail);

final:
	ss_mail_send_sys_res(pstEnv, &pstCtlReq->Extra, iRet < 0 ? 0 : 1);
	return iRet;

error:
	iRet = -1;
	if (pstZoneMail)
	{	
		tmempool_free(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);
	}
	
	goto final;
	return 0;
}

// 接收到select count返回结果后的回调函数,处理一些需要在收到select count返回结果
// 以后处理的事情 : 发送select请求
static int mail_role_count_callback(ZONESVRENV *pstEnv, const char *pszTo,
	                                int iMailSendIdx)
{
	if (iMailSendIdx < 0)
	{
		return -1;
	}

	return mail_select_req(pstEnv, pszTo, iMailSendIdx);
}

// 从数据库中获取邮件元数据
static int mail_fetch_meta(ZONESVRENV *pstEnv, int iResultNum, Mailbox *pstBox)
{
	int i;
	
	pstBox->nNormalNum = 0;
	pstBox->nSystemNum = 0;
	for(i = 0; i < iResultNum; i++)
	{
		ZoneMail *pstZoneMail = mail_alloc(pstEnv);

		if (!pstZoneMail)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,	"not enough memory");
			return -1;
		}

		if (0 > torm_fetch_one_meta(pstEnv->hTorm,
		                           (char *)(&(pstZoneMail->stMail)), 
			                       sizeof(pstZoneMail->stMail)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "inbox[%s] db fetch one meta fail.", pstBox->szOwner);
			return -1;
		}

		if (mail_push_inbox(pstEnv, pstBox, pstZoneMail) < 0)
		{
			tmempool_free(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);
			break;
		}
	}

	return 0;
}

// 过滤收件箱中的无效邮件
static int mail_filter_inbox(ZONESVRENV *pstEnv, Mailbox *pstInbox)
{
	int i;
	Player *pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                                 pstInbox->szOwner);

	if (!pstOwner)
	{
		return -1;
	}

	i = dlist_head(pstEnv->pstMailPool, pstInbox->iMailList);
	while(i != pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;

		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			break;
		}

		/*if (player_black_find_byname(&pstOwner->stRoleData.MiscInfo,
				                     pstZoneMail->stMail.Head.Send) >= 0 ||
			player_enemy_find_byname(&pstOwner->stRoleData.MiscInfo,
                                     pstZoneMail->stMail.Head.Send) >= 0)
		{
			i = pstZoneMail->stEntry.iNext;
			mail_delete(pstEnv, pstInbox, pstZoneMail);
			continue;
		}*/

		i = pstZoneMail->stEntry.iNext;
	}

	return (i == pstInbox->iMailList) ? 0 : -1;
}


// 查询角色的邮件异步返回结果
int mail_select_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,
	                const ActiveMailboxInfo *pstInfo)
{
	int iRet = 0;
	Mailbox *pstInbox = NULL;
	Player *pstTo = NULL;
	ZoneMailSend *pstMailSend = NULL;

	if (TORM_ERR_IS_ERROR(pstSelectRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"owner[%s] db select mail fail",
		           pstInfo->szOwner);
		goto error;
	}

	// 如果角色没有邮件就不用再内存中建立邮箱了
	if ((pstInfo->iMailIdx < 0 && pstSelectRes->SelectResult.ResultNum == 0))
	{
		goto final;
	}

	pstInbox = mail_find_mailbox(pstEnv, pstInfo->szOwner);
	if (pstInbox)
	{
		goto just_post; // 假如邮箱已存在,就直接投递了.
	}

	pstInbox = mail_new_mailbox(pstEnv, pstInfo->szOwner);
	if (!pstInbox)
	{
		goto error;
	}

	if (mail_fetch_meta(pstEnv, pstSelectRes->SelectResult.ResultNum, pstInbox)
		 < 0 )
	{
		goto error;
	}

	mail_filter_inbox(pstEnv, pstInbox);

just_post:
	iRet = mail_select_callback(pstEnv, pstInbox, pstInfo->iMailIdx);

final:
	if (!iRet &&
		(pstTo = z_name_player(pstEnv->pstAppCtx, pstEnv,
	                           pstInfo->szOwner)) != NULL)
	{
		pstTo->stOnline.cMiscFlag |= MISC_FLAG_MAIL_FETCHED;
	}
	
	mail_send_list_del(pstEnv, pstInfo->iMailIdx);
	return iRet;

error:
	iRet = -1;
	
	if (pstInfo->iMailIdx < 0)
	{
		goto final;
	}

	pstMailSend = tmempool_get(pstEnv->pstMailPool, pstInfo->iMailIdx);
	if (!pstMailSend)
	{
		goto final;
	}

	if (pstMailSend->iFlags & MAIL_FLAG_SYSTEM)
	{
		ss_mail_send_sys_res(pstEnv, &pstMailSend->stData.stSystemMail.Extra,
		                     0);
	}
	else
	{
		mail_operating_failed(pstEnv, pstInfo->szOwner, NULL, SYS_MIDDLE2, ZONEERR_MAIL107);
		mail_send_res(pstEnv, pstInfo->szOwner, NULL, 0);
	}
	
	goto final;
	return 0;
}

// 获取角色名字select count的异步返回结果
int mail_role_count_res(ZONESVRENV* pstEnv, 
                        TORMSELECTCOUNTRES *pstSelectCountRes,
                        const ActiveMailboxInfo *pstInfo)
{
	int iRet = 0;
	ZoneMailSend *pstMailSend = NULL;
	
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "owner[%s] db select count failed.",
		           pstInfo->szOwner);
		goto error;
	}

	// 查无此人
	if (pstSelectCountRes->RowsCount == 0)
	{
		goto error;
	}

	iRet = mail_role_count_callback(pstEnv, pstInfo->szOwner,
	                                pstInfo->iMailIdx);
	
final:
	return iRet;

error:
	iRet = -1;
	if (pstInfo->iMailIdx < 0)
	{
		goto final;
	}
		
	pstMailSend = tmempool_get(pstEnv->pstMailPool, pstInfo->iMailIdx);
	if (!pstMailSend)
	{
		goto final;
	}

	if (pstMailSend->iFlags & MAIL_FLAG_SYSTEM)
	{
		ss_mail_send_sys_res(pstEnv, &pstMailSend->stData.stSystemMail.Extra,
		                     0);
	}
	else
	{
		mail_operating_failed(pstEnv, pstMailSend->stData.stNormalMail.szSend,
		                      NULL, SYS_MIDDLE2, ZONEERR_MAIL108);
		mail_send_res(pstEnv, pstMailSend->stData.stNormalMail.szSend, NULL, 0);
	}
	mail_send_list_del(pstEnv, pstInfo->iMailIdx);

	goto final;
	return 0;
}

// 更新角色一封邮件异步返回结果
int mail_update_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, 
	                const ActiveMailboxInfo *pstInfo)
{
	TORMUPDATERES *pstUpdateRes = &pstTormPkg->Body.UpdateRes;
	ZoneMail *pstZoneMail = NULL;

	UNUSED(pstAppCtx);

	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "owner[%s] DB update mail failed.",
		           pstInfo->szOwner);
		return -1;
	}

	if (pstInfo->iMailIdx < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mail list error.");
		return -1;
	}

	pstZoneMail = tmempool_get(pstEnv->pstMailPool, pstInfo->iMailIdx);
	if (!pstZoneMail)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mail list error.");
		return -1;
	}

	pstZoneMail->stInfo.iWriteBackTm = 0;

	if (pstZoneMail->stInfo.iVersion <= pstInfo->iMailVersion &&
		pstZoneMail->stInfo.iDirty == CACHE_FLAG_DIRTY)
	{
		pstZoneMail->stInfo.iDirty = CACHE_FLAG_CLEAN;
	}
	return 0;
}

// 删除角色一封邮件异步返回结果
int mail_delete_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, 
	                const ActiveMailboxInfo *pstInfo)
{
	TORMDELETERES *pstDeleteRes = &pstTormPkg->Body.DeleteRes;
	ZoneMail *pstZoneMail;
	int iRet = 0;

	UNUSED(pstAppCtx);

	if (pstInfo->iMailIdx < 0)
	{
		goto error;
	}	

	pstZoneMail = tmempool_get(pstEnv->pstMailPool, pstInfo->iMailIdx);
	if (!pstZoneMail)
	{
		goto error;
	}

	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "owner[%s] db delete mail failed.",
		           pstInfo->szOwner);
		goto final;
	}
	
	pstZoneMail->stInfo.iWriteBackTm = 0;
	dlist_remove(pstEnv->pstMailPool, pstInfo->iMailIdx);
	tmempool_free(pstEnv->pstMailPool, pstInfo->iMailIdx);

	// Lazy deleting.
	if (--pstEnv->pstPostoffice->stInfo.iDeleteNum == 0)
	{
		tmempool_free(pstEnv->pstMailPool,
			          pstEnv->pstPostoffice->stInfo.iDeleteList);
		pstEnv->pstPostoffice->stInfo.iDeleteList = 0;
	}

final:
	return iRet;

error:
	mail_operating_failed(pstEnv, pstInfo->szOwner, NULL, SYS_MIDDLE2, ZONEERR_MAIL109);
	iRet = -1;
	goto final;
	return 0;
}

// 角色登录时，邮件相关操作
int mail_role_login(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	if (!pstInbox)
	{
		mail_select_req(pstEnv, pstPlayer->stRoleData.RoleName, -1);
	}
	else
	{
		mail_filter_inbox(pstEnv, pstInbox);
		
		pstInbox->iState |= MAILBOX_STATE_CONNECTED;
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_MAIL_FETCHED;
	}
	return 0;
}

// 角色登出时，邮件相关操作
int mail_role_logout(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	if (pstInbox)
	{
		pstInbox->iState &= ~MAILBOX_STATE_CONNECTED;
	}
	
	return 0;
}

// 通知有新邮件
static int mail_new_notify(ZONESVRENV* pstEnv, const Mailbox *pstInbox,
                           const MAILHEAD *pstHead, int iFlag)
{
	CSPKG stPkg;
	CSMAILSVRCHG *pstChg = &stPkg.Body.MailSvr.MailSvrData.MailChgData;
	Player *pstTo = NULL;

	// 如果玩家在线,就发送通知
	if (!(pstInbox->iState & MAILBOX_STATE_CONNECTED))
	{
		return -1;
	}

	pstTo = z_name_player(pstEnv->pstAppCtx, pstEnv, pstInbox->szOwner);
	if (!pstTo)
	{
		return -1;
	}

	if ((pstInbox->nNormalNum + pstInbox->nSystemNum <= MAIL_MAX_NUM) || iFlag == 1)
	{
		pstChg->What = MAIL_CHG_ADD;
		pstChg->ChgInfo.AddedHead = *pstHead;

		stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
		Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
		z_cltmsg_send(pstEnv, pstTo, &stPkg);
	}
	else if (iFlag == 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTo, SYS_MIDDLE2, 
			ZONEERR_MAIL1);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTo, SYS_TIP, 
			ZONEERR_MAIL1);
	}
	
	return 0;
}

// 在加入黑名单、仇人列表之后通知一下
int mail_check_clean_notify(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                        const char *pszRoleName)
{
	int i;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	if (!pstInbox)
	{
		return 0;
	}

	dlist_foreach(pstEnv->pstMailPool, i, pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			break;
		}

		if (strcmp(pstZoneMail->stMail.Head.Send, pszRoleName) == 0)
		{
			break;
		}
	}
	if (i == pstInbox->iMailList)
	{
		return 0;
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP,
	           ZONEERR_MAIL8, pszRoleName);
	return 0;
}

// 返回邮件发送结果
static int mail_send_res(ZONESVRENV* pstEnv, const char *pszRoleName, 
                         Player *pstPlayer, int fSucceeded)
{
	CSPKG stPkg;
	CSMAILSVRSENDRES *pstRes = &stPkg.Body.MailSvr.MailSvrData.MailSendRes;

	if (!pszRoleName && !pstPlayer)
	{
		return -1;
	}

	if (!pstPlayer)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszRoleName);
		if (!pstPlayer)
		{
			return -1; // 玩家不在线,无法通知.
		}
	}

	pstRes->Succeeded = fSucceeded;
	
	stPkg.Body.MailSvr.Operation = MAIL_SVR_SEND_RES;
	Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// 定时邮件操作
int z_active_mail(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount)
{
	static struct timeval stRoundTv;
	static struct timeval stRoundStartTv;
	static  int iActivePos = 0, iRound = 0;
	int i;
	struct timeval stSub;
	long long  llMs;

	if (INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		return 0;
	}
		
	if (0 == stRoundTv.tv_sec)
	{
		llMs = MAIL_ACTIVE_TIMER;
	}
	else
	{
		TV_DIFF(stSub, pstAppCtx->stCurr, stRoundTv);
		TV_TO_MS(llMs, stSub);
	}

	if (0 > llMs)
	{
		TV_CLONE(stRoundTv, pstAppCtx->stCurr);
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}
		
	if (llMs < MAIL_ACTIVE_TIMER)
	{
		return 0;
	}

	if (0 == iActivePos)
	{
		TV_CLONE(stRoundStartTv, pstAppCtx->stCurr);
	}

	for(i = 0; i < iCount; i++) 
	{
		if (iActivePos >= (int)pstEnv->pstPostoffice->nMailboxNum) 
		{
			break;
		}
		
		mail_mailbox_tick(pstEnv,
		                  pstEnv->pstPostoffice->astMailboxs + iActivePos);
		iActivePos++;
	}
	
	if (iActivePos < (int)pstEnv->pstPostoffice->nMailboxNum)
	{
		return 0;
	}

	mail_wash_mailbox(pstEnv);
	mail_wash_send_list(pstEnv);
	mail_delete_commit(pstEnv);
	
	iActivePos = 0;
	iRound++;
	TV_CLONE(stRoundTv, pstAppCtx->stCurr);

	TV_DIFF(stSub, pstAppCtx->stCurr, stRoundStartTv);
	TV_TO_MS(llMs, stSub);
	if (llMs > MAIL_ACTIVE_TIMER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "active mail loop %lld ms", llMs);
	}
	
	return 0;
}

// 在服务器关闭前对邮件的最终操作:Finally Operating
int z_db_mail_fin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv)
{
	size_t i;
	Postoffice *pstPostoffice = pstEnv->pstPostoffice;

	UNUSED(pstAppCtx);

	for(i = 0; i < pstPostoffice->nMailboxNum; i++)
	{
		mail_mailbox_tick(pstEnv, pstPostoffice->astMailboxs + i);
	}

	mail_delete_commit(pstEnv);
	return 0;
}

// 全服务器的所有邮件是否all clean
// all clean 返回1，否则返回0
int z_db_mail_is_allclean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv)
{
	size_t i;
	Postoffice *pstPostoffice = pstEnv->pstPostoffice;

	UNUSED(pstAppCtx);

	for(i = 0; i < pstPostoffice->nMailboxNum; i++)
	{
		if (!mail_is_mailbox_allclean(pstEnv, pstPostoffice->astMailboxs + i))
		{
			return 0;
		}
	}

	if (pstPostoffice->stInfo.iDeleteNum > 0)
	{
		return 0;
	}

	return 1;
}

int mail_role_on_delete(ZONESVRENV *pstEnv, const char *pszRoleName)
{
	int i;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv, pszRoleName);
	if (!pstInbox)
	{
		return 0;
	}

	// 先把邮箱中所有的邮件提交删除
	i = dlist_head(pstEnv->pstMailPool, pstInbox->iMailList);
	while(i != pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			return -1;
		}

		i = pstZoneMail->stEntry.iNext;
		if (mail_delete(pstEnv, pstInbox, pstZoneMail) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "can not delete mail: id=%lld",
			           pstZoneMail->stMail.Head.WID);
		}
		z_rolemail_oplog(pstEnv, NULL, OP_MAIL_SYS_DEL, &pstZoneMail->stMail);
	}

	// 再把邮箱删除了, 以绝后患
	if (mail_destroy_mailbox(pstEnv, pstInbox) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "can not delete inbox: owner=%s",
		           pstInbox->szOwner);
		return -1;
	}
	mail_mailbox_reidx_hash(pstEnv,
	                      (int)(pstInbox - pstEnv->pstPostoffice->astMailboxs));
	return 0;
}

// 返回某玩家是否能发送某邮件
// 能发送返回非0,否则返回0
static int player_mail_send_test(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                             CSMAILCLTSEND *pstSendMail)
{
	Player * pstToPlayer = NULL;
	unsigned int i;

	if (!pstSendMail->To[0])
	{
		mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL110);
		goto error;
	}

	if (!pstSendMail->Title[0])
	{
		mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL111);
		goto error;
	}

	// 党国正在正看者你!
	DirtyReplace(pstSendMail->Title);
	DirtyReplace(pstSendMail->Text);

	// 收件人不能为黑名单，仇人，自己，系统
	if (strcmp(pstPlayer->stRoleData.RoleName, pstSendMail->To) == 0)
	{
		mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL112);
		goto error;
	}

	for(i = 0; i < pstPlayer->stRoleData.MiscInfo.BlackNum; i++)
	{
		if (player_black_find_byname(&pstPlayer->stRoleData.MiscInfo,
		                            (char *)pstSendMail->To) >= 0)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL113);
			goto error;
		}
	}

	for(i = 0; i < pstPlayer->stRoleData.MiscInfo.Enemy.EnemyNum; i++)
	{
		const char *pszName = 
			pstPlayer->stRoleData.MiscInfo.Enemy.Enemys[i].RoleName;

		if (strcmp(pszName, pstSendMail->To) == 0)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL114);
			goto error;
		}
	}

	pstToPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstSendMail->To);
	if (pstToPlayer != NULL)
	{		
		if (player_black_find_byid(&pstToPlayer->stRoleData.MiscInfo,
			                       pstPlayer->stRoleData.RoleID) >= 0)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL115);
			return -1;
		}

		if (player_enemy_find_byname(&pstToPlayer->stRoleData.MiscInfo, 
			                         pstPlayer->stRoleData.RoleName) >= 0)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL116);
			goto error;
		}
	}

	//	测试物品或钱能否邮寄
	if (mail_items_post_test(pstEnv, pstPlayer, pstSendMail) < 0 ||
		mail_money_post_test(pstEnv, pstPlayer, pstSendMail) < 0)
	{
		goto error;
	}

	return 0;
	
error:
	return -1;
}


/*用player_sysmail_fast_send 这个新函数
int player_mail_fast_send(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                      const MailFastSend *pstSendMail)
{
	int i;
	ZoneMail *pstZoneMail = NULL;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv, pstSendMail->szTo);
	Player *pstTo = z_name_player(pstEnv->pstAppCtx, pstEnv, pstSendMail->szTo);
	char *pszHint = "邮件发送失败";

	if (!pstTo)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "can not found receiver.");
		goto error;
	}

	if (!(pstTo->stOnline.cMiscFlag & MISC_FLAG_MAIL_FETCHED))
	{
		pszHint = "服务器暂忙，邮件发送失败，请稍候再试";
		goto error;
	}

	if (!pstInbox)
	{
		pstInbox = mail_new_mailbox(pstEnv, pstSendMail->szTo);
		if (!pstInbox)
		{
			goto error;
		}

		pstInbox->iState |= MAILBOX_STATE_CONNECTED;
	}
	else
	{
		if (pstInbox->nNormalNum >= MAIL_MAX_NORMAL_NUM)
		{
			pszHint = "邮件发送失败,对方邮箱已满";
			goto error;
		}
	}

	pstZoneMail = mail_new_in(pstEnv, 0, pstSendMail->szTo,
		                      pstPlayer->stRoleData.RoleName,
		                      pstSendMail->szTitle);
	if (!pstZoneMail)
	{
		goto error;
	}

	for(i = 0; i < pstSendMail->iItemNum; i++)
	{
		ROLEGRID stDummyGrid;
		const MailFastSendItem *pstSendItem = pstSendMail->astItems + i;

		if (package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		                pstSendItem->iDefID, pstSendItem->iNum,
		                pstSendItem->cListType, NULL,PACKAGE_DEC_NONE) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0,
			           "internal error, item not found.");
			goto error;
		}

		if (item_create(pstEnv->pstAppCtx, pstEnv, &stDummyGrid,
		                pstSendItem->iDefID) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, 
				       "internal error, item can not be created.");
			goto error;
		}
		stDummyGrid.GridData.RoleItem.Num = pstSendItem->iNum;
		
		pstZoneMail->stMail.Detail.Items[pstZoneMail->stMail.Detail.ItemNum++] 
			= stDummyGrid;
	}

	pstZoneMail->stMail.Detail.Gold = pstSendMail->nMoney;
	STRNCPY(pstZoneMail->stMail.Detail.Text, pstSendMail->szText,
		    CCH(pstZoneMail->stMail.Detail.Text));

	mail_push_inbox(pstEnv, pstInbox, pstZoneMail);
	if (pstInbox->nNormalNum + pstInbox->nSystemNum <= MAIL_MAX_NUM)
	{
		mail_new_notify(pstEnv, pstInbox, &pstZoneMail->stMail.Head);
	}

	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
	               "邮件发送成功");

	z_rolemail_oplog(pstEnv, NULL, OP_MAIL_SEND, &pstZoneMail->stMail);
	
	return 0;

error:
	if(pstZoneMail != NULL)
	{
		tmempool_free(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);
	}

	z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, pszHint);
	return -1;
}*/

/*iGridNum > MAIL_MAX_ITEM 则分多封系统邮件发送
    角色在线与不在线都要考虑
*/
int player_sysmail_fast_send(ZONESVRENV *pstEnv, char *pszName, char *pszTitle,
							 char *pszText, ROLEGRID *pstGrid, int iGridNum,
							 int iMoney, char* pSender, int fIsGold)
{
	ZONECTLSENDMAILREQ stReq;
	ZONESENDMAILREQ *pstCore = &stReq.Core;
	int i = iGridNum / MAIL_MAX_ITEM + ((iGridNum % MAIL_MAX_ITEM) > 0 ? 1 : 0);
	int iFlags = MAIL_FLAG_SYSTEM | (fIsGold ? MAIL_FLAG_GOLD : 0);

	if (pszName[0] == 0)
	{
		return 0;
	}

	if (iMoney < 0)
	{
		goto fatal;
	}

	bzero(&stReq.Extra, sizeof(stReq.Extra));
	STRNCPY(pstCore->To, pszName, CCH(pstCore->To));
	STRNCPY(pstCore->Send, LJY_MAIL2, CCH(pstCore->Send));
	STRNCPY(pstCore->Title, pszTitle, CCH(pstCore->Title));
	STRNCPY(pstCore->Text, pszText, CCH(pstCore->Text));

	if (pSender && 0 != *pSender)
	{		
		STRNCPY(pstCore->Send, pSender, CCH(pstCore->Send));
	}

	if (iGridNum == 0)
	{
		pstCore->ItemNum = 0;
		pstCore->Money = iMoney;
		if (ss_mail_sys_send(pstEnv, &stReq, iFlags) < 0)
		{
			goto fatal;
		}
		return 0;
	}

	while(i--)
	{
		int k = (iGridNum >= MAIL_MAX_ITEM) ? MAIL_MAX_ITEM
		                                    : (iGridNum % MAIL_MAX_ITEM);
		pstCore->ItemNum = 0;
		while(k--)
		{
			pstCore->Items[pstCore->ItemNum++] = *pstGrid++;
			iGridNum--;
		}
		pstCore->Money = iMoney;
		iMoney -= iMoney;

		if (ss_mail_sys_send(pstEnv, &stReq, iFlags) < 0)
		{
			goto fatal;
		}
	}

	return 0;

fatal:
	tlog_error(pstEnv->pstLogCat, 0, 0, "send system mail failed.");
	return -1;
}

// 投递邮件到收件箱
static int player_mail_post_inbox(ZONESVRENV* pstEnv, Player *pstPlayer,
                                  CSMAILCLTSEND *pstSendMail, Mailbox *pstInbox)
{
	int iRet = 0;
	ZoneMail *pstZoneMail = NULL;

	if (player_mail_send_test(pstEnv, pstPlayer, pstSendMail) < 0)
	{
		goto error;
	}

	if (mail_post_inbox_test(pstEnv, pstInbox, 0) < 0)
	{
		mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL117);
		goto error;
	}
	
	pstZoneMail = mail_new_in(pstEnv, 0, pstSendMail->To, 
		                      pstPlayer->stRoleData.RoleName,
		                      pstSendMail->Title);
	if (!pstZoneMail)
	{
		goto error;
	}

	if (mail_money_post(pstEnv, pstZoneMail, pstPlayer, pstSendMail) < 0 ||
		mail_items_post(pstEnv, pstZoneMail, pstPlayer, pstSendMail) < 0)
	{
		// 天意难为。
		goto error;
	}

	STRNCPY(pstZoneMail->stMail.Detail.Text, pstSendMail->Text,
		    CCH(pstZoneMail->stMail.Detail.Text));

	if (mail_post_inbox(pstEnv, pstInbox, pstZoneMail) < 0)
	{
		goto error;
	}

	//if (pstInbox->nNormalNum + pstInbox->nSystemNum <= MAIL_MAX_NUM)
	{
		mail_new_notify(pstEnv, pstInbox, &pstZoneMail->stMail.Head, 0);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
	               ZONEERR_MAIL2);
	z_rolemail_oplog(pstEnv, pstPlayer, OP_MAIL_SEND, &pstZoneMail->stMail);

final:
	mail_send_res(pstEnv, NULL, pstPlayer,
	              iRet < 0 ? 0 : (!pstSendMail->ItemNum ? 1 : 2));
	pstInbox->iActive = pstEnv->pstAppCtx->stCurr.tv_sec;
	return iRet;

error:
	if (pstZoneMail != NULL)
	{	
		tmempool_free(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);
	}

	iRet = -1;
	goto final;
	return 0;
}

// 发送所有邮件头快照
int player_mail_head_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i = -1;
	Mailbox *pstInbox = NULL;
	CSPKG stPkg;
	CSMAILSVRHEADSNAPSHOT *pstSnapshot 
		= &stPkg.Body.MailSvr.MailSvrData.MailHeadSnapshot;

	if (!pstPlayer)
	{
		return -1;
	}
	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_MAIL_SNAP;

	pstInbox = mail_find_mailbox(pstEnv, pstPlayer->stRoleData.RoleName);
	if (!pstInbox ||
		(pstInbox->nNormalNum + pstInbox->nSystemNum == 0))
	{
		return -1;
	}

	bzero(pstSnapshot, sizeof(*pstSnapshot));
	dlist_foreach(pstEnv->pstMailPool, i, pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			return -1;
		}

		pstSnapshot->Heads[pstSnapshot->Num++] = pstZoneMail->stMail.Head;
		if (pstSnapshot->Num >= CCH(pstSnapshot->Heads))
		{
			break; // 不要超过最大限制了
		}
	}

	stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_SNAPSHOT;
	Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}
		
// 阅读邮件
int player_mail_read(ZONESVRENV *pstEnv, Player *pstPlayer, tdr_ulonglong WID)
{
	ZoneMail *pstZoneMail = NULL;
	MAIL *pstMail = NULL;
	CSPKG stPkg;
	MAIL *pstContent = &stPkg.Body.MailSvr.MailSvrData.MailContent;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	if (!pstInbox)
	{
		// 角色木有邮件
		return -1;
	}

	pstZoneMail = mail_find(pstEnv, pstInbox, WID);
	if (!pstZoneMail)
	{
		// 找不到指定的邮件
		return -1;
	}

	// 作一下标记,说明邮件已读
	if (!(pstZoneMail->stMail.Head.Flags & MAIL_FLAG_READ))
	{
		MAIL_BEGIN_WRITE(pstZoneMail, pstMail)
			pstZoneMail->stMail.Head.Flags |= MAIL_FLAG_READ;
		MAIL_COMMIT_WRITE(pstZoneMail)

		z_rolemail_oplog(pstEnv, pstPlayer, OP_MAIL_READ, &pstZoneMail->stMail);
	}

	*pstContent = pstZoneMail->stMail;

	stPkg.Body.MailSvr.Operation = MAIL_SVR_CONTENT;
	Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}
		
// 删除邮件
int player_mail_delete(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong WID,int iOpType)
{
	CSPKG stPkg;
	CSMAILSVRCHG *pstChg = &stPkg.Body.MailSvr.MailSvrData.MailChgData;
	ZoneMail *pstZoneMail = NULL;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	if (!pstInbox)
	{
		return -1;
	}

	pstZoneMail = mail_find(pstEnv, pstInbox, WID);
	if (!pstZoneMail)
	{
		return -1;
	}

	pstChg->What = MAIL_CHG_DEL;
	pstChg->ChgInfo.DeletedWIDs.Num = 1;
	pstChg->ChgInfo.DeletedWIDs.WIDs[0] = pstZoneMail->stMail.Head.WID;
	if (mail_delete(pstEnv, pstInbox, pstZoneMail) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "mail deleting error.");
		return -1;
	}
	
	pstInbox->iActive = pstEnv->pstAppCtx->stCurr.tv_sec;

	stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_rolemail_oplog(pstEnv, pstPlayer, iOpType, &pstZoneMail->stMail);
	return 0;
}

// 发送角色剩余的邮件头
static int player_mail_remain(ZONESVRENV *pstEnv, Player *pstPlayer, int iCount)
{
	int i, n = 0, iTotalNum = 0;
	const Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                            pstPlayer->stRoleData.RoleName);
	if (!pstInbox)
	{
		return -1;
	}

	iTotalNum = pstInbox->nNormalNum + pstInbox->nSystemNum;
	if (iTotalNum + iCount <= MAIL_MAX_NUM)
	{
		return 0; // 没有剩余邮件鸟
	}

	dlist_foreach(pstEnv->pstMailPool, i, pstInbox->iMailList)
	{
		const ZoneMail *pstZoneMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			return -1;
		}

		// 补充剩余的邮件头，直至达到MAIL_MAX_NUM
		if (n++ >= MAIL_MAX_NUM - iCount)
		{
			mail_new_notify(pstEnv, pstInbox, &pstZoneMail->stMail.Head, 1);
		}

		if (n >= MAIL_MAX_NUM)
		{
			break;
		}
	}
	return 0;
}

// 删除全部邮件
static int player_mail_all_delete(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  int *piDeletedCount)
{
	int i;
	CSPKG stPkg;
	CSMAILSVRCHG *pstChg = &stPkg.Body.MailSvr.MailSvrData.MailChgData;
	MAILCHGWIDS *pstDelInfo = &pstChg->ChgInfo.DeletedWIDs;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	*piDeletedCount = 0;
	if (!pstInbox || !(pstInbox->nNormalNum + pstInbox->nSystemNum))
	{
		return -1;
	}

	bzero(pstChg, sizeof(*pstChg));

	pstChg->What = MAIL_CHG_DEL;
	
	i = dlist_head(pstEnv->pstMailPool, pstInbox->iMailList);
	while(i != pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			return -1;
		}

		i = pstZoneMail->stEntry.iNext;
		if (0 == mail_delete(pstEnv, pstInbox, pstZoneMail))
		{
			pstDelInfo->WIDs[pstDelInfo->Num++] = pstZoneMail->stMail.Head.WID;
			z_rolemail_oplog(pstEnv, pstPlayer, OP_MAIL_DELETE,
			                 &pstZoneMail->stMail);
		}

		if (pstDelInfo->Num >= MAIL_MAX_NUM)
		{
			break;
		}
	}

	if (pstDelInfo->Num > 0)
	{
		*piDeletedCount = pstDelInfo->Num;

		stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
		Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	return 0;
}
		
// 发送邮件
int player_mail_send(ZONESVRENV *pstEnv, Player *pstPlayer,
                     CSMAILCLTSEND *pstReq)
{
	Mailbox *pstInbox = NULL;
	int iRet = 0, iCookie = -1;

	//新需求玩家不能邮件货币了
	if (pstReq->Gold > 0)
	{
		return -1;
	}

	if ((pstReq->ItemNum > 0 || pstReq->Gold > 0) &&
		player_check_safelock(pstEnv,pstPlayer) < 0)
	{
		return -1;
	}
	
	pstInbox = mail_find_mailbox(pstEnv, pstReq->To);
	if (pstInbox)
	{
		// 优化:对方的邮箱存在,直接发送到邮箱即可
		iRet = player_mail_post_inbox(pstEnv, pstPlayer, pstReq, pstInbox);
		if(iRet < 0)
		{
			goto error;
		}
		goto final;
	}
	
	// 对方无邮箱,需要异步发送邮件了
	iCookie = mail_normal_send_list_add(pstEnv, pstPlayer->stRoleData.RoleName, 
		                                pstReq);
	if (iCookie < 0)
	{
		goto error;
	}
	
	if (z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->To))
	{
		// 优化:接收者在线，说明收件人肯定存在，无需验证收件人存在。
		if (mail_select_req(pstEnv, pstReq->To, iCookie) < 0)
		{
			goto error;
		}
	}
	else
	{
		// 接收者不在线，需要检查收件人名字存不存在。
		// 如果收件人名字存在，在执行mail_select_req继续邮件发送过程。
		if (mail_role_count_req(pstEnv, pstReq->To, iCookie) < 0)
		{
			goto error;
		}
	}

final:
	return iRet;

error:
	mail_send_res(pstEnv, NULL, pstPlayer, 0);
	iRet = -1;
	goto final;
	return 0;
}

// 测试是否能按清单中的内容获取物品
int player_mail_get_test(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const ZoneMail *pstZoneMail, int iHow,
                         tdr_ulonglong ullItemWID )
{
	int iRet = 0;

	switch(iHow)
	{
	case MAIL_GET_ITEM:
		{
			int i = pstZoneMail->stMail.Detail.ItemNum;
			const ROLEGRID *astItems = pstZoneMail->stMail.Detail.Items;

			for(i = 0; i < pstZoneMail->stMail.Detail.ItemNum; i++)
			{
				if (astItems[i].GridData.RoleItem.WID == ullItemWID)
				{
					break;
				}
			}

			if (i >= pstZoneMail->stMail.Detail.ItemNum)
			{
				goto no_item;
			}
			if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			                     astItems + i, 1, 0) < 0)
			{
				goto package_full;
			}
		}
		break;

	case MAIL_GET_MONEY:
		{
			if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_GOLD)
			{
				if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, 
					                       pstZoneMail->stMail.Detail.Gold) < 0)
				{
					goto money_full;
				}
			}
			else
			{
				if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, 
					                       pstZoneMail->stMail.Detail.Gold) < 0)
				{
					goto money_full;
				}
			}
		}
		break;

	case MAIL_GET_ALL:
		{
			int iNum = pstZoneMail->stMail.Detail.ItemNum;
			const ROLEGRID * astItems = pstZoneMail->stMail.Detail.Items;

			if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, astItems,
			                     iNum, 0) < 0)
			{
				goto package_full;
			}

			if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_GOLD)
			{
				if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, 
					                       pstZoneMail->stMail.Detail.Gold) < 0)
				{
					goto money_full_gold;
				}
			}
			else
			{
				if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, 
					                       pstZoneMail->stMail.Detail.Gold) < 0)
				{
					goto money_full;
				}
			}
		}
		break;

	default:
		goto fatal;
		break;
	}

final:
	return iRet;

fatal:
	iRet = -2;
	goto final;

no_item:
	iRet = -3;
	goto final;

package_full:
	iRet = -4;
	goto final;

money_full:
	iRet = -5;
	goto final;
	
money_full_gold:
	iRet = -7;
	goto final;	
	return 0;
}

// 收取邮件中的物品和钱
static int player_mail_do_get(ZONESVRENV *pstEnv, Player *pstPlayer,
                              ZoneMail *pstZoneMail, int iHow,
                              tdr_ulonglong ullItemWID,
                              CSMAILSVRGOODSCHG *pstChgInfo)
{
	int iWhat  = 0;
	MAIL *pstMail = NULL;
	OPROLEGETMAILGOODS stOpGoods;
	
	pstChgInfo->WID = pstZoneMail->stMail.Head.WID;
	pstChgInfo->Money = 0;
	pstChgInfo->Num = 0;

	// 初始化Op Log 数据
	stOpGoods.MailWID = pstZoneMail->stMail.Head.WID;
	stOpGoods.ItemNum = 0;
	stOpGoods.Money = 0;
	
	switch(iHow)
	{
	case MAIL_GET_ITEM:
		{
			int i;
			ROLEGRID *astItems = pstZoneMail->stMail.Detail.Items;

			for(i = 0; i < pstZoneMail->stMail.Detail.ItemNum; i++)
			{
				if (astItems[i].GridData.RoleItem.WID == ullItemWID)
				{
					break;
				}
			}

			stOpGoods.Items[stOpGoods.ItemNum++] = astItems[i];
			pstChgInfo->ItemWIDs[pstChgInfo->Num++] = 
				astItems[i].GridData.RoleItem.WID;

			if (i >= pstZoneMail->stMail.Detail.ItemNum ||
				package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, astItems + i,
			               1, 0) < 0)
			{
				return -1;
			}

			if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_SYSTEM)
			{
				iWhat = IMPITEM_OPTYPE_SYSMAIL;
			}

			package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, astItems + i,1, 0, iWhat);

			MAIL_BEGIN_WRITE(pstZoneMail, pstMail)
				if (pstMail->Detail.ItemNum - i - 1 > 0)
				{
					memmove(
						pstMail->Detail.Items + i,
						pstMail->Detail.Items + i + 1,
						(pstMail->Detail.ItemNum - i - 1) *
						sizeof(pstMail->Detail.Items[0]));
				}
				pstMail->Detail.ItemNum--;
				pstMail->Head.Flags &= ~MAIL_FLAG_NONORM;
			MAIL_COMMIT_WRITE(pstZoneMail)
		}
		break;

	case MAIL_GET_MONEY:
		{
			if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_GOLD)
			{
				if (package_add_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, 
					                  pstZoneMail->stMail.Detail.Gold) < 0)
				{
					return -1;
				}
				pstZoneMail->stMail.Head.Flags &= ~MAIL_FLAG_GOLD;

			}
			else
			{
				if (package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, 
					                  pstZoneMail->stMail.Detail.Gold) < 0)
				{
					return -1;
				}

				pstZoneMail->stMail.Head.Flags &= ~MAIL_FLAG_NONORM;

				if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_SYSTEM)
				{
					//z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SYSMAIL, 0,  pstZoneMail->stMail.Detail.Gold, 0, "邮件获取");
				}
			}

			pstChgInfo->Money = pstZoneMail->stMail.Detail.Gold;
			stOpGoods.Money = pstZoneMail->stMail.Detail.Gold;

			MAIL_BEGIN_WRITE(pstZoneMail, pstMail)
				pstMail->Detail.Gold = 0;
			MAIL_COMMIT_WRITE(pstZoneMail)
		}
		break;

	case MAIL_GET_ALL:
		{
			int i = pstZoneMail->stMail.Detail.ItemNum;
			ROLEGRID *astItems = pstZoneMail->stMail.Detail.Items;

			for(i = 0; i < pstZoneMail->stMail.Detail.ItemNum; i++)
			{
				pstChgInfo->ItemWIDs[pstChgInfo->Num++] = 
					astItems[i].GridData.RoleItem.WID;
				stOpGoods.Items[stOpGoods.ItemNum++] = astItems[i];
			}
			
			if(package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, astItems,
			                    i, 0) < 0)
			{
				return -1;
			}
			if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_GOLD)
			{
				if (package_add_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, 
					                  pstZoneMail->stMail.Detail.Gold) < 0)
				{
					return -1;
				}
			}
			else
			{
				if (package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, 
					                  pstZoneMail->stMail.Detail.Gold) < 0)
				{
					return -1;
				}
			}

			if (pstZoneMail->stMail.Head.Flags & MAIL_FLAG_SYSTEM)
			{
				iWhat = IMPITEM_OPTYPE_SYSMAIL;
			}
			
			package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, astItems,i, 0, iWhat) ;

			pstChgInfo->Money = pstZoneMail->stMail.Detail.Gold;
			stOpGoods.Money = pstZoneMail->stMail.Detail.Gold;

			MAIL_BEGIN_WRITE(pstZoneMail, pstMail)
				bzero(pstMail->Detail.Items, sizeof(pstMail->Detail.Items));
				pstMail->Detail.ItemNum = 0;
				pstMail->Detail.Gold = 0;
				pstMail->Head.Flags &= ~MAIL_FLAG_GOLD;
				pstMail->Head.Flags &= ~MAIL_FLAG_NONORM;
			MAIL_COMMIT_WRITE(pstZoneMail)
		}
		break;

	default:
		return -1;
		break;
	}

	z_role_get_mailgoods_oplog(pstEnv, pstPlayer, &stOpGoods);
	return 0;
}

static void player_mail_get_error(ZONESVRENV* pstEnv, Player *pstPlayer,
                                  int iErr)
{
	switch(iErr)
	{
	case -2:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_MAIL3);
		break;

	case -3:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_MAIL4);
		break;

	case -4:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		               ZONEERR_MAIL5);
		break;

	case -5:	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		                     ZONEERR_MAIL6,
		         		MONEY_TYPE_MONEY_NAME, MONEY_TYPE_MONEY_NAME);
		break;
	case -7:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		                     ZONEERR_MAIL6,
		         		MONEY_TYPE_GODCOIN_NAME, MONEY_TYPE_GODCOIN_NAME);
		break;
	case -6:
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL7);
		break;
	}

}
		
// 收取邮件中的物品和钱
static int player_mail_get(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                       const CSMAILGETTINGLIST *pstGettingList,
	                       int *piDeletedCount)
{
	int iRet = 0;
	CSPKG stPkg;
	CSMAILSVRGOODSCHG *pstChg =
		&stPkg.Body.MailSvr.MailSvrData.MailGoodsChgData;
	ZoneMail *pstZoneMail = NULL;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	*piDeletedCount = 0;
	if (!pstInbox)
	{
		goto error;
	}

	pstZoneMail = mail_find(pstEnv, pstInbox, pstGettingList->WID);
	if (!pstZoneMail)
	{
		goto error;
	}

	iRet = player_mail_get_test(pstEnv, pstPlayer, pstZoneMail,
	                            pstGettingList->How, 
		                        pstGettingList->ItemWID);
	if (iRet < 0)
	{
		goto error;
	}

	bzero(pstChg, sizeof(*pstChg));
	if (player_mail_do_get(pstEnv, pstPlayer, pstZoneMail, pstGettingList->How,
		                   pstGettingList->ItemWID, pstChg) < 0)
	{
		goto error;
	}

	stPkg.Body.MailSvr.Operation = MAIL_SVR_GOODS_CHG;
	Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	

	if (mail_empty(pstZoneMail) && (pstZoneMail->stMail.Detail.Text[0] == 0 || !pstGettingList->NoDel))
	{
		if (player_mail_delete(pstEnv, pstPlayer,
		                       pstZoneMail->stMail.Head.WID,OP_MAIL_GET_DEL) < 0)
		{
			goto final;
		}
		*piDeletedCount = 1;
	}
	else
	{
		stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
		stPkg.Body.MailSvr.MailSvrData.MailChgData.What = MAIL_CHG_MOD;
		stPkg.Body.MailSvr.MailSvrData.MailChgData.ChgInfo.ModHead.WID = pstZoneMail->stMail.Head.WID;
		stPkg.Body.MailSvr.MailSvrData.MailChgData.ChgInfo.ModHead.Flags = pstZoneMail->stMail.Head.Flags;
		Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			
		z_rolemail_oplog(pstEnv, pstPlayer, OP_MAIL_GET_GOODS, &pstZoneMail->stMail);
	}

final:
	return iRet;

error:
	player_mail_get_error(pstEnv, pstPlayer, iRet);
	iRet = -1;
	goto final;
	return 0;
}

// 收取所有邮件中的所有物品和钱
static int player_mail_all_get(ZONESVRENV* pstEnv, Player *pstPlayer,
                               int *piDeletedCount)
{
	int i = 0, iRet = 0, iHasErr = 0, iCount = 0;
	CSPKG stPkg, stPkg2;
	CSMAILSVRGOODSCHG stStub;
	MAILCHGWIDS *pstDelInfo 
		= &stPkg.Body.MailSvr.MailSvrData.MailChgData.ChgInfo.DeletedWIDs;
	Mailbox *pstInbox = mail_find_mailbox(pstEnv,
	                                      pstPlayer->stRoleData.RoleName);

	*piDeletedCount = 0;
	if (!pstInbox)
	{
		goto error;
	}

	bzero(pstDelInfo, sizeof(*pstDelInfo));
	
	i = dlist_head(pstEnv->pstMailPool, pstInbox->iMailList);
	while(i != pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;
		MAIL *pstMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			goto error;
		}

		MAIL_BEGIN_WRITE(pstZoneMail, pstMail)
			pstMail->Head.Flags |= MAIL_FLAG_READ;
		MAIL_COMMIT_WRITE(pstZoneMail)

		if (iCount++ >= MAIL_MAX_NUM)
		{
			break;
		}

		i = pstZoneMail->stEntry.iNext;
		if (mail_empty(pstZoneMail))
		{
			// 如果邮件为空就他妈的别收了。
			continue;
		}

		if (player_mail_get_test(pstEnv, pstPlayer, pstZoneMail, MAIL_GET_ALL,
		                         0ULL) < 0)
		{
			iHasErr = 1;
			continue;
		}
		if (player_mail_do_get(pstEnv, pstPlayer, pstZoneMail, MAIL_GET_ALL,
		                       0ULL, &stStub) < 0)
		{
			iHasErr = 1;
			continue;
		}

		if (mail_empty(pstZoneMail) && pstZoneMail->stMail.Detail.Text[0] == 0)
		{
			// 东西收完以后要删除邮件
			pstDelInfo->WIDs[pstDelInfo->Num++] = pstZoneMail->stMail.Head.WID;
			mail_delete(pstEnv, pstInbox, pstZoneMail);
			z_rolemail_oplog(pstEnv, pstPlayer, OP_MAIL_GET_DEL, &pstZoneMail->stMail);
		}
		else
		{
			stPkg2.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
			stPkg2.Body.MailSvr.MailSvrData.MailChgData.What = MAIL_CHG_MOD;
			stPkg2.Body.MailSvr.MailSvrData.MailChgData.ChgInfo.ModHead.WID = pstZoneMail->stMail.Head.WID;
			stPkg2.Body.MailSvr.MailSvrData.MailChgData.ChgInfo.ModHead.Flags = pstZoneMail->stMail.Head.Flags;
			Z_CSHEAD_INIT(&stPkg2.Head, MAIL_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg2);
		}
	}

	if (pstDelInfo->Num > 0)
	{
		*piDeletedCount = pstDelInfo->Num;

		pstInbox->iActive = pstEnv->pstAppCtx->stCurr.tv_sec;

		stPkg.Body.MailSvr.Operation = MAIL_SVR_HEAD_CHG;
		stPkg.Body.MailSvr.MailSvrData.MailChgData.What = MAIL_CHG_DEL;
		Z_CSHEAD_INIT(&stPkg.Head, MAIL_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	if (iHasErr)
	{
		iRet = -6;
		goto error;
	}

final:
	return iRet;

error:
	player_mail_get_error(pstEnv, pstPlayer, iRet);
	iRet = -1;
	goto final;
	return 0;
}

// 处理客户端发送过来的关于邮件的操作。
int player_mail(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, 
				TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSMAILCLT *pstMailClt = &pstCsPkg->Body.MailClt;
	CSMAILCLTDATA *pstCltData = &pstMailClt->MailCltData;
	int iCount = 0;

	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return 0;
	}

	switch(pstMailClt->Operation)
	{		
	case MAIL_CLT_READ: 	/* 阅读邮件 */
		player_mail_read(pstEnv, pstPlayer, pstCltData->MailWIDForRead);
		break;
		
	case MAIL_CLT_DELETE: 	/* 删除邮件 */
		if (player_check_safelock(pstEnv, pstPlayer) < 0)
		{
			return -1;
		}
		
		if (player_mail_delete(pstEnv, pstPlayer,
		                       pstCltData->MailWIDForDelete,OP_MAIL_DELETE) < 0)
		{
			return -1;
		}
		player_mail_remain(pstEnv, pstPlayer, 1);
		break;

	case MAIL_CLT_ALL_DELETE: 	/* 删除全部邮件 */
		if (player_check_safelock(pstEnv, pstPlayer) < 0)
		{
			return -1;
		}
		
		if (player_mail_all_delete(pstEnv, pstPlayer, &iCount) < 0)
		{
			return -1;
		}
		player_mail_remain(pstEnv, pstPlayer, iCount);
		break;
		
	case MAIL_CLT_SEND: 	/* 发送邮件 */
		if (pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			mail_operating_failed(pstEnv, NULL, pstPlayer, SYS_MIDDLE2, ZONEERR_MAIL118);
			mail_send_res(pstEnv, NULL, pstPlayer, 0);
			return -1;
		}
		player_mail_send(pstEnv, pstPlayer, &pstCltData->MailCltSend);
		break;
		
	case MAIL_CLT_GET: 	/* 收取邮件中的物品和钱 */
		if (player_mail_get(pstEnv, pstPlayer, &pstCltData->MailGettingList,
		                    &iCount) < 0)
		{
			return -1;
		}
		player_mail_remain(pstEnv, pstPlayer, iCount);
		break;

	case MAIL_CLT_ALL_GET: 	/* 收取所有邮件中的所有物品和钱 */
		if (player_mail_all_get(pstEnv, pstPlayer, &iCount) < 0)
		{
			return -1;
		}
		player_mail_remain(pstEnv, pstPlayer, iCount);
		break;

	default:
		return -1;
		break;
	}
	return 0;
}

// 发送系统邮件
int ss_mail_sys_send(ZONESVRENV *pstEnv, const ZONECTLSENDMAILREQ *pstCtlReq,
                     int iFlags)
{
	Mailbox *pstInbox = NULL;
	int iRet = 0, iCookie = 0;
	const ZONESENDMAILREQ *pstReq = &pstCtlReq->Core;

	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		Player *pstPlayer;
		SSPKG stSSPkg;
		SSCMDREQ* pstCmdReq = &stSSPkg.Body.CmdReq;
		int iBusID;

		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->To);
		if (pstPlayer)
		{
			iBusID = player_master_busid_get(pstEnv, pstPlayer);
		}
		else if (g_iZoneBus > 0)
		{
			iBusID = g_iZoneBus;
		}
		else
		{
			return -1;
		}
				
		memset(pstCmdReq,0,sizeof(*pstCmdReq));
		pstCmdReq->Cmd = SS_CMD_MAIL_PROXY;
		pstCmdReq->Data.MailProxy.MailReq = *pstCtlReq;
		pstCmdReq->Data.MailProxy.Flags = iFlags;

		battle_rolename_remove_prefix(pstCmdReq->Data.MailProxy.MailReq.Core.To);
		
		Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, iBusID);
		
		return 0;
	}

	pstInbox = mail_find_mailbox(pstEnv, pstReq->To);
	if (pstInbox)
	{
		iRet = mail_sys_post_inbox(pstEnv, pstCtlReq, pstInbox, iFlags);
		goto final;
	}
	
	iCookie = mail_system_send_list_add(pstEnv, pstCtlReq, iFlags);
	if (iCookie < 0)
	{
		goto error;
	}
	
	if (z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->To))
	{
		if (mail_select_req(pstEnv, pstReq->To, iCookie) < 0)
		{
			goto error;
		}
	}
	else
	{
		if (mail_role_count_req(pstEnv, pstReq->To, iCookie) < 0)
		{
			goto error;
		}
	}

final:
	if (pstCtlReq->Extra.IPInfo.ip != 0)
	{
		ss_mail_send_sys_res(pstEnv, &pstCtlReq->Extra, iRet < 0 ? 0 : 1);
	}
	return iRet;

error:
	iRet = -1;
	goto final;
	return 0;
}

// 删除邮件
int ss_mail_delete(ZONESVRENV *pstEnv, const ZONECTLDELMAILREQ *pstCtlReq)
{
	int iRet = 0;
	Mailbox *pstInbox = NULL;
	Player *pstPlayer = NULL;
	ZoneMail *pstZoneMail = NULL;
	SSPKG stSSPkg;
	const ZONEDELMAILREQ *pstReq = &pstCtlReq->Core;
	ZONECTLDELMAILRES *pstCtlRes = &stSSPkg.Body.CsCtrL.CsCmd.CtlDelMailRes;
	
	pstInbox = mail_find_mailbox(pstEnv, pstReq->RoleName);
	if (!pstInbox)
	{
		goto error;
	}

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->RoleName);
	if (pstPlayer)
	{
		if (player_mail_delete(pstEnv, pstPlayer, pstReq->MailID,OP_MAIL_SYS_DEL) < 0)
		{
			goto error;
		}
		player_mail_remain(pstEnv, pstPlayer, 1);
	}
	else
	{
		pstZoneMail = mail_find(pstEnv, pstInbox, pstReq->MailID);
		if (!pstZoneMail)
		{
			goto error;
		}

		if (mail_delete(pstEnv, pstInbox, pstZoneMail) == 0)
		{
			pstInbox->iActive = pstEnv->pstAppCtx->stCurr.tv_sec;

			z_rolemail_oplog(pstEnv, NULL, OP_MAIL_SYS_DEL, &pstZoneMail->stMail);
		}
	}
	mail_delete_commit(pstEnv); // 立即提交删除,防止数据库数据不同步
	                            // 跟cs_cgi.h中cs_find_first_maillist相关

final:
	ss_init_head(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_ROLE_MAIL_RES;
	pstCtlRes->Core.Succeeded = iRet < 0 ? 0 : 1;
	pstCtlRes->ReqCopy = pstCtlReq->Core;
	pstCtlRes->Extra = pstCtlReq->Extra;
	iRet = ss_sendto_cs_svr(pstEnv, &stSSPkg);
	return iRet;

error:
	iRet = -1;
	goto final;
	return 0;
}


int player_mail_chgname(ZONESVRENV* pstEnv, char *pszOldName, char *pszNewName)
{
	int i;
	Mailbox *pstInbox = NULL;
	Mailbox *pstNewInbox = NULL;
	MailboxHash stKey;

	// 修改发邮件者
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayerOnline;
	int j;
	
	for(i = 0; i < MAX_PLAYER; ++i)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayerOnline = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayerOnline->eStatus)
		{
			continue;
		}

		if (strcmp(pstPlayerOnline->stRoleData.RoleName, pszOldName) == 0)
		{
			continue;
		}

		pstInbox = mail_find_mailbox(pstEnv, pstPlayerOnline->stRoleData.RoleName);
		if (!pstInbox)
		{
			continue;
		}

		j = dlist_head(pstEnv->pstMailPool, pstInbox->iMailList);
		while(j != pstInbox->iMailList)
		{
			ZoneMail *pstZoneMail = NULL;
			
			if ((j < 0) ||
				(pstZoneMail = tmempool_get(pstEnv->pstMailPool, j)) == NULL)
			{
				continue;
			}

			j = pstZoneMail->stEntry.iNext;

			// 系统邮件不搞
			if (pstZoneMail->stMail.Head.Flags&MAIL_FLAG_SYSTEM)
			{
				continue;	
			}

			// 
			if (strcmp(pstZoneMail->stMail.Head.Send, pszOldName) == 0)
			{
				//dlist_remove(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);
				pstZoneMail->stInfo.iDirty = CACHE_FLAG_IMM;
				STRNCPY(pstZoneMail->stMail.Head.Send, pszNewName,sizeof(pstZoneMail->stMail.Head.Send));
				//mail_push_inbox( pstEnv, pstNewInbox, pstZoneMail);
			}
		}
	}
	
	pstInbox = mail_find_mailbox(pstEnv, pszOldName);
	if (!pstInbox)
	{
		// 无邮箱肯定无邮件
		return 0;
	}

	pstNewInbox = mail_new_mailbox(pstEnv, pszNewName);
	if (!pstNewInbox)
	{
		return -1;
	}

	pstNewInbox->iState = pstInbox->iState;

	i = dlist_head(pstEnv->pstMailPool, pstInbox->iMailList);
	while(i != pstInbox->iMailList)
	{
		ZoneMail *pstZoneMail = NULL;
		
		if ((i < 0) ||
			(pstZoneMail = tmempool_get(pstEnv->pstMailPool, i)) == NULL)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "inbox[%s] mail list error.",
			           pstInbox->szOwner);
			return -1;
		}

		i = pstZoneMail->stEntry.iNext;
		dlist_remove(pstEnv->pstMailPool, pstZoneMail->stEntry.iIdx);


		pstZoneMail->stInfo.iDirty = CACHE_FLAG_DIRTY;
		STRNCPY(pstZoneMail->stMail.Head.Recv, pszNewName,
			    sizeof(pstZoneMail->stMail.Head.Recv));
		mail_push_inbox( pstEnv, pstNewInbox, pstZoneMail);
	}

	tmempool_free(pstEnv->pstMailPool, pstInbox->iMailList);
	stKey.nIdx = (size_t)(pstInbox - pstEnv->pstPostoffice->astMailboxs);
	STRNCPY(stKey.szOwner, pstInbox->szOwner, sizeof(stKey.szOwner));
	sht_remove(pstEnv->pstShtMailbox, &stKey, MailHashCmp, MailHashCode);

	if(stKey.nIdx != pstEnv->pstPostoffice->nMailboxNum - 1)
	{
		*pstInbox =
			pstEnv->pstPostoffice->astMailboxs[
				pstEnv->pstPostoffice->nMailboxNum - 1];
	}
	pstEnv->pstPostoffice->nMailboxNum--;


	pstNewInbox->iState |= MAILBOX_STATE_CONNECTED;

	mail_mailbox_reidx_hash(pstEnv, 0);

	
	return 0;
}
