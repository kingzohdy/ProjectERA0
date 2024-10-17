local MAX_GET_ITEM_TIPS				= 4;
local MAX_GET_ITEM_TIPS_TIME		= 15 * 1000;
local MAX_GET_ITEM_TIPS_ALPHA_TIME	= 4 * 1000;   

local t_defaultColor		= { ["r"] = 245, ["g"] = 247, ["b"] = 181 };

local t_TmpGetItemTips		 = {};  -- 存放最近四条的物品获得信息

function GetTmpGetItemTips()
	return t_TmpGetItemTips;
end

function ClearTmpGetItemTips()
	t_TmpGetItemTips = {};
end

local t_MsgType = { [SYS_MSG_TASK] = "#A014", [SYS_MSG_ITEM] = "#A015", [SYS_MSG_STUFF] = "#A013", [SYS_MSG_ARM] = "#A015" };
function AddGetItemMsg( nType, szMsgText )
	if t_MsgType[nType] == nil then
		return;
	end

	AddGameGetItemTips( t_MsgType[nType]..szMsgText );
end

function ClearGetItemTipsRich()
	for i = 1, MAX_GET_ITEM_TIPS do
		local rich = getglobal( "GMGetItemTips_Rich"..i );
		rich:SetAlpha(1);
		rich:Clear();
		rich:clearHistory();
	end
end

function AddGameGetItemTips( szMsgText )
	-- 清空旧提示信息
	local frame = getglobal( "GameGetItemTipsFrame" );

	ClearGetItemTipsRich();

	if NeedDelayShowText()  then
		SetTmpMsg( SYS_MSG_ITEM, szMsgText );
		return;
	end 
	
	if table.getn( t_TmpGetItemTips ) == MAX_GET_ITEM_TIPS then	
		table.remove( t_TmpGetItemTips, MAX_GET_ITEM_TIPS );
	end
	
	table.insert( t_TmpGetItemTips, 1, { GameMgr:getTickTime(), szMsgText } );

	if not frame:IsShown() then	 
		frame:Show();
	end
	GameGetItemTipsFrame:AddLevelRecursive(1);
end

function SetGetItemTip( t_Content )
	for _, szMsg in ipairs(t_Content) do
		AddGameGetItemTips(szMsg);
	end
end

function GameGetItemTipsFrame_OnLoad()
	this:setUpdateTime( 0 );
end

function GameGetItemTipsFrame_OnUpdate()
	local frame = getglobal( "GameGetItemTipsFrame" );
	-- 不在游戏中或loading界面显示时隐藏中央提示
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or table.getn( t_TmpGetItemTips ) == 0 or GameMgr:isInPlayCamerAnim() then 
		 frame:Hide(); 
	end
	
	local fCurTime	= GameMgr:getTickTime();
	for i = 1, table.getn( t_TmpGetItemTips ) do
		local rich = getglobal( "GMGetItemTips_Rich" .. i );
		rich:SetText(t_TmpGetItemTips[i][2], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );

		local fDiffTime = fCurTime - t_TmpGetItemTips[i][1];
		if fDiffTime >= MAX_GET_ITEM_TIPS_TIME then
			if fDiffTime <= ( MAX_GET_ITEM_TIPS_TIME + MAX_GET_ITEM_TIPS_ALPHA_TIME ) then
				local fChgScale = ( fDiffTime - MAX_GET_ITEM_TIPS_TIME ) / MAX_GET_ITEM_TIPS_ALPHA_TIME;
				rich:SetAlpha( 1 - fChgScale );
			else
				rich:Clear(); 
				t_TmpGetItemTips[i][1] = 0;
			end
		end
	end

	local bShow = false;
	for i = 1, table.getn( t_TmpGetItemTips ) do
		if t_TmpGetItemTips[i][1] ~= 0 then
			bShow = true;
		end
	end

	local t_TmpMiddleTips = GetYellowMiddleTips();
	for i = 1, table.getn( t_TmpMiddleTips ) do
		if t_TmpMiddleTips[i][1] ~= 0 then
			bShow = true;
		end
	end

	if not bShow then
		t_TmpGetItemTips = {};
		GameMiddleTipsFrame:Hide();
	end
end
