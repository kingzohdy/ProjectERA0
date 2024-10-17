local t_WaitQueueControl = { lastTime = 0, averagePredicateDiffTime = 70, selfClose = false };
local nLastResponeTime = 0;

function initWaitQueueControl()
	t_WaitQueueControl = { lastTime = 0, averagePredicateDiffTime = 70, selfClose = false };
end

local t_WaitQueueFramEvents =	{	["GE_UPDATE_GETROLELIST"] = {}, 
								};

t_WaitQueueFramEvents["GE_UPDATE_GETROLELIST"].func = 
function ()
	WaitQueueFrame:Hide();
	t_WaitQueueControl["selfClose"] = false;
end

function WaitQueueFrame_OnLoad()
	for event, _ in pairs( t_WaitQueueFramEvents ) do
		this:RegisterEvent( event );
	end

	t_WaitQueueFramEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end

	setmetatable( t_WaitQueueFramEvents, t_WaitQueueFramEvents );
	WaitQueueFrameDescRichText:setCenterLine( true );
	--this:setUpdateTime( 0 );
end

function WaitQueueFrame_OnEvent()
	t_WaitQueueFramEvents[arg1].func();
end

function WaitQueueFrame_Test_OnShow()
	local waitQueueDesc =	{
								"当前服务器人数已满",
								"您的位置是：".." 第 #cff00001#n 位",								
							};

	WaitQueueFrameDescRichText:Clear();

	for i = 1, table.getn( waitQueueDesc ) do
		WaitQueueFrameDescRichText:AddText( waitQueueDesc[i], 50, 30, 10 );
	end
end

function WaitQueueFrame_OnShow()
	t_WaitQueueControl["lastTime"]					= 0;
	t_WaitQueueControl["averagePredicateDiffTime"]	= 70;
end

function WaitQueueFrame_OnHide()
	nLastResponeTime = 0;
end


function WaitQueueFrame_OnUpdate()
	if GameMgr:getTickTime() - t_WaitQueueControl["lastTime"] > 1000 * 60 then
		t_WaitQueueControl["averagePredicateDiffTime"] = t_WaitQueueControl["averagePredicateDiffTime"] * 2;
	end
	if nLastResponeTime ~= 0 and GameMgr:getTickTime() - nLastResponeTime > 15*1000 then
		ReturnToSelect();
		MessageBox( "提示", "网络连接错误" );		
		WaitQueueFrame:Hide();
	end
end

-- 进入官网
function WaitQueueFrame_EnterOffsiteBtn_OnClick()
	GameMgr:openWebSite("http://www.ljy0.com");
end

-- 进入论坛
function WaitQueueFrame_EnterForumBtn_OnClick()
	GameMgr:openWebSite("http://bbs.ljy0.com");
end

function WaitQueueFrame_CancelBtn_OnClick()
	WaitQueueFrame:Hide();
	t_WaitQueueControl["selfClose"] = true;
	LoginMgr:releaseLoginGameServer();
	LoginFrame:Hide();
	LoginBackFrame:Hide();
--	DirServerFrame:Show();
	DirSimpleServerFrame:Show();
	DirFrame_OnClickRefurbishBtn();
	LoginMgr:breakNetConnect();
end

function UpdateQueueInfo( nMax, nPos )
	nLastResponeTime = GameMgr:getTickTime();
	if t_WaitQueueControl["selfClose"] then
		return;
	end

	if not WaitQueueFrame:IsShown() then
		WaitQueueFrame:Show();
	end

	local waitQueueDesc =	{
								"当前服务器人数已满",
								"您的位置是：".." 第 #cff0000" .. nPos .."#n 位",								
							};

	WaitQueueFrameDescRichText:Clear();

	for i = 1, table.getn( waitQueueDesc ) do
		WaitQueueFrameDescRichText:AddText( waitQueueDesc[i], 50, 30, 10 );
	end
end
