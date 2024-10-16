#ifndef __OIDIRTYINTERFACE_H__
#define __OIDIRTYINTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
  Fuction         :  MakeMapCode
  Descption       :  ���ַ�ת��Ϊ��Ӧ������ӳ���������ӳ����
  Input           :  pszCharacter -- Դ�ַ���
                     iMaxCodeLenth-- �����ӳ�����ַ�����������Ƴ���
  Output          :  pszSpellCode -- ת���������ӳ�����ַ���
                     pszPinyinCode-- ת���������ӳ�����ַ���
  Return          :  0            -- ת���ɹ�
                     1            -- ����ת���ɹ������������̫С
                     -1           -- �����������
  Note            :  ���в���ָ�벻��ΪNULL��iMaxCodeLenth��Ϊ��ֵ
************************************************************************/
int MakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth);

/************************************************************************
  Fuction         :  InitializeDirtyCheck
  Descption       :  ��ʼ��OIDirty�����������ļ�
  Input           :  pszChnKeyFile -- ���Ĺؼ����ļ���
                     pszEngKeyFile -- Ӣ�Ĺؼ����ļ���
                     pszRegFile    -- ������ʽ�ļ���
					 pszPinyinFile -- ƴ�����ؼ��ʺ�������ʽ���ļ���
  Output          :  ��
  Return          :  0             -- ��ʼ���ɹ�
                     -1            -- �����ڴ�ռ�ʧ��
                     -2            -- ����Ӣ�Ĺؼ���ʧ��
                     -3            -- �������Ĺؼ���ʧ��
                     -4            -- ����������ʽʧ��
					 -5            -- ����ƴ���ļ�ʧ��
  Note            :  �ĸ��ļ�������ָ�룬������һ����ΪNULL
************************************************************************/
//int InitializeDirtyCheck(char * pszChnKeyFile, char * pszEngKeyFile, char * pszRegFile, char * pszPinyinFile);
int InitializeDirtyCheck(char * pszDirtyFile, int iForce);

/************************************************************************
  Fuction         :  OIDirtyCheck
  Descption       :  �ۺ������
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰���������Ĺؼ���
                     2             -- �໰������Ӣ�Ĺؼ���
                     3             -- �໰������������ʽ
                     4             -- ��ϼ�飬�����໰
                     5             -- ��ϼ�飬Ӣ���໰
                     -1            -- ����û�г�ʼ��
                     -2            -- ���󣬹����ֿ���Ч
                     -100          -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheck(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckChn
  Descption       :  ���Ĺؼ��ּ��
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰���������Ĺؼ���
                     -1            -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckChn(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckEng
  Descption       :  Ӣ�Ĺؼ��ּ��
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰������Ӣ�Ĺؼ���
                     -1            -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckEng(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckReg
  Descption       :  ������ʽ���
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
					 iBeginIdx     -- �ؼ�����ʼ�߽��±꣬��0�ƣ���
                     iEndIdx       -- �ؼ�����ֹ�߽��±꣬��0�ƣ�����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰������������ʽ
                     -1            -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckReg(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyCheckPinyin
  Descption       :  ƴ�����
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰������ƴ������
                     -1            -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyCheckEx
  Descption       :  �ۺ������-��չ�ӿ�
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
                     iBeginIdx     -- �ؼ�����ʼ�߽��±꣬��0�ƣ���
                     iEndIdx       -- �ؼ�����ֹ�߽��±꣬��0�ƣ�����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰���������Ĺؼ���
                     2             -- �໰������Ӣ�Ĺؼ���
                     3             -- �໰������������ʽ
                     4             -- ��ϼ�飬�����໰
                     5             -- ��ϼ�飬Ӣ���໰
                     -1            -- ����û�г�ʼ��
                     -2            -- ���󣬹����ֿ���Ч
                     -100          -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyCheckChnEx
  Descption       :  ���Ĺؼ��ּ��-��չ�ӿ�
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
                     iBeginIdx     -- �ؼ�����ʼ�߽��±꣬��0�ƣ���
                     iEndIdx       -- �ؼ�����ֹ�߽��±꣬��0�ƣ�����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰���������Ĺؼ���
                     -1            -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyCheckEngEx
  Descption       :  Ӣ�Ĺؼ��ּ��-��չ�ӿ�
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
                     iBeginIdx     -- �ؼ�����ʼ�߽��±꣬��0�ƣ���
                     iEndIdx       -- �ؼ�����ֹ�߽��±꣬��0�ƣ�����
  Return          :  0             -- ���ɹ������໰
                     1             -- �໰������Ӣ�Ĺؼ���
                     -1            -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx);

/************************************************************************
  Fuction         :  OIDirtyReplace
  Descption       :  �ۺ������
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ַ��������滻Ϊ*
                     -1            -- ����û�г�ʼ��
                     -2            -- ���󣬹����ֿ���Ч
                     -100          -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyReplace(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyReplace2
  Descption       :  �ۺ������
  Input           :  pszContent    -- �����ַ���
                     iDoorLevel    -- �����໰����
  Output          :  iDirtyLevel   -- �жϺ���໰����
  Return          :  0             -- ���ַ��������滻Ϊ*
                     -1            -- ����û�г�ʼ��
                     -2            -- ���󣬹����ֿ���Ч
                     -100          -- ����û�г�ʼ��
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyReplace2(int iDoorLevel, char * pszContent, int * piDirtyLevel);

/************************************************************************
  Fuction         :  OIDirtyFini
  Descption       :  �ͷ��ڴ�
  Input           :  
                     
  Output          :  
  Return          :  
  Note            :  ���֮ǰ����ҪInitializeDirtyCheck���г�ʼ��
************************************************************************/
int OIDirtyFini();

/************************************************************************
  Fuction         :  InitializeDirtyCheck
  Descption       :  ��ʼ��OIDirty�����������ļ�
  Input           :  pszBuff -- ���ֱ��ļ��ڴ�ӳ��
  			: iBuff 	-- pszBuff����
                   
  Output          :  ��
  Return          :  0             -- ��ʼ���ɹ�
                     -1            -- �����ڴ�ռ�ʧ��
                     -2            -- ����Ӣ�Ĺؼ���ʧ��
                     -3            -- �������Ĺؼ���ʧ��
                     -4            -- ����������ʽʧ��
					 -5            -- ����ƴ���ļ�ʧ��
  Note            :  �ĸ��ļ�������ָ�룬������һ����ΪNULL
************************************************************************/
int InitializeDirtyCheckEx(char * pszBuff,  int iBuff);

#ifdef __cplusplus
}
#endif

#endif /* __OIDIRTYINTERFACE_H__*/
