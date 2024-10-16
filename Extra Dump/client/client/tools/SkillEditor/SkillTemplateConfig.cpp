
#include "stdafx.h"
#include "SkillTemplateConfig.h"
#include <assert.h>

CString g_strCfgPath = "db/skillcfg.txt";

ParseSTCfg::ParseSTCfg(void)
{
	m_pCfgFile = NULL;
	try
	{
		m_pCfgFile = new CStdioFile(g_strCfgPath, CFile::modeRead | CFile::typeText);
	}
	catch(CFileException*)
	{
		CString strErr;
		strErr = "Can't Open file" + g_strCfgPath;
		AfxMessageBox(strErr);
	}
	assert(m_pCfgFile != NULL);
}

ParseSTCfg::~ParseSTCfg(void)
{
	m_pCfgFile->Close();
	m_pCfgFile = NULL;
	for(int i=0; i<(int)vAllTemplate.size(); i++)
	{
		delete vAllTemplate[i];
		vAllTemplate.clear();
	}
}

void ParseSTCfg::CloseFile()
{
	m_pCfgFile->Close();
}

CString ParseSTCfg::GetChineseOfSkillTemplate(CString strSTName)
{
	for(int i=0; i<(int)vAllTemplate.size(); i++)
	{
		if(vAllTemplate[i]->stSkillTemplateName.Compare(strSTName) == 0)
			return vAllTemplate[i]->stChineseOfSkillTemplateName;
	}
	return strSTName;
}

void ParseSTCfg::LoadItemByGroup(CString strSTName, CString strGroup, std::vector<EachItem*>& vResult)
{
	EachTemplate* pEachTemplate = NULL;
	pEachTemplate = LoadTemplateBySTName(strSTName);

	if(pEachTemplate == NULL)
	{
		CString strErr = strSTName + " 技能模版不存在";
		AfxMessageBox(strErr);
		return;
	}

	for(int j=0; j<(int)pEachTemplate->vSubItem.size(); j++)
	{
		if(pEachTemplate->vSubItem[j]->stGroup.Compare(strGroup) == 0)
			vResult.push_back(pEachTemplate->vSubItem[j]);
	}
}

void ParseSTCfg::LoadItemBySTName(CString strSTName, std::vector<EachItem*>& vResult)
{
	EachTemplate* pEachTemplate = NULL;
	pEachTemplate = LoadTemplateBySTName(strSTName);
	if(pEachTemplate == NULL)
	{
		CString strErr = strSTName + " 技能模板不存在";
		AfxMessageBox(strErr);
		return;
	}

	vResult = pEachTemplate->vSubItem;
}

EachTemplate* ParseSTCfg::LoadTemplateBySTName(CString strSTName)
{
	EachTemplate* pEachTemplate = NULL;
	CString strLine;
	for(int i=0; i<(int)vAllTemplate.size(); i++)
	{
		if(vAllTemplate[i]->stSkillTemplateName.Compare(strSTName) == 0)
		{
			pEachTemplate = vAllTemplate[i];
			return pEachTemplate;
		}
	}
	m_pCfgFile->SeekToBegin();
	while(m_pCfgFile->ReadString(strLine))
	{
		//识别技能模版名称
		int nRlt = strLine.Find(strSTName.GetBuffer());
		if(nRlt != -1)
		{
			pEachTemplate = new EachTemplate;
			pEachTemplate->stSkillTemplateName = strSTName;

			int nRltBeg = strLine.Find('\"');
			int nRltEnd = strLine.Find('\"', nRltBeg+1);
			if(nRltBeg != -1 && nRltEnd != -1)
				pEachTemplate->stChineseOfSkillTemplateName = strLine.Mid(nRltBeg+1, nRltEnd-nRltBeg-1);
			else
				pEachTemplate->stChineseOfSkillTemplateName = strSTName;

			BOOL bRead = m_pCfgFile->ReadString(strLine);
			assert(bRead && (strLine.Compare("{") == 0));
			while(m_pCfgFile->ReadString(strLine) && (strLine.Compare("};") != 0))
			{
				strLine.TrimLeft();
				strLine.TrimRight();
				if(strLine.Compare("") == 0)
					continue;
				//分析每一行
				EachItem* pEachItem = ParseEachLine(strLine);
				pEachTemplate->vSubItem.push_back(pEachItem);
			}
			m_pCfgFile->SeekToBegin(); //读完一个技能模板后把文件指针指向开始处
			vAllTemplate.push_back(pEachTemplate);
			return pEachTemplate;
		}
	}
	return pEachTemplate;
}

EachItem* ParseSTCfg::ParseEachLine(CString& strLine)
{
	EachItem* pEachItem = new EachItem;

	int nPosSemicolon = strLine.Find(';');
	if(((nPosSemicolon+1) != strLine.GetLength()) || (nPosSemicolon == -1))
	{
		CString strErr = strLine + " 格式有误";
		AfxMessageBox(strErr);
	}
	int nPosItem = strLine.Find("varname");
	int nPosComma = strLine.Find(',');
	CString res = ParseString(strLine, nPosItem, nPosComma, '\"', '\"');
	pEachItem->stVarname = res;

	nPosItem = strLine.Find("type");
	nPosComma = strLine.Find(',', nPosItem);
	res = ParseString(strLine, nPosItem, nPosComma, '\"', '\"');
	pEachItem->stType = res;

	nPosItem = strLine.Find("defaultvalue");
	nPosComma = strLine.Find(',', nPosItem);
	res = ParseString(strLine, nPosItem, nPosComma, '\"', '\"');
	pEachItem->stDefaultvalue = res;

	nPosItem = strLine.Find("exp");
	nPosComma = strLine.Find(',', nPosItem);
	res = ParseString(strLine, nPosItem, nPosComma, '\"', '\"');
	pEachItem->stExp = res;

	nPosItem = strLine.Find("group");
	nPosComma = strLine.Find(';', nPosItem); //最后一项用 ';' 表示
	res = ParseString(strLine, nPosItem, nPosComma, '\"', '\"');
	pEachItem->stGroup = res;

	return pEachItem;
}

CString ParseSTCfg::ParseString(CString& strStr, int pos1, int pos2, char ch1, char ch2)
{
	int nPos1 = strStr.Find(ch1, pos1);
	assert(nPos1 != -1);
	int nPos2 = strStr.Find(ch2, nPos1+1);
	assert(nPos2 <= pos2);
	return strStr.Mid(nPos1+1, nPos2-nPos1-1);
}

int ParseSTCfg::GetSkillTemplateItemsCount(CString strSTName)
{
	return 0;
}

EachItem* ParseSTCfg::LoadItemByVarname(CString strSTName, CString strVarName)
{
	EachTemplate* pEachTemplate = LoadTemplateBySTName(strSTName);
	if(pEachTemplate == NULL)
	{
		CString strErr = strSTName + " 技能模版不存在";
		AfxMessageBox(strErr);
		return NULL;
	}
	for(int i=0; i<(int)pEachTemplate->vSubItem.size(); i++)
	{
		if(pEachTemplate->vSubItem[i]->stVarname.Compare(strVarName) == 0)
			return pEachTemplate->vSubItem[i];
	}
	return NULL;
}