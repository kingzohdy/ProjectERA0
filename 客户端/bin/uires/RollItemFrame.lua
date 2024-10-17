local MAX_SHOW_NUM = 3;
local t_RollItemData = {};

function ShowRollItem( nBootId )
	table.insert( t_RollItemData, 1, { ["id"]=nBootId,["startTime"]=os.clock() } );
	local slider = getglobal("RollItemFrameScrollBar");
	SetScrollBar( "RollItemFrameScrollBar", MAX_SHOW_NUM, table.getn( t_RollItemData ) );
	if slider:IsShown() then
		if slider:GetValue() == 0 then
			if slider:GetMaxValue() == 1 then
				slider:SetValue( slider:GetMaxValue() );
				RefreshRollData( slider:GetMaxValue() + 1 );
			else
				RefreshRollData( slider:GetValue() + 1 );
			end
		else
			slider:SetValue( slider:GetValue() + 1 );
			RefreshRollData( slider:GetValue() + 2 );
		end
		RollItemFrameScrollUp:Show();
		RollItemFrameScrollCenter:Show();
		RollItemFrameScrollDown:Show();
		RollItemFrameScrollUpBtn:Show();
		RollItemFrameScrollDownBtn:Show();
	else
		RefreshRollData( 1 );
		RollItemFrameScrollUp:Hide();
		RollItemFrameScrollCenter:Hide();
		RollItemFrameScrollDown:Hide();
		RollItemFrameScrollUpBtn:Hide();
		RollItemFrameScrollDownBtn:Hide();
	end
	RollItemFrame:Show();
end

function RefreshRollData( nStartIndex )
	local nIndex = 1;
	for i=nStartIndex,table.getn( t_RollItemData ) do
		if nIndex > MAX_SHOW_NUM then
			break;
		end
		local rollInfo = GameRollMgr:getRollUIInfo( t_RollItemData[i]["id"] );
		if rollInfo ~= nil then
			local btn = getglobal( "RollItemFrameRollItem"..nIndex );
			local itemTex = getglobal( btn:GetName() .. "ItemBtnIconTexture" );
			local numFont = getglobal( btn:GetName() .. "ItemBtnCountFont" );
			local ItemBoxTex = getglobal( btn:GetName() .. "ItemBtnBoxTexture" );
			local UVAnimationTex = getglobal( btn:GetName() .. "ItemBtnUVAnimationTex" );
			local nameFont = getglobal( btn:GetName() .. "NameFont" );
			local progress = getglobal( btn:GetName() .. "Progress" );

			local item = GameRollMgr:getRollItem( t_RollItemData[i]["id"] );
			if item ~= nil then
				ItemBoxTex:Show();
				local itemDef = item:getItemDef();
				local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";			
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				itemTex:SetTexture( IconPath );
				SetEquipImportantEffectUV( itemDef, UVAnimationTex:GetName(),item );
				nameFont:SetText( itemDef.Name );
				local t_NameColor = GetItemNameColor(itemDef);
				nameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );				
				if item:getNum() > 1 then
					numFont:SetText( item:getNum() );
				else
					numFont:SetText("");
				end
				local dt = rollInfo.EndTime - (os.clock() - t_RollItemData[i]["startTime"] );
				local scale = dt/rollInfo.EndTime;
				if scale > 1 then
					scale = 1;
				end
				if scale < 0 then
					scale = 0;
				end
				progress:SetSize( 133*scale,9 );
				btn:SetClientUserData( 0,rollInfo.EndTime );
				btn:SetClientUserData( 1,t_RollItemData[i]["startTime"] );
				btn:SetClientUserData( 2,t_RollItemData[i]["id"]);
			end
			btn:Show();
		end
		nIndex = nIndex + 1;
	end
	for i=nIndex,MAX_SHOW_NUM do
		local btn = getglobal( "RollItemFrameRollItem"..i );
		btn:Hide();
	end
	if table.getn( t_RollItemData ) > 3 then
		RollItemFrame:SetSize( 209,3+69*( nIndex-1) );
	else
		RollItemFrame:SetSize( 183,3+69*( nIndex-1) );
	end
end

function RollItemFrameNeedBtn_OnClick()
	local parent = getglobal( this:GetParent() );
	local nBootId = parent:GetClientUserData( 2 );
	GameRollMgr:sendRollMessage( ROLL_CLT_NEED_TOLUA,nBootId );
end

function RollItemFrameCancelBtn_OnClick()
	local parent = getglobal( this:GetParent() );
	local nBootId = parent:GetClientUserData( 2 );
	GameRollMgr:sendRollMessage( ROLL_CLT_QUIT_TOLUA,nBootId );
end

function RollItemButton_OnUpdate()
	local nTime = this:GetClientUserData( 0 );
	local startTime = this:GetClientUserData( 1 );
	local endTime =  nTime + startTime ;
	if os.clock() >= endTime then
		local nBootId = this:GetClientUserData( 2 );
		GameRollMgr:sendRollMessage( ROLL_CLT_QUIT_TOLUA,nBootId );
	else
		local dt = os.clock() - startTime ;
		local scale = dt/nTime;
		if scale <0 then
			sacle = 0;
		end
		if scale > 1 then
			scale = 1;
		end
		local tex = getglobal( this:GetName() .. "Progress" );
		tex:SetSize( (1-scale)*133,9 );
	end
end

function RollItemFrame_OnShow()
	this:SetPoint( "bottom","ShortCutButtonsFrame","top",-50,-70 );
end

--退出游戏时，放弃所有
function ClearRollInfo()
	for i=1,table.getn( t_RollItemData ) do
		local rollInfo = GameRollMgr:getRollUIInfo( t_RollItemData[i]["id"] );
		if rollInfo ~= nil then
			GameRollMgr:sendRollMessage( ROLL_CLT_QUIT_TOLUA,t_RollItemData[i]["id"] );
		end
	end
	t_RollItemData = {};
end

function RollItemFrameScrollUpBtn_OnClick()
	local slider		= getglobal( "RollItemFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() - slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function RollItemFrameScrollDownBtn_OnClick()
	local slider		= getglobal( "RollItemFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end	
end

function RollItemFrame_ScrollBarValueChange()
	local slider		= getglobal( "RollItemFrameScrollBar" );
	local nValue		= slider:GetValue();
	local nMaxValue		= slider:GetMaxValue();
	RefreshRollData( slider:GetValue() + 1 );
end

function RollItemFrame_OnUpdate()
	for i=1,table.getn( t_RollItemData ) do
		local rollInfo = GameRollMgr:getRollUIInfo( t_RollItemData[i]["id"] );
		if rollInfo ~= nil then
			if os.clock() - t_RollItemData[i]["startTime"] >= rollInfo.EndTime then
				GameRollMgr:sendRollMessage( ROLL_CLT_QUIT_TOLUA,t_RollItemData[i]["id"] );
			end
		end
	end
end

function RollItemFrameRemoveOne( nBootyId )
	for i=1,table.getn( t_RollItemData ) do
		if t_RollItemData[i]["id"] == nBootyId then
			table.remove( t_RollItemData,i );
			break;
		end
	end
	if table.getn( t_RollItemData ) == 0 then
		RollItemFrame:Hide();
	else
		local slider = getglobal( "RollItemFrameScrollBar" );
		SetScrollBar( "RollItemFrameScrollBar", MAX_SHOW_NUM, table.getn( t_RollItemData ) );
		if slider:IsShown() then
			if slider:GetValue() == 0 then
				RefreshRollData( slider:GetValue() + 1 );
			else
				slider:SetValue( slider:GetValue() );
				RefreshRollData( slider:GetValue() + 1 );
			end
			RollItemFrameScrollUp:Show();
			RollItemFrameScrollCenter:Show();
			RollItemFrameScrollDown:Show();
			RollItemFrameScrollUpBtn:Show();
			RollItemFrameScrollDownBtn:Show();
		else
			slider:SetValue( 0 );
			RefreshRollData( 1 );
			RollItemFrameScrollUp:Hide();
			RollItemFrameScrollCenter:Hide();
			RollItemFrameScrollDown:Hide();
			RollItemFrameScrollUpBtn:Hide();
			RollItemFrameScrollDownBtn:Hide();
		end
	end
end

function RollItemFrame_OnLoad()
	for i=1,3 do
		local tex = getglobal("RollItemFrameRollItem"..i.."Bkg");
		tex:SetBlendAlpha( 0.5 );
	end
	RollItemFrameBkg:SetBlendAlpha( 0.5 )
	this:RegisterEvent( "GE_TEAM_LEAVETEAM" );
end

function RollItemFrame_OnEvent()
	if arg1 == "GE_TEAM_LEAVETEAM" then
		this:Hide();
	end
end