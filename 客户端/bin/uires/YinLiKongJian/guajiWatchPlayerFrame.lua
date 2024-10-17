MAX_SUITNUM	= 6;
MAX_BASE_ATTR_NUM	= 7;
MAX_ATTACK_ATTR_NUM	= 8;
MAX_DEFEND_ATTR_NUM	= 6;
MAX_SUIT_ATTR_NUM	= 16;
MAX_BOTTOM_LEAVE_SPACE = 5;

function GetPoseState( armid )
	if armid ~= 0 then
		local arm_sub_type  = math.floor(armid/10000);
		local arm_race_type = math.floor(armid/100000);
		for i = 1, table.getn( t_posestate ) do
			if arm_race_type == t_posestate[i][1] and arm_sub_type == t_posestate[i][2] then
				if t_posestate[i][3]  > POSE_END then
					return 10108; -- 各职业的基本备战动作 后三位108
				else
					return (tonumber(t_posestate[i][3])*1000)+100+tonumber(t_posestate[i][4]); -- 各职业的穿装备的备战动作 后三位108
				end
			end
		end
	end

	return 10108;
end

function GetRoleAttr( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end
	local nValue = actor:getShortAttriValule( nAttrID );
	if nAttrID == ATTR_ID_DAMAGE then
		nValue = math.ceil(nValue/100);
	elseif nAttrID == ATTR_ID_MSPD then
		nValue = math.ceil(nValue/400);
	end
	return nValue;
end

function GetRoleBaseAttr( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getBaseAttriValue( nAttrID );
	
	if nAttrID == ATTR_ID_MSPD then
		nValue = math.ceil(nValue/400);
	end
	return nValue;
end

function GetMulAndAddRoleAttr( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];

	local nAddValue = actor:getShortAttriValule( nAddAttrID );
	local nMulValue = actor:getShortAttriValule( nMulAttrID );
	return nAddValue + nMulValue * nAddValue;
end

function GetRoleMulAndAddBaseAttr( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];

	local nAddValue = actor:getBaseAttriValue( nAddAttrID );
	local nMulValue = actor:getBaseAttriValue( nMulAttrID );
	return nAddValue + nMulValue * nAddValue;
end

function GetMulAndAddRoleAttrDesc( data, actor )
	local nAddAttrID = data["add"];
	local nMulAttrID = data["mul"];
	
	local nAddValue = actor:getShortAttriValule( nAddAttrID );
	local nMulValue = actor:getShortAttriValule( nMulAttrID );
	if nMulValue == 0 then
		return nAddValue;
	else
		return nAddValue.." + "..nMulValue.."%" ;
	end
end

function GetRoleBaoJiDamageAttrDesc( nAttrID, actor )
	local nValue = actor:getShortAttriValule( nAddAttrID );
	return nValue.."%";
end

function GetRoleHPandMPAttrDesc( nAttrID, actor )
	if nAttrID == ATTR_ID_MPMAX then
		local nValue	= actor:getShortAttriValule( ATTR_ID_MP );
		local nMaxValue	= actor:getShortAttriValule( ATTR_ID_MPMAX );
		return nValue.."/"..nMaxValue;
	elseif nAttrID == ATTR_ID_HPMAX then
		local nValue	= actor:getShortAttriValule( ATTR_ID_HP );
		local nMaxValue	= actor:getShortAttriValule( ATTR_ID_HPMAX );
		return nValue.."/"..nMaxValue;
	end
end

function GetRoleMovementAttrDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = GetRoleCurMovementAttr( nAttrID, actor )
	nValue = math.ceil( nValue * 100 / CLIENT_BASE_MSPD);
	return nValue.."%";
end

function GetRoleBaseMovementAttr( nAttrID, actor )
	return CLIENT_BASE_MSPD;
end

function GetRoleCurMovementAttr( nAttrID, actor )
	return actor:getSpeed();
end

function GetRoleAttrPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getIntAttriValue( nAttrID );
	return nValue.."%";
end

function GetRoleAttrPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nValue = actor:getIntAttriValue( nAttrID );
	return nValue.."%";
end

function GetRoleAttrPercent( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end
	return actor:getIntAttriValue( nAttrID );
end

--[[
1、暴击率 =  协议发送的暴击率+ （暴击力/（暴击力+暴击抵抗）/3）
LevelUP表 暴击抵抗 MonDefHeavyHit
--]]

function GetRoleAttrCurBaoJiLv( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end
	local nBaoJiLvValue		= actor:getShortAttriValule( nAttrID );
	local BaoJiLiValue		= actor:getShortAttriValule( ADD_ATTR_RESULT_BASE_ATTR_HEAVYHIT );
	local levelUpDef		= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nBaoJiLvValue;
	end
	local baoJiDiKangValue	= levelUpDef.MonDefHeavyHit;
	local fResult			= nBaoJiLvValue + ( BaoJiLiValue / ( BaoJiLiValue + baoJiDiKangValue ) / 3  );
	
	return math.ceil( fResult );
end

function GetRoleAttrBaoJiLvPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nBaoJiLvValue		= actor:getIntAttriValue( nAttrID );
	local BaoJiLiValue		= actor:getIntAttriValue( ATTR_ID_BASE_ATTR_HEAVY );
	local levelUpDef		= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nBaoJiLvValue.."%";
	end
	local baoJiDiKangValue	= levelUpDef.MonDefHeavyHit;
	local fResult			= nBaoJiLvValue + ( BaoJiLiValue / ( BaoJiLiValue + baoJiDiKangValue ) / 3  ) * 100;
	fResult					= math.min( fResult, 100 );
	local interger, fraction = math.modf( fResult );
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

--[[
2、命中率 =  协议发送的命中率 + （命中力/ 参考闪避力）
LevelUP表 参考闪避力 UIBaseFlee

--]]

function GetRoleAttrCurMingZhongLv( nAttrID, actor )
	if nAttrID < 0 then
		return 0;
	end

	local nMingZhongLvValue		= actor:getShortAttriValule( nAttrID );
	local MingZhongLiValue		= actor:getShortAttriValule( ATTR_ID_HIT );
	local levelUpDef			= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nMingZhongLvValue;
	end
	local MingZhongBaseValue	= levelUpDef.UIBaseFlee;
	local fResult				= nMingZhongLvValue + ( MingZhongLiValue / MingZhongBaseValue );
	
	return math.ceil( fResult );
end

function GetRoleAttrMingZhongLvPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end

	local nMingZhongLvValue		= actor:getShortAttriValule( nAttrID );
	local MingZhongLiValue		= actor:getShortAttriValule( ATTR_ID_HIT );
	local levelUpDef			= getLevelUP( actor:getLv() );
	if levelUpDef == nil then
		return nMingZhongLvValue.."%";
	end
	local MingZhongBaseValue	= levelUpDef.UIBaseFlee;
	local fResult				= nMingZhongLvValue + ( 0.6 + MingZhongLiValue * 0.3 / MingZhongBaseValue ) * 100;
	fResult						= math.min( fResult, 100 )
	local interger, fraction = math.modf( fResult );
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

function GetRoleAttrBaoJiShangHaiPercentDesc( nAttrID, actor )
	if nAttrID < 0 then
		return "";
	end
	local ADJUST_VALUE = 150;
	local fResult	= actor:getShortAttriValule( nAttrID );
	fResult			= fResult / 100;
	
	local interger, fraction = math.modf( fResult );
	interger		= interger + ADJUST_VALUE;
	return ( interger.."."..math.floor( fraction * 100 ) ).."%";
end

t_RoleAttrInfo =
{
["基础属性"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "力量\b",		attrID = ATTR_ID_LILIANG,	desc = "1点力量增加5点物理攻击", },
		[2] = { name = "智力\b",		attrID = ATTR_ID_ZHILI,		desc = "1点智力增加5点魔法攻击", },
		[3] = { name = "体质\b",		attrID = ATTR_ID_TILI,		desc = "1点体质增加25点生命值\n提升非战斗中，生命值每3秒回复效果", },
		[4] = { name = "精神\b",		attrID = ATTR_ID_JINGSHEN,	desc = "1点精神增加25点魔法值\n提升非战斗中，魔法值每3秒回复效果", },
		[5] = { name = "敏捷\b",		attrID = ATTR_ID_FANGYU,	desc = "1点敏捷增加5点物理防御", },
		[6] = { name = "感知\b",		attrID = ATTR_ID_MINJIE,	desc = "1点感知增加5点魔法防御", },
		[7] = { name = "移动速度\b",	attrID = ATTR_ID_MSPD,		desc = "角色当前的移动速度", getAttrDescFunc = GetRoleMovementAttrDesc,
				getBaseAttrFunc = GetRoleBaseMovementAttr,			getCurAttrFunc = GetRoleCurMovementAttr },

		maxElemNum = 7,
	},
},

["攻击属性"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{	
		[1] = { name = "物理攻击\b",	attrID = ATTR_ID_PHSIC_ATK,			maxAttrID = ATTR_ID_PHSIC_MAX_ATK,			desc = "增加物理技能造成的伤害", },
		[2] = { name = "暴击等级\b",	attrID = ATTR_ID_BASE_ATTR_HEAVY,	desc = "增加攻击暴击的几率", },
		[3] = { name = "暴击伤害\b",	attrID = ATTR_ID_HEAVY_VAL,			desc = "暴击造成普通伤害的百分比", 
				getAttrDescFunc = GetRoleAttrBaoJiShangHaiPercentDesc },
		[4] = { name = "暴击率\b",		attrID = ATTR_ID_HEAVY,				desc = "对同等级怪物造成暴击的几率", 
				-- getCurAttrFunc = GetRoleAttrCurBaoJiLv
				getAttrDescFunc = GetRoleAttrBaoJiLvPercentDesc,			 
				},

		[5] = { name = "命中等级\b",	attrID = ATTR_ID_HIT,				desc = "增加攻击命中的几率", },
		[6] = { name = "命中率\b",		attrID = ATTR_ID_HIT_RATE,			desc = "对同等级怪物攻击命中的几率", 
				-- getCurAttrFunc = GetRoleAttrCurMingZhongLv  
				getAttrDescFunc = GetRoleAttrMingZhongLvPercentDesc,		
				},

		[7] = { name = "物理伤害加成\b",attrID = ATTR_ID_PHSIC_HURT,		desc = "造成物理伤害额外增加百分比", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		[8] = { name = "魔法伤害加成\b",attrID = ATTR_ID_ICE_HURT,			desc = "造成魔法伤害额外增加百分比", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		maxElemNum = 8,
	},

	[CLIENT_CAREER_MAGICOR] = 
	{
		[1] = { name = "魔法攻击\b",	attrID = ATTR_ID_ICE_ATK,			maxAttrID = ATTR_ID_PHSIC_MAX_ATK,			desc = "增加魔法技能造成的伤害", },
		[2] = { name = "暴击等级\b",	attrID = ATTR_ID_BASE_ATTR_HEAVY,	desc = "增加攻击暴击的几率", },
		[3] = { name = "暴击伤害\b",	attrID = ATTR_ID_HEAVY_VAL,			desc = "增加暴击造成的伤害", 
				getAttrDescFunc = GetRoleAttrBaoJiShangHaiPercentDesc },

		[4] = { name = "暴击率\b",		attrID = ATTR_ID_HEAVY,				desc = "对同等级怪物造成暴击的几率", 
				-- getCurAttrFunc = GetRoleAttrCurBaoJiLv  
				getAttrDescFunc = GetRoleAttrBaoJiLvPercentDesc,			
				},
		[5] = { name = "命中等级\b",	attrID = ATTR_ID_HIT,				desc = "增加攻击命中的几率", },
		[6] = { name = "命中率\b",		attrID = ATTR_ID_HIT_RATE,			desc = "对同等级怪物攻击命中的几率", 
				-- getCurAttrFunc = GetRoleAttrCurMingZhongLv
				getAttrDescFunc = GetRoleAttrMingZhongLvPercentDesc,		  },
		[7] = { name = "物理伤害加成\b",attrID = ATTR_ID_PHSIC_HURT,		desc = "造成物理伤害额外增加百分比",getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		[8] = { name = "魔法伤害加成\b",attrID = ATTR_ID_ICE_HURT,			desc = "造成魔法伤害额外增加百分比", getAttrDescFunc = GetRoleAttrPercentDesc, getCurAttrFunc = GetRoleAttrPercent },
		maxElemNum = 8,
	},
},

["防御属性"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "物理防御\b",	attrID = ATTR_ID_PHSIC_DEF,			desc = "物理防御越高，减少受到的物理伤害越多", getCurAttrFunc = GetRoleAttrPercent},
		[2] = { name = "魔法防御\b",	attrID = ATTR_ID_ICE_DEF,			desc = "魔法防御越高，减少受到的魔法伤害越多", getCurAttrFunc = GetRoleAttrPercent },
		[3] = { name = "闪避等级\b",	attrID = ATTR_ID_FLEE,				desc = "增加闪避率", },
		[4] = { name = "暴击抵抗\b",	attrID = ATTR_ID_BASE_ATTR_DEFHEAVY,desc = "减少受到暴击的几率", },
		[5] = { name = "物理伤害减免\b",attrID = ATTR_ID_DEF_PHSIC_HURT,	desc = "直接减少受到的物理伤害", },
		[6] = { name = "魔法伤害减免\b",attrID = ATTR_ID_DEF_ICE_HURT,		desc = "直接减少受到的魔法伤害", },

		maxElemNum = 6,
	},
},

["元素攻击力"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "风元素伤害\b",	attrID = ATTR_ID_FIRE_ATK,			desc = "攻击时附加风元素伤害", },
		[2] = { name = "土元素伤害\b",	attrID = ATTR_ID_WATER_ATK,			desc = "攻击时附加土元素伤害", },
		[3] = { name = "电元素伤害\b",	attrID = ATTR_ID_THUNDER_ATK,		desc = "攻击时附加电元素伤害", },
		[4] = { name = "光元素伤害\b",	attrID = ATTR_ID_LIGHT_ATK,			desc = "攻击时附加光元素伤害", },
		[5] = { name = "暗元素伤害\b",	attrID = ATTR_ID_NIGHT_ATK,			desc = "攻击时附加暗元素伤害", },
		maxElemNum = 5,
	},
},

["元素防御力"] = 
{
	[CLIENT_CAREER_WARRIOR] = 
	{
		[1] = { name = "风元素抗性\b",	attrID = ATTR_ID_FIRE_DEF,			desc = "减少受到的风元素伤害", },
		[2] = { name = "土元素抗性\b",	attrID = ATTR_ID_WATER_DEF,			desc = "减少受到的土元素伤害", },
		[3] = { name = "电元素抗性\b",	attrID = ATTR_ID_THUNDER_DEF,		desc = "减少受到的电元素伤害", },
		[4] = { name = "光元素抗性\b",	attrID = ATTR_ID_LIGHT_DEF,			desc = "减少受到的光元素伤害", },
		[5] = { name = "暗元素抗性\b",	attrID = ATTR_ID_NIGHT_DEF,			desc = "减少受到的暗元素伤害", },
		maxElemNum = 5,
	},
},
};

local bWatchPlayerFrameShowFashion = false;

function WatchPlayerFrameOnLoad()
	this:RegisterEvent("GE_WATCHPLAYER_WATCH");


	for i=1, 2, 1 do
		for j=1, 8, 1 do
			local bagButton = getglobal( "WatchPlayerFrameEquip"..((i-1)*8+j));
			--bagButton:SetPoint("TOPLEFT", "WatchPlayerFrame", "TOPLEFT", ( 14 + 5*38*(i-1) + (i-1)*22 ), ( (j-1)*43 + 45 ) );
			bagButton:SetClientUserData( 0, 0 );
		end
	end
end

function WatchPlayerFrame_Close()
	WatchPlayerFrame:Hide();
end

--@Desc:用于响应人物属性面板各种事件消息
--@return: Nothing
function WatchPlayerFrameOnEvent()
	if ( arg1 == "GE_WATCHPLAYER_WATCH" ) then
		if WatchPlayerFrame:IsShown() then
			WatchPlayerFrame:Hide();
		end
		SetWatchPlayerInfo();
		WatchPlayerFrame:Show();
	end

end

-- 观察玩家的界面帧刷新 检测玩家是否在视野范围
function WatchPlayerFrameOnUpdate()
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	if WatchPlayer == nil then
		WatchPlayerFrame:Hide();
	end
	--UpdateWPAttr();
end

function GetWatchPlayerEquipItem( WPData, GridIndex )
	for i = 1, MAX_WEAR do
		local Item = WPData.m_ItemRoleWear[i-1];
		if Item:getGridIdx() == GridIndex then
			return Item;
		end
	end
	return nil;
end

function GetWatchPlayerSuitArmNum( nSuitId )
	local t_SuitArm = {};
	local nSuitArmNum = 0;
	local nSuitTotalNum = getSuitEquipNum( nSuitId );
	for i = 1, nSuitTotalNum do
		for j = 1, MAXEQUIPPOS do
			local bagButton = getglobal( "WatchPlayerFrameEquip"..j );
			local Item = OfflineWatchPlayer:getItem( bagButton:GetClientID() );
			if getSuitSetId( nSuitId, ( i - 1 ) ) == Item:getItemId() and not Item:isValidEnd() then
				t_SuitArm[Item:getItemId()] = 1;
			end
		end
	end
	for _,v in pairs( t_SuitArm ) do
		nSuitArmNum = nSuitArmNum + v;
	end
	return nSuitArmNum;
end

--@desc:玩家信息显示函数
--@return: Nothing
function WatchPlayerFrameOnShow()  
	-- 重置窗口坐标
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	WatchPlayerFrame:SetPoint("topleft", "$parent", "topleft", 80,12 );
	--SetWatchPlayerInfo();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
	WatchPlayerFrame_ShouSuoBtn:Show();
	WatchPlayerFrame_ZhanKaiBtn:Hide();
	ShowWatchPlayerFashionFrames( false );
	WatchPlayerFrame_FashionBtn:DisChecked();
	WatchPlayerFrame_EquipBtn:Checked();
	ShowWatchPlayerFashionFrames( false );
end

function SetWatchPlayerInfo()
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	if WatchPlayer == nil then
		return;
	end

	for i=1, MAXEQUIPPOS do
		local bagButton = getglobal( "WatchPlayerFrameEquip"..i);
		local UVAnimationTex = getglobal( "WatchPlayerFrameEquip"..i.."UVAnimationTex");
		local ItemTex = getglobal( "WatchPlayerFrameEquip"..i.."IconTexture");
		local ItemBoxTex = getglobal( "WatchPlayerFrameEquip"..i.."BoxTexture");
		local redTexture = getglobal( "WatchPlayerFrameEquip"..i.."RedTexture" );
		local BindTexture = getglobal( "WatchPlayerFrameEquip"..i.."BindTexture" );
		local Item = OfflineWatchPlayer:getItem( bagButton:GetClientID() );
		if( Item:getItemId() ~= 0 ) then
			local itemdef = Item:getItemDef();
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName(), Item ); -- 设置装备特效
			bagButton:SetClientUserData(3,CONTAINER_TYPE_WEAR);

			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ItemTex:SetTexture( IconPath );

			ItemBoxTex:Show();
			if Item:isValidEnd() then
				redTexture:Show()
			else
				redTexture:Hide();
			end
			if Item:isItemBind() then
				BindTexture:Show();
			else
				BindTexture:Hide();
			end
		else
			UVAnimationTex:Hide();
			ItemBoxTex:Hide();
			redTexture:Hide();
			BindTexture:Hide();
			ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		end
	end

	--显示玩家主要消息
	local nLv		= WatchPlayer:getLv();
	local szName	= WatchPlayer:getName();

	--显示面板标题
	WatchPlayerFrameTitleFont:SetText( szName );

	local nCurExp = GetWPAttr( ATTR_ID_EXP );
	local nLevelNextExp = WatchPlayer:getLvExp( nLv );

	WatchPlayerFrame_CurExp:SetText( nCurExp );
	WatchPlayerFrame_NextUpLvExp:SetText( nLevelNextExp );

	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	
	ModelViewer:setEntityAvatar(0);
	OfflineWatchPlayer:setAvatarAttrib();
	if bWatchPlayerFrameShowFashion then
		setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	end
	for j=1, 32 do
		if getAvatarAttrib( j-1 ) ~= 0 then
			if j == 1 then
				ModelViewer:setAvatarAttrib(0, j-1, getAvatarAttrib( j-1 ),getAvatarStarLv( j-1 ), true );
			else
				ModelViewer:setAvatarAttrib(0, j-1, getAvatarAttrib( j-1 ),getAvatarStarLv( j-1 ) );
			end
		end
	end
	ModelAngle = 0;
	ModelViewer:setEntityRotate(0,0);
	ModelViewer:playEntityAnim(0, 10100, nil );
	local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
	local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );

	if leftarmid ~= 0 or rightarmid ~= 0 then
		ModelViewer:playEntityAnim(0, GetPoseState( leftarmid ), nil );
		ModelViewer:playEntityAnim(0, GetPoseState( rightarmid ), nil );
	end
	--男性小点
	if WatchPlayer:getGender() == 1 then
		ModelViewer:setPetScale(0,0.8,0.8,0.8);--设置缩放
	else
		ModelViewer:setPetScale(0,0.9,0.9,0.9);--设置缩放
	end	
	ModelViewer:setEntityPosition(0,0,20,0);
	WPAttrBtn:Show();
end

function WatchPlayerFrameOnHide()
	if NormalItemTipsFrame:IsShown() then
		if IsInFrameAbs( "NormalItemTipsFrame", "PlayerContainerFrame" ) then
			NormalItemTipsFrame:Hide();
		end
	end
end

function WPActiveButton_OnEnter()
end

function WPActiveButton_OnLeave()
	GameTooltip:Hide();
end

function WatchPlayerFrame_PetBtn_OnClick()
	RideFrame:Hide();
	WatchPlayerFrame:Hide();
	EquipRealX	= WatchPlayerFrame:GetRealLeft();
	EquipRealY	= WatchPlayerFrame:GetRealTop();
	PetFrame:Show();
end

--@desc:点击左转按钮时向左转动预览模型
--@return: Nothing
function WatchPlayerFrame_TurnLeftBtn_OnMouseDown()
	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelAngle = ModelAngle - 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

--@desc:点击左右转按钮时向右转动预览模型
--@return: Nothing
function WatchPlayerFrame_TurnRightBtn_OnMouseDown()
	local ModelViewer = getglobal("WatchPlayerFrame_ModelView");
	ModelAngle = ModelAngle + 5;
	ModelViewer:setEntityRotate(0,ModelAngle);
end

function WPAttrBtn_CloseBtn_OnClick()
	WPAttrBtn:Hide();
end

function GetWPAttr( nIndex )
	if nIndex == 0 then
		return "";
	end

	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	if WatchPlayer == nil then
		return 0;
	end
	local nValue = WatchPlayer:getShortAttriValule( nIndex );
	if nIndex == ATTR_ID_DAMAGE then
		nValue = math.ceil(nValue/100);
	end
	return nValue;
end

function WPAttrBtn_OnShow()
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();
	UpdateWPAttr();
	if not bWatchPlayerFrameShowFashion then
		WatchPlayerFrame_ShouSuoBtn:Show();
	else
		WatchPlayerFrame_ShouSuoBtn:Hide();
	end
	WatchPlayerFrame_ZhanKaiBtn:Hide();
end

function WPAttrBtn_OnHide()
	if not bWatchPlayerFrameShowFashion then
		WatchPlayerFrame_ZhanKaiBtn:Show();
	else
		WatchPlayerFrame_ZhanKaiBtn:Hide();
	end
	WatchPlayerFrame_ShouSuoBtn:Hide();
	WatchPlayerFrame_ZhanKaiBtn:Show();
end

function WPAttrBtn_AdvanceAttrLabelBtn_OnClick()
	WPAttrBtn_AdvanceAttrLabelBtn:Checked();
	WPAttrBtn_SpecialAttrLabelBtn:DisChecked();

	UpdateWatchNormalAttackAttr();
	UpdateWatchNormalDefendAttr();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
end

function WPAttrBtn_SpecialAttrLabelBtn_OnClick()
	WPAttrBtn_AdvanceAttrLabelBtn:DisChecked();
	WPAttrBtn_SpecialAttrLabelBtn:Checked();

	UpdateWatchSpecialAttackAttr();
	UpdateWatchSpecialDefendAttr();

	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("元");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("素");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("攻");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("击");
	WPAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("力");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("元");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("素");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("防");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("御");
	WPAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("力");end

-- 更新基础属性
function ClearWatchBaseAttr()
	for i = 1, MAX_BASE_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_BaseAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchBaseAttr()
	ClearWatchBaseAttr();
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	local t_attrList	= t_RoleAttrInfo["基础属性"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];
	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local btn = getglobal( "WPAttrBtn_BaseAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新普通攻击属性
function ClearWatchAttackAttr()
	for i = 1, MAX_ATTACK_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchNormalAttackAttr()
	ClearWatchAttackAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	local t_attrList	= t_RoleAttrInfo["攻击属性"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local nCurMaxValue  = nil;
		local nBaseMaxValue = nil;

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新普通防御属性
function ClearWatchDefendAttr()
	for i = 1, MAX_DEFEND_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateWatchNormalDefendAttr()
	ClearWatchDefendAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();

	
	local t_attrList	= t_RoleAttrInfo["防御属性"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊攻击属性
function UpdateWatchSpecialAttackAttr()
	ClearWatchAttackAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	
	local t_attrList	= t_RoleAttrInfo["元素攻击力"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "WPAttrBtn_AttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊防御属性
function UpdateWatchSpecialDefendAttr()
	ClearWatchDefendAttr();
	
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	
	local t_attrList	= t_RoleAttrInfo["元素防御力"][1];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	WatchPlayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	WatchPlayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	WatchPlayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	WatchPlayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "WPAttrBtn_DefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], WatchPlayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新套装属性
function ClearWatchSuitAttr()
	for i = 1, MAX_SUIT_ATTR_NUM do
		local btn = getglobal( "WPAttrBtn_SuitAttrBtn"..i );
		btn:Hide();
	end
	
	local t_hideIcons = { "WPAttrBtnSuitBackGroundButton", };
	util.HideIcons( t_hideIcons );
end

function UpdateWatchSuitAttr()
	ClearWatchSuitAttr();
	
	local t_SuitList = {};
	local isHaveSuitFunc = 
	function ( itemDef )
		for _, data in ipairs( t_SuitList ) do
			if itemDef.SuitSetID == data["suitID"] then
				return true;
			end
		end

		return false;
	end
	
	for i = 1, MAXEQUIPPOS do
		local bagButton	= getglobal( "WatchPlayerFrameEquip"..i);
		local Item		= OfflineWatchPlayer:getItem( bagButton:GetClientID() );
		local itemDef	= Item:getItemDef();
		if itemDef ~= nil and itemDef.SuitSetID > 0 then
			if not isHaveSuitFunc( itemDef ) then
				local SuitSet		= getSuitSet( itemDef.SuitSetID );
				local t_SuitColor	= GetItemNameColor(itemDef);

				local t_oneSuitElem = 
				{	name	= SuitSet.Name,
					value	= GetWatchPlayerSuitArmNum( itemDef.SuitSetID ) .. "/"..getSuitEquipNum( itemDef.SuitSetID ),
					color	= { r = t_SuitColor[1], g = t_SuitColor[2], b = t_SuitColor[3] },
					suitID	= itemDef.SuitSetID,
				};
				table.insert( t_SuitList, t_oneSuitElem );
			end
		end
	end
	
	-- 先设置装备按钮数据
	for i = 1, table.getn( t_SuitList ) do
		local oneSuitData	= t_SuitList[i];
		local btn			= getglobal( "WPAttrBtn_SuitAttrBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneSuitData["name"],	color = oneSuitData["color"],  }, 
							[btn:GetName().."ValueFont"]	= {	value = oneSuitData["value"],	color = oneSuitData["color"], },
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local ONE_SUIT_BTN_HEIGHT			= 16;
	local EXTEND_FRAME_SIZE_LIMIT_NUM	= 2;
	local nSuitNum	= table.getn( t_SuitList );
	if nSuitNum > 0 then
		WPAttrBtnSuitBackGroundButton:Show();
		WPAttrBtnSuitBackGroundButton:SetHeight( math.max( EXTEND_FRAME_SIZE_LIMIT_NUM, table.getn( t_SuitList ) ) * ONE_SUIT_BTN_HEIGHT );
	end
	
	if nSuitNum > EXTEND_FRAME_SIZE_LIMIT_NUM then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nSuitNum * 16;
		WPAttrBtn:SetHeight( size );
	else
		WPAttrBtn:SetHeight( 456 );
	end
	
end

function WPAttrBtn_NodeBtn_OnEnter()
	local szText = this:GetClientString();
	if szText == "" then
		return;
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "top" } );
end

function WPAttrBtn_NodeBtn_OnLeave()
	GameTooltip:Hide();
end

function UpdateWPAttr()
	local WatchPlayer = nil;
	WatchPlayer = OfflineWatchPlayer:getWatchPlayer();
	if WatchPlayer == nil then return end

	--显示玩家主要消息
	local nLv			=	WatchPlayer:getLv();
	local nCurExp			=	WatchPlayer:getShortAttriValule( ATTR_ID_EXP );
	local MaxHp			=	WatchPlayer:getShortAttriValule( ATTR_ID_HPMAX );
	local MaxMp			=	WatchPlayer:getShortAttriValule( ATTR_ID_MPMAX );
	local nLevelNextExp =	WatchPlayer:getLvExp( nLv );

	local t_fonts = 
	{ 
		["WPAttrNameFont"]				= { value = WatchPlayer:getName() }, 
		["WPAttrLvFont"]				= { value = "等级："..WatchPlayer:getLv().."级" }, 
		["WPAttrRaceFont"]				= { value = "职业："..GetCareerName( WatchPlayer:getRace() ) }, 
		["WatchPlayerFrame_CurExp"]		= { value = nCurExp }, 
		["WatchPlayerFrame_NextUpLvExp"]= { value = nLevelNextExp }, 
	};
	util.SetFonts( t_fonts );
	
	-- 更新HP和MP条
	local MAX_HP_LENGTH = 152;
	--显示面板标题
	-- 设置HP
	local maxHp		= WatchPlayer:getMaxHP();
	local hp		= WatchPlayer:getHP();
	hp				= math.max( math.min( hp, maxHp ), 0 );
	local fHpScale	= math.max( math.min( hp / maxHp, 1 ), 0 );
	local hpBtn		= getglobal( "WPAttrBtnFirstBaseLifeAttrButton" );
	local hpTex		= getglobal(  hpBtn:GetName().."Tex" );
	hpTex:SetTexUV( ( 1 - fHpScale ) * MAX_HP_LENGTH, 784, MAX_HP_LENGTH, 11 );
	hpTex:setMask( [[uires\ui\mask\RenWuShuXingMoFa,ShengMing  mask.tga]]);

	-- 设置MP
	local maxMp		= WatchPlayer:getMaxMP();
	local mp		= WatchPlayer:getMP();
	mp				= math.max( math.min( mp, maxMp ), 0 );
	local fMpScale	= math.max( math.min( mp / maxMp, 1 ), 0 );
	local mpBtn		= getglobal( "WPAttrBtnFirstBaseMagicAttrButton" );
	local mpTex		= getglobal(  mpBtn:GetName().."Tex" );
	mpTex:SetTexUV( ( 1 - fMpScale ) * MAX_HP_LENGTH, 796, MAX_HP_LENGTH, 11 );
	mpTex:setMask( [[uires\ui\mask\RenWuShuXingMoFa,ShengMing  mask.tga]]);

	local t_fonts = {	[hpBtn:GetName().."PercentFont"] = { value = hp.."/"..maxHp }, 
						[mpBtn:GetName().."PercentFont"] = { value = mp.."/"..maxMp },
					};
	util.SetFonts( t_fonts );

	UpdateWatchBaseAttr();
	-- 若选择的高级属性	IsEnable
	if WPAttrBtn_AdvanceAttrLabelBtn:IsChecked() then
		UpdateWatchNormalAttackAttr();
		UpdateWatchNormalDefendAttr();
	else
		UpdateWatchSpecialAttackAttr();
		UpdateWatchSpecialDefendAttr();
	end

	UpdateWatchSuitAttr();
end
function ShowWatchPlayerFashionFrames( bShow )
	if bShow then
		for i=1,7 do
			local tex = getglobal( "WatchPlayerFrameLeftEquipbackground" .. i );
			tex:Hide();
			tex = getglobal( "WatchPlayerFrameRightEquipbackground" .. i );
			tex:Hide();
		end
		for i=1,16 do
			local btn = getglobal( "WatchPlayerFrameEquip" .. i );
			btn:Hide();
		end
		-- 经验
		WatchPlayerFrameExpBkg:Hide();
		WatchPlayerFrameExpBkg1:Hide();
		WatchPlayerFrame_CurExpDesc:Hide();
		WatchPlayerFrame_NextUpLvExpDesc:Hide();
		WatchPlayerFrame_CurExp:Hide();
		WatchPlayerFrame_NextUpLvExp:Hide();

		WatchPlayerFrameFashionBkg:Show();
		for i=1,9 do
			local tex = getglobal( "WatchPlayerFrameFashionItemBkg" ..i );
			tex:Show();
		end
		WatchPlayerFrameEquip8:Show();
		WatchPlayerFrameEquip17:Show();
		WatchPlayerFrameEquip18:Show();
		WatchPlayerFrameEquip19:Show();
		WatchPlayerFrameEquip20:Show();
		WatchPlayerFrameEquip21:Show();
		WatchPlayerFrameEquip22:Show();
		WatchPlayerFrame_ZhanKaiBtn:Hide();
		WatchPlayerFrame_ShouSuoBtn:Hide();
	else
		for i=1,7 do
			local tex = getglobal( "WatchPlayerFrameLeftEquipbackground" .. i );
			tex:Show();
			tex = getglobal( "WatchPlayerFrameRightEquipbackground" .. i );
			tex:Show();
		end
		for i=1,16 do
			local btn = getglobal( "WatchPlayerFrameEquip" .. i );
			btn:Show();
		end
		-- 经验
		WatchPlayerFrameExpBkg:Show();
		WatchPlayerFrameExpBkg1:Show();
		WatchPlayerFrame_CurExpDesc:Show();
		WatchPlayerFrame_NextUpLvExpDesc:Show();
		WatchPlayerFrame_CurExp:Show();
		WatchPlayerFrame_NextUpLvExp:Show();

		WatchPlayerFrameFashionBkg:Hide();
		for i=1,9 do
			local tex = getglobal( "WatchPlayerFrameFashionItemBkg" ..i );
			tex:Hide();
		end
		WatchPlayerFrameEquip8:Hide();
		WatchPlayerFrameEquip17:Hide();
		WatchPlayerFrameEquip18:Hide();
		WatchPlayerFrameEquip19:Hide();
		WatchPlayerFrameEquip20:Hide();
		WatchPlayerFrameEquip21:Hide();
		WatchPlayerFrameEquip22:Hide();
		if WPAttrBtn:IsShown() then
			WatchPlayerFrame_ShouSuoBtn:Show();
			WatchPlayerFrame_ZhanKaiBtn:Hide();
		else
			WatchPlayerFrame_ShouSuoBtn:Hide();
			WatchPlayerFrame_ZhanKaiBtn:Show();
		end	
	end
	bWatchPlayerFrameShowFashion = bShow;
	WatchPlayerFrameEquip7:Hide();
end

function WatchPlayerFrameEquipBtn_OnClick()
	if not bWatchPlayerFrameShowFashion then
		return
	end
	ShowWatchPlayerFashionFrames( false );
	WatchPlayerFrame_FashionBtn:DisChecked();
	WatchPlayerFrame_EquipBtn:Checked();

end

function WatchPlayerFrameFashionBtn_OnClick()
	if bWatchPlayerFrameShowFashion then
		return
	end
	ShowWatchPlayerFashionFrames( true );
	WatchPlayerFrame_FashionBtn:Checked();
	WatchPlayerFrame_EquipBtn:DisChecked();
end
