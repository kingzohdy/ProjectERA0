local g_troopFrameButtons = { "TroopMemberButton", "ApplicationListButton", "AroundPlayerButton" };
SkillNilIcon = ""

SCT_NONE	= 0;
SCT_SKILL	= 1;
SCT_ITEM	= 2;
SCT_RIDE	= 3;
SHORTCUT_NONE  = 0;
SHORTCUT_SKILL = 1;
SHORTCUT_ITEM  = 2;
SHORTCUT_RIDE  = 3;
SHORTCUT_FAIRYSKILL = 4;
SHORTCUT_FAIRYDIATHESIS = 8;
RUNE_OPEN_LV	= 40;
MAX_SHORCUT	 = 42;
local MAX_XPSHORCUT	 = 3;
local SHORCUT_XP	 = 1001;
local MAX_XP_LV		 = 3;
local startx		 = 345;

local bReceiveAttr		= true;
local nLastTime			= nil;
local bRecond			= true;
local t_RideInfo		= {};

local nXPDuration		= 0;
local nStartXpTime		= 0;
local bStartXpUse		= false;
local bShowEffec		= false;

local nCurXPValue		= 0;
local nCurRealXp		= 0;
local nLastXpValue		= 0;

local nCurMouseSHortcutIndex;
local YEGEXP_DURATION	= 1;

local team_invite_clock		= 0;
local team_invite_duration	= 15;

local t_SkillId		= {};
local t_XPSkill		= {};
local EXP_TEX_WIDTH = 725;
local nLastExp		= 0;
local nLastXP		= 0;
local nStartXPEffect = 0;

local t_XpBtnInfo = {};

local fengyinLv,fengyinEnd = 0,0	-- 等级封印

local MAX_HIDE_BTN_NUM = 5
nHideShortBtnNum = MAX_HIDE_BTN_NUM;

local MAX_SHORTCUT_BTN_NUM = 42;
function InitShortCutBtns()
	PetButton:Hide();
	RideShortcutButton:Hide();
	SparButton:Hide();
	SecondarySkillShortcutButton:Hide();
	TianFuShortcutButton:Hide()

	for i = 1, MAX_SHORTCUT_BTN_NUM do
		local btn = getglobal( "MagicShortcut"..i );
		if btn ~= nil then
			btn:SetCooldownTimer( 0, 0, true );
		end
	end
	GuideEffectHide( "CharacterButton" );
	GuideEffectHide( "SparButton" )
	nHideShortBtnNum = MAX_HIDE_BTN_NUM
	SetShortCutHideTexture( MAX_HIDE_BTN_NUM );
	local t_CharacterButtonControl = GetCharacterButtonControl();
	t_CharacterButtonControl.startTime = 0
	t_CharacterButtonControl.isShowTips = false
	t_CharacterButtonControl.nextShowExp = 0
	fengyinLv,fengyinEnd = 0,0
end

function SetShortCutHideTexture( num )
	if num > nHideShortBtnNum then return end
	nHideShortBtnNum = num
	if num <= 0 then
		ShortCutFrameGearBackTexure:Hide();
		ShortCutFrameGearTexture:Hide();
		TempShortCutFrameGearBackTexure:Hide();
		TempShortCutFrameGearTexture:Hide();
		TempFairyShortCutFrameGearBackTexure:Hide();
		TempFairyShortCutFrameGearTexture:Hide();
		ClanCollectTmpFrameGearBackTexure:Hide();
		ClanCollectTmpFrameGearTexture:Hide();
		ClanTankTmpFrameGearBackTexure:Hide();
		ClanTankTmpFrameGearTexture:Hide();
	else
		local tNumPos = { 869, 843, 817, 791, 755 }
		ShortCutFrameGearTexture:SetPoint( "topleft","ShortCutFrame","topleft",tNumPos[num],5 );
		TempShortCutFrameGearTexture:SetPoint( "topleft","TempShortCutFrame","topleft",tNumPos[num], 38 );
		TempFairyShortCutFrameGearTexture:SetPoint( "topleft","TempFairyShortCutFrame","topleft",tNumPos[num], 23 );
		ClanCollectTmpFrameGearTexture:SetPoint( "topleft","ClanCollectTmpFrame","topleft",tNumPos[num], 38 );
		ClanTankTmpFrameGearTexture:SetPoint( "topleft","ClanTankTmpFrame","topleft",tNumPos[num], 38 );
		local nCutSize = (MAX_HIDE_BTN_NUM-num)*26;
		ShortCutFrameGearBackTexure:SetTexUV( 602 + nCutSize, 146, 141 - nCutSize, 44 );
		ShortCutFrameGearBackTexure:SetSize(  141 - nCutSize ,44 );
		ShortCutFrameGearBackTexure:Show()

		TempShortCutFrameGearBackTexure:SetTexUV( 602 + nCutSize, 146, 141 - nCutSize, 44 );
		TempFairyShortCutFrameGearBackTexure:SetTexUV( 602 + nCutSize, 146, 141 - nCutSize, 44 );
		ClanCollectTmpFrameGearBackTexure:SetTexUV( 602 + nCutSize, 146, 141 - nCutSize, 44 );
		ClanTankTmpFrameGearBackTexure:SetTexUV( 602 + nCutSize, 146, 141 - nCutSize, 44 );
		
		TempShortCutFrameGearBackTexure:SetSize(  141 - nCutSize ,44 );
		TempFairyShortCutFrameGearBackTexure:SetSize(  141 - nCutSize ,44 );
		ClanCollectTmpFrameGearBackTexure:SetSize(  141 - nCutSize ,44 );
		ClanTankTmpFrameGearBackTexure:SetSize(  141 - nCutSize ,44 );
		
		TempShortCutFrameGearBackTexure:Show()
		TempFairyShortCutFrameGearBackTexure:Show()
		ClanCollectTmpFrameGearBackTexure:Show()
		ClanTankTmpFrameGearBackTexure:Show()

		ShortCutFrameGearTexture:Show()
		TempShortCutFrameGearTexture:Show()
		TempFairyShortCutFrameGearTexture:Show()
		ClanCollectTmpFrameGearTexture:Show()
		ClanTankTmpFrameGearTexture:Show()
	end
end

local t_ShortCutControl = 
{ 
shorCutIdRange = { { ["min"] = 1, ["max"] = 10, }, { ["min"] = 13, ["max"] = 42, }, }, 
};

function GetShortCutControl()
	return t_ShortCutControl;
end

t_ShortCutControl["isShorCutID"] = 
function ( self, data )
	local nBtnID = data["ID"];
	
	local t_shorCutIdRange = self["shorCutIdRange"];
	for _, oneRange in ipairs( t_shorCutIdRange ) do
		if oneRange["min"] <= nBtnID and nBtnID <= oneRange["max"] then
			return true;
		end
	end

	return false;
end

function ShortCutFrame_OnLoad()
	this:RegisterEvent("GE_SHORTLIST_CHANGE");
	this:RegisterEvent("GE_SKILL_COOLD");
	this:RegisterEvent("GE_ONEOFFLINE_UPDATE");
	this:RegisterEvent("UI_ACTOR_EXP_CHG");
	this:RegisterEvent("GE_PET_PACKAGE_INIT")
	this:RegisterEvent("GE_GET_NEW_PET")
	this:RegisterEvent("GE_GET_RIDE_INFO")
	this:RegisterEvent("SECONDARY_SKILL_SVR_INFO")
	this:RegisterEvent("GE_SECONDARYSKILL_SKILL_DEL")
	this:RegisterEvent("GE_SECONDARYSKILL_SKILL_CHG")
	this:RegisterEvent("UI_ACTOR_LEVEL_CHG")
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
	
	HideEmptyShortCut();
	InitShotCutLocalVariable();

	-- HP 低于 15% 时 闪动的全屏贴图 数据初始化
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_MAIN_BUF_LIST_UPDATE");
	ShortCutFrametopright:SetAngle(90);
	ShortCutFramebottomleft:SetAngle(-90);
	ShortCutFramebottomfit:SetAngle(180);
	ShortCutFramebottomright:SetAngle(180);
	ResetShortCut();
	InitShortCutBtns();
	NetWorkStatus:SetClientString( "网络状况：良好" )
	NetWorkStatus:setUpdateTime( 2 )
end

function MoveShortcutFrameShow()
	ExtendSkillFrame:SetPoint("bottom","$parent","bottom",-384,44);
	ShortCutFrame:SetPoint("bottom","$parent","bottom",0,60); 	 
	ExtendSkillRightFrame:SetPoint("right","$parent","right",-1+100,60);   
	PhotoFrame:SetPoint( "topleft", "$parent", "topleft", -PhotoFrame:GetWidth(), 0 );
	
	PhotoFrame:MoveFrame("right", MOVE_DURATION, PhotoFrame:GetWidth(), PhotoFrame:GetHeight());
	ExtendSkillFrame:MoveFrame("top", MOVE_DURATION, ExtendSkillFrame:GetWidth(),144 );	 
	ShortCutFrame:MoveFrame("top", MOVE_DURATION, ShortCutFrame:GetWidth(), ShortCutFrame:GetHeight() );	    
	ExtendSkillRightFrame:MoveFrame("left",MOVE_DURATION, 100, ExtendSkillRightFrame:GetHeight());
	if ExtendSkillLeftFrame:IsShown() then	
		ExtendSkillLeftFrame:SetPoint("right","$parent","right",-47+100,60); 
		ExtendSkillLeftFrame:MoveFrame("left",MOVE_DURATION, 100, ExtendSkillLeftFrame:GetHeight());
	end
	ShortCutButtonsFrame:SetPoint("bottom","$parent","bottom",0,59);
	ShortCutButtonsFrame:MoveFrame("top", MOVE_DURATION, ShortCutButtonsFrame:GetWidth(), ShortCutButtonsFrame:GetHeight() );
end

function ShortCutButtonsFrame_OnShow()
	this:EndMoveFrame();
	this:SetPoint( "bottom","$parent","bottom",0,0 );
end

function ShortCutFrame_Test_OnShow()
	local tex = getglobal( "RoleExpButtonUVAnimationTex" );
	tex:SetUVAnimation( 50, true );
	tex:Show();

	tex = getglobal( "RoleExpButtonWarningUVAnimationTex" );
	tex:SetUVAnimation( 50, true );
	tex:Show();

	tex = getglobal( "RoleExpButtonWarningUVAnimationTex" );
	tex:SetUVAnimation( 50, true );
	tex:Show();

end

function ShortCutFrame_TestShowXinShou()
	local szText		= "测试快捷栏新手tips";
	local xinShouBtn	= getglobal( "ShortCutButtonsFrameXinShouBtn" );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	rich:SetClientString( szText );
	local nSelfDefineExtendWidth = 14;
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );

	local UVATexture = getglobal( "CharacterButtonUVAnimationTex" );
	UVATexture:SetUVAnimation(50,true);
	UVATexture:Show();
	xinShouBtn:Show();
end

function ShortCutFrame_OnShow()
	--CheckCharacterButtonLevelButton();
	HideGuideEffectTip();
	CheckAddFriendReqestsAfteAnim();
	CheckTeamReqestsAfterAnim();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local t_itemcd = Get_ItemCDTable();
	for i = 1, 22 do
		local btn = getglobal( "MagicShortcut" .. i );
		if btn ~= nil and btn:GetClientUserData(0) == SHORTCUT_ITEM then
			btn:SetCooldownTimer( 0, 0, true );
			for i = 1, table.getn( t_itemcd ) do	 
				if t_itemcd[i][1] == btn:GetClientUserData(1) then
					local starttime = container:getItemCDPassTime( btn:GetClientUserData(1) );	  
					btn:SetCooldownTimer( starttime, t_itemcd[i][2], true );
					btn:SetCooldownTextureRect( 3, 3, 38, 38 );						
				end
			end		
		end
	end
	NetWorkStatus:SetPoint( "topleft", "ShortCutButtonsFrame", "topleft",79,23);
end

-- 隐藏指引提示TIPS窗口
function HideShutcutFrameGuideEffectTip( szExceptFrame ) 
	-- 在szExceptFrame为nil的时候隐藏所有的tips面板
	-- 在szExceptFrame不为nil的时候将与指定tips面板不同的面板隐藏
	local t_hideGuideEffect = { "TipsEffectFrame", "PetDialogFrame" }
	for _, szFrame in ipairs( t_hideGuideEffect ) do
		local frame = getglobal( szFrame );
		if ( szExceptFrame == nil and frame:IsShown() ) 
			or ( szExceptFrame ~= nil and szExceptFrame ~= szFrame and frame:IsShown() ) then
			frame:Hide();
		end
	end
end

function InitShotCutLocalVariable()
	-- 血量低闪屏变量初始化
	bWarning	= false;
	bRecond		= false;
	nLastTime	= 0
	nStartTime	= 0;
	nLastHP		= -1;
	LIMITSCALE	= 0.15;
	bReceiveAttr = true;

	nXPDuration = 0;
	nStartXpTime = 0;
	bStartXpUse = false;

	nCurXPValue	= 0;
	nCurRealXp	= 0;
	
	t_SkillId		= {};
	t_XPSkill		= {};
	nLastXpValue	= 0;
	nLastExp		= 0;
	nLastXP		= 0;
	

	for i = 1, MAX_SHORCUT do
		local MagicShortcut			= getglobal("MagicShortcut"..i);   	  
		local MagicShortcutBoxTex	= getglobal("MagicShortcut"..i.."BoxTexture");   
		if MagicShortcut ~= nil then
			MagicShortcut:DisChecked();
			MagicShortcutBoxTex:Hide();
		end
	end
	if getglobal("MagicShortRestore") ~= nil and getglobal("MagicShortfurl") ~= nil then
		--[[
		MagicShortRestore:Hide();
		MagicShortfurl:Show();
		--]]

		MagicShortRestore:Show();
		MagicShortfurl:Hide();
		MagicShortRestore:SetPoint( "topleft","MagicShortcut42","BottomLeft", 6, -6 );	
	end

	local t_hideui = {"XPFrameYiGeUVAnimationTex","XPFrameFullUVAnimationTex"};
	for _, ui in ipairs(t_hideui) do
		local tex = getglobal(ui); 
		if tex ~= nil then
			tex:Hide();
		end
	end
	RoleExpButtonOfflineExpBtn:Hide();
end

function SetReceiveAttr( bRec )
	bReceiveAttr = bRec;
end

function HideEmptyShortCut()

end

function ShowEmptyShortCut()

end

function SetXPShortCutCDTime( xpskillid, szBtnName )
	local skillbtn = getglobal( szBtnName );
	local skillCDTime	= SkillMgr:getCDTime( xpskillid );
	local nCurtime		= GameMgr:getTickTime();
	
	skillbtn:SetCooldownTimer(skillCDTime:past() / 1000, skillCDTime:total() / 1000, true);
	if skillbtn:GetClientID() >= 43 then	 
		skillbtn:SetCooldownTextureRect( 2, 2, 26, 26 );
	else
		skillbtn:SetCooldownTextureRect( 3, 3, 38, 38 );
	end
	--[[
	if skillCDTime ~= nil then
		if nCurtime - skillCDTime.startTime > 0 then
			skillbtn:SetCooldownTimer( (nCurtime - skillCDTime.startTime)/1000, (nCurtime - skillCDTime.startTime + skillCDTime.nPublicCD)/1000, true );
			if skillbtn:GetClientID() >= 43 then	 
				skillbtn:SetCooldownTextureRect( 2, 2, 26, 26 );
			else
				skillbtn:SetCooldownTextureRect( 3, 3, 38, 38 );
			end
		end
	end]]
end

function ResetShortCut()
	-- reload
	for i =1, MAX_SHORCUT do
		if( i <= 10 or i >=13 ) then
			local MagicShortcut = getglobal("MagicShortcut"..i);
			local MagicShortcuttex = getglobal("MagicShortcut"..i.."Icon");
			local MagicShortcutLock = getglobal("MagicShortcut"..i.."Lock");  
			local FontText = getglobal("MagicShortcut"..i.."Count");
			if MagicShortcut ~= nil then
				MagicShortcut:SetClientUserData( 0, 0 );
				MagicShortcut:SetClientUserData( 1, 0 );
				MagicShortcut:SetClientUserData( 2, 0 );
				MagicShortcuttex:SetBlendAlpha( 0 );
				MagicShortcutLock:SetBlendAlpha( 0 );
				FontText:SetText("");
			end
		end
	end
	ShortCutFrame:SetPoint("bottom","$parent","bottom",0,0);
	--end
end

local bWarning		= false;
local bRecond		= false;
local nLastTime		= 0
local nStartTime	= 0;
local nLastHP		= -1;
-- 闪屏的最低血量百分比
local LIMITSCALE	= 0.15;

-- 设置右部技能栏
function SetExtendShow()

end

local t_ShortCutIconPath =
	{
		[SHORTCUT_SKILL] = "";
	}

function GetTrumpSkillLv( nTrumpSkillId )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then 
		return 1;
	end
	local GameTrump = MainPlayer:getTrump();
	local nTrumpNum = GameTrump:getTrumpMaxNum();
	for i = 1, nTrumpNum do
		local RoleTrump = GameTrump:getRoleFairy( i - 1 );
		local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );
		if RoleTrump.FairyID > 0 then 
			for i=1, FAIRY_MAX_SKILL do
				if TrumpDef.FairySkillList[i-1].SkillID == nTrumpSkillId then
					return TrumpDef.FairySkillList[i-1].SkillLevel;
				end
			end
		end	
	end
	return 1;
end

function UpdateShortCutSkill()
	for i = 1, MAX_SHORCUT do
		local MagicShortcuntBtn	= getglobal("MagicShortcut"..i);
		local FontText			= getglobal("MagicShortcut"..i.."Count");
		if MagicShortcuntBtn ~= nil and ( MagicShortcuntBtn:GetClientUserData(0) == SHORTCUT_SKILL or MagicShortcuntBtn:GetClientUserData(0) == SHORTCUT_FAIRYSKILL ) then
			local SpellLv		= SkillMgr:getSpellLv( MagicShortcuntBtn:GetClientUserData(1) );
			if  MagicShortcuntBtn:GetClientUserData(0) == SHORTCUT_FAIRYSKILL then
				SpellLv = GetTrumpSkillLv( MagicShortcuntBtn:GetClientUserData(1) )
			end
			FontText:SetText("lv"..SpellLv);
		end
	end
end

function ShortCutFrame_OnEvent()
	if( arg1 == "GE_SHORTLIST_CHANGE" )then
		local Msg = UIMSG:getUIMsg();
		--print( "ShortCutFrame_OnEvent--Msg.ShortCut.m_nNum = "..Msg.ShortCut.m_nNum );
		if Msg.ShortCut.m_nNum <= MAX_SHORCUT or Msg.ShortCut.m_nNum == 47 then
			local MagicShortcuntBtn		= getglobal("MagicShortcut"..Msg.ShortCut.m_nNum);
			local MagicShortcut			= getglobal("MagicShortcut"..Msg.ShortCut.m_nNum.."Icon");
			local MagicShortcutBoxTex	= getglobal("MagicShortcut"..Msg.ShortCut.m_nNum.."BoxTexture");
			local FontText = getglobal("MagicShortcut"..Msg.ShortCut.m_nNum.."Count");
			--print( "ShortCutFrame_OnEvent--Msg.ShortCut.m_nType = "..Msg.ShortCut.m_nType );
			if ( Msg.ShortCut.m_nType ~= 0 ) then
				if( Msg.ShortCut.m_nType == SHORTCUT_SKILL or Msg.ShortCut.m_nType == SHORTCUT_FAIRYSKILL ) then
					MagicShortcuntBtn:SetClientUserData( 0, Msg.ShortCut.m_nType );
					MagicShortcuntBtn:SetClientUserData( 1, Msg.ShortCut.m_Id );
					local skillLv = SkillMgr:getSpellLv( Msg.ShortCut.m_Id )
					assert( Msg.ShortCut.m_Id > 0 or Msg.ShortCut.m_Lv > 0,"技能ID和等级不能为0" );
					local tagSkillInfo	= SkillMgr:getSpellStaticAtt( Msg.ShortCut.m_Id, skillLv );
					if tagSkillInfo ~= nil then
						local IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga";
						-- 如果没找到物品的图标则使用默认图标显示
						if not IsInExistence( IconPath ) then
							IconPath = DEFAULT_ITEM_PATH;
						end
						MagicShortcut:SetTexture( IconPath );
					else
						local IconPath = GetSkillIconPath()..Msg.ShortCut.m_Id..".tga";
						-- 如果没找到物品的图标则使用默认图标显示
						if not IsInExistence( IconPath ) then
							IconPath = DEFAULT_ITEM_PATH;
						end
						MagicShortcut:SetTexture( IconPath );
					end

					FontText:SetText("lv".. skillLv );
					MagicShortcutBoxTex:Show();
					-- 如果是无双技能设置其冷却时间
					if SkillMgr:isXPSkill( Msg.ShortCut.m_Id ) then
						SetXPShortCutCDTime( Msg.ShortCut.m_Id, MagicShortcuntBtn:GetName() );
					end
				elseif( Msg.ShortCut.m_nType  == SHORTCUT_ITEM ) then
					local mainplayer = ActorMgr:getMainPlayer();
					local container	 = mainplayer:getContainer();
					MagicShortcuntBtn:SetClientUserData( 0, SHORTCUT_ITEM );
					MagicShortcuntBtn:SetClientUserData( 1, Msg.ShortCut.m_Id );
					local itemDef = getItemDef( Msg.ShortCut.m_Id );

					local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					MagicShortcut:SetTexture( IconPath );

					MagicShortcutBoxTex:Show();
					if container:getItemCount( Msg.ShortCut.m_Id ) > 1 then
						FontText:SetText( container:getItemCount( Msg.ShortCut.m_Id ) );
					end
				elseif( Msg.ShortCut.m_nType  == SHORTCUT_RIDE ) then
					MagicShortcuntBtn:SetClientUserData( 0, SHORTCUT_RIDE );
					MagicShortcuntBtn:SetClientUserData( 1, Msg.ShortCut.m_Id );
					local rideDef = getRideDef( Msg.ShortCut.m_Id, Msg.ShortCut.m_Lv );

					local IconPath = GetItemEquipIconPath()..rideDef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					MagicShortcut:SetTexture( IconPath );

					MagicShortcuntBtn:SetClientUserData( 2, Msg.ShortCut.m_nFittingIndex );
					MagicShortcuntBtn:SetClientUserData( 3, Msg.ShortCut.m_Lv );

					local IconPath = GetItemEquipIconPath()..rideDef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					MagicShortcut:SetTexture( IconPath );
					MagicShortcutBoxTex:Show();
					FontText:SetText("");
				end
				MagicShortcut:SetBlendAlpha( 1.0 );
			elseif ( Msg.ShortCut.m_nType == 0 ) then
				FontText:SetText( "" );
				MagicShortcut:SetBlendAlpha( 0 );
				MagicShortcuntBtn:SetClientUserData( 0, SHORTCUT_NONE );
				MagicShortcutBoxTex:Hide();
			end
		end
	end

	if(arg1 == "GE_SKILL_COOLD") then
		local Msg = UIMSG:getUIMsg();
		for i = 1, MAX_SHORCUT do
			if ( i < 11 or i > 12 ) then
				local MagicShortcut = getglobal("MagicShortcut"..i);
				if MagicShortcut:GetClientUserData(1) == Msg.CoolTime.num then
					MagicShortcut:SetCooldownTimer( Msg.CoolTime.starttime, Msg.CoolTime.endtime, true );
					if MagicShortcut:GetClientID() >= 43 then	 
						MagicShortcut:SetCooldownTextureRect( 2, 2, 26, 26 );
					else
						MagicShortcut:SetCooldownTextureRect( 3, 3, 38, 38 );
					end
					-- 如果是无双技能设置其冷却时间
					if SkillMgr:isXPSkill( Msg.CoolTime.num ) then
						SetXPShortCutCDTime( Msg.CoolTime.num, MagicShortcut:GetName() );
					end
				end
			end
		end
	end

	-- 收到 角色属性变化消息时 判断是否闪屏
	if ( arg1 == "UI_ACTOR_ATTRIBUTE_CHG" ) then
		local player = ActorMgr:getMainPlayer();
		if ( player == nil ) then
			return;
		end

		if not bReceiveAttr then
			return;
		end

		local MaxHp  = player:getMaxHP();
		local Hp     = player:getHP();
		if Hp < 0 then
			Hp = 0;
		end

		if nLastHP == -1 then
			nLastHP = Hp;
		end
		if( MaxHp ~= 0 ) and Hp < nLastHP then
			local scale = Hp / MaxHp;
			if scale < LIMITSCALE then
				bWarning	= true;
				bRecond		= true;
				nLastHP		= Hp;
				nStartTime	= os.clock();
			end
		elseif ( MaxHp ~= 0 ) and Hp > nLastHP then
			local scale = Hp / MaxHp;
			if scale >= LIMITSCALE then
				bWarning	= false;
			end
			nLastHP = Hp;
		end
	end
	if arg1 == "GE_ONEOFFLINE_UPDATE" or arg1 == "UI_ACTOR_EXP_CHG" then
		UpdateOfflineExpBtn();
	end
	if arg1 == "GE_MAIN_BUF_LIST_UPDATE" then
		if IsTrumpCamp() then
			CheckXPShow();
		end
	end
	if arg1 == "GE_PET_PACKAGE_INIT" or arg1 == "GE_GET_NEW_PET" then
		local mainplayer = ActorMgr:getMainPlayer();
		local petPackage = mainplayer:getPet();
		if petPackage:getPetNum() > 0 and not PetButton:IsShown() then
			PetButton:Show()
			SetShortCutHideTexture( 4 )
		end
	end
	if arg1 == "GE_GET_RIDE_INFO" then
		local mainplayer = ActorMgr:getMainPlayer();
		local rider	= mainplayer:getRider();
		if rider:getRidNum() > 0 and not RideShortcutButton:IsShown() then
			RideShortcutButton:Show()
			SetShortCutHideTexture( 2 )
		end
	end
	if arg1 == "SECONDARY_SKILL_SVR_INFO" or arg1 == "GE_SECONDARYSKILL_SKILL_DEL" or arg1 == "GE_SECONDARYSKILL_SKILL_CHG" then
		if SecondarySkillMgr:getSecondarySkillNum() > 0 and not SecondarySkillShortcutButton:IsShown() then
			SecondarySkillShortcutButton:Show()
			SetShortCutHideTexture( 1 )
		end
	end
	if arg1 == "UI_ACTOR_LEVEL_CHG" then
		needShowSkillTips();
		UpdateOfflineExpBtn();
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getLv() >= 60 then
			TianFuShortcutButton:Show()
			SetShortCutHideTexture( 0 )
			if mainplayer:getLv() == 60 then
				TianFuShortcutButtonUVAnimationTex:SetUVAnimation(50, true);
				TianFuShortcutButtonUVAnimationTex:Show();
			end
		end
	end
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getLv() < 20 then
			SparButton:Hide();
		elseif not SparButton:IsShown() then
			SparButton:Show();
			SetShortCutHideTexture( 3 )
		end
		if mainplayer:getLv() >= 60 then
			TianFuShortcutButton:Show()
			SetShortCutHideTexture( 0 )
		end
	end
end

local t_OffExpUV = {
						[1] = { {601,695,11,10},{601,706,11,10},{601,717,11,10} },
						[2] = { {202,636,11,10},{202,647,11,10},{202,658,11,10} },
						[3] = { {189,677,11,10},{177,677,11,10},{165,677,11,10} }
					}

function UpdateOfflineExpBtn()
	--刷新离线经验按钮位置
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local nOffExp = mainplayer:getOfflineExp();
	local nPowrldExp = mainplayer:getPworldExp();
	local levelupDef = getLevelUP( mainplayer:getLv() );
	if levelupDef == nil then return end;
	if levelupDef.MaxOfflineExp == 0 or (nOffExp == 0 and mainplayer:getLv() < 20) then
		RoleExpButtonOfflineExpBtn:Hide();
	else
		local nExp = mainplayer:getExp();
		local nNextExp = mainplayer:getLvExp( mainplayer:getLv() );
		local scale = (nExp + nOffExp)/nNextExp;
		if scale < 0 then
			scale = 0;
		end
		if scale > 1 then
			scale = 1;
		end
		RoleExpButtonOfflineExpBtn:SetPoint( "topleft","RoleExp_BackTexture","topleft",722*scale-5,-3 );
		local levelupDef = getLevelUP( mainplayer:getLv() );
		local nMaxPworldExp = levelupDef.MaxPworldOffExp;
		if nMaxPworldExp <= 0 then
			scale = 0;
		else
			scale = nPowrldExp/nMaxPworldExp*100;
		end
		local buttonUV;
		if scale < 49 then
			buttonUV = t_OffExpUV[1];
		elseif scale < 99 then
			buttonUV = t_OffExpUV[2];
		else
			buttonUV = t_OffExpUV[3];
		end
		RoleExpButtonOfflineExpBtn:Show();
		RoleExpButtonOfflineExpBtn:ChangeNormalTexture( buttonUV[1][1],buttonUV[1][2],buttonUV[1][3],buttonUV[1][4] );
		RoleExpButtonOfflineExpBtn:ChangeHighlightTexture( buttonUV[2][1],buttonUV[2][2],buttonUV[2][3],buttonUV[2][4] );
		RoleExpButtonOfflineExpBtn:ChangePushedTexture( buttonUV[3][1],buttonUV[3][2],buttonUV[3][3],buttonUV[3][4] );
	end
end

-- @Desc:Hp低于LIMITSCALE时显示闪屏警告的贴图设置
-- @Param:nFrequency 频闪间隔时间
-- @Param:nDuartion  闪屏持续时间
function SetLowHPTexture( nFrequency, nDuartion )
	local t_WarningTex =
	{
		"ShortCutFrametopfit",
		"ShortCutFramebottomfit",
		"ShortCutFrameleftfit",
		"ShortCutFramerightfit",
		"ShortCutFrametopleft",
		"ShortCutFrametopright",
		"ShortCutFramebottomleft",
		"ShortCutFramebottomright",
	}
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	if not bWarning or MainPlayer:isDead() then
		bRecond = false;
		bWarning = false;
		for i = 1, table.getn( t_WarningTex ) do
			local tex = getglobal( t_WarningTex[i] );
			tex:Hide();
		end
	end

	if bRecond then
		nLastTime = os.clock();
		bRecond = false;
		for i = 1, table.getn( t_WarningTex ) do
			local tex = getglobal( t_WarningTex[i] );
			tex:Show();
		end
	end

	-- 屏闪间隔nFrequency
	if os.clock() - nLastTime > nFrequency then
		bRecond = true;
		for i = 1, table.getn( t_WarningTex ) do
			local tex = getglobal( t_WarningTex[i] );
			tex:Hide();
		end
	end

	-- nDuartion秒后停止屏闪
	if os.clock() - nStartTime > nDuartion then
		bRecond = false;
		bWarning = false;
		for i = 1, table.getn( t_WarningTex ) do
			local tex = getglobal( t_WarningTex[i] );
			tex:Hide();
		end
	end

	-- 动态设置填充区大小
	ShortCutFrametopfit:SetWidth( GetScreenWidth() - 610 );
	ShortCutFramebottomfit:SetWidth( GetScreenWidth() - 610 );
	ShortCutFrameleftfit:SetHeight( GetScreenHeight() - 610 );
	ShortCutFramerightfit:SetHeight(GetScreenHeight() - 610 );
end

function FindShutCutBtnIndex(buttonName)
	for name, index in pairs( UIGV.SS_BtnIndex ) do
		if(name == buttonName) then
			return index;
		end;
	end

	return 0;
end

function FindBtnName(btnIndex)
	for name, index in pairs( UIGV.SS_BtnIndex ) do
		if(index == btnIndex) then
			return name;
		end;
	end

	return "";
end

--功能按键 10个，自左至右为:人物、技能、宠物、任务、交易、物品、好友、队伍、帮会、系统
--人物
function CharacterButtonOnLoad()
	this:RegisterEvent("UI_ACTOR_LEVEL_CHG");
	this:RegisterEvent("UI_ACTOR_EXP_CHG");
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
end

function CharacterButtonOnEvent()
	local MainPlayer = ActorMgr:getMainPlayer()
	if ( arg1 == "UI_ACTOR_LEVEL_CHG" or arg1 == "GE_ENTER_PLAYERLOGIN" ) and MainPlayer:getLv() == LEVEUP_LIMIT_LV then
		local nLevelNextExp = MainPlayer:getLvExp( MainPlayer:getLv() );
		GetCharacterButtonControl().nextShowExp = nLevelNextExp;
	end
	if arg1 == "UI_ACTOR_LEVEL_CHG" or arg1 == "UI_ACTOR_EXP_CHG" or arg1 == "GE_ENTER_PLAYERLOGIN" then
		CheckCharacterButtonXinShouInfo();
	end
end

function CheckCharacterButtonXinShouInfo()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local nCurExp		= MainPlayer:getExp();
	local nLevelNextExp = MainPlayer:getLvExp( MainPlayer:getLv() );
	local t_CharacterButtonControl = GetCharacterButtonControl();	
	if not ( MainPlayer:getLv() >= LEVEUP_LIMIT_LV and MainPlayer:getLv() <= LEVEUP_LIMIT_LV + 1 and nCurExp >= nLevelNextExp ) then
		HideCharacterButtonLevelButton();
		EquipFrameLvXinShouBtn:Hide();
		return;
	end
	
	if GameWizard:isDo(GAMEWIZARD_FIRST_SELF_LEVEL_UP) and GameWizard:isDo(GAMEWIZARD_SECOND_SELF_LEVEL_UP) then
		HideCharacterButtonLevelButton();
		EquipFrameLvXinShouBtn:Hide();
		return;
	end
	if EquipFrame:IsShown() then
		ShowEquipGuide();
	end
	
	if  nCurExp >= t_CharacterButtonControl.nextShowExp then
		t_CharacterButtonControl:startShowLevelUpAnim();
		ShowCharacterButtonLevelButton();
		t_CharacterButtonControl.nextShowExp = t_CharacterButtonControl.nextShowExp + nLevelNextExp
	end
end

local t_CharacterButtonControl = { startTime = 0, isShowTips = false, MAX_SHOW_TIPS_TIME = 5, nextShowExp = 0 };

function GetCharacterButtonControl()
	return t_CharacterButtonControl;
end

t_CharacterButtonControl["isTipsBeingShown"] = 
function ( self )
	return self["isShowTips"];
end

t_CharacterButtonControl["startShowLevelUpAnim"] = 
function ( self )
	self["startTime"]	= os.clock();
	self["isShowTips"]	= true;
end

t_CharacterButtonControl["endShowLevelUpAnim"] = 
function ( self )
	self["startTime"]	= 0;
	self["isShowTips"]	= false;
end

t_CharacterButtonControl["update"] = 
function ( self, data )
	if not self["isShowTips"] then
		return;
	end

	local fCurTime = os.clock();
	if fCurTime - self["startTime"] < self["MAX_SHOW_TIPS_TIME"] then
		return;
	end
	
	self:endShowLevelUpAnim();

	local t_hideIcons = { "ShortCutButtonsFrameXinShouBtn", "SkillButtonXinShouBtn", };
	util.HideIcons( t_hideIcons )
end

function CharacterButtonOnUpdate()
	t_CharacterButtonControl:update();
end

function HideCharacterButtonLevelButton()
	t_hideIcons = { "CharacterButtonUVAnimationTex" };
	util.HideIcons( t_hideIcons )
end

function ShortCutButtonsFrameXinShouBtnCloseBtn_OnClick()
	util.closeParent(this);
end

function ShowCharacterButtonLevelButton()
	local szCharacterBtnName	=  "CharacterButton";
	local characterBtn			= getglobal( szCharacterBtnName );
	local UVATexture			= getglobal( szCharacterBtnName.."UVAnimationTex" );
	UVATexture:SetUVAnimation( 50, true );
	UVATexture:Show();

	local szText		= "点击打开人物界面，进行手动升级\n#P快捷键（C）";
	local xinShouBtn	= getglobal( characterBtn:GetParent().."XinShouBtn" );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	rich:SetClientString( szText );
	local nSelfDefineExtendWidth = 14;
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:SetPoint( "BOTTOMLEFT","CharacterButton","TOPLEFT",10,-17 )
	xinShouBtn:Show();
end

function CharacterButtonOnClick() 
	GuideEffectHide( "CharacterButton" );
	if EquipFrame:IsShown() then
		EquipFrame:Hide();
	else
		EquipFrame:Show();		
	end	
end

function SparButtonOnClick()
	GuideEffectHide( "SparButton" );
	t_hideIcons = { "SparButtonUVAnimationTex" };
	util.HideIcons( t_hideIcons )
	if SparFrame:IsShown() then
		SparFrame:Hide();
	else
		SparFrame:Show();		
	end
end

--技能
function SkillButtonOnClick()
	-- 关闭技能提示
	GuideEffectHide( "SkillButton" );
	t_hideIcons = { "SkillButtonUVAnimationTex" };
	util.HideIcons( t_hideIcons )

	local Frame= getglobal( "SkillFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

--宠物
function PetButtonOnClick()
	GuideEffectHide( "PetButton" );
	-- 判断主角身上是否有宠物，若没有宠物，则不能打开面板
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();

	local Frame= getglobal( "PetFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		if petPackage:getPetNum() == 0 then
			ShowMidTips( "你还没有宠物，不能打开宠物面板" );
			return;
		end
		Frame:Show();
	end
end

function XianButtonOnClick()
	GuideEffectHide( "XianButton" );
	XianButton:SetGray(false);
	local Frame= getglobal( "TrumpFrame" );
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then 
		return;
	end
	
	local bFound = false;
	local GameTrump	= MainPlayer:getTrump();
	local nTrumpNum = GameTrump:getTrumpMaxNum();
	for i = 1, nTrumpNum do
		local RoleTrump = GameTrump:getRoleFairy( i - 1 );
		if RoleTrump.FairyID > 0 then
			bFound = true;
		end
	end
	
	if not bFound then	
		ShowMidTips("你还未拥有精灵！");
		return;
	end
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

--背包
function ContainerButtonOnClick() 
	GuideEffectHide( "ContainerButton" );
	ContainerButton:SetGray(false);

	local Frame= getglobal( "PlayerContainerFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

--任务
function TaskButtonOnClick()   
	GuideEffectHide( "TaskButton" );
	TaskButton:SetGray(false);
	local Frame= getglobal( "TaskLogFrame" );
	if (Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

--公会
function ChanButtonOnClick()
	-- 若主角没加入公会，则打开公会查询面板
	if not IsInClan() then
	--	ShowMidTips( "你当前未加入公会，不能打开公会面板" );
	--	return;
		if not ClanListFrame:IsShown() then
			ClanListFrame:SetPoint( "center", "$parent", "center", 0, 0 );
			ClanListFrame:Show();
		else
			ClanListFrame:Hide()
		end
	else
		ClanButton:SetGray(false);
		if ClanFrame:IsShown() then
			ClanFrame:Hide()
		else
			GuideEffectHide( "ClanButton" );
			ClanFrame:SetPoint( "center", "$parent", "center", 0, 0 );
			ClanFrame:Show();
		end
	end
end

--系统
function SystemButtonOnClick()
	if SystemSelectFrame:IsShown() then
		local frame = getglobal( "SystemSelectFrame" );
		frame:Hide();
	else
		SetSystemSelectFrameActiveInfo();
		SystemSelectFrame:Show();
	end
end

--功能按键 10个，鼠标移入的Tips信息
function CharacterButtonOnEnter()
	local key = GetKeyMap("打开人物界面")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("角色 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "角色");
	end
end

--技能
function SkillButtonOnEnter()
	local key = GetKeyMap("打开技能界面")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("技能 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "技能");
	end
end

--宠物
function PetButtonOnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		SetGameTooltips(this:GetName(), "获得宠物后激活\n宠物可协助人物并肩作战");
		return
	end
	local key = GetKeyMap("打开宠物界面")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("宠物 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "宠物");
	end
end

function XianButtonOnEnter()
	local key = 0 --GetKeyMap("打开精灵面板")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("精灵 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "精灵");
	end
end

--任务
function TaskButtonOnEnter()
	local key = GetKeyMap("打开任务日志")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("任务 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "任务");
	end
end
-- 能源
function SparButtonOnEnter()
	local key = GetKeyMap("打开能源面板")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("能源 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "能源");
	end
end


--物品
--[[
function ItemButtonOnEnter()
	SetGameTooltips(this:GetName(), "背包 ( 快捷键 X )" );
end
--]]

local t_ContainerLockTips = {
					[CONTAINER_LOCK_STATE_NO]  = [[

#Y您目前还未设置安全锁！

#R为了您的财产安全，请设置安全锁密码。

#G设置安全锁：
    1、点击背包内安全锁按键设置安全锁。

]],
[CONTAINER_LOCK_STATE_LOCK] = [[

#Y安全锁解锁锁定状态！

#R请解锁安全锁。

#G安全锁解锁：
    1、请点击背包内安全锁按键进行解锁。

]],
					[CONTAINER_LOCK_STATE_OPEN] = [[

#Y安全锁解锁状态！

#R为了保障您财产安全，离开时请锁上安全锁。

#G安全锁上锁：
    1、点击背包内安全锁按键锁上安全锁。
	2、下线后自动锁上安全锁。

]]}

-- 背包
function ContainerButtonOnEnter()
	local key = GetKeyMap("打开背包")
	if key == 0 then
		key = nil
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	local szTips = "";
	if key then
		szTips = string.format("背包 ( 快捷键 %s )", KeyToString(key));
	else
		szTips = "背包";
	end
	szTips = szTips .. t_ContainerLockTips[lockState];
	SetGameTooltips(this:GetName(), szTips );
end

--好友
function FriendsButtonOnEnter()
	local key = GetKeyMap("打开好友面板")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("好友 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "好友");
	end
end

-- 更新好友系统的好友请求信息，添加：更新结婚相关请求
function FriendsButton_OnUpdate()
	if not CanBeginShortcutAction() then
		return;
	end

	--更新好友的
	UpdateAddFriendList();
	--更新结婚的
	UpdateMarryInfoList();
end

function FriendsButton_OnLoad()
	this:setUpdateTime( 0 );
end

--队伍
function TeamButtonOnEnter()
	local key = GetKeyMap("打开队伍面板")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("队伍 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "队伍");
	end
end

-- 天赋
function TianFuButtonOnClick()
	if (TianFuFrame:IsShown()) then
		TianFuFrame:Hide();
	else
		TianFuFrame:Show();
		TianFuShortcutButtonUVAnimationTex:Hide();
	end
end

function TianFuButtonOnEnter()
	local key = GetKeyMap("打开天赋面板")
	if key == 0 then
		key = nil
	end
	if key then
		SetGameTooltips(this:GetName(), string.format("天赋 ( 快捷键 %s )", KeyToString(key)));
	else
		SetGameTooltips(this:GetName(), "天赋");
	end
end

-- ={ name = ， startTime = , bInvite = }
local t_teamBlink = {};

function GetTeamBlinkPlayers()
	return t_teamBlink;
end

function initTeamBUtton()
	t_teamBlink = {};
end

function AddBlinkTeamName( szName, bInvite )
	table.insert( t_teamBlink,  { name = szName, startTime = GameMgr:getTickTime(), bInvite = bInvite, bHighlight = false,
									lastHighlightTime = 0 } );
	
	if not CanBeginShortcutAction() then
		return;
	end

	if table.getn( t_teamBlink ) == 1 then
		-- 显示第一个闪光
		GuideEffectShow( "", "", "TeamButton", "玩家"..szName..( bInvite and "邀请你\n加入队伍" or "申请\n加入队伍" ), "uires\\ZhuJieMian\\1.dds", 
							352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end
end

function CheckTeamReqestsAfterAnim()
	if not CanBeginShortcutAction() then
		return;
	end

	local nCurTime = GameMgr:getTickTime();
	for index, data in ipairs( t_teamBlink ) do
		data["startTime"] = nCurTime;
	end
	
	if table.getn( t_teamBlink ) > 0 then
		-- 显示第一个闪光
		GuideEffectShow( "", "", "TeamButton", "玩家"..t_teamBlink[1]["name"]..( t_teamBlink[1]["bInvite"] and "邀请你\n加入队伍" or 
							"申请\n加入队伍" ), "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
	end	
end

-- 此为手动删除
function CancelBlinkPlayer( szName )
	for index, data in ipairs( t_teamBlink ) do
		if data["name"] == szName then
			CancelBlinkPlayerByIndex( index )
			break;
		end
	end
end

function CancelAllBlinkApply()
	-- 保存所有邀请信息
	local tempBuffer = {};
	for index, data in ipairs( t_teamBlink ) do
		if data["bInvite"] then
			table.insert( tempBuffer, data );
		end
	end
	
	t_teamBlink = tempBuffer;
	if table.getn( t_teamBlink ) == 0 then
		GuideEffectHide( "TeamButton" );
		TeamButton:SetGray( false );	
	else
		t_teamBlink[1]["startTime"] = GameMgr:getTickTime();
		GuideEffectShow( "", "", "TeamButton", "玩家"..t_teamBlink[1]["name"]..( t_teamBlink[1]["bInvite"] and "邀请你\n加入队伍" or 
							"申请\n加入队伍" ), "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );	
	end
end

function CancelBlinkPlayerByIndex( nIndex )
	table.remove( t_teamBlink, nIndex );
	GuideEffectHide( "TeamButton" );
	if table.getn( t_teamBlink ) == 0 then
		TeamButton:SetGray( false );	
		return;
	end
	
	if nIndex == 1 then
		t_teamBlink[1]["startTime"] = GameMgr:getTickTime();
		GuideEffectShow( "", "", "TeamButton", "玩家"..t_teamBlink[1]["name"]..( t_teamBlink[1]["bInvite"] and "邀请你\n加入队伍" or "申请\n加入队伍" ), "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );		
	end
end

--队伍
function TeamButtonOnClick()
	local teambtn = getglobal("TeamButton");

	local t_teamFrames = { "TeamMemberFrame", "TeamApplyListFrame", "TeamAroundPlayerFrame", "TeamQiYueFrame", };
	local curShowFrame;
	for _, name in ipairs( t_teamFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			curShowFrame = frame;
			break;
		end
	end
	
	-- 有面板显示：1.若没有任何邀请或者申请信息。则直接关闭开的面板；2.若显示的面板不是申请面板且第一个闪光的不是申请人
	if curShowFrame ~= nil and ( table.getn( t_teamBlink ) == 0 or 
								not ( not t_teamBlink[1]["bInvite"] and curShowFrame:GetName() == "TeamApplyListFrame" ) ) then
		curShowFrame:Hide();
		-- 若当前闪烁的是申请人，关闭后要打开申请列表
		if not ( table.getn( t_teamBlink ) > 0 and not t_teamBlink[1]["bInvite"] ) then
			return;
		end
	end
	
	local teamInfo = TeamManager:getTeamInfo();
	-- 若有邀请或申请，优先显示邀请或申请，其次若组队则优先显示队员面板，最后显示周围面板
	local frame = getglobal( ( table.getn( t_teamBlink ) > 0 ) and ( t_teamBlink[1]["bInvite"] and "TroopInviteFrame" or "TeamApplyListFrame" ) or
							( ( teamInfo.MemberNum > 0 ) and "TeamMemberFrame" or "TeamAroundPlayerFrame" ) );
	if not frame:IsShown() then
		frame:SetPoint( "center", "$parent", "center", 0, 0 );
		frame:Show();
	end
	
	-- 使所有原来申请的人不闪动，若第一个是邀请，则不闪动邀请的人
	-- 若显示申请面板
	if table.getn( t_teamBlink ) == 0 then
		return;
	end
	
	if t_teamBlink[1]["bInvite"] then
		local preData = t_teamBlink[1];
		CancelBlinkPlayerByIndex( 1 );
		SetCurInvitePlayer( { name = preData["name"], } );
		local t_fonts = { ["InvitePlayerNameFont"] = { value = preData["name"].."  邀请您加入他的队伍" }, };
		util.SetFonts( t_fonts );
	else
		if TeamApplyListFrame:IsShown() then
			UpdateApplyList();
		else
			TeamApplyListFrame:Show();
		end

		CancelAllBlinkApply();
	end
end

function TeamInviteShow( szName )
	if ShowChangeRoleFrame:IsShown() then
		TeamManager:Team_OP_InviteRefuse( szName );
		return;
	end
	AddBlinkTeamName( szName, true );
end

function TeamButton_OnLoad()
	this:RegisterEvent("GE_TEAM_GET_APPLY");
end

function TeamButton_OnEvent()
	if( arg1 == "GE_TEAM_GET_APPLY" ) then
		if ShowChangeRoleFrame:IsShown() then
			TeamManager:Team_OP_ApplyRefuseAll();
			return;
		end

		local nSize = TeamManager:getTeamApplyInfoSize();
		local checkBtn		= getglobal("TeamApplyListFrameAutoAccept")
		local applyinfo		= TeamManager:getTeamApplyInfoByIndex( nSize - 1 );
		local teamChkBtn	= getglobal("AutoAssistFrame_ChkBtn8");
		if  teamChkBtn:GetCheckState()  and  AutoAssist:getAutoAssistBegin() then
			bAutoAccept	= true;
			--AutoAcceptApply();
		else
			bAutoAccept = checkBtn:GetCheckState();
		end
		if bAutoAccept ~= true then
			AddBlinkTeamName( applyinfo.RoleName, false );
		end
		AutoAcceptApply()
	end
end

local MAX_BLINK_TIME = 15000;
--当有其他玩家邀请加入队伍时，图标间隔播放高亮
function  TeamButtonOnUpdate()
	if table.getn( t_teamBlink ) == 0 or not CanBeginShortcutAction() then
		return;
	end

	local nCurTime = GameMgr:getTickTime();
	-- 控制闪烁
	if t_teamBlink[1]["bHighlight"] then
		TeamButton:SetGray( false );
		t_teamBlink[1]["lastHighlightTime"] = nCurTime;
		t_teamBlink[1]["bHighlight"] = false;
	end

	if nCurTime - t_teamBlink[1]["lastHighlightTime"] > 1000 then
		TeamButton:SetGray( true );
		t_teamBlink[1]["bHighlight"] = true;
	end

	if nCurTime - t_teamBlink[1]["startTime"] <= MAX_BLINK_TIME then
		return;
	end

	if t_teamBlink[1]["bInvite"] then
		TeamManager:Team_OP_InviteRefuse( t_teamBlink[1]["name"], true );
	end
	
	CancelBlinkPlayerByIndex( 1 );
end


local t_ClanLockTips = {
					[CONTAINER_LOCK_STATE_NO]  = [[

#Y您目前还未设置安全锁！

#R为了您的财产安全，请设置安全锁密码。

#G设置安全锁：
    1、点击公会内安全锁按键设置安全锁。

]],
[CONTAINER_LOCK_STATE_LOCK] = [[

#Y安全锁解锁锁定状态！

#R请解锁安全锁。

#G安全锁解锁：
    1、请点击公会内安全锁按键进行解锁。

]],
					[CONTAINER_LOCK_STATE_OPEN] = [[

#Y安全锁解锁状态！

#R为了保障您财产安全，离开时请锁上安全锁。

#G安全锁上锁：
    1、点击公会内安全锁按键锁上安全锁。
	2、下线后自动锁上安全锁。

]]}

--公会
function ChanButtonOnEnter()
	local key = GetKeyMap("打开公会界面")
	if key == 0 then
		key = nil
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	local szTips = "";
	if key then
		szTips = string.format("公会 ( 快捷键 %s )", KeyToString(key));
	else
		szTips = "公会";
	end
	szTips = szTips .. t_ClanLockTips[lockState];
	--SetGameTooltips(this:GetName(), szTips );
	ShowGameTooltip( { rich = "WorldMapGameTooltipRichText", tipsFrame = "WorldMapGameTooltip", text = szTips, frame = this:GetParent(),
						button = this:GetName() } );
end

function ChanButtonOnEnter_OnLeave()
	WorldMapGameTooltip:Hide();
end

--系统
function SystemButtonOnEnter()
	SetGameTooltips(this:GetName(), "系统 ( 快捷键 Esc )" );
end
--鼠标移开
function GameTooltipOnLeave()
	GameTooltipRichText:SetText("",255,255,255);
	GameTooltip:Hide();
end


--为连续攻击显示技能锁而设定的函数
function DoSkill(skillid)
	for i = 1 , UIGV.SS_SkillCount do
		if(skillid == UIGV.SS_SkillInfo[i].skillid) then
			local button = getglobal("MagicShortcut"..i);
			button:AddLock();
			GE:OutputScriptWindow("Cast Skill: ".. skillid);
			GE:CastSkill( skillid );
			break;
		end
	end
end

function DoShutcutSkill(buttonName)
	local button = getglobal(buttonName);
	button:AddLock();
	local index = FindShutCutBtnIndex(buttonName);
	if(index == 0) then
		GE:OutputScriptWindow("Return because index==0");
		return;
	end;

	local skillid = UIGV.SS_SkillInfo[index].skillid;
	GE:OutputScriptWindow("Cast Skill: ".. skillid);
	GE:CastSkill( skillid );
end;

function ShowSkillToolTip(button)
	local btnName = button:GetName();
	local index = FindShutCutBtnIndex(btnName);
	if(index == 0 or index > UIGV.SS_SkillCount)  then
		return;
	end;

	local text = UIGV.SS_SkillInfo[index].name.."(等级"..UIGV.SS_SkillInfo[index].skilllevel..")";
	--SetGameTooltips(button:GetName(),text );
	--SetGameTooltipsColor(UIGV.ItemTipColor.r,UIGV.ItemTipColor.g,UIGV.ItemTipColor.b );
end

function ShortcutOnLoad()
	local FontText = getglobal(this:GetName().."Text");
	FontText:SetText("");
	if( this:GetClientID() <= 10 or this:GetClientID() >=13 ) then
		local MagicShortcut = getglobal("MagicShortcut"..this:GetClientID().."Icon");
		if MagicShortcut ~= nil then
			MagicShortcut:SetBlendAlpha( 0 );
		end
	end
	this:setUpdateTime( 1 )
	this:RegisterEvent("GE_ITEMUSED_PASSCD");
	this:RegisterEvent("GE_SKILL_LIST_UPDATE");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_MAIN_BUF_LIST_UPDATE");
	this:RegisterEvent("GE_MAIN_RIDE_CHG");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	this:RegisterEvent("GE_ITEM_LOCK_CHG");
end

function CanUseShort( id )
	local shortCut = GameShortCut:getUseShortCut( id );
	if shortCut.m_nType == SCT_RIDE then
		if not CanRide() then -- or not CheckIntonateBar( "召唤坐骑" ) then
			return false;
		end
	end
	return true;
end

function ShortcutOnMouse(press)
	if arg1 ~= "RightButton" then
		return
	end
	if not isPointInFrame(this:GetName()) then
		return
	end

	if press then--按下鼠标右键
		local MainPlayer = ActorMgr:getMainPlayer();
		GuideTipsEffectHide(this:GetName());
		nCurMouseSHortcutIndex = this:GetClientID() - 1;
		GameShortCut:setShortCutPress(true)

		if this:GetClientUserData(3) == SHORCUT_XP then
			if MainPlayer:checkXP( this:GetClientUserData(1) ) == 0 then
				local rider	= MainPlayer:getRider();
				if rider:isOnHorse() and not rider:isOwnRide() then
					return;
				end
				GameShortCut:usedShortCut( nCurMouseSHortcutIndex );
			end
			return;
		end

		if IsItemLock( 0, 0, this:GetClientUserData(1) ) then
			ShowMidTips("当前物品被锁定不可使用");
			return;
		end

		local shortCut		= GameShortCut:getUseShortCut( nCurMouseSHortcutIndex );
		local nTargetIndex	= 0;
		if shortCut.m_nType == SCT_ITEM then
			local itemDef = getItemDef( shortCut.m_Id );
			local container = MainPlayer:getContainer();
			local used_Item = container:getItem( itemDef.ItemID  );
			if not CanUseItem( itemDef, true, used_Item ) then
				return;
			end
			if container:getItemCount( itemDef.ItemID ) > 0 then
				
				if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_EXP_BUCHANG ) then
					OpenReturnGiftExp( used_Item )
					return
				end
				if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_BUCHANG ) then
					OpenReturnArmSelect( used_Item, MainPlayer:getLv() )
					return
				end
				if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_SELECT ) then
					for i = 1, MAX_ITEM_RESULT do
						local resultData = itemDef.Result[i-1];
						if resultData.ResultID == RESULT_ITEM_ARM_SELECT then
							OpenReturnArmSelect( used_Item, resultData.ResultVal1*10 )
						end
					end
					return
				end
				if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_GEM_PAG ) then
					OpenGemSelectPag( used_Item )
					return
				end
				
				local t_GameCBTFrameControl = GetGameCBTFrameControl();
				local gameCBT				= MainPlayer:getCBTInfo();
				if gameCBT:isCangBaoTuItem( used_Item:getItemId() ) then
					if t_GameCBTFrameControl:canStartCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } ) then
						t_GameCBTFrameControl:startCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
					end
					return;
				end

				local t_changeCareerControl = GetChangeCareerControl();
				if t_changeCareerControl:isChangeCareerItem( { id = used_Item:getItemId() } ) then
					if t_changeCareerControl:canStartChangeCareer( { id = used_Item:getItemId() } ) then
						t_changeCareerControl:startChangeCareer( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
					end
					return;
				end
				-- 熔岩骇心使用
				if itemDef.ItemID == CLT_RONG_YAN_ITEM_ID or itemDef.ItemID == ZJ_RONGYAN_ITEM_ID_TOLUA then 
					UseRongYanSelectItem( itemDef.ItemID );
					return;
				end
				if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_TASK_RECYCLE ) then
					if MainPlayer:isDead() then
						ShowMidTips("你已经死亡");
					else
						Quest:RequestRecycleTaskByItem( used_Item:getListType(), used_Item:getGridIdx() );
					end
					return
				end
			end
			
			if MainPlayer:isInFly() then
				ShowMidTips("处于飞行状态，不能使用物品");
				return;
			end	
			
			--[[
			if not CanSatisfyDesignation( itemDef )  then
				ShowMidTips("声望未达到要求");
				return;
			end
			--]]

			if itemDef.UseTarget == USE_TARGET_PET then
				nTargetIndex = getFightPetIndex();
			end
		end

		if shortCut.m_nType == SCT_RIDE then
			if not CanRide() then
				return;
			end
		end

		GameShortCut:usedShortCut( nCurMouseSHortcutIndex, nTargetIndex );
	else--松开鼠标右键
		if nCurMouseSHortcutIndex then
			GameShortCut:setShortCutPress(false)
		end
		nCurMouseSHortcutIndex = nil
	end
end

function ShortcutOnClick()
	-- 隐藏指引提示
	if arg1 == "RightButton" then
		return
	end
	if this:GetName() == "XPBufShortcut1" then
		GuideEffectHide( "XPBufShortcut1" );
	end
	GuideTipsEffectHide(this:GetName());
	local id = this:GetClientID() - 1;
	local MainPlayer = ActorMgr:getMainPlayer();
	if this:GetClientUserData(3) == SHORCUT_XP then
		if MainPlayer:getPkTargetName() ~= "" then
			return;
		end
	end
	if this:GetClientUserData(3) == SHORCUT_XP then
		if MainPlayer:checkXP( this:GetClientUserData(1) ) == 0 then
			local rider	= MainPlayer:getRider();
			if rider:isOnHorse() and not rider:isOwnRide() then
				return;
			end
			GameShortCut:usedShortCut(this:GetClientID()-1 );
		end
		return;
	end	

	if IsItemLock( 0, 0, this:GetClientUserData(1) ) then
		ShowMidTips("当前物品被锁定不可使用");
		return;
	end
		
	local shortCut		= GameShortCut:getUseShortCut( id );
	local nTargetIndex	= 0;
	if shortCut.m_nType == SCT_ITEM then
		local itemDef = getItemDef( shortCut.m_Id );
		local container = MainPlayer:getContainer();
		local used_Item = container:getItem( itemDef.ItemID );
		if not CanUseItem( itemDef, true, used_Item ) then
			return;
		end
		if container:getItemCount( itemDef.ItemID ) > 0 then
				
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_EXP_BUCHANG ) then
				OpenReturnGiftExp( used_Item )
				return
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_BUCHANG ) then
				OpenReturnArmSelect( used_Item, MainPlayer:getLv() )
				return
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_SELECT ) then
				for i = 1, MAX_ITEM_RESULT do
					local resultData = itemDef.Result[i-1];
					if resultData.ResultID == RESULT_ITEM_ARM_SELECT then
						OpenReturnArmSelect( used_Item, resultData.ResultVal1*10 )
					end
				end
				return
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_GEM_PAG ) then
				OpenGemSelectPag( used_Item )
				return
			end
			
			local t_GameCBTFrameControl = GetGameCBTFrameControl();
			local gameCBT				= MainPlayer:getCBTInfo();
			if gameCBT:isCangBaoTuItem( used_Item:getItemId() ) then
				
				if t_GameCBTFrameControl:canStartCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } ) then
					t_GameCBTFrameControl:startCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
				end
				return;
			end

			local t_changeCareerControl = GetChangeCareerControl();
			if t_changeCareerControl:isChangeCareerItem( { id = used_Item:getItemId() } ) then
				if t_changeCareerControl:canStartChangeCareer( { id = used_Item:getItemId() } ) then
					t_changeCareerControl:startChangeCareer( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
				end
				return;
			end
		end

		if MainPlayer:isInFly() then
			ShowMidTips("处于飞行状态，不能使用物品");
			return;
		end
		
		--[[
		if not CanSatisfyDesignation( itemDef )  then
			ShowMidTips("声望未达到要求");
			return;
		end
		--]]

		if itemDef.UseTarget == USE_TARGET_PET then
			nTargetIndex = getFightPetIndex();
		end
	end

	if shortCut.m_nType == SCT_RIDE then
		if not CanRide() then
			return;
		end
	end

	GameShortCut:usedShortCut( id, nTargetIndex );
end


--TIPS使用
function ShortcutOnEnter()
	local Type			= this:GetClientUserData(0);
	local nId			= this:GetClientUserData(1);
	local nRideEndTime	= this:GetClientUserData(2);
	local nLv			= this:GetClientUserData(3);
	local szText		= this:GetName();

	local data		= t_XpBtnInfo[this:GetClientID()]; 
	if data ~= nil and data.bmove then
		data.btn:SetPoint( "topleft", "XPFrame", "topleft", data.onenterposx, data.onenterposy );
		data.btn:SetSize( data.maxsize,data.maxsize );
		data.uvatex:SetSize( data.maxuvasize,data.maxuvasize );
		data.boxtex:SetSize( data.maxsize,data.maxsize );
		data.icontex:SetSize( data.maxiconsize,data.maxiconsize );
	end
	
	if Type == SHORTCUT_SKILL then
		SpellTipsFrame:SetClientUserData( 0, SkillMgr:getRuneSkillId(nId) );
		SpellTipsFrame:SetClientUserData( 2, SKILL_TYPE );
		SpellTipsFrame:SetClientString(szText);		
		SpellTipsFrame:Show();
	elseif Type == SHORTCUT_FAIRYSKILL then
		SpellTipsFrame:SetClientUserData( 0, SkillMgr:getRuneSkillId(nId) );
		SpellTipsFrame:SetClientUserData( 1, GetTrumpSkillLv( nId ) )
		SpellTipsFrame:SetClientUserData( 2, SKILL_TYPE );
		SpellTipsFrame:SetClientUserData( 3,DIATHESIS_TYPE );
		SpellTipsFrame:SetClientString(szText);
		SpellTipsFrame:Show();
	elseif Type == SHORTCUT_FAIRYDIATHESIS then
		SpellTipsFrame:SetClientUserData( 0, SkillMgr:getRuneSkillId(nId) );
		SpellTipsFrame:SetClientUserData( 1, GetTrumpSkillLv( nId ) )
		SpellTipsFrame:SetClientUserData( 2, DIATHESIS_TYPE );
		SpellTipsFrame:SetClientUserData( 3,TRUMP_SKILL_TYPE );
		SpellTipsFrame:SetClientString(szText);
		SpellTipsFrame:Show();
	elseif Type == SHORTCUT_RIDE then
		local mainplayer = ActorMgr:getMainPlayer()
		local rider		 = mainplayer:getRider();
		for i = 1,rider:getMaxRideNum() do
			local RideInfo = rider:getRideGridInfo( i - 1 );
			if RideInfo.RideID == nId then
				ShowRideTips( { id = nId, lv = RideInfo.nLevel, relFrame = "ShortCutFrame", button = this:GetName(), rideInfo = RideInfo } );
				break;
			end
		end
		
	elseif Type == SHORTCUT_ITEM then
		local itemDef = getItemDef( nId );
		if  itemDef == nil then
			return;
		end

		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer ~= nil then
			local container = MainPlayer:getContainer();
			local nMaxNum = container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES );
			for i = 1, nMaxNum do
				local ContainerItem = container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
				if itemDef.ItemID == ContainerItem:getItemId() then
					ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, ContainerItem );
					return;
				end
			end
		end
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
	end
end

function Shorcut_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
	SpellTipsFrame:Hide();
	RideTipsFrame:Hide();
	this:DisChecked();
	if nCurMouseSHortcutIndex then
		GameShortCut:setShortCutPress(false)
	end
	nCurMouseSHortcutIndex = nil

	local data		= t_XpBtnInfo[this:GetClientID()]; 
	if data ~= nil and data.bmove then
		data.btn:SetPoint( "topleft", "XPFrame", "topleft", data.onleaveposx, data.onleaveposy );
		data.btn:SetSize( data.minsize,data.minsize );
		data.boxtex:SetSize( data.minsize,data.minsize );
		data.uvatex:SetSize( data.minuvasize,data.minuvasize );
		data.icontex:SetSize( data.miniconsize,data.miniconsize );
	end
end

function Shorcut_Drag()
	-- 隐藏指引提示
	GuideTipsEffectHide(this:GetName());

	if IsItemLock( 0, 0, this:GetClientUserData(1) ) then
		ShowMidTips("当前物品被锁定不可移动");
		return;
	end

	local Id = this:GetClientID();
	if this:GetClientUserData( 3 ) == SHORCUT_XP then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer:checkXP( this:GetClientUserData(1) ) == 0 then
			GameShortCut:usedShortCut(this:GetClientID()-1 );
		end
		return;
	end
	
	if not GameShortCut:canDrag( Id ) then
		return;
	end

	local systemSetting		= GameMgr:getUserSystemConfig();
	local t_ShortCutControl = GetShortCutControl();
	if t_ShortCutControl:isShorCutID( { ["ID"] = Id } ) and systemSetting:getGameOpt( GAME_OPT_LOCK_SHORTCUT ) then
		ShowMidTips( "快捷栏已锁定" );
		return;
	end

	UIMSG:getUIGV().DragFrom = DRAG_SHORTCUT;
	local IconTex = getglobal(this:GetName().."Icon");
	UIBeginDrag(IconTex:GetTexture() , this:GetClientID() , 0 , 0);
end

function ShortcutReceiveDrag()
	-- 隐藏指引提示
	GuideTipsEffectHide(this:GetName());
	local des = this:GetClientID() - 1;
	if not GameShortCut:canReceiveDrag(des + 1) then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	
	local srcBtn = getglobal("MagicShortcut47");
	local srcBtnTex = getglobal("MagicShortcut47Icon");
	srcBtnTex:SetGray(false);
	srcBtn:SetClientUserData( 1, 0 );

	if ( UIMSG:getUIGV().DragFrom == DRAG_SHORTCUT ) then
		local src = UIGetDragUserData1() - 1;

		if src ~= des then
			local desBtn = getglobal("MagicShortcut"..(src + 1));
			local desBtnCount = getglobal("MagicShortcut"..(src + 1).."Count");	 
			local desBtnText = getglobal("MagicShortcut"..(src + 1).."Icon");
			desBtnText:SetGray(false);
			desBtnCount:SetText("");
			desBtn:SetClientUserData( 1, 0 );
			this:SwapCooldownMembers(desBtn);
			GameShortCut:swapShortCut( src, des );
		end
		local IconTex = getglobal(this:GetName().."Icon");
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if ( UIMSG:getUIGV().DragFrom == DRAG_SKILL ) then
		GameShortCut:swapShortCut( srcBtn:GetClientID()-1, des );
		local cd = SkillMgr:getCDTime(UIGetDragUserData1());
		this:SetCooldownTimer(cd:past() / 1000, cd:total() / 1000, true);
		GameShortCut:setShortCut( 1, UIGetDragUserData1(), des, 0 );
		if( GameShortCut:canDrag( srcBtn:GetClientID() )  ) then
			UIMSG:getUIGV().DragFrom = DRAG_SHORTCUT;
			local IconTex = getglobal(srcBtn:GetName().."Icon");
			UIBeginDrag(IconTex:GetTexture() , srcBtn:GetClientID() , 0 , 0);
		else
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
		return;
	end

	if ( UIMSG:getUIGV().DragFrom == CONTAINER_TYPE_SUNDRIES or UIMSG:getUIGV().DragFrom == CONTAINER_TYPE_TASK ) then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end

		local Item	= container:getItem( UIMSG:getUIGV().DragFrom, UIGetDragUserData1() );
		if Item:isExpendable() or Item:getItemId() == CLT_RONG_YAN_ITEM_ID then
			GameShortCut:swapShortCut( srcBtn:GetClientID()-1, des );
			GameShortCut:setShortCut( 2, Item:getItemId(), des, 0 );
			if( GameShortCut:canDrag( srcBtn:GetClientID() )  ) then
				UIMSG:getUIGV().DragFrom = DRAG_SHORTCUT;
				local IconTex = getglobal(srcBtn:GetName().."Icon");
				UIBeginDrag(IconTex:GetTexture() , srcBtn:GetClientID() , 0 , 0);
			else
				UIEndDrag();
				UIMSG:getUIGV().DragFrom = DRAG_NONE;
			end
		end
		
		--UIEndDrag()
		--UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if ( UIMSG:getUIGV().DragFrom == DRAG_RIDE ) then
		GameShortCut:swapShortCut( srcBtn:GetClientID()-1, des );
		GameShortCut:setShortCut( 3, UIGetDragUserData1(), des, UIGetDragUserData2(), UIGetDragUserData3() );
		if( GameShortCut:canDrag( srcBtn:GetClientID() )  ) then
			UIMSG:getUIGV().DragFrom = DRAG_SHORTCUT;
			local IconTex = getglobal(srcBtn:GetName().."Icon");
			UIBeginDrag(IconTex:GetTexture() , srcBtn:GetClientID() , 0 , 0);
		else
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
		return;
	end

	if ( UIMSG:getUIGV().DragFrom == DRAG_TRUMPSKILL ) then
		GameShortCut:swapShortCut( srcBtn:GetClientID()-1, des );
		GameShortCut:setPetFittingShortCut( UIGetDragUserData1(),UIGetDragUserData2(), des, 0 );
		local desBtn = getglobal("Pet_FairyButton"..UIGetDragUserData3());
		this:CopyCooldownMembers(desBtn);
		if( GameShortCut:canDrag( srcBtn:GetClientID() )  ) then
			UIMSG:getUIGV().DragFrom = DRAG_SHORTCUT;
			local IconTex = getglobal(srcBtn:GetName().."Icon");
			UIBeginDrag(IconTex:GetTexture() , srcBtn:GetClientID() , 0 , 0);
		else
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
		return;
	end
end

function ShortcutOnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	if nStartXPEffect > 0 and os.clock() >= nStartXPEffect + 13 then
		if MainPlayer:getXP() >= MainPlayer:getMaxXP() then
			EffectFrameGuide:Show();
			EffectFrameGuideFont:SetClientString( "施放变身技能：\n1.鼠标左键点击该技能\n2.键盘快捷键“Alt+1”" );

			ReSizeTooltip( EffectFrameGuideFont:GetName(), EffectFrameGuide:GetName(), 0 );
			EffectFrameGuide:SetPoint("bottomleft", "XPBufShortcut1", "bottomleft", 7, -63);
			EffectFrame:Show();
			EffectFrame:SetClientString("XPBufShortcut1");
			EffectFrameUVAnimationTex:Hide();
		end
		nStartXPEffect = 0;
	end
end

function ShortcutOnEvent()
	if( arg1 == "GE_ITEMUSED_PASSCD" ) then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end
		if this:GetClientUserData(0) == SHORTCUT_ITEM then
			this:SetCooldownTimer( 0, 0, true );
			local cd		= container:getItemCD( this:GetClientUserData(1) );
			local starttime = container:getItemCDPassTime( this:GetClientUserData(1) );
			this:SetCooldownTimer( starttime, starttime+cd, true );
			if this:GetClientID() >= 43 then	 
				this:SetCooldownTextureRect( 2, 2, 26, 26 );
			else
				this:SetCooldownTextureRect( 3, 3, 38, 38 );
			end
		end
	end

	if arg1 == "GE_SKILL_LIST_UPDATE" or arg1 == "GE_MAIN_BUF_LIST_UPDATE" or arg1 == "UI_ACTOR_ATTRIBUTE_CHG"
		or arg1 == "GE_MAIN_RIDE_CHG" or arg1 == "GE_CONTAINER_UPDATE" or arg1 == "GE_ITEM_LOCK_CHG" then
		local FontText	= getglobal(this:GetName().."CollectText");
		local IconTex	= getglobal(this:GetName().."Icon");

		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end
		local rider	= MainPlayer:getRider();
		if this:GetClientUserData(0) == SHORTCUT_SKILL or this:GetClientUserData(0) == SHORTCUT_FAIRYSKILL then
			local skill_attr	= SkillMgr:getSkillDyAttrib( this:GetClientUserData(1) );
			if skill_attr == nil then
				IconTex:SetGray(true);
				return;
			end
			
			local SpellId		= this:GetClientUserData(1);
			if SpellId ~= 0 then
				local SpellLv		= SkillMgr:getSpellLv(SpellId);
				local CollectUse	= SkillMgr:getSpellCollectNumUse(SpellId, SpellLv );
				local BufStatus		= MainPlayer:getBufStatus();
				this:SetClientUserData( 2, SpellLv );
				if CollectUse > 0 then
					local CollectNum   = SkillMgr:getSpellCollectNumCur(SpellId);
					FontText:SetText( CollectNum );
				else
					local SkillDef = SkillMgr:getSpellStaticAtt(SpellId, SpellLv)
					if SkillDef.AmmID ~= 0 and SkillDef.AmmUse ~= 0 then
						local container = MainPlayer:getContainer();
						FontText:SetText(math.floor(container:getItemCount(SkillDef.AmmID) / SkillDef.AmmUse));
					else
						FontText:SetText( "" );
					end
				end
				if SkillMgr:isXPSkill(SpellId) then
					if MainPlayer:checkXP(SpellId) ~= 0 or MainPlayer:isInStall() 
					or ( rider:isOnHorse() and not rider:isOwnRide() ) then
						IconTex:SetGray(true);
					else
						IconTex:SetGray(false);
					end
				elseif SkillMgr:checkMagic(SpellId) ~= 0
					or not BufStatus:canUsedSpell(SpellId) or (rider:isOnHorse() and not rider:isOwnRide())
					or not MainPlayer:canControl() then  
					IconTex:SetGray(true);
				elseif not SkillMgr:castPlayerLv(SpellId, SpellLv) then
					IconTex:SetGray(true);
				else
					IconTex:SetGray(false);
				end
				if this:GetClientUserData(3) == SHORCUT_XP then
					if MainPlayer:getPkTargetName() ~= "" then
						IconTex:SetGray(true);
					end
				end
			end
		elseif this:GetClientUserData(0) == SHORTCUT_ITEM then
			local mainplayer = ActorMgr:getMainPlayer();
			local container	 = mainplayer:getContainer();
			local countfont	 = getglobal( this:GetName().."Count" );
			if container ~= nil and this:GetClientUserData(1) > 0 then
				local containcount = container:getItemCount( this:GetClientUserData(1) );
				local itemDef = getItemDef( this:GetClientUserData(1) );
				if containcount == 0 then
					countfont:SetText( "" );
					IconTex:SetGray(true);
				else
					if IsItemLock( 0, 0, this:GetClientUserData(1) ) or mainplayer:isInFly() then
						IconTex:SetGray(true);
					else
						IconTex:SetGray(false);
					end
					
					if itemDef.Result[0].ResultID == RESULT_MACHINE_ITEM then
						local item = container:getItem( this:GetClientUserData(1) );
						if item:getItemInstVal1() == 0 then
							IconTex:SetGray(true);
						end
					end

					if containcount == 1 then
						countfont:SetText( "" );
					else
						countfont:SetText( container:getItemCount( this:GetClientUserData(1) ) );
					end
				end
			end
		elseif this:GetClientUserData(0) == SHORTCUT_RIDE then
			local countfont	 = getglobal( this:GetName().."Count" );   
			countfont:SetText( "" );
			FontText:SetText( "" );
			local nRideID = this:GetClientUserData( 1 );
			if isShortcutRideInUse( { rideID = nRideID } ) then
				IconTex:SetGray(false);
			else
				IconTex:SetGray(true);
			end
		end		
	end
end

function SetMagicTextureAnimation(texname, timeslot, loopmode, relframe)
	local tex = getglobal(tostring(texname));
	tex:SetPoint("center", relframe, "center", 0, 0);
	tex:SetUVAnimation(timeslot, loopmode);
end


function File_OnClick()
	if(UIGV.ChatFrameOpen == true) then
		MainChatFrame:Hide();
		ChatFrameMenuButton:Hide();
		UIGV.ChatFrameOpen = false;
	else
		MainChatFrame:Show();
		ChatFrameMenuButton:Show();
		UIGV.ChatFrameOpen = true;
	end
end

function Item_OnClick()
	if(UIGV.bagopen == false) then
		updateContainerFrameAnchors();
		updateTaskContainerFrameAnchors();
		ContainerFrame1:Show();
		ContainerFrame2:Show();
		ContainerFrame1Item15:SetNormalTexture("ui.singlegrid", "blend");
		ContainerFrame1Item16:SetNormalTexture("ui.singlegrid", "blend");
		UIGV.bagopen = true;
	else
		ContainerFrame1:Hide();
		ContainerFrame2:Hide();
		UIGV.bagopen = false;
	end
end

function Skill_OnClick()
	if (GuildNpcFrame:IsShown()) then
		GuildNpcFrame:Hide();
	else
		GuildNpcFrame:Show();
	end

end

function ShowMagicPannel(open)
	if (open == true) then
		for i=1, 9 do
			local magicbutton = getglobal("Magic_0"..i);
			magicbutton:Show();
		end
		Magic_10:Show();
		SkillPannel:Show();
	else
		for i=1, 9 do
			local magicbutton = getglobal("Magic_0"..i);
			magicbutton:Hide();
		end
		Magic_10:Hide();
		SkillPannel:Hide();
	end
end

function SkillPanelCtrl_OnClick()
	if(UIGV.SkillPanelOpen == true) then
		ShowMagicPannel(false);
		UIGV.SkillPanelOpen = false;
	else
		ShowMagicPannel(true);
		UIGV.SkillPanelOpen = true;
	end
end

function Button2OnLoad()
	this:Hide();
end

function Button2OnClick()
	if (this:IsShown()) then
		this:Hide();
	else
		this:Show();
	end
end
function IntonateButton_OnEvent()
	this:SetMinValue(0);
	this:SetMaxValue(arg1);
	this:SetValue(0);
	this:SetStatusBarColor(0, 255, 0);
	if (event == "GC_SKILL_PREPARE_UPDATE")then
		if( arg3 == 0 )then
			this:Hide()
			return
		else
			this:Show();
			this:SetIntonateTimer(arg2, arg3, true)
			return
		end
	end

	if ( arg1 == 0 ) then
		this:Hide()
		return
	else
		this:Show();
	end
	--this:SetNormalRegionWidth(0);

	this:SetIntonateTimer(0, arg1, false);


	--GE:ScriptOutput(this:GetName());
end

--设置Tooltips信息
function SetGameTooltips(name, text)
--	print("SetGameTooltips")
	local ui = getglobal(name)
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = text, frame = ui:GetParent(),
						button = name } )
end

--设置Tooltips信息
function SetGameToolAddTexturetips(name, text, textColor)
	local ui = getglobal(name)
	ShowGameTooltip( { rich = "GameTooltipAddTextureRichText", tipsFrame = "GameTooltipAddTexture", text = text, frame = ui:GetParent(),
						button = name, color = textColor } )
end

function SetGameTooltipsColor(r, g, b)
	GameTooltipText1:SetTextColor(r, g, b);
end

function XP_OnEvent()
	--[[if( event == "GE_LP_MODIFY" ) then
		local Lp,MaxLP= GE:ObserveActorData( arg1,_GCT_.LP,_GCT_.LP_MAX )
		this:SetNormalRegionWidth(Lp/MaxLP);
		if( Lp == MaxLP )then
			XPIcon:Show();
		else
			XPIcon:Hide();
		end
	end--]]
end

function XP_OnClick()
	--IntonateButton:Show();
	--IntonateButton:SetNormalRegionWidth(0);
	--GE:CastSkill( 8 )
end

function ParentOnEnterPressed()
	ChatFrameEditBox:Show();
	SetFocusFrame("ChatFrameEditBox");
	ChatFrameEditBox:SetTextColor(255, 255, 255);
	if(GCHAT_Command == nil) then
		GCHAT_Command = "/s ";
	end;

	ChatFrameEditBox:SetText(GCHAT_Command);

	if(GCHAT_Command == "/s ") then
		ChatFrameEditBox:SetTextColor(255, 255, 255);
	elseif(GCHAT_Command == "/p ") then
		ChatFrameEditBox:SetTextColor(155, 155, 255);
	elseif(GCHAT_Command == "/g ") then
		ChatFrameEditBox:SetTextColor(155, 255, 155);
	elseif(string.find(GCHAT_Command, "/w ", 1, true)) then
		ChatFrameEditBox:SetTextColor(255, 155, 255);
	elseif(string.find(GCHAT_Command, "/1 ", 1, true)) then
		ChatFrameEditBox:SetTextColor(255, 155, 155);
	end


end

function EXP_OnLoad()
	this:RegisterEvent("GE_ACTOR_ATTRIBUTE_CHG");
end

function EXP_OnEvent()
	--if(true) then return end
	if ( arg1 ~= GE:GetLocalPlayerID() ) then return end


	local tpActor = GE:WeakRefCNetActorSlot( arg1 );
	--local tpActor = ActorMgr:WeakRefGameActorSlot( arg1,"CNetActor" )
	if( tpActor == nil )then return end

	if( arg2 == CS_ID_LEVEL )then

		--local tExp,tMaxExp,tHp,tMaxHp,tLevel = tpActor:GetAttribute( _CT_.CS_ID_EXP ,_CT_.CS_ID_EXPUP, _CT_.CS_ID_HP ,_CT_.CS_ID_MAXHP,_CT_.CS_ID_LEVEL )
		local tExp = tpActor:GetAttribute( _CT_.CS_ID_EXP );
		local tMaxExp = tpActor:GetAttribute( _CT_.CS_ID_EXPUP );
		local tHp = tpActor:GetAttribute( _CT_.CS_ID_HP );
		local tMaxHp = tpActor:GetAttribute( _CT_.CS_ID_MAXHP );
		local tLevel = tpActor:GetAttribute( _CT_.CS_ID_LEVEL );
		if( tExp ~= nil )then
			--GE:OutputChatFrame( "tExp:=" .. tostring( tExp ) )
			GE:OutputScriptWindow( "tExp=" .. tostring( tExp ) )
		end
		if( tMaxExp ~= nil )then
			GE:OutputScriptWindow( "tMaxExp=" .. tostring( tMaxExp ) )
		end
		if( tHp ~= nil )then
			GE:OutputScriptWindow( "tHp=" .. tostring( tHp ) )
		end
		if( tMaxHp ~= nil )then
			GE:OutputScriptWindow( "tMaxHp=" .. tostring( tMaxHp ) )
		end
		if( tLevel ~= nil )then
			GE:OutputScriptWindow( "tLevel=" .. tostring( tLevel ) )
		end
	end

	--[[if( arg2 == _CT_.CS_ID_EXP )then
		GE:OutputChatFrame( "Exp+:" .. tostring(arg3) )
	end]]
	if( arg2 == _CT_.CS_ID_LEVEL )then
		--GE:ScriptAssert()
		--GE:OutputChatFrame( "哈哈，你升到 " .. tostring(tLevel) .. " 级了" )
 		GE:OutputChatFrame( "恭喜，你升级了" )
	end

	--local tExp,tMaxExp = tpActor:GetAttribute( _CT_.CS_ID_EXP ,_CT_.CS_ID_EXPUP )
	local tExp = tpActor:GetAttribute( _CT_.CS_ID_EXP );
	local tMaxExp = tpActor:GetAttribute( _CT_.CS_ID_EXPUP );
	if( tExp ~= nil and tMaxExp ~= nil )then
		GE:OutputScriptWindow( tostring(tExp) )
		GE:OutputScriptWindow( tostring(tMaxExp) )
		this:SetNormalRegionWidth( tExp/tMaxExp );
		EXPValue:SetText(tExp.."/"..tMaxExp);
		if(event == "GE_ACTOR_ATTRIBUTE_CHG" and arg3 ~= nil and arg2 == _CT_.CS_ID_EXP and arg3 > 0) then
			GE:OutputChatFrame("你获得了"..arg3.."经验值");
		end
	end
end

-- 吟唱条显示
local LoadingStartTime	= 0;

function IntonateBar_OnLoad()
	this:RegisterEvent("GE_SKILL_POWERCOLLECT_BEGIN");
	this:RegisterEvent("GE_SKILL_POWERCOLLECT_END");
	this:RegisterEvent("GE_SKILL_CHANNEL_BEGIN");
	this:RegisterEvent("GE_SKILL_CHANNEL_END");
	this:RegisterEvent("GE_BREAK_PRE_ACTION");
	this:RegisterEvent("GE_BREAK_RIDE_PRE");
	this:RegisterEvent("GE_PET_PRE_BACK");
	this:RegisterEvent("GE_SECONDARYSKILL_BREAK");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	
	IntonateBar:SetClientUserData( 0, 0 );
	IntonateBar:SetClientUserData( 1, 0 );
	IntonateBar:SetClientUserData( 2, 0 );
	IntonateBar:SetClientUserData( 3, 0 );
end

function BeginUseItem( nPassTime, nListType, nGridIdx )
	IntonateBar_text:SetText("物品使用中");
	IntonateBar:SetClientUserData( 0, ITEM_USE_TYPE );
	IntonateBar:SetClientUserData( 2, nPassTime );
	LockItem(nListType,nGridIdx,0);
	IntonateBar:Show();
end

function IntonateBar_OnEvent()
	if("GE_SKILL_POWERCOLLECT_BEGIN" == arg1 ) then
		local Msg = UIMSG:getUIMsg();
		IntonateBar_text:SetText("技能吟唱中...");
		LoadingStartTime = os.clock() - Msg.PowerCollect.fStartTime;
		IntonateBar:SetClientUserData(0,PREPARE_TYPE);
		IntonateBar:SetClientUserData( 2, math.floor(Msg.PowerCollect.fTotleTime * 1000) );
		IntonateBar:Show();
		if Msg.PowerCollect.times ~= 0 then
			IntonateBarFrame_BarTexture:Hide();
			if Msg.PowerCollect.times == 1 then
				IntonateBarFrame_DyTexture_Before:Hide()
			else
				IntonateBarFrame_DyTexture_Before:Show()
				IntonateBarFrame_DyTexture_Before:SetTexture(string.format("uires\\ui\\DuTiaoJieDuan\\%d.tga", Msg.PowerCollect.times - 1))
			end
			IntonateBarFrame_DyTexture_Current:Show();
			IntonateBarFrame_DyTexture_Current:SetTexture(string.format("uires\\ui\\DuTiaoJieDuan\\%d.tga", Msg.PowerCollect.times));
		end
	elseif("GE_SKILL_POWERCOLLECT_END" == arg1 or "GE_SKILL_CHANNEL_END" == arg1 or
			"GE_BREAK_RIDE_PRE" == arg1 or "GE_SECONDARYSKILL_BREAK" == arg1 ) then
		--print( "IntonateBar_OnEvent--1759" );
		this:Hide();
	elseif "GE_BREAK_PRE_ACTION" == arg1 then
		this:Hide();
	elseif ("GE_SKILL_CHANNEL_BEGIN" == arg1) then
		local Msg = UIMSG:getUIMsg();
		IntonateBar_text:SetText("技能引导中...");
		IntonateBar:SetClientUserData(0,CHANNEL_TYPE);
		IntonateBar:SetClientUserData( 2, math.floor(Msg.PowerCollect.fTotleTime * 1000) );
		IntonateBar:Show();
		--print( "IntonateBar_OnEvent--GE_SKILL_CHANNEL_BEGIN--IntonateBar:Show--1767" );
	elseif ( "GE_PET_PRE_BACK" == arg1 ) then
		local fScale		= 0.1;
		local fTotalTime	= IntonateBar:GetClientUserData( 2 ) / 1000;
		LoadingStartTime	= LoadingStartTime + fScale * fTotalTime;
		if LoadingStartTime >= os.clock() then
			LoadingStartTime = os.clock();
		end
	elseif arg1 == "GE_CONTAINER_UPDATE" then
		local nUseType = this:GetClientUserData( 0 );
		if nUseType == ITEM_USE_TYPE then
			local nListType = this:GetClientUserData( 1 );
			local nGridx	= this:GetClientUserData( 3 );
			local MainPlayer= ActorMgr:getMainPlayer();
			local container	= MainPlayer:getContainer();			
			local item		= container:getItem( nListType, nGridx );
			if item:getItemId() == 0 then
				--container:stopUseItemPreAction();
			end
		end
		--活动引导 牛仔很忙入场券
		local nStartInfo = GamePromotion:getCurrPromitionStartInfo( COWBOY_BUSY_FINALS_ID )
		if nStartInfo.startFlag == PROMOTION_RUNING then
			SetSpecialActivityGuid( COWBOY_BUSY_FINALS_ID );
		end
	end
end

local t_cannotBreakOp =	{	["技能吟唱"]	= { curDesc = "当前处于技能吟唱状态，" }, 
							["技能引导"]	= { curDesc = "当前处于技能引导状态，" },
							["物品使用"]	= { curDesc = "当前处于使用物品状态，" },

							["召唤宠物"]	= { curDesc = "当前处于召唤宠物状态，" },
							["召唤坐骑"]	= { curDesc = "当前处于装备坐骑状态，" },
							["召唤精灵"]	= { curDesc = "当前处于召唤精灵状态，" },
						};
function CheckIntonateBar( szCurOperateDesc )
	if not IntonateBar:IsShown() then
		return true;
	end
	
	local font = getglobal( "IntonateBar_text" );
	for desc, data in pairs( t_cannotBreakOp ) do
		if string.find( font:GetText(), desc ) ~= nil then
			ShowMidTips( data.curDesc.."不能进行"..szCurOperateDesc );
			return false;
		end
	end
end

function IntonateBar_OnUpdate()
	-- 毫秒级更新
	this:setUpdateTime( 0 );

	local times				= os.clock() - LoadingStartTime;
	local nType				= IntonateBar:GetClientUserData( 0 );
	local nIndexId			= IntonateBar:GetClientUserData( 1 );
	local LoadingEndTime	= IntonateBar:GetClientUserData( 2 ) / 1000;
	local nLevel			= IntonateBar:GetClientUserData( 3 );
	
	if nType == 0 and LoadingEndTime == 0 then
		this:Hide()
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ==nil then
		return;
	end 

	if nType == SECONDARYSKILL_RESOLVE_TYPE or nType == SECONDARYSKILL_OTHER_TYPE or nType == COLLECT_TYPE then
		if mainplayer:isInFly() or mainplayer:isInStall() then
			mainplayer:breakChannel()
			--SecondarySkillMgr:requestBreakReciptPre();
			return;
		end
	end
	
	--print( "times = "..times.."--LoadingEndTime = "..LoadingEndTime );
	if times > LoadingEndTime then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer == nil then
			return;
		end
		
		if nType == SECONDARYSKILL_RESOLVE_TYPE then
			SecondarySkillMgr:requestReciptResolve(nIndexId, nLevel);
		end
		if nType ~= PREPARE_TYPE then		
			if IntonateBar:IsShown() then
				IntonateBar:Hide();
			end
		end
		IntonateBar:Hide()
		--print( "IntonateBar_OnUpdate--1858" );
	elseif nType == CHANNEL_TYPE then
		local scale = (times )/LoadingEndTime;
		IntonateBarFrame_BarTexture:SetTexUV( 81+scale*202, 620, (1-scale)*202, 32 );
		IntonateBarFrame_BarTexture:SetSize( (1-scale)*202, 32 );
	else
		--GameExchange:test( "not   CHANNEL_TYPE" );
		local scale = (times )/LoadingEndTime;
		IntonateBarFrame_BarTexture:SetTexUV( 81+(1-scale)*202, 620, scale*202, 32 );
		IntonateBarFrame_BarTexture:SetSize( scale*202, 32 );
		IntonateBarFrame_DyTexture_Current:SetSize(197 * scale, 31);
		IntonateBarFrame_DyTexture_Current:SetPoint("topleft", "IntonateBar", "topleft", 34, 1);
		IntonateBarFrame_DyTexture_Current:SetTexUV(197 * (1-scale), 0, 197 * scale, 31);
	end
end

function IntonateBar_OnShow()
	LoadingStartTime = os.clock();
	IntonateBarFrame_DyTexture_Before:Hide();
	IntonateBarFrame_DyTexture_Current:Hide();
	IntonateBarFrame_BarTexture:Show();
	IntonateBar_OnUpdate();
end

function IntonateBar_OnHide()
	IntonateBar:SetClientUserData( 0, 0 );
	IntonateBar:SetClientUserData( 1, 0 );
	IntonateBar:SetClientUserData( 2, 0 );
	IntonateBar:SetClientUserData( 3, 0 );
end

function skillPreBack( backTime )
	LoadingStartTime = LoadingStartTime + backTime;
	if LoadingStartTime >= os.clock() then
		LoadingStartTime = os.clock();
	end
end

function UpdateXPShortCut()
end

function Shortcut_ThreeFrame_OnUpdate()
	
end

function TmpShortCutEnerge_Test_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "测试能量槽", frame = this:GetParent(),
							button = this:GetName() } );
end

function TmpShortCutEnerge_OnEnter()		
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	if MainPlayer:getEnerge() <= 0 and MainPlayer:getMaxEnerge() <= 0 then
		return;
	end
	
	local eqiup = MainPlayer:getEquip();
	if eqiup:getTmpMachin() ~= 0 and eqiup:getTmpArm() == 0 then
		local szText = "能量: "..MainPlayer:getEnerge().."/"..MainPlayer:getMaxEnerge();
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName() } );
	end
end

function XPBufStatusButton_OnLeave()
	GameTooltip:Hide();
end

function BeginCollect(nTime, id, text)
	local monster = ActorMgr:FindActor(id);
	text = string.gsub(text, "#self#", monster:getName());
	IntonateBar_text:SetText(text);
	IntonateBar:SetClientUserData( 0, COLLECT_TYPE );
	IntonateBar:SetClientUserData( 1, 0 );
	IntonateBar:SetClientUserData( 2, math.floor(nTime * 1000) );
	if nTime == 0 then
		local oldthis = this;
		this = IntonateBar;
		IntonateBar_OnUpdate();
		this = oldthis;
	else
		IntonateBar:Show();	
		--print( "BeginCollect--IntonateBar:Show--1930" );
	end
	
	if SecondarySkillFrame:IsShown() then
		SecondarySkillUpdateSkillDetail();
	end
end

function EndCollect()
	if IntonateBar:IsShown() then
		IntonateBar:Hide();
	end
end

function EndUseItem()
	if IntonateBar:GetClientUserData( 0 ) == ITEM_USE_TYPE and IntonateBar:GetClientUserData( 1 ) > 0 then
		UnLockItem( IntonateBar:GetClientUserData(1),IntonateBar:GetClientUserData(3) );
	end
	IntonateBar:SetClientUserData( 0, 0 );
	IntonateBar:SetClientUserData( 1, 0 );
	IntonateBar:SetClientUserData( 2, 0 );
	IntonateBar:SetClientUserData( 3, 0 );
	IntonateBar_text:SetText("");
	if IntonateBar:IsShown() then
		IntonateBar:Hide();
	end 
	--print( "EndUseItem" );
end

-- 等级封印
function OnGetLevelFeng( lv, endTime,chg )
	if chg == 1 and fengyinLv ~= 0 and fengyinLv < lv then
		SetMessageTips( 10000 + fengyinLv )
	end
	fengyinLv,fengyinEnd = lv, endTime
end

function GetFengYinLv()
	if fengyinLv > 0 then
		local dt = math.floor( (fengyinEnd - getServerTime().sec)/3600 )
		local szTime = ""
		if dt < 1 then
			szTime = math.ceil((fengyinEnd - getServerTime().sec)/60) .. "分钟"
		else
			szTime = dt .. "小时"
		end
		return "当前封印等级为" .. fengyinLv .. "，距离解除封印还有" .. szTime .. "\n（达到封印等级人数越多，解封时间越短）"
	elseif GameMgr:GetMaxLevel() > 0 then
		return "当前等级限制为" .. GameMgr:GetMaxLevel()
	end
	return ""
end

function GetFengYinData()
	return fengyinLv,fengyinEnd
end

--显示经验值Tips
local nCurExp = 0;
local nCurXP  = 0;
local nLevelNextExp = 0;
function RoleExpButton_OnEnter()
	local szText = "经验: ".. BigInt2Str(nCurExp) .."/".. nLevelNextExp .. "（" .. (math.floor(nCurExp * 10000 / nLevelNextExp)) / 100 .. "%）#n\n#cc896fa当前等级获取经验：";

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil then
		-- 被封印了
		if MainPlayer:getLv() == fengyinLv then
			local fengyinDef = getLevelFeng( fengyinLv )
			if fengyinDef == nil then
				szText = szText .. "100%#n\n";
			else
				local levelup = getLevelUP( MainPlayer:getLv() )
				local bfound = false
				if levelup ~= nil then
					local nCount = math.ceil( MainPlayer:getExp()/levelup.LevelNextExp )
					if nCount == 0 then
						nCount = 1
					end
					for i=1,MAX_LEV_FENG_EXPDEC_TOLUA do
						if nCount > 0 and fengyinDef.ExpRateDec[i-1].ExpCount == nCount then
							szText = szText .. fengyinDef.ExpRateDec[i-1].ExpRate .. "%#n\n"
							bfound = true
							break
						end
					end
				end
				if not bfound then
					szText = szText .. "100%#n\n";
				end
			end
		else
			local topLv = GetTopOneLv();
			if MainPlayer:getLv() > 40 and MainPlayer:getLv() ~= 59 and MainPlayer:getLv() < topLv then --特殊处理59级
				local levelExpDef = MainPlayer:getLevelExpDef( topLv );
				local lv = (topLv - MainPlayer:getLv()) > 80 and 80 or (topLv - MainPlayer:getLv());
				local num = 100;
				if levelExpDef ~= nil then
					num = num + levelExpDef.ExpMul[lv - 1];
				end
				szText = szText .. num .. "%#n\n";
			else
				szText = szText .. "100%#n\n";
			end
		end
	end

	szText = szText .. "\n#Y获取经验影响击杀怪物与任务经验";

	szText = szText .. "\n\n" .. GetFengYinLv()
	
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor" } );
end

function RoleExpButton_OnLeave()
	WideGameTooltip:Hide();
end

function CheckXPShow()
	local player = ActorMgr:getMainPlayer();
	nCurXP	= player:getXP();
	--if nCurXP > 0 then
	-- 设置宠物XP技能
	SetXPSkillShow();
	nLastXP = nCurXP;
	if XPFrameFullUVAnimationTex:IsShown() and player:getXP() < player:getMaxXP() then
		XPFrameFullUVAnimationTex:Hide();
	end
	--end
end

--更新经验值逻辑
MAX_STORE_EXP_TIMES = 4;
local t_ExpTemplate = 
{
	[-1] =	"RoleExpBlackTextureTemplate",
	[0] =	"RoleExpGreenTextureTemplate",
	[1] =	"RoleExpYellowTextureTemplate",
	[2] =	"RoleExpOrangeTextureTemplate",
	[3] =	"RoleExpRedTextureTemplate",	
	-- 4重复上面
	[4] =	"RoleExpRedTextureTemplate",
};

function initRoleData()
	nCurExp			= 0;
	nLevelNextExp	= 0;
end

function GetCurTexUVIndex()
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurExp			= mainplayer:getExp();
	local nLevelNextExp		= mainplayer:getLvExp( mainplayer:getLv() );
	if nCurExp < nLevelNextExp then
		return 0;
	end

	if nCurExp ==  nLevelNextExp or nCurExp < 2 * nLevelNextExp then
		return 1;
	end

	if nCurExp ==  2 * nLevelNextExp or nCurExp < 3 * nLevelNextExp then
		return 2;
	end

	if nCurExp ==  3 * nLevelNextExp or nCurExp < 4 * nLevelNextExp then
		return 3;
	end

	if nCurExp >=  4 * nLevelNextExp then
		return 4;
	end
end

function UpdateRoleExpTexUV()
	RoleExp_BackTexture:Hide();
	RoleExp_Texture:Hide();
	RoleExpButtonUVAnimationTex:Hide();
	RoleMaxExpButton:Hide();
	RoleExpButtonWarningUVAnimationTex:Hide();
	local mainplayer	= ActorMgr:getMainPlayer();
	local levelup		= getLevelUP( mainplayer:getLv() )
	nCurExp				= mainplayer:getExp();
	nLevelNextExp		= levelup ~= nil and levelup.LevelNextExp or 0
	
	if nCurExp < 0 then
		local fScale = (-nCurExp)/ nLevelNextExp;
		if fScale > 1 then
			fScale = 1;
		end	
		local tex = getglobal( "RoleExp_Texture" );
		tex:ChangeTextureTemplate( t_ExpTemplate[-1] );
		tex:SetSize( 722 * fScale, 7 );
		tex:Show();
		-- 显示经验闪动特效
		local tex = getglobal( "RoleExpButtonWarningUVAnimationTex" );
		if not tex:IsShown() then
			tex:SetUVAnimation( 50, true );
			tex:Show();
		end
	elseif nCurExp <= nLevelNextExp then
		local fScale = nCurExp/ nLevelNextExp;
		local tex = getglobal( "RoleExp_Texture" );
		tex:ChangeTextureTemplate( t_ExpTemplate[0] );
		tex:SetSize( 722 * fScale, 7 );
		tex:Show();
		if nCurExp == nLevelNextEx then
			local tex = getglobal( "RoleExpButtonUVAnimationTex" );
			if not tex:IsShown() then
				tex:SetUVAnimation( 50, true );
				tex:Show();
			end
			local btn = getglobal( "RoleMaxExpButton" );
			if not btn:IsShown() then
				btn:Show();
			end
		end
	else
		local nTotalScale	= GetCurTexUVIndex();
		-- 显示次层经验条		
		local tex = getglobal( "RoleExp_BackTexture" );
		tex:ChangeTextureTemplate( t_ExpTemplate[nTotalScale-1] );
		tex:SetSize( 722, 7 );
		tex:Show();
		
		local fScale = math.fmod( nCurExp, nLevelNextExp ) / nLevelNextExp;
		local tex = getglobal( "RoleExp_Texture" );
		tex:ChangeTextureTemplate( t_ExpTemplate[nTotalScale] );
		tex:SetSize( 722 * fScale, 7 );
		tex:Show();
		-- 显示经验闪动特效
		local tex = getglobal( "RoleExpButtonUVAnimationTex" );
		if not tex:IsShown() then
			tex:SetUVAnimation( 50, true );
			tex:Show();
		end
		local btn = getglobal( "RoleMaxExpButton" );
		if not btn:IsShown() then
			btn:Show();
		end
	end	
	
	if levelup.ExpLeiji > 0 and nCurExp >= levelup.ExpLeiji * nLevelNextExp then
		AddGameMiddleTips2( "你当前存储的经验值已达到上限，请提升等级，否则无法继续获取经验" );
	end
	nLastExp = nCurExp;
end

local t_RoleExpButtonEvents =	{	["UI_ACTOR_ATTRIBUTE_CHG"]	= {}, ["UI_ACTOR_LEVEL_CHG"] = {}, ["GE_ENTER_PLAYERLOGIN"] = {}, 
									["GE_TRUMP_UPDATE"]			= {},	["UI_ACTOR_XP_CHG"] = {},	["UI_ACTOR_EXP_CHG"] = {},
									["GE_LOGIN_GAME_SETTING"]	= {},
							};

t_RoleExpButtonEvents["UI_ACTOR_LEVEL_CHG"].func = 
function ()
	Quest:updateNpcQuestTitle();
	UpdateRoleExpTexUV()
end

t_RoleExpButtonEvents["GE_LOGIN_GAME_SETTING"].func = 
function ()
	initRoleData();
	UpdateRoleExpTexUV()
end

t_RoleExpButtonEvents["UI_ACTOR_XP_CHG"].func = 
function ()
	if IsTrumpCamp() then
		CheckXPShow();
	end
end

t_RoleExpButtonEvents["GE_TRUMP_UPDATE"].func = 
function ()
	t_RoleExpButtonEvents["UI_ACTOR_XP_CHG"].func();
end

t_RoleExpButtonEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	CheckXPShow();
end

t_RoleExpButtonEvents["UI_ACTOR_EXP_CHG"].func = 
function ()
	UpdateRoleExpTexUV();
end

t_RoleExpButtonEvents["UI_ACTOR_ATTRIBUTE_CHG"].func = 
function ()
	local player = ActorMgr:getMainPlayer();
	nCurXP	= player:getXP();
	--local xptex	= getglobal("XPFrame_XPValueTex"), 
	--XPFrame_XPValueTex:SetTexUV( 288, 253, 100*(player:getXP()/ player:getMaxXP()), 7 );
	XPFrame_XPValueTex:SetSize(100*(player:getXP()/ player:getMaxXP()), 7);
	
	if nLastXP ~= nCurXP then
		-- 设置宠物XP技能
		SetXPSkillShow();
		nLastXP = nCurXP;
		if XPFrameFullUVAnimationTex:IsShown() and player:getXP() < player:getMaxXP() then
			XPFrameFullUVAnimationTex:Hide();
		end
	end
end

function RoleExpButton_OnLoad()
	for event, _ in pairs( t_RoleExpButtonEvents ) do
		this:RegisterEvent( event );
	end

	t_RoleExpButtonEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end

	setmetatable( t_RoleExpButtonEvents, t_RoleExpButtonEvents );
end

function RoleExpButton_OnEvent()
	t_RoleExpButtonEvents[arg1].func();
end

function RoleMaxExpButton_OnClick()
	if not EquipFrame:IsShown() then
		EquipFrame:Show();
	else
		EquipFrame:Hide();
	end
end

function RoleMaxExpButton_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "点击打开人物界面", 
						frame = this:GetParent(), button = this:GetName() } );
end

function SetXPButtonUI( nSkillId )
	table.insert( t_SkillId, nSkillId );
	--[[
	if table.getn( t_SkillId ) == 0 then
		table.insert( t_SkillId, nSkillId );
	else
		local index = 1;
		for i, id in ipairs( t_SkillId ) do
			if t_XPSkill[nSkillId].xpused > t_XPSkill[id].xpused then
				index = i+1;
			end
		end
		table.insert( t_SkillId, index, nSkillId  );
	end
	]]--
end

function SetFairySkill()
	t_XPSkill = {};
	t_SkillId = {};
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local GameTrump		= MainPlayer:getTrump();
	local TrumpMaxNum	= GameTrump:getTrumpMaxNum();
	for i = 1, TrumpMaxNum do
		local RoleTrump = GameTrump:getRoleFairy(i-1);
		if RoleTrump.FairyID > 0 and  GameTrump:isFairyInState( RoleTrump.Stat, FAIRY_STAT_CAMP ) then
			local TrumpDef	= GameTrump:getFairyDef( RoleTrump.FairyID, RoleTrump.FairyLevel );	  
			if TrumpDef ~= nil then
				for j = 1, FAIRY_MAX_SKILL do
					if TrumpDef.FairySkillList[j-1].SkillID ~= 0 then
						local nSkillId = TrumpDef.FairySkillList[j-1].SkillID;
						local SkillDef	= nil;
						if TrumpDef.FairySkillList[j-1].SkillType == SKILL_TYPE then
							SkillDef = SkillMgr:getSpellStaticAtt( nSkillId, TrumpDef.FairySkillList[j-1].SkillLevel );
							if SkillDef ~= nil then
								t_XPSkill[nSkillId] = { skilltype = SHORTCUT_FAIRYSKILL, fairyindex = i, shortcuttype = SHORCUT_XP, xpused=SkillDef.UseXp, iconid = SkillDef.IconID1, bEffect = false  };
								SetXPButtonUI( nSkillId );
							end
						else
							SkillDef = SkillMgr:getDiathesisDef( nSkillId, TrumpDef.FairySkillList[j-1].SkillLevel );
							if SkillDef ~= nil then
								t_XPSkill[nSkillId] = { skilltype = SHORTCUT_FAIRYDIATHESIS, fairyindex = i, shortcuttype = SHORCUT_XP, xpused=SkillDef.UseXp, iconid = SkillDef.IconID, bEffect = false  };
								SetXPButtonUI( nSkillId );
							end
						end						
					end					
				end
			end
		end
	end
	SetXPSkillShow();
end

function SortXPSkillId( a,b )
	return t_XPSkill[a].xpused > t_XPSkill[b].xpused;
end

function SetXPSkillShow()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil or table.getn( t_SkillId ) == 0 then
		--GameExchange:test( "table.getn( t_SkillId ) == 0" );
		return;
	end
	--table.sort( t_SkillId,SortXPSkillId );

	--GameExchange:test( "SetXPSkillShow--2164" );
	--XPFrame_XPValueTex:SetTexUV( 288, 253, 100*(MainPlayer:getXP()/ MainPlayer:getMaxXP()), 7 );
	XPFrame_XPValueTex:SetSize(100*(MainPlayer:getXP()/ MainPlayer:getMaxXP()), 7);
	for i = 1, MAX_XPSHORCUT do
		local ShortcuntBtn	= getglobal("XPBufShortcut"..i);
		local backtex		= getglobal("XPFrame_XPBoxTex"..i);
		local locktex		= getglobal("XPFrame_XPBoxTexLock"..i);
		ShortcuntBtn:Hide(); 
		ShortcuntBtn:SetClientUserData( 0, 0 );
		ShortcuntBtn:SetClientUserData( 1, 0 );
		ShortcuntBtn:SetClientUserData( 3, 0 );
	end
	local index = 1;
	for i, id in ipairs( t_SkillId ) do
		--local index = math.ceil(t_XPSkill[id].xpused/(MainPlayer:getMaxXP()/3));		
		--GameExchange:test( "index = "..index );
		if id > 0 and index > 0 and  index <= MAX_XPSHORCUT then			
			local ShortcuntBtn			= getglobal("XPBufShortcut"..index);
			local ShortcuntBtnIcon		= getglobal("XPBufShortcut"..index.."Icon");
			local ShortcuntBtnBoxTex	= getglobal("XPBufShortcut"..index.."BoxTexture");	   
			local ShortcuntBtnUVATex	= getglobal("XPBufShortcut"..index.."UVAnimationTex");
			ShortcuntBtnUVATex:Hide();
			ShortcuntBtn:SetClientUserData( 0, t_XPSkill[id].skilltype );
			ShortcuntBtn:SetClientUserData( 1, id );
			ShortcuntBtn:SetClientUserData( 2, t_XPSkill[id].fairyindex ); -- 技能所属精灵的栏位
			ShortcuntBtn:SetClientUserData( 3, t_XPSkill[id].shortcuttype );
			ShortcuntBtnBoxTex:Show();
			ShortcuntBtn:Show();
			if SkillMgr:isSkillPassive(id) then
				ShortcuntBtn:Disable()
			else
				ShortcuntBtn:Enable()
			end
			local IconPath;
			if t_XPSkill[id].skilltype == SHORTCUT_FAIRYSKILL then
				IconPath = GetSkillIconPath()..t_XPSkill[id].iconid..".tga";
			else
				IconPath = getDiathesisPath()..t_XPSkill[id].iconid..".tga";
			end
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ShortcuntBtnIcon:SetTexture( IconPath );
			GameShortCut:setShortCut2UI( 1, id, ( ShortcuntBtn:GetClientID() - 1 ), 0 );	 
			SetXPShortCutCDTime( id, ShortcuntBtn:GetName() ); -- 设置技能的冷时间
			-- 如果宠物身上有XP技能则播放特效后显示技能图标
			if MainPlayer:checkXP( id ) == 0 or t_XPSkill[id].skilltype == SHORTCUT_FAIRYDIATHESIS then
				--GameExchange:test( "SetXPSkillShow--2214，id = "..id );
				ShortcuntBtnIcon:SetGray(false);
				if t_XPSkill[id].skilltype ~= SHORTCUT_FAIRYDIATHESIS then
					ShortcuntBtnUVATex:SetUVAnimation( 160, true );
					ShortcuntBtnUVATex:Show();
				end
				if not t_XPSkill[id].bEffect then 
					SetMove(ShortcuntBtn:GetClientID());
					t_XPSkill[id].bEffect = true;
					XPFrameYiGeUVAnimationTex:SetTexUV(0,0,128,128);
					SetXPSkillEffective( t_XPSkill[id].bEffect, os.clock() );
				end
			else
				--GameExchange:test( "SetXPSkillShow--2224，id = "..id );
				t_XPSkill[id].bEffect = false;
				ShortcuntBtnIcon:SetGray(true);
				local data = t_XpBtnInfo[ShortcuntBtn:GetClientID()];
				data.btn:SetPoint( "topleft", "XPFrame", "topleft", data.refposx, data.refposy );
				data.btn:SetSize( data.minsize,data.minsize );
				data.boxtex:SetSize( data.minsize,data.minsize );
				data.uvatex:SetSize( data.minuvasize,data.minuvasize );
				data.icontex:SetSize( data.miniconsize,data.miniconsize );
				data.bmove = false;
			end
			index = index + 1;
		end
	end
	
	if MainPlayer:getXP() ~= 0 and MainPlayer:getXP() >= MainPlayer:getMaxXP() and not GameWizard:isDo( GAMEWIZARD_FIST_TRUMP_QUEST_XP_FULL ) then
		nStartXPEffect = os.clock();		
		GameWizard:doStep( GAMEWIZARD_FIST_TRUMP_QUEST_XP_FULL );
	end
	
	if MainPlayer:getXP() == MainPlayer:getMaxXP() then
		if not XPFrameFullUVAnimationTex:IsShown() then
			XPFrameFullUVAnimationTex:SetUVAnimation(50, true);
			XPFrameFullUVAnimationTex:Show();
		end
	else
		XPFrameFullUVAnimationTex:Hide();
	end
end

function ResetXPBtnInfo()
	local nDefault_RefPosX		= 63-4;
	local nDefault_RefPosY		= 9+1;
	local nDefault_OnEnterposX	= 53-4;
	local nDefault_OnEnterposY	= -20+1;
	local nDefault_OnLeaveposX	= 63-4;
	local nDefault_OnLeaveposY	= -10+1;
	local nDefault_MoveTime		= 0.1;
	local nDefault_MoveHeight	= 20;
	local nDefault_MaxSize		= 42;
	local nDefault_MinSize		= 28;
	local nDefault_MaxIconSize	= 38;
	local nDefault_MinIconSize	= 24;
	local nDefault_MinUVASize	= 42; 
	local nDefault_MaxUVASize	= 58;
	t_XpBtnInfo =
		{
			[43] = 
				{	
					refposx		= nDefault_RefPosX+5,		refposy		= nDefault_RefPosY, 	
					onenterposx = nDefault_OnEnterposX+5, onenterposy = nDefault_OnEnterposY,
					onleaveposx = nDefault_OnLeaveposX+5, onleaveposy = nDefault_OnLeaveposY,
					movetime	= nDefault_MoveTime,	moveheight	= nDefault_MoveHeight, 
					maxiconsize = nDefault_MaxIconSize, maxsize		= nDefault_MaxSize,
					minuvasize  = nDefault_MinUVASize,  maxuvasize	= nDefault_MaxUVASize,
					miniconsize = nDefault_MinIconSize, minsize		= nDefault_MinSize,
					bmove	= false;
					btn		= getglobal("XPBufShortcut1"), 
					icontex	= getglobal("XPBufShortcut1Icon"), 
					boxtex	= getglobal("XPBufShortcut1BoxTexture"), 
					uvatex	= getglobal("XPBufShortcut1UVAnimationTex"), 
					backtex = getglobal("XPFrame_XPBoxTex1"), 
					locktex	= getglobal("XPFrame_XPBoxTexLock1")
				},
			[44] = 
				{	
					refposx		= nDefault_RefPosX + 40-3,		refposy		= nDefault_RefPosY, 	
					onenterposx = nDefault_OnEnterposX + 43-3,	onenterposy = nDefault_OnEnterposY,
					onleaveposx = nDefault_OnLeaveposX + 40-3,	onleaveposy = nDefault_OnLeaveposY,
					movetime	= nDefault_MoveTime,			moveheight	= nDefault_MoveHeight, 
					maxiconsize = nDefault_MaxIconSize,			maxsize		= nDefault_MaxSize,
					minuvasize  = nDefault_MinUVASize,			maxuvasize	= nDefault_MaxUVASize,
					miniconsize = nDefault_MinIconSize,			minsize		= nDefault_MinSize,
					bmove	= false;
					btn		= getglobal("XPBufShortcut2"), 
					icontex	= getglobal("XPBufShortcut2Icon"), 
					boxtex	= getglobal("XPBufShortcut2BoxTexture"), 
					uvatex	= getglobal("XPBufShortcut2UVAnimationTex"), 
					backtex = getglobal("XPFrame_XPBoxTex2"), 
					locktex	= getglobal("XPFrame_XPBoxTexLock2")
				},

			[45] = 
				{	
					refposx		= nDefault_RefPosX + 70,	 refposy		= nDefault_RefPosY, 	
					onenterposx = nDefault_OnEnterposX + 73, onenterposy	= nDefault_OnEnterposY,
					onleaveposx = nDefault_OnLeaveposX + 70, onleaveposy	= nDefault_OnLeaveposY,
					movetime	= nDefault_MoveTime,	moveheight	= nDefault_MoveHeight, 
					maxiconsize = nDefault_MaxIconSize, maxsize		= nDefault_MaxSize,
					minuvasize  = nDefault_MinUVASize,  maxuvasize	= nDefault_MaxUVASize,
					miniconsize = nDefault_MinIconSize, minsize		= nDefault_MinSize,
					bmove	= false;
					btn		= getglobal("XPBufShortcut3"), 
					icontex	= getglobal("XPBufShortcut3Icon"), 
					boxtex	= getglobal("XPBufShortcut3BoxTexture"), 
					uvatex	= getglobal("XPBufShortcut3UVAnimationTex"), 
					backtex = getglobal("XPFrame_XPBoxTex3"), 
					locktex	= getglobal("XPFrame_XPBoxTexLock3")
				},
		};
	for i = 1, MAX_XPSHORCUT do
		local ShortcuntBtn	= getglobal("XPBufShortcut"..i);
		local backtex		= getglobal("XPFrame_XPBoxTex"..i);
		local locktex		= getglobal("XPFrame_XPBoxTexLock"..i);
		ShortcuntBtn:Hide(); 
		backtex:Show();
		locktex:Show();
		ShortcuntBtn:SetClientUserData( 0, 0 );
		ShortcuntBtn:SetClientUserData( 1, 0 );
		ShortcuntBtn:SetClientUserData( 3, 0 );
	end
end

function SetMove( nClientID )
	local data = t_XpBtnInfo[nClientID];
	data.btn:SetPoint( "topleft", "XPFrame", "topleft", data.refposx, data.refposy );
	data.btn:MoveFrame("top", data.movetime, data.btn:GetWidth(), data.moveheight);
	data.bmove = true;
	data.icontex:SetSize( data.miniconsize, data.miniconsize );
	data.uvatex:SetSize( data.minuvasize,data.minuvasize );
	data.boxtex:SetSize( data.minsize, data.minsize );

	data.boxtex:Show();
	
	data.backtex:Hide();
	data.locktex:Hide();
end

local t_relBtnFrame =	{ 
						--["DiathesisButton"]					= { frame = "DiathesisFrame",		tips = "天赋", key = "打开天赋界面" }, 
						--["TrustShortcutButton"]				= { frame = "TrustFrame",			tips = "委托" }, 
						["RideShortcutButton"]				= { frame = "RideFrame",			tips = "骑乘", key = "打开骑乘面板" }, 
						["SecondarySkillShortcutButton"]	= { frame = "SecondarySkillFrame",	tips = "科技生产", key = "打开科技生产" },
						};

--[[
t_relBtnFrame["DiathesisButton"].func = 
function ()
	do return ShowMidTips("该功能暂未开放") end--天赋面板暂时屏蔽
	GuideEffectHide( "DiathesisButton" );
	local Frame = getglobal("DiathesisFrame");
	if(Frame:IsShown()) then
		Frame:Hide();
	else
		Frame:Show();
	end
end

t_relBtnFrame["TrustShortcutButton"].func = 
function ()
	TrustBtn_OnClick();
end
--]]

t_relBtnFrame["RideShortcutButton"].func = 
function ()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then
		return;
	end
	local rider	= mainplayer:getRider();
	if rider:getRidNum() == 0 then
		ShowMidTips( "您还没有坐骑，不能打开坐骑面板" );
		return;
	end

	local frame = getglobal( t_relBtnFrame["RideShortcutButton"].frame );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
	GuideEffectHide( "RideShortcutButton" );
end

t_relBtnFrame["SecondarySkillShortcutButton"].func = 
function ()
	local frame = getglobal( t_relBtnFrame["SecondarySkillShortcutButton"].frame );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
end

function ClickThreeShortCut( szButtonName )
	t_relBtnFrame[szButtonName].func();
end

function ShortcutButton_OnClick()
	ClickThreeShortCut( this:GetName() );
end

function ShortcutButton_OnEnter()
	--[[
	if this:GetName() == "RideShortcutButton" then
		local mainplayer = ActorMgr:getMainPlayer()
		local rider = mainplayer:getRider()
		if rider:getRidNum() == 0 then
			local str = "获得坐骑后激活\n坐骑可大幅度提升移动速度"
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = str, 
						frame = this:GetParent(), button = this:GetName() } );
			return
		end
	elseif this:GetName() == "SecondarySkillShortcutButton" then
		if SecondarySkillMgr:getSecondarySkillNum() == 0 then
			local str = "学会专业后激活\n科技可生产出各种装备道具"
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = str, 
						frame = this:GetParent(), button = this:GetName() } );
			return
		end
	end
	--]]
	local str = t_relBtnFrame[this:GetName()]["tips"]
	local key = GetKeyMap(t_relBtnFrame[this:GetName()].key)
	if key == 0 then
		key = nil
	end
	if key then
		str = string.format("%s ( 快捷键 %s )", str, KeyToString(key))
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = str, 
						frame = this:GetParent(), button = this:GetName() } );	
end

local t_pingTex = 
{
["良好"] = { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 969, y = 388,	width = 31, height = 31 }, },
["正常"] = { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 969, y = 420,	width = 31, height = 31 }, },
["繁忙"] = { path = "uires\\ZhuJieMian\\3.dds", UV = { x = 969, y = 452,	width = 31, height = 31 }, },
};

-- 参考值：往返值 <= 100ms良好，100ms< <=300ms正常，> 300ms繁忙
local function UpdatePing( fDiffTime )
	local szState ="良好";
	if fDiffTime <= 200 then
		szState ="良好";
	elseif fDiffTime <= 400 then
		szState ="正常";
	else
		szState ="繁忙";
	end

	local btn = getglobal( "NetWorkStatus" );
	btn:SetNormalTexture( t_pingTex[szState].path, "blend" )
	btn:ChangeNormalTexture( t_pingTex[szState].UV.x, t_pingTex[szState].UV.y, t_pingTex[szState].UV.width, t_pingTex[szState].UV.height )
	btn:SetClientString(  "网络状况："..szState.."( "..fDiffTime.."ms )" );
end

function NetWorkStatusOnUpdate()
	local ping = LoginMgr:GetPing()
	UpdatePing( ping )
end

function NetWorkStatusOnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text =  this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function DisCheckShortcutButton()
	for i =1, MAX_SHORCUT do
		if( i <= 10 or i >=13 ) then
			local MagicShortcut = getglobal("MagicShortcut"..i);
			if MagicShortcut ~= nil then
				MagicShortcut:DisChecked();
			end
		end
	end
end

function MagicShortfurl_OnClick()
	MagicShortfurl:Hide();
	MagicShortRestore:Show();
	GameTooltipOnLeave();
end

function MagicShortRestore_OnClick()
	this:Hide(); 
	MagicShortfurl:Show();
	GameTooltipOnLeave();
end

function MagicShortcutControl_OnEnter()
	if not MagicShortfurl:IsShown() then
		SetGameTooltips(this:GetName(), "展开快捷栏" );
	else
		SetGameTooltips(this:GetName(), "收缩快捷栏" );
	end
end

function ExtendSkillRightFrame_OnUpdate()
	if MagicShortfurl:IsShown() and not ExtendSkillLeftFrame:IsShown() then
		ExtendSkillLeftFrame:Show();
		MagicShortfurl:SetPoint( "topleft","MagicShortcut42","BottomLeft", -17, -6 );	
	elseif not MagicShortfurl:IsShown() and ExtendSkillLeftFrame:IsShown() then
		ExtendSkillLeftFrame:Hide();  
		MagicShortRestore:SetPoint( "topleft","MagicShortcut42","BottomLeft", 6, -6 );	
	end
end

function ExtendSkillLeftFrame_OnShow()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end
	for i = 23, 32 do
		local btn = getglobal( "MagicShortcut" .. i );
		local t_itemcd = Get_ItemCDTable();
		if btn:GetClientUserData(1) == 0 then
			return;
		end

		if table.getn( t_itemcd ) > 0 and btn:GetClientUserData(0) == SHORTCUT_ITEM then
			btn:SetCooldownTimer( 0, 0, true );
			for i = 1, table.getn( t_itemcd ) do	 
				if t_itemcd[i][1] == btn:GetClientUserData(1) then
					local starttime = container:getItemCDPassTime( btn:GetClientUserData(1) );	  
					btn:SetCooldownTimer( starttime, t_itemcd[i][2], true );
					
					if btn:GetClientID() >= 43 then	 
						btn:SetCooldownTextureRect( 2, 2, 26, 26 );
					else
						btn:SetCooldownTextureRect( 3, 3, 38, 38 );
					end
				end
			end
		elseif btn:GetClientUserData(0) == SHORTCUT_SKILL or btn:GetClientUserData(0) == SHORTCUT_FAIRYSKILL then 
			local skillCDTime	= SkillMgr:getCDTime( btn:GetClientUserData(1) );
			local nCurtime		= GameMgr:getTickTime();
			btn:SetCooldownTimer(skillCDTime:past() / 1000, skillCDTime:total() / 1000, true);
			--[[if skillCDTime ~= nil then
				if nCurtime - skillCDTime.startTime > 0 then
					btn:SetCooldownTimer( (nCurtime - skillCDTime.startTime)/1000, (nCurtime - skillCDTime.startTime + skillCDTime.nPublicCD)/1000, true );
				else
					btn:SetCooldownTimer(0,0,true);
				end
			end]]
		end
	end
end

function SetStartXpUse( bStart, nSecond, nTime )
	bStartXpUse = bStart;
	nXPDuration = nSecond;
	nStartXpTime= nTime;
end

function GetStartXpUse()
	return bStartXpUse;
end

function GetStartXpTime()
	return nStartXpTime;
end

function GetXPDuration()
	return nXPDuration;
end

function XPValueBtnOnEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local szText = "魂能："..MainPlayer:getXP().."/"..MainPlayer:getMaxXP();
	SetGameTooltips(this:GetName(), szText );
end

function XPFrame_OnShow()
	this:SetPoint("BOTTOMRIGHT","$parent","BOTTOMRIGHT", -210, -55 );
	ResetXPBtnInfo();
	SetFairySkill();
	
	if not bHideTmpFrame and TempShortCutFrame:IsShown() then
		XPFrame:Hide();
	end
end

function XPFrame_OnHide()
	if SpellTipsFrame:IsShown() then
		if IsInFrameAbs( "SpellTipsFrame", "XPFrame" ) then
			SpellTipsFrame:Hide();
		end
	end
end

function ShowMouseHabitFrame()
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or CameraCoverFrame:IsShown() then 
		return;
	end
	
	RoleInit();
	local t_SelectLeftHabitFrameControl = GetSelectLeftHabitFrameControl();
	if not t_SelectLeftHabitFrameControl:isNewLogin() then
		return;
	end

	local systemSetting	= GameMgr:getUserSystemConfig();
	local frame = getglobal( "SelectLeftHabitFrame" );
--	frame:Show();
--	frame:SetPoint( "center", "UIClient", "center", 0, 0 );
	t_SelectLeftHabitFrameControl:setNewLoginState( { state = false } );
	t_SelectLeftHabitFrameControl:setHabit( { selHabit = RIGHT_HABIT } );
	local systemSetting	= GameMgr:getUserSystemConfig();
	systemSetting:setGameOptValue( GAME_OPT_LAST_PET_SELECT, 0 );
--	systemSetting:saveGameOpt();
	ConfirmDesicion();
end

-- 新的重写
-- TODO:上面要删除
NONE_HABIT	= 0;
LEFT_HABIT	= 1;
RIGHT_HABIT	= 2;
local t_SelectLeftHabitFrameControl = { selHabit = NONE_HABIT, isNewLoginState = true };

function GetSelectLeftHabitFrameControl()
	return t_SelectLeftHabitFrameControl;
end

t_SelectLeftHabitFrameControl["init"] = 
function ( self )
	self["selHabit"]		= NONE_HABIT;
	self["isNewLoginState"]	= true;
end

t_SelectLeftHabitFrameControl["isNewLogin"] = 
function ( self, data )
	return self["isNewLoginState"];
end

t_SelectLeftHabitFrameControl["setNewLoginState"] = 
function ( self, data )
	self["isNewLoginState"] = data["state"];
end

t_SelectLeftHabitFrameControl["setHabit"] = 
function ( self, data )
	self["selHabit"] = data["selHabit"];
end

t_SelectLeftHabitFrameControl["getHabit"] = 
function ( self )
	return self["selHabit"];
end

local t_SelectLeftHabitFrameFontsColor = 
{	["enter"]	= 
	{ 	
		["SelectLeftHabitFrameLeftHabitIntroBtnCenterIntroFont"]	= { color = { r = 120, g = 245, b = 255 }, },
		["SelectLeftHabitFrameLeftHabitIntroBtnLeftAttackFont"]		= { color = { r = 255, g = 0, b = 0	}, },
		["SelectLeftHabitFrameLeftHabitIntroBtnLeftMoveFont"]		= { color = { r = 120, g = 245, b = 255 }, },
		["SelectLeftHabitFrameLeftHabitIntroBtnRightSelectFont"]	= { color = { r = 120, g = 245, b = 255 }, },


		["SelectLeftHabitFrameRightHabitIntroBtnCenterIntroFont"]	= { color = { r = 120, g = 245, b = 255 }, },
		["SelectLeftHabitFrameRightHabitIntroBtnLeftAttackFont"]	= { color = { r = 120, g = 245, b = 255 }, },
		["SelectLeftHabitFrameRightHabitIntroBtnLeftMoveFont"]		= { color = { r = 120, g = 245, b = 255 }, },
		["SelectLeftHabitFrameRightHabitIntroBtnRightSelectFont"]	= { color = { r = 255, g = 0,	b = 0 }, }, 
	},	
	

	["leave"]	= 
	{	["SelectLeftHabitFrameLeftHabitIntroBtnCenterIntroFont"]	= { color = { r = 255, g = 175, b = 0	}, },
		["SelectLeftHabitFrameLeftHabitIntroBtnLeftAttackFont"]		= { color = { r = 255, g = 0, b = 0	}, },
		["SelectLeftHabitFrameLeftHabitIntroBtnLeftMoveFont"]		= { color = { r = 255, g = 175, b = 0	}, },
		["SelectLeftHabitFrameLeftHabitIntroBtnRightSelectFont"]	= { color = { r = 255, g = 175, b = 0	}, },


		["SelectLeftHabitFrameRightHabitIntroBtnCenterIntroFont"]	= { color = { r = 255, g = 175, b = 0	}, },
		["SelectLeftHabitFrameRightHabitIntroBtnLeftAttackFont"]	= { color = { r = 255, g = 175, b = 0	}, },
		["SelectLeftHabitFrameRightHabitIntroBtnLeftMoveFont"]		= { color = { r = 255, g = 175, b = 0	}, },
		["SelectLeftHabitFrameRightHabitIntroBtnRightSelectFont"]	= { color = { r = 255, g = 0,	b = 0	}, },
	}, 
};

function CheckSelectLeftHabitFrame_LeftHabitIntroBtn()
	local t_fonts = 
	{	
	["SelectLeftHabitFrameLeftHabitIntroBtnCenterIntroFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameLeftHabitIntroBtnCenterIntroFont"]["color"], },

	["SelectLeftHabitFrameLeftHabitIntroBtnLeftAttackFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameLeftHabitIntroBtnLeftAttackFont"]["color"], },

	["SelectLeftHabitFrameLeftHabitIntroBtnLeftMoveFont"]		= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameLeftHabitIntroBtnLeftMoveFont"]["color"], },

	["SelectLeftHabitFrameLeftHabitIntroBtnRightSelectFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameLeftHabitIntroBtnRightSelectFont"]["color"], },
	};

	util.SetFontsColor( t_fonts );

	local t_textures = 
	{
		["SelectLeftHabitFrameLeftHabitIntroBtnLeftArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 824, width = 50, height = 8 },
											},

		["SelectLeftHabitFrameLeftHabitIntroBtnRightArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 833, width = 50, height = 8 },
											},
			
		["SelectLeftHabitFrameLeftHabitIntroBtnCenterArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 799, y = 907, width = 8, height = 50 },
											},
	};
	util.SetTextures( t_textures );
end

function DischeckSelectLeftHabitFrame_LeftHabitIntroBtn()
	local t_fonts = 
	{	
	["SelectLeftHabitFrameLeftHabitIntroBtnCenterIntroFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameLeftHabitIntroBtnCenterIntroFont"]["color"], },

	["SelectLeftHabitFrameLeftHabitIntroBtnLeftAttackFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameLeftHabitIntroBtnLeftAttackFont"]["color"], },

	["SelectLeftHabitFrameLeftHabitIntroBtnLeftMoveFont"]		= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameLeftHabitIntroBtnLeftMoveFont"]["color"], },

	["SelectLeftHabitFrameLeftHabitIntroBtnRightSelectFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameLeftHabitIntroBtnRightSelectFont"]["color"], },
	};

	util.SetFontsColor( t_fonts );

	local t_textures = 
	{
		["SelectLeftHabitFrameLeftHabitIntroBtnLeftArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 842, width = 50, height = 8 },
											},

		["SelectLeftHabitFrameLeftHabitIntroBtnRightArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 851, width = 50, height = 8 },
											},
			
		["SelectLeftHabitFrameLeftHabitIntroBtnCenterArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 790, y = 907, width = 8, height = 50 },
											},
	};
	util.SetTextures( t_textures );
end

function SelectLeftHabitFrame_LeftHabitIntroBtn_OnEnter()
	CheckSelectLeftHabitFrame_LeftHabitIntroBtn();
end

function SelectLeftHabitFrame_LeftHabitIntroBtn_OnLeave()
	if this:IsChecked() then
		return;
	end
	DischeckSelectLeftHabitFrame_LeftHabitIntroBtn();
end

function SelectLeftHabitFrame_LeftHabitIntroBtn_OnClick()
	t_SelectLeftHabitFrameControl:setHabit( { selHabit = LEFT_HABIT } );
	local t_btns = { "SelectLeftHabitFrameLeftHabitIntroBtn", "SelectLeftHabitFrameRightHabitIntroBtn", };
	util.DischeckBtns( t_btns );
	DischeckSelectLeftHabitFrame_RightHabitIntroBtn();
	this:Checked();
	CheckSelectLeftHabitFrame_LeftHabitIntroBtn();
end

function SelectLeftHabitFrame_LeftHabitIntroBtn_OnDoubleClick()
	SelectLeftHabitFrame_LeftHabitIntroBtn_OnClick()
	ConfirmDesicion();
end

function CheckSelectLeftHabitFrame_RightHabitIntroBtn()
	local t_fonts = 
	{	
	["SelectLeftHabitFrameRightHabitIntroBtnCenterIntroFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameRightHabitIntroBtnCenterIntroFont"]["color"], },

	["SelectLeftHabitFrameRightHabitIntroBtnLeftAttackFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameRightHabitIntroBtnLeftAttackFont"]["color"], },

	["SelectLeftHabitFrameRightHabitIntroBtnLeftMoveFont"]		= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameRightHabitIntroBtnLeftMoveFont"]["color"], },

	["SelectLeftHabitFrameRightHabitIntroBtnRightSelectFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["enter"]["SelectLeftHabitFrameRightHabitIntroBtnRightSelectFont"]["color"], },
	};
	util.SetFontsColor( t_fonts );

	local t_textures = 
	{
		["SelectLeftHabitFrameRightHabitIntroBtnLeftArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 824, width = 50, height = 8 },
											},

		["SelectLeftHabitFrameRightHabitIntroBtnRightArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 833, width = 50, height = 8 },
											},
			
		["SelectLeftHabitFrameRightHabitIntroBtnCenterArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 799, y = 907, width = 8, height = 50 },
											},
	};
	util.SetTextures( t_textures );
end

function DischeckSelectLeftHabitFrame_RightHabitIntroBtn()
	local t_fonts = 
	{	
	["SelectLeftHabitFrameRightHabitIntroBtnCenterIntroFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameRightHabitIntroBtnCenterIntroFont"]["color"], },

	["SelectLeftHabitFrameRightHabitIntroBtnLeftAttackFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameRightHabitIntroBtnLeftAttackFont"]["color"], },

	["SelectLeftHabitFrameRightHabitIntroBtnLeftMoveFont"]		= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameRightHabitIntroBtnLeftMoveFont"]["color"], },

	["SelectLeftHabitFrameRightHabitIntroBtnRightSelectFont"]	= 
	{ color = t_SelectLeftHabitFrameFontsColor["leave"]["SelectLeftHabitFrameRightHabitIntroBtnRightSelectFont"]["color"], },
	};

	util.SetFontsColor( t_fonts );

	local t_textures = 
	{
		["SelectLeftHabitFrameRightHabitIntroBtnLeftArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 842, width = 50, height = 8 },
											},

		["SelectLeftHabitFrameRightHabitIntroBtnRightArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 949, y = 851, width = 50, height = 8 },
											},
			
		["SelectLeftHabitFrameRightHabitIntroBtnCenterArrowTex"] = {	path = "uires\\ZhuJieMian\\2.dds", UV = { x = 790, y = 907, width = 8, height = 50 },
											},
	};
	util.SetTextures( t_textures );
end

function SelectLeftHabitFrame_RightHabitIntroBtn_OnEnter()
	CheckSelectLeftHabitFrame_RightHabitIntroBtn();
end

function SelectLeftHabitFrame_RightHabitIntroBtn_OnLeave()
	if this:IsChecked() then
		return;
	end

	DischeckSelectLeftHabitFrame_RightHabitIntroBtn();
end

function SelectLeftHabitFrame_RightHabitIntroBtn_OnClick()
	t_SelectLeftHabitFrameControl:setHabit( { selHabit = RIGHT_HABIT } );
	local t_btns = { "SelectLeftHabitFrameLeftHabitIntroBtn", "SelectLeftHabitFrameRightHabitIntroBtn", };
	util.DischeckBtns( t_btns );
	DischeckSelectLeftHabitFrame_LeftHabitIntroBtn();
	this:Checked();
	CheckSelectLeftHabitFrame_RightHabitIntroBtn();
end

function SelectLeftHabitFrame_RightHabitIntroBtn_OnDoubleClick()
	SelectLeftHabitFrame_RightHabitIntroBtn_OnClick();
	ConfirmDesicion();
end

function SelectLeftHabitFrame_NextAutoWarnBtn_OnClick()
	local chkBtn = getglobal( this:GetParent().."_NextAutoSelectChkBtn" );
	chkBtn:setCheckState( not chkBtn:GetCheckState() );
end

function ConfirmDesicion()
	local systemSetting	= GameMgr:getUserSystemConfig();
	local nSelectHabit	= t_SelectLeftHabitFrameControl:getHabit();
	if nSelectHabit == NONE_HABIT then
		ShowMidTips( "请先选择一种鼠标模式" );
		return;
	end

	if nSelectHabit == LEFT_HABIT then
		systemSetting:setGameOpt( GAME_OPT_SWAP_MOUSE, true );
	elseif nSelectHabit == RIGHT_HABIT then
		systemSetting:setGameOpt( GAME_OPT_SWAP_MOUSE, false );
	end

	local chkBtn = getglobal( "SelectLeftHabitFrame_NextAutoSelectChkBtn" );

	local frame = getglobal( "SelectLeftHabitFrame" );
	frame:Hide();
	systemSetting:saveGameOpt();

	if chkBtn:GetCheckState() then
		GameWizard:doStep( GAMEWIZARD_FIRST_POP_MOUSE_HABIT_XINSHOU_FRAME );
	end
end

-- 确定按钮
function SelectLeftHabitFrame_ConfirmBtn_OnClick()
	--[[
	ActorManager::getSingleton().getGameConfig().setGameOpt(GAME_OPT_SWAP_MOUSE, bool);

	ActorManager::getSingleton().getGameConfig().getGameOpt(GAME_OPT_SWAP_MOUSE);
	true就是左键模式，false就是右键模式，默认是false

	--]]

	ConfirmDesicion();
end

function SelectLeftHabitFrame_OnHide()
	PlayerFirstBorn();
	
	local t_hideIcons = { "ChangeMouseHabitLockFrame", };
	util.HideIcons( t_hideIcons );
end

function SelectLeftHabitFrame_OnShow()
	local t_showIcons = { "ChangeMouseHabitLockFrame", };
	util.ShowIcons( t_showIcons );
	ShowMidTips( "请先设置鼠标习惯，才能开始游戏" );

	local t_btns = { "SelectLeftHabitFrameLeftHabitIntroBtn", "SelectLeftHabitFrameRightHabitIntroBtn", };
	util.DischeckBtns( t_btns );
	t_SelectLeftHabitFrameControl:setHabit( { selHabit = LEFT_HABIT } );
	SelectLeftHabitFrameLeftHabitIntroBtn:Checked();

	local t_chkBtns =	{	
						["SelectLeftHabitFrame_NextAutoSelectChkBtn"]			= { state = true },
						};
	util.SetChkState( t_chkBtns );
	SelectLeftHabitFrameConfigRich:SetText("可在“系统设置”中进行修改",255,0,0);
end

function SelectLeftHabitFrameConfigRich_OnClick()
	local t_mutexFrames = { "SystemSettingFrame", "GameSettingFrame", "MouseSettingFrame" };
	local curShowFrame;
	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			curShowFrame = frame;
			break;
		end
	end

	for _, name in ipairs( t_mutexFrames ) do
		local frame = getglobal( name );
		frame:Hide();
	end

	if curShowFrame ~= nil then
		MouseSettingFrame:SetPoint( "topleft", curShowFrame:GetName(), "topleft", 0, 0 );
	else
		MouseSettingFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	end
	MouseSettingFrame:Show();
end

local MAX_LOOTITEM_NUM = 10;
local MAX_EXIST_TIME = 0.5;
local t_BootShowTime = {};

function LootItemFrame_OnLoad()
	for i=1,MAX_LOOTITEM_NUM do
		local btn = getglobal( "LootItemFrameLootItem"..i );
		btn:setUpdateTime(0);
	end
end

function OnAddNewItem( itemId,nListType,nGrid )
	local itemDef = getItemDef( itemId );
	for i=1,MAX_LOOTITEM_NUM do	
		if t_BootShowTime["LootItemFrameLootItem"..i] == nil or t_BootShowTime["LootItemFrameLootItem"..i] == 0 then
			local btn = getglobal( "LootItemFrameLootItem"..i );
			local tex = getglobal( "LootItemFrameLootItem"..i.."Tex" );
			local backTex = getglobal( "LootItemFrameLootItem" ..i.."BackTex");
			local UVTex = getglobal("LootItemFrameLootItem"..i.."UVAnimationTex");
			local boxTex = getglobal( "LootItemFrameLootItem"..i.."UVAnimationTex" );
			btn:SetClientUserData( 0, itemId );
			SetEquipImportantEffectUV( itemDef, UVTex:GetName());
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";			
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			tex:SetTexture( IconPath );
			tex:SetSize( 40,40 );
			tex:SetBlendAlpha( 1 );
			backTex:SetSize( 40,40 );
			backTex:SetBlendAlpha( 1 );
			UVTex:SetSize( 64,64 );
			UVTex:SetBlendAlpha( 1 );
			boxTex:SetSize(44,44);
			boxTex:SetBlendAlpha(1);	
			UVTex:SetPoint("topleft","LootItemFrameLootItem"..i,"topleft",-11,-11);
			btn:SetPoint( "topleft","ContainerButton","topright",-15,-65 );
			btn:Show();
			t_BootShowTime["LootItemFrameLootItem"..i] = os.clock();
			break;
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	
	-- 是否是机甲兽
	if itemDef.Result[0].ResultID == RESULT_MACHINE_ITEM and nListType == CONTAINER_TYPE_TASK then
		--播放动画，设置快捷栏
		if not PlayerContainerFrame:IsShown() then
			PlayerContainerFrame:Show();
		end
		ShowTaskContainer();
		local btn = getglobal( "SetMachineShortCutFrameBtn" );
		local tex = getglobal( btn:GetName() .. "Tex" );
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";			
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		tex:SetTexture( IconPath );
		btn:SetPoint( "topleft","MagicShortcut42","topleft",-100,0 );
		btn:Show();
		btn:SetClientUserData( 0, GameMgr:getTickTime() );
		btn:SetClientUserData( 1, itemId )
		btn:setUpdateTime( 0 )
	end

	if itemId == 5300884 and not GameWizard:isDo( GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP1 ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP2 );
		if not PlayerContainerFrame:IsShown() then
			PlayerContainerFrame:Show();
		end
		ShowTaskContainer();
		GameWizard:doStep( GAMEWIZARD_FIRST_GET_BROKEN_JIJIASHOU_STEP1 )
	end
end

function LootItemFrameLootItem_OnUpdate()
	local remainTime = os.clock() - t_BootShowTime[ this:GetName() ];
	local scale = 1 - remainTime/MAX_EXIST_TIME ;
	local tex = getglobal( this:GetName().."Tex" );
	local backTex = getglobal( this:GetName().."BackTex");
	local UVTex = getglobal(this:GetName().."UVAnimationTex");
	local boxTex = getglobal(this:GetName().."BoxTexture");
	backTex:SetSize( 40*scale,40*scale );
	tex:SetSize( 40*scale,40*scale );
	tex:SetBlendAlpha( scale );
	backTex:SetBlendAlpha( scale );
	UVTex:SetSize( 64*scale,64*scale );
	UVTex:SetBlendAlpha( scale );
	boxTex:SetSize(44*scale,44*scale);
	boxTex:SetBlendAlpha(scale);
	UVTex:SetPoint("topleft",this:GetName(),"topleft",math.floor(-11*scale),math.floor(-11*scale));
	this:SetPoint( "topleft","ContainerButton","topright",-15,-50*scale*1.3 );
	if scale < 0.5 then
		this:Hide();
		t_BootShowTime[ this:GetName() ] = 0;
	end
end
function LootItemFrameLootItem_OnHide()
	--在背包里有了物品之后才提示获得更好装备的面板
	if not BetterEquipFrame:IsShown() then
		local mainplayer = ActorMgr:getMainPlayer();
		local rider	= mainplayer:getRider();
		if mainplayer:isInStall() or mainplayer:isOnMonsterMachine() then return end
		if mainplayer:getLv() >= 40 then return end
		if not IsBetterEquipGet( this:GetClientUserData( 0 ) ) then return end
		if not AddTmpToList() then return end
		if not SetOneBetterEquip() then return end
		if not CompareWithWear() then return end
		BetterEquipFrame:Show();
	end
end

function SetMachineShortCutFrameBtn_OnUpdate()
	local dtime = GameMgr:getTickTime() - this:GetClientUserData( 0 );
	local pos = dtime * 0.05;
	if pos >= 100 then
		this:Hide();
		GameShortCut:setShortCut( 2, this:GetClientUserData( 1 ), MagicShortcut42:GetClientID() - 1, 0 );
	else
		this:SetPoint( "topleft","MagicShortcut42","topleft",pos-100,0 );
	end
end

local t_PworldMiss = { 211, 219, 101 }; --龙蛋谷,筑梦空间,盘山道
local t_OffExpType = {
			[1] = { "时间城，海滩", 150 },
			[2] = { "引力空间", 150 },
			[3] = { "完全离线", 100 }
			};
-- 这个的话，离线经验就只要显示在显示的时候设置位置，还有升级的时候
function RoleExpButtonOfflineExpBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local levelupDef = getLevelUP( mainplayer:getLv() );
	if levelupDef == nil then return end;

	local offExp = mainplayer:getOfflineExp();
	local maxOffExp = levelupDef.MaxOfflineExp;
	local szText = "养精蓄锐（积累杀怪加成经验）：\n\t\t#G" .. offExp .. "/" .. maxOffExp .. "（" .. math.floor(offExp * 100 / maxOffExp) .. "%）#n\n";

	for i=1, #t_OffExpType do
		szText = szText .. "#B\t\t" .. t_OffExpType[i][1] .."积累效率：" .. t_OffExpType[i][2] .. "%#n\n";
	end
	szText = szText .. "#Y积累的养精蓄锐经验在击杀怪物时，以3倍\n经验的方式返还#n\n";

	local pworldExp = mainplayer:getPworldExp();
	local maxPworldExp = levelupDef.MaxPworldOffExp;
	if maxPworldExp ~= 0 then
		szText = szText .. "\n活动补偿（未参与重要活动的经验）：\n\t\t#G" .. pworldExp .. "/" .. maxPworldExp .."（" .. math.floor(pworldExp * 100 / maxPworldExp) .. "%）#n\n";
		--特殊处理活动副本
		for i=1, #t_PworldMiss do
			local pworldDef = getPworldDef( t_PworldMiss[i] );
			if  t_PworldMiss[i] == 211 and mainplayer:getLv() > 65 then
				--特殊处理龙蛋谷
			elseif pworldDef.PworldID ~= 0 and mainplayer:getLv() >= pworldDef.MinLv and mainplayer:getLv() < pworldDef.MaxLv then
				szText = szText .. "#B\t\t" .. pworldDef.Name .. "可补偿：" .. math.floor( levelupDef.ContributeExp * pworldDef.OffExpMul / 10000 ) .. "#n\n";
			end
		end
		szText = szText .. "#Y积累的活动补偿经验在通关活动副本时，以\n6倍通关经验的方式返还#n\n";
	end
	
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = szText, 
								frame = this:GetParent(), button = "cursor" } );
end

--------------------------------------获得更好的装备面板----------------------------------------------
--装备位置对应索引表，用于确定显示顺序
--1：主手，2：头盔，3：衣，5：副手，6：面，7：肩，11：戒指，15：足，17：项链，10：护符
local t_EquipPosIdx = { [1]=1, [5]=2, [2]=3, [7]=4, [6]=5, [3]=6, [15]=7, [17]=8, [11]=9, [10]=10 }
--新获取的装备记录表 
local t_NewEquipList = { 	--主手
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--副手
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--头盔
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--肩甲	
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--面饰	
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--胸甲
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--鞋子
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--项链
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--戒子	
	{ ListType = 0, GridIdx = 0, itemID = 0, },
	--护符
	{ ListType = 0, GridIdx = 0, itemID = 0, },
}
local t_tmpItem = { ListType = 0, GridIdx = 0, itemID = 0, };
function CanBeAddToBetterEquip( itemDef )
	local mainplayer = ActorMgr:getMainPlayer();
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "时装" then
		return false;
	end
	if GetCareerName( mainplayer:getRace() ) == GetCareer( itemDef.WearPre.Career ) then
		return true;
	end
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "面饰" then
		return true;
	end
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "戒指" then
		return true;
	end
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "项链" then
		return true;
	end
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "护符" then
		return true;
	end
	return false;
end
function IsBetterEquipGet( itemId )
	if itemId == 0 then
		return false;
	end
	if t_tmpItem["itemID"] == itemId then
		return true;
	end
	return false;
end
function AddTmpToList()
	local itemDef = getItemDef( t_tmpItem["itemID"] );
	if itemDef.ItemID ~= 0 then
		local index = t_EquipPosIdx[itemDef.WearPos1]
		t_NewEquipList[index] = t_tmpItem;
		return true;
	end
	return false;
end
function AddToBetterEquip( itemId, listType, gridIdx )
	local itemDef = getItemDef( itemId );
	--这里特别区分下〉5000000
	if itemId >= 5000000 then return end
	if CanBeAddToBetterEquip( itemDef ) then
		if listType ~= CONTAINER_TYPE_SUNDRIES then return end
		if itemDef.WearPos1 == 0 then return end
		local index = t_EquipPosIdx[itemDef.WearPos1]
		if index == nil then return end
		t_tmpItem = { ListType = listType, GridIdx = gridIdx, itemID = itemId, }
		--[[
		BetterEquipFrame:SetClientUserData( 0, ListType );
		BetterEquipFrame:SetClientUserData( 1, GridIdx );
		if not BetterEquipFrame:IsShown() then
			BetterEquipFrame:Show();
		end
		--]]
	end
end
function SetOneBetterEquip()
	--设置一件要显示的装备
	for i = 1, table.getn( t_NewEquipList ) do
		if t_NewEquipList[i].ListType ~= 0 then
			BetterEquipFrame:SetClientUserData( 0, t_NewEquipList[i].ListType );
			BetterEquipFrame:SetClientUserData( 1, t_NewEquipList[i].GridIdx );
			BetterEquipFrame:SetClientUserData( 2, t_NewEquipList[i].itemID );
			t_NewEquipList[i].ListType = 0;
			t_NewEquipList[i].GridIdx = 0;
			t_NewEquipList[i].itemID = 0;
			return true;
		end
	end
	return false;
end
--与身上装备进行比较
function CompareWithWear()
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local Equip = MainPlayer:getEquip();
	local ListType = BetterEquipFrame:GetClientUserData( 0 );
	local GridIdx = BetterEquipFrame:GetClientUserData( 1 );
	local item = container:getItem( ListType, GridIdx );
	if item:getItemId() > 0 then
		local itemDef = item:getItemDef();
		-- 戒指和护符需要特别处理
		local nWearPos = itemDef.WearPos1;
		nWearPos = itemDef.WearPos1;
		if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "戒指" then
			nWearPos = GetEquipRingPos();
		end

		if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "护符" then
			nWearPos = GetEquipXianlianPos();
		end
		local wearEquip = Equip:getItem( nWearPos );
		if wearEquip:getItemId() <= 0 then
			return true;
		end
		local wearEquipItemdef = wearEquip:getItemDef();
		local itemAppriaise = GetEquipAppriaise( itemDef, item );
		local wearEquipAppriaise = GetEquipAppriaise( wearEquipItemdef, wearEquip );
		if wearEquipAppriaise < itemAppriaise then
			return true;
		end
	end
	return false;
end
function BetterEquipFrame_onShow()
	this:SetClientUserData( 4, os.clock() );
	this:SetPoint( "center", "UIClient", "center", 122, 249 );
	BetterEquipFrameDesc:SetText( "	穿上后可提高人物战斗属性！按【B】键打开背包查看！",255,255,0 )									
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local ListType = this:GetClientUserData( 0 );
	local GridIdx = this:GetClientUserData( 1 );
	local item = container:getItem( ListType, GridIdx );
	if item:getItemId() > 0 then
		local itemDef = item:getItemDef();
		if not CanBeAddToBetterEquip( itemDef ) or not CompareWithWear() then
			this:Hide();
			return
		end
		if itemDef.WearPre.RoleLevelMin > MainPlayer:getLv() then
			BetterEquipFrameEquipNow:Hide();
			BetterEquipFrameShowEquip:Show();
		else
			BetterEquipFrameEquipNow:Show();
			BetterEquipFrameShowEquip:Hide();
		end
		--任务套装特别处理
		if itemDef.CtrlFlag == 16 then
			BetterEquipFrameEquipNow:Show();
			BetterEquipFrameShowEquip:Hide();
		end
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		BetterEquipFrameItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, "BetterEquipFrameItemBtnUVAnimationTex");
	else
		this:Hide();
		--[[
		local itemDef = getItemDef( this:GetClientUserData( 2 ) );
		if not CanBeAddToBetterEquip( itemDef ) then return end
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		BetterEquipFrameItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, "BetterEquipFrameItemBtnUVAnimationTex");
		--]]
	end
end
function BetterEquipFrameItemBtn_onEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local ListType = BetterEquipFrame:GetClientUserData( 0 );
	local GridIdx = BetterEquipFrame:GetClientUserData( 1 );
	local item = container:getItem( ListType, GridIdx );
	if item:getItemId() == 0 then
		BetterEquipFrame:Hide();
	end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
end
function BetterEquipFrame_onUpdate()
	local showtime = this:GetClientUserData( 4 );
	local lastTime = os.clock() - showtime;
	if lastTime > 20 then
		this:Hide();
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local ListType = BetterEquipFrame:GetClientUserData( 0 );
	local GridIdx = BetterEquipFrame:GetClientUserData( 1 );
	local item = container:getItem( ListType, GridIdx );
	if item:getItemId() == 0 then
		this:Hide();
	end
end
function BetterEquipFrame_onHide()
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	--[[
	EffectToFrame:SetPoint( "center", "BetterEquipFrame", "center", 0, 0 );
	EffectToFrame:Show();
	EffectToFrame:SetClientString( "ContainerButton" );
	--]]
	if SetOneBetterEquip() then
		if not CompareWithWear() then return end
		this:Show();
	end
end
function BetterEquipFrameEquipNow_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local Equip = MainPlayer:getEquip();
	local ListType = BetterEquipFrame:GetClientUserData( 0 );
	local GridIdx = BetterEquipFrame:GetClientUserData( 1 );
	local itemDef = getItemDef( BetterEquipFrame:GetClientUserData( 2 ) );
	-- 戒指和护符需要特别处理
	local nWearPos = 0;
	nWearPos = itemDef.WearPos1;
	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "戒指" then
		nWearPos = GetEquipRingPos();
	end

	if GetItemTypeName( math.floor( itemDef.ItemID / 10000 ) ) == "护符" then
		nWearPos = GetEquipXianlianPos();
	end
	container:usedItem( ListType, GridIdx, nWearPos, 0 );
	BetterEquipFrame:Hide();
	--[[
	EffectToFrame:SetPoint( "center", "BetterEquipFrame", "center", 0, 0 );
	EffectToFrame:Show();
	EffectToFrame:SetClientString( "CharacterButton" );
	--]]
end
function BetterEquipFrameShowEquip_OnClick()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	BetterEquipFrame:Hide();
end
-----------------------------------------------link特效面板------------------------------------------------
function EffectToFrame_onShow()
	this:SetClientUserData( 0, os.clock() );
	EffectToFrame:SetPoint( "center", "UIClient", "center", 122, 249 );
	EffectToFrameFairyModel:setPetModel( 0,0,"effect\\entity\\huodezhuangbei.ent" )
	EffectToFrameFairyModel:setPetScale(0,5,5,5);
	EffectToFrameFairyModel:setEntityPosition(0,0,100,0);
end
function EffectToFrame_onUpdate()
	local Link = getglobal( this:GetClientString() );
	local lastTime = os.clock() - this:GetClientUserData( 0 );
	if lastTime >= 4 then
		this:Hide();
		return;
	end
	if lastTime > 1 then
		EffectToFrame:SetPoint( "center", Link:GetName(), "center", 0, 0 )
	end
end
---------------------------------------当前为测试版本 不代表最终品质 提示修改---------------------------

function TestWarnFrame_onShow()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end 
	local nLevel = mainplayer:getLv();
	if nLevel < 60 then 	
		TestWarnFrameTex1:Show();
		TestWarnFrameTex2:Hide();
		TestWarnFrameTex3:Hide();
	elseif nLevel > 59 and nLevel < 70 then
		TestWarnFrameTex1:Hide();
		TestWarnFrameTex2:Show();
		TestWarnFrameTex3:Hide();
	elseif nLevel > 69 then  	
		TestWarnFrameTex1:Hide();
		TestWarnFrameTex2:Hide();
		TestWarnFrameTex3:Show();
	end
end