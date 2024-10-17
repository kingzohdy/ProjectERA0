
local ONE_PAGE_NUM = 9;

local tPrivateData = {};

local ACTIONTEXT = "@@#%d,%d,%d#n";
local t_ActionData = { 
						{10700,"欢呼","/cheer"},
						{10701,"飞吻","/kiss"},
						{10702,"跳舞","/dance"},
						{10703,"鼓舞","/encourage"},
						{10704,"装死","/die"},
						{10705,"攻击","/attack"},
						{10706,"鼓掌","/applause"},
						{10707,"耍酷","/cool"},
						{10708,"乞讨","/cadge"},
						{10709,"作揖","/thumb"},
						{10710,"无奈","/however"},
						{10711,"挥手","/hand"},
						{10712,"深思","/think"},
						{10713,"痛苦","/sad"},
					};
local t_ActionString = {
						[10700] = {"%s终于压抑不住内心的兴奋，对着%s就是一阵手舞足蹈的欢呼","%s情不自禁的为自己的美貌欢呼抓狂！"},
						[10701] = {"%s深情的凝望着%s，不羁的一抬头，潇洒的一挥手，矮油～好犀利的一个飞吻！","%s向周围的大众用力的抛出一个飞吻，感谢他们对你的忠实！"},
						[10702] = {"%s心血来潮，决定邀请%s创造场轰轰烈烈的热舞快闪","地球已经无法阻止%s的翩翩起舞了…"},
						[10703] = {"%s对%s喊道：相信自己！我们将创造一个民族！","%s一咬牙关，决心独自扛起拯救地球的重任！"},
						[10704] = {"%s不小心看到了%s的脸，结果被吓得昏死过去","%s因照镜子时发现自己长得太完美，导致猝死"},
						[10705] = {"%s狠狠一拳打过去，%s痛得都哭了！","%s对着前方就是一拳！连飞过的苍蝇都被你打死了！"},
						[10706] = {"以每秒100银币计费，%s给予了%s最热烈最持久的掌声","%s给予周围的大众最崇高的掌声，感谢他们对你的崇拜！"},
						[10707] = {"%s潇洒的一摆手，对%s说：不要迷恋俺，俺只是个传说","%s在不经意摆手间，hold住了整个场面！"},
						[10708] = {"%s一把跪在%s跟前，振振有辞地说道：捐红十字还不如捐我！","%s痛苦的跪在地上，撕声裂肺地吼到：捐钱给可怜人！"},
						[10709] = {"%s腼腆的向%s行了个礼，突然有种心动的感觉","为了感谢大众对%s的爱戴，你彬彬有礼的行了个礼"},
						[10710] = {"%s蛋定的向%s摊了摊手，表示再也不相信爱情了…","%s无奈的摊摊手，表示心有余力而不足，无法一次满足大众的欲望"},
						[10711] = {"由于妈妈喊%s回家吃饭，只能无奈的朝%s挥手道别","%s挥了挥手，带走了所有云彩！"},
						[10712] = {"%s不禁深思困惑，为何会有%s这么奇形怪状的人","%s百思不得其解，长得这么完美到底是不是你的错"},
						[10713] = {"%s在%s面前痛苦的失声大哭道：你说过你会负责的！","%s痛得直打哆嗦，浑身直冒冷汗，真想赶紧找个厕所！"},
						};

function GetActionData()
	return t_ActionData;
end
function ChatActionCheckString( szContext )
	for i=1,table.getn( t_ActionData ) do
		if t_ActionData[i][3] == szContext then
			ChatActionSendPlayAction( t_ActionData[i][1] );
			return true;
		end
	end
end

local lastSendTime = 0;

function ChatActionSendPlayAction( actionID )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if not mainplayer:canPlayChatAction() then
		ShowMidTips("当前状态不能进行该操作");
		return;
	end
	local targetID = mainplayer:GetSelectTarget();

	ChatActionPlayAction( actionID,mainplayer:GetID(),targetID );
	if os.clock() - lastSendTime > 0.5 then
		mainplayer:requestSendMotionAction( actionID,targetID );
		lastSendTime = os.clock();
	end
end

function ChatActionFrameItemBtnOnClick()
	local actionID = this:GetClientUserData( 0 );
	ChatActionSendPlayAction( actionID );
end

function ChatActionFrameItemBtnOnEnter()
	local nIdx = this:GetClientUserData( 1 );
	local szText = t_ActionData[nIdx][2] .. "\t" .. t_ActionData[nIdx][3];
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = "cursor", vertical = "bottom",horizonal="right" } );
end

local tActionIDs = { 0,10702,10703,10704,10713 }
function ChatActionPlayAction( actionId,playerID,targetID )
	local player = ActorMgr:FindActor( playerID );
	local targetPlayer = ActorMgr:FindActor( targetID );
	local mainplayer = ActorMgr:getMainPlayer();
	local bReset = false;
	if player == nil or mainplayer == nil then return end
	if player:canPlayChatAction() then
		local nCurActionId = player:getCurActionId();
		for i=1,table.getn( tActionIDs ) do
			if tActionIDs[i] == nCurActionId then
				bReset = true;
			end
		end
		player:playChatAction( actionId,bReset );
		if t_ActionString[actionId] ~= nil then
			local szText = "";
			if mainplayer:GetID() == playerID then
				if targetPlayer ~= nil then
					szText = string.format( t_ActionString[actionId][1],"你",targetPlayer:getName() );
				else
					szText = string.format( t_ActionString[actionId][2],"你" );
				end
			elseif mainplayer:GetID() == targetID then
				szText = string.format( t_ActionString[actionId][1],player:getName(),"你" );
			else			
				if targetPlayer ~= nil then					
					szText = string.format( t_ActionString[actionId][1],player:getName(),targetPlayer:getName() );
				else					
					szText = string.format( t_ActionString[actionId][2],player:getName() );
				end
			end
			player:addPopWindow( szText );
			--local t_color	= t_mainChatFrameTextColor[CL_CHAT_SAY];
			--UpdatePersonalAndAllChatRich( { ["text"] = szText, ["color"] = t_color, ["chatType"] = nil } );
		end
	end
end

function ChatActionSetShowData( nPage )
	local nStartIndex = (nPage - 1)*ONE_PAGE_NUM + 1;
	local index = 1;
	for i=nStartIndex,ONE_PAGE_NUM*nPage do
		if i > table.getn( t_ActionData ) then
			break;
		end
		local btn		= getglobal("ChatActionFrameItem"..index);
		local btnTex	= getglobal("ChatActionFrameItem"..index.."Icon");
		local nameFont	= getglobal("ChatActionFrameItem"..index.."NameFont");
		local IconPath = "uires\\ZhuJieMian\\LiaoTian\\DongZuo\\"..t_ActionData[i][1]..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		btn:SetClientUserData(0,t_ActionData[i][1]);
		btn:SetClientUserData(1,i);
		btnTex:SetTexture(IconPath);
		nameFont:SetText( t_ActionData[i][2] );
		btn:Show();
		index = index + 1;
	end
	for i = index,ONE_PAGE_NUM do
		local btn = getglobal("ChatActionFrameItem"..i);
		btn:Hide();
	end
	ChatActionFramePageBkgBtnPageFont:SetText("第"..nPage.."页");
end

function ChatActionFrameOnShow()
	tPrivateData.nCurPage = 1;
	ChatActionSetShowData( tPrivateData.nCurPage );
end

function ChatActionFrameLeftPageBtn_OnClick()
	if tPrivateData.nCurPage > 1 then
		tPrivateData.nCurPage = tPrivateData.nCurPage - 1;
		ChatActionSetShowData( tPrivateData.nCurPage );
	end
end

function ChatActionFrameRightPageBtn_OnClick()
	local maxPage = math.floor( (table.getn(t_ActionData)-1)/ONE_PAGE_NUM ) + 1;
	if tPrivateData.nCurPage < maxPage then
		tPrivateData.nCurPage = tPrivateData.nCurPage + 1;
		ChatActionSetShowData( tPrivateData.nCurPage );
	end
end

function ChatActionFrameToLastPageBtn_OnClick()		
	local maxPage = math.floor( (table.getn(t_ActionData)-1)/ONE_PAGE_NUM ) + 1;
	if tPrivateData.nCurPage < maxPage then
		tPrivateData.nCurPage = maxPage;
		ChatActionSetShowData( tPrivateData.nCurPage );
	end
end

function ChatActionFrameToFirstPageBtn_OnClick()
	if tPrivateData.nCurPage > 1 then
		tPrivateData.nCurPage = 1;
		ChatActionSetShowData( tPrivateData.nCurPage );
	end
end