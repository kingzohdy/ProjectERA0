local MAX_ARM_RESET_NUM = 12
local RESET_COST_MONEY = getArmResetMoney()

local selectIdx = 0
local t_LastData = {}

local function UpdateChgTex()
	for i=1,MAX_ARM_RESET_NUM do
		local chg = getglobal( "ArmAttrResetFrameAttrBtn" .. i .. "Chg" )
		chg:Hide()
	end
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( ArmAttrResetFrameArmBtn:GetClientUserData(0), ArmAttrResetFrameArmBtn:GetClientUserData(1) )
	if item:getItemId() == 0 then return end
	for i=1,MAX_ARM_RESET_NUM do
		local btn = getglobal( "ArmAttrResetFrameAttrBtn" .. i )
		local chg = getglobal( btn:GetName() .. "Chg" )
		local rich = getglobal( btn:GetName() .. "Rich" )
		if btn:GetClientUserData(0) == 0 then
			if t_LastData.Idx ~= nil and t_LastData.newVal ~= nil and t_LastData.Idx == i and isSameWid( item:getItemWId(), t_LastData.WID ) then
				if t_LastData.newVal < t_LastData.val then
					chg:SetTexUV( 639,622,11,12 )
					chg:SetSize( 11,12 );
				elseif t_LastData.newVal == t_LastData.val then
					chg:SetTexUV( 728,741,13,4 )
					chg:SetSize( 13,4 );
				else
					chg:SetTexUV( 651,622,11,12 )
					chg:SetSize( 11,12 );
				end
				chg:Show()
				rich:SetWidth( 240 );
			end
		end
	end
end

local function UpdateArmAttr( item )
	for i=1,MAX_ARM_RESET_NUM do
		local btn = getglobal( "ArmAttrResetFrameAttrBtn" .. i )
		btn:Hide()
	end
	local num = 1
	if item ~= nil then
		for i = 1, item:getArmAddAttrNum() do
			local btn		= getglobal( "ArmAttrResetFrameAttrBtn" .. num )
			local rich		= getglobal( btn:GetName() .. "Rich" )
			local fullTex	= getglobal( btn:GetName() .. "Full" )
			local chgTex	= getglobal( btn:GetName() .. "Chg" )

			local szDesc,bFull = "",false;
			local itemDef = item:getItemDef()
			local addAttrInst = item:getArmAddAttr( i-1 )
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
				else
					szDesc = szDesc .. "#c972df9 (" .. maxVal .. ")";
				end
			end

			if szDesc ~= nil then
				szDesc = "☆ " .. szDesc;
				rich:resizeRect( rich:GetTextExtentWidth( szDesc ), 16 );
				rich:SetText( szDesc, 0, 180, 0 );
				rich:ScrollFirst();
				rich:SetWidth( 252 );
				rich:SetClientUserData( 0, 1 );
			end

			if bFull then
				fullTex:Show()
				chgTex:Hide()
				rich:SetWidth( 240 );
				btn:SetClientUserData( 0, 1 )
			else
				fullTex:Hide()
				chgTex:Hide()
				btn:SetClientUserData( 0, 0 )				
			end
			btn:Show()
			num = num + 1
		end

		for i = 1, item:getArmWashAddAttrNum() do
			local btn		= getglobal( "ArmAttrResetFrameAttrBtn" .. num )
			local rich		= getglobal( btn:GetName() .. "Rich" )
			local fullTex	= getglobal( btn:GetName() .. "Full" )
			local chgTex	= getglobal( btn:GetName() .. "Chg" )

			local szDesc,bFull = "",false;
			local itemDef = item:getItemDef()
			local addAttrInst = item:getArmWashAddAttrInst( i-1 )
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
				else
					szDesc = szDesc .. "#c972df9 (" .. maxVal .. ")";
				end
			end

			if szDesc ~= nil then
				szDesc = "☆ " .. szDesc;
				rich:resizeRect( rich:GetTextExtentWidth( szDesc ), 16 );
				rich:SetText( szDesc, 0, 200, 255 );
				rich:ScrollFirst();
				rich:SetWidth( 252 );
				rich:SetClientUserData( 0, 1 );
			end

			if bFull then
				fullTex:Show()
				chgTex:Hide()
				rich:SetWidth( 240 );
				btn:SetClientUserData( 0, 1 )
			else
				fullTex:Hide()
				chgTex:Hide()
				btn:SetClientUserData( 0, 0 )
			end
			btn:Show()
			num = num + 1
		end
		UpdateChgTex()
	end
	
	DisCheckAllBtn( "ArmAttrResetFrameAttrBtn", MAX_ARM_RESET_NUM )
	if selectIdx > 0 then
		local btn = getglobal( "ArmAttrResetFrameAttrBtn" .. selectIdx )
		btn:Checked()
	end
end

local function AutoAddStuffItem()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	if ArmAttrResetFrameStuffBtn:GetClientUserData(2) > 0 then
		return
	end
	local arm = ArmAttrResetFrameArmBtn:GetClientUserData(2)
	if arm > 0 then
		local grid	= 0
		local itemid = 0
		local minlv = 200
		local bind = 0
		local ArmDef = getItemDef( arm )

		local def = getItemDef( arm )
		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
			local val1
			if item:getItemId() > 0 and item:getItemType() == ITEM_SLOTTYPE_ITEM then
				for i=1, MAX_ITEM_RESULT do
					if item:getItemDef().Result[i-1].ResultID == RESULT_ARM_RECAST then
						val1 = item:getItemDef().Result[i-1].ResultVal1
						break
					end
				end
				if val1 ~= nil and ArmDef.WearPre.RoleLevelMin < val1 and val1 <= minlv then
					minlv = val1
					grid = i
					itemid	= item:getItemId()
				end
				if item:isItemBind() then
					bind = 1
				else
					bind = 0
				end
			end
		end
		if itemid > 0 then
			for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
				local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
				local nBind = item:isItemBind() and 1 or 0
				if item:getItemId() == itemid and nBind == bind then
					LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ArmAttrResetFrameStuffBtn" )
				end
			end

			ArmAttrResetFrameStuffBtn:SetClientUserData( 0, CONTAINER_TYPE_SUNDRIES )
			ArmAttrResetFrameStuffBtn:SetClientUserData( 1, grid )
			ArmAttrResetFrameStuffBtn:SetClientUserData( 2, itemid )
			ArmAttrResetFrameStuffBtn:SetClientUserData( 3, bind )
		end
	end
end

local function ClearArmItem()
	local nList = ArmAttrResetFrameArmBtn:GetClientUserData(0)
	local nGrid = ArmAttrResetFrameArmBtn:GetClientUserData(1)
	local nItemId = ArmAttrResetFrameArmBtn:GetClientUserData(2)
	if nList > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end
	ArmAttrResetFrameArmBtn:SetClientUserData( 0, 0 )
	ArmAttrResetFrameArmBtn:SetClientUserData( 1, 0 )
	ArmAttrResetFrameArmBtn:SetClientUserData( 2, 0 )
	ArmAttrResetFrameArmBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	ArmAttrResetFrameArmBtnUVAnimationTex:Hide()
	ArmAttrResetFrameArmBtnBindTexture:Hide()
	ArmAttrResetFrameArmBtnBackTexture:Show()
	t_LastData = {}
	UpdateArmAttr( nil )
end

local function UpdateArmItem()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( ArmAttrResetFrameArmBtn:GetClientUserData(0), ArmAttrResetFrameArmBtn:GetClientUserData(1) )
	if item:getItemId() > 0 and item:getItemId() == ArmAttrResetFrameArmBtn:GetClientUserData(2) then		
		local itemdef = item:getItemDef()
		local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		ArmAttrResetFrameArmBtnIconTexture:SetTexture( IconPath )
		SetEquipImportantEffectUV( itemdef, "ArmAttrResetFrameArmBtnUVAnimationTex",item )
		if item:isItemBind() then
			ArmAttrResetFrameArmBtnBindTexture:Show()
		else
			ArmAttrResetFrameArmBtnBindTexture:Hide()
		end
		ArmAttrResetFrameArmBtnBackTexture:Show()
		UpdateArmAttr( item )
	else
		ClearArmItem()
	end
end

local function GetStuffItem()
	local ret = nil
	if ArmAttrResetFrameStuffBtn:GetClientUserData( 2 ) > 0 then
		local bind = ArmAttrResetFrameStuffBtn:GetClientUserData( 3 )
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local item = container:getItem( ArmAttrResetFrameStuffBtn:GetClientUserData(0), ArmAttrResetFrameStuffBtn:GetClientUserData(1) )
		if item:getItemId() ~= ArmAttrResetFrameStuffBtn:GetClientUserData(2) then
			for i=0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
				item = container:getItem( CONTAINER_TYPE_SUNDRIES, i-1 )
				if item:getItemId() == ArmAttrResetFrameStuffBtn:GetClientUserData(2) then
					if  item:isItemBind() and ArmAttrResetFrameStuffBtn:GetClientUserData(3) > 0 then
						ret = item
						break
					elseif not item:isItemBind() and ArmAttrResetFrameStuffBtn:GetClientUserData(3) == 0 then
						ret = item
						break
					end
				end
			end
		else
			ret = item
		end
	end
	return ret
end

local function ClearStuffItem()
	ArmAttrResetFrameStuffBtn:SetClientUserData( 0, 0 )
	ArmAttrResetFrameStuffBtn:SetClientUserData( 1, 0 )
	ArmAttrResetFrameStuffBtn:SetClientUserData( 2, 0 )
	ArmAttrResetFrameStuffBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	ArmAttrResetFrameStuffBtnUVAnimationTex:Hide()
	ArmAttrResetFrameStuffBtnBindTexture:Hide()
	ArmAttrResetFrameStuffBtnBackTexture:Show()
	ArmAttrResetFrameStuffBtnCountFont:SetText( "" )
	ClearItemLock( "ArmAttrResetFrameStuffBtn" )
end

local function UpdateStuffItem()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local itemId = ArmAttrResetFrameStuffBtn:GetClientUserData(2)
	local bind = ArmAttrResetFrameStuffBtn:GetClientUserData(3)
	local item = container:getItem( ArmAttrResetFrameStuffBtn:GetClientUserData(0),ArmAttrResetFrameStuffBtn:GetClientUserData(1) )
	local num = 0
	if bind > 0 then
		num = container:getBindItemNumNotCheckLock( CONTAINER_TYPE_SUNDRIES, itemId )
	else
		num = container:getUnBindItemNumNotCheckLock( CONTAINER_TYPE_SUNDRIES, itemId )
	end

	if num > 0 then
		local itemdef = getItemDef(itemId )
		local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		ArmAttrResetFrameStuffBtnIconTexture:SetTexture( IconPath )
		SetEquipImportantEffectUV( itemdef, "ArmAttrResetFrameStuffBtnUVAnimationTex", item )
		if bind > 0 then
			ArmAttrResetFrameStuffBtnBindTexture:Show()
		else
			ArmAttrResetFrameStuffBtnBindTexture:Hide()
		end
		ArmAttrResetFrameStuffBtnBackTexture:Show()
		ArmAttrResetFrameStuffBtnCountFont:SetText( num )
	else
		ClearStuffItem()
	end
end

local function ClearItem()
	ClearArmItem()
	ClearStuffItem()
end

local function CheckMoney()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getMoney() < RESET_COST_MONEY then
		ArmAttrResetFrameCost:SetTextColor( 255, 0, 0 )
	else
		ArmAttrResetFrameCost:SetTextColor( 160, 160, 90 )
	end
	ArmAttrResetFrameCost:SetText( RESET_COST_MONEY )
end

local function UpdateItem()
	UpdateArmItem()
	--AutoAddStuffItem()
	UpdateStuffItem()
	CheckMoney()
end

local function Save2ArmArmAttrResetFrame( item )
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		local use = ArmAttrResetFrameStuffBtn:GetClientUserData(2)
		local val1 
			-- 装备未鉴定
		if item:isArmIdentity() then 
			ShowMidTips("此装备还未鉴定，不能进行此操作");
			return;
		end
		if use > 0 then
			local useItemDef = getItemDef( use )
			for i=1,MAX_ITEM_RESULT do
				if useItemDef.Result[i-1].ResultID == RESULT_ARM_RECAST then
					val1 = useItemDef.Result[i-1].ResultVal1
				end	
			end
		end
		if val1 ~= nil and val1 < item:getItemDef().WearPre.RoleLevelMin then
			ShowMidTips( "装备等级与重铸石不符" )
			return
		end
		
		local btn = ArmAttrResetFrameArmBtn
		local list = btn:GetClientUserData(0)
		local grid = btn:GetClientUserData(1)
		if list > 0 then
			UnLockItem( list,grid,0 )
		end
		btn:SetClientUserData( 0, item:getListType() )
		btn:SetClientUserData( 1, item:getGridIdx() )
		btn:SetClientUserData( 2, item:getItemId() )
		LockItem( item:getListType(), item:getGridIdx() , "ArmAttrResetFrame" )
		selectIdx = 0
		AutoAddStuffItem()
		UpdateItem()
	elseif item:getItemType() == ITEM_SLOTTYPE_ITEM then
		local itemDef = item:getItemDef()
		local val1 
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_ARM_RECAST then
				val1 = itemDef.Result[i-1].ResultVal1
			end	
		end
		if val1 == nil then
			ShowMidTips( "请放入重铸石" )
			return
		end
		local btn = ArmAttrResetFrameArmBtn
		if btn:GetClientUserData(2) > 0 then
			local armItemDef = getItemDef( btn:GetClientUserData(2) )
			if val1 < armItemDef.WearPre.RoleLevelMin then
				ShowMidTips( "装备等级与重铸石不符" )
				return
			end
		end
		ClearItemLock( "ArmAttrResetFrameStuffBtn" )
		btn = ArmAttrResetFrameStuffBtn
		btn:SetClientUserData( 0, item:getListType() )
		btn:SetClientUserData( 1, item:getGridIdx() )
		btn:SetClientUserData( 2, item:getItemId() )
		if item:isItemBind() then
			btn:SetClientUserData( 3, 1 )
		else
			btn:SetClientUserData( 3, 0 )
		end
		
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		for i=0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local stuff = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
			if stuff:getItemId() == item:getItemId() and stuff:isItemBind() == item:isItemBind() then
				LockItem( stuff:getListType(), stuff:getGridIdx(), stuff:getItemId(), "ArmAttrResetFrameStuffBtn" )
			end
		end
		UpdateItem()
	end
end

function ClearArmAttrResetBtn( dragdata1, dragdata2, dragdata3, desList, desGrid )
	local btn
	if dragdata3 == 0 then
		btn = ArmAttrResetFrameArmBtn
	else
		btn = ArmAttrResetFrameStuffBtn
		ClearItemLock( "ArmAttrResetFrameStuffBtn" )
	end

	local nListType = btn:GetClientUserData( 0 )
	local nGridIdx	= btn:GetClientUserData( 1 )
	local itemid	= btn:GetClientUserData( 2 )
	if itemid > 0 then
		UnLockItem( nListType, nGridIdx );--解锁
		btn:SetClientUserData( 0,0 )
		btn:SetClientUserData( 1,0 )
		btn:SetClientUserData( 2,0 )
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local item = container:getItem( nListType, nGridIdx )
		if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then			
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE
	end
end

function ArmAttrResetFrame_OnLoad()
	for i=1,MAX_ARM_RESET_NUM do
		local tex = getglobal( "ArmAttrResetFrameBkg" .. i )
		if i%2 == 0 then
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" )
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" )
		end
	end
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ItemFirstCallBack( "ArmAttrResetFrame" , Save2ArmArmAttrResetFrame );
	this:setUpdateTime( 0 )
end

function ArmAttrResetFrame_OnEvent()
	if this:IsShown() then
		UpdateItem()
	end
end

function ArmAttrResetFrame_OnShow()
	selectIdx = 0
	UpdateItem()
end

function ArmAttrResetFrame_OnHide()
	selectIdx = 0
	ClearItem()
end

function ArmAttrResetFrameArmBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_ATTR_RESET;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), 0 );
	end
end

function ArmAttrResetFrameArmBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();

	if dragfrom == DRAG_ARM_ATTR_RESET then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_ITEM then
			ShowMidTips( "请放入要重铸的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end

		local itemDef = item:getItemDef()
		local btn = ArmAttrResetFrameStuffBtn
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local useItem = container:getItem( nList, nGrid )
			if useItem:getItemId() > 0 then
				local itemdef = useItem:getItemDef()
				for i=1,MAX_ITEM_RESULT do
					if itemdef.Result[i-1].ResultID == RESULT_ARM_RECAST then
						if itemDef.WearPre.RoleLevelMin > itemdef.Result[i-1].ResultVal1 then
							ShowMidTips( "装备等级与重铸石不符" )
							return
						end
						break
					end
				end
			end
		end

		nList = this:GetClientUserData(0)
		nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx() , "ArmAttrResetFrame" )
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		if item:isItemBind() then
			this:SetClientUserData( 3, 1 )
		else
			this:SetClientUserData( 3, 0 )
		end
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		selectIdx = 0
		AutoAddStuffItem()
		UpdateItem()
	end
end

function ArmAttrResetFrameArmBtn_OnClick()
	if arg1 == "RightButton" then
		ClearArmItem()
	end
end

function ArmAttrResetFrameArmBtn_OnEnter()
	if this:GetClientUserData( 2 ) > 0 then
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) )
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item )
	end
end

function ArmAttrResetFrameStuffBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_ATTR_RESET;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), 1 );
	end
end

function ArmAttrResetFrameStuffBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();

	if dragfrom == DRAG_ARM_ATTR_RESET then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_AMM then
			ShowMidTips( "请放入重铸石" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end

		local lv
		local itemDef = item:getItemDef()
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_ARM_RECAST then
				lv = itemDef.Result[i-1].ResultVal1				
				break
			end
		end
		if lv == nil then
			ShowMidTips( "请放入重铸石" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end		

		local btn = ArmAttrResetFrameArmBtn
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local arm = container:getItem( nList, nGrid )
			if arm:getItemId() > 0 and arm:getItemDef().WearPre.RoleLevelMin > lv then
				ShowMidTips( "装备等级与重铸石不符" )
				UIEndDrag();
				UIMSG:getUIGV().DragFrom = DRAG_NONE;
				return
			end
		end

		nList = this:GetClientUserData(0)
		nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)

		ClearItemLock( "ArmAttrResetFrameStuffBtn" )
		for i=0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local stuff = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
			if stuff:getItemId() == item:getItemId() and stuff:isItemBind() == item:isItemBind() then
				LockItem( stuff:getListType(), stuff:getGridIdx(), stuff:getItemId(), "ArmAttrResetFrameStuffBtn" )
			end
		end
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		if item:isItemBind() then
			this:SetClientUserData( 3, 1 )
		else
			this:SetClientUserData( 3, 0 )
		end
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		UpdateItem()
	end
end

function ArmAttrResetFrameStuffBtn_OnClick()
	if arg1 == "RightButton" then
		ClearStuffItem()
	end
end

function ArmAttrResetFrameStuffBtn_OnEnter()
	local item = GetStuffItem()
	if item ~= nil and item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item )
	end
end

function ArmAttrResetFrameAttrBtn_OnClick()
	selectIdx = this:GetClientID()
	DisCheckAllBtn( "ArmAttrResetFrameAttrBtn", MAX_ARM_RESET_NUM )
	this:Checked()
end

local function GetArmMulVal( item )
	local idx = 1
	for i=1,item:getArmAddAttrNum() do
		if idx == selectIdx then
			return item:getArmAddAttr(i-1).Val
		end
		idx = idx + 1
	end
	for i=1,item:getArmWashAddAttrNum() do
		if idx == selectIdx then
			return item:getArmWashAddAttrInst(i-1).Val
		end
		idx = idx + 1
	end
	return 100
end

local lastTime = 0
function ArmAttrResetFrameOKBtn_OnClick()
	if ArmAttrResetFrameArmBtn:GetClientUserData( 2 ) == 0 then
		ShowMidTips( "请放入要重铸的装备" )
		return
	end
	if ArmAttrResetFrameStuffBtn:GetClientUserData( 2 ) == 0 then
		ShowMidTips( "请放入重铸石" )
		return
	end

	if selectIdx  == 0 then
		ShowMidTips( "请先选中要重铸的属性" )
		return
	end

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getMoney() < RESET_COST_MONEY then
		ShowMidTips( "银币不足，重铸失败" )
		return
	end

	local btn = getglobal( "ArmAttrResetFrameAttrBtn" .. selectIdx )
	if btn:GetClientUserData( 0 ) > 0 then
		ShowMidTips( "此条附加属性已经重铸为满值，请选择其他未满的属性进行重铸" )
		return
	end

	if os.clock() - lastTime >= 0.1 then
		local container = mainplayer:getContainer()
		local item = container:getItem( ArmAttrResetFrameArmBtn:GetClientUserData(0), ArmAttrResetFrameArmBtn:GetClientUserData(1) )

		t_LastData.Idx = selectIdx
		t_LastData.WID = item:getItemWId()
		t_LastData.val = GetArmMulVal( item, selectIdx )
		t_LastData.newVal = nil
		GameArmProce:requestArmAttrReset( ArmAttrResetFrameArmBtn:GetClientUserData(0), ArmAttrResetFrameArmBtn:GetClientUserData(1),
										ArmAttrResetFrameStuffBtn:GetClientUserData(2), ArmAttrResetFrameStuffBtn:GetClientUserData(3),
										selectIdx - 1 )
		lastTime = os.clock()
	end
end

function ArmAttrResetSuccess( nFlag )
	if nFlag > 0 then
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local btn = ArmAttrResetFrameArmBtn
		if btn:GetClientUserData( 0 ) > 0 then
			local item = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) )
			t_LastData.newVal = GetArmMulVal( item, selectIdx )
		end		
		UpdateChgTex()
	end
end

local MOVE_WIDTH_TICK = 1;
function ArmAttrResetFrame_OnUpdate()
	for i=1,MAX_ARM_RESET_NUM do
		local btn = getglobal( "ArmAttrResetFrameAttrBtn" .. i );
		if btn:IsShown() then
			local rich = getglobal( "ArmAttrResetFrameAttrBtn" .. i .. "Rich" );
			if rich:getLineWidth(0) > rich:GetWidth() then
				local sign = rich:GetClientUserData( 0 )
				local disx = rich:GetDispPosX() + MOVE_WIDTH_TICK * sign
				rich:SetDispPosX( disx )
				if disx + 240 > rich:getLineWidth(0) + 40 then
					rich:SetClientUserData( 0,-1 )
				end
				if disx <= 0 then
					rich:SetClientUserData( 0,1 )
				end
			end
		end
	end
end

---------------------------------------------------技能属性改造----------------------------------------------------------
local function ArmRecreateMessageBoxShown()
	if MessageBoxFrame:IsShown() and (MessageBoxFrame:GetClientString() == "技能属性激活" or MessageBoxFrame:GetClientString() == "技能属性锁定") then
		return true
	end
	return false
end
local function AutoAddRecreateStuff()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local arm = ArmAttrRecreateFrameArmBtn:GetClientUserData(2)
	if arm > 0 then
		local armItemDef = getItemDef( arm )
		local resetItemID = 0
		local grid = 0
		local ArmSkillRecastDef = GameArmProce:getArmSkillRecastDef( math.floor(armItemDef.WearPre.RoleLevelMin/10) )
		resetItemID = ArmSkillRecastDef.ResetItemID
		if resetItemID > 0 then
			local resetItemDef = getItemDef( resetItemID )
			local IconPath = GetItemEquipIconPath()..resetItemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			ArmAttrRecreateFrameStuffBtnIconTexture:SetTexture( IconPath )
			SetEquipImportantEffectUV( resetItemDef, "ArmAttrRecreateFrameStuffBtnUVAnimationTex" )
			ArmAttrResetFrameArmBtnBackTexture:Show()
			ArmAttrRecreateFrameStuffBtn:SetClientUserData( 0, CONTAINER_TYPE_SUNDRIES )
			ArmAttrRecreateFrameStuffBtn:SetClientUserData( 1, grid )
			ArmAttrRecreateFrameStuffBtn:SetClientUserData( 2, resetItemID )
			ArmAttrRecreateFrameCost:SetTextColor( 160,160,90 )
			if mainplayer:getMoney() < ArmSkillRecastDef.ResetMoneyNum then
				ArmAttrRecreateFrameCost:SetTextColor( 255,0,0 )
			end
			
			--计算因为锁定属性，而需要额外消耗的材料数
			local lockNum = 0
			local item = container:getItem( ArmAttrRecreateFrameArmBtn:GetClientUserData(0), ArmAttrRecreateFrameArmBtn:GetClientUserData(1) )
			for i = 1, item:getArmSkillNum() do
				local armSkillAttrOne = item:getArmSkillOne(i-1)
				if armSkillAttrOne.Lock ~= 0 then
					lockNum = lockNum + 1
				end
			end
			local needMoreItemNum = lockNum
			
			ArmAttrRecreateFrameCost:SetText( ArmSkillRecastDef.ResetMoneyNum )
			ArmAttrRecreateFrameStuffBtnCountFont:SetTextColor( 204,128,51 )
			if container:getItemCount(resetItemID) < ArmSkillRecastDef.ResetItemNum then
				ArmAttrRecreateFrameStuffBtnCountFont:SetTextColor( 255,0,0 )
			end
			ArmAttrRecreateFrameStuffBtnCountFont:SetText( container:getItemCount(resetItemID).."/"..(ArmSkillRecastDef.ResetItemNum+needMoreItemNum) )
		end
	else
		ArmAttrRecreateFrameStuffBtn:SetClientUserData( 0, 0 )
		ArmAttrRecreateFrameStuffBtn:SetClientUserData( 1, 0 )
		ArmAttrRecreateFrameStuffBtn:SetClientUserData( 2, 0 )
		ArmAttrRecreateFrameStuffBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		SetEquipImportantEffectUV( nil, "ArmAttrRecreateFrameStuffBtnUVAnimationTex",nil )
		ArmAttrRecreateFrameStuffBtnCountFont:SetText( "" )
		ArmAttrRecreateFrameCost:SetText( 0 )
	end
end
local t_ArmRecreateForbidType = { "项链", "戒指", "时装", "面饰", "徽章", }
function CanAddToArmAttrRecreate(ItemID)
	for i = 1, table.getn( t_ArmRecreateForbidType ) do
		if GetItemType(ItemID) == t_ArmRecreateForbidType[i] then
			return false
		end
	end
	return true
end
function Save2ArmArmAttrRecreateFrame( item )
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		local ItemDef = item:getItemDef()
		if not CanAddToArmAttrRecreate(ItemDef.ItemID) then
			ShowMidTips("此装备无法进行技能属性改造")
			return
		end
		if ItemDef.WearPre.RoleLevelMin < 60 or ItemDef.Important < 3 then
			ShowMidTips("请放入60级以上的紫色装备")
			return
		end
		local btn = ArmAttrRecreateFrameArmBtn
		local list = btn:GetClientUserData(0)
		local grid = btn:GetClientUserData(1)
		if list > 0 then
			UnLockItem( list,grid,0 )
		end
		--放入新的装备的时候，关闭之前的二级弹窗
		if ArmRecreateMessageBoxShown() then
			MessageBoxFrame:Hide()
		end
		
		btn:SetClientUserData( 0, item:getListType() )
		btn:SetClientUserData( 1, item:getGridIdx() )
		btn:SetClientUserData( 2, item:getItemId() )
		LockItem( item:getListType(), item:getGridIdx() , "ArmAttrRecreateFrame" )
		
		if item:getItemId() > 0 then		
			local itemdef = item:getItemDef()
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			ArmAttrRecreateFrameArmBtnIconTexture:SetTexture( IconPath )
			SetEquipImportantEffectUV( itemdef, "ArmAttrRecreateFrameArmBtnUVAnimationTex",item )
			if item:isItemBind() then
				ArmAttrResetFrameArmBtnBindTexture:Show()
			else
				ArmAttrResetFrameArmBtnBindTexture:Hide()
			end
			ArmAttrResetFrameArmBtnBackTexture:Show()
		end
		--设置属性
		SetArmSkillRecreateAttr( item )
		--自动添加改造所需材料
		AutoAddRecreateStuff()
	elseif item:getItemType() == ITEM_SLOTTYPE_ITEM then
		local itemDef = item:getItemDef()
		local btn = ArmAttrResetFrameArmBtn
		if btn:GetClientUserData(2) <= 0 then
			ShowMidTips("请先放入改造装备")
			return
		end
	end
end
function ArmAttrRecreateFrame_OnLoad()
	ItemFirstCallBack( "ArmAttrRecreateFrame" , Save2ArmArmAttrRecreateFrame );
	this:setUpdateTime( 0 )
end
function ArmAttrRecreateFrame_OnHide()
	if ArmRecreateMessageBoxShown() then
		MessageBoxFrame:Hide()
	end
	ClearItemLock( "ArmAttrRecreateFrame" )
	ClearArmRecreateItem()
end
function ClearArmRecreateItem()
	local btn = ArmAttrRecreateFrameArmBtn
	local list = btn:GetClientUserData(0)
	local grid = btn:GetClientUserData(1)
	if list > 0 then
		UnLockItem( list,grid,0 )
	end
		
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	ArmAttrRecreateFrameArmBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	SetEquipImportantEffectUV( nil, "ArmAttrRecreateFrameArmBtnUVAnimationTex",nil )
	SetArmSkillRecreateAttr( nil )
	AutoAddRecreateStuff()
end
function ArmAttrRecreateFrameArmBtn_OnEnter()
	if this:GetClientUserData( 2 ) > 0 then
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) )
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item )
	end
end
function ArmAttrRecreateFrameArmBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();

	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_ITEM then
			ShowMidTips( "请放入要改造的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end

		local itemDef = item:getItemDef()
		if not CanAddToArmAttrRecreate(itemDef.ItemID) then
			ShowMidTips("此装备无法进行技能属性改造")
			return
		end
		if itemDef.WearPre.RoleLevelMin < 60 or itemDef.Important < 3 then
			ShowMidTips("请放入60级以上的紫色装备")
			return
		end		

		nList = this:GetClientUserData(0)
		nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		Save2ArmArmAttrRecreateFrame( item )
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		AutoAddRecreateStuff()
	end
end
function ArmAttrRecreateFrameArmBtn_OnClick()
	if arg1 == "RightButton" then
		ClearItemLock( "ArmAttrRecreateFrame" )
		ClearArmRecreateItem()
	end
end
function ArmAttrRecreateFrameArmBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_BAG;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), 0 );
		ClearItemLock( "ArmAttrRecreateFrame" )
		ClearArmRecreateItem()
	end
end
function ArmAttrRecreateFrameStuffBtn_OnEnter()
	if this:GetClientUserData( 2 ) > 0 then
		local ItemDef = getItemDef( this:GetClientUserData( 2 ) )
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, ItemDef )
	end
end
local t_NumberToChineseNum = { "一", "二", "三", "四", "五", "六", }
local ARM_SKILL_RECREATE_MAX_NUM = 3
function SetArmSkillRecreateAttr( item )
	--先都取消上次显示
	ArmAttrRecreateFrameOKBtn:Disable()
	for i = 1, ARM_SKILL_RECREATE_MAX_NUM do
		local btn = getglobal( "ArmAttrRecreateFrameAttrBtn"..i )
		local Active = getglobal( btn:GetName().."Active" )
		local Lock = getglobal( btn:GetName().."Lock" )
		local Rich = getglobal( btn:GetName().."Rich" )
		Active:Hide()
		Rich:SetText("", 255,255,190)
		Lock:Hide()
	end
	for i = 1, ARM_SKILL_RECREATE_MAX_NUM do
		local btn = getglobal( "ArmAttrRecreateFrameAttrBtn"..i )
		local Active = getglobal( btn:GetName().."Active" )
		local Lock = getglobal( btn:GetName().."Lock" )
		local Rich = getglobal( btn:GetName().."Rich" )
		if item ~= nil then
			--还有属性未激活,最后一条属性显示激活
			if ARM_SKILL_RECREATE_MAX_NUM > item:getArmSkillNum() and i == item:getArmSkillNum()+1 and GetItemType(item:getItemDef().ItemID) ~= "护符"  then
				Active:Show()
				Rich:SetText("#G  激活第"..t_NumberToChineseNum[i].."条技能属性", 255,255,190)
				return
			--其他激活不显示，显示锁定状态,
			else
				if i <= item:getArmSkillNum() then
					local szSkillAttr = getArmSkillDesc( item:getItemDef(), item:getArmSkillOne(i-1) );
					Rich:SetText( "#B"..szSkillAttr, 255,255,190)
					Active:Hide()
					Lock:Show()
					if item:getArmSkillOne(i-1).Lock == 0 then
						Lock:DisChecked()
					else
						Lock:Checked()
					end	
				end
			end
			if item:getArmSkillNum() > 0 then				
				ArmAttrRecreateFrameOKBtn:Enable()
			end
		end
	end
end
function ArmAttrRecreateFrameAttrBtnActive_OnClick()
	local parent = getglobal(this:GetParent())
	local id = parent:GetClientID()
	if id <= 0 or ArmAttrRecreateFrameArmBtn:GetClientUserData(2) == 0 then return end 
	local itemDef = getItemDef( ArmAttrRecreateFrameArmBtn:GetClientUserData(2) )
	local ArmSkillRecastDef = GameArmProce:getArmSkillRecastDef( math.floor( itemDef.WearPre.RoleLevelMin/10 ) )
	local activeItemDef = getItemDef( ArmSkillRecastDef.Active[id-1].ItemID )
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local szActiveNeedItem = ArmSkillRecastDef.Active[id-1].ItemNum.."个"..activeItemDef.Name
	local szActiveNeedMoney = ArmSkillRecastDef.Active[id-1].MoneyNum..GetMoneyDesc(ArmSkillRecastDef.Active[id-1].MoenyType)
	local shortType = 0
	if ArmSkillRecastDef.Active[id-1].ItemNum > container:getItemCount(ArmSkillRecastDef.Active[id-1].ItemID) then
		szActiveNeedItem = "#cff0000"..szActiveNeedItem.."#n"
		shortType = 1
	end
	if ArmSkillRecastDef.Active[id-1].MoneyNum > mainplayer:getMoney() then
		szActiveNeedMoney = "#cff0000"..szActiveNeedMoney.."#n"
		shortType = 2
	end
	local szInfo = string.format( "激活第%s条属性需要消耗%s和%s，确定激活？", t_NumberToChineseNum[id], szActiveNeedItem, 
							szActiveNeedMoney  )
	MessageBox( "技能属性激活", szInfo )
	MessageBoxFrame:SetClientString("技能属性激活");
	MessageBoxFrame:SetClientUserData( 0, ArmAttrRecreateFrameArmBtn:GetClientUserData(0) );
	MessageBoxFrame:SetClientUserData( 1, ArmAttrRecreateFrameArmBtn:GetClientUserData(1) )
	--这里的类型，1材料不足，2银币不足
	MessageBoxFrame:SetClientUserData( 2, shortType )
end
function ArmAttrRecreateFrameAttrBtnLock_OnClick()
	local parent = getglobal(this:GetParent())
	if this:IsChecked() then
		GameArmProce:requestArmSkillLock(parent:GetClientID()-1,ArmAttrRecreateFrameArmBtn:GetClientUserData(1),ArmAttrRecreateFrameArmBtn:GetClientUserData(0))
		return
	end
	local lockNum = 0
	if ArmAttrRecreateFrameArmBtn:GetClientUserData(2) == 0 then return end 
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( ArmAttrRecreateFrameArmBtn:GetClientUserData(0), ArmAttrRecreateFrameArmBtn:GetClientUserData(1) )
	for i = 1, item:getArmSkillNum() do
		local armSkillAttrOne = item:getArmSkillOne(i-1)
		if armSkillAttrOne.Lock ~= 0 then
			lockNum = lockNum + 1
		end
	end
	if lockNum == 3 then return	end
	local itemDef = getItemDef( ArmAttrRecreateFrameArmBtn:GetClientUserData(2) )
	local ArmSkillRecastDef = GameArmProce:getArmSkillRecastDef( math.floor( itemDef.WearPre.RoleLevelMin/10 ) )
	local needMoreItemNum = lockNum + 1
	--[[
	local needMoreMoney = (lockNum + 1)*ArmSkillRecastDef.ResetMoneyNum/100
	local szInfo = string.format( "锁定技能属性后每次技能属性改造需要额外消耗%d个合金和%d%s，确定需要锁定属性？", needMoreItemNum,
						needMoreMoney, GetMoneyDesc(ArmSkillRecastDef.ResetMoneyType) )
						--]]
	local szInfo = string.format( "锁定技能属性后每次技能属性改造需要额外消耗%d个合金，确定需要锁定属性？", needMoreItemNum)
	MessageBox( "技能属性锁定", szInfo )
	MessageBoxFrame:SetClientString("技能属性锁定");
	MessageBoxFrame:SetClientUserData( 0, ArmAttrRecreateFrameArmBtn:GetClientUserData(0) );
	MessageBoxFrame:SetClientUserData( 1, ArmAttrRecreateFrameArmBtn:GetClientUserData(1) );
	MessageBoxFrame:SetClientUserData( 2, parent:GetClientID()-1 );
end
function ArmSKillRecreateResult( nFlag )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local item = container:getItem( ArmAttrRecreateFrameArmBtn:GetClientUserData(0), ArmAttrRecreateFrameArmBtn:GetClientUserData(1) )
	SetArmSkillRecreateAttr( item )
	AutoAddRecreateStuff()
end
function ArmAttrRecreateFrameOKBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local arm = ArmAttrRecreateFrameArmBtn:GetClientUserData(2)
	if arm > 0 then
		local armItemDef = getItemDef( arm )
		local resetItemID = 0
		local ArmSkillRecastDef = GameArmProce:getArmSkillRecastDef( math.floor(armItemDef.WearPre.RoleLevelMin/10) )
		resetItemID = ArmSkillRecastDef.ResetItemID
		if resetItemID > 0 then
			if mainplayer:getMoney() < ArmSkillRecastDef.ResetMoneyNum then
				ShowMidTips( "改造失败，改造银币不足！" )
				return
			end
			if container:getItemCount(resetItemID) < ArmSkillRecastDef.ResetItemNum then
				ShowMidTips( "改造失败，改造材料不足！" )
				return
			end
		end
		GameArmProce:requestArmSkillReset( ArmAttrRecreateFrameArmBtn:GetClientUserData(1), ArmAttrRecreateFrameArmBtn:GetClientUserData(0), 0, 3 )
	end
end
function OpenArmSkillRereateFrame( npcId )
	ClearItemLock( "ArmAttrRecreateFrame" )
	ClearArmRecreateItem()
	SetArmSkillRecreateAttr( nil )
	AutoAddRecreateStuff()
	ArmAttrRecreateFrame:Show()
	util.testNpcDialogDistance( npcId, PetUnBindFrame );
end
function ArmAttrRecreateFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	PlayerContainerFrame:SetPoint("center", "UIClient", "center", 200, 0)
	this:SetPoint( "topright", "PlayerContainerFrame", "topleft", -10, 0 );
end
function ArmAttrRecreateFrame_OnUpdate()
	AutoAddRecreateStuff()
end
