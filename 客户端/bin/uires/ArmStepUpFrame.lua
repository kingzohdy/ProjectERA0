local MAX_COST_ITEM = 10;
local STEP_UP_MIN_LV = 60;
local useItemNum = 0;
local useMoneyNum = 0;
local useArmNum = 0;
local stepArmNum = 0;
local inputArmNum = 0;
local isFull = false;
local t_ImpName = { "��ɫ", "��ɫ", "��ɫ", "��ɫ" };

local function ClearItemBtn( name )
	local btn = getglobal( name );
	local ItemNum   = getglobal( name.."CountFont");
	local UVAnimationTex = getglobal( name.."UVAnimationTex");
	local ItemTex = getglobal( name.."IconTexture");
	local BindTex = getglobal( name.."BindTexture");
	local LockTex = getglobal( name.."LockTexture");
	if btn:GetClientUserData(2) > 0 then
		UnLockItem( btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );
	end
	btn:SetClientUserData( 0, 0 );
	btn:SetClientUserData( 1, 0 );
	btn:SetClientUserData( 2, 0 );
	btn:SetClientUserData( 3, 0 );
	ItemNum:SetText( "" );
	UVAnimationTex:Hide();
	ItemTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	BindTex:Hide();
	LockTex:Hide();
	ArmStepUpFrameOKBtn:Disable();
end

local function ResetCostItemBtn( lock )
	for i=1, MAX_COST_ITEM do
		local btn = getglobal( "ArmStepUpFrameItemBtn" .. i );
		ClearItemBtn( btn:GetName() );
		local lockTex = getglobal( btn:GetName() .. "LockTexture" );
		if lock or isFull or ( useArmNum > 0 and i > useArmNum - stepArmNum ) then
			lockTex:Show();
			btn:Disable();
		else
			lockTex:Hide();
			btn:Enable();
		end
	end
	inputArmNum = 0;
end

local function updateStepArmProgress()
	local font = getglobal( "ArmStepUpFrameStepFont" );
	local tex = getglobal( "ArmStepUpFrameStepProgress" );
	font:SetText( isFull and "������߽ײ�" or stepArmNum .. "/" .. useArmNum );
	local scale = isFull and 1 or stepArmNum / useArmNum;
	tex:SetSize( 160 * scale, 12 );
end

local function SetStepUpInfo( lv, stepDef, use )
	if lv ~= nil and stepDef ~= nil then
		local itemResultDescDef = getItemResultDesc( stepDef.AttrID );
		local t_Desc = {};
		if lv < stepDef.MaxStep then
			local stepLv = lv > 0 and lv - 1 or lv;
			useItemNum = stepDef.StepAttr[lv].ItemNum;
			useMoneyNum = stepDef.StepAttr[lv].MoneyNum;
			useArmNum = stepDef.StepAttr[lv].ArmNum;
			isFull = false;
			local szText = "(����" .. (lv ~= 0 and lv + 1 or 1) .. "/" .. stepDef.MaxStep ..")";
			table.insert( t_Desc, szText );
			szText = lv ~= 0 and ( itemResultDescDef.ResultDesc .. "+" .. stepDef.StepAttr[stepLv].Val ) or "��";
			table.insert( t_Desc, szText );
			szText = itemResultDescDef.ResultDesc .. "+" .. stepDef.StepAttr[lv].Val;
			table.insert( t_Desc, szText );
		else
			useItemNum = 0;
			useMoneyNum = 0;
			useArmNum = 0;
			isFull = true;
			local szText = "(����" .. lv .."/" .. stepDef.MaxStep ..")";
			table.insert( t_Desc, szText );
			szText = itemResultDescDef.ResultDesc .. "+" .. stepDef.StepAttr[lv-1].Val;
			table.insert( t_Desc, szText );
			szText = "������߽ײ�";
			table.insert( t_Desc, szText );
		end
		stepArmNum = use;
		inputArmNum = 0;
		--ArmStepUpFrameNowStepFont:SetText( string.format( "(����%d/%d)", lv, stepDef.MaxStep ) );
		ArmStepUpFrameNextStepFont:SetText( t_Desc[1] );
		ArmStepUpFrameNowDescFont:SetText( t_Desc[2] );
		ArmStepUpFrameNextDescFont:SetText( t_Desc[3] );
		ArmStepUpFrameNow:Show();
		ArmStepUpFrameNext:Show();
		ArmStepUpFrameArrowTex:Show();
		updateStepArmProgress();
		ArmStepUpFrameStep:Show();
	else
		useItemNum = 0;
		useMoneyNum = 0;
		useArmNum = 0;
		stepArmNum = 0;
		inputArmNum = 0;
		isFull = false;
		ArmStepUpFrameNow:Hide();
		ArmStepUpFrameNext:Hide();
		ArmStepUpFrameArrowTex:Hide();
		ArmStepUpFrameStep:Hide();
	end
	ArmStepUpFramePriceFont:SetText(0);
	--ArmStepUpFrameNeedArmFont:SetText( string.format( "����װ����%d��", useArmNum - stepArmNum ) );
end

local function initStepUpFrame()
	SetStepUpInfo();
	--ArmStepUpFrameHaveFont:SetTextColor( 160, 160, 90 );
	ResetCostItemBtn( true );
	ClearItemBtn( "ArmStepUpFrameArmItemBtn" );
	ClearItemBtn( "ArmStepUpFrameStuffItemBtn" );
	ArmStepUpFrameArmItemBtnModelView:Hide();
	ArmStepUpFrameOKBtn:Disable();
	ArmStepUpFrameOKBtn:SetClientUserData( 0, 0 );
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "װ���ںϰ���ʾ" then
		MessageBoxFrame:Hide();
	end
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
	local curNum = useItemNum * inputArmNum;
	curNum = curNum > 999 and ( math.floor(curNum/1000) .. "k" ) or curNum;
	font:SetText( totalNum .. "/" .. curNum );
	totalNum = string.gsub( totalNum, "k", "" );
	local t_Color = { { 255, 255, 0 }, { 230, 0, 0 } };
	local idx = useItemNum * inputArmNum > tonumber(totalNum) and 2 or 1;
	idx = inputArmNum > 0 and idx or 2;
	font:SetTextColor( t_Color[idx][1], t_Color[idx][2], t_Color[idx][3] );
	local icon = getglobal( name .. "IconTexture");
	icon:SetGray( idx > 1 );
end

local function CheckCostEnough()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	--local font = getglobal( "ArmStepUpFrameHaveFont" );
	--font:SetText( mainplayer:getMoney() );

	inputArmNum = 0;
	local totalNum = useArmNum - stepArmNum > MAX_COST_ITEM and MAX_COST_ITEM or useArmNum - stepArmNum;
	for i=1, totalNum do
		local btn = getglobal( "ArmStepUpFrameItemBtn" .. i );
		if btn:GetClientUserData(2) > 0 then
			inputArmNum = inputArmNum + 1;
		end
	end

	local font = getglobal( "ArmStepUpFramePriceFont" );
	font:SetText( useMoneyNum * inputArmNum );
	if mainplayer:getMoney() < useMoneyNum * inputArmNum then
		font:SetTextColor( 230, 0, 0 );
		return false;
	else
		font:SetTextColor( 160, 160, 90 );
	end

	if ArmStepUpFrameArmItemBtn:GetClientUserData(2) == 0 then
		return false;
	end

	local container	= mainplayer:getContainer();
	local stuffId = ArmStepUpFrameStuffItemBtn:GetClientUserData(2);
	if stuffId ~= 0 then
		SetItemNum( "ArmStepUpFrameStuffItemBtn", stuffId );
		if inputArmNum == 0 or container:getItemCount(stuffId) < useItemNum * inputArmNum then
			return false;
		end
	end
	return true;
end

local function SetItemBtn( name, item, itemId )
	local btn = getglobal( name );
	local ItemNum   = getglobal( btn:GetName().."CountFont");
	local ItemTex = getglobal( btn:GetName().."IconTexture");
	local BindTex = getglobal( btn:GetName().."BindTexture");
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
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
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
		ArmStepUpFrameOKBtn:Enable();
	else
		ArmStepUpFrameOKBtn:Disable();
	end
end

local function AutoAddStuffItem( itemId )
	local btn = getglobal( "ArmStepUpFrameStuffItemBtn" );
	local lockTex = getglobal( btn:GetName() .. "LockTexture" );
	ClearItemBtn( btn:GetName() );
	if itemId > 0 then
		lockTex:Hide();
		btn:Enable();
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		local container	= mainplayer:getContainer();
		local t_Item = {};
		for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
			if item:getItemId() == itemId then
				table.insert( t_Item, { item, (item:isItemBind() and 1 or 0) } );
			end
		end

		if #t_Item > 0 then
			table.sort( t_Item, function( a,b )
			return a[2] > b[2];
			end);
			SetItemBtn( btn:GetName(), t_Item[1][1] );
		else
			SetItemBtn( btn:GetName(), nil, itemId );
		end
	else
		lockTex:Show();
		btn:Disable();
	end
end

local function SetArmStepInfo( itemDef, lv, use )
	local stepDef = GameArmProce:getArmStepUpDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important, itemDef.WearPos1 );
	if stepDef ~= nil then
		SetStepUpInfo( lv, stepDef, use );
		AutoAddStuffItem( useItemNum > 0 and stepDef.ItemID or 0 );
		ResetCostItemBtn();
		ArmStepUpFrameOKBtn:SetClientUserData( 0, stepDef.UseArmImp );
	end
end

local function CheckCanPutArmItem( item, itemDef )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("�����Ҫ�ںϵ�װ��");
		return false;
	end
	if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "ʱװ" then
		ShowMidTips( "ʱװ���߲��ܽ����ں�" );
		return false;
	elseif itemDef.WearPos1 == EQUIP_POS_BADGE then
		ShowMidTips( "����װ�������ں�" );
		return false;
	end
	if itemDef.WearPre.RoleLevelMin < STEP_UP_MIN_LV then
		ShowMidTips( "ֻ��" .. STEP_UP_MIN_LV .. "��������װ���ſ��Խ����ں�" );
		return false;
	elseif itemDef.Important < 3 then
		ShowMidTips( "�������ɫƷ������װ��" );
		return false;
	end
	return true;
end

local function CheckCanPutCostItem( item, itemDef )
	local armItemDef = getItemDef( ArmStepUpFrameArmItemBtn:GetClientUserData(2) );
	if armItemDef.ItemID ~= 0 then
		local minImp = ArmStepUpFrameOKBtn:GetClientUserData(0);
		if item:getItemType() ~= ITEM_SLOTTYPE_AMM or itemDef.WearPos1 ~= armItemDef.WearPos1 or
			itemDef.WearPre.RoleLevelMin ~= armItemDef.WearPre.RoleLevelMin or
			itemDef.Important < minImp then
			ShowMidTips( "��������ں�װ��ͬ�ȼ�ͬ��λ" .. ( t_ImpName[minImp] ~= nil and t_ImpName[minImp] or "�ض�" ) .. "Ʒ�����ϵ�װ��" )
			return false;
		end
		return true;
	else
		ShowMidTips("�����Ҫ�ںϵ�װ��");
		return false;
	end
end

function SetArmStepUpItem( name, item )
	SetItemBtn( name, item );
end

function OpenArmStepUpFrame( npcId )
	if not ArmStepUpFrame:IsShown() then
		ArmStepUpFrame:Show();
		util.testNpcDialogDistance( npcId, ArmStepUpFrame );
	end

	if PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	else
		PlayerContainerFrame:Show();
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	end
	ArmStepUpFrame:SetPoint( "topright","PlayerContainerFrame","topleft", 0, 0 );
end

function ArmStepUpResult( flag )
	if flag > 0 then
		local modelview = getglobal( "ArmStepUpFrameArmItemBtnModelView" );
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent");
		modelview:setPetScale( 0, 0.9, 0.9, 0.3 );
		modelview:setEntityPosition(0, 0, 100, 0);
		if not modelview:IsShown() then
			modelview:Show()
		end
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		local container	= mainplayer:getContainer();
		local btn = getglobal( "ArmStepUpFrameArmItemBtn" );
		local nType = btn:GetClientUserData(0);
		local nIdx = btn:GetClientUserData(1);
		local item = container:getItem( nType, nIdx );
		if item:getStepUse() == 0 then
			ShowMidTips( "��" .. item:getDisplayName() .. "���ɹ�����" .. item:getStepLv() .. "�ף��ں���������" );
		else
			local itemDef = item:getItemDef();
			local stepDef = GameArmProce:getArmStepUpDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important, itemDef.WearPos1 );
			local stepLv = item:getStepLv() > 0 and item:getStepLv() - 1 or 0;
			ShowMidTips( "��" .. item:getDisplayName() .. "���ںϽ��� " .. item:getStepUse() .. "/" .. useArmNum );
		end
		SetArmStepInfo( item:getItemDef(), item:getStepLv(), item:getStepUse() );
		SetItemBind( btn:GetName(), item:isItemBind() );

		local stuffId = ArmStepUpFrameStuffItemBtn:GetClientUserData(2);
		if stuffId ~= 0 then
			SetItemNum( "ArmStepUpFrameStuffItemBtn", stuffId );
		end
		ArmStepUpFramePriceFont:SetText(0);
	end
end

function ArmStepUpFrameOKBtn_OnClick()
	if GameExchange:isInExchange() then
		return ShowMidTips( "�����У����ɽ���װ���ں�" );
	end

	if useItemNum == 0 and useMoneyNum == 0 and useArmNum == 0 then
		return ShowMidTips( "��װ��������߽ײ㣬�������ں�" );
	end

	local nBind = ArmStepUpFrameArmItemBtn:GetClientUserData(3);
	if nBind == 0 then
		if ArmStepUpFrameStuffItemBtn:GetClientUserData(3) > 0 then
			nBind = 1;
		else
			local totalNum = useArmNum - stepArmNum > MAX_COST_ITEM and MAX_COST_ITEM or useArmNum - stepArmNum;
			for i=1, totalNum do
				local btn = getglobal( "ArmStepUpFrameItemBtn" .. i );
				if btn:GetClientUserData(3) > 0 then
					nBind = 1;
					break;
				end
			end
		end

		if nBind > 0 then
			MessageBox( "����ʾ", "�����ں���ǰ�ں�װ���ᱻ�󶨣��Ƿ������" );
			MessageBoxFrame:SetClientString("װ���ںϰ���ʾ");
			return;
		end
	end

	ArmStepUpOpt();
end

function ArmStepUpOpt()
	local data = GameArmProce:getArmStepUpData();
	data.ArmNum = inputArmNum;
	data.Src.ArmListType = ArmStepUpFrameArmItemBtn:GetClientUserData(0);
	data.Src.ArmGridIdx = ArmStepUpFrameArmItemBtn:GetClientUserData(1);
	local count = 0;
	local totalNum = useArmNum - stepArmNum > MAX_COST_ITEM and MAX_COST_ITEM or useArmNum - stepArmNum;
	for i=1, totalNum do
		local btn = getglobal( "ArmStepUpFrameItemBtn" .. i );
		if btn:GetClientUserData(2) > 0 then
			data.ArmStuffs[count].ArmListType = btn:GetClientUserData(0);
			data.ArmStuffs[count].ArmGridIdx = btn:GetClientUserData(1);
			count = count + 1;
		end
	end
	GameArmProce:requestArmStepUp( data );
end

function ArmStepUpFrame_OnShow()
	CheckCostEnough();
	SetStepUpInfo();
	ResetCostItemBtn( true );
end

function ArmStepUpFrame_OnHide()
	initStepUpFrame();
end

function ArmStepUpFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ItemFirstCallBack( "ArmStepUpFrame", Save2ArmStepUpItem );

	for i=1, MAX_COST_ITEM do
		local btn = getglobal( "ArmStepUpFrameItemBtn" .. i );
		btn:SetPoint( "topleft", "ArmStepUpFrame", "topleft", 28 + (i-1)%5*46, 121 + math.ceil(i/5)*46 );
		local tex = getglobal( btn:GetName() .. "BackTexture" );
		tex:Show();
	end

	local szText = [[�������60��������׿ԽƷ�ʵ�װ��һ��
��װ���ںϿ�Ϊװ�������ں�����]];
	ArmStepUpFrameRich:SetText( szText, 160, 155, 130 );
end

function ArmStepUpFrame_OnEvent()
	if not this:IsShown() then return end;
	if arg1 == "GE_CONTAINER_UPDATE" then
		local btn = getglobal( "ArmStepUpFrameStuffItemBtn" );
		local itemId = btn:GetClientUserData(2);
		if itemId > 0 then
			ClearItemBtn( btn:GetName() );
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end;
			local container	= mainplayer:getContainer();
			local t_Item = {};
			for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
				local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
				if item:getItemId() == itemId then
					table.insert( t_Item, { item, (item:isItemBind() and 1 or 0) } );
				end
			end

			if #t_Item > 0 then
				table.sort( t_Item, function( a,b )
				return a[2] > b[2];
				end);
				SetItemBtn( btn:GetName(), t_Item[1][1] );
			else
				SetItemBtn( btn:GetName(), nil, itemId );
			end
		end
	elseif arg1 == "GE_CHANGE_MONEY" then
		CheckCostEnough();
	end
end

function Save2ArmStepUpItem( item, nSrcList, nSrcGridIndex )
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "װ���ںϰ���ʾ" then
		return;
	end
	if ImportantItemConfirmFrame:IsShown() then
		ImportantItemConfirmFrame:Hide();
	end
	local itemDef = item:getItemDef();
	if itemDef == nil then return end;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if ArmStepUpFrameArmItemBtn:GetClientUserData(2) == 0 then
		if not CheckCanPutArmItem( item, itemDef ) then
			return;
		end
		local stepDef = GameArmProce:getArmStepUpDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important, itemDef.WearPos1 );
		if stepDef ~= nil and item:getStepLv() == stepDef.MaxStep then
			return ShowMidTips( "��ǰװ���ײ��������������ں�" );
		end
		SetItemBtn( "ArmStepUpFrameArmItemBtn", item );
		SetArmStepInfo( itemDef, item:getStepLv(), item:getStepUse() );
	else
		if not CheckCanPutCostItem( item, itemDef ) then
			return;
		elseif isFull or useArmNum == 0 then
			return ShowMidTips( "��ǰû�пɷ����������" );
		end

		local btn = getglobal( "ArmStepUpFrameItemBtn1" );

		local totalNum = useArmNum - stepArmNum > MAX_COST_ITEM and MAX_COST_ITEM or useArmNum - stepArmNum;
		for i=1, totalNum do
			btn = getglobal( "ArmStepUpFrameItemBtn" .. i );
			if btn:GetClientUserData(2) == 0 then
				break;
			end
		end

		local idx = CheckItemNeedImportantConfirm( item );
		local bImp = itemDef.Important > 3;
		if idx > 0 or bImp then
			ImportantItemConfirmFrameTitle:SetText("װ������ȷ��");
			ShowImportantItemConfirmFrame( "����", item, idx, false, bImp );
			SetItemWarnFrame("ArmStepUpFrame");
			ImportantItemConfirmFrame:SetClientUserData( 0, nSrcList );
			ImportantItemConfirmFrame:SetClientUserData( 1, nSrcGridIndex );
			ImportantItemConfirmFrame:SetClientString( btn:GetName() );
		else
			if ImportantItemConfirmFrame:IsShown() then
				ImportantItemConfirmFrame:Hide();
			end
			SetItemBtn( btn:GetName(), item );
		end
	end
end

function ArmStepUpFrameItemOnEnter( name )
	local btn = getglobal( name );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();

	local nListType = btn:GetClientUserData(0);
	local nGridIndex = btn:GetClientUserData(1);
	local itemId = this:GetClientUserData(2);
	local item = container:getItem( nListType, nGridIndex );
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	elseif itemId > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemId) );
	else
		if name == "ArmStepUpFrameArmItemBtn" then
			SetGameTooltips( this:GetName(), "�����Ҫ�ںϵ�װ��" );
		elseif name == "ArmStepUpFrameStuffItemBtn" then
			SetGameTooltips( this:GetName(), "װ���ں���Ҫ���ĵ�ˮ��" );
		else
			local minImp = ArmStepUpFrameOKBtn:GetClientUserData(0);
			SetGameTooltips( this:GetName(), "��������ں�װ��ͬ�ȼ�ͬ��λ" .. ( t_ImpName[minImp] ~= nil and t_ImpName[minImp] or "�ض�" ) .. "Ʒ�����ϵ�װ��" );
		end
	end
end

function ArmStepUpFrameItemOnRightClick( name )
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "װ���ںϰ���ʾ" then
		MessageBoxFrame:Hide();
	end
	if name == "ArmStepUpFrameStuffItemBtn" then
		return;
	elseif name == "ArmStepUpFrameArmItemBtn" then
		initStepUpFrame();
	else
		ClearItemBtn( name );
		if CheckCostEnough() then
			ArmStepUpFrameOKBtn:Enable();
		end
	end
end

function ArmStepUpFrameItemBtn_OnEnter()
	ArmStepUpFrameItemOnEnter( this:GetName() );
end

function ArmStepUpFrameItemBtn_OnClick()
	if arg1 == "RightButton" then
		ArmStepUpFrameItemOnRightClick( this:GetName() );
	end
end

function ArmStepUpFrameItemBtn_OnBeginDrag()
	if this:GetName() ~= "ArmStepUpFrameStuffItemBtn" then
		ArmStepUpFrameItemOnDrag( this:GetName() );
	end
end

function ArmStepUpFrameItemOnDrag( name )
	--����ʼ�϶�
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "װ���ںϰ���ʾ" then
		return;
	end
	local btn = getglobal( name );
	local nListType	= btn:GetClientUserData(0);
	local nGrid	= btn:GetClientUserData(1);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_STEP_UP;
		local IconTex = getglobal( btn:GetName().."IconTexture" );
		UIBeginDrag( IconTex:GetTexture(), item:getListType(), item:getGridIdx(), btn:GetClientID() );
	end
end

function ClearArmStepUpBtn( btnName, desList, desGrid )
	local btn = getglobal( btnName );
	local nListType = btn:GetClientUserData(0);
	local nGridIdx	= btn:GetClientUserData(1);
	local itemid	= btn:GetClientUserData(2);
	if itemid > 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local item = container:getItem( nListType, nGridIdx );
		if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
			ArmStepUpFrameItemOnRightClick( btnName );
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

function ReceiveArmStepUp( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "װ���ںϰ���ʾ" then
		return;
	end
	if ImportantItemConfirmFrame:IsShown() then
		ImportantItemConfirmFrame:Hide();
	end

	local btn = getglobal( szBtnName );
	-- ���Ŀ�����Ա���
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local srcList	= dragfrom;						-- Դ��������
		local srcGrid	= dragdata1;					-- ��Դ�����ĸ����ӹ���
		local item	= container:getItem( srcList, srcGrid );
		if item:getItemId() == 0 then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		local itemDef	= item:getItemDef();
		
		if szBtnName == "ArmStepUpFrameArmItemBtn" then
			if not CheckCanPutArmItem( item, itemDef ) then
				return;
			end
			local stepDef = GameArmProce:getArmStepUpDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important, itemDef.WearPos1 );
			if stepDef ~= nil and item:getStepLv() == stepDef.MaxStep then
				return ShowMidTips( "��ǰװ���ײ��������������ں�" );
			end

			SetItemBtn( "ArmStepUpFrameArmItemBtn", item );
			SetArmStepInfo( itemDef, item:getStepLv(), item:getStepUse() );
		else
			if ArmStepUpFrameArmItemBtn:GetClientUserData(2) == 0 then
				return ShowMidTips( "���ȷ���Ҫ�ںϵ�װ��" );
			end
			if isFull or useArmNum == 0 then
				return ShowMidTips( "��ǰû�пɷ����������" );
			end
			if not CheckCanPutCostItem( item, itemDef ) then
				return;
			end

			local idx = CheckItemNeedImportantConfirm( item );
			local bImp = itemDef.Important > 3;
			if idx > 0 or bImp then
				ImportantItemConfirmFrameTitle:SetText("װ������ȷ��");
				ShowImportantItemConfirmFrame( "����", item, idx, false, bImp );
				SetItemWarnFrame("ArmStepUpFrame");
				ImportantItemConfirmFrame:SetClientUserData( 0, srcList );
				ImportantItemConfirmFrame:SetClientUserData( 1, srcGrid );
				ImportantItemConfirmFrame:SetClientString( btn:GetName() );
			else
				SetItemBtn( btn:GetName(), item );
			end
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

function ArmStepUpFrameItemBtn_OnRecive()
	if string.find( this:GetName(), "ArmItemBtn", 1, true ) then
		ReceiveArmStepUp( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end
end

function ArmStepUpFrameSignTexBtn_OnEnter()
	local szText = [[1.��%d�������ϵ�װ�����Խ����ں�
2.�ں�Ϊװ�����Ӷ���Ľײ�����
3.��Ҫ�������ں�װ��ͬ�ȼ�ͬ��λ
%sƷ�����ϵ�װ������
4.ÿ���ںϸ������ĵ�װ����������
�Ͻ��ȣ�������ʱ�ײ��1
5.װ�����ױ����ں�����]];
	local minImp = ArmStepUpFrameOKBtn:GetClientUserData(0);
	ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = string.format( szText, STEP_UP_MIN_LV, (t_ImpName[minImp] ~= nil and t_ImpName[minImp] or "�ض�") ),
		frame = this:GetName(),	button = this:GetName() } );
end

function ArmStepUpFrameStepVal_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "�ںϽ������󣬽ײ��1",
		frame = this:GetName(),	button = this:GetName() } );
end

-------------------------------------------��ɫװ������ ����--------------------------------------------
local MAX_ARM_UP_ITEM_NUM = 4;
local MAX_ATTR_OPTION_NUM = 4;
local USE_ID_LEGEND_UP_1 = 25; 	-- ������װ���̳�1����������
local USE_ID_LEGEND_UP_2 = 26; 	-- ������װ���̳�2���������� 
local USE_ID_LEGEND_UP_3 = 27; 	-- ������װ���̳�3���������� 
local srcMainItemID = 0;
local t_ArmUpOrangeTip = 
	{
		[1] = [[#cffffbe����˵����#n
�� ʹ��#cffa0ff��Ӣ����װ��#nΪ��������#cff5000��������װ��#n
�� ����#cffa0ff��Ӣ����װ��#n��ǿ�����ԡ���Ƕ��ʯ����ȫ������
�� �����ɵ�#cff5000��������װ��#n����ӵ��5����ɫ�������Ժ�4
��ϴ������		]],
		[2] = [[#cffffbe����˵����#n
��ѡ��Ҫ�̳е���ɫ�������ԣ��̳����Խ�
��������ɫװ���ϣ�#cff9600���ֻ��ѡ��3������
����#n		]]
	}
function GetItemResultValue1( itemDef, nResult )
	if itemDef.ItemID == 0 then
		return 0;
	end
	for i = 1, MAX_ITEM_RESULT do
		local resultData = itemDef.Result[i-1];
		if resultData.ResultID == nResult then
			return itemDef.Result[i-1].ResultVal2;
		end
	end
	return 0;
end
function ArmUpOrangeFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_CHG");
	this:RegisterEvent( "GE_CHANGE_MONEY" );
	this:RegisterEvent( "GE_GENIUS_UPDATE" );
	local tipRich1 = getglobal("ArmUpOrangeFrameTipRich");
	local tipRich2 = getglobal("ArmUpOrangeFrameTipRich2");
	tipRich1:SetText( t_ArmUpOrangeTip[1],160,155,130 );
	tipRich2:setCenterLine( true );
	local szText = "������ü̳�\n����ɫ����";
	tipRich2:SetText( szText,230,230,90);
	ItemFirstCallBack( "ArmUpOrangeFrame" , Save2ArmUpOrangeFrame );

	for i=1, MAX_ARM_UP_ITEM_NUM do 
		local btn = getglobal( "ArmUpOrangeFrameItemBtn"..i );
		local id = btn:GetClientID();
		if id % 2 > 0 then 
			btn:SetNormalTextureTemplate("BlackCategoryBkg");
		end
	end
end
function ArmUpOrangeFrame_OnEvent()
	if not ArmUpOrangeFrame:IsShown() then 
		return;
	end
	local btn = getglobal( "ArmUpOrangeFrameMainItemBtnItem" )
	local levelUpDef = getArmLevelUPDef( srcMainItemID );

	if arg1 == "GE_CONTAINER_CHG" or arg1 == "GE_CHANGE_MONEY" then
	--	SetSrcArmInformation(  btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );
		SetArmUpNeedItem( levelUpDef );	
		UpdateArmUpCostMoney( levelUpDef );
	elseif arg1 == "GE_GENIUS_UPDATE" then 
		
	end
end

function Save2ArmUpOrangeFrame( item )
	if item:getItemType() == ITEM_SLOTTYPE_ITEM then
		ShowMidTips("�����װ��");
		return;
	end
	-- װ��δ����
	if item:isArmIdentity() then 
		ShowMidTips("��װ����δ���������ܽ��д˲���");
		return;
	end
	if item:getItemId() ~= srcMainItemID then 
		ShowMidTips("��������������Ӧװ��");
		return;
	end
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "��ɫװ��������ʾ" then 
		ShowMidTips("����ȷ���Ƿ���е�ǰװ������");
		return;
	end
	local numFont = getglobal("ArmUpOrangeFrameMainItemBtnNumFont")
	numFont:SetTextColor( 160,160,90 )
	SetSrcArmInformation( srcMainItemID,item:getListType(),item:getGridIdx() );
	numFont:SetText("1/1");
	local levelUpDef = getArmLevelUPDef( srcMainItemID );
	UpdateArmUpCostMoney( levelUpDef );
end

function ArmUpOrangeMainItemBtn_OnReceiveDrag()
	-- grid num id ;
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid );
		Save2ArmUpOrangeFrame( item );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else 
		ShowMidTips( "���Ƚ�װ�����뱳���У�" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
end
function ArmUpOrangeMainItemBtn_OnClick() 
	if arg1 == "RightButton" then
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "��ɫװ��������ʾ" then 
			ShowMidTips("����ȷ���Ƿ���е�ǰװ������");
			return;
		end
		local nList 	= this:GetClientUserData(1)
		local nGrid 	= this:GetClientUserData(2)
		local nItemId 	= this:GetClientUserData(0)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--����
			this:SetClientUserData( 0, 0 )
			this:SetClientUserData( 1, 0 )
			this:SetClientUserData( 2, 0 )
			SetSrcArmInformation( 0,0,0 );
			local levelUpDef = getArmLevelUPDef( srcMainItemID );
			UpdateArmUpCostMoney( levelUpDef );
			if ArmUpOrangeSetFrame:IsShown() then 
				ArmUpOrangeSetFrame:Hide();
			end
		end	
	end
end

function ArmUpOrangeFrame_OnShow()
	srcMainItemID = 0;
	local frame = getglobal( "ArmUpOrangeFrame" );
	local scrollID 		= frame:GetClientUserData( 0 );
	local scrollDef 	= getItemDef( scrollID );
	if scrollDef == nil then
		this:Hide();
		return;
	end
	local itemID = GetItemResultValue1( scrollDef, RESULT_LEGEND_SCROLL );
	if itemID == 0 then 
		return;
	end
	-- ���ó�ɫװ����Ϣ
	srcMainItemID = itemID;
	UpdateArmUpOrangeAttrOptionInfo();
	SetScrollArmInfo( itemID );
	-- �Զ����Srcװ��
	AutoAddSrcArmUpItem( itemID );
end
function SetScrollArmInfo( itemID )
	local levelUpDef = getArmLevelUPDef( itemID );
	if levelUpDef == nil then
		ArmUpOrangeFrame:Hide();
		return;
	end
	local btn = getglobal( "ArmUpOrangeFrameScrollItem" );
	btn:SetClientUserData( 0,levelUpDef.NextItemID );
	btn:SetClientUserData( 1,0 );
	btn:SetClientUserData( 2,0 );
	local texture 	= getglobal( "ArmUpOrangeFrameScrollItemIconTexture" );
	local itemdef 	= getItemDef( levelUpDef.NextItemID )
	local IconPath 	= GetItemEquipIconPath()..itemdef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	texture:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemdef, "ArmUpOrangeFrameScrollItemUVAnimationTex" );
	
	local tianFuFont = getglobal( "ArmUpOrangeFrameTianFuFont" );
	tianFuFont:SetText("װ�������츳��"..itemdef.WearPre.GeniusLimit)

	local scrollID 		= ArmUpOrangeFrame:GetClientUserData( 0 );
	local scrollDef 	= getItemDef( scrollID );
	if scrollDef == nil then
		return;
	end
	local nameFont = getglobal( "ArmUpOrangeFrameNameFont" );
	nameFont:SetText( scrollDef.Name );
	-- ���� ����������Ʒ
	SetArmUpNeedItem( levelUpDef );
	UpdateArmUpCostMoney( levelUpDef );
end

function SetArmUpNeedItem( levelUpDef )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local okBtn = getglobal( "ArmUpOrangeFrameOKBtn" );
	okBtn:Enable();
	for i=1,MAX_ARM_UP_ITEM_NUM do
		local itemId  = levelUpDef.Stuff[i-1].ItemID ;
		local itemNum = levelUpDef.Stuff[i-1].Num ; 
		local btn = getglobal( "ArmUpOrangeFrameItemBtn"..i.."Item" );
		btn:SetClientUserData( 0,itemId );
		btn:SetClientUserData( 1,0 );
		btn:SetClientUserData( 2,0 );
		
		local iconTex = getglobal( "ArmUpOrangeFrameItemBtn"..i.."ItemIconTexture" );
		local nameFon = getglobal( "ArmUpOrangeFrameItemBtn"..i.."NameFont" );
		local numFont = getglobal( "ArmUpOrangeFrameItemBtn"..i.."NumFont" );
		
		local itemdef 	= getItemDef( itemId );
		local IconPath 	= GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		iconTex:SetTexture( IconPath );
		nameFon:SetText( itemdef.Name );
		
		local haveNum = container:getItemCount( itemId );
		numFont:SetText( haveNum.."/"..itemNum );
		
		if container:getItemCount( itemId ) >= itemNum then 
			numFont:SetTextColor( 160,160,90 );
		else 
			numFont:SetTextColor( 255,0,0 );
			okBtn:Disable();
		end
	end
end
function UpdateArmUpCostMoney( levelUpDef )
	local levelUpMoney = levelUpDef.MoneyVal;
	local OptionNum = getCheckOptionInfo();
	
	local useMoney = 0;
	local useDef;
	if OptionNum == 1 then 
		useDef = GameArmProce:getUseDef( USE_ID_LEGEND_UP_1 );
	elseif OptionNum == 2 then 
		useDef = GameArmProce:getUseDef( USE_ID_LEGEND_UP_2 );
	elseif OptionNum == 3 then 
		useDef = GameArmProce:getUseDef( USE_ID_LEGEND_UP_3 );
	end
	local optMoneyFont = getglobal( "ArmUpOrangeSetFrameCostFont" );
	local szText = "";
	if OptionNum > 0 then 
		useMoney = useDef.UseMoney; 
		szText = "�̳�"..OptionNum.."���������Է������ӣ�#cff9600"..useMoney.." ����#n" 	
	end
	optMoneyFont:SetText( szText,255,255,190 );

	levelUpMoney = levelUpMoney + useMoney;
	local moneyFont = getglobal( "ArmUpOrangeFrameCostMoney" );
	moneyFont:SetText( levelUpMoney );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local playerMoney = mainplayer:getMoney();
	local okBtn = getglobal( "ArmUpOrangeFrameOKBtn" );
	if playerMoney < levelUpMoney then 
		moneyFont:SetTextColor( 255,0,0 )	
		okBtn:Disable();
	else 
		moneyFont:SetTextColor( 160,160,90 );
	end
end
function SetSrcArmInformation( itemId,nType,nGrid )	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	
	local srcBtn  = getglobal( "ArmUpOrangeFrameMainItemBtnItem" );
	local numFont = getglobal("ArmUpOrangeFrameMainItemBtnNumFont");
	local nameFont = getglobal("ArmUpOrangeFrameMainItemBtnNameFont");
	local iconTex = getglobal( "ArmUpOrangeFrameMainItemBtnItemIconTexture" );
	local itemDef = getItemDef( srcMainItemID );
	
	if itemId > 0 then 
		local srcItem = container:getItem( nType,nGrid );
		nameFont:SetText( srcItem:getDisplayName() );
	else 
		nameFont:SetText( itemDef.Name );
		iconTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		ArmUpOrangeFrameMainItemBtnItemUVAnimationTex:Hide();
		numFont:SetText("��ӱ���ѡ������װ��");
		UpdateArmUpOrangeAttrOptionInfo();
		return;	
	end
	local lastID  = srcBtn:GetClientUserData( 0 );
	if lastID > 0 then 
		UnLockItem( srcBtn:GetClientUserData(1), srcBtn:GetClientUserData(2), srcBtn:GetClientUserData(0))
	end
	local IconPath 	= GetItemEquipIconPath()..itemDef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	iconTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, "ArmUpOrangeFrameMainItemBtnItemUVAnimationTex" );
	LockItem( nType, nGrid, itemId, "ArmUpOrangeFrame"  );
	srcBtn:SetClientUserData( 0,itemId );
	srcBtn:SetClientUserData( 1,nType );
	srcBtn:SetClientUserData( 2,nGrid );
	UpdateArmUpOrangeAttrOptionInfo();
end

function AutoAddSrcArmUpItem( itemId )
	local srcBtn  = getglobal( "ArmUpOrangeFrameMainItemBtnItem" );
	srcBtn:SetClientUserData( 0,0 );
	srcBtn:SetClientUserData( 1,0 );
	srcBtn:SetClientUserData( 2,0 );
	
	local iconTex = getglobal( "ArmUpOrangeFrameMainItemBtnItemIconTexture" );
	iconTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	ArmUpOrangeFrameMainItemBtnItemUVAnimationTex:Hide();
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	
	local numFont 	= getglobal("ArmUpOrangeFrameMainItemBtnNumFont")
	local nameFont 	= getglobal("ArmUpOrangeFrameMainItemBtnNameFont");
	local itemDef 	= getItemDef( srcMainItemID );
	numFont:SetTextColor( 160,160,90 )
	if container:getItemCount( itemId ) > 1 then 
		numFont:SetText("��ӱ���ѡ������װ��")
		nameFont:SetText(itemDef.Name);
		return;
	elseif container:getItemCount( itemId ) < 1 then 
		numFont:SetText("0/1");
		nameFont:SetText(itemDef.Name);
	end
	for i=0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) - 1 do 
		local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i );
		if item:getItemId() > 0 and item:getItemId() == itemId then 
			SetSrcArmInformation( itemId,CONTAINER_TYPE_SUNDRIES,i );
			numFont:SetText("1/1");
		end
	end
end

function OpenArmUpOrangeFrame( used_Item )
	if used_Item == nil then 
		return;
	end	
	local itemDef = getItemDef( used_Item:getItemId() );
	ClearItemLock("ArmUpOrangeFrame")
	local frame = getglobal( "ArmUpOrangeFrame" );
	if PlayerContainerFrame:IsShown() then
		frame:SetPoint( "topright", "PlayerContainerFrame", "topleft", 0, 0 );
	else 
		frame:SetPoint( "center", "UIClient", "center", 0, 0 );
	end
	frame:SetClientUserData( 0, used_Item:getItemId() );
	frame:SetClientUserData( 1, used_Item:getListType() );
	frame:SetClientUserData( 2, used_Item:getGridIdx() );
	frame:Show();
	LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "ArmUpOrangeFrame" );
end

function ArmUpOrangeFrame_OnHide()
	ClearItemLock("ArmUpOrangeFrame");
	ArmUpOrangeSetFrame:Hide();
end
function ArmUpOrangeItemBtn_OnEnter()
	local itemId = this:GetClientUserData(0);
	if itemId <= 0 then 
		return;
	end
	local nType		= this:GetClientUserData(1);
	local nGrid		= this:GetClientUserData(2);
	local itemDef 	= getItemDef( itemId );
	if itemDef == nil then 
		return;
	end
	if nType > 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local item = container:getItem( nType,nGrid );
		ShowItemTips(  LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
		return;
	end
	ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef);
end
local function IsContainerLocked()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return true; end
	local container = mainplayer:getContainer();

	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		return true;
	end
	return false ;
end

function ArmUpOrangeFrameOKBtn_OnClick()
	local srcArmBtn = getglobal("ArmUpOrangeFrameMainItemBtnItem");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	if srcArmBtn:GetClientUserData(0) <= 0 then 
		ShowMidTips("���ȷ���Ҫ������װ��");
		return;
	end
	if IsContainerLocked() then 
		ShowMidTips("�����������޷�������������֤������ȫ��");
		return;
	end
	local frame = getglobal( "ArmUpOrangeFrame" );
	local nType = srcArmBtn:GetClientUserData(1);
	local nIdx 	= srcArmBtn:GetClientUserData(2);
	local item 	= container:getItem( nType, nIdx );
	local scrollItembtn = getglobal( "ArmUpOrangeFrameScrollItem" );
	local ItemDef = getItemDef( scrollItembtn:GetClientUserData(0) )
	local OptionNum = getCheckOptionInfo();
	
	if OptionNum > 0 then
		MessageBox( "������ʾ", "ȷ����#cffa0ff"..item:getDisplayName().."#nװ��������#cff5000"..ItemDef.Name.."#n����#G�̳�"..OptionNum.."����ɫ��������#n��" );
	else 
		MessageBox( "������ʾ", "ȷ����#cffa0ff"..item:getDisplayName().."#nװ��������#cff5000"..ItemDef.Name.."#n����������ȫ�����̳У�" );
	end
	MessageBoxFrame:SetClientString("��ɫװ��������ʾ");
end
function ArmUpOrangeStart()
	if IsContainerLocked() then 
		ShowMidTips("�����������޷�������������֤������ȫ��");
		return;
	end
	local srcArmBtn = getglobal("ArmUpOrangeFrameMainItemBtnItem");
	local frame = getglobal( "ArmUpOrangeFrame" );
	local OptionNum,nOption = getCheckOptionInfo();
	GameArmProce:requestArmUpOrange( frame:GetClientUserData(1),frame:GetClientUserData(2),srcArmBtn:GetClientUserData(1),srcArmBtn:GetClientUserData(2),OptionNum,nOption );
	ClearItemLock("ArmUpOrangeFrame");
	ArmUpOrangeFrame:Hide();
end

function getCheckOptionInfo()
	local nOption 	= 0; 
	local OptionNum = 0;
	for i=1,MAX_ATTR_OPTION_NUM do 
		if OptionNum == 3 then 
			break;
		end
		local checkBtn = getglobal( "ArmUpOrangeSetFrameOptionBtn"..i.."ChkBtn");
		if checkBtn:GetCheckState() then 
			OptionNum = OptionNum + 1;
			nOption = nOption + 2^( i-1 );
		end
	end
	return OptionNum,nOption;
end
-- ���� ��װ��������tips
function getArmScrollItem( itemDef )
	if CheckItemResult( itemDef, RESULT_LEGEND_SCROLL ) then
		local srcArmID = GetItemResultValue1( itemDef, RESULT_LEGEND_SCROLL );
		if srcArmID > 0 then
			local levelUpDef 	= getArmLevelUPDef( srcArmID );	
			local desArmDef 	= getItemDef( levelUpDef.NextItemID );
			if desArmDef ~= nil then
				return desArmDef.Name;
			end
		end
	end
	return "";
end

function ArmUpOrangeFrameSetBtn_OnClick()
	local srcArmBtn = getglobal("ArmUpOrangeFrameMainItemBtnItem");
	if srcArmBtn:GetClientUserData(0) == 0 then 
		ShowMidTips("�����������Ҫ�Ļ���װ����")
		return;
	end
	if not ArmUpOrangeSetFrame:IsShown() then 
		ArmUpOrangeSetFrame:SetPointAlwaysRecalc("topleft", "ArmUpOrangeFrame", "topright",0 ,0 );
		ArmUpOrangeSetFrame:Show() ;
	else 
		ArmUpOrangeSetFrame:Hide() ;
	end
end
function ArmUpOrangeFrameCancelBtn_OnClick()
	ArmUpOrangeFrame:Hide();
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "��ɫװ��������ʾ" then 
		MessageBoxFrame:Hide();	
	end
end

function ArmUpOrangeSetFrame_OnLoad()
	ArmUpOrangeSetFrameTipRich:SetText( t_ArmUpOrangeTip[2],160,155,130 );
	for i=1, MAX_ATTR_OPTION_NUM do 
		local btn 	= getglobal( "ArmUpOrangeSetFrameOptionBtn"..i );
		local id = btn:GetClientID();
		if id % 2 > 0 then 
			btn:SetNormalTextureTemplate("BlackCategoryBkg");
		end
	end
end
function UpdateArmUpOrangeAttrOptionInfo()
	local srcItemBtn = getglobal( "ArmUpOrangeFrameMainItemBtnItem" )
	local itemID = srcItemBtn:GetClientUserData(0);
	for i=1,MAX_ATTR_OPTION_NUM do
		local checkBtn = getglobal( "ArmUpOrangeSetFrameOptionBtn" .. i .. "ChkBtn");
		local font = getglobal( "ArmUpOrangeSetFrameOptionBtn" .. i .. "Font" );
		checkBtn:setCheckState( false );
		checkBtn:Hide();
		font:SetText("");
	end
	if itemID <= 0 then 
		return;
	end
	local itemType = srcItemBtn:GetClientUserData(1);
	local itemGrid = srcItemBtn:GetClientUserData(2);	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return ; end 
	local container = mainplayer:getContainer();
	local armItem = container:getItem( itemType,itemGrid )	;
	if armItem ~= nil and armItem:getItemId() > 0 then
		local itemDef = armItem:getItemDef();
		for i=1,armItem:getArmAddAttrNum() do
			if i > MAX_ATTR_OPTION_NUM then break end
			local checkBtn = getglobal( "ArmUpOrangeSetFrameOptionBtn" .. i .. "ChkBtn");
			local font = getglobal( "ArmUpOrangeSetFrameOptionBtn" .. i .. "Font" );
			checkBtn:Show();
			local szDesc,bFull = ArmWashFrameGetAddAttrDesc( armItem:getArmAddAttr( i-1 ), itemDef );
			if szDesc ~= "" then
				font:SetText( szDesc );
			end
		end
	end
end
function ArmUpOrangeSetFrame_OnShow()

end
function ArmUpOrangeSetChkBtn_OnClick()
	local optionNum = 0;
	for i=1,MAX_ATTR_OPTION_NUM do
		local checkBtn = getglobal( "ArmUpOrangeSetFrameOptionBtn" .. i .. "ChkBtn");
		if checkBtn:IsShown() and checkBtn:GetCheckState() then 
			optionNum = optionNum + 1;
		end
	end
	if optionNum > 3 then 
		ShowMidTips( "��ǰ����ѡ��̳�3����ɫ����" );
		this:setCheckState(false);
		return;
	end
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "��ɫװ��������ʾ" then 
		ShowMidTips("����ȷ���Ƿ����װ������");
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	if srcMainItemID == 0 then 
		return;
	end
	local levelUpDef = getArmLevelUPDef( srcMainItemID );
	UpdateArmUpCostMoney( levelUpDef );
end
function ArmUpOrangeSetFrameOKBtn_OnClick()
	ArmUpOrangeSetFrame:Hide();
end