local t_SysTipContent		= {};
local fLastSysTipTime		= 0;

local t_defaultColor		= { ["r"] = 124, ["g"] = 235, ["b"] = 232 };
local MAX_SYS_TIP_SHOW_TIME			= 6000;
local MAX_SYS_TIP_NO_CHG_ALPHA_TIME = 2000;

function AddGameSysTip( szMsgText )
	local frame = getglobal( "GameSysTipFrame" );
	if NeedDelayShowText()  then
		SetTmpMsg( SYS_TIP, szMsgText );
		frame:Hide();
		return;
	end
	fLastSysTipTime = GameMgr:getTickTime(); 
	
	local rich = getglobal( "GMTips_SysTipRich" );
	rich:SetAlpha(1); 
	rich:SetText( szMsgText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );	 

	if not frame:IsShown() then
		frame:Show();
	end
end

function SetSysTip( t_Content )
	local rich = getglobal( "GMTips_SysTipRich" );
	t_SysTipContent = t_Content;
	fLastSysTipTime = GameMgr:getTickTime();
	rich:SetText("#P"..t_SysTipContent[1], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
	table.remove( t_SysTipContent, 1 );

	local frame = getglobal( "GameSysTipFrame" );
	if not frame:IsShown() then
		frame:Show();
	end
end

function GameSysTipFrame_OnLoad()
	this:RegisterEvent("SYS_TIP");
	GMTips_SysTipRich:setCenterLine( true )
end

function GameSysTipFrame_OnEvent()
	if arg1 == "SYS_TIP" then
		local Msg			= UIMSG:getUIMsg();
		local t_MidTipsList = GetMidTipsList();
		AddGameSysTip( t_MidTipsList[Msg.MidTipsMsg.m_nMsgType] );
	end
end

function GameSysTipFrame_OnUpdate()
	local frame			= getglobal( "GameSysTipFrame" );
	local rich			= getglobal( "GMTips_SysTipRich" );
	local fCurTime		= GameMgr:getTickTime();
	local fDiffTime		= fCurTime - fLastSysTipTime;

	if fDiffTime >= MAX_SYS_TIP_NO_CHG_ALPHA_TIME then
		if fDiffTime <= MAX_SYS_TIP_SHOW_TIME then 
			local fChgScale = ( fDiffTime - MAX_SYS_TIP_NO_CHG_ALPHA_TIME ) / ( MAX_SYS_TIP_SHOW_TIME - MAX_SYS_TIP_NO_CHG_ALPHA_TIME );
			rich:SetAlpha( 1 - fChgScale );
		else
			if table.getn( t_SysTipContent ) > 0 then
				fLastSysTipTime = GameMgr:getTickTime();
				rich:SetText( t_SysTipContent[1], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
				table.remove( t_SysTipContent, 1 );
			else
				rich:Clear(); 
				frame:Hide();
			end
		end
	end

	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or GameMgr:isInPlayCamerAnim() then  
		this:Hide();
	end
end
