#ifndef ZONE_MAIL_H
#define ZONE_MAIL_H

#include "zone_db.h"

// ��ʱ�ʼ�����
int z_active_mail(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, int iCount);

// �ڷ������ر�ǰ���ʼ������ղ���
int z_db_mail_fin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv);

// ȫ�������������ʼ��Ƿ�all clean
// all clean ����1�����򷵻�0
int z_db_mail_is_allclean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv);

// ��ѯ��ɫ���ʼ��첽���ؽ��
int mail_select_res(ZONESVRENV *pstEnv, TORMSELECTRES *pstSelectRes,
                    const ActiveMailboxInfo *pstInfo);

// ���½�ɫһ���ʼ��첽���ؽ��
int mail_update_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstPkg,
                    const ActiveMailboxInfo *pstInfo);

// ɾ����ɫһ���ʼ��첽���ؽ��
int mail_delete_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstPkg,
                    const ActiveMailboxInfo *pstInfo);

// ��ȡ��ɫ����select count���첽���ؽ��
int mail_role_count_res(ZONESVRENV *pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes,
                        const ActiveMailboxInfo *pstInfo);

// ��ɫ��¼ʱ���ʼ���ز���
int mail_role_login(ZONESVRENV *pstEnv, Player *pstPlayer);

// ��ɫ�ǳ�ʱ���ʼ���ز���
int mail_role_logout(ZONESVRENV *pstEnv, Player *pstPlayer);

// ����ͻ��˷��͹����Ĺ����ʼ��Ĳ�����
int player_mail(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, 
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

// ���������ʼ�ͷ����
int player_mail_head_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer);

// ��ɫ����,�й��ʼ�����
int player_mail_chgname(ZONESVRENV *pstEnv, char *pszOldName, char *pszNewName);

// ���ٷ����ʼ���Ʒ
typedef struct tagMailFastSendItem
{
	char cListType;
	int iDefID;
	int iNum;
}MailFastSendItem;

typedef struct tagMailFastSend
{
	char szTo[ROLE_NAME_LEN]; // �ռ���
	char szTitle[MAIL_MAX_TITLE_LEN]; // ����
	char szText[SHORT_DESC_LEN]; // ����
	int iItemNum; // �ʼĵ���Ʒ����                  
	MailFastSendItem astItems[MAIL_MAX_ITEM]; // �ʼĵ���Ʒ
	unsigned int nMoney; // �ʼĵĽ��
}MailFastSend;

/*ʹ��player_sysmail_fast_send����
// ���ٷ����ʼ�
int player_mail_fast_send(ZONESVRENV *pstEnv, Player *pstPlayer,
                          const MailFastSend *pstSendMail);*/

//���ٷ���ϵͳ�ʼ�
int player_sysmail_fast_send(ZONESVRENV* pstEnv, char *pszName, char *pszTitle,
							 char *pszText, ROLEGRID *pstGrid, int iGridNum,
							 int iMoney, char* pSender, int fIsGold);

// �ڼ���������������б�֮��֪ͨһ��
int mail_check_clean_notify(ZONESVRENV *pstEnv, Player *pstPlayer, 
                            const char *pszRoleName);

// ɾ���ʼ�
int ss_mail_delete(ZONESVRENV *pstEnv, const ZONECTLDELMAILREQ *pstCtlReq);

// ����ϵͳ�ʼ�
int ss_mail_sys_send(ZONESVRENV* pstEnv, const ZONECTLSENDMAILREQ *pstCtlReq,
                     int iFlags);

// ��ɾ����ɫʱ���ô˺���, ��һЩ�ʼ���صĴ���
int mail_role_on_delete(ZONESVRENV *pstEnv, const char *pszRoleName);

//
// ��������
//
extern int g_iMailMaxExistSecs;
extern int g_iMailPostingNeedItemID;
extern int g_iMailboxMaxWashingNum;
extern int g_iMailboxMinWashingNum;


#endif // ZONE_MAIL_H