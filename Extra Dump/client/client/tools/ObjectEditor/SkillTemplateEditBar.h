
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
			AfxMessageBox("没有初始化，无法进行数据操作");
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

	//将产生的数据push到vVarList中
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
			AfxMessageBox("没有初始化，无法进行数据操作");
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
	CString stChineseOfTemplateName; //技能模版中文名称
	std::map<CString, ItemData*> mSubItem; //变量名、数据项
	std::map<CString, CBCGPProp*> mTypeProp; //存储此技能模版的所有组

	/*******************用于添加临时变量**********************/

	struct EachGroupTempParams
	{
		int skillid; //所属技能名，如果为-1，则是此模板的临时参数
		std::vector<std::vector<VarArgument>*> vTempParamsList; //保存临时参数，为了不总是进行copy操作，此处用指针表示
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

	//将属于技能名称stBTS的临时变量vTempParams保存起来，在外面new，由里面统一释放
	void SaveToTempParams(int skillid, std::vector<VarArgument>* vTempParams);
	//删除属于技能名称stBTS的第nPos位的临时变量
	void DelTempParamsAtPos(int skillid, int nPos);
	//得到属于技能名称stBTS的第nPos位的临时变量
	std::vector<VarArgument>* GetTempParamsAtPos(int skillid, int nPos, bool bErase = false);
	//得到属于技能名称stBTS的所有参数组数目
	int GetTempParamsNum(int skillid);
	int GetSkillID();
	////用于创建每项对应的pProp指针，即mSubItem的初始化
	//void InitTemplate(SkillTemplateDesc *psrc);
	////用一组std::vector<EachItem*>值初始化模版中的数据
	//void ResetTemplate(SkillTemplateDesc *psrc);
	//更新技能模版中的pProp显示值
	void UpdateTemplateProps(DATA_UPDATA_TYPE eType, CBCGPPropList& PropList);
	//使技能名称和id对应的prop是否可以编辑
	void EnableSomeItems(bool bEnable = true);
	//给每项的prop设置一个通告窗口
	void SetNotifyWnd(HWND hWnd);
	//用std::vector<VarArgument> vVarList初始化技能此技能模版的每一项
	void SetTemplateEachItem(std::vector<VarArgument>& vVarList);
	//用技能模版中的每一项数据填充std::vector<VarArgument>& vVarList
	void GetTemplateEachItem(std::vector<VarArgument>& vVarList);
	//根据strDefault缺省值获取对应的Prop的指针
	//用来在此Prop上面增加默认的Option选项
	void GetPropByStr(CString strDefault, std::vector<CNotifiedProp*>& vNotifiedProp);
};

//技能快捷键结构
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
	std::vector<Template*> vSkillTemplate; //用来存储所有的技能模版
	void InitializeSkillTemplate(CString strSTName); //选定某一技能模版时，对其进行初始化
	void LoadSkillTemplates(); //从配置文件中加载一组技能模版配置数据进来
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
	CSplitterGridView* m_pGridTarget; //暂时保留，用技能列表替代
	CTreeCtrl m_SkillTree; //所有的技能名称

	CResDropPropListTarget m_resDropSkill;
	CResDropPropListTarget m_resDropTarget;

	CBCGPPropList m_wndPropListSkill;
	CBCGPPropList m_wndPropListTarget;


	bool m_bSpawnHitActor; //EffectViewEdit会根据此变量来判断是否需要传鼠标消息给场景

	enum MonsterAI
	{
		Monster_Stand,
		Monster_Move,
		Monster_Attack
	};
	enum MonsterAI m_eMonsterAI; //怪物AI


	void UpdatePropList();
	void ClearSkillProp();


private:
	Ogre::Entity* m_pShowModel; //EffectMgr加载了ShowModel就是赋值进来
	Ogre::Entity* m_pShowModelTarget; //目标模型
	HTREEITEM hRoot;
	std::map< int, std::string > m_SkillTemplateNameExtraId;
	CString m_strSelectedSkillTemplate;
	//std::string m_strSelectedSkillName;
	int m_SelectedSkillID;
	std::vector<SkillsFastKey*> vSkillsFastKey; //设置各项技能的快捷键


	inline Template* GetCurrentTemplate();

	void AddOptionFromModel(CBCGPProp* pProp, Ogre::Entity* pModel); //从ShowModel中加载攻击、受击动作等名称
	//在其它的地方可能需要加入某一文件夹下的特效文件
	void AddOptionFromPath(CBCGPProp* pProp, const char* spPath = NULL);	//将spPath目录下的文件加到选项中去
	void AddOption(); //根据配置文件中的缺省值，添加可选项

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

	void PlaySkillByFastKey(int nKey=-1); //通过快捷键播放技能
	afx_msg void OnPlaySkillTemplate();
	afx_msg void OnStopPlaySkillTemplate();
	afx_msg void OnSaveSkillTemplate();
	afx_msg void OnDeleteSkill();
	afx_msg void OnOpenSkillTemplate();
	afx_msg void OnSaveSkill(); //将技能保存到Skills.lua
	afx_msg void OnSaveSkillAs();//将技能保存到自己选定的文件夹中

	afx_msg void OnHitActorOp();
	afx_msg void OnUpdateHitActorOp(CCmdUI* pCmdUI);

	//产生怪物角色
	afx_msg void OnBeHitActorOp();
	afx_msg void OnAddMonsters();
	afx_msg void OnClearAllMonsters();

	//怪物的各种AI
	afx_msg void OnMonsterAI();
	afx_msg void OnMonsterAIStand();
	afx_msg void OnUpdateMonsterAIStand(CCmdUI* pCmdUI);
	afx_msg void OnMonsterAIMove();
	afx_msg void OnUpdateMonsterAIMove(CCmdUI* pCmdUI);
	afx_msg void OnMonsterAIAttack();
	afx_msg void OnUpdateMonsterAIAttack(CCmdUI* pCmdUI);


	afx_msg void OnSelectItem(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnKeyDownItem(NMHDR* pNotifyStruct, LRESULT* result); //在技能项上进行键盘操作
	afx_msg LRESULT OnPropChangeFastkey(WPARAM wParam, LPARAM lParam); //在Prop上改变技能的快捷方式

	//对临时变量进行操作的函数
	bool m_bDelState;
	afx_msg void OnAddAsTempParams();
	afx_msg void OnDelTempParams();
	afx_msg void OnUpdateDelTempParams(CCmdUI* pCmdUI);
	void LButtonDownAction(MSG* pMsg); //左键点击
	void MouseMoveOnItem(MSG* pMsg); //鼠标在上面移动
	void UpdateTempParams(int skillid);
	void SpecialEffect(CRect& rect); //对选中的区域进行，这是客户区域
	int ComputeItemOn(CPoint pt); //计算屏幕坐标点在哪个区域中

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};