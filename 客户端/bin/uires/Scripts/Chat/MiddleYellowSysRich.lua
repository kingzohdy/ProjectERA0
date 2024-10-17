local MAX_YELLOW_MID_TIPS		= 3;
local MAX_YELLOW_TIPS_TIME		= 5 * 1000;
local MAX_YELLOW_TIPS_ALPHA_TIME= 4 * 1000;   

local t_TmpMiddleTips		 = {};	-- 存放最近四条的中央提示信息

local t_defaultColor		= { ["r"] = 255, ["g"] = 255, ["b"] = 0 };

function ClearYellowMiddleTips()
	t_TmpMiddleTips = {};
end

function GetYellowMiddleTips()
	return t_TmpMiddleTips;
end

function ClearMiddleTipsRich()
	for i = 1, MAX_YELLOW_MID_TIPS do
		local rich = getglobal( "GMTips_Rich"..i );
		rich:SetAlpha( 1 );  
		rich:Clear();
		rich:clearHistory();
	end
end

function AddGameMiddleTips( szMsgText )	
	-- 清空旧提示信息
	local frame = getglobal( "GameMiddleTipsFrame" );
	ClearMiddleTipsRich();

	if NeedDelayShowText()  then
		SetTmpMsg( SYS_MIDDLE, szMsgText );
		return;
	end 
	
	if table.getn( t_TmpMiddleTips ) == MAX_YELLOW_MID_TIPS then	
		table.remove( t_TmpMiddleTips, MAX_YELLOW_MID_TIPS );
	end

	table.insert( t_TmpMiddleTips, 1, { GameMgr:getTickTime(), szMsgText } );

	if not frame:IsShown() then	 
		frame:Show();
	end
end

function SetMiddleTips( t_Content )
	for _, szMsg in ipairs(t_Content) do
		AddGameMiddleTips(szMsg);
	end
end

function GameMiddleTipsFrame_OnLoad()
	this:setUpdateTime( 0 );
	this:RegisterEvent("GE_MIDTIPS_MSG");
	this:RegisterEvent("GE_CONVOY_DIST_LONG");

	for i = 1, MAX_YELLOW_MID_TIPS do
		local rich = getglobal( "GMTips_Rich"..i );
		rich:setCenterLine( true );
	end
end

function GameMiddleTipsFrame_OnEvent()
	if arg1 == "GE_MIDTIPS_MSG" then
		local Msg			= UIMSG:getUIMsg();
		local t_MidTipsList = GetMidTipsList();
		AddGameMiddleTips( t_MidTipsList[Msg.MidTipsMsg.m_nMsgType] );

	elseif arg1 == "GE_CONVOY_DIST_LONG" then
		local mainplayer	= ActorMgr:getMainPlayer();	
		local convyInfo		= mainplayer:getConvoyInfo();
		local szText		= "你与"..getMonsterName( convyInfo.m_nMonsterResID ).."距离过远，请速返回";
		for _, data in ipairs( t_TmpMiddleTips ) do
			if data[2] == szText then
				return;
			end
		end

		-- 否则显示出来
		AddGameMiddleTips( szText );
	end
end

function GameMiddleTipsFrame_OnUpdate()
	-- 不在游戏中或loading界面显示时隐藏中央提示
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or table.getn( t_TmpMiddleTips ) == 0 or GameMgr:isInPlayCamerAnim() then 
		 this:Hide(); 
	end
	
	local fCurTime	= GameMgr:getTickTime();
	
	for i = 1, table.getn( t_TmpMiddleTips ) do
		local rich = getglobal( "GMTips_Rich" .. i );	
		rich:SetText( t_TmpMiddleTips[i][2], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );

		local fDiffTime = fCurTime - t_TmpMiddleTips[i][1];
		if fDiffTime >= MAX_YELLOW_TIPS_TIME then
			if fDiffTime <= ( MAX_YELLOW_TIPS_TIME + MAX_YELLOW_TIPS_ALPHA_TIME ) then
				local fChgScale = ( fDiffTime - MAX_YELLOW_TIPS_TIME ) / MAX_YELLOW_TIPS_ALPHA_TIME;
				rich:SetAlpha( 1 - fChgScale );
			else
				rich:Clear(); 
				t_TmpMiddleTips[i][1] = 0;
			end
		end
	end

	local bShow = false;
	for i = 1, table.getn( t_TmpMiddleTips ) do
		if t_TmpMiddleTips[i][1] ~= 0 then
			bShow = true;
		end
	end

	local t_TmpGetItemTips = GetTmpGetItemTips();
	for i = 1, table.getn( t_TmpGetItemTips ) do
		if t_TmpGetItemTips[i][1] ~= 0 then
			bShow = true;
		end
	end

	if not bShow then
		t_TmpMiddleTips = {};
		GameMiddleTipsFrame:Hide();
	end
end