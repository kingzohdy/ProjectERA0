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
	guajiMessageboxFont:SetText("系统提示");
	guajiMessageboxsureFont:SetText("确定");
end

function Messagebox_OnEvent()
	local nindex = MudMgr:getMessagestyle();
	if  nindex == 0 then 
		guajiMessageboxText:SetText(MudMgr:getMessagestr(),255,255,190);
		guajiMessageboxFont:SetTextColor(255,255,190);
		guajiMessageboxFont:SetText("温馨提示");
	else
		guajiMessageboxText:SetText(MudMgr:getMessagestr(),255,0,0);
		guajiMessageboxFont:SetTextColor(255,0,0);
		guajiMessageboxFont:SetText("系统提示");
	end
	this:Show();
end

function FriendMessagebox_OnLoad()
	--this:RegisterEvent("GE_REQUEST_FRIEND");
	guajiFriendMessageboxFont:SetText("系统提示");
	guajiFriendMessageboxsureFont:SetText("接受");
	guajiFriendMessageboxDenialFont:SetText("不接受");
end

function FriendMessagebox_OnEvent()
	guajiFriendMessageboxText:SetText(RelationMgr:getReqName().."想加你为好友！",255,0,0);
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
	adddescribeFriendMessageboxFont:SetText("请输入好友附加信息");
	adddescribeFriendMessageboxsureFont:SetText("确定");
end

function adddescribeFriendMessage_sure()
	local RolePlayer;
	if relationindex == FRIENDTYPE then
		AddMessage("对方已经是你好友!",0);
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
	if type(arg1) == "number" then return end --如果是表情符，不做处理
	if arg2 ~= "" then
		if tonumber(arg2) == nil then return end
		--超链接类型
		local linkId = tonumber(arg2)%10;
		--超链接Count
		local linkCountId = (tonumber(arg2) - linkId)/10;

		-- 物品超链接
		if linkId == ITEM_LINK then
			--带<>号物品名称转找为不带<>
			local t_hideIcons = { "LinkItemTipsFrame", "ArmItemTipsFrame", };
			util.HideIcons( t_hideIcons );
			local itemLink = ChatManager:getItemLink(linkCountId);
			if itemLink.m_nItemId > 0 then
				--普通道具的tips
				ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemLink.m_nItemId) );
				return;

			end
		elseif linkId == ARM_LINK then
			local armLink = ChatManager:getArmLink(linkCountId);
			if getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM then
				--装备的tips
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

--移植用观察他人
-- 5030021
CAREER_NON					= 0; 	-- /* 8位存储,高4位表示转职职业,低4位表示现有职业 */
CAREER_WARRIOR				= 1; 	-- /* 转职前战士 */
CAREER_MAGICOR				= 2; 	-- /* 转职前法师 */
CAREER_GUNER				= 3; 	-- /* 转职前枪手 */
CAREER_CHURCH				= 4; 	-- /* 转职前牧师 */

CAREER_WARRIOR_SINGLEHAND	= 17; 	-- /* 刀分支技能的武器需求为单手刀 */
CAREER_WARRIOR_DOUBLEHAND	= 33; 	-- /* 剑分支技能的武器需求为对手剑 */

CAREER_MAGICOR_SINGLEHAND	= 18; 	-- /* 冰分支技能的武器需求为单手魔杖 */
CAREER_MAGICOR_DOUBLEHAND	= 34; 	-- /* 火分支技能的武器需求为双手魔杖 */

CAREER_GUNER_SINGLEHAND		= 19; 	-- /* 手枪分技能支的武器需求为单手枪 */
CAREER_GUNER_DOUBLEHAND		= 35; 	-- /* 机枪分技能支的武器需求为机枪 */

CAREER_CHURCH_SINGLEHAND	= 20; 	-- /* 圣分支技能的武器需求为单手圣杖 */
CAREER_CHURCH_DOUBLEHAND	= 36; 	-- /* 雷分支技能的武器需求为双手圣杖 */
ZHUAN_ZHI_RACE_LIMIT		= 0x10;

