
#include "stdafx.h"
#include "SkillPreview.h"
#include "Coord.h"
#include "EffectManager.h"
#include "ObjectEditor.h"
#include "EditorManager.h"
#include "DBManager.h"

#include "OgreResDB.h"
#include "OgreEntity.h"

CSkillPreview::CSkillPreview(void) : m_pTargetModel(NULL), m_pFlyEffect(NULL)
, m_currState(SKL_NOTING), m_pCoord(NULL), m_tranArr(COOD_NONE), m_bCoord(false), m_vTargetPos(0.0f, 0.0f, 0.0f)
, m_targetType(STT_RESOURCE), m_bEnableTarget(false), m_fDir(180.0f)
{

}

CSkillPreview::~CSkillPreview(void)
{
}

void CSkillPreview::Init()
{
	m_pCoord = new CCoord();
	m_pCoord->CreateVertex();
	m_pCoord->m_fSize = 30.0f;
	ClearSkill();
	m_skillInfo.prepare_action[0] = 0;
	m_skillInfo.cast_action[0] = 0;
	m_skillInfo.behit_action[0] = 0;
	m_skillInfo.behit_effect[0] = 0;
	m_skillInfo.fly_speed = 20;
	m_skillInfo.fly_effect[0] = 0;
	m_skillInfo.fly_start_anchor = 0;
	m_skillInfo.fly_end_anchor = 0;
	m_skillInfo.prepare_time = 0;
	m_skillInfo.name[0] = 0;

}

void CSkillPreview::ClearSkill()
{
	ZeroMemory(&m_skillInfo, sizeof(tagSkillInfo));

}

void CSkillPreview::Destroy()
{
	if(m_bCoord)
		ShowCoord(false);

	OGRE_RELEASE(m_pCoord);
}

bool CSkillPreview::HasBehit(SKILL_DISPLAY_TYPE type)
{
	if(type == SDY_NORMAL_ATTACK 
		|| type == SDY_SPELL_ATTACK
		|| type == SDY_NORMAL_ATTACK_WITH_FLY
		|| type == SDY_SPELL_ATTACK_WITH_FLY
		|| type == SDY_ANY_SKILL
		)
	{
		return true;
	}
	return false;
}

bool CSkillPreview::HasPrepare(SKILL_DISPLAY_TYPE type)
{
	if(type == SDY_SPELL_ATTACK 
		|| type == SDY_SPELL_ATTACK_WITH_FLY
		|| type == SDY_SPELL_SELF_STATE
		|| type == SDY_ANY_SKILL
		)
	{
		return true;
	}
	return false;
}

//SDY_ANY_SKILL,
//SDY_NORMAL_ATTACK,
//SDY_SPELL_ATTACK,
//SDY_NORMAL_ATTACK_WITH_FLY,
//SDY_SPELL_ATTACK_WITH_FLY,
//SDY_NORMAL_SELF_STATE,
//SDY_SPELL_SELF_STATE

bool CSkillPreview::HasLaunch(SKILL_DISPLAY_TYPE type)
{
	if(type == SDY_NORMAL_ATTACK 
		|| type == SDY_SPELL_ATTACK
		|| type == SDY_NORMAL_ATTACK_WITH_FLY
		|| type == SDY_SPELL_ATTACK_WITH_FLY
		|| type == SDY_NORMAL_SELF_STATE
		|| type == SDY_SPELL_SELF_STATE
		|| type == SDY_ANY_SKILL

		)
	{
		return true;
	}
	return false;
}

bool CSkillPreview::HasFly(SKILL_DISPLAY_TYPE type)
{
	if(	type == SDY_NORMAL_ATTACK_WITH_FLY
		|| type == SDY_SPELL_ATTACK_WITH_FLY
		|| type == SDY_ANY_SKILL
		)
	{
		return true;
	}
	return false;
}

bool CSkillPreview::OnLButtonDown(float x, float y)
{
	if(m_bCoord)
	{
		m_tranArr = m_pCoord->SelectArrow(x, y) ;
		if(m_tranArr != COOD_NONE)
			return true;
	}
	return false;
}

bool CSkillPreview::OnLButtonUp(float x, float y)
{
	if(m_tranArr != COOD_NONE)
	{
		SendMainFrameMsg(EEM_UPDATE_SKILLBAR);
	}
	m_tranArr = COOD_NONE;
	return false;
}

bool CSkillPreview::OnMouseMove(float x, float y)
{
	static float lastx = 0.0f;
	static float lasty = 0.0f;
	bool ret = false;
	if(m_bCoord)
	{
		if(m_tranArr != COOD_NONE )
		{
			Ogre::Vector3 offset(0.0f, 0.0f, 0.0f);

			if(m_tranArr == COOD_X)
			{
				offset.x = m_pCoord->GetXTrans(x - lastx, y - lasty);
			}
			if(m_tranArr == COOD_Y)
			{
				offset.y = m_pCoord->GetYTrans(x - lastx, y - lasty);
			}
			if(m_tranArr == COOD_Z)
			{
				offset.z = m_pCoord->GetZTrans(x - lastx, y - lasty);
			}

			if(m_tranArr == COOD_RY)
			{
				m_fDir += m_pCoord->GetTrans(x - lastx, y - lasty, COOD_RY);
			}

			m_vTargetPos += offset;
			if(m_pTargetModel)
			{
				m_pTargetModel->setPosition(m_vTargetPos );
				m_pTargetModel->setRotation(m_fDir, 0.0f, 0.0f);
			}
			ret = true;
		}

		m_pCoord->UpdateHighLight(x, y);
	}
	lastx = x;
	lasty = y;
	return ret;
}

void CSkillPreview::UpdateInteract(float dtick)
{
	if(m_pCoord)
	{
		m_pCoord->SetPosition(m_vTargetPos);
		m_pCoord->UpdateCoord(dtick);
	}
}

void CSkillPreview::ShowCoord(bool bShow)
{
	if(m_bCoord == bShow)
		return;

	if(bShow)
	{
		m_pCoord->AttachCoordToRoom(CEnginAgent::GetInstance()->m_pRoom);
	}
	else
	{
		m_pCoord->DetachCoordFromRoom(CEnginAgent::GetInstance()->m_pRoom);
	}

	m_bCoord = bShow;
}

void CSkillPreview::PlaySkill()
{
	if(m_currState == SKL_NOTING)
	{
		m_fAccTime = 0;
		m_currState = SKL_PREPARE;
		//if(HasPrepare(m_targetType
		CEffectMgr::GetInstance()->PlayMotion(m_skillInfo.prepare_action);
	}
}

void CSkillPreview::StopSkill()
{
	m_currState = SKL_NOTING;
	if(m_pFlyEffect)
	{
		m_pFlyEffect->detachFromScene();
		OGRE_RELEASE(m_pFlyEffect);
	}
}

void CSkillPreview::DoFrame(float dtime)
{
	UpdateInteract(dtime);
	if(m_pTargetModel)
		m_pTargetModel->update(Ogre::TimeToTick(dtime));

	static float flytime = 0.0f;

	if(m_currState == SKL_PREPARE)
	{
		if(m_fAccTime > (m_skillInfo.prepare_time / 1000.0f))
		{
			CEffectMgr::GetInstance()->PlayMotion(m_skillInfo.cast_action);
			m_pFlyEffect = new Ogre::Entity;
			m_pFlyEffect->load(m_skillInfo.fly_effect, true);
			if(m_pFlyEffect)
			{
				/*
				if(m_pFlyEffect->GetMainNode()->IsKindOf(&Ogre::BeamEmitter::m_RTTI) == true 
				&& static_cast<Ogre::BeamEmitter*>(m_pFlyEffect->GetMainNode())->m_pBeamEmitter->m_Desc.m_EndPosType == Ogre::BET_REF_POS ) 
				{
				CEffectMgr::GetInstance()->GetModelActor()->BindModel(m_skillInfo.fly_start_anchor, m_pFlyEffect);
				Ogre::BeamEmitter* pBeam = static_cast<Ogre::BeamEmitter*>(m_pFlyEffect->GetMainNode());
				pBeam->SetRefNode(m_pTargetModel->GetMainNode(), m_skillInfo.fly_end_anchor);
				m_pTargetModel->PlayMotion(m_skillInfo.behit_action);
				}
				else
				{
				Ogre::Matrix4 srcmat = CEffectMgr::GetInstance()->GetModelActor()->GetMainNode()->GetAnchorWorldMatrix(m_skillInfo.fly_start_anchor);
				m_pFlyEffect->GetMainNode()->SetFather(NULL);
				m_pFlyEffect->GetMainNode()->setPosition(srcmat.GetRow3(3));
				m_pFlyEffect->attachToScene(CEnginAgent::GetInstance()->m_pRoom);

				}
				*/
				flytime = 0.0f;
			}
			m_currState = SKL_FLYING;
		}
	}
	else if(m_currState == SKL_FLYING)
	{
		if(m_pTargetModel)
		{
			if(m_pFlyEffect)
			{
				flytime += dtime;
				/*
				//À­Ìõ
				if(m_pFlyEffect->GetMainNode()->IsKindOf(&Ogre::BeamEmitter::m_RTTI) == true 
				&& static_cast<Ogre::BeamEmitter*>(m_pFlyEffect->GetMainNode())->m_pBeamEmitter->m_Desc.m_EndPosType == Ogre::BET_REF_POS ) 
				{
				if(flytime > m_skillInfo.fly_speed / 1000.0f)
				{
				CEffectMgr::GetInstance()->GetModelActor()->UnbindModel(m_pFlyEffect);
				OGRE_RELEASE(m_pFlyEffect);
				CEffectMgr::GetInstance()->GetModelActor()->StopMotion(m_skillInfo.cast_action);
				m_pTargetModel->StopMotion(m_skillInfo.behit_action);

				m_currState = SKL_HITTING;
				}


				}
				//·ÉÐÐ
				else
				{
				Ogre::Matrix4 dstmat = m_pTargetModel->GetMainNode()->GetAnchorWorldMatrix(m_skillInfo.fly_end_anchor);
				Ogre::Vector3 dstpos = dstmat.GetRow3(3);
				Ogre::Vector3 currpos = m_pFlyEffect->GetMainNode()->GetWorldMatrix().GetRow3(3);
				Ogre::Vector3 dir = dstpos - currpos;

				float len = dir.Length();
				Ogre::Normalize(dir);
				float framemove = (m_skillInfo.fly_speed * 0.1f * dtime);
				currpos += dir * (len > framemove ? framemove : len);

				m_pFlyEffect->GetMainNode()->setPosition(currpos);
				if(len < 1)
				{
				m_pFlyEffect->detachFromScene();
				OGRE_RELEASE(m_pFlyEffect);
				m_pTargetModel->PlayMotion(m_skillInfo.behit_action);
				m_currState = SKL_HITTING;
				}
				}
				*/
			}
			else
			{
				m_pTargetModel->playMotion(m_skillInfo.behit_action);
				m_currState = SKL_HITTING;
			}
		}
		else
		{

		}
	}
	else if(m_currState == SKL_HITTING)
	{
		m_currState = SKL_NOTING;
	}
	m_fAccTime += dtime;

}

void CSkillPreview::UpdateTarget()
{
	if(m_targetType == STT_RESOURCE && m_bEnableTarget)
	{
		if(m_strLastTargetName != m_strTargetName )
		{
			if(m_pTargetModel)
			{
				m_pTargetModel->detachFromScene();
				OGRE_RELEASE(m_pTargetModel);

			}
			//m_pTargetModel = Ogre::BuildModelFromChunk(m_strTargetName.GetBuffer());

			m_strLastTargetName = m_strTargetName;
		}
	}

	if(m_pTargetModel)
	{
		m_pTargetModel->setPosition(m_vTargetPos);
		m_pTargetModel->setRotation(m_fDir, 0.0f, 0.0f);
	}
}

void CSkillPreview::SaveToDb()
{
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	if(m_skillInfo.id == 0)
	{
		m_skillInfo.id = NewSkillID();
		pTable->InsertRecord(&m_skillInfo);
	}
	else
	{
		pTable->UpdateRecord(&m_skillInfo);
	}
}

bool CSkillPreview::FindSkill(int id, tagSkillInfo* pSkill)
{
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	tagSkillInfo info;
	info.id = id;
	int found = pTable->FindRecord(pSkill, 1, &info);
	if(found > 0)
		return true;
	return false;
}

bool CSkillPreview::UpdateSkillDB()
{
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	if(m_skillInfo.id == 0)
		return false;
	int count = pTable->UpdateRecord(&m_skillInfo);
	if(count > 0)
		return true;
	return false;
}

bool CSkillPreview::AddToSkillDB()
{
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	if(m_skillInfo.id == 0)
		return false;

	int count = pTable->InsertRecord(&m_skillInfo);
	if(count > 0)
		return true;
	return false;
}


unsigned int CSkillPreview::NewSkillID()
{
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	int num = pTable->FindRecord(NULL, 0, NULL);
	tagSkillInfo* pSkls = new tagSkillInfo[num];
	pTable->FindRecord(pSkls, num, NULL);

	unsigned int maxid = 0;
	for(int i = 0; i < num; i++)
	{
		if(pSkls[i].id > maxid)
			maxid = pSkls[i].id;
	}
	maxid++;
	delete pSkls;
	return maxid;
}

bool CSkillPreview::LoadSkill(int id)
{
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	tagSkillInfo skl;
	skl.id = id;
	if(pTable->FindRecord(&m_skillInfo, 1, &skl) <= 0)
		return false;
	return true;
}

void CSkillPreview::EnableTarget(bool bEnable)
{
	if(bEnable == m_bEnableTarget)
		return;

	if(bEnable)
	{
		if(m_targetType == STT_RESOURCE)
		{
			if (m_strTargetName != "")
			{
				//m_pTargetModel = Ogre::BuildModelFromChunk(m_strTargetName.GetBuffer());
				m_pTargetModel->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
				m_pTargetModel->setPosition(m_vTargetPos);
				m_pTargetModel->setRotation(m_fDir, 0.0f, 0.0f);
			}

		}
		else if(m_targetType == STT_SELF)
		{
			m_pTargetModel = CEffectMgr::GetInstance()->CreateCloneModel();
			m_pTargetModel->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
			m_pTargetModel->setPosition(m_vTargetPos);
			m_pTargetModel->setRotation(m_fDir, 0.0f, 0.0f);
		}
	}
	else
	{
		if(m_pTargetModel != NULL)
		{
			m_pTargetModel->detachFromScene();
			OGRE_RELEASE(m_pTargetModel);
		}
	}

	m_bEnableTarget = bEnable;
}


void CSkillPreview::SetTargetType(SKILL_TARGET_TYPE type)
{
	if(m_targetType == type)
		return;

	if(type == STT_RESOURCE)
	{
		if(m_pTargetModel)
		{
			m_pTargetModel->detachFromScene();
			OGRE_RELEASE(m_pTargetModel);
		}
		if (m_strTargetName != "")
		{
			//m_pTargetModel = Ogre::BuildModelFromChunk(m_strTargetName.GetBuffer());
			if(m_pTargetModel != NULL)
			{
				m_pTargetModel->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
				m_pTargetModel->setPosition(m_vTargetPos);
				m_pTargetModel->setRotation(m_fDir, 0.0f, 0.0f);
			}
		}

	}
	else if(type == STT_SELF)
	{
		if(m_pTargetModel)
		{
			m_pTargetModel->detachFromScene();
			OGRE_RELEASE(m_pTargetModel);
		}

		m_pTargetModel = CEffectMgr::GetInstance()->CreateCloneModel();
		if(m_pTargetModel != NULL)
		{
			m_pTargetModel->attachToScene(CEnginAgent::GetInstance()->m_pRoom);
			m_pTargetModel->setPosition(m_vTargetPos);
			m_pTargetModel->setRotation(m_fDir, 0.0f, 0.0f);
		}
	}
	m_targetType = type;
}