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
	int (*pfnHandle)(Module *, const SSPKG *); // ������
};

struct tagCSHandlerEntry
{
	int iMatchCmd; // ƥ���CS����
	int iMatchStatus; // ƥ���״̬
	CSHandle pfnHandle;
};

struct tagDBHandlerEntry
{
	int iMatchTormCmd; // Torm CMD
	int iMatchCmd; // ����CMD
	int (*pfnHandle)(Module *, const TORMPKG *); // ������
};

struct tagModule
{
	const char *pszName; // ģ����
	ModuleHandle pfnInit; // ��ʼ������
	ModuleHandle pfnFini; // ��������
	long iIntervalTime; // Tick���ʱ��
	ModuleHandle pfnTick; // Tick������
	SSHandlerEntry *astSSHandlers; // SSЭ�鴦����
	CSHandlerEntry *astCSHandlers; // CSЭ�鴦����
	DBHandlerEntry *astDBHandlers; // DBЭ�鴦����
	void *pvData; // ģ���Լ���˽������
	int iTickCount; // Tickʹ�õ�����
};


extern Module gs_ModLogin; // ��¼ģ��
//extern Module gs_ModPlayer; // ��ɫ����ģ��
//extern Module gs_ModChat; // ��ˮģ��
//extern Module gs_ModTraining; // �����й���ģ��


#endif // MUD_MODULE_H
