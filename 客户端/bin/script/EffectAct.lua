function playEffct( x, y, angle, szEffect, delay )
	delay = delay or 60
	if szEffect == nil then
		szEffect = angle
		angle = 90
	end
	EffectManage:playEffect( x, y, angle, szEffect, math.floor(delay * 1000) );
end

function playMontion( id, szEffect )
	local player = ActorMgr:FindActor( id );
	if player ~= nil then
		local pos = string.find( szEffect, "@reset" )
		if pos ~= nil then
			player:playMotion(string.sub( szEffect, 1, pos - 1 ), true)
		else
			player:playMotion(szEffect)
		end
	end
end

function stopMontion( id, szEffect )
	local player = ActorMgr:FindActor( id );
	if player ~= nil then
		player:stopMotion(szEffect)
	end
end

function playAction( id, actId, szEffect )
	local player = ActorMgr:FindActor( id );
	if player ~= nil then
		player:playAction( actId, szEffect)
	end
end

function setWarfog( distance, outeye, posteye, r, g, b)
	GameWarfogMgr:setWarfog( distance, outeye, posteye, r, g, b);
end

function startGray()
	local player = ActorMgr:getMainPlayer();
	if player ~= nil then
		player:startGray();
	end
end

function stopGray()
	local player = ActorMgr:getMainPlayer();
	if player ~= nil then
		player:stopGray();
	end
end

function setOverlayRegion( szName )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil then
		MainPlayer:setOverlayRegion( szName );
	end
end

local t_OverlayRegionList = {};		--保存天气地图id

function clearOverlayRegion()
	t_OverlayRegionList = {};
end

function getOverlayRegion( mapid )
	return t_OverlayRegionList[ mapid ];
end

function setOverlayRegionWithMapid( mapid,szName,fSpeed )
	t_OverlayRegionList[mapid] = {szName,fSpeed};
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		if MapShower:getRealMapId() == mapid then
			mainplayer:setOverlayRegion( szName,fSpeed );
		end
	end
end

function UseTaskItemToMonsterLimit( nTaskID, nPercent, nMonsterResID )
	local t_taskLimitControl = GetTaskLimitControl();
	t_taskLimitControl:setTaskLimit( { taskID = nTaskID, hpPercent = nPercent, monsterResID = nMonsterResID } );
end