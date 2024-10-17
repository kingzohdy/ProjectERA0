
local ONE_PAGE_NUM = 9;

local tPrivateData = {};

local ACTIONTEXT = "@@#%d,%d,%d#n";
local t_ActionData = { 
						{10700,"����","/cheer"},
						{10701,"����","/kiss"},
						{10702,"����","/dance"},
						{10703,"����","/encourage"},
						{10704,"װ��","/die"},
						{10705,"����","/attack"},
						{10706,"����","/applause"},
						{10707,"ˣ��","/cool"},
						{10708,"����","/cadge"},
						{10709,"��Ҿ","/thumb"},
						{10710,"����","/however"},
						{10711,"����","/hand"},
						{10712,"��˼","/think"},
						{10713,"ʹ��","/sad"},
					};
local t_ActionString = {
						[10700] = {"%s����ѹ�ֲ�ס���ĵ��˷ܣ�����%s����һ�������㵸�Ļ���","%s�鲻�Խ���Ϊ�Լ�����ò����ץ��"},
						[10701] = {"%s�����������%s�����һ̧ͷ��������һ���֣����͡���Ϭ����һ�����ǣ�","%s����Χ�Ĵ����������׳�һ�����ǣ���л���Ƕ������ʵ��"},
						[10702] = {"%s��Ѫ��������������%s���쳡������ҵ��������","�����Ѿ��޷���ֹ%s�����������ˡ�"},
						[10703] = {"%s��%s�����������Լ������ǽ�����һ�����壡","%sһҧ���أ����Ķ��Կ������ȵ�������Σ�"},
						[10704] = {"%s��С�Ŀ�����%s������������ŵû�����ȥ","%s���վ���ʱ�����Լ�����̫�������������"},
						[10705] = {"%s�ݺ�һȭ���ȥ��%sʹ�ö����ˣ�","%s����ǰ������һȭ�����ɹ��Ĳ�Ӭ����������ˣ�"},
						[10706] = {"��ÿ��100���ҼƷѣ�%s������%s��������־õ�����","%s������Χ�Ĵ������ߵ���������л���Ƕ���ĳ�ݣ�"},
						[10707] = {"%s������һ���֣���%s˵����Ҫ����������ֻ�Ǹ���˵","%s�ڲ�������ּ䣬holdס���������棡"},
						[10708] = {"%sһ�ѹ���%s��ǰ�������дǵ�˵�������ʮ�ֻ�������ң�","%sʹ��Ĺ��ڵ��ϣ�˺���ѷεغ𵽣���Ǯ�������ˣ�"},
						[10709] = {"%s�������%s���˸���ͻȻ�����Ķ��ĸо�","Ϊ�˸�л���ڶ�%s�İ�����������������˸���"},
						[10710] = {"%s��������%s̯��̯�֣���ʾ��Ҳ�����Ű����ˡ�","%s���ε�̯̯�֣���ʾ�������������㣬�޷�һ��������ڵ�����"},
						[10711] = {"�������躰%s�ؼҳԷ���ֻ�����εĳ�%s���ֵ���","%s���˻��֣������������Ʋʣ�"},
						[10712] = {"%s������˼����Ϊ�λ���%s��ô���ι�״����","%s��˼������⣬������ô���������ǲ�����Ĵ�"},
						[10713] = {"%s��%s��ǰʹ���ʧ����޵�����˵����Ḻ��ģ�","%sʹ��ֱ����£�����ֱð�亹������Ͻ��Ҹ�������"},
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
		ShowMidTips("��ǰ״̬���ܽ��иò���");
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
					szText = string.format( t_ActionString[actionId][1],"��",targetPlayer:getName() );
				else
					szText = string.format( t_ActionString[actionId][2],"��" );
				end
			elseif mainplayer:GetID() == targetID then
				szText = string.format( t_ActionString[actionId][1],player:getName(),"��" );
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
	ChatActionFramePageBkgBtnPageFont:SetText("��"..nPage.."ҳ");
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