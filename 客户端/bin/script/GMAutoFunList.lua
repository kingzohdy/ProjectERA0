
--------------------------------------------------------------------------------------------------
--自动打怪的功能函数列表
--------------------------------------------------------------------------------------------------

--以资源ID查找周围的怪物
function GMGetMonsiterByResId(ResId)
	--[[local counts = ActorMgr:getMonsterCount() - 1;
	for i=0,counts do
		local actor= ActorMgr:getMonster(i);
		if actor:getRealId() == ResId and not actor:isDead() then
			return actor;
		end
	end--]]
	local mainplayer = ActorMgr:getMainPlayer();
	local actor = ActorMgr:getNearestActor(mainplayer:GetID(),15000);
	if actor~= nil and actor:getRealId() == ResId and not actor:isDead() then
		return actor;
	end
	return nil;
end


--查找附近的物品
function GMGetActorItem()
	local counts = ActorMgr:getActorNums() - 1;
	for i=0,counts do
		local actor= ActorMgr:getActor(i);
		if actor:getType() == GAT_ITEM then
			return actor;
		end
	end
	return nil;
end

--查找附近不是某种怪物的actor
function GMFindcActor(tId)
	local counts = ActorMgr:getActorNums() - 1;
	for i=0,counts do
		local actor= ActorMgr:getActor(i);
		if actor:getRealId() ~= tId then
			return actor;
		end
	end
	return nil;
end

--以资源ID查找周围的actor
function GMGetActorByResId(ResId)
	local counts = ActorMgr:getActorNums() - 1;
	for i=0,counts do
		local actor= ActorMgr:getActor(i);
		if actor:getRealId() == ResId and not actor:isDead()then
			return actor;
		end
	end
	return nil;
end

--打开背包
function GMOpenBag()
	ContainerButtonOnClick();
end

--使用物品
function GMUseIteminBag(Itemid)
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item      = container:getItem(Itemid);
	local gridindex = item:getGridIdx() + 1;
	if item:getListType() == CONTAINER_TYPE_SUNDRIES then
		this=getglobal("SundriesContainer");
		ShowSundriesContainer();
		ContainerRightBtnClick("PlayerContainerFrameItem"..gridindex);
	else
		this=getglobal("TaskContainer");
		ShowTaskContainer();
		ContainerRightBtnClick("PlayerContainerFrameItem"..gridindex);
	end
end

--获得背包中的某种物品
function GMGetIteminBag(Itemid)
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item      = container:getItem(Itemid);
	return item;
end

--处理背包满的情况
function GMFullBag()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if container:getUnusedGridNum(3) <=0 then
		ChatManager:requestChat(0,"","//packageclear 3");
	end
end 

--判断是否在物品使用中
function GMIsInUseItem()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	return	container:isUseItemPre();
end

--获得背包中的某种物品数量
function GMGetItemcounts(Itemid)
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	return	container:getItemCount(Itemid);
end
--通过RealId捡取物品
function GMGetItemByid(itemid)
	local actor = GMGetActorByResId(itemid);
	if actor ~= nil then
		ItemPicker:pickItem(actor:GetID());
		return true;
	end
	return false;
end


--通过资源ID寻路到指定位置而不访问NPC
function GMLinktoNpcPostion(mapid,name)
	local npcinfo=MapShower:getNpcInfoByName(mapid,name);
	MapShower:GetMaxMapContal():linkMoveto( mapid, npcinfo.m_nPlayerX, npcinfo.m_nPlayerY);
	
end

--获得当前地图ID
function GMGetCurMapId()
	local MinMapContal	= MapShower:GetMinMapContal();
	local nCurMapId	= MinMapContal:getMapID();
	return nCurMapId;
end

--获得当前目标ID
function GMGetTargetId()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return 0;
	end
	return mainplayer:GetTargetID();
end

--是否在采集中
function GMIsinfight()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return true;
	end
	return mainplayer:isInFight();
end

--是否有怪物在攻击玩家
function GMGetMonAttackplayer()
	for id in util.PlayerEnemys() do
		local actor = ActorMgr:FindActor(id);
		if actor and not actor:isDead() then
			return actor;
		end
	end
end

--怪物是否死亡
function GMActorIsDead(monid)
	local actor = ActorMgr:FindActor( monid );
	if actor == nil or actor:isDead() then
		return true;
	end
	return false;
end

--NPC对话框选择
function GMNpcTalk(index)
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem"..index);
		NpcTalkOpt_OnClick();
		return true;
	end
	return false;
end

--自动寻路
function GMlinknpc(mapid,resid)
	MapShower:GetMaxMapContal():linkMoveto( mapid,resid );
end

--自动寻路
function GMlinkpos(mapid,posx,posy)
	 return MapShower:GetMaxMapContal():linkMoveto( mapid,posx,posy );
end

--寻路到actor
function GMGotoActor(actor)
	return MapShower:GetMaxMapContal():linkMoveto( GMGetCurMapId(),actor:getPlayerX() / 10,actor:getPlayerZ() / 10);
end
--与mainplayer距离
function GMDistance(actor)
	local mainplayer = ActorMgr:getMainPlayer();
	local x =  mainplayer:getPlayerX() - actor:getPlayerX();
	x = x * x;
	local y = mainplayer:getPlayerZ() - actor:getPlayerZ();
	y = y * y;
	return x + y;
end

--选择与自己最近的攻击目标
function GMGetNearestMon()
	local counts = ActorMgr:getActorNums() - 1;
	local dis = -1;
	local factor = nil;
	for i=0,counts do
		local actor= ActorMgr:getActor(i);
		if dis == -1  and actor ~= nil and actor:getType() == GAT_MONSTER and not actor:canCollect() then
			dis = GMDistance(actor);
			factor = actor;
		end
		if  actor ~= nil and GMDistance(actor) < dis and actor:getType() == GAT_MONSTER and not actor:canCollect() then
			dis = GMDistance(actor);
			factor = actor;
		end
	end
	return factor;
end

function GMFindLastdia()
	for i=1,15 do
		local btn = getglobal("NpcTalkSelectSelectItem"..i);
		if not btn:IsShown() then
			return i - 1;
		end
	end
	return 15;
end


--用前置的纹理坐标判断第一个可以接收的任务选项
function GMFindCanAcceptID()
	for i=1,15 do
		local btn = getglobal("NpcTalkSelectSelectItem"..i);
		if btn:IsShown() and btn:GetClientUserData(2) == 2 then
			return i;
		end
	end
	return -1;
end


