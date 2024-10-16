
#pragma once
#include <string>
#include <vector>
#include <map>

//技能快捷键结构
struct SkillsFastKey;

struct FastKeyProps
{
	std::string stTemplateName;
	std::string stSkillName;
	int stNum;
	CNotifiedProp* pProp;
};

class CSkillFastKeyBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CSkillFastKeyBar)

public:
	CSkillFastKeyBar();
	virtual ~CSkillFastKeyBar();

	std::vector<SkillsFastKey*>* pvSkillsFastKey; //从技能模版接收到了快捷键数据
	SkillsFastKey* m_pLocalSetParams;  //本对话框中改变的技能快捷键

	std::map<std::string, CBCGPProp*> mGroup; //模板名称与Prop
	std::vector<FastKeyProps*> vProps; //所有的Props

	CBCGPPropList m_wndSkillFSPropList;
public:
	//添加新快捷键时，调用此函数
	void UpdatePropsWithData(WPARAM wParam, LPARAM lParam);
	void ClearAll();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};