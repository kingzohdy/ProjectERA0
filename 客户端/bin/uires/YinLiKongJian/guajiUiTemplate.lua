	CLIENT_DIR_NOT_WORK = 0;
	CLIENT_DIR_EXCELLENCE = 1;
	CLIENT_DIR_NORMAL = 2;
	CLIENT_DIR_BUSY = 3;
	CLIENT_DIR_FULL = 4;

g_nMale=1;
g_nFemale=2;
pindaoindex=CHATROOM_CHANNEL;
pindaorecindex=0;
relationindex=FRIENDTYPE;
selectroleindex=1;
function System_Min()
	OffMgr:MiniWindow();
end

function System_Close()
	OffMgr:end1();
end


function AddMessage(str,style)
	MudMgr:addMessagestr(str,style);
end

function Message_Close()
	guajiMessagebox:Hide();
end

function Messagebox_OnLoad()
	this:RegisterEvent("GE_MESSAGE_UPDATE");
	guajiMessageboxFont:SetText("ϵͳ��ʾ");
	guajiMessageboxsureFont:SetText("ȷ��");
end

function Messagebox_OnEvent()
	local nindex = MudMgr:getMessagestyle();
	if  nindex == 0 then 
		guajiMessageboxText:SetText(MudMgr:getMessagestr(),255,255,190);
		guajiMessageboxFont:SetTextColor(255,255,190);
		guajiMessageboxFont:SetText("��ܰ��ʾ");
	else
		guajiMessageboxText:SetText(MudMgr:getMessagestr(),255,0,0);
		guajiMessageboxFont:SetTextColor(255,0,0);
		guajiMessageboxFont:SetText("ϵͳ��ʾ");
	end
	this:Show();
end

function FriendMessagebox_OnLoad()
	--this:RegisterEvent("GE_REQUEST_FRIEND");
	guajiFriendMessageboxFont:SetText("ϵͳ��ʾ");
	guajiFriendMessageboxsureFont:SetText("����");
	guajiFriendMessageboxDenialFont:SetText("������");
end

function FriendMessagebox_OnEvent()
	guajiFriendMessageboxText:SetText(RelationMgr:getReqName().."�����Ϊ���ѣ�",255,0,0);
	this:Show();
end

function FriendMessage_acctep()
	RelationMgr:responseAddFriend(RelationMgr:getReqName(),1,RelationMgr:getReqOnlinetype());
	guajiFriendMessageboxText:Clear();
	guajiFriendMessagebox:Hide();
end

function FriendMessage_Denial()
	RelationMgr:responseAddFriend(RelationMgr:getReqName(),0,RelationMgr:getReqOnlinetype());
	guajiFriendMessageboxText:Clear();
	guajiFriendMessagebox:Hide();
end


function adddescribeFriendMessagebox_OnLoad()
	adddescribeFriendMessageboxFont:SetText("��������Ѹ�����Ϣ");
	adddescribeFriendMessageboxsureFont:SetText("ȷ��");
end

function adddescribeFriendMessage_sure()
	local RolePlayer;
	if relationindex == FRIENDTYPE then
		AddMessage("�Է��Ѿ��������!",0);
	elseif relationindex == CHATROOMTYPE then
		RolePlayer=ChatRoomMgr:getPlayerInfo(selectroleindex);
	else
		RolePlayer=RelationMgr:getClanMemberInfo(selectroleindex);
	end
	RelationMgr:requestAddFriend(RolePlayer.m_RoleName,adddescribeFriendMessageboxEdit:GetText(),RolePlayer.m_Onlinetype);
	adddescribeFriendMessagebox:Hide();
	adddescribeFriendMessageboxEdit:Clear();
end

function adddescribeFriendMessage_close()
	adddescribeFriendMessagebox:Hide();
end


function GameTooltipOnLeave()
	GameTooltipRichText:SetText("",255,255,255);
	GameTooltip:Hide();
end

function Messagebox_OnShow()
	if MainPlayer:getGameState() == CHATROOM_STATE then 
		this:SetPoint("topleft","UIClient","topleft",237,140)
	else
		this:SetPoint("topleft","UIClient","topleft",25,200)
	end
	MudMgr:delMessage();
end

function RichText_OnClick()
	if type(arg1) == "number" then return end --����Ǳ��������������
	if arg2 ~= "" then
		if tonumber(arg2) == nil then return end
		--����������
		local linkId = tonumber(arg2)%10;
		--������Count
		local linkCountId = (tonumber(arg2) - linkId)/10;

		-- ��Ʒ������
		if linkId == ITEM_LINK then
			--��<>����Ʒ����ת��Ϊ����<>
			local t_hideIcons = { "LinkItemTipsFrame", "ArmItemTipsFrame", };
			util.HideIcons( t_hideIcons );
			local itemLink = ChatManager:getItemLink(linkCountId);
			if itemLink.m_nItemId > 0 then
				--��ͨ���ߵ�tips
				ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemLink.m_nItemId) );
				return;

			end
		elseif linkId == ARM_LINK then
			local armLink = ChatManager:getArmLink(linkCountId);
			if getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM then
				--װ����tips
				local item = ChatManager:getItem(armLink.m_arm);
				if item ~= nil then
					ShowItemTips(LUA_ITEMTIPS_TYPE_LINK,LUA_ITEMTIPS_ITEM,item);
					return;
				end
			end
		elseif linkId == POSTION_LINK or linkId == MISSION_LINK or linkId == SERVER_LINK or linkId == SECONDARY_SKILL_LINK or linkId == PET_LINK then
			return;
		end

	end
	if arg1~="RightButton" and arg3 == "RightButton" then
		FoucsName = arg1;
		local nX		= GetCursorPosX();
		local nY		= GetCursorPosY();
		RoleOnClickDialog:SetPoint( "topright", "UICLIENT", "topleft", nX, nY );
		RoleOnClickDialog:Show();
	end
	if arg1~="LeftButton" and arg3 == "LeftButton" then
		Siliaopindao_OnClick();
		PingDaoFrameobjectName:SetText(arg1);
		SetFocusFrame("LiaoTianshiEdit");
	end
end

function OnAccelerator(code, press)
end

--��ֲ�ù۲�����
-- 5030021
CAREER_NON					= 0; 	-- /* 8λ�洢,��4λ��ʾתְְҵ,��4λ��ʾ����ְҵ */
CAREER_WARRIOR				= 1; 	-- /* תְǰսʿ */
CAREER_MAGICOR				= 2; 	-- /* תְǰ��ʦ */
CAREER_GUNER				= 3; 	-- /* תְǰǹ�� */
CAREER_CHURCH				= 4; 	-- /* תְǰ��ʦ */

CAREER_WARRIOR_SINGLEHAND	= 17; 	-- /* ����֧���ܵ���������Ϊ���ֵ� */
CAREER_WARRIOR_DOUBLEHAND	= 33; 	-- /* ����֧���ܵ���������Ϊ���ֽ� */

CAREER_MAGICOR_SINGLEHAND	= 18; 	-- /* ����֧���ܵ���������Ϊ����ħ�� */
CAREER_MAGICOR_DOUBLEHAND	= 34; 	-- /* ���֧���ܵ���������Ϊ˫��ħ�� */

CAREER_GUNER_SINGLEHAND		= 19; 	-- /* ��ǹ�ּ���֧����������Ϊ����ǹ */
CAREER_GUNER_DOUBLEHAND		= 35; 	-- /* ��ǹ�ּ���֧����������Ϊ��ǹ */

CAREER_CHURCH_SINGLEHAND	= 20; 	-- /* ʥ��֧���ܵ���������Ϊ����ʥ�� */
CAREER_CHURCH_DOUBLEHAND	= 36; 	-- /* �׷�֧���ܵ���������Ϊ˫��ʥ�� */
ZHUAN_ZHI_RACE_LIMIT		= 0x10;

