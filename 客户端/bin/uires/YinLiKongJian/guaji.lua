guajiFrameWidth=677;
guajiFrameHeight=471;
HaoYouAnniunums= 17;--��ʾ�ĺ��Ѱ�ť����
LiaoTianSlidelength = 269;--���컬������߶�
nHaoyouHuadongindex	=0;
nWorldShieldState	= false;
nClanShieldState	= false;
nPrivateShieldState	= false;
guajiFrametipsisShow    = true;--������ʾֻ��ʾһ��
FoucsName		= "";
function guajiFrametips()
	if guajiFrametipsisShow then
		AddMessage( "�������10���Ӻ�ʼ�ۼ����߾���",0 );
	end
	guajiFrametipsisShow = false;
end
function guajiFrameOnLoad()
	this:RegisterEvent("GE_CHATMSG");
	this:RegisterEvent("GE_CHATREQ");
	this:RegisterEvent("GE_ENTERCHARROOM");
	this:RegisterEvent("GE_TIPS_UPDATE");
	this:RegisterEvent("GE_KICK_OFF")
	this:RegisterEvent("GE_RELATION_UPDATE")
	this:RegisterEvent("GE_EXP_UPATE")
	this:RegisterEvent("SHOW_ROOM_LIST")
	for i=2,HaoYouAnniunums do
	  local jbutton=getglobal("HaoYouAnNiu"..i);
	  local j=i-1;
	 jbutton:SetPoint( "topleft", "HaoYouAnNiu"..j, "bottomleft", 0, 0 );
	end
	PindaoAllHide()
	RelationAnniuHide()
end

function guajiFrameOnShow()
	MainPlayer:setGameState(CHATROOM_STATE);
	local font=getglobal(this:GetName().."Chatroomfont");
	font:SetText("������"..nCurrentConnectindex);
	font=getglobal(this:GetName().."NameFont");
	font:SetText(MainPlayer:getName());
	font=getglobal(this:GetName().."levelFont");
	font:SetText("�ȼ�: "..MainPlayer:getLevel().." ��");
	font=getglobal(this:GetName().."CareerFont");
	font:SetText("ְҵ: "..IdToname(MainPlayer:getCareer()));
	selectroleindex=0;
	Zonghepindao_OnClick();
	HaoYouAnNiu_OnClick();
	JingYanBtnjingyan:SetSize(MainPlayer:getOffExplength() ,8);
end

function PindaoAllHide()
	Zonghepindao:DisChecked();
	ShiJiepindao:DisChecked();
	GongHuipindao:DisChecked();
	SiLiaopindao:DisChecked();
	ChatRoompindao:DisChecked();
end

function RichtextAllHide()
	ZonghepindaoRichText:Hide();
	ShijiepindaoRichText:Hide();
	GonghuipindaoRichText:Hide();
	SiliaopindaoRichText:Hide();
	ChatRoompindaoRichText:Hide();
end

function AllRichtextClear()
	ZonghepindaoRichText:Clear();
	ShijiepindaoRichText:Clear();
	GonghuipindaoRichText:Clear();
	SiliaopindaoRichText:Clear();
	ChatRoompindaoRichText:Clear();
end

function dealchatreq()--�����ɷ��������ص�˽����Ϣ
	if ChatManager:getChatreqType() ~= CL_CHAT_PRIVATE and ChatManager:getChatreqType() ~= CL_CHAT_FRIEND then -- �����ҵ���Ϣchatres�Ѿ���������ͺ��Ե�
		return;
	end
	local str="#A010"
	SiliaopindaoRichText:AddText(str.."#cFF0000��#n#c0054FF��#n".."[#L"..ChatManager:getChatreqrolename().."#n]".."˵: "..ChatManager:getChatreqContent(),160,0,150);
	ZonghepindaoRichText:AddText(str.."#cFF0000��#n#c0054FF��#n".."[#L"..ChatManager:getChatreqrolename().."#n]".."˵: "..ChatManager:getChatreqContent(),160,0,150);

end

function dealchatmsg()
	if ChatManager:getChatmsgtype() == CL_CHAT_WORLD then
		ShijiepindaoRichText:AddText(msgcontent(),0,150,30);
		ZonghepindaoRichText:AddText(msgcontent(),0,150,30);
	elseif ChatManager:getChatmsgtype() == CL_CHAT_MUD_ROOM then
		ZonghepindaoRichText:AddText(msgcontent(),160,0,150);
		ChatRoompindaoRichText:AddText(msgcontent(),160,0,150);
	elseif ChatManager:getChatmsgtype() == CL_CHAT_PRIVATE  then
		SiliaopindaoRichText:AddText(msgcontent(),160,0,150);
		ZonghepindaoRichText:AddText(msgcontent(),160,0,150);
	elseif ChatManager:getChatmsgtype() == CL_CHAT_FRIEND then
		SiliaopindaoRichText:AddText(msgcontent(),255,120,0);
		ZonghepindaoRichText:AddText(msgcontent(),255,120,0);
	elseif ChatManager:getChatmsgtype() == CL_CHAT_CLAN then
		GonghuipindaoRichText:AddText(msgcontent(),0,100,200);
		ZonghepindaoRichText:AddText(msgcontent(),0,100,200);
	end
	putLiaoTianSldertoEnd();--ÿ���¼����ݶ���slider�õ�
	putHistoryFrameSldertoEnd();
end

function putLiaoTianSldertoEnd()
	local richtext = getFoucsRichtext();
	if richtext:GetTextLines() > richtext:GetViewLines() then
		LiaoTianHuadongSlider:Show();
		local nums = richtext:GetTextLines() - richtext:GetViewLines();
		local length= richtext:GetViewLines() / richtext:GetTextLines() * LiaoTianSlidelength;
		LiaoTianHuadongSliderTexture:SetSize(14,length);
		LiaoTianHuadongSlider:SetMaxValue(5*nums);--���컬������steps�趨Ϊ5
		LiaoTianHuadongSlider:SetValue(5*nums);
	else
		LiaoTianHuadongSlider:Hide();	
	end	
end

function LiaoTian_ScrollBarValueChange()
	if this:IsShown() then
		local curindex= (this:GetMaxValue() - this:GetValue()) / 5;
		local richtext = getFoucsRichtext();
		richtext:ScrollEnd();
		for i = 1,curindex do
			richtext:ScrollUp();
		end
	end
end

function putHistoryFrameSldertoEnd()
	local richtext = getglobal("HistoryFrameRichText");
	if richtext:GetTextLines() > richtext:GetViewLines() then
		HistoryFrameHuadongSlider:Show();
		local nums = richtext:GetTextLines() - richtext:GetViewLines();
		local length= richtext:GetViewLines() / richtext:GetTextLines() * 142;
		HistoryFrameHuadongSliderTexture:SetSize(14,length);
		HistoryFrameHuadongSlider:SetMaxValue(5*nums);--���컬������steps�趨Ϊ5
		HistoryFrameHuadongSlider:SetValue(5*nums);
	else
		HistoryFrameHuadongSlider:Hide();	
	end	
end

function HistoryFrame_ScrollBarValueChange()
	if this:IsShown() then
		local curindex= (this:GetMaxValue() - this:GetValue()) / 5;
		local richtext = getglobal("HistoryFrameRichText");
		richtext:ScrollEnd();
		for i = 1,curindex do
			richtext:ScrollUp();
		end
	end
end

function getstr()
	if ChatManager:getChatmsgtype() == CL_CHAT_MUD_ROOM then
		return "#A017";
	elseif ChatManager:getChatmsgtype() == CL_CHAT_WORLD then
		return "#A002";
	elseif ChatManager:getChatmsgtype() == CL_CHAT_CLAN then
		return "#A008";
	elseif ChatManager:getChatmsgtype() == CL_CHAT_FRIEND then
		return "#A016";
	end
	return "#A010";
end

function msgcontent()
        if  ChatManager:getChatmsgrolename()~= MainPlayer:getName() then
		local tpestr=getstr();
		if ChatManager:getChatmsgtype() == CL_CHAT_PRIVATE or ChatManager:getChatmsgtype() == CL_CHAT_FRIEND then
			return tpestr.."[#L"..ChatManager:getChatmsgrolename().."#n]".." #c0054FF��#n#cFF0000��#n˵: "..ChatManager:getChatmsgContent();
		end
		return tpestr.."[#L"..ChatManager:getChatmsgrolename().."#n]˵".." : "..ChatManager:getChatmsgContent();
	end
	return "#A017".."#cFF0000��#n˵".." : "..ChatManager:getChatmsgContent();

end

function getPlayer()
	if  relationindex == CHATROOMTYPE then
	 return ChatRoomMgr:getPlayerInfo(selectroleindex);
	elseif relationindex == FRIENDTYPE then
	 return RelationMgr:getFriendInfo(selectroleindex);
	end
	return RelationMgr:getClanMemberInfo(selectroleindex);
end

local nLastRequestLeaveRoomTime = 0;
function LiaoTianShiNiuOnClick()
	if os.clock() - nLastRequestLeaveRoomTime > 1 then
		MainPlayer:requestLeaveChatRoom();
		nLastRequestLeaveRoomTime = os.clock();
	end
end

function LiaoTianshiEdit_OnEnterPressed()
	if  LiaoTianshiEdit:GetText() == "" then
		return
	end
	if pindaoindex == CHATROOM_CHANNEL or pindaoindex == ZONGHE_CHANNEL then
		MudMgr:requestSendChat(8,"",LiaoTianshiEdit:GetText(),0);
	elseif pindaoindex == PRIVATE_CHANNEL then
		if PingDaoFrameobjectName:GetText() ~= "" then
			MudMgr:requestSendChat(2,PingDaoFrameobjectName:GetText(),LiaoTianshiEdit:GetText(),PingDaoFrameobjectName:GetClientID());
		else
			AddMessage("���Ҽ���˫����Ҫѡ���˽�Ķ���!");
		end
	elseif pindaoindex == WORLD_CHANNEL then
		AddMessage("�����ռ�����Ƶ�����ܷ����κ���Ϣ��ֻ�ܲ鿴��Ϸ�����������Ƶ������������!",0);
	else
		AddMessage("�����ռ乫��Ƶ�����ܷ����κ���Ϣ��ֻ�ܲ鿴��Ϸ�й�����ҵ���������!",0);
	end
	LiaoTianshiEdit:Clear();
	SetFocusFrame("LiaoTianshiEdit");
	if ChatFaceFrame:IsShown() then
		ChatFaceFrame:Hide();
	end
end
function LiaoTianshiEdit_OnFocusLost()
		ChatFaceFrame:Hide();
end

function Zonghepindao_OnClick()
	pindaoindex=ZONGHE_CHANNEL;
	RichtextAllHide()
	PindaoAllHide();
	Zonghepindao:Checked();
	ZonghepindaoRichText:Show();
	putLiaoTianSldertoEnd();
end

function Shijiepindao_OnClick()
	pindaoindex=WORLD_CHANNEL;
	RichtextAllHide()
	PindaoAllHide();
	ShiJiepindao:Checked();
	ShijiepindaoRichText:Show();
	putLiaoTianSldertoEnd();
end

function Gonghuipindao_OnClick()
	pindaoindex=CLAN_CHANNEL;
	RichtextAllHide()
	PindaoAllHide();
	GongHuipindao:Checked();
	GonghuipindaoRichText:Show();
	putLiaoTianSldertoEnd();
end

function Siliaopindao_OnClick()
	pindaoindex=PRIVATE_CHANNEL;
	RichtextAllHide()
	PindaoAllHide();
	SiLiaopindao:Checked();
	SiliaopindaoRichText:Show();
	putLiaoTianSldertoEnd();
end

function ChatRoompindao_OnClick()
	pindaoindex=CHATROOM_CHANNEL;
	RichtextAllHide();
	PindaoAllHide();
	ChatRoompindao:Checked();
	ChatRoompindaoRichText:Show();
	putLiaoTianSldertoEnd();
end



function RoleOnClickDialogWatch_OnClick()
	OfflineWatchPlayer:sendWatchRequest( FoucsName );
	RoleOnClickDialog:Hide();
end

function RoleOnClickDialog_OnLoad()
	RoleOnClickDialogChatFont:SetText("˽��");
	RoleOnClickDialogStealFont:SetText("͵ȡ����");
	RoleOnClickDialogAddFriendFont:SetText("��Ӻ���");
	RoleOnClickDialogDeleteFriendFont:SetText("ɾ������");
	RoleOnClickDialogWatchFont:SetText("�۲�Է�");
end


function TipRicTextupdate()
       local num=MudMgr:getTextTipsnums();
       if num >0 then
       if  MainPlayer:getGameState() == CHATROOM_STATE then
         for i=1,num do
		ZonghepindaoRichText:AddText("#A006 "..MudMgr:getTextTip(i),255,0,0);
		ShijiepindaoRichText:AddText("#A006 "..MudMgr:getTextTip(i),255,0,0);
		GonghuipindaoRichText:AddText("#A006 "..MudMgr:getTextTip(i),255,0,0);
		SiliaopindaoRichText:AddText("#A006 "..MudMgr:getTextTip(i),255,0,0);
		ChatRoompindaoRichText:AddText("#A006"..MudMgr:getTextTip(i),255,0,0);
		HistoryFrameRichText:AddText("#A006 "..MudMgr:getTextTip(i),255,0,0);
		putLiaoTianSldertoEnd();--ÿ���¼����ݶ���slider�õ�
		putHistoryFrameSldertoEnd();
	  end
	end
	MudMgr:ClearTexttip();
       end
end

function RoleOnClickDialogChat_OnClick()
	--HaoYouAnNiuTemplate_OnDoubleClick();
	Siliaopindao_OnClick();
	PingDaoFrameobjectName:SetText(FoucsName);
	--PingDaoFrameobjectName:SetClientID(RolePlayer.m_Onlinetype);
	SetFocusFrame("LiaoTianshiEdit");
	RoleOnClickDialog:Hide();
end

function RoleOnClickDialogSteal_OnClick()
	if	MainPlayer:getLevel() < 21 then
		AddMessage("���ĵȼ�δ�ﵽ21��������͵ȡ���飡",0);
	elseif RelationMgr:IsFriend( FoucsName ) then
		local RolePlayer =  RelationMgr:getFriendInfoByName( FoucsName );
		if RolePlayer.m_uLevel < 21 then
			AddMessage("�Է��ĵȼ�δ�ﵽ21��������͵ȡ���飡",0);
		else
			MainPlayer:requestStealexp(RolePlayer.m_RoleID,RolePlayer.m_RoleName);
		end
	else
		AddMessage("�Է�������ĺ��ѣ�",0);--RolePlayer=ChatRoomMgr:getPlayerInfo(selectroleindex);
	end
	RoleOnClickDialog:Hide();
end


function RoleOnClickDialogAddFriend_OnClick()
	--local RolePlayer = getPlayer();
	if RelationMgr:IsFriend( FoucsName ) then
		AddMessage("�Է��Ѿ��������",0);
	else
		--AddFriendFrameName:SetText( FoucsName );
		--AddFriendFramelevel:SetText("�ȼ�: "..RolePlayer.m_uLevel);
		--AddFriendFramecareer:SetText("ְҵ: "..IdToname(RolePlayer.m_uCareer));
		AddFriendFrameTipRich:SetText("�������".."#c6EFF00"..FoucsName.."#n".."Ϊ����!",255,230,150);
		AddFriendFrame:Show();
		SetFocusFrame("AddFriendFrameboxEditbox");
	end
	RoleOnClickDialog:Hide();
end

function RoleOnClickDialogDeleteFriend_OnClick()
	local RolePlayer = getPlayer();
	if  RelationMgr:IsFriend( RolePlayer.m_RoleName) then
		RelationMgr:requestDeleteFriend(RolePlayer.m_RoleID);
	else
		AddMessage("�Է�������ĺ��ѣ�",0);
	end
	--TODO HERE ȡ��ѡ�У���������ˢ��
	RoleOnClickDialog:Hide();
end

function FaceBtn_OnClick()
	if ChatFaceFrame:IsShown() then
		ChatFaceFrame:Hide();
	else
		ChatFaceFrame:Show();
		SetFocusFrame("LiaoTianshiEdit");
	end
end

local BIAO_QING_HILIGHT_TEX_PATH = [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]];
local t_biaoQingHightUV = { ["x"] = 475, ["y"] = 42, ["width"] = 27, ["height"] = 23 };

function ChatFaceFrame_OnLoad()
	local rich = getglobal( "CFF_ShowFaceRich" );
	rich:SetFaceTexture( [[uires\TuPianLei\LiaoTian\BiaoQingTuBiao\popoface.dds]] );
	rich:SetFaceTexUV( t_biaoQingHightUV["x"], t_biaoQingHightUV["y"], t_biaoQingHightUV["width"], t_biaoQingHightUV["height"] );
end

function ChatFaceFrame_OnShow()
	local rich = getglobal("CFF_ShowFaceRich");
	if rich:GetTextLines() > 0 then
		return;
	else
		local szFaceText = "#001#002#003#004#005#006#007#008#009#010\n"..
			"#011#012#013#014#015#016#018#019#020#021\n"..
			"#022#023#024#025#026#027#028#029#030#031\n"..
			"#032#033#034#035#036#037#038#039#040#041\n"..
			"#042#043#044#045#046#047#048#049#050#051\n"..
			"#052#053#054#055#056#057#058#059#060#061\n"..
			"#062#063#064#065";
		rich:AddText( szFaceText, 255, 255, 255);
	end
end

--����������ʱ,������༭������ʾ����Ӧ�ı������
function CFF_ShowFaceRich_OnClick()
	ChatFaceFrame:Show();
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
	
	LiaoTianshiEdit:AddText( szCode );
	SetFocusFrame("LiaoTianshiEdit");
end

function CFF_ShowFaceRich_OnEnter()
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

--��������Ӧ�¼�

function getFoucsRichtext()
	if pindaoindex == ZONGHE_CHANNEL then
		return	ZonghepindaoRichText;
	elseif pindaoindex == WORLD_CHANNEL then
		return	ShijiepindaoRichText;
	elseif pindaoindex == CLAN_CHANNEL then
		return	GonghuipindaoRichText;
	elseif pindaoindex == CHATROOM_CHANNEL then
		return ChatRoompindaoRichText;
	else
		return SiliaopindaoRichText;
	end
end
 

-- ���������� 
function LiaoTianHuadongUp_OnClick()
	if LiaoTianHuadongSlider:IsShown() then
		local pos=LiaoTianHuadongSlider:GetValue() - 5 ;--�趨�����컬������stepsΪ5
		LiaoTianHuadongSlider:SetValue(pos);
	end
end

function LiaoTianHuadongDown_OnClick()
	if LiaoTianHuadongSlider:IsShown() then
		local pos=LiaoTianHuadongSlider:GetValue() + 5 ;--�趨�����컬������stepsΪ5
		LiaoTianHuadongSlider:SetValue(pos);
	end
end

function HistoryFrameUpBtn_OnClick()
	if HistoryFrameHuadongSlider:IsShown() then
		local pos=HistoryFrameHuadongSlider:GetValue() - 5 ;--�趨�����컬������stepsΪ5
		HistoryFrameHuadongSlider:SetValue(pos);
	end
end

function HistoryFrameDownBtn_OnClick()
	if HistoryFrameHuadongSlider:IsShown() then
		local pos=HistoryFrameHuadongSlider:GetValue() + 5 ;--�趨�����컬������stepsΪ5
		HistoryFrameHuadongSlider:SetValue(pos);
	end
end

--������ť
function QingPingAnNiu_OnClick()
	local foucsrichtext=getFoucsRichtext();
	foucsrichtext:Clear();
end

--Ƶ������
function shielddeal()
	WorldShieldChkBtn:setCheckState(nWorldShieldState);
	GonghuiShieldChkBtn:setCheckState(nClanShieldState);
	PrivateShieldChkBtn:setCheckState(nPrivateShieldState);
end

function ShieldFrame_Close()
		ShieldFrame:Hide();
		shielddeal();
end

function Shielbtn_OnClick()
	if ShieldFrame:IsShown() then
		ShieldFrame:Hide();
		shielddeal();
	else
		ShieldFrame:Show();
	end
end

function Historybtn_OnClick()
	putHistoryFrameSldertoEnd();--ÿ�δ���ʷ��Ϣ���涼�õ�
	if HistoryFrame:IsShown() then 
		HistoryFrame:Hide();
	else
		HistoryFrame:Show();
	end
end

function HistoryFrame_Close()
	HistoryFrame:Hide();
end

function guajiFrameOnClick()
	ChatFaceFrame:Hide();
end

function guajiFrameOnHide()
	RoleOnClickDialog:Hide();
	RoleInfoDialog:Hide();
	ShieldFrame:Hide();
	ChatFaceFrame:Hide();
	AddFriendFrame:Hide();
	HistoryFrame:Hide();
	WatchPlayerFrame:Hide();
	guajiMessagebox:Hide();
	WatchPlayerFrame:Hide();
end

function jingyandi_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "���߾���"..MainPlayer:getOfflineExp().."/"..MainPlayer:getMaxOfflineExp(), 
								frame = this:GetParent(), button = "cursor" } );
end

function LiaoTianTouXiang_OnEnter()
	LiaoTianTouXiangHeadBodyTex:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",1,-5);
	LiaoTianTouXiangHeadTexture:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",1,-5);
	LiaoTianTouXiangHeadHairShadowTex:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",1,-5);
	LiaoTianTouXiangHeadHairTexture:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",1,-5);
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "˫���鿴װ��", 
								frame = this:GetParent(), button = "cursor" } );
end

function LiaoTianTouXiang_OnLeave()
	LiaoTianTouXiangHeadBodyTex:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",4,-5);
	LiaoTianTouXiangHeadTexture:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",4,-5);
	LiaoTianTouXiangHeadHairShadowTex:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",4,-5);
	LiaoTianTouXiangHeadHairTexture:SetPoint("bottomleft","LiaoTianTouXiang","bottomleft",4,-5);
	GameTooltipOnLeave();
end

function LiaoTianTouXiang_OnClick()
	OfflineWatchPlayer:sendWatchRequest(MainPlayer:getName());
end

function StartZoneBtn_OnClick()
	MudMgr:startZone();
end

function AddFriendFrame_Close()
	AddFriendFrameboxEditbox:Clear();
	AddFriendFrame:Hide();
end

function AddFriendFrameSurebtn_OnClick()
	--local RolePlayer = getPlayer();
	RelationMgr:requestAddFriend( FoucsName,  AddFriendFrameboxEditbox:GetText());
	AddFriendFrameboxEditbox:Clear();
	AddFriendFrame:Hide();
end

function ResponseAddFriendFrame_OnLoad()
	this:RegisterEvent("GE_REQUEST_FRIEND");
end

function ResponseAddFriendFrame_OnEvent()
	ResponseAddFriendFrameinfo:SetText("#c6EFF00"..RelationMgr:getReqName().."#n".."���������Ϊ�����Ƿ�ͬ�⣿",255,230,150);
	ResponseAddFriendFrameboxinfo:SetText(RelationMgr:getReqInfo(),255,230,150);
	if MainPlayer:getGameState() == CHATROOM_STATE then 
		this:SetPoint( "topleft", "UIClient",  "topleft", "215", "110");
	else
		this:SetPoint( "topleft", "UIClient",  "topleft", "25", "200");
	end
	this:Show();
end

function ResponseAddFriendFrame_OnShow()
end

function ResponseAddFriendFrame_Close()
	RelationMgr:responseAddFriend(RelationMgr:getReqName(),0,RelationMgr:getReqOnlinetype());
	ResponseAddFriendFrame:Hide();
end

function ResponseAddFriendFrameSurebtn_OnClick()
	RelationMgr:responseAddFriend(RelationMgr:getReqName(),1,RelationMgr:getReqOnlinetype());
	ResponseAddFriendFrame:Hide();
end


function GongGongAnNiu_OnClick()
	HaoYouAnNiuTemplateFontAllhide()
	relationindex=CHATROOMTYPE;
	RelationAnniuupdate();
	GongGongAnNiu:Checked();
end

function ClanAnNiu_OnClick()
	HaoYouAnNiuTemplateFontAllhide()
	relationindex=CLANTYPE;
	RelationAnniuupdate();
	ClanAnNiu:Checked();
end

function HaoYouAnNiu_OnClick()
	HaoYouAnNiuTemplateFontAllhide()
	relationindex=FRIENDTYPE;
	RelationAnniuupdate();
	HaoYouAnNiu:Checked();
end

function HaoYouAnNiuTemplate_OnClick()
	HaoYouAnNiuTemplateAllDisChecked()
	this:Checked();
	selectroleindex=this:GetClientID() + nHaoyouHuadongindex;
	local RolePlayer = getPlayer();
	FoucsName	 = RolePlayer.m_RoleName;
end

function HaoYouAnNiuTemplate_OnMouseDown()
	HaoYouAnNiuTemplateAllDisChecked()
	this:Checked();
	if arg1 == "RightButton" then
		local nX		= GetCursorPosX();
		local nY		= GetCursorPosY();
		RoleOnClickDialog:SetPoint( "topright", "UICLIENT", "topleft", nX, nY );
		RoleOnClickDialog:Show();
	end
	selectroleindex=this:GetClientID() + nHaoyouHuadongindex;
end

function HaoYouAnNiuTemplate_Onlostfouc()
	RoleOnClickDialog:Hide();
end

function HaoYouAnNiuTemplate_OnDoubleClick()
	local RolePlayer=getPlayer();
	if RolePlayer.m_Onlinetype == 0 then
		AddMessage("�Է�������!",0);
	elseif  RolePlayer.m_RoleName == MainPlayer:getName() then
		AddMessage("�������Լ�����!",0);
	else
		Siliaopindao_OnClick();
		PingDaoFrameobjectName:SetText(RolePlayer.m_RoleName);
		PingDaoFrameobjectName:SetClientID(RolePlayer.m_Onlinetype);
		SetFocusFrame("LiaoTianshiEdit");
	end
end

function HaoYouAnNiuTemplate_OnLeave()
	RoleInfoDialog:Hide();
end

function onlinename(i)
	if i == 0 then 
		return "������"
	elseif i==1 then
		return "��Ϸ����"
	else
		return "�����ռ�"
	end
end

function HaoYouAnNiuTemplate_OnEnter()
	RoleInfoDialogRichText:Clear();
	local RolePlayer;
	if relationindex == FRIENDTYPE then
		RolePlayer=RelationMgr:getFriendInfo(this:GetClientID() + nHaoyouHuadongindex);
	elseif relationindex == CHATROOMTYPE then
		RolePlayer=ChatRoomMgr:getPlayerInfo(this:GetClientID() + nHaoyouHuadongindex);
	else
		RolePlayer=RelationMgr:getClanMemberInfo(this:GetClientID() + nHaoyouHuadongindex);
	end
	RoleInfoDialogRichText:AddText(RolePlayer.m_RoleName,255,230,170);
	RoleInfoDialogRichText:AddText("ְҵ:"..IdToname(RolePlayer.m_uCareer),255,230,170);
	RoleInfoDialogRichText:AddText("�ȼ�:"..RolePlayer.m_uLevel,255,230,170);
	RoleInfoDialogRichText:AddText("����״̬:"..onlinename(RolePlayer.m_Onlinetype),255,230,170);	
	RoleInfoDialog:SetPoint( "topright", this:GetName(), "topleft", 0, 0 );
	RoleInfoDialog:Show();
end



function HaoyouHuadongSliderupdate(nums)
	local length = 17 / nums * 378;--�������ܹ�17�������ѻ������򳤶�378
	HaoyouHuadongSliderTexture:SetSize(14,length);
	HaoyouHuadongSlider:SetMaxValue(5*(nums- 17));
	if not HaoyouHuadongSlider:IsShown() then --��һ����ʾ���ѻ�����
		HaoyouHuadongSlider:SetValue(0);
		HaoyouHuadongSlider:Show();
	end
end

function RelationAnniuupdate()
	RelationAnniuHide();
	if  relationindex == CHATROOMTYPE then
		local nums=ChatRoomMgr:getCurrentRoomrolenums();
		local start;
		if  nums > 17 then
			 HaoyouHuadongSliderupdate(nums);
			 nums = 17;
		else
			HaoyouHuadongSlider:Hide();
		end
		for i=1,nums do
		    local RolePlayer=ChatRoomMgr:getPlayerInfo( i + nHaoyouHuadongindex  );
		    local jbutton=getglobal("HaoYouAnNiu"..i);
		    local font=getglobal("HaoYouAnNiu"..i.."Font");
		    jbutton:Enable();
		    font:SetText(RolePlayer.m_RoleName);
		    local tex=getglobal("HaoYouAnNiu"..i.."tipsspacesign");
		    tex:Show();
		    jbutton=getglobal("HaoYouAnNiu"..i.."tips");
		    jbutton:Enable();
		    jbutton:SetClientID(MUD_ONLINE);
		end
	elseif  relationindex == FRIENDTYPE then
		local nums=RelationMgr:getFriendNum();
		local start;
		if  nums  > 17 then
			 HaoyouHuadongSliderupdate(nums);
			 nums = 17;
		else
			HaoyouHuadongSlider:Hide();
		end
		for i=1,nums do
		    local RolePlayer=RelationMgr:getFriendInfo( i + nHaoyouHuadongindex );
		    local jbutton=getglobal("HaoYouAnNiu"..i);
		    local font=getglobal("HaoYouAnNiu"..i.."Font");
		    jbutton:Enable();
		    font:SetText(RolePlayer.m_RoleName);
		    local online=RolePlayer.m_Onlinetype;
		    local tex;
		    jbutton=getglobal("HaoYouAnNiu"..i.."tips");
		    jbutton:Enable();
		    if online == CLIENT_ONLINE then
			tex=getglobal("HaoYouAnNiu"..i.."tipsgamesign");
			tex:Show();

			jbutton:SetClientID(CLIENT_ONLINE);
		   elseif online == MUD_ONLINE then
		   	tex=getglobal("HaoYouAnNiu"..i.."tipsspacesign");
			tex:Show();
			jbutton:SetClientID(MUD_ONLINE);
		  end
		end
	else
		local nums=RelationMgr:getClanMemeberNum();
		local start;
		if  nums  > 17 then
			 HaoyouHuadongSliderupdate(nums);
			 nums = 17;
		else
			HaoyouHuadongSlider:Hide();
		end
		for i=1,nums do
		    local RolePlayer=RelationMgr:getClanMemberInfo( i + nHaoyouHuadongindex );
		    local jbutton=getglobal("HaoYouAnNiu"..i);
		    local font=getglobal("HaoYouAnNiu"..i.."Font");
		    jbutton:Enable();
		    font:SetText(RolePlayer.m_RoleName);
		    local online=RolePlayer.m_Onlinetype;
		    local tex;
		    jbutton=getglobal("HaoYouAnNiu"..i.."tips");
		    jbutton:Enable();
		    if online == CLIENT_ONLINE then
			tex=getglobal("HaoYouAnNiu"..i.."tipsgamesign");
			tex:Show();
			jbutton:SetClientID(CLIENT_ONLINE);
		   elseif online == MUD_ONLINE then
		   	tex=getglobal("HaoYouAnNiu"..i.."tipsspacesign");
			tex:Show();
			jbutton:SetClientID(MUD_ONLINE);
		  end
		end
	end
	
end

function HaoYouAnNiuTemplateAllDisChecked()
	for i=1,HaoYouAnniunums do
	  local jbutton=getglobal("HaoYouAnNiu"..i);
	  jbutton:DisChecked();
	 end
end

function HaoYouAnNiuTemplateAllhide()
	HaoYouAnNiuTemplateAllDisChecked();
	for i=1,HaoYouAnniunums do
	  local jbutton=getglobal("HaoYouAnNiu"..i);
	  jbutton:Disable();
	 end
end

function HaoYouAnNiuTemplateFontAllhide()
	for i=1,HaoYouAnniunums do
	  local Font=getglobal("HaoYouAnNiu"..i.."Font");
	  Font:SetText("");
	  local tex=getglobal("HaoYouAnNiu"..i.."tipsgamesign");
	  tex:Hide();
          tex=getglobal("HaoYouAnNiu"..i.."tipsspacesign");
	  tex:Hide();
	  local jbutton=getglobal("HaoYouAnNiu"..i.."tips");
	  jbutton:SetClientID(OFFLINE);
	  jbutton:Disable();
	 end
end

function HaoYouAnNiuTemplate_tips()
	local str="";
	if this:GetClientID() == CLIENT_ONLINE then
		str="�ͻ�������";
	elseif this:GetClientID() == MUD_ONLINE then
		str="�����ռ�����"
	end
	if str ~="" then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = str, 
								frame = this:GetParent(), button = "cursor" } );
	else
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "������", 
								frame = this:GetParent(), button = "cursor" } );
	end
end

function RelationAnniuHide()
	GongGongAnNiu:DisChecked();
	ClanAnNiu:DisChecked();
	HaoYouAnNiu:DisChecked();
	HaoYouAnNiuTemplateAllhide();
end

function HaoyouHuadong_ScrollBarValueChange()
	if this:IsShown() then
		nHaoyouHuadongindex = this:GetValue() / 5;
		Relationupdate();
	end
end

function HaoyouHuadong_ScrollHide()
	nHaoyouHuadongindex = 0;
end

function HaoyouHuadongUpbtn_OnClick()
	if HaoyouHuadongSlider:IsShown() then
		local pos=HaoyouHuadongSlider:GetValue() - 5 ;
		HaoyouHuadongSlider:SetValue(pos);
	end
end

function HaoyouHuadongDownbtn_OnClick()
	if HaoyouHuadongSlider:IsShown() then
		local pos=HaoyouHuadongSlider:GetValue() + 5 ;
		HaoyouHuadongSlider:SetValue(pos);
	end
end

function Relationupdate()
	if  relationindex == CHATROOMTYPE then
		GongGongAnNiu_OnClick();
	elseif relationindex == CLANTYPE then
		ClanAnNiu_OnClick();
	else
		 HaoYouAnNiu_OnClick();
	end
	selectroleindex=0;
end

function guajiFrameOnEvent()
	if arg1 == "GE_ENTERCHARROOM" and ChatRoomMgr:getCurrentChatRoomIndex() ~= -1 then -- ������������ҵ�ʱ�������������Ҽ�����������ѡ�����˫�������ҵ����󣬹ʽ��������ҽ���
		ZonghepindaoRichText:Clear();
		HistoryFrameRichText:Clear();
		PingDaoFrameobjectName:SetText("");	
		guajiloginSelectRoomFrame:Hide();
		OffMgr:setsize(1);
		guajiFrame:Show();
		guajiFrametips();
	elseif arg1 == "GE_CHATMSG" then
		dealchatmsg();
	elseif arg1 == "GE_CHATREQ" then
		dealchatreq();
	elseif arg1 == "GE_TIPS_UPDATE"  then
		TipRicTextupdate();
	elseif arg1 == "GE_KICK_OFF" and this:IsShown() then
		this:Hide();
		guajiloginFrame:Show();
		OffMgr:setsize(0);
		AllRichtextClear();
	elseif arg1 =="GE_EXP_UPATE" then
		JingYanBtnjingyan:SetSize(MainPlayer:getOffExplength() ,8);
	elseif arg1 == "GE_RELATION_UPDATE"  then
		Relationupdate();
	elseif arg1 == "SHOW_ROOM_LIST" then
		-- ��ʾѡ�������ҽ���
		guajiFrame:Hide();
		guajiloginSelectRoomFrame:Show();
		OffMgr:setsize(0);
		WatchPlayerFrame:Hide();
		if ChatFaceFrame:IsShown() then
			ChatFaceFrame:Hide();
		end
	end
end

function FairyBtn_OnClick()
	--FairyMgr:reverseFairy();
end

function FairyBtn_tips()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "��ʾ��ر����澫��", 
								frame = this:GetParent(), button = "cursor" } );
end

function LiaoTianShiNiu_tips()
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "����������ѡ�����", 
								frame = this:GetParent(), button = "cursor" } );
end

function StartZone_tips()
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�ر������ռ��������Ԫ", 
								frame = this:GetParent(), button = "cursor" } );
end

function Privatechannelbtn_tips()
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "˫����ϵ�б��е�������Է�˽��", 
								frame = this:GetParent(), button = "cursor" } );
end

function LiaoTianshiEdit_tips()
				ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�����ctrl��Enter��������������", 
								frame = this:GetParent(), button = "cursor" } );	
end

function GuaJiInit()
	AllRichtextClear();
	ChatRoomMgr:clear();
	HistoryFrameRichText:Clear();
	PingDaoFrameobjectName:SetText("");
	nWorldShieldState	= false;
	nClanShieldState	= false;
	nPrivateShieldState	= false;
end

function ShieldFrameSurebtn_OnClick()
	if WorldShieldChkBtn:GetCheckState() ~= nWorldShieldState then
		nWorldShieldState = WorldShieldChkBtn:GetCheckState();
		MudMgr:requestChannelSet(CL_CHAT_WORLD, WorldShieldChkBtn:GetCheckState());
	end

	if GonghuiShieldChkBtn:GetCheckState() ~= nClanShieldState then
		nClanShieldState = GonghuiShieldChkBtn:GetCheckState();
		MudMgr:requestChannelSet(CL_CHAT_CLAN, GonghuiShieldChkBtn:GetCheckState());
	end

	if PrivateShieldChkBtn:GetCheckState() ~= nPrivateShieldState then
		nPrivateShieldState = PrivateShieldChkBtn:GetCheckState();
		MudMgr:requestChannelSet(CL_CHAT_PRIVATE, PrivateShieldChkBtn:GetCheckState());
		MudMgr:requestChannelSet(CL_CHAT_FRIEND, PrivateShieldChkBtn:GetCheckState());
	end
	ShieldFrame:Hide();
end

function ShieldFrame_OnShow()
	WorldShieldChkBtn:SetCheckState(nWorldShieldState);
	GonghuiShieldChkBtn:SetCheckState(nClanShieldState);
	PrivateShieldChkBtn:SetCheckState(nPrivateShieldState);
end

function DeleteChatObject(str)
	if PingDaoFrameobjectName:GetText() == str then
		PingDaoFrameobjectName:SetText("");
	end
end