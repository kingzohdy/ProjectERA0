MAX_AUTO_STOP_NUM	= 5; -- 15
RORATE_UNIT_ANGLE	= 30;

local DIFF_CENTER_MOVE	= 1200;
local MAX_MOVE_SCALE	= 0.3;
local MAX_CHG_PROBABILITY		= 4;
-- ��3����֯���ݽṹ
-- ��ͼ�㣬��㣬������
-- ����2����һ���Ǿ�̬�����������е�ͼ��ID����
-- һ�����Ѿ��ܹ��ĵ�ͼ��	

MAX_MIN_COORDS	= 16;

local t_AutoRoundWorlldControl =	{	-- ��̬��
										allDefMaps = {}, 
										-- ���
										-- ��ͼ��
											
										haveRoundMaps = {}, 
										curDestPoint = { mapID = 0, x = 0, y = 0 },
										stopInfo = { startTime = 0, isStop = false }, -- ��һ��ֹͣ��ʼʱ��
										aroundWorldState = false,	-- ��ʶ�Ƿ��ڻ�������״̬��TODO:���ǽ����ϲ�������״̬����


														-- �ڵ�ǰ��ͼ�Ѿ��ƶ����ĵ�ĸ���		�������һ�η���ʱ��
										curMapInfo =	{	haveMoveTimes = 0,			lastRandomTime = 0,		MIN_RANDOM_TIME = 500,
													--[id] = { haveCompleteVisitTimes = 0, },	
															haveCompleteVisitTimes = 0,

															-- TODO: ���л���ͼ�͹�����ͼ����һ��ѭ����ˢ����Щ��������
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
										-- TODO: ���ø���NPC�����
										-- haveVisitedNPC = { [mapID] = { npcList = {}, monsterList = {} }, }
										haveVisitedNPC = {},
										asynchronizeState = false,
									};

-- TODO: ����һ����Ҫ�ƶ�����ڷֲ��ϸ��Ӿ��ȵĵ���Ч��
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

-- ������ͼ����
t_AutoRoundWorlldControl["buildMapsCoord"] = 
function ( self )
	-- �����������ʹ��ĵ�ǰ��ͼ����haveCompleteVisitTimes���ƶ�����ԭ��
	-- ��haveCompleteVisitTimesΪ�����������ƶ�����haveCompleteVisitTimesΪż���������ƶ�
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

	--print( "t_AutoRoundWorlldControl--buildMapsCoord--nMoveGrid = "..nMoveGrid.."��haveCompleteVisitTimes = " 
	--			..self["curMapInfo"]["haveCompleteVisitTimes"] );
	
	local orginalPoint	= { x = nMapSize / 2, y = nMapSize / 2 };
	local nMinCorrdSize	= nMapSize / MAX_MIN_COORDS;
	local nMoveOffset	= nMoveGrid * DIFF_CENTER_MOVE;
	nMoveOffset = 0;
	-- ����ͼ���ֳ�16������
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

-- ��ȡ��������
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

-- ������һ�������󣬸��Ĵ�����ϵ��
t_AutoRoundWorlldControl["passBigArea"] =
function ( self, data )
	local nQueryBigArea				= data["bigArea"];
	local t_bigAreaCollect			= self["curMapInfo"]["bigArea"];
	local t_bigAreaInfo				= self["curMapInfo"]["bigArea"][nQueryBigArea];
	t_bigAreaInfo["probability"]	= t_bigAreaInfo["probability"] * MAX_CHG_PROBABILITY;
	

	-- TODO: Ŀǰ�ݶ����ʷֲ��������С��������ͬ
	-- ͬһ�м���1��ͬһ�м���2���Խ��߼���3
	if nQueryBigArea == 0 then
		return;
	end

	local nMaxNumBigArea	= 4;
	local nRemainder		= nQueryBigArea % nMaxNumBigArea;
	
	if nRemainder == 0 then
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea-2]["probability"] = t_bigAreaCollect[nQueryBigArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea-1]["probability"] = t_bigAreaCollect[nQueryBigArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		t_bigAreaCollect[nQueryBigArea-3]["probability"] = t_bigAreaCollect[nQueryBigArea-3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nQueryBigArea == 1 then
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea+2]["probability"] = t_bigAreaCollect[nQueryBigArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea+1]["probability"] = t_bigAreaCollect[nQueryBigArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		t_bigAreaCollect[nQueryBigArea+3]["probability"] = t_bigAreaCollect[nQueryBigArea+3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 2 then
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea+2]["probability"] = t_bigAreaCollect[nQueryBigArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea-1]["probability"] = t_bigAreaCollect[nQueryBigArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		t_bigAreaCollect[nQueryBigArea+1]["probability"] = t_bigAreaCollect[nQueryBigArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 3 then
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea-2]["probability"] = t_bigAreaCollect[nQueryBigArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		t_bigAreaCollect[nQueryBigArea+1]["probability"] = t_bigAreaCollect[nQueryBigArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		t_bigAreaCollect[nQueryBigArea-1]["probability"] = t_bigAreaCollect[nQueryBigArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end
end

-- ������һ��С����󣬸���С����ϵ��
t_AutoRoundWorlldControl["passSmallArea"] = 
function ( self, data )
	local nQuerySmallArea					= data["smallArea"];
	local t_smallAreaInfo					= self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea];
	t_smallAreaInfo["probability"]			= t_smallAreaInfo["probability"] * MAX_CHG_PROBABILITY;
	t_smallAreaInfo["visitedOnThisTravel"]	= true;
	
	-- �Ƚ�С�����������ĸ��ʸ���
	self:passBigArea( { bigArea = self:getBigAreaIndex( { smallArea = nQuerySmallArea } ) } );

	-- TODO: Ŀǰ�ݶ����ʷֲ����
	-- ͬһ�м���1��ͬһ�м���2���Խ��߼���3
	if nQuerySmallArea == 0 then
		return;
	end

	local nMaxNumSmallAreaInBigArea	= 4;
	local nRemainder				= nQuerySmallArea % nMaxNumSmallAreaInBigArea;
	if nRemainder == 0 then
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-3]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 1 then
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+3]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+3]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 2 then
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end

	if nRemainder == 3 then
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-2]["probability"] * ( MAX_CHG_PROBABILITY - 1 );
		-- ͬһ��
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea+1]["probability"] * ( MAX_CHG_PROBABILITY - 2 );
		-- �Խ���
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] = 
		self["curMapInfo"]["smallAreaCollect"][nQuerySmallArea-1]["probability"] * ( MAX_CHG_PROBABILITY - 3 );
		return;
	end
end

-- ��ô����ĸ���
t_AutoRoundWorlldControl["getBigAreaProbability"] =
function ( self, data )
	return self["curMapInfo"]["bigArea"][data["bigArea"]]["probability"];
end

-- ���С����������ʣ�����һ����ʽ����
t_AutoRoundWorlldControl["getSmallAreaLastProbability"] = 
function ( self, data )
	local nSmallAreaIndex		= data["smallArea"];
	--print( "nSmallAreaIndex = "..nSmallAreaIndex );
	assert( nSmallAreaIndex >= 0 and nSmallAreaIndex <= 16 , "����Ƿ���С��ID" );
	local nBigAreaIndex			= self:getBigAreaIndex( { smallArea = nSmallAreaIndex } );
	--print( "nBigAreaIndex = "..nBigAreaIndex );
	local nBigAreaProbability	= self:getBigAreaProbability( { bigArea = nBigAreaIndex } );
	--print( "x= "..self["curMapInfo"]["smallAreaCollect"][nSmallAreaIndex]["x"] );
	local t_smallAreaInfo		= self["curMapInfo"]["smallAreaCollect"][nSmallAreaIndex];
	return nBigAreaProbability * t_smallAreaInfo["probability"];-- probability
end

-- �Ƚ�2��С�����ĵ�����ȼ�
t_AutoRoundWorlldControl["isLessThan"] = 
function ( self,lhs, rhs )
	local lhsProbability	= self:getSmallAreaLastProbability( { smallArea = lhs["smallArea"] } );
	local rhsProbability	= self:getSmallAreaLastProbability( { smallArea = rhs["smallArea"] } );
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local nCurPosX			= mainplayer:getPlayerX() / 10;
	local nCurPosY			= mainplayer:getPlayerZ() / 10;
	local nlhsDist			= math.sqrt( util.CalPointsDistance( { x = nCurPosX, y = nCurPosY }, { x = lhs["x"], y = lhs["y"] } ) );
	local nrhsDist			= math.sqrt( util.CalPointsDistance( { x = nCurPosX, y = nCurPosY }, { x = rhs["x"], y = rhs["y"] } ) );
	--print( "lhsProbability = "..lhsProbability.."�� rhsProbability = "..rhsProbability.." �� nlhsDist = "..nlhsDist.."��nrhsDist = "..nrhsDist );
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

-- ���ĳ��С�����ĵ����������Ƿ���Ч
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

-- ���ĳ�����������ĵ����������Ƿ���Ч

-- �����������ĵ�����
t_AutoRoundWorlldControl["getNearestCenterPoint"] = 
function ( self )
	local mainplayer			= ActorMgr:getMainPlayer();
	local nCurPosX				= mainplayer:getPlayerX() / 10;
	local nCurPosY				= mainplayer:getPlayerZ() / 10;
	local t_curMapInfo			= self["curMapInfo"]["smallAreaCollect"];
	local nearestPoint;
	
	-- ��Ҫ�ų���ǰ���ڴ���
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
	
	-- �����е㶼�Ѿ����ʹ��ˣ���������Щ��
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
	
	--print( "getNearestCenterPoint = x = "..t_curMapInfo[nNearestPointIndex]["x"].."�� y = "..t_curMapInfo[nNearestPointIndex]["y"] );
	return nearestPoint;
end

-- core	���ķֲ�Ч��ֻ�����ȿ�
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
	-- ���ҵ�ǰ��
	
	while not mainplayer:canWalk( destPoint["x"], destPoint["y"] ) do
		if nXAdjustRadius <= -nRadius and nYAdjustRadius <= -nRadius then
			nCurRotateTimes			= 0;
			centernearestPoint		= self:getNearestCenterPoint();
			destPoint["smallArea"]	= centernearestPoint["smallArea"];
			nXAdjustRadius			= nRadius;
			nYAdjustRadius			= nRadius;
		end
		
		-- �ȸı������꣬�ٸı������
		if nXAdjustRadius > -nRadius then
			nXAdjustRadius = nXAdjustRadius - UNIT_CHG;
		elseif nYAdjustRadius > -nRadius then
			nYAdjustRadius = nYAdjustRadius - UNIT_CHG;
		end

		destPoint["x"] = math.max( math.min( ( centernearestPoint["x"] + nXAdjustRadius ), nMapSize ), 0 );
		destPoint["y"] = math.max( math.min( ( centernearestPoint["y"] + nYAdjustRadius ), nMapSize ), 0 );
	end
	
	--print( "getRandomPoint--destPoint-- x = "..destPoint["x"].."�� y = "..destPoint["y"] );
	return destPoint;
end
--]]

-- TODO: ��ʱ�Ľ������
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

-- ���ָ����NPC�б�
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
	-- TODO: �Ƿ�self:getCurMapIDȫ���滻���������ͼID
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
		--print( "nDist = "..nDist.."�� STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--648" );
		if not self:isVisitedNpc( { npcId = npcInfo.m_nId, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			--print( "getRandomPoint--����NPC��ID��npcId = "..npcInfo.m_nId.."--649" );
			self:addVisitedMapNpc( { mapId = nCurMapID, npcId = npcInfo.m_nId } );
			return npcPosPoint;
		end
	end

	for i = 1, MapShower:getMapSceneAreaCount( nCurMapID ) do
		local monsterInfo		= MapShower:getMapSceneAreaInfo( nCurMapID, i - 1 );
		local monsterPosPoint	= { x = monsterInfo.Pos.X, y = monsterInfo.Pos.Y };
		local nDist				= util.CalPointsDistance( mainPosPoint, monsterPosPoint );
		--print( "nDist = "..nDist.."�� STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--660" );
		if not self:isVisitedMonster( { monsterId = monsterInfo.MonsterID, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			--print( "getRandomPoint--���ʹ����ID��monsterId = "..monsterInfo.m_nId.."--663" );
			self:addVisitedMapMonster( { mapId = nCurMapID, monsterId = monsterInfo.MonsterID } );
			return monsterPosPoint;
		end
	end

	-- ��������������ʾû�з���������NPC��������NPC�б����
	--print( "��չ����б��NPC�б�--669" );
	self:clearVisitedMapNpcList( { mapId = nCurMapID } );
	self:clearVisitedMapMonsterList( { mapId = nCurMapID } );
	for i = 1, MapShower:getMapNpcCount( nCurMapID ) do
		local npcInfo		= MapShower:getMapNpcInfo( nCurMapID, i - 1 );
		local npcPosPoint	= { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		local nDist			= util.CalPointsDistance( mainPosPoint, npcPosPoint );
		--print( "nDist = "..nDist.."�� STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--676" );
		if not self:isVisitedNpc( { npcId = npcInfo.m_nId, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			---print( "getRandomPoint--����NPC��ID��npcId = "..npcInfo.m_nId.."--678" );
			self:addVisitedMapNpc( { mapId = nCurMapID, npcId = npcInfo.m_nId } );
			return { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		end
	end

	for i = 1, MapShower:getMapSceneAreaCount( nCurMapID ) do
		local monsterInfo		= MapShower:getMapSceneAreaInfo( nCurMapID, i - 1 );
		local monsterPosPoint	= { x = monsterInfo.Pos.X, y = monsterInfo.Pos.Y };
		local nDist				= util.CalPointsDistance( mainPosPoint, monsterPosPoint );
		--print( "nDist = "..nDist.."�� STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--688" );
		if not self:isVisitedMonster( { monsterId = monsterInfo.MonsterID, mapId = nCurMapID } ) and nDist >= STANDARD_COMP_SIZE then
			--print( "getRandomPoint--���ʹ����ID��monsterId = "..monsterInfo.m_nId.."--690" );
			self:addVisitedMapMonster( { mapId = nCurMapID, monsterId = monsterInfo.MonsterID } );
			return monsterPosPoint;
		end
	end
end

-- TODO: ��ʱ�Ľ������

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

-- ��ʼ���ڵ�ǰ��ͼ�ƶ��ļ�����
t_AutoRoundWorlldControl["initCurMapsMoveTimeCounter"] = 
function ( self )
	self["curMapInfo"]["haveMoveTimes"]		= 0;
	self["curMapInfo"]["lastRandomTime"]	= 0;
end

-- �����ڵ�ǰ��ͼ���ƶ�������
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

-- ���ӵ�ǰһ��������������������Ѿ��������ĵ�ͼ
t_AutoRoundWorlldControl["addHaveRoundMaps"] = 
function ( self, data )
	table.insert( self["haveRoundMaps"], data["mapID"] );
end

-- ��ȡ��ǰ�ƶ���Ŀ���
t_AutoRoundWorlldControl["getCurDestMovePoint"] = 
function ( self, data )
	return self["curDestPoint"];
end

t_AutoRoundWorlldControl["setCurDestMoveMap"] = 
function ( self, data )
	self["curDestPoint"]["mapID"]	= data["mapID"];
end

-- ���õ�ǰ�ƶ���Ŀ���
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
	-- ����һ��Ҫ�ﵽ��Ŀ���ͼû���������ܵ���һ��Ŀ���ͼ�����Ѿ�������һ��Ŀ���ͼ���������漴һ���µ�ͼ
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

-- �л���ͼ��ʱ�򣬻��ڵ�ͼ��������
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
	-- ����һ��Ҫ�ﵽ��Ŀ���ͼû���������ܵ���һ��Ŀ���ͼ�����Ѿ�������һ��Ŀ���ͼ���������漴һ���µ�ͼ
	self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
	self:initCurMapsMoveTimeCounter();
	self:moveToCurMapRandomPoint();
end

MAX_STOP_TIME	= 3;
-- ��ͬһ�ŵ�ͼ��ʱ�򣬻��ڵ�����
-- ���������ֻ��һ�����飺��������ֹͣ���ܶ�
t_AutoRoundWorlldControl["update"] = 
function ( self )
	-- �ƶ���ʱ����μ�⵱ǰ�ƶ����ĵ��Ƿ����Լ��ϴ����ƶ�����Ŀ���
	-- һ��Ҫ��ͬһ�ŵ�ͼ����
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
	-- ���������ϴ��Զ��ܶ���Ŀ��㣬��ʼ��һ���ܶ�
	-- TODO: �����������Ϣ����
	--print( "t_curDestPoint[mapID] = "..t_curDestPoint["mapID"].."�� nCurMapID = "..nCurMapID.."--910" );
	-- t_curDestPoint["mapID"] == nCurMapID and 
	if not mainplayer:getAutoFind() then
		-- ����һ����Ϣ�㣬��Ϣ5��
		-- ����ֹͣ�ܶ�״̬��Ϣ
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

-- �������ǵ��ƶ�״̬
t_AutoRoundWorlldControl["updateMoveState"] = 
function ( self )
	
end

-- ����ֹͣ�ƶ�״̬
t_AutoRoundWorlldControl["setStopState"] = 
function ( self, data )
	self["stopInfo"]["isStop"]		= data["isStop"];
	self["stopInfo"]["startTime"]	= data["startTime"];
end

-- �Ƿ����б�����ĵ�ͼ�������ʹ��ˣ�������
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

-- ���һ�������ͼID
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
	-- ���ѭ�����жϵ�һ�ֿ��������еĶ����ͼ�б�������ʹ���
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

-- ��ѯ�õ�ͼ�Ƿ��Ѿ����߹�
t_AutoRoundWorlldControl["isMapVisited"] = 
function ( self, data )
	for _, nHaveRoundMapID in ipairs( self["haveRoundMaps"] ) do
		if data["mapID"] == nHaveRoundMapID then
			return true;
		end
	end

	return false;
end

-- �Ƿ��ܼ����ڵ�ǰ��ͼ�ƶ�
t_AutoRoundWorlldControl["isPassMaxPointOnCurMap"] = 
function ( self )
	return self["curMapInfo"]["haveMoveTimes"] >= MAX_AUTO_STOP_NUM;
end

-- �Ƿ��ǹ����ĵ�ͼ
t_AutoRoundWorlldControl["isIsolateMap"] = 
function ( self, data )
	local pConnectedMaps = MapShower:getMultiMapFindControl():getWorldMapSearchDef( data["mapId"] );
	return pConnectedMaps == nil;
end

-- ������һ��Ŀ����Ѱ·
t_AutoRoundWorlldControl["continueMoveToLastRandomPoint"] = 
function ( self )
	local t_lastMoveDestPoint = self:getCurDestMovePoint();
	MaxMapControl:linkMoveto( self:getCurMapID(), t_lastMoveDestPoint["x"], t_lastMoveDestPoint["y"] );
end

-- ��㺯����ѡȡһ�����������ƶ�
t_AutoRoundWorlldControl["moveToCurMapRandomPoint"] = 
function ( self )
	-- ���ڵ�ǰ��ͼ�Ѿ��ƶ����������������ƶ�����һ�ŵ�ͼ
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
	
	-- ����Ҫһ��ѭ����ֱ����һ��������ƶ�Ϊֹ
	local mainplayer = ActorMgr:getMainPlayer();
	--TODO: ������Ӹ��жϣ�ͬʱ���ƶ���·���㲻С�ڶ������С�뾶��Χ
	while nMoveResult ~= MOVE_SUCCESS do
		-- TODO: ����Ƿ�������ѭ��
		--print( "t_AutoRoundWorlldControl--moveToCurMapRandomPoint--1028" );
		randomPoint	= self:getRandomPoint();
		nMoveResult = MaxMapControl:linkMoveto( self:getCurMapID(), randomPoint["x"], randomPoint["y"] );
	end
	
	self:incrCurMapsMoveTimeCounter();
	self:setCurDestMovePoint( { mapID = self:getCurMapID(), x = randomPoint["x"], y = randomPoint["y"] } );
end

-- ��ͼ�㺯�������������һ�ŵ�ͼ
t_AutoRoundWorlldControl["moveToRandomMap"] = 
function ( self )
	-- �����еĵ�ͼ�Ѿ���ȫ�������ˣ������±���
	if self:isAllDefMapsVisit() then
		-- ��յ�ͼ���������Ϣ
		self:clearAllHaveRoundMaps();
		-- TODO: DEL
		self:initAllDefMaps();

		self:addHaveRoundMaps( { mapID = self:getCurMapID() } );
	end
	
	local MaxMapControl	= MapShower:GetMaxMapContal();
	-- TODO: �������
	-- TODO: ���3������ͣ
	local nRandomDestMapID = self:getRandomMapID();
	
	local nMoveResult = MOVE_STATUS_FORBID;
	if nRandomDestMapID ~= 0 then
		local oneMovePoint = self:getOneCanMovePos( { mapId = nRandomDestMapID } );
		if oneMovePoint ~= nil then
			nMoveResult = MaxMapControl:linkMoveto( nRandomDestMapID, oneMovePoint["x"], oneMovePoint["y"] );
		end
	end

	-- ���޷��ɹ��ƶ�����ɾ�������ͼID��ͬʱ����Ѱ����һ�������ƶ��ĵ�ͼ
	local mainplayer = ActorMgr:getMainPlayer();
	-- TODO: �������⣬������յ���ʱ����������˹
	-- ����ͼû���κ�ͨ·����������ͼһֱ��
	while nMoveResult ~= MOVE_SUCCESS and not self:isDefMapsEmpty() do
		self:removeDefMaps( { mapID = nRandomDestMapID } );
		nRandomDestMapID = self:getRandomMapID();
		--print( "t_AutoRoundWorlldControl--moveToRandomMap--nRandomDestMapID = "..nRandomDestMapID.."--1068" );
		if nRandomDestMapID ~= 0 then
			local oneMovePoint = self:getOneCanMovePos( { mapId = nRandomDestMapID } );
			-- TOOD: Ϊʲô���ȡnilֵ
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
	-- TODO: �Ƿ�self:getCurMapIDȫ���滻���������ͼID
	local nCurMapID				=data["mapId"];

	for i = 1, MapShower:getMapNpcCount( nCurMapID ) do
		local npcInfo		= MapShower:getMapNpcInfo( nCurMapID, i - 1 );
		local npcPosPoint	= { x = npcInfo.m_nRealX, y = npcInfo.m_nRealY };
		--print( "nDist = "..nDist.."�� STANDARD_COMP_SIZE = "..STANDARD_COMP_SIZE.."--648" );
		return npcPosPoint;
	end

	for i = 1, MapShower:getMapSceneAreaCount( nCurMapID ) do
		local monsterInfo		= MapShower:getMapSceneAreaInfo( nCurMapID, i - 1 );
		local monsterPosPoint	= { x = monsterInfo.Pos.X, y = monsterInfo.Pos.Y };
		return monsterPosPoint;
	end
end

-- ��ȡ���ǵ�ǰ������ͼID
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
	-- TODO: ���ڸ������������绷�Σ���ǰ�ж��˸����򣬸��������Զ�Ѱ·��������ͼ
	local nCurMapID	= t_AutoRoundWorlldControl:getCurMapID(); -- TODO: �������
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "�������ܽ��л�������" );
		return;
	end

	-- �����һ��Ŀ���ͼ�����漴����Ŀ���ͼ�Ѿ��߹����������漴��һ����ͼ
	local t_AutoRoundWorlldControl = GetAutoRoundWorlldControl();
	t_AutoRoundWorlldControl:buildMapsCoord();
	t_AutoRoundWorlldControl:initAllDefMaps();
	-- ��յ�ͼ���������Ϣ
	t_AutoRoundWorlldControl:clearAllHaveRoundMaps();
	-- ��ʼ����ǰ��ͼ�ƶ�������
	t_AutoRoundWorlldControl:initCurMapsMoveTimeCounter();
	-- ����ʼ��ͼ��Ϊ�Ѿ��������ĵ�ͼ
	t_AutoRoundWorlldControl:addHaveRoundMaps( { mapID = t_AutoRoundWorlldControl:getCurMapID() } );
	t_AutoRoundWorlldControl:setAroundWorldState( { state = true } );
	-- ����ƶ�����һ�ŵ�ͼ
	-- ���µ�ͼ����Ϣ
	-- ��ʼ�ڵ�ǰ��ͼ�ƶ�
	t_AutoRoundWorlldControl:moveToCurMapRandomPoint()
	t_AutoRoundWorlldControl:setCurDestMoveMap( { mapID = t_AutoRoundWorlldControl:getCurMapID() } );
end

function StopAutoRoundWorlld()
	t_AutoRoundWorlldControl:setAroundWorldState( { state = false } );
	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:CmdMoveStop();
end