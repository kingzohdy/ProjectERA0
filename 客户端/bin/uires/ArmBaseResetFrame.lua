local MAX_ATTR_NUM = 5;
local useMoneyNum = 0;
local useItemId = 0;
local t_SuccVal = {};

local szTipDesc = [[●能重置#cff9600【%d级】#n%s的装备，改变装备的绿色属性
●贵重装备只能手动放入废弃装备栏内，无法自动放入
#cc8ff00●只能用同等级段的装备重置(例如:60-69一个等级段)
●可重置部位：主手、副手、头盔、肩膀、胸甲、鞋子]];
--1：主手，2：头盔，3：衣，5：副手，6：面，7：肩，9：徽章，12/13：戒指，15：足，17：项链，20/21：护符
local t_CanInputIdx={ 1, 2, 3, 5, 7, 15 };
local t_RateDesc = { "普通", "中等", "较高", "高", "很高" };

local function CheckCanPutPos( pos )
	for i=1, #t_CanInputIdx do
		if t_CanInputIdx[i] == pos then
			return true;
		end
	end
	return false;
end

local function GetAddAttrDesc( itemDef, attrID, attrVal )
	local szDesc = "";
	local addAttrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
	local newAddAttrDef = getNewAddAttrDef( attrID );
	if addAttrRandDef ~= nil and newAddAttrDef ~= nil then
		local base = newAddAttrDef.Val1[ addAttrRandDef.ValLevel - 1 ];
		local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
		local val = base*attrVal/100*mul/100 ;
		local tAddattrVal = { val, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
		
		szDesc = getNewAttrDesc( attrID, tAddattrVal )
		if szDesc ~= "" then
			szDesc = "● " .. szDesc;
		end
	end
	return szDesc;
end

local function SetArmAttrDesc( name, idx, attrID, attrVal )
	local font = getglobal( "ArmBaseResetFrame" .. name .. "Attr" .. idx .. "Font" );
	local itemId = ArmBaseResetFrameArmItemBtn:GetClientUserData(2);
	if attrID ~= nil and attrVal ~= nil and itemId ~= 0 then
		local szText = GetAddAttrDesc( getItemDef( itemId ), attrID, attrVal );
		font:SetText( szText );
	else
		font:SetText( "" );
	end
end

local function SetResetAttrRate()
	local idx = #t_SuccVal;
	if idx > 0 then
		local val = 0;
		for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
			local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
			if btn:GetClientUserData(2) > 0 then
				local itemDef = getItemDef( btn:GetClientUserData(2) );
				local gradeValDef = GameArmProce:getArmGradeValDef( itemDef.Important );
				if gradeValDef ~= nil then
					val = val + gradeValDef.Val;
				end
			end
		end

		if val == 0 then
			ArmBaseResetFrameArmItemBtnRate:SetText( "" );
			return false;
		end

		for i=1, #t_SuccVal do
			if val < t_SuccVal[i] then
				idx = i;
				break;
			end
		end
		ArmBaseResetFrameArmItemBtnRate:SetText( "极品属性概率：" .. t_RateDesc[idx] );
		return true;
	else
		ArmBaseResetFrameArmItemBtnRate:SetText( "" );
		return false;
	end
end

local function ClearItemBtn( name )
	local btn = getglobal( name );
	local UVAnimationTex = getglobal( name.."UVAnimationTex");
	local ItemTex = getglobal( name.."IconTexture");
	local BindTex = getglobal( name.."BindTexture");
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
end

local function CheckCostEnough()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;

	local font = getglobal( "ArmBaseResetFrameCostFont" );
	if mainplayer:getMoney() < useMoneyNum then
		font:SetTextColor( 230, 0, 0 );
	else
		font:SetTextColor( 160, 160, 90 );
	end

	if not SetResetAttrRate() then
		return false;
	end
	
	local container	= mainplayer:getContainer();
	if useItemId == 0 or container:getItemCount( useItemId ) == 0 then
		return false;
	end

	for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
		local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
		if btn:GetClientUserData(2) > 0 then
			return true;
		end
	end

	return false;
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

local function SetItemBtn( name, item )
	local btn = getglobal( name );
	local ItemTex = getglobal( btn:GetName().."IconTexture");
	local BindTex = getglobal( btn:GetName().."BindTexture");
	if btn:GetClientUserData(2) > 0 then
		UnLockItem( btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );
	end
	btn:SetClientUserData( 0, item and item:getListType() or 0 );
	btn:SetClientUserData( 1, item and item:getGridIdx() or 0 );
	btn:SetClientUserData( 2, item and item:getItemId() or itemId );
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId() );

	local nBind = item:isItemBind() and 1 or 0;
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

	if CheckCostEnough() then
		ArmBaseResetFrameOKBtn:Enable();
	else
		ArmBaseResetFrameOKBtn:Disable();
	end
end

local function ResetCostItemBtn()
	for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
		local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
		ClearItemBtn( btn:GetName() );
	end
	ArmBaseResetFrameOKBtn:Disable();
	ArmBaseResetFrameArmItemBtnRate:SetText("");
end

local function SetArmResetInfo( item )
	t_SuccVal = {};
	useMoneyNum = 0;
	ResetCostItemBtn();
	for i=1, MAX_ATTR_NUM do
		SetArmAttrDesc( "New", i );
	end

	if item ~= nil then
		local itemDef = item:getItemDef();
		local num = item:getArmAddAttrNum();
		for i=1, MAX_ATTR_NUM do
			if i > num then
				SetArmAttrDesc( "Now", i );
			else
				local addAttrInst = item:getArmAddAttr( i-1 );
				SetArmAttrDesc( "Now", i, addAttrInst.AddAttrID, addAttrInst.Val );
			end
		end
		local resetDef = GameArmProce:getArmResetDef( math.floor( itemDef.WearPre.RoleLevelMin / 10 ) );
		if resetDef ~= nil then
			useMoneyNum = resetDef.UseMoney;
			for i=1, ARM_RESET_MAX_DIST_TOLUA do
				table.insert( t_SuccVal, resetDef.Reset[i-1].Val );
			end
			if useItemId > 0 then
				ArmBaseResetFrameAutoBtn:Enable();
			end
		end
	else
		for i=1, MAX_ATTR_NUM do
			SetArmAttrDesc( "Now", i );
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local font = getglobal( "ArmBaseResetFrameCostFont" );
	font:SetText( useMoneyNum );
	if mainplayer:getMoney() < useMoneyNum then
		font:SetTextColor( 230, 0, 0 );
	else
		font:SetTextColor( 160, 160, 90 );
	end
end

local function initBaseResetFrame()
	ClearItemBtn( "ArmBaseResetFrameArmItemBtn" );
	ArmBaseResetFrameArmItemBtnModelView:Hide();
	ArmBaseResetFrameOKBtn:Disable();
	ArmBaseResetFrameOKBtn:SetText( "开始重置" );
	ArmBaseResetFrameAutoBtn:Disable();
	SetArmResetInfo();
	SetResetAttrRate();

	for i=1, MAX_ATTR_NUM do
		SetArmAttrDesc( "Now", i );
		SetArmAttrDesc( "New", i );
	end
end

local function CheckCanPutArmItem( item, itemDef )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入要重置的装备");
		return false;
	end
	
	if not CheckCanPutPos( itemDef.WearPos1 ) then
		ShowMidTips( "此部位的装备无法进行属性重置操作！" );
		return false;
	end

	local minLv = ArmBaseResetFrame:GetClientUserData(2);
	if itemDef.WearPre.RoleLevelMin > minLv then
		ShowMidTips( "此齿轮只能重置" .. minLv + 1 .. "级以下装备的绿色属性" );
		return false;
	elseif itemDef.Important < 1 then
		ShowMidTips( "请放入绿色品质以上的装备" );
		return false;
	end
	return true;
end

local function CheckCanPutCostItem( item, itemDef )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入要作为消耗的废弃装备");
		return false;
	end

	local armItemDef = getItemDef( ArmBaseResetFrameArmItemBtn:GetClientUserData(2) );
	if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
		ShowMidTips( "时装道具不能作为消耗的废弃装备" );
		return false;
	elseif armItemDef.WearPos1 == EQUIP_POS_BADGE then
		ShowMidTips( "徽章装备不能作为消耗的废弃装备" );
		return false;
	elseif math.floor( itemDef.WearPre.RoleLevelMin / 10 ) ~= math.floor( armItemDef.WearPre.RoleLevelMin / 10 ) then
		ShowMidTips( "只能用同等级段的装备进行重置（例如：60-69一个等级段）" );
		return false;
	elseif itemDef.Important < 1 then
		ShowMidTips( "请放入绿色品质以上的装备" );
		return false;
	end
	return true;
end

function ArmBaseResetFrame_OnLoad()
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ItemFirstCallBack( "ArmBaseResetFrame", Save2ArmBaseResetItem );

	for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
		local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
		btn:SetPoint( "topleft", "ArmBaseResetFrame", "topleft", 49 + (i-1)%2*184, 55 + math.ceil(i/2)*92 );
		local tex = getglobal( btn:GetName() .. "BackTexture" );
		tex:Show();
	end

	for i=1, MAX_ATTR_NUM do
		local nowTex = getglobal( "ArmBaseResetFrameNowAttr" .. i .. "BlackTex" );
		local newTex = getglobal( "ArmBaseResetFrameNewAttr" .. i .. "BlackTex" );
		if i%2 == 0 then
			nowTex:Hide();
			newTex:Hide();
		end
	end
end

function ArmBaseResetFrame_OnShow()
	if PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	else
		PlayerContainerFrame:Show();
		PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	end
	this:SetPoint( "topright","PlayerContainerFrame","topleft", 0, 0 );
end

function ArmBaseResetFrame_OnHide()
	UnLockItem( this:GetClientUserData(0), this:GetClientUserData(1) );
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	this:SetClientUserData( 3, 0 );
	initBaseResetFrame();
	useItemId = 0;
end

function ArmBaseResetFrame_OnEvent()
	if not this:IsShown() then return end;
	if arg1 == "GE_CHANGE_MONEY" then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		local font = getglobal( "ArmBaseResetFrameCostFont" );
		if mainplayer:getMoney() < useMoneyNum then
			font:SetTextColor( 230, 0, 0 );
		else
			font:SetTextColor( 160, 160, 90 );
		end
	end
end

function ArmBaseResetFrameAutoBtn_OnEnter()
	SetGameTooltips( this:GetName(), "点击自动放入符合条件的废弃装备，贵重装备将不会被放入" );
end

function ArmBaseResetFrameAutoBtn_OnClick()
	local armId = ArmBaseResetFrameArmItemBtn:GetClientUserData(2);
	if armId == 0 or #t_SuccVal == 0 then
		return ShowMidTips( "请先放入要进行重置的装备" );
	elseif useItemId == 0 then
		return ShowMidTips( "当前已经没有可用的重置道具" );
	else
		local count = 0;
		local val = 0;
		local maxVal = t_SuccVal[#t_SuccVal];
		for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
			local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
			if btn:GetClientUserData(2) > 0 then
				count = count + 1;
				local itemDef = getItemDef( btn:GetClientUserData(2) );
				local gradeValDef = GameArmProce:getArmGradeValDef( itemDef.Important );
				if gradeValDef ~= nil then
					val = val + gradeValDef.Val;
				end
			end
		end

		if count == ARM_BASE_ATTR_RESET_MAX_TOLUA then
			return ShowMidTips( "废弃装备栏已满，无法投入装备" );
		elseif val >= maxVal then
			return ShowMidTips( "极品属性概率已达上限，不需重复投入装备" );
		end
		
		local armDef = getItemDef( armId );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		local container	= mainplayer:getContainer();
		local t_ArmItem = {};
		for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
			local itemDef = item:getItemDef();
			if item:getItemType() == ITEM_SLOTTYPE_AMM and itemDef.Important > 0 and itemDef.Important < 4 and
				math.floor( itemDef.WearPre.RoleLevelMin / 10 ) == math.floor( armDef.WearPre.RoleLevelMin / 10 ) and
				GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) ~= "时装" and itemDef.WearPos1 ~= EQUIP_POS_BADGE and
				not IsItemLock( item:getListType(), item:getGridIdx() ) then
				if CheckItemNeedImportantConfirm( item ) == 0 then
					table.insert( t_ArmItem, { item, itemDef.Important } );
				end
			end
		end

		if #t_ArmItem > 0 then
			table.sort( t_ArmItem, function( a, b )
				return a[2] < b[2];
			end);
			count = 1;
			val = 0;
		else
			return ShowMidTips( "包内没有此等级段内的非贵重装备，无法自动放入！" );
		end

		for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
			if count > #t_ArmItem then
				break;
			end
			if val > maxVal then
				return ShowMidTips( "极品属性概率已达上限，停止放入装备" );
			end

			local imp = 0;
			local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
			if btn:GetClientUserData(2) > 0 then
				local itemDef = getItemDef( btn:GetClientUserData(2) );
				imp = itemDef.Important;
			else
				SetItemBtn( btn:GetName(), t_ArmItem[count][1] );
				imp = t_ArmItem[count][2];
				count = count + 1;
			end

			local gradeValDef = GameArmProce:getArmGradeValDef( imp );
			if gradeValDef ~= nil then
				val = val + gradeValDef.Val;
			end
		end
	end
end

function ArmBaseResetFrameOKBtn_OnClick()
	if this:GetText() == "关  闭" then
		ArmBaseResetFrameCloseBtn_OnClick();
		return;
	else
		if MessageBoxFrame:IsShown() and ( 
			MessageBoxFrame:GetClientString() == "属性重置绑定提示" or
			MessageBoxFrame:GetClientString() == "放弃属性重置保存" or 
			MessageBoxFrame:GetClientString() == "属性重置关闭提示" ) then
			return;
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if mainplayer:getMoney() < useMoneyNum then
		return ShowMidTips( "银币不足，无法进行属性重置" );
	end
	
	local nBind = ArmBaseResetFrameArmItemBtn:GetClientUserData(3);
	if nBind == 0 then
		if ArmBaseResetFrame:GetClientUserData(3) > 0 then
			nBind = 1;
		else
			for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
				local btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
				if btn:GetClientUserData(3) > 0 then
					nBind = 1;
					break;
				end
			end
		end

		if nBind > 0 then
			MessageBox( "绑定提示", "继续重置则重置装备会被绑定，是否继续重置？" );
			MessageBoxFrame:SetClientString("属性重置绑定提示");
			return;
		end
	end
	ArmBaseResetOpt();
end

function ArmBaseResetOpt()
	local data = GameArmProce:getArmBaseResetData();
	local btn = getglobal( "ArmBaseResetFrameArmItemBtn" );
	data.Src.ArmListType = btn:GetClientUserData(0);
	data.Src.ArmGridIdx = btn:GetClientUserData(1);
	btn = getglobal( "ArmBaseResetFrame" );
	data.UseItem.ArmListType = btn:GetClientUserData(0);
	data.UseItem.ArmGridIdx = btn:GetClientUserData(1);
	local count = 0;
	for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
		btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
		if btn:GetClientUserData(2) > 0 then
			data.ArmStuffs[count].ArmListType = btn:GetClientUserData(0);
			data.ArmStuffs[count].ArmGridIdx = btn:GetClientUserData(1);
			count = count + 1;
		end
	end
	data.ArmNum = count;
	GameArmProce:requestArmBaseReset( data );
end

function ArmBaseResetFrameSaveBtn_OnClick()
	local btn = getglobal( "ArmBaseResetFrameArmItemBtn" );
	local nListType = btn:GetClientUserData(0);
	local nGridIdx	= btn:GetClientUserData(1);
	GameArmProce:requestArmBaseResetSave( nListType, nGridIdx );
end

function ArmBaseResetFrameCancelBtn_OnClick()
	ArmBaseResetFrameOpt:Hide();
end

function ArmBaseResetFrameOpt_OnHide()
	for i=1, MAX_ATTR_NUM do
		SetArmAttrDesc( "New", i );
	end
end

function ArmBaseResetFrameCloseBtn_OnClick()
	if ArmBaseResetFrameOpt:IsShown() then
		MessageBox( "", "本次重置结果未保存，确定关闭页面吗？" );
		MessageBoxFrame:SetClientString( "属性重置关闭提示" );
		return;
	end
	ArmBaseResetFrame:Hide();
end

function UseBaseAttrResetItem( nListType, nGridIdx )
	if MessageBoxFrame:IsShown() and ( 
		MessageBoxFrame:GetClientString() == "属性重置绑定提示" or
		MessageBoxFrame:GetClientString() == "放弃属性重置保存" or 
		MessageBoxFrame:GetClientString() == "属性重置关闭提示" ) then
		return;
	end
	
	if ArmBaseResetFrame:IsShown() then
		ArmBaseResetFrameCloseBtn_OnClick();
	else
		ArmBaseResetFrameOpt:Hide();
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx );
	if item:getItemId() == 0 then return end;
	LockItem( nListType, nGridIdx, item:getItemId() );
	useItemId = item:getItemId();
	local itemDef = item:getItemDef();
	local minLv = math.floor(itemDef.Result[0].ResultVal1 % 10) ~= 9 and itemDef.Result[0].ResultVal1 or itemDef.Result[0].ResultVal1 + 1;
	local desc = math.floor(itemDef.Result[0].ResultVal1 % 10) ~= 9 and "" or "以下";
	ArmBaseResetFrameRich:SetText( string.format( szTipDesc, minLv, desc ), 160, 155, 130 );
	ArmBaseResetFrame:SetClientUserData( 0, nListType );
	ArmBaseResetFrame:SetClientUserData( 1, nGridIdx );
	ArmBaseResetFrame:SetClientUserData( 2, itemDef.Result[0].ResultVal1 );
	ArmBaseResetFrame:SetClientUserData( 3, item:isItemBind() and 1 or 0 );
	ArmBaseResetFrameOKBtn:Disable();
	ArmBaseResetFrameAutoBtn:Disable();
	ArmBaseResetFrame:Show();
end

function BaseAttrResetResult( flag, num )
	if flag == 0 then return end;
	ArmBaseResetFrameOpt:Show();
	local btn = getglobal( "ArmBaseResetFrameArmItemBtn" );
	local modelview = getglobal( btn:GetName() .. "ModelView" );
	modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent");
	modelview:setPetScale( 0, 0.9, 0.9, 0.3 );
	modelview:setEntityPosition(0, 0, 100, 0);
	if not modelview:IsShown() then
		modelview:Show()
	end
	for i=1, MAX_ATTR_NUM do
		if i > num then
			SetArmAttrDesc( "New", i );
		else
			local data = GameArmProce:getArmResetResult( i-1 );
			SetArmAttrDesc( "New", i, data.AddAttrID, data.Val );
		end
	end

	ResetCostItemBtn();

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local nListType	= btn:GetClientUserData(0);
	local nGrid	= btn:GetClientUserData(1);
	local item = container:getItem( nListType, nGrid );
	SetItemBind( btn:GetName(), item:isItemBind() );

	local t_Item = {};
	for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
		if item:getItemId() == useItemId then
			table.insert( t_Item, { item, (item:isItemBind() and 1 or 0) } );
		end
	end

	if #t_Item > 0 then
		UnLockItem( ArmBaseResetFrame:GetClientUserData(0), ArmBaseResetFrame:GetClientUserData(1), ArmBaseResetFrame:GetClientUserData(2) );
		table.sort( t_Item, function( a,b )
		return a[2] > b[2];
		end);
		item = t_Item[1][1];
		ArmBaseResetFrame:SetClientUserData( 0, item:getListType() );
		ArmBaseResetFrame:SetClientUserData( 1, item:getGridIdx() );
		ArmBaseResetFrame:SetClientUserData( 3, item:isItemBind() and 1 or 0 );
		LockItem( item:getListType(), item:getGridIdx(), useItemId );
		ArmBaseResetFrameOKBtn:SetText( "开始重置" );
	else
		useItemId = 0;
		ArmBaseResetFrameOKBtn:SetText( "关  闭" );
		ArmBaseResetFrameOKBtn:Enable();
	end
end

function BaseAttrSaveResult( flag )
	if flag > 0 then
		ArmBaseResetFrameOpt:Hide();
		local btn = getglobal( "ArmBaseResetFrameArmItemBtn" );
		local nListType	= btn:GetClientUserData(0);
		local nGrid	= btn:GetClientUserData(1);
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local item = container:getItem( nListType, nGrid );
		SetArmResetInfo( item );
		if useItemId == 0 then
			ArmBaseResetFrameOKBtn:Enable();
		end
	end
end

function ArmBaseResetFrameItemOnEnter( name )
	local btn = getglobal( name );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();

	local nListType = btn:GetClientUserData(0);
	local nGridIndex = btn:GetClientUserData(1);
	local itemId = btn:GetClientUserData(2);
	local item = container:getItem( nListType, nGridIndex );
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	elseif itemId > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemId) );
	else
		if name == "ArmBaseResetFrameArmItemBtn" then
			SetGameTooltips( this:GetName(), "请放入要重置的装备" );
		else
			SetGameTooltips( this:GetName(), "请放入要作为消耗的废弃装备" );
		end
	end
end

function ArmBaseResetFrameItemOnRightClick( name )
	if MessageBoxFrame:IsShown() and ( 
		MessageBoxFrame:GetClientString() == "属性重置绑定提示" or
		MessageBoxFrame:GetClientString() == "放弃属性重置保存" or 
		MessageBoxFrame:GetClientString() == "属性重置关闭提示" ) then
		return;
	end
	if name == "ArmBaseResetFrameArmItemBtn" then
		if ArmBaseResetFrameOpt:IsShown() then
			MessageBox( "", "本次重置结果未保存，#cdc0000放弃保存属性吗？" );
			MessageBoxFrame:SetClientString( "放弃属性重置保存" );
			return;
		end
		initBaseResetFrame();
	else
		ClearItemBtn( name );
		if not SetResetAttrRate() then
			ArmBaseResetFrameOKBtn:Disable();
		end
	end
end

function ArmBaseResetFrameItemBtn_OnClick()
	if arg1 == "RightButton" then
		ArmBaseResetFrameItemOnRightClick( this:GetName() );
	end
end

function ArmBaseResetFrameItemBtn_OnEnter()
	ArmBaseResetFrameItemOnEnter( this:GetName() );
end

function ArmBaseResetFrameItemBtn_OnRecive()
	ReceiveArmBaseReset( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
end

function ArmBaseResetFrameItemBtn_OnBeginDrag()
	ArmBaseResetFrameItemOnDrag( this:GetName() );
end

function ArmBaseResetFrameItemOnDrag( name )
	if MessageBoxFrame:IsShown() and ( 
		MessageBoxFrame:GetClientString() == "属性重置绑定提示" or
		MessageBoxFrame:GetClientString() == "放弃属性重置保存" or 
		MessageBoxFrame:GetClientString() == "属性重置关闭提示" ) then
		return;
	end
	if name == "ArmBaseResetFrameArmItemBtn" and  ArmBaseResetFrameOpt:IsShown() then
		MessageBox( "", "本次重置结果未保存，#cdc0000放弃保存属性吗？" );
		MessageBoxFrame:SetClientString( "放弃属性重置保存" );
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
		UIMSG:getUIGV().DragFrom = DRAG_ARM_BASE_RESET;
		local IconTex = getglobal( btn:GetName().."IconTexture" );
		UIBeginDrag( IconTex:GetTexture(), item:getListType(), item:getGridIdx(), btn:GetClientID() );
	end
end

function SetArmBaseResetItem( name, item )
	SetItemBtn( name, item );
end

function Save2ArmBaseResetItem( item, nSrcList, nSrcGridIndex )
	if MessageBoxFrame:IsShown() and ( 
		MessageBoxFrame:GetClientString() == "属性重置绑定提示" or
		MessageBoxFrame:GetClientString() == "放弃属性重置保存" or 
		MessageBoxFrame:GetClientString() == "属性重置关闭提示" ) then
		return;
	end
	if ImportantItemConfirmFrame:IsShown() then
		ImportantItemConfirmFrame:Hide();
	end
	
	local itemDef = item:getItemDef();
	if itemDef == nil then return end;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	if ArmBaseResetFrameArmItemBtn:GetClientUserData(2) == 0 then
		if not CheckCanPutArmItem( item, itemDef ) then
			return;
		end
		SetItemBtn( "ArmBaseResetFrameArmItemBtn", item );
		SetArmResetInfo( item );
	else
		if useItemId == 0 then
			return ShowMidTips( "当前已经没有可用的重置道具" );
		end
		if not CheckCanPutCostItem( item, itemDef ) then
			return;
		end

		local btn = getglobal( "ArmBaseResetFrameItemBtn1" );
		for i=1, ARM_BASE_ATTR_RESET_MAX_TOLUA do
			btn = getglobal( "ArmBaseResetFrameItemBtn" .. i );
			if btn:GetClientUserData(2) == 0 then
				break;
			end
		end

		local idx = CheckItemNeedImportantConfirm( item );
		local bImp = itemDef.Important > 3;
		if idx > 0 or bImp then
			ImportantItemConfirmFrameTitle:SetText("装备废弃确认");
			ShowImportantItemConfirmFrame( "废弃", item, idx, false, bImp );
			SetItemWarnFrame("ArmBaseResetFrame");
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

function ReceiveArmBaseReset( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if MessageBoxFrame:IsShown() and ( 
		MessageBoxFrame:GetClientString() == "属性重置绑定提示" or
		MessageBoxFrame:GetClientString() == "放弃属性重置保存" or 
		MessageBoxFrame:GetClientString() == "属性重置关闭提示" ) then
		return;
	end
	if ImportantItemConfirmFrame:IsShown() then
		ImportantItemConfirmFrame:Hide();
	end

	local btn = getglobal( szBtnName );
	-- 如果目标来自背包
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local srcList	= dragfrom;						-- 源容器类型
		local srcGrid	= dragdata1;					-- 从源容器哪个格子过来
		local item	= container:getItem( srcList, srcGrid );
		if item:getItemId() == 0 then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		local itemDef	= item:getItemDef();
		
		if szBtnName == "ArmBaseResetFrameArmItemBtn" then
			if ArmBaseResetFrameOpt:IsShown() then
				MessageBox( "", "本次重置结果未保存，#cdc0000放弃保存属性吗？" );
				MessageBoxFrame:SetClientString( "放弃属性重置保存" );
				return;
			end
			if not CheckCanPutArmItem( item, itemDef ) then
				return;
			end

			SetItemBtn( "ArmBaseResetFrameArmItemBtn", item );
			SetArmResetInfo( item );
		else
			if ArmBaseResetFrameArmItemBtn:GetClientUserData(2) == 0 then
				return ShowMidTips( "请先放入需要重置绿色属性的装备！" );
			elseif useItemId == 0 then
				return ShowMidTips( "当前已经没有可用的重置道具" );
			end
			if not CheckCanPutCostItem( item, itemDef ) then
				return;
			end

			local idx = CheckItemNeedImportantConfirm( item );
			local bImp = itemDef.Important > 3;
			if idx > 0 or bImp then
				ImportantItemConfirmFrameTitle:SetText("装备废弃确认");
				ShowImportantItemConfirmFrame( "废弃", item, idx, false, bImp );
				SetItemWarnFrame("ArmBaseResetFrame");
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

function ClearArmBaseResetBtn( btnName, desList, desGrid )
	local btn = getglobal( btnName );
	local nListType = btn:GetClientUserData(0);
	local nGridIdx	= btn:GetClientUserData(1);
	local itemid	= btn:GetClientUserData(2);
	if itemid > 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local item = container:getItem( nListType, nGridIdx );
		if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
			ArmBaseResetFrameItemOnRightClick( btnName );
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end