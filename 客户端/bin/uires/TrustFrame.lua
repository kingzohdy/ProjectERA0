local PAGEMAX				= 4;
local MAXITEM				= 3;
local MAXTRUST				= 5;
local ROLEMAX				= 24;
local nCheckClientId		= 0;
local nCheckRoleClientId	= 0;
local nCheckCurPage			= 0;
local nCheckCurRow			= 0;
local nCheckTrustInfoRow	= 0;
local MAINPLAYER			= 0;
local FRIEND				= 1;
local nTrustInfoType		= 1;
local MAX_AREA_NAME			= 18;
local MAX_LINE_ONE_PAGE		= 17;
local MAX_AWARD_ITEM		= 3;
local MAX_NEED_ITEM			= 3;

--##	XML_page: 4-673		##
--##	���ί�����		##
--##	LUA_page: 13-293	##

local NONE_SELECT			= -1;
local NONE_TRUST			= -1;
local nCheckTrustPage		= 0;
local nChkTrustSilValue		= 0;
local nCheckQuestId			= 0;
local LOWEST_LEVEL			= 3;
local nCanSubTrustNum		= 0;
local nCanNotSubTrustNum	= 0;
local firstMenuUV = {	normal		= {	x = 674, y = 850, width = 173, height = 26 },
						highlight	= {	x = 674, y = 877, width = 173, height = 26 },
						push		= {	x = 674, y = 904, width = 173, height = 26 }
					};

local secondMenuUV =	{	normal		= {	x = 674, y = 688, width = 173, height = 26 },
							highlight	= {	x = 674, y = 715, width = 173, height = 26 },
							push		= {	x = 674, y = 742, width = 173, height = 26 }
						};

local thirdMenuUV =	{	normal		= {	x = 1, y = 1,	width = 1,	 height = 1 },
						highlight	= {	x = 0, y = 382, width = 180, height = 31 },
						push		= {	x = 0, y = 414, width = 180, height = 31 },
					};

-- ��̬��ѯ��
local t_firstLevelArea	=	{
								{ areaType = MAP_AREA_All,		name = "ȫ��", level = 1 },
								--[[{ areaType = MAP_TYPE_OUTSIDE,	name = "Ұ��", level = 1 },
								{ areaType = MAP_TYPE_COPY,		name = "����", level = 1 },
								--]]
							};

local t_secondLevelArea =	{
									[MAP_AREA_ASIA]			= { name = "����",		level = 2 },
									[MAP_AREA_EUROPE]		= { name = "ŷ��",		level = 2 },
									[MAP_AREA_NORTHAMERICA] = { name = "������",	level = 2 },
									[MAP_AREA_SOUTHAMERICA] = { name = "������",	level = 2 },
									[MAP_AREA_AFRICA]		= { name = "����",		level = 2 },
									[MAP_AREA_BIRTH]		= { name = "���ִ�",	level = 2 },
									[MAP_AREA_CITY]			= { name = "����",		level = 2 },
							};
-- ��Ϊί���������������ʾί���������ֵ��б�
local t_areaNameList = {};

function TrustFrame_OnLoad()
	this:RegisterEvent( "GE_TRUST_UPDATEORDERLIST" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent("GE_TRUST_CHANGE");				--����仯	
	this:RegisterEvent("GE_QUEST_GETQUESTINFO");	


	for i = 1, PAGEMAX do
		local btn = getglobal( "TrustInfo" ..i .. "HighlightBtn" );
		btn:SetOverlayRegion( -5, -6, 684, 74 );
	end

	-- ��ʼ��ί�����������б�
	-- �������򰲲�
	-- ���Ȳ�ö����������ڵ�
	local t_allList			= {};
	for secondLevelAreaKey, secondLevelAreaValue in pairs( t_secondLevelArea ) do
		table.insert( t_allList, { level = 2, name = secondLevelAreaValue.name, bCheck = false, area = secondLevelAreaKey } );
		local nMapNum = 0
		
		for i = 1, MapShower:getMapInfoNum( secondLevelAreaKey ) do
			
			local mapInfo = MapShower:getMapInfoByIndex( secondLevelAreaKey, i - 1 );

			-- ��ȫ���б��н��а���
			if Quest:getMapTustNum( mapInfo.m_nMapID ) > 0 then
				table.insert( t_allList, {	level = LOWEST_LEVEL, name = mapInfo.m_szMapName,  area = secondLevelAreaKey,
											mapID = mapInfo.m_nMapID } );
				nMapNum = nMapNum + 1;
			end
		end
		
		-- �жϴ��޽ڵ��Ƿ��ж�Ӧ�ĵ�ͼ��ί������,��û,���Ƴ��ö����ڵ�
		if nMapNum == 0 then
			table.remove( t_allList );
		end
	end

	-- Ȼ����һ���Ͷ����ڵ�
	for i = 1, table.getn( t_firstLevelArea ) do
		-- ����һ���ڵ�
		table.insert( t_areaNameList, { level = t_firstLevelArea[i].level, name = t_firstLevelArea[i].name, bCheck = true,
										areaType = t_firstLevelArea[i].areaType } );

		if t_firstLevelArea[i].areaType == MAP_AREA_All then
			for j = 1, table.getn( t_allList ) do
				table.insert( t_areaNameList, { level = t_allList[j].level, name = t_allList[j].name, bCheck = t_allList[j].bCheck,
								areaType = t_firstLevelArea[i].areaType, area = t_allList[j].area, mapID = t_allList[j].mapID } );
			end
		end
	end
end

function TrustFrame_OnEvent()
	if ( arg1 == "GE_TRUST_UPDATEORDERLIST" ) or ( arg1 == "GE_QUEST_GETQUESTINFO" ) or
			( arg1 == "GE_CONTAINER_UPDATE" ) then
		local trustSlider = getglobal( "Trust_ScrollBar" );
		UpdateTrustInfo( trustSlider:GetValue() + 1 );
	end

	if ( arg1 == "GE_TRUST_CHANGE" ) then
		local trustSlider = getglobal( "Trust_ScrollBar" );
		UpdateTrustInfo( trustSlider:GetValue() + 1 );
		ShowMidTips( "ί�������ύ�ɹ�" );
	end
end

function TrustFrame_OnUpdate()
	local hour		= os.date("%H", os.time())
	local minute	= os.date("%M", os.time())
	local second	= os.date("%S", os.time())
	if tonumber( hour ) == 23 and tonumber( minute ) == 59 and tonumber( second ) == 59 then
		ReSetTrustOnedayNum();
	end
end

local t_modalFrames = {};
function TrustFrame_OnHide()
end

function TrustFrame_OnShow()
	this:SetPoint( "center", "$parent", "center", 0, 0  );
	local nameSlider = getglobal( "TrustName_ScrollBar" );
	nameSlider:SetValue( 0 );
	SetScrollBar( "TrustName_ScrollBar", MAX_AREA_NAME, GetTrustFrameTotalShowLines() );

	-- ����ί�����������б�
	if nameSlider:IsShown() then
		UpdateAreaNameList( nameSlider:GetValue() + 1 );
	else
		UpdateAreaNameList( 1 );
	end
	-- Ĭ��ѡ��ȫ��
	TrustFrame_AreaNameBtn1HighlightBtn:Checked();
	ShowTrustInfo( 1 );

	TrustAwardMoney:SetText( "������Ǯ��");
	TrustAwardExp:SetText( "�������飺" );
	
	local trustSlider = getglobal( "Trust_ScrollBar" );
	trustSlider:SetValue( 0 );
	UpdateTrustInfo( trustSlider:GetValue() + 1 );
end

-- @Desc: ��ȡ��ǰ������ʾ�������б�����
-- @retval: int	������
function GetTrustFrameTotalShowLines()
	local nTotalLines = 0;
	for i = 1, table.getn( t_areaNameList ) do
		if t_areaNameList[i].level == 1 then
			nTotalLines = nTotalLines + 1;
		elseif t_areaNameList[i].level == 2 then
			local firstLevelNode = GetParentNodeInfo( t_areaNameList[i] );
			-- ���丸�ڵ�Ϊչ��״̬, ����ʾ��ǰ��2���ڵ�����,����������ǰ2���ڵ�
			if firstLevelNode.bCheck then
				nTotalLines = nTotalLines + 1;
			end
		elseif t_areaNameList[i].level == 3 then
			-- 3���ڵ�ֻ�������Ӧ��1���ڵ��2���ڵ㶼Ϊչ��״̬��ʱ�����ʾ���Ӧ������
			local firstLevelNode, secondLevelNode = GetParentNodeInfo( t_areaNameList[i] );

			if firstLevelNode.bCheck and secondLevelNode.bCheck then
				nTotalLines = nTotalLines + 1;
			end
		end
	end
	return nTotalLines;
end

--@Desc: ������֤�ڵ������洢�����Ƿ�Ϊ��������
function IsAreaType( szName )
	assert( szName ~= "", "��Ҫ��ѯ�����ַǷ�!" );
	for key, value in pairs( t_firstLevelArea ) do
		if value.name == szName then
			return true;
		end
	end

	return false;
end

function GetNodeLevel( szName )
	assert( szName ~= "", "��Ҫ��ѯ�����ַǷ�!" );
	local t_levelList = {	{ name = "ȫ��", level = 1 },	{ name = "����", level = 1 },	{ name = "Ұ��", level = 1 },
							{ name = "����", level = 2 },	{ name = "ŷ��", level = 2 },	{ name = "����", level = 2 },
							{ name = "������", level = 2 }, { name = "������", level = 2 }, { name = "���ִ�", level = 2 },
							{ name = "����", level = 2 }
							};

	for i = 1, table.getn( t_levelList ) do
		if t_levelList[i].name == szName then
			return t_levelList[i].level;
		end
	end

	-- Ĭ��û�ҵ�������Ͳ�
	return LOWEST_LEVEL;
end

-- @Desc:	����ĳ��ָ���ӽڵ��Ӧ�ĸ��ڵ�
-- @retval: ����2���ڵ㷵�ص�1���ڵ����ϸ������Ϣ
-- @retval: ����3���ڵ㷵�ص�2���ڵ�͵�1���ڵ����ϸ������Ϣ
function GetParentNodeInfo( childNodeInfo )
	if childNodeInfo.level == 2 then
		for i = 1, table.getn( t_areaNameList ) do
			if t_areaNameList[i].level == 1 and t_areaNameList[i].areaType == childNodeInfo.areaType then
				return t_areaNameList[i];
			end
		end
	elseif childNodeInfo.level == 3 then
		for i = 1, table.getn( t_areaNameList ) do
			-- �ȶ�λ�����Ӧ��1���ڵ�
			if t_areaNameList[i].level == 1 and t_areaNameList[i].areaType == childNodeInfo.areaType then
				for j = i + 1,  table.getn( t_areaNameList ) do
					assert( t_areaNameList[j].level ~= 1, "�Ƿ�1���ڵ�!" );
					if t_areaNameList[j].area == childNodeInfo.area then
						return t_areaNameList[i], t_areaNameList[j];
					end
				end
			end
		end
	end
	assert( false, "û�л�ȡ���κνڵ�!"..childNodeInfo.name .. " " .. childNodeInfo.level );
end

--@Desc: ����ί��������ϵ����������б�
function UpdateAreaNameList( nStartContentIndex )
	local nCurShowBtn = 1; -- ��Ϊָ��ť��ָ��
	-- nContentIndex Ϊ����ָ���߼����ݵ�ָ��
	for nContentIndex = nStartContentIndex, table.getn( t_areaNameList ) do
		-- ��鰴ťָ���Ƿ�Խ��
		if nCurShowBtn > MAX_AREA_NAME then
			break;
		end

		local btn			= getglobal( "TrustFrame_AreaNameBtn" .. nCurShowBtn .. "HighlightBtn" );
		local font			= getglobal( "TrustFrame_AreaNameBtn" .. nCurShowBtn .. "HighlightBtnAreaName" );
		local typeDescFont	= getglobal( "TrustFrame_AreaNameBtn" .. nCurShowBtn .. "HighlightBtnAreaTypeDesc" );
		local partionTex	= getglobal( "TrustFrame_AreaNameBtn" .. nCurShowBtn .. "HighlightBtnPartionTex" );

		-- ��鵱ǰ��ʾ�Ľڵ��Ƿ�Ϊ1����2���ڵ�,����,��Ҫ������Ƿ�Ϊչ��״̬
		-- ����ǰ�������߼�����Ϊ1���ڵ�����,��ֱ����ʾ
		if t_areaNameList[nContentIndex].level == 1 then
			font:SetText( t_areaNameList[nContentIndex].name );
			btn:SetClientUserData( 0, t_areaNameList[nContentIndex].areaType );

			btn:SetNormalTextureTemplate( "GongYongAnNiuA1NormalTemplate" );
			btn:SetHighlightTextureTemplate( "GongYongAnNiuA1HighTemplate" );
			btn:SetPushedTextureTemplate( "GongYongAnNiuA1PushedTemplate" );
			btn:SetDisableTextureTemplate( "GongYongAnNiuA1DisabledTemplate" );

			font:SetTextColor( 255, 254, 189 );
			typeDescFont:SetText( "" );
			partionTex:Hide();
			btn:SetOverlayRegion( 0, 0, 173, 26 );
			btn:Show();
			nCurShowBtn = nCurShowBtn + 1;

		-- ��Ϊ2���ڵ�
		elseif t_areaNameList[nContentIndex].level == 2 then
			-- �Ȳ鿴�丸�ڵ��Ƿ�Ϊչ��״̬
			local firstLevelNode = GetParentNodeInfo( t_areaNameList[nContentIndex] );
			-- ���丸�ڵ�Ϊչ��״̬, ����ʾ��ǰ��2���ڵ�����,����������ǰ2���ڵ�
			if firstLevelNode.bCheck then
				font:SetText( t_areaNameList[nContentIndex].name );
				btn:SetClientUserData( 0, t_areaNameList[nContentIndex].areaType );

				btn:SetNormalTextureTemplate( "TransTextureTemplate" );
				btn:SetHighlightTextureTemplate( "PushedTextureTemplate" );
				btn:SetPushedTextureTemplate( "HighlightTextureTemplate" );
				btn:SetDisableTextureTemplate( "TransTextureTemplate" );

				font:SetTextColor( 255, 254, 189 );
				typeDescFont:SetText( "" );
				partionTex:Hide();
				btn:SetOverlayRegion( 0, 0, 173, 26 );
				btn:Show();
				nCurShowBtn = nCurShowBtn + 1;
			end

		elseif t_areaNameList[nContentIndex].level == 3 then
			-- 3���ڵ�ֻ�������Ӧ��1���ڵ��2���ڵ㶼Ϊչ��״̬��ʱ�����ʾ���Ӧ������
			local firstLevelNode, secondLevelNode = GetParentNodeInfo( t_areaNameList[nContentIndex] );
			if firstLevelNode.bCheck and secondLevelNode.bCheck then
				font:SetText( t_areaNameList[nContentIndex].name );
				-- ��ʾҰ�⻹�Ǹ���
				if t_areaNameList[nContentIndex].mapID >= 10000 then
					typeDescFont:SetText( "��������" );
				else
					typeDescFont:SetText( "��Ұ�⣩" );
				end

				typeDescFont:SetTextColor( 133, 122, 86 );

				btn:SetClientUserData( 0, t_areaNameList[nContentIndex].areaType );

				btn:SetNormalTextureTemplate( "TransTextureTemplate" );
				btn:SetHighlightTextureTemplate( "PushedTextureTemplate" );
				btn:SetPushedTextureTemplate( "HighlightTextureTemplate" );
				btn:SetDisableTextureTemplate( "TransTextureTemplate" );

				font:SetTextColor( 133, 122, 86 );
				partionTex:Show();
				btn:SetOverlayRegion( -4, 0, 179, 26 );
				btn:Show();
				nCurShowBtn = nCurShowBtn + 1;
			end
		end
	end

	-- ��ûʹ�õİ�ť��������
	for i = nCurShowBtn, MAX_AREA_NAME do
		local btn	= getglobal( "TrustFrame_AreaNameBtn" .. i .. "HighlightBtn" );
		btn:Hide();

		local font	= getglobal( "TrustFrame_AreaNameBtn" .. i .. "HighlightBtnAreaName" );
		font:SetText( "" );
	end
end

function TrustFrame_Close()
	TrustFrame:Hide();
end

-- @Desc:	��Ϊ��ʾ��ѡ�е�ί�������������ϸ��Ϣ
-- @Param:	nContentIndex	��������߼����ݵ�����
function ShowTrustInfo( nContentIndex )
	DisCheckAllHighLightBtn( "TrustFrame_AreaNameBtn", MAX_AREA_NAME );

	local firstLevelNode;
	local secondLevelNode;
	local nTyp;
	local nArea;
	-- ��ѡ�еĽڵ��������ڵ�,����ʾ���������ί����������
	if t_areaNameList[nContentIndex].level == 1 then

		nType = GetAreaByName( t_areaNameList[nContentIndex].name );
		Quest:selectAllTrustTask( true );
		Quest:clearTrustArea();

		for nKeyArea, _ in pairs( t_secondLevelArea ) do
			Quest:setTrustArea( nKeyArea );
		end
		Quest:reSetTrust( 0 );

	elseif t_areaNameList[nContentIndex].level == 2 then
		firstLevelNode = GetParentNodeInfo( t_areaNameList[nContentIndex] );
		Quest:clearTrustArea();
		Quest:selectAllTrustTask( false );
		Quest:setTrustArea( t_areaNameList[nContentIndex].area );
		Quest:reSetTrust( 0 );

	elseif t_areaNameList[nContentIndex].level == 3 then
		firstLevelNode, secondLevelNode = GetParentNodeInfo( t_areaNameList[nContentIndex] );
		nArea = GetAreaByName( secondLevelNode.name );
		Quest:clearTrustArea();
		Quest:selectAllTrustTask( false );
		Quest:setTrustArea( nArea );
		Quest:reSetTrust( t_areaNameList[nContentIndex].mapID );
		this:Checked();
	end

	nCheckQuestId = 0;
	local trustSlider = getglobal( "Trust_ScrollBar" );
	trustSlider:SetValue( 0 );
	UpdateTrustInfo( trustSlider:GetValue() + 1 );
end

--@Dewc: ��Ϊ������������б��еİ�ť��Ӧ
function TrustFrame_AreaNameBtn_OnClick()
	local slider		= getglobal( "TrustName_ScrollBar" );
	local font			= getglobal( this:GetName() .. "AreaName" );
	local nContentIndex = 0;
	-- ���ǵ�����״̬,����ɸ�ʽ�Ӳ�����
	local nDestAreaType = this:GetClientUserData( 0 );
	for i = 1, table.getn( t_areaNameList ) do
		-- ����ȷ�����Ӧ��1���ڵ�
		if t_areaNameList[i].level == 1 and t_areaNameList[i].areaType == nDestAreaType then
			-- ��Ѱ�ض�һ���ڵ㼯���е��ӽڵ�
			for j = i, table.getn( t_areaNameList ) do
				if t_areaNameList[j].name == font:GetText() then
					nContentIndex = j;
					break;
				end
			end
		end
	end

	-- ������Ľڵ㲻Ϊ3���ڵ�,�򲻻�����������
	if t_areaNameList[nContentIndex].level ~= LOWEST_LEVEL then
		-- �������е�1��2���ڵ�ԭ��������״̬,��Ҫ�Ƚ�����2���ڵ��չ��״̬��Ϊ����״̬
		if not t_areaNameList[nContentIndex].bCheck then
			for i = 1, table.getn( t_areaNameList ) do
				if t_areaNameList[i].level == 2 then
					t_areaNameList[i].bCheck = false;
				end
			end
		end
		-- �������ֵ�1��2���ڵ�ԭ����չ��״̬״̬,�����ı䵱ǰ������Ľڵ�
		if t_areaNameList[nContentIndex].level == 2 then
			t_areaNameList[nContentIndex].bCheck = not t_areaNameList[nContentIndex].bCheck;
		end

		-- �����������1���ڵ�,������2���ڵ��Ϊչ��״̬
		if t_areaNameList[nContentIndex].level == 1 then
			for i = 1, table.getn( t_areaNameList ) do
				if t_areaNameList[i].level == 2 then
					t_areaNameList[i].bCheck = false;
				end
			end
		end

		SetScrollBar( "TrustName_ScrollBar", MAX_AREA_NAME, GetTrustFrameTotalShowLines() );

		if slider:IsShown() then
			UpdateAreaNameList( slider:GetValue() + 1 );
		else
			UpdateAreaNameList( 1 );
		end
	end

	-- ѡ�б�����Ľڵ��Ӧ��ί�������������ϸ��Ϣ
	ShowTrustInfo( nContentIndex );
end

function TrustName_ScrollUpBtn_OnClick()
	local slider = getglobal( "TrustName_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function TrustName_ScrollDownBtn_OnClick()
	local slider	= getglobal( "TrustName_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function TrustName_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "TrustName_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateAreaNameList( nValue + 1 );
	end
end

function ReSetTrustOnedayNum()
	local nTrustNum = Quest:getNumTrust();
	for i = 1, nTrustNum do
		local nQuestId		= Quest:getTrustPageInfo( i-1 );
		if nQuestId == 0 then return end
		local nQuesttime	= Quest:getQusetTime( nQuestId );
		local day			= os.date("%d", os.time());
		local questday		= os.date("%M", nQuesttime);
		if day ~= questday then
			Quest:resetQusetNumOneDay();
			local trustSlider = getglobal( "Trust_ScrollBar" );
			UpdateTrustInfo( trustSlider:GetValue() + 1 );
		end
	end
end

function GetCanSubTrustNum()
	local nRowNum = Quest:getNumTrust();
end

function ClearAllTrustInfo()
	for i = 1, PAGEMAX do
		local button		= getglobal( "TrustInfo" .. i .. "HighlightBtn" );
		button:Disable();

		local trustNameFont	= getglobal( "TrustInfo" .. i .. "HighlightBtnTrustName" );
		trustNameFont:SetText( "" );

		local trustDescFont	= getglobal( "TrustInfo" .. i .. "HighlightBtnDesc" );
		trustDescFont:SetText( "" );

		local orderIcon	= getglobal( "TrustInfo" .. i .. "HighlightBtnOrderIcon" );
		orderIcon:Hide();

		--local noFiniIcon	= getglobal( "TrustInfo" .. i .. "HighlightBtnNoFiniTex" );
		--noFiniIcon:Hide();

		local finiIcon	= getglobal( "TrustInfo" .. i .. "HighlightBtnFiniTex" );
		finiIcon:Hide();

		local totalNumFont	= getglobal( "TrustInfo" .. i .. "HighlightBtnTotalNum" );
		totalNumFont:SetText( "" );

		local levelFont	= getglobal( "TrustInfo" .. i .. "HighlightBtnLevel" );
		levelFont:SetText( "" );

		local trackBtn = getglobal( "TrustInfo" .. i .. "HighlightBtnTrackBtn" );
		trackBtn:SetClientUserData( 0, 0 );
		trackBtn:Disable();
		trackBtn:Hide();

		local trackHeadBtn = getglobal( "TrustInfo" .. i .. "HighlightBtnTrackHeadBtn" );
		trackHeadBtn:Hide();

		for j = 1, MAX_NEED_ITEM do
			local needItemIcon = getglobal( "TrustInfo" .. i .. "HighlightBtnNeedItem" .. j );
			needItemIcon:Hide();

			local needItemIconTex = getglobal( "TrustInfo" .. i .. "HighlightBtnNeedItem" .. j .. "BoxTex" );
			needItemIconTex:Hide();
		end
	end

	-- ����ί����������ѡ��ί���������Ϣ
	TrustAwardMoney:SetText( "" );
	TrustAwardExp:SetText( "" );
	TrustNeedHeadBtn:Hide();
	TrustAwardHeadBtn:Hide();
	TrustAwardMoneyHeadBtn:Hide();
	TrustAwardExpHeadBtn:Hide();

	for i = 1, MAXITEM do
		local itemBtn = getglobal( "TrustInfoNeedItem" .. i );
		itemBtn:Hide();
		
		itemBtn = getglobal( "TrustInfoAwardItem" .. i );
		itemBtn:Hide();
	end
end

function UpdateTrustInfo( nPage )
	DisCheckAllHighLightBtn( "TrustInfo", PAGEMAX );
	ClearAllTrustInfo();
	
	local bShowCanNotSub	= ShowCanNotSubBtn:GetCheckState();
	local mainplayer		= ActorMgr:getMainPlayer();

	if mainplayer == nil then
		return;
	end

	local t_curShowTrust = {};
	-- ��ί�е����ݷֱ���֯�����ύ�벻���ύ�ı���
	for i = 1, Quest:getNumTrust() do
		local nQuestId = Quest:getTrustPageInfo( i - 1 );

		if nQuestId > 0 and bShowCanNotSub then
			table.insert( t_curShowTrust, nQuestId );
		elseif nQuestId > 0 and not bShowCanNotSub and mainplayer:getLv() >= Quest:getQuestLvLimit( nQuestId ) then
			table.insert( t_curShowTrust, nQuestId );
		end
	end
	
	local nButtonIndex = 1;
	-- ��ί�����ݵ�����
	for i = nPage, table.getn( t_curShowTrust ) do
		if nButtonIndex <= PAGEMAX then
			BondingTrustListInfo( i, nButtonIndex, t_curShowTrust[i], COLOR_WHITE );
			nButtonIndex = nButtonIndex + 1;
		end
	end

	-- ����ί�������б�Ĺ�����
	SetScrollBar( "Trust_ScrollBar", PAGEMAX, table.getn( t_curShowTrust ) );

	-- �����²���ί�н�����������Ʒ��Ϣ
	if table.getn( t_curShowTrust ) == 0 then
		return;
	end
	
	if nCheckQuestId == 0 then
		-- ����ǰûѡ���κ�һ��ί��������Ĭ����ʾ��ǰ��ʾ��ί������ĵ�һ�� 
		nCheckQuestId = t_curShowTrust[nPage];
	end
	ShowCheckTrustInfo( nCheckQuestId );
end

function SetFiniTex( szFrame, nValue, t_Color )
	local texture = getglobal(szFrame);
	if GameTrust:trust_TrustIsComplete( nValue ) and Quest:getQusetHaveFinishNumOneDay( nValue ) < Quest:getQuestMaxCanAcceptNumOneDay( nValue ) then
		texture:Show();
	end
end

function SetNoFiniTex( szFrame, nValue, t_Color )
	local texture = getglobal(szFrame);
	if not GameTrust:trust_TrustIsComplete( nValue ) then
		texture:Show();
	end
end

function SetOrderTex( szFrame, nValue, t_Color )
	local texture = getglobal(szFrame);
	local MainPlayerTrustList		= GameTrust:getMainPlayerTrustList();
	if MainPlayerTrustList == nil then
		return;
	end

	for i = 1, MainPlayerTrustList.m_nTrustNum do
		if nValue == MainPlayerTrustList.m_TrustTaskData[i-1].m_uTaskID then
			texture:Show();
			break;
		end
	end
end

function SetTrustButton( szFrame , nValue, t_Color )
	local button		= getglobal( szFrame );
	button:Enable();

	if nCheckQuestId == 0 and szFrame == "TrustInfo1" then
		nCheckQuestId = nValue;
	end

	if nCheckQuestId == nValue then
		button:Checked();
	end

	button:SetClientUserData( 0, nValue );
end

function SetTrackBtn( szFrame , nValue, t_Color )
	local button		= getglobal( szFrame );
	local headButton	= getglobal( button:GetParent() .. "TrackHeadBtn" );
	button:SetClientUserData( 0, nValue );
	button:Enable();
	-- ��⵱ǰί�������Ƿ�׷����
	if Quest:isQuestTrace( nValue ) then
		button:setCheckState( true);
	else
		button:setCheckState( false );
	end
	button:Show();
	headButton:Show();
end

function  BondingTrustListInfo( nPageIndex, nClientId, nQuestId, COLOR_DEFINE )
	local nNeedItemNum		= Quest:getQuestNeedItemNum( nQuestId );
	
	local nAwardItemNum		= Quest:getQuestItemNum( nQuestId, 0 );
	local mainplayer		= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	
	local t_TrustListInfo =
		{
			{"TrustInfo"..nClientId.."HighlightBtn",				nQuestId,																				GetColor()[COLOR_DEFINE], SetTrustButton}, -- �������Ч
			{"TrustInfo"..nClientId.."HighlightBtnFiniTex",			nQuestId,																				GetColor()[COLOR_DEFINE], SetFiniTex}, -- �������Ч
			{"TrustInfo"..nClientId.."HighlightBtnOrderIcon",		nQuestId,																				GetColor()[COLOR_DEFINE], SetOrderTex}, -- ����ͼ��
			{"TrustInfo"..nClientId.."HighlightBtnLevel",			"LV." .. Quest:getQuestLvLimit( nQuestId ),												GetColor()[COLOR_DEFINE], SetFontText},	-- �ȼ�
			{"TrustInfo"..nClientId.."HighlightBtnTotalNum",		(Quest:getQusetHaveFinishNumOneDay( nQuestId ).."/"..Quest:getQuestMaxCanAcceptNumOneDay( nQuestId )),		GetColor()[COLOR_DEFINE], SetFontText}, -- ��ɴ���
			{"TrustInfo"..nClientId.."HighlightBtnTrustName",		Quest:getQuestName( nQuestId ),															GetColor()[COLOR_DEFINE], SetFontText}, -- ί����
			{"TrustInfo"..nClientId.."HighlightBtnDesc",			ReplaceConsumeMoneyDesc( Quest:getQuestDes( nQuestId ), nQuestId, 0 ),															GetColor()[COLOR_DEFINE], SetFontText},	-- ί������
			{"TrustInfo"..nClientId.."HighlightBtnTrackBtn",		nQuestId,																				GetColor()[COLOR_DEFINE], SetTrackBtn},	-- ����׷��ί������
		};

	for i = 1, MAXITEM do
		local nNeedItemId = Quest:getQuestNeedItem( nQuestId, i-1 );
		if nNeedItemId ~= 0 then
			local ItemDef	 = getItemDef(nNeedItemId);
			local t_TrustListNeedItem = { "TrustInfo" .. nClientId .. "HighlightBtnNeedItem" .. i,		ItemDef.IconID,		GetColor()[COLOR_DEFINE], SetTexture};
			table.insert( t_TrustListInfo, t_TrustListNeedItem );

			local t_TrustListNeedItemBox = { "TrustInfo" .. nClientId .. "HighlightBtnNeedItem" .. i .. "BoxTex", "",	"",	SetBoxTexture };
			table.insert( t_TrustListInfo, t_TrustListNeedItemBox );
		else
			local t_TrustListNeedItem = { "TrustInfo" .. nClientId .. "HighlightBtnNeedItem" .. i,		"",					GetColor()[COLOR_DEFINE], SetTextureClear};
			table.insert( t_TrustListInfo, t_TrustListNeedItem );

			local t_TrustListNeedItemBox = { "TrustInfo" .. nClientId .. "HighlightBtnNeedItem" .. i .. "BoxTex", "",	"",	SetTextureClear };
			table.insert( t_TrustListInfo, t_TrustListNeedItemBox );
		end
	end

	for i = 1, table.getn( t_TrustListInfo ) do
		t_TrustListInfo[i][4]( t_TrustListInfo[i][1], t_TrustListInfo[i][2], t_TrustListInfo[i][3] );
	end

	-- �����ǵȼ������Խ�����,������ʾ��Ӧί�еİ�ť������
	local levelFont = getglobal( "TrustInfo"..nClientId.."HighlightBtnLevel" );
	local btn		= getglobal( "TrustInfo"..nClientId.."HighlightBtn" );
	local trackBtn	= getglobal( "TrustInfo"..nClientId.."HighlightBtnTrackBtn" );
	if mainplayer:getLv() < Quest:getQuestLvLimit( nQuestId ) then
		levelFont:SetTextColor( 255, 0, 0 );
		trackBtn:Disable();
	else
		levelFont:SetTextColor( 255, 254, 189 );
	end
end

function SetTextureClear( szFrame , nValue, t_Color )
	local texture		= getglobal( szFrame );
	texture:Hide();
end

--@Desc:���ί���б���ʱ��Ӧ
function TrustInfoHighLightBtn_OnClick()
	nCheckQuestId		= this:GetClientUserData( 0 );
	local trustSlider = getglobal( "Trust_ScrollBar" );
	nCheckTrustPage		= trustSlider:GetValue() + 1;
	UpdateTrustInfo( trustSlider:GetValue() + 1 );
end

function SetNeedTrustButton( szButtonName, nItemID )
	local button		= getglobal( szButtonName );
	local itemButton	= getglobal( szButtonName.."Item" );
	itemButton:SetClientUserData( 0, nItemID );
	button:Show();
end

function  BondingTrustNeedItemInfo( nClientId, nQuestId, COLOR_DEFINE )
	local nTrustNeedItemId		= Quest:getQuestNeedItem( nQuestId, nClientId-1 );
	local nTrustNeedItemCount	= Quest:getQuestNeedItemCount( nQuestId, nClientId-1 );
	local ItemDef				= getItemDef(nTrustNeedItemId);
	local MainPlayer			= ActorMgr:getMainPlayer();
	local container				= MainPlayer:getContainer();
	local count					= container:getItemCount( nTrustNeedItemId );
	local t_TrustListInfo =
		{
			{"TrustInfoNeedItem"..nClientId.."ItemIcon",		ItemDef.IconID,							GetColor()[COLOR_DEFINE], SetTexture},
			{"TrustInfoNeedItem"..nClientId.."Desc",			ItemDef.Name,							GetColor()[COLOR_DEFINE], SetFontText},
			{"TrustInfoNeedItem"..nClientId.."TotalNum",		(count.."/"..nTrustNeedItemCount ),		GetColor()[COLOR_DEFINE], SetFontText},
			{"TrustInfoNeedItem"..nClientId,					ItemDef.ItemID,							"",						  SetNeedTrustButton }
		}				
	for i = 1, table.getn( t_TrustListInfo ) do
		t_TrustListInfo[i][4](t_TrustListInfo[i][1],t_TrustListInfo[i][2],t_TrustListInfo[i][3]);
	end
end

function SetAwardButton( szButtonName, nItemID )
	local button = getglobal( szButtonName );
	button:SetClientUserData( 0, nItemID );
	button:Show();
end

function  BondingTrustAwardItemInfo( nClientId, nQuestId, COLOR_DEFINE, nTrustAwardItemId, nTrustAwardItemCount )
	local ItemDef				= getItemDef(nTrustAwardItemId);
	local MainPlayer			= ActorMgr:getMainPlayer();
	local container				= MainPlayer:getContainer();
	local count					= container:getItemCount( nTrustAwardItemId );

	local t_TrustListInfo =
		{
			{ "TrustInfoAwardItem"..nClientId.."Icon",			ItemDef.IconID,			GetColor()[COLOR_DEFINE],	SetTexture},
			{ "TrustInfoAwardItem"..nClientId.."InnerTotalNum",	nTrustAwardItemCount,	GetColor()[COLOR_DEFINE],	SetFontText},
			{ "TrustInfoAwardItem"..nClientId,					ItemDef.ItemID,			nil,						SetAwardButton },
		}
	
	for i = 1, table.getn( t_TrustListInfo ) do
		t_TrustListInfo[i][4](t_TrustListInfo[i][1],t_TrustListInfo[i][2],t_TrustListInfo[i][3]);
	end
end


--@Desc:��ʾ��ǰѡ�е�ί�е���ϸ��Ϣ
function ShowCheckTrustInfo( nQuestId )
	local szTrustDesc	= ReplaceConsumeMoneyDesc( Quest:getQuestDes( nQuestId ), nQuestId, 0 );
	local nNeedItemNum	= Quest:getQuestNeedItemNum( nQuestId, 0 );
	local nAwardItemNum	= Quest:getQuestItemNum( nQuestId );
	local TaskMoney		= Quest:getQuestMoney( nQuestId, 0 );
	local nQuestExp		= Quest:getQuestExp( nQuestId, 0 );

	for i=1, nNeedItemNum do
		if i > 3 then return end
		BondingTrustNeedItemInfo( i, nQuestId, COLOR_WHITE );
	end


	local nIndexItem = 1;
	for i = 1, nAwardItemNum do
		if nIndexItem > 3 then
			return;
		end
		local nTrustAwardItemId		= Quest:getQuestItem( nQuestId, 0, i-1 );
		if nTrustAwardItemId == 0 then return end
		local nTrustAwardItemCount	= Quest:getQuestItemCount( nQuestId, 0, i-1 );
		--��ȡ��������Ʒ����Ҫְҵ
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end

		if Quest:isQuestItemCareer(nQuestId, 0, i - 1, MainPlayer:getRace()) then
			BondingTrustAwardItemInfo( nIndexItem, nQuestId, COLOR_WHITE, nTrustAwardItemId, nTrustAwardItemCount );
			nIndexItem = nIndexItem + 1;
		end
	end

	TrustAwardMoney:SetText( "������Ǯ��"..TaskMoney.Val );
	TrustAwardExp:SetText( "�������飺"..nQuestExp );
	TrustNeedHeadBtn:Show();
	TrustAwardHeadBtn:Show();
	TrustAwardMoneyHeadBtn:Show();
	TrustAwardExpHeadBtn:Show();
end

--@Desc:��ʾ�����ύί��
function ShowCanNotSubBtn_OnClick()
	local slider = getglobal("Trust_ScrollBar");
	slider:SetValue( 0 );
	
	-- ȡ���Ѿ�ѡ�е�ί������
	nCheckQuestId = 0;
	UpdateTrustInfo( slider:GetValue() + 1 );
end

--@Desc: ���׷�а�ť��Ӧ
function TrustInfoTrackBtn_OnClick()
	if not this:GetCheckState() then
		Quest:delQuestTrace( this:GetClientUserData( 0 ) );
	else
		Quest:addQuestTrace( this:GetClientUserData( 0 ) );
	end
end

--@Desc: ���ί����Ʒ����ʱ����Ӧ
function TrustInfoButton_OnClick()
end

--@Desc:�����ҵ�
function Trust_CancelOrderBtn_OnClick()
	if nCheckQuestId == 0 then
		return;
	end
	if IsTrustOrder( nCheckQuestId ) then -- ί���Ƿ����ᵥ
		GameTrust:trust_CancelOrder( nCheckQuestId );
		--local trustSlider = getglobal( "Trust_ScrollBar" );
		--UpdateTrustInfo( trustSlider:GetValue() + 1 );
	end
end

--@Desc:�����ҵ�
function Trust_OrderBtn_OnClick()
	if nCheckQuestId == 0 then
		return;
	end

	if IsCanOrderTrust( nCheckQuestId ) then -- ί���Ƿ���ύ
		GameTrust:trust_Order( nCheckQuestId );
	end
end

--@Desc:�ύ��ǰί��
function Trust_SubBtn_OnClick()
	if nCheckQuestId == 0 then
		return;
	end
	if IsTrustCanSub() then -- ί���Ƿ���ύ
		Quest:awardQuest( 0, nCheckQuestId, 0 );
		--local trustSlider = getglobal( "Trust_ScrollBar" );
		--UpdateTrustInfo( trustSlider:GetValue() + 1 );
	end
end


--@Desc:ί���Ƿ��ѹҵ�
function IsTrustOrder( nQuestId )
	local MainPlayerTrustList	= GameTrust:getMainPlayerTrustList();
	if MainPlayerTrustList ~= nil then
		for i=1, MainPlayerTrustList.m_nTrustNum do
			if nQuestId == MainPlayerTrustList.m_TrustTaskData[i-1].m_uTaskID then
				return true;
			end
		end
	end
	ShowMidTips("��ί����δ�ҵ�");
	return false;
end

--@Desc:ί���Ƿ���ύ
function IsTrustCanSub()
	if nCheckQuestId == 0 then
		return;
	end
	if IsCanOrderTrust( nCheckQuestId ) then
		local player			= ActorMgr:getMainPlayer();
		local container			= player:getContainer();
		local nTrustNeedItem	= Quest:getQuestNeedItemNum( nCheckQuestId );
		for i = 1, nTrustNeedItem do
			local nTrustNeedItemId	= Quest:getQuestNeedItem(  nCheckQuestId, i-1 );
			local nTrustQuestNum	= Quest:getQuestNeedItemCount( nCheckQuestId, i-1 );
			local nItemCount		= container:getItemCount( nTrustNeedItemId );
			if nItemCount < nTrustQuestNum then
				ShowMidTips("��ί��������Ʒ����");
				return false;
			end
		end
	end

	if Quest:getQusetHaveFinishNumOneDay( nCheckQuestId ) >= Quest:getQuestMaxCanAcceptNumOneDay( nCheckQuestId ) then
		AddGameMiddleTips( GetMidTipsList()[MID_TRUST_HASTODAYLIMITFINISH] );
		return false;
	end

	return true;
end

--@Desc:�Ƿ�ɹҵ�ί��
function IsCanOrderTrust( nQuestId )
	local player		= ActorMgr:getMainPlayer();
	local playerlv		= player:getLv();
	local questLimitLv	= Quest:getQuestLvLimit( nQuestId );

	if playerlv < questLimitLv then
		ShowMidTips("��ǰ�ȼ�δ�ﵽί������");
		return false;
	end

	return true;
end

--@Desc:��������������ϼ�ͷʱ��Ӧ
--@return: Nothing
function Trust_ScrollUpBtn_OnClick()
	local slider = getglobal("Trust_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��������������¼�ͷʱ��Ӧ
--@return: Nothing
function Trust_ScrollDownBtn_OnClick()
	local slider = getglobal("Trust_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue	= slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:��Ϊ�ƶ�������ʱ������ˢ���ı�
--@return: Nothing
function Trust_ScrollBar_OnValueChanged()
	local slider	= getglobal("Trust_ScrollBar");
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateTrustInfo( slider:GetValue() + 1 );
	end
end

--##	XML_page: 674-1044		##
--##	�ҵ������б����		##
--##	LUA_page: 472-659		##

--@Desc:���ѹҵ��б����Ӧ
local t_FriendOrderListFrame	= {};
local nCheckTrustOrderListRow	= 0;
local nCheckListSillerValue		= 0;
local nCheckFriendIndex			= -1;
local g_nCurSelectRoleId = { nHeight = 0, nLow = 0 };
--@Desc:ע����ѹҵ����ݱ仯��Ϣ
function TrustOrderListFrame_OnLoad()
	this:RegisterEvent("GE_TRUST_UPDATETRUSTLIST");
	this:RegisterEvent("GE_TRUST_UPDATEORDERLIST");

	for i = 1, ROLEMAX do
		local btn = getglobal( "FriendOrderList" ..i .. "HighlightBtn" );
		btn:SetOverlayRegion( -8, -7, 187, 23 );
	end
end

function TrustOrderListFrame_OnEvent()
	if ( arg1 == "GE_TRUST_UPDATETRUSTLIST" ) then
		GameTrust:trust_GetOrderInfo();
	end

	if ( arg1 == "GE_TRUST_UPDATEORDERLIST" ) then
		if TrustOrderListFrame:IsShown() then
			DisCheckAllHighLightBtn( "FriendOrderList", ROLEMAX );
			SetScrollBar( "TrustOrderList_ScrollBar", ROLEMAX, GameTrust:getTrustRoleNum( ) );
			local slider = getglobal("TrustOrderList_ScrollBar");
			if slider:IsShown() then
				UpdateTrustOrderList( slider:GetValue() + 1 );
			else
				UpdateTrustOrderList( 1 );
			end
		end
	end
end

function TrustOrderListFrame_OnShow()
	for i = 1, ROLEMAX do
		local btn = getglobal( "FriendOrderList" .. i .. "HighlightBtn" );
		btn:Disable();
	end

	local slider = getglobal( "TrustOrderList_ScrollBar" );
	slider:Hide();
	GameTrust:trust_OpenTrustFace();
end

function TrustOrderListFrame_OnHide()
	if TrustFriendOrderInfoFrame:IsShown() then
		TrustFriendOrderInfoFrame:Hide();
	end

	if TrustContributeFrame:IsShown() then
		TrustContributeFrame:Hide();
	end
end

function FriendListOrderTrust_OnEnter()
	local player		= ActorMgr:getMainPlayer();
	local szRoleName	= player:getName();
	local namefont		= getglobal( this:GetName().."RoleName" );
	if namefont:GetText() ~= szRoleName then
		TrustOrderListFrame_CharacterName_OnEnter();
	end
end

--@Desc:ˢ�º��ѹҵ��б�	nCurLineIndex	����ʼ�߼���������
function UpdateTrustOrderList( nCurLineIndex )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end

	local nRoleNum		= GameTrust:getTrustRoleNum();
	local nIconIndex	= 1; -- ��Ϊָ��ؼ�������ָ��

	-- �ȼ��nStartContentIndex�Ƿ�Ϊ1����Ϊ1��Ҫ��������Ƿ��йҵ���������û�ҵ������ر������ǹҵ�����Ϣ
	if nCurLineIndex == 1 then
		local nMainContentIndex = -1;
		-- �ȼ���߼����ݵ�һ����Ӧ�������Ƿ�������
		local bFoundMainTrust = false;
		for i = 1, GameTrust:getTrustRoleNum() do
			local FriendTrust	= GameTrust:getFriendTrustList( i - 1 );
			local FriendInfo	= GameFriendManager:getFriendInfoByRoleId( FriendTrust.m_RoleId.nHeight, FriendTrust.m_RoleId.nLow );
			if FriendInfo == nil then
				bFoundMainTrust = true;
				nMainContentIndex = i;
				break;
			end
		end

		-- ������������ί������, 
		if bFoundMainTrust then
			BondingFriendOrderList( nMainContentIndex, nIconIndex, mainplayer:getName(), COLOR_WEAK_PURPLE );
			nIconIndex = nIconIndex + 1;
			nCurLineIndex = nCurLineIndex + 1;
		else	-- ������û������ί������, 
			BondingFriendOrderList( NONE_TRUST, nIconIndex, mainplayer:getName(), COLOR_WEAK_PURPLE );
			nIconIndex = nIconIndex + 1;
		end
	end

	for i = 1, nRoleNum do
		if nIconIndex > ROLEMAX then
			break;
		end

		local FriendTrust	= GameTrust:getFriendTrustList( i - 1 );
		local FriendInfo	= GameFriendManager:getFriendInfoByRoleId( FriendTrust.m_RoleId.nHeight, FriendTrust.m_RoleId.nLow );
		-- ���ж�Ӧ�������Ϣ�����Ҵ洢����Ҳ�Ϊ���ǵ�ʱ���ˢ������Ϣ���������
		if FriendInfo ~= nil and mainplayer:getName() ~= FriendInfo.m_szName then
			szRoleName	= FriendInfo.m_szName;
			BondingFriendOrderList( i, nIconIndex, szRoleName, COLOR_WHITE );
			nIconIndex		= nIconIndex + 1;
		end
	end

	for i = nIconIndex, ROLEMAX do
		local button	= getglobal("FriendOrderList"..i.."HighlightBtn");
		local icontex	= getglobal("FriendOrderList"..i.."HighlightBtnIcon");
		local namefont	= getglobal("FriendOrderList"..i.."HighlightBtnRoleName");
		button:Disable();
		icontex:Hide();
		namefont:SetText("");
	end
end

function SetFriendOrderListButton( szFrame , nValue, t_Color )
	local button = getglobal(szFrame);
	button:SetClientUserData( 0, tonumber(nValue) );
end

--@Desc:�󶨺��ѹҵ��б�
function  BondingFriendOrderList( nContentIndex, nIconIndex, szRoleName, COLOR_DEFINE )
	local t_TrustListInfo =
		{
			{"FriendOrderList"..nIconIndex.."HighlightBtn",				nContentIndex,		GetColor()[COLOR_DEFINE],	SetFriendOrderListButton},
			{"FriendOrderList"..nIconIndex.."HighlightBtnIcon",			NONE,				GetColor()[COLOR_DEFINE],	SetButton}, -- ����ͼ��
			{"FriendOrderList"..nIconIndex.."HighlightBtnRoleName",		szRoleName,			GetColor()[COLOR_DEFINE],	SetFontText},	-- �ȼ�
		}

	local btn = getglobal( "FriendOrderList" .. nIconIndex .. "HighlightBtn" );
	btn:Enable();
	for i = 1, table.getn( t_TrustListInfo ) do
		t_TrustListInfo[i][4](t_TrustListInfo[i][1],t_TrustListInfo[i][2],t_TrustListInfo[i][3]);
	end
	
	local tex = getglobal( "FriendOrderList"..nIconIndex.."HighlightBtnIcon" );
	if nContentIndex == NONE_TRUST then
		tex:Hide();
	end
end

--@Desc:���ѹҵ��б�������Ӧ
function FriendOrderListHighLightBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	DisCheckAllHighLightBtn( "FriendOrderList", ROLEMAX );
	this:Checked();

	local frame	= getglobal( this:GetName() );
	local namefont		= getglobal( this:GetName().."RoleName" );
	TrustOrderListFrameLookBtn:SetClientUserData( 0, frame:GetClientUserData( 0 ) )
	TrustOrderListFrameLookBtn:SetClientID( frame:GetClientID( ) );
	TrustOrderListFrameLookBtn:SetClientString( namefont:GetText() );
end

function FriendOrderListHighLightBtn_OnDoubleClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	local frame				= getglobal( this:GetName() );
	local szCheckRoleName	= getglobal( this:GetName().."RoleName" );
	local slider			= getglobal( "TrustOrderList_ScrollBar" );
	local mainPlayer		= ActorMgr:getMainPlayer();

	nCheckTrustOrderListRow	= frame:GetClientID( );
	nCheckFriendIndex		= frame:GetClientUserData( 0 );
	-- ������������ǣ�������û���κ�ί�������򵯳��鿴���
	if nCheckFriendIndex == NONE_TRUST then
		return;
	end
	-- ���浱ǰ��ѡ�еĽ�ɫ��ID��Ϣ
	local FriendTrust			= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	g_nCurSelectRoleId.nHeight	= FriendTrust.m_RoleId.nHeight;
	g_nCurSelectRoleId.nLow		= FriendTrust.m_RoleId.nLow;
	nCheckListSillerValue		= slider:GetValue();

	DisCheckAllHighLightBtn( "FriendOrderList", ROLEMAX );
	this:Checked();
	-- ���������Ϊ���ǣ��ҵ�ǰ����û�κ�ί��������Ϣ�����ܴ��������
	nTrustInfoType = FRIEND;
    if mainPlayer:getName() == szCheckRoleName:GetText() then
		nTrustInfoType = MAINPLAYER;
	end
	TrustFriendOrderInfoFrame:Show();
end

function TrustOrderListFrameLookBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	local bOneSelect = false;
	for i = 1, ROLEMAX do
		local btn = getglobal( "FriendOrderList" .. i .. "HighlightBtn" );
		if btn:IsChecked() then
			bOneSelect = true;
			break;
		end
	end

	if not bOneSelect then
		return;
	end

	local slider			= getglobal( "TrustOrderList_ScrollBar" );
	local mainPlayer		= ActorMgr:getMainPlayer();

	nCheckTrustOrderListRow		= TrustOrderListFrameLookBtn:GetClientID( );
	nCheckFriendIndex			= TrustOrderListFrameLookBtn:GetClientUserData( 0 );
	local FriendTrust			= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	g_nCurSelectRoleId.nHeight	= FriendTrust.m_RoleId.nHeight;
	g_nCurSelectRoleId.nLow		= FriendTrust.m_RoleId.nLow;
	nCheckListSillerValue		= slider:GetValue();

	nTrustInfoType = FRIEND;
    if mainPlayer:getName() == TrustOrderListFrameLookBtn:GetClientString() then
		nTrustInfoType = MAINPLAYER;
	end

	TrustFriendOrderInfoFrame:Show();
end

--@Desc:��������������ϼ�ͷʱ��Ӧ
--@return: Nothing
function TrustOrderList_ScrollUpBtn_OnClick()
	local slider = getglobal("TrustOrderList_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:��������������¼�ͷʱ��Ӧ
--@return: Nothing
function TrustOrderList_ScrollDownBtn_OnClick()
	local slider = getglobal("TrustOrderList_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function TrustOrderListFrameCancelBtn_OnClick()
end

function TrustOrderListFrameLookRecordBtn_OnClick()
	TrustHistoryFrame:Show();
end

--@Desc:��Ϊ�ƶ�������ʱ������ˢ���ı�
--@return: Nothing
function TrustOrderList_ScrollBar_OnValueChanged()
	local slider = getglobal("TrustOrderList_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();
	DisCheckAllHighLightBtn( "FriendOrderList", ROLEMAX );
	local nCheckClientId = nCheckTrustOrderListRow + nCheckListSillerValue - nValue;

	if nValue <= nMaxValue then
		if nCheckClientId > 0 then
			local CheckBtn = getglobal("FriendOrderList"..nCheckClientId.."HighlightBtn");
			CheckBtn:Checked();
		end
		UpdateTrustOrderList( slider:GetValue() + 1 );
	end
end

-- ��ҵ���ҵ������Ϸ��ֲ���ǩ�ĺ��ѱ�ǩʱ��Ӧ
function TrustOrderList_FriendBtn_OnClick()
	FriendShipFrame:SetPoint( "topleft", "TrustOrderListFrame", "topleft", 0, 0 );
	TrustOrderListFrame:Hide();
	FriendShipFrame:Show();
end

--##	XML_page: 1045-1358		##
--##	ί�в鿴����			##
--##	LUA_page: 663-843		##
local nTrustdataIndex	= 0;

function TrustFriendOrderInfoFrame_OnLoad()
	this:RegisterEvent("GE_TRUST_UPDATEORDERLIST");
	this:RegisterEvent("GE_TRUST_UPDATETRUSTLIST");
	for i = 1, MAXTRUST do
		local btn = getglobal( "FrideOrderInfo" ..i .. "HighlightBtn" );
		btn:SetOverlayRegion( -4, -5, 599, 58 );
	end
end

function TrustFriendOrderInfoFrame_OnEvent()
	if ( arg1 == "GE_TRUST_UPDATETRUSTLIST" ) then
		GameTrust:trust_GetOrderInfo();
	end

	if ( arg1 == "GE_TRUST_UPDATEORDERLIST" ) then
		if nCheckTrustOrderListRow ~= NONE then
			UpdateTrustOrderInfo();
		end
	end
end

function TrustFriendOrderInfoFrame_OnShow()
	TrustFrideOrderInfo_RefurbishBtn:Show();
	TrustFrideOrderInfo_CancleBtn:Show();

	if nTrustInfoType == MAINPLAYER then
		-- ��ʾ  ����ί���� ��ť
		TrustFrideOrderInfo_ClearBtn:Show();
		-- ��������  ��ť
		TrustFrideOrderInfo_ContributeBtn:Hide();
	end

	if  nTrustInfoType == FRIEND then
		-- ����  ����ί���� ��ť
		TrustFrideOrderInfo_ClearBtn:Hide();
		-- ��ʾ ����  ��ť
		TrustFrideOrderInfo_ContributeBtn:Show();
	end

	nTrustdataIndex = 1;
	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	local FriendTrust = GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	if FriendTrust.m_nTrustNum > 0 then
		local btn = getglobal( "FrideOrderInfo1HighlightBtn");
		btn:Checked();
	end
	
	UpdateTrustOrderInfo();
end

function ClearAllTrustOrderInfo()
	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	for i = 1, MAXTRUST do
		local button	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtn" );
		local font		= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnTrustName" );
		font:SetText( "" );
		button:Disable();

		for j=1, MAXITEM do
			local oneTrustButton	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j );
			local tex				= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "Icon" );
			local namefont			= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "ItemName" );
			local numfont			= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "TotalNum" );
			tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			oneTrustButton:SetClientUserData( 0, 0 );
			namefont:SetText( "" );
			numfont:SetText( "" );
			oneTrustButton:Hide();
		end
	end
end

function UpdateTrustOrderInfo()
	local FriendTrust = GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	-- ����û���κκ��ѹҵ�����ԭ����ѡ�еĹҵ����ѳ����ҵ���ҵ�������
	if FriendTrust == nil or FriendTrust.m_RoleId.nHeight ~= g_nCurSelectRoleId.nHeight or
		FriendTrust.m_RoleId.nLow ~= g_nCurSelectRoleId.nLow then	
		-- ��ղ鿴��������
		ClearAllTrustOrderInfo();
		-- ��ԭ��ѡ�еĺ���������Ϊ��ѡ���κ�һ��ί������
		nTrustdataIndex		= NONE_SELECT;
		nCheckFriendIndex	= NONE_SELECT;
		g_nCurSelectRoleId.nLow		= 0;
		g_nCurSelectRoleId.nHeight	= 0;
		return;
	end
	
	-- �ڲ鿴�����Ȼ��ί�������ʱ��
	-- �Ƚ���ˢ�½���Ĳ���
	for i = 1, FriendTrust.m_nTrustNum do
		local button	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtn" );
		local namefont	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnTrustName" );
		namefont:SetText( Quest:getQuestName( FriendTrust.m_TrustTaskData[i-1].m_uTaskID) );

		for j = 1, FriendTrust.m_TrustTaskData[i-1].m_uItemNum do
			local nTrustNeedItemId		= FriendTrust.m_TrustTaskData[i-1].m_TrustItem[j-1].m_uItemID;
			local nContributeCount		= FriendTrust.m_TrustTaskData[i-1].m_TrustItem[j-1].m_nNum;
			local nTrustNeedItemCount	= Quest:getQuestNeedItemCount( FriendTrust.m_TrustTaskData[i-1].m_uTaskID, j-1 );
			local ItemDef				= getItemDef( nTrustNeedItemId );
			
			local oneTrustButton	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j );
			local oneTrustItemButton= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "ItemBtn" );
			local IconTex			= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "Icon" );
			local namefont			= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "ItemName" );
			local countfont			= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnOneTrustFrideOrderInfoBtn" .. j .. "TotalNum" );
				
			oneTrustItemButton:SetClientUserData( 0, nTrustNeedItemId );

			local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			IconTex:SetTexture( IconPath );
			namefont:SetText( ItemDef.Name );
			countfont:SetText(  nContributeCount .. "/" .. nTrustNeedItemCount );
			oneTrustButton:Show();
		end

		for j = FriendTrust.m_TrustTaskData[i-1].m_uItemNum + 1, MAXITEM do
			local oneTrustButton	= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j );
			local IconTex			= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j.."Icon" );
			local namefont			= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j.."ItemName" );
			local countfont			= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j.."TotalNum" );
			IconTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			namefont:SetText("");
			countfont:SetText("");
			oneTrustButton:Hide();
		end
		button:Enable();
	end
	
	for i = FriendTrust.m_nTrustNum + 1, MAXTRUST do
		local button	= getglobal("FrideOrderInfo"..i.."HighlightBtn");
		local font		= getglobal( "FrideOrderInfo"..i.."HighlightBtnTrustName" );
		font:SetText( "" );

		for j=1, MAXITEM do
			local oneTrustButton = getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j );
			local tex		= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j.."Icon" );
			local namefont	= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j.."ItemName" );
			local numfont	= getglobal( "FrideOrderInfo"..i.."HighlightBtnOneTrustFrideOrderInfoBtn"..j.."TotalNum" );
			tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			oneTrustButton:SetClientUserData( 0, 0 );
			namefont:SetText( "" );
			numfont:SetText( "" );
			oneTrustButton:Hide();
		end
		button:Disable();
	end
	
	-- �ȼ�鵱ί��������ٵ����,�ı�ѡ��ί�����������
	if nTrustdataIndex >  FriendTrust.m_nTrustNum then
		nTrustdataIndex = 1;
		local btn = getglobal( "FrideOrderInfo1HighlightBtn");
		btn:Checked();
	end
end

--@Desc: ��ҳ�����ǰѡ��ί��ʱ��Ӧ
function TrustFrideOrderInfo_ClearBtn_OnClick()
	-- �����ڶԷ�
	if nTrustdataIndex == NONE_SELECT or TrustContributeFrame:IsShown() then
		return;
	end

	local FriendTrust	= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	local nTrustId		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_uTaskID;
	GameTrust:trust_CancelOrder( nTrustId );
end

-- ����鿴����ȡ����ť
function TrustFrideOrderInfo_CancleBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	TrustFriendOrderInfoFrame:Hide()
end

function TrustItemButton_OnEnter()
	local nItemID = this:GetClientUserData( 0 );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( nItemID ) );
end

function TrustItemButton_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function TrustFrideOrderInfo_CloseBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	TrustFriendOrderInfoFrame:Hide();
end

--@Desc:ˢ�º���ί�йҵ���Ϣ
function TrustFrideOrderInfo_RefurbishBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	GameTrust:trust_GetOrderInfo();
end


function OneTrustFrideOrderItemInfoBtn_OnEnter()
	if TrustContributeFrame:IsShown() then
		return;
	end

	local nItemID = this:GetClientUserData( 0 );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( nItemID ) );

	-- TODO: ��߼�һ������Ϣת���������ڵ���Ϣ
	local parentBtn		= getglobal( this:GetParent() ); 
	local ParentFrame	= getglobal( parentBtn:GetParent() ); 
	ParentFrame:Highlight();
end

function OneTrustFrideOrderItemInfoBtn_OnLeave()
	if TrustContributeFrame:IsShown() then
		return;
	end

	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();

	local parentBtn		= getglobal( this:GetParent() ); 
	local ParentFrame	= getglobal( parentBtn:GetParent() ); 
	ParentFrame:DisHighlight();
end

function OneTrustFrideOrderItemInfoBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	local parentBtn		= getglobal( this:GetParent() ); 
	local ParentFrame	= getglobal( parentBtn:GetParent() ); 
	ParentFrame:Checked();
	nTrustdataIndex		= ParentFrame:GetClientID();
end

function OneTrustFrideOrderInfoBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	local parentBtn	= getglobal( this:GetParent() ); 
	parentBtn:Checked();
	nTrustdataIndex	= parentBtn:GetClientID();
end

function OneTrustFrideOrderInfoBtn_OnDoubleClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	local parentBtn	= getglobal( this:GetParent() ); 
	parentBtn:Checked();
	nTrustdataIndex	= parentBtn:GetClientID();
	ShowPresentFrame( nTrustdataIndex, nCheckFriendIndex );
end

function OneTrustFrideOrderInfoBtn_OnEnter()
	if TrustContributeFrame:IsShown() then
		return;
	end

	local parentBtn		= getglobal( this:GetParent() ); 
	parentBtn:Highlight();
end

function OneTrustFrideOrderInfoBtn_OnLeave()
	if TrustContributeFrame:IsShown() then
		return;
	end

	local parentBtn		= getglobal( this:GetParent() ); 
	parentBtn:DisHighlight();
end

--@Desc:������͵�ǰѡ��ί��ʱ��Ӧ
function TrustFrideOrderInfo_ContributeBtn_OnClick()
	ShowPresentFrame( nTrustdataIndex, nCheckFriendIndex );
end

function ShowPresentFrame( nSelTrustDataIndex, nFriendIndex )
	-- �����ڶԷ�
	if nSelTrustDataIndex == NONE_SELECT or TrustContributeFrame:IsShown() then
		return;
	end
	
	ClearContributeFrame();
	UpdateTrustContribute( nFriendIndex, nSelTrustDataIndex );
	TrustContributeFrame:Show();
end

function FrideOrderInfoBtn_OnClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	this:Checked();
	-- ���ñ�ѡ�е���Ʒ
	local btn = getglobal( this:GetName() );
	nTrustdataIndex = btn:GetClientID();
end

function FrideOrderInfoBtn_OnDoubleClick()
	if TrustContributeFrame:IsShown() then
		return;
	end

	DisCheckAllHighLightBtn( "FrideOrderInfo", MAXTRUST );
	this:Checked();
	-- ���ñ�ѡ�е���Ʒ
	local btn = getglobal( this:GetName() );
	nTrustdataIndex = btn:GetClientID();
	ShowPresentFrame( nTrustdataIndex, nCheckFriendIndex );
end

-- ���ί���������
function ClearContributeFrame()
	for i = 1, MAXITEM do
		local edit = getglobal( "TrustContribute" .. i .. "EditBox" );
		edit:SetText( 0 );
	end

	GameTrust:delTrustItem(); -- ���ί��������
	-- ͬʱ���鿴���İ�ť��disable
	for i = 1, MAXTRUST do
		local button	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtn");
		local font		= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnTrustName");
		if font:GetText() ~= "" then
			button:Disable();
		end
	end
end

--##ί�����������##
function TrustContributeFrame_OnShow()
	
end

function ClearTrustContributeFrame()
	for i = 1, MAXITEM do
		local totalNumFont = getglobal( "TrustContribute" .. i .. "TotalNum" );
		totalNumFont:SetText( "" );

		local descFont = getglobal( "TrustContribute" .. i .. "Desc" );
		descFont:SetText( "" );

		local iconTex = getglobal( "TrustContribute" .. i .. "Icon" );
		iconTex:Hide();
	end
end

--@Desc: ��Ϊ����������ص�ʱ����һЩ��ղ���
function TrustContributeFrame_OnHide()
	ClearTrustContributeFrame();

	for i = 1, MAXTRUST do
		local button	= getglobal( "FrideOrderInfo" .. i .. "HighlightBtn");
		local font		= getglobal( "FrideOrderInfo" .. i .. "HighlightBtnTrustName");
		if font:GetText() ~= "" then
			button:Enable();
		end
	end
end

--@Desc:ˢ�������б�
function UpdateTrustContribute( nCheckFriendIndex, nTrustdataIndex )
	-- ����ʾ�µĽ�������,�����ԭ�������ϵ�����
	ClearTrustContributeFrame();
	local FriendTrust = GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	-- ����ǰѡ�еĺ��ѳ����ҵ�����ҵ��Ѿ�ʧЧ
	if g_nCurSelectRoleId.nHeight ~= FriendTrust.m_RoleId.nHeight or g_nCurSelectRoleId.nLow ~= FriendTrust.m_RoleId.nLow or
		FriendTrust == nil then
			nCheckFriendIndex	= NONE_SELECT;
			nTrustdataIndex		= NONE_SELECT;
			return;
	end

	local FriendTrust	= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	local nItemNum		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_uItemNum;
	local player		= ActorMgr:getMainPlayer();
	local container		= player:getContainer();

	for i=1, nItemNum do
		local nTrustId				= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_uTaskID;
		local nTrustNeedItemId		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_TrustItem[i-1].m_uItemID;
		local nContributeCount		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_TrustItem[i-1].m_nNum;
		local nTrustNeedItemCount	= Quest:getQuestNeedItemCount( nTrustId, i - 1 );
		local ItemDef				= getItemDef( nTrustNeedItemId );
		local nItemInContainer		= container:getItemCount( nTrustNeedItemId );
		local TrustContributeBtn	= getglobal( "TrustContribute" .. i );
		local TrustContributeIcon	= getglobal( "TrustContribute" .. i .. "Icon" );
		local TrustContributeDesc	= getglobal( "TrustContribute" .. i .. "Desc" );
		local TrustContributeTotal	= getglobal( "TrustContribute" .. i .. "TotalNum" );
		local TrustContributeEdit	= getglobal( "TrustContribute" .. i .. "EditBox" );
		TrustContributeDesc:SetText( ItemDef.Name );

		local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		TrustContributeIcon:SetTexture( IconPath );
		TrustContributeIcon:Show();
		TrustContributeTotal:SetText( nContributeCount .. "/" .. nTrustNeedItemCount );
		TrustContributeEdit:SetText( math.min( nItemInContainer, ( nTrustNeedItemCount - nContributeCount ) ) );
		TrustContributeBtn:Show();
	end

	for i=nItemNum+1,  MAXITEM do
		local TrustContributeBtn	= getglobal("TrustContribute"..i);
		TrustContributeBtn:Hide();
	end
end

-- @Desc: ����������Ʒ������
function ContributeEditNum_OnUpdate()
	if this:GetText() == "" then
		return;
	end

	local parent				= getglobal( this:GetParent() );
	local editbox				= getglobal( this:GetName() );
	local nClientId				= parent:GetClientID();
	local FriendTrust			= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	local nTrustNeedItemId		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_TrustItem[nClientId-1].m_uItemID;
	local nContributeCount		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_TrustItem[nClientId-1].m_nNum; -- ί��������Ʒ����
	local nTrustNeedItemCount	= Quest:getQuestNeedItemCount( FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_uTaskID, nClientId - 1 );
	local player				= ActorMgr:getMainPlayer();
	local container				= player:getContainer();

	-- �����ڱ����������Ʒ����
	local nMaxInput		= math.min( container:getItemCount( nTrustNeedItemId ), ( nTrustNeedItemCount - nContributeCount ) );
	local nCurInputNum	= tonumber( editbox:GetText() );

	if nCurInputNum == nil then
		editbox:SetText( 0 );
		nCurInputNum = 0;
	end

	if nCurInputNum > nMaxInput then
		editbox:SetText( nMaxInput );
	end
	CheckEditNumberInput( this );
end

--@Desc:��ί����
function ContributeAddBtn_OnClick()
	local parent				= getglobal( this:GetParent() );
	local editbox				= getglobal( this:GetParent().."EditBox" );
	local nClientId				= parent:GetClientID();
	local FriendTrust			= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
	local nTrustNeedItemId		= FriendTrust.m_TrustTaskData[nTrustdataIndex - 1].m_TrustItem[nClientId-1].m_uItemID;
	local nContributeCount		= FriendTrust.m_TrustTaskData[nTrustdataIndex - 1].m_TrustItem[nClientId-1].m_nNum; -- ί��������Ʒ����
	local nTrustNeedItemCount	= Quest:getQuestNeedItemCount( FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_uTaskID, nClientId - 1 );

	local player	= ActorMgr:getMainPlayer();
	local container = player:getContainer();
	-- �����ڱ����������Ʒ����
	local nMaxInput	= math.min( container:getItemCount( nTrustNeedItemId ), ( nTrustNeedItemCount - nContributeCount ) );

	local nCurNum = tonumber( editbox:GetText() );

	if nCurNum < nMaxInput then
		editbox:SetText( tostring( nCurNum + 1 ) );
	end
end

--@Desc:��ί����
function ContributeDelBtn_OnClick()
	local editbox = getglobal( this:GetParent().."EditBox" );
	local nCurNum = tonumber( editbox:GetText() );
	-- �����ڱ����������Ʒ����

	if nCurNum > 0 then
		editbox:SetText( tostring( nCurNum - 1 ) );
	end
end

--@Desc:ȷ������ί����
function TrustContribute_ContributeBtn_OnClick()
	local FriendTrust	= GameTrust:getFriendTrustList( nCheckFriendIndex-1 );
	local nTaskId		= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_uTaskID;
	local nIndex		= 0;
	local nItemNum		= 0;
	local nTotalItemNum	= 0;

	for i=1, MAXITEM do
		local TrustContributeEditBox = getglobal("TrustContribute"..i.."EditBox");
		nItemNum = tonumber( TrustContributeEditBox:GetText() ) or 0;

		if nItemNum > 0 then
			local nItemId	= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_TrustItem[i-1].m_uItemID;
			GameTrust:addTrustItem( nIndex, nItemId, nItemNum );
			nIndex = nIndex + 1;
			nTotalItemNum = nTotalItemNum + nItemNum;
		end
	end

	local bHaveTrustInContainer = false;
	for i = 1, MAXITEM do
		local button			= getglobal( "TrustContribute" .. i );
		local nClientId			= button:GetClientID();
		local FriendTrust		= GameTrust:getFriendTrustList( nCheckFriendIndex - 1 );
		local nTrustNeedItemId	= FriendTrust.m_TrustTaskData[nTrustdataIndex-1].m_TrustItem[nClientId-1].m_uItemID;
		local player			= ActorMgr:getMainPlayer();
		local container			= player:getContainer();
		local nItemInContainer	= container:getItemCount( nTrustNeedItemId );

		if nItemInContainer > 0 then
			bHaveTrustInContainer = true;
		end
	end
	
	-- �����������жԷ��������Ʒ,���������������Ϊ0���ܵ��ȷ�����ر�������壬��������ʾ
	if nTotalItemNum == 0 and bHaveTrustInContainer then
		ShowMidTips( "��༭��Ʒ������" );
		return;
	end
	
	TrustContributeFrame:Hide();
	if nTotalItemNum == 0 then
		ShowMidTips( "������û�жԷ������ί����Ʒ��" );
		return;
	end

	GameTrust:trust_Contribute( FriendTrust.m_RoleId.nHeight, FriendTrust.m_RoleId.nLow, nTaskId );
end

--@Desc: ��Ϊ�����������ˢ�°�ť��ʱ����Ӧ
function TrustContributeFrame_RefreshBtn_OnClick()
	if nTrustdataIndex == NONE_SELECT then
		return;
	end
	UpdateTrustContribute( nCheckFriendIndex, nTrustdataIndex );
end

function TrustContribute_BackBtn_OnClick()
	SetFocusFrame( this:GetParent() .. "EditBox" );
end

function GetAreaByName( szAreaName )
	local t_TrustArea = { { "����", MAP_AREA_ASIA },			{ "ŷ��", MAP_AREA_EUROPE },	{ "������", MAP_AREA_NORTHAMERICA },
						  { "������", MAP_AREA_SOUTHAMERICA },	{ "����", MAP_AREA_AFRICA },	{ "���ִ�", MAP_AREA_BIRTH },
						  { "����", MAP_AREA_CITY },
						  { "ȫ��", MAP_AREA_All },				{ "����", MAP_TYPE_COPY },		{ "Ұ��", MAP_TYPE_OUTSIDE },
						};
	for i=1, table.getn( t_TrustArea ) do
		if t_TrustArea[i][1] == szAreaName then
			return t_TrustArea[i][2];
		end
	end
end

--@Desc: ��Ϊ�����ʷ��¼�����ʾ��ʱ����Ӧ
function TrustHistoryFrame_OnShow()
	SetScrollBar( "TrustHistoryFrame_ScrollBar", MAX_LINE_ONE_PAGE, TrustHistoryFrame_DisplayRich:GetTextLines() );
end

--@Desc: ��Ϊ�����ʷ��¼����е�ȫ��ɾ���İ�ť��ʱ����Ӧ
function TrustHistoryFrame_DeleteBtn_OnClick()
	TrustHistoryFrame_DisplayRich:Clear();
end

--@Desc: ��Ϊ�����ʷ��¼����е�ȡ���İ�ť��ʱ����Ӧ
function TrustHistoryFrame_CancelBtn_OnClick()
end

--@Desc: ��Ϊ�����ʷ��¼����е����ϵİ�ť��ʱ����Ӧ
function TrustHistoryFrame_ScrollUpBtn_OnClick()
	local slider		= getglobal( "TrustHistoryFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue		= slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc: ��Ϊ�����ʷ��¼����е����µİ�ť��ʱ����Ӧ
function TrustHistoryFrame_ScrollDownBtn_OnClick()
	local slider		= getglobal( "TrustHistoryFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue		= slider:GetMaxValue();
	local nValue		= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc: ��Ϊ�����ʷ��¼����еĹ������ƶ�ʱ����Ӧ
function TrustHistoryFrame_ScrollBar_OnValueChanged()
	local slider		= getglobal( "TrustHistoryFrame_ScrollBar" );
	local nValue		= slider:GetValue();
	local nMaxValue		= slider:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( "TrustHistoryFrame_DisplayRich", nValue );
	end
end

function trustContributeMsg( szContributeMsg )
	ShowMidTips( szContributeMsg );
	local rich = getglobal( "TrustHistoryFrame_DisplayRich" );
	rich:AddText( szContributeMsg, 175, 152 ,113 );
	local slider = getglobal( "TrustHistoryFrame_ScrollBar" );
	SetScrollBar( "TrustHistoryFrame_ScrollBar", MAX_LINE_ONE_PAGE, TrustHistoryFrame_DisplayRich:GetTextLines() );
	slider:SetValue( slider:GetMaxValue() );
	rich:ScrollEnd();
end