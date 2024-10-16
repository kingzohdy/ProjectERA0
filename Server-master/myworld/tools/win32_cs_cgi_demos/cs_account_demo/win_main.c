#include "cs_cgi.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <commctrl.h>

#define _USE_32BIT_TIME_T
#include <time.h>

#pragma warning(disable:4996)

#define UNUSED(arg) ((void)arg)

struct input_box_param
{
	char title[128];
	int result;
};

struct lookup_thread_param
{
	char key[128];
	HWND hwnd;
	HWND account_list_view;
	int region_id;
};

struct cgi_demo_param
{
	char rolename[ROLE_NAME_LEN + 1];
	int region_id;
	int world_id;
};

struct performance_test_param
{
	char account[ACCOUNT_LOGIN_NAME_LEN + 1];
	int region_id;
	int thread_num;
	int loop_num;
	HWND hwnd;
	HWND performance_list_view;
	HANDLE signal;
};

struct performance_test_unit
{
	const struct performance_test_param *universal;
	int index;
	double time;
	int state;
};

static int init_account_list_view(HWND hwnd);
static void init_status_bar(HWND hwnd);
static int add_account_list_view(HWND hwnd, int region_id, const SSACCLITEINFO *acc_info);
static int lookup_account_by_loginname(HWND hwnd);
static int limited_lookup_account_by_loginname(HWND hwnd);
static int error_box(const char *hint);
static int int_input_box(HWND hwnd, const char *title, int *inout);
static void lookup_entry(void *args);
static int limited_lookup_entry(void *args);
static void ip2a(unsigned long addr, char *buf, int len);
static void tm2a(const time_t *time, char *buf, int len);
static int init_performance_test_view(HWND hwnd);
static int performance_test_start(HWND hwnd);
static int performance_test_display_init(HWND hwnd, const struct performance_test_param *param);
static int get_performance_test_param(HWND hwnd, struct performance_test_param *param);
static void performance_test_entry(void *args);
static DWORD CALLBACK performance_test_unit_entry(LPVOID args);

static INT_PTR CALLBACK main_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam);

static INT_PTR CALLBACK account_query_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam);

static INT_PTR CALLBACK input_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam);

static INT_PTR CALLBACK demo_show_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam);

static INT_PTR CALLBACK performance_test_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam);


// 性能计数器相关
#define GET_PER_COUNT(large) \
	if(!QueryPerformanceCounter(&large)) \
		memset(&large, 0, sizeof(large))

#define GET_MIL_SECOND(current, last) \
	(((current.QuadPart - last.QuadPart) / (double)g_frequency.QuadPart) * 1000.0f)


int *g_cs_handle = NULL;
HINSTANCE g_inst = NULL;
HMENU g_right_menu = NULL;
HANDLE g_lookup_thread = NULL;


LARGE_INTEGER g_frequency; // 性能计数器的频率

static int error_box(const char *hint)
{
	HWND hwnd = GetActiveWindow();
	MessageBox(hwnd, hint, "错误", MB_ICONEXCLAMATION|MB_OK);
	return -1;
}

static void ip2a(unsigned long addr, char *buf, int len)
{
	if(!addr)
		strncpy(buf, "N/A", len);
	else
		_snprintf(buf, len, "%u.%u.%u.%u",
				  (addr & 0xff),
				  ((addr & 0xff00) >> 8),
				  ((addr & 0xff0000) >> 16),
				  ((addr & 0xff000000) >> 24));
}

static void tm2a(const time_t *time, char *buf, int len)
{
	if(!*time)
	{
		strncpy(buf, "N/A", len);
	}
	else
	{
		struct tm jiff = *localtime(time);
		_snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d",
		          jiff.tm_year + 1900, jiff.tm_mon + 1, jiff.tm_mday,
				  jiff.tm_hour, jiff.tm_min, jiff.tm_sec);
	}
}

static INT_PTR CALLBACK input_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	struct input_box_param *param = 
		(struct input_box_param *)GetWindowLongPtr(hwnd, DWLP_USER);
	BOOL input_ok = FALSE;

	switch(msg)
	{
	case WM_INITDIALOG:
		SetWindowLongPtr(hwnd, DWLP_USER, lParam);
		param = (struct input_box_param *)GetWindowLongPtr(hwnd, DWLP_USER);
		SetWindowText(hwnd, param->title);
		SetDlgItemInt(hwnd, IDC_INPUT, param->result, FALSE);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			param->result = GetDlgItemInt(hwnd, IDC_INPUT, &input_ok, FALSE);
			if(!input_ok)
				return error_box("输入失败!");
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}

	return FALSE;
}

static int int_input_box(HWND hwnd, const char *title, int *inout)
{
	int res = 0;
	struct input_box_param *param = malloc(sizeof(struct input_box_param));

	strncpy(param->title, title, sizeof(param->title));
	param->result = *inout;

	res = DialogBoxParam(g_inst, MAKEINTRESOURCE(IDD_INPUT_BOX), hwnd, input_dialog_proc,
		                 (LPARAM)param);
	if(res == IDOK)
		*inout = param->result;

	free(param);
	return res;
}

static void init_status_bar(HWND hwnd)
{
	int widths[] = { 150, 300, -1 };
	(void)SendMessage(hwnd, SB_SETPARTS,
		              sizeof(widths)/sizeof(widths[0]), (LPARAM)widths);
	(void)SendMessage(hwnd, SB_SETTEXT, (sizeof(widths)/sizeof(widths[0]) - 1)|SBT_NOBORDERS, 0);
}

static int init_account_list_view(HWND hwnd)
{
	LVCOLUMN column;
	unsigned flags = ListView_GetExtendedListViewStyle(hwnd);

	flags |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyle(hwnd, flags);

	memset(&column, 0, sizeof(column));
	column.mask = LVCF_TEXT | LVCF_WIDTH;
	column.cx = 18;
	column.pszText = "*";
	ListView_InsertColumn(hwnd, 0, &column);

	column.cx = 40;
	column.pszText = "区ID";
	ListView_InsertColumn(hwnd, 1, &column);

	column.cx = 100;
	column.pszText = "帐号名";
	ListView_InsertColumn(hwnd, 2, &column);

	column.cx = 100;
	column.pszText = "IP";
	ListView_InsertColumn(hwnd, 3, &column);

	column.cx = 120;
	column.pszText = "时间";
	ListView_InsertColumn(hwnd, 4, &column);
	return 0;
}

static int add_account_list_view(HWND hwnd, int region_id, const SSACCLITEINFO *acc_info)
{
	LVITEM item;
	int inserted = 0;
	char buf[128] = {0};

	memset(&item, 0, sizeof(item));
	item.mask = LVIF_TEXT;
	item.pszText = acc_info->FreezeEndTime ? "F" : "";
	item.iSubItem = 0;
	item.iItem = ListView_GetItemCount(hwnd);
	inserted = ListView_InsertItem(hwnd, &item);
	if(inserted < 0)
		return -1;

	_snprintf(buf, sizeof(buf), "%d", region_id);
	item.pszText = buf;
	item.iSubItem = 1;
	ListView_SetItem(hwnd, &item);

	strncpy(buf, acc_info->Pass9Account, sizeof(buf));
	item.pszText = buf;
	item.iSubItem = 2;
	ListView_SetItem(hwnd, &item);

	ip2a(acc_info->LastLoginIP, buf, sizeof(buf));
	item.pszText = buf;
	item.iSubItem = 3;
	ListView_SetItem(hwnd, &item);

	tm2a((time_t *)&acc_info->LastLoginTime, buf, sizeof(buf));
	item.pszText = buf;
	item.iSubItem = 4;
	ListView_SetItem(hwnd, &item);

	return 0;
}

// 全局查询过程
static void lookup_entry(void *args)
{
	struct lookup_thread_param *param = args;
	FIND_ACCOUNT_HANDLE find_handle = NULL;
	ACCFINDRES result;
	HWND hwnd = param->hwnd;
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	HWND status_bar = GetDlgItem(hwnd, IDC_STATUS_BAR);

	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP_INPUT), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_LIMITED_LOOKUP), FALSE);
	(void)SendMessage(status_bar, SB_SETTEXT, 0, (LPARAM)"查询中...");

	// 打开查询句柄
	if(cs_find_first_account_by_loginname(g_cs_handle, param->key, &find_handle,
		                                  &result, 2000) < 0)
	{
		error_box("查询失败!");
		goto out;
	}
	do // 获取所有查询结果
	{
		int i = 0;
		for(i = 0; i < result.ResNum; i++)
		{
			add_account_list_view(
				account_list_view,
				result.ReginID,
				result.AccountLiteArray + i);
		}
	}while(cs_find_next_account(find_handle, &result) >= 0);
	cs_close_handle(find_handle); // 最后记得关闭句柄

out:
	(void)SendMessage(status_bar, SB_SETTEXT, 0, (LPARAM)"完成");
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP_INPUT), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_LIMITED_LOOKUP), TRUE);
	free(param);
}


// 分区查询过程
static int limited_lookup_entry(void *args)
{
	struct lookup_thread_param *param = args;
	LIMITED_FIND_ACCOUNT_HANDLE find_handle = NULL;
	ACCFINDRES result;
	HWND hwnd = param->hwnd;
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	HWND status_bar = GetDlgItem(hwnd, IDC_STATUS_BAR);
	char msg[128] = {0};
	LARGE_INTEGER last, counter;

	GET_PER_COUNT(last);
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP_INPUT), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_LIMITED_LOOKUP), FALSE);
	_snprintf(msg, sizeof(msg), "查询大区[%d]...", param->region_id);
	(void)SendMessage(status_bar, SB_SETTEXT, 0, (LPARAM)msg);

	// 打开查询句柄
	if(cs_limited_find_first_account_by_loginname(g_cs_handle, param->region_id, param->key,
	                                              &find_handle, &result, 2000) < 0)
	{
		error_box("查询失败!");
		goto out;
	}
	do // 获取所有查询结果
	{
		int i = 0;
		for(i = 0; i < result.ResNum; i++)
		{
			add_account_list_view(
				account_list_view,
				result.ReginID,
				result.AccountLiteArray + i);
		}
	}while(cs_limited_find_next_account(find_handle, &result) >= 0);
	cs_close_handle(find_handle); // 最后记得关闭句柄

out:
	GET_PER_COUNT(counter);
	_snprintf(msg, sizeof(msg), "耗时: %0.4f 毫秒", GET_MIL_SECOND(counter, last));
	(void)SendMessage(status_bar, SB_SETTEXT, 1, msg);

	(void)SendMessage(status_bar, SB_SETTEXT, 0, (LPARAM)"完成");
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP_INPUT), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_LOOKUP), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_LIMITED_LOOKUP), TRUE);
	free(param);
	return 0;
}

// 全局查找帐号信息
static int lookup_account_by_loginname(HWND hwnd)
{
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	struct lookup_thread_param *param = calloc(1, sizeof(struct lookup_thread_param));
	
	if(!param)
		goto cancel;

	param->hwnd = hwnd;
	if(!GetDlgItemText(hwnd, IDC_LOOKUP_INPUT, param->key, sizeof(param->key)))
		goto cancel;

	ListView_DeleteAllItems(account_list_view);
	if(!_beginthread(lookup_entry, 0, param))
	{
		error_box("无法开始查询过程!");
		goto cancel;
	}

	return 0;
cancel:
	if(param)
		free(param);
	return -1;
}

// 分区查找帐号信息
static int limited_lookup_account_by_loginname(HWND hwnd)
{
	int region_id = 1;
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	struct lookup_thread_param *param = calloc(1, sizeof(struct lookup_thread_param));
	
	if(!param)
		goto cancel;

	param->hwnd = hwnd;
	if(!GetDlgItemText(hwnd, IDC_LOOKUP_INPUT, param->key, sizeof(param->key)))
		goto cancel;
	if(int_input_box(hwnd, "请指定一个大区", &region_id) != IDOK)
		goto cancel;
	param->region_id = region_id;

	ListView_DeleteAllItems(account_list_view);
	if(!_beginthread(limited_lookup_entry, 0, param))
	{
		error_box("无法开始查询过程!");
		goto cancel;
	}

	return 0;

cancel:
	if(param)
		free(param);
	return -1;
}

static int freeze_account(HWND hwnd, BOOL will_freeze)
{
	char login_name[128] = {0}, region_id[128] = {0};
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	int selected_index = ListView_GetNextItem(account_list_view, -1, LVNI_SELECTED);
	int seconds = 100;

	if(selected_index < 0)
		return -1;

	if(will_freeze &&
	   int_input_box(hwnd, "请输入冻结时间(秒)", &seconds) != IDOK)
		return -1;

	ListView_GetItemText(account_list_view, selected_index, 1, region_id, sizeof(region_id));
	ListView_GetItemText(account_list_view, selected_index, 2, login_name, sizeof(login_name));

	// 冻结/解冻一个指定的帐号
	if(cs_set_account_freeze(g_cs_handle, atoi(region_id), login_name, will_freeze, seconds, 2000) >= 0)
		MessageBox(NULL, will_freeze ? "冻结成功!" : "解冻成功!", "成功", MB_OK);
	else
		MessageBox(NULL, will_freeze ? "冻结失败!" : "解冻失败!", "错误", MB_ICONEXCLAMATION|MB_OK);
	return 0;
}

static int silence_account(HWND hwnd, BOOL will_silence)
{
	char login_name[128] = {0}, region_id[128] = {0};
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	int selected_index = ListView_GetNextItem(account_list_view, -1, LVNI_SELECTED);
	int seconds = 100;

	if(selected_index < 0)
		return -1;

	if(will_silence &&
	   int_input_box(hwnd, "请输入禁言时间(秒)", &seconds) != IDOK)
		return -1;

	ListView_GetItemText(account_list_view, selected_index, 1, region_id, sizeof(region_id));
	ListView_GetItemText(account_list_view, selected_index, 2, login_name, sizeof(login_name));

	// 禁言/解禁一个指定的帐号
	if(cs_set_account_silence(g_cs_handle, atoi(region_id), login_name, will_silence, seconds, 2000) >= 0)
		MessageBox(NULL, will_silence ? "禁言成功!" : "解禁成功!", "成功", MB_OK);
	else
		MessageBox(NULL, will_silence ? "禁言失败!" : "解禁失败!", "错误", MB_ICONEXCLAMATION|MB_OK);
	return 0;
}

// 将UNIX的End Line转换为Windows的\r\n
static int unix_le_to_win(const char *raw, char *buf, int len)
{
	int i = 0;

	while(*raw && i<len)
	{
		if(*raw == '\n')
			buf[i++] = '\r';
		if (i >= len-1) break;
		buf[i++] = *raw++;
	}

	if (i>=len-1)
	{
		buf[len - 1] = 0;
	}
	else
	{
		buf[i++] = 0;
	}

	return 0;
}

static int get_demo_param(HWND hwnd, struct cgi_demo_param *param)
{
	BOOL ok = FALSE;
	param->region_id = GetDlgItemInt(hwnd, IDC_REGION_ID, &ok, FALSE);
	if(!ok)
		return -1;
	param->world_id = GetDlgItemInt(hwnd, IDC_WORLD_ID, &ok, FALSE);
	if(!ok)
		return -1;
	ok = GetDlgItemText(hwnd, IDC_ROLE_NAME, param->rolename, sizeof(param->rolename));
	if(!ok)
		return -1;
	return 0;
}

// 获取坐骑信息
static int run_demo_1(HWND hwnd, const struct cgi_demo_param *param)
{
	char raw[1024], buf[1024];
	ZONEGETROLERIDEINFORES res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_get_ride_info(g_cs_handle, param->region_id,
		                param->world_id, param->rolename, &res, 2000) < 0)
		return error_box("cs_get_ride_info失败");

	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "");
	cs_sprintf(g_cs_handle, "ZoneGetRoleRideInfoRes", &res, sizeof(res), raw, sizeof(raw));
	unix_le_to_win(raw, buf, sizeof(buf));
	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	return 0;
}

// 获取角色信息
static int run_demo_2(HWND hwnd, const struct cgi_demo_param *param)
{
	char raw[4096], buf[4096];
	ZONEGETROLEDETAILRES res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_get_role_info(g_cs_handle, param->region_id,
		                param->world_id, (char *)param->rolename, &res, 2000) < 0)
		return error_box("cs_get_role_info失败");

	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "");
	cs_sprintf(g_cs_handle, "ZoneGetRoleDetailRes", &res, sizeof(res), raw, sizeof(raw));
	unix_le_to_win(raw, buf, sizeof(buf));
	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	return 0;
}

// 获取角色声望列表
static int run_demo_3(HWND hwnd, const struct cgi_demo_param *param)
{
	char raw[1024], buf[1024];
	RESPACTINFO res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_respact_list(g_cs_handle, param->region_id,
		               param->world_id, param->rolename, &res, 2000) < 0)
		return error_box("cs_respact_list失败");

	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "");
	cs_sprintf(g_cs_handle, "RespactInfo", &res, sizeof(res), raw, sizeof(raw));
	unix_le_to_win(raw, buf, sizeof(buf));
	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	return 0;
}

// 获取角色邮件列表
static int run_demo_4(HWND hwnd, const struct cgi_demo_param *param)
{
	ZONEGETMAILLISTRES res;
	FIND_MAILLIST_HANDLE find = NULL;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_find_first_maillist(g_cs_handle, param->region_id, param->world_id,
	                          param->rolename, &find, &res, 2000) < 0)
		return error_box("cs_respact_list失败");
	do
	{
		char raw[4096], buf[4096];
		SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "");
		cs_sprintf(g_cs_handle, "ZoneGetMailListRes", &res, sizeof(res), raw, sizeof(raw));
		unix_le_to_win(raw, buf, sizeof(buf));
		SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	}
	while(cs_find_next_maillist(find, &res) >= 0);
	cs_close_handle(find);
	return 0;
}

// 只获取角色精灵列表
static int run_demo_5(HWND hwnd, const struct cgi_demo_param *param)
{
	char raw[4096], buf[4096];
	ZONEGETROLEITEMLISTRES res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_get_item_info(g_cs_handle, param->region_id, param->world_id, param->rolename,
		                &res, 2000) < 0)
		return error_box("cs_get_item_info失败");

	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "");
	cs_sprintf(g_cs_handle, "FairyPackage", &res.ZoneRolePackage.FairyPackage,
	           sizeof(res.ZoneRolePackage.FairyPackage), raw, sizeof(raw));
	unix_le_to_win(raw, buf, sizeof(buf));
	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	return 0;
}

// 清除精灵
static int run_demo_6(HWND hwnd, const struct cgi_demo_param *param)
{
	int i = 0;
	char raw[4096], buf[4096];
	ZONEGETROLEITEMLISTRES res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_get_item_info(g_cs_handle, param->region_id, param->world_id, param->rolename,
		                &res, 2000) < 0)
		return error_box("cs_get_item_info失败");

	i = res.ZoneRolePackage.FairyPackage.FairyNum;
	while(i--)
		if(cs_del_fairy_by_id(g_cs_handle, param->region_id, param->world_id, param->rolename,
		                      res.ZoneRolePackage.FairyPackage.RoleFairys[i].FairyID, NULL, 2000) < 0)
			return error_box("cs_del_fairy_by_id失败");

	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "All fairies has been deleted.");
	return 0;
}

// 获取被删除的物品/宠物
static int run_demo_7(HWND hwnd, const struct cgi_demo_param *param)
{
	char raw[4096], buf[4096];
	ZONEGETROLEDROPRES res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));
	if(cs_get_role_drop_info(g_cs_handle, param->region_id, param->world_id, param->rolename,
		                     &res, 2000) < 0)
		return error_box("cs_get_role_drop_info失败");

	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, "");
	cs_sprintf(g_cs_handle, "ZoneGetRoleDropRes", &res, sizeof(res), raw, sizeof(raw));
	unix_le_to_win(raw, buf, sizeof(buf));
	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	return 0;
}

static int run_demo_8(HWND hwnd, const struct cgi_demo_param *param)
{
	char buf[4096];
	ZONEROLELOGOUTRES res;
	HWND output_wnd = GetDlgItem(hwnd, IDC_DEMO_SHOW_OUTPUT);
	if(!output_wnd)
		return -1;

	memset(&res, 0, sizeof(res));

	if (cs_role_kick_off(g_cs_handle, param->region_id, param->world_id, param->rolename,
	                     &res, 2000) < 0)
		return error_box("cs_role_kick_off失败");

	_snprintf(buf, sizeof(buf), "role: %s has been", param->rolename);
	SetDlgItemText(hwnd, IDC_DEMO_SHOW_OUTPUT, buf);
	return 0;
}

static DWORD CALLBACK performance_test_unit_entry(LPVOID args)
{
	struct performance_test_unit *unit = args;
	int i, inserted = 0;
	DWORD rv = 0;
	char buf[128] = {0};
	LARGE_INTEGER last, current;

	ListView_SetItemText(
		unit->universal->performance_list_view,
		unit->index, 1, "准备");

	rv = WaitForSingleObject(unit->universal->signal, INFINITE);

	ListView_SetItemText(
		unit->universal->performance_list_view,
		unit->index, 1, "运行");

	GET_PER_COUNT(last);
	for(i = 0; i < unit->universal->loop_num; ++i)
	{
		SSACCINFO res;

		unit->state = cs_get_account_info(
			g_cs_handle,
			unit->universal->region_id,
			unit->universal->account,
			&res,
			4000);
		if(unit->state < 0)
			goto error;
	}
	GET_PER_COUNT(current);

	ListView_SetItemText(
		unit->universal->performance_list_view,
		unit->index, 1, "结束");

	unit->time = GET_MIL_SECOND(current, last);

	_snprintf(buf, sizeof(buf), "%0.4f毫秒", unit->time);
	ListView_SetItemText(
		unit->universal->performance_list_view,
		unit->index, 3, buf);

final:
	_snprintf(buf, sizeof(buf), "%d", unit->state);
	ListView_SetItemText(
		unit->universal->performance_list_view,
		unit->index, 2, buf);
	return 0;

error:
	ListView_SetItemText(
		unit->universal->performance_list_view,
		unit->index, 1, "失败");
	goto final;
	return 0;
}

// 性能测试线程
static void performance_test_entry(void *args)
{
	struct performance_test_param *param = args;
	HANDLE *threads = calloc(param->thread_num, sizeof(HANDLE));
	struct performance_test_unit *units = 
		calloc(param->thread_num, sizeof(struct performance_test_unit));
	int i, real_thread_num = 0;
	DWORD rv = 0;
	LARGE_INTEGER last, current;

	EnableWindow(GetDlgItem(param->hwnd, IDC_START), FALSE);
	param->signal = CreateEvent(NULL, TRUE, FALSE, NULL);

	for(i = 0; i < param->thread_num; ++i)
	{
		units[i].universal = param;
		units[i].index = i;
		units[i].state = 0;
		units[i].time = 0.0f;

		threads[i] = CreateThread(NULL, 0, performance_test_unit_entry,
		                          units + i, CREATE_SUSPENDED, NULL);
		if(!threads[i])
			goto out;
		ResumeThread(threads[i]);
		++real_thread_num;
	}
	SetEvent(param->signal);

	GET_PER_COUNT(last);
	rv = WaitForMultipleObjects(param->thread_num, threads, TRUE, INFINITE);
	GET_PER_COUNT(current);

	if(rv == WAIT_OBJECT_0)
	{
		char buf[128] = {0};
		double total = 0.0f;
		_snprintf(buf, sizeof(buf), "%0.4f毫秒", GET_MIL_SECOND(current, last));
		SetDlgItemText(param->hwnd, IDC_TOTAL_TIME, buf);

		for(i = 0; i < param->thread_num; ++i)
			total += units[i].time;
		_snprintf(buf, sizeof(buf), "%0.4f毫秒", total/param->thread_num);
		SetDlgItemText(param->hwnd, IDC_AVGE_TIME, buf);
	}

out:
	for(i = 0; i < real_thread_num; ++i)
	{
		DWORD exit_code = 0;
		CloseHandle(threads[i]);
	}
	if(threads)
		free(threads);
	if(units)
		free(units);
	EnableWindow(GetDlgItem(param->hwnd, IDC_START), TRUE);
	if(param->signal)
		CloseHandle(param->signal);
	free(param);
}

static int get_performance_test_param(HWND hwnd, struct performance_test_param *param)
{
	BOOL ok = FALSE;
	param->thread_num = GetDlgItemInt(hwnd, IDC_THREAD_NUM, &ok, FALSE);
	if(!ok)
		return -1;
	if(param->thread_num < 1)
		return error_box("线程数至少要为1");
	if(param->thread_num > MAXIMUM_WAIT_OBJECTS)
		return error_box("当前操作系统只支持同时等待64个内核对象");
	param->loop_num = GetDlgItemInt(hwnd, IDC_LOOP_NUM, &ok, FALSE);
	if(!ok)
		return -1;
	if(param->loop_num < 1)
		return error_box("循环数至少要为1");
	param->region_id = GetDlgItemInt(hwnd, IDC_REGION_ID, &ok, FALSE);
	if(!ok)
		return -1;
	ok = GetDlgItemText(hwnd, IDC_ACCOUNT_NAME, param->account,
	                    sizeof(param->account));
	if(!ok)
		return -1;
	return 0;
}

static int performance_test_display_init(HWND hwnd, const struct performance_test_param *param)
{
	LVITEM item;
	int i, inserted = 0;
	char buf[128] = {0};

	ListView_DeleteAllItems(param->performance_list_view);

	memset(&item, 0, sizeof(item));
	item.mask = LVIF_TEXT;
	item.pszText = buf;
	item.iSubItem = 0;

	for(i = 0; i < param->thread_num; ++i)
	{
		_snprintf(buf, sizeof(buf), "%04d", i);
		item.iItem = i;
		inserted = ListView_InsertItem(param->performance_list_view, &item);
		if(inserted < 0)
			return -1;
	}
	return 0;
}

// 开始性能测试
static int performance_test_start(HWND hwnd)
{
	BOOL ok = FALSE;
	int n = 0;
	struct performance_test_param *param = 
		calloc(1, sizeof(struct performance_test_param));
	SSACCINFO res;

	param->hwnd = hwnd;
	param->performance_list_view = GetDlgItem(hwnd, IDC_THREAD_SHOW);
	if(get_performance_test_param(hwnd, param) < 0)
		goto error;

	// 预先调用一次cs_get_account_info,预测性能测试能否成功
	if(cs_get_account_info(g_cs_handle, param->region_id, param->account, &res, 4000) < 0)
	{
		error_box("预先调用cs_get_account_info失败!");
		goto error;
	}

	if(performance_test_display_init(hwnd, param))
		goto error;

	// 为防止性能测试阻塞UI消息循环,这项操作将在另一线程中进行
	if(!_beginthread(performance_test_entry, 0, param))
	{
		error_box("线程启动失败!");
		goto error;
	}

	return 0;

error:
	if(param)
		free(param);
	return -1;
}

static int init_performance_test_view(HWND hwnd)
{
	LVCOLUMN column;
	unsigned flags = ListView_GetExtendedListViewStyle(hwnd);

	flags |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyle(hwnd, flags);

	memset(&column, 0, sizeof(column));
	column.mask = LVCF_TEXT | LVCF_WIDTH;
	column.cx = 40;
	column.pszText = "序号";
	ListView_InsertColumn(hwnd, 0, &column);

	column.cx = 40;
	column.pszText = "状态";
	ListView_InsertColumn(hwnd, 1, &column);

	column.cx = 40;
	column.pszText = "代码";
	ListView_InsertColumn(hwnd, 2, &column);

	column.cx = 120;
	column.pszText = "时间";
	ListView_InsertColumn(hwnd, 3, &column);
	return 0;
}

static INT_PTR CALLBACK demo_show_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	struct cgi_demo_param demo_param;

	((void)lParam);
	switch(msg)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hwnd, IDC_REGION_ID, 1, FALSE);
		SetDlgItemInt(hwnd, IDC_WORLD_ID, 1, FALSE);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
#define HANDLER(n) \
	case ID_RUN_DEMO_##n: \
		if(get_demo_param(hwnd, &demo_param) >= 0) \
			run_demo_##n(hwnd, &demo_param); \
		break

		HANDLER(1);
		HANDLER(2);
		HANDLER(3);
		HANDLER(4);
		HANDLER(5);
		HANDLER(6);
		HANDLER(7);
		HANDLER(8);

#undef HANDLER
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}

	return FALSE;
}

static INT_PTR CALLBACK performance_test_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	HWND performance_test_view = GetDlgItem(hwnd, IDC_THREAD_SHOW);
	switch(msg)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hwnd, IDC_REGION_ID, 1, FALSE);
		SetDlgItemInt(hwnd, IDC_THREAD_NUM, 1, FALSE);
		SetDlgItemInt(hwnd, IDC_LOOP_NUM, 1, FALSE);
		init_performance_test_view(performance_test_view);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_START:
			performance_test_start(hwnd);
			break;
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}

	return FALSE;
}

static INT_PTR CALLBACK account_query_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	HWND account_list_view = GetDlgItem(hwnd, IDC_ACCOUNT_LIST);
	HWND status_bar = GetDlgItem(hwnd, IDC_STATUS_BAR);
	POINT pt;
	RECT rc;

	switch(msg)
	{
	case WM_INITDIALOG:
		init_account_list_view(account_list_view);
		init_status_bar(status_bar);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LOOKUP:
			lookup_account_by_loginname(hwnd);
			break;
		case IDC_LIMITED_LOOKUP:
			limited_lookup_account_by_loginname(hwnd);
			break;
		case ID_FREEZE_ACCOUNT:
			freeze_account(hwnd, 1);
			break;
		case ID_THAW_ACCOUNT:
			freeze_account(hwnd, 0);
			break;
		case ID_SILENCE_ACCOUNT:
			silence_account(hwnd, 1);
			break;
		case ID_UNSILENCE_ACCOUNT:
			silence_account(hwnd, 0);
			break;
		case IDCANCEL:
			EndDialog(hwnd, (INT_PTR)HIWORD(wParam));
			break;
		}
		return TRUE;

	case WM_CONTEXTMENU:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		GetWindowRect(account_list_view, &rc);
		if(PtInRect(&rc, pt) &&
		   ListView_GetNextItem(account_list_view, -1, LVNI_SELECTED) >= 0)
			TrackPopupMenu(g_right_menu, TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}

	return FALSE;
}

static INT_PTR CALLBACK main_dialog_proc(
	HWND hwnd, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_SHOW_DEMOS:
			ShowWindow(hwnd, SW_HIDE);
			DialogBoxParam(g_inst, MAKEINTRESOURCE(IDD_DEMOE_SHOW), hwnd,
			               demo_show_dialog_proc, 0);
			ShowWindow(hwnd, SW_SHOW);
			break;
		case IDC_SHOW_PERFORMANCE_TEST:
			ShowWindow(hwnd, SW_HIDE);
			DialogBoxParam(g_inst, MAKEINTRESOURCE(IDD_PERFORMANCE_TEST), hwnd,
			               performance_test_dialog_proc, 0);
			ShowWindow(hwnd, SW_SHOW);
			break;
		case IDC_SHOW_ACCOUNT_QUERY:
			ShowWindow(hwnd, SW_HIDE);
			DialogBoxParam(g_inst, MAKEINTRESOURCE(IDD_ACCOUNT_QUERY), hwnd,
			               account_query_dialog_proc, 0);
			ShowWindow(hwnd, SW_SHOW);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}

INT_PTR WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd)
{
	INT_PTR res = 0;

	UNUSED(prev_instance);
	UNUSED(cmd_line);
	UNUSED(show_cmd);

	// 务必将"proto_ss.tdr"和"cs_cgi.xml"，放置在当前目录
	// 否则不能启动接口cs_cgi接口
	if(init_interface("proto_ss.tdr", &g_cs_handle, "cs_cgi.xml"))
		return error_box("CS CGI 接口环境初始化失败!");

	if(!QueryPerformanceFrequency(&g_frequency))
		return error_box("你的硬件不支持高精度性能计数器!");

	CoInitialize(NULL);
	InitCommonControls();

	g_inst = instance;
	g_right_menu = LoadMenu(instance, MAKEINTRESOURCE(IDR_RIGHT_MENU));
	g_right_menu = GetSubMenu(g_right_menu, 0);

	res = (INT_PTR)DialogBox(instance, MAKEINTRESOURCE(IDD_MAIN_SWITCHER), NULL, main_dialog_proc);
	// 别忘记关闭句柄
	destory_interface(g_cs_handle);

	CoUninitialize();
	return res;
}