
function GMToolsFrameAddSpeedChk_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		if GMToolsFrame_AddSpeedChk:GetCheckState() then
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//applystatus 0 1017 10 65535" );
		else
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//applystatus 0 1017 10 0" );
		end
	end
end

function GMToolsFrameHideChk_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		if GMToolsFrame_HideChk:GetCheckState() then
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//hideon" );
		else
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//hideoff" );
		end
	end
end

function GMToolsFrameEraChk_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		if GMToolsFrame_EraChk:GetCheckState() then
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//earon" );
		else
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//earoff" );
		end
	end
end

function GMToolsFrameInvinChk_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		if GMToolsFrame_InvinChk:GetCheckState() then
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//applystatus 0 1051 1 65535" );
		else
			ChatManager:requestSendActionChat( CL_CHAT_SAY, "", "//applystatus 0 1051 1 0" );
		end
	end
end

function GMToolsFrameMoveBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local mapid = GMToolsFrameSelectBtn:GetClientUserData(0);
		local posx = tonumber( GMToolsFrame_PosXEdit:GetText() );
		if posx == 0 or posx == nil then
			ShowMidTips("输入坐标x");
			return
		end
		local posy = tonumber( GMToolsFrame_PosYEdit:GetText() );
		if posy == 0 or posy == nil then
			ShowMidTips("输入坐标y");
			return
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY, "",string.format("//goto %d %d %d",mapid,posx,posy) );
	end
end

function GMToolsFrameNearBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local playerName = GMToolsFrame_NearNameEdit:GetText();
		if playerName == "" or playerName == nil then
			ShowMidTips("输入跟随玩家名");
			return;
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY, "","//near " .. playerName );
	end
end

function GMToolsFrameCallBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local playerName = GMToolsFrame_CallNameEdit:GetText();
		if playerName == "" or playerName == nil then
			ShowMidTips("输入召唤玩家名");
			return;
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY, "","//callrole " .. playerName );
	end
end

function GMToolsFrameTmpMachineBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local szID = GMToolsFrame_TmpMachineIDEdit:GetText();
		if szID == "" or szID == nil then
			ShowMidTips("输入机械ID");
			return;
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY,"","//ridetmpitem " .. szID );
	end
end

function GMToolsFrameMonsterBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local szID = GMToolsFrame_MonsterIDEdit:GetText();
		if szID == "" or szID == nil then
			ShowMidTips("输入怪物ID");
			return;
		end
		local szNum = GMToolsFrame_MonsterNumEdit:GetText();
		if szNum == "" or szNum == nil then
			ShowMidTips("输入怪物数量");
			return;
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY,"","//monster ".. szID .." " .. szNum );
	end
end

function GMToolsFrameDropItemBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local szID = GMToolsFrame_DropItemIDEdit:GetText();
		if szID == "" or szID == nil then
			ShowMidTips("输入道具ID");
			return;
		end
		local szNum = GMToolsFrame_DropItemNumEdit:GetText();
		if szNum == "" or szNum == nil then
			ShowMidTips("输入道具数量");
			return;
		end
		local szTime = GMToolsFrame_DropItemCollectEdit:GetText();
		if szTime == "" or szTime == nil then
			ShowMidTips("输入采集时间");
			return;
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY,"","//dropitem ".. szID .." " .. szNum .. " " ..szTime );
	end
end

function GMToolsFrameDropMoneyBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local szID = GMToolsFrame_DropMoneyIDEdit:GetText();
		if szID == "" or szID == nil then
			ShowMidTips("输入银币");
			return;
		end
		local szNum = GMToolsFrame_DropMoneyNumEdit:GetText();
		if szNum == "" or szNum == nil then
			ShowMidTips("输入银币堆数");
			return;
		end
		ChatManager:requestSendActionChat( CL_CHAT_SAY,"","//dropmoney ".. szID .." " .. szNum );
	end	
end

function GMToolsFrameAddItemBtn_OnClick()
	if getGameState() == CLIENT_STATE_INWORLD then
		local szID = GMToolsFrame_AddItemIDEdit:GetText();
		if szID == "" or szID == nil then
			ShowMidTips("输入道具ID");
			return;
		end
		local szNum = GMToolsFrame_AddItemNumEdit:GetText();
		if szNum == "" or szNum == nil then
			ShowMidTips("输入道具数量");
			return;
		end
		
		ChatManager:requestSendActionChat( CL_CHAT_SAY,"","//additem ".. szID .." " .. szNum );
	end	
end

function GMToolsFrame_OnShow()
	GMToolsFrameSelectBtnFont:SetText( WipeOffNumber( getMapName( MapShower:getRealMapId() ) ) );
	GMToolsFrameSelectBtn:SetClientUserData( 0,MapShower:getRealMapId() );
end

function GMToolsFrame_OnHide()
	GMSelectNameFrame:Hide();
end

local MAX_SELECT_NAME = 10
function GMToolsFrameSelectBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end

	if GMSelectNameFrame:IsShown() then
		GMSelectNameFrame:Hide();
	else		
		GMSelectNameFrame:Show();
		--GMSelectNameFrame:SetPoint( "topleft", "GMToolsFrameSelectBtn", "bottomleft", 0,0 );
	end
end

function UpdateGMSelectNameFrame( nStartLine )
	for i = 1, MAX_SELECT_NAME do
		local button =  getglobal( "GMSelectNameFrameBtn" .. i );
		local font	 = 	getglobal( "GMSelectNameFrameBtn" .. i .. "AreaName" );
		button:Disable();
		button:SetClientUserData( 0,0 );
		font:SetText( "" );
	end

	local nIconIndex = 1;
	local worldArea = getWorldArea();
	for i = nStartLine, table.getn( worldArea ) do
		if nIconIndex > MAX_SELECT_NAME then
			break;
		end
		if worldArea[i].MapID < MIN_COPY_ID then
			local button =  getglobal( "GMSelectNameFrameBtn" .. nIconIndex );
			local font	 = 	getglobal( "GMSelectNameFrameBtn" .. nIconIndex .. "AreaName" );
			button:Enable();
			button:SetClientUserData( 0,worldArea[i].MapID );
			font:SetText( WipeOffNumber( worldArea[i].MapName ) );
			nIconIndex = nIconIndex + 1;
		end
	end
end

function GMSelectNameFrame_OnShow()
	local slider = getglobal( "GMSelectNameFrame_ScrollBar" );	
	slider:SetValue( 0 );
	local worldArea = getWorldArea();
	SetScrollBar( "GMSelectNameFrame_ScrollBar", MAX_SELECT_NAME, table.getn( worldArea ) );
	UpdateGMSelectNameFrame( slider:GetValue() + 1 );
	GMSelectNameFrame:SetPoint( "topleft","UIParent","topleft",GMToolsFrameSelectBtn:GetRealLeft(),GMToolsFrameSelectBtn:GetRealBottom() );
end

function GMSelectNameFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "GMSelectNameFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function GMSelectNameFrame_ScrollDownBtn_OnClick()
	local slider = getglobal("GMSelectNameFrame_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue	= slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function GMSelectNameFrame_ScrollBar_OnValueChanged()
	local slider	= getglobal( "GMSelectNameFrame_ScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateGMSelectNameFrame( slider:GetValue() + 1 );
	end
end

function GMSelectNameBtn_OnClick()
	local mapid = this:GetClientUserData( 0 );
	GMToolsFrameSelectBtnFont:SetText( WipeOffNumber( getMapName( mapid ) ) );
	GMToolsFrameSelectBtn:SetClientUserData( 0,mapid );	
	GMSelectNameFrame:Hide();
end

-- 颜色选择按钮  2012.04.10
local setColorRGB = {
		{ r = 0,   g = 0,   b = 0 }, 
		{ r = 128, g = 128, b = 128 }, 
		{ r = 128, g = 0,   b = 0 }, 
		{ r = 128, g = 128, b = 0 }, 
		{ r = 0,   g = 128, b = 0 }, 
		{ r = 0,   g = 128, b = 128 }, 
		{ r = 0,   g = 0,   b = 128 }, 
		{ r = 128, g = 0,   b = 128 }, 
		{ r = 128, g = 128, b = 64 }, 
		{ r = 0,   g = 64,  b = 64 }, 
		{ r = 0,   g = 128, b = 255 }, 
		{ r = 0,   g = 64,  b = 128 }, 
		{ r = 128, g = 0,   b = 255 }, 
		{ r = 128, g = 64,  b = 0 },
		{ r = 255, g = 255, b = 255 }, 
		{ r = 192, g = 192, b = 192 }, 
		{ r = 255, g = 0,   b = 0 }, 
		{ r = 255, g = 255, b = 0 }, 
		{ r = 0,   g = 255, b = 0 }, 
		{ r = 0,   g = 255, b = 255 }, 
		{ r = 0,   g = 0,   b = 255 }, 
		{ r = 255, g = 0,   b = 255 }, 
		{ r = 255, g = 255, b = 128 }, 
		{ r = 0,   g = 255, b = 128 }, 
		{ r = 128, g = 255, b = 255 }, 
		{ r = 128, g = 255, b = 255 }, 
		{ r = 255, g = 0,   b = 128 }, 
		{ r = 255, g = 128, b = 64 },
	}; -- 颜色的RGB的值
function GMToolsFrame_OnLoad( )
	for i = 1, 28 do
		local button_color_set = getglobal( "GMToolsFrameSelectColor"..i );
		local tex = getglobal( button_color_set:GetName() .."Tex" );
		button_color_set:SetClientID( i );
		tex:SetColor( setColorRGB[i].r, setColorRGB[i].g, setColorRGB[i].b );		
	end
end

local nSelectIndex = 0 ;

function GMToolsFrameSelectColor_OnClick()
	--discheck all
	for i=1,28 do
		local btn = getglobal( "GMToolsFrameSelectColor" .. i);
		btn:DisChecked();
	end
	if nSelectIndex == this:GetClientID() then
		nSelectIndex = 0;
		return;
	end
	this:Checked();
	nSelectIndex = this:GetClientID();
end

function GMToolsGetChatColor()
	if nSelectIndex ~= 0 then
		return string.format( "#c%02x%02x%02x",setColorRGB[nSelectIndex].r,setColorRGB[nSelectIndex].g,setColorRGB[nSelectIndex].b );
	end
	return nil;
end