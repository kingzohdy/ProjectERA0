-------------------------------------------------------����ݵ�ս-----------------------------------------------------

----------------------------------------------------�������ͼ-----------------------------------------------
local MAX_TRANS_STRONG_NUM = 13
local MAX_RSALE_NUM_TOLUA = 32
local nCurSelectSpanStrongIdx = 13;
local t_strongInfo = {
	--�Ÿ�һ���ݵ�
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 1, stateIndex = STRONG_STAT_NOT_OPEN },
	--���������ݵ�
	{ name="", lv = 2, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 2, stateIndex = STRONG_STAT_NOT_OPEN },
	{ name="", lv = 2, stateIndex = STRONG_STAT_NOT_OPEN },
	--һ�������ݵ�	
	{ name="", lv = 3, stateIndex = STRONG_STAT_NOT_OPEN },
}
function EnterTransForcesMap_OnClick()
	WarForcesFrame:Hide()
	ClanStrongInfoFrame:Hide()
	GameClan_SpanBaseWar:ReqSpanStrongInfo()
end
function EnterSelfForcesMap_OnClick()
	TransWarForcesFrame:Hide()
	WarForcesFrame:Show()
end
function TransWarForcesFrame_OnShow()
	WarForcesFrame:Hide()
	--Ĭ��ѡ�������
	for i = 1, MAX_TRANS_STRONG_NUM do
		local btn = getglobal("TransWarForcesFrameAreaBtn"..i)
		btn:DisChecked()
	end
	TransWarForcesFrameAreaBtn13:Checked()
	nCurSelectSpanStrongIdx = 13;
	SetTransBaseMap();
end
function TransWarForcesFrame_OnHide()
	TransWarForcesFrameStrongChooseGroup:Hide()
	SpanStrongWarAwardFrame:Hide()
end
function TransWarForcesFrame_OnLoad()
	this:RegisterEvent( "GE_SPAN_STRONG_ALL_INFO" )
	this:RegisterEvent( "GE_SPAN_STRONG_CHANGE_INFO" )
	TransWarForcesFrameChoseStrongBtnSetGroup:SetText( " #Lս �� �� ��#n", 255,255,190 )
end
function TransWarForcesFrame_OnEvent()
	if arg1 == "GE_SPAN_STRONG_ALL_INFO" then
		if not TransWarForcesFrame:IsShown() then
			TransWarForcesFrame:Show()
		else
			SetTransBaseMap();
		end
	elseif arg1 == "GE_SPAN_STRONG_CHANGE_INFO" then
		SetTransBaseMap();
	end
end
function IsJoinSpanStrongSale( strongSaleInfo )
	local mainplayer = ActorMgr:getMainPlayer()
	for i = 1, MAX_RSALE_NUM_TOLUA do
		if strongSaleInfo.RPlayer[i-1].ClanID == mainplayer:getClanID() and mainplayer:isInClan() then
			return true;
		end
	end
	return false;
end
function GetClanJoinSpanSaleMoney( strongSaleInfo )
	local mainplayer = ActorMgr:getMainPlayer()
	for i = 1, MAX_RSALE_NUM_TOLUA do
		if strongSaleInfo.RPlayer[i-1].ClanID == mainplayer:getClanID() and mainplayer:isInClan() then
			return strongSaleInfo.RPlayer[i-1].Sale;
		end
	end
	return 0;
end
function SetTransBaseMap()
	local mainplayer = ActorMgr:getMainPlayer()
	for i = 1, MAX_TRANS_STRONG_NUM do
		local btn = getglobal("TransWarForcesFrameAreaBtn"..i)
		local btnName = getglobal( btn:GetName().."Name" )
		local SateTex = getglobal( btn:GetName().."SateTex" )
		local FangTex = getglobal( btn:GetName().."FangTex" )	
		local FangName = getglobal( btn:GetName().."FangName" )
		local GongTex = getglobal( btn:GetName().."GongTex" )
		local GongName = getglobal( btn:GetName().."GongName" )
		local strongInfo = GameClan_SpanBaseWar:getSpanStrongInfoByIndex( i )
		local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef(strongInfo.StrongID)
		btnName:SetText( string.sub( SpanStrongDef.Name, 1, 6 ) )
		--���жϾݵ��Ƿ���
		if strongInfo.IsOpen == 1 then
			local uv = t_strongStateUV[strongInfo.Stat];
			t_strongInfo[i].stateIndex = strongInfo.Stat;
			if strongInfo.Owner.ClanID == 0 then
				uv = t_strongStateUV[STRONG_STAT_NOT_OWNER];
				t_strongInfo[i].stateIndex = STRONG_STAT_NOT_OWNER;
			end
			if strongInfo.Stat == STRONG_STAT_IDLE_TOLUA then
				uv = t_strongStateUV[strongInfo.Stat];
				t_strongInfo[i].stateIndex = strongInfo.Stat;
				if strongInfo.Owner.ClanID == 0  then
					uv = t_strongStateUV[STRONG_STAT_NOT_OWNER];
					t_strongInfo[i].stateIndex = STRONG_STAT_NOT_OWNER;
				end
			elseif strongInfo.Stat == STRONG_STAT_SALE_TOLUA then
				uv = t_strongStateUV[strongInfo.Stat];
				t_strongInfo[i].stateIndex = strongInfo.Stat;
				--�Ƿ�μӾ���
				if IsJoinSpanStrongSale( strongInfo.Sale ) and mainplayer:getClanPosition() ~= CLAN_POS_NORMAL then
					uv = t_strongStateUV[STRONG_STAT_JOINT_SALE];
					t_strongInfo[i].stateIndex = STRONG_STAT_JOINT_SALE;
				end
			elseif strongInfo.Stat == STRONG_STAT_TONGJI_TOLUA then
				uv = t_strongStateUV[strongInfo.Stat];
				t_strongInfo[i].stateIndex = strongInfo.Stat;
			elseif strongInfo.Stat == STRONG_STAT_SELECT_TOLUA then
				uv = t_strongStateUV[strongInfo.Stat];
				t_strongInfo[i].stateIndex = strongInfo.Stat;
				if strongInfo.Owner.ClanID == 0 then
					uv = t_strongStateUV[STRONG_STAT_NOT_OWNER];
					t_strongInfo[i].stateIndex = STRONG_STAT_NOT_OWNER;
				else
					if strongInfo.Owner.ClanID == 0 then
						uv = t_strongStateUV[STRONG_STAT_IDLE_TOLUA];
						t_strongInfo[i].stateIndex = STRONG_STAT_IDLE_TOLUA;
					end
				end
			elseif strongInfo.Stat == STRONG_STAT_FIGHT_TOLUA then
				uv = t_strongStateUV[strongInfo.Stat];
				t_strongInfo[i].stateIndex = strongInfo.Stat;
				if strongInfo.Owner.ClanID == 0 then
					uv = t_strongStateUV[STRONG_STAT_IDLE_TOLUA];
					t_strongInfo[i].stateIndex = STRONG_STAT_IDLE_TOLUA;
				end
			end
			local fangClanName = strongInfo.Owner.ClanName;
			local r, g, b = 255, 255, 255
			FangName:SetText( fangClanName );
			FangName:SetTextColor( r, g, b );
			FangTex:Show();
			if fangClanName == "" or fangClanName == nil then
				FangTex:Hide();
			end
			local gongClanName = strongInfo.Atk.ClanName
			GongTex:Show();
			if gongClanName == "" or gongClanName == nil then
				GongTex:Hide();
			end
			GongName:SetText( gongClanName );
			--�����������,��û�п���
			if (gongClanName ~= "" and gongClanName ~= nil) and strongInfo.Stat ~= STRONG_STAT_FIGHT_TOLUA then
				--�����Լ�����
				if gongClanName ~= mainplayer:getClanName() then
					GongName:SetText( "������ս��..." );
				end
				--���Լ����ᣬ�����Լ����ǹ����Ա
				if gongClanName == mainplayer:getClanName() and mainplayer:getClanPosition() == CLAN_POS_NORMAL then
					GongName:SetText( "������ս��..." );
				end

				SpanStrongSaleFrameStrongStatAddInfo:SetText( "������ս���᣺"..gongClanName );
			end
			if strongInfo.Owner.ClanID == strongInfo.Atk.ClanID and ( gongClanName ~= "" and gongClanName ~= nil) then
				GongTex:Hide();
				GongName:SetText( "" );
				uv = t_strongStateUV[STRONG_STAT_IDLE_TOLUA];
				t_strongInfo[i].stateIndex = STRONG_STAT_IDLE_TOLUA;
			end
			SateTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
		else
			FangName:SetText( "" );
			FangTex:Hide();
			local uv = t_strongStateUV[STRONG_STAT_NOT_OPEN];
			t_strongInfo[i].stateIndex = STRONG_STAT_NOT_OPEN;
			SateTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
			GongTex:Hide();
			GongName:SetText( "" );
		end	
	end
	SetSelectTransBaseInfo();
end
function SetSelectTransBaseInfo()
	for i = 1, MAX_TRANS_STRONG_NUM do
		local btn = getglobal("TransWarForcesFrameAreaBtn"..i)
		if btn:IsChecked() then
			local strongInfo = GameClan_SpanBaseWar:getSpanStrongInfoByIndex( i )
			local btn = getglobal( "TransWarForcesFrameStrongInfoBtn" )
			local title = getglobal( btn:GetName().."TitleFont" )
			local StrongName = getglobal( btn:GetName().."StrongName" )
			local StrongLV = getglobal( btn:GetName().."StrongLV" )
			local StrongPrice = getglobal( btn:GetName().."StrongPrice" )
			local StrongOwnerClan = getglobal( btn:GetName().."StrongOwnerClan" )
			local StrongOwnerClanSvrName = getglobal( btn:GetName().."StrongOwnerClanSvrName" )
			local StrongState = getglobal( btn:GetName().."StrongState" )
			local StrongChanllenger = getglobal( btn:GetName().."StrongChanllenger" )
			local JoinSaleClanNum = getglobal( btn:GetName().."JoinSaleClanNum" )
			local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( strongInfo.StrongID )
			title:SetText( SpanStrongDef.Name )
			StrongName:SetText( "�ݵ����ƣ�"..SpanStrongDef.Name )
			StrongLV:SetText( "�ݵ�ȼ���"..SpanStrongDef.Lvl )
			StrongPrice:SetText( "���ĵͼۣ�".. SpanStrongDef.BaseUse )
			StrongOwnerClan:SetText( "ռ�칫�᣺"..strongInfo.Owner.ClanName )
			StrongOwnerClanSvrName:SetText( "ռ���������"..LoginMgr:getWorldNameById(strongInfo.Owner.SvrID) )
			local StateIndex = t_strongInfo[i].stateIndex
			StrongState:SetText( "�ݵ�״̬��"..t_strongStateUV[StateIndex].name )
			StrongChanllenger:SetText( "��ս���᣺"..strongInfo.Atk.ClanName )
			local fangClanName = strongInfo.Owner.ClanName;
			local gongClanName = strongInfo.Atk.ClanName
			local mainplayer = ActorMgr:getMainPlayer()
			--�����������,��û�п���
			if (gongClanName ~= "" and gongClanName ~= nil) and strongInfo.Stat ~= STRONG_STAT_FIGHT_TOLUA then
				--�����Լ�����
				if gongClanName ~= mainplayer:getClanName() then
					StrongChanllenger:SetText( "������ս��..." )
				end
				--���Լ����ᣬ�����Լ����ǹ����Ա
				if gongClanName == mainplayer:getClanName() and mainplayer:getClanPosition() == CLAN_POS_NORMAL then
					StrongChanllenger:SetText( "������ս��..." )
				end
			end
			JoinSaleClanNum:SetText( "���뾺�Ĺ�������"..strongInfo.Sale.iPlayer .."��" )
			
			local EnterBaseWarBtn = getglobal( btn:GetName().."EnterBaseWarBtn" )
			if StateIndex == STRONG_STAT_FIGHT_TOLUA then
				EnterBaseWarBtn:Enable();
			else
				EnterBaseWarBtn:Disable();
			end
		end
	end
end
function TransWarForcesFrameStrongInfoBtnOwnerAwardBtn_OnClick()
	GameClan_SpanBaseWar:ReqSpanStrongAward( nCurSelectSpanStrongIdx )	
end
function TransWarForcesFrameStrongInfoBtnOwnerAwardShowBtn_OnClick()
	SpanStrongWarAwardFrame:SetClientUserData( 0, nCurSelectSpanStrongIdx )
	SpanStrongWarAwardFrame:Show()
end
function TransWarForceBtn_OnClick()
	TransWarForcesFrameStrongChooseGroup:Hide()
	for i = 1, MAX_TRANS_STRONG_NUM do
		local btn = getglobal("TransWarForcesFrameAreaBtn"..i)
		btn:DisChecked()
	end
	this:Checked()
	nCurSelectSpanStrongIdx = this:GetClientID()
	SetSelectTransBaseInfo()
	SpanStrongWarAwardFrame:Hide()
end
----����ݵ�ѡ��
function TransWarForcesFrameChoseStrongBtnChose_OnClick()
	local parentEditBox = getglobal( this:GetParent().."EditBox" )
	local strongs = getglobal( "TransWarForcesFrameStrongChooseGroup" )
	if strongs:IsShown() then
		strongs:Hide()	
	else
		strongs:Show()
		strongs:SetPoint( "topleft", "UIClient", "topleft", parentEditBox:GetRealLeft(), parentEditBox:GetRealBottom() )
	end
end
function ClanGroupSpanStrongChoseGroupBtnTemplate_OnClick()
	--�й��ᣬ���ǻ᳤
	local mainplayer = ActorMgr:getMainPlayer()
	if not mainplayer:isInClan() or not mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
		ShowMidTips( "��û��Ȩ���޸�ս��" )
		TransWarForcesFrameStrongChooseGroup:Hide()
		return
	end
	--���ݹ���ȼ����Ƿ񿪷��˻ƽ�������ÿ��ŵĹٽ�
	local clanGroupInfo = GameClanManager:getClanGroupInfo()
	local clanview = GameClanManager:getClanViewer()
	local clanInfo = clanview.m_clanInfo
	if this:GetClientID() > clanInfo.m_nClanLevel and this:GetClientID() ~= 6 then
		ShowMidTips( "���Ĺ��ỹδӵ�и�ս��" )
		TransWarForcesFrameStrongChooseGroup:Hide()
		return
	end
	--����Ѿ�ʹ���˾���ƾ֤�����˵�������
	if clanGroupInfo.UseItemGroup == 0 and this:GetClientID() == 6 then
		ShowMidTips( "���Ĺ��ỹδӵ�и�ս��" )
		TransWarForcesFrameStrongChooseGroup:Hide()
		return
	end
	GameClan_SpanBaseWar:ReqSelectGroup( nCurSelectSpanStrongIdx, this:GetClientID() )
	TransWarForcesFrameStrongChooseGroup:Hide()
end
function TransWarForcesFrameChoseStrongBtn_OnUpdate()
	local clanGroupInfo = GameClanManager:getClanGroupInfo();
	for i = 1, clanGroupInfo.GroupNum do
		if clanGroupInfo.GroupInfo[i-1].SpanStrongID == nCurSelectSpanStrongIdx then
			TransWarForcesFrameChoseStrongBtnName:SetText( t_clanGroupName[i] )
			return
		end
	end
	TransWarForcesFrameChoseStrongBtnName:SetText( "�����ս��" )
end
function TransWarForcesFrameChoseStrongBtnSetGroup_OnClick()
	--�й��ᣬ���ǻ᳤
	local mainplayer = ActorMgr:getMainPlayer()
	if not mainplayer:isInClan() or not mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
		ShowMidTips( "��û��Ȩ���޸�ս��" )
		return
	end
	local clanGroupInfo = GameClanManager:getClanGroupInfo();
	for i = 1, clanGroupInfo.GroupNum do
		if clanGroupInfo.GroupInfo[i-1].SpanStrongID == nCurSelectSpanStrongIdx then
			ClanGroupMemberInfoFrame:Show()
			ClanGroupMemberInfoFrameTitleFont:SetText( t_clanGroupName[i] )
			ClanGroupMemberInfoFrame:Show()
			ClanGroupMemberInfoFrame:SetClientUserData( 0, i )
		end
	end
end
----����
local SpanStrongSaleInfo = {
	[1] = [[#cffffbe�ݵ�ս��飺
#ca0a05a��ս��ʱ��Ϊ�������Ľ������20��30-21��30��
#ca0a05a��ս��Ϊ��Դս,ս����ʼ��,˫��ͨ�����ս������ͼ�еĽ���ս����������ս��������,չ����Դ������,����ռ����Դ����������Դ������ս��ʱ����,���ȴﵽ��Դ���޵Ĺ���������ʤ��������ս��ʱ�����ʱ,��δ�ֳ�ʤ��,������Դ�������Ĺ����ʤ�����˫��������Դ������ͬ,��ݵ�ռ�칫���ʤ��]],
	[2] = [[#cffffbe����ʱ�䣺
#ca0a05aÿ����00��00-24��00
#cffffbe���Ĺ���
#ca0a05a����ʹ�������԰�����ʽ���С�
#ca0a05a�ڶ�������12��00�������Ľ����
#ca0a05a������߹���Ϊ����ʤ���ߡ�����ʤ��������ڽ����������20��30�μӾݵ�ս��
#cff7d00  ע����������Ĳ��ɹ�����ǰ����ϵͳ���Զ�������
#cff7d00  ���ĳɹ��������Ĺ�������ս���ʸ�ϵͳ��۳������ʽ�]],
}
local SpanBaseWarSaleTimeList = {
	[1] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	[2] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	[3] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	[4] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	[5] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	[6] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	[7] = "�´ξ���ʱ��Ϊ����00��00-24��00",
}

function GetClanSpanSaleWinnerMoney( strongSaleInfo )
	local winnerPrice = 0
	for i = 1, strongSaleInfo.iSale do
		if strongSaleInfo.RSale[i-1].Sale > winnerPrice then
			winnerPrice = strongSaleInfo.RSale[i-1].Sale;
		end
	end
	return winnerPrice;
end
function GetClanSpanSaleWinClanName( strongSaleInfo )
	local winnerPrice = 0
	local clanName = ""
	for i = 1, strongSaleInfo.iSale do
		if strongSaleInfo.RSale[i-1].Sale > winnerPrice then
			winnerPrice = strongSaleInfo.RSale[i-1].Sale;
			clanName = 	strongSaleInfo.RSale[i-1].ClanName;		
		end
	end
	return clanName;
end
function TransWarForcesFrameStrongInfoBtnJoinBaseWarBtn_OnClick()
	SpanStrongSaleFrame:SetClientUserData( 0, nCurSelectSpanStrongIdx )
	SetSpanStrongSaleInfo()
	SpanStrongSaleFrame:Show()
end
function TransWarForcesFrameStrongInfoBtnEnterBaseWarBtn_OnClick()
	GameClan_SpanBaseWar:SpanStrongEnter( nCurSelectSpanStrongIdx )
end
function SpanStrongSaleFrame_OnLoad()
	SpanStrongSaleFrameInfo:SetText( SpanStrongSaleInfo[1], 255,255,190 )
	SpanStrongSaleFrameAttention:SetText( SpanStrongSaleInfo[2], 255,255,190 )
end
function SetSpanStrongSaleInputHide( bHide )
	if bHide then
		SpanStrongSaleFrameMoneyInput:Hide();
		SpanStrongSaleFrameEnterBtn:Hide();
		SpanStrongSaleFrameEditBoxBkg:Hide();
		SpanStrongSaleFrameInputInfo:Hide();
		SpanStrongSaleFrameStrongStatAddInfo:Show();
		SpanStrongSaleFrameMoneyTex:Hide();
	else
		SpanStrongSaleFrameMoneyInput:Show();
		SpanStrongSaleFrameEnterBtn:Show();
		SpanStrongSaleFrameEditBoxBkg:Show();
		SpanStrongSaleFrameInputInfo:Show();
		SpanStrongSaleFrameStrongStatAddInfo:Hide();
		SpanStrongSaleFrameMoneyTex:Show();
	end
end
function SetSpanStrongSaleInfo()
	--���þݵ���Ϣ
	local nSpanStrongIdx = SpanStrongSaleFrame:GetClientUserData( 0 )
	local strongInfo = GameClan_SpanBaseWar:getSpanStrongInfoByIndex( nSpanStrongIdx )
	local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( nSpanStrongIdx )
	SpanStrongSaleFrameStrongName:SetText( SpanStrongDef.Name );
	SpanStrongSaleFrameStrongLV:SetText( SpanStrongDef.Lvl );
	SpanStrongSaleFrameStrongBaseVal:SetText( SpanStrongDef.BaseUse.."����" );
	if strongInfo.Owner.ClanID == 0 then
		SpanStrongSaleFrameStrongOwner:SetText( "����ռ��" );
	else
		SpanStrongSaleFrameStrongOwner:SetText( strongInfo.Owner.ClanName );
	end
	SpanStrongSaleFrameMoneyInput:SetText( SpanStrongDef.BaseUse );
	--����״̬����
	local saleStat = strongInfo.Stat;
	local saleJoin = IsJoinSpanStrongSale( strongInfo.Sale );
	local mainplayer = ActorMgr:getMainPlayer();
	if saleStat == STRONG_STAT_SALE_TOLUA then		
		if saleJoin then
			SetSpanStrongSaleInputHide( true );
			SpanStrongSaleFrameStrongStat:SetText( "�����ѿ�������" );
			if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
				local myClanSaleMoney = GetClanJoinSpanSaleMoney( strongInfo.Sale );
				SpanStrongSaleFrameStrongStatAddInfo:SetText( "�Ѿ��ģ����ۣ�"..myClanSaleMoney.."����" );
			else
				SpanStrongSaleFrameStrongStatAddInfo:SetText( "" );
			end
		else
			SpanStrongSaleFrameStrongStat:SetText( "�����ѿ�������" );
			SpanStrongSaleFrameStrongStatAddInfo:SetText( "ֻ�л᳤���ܲ��뾺��" );
			if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
				SetSpanStrongSaleInputHide( false );
			else
				SetSpanStrongSaleInputHide( true );
			end
		end
	elseif saleStat == STRONG_STAT_TONGJI_TOLUA then	
		SpanStrongSaleFrameStrongStat:SetText( "��������12��00���ṫ�����Ľ��" )
		SetSpanStrongSaleInputHide( true );
		if saleJoin then
			if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
				local myClanSaleMoney = GetClanJoinSpanSaleMoney( strongInfo.Sale );
				SpanStrongSaleFrameStrongStatAddInfo:SetText( "�Ѿ��ģ����ۣ�"..myClanSaleMoney.."����" );
			else
				SpanStrongSaleFrameStrongStatAddInfo:SetText( "" );
			end
		else
			SpanStrongSaleFrameStrongStatAddInfo:SetText( "" );
		end
	elseif saleStat == STRONG_STAT_SELECT_TOLUA then
		SetSpanStrongSaleInputHide( true );
		local winnerPrice = GetClanSpanSaleWinnerMoney( strongInfo.Sale );
		SpanStrongSaleFrameStrongStatAddInfo:SetText( "������ս��..." );
		SpanStrongSaleFrameStrongStat:SetText( "���Ļ�ʤ���ۣ�"..winnerPrice );
		if winnerPrice == 0 or winnerPrice == nil then
			SpanStrongSaleFrameStrongStat:SetText( "�ܿ�ϧ�޹��Ὰ��" );
		end
	elseif saleStat == STRONG_STAT_FIGHT_TOLUA then
		SetSpanStrongSaleInputHide( true );
		local winnerPrice = GetClanSpanSaleWinnerMoney( strongInfo.Sale );
		local winnerName = GetClanSpanSaleWinClanName( strongInfo.Sale );
		local ownerName = strongInfo.Owner.ClanName
		if winnerName == "" then
			winnerName = "��";
		else
			if (mainplayer:getClanName() == winnerName or mainplayer:getClanName() == ownerName) and mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
				--SpanStrongSaleFrameStrongStatAddInfo:SetText( "������ս���᣺"..winnerName );
			else
				SpanStrongSaleFrameStrongStatAddInfo:SetText( "������ս��..." );
			end
		end
		SpanStrongSaleFrameStrongStat:SetText( "���Ļ�ʤ���ۣ�"..winnerPrice );
	elseif saleStat == STRONG_STAT_IDLE_TOLUA then
		SetSpanStrongSaleInputHide( true );
		local weekday = getWeekDay();
		SpanStrongSaleFrameStrongStat:SetText( "�ݵ�ս�Ѿ�����" );
		SpanStrongSaleFrameStrongStatAddInfo:SetText( SpanBaseWarSaleTimeList[weekday] );
	end
end
function SpanStrongSaleFrameEnterBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	local nSpanStrongIdx = SpanStrongSaleFrame:GetClientUserData( 0 )
	local strongInfo = GameClan_SpanBaseWar:getSpanStrongInfoByIndex( nSpanStrongIdx )
	local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( nSpanStrongIdx )
	local moneystr = SpanStrongSaleFrameMoneyInput:GetText();
	local money = tonumber( moneystr );
	if money == nil then
		ShowMidTips( "������ݵ㾺���ʽ�" )
		return;
	end
	if mainplayer:isInClan() and ( mainplayer:getClanPosition() == CLAN_POS_PRESIDENT ) then
		local MyClanViewer = GameClanManager:getClanViewer();
		local MyClanInfo = MyClanViewer.m_clanInfo;
		if MyClanInfo.m_uClanId == strongInfo.Owner.ClanID then
			ShowMidTips("����ʧ�ܣ��޷�����ռ��ľݵ���о���");
			return;
		end

		if money < SpanStrongDef.BaseUse or money == nil then
			ShowMidTips( "�����ʽ���ڵ׼ۣ��޷�����" );
			return;
		end
		if mainplayer:getMoney() < money then
			ShowMidTips( "�����������㣬����ʧ��" );
			return;
		end
		GameClan_SpanBaseWar:SpanStrongSale( SpanStrongDef.StrongID, money );
		ClanStrongInfoFrame:Hide();
	else
		ShowMidTips("����ʧ�ܣ�ֻ�л᳤��Ȩ�޲��뾺��");
	end
end
--------���ս���������Ϣ
local MAX_SHOW_PLAYER_NUM = 12
local t_SpanStrongWarPlayer = {}
local selfSpanWarInfo = {}
local t_SpanStrongWarInfo = {
	[1] = [[#ca0a05a�������������ɻ��ս������:
#cff0000	1.�ܻ�ɱ�������ܺϳ���40
#cff0000	2.����������0/20
#cff0000	3.����ս����������20����]],
	[2] = [[#ca0a05aս��������ս������ʱ����Զ����
#cffffbe1.ʤ���������Ա��ÿ�˻��2222354����
#cffffbe2.ʧ�ܷ������Ա��ÿ�˻��2254534����]],
}
local t_SpanStrongWarInfoFrameRemindTexUV = {
	["finish"] = { 
	uv = { w = 24, h = 21, x = 576, y = 78,},
	},
	["unFinish"] = { 
	uv = { w = 25, h = 22, x = 604, y = 78, },
	 },	
}
local t_SpanStrongWarTitleRules = {
	"name", "clan", "race", "kill", "contineKill", "assists", "resGetNum",
}
local sortRule = "resGetNum"
local sortRequest = true
local SPAN_BASE_WAR_MIN_WIN_TIME = 20
function SpanStrongWarInfoFrame_OnLoad()
	this:RegisterEvent( "GE_SPAN_STRONG_WAR_INFO_UPDATE" )
	this:RegisterEvent( "GE_SPAN_STRONG_WAR_END" )
end
function SpanStrongWarInfoFrame_OnEvent()
	if arg1 == "GE_SPAN_STRONG_WAR_INFO_UPDATE" then
		GetSpanWarPlayerData()
		SorSpanStrongWarPlayersInfo( sortRequest, sortRule )
		SetSpanStrongWarInfo()
	elseif arg1 == "GE_SPAN_STRONG_WAR_END" then
		SpanStrongWarInfoFrame:Show()
		SpanStrongWarEnd()
	end
end
function GetSpanWarPlayerData()
	t_SpanStrongWarPlayer = {}
	local MainPlayer = ActorMgr:getMainPlayer()
	for i = 1, GameClan_SpanBaseWar:getSpanStrongPlayerNum() do
		local oneInfo = GameClan_SpanBaseWar:getSpanStrongPlayerInfo(i)
		local playerInfo = {
			name = oneInfo.RoleName,
			clan = oneInfo.ClanName,
			race = GetCareerName( oneInfo.Career ),
			kill = oneInfo.CountKill,
			contineKill = oneInfo.MaxContinueKill,
			assists = oneInfo.AssitNum,
			resGetNum = oneInfo.ResGet,
		}
		table.insert( t_SpanStrongWarPlayer, playerInfo )
		--��Ϊ��ͬʱ����������������������������Ƚ���
		local name = string.sub( oneInfo.RoleName, string.find( oneInfo.RoleName,"$",1,true )+1, string.len( oneInfo.RoleName ) )
		if MainPlayer:getName() == name then
			selfSpanWarInfo = {
				die	= oneInfo.Die,
				kill = oneInfo.CountKill,
				contineKill = oneInfo.MaxContinueKill,
				assists = oneInfo.AssitNum,
				resGetNum = oneInfo.ResGet,
				enterTime = oneInfo.EnterTime,
				strongID = oneInfo.MapID,
			}
		end
	end

	SpanStrongWarInfoFrameSelfInfokillNum:SetText("�ܻ�ɱ��"..selfSpanWarInfo["kill"])
	SpanStrongWarInfoFrameSelfInfoContineKill:SetText("�����ɱ��"..selfSpanWarInfo["contineKill"])
	SpanStrongWarInfoFrameSelfInfoAssist:SetText("������"..selfSpanWarInfo["assists"])
	SpanStrongWarInfoFrameSelfInfoResGetNum:SetText("��Դ�����������"..selfSpanWarInfo["resGetNum"])
	SpanStrongWarInfoFrameSelfInfoEnterTime:SetText("����ս��ʱ�䣺"..math.floor(selfSpanWarInfo["enterTime"]/60).."����")
	local Info = "#ca0a05a�������������ɻ��ս������:\n"
	if (selfSpanWarInfo["kill"] + selfSpanWarInfo["assists"]) < 40 then
		local uv = t_SpanStrongWarInfoFrameRemindTexUV["unFinish"]["uv"]
		SpanStrongWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#cff0000	1.�ܻ�ɱ�������ܺϳ���40\n"
	else
		local uv = t_SpanStrongWarInfoFrameRemindTexUV["finish"]["uv"]
		SpanStrongWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#c00ff00	1.�ܻ�ɱ�������ܺϳ���40\n"
	end
	if selfSpanWarInfo["die"] < 20 then
		local uv = t_SpanStrongWarInfoFrameRemindTexUV["unFinish"]["uv"]	
		SpanStrongWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#cff0000	2.����һ�ɱ"..selfSpanWarInfo["die"].."/20��\n"
	else
		local uv = t_SpanStrongWarInfoFrameRemindTexUV["finish"]["uv"]
		SpanStrongWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#c00ff00	2.����һ�ɱ20��\n"
	end
	if math.floor(selfSpanWarInfo["enterTime"]/60) < SPAN_BASE_WAR_MIN_WIN_TIME then
		local uv = t_SpanStrongWarInfoFrameRemindTexUV["unFinish"]["uv"]	
		SpanStrongWarInfoFrameRemindInfoNeed3:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#cff0000	3.����ս����������20����"
	else
		local uv = t_SpanStrongWarInfoFrameRemindTexUV["finish"]["uv"]
		SpanStrongWarInfoFrameRemindInfoNeed3:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#c00ff00	3.����ս����������20����"
	end
	SpanStrongWarInfoFrameRemindInfoRemind1:SetText( Info, 255, 255, 0 );
end
function SorSpanStrongWarPlayersInfo( sortRequest, sortRule )
	if sortRequest then
		table.sort( t_SpanStrongWarPlayer, function( a, b )
			return a[sortRule] > b[sortRule]
		end)
	else
		table.sort( t_SpanStrongWarPlayer, function( a, b )
			return a[sortRule] < b[sortRule]
		end)
	end
end
function AnalyzePlayerName( name )
	local svrID =  tonumber(string.sub(name, string.find(name, "%d+")))
	local svrName = LoginMgr:getWorldNameById(svrID)
	local playerName = string.sub( name, string.find(name,"$",1,true )+1, string.len( name ) )
	return string.format( "[%s]%s", svrName, playerName )
end
function SetSpanStrongWarInfo()
	--���֮ǰ��ʾ��
	for i = 1, MAX_SHOW_PLAYER_NUM do
		local btn = getglobal( "SpanStrongWarInfoFramePlayersInfoPlayerInfo"..i )
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( "" )
		local clanName = getglobal( btn:GetName().."Clan" )
		clanName:SetText( "" )
		local RaceName = getglobal( btn:GetName().."Race" )
		RaceName:SetText( "" )
		local killNumFont = getglobal( btn:GetName().."killNum" )
		killNumFont:SetText( "" )
		local ContineKillFont = getglobal( btn:GetName().."ContineKill" )
		ContineKillFont:SetText( "" )
		local AssistFont = getglobal( btn:GetName().."Assist" )
		AssistFont:SetText( "" )
		local GetResNumFont = getglobal( btn:GetName().."GetResNum" )
		GetResNumFont:SetText( "" )
	end
	if table.getn( t_SpanStrongWarPlayer ) > MAX_SHOW_PLAYER_NUM then
		SpanStrongWarInfoFramePlayersInfoScrollBar:Show()
		SpanStrongWarInfoFramePlayersInfoScrollBar:SetMaxValue( table.getn( t_SpanStrongWarPlayer ) - MAX_SHOW_PLAYER_NUM )
	else
		SpanStrongWarInfoFramePlayersInfoScrollBar:Hide()
		SpanStrongWarInfoFramePlayersInfoScrollBar:SetMaxValue(0);
	end
	SpanStrongWarInfoFramePlayersInfoScrollBar:SetValue(math.min(SpanStrongWarInfoFramePlayersInfoScrollBar:GetMaxValue(), SpanStrongWarInfoFramePlayersInfoScrollBar:GetValue()))
	local endIndex = math.min( table.getn( t_SpanStrongWarPlayer ), MAX_SHOW_PLAYER_NUM + SpanStrongWarInfoFramePlayersInfoScrollBar:GetValue() )
	for i = 1 + SpanStrongWarInfoFramePlayersInfoScrollBar:GetValue(), endIndex do
		local color;
		local mainplayer = ActorMgr:getMainPlayer()
		local name = string.sub( t_SpanStrongWarPlayer[i]["name"], string.find( t_SpanStrongWarPlayer[i]["name"],"$",1,true )+1, string.len( t_SpanStrongWarPlayer[i]["name"] ) )
		if mainplayer:getName() == name then
			color = { 0, 255, 0, }
		else 
			color = { 190, 190, 90, }
		end
		local btn = getglobal( "SpanStrongWarInfoFramePlayersInfoPlayerInfo"..(i - SpanStrongWarInfoFramePlayersInfoScrollBar:GetValue()) )
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( AnalyzePlayerName( t_SpanStrongWarPlayer[i]["name"] ) )
		NameFont:SetTextColor( color[1], color[2], color[3] )
		local clanName = getglobal( btn:GetName().."Clan" )
		clanName:SetText( t_SpanStrongWarPlayer[i]["clan"] )
		clanName:SetTextColor( color[1], color[2], color[3] )
		local RaceName = getglobal( btn:GetName().."Race" )
		RaceName:SetText( t_SpanStrongWarPlayer[i]["race"] )
		RaceName:SetTextColor( color[1], color[2], color[3] )
		local killNumFont = getglobal( btn:GetName().."killNum" )
		killNumFont:SetText( t_SpanStrongWarPlayer[i]["kill"] )
		killNumFont:SetTextColor( color[1], color[2], color[3] )
		local ContineKillFont = getglobal( btn:GetName().."ContineKill" )
		ContineKillFont:SetText( t_SpanStrongWarPlayer[i]["contineKill"] )
		ContineKillFont:SetTextColor( color[1], color[2], color[3] )
		local AssistFont = getglobal( btn:GetName().."Assist" )
		AssistFont:SetText( t_SpanStrongWarPlayer[i]["assists"] )
		AssistFont:SetTextColor( color[1], color[2], color[3] )
		local GetResNumFont = getglobal( btn:GetName().."GetResNum" )
		GetResNumFont:SetText( t_SpanStrongWarPlayer[i]["resGetNum"] )		
		GetResNumFont:SetTextColor( color[1], color[2], color[3] )
	end
end
function SpanStrongWarInfoFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	SpanStrongWarInfoFrameWinInfoResult:SetText( "" )
	SpanStrongWarInfoFrameWinInfoGetMoney:SetText( "" )
	SpanStrongWarInfoFrameWinInfoGetExp:SetText( "" )
	SpanStrongWarInfoFrameWinInfoGetFont:SetText( "" )
	SpanStrongWarInfoFrameWinInfoGetGift:SetText( "" )
	SpanStrongWarInfoFramePlayersInfoScrollBar:SetValue(0);
	SpanStrongWarInfoFrameRemindInfoRemind1:SetText( t_SpanStrongWarInfo[1], 255, 255, 0 );
	SpanStrongWarInfoFrameRemindInfoRemind2:SetText( "", 255, 255, 0 );
	local uv = t_SpanStrongWarInfoFrameRemindTexUV["unFinish"]["uv"]
	SpanStrongWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )	
	SpanStrongWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	GetSpanWarPlayerData()
	SorSpanStrongWarPlayersInfo( sortRequest, sortRule )
	SetSpanStrongWarInfo();
	local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( selfSpanWarInfo["strongID"] );
	local winnerMoneyType = SpanStrongDef.WinAward.Money[0].Type;
	local winnerMoney = SpanStrongDef.WinAward.Money[0].Num;
	local failMoneyType = SpanStrongDef.FailAward.Money[0].Type;
	local failMoney = SpanStrongDef.FailAward.Money[0].Num;
	local winnerItemDef = getItemDef( SpanStrongDef.WinAward.Item[0].ItemID )
	local failItemDef = getItemDef( SpanStrongDef.FailAward.Item[0].ItemID )
	local szAward = "#ca0a05aս��������ս������ʱ����Զ����\n#cffffbe1.ʤ���������Ա��ÿ�˻��%d%s��\n\t%s����������\n#cffffbe2.ʧ�ܷ������Ա��ÿ�˻��%d%s��\n\t%s����������"	
	SpanStrongWarInfoFrameRemindInfoRemind2:SetText( string.format( szAward, winnerMoney, GetMoneyDesc(winnerMoneyType), winnerItemDef.Name, failMoney, GetMoneyDesc(failMoneyType), failItemDef.Name ), 255, 255, 0 );
end
function SpanStrongWarInfoFramePlayersInfo_ScrollUpBtn()
	local slider = SpanStrongWarInfoFramePlayersInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
	SetSpanStrongWarInfo()
end
function SpanStrongWarInfoFramePlayersInfo_ScrollDownBtn()
	local slider	= SpanStrongWarInfoFramePlayersInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
	SetSpanStrongWarInfo()
end
function SpanStrongWarInfoFramePlayersInfo_ScrollBarValueChange()
	SetSpanStrongWarInfo();
end
function SpanStrongWarItemTitle_OnClick()
	local titleIndex = this:GetClientID();	
	if t_SpanStrongWarTitleRules[titleIndex] == sortRule then
		sortRule = t_SpanStrongWarTitleRules[titleIndex]
		sortRequest = not sortRequest
	else
		sortRule = t_SpanStrongWarTitleRules[titleIndex]
		sortRequest = true
	end	
	GetSpanWarPlayerData()
	SorSpanStrongWarPlayersInfo( sortRequest, sortRule )
	SetSpanStrongWarInfo()
end

function SetSpanStrongWarResult( nWinFlag )
	if not SpanStrongWarInfoFrame:IsShown() then
		SpanStrongWarInfoFrame:Show();
	end
	--��ȡ��ҽ��ս���ľݵ�mapID
	local mainplayer = ActorMgr:getMainPlayer()	
	local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( selfSpanWarInfo["strongID"] );
	local strongInfo = GameClan_SpanBaseWar:getSpanStrongInfoByIndex( selfSpanWarInfo["strongID"] )
	if nWinFlag == 1 then
		SpanStrongWarInfoFrameWinInfoResult:SetText( " ʤ��" )
		SpanStrongWarInfoFrameWinInfoGetFont:SetText( "�ѻ�����½���" )
		local winnerMoneyType = SpanStrongDef.WinAward.Money[0].Type;
		local winnerMoney = SpanStrongDef.WinAward.Money[0].Num;
		SpanStrongWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 200, 0 )	
		SpanStrongWarInfoFrameWinInfoGetMoney:SetText(string.format( "���ң�%d%s", winnerMoney, GetMoneyDesc(winnerMoneyType) ))
		local winnerExp = SpanStrongDef.WinAward.Exp;
		local mainplayer = ActorMgr:getMainPlayer()
		local levelupDef = getLevelUP( mainplayer:getLv() )
		local getExp = math.floor( levelupDef.ContributeExp * ( 1.0 * winnerExp / 10000 ) )
		SpanStrongWarInfoFrameWinInfoGetExp:SetTextColor( 255, 200, 0 )	
		SpanStrongWarInfoFrameWinInfoGetExp:SetText(string.format( "���飺%d", getExp))
		local winnerItemDef = getItemDef( SpanStrongDef.WinAward.Item[0].ItemID )
		SpanStrongWarInfoFrameWinInfoGetGift:SetText( string.format( "%s(�ʼ�����)", winnerItemDef.Name ) )
	else
		SpanStrongWarInfoFrameWinInfoResult:SetText( " ʧ��" )
		SpanStrongWarInfoFrameWinInfoGetFont:SetText( "�ѻ�����½���" )
		local failMoneyType = SpanStrongDef.FailAward.Money[0].Type;
		local failMoney = SpanStrongDef.FailAward.Money[0].Num;
		SpanStrongWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 200, 0 )
		SpanStrongWarInfoFrameWinInfoGetMoney:SetText( string.format( "���ң�%d%s", failMoney, GetMoneyDesc( failMoneyType ) ) )
		local failExp = SpanStrongDef.FailAward.Exp;
		local mainplayer = ActorMgr:getMainPlayer()
		local levelupDef = getLevelUP( mainplayer:getLv() )
		local getExp = math.floor( levelupDef.ContributeExp * ( 1.0 * failExp / 10000 ) )
		SpanStrongWarInfoFrameWinInfoGetExp:SetTextColor( 255, 200, 0 )	
		SpanStrongWarInfoFrameWinInfoGetExp:SetText(string.format( "���飺%d", getExp))
		local failItemDef = getItemDef( SpanStrongDef.FailAward.Item[0].ItemID )
		SpanStrongWarInfoFrameWinInfoGetGift:SetText( string.format( "%s(�ʼ�����)", failItemDef.Name ) )
	end
	--û�����������������ʾ�Ľ�Ǯ����������ʾ
	if selfSpanWarInfo["die"] < 20 and ( selfSpanWarInfo["kill"] + selfSpanWarInfo["assists"] ) < 40 and selfSpanWarInfo["enterTime"] < SPAN_BASE_WAR_MIN_WIN_TIME then
		SpanStrongWarInfoFrameWinInfoGetMoney:SetText( "δ�����ý�������" )
		SpanStrongWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 0, 0 )
		SpanStrongWarInfoFrameWinInfoGetFont:SetText( "" )
		SpanStrongWarInfoFrameWinInfoGetExp:SetText( "" )
		SpanStrongWarInfoFrameWinInfoGetGift:SetText( "" )
	end
end
function SpanStrongWarInfoFrameLeave_OnClick()
	GameClan_BaseWar:LeaveStrongWar();
	SpanStrongWarInfoFrame:Hide();
end
--------����ݵ�ս�·�Ư����ʾ
function SpanStrongWarBegin()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPAN_STRONG_WAR ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_SPAN_STRONG_WAR, frame = "SpanStrongWarInfoFrame", 
												path ="uires\\ui\\Za\\ZhanZhengXinXi.tga", index = 0 } );
	end
	--��ʾ��ɱ���
	StrongWarContineKillFrame:Show()
	StrongWarContineKillFrame:SetClientUserData( 1, SPAN_STRONG_CONTINE_KILL_TYPE )
end
function SpanStrongWarEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPAN_STRONG_WAR );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	--������ɱ���
	StrongWarContineKillFrame:Hide()
end
--------����ݵ�ս��Դ���
function LoadSpanStrongPworldID()
	GameClan_SpanBaseWar:loadSpanStrongPworldID()
end
function ClanSpanStrongWarResFrame_OnLoad()
	LoadSpanStrongPworldID()
	this:RegisterEvent( "GE_ENTER_MAP" )
end
function ClanSpanStrongWarResFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then		
		if MapShower:getRealMapId() >= MIN_COPY_ID then
			if GameClan_SpanBaseWar:isInSpanStrongPworld() then
				SpanStrongWarBegin();
				ClanSpanStrongWarResFrame:Show()
			end
		else
			--����·�����ݵ�սƯ��ͼ��
			local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
			local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPAN_STRONG_WAR );
			if nIdx > 0 then
				t_centerXinShouBtnControl:eraseByIndex( nIdx );
				t_centerXinShouBtnControl:updateCenterBtns();
			end
			StrongWarContineKillFrame:Hide()
			ClanSpanStrongWarResFrame:Hide()
		end
	end
end
function ClanSpanStrongWarResFrame_OnUpdate()
	local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( selfSpanWarInfo["strongID"] )
	local strongPworldInfo = GameClan_SpanBaseWar:getSpanStrongWarPworldInfo()
	local resLimit = SpanStrongDef.ResLimit;
	local atkResNum = strongPworldInfo.AtkRes;
	local defResNum = strongPworldInfo.DefRes;
	if atkResNum > resLimit or defResNum > resLimit then
		atkResNum = 0;
		defResNum = 0;
	end
	local atkVal = ( atkResNum / resLimit ) * 141 * GetScreenScaleY();
	local defVal = ( defResNum / resLimit ) * 141 * GetScreenScaleY();
	ClanSpanStrongWarResFrameGongResBtnValFont:SetText( atkResNum.."/"..resLimit );
	ClanSpanStrongWarResFrameFangResBtnValFont:SetText( defResNum.."/"..resLimit )
	ClanSpanStrongWarResFrameGongResBtnGongValTex:SetSize( math.floor( atkVal ), 9);
	ClanSpanStrongWarResFrameFangResBtnFangValTex:SetSize( math.floor( defVal ), 9 );
	UpdateSpanBaseWarResShowOnMap();
end
local t_SpanImportantMapResPath = {
{	
	[1] = "uires\\DiTuLei\\MinimapIcon\\261202.tga",
	[2] = "uires\\DiTuLei\\MinimapIcon\\261201.tga",
	[3] = "uires\\DiTuLei\\MinimapIcon\\261200.tga",
},
{	
	[1] = "uires\\DiTuLei\\MinimapIcon\\261211.tga",
	[2] = "uires\\DiTuLei\\MinimapIcon\\261210.tga",
	[3] = "uires\\DiTuLei\\MinimapIcon\\261209.tga",
},
}
function UpdateSpanBaseWarResShowOnMap()
	ClearBaseWarMapIcon();
	if not GameClan_SpanBaseWar:isInSpanStrongPworld() then return end
	local strongPworldInfo = GameClan_SpanBaseWar:getSpanStrongWarPworldInfo()
	for i = 1, strongPworldInfo.MonNum do
		--/* 1-�ط� 2-����*/
		local iConIndex = 1;
		if strongPworldInfo.Mon[i-1].Camp == 1 then
			iConIndex = 3;
		elseif strongPworldInfo.Mon[i-1].Camp == 2 then
			iConIndex = 2;
		end
		local indexType = ( strongPworldInfo.Mon[i-1].Flag == 1 and 2 or 1 );
		local id = strongPworldInfo.Mon[i-1].MemID;
		AddImportMinimapIcon( id, t_SpanImportantMapResPath[indexType][iConIndex] );
		local MaxMapControl	= MapShower:GetMaxMapContal();
		AddBaseWarMapIcon(id, MaxMapControl:getMapID(), t_SpanImportantMapResPath[indexType][iConIndex], 10*strongPworldInfo.Mon[i-1].X , 10*strongPworldInfo.Mon[i-1].Y );
	end
end
function SetSpanStrongWarAwardInfo( nExp, nBindGold, nBull )
	local szInfo = "ռ�������"
	if nExp > 0 then
		szInfo = szInfo .. "\n\t��60��������Ҵ�־�������"..nExp.."%"
	end
	if nBindGold > 0 then
		szInfo = szInfo .. "\n\t��ÿСʱ��10��ȯ��ÿ�����"..nBindGold.."��ȯ"
	end
	if nBull > 0 then
		szInfo = szInfo .. "\n\t��ÿ���Ӷ�����100������ÿ�������"..nBull.."����"
	end
	SpanStrongWarAwardFrameInfo:SetText( szInfo, 255,255,190 )
end
function GetSpanStrongAward( SpanStrongDef )
	local exp,bindGold,bull = 0,0,0;
	local MAX_AWARD_NUM = 3
	for i = 1, MAX_AWARD_NUM do
		if SpanStrongDef.WorldAward[i-1].Type == 1 then
			exp = SpanStrongDef.WorldAward[i-1].Num
		elseif SpanStrongDef.WorldAward[i-1].Type == 2 then
			bull = SpanStrongDef.WorldAward[i-1].Num
		elseif SpanStrongDef.WorldAward[i-1].Type == 3 then
			bindGold = SpanStrongDef.WorldAward[i-1].Num
		end
	end
	return exp,bindGold,bull
end
function SpanStrongWarAwardFrame_OnShow()
	local strongID = this:GetClientUserData( 0 )
	local SpanStrongDef = GameClan_SpanBaseWar:getSpanStrongDef( strongID )
	SpanStrongWarAwardFrameTitleFont:SetText( SpanStrongDef.Name )
	local nExp, nBindGold, nBull = GetSpanStrongAward( SpanStrongDef )
	SetSpanStrongWarAwardInfo( nExp, nBindGold, nBull )
end
