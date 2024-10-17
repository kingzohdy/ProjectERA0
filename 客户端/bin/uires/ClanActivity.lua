local ClanStrongInfo = {
	[1] = [[#cffffbe�ݵ�ս��飺
#ca0a05a��ս��ʱ��Ϊ�������Ľ������20��30-21��30��
#ca0a05a��ս��Ϊ��Դս��ս����ʼ��˫��ͨ��[�ݵ�������]ɳ�˽���ս��������չ����Դ�����ᣬ����ռ����Դ����������Դ������ս��ʱ���ڣ����ȴﵽ��Դ���޵Ĺ���������ʤ��������ս��ʱ�����ʱ����δ�ֳ�ʤ����������Դ�������Ĺ����ʤ�����˫��������Դ������ͬ����ݵ�ռ�칫���ʤ]],
	[2] = [[#cffffbe����ʱ�䣺
#ca0a05aÿ��һ����00��00-24��00
#cffffbe���Ĺ���
#ca0a05a����ʹ�ù����ʽ��԰�����ʽ���С�
#ca0a05a�ڶ�������12��00�������Ľ����
#ca0a05a������߹���Ϊ����ʤ���ߡ�����ʤ��������ڽ����������20��30�μӾݵ�ս��
#cff7d00  ע����������Ĳ��ɹ�����ǰ�����ʽ�ϵͳ���Զ�������
#cff7d00  ���ĳɹ��������Ĺ�������ս���ʸ�ϵͳ��۳������ʽ�]],
}
MAX_BASE_WAR_RES_MON = 4
local ClanBaseWarSaleTimeList = {
	--[1] = "�´ξ���ʱ��Ϊ��һ00��00-24��00",
	[2] = "�´ξ���ʱ��Ϊ����00��00-24��00",
	--[3] = "�´ξ���ʱ��Ϊ��һ00��00-24��00",
	[4] = "�´ξ���ʱ��Ϊ��һ00��00-24��00",
	[5] = "�´ξ���ʱ��Ϊ��һ00��00-24��00",
	[6] = "�´ξ���ʱ��Ϊ��һ00��00-24��00",
	[7] = "�´ξ���ʱ��Ϊ��һ00��00-24��00",
}
local t_ImportantMapResPath = {
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
function ClanStrongInfoFrame_OnShow()
	SetClanStrongInfo();
	ClanStrongInfoFrameInfo:SetText( ClanStrongInfo[1], 255,255,255 );
	ClanStrongInfoFrameAttention:SetText( ClanStrongInfo[2], 255,255,255 );
end
function SetStrongSaleInputHide( bHide )
	if bHide then
		ClanStrongInfoFrameMoneyInput:Hide();
		ClanStrongInfoFrameEnterBtn:Hide();
		ClanStrongInfoFrameEditBoxBkg:Hide();
		ClanStrongInfoFrameInputInfo:Hide();
		ClanStrongInfoFrameStrongStatAddInfo:Show();
	else
		ClanStrongInfoFrameMoneyInput:Show();
		ClanStrongInfoFrameEnterBtn:Show();
		ClanStrongInfoFrameEditBoxBkg:Show();
		ClanStrongInfoFrameInputInfo:Show();
		ClanStrongInfoFrameStrongStatAddInfo:Hide();
	end
end
function SetClanStrongInfo()
	--���þݵ���Ϣ
	local strongName = GameClan_BaseWar:getStrongPointName();
	ClanStrongInfoFrameStrongName:SetText( strongName );
	local strongLv = GameClan_BaseWar:getStrongPointLV();
	ClanStrongInfoFrameStrongLV:SetText( strongLv );
	local baseUse = GameClan_BaseWar:getStrongPointBaseUse();
	ClanStrongInfoFrameStrongBaseVal:SetText( baseUse );
	local strongOwnerName = GameClan_BaseWar:getStrongPointOwnerName();
	if strongOwnerName == "" or strongOwnerName == nil then
		ClanStrongInfoFrameStrongOwner:SetText( "����ռ��" );
	else
		ClanStrongInfoFrameStrongOwner:SetText( strongOwnerName );
	end
	local saleBase = GameClan_BaseWar:getStrongPointBaseUse();
	ClanStrongInfoFrameMoneyInput:SetText( saleBase );
	--����״̬����
	local saleStat = GameClan_BaseWar:getStrongPointStat();
	local saleJoin = GameClan_BaseWar:IsJoinStrongSale();
	local mainplayer = ActorMgr:getMainPlayer();
	if saleStat == STRONG_STAT_SALE_TOLUA then		
		if saleJoin then
			SetStrongSaleInputHide( true );
			ClanStrongInfoFrameStrongStat:SetText( "�����ѿ�������" );
			if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT then
				local myClanSaleMoney = GameClan_BaseWar:getMyClanSaleMoney();
				ClanStrongInfoFrameStrongStatAddInfo:SetText( "�Ѿ��ģ����ۣ�"..myClanSaleMoney.."�����ʽ�" );
			else
				ClanStrongInfoFrameStrongStatAddInfo:SetText( "" );
			end
		else
			ClanStrongInfoFrameStrongStat:SetText( "�����ѿ�������" );
			if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT then
				SetStrongSaleInputHide( false );
			else
				SetStrongSaleInputHide( true );
			end
		end
	elseif saleStat == STRONG_STAT_TONGJI_TOLUA then	
		ClanStrongInfoFrameStrongStat:SetText( "��������12��00���ṫ�����Ľ��" )
		SetStrongSaleInputHide( true );
		if saleJoin then
			if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT then
				local myClanSaleMoney = GameClan_BaseWar:getMyClanSaleMoney();
				ClanStrongInfoFrameStrongStatAddInfo:SetText( "�Ѿ��ģ����ۣ�"..myClanSaleMoney.."�����ʽ�" );
			else
				ClanStrongInfoFrameStrongStatAddInfo:SetText( "" );
			end
		else
			ClanStrongInfoFrameStrongStatAddInfo:SetText( "" );
		end
	elseif saleStat == STRONG_STAT_SELECT_TOLUA or  saleStat == STRONG_STAT_FIGHT_TOLUA then
		SetStrongSaleInputHide( true );
		local winnerPrice = GameClan_BaseWar:getSaleWinerPrice();
		local winnerName = GameClan_BaseWar:getSaleWinerName();
		if winnerName == "" or winnerName == nil then
			winnerName = "��";
		else
			if (mainplayer:getClanName() == winnerName and mainplayer:getClanPosition() ~= CLAN_POS_NORMAL) and saleStat == STRONG_STAT_SELECT_TOLUA then
				ClanStrongInfoFrameStrongStatAddInfo:SetText( "������ս���᣺"..winnerName );
			else
				ClanStrongInfoFrameStrongStatAddInfo:SetText( "������ս��..." );
			end
		end
		ClanStrongInfoFrameStrongStat:SetText( "���Ļ�ʤ���ۣ�"..winnerPrice );
		if winnerPrice == 0 or winnerPrice == nil then
			ClanStrongInfoFrameStrongStat:SetText( "�ܿ�ϧ�޹��Ὰ��" );
		end
	elseif saleStat == STRONG_STAT_IDLE_TOLUA then
		SetStrongSaleInputHide( true );
		local weekday = getWeekDay();
		ClanStrongInfoFrameStrongStat:SetText( "�ݵ�ս�Ѿ�����" );
		ClanStrongInfoFrameStrongStatAddInfo:SetText( ClanBaseWarSaleTimeList[weekday] );
	end
end
--�յ�������Ϣ����ʾ������Ϣ���
function OpenClanBaseWarSale( npcId )
	if npcId == 0 or npcId == nil then return end
	ClanStrongInfoFrame:Show();
end
function ClanStrongInfoFrameEnterBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	local moneystr = ClanStrongInfoFrameMoneyInput:GetText();
	local money = tonumber( moneystr );
	if money == nil then
		ShowMidTips( "������ݵ㾺���ʽ�" )
		return;
	end
	if mainplayer:isInClan() and ( mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT ) then
		local MyClanViewer = GameClanManager:getClanViewer();
		local MyClanInfo = MyClanViewer.m_clanInfo;
		if MyClanInfo.m_szClanName == GameClan_BaseWar:getStrongPointOwnerName() then
			ShowMidTips("����ʧ�ܣ��޷�����ռ��ľݵ���о���");
			return;
		end

		if money < GameClan_BaseWar:getStrongPointBaseUse() or money == nil then
			ShowMidTips( "�����ʽ���ڵ׼ۣ��޷�����" );
			return;
		end
		if MyClanInfo.m_nMoney < money then
			ShowMidTips( "�����ʽ��㣬����ʧ��" );
			return;
		end
		GameClan_BaseWar:EnterStrongSale( money );
		ClanStrongInfoFrame:Hide();
	else
		ShowMidTips("����ʧ�ܣ�ֻ�л᳤�򸱻᳤��Ȩ�޲��뾺��");
	end
end
--------------������Դ��Ϣ���---------------
function ClanStrongWarResFrame_OnUpdate()
	local resLimit = GameClan_BaseWar:getStrongPworldResLimit();
	local atkResNum = GameClan_BaseWar:getPworldAtkResNum();
	local defResNum = GameClan_BaseWar:getPworldDefResNum();
	if atkResNum > resLimit or defResNum > resLimit then
		atkResNum = 0;
		defResNum = 0;
	end
	local atkVal = ( atkResNum / resLimit ) * 141 * GetScreenScaleY();
	local defVal = ( defResNum / resLimit ) * 141 * GetScreenScaleY();
	ClanStrongWarResFrameGongResBtnValFont:SetText( atkResNum.."/"..resLimit );
	ClanStrongWarResFrameFangResBtnValFont:SetText( defResNum.."/"..resLimit )
	ClanStrongWarResFrameGongResBtnGongValTex:SetSize( math.floor( atkVal ), 9);
	ClanStrongWarResFrameFangResBtnFangValTex:SetSize( math.floor( defVal ), 9 );
	UpdateBaseWarResShowOnMap();
end
function UpdateBaseWarResShowOnMap()
	if not GameClan_BaseWar:IsInBaseWarPworld() then return end
	ClearBaseWarMapIcon();
	for i = 1, MAX_BASE_WAR_RES_MON do
		local clanName = GameClanManager:clanGetClanNameByWid(GameClan_BaseWar:getBaseWarResMonClanWID( i - 1 ));
		local iConIndex = 1;
		if clanName == GameClanManager:clanGetClanNameByWid(GameClan_BaseWar:getAtkClanWID()) then
			iConIndex = 2;
		elseif clanName == GameClanManager:clanGetClanNameByWid(GameClan_BaseWar:getDefClanWID()) then
			iConIndex = 3;
		end
		if clanName == nil or clanName == "" then
			iConIndex = 1;
		end
		local resMonInfo = GameClan_BaseWar:getBaseWarResMonInfo( i - 1 );
		local indexType = ( resMonInfo.Flag == 1 and 2 or 1 );
		local id = GameClan_BaseWar:getBaseWarResMonMemID( i - 1 );
		AddImportMinimapIcon( id, t_ImportantMapResPath[indexType][iConIndex] );
		local MaxMapControl	= MapShower:GetMaxMapContal();
		AddBaseWarMapIcon(id, MaxMapControl:getMapID(), t_ImportantMapResPath[indexType][iConIndex], 10*resMonInfo.X , 10*resMonInfo.Y );
	end
end
---------------�����Զ�������-------------------
local MAX_BANNER_TYPES_NUM = 3;
local MAX_BANNER_SHOW_NUM = 12;
local CUR_SELECT_TYPE = 1;
local CUR_SELECT_SHAPE_LEVEL = 1;
local CUR_SELECT_BACK_LEVEL = 1;
local CUR_SELECT_SIGN_LEVEL = 1;
local CUR_SELECT_SHAPE = 1;
local CUR_SELECT_BACK = 1;
local CUR_SELECT_SIGN = 1;
local t_BannerLevel = { "Ĭ��", "һ��", "����", "����", }
local t_BannerType = { "��״", "����", "���", }
local t_BannerDefList ={};
function LoadBannerDef()
	t_BannerDefList ={};
	for i = 1, MAX_BANNER_TYPES_NUM*MAX_BANNER_SHOW_NUM do
		local bannerDef = GameClan_BaseWar:getBannerDef( i - 1 );
		local typeIndex = bannerDef.Type;
		if t_BannerDefList[typeIndex] == nil then
			table.insert( t_BannerDefList, {} );
		end
		local levelIndex = math.floor(bannerDef.SubType/100%10) + 1;
		if t_BannerDefList[typeIndex][levelIndex] == nil then
			table.insert( t_BannerDefList[typeIndex], {} );
		end
		local t_BannerInfo = { TypeIndex = typeIndex, LevelIndex = levelIndex, IconID = bannerDef.IconID, DefIndex = i, subType = bannerDef.SubType };
		table.insert( t_BannerDefList[typeIndex][levelIndex], t_BannerInfo );		
	end
end
function SetBannerInfo()
	for i = 1, MAX_BANNER_SHOW_NUM do
		local btnTex = getglobal( "BaseWarBannerFrameTexButton"..i.."BackTexture" );
		btnTex:Show();
	end
	local maxBannerNum = table.getn( t_BannerDefList[CUR_SELECT_TYPE] );
	for i = 1, MAX_BANNER_TYPES_NUM  do
		local btn = getglobal( "BaseWarBannerFrameBannerType"..i )
		if i <= CUR_SELECT_TYPE then
			btn:SetPoint( "topleft", "BaseWarBannerFrame", "topleft", 11, 41+26*(i-1) )
		else
			btn:SetPoint( "topleft", "BaseWarBannerFrame", "topleft", 11, 41+26*(i-1)+(maxBannerNum)*22 )
		end
	end
	for i = 1, maxBannerNum do
		local btn = getglobal( "BaseWarBannerFrameBannerSelectButton"..i )
		btn:SetPoint( "topleft", "BaseWarBannerFrame", "topleft", 12, 41+26*CUR_SELECT_TYPE+(i-1)*22 );
		btn:Show();
		local szBtnName = t_BannerLevel[i]..t_BannerType[CUR_SELECT_TYPE].."�Ƽ�";
		if i == 1 then
			szBtnName = t_BannerLevel[i]..t_BannerType[CUR_SELECT_TYPE];
		end
		local btnText = getglobal( btn:GetName().."Text" );
		btnText:SetText( szBtnName );
	end
	BaseWarBannerFrameView:Show();
	BaseWarBannerFrameView:setPetModel( 0, 0, "character\\monster\\261217\\male.omod" );
	for i = 1, MAX_BANNER_TYPES_NUM do
		local szPath = "character\\monster\\261217\\qizi_1.dds"
		if i == 1 then
			szPath = "character\\monster\\261217\\qizi_%d.dds";
			for m = 1, table.getn( t_BannerDefList[i][CUR_SELECT_SHAPE_LEVEL] ) do
				if m == CUR_SELECT_SHAPE then
					local iconIndex = (t_BannerDefList[i][CUR_SELECT_SHAPE_LEVEL][m].DefIndex % 12 == 0 and 12 or t_BannerDefList[i][CUR_SELECT_SHAPE_LEVEL][m].DefIndex % 12 );
					szPath = string.format( szPath, iconIndex );
				end
			end
		elseif i == 2 then
			szPath = "character\\monster\\261217\\diwen_%d.dds";
			for m = 1, table.getn( t_BannerDefList[i][CUR_SELECT_BACK_LEVEL] ) do
				if m == CUR_SELECT_BACK then
					local iconIndex = (t_BannerDefList[i][CUR_SELECT_BACK_LEVEL][m].DefIndex % 12 == 0 and 12 or t_BannerDefList[i][CUR_SELECT_BACK_LEVEL][m].DefIndex % 12 );
					szPath = string.format( szPath, iconIndex );
				end
			end
		elseif i == 3 then
			szPath = "character\\monster\\261217\\wen_%d.dds";
			for m = 1, table.getn( t_BannerDefList[i][CUR_SELECT_SIGN_LEVEL] ) do
				if m == CUR_SELECT_SIGN then
					local iconIndex = (t_BannerDefList[i][CUR_SELECT_SIGN_LEVEL][m].DefIndex % 12 == 0 and 12 or t_BannerDefList[i][CUR_SELECT_SIGN_LEVEL][m].DefIndex % 12 );
					szPath = string.format( szPath, iconIndex );
				end
			end
		end
		--������Ϊ�߻������˳���������Դ��˳��һ����Ҫ����������
		local realTextureIdx = 3;
		if i == 1 then
			realTextureIdx = 2;
		elseif i == 2 then
			realTextureIdx = 1;
		end
		BaseWarBannerFrameView:setEntieyTextureByID( 0, realTextureIdx, szPath );
	end
	BaseWarBannerFrameView:setPetScale( 0, 1.27, 1.27, 1.27);
	BaseWarBannerFrameView:setEntityPosition(0,0,0,0);
	for i = 1, MAX_BANNER_SHOW_NUM do
		local selectIndex = 1;
		local selectIconIdex = 1;
		if CUR_SELECT_TYPE == 2 then
			selectIndex = CUR_SELECT_BACK_LEVEL;
			selectIconIdex = CUR_SELECT_BACK;
		elseif CUR_SELECT_TYPE == 1 then
			selectIndex = CUR_SELECT_SHAPE_LEVEL;
			selectIconIdex = CUR_SELECT_SHAPE;
		elseif CUR_SELECT_TYPE == 3 then
			selectIndex = CUR_SELECT_SIGN_LEVEL;
			selectIconIdex = CUR_SELECT_SIGN;
		end
		local btn = getglobal( "BaseWarBannerFrameBannerSelectButton"..i );
		local btnIcon = getglobal( "BaseWarBannerFrameTexButton"..i );
		btn:DisChecked();
		btnIcon:DisChecked();
		if selectIndex == i then
			btn:Checked();
		end
		if selectIconIdex == i then
			btnIcon:Checked();
		end
	end
	--����Сͼ��
	local curSelectLevel = 1;
	local clanAdv = GameClanManager:getAdvanceInfo();
	local curTechLevel = 0;
	if CUR_SELECT_TYPE == 1 then
		curSelectLevel = CUR_SELECT_SHAPE_LEVEL;
		curTechLevel = clanAdv.Tech.TechLevel[CLAN_TECH_CATEGORY_SHAPE_TOLUA];
	elseif CUR_SELECT_TYPE == 2 then
		curSelectLevel = CUR_SELECT_BACK_LEVEL;
		curTechLevel = clanAdv.Tech.TechLevel[CLAN_TECH_CATEGORY_SHADING_TOLUA];
	elseif CUR_SELECT_TYPE == 3 then
		curSelectLevel = CUR_SELECT_SIGN_LEVEL;
		curTechLevel = clanAdv.Tech.TechLevel[CLAN_TECH_CATEGORY_MARK_TOLUA];
	end
	BaseWarBannerFrameBKTexFont:SetText( t_BannerType[CUR_SELECT_TYPE] );
	BaseWarBannerFrameLevelTexInfo:SetText( t_BannerLevel[curSelectLevel]..t_BannerType[CUR_SELECT_TYPE].."�Ƽ�" )
	if t_BannerLevel[curSelectLevel] == "Ĭ��" then
		BaseWarBannerFrameLevelTexInfo:SetText( t_BannerLevel[curSelectLevel]..t_BannerType[CUR_SELECT_TYPE] )
	end
	for i = 1, MAX_BANNER_SHOW_NUM do
		local btn = getglobal( "BaseWarBannerFrameTexButton"..i );
		btn:Disable();
	end
	for i = 1, table.getn( t_BannerDefList[CUR_SELECT_TYPE][curSelectLevel] ) do
		local index = t_BannerDefList[CUR_SELECT_TYPE][curSelectLevel][i].DefIndex - 1;
		local bannerDef = GameClan_BaseWar:getBannerDef( index );
		local btn = getglobal( "BaseWarBannerFrameTexButton"..i );
		btn:SetClientUserData( 0, index );
		btn:Enable();
		local btnTex = getglobal( "BaseWarBannerFrameTexButton"..i.."IconTexture" );
		local path = string.format( "uires\\TuPianLei\\QiZiIcon\\%d.tga", bannerDef.IconID );
		if IsInExistence( path ) then
			btnTex:SetTexture( path );
		end
		btnTex:SetGray( true );
		btn:SetClientUserData( 1, 0 );
		--�ѵ�ǰ���õ�ͼ���ɫ��ʾȥ��
		if curTechLevel >= bannerDef.TechLimit then
			btnTex:SetGray( false );
			btn:SetClientUserData( 1, 1 );
		end
	end
	--�����������蹫���ʽ�
	local BannerNeed = 0;
	for i = 1 , MAX_BANNER_TYPES_NUM do
		local selectIndex = 1;
		if i == 2 then
			for m = 1, table.getn( t_BannerDefList[i][CUR_SELECT_BACK_LEVEL] ) do
				if m == CUR_SELECT_BACK then
					selectIndex = t_BannerDefList[i][CUR_SELECT_BACK_LEVEL][m].DefIndex;
				end
			end
		elseif i == 1 then
			for m = 1, table.getn( t_BannerDefList[i][CUR_SELECT_SHAPE_LEVEL] ) do
				if m == CUR_SELECT_SHAPE then
					selectIndex = t_BannerDefList[i][CUR_SELECT_SHAPE_LEVEL][m].DefIndex;
				end
			end
		elseif i == 3 then
			for m = 1, table.getn( t_BannerDefList[i][CUR_SELECT_SIGN_LEVEL] ) do
				if m == CUR_SELECT_SIGN then
					selectIndex = t_BannerDefList[i][CUR_SELECT_SIGN_LEVEL][m].DefIndex;
				end
			end
		end
		local bannerDef = GameClan_BaseWar:getBannerDef( selectIndex - 1 );
		BannerNeed = BannerNeed + bannerDef.ClanMoney;
	end
	BaseWarBannerFrameInputInfo:SetText( "���蹫���ʽ�"..BannerNeed );
end
function BaseWarBannerFrame_OnUpdate()
end
function BannerType_Onlick()
	CUR_SELECT_TYPE = this:GetClientID();
	SetBannerInfo();
end
function BannerSelectButton_Onlick()
	if CUR_SELECT_TYPE == 1 then
		CUR_SELECT_SHAPE_LEVEL = this:GetClientID();
		CUR_SELECT_SHAPE = 1;
	elseif CUR_SELECT_TYPE == 2 then
		CUR_SELECT_BACK_LEVEL = this:GetClientID();
		CUR_SELECT_BACK = 1;
	elseif CUR_SELECT_TYPE == 3 then
		CUR_SELECT_SIGN_LEVEL = this:GetClientID();
		CUR_SELECT_SIGN = 1;
	end
	SetBannerInfo();
end
function BannerTexButton_Onlick()
	local bannerDef = GameClan_BaseWar:getBannerDef( this:GetClientUserData( 0 ) );
	if CUR_SELECT_TYPE == 1 then
		CUR_SELECT_SHAPE = this:GetClientID();
	elseif CUR_SELECT_TYPE == 2 then
		CUR_SELECT_BACK = this:GetClientID();
	elseif CUR_SELECT_TYPE == 3 then
		CUR_SELECT_SIGN = this:GetClientID();
	end
	SetBannerInfo();
end
function BannerTexButton_OnEnter()
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local bannerDef = GameClan_BaseWar:getBannerDef( this:GetClientUserData( 0 ) )
	local szTip = "						".. bannerDef.Name .."\n".. "����״̬��".. (this:GetClientUserData( 1 ) == 0 and "#cff0000δ����#cffffbe" or "�Ѽ���" ) .."\nʹ�����ģ�"
	.. bannerDef.ClanMoney.."�����ʽ�\n\n"..(this:GetClientUserData( 1 ) == 0 and "#cff0000" or "" ).."����������"..( bannerDef.TechLimit == 0 and "Ĭ�Ͽ���						" or "������Ҫ�з�����Ƿ��Ƽ�[".. bannerDef.Name .."]�Ƽ�");
	SetGameTooltips( this:GetName(), szTip );
end
function BaseWarBannerFrame_OnShow()
	LoadBannerDef();
	SetBannerInfo();
end
function SetBaseWarBanner( npcID )
	BaseWarBannerFrame:Show();
	BaseWarBannerFrame:SetClientUserData( 0, npcID );
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		local mapId = MapShower:getRealMapId();
		BaseWarBannerFrame:SetClientUserData( 1, mapId );
	else
		local mapId = MaxMapControl:getMapID();
		BaseWarBannerFrame:SetClientUserData( 1, mapId );
	end
	util.testNpcDialogDistance( npcID, BaseWarBannerFrame );
end
function BaseWarBannerFrameRandomBtn_OnCick()
	CUR_SELECT_SHAPE_LEVEL = math.random( 1, table.getn(t_BannerDefList[1]) );
	CUR_SELECT_BACK_LEVEL = math.random( 1, table.getn(t_BannerDefList[2]) );
	CUR_SELECT_SIGN_LEVEL = math.random( 1, table.getn(t_BannerDefList[3]) );
	CUR_SELECT_BACK = math.random( 1, table.getn(t_BannerDefList[2][CUR_SELECT_BACK_LEVEL]) );
	CUR_SELECT_SHAPE = math.random( 1, table.getn(t_BannerDefList[1][CUR_SELECT_SHAPE_LEVEL]) );
	CUR_SELECT_SIGN = math.random( 1, table.getn(t_BannerDefList[3][CUR_SELECT_SIGN_LEVEL]) );
	SetBannerInfo();
end
function BaseWarBannerFrameDefBtn_OnCick()
	CUR_SELECT_BACK_LEVEL = 1;
	CUR_SELECT_SHAPE_LEVEL = 1;
	CUR_SELECT_SIGN_LEVEL = 1;
	CUR_SELECT_BACK = 1;
	CUR_SELECT_SHAPE = 1;
	CUR_SELECT_SIGN = 1;
	SetBannerInfo();
end
function BaseWarBannerFrameSaveBtn_OnCick()
	local canSetXuanYan = true;
	local needMoneyNum = 0;
	local clanAdv = GameClanManager:getAdvanceInfo();
	local shapeTechLevel = clanAdv.Tech.TechLevel[CLAN_TECH_CATEGORY_SHAPE_TOLUA];
	local bannerDef = GameClan_BaseWar:getBannerDef( t_BannerDefList[1][CUR_SELECT_SHAPE_LEVEL][CUR_SELECT_SHAPE].DefIndex - 1 );
	if bannerDef.TechLimit > shapeTechLevel then
		ShowMidTips( bannerDef.Name.."����ʹ�ã��޷�����" );
		canSetXuanYan = false;
	end
	needMoneyNum = needMoneyNum + bannerDef.ClanMoney;
	local backTechLevel = clanAdv.Tech.TechLevel[CLAN_TECH_CATEGORY_SHADING_TOLUA];
	bannerDef = GameClan_BaseWar:getBannerDef( t_BannerDefList[2][CUR_SELECT_BACK_LEVEL][CUR_SELECT_BACK].DefIndex - 1 )
	if bannerDef.TechLimit > backTechLevel then
		ShowMidTips( bannerDef.Name.."����ʹ�ã��޷�����" );
		canSetXuanYan = false;
	end
	needMoneyNum = needMoneyNum + bannerDef.ClanMoney;
	local signTechLevel = clanAdv.Tech.TechLevel[CLAN_TECH_CATEGORY_MARK_TOLUA];
	bannerDef = GameClan_BaseWar:getBannerDef( t_BannerDefList[3][CUR_SELECT_SIGN_LEVEL][CUR_SELECT_SIGN].DefIndex - 1 );
	if bannerDef.TechLimit > signTechLevel then
		ShowMidTips( bannerDef.Name.."����ʹ�ã��޷�����" );
		canSetXuanYan = false;
	end
	needMoneyNum = needMoneyNum + bannerDef.ClanMoney;
	if not canSetXuanYan then
		return;
	end
	local MyClanViewer = GameClanManager:getClanViewer();
	local MyClanInfo = MyClanViewer.m_clanInfo;
	if MyClanInfo.m_nMoney < needMoneyNum then
		ShowMidTips( "�����ʽ��㣬�޷�����" );
		return;
	end
	local npcID = BaseWarBannerFrame:GetClientUserData( 0 );
	local mapId = BaseWarBannerFrame:GetClientUserData( 1 );
	local shapeIndex = t_BannerDefList[1][CUR_SELECT_SHAPE_LEVEL][CUR_SELECT_SHAPE].subType;
	local backIndex = t_BannerDefList[2][CUR_SELECT_BACK_LEVEL][CUR_SELECT_BACK].subType;
	local signIndex = t_BannerDefList[3][CUR_SELECT_SIGN_LEVEL][CUR_SELECT_SIGN].subType;
	MessageBox( "�������ı���", "�����������Ҫ����"..needMoneyNum.."�����ʽ��Ƿ񱣴棿" );
	MessageBoxFrame:SetClientUserData( 0, mapId );
	MessageBoxFrame:SetClientUserData( 1, npcID );
	MessageBoxFrame:SetClientUserData( 2, shapeIndex );
	MessageBoxFrame:SetClientUserData( 3, backIndex );
	MessageBoxFrame:SetClientUserData( 4, signIndex );
end
function BaseWarBannerFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "�������ı���" then
		MessageBoxFrame:Hide();
	end
end
--�Զ��幫������
function SetBaseWarBannerXuanyan( npcID )
	ClanBannerWordEditeFrameXuanYanInput:SetText( "" );
	ClanBannerWordEditeFrame:Show();
	util.testNpcDialogDistance( npcID, ClanBannerWordEditeFrame );
end
function ClanBannerWordEditeFrameOKBtn_OnClick()
	local szText = ClanBannerWordEditeFrameXuanYanInput:GetText();
	local MaxMapControl = MapShower:GetMaxMapContal();
	local mapID = MaxMapControl:getMapID();
	GameClan_BaseWar:SetBannerXuanYan( mapID, szText );
	ClanBannerWordEditeFrame:Hide();
end
function ClanBannerWordEditeFrameCancleBtn_OnClick()
	ClanBannerWordEditeFrame:Hide();
end
------------------------------------------������Դ��ȡ��¼------------------------------------
local CUR_GETRES_PAGE = 1;
local CUR_GETRES_MAX_PAGE = 1;
local MAX_INFO_ONE_PAGE = 10
local ClanViewGetRes;
function ShowGetResInfo( npcId )
	SetGetResInfo()
	ClanGetResFrame:Show();
	util.testNpcDialogDistance( npcId, ClanGetResFrame );
end
function ClanGetResFrame_OnShow()
	ClanGetResFrame:SetPoint( "topleft", "UIClient", "topleft", 400, 200 );
end
function ClanGetResFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer:isInClan() or mainplayer:isDead() then
		ClanGetResFrame:Hide();
	end	
end
function ClanGetResFrame_OnHide()
	CUR_GETRES_PAGE = 1;
	CUR_GETRES_MAX_PAGE = 1;
	GameClanManager:ClearClanViewGetRes();
end
function SetGetResInfo()
	for i = 1, MAX_INFO_ONE_PAGE do
		local Name = getglobal( "ClanGetResFrameGetResInfo"..i.."Name" );
		Name:SetText( "" );
		local Pos = getglobal( "ClanGetResFrameGetResInfo"..i.."Pos" );
		Pos:SetText( "" );
		local GetNum = getglobal( "ClanGetResFrameGetResInfo"..i.."GetNum" );
		GetNum:SetText( "" )
		local ItemName = getglobal( "ClanGetResFrameGetResInfo"..i.."ItemName" );
		ItemName:SetText( "" );
		local UseNum = getglobal( "ClanGetResFrameGetResInfo"..i.."UseNum" );
		UseNum:SetText( "" );
		local UseNumTex = getglobal( "ClanGetResFrameGetResInfo"..i.."UseNumTex" );
		UseNumTex:Hide();
		local GetTime = getglobal( "ClanGetResFrameGetResInfo"..i.."GetTime" );
		GetTime:SetText("");
	end
	ClanViewGetRes = GameClanManager:getCalnViewGetRes();
	CUR_GETRES_MAX_PAGE = ClanViewGetRes.PageCount;
	if CUR_GETRES_MAX_PAGE == 0 or CUR_GETRES_MAX_PAGE == nil then
		ClanGetResFramePageInfo:SetText( "��0ҳ/��0ҳ" );
		return;
	end
	CUR_GETRES_PAGE = ClanViewGetRes.PageCurr + 1;
	for i = 1, ClanViewGetRes.InfoNum do
		local Name = getglobal( "ClanGetResFrameGetResInfo"..i.."Name" );
		Name:SetTextColor( 160, 160, 90 );
		Name:SetText( ClanViewGetRes.GetResInfos[i-1].Name );
		local Pos = getglobal( "ClanGetResFrameGetResInfo"..i.."Pos" );
		local szPos = GetPosition( ClanViewGetRes.GetResInfos[i-1].Position )
		Pos:SetTextColor( 160, 160, 90 );
		Pos:SetText( szPos );
		local GetNum = getglobal( "ClanGetResFrameGetResInfo"..i.."GetNum" );
		GetNum:SetTextColor( 160, 160, 90 );
		GetNum:SetText( ClanViewGetRes.GetResInfos[i-1].GetNum )
		local ItemName = getglobal( "ClanGetResFrameGetResInfo"..i.."ItemName" );
		local itemDef = getItemDef( ClanViewGetRes.GetResInfos[i-1].ItemID )
		ItemName:SetTextColor( 160, 160, 90 );
		ItemName:SetText( itemDef.Name );
		local UseNum = getglobal( "ClanGetResFrameGetResInfo"..i.."UseNum" );
		UseNum:SetTextColor( 160, 160, 90 );
		UseNum:SetText( ClanViewGetRes.GetResInfos[i-1].UseContri );
		local UseNumTex = getglobal( "ClanGetResFrameGetResInfo"..i.."UseNumTex" );
		UseNumTex:Show();
		local GetTime = getglobal( "ClanGetResFrameGetResInfo"..i.."GetTime" );
		local t_time = os.date( "*t", ClanViewGetRes.GetResInfos[i-1].GetTime );
		GetTime:SetTextColor( 160, 160, 90 );
		GetTime:SetText( t_time.year.."-".. t_time.month .."-".. t_time.day .."	".. t_time.hour ..":".. t_time.min );
	end
	if CUR_GETRES_MAX_PAGE == 0 then
		CUR_GETRES_PAGE = 0;
	end
	ClanGetResFramePageInfo:SetText( "��".. CUR_GETRES_PAGE.."ҳ/��"..CUR_GETRES_MAX_PAGE.."ҳ" );
end
function ClanGetResFrameFirstPage_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then return end
	CUR_GETRES_PAGE = 1;
	GameClanManager:Clan_Opt_AskGetResInfo( CUR_GETRES_PAGE - 1 );
	SetGetResInfo();
end
function ClanGetResFramePrePage_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then return end
	if CUR_GETRES_PAGE > 1 then
		CUR_GETRES_PAGE = CUR_GETRES_PAGE - 1;
	end
	GameClanManager:Clan_Opt_AskGetResInfo( CUR_GETRES_PAGE - 1 );
	SetGetResInfo();
end
function ClanGetResFrameNextPage_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then return end
	if CUR_GETRES_PAGE < CUR_GETRES_MAX_PAGE then
		CUR_GETRES_PAGE = CUR_GETRES_PAGE + 1;
	end
	GameClanManager:Clan_Opt_AskGetResInfo( CUR_GETRES_PAGE - 1 );
	SetGetResInfo();
end
function ClanGetResFrameLastPage_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then return end
	CUR_GETRES_PAGE = CUR_GETRES_MAX_PAGE;
	GameClanManager:Clan_Opt_AskGetResInfo( CUR_GETRES_PAGE - 1 );
	SetGetResInfo();
end
-----------------------------------ս������ͼ------------------------------------
local t_WarForces = {
					[1] = [[
					
					
					
					
#cffffbe 	ս����ͼ���п�����
#cffffbe 	ս��ʱ�䣺ÿ����20��00-21��30��
#cffffbe 	��սȨ�ޣ�ֻ����ռ��ݵ�Ĺ���ӵ
#cffffbe 						������ʱ��ǿ���Ȩ��Ȩ
#cffffbe 						��

#cffffbe 	ս��������ϸ��Ϣ����[ʱ�����
#cffffbe 						ָ�ӹ�]����˹����ѯ

#cffffbe 	ս���ص㣺�����ն˿������Ŀ���Ȩ
#cffffbe 						
#cffffbe 	ʤ��������ʤ�����Ὣ���ʱ���ռ
#cffffbe 						��Ȩ��ͬʱ����ȡռ�콱
#cffffbe 						����ʹ���ڶ���Ȩ��]],
					[2] = [[
					
					
					
					
#cffffbe 	ս����ͼ���ض���ս��������
#cffffbe 	ս��ʱ�䣺ÿ�ο������ĵڶ���
#cffffbe 						��20��30-21��30��
#cffffbe 	��սȨ�ޣ���ÿ���ݵ㾺���л�ʤ��
#cffffbe 						�Ὣӵ������ݵ�ռ��Ȩ
#cffffbe 						���ʸ�

#cffffbe 	ս���ص㣺�����ͼ�ڵ�4����Դ��
#cffffbe 						ͼ�ڣ�������Դ
#cffffbe 	ʤ��������ʤ�����Ὣ��þݵ�ռ��
#cffffbe 						Ȩ��ͬʱ����ȡռ�콱��
#cffffbe 						��ʹ���ڶ���Ȩ
#cffffbe 	�ص���ʾ���ݵ�ռ�칫����ھݵ���
#cffffbe						�ڵ�ͼ[�ݵ�������]ɳ��
#cffffbe						����ʱ�����ݵ�ռ��Ȩ]],
					}
local t_WarForcesMapBtnTexture = {
	--���ջ�Ұ
	[1] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LuoRiHuangYe-1.tga", 
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LuoRiHuangYe-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LuoRiHuangYe-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LuoRiHuangYe-4.tga",
	},
	--ʱ���
	[2] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\ShiJianCheng-1.tga", 
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\ShiJianCheng-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\ShiJianCheng-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\ShiJianCheng-4.tga",
	},
	--����
	[3] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\BingDao-1.tga",
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\BingDao-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\BingDao-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\BingDao-4.tga",
	},
	--����ѩԭ
	[4] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YinFengXueYuan-1.tga", 
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YinFengXueYuan-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YinFengXueYuan-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YinFengXueYuan-4.tga",
	},
	--Խ������
	[5] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YueWuYuLin-1.tga", 
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YueWuYuLin-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YueWuYuLin-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\YueWuYuLin-4.tga",
	},
	--����ɳĮ
	[6] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LiuJinShaMo-1.tga", 
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LiuJinShaMo-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LiuJinShaMo-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\LiuJinShaMo-4.tga",
	},
	--��ɳ�ݵ�
	[7] = {
		normalTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\MingShaXianDi-1.tga", 
		HighlightTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\MingShaXianDi-2.tga",
		pushTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\MingShaXianDi-3.tga",
		grayTexture = "uires\\TuPianLei\\ZhanZhengShiLiTu\\MingShaXianDi-4.tga",
	},
}
local t_WarForcesStrongInfos = {
	--���ջ�Ұ
	[1] = {
		mapID = 2400, npcID = 302137, mapName = "���ջ�Ұ", stateIndex = STRONG_STAT_NOT_OPEN,
	},
	--ʱ���
	[2] = {
		mapID = 1000, npcID = 307007, mapName = "ʱ���", stateIndex = WORLD_CITY_NOT_OPEN,
	},
	--����
	[3] = {
		mapID = 2202, npcID = 302134, mapName = "����",  stateIndex = STRONG_STAT_NOT_OPEN,
	},
	--����ѩԭ
	[4] = {
		mapID = 2200, npcID = 302135, mapName = "����ѩԭ", stateIndex = STRONG_STAT_NOT_OPEN,
	},
	--Խ������
	[5] = {
		mapID = 2500, npcID = 302138, mapName = "Խ������",  stateIndex = STRONG_STAT_NOT_OPEN,
	},
	--����ɳĮ
	[6] = {
		mapID = 2300, npcID = 302136, mapName = "����ɳĮ",  stateIndex = STRONG_STAT_NOT_OPEN,
	},
	--��ɳ�ݵ�
	[7] = {
		mapID = 3404, npcID = 302139, mapName = "��ɳ�ݵ�", stateIndex = STRONG_STAT_NOT_OPEN,
	},
}
STRONG_STAT_NOT_OPEN = 16;
STRONG_STAT_NOT_OWNER = 17;
STRONG_STAT_JOINT_SALE = 18;
t_strongStateUV = {
	--��һ�Ŀ���	
	[STRONG_STAT_NO_TOLUA] = { x = "753", y = "301", w = "52", h = "18", name="����", },
	--����
	[STRONG_STAT_IDLE_TOLUA] = { x = "753", y = "301", w = "52", h = "18", name="����", },
	--����
	[STRONG_STAT_SALE_TOLUA] = { x = "647", y = "301", w = "52", h = "18", name="�ɾ���", },
	--������
	[STRONG_STAT_TONGJI_TOLUA] = { x = "912", y = "301", w = "52", h = "18", name="������", },
	--�Ѿ���
	[STRONG_STAT_JOINT_SALE] = { x = "964", y = "301", w = "52", h = "18", name="�Ѿ���", },
	--ս����
	[STRONG_STAT_FIGHT_TOLUA] = { x = "806", y = "301", w = "52", h = "18", name="ս����", },
	--δ����
	[STRONG_STAT_NOT_OPEN] = { x = "859", y = "301", w = "52", h = "18", name="δ����", },
	--����֮��
	[STRONG_STAT_NOT_OWNER] = { x = "700", y = "301", w = "52", h = "18", name="����֮��", },
	--��ս��
	[STRONG_STAT_SELECT_TOLUA] = { x = "647", y = "282", w = "52", h = "18", name="��ս��", },
}
local WORLD_CITY_IDLE = 16;
local WORLD_CITY_NO_OWNER = 17;
local WORLD_CITY_NOT_OPEN = 18;
local t_centerControlStateUV = {
	--����
	[WORLD_CITY_IDLE] = { x = "753", y = "301", w = "52", h = "18", name="����", },
	--ս����
	[WORLD_CITY_STAT_FIGHT_TOLUA] = { x = "806", y = "301", w = "52", h = "18", name="ս����", },
	--δ����
	[WORLD_CITY_NOT_OPEN] = { x = "859", y = "301", w = "52", h = "18", name="δ����", },
	--����֮��
	[WORLD_CITY_NO_OWNER] = { x = "700", y = "301", w = "52", h = "18", name="����֮��", },
	--��ս��
	[WORLD_CITY_STAT_CLEAR_TOLUA] = { x = "647", y = "282", w = "52", h = "18", name="��ս��", },
}
local CUR_SELECT_STONG_INDEX = 0
local CUR_WAR_FORCES_STRONG_NUM = 7;
local StrongAllInfo;
function WarForcesFrame_OnLoad()
	this:setUpdateTime( 5 );
	for i= 1, CUR_WAR_FORCES_STRONG_NUM do
		local btn = getglobal( "WarForcesFrameAreaBtn"..i )
		local btnClientID = btn:GetClientID();
		btn:SetNormalTexture( t_WarForcesMapBtnTexture[btnClientID].normalTexture, "blend" );
		btn:SetHightlightTexture( t_WarForcesMapBtnTexture[btnClientID].HighlightTexture, "blend" );
		btn:SetPushedTexture( t_WarForcesMapBtnTexture[btnClientID].pushTexture, "blend" )
		btn:SetDisableTexture( t_WarForcesMapBtnTexture[btnClientID].pushTexture, "blend" )
	end
end
function GetStrongAllInfo()
	if not ( GameClan_BaseWar:getAllStrongInfoNum() == MAX_STRONG_NUM ) then
		return;
	end
	SetWarForcesInfo();
end
function WarForcesFrame_OnShow()
	if not WarForcesBtn:IsShown() then
		WarForcesFrame:Hide();
		return;
	end
	TransWarForcesFrame:Hide()
	GetStrongAllInfo();
	--��սǰ��������Ϣ
	WarForcesFrameRemindBtnInfo:SetText( "\t\t\t\t\t��   ʾ\n�ݵ�ս��ս��,ս�����ý�������������������ǰ���źø���ս�ŵĲ���",255,0,0 )
end
function WarForcesFrame_OnUpdate()
	if not WarForcesBtn:IsShown() then
		WarForcesFrame:Hide();
		return;
	end
	--������ڿ��ս����رվݵ�����ͼ���
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		WarForcesFrame:Hide();
		return;
	end
	GameClan_BaseWar:RequestAllStrongInfo();
	GetStrongAllInfo();
end
function WarForcesFrame_OnHide()
	CUR_SELECT_STONG_INDEX = 0;
	if MapAwardOreShowFrame:IsShown() then
		MapAwardOreShowFrame:Hide();
	end
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�ݵ�ȷ�Ͽ�" then
		MessageBoxFrame:Hide();
	end
end
function SetWarForcesInfo()
	--�ж��ұ���ʾ
	if CUR_SELECT_STONG_INDEX == 0 then
		WarForcesFrameInfosBtnCenterControlInfo:SetText( t_WarForces[1], 255, 255, 190 )
		WarForcesFrameInfosBtnBaseWarInfo:SetText( t_WarForces[2], 255, 255, 190 )
		WarForcesFrameInfosBtn:Show();
		WarForcesFrameCenterControlInfoBtn:Hide();
		WarForcesFrameStrongInfoBtn:Hide();
	elseif CUR_SELECT_STONG_INDEX == 2 then
		WarForcesFrameInfosBtn:Hide();
		WarForcesFrameCenterControlInfoBtn:Show();
		WarForcesFrameStrongInfoBtn:Hide();
	else
		WarForcesFrameInfosBtn:Hide();
		WarForcesFrameCenterControlInfoBtn:Hide();
		WarForcesFrameStrongInfoBtn:Show();
	end
	local mainplayer = ActorMgr:getMainPlayer()
	--���õ�ͼ�Ͼݵ���Ϣ
	for i= 1, GameClan_BaseWar:getAllStrongInfoNum() do
		local occupInfo = GameClan_BaseWar:getStrongOccupInfoByIndex( i - 1 );
		local strongSaleInfo = GameClan_BaseWar:getStrongSaleInfoByIndex( i - 1 );
		--��ȷ�����ĸ���ͼ�ݵ����Ϣ
		for j = 1, CUR_WAR_FORCES_STRONG_NUM do
			if t_WarForcesStrongInfos[j].mapID == occupInfo.MapID then
				local btn = getglobal( "WarForcesFrameAreaBtn"..j )
				local FangTex  = getglobal( "WarForcesFrameAreaBtn"..j.."FangTex" )
				local FangName = getglobal( "WarForcesFrameAreaBtn"..j.."FangName" )
				local GongTex  = getglobal( "WarForcesFrameAreaBtn"..j.."GongTex" )
				local GongName = getglobal( "WarForcesFrameAreaBtn"..j.."GongName" )
				local SateTex = getglobal( "WarForcesFrameAreaBtn"..j.."SateTex" )
				local btnClientID = btn:GetClientID();
				--���жϾݵ��Ƿ���
				if occupInfo.IsOpen == 1 then
					btn:SetNormalTexture( t_WarForcesMapBtnTexture[btnClientID].normalTexture, "blend" );
					btn:SetHightlightTexture( t_WarForcesMapBtnTexture[btnClientID].HighlightTexture, "blend" );
					btn:SetPushedTexture( t_WarForcesMapBtnTexture[btnClientID].pushTexture, "blend" )
					btn:SetDisableTexture( t_WarForcesMapBtnTexture[btnClientID].pushTexture, "blend" )
					local uv = t_strongStateUV[strongSaleInfo.SaleStat];
					t_WarForcesStrongInfos[j].stateIndex = strongSaleInfo.SaleStat;
						if GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == "" or GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == nil then
						uv = t_strongStateUV[STRONG_STAT_NOT_OWNER];
						t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_NOT_OWNER;
					end
					if strongSaleInfo.SaleStat == STRONG_STAT_IDLE_TOLUA then
						uv = t_strongStateUV[strongSaleInfo.SaleStat];
						t_WarForcesStrongInfos[j].stateIndex = strongSaleInfo.SaleStat;
						if GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == "" or GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == nil then
							uv = t_strongStateUV[STRONG_STAT_NOT_OWNER];
							t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_NOT_OWNER;
						end
					elseif strongSaleInfo.SaleStat == STRONG_STAT_SALE_TOLUA then
						uv = t_strongStateUV[strongSaleInfo.SaleStat];
						t_WarForcesStrongInfos[j].stateIndex = strongSaleInfo.SaleStat;
						if strongSaleInfo.ClanJoin == 1 and mainplayer:getClanPosition() ~= CLAN_POS_NORMAL then
							uv = t_strongStateUV[STRONG_STAT_JOINT_SALE];
							t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_JOINT_SALE;
						end
					elseif strongSaleInfo.SaleStat == STRONG_STAT_TONGJI_TOLUA then
						uv = t_strongStateUV[strongSaleInfo.SaleStat];
						t_WarForcesStrongInfos[j].stateIndex = strongSaleInfo.SaleStat;
					elseif strongSaleInfo.SaleStat == STRONG_STAT_SELECT_TOLUA then
						uv = t_strongStateUV[strongSaleInfo.SaleStat];
						t_WarForcesStrongInfos[j].stateIndex = strongSaleInfo.SaleStat;
						if GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == "" or GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == nil then
							uv = t_strongStateUV[STRONG_STAT_NOT_OWNER];
							t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_NOT_OWNER;
						else
							if GameClanManager:clanGetClanNameByWid( strongSaleInfo.AtkClanWid ) == "" or GameClanManager:clanGetClanNameByWid( strongSaleInfo.AtkClanWid ) == nil then
								uv = t_strongStateUV[STRONG_STAT_IDLE_TOLUA];
								t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_IDLE_TOLUA;
							end
						end
					elseif strongSaleInfo.SaleStat == STRONG_STAT_FIGHT_TOLUA then
						uv = t_strongStateUV[strongSaleInfo.SaleStat];
						t_WarForcesStrongInfos[j].stateIndex = strongSaleInfo.SaleStat;
						if GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == "" or GameClanManager:clanGetClanNameByWid(occupInfo.ClanWid) == nil then
							uv = t_strongStateUV[STRONG_STAT_IDLE_TOLUA];
							t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_IDLE_TOLUA;
						end
					end
					local fangClanName = GameClanManager:clanGetClanNameByWid( occupInfo.ClanWid );
					local OneClanColor = GameClan_BaseWar:getStrongOwnClanFengHaoColor( occupInfo.ClanWid );
					local r, g, b = (OneClanColor / 65536) % 256, (OneClanColor / 256) % 256, OneClanColor % 256
					FangName:SetText( fangClanName );
					--���ʹ�õ���ϵͳĬ����ɫ����Ϊ��ɫ
					if OneClanColor == 4286644096 then
						r, g, b = 255, 255, 255
					end
					FangName:SetTextColor( r, g, b );
					FangTex:Show();
					if fangClanName == "" or fangClanName == nil then
						FangTex:Hide();
					end
					local gongClanName = GameClanManager:clanGetClanNameByWid( strongSaleInfo.AtkClanWid );
					GongTex:Show();
					if gongClanName == "" or gongClanName == nil then
						GongTex:Hide();
					end
					GongName:SetText( gongClanName );
					--�����������,��û�п���
					if (gongClanName ~= "" and gongClanName ~= nil) and strongSaleInfo.SaleStat ~= STRONG_STAT_FIGHT_TOLUA then
						--�����Լ�����
						if gongClanName ~= mainplayer:getClanName() then
							GongName:SetText( "������ս��..." );
						end
						--���Լ����ᣬ�����Լ����ǹ����Ա
						if gongClanName == mainplayer:getClanName() and mainplayer:getClanPosition() == CLAN_POS_NORMAL then
							GongName:SetText( "������ս��..." );
						end
					end
					if gongClanName == fangClanName and ( gongClanName ~= "" and gongClanName ~= nil) then
						GongTex:Hide();
						GongName:SetText( "" );
						uv = t_strongStateUV[STRONG_STAT_IDLE_TOLUA];
						t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_IDLE_TOLUA;
					end
					SateTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
				else
					btn:SetNormalTexture( t_WarForcesMapBtnTexture[btnClientID].grayTexture, "blend" );
					btn:SetHightlightTexture( t_WarForcesMapBtnTexture[btnClientID].grayTexture, "blend" );
					btn:SetPushedTexture( t_WarForcesMapBtnTexture[btnClientID].grayTexture, "blend" )
					btn:SetDisableTexture( t_WarForcesMapBtnTexture[btnClientID].grayTexture, "blend" )
					FangName:SetText( "" );
					FangTex:Hide();
					local uv = t_strongStateUV[STRONG_STAT_NOT_OPEN];
					t_WarForcesStrongInfos[j].stateIndex = STRONG_STAT_NOT_OPEN;
					SateTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
					GongTex:Hide();
					GongName:SetText( "" );
				end
			end
		end
	end
	--���õ�ͼ���п�������Ϣ
	local btn = getglobal( "WarForcesFrameAreaBtn2" );
	local FangTex  = getglobal( "WarForcesFrameAreaBtn2FangTex" );
	local FangName = getglobal( "WarForcesFrameAreaBtn2FangName" );
	local GongTex  = getglobal( "WarForcesFrameAreaBtn2GongTex" );
	local GongName = getglobal( "WarForcesFrameAreaBtn2GongName" );
	local SateTex = getglobal( "WarForcesFrameAreaBtn2SateTex" );
	FangTex:Hide();
	FangName:SetText( "" );
	FangName:SetTextColor( 255, 255, 190 )
	--ʱ���������û�й���
	GongTex:Hide();
	GongName:SetText( "" );
	SateTex:Show();	
	btn:SetNormalTexture( t_WarForcesMapBtnTexture[2].grayTexture, "blend" );
	btn:SetHightlightTexture( t_WarForcesMapBtnTexture[2].grayTexture, "blend" );
	btn:SetPushedTexture( t_WarForcesMapBtnTexture[2].grayTexture, "blend" )
	btn:SetDisableTexture( t_WarForcesMapBtnTexture[2].grayTexture, "blend" )
	local uv = t_centerControlStateUV[WORLD_CITY_NOT_OPEN];
	t_WarForcesStrongInfos[2].stateIndex = WORLD_CITY_NOT_OPEN;
	if WorldCityManager:GerCityFightEnableStat() ~= 0 then
		btn:SetNormalTexture( t_WarForcesMapBtnTexture[2].normalTexture, "blend" );
		btn:SetHightlightTexture( t_WarForcesMapBtnTexture[2].HighlightTexture, "blend" );
		btn:SetPushedTexture( t_WarForcesMapBtnTexture[2].pushTexture, "blend" )
		btn:SetDisableTexture( t_WarForcesMapBtnTexture[2].pushTexture, "blend" )
		uv = t_centerControlStateUV[WORLD_CITY_NO_OWNER];
		t_WarForcesStrongInfos[2].stateIndex = WORLD_CITY_NO_OWNER;
		if WorldCityManager:GetCityKeeperClanID() ~= 0 then
			local clanWID = WorldCityManager:GetCityKeepClanWID()
			local ClanName = GameClanManager:clanGetClanNameByWid( WorldCityManager:GetCityKeepClanWID() );
			if ClanName ~= nil then
				FangTex:Show();
				local OneClanColor = GameClan_BaseWar:getStrongOwnClanFengHaoColor( WorldCityManager:GetCityKeepClanWID() );
				--������ɫ
				local r, g, b = (OneClanColor / 65536) % 256, (OneClanColor / 256) % 256, OneClanColor % 256
				--���ʹ�õ���ϵͳĬ����ɫ����Ϊ��ɫ
				if OneClanColor == 4286644096 then
					r, g, b = 255, 255, 255
				end
				FangName:SetTextColor( r, g, b )
				FangName:SetText( ClanName );
				uv = t_centerControlStateUV[WORLD_CITY_IDLE];
				t_WarForcesStrongInfos[2].stateIndex = WORLD_CITY_IDLE;
			end
		end
		if GameClan_BaseWar:getCityWarFightStat() ~= 0 then
			uv = t_centerControlStateUV[GameClan_BaseWar:getCityWarFightStat()];
			t_WarForcesStrongInfos[2].stateIndex = GameClan_BaseWar:getCityWarFightStat();
		end
	end
	SateTex:SetTexUV( uv.x, uv.y, uv.w, uv.h );
	
	--����ս������ͼ�ұߵ���ʾ��Ϣ
	if CUR_SELECT_STONG_INDEX ~= 0 and CUR_SELECT_STONG_INDEX <= 7 then
		if CUR_SELECT_STONG_INDEX == 2 then
			SetWarForcesCenterControlBtnInfo( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].stateIndex)
		else
			SetWarForcesStrongBtnInfo( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].stateIndex, CUR_SELECT_STONG_INDEX )
		end
	end
	for i = 1, CUR_WAR_FORCES_STRONG_NUM do
		local btn = getglobal( "WarForcesFrameAreaBtn"..i )
		btn:SetNormalTexture( t_WarForcesMapBtnTexture[i].normalTexture, "blend" );
		btn:SetHightlightTexture( t_WarForcesMapBtnTexture[i].HighlightTexture, "blend" );
		btn:SetPushedTexture( t_WarForcesMapBtnTexture[i].pushTexture, "blend" )
		btn:SetDisableTexture( t_WarForcesMapBtnTexture[i].normalTexture, "blend" )
	end
	if CUR_SELECT_STONG_INDEX ~= 0 then
		local btn = getglobal( "WarForcesFrameAreaBtn"..CUR_SELECT_STONG_INDEX )
		btn:SetNormalTexture( t_WarForcesMapBtnTexture[CUR_SELECT_STONG_INDEX].HighlightTexture, "blend" );
		btn:SetHightlightTexture( t_WarForcesMapBtnTexture[CUR_SELECT_STONG_INDEX].HighlightTexture, "blend" );
		btn:SetPushedTexture( t_WarForcesMapBtnTexture[CUR_SELECT_STONG_INDEX].HighlightTexture, "blend" )
		btn:SetDisableTexture( t_WarForcesMapBtnTexture[CUR_SELECT_STONG_INDEX].HighlightTexture, "blend" )
	end
end

function GetTotalAwardMoney()
	local mainplayer = ActorMgr:getMainPlayer()
	--������ڹ��ᣬ���ý����±ߵ�ͳ����
	if not mainplayer:isInClan() then
		return 0
	end
	local clanName = mainplayer:getClanName()
	local nClanPointNum = 0
	local nOwnClanPointMaxLV = 0
	local nMaxClanPointAwardMoney = 0
	--��ͳ��ռ��ľݵ�
	for i = 1, table.getn(t_WarForcesStrongInfos) do
		local mapID = t_WarForcesStrongInfos[i].mapID
		local curMapOwnClan = GameClan_BaseWar:getCurMapStrongOWnerName( mapID )
		--������ڹ���ռ���˸þݵ㣬��ͳ�ƽ���
		if clanName == curMapOwnClan then
			nClanPointNum = nClanPointNum + 1
			if GameClan_BaseWar:getStrongPointLVByMapID( mapID ) > nOwnClanPointMaxLV then
				nOwnClanPointMaxLV = GameClan_BaseWar:getStrongPointLVByMapID( mapID )
				nMaxClanPointAwardMoney = GameClan_BaseWar:getStrongAwardMoney( mapID )
			end			
		end
	end
	--�����ж��Ƿ�ռ����ʱ���
	if WorldCityManager:IsWorldCityKeeper(mainplayer) then
		nClanPointNum = nClanPointNum + 1
		nMaxClanPointAwardMoney = 100000
	end
	local nTotalMoney = nMaxClanPointAwardMoney + ( (nClanPointNum-1)*10000 > 0 and (nClanPointNum-1)*10000 or 0 )
	return nTotalMoney
end

function SetWarForcesCenterControlBtnInfo( stateIndex )
	WarForcesFrameCenterControlInfoBtnCityOwnerClan:SetText( "ռ�칫�᣺��" );
	WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetText( "��" );
	WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetTextColor( 255, 255, 190 )
	WarForcesFrameCenterControlInfoBtnCityOwnerClanPresentName:SetText( "����᳤����" );
	WarForcesFrameCenterControlInfoBtnCityOwnerClanLV:SetText( "����ȼ���0" );
	WarForcesFrameCenterControlInfoBtnCityOwnerClanMemberNum:SetText( "��Ա������0"  );
	WarForcesFrameCenterControlInfoBtnCityChanllengedNum:SetText( "ռ�������0��" );
	--WarForcesFrameCenterControlInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�"..WorldCityManager:GetCityOwnerAwardMoney().."����" );
	WarForcesFrameCenterControlInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�"..GetTotalAwardMoney().."����" );
	if GetTotalAwardMoney() == 0 then
		WarForcesFrameCenterControlInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�δռ��" );
	end
	WarForcesFrameCenterControlInfoBtnEnerCityWarBtn:SetGrayEx( true );
	WarForcesFrameCenterControlInfoBtnEnerCityWarBtn:Disable();
	if WorldCityManager:GetCityKeeperClanID() ~= 0 then
		local clanName = GameClanManager:clanGetClanNameByWid( WorldCityManager:GetCityKeepClanWID() );
		WarForcesFrameCenterControlInfoBtnCityOwnerClan:SetText( "ռ�칫�᣺"..clanName );
		local fengHao = ActorMgr:getClanFengHao( WorldCityManager:GetCityKeepClanWID() );
		if string.find( fengHao, "����", 1, true ) then
			WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetTextColor( 109, 0, 210 )
		elseif string.find( fengHao, "����", 1, true ) then
			WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetTextColor( 24, 101, 6 )
		elseif string.find( fengHao, "����", 1, true ) then
			WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetTextColor( 0, 94, 190 )
		elseif string.find( fengHao, "����", 1, true ) then
			WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetTextColor( 255, 78, 0 )
		end
		WarForcesFrameCenterControlInfoBtnCityOwnerClanFengHao:SetText( fengHao );
		local clanPresentName = GameClanManager:GetClanCaptionNameByWid( WorldCityManager:GetCityKeepClanWID() )
		WarForcesFrameCenterControlInfoBtnCityOwnerClanPresentName:SetText( "����᳤��"..clanPresentName );
		local clanLV = GameClanManager:GetClanLVByWid( WorldCityManager:GetCityKeepClanWID() );
		WarForcesFrameCenterControlInfoBtnCityOwnerClanLV:SetText( "����ȼ���"..clanLV );
		local clanMemNum = GameClanManager:GetClanMemNumByWid( WorldCityManager:GetCityKeepClanWID() );
		WarForcesFrameCenterControlInfoBtnCityOwnerClanMemberNum:SetText( "��Ա������"..clanMemNum );
		
		WarForcesFrameCenterControlInfoBtnCityChanllengedNum:SetText( "ռ�������"..(WorldCityManager:GetCityOwnerKeepNum()+1) );
		--WarForcesFrameCenterControlInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�"..WorldCityManager:GetCityOwnerAwardMoney().."����" );
		WarForcesFrameCenterControlInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�"..GetTotalAwardMoney().."����" );
		if GetTotalAwardMoney() == 0 then
			WarForcesFrameCenterControlInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�δռ��" );
		end
	end
	if t_centerControlStateUV[stateIndex].name == "ս����" then
		WarForcesFrameCenterControlInfoBtnEnerCityWarBtn:SetGrayEx( false );
		WarForcesFrameCenterControlInfoBtnEnerCityWarBtn:Enable();
	end
	
	WarForcesFrameCenterControlInfoBtnCityState:SetText( "����״̬��"..t_centerControlStateUV[stateIndex].name )
end
function SetWarForcesStrongBtnInfo( stateIndex, curSelectIndex )
	WarForcesFrameStrongInfoBtnStrongOwnerClan:SetText( "ռ�칫�᣺��" );
	WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetText( "��" );
	WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetTextColor( 255, 255, 190 )
	WarForcesFrameStrongInfoBtnStrongOwnerClanPresentName:SetText( "����᳤����" );
	WarForcesFrameStrongInfoBtnStrongOwnerClanLV:SetText( "����ȼ���0");
	WarForcesFrameStrongInfoBtnStrongOwnerClanMemberNum:SetText( "��Ա������0" );
	WarForcesFrameStrongInfoBtnStrongState:SetText( "�ݵ�״̬��"..t_strongStateUV[stateIndex].name )
	WarForcesFrameStrongInfoBtnTitleFont:SetText( t_WarForcesStrongInfos[curSelectIndex].mapName )
	WarForcesFrameStrongInfoBtnStrongName:SetText( "�ݵ����ƣ�["..t_WarForcesStrongInfos[curSelectIndex].mapName.."]�ݵ�" );
	WarForcesFrameStrongInfoBtnStrongChanllenger:SetText( "��ս���᣺��" );
	if t_strongStateUV[stateIndex].name == "ս����" then
		WarForcesFrameStrongInfoBtnEnterBaseWarBtn:SetGrayEx( false );
		WarForcesFrameStrongInfoBtnEnterBaseWarBtn:Enable();
	else
		WarForcesFrameStrongInfoBtnEnterBaseWarBtn:SetGrayEx( true );
		WarForcesFrameStrongInfoBtnEnterBaseWarBtn:Disable();
	end
	
	WarForcesFrameStrongInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�0����" )
	local mapID = t_WarForcesStrongInfos[curSelectIndex].mapID
	WarForcesFrameStrongInfoBtnStrongState:SetText( "�ݵ�״̬��"..t_strongStateUV[stateIndex].name )
	--WarForcesFrameStrongInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�"..GameClan_BaseWar:getStrongAwardMoney(mapID).."����" );
	WarForcesFrameStrongInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�"..GetTotalAwardMoney().."����" );
	if GetTotalAwardMoney() == 0 then
		WarForcesFrameStrongInfoBtnOwnerAwardClanMoney:SetText( "ռ���ʽ�δռ��" );
	end
	local buffID = GameClan_BaseWar:getStrongAwardBuffID( mapID );
	local buffLv = GameClan_BaseWar:getStrongAwardBuffLV( mapID );
	local statusDef = getStatusDef( buffID, buffLv );
	WarForcesFrameStrongInfoBtnOwnerAwardBuffer:SetText( "����Ч����"..statusDef.Name );
	WarForcesFrameStrongInfoBtnOwnerAwardContri:SetText( "���˹��ף�"..GameClan_BaseWar:getStrongAwardClanContri( mapID ) );
	local occupInfo;
	local saleInfo;
	for i= 1, GameClan_BaseWar:getAllStrongInfoNum() do
		local OccupInfo = GameClan_BaseWar:getStrongOccupInfoByIndex( i - 1 );
		if t_WarForcesStrongInfos[curSelectIndex].mapID == OccupInfo.MapID then
			occupInfo = OccupInfo;
			saleInfo = GameClan_BaseWar:getStrongSaleInfoByIndex( i -1 );
			if not ( GameClanManager:clanGetClanNameByWid( saleInfo.AtkClanWid ) == "" or GameClanManager:clanGetClanNameByWid( saleInfo.AtkClanWid ) == nil ) then
				WarForcesFrameStrongInfoBtnStrongChanllenger:SetText( "��ս���᣺"..GameClanManager:clanGetClanNameByWid( saleInfo.AtkClanWid ) );
				if t_strongStateUV[stateIndex].name ~= "ս����" then
					local mainplayer = ActorMgr:getMainPlayer()
					--�����Լ�����
					if GameClanManager:clanGetClanNameByWid( saleInfo.AtkClanWid ) ~= mainplayer:getClanName() then
						WarForcesFrameStrongInfoBtnStrongChanllenger:SetText( "��ս���᣺������ս��..." );
					end
					--���Լ����ᣬ�����Լ����ǹ����Ա
					if GameClanManager:clanGetClanNameByWid( saleInfo.AtkClanWid ) == mainplayer:getClanName() and mainplayer:getClanPosition() == CLAN_POS_NORMAL then
						WarForcesFrameStrongInfoBtnStrongChanllenger:SetText( "��ս���᣺������ս��..." );
					end
				end
			end
			
		end
	end
	
	WarForcesFrameStrongInfoBtnStrongLV:SetText( "�ݵ�ȼ���"..GameClan_BaseWar:getStrongPointLVByMapID( mapID ).."���ݵ�" );
	WarForcesFrameStrongInfoBtnStrongPrice:SetText( "���ĵ׼ۣ�"..GameClan_BaseWar:getStrongPointBaseUseByMapID( mapID ).."�����ʽ�" );
	if GameClanManager:clanGetClanNameByWid( occupInfo.ClanWid ) == nil or  GameClanManager:clanGetClanNameByWid( occupInfo.ClanWid ) == "" then return end
	WarForcesFrameStrongInfoBtnStrongOwnerClan:SetText( "ռ�칫�᣺"..GameClanManager:clanGetClanNameByWid( occupInfo.ClanWid ) );
	local fengHao = ActorMgr:getClanFengHao( occupInfo.ClanWid )
		if string.find( fengHao, "����", 1, true ) then
			WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetTextColor( 109, 0, 210 )
		elseif string.find( fengHao, "����", 1, true ) then
			WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetTextColor( 24, 101, 6 )
		elseif string.find( fengHao, "����", 1, true ) then
			WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetTextColor( 0, 94, 190 )
		elseif string.find( fengHao, "����", 1, true ) then
			WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetTextColor( 255, 78, 0 )
		end
	WarForcesFrameStrongInfoBtnStrongOwnerClanFengHao:SetText( fengHao );
	WarForcesFrameStrongInfoBtnStrongOwnerClanPresentName:SetText( "����᳤��"..GameClanManager:GetClanCaptionNameByWid( occupInfo.ClanWid ) );
	WarForcesFrameStrongInfoBtnStrongOwnerClanLV:SetText( "����ȼ���"..GameClanManager:GetClanLVByWid(occupInfo.ClanWid) );
	WarForcesFrameStrongInfoBtnStrongOwnerClanMemberNum:SetText( "��Ա������"..GameClanManager:GetClanMemNumByWid(occupInfo.ClanWid) );
end
function WarForceBtn_OnClick()
	CUR_SELECT_STONG_INDEX = this:GetClientID();
	GetStrongAllInfo();
	if MapAwardOreShowFrame:IsShown() then
		MapAwardOreShowFrame:Hide();
	end
end
function CenterControlInfoBtnOwnerAwardClanMoneyBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "�ݵ㽱�������ڿ��״̬����ȡ" )
		return
	end
	--GameClan_BaseWar:RequestAward( 0, 1 );
	--��������ȷ�Ͽ�
	if GetTotalAwardMoney() == 0 then
		ShowMidTips( "û�п���ȡ��ռ���ʽ�" )
		return
	end
	OwnerAwardClanMoneyInfoFrame:Show()
	OwnerAwardClanMoneyInfoFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	local szInfo = string.format( "����ȡռ���ʽ�#G%d����#n\n������ռ���#c03ffff�ݵ�ȼ�#nԽ�ߣ�ռ���#c03ffff�ݵ�����#nԽ���ý���Խ�ߣ�ÿ��ֻ����ȡһ�Σ�",
								GetTotalAwardMoney() )	
	OwnerAwardClanMoneyInfoFrameText:setCenterLine( true )	
	OwnerAwardClanMoneyInfoFrameText:SetText( szInfo, 255,255,190 )	
end
function CenterControlInfoBtnOwnerFashionableDressBtn_OnClick()
	local desMapID = 4404
	local desNpcID = 307008
	MapShower:GetMaxMapContal():linkMoveto( desMapID, desNpcID );
end
function CenterControlInfoBtnOwnerPersonalMountBtn_OnClick()
	local desMapID = 4404
	local desNpcID = 307008
	MapShower:GetMaxMapContal():linkMoveto( desMapID, desNpcID );
end
function CenterControlInfoBtnMasterStatueBtn_OnClick()
	local desMapID = 1000
	local desNpcID = 307009
	MapShower:GetMaxMapContal():linkMoveto( desMapID, desNpcID );
end
function CenterControlInfoBtnGotoCenterControlBtn_OnClick()
	local desMapID = 1000
	local desNpcID = 307007
	MapShower:GetMaxMapContal():linkMoveto( desMapID, desNpcID );
end
function CenterControlInfoBtnEnerCityWarBtn_OnClick()
	GameClan_BaseWar:EnterStrongWar( 0 );
end
function StrongInfoBtnOwnerAwardClanMoneyBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "�ݵ㽱�������ڿ��״̬����ȡ" )
		return
	end
	--GameClan_BaseWar:RequestAward( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID, 1 );
	--��������ȷ�Ͽ�
	if GetTotalAwardMoney() == 0 then
		ShowMidTips( "û�п���ȡ��ռ���ʽ�" )
		return
	end
	OwnerAwardClanMoneyInfoFrame:Show()
	OwnerAwardClanMoneyInfoFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	local szInfo = string.format( "����ȡռ���ʽ�#G%d����#n\n������ռ���#c03ffff�ݵ�ȼ�#nԽ�ߣ�ռ���#c03ffff�ݵ�����#nԽ���ý���Խ�ߣ�ÿ��ֻ����ȡһ�Σ�",
								GetTotalAwardMoney() )
	OwnerAwardClanMoneyInfoFrameText:setCenterLine( true )	
	OwnerAwardClanMoneyInfoFrameText:SetText( szInfo, 255,255,190 )	
end
function StrongInfoBtnAwardBtn_OnEnter()
	local szInfo = "\t\t\t#G["..t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapName.."]#n�ݵ�ռ�콱��\nռ���ÿ�տɻ��:\n1.����(�᳤����ȡ��ÿ�տ���ȡһ��)\n2.������Դ(ռ����Զ�����������)"
	SetGameTooltips( this:GetName(), szInfo )
end
function StrongInfoBtnOwnerAwardBufferBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "�ݵ㽱�������ڿ��״̬����ȡ" )
		return
	end
	GameClan_BaseWar:RequestAward( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID, 3 );
end
function StrongInfoBtnOwnerAwardContriBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "�ݵ㽱�������ڿ��״̬����ȡ" )
		return
	end
	GameClan_BaseWar:RequestAward( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID, 4 );
end
function StrongInfoBtnClanResBtn_OnClick()
	if MapAwardOreShowFrame:IsShown() then
		MapAwardOreShowFrame:Hide();
	else
		local awardOreDef = GameClan_BaseWar:getMapAwardOreDef( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID );
		ShowMapAwardOreFrame( awardOreDef );
	end
end
function StrongInfoBtnJoinBaseWarBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬�޷����оݵ����" );
		return;
	end
	if GameClan_BaseWar:getAllStrongInfoNum() <= 0 then return end
	for i = 1, GameClan_BaseWar:getAllStrongInfoNum() do
		local strongSaleInfo = GameClan_BaseWar:getStrongSaleInfoByIndex( i - 1 );
		if strongSaleInfo.MapID == t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID then
			GameClan_BaseWar:setStrongBaseWarSaleInfoByIndex( i - 1 );
			ClanStrongInfoFrame:Show();
		end
	end
end
function StrongInfoBtnGoToBaseWarMapBtnBtn_OnClick()
	local desMapID = t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID
	local desNpcID = t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].npcID
	MapShower:GetMaxMapContal():linkMoveto( desMapID, desNpcID );
end
function StrongInfoBtnEnterBaseWarBtn_OnClick()
	local hasEnteredID = GameClan_BaseWar:getStrongWarTodayEnteredID()
	if hasEnteredID == 0 then
		MessageBox( "", "���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����" )
		MessageBoxFrame:SetClientUserData(0, t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID );
		MessageBoxFrame:SetClientString( "�ݵ�ȷ�Ͽ�" )
	elseif hasEnteredID == t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID then
		GameClan_BaseWar:EnterStrongWar( t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID );
	elseif hasEnteredID ~= t_WarForcesStrongInfos[CUR_SELECT_STONG_INDEX].mapID then
		local enteredStrongName = GameClan_BaseWar:getStrongWarTodayEnteredStrongName();
		ShowMidTips( "�����Ѳμ�"..enteredStrongName.."ս�����ɽ��뵱ǰ�ݵ�ս��" )
	end
end
function OwnerAwardClanMoneyInfoFrameAcceptBtnOnClick()
	GameClan_BaseWar:RequestAward( 0, 0 );
	OwnerAwardClanMoneyInfoFrame:Hide()
end
-------------------------------��ͼռ�콱�����----------------
local szMapAwardOreInfo = [[#c00ff00 �������������
#c00ff00  1.ռ��ݵ�
#c00ff00  2.������ʱ���Զ�����������]]
local MAP_AWARD_ORE_LIST_LUA = 8
local szMapAward = "\n";
function ShowMapAwardOreFrame( awardOreDef )
	if awardOreDef == nil then return end
	--���ý���
	szMapAward = "";
	local awardNum = 1;
	for i = 1, MAP_AWARD_ORE_LIST_LUA do
		local ore = awardOreDef.Ore[i-1];
		if ore.OreID ~= 0 and ore.Num ~= 0 then
			local itemDef = getItemDef( ore.OreID );
			szMapAward = szMapAward.."\t\t".. itemDef.Name.."\t\t\t\t\t\t".. ore.Num .."��\n"
			awardNum = awardNum + 1;
		end
	end
	MapAwardOreShowFrame:SetSize( 266, 105 + 16*( awardNum - 1 ) )
	MapAwardOreShowFrameAwardBK:SetSize( 252, 16*awardNum );
	MapAwardOreShowFrameAward:SetText( szMapAward, 255,255,255 )
	MapAwardOreShowFrame:Show();
	MapAwardOreShowFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
end
function MapAwardOreShowFrame_OnLoad()
	MapAwardOreShowFrameAward:SetSize( 252, 176 );
	MapAwardOreShowFrameAwardInfo:SetText( szMapAwardOreInfo, 255,255,255 );
end
function MapAwardOreShowFrame_OnUpdate()
end
-------------------��ɱ���--------------------
--������GetClientUserData,1��ʾ��ɳ��������ս����2��ʾ̹�˴�ս,3��ʾ����ݵ�ս������ֵ��ʾ�ݵ�ս
BATTLE_CONTINE_KILL_TYPE = 1
TANK_CONTINE_KILL_TYPE = 2
SPAN_STRONG_CONTINE_KILL_TYPE = 3
local t_NumberTexUv = {
	[1] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="0",		Y="0",		W="70",		H="80", },
	[2] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="70",		Y="0",		W="70",		H="80", },
	[3] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="140",	Y="0",		W="70",		H="80", },
	[4] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="210",	Y="0",		W="70",		H="80", },
	[5] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="280",	Y="0",		W="70",		H="80", },
	[6] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="0",		Y="80",		W="70",		H="80", },
	[7] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="70",		Y="80",		W="70",		H="80", },
	[8] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="140",	Y="80",		W="70",		H="80", },
	[9] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="210",	Y="80",		W="70",		H="80", },
	[0]	= { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga",	X="280",	Y="80",		W="70",		H="80", },
	["��ɱ"] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga", X="0",	Y="160",	W="260",	H="130", },
	["˫ɱ"] = { path = "uires\\ShuZiXiangGuanZiYuan\\new\\JiShaShu.tga", X="260",	Y="160",	W="230",	H="130", },
}

local alpha_time = 0

function StrongWarContineKillFrame_OnLoad()
	this:setUpdateTime( 0.1 )
	StrongWarContineKillFrameKillNum1:SetBlendAlpha(0)
	StrongWarContineKillFrameKillNum2:SetBlendAlpha(0)
	StrongWarContineKillFrameKillNum3:SetBlendAlpha(0)
	StrongWarContineKillFrameKillTex:SetBlendAlpha(0)
	StrongWarContineKillFrameKillBack1:SetBlendAlpha(0)	
	StrongWarContineKillFrameKillBack2:SetBlendAlpha(0)	
end

function StrongWarContineKillFrame_OnHide()
	this:SetClientUserData(0,0)
	this:SetClientUserData(1,0)
	StrongWarContineKillFrameKillNum1:SetBlendAlpha(0)
	StrongWarContineKillFrameKillNum2:SetBlendAlpha(0)
	StrongWarContineKillFrameKillNum3:SetBlendAlpha(0)
	StrongWarContineKillFrameKillTex:SetBlendAlpha(0)
	StrongWarContineKillFrameKillBack1:SetBlendAlpha(0)	
	StrongWarContineKillFrameKillBack2:SetBlendAlpha(0)	
end

function SetContineKillTexForStrongWar(ContineKill)
	if math.floor(ContineKill/100) ~= 0 then
		StrongWarContineKillFrameKillBack1:Show()
		StrongWarContineKillFrameKillBack2:Hide()
		local KillNumOne = math.floor(ContineKill/100)
		StrongWarContineKillFrameKillNum1:SetTexture( t_NumberTexUv[KillNumOne].path );
		StrongWarContineKillFrameKillNum1:SetTexUV( t_NumberTexUv[KillNumOne].X, t_NumberTexUv[KillNumOne].Y, t_NumberTexUv[KillNumOne].W, t_NumberTexUv[KillNumOne].H );
		local KillNumTwo = math.floor(ContineKill%100/10)
		StrongWarContineKillFrameKillNum2:SetTexture( t_NumberTexUv[KillNumTwo].path );
		StrongWarContineKillFrameKillNum2:SetTexUV( t_NumberTexUv[KillNumTwo].X, t_NumberTexUv[KillNumTwo].Y, t_NumberTexUv[KillNumTwo].W, t_NumberTexUv[KillNumTwo].H );
		local KillNumThree = ContineKill%100%10
		StrongWarContineKillFrameKillNum3:SetTexture( t_NumberTexUv[KillNumThree].path );
		StrongWarContineKillFrameKillNum3:SetTexUV( t_NumberTexUv[KillNumThree].X, t_NumberTexUv[KillNumThree].Y, t_NumberTexUv[KillNumThree].W, t_NumberTexUv[KillNumThree].H );
		StrongWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["��ɱ"].X, t_NumberTexUv["��ɱ"].Y, t_NumberTexUv["��ɱ"].W, t_NumberTexUv["��ɱ"].H );
	elseif math.floor(ContineKill%100/10) ~= 0 then
		StrongWarContineKillFrameKillBack1:Hide()
		StrongWarContineKillFrameKillBack2:Show()
		StrongWarContineKillFrameKillNum1:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local KillNumTwo = math.floor(ContineKill%100/10)
		StrongWarContineKillFrameKillNum2:SetTexture( t_NumberTexUv[KillNumTwo].path );
		StrongWarContineKillFrameKillNum2:SetTexUV( t_NumberTexUv[KillNumTwo].X, t_NumberTexUv[KillNumTwo].Y, t_NumberTexUv[KillNumTwo].W, t_NumberTexUv[KillNumTwo].H );
		local KillNumThree = ContineKill%100%10
		StrongWarContineKillFrameKillNum3:SetTexture( t_NumberTexUv[KillNumThree].path );
		StrongWarContineKillFrameKillNum3:SetTexUV( t_NumberTexUv[KillNumThree].X, t_NumberTexUv[KillNumThree].Y, t_NumberTexUv[KillNumThree].W, t_NumberTexUv[KillNumThree].H );
		StrongWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["��ɱ"].X, t_NumberTexUv["��ɱ"].Y, t_NumberTexUv["��ɱ"].W, t_NumberTexUv["��ɱ"].H );
	elseif ContineKill%100%10 ~= 0 then
		StrongWarContineKillFrameKillBack1:Hide()
		StrongWarContineKillFrameKillBack2:Hide()
		StrongWarContineKillFrameKillNum1:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		StrongWarContineKillFrameKillNum2:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		local KillNumThree = ContineKill%100%10
		if KillNumThree == 2 then
			StrongWarContineKillFrameKillNum3:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			StrongWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["˫ɱ"].X, t_NumberTexUv["˫ɱ"].Y, t_NumberTexUv["˫ɱ"].W, t_NumberTexUv["˫ɱ"].H );
		else
			StrongWarContineKillFrameKillNum3:SetTexture( t_NumberTexUv[KillNumThree].path );
			StrongWarContineKillFrameKillNum3:SetTexUV( t_NumberTexUv[KillNumThree].X, t_NumberTexUv[KillNumThree].Y, t_NumberTexUv[KillNumThree].W, t_NumberTexUv[KillNumThree].H );
			StrongWarContineKillFrameKillTex:SetTexUV( t_NumberTexUv["��ɱ"].X, t_NumberTexUv["��ɱ"].Y, t_NumberTexUv["��ɱ"].W, t_NumberTexUv["��ɱ"].H );
		end
	end
end

function StrongWarContineKillFrame_OnUpdate()
	local oldContineKill = this:GetClientUserData(0)
	local newContineKill = 0
	if this:GetClientUserData(1) == BATTLE_CONTINE_KILL_TYPE then
		newContineKill = GameBattle:getMainPlayerCurKill();
	elseif this:GetClientUserData(1) == TANK_CONTINE_KILL_TYPE then
		newContineKill = GamePoly:getTankContineKill();
	elseif this:GetClientUserData(1) == SPAN_STRONG_CONTINE_KILL_TYPE then
		newContineKill = GameClan_SpanBaseWar:getSpanStrongSelfContinueKill();
	else
		newContineKill = GameClan_BaseWar:getStrongWarSelfCurContineKill();
	end
	this:SetClientUserData(0,newContineKill)
	if newContineKill ~= oldContineKill and newContineKill >= 2 then
		--������ɱ��ĿͼƬ
		SetContineKillTexForStrongWar(newContineKill)
		StrongWarContineKillFrameKillNum1:SetBlendAlpha( 1 )
		StrongWarContineKillFrameKillNum2:SetBlendAlpha( 1 )
		StrongWarContineKillFrameKillNum3:SetBlendAlpha( 1 )
		StrongWarContineKillFrameKillTex:SetBlendAlpha( 1 )
		StrongWarContineKillFrameKillBack1:SetBlendAlpha( 1 )
		StrongWarContineKillFrameKillBack2:SetBlendAlpha( 1 )
		StrongWarContineKillFrame:SetSize( 280*1.2, 90*1.25 )
		alpha_time = os.clock()
		return
	end
	if alpha_time > 0 and os.clock() - alpha_time < 5 then
		StrongWarContineKillFrame:SetSize( 280, 90 )
		local alpha = 1 - 0.2*( os.clock() - alpha_time )
		StrongWarContineKillFrameKillNum1:SetBlendAlpha( alpha )
		StrongWarContineKillFrameKillNum2:SetBlendAlpha( alpha )
		StrongWarContineKillFrameKillNum3:SetBlendAlpha( alpha )
		StrongWarContineKillFrameKillTex:SetBlendAlpha( alpha )
		StrongWarContineKillFrameKillBack1:SetBlendAlpha( alpha )
		StrongWarContineKillFrameKillBack2:SetBlendAlpha( alpha )
	else
		StrongWarContineKillFrameKillNum1:SetBlendAlpha(0)
		StrongWarContineKillFrameKillNum2:SetBlendAlpha(0)
		StrongWarContineKillFrameKillNum3:SetBlendAlpha(0)
		StrongWarContineKillFrameKillTex:SetBlendAlpha(0)
		StrongWarContineKillFrameKillBack1:SetBlendAlpha(0)	
		StrongWarContineKillFrameKillBack2:SetBlendAlpha(0)	
	end
end
------------------�ݵ�սƯ��ͼ��------------
function StrongWarBegin()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_STRONG_WAR ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_STRONG_WAR, frame = "StrongWarInfoFrame", 
												path ="uires\\ui\\Za\\ZhanZhengXinXi.tga", index = 0 } );
	end
	--��ʾ��ɱ���
	StrongWarContineKillFrame:Show()
end
function StrongWarEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_STRONG_WAR );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	SetStrongWarResult();
	--��ʾ��ɱ���
	StrongWarContineKillFrame:Hide()
end
------------------�ݵ�ս��Ϣ���-------------
local t_StrongWarInfo = {
	[1] = [[#ca0a05a�������������ɻ��ս������:
#cff0000	1.�ܻ�ɱ�������ܺϳ���40
#cff0000	2.����������0/20
#cff0000	3.����ս����������20����]],
	[2] = [[#ca0a05aս��������ս������ʱ����Զ����
#cffffbe1.ʤ���������Ա��ÿ�˻��2222354����
#cffffbe2.ʧ�ܷ������Ա��ÿ�˻��2254534����]],
}

local BASE_WAR_MIN_WIN_TIME = 20

local t_StrongWarPlayersInfo = {}
local t_StrongWarSelfInfo = {}
local sortRule = "resGetNum"
local sortRequest = true
local MAX_SHOW_PLAYER_NUM = 12

local t_StrongWarInfoFrameRemindTexUV = {
	["finish"] = { 
	uv = { w = 24, h = 21, x = 576, y = 78,},
	},
	["unFinish"] = { 
	uv = { w = 25, h = 22, x = 604, y = 78, },
	 },	
}
local t_StrongWarTitleRules = {
	"name", "clan", "race", "kill", "contineKill", "assists", "resGetNum",
}

function GetPlayersData()
	t_StrongWarPlayersInfo = {}
	local playersNum = GameClan_BaseWar:getStrongWarPlayerNum()
	for i = 1, playersNum do
		local playerInfo = {
			name = GameClan_BaseWar:getStrongWarPlayerName( i - 1 ),
			clan = GameClan_BaseWar:getStrongWarPlayerClanName( i - 1 ),
			race = GetCareerName( GameClan_BaseWar:getStrongWarPlayerRaceId( i - 1 ) ),
			kill = GameClan_BaseWar:getStrongWarPlayerKill( i - 1 ),
			contineKill = GameClan_BaseWar:getStrongWarPlayerMaxContineKill( i - 1 ),
			assists = GameClan_BaseWar:getStrongWarPlayerAssist( i -1 ),
			resGetNum = GameClan_BaseWar:getStrongWarPlayerResGetNum( i - 1 ),
		}
		table.insert( t_StrongWarPlayersInfo, playerInfo );
	end
	local selPassTime = GameClan_BaseWar:getStrongWarSelfEnterTime()
	t_StrongWarSelfInfo = {
			kill = GameClan_BaseWar:getStrongWarSelfKill(),
			contineKill = GameClan_BaseWar:getStrongWarSelfMaxContineKill(),
			assists = GameClan_BaseWar:getStrongWarSelfAssist(),
			resGetNum = GameClan_BaseWar:getStrongWarSelfResGetNum(),
			enterTime = math.floor(selPassTime/60),
	}
	StrongWarInfoFrameSelfInfokillNum:SetText("�ܻ�ɱ��"..t_StrongWarSelfInfo["kill"])
	StrongWarInfoFrameSelfInfoContineKill:SetText("�����ɱ��"..t_StrongWarSelfInfo["contineKill"])
	StrongWarInfoFrameSelfInfoAssist:SetText("������"..t_StrongWarSelfInfo["assists"])
	StrongWarInfoFrameSelfInfoResGetNum:SetText("��Դ�����������"..t_StrongWarSelfInfo["resGetNum"])
	StrongWarInfoFrameSelfInfoEnterTime:SetText("����ս��ʱ�䣺"..t_StrongWarSelfInfo["enterTime"].."����")
	local Info = "#ca0a05a�������������ɻ��ս������:\n"
	if (t_StrongWarSelfInfo["kill"] + t_StrongWarSelfInfo["assists"]) < 40 then
		local uv = t_StrongWarInfoFrameRemindTexUV["unFinish"]["uv"]
		StrongWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#cff0000	1.�ܻ�ɱ�������ܺϳ���40\n"
	else
		local uv = t_StrongWarInfoFrameRemindTexUV["finish"]["uv"]
		StrongWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#c00ff00	1.�ܻ�ɱ�������ܺϳ���40\n"
	end
	if GameClan_BaseWar:getStrongWarSelfDie() < 20 then
		local uv = t_StrongWarInfoFrameRemindTexUV["unFinish"]["uv"]	
		StrongWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#cff0000	2.����һ�ɱ"..GameClan_BaseWar:getStrongWarSelfDie().."/20��\n"
	else
		local uv = t_StrongWarInfoFrameRemindTexUV["finish"]["uv"]
		StrongWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#c00ff00	2.����һ�ɱ20��\n"
	end
	if t_StrongWarSelfInfo["enterTime"] < BASE_WAR_MIN_WIN_TIME then
		local uv = t_StrongWarInfoFrameRemindTexUV["unFinish"]["uv"]	
		StrongWarInfoFrameRemindInfoNeed3:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#cff0000	3.����ս����������20����"
	else
		local uv = t_StrongWarInfoFrameRemindTexUV["finish"]["uv"]
		StrongWarInfoFrameRemindInfoNeed3:SetTexUV( uv.x, uv.y, uv.w, uv.h )
		Info = Info.."#c00ff00	3.����ս����������20����"
	end
	StrongWarInfoFrameRemindInfoRemind1:SetText( Info, 255, 255, 0 );
end

function SorStrongWarPlayersInfo( sortRequest, sortRule )
	if sortRequest then
		table.sort( t_StrongWarPlayersInfo, function( a, b )
			return a[sortRule] > b[sortRule]
		end)
	else
		table.sort( t_StrongWarPlayersInfo, function( a, b )
			return a[sortRule] < b[sortRule]
		end)
	end
end

function UpdatePlayersInfo()
	--���֮ǰ��ʾ��
	for i = 1, MAX_SHOW_PLAYER_NUM do
		local btn = getglobal( "StrongWarInfoFramePlayersInfoPlayerInfo"..i )
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
	if table.getn( t_StrongWarPlayersInfo ) > MAX_SHOW_PLAYER_NUM then
		StrongWarInfoFramePlayersInfoScrollBar:Show()
		StrongWarInfoFramePlayersInfoScrollBar:SetMaxValue( table.getn( t_StrongWarPlayersInfo ) - MAX_SHOW_PLAYER_NUM )
	else
		StrongWarInfoFramePlayersInfoScrollBar:Hide()
		StrongWarInfoFramePlayersInfoScrollBar:SetMaxValue(0);
	end
	StrongWarInfoFramePlayersInfoScrollBar:SetValue(math.min(StrongWarInfoFramePlayersInfoScrollBar:GetMaxValue(), StrongWarInfoFramePlayersInfoScrollBar:GetValue()))
	local endIndex = math.min( table.getn( t_StrongWarPlayersInfo ), MAX_SHOW_PLAYER_NUM + StrongWarInfoFramePlayersInfoScrollBar:GetValue() )
	for i = 1 + StrongWarInfoFramePlayersInfoScrollBar:GetValue(), endIndex do
		local color;
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer:getName() == t_StrongWarPlayersInfo[i]["name"] then
			color = { 0, 255, 0, }
		else 
			color = { 190, 190, 90, }
		end
		local btn = getglobal( "StrongWarInfoFramePlayersInfoPlayerInfo"..(i - StrongWarInfoFramePlayersInfoScrollBar:GetValue()) )
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( t_StrongWarPlayersInfo[i]["name"] )
		NameFont:SetTextColor( color[1], color[2], color[3] )
		local clanName = getglobal( btn:GetName().."Clan" )
		clanName:SetText( t_StrongWarPlayersInfo[i]["clan"] )
		clanName:SetTextColor( color[1], color[2], color[3] )
		local RaceName = getglobal( btn:GetName().."Race" )
		RaceName:SetText( t_StrongWarPlayersInfo[i]["race"] )
		RaceName:SetTextColor( color[1], color[2], color[3] )
		local killNumFont = getglobal( btn:GetName().."killNum" )
		killNumFont:SetText( t_StrongWarPlayersInfo[i]["kill"] )
		killNumFont:SetTextColor( color[1], color[2], color[3] )
		local ContineKillFont = getglobal( btn:GetName().."ContineKill" )
		ContineKillFont:SetText( t_StrongWarPlayersInfo[i]["contineKill"] )
		ContineKillFont:SetTextColor( color[1], color[2], color[3] )
		local AssistFont = getglobal( btn:GetName().."Assist" )
		AssistFont:SetText( t_StrongWarPlayersInfo[i]["assists"] )
		AssistFont:SetTextColor( color[1], color[2], color[3] )
		local GetResNumFont = getglobal( btn:GetName().."GetResNum" )
		GetResNumFont:SetText( t_StrongWarPlayersInfo[i]["resGetNum"] )		
		GetResNumFont:SetTextColor( color[1], color[2], color[3] )
	end
end

function PlayersInfoChanges()	
	--��ȡ��Ϣ
	GetPlayersData();
	--����
	SorStrongWarPlayersInfo( sortRequest, sortRule );
	--��ʾ
	UpdatePlayersInfo();
end
	
function StrongWarInfoFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	StrongWarInfoFrameWinInfoResult:SetText( "" )
	StrongWarInfoFrameWinInfoGetMoney:SetText( "" )
	StrongWarInfoFrameWinInfoGetExp:SetText( "" )
	StrongWarInfoFrameWinInfoGetFont:SetText( "" )
	StrongWarInfoFrameWinInfoGetGift:SetText( "" )
	StrongWarInfoFramePlayersInfoScrollBar:SetValue(0);
	StrongWarInfoFrameRemindInfoRemind1:SetText( t_StrongWarInfo[1], 255, 255, 0 );
	StrongWarInfoFrameRemindInfoRemind2:SetText( "", 255, 255, 0 );
	local uv = t_StrongWarInfoFrameRemindTexUV["unFinish"]["uv"]
	StrongWarInfoFrameRemindInfoNeed1:SetTexUV( uv.x, uv.y, uv.w, uv.h )	
	StrongWarInfoFrameRemindInfoNeed2:SetTexUV( uv.x, uv.y, uv.w, uv.h )
	--��ȡ��Ϣ
	GetPlayersData();
	--����
	SorStrongWarPlayersInfo( sortRequest, sortRule );
	--��ʾ
	UpdatePlayersInfo();
	local winnerMoneyType = GameClan_BaseWar:getStrongWarWinnerMoneyType()
	local winnerMoney = GameClan_BaseWar:getStrongWarWinnerMoney()
	local failMoneyType = GameClan_BaseWar:getStrongWarFailMoneyType()
	local failMoney = GameClan_BaseWar:getStrongWarFailMoney()
	local winnerItemDef = getItemDef( GameClan_BaseWar:getStrongWarWinnerItemID() )
	local failItemDef = getItemDef( GameClan_BaseWar:getStrongWarFailItemID() )
	local szAward = "#ca0a05aս��������ս������ʱ����Զ����\n#cffffbe1.ʤ���������Ա��ÿ�˻��%d%s��\n\t%s����������\n#cffffbe2.ʧ�ܷ������Ա��ÿ�˻��%d%s��\n\t%s����������"	
	StrongWarInfoFrameRemindInfoRemind2:SetText( string.format( szAward, winnerMoney, GetMoneyDesc(winnerMoneyType), winnerItemDef.Name, failMoney, GetMoneyDesc(failMoneyType), failItemDef.Name ), 255, 255, 0 );
end
function StrongWarInfoFramePlayersInfo_ScrollUpBtn()
	local slider = StrongWarInfoFramePlayersInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
	UpdatePlayersInfo()
end
function StrongWarInfoFramePlayersInfo_ScrollDownBtn()
	local slider	= StrongWarInfoFramePlayersInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
	UpdatePlayersInfo()
end
function StrongWarInfoFramePlayersInfo_ScrollBarValueChange()
	UpdatePlayersInfo();
end
function StrongWarItemTitle_OnClick()
	local titleIndex = this:GetClientID();	
	if t_StrongWarTitleRules[titleIndex] == sortRule then
		sortRule = t_StrongWarTitleRules[titleIndex]
		sortRequest = not sortRequest
	else
		sortRule = t_StrongWarTitleRules[titleIndex]
		sortRequest = true
	end	
	--��ȡ��Ϣ
	GetPlayersData();
	--����
	SorStrongWarPlayersInfo( sortRequest, sortRule );
	--��ʾ
	UpdatePlayersInfo();
end
function SetStrongWarResult()
	if not StrongWarInfoFrame:IsShown() then
		StrongWarInfoFrame:Show();
	end
	StrongWarInfoFrameLeave:Hide();
	--��ȡ��ҽ��ս���ľݵ�mapID
	local enterMapID = GameClan_BaseWar:getStrongWarTodayEnteredID();
	local mainplayer = ActorMgr:getMainPlayer()	
	if GameClan_BaseWar:getCurMapStrongOWnerName( enterMapID ) == mainplayer:getClanName() then
		StrongWarInfoFrameWinInfoResult:SetText( " ʤ��" )
		StrongWarInfoFrameWinInfoGetFont:SetText( "�ѻ�����½���" )
		local winnerMoneyType = GameClan_BaseWar:getStrongWarWinnerMoneyType()
		local winnerMoney = GameClan_BaseWar:getStrongWarWinnerMoney()
		StrongWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 200, 0 )	
		StrongWarInfoFrameWinInfoGetMoney:SetText(string.format( "���ң�%d%s", winnerMoney, GetMoneyDesc(winnerMoneyType) ))
		local winnerExp = GameClan_BaseWar:getStrongWarWinnereExp();
		local mainplayer = ActorMgr:getMainPlayer()
		local levelupDef = getLevelUP( mainplayer:getLv() )
		local getExp = math.floor( levelupDef.ContributeExp * ( 1.0 * winnerExp / 10000 ) )
		StrongWarInfoFrameWinInfoGetExp:SetTextColor( 255, 200, 0 )	
		StrongWarInfoFrameWinInfoGetExp:SetText(string.format( "���飺%d", getExp))
		local winnerItemDef = getItemDef( GameClan_BaseWar:getStrongWarWinnerItemID() )
		StrongWarInfoFrameWinInfoGetGift:SetText( string.format( "%s(�ʼ�����)", winnerItemDef.Name ) )
	else
		StrongWarInfoFrameWinInfoResult:SetText( " ʧ��" )
		StrongWarInfoFrameWinInfoGetFont:SetText( "�ѻ�����½���" )
		local failMoneyType = GameClan_BaseWar:getStrongWarFailMoneyType()
		local failMoney = GameClan_BaseWar:getStrongWarFailMoney()
		StrongWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 200, 0 )
		StrongWarInfoFrameWinInfoGetMoney:SetText( string.format( "���ң�%d%s", failMoney, GetMoneyDesc( failMoneyType ) ) )
		local failExp = GameClan_BaseWar:getStrongWarFailExp();
		local mainplayer = ActorMgr:getMainPlayer()
		local levelupDef = getLevelUP( mainplayer:getLv() )
		local getExp = math.floor( levelupDef.ContributeExp * ( 1.0 * failExp / 10000 ) )
		StrongWarInfoFrameWinInfoGetExp:SetTextColor( 255, 200, 0 )	
		StrongWarInfoFrameWinInfoGetExp:SetText(string.format( "���飺%d", getExp))
		local failItemDef = getItemDef( GameClan_BaseWar:getStrongWarFailItemID() )
		StrongWarInfoFrameWinInfoGetGift:SetText( string.format( "%s(�ʼ�����)", failItemDef.Name ) )
	end
	--û�����������������ʾ�Ľ�Ǯ����������ʾ
	if GameClan_BaseWar:getStrongWarSelfDie() < 20 and ( GameClan_BaseWar:getStrongWarSelfKill() + GameClan_BaseWar:getStrongWarSelfAssist() ) < 40 and t_StrongWarSelfInfo["enterTime"] < BASE_WAR_MIN_WIN_TIME then
		StrongWarInfoFrameWinInfoGetMoney:SetText( "δ�����ý�������" )
		StrongWarInfoFrameWinInfoGetMoney:SetTextColor( 255, 0, 0 )
		StrongWarInfoFrameWinInfoGetFont:SetText( "" )
		StrongWarInfoFrameWinInfoGetExp:SetText( "" )
		StrongWarInfoFrameWinInfoGetGift:SetText( "" )
	end
end
function StrongWarInfoFrameLeave_OnClick()
	GameClan_BaseWar:LeaveStrongWar();
	StrongWarInfoFrame:Hide();
end

----------------------------��������ս����ҫ-----------------------------------------
local t_BannerGlory = {
					[1] = [[
					
					
#c461e0a	����ƣ�ս����ҫ#c554623(����)
#c461e0a	����װ�ã�������Ŧ
#c461e0a	����ȼ�����10��]],
					[2] = [[#c461e0a	��פ����[��������]�£����ɻ�ô�������
#c461e0a	��פ��ʱ��Խ�����ɻ�þ���Խ��

#c461e0a	�����ʸ񣺾ݵ㹫��ɿ���
#c461e0a	�μ��ʸ������˿ɲμ�
#c461e0a	����ʱ�䣺ÿ��[��������]ÿ�γ���10����
#c461e0a	����λ�ã�
#c554623	  ����1���ݵ㣺#L����@@[302141,2202]#n%s
#c554623	  ����1���ݵ㣺#L����ѩԭ@@[302140,2200]#n%s
#c554623	  ����1���ݵ㣺#L����ɳĮ@@[302142,2300]#n%s
#c554623	  ����2���ݵ㣺#L���ջ�Ұ@@[302143,2400]#n%s
#c554623	  ����2���ݵ㣺#LԽ������@@[302144,2500]#n%s
#c554623	  ����3���ݵ㣺#L��ɳ�ݵ�@@[302145,3404]#n%s
#c461e0a	��[������Ŧ]�ɴ��͵��ѿ�����[��������]��

#c0a6400	��ܰ��ʾ��
#c0a6400	ǰ���ѿ�����[��������],���ܳɹ���þ���]],
					[3] = [[#c0a6400(��ڼ������µ��˶��ɻ�ý���)]],
}
local t_StrongBannerGloryState = {
	[2202]={ false,"#c554623(δ����)" },
	[2200]={ false,"#c554623(δ����)" },
	[2300]={ false,"#c554623(δ����)" },
	[2400]={ false,"#c554623(δ����)" },
	[2500]={ false,"#c554623(δ����)" },
	[3404]={ false,"#c554623(δ����)" },
}
local AWARD_ITEM_ID = 5510000
function ClanBannerGloryBegin()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_BANNER_GLORY ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_BANNER_GLORY, frame = "StrongWarInfoFrame", 
												path ="uires\\ui\\Za\\ZhanZhengXinXi.tga", index = 0 } );
	end
end
function ClanBannerGloryEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_BANNER_GLORY );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	ClanBannerGloryFrame:Hide();
end
function SetClanBannerGloryInfo()
	local strongGloryInfos = GameClan_BaseWar:getStrongBannerGloryInfo()
	local isAnyBannerGloryOpen = false
	for i = 1, strongGloryInfos.Num do
		local info = strongGloryInfos.Glory[i-1]
		--�ж�״̬
		if info.Open == 1 then
			isAnyBannerGloryOpen = true
			--������¿�����
			if not t_StrongBannerGloryState[info.MapID][1] then
				local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
				--�������Ư�����ݣ����ٴ���ʾ��˸��Ч��û�У���Ʈ��������ʾ
				local index = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_BANNER_GLORY )
				if index == 0 then
					t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_BANNER_GLORY, frame = "StrongWarInfoFrame", 
															path ="uires\\ui\\Za\\ZhanZhengXinXi.tga", index = 0 } );
				else
					local btn = getglobal( "XinShouCenterGuideButton"..index );
					local TipsUVAnimTex = getglobal( btn:GetName() .. "UVAnimationTex" )
					if TipsUVAnimTex ~= nil then
						TipsUVAnimTex:Show();
					end
				end
				t_StrongBannerGloryState[info.MapID][1] = true
				t_StrongBannerGloryState[info.MapID][2] = "#c0a6400(�ѿ���)"
			end
		else
			local time = os.date( "*t", info.StartTime )
			local serverTime = os.date( "*t", getServerTime().sec )
			t_StrongBannerGloryState[info.MapID][1] = false
			--�����ͬһ�����ʾ�ѽ������������ʾδ����
			if time.year == serverTime.year and time.month == serverTime.month and time.day == serverTime.day then
				t_StrongBannerGloryState[info.MapID][2] = "#c808080(�ѽ���)"
			else
				t_StrongBannerGloryState[info.MapID][2] = "#c554623(δ����)"
			end
		end
	end
	--���û���κ�ս���ǿ����ģ���������ʾ��ս���Ư������
	if not isAnyBannerGloryOpen then
		ClanBannerGloryEnd()
	end
	--��ս��״̬��Ϣ������ʾ
	local szInfo = string.format( t_BannerGlory[2],t_StrongBannerGloryState[2202][2],t_StrongBannerGloryState[2200][2],
	t_StrongBannerGloryState[2300][2],t_StrongBannerGloryState[2400][2],t_StrongBannerGloryState[2500][2],t_StrongBannerGloryState[3404][2] )
	ClanBannerGloryFrameRichRole:SetText( szInfo,255,255,255 )
	SetStrongBannerGloryTime()
end
function SetStrongBannerGloryTime()
	ClanBannerGloryFrameEndTime:SetText( "" );
	ClanBannerGloryFrameEndTimeFont:SetText( "" )
end
function ClanBannerGloryFrame_OnLoad()
	ClanBannerGloryFrameRichInfo:SetText( t_BannerGlory[1],255,255,255 )
	local szInfo = string.format( t_BannerGlory[2],t_StrongBannerGloryState[2202][2],t_StrongBannerGloryState[2200][2],
	t_StrongBannerGloryState[2300][2],t_StrongBannerGloryState[2400][2],t_StrongBannerGloryState[2500][2],t_StrongBannerGloryState[3404][2] )
	ClanBannerGloryFrameRichRole:SetText( szInfo,255,255,255 )
	local itemdef				= getItemDef( AWARD_ITEM_ID );
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	ClanBannerGloryFrameRichAward:SetText( t_BannerGlory[3],255,255,255 )
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ClanBannerGloryFrameAwardItemBtnIconTexture:SetTexture(IconPath)
	
	this:RegisterEvent( "GE_UPDATE_GETROLELIST" )
end
function ClanBannerGloryFrame_OnEvent()
	if arg1 == "GE_UPDATE_GETROLELIST" then
		t_StrongBannerGloryState = {
			[2202]={ false,"#R(δ����)#n" },
			[2200]={ false,"#R(δ����)#n" },
			[2300]={ false,"#R(δ����)#n" },
			[2400]={ false,"#R(δ����)#n" },
			[2500]={ false,"#R(δ����)#n" },
			[3404]={ false,"#R(δ����)#n" },
		}
	end
end
function ClanBannerGloryFrame_OnUpdate()
	SetStrongBannerGloryTime();
end
function ClanBannerGloryFrameAwardItemBtn_OnEnter()
	local itemdef				= getItemDef( AWARD_ITEM_ID );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end
