
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

		/**找出满足条件的记录， 如果pobj为NULL， 找出所有记录， 否则找出和参照对象pobj的key一样的记录
		*@param[in] pobj 参照对象的地址
		*@param[out] pfounds 获取对象的存储数组， 如果pfounds为NULL, 不返回对象内容
		*@param[in] max_found 最大获取多少个
		*@retval <0 处理失败
		*@retval >=0 实际得到的记录个数
		*/
		virtual int FindRecord(void *pfounds, int max_found, const void *pobj) = 0;

		/**找出满足条件的记录，该函数支持条件为unsigned int类型
		*@param[in] condition 条件
		*@param[in] keyword 字段
		*@param[out] pfounds 获取对象的存储数组， 如果pfounds为NULL, 不返回对象内容
		*@param[in] max_found 最大获取多少个
		*@retval <0 处理失败
		*@retval >=0 实际得到的记录个数
		*/
		virtual int FindRecord(void *pfounds, int max_found, char* keyword , unsigned int uCondition) = 0;

		/**找出id在范围[min_id, max_id]内的记录， 仅适用支持id的table
		*@param[out] pfounds 获取对象的存储数组， 如果pfounds为NULL, 不返回对象内容
		*@param[in] max_found 最大获取多少个
		*@retval <0 处理失败
		*@retval >=0 实际得到的记录个数
		*/
		virtual int FindRecord(void *pfounds, int max_found, unsigned int min_id, unsigned int max_id) = 0;

		/**插入一个记录(对象或者说结构体）
		*@param[in] pobj 插入对象的地址
		*@retval <0 处理失败
		*@retval >=0 受影响的记录个数
		*/
		virtual int InsertRecord(const void *pobj) = 0;

		/**删除记录(对象或者说结构体）
		*@param[in] pobj 删除参照对象的地址， 将删除所有和pobj key值一样的记录
		*@retval <0 处理失败
		*@retval >=0 受影响的记录个数
		*/
		virtual int DeleteRecord(const void *pobj) = 0;

		/**更新记录(对象或者说结构体）
		*@param[in] pobj 更新记录的数据地址
		*@retval <0 处理失败
		*@retval >=0 受影响记录个数
		*/
		virtual int UpdateRecord(const void *pobj) = 0;


	protected:
		virtual ~ResTable(){}
	};

	class ResDB
	{
	public:
		virtual ~ResDB(){}

		/**从数据库里取得一个资源表
		*@param[in] pname 资源表的名字， 必须和资源结构说明文件里面的结构名字， 以及数据库中table名字一致
		*@retval ==NULL 没有这个资源表
		*@retval !=NULL 资源表的指针
		*/
		virtual ResTable *GetTable(const char *pname) = 0;
	};

	_OgreExport  ResDB *CreateResDBFromRealDB(const char *metafile, const char *hostip, const char *dbname, const char *user, const char *passwd);
	_OgreExport  ResDB *CreateResDBFromFiles(const char *metafile, const char *dir, void *pre_alloc_buf, unsigned int bufsize);

}


#endif