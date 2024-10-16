#include "GameTopCareer.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "OgreCamera.h"
#include "GameCamera.h"
#include "OgreUIRenderer.h"
#include "AvatarEntity.h"
#include "ClanInfo.h"
#include "../UILib/ui_framemgr.h"

using namespace Ogre;

GameTopCareer::GameTopCareer(void)
{
	for ( int i = 0; i < MAX_SUPPORT_MEM_NUM; i++ )
	{
		Effigy_Career[i] = NULL;
	}
	memset( m_EffigyCareerInfo, 0, sizeof(tagDFFirstInfo)*MAX_SUPPORT_MEM_NUM );
}

GameTopCareer::~GameTopCareer(void)
{
}

int GameTopCareer::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	return 0;
}

void GameTopCareer::Update(unsigned int dtick)
{
	for ( int i = 0; i < MAX_SUPPORT_MEM_NUM; i++ )
	{
		if ( Effigy_Career[i] )
		{
			Effigy_Career[i]->update(dtick);
		}
	}
}

void GameTopCareer::draw(FollowCamera *pcamera)
{
	//八位职业冠军的雕像,按照DIANFENG_JOB_TYPE的顺序来
	for ( int i = 0; i < MAX_SUPPORT_MEM_NUM; i++ )
	{
		if ( Effigy_Career[i] )
		{
			Ogre::Camera *camera = pcamera->GetCameraNode();
			const Ogre::Vector3 &pos = Effigy_Career[i]->getAnchorWorldPos(106);
			if ( pcamera->GetCameraNode()->canSeePointInWorld(pos) )
			{
				Ogre::Vector3 scrpos;
				camera->pointWorldToWindow(scrpos, pos);
				Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
				float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ? g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
				float width, height;
				string defNames[MAX_SUPPORT_MEM_NUM] = { "第一征服", "第一守护", "第一驭雷", "第一圣佑", "第一猎杀", "第一毁灭", "第一炼魂", "第一聚能" };
				prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], defNames[i].c_str(), width, height);
				prenderer->renderText(s_hFontTexture[NAME_FONT], FONTSYTLE_BORDER, defNames[i].c_str(), scrpos.x - (width * 0.5f) * fScale,
					scrpos.y + (height  - 40) * fScale, Ogre::ColorQuad(0xffd986ff), fScale, false, 0);
				const char *name = m_EffigyCareerInfo[i].RoleName;
				if (name[0] == '\0')
				{
					name = "待定";
				}
				prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], name, width, height);
				prenderer->renderText(s_hFontTexture[NAME_FONT], FONTSYTLE_BORDER, name, scrpos.x - (width * 0.5f) * fScale,
					scrpos.y + (height*2 - 40) * fScale, Ogre::ColorQuad(0xfff6ff00), fScale, false, 0);
			}
		}
	}
}

void GameTopCareer::ClearCareerEffigy()
{
	for ( int i = 0; i < MAX_SUPPORT_MEM_NUM; i++ )
	{
		if ( Effigy_Career[i] )
		{
			Effigy_Career[i]->detachFromScene();
			Effigy_Career[i]->release();
			Effigy_Career[i] = NULL;
		}
	}
}

void GameTopCareer::UpdateCareerEffigy()
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return;
	}
	GameMap *map = player->getCurMap();
	if (map == NULL)
	{
		return;
	}
	if ( m_EffigyCareerInfo[0].MapID != map->getRealId() )
	{
		return;
	}
	//八位职业冠军的雕像
	for ( int i = 0; i < MAX_SUPPORT_MEM_NUM; i++ )
	{
		int PoseState = POSE_NONE;
		if( strcmp( m_EffigyCareerInfo[i].RoleName, "" ) != 0 )
		{
			AvatarEntity *pavatar = new AvatarEntity;
			int resid = getPlayerDisplayId(m_EffigyCareerInfo[i].RoleGender, m_EffigyCareerInfo[i].RoleCareer);
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
			pavatar->equipItem(POS_ID_HEAD, m_EffigyCareerInfo[i].RoleHead, 0);
			pavatar->equipItem(POS_ID_FACE, m_EffigyCareerInfo[i].RoleFace, 0);
			pavatar->equipItem(POS_ID_HAIR, m_EffigyCareerInfo[i].RoleHair, 0);
			int left = 0, right = 0;
			for (size_t j = 0; j < m_EffigyCareerInfo[i].RoleAvatar.Num; ++j)
			{
				const tagArmProf Arm = m_EffigyCareerInfo[i].RoleAvatar.Arms[j];
				pavatar->equipItem(EquipPos_2_PosId[Arm.GridIdx], Arm.ArmDefID, Arm.StarLv);
				if (Arm.GridIdx == EQUIP_POS_LEFT)
				{
					left = Arm.ArmDefID / 10000;
				}
				else if (Arm.GridIdx == EQUIP_POS_RIGHTHAND)
				{
					right = Arm.ArmDefID / 10000;
				}
				else if ( EquipPos_2_PosId[Arm.GridIdx] == POS_ID_FASHION_HEAD )
				{
					int nHair = DBDataManager::getSingleton().getItemDisplayInfo(Arm.ArmDefID).hairstyle;
					nHair = nHair > 1 ? 9 : m_EffigyCareerInfo[i].RoleHair;
					pavatar->equipItem(POS_ID_HAIR, nHair, 0);
				}
			}
			pavatar->setAvatarAttrib(0, m_EffigyCareerInfo[i].RoleCareer);
			pavatar->setAvatarAttrib(1, info.gender);
			if (right == ARM_SWORD || right == ARM_KNIFE)
			{
				if (left == ARM_SWORD_LEFT)
				{
					PoseState = POSE_WEAPON_SHORTWEAPON;
				}
				else
				{
					PoseState = POSE_SHORTWEAPON_R;
				}
			}
			else if (right == ARM_BOTHSWORD)
			{
				PoseState = POSE_WEAPON_SHORTWEAPON;
			}
			else if (right == ARM_HEVY_SCATTERGUN)
			{
				PoseState = POSE_SCATTERGUN;
			}
			else if (right == ARM_BITH_GUN)
			{
				PoseState = POSE_SCATTERGUN;
			}
			else if (right == ARM_RIGHT_GUN)
			{
				if (left == ARM_GUNNER_WEAR_LEFT)
				{
					PoseState = POSE_DOUBLEGUN;
				}
				else
				{
					PoseState = POSE_GUN_R;
				}
			}
			else if (right / 10 == ARM_WIZARD)
			{
				PoseState = POSE_WAND;
			}
			else if (right / 10 == ARM_CHURCH)
			{
				PoseState = POSE_CHURCH_WAND;
			}
			pavatar->updateAttrib();

			SetEffigyStarEffect( pavatar, m_EffigyCareerInfo[i].RoleAvatar.ArmStar );

			Effigy_Career[i] = pavatar;
		}
		else
		{
			//DIANFENG_JOB_TYPE顺序
			int resIDs[MAX_SUPPORT_MEM_NUM] = {202114,202110,202117,202113,202112,202116,202111,202115};
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resIDs[i]);
			AvatarEntity *pavatar = new AvatarEntity;
			pavatar->equipItem(POS_ID_HAIR, info.userdata[0], 0);
			pavatar->equipItem(POS_ID_FACE, info.userdata[1], 0);
			pavatar->equipItem(POS_ID_CLOSES, info.userdata[2], 0);
			pavatar->equipItem(POS_ID_LEFTHAND, info.userdata[3], 0);
			pavatar->equipItem(POS_ID_RIGHTHAND, info.userdata[4], 0);
			pavatar->equipItem(POS_ID_SHOULD, info.userdata[6], 0);
			pavatar->equipItem(POS_ID_HEAD, info.userdata[11], 0);
			int left = info.userdata[3]/10000, right = info.userdata[4]/10000;
			pavatar->setAvatarAttrib(0, info.race);
			pavatar->setAvatarAttrib(1, info.gender);
			if (right == ARM_SWORD || right == ARM_KNIFE)
			{
				if (left == ARM_SWORD_LEFT)
				{
					PoseState = POSE_WEAPON_SHORTWEAPON;
				}
				else
				{
					PoseState = POSE_SHORTWEAPON_R;
				}
			}
			else if (right == ARM_BOTHSWORD)
			{
				PoseState = POSE_WEAPON_SHORTWEAPON;
			}
			else if (right == ARM_HEVY_SCATTERGUN)
			{
				PoseState = POSE_SCATTERGUN;
			}
			else if (right == ARM_BITH_GUN)
			{
				PoseState = POSE_SCATTERGUN;
			}
			else if (right == ARM_RIGHT_GUN)
			{
				if (left == ARM_GUNNER_WEAR_LEFT)
				{
					PoseState = POSE_DOUBLEGUN;
				}
				else
				{
					PoseState = POSE_GUN_R;
				}
			}
			else if (right / 10 == ARM_WIZARD)
			{
				PoseState = POSE_WAND;
			}
			else if (right / 10 == ARM_CHURCH)
			{
				PoseState = POSE_CHURCH_WAND;
			}
			pavatar->updateAttrib();

			Effigy_Career[i] = pavatar;
		}
		Effigy_Career[i]->SetForceShadowMapCull(true);
		Effigy_Career[i]->attachToScene(map->getShowGameMap()->getGameScene());
		int x, y, z;
		x = m_EffigyCareerInfo[i].Pos.X * 10;
		z = m_EffigyCareerInfo[i].Pos.Y * 10;
		int trans = i%2 == 0 ? 90.0f : -90.0f;
		map->getTerrainH(x, z, y);
		Effigy_Career[i]->setPosition(GameXYZ(x, y, z));
		Effigy_Career[i]->setScale(2.5f);
		Effigy_Career[i]->setRotation(trans, 0, 0);
		Effigy_Career[i]->playAnim(PoseState * 1000 + ACTSTA_NORMAL * 100 + ACTSTA_NORMAL_STAND3);
		Effigy_Career[i]->update(60000);
		Effigy_Career[i]->resetUpdate(true);

	}
}

void GameTopCareer::SetEffigyStarEffect( Ogre::Entity *Effigy, int nStar )
{
	char szBuf[256];
	if ( nStar == 9 )
	{
		sprintf( szBuf, "wanmeiX9");
	}
	else if ( nStar == 8 )
	{
		sprintf( szBuf, "wanmeiX8");
	}
	else if ( nStar == 7 )
	{
		sprintf( szBuf, "wanmeiX7");
	}
	else if ( nStar == 6 )
	{
		sprintf( szBuf, "wanmeiX6");
	}
	else
	{
		Effigy->stopMotion( 30006 );
		return;
	}
	Effigy->playMotion(szBuf, true, 30006);
}

void GameTopCareer::SetEffigyCareerInfo( int idx, const tagDFFirstInfo info )
{
	m_EffigyCareerInfo[idx-1] = info;
}

void GameTopCareer::SetCareerChampionInfo( int idx, const tagDFFirstInfo info )
{
	m_CareerChampionInfo[idx-1] = info;
}

int GameTopCareer::getCareerChampionRace( int careerIdx )
{
	if( strcmp( m_CareerChampionInfo[careerIdx].RoleName, "" ) == 0 )
	{
		int resIDs[MAX_SUPPORT_MEM_NUM] = {202114,202110,202117,202113,202112,202116,202111,202115};
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resIDs[careerIdx]);
		return info.race;
	}
	else
	{
		return m_CareerChampionInfo[careerIdx].RoleCareer;
	}
}

int GameTopCareer::getCareerChampionGender( int careerIdx )
{
	if( strcmp( m_CareerChampionInfo[careerIdx].RoleName, "" ) == 0 )
	{
		int resIDs[MAX_SUPPORT_MEM_NUM] = {202114,202110,202117,202113,202112,202116,202111,202115};
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resIDs[careerIdx]);
		return info.gender;
	}
	else
	{
		return m_CareerChampionInfo[careerIdx].RoleGender-1;
	}
}

int GameTopCareer::getCareerChampionAvatarId( int careerIdx, int posIdx )
{

	if( strcmp( m_CareerChampionInfo[careerIdx].RoleName, "" ) == 0 )
	{
		if ( posIdx < POS_ID_HAIR )
		{
			return 0;
		}
		//DIANFENG_JOB_TYPE顺序
		int resIDs[MAX_SUPPORT_MEM_NUM] = {202114,202110,202117,202113,202112,202116,202111,202115};
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resIDs[careerIdx]);
		return info.userdata[posIdx-2];
	}
	else
	{
		for( int i = 0; i<MAX_AVATAR; i++ )
		{
			if ( posIdx == POS_ID_HEAD )
			{
				return m_CareerChampionInfo[careerIdx].RoleHead;
			}
			else if ( posIdx == POS_ID_HAIR )
			{
				return m_CareerChampionInfo[careerIdx].RoleHair;
			}
			else if ( posIdx == POS_ID_FACE )
			{
				return m_CareerChampionInfo[careerIdx].RoleFace;
			}
			int armPosID = EquipPos_2_PosId[m_CareerChampionInfo[careerIdx].RoleAvatar.Arms[i].GridIdx];
			if ( armPosID == posIdx && i < m_CareerChampionInfo[careerIdx].RoleAvatar.Num )
			{
				return m_CareerChampionInfo[careerIdx].RoleAvatar.Arms[i].ArmDefID;
			}
		}
	}
	return 0;
}

int GameTopCareer::getCareerChampionAvatarStarLv( int careerIdx, int posIdx )
{
	if( strcmp( m_CareerChampionInfo[careerIdx].RoleName, "" ) != 0 )
	{
		for( int i = 0; i<MAX_AVATAR; i++ )
		{
			if ( posIdx + 2 == POS_ID_HEAD || posIdx + 2 == POS_ID_HAIR || posIdx + 2 == POS_ID_FACE )
			{
				return 0;
			}
			int armPosID = EquipPos_2_PosId[m_CareerChampionInfo[careerIdx].RoleAvatar.Arms[i].GridIdx];
			if ( armPosID == posIdx + 2 )
			{
				return m_CareerChampionInfo[careerIdx].RoleAvatar.Arms[i].StarLv;
			}
		}
	}
	return 0;
}

const char* GameTopCareer::getCareerChampionName( int careerIdx )
{
	return m_CareerChampionInfo[careerIdx].RoleName;
}

const char* GameTopCareer::getCareerChampionClanName( int careerIdx )
{
	return ClanInfo::getSingleton().getClanName( (int)m_CareerChampionInfo[careerIdx].ClanId );
}

const char* GameTopCareer::getSupportNameByIdx( int idx )
{
	return m_GuessMingDan.SupportMingDan[idx].Name;
}

int GameTopCareer::getGuessGroupNum()
{
	return m_CurCareerGroupInfo.MemNum;
}

const char* GameTopCareer::getGuessGroupNameByIndex( int idx )
{
	return m_CurCareerGroupInfo.GroupMemInfo[idx].RoleName;
}

const char* GameTopCareer::getGuessGroupClanNameByIndex( int idx )
{
	return ClanInfo::getSingleton().getClanName( (int)m_CurCareerGroupInfo.GroupMemInfo[idx].ClanID );
}

int GameTopCareer::getGuessGroupSupportNumByIndex( int idx )
{
	return m_CurCareerGroupInfo.GroupMemInfo[idx].YiSupportNum ;
}

void GameTopCareer::SelectTopCareerWiner( int groupIdx, int idx )
{
	if ( idx < 0 )
	{
		memset( &m_SupportMem, 0, sizeof(tagSupportMem) );
	}
	m_SupportMem[groupIdx].RoleID = m_CurCareerGroupInfo.GroupMemInfo[idx].RoleID;
}
bool GameTopCareer::IsTopCareerSelected( int groupIdx )
{
	return m_SupportMem[groupIdx].RoleID == 0 ? false : true;
}

void GameTopCareer::requestTopCareerSupportInfo( int idx )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_DIANFENG_GROUP_INFO_C;
	msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengGroupInfoC.DFversion = 0;
	msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengGroupInfoC.WhichOne = idx;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameTopCareer::requestGuessTopCareer()
{
	tagCSPkgBody msgbody;
	MainGamePlayer *pMain = ActorManager::getSingleton().getMainPlayer();
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_DIANFENG_JING_CAI;
	strncpy( msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengJingCai.GuessRoleName, pMain->getName(), ROLE_NAME_LEN );
	msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengJingCai.Num = MAX_SUPPORT_MEM_NUM;
	memcpy( msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengJingCai.SupportMem, m_SupportMem, sizeof(SUPPORTMEM)*MAX_SUPPORT_MEM_NUM );
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameTopCareer::requestJoinTopCareer()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_DIANFENG_BAOMING_CLT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengBaoMing.BaoMingReq = 1;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameTopCareer::requestTopCareerTrans()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_DIANFENG_TRANSMIT_CLT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengTransmit.TransmitReq = 1;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameTopCareer::requestTopCareerIfGuessed()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_DIANFENG_IF_GUESS_CLT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.DianFengIfGuess.IfGuessReq = 1;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}