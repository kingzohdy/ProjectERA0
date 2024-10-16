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
* @brief ���ļ������������ʧ�ܣ����ȴ�
* @param iOffSet ������ƫ����
* @param iSize �����������С
* @param iWaitFlag ����ʧ���Ƿ�ȴ���Ĭ��Ϊ�ȴ�
*
* @return 0-�ɹ� ����-ʧ��
*/
int filelock_lock(int iType, int iOffSet, int iSize, int iWaitFlag /*= 1*/);

/**
* @brief ����ļ���
* @param iOffSet ������ƫ����
* @param iSize �����������С
*
* @return 0-�ɹ� ����-ʧ��
*/
int filelock_unlock(int iOffSet, int iSize);

#ifdef  __cplusplus
}
#endif

#endif

