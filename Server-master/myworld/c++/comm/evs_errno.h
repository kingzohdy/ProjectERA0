/******************************************************************************************
FileName            : evs_appframe.h
Author              : markouyang(ouyang jiangping)//Author name here
Version             : 
Date Of Creation    : 2009��12��16��
Description         :
					  vb.net�еĳ�������ʹ��excetions������ON Error GOTO�﷨��ʵ�ֵģ�ON Error
					  GOTO�����Ѿ���֧���ˣ�Visual Basic�����ṩһ���ṹ���ĳ�������̣�ʹ��Try.
					  ..Catch...Finally�������ṹ���ĳ���������Ǻ�exceptions�йصģ�������Select 
					  Case����While��һ������ѡ���ϵ�Ĵ���ṹ����ʹ�ó���Ĵ�������н�׳�ԣ�����
					  ���и��Ӹ��ӵĴ�����exceptions�����л����������ģ����д������ʱ������
					  ����̾ͻ����ε�ʹ����Tryģ���в�����������д��룬����ʹ��Catch��������ȡ��
					  ��

Others              :  

Function List       : 
1.  ......
Modification History: 
1.Date  :
Author  :
Modification  : 
******************************************************************************************/
#ifndef __ERRNO_H__
#define __ERRNO_H__
#include <iostream>

namespace evslib
{
class EVS_SYSRET
{
public:
	EVS_SYSRET(){};
	~EVS_SYSRET(){};
public:
	//�����붨��
	const static  int RET_SYSTEM_ERR				= -1;
	const static  int RET_SUCC						= 0;//�ɹ�
	const static  int RET_ERROR						= 1;//ʧ��
	const static  int RET_NOENOUGHNODE				= 2;//û���㹻�Ľڵ�
	const static  int RET_READFILEFAIL				= 3;//���ļ�ʧ��
	const static  int RET_NOFINDCMD					= 4;//û�ҵ���Ӧ��������
	const static  int RET_ENDECODE					= 5;//�ӽ���ʧ��
	const static  int RET_OPEN_FILE					= 6;//���ļ�ʧ��
	const static  int RET_FILE_STAT					= 7;//�ļ�״̬����
	const static  int RET_FILE_SIZE					= 8;//�ļ���С����
	const static  int RET_MMAP_FAILED				= 9;//mmap�ļ�ʧ��
	const static  int RET_FILE_SEEK					=10;//seek�ļ�ʧ��
	const static  int RET_FILE_WRITE				=11;//д�ļ�ʧ��
	const static  int RET_UNMAP_FAILED				=12;//unmap�ļ�ʧ��
	const static  int RET_SYNC_MODE_CHANGE			=13;//mmap�ļ���дģʽ�ĸı�
	const static  int RET_SEM_LOCK_INIT				=14;//��ʼ��sem V��ʧ��
	const static  int RET_SEM_LOCK					=15;//sem����ʧ��
	const static  int RET_LOCK_RELEASE				=16;//sem����ʧ��
	const static  int RET_LOCK_STATE				=17;//sem��״̬ʧ��
	const static  int RET_QUEUE_SEMLOCK				=18;//queue����ʧ��
	const static  int RET_QUEUE_SHMAT_SHM			=19;//shmatʧ��
	const static  int RET_QUEUE_GET_SHM				=20;//get shmʧ��
	const static  int RET_QUEUE_FULL				=21;//queue����
	const static  int RET_QUEUE_EMPTY				=22;//queue����
	const static  int RET_CREATE_FILE_LOCK			=23;//�����ļ���ʧ��
	const static  int RET_SHM_EXIST					=24;//�����ڴ��Ѿ�����
	const static  int RET_POOL_ILLEGAL_VARIABLE		=25;//�Ƿ�����
	const static  int RET_POOL_IN_BUF_IS_SMALL		=26;//�����buf̫С
	const static  int RET_POOL_IN_KEY_IS_ILLEGAL	=27;//�����store_key�ǷǷ���
	const static  int RET_POOL_KEY_HAD_RELEASE		=28;//store_key�Ѿ����ͷ���
	const static  int RET_POOL_DEFRAG_IS_TOO_LARGE	=29;//�������Ƭ��ֵ̫����
	const static  int RET_POOL_NO_BIG_MEMBUF		=30;//û���㹻����ڴ���������ˣ�
	const static  int RET_POOL_NO_DEFRAG			=31;//û�д�����Ƭ�ɷ�����
	const static  int RET_SHMHASH_GETSHM_ERR		=32;//hash��ȡ�����ڴ�ʧ��
	const static  int RET_SHMHASH_ATTACHSHM_ERR		=33;//hash attach shmʧ��
	const static  int RET_SHMHASH_NO_NODE_ERR		=34;//hash mapû�нڵ���
	const static  int RET_SHMHASH_INSERT_NODE_ERR	=35;//hash map ����ڵ�ʧ��
	const static  int RET_SHMHASH_NODE_EXIST		=36;//���û��ڵ��Ѿ�����
	const static  int RET_SHMHASH_NO_USERNODE		=37;//û���û��ڵ�
	const static  int RET_SHMHASH_NO_FREENODE		=38;//û�п��нڵ�
	const static  int RET_INIFILE_SECTION			=39;//ȡ�����ֶγ�����		
	const static  int RET_SEPARATOR_FAIL			=40;//��ͷ�ķָ������
	const static  int RET_CHECKSUM_PKG_FAIL			=41;//checksum���ݰ�ʧ����
	const static  int RET_RECV_MULTI_PKG			=42;//�ܵ�1�����ϵ����ݰ���
	const static  int RET_RECV_SMALL_PKG			=43;//һ���������ݶ�û������
	const static  int RET_CLOSE_FD					=44;//֪ͨ�رո�����


public:
	//�ñȽϱ����Ƚϼ򵥵ķ���,����������ݰ�
	const static char *  ErrString(int _string_id_)
	{
		static char ErrMsg[512];
		memset(ErrMsg,0,sizeof(ErrMsg));

		switch(_string_id_)
		{
		case RET_SUCC:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: success",_string_id_);
			break;
		case RET_ERROR:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: system err",_string_id_);
			break;
		case RET_NOENOUGHNODE:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: no enough shm user`s node",_string_id_);
			break;
		case RET_READFILEFAIL:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: conf file is not valid",_string_id_);
			break;
		case RET_NOFINDCMD:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: not find cmd",_string_id_);
			break;
		case RET_ENDECODE:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: encode or decode fail",_string_id_);
			break;
		default:
			snprintf(ErrMsg,sizeof(ErrMsg),"error C%d: not find err code",_string_id_);
			break;

		}

		return ErrMsg;
	}
};
}
#endif	/* __ERRNO_H__ */

