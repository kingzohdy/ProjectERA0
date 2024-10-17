-------------------------特殊处理的游戏地图--------------------------------------------------------------
--特殊之处:2048*2048地图,它给定的全部地图左上角顶点与游戏实际左上角顶点并不对应,而是有一定的偏移程度

local g_ImportantStatic = {}
local g_ImportantDynamic = {}
local g_ImportantVirtual = {}
local g_ImportantBaseWar = {}				--据点战资源点标记
local g_MachineWarBoss = {}					--尖峰时刻阵营boss位置
local g_ClanWarCampMon = {}						--跨服公会战枢纽

local nCurSelectNpcName = 0;
local MAX_NPC_NAME		= 18;
local MAX_NPC_POS		= 42;
-------------------------全局变量区-----------------------------------------------------------------------------------
local g_nReferMapId		= 0;	-- 用于存储当前显示的地图ID
local g_nRealMapId		= 0;

local LAST_WORLD_MAP_SUDI_BTN_USE	= 0;
local MAX_SELECT_NAME	= 10;
local MAX_TEAM_MEMBER	= 6;
local MAX_CAMP_MEMBER	= 0;
local MAX_SCENE_AREA	= 10;
local bSeeOtherAreaMap	= false;-- 标志当前是否处于查看其他区域地图的状态
local LAST_IMPORTANT_BTN_USE = 0;
local LAST_ORE_BTN_USE = 0;
local LAST_BIG_ORE_BTN_USE = 0;
local LAST_PWORLD_BOSS_USE = 0;			--用于显示尖峰时刻副本特殊boss
--@Desc: 此为存储紧耦合的X,Y坐标输入框
local t_gCoordInputEdit = { "FWorldMapFrame_XCoordInputEdit", "FWorldMapFrame_YCoordInputEdit" };

------------------------------地图信息-------------------------------
-- TODO: 考虑到以后用矩形来计算
local g_nViewWidht			= 578;
local g_nViewHeight			= 578;
local g_nMapTextureWidth	= 1024;
local g_nMapTextureHeight	= 1024;
local g_fZoom				= g_nViewWidht / g_nMapTextureWidth;
local g_szViewPortName		= "WorldMapFrameMapTexture";		-- 视口名称

------------------鼠标信息--------------------------------------------
local g_nMouseClickLastTime	= 0;		-- 此为最后一次点击鼠标的时间,以用来控制特效的显示时间\

-----------------地图上显示的NPC类型-----------------------------------
local ALL_NPC		= 0;
local FUNCTION_NPC	= 8;	--功能NPC
local QUEST_NPC		= 16;	--任务NPC
local ACT_NPC		= 32;	--活动NPC
local TRAN_NPC		= 64;	--传送NPC
local MONOSTER		= 1;
local CHAOS_MONSTER_ID	= 302169;	-- 混乱元素id
MIN_COPY_ID	= 10000;
local MAX_MAP_ORE_LIST = 8;

local g_nCurNpcType	= ALL_NPC;--默认总是显示全部NPC

--@Desc:此为用于存储控制地图上显示的NPC信息的5个按钮,用于控制着5个按钮的高亮状态
local t_gDisplayButton	= { 
							["WorldMapFrame_AllButton"]			= ALL_NPC, 
							["WorldMapFrame_FunctionButton"]	= FUNCTION_NPC , 
							["WorldMapFrame_QuestButton"]		= QUEST_NPC ,
							["WorldMapFrame_ActivityButton"]	= ACT_NPC , 
							["WorldMapFrame_MonsterButton"]		= MONOSTER
							};

-------------------------缓存地图上各种类型的NPC的表-----------------------
--@节点结构:node = { npcID, npcName, npcPositionX, npcPositionY }

local t_gNpcTypeList		= { [ALL_NPC] = {}, [FUNCTION_NPC] = {}, [QUEST_NPC] = {}, [ACT_NPC] = {}, 
								[TRAN_NPC] = {}, [MONOSTER] = {} };

local t_gNpcTypeDesc		= { [FUNCTION_NPC] = "功能", [QUEST_NPC] = "任务", [ACT_NPC] = "活动", [TRAN_NPC] = "传送" };
---------------------地图特效----------------------------------------------
local g_nTargetTexNum	= 1;

local g_nAutoTexNum		= 1;

tWeatherData = nil

-- 当前选择要显示的地图所处位置的信息
local g_nCurSelectShowRealMapID		= 0;
local g_nCurSelectShowReferMapID	= 0;
-- 显示世界地图的时候要隐藏的区域地图的控件
local t_areaIcon =	{	"WorldMapFrame_MainCoordBtn",		"WorldMapFrame_AllButton",			"WorldMapFrame_FunctionButton", 
						"WorldMapFrame_QuestButton",		"WorldMapFrame_ActivityButton",		"WorldMapFrame_MonsterButton",	"WorldMapFrame_NamesBackBtn",		"WorldMapFrame_SliderBack",			"WorldMapFrame_ScrollUpBtn", 
						"WorldMapFrame_ScrollDownBtn",		"WorldMapFrame_ScrollBar",			"WorldMapFrame_CoordInputHeadBtn", "WorldMapFrame_XCoordInputHeadBtn", "WorldMapFrame_XCoordInputBtn",		"WorldMapFrame_XCoordInputEdit", 
						"WorldMapFrame_YCoordInputHeadBtn", "WorldMapFrame_YCoordInputBtn",		"WorldMapFrame_YCoordInputEdit", "WorldMapFrame_AutoFindPathButton", "WorldmapMainShowButton",			"WorldMapFrame_NpcNameInputHeadBtn", 
						"WorldMapFrame_NpcNameInputBtn",	"WorldMapFrame_NpcNameInputEdit",	"WorldMapFrame_AutoFindPathByNameButton", "WorldMapFrame_ShowTaskBtn",		"WorldMapFrame_NpcTaskInputBtn",	"WorldMapFrame_WorldBtn",
					};

local t_failReasons = 
{
	[MOVE_TIME_INTERVAL_SMALL]		= { desc = "2次寻路时间间隔太短，寻路失败" },
	[MOVE_SPEED_ZERO]				= { desc = "移动速度为0，寻路失败" },
	[MOVE_STATUS_FORBID]			= { desc = "当前状态不允许移动，寻路失败" },
	[MOVE_TYPE_NOT_NORMAL]			= { desc = "当前状态不允许移动，寻路失败" },
	[MOVE_STATE_CANNOT_MOVE]		= { desc = "当前状态不允许移动，寻路失败" },
	[MOVE_IN_TETANIC]				= { desc = "当前状态不允许移动，寻路失败" },
	[MOVE_POINT_CANNOT_WALK]		= { desc = "目标在副本中或不可到达区域，寻路失败" },
	[MOVE_ALREADY_IN_DEST]			= { desc = "主角已在目标点，无需寻路" },
	[MOVE_DIST_INTERVAL_SMALL]		= { desc = "距离太短无需寻路" },
	[MID_MULTIMAP_AUTOFIND_FAIL]	= { desc = "不能自动寻路到所选择的位置" },
	[MID_PATH_FINDNONPC]			= { desc = "目标NPC不在当前地图" },
};

MapOres = {}
MapBigOres = {}
local lastUpdateMapOre = 0

function GetMapOres()
	local i = 0
	local j = MapOre:GetOreCount()
	return function()
		if i >= j then
			return
		end
		local n = i
		i = i + 1
		return MapOre:GetOreId(n), MapOre:GetOreMemId(n), MapOre:GetOreMap(n), MapOre:GetOreX(n), MapOre:GetOreY(n)
	end
end

function GetMapBigOres()
	local i = 0
	local j = MapOre:GetBigOreCount()
	return function()
		if i >= j then
			return
		end
		local n = i
		i = i + 1
		return MapOre:GetBigOreId(n), MapOre:GetBigOreMemId(n), MapOre:GetBigOreMap(n), MapOre:GetBigOreX(n), MapOre:GetBigOreY(n)
	end
end

function UpdateMapOres()
	local now = os.clock()
	if now < lastUpdateMapOre then
		lastUpdateMapOre = now
	end
	if now - lastUpdateMapOre < 0.1 then
		return
	end
	lastUpdateMapOre = now
	local player = ActorMgr:getMainPlayer()
	local map = player:getMapRealId()
	local x, y = player:getPlayerX() / 10, player:getPlayerZ() / 10
	local radius = MapOre:GetOreDetectRadius()
	radius = radius * radius
	for _, ore in ipairs(MapOres) do
		if ore.show then
			ore.show = nil
		end
		if ore.map == map and MapOre:CanCollectOre(ore.id) then
			local dx = ore.x - x
			local dy = ore.y - y
			if dx * dx + dy * dy < radius then
				ore.show = true
			end
		end
	end
end

local nLastRequestTime = 0;
local nLastShowFailTime = 0;

function ShowMoveFailReson( nFailCode )
	local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
	if t_AutoRoundWorlldControl:isAroundWorld() then
		return;
	end

	if nLastShowFailTime > 0 and os.clock() - nLastShowFailTime < 6 then
		return;
	end

	if ( nFailCode == MOVE_POINT_CANNOT_WALK or nFailCode == MID_MULTIMAP_AUTOFIND_FAIL ) and 
			TeamManager:isInTeamFollow() then
		TeamManager:Team_OP_CancelTeamFollow();	
		ShowMidTips( "您或跟随目标处于不能跟随位置，跟随失败" );
		nLastShowFailTime = os.clock();
		return;	
	end

	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
		ShowMidTips( "您或跟随目标处于不能跟随位置，跟随失败" );
	else
		ShowMidTips( t_failReasons[nFailCode]["desc"] );
	end

	nLastShowFailTime = os.clock();
end

-- 保存世界所有的地区
local t_WorldArea = {};
-- 保存世界所有的副本
local t_WorldCopy = {};
--@Desc: 此为用来加载区域地图
function Maxmap_OnLoad()
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "GE_ENTER_MAP" );
	this:RegisterEvent( "GE_CHECK_MULTIMAP_AUTOFIND" );
	this:RegisterEvent( "GE_END_AUTOFIND" );
	this:RegisterEvent( "GE_UPDATE_NPC" );
	this:RegisterEvent( "GE_QUEST_CHANGE" );
	this:RegisterEvent( "GE_QUEST_GETQUESTINFO" );
	this:RegisterEvent( "UI_ACTOR_LEVEL_CHG" );
	this:RegisterEvent( "GE_MAP_ORE_UPDATE" );
	this:RegisterEvent( "GE_MAP_BIG_ORE_UPDATE" );
	
	WorldmapAreaPosBtn1:SetClientString( "十三号矿洞" );
	WorldmapAreaPosBtn2:SetClientString( "落日荒野" );
	WorldmapAreaPosBtn3:SetClientString( "天空岛" );
	WorldmapAreaPosBtn4:SetClientString( "时间城" );
	WorldmapAreaPosBtn5:SetClientString( "银风雪原" );
	WorldmapAreaPosBtn6:SetClientString( "风回裂谷" );
	WorldmapAreaPosBtn7:SetClientString( "牢城" );
	WorldmapAreaPosBtn8:SetClientString( "普罗圣地" );
	WorldmapAreaPosBtn9:SetClientString( "荫川林地" );
	WorldmapAreaPosBtn10:SetClientString( "流金沙漠" );
	WorldmapAreaPosBtn11:SetClientString( "阿努比斯神殿" );
	WorldmapAreaPosBtn12:SetClientString( "阿努比斯废墟" );
	WorldmapAreaPosBtn13:SetClientString( "越雾雨林" );
	WorldmapAreaPosBtn14:SetClientString( "越雾甬道" );
	WorldmapAreaPosBtn15:SetClientString( "西部荒野" );
	WorldmapAreaPosBtn16:SetClientString( "细语丘陵" );
	WorldmapAreaPosBtn17:SetClientString( "鸣沙陷地" );
	WorldmapAreaPosBtn18:SetClientString( "法老陵" );
	WorldmapAreaPosBtn19:SetClientString( "阿努比斯祭坛" );
	WorldmapAreaPosBtn20:SetClientString( "冰岛" );
	WorldmapAreaPosBtn21:SetClientString( "碎金矿坑" );
	WorldmapAreaPosBtn22:SetClientString( "迈阿荒原" );
	WorldmapAreaPosBtn23:SetClientString( "鲜血庇护所" );
	WorldmapAreaPosBtn24:SetClientString( "莫名遗迹" );
	WorldmapAreaPosBtn25:SetClientString( "祈妙神坛" );
	WorldmapAreaPosBtn26:SetClientString( "牢城审讯室" );
	WorldmapAreaPosBtn27:SetClientString( "失落之域一层" );
	WorldmapAreaPosBtn28:SetClientString( "钢铁要塞" )
	-- 设置表示时间城的点的纹理
	WorldmapAreaPosBtn4:ChangeNormalTexture( 684, 60, 24, 25 );
	WorldmapAreaPosBtn4:ChangeHighlightTexture( 709, 60, 24, 25 );
	WorldmapAreaPosBtn4:ChangePushedTexture( 734, 60, 24, 25 );
	WorldMapFrameShowAttackPosBtn:setUpdateTime( 0 );
	WorldMapFrameShowVenuePosBtn:setUpdateTime( 0 );
end

local bEnterMap = false;
local bEnterPworld = false;

function getEnterMap()
	return bEnterMap, bEnterPworld;
end

function resetEnterMap()
	bEnterMap = false;
	bEnterPworld = false;
end

function Maxmap_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" or arg1 == "GE_ENTER_MAP" then
		local MaxMapControl	= MapShower:GetMaxMapContal();
		g_nReferMapId		= MaxMapControl:getMapID();
		g_nRealMapId		= MapShower:getRealMapId();
		MaxMapControl:setMapZoom( g_fZoom );
		MaxMapControl:setView( g_nViewWidht, g_nViewHeight );
		-- 设置地图的纹理
		local nTexID = 0;
		if g_nReferMapId == 0 then
			nTexID = g_nRealMapId;
		else
			nTexID = g_nReferMapId;
		end

		if CLIENT_STALL_MAP ~= g_nReferMapId then
			local t_hideIcons = { "StallSaleFrame", };
			util.HideIcons( t_hideIcons );
		end
		
		g_nCurSelectShowRealMapID	= g_nRealMapId;
		g_nCurSelectShowReferMapID	= g_nReferMapId;
		InitNpcModelList();
		ClearImportWordMapIcon();
		ClearImportWordMapVirtualIcon();
		ClearMachineWarBoss();
		if t_specialMapDesc[nTexID] ~= nil then
			MaxMapControl:setMapTextRect(	t_specialMapDesc[nTexID].mapValidNormalizeTextureWidth, 
											t_specialMapDesc[nTexID].mapValidNormalizeTextureHeight, 
											t_specialMapDesc[nTexID].leftOffset, t_specialMapDesc[nTexID].topOffset,
											t_specialMapDesc[nTexID].completeTextureWidth,	t_specialMapDesc[nTexID].completeTextureHeight,
											t_specialMapDesc[nTexID].clicpTextureWidth,		t_specialMapDesc[nTexID].clicpTextureHeight,
											g_nViewWidht, g_nViewHeight );
		else
			MaxMapControl:setMapTextRect( g_nMapTextureWidth, g_nMapTextureHeight, 0, 0, g_nMapTextureWidth, g_nMapTextureHeight,
											g_nMapTextureWidth, g_nMapTextureHeight, g_nViewWidht, g_nViewHeight );
		end
		
		if math.floor(nTexID  / 30000) == 1 then
			nTexID = 30000
		end
		local tex = "uires\\DiTuLei\\QuYuDiTu\\"..nTexID..".dds";
		WorldMapFrameMapTexture:SetTexture( tex );
		MaxMapControl:updateMapInfo();
		-- 设置当前地图所处的地图名称
		
		if g_nRealMapId >= MIN_COPY_ID then
			-- 若当前进入的是副本，则设置副本名字，并将地区名字清空
			WorldMapFrame_AreaSelectFont:SetText( "地区" );
			WorldMapFrame_CopySelectFont:SetText( WipeOffNumber( MaxMapControl:getMapName( g_nRealMapId ) ) );
		else
			-- 若进入的是地图，则设置地图名字，并将副本名字清空
			WorldMapFrame_AreaSelectFont:SetText( WipeOffNumber( MaxMapControl:getMapName( g_nRealMapId ) ) );
			WorldMapFrame_CopySelectFont:SetText( "副本" );
		end
		
		-- 当切换场景的时候,关闭原来显示的区域地图
		WorldMapFrame:Hide();

		-- 若为加载地图信息，缓存所有地图的信息,用于下拉显示tips使用
		if table.getn( t_WorldArea ) == 0 then
			for i = 1, MapShower:getMapNum() do
				local mapInfo	= MapShower:getMapInfoByIndex( i - 1 );
				if mapInfo.DontList == 0 then
					local t_info	= { MapID = mapInfo.m_nMapID, MapName = getMapName(mapInfo.m_nMapID), 
														ReferMapID = mapInfo.m_nReferMapID, MapArea = mapInfo.m_nMapArea };
					if mapInfo.m_nMapID < MIN_COPY_ID then
						if mapInfo.m_nReferMapID == 0 then
							table.insert( t_WorldArea, t_info );
						end
					elseif mapInfo.m_nMapID ~= 60000  then
						if mapInfo.m_nReferMapID == 0 then
							table.insert( t_WorldCopy, t_info );
						end
					end
				end
			end			
		end

		-- 检查是否要进行跨区域寻路
		local mainplayer = ActorMgr:getMainPlayer();
		local multiMapFind = MapShower:getMultiMapFindControl();
		if g_nRealMapId >= MIN_COPY_ID then
			multiMapFind:loadShortestPaths( g_nRealMapId );
		else
			multiMapFind:loadShortestPaths( nTexID );
		end				
		multiMapFind:continueNextMapAuto( mainplayer );

		-- 若NPC对话面板还显示，关闭
		if NpcTalkFrame:IsShown() then
			Close_NpcDialog( "NpcTalkFrame" );
		end

		if arg1 == "GE_ENTER_PLAYERLOGIN" then
			if not GameWizard:isDo( GAMEWIZARD_FIRST_POP_MOUSE_HABIT_XINSHOU_FRAME ) then
				ShowMouseHabitFrame();
			end
		end
		WorldMapFrameBuffBtn:Hide();
		MapShower:CheckAutoPathFind();

		if arg1 == "GE_ENTER_MAP" then
			--检查是否要取消组队跟随
			if g_nRealMapId == CLAN_CITY_MAP_ID or g_nRealMapId == BEACH_MAP_ID or g_nRealMapId == BRAVEISLAND_MAP_ID or 
				g_nRealMapId == CARNIVAL_MAP_ID or GameBattle:isRun() or checkInHundredTown() then
				if mainplayer:isInTeam() and ( TeamManager:isInTeamFollow() or isAnyTeamMemberInFollow() ) then
					TeamManager:Team_OP_CancelTeamFollow();
				end
			end
			if g_nRealMapId >= MIN_COPY_ID then
				bEnterPworld = true;
			end
			if TeamManager:isInTeamFollow() then
				bEnterMap = true;
			end
			TroopInviteTeamFollowFrame:Hide();
			ChangeWeather()
		end
		-- 切换地图关闭界面
		local t_Frames = { "ArmRecastFrame", "ArmProceStarFrame","ArmWashFrame", "ArmProceBindFrame", "ArmProceStarMoveFrame", "EquipJieFengFrame",
							"ArmProceFrame", "PetSkillResetFrame", "PetSkillLevelUpFrame", "Pet_DevelopmentBuildUpFrame", 
							"PetGrowResetFrame", "PetProceFrame", "MagicSmelterFrame", "MessageBoxForGameFrame", 
							"MessageBoxFrameForVip", "MessageBoxForItemFrame", "WorldMapSendFrame" } 
		util.HideIcons( t_Frames )
	end

	if arg1 == "GE_CHECK_MULTIMAP_AUTOFIND" then
		MapShower:getMultiMapFindControl():CheckArriveTarget( ActorMgr:getMainPlayer() );
	end
	
	if arg1 == "GE_END_AUTOFIND" then
		Maxmap_LastAutoLineButton:Hide();
		MapLine:ClearDrawPointList();
	end
	
	if arg1 == "GE_UPDATE_NPC" then
		InitNpcModelList();

		local slider = getglobal( "WorldMapFrame_ScrollBar" );
		-- 更新区域地图的名字列表
		if slider:IsShown() then
			UpdateNpcNameList( slider:GetValue() + 1 ); 
		else
			UpdateNpcNameList( 1 );
		end
		
		-- 检查是否显示怪物图\=09876		
		if g_nCurNpcType == MONOSTER or g_nCurNpcType == ALL_NPC then
			UpdateSceneButtonOnMap();
		end

		-- 设置在地图上出现的NPC控件
		if g_nCurNpcType ~= MONOSTER then
			UpdateNpcButtonOnMap();	-- 更新在区域地图上显示的NPC控件
		end
	end
	
	if arg1 == "GE_QUEST_CHANGE" or arg1 == "GE_QUEST_GETQUESTINFO" then
		UpdateNpcPosButtonTasTex();
	end

	if arg1 == "UI_ACTOR_LEVEL_CHG" then
		if not WorldMapFrame:IsShown() then
			return;
		end
		Quest:updateNpcQuestTitle( g_nCurSelectShowRealMapID, g_nCurSelectShowReferMapID );
		UpdateNpcPosButtonTasTex();
	end
	
	if arg1 == "GE_MAP_ORE_UPDATE" then
		MapOres = {}
		for id, handle, map, x, y in GetMapOres() do
			table.insert(MapOres, {
				id = id,
				handle = handle,
				map = map,
				x = x,
				y = y,
				name = getMonsterName(id)
			})
		end
	elseif arg1 == "GE_MAP_BIG_ORE_UPDATE" then
		MapBigOres = {}
		for id, handle, map, x, y in GetMapBigOres() do
			table.insert(MapBigOres, {
				id = id,
				handle = handle,
				map = map,
				x = x,
				y = y,
				name = getMonsterName(id)
			})
		end
	end
end

-- 根据鼠标在屏幕的偏移量来计算鼠标所点击地方的实际场景坐标
function getMouseCurPos( nScreenX, nScreenY )
	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end
	
	local nMapClicpTextWidth	=  t_specialMapDesc[nTexID] ~= nil and t_specialMapDesc[nTexID].clicpTextureWidth 
									or g_nMapTextureWidth;
	local nMapClicpTextHeight	= t_specialMapDesc[nTexID] ~= nil and t_specialMapDesc[nTexID].clicpTextureHeight 
									or g_nMapTextureHeight;

	local nMapTextLeft	= t_specialMapDesc[nTexID] ~= nil and t_specialMapDesc[nTexID].leftOffset or 0;
	local nMapTextTop	= t_specialMapDesc[nTexID] ~= nil and t_specialMapDesc[nTexID].topOffset or 0;

	local nMapWorldTextWidth	= t_specialMapDesc[nTexID] ~= nil and t_specialMapDesc[nTexID].completeTextureWidth 
									or g_nMapTextureWidth;
	local nMapWorldTextHeight	= t_specialMapDesc[nTexID] ~= nil and t_specialMapDesc[nTexID].completeTextureHeight 
									or g_nMapTextureHeight;
	
	local mapSize = MapShower:getMapSize( nTexID );

	local nPlayerOffsetXOnNormalizeTexture = nScreenX / g_fZoom;
	local nPlayerOffsetYOnNormalizeTexture = nScreenY / g_fZoom;

	local nPlayerOffsetXOnClicpTexture = nPlayerOffsetXOnNormalizeTexture * nMapClicpTextWidth  / NORMALIZE_TEX_WIDTH;
	local nPlayerOffsetYOnClicpTexture = nPlayerOffsetYOnNormalizeTexture * nMapClicpTextHeight / NORMALIZE_TEX_HEIGHT;

	local nPlayerOffsetXOnWorldTexture = nPlayerOffsetXOnClicpTexture + nMapTextLeft;
	local nPlayerOffsetYOnWorldTexture = nPlayerOffsetYOnClicpTexture + nMapTextTop;
	local nRealX = nPlayerOffsetXOnWorldTexture * mapSize.first / nMapWorldTextWidth;
	local nRealY = mapSize.second - ( nPlayerOffsetYOnWorldTexture * mapSize.second / nMapWorldTextHeight );

	return { x = nRealX, y = nRealY };
end

-- 根据指定的偏移量和地图ID获取在纹理上的偏移量
function getPointCoordOnTex( nRealX, nRealY, nMapWidth, nMapHeight, nMapID )
	local nMapTextLeft	= t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].leftOffset or 0;
	local nMapTextTop	= t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].topOffset or 0;
	local nMapWorldTextWidth	= t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].completeTextureWidth 
									or g_nMapTextureWidth;
	local nMapWorldTextHeight	= t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].completeTextureHeight 
									or g_nMapTextureHeight;
	local nMapClicpTextWidth	=  t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].clicpTextureWidth 
									or g_nMapTextureWidth;
	local nMapClicpTextHeight	= t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].clicpTextureHeight 
									or g_nMapTextureHeight;

	local nPlayerOffsetXOnClicpTexture = nRealX * nMapWorldTextWidth / nMapWidth - nMapTextLeft;
	local nPlayerOffsetYOnClicpTexture = ( nMapHeight- nRealY ) * nMapWorldTextHeight / nMapHeight - nMapTextTop;

	local nPlayerOffsetXOnNormalizeTexture = nPlayerOffsetXOnClicpTexture * NORMALIZE_TEX_WIDTH / nMapClicpTextWidth;
	local nPlayerOffsetYOnNormalizeTexture = nPlayerOffsetYOnClicpTexture * NORMALIZE_TEX_HEIGHT / nMapClicpTextHeight;
	
	return { x = nPlayerOffsetXOnNormalizeTexture, y = nPlayerOffsetYOnNormalizeTexture };
end

function GetCurMapTexID()
	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end

	return nTexID;
end

function ChangeAreaMap( nReferMapId, nRealMapId )	
	-- 显示区域地图才显示的控件，并隐藏世界地图使用的控件
	local nTexID = 0;
	if nReferMapId == 0 then
		nTexID = nRealMapId;
	else
		nTexID = nReferMapId;
	end
	local MaxMapControl	= MapShower:GetMaxMapContal();
	MaxMapControl:SetMapSize( nTexID );

	ShowAreaMap( nRealMapId );
	nCurSeeRealMapID = nRealMapId;
	-- 若正在查看与主角所在的地图相同
	if nRealMapId == MapShower:getRealMapId() then
		bSeeOtherAreaMap = false;
		-- 显示表示主角位置的图标
		WorldmapMainShowButton:Show();
		MapLine:Show();
		WorldMapFrameClanCityMemberLine:Show();			
	else -- 若正在查看与主角所在地图的不同
		bSeeOtherAreaMap = true;
		-- 隐藏表示主角位置的图标
		WorldmapMainShowButton:Hide();
		MapLine:Hide();
		WorldMapFrameClanCityMemberLine:Hide();
		Maxmap_LastAutoLineButton:Hide();
	end

	g_nCurSelectShowRealMapID	= nRealMapId; -- 默认显示的地图为当前主角所在的地图
	g_nCurSelectShowReferMapID	= nReferMapId;
	
	if nRealMapId >= MIN_COPY_ID then
		-- 若当前进入的是副本，则设置副本名字，并将地区名字清空
		WorldMapFrame_AreaSelectFont:SetText( "地区" );
		WorldMapFrame_CopySelectFont:SetText( WipeOffNumber( getMapName( nRealMapId ) ) );
	else
		-- 若进入的是地图，则设置地图名字，并将副本名字清空
		WorldMapFrame_AreaSelectFont:SetText( WipeOffNumber( getMapName( nRealMapId ) ) );
		WorldMapFrame_CopySelectFont:SetText( "副本" );
	end

	-- 清空所有队友图标信息
	ClearMaxMapTeamPosBtn();
	-- 设置地图的基本状态信息	
	
	-- 第一次显示区域地图默认显示全部NPC
	g_nCurNpcType = ALL_NPC;
	InitNpcModelList();
	-- 设置地图的纹理
	
	if math.floor(nTexID  / 100) == 300 then
		nTexID = 30000
	end
	local tex = "uires\\DiTuLei\\QuYuDiTu\\"..nTexID..".dds";
	WorldMapFrameMapTexture:SetTexture( tex );

	local nMapPosWidth	= g_nViewWidht / g_fZoom;
	local nMapPosHeight = g_nViewHeight / g_fZoom;
	WorldMapFrameMapTexture:SetTexUV( 0, 0, nMapPosWidth, nMapPosHeight );
	
	-- 设置滚动条和默认初始显示全部NPC名字
	local t_curDisplayList = t_gNpcTypeList[g_nCurNpcType];
	local slider = getglobal( "WorldMapFrame_ScrollBar" );	
	slider:SetValue( 0 );
	SetMaxMapScrollBar( t_curDisplayList );	
	
	nCurSelectNpcName = 0;
	DisCheckAllBtn( "NpcNameButton", MAX_NPC_NAME );
	-- 更新区域地图的名字列表
	if slider:IsShown() then
		UpdateNpcNameList( slider:GetValue() + 1 ); 
	else
		UpdateNpcNameList( 1 );
	end
	
	-- 更新区域地图上的怪物和NPC图标
	-- 检查是否显示怪物图标
	if g_nCurNpcType == MONOSTER or g_nCurNpcType == ALL_NPC then
		UpdateSceneButtonOnMap();
	end

	-- 设置在地图上出现的NPC控件
	if g_nCurNpcType ~= MONOSTER then
		UpdateNpcButtonOnMap();	-- 更新在区域地图上显示的NPC控件
	end
	
	-- 设置好界面上面的按钮显示状态
	for name, _ in pairs( t_gDisplayButton ) do
		local btn = getglobal( name );
		btn:Enable();
		btn:DisHighlight();
	end

	WorldMapFrame_AllButton:Disable();
	WorldMapFrame_XCoordInputEdit:Clear();
	WorldMapFrame_YCoordInputEdit:Clear();
	WorldMapFrame_MainCoordFont:SetText( "" );

	UpdateWorldMapClanMapPos();
end

-- 显示世界地图
function ShowWorldMap()
	for i = 1, table.getn( t_areaIcon ) do
		local btn = getglobal( t_areaIcon[i] );
		btn:Hide();
	end

	WorldMapFrame_ShowWorldBtn:Show();
	WorldMapFrame_ShowMapBtn:Hide();
	WorldMapFrame_ResourceBtn:Hide();

	for i = 1, MAX_NPC_NAME do
		local btn = getglobal( "NpcNameButton" .. i );
		btn:Hide();
	end
	
	ClearMaxMapNpcPosBtn();
	
	for i = 1, LAST_IMPORTANT_BTN_USE do
		local btn = getglobal("WordmapImportantBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_IMPORTANT_BTN_USE = 0
	
	for i = 1, LAST_ORE_BTN_USE do
		local btn = getglobal("WordmapOreBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_ORE_BTN_USE = 0
	
	for i = 1, LAST_BIG_ORE_BTN_USE do
		local btn = getglobal("WordmapBigOreBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_BIG_ORE_BTN_USE = 0
	
	local MAX_CLAN_CAMP = 3
	for i = 1, MAX_CLAN_CAMP do
		local btn = getglobal("WordmapClanCampBtnTemplate" .. i)
		if btn then
			btn:Hide()
		end
	end

	for i = 1, MAX_TEAM_MEMBER do
		local btn = getglobal( "WorldmapTeamPosBtn" .. i );
		btn:Hide();
	end
	ClearMaxmapSceneNameButton();

	WorldmapCaptainPosBtn:Hide();
	WorldMapFrameHeadFont:SetText( "世界地图" );
	
	WorldMapFrame_AreaSelectFont:SetText( "地区" );
	WorldMapFrame_CopySelectFont:SetText( "副本" );
	Maxmap_LastAutoLineButton:Hide();
	WorldMapFrameBuffBtn:Hide();
	WorldMapFrameCityAttackBtn:Hide();
	WorldMapFrameCityVenueBtn:Hide();
	WorldMapFrameShowAttackPosBtn:Hide();
	WorldMapFrameShowVenuePosBtn:Hide();

	local mapInfo = MapShower:getMapInfoByID( MapShower:getRealMapId() )
	for i=1,22 do
		local btn = getglobal( "WorldmapAreaPosBtn" .. i )
		if mapInfo ~= nil and string.find( mapInfo.m_szMapName, btn:GetClientString(), 1, true ) then
			btn:ChangeNormalTexture( 908, 603, 24, 25 );
			btn:ChangeHighlightTexture( 933, 603, 24, 25 );
			btn:ChangePushedTexture( 958, 603, 24, 25 );
		elseif i == 4 then
			btn:ChangeNormalTexture( 684, 60, 24, 25 );
			btn:ChangeHighlightTexture( 709, 60, 24, 25 );
			btn:ChangePushedTexture( 734, 60, 24, 25 );
		else			
			btn:ChangeNormalTexture( 684, 34, 24, 25 );
			btn:ChangeHighlightTexture( 709, 34, 24, 25 );
			btn:ChangePushedTexture( 734, 34, 24, 25 );
		end
	end
end

function checkNeedShowResBtn( nRealMapId )
	local mapOreDef = MapShower:getMapOreDef( nRealMapId );
	if mapOreDef == nil then
		return false;
	else
		for i=1, MAX_MAP_ORE_LIST do
			if mapOreDef.OreList[i-1].MonsterID ~= 0 and (mapOreDef.OreList[i-1].MonsterID < 295000 or mapOreDef.OreList[i-1].MonsterID > 295302) then
				return false;
			else
				return true;
			end
		end
	end
	return true;
end

-- 显示区域地图
function ShowAreaMap( nRealMapId )
	for i = 1, table.getn( t_areaIcon ) do
		local btn = getglobal( t_areaIcon[i] );
		btn:Show();
	end

	WorldMapFrame_ShowWorldBtn:Hide();
	WorldMapFrame_ShowMapBtn:Show();
	if checkNeedShowResBtn( nRealMapId ) then
		WorldMapFrame_ResourceBtn:SetClientUserData( 0, nRealMapId );
		WorldMapFrame_ResourceBtn:Show();
	else
		WorldMapFrame_ResourceBtn:SetClientUserData( 0, 0 );
		WorldMapFrame_ResourceBtn:Hide();
	end

	for i = 1, MAX_NPC_NAME do
		local btn = getglobal( "NpcNameButton" .. i );
		btn:Show();
	end

	WorldMapFrameHeadFont:SetText( "区域地图" );
	-- 对地图id 判断
	if nRealMapId >= MIN_COPY_ID then
		WorldMapFrameCityAttackBtn:Hide();
		WorldMapFrameCityVenueBtn:Hide();
	else
		WorldMapFrameCityAttackBtn:Show();
		WorldMapFrameCityVenueBtn:Show();
	end
end

function CheckCityMemberPosInfo()
	if MapShower:getRealMapId() == WorldCityMapID then
		if os.clock() - nLastRequestTime >= 5 then
		WorldCityManager:RequestClanMemberPos();
		nLastRequestTime = os.clock();
		end
	end
end
function CheckBaseWarClanMemPosInfo()
	if GameClan_BaseWar:IsInBaseWarPworld() then
		if os.clock() - nLastRequestTime >= 5 then
			WorldCityManager:RequestClanMemberPos();
			nLastRequestTime = os.clock();
		end
	end
end

function UpdateClanCityMemberPos( posInfo )
	WorldMapFrameClanCityMemberLine:ClearDrawPointList();
	if WorldMapFrame:IsShown() then
		if g_nCurSelectShowRealMapID == WorldCityMapID or GameClan_BaseWar:IsInBaseWarPworld() then
			local MaxMapControl	= MapShower:GetMaxMapContal();
			local MapInfo = MaxMapControl:updateMapInfo();
			for i=1,posInfo.MemNum do
				if ActorMgr:getMainPlayer():GetID() ~= posInfo.Members[i-1].MemID and not TeamManager:isInTeamById( posInfo.Members[i-1].MemID ) then
					local pos = MaxMapControl:getPointCoordOnTex( posInfo.Members[i-1].Pos.X, posInfo.Members[i-1].Pos.Y );
					local nX = ( pos.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom;
					local nY = ( pos.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom;
					WorldMapFrameClanCityMemberLine:AddPoint( nX,nY );
				end
			end
		end
	end
end

--@Desc:此为显示区域地图的时候响应
function MaxmapOnShow()
	WorldMapFrame_ShowTaskBtn:setCheckState( true );
	-- 刚打开区域地图的时候总是查看当前主角所在地图的信息	
	local MaxMapControl	= MapShower:GetMaxMapContal();	
	ChangeAreaMap( MaxMapControl:getMapID(), MapShower:getRealMapId() );
	UpdateNpcPosButtonTasTex();
	UpdateSuDiButtonOnMap();
	UpdateStallBiaoJiButtonOnMap();
	UpdateMachineWarBossOnMap();
	SetMapDisplayInfo();
	local offset = math.random( 20 ) - 10
	this:SetPoint( "center", "$parent", "center", -45 + offset, offset );
	nLastRequestTime = 0;
	CheckCityMemberPosInfo();
	CheckBaseWarClanMemPosInfo();
	WorldMapFrameClanCityMemberLine:ClearDrawPointList();	
end

--@Desc: 用于初始化各种类型NPC的MODEL列表
function InitNpcModelList()
	-- 首先清空原来的玩家名字,以重新填充
	for key, list in pairs( t_gNpcTypeList ) do
		t_gNpcTypeList[key] = {};
	end
	
	local MaxMapControl	= MapShower:GetMaxMapContal();	

	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end
	nTexID = g_nCurSelectShowRealMapID >= MIN_COPY_ID and g_nCurSelectShowRealMapID or nTexID;

	local npcInfo;
	local npc;
	local node;
	for i = 1, MapShower:getMapNpcCount( nTexID ) do 
		npcInfo = MapShower:getMapNpcInfo( nTexID, i-1 );
		if npcInfo.m_bShow then 
		 	--临时商人(混乱元素)按钮，区域地图中不要显示
		 	if npcInfo.m_nId ~= CHAOS_MONSTER_ID then
				node = { id = npcInfo.m_nId, name = npcInfo.m_szName, realX = npcInfo.m_nRealX , realY = npcInfo.m_nRealY };
				--选择将NPC插入哪个列表
				table.insert( t_gNpcTypeList[ALL_NPC], node );
				
				if MaxMapControl:isNpcRaceType( npcInfo.m_nId, FUNCTION_NPC ) then	--若为功能NPC,则将其插入功能NPC列表
					table.insert( t_gNpcTypeList[FUNCTION_NPC], node );
				elseif MaxMapControl:isNpcRaceType( npcInfo.m_nId, QUEST_NPC ) then --若为任务NPC,则将其插入任务NPC列表 
					table.insert( t_gNpcTypeList[QUEST_NPC], node );
				elseif MaxMapControl:isNpcRaceType( npcInfo.m_nId, ACT_NPC ) then	--若为活动NPC,则将其插入活动NPC列表 
					table.insert( t_gNpcTypeList[ACT_NPC], node );
				elseif MaxMapControl:isNpcRaceType( npcInfo.m_nId, TRAN_NPC ) then	--若为传送NPC,则将其插入传送NPC列表
					table.insert( t_gNpcTypeList[TRAN_NPC], node );
				end
			end
		end
	end
	
	for i = 1, GameClanManager:getClanNpcsCount() do 
		npcInfo = GameClanManager:getClanNpcInfo(i - 1)
		if npcInfo.m_bShow then
			node = {id = npcInfo.m_nId, name = npcInfo.m_szName, realX = npcInfo.m_nRealX , realY = npcInfo.m_nRealY}
			--选择将NPC插入哪个列表
			table.insert( t_gNpcTypeList[ALL_NPC], node)
			
			if MaxMapControl:isNpcRaceType( npcInfo.m_nId, FUNCTION_NPC ) then	--若为功能NPC,则将其插入功能NPC列表
				table.insert( t_gNpcTypeList[FUNCTION_NPC], node );
			elseif MaxMapControl:isNpcRaceType( npcInfo.m_nId, QUEST_NPC ) then --若为任务NPC,则将其插入任务NPC列表 
				table.insert( t_gNpcTypeList[QUEST_NPC], node );
			elseif MaxMapControl:isNpcRaceType( npcInfo.m_nId, ACT_NPC ) then	--若为活动NPC,则将其插入活动NPC列表 
				table.insert( t_gNpcTypeList[ACT_NPC], node );
			elseif MaxMapControl:isNpcRaceType( npcInfo.m_nId, TRAN_NPC ) then	--若为传送NPC,则将其插入传送NPC列表
				table.insert( t_gNpcTypeList[TRAN_NPC], node );
			end
		end
	end

	-- 将传送NPC加到功能NPC中
	for i = 1, table.getn( t_gNpcTypeList[TRAN_NPC] ) do
		if not isInNpcList( t_gNpcTypeList[FUNCTION_NPC], t_gNpcTypeList[TRAN_NPC][i] ) then
			table.insert( t_gNpcTypeList[FUNCTION_NPC], t_gNpcTypeList[TRAN_NPC][i] );
		end
	end
end

function CreatePworldNPC( nDefID, nPosX, nPosY )
	AddNpcList( nDefID, nPosX, nPosY );

	local slider = getglobal( "WorldMapFrame_ScrollBar" );
	-- 更新区域地图的名字列表
	if slider:IsShown() then
		UpdateNpcNameList( slider:GetValue() + 1 ); 
	else
		UpdateNpcNameList( 1 );
	end

	-- 检查是否显示怪物图标
	if g_nCurNpcType == MONOSTER or g_nCurNpcType == ALL_NPC then
		UpdateSceneButtonOnMap();
	end

	if g_nCurNpcType == MONOSTER or g_nCurNpcType == ALL_NPC then
		UpdateSceneButtonOnMap();
	end
end

function AddNpcList( nDefID, nPosX, nPosY )
	local MaxMapControl	= MapShower:GetMaxMapContal();	
	local szName	= getMonsterName( nDefID );
	local node		= { id = nDefID, name = szName, realX = nPosX , realY = nPosY };
	table.insert( t_gNpcTypeList[ALL_NPC], node );

	if MaxMapControl:isNpcRaceType( nDefID, FUNCTION_NPC ) then	--若为功能NPC,则将其插入功能NPC列表
		table.insert( t_gNpcTypeList[FUNCTION_NPC], node );
	elseif MaxMapControl:isNpcRaceType( nDefID, QUEST_NPC ) then	--若为任务NPC,则将其插入任务NPC列表 
		table.insert( t_gNpcTypeList[QUEST_NPC], node );
	elseif MaxMapControl:isNpcRaceType( nDefID, ACT_NPC ) then		--若为活动NPC,则将其插入活动NPC列表 
		table.insert( t_gNpcTypeList[ACT_NPC], node );
	elseif MaxMapControl:isNpcRaceType( nDefID, TRAN_NPC ) then	--若为传送NPC,则将其插入传送NPC列表
		table.insert( t_gNpcTypeList[TRAN_NPC], node );
	end

	-- 将传送NPC加到功能NPC中
	for i = 1, table.getn( t_gNpcTypeList[TRAN_NPC] ) do
		if not isInNpcList( t_gNpcTypeList[FUNCTION_NPC], t_gNpcTypeList[TRAN_NPC][i] ) then
			table.insert( t_gNpcTypeList[FUNCTION_NPC], t_gNpcTypeList[TRAN_NPC][i] );
		end
	end
end

function DelNpcList( nDefID )
	for _, npcList in pairs( t_gNpcTypeList ) do
		for index, data in ipairs( npcList ) do
			if data["id"] == nDefID then
				table.remove( npcList, index );
				break;
			end
		end
	end
end

function DeletePworldNPC( nDefID )
	DelNpcList( nDefID );

	local slider = getglobal( "WorldMapFrame_ScrollBar" );
	-- 更新区域地图的名字列表
	if slider:IsShown() then
		UpdateNpcNameList( slider:GetValue() + 1 ); 
	else
		UpdateNpcNameList( 1 );
	end

	-- 检查是否显示怪物图标
	if g_nCurNpcType == MONOSTER or g_nCurNpcType == ALL_NPC then
		UpdateSceneButtonOnMap();
	end

	if g_nCurNpcType == MONOSTER or g_nCurNpcType == ALL_NPC then
		UpdateSceneButtonOnMap();
	end
end

-- 检测传送NPC是否已经在功能NPC中
function isInNpcList( list, element )
	for _, data in ipairs( list ) do
		if data["id"] == element["id"] then
			return true;
		end
	end

	return false;
end

--@Desc:用于对显示队友的控件进行清空操作
function ClearMaxMapTeamPosBtn()
	local button;
	for i = 1, MAX_TEAM_MEMBER do		
		button = getglobal("WorldmapTeamPosBtn"..i);

		SetButtonInfo( { szName = button:GetName(), ClientString = "", ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
		button:Hide();
	end	
	
	SetButtonInfo( { szName = "WorldmapCaptainPosBtn", ClientString = "", ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
	WorldmapCaptainPosBtn:Hide();
end

function UpdateStallBiaoJiButtonOnMap()
	local t_StallBuyFrameBiaoJiStalls = GetStallBuyFrameBiaoJiStalls();
	for i = 1, MAX_STALL_BIAO_JI_NUM do
		local btn = getglobal( "StallBiaoJiBtn"..i );
		btn:Hide();
	end

	local nBtnIndex				= 1;
	local MaxMapControl			= MapShower:GetMaxMapContal();
	local MapInfo				= MaxMapControl:updateMapInfo();
	local nMapFrameCompareMapID = g_nCurSelectShowReferMapID ~= 0 and g_nCurSelectShowReferMapID or g_nCurSelectShowRealMapID;
	local mapSize				= MapShower:getMapSize( nMapFrameCompareMapID );
	for name, roleData in pairs( t_StallBuyFrameBiaoJiStalls["trackList"] ) do	
		local btn = FindButton( "StallBiaoJiBtn"..nBtnIndex );
		local nRoleCompareMapID	= roleData["mapID"];
		if nMapFrameCompareMapID == nRoleCompareMapID then
			local texOffset	= getPointCoordOnTex( roleData["x"], roleData["y"], mapSize.first, mapSize.second, nMapFrameCompareMapID );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show();
			SetButtonInfo( {	szName = btn:GetName(), ClientString = name.."的摊位", 
								ClientUserData0 = 0,
								ClientUserData1 = roleData["x"], ClientUserData2 = roleData["y"] } );
			nBtnIndex = nBtnIndex + 1;
		end
	end
	local index = 1
	for id, args in pairs(g_ImportantStatic) do
		if nMapFrameCompareMapID == args.map then
			local actor = ActorMgr:FindActor(id)
			if actor then
				local btn = FindButton("WordmapImportantBtn"..index);
				if not btn then
					btn = CreateButton( "WordmapImportantBtn"..index, "WordmapImportantBtnTemplate", WorldMapFrame );
				end
				local x, y = math.floor(args[1] / 10 + 0.5), math.floor(args[2] / 10 + 0.5)
				local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
				SetButtonInfo( {	szName = btn:GetName(), ClientString = GetPlayerName(actor:getName()), 
									ClientUserData0 = 0,
									ClientUserData1 = x, ClientUserData2 = y } );
				btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
				btn:Show()
				btn = getglobal("WordmapImportantBtn"..index.."Tex")
				local path = args.path
				if type(path) == type("") then
					btn:SetTexture(path)
				else
					btn:SetTexture(path.path)
					btn:SetTexUV(path.x, path.y, path.w, path.h)
				end
				index = index + 1;
			end
		end
	end
	for _, args in pairs(g_ImportantVirtual) do
		if nMapFrameCompareMapID == args.map then
			local btn = FindButton("WordmapImportantBtn"..index);
			if not btn then
				btn = CreateButton( "WordmapImportantBtn"..index, "WordmapImportantBtnTemplate", WorldMapFrame );
			end
			local x, y = args.x, args.y
			local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
			SetButtonInfo( {	szName = btn:GetName(), ClientString = args.name, 
								ClientUserData0 = 0,
								ClientUserData1 = x, ClientUserData2 = y } );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show()
			btn = getglobal("WordmapImportantBtn"..index.."Tex")
			local path = args.path
			if type(path) == type("") then
				btn:SetTexture(path)
			else
				btn:SetTexture(path.path)
				btn:SetTexUV(path.x, path.y, path.w, path.h)
			end
			index = index + 1;
		end
	end
	--显示据点战MAP标记
	for _, args in pairs(g_ImportantBaseWar) do
		if nMapFrameCompareMapID == args.map then
			local btn = FindButton("WordmapImportantBtn"..index);
			if not btn then
				btn = CreateButton( "WordmapImportantBtn"..index, "WordmapImportantBtnTemplate", WorldMapFrame );
			end
			local x, y = math.floor(args.x / 10 + 0.5), math.floor(args.y / 10 + 0.5)
			local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
			SetButtonInfo( {	szName = btn:GetName(), ClientString = "资源点", 
								ClientUserData0 = 0,
								ClientUserData1 = x, ClientUserData2 = y } );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show()
			btn = getglobal("WordmapImportantBtn"..index.."Tex")
			local path = args.path
			if type(path) == type("") then
				btn:SetTexture(path)
			else
				btn:SetTexture(path.path)
				btn:SetTexUV(path.x, path.y, path.w, path.h)
			end
			index = index + 1;
		end
	end
	for i = index, LAST_IMPORTANT_BTN_USE do
		btn = getglobal("WordmapImportantBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_IMPORTANT_BTN_USE = index - 1
	
	index = 1
	for _, ore in ipairs(MapOres) do
		-- "4407 地图特殊处理"
		if ore.show and( nMapFrameCompareMapID == ore.map or nMapFrameCompareMapID == 4406 )then 
			local btn = FindButton("WordmapOreBtn"..index);
			if not btn then
				btn = CreateButton("WordmapOreBtn"..index, "WordmapOreBtnTemplate", WorldMapFrame);
			end
			local x, y = ore.x, ore.y
			local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
			SetButtonInfo( {	szName = btn:GetName(), ClientString = ore.name, 
								ClientUserData0 = 0,
								ClientUserData1 = x, ClientUserData2 = y } );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show()
			index = index + 1;
		end
	end
	for i = index, LAST_ORE_BTN_USE do
		btn = getglobal("WordmapOreBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_ORE_BTN_USE = index - 1
	
	index = 1
	for _, ore in ipairs(MapBigOres) do
		if nMapFrameCompareMapID == ore.map then
			local btn = FindButton("WordmapBigOreBtn"..index);
			if not btn then
				btn = CreateButton("WordmapBigOreBtn"..index, "WordmapBigOreBtnTemplate", WorldMapFrame);
			end
			local x, y = ore.x, ore.y
			local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
			SetButtonInfo( {	szName = btn:GetName(), ClientString = ore.name, 
								ClientUserData0 = 0,
								ClientUserData1 = x, ClientUserData2 = y } );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show()
			index = index + 1;
		end
	end
	for i = index, LAST_BIG_ORE_BTN_USE do
		btn = getglobal("WordmapBigOreBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_BIG_ORE_BTN_USE = index - 1
end

function UpdateClanWarCampMonOnMap()
	local index = 1
	local MaxMapControl			= MapShower:GetMaxMapContal();
	local nMapFrameCompareMapID = g_nCurSelectShowReferMapID ~= 0 and g_nCurSelectShowReferMapID or g_nCurSelectShowRealMapID;
	local mapSize				= MapShower:getMapSize( nMapFrameCompareMapID );
	local MAX_CLAN_CAMP = 3
	for i = 1, MAX_CLAN_CAMP do
		local btn = getglobal("WordmapClanCampBtnTemplate" .. i)
		if btn then
			btn:Hide()
		end
	end
	--显示跨服公会战枢纽	
	for _, args in pairs(g_ClanWarCampMon) do
		if nMapFrameCompareMapID == args.map then			
			local btn = FindButton("WordmapClanCampBtnTemplate"..index);
			if not btn then
				btn = CreateButton( "WordmapClanCampBtnTemplate"..index, "WordmapClanCampBtnTemplate", WorldMapFrame );
			end
			local x, y = math.floor(args.x / 10 + 0.5), math.floor(args.y / 10 + 0.5)
			local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
			SetButtonInfo( {	szName = btn:GetName(), ClientString = args.name, 
								ClientUserData0 = 0,
								ClientUserData1 = x, ClientUserData2 = y } );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show()
			btn = getglobal("WordmapClanCampBtnTemplate"..index.."Tex")
			local path = args.path
			if type(path) == type("") then
				btn:SetTexture(path)
			else
				btn:SetTexture(path.path)
				btn:SetTexUV(path.x, path.y, path.w, path.h)
			end
			index = index + 1;
		end
	end
end

function UpdateMachineWarBossOnMap()
	local index = 1
	local MaxMapControl			= MapShower:GetMaxMapContal();
	local nMapFrameCompareMapID = g_nCurSelectShowReferMapID ~= 0 and g_nCurSelectShowReferMapID or g_nCurSelectShowRealMapID;
	local mapSize				= MapShower:getMapSize( nMapFrameCompareMapID );
	--显示尖峰时刻阵营boss位置	
	for _, args in pairs(g_MachineWarBoss) do
		if nMapFrameCompareMapID == args.map then
			local btn = FindButton("WordmapPworldBossBtnTemplate"..index);
			if not btn then
				btn = CreateButton( "WordmapPworldBossBtnTemplate"..index, "WordmapPworldBossBtnTemplate", WorldMapFrame );
			end
			local x, y = math.floor(args.x / 10 + 0.5), math.floor(args.y / 10 + 0.5)
			local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
			SetButtonInfo( {	szName = btn:GetName(), ClientString = args.name, 
								ClientUserData0 = 0,
								ClientUserData1 = x, ClientUserData2 = y } );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show()
			btn = getglobal("WordmapPworldBossBtnTemplate"..index.."Tex")
			local path = args.path
			if type(path) == type("") then
				btn:SetTexture(path)
			else
				btn:SetTexture(path.path)
				btn:SetTexUV(path.x, path.y, path.w, path.h)
			end
			index = index + 1;
		end
	end
	for i = index, LAST_PWORLD_BOSS_USE do
		btn = getglobal("WordmapPworldBossBtnTemplate" .. i)
		if btn then
			btn:Hide()
		end
	end
	LAST_PWORLD_BOSS_USE = index - 1
end

function UpdateSuDiButtonOnMap()
	local t_trackRoleControl	= GetTrackRoleControl();
	local t_allRoles			= t_trackRoleControl:getAllTrackRoles();
	local nTrackNum				= t_trackRoleControl:getAllTrackRolesNum();
	
	for i = LAST_WORLD_MAP_SUDI_BTN_USE + 1, nTrackNum do
		CreateButton( "WorldmapSuDiBtn"..i, "WorldmapSuDiBtnTemplate", WorldMapFrame );
	end

	if nTrackNum > LAST_WORLD_MAP_SUDI_BTN_USE then
		LAST_WORLD_MAP_SUDI_BTN_USE = nTrackNum;
	end

	for i = 1, LAST_WORLD_MAP_SUDI_BTN_USE do
		local btn = FindButton( "WorldmapSuDiBtn"..i );
		btn:Hide();
	end
	
	local nBtnIndex				= 1;
	local MaxMapControl			= MapShower:GetMaxMapContal();
	local MapInfo				= MaxMapControl:updateMapInfo();
	local nMapFrameCompareMapID = g_nCurSelectShowReferMapID ~= 0 and g_nCurSelectShowReferMapID or g_nCurSelectShowRealMapID;
	local mapSize				= MapShower:getMapSize( nMapFrameCompareMapID );

	for name, roleData in pairs( t_allRoles ) do
		local btn = FindButton( "WorldmapSuDiBtn"..nBtnIndex );
		
		local nRoleCompareMapID	= roleData["referMap"] ~= 0 and roleData["referMap"] or roleData["map"];
		if nMapFrameCompareMapID == nRoleCompareMapID then
			local texOffset	= getPointCoordOnTex( roleData["x"], roleData["y"], mapSize.first, mapSize.second, nMapFrameCompareMapID );
			btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			btn:Show();
			SetButtonInfo( {	szName = btn:GetName(), ClientString = name, 
								ClientUserData0 = roleData["map"],
								ClientUserData1 = roleData["x"], ClientUserData2 = roleData["y"] } );
			nBtnIndex = nBtnIndex + 1;
		end
	end
end

--@Desc: 用于更新显示在地图上的队友控件
function UpdateTeamButtonOnMap()
	-- 清空原来的控件信息并隐藏控件
	ClearMaxMapTeamPosBtn();
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return; 
	end

	-- 用于调整在区域地图上队友的图标
	-- 先判断主角是否处于组队状态
	local teamInfo = TeamManager:getTeamInfo();--获取队伍信息
	if teamInfo.MemberNum == 0 then
		return;
	end
	
	-- 在地图上显示队友的位置信息
	local button;
	local MaxMapControl		= MapShower:GetMaxMapContal();
	local MapInfo			= MaxMapControl:updateMapInfo();
	local nTeamCount		= 1;
	local mainPlayerTeamInfo = TeamManager:getTeamMemberInfo( mainplayer:getName() );
	local teamMapPosition	= {};

	local nCurSeeMapID = 0;
	
	if bSeeOtherAreaMap then
		--print( " true " );
		nCurSeeMapID = GetCurMapTexID();
	else
		--print( " false " );
		nCurSeeMapID =  mainPlayerTeamInfo.MapID;
	end

	--print( "nCurSeeMapID = "..nCurSeeMapID.."--839" );

	for i = 1, teamInfo.MemberNum do
		local teamMember = MaxMapControl:updateMapTeamMemberInfo( i - 1 );
		-- 若当前获取的是队友而非队长的信息

		--print( "teamMember.m_nMapId = "..teamMember.m_nMapId.."--845" );

		if teamMember.m_nPosition == 0 and teamMember.m_nOffLineTime == 0 and
			not CompareRoleName( teamMember.m_szName, mainplayer:getName() ) and teamMember.m_nMapId == nCurSeeMapID then
			button = getglobal( "WorldmapTeamPosBtn" .. nTeamCount );
			SetButtonInfo( { szName = button:GetName(), ClientString = GetPlayerName(teamMember.m_szName), 
										ClientUserData0 = teamMember.m_nMapId,
										ClientUserData1 = teamMember.m_nRealX, ClientUserData2 = teamMember.m_nRealY } );

			teamMapPosition = MaxMapControl:getPointCoordOnTex(  teamMember.m_nRealX, teamMember.m_nRealY );
			local nX = ( teamMapPosition.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom;
			local nY = ( teamMapPosition.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom;
			button:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", nX, nY );
			button:Show();

			nTeamCount = nTeamCount + 1;
		-- 显示队长信息
		elseif teamMember.m_nPosition == 1 and teamMember.m_nOffLineTime == 0 and
				not CompareRoleName( teamMember.m_szName, mainplayer:getName() ) and teamMember.m_nMapId == nCurSeeMapID then
			button = getglobal( "WorldmapCaptainPosBtn" );
			SetButtonInfo( { szName = button:GetName(), ClientString = GetPlayerName(teamMember.m_szName), 
										ClientUserData0 = teamMember.m_nMapId,
										ClientUserData1 = teamMember.m_nRealX, ClientUserData2 = teamMember.m_nRealY } );
		
			teamMapPosition = MaxMapControl:getPointCoordOnTex(  teamMember.m_nRealX, teamMember.m_nRealY );
			local nX = ( teamMapPosition.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom;
			local nY = ( teamMapPosition.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom;
			button:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", nX, nY );
			button:Show();
		end
	end
	
	-- 若主角是队长或队长和主角不在一张图上,则隐藏队长图标	
	if teamInfo.MemberNum > 0 and ( mainplayer:isCaptain() or WorldmapCaptainPosBtn:GetClientUserData( 0 ) ~= nCurSeeMapID ) then
		WorldmapCaptainPosBtn:Hide();
	end

	-- 隐藏余下未使用的队员button
	for i = nTeamCount, MAX_TEAM_MEMBER do
		local button = getglobal( "WorldmapTeamPosBtn" .. i );
		button:Hide();
	end
end

--@Desc:用于对显示分类区域的控件进行清空操作
function ClearMaxmapSceneNameButton()
	local nIndex = 1;
	while true do
		local button = getglobal( "SceneAreaNameBtn"..nIndex );
		if button == nil then
			break;
		end

		SetButtonInfo( { szName = "SceneAreaNameBtn"..nIndex, ClientString = "", ClientUserData0 = 0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
		button:Hide();
		nIndex = nIndex + 1;
	end
end

local t_monsterCanShow =  {	[7] = { desc = "BOSS" } };
--@Desc: 用于更新显示在地图上的NPC控件
function UpdateSceneButtonOnMap()
	-- 清空原来的控件信息并隐藏控件
	ClearMaxmapSceneNameButton();
	
	local MaxMapControl	= MapShower:GetMaxMapContal();
	local MapInfo		= MaxMapControl:updateMapInfo();
	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end
	
	local mapSize = MapShower:getMapSize( nTexID );
	nTexID = g_nCurSelectShowRealMapID >= MIN_COPY_ID and g_nCurSelectShowRealMapID or nTexID;
	-- NPC人数要确保都能显示出来
	for i = 1, MapShower:getMapSceneAreaCount( nTexID ) do
		local sceneAreaInfo		= MapShower:getMapSceneAreaInfo( nTexID, i - 1 );
		local nMonSubType		= getMonsterSubtype( sceneAreaInfo.MonsterID );
		local sceneAreaNameButton	= getglobal( "SceneAreaNameBtn" .. i ) or 
										CreateButton( "SceneAreaNameBtn" .. i, "SceneAreaNameBtnTemplate", 
											getglobal( "WorldMapFrame" ) );
		SetButtonInfo( { szName = sceneAreaNameButton:GetName(), ClientString = sceneAreaInfo.Name, 
								ClientUserData0 = 0,
								ClientUserData1 = sceneAreaInfo.Pos.X, ClientUserData2 = sceneAreaInfo.Pos.Y } );

		local texOffset	= getPointCoordOnTex( sceneAreaInfo.Pos.X, sceneAreaInfo.Pos.Y, mapSize.first, mapSize.second, nTexID );
		
		local tex = getglobal( "SceneAreaNameBtn" .. i .. "Tex" );
		-- 只有在副本的时候才显示骷髅头图标，世界boss
		if CanShowMonster( nMonSubType, t_monsterCanShow ) and ( nTexID >= MIN_COPY_ID or IsWorldBoss( sceneAreaInfo.MonsterID ) ) then
			tex:SetTexture( [[uires\ui\Za\boss  BiaoShi.tga]] );
			tex:SetTexRelUV( 1, 1 );
			tex:SetSize( 28, 37 );
			sceneAreaNameButton:SetSize( 28, 37 );
		else
			tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
			tex:SetTexUV( 390, 35, 17, 16 );
			tex:SetSize( 17, 16 );
			sceneAreaNameButton:SetSize( 17, 16 );
		end

		sceneAreaNameButton:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
		sceneAreaNameButton:Show();
	end
end

-- @Desc:用于对显示NPC的控件进行清空操作
function ClearMaxMapNpcPosBtn()
	local nIndex = 1;
	while true do
		local button = getglobal( "WorldmapNpcPosBtn"..nIndex );
		if button == nil then
			break;
		end

		SetButtonInfo( { szName = button:GetName(), ClientString = "", ClientUserData0 =0,
							ClientUserData1 = -1, ClientUserData2 = -1 } );
		button:Hide();
		nIndex = nIndex + 1;
	end
end

function SetNpcPosButtonTasTex( button, taskTex, npcId, npcTypeTex )
	if button == nil or npcId == 0 or not button:IsShown() then
		return;
	end
	
	local nMapId = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nMapId = g_nCurSelectShowRealMapID;
	else
		nMapId = g_nCurSelectShowReferMapID;
	end
	local npcInfo = MapShower:getNpcInfoById( nMapId, npcId );
	if npcInfo and npcInfo.m_szMapDesc ~= "" and IsInExistence( "uires\\TuPianLei\\NPC\\" .. npcInfo.m_szMapDesc .. ".tga" ) then
		npcTypeTex:SetTexture( "uires\\TuPianLei\\NPC\\" .. npcInfo.m_szMapDesc .. ".tga" );
		npcTypeTex:Show()
		if t_MapNpcSize[npcInfo.m_szMapDesc] ~= nil then
			npcTypeTex:SetSize( t_MapNpcSize[npcInfo.m_szMapDesc],19 )
			button:SetSize( t_MapNpcSize[npcInfo.m_szMapDesc],19 )
		else
			npcTypeTex:SetSize( 32,19 )
			button:SetSize( 32,19 )
		end
	else
		npcTypeTex:Hide()
		button:SetSize( 17,22 )
	end

	local nTaskChkBtn = getglobal( "WorldMapFrame_ShowTaskBtn" );
	if nTaskChkBtn:GetCheckState() then
		local nNPCstate	= getNpcQuestShow( npcId );
		-- 设置NPC位置按钮的任务图标
		if ( nNPCstate == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT ) then
			taskTex:SetTexUV( 508,45,17,16 )
			taskTex:SetSize( 17,16 );
			taskTex:Show();
		elseif ( nNPCstate == CLIENT_TASK_STAT_FINI ) then
			taskTex:SetTexUV( 472,45,17,22 )
			taskTex:SetSize( 17,22 );
			taskTex:Show();
		elseif ( nNPCstate == CLIENT_TASK_STAT_FAIL ) then
			taskTex:SetTexUV( 490,45,17,22 )
			taskTex:SetSize( 17,22 );
			taskTex:Show();
		elseif ( nNPCstate == CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT ) then
			taskTex:SetTexUV( 526,45,17,22 )
			taskTex:SetSize( 17,22 );
			taskTex:Show();
		elseif ( nNPCstate == CLIENT_TASK_STAT_CANNOTACCEPT ) then
			taskTex:SetTexUV( 989,874,17,22 );
			taskTex:SetSize( 17,22 );
			taskTex:Show();
		elseif not npcTypeTex:IsShown() then
			taskTex:SetTexUV( 390,51,17,16 )
			taskTex:SetSize( 15,15 );
			taskTex:Show();
		else
			taskTex:Hide()
		end
	elseif npcTypeTex:IsShown() then
		taskTex:Hide()
	end
end

function ClearNpcTaskTex()
	local nIndex = 1;
	while true do
		local button = getglobal( "WorldmapNpcPosBtn"..nIndex );
		if button == nil then
			break;
		end

		if button:IsShown() then
			local taskTex	= getglobal( button:GetName() .. "TaskNpcTex" );
			taskTex:SetTexUV( 390,51,17,16 )
			taskTex:SetSize( 15,15 );
		end

		nIndex = nIndex + 1;
	end
end

function UpdateNpcPosButtonTasTex( )
	local nTaskChkBtn = getglobal( "WorldMapFrame_ShowTaskBtn" );
	if not nTaskChkBtn:GetCheckState() then
		ClearNpcTaskTex()
	end
	local nIndex = 1;
	while true do
		local button = getglobal( "WorldmapNpcPosBtn"..nIndex );
		if button == nil then
			break;
		end

		if button:IsShown() then
			local taskTex	= getglobal( button:GetName() .. "TaskNpcTex" );
			local npcTypeTex = getglobal( button:GetName() .. "NpcTypeTex" );
			local npcId		= button:GetClientUserData( 0 );
			SetNpcPosButtonTasTex( button, taskTex, npcId, npcTypeTex );
		end

		nIndex = nIndex + 1;
	end
end

--@Desc: 用于更新显示在地图上的NPC控件
function UpdateNpcButtonOnMap()
	-- 清空原来的控件信息并隐藏控件
	ClearMaxMapNpcPosBtn();

	local button;
	local MaxMapControl		= MapShower:GetMaxMapContal();
	local MapInfo			= MaxMapControl:updateMapInfo();
	local t_curDisplayList	= t_gNpcTypeList[g_nCurNpcType];
	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end
	
	local mapSize = MapShower:getMapSize( nTexID );
	nTexID = g_nCurSelectShowRealMapID >= MIN_COPY_ID and g_nCurSelectShowRealMapID or nTexID;
	--NPC人数要确保都能显示出来
	for i = 1, table.getn( t_curDisplayList ) do
		button = getglobal( "WorldmapNpcPosBtn" .. i ) or 
					CreateButton( "WorldmapNpcPosBtn" .. i, "WorldmapNpcPosBtnTemplate", WorldMapFrame );
		--临时商人(混乱元素)区域地图中不要显示
		if t_curDisplayList[i].id ~= CHAOS_MONSTER_ID then 
			ShowNpcPosButton( button, t_curDisplayList[i].id, nTexID, mapSize );
		end
	end

	UpdateNpcPosButtonTasTex();
end

function WorldMapSetMoveNpcPosBtn( pNpc )
	if WorldMapFrame:IsShown() then
		local nMapId = 0;
		if g_nCurSelectShowReferMapID == 0 then
			nMapId = g_nCurSelectShowRealMapID;
		else
			nMapId = g_nCurSelectShowReferMapID;
		end

		if nMapId == pNpc:getMapId() then
			local mapSize = MapShower:getMapSize( nMapId );
			local t_curDisplayList	= t_gNpcTypeList[g_nCurNpcType];
			local button = getglobal( "WorldmapNpcPosBtn" .. #t_curDisplayList + 1 ) 
				or CreateButton( "WorldmapNpcPosBtn" .. #t_curDisplayList + 1, "WorldmapNpcPosBtnTemplate", WorldMapFrame );
			
			local posX,posY = pNpc:getPlayerX()/10, pNpc:getPlayerZ()/10;
			SetButtonInfo( { szName = button:GetName(), ClientString = pNpc:getName(), ClientUserData0 = 0,
								ClientUserData1 = posX, ClientUserData2 = posY } );

			local texOffset	= getPointCoordOnTex( posX, posY, mapSize.first, mapSize.second, nMapId );
			button:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
			if not button:IsShown() then
				local taskTex	= getglobal( button:GetName() .. "TaskNpcTex" );
				taskTex:SetTexUV( 390,51,17,16 )
				taskTex:SetSize( 15,15 );
				local npcTypeTex = getglobal( button:GetName() .. "NpcTypeTex" );
				npcTypeTex:Hide();
				button:Show();
				button:Enable();
			end
		end
	end
end

function ShowNpcPosButton( button, nNpcId, nMapId, mapSize )
	local npcInfo = MapShower:getNpcInfoById( nMapId, nNpcId );
	if not npcInfo then
		npcInfo = GameClanManager:getClanNpcInfoById(nNpcId);
	end
	if not npcInfo or not npcInfo.m_bShow then
		button:Hide();
		return;
	end
	
	local szFuncDesc = getNpcDesc( nNpcId );
	if szFuncDesc ~= "" then
		szFuncDesc = szFuncDesc.." ";
	end
	SetButtonInfo( { szName = button:GetName(), ClientString =  szFuncDesc..npcInfo.m_szName, 
						ClientUserData0 = nNpcId, ClientUserData1 = npcInfo.m_nRealX, ClientUserData2 = npcInfo.m_nRealY } );
	
	
	local texOffset	= getPointCoordOnTex( npcInfo.m_nRealX, npcInfo.m_nRealY, mapSize.first, mapSize.second, nMapId );
	button:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
	button:Show();
end

function getNpcTypeDesc( nId )
	local MaxMapControl = MapShower:GetMaxMapContal();
	if MaxMapControl:isNpcRaceType( nId, FUNCTION_NPC ) then
		--return t_gNpcTypeDesc[FUNCTION_NPC];
		return "功能";
	end

	if MaxMapControl:isNpcRaceType( nId, QUEST_NPC ) then
		--return t_gNpcTypeDesc[QUEST_NPC];
		return "任务";
	end 

	if MaxMapControl:isNpcRaceType( nId, ACT_NPC ) then
		--return t_gNpcTypeDesc[ACT_NPC];
		return "活动";
	end 

	if MaxMapControl:isNpcRaceType( nId, TRAN_NPC ) then
		--return t_gNpcTypeDesc[TRAN_NPC];
		return "传送";
	end 
end

function ClearMaxmapNpcNameButton()
	local npcName;
	local npcNameButton;
	local npcDesc;

	for i = 1, MAX_NPC_NAME do 
		npcName = getglobal( "NpcNameButton" .. i .. "Text" );
		npcDesc	= getglobal( "NpcNameButton" .. i .. "DescText" );
		npcName:SetText( "" );
		npcDesc:SetText( "" );
		npcName:SetClientUserData( 0, 0 );
		npcName:SetClientString( "" );

		npcNameButton = getglobal( "NpcNameButton" .. i );
		npcNameButton:Disable();
		npcNameButton:SetClientUserData( 0, 0 );
		npcNameButton:SetClientString( "" );
	end
end

--@Desc: 此为用于更新NPC名字列表
function UpdateNpcNameList( nStartTableLine )
	ClearMaxmapNpcNameButton();
	DisCheckAllBtn( "NpcNameButton", MAX_NPC_NAME );
	local nCurNameLine = 1;
	local t_curDisplayList = t_gNpcTypeList[g_nCurNpcType];
	local npcName;
	local npcNameButton;
	local npcDesc;
	local MaxMapControl	= MapShower:GetMaxMapContal();

	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end
	nTexID = g_nCurSelectShowRealMapID >= MIN_COPY_ID and g_nCurSelectShowRealMapID or nTexID;
	
	if g_nCurNpcType == MONOSTER then
		local slider			= getglobal( "WorldMapFrame_ScrollBar" );	
		local nStartTableLine	= slider:GetValue( ) + 1;
		-- 同时刷新怪物名字表
		local nSceneAreaCount	= MapShower:getMapSceneAreaCount( nTexID );
		ClearMaxmapNpcNameButton();
		for i = 1, nSceneAreaCount do
			if nStartTableLine > nSceneAreaCount then return end--要显示行数超过表中实际存储的内容
			
			if nCurNameLine > MAX_NPC_NAME then return end--若可显示控件都用尽
			
			-- 获取怪物的详细信息
			local sceneAreaInfo	= MapShower:getMapSceneAreaInfo( nTexID, nStartTableLine - 1 );	
			local nRealX		= sceneAreaInfo.Pos.X;
			local nRealY		= sceneAreaInfo.Pos.Y;
			local szName		= sceneAreaInfo.Name;

			npcNameButton = getglobal( "NpcNameButton" .. nCurNameLine );
			npcNameButton:SetClientUserData( 0, 0 );
			npcNameButton:SetClientUserData( 1, nRealX );
			npcNameButton:SetClientUserData( 2, nRealY );
			npcNameButton:SetClientString( szName );
			npcNameButton:Enable();
			if nCurSelectNpcName == nStartTableLine then
				npcNameButton:Checked();
			end
			
			npcDesc	= getglobal( "NpcNameButton" .. nCurNameLine .. "DescText" );
			--npcDesc:SetText( "" );
			npcDesc:SetText( "Lv."..getMonsterLevel( sceneAreaInfo.MonsterID ).." ".. szName );

			npcName = getglobal( "NpcNameButton" .. nCurNameLine .. "Text" );
			--npcName:SetText( szName );
			npcName:SetText( "" );
			npcName:SetClientString( szName );
			
			local nOffsetX = npcDesc:GetTextExtentWidth( npcDesc:GetText() ) / GetScreenScaleY() + 3;
			npcName:SetPoint( "topleft", npcDesc:GetName(), "topleft", nOffsetX, 0 );

			nStartTableLine = nStartTableLine + 1;
			nCurNameLine = nCurNameLine + 1;
		end
		return;
	end
	
	for i = nStartTableLine, table.getn( t_curDisplayList ) do
		if nCurNameLine > MAX_NPC_NAME then break end--若可显示控件都用尽
		npcNameButton = getglobal( "NpcNameButton" .. nCurNameLine );
		
		if ShowNpcNameButton( npcNameButton, nTexID, t_curDisplayList[i].id ) then
			if nCurSelectNpcName == i then
				npcNameButton:Checked();
			end		
			nCurNameLine	= nCurNameLine + 1;
		end
	end
end

function ShowNpcNameButton( button, nMapId, nNpcId )
	local npcInfo = MapShower:getNpcInfoById( nMapId, nNpcId );
	if not npcInfo then
		npcInfo = GameClanManager:getClanNpcInfoById(nNpcId);
	end
	if not npcInfo or not npcInfo.m_bShow then
		return false
	end

	SetButtonInfo( { szName = button:GetName(), ClientString = npcInfo.m_szName, 
							ClientUserData0 = nNpcId,
							ClientUserData1 = npcInfo.m_nRealX, ClientUserData2 = npcInfo.m_nRealY } );
	button:Enable();
	
	local npcDesc	= getglobal( button:GetName() .. "DescText" );
	local szNpcDesc = getNpcDesc( nNpcId );
	if szNpcDesc == "" then
		npcDesc:SetText( "" );
	else
		npcDesc:SetText( szNpcDesc );
	end

	local npcName = getglobal( button:GetName() .. "Text" );
	local szNpcTypeDesc = getNpcTypeDesc( nNpcId );
	npcName:SetText( npcInfo.m_szName  );
	npcName:SetClientUserData( 0, nNpcId );
	npcName:SetClientString( npcInfo.m_szName );
	
	local nOffsetX = npcDesc:GetTextExtentWidth( npcDesc:GetText() ) / GetScreenScaleY() + 3;
	npcName:SetPoint( "topleft", npcDesc:GetName(), "topleft", nOffsetX, 0 );
	return true;
end

--@Desc:设置滚动条的位置
function SetMaxMapScrollBar( t_npcList )
	--确定技能学习面板显示的具体技能
	local slider		= getglobal( "WorldMapFrame_ScrollBar" );
	local nTableNum		= table.getn( t_npcList );--此当前表中可显示的内容的行数
	local nNum			= nTableNum - MAX_NPC_NAME;
	
	if nNum > 0  then
		slider:SetMaxValue( nNum );
		slider:Show();
	else
		slider:SetMaxValue( 1 );
		slider:Hide();
	end
	
	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );
	slider:SetPoint( "top", "WorldMapFrame_ScrollUpBtn", "bottom", -1, 0 );

	-- 在学习了技能以后,要特别检测用的,以防止
	if slider:GetValue() > slider:GetMaxValue( ) then
		slider:SetValue( slider:GetMaxValue( ) );
	end
end

--@Desc: 此为点击向上按钮的时候响应
function MaxMapFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "WorldMapFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc: 此为点击向下按钮的时候响应
function MaxMapFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( "WorldMapFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:此为滚动条滚动的时候响应
function MaxMapFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "WorldMapFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	
	if nValue <= nMaxValue then
		UpdateNpcNameList( nValue + 1 );
	end
end

function GetSelectPosButton( nameBtn )
	local button;
	local nIndex = 1;
	-- 若显示的是NPC
	if g_nCurNpcType ~= MONOSTER then
		while true do
			button = getglobal( "WorldmapNpcPosBtn"..nIndex );
			if button == nil then
				break;
			end

			if button:IsShown() and button:GetClientString() == nameBtn:GetClientString() then
				break;
			end

			nIndex = nIndex + 1;
		end

		return button;
	end

	-- 若显示的是怪物
	while true do
		button = getglobal( "SceneAreaNameBtn"..nIndex );
		if button == nil then
			break;
		end

		if button:GetClientString() == nameBtn:GetClientString() then
			break;
		end

		nIndex = nIndex + 1;
	end

	return button;
end

--@Desc: 此为双击地图右边NPC名字的时候响应 --TODO:调整闪烁效果与NPC按钮对应
function WorldMapFrame_NpcName_OnDoubleClick()
	--先找到存储当前指定NPC对应的显示控件
	local button = GetSelectPosButton( this );

	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	
	--自动寻路
	local nNpcId = this:GetClientUserData( 0 );
	MoveToDestPoint( { npcId = nNpcId, realX = this:GetClientUserData( 1 ), realY = this:GetClientUserData( 2 ), mapId = nTexID } );
end

--@Desc:此为点击NPC名字的时候响应
function WorldMapFrame_NpcName_OnClick()
	for i = 1, MAX_NPC_NAME do
		local btn = getglobal( "NpcNameButton" .. i );
		btn:DisChecked();
	end
	
	this:Checked();

	local slider		= getglobal( "WorldMapFrame_ScrollBar" );	
	nCurSelectNpcName	= this:GetClientID() + slider:GetValue( );
end

-- @Desc: 此为点击NPC类型按钮的时候响应
function Maxmap_NPC_OnClick()
	-- 若点击的按钮时已被选定的按钮,则对点击操作没反应
	if g_nCurNpcType == t_gDisplayButton[this:GetName()] then
		return;
	end

	-- 设置按钮的disable和enable状态
	local button;
	for name, _ in pairs( t_gDisplayButton ) do
		button = getglobal( name );
		if not button:IsEnable() then
			button:Enable();
			button:DisHighlight();
		end
	end
	
	button = getglobal( this:GetName() );
	button:Disable();
	
	-- 设置当前被选定的NPC页面类型
	g_nCurNpcType = t_gDisplayButton[this:GetName()];
	
	nCurSelectNpcName = 0;
	DisCheckAllBtn( "NpcNameButton", MAX_NPC_NAME );
	-- 设置滚动条和地图上NPC显示内容
	local slider = getglobal( "WorldMapFrame_ScrollBar" );
	SetMaxMapScrollBar( t_gNpcTypeList[g_nCurNpcType] );
	if slider:IsShown() then
		UpdateNpcNameList( slider:GetValue() + 1 ); 
	else
		UpdateNpcNameList( 1 );
	end
	-- 检查是否要隐藏怪物图标
	if g_nCurNpcType ~= ALL_NPC and g_nCurNpcType ~= MONOSTER then
		ClearMaxmapSceneNameButton();
	else
		UpdateSceneButtonOnMap();
	end
	
	UpdateNpcButtonOnMap();
end

function Maxmap_Monster_OnClick()	
	-- 若点击的按钮时已被选定的按钮,则对点击操作没反应
	if g_nCurNpcType == MONOSTER then
		return;
	end
	
	-- 设置按钮的disable和enable状态
	local button;
	for name, _ in pairs( t_gDisplayButton ) do
		button = getglobal( name );
		if not button:IsEnable() then
			button:Enable();
			button:DisHighlight();
		end
	end
	
	button = getglobal( "WorldMapFrame_MonsterButton" );
	button:Disable();
	
	local nTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nTexID = g_nCurSelectShowRealMapID;
	else
		nTexID = g_nCurSelectShowReferMapID;
	end

	-- 设置当前被选定的页面类型
	g_nCurNpcType = MONOSTER;

	-- 若点击怪物按钮后,先隐藏所有NPC点
	ClearMaxMapNpcPosBtn();
	
	-- 再显示所有怪物点
	UpdateSceneButtonOnMap();
	local MaxMapControl		= MapShower:GetMaxMapContal();
	local nSceneAreaCount	= MapShower:getMapSceneAreaCount( nTexID );
	local nCurNameLine		= 0;

	--NPC人数要确保都能显示出来
	local slider			= getglobal( "WorldMapFrame_ScrollBar" );	
	local nStartTableLine	= slider:GetValue() + 1;
	t_gNpcTypeList[g_nCurNpcType] = {};
	-- 同时刷新怪物名字表
	ClearMaxmapNpcNameButton();
	nCurSelectNpcName = 0;
	DisCheckAllBtn( "NpcNameButton", MAX_NPC_NAME );
	for i = 1, nSceneAreaCount do
		-- 要显示行数超过表中实际存储的内容
		if nStartTableLine > nSceneAreaCount then 
			break;
		end

		nCurNameLine = nCurNameLine + 1;
--[[
		-- 若可显示控件都用尽
		if nCurNameLine > MAX_NPC_NAME then 
			break; 
		end
]]--
		local sceneAreaInfo		= MapShower:getMapSceneAreaInfo( nTexID, nStartTableLine - 1 );
		local nRealX			= sceneAreaInfo.Pos.X;
		local nRealY			= sceneAreaInfo.Pos.Y;
		local szName			= sceneAreaInfo.Name;
		local oneMonsterInfo	=	{	["id"]		= 0,		["name"]	= szName, ["realX"] = nRealX , 
										["realY"]	= nRealY,	["level"]	= getMonsterLevel( sceneAreaInfo.MonsterID )
									};
		table.insert( t_gNpcTypeList[g_nCurNpcType], oneMonsterInfo );

		nStartTableLine = nStartTableLine + 1;
	end

	table.sort( t_gNpcTypeList[g_nCurNpcType], 
				function ( lhs, rhs )
					if lhs["level"] == rhs["level"] then
						return lhs["name"] < rhs["name"];
					else
						return lhs["level"] < rhs["level"];
					end
				end );
	
	local t_monsterList = t_gNpcTypeList[g_nCurNpcType];
	for i = 1, table.getn( t_monsterList ) do		
		if i > MAX_NPC_NAME then 
			break; 
		end
		-- 获取怪物的详细信息
		local oneMonsterInfo	= t_monsterList[i];
		local nRealX			= oneMonsterInfo["realX"];
		local nRealY			= oneMonsterInfo["realY"];
		local szName			= oneMonsterInfo["name"];

		local npcNameButton = getglobal( "NpcNameButton" .. i );
		npcNameButton:SetClientUserData( 0, 0 );
		npcNameButton:SetClientUserData( 1, nRealX );
		npcNameButton:SetClientUserData( 2, nRealY );
		npcNameButton:SetClientString( szName );
		npcNameButton:Enable();
		
		local npcDesc	= getglobal( "NpcNameButton" .. i .. "DescText" );
		--npcDesc:SetText( "" );
		npcDesc:SetText( "Lv."..oneMonsterInfo["level"].." ".. szName );

		local npcName = getglobal( "NpcNameButton" .. i .. "Text" );
		--npcName:SetText( szName );
		npcName:SetText( "" );
		npcName:SetClientString( szName  );
		npcName:Show();
		
		local nOffsetX = npcDesc:GetTextExtentWidth( npcDesc:GetText() ) / GetScreenScaleY() + 3;
		npcName:SetPoint( "topleft", npcDesc:GetName(), "topleft", nOffsetX, 0 );		
	end

	SetMaxMapScrollBar( t_gNpcTypeList[g_nCurNpcType] );
end

function MaxmapOnClose()
	local frame = getglobal( "WorldMapFrame" );
	frame:Hide();
end

-- 区域选项下拉菜单
function WorldMapFrame_AreaDownBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end

	if WorldmapSelectNameFrame:IsShown() then
		WorldmapSelectNameFrame:Hide();
	else
		WorldmapSelectNameFrame:SetClientString( "WorldMapFrame_AreaSelectBtn" );
		WorldmapSelectNameFrame:SetPoint( "topleft", "WorldMapFrame_AreaSelectBtn", "bottomleft", 0, 0 );
		WorldmapSelectNameFrame:Show();
	end
end

-- 副本选项下拉菜单
function WorldMapFrame_CopyDownBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end

	if WorldmapSelectNameFrame:IsShown() then
		WorldmapSelectNameFrame:Hide();
	else
		WorldmapSelectNameFrame:SetClientString( "WorldMapFrame_CopySelectBtn" );
		WorldmapSelectNameFrame:SetPoint( "topleft", "WorldMapFrame_CopySelectBtn", "bottomleft", 0, 0 );
		WorldmapSelectNameFrame:Show();
	end
end

function WorldmapAreaPosBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	
	local t_hideFrame = { "GameTooltip", "WorldMapGameTooltip" };
	for _, name in ipairs( t_hideFrame ) do
		local tipsFrame = getglobal( name );
		if tipsFrame:IsShown() then
			tipsFrame:Hide();
		end
	end

	local mapInfo = MapShower:getMapInfoByName( this:GetClientString() );
	Quest:updateNpcQuestTitle( mapInfo.m_nMapID, mapInfo.m_nReferMapID );
	ChangeAreaMap( mapInfo.m_nReferMapID, mapInfo.m_nMapID );
	GameTooltip:Hide();
end

local t_adviseContent = {	
							[3400]	= "建议冒险等级44~59级",
							[2400]	= "建议冒险等级40~44级",
							[2000]	= "建议冒险等级0~10级",							
							[1000]	= "时间城",
							[2200]	= "建议冒险等级20~29级",
							[2201]	= "建议冒险等级70~73级",							
							[3200]	= "建议冒险等级74~77级",
							[3100]	= "建议冒险等级63~66级",
							[2104]	= "建议冒险等级60~63级",							
							[2300]	= "建议冒险等级30~33级",
							[3300]	= "建议冒险等级34~37级",
							[3303]	= "建议冒险等级34~38级",
							[2500]	= "建议冒险等级50~54级",
							[3500]	= "建议冒险等级53~58级",
							[3402]	= "建议冒险等级80-85级",
							[2100]	= "建议冒险等级10~19级",
							[3404]	= "建议冒险等级75~78级",
							[3304]	= "建议冒险等级79~83级",
							[3302]	= "建议冒险等级38~42级",
							[3502]	= "建议冒险等级55~59级",
							[2202]	= "建议冒险等级18~26级",
							[3403]	= "建议冒险等级94~99级",
							[3410]	= "建议冒险等级40~54级",
							[3510]	= "建议冒险等级50~64级",
							[3110]	= "建议冒险等级60~74级",
							[3210]	= "建议冒险等级70~80级",
							[3211]	= "建议冒险等级65~80级",
							[4500]	= "建议冒险等级80~85级",
						};
function WorldmapAreaPosBtn_OnEnter()	
	local mapInfo = MapShower:getMapInfoByName( this:GetClientString() );
	local nTexID		= 0;
	if mapInfo.m_nReferMapID == 0 then
		nTexID = mapInfo.m_nMapID;
	else
		nTexID =  mapInfo.m_nReferMapID;
	end

	if t_adviseContent[nTexID] == nil and MapShower:getMapSceneAreaCount( nTexID ) == 0 then
		return;
	end

	local szText = t_adviseContent[nTexID] ~= nil and t_adviseContent[nTexID] or "";
	
	local t_areaInfo = {};
	for i = 1, MapShower:getMapSceneAreaCount( nTexID ) do
		local sceneInfo = MapShower:getMapSceneAreaInfo( nTexID, i - 1 );
		table.insert( t_areaInfo, { level = getMonsterLevel( sceneInfo.MonsterID ), name = sceneInfo.Name } );
		--szText = szText .. "\n" .. "Lv."..getMonsterLevel( sceneInfo.MonsterID )..sceneInfo.Name;
	end
	
	table.sort( t_areaInfo, 
	function ( lhs, rhs )
		return lhs["level"] < rhs["level"];
	end
	);

	for i = 1, table.getn( t_areaInfo ) do
		local elemData = t_areaInfo[i];
		szText = szText .. "\n" .. "Lv."..elemData["level"]..elemData["name"];
	end
	
	ShowGameTooltip( { rich = "WorldMapGameTooltipRichText", tipsFrame = "WorldMapGameTooltip", text = szText, frame = "WorldMapFrame",
						button = this:GetName() } );
end

function WorldmapAreaPosBtn_OnLeave()
	GameTooltip:Hide();
	WorldMapGameTooltip:Hide();
end

function MaxmapOnHide()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	--将所有的按钮状态置为普通状态,而不是高亮状态
	local button;
	for name, _ in pairs( t_gDisplayButton ) do
		button = getglobal( name );
		if not button:IsEnable() then
			button:Enable();
			button:DisHighlight();
		end
	end
	
	ClearMaxmapNpcNameButton();
	ClearMaxMapNpcPosBtn();
	
	if GameTooltip:IsShown() and GameTooltip:GetClientString() == "WorldMapFrame" then
		GameTooltip:Hide();
	end

	-- 将所有处于高亮状态的button都dishighlight
	for i = 1, MAX_NPC_NAME do
		local button = getglobal( "NpcNameButton" .. i );
		button:DisHighlight();
	end
	
	WorldmapSelectNameFrame:Hide();
	if UIIsInDragState() and UIMSG:getUIGV().DragFrom == DRAG_CLAN_CITY then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	WorldMapFrameShowAttackPosBtn:Hide();
	WorldMapFrameShowVenuePosBtn:Hide();
end

function WorldMapFrame_XCoordInputEdit_OnUpdate()
	if tonumber( this:GetText() ) == nil then return end

	local mapWidth = MapShower:GetMaxMapContal():getMapWidth();
	if tonumber ( this:GetText() ) > mapWidth / 100  then
		this:SetText( tostring(  mapWidth / 100 ) );
	end
	CheckEditNumberInput( this );
end

function WorldMapFrame_YCoordInputEdit_OnUpdate()
	if tonumber( this:GetText() ) == nil then return end

	-- 考虑未输入任何值的情况
	local mapHeight = MapShower:GetMaxMapContal():getMapHeight();
	
	if tonumber( this:GetText() ) > mapHeight / 100  then
		this:SetText( tostring(  mapHeight / 100 ) );
	end
	CheckEditNumberInput( this );
end

--@Desc: 此为点击坐标旁边的自动寻径按钮的时候响应
function Maxmap_AutoFindPath_OnClick()
	if WorldMapFrame_YCoordInputEdit:GetText() == "" or WorldMapFrame_XCoordInputEdit:GetText() == "" then
		return;
	end

	local nXOffset = tonumber( WorldMapFrame_XCoordInputEdit:GetText() ) * 100;
	local nYOffset = tonumber( WorldMapFrame_YCoordInputEdit:GetText() ) * 100;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end

	local nCurSelectTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nCurSelectTexID = g_nCurSelectShowRealMapID;
	else
		nCurSelectTexID = g_nCurSelectShowReferMapID;
	end

	MapShower:GetMaxMapContal():linkMoveto( nCurSelectTexID, nXOffset, nYOffset );	
	WorldMapFrame_XCoordInputEdit:Clear();
	WorldMapFrame_YCoordInputEdit:Clear();
	
end

--@Desc: 此为点击自动寻径按钮的时候响应
function Maxmap_AutoFindPath_OnClick()
	if WorldMapFrame_YCoordInputEdit:GetText() == "" or WorldMapFrame_XCoordInputEdit:GetText() == "" then
		return;
	end

	local nXOffset = tonumber( WorldMapFrame_XCoordInputEdit:GetText() ) * 100;
	local nYOffset = tonumber( WorldMapFrame_YCoordInputEdit:GetText() ) * 100;
	local mainplayer = ActorMgr:getMainPlayer();

	local nCurSelectTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nCurSelectTexID = g_nCurSelectShowRealMapID;
	else
		nCurSelectTexID = g_nCurSelectShowReferMapID;
	end

	MapShower:GetMaxMapContal():linkMoveto( nCurSelectTexID, nXOffset, nYOffset );	
	WorldMapFrame_XCoordInputEdit:Clear();
	WorldMapFrame_YCoordInputEdit:Clear();
	
end

function WorldMapFrame_AutoFindPathByNameButton_OnClick()
	-- 查找的NPC与主角必须在同一张地图上面
	local szNpcName = WorldMapFrame_NpcNameInputEdit:GetText();
	local MaxMapControl = MapShower:GetMaxMapContal();
	local npc	= MapShower:getNpcInfoByName( MaxMapControl:getMapID(), szNpcName );
	local edit = getglobal( "WorldMapFrame_NpcNameInputEdit" );
	edit:Clear();
	
	-- 若在本张地图没找到对应的NPC则不作任何处理
	if npc == nil then
		ShowMidTips( "所寻找的NPC不在主角所在地图上" );
		return;
	end
	MaxMapControl:linkMoveto( MapShower:getRealMapId(), npc.m_nId );
end

function WorldMapFrame_ShowTaskBtn_OnClick()
	UpdateNpcPosButtonTasTex();
end

function SetMainPlayerCoord()
	local x = GetCursorPosX();
	local y = GetCursorPosY();

	local nx = WorldMapFrameMapTexture:GetRealLeft();
	local ny = WorldMapFrameMapTexture:GetRealTop();

	local realx = x - nx ;
	local realy = y - ny ;

	local cursorPostion = getMouseCurPos( realx, realy );
	if cursorPostion.x < 0 or cursorPostion.y < 0 then
		return;
	end

	local szText = ( math.ceil( cursorPostion.x / 100 ) ) .. "，" .. ( math.ceil( cursorPostion.y / 100 ) );
	WorldMapFrame_MainCoordFont:SetText( "鼠标坐标：" .. szText );	
end

function WorldMapFrame_ShowMapBtn_OnEnter()
	SetMainPlayerCoord();
end

function WorldMapFrame_WorldBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end

	ShowWorldMap();
	g_nCurSelectShowRealMapID	= 0;
	g_nCurSelectShowReferMapID	= 0
end

function WorldMapFrame_ResourceBtn_OnEnter()
	local mapId = this:GetClientUserData(0);
	local mapOreDef = MapShower:getMapOreDef( mapId );
	if mapOreDef ~= nil then
		local szText = "此地图可获得资源：#c00dcdc";
		for i=1, MAX_MAP_ORE_LIST do
			if mapOreDef.OreList[i-1].MonsterID ~= 0 then
				szText = szText .. "\n" .. getMonsterName( mapOreDef.OreList[i-1].MonsterID );
			end
		end
		SetGameTooltips( this:GetName(), szText );
	end
end

function WorldMapFrame_ShowMapBtn_OnClick()
	if arg1 == "RightButton" then
		ShowWorldMap();
		g_nCurSelectShowRealMapID	= 0;
		g_nCurSelectShowReferMapID	= 0
	end
end

function WorldMapFrame_ShowMapBtn_OnMouseMove()
	SetMainPlayerCoord();
	
end

function WorldMapFrame_ShowMapBtn_OnLeave()
	WorldMapFrame_MainCoordFont:SetText( "" );
end

--@Desc: 此为鼠标点击区域地图的时候响应
function Maxmap_OnMouseDown()
	if WorldMapFrame_ShowWorldBtn:IsShown() then
		return;
	end

	if UIIsInDragState() and UIMSG:getUIGV().DragFrom == DRAG_CLAN_CITY then
		WorldMapFrame_ShowMapBtn_ReceiveDrag();
		return;
	end

	local MaxMapControl = MapShower:GetMaxMapContal();
	if arg1 == "LeftButton" then 
		if not isPointInFrame( "WorldMapFrameMapTexture" ) then
			ShowMidTips( "所选择位置不可达" );
			return;
		end
		WorldMapFrame_AllButton:Disable();
		local nCurMouseClickPosX = GetCursorPosX();
		local nCurMouseClickPosY = GetCursorPosY();

		local nMapTextureRealLeft	= WorldMapFrameMapTexture:GetRealLeft();
		local nMapTextureRealTop	= WorldMapFrameMapTexture:GetRealTop();
		local realx = nCurMouseClickPosX - nMapTextureRealLeft;--获得当前鼠标点击的点在对应视口的坐标值
		local realy = nCurMouseClickPosY - nMapTextureRealTop;
		
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		
		local nMapValidNormalizeTextWidth	=  t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].mapValidNormalizeTextureWidth 
												or g_nMapTextureWidth;
		local nMapValidNormalizeTextHeight	= t_specialMapDesc[nMapID] ~= nil and t_specialMapDesc[nMapID].mapValidNormalizeTextureHeight 
												or g_nMapTextureHeight;
		if ( realx / g_fZoom > nMapValidNormalizeTextWidth ) or
			( realy / g_fZoom > nMapValidNormalizeTextHeight ) then
				return;
		end
		
		local nTexID = 0;
		if g_nCurSelectShowReferMapID == 0 then
			nTexID = g_nCurSelectShowRealMapID;
		else
			nTexID = g_nCurSelectShowReferMapID;
		end
		local destPosition	= getMouseCurPos( realx, realy );
		local mapSize		= MapShower:getMapSize( nTexID );
		
		if destPosition.x < 0 or destPosition.x > mapSize.first or destPosition.y < 0 or destPosition.y > mapSize.second then
			ShowMidTips( "所选择位置不可达" );
			return;
		end

		if MaxMapControl:getMapID() ~= nTexID then
			if nTexID >= MIN_COPY_ID then
				ShowMidTips( "不可以自动寻路到达副本" );
				return;
			else
				ChangeAreaMap( MaxMapControl:getMapID(), MapShower:getRealMapId() );
				MaxMapControl:linkMoveto( nTexID,  destPosition.x, destPosition.y );				
			end		

		else	-- 若在相同的地图上
			MaxMapControl:linkMoveto( nTexID,  destPosition.x, destPosition.y );
		end

		
		-- 要处理当点击的范围超出地图的实际有效区域的情况
	end
	
	-- TODO: 右键是否要停止移动
	if (arg1 == "RightButton") then
		MaxMapControl:endMoveMap();
	end
end

function SetMapDisplayInfo()
	local MaxMapControl = MapShower:GetMaxMapContal();
	MaxMapControl:DrawPoint( "MapLine" );
	MaxMapControl:setMapZoom( g_fZoom );

	local mainplayer = ActorMgr:getMainPlayer();
	local MapInfo		= MaxMapControl:updateMapInfo();
	local szMapName		= MaxMapControl:getMapName( MaxMapControl:getMapID() );
	
	local slider = getglobal( "WorldMapFrame_ScrollBar" );
	local nValue = slider:GetValue();

	--用于调整在区域地图上代表主角玩家的图标方向
	local mainplayerPosition = MaxMapControl:getPointScreenCoord( mainplayer:getPlayerX(), mainplayer:getPlayerZ(), "WorldMapFrameMapTexture", 
																	10 );
	WorldmapMainShowButton:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", 
					mainplayerPosition.first, 
					mainplayerPosition.second );

	WorldmapMainShowButton:SetAngle( MapInfo.fAngle );
	WorldmapMainShowButton:Show();

	-- 更新队伍成员的位置信息
	UpdateTeamButtonOnMap();
end

--@Desc: 此为用于每秒刷新区域地图显示信息
function Maxmap_OnUpdate()
	-- 若当前正在显示世界地图或者正在查看其他区域地图，则不更新地图上面的任何信息
	if WorldMapFrame_ShowWorldBtn:IsShown() then
		return;
	end
	UpdateMapOres();
	
	UpdateSuDiButtonOnMap();
	UpdateStallBiaoJiButtonOnMap();
	UpdateMachineWarBossOnMap();
	UpdateClanWarCampMonOnMap();
	-- 更新重要数据
	UpdateCampPlayerOnMap();
	if bSeeOtherAreaMap then
		UpdateTeamButtonOnMap();
		return;
	end

	SetMapDisplayInfo();

	CheckCityMemberPosInfo();
	CheckBaseWarClanMemPosInfo();
end

function DrawEffect( x, y, bPlayEmotion )
	-- 若当前正在显示世界地图或者正在查看其他区域地图，则不更新地图上面的任何信息
	if WorldMapFrame_ShowWorldBtn:IsShown() or bSeeOtherAreaMap then
		return;
	end

	if bPlayEmotion == 1 then
		Maxmap_LastAutoLineButton:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", x - WorldMapFrameMapTexture:GetRealLeft(), 
						y - WorldMapFrameMapTexture:GetRealTop() );
		Maxmap_LastAutoLineButton:Show();
	elseif bPlayEmotion == 0 then
		Maxmap_LastAutoLineButton:Hide();
	end
end

--@Desc: 此为鼠标靠近地图上的NPC时响应,弹出对应的TIPS
function Maxmap_PosBtn_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
						button = this:GetName() } );
	SetMainPlayerCoord();
end

function Maxmap_WorldmapSuDiBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if UIIsInDragState() and UIMSG:getUIGV().DragFrom == DRAG_CLAN_CITY then
		WorldMapFrame_ShowMapBtn_ReceiveDrag();
		return;
	end

	--自动寻路
	local mainplayer = ActorMgr:getMainPlayer();	
	local MaxMapControl = MapShower:GetMaxMapContal();
	local nCurSelectTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nCurSelectTexID = g_nCurSelectShowRealMapID;
	else
		nCurSelectTexID = g_nCurSelectShowReferMapID;
	end
	
	-- 检查若要切换地图，则将上一张地图所显示的tips内容隐藏
	if nCurSelectTexID ~= MaxMapControl:getMapID() and GameTooltip:GetClientString( ) == this:GetParent() then
		GameTooltip:Hide();
	end
	MoveToDestPoint( { npcId = 0, realX = this:GetClientUserData( 1 ), 
						realY = this:GetClientUserData( 2 ), mapId = this:GetClientUserData( 0 ) } );
end

--@Desc: 此为鼠标离开地图上的NPC时响应,隐藏对应的NPC的TIPS
function Maxmap_PosBtn_OnLeave()
	GameTooltip:Hide();
end

function WorldmapMainShowButton_OnEnter()
	SetMainPlayerCoord();
end

--@Desc: 此为地图中的特效自动闪动
function Maxmap_SelectNpcPos_OnUpdate()
	if os.difftime( os.clock(), g_nMouseClickLastTime ) > 5 then
		g_nMouseClickLastTime = os.clock();
		this:Hide();
	end

	if g_nTargetTexNum == 1 then
		Maxmap_SelectNpcTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\1.tga");
		g_nTargetTexNum = 2;
	elseif  g_nTargetTexNum == 2 then
		Maxmap_SelectNpcTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\2.tga");
		g_nTargetTexNum = 3;
	elseif g_nTargetTexNum == 3 then
		Maxmap_SelectNpcTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\3.tga");
		g_nTargetTexNum = 4;
	elseif g_nTargetTexNum == 4 then
		Maxmap_SelectNpcTexture:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\4.tga");
		g_nTargetTexNum = 1;
	end
end

function Maxmap_AutoEffect_OnUpdate()
	local tex = getglobal( this:GetName() .. "Texture" );
	if g_nAutoTexNum == 1 then
		tex:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\1.tga");
		g_nAutoTexNum = 2;
	elseif  g_nAutoTexNum == 2 then
		tex:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\2.tga");
		g_nAutoTexNum = 3;
	elseif g_nAutoTexNum == 3 then
		tex:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\3.tga");
		g_nAutoTexNum = 4;
	elseif g_nAutoTexNum == 4 then
		tex:SetTexture("uires\\ZhuJieMian\\XiaoDiTu\\XiaoDiTu\\npc  TeXiaoZhenDongHua\\4.tga");
		g_nAutoTexNum = 1;
	end
end

function WorldmapTeamPosBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if UIIsInDragState() and UIMSG:getUIGV().DragFrom == DRAG_CLAN_CITY then
		WorldMapFrame_ShowMapBtn_ReceiveDrag();
		return;
	end

	--自动寻路
	local mainplayer = ActorMgr:getMainPlayer();
	
	local MaxMapControl = MapShower:GetMaxMapContal();
	local nCurSelectTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nCurSelectTexID = g_nCurSelectShowRealMapID;
	else
		nCurSelectTexID = g_nCurSelectShowReferMapID;
	end
	
	-- 检查若要切换地图，则将上一张地图所显示的tips内容隐藏
	if nCurSelectTexID ~= MaxMapControl:getMapID() and GameTooltip:GetClientString( ) == this:GetParent() then
		GameTooltip:Hide();
	end
	MoveToDestPoint( { npcId = 0, realX = this:GetClientUserData( 1 ), 
						realY = this:GetClientUserData( 2 ), mapId = nCurSelectTexID } );
end

--@Desc: 此为鼠标点击对应NPC时候响应,以显示相应的特效
function Maxmap_PosBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if UIIsInDragState() and UIMSG:getUIGV().DragFrom == DRAG_CLAN_CITY then
		WorldMapFrame_ShowMapBtn_ReceiveDrag();
		return;
	end

	--自动寻路
	local mainplayer = ActorMgr:getMainPlayer();	
	local MaxMapControl = MapShower:GetMaxMapContal();
	local nCurSelectTexID = 0;
	if g_nCurSelectShowReferMapID == 0 then
		nCurSelectTexID = g_nCurSelectShowRealMapID;
	else
		nCurSelectTexID = g_nCurSelectShowReferMapID;
	end
	
	-- 检查若要切换地图，则将上一张地图所显示的tips内容隐藏
	if nCurSelectTexID ~= MaxMapControl:getMapID() and GameTooltip:GetClientString( ) == this:GetParent() then
		GameTooltip:Hide();
	end
	MoveToDestPoint( { npcId = this:GetClientUserData( 0 ), realX = this:GetClientUserData( 1 ), 
						realY = this:GetClientUserData( 2 ), mapId = nCurSelectTexID } );
end

function MoveToDestPoint( destInfo )
	local MaxMapControl = MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() ~= destInfo.mapId then
		if destInfo.mapId >= MIN_COPY_ID then
			ShowMidTips( "不可以自动寻路到达副本" );
			return;
		else
			ChangeAreaMap( MaxMapControl:getMapID(), MapShower:getRealMapId() );
			if destInfo.npcId == 0 then
				MaxMapControl:linkMoveto( destInfo.mapId, destInfo.realX, destInfo.realY );
			else
				MaxMapControl:linkMoveto( destInfo.mapId, destInfo.npcId );
			end
		end	
	else
		if destInfo.npcId == 0 then
			MaxMapControl:linkMoveto( destInfo.mapId, destInfo.realX, destInfo.realY );
		elseif destInfo.realX and destInfo.realX > 0 and destInfo.realY and destInfo.realY > 0 then
			MaxMapControl:linkMoveto( destInfo.mapId, destInfo.realX, destInfo.realY )
		else
			MaxMapControl:linkMoveto( destInfo.mapId, destInfo.npcId );
		end
	end
end

-- 点击地区名字选择框的向上按钮
function WorldmapSelectNameFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "WorldmapSelectNameFrame_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

-- 点击地区名字选择框的向下按钮
function WorldmapSelectNameFrame_ScrollDownBtn_OnClick()
	local slider = getglobal("WorldmapSelectNameFrame_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	local nValue	= slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function WipeOffNumber( szContent )
	local szWipeContent;
	for i = string.len( szContent ),1, -1 do
		if tonumber( string.sub( szContent, i, i ) ) == nil  then
			szWipeContent = string.sub( szContent, 1, i );
			break;
		end
	end

	return szWipeContent == nil and szContent or szWipeContent;
end

-- 用于刷新面板所显示的地图名字
function UpdateWorldmapSelectName( nStartLine )
	ClearWorldmapSelectNameBtn();

	local nIconIndex = 1;

	if WorldmapSelectNameFrame:GetClientString() == "WorldMapFrame_AreaSelectBtn" then
		for i = nStartLine, table.getn( t_WorldArea ) do
			if nIconIndex > MAX_SELECT_NAME then
				break;
			end

			if t_WorldArea[i].MapID < MIN_COPY_ID then
				local button =  getglobal( "WorldmapSelectNameBtn" .. nIconIndex );
				local font	 = 	getglobal( "WorldmapSelectNameBtn" .. nIconIndex .. "AreaName" );
				button:Enable();
				button:SetClientString( t_WorldArea[i].MapName );
				font:SetText( WipeOffNumber( t_WorldArea[i].MapName ) );
				nIconIndex = nIconIndex + 1;
			end
		end

	elseif WorldmapSelectNameFrame:GetClientString() == "WorldMapFrame_CopySelectBtn" then
		for i = nStartLine, table.getn( t_WorldCopy ) do
			if nIconIndex > MAX_SELECT_NAME then
				break;
			end

			if t_WorldCopy[i].MapID >= MIN_COPY_ID then
				local button =  getglobal( "WorldmapSelectNameBtn" .. nIconIndex );
				local font	 = 	getglobal( "WorldmapSelectNameBtn" .. nIconIndex .. "AreaName" );
				button:Enable();
				button:SetClientString( t_WorldCopy[i].MapName );
				font:SetText( WipeOffNumber( t_WorldCopy[i].MapName ) );
				nIconIndex = nIconIndex + 1;
			end
		end
	end
end

-- 地区名字选择框的滚动条滚动的时候响应
function WorldmapSelectNameFrame_ScrollBar_OnValueChanged()
	local slider	= getglobal( "WorldmapSelectNameFrame_ScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateWorldmapSelectName( slider:GetValue() + 1 );
	end
end

-- 点击地区名字的时候响应
function WorldmapSelectNameBtn_OnClick()
	-- 切换区域地图
	local font = getglobal( this:GetName() .. "AreaName" );
	local mapInfo = MapShower:getMapInfoByName( this:GetClientString() );
	WorldmapSelectNameFrame:Hide();
	local nCurShowTexID = mapInfo.m_nReferMapID == 0 and mapInfo.m_nMapID or mapInfo.m_nReferMapID;

	local nCurSelectTexID = g_nCurSelectShowReferMapID == 0 and g_nCurSelectShowRealMapID or
								g_nCurSelectShowReferMapID;
	-- 若2张地图共用一张地图则无需切换
	if nCurSelectTexID == nCurShowTexID then
		return;
	end
	
	Quest:updateNpcQuestTitle( mapInfo.m_nMapID, mapInfo.m_nReferMapID );
	ChangeAreaMap( mapInfo.m_nReferMapID, mapInfo.m_nMapID )
end

function ClearWorldmapSelectNameBtn()
	for i = 1, MAX_SELECT_NAME do
		local button =  getglobal( "WorldmapSelectNameBtn" .. i );
		local font	 = 	getglobal( "WorldmapSelectNameBtn" .. i .. "AreaName" );
		button:Disable();
		button:SetClientString( "" );
		font:SetText( "" );
	end
end

-- 显示名字选择框
function WorldmapSelectNameFrame_OnShow()
	local slider = getglobal( "WorldmapSelectNameFrame_ScrollBar" );	
	slider:SetValue( 0 );

	local nIconIndex = 1;
	if this:GetClientString() == "WorldMapFrame_AreaSelectBtn" then
		SetScrollBar( "WorldmapSelectNameFrame_ScrollBar", MAX_SELECT_NAME, table.getn( t_WorldArea ) );

	elseif this:GetClientString() == "WorldMapFrame_CopySelectBtn" then
		SetScrollBar( "WorldmapSelectNameFrame_ScrollBar", MAX_SELECT_NAME, table.getn( t_WorldCopy ) );
	end

	UpdateWorldmapSelectName( slider:GetValue() + 1 );
	--SetFocusFrame("WorldmapSelectNameFrame");
end

function UpdateCampPlayerOnMap()
	local index = 1
	local ActorMgr = ActorMgr
	local nMapFrameCompareMapID = g_nCurSelectShowReferMapID ~= 0 and g_nCurSelectShowReferMapID or g_nCurSelectShowRealMapID;
	local mapSize				= MapShower:getMapSize( nMapFrameCompareMapID );
	local mainplayer			= ActorMgr:getMainPlayer()
	if mainplayer and not bSeeOtherAreaMap then
		local campIdx = mainplayer:getCampIdx()
		if campIdx ~= 0 then
			for id in pairs(nearPlayer) do
				local actor = ActorMgr:FindActor(id)
				if actor and actor:getCampIdx() == campIdx then
					local btn = FindButton("WorldmapCampPosBtn"..index);
					if not btn then
						btn = CreateButton( "WorldmapCampPosBtn"..index, "WorldmapCampPlayerPosBtnTemplate", WorldMapFrame );
					end
					--处理下跨服战场的玩家名字
					local playerName = GetPlayerName(actor:getName())
					if actor:getServerId() ~= -1 then
						playerName = "["..actor:getActorWorldName().."]"..playerName
					end
					local x, y = math.floor(actor:getPlayerX() / 10 + 0.5), math.floor(actor:getPlayerZ() / 10 + 0.5)
					local texOffset	= getPointCoordOnTex( x, y, mapSize.first, mapSize.second, nMapFrameCompareMapID );
					SetButtonInfo( {	szName = btn:GetName(), ClientString = playerName, 
										ClientUserData0 = 0,
										ClientUserData1 = x, ClientUserData2 = y } );
					btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", texOffset.x * g_fZoom, texOffset.y * g_fZoom );
					btn:Show()
					index = index + 1;
				end
			end
		end
	end
	for i = index, MAX_CAMP_MEMBER do
		btn = getglobal("WorldmapCampPosBtn" .. i)
		if btn then
			btn:Hide()
		end
	end
	MAX_CAMP_MEMBER = index - 1;
end

function AddImportWordMapIcon(id, mapid, path, ...)
	local args = {...}
	args.path = path
	args.map = mapid
	if #args == 0 then
		g_ImportantDynamic[id] = args
	else
		g_ImportantStatic[id] = args
	end
end

function RemoveImportWordMapIcon(id)
	g_ImportantStatic[id] = nil
	g_ImportantDynamic[id] = nil
end

function ClearImportWordMapIcon()
	g_ImportantStatic = {}
	g_ImportantDynamic = {}
end
function AddBaseWarMapIcon(id, mapid, path, x, y)
	RemoveBaseWarMapIcon(id);
	local args = {}
	args.path = path
	args.map = mapid
	args.x = x
	args.y = y
	g_ImportantBaseWar[id] = args;
end

function RemoveBaseWarMapIcon(id)
	g_ImportantBaseWar[id] = nil;
end

function ClearBaseWarMapIcon()
	g_ImportantBaseWar = {};
end
function AddMachineWarBoss( name, mapid, path, x, y )
	local args = {}
	args.map = mapid
	args.path = path
	args.x = x
	args.y = y
	args.name = name
	table.insert( g_MachineWarBoss, args )
end
function ClearMachineWarBoss()
	g_MachineWarBoss = {}
end

function AddClanWarCampMon(  monsterId, name, mapid, path, x, y )
	--如果已有，存储新的信息
	for i = 1, table.getn( g_ClanWarCampMon ) do
		if g_ClanWarCampMon[i].monsterId == monsterId then
			g_ClanWarCampMon[i].name = name
			g_ClanWarCampMon[i].map = mapid
			g_ClanWarCampMon[i].path = path
			g_ClanWarCampMon[i].x = x
			g_ClanWarCampMon[i].y = y		
			return
		end
	end
	local args = {}
	args.monsterId = monsterId
	args.name = name
	args.map = mapid
	args.path = path
	args.x = x
	args.y = y
	table.insert( g_ClanWarCampMon, args )
end
function ClearClanWarCampMon()
	g_ClanWarCampMon = {}
end

function AddImportWordMapVirtualIcon(flag, mapid, path, name, x, y)
	g_ImportantVirtual[flag] = {map = mapid, path = path, name = name, x = x, y = y}
end

function RemoveImportWordMapVirtualIcon(id)
	g_ImportantVirtual[id] = nil
end

function ClearImportWordMapVirtualIcon()
	g_ImportantVirtual = {}
end

function WorlMapFrame_EditBox_OnEscapePressed()
	if this:GetText() ~= "" then
		this:SetText( "" );
	else
		util.closeParent(this);
		ClearFocus();
	end
end

function getWorldArea()
	return t_WorldArea;
end

function WorldmapCaptainPosBtn_OnShow()
	local tex = getglobal( this:GetName().."UVAnimationTex" );
	tex:SetUVAnimation( 250, true);
	tex:Show();
end

function WorldmapTeamPosBtn_OnShow()
	local tex = getglobal( this:GetName().."UVAnimationTex" );
	tex:SetUVAnimation( 250, true);
	tex:Show();
end

function WorldmapMainShowButton_OnShow()
	local tex = getglobal( this:GetName().."UVAnimationTex" );
	tex:SetUVAnimation( 250, true);
	tex:Show();
end

function SetBuffMapPos( nType, nId, name, x, y )
	local btn = getglobal("WorldMapFrameBuffBtn")
	-- 0 消失
	if nType == 0 and nId == btn:GetClientUserData(0) then
		btn:Hide()
		btn:SetClientUserData(0,0)
		btn:SetClientString("")
	elseif nType == 1 then		
		btn:SetClientUserData(0,nId);
		local MaxMapControl	= MapShower:GetMaxMapContal();
		local MapInfo = MaxMapControl:updateMapInfo();
		local pos = MaxMapControl:getPointCoordOnTex(  x, y );
		local nX = ( pos.first	- MapInfo.nMapPosStartX ) * MapInfo.fZoom;
		local nY = ( pos.second	- MapInfo.nMapPosStartY ) * MapInfo.fZoom;
		btn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", nX, nY );
		local tex = getglobal( btn:GetName().."UVAnimationTex" );
		tex:SetUVAnimation( 250, true);
		btn:SetClientString( name );
		if (g_nCurSelectShowRealMapID ~= 0 or g_nCurSelectShowReferMapID ~= 0) and g_nCurSelectShowRealMapID == g_nRealMapId then
			btn:Show();
		else
			btn:Hide();
		end
	end
end

function WorldMapFrameBuffBtn_OnEnter()
	SetGameTooltips(this:GetName(), "当前状态所有者：\n" .. this:GetClientString() )
end

function WorldMapFrameCityAttackBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInClan() and ( mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT ) then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_CLAN_CITY;
		UIBeginDrag("uires\\texture\\ShuBiaoXiangGuan\\ShuBiao_JinGong.tga",CLAN_MAP_POS_TYPE_ATK_TOLUA,0,0,false,16,16);	
	else
		ShowMidTips("只有公会会长或副会长才有权限进行此操作")
	end
	
end

function WorldMapFrameCityVenueBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInClan() and ( mainplayer:getClanPosition() == CLAN_POS_PRESIDENT or mainplayer:getClanPosition() == CLAN_POS_VICE_PRESIDENT ) then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_CLAN_CITY;
		UIBeginDrag("uires\\texture\\ShuBiaoXiangGuan\\ShuBiao_JiHe.tga",CLAN_MAP_POS_TYPE_VENUE_TOLUA,0,0,false,16,16);
	else
		ShowMidTips("只有公会会长或副会长才有权限进行此操作")
	end
end

function WorldMapFrame_ShowMapBtn_ReceiveDrag()
	if UIMSG:getUIGV().DragFrom == DRAG_CLAN_CITY then
		if isPointInFrame( "WorldMapFrameMapTexture" ) then
			local nCurMouseClickPosX = GetCursorPosX();
			local nCurMouseClickPosY = GetCursorPosY();

			local nMapTextureRealLeft	= WorldMapFrameMapTexture:GetRealLeft();
			local nMapTextureRealTop	= WorldMapFrameMapTexture:GetRealTop();
			local realx = nCurMouseClickPosX - nMapTextureRealLeft;--获得当前鼠标点击的点在对应视口的坐标值
			local realy = nCurMouseClickPosY - nMapTextureRealTop;
			
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end			
			local nTexID = 0;
			if g_nCurSelectShowReferMapID == 0 then
				nTexID = g_nCurSelectShowRealMapID;
			else
				nTexID = g_nCurSelectShowReferMapID;
			end
			if nTexID >= MIN_COPY_ID then
				ShowMidTips("标点无效");
				return;
			end
			local destPosition	= getMouseCurPos( realx, realy );
			local mapSize		= MapShower:getMapSize( nTexID );
			
			if destPosition.x < 0 or destPosition.x > mapSize.first or destPosition.y < 0 or destPosition.y > mapSize.second then
				ShowMidTips( "标点无效" );
				return;
			end
			local nType = UIGetDragUserData1();
			GameClanManager:clan_Opt_SetMapPos( nTexID, destPosition.x, destPosition.y, nType );

			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
	end	
end

local nFadeTime = 0.6;
local nStartShowAttackTime = 0;
function WorldMapFrameShowAttackPosBtn_OnUpdate()
	local clanMapInfo = GameClanManager:getClanMapPos();
	local MaxMapControl	= MapShower:GetMaxMapContal();
	local MapInfo = MaxMapControl:updateMapInfo();

	if clanMapInfo.AtkInfo.MapID > 0 and g_nCurSelectShowRealMapID == clanMapInfo.AtkInfo.MapID then
		local mapSize = MapShower:getMapSize( clanMapInfo.AtkInfo.MapID );
		local texOffset	= getPointCoordOnTex( clanMapInfo.AtkInfo.Pos.X, clanMapInfo.AtkInfo.Pos.Y, mapSize.first, mapSize.second, clanMapInfo.AtkInfo.MapID );
		local nX = ( texOffset.x ) * MapInfo.fZoom;
		local nY = ( texOffset.y ) * MapInfo.fZoom;
		WorldMapFrameShowAttackPosBtn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", nX, nY );
		local serverTime = getServerTime().sec;
		if clanMapInfo.AtkInfo.EndTime < serverTime then
			local alpha = WorldMapFrameShowAttackPosBtnTex:GetBlendAlpha();
			alpha = alpha - 0.01;
			if alpha <= 0 then
				WorldMapFrameShowAttackPosBtn:Hide();
				return;
			end
			WorldMapFrameShowAttackPosBtnTex:SetBlendAlpha( alpha );
			WorldMapFrameShowAttackPosBtnEffectTex:SetBlendAlpha( alpha );
	
			local dt = os.clock() - nStartShowAttackTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;
			
			WorldMapFrameShowAttackPosBtnEffectTex:SetSize( 64*size,64*size );
		else
			if not WorldMapFrameShowAttackPosBtn:IsShown() then
				nStartShowAttackTime = os.clock();
			end
			WorldMapFrameShowAttackPosBtn:Show();
			WorldMapFrameShowAttackPosBtnTex:SetBlendAlpha( 1 );
			WorldMapFrameShowAttackPosBtnEffectTex:SetBlendAlpha( 1 );

			local dt = os.clock() - nStartShowAttackTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			WorldMapFrameShowAttackPosBtnEffectTex:SetSize( 64*size,64*size );
		end
	else
		WorldMapFrameShowAttackPosBtn:Hide();
	end
end

local nStartShowVenueTime = 0;
function WorldMapFrameShowVenuePosBtn_OnUpdate()
	local clanMapInfo = GameClanManager:getClanMapPos();
	local MaxMapControl	= MapShower:GetMaxMapContal();
	local MapInfo = MaxMapControl:updateMapInfo();

	if clanMapInfo.VenueInfo.MapID > 0 and g_nCurSelectShowRealMapID == clanMapInfo.VenueInfo.MapID then
		local mapSize = MapShower:getMapSize( clanMapInfo.VenueInfo.MapID );
		local texOffset	= getPointCoordOnTex( clanMapInfo.VenueInfo.Pos.X, clanMapInfo.VenueInfo.Pos.Y, mapSize.first, mapSize.second, clanMapInfo.VenueInfo.MapID );
		local nX = ( texOffset.x ) * MapInfo.fZoom;
		local nY = ( texOffset.y ) * MapInfo.fZoom;
		WorldMapFrameShowVenuePosBtn:SetPoint( "center", "WorldMapFrameMapTexture", "topleft", nX, nY );
		local serverTime = getServerTime().sec;
		if clanMapInfo.VenueInfo.EndTime < serverTime then
			local alpha = WorldMapFrameShowVenuePosBtnTex:GetBlendAlpha();			
			alpha = alpha - 0.02;
			if alpha <= 0 then
				WorldMapFrameShowVenuePosBtn:Hide();
				return;
			end
			WorldMapFrameShowVenuePosBtnTex:SetBlendAlpha( alpha );
			WorldMapFrameShowVenuePosBtnEffectTex:SetBlendAlpha( alpha );

			local dt = os.clock() - nStartShowVenueTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			WorldMapFrameShowVenuePosBtnEffectTex:SetSize( 64*size,64*size );
		else
			if not WorldMapFrameShowVenuePosBtn:IsShown() then
				nStartShowVenueTime = os.clock();
			end
			WorldMapFrameShowVenuePosBtn:Show();
			WorldMapFrameShowVenuePosBtnTex:SetBlendAlpha( 1 );
			WorldMapFrameShowVenuePosBtnEffectTex:SetBlendAlpha( 1 );			

			local dt = os.clock() - nStartShowVenueTime;
			local size = 0.5 + 0.5*(dt%nFadeTime)/nFadeTime;

			WorldMapFrameShowVenuePosBtnEffectTex:SetSize( 64*size,64*size );
		end
	else
		WorldMapFrameShowVenuePosBtn:Hide();
	end
end

function UpdateWorldMapClanMapPos()
	local clanMapInfo = GameClanManager:getClanMapPos();
	local serverTime = getServerTime().sec;	
	if clanMapInfo.AtkInfo.EndTime >= serverTime or WorldMapFrameShowAttackPosBtn:IsShown() then
		WorldMapFrameShowAttackPosBtn_OnUpdate();
	end
	if clanMapInfo.VenueInfo.EndTime >= serverTime or WorldMapFrameShowVenuePosBtn:IsShown() then
		WorldMapFrameShowVenuePosBtn_OnUpdate();
	end
	UpdateMiniMapClanMapPos();
end

function WorldMapFrameCityAttackBtn_OnEnter()
	SetGameTooltips(this:GetName(), "可在地图上标记出进攻标识" );
end

function WorldMapFrameCityVenueBtn_OnEnter()
	SetGameTooltips(this:GetName(), "可在地图上标记出集合标识" );
end

function OnWeatherChange( nType, master )
	if tWeatherData == nil then
		master = 1
	end
	tWeatherData = {}
	tWeatherData.type = nType
	tWeatherData.master = master
	ChangeWeather()
end

local WEATHER_TYPE_DAY = 1 		--/* 白天 */
local WEATHER_TYPE_DUSK = 2 	--/* 黄昏 */
local WEATHER_TYPE_NIGHT = 3 	--/* 夜晚 */
local WEATHER_TYPE_RAIN = 4 	--/* 雨天 */
local tRegionName = { [WEATHER_TYPE_DAY]	= "全局区域",
					  [WEATHER_TYPE_DUSK]	= "区域_3",
					  [WEATHER_TYPE_NIGHT]	= "区域_4",
					  [WEATHER_TYPE_RAIN]	= "区域_2",}

function ChangeWeather()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	if tWeatherData ~= nil and MapShower:getRealMapId() == 1000 then
		mainplayer:setOverlayRegion( tRegionName[tWeatherData.type], tWeatherData.master == 1 and 100 or 0.1 )
	end
end

function checkInHundredTown()
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		if math.floor( nRealMapID / 100 ) == 300 then
			return true;
		end
	end
	return false;
end

function ClearPVPMapNPCWithoutID()
	local nCurMapID	= MapShower:getRealMapId()
	MapShower:clearMapNpc( nCurMapID )
	MapShower:ClearPVPMapNPC( nCurMapID )
end