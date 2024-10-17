local	MAX_SKILL_TABLE	=	10;
local	MAX_CHKBTN_NUM	=	10;
local	MAX_DEFALT_SKILL_NUM	= 2;

local   MAX_AUTO_SKILL_LIST_NUM = 33;
local	Catch_Monster_Option	= 10;
local	RESULT_MP_ADD		= 47;
local	RESULT_HP_ADD		= 46;
local	STATUS_RESULT_HPADD = 25;
local	STATUS_RESULT_MPADD = 26;

local 	VipStart			= false;
local   VipCountDown		= 0;
-- 挂机死亡复活回挂机点
local 	vipReturnFlag		= false;
-- 装备分解
local DecomposeEnd	= false;
local ShoppingEnd	= false;
local RESOLVE_MAX	= 8;

 t_btnAutoAssistSkillToResult = 
{
	{ SkillIndex = 1, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 2, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 3, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 4, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 5, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 6, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 7, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 8, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 9, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 10, SkillId = 0,	SkillLv = 0,},
	{ SkillIndex = 11, SkillId = 0,	SkillLv = 0,},
}; 
local t_itemPick  = {};
local t_skillList =
{
	 1010,1012,1014,1016,1018,1024,1028,1032,1034,1035,1120,1210,1219,1222,1026,1116,1212,
	 2010,2012,2014,2019,2020,2113,2114,2118,2119,2121,2210,2016,2218,
	 3010,3011,3014,3016,3020,3022,3034,3037,3211,3104,3218,
	 4000,4002,4027,4036,4043,4116,4123,4210,4211,4040,4042,4034,4006,4120,4038,4222,4044,9999,
};
local t_autoSkillList	=
{
	1010,1014,1016,1018,1032,1034,1035,1120,1210,1222,
	2010,2012,2014,2020,2113,2118,2121,2210,
	3010,3011,3016,3020,3022,3034,3037,3211,
	4000,4002,4027,4036,4043,4123,4210,
};
	
function  AutoAssistSkillReceiveDrag()
	local	srcList		=	UIMSG:getUIGV().DragFrom;
	local	desIndex	=	this:GetClientID()  ;
	local	Msg			=	UIMSG:getUIGV();
	local	hiddenBtn	=	getglobal("AutoAssistFrame_SkillBtn11");
	local	IconTexTem	=	getglobal("AutoAssistFrame_SkillBtn11Icon");
	if srcList ~= DRAG_AUTO_ASSIST and srcList ~= DRAG_SKILL and srcList ~= DRAG_SHORTCUT then
		ShowMidTips("非技能类无法放入！");
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end	
	if( UIMSG:getUIGV().DragFrom == DRAG_AUTO_ASSIST ) then
		local	srcIndex	=	UIGetDragUserData1();
		local	srcBtn		=	getglobal("AutoAssistFrame_SkillBtn"..srcIndex)
		local	tmpSpellId  ;
		local	tmpSpellLv  ;
		local	desIconPath;
		local	srcIconPath;
		tmpSpellId	=	t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
		tmpSpellLv	=	SkillMgr:getSpellLv(tmpSpellId);
		t_btnAutoAssistSkillToResult[desIndex]["SkillId"] = t_btnAutoAssistSkillToResult[srcIndex]["SkillId"];
		t_btnAutoAssistSkillToResult[desIndex]["SkillLv"] = t_btnAutoAssistSkillToResult[srcIndex]["SkillLv"];
		t_btnAutoAssistSkillToResult[srcIndex]["SkillId"] = tmpSpellId;
		t_btnAutoAssistSkillToResult[srcIndex]["SkillLv"] = tmpSpellLv;
		
		if t_btnAutoAssistSkillToResult[desIndex]["SkillId"] == 0 then
			desIconPath	=	"uires\\ui\\mask\\TouMingTongDao.tga";
		else
			local desSkillDef = SkillMgr:getSpellStaticAtt(t_btnAutoAssistSkillToResult[desIndex]["SkillId"], t_btnAutoAssistSkillToResult[desIndex]["SkillLv"]);
			desIconPath = GetSkillIconPath()..desSkillDef.IconID1..".tga";
		end
		if not IsInExistence( desIconPath ) and t_btnAutoAssistSkillToResult[desIndex]["SkillId"] ~= 0 then
			desIconPath = DEFAULT_SKILL_PATH;
		end
		local desIconTex  = getglobal(this:GetName().."Icon");
		desIconTex:SetTexture(desIconPath);
		
		if t_btnAutoAssistSkillToResult[srcIndex]["SkillId"] == 0 then
			srcIconPath	=	"uires\\ui\\mask\\TouMingTongDao.tga";
		else
			local srcSkillDef = SkillMgr:getSpellStaticAtt(tmpSpellId, tmpSpellLv);
			srcIconPath = GetSkillIconPath()..srcSkillDef.IconID1..".tga";
		end
		if not IsInExistence( srcIconPath ) and tmpSpellId ~= 0 then
			srcIconPath = DEFAULT_SKILL_PATH;
		end
		local srcIconTex  = getglobal("AutoAssistFrame_SkillBtn"..srcIndex.."Icon");
		srcIconTex:SetTexture(srcIconPath);
		
		srcBtn:SetClientUserData(0,t_btnAutoAssistSkillToResult[srcIndex]["SkillId"]);
		if t_btnAutoAssistSkillToResult[srcIndex]["SkillId"]	==	0	or srcIndex ~=11 then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		elseif  srcIndex == 11 and t_btnAutoAssistSkillToResult[desIndex]["SkillId"] ~= 0 then
			UIBeginDrag(srcIconTex:GetTexture() , hiddenBtn:GetClientID() , 0 , 0);
			UIMSG:getUIGV().DragFrom = DRAG_AUTO_ASSIST;
		end
		if  srcIndex ~= 11 then
			AutoAssist:getAutoAssistOpt().m_nSkillId[desIndex-1]	= t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
			AutoAssist:getAutoAssistOpt().m_nSkillId[srcIndex-1]	= t_btnAutoAssistSkillToResult[srcIndex]["SkillId"];
		else
			AutoAssist:getAutoAssistOpt().m_nSkillId[desIndex-1]	= t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
		end
		this:SwapCooldownMembers(srcBtn);
	end
	if ( UIMSG:getUIGV().DragFrom == DRAG_SKILL ) then
		t_btnAutoAssistSkillToResult[11]["SkillId"] = t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
		t_btnAutoAssistSkillToResult[11]["SkillLv"] = SkillMgr:getSpellLv( t_btnAutoAssistSkillToResult[11]["SkillId"] );
	    local SpellId  = UIGetDragUserData1()  ;
		local SpellLv  = SkillMgr:getSpellLv(SpellId);
		if not fitDragSkill( SpellId ) then
			ShowMidTips("该技能不适合辅助功能！");
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			--GameMgr:cancelNextSelect();
			return;
		end
		local IconTex  = getglobal(this:GetName().."Icon");
		t_btnAutoAssistSkillToResult[desIndex]["SkillId"] = SpellId;
		t_btnAutoAssistSkillToResult[desIndex]["SkillLv"] = SpellLv;
		local SkillDef	= SkillMgr:getSpellStaticAtt(SpellId, SpellLv);
		local IconPath	= GetSkillIconPath()..SkillDef.IconID1..".tga";
		
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_SKILL_PATH;
		end
		IconTex:SetTexture(IconPath);
		local cd = SkillMgr:getCDTime(UIGetDragUserData1());
		this:SetCooldownTimer(cd:past() / 1000, cd:total() / 1000, true);
		if(t_btnAutoAssistSkillToResult[11]["SkillId"]	~=	0) then
			local SkillDefTem	=	SkillMgr:getSpellStaticAtt(t_btnAutoAssistSkillToResult[11]["SkillId"], t_btnAutoAssistSkillToResult[11]["SkillLv"]);
			local IconPathTem	=	GetSkillIconPath()..SkillDefTem.IconID1..".tga";
			if not IsInExistence( IconPathTem ) then
				IconPathTem = DEFAULT_SKILL_PATH;
			end
			IconTexTem:SetTexture(IconPathTem);
			UIMSG:getUIGV().DragFrom = DRAG_AUTO_ASSIST;
			UIBeginDrag(IconTexTem:GetTexture() , hiddenBtn:GetClientID() , 0 , 0);
		else
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
		AutoAssist:getAutoAssistOpt().m_nSkillId[desIndex-1] = t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
	end
	if( UIMSG:getUIGV().DragFrom == DRAG_SHORTCUT ) then
		local	shortcutIndex	= UIGetDragUserData1();
		local	shortcutTemp	= getglobal("MagicShortcut"..shortcutIndex);
		local	m_nType			= shortcutTemp:GetClientUserData(0);
		local	m_Id			= shortcutTemp:GetClientUserData(1);
		local	level  			= SkillMgr:getSpellLv( m_Id );
		if( m_nType == SHORTCUT_SKILL or m_nType == SHORTCUT_FAIRYSKILL )then
			local srcSkillDef	= SkillMgr:getSpellStaticAtt(m_Id, level);
			local desIconTex	= getglobal(this:GetName().."Icon");
			local desIconPath	= GetSkillIconPath()..srcSkillDef.IconID1..".tga";
			if not fitDragSkill( m_Id ) then
				ShowMidTips("该技能不适合辅助功能！");
				UIEndDrag();
				UIMSG:getUIGV().DragFrom = DRAG_NONE;
				return;
			end
			t_btnAutoAssistSkillToResult[11]["SkillId"] = t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
			t_btnAutoAssistSkillToResult[11]["SkillLv"] = SkillMgr:getSpellLv( t_btnAutoAssistSkillToResult[11]["SkillId"] );
			t_btnAutoAssistSkillToResult[desIndex]["SkillId"] = m_Id;
			t_btnAutoAssistSkillToResult[desIndex]["SkillLv"] = level;
			if not IsInExistence( desIconPath ) then
				desIconPath = DEFAULT_SKILL_PATH;
			end
			desIconTex:SetTexture(desIconPath);
			local cd = SkillMgr:getCDTime(m_Id);
			this:SetCooldownTimer(cd:past() / 1000, cd:total() / 1000, true);
			if(t_btnAutoAssistSkillToResult[11]["SkillId"]	~=	0) then
				local SkillDefTem	=	SkillMgr:getSpellStaticAtt(t_btnAutoAssistSkillToResult[11]["SkillId"], t_btnAutoAssistSkillToResult[11]["SkillLv"]);
				local IconPathTem	=	GetSkillIconPath()..SkillDefTem.IconID1..".tga";
				if not IsInExistence( IconPathTem ) then
					IconPathTem = DEFAULT_SKILL_PATH;
				end
				IconTexTem:SetTexture(IconPathTem);
				UIMSG:getUIGV().DragFrom = DRAG_AUTO_ASSIST;
				UIBeginDrag(IconTexTem:GetTexture() , hiddenBtn:GetClientID() , 0 , 0);
			else
				UIEndDrag();
				UIMSG:getUIGV().DragFrom = DRAG_NONE;
			end
			AutoAssist:getAutoAssistOpt().m_nSkillId[desIndex-1] = t_btnAutoAssistSkillToResult[desIndex]["SkillId"];
		else
			ShowMidTips("非技能类无法放入！");
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			--GameMgr:cancelNextSelect();
		end
	end
	this:SetClientUserData(0, t_btnAutoAssistSkillToResult[desIndex]["SkillId"]);
	AutoAssist:saveAutoAssistOpt();
end

function AutoAssistSkillDrag()
	UIMSG:getUIGV().DragFrom = DRAG_AUTO_ASSIST;
	local IconTex = getglobal(this:GetName().."Icon");
	if t_btnAutoAssistSkillToResult[this:GetClientID()]["SkillId"] == 0 then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		UIBeginDrag(IconTex:GetTexture() ,this:GetClientID(), 0 ,0 );
		--UIBeginDrag(tex, skillId, id, index)
	end
end

function AutoAssist_RecoveryReceiveDrag()
	local srcList	 = UIMSG:getUIGV().DragFrom;
	local mainPlayer = ActorMgr:getMainPlayer();
	local itemDef;
	local itemId;
	local srcGrid;
	if mainPlayer == nil then return end

	local container	= mainPlayer:getContainer();
	if container == nil then return	end
	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= CONTAINER_TYPE_STUFF and srcList ~= DRAG_SHORTCUT then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end	
	if srcList == DRAG_SHORTCUT then
		local	shortcutIndex	= UIGetDragUserData1();
		local	shortcutTemp	= getglobal("MagicShortcut"..shortcutIndex);
		if shortcutTemp:GetClientUserData(0) == SHORTCUT_ITEM then
			itemDef	= getItemDef( shortcutTemp:GetClientUserData(1));
			itemId	= shortcutTemp:GetClientUserData(1)
		end
	else
		itemId  = UIGetDragUserData3();
		itemDef = getItemDef(UIGetDragUserData3());
	end
	if  this:GetName() =="AutoAssistFrame_HPMedicineBtn" then
		if isFood( itemDef,STATUS_RESULT_HPADD ,RESULT_HP_ADD ) then
			local IconTex 	= getglobal(this:GetName().."Icon");
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			local t_tgas	= { [this:GetName().."Icon"] = { path = szPath }, };
			util.SetTgas( t_tgas );
			local hpBtnTex	= getglobal("AutoAssistFrame_HPMedicineBtnIcon");
			this:SetClientUserData(0,itemId);	
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			AutoAssist_FoodCheck();
			AutoAssist:getAutoAssistOpt().m_hpId = itemId;
		else
			ShowMidTips(" 请放入恢复生命值的物品");
		end
	end
	if this:GetName() =="AutoAssistFrame_MPMedicineBtn" then
		if isFood( itemDef,STATUS_RESULT_MPADD,RESULT_MP_ADD ) then
			local IconTex 	= getglobal(this:GetName().."Icon");
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			local t_tgas	= { [this:GetName().."Icon"] = { path = szPath }, };
			util.SetTgas( t_tgas );
			local mpBtnTex	= getglobal("AutoAssistFrame_MPMedicineBtnIcon");
			this:SetClientUserData(0,itemId);	
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			AutoAssist_FoodCheck();
			AutoAssist:getAutoAssistOpt().m_mpId = itemId;
		else 
			ShowMidTips(" 请放入恢复魔法值的物品");
		end
	end

	if this:GetParent() == "AutoAssistFrame" then
		AutoAssist:saveAutoAssistOpt();	
	else
		AutoRecoveryFrameReceiveDrag( this:GetName(), itemDef );
	end
end

function AutoAssistFrame_LimitHPRecoveryBarValueChange()
	local sliderValue	= this:GetValue();
   	local  hpFont		= getglobal("AutoAssistFrame_LimitHPRecoveryBtnFont");
   	hpFont:SetText("生命值低于"..sliderValue.."%时使用食物");
   	AutoAssist:getAutoAssistOpt().m_hpSliderVal = sliderValue;
   	AutoAssist:saveAutoAssistOpt();	
end
function AutoAssistFrame_LimitMPRecoveryBarValueChange()
	local sliderValue   = this:GetValue();
   	local  hpFont = getglobal("AutoAssistFrame_LimitMPRecoveryBtnFont");
   	hpFont:SetText("魔法值低于"..sliderValue.."%时使用食物");
   	AutoAssist:getAutoAssistOpt().m_mpSliderVal = sliderValue;
   	AutoAssist:saveAutoAssistOpt();	
end
function AutoAssistFrameHeadButton_OnClick()
	local Btn = getglobal("AutoAssistFrame_ChkBtn"..this:GetClientID() )
	Btn:setCheckState( not Btn:GetCheckState() );
	AutoAssist:getAutoAssistOpt().m_bChkBtn[this:GetClientID() - 1] = Btn:GetCheckState();
	AutoAssist:saveAutoAssistOpt();
	--不攻击可捕捉宠物选项取消 恢复SubType
	if this:GetClientID() == Catch_Monster_Option and Btn:GetCheckState() == false then
		AutoAssist:ResumePetSubType();
	end
end

function AutoAssistFrameCheckButton_OnClick()
	local chkBtn = getglobal("AutoAssistFrame_ChkBtn"..this:GetClientID() );
	AutoAssist:getAutoAssistOpt().m_bChkBtn[this:GetClientID() - 1] = chkBtn:GetCheckState();
	AutoAssist:saveAutoAssistOpt();	
	--不攻击可捕捉宠物选项取消 恢复SubType
	if this:GetClientID() == Catch_Monster_Option and chkBtn:GetCheckState() == false then
		AutoAssist:ResumePetSubType();
	end
end

function AutoAssistFrame_PetRecovery_OnClick()
	if not AutoRecoveryFrame:IsShown() then
		AutoRecoveryFrame:Show();
	else
		AutoRecoveryFrame:Hide();
	end
end
local PKModeName = {
	[0] = "和平模式",
	"中立模式",
	"自由模式",
	"队伍模式",
	"公会模式",
	"阵营模式",
	"公会宣战模式",
}

function AutoAssistFrame_OnClick()
	local frame		= getglobal( "AutoAssistHintFrame" );
	local begnFont	= getglobal("AssistBeginBtnFont");
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end
	if not ShortCutFrame:IsShown() or mainPlayer:isInFly() then
		ShowMidTips("该状态不可以开启自动辅助！");
		return;
	end
	if mainPlayer:getLv() < 11 then
		ShowMidTips("人物达到11级后开启自动辅助功能！");
		return;
	end
   	if( begnFont:GetText() == "开始辅助" ) then
		local nCurMapID	= MapShower:getRealMapId();
		local pkMode	= GameActorPVP:getPkMode();
		if getAutoQusetState() then 
			if GameActorPVP:getPkMode() > 1 then
				GameActorPVP:changePKMode(1);
			end
		elseif nCurMapID < MIN_COPY_ID and ( pkMode == 2 or pkMode == 3 or pkMode == 4 ) then 
			local szText = "当前您的PK模式为#cff0000"..PKModeName[pkMode].."#n\n是否确认开启辅助功能"
			MessageBox( "提示",szText );
			MessageBoxFrame:SetClientString( "辅助模式切换" );
			return;
		end	
   		AutoAssistFunctionStart();
	else
   		AutoAssist:setAutoAssistBegin(false);
   		AutoAssist:setAssistMapID( 0 );
   		begnFont:SetText("开始辅助");
		VipStart = false;
   		AutoAssistHintFrame:Hide();
 		DecomposeEnd 	= false;
 		ShoppingEnd		= false;
 		vipReturnFlag	= false;
		VipAutoAssistFrameTimeFont:SetText("停止");
   	end
end

function AutoAssistFunctionStart()
	--[[
	if TeamFollowHintFrame:IsShown() or FollowHintFrame:IsShown() then
		ShowMidTips("跟随状态不可以开启自动辅助！");
		return;
	end  --]]
	local begnFont 	= getglobal("AssistBeginBtnFont");
	local frame		= getglobal( "AutoAssistHintFrame" );
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();	
	end
	if TeamFollowHintFrame:IsShown() then
		TeamManager:Team_OP_CancelTeamFollow();	
	end
	local petChkBtn = getglobal("AutoAssistFrame_ChkBtn9");
	if TargetPetChuZhengBtn:IsShown() and petChkBtn:GetCheckState() then
		TargetPetChuZhengBtn_OnClick();
	end
	AutoAssist:HandleAutoFind();
	t_itemPick = {};
	FirstUseAutoAssist();
	frame:SetPoint( "center", "UIClient", "center", 0, -120 );
	begnFont:SetText("取消辅助");
	AutoAssist:setAutoAssistBegin(true);
--  AutoAssist:setAssistMapID( mainPlayer:getMapId() );
  		
	VipCountDown = getServerTime().sec
	AutoAssistHintFrame:Show();	
	AutoAssist:saveNativePos();
end

function AutoAssistFrame_OnShow()
	local begnFont 	= getglobal("AssistBeginBtnFont");
	local texture 	= getglobal("AutoAssistFrame_VIPBtnTipTexture");
	if AutoAssist:getAutoAssistBegin() then
		begnFont:SetText("取消辅助");
	else
		begnFont:SetText("开始辅助");
	end
	--elmer 是否是火焰徽章
	if VipManager:isEnableVip( STAR_STONE_FIRE ) then
		texture:SetGray( false );
	else
		texture:SetGray( true );
	end
end

function AutoAssistFrameItem_OnEnter()
	local itemid = this:GetClientUserData(0)
	if itemid ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemid));
		return;
	end
end

function AutoAssistFrameSkill_OnEnter()
	local id = t_btnAutoAssistSkillToResult[this:GetClientID()]["SkillId"];
	if id ~= 0 then
		SpellTipsFrame:SetClientUserData(0, SkillMgr:getRuneSkillId(id))
		SpellTipsFrame:SetClientUserData(1, SkillMgr:getSpellLv(id))
		SpellTipsFrame:SetClientUserData(2, SKILL_TYPE)
		SpellTipsFrame:SetClientString(this:GetName())
		SpellTipsFrame:Show()
	end
end

function AutoAssistFrame_OnLeave()
	SpellTipsFrame:Hide();
	NormalItemTipsFrame:Hide();
end

local skillStep = 1;
local RepairMap = 0;
local RepairNpc = 0;
function AutoAssistHintFrame_OnUpdate()
	local vipData = AutoAssist:getVIPAutoAssistOpt();
	if ReviveFrame:IsShown() and VipManager:isEnableVip( STAR_STONE_FIRE ) and vipData.m_bVipChkBtn[VIP_ASSIST_OPTION_RETURN_POS]  then
		ReviveReleaseBtnOnClick();
		vipReturnFlag = true;
		VipStart = true;
		return;
	end
	
	if VipStart then
		VIPAutoAssistUpdate();
	else
		CommonAutoAssistUpdate();
	end
end
function CommonAutoAssistUpdate()	
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end
	if not ShortCutFrame:IsShown() or mainPlayer:isInFly() then
		local 	begnFont = getglobal("AssistBeginBtnFont");
		AutoAssist:setAutoAssistBegin(false);
   		begnFont:SetText("开始辅助");
   		AutoAssistHintFrame:Hide();
		return;
	end
	--elmer 火焰徽章玩家 vip辅助倒计时
	if VipManager:isEnableVip( STAR_STONE_FIRE ) and not getAutoQusetState() then
		VipAssistCountDown();
	end
	HpCheckUpdate();
	MpCheckUpdate();
	
	local pet			= mainPlayer:getPet();
	local targetId		= mainPlayer:GetTargetID();
	local targetActor	= ActorMgr:FindActor(targetId);
	
	local container	= mainPlayer:getContainer();
	if container == nil then return	end
	
	if mainPlayer:isDead() then
		AutoAssistHintFrame:Hide();
		AutoAssistFrame_OnClick();
	end
	if  table.getn(t_itemPick) ~= 0  and ItemPicker:PickItemsForAutoAssist(t_itemPick[1]) and  mainPlayer:isInStand() then 
		table.remove(t_itemPick,1);
	end
	if not AutoAssist:isInfixedRangeAssist() 
		and table.getn(t_itemPick) == 0 
		and mainPlayer:isInStand() 
		and ( targetActor == nil or targetActor:isDead() ) then
		
		table.insert(t_itemPick, -1 );
	end

	if table.getn(t_itemPick) == 0 
		 and  not AutoAssist:targetCanAttack( targetId ) 
		 and mainPlayer:isInStand() then
		GameShortCut:usedTab();
	end
	if ( not mainPlayer:isInPilState()  and mainPlayer:isInStand() and  not pet:isPre() ) then
		for i = skillStep , MAX_SKILL_TABLE do
		    if t_btnAutoAssistSkillToResult[i]["SkillId"] ~= 0  
		    	and SkillMgr:getCDTime(t_btnAutoAssistSkillToResult[i]["SkillId"]):remain() == 0 
		    	and table.getn(t_itemPick) == 0  then
		  		CanCapturePetTargetChange( t_btnAutoAssistSkillToResult[i]["SkillId"] , i );
			end
			if t_btnAutoAssistSkillToResult[i]["SkillId"] == 0 then
				skillStep = i - math.floor( i / MAX_SKILL_TABLE) * MAX_SKILL_TABLE + 1;
			end
			if skillStep == 10 then
				skillStep = 1;
			end
		end
	end	
end
function VipAssistCountDown()
	local vipData 		= AutoAssist:getVIPAutoAssistOpt();
	local mainPlayer 	= ActorMgr:getMainPlayer();
	local font 			= getglobal("VipAutoAssistFrameTimeFont");
	local curTime 		= getServerTime().sec;
	local minite		= vipData.m_nInterval;
	if not vipData.m_bVipChkBtn[VIP_ASSIST_OPTION_INTERVAL] then
		font:SetText("停止");
		return;
	end 
	local leftTime	= minite * 60 - ( math.floor( curTime - VipCountDown ) ) ;
	if leftTime < 60 and leftTime > 0 then
		font:SetText( "运行中，剩余时间"..leftTime.."秒");
	elseif leftTime >= 60 then
		local leftMin = math.floor( leftTime / 60 );
		font:SetText( "运行中，剩余时间"..leftMin.."分钟");
	else
		leftTime = 0;
		font:SetText( "运行中，剩余时间0秒" );
	end
	if vipReturnFlag then
		if leftTime <= 0 then 
			VipCountDown = curTime;
		end
		return;
	else 
		if leftTime <= 0 then 
			VipStart 		= true;
			DecomposeEnd 	= false;
	 		ShoppingEnd		= false;
			local mapDef 	= getMapDef( mainPlayer:getMapId() );
			RepairMap 		= mapDef.RepairMap;
			RepairNpc		= mapDef.RepairNpc; 
		end
	end	
end

function CanCapturePetTargetChange( skillID,i )
	local cheBtn = getglobal("AutoAssistFrame_ChkBtn10");
	local mainPlayer	= ActorMgr:getMainPlayer();
	local targetId		= mainPlayer:GetSelectTarget();
	local TargetObj		= ActorMgr:FindActor( targetId );
	if TargetObj == nil then return end 
	if cheBtn:GetCheckState() then
		AutoAssist:GetNearestCaptureMonster();
	end
	if cheBtn:GetCheckState() and TargetObj:getType() == GAT_MONSTER and TargetObj:getMonsterPhysicType() == 3 then
		GameShortCut:usedTab();
	else
		
		GameShortCut:useSkill( skillID );
		skillStep = i - math.floor( i / MAX_SKILL_TABLE ) * MAX_SKILL_TABLE + 1;
	end
end

function AutoAssistFrame_OnLoad()
	this:RegisterEvent("GE_SKILL_COOLD");
	this:RegisterEvent("GE_MAIN_PET_DEAD");	
	--this:RegisterEvent("GE_ENTER_PWORLD");
	--this:RegisterEvent("GE_REENTER_PWORLD");
	this:RegisterEvent("GE_BEGIN_AUTO_FIND");
	this:RegisterEvent("GE_CONTAINER_CHG");
	this:RegisterEvent("GE_MESSAGE_DEAD");
end

function AutoAssistFrame_OnEvent()
	if(arg1 == "GE_SKILL_COOLD") then
		local Msg = UIMSG:getUIMsg();
		for i = 1, MAX_SKILL_TABLE do
			local AutoAssistBtn = getglobal("AutoAssistFrame_SkillBtn"..i);
			if  t_btnAutoAssistSkillToResult[i]["SkillId"] ~= 0  and AutoAssistFrame:IsShown() then
				local skillCDTime	= SkillMgr:getCDTime( t_btnAutoAssistSkillToResult[i]["SkillId"] );
				local nCurtime		= GameMgr:getTickTime();
				AutoAssistBtn:SetCooldownTimer(skillCDTime:past() / 1000, skillCDTime:total() / 1000, true);
			elseif t_btnAutoAssistSkillToResult[i]["SkillId"] == 0 then
				AutoAssistBtn:SetCooldownTimer(0,0, true);
			end
		end
	end
	if arg1 == "GE_MAIN_PET_DEAD" then
		local	chkBtn	=	getglobal("AutoAssistFrame_ChkBtn9");
		if chkBtn:GetCheckState()	and	AutoAssist:getAutoAssistBegin() then
			TargetPetChuZhengBtn_OnClick();
		end
	end
	if arg1 == "GE_BEGIN_AUTO_FIND" then
		if AutoAssist:getAutoAssistBegin() and not VipStart then
			AutoAssistFrame_OnClick();
		end	
	end
	if arg1 == "GE_CONTAINER_CHG"  then
		local mainPlayer = ActorMgr:getMainPlayer();
		if mainPlayer == nil then return end
		local container	= mainPlayer:getContainer();
		if container == nil then  return end
		local itemHpId = AutoAssistFrame_HPMedicineBtn:GetClientUserData(0);
		local itemMpId = AutoAssistFrame_MPMedicineBtn:GetClientUserData(0); 
		if AutoAssistHintFrame:IsShown() and (container:getItemCount( itemHpId ) == 0 or container:getItemCount( itemMpId ) == 0 ) then
			AutoAssist_FoodCheck();
		end
	end
	--主角死亡 取消挂机
	if arg1 == "GE_MESSAGE_DEAD" then
		local nCurMapID	= MapShower:getRealMapId();
		local vipData = AutoAssist:getVIPAutoAssistOpt();
		if AutoAssist:getAutoAssistBegin() then
			if not ( VipManager:isEnableVip( STAR_STONE_FIRE ) and vipData.m_bVipChkBtn[VIP_ASSIST_OPTION_RETURN_POS] ) 
				or nCurMapID >= MIN_COPY_ID then
				AutoAssistFrame_OnClick();
			end
			--挂机死亡 分解界面关闭
			PlayerContainerResolveFrame:Hide();
 			PlayerContainerFrame:Hide();
		end
	end
end 
 --petPackage:isDead()  GAME_OPT_LAST_PET_SELECT
function HpCheckUpdate()
	local hpSlider		= getglobal("AutoAssistFrame_LimitHPRecoveryBar");
	local hpBtn			= getglobal("AutoAssistFrame_HPMedicineBtn");
	local hpBtnTex		= getglobal("AutoAssistFrame_HPMedicineBtnIcon");
	local sliderValue	= hpSlider:GetValue();
	local mainPlayer	= ActorMgr:getMainPlayer();
	local itemId		= hpBtn:GetClientUserData(0);
	if mainPlayer == nil then return end

	local container = mainPlayer:getContainer();
	if container == nil then return; end
	if container:getItemCount( itemId ) == 0 then
		hpBtnTex:SetGray( true );
		return;
	else
		hpBtnTex:SetGray( false );
	end
	local hp	= mainPlayer:getHP();
	local maxHp	= mainPlayer:getMaxHP();
	if(hp < sliderValue * maxHp /100) then
		container:usedItem(itemId);
		--container:usedItem( item:getListType(), item:getGridIdx(), 0, 0 );
	end
end

function MpCheckUpdate()
	local mpSlider		= getglobal("AutoAssistFrame_LimitMPRecoveryBar");
	local mpBtn		= getglobal("AutoAssistFrame_MPMedicineBtn");
	local mpBtnTex		= getglobal("AutoAssistFrame_MPMedicineBtnIcon");
	local sliderValue	= mpSlider:GetValue();
	local mainPlayer	= ActorMgr:getMainPlayer();
	local itemId		= mpBtn:GetClientUserData(0);
	if mainPlayer == nil then return end
	
	local container	= mainPlayer:getContainer();
	if container == nil then return; end
	if container:getItemCount( itemId ) == 0 then
		mpBtnTex:SetGray( true );
		return;
	else
		mpBtnTex:SetGray( false );
	end
	local mp	= mainPlayer:getMP();
	local maxMp	= mainPlayer:getMaxMP();
	if(mp < sliderValue * maxMp /100) then
		container:usedItem(itemId);
		--container:usedItem( item:getListType(), item:getGridIdx(), 0, 0 );
	end
end

function AutoAssistPick( id ,itemsID)
	local mainPlayer	= ActorMgr:getMainPlayer();
	local chkBtnFood	= getglobal("AutoAssistFrame_ChkBtn5");
	local chkBtnTask	= getglobal("AutoAssistFrame_ChkBtn6");
	local chkBtnOther	= getglobal("AutoAssistFrame_ChkBtn7");
	local itemDef		= getItemDef( id);
	local ammChkBtn		= getglobal("AutoAssistFrame_ChkBtn"..itemDef.Important+1);
	if mainPlayer == nil then return end

	local container	= mainPlayer:getContainer();
	if container == nil then return; end
	--[[	--背包满状态
	if not CanSaveToLeavePile( container, CONTAINER_TYPE_SUNDRIES, id, 1,false ) then
		return;
	end
	--]]
	if  isFood( itemDef,STATUS_RESULT_HPADD ,RESULT_HP_ADD ) or isFood( itemDef,STATUS_RESULT_MPADD ,RESULT_MP_ADD ) then
		if chkBtnFood:GetCheckState() then
			table.insert(t_itemPick, itemsID);
		end
		return;
	elseif getItemType( id ) == ITEM_SLOTTYPE_AMM then
		if ammChkBtn:GetCheckState()  then
			table.insert(t_itemPick,itemsID);
		end
		return;
	elseif id >= 5300000 and id <= 5310000 then
		if chkBtnTask:GetCheckState() then
			table.insert(t_itemPick, itemsID);
		end 
		return;
	elseif chkBtnOther:GetCheckState() then
		table.insert( t_itemPick, itemsID );
	end						
end

function isFood( itemDef,itemStatusType,itemType )
	for i=1,MAX_ITEM_RESULT do
		if(itemDef.Result[i-1].ResultID == RESULT_SELF_STATUS) then
			local statusDef = getStatusDef( itemDef.Result[i-1].ResultVal2, itemDef.Result[i-1].ResultVal3 );
			if statusDef ~= nil then
				for j=1,MAX_ITEM_RESULT do
					if itemStatusType ~= 0 and statusDef.Result[j-1].ResultID == itemStatusType then
						return true;
					end
				end
			end
		end
		if itemType ~= 0 and itemDef.Result[i-1].ResultID == itemType then
			return true;
		end
	end
	return false;                                                                                                                                                                                                                                                                                                                                                                                                                       
end

function AutoAssistLoadSevData()
	local	structData	=	AutoAssist:getAutoAssistOpt();
	for i=1 ,MAX_SKILL_TABLE do
		local	skillBtnTex	= getglobal("AutoAssistFrame_SkillBtn"..i.."Icon");
		local	m_Id		= AutoAssist:getAutoAssistOpt().m_nSkillId[i-1];
		local	level  		= SkillMgr:getSpellLv( m_Id );
		t_btnAutoAssistSkillToResult[i]["SkillId"]	=	m_Id;
		t_btnAutoAssistSkillToResult[i]["SkillLv"]	=	level;
		if m_Id ~= 0  then
			local	skillDef	= SkillMgr:getSpellStaticAtt(m_Id, level);
			local	srcIconPath = GetSkillIconPath()..skillDef.IconID1..".tga";
			skillBtnTex:SetTexture(srcIconPath);
		else
			local	srcIconPath	= "uires\\ui\\mask\\TouMingTongDao.tga";
			skillBtnTex:SetTexture(srcIconPath);
		end	
	end
	local	hpSlider = getglobal("AutoAssistFrame_LimitHPRecoveryBar");
	hpSlider:SetValue( structData.m_hpSliderVal );
	local	mpSlider = getglobal("AutoAssistFrame_LimitMPRecoveryBar");
	mpSlider:SetValue( structData.m_mpSliderVal );
	local	mpBtn	= getglobal("AutoAssistFrame_MPMedicineBtn");
	local	hpBtn	= getglobal("AutoAssistFrame_HPMedicineBtn");
	if	structData.m_hpId ~= 0 then
		hpBtn:SetClientUserData(0,structData.m_hpId);
		local	hpItemDef	= getItemDef(structData.m_hpId);
		local	szPath		= GetItemEquipIconPath()..hpItemDef.IconID..".tga";
		local	t_hpgas		= { ["AutoAssistFrame_HPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_hpgas );
	else
		hpBtn:SetClientUserData(0,0);
		local	szPath		= "uires\\ui\\mask\\TouMingTongDao.tga";
		local	t_hpgas		= { ["AutoAssistFrame_HPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_hpgas );
	end
	if structData.m_mpId ~= 0 then
		mpBtn:SetClientUserData(0,structData.m_mpId);
		local	mpItemDef	= getItemDef(structData.m_mpId);
		local	mpIconTex 	= getglobal("AutoAssistFrame_MPMedicineBtnIcon");
		local	szPath 		= GetItemEquipIconPath()..mpItemDef.IconID..".tga";
		mpIconTex:SetTexture(szPath);
	else
		mpBtn:SetClientUserData(0,0);
		local	szPath		= "uires\\ui\\mask\\TouMingTongDao.tga";
		local	t_hpgas		= { ["AutoAssistFrame_MPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_hpgas );
	end
	for i=1, MAX_CHKBTN_NUM do
		local chkBtn = getglobal("AutoAssistFrame_ChkBtn"..i);
		chkBtn:setCheckState(structData.m_bChkBtn[i-1]);
	end
	SetVipAssistOption();
	AutoAssist:saveVIPAssistOpt();
end
		
function AutoAssist_FoodCheck()
	local	mainPlayer	= ActorMgr:getMainPlayer();
	local	hpBtn		= getglobal("AutoAssistFrame_HPMedicineBtn");
	local	hpBtnTex	= getglobal("AutoAssistFrame_HPMedicineBtnIcon");
	local	mpBtn		= getglobal("AutoAssistFrame_MPMedicineBtn");
	local	mpBtnTex	= getglobal("AutoAssistFrame_MPMedicineBtnIcon");
	local	itemHpId	= hpBtn:GetClientUserData(0);
	local	itemMpId	= mpBtn:GetClientUserData(0);
	if mainPlayer == nil then return end

	local container		=	mainPlayer:getContainer();
	if container == nil then  return end
	if container:getItemCount( itemHpId ) ~= 0 then
		hpBtnTex:SetGray( false );
	else
		hpBtnTex:SetGray( true );
		SetHpFoodItemClassification();
	end
	if container:getItemCount( itemMpId ) ~= 0 then
		mpBtnTex:SetGray( false );
	else
		mpBtnTex:SetGray( true );
		SetMpFoodItemClassification();
	end	
end

function setFoodForAutoAssist( hpID,mpID)
	local	hpBtn		= getglobal("AutoAssistFrame_HPMedicineBtn");
	local	hpBtnTex	= getglobal("AutoAssistFrame_HPMedicineBtnIcon");
	local	mpBtn		= getglobal("AutoAssistFrame_MPMedicineBtn");
	local	mpBtnTex	= getglobal("AutoAssistFrame_MPMedicineBtnIcon");
	local	itemDef;
	if	hpID ~= 0 then
		itemDef	=	getItemDef( hpID );
		local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
		local t_tgas	= { ["AutoAssistFrame_HPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_tgas );
		hpBtn:SetClientUserData(0,hpID);	
		hpBtnTex:SetGray( false );
	else
		local szPath	= "uires\\ui\\mask\\TouMingTongDao.tga";
		local t_tgas	= { ["AutoAssistFrame_HPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_tgas );
		hpBtn:SetClientUserData(0,0);
	end
	if	mpID ~= 0 then
		itemDef	=	getItemDef( mpID );
		local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
		local t_tgas	= { ["AutoAssistFrame_MPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_tgas );
		mpBtn:SetClientUserData(0,mpID);	
		mpBtnTex:SetGray( false );
	else
		local szPath	= "uires\\ui\\mask\\TouMingTongDao.tga";
		local t_tgas	= { ["AutoAssistFrame_MPMedicineBtnIcon"] = { path = szPath }, };
		util.SetTgas( t_tgas );
		mpBtn:SetClientUserData(0,0);
	end
	AutoAssist:getAutoAssistOpt().m_mpId = mpID;
	AutoAssist:getAutoAssistOpt().m_hpId = hpID;
	AutoAssist:saveAutoAssistOpt();
	
end
function UpdateSkillAutoAssist(skillId)
	local skillFlag	= 1;
	for i = 1,MAX_AUTO_SKILL_LIST_NUM do
		if(skillId == t_autoSkillList[i]) then
			break;
		else
			skillFlag = skillFlag + 1;
		end
	end
	if skillFlag <= MAX_AUTO_SKILL_LIST_NUM then
		for i = 1,MAX_SKILL_TABLE do
			local	skillBtn	= getglobal("AutoAssistFrame_SkillBtn"..i);
			local	skillIcon_btn	= getglobal("AutoAssistFrame_SkillBtn"..i.."Icon")
			local	skillDef	= SkillMgr:getSpellStaticAtt(skillId, SkillMgr:getSpellLv(skillId))
			local	IconPath	= GetSkillIconPath()..skillDef.IconID1..".tga"
			if t_btnAutoAssistSkillToResult[i]["SkillId"] == 0 then
				t_btnAutoAssistSkillToResult[i]["SkillId"] = skillId;
				skillIcon_btn:SetTexture(IconPath);
				skillBtn:SetClientUserData(0,skillId);
				AutoAssist:getAutoAssistOpt().m_nSkillId[i-1] =	skillId;
				break;
			end
		end
	end
	AutoAssist:saveAutoAssistOpt();
end

function fitDragSkill( skillId )
	local runeId	= SkillMgr:getRuneSkillId( skillId );
	local skillDef	= SkillMgr:getSpellStaticAtt( runeId, SkillMgr:getSpellLv(runeId));
	if skillDef.Auto == 0 then
		return false;
	end
	return true;
end

function CheckSkillForAutoAssist( nSkillID )
	for i=1,MAX_SKILL_TABLE do
		if nSkillID ~= 0 and t_btnAutoAssistSkillToResult[i]["SkillId"] == nSkillID then
			local runeId	= SkillMgr:getRuneSkillId( nSkillID );
			local skillDef	= SkillMgr:getSpellStaticAtt( runeId, SkillMgr:getSpellLv(runeId));
			if skillDef.Auto == 0 then
				ClearNoFitSkillInfo( i );
				ShowMidTips("该技能不适合辅助功能");
				break;
			end
		end
	end
end

function ClearNoFitSkillInfo( index )
	local skillButton	= getglobal( "AutoAssistFrame_SkillBtn"..index );
	local skillBtnTex	= getglobal("AutoAssistFrame_SkillBtn"..index.."Icon");
	AutoAssist:getAutoAssistOpt().m_nSkillId[index-1]	= 0;
	t_btnAutoAssistSkillToResult[index]["SkillId"]		= 0;
	t_btnAutoAssistSkillToResult[index]["SkillLv"]		= 0;
	local srcIconPath =	"uires\\ui\\mask\\TouMingTongDao.tga";
	skillBtnTex:SetTexture( srcIconPath );
	skillButton:SetClientUserData( 0, 0 );
	AutoAssist:saveAutoAssistOpt();
end

function FirstUseAutoAssist()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_IN_AUTO_ASSIST) then
		local hpID	= 0;
		local mpID	= 0;
		local hplevel 	= 0;
		local mplevel 	= 0;
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then	return 	end
		local container = mainplayer:getContainer();
		for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item 	=	container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
			local itemDef	= item:getItemDef();
			if itemDef ~= nil and itemDef.WearPre.RoleLevelMin <= mainplayer:getLv() then
				if isFood(itemDef,STATUS_RESULT_HPADD ,0) and itemDef.WearPre.RoleLevelMin > hplevel then
					hplevel	= itemDef.WearPre.RoleLevelMin;
					hpID	= itemDef.ItemID ;
				elseif isFood(itemDef,STATUS_RESULT_MPADD,0) and itemDef.WearPre.RoleLevelMin > mplevel then
					mplevel	= itemDef.WearPre.RoleLevelMin;
					mpID	= itemDef.ItemID ;
				end
			end
		end
		--第一次打开面板，食物设置
		setFoodForAutoAssist(hpID,mpID);
		GameWizard:doStep(GAMEWIZARD_FIRST_IN_AUTO_ASSIST);
	end
end

function SetHpFoodItemClassification()
	local btn	= getglobal("AutoAssistFrame_HPMedicineBtn")
	local HpId	= btn:GetClientUserData(0);
	if HpId ~= 0 then
		local hpDef = getItemDef( HpId );
		if isFood( hpDef,STATUS_RESULT_HPADD,0) and isFood( hpDef,STATUS_RESULT_MPADD,0) then
			GetOptimalFoodItem( STATUS_RESULT_HPADD,STATUS_RESULT_MPADD,0,0,0,2);
			return;
		elseif isFood( hpDef,0,RESULT_HP_ADD) and isFood( hpDef,0,RESULT_MP_ADD) then
			GetOptimalFoodItem( 0,0,RESULT_HP_ADD,RESULT_MP_ADD,0,2);
			return;
		end
		if isFood( hpDef,STATUS_RESULT_HPADD,0) then
			GetOptimalFoodItem( STATUS_RESULT_HPADD,STATUS_RESULT_MPADD,0,0,0,1);
		end
		if isFood( hpDef,0,RESULT_HP_ADD) then
			GetOptimalFoodItem( 0,0,RESULT_HP_ADD,RESULT_MP_ADD,0,1);
		end
	end
end
function SetMpFoodItemClassification()
	local btn	= getglobal("AutoAssistFrame_MPMedicineBtn")
	local MpId	= btn:GetClientUserData(0);
	if MpId ~= 0 then
		local mpDef = getItemDef( MpId );
		if isFood( mpDef,STATUS_RESULT_HPADD,0) and isFood( mpDef,STATUS_RESULT_MPADD,0) then
			GetOptimalFoodItem( STATUS_RESULT_HPADD,STATUS_RESULT_MPADD,0,0,1,2);
			return;
		elseif isFood( mpDef,0,RESULT_HP_ADD) and isFood( mpDef,0,RESULT_MP_ADD) then
			GetOptimalFoodItem( 0,0,RESULT_HP_ADD,RESULT_MP_ADD,1,2);
			return;
		end
		if isFood( mpDef,STATUS_RESULT_MPADD,0) then
			GetOptimalFoodItem( STATUS_RESULT_MPADD,STATUS_RESULT_HPADD,0,0,1,1);
			return;
		end

		if isFood( mpDef,0,RESULT_MP_ADD) then
			GetOptimalFoodItem( 0,0,RESULT_MP_ADD,RESULT_HP_ADD,1,1);
			return;
		end
	end
end
function GetOptimalFoodItem( buffType1,buffType2 ,statusType1,statusType2,HMPType,itemFlag )
	local medicineID	= 0;
	local medicinelv 	= 0;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 	return 	end
	local container = mainplayer:getContainer();
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item 	=	container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
		local itemDef	=	item:getItemDef();
		if itemDef ~= nil and itemDef.WearPre.RoleLevelMin <= mainplayer:getLv() then
			if itemFlag == 2 
				and isFood(itemDef,buffType1,statusType1) 
				and isFood(itemDef,buffType2,statusType2)
				and itemDef.WearPre.RoleLevelMin > medicinelv then
					
				medicinelv	= itemDef.WearPre.RoleLevelMin;
				medicineID	= itemDef.ItemID ;
			
			elseif itemFlag == 1
			 		and isFood(itemDef,buffType1,statusType1) 
			 		and	not isFood(itemDef,buffType2,statusType2)
			 		and itemDef.WearPre.RoleLevelMin > medicinelv then

				medicinelv	= itemDef.WearPre.RoleLevelMin;
				medicineID	= itemDef.ItemID ;
			end
		end
	end
	AutoAddMedicineFood(medicineID,HMPType);
end

function AutoAddMedicineFood( medicineID , HMPType)
	if HMPType == 0 then
		local	hpBtn		= getglobal("AutoAssistFrame_HPMedicineBtn");
		local	hpBtnTex	= getglobal("AutoAssistFrame_HPMedicineBtnIcon");
		if	medicineID ~= 0 then
			local itemDef	= getItemDef( medicineID );
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			local t_tgas	= { ["AutoAssistFrame_HPMedicineBtnIcon"] = { path = szPath }, };
			util.SetTgas( t_tgas );
			hpBtn:SetClientUserData(0,medicineID);	
			hpBtnTex:SetGray( false );
			AutoAssist:getAutoAssistOpt().m_hpId = medicineID;
			AutoAssist:saveAutoAssistOpt();
		end
	else
		local	mpBtn		= getglobal("AutoAssistFrame_MPMedicineBtn");
		local	mpBtnTex	= getglobal("AutoAssistFrame_MPMedicineBtnIcon");
		if	medicineID ~= 0 then
			local itemDef	= getItemDef( medicineID );
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			local t_tgas	= { ["AutoAssistFrame_MPMedicineBtnIcon"] = { path = szPath }, };
			util.SetTgas( t_tgas );
			mpBtn:SetClientUserData(0,medicineID);	
			mpBtnTex:SetGray( false );
			AutoAssist:getAutoAssistOpt().m_mpId = medicineID;
			AutoAssist:saveAutoAssistOpt();
		end
	end
end

---------------------------------------------- VIP高级辅助------------------------------------------
local VIP_FOOD_TYPE_HP = 1;
local VIP_FOOD_TYPE_MP = 2;
--回血、回蓝各五种食物
 t_VIPAssistFood = 
{
	[VIP_FOOD_TYPE_HP] = { 5010000, 5010001, 5010002, 5010003, 5010004 },
	[VIP_FOOD_TYPE_MP] = { 5011000, 5011001, 5011002, 5011003, 5011004 },
}
function SetVipAssistOption()
	local vipData = AutoAssist:getVIPAutoAssistOpt();
	--勾选项设置
	for i = 1,  VIP_ASSIST_OPTION_END do
		local chkBtn = getglobal( "VipAutoAssistFrame_ChkBtn"..i );
		chkBtn:setCheckState( vipData.m_bVipChkBtn[i-1] );
	end
	--间隔时间
	VipAutoAssistFrameTimesEdit:SetText( vipData.m_nInterval );
	--食物选择
	local HpFont 	= getglobal("VipAutoAssistHpChooseBtnFont");
	local MpFont 	= getglobal("VipAutoAssistMpChooseBtnFont");
	local HpBtn 	= getglobal("VipAutoAssistHpChooseBtn");
	local MpBtn 	= getglobal("VipAutoAssistMpChooseBtn");
	local HpIndex	= vipData.m_nHpIndex > 0 and vipData.m_nHpIndex or 1;
	local MpIndex	= vipData.m_nMpIndex > 0 and vipData.m_nMpIndex or 1;
	local HpItemDef	= getItemDef( t_VIPAssistFood[VIP_FOOD_TYPE_HP][HpIndex] );
	local MpItemDef	= getItemDef( t_VIPAssistFood[VIP_FOOD_TYPE_MP][MpIndex] );
	HpFont:SetText( HpItemDef.Name );
	MpFont:SetText( MpItemDef.Name );
	HpBtn:SetClientUserData( 0, HpIndex);
	MpBtn:SetClientUserData( 0, MpIndex);

	--食物数量设置
	VipAutoAssistFrameHpNumEdit:SetText( vipData.m_nHPNum );
	VipAutoAssistFrameMpNumEdit:SetText( vipData.m_nMPNum );
end

function AutoAssistFrame_VIPBtn_OnClick()
	if VipAutoAssistFrame:IsShown() then
		VipAutoAssistFrame:Hide();
	else
		VipAutoAssistFrame:Show();
	end
end

function VIPAutoAssistFrame_OnShow()
	if AutoAssistFrame:IsShown() then
		this:SetPointAlwaysRecalc( "bottomleft", "AutoAssistFrame", "bottomright", 0, 0 );
	else
		this:SetPoint( "center", "UIClient", "center", 0, 0 );
	end
	FirstUseVipAutoAssist();
	SetVipAssistOption();
	local font = getglobal("VipAutoAssistFrameTimeFont");
	if not AutoAssist:getAutoAssistBegin() or not VipManager:isEnableVip( STAR_STONE_FIRE ) then
		font:SetText( "停止");
	end
	VipAutoAssistFrameRich:SetText("分钟进行#cff0000(不支持危险地宫)#n:", 255, 255, 190 );	
end
function FirstUseVipAutoAssist()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_IN_VIP_ASSIST) then
		AutoAssist:resetVipAssist();
		AutoAssist:saveVIPAssistOpt();
		GameWizard:doStep(GAMEWIZARD_FIRST_IN_VIP_ASSIST);
	end
end
local EnterMap = false;
function VIPAutoAssistFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_MAP");
	this:RegisterEvent("GE_VIP_INFO_UPDATE");
end

function VIPAutoAssistFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		EnterMap = true;
	end
	if arg1 == "GE_VIP_INFO_UPDATE" then
		local texture 	= getglobal("AutoAssistFrame_VIPBtnTipTexture");
		--elmer 是否是火焰徽章
		if VipManager:isEnableVip( STAR_STONE_FIRE ) then
			texture:SetGray( false );
		else
			texture:SetGray( true );
		end
	end 
end

---处理卡的状况
local ReMoveFlag 	= false;
local MarkFlag		= true;
local FlagTime		= 0;
----
local t_VIPCloseFrame = { "EnchantFrame", "BaoShiXiangQianFrame", "IdentifyFrame","ArmAttrMoveFrame","ArmRecastFrame","ArmWashFrame","EquipFrame"}
function VIPAutoAssistUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	
	if vipReturnFlag and not getAutoQusetState() then
		VipAssistCountDown();
	end
	if not NeedGotoShopNPC() then
		ShoppingEnd = true;
	end 
	HpCheckUpdate();
	MpCheckUpdate();
	-- 进入地图时 做一个停止操作 重新寻路
   	if EnterMap then 
   		local multiMapFind = MapShower:getMultiMapFindControl();
		multiMapFind:stopMultiFind();
		EnterMap = false;
		return;
	end                                                                                                                     
	if mainplayer:isInStand() then
		local vipData = AutoAssist:getVIPAutoAssistOpt();
		for _, name in pairs( t_VIPCloseFrame ) do
			local frame = getglobal( name );
			if frame:IsShown() then
				frame:Hide();
			end
		end
		-- 挂机死亡 回挂机点
		if vipReturnFlag then
			local curTime = getServerTime().sec;
			local minite = vipData.m_nInterval;
	 		if AutoAssist:ReturnAssistPosition() then 
				vipReturnFlag = false;
			else 
				return;
			end
					
			local leftTime	= minite * 60 - ( math.floor( curTime - VipCountDown ) ) ;
			if leftTime <= 0 then 
				VipStart = true;
			else 
				VipStart = false;
			end
			return;
		end
		if not vipData.m_bVipChkBtn[VIP_ASSIST_OPTION_INTERVAL] then
			VipAutoAssistFrameTimeFont:SetText("停止");
			VipCountDown = getServerTime().sec;
			VipStart = false;
			return;
		end 
		-- 买卖物品完毕 返回挂机点
		if ShoppingEnd  and AutoAssist:ReturnAssistPosition() then
			-- 正在分解装备
			if not DecomposeEnd then
				DecomposeContainerEquip()
				return;	
			end	
		end
		-- 分解完毕 切换普通辅助
		if DecomposeEnd then
			VipCountDown = getServerTime().sec;
			VipStart = false;
			return;
		end
--		NpcTalkTooLongDisposal();
		if 	not ShoppingEnd then
			-- 打开商店
			if IsTalkToNpc() then
				return;
			end	
			-- 买卖物品、修装备
			if IsTradingOrRepair() then
				return;
			end
		end
	end
end

function NeedGotoShopNPC()
	local vipData = AutoAssist:getVIPAutoAssistOpt();
	if VipManager:isOwnVipFunc( STAR_RESULT_SHOP_TOLUA ) then 
		return true;
	end
	if RepairNpc <= 0 then 
		return false;
	end
	for i=1,6 do
		if vipData.m_bVipChkBtn[i] then
			return true;
		end
	end
	return false;
end
--与NPC对话 丢包现象处理
function NpcTalkTooLongDisposal()
	local actorID 	= ActorMgr:getActorIdByResId( RepairNpc );
	local pActor	= ActorMgr:FindActor( actorID );
	if pActor ~= nil then
		local curTime 	= getServerTime().sec ;
		local nDist 	= util.CalActorsDistance( pActor, ActorMgr:getMainPlayer() );

		if nDist < 5 and ( curTime - FlagTime ) > 5 then
			NpcDialog:send_NpcTalk(actorID) ;
			FlagTime = curTime;
		end 
	end
	--[[
	if ReMoveFlag then
		local curTime = getServerTime().sec ;
		if ( curTime - FlagTime ) > 6 then
			NpcDialog:send_NpcTalk(actorID) ;
			MarkFlag	= true;
			ReMoveFlag  = false;
		end 
	end
	--]]
end

function IsTalkToNpc()
	--目标id是npcid  这里要加上的
	if NpcTalkFrame:IsShown() then
		local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
		local allOpts 				= t_NpcTalkFrameControl:getAllOpts();

		for i=1,#allOpts do
			if string.find( allOpts[i]["oneOptContent"],"打开商店",1,true ) then
				NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
				return true;
			end
		end
	end
	return false;
end
local LinkTime = 0;
function IsTradingOrRepair()
	if ShopFrame:IsShown() then
		local vipData = AutoAssist:getVIPAutoAssistOpt();
		--第一步 买东西
		VipAutoAssistBuyShopItem( vipData );
		--第二步 修理装备、卖东西
		RepairEquipAndSaleItem( vipData );
		ShopFrame:Hide();
		ShoppingEnd = true;
		return true;	
	end
	local actorID 	= ActorMgr:getActorIdByResId( RepairNpc );
	local pActor	= ActorMgr:FindActor( actorID );
	local curTime 	= getServerTime().sec ;
	if VipManager:isOwnVipFunc( STAR_RESULT_SHOP_TOLUA ) then 
		ContainerFrame_VipShopBtn_OnClick();
	else
		if pActor ~= nil then	
			local nDist 	= util.CalActorsDistance( pActor, ActorMgr:getMainPlayer() );
			if nDist < 5 and ( curTime - FlagTime ) > 5  and NpcDialog:getTalkNpcMemID() == actorID then
				NpcDialog:send_CloseTalk();
		--		MapShower:GetMaxMapContal():linkMoveto( RepairMap, RepairNpc );
				FlagTime = curTime;
				return false;
			end 
			if nDist > ( NpcDialog:getMaxNpcTalkDist( pActor ) / 100 ) ^ 2  and NpcDialog:getTalkNpcMemID() == actorID then
				NpcDialog:send_CloseTalk();
				LinkTime = curTime;
				return false;
			end
		end
		if ( curTime - LinkTime ) > 2 then
			MapShower:GetMaxMapContal():linkMoveto( RepairMap, RepairNpc );
			LinkTime = curTime;
		end
	end
	return false;
	-- 拥有 随身商店	
	--[[
	if VipManager:isOwnVipFunc( STAR_RESULT_SHOP_TOLUA ) then 
		ContainerFrame_VipShopBtn_OnClick();
	else
		if pActor ~= nil then
			local nDist = util.CalActorsDistance( pActor, ActorMgr:getMainPlayer() );
			if nDist > ( NpcDialog:getMaxNpcTalkDist( pActor ) / 100 ) ^ 2  and NpcDialog:getTalkNpcMemID() == actorID then
				NpcDialog:send_CloseTalk();
				return false;
			elseif nDist > ( NpcDialog:getMaxNpcTalkDist( pActor ) / 100 ) ^ 2 then
				MapShower:GetMaxMapContal():linkMoveto( RepairMap, RepairNpc );
				return false;
			end
		else
			MapShower:GetMaxMapContal():linkMoveto( RepairMap, RepairNpc );
			return false;
		end
	end
	--]]
end
function VipAutoAssistBuyShopItem( VipData )
	local MainPlayer 	= ActorMgr:getMainPlayer();
	local container	 	= MainPlayer:getContainer();
	local lockState 	= container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("背包锁已锁无法购买，请先验证背包安全锁");
		return;
	end
	local ShopList		= NpcShop:getShopList();
	local nShopItemNum	= ShopList.m_nShopItemNum;	--出售物品的数量

	for i = 1, nShopItemNum do
		local ShopSellItem	= ShopList.m_ShopSellItems[i-1];
		local nId		= ShopSellItem.GoodsID;
		local nItemCount	= container:getItemCount(nId);
		local HpIndex		= VipData.m_nHpIndex > 0 and VipData.m_nHpIndex or 1;
		local MpIndex		= VipData.m_nMpIndex > 0 and VipData.m_nMpIndex or 1;
		if VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_HPFOOD] and nId == t_VIPAssistFood[VIP_FOOD_TYPE_HP][HpIndex] then
			local nNum = VipData.m_nHPNum - nItemCount;
			if nNum > 0 then
				NpcShop:buyItem( i-1, nNum );
			end
		end
		if VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_MPFOOD] and nId == t_VIPAssistFood[VIP_FOOD_TYPE_MP][MpIndex] then
			local nNum = VipData.m_nMPNum - nItemCount;
			if nNum > 0 then
				NpcShop:buyItem( i-1, nNum );
			end
		end
	end
end
function RepairEquipAndSaleItem( VipData )
	local MainPlayer = ActorMgr:getMainPlayer();
	--修理所有装备
	if VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_REPAIR] then
		local nAllPrice = NpcShop:getRepairAllPrice();
		local nMoney 	= MainPlayer:getMoney();
		local nBull		= MainPlayer:getBull();
		if nMoney + nBull < nAllPrice then 
			ShowMidTips("绑银不足，无法修理");
		else 
			NpcShop:repairAllItem();
		end
	end    
	
	--出售装备
	local container		= MainPlayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("背包锁已锁无法出售，请先验证背包安全锁");
		return;
	end
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item 		= container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
		local itemDef 	= item:getItemDef();
		local idx 		= CheckItemNeedImportantConfirm( item );
		if itemDef ~= nil and item:getItemType() == ITEM_SLOTTYPE_AMM and not IsFashionItem( itemDef ) and not IsItemLock(CONTAINER_TYPE_SUNDRIES,i-1 ) and idx == 0 then
			--出售白装、绿装、蓝装
			if ( VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_SELL_NORMAL] and itemDef.Important == ITEM_IMPORTANT_NORMAL )
			or ( VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_SELL_EXCELLENCE] and itemDef.Important == ITEM_IMPORTANT_EXCELLENCE )
			or ( VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_SELL_BEST] and itemDef.Important == ITEM_IMPORTANT_BEST ) then

				NpcShop:sellItem( CONTAINER_TYPE_SUNDRIES, i-1, 1 );
			end
		end
	end
end

function DecomposeContainerEquip()
	local VipData 		= AutoAssist:getVIPAutoAssistOpt();
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	if not VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_DECOPOSE_EXCELLENCE] and 
		not	VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_DECOPOSE_BEST] then 
		DecomposeEnd = true;
		return;	
	end
	local lockState 	= container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("背包锁已锁无法进行分解，请先验证背包安全锁");
		DecomposeEnd = true;
		return;
	end
	-- 消耗银币提示界面 停止分解
	if CostMoneyConfirmFrame:IsShown() then 
		CostMoneyConfirmFrameCancelBtn_OnClick();
		DecomposeEnd = true;
		return;
	end
	-- 背包空间不足
	if container:getUnusedGridNum( CONTAINER_TYPE_SUNDRIES ) == 0 then 
	--	ShowMidTips("背包空间不足，请整理背包！");
		DecomposeEnd = true;
		return;
	end
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	if not PlayerContainerResolveFrame:IsShown() then
		ContainerResolveBtn_onClick();
	end
	
	if IsEquipResolved then
		return;
	end
	if UIIsInDragState() then
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	for i = 1, RESOLVE_MAX do
		local btn			= getglobal( "PlayerContainerResolveFrameItem"..i );
		local desList		= btn:GetClientUserData( 1 );
		local desGrid		= btn:GetClientUserData( 2 );
		if desList > 0 then 
			PlayerContainerResolveBegin_onClick();
			return;
		end 	
	end
	local ItemNum = 0;
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item 	= container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 );
		local idx 	= CheckItemNeedImportantConfirm( item );
		if item ~= nil and item:getItemType() == ITEM_SLOTTYPE_AMM and not IsItemLock(CONTAINER_TYPE_SUNDRIES,i-1 ) and idx == 0 then
			--不分解带完美星的装备
			if not ( VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_NO_PERFECT_STAR] and item:getArmPefectStarNum() > 0 ) then
				if ChooseCanDecomposeAmmItem( VipData,item,i) then
					ItemNum = ItemNum + 1;
				end
			end	
		end
		if ItemNum >= RESOLVE_MAX then
			break;
		end
	end
	if ItemNum == 0 then
		DecomposeEnd = true;
		PlayerContainerFrameLeftPageBtn_OnClick();
		PlayerContainerResolveFrame:Hide();
		PlayerContainerFrame:Hide();
	else
		PlayerContainerResolveBegin_onClick();
	end
end

function ChooseCanDecomposeAmmItem( VipData,item,Index )
	local BtnIndex 		= Index;
	local itemDef 		= item:getItemDef();
	local MainPlayer	= ActorMgr:getMainPlayer();
	if item == nil then 
		return false;
	end
	-- 装备等级高于玩家等级20级
	if MainPlayer:getLv() + 20 < itemDef.WearPre.RoleLevelMin then 
		return false;
	end
	-- 装备未鉴定
	if item:isArmIdentity() then 
		return false;
	end
	if item:isFashionSuit() then 
		return false;
	end
	-- 不能分解
	if itemDef.NoResolve == 1 then 
		return false;
	end
	-- 分解绿装
	if VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_DECOPOSE_EXCELLENCE] and itemDef.Important == ITEM_IMPORTANT_EXCELLENCE then
		if Index > nSUNDRIESGridMax then
			PlayerContainerFrameRightPageBtn_OnClick();
			BtnIndex = 	Index - nSUNDRIESGridMax;
		end
		AddItemToResolveEquipFrame( item, CONTAINER_TYPE_SUNDRIES,Index -1 )
	--	AddItemToContainerEquipResolve( item, CONTAINER_TYPE_SUNDRIES,Index )
	--	ContainerRightBtnClick( "PlayerContainerFrameItem"..BtnIndex )        --右键点击功能
		return true;
	end
	-- 分解蓝装
	if VipData.m_bVipChkBtn[VIP_ASSIST_OPTION_DECOPOSE_BEST] and itemDef.Important == ITEM_IMPORTANT_BEST then
		if Index > nSUNDRIESGridMax then
			PlayerContainerFrameRightPageBtn_OnClick();
			BtnIndex = 	Index - nSUNDRIESGridMax;
		end
		AddItemToResolveEquipFrame( item, CONTAINER_TYPE_SUNDRIES,Index-1 )
	--	ContainerRightBtnClick( "PlayerContainerFrameItem"..BtnIndex ) ;
		return true;
	end	
	return false;
end

function VipAssistApplyBtn_OnClick()
	if not VipManager:isEnableVip( STAR_STONE_FIRE ) then
		ShowMidTips("您没有火焰徽章特权，不能使用高级辅助");	
		return;
	end	
	local VipData = AutoAssist:getVIPAutoAssistOpt();
	for i=1, VIP_ASSIST_OPTION_END do
		local chkBtn = getglobal("VipAutoAssistFrame_ChkBtn"..i);
		VipData.m_bVipChkBtn[i-1] = chkBtn:GetCheckState();
	end
	VipData.m_nInterval = tonumber( VipAutoAssistFrameTimesEdit:GetText() );
	VipData.m_nHpIndex	= VipAutoAssistHpChooseBtn:GetClientUserData( 0 );
	VipData.m_nMpIndex	= VipAutoAssistMpChooseBtn:GetClientUserData( 0 );
	VipData.m_nHPNum 	= tonumber( VipAutoAssistFrameHpNumEdit:GetText() );
	VipData.m_nMPNum 	= tonumber( VipAutoAssistFrameMpNumEdit:GetText() );
	AutoAssist:saveVIPAssistOpt();
	ShowMidTips("高级辅助设置成功");
end

function VipAssistCancelBtn_OnClick()
	VipAutoAssistFrame:Hide();
end

function VIPAutoAssistFrameOptCheckBtn_OnClick()
	--elmer 是否是火焰徽章
	if not VipManager:isEnableVip( STAR_STONE_FIRE ) then
		this:setCheckState( not this:GetCheckState() );
		ShowMidTips("您没有火焰徽章特权，不能使用高级辅助");	
		return;
	end
end

function VipAutoAssistFoodSelectBtn_OnClick()
	if VipAutoAssistHpSelectBtn:IsShown() then
		VipAutoAssistHpChooseBtn:SetClientUserData( 0, this:GetClientID() );
		VipAutoAssistHpSelectBtn:Hide();
		local HpFont = getglobal( this:GetName().."Font")
		VipAutoAssistHpChooseBtnFont:SetText( HpFont:GetText() );
	else
		VipAutoAssistMpChooseBtn:SetClientUserData( 0, this:GetClientID() );
		VipAutoAssistMpSelectBtn:Hide();
		local MpFont = getglobal( this:GetName().."Font")
		VipAutoAssistMpChooseBtnFont:SetText( MpFont:GetText() );
	end
end

function VipAutoAssistHpScrollDownBtnOnClick()
	if VipAutoAssistHpSelectBtn:IsShown() then
		VipAutoAssistHpSelectBtn:Hide();
	else
		VipAutoAssistHpSelectBtn:Show();
	end
end

function VipAutoAssistMpScrollDownBtnOnClick()
	if VipAutoAssistMpSelectBtn:IsShown() then
		VipAutoAssistMpSelectBtn:Hide();
	else
		VipAutoAssistMpSelectBtn:Show();
	end
end
function vipAssistEdit_onFocusLost()
	local nNumber = this:GetText();
	if nNumber == "" then
		nNumber = "0";
	else 
		nNumber = tonumber(nNumber);
	end
	this:SetText(nNumber);
end
function VipAssistFrameWarningBtnOnEnter()
	local szText = "不支持失落之域";
	SetGameTooltips(this:GetName(),szText)
end
function CareerChangeClearSkill()
	for i = 1, MAX_SKILL_TABLE do		
		AutoAssist:getAutoAssistOpt().m_nSkillId[i-1]	= 0;
	end
	AutoAssist:saveAutoAssistOpt()
end
