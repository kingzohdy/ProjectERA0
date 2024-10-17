
local t_FlyPath_1 =
{
	{ 16202, 16271, 33    },
	{ 18516, 10304, 404   },
    { 22489, 9842, -3089  },
    { 29229, 19111, 1200  },
    { 31277, 18845, 887  },
	{ 35979, 15414, 4213 },
	{ 39353, 16358, 3449 },
	{ 40953, 21823, 5000 },
	{ 39468, 24640, 2862 },
    { 30744,  24857, 1009 },
	{ 29478,  30004, 1808 },
	{ 26291,  31247, 1000 },
	{ 20394 , 32114, -900 },
	{ 14962, 33358, -2900},
    { 13667, 34330, -3500  },
    { 12453, 35490, -2200  },
    { 9511, 34444, -1009 },
    { 8861, 32172, 1811  },
     { 11098,26395,3000 },
    { 13360,24385,1170   },
}

local t_FlyPath_2 =
{
	{ 16202, 16271, 33    },
	{ 18835,14741,749  },
	{ 21839,14122,-1100 },
	{ 24916,13255,-3975},
	{ 24952,12481,-5480},
}

local t_FlyPath_3 =
{
	{ 18159, 8631, -4750 },
	{ 20470, 6910, -4420 },
    { 23136, 7880, -6748 },
}

function addFlyPoint( id, index )
	local t_FlayPath = getglobal("t_FlyPath_"..index);
	if index == 1 then
		for i = 1, table.getn(t_FlyPath_1) do
			local player = ActorMgr:FindActor(id);
			if player ~= nil then
				local specialMove = player:getSpecialMove();
				specialMove:addFlyPath( t_FlyPath_1[i][1], t_FlyPath_1[i][2], t_FlyPath_1[i][3] );
			end
		end
	end

	if index == 2 then
		for i = 1, table.getn(t_FlyPath_2) do
			local player = ActorMgr:FindActor(id);
			if player ~= nil then
				local specialMove = player:getSpecialMove();
				specialMove:addFlyPath( t_FlyPath_2[i][1], t_FlyPath_2[i][2], t_FlyPath_2[i][3] );
			end
		end
	end

	if index == 3 then
		for i = 1, table.getn(t_FlyPath_3) do
			local player = ActorMgr:FindActor(id);
			if player ~= nil then
				local specialMove = player:getSpecialMove();
				specialMove:addFlyPath( t_FlyPath_3[i][1], t_FlyPath_3[i][2], t_FlyPath_3[i][3] );
			end
		end
	end
end

--FLY_ANIM_START_INDEX 100
--当2个坐标都是0的时候，不修改主角位置
local t_FlyAnimTable = {[1] = {"db\\fly\\a.omod",16202,16271,0,110000,0},
						[FLY_ANIM_START_INDEX + 1] = {"db\\fly\\FB1.omod",25067,12597,0,110000,0},
						[FLY_ANIM_START_INDEX + 2] = {"db\\fly\\atob.omod",31475,20641,303021,5500005,0},
						[FLY_ANIM_START_INDEX + 3] = {"db\\fly\\btoc.omod",0,0,0,5500005,0},
						[FLY_ANIM_START_INDEX + 4] = {"db\\fly\\yanhuo.omod",30900,20900,0,110000,5200,true},
						[FLY_ANIM_START_INDEX + 5] = {"db\\fly\\FB2.omod",23136,7880,0,110000,0},

						--站点1 游乐场 ,站点2 工匠之家 站点3 第九街区 ,站点4 自由港
						[FLY_ANIM_START_INDEX + 6] = {"db\\fly\\TimeCityLine1.omod",14100,26000,0,540010,0},	--站点1 -- 站点2
						[FLY_ANIM_START_INDEX + 7] = {"db\\fly\\TimeCityLine2.omod",33300,48600,0,540010,0},	--站点2 -- 站点1
						[FLY_ANIM_START_INDEX + 8] = {"db\\fly\\TimeCityLine3.omod",41400,26800,0,540010,0},	--站点1 -- 站点3
						[FLY_ANIM_START_INDEX + 9] = {"db\\fly\\TimeCityLine4.omod",33300,48600,0,540010,0},	--站点3 -- 站点1
						[FLY_ANIM_START_INDEX + 10] = {"db\\fly\\TimeCityLine5.omod",26500,10700,0,540010,0},	--站点2 -- 站点4
						[FLY_ANIM_START_INDEX + 11] = {"db\\fly\\TimeCityLine6.omod",14100,26000,0,540010,0},	--站点4 -- 站点2
						[FLY_ANIM_START_INDEX + 12] = {"db\\fly\\TimeCityLine8.omod",41400,26800,0,540010,0},	--站点4 -- 站点3
						[FLY_ANIM_START_INDEX + 13] = {"db\\fly\\TimeCityLine7.omod",26500,10700,0,540010,0},	--站点3 -- 站点4
						[FLY_ANIM_START_INDEX + 14] = {"db\\fly\\zcfly1.omod",17755,18901,0,540010,0},	--绿洲 --沙漠
						[FLY_ANIM_START_INDEX + 15] = {"db\\fly\\zcfly2.omod",11607,6789,0,540010,0},	-- 沙漠--绿洲
						[FLY_ANIM_START_INDEX + 16] = {"db\\fly\\xueyuanfly1.omod",35300,26800,0,540003,0},	--雪原任务
						[FLY_ANIM_START_INDEX + 17] = {"db\\fly\\xueyuanfly2.omod",35500,26900,0,540003,0},	--雪原任务2
						[FLY_ANIM_START_INDEX + 18] = {"db\\fly\\atob.omod",31475,20641,333011,5500005,0},	--新手村 -时间城
						[FLY_ANIM_START_INDEX + 19] = {"db\\fly\\jzsd.omod",19346,20771,0,540003,0},		-- 70 FB 进
						[FLY_ANIM_START_INDEX + 20] = {"db\\fly\\jzsdb.omod",19422,15163,0,540003,0},		-- 70FB 返回
						[FLY_ANIM_START_INDEX + 21] = {"db\\fly\\111.omod",12655,40356,0,540010,0},		--新手村飞行动画
						[FLY_ANIM_START_INDEX + 22] = {"db\\fly\\boat.omod",12100,36300,0,304093,0},			-- 冰岛 动画

						[FLY_ANIM_START_INDEX + 24] = {"db\\fly\\3402-1.omod",33600,18500,0,540010,0},	--  荒野 1
						[FLY_ANIM_START_INDEX + 25] = {"db\\fly\\3402-2.omod",33100,34500,0,540010,0},	--  荒野 2
						
						[FLY_ANIM_START_INDEX + 26] = {"db\\fly\\43000-1.omod",23739,13761,0,540010,0},	--  死亡之星 1
						[FLY_ANIM_START_INDEX + 27] = {"db\\fly\\43000-2.omod",14303,23248,0,540010,0},	--  死亡之星 2
						[FLY_ANIM_START_INDEX + 28] = {"db\\fly\\43000-3.omod",10287,10829,0,540010,0},	--  死亡之星 3
						}

function addEndPoint( id, indexId )
	SetOnHorseId( id, indexId );
	local player = ActorMgr:FindActor(id);
	if player ~= nil then
		local specialMove = player:getSpecialMove();

		if t_FlyAnimTable[indexId] ~= nil then
			local omodPath = t_FlyAnimTable[indexId][1] ;
			local x = t_FlyAnimTable[indexId][2] ;
			local y = t_FlyAnimTable[indexId][3] ;
			local npcId = t_FlyAnimTable[indexId][4];
			local modelId = t_FlyAnimTable[indexId][5];
			local nDelayTick = t_FlyAnimTable[indexId][6];
			local bHideUI = ( t_FlyAnimTable[indexId][7] == true and true or false );
			specialMove:playAnimFly( id, omodPath,modelId , x, y,npcId,nDelayTick,bHideUI );
		end
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	if MainPlayer:GetID() == id and ( indexId == 102 or indexId == 118 ) then
		GameWizard:doStep(GAMEWIZARD_FIRST_LOGINTIMECITY);
	end

	if player == ActorMgr:getMainPlayer() and indexId == 104 then
		ShowCameraCoverFrame(false,indexId);
	end
end

--FLY_ANIM_PATH_INDEX 1000
local t_FlyAnimPath = {
						[FLY_ANIM_PATH_INDEX + 1] = {FLY_ANIM_START_INDEX + 6,FLY_ANIM_START_INDEX + 10},	-- 站点1 -站2- 站点4
						[FLY_ANIM_PATH_INDEX + 2] = {FLY_ANIM_START_INDEX + 12,FLY_ANIM_START_INDEX + 9},	-- 站点4 -站3- 站点1
						[FLY_ANIM_PATH_INDEX + 3] = {FLY_ANIM_START_INDEX + 10,FLY_ANIM_START_INDEX + 12},	-- 站点2 -站4- 站点3
						[FLY_ANIM_PATH_INDEX + 4] = {FLY_ANIM_START_INDEX + 9,FLY_ANIM_START_INDEX + 6},	-- 站点3 -站1- 站点2
						}

function addAnimFlyPath( id,indexId )
	local player = ActorMgr:FindActor(id);
	if player ~= nil then
		local specialMove = player:getSpecialMove();
		for i=1,table.getn(t_FlyAnimPath[indexId]) do
			specialMove:InsertAnimPath(t_FlyAnimPath[indexId][i]);
		end
	end
end

function continueAnimFly( id,indexId )
	local player = ActorMgr:FindActor(id);
	if player ~= nil then
		local specialMove = player:getSpecialMove();
		if t_FlyAnimTable[indexId] ~= nil then
			local omodPath = t_FlyAnimTable[indexId][1] ;
			local x = t_FlyAnimTable[indexId][2] ;
			local y = t_FlyAnimTable[indexId][3] ;
			local npcId = t_FlyAnimTable[indexId][4];
			local modelId = t_FlyAnimTable[indexId][5];
			specialMove:continueAnimFly( id, omodPath,modelId , x, y,npcId);
		end
	end
end