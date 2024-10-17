MAX_AUTO_STOP_NUM	= 5; -- 15
RORATE_UNIT_ANGLE	= 30;

local DIFF_CENTER_MOVE	= 1200;
local MAX_MOVE_SCALE	= 0.3;
local MAX_CHG_PROBABILITY		= 4;
-- 分3层组织数据结构
-- 地图层，点层，次数层
-- 就列2个表，一个是静态表，里面有所有地图的ID数据
-- 一个是已经跑过的地图点	

MAX_MIN_COORDS	= 16;

local t_AutoRoundWorlldControl =	{	-- 静态层
										allDefMaps = {}, 
										-- 点层
										-- 地图层
											
										haveRoundMaps = {}, 
										curDestPoint = { mapID = 0, x = 0, y = 0 },
										stopInfo = { startTime = 0, isStop = false }, -- 上一次停止开始时间
										aroundWorldState = false,	-- 标识是否处于环游世界状态，TODO:考虑将它合并到并行状态表中


														-- 在当前地图已经移动过的点的个数		随机器上一次发生时间
										curMapInfo =	{	haveMoveTimes = 0,			lastRandomTime = 0,		MIN_RANDOM_TIME = 500,
													--[id] = { haveCompleteVisitTimes = 0, },	
															haveCompleteVisitTimes = 0,

															-- TODO: 在切换地图和孤立地图结束一遍循环后刷新这些概率数据
														smallAreaCollect =
															{
															[0] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, 
															probability = 1, accumVisitTimes = 1 },	
															[1] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 }, 
															[2] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 },
															[3] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 }, 
															[4] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 }, 
															[5] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 },
															[6] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 }, 
															[7] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1, 
																	accumVisitTimes = 1 }, 
															[8] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1 },
															[9] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1, 
																	accumVisitTimes = 1  }, 
															[10] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1  },
															[11] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1  },
															[12] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1, 
																	accumVisitTimes = 1  },
															[13] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1, 
																	accumVisitTimes = 1  },
															[13] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1, 
																	accumVisitTimes = 1  },
															[14] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1  },
															[15] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1, 
																	accumVisitTimes = 1 },
															[16] = { x = 0, y = 0, visitedOnThisTravel = false, curRotateTimes = 0, probability = 1,
																	accumVisitTimes = 1  },
															},

															bigArea =	{	[0] = { probability = 1, },
																			[1] = { probability = 1, }, [2] = { probability = 1, }, 
																			[3] = { probability = 1, }, [4] = { probability = 1, }, 
																		},
														},	
										-- TODO: 先用各个NPC点访问
										-- haveVisitedNPC = { [mapID] = { npcList = {}, monsterList = {} }, }
										haveVisitedNPC = {},
										asynchronizeState = false,
									};

-- TODO: 还差一个让要移动点的在分布上更加均匀的调整效果
function GetAutoRoundWorlldControl()
	return t_AutoRoundWorlldControl;
end

t_AutoRoundWorlldControl["init"] = 
function ( self )
	self["curDestPoint"]					= { mapID = 0, x = 0, y = 0 };
	self["stopInfo"]						= { startTime = 0, isStop = false };
	self["aroundWorldState"]				= false;
	self["curMapInfo"]["haveMoveTimes"]		= 0;
	self["curMapInfo"]["lastRandomTime"]	= 0;
	self["haveVisitedNPC"]					= {};
	self["asynchronizeState"]				= false;
	self["aroundWorldState"]				= false;
	self:clearAllHaveRoundMaps();
	self:clearAllDefMaps();
end

-- 构建地图坐标
t_AutoRoundWorlldControl["buildMapsCoord"] = 
function ( self )
	-- 根据完整访问过的当前地图次数haveCompleteVisitTimes，移动坐标原点
	-- 若haveCompleteVisitTimes为奇数则向左移动，若haveCompleteVisitTimes为偶数则向右移动
	local nMoveGrid = 0;
	--[[
	if self["curMapInfo"]["haveCompleteVisitTimes"] % 2 == 0 then
		nMoveGrid = math.ceil( self["curMapInfo"]["haveCompleteVisitTimes"] / 2 );
	else
		nMoveGrid = -1 * math.ceil( self["curMapInfo"]["haveCompleteVisitTimes"] / 2 );
	end
	--]]
	
	local MaxMapControl	= MapShower:GetMaxMapContal();
	local nMapSize		= MaxMapControl:getMapWidth();
	if math.abs( nMoveGrid ) * DIFF_CENTER_MOVE >= MAX_MOVE_SCALE * 0.5 * nMapSize then
		nMoveGrid = 0;
		self["curMapInfo"]["haveCompleteVisitTimes"] = 0;
	end

	--print( "t_AutoRoundWorlldControl--buildMapsCoord--nMoveGrid = "..nMoveGrid.."，haveCompleteVisitTimes = " 
	--			..self["curMapInfo"]["haveCompleteVisitTimes"] );
	
	local orginalPoint	= { x = nMapSize / 2, y = nMapSize / 2 };
	local nMinCorrdSize	= nMapSize / MAX_MIN_COORDS;
	local nMoveOffset	= nMoveGrid * DIFF_CENTER_MOVE;
	nMoveOffset = 0;
	-- 将地图划分成16个格子
	local t_curMapInfo = self["curMapInfo"]["smallAreaCollect"];
	t_curMapInfo[0] = { x = orginalPoint["x"] + nMoveOffset, y = orginalPoint["y"] + nMoveOffset, curRotateTimes = t_curMapInfo[0]["curRotateTimes"],
						probability = t_curMapInfo[0]["probability"],	visitedOnThisTravel = t_curMapInfo[0]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[0]["accumVisitTimes"] };

	t_curMapInfo[1] = { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 3 / 4, 
						curRotateTimes = t_curMapInfo[1]["curRotateTimes"],
						probability = t_curMapInfo[1]["probability"],	visitedOnThisTravel = t_curMapInfo[1]["visitedOnThisTravel"], 
						accumVisitTimes = t_curMapInfo[1]["accumVisitTimes"] 
						};
	t_curMapInfo[2] = { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 3 / 4,
						curRotateTimes = t_curMapInfo[2]["curRotateTimes"],
						probability = t_curMapInfo[2]["probability"],	visitedOnThisTravel = t_curMapInfo[2]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[2]["accumVisitTimes"] 
						};

	t_curMapInfo[3] = { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 1 / 4,
						curRotateTimes = t_curMapInfo[3]["curRotateTimes"],
						probability = t_curMapInfo[3]["probability"],	visitedOnThisTravel = t_curMapInfo[3]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[3]["accumVisitTimes"] 
						};
	t_curMapInfo[4] = { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 1 / 4,
						curRotateTimes = t_curMapInfo[4]["curRotateTimes"],
						probability = t_curMapInfo[4]["probability"],	visitedOnThisTravel = t_curMapInfo[4]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[4]["accumVisitTimes"] 
						};

	t_curMapInfo[5] = { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 3 / 4,
						curRotateTimes = t_curMapInfo[5]["curRotateTimes"],
						probability = t_curMapInfo[5]["probability"],	visitedOnThisTravel = t_curMapInfo[5]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[5]["accumVisitTimes"] 
						};
	t_curMapInfo[6] = { x = orginalPoint["x"] + nMoveOffset + 0.5 *  nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 3 / 4,
						curRotateTimes = t_curMapInfo[6]["curRotateTimes"],
						probability = t_curMapInfo[6]["probability"],	visitedOnThisTravel = t_curMapInfo[6]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[6]["accumVisitTimes"] 
					};
	t_curMapInfo[7] = { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 1 / 4,
						curRotateTimes = t_curMapInfo[7]["curRotateTimes"],
						probability = t_curMapInfo[7]["probability"],	visitedOnThisTravel = t_curMapInfo[7]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[7]["accumVisitTimes"] 
					};
	t_curMapInfo[8] = { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset - 0.5 * nMapSize * 1 / 4,
						curRotateTimes = t_curMapInfo[8]["curRotateTimes"],
						probability = t_curMapInfo[8]["probability"],	visitedOnThisTravel = t_curMapInfo[8]["visitedOnThisTravel"],
						accumVisitTimes = t_curMapInfo[8]["accumVisitTimes"] 
					};

	t_curMapInfo[9]		= { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 1 / 4,
							curRotateTimes = t_curMapInfo[9]["curRotateTimes"],
							probability = t_curMapInfo[9]["probability"],	visitedOnThisTravel = t_curMapInfo[9]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[9]["accumVisitTimes"] 
						};
	t_curMapInfo[10]	= { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 1 / 4,
							curRotateTimes = t_curMapInfo[10]["curRotateTimes"],
							probability = t_curMapInfo[10]["probability"],	visitedOnThisTravel = t_curMapInfo[10]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[10]["accumVisitTimes"] 
							};
	t_curMapInfo[11]	= { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 3 / 4,
							curRotateTimes = t_curMapInfo[11]["curRotateTimes"],
							probability = t_curMapInfo[11]["probability"],	visitedOnThisTravel = t_curMapInfo[11]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[11]["accumVisitTimes"] 
							};
	t_curMapInfo[12]	= { x = orginalPoint["x"] + nMoveOffset - 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 3 / 4,
							curRotateTimes = t_curMapInfo[12]["curRotateTimes"],
							probability = t_curMapInfo[12]["probability"],	visitedOnThisTravel = t_curMapInfo[12]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[12]["accumVisitTimes"] 
							};

	t_curMapInfo[13]	= { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 1 / 4,
							curRotateTimes = t_curMapInfo[13]["curRotateTimes"],
							probability = t_curMapInfo[13]["probability"],	visitedOnThisTravel = t_curMapInfo[13]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[13]["accumVisitTimes"] 
						};
	t_curMapInfo[14]	= { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 1 / 4,
							curRotateTimes = t_curMapInfo[14]["curRotateTimes"],
							probability = t_curMapInfo[14]["probability"],	visitedOnThisTravel = t_curMapInfo[14]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[14]["accumVisitTimes"] 
							};
	t_curMapInfo[15]	= { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 1 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 3 / 4,
							curRotateTimes = t_curMapInfo[15]["curRotateTimes"],
							probability = t_curMapInfo[15]["probability"],	visitedOnThisTravel = t_curMapInfo[15]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[15]["accumVisitTimes"] 
							};
	t_curMapInfo[16]	= { x = orginalPoint["x"] + nMoveOffset + 0.5 * nMapSize * 3 / 4, y = orginalPoint["y"] + nMoveOffset + 0.5 * nMapSize * 3 / 4,
							curRotateTimes = t_curMapInfo[16]["curRotateTimes"],
							probability = t_curMapInfo[16]["probability"],	visitedOnThisTravel = t_curMapInfo[16]["visitedOnThisTravel"],
							accumVisitTimes = t_curMapInfo[16]["accumVisitTimes"] 
							};
	self["curMapInfo"]["haveCompleteVisitTimes"] = self["curMapInfo"]["haveCompleteVisitTimes"] + 1;
end

-- 获取大区索引
t_AutoRoundWorlldControl["getBigAreaIndex"] = 
function ( self, data )
	local t_smallAreaMapToBigArea = 
	{
	[0] = { 0 },
	[1] = { 1, 2, 3, 4 },
	[2] = { 5, 6, 7, 8 },
	[3] = { 9, 10, 11, 12 },
	[4] = { 13, 14, 15, 16 },
	};
	
	local nQuerySmallArea = data["smallArea"];
	for index, smallAreaCollect in pairs( t_smallAreaMapToBigArea ) do
		for _, smallAreaNumber in ipairs( smallAreaCollect ) do
			if smallAreaNumber == nQuerySmallArea then
				return index;
			end
		end
	end
end

-- 当经过一个大区后，更改大区的系数
t_AutoRoundWorlldControl["passBigArea"] =
function ( self, data )
	local nQueryBigArea				= data["bigArea"];
	local t_bigAreaCollect			= self["curMapInfo"]["bigArea"];
	local t_bigAreaInfo				= self["curMapInfo"]["bigArea"][nQueryBigArea];
	t_bigAreaInfo["probability"]	= t_bigAreaInfo["probability"] * MAX_CHG_PROBABILITY;
	

	-- TODO: 目前暂定概率分布情况，与小区更改相同
	-- 同一列减少1，同一行减少2，对角线减少3
	if nQueryBigArea == 0 then
		return;
	end

	local nMaxNumBigArea	= 4;
	local nRemainder		= nQueryBigArea % nMaxNumBigArea;
	
	if nRemainder == 0 then
		-- 同一列
		t_bigAreaCollect[nQueryBigArea-2]["probability"] = t_bigAreaCollect[nQueryBigArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		t_bigAreaCollect[nQueryBigArea-1]["probability"] = t_bigAreaCollect[nQueryBigArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		t_bigAreaCollect[nQueryBigArea-3]["probability"] = t_bigAreaCollect[nQueryBigArea-3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nQueryBigArea == 1 then
		-- 同一列
		t_bigAreaCollect[nQueryBigArea+2]["probability"] = t_bigAreaCollect[nQueryBigArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		t_bigAreaCollect[nQueryBigArea+1]["probability"] = t_bigAreaCollect[nQueryBigArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		t_bigAreaCollect[nQueryBigArea+3]["probability"] = t_bigAreaCollect[nQueryBigArea+3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 2 then
		-- 同一列
		t_bigAreaCollect[nQueryBigArea+2]["probability"] = t_bigAreaCollect[nQueryBigArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		t_bigAreaCollect[nQueryBigArea-1]["probability"] = t_bigAreaCollect[nQueryBigArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		t_bigAreaCollect[nQueryBigArea+1]["probability"] = t_bigAreaCollect[nQueryBigArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 3 then
		-- 同一列
		t_bigAreaCollect[nQueryBigArea-2]["probability"] = t_bigAreaCollect[nQueryBigArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		t_bigAreaCollect[nQueryBigArea+1]["probability"] = t_bigAreaCollect[nQueryBigArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		t_bigAreaCollect[nQueryBigArea-1]["probability"] = t_bigAreaCollect[nQueryBigArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end
end

-- 当经过一个小区域后，更改小区的系数
t_AutoRoundWorlldControl["passSmallArea"] = 
function ( self, data )
	local nQuerySmallArea					= data["smallArea"];
	local t_smallAreaInfo					= self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea];
	t_smallAreaInfo["probability"]			= t_smallAreaInfo["probability"] * MAX_CHG_PROBABILITY;
	t_smallAreaInfo["visitedOnThisTravel"]	= true;
	
	-- 先将小区所处大区的概率更改
	self:passBigArea( { bigArea = self:getBigAreaIndex( { smallArea = nQuerySmallArea } ) } );

	-- TODO: 目前暂定概率分布情况
	-- 同一列减少1，同一行减少2，对角线减少3
	if nQuerySmallArea == 0 then
		return;
	end

	local nMaxNumSmallAreaInBigArea	= 4;
	local nRemainder				= nQuerySmallArea % nMaxNumSmallAreaInBigArea;
	if nRemainder == 0 then
		-- 同一列
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-3]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 1 then
		-- 同一列
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+3]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 2 then
		-- 同一列
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 3 then
		-- 同一列
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- 同一行
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- 对角线
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end
end

-- 获得大区的概率
t_AutoRoundWorlldControl["getBigAreaProbability"] =
function ( self, data )
	return self["curMapInfo"]["bigArea"][data["bigArea"]]["probability"];
end

-- 获得小区的整体概率，这是一个链式概率
t_AutoRoundWorlldControl["getSmallAreaLastProbability"] = 
function ( self, data )
	local nSmallAreaIndex		= data["smallArea"];
	--print( "nSmallAreaIndex = "..nSmallAreaIndex );
	assert( nSmallAreaIndex >= 0 and nSmallAreaIndex <= 16 , "传入非法的小区ID" );
	local nBigAreaIndex			= self:getBigAreaIndex( { smallArea = nSmallAreaIndex } );
	--print( "nBigAreaIndex = "..nBigAreaIndex );
	local nBigAreaProbability	= self:getBigAreaProbability( { bigArea = nBigAreaIndex } );
	--print( "x= "..self["curMapInfo"]["smallAreaCollect"][nSmallAreaIndex]["x"] );
	local t_smallAreaInfo		= self["curMapInfo"]["smallAreaCollect"][nSmallAreaIndex];
	return nBigAreaProbability * t_smallAreaInfo["probability"];-- probability
end

-- 比较2个小区中心点的优先级
t_AutoRoundWorlldControl["isLessThan"] = 
function ( self,lhs, rhs )
	local lhsProbability	= self:getSmallAreaLastProbability( { smallArea = lhs["smallArea"] } );
	local rhsProbability	= self:getSmallAreaLastProbability( { smallArea = rhs["smallArea"] } );
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurPosX			= mainplayer:getPlayerX() / 10;
	local nCurPosY			= mainplayer:getPlayerZ() / 10;
	local nlhsDist			= math.sqrt( util.CalPointsDistance( { x = nCurPosX, y = nCurPosY }, { x = lhs["x"], y = lhs["y"] } ) );
	local nrhsDist			= math.sqrt( util.CalPointsDistance( { x = nCurPosX, y = nCurPosY }, { x = rhs["x"], y = rhs["y"] } ) );
	--print( "lhsProbability = "..lhsProbability.."， rhsProbability = "..rhsProbability.." ， nlhsDist = "..nlhsDist.."，nrhsDist = "..nrhsDist );
	return ( lhsProbability * nlhsDist * lhs["accumVisitTimes"] ) < ( rhsProbability * nrhsDist * rhs["accumVisitTimes"] );
end

t_AutoRoundWorlldControl["getBigAreaIndexByPos"] = 
function ( self, data )
	local MaxMapControl		= MapShower:GetMaxMapContal();
	local nMapSize			= MaxMapControl:getMapWidth();
	local nQueryX			= data["x"];
	local nQueryY			= data["y"];
	local t_mapRange			= 
	{ 
	[0] = { xMin = nMapSize / 2,	yMin = nMapSize / 2,	xMax = nMapSize / 2,	yMax = nMapSize / 2, }, 
	[1] = { xMin = 0,				yMin = 0,				xMax = nMapSize / 2,	yMax = nMapSize / 2, }, 
	[2] = { xMin = nMapSize / 2,	yMin = 0,				xMax = nMapSize,		yMax = nMapSize / 2, }, 
	[3] = { xMin = 0,				yMin = nMapSize / 2,	xMax = nMapSize / 2,	yMax = nMapSize, }, 
	[4] = { xMin = nMapSize / 2,	yMin = nMapSize / 2,	xMax = nMapSize,		yMax = nMapSize, }, 
	};
	for index, data in pairs( t_mapRange ) do
		if nQueryX >= data["xMin"] and nQueryX <= data["xMax"] and nQueryY >= data["yMin"] and nQueryY <= data["yMax"] then
			return index;
		end
	end
end

-- 检测某个小区中心点整块区域是否有效
t_AutoRoundWorlldControl["isTotalBigAreaCanWalk"] = 
function ( self, data )
	local MaxMapControl		= MapShower:GetMaxMapContal();
	local nMapSize			= MaxMapControl:getMapWidth();
	local t_bigAreaRange = 
	{
	[1] = { xMin = 0,				xMax = nMapSize / 2,	yMin = 0,				yMax = nMapSize / 2 },
	[2] = { xMin = nMapSize / 2,	xMax = nMapSize,		yMin = 0,				yMax = nMapSize / 2 },
	[3] = {  xMin = 0,				xMax = nMapSize / 2,	yMin = nMapSize / 2,	yMax = nMapSize },
	[4] = {  xMin = nMapSize / 2,	xMax = nMapSize,		yMin = nMapSize / 2,	yMax = nMapSize },
	};
end

-- 检测某个大区区中心点整块区域是否有效

-- 获得最近的中心点坐标
t_AutoRoundWorlldControl["getNearestCenterPoint"] = 
function ( self )
	local mainplayer			= ActorMgr:getMainPlayer();
	local nCurPosX				= mainplayer:getPlayerX() / 10;
	local nCurPosY				= mainplayer:getPlayerZ() / 10;
	local t_curMapInfo			= self["curMapInfo"]["smallAreaCollect"];
	local nearestPoint;
	
	-- 还要排除当前所在大区
	local nCurBigArea = self:getBigAreaIndexByPos( { x = nCurPosX, y = nCurPosY } );
	for i = 0, MAX_MIN_COORDS do
		local curComparePoint = t_curMapInfo[i];
		if curComparePoint["x"] ~= nCurPosX or curComparePoint["y"] ~= nCurPosY and 
			self:getBigAreaIndex( { smallArea = i } ) ~= nCurBigArea then
			nearestPoint = { x = curComparePoint["x"], y = curComparePoint["y"], smallArea = i, accumVisitTimes = curComparePoint["accumVisitTimes"] };
			break;
		end
	end

	for i = 0, MAX_MIN_COORDS do
		local curComparePoint = t_curMapInfo[i];
		if curComparePoint["x"] == nCurPosX and curComparePoint["y"] == nCurPosY then
			curComparePoint["visitedOnThisTravel"] = true;
			break;
		end
	end
	
	for i = 1, MAX_MIN_COORDS do
		local curComparePoint = { x = t_curMapInfo[i]["x"], y = t_curMapInfo[i]["y"], smallArea = i, 
									accumVisitTimes = t_curMapInfo[i]["accumVisitTimes"] };
		-- smallArea
		--print( "t_AutoRoundWorlldControl--getNearestCenterPoint--365" );
		if not t_curMapInfo[i]["visitedOnThisTravel"] and self:isLessThan( curComparePoint, nearestPoint ) and
			t_curMapInfo[i]["x"] ~= nCurPosX or t_curMapInfo[i]["y"] ~= nCurPosY
			and self:getBigAreaIndex( { smallArea = i } ) ~= nCurBigArea then
			--print( "t_AutoRoundWorlldControl--getNearestCenterPoint--367" );
			nearestPoint = curComparePoint;
			curComparePoint["visitedOnThisTravel"] = true;
		end
	end
	
	-- 若所有点都已经访问过了，则重置这些点
	local bAllVisited = true;
	for i = 0, MAX_MIN_COORDS do
		if not t_curMapInfo[i]["visitedOnThisTravel"] then
			bAllVisited = false;
			break;
		end
	end
	
	if bAllVisited then
		self:buildMapsCoord();
		for i = 0, MAX_MIN_COORDS do
			t_curMapInfo[i]["visitedOnThisTravel"] = false;
			t_curMapInfo[i]["probability"]			= 1;
		end
	end
	
	--print( "getNearestCenterPoint = x = "..t_curMapInfo[nNearestPointIndex]["x"].."， y = "..t_curMapInfo[nNearestPointIndex]["y"] );
	return nearestPoint;
end

-- core	更改分布效果只需改这既可
--[[
t_AutoRoundWorlldControl["getRandomPoint"] = 
function ( self )		
	local mainplayer		= ActorMgr:getMainPlayer();
	local centernearestPoint=self:getNearestCenterPoint();
	-- { point = centernearestPoint, smallArea = 1 }
	local destPoint			= centernearestPoint;
	local MAX_RORATE_TIMES	= 360 / RORATE_UNIT_ANGLE;
	local nCurRotateTimes	= 1;
	local MaxMapControl		= MapShower:GetMaxMapContal();
	local nMapSize			= MaxMapControl:getMapWidth();
	--local nRadius			= math.sqrt( ( 2 * ( 1 / 4 * nMapSize ) ) ^ 2  );
	local nRadius			= 1 / 4 * nMapSize;
	local nXAdjustRadius	= nRadius;
	local nYAdjustRadius	= nRadius;
	local UNIT_CHG			= 50
	-- 先找当前点
	
	while not mainplayer:canWalk( destPoint["x"], destPoint["y"] ) do
		if nXAdjustRadius <= -nRadius and nYAdjustRadius <= -nRadius then
			nCurRotateTimes			= 0;
			centernearestPoint		= self:getNearestCenterPoint();
			destPoint["smallArea"]	= centernearestPoint["smallArea"];
			nXAdjustRadius			= nRadius;
			nYAdjustRadius			= nRadius;
		end
		
		-- 先改变纵坐标，再改变横坐标
		if nXAdjustRadius > -nRadius then
			nXAdjustRadius = nXAdjustRadius - UNIT_CHG;
		elseif nYAdjustRadius > -nRadius then
			nYAdjustRadius = nYAdjustRadius - UNIT_CHG;
		end

		destPoint["x"] = math.max( math.min( ( centernearestPoint["x"] + nXAdjustRadius ), nMapSize ), 0 );
		destPoint["y"] = math.max( math.min( ( centernearestPoint["y"] + nYAdjustRadius ), nMapSize ), 0 );
	end
	
	--print( "getRandomPoint--destPoint-- x = "..destPoint["x"].."， y = "..destPoint["y"] );
	return destPoint;
end
--]]

-- TODO: 暂时的解决方案
t_AutoRoundWorlldControl["isVisitedNpc"] = 
function ( self, data )
	local nCurMapID = data["mapId"];
	if self["haveVisitedNPC"][nCurMapID] == nil then
		self["haveVisitedNPC"][nCurMapID] = { npcList = {}, monsterList = {} };
		--print( "isVisitedNpc--556" );
		return false;
	end
	
	for _, npcId in ipairs( self["haveVisitedNPC"][nCurMapID]["npcList"] ) do
		if npcId == data["npcId"] then
			--print( "isVisitedNpc--563" );
			return true;
		end
	end
	
	--print( "isVisitedNpc--566" );
	return false;
end

t_AutoRoundWorlldControl["isVisitedMonster"] = 
function ( self, data )
	local nCurMapID = data["mapId"];
	if self["haveVisitedNPC"][nCurMapID] == nil then
		self["haveVisitedNPC"][nCurMapID] = { npcList = {}, monsterList = {} };
		return false;
	end
	
	for _, monsterId in ipairs( self["haveVisitedNPC"][nCurMapID]["monsterList"] ) do
		if monsterId == data["monsterId"] then
			return true;
		end
	end

	return false;
end

t_AutoRoundWorlldControl["addVisitedMapNpc"] = 
function ( self, data )
	local t_npcList = {};
	if self["haveVisitedNPC"][data["mapId"]] == nil then
		self["haveVisitedNPC"][data["mapId"]] = { npcList = {}, monsterList = {} };
		t_npcList = self["haveVisitedNPC"][data["mapId"]]["npcList"];
	end
	table.insert( self["haveVisitedNPC"][data["mapId"]]["npcList"], data["npcId"] );
end

t_AutoRoundWorlldControl["addVisitedMapMonster"] = 
function ( self, data )
	local t_monsterList = {};
	if self["haveVisitedNPC"][data["mapId"]] == nil then
		self["haveVisitedNPC"][data["mapId"]] = { npcList = {}, monsterList = {} };
		t_monsterList = self["haveVisitedNPC"][data["mapId"]]["monsterList"];
	end
	table.insert( self["haveVisitedNPC"][data["mapId"]]["monsterList"], data["monsterId"] );
end

-- 清除指定的NPC列表
t_AutoRoundWorlldControl["clearVisitedMapNpcList"] = 
function ( self, data )
	local nMapID = data["mapId"];
	if self["haveVisitedNPC"][nMapID] == nil then
		return;
	end

	local t_npcList = self["haveVisitedNPC"][nMapID]["npcList"];
	for i = 1, table.getn( t_npcList ) do
		table.remove( t_npcList );
	end
end

t_AutoRoundWorlldControl["clearVisitedMapMonsterList"] = 
function ( self, data )
	local nMapID = data["mapId"];
	if self["haveVisitedNPC"][nMapID] == nil then
		return;
	end

	local t_monsterList = self["haveVisitedNPC"][nMapID]["monsterList"];
	for i = 1, table.getn( t_monsterList ) do
		table.remove( t_monsterList );
	end
end

t_AutoRoundWorlldControl["getRandomPoint"] = 
function ( self )
	local MaxMapControl		= MapShower:GetMaxMapContal();
	-- TODO: 是否将self:getCurMapID全部替换返回这个地图ID
	local nCurMapID				= MaxMapControl:getMapID();
	local nMapSize				= MaxMapControl:getMapWidth();
	local STANDARD_COMP_SIZE	= ( nMapSize / 10 ) ^ 2;
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurPosX			= mainplayer:getPlayerX() / 10;
	local nCurPosY			= mainplayer:getPlayerZ() / 10;
	local mainPosPoint		= { x = nCurPosX, y = nCurPosY };

	for i = 1, MapShower:getMapNpcCount( nCurMapID ) do
		local npcInfo		= MapShower:getMapNpcInfo( nCurMapID, i - 1 );
		local npcPosPoint	= { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		local nDist			= util.CalPointsDistance( mainPosPoint, npcPosPoint );
		--print( "nDist = "..nDist.."， STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--648" );
		if not self:isVisitedNpc( { npcId = npcInfo.m_nId, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			--print( "getRandomPoint--访问NPC的ID是npcId = "..npcInfo.m_nId.."--649" );
			self:addVisitedMapNpc( { mapId = nCurMapID, npcId = npcInfo.m_nId } );
			return npcPosPoint;
		end
	end

	for i = 1, MapShower:getMapSceneAreaCount( nCurMapID ) do
		local monsterInfo		= MapShower:getMapSceneAreaInfo( nCurMapID, i - 1 );
		local monsterPosPoint	= { x = monsterInfo.Pos.X, y = monsterInfo.Pos.Y };
		local nDist				= util.CalPointsDistance( mainPosPoint, monsterPosPoint );
		--print( "nDist = "..nDist.."， STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--660" );
		if not self:isVisitedMonster( { monsterId = monsterInfo.MonsterID, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			--print( "getRandomPoint--访问怪物的ID是monsterId = "..monsterInfo.m_nId.."--663" );
			self:addVisitedMapMonster( { mapId = nCurMapID, monsterId = monsterInfo.MonsterID } );
			return monsterPosPoint;
		end
	end

	-- 若到了这儿，则表示没有符合条件的NPC，则将所有NPC列表清空
	--print( "清空怪物列表和NPC列表--669" );
	self:clearVisitedMapNpcList( { mapId = nCurMapID } );
	self:clearVisitedMapMonsterList( { mapId = nCurMapID } );
	for i = 1, MapShower:getMapNpcCount( nCurMapID ) do
		local npcInfo		= MapShower:getMapNpcInfo( nCurMapID, i - 1 );
		local npcPosPoint	= { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		local nDist			= util.CalPointsDistance( mainPosPoint, npcPosPoint );
		--print( "nDist = "..nDist.."， STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--676" );
		if not self:isVisitedNpc( { npcId = npcInfo.m_nId, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			---print( "getRandomPoint--访问NPC的ID是npcId = "..npcInfo.m_nId.."--678" );
			self:addVisitedMapNpc( { mapId = nCurMapID, npcId = npcInfo.m_nId } );
			return { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		end
	end

	for i = 1, MapShower:getMapSceneAreaCount( nCurMapID ) do
		local monsterInfo		= MapShower:getMapSceneAreaInfo( nCurMapID, i - 1 );
		local monsterPosPoint	= { x = monsterInfo.Pos.X, y = monsterInfo.Pos.Y };
		local nDist				= util.CalPointsDistance( mainPosPoint, monsterPosPoint );
		--print( "nDist = "..nDist.."， STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--688" );
		if not self:isVisitedMonster( { monsterId = monsterInfo.MonsterID, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			--print( "getRandomPoint--访问怪物的ID是monsterId = "..monsterInfo.m_nId.."--690" );
			self:addVisitedMapMonster( { mapId = nCurMapID, monsterId = monsterInfo.MonsterID } );
			return monsterPosPoint;
		end
	end
end

-- TODO: 暂时的解决方案

t_AutoRoundWorlldControl["isAroundWorld"] = 
function ( self )
	return self["aroundWorldState"];
end

t_AutoRoundWorlldControl["setAroundWorldState"] = 
function ( self, data )
	self["aroundWorldState"] = data["state"];
end

t_AutoRoundWorlldControl["clearAllDefMaps"] = 
function ( self )
	for i = 1, table.getn( self["allDefMaps"] ) do
		table.remove( self["allDefMaps"] );
	end
end

-- 初始化在当前地图移动的计数器
t_AutoRoundWorlldControl["initCurMapsMoveTimeCounter"] = 
function ( self )
	self["curMapInfo"]["haveMoveTimes"]		= 0;
	self["curMapInfo"]["lastRandomTime"]	= 0;
end

-- 增加在当前地图的移动计数器
t_AutoRoundWorlldControl["incrCurMapsMoveTimeCounter"] = 
function ( self )
	self["curMapInfo"]["haveMoveTimes"]	= self["curMapInfo"]["haveMoveTimes"] + 1;
	local pConnectedMaps = MapShower:getMultiMapFindControl():getWorldMapSearchDef( self:getCurMapID() );
	if pConnectedMaps == nil and self["curMapInfo"]["haveMoveTimes"] >= MAX_AUTO_STOP_NUM then
		self["curMapInfo"]["haveMoveTimes"] = 0;
	end
end

t_AutoRoundWorlldControl["initAllDefMaps"] = 
function ( self )
	self:clearAllDefMaps();
	for i = 1, MapShower:getMapNum() do
		local mapInfo = MapShower:getMapInfoByIndex( i - 1 );
		if mapInfo.m_nMapID < MIN_COPY_ID and not self:isIsolateMap( { mapId = mapInfo.m_nMapID } ) then
			table.insert( self["allDefMaps"], mapInfo.m_nMapID );
		end
	end
end

t_AutoRoundWorlldControl["removeDefMaps"] = 
function ( self, data )
	for index, nMapID in ipairs( self["allDefMaps"] ) do
		if data["mapID"] == nMapID then
			table.remove( self["allDefMaps"], index );
			break;
		end
	end
end

t_AutoRoundWorlldControl["isDefMapsEmpty"] = 
function ( self )
	return table.getn( self["allDefMaps"] ) == 0;
end

t_AutoRoundWorlldControl["clearAllHaveRoundMaps"] = 
function ( self )
	for i = 1, table.getn( self["haveRoundMaps"] ) do
		table.remove( self["haveRoundMaps"] );
	end
end

-- 增加当前一次完整世界遍历过程中已经遍历过的地图
t_AutoRoundWorlldControl["addHaveRoundMaps"] = 
function ( self, data )
	table.insert( self["haveRoundMaps"], data["mapID"] );
end

-- 获取当前移动的目标点
t_AutoRoundWorlldControl["getCurDestMovePoint"] = 
function ( self, data )
	return self["curDestPoint"];
end

t_AutoRoundWorlldControl["setCurDestMoveMap"] = 
function ( self, data )
	self["curDestPoint"]["mapID"]	= data["mapID"];
end

-- 设置当前移动的目标点
t_AutoRoundWorlldControl["setCurDestMovePoint"] = 
function ( self, data )
	self["curDestPoint"]["x"]		= data["x"];
	self["curDestPoint"]["y"]		= data["y"];
end

--
t_AutoRoundWorlldControl["beginAsynchronize"] =
function ( self )
	self["asynchronizeState"] = true;
end

t_AutoRoundWorlldControl["endAsynchronize"] =
function ( self )
	self["asynchronizeState"] = false;

	if self:isMapVisited( { mapID = self:getCurMapID() } ) then
		self:moveToRandomMap();
		--print( "t_AutoRoundWorlldControl--endAsynchronize--804" );
		--self:moveToCurMapRandomPoint();
		return;
	end
	
	--print( "t_AutoRoundWorlldControl--changeMap--817" );
	self:buildMapsCoord();
	-- 若上一次要达到的目标地图没到达，则继续跑到上一次目标地图，若已经到达上一次目标地图，则重新随即一个新地图
	self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
	self:initCurMapsMoveTimeCounter();
	self:moveToCurMapRandomPoint();

	if self["stopInfo"]["isStop"] then
		self["stopInfo"]["startTime"] = os.clock();
	end
end

t_AutoRoundWorlldControl["isInAsynchronize"] =
function ( self )
	return  self["asynchronizeState"];
end

-- 切换地图的时候，基于地图层或点层更新
t_AutoRoundWorlldControl["changeMap"] = 
function ( self )
	if not self:isAroundWorld() then
		return;
	end
	
	if LoadingFrame:IsShown() then
		--print( "t_AutoRoundWorlldControl--changeMap--LoadingFrame:IsShown()--798" );
		self:beginAsynchronize();
		return;
	end

	if GameMgr:isInPlayCamerAnim() then
		self:beginAsynchronize();
		return;
	end
	
	local mainplayer		= ActorMgr:getMainPlayer();
	if not mainplayer:canMove() then
		--print( "t_AutoRoundWorlldControl--changeMap-- not mainplayer:canMove()--808" );
		return;
	end

	if self:isMapVisited( { mapID = self:getCurMapID() } ) then
		self:moveToRandomMap();
		return;
	end
	
	--print( "t_AutoRoundWorlldControl--changeMap--817" );
	self:buildMapsCoord();
	-- 若上一次要达到的目标地图没到达，则继续跑到上一次目标地图，若已经到达上一次目标地图，则重新随即一个新地图
	self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
	self:initCurMapsMoveTimeCounter();
	self:moveToCurMapRandomPoint();
end

MAX_STOP_TIME	= 3;
-- 在同一张地图的时候，基于点层更新
-- 这个心跳器只做一件事情：控制人物停止和跑动
t_AutoRoundWorlldControl["update"] = 
function ( self )
	-- 移动的时候如何检测当前移动到的点是否是自己上次想移动到的目标点
	-- 一定要在同一张地图更新
	if not self:isAroundWorld() then
		--print( "t_AutoRoundWorlldControl--update--874" );
		return;
	end
	
	if LoadingFrame:IsShown() then
		--print( "t_AutoRoundWorlldControl--update--880" );
		return;
	end

	if GameMgr:isInPlayCamerAnim() then
		--print( "t_AutoRoundWorlldControl--update--886" );
		return;
	end
	
	local mainplayer		= ActorMgr:getMainPlayer();
	if not mainplayer:canMove() then
		--print( "t_AutoRoundWorlldControl--update--893" );
		return;
	end

	if self:isInAsynchronize() then
		--print( "t_AutoRoundWorlldControl--update--899" );
		self:endAsynchronize();
		return;
	end

	local nCurMapID			= self:getCurMapID();
	local nCurPosX			= mainplayer:getPlayerX() / 10;
	local nCurPosY			= mainplayer:getPlayerZ() / 10;
	local t_curDestPoint	= self["curDestPoint"];
	-- 若到达了上次自动跑动的目标点，则开始下一次跑动
	-- TODO: 在这儿加入休息动作
	--print( "t_curDestPoint[mapID] = "..t_curDestPoint["mapID"].."， nCurMapID = "..nCurMapID.."--910" );
	-- t_curDestPoint["mapID"] == nCurMapID and 
	if not mainplayer:getAutoFind() then
		-- 到达一个休息点，休息5秒
		-- 更新停止跑动状态信息
		if self["stopInfo"]["isStop"] then
			if os.clock() - self["stopInfo"]["startTime"] > MAX_STOP_TIME then
				self["stopInfo"]["isStop"]		= false;
				self["stopInfo"]["startTime"]	= 0;
			end
		else
			self["stopInfo"]["isStop"]		= true;
			self["stopInfo"]["startTime"]	= os.clock();
		end

		if self["stopInfo"]["isStop"] then
			--print( "t_AutoRoundWorlldControl--update--isStop--925" );
			return;
		end

		self:moveToCurMapRandomPoint();
	end
end

-- 更新主角的移动状态
t_AutoRoundWorlldControl["updateMoveState"] = 
function ( self )
	
end

-- 设置停止移动状态
t_AutoRoundWorlldControl["setStopState"] = 
function ( self, data )
	self["stopInfo"]["isStop"]		= data["isStop"];
	self["stopInfo"]["startTime"]	= data["startTime"];
end

-- 是否所有被定义的地图都被访问过了，若是则
t_AutoRoundWorlldControl["isAllDefMapsVisit"] = 
function ( self )
	local t_allDefMaps			= self["allDefMaps"];
	local t_allHaveRoundMaps	= self["haveRoundMaps"];

	if table.getn( t_allHaveRoundMaps ) == 0 then
		return false;
	end

	for _, nDefMapID in ipairs( t_allDefMaps ) do
		if not self:isMapVisited( { mapID = nDefMapID } ) then
			return false;
		end
	end

	return true;
end

-- 获得一个随机地图ID
t_AutoRoundWorlldControl["getRandomMapID"] = 
function ( self )
	local t_allDefMaps	= self["allDefMaps"];
	if table.getn( t_allDefMaps ) == 0 then
		--print( "t_AutoRoundWorlldControl--getRandomMapID--239" );
		self:clearAllHaveRoundMaps();
		-- TODO: DEL
		self:initAllDefMaps();

		self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
		--print( "nResultMapID = "..0 );
		return 0;
	end
	
	
	local t_temDefMaps = {};
	for i = 1, table.getn( t_allDefMaps ) do	
		table.insert( t_temDefMaps, t_allDefMaps[i] );
	end

	local nRandomIndex	= 1;
	if table.getn( t_temDefMaps ) > 1 then
		math.randomseed( GameMgr:getTickTime() );
		nRandomIndex = math.random( 1, table.getn( t_temDefMaps ) );
	end

	local nResultMapID	= t_temDefMaps[nRandomIndex];
	-- 这儿循环被中断的一种可能是所有的定义地图列表均被访问过了
	while self:isMapVisited( { mapID = nResultMapID } ) do
		--print( "t_AutoRoundWorlldControl--nResultMapID = "..nResultMapID.."--973" );
		table.remove( t_temDefMaps, nRandomIndex );

		if ( table.getn( t_temDefMaps ) == 0 ) then
			break;
		end

		if table.getn( t_temDefMaps ) > 1 then
			math.randomseed( GameMgr:getTickTime() );
			nRandomIndex = math.random( 1, table.getn( t_temDefMaps ) );
			
		else
			nRandomIndex = 1;
		end

		nResultMapID = t_temDefMaps[nRandomIndex];
	end

	if ( table.getn( t_temDefMaps ) ) == 0 then
		--print( "t_AutoRoundWorlldControl--getRandomMapID--1006" );
		self:clearAllHaveRoundMaps();
		-- TODO: DEL
		self:initAllDefMaps();

		self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
		--print( "nResultMapID = 0".."--997" );
		return 0;
	end
	
	--print( "nResultMapID = "..nResultMapID.."--1001" );
	return nResultMapID;
end

-- 查询该地图是否已经被走过
t_AutoRoundWorlldControl["isMapVisited"] = 
function ( self, data )
	for _, nHaveRoundMapID in ipairs( self["haveRoundMaps"] ) do
		if data["mapID"] == nHaveRoundMapID then
			return true;
		end
	end

	return false;
end

-- 是否还能继续在当前地图移动
t_AutoRoundWorlldControl["isPassMaxPointOnCurMap"] = 
function ( self )
	return self["curMapInfo"]["haveMoveTimes"] >= MAX_AUTO_STOP_NUM;
end

-- 是否是孤立的地图
t_AutoRoundWorlldControl["isIsolateMap"] = 
function ( self, data )
	local pConnectedMaps = MapShower:getMultiMapFindControl():getWorldMapSearchDef( data["mapId"] );
	return pConnectedMaps == nil;
end

-- 继续上一次目标点的寻路
t_AutoRoundWorlldControl["continueMoveToLastRandomPoint"] = 
function ( self )
	local t_lastMoveDestPoint = self:getCurDestMovePoint();
	MaxMapControl:linkMoveto( self:getCurMapID(), t_lastMoveDestPoint["x"], t_lastMoveDestPoint["y"] );
end

-- 点层函数：选取一个点进行随机移动
t_AutoRoundWorlldControl["moveToCurMapRandomPoint"] = 
function ( self )
	-- 若在当前地图已经移动超过最大次数，则移动到下一张地图
	if not self:isIsolateMap( { mapId = self:getCurMapID() } ) and self:isPassMaxPointOnCurMap() then
		self["curMapInfo"]["haveMoveTimes"] = 0;
		self:moveToRandomMap();
		return;
	end

	local randomPoint	= self:getRandomPoint();
	if randomPoint == nil then
		self["curMapInfo"]["haveMoveTimes"] = 0;
		self:moveToRandomMap();
		return;
	end

	local nCurMapID		= self:getCurMapID();
	local MaxMapControl	= MapShower:GetMaxMapContal();
	local nMoveResult	= MaxMapControl:linkMoveto( self:getCurMapID(), randomPoint["x"], randomPoint["y"] );
	
	-- 这需要一个循环，直到第一个点可以移动为止
	local mainplayer = ActorMgr:getMainPlayer();
	--TODO: 在这儿加个判断，同时所移动的路径点不小于定义的最小半径范围
	while nMoveResult ~= MOVE_SUCCESS do
		-- TODO: 这儿是否会出现死循环
		--print( "t_AutoRoundWorlldControl--moveToCurMapRandomPoint--1028" );
		randomPoint	= self:getRandomPoint();
		nMoveResult = MaxMapControl:linkMoveto( self:getCurMapID(), randomPoint["x"], randomPoint["y"] );
	end
	
	self:incrCurMapsMoveTimeCounter();
	self:setCurDestMovePoint( { mapID = self:getCurMapID(), x = randomPoint["x"], y = randomPoint["y"] } );
end

-- 地图层函数：随机移向另一张地图
t_AutoRoundWorlldControl["moveToRandomMap"] = 
function ( self )
	-- 若所有的地图已经被全部遍历了，则重新遍历
	if self:isAllDefMapsVisit() then
		-- 清空地图层的所有信息
		self:clearAllHaveRoundMaps();
		-- TODO: DEL
		self:initAllDefMaps();

		self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
	end
	
	local MaxMapControl	= MapShower:GetMaxMapContal();
	-- TODO: 测试语句
	-- TODO: 这个3个会暂停
	local nRandomDestMapID = self:getRandomMapID();
	
	local nMoveResult = MOVE_STATUS_FORBID;
	if nRandomDestMapID ~= 0 then
		local oneMovePoint = self:getOneCanMovePos( { mapId = nRandomDestMapID } );
		if oneMovePoint ~= nil then
			nMoveResult = MaxMapControl:linkMoveto( nRandomDestMapID, oneMovePoint["x"], oneMovePoint["y"] );
		end
	end

	-- 若无法成功移动，则删除这个地图ID，同时重新寻找下一个可以移动的地图
	local mainplayer = ActorMgr:getMainPlayer();
	-- TODO: 严重问题，进入天空岛有时候会在这儿卡斯
	-- 若地图没有任何通路，则就在这地图一直走
	while nMoveResult ~= MOVE_SUCCESS and not self:isDefMapsEmpty() do
		self:removeDefMaps( { mapID = nRandomDestMapID } );
		nRandomDestMapID = self:getRandomMapID();
		--print( "t_AutoRoundWorlldControl--moveToRandomMap--nRandomDestMapID = "..nRandomDestMapID.."--1068" );
		if nRandomDestMapID ~= 0 then
			local oneMovePoint = self:getOneCanMovePos( { mapId = nRandomDestMapID } );
			-- TOOD: 为什么会获取nil值
			if oneMovePoint ~= nil then
				nMoveResult = MaxMapControl:linkMoveto( nRandomDestMapID, oneMovePoint["x"], oneMovePoint["y"] );
			end
			
			--print( "t_AutoRoundWorlldControl--moveToRandomMap--nRandomDestMapID = "..nRandomDestMapID.."--1071" );
		end
	end

	self:setCurDestMoveMap( { mapID = nRandomDestMapID } );
end

t_AutoRoundWorlldControl["getOneCanMovePos"] = 
function ( self, data )
	local MaxMapControl		= MapShower:GetMaxMapContal();
	-- TODO: 是否将self:getCurMapID全部替换返回这个地图ID
	local nCurMapID				=data["mapId"];

	for i = 1, MapShower:getMapNpcCount( nCurMapID ) do
		local npcInfo		= MapShower:getMapNpcInfo( nCurMapID, i - 1 );
		local npcPosPoint	= { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		--print( "nDist = "..nDist.."， STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--648" );
		return npcPosPoint;
	end

	for i = 1, MapShower:getMapSceneAreaCount( nCurMapID ) do
		local monsterInfo		= MapShower:getMapSceneAreaInfo( nCurMapID, i - 1 );
		local monsterPosPoint	= { x = monsterInfo.Pos.X, y = monsterInfo.Pos.Y };
		return monsterPosPoint;
	end
end

-- 获取主角当前所处地图ID
t_AutoRoundWorlldControl["getCurMapID"] = 
function ( self )
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end

	return nCurMapID;
end

function BeginAutoRoundWorlld()
	-- TODO: 若在副本中则不能世界环游，以前有定了个规则，副本不能自动寻路到其他地图
	local nCurMapID	= t_AutoRoundWorlldControl:getCurMapID(); -- TODO: 测试语句
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "副本不能进行环游世界" );
		return;
	end

	-- 随机走一个目标地图，若随即到的目标地图已经走过，则重新随即换一个地图
	local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
	t_AutoRoundWorlldControl:buildMapsCoord();
	t_AutoRoundWorlldControl:initAllDefMaps();
	-- 清空地图层的所有信息
	t_AutoRoundWorlldControl:clearAllHaveRoundMaps();
	-- 初始化当前地图移动计数器
	t_AutoRoundWorlldControl:initCurMapsMoveTimeCounter();
	-- 将起始地图作为已经遍历过的地图
	t_AutoRoundWorlldControl:addHaveRoundMaps( { mapID = t_AutoRoundWorlldControl:getCurMapID() } );
	t_AutoRoundWorlldControl:setAroundWorldState( { state = true } );
	-- 随机移动到另一张地图
	-- 更新地图层信息
	-- 开始在当前地图移动
	t_AutoRoundWorlldControl:moveToCurMapRandomPoint()
	t_AutoRoundWorlldControl:setCurDestMoveMap( { mapID = t_AutoRoundWorlldControl:getCurMapID() } );
end

function StopAutoRoundWorlld()
	t_AutoRoundWorlldControl:setAroundWorldState( { state = false } );
	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:CmdMoveStop();
end