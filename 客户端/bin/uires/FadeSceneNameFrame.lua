local g_nPreShowMapID = 0;

-- 定义资源的静态读取表
-- 区域字体静态读取表
local t_areaNameFontList =	{	
								[MAP_AREA_ASIA]			= { x = 0,   y = 652, width = 116, height = 60 },
								[MAP_AREA_EUROPE]		= { x = 234, y = 652, width = 116, height = 60 },
								[MAP_AREA_NORTHAMERICA] = { x = 869, y = 652, width = 116, height = 60 },
								[MAP_AREA_SOUTHAMERICA] = { x = 752, y = 652, width = 116, height = 60 },
								[MAP_AREA_AFRICA]		= { x = 468, y = 652, width = 116, height = 60 },
								[MAP_AREA_BIRTH]		= { x = 585, y = 652, width = 116, height = 60 },
								[MAP_AREA_CITY]			= { x = 117, y = 652, width = 116, height = 60 },
								[MAP_AREA_COPY]			= { x = 351, y = 652, width = 116, height = 60 },
							};

-- 地图字体静态读取表
local t_mapNameFontList =	{	
								[1000]	= { x = 480,	y = 713,	width = 95,		height = 39 },
								[2000]	= { x = 96,		y = 713,	width = 95,		height = 39 },
								[2001]	= { x = 96,		y = 713,	width = 95,		height = 39 },
								[2002]	= { x = 96,		y = 713,	width = 95,		height = 39 },
								[2003]	= { x = 96,		y = 713,	width = 95,		height = 39 },
								[2100]	= { x = 576,	y = 713,	width = 132,	height = 39 },
								[2101]	= { x = 576,	y = 713,	width = 132,	height = 39 },
								[2102]	= { x = 576,	y = 713,	width = 132,	height = 39 },
								[2103]	= { x = 576,	y = 713,	width = 132,	height = 39 },
								[2200]	= { x = 709,	y = 713,	width = 132,	height = 39 },
								[2300]	= { x = 842,	y = 713,	width = 132,	height = 39 },
								[12300]	= { x = 842,	y = 713,	width = 132,	height = 39 },
								[2400]	= { x = 133,	y = 753,	width = 132,	height = 39 },
								[2500]	= { x = 0,		y = 753,	width = 132,	height = 39 },
								[2501]	= { x = 591,	y = 753,	width = 132,	height = 39 },
								[10100]	= { x = 192,	y = 713,	width = 95,		height = 39 },
								[10200]	= { x = 857,	y = 753,	width = 68,		height = 39 },
								[21100]	= { x = 384,	y = 713,	width = 95,		height = 39 },
								[50000]	= { x = 399,	y = 753,	width = 95,		height = 39 },
								[50200]	= { x = 495,	y = 753,	width = 95,		height = 39 },
								[50300]	= { x = 266,	y = 753,	width = 132,	height = 39 },
								[2501]	= { x = 591,	y = 753,	width = 132,	height = 39	},
                                [2400]	= { x = 724,	y = 753,	width = 132,	height = 39	},
								[4400]	= { x = 857,	y = 753,	width = 132,	height = 39	},
								[10400]	= { x = 288,	y = 713,	width = 95,		height = 39	},
								[3300]	= { x = 871,	y = 793 ,	width = 132,	height = 39	},
								[3400]	= { x = 738,	y = 833 ,	width = 132 ,	height = 39	},
								[3500]	= { x = 871,	y = 833 ,	width = 132 ,	height = 39	},
							};
local t_textureList =	{ "FadeSceneNameFrame_SceneNameBackTex", "FadeSceneNameFrame_AreaNameTex", "FadeSceneNameFrame_CircleTex", 
							"FadeSceneNameFrame_ShortSceneNameTex", "FadeSceneNameFrame_LongSceneNameTex"
						};

local t_FadeSceneNameFrameEvents = 
{	-- 进入副本
	["GE_ENTER_MAP"] = {},	["GE_ENTER_PLAYERLOGIN"] = {},		
};

t_FadeSceneNameFrameEvents["GE_ENTER_MAP"].func = 
function ()
	-- 设置地图名字
	local frame = getglobal( "FadeSceneNameFrame" );
	local nCurEnterMapID = MapShower:getRealMapId();
	if nCurEnterMapID ~= g_nPreShowMapID then
		g_nPreShowMapID = nCurEnterMapID;
		frame:Show();
		-- 查看是否有要忽略的交易请求
		RejectAllExchangeInvite( CHG_MAP_REJECT );
		RejectAllRideInvite();
		-- 若处于交易之中，则取消交易
		if TradeFrame:IsShown() then
			GameExchange:cancelExchange( CHG_MAP_REJECT );
			ClearTradeFrame();
		end
	end
end

t_FadeSceneNameFrameEvents["GE_ENTER_PLAYERLOGIN"].func = 
function ()
	t_FadeSceneNameFrameEvents["GE_ENTER_MAP"].func();
end

function FadeSceneNameFrame_OnLoad()
	this:setUpdateTime( 0 );

	for event, _ in pairs( t_FadeSceneNameFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_FadeSceneNameFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_FadeSceneNameFrameEvents, t_FadeSceneNameFrameEvents );

	this:setUpdateTime( 0 );
end

function FadeSceneNameFrame_OnEvent()
	if CameraCoverFrame:IsShown() then
		return;
	end
	t_FadeSceneNameFrameEvents[arg1].func();
end

function FadeSceneNameFrame_OnShow()
	local mapInfo = MapShower:getMapInfoByID( MapShower:getRealMapId() );

	if t_areaNameFontList[mapInfo.m_nMapArea] == nil then
		this:Hide();
		return;
	end

	-- 设置区域名字
	if mapInfo.m_nMapID >= MIN_COPY_ID then 
		-- 设置副本区域名字
		FadeSceneNameFrame_AreaNameTex:SetTexUV( t_areaNameFontList[MAP_AREA_COPY].x, t_areaNameFontList[MAP_AREA_COPY].y,
												t_areaNameFontList[MAP_AREA_COPY].width, t_areaNameFontList[MAP_AREA_COPY].height );
	else 
		-- 设置非副本区域名字
		FadeSceneNameFrame_AreaNameTex:SetTexUV( t_areaNameFontList[mapInfo.m_nMapArea].x,		t_areaNameFontList[mapInfo.m_nMapArea].y,
												t_areaNameFontList[mapInfo.m_nMapArea].width,	t_areaNameFontList[mapInfo.m_nMapArea].height );
	end
	
	if t_mapNameFontList[mapInfo.m_nMapID] == nil then
		this:Hide();
		return;
	end

	if t_mapNameFontList[mapInfo.m_nMapID].width == 95 then
		FadeSceneNameFrame_ShortSceneNameTex:SetTexUV( t_mapNameFontList[mapInfo.m_nMapID].x,	t_mapNameFontList[mapInfo.m_nMapID].y,
												t_mapNameFontList[mapInfo.m_nMapID].width,		
												t_mapNameFontList[mapInfo.m_nMapID].height );
		FadeSceneNameFrame_ShortSceneNameTex:Show();
		FadeSceneNameFrame_LongSceneNameTex:Hide();

	elseif t_mapNameFontList[mapInfo.m_nMapID].width == 132 then
		FadeSceneNameFrame_LongSceneNameTex:SetTexUV( t_mapNameFontList[mapInfo.m_nMapID].x,	t_mapNameFontList[mapInfo.m_nMapID].y,
												t_mapNameFontList[mapInfo.m_nMapID].width,		
												t_mapNameFontList[mapInfo.m_nMapID].height );
		FadeSceneNameFrame_ShortSceneNameTex:Hide();
		FadeSceneNameFrame_LongSceneNameTex:Show();
	end
	
	FadeSceneNameFrame_SceneNameBackTex:Show();
	FadeSceneNameFrame_AreaNameTex:Show();
	FadeSceneNameFrame_CircleTex:Show();
	
	for _, szTextureName in ipairs( t_textureList ) do
		local tex = getglobal( szTextureName );
		if tex:IsShown() then
			util.UIFrameToFadeIn(	szTextureName,	0, 1,	5000, 100 );
			util.UIFrameToFadeKeep( szTextureName,	1, 2000 );
			util.UIFrameToFadeOut(	szTextureName,	1, 0,	5000, 100 );
		end
	end
end

function FadeSceneNameFrame_OnUpdate()
	for _, szTextureName in ipairs( t_textureList ) do
		local tex = getglobal( szTextureName );
		if tex:IsShown() then
			return;
		end
	end
	this:Hide();
end

function FadeTaskNameFrame_OnLoad()
end

function FadeTaskNameFrame_OnShow()
	-- 任务章节渐隐
	util.UIFrameToFadeIn( "FadeTaskNameFrame_LongSceneNameTex",		0, 1,	5000, 100 );
	util.UIFrameToFadeKeep( "FadeTaskNameFrame_LongSceneNameTex",	1, 1000 );
	util.UIFrameToFadeOut( "FadeTaskNameFrame_LongSceneNameTex",	1, 0,	4000, 100 );

	-- 背景花边渐隐
	util.UIFrameToFadeIn( "FadeTaskNameFrame_SceneNameBackTex",		0, 1,	5000, 100 );
	util.UIFrameToFadeKeep( "FadeTaskNameFrame_SceneNameBackTex",	1, 800 );
	util.UIFrameToFadeOut( "FadeTaskNameFrame_SceneNameBackTex",	1, 0,	4000, 100 );
end

function FadeTaskNameFrame_OnUpdate()
	local tex = getglobal( this:GetName() .. "_SceneNameBackTex" );
	if tex:IsShown() then
		return;
	end

	tex = getglobal( this:GetName() .. "_LongSceneNameTex" );
	if tex:IsShown() then
		return;
	end

	this:Hide();
end

XINSHOU_HUO_YAN_DAO1_TYPE	= 0;
XINSHOU_HUO_YAN_DAO2_TYPE	= 1;
XINSHOU_DA_PAO_TYPE			= 2;
XINSHOU_SHOU_LEI_TYPE		= 3;
XINSHOU_JI_JIA_TYPE			= 4;

local t_showXinShouFade = 
{
-- 竞技场	火焰刀	GAMEWIZARD_FIRST_SAWSWORD		1420000
-- 500
[XINSHOU_HUO_YAN_DAO1_TYPE] = { wizardOp = GAMEWIZARD_FIRST_SWORD_PWORLD_ANILM,	curTaskId = 1054, endTaskId = 1062,	
			path = "uires\\ui\\LinShiDaoJuTiShiTu\\LinShiDaoJu  HuoYanDao.jpg" },
-- 天空岛	火焰刀	1420000
-- 502
[XINSHOU_HUO_YAN_DAO2_TYPE] = { wizardOp = GAMEWIZARD_FIRST_SWORD_PWORLD_ANILM,	curTaskId = 1062, endTaskId = 1062,
			path = "uires\\ui\\LinShiDaoJuTiShiTu\\LinShiDaoJu  HuoYanDao.jpg" },

-- 盘上道	大炮	GAMEWIZARD_FIRST_SAWPAOTAI		1460002
-- 101
[XINSHOU_DA_PAO_TYPE] = { wizardOp = GAMEWIZARD_FIRST_PAOTAI_PWORLD_ANILM,	curTaskId = 0,		path = "uires\\ui\\LinShiDaoJuTiShiTu\\LinShiDaoJu  Pao.jpg" },

-- 战鼓村	手雷	GAMEWIZARD_FIRST_SAW_SHOULEI	1400002
-- 104
[XINSHOU_SHOU_LEI_TYPE] = { wizardOp = GAMEWIZARD_FIRST_SHOULEI_PWORLD_ANILM, curTaskId = 0,		path = "uires\\ui\\LinShiDaoJuTiShiTu\\LinShiDaoJu ShouLei.jpg" },

-- 天空回廊	机甲兽	GAMEWIZARD_FIRST_SAW_JIJIASHOU	1460001
-- 503
[XINSHOU_JI_JIA_TYPE] = { wizardOp = GAMEWIZARD_FIRST_JIJIA_PWORLD_ANILM,	curTaskId = 0,		path = "uires\\ui\\LinShiDaoJuTiShiTu\\LinShiDaoJu  JiXieShou.jpg" },
};

local t_pworldSceneEvents = 
{	-- 进入副本
	["GE_ENTER_PWORLD"] = {},			
};

t_pworldSceneEvents["GE_ENTER_PWORLD"].func = 
function ()
	local nRealMapId	= MapShower:getRealMapId();
	local nPworldId		= math.floor( nRealMapId / 100 );
	
	if nPworldId == 500 and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_ENTER_FUBEN_OPTION ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_ENTER_FUBEN_OPTION );
	end
	-- 此为进入飞行副本时候不闪烁NPC对话面板选项使用的
	if nPworldId == 502 and not GameWizard:isDo( GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY );
	end
end

function PworldSceneFrame_OnLoad()
	for event, _ in pairs( t_pworldSceneEvents ) do
		this:RegisterEvent( event );
	end

	t_pworldSceneEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_pworldSceneEvents, t_pworldSceneEvents );

	this:setUpdateTime( 0 );
end

function PworldSceneFrame_OnEvent()
	t_pworldSceneEvents[arg1].func();
end

local STOP_PLAYING = 0;
local KEEP_PLAYING = 1;
local t_playSizeAnimControl = { MAX_PLAY_TIME = 2, startTime = 0, playintState = STOP_PLAYING,  MAX_WIDTH = 600, MAX_HEIGHT = 341 };
t_playSizeAnimControl["begin"] = 
function ( self )
	self["startTime"] = os.clock();
	local frame = getglobal( "PworldSceneFrame" );
	--frame:SetSize( 0.1 * self["MAX_WIDTH"], 0.1 * self["MAX_HEIGHT"] );
	self["playintState"] = KEEP_PLAYING;
	local tex = getglobal( frame:GetName().."BackTex" );
	--tex:SetSize( 0.1 * self["MAX_WIDTH"], 0.1 * self["MAX_HEIGHT"] );
	tex:SetBlendAlpha( 0 );
	
	local t_chgSizeIcons = 
	{
	[frame:GetName()]				=	{	width	= 0.1 * self["MAX_WIDTH"],	
											height	= 0.1 * self["MAX_HEIGHT"] 
										},

	[frame:GetName().."BackTex"]	=	{	width	= 0.1 * self["MAX_WIDTH"],	
											height	= 0.1 * self["MAX_HEIGHT"] 
										},
	[frame:GetName().."_CloseBtn"]	=	{	width	= 0.1 * 25,	
											height	= 0.1 * 25 
										},
	};	
	util.SetIconsSize( t_chgSizeIcons );
	
	local btn = getglobal( frame:GetName().."_CloseBtn" );
	btn:SetOverlayRegion( 0, 0, 0.1 * 25, 0.1 * 25 );
	btn:SetNormalTextureSize( 0.1 * 25, 0.1 * 25 );
end

t_playSizeAnimControl["endPlay"] = 
function ( self )
	self["startTime"]		= 0;
	self["playintState"]	= STOP_PLAYING;
end

t_playSizeAnimControl["isPlayingAnim"] = 
function ( self )
	return self["playintState"] == KEEP_PLAYING;
end

t_playSizeAnimControl["update"] = 
function ( self )
	if not self:isPlayingAnim() then
		return;
	end

	local fCurTime = os.clock();
	if fCurTime - self["startTime"] >= self["MAX_PLAY_TIME"] then
		self:endPlay();
		return;
	end

	local frame		= getglobal( "PworldSceneFrame" );
	local fPassTime	= ( fCurTime - self["startTime"] );
	local fScale	= ( 0.1 + 0.9 * fPassTime / self["MAX_PLAY_TIME"] );
	--frame:SetSize( fScale * self["MAX_WIDTH"], fScale * self["MAX_HEIGHT"] );
	local tex = getglobal( frame:GetName().."BackTex" );
	--tex:SetSize( fScale * self["MAX_WIDTH"], fScale * self["MAX_HEIGHT"] );
	tex:SetBlendAlpha( fPassTime / self["MAX_PLAY_TIME"] );

	local t_chgSizeIcons = 
	{
	[frame:GetName()]				=	{	width	= fScale * self["MAX_WIDTH"],	
											height	= fScale * self["MAX_HEIGHT"] 
										},

	[frame:GetName().."BackTex"]	=	{	width	= fScale * self["MAX_WIDTH"],	
											height	= fScale * self["MAX_HEIGHT"] 
										},

	[frame:GetName().."_CloseBtn"]	=	{	width	= fScale * 25,	
											height	= fScale * 25 
										},
	};	
	util.SetIconsSize( t_chgSizeIcons );

	local btn = getglobal( frame:GetName().."_CloseBtn" );
	btn:SetOverlayRegion( 0, 0, fScale * 25, fScale * 25 );
	btn:SetNormalTextureSize( fScale * 25, fScale * 25 );
end

-- 淡入淡出的显示效果
function PworldSceneFrame_OnShow()
	local nRealMapId			= this:GetClientUserData( 0 );
	local nPworldId				= math.floor( nRealMapId / 100 );
	--local t_xinShouPworldInfo	= t_showXinShouFade[nPworldId];
	local nXinShouType			= this:GetClientUserData( 1 );
	local t_xinShouPworldInfo	= t_showXinShouFade[nXinShouType];
	if t_xinShouPworldInfo == nil  then
		return;
	end
	
	-- TODO: 重构为显示与新手指引无关
	if not GameWizard:isDo( t_xinShouPworldInfo["wizardOp"] ) then
		local tex = getglobal( "PworldSceneFrameBackTex" );
		tex:SetTexture( t_xinShouPworldInfo["path"] );
		if ( t_xinShouPworldInfo["curTaskId"] == 0 or Quest:isQuestRun( t_xinShouPworldInfo["endTaskId"] ) ) then
			GameWizard:doStep( t_xinShouPworldInfo["wizardOp"] );
		end
	end
	
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function PworldSceneFrame_OnUpdate()
	t_playSizeAnimControl:update();
end