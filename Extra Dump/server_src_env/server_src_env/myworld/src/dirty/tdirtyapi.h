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
	Descption		:	��ʼ��OIDirty�����������ļ�
	Input			:	pszDirtyFile	--���ֱ��������Ϊ�մ�Agent��ʼ��
										�ļ�����gbk�ַ���
						uiBusid			--ҵ��ID����Ŀ��ҵ��ID��ͬ������0
	Output			:	��
	Return			:	0				-- ��ʼ���ɹ�
						-1				-- �����ڴ�ռ�ʧ��
						-2				-- ����Ӣ�Ĺؼ���ʧ��
						-3				-- �������Ĺؼ���ʧ��
						-4				-- ����������ʽʧ��
						-5				-- ����ƴ���ļ�ʧ��
	Note			:	
************************************************************************/

int tInitializeDirtyCheck(char * pszDirtyFile, unsigned int uiBusid);

/************************************************************************
	Fuction			:	tSetDirtyMd5File
	Descption		:	�������ֱ��md5�����ļ���,��Ҫ��tInitializeDirtyCheckǰ����
	Input			:	pszDirtyMd5File	--���ֱ��������Ϊ�մ�Agent��ʼ��
	Output			:	��
	Return			:	0				-- ��ʼ���ɹ�
						-1				-- ʧ��

	Note			:	
************************************************************************/

int tSetDirtyMd5File(char *pszDirtyMd5File);

/************************************************************************
	Fuction			:	tDirtyCheck
	Descption		:	�ۺ������
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
	Return			:	0				-- ���ɹ������໰

						1				-- �໰���������Ĺؼ���
						2				-- �໰������Ӣ�Ĺؼ���
						3				-- �໰������������ʽ
						4				-- ��ϼ�飬�����໰
						5				-- ��ϼ�飬Ӣ���໰
						-1				-- ����û�г�ʼ��
						-2				-- ���󣬹����ֿ���Ч
						-100			-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyCheck(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyReplace
	Descption		:	�ۺ������
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
					:	pszContent		-- ���ع����ַ����������˵��ֽ�
										��*����
	Return			:	0				-- �������ֳɹ�
						-1				-- ����û�г�ʼ��
						-2				-- ���󣬹����ֿ���Ч
						-100			-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyReplace(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyReplace2
	Descption		:	�ۺ������
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel == iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
					:	pszContent		-- ���ع����ַ����������˵��ֽ�
										��*����
	Return			:	0				-- �������ֳɹ�
						-1				-- ����û�г�ʼ��
						-2				-- ���󣬹����ֿ���Ч
						-100			-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyReplace2(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	utf8DirtyCheck
	Descption		:	�ۺ������
	Input			:	pszContent		-- �����ַ���UTF-8�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
	Return			:	0				-- ���ɹ������໰

						1				-- �໰���������Ĺؼ���
						2				-- �໰������Ӣ�Ĺؼ���
						3				-- �໰������������ʽ
						4				-- ��ϼ�飬�����໰
						5				-- ��ϼ�飬Ӣ���໰
						-1				-- ����û�г�ʼ��
						-2				-- ���󣬹����ֿ���Ч
						-100			-- ����û�г�ʼ��
						-200			-- ����UTF-8�������

	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int utf8DirtyCheck	(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);

/************************************************************************
	Fuction			:	utf8DirtyReplace
	Descption		:	�ۺ������
	Input			:	pszContent		-- �����ַ���UTF-8�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
					:	pszContent		-- ���ع����ַ����������˵��ֽ�
										��*����
	Return			:	0				-- �������ֳɹ�
						-1				-- ����û�г�ʼ��
						-2				-- ���󣬹����ֿ���Ч
						-100			-- ����û�г�ʼ��
						-200			-- ����UTF-8�������

	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int utf8DirtyReplace(int iDoorLevel, char * pszUTF8Content, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtySetAutoUpdate
	Descption		:	�������ֱ��Զ����½ӿڣ�Ĭ���Զ�����
						
	Input           :	iAutoUpdate		-- 0 ���Զ�����
										-- 1 �Զ�����
	Output			:	��
	Return			:	��
	Note			:	����֮ǰ������ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

void tDirtySetAutoUpdate(int iAutoUpdate);

/************************************************************************
	Fuction			:	tDirtyUpdateCheck
	Descption		:	�Զ����������ֱ�����
	Input           :	��
	Output			:	��
	Return			:	0				-- ���ֱ��ޱ仯
						-1				-- �������ֱ�ʧ��
						1				-- �������ֱ�ɹ�
	Note			:	�ù��ܽ�Linuxƽ̨��Ч,windowsƽ̨��Ч
						tDirtySetAutoUpdate����Ϊ�Զ�����
************************************************************************/

int tDirtyUpdateCheck();

/************************************************************************
	Fuction			:	tGetDirtywordVersion
	Descption		:	���dirtylib��ǰ���ص����ֱ�MD5��Ϣ
						
	Input           :	��
	Output			:	��
	Return			:	
	Note			:	
************************************************************************/

const char *tGetDirtywordVersion();

/************************************************************************
	Fuction			:	tSetDirtywordVersion
	Descption		:	����dirtylib��ǰ���ص����ֱ�MD5��Ϣ
						
	Input           :	pszDirtyFile		-- ���ֱ��ļ���
	Output			:	��
	Return			:	
	Note			:	
************************************************************************/


void tSetDirtywordVersion(char* pszDirtyFile);

/************************************************************************
	Fuction			:	tDirtyCheckChn
	Descption		:	���������
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
	Return			:	0				-- ���ɹ������໰
						1				-- ���ɹ����������Ĺؼ���
						-1				-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
						���������Ӣ��Ϲؼ��ֿ��ܸı䷵��pszContent
						�ı�ĳЩȫ���ַ�Ϊ���
************************************************************************/

int tDirtyCheckChn(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckEng
	Descption		:	Ӣ�Ĺؼ��ּ��
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
	Return			:	0				-- ���ɹ������໰
						1				-- ���ɹ�������Ӣ�Ĺؼ���
						-1				-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyCheckEng(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckReg
	Descption		:	������ʽ���
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
	Return			:	0				-- ���ɹ������໰
						1				-- ���ɹ�������������ʽ
						-1				-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
						�ù��ܽ�Linuxƽ̨��Ч,windowsƽ̨��Ч
************************************************************************/

int tDirtyCheckReg(int iDoorLevel, char * pszContent, int * iDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckPinyin
	Descption		:	ƴ�����
	Input			:	pszContent		-- �����ַ���gbk�ַ���
						iDoorLevel		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
										iDirtyLevel >= iDoorLevel������
	Output			:	iDirtyLevel		-- �жϺ���໰����
	Return			:	0				-- ���ɹ������໰
						1				-- ���ɹ�������ƴ���ؼ���
						-1				-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
	Fuction			:	tDirtyCheckEx
	Descption		:	�ۺ������-��չ�ӿ�
	Input           :	pszContent		-- �����ַ���gbk�ַ���
						iDoorLeve		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
	Output			:	iDirtyLevel		-- �жϺ���໰����
						iBegIdx			-- �ؼ�����ʼ�߽��±꣬��0�ƣ���0
						iEndIdx			-- �ؼ�����ֹ�߽��±꣬��0�ƣ�����0
	Return			:	0				-- ���ɹ������໰
						1				-- �໰���������Ĺؼ���
						2				-- �໰������Ӣ�Ĺؼ���
						3				-- �໰������������ʽ
						4				-- ��ϼ�飬�����໰
						5				-- ��ϼ�飬Ӣ���໰
						-1				-- ����û�г�ʼ��
						-2				-- ���󣬹����ֿ���Ч
						-100			-- ����û�г�ʼ��
	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
						�����ϼ��iBegIdx��iEndIdxʼ�շ���Ϊ0
************************************************************************/
int tDirtyCheckEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/************************************************************************
	Fuction			:	tDirtyCheckChnEx
	Descption		:	���Ĺؼ��ּ��-��չ�ӿ�
	Input           :	pszContent		-- �����ַ���gbk�ַ���
						iDoorLeve		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
	Output			:	iDirtyLevel		-- �жϺ���໰����
						iBegIdx			-- �ؼ�����ʼ�߽��±꣬��0�ƣ���0
						iEndIdx			-- �ؼ�����ֹ�߽��±꣬��0�ƣ�����0
	Return			:	0				-- ���ɹ������໰
						1				-- �໰���������Ĺؼ���
						-1				-- ����û�г�ʼ��

	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/************************************************************************
	Fuction			:	tDirtyCheckEngEx
	Descption		:	Ӣ�Ĺؼ��ּ��-��չ�ӿ�
	Input           :	pszContent		-- �����ַ���gbk�ַ���
						iDoorLeve		-- �����໰������0���ֱ�������
										���ֶ��ɹ��ˣ�����ֻ����
	Output			:	iDirtyLevel		-- �жϺ���໰����
						iBegIdx			-- �ؼ�����ʼ�߽��±꣬��0�ƣ���0
						iEndIdx			-- �ؼ�����ֹ�߽��±꣬��0�ƣ�����0
	Return			:	0				-- ���ɹ������໰
						1				-- �໰���������Ĺؼ���
						-1				-- ����û�г�ʼ��

	Note			:	���֮ǰ����ҪtInitializeDirtyCheck���г�ʼ��
************************************************************************/

int tDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * iDirtyLevel, int * iBegIdx, int * iEndIdx);

/************************************************************************
	Fuction			:	tMakeMapCode
	Descption		:	���ַ�ת��Ϊ��Ӧ������ӳ���������ӳ����
	Input           :	pszCharacter	-- Դ�ַ���gbk�ַ���
						iMaxCodeLenth	-- �����ӳ�����ַ�����������Ƴ���

	Output			:	pszSpellCode	-- ת���������ӳ�����ַ���
						pszPinyinCode	-- ת���������ӳ�����ַ���

	Return			:	0				-- ת���ɹ�
						1				-- ����ת���ɹ������������̫С
						-1				-- �����������

	Note			:	���в���ָ�벻��ΪNULL��iMaxCodeLenth��Ϊ��ֵ
************************************************************************/

int tMakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth);

/************************************************************************
	Fuction			:	tDirtyFini
	Descption		:	�ͷ��ڴ�
	Input			:	
	Output			:	
	Return			:	

	Note			:	
************************************************************************/

int tDirtyFini();

/************************************************************************
	Fuction			:	utf8_strlen
	Descption		:	utf8�ַ���ʵ���ȼ���
	Input			:	s				-- �����ַ�����������utf-8����	
	Output			:	
	Return			:	s utf-8���볤��

	Note			:	
************************************************************************/

size_t utf8_strlen(const char *s);

/************************************************************************
	Fuction			:	utf8_strstr
	Descption		:	utf8�ַ���ʵ���ȼ���
	Input			:	s				-- �����ַ�����������utf-8����	
	Output			:	
	Return			:	>0 				-- utf-8���볤��

	Note			:	
************************************************************************/

const char* utf8_strstr(const char *s1, const char *s2, size_t n);

/************************************************************************
	Fuction			:	utf8SpecialCharFilterInit
	Descption		:	�����ַ����˳�ʼ��
	Input			:	piHandle		--	���ָ��
						pszFileName		--	�����ַ��ļ�asci��ʽ
	Output			:	
	Return			:	0				--	��ʼ���ɹ�
						-1				--	��ʼ��ʧ��

	Note			:	
************************************************************************/
int utf8SpecialCharFilterInit(unsigned long int* piHandle, const char * pszFileName);

/************************************************************************
	Fuction			:	utf8SpecialCharFilterScan
	Descption		:	
	Input			:	piHandle		--	���
						pszString		--	utf8�����ַ���
	Output			:	
	Return			:	1				--	��⵽�����ַ�
						0				--	û�м�⵽�����ַ�	

	Note			:	
************************************************************************/

int utf8SpecialCharFilterScan(unsigned long int iHandle, const char *pszString);

/************************************************************************
	Fuction			:	utf8SpecialCharFilterFini
	Descption		:	�ͷž��
	Input			:	piHandle		--	���ָ��

	Output			:	
	Return			:

	Note			:	
************************************************************************/

void utf8SpecialCharFilterFini(unsigned long int* piHandle);

/************************************************************************
	Fuction			:	tInitializeDirtyCheckEx
	Descption		:	��ʼ��OIDirty�����������ļ�
	Input			:	pszBuff			-- ���ֱ��ڴ�ӳ��ָ�����\0��β
						iBuff			-- ���ֱ��ڴ�ӳ�񳤶�
	Output			:	��
	Return			:	0				-- ��ʼ���ɹ�
						-1				-- �����ڴ�ռ�ʧ��
						-2				-- ����Ӣ�Ĺؼ���ʧ��
						-3				-- �������Ĺؼ���ʧ��
						-4				-- ����������ʽʧ��
						-5				-- ����ƴ���ļ�ʧ��
	Note			:	
************************************************************************/

int tInitializeDirtyCheckEx(char * pszBuff,  int iBuff);

#ifdef __cplusplus
}
#endif

#endif /* __TDIRTY_H__*/
