local Chatroomindex = 1;
local nLastRefreshTime = 0;
nCurrentConnectindex = 0;--��ǰ�������ӵ������Ҵ��ţ�Ҳ�ǽ��������ҵ�ʱ��ǰ��������
function QiDongLITianShi_OnClick()
	if not ChatRoomMgr:getCanConnnect() then
		AddMessage("��������������"..nCurrentConnectindex.."�����Ե�!",0);
		return;
	end
	ChatRoomMgr:requestEnterRoom(Chatroomindex);
	nCurrentConnectindex = Chatroomindex;
	--[[
	if Chatroomindex ~= ChatRoomMgr:getCurrentChatRoomIndex()  then
		ChatRoomMgr:requestEnterRoom(Chatroomindex);
		nCurrentConnectindex = Chatroomindex;
	else
		nCurrentConnectindex = ChatRoomMgr:getCurrentChatRoomIndex();
		guajiloginSelectRoomFrame:Hide();
		OffMgr:setsize(1);
		guajiFrame:Show();
		guajiFrametips();
	end
	]]--
end

function guajiloginSelectRoomFrameOnLoad()
	this:RegisterEvent("GE_ENTERCHARROOM_LIST");
	this:RegisterEvent("GE_ENTERCHARROOM_LIST1");
	this:RegisterEvent("GE_ENTERCHARROOM_LIST2");
	this:RegisterEvent("GE_ENMOTION_UPADATE");
	this:RegisterEvent("GE_KICK_OFF")
	local str="liaotian1";
	local jbutton=getglobal(str);
	jbutton:SetPoint( "topleft", "XuanZeLiaoTianShi", "topleft", 10, 40 );
	jbutton:Disable();
	jbutton=getglobal("liaotian6");
	jbutton:SetPoint( "topleft", "liaotian1", "topright", 5, 0 );
	jbutton:Disable();
	for i=2,5 do
		str="liaotian"..i;
		jbutton=getglobal(str);
		local j=i-1;
		str="liaotian"..j;
		jbutton:SetPoint( "topleft", str, "bottomleft", 0, 20 );
		jbutton:Disable();
		local i1=i+5;
		str="liaotian"..i1;
		jbutton=getglobal(str);
		local j1=i1-1;
		str="liaotian"..j1;
		jbutton:SetPoint( "topleft", str, "bottomleft", 0, 20 );
		jbutton:Disable();

	end
	

end

function guajiloginSelectRoomFrameOnHide()
	guajiMessagebox:Hide();
end
function SelectRoom()
	if Chatroomindex > 0 then
		local jbutton=getglobal("liaotian"..Chatroomindex);
		jbutton:Checked();
	end
end

function updateChatroom()
	local num=ChatRoomMgr:getChatRoomNums();
	for i=1,num do
	   local jbutton=getglobal("liaotian"..i);
	   jbutton:SetText("������"..i.."["..ChatRoomMgr:getRoomPlayerNums(i).."]");
	   jbutton:Enable();
	   jbutton:DisChecked();
	end
end
function   ShowChatroom()
	CanRoleEnter = true;
	local num=ChatRoomMgr:getChatRoomNums();
	for i=1,num do
	   local jbutton=getglobal("liaotian"..i);
	   jbutton:SetText("������"..i.."["..ChatRoomMgr:getRoomPlayerNums(i).."]");
	   jbutton:Enable();
	   jbutton:DisChecked();
	end
	SelectRoom();
	selectman:Hide();
	guajiloginFrame:Hide();
	SelectActorAllDischecked();
	guajiloginSelectRoomFrame:Show();
	SetPhoto(  MainPlayer:getHair(), MainPlayer:getFace(), "TouXiangHeadHairTexture", "TouXiangHeadTexture", MainPlayer:getGender(),  MainPlayer:getHead(),
				"TouXiangHeadHairShadowTex", "TouXiangHeadBodyTex","resource\\HEADMASK.tga");
	SetPhoto(  MainPlayer:getHair(), MainPlayer:getFace(), "LiaoTianTouXiangHeadHairTexture", "LiaoTianTouXiangHeadTexture", MainPlayer:getGender(),  MainPlayer:getHead(),
				"TouXiangHeadHairShadowTex", "LiaoTianTouXiangHeadBodyTex","resource\\HEADMASK.tga");
	PlayerinfoAreaName:SetText(MainPlayer:getName());
	PlayerinfoAreaCareer:SetText("ְ ҵ :"..IdToname(MainPlayer:getCareer()));
	PlayerinfoAreaLevel:SetText("�� �� :"..MainPlayer:getLevel());
	--[[
	if ChatRoomMgr:getCurrentChatRoomIndex() ~= -1 then --���ܳ��������ӳ�
		AddMessage("���Ѿ��������������"..ChatRoomMgr:getCurrentChatRoomIndex().."�� ��ͨ����������Ұ�ť�л����������ң�",0);
	else	
		AddMessage("�����������������������Ե�һ����������������ٺ���˫������е������ң�����ָ�������ң�",0);
	end
	]]--
end



function guajiloginSelectRoomFrameOnEvent()
       if arg1 == "GE_ENTERCHARROOM_LIST" then
	MainPlayer:requestEnterGameMudWorld();
       elseif arg1 == "GE_ENTERCHARROOM_LIST1" then
        MainPlayer:requestRoomList();
	GuaJiInit();
       elseif arg1 == "GE_ENTERCHARROOM_LIST2" then
		if MainPlayer:getGameState() ~= ROOMLIST_STATE and MainPlayer:getGameState() ~= CHATROOM_STATE then --������Ϣ�ڵ���¼�֮��
			ShowChatroom();
		else
			updateChatroom();
		end
       elseif arg1 == "GE_KICK_OFF" and this:IsShown() then
	 this:Hide();
	 guajiloginFrame:Show();
	 selectman:Hide();
	 AllRichtextClear();
	elseif arg1 == "GE_ENMOTION_UPADATE" then
		xinqingEdit:SetText(MainPlayer:getEmotion());
       end
end

function guajiloginSelectRoomFrameOnShow()
	MainPlayer:setGameState(ROOMLIST_STATE);
	if ResponseAddFriendFrame:IsShown() then
		ResponseAddFriendFrame:SetPoint( "topleft", "UIClient",  "topleft", "25", "200");
	end
	if guajiMessagebox:IsShown() then
		guajiMessagebox:SetPoint("topleft","UIClient","topleft",25,200);
	end
	MainPlayer:requestRoomList();
	--OfflineWatchPlayer:sendWatchRequest(MainPlayer:getName());
end

function guajiloginSelectRoomFrameOnUpdate()
end


function GongYongAnNiuY1liaotianTemplate_OnClick()
	if this:IsChecked() then
		QiDongLITianShi_OnClick();
	else
		local jbutton=getglobal("liaotian"..Chatroomindex);
		jbutton:DisChecked();
		this:Checked();
		Chatroomindex=this:GetClientID();
	end
end

function LoginoutBtn_OnClick()
	guajiloginSelectRoomFrame:Hide();
	guajiloginFrame:Show();
	MudMgr:requestLogout();
	AllRichtextClear();
end

function xinqingEdit_OnEnterPressed()
	ClearFocus();
end

function FaBuxinqingbtn_OnClick()
	MainPlayer:requestEmotion(xinqingEdit:GetText());
	xinqingEdit:Clear();
end

function TouXiang_OnEnter()
	TouXiangHeadBodyTex:SetPoint("bottomleft","TouXiang","bottomleft",1,-5);
	TouXiangHeadTexture:SetPoint("bottomleft","TouXiang","bottomleft",1,-5);
	TouXiangHeadHairShadowTex:SetPoint("bottomleft","TouXiang","bottomleft",1,-5);
	TouXiangHeadHairTexture:SetPoint("bottomleft","TouXiang","bottomleft",1,-5);
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "˫������ѡ���ɫ", 
								frame = this:GetParent(), button = "cursor" } );
end

function TouXiang_OnLeave()
	TouXiangHeadBodyTex:SetPoint("bottomleft","TouXiang","bottomleft",4,-5);
	TouXiangHeadTexture:SetPoint("bottomleft","TouXiang","bottomleft",4,-5);
	TouXiangHeadHairShadowTex:SetPoint("bottomleft","TouXiang","bottomleft",4,-5);
	TouXiangHeadHairTexture:SetPoint("bottomleft","TouXiang","bottomleft",4,-5);
	GameTooltipOnLeave();
end

function TouXiang_OnClick()
	selectman:Show();
end