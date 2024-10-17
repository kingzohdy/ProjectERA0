local fLastTime2			= 0;
local t_MiddleTipsContent2	= {};

local t_defaultColor		= { ["r"] = 255, ["g"] = 0, ["b"] = 255 };
local MAX_MIDDLE_TIPS2_NO_CHG_ALPHA_TIME	= 2000;
local MAX_SHOW_MIDDLE_TIPS2_TIME			= 6000;


function AddGameMiddleTips2( szMsgText )
	local frame = getglobal( "GameMiddleTipsFrame2" );
	if NeedDelayShowText() then
		SetTmpMsg( SYS_MIDDLE2, szMsgText );
		frame:Hide();
		return;
	end 

	fLastTime2 = GameMgr:getTickTime();

	local rich = getglobal( "GMTips_SecondMidRich" );
	rich:SetAlpha( 1 ); 
	rich:SetText( szMsgText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );	 

	if not frame:IsShown() then
		frame:Show();
	end
end

function SetMiddleTips2( t_Content )
	local rich				= getglobal( "GMTips_SecondMidRich" );
	t_MiddleTipsContent2	= t_Content;
	fLastTime2				= GameMgr:getTickTime();
	rich:SetText( t_MiddleTipsContent2[1], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
	table.remove( t_MiddleTipsContent2, 1 );

	local frame = getglobal( "GameMiddleTipsFrame2" );
	if not frame:IsShown() then
		frame:Show();
	end
end

function GameMiddleTipsFrame2_OnLoad()
	this:RegisterEvent("SYS_MIDDLE2");
	GMTips_SecondMidRich:setCenterLine(true);
end

function GameMiddleTipsFrame2_OnEvent()
	if arg1 == "SYS_MIDDLE2" then
		local Msg			= UIMSG:getUIMsg();
		local t_MidTipsList = GetMidTipsList();
		AddGameMiddleTips2( t_MidTipsList[Msg.MidTipsMsg.m_nMsgType] );
	end
end

function GameMiddleTipsFrame2_OnUpdate()
	local fCurTime	= GameMgr:getTickTime();
	local fDiffTime = fCurTime - fLastTime2;
	local frame		= getglobal( "GameMiddleTipsFrame2" );
	local rich		= getglobal( "GMTips_SecondMidRich" );

	if fDiffTime >= MAX_MIDDLE_TIPS2_NO_CHG_ALPHA_TIME then
		if fDiffTime <= MAX_SHOW_MIDDLE_TIPS2_TIME then 
			local fChgScale = ( fDiffTime - MAX_MIDDLE_TIPS2_NO_CHG_ALPHA_TIME ) / ( MAX_SHOW_MIDDLE_TIPS2_TIME - MAX_MIDDLE_TIPS2_NO_CHG_ALPHA_TIME );
			rich:SetAlpha( 1 - fChgScale );
		else
			if table.getn( t_MiddleTipsContent2 ) > 0 then
				fLastTime2 = GameMgr:getTickTime();
				rich:SetText( t_MiddleTipsContent2[1], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
				table.remove( t_MiddleTipsContent2, 1 );
			else
				rich:Clear(); 
				frame:Hide();
			end
		end
	end

	if getGameState() ~= STATE_INWORLD  or LoadingFrame:IsShown() or GameMgr:isInPlayCamerAnim() then  
		this:Hide();
	end
end