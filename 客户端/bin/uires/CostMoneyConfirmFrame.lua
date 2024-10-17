--------------------------------------- �������Ķ�����ʾ�� -------------------------------------------
local bShowCostWarn = true;
local WarnFrame = nil;

function CostMoneyConfirmFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	CostMoneyConfirmFrameMessageFont:SetText("�������㣬����������ֱ���������ҡ�\n\t\t\t\t\t\t�Ƿ����������");
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

	if frame:GetClientString() == "�츳���" then
		local nType = frame:GetClientUserData( 0 );
		local attrId = frame:GetClientUserData( 1 );
		local flag = frame:GetClientUserData( 2 );
		local lv = GeniusMgr:getAttrLv(nType,attrId);
		GeniusMgr:LevelUp( GeniusMgr:getGeniusType( nType ), attrId, lv, flag );
	elseif frame:GetClientString() == "���紫��" then
		local npcId = frame:GetClientUserData( 0 );
		GameMapSendingManager:mapSendingSelect( npcId )
		local MapSendInfoFrame = getglobal("WorldMapSendInfoFrame")
		MapSendInfoFrame:Hide()
		local MapSendFrame = getglobal("WorldMapSendFrame")
		MapSendFrame:Hide()
	elseif frame:GetClientString() == "������Ʒ" then
		local nCurIndex = frame:GetClientUserData( 0 );
		NpcShop:buyItem( nCurIndex, 1 );
	elseif frame:GetClientString() == "��������" then
		local nCurIndex = frame:GetClientUserData( 0 );
		local nNumber = frame:GetClientUserData( 1 );
		NpcShop:buyItem( nCurIndex, nNumber );
	elseif frame:GetClientString() == "�ع���Ʒ" then
		local nIndex = frame:GetClientUserData( 0 );
		BuySaveItem( nIndex );
	elseif frame:GetClientString() == "����ѧϰ" then
		local nNpcId = frame:GetClientUserData( 0 );
		local nSkillId = frame:GetClientUserData( 1 );
		local nSkillLv = frame:GetClientUserData( 2 );
		GameSkillLearn:skillLearn( nNpcId, nSkillId, nSkillLv );
	elseif frame:GetClientString() == "��������" then
		local nNpcId = frame:GetClientUserData( 0 );
		local nSkillId = frame:GetClientUserData( 1 );
		local nSkillLv = frame:GetClientUserData( 2 );
		GameSkillLearn:skillLvUp( nNpcId, nSkillId, nSkillLv );
	elseif frame:GetClientString() == "����ǿ��" then
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
	elseif frame:GetClientString() == "����û�" then
		local petMgr = mainplayer:getPet();
		local id = PetGrowResetFrameReset:GetClientString();
		petMgr:ResetPetGrow( bytesint(id) );
		SetPetGrowResetState( id );
	elseif frame:GetClientString() == "���＼������" then
		local petMgr = mainplayer:getPet();
		local id = PetSkillResetFrameReset:GetClientString();
		petMgr:ResetPetSkill( bytesint(id) );
		SetPetSkillResetState( id );
	elseif frame:GetClientString() == "���＼������" then
		local petMgr = mainplayer:getPet();
		local heightId = frame:GetClientUserData( 0 );
		local lowId = frame:GetClientUserData( 1 );
		local idx = frame:GetClientUserData( 2 );
		petMgr:LevelUpPetSkill( heightId, lowId, idx );
	elseif frame:GetClientString() == "��ʯ����" then
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
	elseif frame:GetClientString() == "װ���ֽ�" then
		local container = mainplayer:getContainer();
		local nList = frame:GetClientUserData( 0 );
		local nGrid = frame:GetClientUserData( 1 );
		local nState = frame:GetClientUserData( 2 );
		local Item = container:getItem( nList, nGrid );
		GameArmProce:requestContainerResolvePre( Item:getItemWId(), nList, nGrid, 0 );
	elseif frame:GetClientString() == "�Ƽ�����" then
		local nSkillType = frame:GetClientUserData( 0 );
		local nRecipeID = frame:GetClientUserData( 1 );
		SecondarySkillMgr:requestUseSkillPre( nSkillType, nRecipeID );
	elseif frame:GetClientString() == "�Ƽ�ѧϰ" then
		local nSkillType = frame:GetClientUserData( 0 );
		local nSkillID = frame:GetClientUserData( 1 );
		SecondarySkillMgr:requestLearnRecipt( nSkillType,nSkillID );
	elseif frame:GetClientString() == "װ������" then
		local stuffItem = ArmRecastFrameNeedItemBtn:GetClientUserData( 2 )
		local armListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
		local armGridIndex = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
		local nBind = ArmRecastFrameNeedItemBtn:GetClientUserData( 3 )
		local container = mainplayer:getContainer()
		local item = container:getItem( armListType, armGridIndex )
		if not item:isItemBind() and nBind == 1 then
			MessageBox("����ʾ","����������ǰװ���ᱻ�󶨣��Ƿ����������");
			MessageBoxFrame:SetClientString("������");
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
	elseif frame:GetClientString() == "װ��ǿ��" then
		local nGemBind = frame:GetClientUserData( 0 );
		local nItemBind = frame:GetClientUserData( 1 );
		if nGemBind == 1 and nItemBind == 0 then
			MessageBox( "װ��ǿ������ʾ", "����ǿ����ǰװ���ᱻ�󶨣��Ƿ����ǿ����");
			MessageBoxFrame:SetClientString("װ��ǿ������ʾ");
		else
			RequestArmStarAdd();
		end
	elseif frame:GetClientString() == "װ��ϴ��" then
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
			MessageBox("������ʾ","��װ���������������ԣ���ͨϴ���������Խ���ı䣬ȷ����Ҫϴ����");
			MessageBoxFrame:SetClientString("����ϴ��");
			return;
		end

		if not arm:isItemBind() and nBind == 1 then
			MessageBox("����ʾ","����ϴ����ǰװ���ᱻ�󶨣��Ƿ����ϴ����");
			MessageBoxFrame:SetClientString("��ϴ��");
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
	elseif frame:GetClientString() == "װ����" then
		local nList = frame:GetClientUserData( 0 );
		local nGrid = frame:GetClientUserData( 1 );
		GameArmProce:bindArm( nList,nGrid );
	elseif frame:GetClientString() == "ԭ�ظ���" then
		local nType = frame:GetClientUserData( 0 );
		mainplayer:revivePlayer( nType,0, 0 );
	elseif frame:GetClientString() == "��Ʒ����" then
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
	elseif frame:GetClientString() == "��������" then
		local id = frame:GetClientUserData( 0 );
		local nNeedBull = frame:GetClientUserData( 1 );
		RuneUnLock( id, nNeedBull );
	elseif frame:GetClientString() == "��������" then
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
	if frame:GetClientString() == "װ���ֽ�" then
		if frame:GetClientUserData( 2 ) > 0 then
			CancelFenjieByComfirm();
		end
	elseif frame:GetClientString() == "�Ƽ�����" then
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


-------------------------------------------��Ҫ��Ʒ���β���ȷ�Ͽ�-----------------------------------------
local parentFrame = nil;
local MAX_STAR_MUL_LIST = 10;
local t_ResultDesc = { 
			[1] = "��װ���б�ʯ��Ƕ", 
			[2] = "��װ��ǿ�������Ǽ�����", 
			[3] = "��װ����������ǿ��������", 
			[4] = "��װ������ɫ��������",
			[5] = "�ñ�ʯΪ2�����ϱ�ʯ",
			[6] = "��װ��Ϊ��Ҫ��Ʒ",
			[7] = "��װ�����й��ں�",
			[8]	= "��װ��������ǿ����",
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
		
			--��������
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
	if optType == "ת��" then
		szText = "��ȷ�����Ǵ�װ�����Ǽ���\n#cf57300"
	elseif optType == "��������ת��" then
		szText = "��ȷ��ת�ƴ�װ���ļ���������\n".. RGBColorToRichColor(t_Color) .." �� ".. item:getDisplayName() .. "��\n#cf57300"
	else
		szText = "��ȷ��" .. optType .. "�õ�����\n" .. RGBColorToRichColor(t_Color) .." �� ".. item:getDisplayName() .. "��\n#cf57300";
	end
	
	if idx > 0 then
		szText = szText .. t_ResultDesc[idx] .. "#n\n";
	elseif imp then
		szText = szText .. "��װ��Ʒ��Ϊ��ɫ������#n\n";
	end
	if needOk and idx ~= 5 then
		szText = szText .. "�������������·����롰OK��";
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
		return ShowMidTips("�����ȷ�������");
	end

	local font = getglobal( "ImportantItemConfirmFrameTitle" );
	if font:GetText() == "���߶���ȷ��" then
		TropItem();
		playUISound( "sounds\\UI\\diuqi.wav"  );
	elseif font:GetText() == "���߳���ȷ��" then
		local nType	= ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	= ImportantItemConfirmFrame:GetClientUserData(1);
		local num	= ImportantItemConfirmFrame:GetClientUserData(2);
		NpcShop:sellItem( nType, nIndex, num );
		playUISound( "sounds\\UI\\shoumai.wav" );
	elseif font:GetText() == "��ʯ�ֽ�ȷ��" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local flag	 = ImportantItemConfirmFrame:GetClientUserData(2);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		GameArmProce:requestContainerResolvePre( Item:getItemWId(), nType, nIndex, flag );
	elseif font:GetText() == "װ���ֽ�ȷ��" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local flag	 = ImportantItemConfirmFrame:GetClientUserData(2);
		local money	 = ImportantItemConfirmFrame:GetClientUserData(3);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		
		if checkNeedWarnCost( money ) then
			CostMoneyConfirmFrame:SetClientString("װ���ֽ�");
			CostMoneyConfirmFrame:SetClientUserData( 0, nType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nIndex );
			CostMoneyConfirmFrame:SetClientUserData( 2, 0 );
			SetWarnFrame( "PlayerContainerResolveFrame" );
			ImportantItemConfirmFrame:Hide();
			CostMoneyConfirmFrame:Show();
			return;
		end
		GameArmProce:requestContainerResolvePre( Item:getItemWId(), nType, nIndex, flag );
	elseif font:GetText() == "װ��ת��ȷ��" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		if not Item:isItemBind() then
			ImportantItemConfirmFrame:Hide();
			MessageBox( "����ʾ", "ת�ǻ�ʹĿ��װ���󶨣�ȷ������ת�ǣ�" );
			MessageBoxFrame:SetClientString("Ʒ��ת�ư���ʾ");
			return;
		end
		CallArmProceStarMove();
	elseif font:GetText() == "װ������ȷ��" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		SetArmStepUpItem( ImportantItemConfirmFrame:GetClientString(), Item );
	elseif font:GetText() == "װ������ȷ��" then
		local nType	 = ImportantItemConfirmFrame:GetClientUserData(0);
		local nIndex	 = ImportantItemConfirmFrame:GetClientUserData(1);
		local MainPlayer = ActorMgr:getMainPlayer();
		local container	 = MainPlayer:getContainer();
		local Item 	 = container:getItem( nType, nIndex );
		SetArmBaseResetItem( ImportantItemConfirmFrame:GetClientString(), Item );
	elseif font:GetText() == "װ������ת��ȷ��" then
		ArmAttrMoveStart(parentFrame);
	elseif font:GetText() == "װ������ת��ȷ��" then		
		ArmSkillAttrMoveStart();
	end
	ImportantItemConfirmFrame:Hide();
end

function ImportantItemConfirmFrameCancelBtn_OnClick()
	local font = getglobal( "ImportantItemConfirmFrameTitle" );
	if string.find( font:GetText(), "�ֽ�ȷ��", 1, true ) then
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


---------------------------------------------------��Ҳ���ȷ�Ͽ�-------------------------------------------------
function SwitchGoldFrame()
	ExchangePointFrame:Hide();
	if MarketManager:getCurDianquan() > 0 then
		MessageBoxForVip( "��ʾ", "����ȡ#cffff00���#n���㣬��#cffff00��ȡ���#n�����²���" )
		MessageBoxFrameForVipAcceptButton:SetText("��ȡ���");
	else
		MessageBoxForVip( "��ʾ", "����#cffff00���#n���㣬�Ƿ�#cffff00���ٳ�ֵ#n" )
		MessageBoxFrameForVipAcceptButton:SetText("���ٳ�ֵ");
	end
end

function ShowBuyMoneyFrame()
	if MarketBuyTicketFrame:IsShown() then
		MarketBuyTicketFrame:Hide();
	end
	MessageBoxForVip( "��ʾ", "����#cffff00����#n�������㣬�Ƿ���Ҫͨ��#cffff00��ҽ���#n����" )
	MessageBoxFrameForVipAcceptButton:SetText("��������");
end