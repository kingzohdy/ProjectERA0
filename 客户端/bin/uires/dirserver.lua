g_szIP="192.168.0.77";
g_nPort=6721;
g_bIsRefurbish = false;
-- 优先级， 双线 》荐 》新

local MAX_AREA_NUM				= 8;
local SECOND_MAX_AREA_NUM		= 4;
local MAX_WORLD_NUM				= 16;
local MAX_RECOMMENT_AREA_NUM	= 4;
local MAX_AREA_LINE_NUM			= 4;
local MAX_BTN_PER_LINE			= 5;

local MAX_DIAN_XIN_AREA_NUM		= 10;
local MAX_WANG_TONG_AREA_NUM	= 10;

SelectWorldInfo =
{
	["bigworld"]		= "",
	["world"]			= "",
	["ip"]				= "",
	["port"]			= 0,
	["worldId"]			= 0,
	["bigWorldIndex"]	= -1,
	["worldIndex"]		= -1,
};


AUTH_ERR_SYS	= -1;
AUTH_ERR_NET	= -2;
AUTH_ERR_PASSWD	= -3;
AUTH_ERR_TDR	= -4;
AUTH_ERR_NOUSR	= -5;

local nLastRefreshTime = 0;

local t_ServerColor = {
						[CLIENT_DIR_NOT_WORK] = {205,205,205};
						[CLIENT_DIR_EXCELLENCE] = {80,200,0};
						[CLIENT_DIR_NORMAL] = {80,200,0};
						[CLIENT_DIR_BUSY] = {255,210,0};
						[CLIENT_DIR_FULL] = {200,0,0};
						}
local t_ServerBusyState = 
{
	[CLIENT_DIR_NOT_WORK]	= { desc = "(维护)", path = "uires\\TuPianLei\\XuanZeFuWuQi\\ZhuangTai5.tga", UV = { x = 945, y = 884, width = 14, height = 14 },	Color = { 100,100,100  } },
	[CLIENT_DIR_EXCELLENCE] = { desc = "(流畅)", path = "uires\\TuPianLei\\XuanZeFuWuQi\\ZhuangTai4.tga", UV = { x = 945, y = 869, width = 14, height = 14 },	Color = { 0,  230, 0} },
	[CLIENT_DIR_NORMAL]		= { desc = "(一般)", path = "uires\\TuPianLei\\XuanZeFuWuQi\\ZhuangTai3.tga", UV = { x = 931, y = 858, width = 14, height = 14 },	Color = {230, 200, 0 }},
	[CLIENT_DIR_BUSY]		= { desc = "(繁忙)", path = "uires\\TuPianLei\\XuanZeFuWuQi\\ZhuangTai2.tga", UV = { x = 358, y = 502, width = 14, height = 14 },	Color = {150, 0, 255}},
	[CLIENT_DIR_FULL]		= { desc = "(爆满)", path = "uires\\TuPianLei\\XuanZeFuWuQi\\ZhuangTai1.tga", UV = { x = 916, y = 858, width = 14, height = 14 }, Color = {230,0,0 }},
};

local function UpdateDirSelectLineChk()
	for i=1,3 do
		local btn = getglobal( "DirServerFrameLineChk" .. i )
		btn:setCheckState( false )
	end
	--DisCheckAllBtn( "DirServerFrameLineChk", 3 )
	local idx = LoginMgr:GetIpMode() + 1
	if idx > 0 then
		local btn = getglobal( "DirServerFrameLineChk" .. idx )
		btn:setCheckState( true )
	end
end

function GetServerBusyDesc( data )
	local t_oneBusyInfo = t_ServerBusyState[data["state"]];
	if t_oneBusyInfo == nil then
		local t_defaultBusyInfo = t_ServerBusyState[CLIENT_DIR_NOT_WORK];
		return t_defaultBusyInfo["desc"];
	end

	return t_oneBusyInfo["desc"];
end
function GetServerBusyColor( data )
	local t_oneBusyInfo = t_ServerBusyState[data["state"]];
	if t_oneBusyInfo == nil then
		local t_defaultBusyInfo = t_ServerBusyState[CLIENT_DIR_NOT_WORK];
		return t_defaultBusyInfo["Color"];
	end

	return t_oneBusyInfo["Color"];
end
function GetServerBusyTex( data )
	local t_oneBusyInfo = t_ServerBusyState[data["state"]];
	if t_oneBusyInfo == nil then
		local t_defaultBusyInfo = t_ServerBusyState[CLIENT_DIR_NOT_WORK];
		return { ["path"] = t_defaultBusyInfo["path"], ["UV"] = t_defaultBusyInfo["UV"] };
	end

	return { ["path"] = t_oneBusyInfo["path"], ["UV"] = t_oneBusyInfo["UV"] };
end

function GetFirstRecommendWorld()
	local nAreaIndex = 0;
	local nWorldIndex = 0;
	for i = 1, LoginMgr:getNumBigWorld() do
		local bFound = false;
		local bigWorldInfo = LoginMgr:getBigWorld( i - 1 );
		for j = 1, bigWorldInfo:getNumWorld() do
			local oneWorldInfo = bigWorldInfo:getIthWorld( j - 1 );	
			if oneWorldInfo.recommend then
				nWorldIndex = j - 1;
				bFound = true;
				break;
			end
		end

		if bFound then
			nAreaIndex = i - 1;
			break;
		end
	end
	return nAreaIndex, nWorldIndex;
end

function InitSelectAreaAndWorld()
	if SelectWorldInfo["bigWorldIndex"] ~= -1 then
		return;
	end

	local nLastSelectAreaIndex = LoginMgr:getLastSelectArea();
	local nLastSelectWorldIndex = LoginMgr:getLastSelectWorld();
	local nAreaIndex, nWorldIndex = GetFirstRecommendWorld();

	if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = nAreaIndex;
	end

	local oneBigWorld = LoginMgr:getBigWorld( nLastSelectAreaIndex );
	if nLastSelectWorldIndex < 0 or nLastSelectWorldIndex >= oneBigWorld:getNumWorld() then
		nLastSelectWorldIndex = nWorldIndex;
	end
	local world = oneBigWorld:getIthWorld( nLastSelectWorldIndex );
	SelectWorldInfo.bigworld = oneBigWorld:getName();
	SaveSelectWorldInfo( world, nLastSelectAreaIndex + 1, nLastSelectWorldIndex + 1 );
end

-- 点击大区按钮
function AreaBtn_OnClick()
	--将当前所有的BigWorld设为可用状态
	local nSelectAreaIndex = this:GetClientUserData( 0 );
	
	local oneBigWorld			= LoginMgr:getBigWorld( nSelectAreaIndex - 1 )
	SelectWorldInfo.bigworld	= oneBigWorld:getName();
	local nSelectWorldIndex		= 0;
	local world					= oneBigWorld:getIthWorld( nSelectWorldIndex );
	SaveSelectWorldInfo( world, nSelectAreaIndex, nSelectWorldIndex + 1 );
	DisCheckAllBtn( "BigWorldBtn", MAX_AREA_NUM );
	DisCheckAllBtn( "SecondTypeBigWorldBtn", SECOND_MAX_AREA_NUM );
	UpdateDirServerSelectAreaWorld( { areaIndex = nSelectAreaIndex } );
	this:Checked();

	local nLastSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"] - 1;

	--UpdateRecommendAreas();
end

function RecommendTBigWorldBtn_OnClick()
	DisCheckAllBtn( "DirServerFrameRecomendBigWorldBtn", MAX_RECOMMENT_AREA_NUM );
	this:Checked();
	RecoveryAllFontColor("DirServerFrameRecomendBigWorldBtn", MAX_RECOMMENT_AREA_NUM );
	
	local nSelectAreaIndex		= this:GetClientUserData( 0 );
	local oneBigWorld			= LoginMgr:getBigWorld( nSelectAreaIndex - 1 )
	SelectWorldInfo.bigworld	= oneBigWorld:getName();

	local nSelectWorldIndex		= this:GetClientUserData( 1 ) - 1;
	local world					= oneBigWorld:getIthWorld( nSelectWorldIndex );
	SaveSelectWorldInfo( world, nSelectAreaIndex, nSelectWorldIndex + 1 );
	UpdateDirServerSelectAreaWorld( { areaIndex = nSelectAreaIndex } );
	local nAreaIndex;
	DisCheckAllBtn( "BigWorldBtn", MAX_AREA_NUM );
	for i = 1, MAX_AREA_NUM do
		local btn = getglobal( "BigWorldBtn"..i );
		if not btn:IsShown() then 
			break;
		end
		nAreaIndex	= btn:GetClientUserData( 0 );
		if nAreaIndex == nSelectAreaIndex then
			btn:Checked();
		end
	end
	DisCheckAllBtn( "SecondTypeBigWorldBtn", SECOND_MAX_AREA_NUM );
	for j = 1, SECOND_MAX_AREA_NUM do 
		local SecondBtn = getglobal( "SecondTypeBigWorldBtn"..j );
		if not btn:IsShown() then 
			break;
		end
		nAreaIndex	= SecondBtn:GetClientUserData( 0 );
		if nAreaIndex == nSelectAreaIndex then
			SecondBtn:Checked();
		end
	end
	local BigWFont	= getglobal( this:GetName().."Font" );
	local WFont 	= getglobal( this:GetName().."WorldFont" );
	BigWFont:SetTextColor(255,255,190);
	WFont:SetTextColor(255,255,190);
end

function RecommendTBigWorldBtn_OnDoubleClick()
	RecommendTBigWorldBtn_OnClick();
	DirFrame_OnClickSelectWorldBtn();
end
function RecommendTBigWorldBtn_OnEnter()
	if not this:IsChecked() then 
		local BigWFont	= getglobal( this:GetName().."Font" );
		local WFont 	= getglobal( this:GetName().."WorldFont" );
		BigWFont:SetTextColor(255,255,255);
		WFont:SetTextColor(255,255,255);
	end
	
end
function RecommendTBigWorldBtn_OnLeave()
	if not this:IsChecked() then 
		local BigWFont	= getglobal( this:GetName().."Font" );
		local WFont 	= getglobal( this:GetName().."WorldFont" );
		BigWFont:SetTextColor(220,220,100);
		WFont:SetTextColor(220,220,100);
	end
end
--elmer
function RecoveryAllFontColor( btnName, MaxNum )
	for i=1, MaxNum do  
		local btn = getglobal(btnName..i);
		local BigWFont	= getglobal( btn:GetName().."Font" );
		local WFont 	= getglobal( btn:GetName().."WorldFont" );
		BigWFont:SetTextColor(220,220,100);
		WFont:SetTextColor(220,220,100);
	end
end

-- 点击世界按钮
function TWorldBtn_OnClick()
	local i = this:GetClientUserData(0);
	local j = this:GetClientUserData(1);

	--将当前选中的Word设为不可用状态
	local smallWord = getglobal("WorldBtn"..j);
	local bw		= LoginMgr:getBigWorld(i-1);
	local world		= bw:getIthWorld(j-1);

	--保存当前被选中Word的信息
	SelectWorldInfo.bigworld = bw:getName();
	SaveSelectWorldInfo(world, i, j );

	DisCheckAllBtn( "WorldBtn", MAX_WORLD_NUM );
	this:Checked();
end

function DirFrame_OnDoubleClickWorldBtn()
	TWorldBtn_OnClick();
	DirFrame_OnClickSelectWorldBtn();
end

-- 确定  登录
function DirFrame_OnClickSelectWorldBtn()
	if SelectWorldInfo.busy == CLIENT_DIR_NOT_WORK and not GameMgr:IsTestMode() then
		return;
	end

	if DirConfirmQuiGameFrame:IsShown() then
		return;
	end

	RefreshIthWorldInfo();

	DirMessageBoxFrameDescFont:SetText("本服务器含有PvP内容，未成年人请慎入");
	DirMessageBoxFrame:Show();
	
	local nLastSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"] - 1;
	if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = 0;
	end
	local selectBigWorld		= LoginMgr:getBigWorld( nLastSelectAreaIndex );
	local nLastSelectWorldIndex = SelectWorldInfo["worldIndex"] - 1;
	if nLastSelectWorldIndex < 0 or nLastSelectWorldIndex >= selectBigWorld:getNumWorld() then
		nLastSelectWorldIndex = 0;
	end
	local selectWorld = selectBigWorld:getIthWorld( nLastSelectWorldIndex );
	LoginMgr:getOptimalBakIp( selectWorld );
end

function DirFrameOkBtn_OnClick()
	if DirMessageBoxFrameDescFont:GetText() == "本服务器含有PvP内容，未成年人请慎入" then
		if LoginMgr:GetIpMode() ~= -1 then
			EnterLoginFrame();
		else
			Login_MessageBoxOK( { ["text"] = "请选择线路模式", ["opDesc"] = "" } );
		end
	end
	DirMessageBoxFrame:Hide();
end

function DirFrameCancelBtn_OnClick()
	DirMessageBoxFrame:Hide();
end

function EnterLoginFrame()
	DirServerFrame:Hide();
	DirSimpleServerFrame:Hide();
	LoginFrame:Show();
	LoginBackFrame:Show();
	GameBackground:show();					 -- 显示登录场景背景
	GameBackground:playCameraAnim(99100, 0); -- 把相机定格在登录位置
	GameBackground:setViewFlag( VIEW_POINT_LOGIN );
end

local t_DirServerFrameControl = {};
t_DirServerFrameControl["canStartCangBao"] = 
function ( self, data )
end

function ClearDirServerBigArea()
	for i = 1, MAX_AREA_NUM do
		local areaBtn = getglobal( "BigWorldBtn"..i );
		areaBtn:Hide();

		local newFlagTex = getglobal( areaBtn:GetName().."NewFlagTex" );
		newFlagTex:Hide();
		local uvTex	=  getglobal( areaBtn:GetName().."UVAnimationTex" );
		uvTex:Hide();
		local shuangXianFlagTex = getglobal( areaBtn:GetName().."ShuangXianFlagTex" );
		shuangXianFlagTex:Hide();
		
		areaBtn:DisChecked();
		-- <=4个大区的按钮
		local secondTypeAreaBtn = getglobal( "SecondTypeBigWorldBtn"..i );
		if secondTypeAreaBtn ~= nil then 
			secondTypeAreaBtn:Hide();
			local secondNewFlagTex = getglobal( secondTypeAreaBtn:GetName().."NewFlagTex" );
			secondNewFlagTex:Hide();
			local secUVTex	=  getglobal( secondTypeAreaBtn:GetName().."UVAnimationTex" );
			secUVTex:Hide();
			local secShuangXianFlagTex = getglobal( secondTypeAreaBtn:GetName().."ShuangXianFlagTex" );
			secShuangXianFlagTex:Hide();		
			secondTypeAreaBtn:DisChecked();
		end
	end
end

function UpdateDirServerBigArea()
	ClearDirServerBigArea();

	-- 构建大区运营商分类列表
	local t_netAreaBigWorldList = { [CLIENT_NET_AREA_DIAN_XIN] = {}, [CLIENT_NET_AREA_WANG_TONG] = {}, [CLIENT_NET_AREA_SHUANG_XIAN] = {}, };
	for i = 1, math.min( LoginMgr:getNumBigWorld(), MAX_AREA_NUM ) do
		local bigWorldInfo	= LoginMgr:getBigWorld( i - 1 );
		if bigWorldInfo:isDianXinArea() or bigWorldInfo:isShuangXianArea() then
			table.insert( t_netAreaBigWorldList[CLIENT_NET_AREA_DIAN_XIN], i );
		elseif bigWorldInfo:isWangTongArea() then
			table.insert( t_netAreaBigWorldList[CLIENT_NET_AREA_WANG_TONG], i );
		end
	end
	UpdateDianXinAreaBtns( t_netAreaBigWorldList[CLIENT_NET_AREA_DIAN_XIN] );
	UpdateWangTongAreaBtns( t_netAreaBigWorldList[CLIENT_NET_AREA_WANG_TONG] );
	UpdateRecommendAreas();
end

function UpdateDianXinAreaBtns( t_netAreaBigWorldList )
	for i = 1, math.min( MAX_DIAN_XIN_AREA_NUM, table.getn( t_netAreaBigWorldList ) ) do
		local btn;
		if table.getn( t_netAreaBigWorldList ) > 4 then 
			btn = getglobal( "BigWorldBtn"..i );
		else 
			btn = getglobal( "SecondTypeBigWorldBtn"..i );
		end

		if btn == nil then 
			break;
		end
		local nAreaIndex	= t_netAreaBigWorldList[i];
		local bigWorldInfo	= LoginMgr:getBigWorld( nAreaIndex - 1 );
		SetOneAreaBtn( bigWorldInfo, btn, nAreaIndex );
	end
end

function UpdateWangTongAreaBtns( t_netAreaBigWorldList )
	local WANT_TONG_BTN_INDEX_ADJUST = 10;
	for i = 1, math.min( MAX_DIAN_XIN_AREA_NUM, table.getn( t_netAreaBigWorldList ) ) do
		local btn;
		if table.getn( t_netAreaBigWorldList ) > 4 then 
			btn = getglobal( "BigWorldBtn"..( i + WANT_TONG_BTN_INDEX_ADJUST ) );
		else 
			btn = getglobal( "SecondTypeBigWorldBtn"..( i + WANT_TONG_BTN_INDEX_ADJUST ) );
		end

		if btn == nil then 
			break;
		end
		local nAreaIndex	= t_netAreaBigWorldList[i];
		local bigWorldInfo	= LoginMgr:getBigWorld( nAreaIndex - 1 );
		SetOneAreaBtn( bigWorldInfo, btn, nAreaIndex );
	end
end

function SetOneAreaBtn( bigWorldInfo, btn, nAreaIndex )
	btn:SetText( bigWorldInfo:getName() );
	btn:SetClientUserData( 0, nAreaIndex );
	btn:Show();
	if bigWorldInfo:getName() == SelectWorldInfo["bigworld"] then
		btn:Checked();
	end
	--[[
	if bigWorldInfo:isShuangXianArea() then
		local shuangXianFlagTex = getglobal( btn:GetName().."ShuangXianFlagTex" );
		shuangXianFlagTex:Show();
	end
	--]]
	if bigWorldInfo:isNewBigWorld() then
		local newFlagTex = getglobal( btn:GetName().."NewFlagTex" );
--		newFlagTex:Show();
		local uvTex	=  getglobal( btn:GetName().."UVAnimationTex" );
		uvTex:SetUVAnimation( 80, true );
		uvTex:Show();
	end
	local signTex = getglobal( btn:GetName().."SignTex" );
	if LoginMgr:getNumBigWorld() < 5 then 
		if string.find( bigWorldInfo:getName() , "时间城") ~= nil then
			signTex:SetTexture("uires\\TuPianLei\\XuanZeFuWuQi\\ShiJianChengDuoXian.tga" );
			signTex:Show();
		elseif string.find( bigWorldInfo:getName() , "天空") ~= nil then
			signTex:SetTexture("uires\\TuPianLei\\XuanZeFuWuQi\\TianKongShuangXian.tga" );
			signTex:Show();	
		else 
			signTex:Hide();
		end
	end
end

function ClearDirServerSelectAreaWorld()
	for i = 1, MAX_WORLD_NUM do
		local btn = getglobal( "WorldBtn"..i );
		btn:DisChecked();
		btn:Hide();

		local recommendFlagText = getglobal( btn:GetName().."RecommendFlagTex" );
		recommendFlagText:Hide();
		btn:DisChecked();
	end
end

function UpdateDirServerSelectAreaWorld( data )
	ClearDirServerSelectAreaWorld();
	local nSelectAreaIndex	= data["areaIndex"];
	local bigWorldInfo		= LoginMgr:getBigWorld( nSelectAreaIndex - 1 )
	for i = 1, math.min( bigWorldInfo:getNumWorld(), MAX_WORLD_NUM ) do
		local oneWorld	= bigWorldInfo:getIthWorld( i - 1 );
		local btn		= getglobal( "WorldBtn"..i );
		
		btn:SetText( oneWorld.name..GetServerBusyDesc( { ["state"] = oneWorld.busy } ) );
		btn:SetClientUserData( 0, nSelectAreaIndex );
		btn:SetClientUserData( 1, i );
	
		local tex 		= getglobal( btn:GetName().."BusyFlagTex" );
		local texPath 	= GetServerBusyTex( { ["state"] = oneWorld.busy } );
		tex:SetTexture(texPath["path"]);

		if oneWorld.name == SelectWorldInfo.world then
			btn:Checked();
			SelectWorldInfo.busy = oneWorld.busy;
		end

		if oneWorld.recommend then
			local recommendFlagText = getglobal( btn:GetName().."RecommendFlagTex" );
			recommendFlagText:Show();
		end
		btn:Show();
	end
	UpdateRecommendAreas();
end

function ClearRecommendAreas()
	for i = 1, MAX_RECOMMENT_AREA_NUM do
		local btn = getglobal( "DirServerFrameRecomendBigWorldBtn"..i );
		btn:Hide();
		btn:DisChecked();
	end
end

function UpdateRecommendAreas()
	ClearRecommendAreas();

	local nBtnIndex			= 1;
	--local nSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"];	
	for nBigWorldIndex = 1, LoginMgr:getNumBigWorld() do
		local bigWorldInfo		= LoginMgr:getBigWorld( math.max( nBigWorldIndex - 1, 0 ) );
		for i = 1, bigWorldInfo:getNumWorld() do
			if nBtnIndex > MAX_RECOMMENT_AREA_NUM then
				break;
			end

			local oneWorldInfo	= bigWorldInfo:getIthWorld( i - 1 );
			
			if oneWorldInfo.recommend then
				local btn		= getglobal( "DirServerFrameRecomendBigWorldBtn"..nBtnIndex );

				local BigWFont	= getglobal( btn:GetName().."Font" );
				local WFont 	= getglobal( btn:GetName().."WorldFont" );
				BigWFont:SetText( bigWorldInfo:getName() );
				WFont:SetText( oneWorldInfo.name );
				BigWFont:SetTextColor(220,220,100);
				WFont:SetTextColor(220,220,100);
				
				btn:SetClientUserData( 0, nBigWorldIndex );
				btn:SetClientUserData( 1, i );
				btn:Show();
				
				if oneWorldInfo.name == SelectWorldInfo["world"] then
					btn:Checked(); 
					BigWFont:SetTextColor(255,255,255);
					WFont:SetTextColor(255,255,255);
				end
				nBtnIndex = nBtnIndex + 1;
				local uvTex	=  getglobal( btn:GetName().."UVAnimationTex" );
				local newTex = getglobal( btn:GetName().."NewFlagTex" );
				if bigWorldInfo:isNewBigWorld() then 
					newTex:Show();
					uvTex:SetUVAnimation( 80, true );
					uvTex:Show();
				else 
					newTex:Hide();
					uvTex:Hide();
				end
			end
		end
	end
end

function DirServerFrame_OnShow()
	if CSFashionFrame:IsShown() then
		CSFashionFrame:Hide();
	end
	local t_hideIcons = { "PetUI", };
	util.HideIcons( t_hideIcons );
	--local tex = getglobal( this:GetName().."BackTex" );
	--tex:SetTexture("uires\\TuPianLei\\XuanFuWuQiDiTu.jpg");
	UpdateDirSelectLineChk()
end

function DirServerFrame_OnHide()
	--local tex = getglobal( this:GetName().."BackTex" );
	--tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
end

local t_DirServerFrameEvents = { ["GE_UPDATE_DIRINFO"]	= {},	["GE_APP_START"]		= {}, 
										["GE_KICKOFF_INFO"]		= {},	["GE_NEED_REGIST_CODE"]	= {},
									  };

t_DirServerFrameEvents["GE_APP_START"].func = 
function ()
	LoginMgr:requestDirInfo(g_szIP, g_nPort);
	UpdateDirSelectLineChk()
end

t_DirServerFrameEvents["GE_UPDATE_DIRINFO"].func = 
function ()
	--如果是属于刷新
	
	nLastRefreshTime = GameMgr:getTickTime();
	local nbigworld = LoginMgr:getNumBigWorld();
	if nbigworld <= 0 then
		return
	end
	if g_bIsRefurbish then	
		UpdateDirServerBigArea();
		for i = 1, math.min( nbigworld, MAX_AREA_NUM ) do
			local oneBigWorld	= LoginMgr:getBigWorld(i-1);
			local btn	= getglobal("BigWorldBtn"..i);
			if oneBigWorld:getName() == SelectWorldInfo.bigworld then
				UpdateDirServerSelectAreaWorld( { areaIndex = i } );
			end
		end
		g_bIsRefurbish = false;
		
	else
		InitSelectAreaAndWorld();
		UpdateDirServerBigArea();
		local nLastSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"] - 1;
		if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
			nLastSelectAreaIndex = 0;
		end
		
		local selectBigWorld		= LoginMgr:getBigWorld( nLastSelectAreaIndex );
		local nLastSelectWorldIndex = SelectWorldInfo["worldIndex"] - 1;
		

		if nLastSelectWorldIndex < 0 or nLastSelectWorldIndex >= selectBigWorld:getNumWorld() then
			nLastSelectWorldIndex = 0;
		end

		local selectWorld = selectBigWorld:getIthWorld( nLastSelectWorldIndex );
		
		SelectWorldInfo.bigworld = selectBigWorld:getName();			
		SaveSelectWorldInfo( selectWorld, nLastSelectAreaIndex + 1, nLastSelectWorldIndex + 1 );
		for i = 1, math.min( LoginMgr:getNumBigWorld(), MAX_AREA_NUM ) do
			local oneBigWorld	= LoginMgr:getBigWorld(i-1);
			local btn	= getglobal("BigWorldBtn"..i);
			if oneBigWorld:getName() == SelectWorldInfo.bigworld then
				
				UpdateDirServerSelectAreaWorld( { areaIndex = i } );
			end
		end
	end
	local nLastSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"] - 1;
	if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = 0;
	end
end

t_DirServerFrameEvents["GE_KICKOFF_INFO"].func = 
function ()
--	DirServerFrame:Show();
	DirSimpleServerFrame:Show();
	DirFrame_OnClickRefurbishBtn();
	BirthPlaceFrame:Hide();
	CharacterSelectFrame:Hide();
	SelectRoleOnKickOff();
	GameBackground:show();
	GameBackground:releaseEntity(0);
	GameBackground:playCameraAnim(99100, 0);
	GameBackground:setViewFlag( VIEW_POINT_LOGIN );
	if string.find( LoginMgr:getOffLineText(),"来自ip地址为",1,true ) then
		DirMessageBoxFrameDescFont:SetText( LoginMgr:getOffLineText() );
		DirMessageBoxFrame:Show();
	else
		Login_MessageBoxOK( { ["text"] = LoginMgr:getOffLineText(), ["opDesc"] = "" } );
	end
end

t_DirServerFrameEvents["GE_NEED_REGIST_CODE"].func = 
function ()
	DirServer_InputRegisterCodeFrame:Show();
end

function DirServerFrame_OnLoad()
	for event, _ in pairs( t_DirServerFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_DirServerFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_DirServerFrameEvents, t_DirServerFrameEvents );

	DirServerFrameQuickStartBtn:Disable()
end

function DirServerFrame_OnEvent()
	t_DirServerFrameEvents[arg1].func();
end

--保存当前被选中World的信息
function SaveSelectWorldInfo(world, nBigWorldIndex, nWorldIndex )
	if world ~= nil then
		SelectWorldInfo.world	= world.name;
		SelectWorldInfo.worldId = world.worldid;
		SelectWorldInfo.busy	= world.busy;
		SelectWorldInfo["bigWorldIndex"]	= nBigWorldIndex;
		SelectWorldInfo["worldIndex"]		= nWorldIndex;   
		
		local BigWorld = LoginMgr:getBigWorld( nBigWorldIndex - 1 );   --ELMER
		local szText 		= BigWorld:getName().." "..world.name;
		local SvrFont 		= getglobal("DirServerFrameSelectWorldBackBtnSeverFont");
			SvrFont:SetText(szText);

		local SvrBusyFont	= getglobal("DirServerFrameSelectWorldBackBtnSeverBusyFont");
		SetFontText( SvrBusyFont:GetName(),GetServerBusyDesc( { ["state"] = world.busy } ), GetServerBusyColor( { ["state"] = world.busy } ));
		SvrBusyFont:SetPoint( "topleft", SvrFont:GetName(), "topleft", SvrFont:GetTextExtentWidth( SvrFont:GetText() ) / GetScreenScaleY() + 1, 0 );

	end
end

--刷新
local bRefurbish = false;
local nLastTime = 0;
function DirFrame_OnClickRefurbishBtn()
	--RefurbishBtn:Disable();
	if bRefurbish then return end
	g_bIsRefurbish=true;
	bRefurbish = true;
	nLastTime = os.clock();
	LoginMgr:dirRefurbish(g_szIP, g_nPort);
end
--退出
function DirFrame_OnClickQuitBtn()
	DirSimpleServerFrame:Show();
	DirServerFrame:Hide();
end
function DirServerFrame_OnUpdate()
	if bRefurbish then
		if os.difftime ( os.clock(), nLastTime) >= 7 then
			bRefurbish = false;
		end
	end
	local verFont = getglobal("DirServerFrameSelectWorldBackBtnVersionFont");
	verFont:SetText( GameMgr:GetVersionName() );
	if nLastRefreshTime ~= 0 and GameMgr:getTickTime() - nLastRefreshTime > 50*1000 then
		DirFrame_OnClickRefurbishBtn();
		nLastRefreshTime = 0;
	end
end

function DirServerFrame_OnEnterPress()
	if SelectWorldInfo.worldid ~= 0 then
		DirFrame_OnClickSelectWorldBtn();
	end
end

function DirFrame_OnClickQuitGameBtn()
	GameMgr:QuitGame();
end

local t_ShowTime = 0;
-- 激活码框显示
function DirServer_InputRegisterCodeFrame_OnShow()
	local edit = getglobal( "DirServer_InputRegisterCodeFrame_InputEdit" );
	edit:Clear();
	--SetFocusFrame( edit:GetName() );
	--开始计时
	t_ShowTime = os.clock();
	DirServer_InputRegisterCodeFrame:SetPoint("center","$parent","center",0,0);
	LoginSelectServerBtnFont:SetText("当前服务器："..SelectWorldInfo.world );
end

-- 立即激活
function DirServer_InputRegisterCodeFrame_RegisterBtn_OnClick()
	local edit = getglobal( "DirServer_InputRegisterCodeFrame_InputEdit" );
	LoginSelectServerBtnFont:SetText("正在激活中……");
	LoginMgr:requestActiveAccount( SelectWorldInfo.bigWorldIndex - 1, SelectWorldInfo.worldIndex - 1, edit:GetText() );
	DirServer_InputRegisterCodeFrame:Hide();
end

-- 领取激活码
function DirServer_InputRegisterCodeFrame_GetRegisterCodeBtn_OnClick()
	GameMgr:openWebSite( "http://www.ljy0.com/active.php#getcode" );
	DirServer_InputRegisterCodeFrame:Hide();
end

function DirServer_InputRegisterCodeFrame_EditBackBtn_OnClick()
	SetFocusFrame( "DirServer_InputRegisterCodeFrame_InputEdit" );
end

function DirServer_InputRegisterCodeFrame_OnUpdate()
	if os.clock() - t_ShowTime >= 3*60 then
		DirServer_InputRegisterCodeFrame:Hide();
	end
end

function DirMessageBoxFrame_OnShow()
	DirServerFrameZheZhaoBtn:Show();
	DirServerFrameZheZhaoBtnTex:SetColor(0,0,0);
	DirServerFrameZheZhaoBtnTex:SetBlendAlpha( 0.5 );
	DirConfirmQuiGameFrame:Hide();
end

function DirConfirmQuiGameFrame_OnShow()
	DirMessageBoxFrame:Hide();
end

function DirMessageBoxFrame_OnHide()
	DirServerFrameZheZhaoBtn:Hide();
	DirSimpleServerFrameZheZhaoBtn:Hide();
end

function DirFrameQuickStartBtn_OnClick()
	local t_hideIcons = {	"CharacterSelectFrameTopZheZhaoTex","CharacterSelectFrameBottomZheZhaoTex", "CharacterSelectFrameLeftZheZhaoChiLunTex",
							"CharacterSelectFrameRightZheZhaoChiLunTex", "SelectFrame" };
	util.HideIcons( t_hideIcons );
	LoginMgr:requestEnterLoginWithoutUserName();
	local nLastSelectAreaIndex	= LoginMgr:getLastSelectArea();
	local nLastSelectWorldIndex = LoginMgr:getLastSelectWorld();
	local nAreaIndex, nWorldIndex = GetFirstRecommendWorld();

	if nLastSelectAreaIndex <= 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = nAreaIndex;
	end

	local oneBigWorld = LoginMgr:getBigWorld( nLastSelectAreaIndex );
	if nLastSelectWorldIndex <= 0 or nLastSelectWorldIndex >= oneBigWorld:getNumWorld() then
		nLastSelectWorldIndex = nWorldIndex;
	end
	local world = oneBigWorld:getIthWorld( nLastSelectWorldIndex );
	SelectWorldInfo.bigworld = oneBigWorld:getName();
	SaveSelectWorldInfo( world, nLastSelectAreaIndex + 1, nLastSelectWorldIndex + 1 );
end

function RefreshIthWorldInfo()
	InitSelectAreaAndWorld();
	UpdateDirServerBigArea();
	local nLastSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"] - 1;
	if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = 0;
	end

	local selectBigWorld		= LoginMgr:getBigWorld( nLastSelectAreaIndex );
	local nLastSelectWorldIndex = SelectWorldInfo["worldIndex"] - 1;

	if nLastSelectWorldIndex < 0 or nLastSelectWorldIndex >= selectBigWorld:getNumWorld() then
		nLastSelectWorldIndex = 0;
	end

	local selectWorld = selectBigWorld:getIthWorld( nLastSelectWorldIndex );
	SelectWorldInfo.bigworld = selectBigWorld:getName();			
	SaveSelectWorldInfo( selectWorld, nLastSelectAreaIndex + 1, nLastSelectWorldIndex + 1 );
end

function DirServerFrameLineChk_OnClick()
	LoginMgr:SetIpMode( this:GetClientID() )
	UpdateDirSelectLineChk()
end

-----------------------------------------------简易登录界面----------------------------------------
local MAX_SIMPLE_RECOMMENT_AREA_NUM	= 2;
local nSimRefreshTime = 0;
local bPlayMusic = true;
local nMaskFlag = 0;

function DirSimpleServerFrame_OnLoad()
	this:RegisterEvent( "GE_UPDATE_DIRINFO" );
	this:RegisterEvent( "GE_APP_START" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:setUpdateTime(0.1);
	SetPlayBGMflag( true );
	DirSimpleServerFrameBackTex:SetColor( 0, 0, 0 );
end

function DirSimpleServerFrame_OnEvent()
	if(arg1 == "GE_UPDATE_DIRINFO") then
		local nbigworld = LoginMgr:getNumBigWorld();
		if nbigworld <= 0 then
			return
		end
		nSimRefreshTime = GameMgr:getTickTime();
		InitSimpleSelectWorld();
		UpdateSimpleRecommendAreas();
	end
	if( arg1 == "GE_APP_START" ) then
		LoginMgr:requestDirInfo(g_szIP, g_nPort);
	elseif arg1 == "GE_ENTER_PLAYERLOGIN" then
		SetPlayBGMflag( true );
	end
end
function ClearSimpleRecommendAreas()
	for i = 1, MAX_SIMPLE_RECOMMENT_AREA_NUM do
		local btn = getglobal( "DirSimpleServerFrameRecomendWorldBtn"..i );
		btn:Hide();
		btn:DisChecked();
	end
end
local SelectWorldName = "";
function InitSimpleSelectWorld()
	local nLastSelectAreaIndex 	= LoginMgr:getLastSelectArea();
	local nLastSelectWorldIndex = LoginMgr:getLastSelectWorld();
	local nAreaIndex, nWorldIndex = GetFirstRecommendWorld();

	if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = nAreaIndex;
	end

	local oneBigWorld 	= LoginMgr:getBigWorld( nLastSelectAreaIndex );
	if nLastSelectWorldIndex < 0 or nLastSelectWorldIndex >= oneBigWorld:getNumWorld() then
		nLastSelectWorldIndex = nWorldIndex;
	end
	local world	= oneBigWorld:getIthWorld( nLastSelectWorldIndex );	
	SelectWorldName	 	= world.name;
	local szText 		= oneBigWorld:getName().." "..world.name;	
	local SvrFont 		= getglobal("DirSimpleServerFrameSelectWorldBackBtnSeverFont");
	SvrFont:SetText(szText);

	local SvrBusyFont	= getglobal("DirSimpleServerFrameSelectWorldBackBtnSeverBusyFont");
	SetFontText( SvrBusyFont:GetName(),GetServerBusyDesc( { ["state"] = world.busy } ), GetServerBusyColor( { ["state"] = world.busy } ));
	SvrBusyFont:SetPoint( "topleft", SvrFont:GetName(), "topleft", SvrFont:GetTextExtentWidth( SvrFont:GetText() ) / GetScreenScaleY() + 1, 0 );
	SaveSelectWorldInfo( world, nLastSelectAreaIndex + 1, nLastSelectWorldIndex + 1 );
end

function UpdateSimpleRecommendAreas()
	ClearSimpleRecommendAreas();
	local nBtnIndex			= 1;
	for nBigWorldIndex = 1, LoginMgr:getNumBigWorld() do
		local bigWorldInfo		= LoginMgr:getBigWorld( math.max( nBigWorldIndex - 1, 0 ) );
		for i = 1, bigWorldInfo:getNumWorld() do
			if nBtnIndex > MAX_SIMPLE_RECOMMENT_AREA_NUM then
				break;
			end

			local oneWorldInfo	= bigWorldInfo:getIthWorld( i - 1 );
			
			if oneWorldInfo.recommend then
				local btn		= getglobal( "DirSimpleServerFrameRecomendWorldBtn"..nBtnIndex );

				local BigWFont	= getglobal( btn:GetName().."WorldFont" );
				local WFont 	= getglobal( btn:GetName().."ServerFont" );
				BigWFont:SetText( bigWorldInfo:getName() );
				WFont:SetText( oneWorldInfo.name );
				BigWFont:SetTextColor(220,220,60);
				WFont:SetTextColor(255,255,120);
				
				btn:SetClientUserData( 0, nBigWorldIndex );
				btn:SetClientUserData( 1, i );
				btn:Show();
				local rich = getglobal("DirSimpleServerFrameTipRich"..nBtnIndex);
				rich:SetText("适合电信、网通、校园网、移动、铁通、 长城宽带等用户入驻",120,120,50 );
				if oneWorldInfo.name == SelectWorldName then
					btn:Checked(); 
					BigWFont:SetTextColor(255,255,150);
					WFont:SetTextColor(255,255,190);
				end
				nBtnIndex = nBtnIndex + 1;
			--	local uvTex	=  getglobal( btn:GetName().."UVAnimationTex" );
				local newTex = getglobal( btn:GetName().."NewFlagTex" );
				if bigWorldInfo:isNewBigWorld() then 
					newTex:Show();
			--		uvTex:SetUVAnimation( 80, true );
			--		uvTex:Show();
				else 
					newTex:Hide();
			--		uvTex:Hide();
				end
			end
		end
	end
end

function DirSimpleServerFrame_OnShow()
	if CSFashionFrame:IsShown() then
		CSFashionFrame:Hide();
	end
	InitSimpleSelectWorld();
	UpdateSimpleRecommendAreas();
end

function DirSimpleServerFrame_OnHide()
	nSimRefreshTime = 0
end

function DirSimpleServerFrame_OnUpdate()
	if bRefurbish then
		if os.difftime ( os.clock(), nLastTime) >= 7 then
			bRefurbish = false;
		end
	end
	if nSimRefreshTime ~= 0 and GameMgr:getTickTime() - nSimRefreshTime > 50*1000 then
		nSimRefreshTime = 0;
		DirFrame_OnClickRefurbishBtn();
	end

	--登录界面背景处理
	if NeedPlayBGM() then
		if not GameMgr:isVideoPlaying() and GameBackground:isShown() then
			--处理撕裂问题
			if nMaskFlag == 0 then
				nMaskFlag = os.clock();
			else
				local alpha = 1 - string.format( "%.2f", os.clock() - nMaskFlag );
				DirSimpleServerFrameBackTex:SetBlendAlpha( alpha );
				if alpha <= 0 then
					DirSimpleServerFrameBackTex:SetBlendAlpha( 0 );
					GameBackground:playBGM( 0 );
					SetPlayBGMflag( false );
				end
			end
		end
	end
end
function SimpleRecommendWorldBtn_OnClick()
	DisCheckAllBtn( "DirSimpleServerFrameRecomendWorldBtn", MAX_SIMPLE_RECOMMENT_AREA_NUM );
	this:Checked();
	RecoveryAllSimpleFontColor("DirSimpleServerFrameRecomendWorldBtn", MAX_SIMPLE_RECOMMENT_AREA_NUM );
	
	local nSelectAreaIndex		= this:GetClientUserData( 0 );
	local oneBigWorld			= LoginMgr:getBigWorld( nSelectAreaIndex - 1 )

	local nSelectWorldIndex		= this:GetClientUserData( 1 ) - 1;
	local world					= oneBigWorld:getIthWorld( nSelectWorldIndex );
	
	SaveSelectWorldInfo( world, nSelectAreaIndex, nSelectWorldIndex + 1 );
	UpdateDirServerSelectAreaWorld( { areaIndex = nSelectAreaIndex } );
	
	local BigWFont	= getglobal( this:GetName().."WorldFont" );
	local WFont 	= getglobal( this:GetName().."ServerFont" );
	BigWFont:SetTextColor(255,255,150);
	WFont:SetTextColor(255,255,190);
	
	SelectWorldName	 	= world.name;
	local szText 		= oneBigWorld:getName().." "..world.name;	
	local SvrFont 		= getglobal("DirSimpleServerFrameSelectWorldBackBtnSeverFont");
	SvrFont:SetText(szText);
	local SvrBusyFont	= getglobal("DirSimpleServerFrameSelectWorldBackBtnSeverBusyFont");
	SetFontText( SvrBusyFont:GetName(),GetServerBusyDesc( { ["state"] = world.busy } ), GetServerBusyColor( { ["state"] = world.busy } ));
	SvrBusyFont:SetPoint( "topleft", SvrFont:GetName(), "topleft", SvrFont:GetTextExtentWidth( SvrFont:GetText() ) / GetScreenScaleY() + 1, 0 );
end
function RecoveryAllSimpleFontColor( btnName, MaxNum )
	for i=1, MaxNum do  
		local btn = getglobal(btnName..i);
		local BigWFont	= getglobal( btn:GetName().."WorldFont" );
		local WFont 	= getglobal( btn:GetName().."ServerFont" );
		BigWFont:SetTextColor(220,220,60);
		WFont:SetTextColor(255,255,120);
	end
end
function SimpleRecommendWorldBtn_OnDoubleClick()
	SimpleRecommendWorldBtn_OnClick();
	DirSimpleFrameSelectBtn_OnClick();
end
function DirSimpleFrameSelectBtn_OnClick()
	if SelectWorldInfo.busy == CLIENT_DIR_NOT_WORK and not GameMgr:IsTestMode() then
		return;
	end

	if DirConfirmQuiGameFrame:IsShown() then
		return;
	end

--	RefreshIthWorldInfo();

	DirMessageBoxFrameDescFont:SetText("本服务器含有PvP内容，未成年人请慎入");
	DirMessageBoxFrame:Show();

	DirSimpleServerFrameZheZhaoBtn:Show();
	DirSimpleServerFrameZheZhaoBtnTex:SetColor(0,0,0);
	DirSimpleServerFrameZheZhaoBtnTex:SetBlendAlpha( 0.5 );
	DirConfirmQuiGameFrame:Hide();

	local nLastSelectAreaIndex	= SelectWorldInfo["bigWorldIndex"] - 1;
	if nLastSelectAreaIndex < 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = 0;
	end
	local selectBigWorld		= LoginMgr:getBigWorld( nLastSelectAreaIndex );
	local nLastSelectWorldIndex = SelectWorldInfo["worldIndex"] - 1;
	if nLastSelectWorldIndex < 0 or nLastSelectWorldIndex >= selectBigWorld:getNumWorld() then
		nLastSelectWorldIndex = 0;
	end
	local selectWorld = selectBigWorld:getIthWorld( nLastSelectWorldIndex );
	LoginMgr:getOptimalBakIp( selectWorld );
end
function SimpleRecommendWorldBtn_OnEnter()
	if not this:IsChecked() then 
		local BigWFont	= getglobal( this:GetName().."WorldFont" );
		local WFont 	= getglobal( this:GetName().."ServerFont" );
		BigWFont:SetTextColor(255,255,120);
		WFont:SetTextColor(255,255,255);
	end
end
function SimpleRecommendWorldBtn_OnLeave()
	if not this:IsChecked() then 
		local BigWFont	= getglobal( this:GetName().."WorldFont" );
		local WFont 	= getglobal( this:GetName().."ServerFont" );
		BigWFont:SetTextColor(220,220,60);
		WFont:SetTextColor(255,255,120);
	end
end
function SimpleSelectOtherWorldBtn_OnClick()
	DirSimpleServerFrame:Hide();
	DirServerFrame:Show();
end

function NeedPlayBGM()
	return bPlayMusic and getGameState() ~= CLIENT_STATE_INWORLD;
end

function SetPlayBGMflag( state )
	bPlayMusic = state;
	nMaskFlag = state and 0 or nMaskFlag;
	DirSimpleServerFrameBackTex:SetBlendAlpha( state and 1 or 0 );
end