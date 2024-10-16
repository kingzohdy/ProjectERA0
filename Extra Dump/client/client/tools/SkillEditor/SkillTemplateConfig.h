
/* 分析技能模版的配置文件的类，通过此类的配合，使技能模版能动态的编辑技能中的各个参数，ParseSTCfg：ParseSkillTemplateConfig*/
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
	CString stChineseOfSkillTemplateName; //技能的中文名称
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

	//获得每个技能模版的中文名称
	CString GetChineseOfSkillTemplate(CString strSTName);

	//得到strSTName技能模版中的参数中项数
	int GetSkillTemplateItemsCount(CString strSTName);

	//通过组名strGroup加载strSTName技能模版中的各项
	void LoadItemByGroup(CString strSTName, CString strGroup, std::vector<EachItem*>& vResult);

	//通过技能模板名称strSTName加载技能模版中的各项
	void LoadItemBySTName(CString strSTName, std::vector<EachItem*>& vResult);

	//通过变量名strVarName加载strSTName技能模板中的项
	EachItem* LoadItemByVarname(CString strSTName, CString strVarName);

private:
	//通过技能模板名称strSTName加载技能模版
	EachTemplate* LoadTemplateBySTName(CString strSTName);

	EachItem* ParseEachLine(CString& strLine);

	//检测出strStr中的位置在pos1与pos2之间的，且在字符ch1与ch2之间的字符串
	CString ParseString(CString& strStr, int pos1, int pos2, char ch1, char ch2);

public:
	//关闭文件，这个为了多开编辑器，因为不关闭配置文件，那么初始化的时候就出错
	void CloseFile();
};
