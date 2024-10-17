
--##	�����ʼ�����(������)		##--

local MAXMAILHEAD			= 8;
local MAXSENDITEM			= 8;
local MAXSHOWCONTACTNUM		= 20;
local MAILSENDOUTTIME		= 10;
local nCurMailPage			= 0;
local nCurMailPageRow		= 1;
local nCurMailPageId		= 0;
local nCheckMailPageId		= 0;
local nPageTotal			= 0;
local nTotalHeadNum			= 0;
local nCurMoney				= 0;
local t_Contact				= {};
local n_SendSuccess			= 1;
local n_MailSendTime		= 0;
local n_TimeFlag			= 0;
SEND_MAIL_COST				= 10;
local MAIL_FLAG_READ		= 2;
local MAIL_FLAG_SYSTEM		= 1;
local MAIL_FLAG_NONORM		= 8;

function InitMailLocalVariable()
	nCurMailPage		= 0;
	nCurMailPageRow		= 1;
	n_SendSuccess		= 1;
	n_TimeFlag			= 0;
	n_MailSendTime		= 0;
	t_Contact		= {};
end

--@Desc:��������
function CalculatedPostage()
	--[[
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end
	local money = getPlayerMoney( mainplayer );
	local nPostCost = 0;
	for i = 1, MAXMAILITEM do
		local SendItemBtn	= getglobal( "MailSendBoxSendItem"..i);
		if SendItemBtn:GetClientUserData(2) ~= 0 then
			local ItemDef = getItemDef( SendItemBtn:GetClientUserData(2) );
			local nItemNum = SendItemBtn:GetClientUserData(3);
			nPostCost = (ItemDef.BuyPrice) * nItemNum;
		end
	end

	if MailSendMoney_EditBox:GetText() ~= "" then
		if tonumber( MailSendMoney_EditBox:GetText()) + tonumber( MailSendBoxPostageCountFont:GetText() ) > money then
			if money == 0 then
				MailSendMoney_EditBox:SetText("");
			else
				MailSendMoney_EditBox:SetText( math.ceil( money * 100 / 101 ) - 1 );
			end
			ShowMidTips( "�����ʼ�����"..MONEY_TYPE_MONEY_NAME.."����������ӵ��"..MONEY_TYPE_MONEY_NAME.."��,��ǿ����Ϊ�ܷ��͵����"..MONEY_TYPE_MONEY_NAME.."��" );
		end
		if money ~= 0 then
			nPostCost = nPostCost + tonumber(MailSendMoney_EditBox:GetText());
		end
	end

	if math.floor( nPostCost/100 ) == 0 then
		nPostCost = 1;
	else
		nPostCost = math.floor( nPostCost / 100 );
	end
	MailSendItemBoxPostageCountFont:SetText(tostring(SEND_MAIL_COST));
	--]]
	
end

function MailInfoIcon_OnEnter()
	local button = getglobal( this:GetParent() );
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = button:GetParent(),
						button = this:GetName(), horizonal = "left" } );
end

function MailInfoIcon_OnLeave()
	GameTooltip:Hide();
end
function MailReadFrame_FraudWarning_OnEnter()
	SetGameTooltips(this:GetName(), "���Ԫ�ٷ���Զ����ͨ���ʼ�����������������н���Ϣ���Լ��������ȡ�˺�������Ϣ����Ϊ��" );
end

function MailHeadFrame_OnShow()	
	--�����ս���У�����ʾ
	if LoginMgr:isInBattleWorld() then
		ShowMidTips("ս���в��ܽ��иò���");
		this:Hide();
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isOutSide() then
		ShowMidTips("���������ò���");
		this:Hide();
		return;
	end
	-- ���ô�������	
	nCurMailPageId = 0;
	if SysMailsFullFont:GetText() ~= "" then
		MailHeadFrameMailNormalFullBtn:Hide();
		MailHeadFrameMailSysFullBtn:Show();
	end 
	if NormalMailsFullFont:GetText() ~= "" then
		if not MailHeadFrameMailSysFullBtn:IsShown() then
			MailHeadFrameMailNormalFullBtn:Show();
		else
			MailHeadFrameMailNormalFullBtn:Hide();
		end
	end
	local mailheadbtn = getglobal( this:GetName().."MailHeadBtn" );
	local mailsendbtn = getglobal( this:GetName().."MailSendBtn" );
	mailheadbtn:Checked();
	mailsendbtn:DisChecked();
	mailheadbtn:Show();
	mailsendbtn:Show();
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	checkBtn:setCheckState(true);
	UpdataMailPage( nCurMailPageId );
	--С��ͼ�ʼ���Чֹͣ
	ResetMiniMapData();
end

function MailHeadFrame_OnHide()
	if MailReadFrame:IsShown() then
		MailReadFrame:Hide();
	end
	MailOperationTipsFrame:Hide();
	if not MailSendItemFrame:IsShown() then   --Elmer
		ClearSendItemInfo();
		MailItemSender_EditBox:Clear();
	end
end

function MailHeadFrame_OnLoad()
	this:RegisterEvent("GE_USER_MAIL_FULL");
	this:RegisterEvent("GE_SYS_MAIL_FULL");
	this:RegisterEvent("GE_CHG_MAIL_SENDER_NAME");
	this:RegisterEvent("GE_MAIL_NEWMAIL");
	for i=1,MAXMAILHEAD do
		local btn = getglobal("MailInfo"..i);
		btn:SetOverlayRegion( 0, 0, 374, 49 )
	end
end

function MailHeadFrame_OnEvent()
	if arg1 == "GE_USER_MAIL_FULL" then
		ShowMidTips( "����һ�����ʼ�����ͨ�ʼ����������뼰ʱ����" );
		SetSystemMessage( "����һ�����ʼ�����ͨ�ʼ����������뼰ʱ����" );
		NormalMailsFullFont:SetText("������ͨ�����������뼰\nʱ�������ͷ�һ���ռ�");
		if MailHeadFrameMailSysFullBtn:IsShown() then
			MailHeadFrameMailNormalFullBtn:Hide();
		else
			MailHeadFrameMailNormalFullBtn:Show();
		end
	end

	if arg1 == "GE_SYS_MAIL_FULL" then
		ShowMidTips( "����һ�����ʼ����ʼ����������뼰ʱ����" );
		SetSystemMessage( "����һ�����ʼ����ʼ����������뼰ʱ����" );
		SysMailsFullFont:SetText("���������������뼰ʱ��\n�����ͷ�һ���ռ�")
		if MailHeadFrameMailNormalFullBtn:IsShown() then
			MailHeadFrameMailNormalFullBtn:Hide();
		end
		MailHeadFrameMailSysFullBtn:Show();
	end

	if arg1 == "GE_CHG_MAIL_SENDER_NAME" then
		if this:IsShown() then
			UpdataMailPage( nCurMailPageId );
		end
	end
	if arg1 == "GE_MAIL_NEWMAIL" then
		if MailReadFrame:IsShown() then
			Mail_ReadBtn_OnClick();
		end
	end
end

--@Desc:ˢ��һҳ�ʼ�ͷ
function UpdataMailPage( nPage )
	if MailHeadFrame:IsShown() then
		if nPage < 0 then
			nPage = 0;
		end
		nCurMailPageId = nPage;
		nCurMailPage   = nPage + 1;
		local PageMail = GameMailManager:getOnePageMail( nPage );
		if PageMail == nil then
			return;
		end

		--����״̬:������ҳ��������nPageTotal
		nPageTotal	= math.ceil( PageMail.m_nTotalHeadNum/MAXMAILHEAD );
		nTotalHeadNum	= PageMail.m_nMailNum;
		if nPageTotal == 0 then
			nPageTotal = 1;
		end
		if nCurMailPageId + 1 > nPageTotal then
			UpdataMailPage( nPageTotal - 1 );
			return;
		end
		MailListPageFont:SetText( "��"..(nCurMailPageId+1).."/".. nPageTotal.."ҳ" );

		--local highlightbtn = getglobal( "MailInfo"..nCurMailPageRow.."HighlightBtn");
		local highlightbtn = getglobal( "MailInfo"..nCurMailPageRow );
		if highlightbtn ~= nil then
			highlightbtn:Checked();
		end
		ChangeMailHeadBackTexture();
		for i = 1, PageMail.m_nMailNum do
			BondingMailHead( PageMail.m_MainData[i - 1].m_MailHead, i, COLOR_WHITE);
		end

		for i = PageMail.m_nMailNum + 1, MAXMAILHEAD do
			local btn		= getglobal( "MailInfo"..i );
			local IconBtn	= getglobal( "MailInfo"..i.."IconBtn" );
			local icontex	= getglobal( "MailInfo"..i.."IconBtnIcon" );
			local namefont	= getglobal( "MailInfo"..i.."SendName" );
			local titlefont	= getglobal( "MailInfo"..i.."MailTitle" );
			local datafont	= getglobal( "MailInfo"..i.."UsefulDate" );
			local TipFont	= getglobal( "MailInfo"..i.."TipFont");
			local checkBtn 	= getglobal( "MailInfo"..i.."CheckBtn" );
			btn:SetClientUserData(0,0);
			icontex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			IconBtn:Disable();
			namefont:SetText("");
			titlefont:SetText("");
			datafont:SetText("");
			TipFont:SetText("");
			checkBtn:Hide();
			btn:Disable();
		end
		if nTotalHeadNum == 0 then
			DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
		end
		local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
		local state = checkBtn:GetCheckState() ;
		if state then 
			SetMailHeadCheckState( state )
		end
	end
end

function ChangeMailHeadBackTexture()
	for j=1,MAXMAILHEAD do
		local tex = getglobal( "MailInfo"..j.."BkgTexture" )
		if j%2 == 0 then				
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" )
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" )
		end
	end
end 

function Mail_LeftPageBtn_OnClick()
	nCurMailPageId = nCurMailPageId - 1;
	if nCurMailPageId == -1 then
		nCurMailPageId = nCurMailPageId + 1;
		return;
	end
	-- �������������һҳ�ʼ�����
	UpdataMailPage( nCurMailPageId );
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	local state = checkBtn:GetCheckState() ;
	if not state then 
		SetMailHeadCheckState( false );
	end
	-- ���ҵ�ǰ���е������ø��и���
	DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
	if nCheckMailPageId == nCurMailPageId then
		--local button = getglobal("MailInfo"..nCurMailPageRow.."HighlightBtn");
		local button = getglobal("MailInfo"..nCurMailPageRow );
		if button ~= nil then
			button:Checked();
		end
	end
	MailReadFrame:Hide();
end

function Mail_RightPageBtn_OnClick()
	nCurMailPageId = nCurMailPageId + 1;
	if nCurMailPage >= nPageTotal then
		nCurMailPageId = nCurMailPageId - 1;
		return;
	end
	-- �������������һҳ�ʼ�����
	UpdataMailPage( nCurMailPageId );
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	local state = checkBtn:GetCheckState() ;
	if not state then 
		SetMailHeadCheckState( false );
	end
	-- ���ҵ�ǰ���е������ø��и���
	DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
	if nCheckMailPageId == nCurMailPageId then
		--local button = getglobal("MailInfo"..nCurMailPageRow.."HighlightBtn");
		local button = getglobal("MailInfo"..nCurMailPageRow )
		if button ~= nil then
			button:Checked();
		end
	end
	MailReadFrame:Hide();
end

function SetCanReadDay( szFrame, nValue, t_Color )
	local font	= getglobal(szFrame);
	local ServerTime = GameClanManager:getServerTime();
	local recieveday = math.floor(( ServerTime - nValue )/86400);
	if 60 - ( ServerTime - nValue )/86400 <= 1 then
		if 1440 - ( ServerTime - nValue ) / 3600 <= 1 then
			if 86400 - ( ServerTime - nValue ) / 60 <= 1 then
				font:SetText("�ʼ���Чʱ��: "..math.floor(( 5184000 - ( ServerTime - nValue ) )).."��");
			else
				font:SetText("�ʼ���Чʱ��: "..math.floor(( 86400 - ( ServerTime - nValue ) / 60 )).."����");
			end
		else
			font:SetText("�ʼ���Чʱ�䣺"..math.floor(( 1440 - ( ServerTime - nValue ) / 3600 ) ).."Сʱ" );
		end
	else
		font:SetText( "�ʼ���Чʱ�䣺"..math.floor( 60 - ( ServerTime - nValue )/86400 ).."��" );
	end
end

--@Desc:���а��ʼ�ͷ
function BondingMailHead( MailHead, nClientId, COLOR_DEFINE )
	local nameColor	 = bitAnd(MailHead.m_nFlags , MAIL_FLAG_SYSTEM) and COLOR_GREEN or COLOR_DEFINE;
	local t_MailHead = {
							{"MailInfo"..nClientId,					nClientId,				GetColor()[COLOR_DEFINE], SetButton},
							{"MailInfo"..nClientId.."IconBtnIcon",	MailHead.m_nFlags,		GetColor()[COLOR_DEFINE], SetMailState},
							{"MailInfo"..nClientId.."SendName",		MailHead.m_szSend,		GetColor()[nameColor], SetFontText}, -- ������
							{"MailInfo"..nClientId.."MailTitle",	MailHead.m_szTitle,		GetColor()[COLOR_DEFINE], SetFontText}, -- �ʼ�����
							{"MailInfo"..nClientId.."UsefulDate",	MailHead.m_nTime,		GetColor()[COLOR_DEFINE], SetCanReadDay}, -- ��Ч��
			 }
	local btn = getglobal( t_MailHead[1][1]);
	if btn == nil then
		return;
	end
	btn:Enable();
	
	local iconBtn = getglobal( "MailInfo"..nClientId.."IconBtn" );
	if iconBtn == nil then	return;	end
	iconBtn:Enable();
	
	t_MailHead[1][4](t_MailHead[1][1],t_MailHead[1][2],t_MailHead[1][3]);
	t_MailHead[2][4](t_MailHead[2][1],t_MailHead[2][2],t_MailHead[2][3]);
	t_MailHead[5][4](t_MailHead[5][1],t_MailHead[5][2],t_MailHead[5][3]);
	t_MailHead[3][4](t_MailHead[3][1],t_MailHead[3][2],t_MailHead[3][3]);
	local nValue = MailHead.m_nFlags;
	if not bitAnd( nValue,MAIL_FLAG_SYSTEM ) and bitAnd( nValue,MAIL_FLAG_NONORM ) then
		SetMailHeadIcon( nClientId,nValue );
	end
	
	local tipfont 	= getglobal("MailInfo"..nClientId.."TipFont");
	local sender	= getglobal(t_MailHead[3][1]);
	local useDate	= getglobal(t_MailHead[5][1]);
	if not bitAnd( nValue,MAIL_FLAG_SYSTEM ) then
		tipfont:SetText("��������ҵ��ʼ���");
		useDate:SetPoint("topleft","MailInfo"..nClientId,"topleft",222,22);
	else
		tipfont:SetText("");
		useDate:SetPoint("left","MailInfo"..nClientId,"left",222,0);
	end
	
	local Title = getglobal( t_MailHead[4][1] );
	if Title ~= nil then
		Title:SetText( t_MailHead[4][2] );
	end
	local checkBtn	= getglobal( "MailInfo"..nClientId.."CheckBtn" );
	checkBtn:Show();
	-- ���������ϵ��
	--AddRecentlyContact( MailHead.m_szSend );
end

function SetMailHeadIcon(nClientId,flagValue)
	local texture = getglobal("MailInfo"..nClientId.."IconBtnIcon");
	if bitAnd( flagValue,MAIL_FLAG_READ ) then
		local IconPath = GetItemEquipIconPath().."5500006.tga";
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( IconPath );
	else
		local IconPath = GetItemEquipIconPath().."5500005.tga";
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( IconPath );
	end
end
--@Desc:���������ϵ��
function AddRecentlyContact( szRoleName )
	for i=1,table.getn( t_Contact ) do
		if szRoleName == t_Contact[i] then
			table.remove( t_Contact, i );			
		end
	end
	table.insert( t_Contact, szRoleName );
end

function MailInfo_OnClick()
	if nTotalHeadNum == 0 then
		return;
	end
	nCurMailPageRow		= this:GetClientUserData(0);
	nCheckMailPageId	= nCurMailPageId;
	DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
	this:Checked();
	if MailReadFrame:IsShown() then
		Mail_ReadBtn_OnClick();
	end
	if arg1 == "RightButton" then
		MailOperationTipsFrame:Show();
		MailOperationTipsFrame:SetPoint( "topright",this:GetName(),"bottomright",-2,2 )
	end
end

function MailInfo_OnDoubleClick()
	MailInfo_OnClick();
	if MailReadFrame:IsShown() then
		MailReadFrame:Hide();
		return;
	end
	Mail_ReadBtn_OnClick();
end

function Mail_OpenAllBtn_OnClick()
	GameMailManager:mail_Get_All();
	UpdataMailPage( nCurMailPageId );
	if MailReadFrame:IsShown() then
		MailReadFrame:Hide();
	end
end

-- �ظ��ʼ�
function MailHead_ReplyBtn_OnClick()
	if nCurMailPageRow == 0 or nTotalHeadNum == 0 then
		return;
	end

	MailSendBtn_OnClick();
	local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
	MailItemSender_EditBox:SetText( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_szSend );
end

-- ��ȡ�ʼ�
function Mail_ReadBtn_OnClick()
	if nCurMailPageRow <= 0 or nTotalHeadNum == 0 then
		return;
	end
	
	local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
	MailReadFrame:SetClientUserData( 0, PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nHeight );
	MailReadFrame:SetClientUserData( 1, PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nLow );
	MailReadFrame:SetClientUserData( 2, nCurMailPageRow );
	MailReadFrame:SetClientUserData( 3, nCurMailPageId );
	MailReadFrame:SetClientString( "READ_MAIL" );
	GameMailManager:mail_Read( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nHeight, PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nLow );
	UpdataMailPage( nCurMailPageId );
	if  bitAnd( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_nFlags,MAIL_FLAG_GOLD_TOLUA ) then
		MailMoneyButton1_EditBackBtn:SetText("���");
		MailMoneyIcon1_EditBackBtn:ChangeNormalTexture( 99, 494, 17, 18 );
		MailMoneyIcon1_EditBackBtn:SetClientUserData( 0,1 );

	else
		MailMoneyButton1_EditBackBtn:SetText("����");
		MailMoneyIcon1_EditBackBtn:ChangeNormalTexture( 117,494,17,18 );
		MailMoneyIcon1_EditBackBtn:SetClientUserData(0,3);
	end
	local logoBtn =	getglobal("MailReadFrame_WanYou");
	local warnBtn = getglobal("MailReadFrame_FraudWarning");
	if bitAnd( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_nFlags,MAIL_FLAG_SYSTEM ) then  --Elmer
		MailGetBoxSenderFont:SetTextColor(66,168,61);	
		warnBtn:Hide();
		logoBtn:Show();
	else
		MailGetBoxSenderFont:SetTextColor(255,  255, 255);
		warnBtn:Show();
		logoBtn:Hide();
	end
	MailReadFrame:Show();
	if MailOperationTipsFrame:IsShown() then
		MailOperationTipsFrame:Hide();
	end
	if MessageBoxFrame:IsShown() and  MessageBoxFrame:GetClientString() == "����ת������" then 
		MessageBoxFrame:Hide();
	end
end

-- ɾ���ʼ�
function Mail_DeleteBtn_OnClick()
	if nCurMailPageRow == 0 or nTotalHeadNum == 0 then
		return;
	end
	MessageBox("ɾ���ʼ�", "��ȷ��Ҫɾ�����ʼ���");
	--DeleteCheckMail();
	if MailOperationTipsFrame:IsShown() then
		MailOperationTipsFrame:Hide();
	end
end

function DeleteCheckMail()
	local nCurrentMailPage = nCurMailPageId;
	local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
	if MailReadFrame:IsShown() then
		if MailReadFrame:GetClientString() == "READ_MAIL" then
			GameMailManager:mail_Delete( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nHeight, PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nLow );
			if nCurMailPageRow == MailReadFrame:GetClientUserData( 2 ) then
				ReSetMailContent();
				MailReadFrame:Hide();
			end
			if nCurMailPageRow < MailReadFrame:GetClientUserData( 2 ) then
				MailReadFrame:SetClientUserData( 2, MailReadFrame:GetClientUserData( 2 ) - 1 );
			end
		elseif MailReadFrame:GetClientString() == "DELETE_MAIL" then
			GameMailManager:mail_Delete( PageMail.m_MainData[MailReadFrame:GetClientUserData( 2 ) - 1].m_MailHead.m_WID.nHeight, PageMail.m_MainData[MailReadFrame:GetClientUserData( 2 ) - 1].m_MailHead.m_WID.nLow );
			MailReadFrame:Hide();
			ReSetMailContent();
		end
	else
		GameMailManager:mail_Delete( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nHeight, PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_WID.nLow );
	end
	UpdataMailPage( nCurMailPageId );
	DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
	nCurMailPageRow = 0;
end

--##������ ��ǩ������Ӧ
function MailSendBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	MailSendItemTitle_EditBox:SetText("����"..mainplayer:getName().."���ʼ�"); --Elmer
	if MailSendItemFrame:IsShown() then
		return;
	end
	MailSendItemFrame:SetPoint("topleft", "$parent", "topleft",MailHeadFrame:GetRealLeft(), MailHeadFrame:GetRealTop());
	MailSendItemFrame:Show();
	MailHeadFrame:Hide();
	--[[
	if MailReadFrame:IsShown() then
		MailReadFrame:Hide();
	end
	]]--
end

--##�ʼ�ͷ ��ǩ������Ӧ
function MailHeadBtn_OnClick()
	if MailHeadFrame:IsShown() then
		return;
	end
	MailHeadFrame:SetPoint("topleft", "$parent", "topleft",MailSendItemFrame:GetRealLeft(), MailSendItemFrame:GetRealTop());
	MailHeadFrame:Show();
	MailSendItemFrame:Hide();
end

function MailSendBox_ContactBtn_OnClick() 
	ContactListTipsFrame:SetClientUserData(0,CONTACT_TYPE_MAIL);
	ContactListTipsFrame:Show();
end

function MailSendItemBox_ContactBtn_OnClick() 
	ContactListTipsFrame:SetClientUserData(0,CONTACT_TYPE_SENDITEM_MAIL);
	ContactListTipsFrame:Show();
end

function ContactListTipsFrame_OnShow()
	ReSetContactListTipsFramePoint();
	DisCheckAllHighLightBtn( "ContactButton", MAXSHOWCONTACTNUM );
	local nContactNum	= 0;
	local nBtnIndex		= 0;  
	local t_TmpContact	= {};
	local szRefBtn		= "";
	if ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MAIL then
		t_TmpContact = t_Contact;
		szRefBtn = "MailSendBox_ContactBtn";
	elseif ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_SENDITEM_MAIL then
		t_TmpContact = t_Contact;
		szRefBtn = "MailItemSendBox_ContactBtn";
	elseif ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MARKET then
		t_TmpContact = GetMarketContact();
		szRefBtn = "MarketContributeConfirmFrameSelectFriendBtn";
	end
	if table.getn( t_TmpContact ) >= MAXSHOWCONTACTNUM then
		for i = table.getn( t_Contact ) - MAXSHOWCONTACTNUM + 1, table.getn( t_TmpContact ) do
			local namebtn	= getglobal( "ContactButton"..( MAXSHOWCONTACTNUM - nBtnIndex ) );
			local namefont	= getglobal( "ContactButton"..( MAXSHOWCONTACTNUM - nBtnIndex ).."Name" );
			namebtn:Show();
			namefont:SetText( t_TmpContact[i] );
			nContactNum = nContactNum + 1;
			nBtnIndex = nBtnIndex + 1;
		end
	else
		for i = 1, table.getn( t_TmpContact ) do
			local namebtn	= getglobal( "ContactButton"..( table.getn( t_TmpContact ) - nBtnIndex ) );
			local namefont	= getglobal( "ContactButton"..( table.getn( t_TmpContact ) - nBtnIndex ).."Name" );
			namebtn:Show();
			namefont:SetText( t_TmpContact[i] );
			nContactNum = nContactNum + 1;
			nBtnIndex = nBtnIndex + 1;
		end
	end

	if nContactNum == 0 then
		ContactListTipsFrame:Hide();
		return;
	end

	for i = nContactNum + 1, 20 do
		local namebtn = getglobal( "ContactButton"..i );
		namebtn:Hide();
	end

	SetFocusFrame( "ContactListTipsFrame" );
	ContactListTipsFrame:SetSize(150,nContactNum*15+50);
end

function ContactListTipsFrame_OnUpdate()
	ReSetContactListTipsFramePoint();
end

function ReSetContactListTipsFramePoint()
	local szRefBtn = "";
   	if ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MAIL then
		szRefBtn = "MailSendBox_ContactBtn";
   	elseif ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_SENDITEM_MAIL then
		szRefBtn = "MailItemSendBox_ContactBtn";
	elseif ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MARKET then
		szRefBtn = "MarketContributeConfirmFrameSelectFriendBtn";
	end

	if szRefBtn ~= "" then
		local btn = getglobal(szRefBtn);		
		if  ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MARKET then
			ContactListTipsFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT", btn:GetRealLeft()+btn:GetRealWidth()+30, btn:GetRealTop() );
		else
			ContactListTipsFrame:SetPoint("TOPLEFT", "$parent", "TOPLEFT", btn:GetRealLeft()+btn:GetRealWidth(), btn:GetRealTop() );
		end
	end
end

function ContactButton_OnClick()
	local namefont	= getglobal( this:GetParent().."Name" );
	if ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MAIL then
		MailItemSender_EditBox:SetText( namefont:GetText() );
		SetFocusFrame( "MailSendTitle_EditBox" );
	elseif ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_SENDITEM_MAIL then
		MailItemSender_EditBox:SetText( namefont:GetText() );
		SetFocusFrame( "MailItemSender_EditBox" );
	elseif ContactListTipsFrame:GetClientUserData(0) == CONTACT_TYPE_MARKET then
		MarketContributeConfirmFrameNameEdit:SetText( namefont:GetText() ); 
		SetFocusFrame( "MarketContributeConfirmFrameNameEdit" );
	end	
	ContactListTipsFrame:Hide();
end

function ContactListTipsFrame_OnHide()
	--ContactListTipsFrame:SetClientUserData(0,0);
end

--##	�鿴�ʼ�����(�ռ���)		##

function MailReadFrame_OnLoad()
	this:RegisterEvent("GE_MAIL_CONTENT");
	this:RegisterEvent("GE_CHG_MAIL_SENDER_NAME");

	for i = 1, MAXSENDITEM do
		local SendItemBackTex = getglobal( "MailGetBoxSendItem"..i.."BackTexture" )
		SendItemBackTex:Show();
	end
end

function MailReadFrame_OnEvent()
	if ( arg1 == "GE_MAIL_CONTENT") then
		ReSetMailContent();
		local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
		BondingMailContent( PageMail.m_MainData[nCurMailPageRow-1], COLOR_WHITE);
		UpdataMailPage( nCurMailPageId );
	end
	
	if ( arg1 == "GE_CHG_MAIL_SENDER_NAME") then
		if this:IsShown() then
			ReSetMailContent();
			local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
			BondingMailContent( PageMail.m_MainData[nCurMailPageRow-1], COLOR_WHITE);
		end
	end
end

function MailReadFrame_OnShow()
	ReSetMailContent();	
	--MailHeadFrame:SetPoint("center", "$parent", "center", -180, 0 );
	--MailReadFrame:SetPoint("center", "$parent", "center", 216, 0 );
end

function SetItemButton( szFrame , nValue, t_Color )
	local button = getglobal(szFrame);
	button:Show();
	button:SetClientUserData( 2, nValue );
end

function ReSetMailContent()
	MailGetBoxSenderFont:SetText("");
	MailGetBoxTitleFont:SetText("");
	MailContent_Rich:SetText("",255,255,255);
	MailGetBoxMoneysCountFont:SetText("");
	MailReceiveMoneyBtn:Hide();
	for i=1, MAXMAILITEM do
		local petButton = getglobal( "MailGetBoxSendItem"..i );
		local icontex   = getglobal( "MailGetBoxSendItem"..i.."IconTexture");
		local countfont   = getglobal( "MailGetBoxSendItem"..i.."CountFont");
		petButton:DisChecked()
		petButton:SetClientUserData( 2, 0 );
		icontex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		countfont:SetText("");
	end
end

--@Desc:���ʼ���������
function BondingMailContent( MainData, COLOR_DEFINE )
	local t_MailHead = {
							{"MailGetBoxSenderFont",		MainData.m_MailHead.m_szSend,	GetColor()[COLOR_DEFINE], SetFontText},-- ������
							{"MailGetBoxTitleFont",			MainData.m_MailHead.m_szTitle,	GetColor()[COLOR_DEFINE], SetFontText}, -- �ʼ�����
							{"MailContent_Rich",			MainData.m_MailDetail.m_szText,	GetColor()[COLOR_DEFINE], SetRichText}, -- �ʼ�����
							{"MailGetBoxMoneysCountFont",	MainData.m_MailDetail.m_uGold,	GetColor()[COLOR_DEFINE], SetFontText}, -- �յ�������
							{"MailGetBoxSenderTipFont",		"��������ҵ��ʼ���",		GetColor()[COLOR_YELLOW], SetFontText} -- �ʼ���������ʾ
						}

	MailGetBoxSenderFont:SetText( t_MailHead[1][2] );
	MailGetBoxTitleFont:SetText( t_MailHead[2][2] );
	MailContent_Rich:SetText( t_MailHead[3][2], 88, 73, 53 );
	MailGetBoxMoneysCountFont:SetText( t_MailHead[4][2] );

	local sender	= getglobal("MailGetBoxSenderFont");
	local tips		= getglobal("MailGetBoxSenderTipFont")
	local value		= MainData.m_MailHead.m_nFlags;
	if not bitAnd( value,MAIL_FLAG_SYSTEM ) then
		tips:SetText(t_MailHead[5][2]);
	else
		tips:SetText("");
	end
	
	if MainData.m_MailDetail.m_ItemNum <= 0 then
		for i=1, MAXSENDITEM do
			local btn = getglobal( "MailGetBoxSendItem"..i );
			if btn == nil then
				return;
			end
			btn:Disable();
		end
		if MainData.m_MailDetail.m_uGold <= 0 then
			MailRead_GetGoodsBtn:Disable();
			MailReceiveMoneyBtn:Hide();
		else
			MailRead_GetGoodsBtn:Enable();
			MailReceiveMoneyBtn:Show();
		end
	else
		MailRead_GetGoodsBtn:Enable();
	end

	for i=1, MainData.m_MailDetail.m_ItemNum do
		local Item		= MainData.m_MailDetail.m_Items[i-1];
		local ItemDef	= Item:getItemDef();
		table.insert( t_MailHead, {"MailGetBoxSendItem"..i, Item:getItemId(), GetColor()[COLOR_DEFINE], SetItemButton}) ;-- �յ�����Ʒ
		if Item:getNum() == 1 then
			table.insert( t_MailHead, {"MailGetBoxSendItem"..i.."CountFont", "", GetColor()[COLOR_DEFINE], SetFontText}) ;-- �յ�����Ʒ
		else
			table.insert( t_MailHead, {"MailGetBoxSendItem"..i.."CountFont", Item:getNum(),	GetColor()[COLOR_DEFINE], SetFontText}) ;-- �յ�����Ʒ
		end
		table.insert( t_MailHead, {"MailGetBoxSendItem"..i.."IconTexture", ItemDef.IconID, GetColor()[COLOR_DEFINE], SetTexture}) ;-- �յ�����Ʒ
		local btn	= getglobal( "MailGetBoxSendItem"..i );
		if btn == nil then
			return;
		end
		btn:Enable();
	end

	for i = MainData.m_MailDetail.m_ItemNum + 1, MAXSENDITEM do
		local btn	= getglobal( "MailGetBoxSendItem"..i );
		local btnCount	= getglobal( "MailGetBoxSendItem"..i.."CountFont" );
		local btnIcon	= getglobal( "MailGetBoxSendItem"..i.."IconTexture" );
		if btn == nil then
			return;
		end
		btn:Disable();
	end

	for i=6, table.getn( t_MailHead ) do
		t_MailHead[i][4]( t_MailHead[i][1], t_MailHead[i][2], t_MailHead[i][3] );
	end
end

-- ȫ����ȡ��ť
function MailRead_GetAllGoodsBtn_OnClick()
	local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
	local MailData = PageMail.m_MainData[MailReadFrame:GetClientUserData( 2 ) - 1];
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	if MailData.m_MailDetail.m_ItemNum <= 0 and MailData.m_MailDetail.m_uGold <= 0 then
		return ;
	end
	if not DetectMoneyTopLimit( MailData ) then
		GameMailManager:mail_GetAll( MailReadFrame:GetClientUserData( 0 ), MailReadFrame:GetClientUserData( 1 ) );
		UpdataMailPage( nCurMailPageId );
	end
end

function AgreeMoneyToBindMoney()
	local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
	local MailData = PageMail.m_MainData[MailReadFrame:GetClientUserData( 2 ) - 1];
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	GameMailManager:mail_GetAll( MailReadFrame:GetClientUserData( 0 ), MailReadFrame:GetClientUserData( 1 ) );
	UpdataMailPage( nCurMailPageId );
end
--���Я����������
function DetectMoneyTopLimit( mainData )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local curMoney		= mainplayer:getMoney() ;
	local levelupDef	= getLevelUP( mainplayer:getLv() )
	if levelupDef ~= nil and 
		MailReceiveMoneyBtn:IsShown() and 
		MailMoneyButton1_EditBackBtn:GetText() == "����" and
		( curMoney + mainData.m_MailDetail.m_uGold > levelupDef.MaxPackageMoney ) then
		
		MessageBox("��ʾ","��ȡ�󽫳�����������Я�����ޣ�����ʱ���Զ�ת��Ϊ�������뼰ʱ���д���");
		MessageBoxFrame:SetClientString( "����ת������" );
		return true;
	else 
		return false;
	end
end

function MailRead_DeleteBtn_OnClick()
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	MailReadFrame:SetClientString( "DELETE_MAIL" );
	MessageBox("ɾ���ʼ�", "��ȷ��Ҫɾ�����ʼ���");
	--DeleteCheckMail();
end

function MailRead_ReplyBtn_OnClick()
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	if not MailSendItemFrame:IsShown() then
		MailSendItemFrame:Show();
		MailSendItemFrame:SetPoint("topleft", "$parent", "topleft",MailHeadFrame:GetRealLeft(), MailHeadFrame:GetRealTop());
		MailHeadFrame:Hide();
	end
	MailSendBtn_OnClick();
	nCurMailPageRow = MailReadFrame:GetClientUserData( 2 );
	local PageMail = GameMailManager:getOnePageMail( nCheckMailPageId );
	MailItemSender_EditBox:SetText( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_szSend );
	MailSendItemTitle_EditBox:SetText( PageMail.m_MainData[nCurMailPageRow-1].m_MailHead.m_szTitle );
end

--##	�����ʼ�����(������)		##--
-- ˢ�´������ʼ���Ʒ
function MailSendItemUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	local check = getglobal("MailItemSendItemCheckBtn");
	if check:GetCheckState() and container:getItemCount( CLIENT_MAIL_PACKAGE_ID ) <= 0 and container:getItemCount( CLIENT_MAIL_BINDED_PACKAGE_ID ) <= 0 then
		check:setCheckState( false );
		GameMailManager:clearSendItem();
	end

	local senddata	= GameMailManager:getSendData();
	for i=1, MAXMAILITEM do
		local item		= senddata.m_Items[i-1]
		local btn	= getglobal( "MailSendBoxSendItem"..i );
		local IconTex	= getglobal( "MailSendBoxSendItem"..i.."IconTexture" );
		local CountFont = getglobal( "MailSendBoxSendItem"..i.."CountFont" );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		if item:getItemId() ~= 0 then
			--�жϱ������Ƿ��������Ʒ
			local containerItem = container:getItem( item:getListType(),item:getGridIdx() );
			if containerItem:getItemId() == 0 then
				UnLockItem( item:getListType(),item:getGridIdx(),item:getItemId() );
				item:releaseItem();
				IconTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
				CountFont:SetText( "" );
				GameMailManager:delSendItem( i-1 );
			else
				local itemdef = item:getItemDef();
			
				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				IconTex:SetTexture( IconPath );
				CountFont:SetText( "" );
				if item:getNum() > 1 then
					CountFont:SetText( item:getNum() );
				end
			end
		else
			IconTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			CountFont:SetText( "" );
		end
	end
end

-- �жϼ�������������Ʒ�Ƿ��Ѿ��ڴ������б��� ����Item
function IsInMainSendItem( srcitem )
	for i = 1, MAXMAILITEM do
		local senddata	= GameMailManager:getSendData();
		local item	= senddata.m_Items[i-1];
		local itemwid	= item:getItemWId();
		if item ~= nil then
			if srcitem:getItemId() == item:getItemId() then
				return true;
			end
		end
	end
	return false;
end

--@Desc:�����ʼ���ť
function MailSendBox_ReSetBtn_OnClick()
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	ClearSendInfo();
	ClearItemLock( "MailSendItemFrame" );
end

function MailSendItemBox_ReSetBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "�����ʼ�" then
		return;
	end
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	ClearSendItemInfo();
	MailItemSender_EditBox:Clear();
	ClearItemLock( "MailSendItemFrame" );
	-- ����ƾ֤��������
	LockItem( MailSendItemFrame:GetClientUserData(0), MailSendItemFrame:GetClientUserData(1), 0, "MailSendItemFrame" ); 
end

function ClearSendInfo()
	MailSendContent_EditBox:Clear();
	MailSendItemTitle_EditBox:Clear();
	--MailItemSender_EditBox:Clear();
	
end

function CheckMaxSendMoney()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil or this:GetText() == "" then
		return;
	end
	
	local price =  tonumber( this:GetText() ); 
	if price == nil then
		return;
	end
	if not MailItemSendItemCheckBtn:GetCheckState() and price > 0 then
		ShowMidTips("�ʼ�������Ҫ�������µ���")
		this:SetText("")
		return;
	end
	if price > getPlayerMoney( MainPlayer ) then
		this:SetText( tonumber(getPlayerMoney( MainPlayer )) );
	else
		this:SetText( price );
	end	
end

function ClearSendItemInfo()
	GameMailManager:clearSendItem();
	MailSendItemContent_EditBox:Clear();
	local mainplayer = ActorMgr:getMainPlayer();
	MailSendItemTitle_EditBox:SetText("����"..mainplayer:getName().."���ʼ�"); --Elmer
	--MailItemSender_EditBox:Clear();
	for i = 1, MAXMAILITEM do
		local SendItemBtn	= getglobal( "MailSendBoxSendItem"..i);
		local SendItemTex	= getglobal( "MailSendBoxSendItem"..i.."IconTexture");
		local SendItemFont	= getglobal( "MailSendBoxSendItem"..i.."CountFont");
		SendItemBtn:SetClientUserData( 2, 0 );
		SendItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		SendItemFont:SetText("");
	end
end

function ClearSendData( nFlag )
	-- ���������ϵ��
	AddRecentlyContact( MailItemSender_EditBox:GetText() );
	ClearItemLock( "MailSendItemFrame" );
	ClearSendItemInfo();
	MailSendItemUpdate();
	GameMailManager:clearSendItem();
	local check = getglobal("MailItemSendItemCheckBtn");
	if check:GetCheckState() then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container:getItemCount(CLIENT_MAIL_BINDED_PACKAGE_ID ) <= 0 and container:getItemCount( CLIENT_MAIL_PACKAGE_ID ) <= 0 then		
			check:setCheckState( false );
		else
			--��ȡλ������λ��
			local nPos = container:getItemPos( CONTAINER_TYPE_SUNDRIES,CLIENT_MAIL_BINDED_PACKAGE_ID );
			if nPos < 0 then
				nPos = container:getItemPos( CONTAINER_TYPE_SUNDRIES,CLIENT_MAIL_PACKAGE_ID );
			end
			MailSendItemFrame:SetClientUserData( 0,CONTAINER_TYPE_SUNDRIES );
			MailSendItemFrame:SetClientUserData( 1,nPos );
			LockItem( MailSendItemFrame:GetClientUserData(0), MailSendItemFrame:GetClientUserData(1), 0, "MailSendItemFrame" ); 
		end
		MailSendItemRefreshItemBox();
	end
end

function MailSendItemBox_SendBtn_OnClick()
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then 
		ShowMidTips( "��������״̬�����ܷ����ʼ�!" );
		return;
	end
	if MailItemSender_EditBox:GetText() == "" or MailSendItemTitle_EditBox:GetText() == "" then
		ShowMidTips( "����ʧ��, ��ȷ���ռ��˺ͱ��ⲻΪ��!" );
		return;
	else		
		local MailSendData = GameMailManager:getSendData();
		if MailSendData.m_nItemNum > 0 then			
			MessageBox("�����ʼ�","�ʼ��к��й�����Ʒ����ȷ���Ƿ��ʼ�");
			--MessageBoxFrame:SetClientString(MailSendItemMoney_EditBox:GetText());
			return;
		else
			local check = getglobal("MailItemSendItemCheckBtn");
			if check:GetCheckState() then
				ShowMidTips("�������Ҫ�ʼĵ���Ʒ");
				return;
			end
		end
		SendItemMail();
	end
end

function MailFrame_OnUpdate()
	CalculatedPostage();
	if n_TimeFlag == 1 then
		if os.clock() - n_MailSendTime > MAILSENDOUTTIME then
			n_SendSuccess = 1;
			n_TimeFlag = 0;
		end
	end

	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "�����ʼ�" then
		MailItemSender_EditBox:enableEdit( false );
		MailSendItemTitle_EditBox:enableEdit( false );
		MailSendItemContent_EditBox:enableEdit( false );
		--MailSendItemMoney_EditBox:enableEdit( false );
	else
		MailItemSender_EditBox:enableEdit( true );
		MailSendItemTitle_EditBox:enableEdit( true );
		MailSendItemContent_EditBox:enableEdit( true );
		--MailSendItemMoney_EditBox:enableEdit( true );
	end
end

function SendItemMail()
	if n_SendSuccess == 0 then
		--ShowMidTips( "����æ�����Ժ��ٷ���" );
		return;
	else
		n_SendSuccess = 0;
	end
	if MailItemSender_EditBox:GetText() ~= "" then
		GameMailManager:mail_Send( MailItemSender_EditBox:GetText(), MailSendItemTitle_EditBox:GetText(), MailSendItemContent_EditBox:GetText(), MailSendItemFrame:GetClientUserData(0), MailSendItemFrame:GetClientUserData(1));
		n_MailSendTime = os.clock();
		n_TimeFlag = 1;
	end
end

function MailSendItemFrame_OnLoad()
	for i = 1, MAXSENDITEM do
		local SendItemBackTex = getglobal( "MailSendBoxSendItem"..i.."BackTexture" )
		SendItemBackTex:Show();

		local SendItem = getglobal( "MailSendBoxSendItem"..i );
		SendItem:SetClientUserData( 0 , DRAG_MAIL );
		SendItem:SetClientUserData( 1 , i );
	end
	ItemFirstCallBack("MailSendItemFrame", AddItemToMail );
	this:RegisterEvent("GE_CONTAINER_UPDATE");
end

function MailSendItemFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		MailSendItemUpdate();
	end
end

function MailSendItemFrame_OnShow()
	-- ���ô�������		
	--this:SetPoint("center", "$parent", "center", 0, 0 );
	--��ձ���������Ʒ�б�
	ClearItemLock( "MailSendItemFrame" );

	local mailheadbtn = getglobal( this:GetName().."MailHeadBtn" );
	local mailsendbtn = getglobal( this:GetName().."MailSendBtn" );
	mailheadbtn:DisChecked();
	mailsendbtn:Checked();
	GameMailManager:clearSendItem();
	MailSendItemUpdate();
	local mainplayer = ActorMgr:getMainPlayer();
	if MailSendItemTitle_EditBox:GetText() == "" then
		MailSendItemTitle_EditBox:SetText("����"..mainplayer:getName().."���ʼ�"); --Elmer
	end
	
	--MailSendItemContent_EditBox:Clear();
	MailItemSender_EditBox:Show();
	SetFocusFrame( "MailItemSender_EditBox" );
	local check = getglobal("MailItemSendItemCheckBtn");
	check:setCheckState( false );
	MailSendItemRefreshItemBox();
	this:SetClientUserData( 0,0 );
	this:SetClientUserData( 0,-1 );
end

function MailSendItemFrame_OnHide()
	--��ձ���������Ʒ�б�
	ClearItemLock( "MailSendItemFrame" );
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
	GameTooltip:Hide();
	MailSendItemContent_EditBox:enableIME( false );
	ContactListTipsFrame:Hide();
	AutoHideNomalTips( "MailSendItemFrame" );
	--[[
	if MailReadFrame:IsShown() then
		MailReadFrame:Hide();
	end
	]]--
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "�����ʼ�" then
		MessageBoxFrame:Hide();
	end
	if not MailHeadFrame:IsShown() then   --Elmer
		ClearSendItemInfo();
		MailItemSender_EditBox:Clear();
	end
end

function Mail_ScrollUpBtn_OnClick()

end

function Mail_ScrollDownBtn_OnClick()

end

function Mail_ScrollBar_OnValueChanged()

end

function MailSendMoney_EditBackBtn_OnClick()

end

function HideContactListFrame()
	HideUIPanel("ContactListTipsFrame");
end

function Mail_FirstPageBtn_OnClick()
	if nCurMailPageId ~= 0 then
		UpdataMailPage( 0 );
		-- ���ҵ�ǰ���е������ø��и���
		DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
		MailReadFrame:Hide();
		
		local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
		local state = checkBtn:GetCheckState() ;
		if not state then 
			SetMailHeadCheckState( false );
		end
	end
end

function Mail_LastPageBtn_OnClick()
	if nCurMailPageId ~= nPageTotal - 1 then
		UpdataMailPage( nPageTotal - 1 );
		-- ���ҵ�ǰ���е������ø��и���
		DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
		MailReadFrame:Hide();
		
		local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
		local state = checkBtn:GetCheckState() ;
		if not state then 
			SetMailHeadCheckState( false );
		end
	end
end

function DisCheckAllMailHighLightBtn( szButtonName, nMax )
	for i=1, nMax do
		local CheckBtn = getglobal( szButtonName..i );
		if CheckBtn ~= nil then
			CheckBtn:DisChecked();
			CheckBtn:DisHighlight();
		end
	end
end

function GetCheckMailInfo()
	local t_checkmail = { nCheckMailPageId, nCurMailPageRow };
	return t_checkmail;
end

function NewMails_Tips( n_NewMailNum, n_UserMailBoxFull, n_SysMailBoxFull )
	local strNewMailNumTips		= "";
	local strUserMailFullTips	= "";
	local strSysMailFullTips	= "";
	if n_NewMailNum ~= 0 then
		strNewMailNumTips = "����"..n_NewMailNum.."��δ���ʼ�!";
		if not MailHeadFrame:IsShown() then
			SetMailBtnHighlight();
			MailBtn:SetClientString("����"..n_NewMailNum.."��δ���ʼ�����ע�����");
		end
	end
	if n_UserMailBoxFull ~= 0 then
		strUserMailFullTips	= " ��ͨ�ʼ����������뼰ʱ����";
		NormalMailsFullFont:SetText("������ͨ�����������뼰\nʱ�������ͷ�һ���ռ�");
	end
	if n_SysMailBoxFull ~= 0 then
		strSysMailFullTips	= " �ʼ����������뼰ʱ����";
		SysMailsFullFont:SetText("���������������뼰ʱ��\n�����ͷ�һ���ռ�")
	end
	if strNewMailNumTips == "" and strUserMailFullTips =="" and strSysMailFullTips =="" then
		return;
	end
	ShowMidTips( strNewMailNumTips..strUserMailFullTips..strSysMailFullTips );
	SetSystemMessage( strNewMailNumTips..strUserMailFullTips..strSysMailFullTips );
end

function ItemAlreadyExistTips()
	ShowMidTips( "����Ʒ����Ʒ���������Ѵ���" );
end
local delMailIndex = 1;
local delMailStart = false;
function Mail_DeleteALLBtn_OnClick()
	if nTotalHeadNum == 0 then
		return;
	end
	for i = 1, MAXMAILHEAD do	
		local mailCheckBtn 	= getglobal( "MailInfo"..i.."CheckBtn" ); 
		if mailCheckBtn:IsShown() and mailCheckBtn:GetCheckState() then
			break;
		end
		if i == MAXMAILHEAD  then 
			ShowMidTips("δ��ѡ�κ��ʼ���");
			return;	
		end	
	end
	local mainplayer	= ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	local state = checkBtn:GetCheckState() ;
	if state then 
		DelAllMailConfirmFrameShortMessageFont:SetText("ȷ��Ҫɾ�������ʼ���?");
		DelAllMailConfirmFrameLongMessageFont:SetText("��ע�⣺�ռ���������δ��ȡ�����ҡ���һ���Ʒ������һ��ɾ��������");
		DelAllMailConfirmFrameLongMessageFont:SetTextColor( 202, 54,  33 );
	else 
		DelAllMailConfirmFrameShortMessageFont:SetText("ȷ��Ҫɾ���ѹ�ѡ�ʼ���?");
		DelAllMailConfirmFrameLongMessageFont:SetText("��ע�⣺�ռ���������δ��ȡ�����ҡ���һ���Ʒ������һ��ɾ��������");
		DelAllMailConfirmFrameLongMessageFont:SetTextColor( 202, 54,  33 );
	end
	DelAllMailConfirmFrame:Show();
end

function MailReadFrame_OnHide()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
	GameTooltip:Hide();
	ReSetMailContent();
	AutoHideNomalTips( "MailReadFrame" );
end
local t_DelMailWid = {};
function DeleteAllMail()
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	local state = checkBtn:GetCheckState() ;
	if state then 
		GameMailManager:mail_Delete_All();
		
		ReSetMailContent();
		UpdataMailPage( 0 );	
		nTotalHeadNum = 0;
	else 
		local PageMail = GameMailManager:getOnePageMail( nCurMailPageId );
		if PageMail == nil then
			return;
		end
		t_DelMailWid = {};
		for i = 1, MAXMAILHEAD do
			local mailCheckBtn 	= getglobal( "MailInfo"..i.."CheckBtn" ); 
			if 	mailCheckBtn:IsShown() and mailCheckBtn:GetCheckState() then	
				local mailHead = PageMail.m_MainData[i - 1].m_MailHead;
				table.insert( t_DelMailWid, { wHid = mailHead.m_WID.nHeight, wLid = mailHead.m_WID.nLow } );
			end
		end
		SetMailHeadCheckState( false );
		delMailStart = true;
		delMailIndex = 1;
	end
	MailReadFrame:Hide();
	DisCheckAllMailHighLightBtn( "MailInfo", MAXMAILHEAD );
	DelAllMailConfirmFrame:Hide();
end

function Mail_DeleteALLBtn_OnEnter()
	SetGameTooltips(this:GetName(), "ɾ���ѹ�ѡ�ʼ�" );
end

function Mail_DeleteALLBtn_OnUpdate() 
	if not delMailStart then 
		return;
	end
	local num = 0;
	for i = delMailIndex, #t_DelMailWid do
		GameMailManager:mail_Delete( t_DelMailWid[i].wHid, t_DelMailWid[i].wLid );	
		num = num + 1;
		delMailIndex = delMailIndex + 1;		
		if i == #t_DelMailWid then 
			delMailStart = false;
			delMailIndex = 1;
			break;
		end
		if num == 2 then
			break;
		end
	end	
end 

function GetAllGoods( nItemIDHigh, nItemIDLow )
	if nItemIDHigh == MailReadFrame:GetClientUserData( 0 ) and nItemIDLow == MailReadFrame:GetClientUserData( 1 ) then
		for i=1, MAXMAILITEM do
			local petButton = getglobal( "MailGetBoxSendItem"..i );
			local icontex   = getglobal( "MailGetBoxSendItem"..i.."IconTexture");
			local countfont   = getglobal( "MailGetBoxSendItem"..i.."CountFont");

			if petButton:IsChecked() then
				--GameMailManager:mail_GetGoods( MailReadFrame:GetClientUserData( 0 ), MailReadFrame:GetClientUserData( 1 ), i - 1 );
				petButton:SetClientUserData( 2, 0 );
				icontex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
				countfont:SetText("");
				
				petButton:DisChecked();
				NormalItemTipsFrame:Hide();
				ArmItemTipsFrame:Hide();
				SecondArmItemTipsFrame:Hide();
				GameTooltip:Hide();
				return;
			end
		end
	end
end

function DeleteEffect( nItemIDHigh, nItemIDLow ,nFlags)
	if nItemIDHigh == MailReadFrame:GetClientUserData( 0 ) and nItemIDLow == MailReadFrame:GetClientUserData( 1 ) then
		for i=1, MAXMAILITEM do
			local petButton = getglobal( "MailGetBoxSendItem"..i );
			local icontex   = getglobal( "MailGetBoxSendItem"..i.."IconTexture");
			local countfont   = getglobal( "MailGetBoxSendItem"..i.."CountFont");
			petButton:SetClientUserData( 2, 0 );
			icontex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			countfont:SetText("");
		end
		UpdataMailPage( nCurMailPageId );
		MailReadFrame:Hide();
	end

	--�ʼ�ɾ��
	MailHeadFrameMailSysFullBtn:Hide();
	SysMailsFullFont:SetText("");
	if NormalMailsFullFont:GetText() ~= "" then
		MailHeadFrameMailNormalFullBtn:Show();
	end
	--��ͨ�ʼ�ɾ��
	if not bitAnd( nFlags,MAIL_FLAG_SYSTEM ) then
		NormalMailsFullFont:SetText("");
		MailHeadFrameMailNormalFullBtn:Hide();
	end
end

function MailHeadFrame_OnUpadate()
	UpdataMailPage( nCurMailPageId );
end

function MailSendItemBox_CanelBtn_Click()
	if MailSendItemContent_EditBox:GetText() ~= "" then
		MessageBox("�ʼ���ʾ","��δ�����ʼ����ݣ��Ƿ�����ر��ʼ����棿");
		return;
	end
	MailSendItemFrame:Hide();
	if UIIsInDragState() then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

function MailSendReply()
	n_SendSuccess = 1;
	n_TimeFlag = 0;
end

function AddItemToMail( item, nlisttype, nGridIndex )
	local check = getglobal("MailItemSendItemCheckBtn");
	if not check:GetCheckState() then
		if item:getItemId() == CLIENT_MAIL_PACKAGE_ID or item:getItemId() == CLIENT_MAIL_BINDED_PACKAGE_ID then		
			MailSendItemFrame:SetClientUserData(0,nlisttype);
			MailSendItemFrame:SetClientUserData(1,nGridIndex);			
			check:setCheckState( true );
			MailSendItemRefreshItemBox();
			LockItem( nlisttype, nGridIndex, item:getItemId(), "MailSendItemFrame" ); 			
		end
		return;
	end
	local senddata	= GameMailManager:getSendData();
	if item == nil or senddata == nil then
		return;
	end

	if senddata.m_nItemNum == MAXMAILITEM then
		ShowMidTips("���ʧ��,������Ʒ������");
	end

	for i=1, MAXMAILITEM do
		local ExistItem		= senddata.m_Items[i-1];
		if ExistItem == nil then
			return;
		end
		if ExistItem:getItemId() == 0 then
			ReceiveMail( "MailGetBoxSendItem"..i, item:getListType(), item:getGridIdx(), 0, 0 );
			return;
		end
	end
end

function MailItemSendItemCheckBtn_OnClick()
	local check = getglobal("MailItemSendItemCheckBtn");
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "�����ʼ�" then
		check:setCheckState( not check:GetCheckState() )
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container:getItemCount(CLIENT_MAIL_BINDED_PACKAGE_ID ) <= 0 and container:getItemCount( CLIENT_MAIL_PACKAGE_ID ) <= 0 then
		
		check:setCheckState( false );
		ShowMidTips("��Ҫ���£��ſ��Է�����Ʒ");
	else
		--��ȡλ������λ��
		local nPos = container:getUnLockItemPos( CONTAINER_TYPE_SUNDRIES,CLIENT_MAIL_BINDED_PACKAGE_ID );
		if nPos < 0 then
			nPos = container:getUnLockItemPos( CONTAINER_TYPE_SUNDRIES,CLIENT_MAIL_PACKAGE_ID );
		end
		if nPos < 0 then
			--ShowMidTips("��Ʒ�ѱ�����");
			check:setCheckState( false );
		else
			MailSendItemFrame:SetClientUserData( 0,CONTAINER_TYPE_SUNDRIES );
			MailSendItemFrame:SetClientUserData( 1,nPos );
			LockItem( MailSendItemFrame:GetClientUserData(0), MailSendItemFrame:GetClientUserData(1), 0, "MailSendItemFrame" );
		end
	end
	MailSendItemRefreshItemBox();
end

function MailSendItemRefreshItemBox()
	local check = getglobal("MailItemSendItemCheckBtn");
	for i=1, MAXMAILITEM do
		local btn = getglobal( "MailSendBoxSendItem"..i );
		local lockTex = getglobal("MailSendBoxSendItem"..i.."LockTexture");
		if not check:GetCheckState() then
			lockTex:Show();
			btn:Disable();			
			GameMailManager:clearSendItem();
			MailSendItemUpdate();
			ClearItemLock( "MailSendItemFrame" );
			MailSendItemFrame:SetClientUserData( 0,0 );
			MailSendItemFrame:SetClientUserData( 1,-1 );
		else
			lockTex:Hide();
			btn:Enable();
		end
	end
end

function MailItemBullIcon_EditBackBtn_OnEnter()
	SetGameTooltips(this:GetName(), "�ʼ���Ҫ����10����" );
end

function MailFramesCloseBtn_OnClick()
	if MailSendItemContent_EditBox:GetText() ~= "" then
		MessageBox("�ʼ���ʾ","��δ�����ʼ����ݣ��Ƿ�����ر��ʼ����棿");
		return;
	end
	if this:GetParent() == "MailHeadFrame" or this:GetParent() == "MailSendItemFrame" then
		if MailReadFrame:IsShown() then
			MailReadFrame:Hide();
		end
	end
	HideUIPanel(this:GetParent());
end

function MailFramesClose( FrameName )
	if MailSendItemContent_EditBox:GetText() ~= "" then
		MessageBox("�ʼ���ʾ","��δ�����ʼ����ݣ��Ƿ�����ر��ʼ����棿");
		return;
	end
	if MailReadFrame:IsShown() then
			MailReadFrame:Hide();
	end
	local frame = getglobal( FrameName );
	frame:Hide();
end
----------------------------- һ����ȡ�ʼ�-------------------------------
function MailHeadSelectAllCheckBtn_OnClick()
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	local state = checkBtn:GetCheckState();
	SetMailHeadCheckState( state )
end

function MailHeadSelectAllBtn_OnClick()
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	checkBtn:setCheckState( not checkBtn:GetCheckState() );
	local state = checkBtn:GetCheckState() ;
	SetMailHeadCheckState( state )
end

function SetMailHeadCheckState( state )
	for i=1,MAXMAILHEAD do
		local mailCheckBtn 	= getglobal( "MailInfo"..i.."CheckBtn" ); 
		if 	mailCheckBtn:IsShown() then
			mailCheckBtn:setCheckState( state );
		end
	end
end

function MailInfoCheckBtn_OnClick()
	local state = this:GetCheckState();
	local PageMail = GameMailManager:getOnePageMail( nCurMailPageId );
	if PageMail == nil then
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	local mailBtn  = getglobal( this:GetParent() );
	local mailData = PageMail.m_MainData[mailBtn:GetClientID() - 1].m_MailDetail;
	local mailHead = PageMail.m_MainData[mailBtn:GetClientID() - 1].m_MailHead;
	if not state then 
		local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
		checkBtn:setCheckState(false);
	else 
	--[[
		if bitAnd( mailHead.m_nFlags, MAIL_FLAG_READ ) and not bitAnd( mailHead.m_nFlags,MAIL_FLAG_NONORM ) then
			ShowMidTips("���ʼ��Ѳ鿴������ȡ��Ʒ������һ����ȡ");
			this:setCheckState(false);
		end
		--]]
	end
end
local sendMailIndex = 1;
local sendMailStart = false;

function Mail_ReceiveAllBtn_OnClick()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local PageMail	 	= GameMailManager:getOnePageMail( nCurMailPageId );
	if PageMail == nil then
		return;
	end
	for i = 1, MAXMAILHEAD do	
		-- �����ռ䲻��
		local mailHead 		= PageMail.m_MainData[i - 1].m_MailHead;
		local mailCheckBtn 	= getglobal( "MailInfo"..i.."CheckBtn" ); 
		if mailCheckBtn:IsShown() and mailCheckBtn:GetCheckState() then
			if container:getUnusedGridNum( CONTAINER_TYPE_SUNDRIES ) == 0 
			  and not bitAnd( mailHead.m_nFlags,MAIL_FLAG_SYSTEM ) 
			  and bitAnd( mailHead.m_nFlags,MAIL_FLAG_NONORM ) then 
				ShowMidTips("�����ռ䲻�㣬����������");
				return;
			end
		end 
	end
	for i = 1, MAXMAILHEAD do	
		local mailCheckBtn 	= getglobal( "MailInfo"..i.."CheckBtn" ); 
		if mailCheckBtn:IsShown() and mailCheckBtn:GetCheckState() then
			break;
		end
		if i == MAXMAILHEAD  then 
			ShowMidTips("δ��ѡ�κ��ʼ���");
			return;	
		end	
	end
	
	local checkBtn = getglobal("MailHeadFrameSelectAllBtnCheckBtn");
	local state = checkBtn:GetCheckState() ;
	if state then 
		GameMailManager:mail_Get_All();
	else
		sendMailStart = true;
	end
	if MailReadFrame:IsShown() then 
		MailReadFrame:Hide();
	end
end

function Mail_ReceiveAllBtn_OnEnter()
	SetGameTooltips(this:GetName(), "��ȡ�ѹ�ѡ�ʼ�" );
end

function Mail_ReceiveAllBtn_OnUpdate()
	if not sendMailStart then 
		return;
	end
	local num = 0;
	for i = sendMailIndex, MAXMAILHEAD do
		local mailCheckBtn 	= getglobal( "MailInfo"..i.."CheckBtn" ); 
		if 	mailCheckBtn:IsShown() and mailCheckBtn:GetCheckState() then
			local PageMail = GameMailManager:getOnePageMail( nCurMailPageId );
			if PageMail == nil then
				return;
			end
			local mailHead = PageMail.m_MainData[i - 1].m_MailHead;
			GameMailManager:mail_GetAll( mailHead.m_WID.nHeight, mailHead.m_WID.nLow,1 );	
			num = num + 1;
		end
		sendMailIndex = sendMailIndex + 1;
		if i == MAXMAILHEAD then 
			sendMailStart = false;
			sendMailIndex = 1;
			break;
		end
		if num == 2 then
			break;
		end	
	end	
end