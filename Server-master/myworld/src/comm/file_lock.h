/***************************************************
// Project	:   comm
// Program 	: 
// FileName : file_lock.h
// Purpose  : 
// Author 	: ouyang@wyyl 2010/11/26
****************************************************/
#ifndef _FILE_LOCK_H_
#define _FILE_LOCK_H_


#ifdef  __cplusplus
extern "C" {
#endif

static const int FILE_LOCK_READ = 1;
static const int FILE_LOCK_WRITE = 2;

void filelock_close(void);

int filelock_init(const char *pszFileName);

/**
* @brief 加文件锁，如果加锁失败，将等待
* @param iOffSet 加锁的偏移量
* @param iSize 加锁的区域大小
* @param iWaitFlag 加锁失败是否等待，默认为等待
*
* @return 0-成功 其他-失败
*/
int filelock_lock(int iType, int iOffSet, int iSize, int iWaitFlag /*= 1*/);

/**
* @brief 解除文件锁
* @param iOffSet 加锁的偏移量
* @param iSize 加锁的区域大小
*
* @return 0-成功 其他-失败
*/
int filelock_unlock(int iOffSet, int iSize);

#ifdef  __cplusplus
}
#endif

#endif

