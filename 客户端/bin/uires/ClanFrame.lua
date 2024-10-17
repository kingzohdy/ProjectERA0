--local MAXCLANLISTROW		= 15;
local MAXCLANMEMROW		= 14;
local MAXAPPLYJOINCLANROW	= 12;
local MAXDIPLOMATISM		= 10;
local MAXDIPLOMATISMLIST	= 8;
local MAXASSISTNUM		= 2;
local MAX_BUILDING_NUM		= 10;
local NpcId;
MAX_CLAN_LEVEL			= 5; -- �������ߵȼ�
MAX_CLAN_LIST_NUM               = 15;
CLAN_MAX_BUILDING_NUM		= 4;
MIN_LVEL_CREATE_CLAN		= 30;

--@Desc:�����Աְ��
CLAN_POS_NOT_MEMBER = 0 	--/* �ǻ�Ա */
CLAN_POS_NORMAL = 1		--/* ��Ա */
CLAN_POS_TECH_OFFICER = 2 	--/* �Ƽ���Ա */
CLAN_POS_COLLECTION_OFFICER = 3 --/* �ɼ���Ա */
CLAN_POS_FACTORY_OFFICER = 4 	--/* ������Ա */
CLAN_POS_PET_OFFICER = 5 	--/* �����Ա */
CLAN_POS_WALL_OFFICER = 6 	--/* ������Ա */
CLAN_POS_VICE_PRESIDENT = 7 	--/* ���᳤ */
CLAN_POS_PRESIDENT = 8	 	--/* �᳤ */
CLAN_POS_GROUP_1 = 9	--/* 1ս���ų� */
CLAN_POS_GROUP_2 = 10	--/* 2ս���ų� */
CLAN_POS_GROUP_3 = 11	--/* 3ս���ų� */
CLAN_POS_GROUP_4 = 12	--/* 4ս���ų� */
CLAN_POS_GROUP_5 = 13 	--/* 5ս���ų� */
CLAN_POS_GROUP_6 = 14	--/* 6ս���ų� */


local bClanFrameCareerDesc = 1;
local bClanFrameNickNameDesc = false;
local bClanFrameContributeDesc = false;
local bClanFrameOnlineDesc = false;
local bClanFramePosDesc = false;
local bClanFramelvDesc = false;
local bClanFrameNameDesc = false;
local bClanFrameColorDesc = false;

local SelectColors = {
	{"��ɫ", 0xFF007DFF},
	{"��ɫ", 0xFFFF0000},
	{"��ɫ", 0xFFFF7D00},
	{"��ɫ", 0xFFFF00FF},
	{"��ɫ", 0xFF00FF00},
	{"��ɫ", 0xFF969696},
}

local ColorsIndex = {}

for i, t in ipairs(SelectColors) do
	ColorsIndex[t[2]] = {i, t[1]}
end

local LabelList = {}

--##	XML_page: 1338-1972		##
--##	�����б����			##
--##	LUA_page: 12-277		##

local t_clanListInfo = { totalPage = 0, curClanListPage = 0, selectClan = { index = 0, clanID = 0 } };

function ClanListFrame_OnLoad()
	for i = 1, MAX_CLAN_LIST_NUM do
		local btn = getglobal( "ClanListInfo" ..i );
		btn:SetOverlayRegion( -2, -5, 319, 28 );
	end
end

--@Desc:�����б�����ʱ��Ӧ
function ClanListFrame_OnShow()
	-- ��������:����鿴�����һ����Ϣ
	t_clanListInfo.isSearch					= false;
	t_clanListInfo["curClanListPage"]		= ONEPAGE;
	t_clanListInfo["selectClan"]["clanID"]	= 0
	this:SetPoint( "center", "$parent", "center", 0, 0 );
	bClanListIDDesc = false;
	bClanListNameDesc = false;
	bClanListLevelDesc = false;
	bClanListMemberDesc = false;
	ClanList_SearchClanBtn:Enable();
	-- ��������ʾ���������
	updataClanPage();
end

function ClanListFrame_OnHide()
	NpcId = nil
end

function ClearClanListInfo()
	for i = 1, MAXCLANLISTROW do
		local btn			= getglobal( "ClanListInfo"..i );
		local EnemyClanTex	= getglobal( "ClanListInfo"..i.."EnemyTex" );
		local ClanIdFont	= getglobal( "ClanListInfo"..i.."ClanIdFont" );
		local ClanNameFont	= getglobal( "ClanListInfo"..i.."ClanNameFont" );
		local ClanLvFont	= getglobal( "ClanListInfo"..i.."ClanLvFont" );
		local ClanNumFont	= getglobal( "ClanListInfo"..i.."ClanMemNumFont" );
		local ClanColor		= getglobal( "ClanListInfo"..i.."ClanColor" );
		local ClanColorFrame= getglobal( "ClanListInfo"..i.."ClanColorFrame" );
		EnemyClanTex:Hide();
		ClanIdFont:SetText("");
		ClanNameFont:SetText("");
		ClanLvFont:SetText("");
		ClanNumFont:SetText("");
		ClanColor:Hide();
		ClanColorFrame:Hide();
		btn:Disable();
	end
end

function updataSearchClan()
	t_clanListInfo.isSearch = true;
	updataClanPage();
end

--4.6�������˸��µ���ɫ����152��166�У�173��174�ǰ�ť������
--@Desc:ˢ�¹����ѯҳ�棬����( ��ǰҳ�� )
function updataClanPage()
	ClearClanListInfo();
	ClearSelectClanInfo();
	DisCheckAllBtn( "ClanListInfo", MAXCLANLISTROW );
	ClanListFrame_ClanColor:Hide()
	ClanList_ColorChose:Hide()
	
	local nClanTotalNum = 0;
	if t_clanListInfo.isSearch then		 
		nClanTotalNum = GameClanManager:getQueryClanTotalNum();
	else
		nClanTotalNum	= GameClanManager:getClanTotalNum();
	end

	if nClanTotalNum == 0 then
		return;
	end

	-- ����״̬:������ҳ��������
	t_clanListInfo["totalPage"] = math.ceil( nClanTotalNum / MAXCLANLISTROW );
	ClanListFramePageFont:SetText( "��" .. t_clanListInfo["curClanListPage"] .. "/" .. t_clanListInfo["totalPage"] .. "ҳ" );

	for i = 1, MAXCLANLISTROW do
		local OneClanInfo =  GameClanManager:getOneClanInfoList( t_clanListInfo["curClanListPage"] - 1, i - 1, t_clanListInfo.isSearch );
		
		if OneClanInfo == nil then
			break;
		end
		local btn	= getglobal( "ClanListInfo"..i );
		BondingClanListFrame( OneClanInfo, btn );
		local fonts = {
			getglobal( "ClanListInfo"..i.."ClanIdFont" ),
			getglobal( "ClanListInfo"..i.."ClanNameFont" ),
			getglobal( "ClanListInfo"..i.."ClanLvFont" ),
			getglobal( "ClanListInfo"..i.."ClanMemNumFont" ),
		}
		for _, font in ipairs(fonts) do
			font:SetTextColor(255, 254, 189)
		end
		if GameClanManager:hasClanColor(OneClanInfo.m_ClanInfo.m_uClanId) then
			local OneClanColor = GameClanManager:getClanColor(OneClanInfo.m_ClanInfo.m_uClanId);
			if ColorsIndex[OneClanColor] then
				local clanColorTex		= getglobal( "ClanListInfo"..i.."ClanColor" )
				local clanColorFrame	= getglobal( "ClanListInfo"..i.."ClanColorFrame" )
				--������ɫ
				local r, g, b = (OneClanColor / 65536) % 256, (OneClanColor / 256) % 256, OneClanColor % 256
				clanColorTex:SetColor(r, g, b)
				clanColorTex:Show()
				clanColorFrame:Show()
				for _, font in ipairs(fonts) do
					font:SetTextColor(r, g, b)
				end
			end
		end
	end

	-- ��ûѡ���κι��ᣬ��ˢ����ר����Ϣ
	if t_clanListInfo["selectClan"]["clanID"] == 0 then		
		return;
	end
	-- ˢ�¹����б���Ϣ��Ҫˢ�±�ѡ���Ĺ����ר����Ϣ
	local selClanInfo	= GameClanManager:getOneClanInfoList( t_clanListInfo["curClanListPage"] -1, t_clanListInfo["selectClan"]["index"], t_clanListInfo.isSearch );
	UpdateSelectClanInfo( selClanInfo );
end

function ClearSelectClanInfo()
	local t_selectClanInfoFonts =	{
										"ClanListFrame_NameFont",			"ClanListPresidentNameFont",		"ClanListCoPresidentName1Font",
										"ClanListFrame_ClanLevelFont",		"ClanListFrame_MoneyFont",			"ClanListFrame_NumPeopleFont",
										"ClanListFrame_ActiveFont",			"ClanListFrame_BuildingValueFont",	"ClanListFrame_CityLevelFont",
									};

	for _, name in ipairs( t_selectClanInfoFonts ) do
		local font = getglobal( name );
		font:SetText( "" );
	end

	local t_selectClanInfoRichs	= { "ClanListManifestoRich" };
	for _, name in ipairs( t_selectClanInfoRichs ) do
		local rich = getglobal( name );
		rich:Clear();
	end
	
	local t_selectClanInfoBtns = { "ClanList_EnterClanCity" };
	for _, name in ipairs( t_selectClanInfoBtns ) do
		local btn = getglobal( name );
		btn:Hide();
	end
end

--4.6��ɫ��ť����ʾ206��207
function UpdateSelectClanInfo( selClanInfo )
	ClearSelectClanInfo();
	-- ˢ�¹����б���Ϣ��Ҫˢ�±�ѡ���Ĺ����ר����Ϣ
	ClanListFrame_ClanColor:Show()		
	if selClanInfo == nil then
		return;
	end

	local t_selClanInfo =	{
								["ClanListFrame_NameFont"]			= { value = selClanInfo.m_ClanInfo.m_szClanName },								
								["ClanListPresidentNameFont"]		= { value = selClanInfo.m_ClanBrief.Leader.m_szRoleName },								
								["ClanListFrame_ClanLevelFont"]		= { value = selClanInfo.m_ClanInfo.m_nClanLevel },
								["ClanListFrame_NumPeopleFont"]		= { value = selClanInfo.m_ClanInfo.m_nClanMemNum },
								["ClanListFrame_ActiveFont"]		= { value = selClanInfo.m_ClanInfo.m_nActiveValue },
								["ClanListFrame_MoneyFont"]			= { value = selClanInfo.m_ClanInfo.m_nMoney },
								["ClanListFrame_BuildingValueFont"]	= { value = selClanInfo.m_ClanInfo.m_nBuildingValue.."/10000" },
								["ClanListFrame_CityLevelFont"]		= { value = selClanInfo.m_cityInfo.Level.."��" },
							};

	for name, data in pairs( t_selClanInfo ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	if selClanInfo.m_ClanBrief.AssistNum > 0 then
		ClanListCoPresidentName1Font:SetText( selClanInfo.m_ClanBrief.Assists[0].m_szRoleName );
	end

	-- ���ù�������
	local t_selClanRich = { ["ClanListManifestoRich" ] = { value = selClanInfo.m_ClanBrief.XuanGao, color = { r = 255, g = 254, b = 189 } } };
	for name, data in pairs( t_selClanRich ) do
		local rich = getglobal( name );
		rich:SetText( data.value, data.color.r, data.color.g, data.color.b );
	end

	local btn = getglobal( "ClanListInfo" .. t_clanListInfo["selectClan"]["index"] + 1 );
	btn:Checked();
	
	if selClanInfo.m_cityInfo.Level == 0 then
		ClanList_EnterClanCity:Disable()
	else
		ClanList_EnterClanCity:Enable()
	end
	if NpcId then
		ClanList_EnterClanCity:Show()
	end
end

--@Desc:�󶨹�����ϸ��Ϣ( ��ǰ�� )
function BondingClanListFrame( OneClanInfo, button )
	if OneClanInfo == nil then
		return;
	end
	
	local t_ClanListFonts = {
								[button:GetName() .. "ClanIdFont"]		= { value = OneClanInfo.m_ClanInfo.m_uClanId },
								[button:GetName() .. "ClanNameFont"]	= { value = OneClanInfo.m_ClanInfo.m_szClanName },	
								[button:GetName() .. "ClanLvFont"]		= { value = OneClanInfo.m_ClanInfo.m_nClanLevel },	
								[button:GetName() .. "ClanMemNumFont"]	= { value = OneClanInfo.m_ClanInfo.m_nClanMemNum },	
							}
	for name, data in pairs( t_ClanListFonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end
	if GameClanManager:isEnemyClan(OneClanInfo.m_ClanInfo.m_clanLongId) then
		local tex = getglobal(button:GetName().."EnemyTex")
		tex:Show()
	end

	button:Enable();
end

--4.6��ɫ��ť������Ƶ��������ʾ��Ϣ
function ClanList_ClanColorBtn_OnEnter()
	SetGameTooltips(this:GetName(), "���ù�����ɫ")
end

--4.6��ɫ��ť�����ʾѡ�����
function ClanList_ClanColorBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local position = mainplayer:getClanPosition()
	if position == CLAN_POS_VICE_PRESIDENT or position == CLAN_POS_PRESIDENT then
		if t_clanListInfo["selectClan"]["clanID"] ~= 0 then
			local selectClanInfo = GameClanManager:getOneClanInfoList(t_clanListInfo["curClanListPage"] - 1, t_clanListInfo["selectClan"]["index"], t_clanListInfo.isSearch)
			local color = GameClanManager:getDefaultClanColor(selectClanInfo.m_ClanInfo.m_clanLongId)
			ClanList_ColorChoseColorDefaultTex:SetColor((color / 65536) % 256, (color / 256) % 256, color % 256)
			ClanList_ColorChose:Show()
		end
	else
		ShowMidTips("ֻ�л᳤�����᳤�ſɽ��д˲���")
	end
end

function ClanList_ColorChose_OnLoad()
	for i, t in ipairs(SelectColors) do
		local btn = getglobal("ClanList_ColorChoseColor"..i)
		btn:SetText(t[1])
		local tex = getglobal("ClanList_ColorChoseColor"..i.."Tex")
		tex:SetColor((t[2] / 65536) % 256, (t[2] / 256) % 256, t[2] % 256)
	end
end

function ClanList_ColorChose_OnShow()
	SetFocusFrame(this)
end

function ClanList_ColorChose_OnFocusLost()
	this:Hide()
end

--4.6��ɫѡ��Ĺرհ�ť
function ClanList_ClanColorClose_OnClick()
	ClanList_ColorChose:Hide()
end

function ClanList_ColorChoseSelect_OnClick()
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		return;
	end
	local id = this:GetClientID()
	if id == 0 then
		GameClanManager:clan_Opt_SetClanColor(t_clanListInfo["selectClan"]["clanID"], 0)
	else
		GameClanManager:clan_Opt_SetClanColor(t_clanListInfo["selectClan"]["clanID"], SelectColors[id][2])
	end
	ClanList_ColorChose:Hide()
end

function ClanListSortClanColorBtn_OnEnter()
	SetGameTooltips(this:GetName(), "��������ɫ����")
end

--4.6��ɫ����ť�����Ӧ
function ClanListSortClanColorBtn_OnClick()
	bClanFrameColorDesc = not bClanFrameColorDesc;
	if bClanFrameColorDesc then
		GameClanManager:sortOtherClan(OTHER_CLAN_SORT_COLOR, 1, t_clanListInfo.isSearch);
	else
		GameClanManager:sortOtherClan(OTHER_CLAN_SORT_COLOR, -1, t_clanListInfo.isSearch);
	end
	updataClanPage();
end

function ClanList_EnterClanCity_OnEnter()
	SetGameTooltips(this:GetName(), "����ɽ���˹���ĳ���")
end

function ClanList_EnterClanCity_OnClick()
	local id = t_clanListInfo["selectClan"]["clanID"]
	if id == 0 then
		return
	end
	GameClanManager:clan_Opt_EnterCity(id)
end

local bClanListIDDesc = false;
local bClanListNameDesc = false;
local bClanListLevelDesc = false;
local bClanListMemberDesc = false;

--����id ����
function ClanListSortClanIDBtn_OnClick()
	bClanListIDDesc = not bClanListIDDesc ;
	if bClanListIDDesc then
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_ID, 1,t_clanListInfo.isSearch );
	else
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_ID, -1,t_clanListInfo.isSearch );
	end
	updataClanPage();
end
--����������
function ClanListSortClanNameBtn_OnClick()
	bClanListNameDesc = not bClanListNameDesc ;
	if bClanListNameDesc then
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_NAME, 1,t_clanListInfo.isSearch );
	else
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_NAME, -1,t_clanListInfo.isSearch );
	end
	updataClanPage();
end

function ClanListSortClanLevelBtn_OnClick()
	bClanListLevelDesc = not bClanListLevelDesc ;
	if bClanListLevelDesc then
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_LEVEL, 1,t_clanListInfo.isSearch );
	else
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_LEVEL, -1,t_clanListInfo.isSearch );
	end
	updataClanPage();
end

function ClanListSortClanMemberBtn_OnClick()
	bClanListMemberDesc = not bClanListMemberDesc ;
	if bClanListMemberDesc then
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_NUM, 1,t_clanListInfo.isSearch );
	else
		GameClanManager:sortOtherClan( OTHER_CLAN_SORT_NUM, -1,t_clanListInfo.isSearch );
	end
	updataClanPage();
end

--@Desc:��������б���ʱ��Ӧ
function ClanListFontBtn_OnClick()
	t_clanListInfo["selectClan"]["index"]	= this:GetClientID() - 1;
	local OneClanInfo = GameClanManager:getOneClanInfoList( ( t_clanListInfo["curClanListPage"] - 1 ), t_clanListInfo["selectClan"]["index"], t_clanListInfo.isSearch );
	t_clanListInfo["selectClan"]["clanID"]	= OneClanInfo.m_ClanInfo.m_uClanId;
	DisCheckAllBtn( "ClanListInfo", MAXCLANLISTROW );
	this:Checked();
	-- ˢ�¹����б���Ϣ��Ҫˢ�±�ѡ���Ĺ����ר����Ϣ
	local selClanInfo	= GameClanManager:getOneClanInfoList( t_clanListInfo["curClanListPage"] -1, t_clanListInfo["selectClan"]["index"], t_clanListInfo.isSearch );
	UpdateSelectClanInfo( selClanInfo );
end

function ClanListFontBtn_OnEnter()
	local tex = getglobal(this:GetName().."EnemyTex")
	if tex and tex:IsShown() then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�ж�״̬", 
								frame = this:GetParent(), button = "cursor" } );
	end
end

--@Desc:������빫��
function ClanList_ApplyJionClanBtn_OnClick()
	-- ��û��ѡ���κι��ᣬ������������κι���
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		return;
	end

	-- ���ж��������Ѿ����빫��
	if IsInClan() then
		ShowMidTips( "���Ѿ�����һ�����ᣬ���������������һ������" );
		return;
	end

	local OneClanInfo = GameClanManager:getOneClanInfoList( ( t_clanListInfo["curClanListPage"] - 1 ), t_clanListInfo["selectClan"]["index"], t_clanListInfo.isSearch );
	if OneClanInfo == nil then
		return;
	end
	
	local clanInfo = OneClanInfo.m_ClanInfo;
	GameClanManager:clan_Opt_ApplyJoin( clanInfo.m_clanLongId.nHeight,
										clanInfo.m_clanLongId.nLow,
										clanInfo.m_uClanId,
										clanInfo.m_szClanName );
end

--@Desc:����ͬ�˰�ť
function ClanList_ApplyJoinUnionBtn_OnClick()	
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		return;
	end	
	if not IsInClan() then
		ShowMidTips( "�㻹û���빫�ᣬû��Ȩ�޽��иò���" );
		return;
	end
	local t_canApplyJoinPositions = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canApplyJoinPositions ) do
		if position == mainplayer:getClanPosition() then
			local OneClanInfo = GameClanManager:getOneClanInfoList( (t_clanListInfo["curClanListPage"]-1), t_clanListInfo["selectClan"]["index"],t_clanListInfo.isSearch );
			if OneClanInfo == nil then
				return;
			end			
			local clanInfo = OneClanInfo.m_ClanInfo;
			GameClanManager:clan_Opt_ApplyJoinUnion(	clanInfo.m_clanLongId.nHeight,
														clanInfo.m_clanLongId.nLow,
														clanInfo.m_uClanId,
														clanInfo.m_szClanName );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ������ͬ��" );
end

--@Desc:����ͬ�˰�ť
function ClanList_InviteJoinUnionBtn_OnClick()
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		return;
	end	
	if not IsInClan() then
		ShowMidTips( "�㻹û���빫�ᣬû��Ȩ�޽��иò���" );
		return;
	end
	local t_canApplyJoinPositions = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canApplyJoinPositions ) do
		if position == mainplayer:getClanPosition() then
			local OneClanInfo = GameClanManager:getOneClanInfoList( (t_clanListInfo["curClanListPage"]-1), t_clanListInfo["selectClan"]["index"],t_clanListInfo.isSearch );
			if OneClanInfo == nil then
				return;
			end
			local clanInfo = OneClanInfo.m_ClanInfo;
			GameClanManager:clan_Opt_InviteJoinUnion(	clanInfo.m_clanLongId.nHeight,
														clanInfo.m_clanLongId.nLow,
														clanInfo.m_clanInfo.m_uClanId,
														clanInfo.m_szClanName );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ������ͬ��" );
end

--@Desc:�˳�ͬ�˰�ť
function ClanList_ExitUnionBtn_OnClick()
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		return;
	end	
	if not IsInClan() then
		ShowMidTips( "�㻹û���빫�ᣬû��Ȩ�޽��иò���" );
		return;
	end
	local t_canApplyJoinPositions = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canApplyJoinPositions ) do
		if position == mainplayer:getClanPosition() then
			local OneClanInfo = GameClanManager:getOneClanInfoList( (t_clanListInfo["curClanListPage"]-1), t_clanListInfo["selectClan"]["index"],t_clanListInfo.isSearch );
			if OneClanInfo == nil then
				return;
			end			
			local clanInfo = OneClanInfo.m_ClanInfo;
			GameClanManager:clan_Opt_ExitUnion(		clanInfo.m_clanLongId.nHeight,
													clanInfo.m_clanLongId.nLow,
													clanInfo.m_uClanId,
													clanInfo.m_szClanName );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ��ȡ��ͬ�˹�ϵ" );
end

--@Desc:��ӵжԹ��ᰴť
function ClanList_SetEnemyBtn_OnClick()
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		ShowMidTips( "û��ѡ�й��ᣬ�޷���ս" );
		return;
	end
	if not IsInClan() then
		ShowMidTips( "�㻹û���빫�ᣬû��Ȩ�޽��иò���" );
		return;
	end
	local t_cannotApplyJoinPositions = { CLAN_POS_NOT_MEMBER,CLAN_POS_NORMAL };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_cannotApplyJoinPositions ) do
		if position == mainplayer:getClanPosition() then
			ShowMidTips( "��û��Ȩ����ӵжԹ���" );
			return;
		end
	end
	local OneClanInfo = GameClanManager:getOneClanInfoList( (t_clanListInfo["curClanListPage"]-1), t_clanListInfo["selectClan"]["index"],t_clanListInfo.isSearch );
	if OneClanInfo == nil then
		return;
	end
	local clanInfo = OneClanInfo.m_ClanInfo;
	local ClanDiplomatism = GameClanManager:getClanDipLomatism();
	for i=1,ClanDiplomatism.m_nEnemyNum do
		if ClanDiplomatism.m_Enemys[i-1].m_uClanId == clanInfo.m_uClanId then
			ShowMidTips("���ɶ�" .. ClanDiplomatism.m_Enemys[i-1].m_szClanName .."�ظ���ս" );
			return;
		end
	end
	MessageBox( "������ս","������ս����" .. CLAN_WAR_COST_TOLUA .. "���ң�����������Сʱ��ս״̬" );
	MessageBoxFrame:SetClientString( clanInfo.m_szClanName );
	MessageBoxFrame:SetClientUserData( 0, clanInfo.m_clanLongId.nHeight );
	MessageBoxFrame:SetClientUserData( 1, clanInfo.m_clanLongId.nLow )
	MessageBoxFrame:SetClientUserData( 2, clanInfo.m_uClanId );
end

--@Desc:ɾ���жԹ���
function ClanList_CancleEnemyBtn_OnClick()
	if t_clanListInfo["selectClan"]["clanID"] == 0 then
		return;
	end	
	if not IsInClan() then
		ShowMidTips( "�㻹û���빫�ᣬû��Ȩ�޽��иò���" );
		return;
	end
	local t_canApplyJoinPositions = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canApplyJoinPositions ) do
		if position == mainplayer:getClanPosition() then
			local OneClanInfo = GameClanManager:getOneClanInfoList( (t_clanListInfo["curClanListPage"]-1), t_clanListInfo["selectClan"]["index"],t_clanListInfo.isSearch );
			if OneClanInfo == nil then
				return;
			end
			local clanInfo = OneClanInfo.m_ClanInfo;
			GameClanManager:clan_Opt_ExitEnemy(	clanInfo.m_clanLongId.nHeight,
												clanInfo.m_clanLongId.nLow,
												clanInfo.m_uClanId,
												clanInfo.m_szClanName );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ��ɾ���жԹ���" );
end

----------------------------------------------------------------------------------------------------------------

local 
function ClanFramesCloseSubFrame()
	local t_CloseFrames	= {
		"ClanLevelUpFrame",		"Clan_EditFrame",				"ClanFrame_ConfirmFrame",
		"ApplyJoinClanFrame",	"ClanDiplomatismFrame",			"Clan_SetNiChengInputFrame",
	};
	for i = 1, table.getn( t_CloseFrames ) do
		local frame = getglobal( t_CloseFrames[i] );
		frame:Hide();
	end
end

--##	XML_page: 4-940		 ##
--##	�����Ա����		 ##
--##	LUA_page: 278-674	 ##

local ONLINE			= 1;
local NOTONLINE			= 0;
local t_clanFrameSelectMember = { heightId = 0, lowId = 0, name = "" };

local tPageData = {};
tPageData.nCurPage = 1;
tPageData.nTotalPage = 1;

local bSearchMember = false;
local bHasNewMemberJoin = false;
local bHasRelateChnage = false;
local nClanLearnCurPage, nClanLearnMaxPage = 1, 1
local LearnType = ""
local MAX_CLAN_SKILL_TYPE = 4;
local MAX_CLAN_SKILL_NUM = 4;

local function ClanSkillFilter(id, level)
	if ClanFrameTechOnlyCanLearn:GetCheckState() then
		return CanLearnClanSkill(id, level)
	end
	return true
end

function ClanFrameSetMaxPage()
	local ClanViewer = GameClanManager:getClanViewer();
	local nMaxNum = 0;
	if bSearchMember and ClanFrameListSearchNameEdit:GetText() ~= "" then
		local name = ClanFrameListSearchNameEdit:GetText();
		for i=1,ClanViewer.m_clanInfo.m_nClanMemNum do
			if string.find( ClanViewer.m_Members[i-1].RoleMember.m_szRoleName,name,1,true ) then
				nMaxNum = nMaxNum + 1;
			end
		end
	else
		nMaxNum = ClanViewer.m_clanInfo.m_nClanMemNum;
	end
	
	tPageData.nTotalPage = math.floor( (nMaxNum - 1)/MAXCLANMEMROW ) + 1;
	if tPageData.nTotalPage == 0 then
		tPageData.nTotalPage = 1;
	end
	if tPageData.nCurPage > tPageData.nTotalPage then
		tPageData.nCurPage = tPageData.nTotalPage;
	end
	UpdateClanMemberInfo();
end

local function UpdateClanFrameTech()
	nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameUpdateSkill(LearnType, "ClanSkillInfo", "ClanFrameTechLearnBtn", "ClanFrameTechScrollBar", nClanLearnCurPage, ClanSkillFilter)
	ClanFrameTechPageText:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage)
end

local function ClanTechTypeChange(btn)
	for i=1, MAX_CLAN_SKILL_TYPE do
		local btn = getglobal("ClanFrameTechTypeBtn"..i)
		btn:DisChecked()
	end
	btn:Checked()
	local font = getglobal(btn:GetName().."NameFont")
	LearnType = font:GetText()
	UpdateClanFrameTech()
end

function ClanFrame_OnLoad()
	this:RegisterEvent("GE_CLAN_UPDATEMEMBER");
	this:RegisterEvent("GE_CLAN_SVR_INFO");
	this:RegisterEvent("GE_CLAN_DISABLEFRAME");
	this:RegisterEvent("GE_CLAN_CITY_SVR_INFO");
	this:RegisterEvent("GE_CLAN_NOTIFY_MEMBER");
	this:RegisterEvent("GE_CLAN_NOTIFY_DIPLOMATISM");
	this:RegisterEvent("GE_CLANLIST_UPDATE");
	this:RegisterEvent("GE_CHG_CLAN_MEMBER_NAME");
	this:RegisterEvent("GE_CLAN_BUILDING_COMPLETE");
	this:RegisterEvent("GE_CLAN_RESEARCH_COMPLATE");
	this:RegisterEvent("GE_CLAN_ADVANCE_UPDATE");
	this:RegisterEvent("GE_CLAN_MAKING_UPDATE");
	this:RegisterEvent("GE_CLAN_STORAGE_UPDATE");
	
	for i = 1, MAXCLANMEMROW do
		local btn = getglobal("ClanFrameListItem"..i)
		if i % 2 == 0 then
			btn:SetNormalTextureTemplate("WhiteCategoryBkg")
			btn:SetDisableTextureTemplate("WhiteCategoryBkg")
		else
			btn:SetNormalTextureTemplate("BlackCategoryBkg")
			btn:SetDisableTextureTemplate("BlackCategoryBkg")
		end
	end
	InitClanTechTypeBtn("ClanFrameTechTypeBtn");
	for i=1,MAX_CLAN_SKILL_NUM do
		local tex = getglobal( "ClanSkillInfo"..i.."TexBtnTex" );
		tex:setMask("uires\\ui\\mask\\GongHuiYuanXingTuBiaoBiaoShi  mask.tga");
	end
	do
		local i = 1
		while true do
			local btn = getglobal("ClanFrameMapResource"..i)
			if not btn then
				break
			end
			if i % 2 == 0 then
				btn:SetNormalTextureTemplate("WhiteCategoryBkg")
				btn:SetDisableTextureTemplate("WhiteCategoryBkg")
			else
				btn:SetNormalTextureTemplate("BlackCategoryBkg")
				btn:SetDisableTextureTemplate("BlackCategoryBkg")
			end
			i = i + 1
		end
	end
end

function ClanFrame_OnEvent()
	if arg1 == "GE_CHG_CLAN_MEMBER_NAME" then
		if ClanFrameList:IsShown() then	
			UpdateClanMemberInfo();
		end
		if ClanGetResFrame:IsShown() then
			SetGetResInfo()
		end
		if ClanDonateLogFrame:IsShown() then
			UpdatePageInfo()
		end
	elseif arg1 == "GE_CLAN_BUILDING_COMPLETE" or arg1 == "GE_CLAN_RESEARCH_COMPLATE" then
		if ClanFrameMap:IsShown() then
			ReSetClanMapInfo();
		end
	elseif arg1 == "GE_CLAN_UPDATEMEMBER" then
		local ClanViewer	= GameClanManager:getClanViewer();
		SetClanBtnState();
		if ClanFrameList:IsShown() then
			ClanFrameSetMaxPage();
		end
	elseif arg1 == "GE_CLAN_SVR_INFO" then
		local ClanViewer	= GameClanManager:getClanViewer();
		local cityInfo		= GameClanManager:getClanCity():getCityInfo();
		if ClanFrame:IsShown() then
			BondingClanDefInfo( ClanViewer, cityInfo );	
			SetClanBtnState();
		end
		if ClanFrameList:IsShown() then
			ClanFrameSetMaxPage();
		end
		if ClanFrameProduct:IsShown() then
			timer(UpdateClanFrameProductCategoryList)
		end
	elseif arg1 == "GE_CLAN_CITY_SVR_INFO" then
		local cityInfo	= GameClanManager:getClanCity():getCityInfo();
		if ClanFrame:IsShown() then
			ClanFrameBasicInfoCityLevel:SetText( cityInfo.m_nLevel );
		end
	elseif arg1 == "GE_CLAN_DISABLEFRAME" then
		ClanFrame:Hide();
		Clan_SetNiChengInputFrame:Hide();
		Clan_EditFrame:Hide();
		ClanDiplomatismFrame:Hide();
		ClanFrame_ConfirmFrame:Hide();
		ClanSetPositionFrame:Hide();
		ClanSetPosDropDownFrame:Hide();
		ResetWorldCityFightCaptureBuildOwner();
		if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "��ɢ����" then
			MessageBoxFrame:Hide();
		end
		if ClanMemberSummonFrame:IsShown() then
			ClanMemberSummonFrame:Hide();
		end
	elseif arg1 == "GE_CLAN_NOTIFY_MEMBER" then
		GuideEffectShow( "", "", "ClanButton", "�����������빫��", "uires\\ZhuJieMian\\1.dds", 
							352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
		bHasNewMemberJoin = true;
		if ClanFrame:IsShown() then
			ClanFrameReserveMember_Texture:Show();
			ClanFrameReserveMember_Texture:SetUVAnimation(50,true);
		end
	elseif arg1 == "GE_CLAN_NOTIFY_DIPLOMATISM" then
		GuideEffectShow( "", "", "ClanButton", "�й�������ͬ�˻�ж�", "uires\\ZhuJieMian\\1.dds", 
							352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
		bHasRelateChnage = true;
		if ClanFrame:IsShown() then
			ClanFrameRelation_Texture:Show();
			ClanFrameRelation_Texture:SetUVAnimation(50,true);
		end
	elseif arg1 == "GE_CLANLIST_UPDATE" then
		if ClanListFrame:IsShown() then
			updataClanPage();
		end
	elseif arg1 == "GE_CLAN_ADVANCE_UPDATE" then
		if ClanFrameTech:IsShown() then
			UpdateClanFrameTech()
		end
	elseif arg1 == "GE_CLAN_STORAGE_UPDATE" then
		if ClanFrameMap:IsShown() then
			timer(UpdateClanMapResource)
		end
		if ClanFrameProduct:IsShown() then
			timer(UpdateClanFrameProductSelectData)
			timer(UpdateClanFrameProductCategoryList)
		end
	elseif arg1 == "GE_CLAN_MAKING_UPDATE" then
		if ClanFrameProduct:IsShown() then
			SetClanFrameProductLabelList()
			UpdateClanFrameProductSelectData()
		end
		--���ڼ�鹫��ʣ���ʽ��Ƿ�����֧����һ�ο۷�
		CheckMoneyForNextCost()
	end
end

--@Desc:�����������ʱ��Ӧ
function ClanFrame_OnShow()
	--�رպ��ٴ򿪣����ԭ���Ĳ�����ʾ
	bSearchMember = false
	ClanFrameListSearchNameEdit:SetText("")
	tPageData.nCurPage = 1
	
	GameClanManager:clan_Opt_GetClanInfo();
	ClanFrameList_OnTab();
	SetClanBtnState();
	t_clanFrameSelectMember = { heightId = 0, lowId = 0, name = "" };
	local ClanViewer	= GameClanManager:getClanViewer();
	--�Ϸ��� �������
	if string.find( ClanViewer.m_clanInfo.m_szClanName,"*",1,true ) and GetPlayPositionInClan() == CLAN_POS_PRESIDENT then
		ContainerChangeRoleFrame:SetClientUserData( 0,0 );
		ContainerChangeRoleFrame:Show();
		ClanFrame:Hide();
		return;
	end
	
	if bHasNewMemberJoin then
		ClanFrameReserveMember_Texture:Show();
		ClanFrameReserveMember_Texture:SetUVAnimation(50,true);
	end
	if bHasRelateChnage then
		ClanFrameRelation_Texture:Show();
		ClanFrameRelation_Texture:SetUVAnimation(50,true)
	end

	ClanFrameLockBtn:SetNormalTexture("uires\\ZhuJieMian\\1.dds","blend");
	ClanFrameLockBtn:SetHightlightTexture("uires\\ZhuJieMian\\1.dds","blend");
	ClanFrameLockBtn:SetPushedTexture("uires\\ZhuJieMian\\1.dds","blend");

	ClanFrameLockBtnEffectTex:Hide();
	if not bHaveOpenContainer then
		UpdateContainerLockEffect( "ClanFrameLockBtn" );
		bHaveOpenContainer = true;
	end
	ContainerButtonLockAnimationTex:Hide();
	ClanButtonLockAnimationTex:Hide();
	UpdateClanMemberInfo();

	-- ������еȼ�����0���һ�δ�򿪹����е�ͼ���棬����ʾ�򿪳��е�ͼ
	local cityInfo = GameClanManager:getClanCity():getCityInfo();

	if cityInfo.m_nLevel > 0 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_MAP) then
			ShowClanHelpBtn(ClanFrameMapTab, "��Ĺ����Ѿ�ӵ���˳��У����Ե�˲鿴��ͼ")
		end
	else
		HideClanHelpBtn(ClanFrameMapTab)
	end
	
	-- ����Ƽ��ȼ�����0���һ�δ�򿪹��Ƽ����棬����ʾ�򿪿Ƽ�����
	if cityInfo.m_buildings[0].Level > 0 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_TECH) then
			ShowClanHelpBtn(ClanFrameTechTab, "��Ĺ����Ѿ�ӵ���˿Ƽ����ģ����Ե�˲鿴�Ƽ�")
		end
	else
		HideClanHelpBtn(ClanFrameTechTab)
	end
	
	-- �������ȼ�����0���һ�δ�򿪹�������棬����ʾ���������
	if cityInfo.m_buildings[2].Level > 0 then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_PRODUCT) then
			ShowClanHelpBtn(ClanFrameProductTab, "��Ĺ����Ѿ�ӵ���˹��������Ե�˲鿴�������")
		end
	else
		HideClanHelpBtn(ClanFrameProductTab)
	end
end

-- �رչ������ʱ����Ӧ�������ҲӦ�ر� ��[�����б�]��[�����ϵ]��
function ClanFrame_OnHide()
	ClanFrameReserveMember_Texture:Hide();
	bHasNewMemberJoin = false;
	ClanFrameRelation_Texture:Hide();
	bHasRelateChnage = false;
	if ClanBasicMenu:IsShown() then
		ClanBasicMenu:Hide();
	end
	if ClanApplyMenu:IsShown() then
		ClanApplyMenu:Hide();
	end
	ClanFramesCloseSubFrame();
end

function ClanFrameTechType_OnClick()
	if this:IsChecked() then
		return
	end
	ClanTechTypeChange(this)
	FindLearnableTechAndShowHelp()
end

function ClanFrameTech_ScrollUpBtn_OnClick()
	local slider = ClanFrameTechScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= 0 then
		slider:SetValue(nValue)
	end
end

function ClanFrameTech_ScrollDownBtn_OnClick()
	local slider = ClanFrameTechScrollBar
	if not slider:IsShown() then
		return
	end
	local nMaxValue = slider:GetMaxValue()
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= nMaxValue then
		slider:SetValue(nValue)
	end
end

function ClanFrameTechScrollBar_OnValueChanged()
	local slider = ClanFrameTechScrollBar
	BondClanLearnSkillFrameSkilllInfo(LearnType, slider:GetValue() + 1, "ClanSkillInfo", ClanSkillFilter)
	BondClanLearnSkillFrameSkillProgress("ClanSkillInfo")

	FindLearnableTechAndShowHelp()
end

function ClanFrameTechOnlyCanLearn_OnClick()
	local slider = ClanFrameTechScrollBar
	SetClanLearnSkillFrameScroll(LearnType, "ClanFrameTechScrollBar", ClanSkillFilter);
	if slider:IsShown() then
		BondClanLearnSkillFrameSkilllInfo(LearnType, slider:GetValue() + 1, "ClanSkillInfo", ClanSkillFilter)
	else
		BondClanLearnSkillFrameSkilllInfo(LearnType, 1, "ClanSkillInfo", ClanSkillFilter)
	end
	BondClanLearnSkillFrameSkillProgress("ClanSkillInfo")
end

function ClanFrameTechLeftPage_OnClick()
	if nClanLearnCurPage > 1 then
		nClanLearnCurPage = nClanLearnCurPage - 1;
		nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameRefreshLearning("ClanFrameTechLearnBtn", nClanLearnCurPage);
		ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	end
end

function ClanFrameTechPrevPage_OnClick()
	if nClanLearnCurPage > 1 then
		nClanLearnCurPage = nClanLearnCurPage - 1;
		nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameRefreshLearning("ClanFrameTechLearnBtn", nClanLearnCurPage);
		ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	end
end

function ClanFrameTechNextPage_OnClick()
	if nClanLearnCurPage < nClanLearnMaxPage then
		nClanLearnCurPage = nClanLearnCurPage + 1;
		nClanLearnCurPage, nClanLearnMaxPage = ClanLearnSkillFrameRefreshLearning("ClanFrameTechLearnBtn", nClanLearnCurPage);
		ClanLearnSkillFramePagePageFont:SetText(nClanLearnCurPage.."/"..nClanLearnMaxPage);
	end
end

local tabs = {"ClanFrameListTab", "ClanFrameMapTab", "ClanFrameTechTab", "ClanFrameProductTab", "ClanFrameSkillTab", "ClanFrameGroupTab"}
local pannels = {"ClanFrameList", "ClanFrameMap", "ClanFrameTech", "ClanFrameProduct", "ClanFrameSkillButton", "ClanFrameGroup"}

local function ClearTabs()
	for _, name in ipairs(tabs) do
		local btn = getglobal(name)
		btn:DisChecked()
	end
	for _, name in ipairs(pannels) do
		local frame = getglobal(name)
		frame:Hide()
	end
end

function ClanFrameList_OnTab()
	if ClanFrameListTab:IsChecked() then
		return
	end
	ClearTabs()
	ClanFrameListTab:Checked()
	bClanFrameOnlineDesc = false
	bClanFrameContributeDesc = false
	bClanFrameNickNameDesc = false
	bClanFrameCareerDesc = 1
	bClanFramePosDesc = false
	bClanFramelvDesc = false
	bClanFrameNameDesc = false
	bSearchMember = false
	ClanFrameListSearchNameEdit:SetText("")
	tPageData.nCurPage = 1
	ClanFrameSetMaxPage()
	ClanFrameList:Show()
end

function ClanFrameMap_OnTab()
	if ClanFrameMapTab:IsChecked() then
		return
	end
	local cityInfo = GameClanManager:getClanCity():getCityInfo()
	if cityInfo.m_nLevel == 0 then
		ShowMidTips("���Ĺ��ᵱǰ��û�г��У��޷������");
		return;
	else
		-- ȡ�������������ٴ���ʾ
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_MAP) then
			HideClanHelpBtn(ClanFrameMapTab)
			GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_MAP)
		end
	end
	ClearTabs()
	ClanFrameMapTab:Checked()
	ReSetClanMapInfo(cityInfo)
	UpdateClanMapResource()
	ClanFrameMap:Show()
end

function ClanFrameTech_OnTab()
	if ClanFrameTechTab:IsChecked() then
		return
	end
	local cityInfo = GameClanManager:getClanCity():getCityInfo()
	if cityInfo.m_buildings[0].Level == 0 then
		ShowMidTips("���Ĺ��ᵱǰ��û�пƼ����ģ��޷������")
		return
	else
		-- ȡ�������������ٴ���ʾ
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_TECH) then
			HideClanHelpBtn(ClanFrameTechTab)
			GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_TECH)
		end
	end
	ClearTabs()
	ClanFrameTechTab:Checked()
	ClanTechTypeChange(ClanFrameTechTypeBtn1)
	ClanFrameTech:Show()

	FindLearnableTechAndShowHelp()
end

function ClanFrameProduct_OnTab()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬���ܽ��й�����صĲ���" )
		return
	end
	if ClanFrameProductTab:IsChecked() then
		return
	end
	local cityInfo = GameClanManager:getClanCity():getCityInfo()
	if cityInfo.m_buildings[2].Level == 0 then
		ShowMidTips("���Ĺ��ᵱǰ��û�й������޷������")
		return
	else
		-- ȡ�������������ٴ���ʾ
		if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_PRODUCT) then
			HideClanHelpBtn(ClanFrameProductTab)
			GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_PRODUCT)
		end
	end
	ClearTabs()
	ClanFrameProductTab:Checked()
	ClanFrameProduct:Show()

	ClanFrameProductOnShow()

	SelectListBox(1)

	if #LabelList > 0 then
		SelectClanFrameProductLabel(2)
	end
end	

function UpdateClanMapResource()
	local m, n = 1, 9
	for name, amount, limit in GetClanResource() do
		if string.find(name, "��$") then
			ui = getglobal("ClanFrameMapResource"..m)
			m = m + 1
		else
			ui = getglobal("ClanFrameMapResource"..n)
			n = n + 1
		end
		if limit == 0 then
			ui:SetText(string.format("%s:%d/��", name, amount))
		else
			ui:SetText(string.format("%s:%d/%d", name, amount, limit))
		end
	end
end

function IsInClan()
	return GameClanManager:getClanViewer().m_clanInfo.m_szClanName ~= "";
end

--@Desc:���ð�ť״̬
function SetClanBtnState()
	if GetPlayPositionInClan() == CLAN_POS_PRESIDENT or GetPlayPositionInClan() == CLAN_POS_VICE_PRESIDENT then
		ClanFramePostEdit:Show();
	else
		ClanFramePostEdit:Hide();
	end
end

function ClearClanMemberInfo()
	for i = 1, MAXCLANMEMROW do
		local button = getglobal("ClanFrameListItem"..i);
		local t_fonts =		{	"ClanFrameListItem"..i.."Name",		"ClanFrameListItem"..i.."Level",		"ClanFrameListItem"..i.."Position",
								"ClanFrameListItem"..i.."Career",	"ClanFrameListItem"..i.."NickName",	"ClanFrameListItem"..i.."Contribute",
								"ClanFrameListItem"..i.."LastOnline"
							};
		local t_frames =	{
			--"ClanFrameListItem"..i.."ContributeIcon",
		};
		for _, name in ipairs( t_fonts ) do
			local font = getglobal( name );
			font:SetText( "" );
		end
		for _, name in ipairs(t_frames) do
			local frame = getglobal(name);
			frame:Hide();
		end
		button:Disable();
	end
end

function updateClanBulletin(text, time)
	ClanFramePostText:SetText(text, 255, 254, 189 );
	if time == 0 then
		ClanFramePostEditTime:SetText("");
	else
		local szModifyDate = os.date( "%Y/%m/%d", time);
		ClanFramePostEditTime:SetText( szModifyDate );
	end
end

--@Desc:ˢ�¹����Ա�б�
function UpdateClanMemberInfo( )
	-- ��ȷ����ʼˢ��ҳ
	local ClanViewer	= GameClanManager:getClanViewer();

	ClearClanMemberInfo();
	DisCheckAllBtn( "ClanFrameListItem", MAXCLANMEMROW );
	
	nPage = (tPageData.nCurPage-1)*MAXCLANMEMROW + 1;

	local nClientId	= 1;
	local ClanInfo	= ClanViewer.m_clanInfo;
	local cityInfo	= GameClanManager:getClanCity():getCityInfo();
	BondingClanDefInfo( ClanViewer, cityInfo );

	local searchName = ClanFrameListSearchNameEdit:GetText();
	if bSearchMember and searchName ~= "" then
		local nIndex = 0;
		for i=1, ClanViewer.m_clanInfo.m_nClanMemNum do
			if nClientId > MAXCLANMEMROW then
				break;
			end
			if string.find( ClanViewer.m_Members[i-1].RoleMember.m_szRoleName,searchName,1,true ) then
				nIndex = nIndex + 1;
				if nIndex >=nPage then
					BondingClanMemInfo( ClanViewer, i, nClientId );
					nClientId = nClientId + 1;
				end
			end
		end
	else
		for j = nPage, ClanViewer.m_clanInfo.m_nClanMemNum do
			if nClientId > MAXCLANMEMROW then
				break;
			end
			BondingClanMemInfo( ClanViewer, j, nClientId );
			nClientId = nClientId + 1;
		end
	end
	
	ClanFrameListPage:SetText("��"..tPageData.nCurPage.."/"..tPageData.nTotalPage.."ҳ");
	
	-- ��һ��ѡ����ѡ�е����ֵĲ���
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end

	for i = 1, MAXCLANMEMROW do
		local btn = getglobal("ClanFrameListItem"..i);
		local font = getglobal("ClanFrameListItem"..i.."Name");
		if btn:IsShown() and font:GetText() == t_clanFrameSelectMember["name"] then
			btn:Checked();
			return;
		end
	end

	-- ��û���κΰ�ť��ѡ������˵��ԭ����ѡ���Ķ����Ѳ��ٹ����б�
	t_clanFrameSelectMember = { heightId = 0, lowId = 0, name = "" };
end

--��ȡ�Լ������Ա����
local MAX_CLAN_SKILL_RESULT = 3		--/* ��󹫻Ἴ��Ч������ */
function GetPlayerClanMaxMemNum()
	local clanMaxMemNum = 0
	local skillDef = GameClanManager:getClanSkillDef(MEM_NUM_CLAN_SKILL_ID_TOLUA, 0)
	local clanSkill = GameClanManager:getClanSkillInfo();
	for i=1,clanSkill.Num do
		local info = clanSkill.ClanSkill[i-1];
		if info.SkillID == MEM_NUM_CLAN_SKILL_ID_TOLUA then
			skillDef = GameClanManager:getClanSkillDef(info.SkillID, info.Level)
		end
	end
	if skillDef.SkillID > 0 then
		for i = 1, MAX_CLAN_SKILL_RESULT do
			if skillDef.Result[i-1].ResultID == RESULT_CLAN_MEM_NUM_TOP then
				clanMaxMemNum = skillDef.Result[i-1].ResultVal1
			end
		end
	end
	return clanMaxMemNum
end

--@Desc:�󶨹��ᾲ̬��Ϣ
function BondingClanDefInfo( ClanViewer, cityInfo )
	local MemberOnLineValue = GetOnlineMember( ClanViewer.m_clanInfo.m_nClanMemNum, ClanViewer.m_Members );
	local t_ClanDefInfo = {
							{"ClanFrameBasicInfoName",			ClanViewer.m_clanInfo.m_szClanName }, -- ����
							{"ClanFrameBasicInfoLevel",			ClanViewer.m_clanInfo.m_nClanLevel }, -- �ȼ�
							{"ClanFrameBasicInfoProsper",		ClanViewer.m_clanInfo.m_nActiveValue }, -- ���ٶ�
							{"ClanFrameBasicInfoStable",			ClanViewer.m_clanInfo.m_nBuildingValue.."/10000" }, -- �����
							--{"ClanFrameBasicInfoPresident",		ClanViewer.m_PresidentInfo.m_szRoleName }, -- �᳤
							{"ClanFrameBasicInfoMoney",			ClanViewer.m_clanInfo.m_nMoney }, -- ��Ǯ
							{"ClanFrameBasicInfoMemberOnLine",	MemberOnLineValue.."/"..ClanViewer.m_clanInfo.m_nClanMemNum }, -- ��������
							{"ClanFrameBasicInfoCityLevel",		cityInfo.m_nLevel }, -- ���еȼ�	
							{"ClanFrameBasicInfoClanMaxMemNum",		GetPlayerClanMaxMemNum() }, -- �����Ա����	
							{"ClanFrameBasicInfoTechique",		ClanViewer.m_clanInfo.m_nSkillPoint },	--�Ƽ���
						  };
	ClanFrameBasicInfoName:SetClientUserData( 0,ClanViewer.m_clanInfo.m_uClanId );
	for i=1, table.getn( t_ClanDefInfo ) do
		local FontName = getglobal( t_ClanDefInfo[i][1] );
		FontName:SetText( t_ClanDefInfo[i][2] );
	end
	ClanFramePostText:SetText( ClanViewer.m_clanInfo.m_szBulletinText, 255, 254, 189 );
	if ClanViewer.m_clanInfo.m_BulletinTextLastEditTime == 0 then
		ClanFramePostEditTime:SetText("");
	else
		local szModifyDate = os.date( "%Y/%m/%d�޸�",ClanViewer.m_clanInfo.m_BulletinTextLastEditTime );
		ClanFramePostEditTime:SetText( szModifyDate );
	end
	ClanFrameBasicInfoNameTipBtn:SetClientUserData(0, ClanViewer.m_clanInfo.m_uClanId)
	if ClanFrameBasicInfoPresident:GetTextExtentWidth(ClanViewer.m_PresidentInfo.m_szRoleName) > 115 then
		ClanFrameBasicInfoPresident:SetText( string.sub( ClanViewer.m_PresidentInfo.m_szRoleName,0,9 ) .. "..." );
		ClanFrameBasicInfoPresidentTipBtn:SetClientString( ClanViewer.m_PresidentInfo.m_szRoleName );
	else
		ClanFrameBasicInfoPresident:SetText( ClanViewer.m_PresidentInfo.m_szRoleName );
		ClanFrameBasicInfoPresidentTipBtn:SetClientString( "" );
	end
	--�������ʽ�����֧����һ�ο۷�ʱ���ı���ʾ��ɫ
	if ClanViewer.m_clanInfo.m_nMoney >= GetClanNextMoney() then
		ClanFrameBasicInfoMoney:SetTextColor( 255,255,190 )
	else
		ClanFrameBasicInfoMoney:SetTextColor( 255,0,0 )
	end
end

function GetTimeText( member )
	if member.Online == 1 then
		return "����"
	elseif member.Online == 2 then
		return "�����ռ�"
	end

	local dTime = GameClanManager:getServerTime() - member.LastOnline;
	if dTime >= 24*60*60 then
		local day = math.floor( dTime / (24*60*60) );
		if day > 10 then 
			return ">10��"
		else 
			return ">" .. day .. "��"
		end
	elseif dTime >= 10*60*60 then
		return ">10Сʱ"
	elseif dTime >= 60*60 then
		return ">1Сʱ"
	elseif dTime > 60 then
		return "<1Сʱ"
	else
		return "<1����"
	end
end

function IsInClanGroup( nClanGroupID )
	if nClanGroupID > 0 and nClanGroupID <= MAX_CLAN_GROUP_NUM_TOLUA then
		return true
	end
	return false
end

function getClanGroupName( nClanGroupID )
	local t_szCLanGroup = { "߱ɱս��","��ɱս��","��ɱս��","��ɱս��","��ɱս��","�ƽ�ս��", }
	return t_szCLanGroup[ nClanGroupID ];
end

function getClanGroupAndPosName( groupID, pos )
	if pos == CLAN_POS_PRESIDENT then
		local szGroupInfo = string.gsub( getClanGroupName(groupID), "ս��", "" )
		szGroupInfo = string.format( "(%s)", szGroupInfo )
		return "�᳤"..szGroupInfo
	elseif pos == CLAN_POS_VICE_PRESIDENT then
		local szGroupInfo = string.gsub( getClanGroupName(groupID), "ս��", "" )
		szGroupInfo = string.format( "(%s)", szGroupInfo )
		return "���᳤"..szGroupInfo
	elseif pos == CLAN_POS_GROUP_1 then
		return getClanGroupName(groupID).."��"
	elseif pos == CLAN_POS_GROUP_2 then
		return getClanGroupName(groupID).."��"
	elseif pos == CLAN_POS_GROUP_3 then
		return getClanGroupName(groupID).."��"
	elseif pos == CLAN_POS_GROUP_4 then
		return getClanGroupName(groupID).."��"
	elseif pos == CLAN_POS_GROUP_5 then
		return getClanGroupName(groupID).."��"
	elseif pos == CLAN_POS_GROUP_6 then
		return getClanGroupName(groupID).."��"
	else
		return getClanGroupName(groupID)
	end
end

--@Desc:�󶨹����Ա�б�
function BondingClanMemInfo( ClanViewer, nPage, nClientId )
	local jobName = "";
	if string.len( ClanViewer.m_Members[nPage-1].Title ) == 0 then
		jobName = GetPosition( ClanViewer.m_Members[nPage-1].Position );
		--��ս�ŵ������ʾ���ս����Ϣ
		if IsInClanGroup( ClanViewer.m_Members[nPage-1].GroupID ) then
			jobName = getClanGroupAndPosName( ClanViewer.m_Members[nPage-1].GroupID, ClanViewer.m_Members[nPage-1].Position )
		end
	else
		jobName = ClanViewer.m_Members[nPage-1].Title ;
	end
	local t_ClanMemInfo = {
							--{"ClanFrameListItem"..nClientId.."Name",			ClanViewer.m_Members[nPage-1].RoleMember.m_szRoleName }, -- ����
							{"ClanFrameListItem"..nClientId.."Level",			ClanViewer.m_Members[nPage-1].Level }, -- �ȼ�
							{"ClanFrameListItem"..nClientId.."Position",		jobName }, -- �ٽ�
							{"ClanFrameListItem"..nClientId.."Career",		GetCareerName(ClanViewer.m_Members[nPage-1].Career) }, -- ְҵ
							{"ClanFrameListItem"..nClientId.."NickName",		ClanViewer.m_Members[nPage-1].NickName }, -- �ǳ�
							{"ClanFrameListItem"..nClientId.."Contribute",	ClanViewer.m_Members[nPage-1].Contribute .. "/" .. ClanViewer.m_Members[nPage-1].TotalContribute }, -- ���׶�
							{"ClanFrameListItem"..nClientId.."LastOnline",	GetTimeText(ClanViewer.m_Members[nPage-1]) }, -- �������ʱ��
						  };
	local t_frames = {
		--"ClanFrameListItem"..nClientId.."ContributeIcon",
	};
	
	local fontColor = {};

	if ClanViewer.m_Members[nPage-1].Online == 0  then
		fontColor = {160, 160, 160};
	else
		fontColor = { 255,254,189 };
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			if ClanViewer.m_Members[nPage-1].RoleMember.m_szRoleName == mainplayer:getName() then
				fontColor = { 120,235,50 };
			end
		end
	end
	local nameFont = getglobal( "ClanFrameListItem"..nClientId.."Name" );
	local nameBtn = getglobal( "ClanFrameListItem"..nClientId.."NameBtn" )
	if nameFont:GetTextExtentWidth( ClanViewer.m_Members[nPage-1].RoleMember.m_szRoleName ) > 120 then
		nameFont:SetText( string.sub(ClanViewer.m_Members[nPage-1].RoleMember.m_szRoleName,0,9) .. "..." );
		nameBtn:SetClientString( ClanViewer.m_Members[nPage-1].RoleMember.m_szRoleName );
	else
		nameFont:SetText( ClanViewer.m_Members[nPage-1].RoleMember.m_szRoleName );
		nameBtn:SetClientString( "" );
	end
	nameFont:SetTextColor( fontColor[1],fontColor[2],fontColor[3] );

	local btn = getglobal( "ClanFrameListItem"..nClientId );
	btn:SetClientUserData(0,nPage-1 );
	for i=1, table.getn( t_ClanMemInfo ) do
		local FontName = getglobal( t_ClanMemInfo[i][1] );
		FontName:SetText( t_ClanMemInfo[i][2] );
		FontName:SetTextColor( fontColor[1],fontColor[2],fontColor[3] );
	end
	for _, name in ipairs(t_frames) do
		local frame = getglobal(name)
		frame:Show()
	end
	btn:Enable();
end

function ClanFrameListName_OnEnter()
	local name = this:GetClientString();
	if name ~= "" then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = name, frame = this:GetParent(),
						button = "cursor", horizonal = "right",vertical ="bottom" } );
	end
	for i=1,MAXCLANMEMROW do
		local runingBtn = getglobal("ClanFrameListItem"..i);
		if runingBtn ~= nil then
			runingBtn:DisHighlight();
		end
	end

	local infoBtn = getglobal( this:GetParent() );
	infoBtn:Highlight();
end

function ClanFrameListName_OnLeave()
	 for i=1,MAXCLANMEMROW do
		local runingBtn = getglobal("ClanFrameListItem"..i);
		if runingBtn ~= nil then
			runingBtn:DisHighlight();
		end
	end
	GameTooltip:Hide();
end

function ClanFrameListName_OnClick()
	DisCheckAllBtn( "ClanFrameListItem", MAXCLANMEMROW );
	local btn = getglobal( this:GetParent() );
	btn:Checked();
	
	local font = getglobal( btn:GetName() .. "Name" );
	
	local ClanViewer	= GameClanManager:getClanViewer();
	local ClanInfo		= ClanViewer.m_clanInfo;
	for i = 1, ClanInfo.m_nClanMemNum do
		local member = ClanViewer.m_Members[i-1].RoleMember;
		local roleName = "";
		if this:GetClientString() ~= "" then
			roleName = this:GetClientString();
		else
			roleName = font:GetText();
		end
		if member.m_szRoleName == roleName then
			t_clanFrameSelectMember = { heightId = member.m_RoleLongId.nHeight, lowId = member.m_RoleLongId.nLow, 
										name = roleName };
			if arg1 == "RightButton" then
				ClanBasicMenu:Show();
			end
			break;
		end
	end
end

--@Desc:��������б���ʱ��Ӧ
function ClanFrameListItem_OnClick()		
	DisCheckAllBtn( "ClanFrameListItem", MAXCLANMEMROW );
	this:Checked();
	
	local font = getglobal( this:GetName() .. "Name" );
	local fontBtn = getglobal( this:GetName() .."NameBtn" );
	
	local ClanViewer	= GameClanManager:getClanViewer();
	local ClanInfo		= ClanViewer.m_clanInfo;
	for i = 1, ClanInfo.m_nClanMemNum do
		local member = ClanViewer.m_Members[i-1].RoleMember;
		local roleName = "";
		if fontBtn:GetClientString() ~= "" then
			roleName = fontBtn:GetClientString();
		else
			roleName = font:GetText();
		end
		if member.m_szRoleName == roleName then
			t_clanFrameSelectMember = { heightId = member.m_RoleLongId.nHeight, lowId = member.m_RoleLongId.nLow, 
										name = roleName };
			if arg1 == "RightButton" then
				ClanBasicMenu:Show();
			end
			break;
		end
	end	
end

function ClanFrameListFirstPage_OnClick()
	if tPageData.nCurPage ~= 1 then
		tPageData.nCurPage = 1;
		UpdateClanMemberInfo();
	end
end

function ClanFrameListPrevPage_OnClick()
	if tPageData.nCurPage > 1 then
		tPageData.nCurPage = tPageData.nCurPage -1;
		UpdateClanMemberInfo();
	end
end

function ClanFrameListNextPage_OnClick()
	if tPageData.nCurPage < tPageData.nTotalPage then
		tPageData.nCurPage = tPageData.nCurPage + 1;
		UpdateClanMemberInfo();
	end
end

function ClanFrameListLastPage_OnClick()
	if tPageData.nCurPage ~= tPageData.nTotalPage then
		tPageData.nCurPage = tPageData.nTotalPage;
		UpdateClanMemberInfo();
	end
end

function ClanFrameList_OnSearch()
	bSearchMember = true;
	tPageData.nCurPage = 1;
	ClanFrameSetMaxPage()
end

--@Desc:��ȡ�������߳�Ա����
function GetOnlineMember( nClanMemNum, ClanMembers )
	local nOnlineCount = 0;
	for i = 1, nClanMemNum do
		if ClanMembers[i-1].Online == 1 then
			nOnlineCount = nOnlineCount + 1;
		end
	end
	return nOnlineCount;
end

--@Desc:��������
function ClanFrameUpgrade_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬���ܽ��й�����صĲ���" )
		return
	end
	if ClanLevelUpFrame:IsShown() then
		ClanLevelUpFrame:Hide();
		return;
	end	
	local t_positionsCanLevelUp = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local levelUpClanDef	= getClanLevelDef( ClanInfo.m_nClanLevel + 1 );
	-- ������ȼ��ﵽ��ߵȼ����򲻵�����壬����������ʾ
	if not levelUpClanDef then
		ShowMidTips( "�����Ѵﵽ��ߵȼ����޷�����" );	
		return;
	end

	for _, position in ipairs( t_positionsCanLevelUp ) do
		if position == mainplayer:getClanPosition() then
			ClanLevelUpFrame:Show();
			ClanLevelUpFrame:AddLevelRecursive(1);
			return;
		end
	end
	ShowMidTips( "ֻ�л᳤���߸��᳤������������" );	
end

--@Desc:�༭����
function ClanFramePostEdit_OnClick()
	if Clan_EditFrame:IsShown() and Clan_EditFrame:GetClientString() == "�༭����" then
		Clan_EditFrame:Hide();
		return;
	end
	local t_canEditNoticesPositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canEditNoticesPositoins ) do
		if mainplayer:getClanPosition() == position then
			Clan_EditFrame:Show();
			ClanEditFrameNoticeInfo:SetText("�༭����");
			Clan_EditFrame:SetClientString("�༭����");
			Clan_EditFrame:AddLevelRecursive(1);
			local ClanViewer = GameClanManager:getClanViewer();
			Clan_Edit_SetNameEditBox:SetText( ClanViewer.m_clanInfo.m_szBulletinText );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ�ޱ༭���ṫ��" );
end

local t_clanInputFrameSpace =	{									
									["�����ǳ�"]	= { func = function ( data )
																		-- ��ְλת�ö����Ѿ����ڹ����ˣ�����ת��
																		if not GameClanManager:isClanMember( t_clanFrameSelectMember["name"] ) then
																			ShowMidTips( "����Ѿ����ڹ���" );
																			return true;
																		end

																		GameClanManager:clan_Opt_SetNickName( data.member.m_RoleLongId.nHeight,
																												data.member.m_RoleLongId.nLow, 
																												data.name );
																		return true;
																	end,
														checkFunc = function ( ) 
																		local t_canModifyNiChengPositoins = { 
																			CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
																			CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
																			CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
																			CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
																		-- ��������ǳƣ�Ҫ��ٽױ���Ϊ�᳤�����᳤
																		--�¼ӵ�ս�ų�ҲҪ��Ȩ���޸��ǳ�
																		local mainplayer = ActorMgr:getMainPlayer();
																		for _, position in ipairs( t_canModifyNiChengPositoins ) do
																			if mainplayer:getClanPosition() == position then
																				return true;																				
																			end
																		end
																		Clan_SetNiChengInputFrame:Hide();
																				ShowMidTips( "��û��Ȩ���޸��ǳƣ����Զ��رո����" );
																				return false;																		
																	end
														},
									["��������"]	= { func = function ( data )				
																		if data.name == "" then
																			ShowMidTips( "����Ĺ������ֲ���Ϊ��" );
																			return false;
																		end
																		local money = Clan_CreateInputFrame:GetClientUserData( 0 );
																		if checkNeedWarnCost( money ) then
																			CostMoneyConfirmFrame:SetClientString("��������");
																			SetWarnFrame("Clan_CreateInputFrame");
																			CostMoneyConfirmFrame:Show();
																			return false;
																		end
																		GameClanManager:clan_Opt_Create( data.name );
																		ClearFocus();
																		return true;
																end,
														checkFunc = function () return true; end
														},
									
								};

--��ӻ�Ա
function ApplyJoinClanFrameAddMember_OnClick()
	--�¼ӵ�ս�ų�ҲҪ��Ȩ����ӻ�Ա
	local t_canAddMemberPositoins = { 
		CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
		CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
		CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
		CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
	local mainplayer = ActorMgr:getMainPlayer();
	for _, position in ipairs( t_canAddMemberPositoins ) do
		if mainplayer:getClanPosition() == position then
			local edit = getglobal("ApplyJoinClanFrameNameEdit");
			local name = edit:GetText();
			if name == "" then
				ShowMidTips("�����������");
				return
			end
			GameClanManager:clan_Opt_Invite( name );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ����ӻ�Ա" );
end

--@Desc:Ԥ����Ա��ť
function ClanFrameReserverMember_OnClick()	
	local t_canReserveMemberPositoins = { 
		CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
		CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
		CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
		CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
	local mainplayer = ActorMgr:getMainPlayer();

	for _, position in ipairs( t_canReserveMemberPositoins ) do
		if mainplayer:getClanPosition() == position then
			if ApplyJoinClanFrame:IsShown() then
				ApplyJoinClanFrame:Hide();
			else
				ApplyJoinClanFrame:Show();
				ApplyJoinClanFrame:AddLevelRecursive();
			end
			if bHasNewMemberJoin == true then
				bHasNewMemberJoin = false;
				ClanFrameReserveMember_Texture:Hide();
			end
			return;
		end
	end
	ShowMidTips( "��û��Ȩ�޽��и������" );
end

function Clan_PrivateOnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if t_clanFrameSelectMember["name"] == mainplayer:getName() then
		ShowMidTips( "�������Լ�˽��" );
		return;
	end

	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = t_clanFrameSelectMember["name"] } );

	SetChatType( CL_CHAT_PRIVATE );
	UpdateChatInsertFrameEnterFocus();
	SetFocusFrame("CSChatInsert");
end

function ClanDetailOnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	RankMgr:requestWatchPlayer(  t_clanFrameSelectMember["name"] );
end

function ClanAddFriendOnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if t_clanFrameSelectMember["name"] == mainplayer:getName() then
		ShowMidTips( "��������Լ�Ϊ����" );
		return;
	end
	GameFriendManager:addFriend( t_clanFrameSelectMember["name"], "" );
end
--[[
function ClanTeamApplyOnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if t_clanFrameSelectMember["name"] == mainplayer:getName() then
		ShowMidTips( "�������Լ��������" );
		return;
	end
	TeamManager:Team_OP_Apply( t_clanFrameSelectMember["name"] );
end
--]]
function ClanTeamInviteOnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if t_clanFrameSelectMember["name"] == mainplayer:getName() then
		ShowMidTips( "�������Լ������������" );
		return;
	end
	TeamManager:Team_OP_Add( t_clanFrameSelectMember["name"] );
end

--@Desc:���ùٽ�
function Clan_RaiseLVBtn_OnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local t_canRaiseLVPositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	for _, position in ipairs( t_canRaiseLVPositoins ) do
		if mainplayer:getClanPosition() == position then
			ClanSetPositionFrame:SetClientUserData( 0,t_clanFrameSelectMember["lowId"] );
			ClanSetPositionFrame:SetClientUserData( 1,t_clanFrameSelectMember["heightId"] );
			ClanSetPositionFrame:SetClientString( t_clanFrameSelectMember["name"] );
			ClanSetPositionFrame:Show();
			return;
		end
	end
	ShowMidTips( "��û��Ȩ�����ùٽ�" );
end

--[[
--@Desc:���͵ȼ���ť
function Clan_FallLVBtn_OnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if t_clanFrameSelectMember["name"] == mainplayer:getName() then
		ShowMidTips( "�����Լ����Լ�����ְλ" );
		return;
	end

	local t_cannotFallLVPositoins = { CLAN_POS_NOT_MEMBER, CLAN_POS_NORMAL, CLAN_POS_OFFICER };	

	for _, position in ipairs( t_cannotFallLVPositoins ) do
		if mainplayer:getClanPosition() == position then
			ShowMidTips( "��û��Ȩ�޽�������ְλ" );
			return;
		end
	end
	
	GameClanManager:clan_Opt_DownPostion( t_clanFrameSelectMember["heightId"], t_clanFrameSelectMember["lowId"], 
											t_clanFrameSelectMember["name"] );	
end
--]]

--@Desc:ְλת�ð�ť
function Clan_PositionChangeBtn_OnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if t_clanFrameSelectMember["name"] == mainplayer:getName() then
		ShowMidTips( "�����Լ����Լ�ת��ְλ" );
		return;
	end
	local t_canPositionChangePositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	for _, position in ipairs( t_canPositionChangePositoins ) do
		if mainplayer:getClanPosition() == position then
			-- ����ѡ��������Ƿ�����
			local ClanViewer	= GameClanManager:getClanViewer();
			local ClanInfo	= ClanViewer.m_clanInfo;

			for i = 1, ClanViewer.m_clanInfo.m_nClanMemNum do
				local member = ClanViewer.m_Members[i-1];
				if t_clanFrameSelectMember["name"] == member.RoleMember.m_szRoleName and member.Online ~= ONLINE  then
					ShowMidTips( "����Ҳ����ߣ����ܽ�ְλת�ø���" );
					return;
				end
			end
			local font = getglobal( "ClanFrame_ConfirmFrame_MessageText" );
			font:SetText( "ȷ�ϰ�ְλת�ø�"..t_clanFrameSelectMember["name"] );
			ClanFrame_ConfirmFrame:Show();
			ClanFrame_ConfirmFrame_AcceptButton:SetClientString( t_clanFrameSelectMember["name"] );
			ClanFrame_ConfirmFrame:SetClientString( "Clan_PositionChangeBtn" );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ������ְλ" );
end


--@Desc:�����ǳư�ť
function Clan_SetNiChengBtn_OnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local t_canModifyNiChengPositoins = { 
	CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
	CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
	CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
	CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canModifyNiChengPositoins ) do
		if mainplayer:getClanPosition() == position then
			Clan_SetNiChengInputFrame:SetClientString("�����ǳ�");
			Clan_SetNiChengInputFrame:Show();
			return;
		end
	end
	ShowMidTips( "��û��Ȩ���޸��ǳ�" );
end

--Desc:������ᰴť
function Clan_DelMemberBtn_OnClick()
	if t_clanFrameSelectMember["name"] == "" then
		return;
	end
	local t_canDelMemberPositoins = { 
	CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
	CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
	CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
	CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	for _, position in ipairs( t_canDelMemberPositoins ) do
		if mainplayer:getClanPosition() == position then
			if t_clanFrameSelectMember["name"] == mainplayer:getName() then
				ShowMidTips( "���ܽ��Լ��߳�����" );
				return;
			end
			local font = getglobal( "ClanFrame_ConfirmFrame_MessageText" );
			font:SetText( "ȷ�ϰ�"..t_clanFrameSelectMember["name"].."�߳�������" );
			ClanFrame_ConfirmFrame:Show();
			ClanFrame_ConfirmFrame_AcceptButton:SetClientString( t_clanFrameSelectMember["name"] );
			ClanFrame_ConfirmFrame:SetClientString( "Clan_DelMemberBtn" );
			return;
		end
	end
	ShowMidTips( "��û��Ȩ�޽������߳�����" );
end


--@Desc:�⽻��ϵ��ť
function ClanFrameRelation_OnClick()
	if ClanDiplomatismFrame:IsShown() then
		ClanDiplomatismFrame:Hide();
	else
		ClanDiplomatismFrame:Show();
		ClanDiplomatismFrame:AddLevelRecursive(1);
	end
	if bHasRelateChnage == true then
		bHasRelateChnage = false;
		ClanFrameRelation_Texture:Hide();
	end
end

--@Desc:�˳����ᰴť
function Clan_QuitClanBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT then
		ShowMidTips( "�᳤�����˳�����" );
		return;
	end
	local font = getglobal( "ClanFrame_ConfirmFrame_MessageText" );
	font:SetText( "ȷ��Ҫ�˳����᣿" );
	ClanFrame_ConfirmFrame:Show();
	Clan_QuitClanBtn:SetClientString( mainplayer:getName() );
	ClanFrame_ConfirmFrame:SetClientString( "Clan_QuitClanBtn" );
end

--@Desc:��ӳ�Ա �����ǳ� �༭����
function Clan_InputConfirmBtn_OnClick()
	-- �ȼ������ߵ�Ȩ��
	local frame = getglobal( this:GetParent() );
	if not t_clanInputFrameSpace[frame:GetClientString()].checkFunc() then
		return;
	end	
	local edit = getglobal( this:GetParent() .. "_Input_SetNameEditBox" );
	local ClanViewer = GameClanManager:getClanViewer();
	local selMember;
	for i = 1, ClanViewer.m_clanInfo.m_nClanMemNum do
		if ClanViewer.m_Members[i-1].RoleMember.m_szRoleName == t_clanFrameSelectMember["name"] then
			selMember = ClanViewer.m_Members[i-1].RoleMember;
			break;
		end
	end
	local bClosFrame = t_clanInputFrameSpace[frame:GetClientString()].func( { name = edit:GetText(), member = selMember } );
	if bClosFrame then
		frame:Hide();
	end
end

function Clan_InputCancleBtn_OnClick()
	local frame = getglobal( this:GetParent() );
	ClearFocus();
	frame:Hide();
end

--@Desc:��������
function ClanFrameSortNameBtn_OnClick()
	bClanFrameNameDesc = not bClanFrameNameDesc ;
	if bClanFrameNameDesc then
		GameClanManager:sortMemberList( QUICKSORT_BY_NAME, 1 );
	else
		GameClanManager:sortMemberList( QUICKSORT_BY_NAME, -1 );
	end
	UpdateClanMemberInfo( );
end

--@Desc:�ȼ�����
function ClanFrameSortLvBtn_OnClick()
	bClanFramelvDesc = not bClanFramelvDesc ;
	if bClanFramelvDesc then
		GameClanManager:sortMemberList( QUICKSORT_BY_LEVEL, 1 );
	else
		GameClanManager:sortMemberList( QUICKSORT_BY_LEVEL, -1 );
	end
	UpdateClanMemberInfo( );
end

--@Desc:��ְ����
function ClanFrameSortPosBtn_OnClick()
	bClanFramePosDesc = not bClanFramePosDesc ;
	--�����ս�ź���ͨ��Ա����ʾ����ս�ţ���������˶���ͨ��Աս�ŵ�����
	if bClanFramePosDesc then
		GameClanManager:sortMemberList( QUICKSORT_BY_POSITION, 1 );
		GameClanManager:sortMemberList( QUICKSORT_BY_GROUP, 1 )
	else
		GameClanManager:sortMemberList( QUICKSORT_BY_POSITION, -1 );
		GameClanManager:sortMemberList( QUICKSORT_BY_GROUP, -1 )
	end
	UpdateClanMemberInfo( );
end

--@Desc:ְҵ����
function ClanFrameSortCareerBtn_OnClick()	
	GameClanManager:sortMemberList( QUICKSORT_BY_CAREER, bClanFrameCareerDesc );
	bClanFrameCareerDesc = bClanFrameCareerDesc + 1;
	if bClanFrameCareerDesc > MAX_CAREER_NUM then
		bClanFrameCareerDesc = 1;
	end
	UpdateClanMemberInfo( );
end

--@Desc:�ǳ�����
function ClanFrameSortNickNameBtn_OnClick()
	bClanFrameNickNameDesc = not bClanFrameNickNameDesc ;
	if bClanFrameNickNameDesc then
		GameClanManager:sortMemberList( QUICKSORT_BY_NICKNAME, 1 );
	else
		GameClanManager:sortMemberList( QUICKSORT_BY_NICKNAME, -1 );
	end
	UpdateClanMemberInfo( );
end

--@Desc:���׶�����
function ClanFrameContributeBtn_OnClick()
	bClanFrameContributeDesc = not bClanFrameContributeDesc ;
	if bClanFrameContributeDesc then
		GameClanManager:sortMemberList( QUICKSORT_BY_CONTRIBUTE, 1 );
	else
		GameClanManager:sortMemberList( QUICKSORT_BY_CONTRIBUTE, -1 );
	end
	UpdateClanMemberInfo( );
end

--@Desc:����ʱ������
function ClanFrameSortOnLineBtn_OnClick()
	bClanFrameOnlineDesc = not bClanFrameOnlineDesc ;
	if bClanFrameOnlineDesc then
		GameClanManager:sortMemberList( QUICKSORT_BY_ONLINE, 1 );
	else
		GameClanManager:sortMemberList( QUICKSORT_BY_ONLINE, -1 );
	end
	UpdateClanMemberInfo( );
end

----------------------------- ����Ƽ����� -----------------------------

function ClanSkillProgress_OnUpdate()
	local nEndTime = this:GetClientUserData( 0 );
	local nBeginTime = this:GetClientUserData( 2 );
	local nDelta = this:GetClientUserData( 1 );
	if nEndTime > 0 then
		local nPass = GameClanManager:getServerTime() - nBeginTime + nDelta - ( nEndTime - nBeginTime ) ;
		local scale = nPass/this:GetClientUserData(1);
		if scale > 1 then
			scale = 1;
		end
		local progressTex = getglobal(this:GetName().."ProgressTex");
		progressTex:SetTexUV( 549,996,340*scale,13 );
		progressTex:SetSize(340*scale,13);
	end
end

function ClanSkillProgress_OnEnter()
	local nEndTime = this:GetClientUserData( 0 );
	local nLeftTime = nEndTime - GameClanManager:getServerTime();
	if nLeftTime > 0 then		
		local szText = "����ʣ��ʱ�䣺";
		if nLeftTime >= 3600 then
			szText = szText .. math.floor( nLeftTime / 3600 ) .. "ʱ";
		end
		if nLeftTime >= 60 then
			szText = szText .. math.floor( math.fmod( nLeftTime, 3600 ) / 60 ) .. "��";
		end
		szText = szText .. math.fmod( nLeftTime, 60 ) .. "��";
		--TODO HERE ����horizonal
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
	end
end

------------------	�����ͼ	------------------------------

function ClanMapBondClanDefInfo( ClanViewer,cityInfo )
	local MemberOnLineValue = GetOnlineMember( ClanViewer.m_clanInfo.m_nClanMemNum, ClanViewer.m_Members );
	local t_ClanDefInfo = {
							{"ClanMap_Name",			ClanViewer.m_clanInfo.m_szClanName }, -- ����
							{"ClanMapSkill_Lv",			ClanViewer.m_clanInfo.m_nClanLevel }, -- �ȼ�
							{"ClanMap_Prosper",			ClanViewer.m_clanInfo.m_nActiveValue }, -- ���׶�
							{"ClanMap_BuildingFont",	ClanViewer.m_clanInfo.m_nBuildingValue.."/10000" }, -- �����
							--{"ClanMap_President",		ClanViewer.m_PresidentInfo.m_szRoleName }, -- �᳤
							{"ClanMap_Money",			ClanViewer.m_clanInfo.m_nMoney }, -- ��Ǯ
							{"ClanMap_MemberOnLine",	MemberOnLineValue.."/"..ClanViewer.m_clanInfo.m_nClanMemNum }, -- ��������
							{"ClanMap_CityLevelFont",	cityInfo.m_nLevel }, -- ���еȼ�	
							{"ClanMap_TechiqueFont",	ClanViewer.m_clanInfo.m_nSkillPoint},	--�Ƽ���
						  };
	ClanMap_NameBtn:SetClientUserData( 0,ClanViewer.m_clanInfo.m_uClanId );
	for i=1, table.getn( t_ClanDefInfo ) do
		local FontName = getglobal( t_ClanDefInfo[i][1] );
		FontName:SetText( t_ClanDefInfo[i][2] );
	end
	ClanMap_NoticeRich:SetText( ClanViewer.m_clanInfo.m_szBulletinText, 255, 254, 189 );
	if ClanViewer.m_clanInfo.m_BulletinTextLastEditTime == 0 then
		ClanMap_EditNoticesTimeFont:SetText("");
	else
		local szModifyDate = os.date( "%Y/%m/%d",ClanViewer.m_clanInfo.m_BulletinTextLastEditTime );
		ClanMap_EditNoticesTimeFont:SetText( szModifyDate );
	end
	if ClanMap_President:GetTextExtentWidth( ClanViewer.m_PresidentInfo.m_szRoleName ) > 115 then
		ClanMap_President:SetText( string.sub( ClanViewer.m_PresidentInfo.m_szRoleName,0,9 ) .. "..." );
		ClanMap_PresidentNameBtn:SetClientString( ClanViewer.m_PresidentInfo.m_szRoleName );
	else
		ClanMap_President:SetText( ClanViewer.m_PresidentInfo.m_szRoleName );
		ClanMap_PresidentNameBtn:SetClientString( "" );
	end
end

function ClanFrameMap_OnShow()
	
end

function ClearClanMapNpcPosBtn()
	for i=1,10 do
		local button = getglobal( "ClanFrameMapNpcPosBtn"..i );
		SetButtonInfo( { szName = button:GetName(), ClientString = "", ClientUserData0 =0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
		button:Hide();
	end
	for i=1,6 do 
		local btn = getglobal("ClanFrameMapIconBtn"..i);
		btn:Hide();
	end
end

local t_ClanBuildingName = {
							[1] = {"kejizhongxin","�Ƽ�����"},
							[2] = {"caijizhongxin","�ɼ�����"},
							[3] = {"gongchang","����"},
							[4] = {"shoulan","����"},
							[5] = {"","�Ƿ�"},
							[6] = {"chengshizhujianzhu","ָ������"},
							}

function ReSetClanMapInfo( cityInfo )
	ClearClanMapNpcPosBtn();
	ClanFrameMapTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	local cityInfo = GameClanManager:getClanCity():getCityInfo();
	if not ( cityInfo.m_nLevel > 0 ) then
		return;
	end	
	local tex = "uires\\DiTuLei\\QuYuDiTu\\"..CLAN_CITY_MAP_ID..".dds";
	ClanFrameMapTex:SetTexture( tex );
	MapShower:calculateMapSize( CLAN_CITY_MAP_ID );
	local mapSize = MapShower:getMapSize( CLAN_CITY_MAP_ID );

	local nIndex = 1;
	for i = 1, MapShower:getMapNpcCount( CLAN_CITY_MAP_ID ) do 
		if nIndex > 10 then
			break;
		end
		npcInfo = MapShower:getMapNpcInfo( CLAN_CITY_MAP_ID, i-1 );
		if npcInfo.m_bShow then
			local button = getglobal("ClanFrameMapNpcPosBtn"..nIndex);
			local szFuncDesc = getNpcDesc( npcInfo.m_nId );
			if szFuncDesc ~= "" then
				szFuncDesc = szFuncDesc.." ";
			end
			SetButtonInfo( { szName = button:GetName(), ClientString =  szFuncDesc..npcInfo.m_szName, 
								ClientUserData0 = nNpcId, ClientUserData1 = npcInfo.m_nRealX, ClientUserData2 = npcInfo.m_nRealY } );
			local mapSize = MapShower:getMapSize( CLAN_CITY_MAP_ID );
			local texOffset	= getPointCoordOnTex( npcInfo.m_nRealX, npcInfo.m_nRealY, mapSize.first, mapSize.second, CLAN_CITY_MAP_ID );
			button:SetPoint( "center", "ClanFrameMapTex", "topleft", texOffset.x * 415/1024, texOffset.y * 408/1024 );
			button:Show();
			nIndex = nIndex + 1;
		end		
	end

	local nIndex = 1;
	for i,names in ipairs( t_ClanBuildingName ) do
		local btn = getglobal("ClanFrameMapIconBtn"..nIndex);
		local tex = getglobal("ClanFrameMapIconBtn"..nIndex.."Tex");
		if i <= 5 then
			local level = cityInfo.m_buildings[i-1].Level;
			if level > 0 then
				local IconPath = "uires\\TuPianLei\\GongGongJianZhuTuBiao\\"..names[1]..level..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				tex:SetTexture( IconPath );
				btn:Show();
			else
				btn:Hide();
			end
			btn:SetClientUserData( 0,i );
			btn:SetClientUserData( 1,level );
		else
			local IconPath = "uires\\TuPianLei\\GongGongJianZhuTuBiao\\"..names[1]..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			tex:SetTexture( IconPath );
			btn:SetClientUserData( 0,i );
			btn:SetClientUserData( 1,cityInfo.m_nLevel );
			btn:Show();
		end
		nIndex = nIndex + 1;
	end
end

local tBuildingTech = {
						[CLAN_IDX_COLLECTION_CENTER_TOLUA] = {CLAN_TECH_CATEGORY_COLLECTOR_ATTRIBUTES_TOLUA,CLAN_TECH_CATEGORY_COLLECTOR_MAKING_TOLUA,CLAN_TECH_CATEGORY_COLLECTOR_ALTERATION_TOLUA},
						[CLAN_IDX_FACTORY_TOLUA] = {CLAN_TECH_CATEGORY_MACHINE_MAKING_TOLUA,CLAN_TECH_CATEGORY_TANK_MAKING_TOLUA},
						[CLAN_IDX_FARM_TOLUA] = {CLAN_TECH_CATEGORY_PET_TOLUA,CLAN_TECH_CATEGORY_PET_ALTERATION_TOLUA},
						}

function ClanFrameMapIconBtn_OnEnter()	
	local nIdx = this:GetClientUserData( 0 );
	local nLevel = this:GetClientUserData( 1 );
	local clanAdv = GameClanManager:getAdvanceInfo();
	local nCostTech = 0;
	if nIdx == CLAN_IDX_TECH_CENTER then
		nCostTech = clanAdv.UsedTechPoint;
	end
	local techs = tBuildingTech[nIdx - 1];
	if techs ~= nil then
		for i=1,table.getn( techs ) do
			local nTechLevel = clanAdv.Tech.TechLevel[ techs[i] ];
			for nLv=1,nTechLevel do
				local clanTechDef = GameClanManager:getClanTechDef( techs[i],nLv );
				if clanTechDef ~= nil then
					nCostTech = nCostTech + clanTechDef.NeedTechPointCost;
				end
			end
		end
	end
	local szTips = "�������ƣ�".. t_ClanBuildingName[nIdx][2];
	if nLevel > 0 then
		szTips = szTips .. "\n�����ȼ���".. nLevel .."��";
	end
	if nCostTech > 0 then
		szTips = szTips.."\n��Ͷ��ĿƼ��㣺"..nCostTech;
	end
	SetGameTooltips( this:GetName(),szTips );
	this:SetSize( 48,48 );
end

function ClanFrameMapIconBtn_OnLeave()
	GameTooltipOnLeave();
	this:SetSize( 32,32 );
end

--##	XML_page: 932-1336		##
--##	��ṫ���������		##
--##	LUA_page: 681-844		##
--@Desc:�����Ա�����б���ʾʱ��Ӧ
-- ���汻ѡ�е������Ϣ
local t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };

local t_ApplyJoinListData = {};

local SORT_TYPE_PLAYERNAME	= 1;		--���������
local SORT_TYPE_PLAYERLEVEL	= 2;		-- ��Ҽ�������
local SORT_TYPE_PLAYERCAREER= 3;		--���ְҵ����



local nCurApplySortType = SORT_TYPE_PLAYERNAME;
local bCurApplySortDesc = true;


function SortApplyJoin( a,b )
	local bResult;
	if nCurApplySortType == SORT_TYPE_PLAYERNAME then
		bResult= a.name > b.name;
	elseif nCurApplySortType == SORT_TYPE_PLAYERLEVEL then
		bResult = a.level > b.level;
	elseif nCurApplySortType == PLAYERCAREER then
		bResult = a.career > b.career;
	end
	if bCurApplySortDesc then
		return bResult;
	else
		return not bResult;
	end
end

function ApplyJoinClan_OnLoad()
	this:RegisterEvent("GE_CLAN_JOINLIST");
end

function ApplyJoinClan_OnEvent()
	if (arg1 == "GE_CLAN_JOINLIST" ) then
		local slider = getglobal( "ApplyJoinClan_ScrollBar" );
		local slider = getglobal( "ApplyJoinClan_ScrollBar" );
		t_ApplyJoinListData = {};		
		local ApplyJoinList = GameClanManager:getApplyJoinList();
		for i=1,ApplyJoinList.m_nRoleNum do			
			table.insert( t_ApplyJoinListData,{ name=ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_szRoleName,level=ApplyJoinList.m_ApplyRoles[i-1].Level,career=ApplyJoinList.m_ApplyRoles[i-1].Career } );
		end
		table.sort( t_ApplyJoinListData,SortApplyJoin );
		SetScrollBar( "ApplyJoinClan_ScrollBar", MAXAPPLYJOINCLANROW, ApplyJoinList.m_nRoleNum );		
		if slider:IsShown() then
			UpdateApplyJoinClanList( slider:GetValue() + 1 );
		else
			UpdateApplyJoinClanList( 1 );
		end
	end
end

function ApplyJoinClan_OnShow()
	GameClanManager:clan_Opt_LookJoinList();
	t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };
	local slider = getglobal( "ApplyJoinClan_ScrollBar" );
	slider:SetValue( 0 );

	SetScrollBar( "ApplyJoinClan_ScrollBar", MAXAPPLYJOINCLANROW, table.getn( t_ApplyJoinListData ) );
	
	if slider:IsShown() then
		UpdateApplyJoinClanList( slider:GetValue() + 1 );
	else
		UpdateApplyJoinClanList( 1 );
	end	
	ApplyJoinClanFrameNameEdit:SetText("");
	ApplyJoinClanFrame:SetPoint( "center", "$parent", "center", 0, 0 );
end

-- ���ݽ�ɫ���ֻ�ý�ɫ��Ϣ
function GetApplyMemberInfo( szRoleName )
	local ApplyJoinList = GameClanManager:getApplyJoinList();
	for i = 1, ApplyJoinList.m_nRoleNum do
		local member = ApplyJoinList.m_ApplyRoles[i-1].RoleMember;
		if member.m_szRoleName == szRoleName then
			return member;
		end
	end

	return nil;
end

--@Desc:�󶨹�����ϸ��Ϣ( ��ǰ�� )
function BondingApplyClanList( ApplyJoinList, nClientId )
	local t_ApplyClanList = {
								{"ApplyJoinClanInfo"..nClientId.."NameFont", ApplyJoinList.name }, -- ����
								{"ApplyJoinClanInfo"..nClientId.."LvFont",	ApplyJoinList.level }, -- �ȼ�
								{"ApplyJoinClanInfo"..nClientId.."CareerFont",GetCareerName(ApplyJoinList.career) }, -- ְҵ
							}
	if nClientId > MAXAPPLYJOINCLANROW then return end 
	for i=1, table.getn( t_ApplyClanList ) do
		local font = getglobal( t_ApplyClanList[i][1] );
		font:SetText( t_ApplyClanList[i][2] );
	end
	local btn = getglobal( "ApplyJoinClanInfo"..nClientId );
	btn:Enable();
end

--@Desc:ˢ�¹��������б�
function UpdateApplyJoinClanList( nPage )
	for i = 1 , MAXAPPLYJOINCLANROW do
		local button = getglobal("ApplyJoinClanInfo"..i);
		local t_fons = { "ApplyJoinClanInfo"..i.."NameFont", "ApplyJoinClanInfo"..i.."LvFont", "ApplyJoinClanInfo"..i.."CareerFont" };
		for _, name in ipairs( t_fons ) do
			local font = getglobal( name );
			font:SetText( "" );
		end
		button:Disable();
	end
	DisCheckAllBtn( "ApplyJoinClanInfo", MAXAPPLYJOINCLANROW );

	local nCount		= 1;

	for i = nPage, table.getn( t_ApplyJoinListData )  do
		BondingApplyClanList( t_ApplyJoinListData[i], nCount );
		nCount = nCount + 1;
	end

	-- ���û��ѡ�У�ѡ�е�һ��
	if t_selectApplyRole["name"] == "" or t_selectApplyRole["name"] == nil then
		local button = getglobal("ApplyJoinClanInfo1" );
		local font = getglobal( "ApplyJoinClanInfo1NameFont" );
		if font:GetText() ~= "" then
			local member = GetApplyMemberInfo( font:GetText() );
			if member ~= nil then
				button:Checked();
				t_selectApplyRole = { name=font:GetText(),heightId = member.m_RoleLongId.nHeight,lowId = member.m_RoleLongId.nLow };
			end			
		end
		return;
	end

	for i = 1, MAXAPPLYJOINCLANROW do
		local button = getglobal("ApplyJoinClanInfo"..i);
		local font = getglobal( "ApplyJoinClanInfo"..i.."NameFont" );
		if font:GetText() == t_selectApplyRole["name"] then
			button:Checked();
			return;
		end
	end
	-- ��û�ҵ���ѡ���������Ϣ˵��������Ѿ���ɾ����
	t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };
end

--�����������ť����
function ApplyJoinClan_SortNameBtn_OnClick()
	nCurSortType = SORT_TYPE_PLAYERNAME;
	bCurApplySortDesc = not bCurApplySortDesc;
	table.sort( t_ApplyJoinListData,SortApplyJoin );
	local slider = getglobal( "ApplyJoinClan_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( "ApplyJoinClan_ScrollBar", MAXAPPLYJOINCLANROW, table.getn( t_ApplyJoinListData ) );
	UpdateApplyJoinClanList( 1 );	
end

function ApplyJoinClan_SortLevelBtn_OnClick()
	nCurSortType = SORT_TYPE_PLAYERLEVEL;
	bCurApplySortDesc = not bCurApplySortDesc;
	table.sort( t_ApplyJoinListData,SortApplyJoin );
	local slider = getglobal( "ApplyJoinClan_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( "ApplyJoinClan_ScrollBar", MAXAPPLYJOINCLANROW, table.getn( t_ApplyJoinListData ) );
	UpdateApplyJoinClanList( 1 );
end

function ApplyJoinClan_SortCareerBtn_OnClick()
	nCurSortType = SORT_TYPE_PLAYERCAREER;
	bCurApplySortDesc = not bCurApplySortDesc;
	table.sort( t_ApplyJoinListData,SortApplyJoin );
	local slider = getglobal( "ApplyJoinClan_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( "ApplyJoinClan_ScrollBar", MAXAPPLYJOINCLANROW, table.getn( t_ApplyJoinListData ) );
	UpdateApplyJoinClanList( 1 );

end

--@Desc:��������б����ʱ��Ӧ
function ApplyJoinClanFontBtn_OnClick()
	DisCheckAllBtn( "ApplyJoinClanInfo", MAXAPPLYJOINCLANROW );
	this:Checked();

	-- ���ñ�ѡ���������Ϣ
	local font			= getglobal( this:GetName() .. "NameFont" );
	local szRoleName	= font:GetText();
	local member		= GetApplyMemberInfo( szRoleName );
	if member == nil then
		return;
	end
	t_selectApplyRole = { name = member.m_szRoleName, heightId = member.m_RoleLongId.nHeight, lowId = member.m_RoleLongId.nLow };
	
	if arg1 == "RightButton" then
		ClanApplyMenu:Show();
	end
end

function CheckOperateRight()
	local t_canReserveMemberPositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();
	for _, position in ipairs( t_canReserveMemberPositoins ) do
		if mainplayer:getClanPosition() == position then
			return true;
		end
	end
	return false;	
end

function CheckClanGroupLeaderRight()
	local t_canReserveMemberPositoins = { 
	CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
	CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
	CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
	CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
	local mainplayer = ActorMgr:getMainPlayer();
	for _, position in ipairs( t_canReserveMemberPositoins ) do
		if mainplayer:getClanPosition() == position then
			return true;
		end
	end
	return false;	
end

function CheckCanApplyJoinClan()
	local t_canReserveMemberPositoins = { 
		CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
		CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
		CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
		CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
	local mainplayer = ActorMgr:getMainPlayer();
	for _, position in ipairs( t_canReserveMemberPositoins ) do
		if mainplayer:getClanPosition() == position then			
			return true;
		end
	end
	ApplyJoinClanFrame:Hide();
	ShowMidTips( "��û��Ȩ�޽��и�����������Զ��رո����" );
	return false;
end

--˽��
function ApplyJoinClan_PrivateOnClick()
	if t_selectApplyRole["name"] == "" then
		return;
	end

	local t_ChatInsertFrameControl = GetChatInsertFrameControl();
	t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = t_selectApplyRole["name"] } );

	SetChatType( CL_CHAT_PRIVATE );
	UpdateChatInsertFrameEnterFocus();
	SetFocusFrame("CSChatInsert");
end

--�鿴��Ϣ
function ApplyJoinClan_DetailOnClick()
	if t_selectApplyRole["name"] == "" then
		return;
	end
	RankMgr:requestWatchPlayer(  t_selectApplyRole["name"] );
end

--��Ϊ����
function ApplyJoinClan_AddFriendOnClick()
	if t_selectApplyRole["name"] == "" then
		return;
	end
	GameFriendManager:addFriend( t_selectApplyRole["name"], "" );
end

--@Desc:��׼���
function ApplyJoinClan_ApproveBtn_OnClick()
	if not CheckCanApplyJoinClan() then
		return;
	end

	if t_selectApplyRole["name"] == "" then
		ShowMidTips("��ѡ��һ����ɫ")
		return;
	end

	GameClanManager:clan_Opt_CensorJoin( t_selectApplyRole["heightId"], t_selectApplyRole["lowId"], t_selectApplyRole["name"], 1 );
	t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };
end

--@Desc:ȫ����׼���
function ApplyJoinClan_ApproveAllBtn_OnClick()
	if not CheckCanApplyJoinClan() then
		return;
	end

	local ApplyJoinList = GameClanManager:getApplyJoinList();
	if  ApplyJoinList.m_nRoleNum == 0 then
		ShowMidTips("û�н�ɫ�������")
		return;
	end

	for i = 1, ApplyJoinList.m_nRoleNum do
		local nHightId		= ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_RoleLongId.nHeight;
		local nLowId		= ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_RoleLongId.nLow;
		local szRoleName	= ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_szRoleName;
		GameClanManager:clan_Opt_CensorJoin( nHightId, nLowId, szRoleName, 1 );
	end
	t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };
end

--@Desc:�����������
function ApplyJoinClan_DenialBtn_OnClick()
	if not CheckCanApplyJoinClan() then
		return;
	end
	
	if t_selectApplyRole["name"] == "" then
		ShowMidTips("��ѡ��һ����ɫ")
		return;
	end
	GameClanManager:clan_Opt_CensorJoin( t_selectApplyRole["heightId"], t_selectApplyRole["lowId"], t_selectApplyRole["name"], 0 );
	t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };
end

--@Desc:����ȫ���������
function ApplyJoinClan_DenialAllBtn_OnClick()
	if not CheckCanApplyJoinClan() then
		return;
	end
	
	local ApplyJoinList = GameClanManager:getApplyJoinList();
	if  ApplyJoinList.m_nRoleNum == 0 then
		ShowMidTips("û�н�ɫ�������")
		return;
	end
	for i = 1, ApplyJoinList.m_nRoleNum do
		local nHightId		= ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_RoleLongId.nHeight;
		local nLowId		= ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_RoleLongId.nLow;
		local szRoleName	= ApplyJoinList.m_ApplyRoles[i-1].RoleMember.m_szRoleName;
		GameClanManager:clan_Opt_CensorJoin( nHightId, nLowId, szRoleName, 0 );
	end
	t_selectApplyRole = { name = "", heightId = 0, lowId = 0 };
end

--@Desc:��������������ϼ�ͷʱ��Ӧ
--@return: Nothing
function ApplyJoinClan_ScrollUpBtn_OnClick()
	local slider = getglobal("ApplyJoinClan_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��������������¼�ͷʱ��Ӧ
--@return: Nothing
function ApplyJoinClan_ScrollDownBtn_OnClick()
	local slider = getglobal("ApplyJoinClan_ScrollBar");
	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ�ƶ�������ʱ������ˢ���ı�
--@return: Nothing
function ApplyJoinClan_ScrollBar_OnValueChanged()
	local slider	= getglobal("ApplyJoinClan_ScrollBar");
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		UpdateApplyJoinClanList( slider:GetValue() + 1 );
	end
end

--##	XML_page: 2061-2557		##
--##	�����⽻��ϵ����		##
--##	LUA_page: 847-984		##

local t_selecApplyClan = { name = "", id = 0, relation = "", heightId = 0, lowId = 0 };

local t_DiplomatismData = {};
local t_DiplomatismApplyData = {};

local TYPE_FRIEND = 1;	--ͬ��
local TYPE_ENEMY = 2;	--�ж�

local DiplomatismTypeUV = { [TYPE_FRIEND] = {742,86,16,16},[TYPE_ENEMY] = {725,86,16,16},}

local SORT_TYPE_CLAN_NAME		= 4;
local SORT_TYPE_CLAN_CAPTIONNAME= 5;
local SORT_TYPE_CLAN_RELATION	= 6;

local nCurDiplomatismAllSortType = SORT_TYPE_CLAN_NAME;
local bCurDiplomatismAllSortDesc = false;

local nCurDiplomatismSortType = SORT_TYPE_CLAN_NAME;
local bCurDiplomatismSortDesc = false;

function SortClanDiplomatism( a,b )
	local bResult;
	if nCurDiplomatismAllSortType == SORT_TYPE_CLAN_NAME then
		bResult = a.ClanName < b.ClanName;
	elseif nCurDiplomatismAllSortType == SORT_TYPE_CLAN_CAPTIONNAME then
		bResult = a.CaptionName < b.CaptionName
	elseif nCurDiplomatismAllSortType == SORT_TYPE_CLAN_RELATION then
		bResult = a.Type < b.Type;
	end
	if bCurDiplomatismAllSortDesc then
		return bResult;
	else
		return not bResult;
	end
end

function SortClanApplyDiplomatism( a,b )
	local bResult;
	if nCurDiplomatismSortType == SORT_TYPE_CLAN_NAME then
		bResult = a.ClanName < b.ClanName;
	elseif nCurDiplomatismSortType == SORT_TYPE_CLAN_CAPTIONNAME then
		bResult = a.CaptionName < b.CaptionName
	elseif nCurDiplomatismSortType == SORT_TYPE_CLAN_RELATION then
		bResult = a.Type < b.Type;
	end
	if bCurDiplomatismSortDesc then
		return bResult;
	else
		return not bResult;
	end
end

function ClanDiplomatism_OnLoad()
	this:RegisterEvent("GE_CLAN_DIPLOMATISM");
	for i=1,MAXDIPLOMATISM do
		local btn = getglobal( "ClanDiplomatismAllInfo"..i );
		--btn:SetOverlayRegion( -5, -7, 240, 36 );
	end
	for i=1, MAXDIPLOMATISMLIST do
		local btn = getglobal( "ClanDiplomatismInfo"..i );		
		--btn:SetOverlayRegion( -2, -7, 302, 34 );
	end
end

function ClanDiplomatismAllSortClanCaptionNameBtn_OnClick()
	nCurDiplomatismAllSortType = SORT_TYPE_CLAN_CAPTIONNAME;
	bCurDiplomatismAllSortDesc = not bCurDiplomatismAllSortDesc;
	table.sort( t_DiplomatismData,SortClanDiplomatism );
	local slider = getglobal( "ClanDiplomatismAll_ScrollBar" );
	slider:SetValue( 0 );
	UpdateClanDisplomatism( 1 );
end

function ClanDiplomatismAllSortClanNameBtn_OnClick()
	nCurDiplomatismAllSortType = SORT_TYPE_CLAN_NAME;
	bCurDiplomatismAllSortDesc = not bCurDiplomatismAllSortDesc;
	table.sort( t_DiplomatismData,SortClanDiplomatism );
	local slider = getglobal( "ClanDiplomatismAll_ScrollBar" );
	slider:SetValue( 0 );
	UpdateClanDisplomatism( 1 );	
end

function ClanDiplomatismSortClanNameBtn_OnClick()
	nCurDiplomatismSortType = SORT_TYPE_CLAN_CAPTIONNAME;
	bCurDiplomatismSortDesc = not bCurDiplomatismSortDesc;
	table.sort( t_DiplomatismApplyData,SortClanApplyDiplomatism );
	local slider = getglobal( "ClanDiplomatism_ScrollBar" );
	slider:SetValue( 0 );
	UpdateClanDiplomatismList( 1 );
end

function ClanDiplomatismSortClanRelationBtn_OnClick()
	nCurDiplomatismSortType = SORT_TYPE_CLAN_RELATION;
	bCurDiplomatismSortDesc = not bCurDiplomatismSortDesc;
	table.sort( t_DiplomatismApplyData,SortClanApplyDiplomatism );
	local slider = getglobal( "ClanDiplomatism_ScrollBar" );
	slider:SetValue( 0 );
	UpdateClanDiplomatismList( 1 );
end

function ClanDiplomatismSortClanCaptionNameBtn_OnClick()
	nCurDiplomatismSortType = SORT_TYPE_CLAN_NAME;
	bCurDiplomatismSortDesc = not bCurDiplomatismSortDesc;
	table.sort( t_DiplomatismApplyData,SortClanApplyDiplomatism );
	local slider = getglobal( "ClanDiplomatism_ScrollBar" );
	slider:SetValue( 0 );
	UpdateClanDiplomatismList( 1 );
end

function ClanDiplomatism_OnEvent()
	if (arg1 == "GE_CLAN_DIPLOMATISM" ) then
		local slider = getglobal( "ClanDiplomatism_ScrollBar" );
		t_DiplomatismData = {};
		t_DiplomatismApplyData = {};
		local ClanDiplomatism = GameClanManager:getClanDipLomatism();
		
		for i=1, ClanDiplomatism.m_nFriendNum do
			local chanInfo = ClanDiplomatism.m_Friends[i-1];
			table.insert( t_DiplomatismData,{ClanName=chanInfo.m_szClanName,Type=TYPE_FRIEND,CaptionName=chanInfo.m_szCaptionName,id=chanInfo.m_uClanId,nTime=chanInfo.m_nTime})
		end
		for i=1,ClanDiplomatism.m_nEnemyNum do
			local chanInfo = ClanDiplomatism.m_Enemys[i-1];
			table.insert( t_DiplomatismData,{ClanName=chanInfo.m_szClanName,Type=TYPE_ENEMY,CaptionName=chanInfo.m_szCaptionName,id=chanInfo.m_uClanId,nTime=chanInfo.m_nTime})
		end
		for i=1,ClanDiplomatism.m_nApplyFriendNum do
			local chanInfo = ClanDiplomatism.m_ApplyFriends[i-1].m_ClanInfo;
			table.insert( t_DiplomatismApplyData,{ClanName=chanInfo.m_szClanName,Type=TYPE_FRIEND,CaptionName=chanInfo.m_szCaptionName,id=chanInfo.m_uClanId})
		end
		for i=1,ClanDiplomatism.m_nApplyEnemyNum do
			local chanInfo = ClanDiplomatism.m_ApplyEnemys[i-1].m_ClanInfo;
			table.insert( t_DiplomatismApplyData,{ClanName=chanInfo.m_szClanName,Type=TYPE_ENEMY,CaptionName=chanInfo.m_szCaptionName,id=chanInfo.m_uClanId})
		end
		table.sort( t_DiplomatismData,SortClanDiplomatism );
		table.sort( t_DiplomatismApplyData,SortClanApplyDiplomatism );

		nCount = ClanDiplomatism.m_nApplyEnemyNum + ClanDiplomatism.m_nApplyFriendNum;
		SetScrollBar( "ClanDiplomatism_ScrollBar", MAXDIPLOMATISMLIST, nCount );
		if slider:IsShown() then
			UpdateClanDiplomatismList( slider:GetValue() + 1 );
		else
			UpdateClanDiplomatismList( 1 );
		end

		slider = getglobal( "ClanDiplomatismAll_ScrollBar" );
		nCount = ClanDiplomatism.m_nEnemyNum + ClanDiplomatism.m_nFriendNum;
		SetScrollBar( "ClanDiplomatismAll_ScrollBar",MAXDIPLOMATISM,nCount );
		if slider:IsShown() then
			UpdateClanDisplomatism( slider:GetValue() + 1 );
		else
			UpdateClanDisplomatism( 1 );
		end
	end
end

function ClanDiplomatism_OnShow()
	-- ��������������⽻��ϵ�б�
	t_selecApplyClan = { name = "", id = 0, relation = "", heightId = 0, lowId = 0 };
	GameClanManager:clan_Opt_LookDiplomatism();
	DisCheckAllBtn( "ClanDiplomatismAllInfo", MAXDIPLOMATISM );
	DisCheckAllBtn( "ClanDiplomatismInfo", MAXDIPLOMATISMLIST );

	local slider = getglobal( "ClanDiplomatism_ScrollBar" );
	slider:SetValue( 0 );
	local ClanDiplomatism = GameClanManager:getClanDipLomatism();
	local nCount = ClanDiplomatism.m_nApplyEnemyNum + ClanDiplomatism.m_nApplyFriendNum;
	SetScrollBar( "ClanDiplomatism_ScrollBar", MAXDIPLOMATISMLIST, nCount );

	if slider:IsShown() then
		UpdateClanDiplomatismList( slider:GetValue() + 1 );
	else
		UpdateClanDiplomatismList( 1 );
	end

	slider = getglobal( "ClanDiplomatismAll_ScrollBar" );
	slider:SetValue( 0 );
	nCount = ClanDiplomatism.m_nEnemyNum + ClanDiplomatism.m_nFriendNum;
	SetScrollBar( "ClanDiplomatismAll_ScrollBar",MAXDIPLOMATISM,nCount );
	if slider:IsShown() then
		UpdateClanDisplomatism( slider:GetValue() + 1 );
	else
		UpdateClanDisplomatism( 1 )
	end
	this:SetPoint( "center", "$parent", "center", 0, 0 );
end

--@Desc:�󶨹����⽻�б�
function BondingDiplomatismList( ClanInfo, nClientId )
	local t_fonts =		{	["ClanDiplomatismInfo"..nClientId.."ClanIdFont"]	= { value = ClanInfo.m_uClanId },
							["ClanDiplomatismInfo"..nClientId.."ClanNameFont"]	= { value = ClanInfo.m_szClanName }, 
							["ClanDiplomatismInfo"..nClientId.."RelationFont"]	= { value = ClanInfo.Type }, 
						};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );
	end

	local btn = getglobal( "ClanDiplomatismInfo"..nClientId );
	btn:Enable();
end

function ClanDiplomatismIcon_OnEnter()
	local Type = this:GetClientUserData( 0 );
	if Type == TYPE_FRIEND then
		SetGameTooltips(this:GetName(),"ͬ��");
	elseif Type == TYPE_ENEMY then
		SetGameTooltips(this:GetName(),"�ж�");
	end
	local parent = getglobal( this:GetParent() );
	parent:Checked();
end

function ClanDiplomatismIcon_OnLeave()
	local parent = getglobal( this:GetParent() );
	parent:DisChecked();
	GameTooltipOnLeave();
end

-- ˢ�¹����⽻
function UpdateClanDisplomatism( nStarIndex )
	ClearDiplomatism();
	DisCheckAllBtn( "ClanDiplomatismAllInfo", MAXDIPLOMATISM );

	local nIndex = 1;
	for i = nStarIndex ,table.getn( t_DiplomatismData ) do
		if nIndex > MAXDIPLOMATISM then
			break;
		end
		local btn				= getglobal( "ClanDiplomatismAllInfo"..nIndex );
		local TexBtn			= getglobal( "ClanDiplomatismAllInfo"..nIndex.."Icon" );
		local RelationTex		= getglobal( "ClanDiplomatismAllInfo"..nIndex.."IconRelationTex" );
		local NameFont			= getglobal( "ClanDiplomatismAllInfo"..i.."NameFont" );
		local CaptionNameFont	= getglobal( "ClanDiplomatismAllInfo"..nIndex.."CaptionNameFont" );
		local TexUV = DiplomatismTypeUV[t_DiplomatismData[i].Type];
		if not TexBtn:IsShown() then
			TexBtn:Show();
		end
		TexBtn:SetClientUserData( 0,t_DiplomatismData[i].Type );
		TexBtn:Enable();
		RelationTex:Show();
		RelationTex:SetTexUV( TexUV[1],TexUV[2],TexUV[3],TexUV[4] );
		NameFont:SetText(t_DiplomatismData[i].ClanName);
		CaptionNameFont:SetText( t_DiplomatismData[i].CaptionName );
		
		btn:Enable();
		btn:SetClientUserData( 0,i );
		nIndex = nIndex + 1;
	end
	for i=nIndex,MAXDIPLOMATISM do
		local btn				= getglobal( "ClanDiplomatismAllInfo"..i );
		local TexBtn			= getglobal( "ClanDiplomatismAllInfo"..i.."Icon" );
		local RelationTex		= getglobal( "ClanDiplomatismAllInfo"..i.."IconRelationTex" );
		local NameFont			= getglobal( "ClanDiplomatismAllInfo"..i.."NameFont" );
		local CaptionNameFont	= getglobal( "ClanDiplomatismAllInfo"..i.."CaptionNameFont" );
		btn:Disable();
		TexBtn:Disable();
		TexBtn:Hide();
		TexBtn:SetClientUserData( 0,0 );
		RelationTex:Hide();
		NameFont:SetText("");
		CaptionNameFont:SetText("");
		btn:SetClientUserData( 0,0 );
	end
end

--@Desc:ˢ�¹��������⽻��ϵ�б�
function UpdateClanDiplomatismList( nPage )
	--ClearDiplomatisInfo();
	ClearDiplomatismList()
	DisCheckAllBtn( "ClanDiplomatismInfo", MAXDIPLOMATISMLIST );
	
	--�����ϵ�б�
	local nIndex = 1;
	for i=nPage,table.getn( t_DiplomatismApplyData ) do
		if nIndex > MAXDIPLOMATISMLIST then
			break;
		end
		local btn					= getglobal( "ClanDiplomatismInfo"..nIndex );
		local ClanNameFont			= getglobal( "ClanDiplomatismInfo"..nIndex.."ClanNameFont" );
		local ClanCaptionNameFont	= getglobal( "ClanDiplomatismInfo"..nIndex.."ClanCaptionNameFont" );
		local RelationFont			= getglobal( "ClanDiplomatismInfo"..nIndex.."RelationFont");

		btn:Enable();
		btn:SetClientUserData(0,t_DiplomatismApplyData[i].id);
		ClanNameFont:SetText( t_DiplomatismApplyData[i].ClanName );
		ClanCaptionNameFont:SetText( t_DiplomatismApplyData[i].CaptionName );
		if t_DiplomatismApplyData[i].Type == TYPE_FRIEND then
			RelationFont:SetText("ͬ��")
		else
			RelationFont:SetText("�ж�");
		end
		if nIndex == 1 and t_selecApplyClan["id"] == 0 then
			t_selecApplyClan = { name = ClanNameFont:GetText(), id = btn:GetClientUserData(0), relation = RelationFont:GetText() };
		end
		nIndex = nIndex + 1;
	end
	
	if t_selecApplyClan["id"] == 0 then
		return;		
	end
	-- checkסԭ����ѡ���İ�ť
	for i = 1, MAXDIPLOMATISMLIST do
		local btn = getglobal( "ClanDiplomatismInfo" .. i );
		
		if btn:GetClientUserData(0) == t_selecApplyClan["id"] then
			btn:Checked();
			return;
		end
	end
end

--�����ϵ
function ClanDiplomatismAllBtn_OnClick()
end

function ClanDiplomatismAllBtn_OnEnter()
	local nIdx = this:GetClientUserData( 0 );
	if t_DiplomatismData[nIdx].nTime > 0 then
		local severTime = GameClanManager:getServerTime();
		local nLeft = t_DiplomatismData[nIdx].nTime - severTime;
		if nLeft > 0 then
			local szText = "�ж�ʣ��ʱ�䣺";
			if nLeft >= 3600 then
				szText = szText .. math.floor( nLeft / 3600 ) .. "ʱ";
			end
			if nLeft >= 60 then
				szText = szText .. math.floor( math.fmod( nLeft, 3600 ) / 60 ) .. "��";
			end
			szText = szText .. math.fmod( nLeft, 60 ) .. "��";			
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
							button = this:GetName(), horizonal = "right" } );
		end
	end
end

function ClanDiplomatismFontBtn_OnClick()
	local RelationFont	= getglobal(this:GetName().."RelationFont");
	local nameFont		= getglobal(this:GetName().."ClanNameFont");
	local slider		= getglobal( "ClanDiplomatism_ScrollBar" );
	
	
	DisCheckAllBtn( "ClanDiplomatismInfo", MAXDIPLOMATISMLIST );
	this:Checked();
	
	t_selecApplyClan = { name = nameFont:GetText(), id = this:GetClientUserData(0), relation = RelationFont:GetText() };
	
	local ClanDiplomatism = GameClanManager:getClanDipLomatism();
	for i = 1, ClanDiplomatism.m_nApplyFriendNum do
		local clanInfo = ClanDiplomatism.m_ApplyFriends[i-1].m_ClanInfo;
		if t_selecApplyClan["id"] == clanInfo.m_uClanId then
			t_selecApplyClan["heightId"]	= clanInfo.m_clanLongId.nHeight;
			t_selecApplyClan["lowId"]		= clanInfo.m_clanLongId.nLow;
			return;
		end
	end

	for i = 1, ClanDiplomatism.m_nApplyEnemyNum do
		local clanInfo = ClanDiplomatism.m_ApplyEnemys[i-1].m_ClanInfo;
		if t_selecApplyClan["id"] == clanInfo.m_uClanId then
			t_selecApplyClan["heightId"]	= clanInfo.m_clanLongId.nHeight;
			t_selecApplyClan["lowId"]		= clanInfo.m_clanLongId.nLow;
			return;
		end
	end
end

function CheckCanOperateDiplomatism()
	local t_canSeeRelationPositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
	local mainplayer = ActorMgr:getMainPlayer();

	for _, position in ipairs( t_canSeeRelationPositoins ) do
		if mainplayer:getClanPosition() == position then			
			return;
		end
	end
	ClanDiplomatismFrame:Hide();
	ShowMidTips( "��û��Ȩ�޽����⽻��ϵ�Ĳ��������Զ��رո����" );
end

function ClanDiplomatism_ConfirmBtn_OnClick()
	CheckCanOperateDiplomatism();
	if t_selecApplyClan["id"] == 0 then
		ShowMidTips("��ѡ��һ������");
		return;
	end
	if t_selecApplyClan.heightId == nil or t_selecApplyClan.lowId == nil then
		local ClanDiplomatism = GameClanManager:getClanDipLomatism();
		for i = 1, ClanDiplomatism.m_nApplyFriendNum do
			local clanInfo = ClanDiplomatism.m_ApplyFriends[i-1].m_ClanInfo;
			if t_selecApplyClan["id"] == clanInfo.m_uClanId then
				t_selecApplyClan["heightId"]	= clanInfo.m_clanLongId.nHeight;
				t_selecApplyClan["lowId"]		= clanInfo.m_clanLongId.nLow;
			end
		end
		for i = 1, ClanDiplomatism.m_nApplyEnemyNum do
			local clanInfo = ClanDiplomatism.m_ApplyEnemys[i-1].m_ClanInfo;
			if t_selecApplyClan["id"] == clanInfo.m_uClanId then
				t_selecApplyClan["heightId"]	= clanInfo.m_clanLongId.nHeight;
				t_selecApplyClan["lowId"]		= clanInfo.m_clanLongId.nLow;
			end
		end
	end
	local ClanDiplomatism = GameClanManager:getClanDipLomatism();
	local nUnionNum		  = ClanDiplomatism.m_nFriendNum;
	if t_selecApplyClan["relation"] == "ͬ��" then
		GameClanManager:clan_Opt_CensorJoinUnion( t_selecApplyClan["heightId"], t_selecApplyClan["lowId"], t_selecApplyClan["id"],
													t_selecApplyClan["name"], 1 );
	end

	if t_selecApplyClan["relation"] == "�ж�" then
		GameClanManager:clan_Opt_CensorJoinEnemy( t_selecApplyClan["heightId"], t_selecApplyClan["lowId"], t_selecApplyClan["id"],
													t_selecApplyClan["name"], 1 );
	end

	t_selecApplyClan = { name = "", id = 0, relation = "", heightId = 0, lowId = 0 };
end

function ClanDiplomatism_CancleBtn_OnClick()
	CheckCanOperateDiplomatism();
	if t_selecApplyClan["id"] == 0 then
		ShowMidTips("��ѡ��һ������");
		return;
	end
	if t_selecApplyClan.heightId == nil or t_selecApplyClan.lowId == nil then
		local ClanDiplomatism = GameClanManager:getClanDipLomatism();
		for i = 1, ClanDiplomatism.m_nApplyFriendNum do
			local clanInfo = ClanDiplomatism.m_ApplyFriends[i-1].m_ClanInfo;
			if t_selecApplyClan["id"] == clanInfo.m_uClanId then
				t_selecApplyClan["heightId"]	= clanInfo.m_clanLongId.nHeight;
				t_selecApplyClan["lowId"]		= clanInfo.m_clanLongId.nLow;
			end
		end
		for i = 1, ClanDiplomatism.m_nApplyEnemyNum do
			local clanInfo = ClanDiplomatism.m_ApplyEnemys[i-1].m_ClanInfo;
			if t_selecApplyClan["id"] == clanInfo.m_uClanId then
				t_selecApplyClan["heightId"]	= clanInfo.m_clanLongId.nHeight;
				t_selecApplyClan["lowId"]		= clanInfo.m_clanLongId.nLow;
			end
		end
	end

	local ClanDiplomatism = GameClanManager:getClanDipLomatism();
	local nUnionNum		  = ClanDiplomatism.m_nFriendNum;
	if t_selecApplyClan["relation"] == "ͬ��" then
		GameClanManager:clan_Opt_CensorJoinUnion( t_selecApplyClan["heightId"], t_selecApplyClan["lowId"], t_selecApplyClan["id"],
													t_selecApplyClan["name"], 0  );
	end

	if t_selecApplyClan["relation"] == "�ж�" then
		GameClanManager:clan_Opt_CensorJoinEnemy( t_selecApplyClan["heightId"], t_selecApplyClan["lowId"], t_selecApplyClan["id"],
													t_selecApplyClan["name"], 0 );
	end

	t_selecApplyClan = { name = "", id = 0, relation = "", heightId = 0, lowId = 0 };
end

--@Desc:��������������ϼ�ͷʱ��Ӧ
--@return: Nothing
function ClanDiplomatism_ScrollUpBtn_OnClick()
	local slider = getglobal("ClanDiplomatism_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��������������¼�ͷʱ��Ӧ
--@return: Nothing
function ClanDiplomatism_ScrollDownBtn_OnClick()
	local slider = getglobal("ClanDiplomatism_ScrollBar");
	local nMaxValue = slider:GetMaxValue();
	local nValue = slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ�ƶ�������ʱ������ˢ���ı�
--@return: Nothing
function ClanDiplomatism_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "ClanDiplomatism_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateClanDiplomatismList( nValue + 1 );
	end
end

function ClanDiplomatismAll_ScrollUpBtn_OnClick()
	local slider = getglobal("ClanDiplomatismAll_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function ClanDiplomatismAll_ScrollDownBtn_OnClick()
	local slider = getglobal("ClanDiplomatismAll_ScrollBar");
	local nMaxValue = slider:GetMaxValue();
	local nValue = slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function ClanDiplomatismAll_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "ClanDiplomatismAll_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateClanDisplomatism( nValue + 1 );
	end
end

function ClanDiScrollBar_OnMouseWheel()
	local slider = nil;
	if isPointInFrame( "Clan_LeftPartBkgBtn" ) then
		slider = getglobal("ClanDiplomatismAll_ScrollBar");
	elseif isPointInFrame( "Clan_RightPartBkgBtn" ) then
		slider = getglobal("ClanDiplomatism_ScrollBar"); 
	end
	if not slider:IsShown() then
		return;
	end

	local nScrollLine	= arg1;
	local absChange		= math.abs( arg1 );
	-- �����Ϲ���
	if arg1 > 0 then
		local nValue = slider:GetValue() - absChange * slider:GetValueStep();
		if nValue >= 0 then
			slider:SetValue( nValue );
		else
			slider:SetValue( 0 );
		end
	-- �����¹���
	else
		local nMaxValue = slider:GetMaxValue();
		local nValue	= slider:GetValue() + absChange * slider:GetValueStep();

		if nValue <= nMaxValue then
			slider:SetValue( nValue );
		else
			slider:SetValue( nMaxValue );
		end
	end
end

--##��������##
local nApplyJoinHightId		= 0;
local nApplyJoinLowId		= 0;
local nApplyJoinClanId		= 0;
local szApplyJoinClanName;
local szApplyJoinHostName;

function Clan_InputFrame_OnShow()
	local edit = getglobal( this:GetName() .. "_Input_SetNameEditBox" );
	edit:Clear();
	edit:SetText("");
	edit:Show();

	SetFocusFrame( edit:GetName() );
	this:SetPoint( "center", "$parent", "center", 0, 0 );
end

--@Desc:���س���Ӧ
function Clan_Input_OnEnterPressed()
	Clan_InputConfirmBtn_OnClick();
end

function askJoinClan( nHightId, nLowId, nClanId, szClanName, szHostName )
	nApplyJoinHightId	= nHightId;
	nApplyJoinLowId		= nLowId;
	nApplyJoinClanId	= nClanId;
	szApplyJoinClanName	= szClanName;
	szApplyJoinHostName	= szHostName;
	RequestJoinClanFrame_InfoFont:SetText( szHostName.."���������"..szClanName.."����" );
	RequestJoinClanFrame:Show();
end

local nJoinUnionHightId		= 0;
local nJoinUnionLowId		= 0;
local nJoinUnionClanId		= 0;
local szJoinUnionClanName;
local szJoinUnionHostName;

function askJoinUnion( nHightId, nLowId, nClanId, szClanName, szHostName )
	MessageBox("��������", szHostName.."���������"..szClanName.."����");
	nJoinUnionHightId	= nHightId;
	nJoinUnionLowId		= nLowId;
	nJoinUnionClanId	= nClanId;
	szJoinUnionClanName	= szClanName;
	szJoinUnionHostName	= szHostName;
end

function AnswerJoinUnion()
	if nApplyJoinHightId ~= NONE and nApplyJoinLowId ~= NONE then
		GameClanManager:clan_Opt_AnserJoinUnion(nApplyJoinHightId, nApplyJoinLowId, nApplyJoinClanId, szApplyJoinClanName, szApplyJoinHostName, 1 );
	end
end

--@Desc:�����б��ҷ�ҳ��ť
function ClanList_RightPageBtn_OnClick()
	-- ����ǰҳ��Ϊ���ҳ�����������ҷ�ҳ
	if t_clanListInfo["curClanListPage"] >= t_clanListInfo["totalPage"] then
		-- ��Ϊ���һҳ��������������󣬻�����һҳ������
		GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["totalPage"] );
		return;
	end

	-- ȡ���ϴ�ѡ���Ĺ��� 
	t_clanListInfo["selectClan"]["clanID"]	= 0;
	t_clanListInfo["curClanListPage"]		= t_clanListInfo["curClanListPage"] + 1;
	GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["curClanListPage"] - 1 );
	local nClanTotalNum	= GameClanManager:getClanTotalNum();
	t_clanListInfo["totalPage"] = math.ceil( nClanTotalNum/MAXCLANLISTROW );
	updataClanPage();
end


function ClanList_ToLastPageBtn_OnClick()
	-- ����ǰ��ʾҳ��Ϊ���һҳ�����ܼ������ҷ�ҳ
	if t_clanListInfo["curClanListPage"] >= t_clanListInfo["totalPage"] then
		GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["totalPage"] );
		return;
	end
	
	-- ȡ���ϴ�ѡ���Ĺ��� 
	t_clanListInfo["selectClan"]["clanID"]	= 0;
	t_clanListInfo["curClanListPage"]		= t_clanListInfo["totalPage"];
	-- �������������һҳ��������
	GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["curClanListPage"] - 1 );
	updataClanPage();
end

--@Desc:�����б���ҳ��ť
function ClanList_LeftPageBtn_OnClick()
	-- ����ǰ��ʾ��ҳ��Ϊ��һҳ�����ݣ�������ǰһҳ��������
	if t_clanListInfo["curClanListPage"] <= 1 then
		GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["curClanListPage"] - 1 );	
		return;
	end
	
	-- ȡ���ϴ�ѡ���Ĺ��� 
	t_clanListInfo["selectClan"]["clanID"]	= 0;
	t_clanListInfo["curClanListPage"]		= t_clanListInfo["curClanListPage"] - 1;
	-- �������������һҳ��������
	GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["curClanListPage"] - 1 );	
	updataClanPage();
end

--@Desc:�����б��һҳ��ť
function ClanList_ToFirstPageBtn_OnClick()
	-- ����ǰ��ʾ��ҳ����������һҳ���������������ҳ
	if t_clanListInfo["curClanListPage"] <= 1 then
		GameClanManager:clan_Opt_GetPageClan( 0 );
		return;
	end

	-- ȡ���ϴ�ѡ���Ĺ��� 
	t_clanListInfo["selectClan"]["clanID"]	= 0;
	t_clanListInfo["curClanListPage"]		= 1;
	-- �������������һҳ��������
	GameClanManager:clan_Opt_GetPageClan( t_clanListInfo["curClanListPage"] - 1 );
	updataClanPage();
end

function ClanList_SearchClanBtn_OnClick()
	local edit = getglobal( "Clan_Search_ClanNameEditBox" );
	local szText = edit:GetText();
	--[[
	local nNum = tonumber( szText );
	if nNum ~= nil then
		GameClanManager:clan_Opt_SearchClanByID( nNum );
	else
		GameClanManager:clan_Opt_SearchClanByName( szText );
	end
	]]--
	GameClanManager:clan_Opt_SearchClanByName( szText );
	edit:Clear();
end

function ClanList_SearchClanBt_OnLeave()
	GameTooltip:Hide();
end

function ClanInputFrame_EditBackBtn_OnClick()
	SetFocusFrame( this:GetParent() .. "_Input_SetNameEditBox" );
end

function ClanInputFrame_SearchClanNameEditBackBtn_OnClick()
	SetFocusFrame( "Clan_Search_ClanNameEditBox" );
end

function Clan_Edit_OnTextSet()
	if string.find(this:GetText(),"\n") then
		local szText = string.gsub(this:GetText(),"\n","");
		this:SetText(szText);
	elseif string.find(this:GetText(),"\\n") then
		local szText = string.gsub(this:GetText(),"\\n","��n");
		this:SetText(szText);
	end
end

function Clan_EditConfirmBtn_OnClick()
	local szInputName = Clan_Edit_SetNameEditBox:GetText();
	if Clan_EditFrame:GetClientString() == "�޸�����" then
		GameClanManager:clan_Opt_EditManifesto( szInputName );
		Clan_EditFrame:Hide();
	else
		local t_canEditNoticesPositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };
		local mainplayer = ActorMgr:getMainPlayer();
		for _, position in ipairs( t_canEditNoticesPositoins ) do
			if mainplayer:getClanPosition() == position then				
				GameClanManager:clan_Opt_EditBulletin( szInputName );
				Clan_EditFrame:Hide();
				return;
			end
		end
		Clan_EditFrame:Hide();
		ShowMidTips( "������û��Ȩ�ޱ༭���ṫ�棬���Զ��رո�ҳ��" );
	end
end

function ClanEditFrame_EditBackBtn_OnClick()
	SetFocusFrame("Clan_Edit_SetNameEditBox");
end

local t_joinClanFrameInfo = { startTime = 0 };
-- ������빫������
function RequestJoinClanFrame_OnShow()
	t_joinClanFrameInfo.startTime = GameMgr:getTickTime();
	this:SetPoint( "center", "$parent", "center", 0, 0 );
end

function RequestJoinClanFrame_OnUpdate()
	if GameMgr:getTickTime() - t_joinClanFrameInfo.startTime > 15000 then
		GameClanManager:clan_Opt_AnswerJoinClan(nApplyJoinHightId, nApplyJoinLowId, nApplyJoinClanId, szApplyJoinClanName, szApplyJoinHostName, 2 );
		this:Hide();
	end
end

-- ͬ��
function RequestJoinClanFrame_AcceptBtn_OnClick()
	GameClanManager:clan_Opt_AnswerJoinClan(nApplyJoinHightId, nApplyJoinLowId, nApplyJoinClanId, szApplyJoinClanName, szApplyJoinHostName, 1 );
	RequestJoinClanFrame:Hide();
end

-- �ܾ�
function RequestJoinClanFrame_RefuseBtn_OnClick()
	GameClanManager:clan_Opt_AnswerJoinClan(nApplyJoinHightId, nApplyJoinLowId, nApplyJoinClanId, szApplyJoinClanName, szApplyJoinHostName, 0 );
	RequestJoinClanFrame:Hide();
end

function Clan_EditFrame_OnShow()
	SetFocusFrame("Clan_EditFrame");
	this:SetPoint( "center", "$parent", "center", 0, 0 );
end

function Clan_Edit_OnEnterPressed()
	Clan_EditConfirmBtn_OnClick();
end

function showClanListFrame(nActorId )
	NpcId = nActorId;
	ClanListFrame:SetPoint( "center", "$parent", "center", 0, 0 );
	ClanListFrame:Show();
	util.testNpcDialogDistance( nActorId, ClanListFrame );
end

function ClearDiplomatism()
	for i = 1, MAXDIPLOMATISM do
		local TexBtn			= getglobal( "ClanDiplomatismAllInfo"..i.."Icon" );
		local tex				= getglobal("ClanDiplomatismAllInfo"..i.."IconRelationTex");
		local clanCaptionName	= getglobal("ClanDiplomatismAllInfo"..i.."CaptionNameFont");
		TexBtn:Disable();
		TexBtn:SetClientUserData( 0,0 );
		tex:Hide();
		clanCaptionName:SetText( "" );
	end
end
function ClearDiplomatismList()
	for i = 1, MAXDIPLOMATISMLIST do
		local btn					= getglobal( "ClanDiplomatismInfo"..i );
		local nameFont				= getglobal( "ClanDiplomatismInfo"..i .. "ClanNameFont" );
		local ClanCaptionNameFont	= getglobal( "ClanDiplomatismInfo"..i .. "ClanCaptionNameFont" );
		local RelationFont	= getglobal( "ClanDiplomatismInfo"..i .. "RelationFont" );
		btn:Disable();
		nameFont:SetText( "" );
		ClanCaptionNameFont:SetText( "" );
		RelationFont:SetText( "" );
	end
end

local t_confirmOp = {
						-- ְλת��
						["Clan_PositionChangeBtn"]	= 
						function ()
							-- �����Լ�ת��ְλ
							local mainplayer = ActorMgr:getMainPlayer();
							if t_clanFrameSelectMember["name"] == mainplayer:getName() then
								ShowMidTips( "���ܸ��Լ�����ְλ" );
								return;
							end
							local t_canPositionChangePositoins = { CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT };							
							for _, position in ipairs( t_canPositionChangePositoins ) do
								if mainplayer:getClanPosition() == position then
									local btn = getglobal( "ClanFrame_ConfirmFrame_AcceptButton" );
									local szMemberName = btn:GetClientString();
									if not GameClanManager:isClanMember( t_clanFrameSelectMember["heightId"], t_clanFrameSelectMember["lowId"] ) then
										ShowMidTips( "���"..szMemberName.."�Ѿ����ڹ���" );
										return;
									end
									GameClanManager:clan_DemiseJob( t_clanFrameSelectMember["heightId"], t_clanFrameSelectMember["lowId"],
																	t_clanFrameSelectMember["name"] );
									return;
								end
							end
							ClanFrame_ConfirmFrame:Hide();
							ShowMidTips( "��û��Ȩ������ְλ�����Զ��رո����" );
														
							-- ��ְλת�ö����Ѿ����ڹ����ˣ�����ת��
							
						end,

						-- �߳�����
						["Clan_DelMemberBtn"]	= 
						function ()
							-- ���ܽ��Լ��������
							local mainplayer = ActorMgr:getMainPlayer();
							if t_clanFrameSelectMember["name"] == mainplayer:getName() then
								ShowMidTips( "���ܽ��Լ��������" );
								return;
							end
							--�¼ӵ�ս�ų�ҲҪ��Ȩ���߳�����
							local t_canDelMemberPositoins = { 
								CLAN_POS_VICE_PRESIDENT, CLAN_POS_PRESIDENT,
								CLAN_POS_GROUP_1, CLAN_POS_GROUP_2,
								CLAN_POS_GROUP_3, CLAN_POS_GROUP_4,
								CLAN_POS_GROUP_5, CLAN_POS_GROUP_6, };
							local mainplayer = ActorMgr:getMainPlayer();
							for _, position in ipairs( t_canDelMemberPositoins ) do
								if mainplayer:getClanPosition() == position then
									-- ��ְλת�ö����Ѿ����ڹ����ˣ�����ת��
									local btn = getglobal( "ClanFrame_ConfirmFrame_AcceptButton" );
									local szMemberName = btn:GetClientString();
									if not GameClanManager:isClanMember( t_clanFrameSelectMember["heightId"], t_clanFrameSelectMember["lowId"] ) then
										ShowMidTips( "���"..szMemberName.."�Ѿ����ڹ���" );
										return;
									end
									GameClanManager:clan_Opt_DelMember( t_clanFrameSelectMember["heightId"], t_clanFrameSelectMember["lowId"],t_clanFrameSelectMember["name"] );
									t_clanFrameSelectMember = { heightId = 0, lowId = 0, name = "" };
									return;
								end
							end
							ClanFrame_ConfirmFrame:Hide();
							ShowMidTips( "��û��Ȩ�޽������߳����ᣬ���Զ��رո����" );							
							
						end,

						-- �˳�����
						["Clan_QuitClanBtn"]	= 
						function ()
							GameClanManager:clan_Opt_Exit();
							ClanFrame:Hide();
							t_clanFrameSelectMember = { heightId = 0, lowId = 0, name = "" };
						end,
					};

-- ɾ�������Ա�Ķ���ȷ�����
function ClanFrame_ConfirmFrame_OnShow()
	this:SetPoint( "center", "$parent", "center", 0, 0 );
end

function ClanFrame_ConfirmFrame_AcceptButton_OnClick()
	t_confirmOp[ClanFrame_ConfirmFrame:GetClientString()]();
	ClanFrame_ConfirmFrame:Hide();
end

function ClanFrame_ConfirmFrame_CancelButton_OnClick()
	ClanFrame_ConfirmFrame:Hide();
end

function ClearFont( t_fonts )
	for _, szName in ipairs( t_fonts ) do
		local font = getglobal( szName );
		font:SetText( "" );
	end
end

function ClanFrameMapPosBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
end

function ClanAskExitEnemy(szHostName,szClanName)
	MessageBox("�����ж�",szClanName.."��"..szHostName.."���볷���жԹ�ϵ");
end

-----------------------			���ùٽ�		------------------------------------

local t_PositionDesc = {
						[CLAN_POS_NOT_MEMBER] = {""},
						[CLAN_POS_NORMAL] = {""},
						[CLAN_POS_TECH_OFFICER] = {"1��ֻӵ�пƼ������������","����Ȩ��"},
						[CLAN_POS_COLLECTION_OFFICER] = {"1��ֻӵ�вɼ������������","����Ȩ��"},
						[CLAN_POS_FACTORY_OFFICER] = {"1��ֻӵ�й���������ز���","Ȩ��"},
						[CLAN_POS_PET_OFFICER] = {"1��ֻӵ������Ƽ��������","����Ȩ��"},
						[CLAN_POS_WALL_OFFICER] = {"1��ֻӵ�н���������ز���","Ȩ��"},
						[CLAN_POS_VICE_PRESIDENT] = {"1����ӻ�Ա","2���߳���Ա","3�����ù���","4���⽻��ϵ����","5�������ǳ�","6������/���򹫻����","7������/���򹫻Ὠ��","8������Ƽ�����","9�������������","10�����ù�Ա�Ĺٽ�"},
						[CLAN_POS_PRESIDENT] = {"���ἰ���������ع��ܶ�","����Ȩ�޲���"},
						[CLAN_POS_GROUP_1] = {"1����ӻ�Ա","2�������Ա","3������ս�ų�Ա","4�������ǳ�","5������Ƽ����������Ȩ��"},
						[CLAN_POS_GROUP_2] = {"1����ӻ�Ա","2�������Ա","3������ս�ų�Ա","4�������ǳ�","5������Ƽ����������Ȩ��"},
						[CLAN_POS_GROUP_3] = {"1����ӻ�Ա","2�������Ա","3������ս�ų�Ա","4�������ǳ�","5������Ƽ����������Ȩ��"},
						[CLAN_POS_GROUP_4] = {"1����ӻ�Ա","2�������Ա","3������ս�ų�Ա","4�������ǳ�","5������Ƽ����������Ȩ��"},
						[CLAN_POS_GROUP_5] = {"1����ӻ�Ա","2�������Ա","3������ս�ų�Ա","4�������ǳ�","5������Ƽ����������Ȩ��"},
						[CLAN_POS_GROUP_6] = {"1����ӻ�Ա","2�������Ա","3������ս�ų�Ա","4�������ǳ�","5������Ƽ����������Ȩ��"},
						}
local tCurShowDesc;
function UpdatePositionDesc()
	local pos = ClanSetPositionFrame:GetClientUserData( 2 );
	tCurShowDesc = t_PositionDesc[pos];
	local slider = getglobal( "ClanSetPositionFrameSlider" );
	SetScrollBar( "ClanSetPositionFrameSlider", 6, table.getn( tCurShowDesc ) );
	slider:SetValue( 0 );
	UpdateSliderPositionDesc( slider:GetValue() + 1 );
end

function UpdateSliderPositionDesc( idx )
	local nIndex = 1;
	for i=idx,table.getn(tCurShowDesc) do
		if nIndex > 6 then
			break;
		end
		local font = getglobal("ClanSetPositionFrameRightDesc"..nIndex);
		font:SetText( tCurShowDesc[i] );
		nIndex = nIndex + 1;
	end
	for i=nIndex,6 do
		local font = getglobal("ClanSetPositionFrameRightDesc"..i);
		font:SetText( "" );
	end
end

function ClanSetPositionFrame_OnShow()
	local rich = getglobal("ClanSetPositionFrameNameRich");
	local name = this:GetClientString();
	rich:SetText( "����#c00ff00"..name.."#nΪ��",255,255,190 );
	local memberInfo = GameClanManager:getMemberInfo( name );
	if memberInfo ~= nil then
		if string.len( memberInfo.Title ) == 0 then
			ClanSetPositionFrameNameJobEdit:SetText( GetPosition( memberInfo.Position ) );
		else
		ClanSetPositionFrameNameJobEdit:SetText( memberInfo.Title );
		end
		this:SetClientUserData( 2,memberInfo.Position );
	end
	UpdatePositionDesc();
	this:SetPoint("center","UIClient","center",0,0);
end

function ClanSetPositionFrame_OnHide()
	if ClanSetPosDropDownFrame:IsShown() then
		ClanSetPosDropDownFrame:Hide();
	end
end

function ClanSetPosFrameScrollDownBtnOnClick()
	local slider	= getglobal( "ClanSetPositionFrameSlider" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function ClanSetPosFrameScrollUpBtnOnClick()
	local slider = getglobal( "ClanSetPositionFrameSlider" );
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function ClanSetPosFrameSliderOnValueChanged()
	local slider	= getglobal( "ClanSetPositionFrameSlider" );
	UpdateSliderPositionDesc( slider:GetValue() + 1 );
end

function ClanSetPosFrameRestoreBtnOnEnter()
	SetGameTooltips(this:GetName(), "�ָ�ϵͳĬ������" );
end

function ClanSetPosFrameModifyBtnOnEnter()
	SetGameTooltips(this:GetName(), "����������ƺ�\n����˴���������" );
end

function ClanSetPositionFrameRestoreBtnOnClick()
	local pos = ClanSetPositionFrame:GetClientUserData( 2 );
	local szText = GetPosition(pos);
	local editText = ClanSetPositionFrameNameJobEdit:GetText();
	if editText == szText then
		ShowMidTips( "�˹���������ΪϵͳĬ�ϵĹ�������" );
		return;
	end
	ClanSetPositionFrameNameJobEdit:SetText( szText );
end

function ClanSetPositionFrameModifyBtnOnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local pos = ClanSetPositionFrame:GetClientUserData( 2 );
	local szTitle = ClanSetPositionFrameNameJobEdit:GetText();
	if mainplayer:getName() == ClanSetPositionFrame:GetClientString() and mainplayer:getClanPosition() ~= pos then
		ShowMidTips("�����޸��Լ���ְλ");
		return
	end
	if pos >= CLAN_POS_GROUP_1 then
		if mainplayer:getClanPosition() ~= CLAN_POS_VICE_PRESIDENT and mainplayer:getClanPosition() ~= CLAN_POS_PRESIDENT then
			ShowMidTips("��û��Ȩ�����ø�ְλ");
			return;
		end
	else
		if mainplayer:getClanPosition() < pos then
			ShowMidTips("��û��Ȩ�����ø�ְλ");
			return;
		end
	end
	local nHigh = ClanSetPositionFrame:GetClientUserData( 1 );
	local nLow = ClanSetPositionFrame:GetClientUserData( 0 );
	if szTitle == GetPosition( pos ) then
		szTitle = "";
	end
	GameClanManager:clan_Opt_Set_Job_And_Title( pos,nLow,nHigh,szTitle );
	return true;
end

function ClanSetPosFrameOkBtnOnClick()
	if ClanSetPositionFrameModifyBtnOnClick() then
		ClanSetPositionFrame:Hide();
	end
end

function ClanSetPosFrameDownBtnOnClick()
	if ClanSetPosDropDownFrame:IsShown() then
		ClanSetPosDropDownFrame:Hide();
	else
		ClanSetPosDropDownFrame:Show();
	end
end

function ClanSetPosDropDownFrameBtnOnClick()
	ClanSetPosDropDownFrame:Hide();
	local id = this:GetClientID();
	ClanSetPositionFrame:SetClientUserData( 2,id );
	ClanSetPositionFrameNameJobEdit:SetText( GetPosition( id ) );
	UpdatePositionDesc();
end

function ClanSetPosDropDownFrameOnShow()
	this:SetPoint("topleft","UIClient","topleft",ClanSetPositionFrameNameJobEdit:GetRealLeft(),ClanSetPositionFrameNameJobEdit:GetRealBottom());
	--���ݹ���ȼ����Ƿ񿪷��˻ƽ�������ÿ��ŵĹٽ�
	local clanGroupInfo = GameClanManager:getClanGroupInfo()
	local clanview = GameClanManager:getClanViewer()
	local clanInfo = clanview.m_clanInfo
	local openGroupNum = clanInfo.m_nClanLevel
	local MAX_CLAN_GROUP = 6
	local GROUP_POS_START_ID = 9
	for i = 1, MAX_CLAN_GROUP do
		local btn = getglobal( this:GetName().."Btn"..(i-1+GROUP_POS_START_ID) )
		btn:Hide()
	end
	for i = 1, clanInfo.m_nClanLevel do	
		local btn = getglobal( this:GetName().."Btn"..(i-1+GROUP_POS_START_ID) )
		btn:Show()
	end
	--����Ѿ�ʹ���˾���ƾ֤�����˵�������
	if clanGroupInfo.UseItemGroup ~= 0 then
		openGroupNum = openGroupNum + 1
		ClanSetPosDropDownFrameBtn14:SetPoint( "topleft", "ClanSetPosDropDownFrame", "topleft", 1, 1+20*(clanInfo.m_nClanLevel+3)  )
		ClanSetPosDropDownFrameBtn14:Show()
	end
	this:SetSize( this:GetWidth(), 63+20*openGroupNum )
end

function ClanMemeberOnLine( roleName,onlineFlag )
	local szMsg = "";
	if onlineFlag == 0 then
		--����
		szMsg = "#A008#c14851D" .. roleName .. "#n ������";
	elseif onlineFlag == 1 then
		--����
		szMsg = "#A008#c14851D" .. roleName .. "#n ������";
	elseif onlineFlag == 2 then
		--����
		szMsg = "#A008#c14851D" .. roleName .. "#n �����˹һ�";
	end

	local nCanPingBiChatType	= GetPingDaoChatType( { ["anim"] = "#A008" } );
	local t_color				= t_mainChatFrameTextColor[nCanPingBiChatType];
	UpdatePersonalAndAllChatRich( { ["text"] = szMsg, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );

	local historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanRich" );
	historyRich:AddText( szMsg, 122, 211, 43 );
	UpdateHistorySliderRange( { ["name"] = historyRich:GetName() } );
end

function ClanFrameName_OnTip()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "����ID��"..this:GetClientUserData(0), frame = this:GetParent(),
						button = "cursor"} );
end

function ClanFramePresident_OnTip()
	if this:GetClientString() ~= "" then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = "cursor"} );
	end
end

function ClanFrameProsper_OnTip()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�����Ա��������Խ�࣬���ٶ�Խ��", frame = this:GetParent(),
						button = "cursor"} );
end

function ClanFrameTechique_OnTip()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "������������ӿƼ���", frame = this:GetParent(),
						button = "cursor"} );
end

function ClanFrameClanMaxMemNum_OnTip()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "ѧϰ�����Ա���ܿ�����߳�Ա����", frame = this:GetParent(),
						button = "cursor"} );
end

function GetClanNextMoney()
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local ClanLevelDef		= getClanLevelDef( ClanInfo.m_nClanLevel )
	local LastCostTime		= GameClanManager:getClanCostLastTime()
	local CostMoney = 0	
	if ClanLevelDef.WorkingCost.IntervalTime > 0 then
		local timeLeast = getServerTime().sec - LastCostTime
		CostMoney = (math.ceil(timeLeast/ClanLevelDef.WorkingCost.IntervalTime))*ClanLevelDef.WorkingCost.Memory
		if CostMoney < ClanLevelDef.WorkingCost.Memory then
			CostMoney = ClanLevelDef.WorkingCost.Memory
		end
	end
	--ά������ = ÿ������*(1+(100-��ǰ���ᰲ����/100)^2/2000)
	local a = ( 100 - ClanInfo.m_nBuildingValue/100.0 )^2/2000.0
	CostMoney = CostMoney*( 1 + a )
	return CostMoney
end
function ClanFrameMoney_OnTip()
	local ClanViewer = GameClanManager:getClanViewer();
	local szMoney = GetClanNextMoney() < ClanViewer.m_clanInfo.m_nMoney and "#B"..GetClanNextMoney().."#n" or "#cff7d00"..GetClanNextMoney().."#n"
	local szInfo = "�ʽ��ȡ;����\n1. ��������������ӹ����ʽ�\n2. �򹫻�������ҿ����ӹ����ʽ�\n\nά����˵����\n1. ����ÿ#G24Сʱ#n����1��ά����\n2. ����ĵȼ�Խ�ߣ�ά����Խ��\n3. ���ᰲ����Խ�ߣ�ά����Խ��\n#B4. δ�����ά���ѣ��ۼƵ��´ν���#n\n\n�´ν���Ĺ���ά���ѣ�%s\n���´ν���ά����ʣ�ࣺ#G%2d:%2d:%2d#n"
	local nextCostTime = GameClanManager:getClanCostNextTime() - getServerTime().sec
	szInfo = string.format( szInfo, szMoney, math.floor(nextCostTime/3600), math.floor(nextCostTime%3600/60), nextCostTime%60 )
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szInfo, frame = this:GetParent(),
						button = "cursor"} );
end
function CheckMoneyForNextCost()
	if GameClanManager:getClanCostLastTime() == 0 then return end
	local mainplayer = ActorMgr:getMainPlayer()
	if not mainplayer:isInClan() then return end
	local ClanViewer = GameClanManager:getClanViewer();
	local ClanLevelDef = getClanLevelDef( ClanViewer.m_clanInfo.m_nClanLevel )
	if ClanViewer.m_clanInfo.m_nMoney < GetClanNextMoney() then
		local szInfo = ""
		local levelDownTime = ClanLevelDef.LevelDownTime - ( getServerTime().sec - GameClanManager:getClanCostLastTime() )
		local levelDownInfo = math.floor(levelDownTime/3600/24) .."��".. math.floor(levelDownTime/3600%24) .."Сʱ".. math.floor(levelDownTime/60%60) .."��".. math.floor(levelDownTime%60) .."��"
		if mainplayer:getClanPosition() == CLAN_POS_NORMAL then
			return
		else
			if ClanViewer.m_clanInfo.m_nClanLevel == 1 then
				szInfo = string.format( "���Ĺ����´�ά������Ϊ��%d�����ᵱǰ�Ĺ����ʽ�%d������֧���´�ά���ѣ�������Խ�ͣ�ά������Խ�ߣ����ά���ʽ��������³���%s��ϵͳ����ɢ����", GetClanNextMoney(), ClanViewer.m_clanInfo.m_nMoney, levelDownInfo )
			else
				szInfo = string.format( "���Ĺ����´�ά������Ϊ��%d�����ᵱǰ�Ĺ����ʽ�%d������֧���´�ά���ѣ�������Խ�ͣ�ά������Խ�ߣ����ά���ʽ��������³���%s�����Ὣ�Զ�����", GetClanNextMoney(), ClanViewer.m_clanInfo.m_nMoney, levelDownInfo )
			end
		end
		local szShowText			= "#A008"..szInfo;
		local nCanPingBiChatType	= GetPingDaoChatType( { ["anim"] = "#A008" } );
		local t_color				= t_mainChatFrameTextColor[nCanPingBiChatType];
		UpdatePersonalAndAllChatRich( { ["text"] = szShowText, ["color"] = t_color, ["chatType"] = nCanPingBiChatType } );
		historyRich = getglobal( "ChatHistoryMsgMgrFrame_ClanRich" );
		historyRich:AddText( szShowText, t_color["r"], t_color["g"], t_color["b"] );
		-- ��Ϊ��ɫ
		AddGameSysTip( szText );
	end
end
-------------------------------------------		���������������	---------------------------------------------------

local MAX_CATAGORY_NUM	= 16;		--��ǩҳ��������

local t_LabelNameList = {};
local nCurSelType = 0;			--��ǰѡ�еı�ǩ
local LabelNames = {}
local LabelTypes = {}
local LabelTypeOpen = {}

function openClanFrameProductMakingMachineFrame( nActorId )
	ClanFrameProduct:Show();
	util.testNpcDialogDistance( nActorId, ClanFrameProduct );
end

local firstMenuUV =		{	normal		= {	x = 868, y = 493, width = 156, height = 27 },
							highlight	= {	x = 868, y = 521, width = 156, height = 27 },
							push		= {	x = 417, y = 480, width = 156, height = 27 }
						};

--��ǩҳ��2���˵���ͼUV
local secondMenuUV =	{	normal		= {	x = 266, y = 911, width = 166, height = 35 },
							highlight	= {	x = 266, y = 948, width = 166, height = 35 },
							push		= {	x = 266, y = 984, width = 166, height = 35 },
						};

function SetClanFrameProductLabelList()
	local newTypeOpen = {}
	LabelTypes = {}
	LabelNames = {}
	local clanAdv = GameClanManager:getAdvanceInfo()
	for i = 0, GameClanManager:getProductNum() - 1 do
		local productDef = GameClanManager:getProductDefByIndex(i)
		local clanViewer = GameClanManager:getClanViewer()

		if productDef then

			if ClanFrameProductProduceListSelectProduceListSelectString:GetText() == "ȫ��" then

				local type = productDef.TypeName
				if type ~= "" and productDef.ResourceID == 0 then
					if not LabelNames[type] then
						LabelNames[type] = {}
						table.insert(LabelTypes, type)
						if LabelTypeOpen[type] then
							newTypeOpen[type] = true
						end
					end
					table.insert(LabelNames[type], i)
				end

			elseif ClanFrameProductProduceListSelectProduceListSelectString:GetText() == "������" then

				if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] >= productDef.TechRequired[0].Level and
				   clanViewer.m_clanInfo.m_nMoney >= productDef.NeedMoneyCost and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[0].ID) >= productDef.NeedMaterialCost[0].Num and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[1].ID) >= productDef.NeedMaterialCost[1].Num and
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[2].ID) >= productDef.NeedMaterialCost[2].Num then
					local type = productDef.TypeName
					if type ~= "" and productDef.ResourceID == 0 then
						if not LabelNames[type] then
							LabelNames[type] = {}
							table.insert(LabelTypes, type)
							if LabelTypeOpen[type] then
								newTypeOpen[type] = true
							end
						end
						table.insert(LabelNames[type], i)
					end
				end

			elseif ClanFrameProductProduceListSelectProduceListSelectString:GetText() == "��������" then

				if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] < productDef.TechRequired[0].Level or
				   clanViewer.m_clanInfo.m_nMoney < productDef.NeedMoneyCost or
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[0].ID) < productDef.NeedMaterialCost[0].Num or
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[1].ID) < productDef.NeedMaterialCost[1].Num or
				   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[2].ID) < productDef.NeedMaterialCost[2].Num then
					local type = productDef.TypeName
					if type ~= "" and productDef.ResourceID == 0 then
						if not LabelNames[type] then
							LabelNames[type] = {}
							table.insert(LabelTypes, type)
							if LabelTypeOpen[type] then
								newTypeOpen[type] = true
							end
						end
						table.insert(LabelNames[type], i)
					end
				end
			end
		end
	end
	LabelTypeOpen = newTypeOpen
	SortClanFrameProductLabelList()
end

function ClearClanFrameProductSelectData()
	for i = 1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
		local ui = getglobal("ClanFrameProductResourceNeedName"..i);
		if ui then
			ui:SetText("");
		end
		ui = getglobal("ClanFrameProductResourceTotalName"..i);
		if ui then
			ui:SetText("");
		end
	end
	ClanFrameProductSkillNeedBkgValue:SetText("");
	ClanFrameProductMoneyNeedBkgValue:SetText("");
	ClanFrameProductItemDescRich:SetText("",255,255,255);
	ClanFrameProductDescRich:SetText("",255,255,255);
	ClanFrameProductLeftTimeBkgValue:SetText("");
	ClanFrameProductMakingDescBkgIconButtonTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ClanFrameProductProgressBkgTex:SetSize(0,13);
	local progressBtn = getglobal("ClanFrameProductProgressBkg");
	progressBtn:SetClientUserData(0,0);
	ClanFrameProductMakingBkgLeftMakeNum:SetText("");
end

function UpdateClanFrameProductSelectData()
	ClearClanFrameProductSelectData();

	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local clanAdv = GameClanManager:getAdvanceInfo()
	local clanViewer = GameClanManager:getClanViewer()
	local nMakingNum = tonumber( ClanFrameProductMakeNumEdit:GetText() );
	local timeNeed		= getglobal("ClanFrameProductTimeNeedBkgValue");
	local skillPoint	= getglobal("ClanFrameProductSkillPointGainBkgValue");

	if productDef then 

		local clanTechDef = GameClanManager:getClanTechDef( productDef.TechRequired[0].TechCategory,productDef.TechRequired[0].Level );
		if clanTechDef then
			ClanFrameProductSkillNeedBkgValue:SetText(clanTechDef.Name);
		else
			ClanFrameProductSkillNeedBkgValue:SetText("")
		end

		if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] >= productDef.TechRequired[0].Level then
			ClanFrameProductSkillNeedBkgValue:SetTextColor(255, 255, 190)
		else
			local progress = GameClanManager:getTechProgress(clanTechDef.Category)
			local upgradeLevel = progress and progress.Value or 0

			if productDef.TechRequired[0].Level == upgradeLevel then
			
			local szText = string.format("%s��ѧϰ��)", clanTechDef.Name)
			ClanFrameProductSkillNeedBkgValue:SetText(szText)

			else
			
			local szText = string.format("%s��δѧϰ)", clanTechDef.Name)
			ClanFrameProductSkillNeedBkgValue:SetText(szText)

			end

			ClanFrameProductSkillNeedBkgValue:SetTextColor(255, 0, 0)
		end
		local timeNum = math.floor(productDef.TimeCost / 60)
		ClanFrameProductMoneyNeedBkgValue:SetText( productDef.NeedMoneyCost * nMakingNum );
		timeNeed:SetText( timeNum * nMakingNum.."����" );
		skillPoint:SetText( productDef.ProductTech * nMakingNum);
		if clanViewer.m_clanInfo.m_nMoney >= productDef.NeedMoneyCost * nMakingNum then
			ClanFrameProductMoneyNeedBkgValue:SetTextColor(255, 255, 190)
		else
			ClanFrameProductMoneyNeedBkgValue:SetTextColor(255, 0, 0)
		end

		ClanFrameProductItemDescRich:SetText( productDef.DescText,255,254,189 );
		local itemDef = getItemDef( productDef.ID );
		if itemDef then
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ClanFrameProductItemIconButtonTex:Show();
			ClanFrameProductItemIconButtonTex:SetTexture( IconPath );
		else
			ClanFrameProductItemIconButtonTex:Hide();
		end
		local nNum = 1;
		
		for i=1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			if productDef.NeedMaterialCost[i-1].ID > 0 then
				local itemDef = getItemDef( productDef.NeedMaterialCost[i-1].ID );
				if itemDef ~= nil then
					local Need = getglobal("ClanFrameProductResourceNeedName"..i);
					local Total = getglobal("ClanFrameProductResourceTotalName"..i);
					local must = productDef.NeedMaterialCost[i-1].Num
					local max = GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[i-1].ID);
					if Need then
						Need:SetText(string.format("%s %d", itemDef.Name, must * nMakingNum));
						if must * nMakingNum <= max then
							Need:SetTextColor(255, 255, 190);
						else
							Need:SetTextColor(255, 0, 0);
						end
					end
					if Total then
						Total:SetText(string.format("%s %d", itemDef.Name, max));
					end
					nNum = nNum + 1;						
				end
			end
		end
		for i=nNum, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			local ui = getglobal("ClanFrameProductResourceNeedName"..i);
			if ui then
				ui:SetText("");
			end
			ui = getglobal("ClanFrameProductResourceTotalName"..i);
			if ui then
				ui:SetText("");
			end
		end
		
		local makeProgress	= GameClanManager:getProductProgressByIdx(0);
		local descRich 		= getglobal("ClanFrameProductDescRich");
		local iconTex 		= getglobal("ClanFrameProductMakingDescBkgIconButtonTex");
		local timeFont 		= getglobal("ClanFrameProductLeftTimeBkgValue");
		local progressTex 	= getglobal("ClanFrameProductProgressBkgTex");
		local progressBtn 	= getglobal("ClanFrameProductProgressBkg");
		if makeProgress ~= nil then
			productDef = GameClanManager:getProductDefByID( makeProgress.ID );
			descRich:SetText( "\t\t"..productDef.DescText,255,254,189 );
			if productDef.Category == 0 then
				local itemDef = getItemDef( productDef.ID );
				if itemDef ~= nil then
					local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					iconTex:SetTexture( IconPath );
				end
			elseif productDef.Category == 1 then
				local IconID = getItemDef( productDef.ID ).IconID;
				local IconPath = GetItemEquipIconPath()..IconID..".tga";
				iconTex:SetTexture( IconPath );
			end
			local nServerTime = GameClanManager:getServerTime();
			local clanProductDef = GameClanManager:getProductDefByID( makeProgress.ID )
			local nLeftTime = nServerTime - makeProgress.EndTime;
			local nDelta = clanProductDef.TimeCost ;
			local nLeft = makeProgress.EndTime - nServerTime;
			local Scale =1-nLeft/nDelta;
			if Scale < 0 then
				Scale = 0;
			end
			if Scale > 1 then
				Scale = 1;
			end
			local szText = "";
			if nLeft > 0 then
				if nLeft >= 3600 then
					szText = szText .. math.floor( nLeft / 3600 ) .. "ʱ";
				end
				if nLeft >= 60 then
					szText = szText .. math.floor( math.fmod( nLeft, 3600 ) / 60 ) .. "��";
				end
				szText = szText .. math.fmod( nLeft, 60 ) .. "��";
			end
			timeFont:SetText( szText );
			progressBtn:SetClientUserData( 0,makeProgress.EndTime );
			progressBtn:SetClientUserData( 1,makeProgress.BeginTime );
			progressTex:SetSize(340*Scale,13);
			ClanFrameProductMakingBkgLeftMakeNum:SetText( makeProgress.Value )
		end
		--ClanFrameProductMakeNumEdit:SetText( 1 );
	end
end


function ClanFrameProductProgressBkg_OnEnter()
	local nEndTime = this:GetClientUserData( 0 );
	local nLeftTime = nEndTime - GameClanManager:getServerTime();
	if nLeftTime > 0 then		
		local szText = "ʣ��ʱ�䣺";
		if nLeftTime >= 3600 then
			szText = szText .. math.floor( nLeftTime / 3600 ) .. "ʱ";
		end
		if nLeftTime >= 60 then
			szText = szText .. math.floor( math.fmod( nLeftTime, 3600 ) / 60 ) .. "��";
		end
		szText = szText .. math.fmod( nLeftTime, 60 ) .. "��";		
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName(), horizonal = "right" } );
	end
end

function ClanFrameProductProgressBkg_OnUpdate()
	local nEndTime = this:GetClientUserData( 0 );
	local nStartTime = this:GetClientUserData( 1 );
	if nEndTime > 0 then
		local nPass =  - this:GetClientUserData( 2 );
		local scale = (GameClanManager:getServerTime() - nStartTime)/(nEndTime - nStartTime);
		if scale > 1 then
			scale = 1;
		end
		local progressTex = getglobal(this:GetName().."Tex");
		progressTex:SetSize(340*scale,13);
		local nLeftTime = nEndTime - GameClanManager:getServerTime();
		if nLeftTime > 0 then		
			local szText = "";
			if nLeftTime >= 3600 then
				szText = szText .. math.floor( nLeftTime / 3600 ) .. "ʱ";
			end
			if nLeftTime >= 60 then
				szText = szText .. math.floor( math.fmod( nLeftTime, 3600 ) / 60 ) .. "��";
			end
			szText = szText .. math.fmod( nLeftTime, 60 ) .. "��";
			ClanFrameProductLeftTimeBkgValue:SetText( szText );
		end
	end
end

function ClanFrameProduct_ScrollBar_OnValueChanged()
	UpdateClanFrameProductCategoryList()
end

function ClanFrameProduct_ScrollUpBtn_OnClick()
	local slider = ClanFrameProductScrollBar
	local value = slider:GetValue()
	if value > slider:GetMinValue() then
		slider:SetValue(value - 1)
	end
end

function ClanFrameProduct_ScrollDownBtn_OnClick()
	local slider = ClanFrameProductScrollBar
	local value = slider:GetValue()
	if value < slider:GetMaxValue() then
		slider:SetValue(value + 1)
	end
end

function ClanFrameProduct_CategoryBtn_OnClick()
	SelectClanFrameProductLabel(this:GetClientID())
end

function ClanFrameProduct_OpenListBtn_OnClick()
	local parent = getglobal(this:GetParent())
	SelectCategoryBtnBtn(parent:GetClientID())
end

function SelectCategoryBtnBtn(i)
	local start = ClanFrameProductScrollBar:GetValue()
	local index = i + start
	local label = LabelList[index]
	LabelTypeOpen[label] = not LabelTypeOpen[label]
	SortClanFrameProductLabelList()
end

function SelectClanFrameProductLabel(i)
	local start = ClanFrameProductScrollBar:GetValue()
	local index = i + start
	local label = LabelList[index]
	if type(label) == type("") then
	else
		if nCurSelType then
			for i = 1, MAX_CATAGORY_NUM do
				local index = i + start
				local label = LabelList[index]
				if type(label) ~= type("") and label == nCurSelType then
					local btn = getglobal("ClanFrameProductCategoryBtn"..i.."HighlightBtn")
					btn:DisChecked()
					break
				end
			end
		end
		nCurSelType = label
		local btn = getglobal("ClanFrameProductCategoryBtn"..i.."HighlightBtn")
		btn:Checked()
		ClanFrameProductMakeNumEdit:SetText(1)
		UpdateClanFrameProductSelectData()
	end
end

function UpdateClanFrameProductCategoryList()

	local clanAdv = GameClanManager:getAdvanceInfo()
	local clanViewer = GameClanManager:getClanViewer()

	local start = ClanFrameProductScrollBar:GetValue()
	local n = #LabelList
	for i = 1, MAX_CATAGORY_NUM do
		if i + start > n then
			break
		end
		
		local btn = getglobal("ClanFrameProductCategoryBtn"..i.."HighlightBtn")
		local font = getglobal("ClanFrameProductCategoryBtn"..i.."HighlightBtnType")
		local openBtn = getglobal("ClanFrameProductCategoryBtn"..i.."HighlightBtnOpenList")
		local label = LabelList[i + start]
		
		btn:Show()

		if type(label) == type("") then
			btn:SetNormalTextureTemplate("TransTextureTemplate")
			btn:SetHighlightTextureTemplate("TransTextureTemplate")
			btn:SetPushedTextureTemplate("TransTextureTemplate")
			btn:SetDisableTextureTemplate("TransTextureTemplate")
			local itemName = string.format("    %s", label)
			font:SetText(itemName)
			font:SetTextColor(255, 255, 190)
			if LabelTypeOpen[label] then
				openBtn:SetNormalTextureTemplate("$parentCloseListNormalTexTemplate")
				openBtn:SetHighlightTextureTemplate("$parentCloseListHighlightTexTemplate")
				openBtn:SetPushedTextureTemplate("$parentCloseListPushedTexTemplate")		
			else
				openBtn:SetNormalTextureTemplate("$parentOpenListNormalTexTemplate")
				openBtn:SetHighlightTextureTemplate("$parentOpenListHighlightTexTemplate")
				openBtn:SetPushedTextureTemplate("$parentOpenListPushedTexTemplate")
			end
			openBtn:Show();
		else
			openBtn:Hide();
			btn:SetNormalTextureTemplate("TransTextureTemplate")
			btn:SetHighlightTextureTemplate("HighlightTextureTemplate")
			btn:SetPushedTextureTemplate("PushedTextureTemplate")
			btn:SetDisableTextureTemplate("TransTextureTemplate")
			font:SetText(GameClanManager:getProductDefByIndex(label).Name)
			local productDef = GameClanManager:getProductDefByIndex(label)

			if clanAdv.Tech.TechLevel[productDef.TechRequired[0].TechCategory] >= productDef.TechRequired[0].Level and
			   clanViewer.m_clanInfo.m_nMoney >= productDef.NeedMoneyCost and
			   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[0].ID) >= productDef.NeedMaterialCost[0].Num and
			   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[1].ID) >= productDef.NeedMaterialCost[1].Num and
			   GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[2].ID) >= productDef.NeedMaterialCost[2].Num then
			
			   font:SetTextColor(160, 160, 90)
			elseif IsClanSkillLearned( productDef.TechRequired[0].TechCategory, productDef.TechRequired[0].Level ) then
				font:SetTextColor(160, 160, 90); --������ɫ Elmer
			else
			   font:SetTextColor(255, 0, 0)
			end
			
			if nCurSelType == label then
				btn:Checked()
			else
				btn:DisChecked()
			end
		end
	end
	for i = n - start + 1, MAX_CATAGORY_NUM do
		local btn = getglobal("ClanFrameProductCategoryBtn"..i.."HighlightBtn")
		btn:Hide()
	end
end

function SortClanFrameProductLabelList()
	LabelList = {}
	for _, type in ipairs(LabelTypes) do
		table.insert(LabelList, type)
		if LabelTypeOpen[type] then
			for _, index in ipairs(LabelNames[type]) do
				table.insert(LabelList, index)
			end
		end
	end
	local slider = ClanFrameProductScrollBar
	local n = #LabelList
	local diff = n - MAX_CATAGORY_NUM
	if diff > 0 then
		if not slider:IsShown() then
			slider:Show()
		end
		slider:SetMaxValue(diff)
		if slider:GetValue() > diff then
			slider:SetValue(diff)
		end
	else
		slider:Hide()
		slider:SetValue(0)
	end
	UpdateClanFrameProductCategoryList()
end

--��ʼ����
function ClanFrameProductStartBtn_OnClick()
	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local nNum = tonumber( ClanFrameProductMakeNumEdit:GetText() );
	if nNum == nil or nNum < 1 then
		ShowMidTips("��������������");
		return;
	end
	if productDef ~= nil then		
		GameClanManager:clan_Opt_Make(productDef.ID, nNum);
	end
end

--ֹͣ����
function ClanFrameProductStopBtn_OnClick()
	local makeProgress = GameClanManager:getProductProgressByIdx(0);
	if makeProgress == nil then
		ShowMidTips("û�������������Ʒ");
		return;
	end
	GameClanManager:clan_Opt_CancleMake(makeProgress.ID);
end

--��������
function ClanFrameProductSpeedBtn_OnClick()
	local makeProgress = GameClanManager:getProductProgressByIdx(0);
	if makeProgress == nil then
		ShowMidTips("û�������������Ʒ");
		return;
	end

	local ADD_TYPE_PRODUCE = 2
	ClanAddSpeedFrame:SetClientUserData( 3,makeProgress.ID );
	ClanAddSpeedFrame:SetClientUserData( 0,ADD_TYPE_PRODUCE );
	ClanAddSpeedFrame:Show();
end

function ClanFrameProductOnShow()
	nCurSelType = nil
	LabelTypeOpen = {}
	ClanFrameProductMakeNumEdit:SetText(1)
	SetClanFrameProductLabelList()
	ClanFrameProductScrollBar:SetValue(0)
end

function ProduceListSelectDownBtn_Click()
	local listBox = getglobal("ClanFrameProductSelectListBox")
	if listBox:IsShown() then
		listBox:Hide()
	else
		listBox:Show()
	end
end

function ProduceSelectListBox_OnShow()
	ClanFrameProductSelectListBoxSelectBtn1LayerName:SetText("ȫ��")
	ClanFrameProductSelectListBoxSelectBtn2LayerName:SetText("������")
	ClanFrameProductSelectListBoxSelectBtn3LayerName:SetText("��������")
end

function ProduceSelectListBoxBtn_OnClick()
	local id = this:GetClientID()
	SelectListBox(id)

	local listBox = getglobal("ClanFrameProductSelectListBox")
	listBox:Hide()
end

function SelectListBox(id)
	
	if id == 1 then
		ClanFrameProductProduceListSelectProduceListSelectString:SetText("ȫ��")

	elseif id == 2 then
		ClanFrameProductProduceListSelectProduceListSelectString:SetText("������")

	elseif id == 3 then
		ClanFrameProductProduceListSelectProduceListSelectString:SetText("��������")

	end
	
	t_LabelNameList = {};
	nCurSelType = 0;
	LabelNames = {}
	LabelTypes = {}
	LabelTypeOpen = {}
	LabelList = {}

	SetClanFrameProductLabelList()

	local start = ClanFrameProductScrollBar:GetValue()
	local n = #LabelList
	local firstBtn = false

	for i = 1, MAX_CATAGORY_NUM do
		if i + start > n then
			break
		end

		local label = LabelList[i + start]
		if type(label) == type("") then
			if firstBtn == false then
				firstBtn = true
				LabelTypeOpen[label] = true
			else
				LabelTypeOpen[label] = false
			end
		end
	end

	SortClanFrameProductLabelList()
	UpdateClanFrameProductCategoryList()

	if #LabelList > 0 then
		SelectClanFrameProductLabel(2)
	end
end

function ItemBtn_OnEnter()
	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local itemDef = getItemDef( productDef.ID );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
end

function ItemBtn_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function MakingItemBtn_OnEnter()
	local szPath = ClanFrameProductMakingDescBkgIconButtonTex:GetTexture()
	if szPath ~= "uires\\ui\\mask\\TouMingTongDao.tga" then
		local makeProgress = GameClanManager:getProductProgressByIdx(0);
		if makeProgress then
			local itemDef = getItemDef(makeProgress.ID);
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef);
		end
	end
end

function ClanFrameProductNumEditOnFocus()
	timer(function()
		ClanFrameProductMakeNumEdit:setSelBegin(0)
		ClanFrameProductMakeNumEdit:setCursorPos(#ClanFrameProductMakeNumEdit:GetText())
	end)
end

function NumEditUpdate()
	ChangeNeed()
end

function ChangeNeed()

	local nMakingNum = tonumber( ClanFrameProductMakeNumEdit:GetText() );
	local productDef = GameClanManager:getProductDefByIndex( nCurSelType );
	local clanViewer = GameClanManager:getClanViewer();
	local timeNeed		= getglobal("ClanFrameProductTimeNeedBkgValue");
	local skillPoint	= getglobal("ClanFrameProductSkillPointGainBkgValue");
	
		for i=1, MAX_CLAN_MAKE_MATERIAL_TOLUA do
			if productDef.NeedMaterialCost[i-1].ID > 0 then
				local itemDef = getItemDef( productDef.NeedMaterialCost[i-1].ID );
				if itemDef ~= nil then
					local Need = getglobal("ClanFrameProductResourceNeedName"..i);
					local must = productDef.NeedMaterialCost[i-1].Num
					local max = GameClanManager:getStorageResourceNum(productDef.NeedMaterialCost[i-1].ID);
					if Need then
						if nMakingNum == nil then
							nMakingNum = 0
						end

						Need:SetText(string.format("%s %d", itemDef.Name, must * nMakingNum));
						if must * nMakingNum <= max then
							Need:SetTextColor(255, 255, 190);
						else
							Need:SetTextColor(255, 0, 0);
						end
					end					
				end
			end
		end

	local timeNum = math.floor(productDef.TimeCost / 60);
	timeNeed:SetText( timeNum * nMakingNum.."����" );
	skillPoint:SetText( productDef.ProductTech * nMakingNum);
	
	ClanFrameProductMoneyNeedBkgValue:SetText( productDef.NeedMoneyCost * nMakingNum );

	if productDef.NeedMoneyCost * nMakingNum <= clanViewer.m_clanInfo.m_nMoney then
		ClanFrameProductMoneyNeedBkgValue:SetTextColor(255, 255, 190);
	else
		ClanFrameProductMoneyNeedBkgValue:SetTextColor(255, 0, 0);
	end
end

--           ������ʾ           --

local bHaveFindLearnableTech = false

-- ��ʾ����
function ShowClanHelpBtn(which, text)
	if which then
		local animationTexture = getglobal(which:GetName().."UVAnimationTex")
		animationTexture:SetUVAnimation(80, true)
		animationTexture:Show()
	end
	local btn = getglobal(which:GetName() .. "Help")
	btn:Show()
	local rich = getglobal(btn:GetName() .. "Rich")
	rich:SetClientString(text or "")
	ReSizeTooltip(btn:GetName() .. "Rich", btn:GetName(), 14)
end

-- ���ذ���
function HideClanHelpBtn(which)
	if which then
		local animationTexture = getglobal(which:GetName().."UVAnimationTex")
		animationTexture:Hide()
	end
	local btn = getglobal(which:GetName() .. "Help")
	btn:Hide()
end

-- ���ҿ�ѧϰ�ĿƼ�������ʾ����
function FindLearnableTechAndShowHelp()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_HAVE_CLAN_LEARN_TECH) then
		if not bHaveFindLearnableTech then
			for i = 1, 4 do
				local btn = getglobal("ClanSkillInfo" .. i)
				local id, level
				id    = btn:GetClientUserData(0)
				level = btn:GetClientUserData(1)

				if CanLearnClanSkill(id, level) then
					btn = getglobal("ClanSkillInfo" .. i .. "LearnBtn")
					local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
					animationTexture:SetUVAnimation(80, true)
					animationTexture:Show()
					btn = getglobal(btn:GetName() .. "Help")
					btn:Show()
					local rich = getglobal(btn:GetName() .. "Rich")
					rich:SetClientString("���ѧϰ�Ƽ�")
					ReSizeTooltip(btn:GetName() .. "Rich", btn:GetName(), 14)
					bHaveFindLearnableTech = true
					break
				end
			end
		else
			for i = 1, 4 do
				btn = getglobal("ClanSkillInfo" .. i .. "LearnBtn")
				local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
				animationTexture:Hide()
				btn = getglobal("ClanSkillInfo" .. i .. "LearnBtnHelp")
				btn:Hide()
			end

			bHaveFindLearnableTech = false
			FindLearnableTechAndShowHelp()
		end
	else
		for i = 1, 4 do
			btn = getglobal("ClanSkillInfo" .. i .. "LearnBtn")
			local animationTexture = getglobal(btn:GetName().."UVAnimationTex")
			animationTexture:Hide()
			btn = getglobal("ClanSkillInfo" .. i .. "LearnBtnHelp")
			btn:Hide()
		end
	end
end

--------------------------------------------������¼--------------------------------------------------

local MAX_INFO_NUM = 15;
local nSortType = 1;
local t_DonateLog = {};
local nTotalPage = 1;
local nCurPage = 0;
local bTurnFlag = true;

function ClanDonateLogFrame_OnLoad()
	for i=1, MAX_INFO_NUM do
		btn = getglobal( "ClanDonateLogFrameBackDonateInfo" .. i );
		if i%2 == 0 then
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
		end
	end
	this:RegisterEvent("GE_CLAN_DONATE_LOG_UPDATE");
	this:RegisterEvent("GE_CLAN_DISABLEFRAME");
end

function ClanDonateLogFrame_OnEvent()
	if arg1 == "GE_CLAN_DONATE_LOG_UPDATE" then
		if not ClanDonateLogFrame:IsShown() then
			return;
		end
		GameClanManager:SortDonateLog( nSortType, bTurnFlag );		
		t_DonateLog = {};
		local num = GameClanManager:GetClanDonateLogNum();
		SetTotalPage(num);
		for i=1,num do
			local info = GameClanManager:GetClanDonateLog( i-1 );
			table.insert( t_DonateLog, info );
		end
		UpdatePageInfo();
	elseif arg1 == "GE_CLAN_DISABLEFRAME" then
		this:Hide();
	end
end

function SetTotalPage( num )	
	nTotalPage = math.ceil( num / MAX_INFO_NUM );
	nTotalPage = ( nTotalPage == 0 ) and 1 or nTotalPage;
end

function UpdatePageInfo()
	local nCount = 1;
	local btn;
	for i=(nCurPage*MAX_INFO_NUM + 1),(nCurPage*MAX_INFO_NUM + MAX_INFO_NUM) do
		if i > #t_DonateLog or nCount > MAX_INFO_NUM then
			break;
		end
		local name = "ClanDonateLogFrameBackDonateInfo" .. nCount;
		local info = t_DonateLog[i];		
		local fonts = {MemberNameFont = info.RoleMember.m_szRoleName, DonateMoneyFont = info.DonateNum, 
					ClanMoneyFont = info.DonateClanMoney, MemberContriFont = info.DonateContri}
		for suffix, value in pairs(fonts) do
			btn = getglobal(name..suffix);
			btn:SetText(value);
		end
		nCount = nCount + 1;
	end
	for i=nCount,MAX_INFO_NUM do
		local name = "ClanDonateLogFrameBackDonateInfo"..i;
		local fonts = {"MemberNameFont", "DonateMoneyFont", "ClanMoneyFont", "MemberContriFont"};
		for _, suffix in ipairs(fonts) do
			btn = getglobal(name..suffix);
			btn:SetText("");
		end
	end
	ClanDonateLogPageFont:SetText( "��" .. nCurPage + 1 .. "/" .. nTotalPage .. "ҳ" );
end

function ClanDonateLogFrame_OnShow()
	local btn = getglobal( "ClanDonateLogMemberNameBtn" );
	btn:Disable();
	util.ShowInCenter(this);
	UpdatePageInfo();
end

function OpenClanDonateLogFrame( npcId )
	nSortType = 1;
	nCurPage = 0;
	bTurnFlag = true;
	GameClanManager:SortDonateLog( nSortType, bTurnFlag );
	local num = GameClanManager:GetClanDonateLogNum();
	SetTotalPage(num);
	t_DonateLog = {};
	for i=1,num do
		local info = GameClanManager:GetClanDonateLog( i-1 );
		table.insert( t_DonateLog, info );
	end
	if not ClanDonateLogFrame:IsShown() then
		ClanDonateLogFrame:Show();
		util.testNpcDialogDistance(npcId, ClanDonateLogFrame);
	else
		ClanDonateLogFrame:Show();
	end
end

function DonateListLeftPageBtn_OnClick()
	if nCurPage < 1 then
		return;
	end
	nCurPage = nCurPage - 1;
	UpdatePageInfo();
end

function DonateListRightPageBtn_OnClick()
	if nCurPage + 1 >= nTotalPage then
		return;
	end
	nCurPage = nCurPage + 1;
	UpdatePageInfo();
end

function DonateListToLastPageBtn_OnClick()
	nCurPage = nTotalPage - 1;
	UpdatePageInfo();
end

function DonateListToFirstPageBtn_OnClick()
	nCurPage = 0;
	UpdatePageInfo();
end

function ClanDonateLogDonateMoneyBtn_OnClick()
	if nSortType ~= 2 then
		nSortType = 2;
		nCurPage = 0;
		bTurnFlag = true;
	else		
		bTurnFlag = not bTurnFlag;
	end
	GameClanManager:SortDonateLog( nSortType, bTurnFlag );
	UpdatePageInfo();
end

function ClanDonateLogClanMoneyBtn_OnClick()
	if nSortType ~= 3 then
		nSortType = 3;
		nCurPage = 0;
		bTurnFlag = true;
	else		
		bTurnFlag = not bTurnFlag;
	end
	GameClanManager:SortDonateLog( nSortType, bTurnFlag );	
	UpdatePageInfo();
end

function ClanDonateLogMemberContriBtn_OnClick()
	if nSortType ~= 4 then
		nSortType = 4;
		nCurPage = 0;
		bTurnFlag = true;		
	else		
		bTurnFlag = not bTurnFlag;
	end
	GameClanManager:SortDonateLog( nSortType, bTurnFlag );	
	UpdatePageInfo();
end

-----------------------------------���Ἴ��--------------------------------------

local MAX_CLAN_SKILL_BTN = 12;
local MAX_MY_SKILL_BTN = 14;
local MAX_CLAN_SKILL_ROW = 4;
local MAX_MY_SKILL_ROW = 7;
local t_AllSkillList = {};
local t_ClanSkillList = {};
local t_MySkillList = {};
local nCurPage = 1;
local nTotalPage = 1;

function ClanSkillResearch( nId, nLv )
	local mainplayer = ActorMgr:getMainPlayer();
	local position = mainplayer:getClanPosition();

	if position == CLAN_POS_PRESIDENT then		
		local ClanViewer		= GameClanManager:getClanViewer();
		local ClanInfo			= ClanViewer.m_clanInfo;
		local bResearch = isInRearchList(nId);
		local skillDef;
		if bResearch then
			skillDef = GameClanManager:getClanSkillDef( nId, nLv + 1 );
		else
			skillDef = GameClanManager:getClanSkillDef( nId, 1 );
		end

		if skillDef.SkillID == 0 then
			ShowMidTips("���ܵȼ��Ѵ����ޣ������о��ü���");
			return;
		end

		if skillDef.ResearchNeedClanLevel > ClanInfo.m_nClanLevel then
			ShowMidTips("����ȼ����㣬�����о��ü���");
			return;
		end

		if skillDef.ResearchNeedClanMoney > ClanInfo.m_nMoney then
			ShowMidTips("�����ʽ��㣬�����о��ü���");
			return;
		end

		MessageBox("��ʾ","�з�<#c0ad700" .. skillDef.Name .. "Lv" .. skillDef.Level .. "#n>��Ҫ����#c0ad700" .. skillDef.ResearchNeedClanMoney .. "#n�����ʽ��Ƿ��з�" );
		MessageBoxFrame:SetClientString("���Ἴ���з�");
		MessageBoxFrame:SetClientUserData( 0, skillDef.SkillID );
		MessageBoxFrame:SetClientUserData( 1, skillDef.Level );
	else
		ShowMidTips("ֻ�л᳤���ܽ��м����з�");
	end
end

function ClanSkillLearn( nId, nLv, szType )
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local skillDef;
	if szType == "ѧϰ" then
		skillDef = GameClanManager:getClanSkillDef( nId, 1 );
		if skillDef.AutoLearn == 2 then
			ShowMidTips("����" .. szType  .. "�ü���");
			return
		end
	elseif szType == "����" then
		skillDef = GameClanManager:getClanSkillDef( nId, nLv + 1 );
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local clanMember = GameClanManager:getMemberInfo( mainplayer:getName());

	if clanMember ~= nil then
		if skillDef.SkillID == 0 then
			ShowMidTips("���ܵȼ��Ѵ����ޣ����������ü���");
			return;
		end

		if clanMember.Contribute >= skillDef.LearnNeedContri then
			MessageBox("��ʾ", szType .. "<#c0ad700" .. skillDef.Name .. "Lv" .. skillDef.Level .. "#n>��Ҫ����#c0ad700" .. skillDef.LearnNeedContri .. "#n���׶ȣ��Ƿ�" .. szType );
			MessageBoxFrame:SetClientString("���Ἴ��ѧϰ");
			MessageBoxFrame:SetClientUserData( 0, skillDef.SkillID );
			MessageBoxFrame:SetClientUserData( 1, skillDef.Level );
		else
			ShowMidTips("���ṱ�׶Ȳ��㣬����" .. szType  .. "�ü���");
		end
	else
		ShowMidTips("����" .. szType  .. "�ü���");
	end
end

function ResetSkillTab()
	t_AllSkillList = {};
	t_ClanSkillList = {};
	t_MySkillList = {};	
	ClanSkillBtn_ScrollBar:SetValue(0);
	nCurPage = 0;
	nTotalPage = 1;
end

function ClanFrameSkill_OnTab()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬���ܽ��й�����صĲ���" )
		return
	end
	if ClanFrameSkillTab:IsChecked() then
		return
	end
	ClearTabs()
	ClanFrameSkillTab:Checked()
	ResetSkillTab();

	GetAllClanSkill();
	local clanSkill = GameClanManager:getClanSkillInfo();
	for i=1,clanSkill.Num do
		local info = clanSkill.ClanSkill[i-1];
		table.insert( t_ClanSkillList, { id = info.SkillID, lv = info.Level, valiLv = info.ValidLevel } );
	end
	local mySkill = GameClanManager:getPlayerClanSkillInfo();
	for i=1,mySkill.Num do
		local info = mySkill.ClanSkill[i-1];
		table.insert( t_MySkillList, { id = info.SkillID, lv = info.Level, idx = info.ClanSkillIdx } );
	end
	ClanFrameSkillButton:Show();
end
function ClanFrameGroup_OnTab()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then
		ShowMidTips( "���״̬���ܽ��й�����صĲ���" )
		return
	end
	if ClanFrameGroupTab:IsChecked() then
		return
	end
	ClearTabs()
	ClanFrameGroupTab:Checked()
	ClanFrameGroup:Show();
end

function GetAllClanSkill()	
	local num = GameClanManager:getAllCLanSkillNum();
	for i=1,num do
		local info = GameClanManager:getClanSkillDefByIndex(i-1);
		if info.Level == 1 then
			table.insert( t_AllSkillList, { id = info.SkillID, lv = info.Level } );
		end
	end
end

function UpdateClanSkillInfo( nSliderValue )
	local num = math.ceil( #t_ClanSkillList / 3 );
	if num > MAX_CLAN_SKILL_ROW then
		ClanSkillBtn_ScrollBar:SetMaxValue( num - MAX_CLAN_SKILL_ROW );
		ClanSkillBtn_ScrollBar:Show();
		ClanSkillBtn_ScrollUpBtn:Show();
		ClanSkillBtn_ScrollDownBtn:Show();
	else
		ClanSkillBtn_ScrollBar:Hide();
		ClanSkillBtn_ScrollUpBtn:Hide();
		ClanSkillBtn_ScrollDownBtn:Hide();
	end

	local nCount = 1;
	local btn;
	nSliderValue = (nSliderValue > 1) and ((nSliderValue - 1) * 3 + 1) or 1;
	--�������������
	for i=nSliderValue, #t_AllSkillList do
		if nCount > MAX_CLAN_SKILL_BTN then
			break;
		end
		local info;
		local inRSkill,curLv,valiLv = isInRearchList( t_AllSkillList[i].id );
		if inRSkill then
			info = GameClanManager:getClanSkillDef( t_AllSkillList[i].id, ((valiLv == 0) and 1 or valiLv));
		else
			info = GameClanManager:getClanSkillDef( t_AllSkillList[i].id, t_AllSkillList[i].lv );
		end
		local name = "ClanSkillBtnInfo" .. nCount;
		btn = getglobal(name);
		btn:SetClientUserData( 0, info.SkillID );
		btn:SetClientUserData( 1, info.Level );
		btn:SetClientString( name .. "SkillBtn" );
		local tex = getglobal(name .. "SkillBtnIcon");
		local IconPath = GetSkillIconPath()..info.IconID1..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end		
		tex:SetTexture( IconPath );
		local colorTex = getglobal(name .. "SkillBtnUVAnimationTex");
		colorTex:Hide();
		--����ѧϰ���з�״̬
		btn = getglobal(name .. "ResearchBtn");
		if inRSkill then
			tex:SetGray( false );
			if curLv > valiLv then
				btn:Disable();
				if valiLv == 0 then
					tex:SetGray( true );
				end
				setLearnBtnText( t_AllSkillList[i].id, name .. "LearnBtn");
			else
				if checkCanResearch( t_AllSkillList[i].id, curLv + 1 ) then
					colorTex:SetUVAnimation(100, true);
					colorTex:Show();
					btn:Enable();
					setLearnBtnText( t_AllSkillList[i].id, name .. "LearnBtn");
				else
					btn:Disable();
					setLearnBtnText( t_AllSkillList[i].id, name .. "LearnBtn");
				end
			end
		else
			if checkCanResearch( t_AllSkillList[i].id, t_AllSkillList[i].lv ) then
				tex:SetGray( false );
				colorTex:SetUVAnimation(100, true);
				colorTex:Show();
				btn:Enable();
			else
				tex:SetGray( true );
				btn:Disable();
			end
			setLearnBtnText( 0, name .. "LearnBtn");
		end
		nCount = nCount + 1;
	end
	--����û�м��ܵ�btn
	for i=nCount, MAX_CLAN_SKILL_BTN do
		local name = "ClanSkillBtnInfo" .. i;
		btn = getglobal(name);
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientString( "" );
		local tex = getglobal(name .. "SkillBtnIcon");
		tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
		btn = getglobal(name .. "ResearchBtn");
		btn:Disable();
		btn = getglobal(name .. "LearnBtn");
		btn:SetText("ѧϰ");
		btn:Disable();
	end
end

function isInRearchList( id )
	for i=1, #t_ClanSkillList do
		if t_ClanSkillList[i].id == id then
			return true, t_ClanSkillList[i].lv, t_ClanSkillList[i].valiLv;
		end
	end
	return false, 0, 0;
end

function isInLearnList( id )
	for i=1, #t_MySkillList do
		if t_MySkillList[i].id == id then
			local valiLv = t_ClanSkillList[t_MySkillList[i].idx + 1].valiLv;
			if t_MySkillList[i].lv > valiLv then
				valiLv = (valiLv == 0) and 1 or valiLv;
				return true,valiLv;
			end
			return true,t_MySkillList[i].lv;
		end
	end
	return false;
end

function checkCanResearch( id, lv )	
	local skillDef = GameClanManager:getClanSkillDef( id, lv );
	if skillDef.SkillID ~= 0 then
		local ClanViewer		= GameClanManager:getClanViewer();
		local ClanInfo			= ClanViewer.m_clanInfo;

		if skillDef.ResearchNeedClanLevel > ClanInfo.m_nClanLevel then
			return false;
		end
		if skillDef.ResearchNeedClanMoney > ClanInfo.m_nMoney then
			return false;
		end
		return true;
	else
		return false;
	end
end

function checkCanLearn( id, lv )
	if id == 0 then
		return false;
	end
	local skillDef = GameClanManager:getClanSkillDef( id, lv );
	if skillDef.SkillID == 0 then
		return false;
	end
	
	--����ѧϰ�ļ���
	if skillDef.AutoLearn == 2 then
		return false
	end

	if lv == 1 then
		local ClanViewer		= GameClanManager:getClanViewer();
		local ClanInfo			= ClanViewer.m_clanInfo;

		if skillDef.ResearchNeedClanLevel > ClanInfo.m_nClanLevel then
			return false;
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local clanMember = GameClanManager:getMemberInfo( mainplayer:getName());

	if clanMember ~= nil then
		if skillDef.LearnNeedContri > clanMember.Contribute then
			return false;
		end
		return true;
	else
		return false;
	end
end

function setLearnBtnText( id, name )
	local status = false;
	local btn = getglobal(name);
	if id ~= 0 then
		for i=1, #t_MySkillList do
			if t_MySkillList[i].id == id then
				status = true;
				local lv = t_ClanSkillList[t_MySkillList[i].idx + 1].valiLv;
				if (t_MySkillList[i].lv + 1) > lv then
					btn:Disable();
				else
					if checkCanLearn( id, t_MySkillList[i].lv + 1 ) then
						btn:Enable();
					else
						btn:Disable();
					end
				end
				break;
			end
		end
	end
	if status then
		btn:SetText("����");
	else
		btn:SetText("ѧϰ");
		if checkCanLearn( id, 1 ) then
			btn:Enable();
		else
			btn:Disable();
		end
	end
end

function UpdateMySkillInfo()
	local nCount = 1;
	local btn;
	local nPage = nCurPage;
	nPage = (nPage > 0) and (nPage* MAX_MY_SKILL_BTN + 1) or 1;
	for i=nPage, #t_MySkillList do
		if nCount > MAX_MY_SKILL_BTN or i > #t_MySkillList then
			break;
		end		
		local valiLv = t_ClanSkillList[t_MySkillList[i].idx + 1].valiLv;
		local info;
		if t_MySkillList[i].lv > valiLv then
			info = GameClanManager:getClanSkillDef( t_MySkillList[i].id, ((valiLv == 0) and 1 or valiLv));
		else
			info = GameClanManager:getClanSkillDef( t_MySkillList[i].id, t_MySkillList[i].lv );
		end
		local name = "MyLearnedSkillBtnInfo" .. nCount;
		btn = getglobal(name);
		btn:SetClientUserData( 0, info.SkillID );
		btn:SetClientUserData( 1, info.Level );
		btn:SetClientString( name );
		local tex = getglobal(name .. "Icon");
		local IconPath = GetSkillIconPath()..info.IconID1..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		tex:SetTexture( IconPath );
		if valiLv == 0 then
			tex:SetGray(true);
		else
			tex:SetGray(false);
		end
		nCount = nCount + 1;
	end

	for i=nCount, MAX_MY_SKILL_BTN do
		local name = "MyLearnedSkillBtnInfo" .. i;
		btn = getglobal(name);
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientString( "" );
		local tex = getglobal(name .. "Icon");
		tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	end

	nTotalPage = math.ceil( #t_MySkillList / MAX_MY_SKILL_BTN );
end

function ClanFrameSkillBtn_OnLoad()
	this:RegisterEvent("GE_CLAN_SKILL_UPDATE");
	this:RegisterEvent("GE_PLAYER_CLAN_SKILL_UPDATE");
	this:RegisterEvent("GE_CLAN_CONTRIBUTE_CHANGE");
	this:RegisterEvent("GE_CLAN_MONEY_UPDATE");
	this:RegisterEvent("GE_CLAN_LEVEL_UPDATE");
end

function ClanFrameSkillBtn_OnEvent()
	if arg1 == "GE_CLAN_SKILL_UPDATE" then
		UpdateBuff(); --ˢ�¹��Ἴ��buff
		if not ClanFrame:IsShown() or not ClanFrameSkillTab:IsChecked() then
			return;
		end
		t_ClanSkillList = {};
		local clanSkill = GameClanManager:getClanSkillInfo();
		for i=1,clanSkill.Num do
			local info = clanSkill.ClanSkill[i-1];
			table.insert( t_ClanSkillList, { id = info.SkillID, lv = info.Level, valiLv = info.ValidLevel } );
		end
		UpdateClanSkillInfo( ClanSkillBtn_ScrollBar:GetValue() );
		UpdateMySkillInfo();
		-- �����Ա����
		ClanFrameBasicInfoClanMaxMemNum:SetText( GetPlayerClanMaxMemNum() )	
	elseif arg1 == "GE_PLAYER_CLAN_SKILL_UPDATE" then
		UpdateBuff(); --ˢ�¹��Ἴ��buff
		if not ClanFrame:IsShown() or not ClanFrameSkillTab:IsChecked() then
			return;
		end
		t_MySkillList = {};
		local mySkill = GameClanManager:getPlayerClanSkillInfo();
		for i=1,mySkill.Num do
			local info = mySkill.ClanSkill[i-1];
			table.insert( t_MySkillList, { id = info.SkillID, lv = info.Level, idx = info.ClanSkillIdx } );
		end
		UpdateClanSkillInfo( ClanSkillBtn_ScrollBar:GetValue() );
		UpdateMySkillInfo();
	elseif arg1 == "GE_CLAN_CONTRIBUTE_CHANGE" then
		if not ClanFrame:IsShown() or not ClanFrameSkillTab:IsChecked() then
			return;
		end
		UpdateClanSkillInfo( ClanSkillBtn_ScrollBar:GetValue() );
		UpdateMySkillInfo();
	elseif arg1 == "GE_CLAN_MONEY_UPDATE" then
		if not ClanFrame:IsShown() or not ClanFrameSkillTab:IsChecked() then
			return;
		end
		UpdateClanSkillInfo( ClanSkillBtn_ScrollBar:GetValue() );
		UpdateMySkillInfo();
	elseif arg1 == "GE_CLAN_LEVEL_UPDATE" then
		UpdateBuff(); --ˢ�¹��Ἴ��buff
		if not ClanFrame:IsShown() or not ClanFrameSkillTab:IsChecked() then
			return;
		end
		UpdateClanSkillInfo( ClanSkillBtn_ScrollBar:GetValue() );
		UpdateMySkillInfo();
	end
end

function ClanFrameSkillBtn_OnShow()

	local num = math.ceil( #t_ClanSkillList / 3 );
	if num > MAX_CLAN_SKILL_ROW then
		ClanSkillBtn_ScrollBar:SetMaxValue( num - MAX_CLAN_SKILL_ROW );
		ClanSkillBtn_ScrollBar:SetValue(0);
		ClanSkillBtn_ScrollBar:Show();
		ClanSkillBtn_ScrollUpBtn:Show();
		ClanSkillBtn_ScrollDownBtn:Show();
	else
		ClanSkillBtn_ScrollBar:Hide();
		ClanSkillBtn_ScrollUpBtn:Hide();
		ClanSkillBtn_ScrollDownBtn:Hide();
	end

	UpdateClanSkillInfo(1);
	UpdateMySkillInfo();
end

function ClanFrameSkillTabResearchBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end

	local btn = getglobal( this:GetParent() );
	ClanSkillResearch( btn:GetClientUserData( 0 ), btn:GetClientUserData( 1 ) );
end

function ClanFrameSkillTabLearnBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("������֤������ȫ��");
		return;
	end

	local btn = getglobal( this:GetParent() );
	local id = btn:GetClientUserData( 0 );
	local isLearn, lv = isInLearnList( id );
	if not isLearn then
		ClanSkillLearn( id, 1, this:GetText() );
	else
		ClanSkillLearn( id, lv, this:GetText() );
	end
end

function ClanSkillBtn_ScrollUpBtn_OnClick()
	local slider = getglobal("ClanSkillBtn_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function ClanSkillBtn_ScrollDownBtn_OnClick()
	local slider = getglobal("ClanSkillBtn_ScrollBar");
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function ClanSkillBtn_ScrollBar_OnValueChanged()
	local slider = getglobal("ClanSkillBtn_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateClanSkillInfo( nValue + 1 );
	end
end

function ClanSkillTabLastPageBtn_OnClick()
	if nCurPage < 1 then
		return;
	end
	nCurPage = nCurPage - 1;
	UpdateMySkillInfo();
end

function ClanSkillTabNextPageBtn_OnClick()
	if (nCurPage + 1) >= nTotalPage then
		return;
	end
	nCurPage = nCurPage + 1;
	UpdateMySkillInfo();
end

function ClanSkillBtn_OnEnter()
	local btn = getglobal( this:GetParent() );
	if btn:GetClientString() ~= "" then
		ClanSkillTipsFrame:SetClientUserData(0, btn:GetClientUserData(0));
		ClanSkillTipsFrame:SetClientUserData(1, btn:GetClientUserData(1));
		ClanSkillTipsFrame:SetClientString(btn:GetClientString());
		ClanSkillTipsFrame:Show();
	end
end

function ClanSkillBtn_OnLeave()
	ClanSkillTipsFrame:Hide();
end

function MySkillBtn_OnEnter()
	if this:GetClientString() ~= "" then
		ClanSkillTipsFrame:SetClientUserData(0, this:GetClientUserData(0));
		ClanSkillTipsFrame:SetClientUserData(1, this:GetClientUserData(1));
		ClanSkillTipsFrame:SetClientString(this:GetClientString());
		ClanSkillTipsFrame:Show();
	end
end

function MySkillBtn_OnLeave()
	ClanSkillTipsFrame:Hide();
end

----------------------------------------------------����ս�����-------------------------------------------
t_clanGroupName = {
	[0] = "",
	[1] = "߱ɱս��",
	[2] = "��ɱս��",
	[3] = "��ɱս��",
	[4] = "��ɱս��",
	[5] = "��ɱս��",
	[6] = "�ƽ�ս��",
}
local t_MapStrongName = {
	[0]		= "��",
	[2400]	= "���ջ�Ұ",
	[2202]	= "����",
	[2200]	= "����ѩԭ",
	[2500]	= "Խ������",
	[2300]	= "����ɳĮ",
	[3404]	= "��ɳ�ݵ�",	
}
local t_clanGroupLeaderPos ={
    CLAN_POS_GROUP_1,
    CLAN_POS_GROUP_2,
    CLAN_POS_GROUP_3,
    CLAN_POS_GROUP_4,
    CLAN_POS_GROUP_5,
    CLAN_POS_GROUP_6,
}
local t_ClanGroupDesc = {
	[1] = [[    
#cffffbe	1������ս�ų�Ա��ս�ų�Աֻ�ɽ����ս�Ŷ�Ӧ�ġ������ݵ㡱
#cffffbe	2���᳤�����᳤�������ⲿ��ս�ų�Ա�������Լ���
#cffffbe	3��ս���ų��ɲ����ս�ų�Ա�������ɰ�����ս�ų�Ա����Ϊ��ս�ų�Ա
#c00ff00	4���ݵ�ս��ս��ս�����ý����ܽ����κε��������ڿ�սǰ��ǰ�����ս��]],
}
function GetGroupMemberNum( groupIdx )
	local groupMemNum = 0
	local ClanViewer	= GameClanManager:getClanViewer();
	local ClanMemNum	= ClanViewer.m_clanInfo.m_nClanMemNum;
	for i = 1, ClanMemNum do
		local member = ClanViewer.m_Members[i-1];
		if member.GroupID == groupIdx then
			groupMemNum = groupMemNum + 1;
		end
	end
	return groupMemNum;
end
function IsGroupHasLeader( groupIdx )
	local ClanViewer	= GameClanManager:getClanViewer();
	local ClanMemNum	= ClanViewer.m_clanInfo.m_nClanMemNum;
	local leaderPos = CLAN_POS_PRESIDENT + groupIdx
	for i = 1, ClanMemNum do
		local member = ClanViewer.m_Members[i-1];
		if member.GroupID == groupIdx and member.Position == leaderPos then
			return true
		end
	end
	return false;
end
function GetGroupLeaderNameAndLevel( groupIdx )
	local ClanViewer	= GameClanManager:getClanViewer();
	local ClanMemNum	= ClanViewer.m_clanInfo.m_nClanMemNum;
	for i = 1, ClanMemNum do
		local member = ClanViewer.m_Members[i-1];
		if member.Position == t_clanGroupLeaderPos[groupIdx] then
			return member.RoleMember.m_szRoleName, member.Level;
		end
	end
	return "", "";
end

function UpdateClanFrameGroup()
	local clanGroupInfo = GameClanManager:getClanGroupInfo()
	for i = 1, MAX_CLAN_GROUP_NUM_TOLUA do
		local groupBtn = getglobal( "ClanFrameGroupGroup"..i )
		local groupStrongChoseName = getglobal( groupBtn:GetName().."StrongChoseName" )
		local map = clanGroupInfo.GroupInfo[i-1].StrongMap;		
		groupStrongChoseName:SetText( t_MapStrongName[map] )
		local groupName = getglobal( groupBtn:GetName().."GroupName" )
		groupName:SetText( t_clanGroupName[i] )
		local groupMemNum = getglobal( groupBtn:GetName().."NumFont" )
		groupMemNum:SetText( GetGroupMemberNum( i ).."/"..(CLAN_GROUP_MEMBER_MAX_NUM + 1) );
		local szLeaderName, level = GetGroupLeaderNameAndLevel( i )
		local groupLeader = getglobal( groupBtn:GetName().."LeaderName" )
		groupLeader:SetText( szLeaderName )
		local groupLevel = getglobal( groupBtn:GetName().."LevelFont" )
		groupLevel:SetText( level )
		local groupMemberInfo = getglobal( groupBtn:GetName().."MemberInfo" )
		groupMemberInfo:setCenterLine( true )
		groupMemberInfo:SetClientUserData( 0, groupBtn:GetClientID() )
		local ClanViewer	= GameClanManager:getClanViewer();
		local ClanLevel	= ClanViewer.m_clanInfo.m_nClanLevel;
		if ClanLevel >= i then
			groupMemberInfo:SetText( "#L#c00ff00�鿴\����#n", 255, 255, 190 )
		else
			groupMemberInfo:SetText( i.."������\n�Զ�����", 255, 80, 80 )
		end
		if i >= 6 then
			if clanGroupInfo.UseItemGroup == 0 then
				groupMemberInfo:SetText( "#Lս��ƾ֤#n\n#L�������#n", 255, 255, 190 )
			else
				groupMemberInfo:SetText( "#L#c00ff00�鿴\����#n", 255, 255, 190 )
			end
		end
	end
end

function ClanFrameGroup_OnShow()
	UpdateClanFrameGroup();
	ClanFrameGroupDesc:SetText( t_ClanGroupDesc[1], 255,255,190 )
end
function ClanFrameGroup_OnUpdate()
	UpdateClanFrameGroup();
end
function ClanFrameGroup_OnHide()
	ClanFrameGroupChoseStrong:Hide()
end
function ClanFrameGroupMemberInfo_OnClick()
	local clanGroupInfo = GameClanManager:getClanGroupInfo()
	local ClanViewer	= GameClanManager:getClanViewer();
	local clanLevel	= ClanViewer.m_clanInfo.m_nClanLevel;
	if this:GetClientUserData(0) == 6 and clanGroupInfo.UseItemGroup == 0 then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer == nil then return end
		local container = mainplayer:getContainer()
		if container == nil then return end
		if container:getItemCount( CLAN_OPEN_GROUP_ITEM_ID_TOLUA ) <= 0 then
			ShowMidTips("��û��ս��ƾ֤���޷�����ƽ�ս��")
			return
		else
			GameClanManager:Clan_Opt_OpenGroup( 6 )
			return
		end
	end
	if clanLevel < this:GetClientUserData(0) and this:GetClientUserData(0) ~= 6 then
		ShowMidTips("���Ĺ��ỹδӵ�и�ս��")
		return
	end
	ClanGroupMemberInfoFrameTitleFont:SetText( t_clanGroupName[this:GetClientUserData(0)] )
	ClanGroupMemberInfoFrame:Show()
	ClanGroupMemberInfoFrame:SetClientUserData( 0, this:GetClientUserData(0) )
end
function ChoseStrongBtnTemplateChose_OnClick()
	local parent = getglobal( this:GetParent() )
	local parentEditBox = getglobal( this:GetParent().."EditBox" )
	local groupStrong = getglobal( parent:GetParent() )
	local groupBtn = getglobal( groupStrong:GetParent() )
	local strongs = getglobal( "ClanFrameGroupChoseStrong" )
	if strongs:IsShown() then
		strongs:Hide()
		strongs:SetClientUserData( 0, 0 )		
	else
		strongs:Show()
		strongs:SetPoint( "topleft", "UIClient", "topleft", parentEditBox:GetRealLeft(), parentEditBox:GetRealBottom() )
		strongs:SetClientUserData( 0, groupBtn:GetClientID() )		
	end
end
local t_SelcetStrongName = {
	[0]={ name = "��", mapId = 0,},
	[1]={ name = "����", mapId = 2202,},
	[2]={ name = "����ѩԭ", mapId = 2200,},
	[3]={ name = "����ɳĮ", mapId = 2300,},
	[4]={ name = "���ջ�Ұ", mapId = 2400,},
	[5]={ name = "Խ������", mapId = 2500,},
	[6]={ name = "��ɳ�ݵ�", mapId = 3404,},
}

function IsMapHasGroup( mapId, groupIdx )	
	local clanGroupInfo = GameClanManager:getClanGroupInfo()
	for i = 1, clanGroupInfo.GroupNum do
		local oneGroupMapInfo = clanGroupInfo.GroupInfo[i-1]
		if oneGroupMapInfo.StrongMap == mapId and i ~= groupIdx and mapId ~= 0 then
			return true
		end
	end
	return false
end
function ChoseStrongBtnTemplateSelectStrongBtn_OnClick()
	ClanFrameGroupChoseStrong:Hide()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getClanPosition() ~= CLAN_POS_PRESIDENT and mainplayer:getClanPosition() ~= CLAN_POS_VICE_PRESIDENT then
		ShowMidTips("��û��Ȩ�޸��ľݵ�")
		return		
	end
	local groupIdx = ClanFrameGroupChoseStrong:GetClientUserData(0)
	if IsMapHasGroup( t_SelcetStrongName[this:GetClientID()].mapId, ClanFrameGroupChoseStrong:GetClientUserData(0) ) then
		ShowMidTips("�����ظ�ѡ��ͬһ���ݵ�")
		return
	end
	GameClanManager:Clan_Opt_SetGroupStrongMap( groupIdx, t_SelcetStrongName[this:GetClientID()].mapId )
end
function ClanFrameGroupChoseStrong_OnUpdate()
	if not ClanFrameGroup:IsShown() or not ClanFrame:IsShown() then
		this:Hide()
	end
end
----������ų�Ա��Ϣ
local CLAN_GROUP_SHOW_MEMBER_MAX_NUM = 15

local curSortOrder = true
local curSortElement = "default"
local t_SortElements = {
	[1] = "default",
	[2] = "roleName",
	[3] = "level",
	[4] = "career",
	[5] = "groupID",
	[6] = "nickName",
	[7] = "time",
}

local t_ServerMemberList = {}
local t_GroupShowMemberList = {}
function SortClanGroupMemberList( memberList, sortOrder, sortElement )
	if sortElement == "default" then return end
	if sortOrder then
		table.sort( memberList, function( a, b )
			if sortElement == "time" then
				if a["online"]==0 and b["online"]==0 then
					return a["lastOnline"] > b["lastOnline"]
				else
					return a["online"] > b["online"]
				end
			else
				return a[sortElement] > b[sortElement]
			end
		end)
	else
		table.sort( memberList, function( a, b )
			if sortElement == "time" then
				if a["online"]==0 and b["online"]==0 then
					return a["lastOnline"] < b["lastOnline"]
				else
					return a["online"] < b["online"]
				end
			else
				return a[sortElement] < b[sortElement]
			end
		end )
	end
end
function SetGroupShowMemberList()
	t_ServerMemberList = {}
	if ClanGroupMemberInfoFrameOnlyShowSelfGroupCheckBtn:GetCheckState() then
		local ClanViewer	= GameClanManager:getClanViewer();
		local ClanMemNum	= ClanViewer.m_clanInfo.m_nClanMemNum;
		local groupIdx = ClanGroupMemberInfoFrame:GetClientUserData(0);
		for i = 1, ClanMemNum do
			local member = ClanViewer.m_Members[i-1];
			if member.GroupID == groupIdx then
				local oneMember = {
					roleName = member.RoleMember.m_szRoleName,
					level = member.Level,
					career = member.Career,
					groupID = member.GroupID,
					nickName = member.NickName,
					leaveTime = GetTimeText( member ),
					memberIdx = i-1,
					pos = member.Position,
					lastOnline = member.LastOnline,
					online = member.Online,
				}
				table.insert( t_ServerMemberList, oneMember )
			end
		end
	else
		local ClanViewer	= GameClanManager:getClanViewer();
		local ClanMemNum	= ClanViewer.m_clanInfo.m_nClanMemNum;
		for i = 1, ClanMemNum do
			local member = ClanViewer.m_Members[i-1];
			local groupIdx = ClanGroupMemberInfoFrame:GetClientUserData(0);
			local oneMember = {
				roleName = member.RoleMember.m_szRoleName,
				level = member.Level,
				career = member.Career,
				groupID = member.GroupID,
				nickName = member.NickName,
				leaveTime = GetTimeText( member ),
				memberIdx = i-1,
				pos = member.Position,
				lastOnline = member.LastOnline,
				online = member.Online,
			}
			table.insert( t_ServerMemberList, oneMember )
		end
	end
	--�˶Է������µı�ͱ��ر�
	--��ȡ������û�е��µĳ�Ա
	for i = 1, table.getn( t_ServerMemberList ) do
		local find = false
		for j = 1, table.getn( t_GroupShowMemberList ) do
			if t_GroupShowMemberList[j].roleName == t_ServerMemberList[i].roleName then
				find = true
			end
		end
		if not find then
			table.insert( t_GroupShowMemberList, t_ServerMemberList[i] )
		end
	end
	--��ȡ�����б��г�Ա��������Ϣ��û���ҵ��ľ�ɾ��
	for i = 1, table.getn( t_GroupShowMemberList ) do
		local find = false		
		for j = 1, table.getn( t_ServerMemberList ) do
			if t_ServerMemberList[j].roleName == t_GroupShowMemberList[i].roleName then
				find = true
				t_GroupShowMemberList[i] = t_ServerMemberList[j];
			end
		end
		if not find then
			table.remove( t_GroupShowMemberList, i )
			i = i - 1
		end
	end
end

function SetClanGroupName( groupID, pos )
	if pos == CLAN_POS_PRESIDENT then
		local szGroupInfo = string.gsub( t_clanGroupName[groupID], "ս��", "" )
		szGroupInfo = szGroupInfo == "" and "" or string.format( "(%s)", szGroupInfo )
		return "�᳤"..szGroupInfo
	elseif pos == CLAN_POS_VICE_PRESIDENT then
		local szGroupInfo = string.gsub( t_clanGroupName[groupID], "ս��", "" )
		szGroupInfo = szGroupInfo == "" and "" or string.format( "(%s)", szGroupInfo )
		return "���᳤"..szGroupInfo
	elseif pos == CLAN_POS_GROUP_1 then
		return t_clanGroupName[groupID].."(�ų�)"
	elseif pos == CLAN_POS_GROUP_2 then
		return t_clanGroupName[groupID].."(�ų�)"
	elseif pos == CLAN_POS_GROUP_3 then
		return t_clanGroupName[groupID].."(�ų�)"
	elseif pos == CLAN_POS_GROUP_4 then
		return t_clanGroupName[groupID].."(�ų�)"
	elseif pos == CLAN_POS_GROUP_5 then
		return t_clanGroupName[groupID].."(�ų�)"
	elseif pos == CLAN_POS_GROUP_6 then
		return t_clanGroupName[groupID].."(�ų�)"
	else
		return t_clanGroupName[groupID]
	end
end

function SetClanGroupMemberInfo()
	local mainplayer = ActorMgr:getMainPlayer()
	--���֮ǰ��ʾ��
	for i = 1, CLAN_GROUP_SHOW_MEMBER_MAX_NUM do
		local btn = getglobal( "ClanGroupMemberInfoFrameInfoMember"..i )
		btn:Hide()
		btn:SetClientUserData( 0, 0 );
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( "" )
		local Level = getglobal( btn:GetName().."Level" )
		Level:SetText( "" )
		local RaceName = getglobal( btn:GetName().."RaceName" )
		RaceName:SetText( "" )
		local GroupName = getglobal( btn:GetName().."GroupName" )
		GroupName:SetText( "" )
		local NickName = getglobal( btn:GetName().."NickName" )
		NickName:SetText( "" )
		local LeaveTime = getglobal( btn:GetName().."LeaveTime" )
		LeaveTime:SetText( "" )
		local checkBtn = getglobal( btn:GetName().."CheckBtn" )
		checkBtn:setCheckState( false )
	end
	local groupIdx = ClanGroupMemberInfoFrame:GetClientUserData(0);
	ClanGroupMemberInfoFrameTotalNum:SetText( "��ս�ų�Ա������".. GetGroupMemberNum( groupIdx ) .."/"..(CLAN_GROUP_MEMBER_MAX_NUM + 1) )
	--����scrollbtn���ֵ
	if table.getn(t_GroupShowMemberList) > CLAN_GROUP_SHOW_MEMBER_MAX_NUM then
		ClanGroupMemberInfoFrameInfoScrollBar:Show()
		ClanGroupMemberInfoFrameInfoScrollBar:SetMaxValue( table.getn(t_GroupShowMemberList) - CLAN_GROUP_SHOW_MEMBER_MAX_NUM )
	else
		ClanGroupMemberInfoFrameInfoScrollBar:Hide()
		ClanGroupMemberInfoFrameInfoScrollBar:SetMaxValue(0);
	end
	PworldPolyPlayreInfoFrameInfoScrollBar:SetValue(math.min(ClanGroupMemberInfoFrameInfoScrollBar:GetMaxValue(), ClanGroupMemberInfoFrameInfoScrollBar:GetValue()))
	local endIndex = math.min( table.getn(t_GroupShowMemberList), CLAN_GROUP_SHOW_MEMBER_MAX_NUM + ClanGroupMemberInfoFrameInfoScrollBar:GetValue() )
	for i = 1 + ClanGroupMemberInfoFrameInfoScrollBar:GetValue(), endIndex do
		local member = t_GroupShowMemberList[i];
		local btn = getglobal( "ClanGroupMemberInfoFrameInfoMember"..(i-ClanGroupMemberInfoFrameInfoScrollBar:GetValue()) )
		btn:Show()
		btn:SetClientUserData( 0, member.memberIdx )
		local color = { 190,190,90, }
		if member.online == 0 then
			color = {160, 160, 160};
		else
			if mainplayer ~= nil then
				if member.roleName == mainplayer:getName() then
					color = { 120,235,50 };
				end
			end
		end
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( member.roleName )
		NameFont:SetTextColor( color[1], color[2], color[3] )
		local Level = getglobal( btn:GetName().."Level" )
		Level:SetText( member.level )
		Level:SetTextColor( color[1], color[2], color[3] )
		local RaceName = getglobal( btn:GetName().."RaceName" )
		RaceName:SetText( GetCareerName( member.career ) )
		RaceName:SetTextColor( color[1], color[2], color[3] )
		local GroupName = getglobal( btn:GetName().."GroupName" )
		GroupName:SetText( SetClanGroupName( member.groupID, member.pos ) )
		GroupName:SetTextColor( color[1], color[2], color[3] )
		local NickName = getglobal( btn:GetName().."NickName" )
		NickName:SetText( member.nickName )
		NickName:SetTextColor( color[1], color[2], color[3] )
		local LeaveTime = getglobal( btn:GetName().."LeaveTime" )
		LeaveTime:SetText(  member.leaveTime );
		LeaveTime:SetTextColor( color[1], color[2], color[3] )
		local checkBtn = getglobal( btn:GetName().."CheckBtn" )
		checkBtn:setCheckState( ( groupIdx == member.groupID ) )
	end
end
function ClanGroupMemberInfoFrame_OnShow()
	ClanGroupMemberInfoFrameOnlyShowSelfGroupCheckBtn:setCheckState( false );
	curSortOrder = true
	curSortElement = "default"

	SetGroupShowMemberList()
	SetClanGroupMemberInfo()
end
function ClanGroupMemberInfoFrame_OnUpdate()
	SetGroupShowMemberList()
	SetClanGroupMemberInfo()
end
function ClanGroupMemberInfoFrame_ScrollUpBtn()
	local slider = ClanGroupMemberInfoFrameInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end
function ClanGroupMemberInfoFrame_ScrollDownBtn()
	local slider	= ClanGroupMemberInfoFrameInfoScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end
function ClanGroupMemberInfoFrame_ScrollBarValueChange()
end
function ClanGroupMemberInfoFrameOnlyShowSelfGroup_OnClick()
	SetGroupShowMemberList()
	SetClanGroupMemberInfo()
	ClanGroupMemberInfoFrameOnlyShowSelfGroupCheckBtn:setCheckState( not ClanGroupMemberInfoFrameOnlyShowSelfGroupCheckBtn:GetCheckState() )
end
function ClanGroupMemberItemTemplate_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	local memIdx = this:GetClientUserData(0)
	local ClanViewer	= GameClanManager:getClanViewer();
	local member = ClanViewer.m_Members[memIdx];
	local groupIdx = ClanGroupMemberInfoFrame:GetClientUserData(0);
	--���Ȩ��1.��ͨ��Ա
	if mainplayer:getClanPosition() < CLAN_POS_VICE_PRESIDENT then
		ShowMidTips( "�ٽײ��㣬�޷����д˲���" )
		return;
	end
	--Ȩ�޼��2.ս�ų�
	if mainplayer:getClanPosition() > CLAN_POS_PRESIDENT then
		if mainplayer:getClanGroupID() ~= groupIdx then
			ShowMidTips( "��Ȩ�޶�����ս�Ž�����Ա����" )
			return			
		end
		if member.Position == CLAN_POS_PRESIDENT or member.Position == CLAN_POS_VICE_PRESIDENT then
			ShowMidTips( "�ٽײ��㣬�޷����д˲���" )
			return;
		end
		if member.GroupID ~= 0 and mainplayer:getClanGroupID() ~= member.GroupID then
			ShowMidTips( "�ó�Ա����ֻ���ɻ᳤���߸��᳤����" )
			return
		end
	end
	--Ȩ�޼��3.���᳤
	if mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT then
		if member.Position == CLAN_POS_PRESIDENT then
			ShowMidTips( "�ٽײ��㣬�޷����д˲���" )
		end
	end
	local checkBtn = getglobal( this:GetName().."CheckBtn" )
	if not checkBtn:GetCheckState() then
		--�����Ա��������
		if GetGroupMemberNum( groupIdx ) >= (CLAN_GROUP_MEMBER_MAX_NUM + 1) then
			ShowMidTips( t_clanGroupName[groupIdx].."��Ա����" )
			return 
		end
		--�����Ա���ﵽ���ֵ������û��ս�ų�
		if GetGroupMemberNum( groupIdx ) == CLAN_GROUP_MEMBER_MAX_NUM and not IsGroupHasLeader( groupIdx ) then
			ShowMidTips( "ÿ��ս��ֻ������29����Ա" )
			return
		end
		GameClanManager:Clan_Opt_SetGroup( this:GetClientUserData(0), groupIdx, 1 )
	else
		GameClanManager:Clan_Opt_SetGroup( this:GetClientUserData(0), groupIdx, 0 )
	end
end

function ClanGroupMemberItemTitleTemplate_OnClick()
	local index = this:GetClientID()
	if curSortElement ~= t_SortElements[index] then
		curSortOrder = true
	else
		curSortOrder = not curSortOrder
	end
	curSortElement = t_SortElements[index]
	SortClanGroupMemberList( t_GroupShowMemberList, curSortOrder, curSortElement );
end
