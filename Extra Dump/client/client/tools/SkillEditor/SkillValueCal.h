#pragma once
#include "resource.h"
#include "cs_net.h"
#include "proto_comm.h"
#include <map>
#include <vector>
#include "resdb_meta.h"

// SkillValueCal dialog

class SkillValueCal : public CDialog
{
	DECLARE_DYNAMIC(SkillValueCal)
	
	int		m_nAttackerLv;

	int		m_nAttackerAddEquip;
	int		m_nHitAddEquip;

	int		m_nAttackSkillId;
	int		m_nAttackSkillLv;

	int		m_nAttackMonsterId;
	int		m_nHitMonsterId;
	
	int		m_nAttackerType;
	int		m_nHitType;

	int		m_nAtkAmmCount;
	int		m_nHitAmmCount;

	CaclInput		m_Input;
	CaclOutput		m_OutPut;

	std::map<int, ITEMDEF>			m_ItemDefMap;
	std::vector<ARMLEVELHURTDEF>	m_HurtDef;
public:
	SkillValueCal(CWnd* pParent = NULL);   // standard constructor
	virtual ~SkillValueCal();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedAttackPlayer();
	afx_msg void OnBnClickedAttackMonster();
	afx_msg void OnBnClickedHitPlayer();
	afx_msg void OnBnClickedHitMonster();
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton4();
};
