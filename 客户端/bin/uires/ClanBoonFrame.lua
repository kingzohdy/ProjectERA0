local curShowType = 1;
local WEEK_GOAL_NUM = 4;
local RECORD_PAGE_LIST = 6;
local nPageIdx = 1;
local MAX_ASSIGN_LIST = 7;
local t_BonusDesc = {
			[BOON_ADD_TYPE_BINGDAO_DEF_SUCC_TOLUA]	= "防守冰岛据点成功，公会工资提成+",
			[BOON_ADD_TYPE_BINGDAO_DEF_FAIL_TOLUA]	= "失去冰岛据点，公会工资提成",
			[BOON_ADD_TYPE_BINGDAO_ATK_SUCC_TOLUA]	= "占领冰岛据点成功，公会工资提成+",
			[BOON_ADD_TYPE_XUEYUAN_DEF_SUCC_TOLUA]	= "防守银风雪原据点成功，公会工资提成+",
			[BOON_ADD_TYPE_XUEYUAN_DEF_FAIL_TOLUA]	= "失去银风雪原据点，公会工资提成",
			[BOON_ADD_TYPE_XUEYUAN_ATK_SUCC_TOLUA]	= "占领银风雪原据点成功，公会工资提成+",
			[BOON_ADD_TYPE_SHAMO_DEF_SUCC_TOLUA]	= "防守流金沙漠据点成功，公会工资提成+",
			[BOON_ADD_TYPE_SHAMO_DEF_FAIL_TOLUA]	= "失去流金沙漠据点，公会工资提成",
			[BOON_ADD_TYPE_SHAMO_ATK_SUCC_TOLUA]	= "占领流金沙漠据点成功，公会工资提成+",
			[BOON_ADD_TYPE_LUORI_DEF_SUCC_TOLUA]	= "防守落日荒野据点成功，公会工资提成+",
			[BOON_ADD_TYPE_LUORI_DEF_FAIL_TOLUA]	= "失去落日荒野据点，公会工资提成",
			[BOON_ADD_TYPE_LUORI_ATK_SUCC_TOLUA]	= "占领落日荒野据点成功，公会工资提成+",
			[BOON_ADD_TYPE_YULIN_DEF_SUCC_TOLUA]	= "防守越雾雨林据点成功，公会工资提成+",
			[BOON_ADD_TYPE_YULIN_DEF_FAIL_TOLUA]	= "失去越雾雨林据点，公会工资提成",
			[BOON_ADD_TYPE_YULIN_ATK_SUCC_TOLUA]	= "占领越雾雨林据点成功，公会工资提成+",
			[BOON_ADD_TYPE_MINGSHA_DEF_SUCC_TOLUA]	= "防守鸣沙陷地据点成功，公会工资提成+",
			[BOON_ADD_TYPE_MINGSHA_DEF_FAIL_TOLUA]	= "失去鸣沙陷地据点，公会工资提成",
			[BOON_ADD_TYPE_MINGSHA_ATK_SUCC_TOLUA]	= "占领鸣沙陷地据点成功，公会工资提成+",
			[BOON_ADD_TYPE_SHIJIAN_DEF_SUCC_TOLUA]	= "防守时间城成功，公会工资提成+",
			[BOON_ADD_TYPE_SHIJIAN_DEF_FAIL_TOLUA]	= "失去时间城，公会工资提成",
			[BOON_ADD_TYPE_SHIJIAN_ATK_SUCC_TOLUA]	= "占领时间城成功，公会工资提成+",
			}

function OpenClanBoonFrame( npcId )
	local clanLv = GameClanManager:getClanViewer().m_clanInfo.m_nClanLevel;
	if clanLv < CLAN_BOON_LEVEL_TOLUA then
		local boonData = ClanBoon:getClanBoonData();
		if boonData.Assign.BindGlod == 0 then
			return AddGameMiddleTips2( "您的公会至少达到" .. CLAN_BOON_LEVEL_TOLUA .. "级才可以领取官员福利" );
		end
	end
	if not ClanBoonFrame:IsShown() then
		ClanBoonFrame:Show();
		util.testNpcDialogDistance(npcId, ClanBoonFrame);
	else
		ClanBoonFrame:Show();
	end
end

local function DisCheckAllTypeBtn()
	ClanBoonFrameCondType:DisChecked();
	ClanBoonFrameAssignType:DisChecked();
end

function ClanBoonFrameTypeBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "确认分配" then
		return;
	end
	DisCheckAllTypeBtn();
	this:Checked();
	curShowType = this:GetClientID();
	updateClanBoonFrame();
end

function ClanBoonFrame_Show()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	DisCheckAllTypeBtn();
	ClanBoonFrameCondType:Checked();
	InitAssignEditData();
	updateClanBoonFrame();
end

function ClanBoonFrame_Hide()
	curShowType = 1;
	ClanBoonFrameCondBtnRecordFrame:Hide();
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "确认分配" then
		MessageBoxFrame:Hide();
	end
end

local assignDesc = [[#cffffbe分配规则：#n\n
1.本周可以分配上周获取的福
   利，每周只能分配与领取一
   次，逾期奖励作废。
2.会长可以领取固定金券并分
   配系统奖励的金券福利。
3.会长分配结束后，只有指定
   角色可以获取本周的奖励，
   奖励不会随公会的职位变动
   而变动。]];

function ClanBoonFrame_OnLoad()
	this:RegisterEvent( "GE_CLAN_BOON_UPDATE" );
	this:RegisterEvent( "GE_CLAN_DISABLEFRAME" );
	this:RegisterEvent( "GE_CLAN_LEVEL_UPDATE" );
	this:RegisterEvent( "GE_CLAN_BOON_ALL" );
	this:RegisterEvent( "GE_CLAN_POSITION_CHANGE" );
	this:RegisterEvent( "GE_CLAN_MEMBER_DEL" );
	ClanBoonFrameAssignBtnRich:SetText( assignDesc, 160, 160, 90 );

	for i = 1, MAX_ASSIGN_LIST do
		local btn = getglobal( "ClanBoonFrameAssignBtnList" ..i );
		local t_Tex = {
			btn:GetName() .. "JobBkg", btn:GetName() .. "NameBkg", btn:GetName() .. "MoneyBkg",
			btn:GetName() .. "StateBkg", };
		for j=1,#t_Tex do
			local tex = getglobal( t_Tex[j] )
			if math.mod( i,2 ) == 0 then
				tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
			else
				tex:ChangeTextureTemplate( "BlackCategoryBkg" );
			end
		end
	end

	ClanBoonFrameCondBtnWeekFont2:SetText("会长可以获取500金券的固定工资奖励\n公会官员最高可以获取总计5000金券的奖励");
end

function ClanBoonFrame_OnEvent()
	if arg1 == "GE_CLAN_BOON_UPDATE" then
		if this:IsShown() then
			updateClanBoonFrame();
		end
		if ClanBoonFrameCondBtnRecordFrame:IsShown() then
			updateCondRecordPage();
		end
	elseif arg1 == "GE_CLAN_BOON_ALL" or arg1 == "GE_CLAN_POSITION_CHANGE" or
		arg1 == "GE_CLAN_LEVEL_UPDATE" or arg1 == "GE_CLAN_MEMBER_DEL" then
		if this:IsShown() then
			updateClanBoonFrame();
			if curShowType == 2 then
				InitAssignEditData();
			end
		end
		if ClanBoonFrameCondBtnRecordFrame:IsShown() then
			updateCondRecordPage();
		end
	elseif arg1 == "GE_CLAN_DISABLEFRAME" then
		this:Hide();
	end
end

function updateClanBoonFrame()
	if curShowType == 1 then
		updateClanCondInfo();
		if not ClanBoonFrameCondBtn:IsShown() then
			ClanBoonFrameCondBtn:Show();
		end
	else
		updateClanAssignInfo();
		if not ClanBoonFrameAssignBtn:IsShown() then
			ClanBoonFrameAssignBtn:Show();
		end
	end
end

function updateClanCondInfo()
	if ClanBoonFrameAssignBtn:IsShown() then
		ClanBoonFrameAssignBtn:Hide();
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local boonData = ClanBoon:getClanBoonData();

	local t_state = { { "未达成", 120, 120, 120 }, { "已达成", 0, 220, 0 }, };
	local idx = boonData.Assign.BindGlod > 0 and 2 or 1;
	ClanBoonFrameCondBtnStateFont:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnStateFont:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );

	if ClanBoonFrameCondBtnClanFont:GetTextExtentWidth( mainplayer:getClanName() ) > 96 then
		ClanBoonFrameCondBtnClanFont:SetText( "公会名称：" .. string.sub(mainplayer:getClanName(),0,12) .. "..." )
	else
		ClanBoonFrameCondBtnClanFont:SetText( "公会名称：" .. mainplayer:getClanName() );
	end
	if ClanBoonFrameCondBtnNameFont:GetTextExtentWidth( mainplayer:getName() ) > 96 then
		ClanBoonFrameCondBtnNameFont:SetText( "角色名称：" .. string.sub(mainplayer:getName(),0,12) .. "..." )
	else
		ClanBoonFrameCondBtnNameFont:SetText( "角色名称：" .. mainplayer:getName() );
	end
	ClanBoonFrameCondBtnPosFont:SetText( "公会职务：" .. GetPosition(mainplayer:getClanPosition()) );

	local flag = 0;
	local money = 0;
	--local moneyType = mainplayer:getClanPosition() == CLAN_POS_PRESIDENT and ATTR_ID_GODCOIN or ATTR_ID_BINDGODCOIN;
	local moneyType = ATTR_ID_BINDGODCOIN;
	local psdFlag = 1;
	if mainplayer:getName() == boonData.Assign.AssignGlod.Name then
		money = boonData.Assign.AssignGlod.Money;
		flag  = boonData.Assign.AssignGlod.Flag;
		--moneyType = ATTR_ID_GODCOIN;
		psdFlag = 0;
	else
		flag = 1;
	end
	
	if flag > 0 then
		for i=1, boonData.Assign.Office do
			if mainplayer:getName() == boonData.Assign.AssignBindGlod[i-1].Name then
				money = boonData.Assign.AssignBindGlod[i-1].Money;
				flag  = boonData.Assign.AssignBindGlod[i-1].Flag;
				--moneyType = ATTR_ID_BINDGODCOIN;
				psdFlag = 1;
				break;
			end
		end
	end

	if money <= 0 and mainplayer:getClanPosition() > CLAN_POS_NORMAL and mainplayer:getClanPosition() ~= CLAN_POS_PRESIDENT
		and boonData.Assign.BindGlod > 0 and boonData.Assign.Office == 0 then
		ClanBoonFrameCondBtnSalaryFont:SetText( "等待分配" );
	else
		ClanBoonFrameCondBtnSalaryFont:SetText( money .. GetMoneyDesc( moneyType ) );
	end

	ClanBoonFrameCondBtnGetSalary:SetClientUserData( 0, money );
	ClanBoonFrameCondBtnGetSalary:SetClientUserData( 1, psdFlag );
	if flag == 0 and money > 0 then
		ClanBoonFrameCondBtnGetSalary:Enable();
	else
		ClanBoonFrameCondBtnGetSalary:Disable();
	end
	ClanBoonFrameCondBtnGetSalary:SetText( flag > 0 and "已  领  取" or "领取上周工资" );

	--达成条件
	local count = 0;
	idx = boonData.Cond.Tech < CLAN_TECH_NUM_TOLUA and 1 or 2;
	count = idx > 1 and count + 1 or count;
	ClanBoonFrameCondBtnTechBtnFont1:SetText( string.format("1.获得科技点总数达到(%d/%d)点", (idx > 1 and CLAN_TECH_NUM_TOLUA or boonData.Cond.Tech), CLAN_TECH_NUM_TOLUA) );
	ClanBoonFrameCondBtnTechBtnFont2:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnTechBtnFont2:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );

	local ave = boonData.Cond.AveNum > 0 and math.floor( boonData.Cond.Ave / boonData.Cond.AveNum ) or 0;
	idx = ave < CLAN_ONLINE_AVE_TOLUA and 1 or 2;
	count = idx > 1 and count + 1 or count;
	ClanBoonFrameCondBtnOnlineBtnFont1:SetText( string.format("2.平均在线人数(%d/%d)人", (idx > 1 and CLAN_ONLINE_AVE_TOLUA or ave), CLAN_ONLINE_AVE_TOLUA) );
	ClanBoonFrameCondBtnOnlineBtnFont2:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnOnlineBtnFont2:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );

	idx = boonData.Cond.Contri < CLAN_CONTRI_NUM_TOLUA and 1 or 2;
	count = idx > 1 and count + 1 or count;
	ClanBoonFrameCondBtnContriBtnFont1:SetText( string.format("3.获得贡献度达到(%d/%d)点", (idx > 1 and CLAN_CONTRI_NUM_TOLUA or boonData.Cond.Contri), CLAN_CONTRI_NUM_TOLUA) );
	ClanBoonFrameCondBtnContriBtnFont2:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnContriBtnFont2:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );

	idx = boonData.Cond.Gold < CLAN_GOLD_NUM1_TOLUA and 1 or 2;
	count = idx > 1 and count + 1 or count;
	ClanBoonFrameCondBtnGoldBtn1Font1:SetText( string.format("4.公会成员累计充值(%d/%d)金币", (idx > 1 and CLAN_GOLD_NUM1_TOLUA or boonData.Cond.Gold), CLAN_GOLD_NUM1_TOLUA) );
	ClanBoonFrameCondBtnGoldBtn1Font2:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnGoldBtn1Font2:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );
	
	ClanBoonFrameCondBtnProcessFont:SetText( string.format( "完成度：%d%%", math.floor(count*100/WEEK_GOAL_NUM) ) );
	
	--[[
	idx = count < WEEK_GOAL_NUM and 1 or 2;
	ClanBoonFrameCondBtnCountBtnFont1:SetText( "1.达成所有“领取金券条件”" );
	ClanBoonFrameCondBtnCountBtnFont2:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnCountBtnFont2:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );

	idx = boonData.Cond.Gold < CLAN_GOLD_NUM2_TOLUA and 1 or 2;
	ClanBoonFrameCondBtnGoldBtn2Font1:SetText( string.format("2.公会成员累计充值(%d/%d)金币", (idx > 1 and CLAN_GOLD_NUM2_TOLUA or boonData.Cond.Gold), CLAN_GOLD_NUM2_TOLUA) );
	ClanBoonFrameCondBtnGoldBtn2Font2:SetText( t_state[idx][1] );
	ClanBoonFrameCondBtnGoldBtn2Font2:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );
	--]]

	count = 0;
	for i=1, boonData.Cond.EventNum do
		count = count + boonData.Cond.Event[i-1].Per;
	end
	ClanBoonFrameCondBtnBonusFont:SetText( string.format( "加成值：%d%%", count > 0 and count or 0 ) );
end

function updateClanAssignInfo()
	if ClanBoonFrameCondBtn:IsShown() then
		ClanBoonFrameCondBtn:Hide();
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local boonData = ClanBoon:getClanBoonData();
	--ClanBoonFrameAssignBtnGoldFont:SetText( boonData.Assign.Glod .. "金币" );
	ClanBoonFrameAssignBtnGoldFont:SetText( boonData.Assign.Glod .. "金券" );
	ClanBoonFrameAssignBtnBindGoldFont:SetText( boonData.Assign.BindGlod .. "金券" );

	if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
		ClanBoonFrameAssignBtnResetBtn:Show();
		ClanBoonFrameAssignBtnConfirmBtn:Show();
	else
		ClanBoonFrameAssignBtnResetBtn:Hide();
		ClanBoonFrameAssignBtnConfirmBtn:Hide();
		if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "确认分配" then
			MessageBoxFrame:Hide();
		end
	end
	CheckCanEditAssign( mainplayer:getClanPosition(), boonData.Assign.Office, boonData.Assign.BindGlod );

	local t_state = { { "已领取", 120, 120, 120 }, { "未领取", 0, 220, 0 }, { "未分配", 120, 120, 120 }, };
	if boonData.Assign.Office > 0 then
		local money = 0;
		money = money + SetAssignListInfo( 0, true );
		for i=1, boonData.Assign.Office do
			money = money + SetAssignListInfo( i, true );
		end

		for i=boonData.Assign.Office + 1, MAX_ASSIGN_LIST do
			local btn = getglobal( "ClanBoonFrameAssignBtnList" .. i );
			local font = getglobal( btn:GetName() .. "JobFont" );
			font:SetText( "" );
			font = getglobal( btn:GetName() .. "NameFont" );
			font:SetText( "" );
			font = getglobal( btn:GetName() .. "MoneyFont" );
			font:SetText( "" );
			font = getglobal( btn:GetName() .. "StateFont" );
			font:SetText( "" );
		end

		ClanBoonFrameAssignBtnUnusedFont:SetText( "剩余分配：" .. (boonData.Assign.BindGlod - money > 0 and boonData.Assign.BindGlod - money or 0) );
		ClanBoonFrameAssignBtnUsedFont:SetText( "已经分配：" .. money );
		SetAssignBtnState( false, boonData.Assign.BindGlod );
	else
		local num = GameClanManager:getClanViewer().m_clanInfo.m_nClanLevel + 1;
		for i = 0, MAX_ASSIGN_LIST do
			local btn = getglobal( "ClanBoonFrameAssignBtnList" .. i );
			local pos = btn:GetClientID();
			local font = getglobal( btn:GetName() .. "JobFont" );
			font:SetText( i > num and "" or GetPosition(pos) );
			font = getglobal( btn:GetName() .. "NameFont" );
			if i > num then
				font:SetText( "" );
			else
				local szText = GameClanManager:getMemberNameByPos(pos);
				if font:GetTextExtentWidth( szText ) > 102 then
					szText = string.sub(szText,0,10) .. "...";
				end
				font:SetText( szText );
			end
			if font:GetText() == "" then
				local edit = getglobal( btn:GetName() .. "Num" );
				edit:Hide();
				font = getglobal( btn:GetName() .. "MoneyFont" );
				font:SetText( "" );
			else
				font = getglobal( btn:GetName() .. "MoneyFont" );
				--font:SetText( pos == CLAN_POS_PRESIDENT and (boonData.Assign.Glod .. "金币") or "        金券" );
				szText = mainplayer:getClanPosition() ~= CLAN_POS_PRESIDENT and "" or "        金券";
				font:SetText( pos == CLAN_POS_PRESIDENT and (boonData.Assign.Glod .. "金券") or szText );
			end
			font = getglobal( btn:GetName() .. "StateFont" );
			local idx = boonData.Assign.AssignGlod.Flag > 0 and 1 or 2;
			idx = i > 0 and 3 or idx;
			font:SetText( i > num and "" or t_state[idx][1] );
			font:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );
		end
		SetAssignBtnState( true, boonData.Assign.BindGlod );
	end
end

function SetAssignListInfo( i )
	local boonData = ClanBoon:getClanBoonData();
	local data = boonData.Assign;
	local btn = getglobal( "ClanBoonFrameAssignBtnList" .. i );
	local font = getglobal( btn:GetName() .. "JobFont" );
	font:SetText( GetPosition( i > 0 and data.AssignBindGlod[i-1].Pos or data.AssignGlod.Pos ) );
	font = getglobal( btn:GetName() .. "NameFont" );
	font:SetText( i > 0 and data.AssignBindGlod[i-1].Name or data.AssignGlod.Name );
	font = getglobal( btn:GetName() .. "MoneyFont" );
	--font:SetText( i > 0 and (data.AssignBindGlod[i-1].Money .. "金券") or (data.AssignGlod.Money .. "金币") );
	font:SetText( i > 0 and (data.AssignBindGlod[i-1].Money .. "金券") or (data.AssignGlod.Money .. "金券") );
	font = getglobal( btn:GetName() .. "StateFont" );
	local flag = i > 0 and data.AssignBindGlod[i-1].Flag or data.AssignGlod.Flag;
	local idx = flag > 0 and 1 or 2;
	local t_state = { { "已领取", 120, 120, 120 }, { "未领取", 0, 220, 0 }, { "未分配", 120, 120, 120 }, };
	font:SetText( t_state[idx][1] );
	font:SetTextColor( t_state[idx][2], t_state[idx][3], t_state[idx][4] );
	return i > 0 and data.AssignBindGlod[i-1].Money or 0;
end

function CheckCanEditAssign( pos, state, money )
	if pos ~= CLAN_POS_PRESIDENT or state > 0 or money <= 0 then
		for i = 1, MAX_ASSIGN_LIST do
			local edit = getglobal( "ClanBoonFrameAssignBtnList" .. i .. "Num" );
			edit:Hide();
		end
	else
		local num = GameClanManager:getClanViewer().m_clanInfo.m_nClanLevel + 1;
		for i = 1, MAX_ASSIGN_LIST do
			local edit = getglobal( "ClanBoonFrameAssignBtnList" .. i .. "Num" );
			if i > num then
				edit:Hide();
			else
				edit:Show();
			end
		end
	end
end

function SetAssignBtnState( bAssign, money )
	if bAssign and money > 0 then
		ClanBoonFrameAssignBtnResetBtn:Enable();
		ClanBoonFrameAssignBtnConfirmBtn:Enable();
	else
		ClanBoonFrameAssignBtnResetBtn:Disable();
		ClanBoonFrameAssignBtnConfirmBtn:Disable();
	end
	ClanBoonFrameAssignBtnResetBtn:SetClientUserData( 0, money );
end

function ClanBoonFrameCondBtnGetSalary_OnClick()
	ClanBoon:requestClanBoonGet( this:GetClientUserData(0), this:GetClientUserData(1) );
end

function ClanBoonFrameCondBtnSalaryBonus_OnClick()
	if not ClanBoonFrameCondBtnRecordFrame:IsShown() then
		ClanBoonFrameCondBtnRecordFrame:Show();
	else
		ClanBoonFrameCondBtnRecordFrame:Hide();
	end
end

function getBonusTime( nTime )
	return os.date("%Y-%m-%d  %H:%M", nTime);
end

function updateCondRecordPage()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local boonData = ClanBoon:getClanBoonData();
	local rich = getglobal( "ClanBoonFrameCondBtnRecordFrameRich" );
	local szText = "";
	for i=nPageIdx, nPageIdx + RECORD_PAGE_LIST - 1 do
		if i > boonData.Cond.EventNum then
			break;
		end
		szText = szText .. "#cffffbe" .. getBonusTime(boonData.Cond.Event[i-1].Time) .. "\n#c00e600" .. mainplayer:getClanName() .. "#n" .. t_BonusDesc[boonData.Cond.Event[i-1].ID] .. boonData.Cond.Event[i-1].Per .. "%\n";
	end
	rich:SetText( szText, 160, 160, 90 );
end

function ClanBoonFrameCondBtnRecordFrame_OnShow()
	updateCondRecordPage();
end

function ClanBoonFrameCondBtnRecordFrame_OnHide()
	nPageIdx = 1
end

function ClanBoonFrameCondBtnRecordFrameLeft_OnClick()
	nPageIdx = ( nPageIdx - RECORD_PAGE_LIST > 0 ) and nPageIdx - RECORD_PAGE_LIST or 1;
	updateCondRecordPage();
end

function ClanBoonFrameCondBtnRecordFrameRight_OnClick()
	local boonData = ClanBoon:getClanBoonData();
	nPageIdx = ( nPageIdx + RECORD_PAGE_LIST > boonData.Cond.EventNum ) and nPageIdx or nPageIdx + RECORD_PAGE_LIST;
	updateCondRecordPage();
end

function InitAssignEditData()
	for i = 1, MAX_ASSIGN_LIST do
		local font = getglobal( "ClanBoonFrameAssignBtnList" .. i .. "NumEdit" );
		font:SetText(0);
	end
	local boonData = ClanBoon:getClanBoonData();
	ClanBoonFrameAssignBtnUnusedFont:SetText( "剩余分配：" .. boonData.Assign.BindGlod );
	ClanBoonFrameAssignBtnUsedFont:SetText( "已经分配：0" );
end

function ClanBoonFrameAssignBtn_Hide()
	ClearFocus();
end

function GetAssignCount( name )
	local money = 0;
	for i = 1, MAX_ASSIGN_LIST do
		local font = getglobal( "ClanBoonFrameAssignBtnList" .. i .. "NumEdit" );
		if font:GetName() ~= name and font:GetText() ~= "" then
			money = money + tonumber(font:GetText());
		end
	end
	return money;
end

local function updateAssignMoney()
	local money = GetAssignCount( "" );
	local boonData = ClanBoon:getClanBoonData();
	ClanBoonFrameAssignBtnUnusedFont:SetText( "剩余分配：" .. (boonData.Assign.BindGlod - money > 0 and boonData.Assign.BindGlod - money or 0) );
	ClanBoonFrameAssignBtnUsedFont:SetText( "已经分配：" .. money );
end

function ClanBoonFrameEdit_OnFocusLost()
	local totalMoney = ClanBoonFrameAssignBtnResetBtn:GetClientUserData(0);
	if this:GetText() == "" or totalMoney <= 0 then
		this:SetText(0);
		return;
	end

	local money = tonumber(this:GetText());
	if money == nil or money < 0 then
		this:SetText(0);
		return;
	end

	local assignMoney = GetAssignCount( this:GetName() );
	local canUseMoney = totalMoney - assignMoney;
	if canUseMoney <= 0 then
		this:SetText(0);
	else
		money = money > canUseMoney and canUseMoney or money;
		this:SetText(money);
	end

	updateAssignMoney();
end

function ClanBoonFrameEdit_OnTabPressed()
	local idx = string.sub( this:GetName(), string.find(this:GetName(),"%d") );
	idx = idx + 1 > MAX_ASSIGN_LIST and 1 or idx + 1;
	ClearFocus();
	SetFocusFrame( "ClanBoonFrameAssignBtnList" .. idx .. "NumEdit" );
end

local STEP_VALUE = 10;
function ClanBoonFrameScrollUp_OnClick()
	local totalMoney = ClanBoonFrameAssignBtnResetBtn:GetClientUserData(0);
	local idx = string.sub( this:GetName(), string.find(this:GetName(),"%d") );
	local font = getglobal( "ClanBoonFrameAssignBtnList" .. idx .. "NumEdit" );
	if font:GetText() == "" or totalMoney <= 0 then
		font:SetText(0);
		return;
	end

	local money = tonumber(font:GetText());
	if money == nil or money < 0 then
		font:SetText(0);
		return;
	end

	local assignMoney = GetAssignCount( this:GetName() );
	local canUseMoney = totalMoney - assignMoney;
	if canUseMoney > 0 then
		font:SetText( STEP_VALUE > canUseMoney and money + canUseMoney or money + STEP_VALUE );
		updateAssignMoney();
	end
end

function ClanBoonFrameScrollDown_OnClick()
	local totalMoney = ClanBoonFrameAssignBtnResetBtn:GetClientUserData(0);
	local idx = string.sub( this:GetName(), string.find(this:GetName(),"%d") );
	local font = getglobal( "ClanBoonFrameAssignBtnList" .. idx .. "NumEdit" );
	if font:GetText() == "" or totalMoney <= 0 then
		font:SetText(0);
		return;
	end

	local money = tonumber(font:GetText());
	if money == nil or money <= 0 then
		font:SetText(0);
		return;
	end

	font:SetText( money - STEP_VALUE > 0 and money - STEP_VALUE or 0 );
	updateAssignMoney();
end

function ClanBoonFrameAssignBtnReset_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "确认分配" then
		return;
	end
	InitAssignEditData();
end

function ClanBoonFrameAssignBtnConfirm_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "确认分配" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:getClanPosition() ~= CLAN_POS_PRESIDENT then
		return ShowMidTips("只有会长能对工资进行分配");
	end
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		AddGameMiddleTips2("请先认证安全锁密码");
		return false;
	end

	local boonData = ClanBoon:getClanBoonData();
	if boonData.Assign.Office > 0 then
		return ShowMidTips("这周工资已经分配完毕，无法进行再次分配");
	end
	if boonData.Assign.BindGlod <= 0 then
		return ShowMidTips("当前没有可分配的工资");
	end

	local money = SetAssignCondData();
	local leftMoney = boonData.Assign.BindGlod - money;
	if leftMoney > 0 then
		MessageBoxOK("确认分配","您还有#c00dc00" .. leftMoney .. "#n金券没有分配，\n请将剩余金券分配后再点击确认分配");
	else
		MessageBox("确认分配","确认分配后本周不能再修改分配比例，确认分配？");
	end
	MessageBoxFrame:SetClientUserData( 0, money );
	MessageBoxFrame:SetClientUserData( 1, leftMoney );
end

function SetAssignCondData()
	ClanBoon:resetAssignData();
	local assignData = ClanBoon:getBoonAssignData();
	local count = 0;
	local money = 0;
	local name = "ClanBoonFrameAssignBtnList";
	for i=1, BOON_GET_MAX_TOLUA do
		local btn = getglobal( name .. i );
		local nameFont = getglobal( name .. i .. "NameFont" );
		local moneyFont = getglobal( name .. i .. "NumEdit" );
		if nameFont:GetText() ~= "" and tonumber(moneyFont:GetText()) > 0 then
			ClanBoon:setBoonAssignDataByIndex( count, nameFont:GetText(), btn:GetClientID(), tonumber(moneyFont:GetText()) );
			money = money + tonumber(moneyFont:GetText());
			count = count + 1;
		end
	end
	assignData.Office = count;
	return money;
end