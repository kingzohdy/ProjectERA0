local MAX_TMP_BUGLE_MSG		 = 2;
local CHAT_BUGLEMSG_DURATION = 595 * 1000;	-- 小喇叭消息生存时间
local CHAT_BUGLEMSG_DURATION_ALPHATIME = 5 * 1000;
local DEFAULT_BUGLE_FRAME_HEIGHT = 40;

local t_TmpBugleMsg			 = {};  -- 存放最近两条的喇叭提示信息

function InitChatShowBugleMsgFrame()
	t_TmpBugleMsg = {};
end

function ChatShowBugleMsgFrame_OnLoad()
	this:setUpdateTime( 0 );
	this:RegisterEvent("GE_ENTER_MAP");
end

function ChatShowBugleMsgFrame_OnEvent()
	if(arg1 == "GE_ENTER_MAP") then
		if LoadingFrame:IsShown() or GameMgr:isInPlayCamerAnim() or table.getn( t_TmpBugleMsg ) == 0 then 
			this:Hide();
		else 
			this:Show();
		end
	end
end

function ChatShowBugleMsgFrame_OnShow()
end

function ChatShowBugleMsgFrame_OnHide()
	if LinkItemTipsFrame:IsShown() then
		if IsInFrameAbs( "LinkItemTipsFrame", "ChatShowBugleMsgFrame" ) then
			LinkItemTipsFrame:Hide();
		end
	end
end

function ChatShowBugleMsgFrame_OnUpdate()		
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or GameMgr:isInPlayCamerAnim() or table.getn( t_TmpBugleMsg ) == 0 then  
		this:Hide();
	end
	
	local fCurTime	= GameMgr:getTickTime();
	local nLine		= 0;
	local rich1		= getglobal( "CSBMF_BugleMsgRich1" );
	local rich2		= getglobal( "CSBMF_BugleMsgRich2" );

	for i = 1, table.getn( t_TmpBugleMsg ) do
		local rich = getglobal( "CSBMF_BugleMsgRich" .. i );

		if not t_TmpBugleMsg[i][3] then
			local t_color		= t_mainChatFrameTextColor[t_TmpBugleMsg[i][4]];
			t_TmpBugleMsg[i][3] = true;
			rich:SetText( t_TmpBugleMsg[i][2], t_color["r"], t_color["g"], t_color["b"] );
		end

		if i == MAX_TMP_BUGLE_MSG then
			rich2:SetPoint("topleft", "ChatShowBugleMsgFrame", "topleft", 0,  rich1:GetTotalHeight() / GetScreenScaleY() );
			--rich:SetPoint("topleft", "ChatShowBugleMsgFrame", "topleft", 0,  nLine * 21 - 5 );
		else
			--rich1:SetPoint("topleft", "ChatShowBugleMsgFrame", "topleft", 0,  nLine * 21 - 5 );
		end

		nLine = nLine + rich:GetTotalHeight() / GetScreenScaleY();
		local fDiffTime = fCurTime - t_TmpBugleMsg[i][1];
		if fDiffTime >= CHAT_BUGLEMSG_DURATION then
			if fDiffTime <= ( CHAT_BUGLEMSG_DURATION + CHAT_BUGLEMSG_DURATION_ALPHATIME ) then
				local fChgScale = ( fDiffTime - CHAT_BUGLEMSG_DURATION ) / CHAT_BUGLEMSG_DURATION_ALPHATIME;
				local nAlpha	= 1 - fChgScale;
				if nAlpha < 0 then
					nAlpha = 0;
				elseif nAlpha > 1 then
					nAlpha = 1;
				end
				rich:SetAlpha( nAlpha );
			else
				rich:Clear(); 
				t_TmpBugleMsg[i][1] = 0;
			end
		end
	end

	local bShow = false;
	for i = 1, table.getn( t_TmpBugleMsg ) do
		if t_TmpBugleMsg[i][1] ~= 0 then
			bShow = true;
		end
	end

	if not bShow then
		t_TmpBugleMsg = {};
		this:Hide();
	end
end

function ClearBugleRich()
	for i = 1, MAX_TMP_BUGLE_MSG do
		local rich = getglobal( "CSBMF_BugleMsgRich"..i );
		rich:SetAlpha(1);  
		rich:Clear();
		rich:clearHistory();
	end
end

function ChatShowBugleMsgFrameShow( szContant,nType )
	-- 清空旧提示信息
	ClearBugleRich();

	if NeedDelayShowText()  then
		SetTmpMsg( SYS_BUGLE, szContant );
		return;
	end
	
	BugleUVAnimationTex:SetUVAnimation( 50, false );

	if table.getn(t_TmpBugleMsg) == MAX_TMP_BUGLE_MSG then	
		table.remove( t_TmpBugleMsg, MAX_TMP_BUGLE_MSG );
	end
	-- { os.clock(), szContant, false } 开始时间 ， 显示内容， 是否显示过
	table.insert(t_TmpBugleMsg,1,{ GameMgr:getTickTime(), szContant, false,nType }); 
	for i = 1, table.getn( t_TmpBugleMsg ) do
		t_TmpBugleMsg[i][3] = false;
	end

	if not ChatShowBugleMsgFrame:IsShown() then	 
		ChatShowBugleMsgFrame:Show();
	end
end

function SetBugleTip( t_Content )
	for _, szMsg in ipairs(t_Content) do
		ChatShowBugleMsgFrameShow( szMsg,CL_CHAT_ITEM );
	end
end

--------------------------------------炫耀频道--------------------------------------------

local BUGLEMSG_ALL_DURATION 		= 15 * 1000;	-- 跨服喇叭消息生存时间
local BUGLEMSG_ALL_DURATION_MUL 	= 5 * 1000		-- 多个跨服喇叭的刷新间隔
local BUGLEMSG_ALL_DURATION_FADE 	= 12 * 1000 	-- 炫耀频道开始消退时间间隔
local t_BugleMsg = {};

function InitChatAllBugleMsgFrame()
	t_BugleMsg = {};
end


function ShowChatAllBugleMsgFrame( szContant )
	-- { os.clock(), szContant, false } 开始时间 ， 显示内容， 是否显示过
	table.insert(t_BugleMsg,{ startTime = GameMgr:getTickTime(), text = szContant, bShow = false });

	if not ChatAllBugleMsgFrame:IsShown() then	 
		ChatAllBugleMsgFrame:Show();
	end
end

function ChatAllBugleMsgFrame_OnLoad()
	this:setUpdateTime( 0 );
	ChatAllBugleMsgFrameMsgRich:setCenterLine( true );
	this:RegisterEvent("GE_ENTER_MAP");
end

function ChatAllBugleMsgFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or GameMgr:isInPlayCamerAnim() or #t_BugleMsg == 0 then
			this:Hide();
		else 
			this:Show();
		end
	end
end


function ChatAllBugleMsgFrame_OnUpdate()
	if getGameState() ~= STATE_INWORLD or LoadingFrame:IsShown() or GameMgr:isInPlayCamerAnim() or #t_BugleMsg == 0 then
		this:Hide();
		return;
	end

	local fCurTime = GameMgr:getTickTime();
	if not t_BugleMsg[1].bShow then
	--	local t_color = t_mainChatFrameTextColor[CL_CHAT_SPAN];  155, 255, 155
		ChatAllBugleMsgFrameMsgRich:SetText( t_BugleMsg[1].text,0,255,0);
		ChatAllBugleMsgFrameMsgRich:SetAlpha( 1 );
		if ChatAllBugleMsgFrameMsgRich:GetTextLines() > 1 then 
			ChatAllBugleMsgFrameBackMidTex2:Show();
		else 
			ChatAllBugleMsgFrameBackMidTex2:Hide();
		end
		ChatAllBugleMsgFrameUVAnimationTex:SetUVAnimation( 50, false );
		t_BugleMsg[1].startTime = fCurTime;
		t_BugleMsg[1].bShow = true;
	else
		local liveTime = #t_BugleMsg > 1 and BUGLEMSG_ALL_DURATION_MUL or BUGLEMSG_ALL_DURATION;
		local passTime = fCurTime - t_BugleMsg[1].startTime;

		if passTime >= liveTime then
			table.remove( t_BugleMsg, 1 );
		elseif passTime > BUGLEMSG_ALL_DURATION_FADE then
			local fChgScale = ( passTime - BUGLEMSG_ALL_DURATION_FADE ) / ( BUGLEMSG_ALL_DURATION - BUGLEMSG_ALL_DURATION_FADE );
			ChatAllBugleMsgFrameMsgRich:SetAlpha( 1 - fChgScale );
		end
	end
end

function ChatAllBugleMsgFrame_OnShow()
end

function ChatAllBugleMsgFrame_OnHide()
end