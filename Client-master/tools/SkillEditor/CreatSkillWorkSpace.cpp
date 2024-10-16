#include "StdAfx.h"
#include "CreatSkillWorkSpace.h"
#include "GameSkill_Mgr.h"
#include "NotifyProperty.h"
#include "EnginAgent.h"
#include <vector>

IMPLEMENT_DYNAMIC(CCreatSkillWorkSpace, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(CCreatSkillWorkSpace, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()

	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
END_MESSAGE_MAP()

CCreatSkillWorkSpace::CCreatSkillWorkSpace(void): m_pSkillTemplate( NULL ), m_nSelectSkillId( 0 ), m_nCurSelectSkillId( 0 )
{
	for( int i = 0; i < MAX_PROPVALUE; i++ )
	{
		m_pPropValue[i] = NULL;
	}
}

CCreatSkillWorkSpace::~CCreatSkillWorkSpace(void)
{
}

int CCreatSkillWorkSpace::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 220, 900), this, ID_PRPPLIST_SKILL_PROP);
	m_wndPropList.EnableHeaderCtrl(FALSE);

	return 0;
}

void CCreatSkillWorkSpace::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

}

void CCreatSkillWorkSpace::OnKillFocus(CWnd* pNewWnd)
{
	CBCGPDockingControlBar::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}

LRESULT CCreatSkillWorkSpace::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	UpdateDispProp( PROP_TO_DATA );
	if( m_pSkillTemplate != NULL )
	{
		UpdateDispSkillPropValue( PROP_TO_DATA );
	}
	
	if( !m_strTemplateName.IsEmpty() && m_strCurTemplateName != m_strTemplateName )
	{
		m_strCurTemplateName = m_strTemplateName;
		if( m_pSkillTemplate != NULL )
		{
			m_wndPropList.DeleteProperty( m_pSkillTemplate );
			delete m_pSkillTemplate;
			m_pSkillTemplate = NULL;

			m_CurSkillItem.clear();
			for( int i = 0; i < MAX_PROPVALUE; i++ )
			{
				m_pPropValue[i] = NULL;
			}
			m_nCurSelectSkillId = 0;
		}

		m_pSkillTemplate = new CBCGPProp("技能编辑");
		
		UpdateDispSkillPropValue( CREATE_PROP );
		m_pSelectSkillId->RemoveAllOptions();
		std::map<int, ATTRIB_MAP>::iterator iter =  SkillManager::m_SkillAttribMap.begin();
		for( ; iter != SkillManager::m_SkillAttribMap.end(); ++iter )
		{
			if( m_strTemplateName.Compare( (*( (std::string*)(iter->second)["type"] ) ).c_str() ) == 0 )
			{
				char szId[256];
				sprintf( szId, "%d", *( (int*)(iter->second)["id"] ) );
				m_pSelectSkillId->AddOption( szId );
			}
		}
		m_pSkillTemplate->AddSubItem(m_pSelectSkillId);
		m_pSelectSkillId->SetNotifyTarget( GetSafeHwnd() );
		m_wndPropList.AddProperty(m_pSkillTemplate);
	}

	if( m_nSelectSkillId != m_nCurSelectSkillId && m_pSkillTemplate != NULL && m_nSelectSkillId != 0 )
	{
		DATA_UPDATA_TYPE type = CREATE_PROP;
		m_CurSkillItem.clear();
		
		m_nCurSelectSkillId = m_nSelectSkillId;

		m_ParseSTCfg.LoadItemBySTName( m_strTemplateName, m_CurSkillItem );
		for( int i = 0; i < m_CurSkillItem.size(); i++ )
		{
			if( m_pPropValue[i] != NULL )
			{
				m_pSkillTemplate->RemoveSubItem( (CBCGPProp*&)m_pPropValue[i] );
			}
			
			const char* szName = m_CurSkillItem[i]->stVarname.GetString();
			if( m_CurSkillItem[i]->stType == "int" )
			{
				PropUpdate_int((CBCGPProp**)&m_pPropValue[i], ( (int*)(getAttrib( m_nCurSelectSkillId, szName )) ), 
					type, m_CurSkillItem[i]->stExp);
			}else if ( m_CurSkillItem[i]->stType == "tchar" )
			{
				m_strPropStrValue[i] = ( *(std::string*)(getAttrib( m_nSelectSkillId, szName )) ).c_str();
				PropUpdate_cstring((CBCGPProp**)&m_pPropValue[i], &m_strPropStrValue[i], type, m_CurSkillItem[i]->stExp );
			}
			
			m_pSkillTemplate->AddSubItem( m_pPropValue[i] );
			m_pPropValue[i]->SetNotifyTarget( GetSafeHwnd() );
			m_pSkillTemplate->Redraw();
		}

		type = DATA_TO_PROP;
		for( int i = 0; i < m_CurSkillItem.size(); i++ )
		{
			const char* szName = m_CurSkillItem[i]->stVarname.GetString();
			if( m_CurSkillItem[i]->stType == "int" )
			{
				PropUpdate_int((CBCGPProp**)&m_pPropValue[i], ( (int*)(getAttrib( m_nSelectSkillId, szName )) ), 
					type, m_CurSkillItem[i]->stExp);
			}else if ( m_CurSkillItem[i]->stType == "tchar" )
			{
				m_strPropStrValue[i] = ( *(std::string*)(getAttrib( m_nSelectSkillId, szName )) ).c_str();
				PropUpdate_cstring((CBCGPProp**)&m_pPropValue[i], &m_strPropStrValue[i], type, m_CurSkillItem[i]->stExp );
			}
		}
		m_wndPropList.SetRedraw( true );
	}else if ( m_nSelectSkillId != 0 && m_nSelectSkillId == m_nCurSelectSkillId && m_nSelectSkillId != 0 )
	{
		DATA_UPDATA_TYPE type = PROP_TO_DATA;
		for( int i = 0; i < m_CurSkillItem.size(); i++ )
		{
			const char* szName = m_CurSkillItem[i]->stVarname.GetString();
			if( m_CurSkillItem[i]->stType == "int" )
			{
				PropUpdate_int((CBCGPProp**)&m_pPropValue[i], ( (int*)(getAttrib( m_nCurSelectSkillId, szName )) ), 
					type, m_CurSkillItem[i]->stExp);
			}else if ( m_CurSkillItem[i]->stType == "tchar" )
			{
				PropUpdate_cstring((CBCGPProp**)&m_pPropValue[i], &m_strPropStrValue[i], type, m_CurSkillItem[i]->stExp );
				( *(std::string*)(getAttrib( m_nSelectSkillId, szName )) ) = m_strPropStrValue[i].GetString();
			}
		}
	}

	return 0;
}

void CCreatSkillWorkSpace::UpdateDispProp(DATA_UPDATA_TYPE type)
{
	PropUpdate_cstring((CBCGPProp**)&m_pTemplateName, &m_strTemplateName, type, _T("模板选择"));
}

void CCreatSkillWorkSpace::UpdateDispSkillPropValue(DATA_UPDATA_TYPE type)
{
	PropUpdate_int((CBCGPProp**)&m_pSelectSkillId, &m_nSelectSkillId, type, _T("技能ID"));
}

void CCreatSkillWorkSpace::initSkillTemplate()
{
	CBCGPProp* pSkillTemplate = new CBCGPProp("技能模板选择");
	UpdateDispProp( CREATE_PROP );
	
	pSkillTemplate->AddSubItem( m_pTemplateName );

	std::map<int, ATTRIB_MAP>::iterator iter =  SkillManager::m_SkillAttribMap.begin();
	for( ; iter != SkillManager::m_SkillAttribMap.end(); ++iter )
	{
		std::string strTypeName = *( (std::string*)(iter->second)["type"] );
		bool bFind = false;
		std::vector<std::string>::iterator iter = m_TemplateName.begin();
		for( ; iter != m_TemplateName.end(); ++iter )
		{
			if( *iter == strTypeName )
			{
				bFind = true;
			}
		}
		if( !bFind )
		{
			m_TemplateName.push_back( strTypeName );
			m_pTemplateName->AddOption( strTypeName.c_str() );
		}
	}
	m_pTemplateName->SetNotifyTarget( GetSafeHwnd() );
	
	m_wndPropList.AddProperty(pSkillTemplate);
	UpdateDispProp( DATA_TO_PROP );
}

void* CCreatSkillWorkSpace::getAttrib( int skillId, const char* szName )
{
	return SkillManager::m_SkillAttribMap[skillId][szName];
}