/******************************************************************************************
FileName            : evs_appframe.h
Author              : markouyang(ouyang jiangping)//Author name here
Version             : 
Date Of Creation    : 2009年12月16日
Description         :
					  vb.net中的出错处理是使用excetions而不是ON Error GOTO语法来实现的，ON Error
					  GOTO现在已经不支持了，Visual Basic现在提供一个结构化的出错处理过程，使用Try.
					  ..Catch...Finally表述。结构化的出错处理过程是和exceptions有关的，类似于Select 
					  Case或者While的一个具有选择关系的代码结构。这使得程序的代码更具有健壮性，可以
					  进行更加复杂的错误处理。exceptions的运行机理是这样的，当有错误出现时，错误处
					  理过程就会屏蔽掉使得在Try模块中产生错误的运行代码，并且使用Catch表述来获取错
					  误

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
	//出错码定义
	const static  int RET_SYSTEM_ERR				= -1;
	const static  int RET_SUCC						= 0;//成功
	const static  int RET_ERROR						= 1;//失败
	const static  int RET_NOENOUGHNODE				= 2;//没有足够的节点
	const static  int RET_READFILEFAIL				= 3;//读文件失败
	const static  int RET_NOFINDCMD					= 4;//没找到对应的命令字
	const static  int RET_ENDECODE					= 5;//加解密失败
	const static  int RET_OPEN_FILE					= 6;//打开文件失败
	const static  int RET_FILE_STAT					= 7;//文件状态出错
	const static  int RET_FILE_SIZE					= 8;//文件大小出错
	const static  int RET_MMAP_FAILED				= 9;//mmap文件失败
	const static  int RET_FILE_SEEK					=10;//seek文件失败
	const static  int RET_FILE_WRITE				=11;//写文件失败
	const static  int RET_UNMAP_FAILED				=12;//unmap文件失败
	const static  int RET_SYNC_MODE_CHANGE			=13;//mmap文件回写模式的改变
	const static  int RET_SEM_LOCK_INIT				=14;//初始化sem V锁失败
	const static  int RET_SEM_LOCK					=15;//sem加锁失败
	const static  int RET_LOCK_RELEASE				=16;//sem解锁失败
	const static  int RET_LOCK_STATE				=17;//sem锁状态失败
	const static  int RET_QUEUE_SEMLOCK				=18;//queue的锁失败
	const static  int RET_QUEUE_SHMAT_SHM			=19;//shmat失败
	const static  int RET_QUEUE_GET_SHM				=20;//get shm失败
	const static  int RET_QUEUE_FULL				=21;//queue满了
	const static  int RET_QUEUE_EMPTY				=22;//queue空了
	const static  int RET_CREATE_FILE_LOCK			=23;//创建文件锁失败
	const static  int RET_SHM_EXIST					=24;//共享内存已经存在
	const static  int RET_POOL_ILLEGAL_VARIABLE		=25;//非法变量
	const static  int RET_POOL_IN_BUF_IS_SMALL		=26;//传入的buf太小
	const static  int RET_POOL_IN_KEY_IS_ILLEGAL	=27;//传入的store_key是非法的
	const static  int RET_POOL_KEY_HAD_RELEASE		=28;//store_key已经被释放了
	const static  int RET_POOL_DEFRAG_IS_TOO_LARGE	=29;//传入的碎片的值太大了
	const static  int RET_POOL_NO_BIG_MEMBUF		=30;//没有足够大的内存块来分配了！
	const static  int RET_POOL_NO_DEFRAG			=31;//没有磁盘碎片可分配了
	const static  int RET_SHMHASH_GETSHM_ERR		=32;//hash获取共享内存失败
	const static  int RET_SHMHASH_ATTACHSHM_ERR		=33;//hash attach shm失败
	const static  int RET_SHMHASH_NO_NODE_ERR		=34;//hash map没有节点了
	const static  int RET_SHMHASH_INSERT_NODE_ERR	=35;//hash map 插入节点失败
	const static  int RET_SHMHASH_NODE_EXIST		=36;//该用户节点已经存在
	const static  int RET_SHMHASH_NO_USERNODE		=37;//没有用户节点
	const static  int RET_SHMHASH_NO_FREENODE		=38;//没有空闲节点
	const static  int RET_INIFILE_SECTION			=39;//取配置字段出错了		
	const static  int RET_SEPARATOR_FAIL			=40;//包头的分割符不对
	const static  int RET_CHECKSUM_PKG_FAIL			=41;//checksum数据包失败了
	const static  int RET_RECV_MULTI_PKG			=42;//受到1个以上的数据包了
	const static  int RET_RECV_SMALL_PKG			=43;//一个包的数据都没收收完
	const static  int RET_CLOSE_FD					=44;//通知关闭该连接


public:
	//用比较笨，比较简单的方法,输出错误内容吧
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

