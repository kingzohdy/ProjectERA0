
local SINGLE_BUBBLE_HOUSE_PWORLD_ID = 209
local TEAM_BUBBLE_HOUSE_PWORLD_ID = 210

local MAX_SINGLE_BUBBLE_HOUSE_MONSTER = 30
local MAX_TEAM_BUBBLE_HOUSE_MONSTER = 60

local curPworldMaxMonster = 60

function BubbleHouseFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_PWORLD" )
	this:RegisterEvent( "GE_LEAVE_PWORLD" )
end

function IsInBubbleHouse()
	local mapid = MapShower:getRealMapId();
	if mapid >= CLIENT_GAME_PWORLD_MAPID then
		local nPworldId = mapid/100;
		if nPworldId == SINGLE_BUBBLE_HOUSE_PWORLD_ID or nPworldId == TEAM_BUBBLE_HOUSE_PWORLD_ID then
			return true
		end
	end
	return false;
end

function SetBubbleHouseMaxMonster()
	local mapid = MapShower:getRealMapId();
	if mapid >= CLIENT_GAME_PWORLD_MAPID then
		local nPworldId = mapid/100;
		if nPworldId == SINGLE_BUBBLE_HOUSE_PWORLD_ID then
			curPworldMaxMonster = MAX_SINGLE_BUBBLE_HOUSE_MONSTER
		elseif nPworldId == TEAM_BUBBLE_HOUSE_PWORLD_ID then
			curPworldMaxMonster = MAX_TEAM_BUBBLE_HOUSE_MONSTER
		end
	end
end

function GetBubbleMonsterNum()
	local stoneCount,scissorsCount,ClothWrapper = 0, 0, 0
	for i = 1, ActorMgr:getActorNums() do
		local actor = ActorMgr:getActor( i - 1 )
		if actor ~= nil then
			if actor:getName() == "Ê¯Í·" then
				stoneCount = stoneCount + 1
			elseif actor:getName() == "¼ôµ¶" then
				scissorsCount = scissorsCount + 1
			elseif actor:getName() == "°ü¸¤" then
				ClothWrapper = ClothWrapper + 1
			end
		end
	end
	return stoneCount,scissorsCount,ClothWrapper
end

function BubbleHouseFrame_OnEvent()
	if arg1 == "GE_ENTER_PWORLD" then
		if IsInBubbleHouse() then
			SetBubbleHouseMaxMonster()
			BubbleHouseFrame:Show()
		end
	elseif arg1 == "GE_LEAVE_PWORLD" then
		if IsInBubbleHouse() then
			BubbleHouseFrame:Hide()
		end
	end
end
function BubbleHouseFrame_OnUpdate()
	local stones, scissors, clothWrapper = GetBubbleMonsterNum()
	BubbleHouseFrameStoneNum:SetSize( 66*stones/curPworldMaxMonster, 9 )
	BubbleHouseFrameScissorsNum:SetSize( 66*scissors/curPworldMaxMonster, 9 )
	BubbleHouseFrameClothWrapperNum:SetSize( 66*clothWrapper/curPworldMaxMonster, 9 )
end
