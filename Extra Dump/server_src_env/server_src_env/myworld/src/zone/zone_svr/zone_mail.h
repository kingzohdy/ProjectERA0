#ifndef ZONE_MAIL_H
#define ZONE_MAIL_H

#include "zone_db.h"

// 定时邮件操作
int z_active_mail(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount);

// 在服务器关闭前对邮件的最终操作
int z_db_mail_fin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv);

// 全服务器的所有邮件是否all clean
// all clean 返回1，否则返回0
int z_db_mail_is_allclean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv);

// 查询角色的邮件异步返回结果
int mail_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes,
                    const ActiveMailboxInfo *pstInfo);

// 更新角色一封邮件异步返回结果
int mail_update_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstPkg,
                    const ActiveMailboxInfo *pstInfo);

// 删除角色一封邮件异步返回结果
int mail_delete_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstPkg,
                    const ActiveMailboxInfo *pstInfo);

// 获取角色名字select count的异步返回结果
int mail_role_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes,
                        const ActiveMailboxInfo *pstInfo);

// 角色登录时，邮件相关操作
int mail_role_login(ZONESVRENV *pstEnv, Player *pstPlayer);

// 角色登出时，邮件相关操作
int mail_role_logout(ZONESVRENV *pstEnv, Player *pstPlayer);

// 处理客户端发送过来的关于邮件的操作。
int player_mail(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, 
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

// 发送所有邮件头快照
int player_mail_head_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer);

// 角色改名,有关邮件操作
int player_mail_chgname(ZONESVRENV *pstEnv, char *pszOldName, char *pszNewName);

// 快速发送邮件物品
typedef struct tagMailFastSendItem
{
	char cListType;
	int iDefID;
	int iNum;
}MailFastSendItem;

typedef struct tagMailFastSend
{
	char szTo[ROLE_NAME_LEN]; // 收件人
	char szTitle[MAIL_MAX_TITLE_LEN]; // 标题
	char szText[SHORT_DESC_LEN]; // 正文
	int iItemNum; // 邮寄的物品数量                  
	MailFastSendItem astItems[MAIL_MAX_ITEM]; // 邮寄的物品
	unsigned int nMoney; // 邮寄的金币
}MailFastSend;

/*使用player_sysmail_fast_send函数
// 快速发送邮件
int player_mail_fast_send(ZONESVRENV *pstEnv, Player *pstPlayer,
                          const MailFastSend *pstSendMail);*/

//快速发送系统邮件
int player_sysmail_fast_send(ZONESVRENV* pstEnv, char *pszName, char *pszTitle,
							 char *pszText, ROLEGRID *pstGrid, int iGridNum,
							 int iMoney, char* pSender, int fIsGold);

// 在加入黑名单、仇人列表之后通知一下
int mail_check_clean_notify(ZONESVRENV *pstEnv, Player *pstPlayer, 
                            const char *pszRoleName);

// 删除邮件
int ss_mail_delete(ZONESVRENV *pstEnv, const ZONECTLDELMAILREQ *pstCtlReq);

// 发送系统邮件
int ss_mail_sys_send(ZONESVRENV* pstEnv, const ZONECTLSENDMAILREQ *pstCtlReq,
                     int iFlags);

// 在删除角色时调用此函数, 做一些邮件相关的处理
int mail_role_on_delete(ZONESVRENV *pstEnv, const char *pszRoleName);

//
// 配置数据
//
extern int g_iMailMaxExistSecs;
extern int g_iMailPostingNeedItemID;
extern int g_iMailboxMaxWashingNum;
extern int g_iMailboxMinWashingNum;


#endif // ZONE_MAIL_H