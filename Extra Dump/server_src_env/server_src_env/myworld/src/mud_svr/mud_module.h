#ifndef MUD_MODULE_H
#define MUD_MODULE_H

typedef struct tagModule Module;
typedef struct tagMudPlayer MudPlayer;


typedef int (*ModuleHandle)(Module *);
typedef int (*CSHandle)(Module *, MudPlayer *, const CSPKG *);

typedef struct tagSSHandlerEntry SSHandlerEntry;
typedef struct tagCSHandlerEntry CSHandlerEntry;
typedef struct tagDBHandlerEntry DBHandlerEntry;

struct tagSSHandlerEntry
{
	int iMatchCmd; // CMD
	int (*pfnHandle)(Module *, const SSPKG *); // 处理函数
};

struct tagCSHandlerEntry
{
	int iMatchCmd; // 匹配的CS命令
	int iMatchStatus; // 匹配的状态
	CSHandle pfnHandle;
};

struct tagDBHandlerEntry
{
	int iMatchTormCmd; // Torm CMD
	int iMatchCmd; // 自身CMD
	int (*pfnHandle)(Module *, const TORMPKG *); // 处理函数
};

struct tagModule
{
	const char *pszName; // 模块名
	ModuleHandle pfnInit; // 初始化方法
	ModuleHandle pfnFini; // 结束方法
	long iIntervalTime; // Tick间隔时间
	ModuleHandle pfnTick; // Tick处理函数
	SSHandlerEntry *astSSHandlers; // SS协议处理器
	CSHandlerEntry *astCSHandlers; // CS协议处理器
	DBHandlerEntry *astDBHandlers; // DB协议处理器
	void *pvData; // 模块自己的私有数据
	int iTickCount; // Tick使用的数据
};


extern Module gs_ModLogin; // 登录模组
//extern Module gs_ModPlayer; // 角色管理模组
//extern Module gs_ModChat; // 吹水模组
//extern Module gs_ModTraining; // 人人有功练模组


#endif // MUD_MODULE_H
