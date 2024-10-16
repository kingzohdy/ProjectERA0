
/* ��������ģ��������ļ����࣬ͨ���������ϣ�ʹ����ģ���ܶ�̬�ı༭�����еĸ���������ParseSTCfg��ParseSkillTemplateConfig*/
#pragma once
#include "afx.h"
#include <vector>

//using namespace std;
#undef CString
struct EachItem
{
	CString stVarname;
	CString stType;
	CString stDefaultvalue;
	CString stExp;
	CString stGroup;
};

struct EachTemplate
{
	CString stSkillTemplateName;
	CString stChineseOfSkillTemplateName; //���ܵ���������
	std::vector<EachItem*> vSubItem;

	EachTemplate()
	{
		stSkillTemplateName = "";
	}

	~EachTemplate()
	{
		for(int i=0; i<(int)vSubItem.size(); i++)
			delete vSubItem[i];
		vSubItem.clear();
	}
};

class ParseSTCfg
{
public:
	CStdioFile *m_pCfgFile;
	std::vector<EachTemplate*> vAllTemplate;
public:
	ParseSTCfg(void);
	~ParseSTCfg(void);

	//���ÿ������ģ�����������
	CString GetChineseOfSkillTemplate(CString strSTName);

	//�õ�strSTName����ģ���еĲ���������
	int GetSkillTemplateItemsCount(CString strSTName);

	//ͨ������strGroup����strSTName����ģ���еĸ���
	void LoadItemByGroup(CString strSTName, CString strGroup, std::vector<EachItem*>& vResult);

	//ͨ������ģ������strSTName���ؼ���ģ���еĸ���
	void LoadItemBySTName(CString strSTName, std::vector<EachItem*>& vResult);

	//ͨ��������strVarName����strSTName����ģ���е���
	EachItem* LoadItemByVarname(CString strSTName, CString strVarName);

private:
	//ͨ������ģ������strSTName���ؼ���ģ��
	EachTemplate* LoadTemplateBySTName(CString strSTName);

	EachItem* ParseEachLine(CString& strLine);

	//����strStr�е�λ����pos1��pos2֮��ģ������ַ�ch1��ch2֮����ַ���
	CString ParseString(CString& strStr, int pos1, int pos2, char ch1, char ch2);

public:
	//�ر��ļ������Ϊ�˶࿪�༭������Ϊ���ر������ļ�����ô��ʼ����ʱ��ͳ���
	void CloseFile();
};
