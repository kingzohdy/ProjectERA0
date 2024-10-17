local MAX_WASH_ATTR_NUM = 10
local nLastRequestRecast = 0
local MOVE_WIDTH_TICK = 1;

function ArmWashFrameProcessOnReciveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3, bDrag )
	if dragfrom == DRAG_ARM_WASH then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateArmWashFrameItemInfo();
	elseif dragfrom == CONTAINER_TYPE_TASK then
		ShowMidTips( "请放入装备或者洗炼石" )
		return
	elseif dragfrom ~= CONTAINER_TYPE_SUNDRIES then
		ShowMidTips("只有背包内装备才可进行此操作");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local srcList		= dragfrom;						-- 源容器类型
	local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
	local item			= container:getItem( srcList, srcGrid );
	local itemdef = item:getItemDef();
	local btn = getglobal( szBtnName )
	if szBtnName == "ArmWashFrameStuffItemBtn" then		
		if not CheckItemResult( itemdef, RESULT_ARM_WASH_STUFF ) and not CheckItemResult( itemdef, RESULT_ARM_WASH_STUFF_MUST ) then
			if bDrag then
				ShowMidTips( "请放入洗炼石" )
			else
				ShowMidTips( "请放入装备或者洗炼石" )
			end			
			return
		end

		local nArmListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
		local nArmGridIdx  = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
		local armItem = container:getItem( nArmListType,nArmGridIdx )
		if armItem:getItemId() > 0 then
			local nMaxWashLv = 0;
			for i=1,MAX_ITEM_RESULT do
				if itemdef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF or itemdef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF_MUST then
					nMaxWashLv = itemdef.Result[i-1].ResultVal1;
					break;
				end
			end
			-- 对等级判断
			if armItem:getItemDef().WearPre.RoleLevelMin > nMaxWashLv then
				ShowMidTips( "装备等级与洗炼石不符" )
				return;
			end
		end

		local nList = btn:GetClientUserData( 0 );
		local nGrid  = btn:GetClientUserData( 1 );
		local nItemId = btn:GetClientUserData( 2 );
		
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		btn:SetClientUserData( 2,item:getItemId() )
		if item:isItemBind() then
			btn:SetClientUserData( 3,1 )
		else
			btn:SetClientUserData( 3,0 )
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateArmWashFrameItemInfo();
	else
		if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
			if bDrag then
				ShowMidTips( "请放入装备" )
			else
				ShowMidTips( "请放入装备或者洗炼石" );
			end
			return;
		end

		if itemdef.Important == 0 then
			ShowMidTips( "白色品质不能洗炼" );
			return;
		end

		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
			ShowMidTips( "时装不能洗炼" )
			return
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能洗炼" );
			return;
		end
		if item:isArmFengYin() then
			ShowMidTips("封印装备无法进行此操作");
			return;
		end
		if item:isArmIdentity() then 
			ShowMidTips("此装备还未鉴定，不能进行此操作");
			return;
		end
		local stuffItemId = ArmWashFrameStuffItemBtn:GetClientUserData( 2 )
		if container:getItemCount( stuffItemId ) > 0 then
			local stuffItemDef = getItemDef( stuffItemId )
			local nMaxWashLv = 0;
			for i=1,MAX_ITEM_RESULT do
				if stuffItemDef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF or stuffItemDef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF_MUST then
					nMaxWashLv = stuffItemDef.Result[i-1].ResultVal1;
					break;
				end
			end
			-- 对等级判断
			if itemdef.WearPre.RoleLevelMin > nMaxWashLv then
				ShowMidTips( "装备等级与洗炼石不符" )
				return;
			end
		end

		if ArmWashFrameSaveBtn:IsShown() then
			MessageBox( "", "本次洗炼结果未保存，确定放弃保存吗？" )
			MessageBoxFrame:SetClientString( "洗炼替换保存" )
			MessageBoxFrame:SetClientUserData( 0, srcList )
			MessageBoxFrame:SetClientUserData( 1, srcGrid )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		
		local nList = btn:GetClientUserData( 0 );
		local nGrid  = btn:GetClientUserData( 1 );
		local armItem = container:getItem( nList,nList );			
		local nItemId	= armItem:getItemId();
		if nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		ArmWashFrameAutoAddNeedItem( itemdef, "ArmWashFrame" );
		UpdateArmWashFrameItemInfo();
		ArmWashFrameUpdateNewAddAttr( );
	end
end

function ArmWashFrameAutoAddNeedItem( ArmDef, name )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local itemBtn	= getglobal( name .. "StuffItemBtn");
	local armBtn	= getglobal( name .. "ArmItemBtn");
	local WashItemGrid	= 0;
	local WashItemBind	= 0;
	local WashitemID	= 0;
	if itemBtn:GetClientUserData( 2 ) == 0 then
		local minNeedLv = 200; 
		local nWashLv	= 0;
		local nWash		= false;
		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item		= container:getItem( CONTAINER_TYPE_SUNDRIES, i );
			local itemId	= item:getItemId();
			if itemId > 0 then
				local ItemDef	= getItemDef( itemId );
				for i=1,MAX_ITEM_RESULT do
					if ItemDef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF then
						nWashLv = ItemDef.Result[i-1].ResultVal1;
						nWash	= true;
						break;
					end
				end
				if nWash and ArmDef.WearPre.RoleLevelMin < nWashLv and nWashLv <= minNeedLv and item:isItemBind() then
					minNeedLv		= nWashLv;
					WashItemGrid	= i;
					WashitemID		= itemId;
					WashItemBind	= 1;
				end
				if nWash and ArmDef.WearPre.RoleLevelMin < nWashLv and nWashLv < minNeedLv then
					minNeedLv		= nWashLv;
					WashItemGrid	= i;
					WashitemID		= itemId;
					WashItemBind	= 0;
				end
			end
			nWash = false;
		end
		if minNeedLv == 200 then
			WashItemGrid	= 0;
			WashitemID		= 0;
		end
	end
	if WashitemID > 0 then
		itemBtn:SetClientUserData( 0 ,CONTAINER_TYPE_SUNDRIES );
		itemBtn:SetClientUserData( 1 ,WashItemGrid );
		itemBtn:SetClientUserData( 2 ,WashitemID );
		itemBtn:SetClientUserData( 3 ,WashItemBind );
	end
end

function Save2ArmWashFrame( item, nSrcList, nSrcGridIndex )
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		ArmWashFrameProcessOnReciveDrag( "ArmWashFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	else
		ArmWashFrameProcessOnReciveDrag( "ArmWashFrameStuffItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	end
	if ArmWashFrame:IsShown() then
		return false;
	end
end

function OnArmWashSucc( washData )
	if ArmWashFrame:IsShown() then
		ArmWashFrameUpdateNewAddAttr( washData, false );
	else
		ArmAdvWashFrameUpdateNewAddAttr( washData, false );
	end
end

function ArmWashFrameGetAddAttrDesc( addAttrInst, itemDef )
	local szDesc,bFull = "",false;
	local addAttrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
	local newAddAttrDef = getNewAddAttrDef( addAttrInst.AddAttrID );
	if addAttrRandDef ~= nil and newAddAttrDef ~= nil then
		local base = newAddAttrDef.Val1[ addAttrRandDef.ValLevel - 1 ];
		local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
		local val = base*addAttrInst.Val/100*mul/100 ;
		local tAddattrVal = { val, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
		
		szDesc = getNewAttrDesc( addAttrInst.AddAttrID, tAddattrVal )
		if szDesc ~= "" then
			szDesc = "● " .. szDesc;
		end
		
		local nowVal = getNewAttrVal( newAddAttrDef.AddAttrDesc, tAddattrVal );
		local fullVal = newAddAttrDef.RangeList[addAttrRandDef.AddMulLevel - 1].MaxnMul;
		tAddattrVal = { base*fullVal/100*mul/100, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 };
		local maxVal = getNewAttrVal( newAddAttrDef.AddAttrDesc, tAddattrVal );
		if nowVal == maxVal then
			bFull = true;
		end
	end
	return szDesc,bFull;
end

-- 显示 装备原关键字属性
function ArmWashFrameUpdateOldAddAttr( arm )
	for i=1,MAX_WASH_ATTR_NUM do
		local tex = getglobal( "ArmWashFrameOldInfoBtn" .. i .. "Tex");
		local rich = getglobal( "ArmWashFrameOldInfoBtn" .. i .. "Rich" );
		tex:Hide();
		rich:SetText("",0,0,0)
	end
	local nIndex = 1;
	if arm ~= nil and arm:getItemId() > 0 then
		local itemDef = arm:getItemDef();
		for i=1,arm:getArmAddAttrNum() do
			if nIndex > MAX_WASH_ATTR_NUM then break end
			local tex = getglobal( "ArmWashFrameOldInfoBtn" .. nIndex .. "Tex" );
			local rich = getglobal( "ArmWashFrameOldInfoBtn" .. nIndex .. "Rich" );
			local szDesc,bFull = ArmWashFrameGetAddAttrDesc( arm:getArmAddAttr( i-1 ), itemDef );
			if szDesc ~= "" then
				rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
				rich:SetText( szDesc,0,180,0 );				
				rich:SetClientUserData( 0,1 )
				rich:ScrollFirst();
				if bFull then
					tex:Show()
					rich:SetWidth( 114 )
				else
					rich:SetWidth( 129 )
				end
				nIndex = nIndex + 1
			end
		end
		for i=1,arm:getArmWashAddAttrNum() do
			if nIndex > MAX_WASH_ATTR_NUM then break end
			local tex = getglobal( "ArmWashFrameOldInfoBtn" .. nIndex .. "Tex" );
			local rich = getglobal( "ArmWashFrameOldInfoBtn" .. nIndex .. "Rich" );
			local szDesc,bFull = ArmWashFrameGetAddAttrDesc( arm:getArmWashAddAttrInst( i-1 ), itemDef );
			if szDesc ~= "" then				
				rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
				rich:SetText( szDesc,0,200,255 );	
				rich:SetClientUserData( 0,1 )
				rich:ScrollFirst();
				if bFull then
					tex:Show()
					rich:SetWidth( 114 )
				else
					rich:SetWidth( 129 )
				end
				nIndex = nIndex + 1
			end
		end
		if arm:getArmWashAddAttrNum() < 4 then
			local rich = getglobal( "ArmWashFrameOldInfoBtn" .. nIndex .. "Rich" );	
			local szDesc = "洗炼有几率增加蓝色属性";
			rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 );
			rich:SetText( szDesc,0,200,255 );				
			rich:SetClientUserData( 0,1 );
			rich:ScrollFirst();
			rich:SetWidth( 129 );
		end
	end
	ArmWashFrameSaveBtn:SetClientUserData( 1,nIndex );
end

-- 显示 洗炼后的 关键字
function ArmWashFrameUpdateNewAddAttr( newAddAttr, bSave )
	ArmWashFrameSaveBtn:Hide();
	ArmWashFrameCancelBtn:Hide();
	ArmWashFrameSaveBkgTex:Hide();
	ArmWashFrameSaveFont:Hide();
	ArmWashFrameArrowTex:Hide();
	for i=1,MAX_WASH_ATTR_NUM do
		local tex = getglobal( "ArmWashFrameNewInfoBtn" .. i .. "Tex" );
		local rich = getglobal( "ArmWashFrameNewInfoBtn" .. i .. "Rich" );
		tex:Hide();
		rich:SetText("",0,0,0)
	end
	local nIndex = 1;
	if newAddAttr == nil then		
		if bSave then
			ArmWashFrameSaveBkgTex:Show();
			ArmWashFrameSaveFont:Show();		
		end
	else		
		local nList = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
		local nGrid  = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer();
		local armItem = container:getItem( nList,nGrid );		
		if armItem:getItemId() > 0 then
			for i=1,newAddAttr.AddAttrNum do
				if nIndex > MAX_WASH_ATTR_NUM then break end
				local tex = getglobal( "ArmWashFrameNewInfoBtn" .. nIndex .. "Tex" );
				local rich = getglobal( "ArmWashFrameNewInfoBtn" .. nIndex .. "Rich" );
				local szDesc,bFull = ArmWashFrameGetAddAttrDesc( newAddAttr.AddAttr[i-1], armItem:getItemDef() );
				if szDesc ~= "" then
					rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
					rich:SetText( szDesc,0,180,0 );
					rich:SetClientUserData( 0,1 )
					rich:ScrollFirst();
					if bFull then
						tex:Show()
						rich:SetWidth( 114 )
					else
						rich:SetWidth( 129 )
					end
					nIndex = nIndex + 1
				end			
			end

			for i=1,newAddAttr.WashAttrNum do
				if nIndex > MAX_WASH_ATTR_NUM then break end
				local tex = getglobal( "ArmWashFrameNewInfoBtn" .. nIndex .. "Tex" );
				local rich = getglobal( "ArmWashFrameNewInfoBtn" .. nIndex .. "Rich" );
				local szDesc,bFull = ArmWashFrameGetAddAttrDesc( newAddAttr.WashAddAttr[i-1], armItem:getItemDef() );
				if szDesc ~= "" then
					rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
					rich:SetText( szDesc,0,200,255 );	
					rich:SetClientUserData( 0,1 )
					rich:ScrollFirst();
					if bFull then
						tex:Show()
						rich:SetWidth( 114 )
					else
						rich:SetWidth( 129 )
					end
					nIndex = nIndex + 1
				end			
			end
			ArmWashFrameSaveBkgTex:Show();
			ArmWashFrameSaveBtn:Show();
			ArmWashFrameCancelBtn:Show();
			ArmWashFrameArrowTex:Show();
		end
	end
	ArmWashFrameSaveBtn:SetClientUserData( 0,nIndex );
end

function ClearArmWashItemInfo()
	-- 清理界面
	ArmWashFrameArmItemBtnBindTexture:Hide();
	ArmWashFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmWashFrameArmItemBtnUVAnimationTex:Hide();
	ArmWashFrameArmItemBtnCountFont:SetText("");
	
	ArmWashFrameStuffItemBtnBindTexture:Hide();
	ArmWashFrameStuffItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmWashFrameStuffItemBtnUVAnimationTex:Hide();
	ArmWashFrameStuffItemBtnCountFont:SetText("");

	ArmWashFrameCostFont:SetText("0")

	ArmWashFrameUpdateOldAddAttr( nil );
end

function UpdateArmWashFrameItemInfo()
	ClearArmWashItemInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local bEnable = true;
	-- 装备
	local armList = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
	local armItem = container:getItem( armList, armGrid );	
	if armItem:getItemId() > 0 then
		local armItemDef = armItem:getItemDef();

		local IconPath = GetItemEquipIconPath()..armItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		ArmWashFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( armItemDef, "ArmWashFrameArmItemBtnUVAnimationTex",armItem );
		if armItem:isItemBind() then
			ArmWashFrameArmItemBtnBindTexture:Show();
		else
			ArmWashFrameArmItemBtnBindTexture:Hide();
		end

		ArmWashFrameUpdateOldAddAttr( armItem )
		
		local nNeedMoney = (math.ceil(armItem:getItemDef().WearPre.RoleLevelMin/10))*5;
		ArmWashFrameCostFont:SetText( nNeedMoney );
	else
		bEnable = false;
	end

	-- 材料
	local stuffList = ArmWashFrameStuffItemBtn:GetClientUserData( 0 );
	local stuffGrid = ArmWashFrameStuffItemBtn:GetClientUserData( 1 );
	local stuffItemId = ArmWashFrameStuffItemBtn:GetClientUserData( 2 );
	local stuffBind = ArmWashFrameStuffItemBtn:GetClientUserData( 3 );
	local stuffItem = container:getItem( stuffList, stuffGrid )
	ClearItemLock( "ArmWashFrameStuffItemBtn" )
	local nNum = 0	
	if stuffBind == 1 then
		nNum = container:getBindItemNum( CONTAINER_TYPE_SUNDRIES, stuffItemId )		
	else
		nNum = container:getUnBindItemNum( CONTAINER_TYPE_SUNDRIES, stuffItemId )
	end
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
		local t = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
		local nBind = t:isItemBind() == true and 1 or 0
		if stuffItemId ~= 0 and t:getItemId() == stuffItemId and nBind == stuffBind then
			LockItem( t:getListType(), t:getGridIdx(), t:getItemId(), "ArmWashFrameStuffItemBtn" )
		end
	end
	if nNum > 0 then
		local stuffItemDef = getItemDef( stuffItemId )
		ArmWashFrameStuffItemBtnCountFont:SetText( nNum )
		
		IconPath = GetItemEquipIconPath()..stuffItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmWashFrameStuffItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( stuffItemDef, "ArmWashFrameStuffItemBtnUVAnimationTex", stuffItem );		
		if stuffBind == 1 then
			ArmWashFrameStuffItemBtnBindTexture:Show()
		else
			ArmWashFrameStuffItemBtnBindTexture:Hide();
		end
	else
		ArmWashFrameStuffItemBtn:SetClientUserData( 2,0 )
		--bEnable = false;
	end
	if stuffItem:getItemId() == 0 and stuffList ~= 0 then
		ArmWashFrameStuffItemBtn:SetClientUserData( 0,0 )
		ArmWashFrameStuffItemBtn:SetClientUserData( 1,0 )
	end

	if bEnable then
		ArmWashFrameOKBtn:Enable();
	else
		ArmWashFrameOKBtn:Disable();
	end
end

function ArmWashFrameCloseBtn_OnClick()
	if ArmWashFrameSaveBtn:IsShown() then
		MessageBox("洗炼","本次洗炼结果未保存，确定关闭洗炼吗？")
		MessageBoxFrame:SetClientString( "保存洗炼" )
	else
		ArmWashFrame:Hide();
	end
end

function ArmWashFrame_OnLoad()
	for i=1,MAX_WASH_ATTR_NUM do
		local tex = getglobal( "ArmWashFrameOldAttrBtnBkg" .. i );
		local tex1 = getglobal( "ArmWashFrameNewAttrBtnBkg" .. i )
		if math.mod( i,2 ) == 0 then
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
			tex1:ChangeTextureTemplate( "WhiteCategoryBkg" );
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" );
			tex1:ChangeTextureTemplate( "BlackCategoryBkg" );
		end
	end
	ItemFirstCallBack( "ArmWashFrame" , Save2ArmWashFrame );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:setUpdateTime( 0 )
end

function ArmWashFrame_OnShow()
	UpdateArmWashFrameItemInfo();
	ArmWashFrameUpdateNewAddAttr( nil )

	--[[local btn = getglobal( "ArmWashFrameAdvBtn" );
	local tipBtn = getglobal( "ArmWashFrameAdvTipBtn" );
	if GeniusMgr:getAllGeniusValue() < ADV_WASH_CAN_USE_LV then
		btn:Disable();
		tipBtn:Show();
	else
		btn:Enable();
		tipBtn:Hide();
	end--]]
end

function ArmWashFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		if ArmWashFrame:IsShown() then
			UpdateArmWashFrameItemInfo();			
		end
	end
end

function ArmWashFrame_OnHide()
	if MessageBoxFrame:IsShown() and ( MessageBoxFrame:GetClientString() == "绑定洗炼" or MessageBoxFrame:GetClientString() == "保存洗炼" or
		MessageBoxFrame:GetClientString() == "放弃洗炼保存" or MessageBoxFrame:GetClientString() == "洗炼替换保存" 
		or MessageBoxFrame:GetClientString() == "洗炼未保存移动" or MessageBoxFrame:GetClientString() == "确定保存洗炼" 
		or MessageTitle:GetText() == "洗炼切换转星" or MessageTitle:GetText() == "洗炼切换绑定" 
		or MessageTitle:GetText() == "洗炼切换重铸" or MessageTitle:GetText() == "洗炼切换强化" ) then
		MessageBoxFrame:Hide();
	end

	local nListType	= ArmWashFrameArmItemBtn:GetClientUserData( 0 ) ;
	local nGrid		= ArmWashFrameArmItemBtn:GetClientUserData( 1 );
	if nListType > 0 then
		UnLockItem( nListType,nGrid,0 );
	end
	ArmWashFrameArmItemBtn:SetClientUserData( 0,0 )
	ArmWashFrameArmItemBtn:SetClientUserData( 1,0 )
	ArmWashFrameArmItemBtn:SetClientUserData( 2,0 )
	ArmWashFrameArmItemBtn:SetClientUserData( 3,0 )

	ClearItemLock( "ArmWashFrameStuffItemBtn" )

	ArmWashFrameStuffItemBtn:SetClientUserData( 0,0 )
	ArmWashFrameStuffItemBtn:SetClientUserData( 1,0 )
	ArmWashFrameStuffItemBtn:SetClientUserData( 2,0 )
	ArmWashFrameStuffItemBtn:SetClientUserData( 3,0 )

	if dragfrom == DRAG_ARM_WASH then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE; 
	end
	ArmWashFrameComfirmBtnCancelBtn_OnClick();
	nLastRequestRecast = 0
end

function ArmWashFrame_OnUpdate()
	for i=1,MAX_WASH_ATTR_NUM do
		local rich = getglobal("ArmWashFrameOldInfoBtn" .. i .. "Rich")
		local width = rich:GetWidth()
		if rich:getLineWidth(0) > width then
			local sign = rich:GetClientUserData( 0 )
			local disx = rich:GetDispPosX() + MOVE_WIDTH_TICK * sign
			rich:SetDispPosX( disx )
			if disx + 114 > 140 + 40 then
				rich:SetClientUserData( 0,-1 )
			end
			if disx <= 0 then
				rich:SetClientUserData( 0,1 )
			end			
		end
		rich = getglobal("ArmWashFrameNewInfoBtn" .. i .. "Rich")
		width = rich:GetWidth()
		if rich:getLineWidth(0) > width then
			local sign = rich:GetClientUserData( 0 )
			local disx = rich:GetDispPosX() + MOVE_WIDTH_TICK * sign
			rich:SetDispPosX( disx )
			if disx + 114 > 140 + 40 then
				rich:SetClientUserData( 0,-1 )
			end
			if disx <= 0 then
				rich:SetClientUserData( 0,1 )
			end			
		end
	end
end

function ArmWashFrameArmItemBtn_OnClick()
	if arg1 == "RightButton" then
		if this:GetName() == "ArmWashFrameArmItemBtn" then
			if ArmWashFrameSaveBtn:IsShown() then
				MessageBox( "", "本次洗炼结果未保存，确定放弃保存吗？" )
				MessageBoxFrame:SetClientString( "放弃洗炼保存" )
			else
				local nListType = this:GetClientUserData( 0 );
				local nGridIdx	= this:GetClientUserData( 1 );
				UnLockItem( nListType, nGridIdx );--解锁
				this:SetClientUserData( 0,0 )
				this:SetClientUserData( 1,0 )
				this:SetClientUserData( 2,0 )
				this:SetClientUserData( 3,0 )
				UpdateArmWashFrameItemInfo();
				ArmWashFrameUpdateNewAddAttr( );
			end
		else
			ClearItemLock( "ArmWashFrameStuffItemBtn" )
			this:SetClientUserData( 0,0 )
			this:SetClientUserData( 1,0 )
			this:SetClientUserData( 2,0 )
			this:SetClientUserData( 3,0 )
			UpdateArmWashFrameItemInfo();
		end
	end
end

function ArmWashFrameArmItemBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local nListType = this:GetClientUserData( 0 )
	local nGridIndex = this:GetClientUserData( 1 )
	local item = container:getItem( nListType,nGridIndex )
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	else
		local itemId = this:GetClientUserData( 2 )
		local nBind = this:GetClientUserData( 3 )
		local stuffItem = nil
		for i=1,container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
			item = container:getItem( CONTAINER_TYPE_SUNDRIES, (i-1) );
			if item:getItemId() == itemId then
				if nBind == 1 and item:isItemBind() then
					stuffItem = item;
					break;
				elseif nBind == 0 and not item:isItemBind() then
					stuffItem = item;
					break;
				end
			end
		end
		if stuffItem ~= nil then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, stuffItem );
		elseif itemId > 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( itemId ) )
		end	
	end
	if item:getItemId() <= 0 then
		if string.find( this:GetName(), "ArmItemBtn", 1, true) then
			SetGameTooltips( this:GetName(), "请放入要洗炼的装备" );
		else
			SetGameTooltips( this:GetName(), "请放入对应装备等级的洗炼石" );
		end
	end
end

function ArmWashFrameArmItemBtn_OnRecive()
	ArmWashFrameProcessOnReciveDrag( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3(), true );
end

function ArmWashFrameArmItemBtn_OnBeginDrag()
	--处理开始拖动
	local nListType	= this:GetClientUserData( 0 );
	local nGrid		= this:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_WASH;
		local IconTex = getglobal(this:GetName().."IconTexture");
		if this:GetName() == "ArmWashFrameArmItemBtn" then
			UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),1 );
		elseif this:GetName() == "ArmWashFrameStuffItemBtn" then
			UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),2 );
		elseif this:GetName() == "ArmAdvWashFrameArmItemBtn" then
			UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),3 );
		elseif this:GetName() == "ArmAdvWashFrameStuffItemBtn" then
			UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),4 );
		end
	end
end

function ArmWashFrameItemBtnClearBtn( szBtnName, desList, desGrid )
	if szBtnName ~= "ArmWashFrameStuffItemBtn" and ArmWashFrameSaveBtn:IsShown() then
		MessageBox( "","本次洗炼结果未保存，确定放弃保存吗？" )
		MessageBoxFrame:SetClientString( "洗炼未保存移动" )
		MessageBoxFrame:SetClientUserData( 0, desList )
		MessageBoxFrame:SetClientUserData( 1, desGrid )
		return
	end
	local btn = getglobal( szBtnName );
	local nListType = btn:GetClientUserData( 0 );
	local nGridIdx	= btn:GetClientUserData( 1 );
	UnLockItem( nListType, nGridIdx );--解锁
	btn:SetClientUserData( 0,0 )
	btn:SetClientUserData( 1,0 )
	btn:SetClientUserData( 2,0 )
	btn:SetClientUserData( 3,0 )
	UpdateArmWashFrameItemInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx )
	if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
		container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
	end
end

function ArmWashFrameOKBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	-- 洗炼
	if TradeFrame:IsShown() then
		ShowMidTips("交易中不可洗炼");
		return;
	end

	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		return;
	end
	if ArmWashFrameSaveBtn:IsShown() and ArmWashFrameSaveBtn:GetClientUserData(0) > ArmWashFrameSaveBtn:GetClientUserData(1) then
		MessageBox("","上次洗炼结果比当前属性多一条属性，点击确定保存");
		MessageBoxFrame:SetClientString("确定保存洗炼");
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local stuffItem = ArmWashFrameStuffItemBtn:GetClientUserData( 2 )
	local nBind = ArmWashFrameStuffItemBtn:GetClientUserData( 3 );
	local armListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
	
	if stuffItem == 0 then
		local item = container:getItem( armListType, armGridIndex );
		MessageBoxForItem("提示","洗炼石");
		MessageBoxForItemFrame:SetClientUserData(0,0);
		MessageBoxForItemFrame:SetClientUserData(1,item:getItemId());
		MessageBoxForItemFrame:SetClientString("洗炼石不足");
		return;
	end
	-- 费用
	local nMoney = getPlayerMoney( mainplayer )
	if nMoney < tonumber( ArmWashFrameCostFont:GetText() ) then
		ShowMidTips("绑银不足，无法洗炼")
		return
	end

	if checkNeedWarnCost( tonumber( ArmWashFrameCostFont:GetText() ) ) then
		CostMoneyConfirmFrame:SetClientString("装备洗炼");
		SetWarnFrame( "ArmWashFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end
	
	local arm = container:getItem( armListType,armGridIndex )
	if os.clock() - nLastRequestRecast > 0.5 then
		nLastRequestRecast = os.clock();
		if getWashItemLockNum( arm ) > 0 then
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
	end
end

function ArmWashFrameComfirmBtnSaveBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		return;
	end
	local armListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
	GameArmProce:saveWashArm( armListType, armGridIndex )
	ArmWashFrameUpdateNewAddAttr( nil, true )
	GameTooltipOnLeave();
end

function ArmWashFrameComfirmBtnSaveBtn_OnEnter()
	SetGameTooltips(this:GetName(),"保存此次洗炼结果")
end

function ArmWashFrameComfirmBtnCancelBtn_OnEnter()	
	SetGameTooltips(this:GetName(),"放弃此次洗炼结果")
end

function ArmWashFrameComfirmBtnCancelBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		return;
	end
	ArmWashFrameUpdateNewAddAttr( );
	GameTooltipOnLeave();
end

function ArmWashFrameSignTexBtn_OnEnter()
	local szText = [[1)	装备洗炼可重置装备附加属性
2)	每次装备洗炼消耗一个与装备等级相应的洗炼石 
3)	装备洗炼可能增加装备附加属性条目
4)	装备自带的绿色属性只改变数值不改变条目
]]
	SetGameTooltips( this:GetName(), szText )
end

----------------------------------------------高级洗炼-------------------------------------------------
local t_ItemWid = {};
local USE_ID_WASH_SAFE = 17;

function ArmWashFrameAdvBtn_OnClick()
	if GeniusMgr:getAllGeniusValue() < ADV_WASH_CAN_USE_LV then
		return ShowMidTips( "需要总天赋值" .. ADV_WASH_CAN_USE_LV .. "点，高级洗练可锁定属性使该属性在洗练时不出现变动" );
	end

	if ArmWashFrameSaveBtn:IsShown() then
		MessageBox("洗炼","本次洗炼结果未保存，确定关闭洗炼吗？")
		MessageBoxFrame:SetClientString( "保存洗炼" )
		MessageBoxFrame:SetClientUserData( 0, 1 );
	else
		ArmWashFrame:Hide();
		ArmAdvWashFrame:Show();
	end
end

function ArmAdvWashFrameCloseBtn_OnClick()
	if ArmAdvWashFrameSaveBtn:IsShown() then
		MessageBox("洗炼","本次洗炼结果未保存，确定关闭洗炼吗？")
		MessageBoxFrame:SetClientString( "保存洗炼" )
	else
		ArmAdvWashFrame:Hide();
	end
end

function ArmAdvWashFrame_OnLoad()
	for i=1,MAX_WASH_ATTR_NUM do
		local tex = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Bkg" );
		local tex1 = getglobal( "ArmAdvWashFrameNewAttrBtn" .. i .. "Bkg" );
		if math.mod( i,2 ) == 0 then
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
			tex1:ChangeTextureTemplate( "WhiteCategoryBkg" );
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" );
			tex1:ChangeTextureTemplate( "BlackCategoryBkg" );
		end
	end

	ArmAdvWashFrameCostTip:SetText( "被锁定的属性使用高级洗炼不会有任何改变，含#c0ad700属性类型#n和#c0ad700增加值", 255, 255, 190 )
	ItemFirstCallBack( "ArmAdvWashFrame" , Save2ArmAdvWashFrame );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:setUpdateTime( 0 );
end

function ArmAdvWashFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		t_ItemWid = {};
	elseif arg1 == "GE_CONTAINER_UPDATE" and this:IsShown() then
		UpdateArmAdvWashFrameItemInfo();
	end
end

function checkNeedRecord( armItem )
	local bRecord = false;
	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			bRecord = true;
			break;
		end
	end
	if not bRecord then
		table.insert( t_ItemWid, { wid = armItem:getItemWId(), lock = 0, num = 0, attr = {}, safe = 0, } );
	end
end

function updateWashItemLock( armItem )
	local value = 0;
	local num = 0;
	local t_attrId = {};
	for i=1, MAX_WASH_ATTR_NUM do
		local btn = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Lock" );
		if btn:IsShown() and btn:IsChecked() then
			value = value + 2^(i-1);
			num = num + 1;
			table.insert( t_attrId, { btn:GetClientUserData(0), btn:GetClientUserData(1) } );
		end
	end

	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			t_ItemWid[i].lock = value;
			t_ItemWid[i].num = num;
			t_ItemWid[i].attr = t_attrId;
		end
	end

	return num;
end

function setWashItemLock( armItem, nType )
	local num = updateWashItemLock( armItem );
	AddGameMiddleTips2( "属性" ..( nType > 0 and "解锁" or "锁定" ) .. "成功，此装备已锁定" .. num .. "条属性" );
end

function removeWashItemLock( armItem )
	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			table.remove( t_ItemWid, i );
		end
	end
end

function getWashItemLockValue( armItem )
	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			return t_ItemWid[i].lock;
		end
	end
	return 0;
end

function getWashItemLockNum( armItem )
	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			return t_ItemWid[i].num;
		end
	end
	return 0;
end

function ArmAdvWashFrameProcessOnReciveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3, bDrag )
	if dragfrom == DRAG_ARM_WASH then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateArmAdvWashFrameItemInfo();
	elseif dragfrom == CONTAINER_TYPE_TASK then
		ShowMidTips( "请放入装备或者洗炼石" )
		return
	elseif dragfrom ~= CONTAINER_TYPE_SUNDRIES then
		ShowMidTips("只有背包内装备才可进行此操作");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local srcList		= dragfrom;						-- 源容器类型
	local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
	local item			= container:getItem( srcList, srcGrid );
	local itemdef = item:getItemDef();
	local btn = getglobal( szBtnName )
	if szBtnName == "ArmAdvWashFrameStuffItemBtn" then		
		if not CheckItemResult( itemdef, RESULT_ARM_WASH_STUFF ) then
			if bDrag then
				ShowMidTips( "请放入洗炼石" )
			else
				ShowMidTips( "请放入装备或者洗炼石" )
			end			
			return
		end

		local nArmListType = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
		local nArmGridIdx  = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
		local armItem = container:getItem( nArmListType,nArmGridIdx )
		if armItem:getItemId() > 0 then
			local nMaxWashLv = 0;
			for i=1,MAX_ITEM_RESULT do
				if itemdef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF then
					nMaxWashLv = itemdef.Result[i-1].ResultVal1;
					break;
				end
			end
			-- 对等级判断
			if armItem:getItemDef().WearPre.RoleLevelMin > nMaxWashLv then
				ShowMidTips( "装备等级与洗炼石不符" )
				return;
			end
		end

		local nList = btn:GetClientUserData( 0 );
		local nGrid  = btn:GetClientUserData( 1 );
		local nItemId = btn:GetClientUserData( 2 );
		
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		btn:SetClientUserData( 2,item:getItemId() )
		if item:isItemBind() then
			btn:SetClientUserData( 3,1 )
		else
			btn:SetClientUserData( 3,0 )
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateArmAdvWashFrameItemInfo();
	else
		if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
			if bDrag then
				ShowMidTips( "请放入装备" )
			else
				ShowMidTips( "请放入装备或者洗炼石" );
			end
			return;
		end

		if itemdef.Important == 0 then
			ShowMidTips( "白色品质不能洗炼" );
			return;
		end

		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
			ShowMidTips( "时装不能洗炼" )
			return
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能洗炼" );
			return;
		end
		if item:isArmFengYin() then
			ShowMidTips("封印装备无法进行此操作");
			return;
		end
		if item:isArmIdentity() then 
			ShowMidTips("此装备还未鉴定，不能进行此操作");
			return;
		end
		local stuffItemId = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 2 )
		if container:getItemCount( stuffItemId ) > 0 then
			local stuffItemDef = getItemDef( stuffItemId )
			local nMaxWashLv = 0;
			for i=1,MAX_ITEM_RESULT do
				if stuffItemDef.Result[i-1].ResultID == RESULT_ARM_WASH_STUFF then
					nMaxWashLv = stuffItemDef.Result[i-1].ResultVal1;
					break;
				end
			end
			-- 对等级判断
			if itemdef.WearPre.RoleLevelMin > nMaxWashLv then
				ShowMidTips( "装备等级与洗炼石不符" )
				return;
			end
		end

		if ArmAdvWashFrameSaveBtn:IsShown() then
			MessageBox( "", "本次洗炼结果未保存，确定放弃保存吗？" )
			MessageBoxFrame:SetClientString( "洗炼替换保存" )
			MessageBoxFrame:SetClientUserData( 0, srcList )
			MessageBoxFrame:SetClientUserData( 1, srcGrid )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		
		local nList = btn:GetClientUserData( 0 );
		local nGrid  = btn:GetClientUserData( 1 );
		local armItem = container:getItem( nList,nList );
		local nItemId	= armItem:getItemId();
		if nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		checkNeedRecord( item );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		ArmWashFrameAutoAddNeedItem( itemdef, "ArmAdvWashFrame" );
		UpdateArmAdvWashFrameItemInfo();
		ArmAdvWashFrameUpdateNewAddAttr( );
	end
end

function Save2ArmAdvWashFrame( item, nSrcList, nSrcGridIndex )
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		ArmAdvWashFrameProcessOnReciveDrag( "ArmAdvWashFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	else
		ArmAdvWashFrameProcessOnReciveDrag( "ArmAdvWashFrameStuffItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	end
end

function ArmAdvWashFrameGetAddAttrDesc( addAttrInst, itemDef, wId, nType )
	local szDesc,bFull,bLock = "",false,false;
	local addAttrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
	local newAddAttrDef = getNewAddAttrDef( addAttrInst.AddAttrID );
	if addAttrRandDef ~= nil and newAddAttrDef ~= nil then
		local base = newAddAttrDef.Val1[ addAttrRandDef.ValLevel - 1 ];
		local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
		local val = base*addAttrInst.Val/100*mul/100 ;
		local tAddattrVal = { val, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
		szDesc = getNewAttrDesc( addAttrInst.AddAttrID, tAddattrVal )

		local nowVal = getNewAttrVal( newAddAttrDef.AddAttrDesc, tAddattrVal );
		local fullVal = newAddAttrDef.RangeList[addAttrRandDef.AddMulLevel - 1].MaxnMul;
		tAddattrVal = { base*fullVal/100*mul/100, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 };
		local maxVal = getNewAttrVal( newAddAttrDef.AddAttrDesc, tAddattrVal );
		if nowVal == maxVal then
			bFull = true;
		--else
			--szDesc = szDesc .. "#c972df9 (" .. maxVal .. ")";
		end

		for i=1, #t_ItemWid do
			if isSameWid( t_ItemWid[i].wid, wId ) then
				for j=1, #t_ItemWid[i].attr do
					if t_ItemWid[i].attr[j][1] == nType and t_ItemWid[i].attr[j][2] == addAttrInst.AddAttrID then
						bLock = true;
						break;
					end
				end
				break;
			end
		end
	end
	return szDesc,bFull,bLock;
end

-- 显示 装备原关键字属性
function ArmAdvWashFrameUpdateOldAddAttr( arm )
	for i=1,MAX_WASH_ATTR_NUM do
		local tex = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Full");
		tex:Hide();
		local btn = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Lock");
		btn:Hide();
		btn = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Add");
		btn:Hide();
		local rich = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Rich" );
		rich:SetText("",0,0,0)
	end
	local nIndex = 1;
	if arm ~= nil and arm:getItemId() > 0 then
		local itemDef = arm:getItemDef();
		for i=1,arm:getArmAddAttrNum() do
			if nIndex > MAX_WASH_ATTR_NUM then break end
			local tex = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Full" );
			local rich = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Rich" );
			local lock = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Lock" );
			local szDesc,bFull,bLock = ArmAdvWashFrameGetAddAttrDesc( arm:getArmAddAttr( i-1 ), itemDef, arm:getItemWId(), 1 );
			if szDesc ~= "" then
				lock:SetClientUserData( 0, 1 );
				lock:SetClientUserData( 1, arm:getArmAddAttr( i-1 ).AddAttrID )
				rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
				rich:SetText( szDesc,0,180,0 );				
				rich:SetClientUserData( 0,1 )
				rich:ScrollFirst();
				if bFull then
					tex:Show()
					rich:SetWidth( 148 )
				else
					rich:SetWidth( 163 )
				end
				lock:Show();
				if bLock then
					lock:Checked();
				else
					lock:DisChecked();
				end
				nIndex = nIndex + 1
			end
		end
		for i=1,arm:getArmWashAddAttrNum() do
			if nIndex > MAX_WASH_ATTR_NUM then break end
			local tex = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Full" );
			local rich = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Rich" );
			local lock = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Lock" );
			local szDesc,bFull,bLock = ArmAdvWashFrameGetAddAttrDesc( arm:getArmWashAddAttrInst( i-1 ), itemDef, arm:getItemWId(), 2 );
			if szDesc ~= "" then
				lock:SetClientUserData( 0, 2 );
				lock:SetClientUserData( 1, arm:getArmWashAddAttrInst( i-1 ).AddAttrID )
				rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
				rich:SetText( szDesc,0,200,255 );	
				rich:SetClientUserData( 0,1 )
				rich:ScrollFirst();
				if bFull then
					tex:Show()
					rich:SetWidth( 148 )
				else
					rich:SetWidth( 163 )
				end
				lock:Show();
				if bLock then
					lock:Checked();
				else
					lock:DisChecked();
				end
				nIndex = nIndex + 1
			end
		end
		if arm:getArmWashAddAttrNum() < 4 then
			local rich = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Rich" );
			local szDesc = "\t\t洗炼有几率增加蓝色属性";
			rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 );
			rich:SetText( szDesc,0,200,255 );
			rich:SetClientUserData( 0,1 );
			rich:ScrollFirst();
			rich:SetWidth( 163 );
			local btn = getglobal( "ArmAdvWashFrameOldAttrBtn" .. nIndex .. "Add" );
			btn:SetClientUserData( 0, arm:getArmWashAddAttrNum() );
			btn:Show();
		end
		updateWashItemLock( arm );
	end
	ArmAdvWashFrameSaveBtn:SetClientUserData( 1,nIndex );
end

-- 显示 洗炼后的 关键字
function ArmAdvWashFrameUpdateNewAddAttr( newAddAttr, bSave )
	ArmAdvWashFrameSaveBtn:Hide();
	ArmAdvWashFrameCancelBtn:Hide();
	ArmAdvWashFrameSaveBkgTex:Hide();
	ArmAdvWashFrameSaveFont:Hide();
	ArmAdvWashFrameArrowTex:Hide();
	for i=1,MAX_WASH_ATTR_NUM do
		local tex = getglobal( "ArmAdvWashFrameNewAttrBtn" .. i .. "Full" );
		tex:Hide();
		local btn = getglobal( "ArmAdvWashFrameNewAttrBtn" .. i .. "Lock");
		btn:Hide();
		btn = getglobal( "ArmAdvWashFrameNewAttrBtn" .. i .. "Add");
		btn:Hide();
		local rich = getglobal( "ArmAdvWashFrameNewAttrBtn" .. i .. "Rich" );
		rich:SetText("",0,0,0)
	end
	local nIndex = 1;
	if newAddAttr == nil then		
		if bSave then
			ArmAdvWashFrameSaveBkgTex:Show();
			ArmAdvWashFrameSaveFont:Show();		
		end
	else		
		local nList = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
		local nGrid  = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer();
		local armItem = container:getItem( nList,nGrid );		
		if armItem:getItemId() > 0 then
			for i=1,newAddAttr.AddAttrNum do
				if nIndex > MAX_WASH_ATTR_NUM then break end
				local tex = getglobal( "ArmAdvWashFrameNewAttrBtn" .. nIndex .. "Full" );
				local rich = getglobal( "ArmAdvWashFrameNewAttrBtn" .. nIndex .. "Rich" );
				local lock = getglobal( "ArmAdvWashFrameNewAttrBtn" .. nIndex .. "Lock" );
				local szDesc,bFull,bLock = ArmAdvWashFrameGetAddAttrDesc( newAddAttr.AddAttr[i-1], armItem:getItemDef(), armItem:getItemWId(), 1 );
				if szDesc ~= "" then
					--lock:SetClientUserData( 0, 1 );
					--lock:SetClientUserData( 1, newAddAttr.AddAttr[i-1].AddAttrID )
					rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
					rich:SetText( szDesc,0,180,0 );
					rich:SetClientUserData( 0,1 )
					rich:ScrollFirst();
					if bFull then
						tex:Show()
						rich:SetWidth( 148 )
					else
						rich:SetWidth( 163 )
					end
					lock:Show();
					if bLock then
						lock:ChangeNormalTexture( 284, 578, 13, 14 );
					else
						lock:ChangeNormalTexture( 732, 944, 13, 14 );
					end
					lock:Disable();
					nIndex = nIndex + 1
				end			
			end

			for i=1,newAddAttr.WashAttrNum do
				if nIndex > MAX_WASH_ATTR_NUM then break end
				local tex = getglobal( "ArmAdvWashFrameNewAttrBtn" .. nIndex .. "Full" );
				local rich = getglobal( "ArmAdvWashFrameNewAttrBtn" .. nIndex .. "Rich" );
				local lock = getglobal( "ArmAdvWashFrameNewAttrBtn" .. nIndex .. "Lock" );
				local szDesc,bFull,bLock = ArmAdvWashFrameGetAddAttrDesc( newAddAttr.WashAddAttr[i-1], armItem:getItemDef(), armItem:getItemWId(), 2 );
				if szDesc ~= "" then
					--lock:SetClientUserData( 0, 2 )
					--lock:SetClientUserData( 1, newAddAttr.WashAddAttr[i-1].AddAttrID )
					rich:resizeRect( rich:GetTextExtentWidth( szDesc ),16 )
					rich:SetText( szDesc,0,200,255 );	
					rich:SetClientUserData( 0,1 )
					rich:ScrollFirst();
					if bFull then
						tex:Show()
						rich:SetWidth( 148 )
					else
						rich:SetWidth( 163 )
					end
					lock:Show();
					if bLock then
						lock:ChangeNormalTexture( 284, 578, 13, 14 );
					else
						lock:ChangeNormalTexture( 732, 944, 13, 14 );
					end
					lock:Disable();
					nIndex = nIndex + 1
				end			
			end
			ArmAdvWashFrameSaveBkgTex:Show();
			ArmAdvWashFrameSaveBtn:Show();
			ArmAdvWashFrameCancelBtn:Show();
			ArmAdvWashFrameArrowTex:Show();
		end
	end
	ArmAdvWashFrameSaveBtn:SetClientUserData( 0,nIndex );
end

function ClearArmAdvWashItemInfo()
	-- 清理界面
	ArmAdvWashFrameArmItemBtnBindTexture:Hide();
	ArmAdvWashFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmAdvWashFrameArmItemBtnUVAnimationTex:Hide();
	ArmAdvWashFrameArmItemBtnCountFont:SetText("");
	
	ArmAdvWashFrameStuffItemBtnBindTexture:Hide();
	ArmAdvWashFrameStuffItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmAdvWashFrameStuffItemBtnUVAnimationTex:Hide();
	ArmAdvWashFrameStuffItemBtnCountFont:SetText("");

	ArmAdvWashFrameCostFont:SetText("0")
	ArmAdvWashFrameCostFont:SetTextColor( 160, 160, 90 );

	ArmAdvWashFrameUpdateOldAddAttr( );
	ArmAdvWashFrameSafe:setCheckState( false );
end

function UpdateArmAdvWashFrameItemInfo()
	ClearArmAdvWashItemInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local bEnable = true;
	-- 装备
	local armList = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	local armItem = container:getItem( armList, armGrid );	
	if armItem:getItemId() > 0 then
		local armItemDef = armItem:getItemDef();

		local IconPath = GetItemEquipIconPath()..armItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		ArmAdvWashFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( armItemDef, "ArmAdvWashFrameArmItemBtnUVAnimationTex",armItem );
		if armItem:isItemBind() then
			ArmAdvWashFrameArmItemBtnBindTexture:Show();
		else
			ArmAdvWashFrameArmItemBtnBindTexture:Hide();
		end

		ArmAdvWashFrameUpdateOldAddAttr( armItem )
		
		local lockNum = getWashItemLockNum( armItem );
		local nNeedMoney = 0;
		local bRed = false;
		if getArmSafe( armItem ) > 0 then
			local useDef = GameArmProce:getUseDef( USE_ID_WASH_SAFE );
			nNeedMoney = useDef.UseJinbi;
			ArmAdvWashFrameSafe:setCheckState( true );
		end
		if nNeedMoney > 0 or lockNum > 0 then
			local highWashDef = GameArmProce:getHighWashDef( lockNum );
			if highWashDef ~= nil then
				nNeedMoney = nNeedMoney + highWashDef.UseVal;
			end
			ArmAdvWashFrameMoneyTexBtnTexture:SetTexUV(99,494,17,18);
			ArmAdvWashFrameMoneyTexBtnTexture:SetSize(17,18);
			ArmAdvWashFrameMoneyTexBtn:SetClientUserData(0,1);
			bRed = mainplayer:getGodCoin() < nNeedMoney and true or false;
		else
			nNeedMoney = (math.ceil(armItem:getItemDef().WearPre.RoleLevelMin/10))*5;
			ArmAdvWashFrameMoneyTexBtnTexture:SetTexUV(78,494,20,18);
			ArmAdvWashFrameMoneyTexBtnTexture:SetSize(20,18);
			ArmAdvWashFrameMoneyTexBtn:SetClientUserData(0,4);
			bRed = getPlayerMoney( mainplayer ) < nNeedMoney and true or false;
		end

		ArmAdvWashFrameCostFont:SetText( nNeedMoney );
		if bRed then
			ArmAdvWashFrameCostFont:SetTextColor( 210, 40, 50 );
		end
	else
		bEnable = false;
	end

	-- 材料
	local stuffList = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 0 );
	local stuffGrid = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 1 );
	local stuffItemId = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 2 );
	local stuffBind = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 3 );
	local stuffItem = container:getItem( stuffList, stuffGrid )
	ClearItemLock( "ArmAdvWashFrameStuffItemBtn" )
	local nNum = 0	
	if stuffBind == 1 then
		nNum = container:getBindItemNum( CONTAINER_TYPE_SUNDRIES, stuffItemId )		
	else
		nNum = container:getUnBindItemNum( CONTAINER_TYPE_SUNDRIES, stuffItemId )
	end
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
		local t = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
		local nBind = t:isItemBind() == true and 1 or 0
		if stuffItemId ~= 0 and t:getItemId() == stuffItemId and nBind == stuffBind then
			LockItem( t:getListType(), t:getGridIdx(), t:getItemId(), "ArmAdvWashFrameStuffItemBtn" )
		end
	end
	if nNum > 0 then
		local stuffItemDef = getItemDef( stuffItemId )
		ArmAdvWashFrameStuffItemBtnCountFont:SetText( nNum )
		
		IconPath = GetItemEquipIconPath()..stuffItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmAdvWashFrameStuffItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( stuffItemDef, "ArmAdvWashFrameStuffItemBtnUVAnimationTex", stuffItem );		
		if stuffBind == 1 then
			ArmAdvWashFrameStuffItemBtnBindTexture:Show()
		else
			ArmAdvWashFrameStuffItemBtnBindTexture:Hide();
		end
	else
		ArmAdvWashFrameStuffItemBtn:SetClientUserData( 2,0 )
		--bEnable = false;
	end
	if stuffItem:getItemId() == 0 and stuffList ~= 0 then
		ArmAdvWashFrameStuffItemBtn:SetClientUserData( 0,0 )
		ArmAdvWashFrameStuffItemBtn:SetClientUserData( 1,0 )
	end

	if bEnable then
		ArmAdvWashFrameOKBtn:Enable();
	else
		ArmAdvWashFrameOKBtn:Disable();
	end
end

function ArmAdvWashFrameAttrLockBtn_OnEnter()
	if this:IsChecked() then
		SetGameTooltips(this:GetName(),"点击解锁属性，解锁后洗炼可改变此属性");
	else
		SetGameTooltips(this:GetName(),"点击锁定属性，锁定后的属性洗炼时不会有任何改变");
	end
end

function ArmAdvWashFrameAttrLockBtn_OnClick()
	if ArmAdvWashFrameSaveBtn:IsShown() then
		MessageBox( "", "本次洗炼结果还未保存，无法进行属性锁定操作" )
		MessageBoxFrame:SetClientString( "未保存锁定洗炼" )
		return;
	end

	if LockAttrConfirmFrame:IsShown() then
		return;
	end

	if string.find( this:GetName(), "NewAttrBtn", 1, true) then
		return ShowMidTips( "请点击现有属性进行锁定和解锁操作" );
	end

	if not this:IsChecked() then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_WARN ) then
			LockAttrConfirmFrame:Show();
			LockAttrConfirmFrame:SetClientString( this:GetName() );
			LockAttrConfirmFrameMessageFont:SetText( "锁定属性锁后每次洗练需要额外消耗#cffff00金币#n，确定需要锁定属性？", 255, 229, 189 );
			return;
		end
	end
	
	local armList = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local armItem = container:getItem( armList, armGrid );	
	if armItem:getItemId() > 0 then
		if this:IsChecked() then
			this:DisChecked();
			setWashItemLock( armItem, 1 );
		else
			this:Checked();
			setWashItemLock( armItem, 0 );
		end
		UpdateArmAdvWashFrameItemInfo();
	end
end


function ArmAdvWashFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
		this:SetPoint( "topright","PlayerContainerFrame","topleft", 0, 0 )
	else
		this:SetPoint( "topright","ArmWashFrame","topright", 0, 0 )
	end
	UpdateArmAdvWashFrameItemInfo();
	ArmAdvWashFrameUpdateNewAddAttr( );
end

function ArmAdvWashFrame_OnHide()
	if MessageBoxFrame:IsShown() and ( MessageBoxFrame:GetClientString() == "绑定洗炼" or MessageBoxFrame:GetClientString() == "保存洗炼" or
		MessageBoxFrame:GetClientString() == "放弃洗炼保存" or MessageBoxFrame:GetClientString() == "洗炼替换保存" 
		or MessageBoxFrame:GetClientString() == "洗炼未保存移动" or MessageBoxFrame:GetClientString() == "确定保存洗炼"
		or MessageBoxFrame:GetClientString() == "锁定洗炼" or MessageBoxFrame:GetClientString() == "激活蓝色属性" ) then
		MessageBoxFrame:Hide();
	end
	LockAttrConfirmFrame:Hide();

	local nListType	= ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 ) ;
	local nGrid		= ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	if nListType > 0 then
		UnLockItem( nListType,nGrid,0 );
	end
	ArmAdvWashFrameArmItemBtn:SetClientUserData( 0,0 )
	ArmAdvWashFrameArmItemBtn:SetClientUserData( 1,0 )
	ArmAdvWashFrameArmItemBtn:SetClientUserData( 2,0 )
	ArmAdvWashFrameArmItemBtn:SetClientUserData( 3,0 )

	ClearItemLock( "ArmAdvWashFrameStuffItemBtn" )

	ArmAdvWashFrameStuffItemBtn:SetClientUserData( 0,0 )
	ArmAdvWashFrameStuffItemBtn:SetClientUserData( 1,0 )
	ArmAdvWashFrameStuffItemBtn:SetClientUserData( 2,0 )
	ArmAdvWashFrameStuffItemBtn:SetClientUserData( 3,0 )

	if dragfrom == DRAG_ARM_WASH then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE; 
	end
	ArmAdvWashFrameComfirmBtnCancelBtn_OnClick();
	nLastRequestRecast = 0
end

function ArmAdvWashFrame_OnUpdate()
	for i=1,MAX_WASH_ATTR_NUM do
		local rich = getglobal("ArmAdvWashFrameOldAttrBtn" .. i .. "Rich")
		local width = rich:GetWidth()
		if rich:getLineWidth(0) > width then
			local sign = rich:GetClientUserData( 0 )
			local disx = rich:GetDispPosX() + MOVE_WIDTH_TICK * sign
			rich:SetDispPosX( disx )
			if disx + 148 > 174 + 40 then
				rich:SetClientUserData( 0,-1 )
			end
			if disx <= 0 then
				rich:SetClientUserData( 0,1 )
			end			
		end
		rich = getglobal("ArmAdvWashFrameNewAttrBtn" .. i .. "Rich")
		width = rich:GetWidth()
		if rich:getLineWidth(0) > width then
			local sign = rich:GetClientUserData( 0 )
			local disx = rich:GetDispPosX() + MOVE_WIDTH_TICK * sign
			rich:SetDispPosX( disx )
			if disx + 148 > 174 + 40 then
				rich:SetClientUserData( 0,-1 )
			end
			if disx <= 0 then
				rich:SetClientUserData( 0,1 )
			end			
		end
	end
end

function ArmAdvWashFrameArmItemBtn_OnClick()
	if arg1 == "RightButton" then
		if this:GetName() == "ArmAdvWashFrameArmItemBtn" then
			if ArmAdvWashFrameSaveBtn:IsShown() then
				MessageBox( "", "本次洗炼结果未保存，确定放弃保存吗？" )
				MessageBoxFrame:SetClientString( "放弃洗炼保存" )
			else
				local nListType = this:GetClientUserData( 0 );
				local nGridIdx	= this:GetClientUserData( 1 );
				UnLockItem( nListType, nGridIdx );--解锁
				this:SetClientUserData( 0,0 )
				this:SetClientUserData( 1,0 )
				this:SetClientUserData( 2,0 )
				this:SetClientUserData( 3,0 )
				UpdateArmAdvWashFrameItemInfo();
				ArmAdvWashFrameUpdateNewAddAttr( );
			end
		else
			ClearItemLock( "ArmAdvWashFrameStuffItemBtn" )
			this:SetClientUserData( 0,0 )
			this:SetClientUserData( 1,0 )
			this:SetClientUserData( 2,0 )
			this:SetClientUserData( 3,0 )
			UpdateArmAdvWashFrameItemInfo();
		end
	end
end

function ArmAdvWashFrameArmItemBtn_OnRecive()
	ArmAdvWashFrameProcessOnReciveDrag( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3(), true );
end

function ArmAdvWashFrameItemBtnClearBtn( szBtnName, desList, desGrid )
	if szBtnName ~= "ArmAdvWashFrameStuffItemBtn" and ArmAdvWashFrameSaveBtn:IsShown() then
		MessageBox( "","本次洗炼结果未保存，确定放弃保存吗？" )
		MessageBoxFrame:SetClientString( "洗炼未保存移动" )
		MessageBoxFrame:SetClientUserData( 0, desList )
		MessageBoxFrame:SetClientUserData( 1, desGrid )
		return
	end

	local btn = getglobal( szBtnName );
	local nListType = btn:GetClientUserData( 0 );
	local nGridIdx	= btn:GetClientUserData( 1 );
	UnLockItem( nListType, nGridIdx );--解锁
	btn:SetClientUserData( 0,0 )
	btn:SetClientUserData( 1,0 )
	btn:SetClientUserData( 2,0 )
	btn:SetClientUserData( 3,0 )
	UpdateArmAdvWashFrameItemInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx )
	if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
		container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
	end
end

function ArmAdvWashFrameOKBtn_OnClick()
	if LockAttrConfirmFrame:IsShown() then
		return;
	end
	-- 洗炼
	if TradeFrame:IsShown() then
		ShowMidTips("交易中不可洗炼");
		return;
	end

	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		return;
	end

	if ArmAdvWashFrameSaveBtn:IsShown() and ArmAdvWashFrameSaveBtn:GetClientUserData(0) > ArmAdvWashFrameSaveBtn:GetClientUserData(1) then
		MessageBox("","上次洗炼结果比当前属性多一条属性，点击确定保存");
		MessageBoxFrame:SetClientString("确定保存洗炼");
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	
	local stuffItem = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 2 )
	local nBind = ArmAdvWashFrameStuffItemBtn:GetClientUserData( 3 );
	local armListType = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	local container = mainplayer:getContainer();
	local arm = container:getItem( armListType,armGridIndex );
	if stuffItem == 0 then
		MessageBoxForItem("提示","洗炼石");
		MessageBoxForItemFrame:SetClientUserData(0,0);
		MessageBoxForItemFrame:SetClientUserData(1,arm:getItemId());
		MessageBoxForItemFrame:SetClientString("洗炼石不足");
		return;
	end

	-- 费用
	local lockNum = getWashItemLockNum( arm );
	if lockNum + 1 >= ArmAdvWashFrameSaveBtn:GetClientUserData(1) then
		return ShowMidTips("该装备附加属性已全部锁定，无法洗炼");
	end

	if lockNum > 0 or getArmSafe(arm) > 0 then
		if mainplayer:getGodCoin() < tonumber( ArmAdvWashFrameCostFont:GetText() ) then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
			return;
		end
	else
		local nMoney = getPlayerMoney( mainplayer )
		if nMoney < tonumber( ArmAdvWashFrameCostFont:GetText() ) then
			return ShowMidTips("绑银不足，无法洗炼");
		end

		if checkNeedWarnCost( tonumber( ArmAdvWashFrameCostFont:GetText() ) ) then
			CostMoneyConfirmFrame:SetClientString("装备洗炼");
			SetWarnFrame( "ArmAdvWashFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
	end

	if os.clock() - nLastRequestRecast > 0.5 then
		nLastRequestRecast = os.clock();
		if not arm:isItemBind() and nBind == 1 then
			MessageBox("绑定提示","继续洗炼则当前装备会被绑定，是否继续洗炼？");
			MessageBoxFrame:SetClientString("绑定洗炼");
			MessageBoxFrame:SetClientUserData( 0, stuffItem )
			MessageBoxFrame:SetClientUserData( 1, armListType )
			MessageBoxFrame:SetClientUserData( 2, armGridIndex )
			MessageBoxFrame:SetClientUserData( 3, nBind )
		else
			GameArmProce:requestArmWash( stuffItem, armListType, armGridIndex, nBind, getWashItemLockValue(arm), getArmSafe(arm), 0 )
		end
	end
end

function ArmAdvWashFrameComfirmBtnSaveBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		return;
	end
	local armListType = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	GameArmProce:saveWashArm( armListType, armGridIndex )
	ArmAdvWashFrameUpdateNewAddAttr( nil, true )
	GameTooltipOnLeave();
end

function ArmAdvWashFrameComfirmBtnCancelBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		return;
	end
	ArmAdvWashFrameUpdateNewAddAttr( );
	GameTooltipOnLeave();
end

function LockAttrConfirmFrame_OnShow()
	LockAttrConfirmFrameHideBtn:setCheckState( false );
	ArmAdvWashFrameSafe:Disable();
	LockAttrConfirmFrameMessageFont:setCenterLine( true );
	util.ShowInCenter(this);
end

function LockAttrConfirmFrame_OnHide()
	ArmAdvWashFrameSafe:Enable();
end

function LockAttrConfirmFrameConfirmBtn_OnClick()
	local btnName = LockAttrConfirmFrame:GetClientString();
	local btn = getglobal( btnName );
	local armList = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local armItem = container:getItem( armList, armGrid );

	if string.find( btnName, "Lock", 1, true ) then
		if LockAttrConfirmFrameHideBtn:GetCheckState() then
			GameWizard:doStep( GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_WARN );
		end
		HideUIFrame(this:GetParent());

		if armItem:getItemId() > 0 then
			btn:Checked();
			setWashItemLock( armItem, 0 );
		end
	else
		if LockAttrConfirmFrameHideBtn:GetCheckState() then
			GameWizard:doStep( GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_SAFE );
		end
		HideUIFrame(this:GetParent());
		if armItem:getItemId() > 0 then
			btn:setCheckState( true );
			setArmSafe( armItem, 1 );
		end
	end
	UpdateArmAdvWashFrameItemInfo();
end

function LockAttrConfirmFrameCancelBtn_OnClick()
	HideUIFrame(this:GetParent());
	ArmAdvWashFrameSafe:setCheckState( false );
end

function ArmAdvWashFrameAttrAddBtn_OnClick()
	if LockAttrConfirmFrame:IsShown() then
		return;
	end
	local t_Num = { [1] = 18, [2] = 19, [3] = 20, [4] = 21 };
	local num = this:GetClientUserData(0);
	local useDef = GameArmProce:getUseDef( t_Num[num+1] );
	if useDef == nil then return end;
	local szText = string.format( "直接激活此条蓝色属性需要消耗#cffff00%d金币#n，\n是否确认激活？", useDef.UseJinbi );
	MessageBox("激活蓝色属性",szText);
	MessageBoxFrame:SetClientString("激活蓝色属性");
	MessageBoxFrame:SetClientUserData( 0, num );
	MessageBoxFrame:SetClientUserData( 1, useDef.UseJinbi );
end

function setArmSafe( armItem, value )
	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			t_ItemWid[i].safe = value;
		end
	end
end

function getArmSafe( armItem )
	for i=1, #t_ItemWid do
		if isSameWid( t_ItemWid[i].wid, armItem:getItemWId() ) then
			return t_ItemWid[i].safe;
		end
	end
	return 0;
end

function ArmAdvWashFrameSafe_OnClick()
	if not ArmAdvWashFrameOldAttrBtn1Lock:IsShown() then
		this:setCheckState( false );
		return ShowMidTips("请先放入装备");
	end
	local armList = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local armItem = container:getItem( armList, armGrid );
	if armItem:getArmWashAddAttrNum() == 0 then
		this:setCheckState( false );
		return ShowMidTips("此装备附加属性没有蓝色属性，无需进行洗炼保护");
	end
	
	if this:GetCheckState() then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_SHOW_LOCK_ATTR_SAFE ) then
			LockAttrConfirmFrame:Show();
			LockAttrConfirmFrame:SetClientString( this:GetName() );
			local szText = "保护蓝色属性在洗炼时条数不减少，需要额外消耗\n#cffff00" .. GameArmProce:getUseDef( USE_ID_WASH_SAFE ).UseJinbi .. "金币#n，是否确认保护？"
			LockAttrConfirmFrameMessageFont:SetText( szText, 255, 229, 189 );
			return;
		end
		setArmSafe( armItem, 1 );
	else
		setArmSafe( armItem, 0 );
	end
	UpdateArmAdvWashFrameItemInfo();
end

function ArmAdvWashFrameComfirmBtnSaveBtn_OnShow()
	for i=1,MAX_WASH_ATTR_NUM do
		local btn = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Add");
		btn:Disable();
	end
end

function ArmAdvWashFrameComfirmBtnSaveBtn_OnHide()
	for i=1,MAX_WASH_ATTR_NUM do
		local btn = getglobal( "ArmAdvWashFrameOldAttrBtn" .. i .. "Add");
		btn:Enable();
	end
end