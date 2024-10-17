local useItemNum = 1;
local useMoneyNum = 0;
local activeIdx = 0;

function ArmSkillActiveFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ItemFirstCallBack( "ArmSkillActiveFrame", Save2ArmSkillActiveItem );
	local szText = [[●只有存在未激活技能属性的装备才能进行激活
●每次激活需要消耗一定数量的激活石
●技能激活后穿戴上即获得相应技能的加成
	]]
	ArmSkillActiveFrameRich:SetText( szText, 160, 155, 130 );
end

local function CheckCostEnough()
	local stuffItemId = ArmSkillActiveFrameStuffItemBtn:GetClientUserData(2);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		local container	= mainplayer:getContainer();
		local num = container:getItemCount( stuffItemId );
		local font = getglobal( "ArmSkillActiveFrameNeedFont" );
		if num < useItemNum then
			font:SetTextColor( 230, 0, 0 );
		else
			font:SetTextColor( 160, 160, 90 );
		end

		font = getglobal( "ArmSkillActiveFrameCostFont" );
		if mainplayer:getMoney() < useMoneyNum then
			font:SetTextColor( 230, 0, 0 );
		else
			font:SetTextColor( 160, 160, 90 );
		end
	end

	if ArmSkillActiveFrameOKBtn:GetText() ~= "关  闭" and 
	ArmSkillActiveFrameArmItemBtn:GetClientUserData(2) > 0 then
		return true;
	end
	return false;
end

local function ClearItemBtn( name )
	local btn = getglobal( name );
	local UVAnimationTex = getglobal( name.."UVAnimationTex");
	local ItemTex = getglobal( name.."IconTexture");
	local BindTex = getglobal( name.."BindTexture");
	local ItemNum = getglobal( btn:GetName().."CountFont");
	if btn:GetClientUserData(2) > 0 then
		UnLockItem( btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );
	end
	btn:SetClientUserData( 0, 0 );
	btn:SetClientUserData( 1, 0 );
	btn:SetClientUserData( 2, 0 );
	btn:SetClientUserData( 3, 0 );
	UVAnimationTex:Hide();
	ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	BindTex:Hide();
	ItemNum:SetText("");
	ArmStepUpFrameOKBtn:Disable();
end

local function SetItemBind( name, bBind )
	local btn = getglobal( name );
	local bindTex = getglobal( btn:GetName() .. "BindTexture" );
	btn:SetClientUserData( 3, bBind and 1 or 0 );
	if bBind then
		bindTex:Show();
	else
		bindTex:Hide();
	end
end

local function SetItemNum( name, id )
	local mainplayer = ActorMgr:getMainPlayer();
	local totalNum = 0;
	if mainplayer ~= nil then
		local container	= mainplayer:getContainer();
		totalNum = container:getItemCount( id );
		totalNum = totalNum > 999 and ( math.floor(totalNum/1000) .. "k" ) or totalNum;
	end
	
	local font = getglobal( name .. "CountFont" );
	font:SetText( totalNum .. "/" .. useItemNum );
	totalNum = string.gsub( totalNum, "k", "" );
	local t_Color = { { 255, 255, 0 }, { 230, 0, 0 } };
	local idx = useItemNum > tonumber(totalNum) and 2 or 1;
	font:SetTextColor( t_Color[idx][1], t_Color[idx][2], t_Color[idx][3] );
	local icon = getglobal( name .. "IconTexture");
	icon:SetGray( idx > 1 );
end

local function SetItemBtn( name, item, itemId )
	local btn = getglobal( name );
	local ItemTex = getglobal( btn:GetName().."IconTexture");
	local BindTex = getglobal( btn:GetName().."BindTexture");
	local ItemNum = getglobal( btn:GetName().."CountFont");
	if btn:GetClientUserData(2) > 0 then
		UnLockItem( btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );
	end
	btn:SetClientUserData( 0, item and item:getListType() or 0 );
	btn:SetClientUserData( 1, item and item:getGridIdx() or 0 );
	btn:SetClientUserData( 2, item and item:getItemId() or itemId );
	local nBind = 0;
	if item then
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId() );
		nBind = item:isItemBind() and 1 or 0;
	end
	btn:SetClientUserData( 3, nBind );
	if nBind > 0 then
		BindTex:Show();
	else
		BindTex:Hide();
	end
	
	local itemDef = item and item:getItemDef() or getItemDef(itemId);
	local IconPath = GetItemEquipIconPath() .. itemDef.IconID .. ".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ItemTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimationTex" );
	if itemDef.MaxPile > 1 then
		SetItemNum( btn:GetName(), itemDef.ItemID );
	else
		ItemNum:SetText("");
	end

	if CheckCostEnough() then
		ArmSkillActiveFrameOKBtn:Enable();
	else
		ArmSkillActiveFrameOKBtn:Disable();
	end
end

local function initArmSkillActiveFrame()
	ClearItemBtn( "ArmSkillActiveFrameArmItemBtn" );
	ArmSkillActiveFrameOKBtn:Disable();
	ArmSkillActiveFrameOKBtn:SetText( "激活属性" );
	ArmSkillActiveFrameNeedFont:SetText(" ");
	ArmSkillActiveFrameCostFont:SetText("0");
	ArmSkillActiveFrameStep:Hide();
	useItemNum = 1;
	useMoneyNum = 0;
	activeIdx = 0;
end

local function CheckCanPutArmItem( item, itemDef )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips( "请放入要激活技能属性的装备" );
		return false;
	end
	local nMax, num = item:getArmSkillMax(), item:getArmSkillNum();
	if nMax == 0 or nMax == num then
		ShowMidTips( "此装备没有需要激活的技能属性" );
		return false;
	end
	local useDef = GameArmProce:getArmSkillNumDef( math.floor( itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important, itemDef.WearPos1 );
	local stuffItemId = ArmSkillActiveFrameStuffItemBtn:GetClientUserData(2);
	if useDef == nil or useDef.Active[num].UseItemID ~= stuffItemId then
		ShowMidTips( "放入的装备与激活石不符" );
		return false;
	end
	return true;
end

local function SetArmSkillInfo( item, state )
	if MessageBoxFrameForVip:IsShown() and MessageBoxFrameForVipAcceptButton:GetText() == "购买银币" then
		MessageBoxFrameForVip:Hide();
	end

	local itemDef = item:getItemDef();
	local useDef = GameArmProce:getArmSkillNumDef( math.floor( itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important, itemDef.WearPos1 );
	local t_Font = { "StepNowFont", "StepNextActiveFont", "StepNextFont", "StepNextStateFont", "NeedFont", "CostFont" };
	local t_Desc = {};
	local nMax = item:getArmSkillMax();
	table.insert( t_Desc, getArmSkillDesc( item:getItemDef(), item:getArmSkillOne(activeIdx) ) );
	local szDesc, szState, szNext = " ", " ", " ";
	if activeIdx + 1 ~= nMax then
		szDesc = getArmSkillDesc( item:getItemDef(), item:getArmSkillOne(activeIdx+1) );
		szState = "（未激活）";
		szNext = "下次可激活属性：";
	end
	table.insert( t_Desc, szNext );
	table.insert( t_Desc, szDesc );
	table.insert( t_Desc, szState );
	useItemNum = useDef.Active[activeIdx].UseItemNum;
	table.insert( t_Desc, state and " " or (getItemDef( useDef.Active[activeIdx].UseItemID ).Name .. useItemNum .. "个") );
	useMoneyNum = useDef.Active[activeIdx].UseMoney;
	table.insert( t_Desc, state and " " or useMoneyNum );

	for i=1, #t_Font do
		local font = getglobal( "ArmSkillActiveFrame" .. t_Font[i] );
		if t_Desc[i] ~= nil then
			font:SetText( t_Desc[i] );
		end
	end

	if state then
		ArmSkillActiveFrameStepNowStateFont:SetText( "（已激活）" );
		ArmSkillActiveFrameStepNowStateFont:SetTextColor( 255, 150, 0 );
		ArmSkillActiveFrameOKBtn:SetText( state );
	else
		ArmSkillActiveFrameStepNowStateFont:SetText( "（未激活）" );
		ArmSkillActiveFrameStepNowStateFont:SetTextColor( 160, 155, 130 );
		ArmSkillActiveFrameOKBtn:SetText( "激活属性" );
	end

	local btn = getglobal( "ArmSkillActiveFrameStuffItemBtn" );
	SetItemNum( btn:GetName(), btn:GetClientUserData(2) );

	if not ArmSkillActiveFrameStep:IsShown() then
		ArmSkillActiveFrameStep:Show();
	end
end

function ArmSkillActiveFrame_OnEvent()
	if not this:IsShown() then return end;
	if arg1 == "GE_CONTAINER_UPDATE" then
		local btn = getglobal( "ArmSkillActiveFrameStuffItemBtn" );
		local nListType = btn:GetClientUserData(0);
		local nGridIndex = btn:GetClientUserData(1);
		local itemId = btn:GetClientUserData(2);
		SetItemNum( btn:GetName(), itemId );
		--[[
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then return end;
		local container	= mainplayer:getContainer();
		local item = container:getItem( nListType, nGridIndex );
		local num = container:getItemCount( itemId );
		if item:getItemId() == 0 and num > 0 then
			local t_Item = {};
			for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
				item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
				if item:getItemId() == itemId then
					table.insert( t_Item, { item, (item:isItemBind() and 1 or 0) } );
				end
			end

			table.sort( t_Item, function( a,b )
			return a[2] > b[2];
			end);
			item = t_Item[1][1];
			SetItemBtn( btn:GetName(), item );
		end
		--]]
		CheckCostEnough();
	elseif arg1 == "GE_CHANGE_MONEY" then
		CheckCostEnough();
	end
end

function Save2ArmSkillActiveItem( item, nSrcList, nSrcGridIndex )
	if MessageBoxFrameForVip:IsShown() and MessageBoxFrameForVipAcceptButton:GetText() == "购买银币" then
		MessageBoxFrameForVip:Hide();
	end
	if ArmSkillActiveFrameOKBtn:GetText() == "关  闭" then
		return ShowMidTips( "请先关闭当前窗口" );
	end

	local itemDef = item:getItemDef();
	if itemDef == nil then return end;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	if not CheckCanPutArmItem( item, itemDef ) then
		return;
	end
	activeIdx = item:getArmSkillNum();
	SetArmSkillInfo( item );
	SetItemBtn( "ArmSkillActiveFrameArmItemBtn", item );
end

function ArmSkillActiveFrameItemBtn_OnClick()
	if arg1 == "RightButton" and string.find( this:GetName(), "ArmItemBtn", 1, true ) then
		if this:GetClientUserData(2) == 0 then
			return;
		end
		if ArmSkillActiveFrameOKBtn:GetText() == "关  闭" then
			ArmSkillActiveFrame:Hide();
			return;
		end
		initArmSkillActiveFrame();
		if MessageBoxFrameForVip:IsShown() and MessageBoxFrameForVipAcceptButton:GetText() == "购买银币" then
			MessageBoxFrameForVip:Hide();
		end
	end
end

function ArmSkillActiveFrameItemBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();

	local nListType = this:GetClientUserData(0);
	local nGridIndex = this:GetClientUserData(1);
	local itemId = this:GetClientUserData(2);
	local item = container:getItem( nListType, nGridIndex );
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	elseif itemId > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemId) );
	else
		if string.find( this:GetName(), "ArmItemBtn", 1, true ) then
			SetGameTooltips( this:GetName(), "请放入要激活技能属性的装备" );
		else
			SetGameTooltips( this:GetName(), "请放入激活石" );
		end
	end
end

function ArmSkillActiveFrameItemBtn_OnBeginDrag()
	if string.find( this:GetName(), "StuffItemBtn", 1, true ) then
		return;
	end
	local nListType	= this:GetClientUserData(0);
	local nGrid	= this:GetClientUserData(1);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_SKILL_ACTIVE;
		local IconTex = getglobal( this:GetName().."IconTexture" );
		UIBeginDrag( IconTex:GetTexture(), item:getListType(), item:getGridIdx(), 0 );
	end
end

function ArmSkillActiveFrameItemBtn_OnRecive()
	-- 如果目标来自背包
	if UIMSG:getUIGV().DragFrom == CONTAINER_TYPE_SUNDRIES then
		if ArmSkillActiveFrameOKBtn:GetText() == "关  闭" then
			return ShowMidTips( "请先关闭当前窗口" );
		end

		if string.find( this:GetName(), "ArmItemBtn", 1, true ) then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end
			local container = mainplayer:getContainer();
			local srcList	= UIMSG:getUIGV().DragFrom;			-- 源容器类型
			local srcGrid	= UIGetDragUserData1();				-- 从源容器哪个格子过来
			local item	= container:getItem( srcList, srcGrid );
			if item:getItemId() == 0 then
				UIEndDrag();
				UIMSG:getUIGV().DragFrom = DRAG_NONE;
				return;
			end
			local itemDef	= item:getItemDef();
			if not CheckCanPutArmItem( item, itemDef ) then
				return;
			end

			activeIdx = item:getArmSkillNum();
			SetArmSkillInfo( item );
			SetItemBtn( "ArmSkillActiveFrameArmItemBtn", item );
		end
	end
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function ArmSkillActiveFrameOKBtn_OnClick()
	if this:GetText() == "关  闭" then
		ArmSkillActiveFrame:Hide();
		return;
	end
	local armItemId = ArmSkillActiveFrameArmItemBtn:GetClientUserData(2);
	if armItemId == 0 then
		return ShowMidTips( "请先放入需要激活属性的装备" );
	end
	local stuffItemId = ArmSkillActiveFrameStuffItemBtn:GetClientUserData(2);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container	= mainplayer:getContainer();
	local num = container:getItemCount( stuffItemId );
	if num < useItemNum then
		return ShowMidTips( getItemDef( stuffItemId ).Name .. "数量不足，激活失败" );
	elseif mainplayer:getMoney() < useMoneyNum then
		ShowBuyMoneyFrame();
		return ShowMidTips( "银币不足，激活失败" );
	end
	this:Disable();
	
	local armList = ArmSkillActiveFrameArmItemBtn:GetClientUserData(0);
	local armGrid = ArmSkillActiveFrameArmItemBtn:GetClientUserData(1);
	local useList = ArmSkillActiveFrameStuffItemBtn:GetClientUserData(0);
	local useGrid = ArmSkillActiveFrameStuffItemBtn:GetClientUserData(1);
	GameArmProce:requestArmSkillActive( activeIdx, armList, armGrid, useList, useGrid );
end

function ArmSkillActiveFrame_OnShow()
	if PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	else
		PlayerContainerFrame:Show();
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	end
	this:SetPoint( "topright","PlayerContainerFrame","topleft", 0, 0 );
end

function ArmSkillActiveFrame_OnHide()
	initArmSkillActiveFrame();
	ClearItemBtn( "ArmSkillActiveFrameStuffItemBtn" );
end

function UseArmSkillActiveItem( nListType, nGridIdx )
	if ArmSkillActiveFrame:IsShown() then
		ArmSkillActiveFrame:Hide();
	else
		initArmSkillActiveFrame();
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx );
	if item:getItemId() == 0 then return end;
	SetItemBtn( "ArmSkillActiveFrameStuffItemBtn", item );
	ArmSkillActiveFrame:Show();
end

function ArmSkillActiveResult( flag )
	local mainplayer = ActorMgr:getMainPlayer();
	if flag == 0 or mainplayer == nil then return end;
	local btn = getglobal( "ArmSkillActiveFrameArmItemBtn" );
	local modelview = getglobal( btn:GetName() .. "ModelView" );
	modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent");
	modelview:setPetScale( 0, 0.9, 0.9, 0.3 );
	modelview:setEntityPosition(0, 0, 100, 0);
	if not modelview:IsShown() then
		modelview:Show()
	end

	local container	= mainplayer:getContainer();
	local nListType = btn:GetClientUserData(0);
	local nGridIndex = btn:GetClientUserData(1);
	local item = container:getItem( nListType, nGridIndex );
	SetItemBind( btn:GetName(), item:isItemBind() );
	SetArmSkillInfo( item, "关  闭" );
	ArmSkillActiveFrameOKBtn:Enable();
end

function ClearArmSkillActiveBtn( btnName, desList, desGrid )
	local btn = getglobal( btnName );
	local nListType = btn:GetClientUserData(0);
	local nGridIdx	= btn:GetClientUserData(1);
	local itemid	= btn:GetClientUserData(2);
	if itemid > 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local item = container:getItem( nListType, nGridIdx );
		if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
			initArmSkillActiveFrame();
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
			if ArmSkillActiveFrameOKBtn:GetText() == "关  闭" then
				ArmSkillActiveFrame:Hide();
			end
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end
