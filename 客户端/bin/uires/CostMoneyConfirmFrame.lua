--------------------------------------- 银币消耗二次提示框 -------------------------------------------
local bShowCostWarn = true;
local WarnFrame = nil;

function CostMoneyConfirmFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	CostMoneyConfirmFrameMessageFont:SetText("绑银不足，继续操作将直接消耗银币。\n\t\t\t\t\t\t是否继续操作？");
end

function CostMoneyConfirmFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		bShowCostWarn = true;
	end
end

function SetWarnFrame( frame )
	WarnFrame = frame;
end

function checkNeedWarnCost( nValue )
	if not bShowCostWarn or nValue == nil or nValue <= 0 then
		return false;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getBull() < nValue then
		return true;
	end

	local levelupDef = getLevelUP( mainplayer:getLv() );
	if levelupDef ~= nil then
		local leftNum = levelupDef.DailyUseBull - mainplayer:getDailyUsedBull();
		if leftNum < nValue then
			return true;
		end
	end
	return false;
end

function CostMoneyConfirmFrame_OnShow()
	CostMoneyConfirmFrameHideBtn:setCheckState( false );
	CostMoneyConfirmFrame:AddLevelRecursive(1);
	util.ShowInCenter(this);
end

function CostMoneyConfirmFrame_OnHide()
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	this:SetClientString( "" );
	WarnFrame = nil;
end

function CostMoneyConfirmFrameConfirmBtn_OnClick()
	local frame = getglobal( "CostMoneyConfirmFrame" );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	if frame:GetClientString() == "天赋面板" then
		local nType = frame:GetClientUserData( 0 );
		local attrId = frame:GetClientUserData( 1 );
		local flag = frame:GetClientUserData( 2 );
		local lv = GeniusMgr:getAttrLv(nType,attrId);
		GeniusMgr:LevelUp( GeniusMgr:getGeniusType( nType ), attrId, lv, flag );
	elseif frame:GetClientString() == "世界传送" then
		local npcId = frame:GetClientUserData( 0 );
		GameMapSendingManager:mapSendingSelect( npcId )
		local MapSendInfoFrame = getglobal("WorldMapSendInfoFrame")
		MapSendInfoFrame:Hide()
		local MapSendFrame = getglobal("WorldMapSendFrame")
		MapSendFrame:Hide()
	elseif frame:GetClientString() == "购买物品" then
		local nCurIndex = frame:GetClientUserData( 0 );
		NpcShop:buyItem( nCurIndex, 1 );
	elseif frame:GetClientString() == "批量购买" then
		local nCurIndex = frame:GetClientUserData( 0 );
		local nNumber = frame:GetClientUserData( 1 );
		NpcShop:buyItem( nCurIndex, nNumber );
	elseif frame:GetClientString() == "回购物品" then
		local nIndex = frame:GetClientUserData( 0 );
		BuySaveItem( nIndex );
	elseif frame:GetClientString() == "技能学习" then
		local nNpcId = frame:GetClientUserData( 0 );
		local nSkillId = frame:GetClientUserData( 1 );
		local nSkillLv = frame:GetClientUserData( 2 );
		GameSkillLearn:skillLearn( nNpcId, nSkillId, nSkillLv );
	elseif frame:GetClientString() == "技能升级" then
		local nNpcId = frame:GetClientUserData( 0 );
		local nSkillId = frame:GetClientUserData( 1 );
		local nSkillLv = frame:GetClientUserData( 2 );
		GameSkillLearn:skillLvUp( nNpcId, nSkillId, nSkillLv );
	elseif frame:GetClientString() == "宠物强化" then
		local container = mainplayer:getContainer();
		local petPackage = mainplayer:getPet();
		local btn = getglobal( "Pet_DevelopmentBuildUpFrame_QiangHuaBtn" );
		local nList = btn:GetClientUserData( 0 );
		local nGrid = btn:GetClientUserData( 1 );
		local item = container:getItem( nList,nGrid );
		local heightId = frame:GetClientUserData( 0 );
		local lowId = frame:GetClientUserData( 1 );
		local curLv = frame:GetClientUserData( 2 );
		local petStrengthenDef = getPetStrengthenDef( curLv );
		if petPackage:petStrengthen( heightId, lowId, item, nil, petStrengthenDef.UseItemNum, curLv ) then
			SetStrengthenState();
		end
	elseif frame:GetClientString() == "宠物幻化" then
		local petMgr = mainplayer:getPet();
		local id = PetGrowResetFrameReset:GetClientString();
		petMgr:ResetPetGrow( bytesint(id) );
		SetPetGrowResetState( id );
	elseif frame:GetClientString() == "宠物技能重置" then
		local petMgr = mainplayer:getPet();
		local id = PetSkillResetFrameReset:GetClientString();
		petMgr:ResetPetSkill( bytesint(id) );
		SetPetSkillResetState( id );
	elseif frame:GetClientString() == "宠物技能升级" then
		local petMgr = mainplayer:getPet();
		local heightId = frame:GetClientUserData( 0 );
		local lowId = frame:GetClientUserData( 1 );
		local idx = frame:GetClientUserData( 2 );
		petMgr:LevelUpPetSkill( heightId, lowId, idx );
	elseif frame:GetClientString() == "晶石吸收" then
		local sparSorbData = GameArmProce:getSparSorbData();
		local container = mainplayer:getContainer();
		for i=1,frame:GetClientUserData( 0 ) do
			local btn = getglobal( "SparSorbFrameStuffItemBtn" .. i )
			local nList = btn:GetClientUserData( 0 )
			local nGrid = btn:GetClientUserData( 1 )
			local item = container:getItem( nList, nGrid )
			if item:getItemId() > 0 then
				sparSorbData.StuffData[sparSorbData.StuffNum].ListType = item:getListType()
				sparSorbData.StuffData[sparSorbData.StuffNum].GridIdx = item:getGridIdx()
				sparSorbData.StuffData[sparSorbData.StuffNum].ItemNum = item:getNum()
				sparSorbData.StuffData[sparSorbData.StuffNum].ItemID = item:getItemId()
				sparSorbData.StuffNum = sparSorbData.StuffNum + 1
			end
		end
		local srcList = SparSorbFrameItemBtn:GetClientUserData( 0 );
		local srcGrid = SparSorbFrameItemBtn:GetClientUserData( 1 );
		sparSorbData.Src.ArmListType = srcList;
		sparSorbData.Src.ArmGridIdx = srcGrid;
		GameArmProce:requestSparSorb();
	elseif frame:GetClientString() == "装备分解" then
		local container = mainplayer:getContainer();
		local nList = frame:GetClientUserData( 0 );
		local nGrid = frame:GetClientUserData( 1 );
		local nState = frame:GetClientUserData( 2 );
		local Item = container:getItem( nList, nGrid );
		GameArmProce:requestContainerResolvePre( Item:getItemWId(), nList, nGrid, 0 );
	elseif frame:GetClientString() == "科技制造" then
		local nSkillType = frame:GetClientUserData( 0 );
		local nRecipeID = frame:GetClientUserData( 1 );
		SecondarySkillMgr:requestUseSkillPre( nSkillType, nRecipeID );
	elseif frame:GetClientString() == "科技学习" then
		local nSkillType = frame:GetClientUserData( 0 );
		local nSkillID = frame:GetClientUserData( 1 );
		SecondarySkillMgr:requestLearnRecipt( nSkillType,nSkillID );
	elseif frame:GetClientString() == "装备重铸" then
		local stuffItem = ArmRecastFrameNeedItemBtn:GetClientUserData( 2 )
		local armListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
		local armGridIndex = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
		local nBind = ArmRecastFrameNeedItemBtn:GetClientUserData( 3 )
		local container = mainplayer:getContainer()
		local item = container:getItem( armListType, armGridIndex )
		if not item:isItemBind() and nBind == 1 then
			MessageBox("绑定提示","继续重铸则当前装备会被绑定，是否继续重铸？");
			MessageBoxFrame:SetClientString("绑定重铸");
			MessageBoxFrame:SetClientUserData( 0, stuffItem )
			MessageBoxFrame:SetClientUserData( 1, armListType )
			MessageBoxFrame:SetClientUserData( 2, armGridIndex )
			MessageBoxFrame:SetClientUserData( 3, nBind )
		else
			local must = 0
			if CheckItemResult( getItemDef(stuffItem), RESULT_ARM_RECAST_MUST ) then
				must = 1
			end
			GameArmProce:normalRecastArm( stuffItem, armListType, armGridIndex, nBind, must );
		end		
	elseif frame:GetClientString() == "装备强化" then
		local nGemBind = frame:GetClientUserData( 0 );
		local nItemBind = frame:GetClientUserData( 1 );
		if nGemBind == 1 and nItemBind == 0 then
			MessageBox( "装备强化绑定提示", "继续强化则当前装备会被绑定，是否继续强化？");
			MessageBoxFrame:SetClientString("装备强化绑定提示");
		else
			RequestArmStarAdd();
		end
	elseif frame:GetClientString() == "装备洗炼" then
		local stuffBtn = getglobal( "ArmWashFrameStuffItemBtn" );
		local armBtn = getglobal( "ArmWashFrameArmItemBtn" );
		if ArmAdvWashFrame:IsShown() then
			stuffBtn = getglobal( "ArmAdvWashFrameStuffItemBtn" );
			armBtn = getglobal( "ArmAdvWashFrameArmItemBtn" );
		end
		local stuffItem = stuffBtn:GetClientUserData( 2 )
		local nBind = stuffBtn:GetClientUserData( 3 );
		local armListType = armBtn:GetClientUserData( 0 );
		local armGridIndex = armBtn:GetClientUserData( 1 );
		local container = mainplayer:getContainer();
		local arm = container:getItem( armListType,armGridIndex )

		if ArmWashFrame:IsShown() and getWashItemLockNum( arm ) > 0 then
			MessageBox("锁定提示","该装备存在锁定的属性，普通洗炼锁定属性将会改变，确定需要洗炼？");
			MessageBoxFrame:SetClientString("锁定洗炼");
			return;
		end

		if not arm:isItemBind() and nBind == 1 then
			MessageBox("绑定提示","继续洗炼则当前装备会被绑定，是否继续洗炼？");
			MessageBoxFrame:SetClientString("绑定洗炼");
			MessageBoxFrame:SetClientUserData( 0, stuffItem )
			MessageBoxFrame:SetClientUserData( 1, armListType )
			MessageBoxFrame:SetClientUserData( 2, armGridIndex )
			MessageBoxFrame:SetClientUserData( 3, nBind )
		else
			local must = 0 
			if CheckItemResult( getItemDef(stuffItem), RESULT_ARM_WASH_STUFF_MUST ) then
				must = 1
			end
			GameArmProce:requestArmWash( stuffItem, armListType, armGridIndex, nBind, 0, 0, must )
		end
	elseif frame:GetClientString() == "装备绑定" then
		local nList = frame:GetClientUserData( 0 );
		local nGrid = frame:GetClientUserData( 1 );
		GameArmProce:bindArm( nList,nGrid );
	elseif frame:GetClientString() == "原地复活" then
		local nType = frame:GetClientUserData( 0 );
		mainplayer:revivePlayer( nType,0, 0 );
	elseif frame:GetClientString() == "物品修理" then
		local nList = frame:GetClientUserData( 0 );
		local nGrid = frame:GetClientUserData( 1 );
		local nItemID = frame:GetClientUserData( 2 );
		if nItemID > 0 then
			NpcShop:repairOneItem( nList, nGrid, nItemID );
		else
			NpcShop:repairAllItem();
		end
		local animationTexture	= getglobal( "SRF_OKBtnUVAnimationTex" );
		if animationTexture:IsShown() then
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CONFIRM_REPAIR_BTN );
		end		
		animationTexture:Hide();
		ShopRepairFrame:Hide();
	elseif frame:GetClientString() == "开启符文" then
		local id = frame:GetClientUserData( 0 );
		local nNeedBull = frame:GetClientUserData( 1 );
		RuneUnLock( id, nNeedBull );
	elseif frame:GetClientString() == "创建公会" then
		local edit = getglobal( "Clan_CreateInputFrame_Input_SetNameEditBox" );
		GameClanManager:clan_Opt_Create( edit:GetText() );
		Clan_CreateInputFrame:Hide();
	end

	if CostMoneyConfirmFrameHideBtn:GetCheckState() then
		bShowCostWarn = false;
	end
	CostMoneyConfirmFrame:Hide();
end

function CostMoneyConfirmFrameCancelBtn_OnClick()
	local frame = getglobal( "CostMoneyConfirmFrame" );
	if frame:GetClientString() == "装备分解" then
		if frame:GetClientUserData( 2 ) > 0 then
			CancelFenjieByComfirm();
		end
	elseif frame:GetClientString() == "科技制造" then
		SecondarySkillFrameCancelMakeBtn_OnClick();
	end
	CostMoneyConfirmFrame:Hide();
end

function CostMoneyConfirmFrame_OnUpdate()
	if WarnFrame ~= nil then
		local frame = getglobal( WarnFrame );
		if not frame:IsShown() then
			CostMoneyConfirmFrame:Hide();
		end
	end
end


-------------------------------------------重要物品二次操作确认框-----------------------------------------
local parentFrame = nil;
local MAX_STAR_MUL_LIST = 10;
local t_ResultDesc = { 
			[1] = "该装备有宝石镶嵌", 
			[2] = "该装备强化了四星及以上", 
			[3] = "该装备三星完美强化及以上", 
			[4] = "该装备有蓝色附加属性",
			[5] = "该宝石为2级以上宝石",
			[6] = "该装备为重要物品",
			[7] = "该装备进行过融合",
			[8]	= "该装备有完美强化星",
			}

function SetItemWarnFrame( frame )
	parentFrame = frame;
end

function ImportantItemConfirmFrame_OnLoad()
	ImportantItemConfirmFrameMessageFont:setCenterLine( true );
end

function CheckeItemImportantConfirmForSpecial( item, frame )
	if frame == "ArmAttrMoveFrame" or frame == "ArmSkillAttrMoveFrame" then
		if item == nil then
			return 0;
		end
		if item:getItemType() == ITEM_SLOTTYPE_AMM then
			for i = 1, item:getHoleNum() do
				if item:getHoleGemId(i-1) > 0 then
					return 1;
				end
			end
		
			--有完美星
			for i=1, item:getStart() do
				local armStarDef = getArmStarDef( item:getStart() );
				local starValIdx = item:getArmAddStarValIdx( i - 1 );
				if starValIdx >= 0 and starValIdx <= MAX_STAR_MUL_LIST then
					local mul = armStarDef.StarInfo[starValIdx].Mul;
					local minMul,maxMul = getArmDefMinAndMaxMul( armStarDef );
					if mul >= maxMul then
						return 8;
					end
				end
			end
		end
	end
	return 0;
end

function CheckItemNeedImportantConfirm( item )
	if item == nil then
		return 0;
	end
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		for i = 1, item:getHoleNum() do
			if item:getHoleGemId(i-1) > 0 then
				return 1;
			end
		end
		
		if item:getStart() > 3 then
			return 2;
		end
		
		for i=3, item:getStart() do
			local starValIdx = item:getArmAddStarValIdx( i-1 );
			if starValIdx >= MAX_STAR_MUL_LIST - 1 then
				return 3;
			end
		end

		if item:getArmWashAddAttrNum() > 0 then
			return 4;
		end

		if item:getItemDef().WearPos1 == EQUIP_POS_BADGE then
			return 6;
		end

		if item:getStepLv() > 0 or item:getStepUse() > 0 then
			return 7;
		end
	else
		local itemDef = item:getItemDef();
		if itemDef == nil then 
			return 0;
		end

		local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
		if gemNewDef ~= nil then
			if gemNewDef.Level > 1 then
				return 5;
			end
		end

		if math.floor( itemDef.ItemID / 100 ) == 50526 then
			return 6;
		end
	end
	return 0;
end

function ShowImportantItemConfirmFrame( optType, item, idx, needOk, imp )
	local itemDef = item:getItemDef();
	if itemDef == nil then return end;
	if ImportantItemConfirmFrame:IsShown() then
		ImportantItemConfirmFrame:Hide();
	end
	local t_Color = GetItemNameColor(itemDef);
	local szText = "";
	if optType == "转星" then
		szText = "您确定覆盖此装备的星级吗？\n#cf57300"
	elseif optType == "技能属性转移" then
		szText = "您确定转移此装备的技能属性吗？\n".. RGBColorToRichColor(t_Color) .." 【 ".. item:getDisplayName() .. "】\n#cf57300"
	else
		szText = "您确定" .. optType .. "该道具吗？\n" .. RGBColorToRichColor(t_Color) .." 【 ".. item:getDisplayName() .. "】\n#cf57300";
	end
	
	if idx > 0 then
		szText = szText .. t_ResultDesc[idx] .. "#n\n";
	elseif imp then
		szText = szText .. "该装备品质为粉色或以上#n\n";
	end
	if needOk and idx ~= 5 then
		szText = szText .. "继续操作请在下方输入“OK”";
		ImportantItemConfirmFrameEditBkg:Show();
		ImportantItemConfirmFrameEdit:Show();
		ImportantItemConfirmFrameEdit:SetText("");
	else
		ImportantItemConfirmFrameEditBkg:Hide();
		ImportantItemConfirmFrameEdit:Hide();
	end

	local rich = getglobal( "ImportantItemConfirmFrameMessageFont" );
	rich:SetText( szText, 255, 255, 190 );
	local height = rich:GetTextLines() > 3 and 80 or 65;
	height = height + ( rich:GetTextLines() < 3 and 62 or rich:GetTextLines() * 25 );
	ImportantItemConfirmFrame:SetHeight( height );
	ImportantItemConfirmFrame:Show();
end

function ImportantItemConfirmFrame_OnShow()
	this:AddLevelRecursive(1);
	util.ShowInCenter(this);
end

function ImportantItemConfirmFrame_OnHide()
	ClearItemLock( "ImportantItemConfirmFrame" );
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	this:SetClientUserData( 3, 0 );
	this:SetClientString( "" );
	parentFrame = nil;
	ClearFocus();
end

function ImportantItemConfirmFrameConfirmBtn_OnClick()
	local edit = getglobal( "ImportantItemConfirmFrameEdit" );
	if edit:IsShown() and edit:GetText() ~= "OK" then
		return ShowMidTips("输入的确认码错误");
	end

	local font = getglobal( "ImportantItemConfirmFrameTitle" );
	if font:GetText() == "道具丢弃确认" then
		TropItem();
		playUISound( "sounds\\UI\\diuqi.wav"  );
	elseif font:GetText() == "道具出售确认" then
		local nType	= ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	= ImportantItemConfirmFrame:GetClientUserData(1);
		local num	= ImportantItemConfirmFrame:GetClientUserData(2);
		NpcShop:sellItem( nType, nIndex, num );
		playUISound( "sounds\\UI\\shoumai.wav" );
	elseif font:GetText() == "宝石分解确认" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local flag	 = ImportantItemConfirmFrame:GetClientUserData(2);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		GameArmProce:requestContainerResolvePre( Item:getItemWId(), nType, nIndex, flag );
	elseif font:GetText() == "装备分解确认" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local flag	 = ImportantItemConfirmFrame:GetClientUserData(2);
		local money	 = ImportantItemConfirmFrame:GetClientUserData(3);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		
		if checkNeedWarnCost( money ) then
			CostMoneyConfirmFrame:SetClientString("装备分解");
			CostMoneyConfirmFrame:SetClientUserData( 0, nType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nIndex );
			CostMoneyConfirmFrame:SetClientUserData( 2, 0 );
			SetWarnFrame( "PlayerContainerResolveFrame" );
			ImportantItemConfirmFrame:Hide();
			CostMoneyConfirmFrame:Show();
			return;
		end
		GameArmProce:requestContainerResolvePre( Item:getItemWId(), nType, nIndex, flag );
	elseif font:GetText() == "装备转星确认" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		if not Item:isItemBind() then
			ImportantItemConfirmFrame:Hide();
			MessageBox( "绑定提示", "转星会使目标装备绑定，确定继续转星？" );
			MessageBoxFrame:SetClientString("品质转移绑定提示");
			return;
		end
		CallArmProceStarMove();
	elseif font:GetText() == "装备消耗确认" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		SetArmStepUpItem( ImportantItemConfirmFrame:GetClientString(), Item );
	elseif font:GetText() == "装备废弃确认" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		SetArmBaseResetItem( ImportantItemConfirmFrame:GetClientString(), Item );
	elseif font:GetText() == "装备属性转移确认" then
		ArmAttrMoveStart(parentFrame);
	elseif font:GetText() == "装备技能转移确认" then		
		ArmSkillAttrMoveStart();
	end
	ImportantItemConfirmFrame:Hide();
end

function ImportantItemConfirmFrameCancelBtn_OnClick()
	local font = getglobal( "ImportantItemConfirmFrameTitle" );
	if string.find( font:GetText(), "分解确认", 1, true ) then
		CancelFenjieByComfirm();
	end
	ImportantItemConfirmFrame:Hide();
end

function ImportantItemConfirmFrame_OnUpdate()
	if parentFrame ~= nil then
		local frame = getglobal( parentFrame );
		if not frame:IsShown() then
			this:Hide();
		end
	end
end


---------------------------------------------------金币不足确认框-------------------------------------------------
function SwitchGoldFrame()
	ExchangePointFrame:Hide();
	if MarketManager:getCurDianquan() > 0 then
		MessageBoxForVip( "提示", "已提取#cffff00金币#n不足，请#cffff00提取金币#n后重新操作" )
		MessageBoxFrameForVipAcceptButton:SetText("提取金币");
	else
		MessageBoxForVip( "提示", "您的#cffff00金币#n不足，是否#cffff00快速充值#n" )
		MessageBoxFrameForVipAcceptButton:SetText("快速充值");
	end
end

function ShowBuyMoneyFrame()
	if MarketBuyTicketFrame:IsShown() then
		MarketBuyTicketFrame:Hide();
	end
	MessageBoxForVip( "提示", "您的#cffff00银币#n数量不足，是否需要通过#cffff00金币交易#n购买" )
	MessageBoxFrameForVipAcceptButton:SetText("购买银币");
end