
--------------------------------------------------------------------------------------------------
--�Զ���ֵĹ��ܺ����б�
--------------------------------------------------------------------------------------------------

--����ԴID������Χ�Ĺ���
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


--���Ҹ�������Ʒ
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

--���Ҹ�������ĳ�ֹ����actor
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

--����ԴID������Χ��actor
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

--�򿪱���
function GMOpenBag()
	ContainerButtonOnClick();
end

--ʹ����Ʒ
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

--��ñ����е�ĳ����Ʒ
function GMGetIteminBag(Itemid)
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item      = container:getItem(Itemid);
	return item;
end

--�������������
function GMFullBag()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if container:getUnusedGridNum(3) <=0 then
		ChatManager:requestChat(0,"","//packageclear 3");
	end
end 

--�ж��Ƿ�����Ʒʹ����
function GMIsInUseItem()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	return	container:isUseItemPre();
end

--��ñ����е�ĳ����Ʒ����
function GMGetItemcounts(Itemid)
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	return	container:getItemCount(Itemid);
end
--ͨ��RealId��ȡ��Ʒ
function GMGetItemByid(itemid)
	local actor = GMGetActorByResId(itemid);
	if actor ~= nil then
		ItemPicker:pickItem(actor:GetID());
		return true;
	end
	return false;
end


--ͨ����ԴIDѰ·��ָ��λ�ö�������NPC
function GMLinktoNpcPostion(mapid,name)
	local npcinfo=MapShower:getNpcInfoByName(mapid,name);
	MapShower:GetMaxMapContal():linkMoveto( mapid, npcinfo.m_nPlayerX, npcinfo.m_nPlayerY);
	
end

--��õ�ǰ��ͼID
function GMGetCurMapId()
	local MinMapContal	= MapShower:GetMinMapContal();
	local nCurMapId	= MinMapContal:getMapID();
	return nCurMapId;
end

--��õ�ǰĿ��ID
function GMGetTargetId()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return 0;
	end
	return mainplayer:GetTargetID();
end

--�Ƿ��ڲɼ���
function GMIsinfight()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return true;
	end
	return mainplayer:isInFight();
end

--�Ƿ��й����ڹ������
function GMGetMonAttackplayer()
	for id in util.PlayerEnemys() do
		local actor = ActorMgr:FindActor(id);
		if actor and not actor:isDead() then
			return actor;
		end
	end
end

--�����Ƿ�����
function GMActorIsDead(monid)
	local actor = ActorMgr:FindActor( monid );
	if actor == nil or actor:isDead() then
		return true;
	end
	return false;
end

--NPC�Ի���ѡ��
function GMNpcTalk(index)
	if NpcTalkFrame:IsShown() then
		this=getglobal("NpcTalkSelectSelectItem"..index);
		NpcTalkOpt_OnClick();
		return true;
	end
	return false;
end

--�Զ�Ѱ·
function GMlinknpc(mapid,resid)
	MapShower:GetMaxMapContal():linkMoveto( mapid,resid );
end

--�Զ�Ѱ·
function GMlinkpos(mapid,posx,posy)
	 return MapShower:GetMaxMapContal():linkMoveto( mapid,posx,posy );
end

--Ѱ·��actor
function GMGotoActor(actor)
	return MapShower:GetMaxMapContal():linkMoveto( GMGetCurMapId(),actor:getPlayerX() / 10,actor:getPlayerZ() / 10);
end
--��mainplayer����
function GMDistance(actor)
	local mainplayer = ActorMgr:getMainPlayer();
	local x =  mainplayer:getPlayerX() - actor:getPlayerX();
	x = x * x;
	local y = mainplayer:getPlayerZ() - actor:getPlayerZ();
	y = y * y;
	return x + y;
end

--ѡ�����Լ�����Ĺ���Ŀ��
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


--��ǰ�õ����������жϵ�һ�����Խ��յ�����ѡ��
function GMFindCanAcceptID()
	for i=1,15 do
		local btn = getglobal("NpcTalkSelectSelectItem"..i);
		if btn:IsShown() and btn:GetClientUserData(2) == 2 then
			return i;
		end
	end
	return -1;
end


