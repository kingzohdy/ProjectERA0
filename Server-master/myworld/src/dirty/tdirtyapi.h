/*
**  @file $RCSfile: tdirtyapi.h,v $
**  general description of this module
**  $Id: tdirtyapi.h,v 1.3 2013/01/29 11:12:47 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/01/29 11:12:47 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef __TDIRTY_H__
#define __TDIRTY_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
	Fuction			:	tInitializeDirtyCheck
	Descption		:	初始化OIDirty，加载配置文件
	Input			:	pszDirtyFile	--脏字表名称如果为空从Agent初始化
										文件编码gbk字符集
						uiBusid			--业务ID各项目组业务ID不同测试填0
	Output			:	无
	Return			:	0				-- 初始化成功
						-1				-- 分配内存空间失败
						-2				-- 加载英文关键字失败
						-3				-- 加载中文关键字失败
						-4				-- 加载正则表达式失败
						-5				-- 加载拼音文件失败
	Note			:	
************************************************************************/

int tInitializeDirtyCheck(char * pszDirtyFile, unsigned int uiBusid);

/************************************************************************
	Fuction			:	tSetDirtyMd5File
	Descption		:	设置脏字表的md5交互文件名,需要在tInitializeDirtyCheck前调用
	Input			:	pszDirtyMd5File	--脏字表名称如果为空从Agent初始化
	Output			:	无
	Return			:	0				-- 初始化成功
						-1				-- 失败

	Note			:	
************************************************************************/

int tSetDirtyMd5File(char *pszDirtyMd5File);

/************************************************************************
	Fuction			:	tDirtyCheck
	Descption		:	综合语句检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
	Return			:	0				-- 检查成功，非脏话

						1				-- 脏话，符合中文关键字
						2				-- 脏话，符合英文关键字
						3				-- 脏话，符合正则表达式
						4				-- 混合检查，中文脏话
						5				-- 混合检查，英文脏话
						-1				-- 错误，没有初始化
						-2				-- 错误，过滤字库无效
						-100			-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyCheck(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyReplace
	Descption		:	综合语句检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
					:	pszContent		-- 返回过滤字符串，被过滤得字节
										用*代替
	Return			:	0				-- 过滤脏字成功
						-1				-- 错误，没有初始化
						-2				-- 错误，过滤字库无效
						-100			-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyReplace(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyReplace2
	Descption		:	综合语句检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel == iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
					:	pszContent		-- 返回过滤字符串，被过滤得字节
										用*代替
	Return			:	0				-- 过滤脏字成功
						-1				-- 错误，没有初始化
						-2				-- 错误，过滤字库无效
						-100			-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyReplace2(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	utf8DirtyCheck
	Descption		:	综合语句检查
	Input			:	pszContent		-- 输入字符串UTF-8字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
	Return			:	0				-- 检查成功，非脏话

						1				-- 脏话，符合中文关键字
						2				-- 脏话，符合英文关键字
						3				-- 脏话，符合正则表达式
						4				-- 混合检查，中文脏话
						5				-- 混合检查，英文脏话
						-1				-- 错误，没有初始化
						-2				-- 错误，过滤字库无效
						-100			-- 错误，没有初始化
						-200			-- 错误，UTF-8编码错误

	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int utf8DirtyCheck	(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);

/************************************************************************
	Fuction			:	utf8DirtyReplace
	Descption		:	综合语句检查
	Input			:	pszContent		-- 输入字符串UTF-8字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
					:	pszContent		-- 返回过滤字符串，被过滤得字节
										用*代替
	Return			:	0				-- 过滤脏字成功
						-1				-- 错误，没有初始化
						-2				-- 错误，过滤字库无效
						-100			-- 错误，没有初始化
						-200			-- 错误，UTF-8编码错误

	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int utf8DirtyReplace(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtySetAutoUpdate
	Descption		:	设置脏字表自动更新接口，默认自动更新
						
	Input           :	iAutoUpdate		-- 0 不自动更新
										-- 1 自动更新
	Output			:	无
	Return			:	无
	Note			:	设置之前，不需要tInitializeDirtyCheck进行初始化
************************************************************************/

void tDirtySetAutoUpdate(int iAutoUpdate);

/************************************************************************
	Fuction			:	tDirtyUpdateCheck
	Descption		:	自动检查更新脏字表并更新
	Input           :	无
	Output			:	无
	Return			:	0				-- 脏字表无变化
						-1				-- 更新脏字表失败
						1				-- 更新脏字表成功
	Note			:	该功能仅Linux平台有效,windows平台无效
						tDirtySetAutoUpdate设置为自动更新
************************************************************************/

int tDirtyUpdateCheck();

/************************************************************************
	Fuction			:	tGetDirtywordVersion
	Descption		:	获得dirtylib当前加载的脏字表MD5信息
						
	Input           :	无
	Output			:	无
	Return			:	
	Note			:	
************************************************************************/

const char *tGetDirtywordVersion();

/************************************************************************
	Fuction			:	tSetDirtywordVersion
	Descption		:	设置dirtylib当前加载的脏字表MD5信息
						
	Input           :	pszDirtyFile		-- 脏字表文件名
	Output			:	无
	Return			:	
	Note			:	
************************************************************************/


void tSetDirtywordVersion(char* pszDirtyFile);

/************************************************************************
	Fuction			:	tDirtyCheckChn
	Descption		:	中文语句检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
	Return			:	0				-- 检查成功，非脏话
						1				-- 检查成功，符合中文关键字
						-1				-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
						如果存在中英混合关键字可能改变返回pszContent
						改变某些全角字符为半角
************************************************************************/

int tDirtyCheckChn(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckEng
	Descption		:	英文关键字检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
	Return			:	0				-- 检查成功，非脏话
						1				-- 检查成功，符合英文关键字
						-1				-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyCheckEng(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckReg
	Descption		:	正则表达式检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
	Return			:	0				-- 检查成功，非脏话
						1				-- 检查成功，符合正则表达式
						-1				-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
						该功能仅Linux平台有效,windows平台无效
************************************************************************/

int tDirtyCheckReg(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckPinyin
	Descption		:	拼音检查
	Input			:	pszContent		-- 输入字符串gbk字符集
						iDoorLevel		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
										iDirtyLevel >= iDoorLevel的脏字
	Output			:	iDirtyLevel		-- 判断后的脏话级别
	Return			:	0				-- 检查成功，非脏话
						1				-- 检查成功，符合拼音关键字
						-1				-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckEx
	Descption		:	综合语句检查-扩展接口
	Input           :	pszContent		-- 输入字符串gbk字符集
						iDoorLeve		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
	Output			:	iDirtyLevel		-- 判断后的脏话级别
						iBegIdx			-- 关键字起始边界下标，从0计，含0
						iEndIdx			-- 关键字终止边界下标，从0计，不含0
	Return			:	0				-- 检查成功，非脏话
						1				-- 脏话，符合中文关键字
						2				-- 脏话，符合英文关键字
						3				-- 脏话，符合正则表达式
						4				-- 混合检查，中文脏话
						5				-- 混合检查，英文脏话
						-1				-- 错误，没有初始化
						-2				-- 错误，过滤字库无效
						-100			-- 错误，没有初始化
	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
						如果混合检查iBegIdx和iEndIdx始终返回为0
************************************************************************/
int tDirtyCheckEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/************************************************************************
	Fuction			:	tDirtyCheckChnEx
	Descption		:	中文关键字检查-扩展接口
	Input           :	pszContent		-- 输入字符串gbk字符集
						iDoorLeve		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
	Output			:	iDirtyLevel		-- 判断后的脏话级别
						iBegIdx			-- 关键字起始边界下标，从0计，含0
						iEndIdx			-- 关键字终止边界下标，从0计，不含0
	Return			:	0				-- 检查成功，非脏话
						1				-- 脏话，符合中文关键字
						-1				-- 错误，没有初始化

	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/************************************************************************
	Fuction			:	tDirtyCheckEngEx
	Descption		:	英文关键字检查-扩展接口
	Input           :	pszContent		-- 输入字符串gbk字符集
						iDoorLeve		-- 门限脏话级别，填0脏字表中所有
										脏字都可过滤，否则只过滤
	Output			:	iDirtyLevel		-- 判断后的脏话级别
						iBegIdx			-- 关键字起始边界下标，从0计，含0
						iEndIdx			-- 关键字终止边界下标，从0计，不含0
	Return			:	0				-- 检查成功，非脏话
						1				-- 脏话，符合中文关键字
						-1				-- 错误，没有初始化

	Note			:	检查之前，需要tInitializeDirtyCheck进行初始化
************************************************************************/

int tDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/************************************************************************
	Fuction			:	tMakeMapCode
	Descption		:	把字符转换为相应的字形映射码和字音映射码
	Input           :	pszCharacter	-- 源字符串gbk字符集
						iMaxCodeLenth	-- 输出的映射码字符串的最大限制长度

	Output			:	pszSpellCode	-- 转换后的字形映射码字符串
						pszPinyinCode	-- 转换后的字音映射码字符串

	Return			:	0				-- 转换成功
						1				-- 部分转换成功，输出缓冲区太小
						-1				-- 输入参数错误

	Note			:	所有参数指针不能为NULL，iMaxCodeLenth须为正值
************************************************************************/

int tMakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth);

/************************************************************************
	Fuction			:	tDirtyFini
	Descption		:	释放内存
	Input			:	
	Output			:	
	Return			:	

	Note			:	
************************************************************************/

int tDirtyFini();

/************************************************************************
	Fuction			:	utf8_strlen
	Descption		:	utf8字符真实长度计算
	Input			:	s				-- 输入字符串，必须是utf-8编码	
	Output			:	
	Return			:	s utf-8编码长度

	Note			:	
************************************************************************/

size_t utf8_strlen(const char *s);

/************************************************************************
	Fuction			:	utf8_strstr
	Descption		:	utf8字符真实长度计算
	Input			:	s				-- 输入字符串，必须是utf-8编码	
	Output			:	
	Return			:	>0 				-- utf-8编码长度

	Note			:	
************************************************************************/

const char* utf8_strstr(const char *s1, const char *s2, size_t n);

/************************************************************************
	Fuction			:	utf8SpecialCharFilterInit
	Descption		:	特殊字符过滤初始化
	Input			:	piHandle		--	句柄指针
						pszFileName		--	特殊字符文件asci格式
	Output			:	
	Return			:	0				--	初始化成功
						-1				--	初始化失败

	Note			:	
************************************************************************/
int utf8SpecialCharFilterInit(unsigned long int* piHandle, const char * pszFileName);

/************************************************************************
	Fuction			:	utf8SpecialCharFilterScan
	Descption		:	
	Input			:	piHandle		--	句柄
						pszString		--	utf8编码字符串
	Output			:	
	Return			:	1				--	检测到特殊字符
						0				--	没有检测到特殊字符	

	Note			:	
************************************************************************/

int utf8SpecialCharFilterScan(unsigned long int iHandle, const char *pszString);

/************************************************************************
	Fuction			:	utf8SpecialCharFilterFini
	Descption		:	释放句柄
	Input			:	piHandle		--	句柄指针

	Output			:	
	Return			:

	Note			:	
************************************************************************/

void utf8SpecialCharFilterFini(unsigned long int* piHandle);

/************************************************************************
	Fuction			:	tInitializeDirtyCheckEx
	Descption		:	初始化OIDirty，加载配置文件
	Input			:	pszBuff			-- 脏字表内存映像指针必须\0结尾
						iBuff			-- 脏字表内存映像长度
	Output			:	无
	Return			:	0				-- 初始化成功
						-1				-- 分配内存空间失败
						-2				-- 加载英文关键字失败
						-3				-- 加载中文关键字失败
						-4				-- 加载正则表达式失败
						-5				-- 加载拼音文件失败
	Note			:	
************************************************************************/

int tInitializeDirtyCheckEx(char * pszBuff,  int iBuff);

#ifdef __cplusplus
}
#endif

#endif /* __TDIRTY_H__*/
