
#pragma once
#include <string>
#include <vector>
#include <map>

//���ܿ�ݼ��ṹ
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

	std::vector<SkillsFastKey*>* pvSkillsFastKey; //�Ӽ���ģ����յ��˿�ݼ�����
	SkillsFastKey* m_pLocalSetParams;  //���Ի����иı�ļ��ܿ�ݼ�

	std::map<std::string, CBCGPProp*> mGroup; //ģ��������Prop
	std::vector<FastKeyProps*> vProps; //���е�Props

	CBCGPPropList m_wndSkillFSPropList;
public:
	//����¿�ݼ�ʱ�����ô˺���
	void UpdatePropsWithData(WPARAM wParam, LPARAM lParam);
	void ClearAll();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};