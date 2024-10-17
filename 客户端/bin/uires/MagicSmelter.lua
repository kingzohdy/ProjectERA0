local result
local lastTick
local stop
local resetOne
local MAX_SMELTER_NUM = 5;
local MAX_REWARD_NUM = 5;
local RECAST_ITEM_ID = 5052310;
local waitTime = 0.1;
local loop = 0;
local needResult;
local randomKey;
local nowPoint;
local t_SmelterList;
local t_RecastBtn = { "MagicSmelterFrameRecastOnce", "MagicSmelterFrameSmelterItem4RecastThis", "MagicSmelterFrameSmelterItem5RecastThis" };
local t_PinZhi = { 
			[0] = { "白色", 255, 255, 255, "白色：收益极少" }, 
			[1] = { "绿色", 0, 255, 0, "绿色：收益普通" },
			[2] = { "蓝色", 0, 255, 255, "蓝色：收益一般" },
			[3] = { "紫色", 255, 70, 255, "紫色：收益丰富" },
			[4] = { "金色", 200, 200, 100, "金色：收益巨大" },
			};

local t_RewardBtn = {
			[1] = { 115, 0, 0, 0, 0 },
			[2] = { 91, 139, 0, 0, 0 },
			[3] = { 67, 115, 163, 0, 0},
			[4] = { 43, 91, 139, 187, 0 },
			[5] = { 19, 67, 115, 163, 211 },
			};

function GetAllSmelterItem()
	t_SmelterList = {};
	for i=1, MAX_SMELTER_NUM do
		local id = MagicSmelterManager:getRongluIdByPinZhi( i - 1 );
		if id ~= 0 then
			table.insert( t_SmelterList, id );
		end
	end
end

function MagicSmelterFrame_OnLoad()
	MagicSmelterFrame:setUpdateTime(0);
	this:RegisterEvent("GE_QUEST_ITEM_CHANGE");
	this:RegisterEvent("GE_QUEST_DELQUESTRUN");
	GetAllSmelterItem();
	local t_tex = { "Bai.tga", "Lv.tga", "Lan.tga", "Zi.tga", "Jin.tga" };
	for i=1, MAX_SMELTER_NUM do
		local btn = getglobal("MagicSmelterFrameSmelterItem" .. i);
		btn:SetClientString( t_PinZhi[i-1][5] );
		local tex = getglobal("MagicSmelterFrameSmelterItem" .. i .. "Icon");
		tex:SetTexture( "uires\\TuPianLei\\RongLu\\" .. t_tex[i] );
		if i < MAX_SMELTER_NUM - 1 then
			tex = getglobal("MagicSmelterFrameSmelterItem" .. i .. "RecastThis");
			tex:Hide();
		end
	end
end

function MagicSmelterRecastOnce_OnClick()
	if nowPoint == 5 then
		ShowMidTips("熔炉当前已是最高等级，不需要重铸");
		return;
	end

	waitTime = 0.1;
	resetOne = true;
	stop = false;
	randomKey = math.random( 1, MAX_SMELTER_NUM );--随机开始减速的时间
	if MagicSmelterFrame:GetClientUserData(2) > 0 then
		MagicSmelterManager:requestRecastSmelter( MagicSmelterFrame:GetClientUserData(1) );
		for i=1, #t_RecastBtn do
			local btn = getglobal( t_RecastBtn[i] );
			btn:Disable();
		end
	end
end

function MagicSmelterRecastThis_OnClick()
	if nowPoint == 5 then
		ShowMidTips("熔炉当前已是最高等级，不需要重铸");
		return;
	end
	
	local parent = getglobal( this:GetParent() );
	if nowPoint == parent:GetClientID() then
		ShowMidTips("熔炉当前已是该等级，不需要重铸");
		return;
	end

	waitTime = 1;
	resetOne = false;
	needResult = parent:GetClientID();
	if MagicSmelterFrame:GetClientUserData(2) > 0 then
		MagicSmelterManager:requestRecastSmelter( MagicSmelterFrame:GetClientUserData(1) );
		for i=1, #t_RecastBtn do
			local btn = getglobal( t_RecastBtn[i] );
			btn:Disable();
		end
	end
	MagicSmelterFrameRecastStop:Show();
end

function MagicSmelterRecastStop_OnClick()
	this:Hide();
	result = nil;
	needResult = nil;
	lastTick = nil;
	if GetRecastItemNum() > 0 then
		for i=1, #t_RecastBtn do
			local btn = getglobal( t_RecastBtn[i] );
			btn:Enable();
		end
	end
end

function SetRecastResult( id )
	local rongluDef = MagicSmelterManager:getRongluDef( id );
	if rongluDef ~= nil then
		result = rongluDef.Pinzhi + 1;--确保收到结果后再开始循环
		lastTick = getServerTime();
	else
		if needResult ~= nil then
			RepeatRecastLoop();
		else
			MagicSmelterFrameRecastStop:Hide();
			if GetRecastItemNum() > 0 then
				for i=1, #t_RecastBtn do
					local btn = getglobal( t_RecastBtn[i] );
					btn:Enable();
				end
			end
		end
	end
end

function MagicSmelterStopTurn()
	if result then
		stop = true;
		waitTime = 0.5;
		loop = 0;
	end
end

function DisCheckSelSmelter()
	for i=1, MAX_SMELTER_NUM do
		local btn = getglobal("MagicSmelterFrameSmelterItem" .. i);
		btn:DisChecked();
	end
end

function SetCheckSmelter( idx )
	DisCheckSelSmelter();
	local btn = getglobal("MagicSmelterFrameSmelterItem" .. idx);
	btn:Checked();
end

function RepeatRecastLoop()
	GetSmelterItem();
	if MagicSmelterFrame:GetClientUserData(2) > 0 then
		MagicSmelterManager:requestRecastSmelter( MagicSmelterFrame:GetClientUserData(1) );
	end
end


function UpdateRewardItems( id, idx )
	if id == 0 or idx < 1 or idx > MAX_REWARD_NUM then
		return;
	end
	local name = "MagicSmelterFrameSmelterReward" .. idx;
	local btn = getglobal(name);
	btn:SetClientUserData( 0, id );
	btn:Show();

	local icon = getglobal(name .. "Icon")
	if icon then
		local ItemDef = getItemDef(id)
		local IconPath = DEFAULT_ITEM_PATH
		if ItemDef then
			IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
		end
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH
		end
		icon:SetTexture(IconPath)
		icon:SetTexRelUV(1, 1)
		SetEquipImportantEffectUV(ItemDef, name.."UVAnimationTex")
	end
end

function GetRecastItemNum()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return 0;
	end
	local container	= mainplayer:getContainer();
	return container:getItemCount( RECAST_ITEM_ID );
end

function UpdateRecastResult( id )
	if MagicSmelterFrame:IsShown() then
		local rongluDef = MagicSmelterManager:getRongluDef( id );
		local lv = MagicSmelterManager:getRongluLv();
		local num = 0;
		if rongluDef ~= nil then
			for i=1, MAX_REWARD_NUM do
				if rongluDef.Grade[lv].IconIDs[num] == 0 then
					break;
				else
					UpdateRewardItems( rongluDef.Grade[lv].IconIDs[num], i );
					num = num + 1;
				end
			end
			MagicSmelterFramePinZhiFont:SetText( t_PinZhi[rongluDef.Pinzhi][1] );
			MagicSmelterFramePinZhiFont:SetTextColor( t_PinZhi[rongluDef.Pinzhi][2], t_PinZhi[rongluDef.Pinzhi][3], t_PinZhi[rongluDef.Pinzhi][4] );
		end

		for i=1, num do
			local btn = getglobal( "MagicSmelterFrameSmelterReward" .. i );
			btn:SetPoint( "topleft", "MagicSmelterFrame", "topleft", t_RewardBtn[num][i], 183 );
		end

		for i = num + 1, MAX_REWARD_NUM do
			local btn = getglobal( "MagicSmelterFrameSmelterReward" .. i );
			btn:SetClientUserData( 0, 0 );
			btn:Hide();
		end

		if not needResult then
			MagicSmelterFrameRecastStop:Hide();
			if GetRecastItemNum() > 0 then
				for i=1, #t_RecastBtn do
					local btn = getglobal( t_RecastBtn[i] );
					btn:Enable();
				end
			end
		end
	end
end

function MagicSmelterFrame_OnUpdate()
	if resetOne then
		if lastTick then
			local now = getServerTime()
			local during = (now.sec - lastTick.sec) + (now.usec - lastTick.usec) * 0.001
			local needChange = true;
			if during >= waitTime then
				if stop then
					lastTick = now
					if loop > 0 then
						if nowPoint == result then
							MagicSmelterManager:endRecastSmelter( MagicSmelterFrame:GetClientUserData(1), t_SmelterList[result] );
							lastTick = nil;
							result = nil;
							loop = 0;
							needChange = false;
						else
							nowPoint = nowPoint + 1 > MAX_SMELTER_NUM and 1 or (nowPoint + 1);
						end
					else
						if nowPoint == MAX_SMELTER_NUM then
							loop = loop + 1;
						end
						nowPoint = nowPoint + 1 > MAX_SMELTER_NUM and 1 or (nowPoint + 1);
					end
				else
					lastTick = now
					if nowPoint == MAX_SMELTER_NUM then
						loop = loop + 1;
					end
					nowPoint = nowPoint + 1 > MAX_SMELTER_NUM and 1 or (nowPoint + 1);
					if loop > 4 and nowPoint == randomKey then
						MagicSmelterStopTurn();
					end
				end
				if needChange then
					MagicSmelterManager:playRongluRecastSound();
					SetCheckSmelter( nowPoint );
				end
			end
		end
	else
		if needResult and lastTick then
			local now = getServerTime()
			local during = (now.sec - lastTick.sec) + (now.usec - lastTick.usec) * 0.001
			if during >= waitTime and result ~= nil then
				lastTick = now;
				nowPoint = result;
				MagicSmelterManager:playRongluRecastSound();
				SetCheckSmelter( nowPoint );
				MagicSmelterManager:endRecastSmelter( MagicSmelterFrame:GetClientUserData(1), t_SmelterList[nowPoint] );
				if nowPoint >= needResult then
					needResult = nil;
				else
					local mainplayer = ActorMgr:getMainPlayer();
					if mainplayer ~= nil then
						local container	= mainplayer:getContainer();
						local num = container:getItemCount( RECAST_ITEM_ID );
						if num > 1 then
							RepeatRecastLoop();
						else
							ShowMidTips("背包中已经没有重铸符，重铸停止");
							needResult = nil;
						end
					end
				end
				result = nil;
				lastTick = nil;
			end
		end
	end
end

function SetRecastItem()
	--查找当前背包中的熔炉重铸石
	local num = GetRecastItemNum();
	if num == 0 then
		for i=1, #t_RecastBtn do
			local btn = getglobal( t_RecastBtn[i] );
			btn:Disable();
		end
	end
	local color = num > 0 and "#cffa000" or "#cff0000";
	local szText = "更换一次，需要消耗一个【熔炉重铸符】\n当前的【熔炉重铸符】还有" .. color .. " " .. num .. " #n个";
	MagicSmelterFrameRecastItemFont:SetText( szText, 255, 255, 190 );
end

function InitMagicSmelterFrame()
	result = nil
	lastTick = nil
	stop = false;
	needResult = nil;
	MagicSmelterFrame:SetClientUserData(0, 0);
	MagicSmelterFrame:SetClientUserData(1, 0);
	MagicSmelterFrame:SetClientUserData(2, 0);
	MagicSmelterFrame:SetClientUserData(3, 0);

	for i=1, #t_RecastBtn do
		local btn = getglobal( t_RecastBtn[i] );
		btn:Enable();
	end
end

function UnLockSmelterItem()
	local nType = MagicSmelterFrame:GetClientUserData( 0 );
	local nIdx = MagicSmelterFrame:GetClientUserData( 1 );
	if MagicSmelterFrame:GetClientUserData( 2 ) ~= 0 then
		UnLockItem( nType, nIdx );
	end
end

function GetSmelterItem()
	UnLockSmelterItem();
	local questItemID = MagicSmelterManager:getRongluRecastEndID();
	if questItemID == 0 then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container	= mainplayer:getContainer();
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		local itemDef = item:getItemDef();
		if itemDef ~= nil and itemDef.Result[0].ResultID == RESULT_RONGLU then
			if itemDef.ItemID == questItemID then
				MagicSmelterFrame:SetClientUserData(0, item:getListType());
				MagicSmelterFrame:SetClientUserData(1, item:getGridIdx());
				MagicSmelterFrame:SetClientUserData(2, itemDef.ItemID);
				LockItem( item:getListType(), item:getGridIdx(), itemDef.ItemID );
				local rongluDef = MagicSmelterManager:getRongluDef( itemDef.ItemID );
				if rongluDef ~= nil then
					MagicSmelterFrame:SetClientUserData(3, rongluDef.Pinzhi + 1);
				end
				break;
			end
		end
	end
end

function OpenMagicSmelterFrame( npcId )
	if MagicSmelterFrame:IsShown() then
		return;
	end
	InitMagicSmelterFrame();
	GetSmelterItem();
	nowPoint = MagicSmelterFrame:GetClientUserData(3);
	if nowPoint == 0 then
		ShowMidTips("背包中没有可用的熔炉，无法进行重铸操作");
		return;
	end
	SetCheckSmelter(nowPoint);
	SetRecastItem();
	MagicSmelterFrameRecastStop:Hide();
	MagicSmelterFrame:Show();
	--util.testNpcDialogDistance( npcId, MagicSmelterFrame );
end

function MagicSmelterFrame_OnEvent()
	if arg1 == "GE_QUEST_ITEM_CHANGE" then
		if MagicSmelterFrame:IsShown() then
			SetRecastItem();
			if not needResult then
				GetSmelterItem();
			end
		end
	elseif arg1 == "GE_QUEST_DELQUESTRUN" then
		--删除熔炉任务时关闭打开的重铸面板
		if MagicSmelterFrame:IsShown() then
			local bShow = false;
			local nNum = Quest:getNumQusetRun();
			for i = 1, nNum do
				local nQuestId = Quest:getQuestRunId(i-1);
				if nQuestId > 0 and Quest:getQuestType(nQuestId) == CLIENT_TASK_TYPE_RONGLU then
					bShow = true;
					break;
				end
			end

			if not bShow then
				MagicSmelterFrame:Hide();
			end
		end
	end
end

function MagicSmelterFrame_OnShow()
	util.ShowInCenter(this);
	UpdateRecastResult( MagicSmelterFrame:GetClientUserData(2) );
end

function MagicSmelterFrame_OnHide()
	MagicSmelterManager:breakRecastSmelter();
	UnLockSmelterItem()
end

function SmelterItem_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), frame = this:GetParent(),
							button = this:GetName() } );
end

function SmelterItem_OnLeave()
	GameTooltip:Hide();
end

function SmelterReward_OnEnter()
	local itemid = this:GetClientUserData(0)
	if itemid ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemid));
		return;
	end
end

function SmelterReward_OnLeave()
	NormalItemTipsFrame:Hide();
end

function MagicSmelterRecastThis_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "点击后，自动更换到这个品质为止", frame = this:GetParent(),
							button = this:GetName() } );
end

function MagicSmelterRecastThis_OnLeave()
	GameTooltip:Hide();
end