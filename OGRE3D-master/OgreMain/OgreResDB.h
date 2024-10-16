
#ifndef __OgreResDB_H__
#define __OgreResDB_H__

#ifdef _WIN32
#include "OgrePrerequisites.h"
#else
#define _OgreExport extern
#endif //_WIN32

namespace Ogre
{
	class ResTable
	{
	public:
		virtual int GetRecordSize() = 0;

		/**�ҳ����������ļ�¼�� ���pobjΪNULL�� �ҳ����м�¼�� �����ҳ��Ͳ��ն���pobj��keyһ���ļ�¼
		*@param[in] pobj ���ն���ĵ�ַ
		*@param[out] pfounds ��ȡ����Ĵ洢���飬 ���pfoundsΪNULL, �����ض�������
		*@param[in] max_found ����ȡ���ٸ�
		*@retval <0 ����ʧ��
		*@retval >=0 ʵ�ʵõ��ļ�¼����
		*/
		virtual int FindRecord(void *pfounds, int max_found, const void *pobj) = 0;

		/**�ҳ����������ļ�¼���ú���֧������Ϊunsigned int����
		*@param[in] condition ����
		*@param[in] keyword �ֶ�
		*@param[out] pfounds ��ȡ����Ĵ洢���飬 ���pfoundsΪNULL, �����ض�������
		*@param[in] max_found ����ȡ���ٸ�
		*@retval <0 ����ʧ��
		*@retval >=0 ʵ�ʵõ��ļ�¼����
		*/
		virtual int FindRecord(void *pfounds, int max_found, char* keyword , unsigned int uCondition) = 0;

		/**�ҳ�id�ڷ�Χ[min_id, max_id]�ڵļ�¼�� ������֧��id��table
		*@param[out] pfounds ��ȡ����Ĵ洢���飬 ���pfoundsΪNULL, �����ض�������
		*@param[in] max_found ����ȡ���ٸ�
		*@retval <0 ����ʧ��
		*@retval >=0 ʵ�ʵõ��ļ�¼����
		*/
		virtual int FindRecord(void *pfounds, int max_found, unsigned int min_id, unsigned int max_id) = 0;

		/**����һ����¼(�������˵�ṹ�壩
		*@param[in] pobj �������ĵ�ַ
		*@retval <0 ����ʧ��
		*@retval >=0 ��Ӱ��ļ�¼����
		*/
		virtual int InsertRecord(const void *pobj) = 0;

		/**ɾ����¼(�������˵�ṹ�壩
		*@param[in] pobj ɾ�����ն���ĵ�ַ�� ��ɾ�����к�pobj keyֵһ���ļ�¼
		*@retval <0 ����ʧ��
		*@retval >=0 ��Ӱ��ļ�¼����
		*/
		virtual int DeleteRecord(const void *pobj) = 0;

		/**���¼�¼(�������˵�ṹ�壩
		*@param[in] pobj ���¼�¼�����ݵ�ַ
		*@retval <0 ����ʧ��
		*@retval >=0 ��Ӱ���¼����
		*/
		virtual int UpdateRecord(const void *pobj) = 0;


	protected:
		virtual ~ResTable(){}
	};

	class ResDB
	{
	public:
		virtual ~ResDB(){}

		/**�����ݿ���ȡ��һ����Դ��
		*@param[in] pname ��Դ������֣� �������Դ�ṹ˵���ļ�����Ľṹ���֣� �Լ����ݿ���table����һ��
		*@retval ==NULL û�������Դ��
		*@retval !=NULL ��Դ���ָ��
		*/
		virtual ResTable *GetTable(const char *pname) = 0;
	};

	_OgreExport  ResDB *CreateResDBFromRealDB(const char *metafile, const char *hostip, const char *dbname, const char *user, const char *passwd);
	_OgreExport  ResDB *CreateResDBFromFiles(const char *metafile, const char *dir, void *pre_alloc_buf, unsigned int bufsize);

}


#endif