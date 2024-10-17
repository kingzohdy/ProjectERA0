-- Ц���������
OLD_FACE_TEXT = "#001#002#003#004#005#006#007#008#009".."\n"..
							"#010#011#012#013#014#015#016#017#018".."\n"..
							"#019#020#021#022#023#024#025#026#027".."\n"..
							"#028#029#030#031#032#033#034#035#036".."\n"..
							"#037#038#039#040#041#042#043#044#045";
--[[
FACE_TEXT = "#001#002#003#004#005#006#007#008#009#010\n"..
			"#011#012#013#014#015#016#017#018#019#020\n"..
			"#021#022#023#024#025#026#027#028#029#030\n"..
			"#031#032#033#034#035#036#037#038#039#040\n"..
			"#041#042#043#044#045#046#047#048#049#050";
--]]
FACE_TEXT = "#001#002#003#004#005#006#007#008#009#010\n"..
			"#011#012#013#014#015#016#018#019#020#021\n"..
			"#022#023#024#025#026#027#028#029#030#031\n"..
			"#032#033#034#035#036#037#038#039#040#041\n"..
			"#042#043#044#045#046#047#048#049#050#051\n"..
			"#052#053#054#055#056#057#058#059#060#061\n"..
			"#062#063#064#065";

BIAO_QING_HILIGHT_TEX_PATH = [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]];
t_biaoQingHightUV = { ["x"] = 475, ["y"] = 42, ["width"] = 27, ["height"] = 23 };

function ChatFaceFrame_OnLoad()
	local rich = getglobal( this:GetName().."_Rich" );
	rich:SetFaceTexture( [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]] );
	rich:SetFaceTexUV( t_biaoQingHightUV["x"], t_biaoQingHightUV["y"], t_biaoQingHightUV["width"], t_biaoQingHightUV["height"] );
end

function ChatFaceFrame_OnShow()
	local rich = getglobal( this:GetName().."_Rich" );
	if rich:GetTextLines() > 0 then
		return;		
	end
	
	util.CompleteShowAllTextOnRich( {	["rich"] = rich:GetName(), ["text"] = FACE_TEXT, ["frame"] = this:GetName(), 
										["color"] = { r = 255, g = 255, b = 255 }, } );
	util.AdjustBigtipsScreenPosEx( { tipsFrame = this:GetName(), button = "CBFFaceBtn" } );
end

function ChatFaceFrame_Rich_OnEnter()
	if arg2 ~= "RTOT_FACE_ONENTER" then
		return;
	end

	if arg1 < 1 then 
		return;
	end

	--�������
	local nCode = arg1;
	local szCode;
	if nCode >= 100 then
		szCode = "#"..tostring(nCode);
	elseif nCode >= 10 then
		szCode = "#0"..tostring(nCode);
	elseif nCode > 0 then
		szCode = "#00"..tostring(nCode);
	end
	local t_faceTable = GetRototFaceTable();
	for i = 1, table.getn( t_faceTable ) do
	   if t_faceTable[i][1] == 	szCode then	
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "#"..szCode.." "..t_faceTable[i][2], 
								frame = this:GetParent(), button = "cursor" } );
	   end
	end
end

-- ����������Ц������
function MainChatFaceFrame_OnShow()
	local rich = getglobal( this:GetName().."_Rich" );
	if rich:GetTextLines() > 0 then
		return;		
	end
	
	util.CompleteShowAllTextOnRich( {	["rich"] = rich:GetName(), ["text"] = FACE_TEXT, ["frame"] = this:GetName(),
										["color"] = { r = 255, g = 255, b = 255 }, } );
	this:SetPoint( "BottomLeft", "MainChatFrame_FaceBtn", "TopRight", 0, 0 );
end

function MainChatFaceFrame_Rich_OnClick()
	if arg1 < 1 then return end

	--�������
	local nCode = arg1;
	local szCode;
	if nCode >= 100 then
		szCode = "#"..tostring(nCode);
	elseif nCode >= 10 then
		szCode = "#0"..tostring(nCode);
	elseif nCode > 0 then
		szCode = "#00"..tostring(nCode);
	end
	
	local frame			= getglobal( this:GetParent() );
	local inputFrame	= getglobal( "ChatInsertFrame" );
	local laBaInputFrame= getglobal( "ChatBugleFrame" );
	if laBaInputFrame:IsShown() then
		CBFChatInsert:AddText( szCode );
		SetFocusFrame("CBFChatInsert");
	elseif inputFrame:IsShown() then
		CSChatInsert:AddText( szCode );
		SetFocusFrame("CSChatInsert");
	else
		ShortCutSendMessage( { ["text"] = szCode } );
		frame:Hide();
	end
end

-- С����������Ц������
--����������ʱ,������༭������ʾ����Ӧ�ı������
function CFF_ShowFaceRich_OnClick()
	if arg1 < 1 then return end

	--�������
	local nCode = arg1;
	local szCode;
	if nCode >= 100 then
		szCode = "#"..tostring(nCode);
	elseif nCode >= 10 then
		szCode = "#0"..tostring(nCode);
	elseif nCode > 0 then
		szCode = "#00"..tostring(nCode);
	end
	
	local frame = getglobal("ChatFaceFrame");
	CBFChatInsert:AddText( szCode );
	SetFocusFrame("CBFChatInsert");
end

-- ��ݷ��Ա༭��
function ChatShortcutSendMsgFrame_ChatEnterPressed()
	local frame= getglobal( "ChatInsertFrame" );
	if (frame:IsShown()) then
		Private_QuitSendChatMessage();
	elseif not frame:IsShown() then
		frame:Show();
		SetFocusFrame("CSChatInsert");
	end

	ClearSetLink();
end

MAX_QUICK_CHAT_MSG = 6;
local t_Lines = {};

function LoadMsg()
	t_Lines = {};

	local data = ChatManager:getChatConfigData();
	if data == nil then 
		return 
	end

	while true do
		local i = 0;
		i = string.find(data,"\n",i+1)
		
		if i == nil then
			table.insert(t_Lines,data);
			break
		end
		table.insert(t_Lines,string.sub(data,0,i-1));
		data = string.sub(data,i+1);
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	for i = 1, table.getn(t_Lines) do
		local edit = getglobal("CSSMFInsertEdit"..(i));
		if edit ~= nil then
			edit:SetText(t_Lines[i]);
		else
			break;
		end
	end
end

function SaveMsg()
	t_Lines = {};
	for i = 1, MAX_QUICK_CHAT_MSG do
		local edit = getglobal("CSSMFInsertEdit"..i);
		table.insert( t_Lines, edit:GetText() );
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	local szWriteText = CSSMFInsertEdit1:GetText().."\n"..CSSMFInsertEdit2:GetText().."\n"..CSSMFInsertEdit3:GetText().."\n"
	..CSSMFInsertEdit4:GetText().."\n"..CSSMFInsertEdit5:GetText().."\n"..CSSMFInsertEdit6:GetText();
	ChatManager:setChatConfigData(szWriteText,string.len(szWriteText));
end

-- ��ʾ��ݷ������
function ChatShortcutSendMsgFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local bFirstEmpty = true;
	for i = 1, math.min( table.getn(t_Lines), MAX_QUICK_CHAT_MSG ) do
		local edit = getglobal("CSSMFInsertEdit"..i);
		edit:SetText(t_Lines[i]);
		if edit:GetText() == "" and bFirstEmpty then
			SetFocusFrame( edit:GetName() );
			bFirstEmpty = false;
		end
	end

	if bFirstEmpty then
		local lastEdit = getglobal( "CSSMFInsertEdit"..( table.getn(t_Lines) + 1 ) );
		if lastEdit ~= nil then
			SetFocusFrame( lastEdit:GetName() );
		else
			SetFocusFrame( "CSSMFInsertEdit1" );
		end
	end
end


-- ���ȡ����ť
function CSSMFCancelBtn_OnClick()
	for i = 1, MAX_QUICK_CHAT_MSG do
		local edit = getglobal("CSSMFInsertEdit"..i);
		edit:Clear();
	end
	LoadMsg();
	ChatShortcutSendMsgFrame:Hide();
end

-- ���ȷ����ť
function CSSMFSureBtn_OnClick()
	ShowMidTips("��ݷ������óɹ�");
	SaveMsg();
	ChatShortcutSendMsgFrame:Hide();
end


-- �µı���Ͷ������
local t_ChatFaceAndActionFrameControl = { ["relFrame"] = "", ["page"] = 1 };
local ONE_PAGE_MAX_ACTION_NUM = 10;

function GetChatFaceAndActionFrameControl()
	return t_ChatFaceAndActionFrameControl;
end

t_ChatFaceAndActionFrameControl["getCurPage"] = 
function ( self )
	return self["page"];
end

t_ChatFaceAndActionFrameControl["setCurPage"] = 
function ( self, data )
	self["page"] = data["page"];
end

t_ChatFaceAndActionFrameControl["getRelFrame"] = 
function ( self )
	return self["relFrame"];
end

function Get_ChatFaceAndActionFrame_MaxPage()
	local t_ActionData	= GetActionData();
	return math.ceil( table.getn( t_ActionData ) / ONE_PAGE_MAX_ACTION_NUM );
end

function ChatFaceAndActionFrame_OnLoad()
	local rich = getglobal( this:GetName().."_FaceRich" );
	rich:SetFaceTexture( [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]] );
	rich:SetFaceTexUV( t_biaoQingHightUV["x"], t_biaoQingHightUV["y"], t_biaoQingHightUV["width"], t_biaoQingHightUV["height"] );
	rich:setTwoFaceInterval( 3.5, 1 );
end

-- ��ʾ���
function ChatFaceAndActionFrame_OnShow()
	this:SetPoint( "bottomleft", "$parent", "bottomleft", 166, -147 );
	t_ChatFaceAndActionFrameControl:setCurPage( { ["page"] = 1 } );
	Update_ChatFaceAndActionFrame_FaceSubFrame();
	Update_ChatFaceAndActionFrame_ActionSubFrame();
	Update_ChatFaceAndActionFrame_PageNum();
end

-- ��һҳ��ť
function ChatFaceAndActionFrame_PrePageButton_OnClick()
	local nCurPage	= t_ChatFaceAndActionFrameControl:getCurPage();
	if nCurPage == 1 then
		return;
	end

	t_ChatFaceAndActionFrameControl:setCurPage( { ["page"] = nCurPage - 1 } );
	Update_ChatFaceAndActionFrame_FaceSubFrame();
	Update_ChatFaceAndActionFrame_ActionSubFrame();
	Update_ChatFaceAndActionFrame_PageNum();
end

-- ��һҳ��ť
function ChatFaceAndActionFrame_NextPageButton_OnClick()
	local nCurPage	= t_ChatFaceAndActionFrameControl:getCurPage();
	local nMaxPage	= Get_ChatFaceAndActionFrame_MaxPage();
	if nCurPage == nMaxPage then
		return;
	end

	t_ChatFaceAndActionFrameControl:setCurPage( { ["page"] = nCurPage + 1 } );
	Update_ChatFaceAndActionFrame_FaceSubFrame();
	Update_ChatFaceAndActionFrame_ActionSubFrame();
	Update_ChatFaceAndActionFrame_PageNum();
end

-- ����ҳ��
function Update_ChatFaceAndActionFrame_PageNum()
	local nCurPage	= t_ChatFaceAndActionFrameControl:getCurPage();
	ChatFaceAndActionFrame_PageButton:SetText( "��"..nCurPage.." / "..Get_ChatFaceAndActionFrame_MaxPage().."ҳ" );
end

-- ����Ц�������
function Update_ChatFaceAndActionFrame_FaceSubFrame()
	local nCurPage	= t_ChatFaceAndActionFrameControl:getCurPage();
	local rich		= getglobal( "ChatFaceAndActionFrame_FaceRich" );
	if nCurPage == 1 then
		--rich:SetDispPos( 0 )
		if rich:GetTextLines() > 0 then
			return;		
		end
		util.CompleteShowAllTextOnRich( {	["rich"] = rich:GetName(), ["text"] = FACE_TEXT, 
											["color"] = { r = 255, g = 255, b = 255 }, } );		
	else
		--rich:SetDispPos( 4 * rich:GetLineHeight(1) )
	end
end

-- ���¶��������
function Update_ChatFaceAndActionFrame_ActionSubFrame()
	local nCurPage		= t_ChatFaceAndActionFrameControl:getCurPage();
	local t_ActionData	= GetActionData();
	local nStartIndex	= ( nCurPage - 1 ) * ONE_PAGE_MAX_ACTION_NUM + 1;
	local nBtnIndex		= 1;
	for i = nStartIndex, ONE_PAGE_MAX_ACTION_NUM * nCurPage do
		if i > table.getn( t_ActionData ) then
			break;
		end

		local btn		= getglobal( "ChatFaceAndActionFrameItem"..nBtnIndex );
		local btnTex	= getglobal( btn:GetName().."Icon" );
		local nameFont	= getglobal( btn:GetName().."NameFont" );
		local IconPath	= "uires\\ZhuJieMian\\LiaoTian\\DongZuo\\"..t_ActionData[i][1]..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end

		btn:SetClientUserData( 0,t_ActionData[i][1] );
		btn:SetClientUserData(1,i);
		btnTex:SetTexture( IconPath );
		nameFont:SetText( t_ActionData[i][2] );
		btn:Show();
		nBtnIndex = nBtnIndex + 1;
	end

	for i = nBtnIndex, ONE_PAGE_MAX_ACTION_NUM do
		local btn = getglobal( "ChatFaceAndActionFrameItem"..i );
		btn:Hide();
	end
	
end