#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cs_cgi.h"
#include "mysql/mysql.h"

struct test_routine_entry
{
	const char *name; // ��������
	int argc; // ��Ҫ�Ĳ�������
	int (*route)(int *, int, char *[]); // �ص�����
	const char *des; // ������Ϣ
	const char *man; // �ֲ�
};


static int test_help(int *handle, int argc, char *argv[]);
static int test_get_acc_info(int *handle, int argc, char *argv[]);
static int test_set_acc_uin_flag(int *handle, int argc, char *argv[]);
static int test_find_acc_by_rolename(int *handle, int argc, char *argv[]);
static int test_find_acc_by_loginname(int *handle, int argc, char *argv[]);
static int test_limited_find_acc_by_loginname(int *handle, int argc, char *argv[]);
static int test_set_acc_silence(int *handle, int argc, char *argv[]);
static int test_set_acc_freeze(int *handle, int argc, char *argv[]);
static int test_find_role_by_rolename(int *handle, int argc, char *argv[]);
static int test_find_role_by_loginname(int *handle, int argc, char *argv[]);
static int test_restore_deleted_role(int *handle, int argc, char *argv[]);
static int test_get_role_maillist(int *handle, int argc, char *argv[]);
static int test_get_role_maildata(int *handle, int argc, char *argv[]);
static int test_del_role_mail(int *handle, int argc, char *argv[]);
static int test_send_sys_mail(int *handle, int argc, char *argv[]);
static int test_copy_role(int *handle, int argc, char *argv[]);
static int test_get_bulletin_list(int *handle, int argc, char *argv[]);
static int test_add_bulletin(int *handle, int argc, char *argv[]);
static int test_del_bulletin(int *handle, int argc, char *argv[]);
static int test_get_skill_info(int *handle, int argc, char *argv[]);
static int test_get_item_info(int *handle, int argc, char *argv[]);
static int test_add_role_designation(int *handle, int argc, char *argv[]);
static int test_del_role_designation(int *handle, int argc, char *argv[]);
static int test_get_role_designation_list(int *handle, int argc, char *argv[]);
static int test_set_role_respact(int *handle, int argc, char *argv[]);
static int test_del_role_respact(int *handle, int argc, char *argv[]);
static int test_get_role_respact_list(int *handle, int argc, char *argv[]);
static int test_query_account_point(int *handle, int argc, char *argv[]);
static int test_get_clan_all_info(int *handle, int argc, char *argv[]);
static int test_del_clan_member(int *handle, int argc, char *argv[]);
static int test_disband_clan(int *handle, int argc, char *argv[]);
static int test_clear_clan_diplomatism(int *handle, int argc, char *argv[]);
static int test_get_package_max_num(int *handle, int argc, char *argv[]);
static int test_set_package_max_num(int *handle, int argc, char *argv [ ]);
static int test_get_package_item_info(int *handle, int argc, char *argv [ ]);
static int test_cash_props_gift_info(int *handle, int argc, char *argv[]);
static int test_get_role_drop_info(int *handle, int argc, char *argv[]);
static int test_role_drop_restore(int *handle, int argc, char *argv[]);
static int test_role_hatch_pet(int *handle, int argc, char *argv[]);
static int test_find_clan_by_name(int *handle, int argc, char *argv[]);
static int test_del_clan_diplomatism(int *handle, int argc, char *argv[]);
static int test_exchange_clan_leader(int *handle, int argc, char *argv[]);
static int test_set_clan_base_attr(int *handle, int argc, char *argv[]);
static int test_analy_interface(int *handle, int argc, char *argv[]);
static int test_offline_add_item(int *handle, int argc, char *argv[]);
static int test_find_all_role(int *handle, int argc, char *argv[]);
static int test_reload_server(int *handle, int argc, char *argv[]);
static int test_set_second_pwd(int *handle,int argc,char *argv[]);



static struct test_routine_entry test_routine_table[] =
{
	{
		"help", 
		0, test_help,
		"��ȡ������", 
		"�÷� : cs_test help [����]",
	},
	
	{
		"get-acc-info", 
		2, test_get_acc_info,
		"�����ʺ�ID��ȡ�ʺ���Ϣ��", 
		"�÷� : cs_test get-acc-info ��ID �ʺ���",
	},

	{
		"set-acc-uin-flag",
		3, test_set_acc_uin_flag,
		"�����ʺŵ�Uin Flag��",
		"�÷� : cs_test set-acc-uin-flag ����ID �ʺ��� UinFlag",
	},
	 
	{
		"find-acc-by-rolename",
		1, test_find_acc_by_rolename,
		"ͨ����ɫ�������ʺ���Ϣ��֧��ͨ���ģ����ѯ��",
		"�÷� : cs_test find-acc-by-rolename ��ɫ����ͨ���ַ���\n"
		"ͨ���˵�� : '%' ��ʾ0~�������ַ���'_'��ʾһ�������ַ���",
	},
	
	{
		"find-acc-by-loginname",
		1, test_find_acc_by_loginname,
		"ͨ���ʺ��������ʺ���Ϣ��֧��ͨ���ģ����ѯ��",
		"�÷� : cs_test find-acc-by-loginname �ʺ�����ͨ���ַ���\n"
		"ͨ���˵�� : '%' ��ʾ0~�������ַ���'_'��ʾһ�������ַ���",
	},

	{
		"limited-find-acc-by-loginname",
		2, test_limited_find_acc_by_loginname,
		"��ѯ�ʺŵ�ID��֧��ͨ���ģ����ѯ��",
		"�÷� : cs_test limited-find-acc-by-loginname ��ID ��ɫ����ͨ���ַ���\n"
		"ͨ���˵�� : '%' ��ʾ0~�������ַ���'_'��ʾһ�������ַ���",
	},
	
	{
		"set-acc-silence",
		4, test_set_acc_silence,
		"���û����ʺŽ��ԡ�",
		"�÷� : cs_test get-acc-info ����ID �ʺ��� 1(����)/0(���) ����ʱ��\n"
		"˵�� : ʱ�䵥λΪ�룬ʱ��=-1�����ý��ԣ��������ʱ�����Խ���ʱ�䡣",
	},
	
	{
		"set-acc-freeze",
		4, test_set_acc_freeze,
		"���û����ʺŶ��ᡣ",
		"�÷� : cs_test set-acc-freeze ����ID �ʺ��� 1(����)/0(�ⶳ) ����ʱ��\n"
		"˵�� : ʱ�䵥λΪ�룬ʱ��=-1�����ö��ᣬ�������ʱ�����Զ���ʱ�䡣",
	},
	
	{
		"find-role-by-rolename",
		3, test_find_role_by_rolename,
		"ͨ����ɫ����ѯ��ɫ��Ϣ��",
		"�÷� : cs_test find-role-by-rolename ����ID ��ID ��ɫ����ͨ����ַ���\n"
		"ͨ���˵�� : '%' ��ʾ0~�������ַ���'_'��ʾһ�������ַ���",
	},

	{
		"find-role-by-loginname",
		3, test_find_role_by_loginname,
		"ͨ���ʺ�����ȡ��ɫ��Ϣ��",
		"�÷� : cs_test find-role-by-rolename ����ID ��ID �ʺ�����ͨ����ַ���\n"
		"ͨ���˵�� : '%' ��ʾ0~�������ַ���'_'��ʾһ�������ַ���",
	},
	
	{
		"restore-deleted-role",
		3, test_restore_deleted_role,
		"�ָ���ɾ���Ľ�ɫ��",
		"�÷� : cs_test restore-deleted-role ��ID ��ID ��ɫID",
	},
	
	{
		"get-role-maillist",
		3, test_get_role_maillist,
		"��ȡĳ����ɫ�������ʼ�ͷ��Ϣ��",
		"�÷� : cs_test get-role-maillist ��ID ��ID ��ɫ��",
	},
	
	{
		"get-role-maildata",
		4, test_get_role_maildata,
		"��ȡĳ����ɫ��ĳ���ʼ�����ϸ��Ϣ��",
		"�÷� : cs_test get-role-maildata ��ID ��ID ��ɫ�� �ʼ�ID",
	},
	
	{
		"del-role-mail",
		4, test_del_role_mail,
		"ɾ��ĳ����ɫ��ĳ���ʼ���",
		"�÷� : cs_test del-role-mail ��ID ��ID ��ɫ�� �ʼ�ID",
	},
	
	{
		"send-sys-mail",
		6, test_send_sys_mail,
		"��ĳ����ҷ���һ��ϵͳ�ʼ���",
		"�÷� : cs_test send-sys-mail ��ID ��ID �ռ��˽�ɫ�� �������� ���� ����\n"
		"˵�� : \"�ռ��˽�ɫ��\" ��������ʵ���ڵĽ�ɫ��, \"��������\"��������д��",
	},
	
	{
		"copy-role",
		6, test_copy_role,
		"��һ����ɫ������Ϣ���Ƶ���һ�Ѵ��ڵĽ�ɫ��",
		"�÷� : cs_test copy-role ��ID ��ID Ŀ���ɫ�ʺ�ID Ŀ���ɫID Ŀ���ɫ�� "
		"Դ��ɫ�ʺ�ID Դ��ɫID Դ��ɫ��\n"
		"˵�� : Ŀ���ɫ�����Դ��ɫ��ͬһ��������Ŀ���ɫ�������߲���ʩ�д˲�����",
	},
	
	{
		"get-bulletin-list",
		2, test_get_bulletin_list,
		"��ȡĳ���������Ĺ�����Ϣ�б�",
		"�÷� : cs_test get-bulletin-list ��ID ��ID",
	},
	
	{
		"add-bulletin",
		5, test_add_bulletin,
		"��ĳ�����������һ��������Ϣ��",
		"�÷� : cs_test add-bulletin ��ID ��ID �������� �������ʱ�� ����ʱ��\n"
		"˵�� : ʱ��ĵ�λ�����룬\"����ʱ��\"����С��\"�������ʱ��\"��",
	},
	
	{
		"del-bulletin",
		3, test_del_bulletin,
		"��ĳ����������ɾ��һ��������Ϣ��",
		"�÷� : cs_test del-bulletin ��ID ��ID ����ID",
	},
	
	{
		"get-skill-info",
		4, test_get_skill_info,
		"��ȡ��������Ϣ��",
		"�÷� : cs_test get-skill-info ��ID ��ID ��ɫID ��ɫ��",
	},
	
	{
		"get-item-info",
		4, test_get_item_info,
		"��ȡ������Ʒ��Ϣ��",
		"�÷� : cs_test get-item-info ��ID ��ID ��ɫID ��ɫ��",
	},

	{
		"add-role-designation",
		4, test_add_role_designation,
		"����һ����ɫ�ƺš�",
		"�÷� : add-role-designation ��ID ��ID ��ɫ�� �ƺ�ID",
	},

	{
		"del-role-designation",
		4, test_del_role_designation,
		"ɾ��һ����ɫ�ƺš�",
		"�÷� : del-role-designation ��ID ��ID ��ɫ�� �ƺ�ID",
	},

	{
		"get-role-designation-list",
		3, test_get_role_designation_list,
		"��ȡ��ɫ�ƺ��б�",
		"�÷� : get-role-designation-list ��ID ��ID ��ɫ��",
	},

	{
		"set-role-respact",
		5, test_set_role_respact,
		"���ӽ�ɫ����ֵ��",
		"�÷� : set-role-respact ��ID ��ID ��ɫ�� ������ӪID ���ӵ�����ֵ",
	},

	{
		"del-role-respact",
		4, test_del_role_respact,
		"ɾ����ɫ������",
		"�÷� : del-role-respact ��ID ��ID ��ɫ�� ������ӪID",
	},

	{
		"get-role-respact-list",
		3, test_get_role_respact_list,
		"��ȡ��ɫ�����б�",
		"�÷� : get-role-respact-list ��ID ��ID ��ɫ��",
	},
	
	{
		"query-account-point",
		2, test_query_account_point,
		"��ȡ�ʺŵĵ�ȯ��",
		"�÷� : query-account-point ��ID �ʺ�",
	},
	
	{
		"cash-props-gift",
		4, test_cash_props_gift_info,
		"��ֵ���񵽴���",
		"�÷� : cash-props-gift ��ID �ʺ� ���id �����",
	},

	{
		"get-clan-all-info",
		3, test_get_clan_all_info,
		"��ȡָ�������ȫ����Ϣ",
		"�÷� : get-clan-all-info ��ID ��ID ������"
	},

	{
		"del-clan-member",
		4, test_del_clan_member,
		"��ָ���Ĺ�����ɾ��һ��ָ���Ļ�Ա",
		"�÷� : del-clan-member ��ID ��ID ������ ��ɫ��",
	},

	{
		"disband-clan",
		3, test_disband_clan,
		"��ɢһ��ָ���Ĺ���",
		"�÷� : disband-clan ��ID ��ID ������",
	},

	{
		"clear-clan-diplomatism",
		3, test_clear_clan_diplomatism,
		"���һ��������⽻��ϵ",
		"�÷� : clear-clan-diplomatism ��ID ��ID ������",
	},

	{
		"get-package-max-num",
		4, test_get_package_max_num,
		"��ȡָ�������Ľ�����������",
		"�÷� : get-package-max-num ��ID ��ID ��ɫ����������",
	},

	{
		"set-package-max-num",
		5, test_set_package_max_num,
		"����ָ�������Ľ�����������",
		"�÷� : set-package-max-num ��ID ��ID ��ɫ�� �������� ���ӵĸ�����",
	},
	
	{
		"get-package-item-info",
		4, test_get_package_item_info,
		"��ѯ�̳���ʱ(ָ��)�����е���Ʒ",
		"�÷� : get-package-item-info ��ID ��ID ��ɫ�� ��������",
	},

	{
		"get-role-drop-info",
		3, test_get_role_drop_info,
		"��ȡ��ɫ����������",
		"�÷� : get-role-drop-info ��ID ��ID ��ɫ��",
	},

	{
		"role-drop-restore",
		3, test_role_drop_restore,
		"��ȡ��ɫ����������",
		"�÷� : role-drop-restore ��ID ��ID ��ɫ��",
	},

	{
		"role-hatch-pet",
		3, test_role_hatch_pet,
		"��ȡ��ɫ����������",
		"�÷� : role-hatch-pet ��ID ��ID ��ɫ��",
	},

	{
		"find-clan-by-name",
		3, test_find_clan_by_name,
		"ģ����ѯ������",
		"�÷� : find-clan-by-name ��ID ��ID ��������ͨ����ַ���\n"
		"ͨ���˵�� : '%' ��ʾ0~�������ַ���'_'��ʾһ�������ַ���",
	},

	{
		"del-clan-diplomatism",
		4, test_del_clan_diplomatism,
		"ɾ��һ��ָ���Ĺ����⽻��ϵ",
		"�÷� : del-clan-diplomatism ��ID ��ID ������ Ŀ�깫������",
	},

	{
		"exchange-clan-leader",
		4, test_exchange_clan_leader,
		"������Ļ᳤ְλ������ָ����ɫ",
		"�÷� : exchange-clan-leader ��ID ��ID ������ Ŀ���ɫ����",
	},

	{
		"set_clan_base_attr", 
		7, test_set_clan_base_attr,
		"����һ������Ļ�����Դ,��,��,��,��", 
		"�÷� : cs_test set_clan_base_attr  ��ID, ��ID , ������, ��Ǯ\n"
        ", ������, ���ٶ�, �Ƽ�����\n"
		"˵�� :",
	},

	{
		"analy_interface", 
		0, test_analy_interface,
		"����Blob����", 
		"�÷� : cs_test analy_interface ",
	},
		
	{
		"offline_add_item", 
		5, test_offline_add_item,
		"�����߽�ɫ����Ʒ", 
		"�÷� : cs_test offline_add_item ��ID ��ID Ŀ���ɫ�� ��ƷID ��Ʒ����",
	},	

	{
		"find_all_role", 
		0, test_find_all_role,
		"��ȡ������ģ����ѯ��ɫ������������", 
		"�÷� : cs_test find_all_role ��ɫ��\n",
	},

	{
		"reload_server", 
		0, test_reload_server,
		"ָ��������reload", 
		"�÷� : cs_test reload_server  ��ID, ��ID , reload������Ϣ\n"
		"˵�� :",
	},

	{
		"set_second_pwd",
		4,test_set_second_pwd,
		"���ý�ɫ��������",
		"�÷�  ��cs_test set_second_pwd ��ID ��ID ��ɫ���� ����",
	},

};

static unsigned long long atoull(const char *p)
{
	int n = !p ? 0 : strlen(p);
	unsigned long long ret = 0ULL, exp = 1ULL;

	while(n--)
	{
		if(p[n] < '0' || p[n] > '9')
		{
			ret = 0ULL;
			goto out;
		}
		ret += (p[n] - '0') * exp;
		exp *= 10;
	}
	
out:
	return ret;
}
/*
static unsigned int htou(const char *p)
{
	int n = !p ? 0 : strlen(p);
	unsigned int ret = 0U, offset = 0U;

	if(n > 8)
	{
		ret = 0xffffffff;
		goto out;
	}

	while(n--)
	{
		if(p[n] >= '0' && p[n] <= '9')
			ret += ((p[n] - '0') << offset);
		else if(p[n] >= 'a' && p[n] <= 'f')
			ret += ((p[n] - 'a' + 10) << offset);
		else if(p[n] >= 'A' && p[n] <= 'F')
			ret += ((p[n] - 'A' + 10) << offset);
		else
		{
			ret = 0U;
			goto out;
		}
		offset += 4;
	}
	
out:
	return ret;
}
*/

static int test_get_acc_info(int *handle, int argc, char *argv[])
{
	SSACCINFO info;
	
	(void)argc;
	if(cs_get_account_info(handle, atoi(argv[0]), argv[1], &info, 2000) < 0)
		return -2;
		
	cs_print_data(handle, "SSAccInfo", &info, sizeof(info));
	return 0;
}

//104 ��� AccountSilenceEndTime
struct OLD_VERSION 
{
	long lVersion;
	long lSize;
	int Uin;                            
    int SilenceEndTime;                  	/*   ���Խ���ʱ�� */
};
//181 ��� AccountSilenceEndTime
struct NEW_VERSION 
{
	long lVersion;
	long lSize;
	int Uin;                            
    int SilenceEndTime;                  	/*   ���Խ���ʱ�� */
    int SilenceStartTime;                	/*  Ver.181 ʲôʱ��ʼ���Ե� */
};
static int test_analy_interface(int *handle, int argc, char *argv[])
{
	struct OLD_VERSION stOld;
	struct NEW_VERSION stNew;
	char szMsg[2048000];
	int* pHandle = NULL;


	szMsg[0] = 0;

	stOld.lSize = 2 * sizeof(int);
	stOld.lVersion = 104;
	
	stOld.Uin = 100;
	stOld.SilenceEndTime = 1000;

	stNew.lSize = 3* sizeof(int);
	stNew.lVersion = 181;

	stNew.Uin = 200;
	stNew.SilenceEndTime = 2000;
	stNew.SilenceStartTime = 20000;

	int ret = init_analy_interface("../../cfg/db_meta/role_db_meta.tdr",&pHandle);
	printf("Result: %d,%p\n", ret,pHandle);

/*		
	ret = analy_interface(pHandle,"AccountSilenceEndTime",(char*)&stOld,sizeof(stOld),szMsg, 512);
	printf("Result: %d,%s\n", ret,szMsg);

	ret = analy_interface(pHandle,"AccountSilenceEndTime",(char*)&stNew,sizeof(stNew),szMsg, 512);
	printf("Result: %d,%s\n", ret,szMsg);
*/

	MYSQL_RES *res;
	MYSQL_ROW row;
	int err;
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
		return -1;

	if(!mysql_real_connect(
		conn,
		"192.168.0.57",
		"mmog",
		"mmog",
		"world_log_2_22",
		0,
		NULL,
		CLIENT_COMPRESS))
	{
		return -1;
	}

	err = mysql_query(conn, "SELECT LogRoleData FROM `LogRoleSnapshot_130116` where Uin = 2332 and RoleID = 11681784041105579779  and Logtime = '2013-01-16 18:48:39';");
	if(err)
		return -1;

	res = mysql_store_result(conn);
	if(!res)
		return -1;

	row = NULL;
	
	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
			continue;
		unsigned long * pLength =  mysql_fetch_lengths(res);
		printf("%d\n--\n",*pLength);
		printf("%s\n",(char*)(row[0]));
		int iMsg = sizeof(szMsg);
		
		analy_interface(pHandle,"RoleData",(char*)(row[0]),16,szMsg, &iMsg);
		printf("%s\n",szMsg);
		break;
		// res_to_bin(pstResLib, hResDBHandle, (char*)(row[0]));
	}
	

	

	if (conn)
		mysql_close(conn);


	ret = fini_analy_interface((int **)&pHandle);
	printf("Result: %d,%p\n", ret,pHandle);


	return ret < 0 ? -2 : 0;
}

static int test_set_acc_uin_flag(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_set_account_uin_flag(handle, atoi(argv[0]), argv[1], atoi(argv[2]),
								0,
	                           2000) < 0)
		return -2;
	return 0;
}

static int test_find_acc_by_rolename(int *handle, int argc, char *argv[])
{
	ACCFINDRES res;
	FIND_ACCOUNT_HANDLE fh = NULL;

	if(cs_find_first_account_by_rolename(handle, argv[0], &fh, &res, 2000) < 0)
		return -2;
	do
	{
		cs_print_data(handle, "AccFindRes", &res, sizeof(res));
	}while(cs_find_next_account(fh, &res) >= 0);
	cs_close_handle(fh);
	return 0;
}

static int test_find_acc_by_loginname(int *handle, int argc, char *argv[])
{
	ACCFINDRES res;
	FIND_ACCOUNT_HANDLE fh = NULL;

	if(cs_find_first_account_by_loginname(handle, argv[0], &fh, &res, 2000) < 0)
		return -2;
	do
	{
		cs_print_data(handle, "AccFindRes", &res, sizeof(res));
	}while(cs_find_next_account(fh, &res) >= 0);
	cs_close_handle(fh);
	return 0;
}

static int test_limited_find_acc_by_loginname(int *handle, int argc, char *argv[])
{
	ACCFINDRES res;
	LIMITED_FIND_ACCOUNT_HANDLE lfh = NULL;

	if(cs_limited_find_first_account_by_loginname(handle, atoi(argv[0]), argv[1],
	                                              &lfh, &res, 2000) < 0)
		return -2;
	do
	{
		cs_print_data(handle, "AccFindRes", &res, sizeof(res));
	}while(cs_limited_find_next_account(lfh, &res) >= 0);
	cs_close_handle(lfh);
	return 0;
}

static int test_set_acc_silence(int *handle, int argc, char *argv[])
{	
	(void)argc;

	if(cs_set_account_silence(handle, atoi(argv[0]), argv[1], atoi(argv[2]),
	                          atoi(argv[3]), 2000) < 0)
		return -2;
		
	return 0;
}

static int test_set_acc_freeze(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_set_account_freeze(handle, atoi(argv[0]), argv[1], atoi(argv[2]),
	                         atoi(argv[3]),"test", 2000) < 0)
		return -2;
		
	return 0;
}

static int test_find_role_by_loginname(int *handle, int argc, char *argv[])
{
	ACCFINDROLELISTRES res;
	FIND_ACCOUNT_ROLE_HANDLE fh = NULL;

	if(cs_find_first_role_by_loginname(handle, atoi(argv[0]), atoi(argv[1]),
	                                   argv[2], &fh, &res, 2000) < 0)
		return -2;
	cs_print_data(handle, "AccFindRoleListRes", &res, sizeof(res));
	return 0;
}

static int test_find_role_by_rolename(int *handle, int argc, char *argv[])
{
	ACCFINDROLELISTRES res;
	FIND_ACCOUNT_ROLE_HANDLE fh = NULL;

	if(cs_find_first_role_by_rolename(handle, atoi(argv[0]), atoi(argv[1]),
	                                  argv[2], &fh, &res, 2000) < 0)
		return -2;
	cs_print_data(handle, "AccFindRoleListRes", &res, sizeof(res));
	return 0;
}

static int test_restore_deleted_role(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_restore_deleted_role(handle, atoi(argv[0]), atoi(argv[1]),
	                           atoull(argv[2]), 2000) < 0)
		return -2;

	return 0;
}

static int test_get_role_maillist(int *handle, int argc, char *argv[])
{
	ZONEGETMAILLISTRES res;
	FIND_MAILLIST_HANDLE fmh = NULL;
	
	(void)argc;
	if(cs_find_first_maillist(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                          &fmh, &res, 4000) < 0)
		return -2;
	do
	{
		cs_print_data(handle, "ZoneGetMailListRes", &res, sizeof(res));	
	}while(cs_find_next_maillist(fmh, &res) >= 0);
	return 0;
}

static int test_get_role_maildata(int *handle, int argc, char *argv[])
{
	MAIL res;

	(void)argc;
	if(cs_get_role_maildata(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                        atoull(argv[3]), &res, 2000) < 0)
		return -2;

	cs_print_data(handle, "Mail", &res, sizeof(res));
	return 0;
}

static int test_del_role_mail(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_del_role_mail(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                    atoull(argv[3]), 2000) < 0)
		return -2;

	return 0;
}

static int test_send_sys_mail(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_send_sys_mail(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                    argv[3], argv[4], argv[5], NULL, 0, 0, 2000) < 0)
		return -2;
		
	return 0;
}

static int test_copy_role(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_copy_role(handle, atoi(argv[0]), atoi(argv[1]), atoull(argv[2]),
	                argv[3], atoull(argv[4]), argv[5], 2000) < 0)
		return -2;
		
	return 0;
}

static int test_get_bulletin_list(int *handle, int argc, char *argv[])
{
	ZONEGETBULLETINLISTRES res;

	(void)argc;
	if(cs_get_bulletin_list(handle, atoi(argv[0]), atoi(argv[1]), &res, 2000) < 0)
		return -2;

	cs_print_data(handle, "ZoneGetBulletinListRes", &res, sizeof(res));	
	return 0;
}

static int test_add_bulletin(int *handle, int argc, char *argv[])
{
	int id = 0;
	
	(void)argc;
	//if(cs_add_bulletin(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
		                 //atoi(argv[3]), atoi(argv[4]), &id, 2000) < 0)
		//return -2;
	
	printf("�¹���ID : %d\n", id);
	return 0;
}

static int test_del_bulletin(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_del_bulletin(handle, atoi(argv[0]), atoi(argv[1]), atoi(argv[2]), 
		                 2000) < 0)
		return -2;
		
	return 0;
}

static int test_get_skill_info(int *handle, int argc, char *argv[])
{
	/*
	ZONEGETROLESKILLINFORES res;
	
	(void)argc;
	if(cs_get_skill_info(handle, atoi(argv[0]), atoi(argv[1]), atoull(argv[2]),
		                   argv[3], &res, 2000) < 0)
		return -2;

	cs_print_data(handle, "ZoneGetRoleSkillInfoRes", &res, sizeof(res));
	*/
	return 0;
}

static int test_get_item_info(int *handle, int argc, char *argv[])
{
	/*
	ZONEGETROLEITEMLISTRES res;
	
	(void)argc;
	if(cs_get_item_info(handle, atoi(argv[0]), atoi(argv[1]), atoull(argv[2]),
		                  argv[3], &res, 2000) < 0)
		return -2;

	cs_print_data(handle, "ZoneGetRoleItemListRes", &res, sizeof(res));
	*/
	return 0;
}

static int test_add_role_designation(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_add_designation(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                      atoi(argv[3]), 3000) < 0)
	{
		return -2;
	}
	return 0;
}

static int test_del_role_designation(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_del_designation(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                      atoi(argv[3]), 3000) < 0)
	{
		return -2;
	}
	return 0;
}

static int test_get_role_designation_list(int *handle, int argc, char *argv[])
{
	ROLEDESIGNATIONS res;
	(void)argc;
	if(cs_get_designation_list(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                           &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "RoleDesignations", &res, sizeof(res));
	return 0;
}

static int test_set_role_respact(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_set_respact(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                  atoi(argv[3]), atoi(argv[4]), 3000) < 0)
	{
		return -2;
	}
	return 0;
}

static int test_del_role_respact(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_del_respact(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                  atoi(argv[3]), 3000) < 0)
	{
		return -2;
	}
	return 0;
}

static int test_get_role_respact_list(int *handle, int argc, char *argv[])
{
	RESPACTINFO res;
	(void)argc;
	if(cs_respact_list(handle, atoi(argv[0]), atoi(argv[1]), argv[2], &res,
	                   3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "RespactInfo", &res, sizeof(res));
	return 0;
}

static int test_query_account_point(int *handle, int argc, char *argv[])
{
	BILLRECORD res;
	(void)argc;
	if(cs_query_bill_point(handle, atoi(argv[0]),argv[1], &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "BillRecord", &res, sizeof(res));
	return 0;
}

static int test_cash_props_gift_info(int *handle, int argc, char *argv[])
{
	CASHPROPSGIFTINFORES res;
	(void)argc;
	if(cs_cash_props_gift_bill(handle, atoi(argv[0]),argv[1],0,atoi(argv[2]),atoi(argv[3]), &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "CashPropsGiftInfoRes", &res, sizeof(res));
	return 0;
}

static int test_get_clan_all_info(int *handle, int argc, char *argv[])
{
	ZONEGETCLANALLINFORES res;
	(void)argc;
	if(cs_get_clan_all_info(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                        &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "ZoneGetClanAllInfoRes", &res, sizeof(res));
	return 0;
}

static int test_del_clan_member(int *handle, int argc, char *argv[])
{
	int ret = cs_del_clan_member(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                             argv[3], 3000);
	printf("Result: %d\n", ret);
	return ret < 0 ? -2 : 0;
}

static int test_disband_clan(int *handle, int argc, char *argv[])
{
	int ret = cs_disband_clan(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                          3000);
	printf("Result: %d\n", ret);
	return ret < 0 ? -2 : 0;
}

static int test_clear_clan_diplomatism(int *handle, int argc, char *argv[])
{
	int ret = cs_clear_clan_diplomatism(handle, atoi(argv[0]), atoi(argv[1]),
	                                    argv[2], 3000);
	printf("Result: %d\n", ret);
	return ret < 0 ? -2 : 0;
}

static int test_set_package_max_num(int *handle, int argc, char *argv[])
{
	ZONEGETROLEPACKAGERES res;
	(void)argc;
	if(cs_set_package_info(handle, atoi(argv[0]), atoi(argv[1]), argv[2], 
					atoi(argv[3]), atoi(argv[4]), &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "ZoneSetRolePackageRes", &res, sizeof(res));
	return 0;
}

static int test_get_role_drop_info(int *handle, int argc, char *argv[])
{
	ZONEGETROLEDROPRES res;
	(void)argc;
	if(cs_get_role_drop_info(handle, atoi(argv[0]), atoi(argv[1]), argv[2], 
					&res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "ZoneGetRoleDropRes", &res, sizeof(res));
	return 0;
}

static int test_role_drop_restore(int *handle, int argc, char *argv[])
{
	ZONEGETROLEDROPRES res;
	ROLEDROPRESTORERES stRestoreres;
	ROLEDROPRESTOREINFO stDropInfo;
	
	(void)argc;
	if(cs_get_role_drop_info(handle, atoi(argv[0]), atoi(argv[1]), argv[2], 
					&res, 3000) < 0)
	{
		return -2;
	}

	stDropInfo.Grid= res.DropData.DropGrids[0].Grid;

	if(cs_role_drop_restore(handle, atoi(argv[0]), atoi(argv[1]), ROLE_DROP_GRID, 
					&stDropInfo, argv[2],
					&stRestoreres, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "RoleDropRestoreRes", &stRestoreres, sizeof(stRestoreres));
	return 0;
}

static int test_role_hatch_pet(int *handle, int argc, char *argv[])
{
	ZONEROLEHATCHPETRES res;

	if(cs_role_hatch_pet(handle, atoi(argv[0]), atoi(argv[1]), argv[2], 
					1,  15, 3, 10,
					&res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "ZoneRoleHatchPetRes", &res, sizeof(res));
	return 0;
}

static int test_get_package_item_info(int *handle, int argc, char *argv[])
{
	ZONEGETPACKAGEITEMINFORES res;
	(void)argc;
	if(cs_get_package_item_info(handle, atoi(argv[0]), atoi(argv[1]), argv[2], 
					atoi(argv[3]),  &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "ZoneGetPackageItemInfoRes", &res, sizeof(res));
	return 0;
}

static int test_get_package_max_num(int *handle, int argc, char *argv[])
{
	ZONEGETROLEPACKAGERES res;
	(void)argc;
	if(cs_get_package_info(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                       atoi(argv[3]), &res, 3000) < 0)
	{
		return -2;
	}

	cs_print_data(handle, "ZoneGetRolePackageRes", &res, sizeof(res));
	return 0;
}

static int test_find_clan_by_name(int *handle, int argc, char *argv[])
{
	ZONEFINDCLANRES res;
	FIND_CLAN_HANDLE fh = NULL;
	(void)argc;
	if(cs_find_first_clan_profile_by_name(handle, atoi(argv[0]), atoi(argv[1]),
	                                      argv[2], &fh, &res, 3000) < 0)
		return -2;
	do
	{
		cs_print_data(handle, "ZoneFindClanRes", &res, sizeof(res));	
	}while(cs_find_next_clan_profile(fh, &res) >= 0);

	cs_close_handle(fh);
	return 0;
}

static int test_del_clan_diplomatism(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_del_clan_diplomatism(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                           argv[3], 3000) < 0)
		return -2;
	return 0;
}

static int test_exchange_clan_leader(int *handle, int argc, char *argv[])
{
	(void)argc;
	if(cs_exchange_clan_leader(handle, atoi(argv[0]), atoi(argv[1]), argv[2],
	                           argv[3], 3000) < 0)
		return -2;
	return 0;
}

static int test_set_clan_base_attr(int *handle, int argc, char *argv[])
{
	int ret = cs_set_clan_base_attr(handle,atoi(argv[0]), atoi(argv[1]),
								argv[2], atoi(argv[3]), atoi(argv[4]),
								atoi(argv[5]), atoi(argv[6]),3000);
	printf("Result: %d\n", ret);
	return ret < 0 ? -2 : 0;
}

static int test_offline_add_item(int *handle, int argc, char *argv[])
{
	int ret = 0;
	ZONECREATEITEMREQ stReq;
	ZONECREATEITEMRES stOut;

	if (atoi(argv[3]) < 0 || atoi(argv[4]) < 1)
	{
		return -2;
	}

	memset(&stReq,0,sizeof(stReq));
	memset(&stOut,0,sizeof(stOut));

	stReq.Num = 1;
	stReq.ItemIdents[0].DefID = atoi(argv[3]);
	stReq.ItemIdents[0].Num = atoi(argv[4]);

	

	ret = cs_create_role_item(handle,&stReq,&stOut);	

	printf("cs_create_role_item Result: %d\n", ret);	

	ret = cs_set_item_info(handle,atoi(argv[0]),atoi(argv[1]),argv[2],
							5,&stOut.RoleGrids[0],2,NULL,3000);
	
	printf("cs_set_item_info Result: %d\n", ret);

	if (ret < 0)
	{
		return -2;
	}

	printf("Result: %d\n", ret);
	return ret < 0 ? -2 : 0;
}

struct _FindAccountRoleContext
{
	int *pHandle;
	int iTimeOut;
	SSFINDACCOUNTROLELISTKEY stKey;
	int iRegionID;
	int iWorldID;
	int iPrevCount;
	int iQueryIndex;
	int iByWhat;
};

static int test_find_all_role(int *handle, int argc, char *argv[])
{
	int ret = 0;
	FIND_ACCOUNT_ROLE_HANDLE hFind;
	ACCFINDROLELISTRES stOut;
	int i = 0;

	if (argc < 1)
	{
		return -2;
	}

	ret = cs_find_all_first_role_by_rolename(handle, argv[0], &hFind, &stOut,3000);
	
	printf("cs_find_all_first_role_by_rolename Result: %d\n", ret);
	
	if (ret < 0)
	{
		return ret;
	}

	cs_print_data(handle, "AccFindRoleListRes", &stOut, sizeof(stOut));

	for(i = 0; i < stOut.ResNum; i++)
	{
		printf("region %d world %d name %s\n",hFind->iRegionID,hFind->iWorldID,stOut.RoleArray[i].RoleName);
	}
	
	while(0 == (ret = cs_find_all_next_role(handle,hFind, &stOut))) 
	{
		for(i = 0; i < stOut.ResNum; i++)
		{
			printf("region %2d world %2d name %s\n",hFind->iRegionID,hFind->iWorldID,stOut.RoleArray[i].RoleName);
		}
		cs_print_data(handle, "AccFindRoleListRes", &stOut, sizeof(stOut));
	}
	
	cs_close_handle(hFind);
	
	printf("Result: %d\n", ret);
	return ret < 0 ? -2 : 0;
}

static int test_reload_server(int *handle, int argc, char *argv[])
{
	int ret = 0;

	ZONERELOADRES stOut;

	if (argc < 2)
	{
		return -2;
	}
		
	ret = cs_reload_server(handle,atoi(argv[0]),atoi(argv[1]),&stOut,3000);

	cs_print_data(handle, "AccFindRoleListRes", &stOut, sizeof(stOut));
	
	printf("Result: %d\n", ret);
	
	return ret < 0 ? -2 : 0;
}

static int test_set_second_pwd(int *handle,int argc,char *argv[])
{
	int ret = 0;
	ret = cs_set_second_pwd(handle,atoi(argv[0]),atoi(argv[1]),argv[2],argv[3],3000);
	printf("Result: %d\n",ret);
	return ret < 0 ? -2 : 0;
}

static void print_all_des()
{
	int i = sizeof(test_routine_table)/sizeof(test_routine_table[0]);
	const struct test_routine_entry *curr = test_routine_table;
	
	while(i--)
	{
		printf(curr->name);
		if(curr->des)
			printf(" : %s", curr->des);
		putchar('\n');
		curr++;
	}
}

static int test_help(int *handle, int argc, char *argv[])
{
	(void)handle;
	
	if(argc == 0)
	{
		puts("\n�÷� : cs_test ���� [[[����1 [[����2] ����3] ... ]");
		puts("����һ�� : ");
		print_all_des();
	}
	else
	{
		int i = sizeof(test_routine_table)/sizeof(test_routine_table[0]);
		const struct test_routine_entry *curr = test_routine_table;
		while(i--)
		{
			if(strcmp(curr->name, argv[0]) == 0)
			{
				printf("\n���� : %s\n���� : %s\n", curr->name, curr->des);
				puts(curr->man);
				break;
			}
			curr++;
		}
	}
	return 0;
}

static int process_acc_test(int argc, char *argv[])
{
	int *handle = NULL, i = 0, ret = -1;
	struct test_routine_entry *curr = NULL;
	
	if(argc < 2)
	{
		ret = -1;
		goto out;
	}
	
	if(init_interface("../../cfg/proto_ss.tdr", &handle, "cs_cgi.xml"))
	{
		printf("��ʼ��ʧ��!\n");
		return -1;
	}

	srand((unsigned)time( NULL ));
	
	i = sizeof(test_routine_table)/sizeof(test_routine_table[0]);
	curr = test_routine_table;
	while(i--)
	{
		if(strcmp(curr->name, argv[1]) == 0)
		{
			if(curr->argc > argc - 2)
			{
				ret = -4;
				break;
			}
			
			ret = curr->route(handle, argc - 2, argv + 2);
			break;
		}
		curr++;
	}
	
	puts("----------------------------------------------------------------------");
out:
	switch(ret)
	{
	case -4:
		printf("��������.\n");
		if(curr->man)
			printf("%s\n", curr->man);
		break;

	case -3:
		printf("����ʧ��.\n");
		break;
		
	case -2:
		printf("�ӿڵ���ʧ��.\n");
		break;
		
	case -1:
		printf("ָ�����������.\n");
		print_all_des();
		break;
		
	case 0:
		printf("���� \"%s\" ִ�гɹ�!\n", argv[1]);
		break;
	}
	return ret;
}


int main(int argc, char* argv[])
{
	return process_acc_test(argc, argv);
}
