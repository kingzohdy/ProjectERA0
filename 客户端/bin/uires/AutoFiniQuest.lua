local bStartAutoQuest = false;
local MAX_CONFIG_LIST = 6;
local curQuestId = 0;
local curRecycleLv = 0;
local lastStep = 1;
local t_lastPos = {};
local t_failQuest = {};
local t_recycleList = {};
local EndTalkFlag = true;
local reTalkTick = 0;
local needToRide = true;

local t_AutoList = {
			[1] = { type = CLIENT_TASK_TYPE_TRUST, desc = "别针", opt = false, finish = false, },
			[2] = { type = CLIENT_TASK_TYPE_CHG_RECYCLE, desc = "环组",opt = false, finish = false, },
			[3] = { type = CLIENT_TASK_TYPE_REP, desc = "日常", opt = false, finish = false },
			[4] = { type = CLIENT_TASK_TYPE_DART_CAR, desc = "商运", opt = false, finish = false },
			[5] = { type = CLIENT_TASK_TYPE_GOLD, desc = "黄金", opt = false, finish = false },
			[6] = { type = CLIENT_TASK_TYPE_DAILY_GOAL, desc = "海滩", opt = false, finish = false },
			};

local t_QuestType = {
			[1] = { 1552,1554,1555,1570,1571,1572,1574,1575,1577,1578,1586,1587,1588,1591,1592,1595,
				1737,1738,1740,1771,1773,1777,1781,1782,1859,1860,1871,1872,1873,1875,1876,1880,
				1881,1883,1884,1941,1964,1971,1972,1974,1975,1976,1978,1979,1980,2021,2022,2023,
				2024,2051,2052,2053,2055,2132,2133,2134,3351,3352,3353,3354,3355,3356,3357,3358,
				3359,3360,3361, }, --击杀
			[2] = { 1680,1681,1683,1684,1685,1688,1691,1692,1693,1695,1696,1776,1779,1859,2135,2136, }, --杀怪拾取
			[3] = { 1551,1573,1579,1682,1686,1689,1774,1785,1858,1877,1965,1967,1968,2131, }, --采集
			[4] = { 1576,1687,1694,1780,1874,1879,1973,1977, }, --购买
			[5] = { 1553,1861,1862,1863 }, --使用物品
			[6] = { }, --提交物品
			[7] = { 1966, }, --护送
			}

local t_CollectItem = { [1551] = 293212, [1858] = 293160, [1877] = 293163, 
		[1967] = 293198, [1965] = 293197, [1968] = 293199, [2131] = 293241,
		[1573] = 293033, [1579] = 293032, [1682] = 293104, [1686] = 293106,
		[1689] = 293105, [1774] = 293116, [1785] = 293117,}--采集怪物id

function getQuestDoType( id )
	for i=1, #t_QuestType do
		for j=1, #t_QuestType[i] do
			if t_QuestType[i][j] == id then
				return i;
			end
		end
	end
	return 0;
end

function getAutoQusetState()
	return bStartAutoQuest;
end

function setAutoQusetState( state )
	if AutoAssist:getAutoAssistBegin() then
		AutoAssistFrame_OnClick();
	end

	bStartAutoQuest = state;
	curQuestId = 0;
	curRecycleLv = 0;
	resetAllStep();
	t_failQuest = {};
	t_recycleList = {};
	reTalkTick = 0;
	needToRide = true;
	setAutoQuestShortCutTex();
end

function setAutoQuestShortCutTex()
	local font1 = getglobal( "AutoFiniQuestFrameShortCutControlBtn" );
	local font2 = getglobal( "AutoFiniQuestConfigFrameControlBtn" );

	if getAutoQusetState() then
		font1:SetText( "停 止" );
		font2:SetText( "停 止" );
		local uvTex = getglobal( "AutoFiniQuestFrameShortCutStopTex" );
		uvTex:Hide();
		uvTex = getglobal( "AutoFiniQuestFrameShortCutActiveTex" );
		uvTex:SetUVAnimation( 200, true );
		uvTex:Show();
	else
		font1:SetText( "开 启" );
		font2:SetText( "开 启" );
		AutoFiniQuestFrameShortCutFont:SetText("");
		local uvTex = getglobal( "AutoFiniQuestFrameShortCutStopTex" );
		uvTex:Show();
		uvTex = getglobal( "AutoFiniQuestFrameShortCutActiveTex" );
		uvTex:Hide();
	end
end

function hadTalkToNpc( id )
	if NpcTalkFrame:IsShown() then
		local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
		local allOpts = t_NpcTalkFrameControl:getAllOpts();
		-- 完成的任务
		for i=1,#allOpts do
			if string.find( allOpts[i]["oneOptContent"],"(完成)",1,true ) and 
				string.find( allOpts[i]["oneOptContent"],t_AutoList[lastStep].desc,1,true ) then
				NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
				return true;
			end
		end

		if Quest:getQuestRunState( id ) == CLIENT_TASK_STAT_NO then--限制一次只接一个任务
			-- 可接的任务
			for i=1,#allOpts do
				if string.find( allOpts[i]["oneOptContent"],"(可接)",1,true ) and 
					string.find( allOpts[i]["oneOptContent"],t_AutoList[lastStep].desc,1,true ) then
					NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
					return true;
				end
			end
		end

		if t_lastPos.type ~= 4 then
			if lastStep ~= 4 then
				NpcTalkFrame:Hide();
			end
		else
			-- 特别处理购物类型的任务
			local bFound = false;
			for i=1,#allOpts do
				if string.find( allOpts[i]["oneOptContent"],"打开商店",1,true ) then
					bFound = true;
				end
			end

			if not bFound then
				NpcTalkFrame:Hide();
			end
		end
	end
	if TaskAcceptFrame:IsShown() then
		TaskAcceptFrame_AcceptBtn_OnClick()
		return true;
	end
	if TaskAwardFrame:IsShown() then
		TaskAwardFrame_GetBtn_OnClick()
		return true;
	end
	return false;
end

function checkInRecycleList( lv, recycleId )--检查环组是否已记录过
	if t_recycleList[lv] == nil then
		t_recycleList[lv] = recycleId;
	end
end

function setRecycleTask( id, lv, bRun )
	if not bRun and curRecycleLv ~= 0 and curRecycleLv ~= lv then
		--限制只接和已进行过的环组同级的任务
		return false;
	else
		local recycleId = bRun and Quest:getQuestRunRecycleID( id ) or Quest:getFirstCanAcceptRecycleTaskID( id );
		if Quest:getQuestRecycleIndex( id, recycleId ) >= VipManager:getAutoQuestOpt().m_Num[1] then
			return false;
		end
		checkInRecycleList( lv, recycleId )
		curRecycleLv = lv;
		curQuestId = id;
		return true;
	end
end

function canDoQuest( id )
	if id >= 4000 or ( id > 3121 and id < 3131 ) then--暂时处理
		return false;
	end

	for i=1, #t_failQuest do
		if t_failQuest[i] == id then
			return false;
		end
	end

	return true;
end

function getAutoFiniQuest( nType )
	if nType == CLIENT_TASK_TYPE_CHG_RECYCLE then--接取今日目标
		autoGetDailyGoal( "循环任务" );
	end
	
	for i = 1, Quest:getNumQusetRun() do
		local nQuestId = Quest:getQuestRunId(i-1);
		if nQuestId > 0 and canDoQuest( nQuestId ) then
			local nTaskType	= Quest:getQuestType(nQuestId);  --获取任务类型
			if nTaskType == nType then
				if nTaskType == CLIENT_TASK_TYPE_CHG_RECYCLE then--特殊处理环组任务
					local nLv = Quest:getQuestLvLimit(nQuestId);
					if setRecycleTask( nQuestId, nLv, true ) then
						break;
					end
				elseif nTaskType == CLIENT_TASK_TYPE_GOLD then--特殊处理赏善罚恶
					if Quest:getQuestOneDayFiniNum(nQuestId,0) < VipManager:getAutoQuestOpt().m_Num[4] then
						curQuestId = nQuestId;
					end
				else
					curQuestId = nQuestId;
					break;
				end
			end
		end
	end

	if curQuestId == 0 then
		for i = 1, Quest:getNumCanAcceptQuest() do
			local nQuestId = Quest:getCanAcceptQuestId(i-1);
			if nQuestId > 0 and not Quest:IsTrustQuest( nQuestId ) and nQuestId < 10000 
			and Quest:canAcceptQuest( nQuestId ) ~= CLINT_CAN_NOT_ACCEPT_TASK_BY_LITTLE_LV 
			and canDoQuest( nQuestId ) then
				local nTaskType		= Quest:getQuestType(nQuestId);  --获取任务类型
				if nTaskType == nType then
					if nTaskType == CLIENT_TASK_TYPE_CHG_RECYCLE then--特殊处理环组任务
						local nLv = Quest:getQuestLvLimit(nQuestId);
						if setRecycleTask( nQuestId, nLv, false ) then
							break;
						end
					elseif nTaskType == CLIENT_TASK_TYPE_GOLD then--特殊处理赏善罚恶
						if Quest:getQuestOneDayFiniNum(nQuestId,0) < VipManager:getAutoQuestOpt().m_Num[4] then
							curQuestId = nQuestId;
						end
					else
						curQuestId = nQuestId;
						break;
					end
				end
			end
		end
	end
end

function resetAllStep()
	lastStep = 1;
	for i=1, #t_AutoList do
		t_AutoList[i].finish = false;
	end
end

local function getCurStep()
	for i=1, #t_AutoList do
		if i >= lastStep and t_AutoList[i].opt and not t_AutoList[i].finish then
			return i;
		end
	end
	return 0;
end

function CheckNeedRemoveCurQuest()
	if not (curQuestId > 1) then
		return;
	end
	local bFound = false;
	for i = 1, Quest:getNumQusetRun() do
		local nQuestId = Quest:getQuestRunId(i-1);
		if nQuestId == curQuestId then
			bFound = true;
			break;
		end
	end
	if not bFound then
		if Quest:isRecycleTask( curQuestId ) then
			local mainplayer = ActorMgr:getMainPlayer();
			if Quest:getQuestLvLimit( curQuestId ) < mainplayer:getLv() then
				curRecycleLv = 0;
			else
				local recycleId = t_recycleList[curRecycleLv];
				--print('id',curQuestId,'cycId',recycleId,'lv',curRecycleLv,Quest:getQuestRecycleIndex( curQuestId, recycleId ))
				if recycleId ~= nil and Quest:getQuestRecycleIndex( curQuestId, recycleId ) >= VipManager:getAutoQuestOpt().m_Num[1] then
					curRecycleLv = 0;
				end
			end
		end
		curQuestId = 0;
	end
end

function GetNearActorByResId(ResId)
	local counts = ActorMgr:getActorNums() - 1;
	for i=0,counts do
		local actor= ActorMgr:getActor(i);
		if actor:getRealId() == ResId and not actor:isDead() then 
		--暂定任务地点15米内
			local dist = util.CalPointsDistance( { x = actor:getPlayerX() / 10, y = actor:getPlayerZ() / 10 }, { x = t_lastPos.PosX, y = t_lastPos.PosY } );
			if dist < 2250000 then
				return actor;
			end
		end
	end
	return nil;
end

function checkNearQuestPos( nState, nMapId, nNpcId, nMapPosX, nMapPosY )
	if nState ~= CLIENT_TASK_STAT_FINI and nState ~= CLIENT_TASK_STAT_NO then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then
			return false;
		end
		
		if nNpcId > 0 then
			local npcId = ActorMgr:getActorIdByResId( nNpcId );
			local npc = ActorMgr:FindActor( npcId );
			if npc then
				if util.CalActorsDistance(npc, mainplayer) <= (NpcDialog:getMaxNpcTalkDist( npc ) / 100) ^ 2 then
					if NpcTalkFrame:IsShown() then
						return true;
					elseif t_lastPos.type == 4 and ShopFrame:IsShown() then
						return true;
					end
				end

			end
		else
			if MapShower:getRealMapId() == nMapId then
				local dist = util.CalPointsDistance( { x = mainplayer:getPlayerX() / 10, y = mainplayer:getPlayerZ() / 10 }, { x = nMapPosX, y = nMapPosY } );
				if not AutoAssist:getAutoAssistBegin() then
					if t_lastPos.type > 2 then
						if dist <= 2250000 then--1500^2，范围暂定15米
							return true;
						end
					else
						if dist <= 90000 then--需要挂机的任务范围暂定3米
							return true;
						end
					end
				elseif AutoAssist:getAutoAssistBegin() and dist <= 12250000 then
					return true;
				end
			end
		end
		return false;
	end
end
--自动购买任务物品
function autoBuyQuestItem( id )
	if not ShopFrame:IsShown() and NpcTalkFrame:IsShown() then
		local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
		local allOpts = t_NpcTalkFrameControl:getAllOpts();
		for i=1,#allOpts do
			if string.find( allOpts[i]["oneOptContent"],"打开商店",1,true ) then
				NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
				return;
			end
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	if not t_lastPos.done then
		local ShopList = NpcShop:getShopList();
		for i=1, Quest:getQuestNeedItemNum(id) do
			local itemId = Quest:getQuestNeedItem(id,i-1);
			local idx = NpcShop:findItemindex(itemId);
			local num = Quest:getQuestNeedItemCount(id,i-1);
			local itemDef = getItemDef(itemId);
			if itemDef.ItemID == 0 or getPlayerMoney( mainplayer ) < math.floor(itemDef.BuyPrice * ShopList.RebateCount/100) * num then
				removeCurQuest();
				return ShowMidTips( "绑银不足，无法购买物品" );
			else
				NpcShop:buyItem( idx, num );
			end
		end
		NpcDialog:send_CloseTalk();
		t_lastPos.done = true;
	else
		for i=1, Quest:getQuestNeedItemNum(id) do
			local container	= mainplayer:getContainer();
			local itemId = Quest:getQuestNeedItem(id,i-1);
			local num = Quest:getQuestNeedItemCount(id,i-1);
			if container:getItemCount( itemId ) < num then
				t_lastPos.done = false;
				break;
			end
		end
	end
end
--自动使用任务物品
function autoUseTaskItem( id, szDes )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local itemId = ReplaceTaskTraceTip(id, szDes).nItemId;
	local itemDef = getItemDef( itemId );
	if itemDef.ItemID == 0 then return end;

	local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if TargetObj ~= nil and not TargetObj:isDead() then
		if TargetObj:getResId() == itemDef.UseTargetID then
			if container:canUseTaskItem( itemId ) then
				container:usedItem( itemId );
				return;
			end
		else
			TargetObj = GetNearActorByResId( itemDef.UseTargetID );
		end
	else
		TargetObj = GetNearActorByResId( itemDef.UseTargetID );
	end

	if TargetObj ~= nil then
		mainplayer:SetSelectTarget( TargetObj:GetID() );
		if container:canUseTaskItem( itemId ) then
			container:usedItem( itemId );
		else
			MapShower:GetMaxMapContal():linkMoveto( MapShower:getRealMapId(),TargetObj:getPlayerX() / 10,TargetObj:getPlayerZ() / 10 );
		end
	end
end

function checkQuestItemEnough( id )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	for i=1, Quest:getQuestNeedItemNum(id) do
		local container	= mainplayer:getContainer();
		local itemId = Quest:getQuestNeedItem(id,i-1);
		local num = Quest:getQuestNeedItemCount(id,i-1);
		if container:getItemCount( itemId ) < num then
			return false;
		end
	end
	return true;
end

function removeCurQuest()
	table.insert( t_failQuest, curQuestId );
	curQuestId = 0;
end

--获取任务地点
function getQuestPosByDes( id, szDes )
	local startPos,endPos;
	local t_Pos = {};
	local idx = 1;
	while idx ~= nil do
		_,startPos = string.find( szDes,"@@",idx,true );
		endPos = string.find( szDes,"#n",startPos,true );
		idx = endPos;
		if startPos ~= nil and endPos ~= nil then
			table.insert(t_Pos,{ startPos, endPos, string.sub( szDes, startPos + 1, endPos - 1 ) } );
		else
			break;
		end
	end

	if t_lastPos.state ~= CLIENT_TASK_STAT_FINI and t_lastPos.state ~= CLIENT_TASK_STAT_NO then
		if t_lastPos.type == 7 then
			for i=1, #t_Pos do
				if not string.find(t_Pos[i][3], "[", 1, true) then
					return t_Pos[i][1],t_Pos[i][2];
				end
			end
		elseif t_lastPos.type == 1 and #t_Pos > 1 then
			for i=1, #t_Pos do
				if not Quest:checkQuestMonsterKilled( id, i-1 ) then
					if t_lastPos.step < i then
						t_lastPos.step = i;
						return t_Pos[i][1],t_Pos[i][2],true;
					else
						return t_Pos[i][1],t_Pos[i][2];
					end
				end
			end
		end
	end
	return t_Pos[1][1],t_Pos[1][2];
end

--自动接取日常任务
function autoGetDailyGoal( szType )	
	for i = 1, Quest:getNumCanAcceptQuest() do
		local nQuestId = Quest:getCanAcceptQuestId(i-1);
		if nQuestId > 0 and Quest:getQuestType(nQuestId) == CLIENT_TASK_TYPE_DAILY_GOAL 
		and Quest:canAcceptQuest( nQuestId ) == CLINT_CAN_ACCEPT_TASK then
			if string.find( Quest:getQuestName(nQuestId), szType, 1, true ) then
				Quest:acceptQuest( 0, nQuestId );
				ShowMidTips( "自动接取任务" .. Quest:getQuestName(nQuestId) );
				break;
			end
		end
	end
end

function checkCanRideInAutoQuest( mainplayer )
	if not needToRide then
		return false;
	end
	if mainplayer == nil or mainplayer:isDead() then
		return false;
	end
	local BufStatus		= mainplayer:getBufStatus();
	local rider		= mainplayer:getRider();
	if rider:isOnHorse() then
		return false;
	end
	if rider:getRidNum() == 0 then
		return false;
	end
	if AutoAssist:getAutoAssistBegin() then
		return false;
	end

	if mainplayer:isOnMonsterMachine() then
		return false;
	end

	-- 判断当前地图是否能进行骑乘
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_RIDE ) then
			return false;
		end
	end

	-- 判断主角是否处于某些不能骑乘的状态
	for _, id in ipairs( t_cannotRideStatus ) do
		if BufStatus:checkBuf( id ) then
			return false;
		end
	end

	-- 处于变身状态，不能骑乘
	if not rider:isOnHorse() and mainplayer:isInMorph() then
		return false;
	end
	
	-- 处于战斗状态，不能骑乘
	if not rider:isOnHorse() and mainplayer:isInFight() then
		return false;
	end

	-- 处于自动飞行状态，不能骑乘
	if mainplayer:isInFly() then
		return false;
	end
	return true;
end

function autoRideHorse()
	local mainplayer = ActorMgr:getMainPlayer();
	if checkCanRideInAutoQuest( mainplayer ) then
		local id,lv = getLastSelectRide();
		local rider = mainplayer:getRider();
		if id ~= 0 and lv ~= 0 then
			local mapDef = getMapDef( MapShower:getRealMapId() );
			local rideInfo = rider:getRideInfoByRideId(id);
			local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
			if not rideInfo:isValidEnd() and bitAnd( mapDef.MapType, rideDef.RideMapFlag ) then
				rider:MainPlayerRidePre( id, lv );
			else
				rideOnCanUseRide();
			end
		else
			rideOnCanUseRide();
		end
	end
end

function updateAutoCommonQuest()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:isDead() then
		return;
	end
	if curQuestId ~= 0 and mainplayer:isInStand() then
		local nRealMapID = MapShower:getRealMapId();
		local nState = Quest:getQuestRunState( curQuestId );
		local nTaskType	= Quest:getQuestType( curQuestId );
		local szDes = "";
		
		if hadTalkToNpc( curQuestId ) then
			return;
		end

		if nState == CLIENT_TASK_STAT_NO then
			szDes = Quest:getCanAcceptQuestReqDesc( curQuestId );
		elseif nState == CLIENT_TASK_STAT_FINI then
			szDes = Quest:getQuestFiniTraceDes( curQuestId );
		elseif nState == CLIENT_TASK_STAT_FAIL then
			removeCurQuest();
			return;
		else
			if Quest:isQuestTimeLimit( curQuestId ) then
				local nTaskEndTime = Quest:getTaskEndTime( curQuestId );
				if nTaskEndTime <= 0 then
					removeCurQuest();
					return;
				end
			end
			szDes = Quest:getQuestNoFiniTraceDes( curQuestId );
		end

		--print( Quest:getQuestName( curQuestId ), curQuestId, nState );
		local startPos,endPos,bChange = getQuestPosByDes( curQuestId, szDes );
		if startPos == nil or endPos == nil then
			removeCurQuest();
			return ShowMidTips("无法找到当前任务地点，自动放弃该任务");
		end
		local strLinkItemHref = string.sub( szDes, startPos + 1, endPos - 1 );
		local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( strLinkItemHref );
		local nMapId, nMapPosX, nMapPosY = 0, 0, 0;
		if not bLinkNpc then
			if t_lastPos.MapId ~= 0 and t_lastPos.state == nState then
				nMapId, nMapPosX, nMapPosY = t_lastPos.MapId, t_lastPos.PosX, t_lastPos.PosY;
			else
				nMapId, nMapPosX, nMapPosY = GetLinkPos( strLinkItemHref );
				t_lastPos.PosX = nMapPosX;
				t_lastPos.PosY = nMapPosY;
				t_lastPos.MapId = nMapId;
				t_lastPos.state	= nState;
			end
			if bChange then--切换任务怪物的地点
				nMapId, nMapPosX, nMapPosY = GetLinkPos( strLinkItemHref );
				t_lastPos.PosX = nMapPosX;
				t_lastPos.PosY = nMapPosY;
				t_lastPos.MapId = nMapId;
			end
		end

		if t_lastPos.type == 6 and t_lastPos.state ~= CLIENT_TASK_STAT_FINI and t_lastPos.state ~= CLIENT_TASK_STAT_NO then
			--提交物品
			if not checkQuestItemEnough( curQuestId ) then
				removeCurQuest();
				return ShowMidTips( "当前任务所需物品不足，自动放弃该任务" );
			end
		end

		if checkNearQuestPos( nState, nMapId, nNpcId, nMapPosX, nMapPosY ) then
			if t_lastPos.type == 1 then--杀怪
				if not AutoAssist:getAutoAssistBegin() then
					AutoAssistFrame_OnClick();
				end
			elseif t_lastPos.type == 2 then--杀怪拾取
				if not AutoAssist:getAutoAssistBegin() then
					AutoAssistFrame_OnClick();
				end

				if not AutoAssist:getAutoAssistOpt().m_bChkBtn[5] then
					AutoAssist:getAutoAssistOpt().m_bChkBtn[5] = true;
					AutoAssist:saveAutoAssistOpt();	
					AutoAssistFrame_ChkBtn6:setCheckState( true );
				end
			elseif t_lastPos.type == 3 then--采集
				if AutoAssist:getAutoAssistBegin() then
					AutoAssistFrame_OnClick();
				end
				
				local resId = t_CollectItem[curQuestId] ~= nil and t_CollectItem[curQuestId] or 0;
				local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
				if TargetObj ~= nil and not TargetObj:isDead() and TargetObj:getResId() == resId then
					GameCollect:collect(TargetObj:GetID());
				else
					local actor = GetNearActorByResId( resId );
					if actor ~= nil then
						GameCollect:collect(actor:GetID());
					end
				end
			elseif t_lastPos.type == 4 then--购买
				if AutoAssist:getAutoAssistBegin() then
					AutoAssistFrame_OnClick();
				end
				autoBuyQuestItem( curQuestId );
			elseif t_lastPos.type == 5 then--使用物品
				if AutoAssist:getAutoAssistBegin() then
					AutoAssistFrame_OnClick();
				end
				autoUseTaskItem( curQuestId, szDes );
			elseif t_lastPos.type == 7 then--护送任务
				MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
			end
			return;
		end
		
		--寻路
		local multiMapFind = MapShower:getMultiMapFindControl();
		if multiMapFind:getLastAutoFindTime() > 0 then
			return;
		end

		if bLinkNpc == true then
			local npcId = ActorMgr:getActorIdByResId( nNpcId );
			local npc = ActorMgr:FindActor( npcId );
			--防止拉扯
			if npc ~= nil and NpcDialog:getTalkNpcMemID() == npcId then
				if util.CalActorsDistance(npc, mainplayer) > 5 then
					NpcDialog:send_CloseTalk();
				elseif os.clock() - reTalkTick > 5 then
					NpcDialog:send_CloseTalk();
					reTalkTick = os.clock();
					return;
				end
			end
			MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
			return;
		end

		if nMapId ~= 0 then
			MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
		end
	end
end

function AutoFiniQuestFrame_OnLoad()
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
	this:RegisterEvent("GE_QUEST_CHANGE");
	this:RegisterEvent("GE_ENTER_MAP");
	this:RegisterEvent("GE_CLAN_UNBINDMACHINE");
	this:RegisterEvent("GE_ACTION_REVIVE");
	this:RegisterEvent("GE_ENTER_PWORLD");
	this:RegisterEvent("GE_LEAVE_PWORLD");
	this:setUpdateTime( 1 );
end

function initFiniQuestData()
	local configData = VipManager:getAutoQuestOpt();
	local name = "AutoFiniQuestConfigFrameOption";
	for i=1, #t_AutoList do
		t_AutoList[i].finish = false;
		t_AutoList[i].opt = configData.m_bChkBtn[i-1];
		local chkBtn = getglobal( name .. i );
		chkBtn:setCheckState( configData.m_bChkBtn[i-1] );
	end
	AutoFiniQuestConfigFrameOption2ChooseBtnFont:SetText(configData.m_Num[1]);
	AutoFiniQuestConfigFrameOption5ChooseBtnFont:SetText(configData.m_Num[4]);
	lastStep = 1;
	curRecycleLv = 0;
	reTalkTick = 0;
	t_failQuest = {};
	t_recycleList = {};
	needToRide = true;
end

local function SetLastShowFrame()
	if AutoFiniQuestFrameShortCut:IsShown() then
		AutoFiniQuestFrameShortCut:Hide();
		AutoFiniQuestFrame:SetClientString( "AutoFiniQuestFrameShortCut" );
	else
		AutoFiniQuestMiniFrame:Hide();
		AutoFiniQuestFrame:SetClientString( "AutoFiniQuestMiniFrame" );
	end
end

function AutoFiniQuestFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_SET_AUTO_QUEST_DEFAULT ) then
			local configData = VipManager:getAutoQuestOpt();
			for i=1, MAX_CONFIG_LIST do
				configData.m_bChkBtn[i-1] = i ~= 4 and true or false;--商运默认不勾选
			end
			configData.m_Num[1] = 3;--环组
			configData.m_Num[4] = 3;--赏金
			VipManager:saveAutoQuestOpt();
			GameWizard:doStep( GAMEWIZARD_FIRST_SET_AUTO_QUEST_DEFAULT );
		end
		initFiniQuestData();
	elseif getAutoQusetState() then
		if arg1 == "GE_QUEST_CHANGE" then
			CheckNeedRemoveCurQuest();
		elseif arg1 == "GE_ENTER_MAP" then
			if MapShower:getRealMapId() >= MIN_COPY_ID then
				setAutoQusetState( false );
			end
			EndTalkFlag = false;
		elseif arg1 == "GE_CLAN_UNBINDMACHINE" then
			if lastStep == 4 then
				local mainplayer = ActorMgr:getMainPlayer();
				if mainplayer ~= nil then
					t_lastPos.PosX = mainplayer:getPlayerX() / 10;
					t_lastPos.PosY = mainplayer:getPlayerZ() / 10;
					t_lastPos.MapId = MapShower:getRealMapId();
				end
				mainplayer:CmdMoveStop();--特殊处理强制停止
			end
		elseif arg1 == "GE_ACTION_REVIVE" then
			--黑名在思过厅复活取消自动任务
			if GameActorPVP:getPkValue()/10 >= 200 and MapShower:getRealMapId() == 1000 then
				setAutoQusetState( false );
			end
			needToRide = true;
		end
	else
		if arg1 == "GE_ENTER_PWORLD" then
			SetLastShowFrame();
		elseif arg1 == "GE_LEAVE_PWORLD" then
			local frame = getglobal( this:GetClientString() );
			if frame ~= nil then
				frame:Show();
			else
				AutoFiniQuestFrameShortCut:Show();
			end
		end
	end
end

function AutoFiniQuestFrame_OnShow()
	this:SetPoint( "topright", "UIClient", "topright", "-125", "163" );
	setAutoQusetState( false );
	setAutoQuestShortCutTex();
	AutoFiniQuestFrameShortCut:Show();
end

function AutoFiniQuestFrame_OnHide()
	this:SetClientString("");
	setAutoQusetState( false );
	AutoFiniQuestMiniFrame:Hide();
end

function updateAutoFiniQuest()
	if not getAutoQusetState() then
		return;
	end

	if curQuestId == 0 then
		t_lastPos = {};
		local step = getCurStep();
		if step > 0 then
			if step > lastStep then
				t_AutoList[lastStep].finish = true;
				lastStep = step;
				needToRide = true;
			end
			if step == 1 then--别针任务
				if not AutoFinishVillaTask() then
					t_AutoList[lastStep].finish = true;
				else
					AutoFiniQuestFrameShortCutFont:SetText( "[别针换别墅]" );
				end
				return;
			elseif step == 4 then--商运任务
				getDartCarQuest();
				if curQuestId ~= 0 then
					AutoFiniQuestFrameShortCutFont:SetText( "[商运]" );
					local mainplayer = ActorMgr:getMainPlayer();
					--可能要找车，记录下地点
					if mainplayer ~= nil then
						t_lastPos.PosX = mainplayer:getPlayerX() / 10;
						t_lastPos.PosY = mainplayer:getPlayerZ() / 10;
						t_lastPos.MapId = MapShower:getRealMapId();
					end
				else
					--print('del',step,lastStep)
					t_AutoList[lastStep].finish = true;
				end
			elseif step == 6 then--海滩任务
				autoGetDailyGoal( "海滩" );
				if checkNeedGoBeach() then
					getBeachQuestPos();
					curQuestId = 1;
					AutoFiniQuestFrameShortCutFont:SetText( "[阳光海滩]" );
				else
					t_AutoList[lastStep].finish = true;
				end
			else--日常，环组，赏金
				getAutoFiniQuest( t_AutoList[step].type );
				if curQuestId ~= 0 then
					t_lastPos.type = getQuestDoType( curQuestId );
					t_lastPos.done = false;
					t_lastPos.step = 1;
					AutoFiniQuestFrameShortCutFont:SetText( string.format("[%s]", Quest:getQuestName(curQuestId)) );
				else
					t_AutoList[lastStep].finish = true;
				end
				--print('type',t_lastPos.type,curQuestId)
			end
		else
			setAutoQusetState( false );
			return ShowMidTips("当前没有可执行的任务");
		end
	else
		autoRideHorse();
		
		if lastStep == 4 then
			updateAutoDartCarQuest();
		elseif lastStep == 6 then
			updateAutoBeachQuest()
		else
			updateAutoCommonQuest();
		end
	end
end

function canAutoQuest()
	if not VipManager:isEnableVip( STAR_STONE_FIRE ) then
		ShowMidTips("您没有火焰徽章特权，不能使用自动任务");	
		return false;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		AddGameMiddleTips2("请先认证安全锁密码");
		return false;
	end
	
	if mainplayer:isDead() then
		ShowMidTips("请先复活再使用此功能");
		return false;
	end

	if mainplayer:isInStall() then
		ShowMidTips("你正在摆摊中，无法开始自动任务");
		return false;
	end

	if mainplayer:isInPK() then
		ShowMidTips("你正在切磋中，无法开始自动任务");
		return false;
	end

	if GameExchange:isInExchange() then
		ShowMidTips("你正在交易中，无法开始自动任务");
		return false;
	end

	if mainplayer:isInFly() then
		ShowMidTips("当前状态下无法开始自动任务");
		return false;
	end

	if GameFollowManager:isWeiXing() or TeamManager:isInTeamFollow() then
		ShowMidTips("跟随中无法开始自动任务");
		return false;
	end

	if mainplayer:isInRide() then
		local rider = mainplayer:getRider();
		if not rider:isOwnRide() then
			ShowMidTips("骑乘他人坐骑时无法开始自动任务");
			return false;
		end
	end

	local equip = mainplayer:getEquip()
	if equip:isUsedTmp() then
		local nTmpItemId = equip:getTmpMachin();				
		if nTmpItemId ~= nil or nTmpItemId ~= 0 then
			ShowMidTips("当前状态下无法开始自动任务");
			return false;
		end					
	end
	
	if mainplayer:isOnMonsterMachine() then
		ShowMidTips("使用怪物机械时无法开始自动任务");
		return false;
	end

	if MapShower:getRealMapId() >= MIN_COPY_ID then
		ShowMidTips( "当前处于副本中，无法开始自动任务" );
		return false;
	end

	mainplayer:CmdMoveStop();
	return true;
end

function AutoFiniQuestFrameControlBtn_OnClick()
	if getAutoQusetState() then
		setAutoQusetState( false );
		return;
	end

	if canAutoQuest() then
		local systemSetting = GameMgr:getUserSystemConfig();
		if systemSetting:getGameOptValue( GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW ) ~= 0 then
			systemSetting:setGameOptValue( GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW, 0 );
			ShowMidTips("自动任务开始，自动接受组队跟随已设置为关闭");
		end
		if GameActorPVP:getPkMode() > 1 then
			GameActorPVP:changePKMode(1);
		end
		setAutoQusetState( true );
	end
end

function AutoFiniQuestFrameHideBtn_OnClick()
	if not AutoFiniQuestConfigFrame:IsShown() then
		AutoFiniQuestConfigFrame:Show();
	else
		AutoFiniQuestConfigFrame:Hide();
	end
end

function AutoFiniQuestFrameDwindleBtn_OnEnter()
	SetGameTooltips( this:GetName(),  "最小化" );
end

function AutoFiniQuestFrameDwindleBtn_OnClick()
	HideUIFrame(this:GetParent());
	AutoFiniQuestMiniFrame:Show();
end

function AutoFiniQuestFrameShortCut_OnShow()
	AutoFiniQuestMiniFrame:Hide();
	AutoFiniQuestFrame:setInputTransparent( false );
end

function AutoFiniQuestFrameShortCut_OnHide()
	AutoFiniQuestFrame:setInputTransparent( true );
end

function AutoFiniQuestMiniFrame_OnClick()
	AutoFiniQuestFrameShortCut:Show();
end

function AutoFiniQuestMiniFrame_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "点击后打开自动任务面板", 
								frame = this:GetName(), button = this:GetName() } );
end

function AutoFiniQuestMiniFrame_OnShow()
	local tex = getglobal( this:GetName() .. "UVAnimationTex" );
	tex:SetUVAnimation( 50, true );
	tex:Show();
	AutoQuestSetEffectTimeTick();
end

-----------------------------------------自动别针任务-----------------------------------------------
local desNpc = 302111;
local NpcID	 = 0;
local IsExchange = false;
function AutoFinishVillaTask()
	local NpcResId		= 302111;
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:isDead() then
		return true;
	end
	autoRideHorse();

	if mainplayer:isInStand() then
		local questId = Quest:GetNpcQuestID( NpcResId );
		if questId == 0 then
			return false;
		end	
		if IsQuickAcceptVillaTask( NpcResId,questId) then
			return true;
		end
		if IsQuickExchangeBrooch() then
			return true;
		end
		
		--print("NpcID and curtalkID IS ",NpcID,NpcDialog:getTalkNpcMemID());
		--与NPC对话拉扯，重新选择NPC
		if EndTalkFlag then
			local szPosDes = Quest:getCanAcceptQuestReqDesc( questId );
			if szPosDes ~= "" then
				--未接别针任务 先接任务
				LinkMoveToDestination( szPosDes );
			else
				--已接别针任务 换别针
				LinkMoveToExchangeNpc( questId );
			end
			EndQuestNpcTalk( NpcID );
		end
		EndTalkFlag = true;
	end
	return true;
end

function EndQuestNpcTalk( nId )
	local npc = ActorMgr:FindActor( nId );
	if npc == nil then
		return;
	end
	if NpcDialog:getTalkNpcMemID() == nId then
		local curTime = os.clock();
		if util.CalActorsDistance(npc, ActorMgr:getMainPlayer()) > 7 then
			NpcDialog:send_CloseTalk();
		elseif curTime - reTalkTick > 6 and curTime - reTalkTick < 12 then
			reTalkTick = os.clock();
			NpcDialog:send_CloseTalk();
		end
	end
end

function IsQuickAcceptVillaTask( npcid,TaskId )
	--快速接受别针任务
	if TaskAcceptFrame:IsShown() and TaskAcceptFrame:GetClientUserData( 1 ) == TaskId then
		TaskAcceptFrame_AcceptBtn_OnClick();
		reTalkTick = os.clock();
		return true;

	--快速提交别针任务
	elseif TaskAwardFrame:IsShown() and TaskAwardFrame:GetClientUserData( 1 ) == TaskId then
		TaskAwardFrame_GetBtn_OnClick();
		reTalkTick = os.clock();
		return true;
	end	
	--与别针npc对话开始
	local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
	local npc					= ActorMgr:FindActor( t_NpcTalkFrameControl:getNpcID() );
	if npc == nil then 
		return false; 
	end
	
	local npcResId = npc:getResId();
	if NpcTalkFrame:IsShown() and npcResId == npcid then
		doQuickAcceptQuest();
		return true;
	end
	return false;
end
local exchangeTime 		= 0;
local switchVillaTime 	= 0;
local MAX_SWITCH_INTERVAL_TIME = 5;
function IsQuickExchangeBrooch()
	--快速交换别针
	local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
	local npc					= ActorMgr:FindActor( t_NpcTalkFrameControl:getNpcID() );
	if npc == nil then return; end
	
	local npcResId = npc:getResId();

	if NpcTalkFrame:IsShown() and npcResId == desNpc then
		NpcID = t_NpcTalkFrameControl:getNpcID();
		doQuickExchangeBrooch();
		return true;	
	end	
	return false;
end
function doQuickExchangeBrooch()
	local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
	local allOpts 				= t_NpcTalkFrameControl:getAllOpts();
	local curTime = os.clock();
	for i=1,#allOpts do
		if string.find( allOpts[i]["oneOptContent"],"别针换别墅",1,true ) and ( curTime - switchVillaTime ) > MAX_SWITCH_INTERVAL_TIME then 
			NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
			switchVillaTime = curTime;
			reTalkTick = curTime;
			return;
		end
		if string.find( allOpts[i]["oneOptContent"],"交换",1,true ) and ( curTime - exchangeTime ) > MAX_SWITCH_INTERVAL_TIME then 
			NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
			exchangeTime = curTime;
			reTalkTick = curTime;
			return;
		end
	end
	for i=1,#allOpts do
		if string.find( allOpts[i]["oneOptContent"],"别针换别墅",1,true ) or 
			string.find( allOpts[i]["oneOptContent"],"交换",1,true ) then
			return;
		end
	end
	NpcTalkFrame:Hide();
end

function LinkMoveToDestination( szPosDes )
	if szPosDes == "" then
		return;
	end

	local szLinkInfo;
	local nFirstPos 	= string.find( szPosDes, "@@", 1, nLength );
	local nEndPos 		= string.find( szPosDes, "#n", 1, nLength );
	local nNamePos		= string.find( szPosDes, "#L", 1, nLength );
	szLinkInfo 			= string.sub( szPosDes, nFirstPos+2, nEndPos - 1 );
--	local sNpcName 		= string.sub( szPosDes, nNamePos+2, nFirstPos - 1 );
	
	local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( szLinkInfo );
--	print(bLinkNpc, nNpcId, nTargetMapId,szPosDes);
	local actorID = ActorMgr:getActorIdByResId( nNpcId );
	if actorID > 0 then
		NpcID = actorID;
	else
		NpcID = 0;
	end
	desNpc = nNpcId;
	if bLinkNpc == true then
		-- 若主角与目标NPC不在同一张地图上
		MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
		return;
	end	
	local nMapId, nMapPosX, nMapPosY = GetLinkPos( szPosDes );
	if nMapId ~= 0 then
		MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
	end
end

function LinkMoveToExchangeNpc( nQuestId )
	local nState 	= Quest:getQuestRunState( nQuestId );
	local szDes		= "";  --追踪描述
	
	if nState == CLIENT_TASK_STAT_FINI then
		szDes 		= Quest:getQuestFiniTraceDes( nQuestId ); 
	else
		szDes = Quest:getQuestNoFiniTraceDes( nQuestId );	  			
		szDes = ReplaceTaskTraceTip( nQuestId, szDes ).szDes;	-- 替换需要右键使用的任务物品Tips描述
	end
	LinkMoveToDestination( szDes );
end

------------------------------------------自动商运-------------------------------------------
local dartCarQuestId = 1096;
local requestItemId = 5300945;
function getDartCarQuest()
	if Quest:canAcceptQuest(dartCarQuestId) ~= CLINT_CAN_ACCEPT_TASK then
		return;
	end
	curQuestId = dartCarQuestId;
	autoGetDailyGoal( "商运" );
end

function getDartCarQuestPos( id, szDes )
	local startPos,endPos;
	local step = 1;
	local t_Pos = {};
	local idx = 1;
	while idx ~= nil do
		_,startPos = string.find( szDes,"@@",idx,true );
		endPos = string.find( szDes,"#n",startPos,true );
		idx = endPos;
		if startPos ~= nil and endPos ~= nil then
			table.insert(t_Pos,{ startPos, endPos } );
		else
			break;
		end
	end

	--print('id',id,szDes,'\n',ReplaceTaskTraceTip(id, szDes).nItemId)
	if #t_Pos > 1 and ReplaceTaskTraceTip(id, szDes).nItemId == requestItemId then
		step = 2;
	end

	return t_Pos[step][1],t_Pos[step][2];
end

function GetNearDartCar()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local counts = ActorMgr:getActorNums() - 1;
	for i=0,counts do
		local actor= ActorMgr:getActor(i);
		if actor:getOwnerId() == mainplayer:GetID() and not actor:isDead() 
		and string.find( actor:getName(), "货车", 1, true ) then 
			return actor;
		end
	end
	return nil;
end

function updateAutoDartCarQuest()
	local mainplayer = ActorMgr:getMainPlayer();
	if curQuestId ~= 0 and mainplayer ~= nil and mainplayer:isInStand() and not mainplayer:isDead() then
		local nRealMapID = MapShower:getRealMapId();
		local nState = Quest:getQuestRunState( curQuestId );
		local nTaskType	= Quest:getQuestType( curQuestId );
		local szDes = "";
		
		if hadTalkToNpc( curQuestId ) then
			return;
		end

		if nState == CLIENT_TASK_STAT_NO then
			szDes = Quest:getCanAcceptQuestReqDesc( curQuestId );
		elseif nState == CLIENT_TASK_STAT_FINI then
			szDes = Quest:getQuestFiniTraceDes( curQuestId );
		elseif nState == CLIENT_TASK_STAT_FAIL then
			removeCurQuest();
			t_AutoList[lastStep].finish = true;
			return ShowMidTips("商运任务已失败，自动放弃该任务");
		else
			if Quest:isQuestTimeLimit( curQuestId ) then
				local nTaskEndTime = Quest:getTaskEndTime( curQuestId );
				if nTaskEndTime <= 0 then
					removeCurQuest();
					return;
				end
			end
			szDes = Quest:getQuestNoFiniTraceDes( curQuestId );
		end

		--print( Quest:getQuestName( curQuestId ), curQuestId, nState );
		local startPos,endPos = getDartCarQuestPos( curQuestId, szDes );
		if startPos == nil or endPos == nil then
			removeCurQuest();
			t_AutoList[lastStep].finish = true;
			return ShowMidTips("无法找到当前任务地点，自动放弃该任务");
		end
		local strLinkItemHref = string.sub( szDes, startPos + 1, endPos - 1 );
		local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( strLinkItemHref );
		local nMapId, nMapPosX, nMapPosY = 0, 0, 0;
		
		if nState == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT and not mainplayer:isOnMonsterMachine() then
			if t_lastPos.MapId ~= nil and t_lastPos.PosX ~= nil and t_lastPos.PosY ~= nil then
				nMapPosX = t_lastPos.PosX;
				nMapPosY = t_lastPos.PosY;
				nMapId = t_lastPos.MapId;
			end

			if MapShower:getRealMapId() == nMapId then
				local dist = util.CalPointsDistance( { x = mainplayer:getPlayerX() / 10, y = mainplayer:getPlayerZ() / 10 }, { x = nMapPosX, y = nMapPosY } );
				if dist <= 400 then--2米内
					local pActor = GetNearDartCar();
					if pActor == nil then
						removeCurQuest();
						t_AutoList[lastStep].finish = true;
						return ShowMidTips( "无法找到货车，自动放弃该任务" );
					end
					--重新上车，若距离过远则进行寻路
					if util.CalActorsDistance(pActor, mainplayer) < 5 then
						local rider		= mainplayer:getRider();
						if rider:isOnHorse() then
							rider:requestRideDown();
							needToRide = false;
						else
							mainplayer:requestRideMonsterMachine( pActor:GetID() );
							needToRide = true;
						end
						return;
					else
						t_lastPos.PosX = pActor:getPlayerX() / 10;
						t_lastPos.PosY = pActor:getPlayerZ() / 10;
						nMapPosX = t_lastPos.PosX;
						nMapPosY = t_lastPos.PosY;
						bLinkNpc = false;
					end
				end
			end
		else
			local npcId = ActorMgr:getActorIdByResId( nNpcId );
			local npc = ActorMgr:FindActor( npcId );
			if npc and NpcTalkFrame:IsShown() then
				local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
				local allOpts = t_NpcTalkFrameControl:getAllOpts();
				for i=1,#allOpts do
					if string.find( allOpts[i]["oneOptContent"],"商运物资",1,true ) then
						NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
						return;
					end
				end
			end
		end
		
		--寻路
		local multiMapFind = MapShower:getMultiMapFindControl();
		if multiMapFind:getLastAutoFindTime() > 0 then
			return;
		end

		if bLinkNpc == true then
			local npcId = ActorMgr:getActorIdByResId( nNpcId );
			local npc = ActorMgr:FindActor( npcId );
			if npc ~= nil then
				--防止拉扯
				--print(npc:getName(),NpcDialog:getTalkNpcMemID())
				if NpcDialog:getTalkNpcMemID() == npcId then
					if util.CalActorsDistance(npc, mainplayer) > 5 then
						NpcDialog:send_CloseTalk();
					elseif os.clock() - reTalkTick > 5 then
						NpcDialog:send_CloseTalk();
						reTalkTick = os.clock();
						return;
					end
				end
			end
			MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
			return;
		end

		if nMapId ~= 0 then
			MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
		end
	end
end

------------------------------------------自动海滩-------------------------------------------

function checkNeedGoBeach()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:getLv() < 50 then
		ShowMidTips("当前等级无法进入阳光海滩");
		return false;
	end

	if BusyMgr:GetBeachTime() > 3600 then
		ShowMidTips("你的海滩经验时间已用完，不需要进行海滩任务");
		return false;
	end
	return true;
end

function getBeachQuestPos()
	local t_BeachPos = { { 28900,14500 }, { 29000, 15300 }, { 28900, 16200 }, };
	local idx = math.random( 1, #t_BeachPos );
	t_lastPos.PosX, t_lastPos.PosY = t_BeachPos[idx][1], t_BeachPos[idx][2];
end

function updateAutoBeachQuest()
	if t_AutoList[lastStep].finish then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil or mainplayer:isDead() then
		return;
	end
	if mainplayer:isInStand() then
		local nNpcId = 0;
		if MapShower:getRealMapId() ~= BEACH_MAP_ID then
			nNpcId = 307003--海滩传送npc
			if MapShower:getRealMapId() == 1000 then
				local npcId = ActorMgr:getActorIdByResId( nNpcId );
				local npc = ActorMgr:FindActor( npcId );
				if npc and NpcTalkFrame:IsShown() then
					local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
					local allOpts = t_NpcTalkFrameControl:getAllOpts();
					for i=1,#allOpts do
						if string.find( allOpts[i]["oneOptContent"],"进入阳光海滩",1,true ) then
							NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
							return;
						end
					end
				end
			end
		else
			local dist = util.CalPointsDistance( { x = mainplayer:getPlayerX() / 10, y = mainplayer:getPlayerZ() / 10 }, { x = t_lastPos.PosX, y = t_lastPos.PosY } );
			if dist <= 900 then--3米内
				local idx = checkHaveATV(mainplayer);
				if idx > 0 then
					local rider = mainplayer:getRider();
					local rideInfo = rider:getRideGridInfo( idx - 1 );
					if rider:isOnHorse() and rider:getRideItem().RideID == rideInfo.RideID then
						removeCurQuest();
						t_AutoList[lastStep].finish = true;
					elseif CanRide() then
						rider:MainPlayerRidePre( rideInfo.RideID, rideInfo.nLevel );
					end
				else
					removeCurQuest();
					t_AutoList[lastStep].finish = true;
				end
				return;
			end
		end
		
		local multiMapFind = MapShower:getMultiMapFindControl();
		if multiMapFind:getLastAutoFindTime() > 0 then
			return;
		end

		if nNpcId > 0 then
			local npcId = ActorMgr:getActorIdByResId( nNpcId );
			local npc = ActorMgr:FindActor( npcId );
			if npc ~= nil then
				--防止拉扯
				if NpcDialog:getTalkNpcMemID() == npcId then
					if util.CalActorsDistance(npc, mainplayer) > 5 then
						NpcDialog:send_CloseTalk();
					elseif os.clock() - reTalkTick > 5 then
						NpcDialog:send_CloseTalk();
						reTalkTick = os.clock();
						return;
					end
				end
			end
			MapShower:GetMaxMapContal():linkMoveto( 1000, nNpcId );
			return;
		else
			MapShower:GetMaxMapContal():linkMoveto( BEACH_MAP_ID, t_lastPos.PosX, t_lastPos.PosY );
		end
	end
end

--选择没有过期的坐骑，速度最快的
function rideOnCanUseRide()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local mapDef = getMapDef( MapShower:getRealMapId() );
	local rider = mainplayer:getRider();
	local t_Ride = {};
	local idx = rider:getRidNum();
	while idx > 0 do
		idx = idx - 1;
		local rideInfo = rider:getRideGridInfo(idx);
		local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
		if not rideInfo:isValidEnd() and bitAnd( mapDef.MapType, rideDef.RideMapFlag ) then
			table.insert( t_Ride, { idx, rideDef.RideSpd } );
		end
	end
	if #t_Ride > 0 then
		table.sort( t_Ride, 
		function( a, b )
			return a[2] > b[2];
		end);
		idx = t_Ride[1][1];
		local rideInfo = rider:getRideGridInfo(idx);
		rider:MainPlayerRidePre( rideInfo.RideID, rideInfo.nLevel );
	end
end

--检查是否有沙滩车
function checkHaveATV( mainplayer )
	local rider = mainplayer:getRider( );
	for i = 1, rider:getRidNum() do
		local rideInfo = rider:getRideGridInfo( i - 1 );
		local rideDef = getRideDef( rideInfo.RideID, rideInfo.nLevel );
		if string.find( rideDef.RideName, "沙滩车", 1, true ) then
			return i;
		end
	end
	return 0;
end

--------------------------------------------自动任务设置界面--------------------------------------------------
local MAX_CONFIG_TEX = 8;
local MAX_RECYCLE_TIME = 10;
local MAX_GOLD_TIME = 3;--默认每日赏金任务次数
local MAX_DOWN_LIST = 10;

function AutoFiniQuestConfigFrame_OnLoad()
	for i=1,MAX_CONFIG_TEX do
		if i%2 == 0 then
			local tex = getglobal( "AutoFiniQuestConfigFrameListTex" .. i );
			tex:ChangeTextureTemplate( "BlackCategoryBkg" );
		end
	end
end

function AutoFiniQuestConfigFrame_OnEvent()
end

function AutoFiniQuestConfigFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function AutoFiniQuestConfigFrame_OnHide()
	AutoFiniQuestConfigSelectFrame:Hide();
end

function AutoFiniQuestConfigFrame_OnClick()
	if AutoFiniQuestConfigSelectFrame:IsShown() then
		AutoFiniQuestConfigSelectFrame:Hide();
	end
end

function AutoFiniQuestConfigFrameOption_OnClick()
	VipManager:getAutoQuestOpt().m_bChkBtn[this:GetClientID() - 1] = this:GetCheckState();
	VipManager:saveAutoQuestOpt();
	t_AutoList[this:GetClientID()].opt = this:GetCheckState();
end

function AutoFiniQuestConfigSelectBtn_OnClick()
	local idx = AutoFiniQuestConfigSelectFrame:GetClientUserData(0);
	local font = getglobal( "AutoFiniQuestConfigFrameOption" .. idx .. "ChooseBtnFont" )
	VipManager:getAutoQuestOpt().m_Num[idx-1] = this:GetClientID();
	font:SetText(this:GetClientID());
	VipManager:saveAutoQuestOpt();
	HideUIFrame(this:GetParent());
end

function AutoFiniQuestConfigScrollDownBtnOnClick()
	local parent = getglobal( this:GetParent() );
	local frame = getglobal( "AutoFiniQuestConfigSelectFrame" );
	if not frame:IsShown() then
		frame:SetClientUserData( 0, parent:GetClientID() );
		frame:Show();
	else
		if frame:IsShown() and frame:GetClientUserData(0) == parent:GetClientID() then
			frame:Hide();
		else
			frame:SetClientUserData( 0, parent:GetClientID() );
			frame:Show();
		end
	end
end

function AutoFiniQuestConfigSelectBtn_OnShow()
	local idx = this:GetClientUserData( 0 );
	this:SetPoint( "topleft", "AutoFiniQuestConfigFrameOption" .. idx .. "ChooseBtn", "bottomleft", 0, 0 );
	if idx == 5 then
		local num = MAX_GOLD_TIME;
		if VipManager:isOwnVipFunc( STAR_RESULT_SHANGSAN_TOLUA ) then
			num = num + VipManager:getShangShanNum();
		end

		AutoFiniQuestConfigSelectFrame:SetHeight( 21*num );
		for i=1, MAX_DOWN_LIST do
			local btn = getglobal( "AutoFiniQuestConfigSelectFrameSelectBtn" .. i );
			if i > num then
				btn:Hide();
			else
				btn:Show();
			end
		end
	else
		for i=1, MAX_DOWN_LIST do
			local btn = getglobal( "AutoFiniQuestConfigSelectFrameSelectBtn" .. i );
			btn:Show();
		end
		AutoFiniQuestConfigSelectFrame:SetHeight( 211 );
	end
end

function checkNeedResetAutoQuestNum()
	if not VipManager:isOwnVipFunc( STAR_RESULT_SHANGSAN_TOLUA ) then
		if VipManager:getAutoQuestOpt().m_Num[4] > MAX_GOLD_TIME then
			VipManager:getAutoQuestOpt().m_Num[4] = MAX_GOLD_TIME;
			VipManager:saveAutoQuestOpt();
			AutoFiniQuestConfigFrameOption5ChooseBtnFont:SetText( MAX_GOLD_TIME );
			AutoFiniQuestConfigSelectFrame:Hide();
		end
	end
end