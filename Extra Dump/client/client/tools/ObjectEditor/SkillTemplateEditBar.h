
#pragma once
#include "ObjectEditorTypes.h"
#include "EditorObject.h"
#include "SkillTemplateConfig.h"
#include "NormalSplitterWnd.h"
#include "ResourceDropTarget.h"
#include <map>

#ifndef NAME_LENGHT
#define NAME_LENGHT 64
#endif

#ifdef CString
#undef CString
#endif

struct CoreData
{
	enum
	{
		eInt,
		eFloat,
		eChar
	};
	int nType;

	union
	{
		int stInt;
		float stFloat;
		char stChar[NAME_LENGHT];
	};

	CoreData()
	{
		nType = -1;
	}
};

struct ItemData
{
	CNotifiedProp* pProp;
	CoreData stData;
	//SkillVarDesc* stEachItem;

	ItemData()
	{ 
		pProp = NULL;
		//stEachItem = NULL;
	}

	//void InitCoreData(SkillVarDesc* pEachItem)
	//{
	//	stEachItem = pEachItem;

	//	CString strStr = pEachItem->type.c_str();
	//	strStr.MakeLower();
	//	if(strStr.Compare("int") == 0)
	//	{
	//		stData.nType = CoreData::eInt;
	//		stData.stInt = (pEachItem->defvalue.empty()) ? 0 : atoi(pEachItem->defvalue.c_str());
	//	}
	//	else if(strStr.Compare("float") == 0)
	//	{
	//		stData.nType = CoreData::eFloat;
	//		stData.stFloat = (pEachItem->defvalue.empty()) ? 0 : (float)atof(pEachItem->defvalue.c_str());
	//	}
	//	else if(strStr.Compare("string") == 0)
	//	{
	//		stData.nType = CoreData::eChar;
	//		memset(stData.stChar, 0, NAME_LENGHT);
	//		_snprintf(stData.stChar, NAME_LENGHT, "%s", pEachItem->defvalue.c_str());
	//	}
	//}

	void SetCoreData(VarArgument& varArgument)
	{
		switch(varArgument.mValueType)
		{
		case VarArgument::VA_INT:
			varArgument.Get(stData.stInt);
			break;
		case VarArgument::VA_Number:
			varArgument.Get(stData.stFloat);
			break;
		case VarArgument::VA_STRING:
			varArgument.Get(stData.stChar, NAME_LENGHT);
			break;
		default: AfxMessageBox("void SetCoreData(VarArgument& varArgument) Wrong !"); break;
		}
	}

	void SetCoreData(void* pData = NULL)
	{
		if(stData.nType == -1)
		{
			AfxMessageBox("û�г�ʼ�����޷��������ݲ���");
			return;
		}
		switch(stData.nType)
		{
		case CoreData::eInt: stData.stInt = *((int*)pData); break;
		case CoreData::eFloat: stData.stFloat = *((float*)pData); break;
		case CoreData::eChar:
			{
				char* psChar = (char*)pData;
				_snprintf(stData.stChar, NAME_LENGHT, "%s", psChar);
			}
			break;
		}
	}

	//������������push��vVarList��
	void GetCoreData(std::vector<VarArgument>& vVarList)
	{
		switch(stData.nType)
		{
		case CoreData::eInt:
			//vVarList.push_back(VarArgument(TEXT(stEachItem->varname.c_str()), stData.stInt));
			break;
		case CoreData::eFloat:
			//vVarList.push_back(VarArgument(TEXT(stEachItem->varname.c_str()), stData.stFloat));
			break;
		case CoreData::eChar:
			//vVarList.push_back(VarArgument(TEXT(stEachItem->varname.c_str()), stData.stChar));
			break;
		default: AfxMessageBox("GetCoreData(std::vector<VarArgument>& vVarList) Wrong !"); break;
		}
	}

	void GetCoreData(void* pData = NULL)
	{
		if(stData.nType == -1)
		{
			AfxMessageBox("û�г�ʼ�����޷��������ݲ���");
			return;
		}
		switch(stData.nType)
		{
		case CoreData::eInt: pData = (void*)&stData.stInt; break;
		case CoreData::eFloat: pData = (void*)&stData.stFloat; break;
		case CoreData::eChar: pData = (void*)stData.stChar; break;
		}
	}
};
struct EachGroupTempParams;
struct Template
{
	int skillid;
	CString stTemplateName;
	CString stChineseOfTemplateName; //����ģ����������
	std::map<CString, ItemData*> mSubItem; //��������������
	std::map<CString, CBCGPProp*> mTypeProp; //�洢�˼���ģ���������

	/*******************���������ʱ����**********************/

	struct EachGroupTempParams
	{
		int skillid; //���������������Ϊ-1�����Ǵ�ģ�����ʱ����
		std::vector<std::vector<VarArgument>*> vTempParamsList; //������ʱ������Ϊ�˲����ǽ���copy�������˴���ָ���ʾ
		~EachGroupTempParams()
		{
			for(int i=0; i<(int)vTempParamsList.size(); i++)
			{
				delete vTempParamsList[i];
			}
		}
	};

	std::vector<EachGroupTempParams*> vAllGroupParams;

	Template();
	Template(CString strSTName);
	~Template();

	//�����ڼ�������stBTS����ʱ����vTempParams����������������new��������ͳһ�ͷ�
	void SaveToTempParams(int skillid, std::vector<VarArgument>* vTempParams);
	//ɾ�����ڼ�������stBTS�ĵ�nPosλ����ʱ����
	void DelTempParamsAtPos(int skillid, int nPos);
	//�õ����ڼ�������stBTS�ĵ�nPosλ����ʱ����
	std::vector<VarArgument>* GetTempParamsAtPos(int skillid, int nPos, bool bErase = false);
	//�õ����ڼ�������stBTS�����в�������Ŀ
	int GetTempParamsNum(int skillid);
	int GetSkillID();
	////���ڴ���ÿ���Ӧ��pPropָ�룬��mSubItem�ĳ�ʼ��
	//void InitTemplate(SkillTemplateDesc *psrc);
	////��һ��std::vector<EachItem*>ֵ��ʼ��ģ���е�����
	//void ResetTemplate(SkillTemplateDesc *psrc);
	//���¼���ģ���е�pProp��ʾֵ
	void UpdateTemplateProps(DATA_UPDATA_TYPE eType, CBCGPPropList& PropList);
	//ʹ�������ƺ�id��Ӧ��prop�Ƿ���Ա༭
	void EnableSomeItems(bool bEnable = true);
	//��ÿ���prop����һ��ͨ�洰��
	void SetNotifyWnd(HWND hWnd);
	//��std::vector<VarArgument> vVarList��ʼ�����ܴ˼���ģ���ÿһ��
	void SetTemplateEachItem(std::vector<VarArgument>& vVarList);
	//�ü���ģ���е�ÿһ���������std::vector<VarArgument>& vVarList
	void GetTemplateEachItem(std::vector<VarArgument>& vVarList);
	//����strDefaultȱʡֵ��ȡ��Ӧ��Prop��ָ��
	//�����ڴ�Prop��������Ĭ�ϵ�Optionѡ��
	void GetPropByStr(CString strDefault, std::vector<CNotifiedProp*>& vNotifiedProp);
};

//���ܿ�ݼ��ṹ
struct SkillsFastKey 
{
	std::string stSkillTemplateName;
	std::string stSkillName;
	int skillid;
	int stNum;
};

class CSkillTemplateEdit : public CWnd
{
	DECLARE_DYNAMIC(CSkillTemplateEdit)

public:
	CSkillTemplateEdit();
	virtual ~CSkillTemplateEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:
	ParseSTCfg m_ParseSTCfg;
	std::vector<Template*> vSkillTemplate; //�����洢���еļ���ģ��
	void InitializeSkillTemplate(CString strSTName); //ѡ��ĳһ����ģ��ʱ��������г�ʼ��
	void LoadSkillTemplates(); //�������ļ��м���һ�鼼��ģ���������ݽ���
	inline CString SkillTemplateNameE2C(CString strEngName)
	{
		for(int i=0; i<(int)vSkillTemplate.size(); i++)
		{
			if(vSkillTemplate[i]->stTemplateName.Compare(strEngName) == 0)
				return vSkillTemplate[i]->stChineseOfTemplateName;
		}
		return strEngName;
	}
	inline CString SkillTemplateNameC2E(CString strChnName)
	{
		for(int i=0; i<(int)vSkillTemplate.size(); i++)
		{
			if(vSkillTemplate[i]->stChineseOfTemplateName.Compare(strChnName) == 0)
				return vSkillTemplate[i]->stTemplateName;
		}
		return strChnName;
	}

	void SetVarToSkill(int skillid, std::vector<VarArgument>&arglist);
	void GetVarFromSkill(int skillid, std::vector<VarArgument>&arglist);

public:
	CBCGPToolBar m_wndToolBar;

	CNormalSplitterWnd m_wndSplitter;
	CSplitterGridView* m_pGridSkill;
	CSplitterGridView* m_pGridTarget; //��ʱ�������ü����б����
	CTreeCtrl m_SkillTree; //���еļ�������

	CResDropPropListTarget m_resDropSkill;
	CResDropPropListTarget m_resDropTarget;

	CBCGPPropList m_wndPropListSkill;
	CBCGPPropList m_wndPropListTarget;


	bool m_bSpawnHitActor; //EffectViewEdit����ݴ˱������ж��Ƿ���Ҫ�������Ϣ������

	enum MonsterAI
	{
		Monster_Stand,
		Monster_Move,
		Monster_Attack
	};
	enum MonsterAI m_eMonsterAI; //����AI


	void UpdatePropList();
	void ClearSkillProp();


private:
	Ogre::Entity* m_pShowModel; //EffectMgr������ShowModel���Ǹ�ֵ����
	Ogre::Entity* m_pShowModelTarget; //Ŀ��ģ��
	HTREEITEM hRoot;
	std::map< int, std::string > m_SkillTemplateNameExtraId;
	CString m_strSelectedSkillTemplate;
	//std::string m_strSelectedSkillName;
	int m_SelectedSkillID;
	std::vector<SkillsFastKey*> vSkillsFastKey; //���ø���ܵĿ�ݼ�


	inline Template* GetCurrentTemplate();

	void AddOptionFromModel(CBCGPProp* pProp, Ogre::Entity* pModel); //��ShowModel�м��ع������ܻ�����������
	//�������ĵط�������Ҫ����ĳһ�ļ����µ���Ч�ļ�
	void AddOptionFromPath(CBCGPProp* pProp, const char* spPath = NULL);	//��spPathĿ¼�µ��ļ��ӵ�ѡ����ȥ
	void AddOption(); //���������ļ��е�ȱʡֵ����ӿ�ѡ��

public:
	void UpdateSkillTree();
	void UpdateSkillTemplateName();
public:
	void SetShowModel(Ogre::Entity* pShowModel) 
	{
		m_pShowModel = pShowModel;
	}

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSelectSkillTemplateType();

	void PlaySkillByFastKey(int nKey=-1); //ͨ����ݼ����ż���
	afx_msg void OnPlaySkillTemplate();
	afx_msg void OnStopPlaySkillTemplate();
	afx_msg void OnSaveSkillTemplate();
	afx_msg void OnDeleteSkill();
	afx_msg void OnOpenSkillTemplate();
	afx_msg void OnSaveSkill(); //�����ܱ��浽Skills.lua
	afx_msg void OnSaveSkillAs();//�����ܱ��浽�Լ�ѡ�����ļ�����

	afx_msg void OnHitActorOp();
	afx_msg void OnUpdateHitActorOp(CCmdUI* pCmdUI);

	//���������ɫ
	afx_msg void OnBeHitActorOp();
	afx_msg void OnAddMonsters();
	afx_msg void OnClearAllMonsters();

	//����ĸ���AI
	afx_msg void OnMonsterAI();
	afx_msg void OnMonsterAIStand();
	afx_msg void OnUpdateMonsterAIStand(CCmdUI* pCmdUI);
	afx_msg void OnMonsterAIMove();
	afx_msg void OnUpdateMonsterAIMove(CCmdUI* pCmdUI);
	afx_msg void OnMonsterAIAttack();
	afx_msg void OnUpdateMonsterAIAttack(CCmdUI* pCmdUI);


	afx_msg void OnSelectItem(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnKeyDownItem(NMHDR* pNotifyStruct, LRESULT* result); //�ڼ������Ͻ��м��̲���
	afx_msg LRESULT OnPropChangeFastkey(WPARAM wParam, LPARAM lParam); //��Prop�ϸı似�ܵĿ�ݷ�ʽ

	//����ʱ�������в����ĺ���
	bool m_bDelState;
	afx_msg void OnAddAsTempParams();
	afx_msg void OnDelTempParams();
	afx_msg void OnUpdateDelTempParams(CCmdUI* pCmdUI);
	void LButtonDownAction(MSG* pMsg); //������
	void MouseMoveOnItem(MSG* pMsg); //����������ƶ�
	void UpdateTempParams(int skillid);
	void SpecialEffect(CRect& rect); //��ѡ�е�������У����ǿͻ�����
	int ComputeItemOn(CPoint pt); //������Ļ��������ĸ�������

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};