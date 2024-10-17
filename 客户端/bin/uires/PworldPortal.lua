local MAX_PWORLD_LIST_NUM = 8;
local MAX_REWARD_ITEM_NUM = 5;
local lastSelectPworld;
local lastSelectDiff;
local selectEntry;
local t_PworldList = {};

local difficultyMap = {
	[PworldDifficulty.EASY] = {
		suffix = "Easy",
		tex = {
			x = 112, y = 636, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "简    单",
	},
	[PworldDifficulty.GENERAL] = {
		suffix = "General",
		tex = {
			x = 157, y = 636, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "普    通",
	},
	[PworldDifficulty.HARD] = {
		suffix = "Hard",
		tex = {
			x = 164, y = 611, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "困    难",
	},
	[PworldDifficulty.EXPERT] = {
		suffix = "Expert",
		tex = {
			x = 786, y = 878, w = 44, h = 24,
			path = "uires\\ZhuJieMian\\2.dds",
		},
		text = "专    家",
	},
}

local difficultyTips = {
	[PworldDifficulty.EASY]		= "简单难度副本，今日可进入次数为：%d",
	[PworldDifficulty.GENERAL]	= "普通难度副本，今日可进入次数为：%d",
	[PworldDifficulty.HARD]		= "困难难度副本，今日可进入次数为：%d",
	[PworldDifficulty.EXPERT]	= "专家难度副本，今日可进入次数为：%d",
	
	default = "活动副本，今日可进入次数为：%d",
}

local difficultyTipsNolimit = {
	[PworldDifficulty.EASY]		= "简单难度副本，不限进入次数",
	[PworldDifficulty.GENERAL]	= "普通难度副本，不限进入次数",
	[PworldDifficulty.HARD]		= "困难难度副本，不限进入次数",
	[PworldDifficulty.EXPERT]	= "专家难度副本，不限进入次数",
	
	default = "活动副本，不限进入次数",
}

function GetDynLevel()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return 1;
	end
	
	local DynLv = 1;
	local TotalLv = 0;
	if mainplayer:isInTeam() then
		DynLv = mainplayer:getLv();
		local teamInfo = TeamManager:getTeamInfo();
		for i = 1, teamInfo.MemberNum do
			local teamMember = teamInfo.TeamMember[i-1];
			if teamMember.Level < DynLv then
				DynLv = teamMember.Level;
			end
			TotalLv = TotalLv + teamMember.Level;
		end
		local lv = TotalLv / teamInfo.MemberNum;
		if lv > DynLv + 10 then
			DynLv = DynLv + 10;
		else
			DynLv = lv;
		end
	else
		DynLv = mainplayer:getLv();
	end

	return math.floor( DynLv );
end

function DisCheckAllSelectBtn()
	for _, attrib in pairs(difficultyMap) do
		local btn = getglobal("PworldPortalFrameSelect"..attrib.suffix.."Btn");
		btn:DisChecked();
	end
end

function DisCheckAllPworldEntry()
	for i=1, MAX_PWORLD_LIST_NUM do
		local btn = getglobal( "PworldPortalFramePworldEntry"..i );
		btn:DisChecked();
	end
end

function ResetPworldPortalFrame()
	lastSelectPworld = 0;
	lastSelectDiff = 0;
	selectEntry = "";

	local slider = getglobal("PworldPortalFrame_ScrollBar");
	SetScrollBar( "PworldPortalFrame_ScrollBar", MAX_PWORLD_LIST_NUM, #t_PworldList );
	slider:SetValue( 0 );

	DisCheckAllPworldEntry();
	DisCheckAllSelectBtn();
end

function GetDifficultyType( nFlag, nId )
	if nFlag == CLT_AUTO_TEAM_TASK then
		return 0;
	end

	local nType = 0;
	for i = 1, PworldDifficulty.MAXNUM - 1 do
		if PworldDifficulty:getDifficultyPworldId( nId, i ) ~= 0 then
			nType = 1;
			break
		end
	end

	return nType;
end

function SortPworldList()
	if table.getn( t_PworldList ) <= 2 then
		return;
	end

	table.sort( t_PworldList, 
	function ( lhs, rhs )
		return lhs.level < rhs.level;
	end
	);
end

function GetPworldList()
	t_PworldList = {};
	for  i = 1, GamePromotion:getAllNum() do
		local nRow	= GamePromotion:getAllRow( i - 1 );
		local nResult	= GamePromotion:getAutoTeamStateByNoCountLimit( nRow );
		
		if nResult ~= PROMOTION_NOT_NEED_AUTOTEAM and nResult ~= PROMOTION_NOT_TODAY then
			local activeInfo	= GamePromotion:getPromotionData( nRow );
			table.insert( t_PworldList, { relateId = activeInfo.m_nRelateId, name = PworldDifficulty:getDiffPworldName( activeInfo.m_nRelateId ), 
				level = activeInfo.m_nLevel, diffType = GetDifficultyType( activeInfo.m_nFlag, activeInfo.m_nRelateId ), row = nRow } );
		end
	end
	SortPworldList();
end

function SelectPworldEntry( idx )
	DisCheckAllPworldEntry();
	if idx ~= nil then
		local btn = getglobal("PworldPortalFramePworldEntry" .. idx );
		btn:Checked();
	end
end

function UpdatePworldList( nIndex )
	local count = 1;
	local hasSelect = nil;
	for i=nIndex, #t_PworldList do
		if count > MAX_PWORLD_LIST_NUM then
			break;
		end

		local name = "PworldPortalFramePworldEntry"..count;
		local btn = getglobal( name );
		btn:SetClientUserData( 0, t_PworldList[i].relateId );
		btn:SetClientUserData( 1, t_PworldList[i].diffType );
		btn:SetClientString( t_PworldList[i].name );
		btn:Enable();
				
		btn = getglobal( name .. "PworldFont" );
		btn:SetText( t_PworldList[i].name );
		btn:SetTextColor( 160, 160, 90 );
		btn = getglobal( name .. "MinLevelFont" );
		btn:SetText( "（传送等级：" .. t_PworldList[i].level .. "级）" );

		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then break end;

		if t_PworldList[i].level <= mainplayer:getLv() then
			btn:SetTextColor( 0, 255, 0 );
		else
			btn:SetTextColor( 255, 0, 0 );
		end

		if t_PworldList[i].diffType == 0 then
			local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(t_PworldList[i].relateId, 0));
			--迷之塔一直绿色显示
			if not IsMiZhiTaPworld(PworldDifficulty:getDifficultyPworldId(t_PworldList[i].relateId, 0)) then
				if data.freeTimeTotal - data.freeTimeUse == 0 then
					btn = getglobal( name .. "PworldFont" );
					btn:SetTextColor( 120, 120, 120 );
					btn = getglobal( name .. "MinLevelFont" );
					btn:SetTextColor( 120, 120, 120 );
				end
			end
		end

		if t_PworldList[i].name == selectEntry then
			hasSelect = count;
		elseif selectEntry == "" then
			selectEntry = t_PworldList[1].name;
			hasSelect = 1;
		end

		count = count + 1;
	end

	SelectPworldEntry( hasSelect );

	for i=count, MAX_PWORLD_LIST_NUM do
		local name = "PworldPortalFramePworldEntry"..i;
		local btn = getglobal( name );
		btn:Disable();
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientString( "" );
		btn = getglobal( name .. "PworldFont" );
		btn:SetText( "" );
		btn = getglobal( name .. "MinLevelFont" );
		btn:SetText( "" );
	end
end

function SetPworldType( nType )
	local btn;
	local name = "PworldPortalFrameDifficulty";
	if nType > 0 then
		btn = getglobal( name );
		btn:Show();
		btn = getglobal( name .. "Dyn" );
		btn:Hide();
		btn = getglobal( name .. "DynLevel" );
		btn:SetText( "" );
		PworldPortalFrameLock:Hide();
		PworldPortalFrameSelect:Show();
	else		
		btn = getglobal( name );
		btn:Hide();
		btn = getglobal( name .. "Dyn" );
		btn:Show();
		btn = getglobal( name .. "CostInfo" );
		btn:SetText( "" );
		PworldPortalFrameLock:Show();
		PworldPortalFrameSelect:Hide();
	end
end

function GetPworldListIndex()
	for i=1, #t_PworldList do
		if t_PworldList[i].relateId == lastSelectPworld then
			return i;
		end
	end
end

function UpdatePworldInfo()
	local pworldId = PworldDifficulty:getDifficultyPworldId( lastSelectPworld, lastSelectDiff );
	local pworldDef = getPworldDef( pworldId );
	if pworldDef == nil or pworldDef.PworldID == 0 then
		return;
	end

	local btn;
	local name = "PworldPortalFrameDifficulty";
	local idx = GetPworldListIndex();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	if t_PworldList[idx].level <= mainplayer:getLv() then
		PworldPortalFrameEnter:setInputTransparent( false );
		PworldPortalFrameEnter:SetGray( false );
	else
		PworldPortalFrameEnter:setInputTransparent( true );
		PworldPortalFrameEnter:SetGray( true );
	end
	PworldPortalFrame_VigBtn:Hide()
	PworldPortalFrameGetVig:Hide()
	if pworldDef.DynType > 0 then
		btn = getglobal( name .. "DynLevel" );
		local level = GetDynLevel();
		local activeInfo = GamePromotion:getPromotionData( t_PworldList[idx].row );
		btn:SetText( "（" .. ( level < activeInfo.m_nLevel and activeInfo.m_nLevel or level) .. "级副本）" );

		local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastSelectPworld, 0));
		if data.freeTimeTotal - data.freeTimeUse == 0 then
			PworldPortalFrameEnter:setInputTransparent( true );
			PworldPortalFrameEnter:SetGray( true );
		end
		btn = getglobal( "PworldPortalFrameLeftTitle2" );
		btn:SetText("队伍经验：");

		btn = getglobal( name .. "Desc" );
		btn:SetText( PworldDifficulty:getDifficultyDesc( lastSelectPworld, lastSelectDiff ) );
		btn:SetTextColor( 0, 255, 0 );
	else
		--先清除上次显示的动态副本的信息
		btn = getglobal( name .. "DynLevel" );
		btn:SetText( "" );
		for difficulty, attrib in pairs(difficultyMap) do
			btn = getglobal("PworldPortalFrameSelect"..attrib.suffix.."Btn");
			local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastSelectPworld, difficulty));
			if data.freeTimeTotal ~= 0 and data.freeTimeTotal - data.freeTimeUse == 0 then
				btn:Disable();
				if difficulty > 0 and difficulty == lastSelectDiff then
					lastSelectDiff = lastSelectDiff - 1;
					pworldId = PworldDifficulty:getDifficultyPworldId( lastSelectPworld, lastSelectDiff );
					pworldDef = getPworldDef( pworldId );
				end
			else
				btn:Enable();
			end
		end

		btn = getglobal( name );
		local diffTex = difficultyMap[lastSelectDiff].tex
		btn:SetTexture( diffTex.path );
		btn:SetTexUV( diffTex.x, diffTex.y, diffTex.w, diffTex.h );
		btn:Show();
		btn = getglobal( name .. "CostInfo" );
		if pworldDef.EnterUseVig ~= 0 then
			btn:SetText( "（每人消耗" .. pworldDef.EnterUseVig .."活力值）" );
			PworldPortalFrame_VigBtn:Show()
			PworldPortalFrameGetVig:Show()
		else
			btn:SetText( "" );
		end

		DisCheckAllSelectBtn();
		btn = getglobal("PworldPortalFrameSelect" .. difficultyMap[lastSelectDiff].suffix .. "Btn");
		btn:Checked();

		btn = getglobal( "PworldPortalFrameLeftTitle2" );
		btn:SetText("推荐级别：");

		btn = getglobal( name .. "Desc" );
		btn:SetText( PworldDifficulty:getDifficultyDesc( lastSelectPworld, lastSelectDiff ) );
		btn:SetTextColor( 255, 255, 190 );
	end

	btn = getglobal( "PworldPortalFrameLeftTitle3" );
	if pworldDef.AutoPworldNum > 1 then
		btn:SetText("进入人数：队伍人数不能少于" .. pworldDef.AutoPworldNum .. "人" );
	else
		btn:SetText("");
	end

	--[[btn = getglobal( "PworldPortalFramePortalCost" );
	if pworldDef.AutoPworldUseVal > 0 then
		btn:SetText( "进行传送需要消耗：" .. pworldDef.AutoPworldUseVal .. " " .. GetMoneyDesc( pworldDef.AutoPworldUseType ) );
	else
		btn:SetText( "进行传送需要消耗：-----" );
	end--]]

	for i = 1, MAX_REWARD_ITEM_NUM do
		btn = getglobal("PworldPortalFrameItem"..i)
		local id = PworldDifficulty:getDifficultyReward(lastSelectPworld, lastSelectDiff, i - 1)
		if id == 0 then
			btn:Hide()
		else
			btn:Show()
			btn:SetClientUserData(0, id)
			name = btn:GetName()
			local icon = getglobal(name.."Icon")
			if icon then
				local ItemDef = getItemDef(id)
				local IconPath = DEFAULT_ITEM_PATH
				if ItemDef then
					IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
				end
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence(IconPath) then
					IconPath = DEFAULT_ITEM_PATH
				end
				icon:SetTexture(IconPath)
				icon:SetTexRelUV(1, 1)
				SetEquipImportantEffectUV(ItemDef, name.."UVAnimationTex")
			end
		end
	end
end

function OpenPworldPortalFrame( nId, nFlag )
	if nId == 0 then
		PworldPortalPrepareFrame:Hide();
		if nFlag == 0 then
			PworldPortalFrame:Show();
		else
			EndPlayAutoTeamBtnEffect()
		end
	else
		PworldPortalFrame:Hide();
		local frame = getglobal( "PworldPortalConfirmFrame" );
		frame:SetClientUserData( 0, nId );

		local font = getglobal( "PworldPortalConfirmFrameMessageFont" );
		local pworldDef = getPworldDef( nId );
		if pworldDef.PworldID ~= 0 then
			font:SetText( "#B" .. pworldDef.Name .. "#n已开启，是否需要马上传送？", 255, 255, 190 );
		end
		if nFlag == 0 then
			frame:Show();
		elseif not frame:IsShown() then
			local pworldId = math.floor( MapShower:getRealMapId()/100 )
			if pworldId ~= nId then
				BeginPlayAutoTeamBtnEffect()
			end
		end
	end
end

function PworldPortalFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
	this:RegisterEvent("GE_ENTER_MAP");
	this:RegisterEvent("GE_MAIN_JOIN_TEAM_SUCC");
	this:RegisterEvent("GE_TEAM_LEAVETEAM");
	this:RegisterEvent("GE_TEAM_MEMBER_NUM_CHG");
	this:RegisterEvent("GE_PWORLD_LIMIT_UPDATE");

	local i = 2;
	local ui;
	while true do
		ui = getglobal( "PworldPortalFramePworldEntry" .. i .. "BlackTex" );
		if not ui then
			break
		end
		ui:Hide();
		i = i + 2;
	end

	for difficulty, attrib in pairs(difficultyMap) do
		local button = getglobal("PworldPortalFrameSelect"..attrib.suffix.."Btn")
		button:SetClientUserData(0, difficulty)
		button:SetText(attrib.text)
	end
end

function InitPworldPortalFrame()
	GetPworldList();
	ResetPworldPortalFrame();
	UpdatePworldList( 1 );
	local name = "PworldPortalFramePworldEntry1";
	local btn = getglobal( name );
	selectEntry = btn:GetClientString();
	SelectPworld( name );
	SetPworldType( btn:GetClientUserData(1) );
	UpdatePworldInfo();

	InitPortalConfirmFrame();
end

function PworldPortalFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		InitPworldPortalFrame();
		AutoPworldManager:requestPortalFrame( 1 );
	elseif arg1 == "GE_ENTER_MAP" then
		if PworldPortalFrame:IsShown() then
			PworldPortalFrame:Hide();
		end
		if PworldPortalConfirmFrame:IsShown() then
			PworldPortalConfirmFrame:Hide();
		end
	elseif arg1 == "GE_MAIN_JOIN_TEAM_SUCC" then
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "副本传送" then
			MessageBoxFrame:Hide();
		end

		local mainplayer = ActorMgr:getMainPlayer();
		local btn = getglobal( "PworldPortalFrameEnter" );
		if mainplayer == nil or ( mainplayer:isInTeam() and not mainplayer:isCaptain() ) then
			btn:Hide();
		else
			btn:Show();
		end
		EndPlayAutoTeamBtnEffect();

		if PworldPortalFrame:IsShown() then
			local slider = getglobal( "PworldPortalFrame_ScrollBar" );
			UpdatePworldList( slider:GetValue() + 1 );
			UpdatePworldInfo();
		end
	elseif arg1 == "GE_TEAM_LEAVETEAM" then
		if PworldPortalConfirmFrame:IsShown() then
			PworldPortalConfirmFrame:Hide();
		end
		PworldPortalFrameEnter:Show();
		EndPlayAutoTeamBtnEffect();

		if PworldPortalFrame:IsShown() then
			local slider = getglobal( "PworldPortalFrame_ScrollBar" );
			UpdatePworldList( slider:GetValue() + 1 );
			UpdatePworldInfo();
		end
	elseif arg1 == "GE_TEAM_MEMBER_NUM_CHG" then
		if PworldPortalFrame:IsShown() then
			local slider = getglobal( "PworldPortalFrame_ScrollBar" );
			UpdatePworldList( slider:GetValue() + 1 );
			UpdatePworldInfo();
		end
	end
end

function PworldPortalFrame_OnShow()
	if PworldDifficultyTipsFrame:IsShown() then
		PworldDifficultyTipsFrame:Hide();
	end

	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	local btn = getglobal( "PworldPortalFrameEnter" );
	if mainplayer == nil or ( mainplayer:isInTeam() and not mainplayer:isCaptain() ) then
		btn:Hide();
	else
		btn:Show();
	end
	local slider = getglobal( "PworldPortalFrame_ScrollBar" );
	UpdatePworldList( slider:GetValue() + 1 );
	UpdatePworldInfo();
end

function PworldPortalFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "副本传送" then
		MessageBoxFrame:Hide();
	end
end

function PworldPortalReward_OnEnter()
	local id = this:GetClientUserData(0)
	if id and id ~= 0 then
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id));
	end
end

function PworldPortalReward_OnLeave()
	ItemButton_OnLeave();
end

function PworldPortalFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "PworldPortalFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function PworldPortalFrame_ScrollDownBtn_OnClick()
	local slider = getglobal( "PworldPortalFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function PworldPortalFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("PworldPortalFrame_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdatePworldList( nValue + 1 );
	end
end

function PworldPortal_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	
	if mainplayer:isInTeam() and not mainplayer:isCaptain() then
		ShowMidTips("只有队长能开启副本传送");
		return;
	end

	if mainplayer:isDead() then
		ShowMidTips("请先复活再使用此功能");
		return;
	end

	if mainplayer:isInStall() then
		ShowMidTips("你正在摆摊中，无法传送");
		return;
	end

	if mainplayer:isInPK() then
		ShowMidTips("你正在切磋中，无法传送");
		return;
	end

	if GameExchange:isInExchange() then
		ShowMidTips("你正在交易中，无法传送");
		return;
	end

	if mainplayer:isInFly() then
		ShowMidTips("当前状态下无法传送");
		return;
	end

	local equip = mainplayer:getEquip()
	if equip:isUsedTmp() then
		local nTmpItemId = equip:getTmpMachin();				
		if nTmpItemId ~= nil or nTmpItemId ~= 0 then
			ShowMidTips("当前状态下无法传送");
			return;
		end
	end

	if mainplayer:isOnMonsterMachine() then
		ShowMidTips("使用机械中无法传送");
		return;
	end

	if lastSelectPworld ~= 0 then
		local pworldId = PworldDifficulty:getDifficultyPworldId( lastSelectPworld, lastSelectDiff );
		local PworldDef = getPworldDef( pworldId );

		local teamInfo = TeamManager:getTeamInfo();
		if PworldDef.AutoPworldNum > 1 and teamInfo.MemberNum < PworldDef.AutoPworldNum then
			ShowMidTips( "队伍人数不足" .. PworldDef.AutoPworldNum .. "人，无法开启副本" );
			return;
		end

		--[[
		if getPlayerMoney( mainplayer ) < PworldDef.AutoPworldUseVal then
			ShowMidTips( "绑银不足，无法传送" );
			return;
		end
		--]]
		
		if PworldDef.AutoPworldNum > 0 then
			for i=1,teamInfo.MemberNum do
				if teamInfo.TeamMember[i-1].Level < PworldDef.MinLv then
					ShowMidTips( teamInfo.TeamMember[i-1].MemberName .. "等级低于副本最小等级要求" )
					return
				end
				if teamInfo.TeamMember[i-1].Level > PworldDef.MaxLv then
					ShowMidTips( teamInfo.TeamMember[i-1].MemberName .. "等级高于副本最大等级要求" )
					return
				end
			end
		end

		if PworldDef.EnterUseVig <= 0 then
			AutoPworldManager:requestPworldPortal( pworldId );
			return;
		end

		if mainplayer:getVigor() < PworldDef.EnterUseVig then
			ShowMidTips( "活力值不足，无法传送" );
		else
			MessageBox( "活力值消耗提示", "进入该副本需要消耗"..PworldDef.EnterUseVig.."点活力值" )
			MessageBoxFrame:SetClientString( "副本传送" );
			MessageBoxFrame:SetClientUserData( 0, pworldId );
		end
	end
end

function PworldPortalFrameLock_OnEnter()
	local str
	local pworldId = PworldDifficulty:getDifficultyPworldId( lastSelectPworld, lastSelectDiff );
	local pworldDef = getPworldDef( pworldId );
	if pworldDef.PworldID == 0 then
		return;
	end

	local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastSelectPworld, lastSelectDiff));
	if pworldDef.DynType > 0 then
		if data.freeTimeTotal == 0 then
			str = difficultyTipsNolimit.default or ""
		else
			str = string.format(difficultyTips.default or "", data.freeTimeTotal - data.freeTimeUse)
		end
	else
		
		if data.freeTimeTotal == 0 then
			str = difficultyTipsNolimit[difficulty] or ""
		else
			str = string.format(difficultyTips[difficulty] or "", data.freeTimeTotal - data.freeTimeUse)
		end
	end
	
	if str == "" then return end
	
	SetGameTooltips(this:GetName(), str)
end

function PworldPortalFrameLock_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function PworldPortalSelect_OnEnter()
	local str
	local difficulty = this:GetClientUserData(0)
	local data = PworldLimitInfo:getPworldData(PworldDifficulty:getDifficultyPworldId(lastSelectPworld, difficulty))
	if data.freeTimeTotal == 0 then
		str = difficultyTipsNolimit[difficulty] or ""
	else
		str = string.format(difficultyTips[difficulty] or "", data.freeTimeTotal - data.freeTimeUse)
	end
	SetGameTooltips(this:GetName(), str)
end

function PworldPortalSelect_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function SelectPworld( name )
	local btn = getglobal( name );
	local id = btn:GetClientUserData( 0 );	
	local pworldId = PworldDifficulty:getDifficultyPworldId( id, 0 );
	local mapid = PworldDifficulty:getPworldReferMapId( pworldId );
	local path = string.format("uires\\DiTuLei\\QuYuDiTu\\%d.dds", mapid );
	if not IsInExistence(path) then
		PworldPortalFrameMap:Hide();
	else
		PworldPortalFrameMap:Show();
		PworldPortalFrameMap:SetTexture( path );
	end
	PworldPortalFrameMapName:SetText( btn:GetClientString() );
	lastSelectPworld = id;
	if btn:GetClientUserData( 1 ) == 0 then
		lastSelectDiff = 0;
	end
	UpdatePworldInfo();
end

function PworldEntry_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "副本传送" then
		MessageBoxFrame:Hide();
	end

	DisCheckAllPworldEntry();
	this:Checked();
	SelectPworld( this:GetName() );
	SetPworldType( this:GetClientUserData( 1 ) );
	selectEntry = this:GetClientString();
end

function InitPortalConfirmFrame()
	local frame = getglobal( "PworldPortalConfirmFrame" );
	frame:SetClientUserData( 0, 0 );
	local font = getglobal( "PworldPortalConfirmFrameMessageFont" );
	font:SetText( "", 255, 255, 0 );
end

--迷之塔副本id列表
local t_MiZhiTaPworldID={321,322,323,324,325,326,327,328,329};
function IsMiZhiTaPworld( nId )
	for i = 1, table.getn( t_MiZhiTaPworldID ) do
		if t_MiZhiTaPworldID[i] == nId then
			return true
		end
	end
	return false
end
function ConfirmPworldPortal( nCancel,nId,num )
	if PworldPortalFrame:IsShown() then
		PworldPortalFrame:Hide();
	end
	--如果是迷之塔副本
	if IsMiZhiTaPworld( nId ) then
		ShowMiZhiTaReadyFrame( nId )
		return
	end
	if nCancel == 1 then
		PworldPortalPrepareFrame:Hide()
		PworldPortalConfirmFrame:Hide()
		EndPlayAutoTeamBtnEffect()
		return
	end
	local pworldDef = getPworldDef( nId )
	if pworldDef == nil then return	end
	local frame
	if num > 0 then
		frame = getglobal( "PworldPortalPrepareFrame" )
		PworldPortalConfirmFrame:Hide()
		PworldPortalPrepareFramePworldName:SetText( pworldDef.Name )
	else
		frame = getglobal( "PworldPortalConfirmFrame" )
		PworldPortalPrepareFrame:Hide()
		local font = getglobal( "PworldPortalConfirmFrameMessageFont" );		
		font:SetText( "#B" .. pworldDef.Name .. "#n已开启，是否需要马上传送？", 255, 255, 190 );
		local nCurMapID	= MapShower:getRealMapId();
		if nCurMapID >= MIN_COPY_ID then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end

			if mainplayer:isInTeam() and not mainplayer:isCaptain() then
				ShowMidTips( "队长已开启<" .. pworldDef.Name .. ">的传送" );
				return;
			end
		end
	end
	
	frame:SetClientUserData( 0, nId );

	frame:Show();
end

function PworldPortalSelect_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "副本传送" then
		MessageBoxFrame:Hide();
	end

	lastSelectDiff = this:GetClientUserData( 0 );
	UpdatePworldInfo();
end


function PworldPortalConfirmFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	EndPlayAutoTeamBtnEffect();
end

function PworldPortalConfirmFrame_OnHide()
	local nCurMapID	= MapShower:getRealMapId();
	if nCurMapID < MIN_COPY_ID and AutoPworldManager:getPworldState() > 0 then
		BeginPlayAutoTeamBtnEffect();
	else
		EndPlayAutoTeamBtnEffect();
	end
end

function PworldPortalConfirmFrameConfirmBtn_OnClick()
	local frame = getglobal( "PworldPortalConfirmFrame" );
	AutoPworldManager:requestPworldPortal( frame:GetClientUserData( 0 ) );
	util.closeParent( this );
end

local MAX_PLAYER_NUM = 6
local function UpdatePrepareFrame()
	PworldPortalPrepareFrameOkBtn:Enable()
	PworldPortalPrepareFrameOkBtn:SetText("准 备")
	PworldPortalPrepareFrameCancelBtn:Enable()
	local num = AutoPworldManager:getMemberNum()
	local mainplayer = ActorMgr:getMainPlayer()
	for i=1,num do
		local data = AutoPworldManager:getMemberData( i-1 )
		local member = TeamManager:getTeamMemberInfoById( data.MemID )
		local t_fonts = {	["PworldPortalPrepareFrameLine" .. i .. "Name"] = { value = member.MemberName },
							["PworldPortalPrepareFrameLine" .. i .. "Lv"]	= { value = member.Level },
							["PworldPortalPrepareFrameLine" .. i .. "Career"]= { value = t_raceList[member.Career] },
						}
		util.SetFonts( t_fonts )
		local tex = getglobal( "PworldPortalPrepareFrameLine" .. i .. "StatusTex" )
		tex:Show()
		if data.Ready == 1 then
			tex:ChangeTextureTemplate( "ReadyTex" )
		else
			tex:ChangeTextureTemplate( "PrepareTex" )
		end
		
		tex = getglobal( "PworldPortalPrepareFrameLine" .. i .. "CaptainTex" );
		if member.Position == TEAM_POSITION_CAPTAIN then
			tex:Show();
		else
			tex:Hide();
		end

		if data.MemID == mainplayer:GetID() then
			if data.Ready == 1 then
				PworldPortalPrepareFrameOkBtn:Disable()
				PworldPortalPrepareFrameCancelBtn:Disable()
			end
			if mainplayer:isCaptain() then
				PworldPortalPrepareFrameOkBtn:Enable()
				PworldPortalPrepareFrameOkBtn:SetText( "传 送" )
			end
		end
	end
	for i=num+1,MAX_PLAYER_NUM do
		local t_fonts = {	["PworldPortalPrepareFrameLine" .. i .. "Name"] = { value = "" },
							["PworldPortalPrepareFrameLine" .. i .. "Lv"]	= { value = "" },
							["PworldPortalPrepareFrameLine" .. i .. "Career"]= { value = "" },							
						}
		util.SetFonts( t_fonts )
		local tex = getglobal( "PworldPortalPrepareFrameLine" .. i .. "StatusTex" )
		tex:Hide()
		tex = getglobal( "PworldPortalPrepareFrameLine" .. i .. "CaptainTex" );
		tex:Hide();
		local t_hideIcons = { "PworldPortalPrepareFrameTex" ..i+1 .."1", "PworldPortalPrepareFrameTex" ..i+1 .."2", "PworldPortalPrepareFrameTex" ..i+1 .."3", "PworldPortalPrepareFrameTex" ..i+1 .."4" }
		util.HideIcons( t_hideIcons )
	end
	local height = 22*num
	PworldPortalPrepareFrameBkg1:SetSize( 291, 26 + height )
	PworldPortalPrepareFrame:SetSize( 308, 110 + height )
	PworldPortalPrepareFrame_OnUpdate()
end

function PworldPortalPrepareFrame_OnShow()
	EndPlayAutoTeamBtnEffect()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	UpdatePrepareFrame()
end

function PworldPortalPrepareFrame_OnUpdate()
	local t = getServerTime().sec
	local start = AutoPworldManager:getStartTime()
	local left = 60 - t + start
	if left > 0 then
		PworldPortalPrepareFrameTimeFont:SetText( left .. "秒" )
	else
		this:Hide()
	end
end

function PworldPortalPrepareFrameOkBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isCaptain() then
		local num = AutoPworldManager:getMemberNum()
		for i=1,num do
			local data = AutoPworldManager:getMemberData(i-1)
			if data.Ready ~= 1 then
				ShowMidTips( "队友未准备" )
				return
			end
		end
	end
	AutoPworldManager:requestAgree( true )
end

function PworldPortalPrepareFrameCancelBtn_OnClick()
	AutoPworldManager:requestAgree( false )
end

function PworldPortalPrepareFrameMiniBtn_OnClick()
	BeginPlayAutoTeamBtnEffect()
	util.closeParent(this)
end
---------------------------------------------------迷之塔-队友准备界面------------------------------------------------------------
local t_MiZhiTaTeam = { {}, {}, {}, {}, {}, {}, }
local TOTAL_TEAM_NUM = 0
local MAX_TEAM_MEMBER = 6
local t_ReadyTex = {
	--已准备
	{ path = "uires\\ZhuJieMian\\1.dds", uv = {x=700, y=284, w=54, h=16,}, },
	--未准备
	{ path = "uires\\ZhuJieMian\\1.dds", uv = {x=755, y=284, w=54, h=16,}, },
}
function AddTeamMember( nIdx, szName, nLayer )
	t_MiZhiTaTeam[nIdx] = { name = szName, layer = nLayer, lv = 0, career = "", ready = 0, position = 0, }
	TOTAL_TEAM_NUM = nIdx
end
function SetMiZhiTaTeamMemBerInfo()
	for j = 1, TOTAL_TEAM_NUM do
		for i = 1, AutoPworldManager:getMemberNum() do
			local data = AutoPworldManager:getMemberData( i-1 )
			local member = TeamManager:getTeamMemberInfoById( data.MemID )
			if t_MiZhiTaTeam[j]["name"] == member.MemberName then
				t_MiZhiTaTeam[j]["lv"] = member.Level
				t_MiZhiTaTeam[j]["career"] = t_raceList[member.Career]
				t_MiZhiTaTeam[j]["ready"] = data.Ready
				t_MiZhiTaTeam[j]["position"] = member.Position
			end
		end
	end
end
function ShowMiZhiTaReadyFrame( nId )
	SetMiZhiTaTeamMemBerInfo()
	SetTeamReadyStatus()
	local t = getServerTime().sec
	local start = AutoPworldManager:getStartTime()
	local left = 60 - t + start
	if not MiZhiTaGetReadyFrame:IsShown() and left > 0 then
		MiZhiTaGetReadyFrame:Show()
	end
	MiZhiTaGetReadyFrame:SetClientUserData( 0, nId );
end
function SetTeamReadyStatus()
	local mainplayer = ActorMgr:getMainPlayer()
	--清空以前的显示
	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal("MiZhiTaGetReadyFrameTeamMember"..i)
		local CaptainTex = getglobal( btn:GetName().."CaptainTex" )
		CaptainTex:Hide()
		local Name = getglobal( btn:GetName().."Name" )
		Name:SetText( "" )
		local LV = getglobal( btn:GetName().."LV" )
		LV:SetText( "" )
		local RaceName = getglobal( btn:GetName().."RaceName" )
		RaceName:SetText( "" )
		local LayerPassNum = getglobal( btn:GetName().."LayerPassNum" )
		LayerPassNum:SetText( "" )
		local LayerPassNum = getglobal( btn:GetName().."LayerPassNum" )
		LayerPassNum:SetText( "" )
		local Status = getglobal( btn:GetName().."Status" )
		Status:Hide()
	end
	local maxLayerMemberName = t_MiZhiTaTeam[1]["name"]
	local maxMemberLayer = t_MiZhiTaTeam[1]["layer"]
	for i = 1, TOTAL_TEAM_NUM do
		local btn = getglobal("MiZhiTaGetReadyFrameTeamMember"..i)
		local CaptainTex = getglobal( btn:GetName().."CaptainTex" )
		if t_MiZhiTaTeam[i]["position"] == TEAM_POSITION_CAPTAIN then
			CaptainTex:Show()
		end
		local Name = getglobal( btn:GetName().."Name" )
		Name:SetText( t_MiZhiTaTeam[i]["name"] )
		local LV = getglobal( btn:GetName().."LV" )
		LV:SetText( t_MiZhiTaTeam[i]["lv"] )
		local RaceName = getglobal( btn:GetName().."RaceName" )
		RaceName:SetText( t_MiZhiTaTeam[i]["career"] )
		local LayerPassNum = getglobal( btn:GetName().."LayerPassNum" )
		LayerPassNum:SetText( t_MiZhiTaTeam[i]["layer"] )
		local Status = getglobal( btn:GetName().."Status" )
		Status:Show()
		local uv = t_MiZhiTaTeam[i]["ready"] == 1 and t_ReadyTex[1]["uv"] or t_ReadyTex[2]["uv"]
		Status:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		
		if t_MiZhiTaTeam[i]["layer"] > maxMemberLayer then
			maxMemberLayer = t_MiZhiTaTeam[i]["layer"]
			maxLayerMemberName = t_MiZhiTaTeam[i]["name"]
		end
		if t_MiZhiTaTeam[i]["name"] == mainplayer:getName() then
			--设置传送与准备按钮
			if t_MiZhiTaTeam[i]["ready"] == 1 then
				MiZhiTaGetReadyFrameOkBtn:Disable()
				if mainplayer:isCaptain() then
					MiZhiTaGetReadyFrameTransBtn:Show()
				else
					MiZhiTaGetReadyFrameTransBtn:Hide()
				end
			else
				MiZhiTaGetReadyFrameOkBtn:Enable()
				MiZhiTaGetReadyFrameTransBtn:Hide()
			end
		end
	end
	--设置队友最大层数提醒
	local szInfo = string.format("你的队友#G%s#n已完成#cfa800a第%d层#n迷之塔，将进入#cfa800a第%d层#n迷之塔如果你的已记录层数少于%d层，进入后前面进度将被覆盖",	
	 maxLayerMemberName, maxMemberLayer, maxMemberLayer+1, maxMemberLayer )
	MiZhiTaGetReadyFrameRemind:SetText( szInfo, 255, 255, 190 )
	MiZhiTaGetReadyFrameTitle:SetText( "迷之塔-第".. maxMemberLayer+1 .."层" )
end
function MiZhiTaGetReadyFrameOKBtn_OnClick()
	this:Disable()
	AutoPworldManager:requestAgree( true )
end
function MiZhiTaGetReadyFrameTransBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isCaptain() then
		local num = AutoPworldManager:getMemberNum()
		for i=1,num do
			local data = AutoPworldManager:getMemberData(i-1)
			if data.Ready ~= 1 then
				ShowMidTips( "队友未准备" )
				return
			end
		end
	end
	AutoPworldManager:requestAgree( true )
end
function MiZhiTaGetReadyFrameCancelBtn_OnClick()
	AutoPworldManager:requestAgree( false )
end
function IsMiZhiTaPworldTimeOut()
	local t = getServerTime().sec
	local start = AutoPworldManager:getStartTime()
	local left = 60 - t + start
	if left > 0 then
		return false
	else
		return true
	end
end
function MiZhiTaGetReadyFrame_OnUpdate()
	local t = getServerTime().sec
	local start = AutoPworldManager:getStartTime()
	local left = 60 - t + start
	if left > 0 then
		MiZhiTaGetReadyFrameTimeFont:SetText( left .. "秒" )
	else
		this:Hide()
	end
end

function PworldPortalFrame_VigBtnProgressText_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	local maxVig	= mainplayer:getVigorMax();
	local curVig	= mainplayer:getVigor();
	PworldPortalFrame_VigBtnProgressText:SetText( "活力值："..curVig.."/"..maxVig )
end
--获取活力值
function PworldPortalFrameGetVig_OnLoad()
	PworldPortalFrameGetVig:SetText( "#L#G获得活力值#n", 255,255,190 );
end
function PworldPortalFrameGetVig_OnClick()
	GeiVigFrame:Show()
end
function GeiVigFrame_OnLoad()
	GeiVigFrameText:SetText( "\t如何增加“活力值”：\n\t\t\t1. 通过XXXX活动获得“活力值道具”\n\t\t\t2. 进入幻境击杀小怪或BOSS获取活力值\n\t\t\t3. 通过商城购买“活力值道具”", 255,255,190 )
end
