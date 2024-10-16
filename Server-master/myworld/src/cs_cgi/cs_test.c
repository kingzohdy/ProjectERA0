#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cs_cgi.h"
#include "mysql/mysql.h"

struct test_routine_entry
{
	const char *name; // 例程名字
	int argc; // 需要的参数个数
	int (*route)(int *, int, char *[]); // 回调函数
	const char *des; // 描述信息
	const char *man; // 手册
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
		"获取帮助。", 
		"用法 : cs_test help [命令]",
	},
	
	{
		"get-acc-info", 
		2, test_get_acc_info,
		"根据帐号ID获取帐号信息。", 
		"用法 : cs_test get-acc-info 区ID 帐号名",
	},

	{
		"set-acc-uin-flag",
		3, test_set_acc_uin_flag,
		"设置帐号的Uin Flag。",
		"用法 : cs_test set-acc-uin-flag 大区ID 帐号名 UinFlag",
	},
	 
	{
		"find-acc-by-rolename",
		1, test_find_acc_by_rolename,
		"通过角色名查找帐号信息，支持通配符模糊查询。",
		"用法 : cs_test find-acc-by-rolename 角色名或通配字符串\n"
		"通配符说明 : '%' 表示0~任意多个字符，'_'表示一个任意字符。",
	},
	
	{
		"find-acc-by-loginname",
		1, test_find_acc_by_loginname,
		"通过帐号名查找帐号信息，支持通配符模糊查询。",
		"用法 : cs_test find-acc-by-loginname 帐号名或通配字符串\n"
		"通配符说明 : '%' 表示0~任意多个字符，'_'表示一个任意字符。",
	},

	{
		"limited-find-acc-by-loginname",
		2, test_limited_find_acc_by_loginname,
		"查询帐号的ID，支持通配符模糊查询。",
		"用法 : cs_test limited-find-acc-by-loginname 区ID 角色名或通配字符串\n"
		"通配符说明 : '%' 表示0~任意多个字符，'_'表示一个任意字符。",
	},
	
	{
		"set-acc-silence",
		4, test_set_acc_silence,
		"设置或解除帐号禁言。",
		"用法 : cs_test get-acc-info 大区ID 帐号名 1(禁言)/0(解除) 禁言时间\n"
		"说明 : 时间单位为秒，时间=-1则永久禁言，解除禁言时将忽略禁言时间。",
	},
	
	{
		"set-acc-freeze",
		4, test_set_acc_freeze,
		"设置或解除帐号冻结。",
		"用法 : cs_test set-acc-freeze 大区ID 帐号名 1(冻结)/0(解冻) 冻结时间\n"
		"说明 : 时间单位为秒，时间=-1则永久冻结，解除冻结时将忽略冻结时间。",
	},
	
	{
		"find-role-by-rolename",
		3, test_find_role_by_rolename,
		"通过角色名查询角色信息。",
		"用法 : cs_test find-role-by-rolename 大区ID 服ID 角色名或通配符字符串\n"
		"通配符说明 : '%' 表示0~任意多个字符，'_'表示一个任意字符。",
	},

	{
		"find-role-by-loginname",
		3, test_find_role_by_loginname,
		"通过帐号名获取角色信息。",
		"用法 : cs_test find-role-by-rolename 大区ID 服ID 帐号名或通配符字符串\n"
		"通配符说明 : '%' 表示0~任意多个字符，'_'表示一个任意字符。",
	},
	
	{
		"restore-deleted-role",
		3, test_restore_deleted_role,
		"恢复被删除的角色。",
		"用法 : cs_test restore-deleted-role 区ID 服ID 角色ID",
	},
	
	{
		"get-role-maillist",
		3, test_get_role_maillist,
		"获取某个角色的所有邮件头信息。",
		"用法 : cs_test get-role-maillist 区ID 服ID 角色名",
	},
	
	{
		"get-role-maildata",
		4, test_get_role_maildata,
		"获取某个角色的某封邮件的详细信息。",
		"用法 : cs_test get-role-maildata 区ID 服ID 角色名 邮件ID",
	},
	
	{
		"del-role-mail",
		4, test_del_role_mail,
		"删除某个角色的某封邮件。",
		"用法 : cs_test del-role-mail 区ID 服ID 角色名 邮件ID",
	},
	
	{
		"send-sys-mail",
		6, test_send_sys_mail,
		"给某个玩家发送一封系统邮件。",
		"用法 : cs_test send-sys-mail 区ID 服ID 收件人角色名 发件人名 标题 正文\n"
		"说明 : \"收件人角色名\" 必须是真实存在的角色名, \"发件人名\"可随意填写。",
	},
	
	{
		"copy-role",
		6, test_copy_role,
		"将一个角色所有信息复制到另一已存在的角色。",
		"用法 : cs_test copy-role 区ID 服ID 目标角色帐号ID 目标角色ID 目标角色名 "
		"源角色帐号ID 源角色ID 源角色名\n"
		"说明 : 目标角色必须和源角色在同一服务器，目标角色必须离线才能施行此操作。",
	},
	
	{
		"get-bulletin-list",
		2, test_get_bulletin_list,
		"获取某个服务器的公告信息列表。",
		"用法 : cs_test get-bulletin-list 区ID 服ID",
	},
	
	{
		"add-bulletin",
		5, test_add_bulletin,
		"向某个服务器添加一条公告信息。",
		"用法 : cs_test add-bulletin 区ID 服ID 公告正文 滚动间隔时间 持续时间\n"
		"说明 : 时间的单位都是秒，\"持续时间\"不能小于\"滚动间隔时间\"。",
	},
	
	{
		"del-bulletin",
		3, test_del_bulletin,
		"从某个服务器中删除一条公告信息。",
		"用法 : cs_test del-bulletin 区ID 服ID 公告ID",
	},
	
	{
		"get-skill-info",
		4, test_get_skill_info,
		"获取任务技能信息。",
		"用法 : cs_test get-skill-info 区ID 服ID 角色ID 角色名",
	},
	
	{
		"get-item-info",
		4, test_get_item_info,
		"获取人物物品信息。",
		"用法 : cs_test get-item-info 区ID 服ID 角色ID 角色名",
	},

	{
		"add-role-designation",
		4, test_add_role_designation,
		"增加一个角色称号。",
		"用法 : add-role-designation 区ID 服ID 角色名 称号ID",
	},

	{
		"del-role-designation",
		4, test_del_role_designation,
		"删除一个角色称号。",
		"用法 : del-role-designation 区ID 服ID 角色名 称号ID",
	},

	{
		"get-role-designation-list",
		3, test_get_role_designation_list,
		"获取角色称号列表。",
		"用法 : get-role-designation-list 区ID 服ID 角色名",
	},

	{
		"set-role-respact",
		5, test_set_role_respact,
		"增加角色声望值。",
		"用法 : set-role-respact 区ID 服ID 角色名 声望阵营ID 增加的声望值",
	},

	{
		"del-role-respact",
		4, test_del_role_respact,
		"删除角色声望。",
		"用法 : del-role-respact 区ID 服ID 角色名 声望阵营ID",
	},

	{
		"get-role-respact-list",
		3, test_get_role_respact_list,
		"获取角色声望列表。",
		"用法 : get-role-respact-list 区ID 服ID 角色名",
	},
	
	{
		"query-account-point",
		2, test_query_account_point,
		"获取帐号的点券数",
		"用法 : query-account-point 区ID 帐号",
	},
	
	{
		"cash-props-gift",
		4, test_cash_props_gift_info,
		"冲值送礼到大区",
		"用法 : cash-props-gift 区ID 帐号 礼包id 礼包数",
	},

	{
		"get-clan-all-info",
		3, test_get_clan_all_info,
		"获取指定公会的全部信息",
		"用法 : get-clan-all-info 区ID 服ID 公会名"
	},

	{
		"del-clan-member",
		4, test_del_clan_member,
		"从指定的公会中删除一个指定的会员",
		"用法 : del-clan-member 区ID 服ID 公会名 角色名",
	},

	{
		"disband-clan",
		3, test_disband_clan,
		"解散一个指定的公会",
		"用法 : disband-clan 区ID 服ID 公会名",
	},

	{
		"clear-clan-diplomatism",
		3, test_clear_clan_diplomatism,
		"清除一个公会的外交关系",
		"用法 : clear-clan-diplomatism 区ID 服ID 公会名",
	},

	{
		"get-package-max-num",
		4, test_get_package_max_num,
		"获取指定背包的解锁格子数量",
		"用法 : get-package-max-num 区ID 服ID 角色名背包类型",
	},

	{
		"set-package-max-num",
		5, test_set_package_max_num,
		"增加指定背包的解锁格子数量",
		"用法 : set-package-max-num 区ID 服ID 角色名 背包类型 增加的格子数",
	},
	
	{
		"get-package-item-info",
		4, test_get_package_item_info,
		"查询商城临时(指定)背包中的物品",
		"用法 : get-package-item-info 区ID 服ID 角色名 背包类型",
	},

	{
		"get-role-drop-info",
		3, test_get_role_drop_info,
		"获取角色丢弃的数据",
		"用法 : get-role-drop-info 区ID 服ID 角色名",
	},

	{
		"role-drop-restore",
		3, test_role_drop_restore,
		"获取角色丢弃的数据",
		"用法 : role-drop-restore 区ID 服ID 角色名",
	},

	{
		"role-hatch-pet",
		3, test_role_hatch_pet,
		"获取角色丢弃的数据",
		"用法 : role-hatch-pet 区ID 服ID 角色名",
	},

	{
		"find-clan-by-name",
		3, test_find_clan_by_name,
		"模糊查询公会名",
		"用法 : find-clan-by-name 区ID 服ID 公会名或通配符字符串\n"
		"通配符说明 : '%' 表示0~任意多个字符，'_'表示一个任意字符。",
	},

	{
		"del-clan-diplomatism",
		4, test_del_clan_diplomatism,
		"删除一个指定的公会外交关系",
		"用法 : del-clan-diplomatism 区ID 服ID 公会名 目标公会名。",
	},

	{
		"exchange-clan-leader",
		4, test_exchange_clan_leader,
		"将公会的会长职位交换给指定角色",
		"用法 : exchange-clan-leader 区ID 服ID 公会名 目标角色名。",
	},

	{
		"set_clan_base_attr", 
		7, test_set_clan_base_attr,
		"设置一个公会的基本资源,金,定,繁,科", 
		"用法 : cs_test set_clan_base_attr  区ID, 服ID , 公会名, 金钱\n"
        ", 安定度, 繁荣度, 科技点数\n"
		"说明 :",
	},

	{
		"analy_interface", 
		0, test_analy_interface,
		"解析Blob数据", 
		"用法 : cs_test analy_interface ",
	},
		
	{
		"offline_add_item", 
		5, test_offline_add_item,
		"给离线角色加物品", 
		"用法 : cs_test offline_add_item 区ID 服ID 目标角色名 物品ID 物品数量",
	},	

	{
		"find_all_role", 
		0, test_find_all_role,
		"获取所有区模糊查询角色名的其他部分", 
		"用法 : cs_test find_all_role 角色名\n",
	},

	{
		"reload_server", 
		0, test_reload_server,
		"指定服务器reload", 
		"用法 : cs_test reload_server  区ID, 服ID , reload返回信息\n"
		"说明 :",
	},

	{
		"set_second_pwd",
		4,test_set_second_pwd,
		"设置角色二级密码",
		"用法  ：cs_test set_second_pwd 区ID 服ID 角色名字 密码",
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

//104 版的 AccountSilenceEndTime
struct OLD_VERSION 
{
	long lVersion;
	long lSize;
	int Uin;                            
    int SilenceEndTime;                  	/*   禁言结束时间 */
};
//181 版的 AccountSilenceEndTime
struct NEW_VERSION 
{
	long lVersion;
	long lSize;
	int Uin;                            
    int SilenceEndTime;                  	/*   禁言结束时间 */
    int SilenceStartTime;                	/*  Ver.181 什么时候开始禁言的 */
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
	
	printf("新公告ID : %d\n", id);
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
		puts("\n用法 : cs_test 命令 [[[参数1 [[参数2] 参数3] ... ]");
		puts("命令一览 : ");
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
				printf("\n命令 : %s\n描述 : %s\n", curr->name, curr->des);
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
		printf("初始化失败!\n");
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
		printf("参数错误.\n");
		if(curr->man)
			printf("%s\n", curr->man);
		break;

	case -3:
		printf("操作失败.\n");
		break;
		
	case -2:
		printf("接口调用失败.\n");
		break;
		
	case -1:
		printf("指定的命令错误.\n");
		print_all_des();
		break;
		
	case 0:
		printf("命令 \"%s\" 执行成功!\n", argv[1]);
		break;
	}
	return ret;
}


int main(int argc, char* argv[])
{
	return process_acc_test(argc, argv);
}
