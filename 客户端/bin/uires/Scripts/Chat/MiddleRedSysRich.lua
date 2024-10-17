-- 红色字体的频道1
local fLastBulletinTime		= 0;
local t_BulletinContent		= {};

local t_defaultColor		= { ["r"] = 239, ["g"] = 74, ["b"] = 37 };
local MAX_NO_CHG_ALPHA_TIME = 2000;
local MAX_SHOW_RED_SYS_TIME	= 6000;

function NeedDelayRedTipsShowText()
	if LoadingFrame:IsShown() or GameBulletintTipsFrame:IsShown() then
		return true;
	end

	return false;
end

function BeginRedTips()
	if IsRedMidTipsEmpty() then
		return;
	end
	
	local szMsgText = t_BulletinContent[1];
	table.remove( t_BulletinContent, 1 );
	AddGameSysBulletint( szMsgText );
end

function AddGameSysBulletint( szMsgText )
	local frame = getglobal( "GameSysBulletintFrame" );

	if NeedDelayRedTipsShowText() then
		SetTmpMsg( SYS_BULLETIN, szMsgText );
		frame:Hide();
		return;
	end 

	fLastBulletinTime = GameMgr:getTickTime();
	local rich = getglobal( "GMTips_SysBulletintRich" );
	rich:SetAlpha( 1 ); 
	rich:SetText( szMsgText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );	 

	if not frame:IsShown() then
		frame:Show();
	end
end

function SetBulletin( t_Content )
	t_BulletinContent	= t_Content;
	local szMsgText		= t_BulletinContent[1];
	table.remove( t_BulletinContent, 1 );

	AddGameSysBulletint( szMsgText );
end

function IsRedMidTipsEmpty()
	return table.getn( t_BulletinContent ) == 0;
end

function GameSysBulletintFrame_OnLoad()
	this:RegisterEvent("SYS_BULLETIN");
	GMTips_SysBulletintRich:setCenterLine( true )
end

function GameSysBulletintFrame_OnEvent()
	if arg1 == "SYS_BULLETIN" then
		local Msg			= UIMSG:getUIMsg();
		local t_MidTipsList = GetMidTipsList();
		AddGameSysBulletint( t_MidTipsList[Msg.MidTipsMsg.m_nMsgType] );
	end
end

function GameSysBulletintFrame_OnUpdate()
	local fCurTime	= GameMgr:getTickTime();
	local fDiffTime = fCurTime - fLastBulletinTime;
	local rich		= getglobal( "GMTips_SysBulletintRich" );
	local frame		= getglobal( "GameSysBulletintFrame" );

	if fDiffTime >= MAX_NO_CHG_ALPHA_TIME then
		if fDiffTime <= MAX_SHOW_RED_SYS_TIME then 
			local fChgScale = ( fDiffTime - MAX_NO_CHG_ALPHA_TIME ) / ( MAX_SHOW_RED_SYS_TIME - MAX_NO_CHG_ALPHA_TIME );
			rich:SetAlpha( 1 - fChgScale );
		else
			if table.getn( t_BulletinContent ) > 0 then
				fLastBulletinTime = GameMgr:getTickTime();				
				rich:SetText( "#P"..t_BulletinContent[1], t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
				table.remove( t_BulletinContent, 1 );
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