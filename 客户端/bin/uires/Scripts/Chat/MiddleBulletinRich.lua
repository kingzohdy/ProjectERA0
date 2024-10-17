local TEXTEXTENT_OFFSET		= 60;
local SCROLL_WIDTH			= 510;
local MOVE_WIDTH_TICK		 = 1;
local MAX_BULLETIN_RECT_WIDTH = 3550;

local szLastbulletinText	= "";
local nLastScreenWidth			= 0;
local nLastScreenHeight			= 0;
local t_BulletinShow		= {};
local nBeginBulletintTime	= 0;
local nBulletinIndex		= 1;
local nTextExtent			= 0;	--当前显示消息的长度
local nWidth				= 0;
local nX					= 0;
local bSetTime				= false;

local t_defaultColor		= { ["r"] = 0, ["g"] = 255, ["b"] = 0 };

function ClearBulletin()
	t_BulletinShow = {};
end

function InitGameBulletintTipsData()
	szLastbulletinText  = "";
	nLastScreenWidth		= 0;
	nLastScreenHeight		= 0;
	nBeginBulletintTime	= GameMgr:getTickTime() / 1000;
	nTextExtent			= 0;
	nBulletinIndex		= 1;
	nWidth				= 0;
	nX					= 0;
	bSetTime			= false;
end

-- 是否显示下一条公告
function IsNextBulletinShow( szRichName, nBulletinNum, nBulletinIndex )
	local rich  = getglobal( szRichName );
	-- 当前公告显示未结束或显示列表不为时
	if rich:GetTextLines() ~= 0 or nBulletinNum == 0 then
		return false;
	end

	if nBulletinIndex > nBulletinNum  then
		nBulletinIndex = 1;
	end
	
	--print( "IsNextBulletinShow--nBulletinIndex = "..nBulletinIndex );
	local CurBulletin = ChatManager:getBulletinEntry( nBulletinIndex - 1 );
	local frame = getglobal( "GameBulletintTipsFrame" );
	--如果是第一次显示的话，不判断间隔？
	if t_BulletinShow[CurBulletin.ID] and ( GameMgr:getTickTime() / 1000 - nBeginBulletintTime ) < CurBulletin.RollInterval then
		frame:Hide();
		return false;
	end
	
	local szBulletinText = CurBulletin.Text;
	rich:SetPoint( "left", frame:GetName(), "right", 800, 0 );
	nTextExtent = rich:GetTextExtentWidth( szBulletinText ) + TEXTEXTENT_OFFSET;
	nWidth		= 0;
	nX			= 0;
	
	rich:SetText( szBulletinText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
	rich:SetWidth( 0 );
	rich:resizeRect( 0,16 );
	rich:Hide();
	szLastbulletinText	= szBulletinText;
	nLastScreenWidth	= GetScreenWidth();
	nLastScreenHeight	= GetScreenHeight();
	nBeginBulletintTime = GameMgr:getTickTime() / 1000;
	if not t_BulletinShow[CurBulletin.ID] then  
		AddBulletInText(szBulletinText);
	end
	t_BulletinShow[CurBulletin.ID] = true;
	if CurBulletin.RollInterval == CLIENT_SHOP_BULLETIN_TIME then
		ChatManager:removeBulletinEntry( CurBulletin.ID );
	end

	--每一次显示公告的开始位置
	rich:SetPoint( "left", frame:GetName(), "right", 0, 0 );
	if not frame:IsShown() then
		frame:Show();
	end

	return true;
end

function GameBulletintTipsFrame_OnShow()
	local mutexFrame = getglobal( "GameSysBulletintFrame" );
	if mutexFrame:IsShown() then
		mutexFrame:Hide();
	end
end

function GameBulletintTipsFrame_OnHide()  
	szLastbulletinText  = "";
	BeginRedTips();
end

function GameBulletint_OnUpdate()
	local frame = getglobal( "GameBulletintTipsFrame" );
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or CameraCoverFrame:IsShown() then 
		frame:Hide();
	end
end

-- 中央滚动公告界面每帧刷新
function GameBulletintTipsFrame_OnUpdate( )
	local frame = getglobal( "GameBulletintTipsFrame" );
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or CameraCoverFrame:IsShown() then
		if frame:IsShown() then
			frame:Hide();
		end
		
		return;
	end

	local nBulletinNum	= ChatManager:getBulletinNum();
	local rich			= getglobal( "GBTips_Rich" );
	
	--公告显示位置仍在游戏窗口内时,改变它的offset
	if nX < nTextExtent then
		bSetTime = true;
		if isPointInFrame( frame:GetName() ) then
			return;
		end
		
		nWidth = nWidth + MOVE_WIDTH_TICK; 

		if rich:GetWidth() < SCROLL_WIDTH then
			if not rich:IsShown() then
				rich:Show();
			end

			if szLastbulletinText ~= "" and ( nLastScreenWidth ~= GetScreenWidth() or nLastScreenHeight ~= GetScreenHeight() ) then
				rich:resizeRect( MAX_BULLETIN_RECT_WIDTH,16 );
				rich:SetText( szLastbulletinText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
				nLastScreenWidth = GetScreenWidth();
				nLastScreenHeight = GetScreenHeight();
			end

			rich:ScrollFirst();	
			rich:SetWidth( nWidth );
			rich:SetPoint("left", frame:GetName(), "right",-nWidth,0);
		else
			nX = (nWidth) - SCROLL_WIDTH;
			if ( nLastScreenWidth ~= GetScreenWidth() or nLastScreenHeight ~= GetScreenHeight() ) then
				rich:resizeRect( MAX_BULLETIN_RECT_WIDTH,16 );
				rich:SetText( szLastbulletinText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
				rich:SetWidth( SCROLL_WIDTH );
				nLastScreenWidth	= GetScreenWidth();
				nLastScreenHeight	= GetScreenHeight();
			end

			rich:SetDispPosX( nX );
		end
	elseif nBeginBulletintTime > 0 and nX >= nTextExtent then
		rich:resizeRect( MAX_BULLETIN_RECT_WIDTH,16 );
		rich:Clear();  														  
		rich:SetPoint( "left", frame:GetName(), "right", 800, 0 );
		
		if bSetTime then
			if nBulletinIndex >= nBulletinNum then
				nBulletinIndex = 1;
			else
				nBulletinIndex = nBulletinIndex + 1;
			end
		end
		bSetTime = false;
	end
   
	-- 列表为空而且公告显示结束时,隐藏公告窗口
	if (rich:GetTextLines() < 1 and nBulletinNum == 0) or CameraCoverFrame:IsShown() then
		if frame:IsShown() then
			frame:Hide();
		end
	end	

	if not IsNextBulletinShow( "GBTips_Rich", nBulletinNum, nBulletinIndex ) then	 
		return;
	end
end

function  AddBulletInText( sText )
	local szShowText		= "#A009"..sText;
	local nCanPingBiChatType= GetPingDaoChatType( { ["anim"] = "#A006" } );
	UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_defaultColor, ["chatType"] = nCanPingBiChatType } );

	historyRich = getglobal( "ChatHistoryMsgMgrFrame_SystemRich" );
	historyRich:AddText( szShowText, t_defaultColor["r"], t_defaultColor["g"], t_defaultColor["b"] );
	UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
end
