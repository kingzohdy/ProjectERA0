/*
**  @file $RCSfile: tagentapi.h,v $
**  general description of this module
**  $Id: tagentapi.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:29 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef __TAGENT_API__
#define __TAGENT_API__


#ifdef WIN32
# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */

#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libtagentapi"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libtagentapi.lib")
#endif

#endif/*#ifdef WIN32*/

#ifdef __cplusplus
extern "C" 
{
#endif

#if defined(__i386__) && !defined(_WIN64)
#define uint2korr(A)				(*((uint16_t *) (A)))
#define uint4korr(A)				(*((unsigned long *) (A)))

#define int2store(T,A)				*((uint16_t*) (T))= (uint16_t) (A)
#define int4store(T,A)				*((long *) (T))= (long) (A)
#endif /* __i386__ */

#define PKGHEADLEN				sizeof(tagPkgHead)
#define MSG_MAGIC				(unsigned short)0x8899	//��ͷMagic

#define ID_MSG_HELLO			0x00	//����Э�� agent->centerd or dirtyd->centerd
#define ID_MSG_AGENTREGISTER	0x01	//agentע��Э�� agent->centerd
#define ID_MSG_MASTEREGISTER	0x02	//masterע��Э�� dirtyd->centerd
#define ID_MSG_CENTERDCASCADE	0x03	//centerd����

#define ID_MSG_LOCATIONREQ	0x04	//agent ����centerd��ַ
#define ID_MSG_LOCATIONRES		0x05	//����centerd��ַ

#define ID_MSG_SNAPSHOTREPORT	0x06	//agent�ڵ�����ϱ�

#define ID_MSG_FILEUPDATE		0x0a	//�ļ��·�֪ͨЭ�� dirty->agent
#define ID_MSG_FILEREQUEST		0x0b	//�ļ�����Э�� agent->dirty
#define ID_MSG_FILEPUSH		0x0c	//�ļ��·�Э�� dirty->agent
#define ID_MSG_FILEREPORT		0x0d	//�ļ���ȡ�ɹ�Ӧ�� agent->dirty
#define ID_MSG_CHROMO			0xff	//ȾɫЭ�� ��������ͨ·
#define ID_MSG_TRANSFER		0xfff0	//����͸��

#define ID_CMD_DISPATCH		0x00	//��Ϣ�㲥
#define ID_CMD_UP				0x01	//��Ϣ���ݰ�ͷAppIDת��
#define ID_CMD_DOWN			0x02	//��Ϣ���ݰ�ͷdestIPת��

#define ID_BUSSID_BROADCAST	0xffff	//appid�ڵ���ϢȺ��

#define ERROR_NET_ERROR		(-0x01)
#define ERROR_HEAD_ERROR		(-0x02)
#define ERROR_SIZE_ERROR		(-0x03)

#define ERROR_BUS_NODATA		(-0x11)
#define ERROR_LOGIC				(-0x21)

#define ERROR_FILE_NOEXIST		(-0x31)
#define ERROR_FILE_POSITION	(-0x32)

#define MSG_MAGIC_POS 14

typedef struct tagPkgHead
{
	unsigned short	uLen;
	unsigned short	uMsgCmd;

	unsigned int		uiAppid;
	unsigned int		uiBusid;
	
	unsigned char		cMsgVer;
	unsigned char		cType;
	unsigned short	uMagic;
	
	unsigned int		uiSourceIP;
	unsigned int		uiDestIP;
} tagPkgHead;

#define ID_APPID_CENTERD		(0x00)    //Ĭ��centerd 	appid
#define ID_APPID_DIRTY			(0x01)    //Ĭ��dirtyd  	appid
#define ID_APPID_BUSCONFIG		(0x02)    //Ĭ��busconfig appid
#define ID_APPID_TCONND		(0x03)    //Ĭ��tconnd    appid
#define ID_APPID_PROCMNG		(0x04)    //process manage  appid
#define ID_APPID_TMAC			(0xff)	//Ĭ��tmac		appid
#define TAGENT_DEFAULT_BUSINESS_ID	0	//default business id

#define EXCHANGESIZE			1024*1024
#define EXCBLOCKSIZE			4096

#define EXCHANGEKEY			0xFFEE0001
#define EXCHEADSIZE 			64

typedef struct tagExchnageHead
{
	int		iExchangeSize;
	int		iBlockSize;
	int		iCreateTime;
	int		iMagic;
	int		iUsed;
	int		iVersion;
	
	char		szDesc[EXCHEADSIZE-24];
} EXCHANGEHEAD, *LPEXCHANGEHEAD;

typedef struct tagExchangeBlock
{
	unsigned int	uiAppid;
	unsigned int	uiBusid;
} EXCHANGEBLOCK, *LPEXCHANGEBLOCK;


typedef struct tagExchangeMng 
{
	char*	pShm;					
	void*	aSem;
	
	EXCHANGEHEAD*	pstExcHead;	
} EXCHANGEMNG, *LPEXCHANGEMNG;

/*********************************************************************
  Fuction         	:  agent_api_init
  Descption       	:  ��ʼ��agent APIs
  Input           	:  ppstExcMng		-- ���ָ��ָ��
  Output          	:  ppstExcMng		-- ���ؾ��ʵ��
  Return          	:  0            	-- ע��ɹ�
                      -1           		-- �ڴ������
  Note            	:  ppstExcMng ��ʼ��ΪNULL��agent_api_init�����
  					   �ڴ�
*********************************************************************/
int	agent_api_init(LPEXCHANGEMNG *ppstExcMng);

/*********************************************************************
  Fuction         	:  agent_api_register
  Descption       	:  ע�ᵽbusAgent
  Input           	:  pstExcMng		-- ���ָ��
                       uiAppid			-- Ӧ��ID
			   		   uiBusid			-- ҵ��ID
  Output          	:  ppstExcMng		-- ���ؾ��ʵ��
  Return          	:  0            	-- ע��ɹ�
                      -1           		-- ע��ʧ��
  Note            	:  ppstExcMng ��ʼ��ΪNULL
*********************************************************************/
int	agent_api_register(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);
/*********************************************************************
  Fuction         	:  agent_api_unregister
  Descption       	:  ȡ��ע��
  Input           	:  pstExcMng		-- ���ָ��
                       uiAppid			-- Ӧ��ID
			   		   uiBusid			-- ҵ��ID
  Output          	:  ppstExcMng		-- ���ؾ��ʵ��
  Return          	:  0            	-- ȡ��ע��ɹ�
                      -1           		-- ȡ��ע��ʧ��
  Note            	:  ppstExcMng ��ʼ��ΪNULL
*********************************************************************/
int	agent_api_unregister(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);

/*********************************************************************
  Fuction         	:  agent_api_get_bussid
  Descption       	:  ����appid���bussid
  Input           	:  ppstExcMng		-- ���ָ��
                       uiAppid			-- Ӧ��ID
			   		   astBussID		-- ҵ��ID����
			   		   piSize			-- ҵ��ID����ָ��
  Output          	:  astBussID		-- ҵ��ID
			   		   piBussSize		-- ҵ�� ID����
  Return          	:  0            	-- ��ȡ�ɹ�
                       -1       		-- ��ȡʧ�ܣ�����ԭ����pstExcMng
                       					   û�г�ʼ��
  Note            	:  
*********************************************************************/
int	agent_api_get_bussid(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int astuiBusid[], int *piLen);

/*********************************************************************
  Fuction         	:  agent_api_get_blocks
  Descption       	:  ��ȡ�Ѿ�ע���blocks��Ϣ
  Input           	:  ppstExcMng		-- ���ָ��
                       ppstBlock		-- block����ָ���ָ��
			   		   piBlockSize		-- block����
  Output          	:  ppstBlock		-- block����ָ���ָ��
			   		   piBlockSize		-- block����
  Return          	:  0            	-- ��ȡ�ɹ�
                       -1       		-- ��ȡʧ�ܣ�����ԭ����pstExcMng
                       					   û�г�ʼ��
  Note            	:  ppstBlock ��Ҫfree
*********************************************************************/
int	agent_api_get_blocks(LPEXCHANGEMNG pstExcMng, LPEXCHANGEBLOCK pstBlock[], int *piLen);

/*********************************************************************
  Fuction         	:  agent_api_get_freebufptr
  Descption       	:  ����appid��bussid����freebufָ��
  Input           	:  ppstExcMng		-- ���ָ��
                       uiAppid			-- Ӧ��ID
			   		   uBussSize		-- ҵ��ID
  Output          	: 
  Return          	:  0            	-- ��ȡ�ɹ�
                       -1       		-- ��ȡʧ��
  Note            	:  
*********************************************************************/
char	*agent_api_get_freebufptr(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);


/*********************************************************************
	Fuction         	:  agent_api_destroy
	Descption       	:  ע�����
	Input           	:  pstExcMng		-- ���
	Output          	:  
	Return          	: 
	Note            	:  
*********************************************************************/

void	agent_api_destroy(LPEXCHANGEMNG pstExcMng);


#ifdef __cplusplus
}
#endif

#endif /* __TAGENT_API__*/
