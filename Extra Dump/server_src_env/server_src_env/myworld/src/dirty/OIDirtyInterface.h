#ifndef __OIDIRTYINTERFACE_H__
#define __OIDIRTYINTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
  Fuction         :  MakeMapCode
  Descption       :  把字符转换为相应的字形映射码和字音映射码
  Input           :  pszCharacter -- 源字符串
                     iMaxCodeLenth-- 输出的映射码字符串的最大限制长度
  Output          :  pszSpellCode -- 转换后的字形映射码字符串
                     pszPinyinCode-- 转换后的字音映射码字符串
  Return          :  0            -- 转换成功
                     1            -- 部分转换成功，输出缓冲区太小
                     -1           -- 输入参数错误
  Note            :  所有参数指针不能为NULL，iMaxCodeLenth须为正值
************************************************************************/
int MakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth);

/************************************************************************
  Fuction         :  InitializeDirtyCheck
  Descption       :  初始化OIDirty，加载配置文件
  Input           :  pszChnKeyFile -- 中文关键字文件名
                     pszEngKeyFile -- 英文关键字文件名
                     pszRegFile    -- 正则表达式文件名
					 pszPinyinFile -- 拼音（关键词和正则表达式）文件名
  Output          :  无
  Return          :  0             -- 初始化成功
                     -1            -- 分配内存空间失败
                     -2            -- 加载英文关键字失败
                     -3            -- 加载中文关键字失败
                     -4            -- 加载正则表达式失败
					 -5            -- 加载拼音文件失败
  Note            :  四个文件名参数指针，至少有一个不为NULL
************************************************************************/
//int InitializeDirtyCheck(char * pszChnKeyFile, char * pszEngKeyFile, char * pszRegFile, char * pszPinyinFile);
int InitializeDirtyCheck(char * pszDirtyFile, int iForce);

/************************************************************************
  Fuction         :  OIDirtyCheck
  Descption       :  综合语句检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合中文关键字
                     2             -- 脏话，符合英文关键字
                     3             -- 脏话，符合正则表达式
                     4             -- 混合检查，中文脏话
                     5             -- 混合检查，英文脏话
                     -1            -- 错误，没有初始化
                     -2            -- 错误，过滤字库无效
                     -100          -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheck(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckChn
  Descption       :  中文关键字检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合中文关键字
                     -1            -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckChn(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckEng
  Descption       :  英文关键字检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合英文关键字
                     -1            -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckEng(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckReg
  Descption       :  正则表达式检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
					 iBeginIdx     -- 关键字起始边界下标，从0计，含
                     iEndIdx       -- 关键字终止边界下标，从0计，不含
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合正则表达式
                     -1            -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckReg(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyCheckPinyin
  Descption       :  拼音检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合拼音限制
                     -1            -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckEx
  Descption       :  综合语句检查-扩展接口
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
                     iBeginIdx     -- 关键字起始边界下标，从0计，含
                     iEndIdx       -- 关键字终止边界下标，从0计，不含
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合中文关键字
                     2             -- 脏话，符合英文关键字
                     3             -- 脏话，符合正则表达式
                     4             -- 混合检查，中文脏话
                     5             -- 混合检查，英文脏话
                     -1            -- 错误，没有初始化
                     -2            -- 错误，过滤字库无效
                     -100          -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyCheckChnEx
  Descption       :  中文关键字检查-扩展接口
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
                     iBeginIdx     -- 关键字起始边界下标，从0计，含
                     iEndIdx       -- 关键字终止边界下标，从0计，不含
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合中文关键字
                     -1            -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyCheckEngEx
  Descption       :  英文关键字检查-扩展接口
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
                     iBeginIdx     -- 关键字起始边界下标，从0计，含
                     iEndIdx       -- 关键字终止边界下标，从0计，不含
  Return          :  0             -- 检查成功，非脏话
                     1             -- 脏话，符合英文关键字
                     -1            -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyReplace
  Descption       :  综合语句检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 将字符串脏字替换为*
                     -1            -- 错误，没有初始化
                     -2            -- 错误，过滤字库无效
                     -100          -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyReplace(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyReplace2
  Descption       :  综合语句检查
  Input           :  pszContent    -- 输入字符串
                     iDoorLevel    -- 门限脏话级别
  Output          :  iDirtyLevel   -- 判断后的脏话级别
  Return          :  0             -- 将字符串脏字替换为*
                     -1            -- 错误，没有初始化
                     -2            -- 错误，过滤字库无效
                     -100          -- 错误，没有初始化
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyReplace2(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyFini
  Descption       :  释放内存
  Input           :  
                     
  Output          :  
  Return          :  
  Note            :  检查之前，需要InitializeDirtyCheck进行初始化
************************************************************************/
int OIDirtyFini();

/************************************************************************
  Fuction         :  InitializeDirtyCheck
  Descption       :  初始化OIDirty，加载配置文件
  Input           :  pszBuff -- 脏字表文件内存映像
  			: iBuff 	-- pszBuff长度
                   
  Output          :  无
  Return          :  0             -- 初始化成功
                     -1            -- 分配内存空间失败
                     -2            -- 加载英文关键字失败
                     -3            -- 加载中文关键字失败
                     -4            -- 加载正则表达式失败
					 -5            -- 加载拼音文件失败
  Note            :  四个文件名参数指针，至少有一个不为NULL
************************************************************************/
int InitializeDirtyCheckEx(char * pszBuff,  int iBuff);

#ifdef __cplusplus
}
#endif

#endif /* __OIDIRTYINTERFACE_H__*/
