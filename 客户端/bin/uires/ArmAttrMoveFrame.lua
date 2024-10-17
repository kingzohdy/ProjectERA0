
local BASE_ATTR_MOVE_TYPE 	= 0;   		-- 基础属性转移
local SKILL_ATTR_MOVE_TYPE 	= 1;		-- 技能属性转移

local MOVE_COST_MONEY = getArmMoveMoney( BASE_ATTR_MOVE_TYPE );
local SKILL_MOVE_COST_MONEY = getArmMoveMoney( SKILL_ATTR_MOVE_TYPE );

local ARM_SKILL_MOVE_UITYPE_LOW 	= 88 ;  -- 技能转移 低级界面
local ARM_SKILL_MOVE_UITYPE_HIGH 	= 89;	-- 技能转移 高级界面

local function AutoStuff()
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local flag = ArmAttrMoveFrame:IsShown() and 0 or 1;
	local srcBtn = getglobal( frame .. "SrcBtn" );
	local dstBtn = getglobal( frame .. "DstBtn" );
	local useBtn = getglobal( frame .. "UseBtn" );

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	if useBtn:GetClientUserData(2) > 0 then
		return
	end
	local arm = srcBtn:GetClientUserData(2)

	if arm == 0 then
		arm = dstBtn:GetClientUserData(2)
	end
	if arm > 0 then
		local grid	= 0;
		local itemid = 0;
		local minlv = 200; 

		local ArmDef = getItemDef( arm )
		
		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
			local val1
			if item:getItemId() > 0 and item:getItemType() == ITEM_SLOTTYPE_ITEM then
				for i=1, MAX_ITEM_RESULT do
					if item:getItemDef().Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE and
						item:getItemDef().Result[i-1].ResultVal2 == flag then
						val1 = item:getItemDef().Result[i-1].ResultVal1
						break
					end
				end
				if val1 ~= nil and ArmDef.WearPre.RoleLevelMin <= val1 and val1 <= minlv then
					minlv = val1
					grid = i
					itemid	= item:getItemId()
				end
			end
		end
		if itemid > 0 then
			LockItem( CONTAINER_TYPE_SUNDRIES, grid , frame )
			useBtn:SetClientUserData( 0, CONTAINER_TYPE_SUNDRIES )
			useBtn:SetClientUserData( 1, grid )
			useBtn:SetClientUserData( 2, itemid )
		end
	end
end

local function ClearSrcItem( frame )
	local srcBtn = getglobal( frame .. "SrcBtn" );
	local nList = srcBtn:GetClientUserData(0)
	local nGrid = srcBtn:GetClientUserData(1)
	local nItemId = srcBtn:GetClientUserData(2)
	if nList > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end
	srcBtn:SetClientUserData( 0, 0 )
	srcBtn:SetClientUserData( 1, 0 )
	srcBtn:SetClientUserData( 2, 0 )
	local tex = getglobal( frame .. "SrcBtnIconTexture" );
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	tex = getglobal( frame .. "SrcBtnUVAnimationTex" );
	tex:Hide();
	tex = getglobal( frame .. "SrcBtnBindTexture" );
	tex:Hide();
	tex = getglobal( frame .. "SrcBtnBackTexture" );
	tex:Show();
	local font = getglobal( frame .. "SrcBtnCountFont" );
	font:SetText("")
	font = getglobal( frame .. "SrcName" );
	font:SetText( "放入转移装备" )
end

local function UpdateSrcItem()
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local srcBtn = getglobal( frame .. "SrcBtn" );

	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( srcBtn:GetClientUserData(0), srcBtn:GetClientUserData(1) )
	if item:getItemId() > 0 and item:getItemId() == srcBtn:GetClientUserData(2) then		
		local itemdef = item:getItemDef()
		local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		local tex = getglobal( frame .. "SrcBtnIconTexture" );
		tex:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemdef, frame .. "SrcBtnUVAnimationTex",item )
		tex = getglobal( frame .. "SrcBtnBindTexture" );
		if item:isItemBind() then
			tex:Show()
		else
			tex:Hide()
		end
		tex = getglobal( frame .. "SrcBtnBackTexture" );
		tex:Show()
		local font = getglobal( frame .. "SrcName" );
		font:SetText( itemdef.Name )
	else
		ClearSrcItem( frame )
	end
end

local function ClearDstItem( frame )
	local dstBtn = getglobal( frame .. "DstBtn" );
	local nList = dstBtn:GetClientUserData(0)
	local nGrid = dstBtn:GetClientUserData(1)
	local nItemId = dstBtn:GetClientUserData(2)
	if nList > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end
	dstBtn:SetClientUserData( 0, 0 )
	dstBtn:SetClientUserData( 1, 0 )
	dstBtn:SetClientUserData( 2, 0 )
	local tex = getglobal( frame .. "DstBtnIconTexture" );
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	tex = getglobal( frame .. "DstBtnUVAnimationTex" );
	tex:Hide();
	tex = getglobal( frame .. "DstBtnBindTexture" );
	tex:Hide();
	tex = getglobal( frame .. "DstBtnBackTexture" );
	tex:Show();
	local font = getglobal( frame .. "DstName" );
	font:SetText( "目标装备" )
end

local function UpdateDstItem()
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local dstBtn = getglobal( frame .. "DstBtn" );
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( dstBtn:GetClientUserData(0), dstBtn:GetClientUserData(1) )
	if item:getItemId() > 0 and item:getItemId() == dstBtn:GetClientUserData(2) then		
		local itemdef = item:getItemDef()
		local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		local tex = getglobal( frame .. "DstBtnIconTexture" );
		tex:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemdef, frame .. "DstBtnUVAnimationTex",item )
		tex = getglobal( frame .. "DstBtnBindTexture" );
		if item:isItemBind() then
			tex:Show()
		else
			tex:Hide()
		end
		tex = getglobal( frame .. "DstBtnBackTexture" );
		tex:Show()
		local font = getglobal( frame .. "DstName" );
		font:SetText( itemdef.Name )
	else
		ClearDstItem( frame )		
	end
end

local function ClearUseItem( frame )
	local useBtn = getglobal( frame .. "UseBtn" );
	local nList = useBtn:GetClientUserData(0)
	local nGrid = useBtn:GetClientUserData(1)
	local nItemId = useBtn:GetClientUserData(2)
	if nList > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end
	useBtn:SetClientUserData( 0, 0 )
	useBtn:SetClientUserData( 1, 0 )
	useBtn:SetClientUserData( 2, 0 )
	local tex = getglobal( frame .. "UseBtnIconTexture" );
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	tex = getglobal( frame .. "UseBtnUVAnimationTex" );
	tex:Hide();
	tex = getglobal( frame .. "UseBtnBindTexture" );
	tex:Hide();
	tex = getglobal( frame .. "UseBtnBackTexture" );
	tex:Show();
	local font = getglobal( frame .. "UseBtnCountFont" );
	font:SetText("")
	font = getglobal( frame .. "UseName" );
	font:SetText( "放入转移石" )
end

local function UpdateUseItem()
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local useBtn = getglobal( frame .. "UseBtn" );
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( useBtn:GetClientUserData(0), useBtn:GetClientUserData(1) )
	if item:getItemId() > 0 and item:getItemId() == useBtn:GetClientUserData(2) then		
		local itemdef = item:getItemDef()
		local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		local tex = getglobal( frame .. "UseBtnIconTexture" );
		tex:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemdef, frame .. "UseBtnUVAnimationTex",item )
		tex = getglobal( frame .. "UseBtnBindTexture" );
		if item:isItemBind() then
			tex:Show()
		else
			tex:Hide()
		end
		tex = getglobal( frame .. "UseBtnBackTexture" );
		tex:Show()
		local font = getglobal( frame .. "UseBtnCountFont" );
		font:SetText( item:getNum() )
		font = getglobal( frame .. "UseName" );
		font:SetText( itemdef.Name )
	else
		ClearUseItem( frame )
	end
end

local function CheckBtnState()
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local money = getglobal( frame .. "CostMoney" );
	local srcBtn = getglobal( frame .. "SrcBtn" );
	local dstBtn = getglobal( frame .. "DstBtn" );
	local srcName = getglobal( frame .. "SrcName" );
	local dstName = getglobal( frame .. "DstName" );

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getMoney() < MOVE_COST_MONEY then
		money:SetTextColor( 255, 0, 0 )
	else
		money:SetTextColor( 255, 255, 190 )
	end

	local srcID = srcBtn:GetClientUserData( 2 )
	local dstID = dstBtn:GetClientUserData( 2 )
	srcName:SetTextColor( 160,160,90 )
	dstName:SetTextColor( 160,160,90 )
	if srcID == 0 or dstID == 0 then
		return;	
	end
	
	if frame == "ArmAttrMoveFrame" then
		if srcID ~= dstID then
			srcName:SetTextColor( 255,0,0 )
			dstName:SetTextColor( 255,0,0 )
		end
	else
		local srcItemDef = getItemDef( srcID );
		local dstItemDef = getItemDef( dstID );
		if srcItemDef.WearPre.Career ~= dstItemDef.WearPre.Career or srcItemDef.WearPos1 ~= dstItemDef.WearPos1 or
			srcItemDef.WearPre.RoleLevelMin ~= dstItemDef.WearPre.RoleLevelMin then
			srcName:SetTextColor( 255,0,0 )
			dstName:SetTextColor( 255,0,0 )
		end
	end
end

local function UpdateItem()
	UpdateSrcItem()
	UpdateDstItem()
	AutoStuff()
	UpdateUseItem()
	CheckBtnState()
end

local function ClearItem( frame )
	ClearSrcItem( frame )
	ClearDstItem( frame )
	ClearUseItem( frame )
end

function ClearArmAttrMoveBtn( btnName, desList, desGrid )
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local btn = getglobal( frame .. btnName );
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
	UpdateItem()
end

local function Save2ArmArmAttrMoveFrame( item )
	local btn,list
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		btn = ArmAttrMoveFrameSrcBtn
		list = btn:GetClientUserData(0)
		local use = ArmAttrMoveFrameUseBtn:GetClientUserData(2)
		local val1 
		-- 装备未鉴定
		if item:isArmIdentity() then 
			return ShowMidTips("此装备还未鉴定，不能进行此操作");
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能进行属性转移" );
			return;
		end
		if use > 0 then
			local useItemDef = getItemDef( use )
			for i=1,MAX_ITEM_RESULT do
				if useItemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
					val1 = useItemDef.Result[i-1].ResultVal1
				end	
			end
		end
		if list == 0 then			
			if val1 ~= nil and val1 < item:getItemDef().WearPre.RoleLevelMin then
				return ShowMidTips( "装备等级与转移石不符" )
			end
			btn:SetClientUserData( 0, item:getListType() )
			btn:SetClientUserData( 1, item:getGridIdx() )
			btn:SetClientUserData( 2, item:getItemId() )
			LockItem( item:getListType(), item:getGridIdx() , "ArmAttrMoveFrame" )			
			UpdateItem()
			return
		end
		btn = ArmAttrMoveFrameDstBtn
		list = btn:GetClientUserData(0)
		if list == 0 then
			if val1 ~= nil and val1 < item:getItemDef().WearPre.RoleLevelMin then
				return ShowMidTips( "装备等级与转移石不符" )
			end
			btn:SetClientUserData( 0, item:getListType() )
			btn:SetClientUserData( 1, item:getGridIdx() )
			btn:SetClientUserData( 2, item:getItemId() )
			LockItem( item:getListType(), item:getGridIdx() , "ArmAttrMoveFrame" )
			UpdateItem()
			return
		end
		ShowMidTips( "更换属性转移的装备，请先取回装备栏内的装备" )
	elseif item:getItemType() == ITEM_SLOTTYPE_ITEM then
		local itemdef = item:getItemDef()
		local val1
		local val2
		for i=1,MAX_ITEM_RESULT do
			if itemdef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
				val1 = itemdef.Result[i-1].ResultVal1
				val2 = itemdef.Result[i-1].ResultVal2
				break
			end
		end
		if val1 == nil or val2 == nil then
			return ShowMidTips( "请放入转移石" )
		end
		if val2 > 0 then
			return ShowMidTips( "该转移石只能用于属性高阶转移" );
		end

		local arm = ArmAttrMoveFrameSrcBtn:GetClientUserData(2)
		if arm == 0 then
			arm = ArmAttrMoveFrameDstBtn:GetClientUserData(2)
		end
		if arm > 0 then
			local armDef = getItemDef(arm)
			if val1 < armDef.WearPre.RoleLevelMin then
				return ShowMidTips( "装备等级与转移石不符" )
			end
		end

		btn = ArmAttrMoveFrameUseBtn
		if btn:GetClientUserData(2) > 0 then
			UnLockItem( btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );--解锁
		end
		btn:SetClientUserData( 0, item:getListType() )
		btn:SetClientUserData( 1, item:getGridIdx() )
		btn:SetClientUserData( 2, item:getItemId() )
		LockItem( item:getListType(), item:getGridIdx() , "ArmAttrMoveFrame" )
		UpdateItem()
		return
	end
end

local szDesc = [[
☆相同的装备才能转属性
#c00e600☆将装备上的绿色附加属性替换至目标装备
☆转移后目标装备绑定，转移装备破碎消失#n
#cffff6e相同装备：同等级、同品质、同职业、同部位、同名称。#c00dcdc(杰出的 迅捷 修士指环 名称:修士指环)#n]]
function ArmAttrMoveFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ArmAttrMoveFrameRichText:SetText( szDesc,160,155,130 )
	ItemFirstCallBack( "ArmAttrMoveFrame" , Save2ArmArmAttrMoveFrame );
end

function ArmAttrMoveFrame_OnEvent()
	if this:IsShown() then
		if arg1 == "GE_CHANGE_MONEY" then
			CheckBtnState()			
		elseif arg1 == "GE_CONTAINER_UPDATE" then
			UpdateItem()
		end
	end
end

function ArmAttrMoveFrame_OnShow()
	local btn = ArmAttrMoveFrameSrcBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	btn = ArmAttrMoveFrameDstBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	btn = ArmAttrMoveFrameUseBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	ArmAttrMoveFrameCostMoney:SetText( MOVE_COST_MONEY )
	UpdateItem()
end

function ArmAttrMoveFrame_OnHide()
	ClearItem( this:GetName() )
end

function ArmAttrMoveFrameSrcBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_ATTR_MOVE_SRC;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), this:GetClientUserData(2) );
	end
end

function ArmAttrMoveFrameSrcBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local flag = ArmAttrMoveFrame:IsShown() and 0 or 1;
	if dragfrom == DRAG_ARM_ATTR_MOVE_SRC then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == DRAG_ARM_ATTR_MOVE_DST then		
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_ITEM then
			ShowMidTips( "请放入要转移的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能进行属性转移" );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		local itemDef = item:getItemDef()
		if flag > 0 and itemDef.Important < 3 then
			ShowMidTips( "请放入紫色品质以上装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if flag > 0 and itemDef.Important == 7 then
			ShowMidTips( "传奇装备无法进行高级属性转移" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		local btn = getglobal( frame .. "UseBtn" );
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local useItem = container:getItem( nList, nGrid )
			if useItem:getItemId() > 0 then
				local useItemDef = useItem:getItemDef()
				for i=1,MAX_ITEM_RESULT do
					if useItemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
						if itemDef.WearPre.RoleLevelMin > useItemDef.Result[i-1].ResultVal1 then
							ShowMidTips( "装备等级与转移石不符" )
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
		LockItem( item:getListType(), item:getGridIdx() , frame )
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		UpdateItem()
	end
end

function ArmAttrMoveFrameBtn_OnClick()
	if arg1 == "RightButton" then
		local nList = this:GetClientUserData(0)
		local nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
			this:SetClientUserData( 0, 0 )
			this:SetClientUserData( 1, 0 )
			this:SetClientUserData( 2, 0 )
			if string.find( this:GetName(), "UseBtn", 1, true ) then
				ClearUseItem( this:GetParent() )
			else
				UpdateItem()
			end
		end
	end
end

function ArmAttrMoveFrameBtn_OnEnter()
	if this:GetClientUserData( 2 ) > 0 then
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) )
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item )
	end
end

function ArmAttrMoveFrameDstBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_ATTR_MOVE_DST;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), this:GetClientUserData(2) );
	end
end

function ArmAttrMoveFrameDstBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local flag = ArmAttrMoveFrame:IsShown() and 0 or 1;
	if dragfrom == DRAG_ARM_ATTR_MOVE_DST then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == DRAG_ARM_ATTR_MOVE_SRC then
		--TODO HERE 这里是否需要交换
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_ITEM then
			ShowMidTips( "请放入要转移的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能进行属性转移" );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		local itemDef = item:getItemDef()
		if flag > 0 and itemDef.Important < 3 then
			ShowMidTips( "请放入紫色品质以上装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if flag > 0 and itemDef.Important == 7 then
			ShowMidTips( "传奇装备无法进行高级属性转移" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		local btn = getglobal( frame .. "UseBtn" );
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local useItem = container:getItem( nList, nGrid )
			if useItem:getItemId() > 0 then
				local useItemDef = useItem:getItemDef()
				for i=1,MAX_ITEM_RESULT do
					if useItemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
						if itemDef.WearPre.RoleLevelMin > useItemDef.Result[i-1].ResultVal1 then
							return ShowMidTips( "装备等级与转移石不符" )
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
		LockItem( item:getListType(), item:getGridIdx() , frame )
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		UpdateItem()
	end
end

function ArmAttrMoveFrameUseBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_ATTR_MOVE_USE;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), this:GetClientUserData(2) );
	end
end

function ArmAttrMoveFrameUseBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local frame = ArmAttrMoveFrame:IsShown() and "ArmAttrMoveFrame" or "ArmAdvAttrMoveFrame";
	local flag = ArmAttrMoveFrame:IsShown() and 0 or 1;
	if dragfrom == DRAG_ARM_ATTR_MOVE_USE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_AMM then
			ShowMidTips( "请放入转移石" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end

		local itemDef = item:getItemDef()
		local val1
		local val2
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
				val1 = itemDef.Result[i-1].ResultVal1
				val2 = itemDef.Result[i-1].ResultVal2
				break
			end
		end
		if val1 == nil or val2 == nil then
			return ShowMidTips( "请放入转移石" )
		end
		if flag ~= val2 then
			return ShowMidTips( string.format( "该转移石只能用于属性%s转移", val2 > 0 and "高阶" or "普通" ) );
		end

		local btn = getglobal( frame .. "SrcBtn" );
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local srcItem = container:getItem( nList, nGrid )
			if srcItem:getItemId() > 0 and val1 < srcItem:getItemDef().WearPre.RoleLevelMin then
				return ShowMidTips("装备等级与转移石不符")
			end
		end

		btn = getglobal( frame .. "DstBtn" );
		nList = btn:GetClientUserData(0)
		nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local dstItem = container:getItem( nList, nGrid )
			if dstItem:getItemId() > 0 and val1 < dstItem:getItemDef().WearPre.RoleLevelMin then
				return ShowMidTips("装备等级与转移石不符")
			end
		end

		nList = this:GetClientUserData(0)
		nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx() , frame )
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		UpdateItem()
	end
end


-- 添加多次点击保护
local lastTime = 0
function ArmAttrMoveFrameOKBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getMoney() < MOVE_COST_MONEY then
		ShowBuyMoneyFrame();
		return ShowMidTips( "银币不足，无法进行属性转移" )
	end
	local parent = this:GetParent();
	local srcBtn = getglobal( parent .. "SrcBtn" );
	local dstBtn = getglobal( parent .. "DstBtn" );
	local useBtn = getglobal( parent .. "UseBtn" );

	if srcBtn:GetClientUserData(2) == 0 or dstBtn:GetClientUserData(2) == 0 then
		return ShowMidTips( "请先放入装备" )
	end

	if string.find( parent, "Adv", 1, true ) then
		local srcItemDef = getItemDef( srcBtn:GetClientUserData(2) );
		local dstItemDef = getItemDef( dstBtn:GetClientUserData(2) );
		if srcItemDef.Important < 3 or dstItemDef.Important < 3 then
			return ShowMidTips( "属性高阶转移只能对紫色品质以上的装备进行转移" )
		end
		if srcItemDef.WearPre.Career ~= dstItemDef.WearPre.Career or srcItemDef.WearPos1 ~= dstItemDef.WearPos1 or
			srcItemDef.WearPre.RoleLevelMin ~= dstItemDef.WearPre.RoleLevelMin then
			return ShowMidTips( "属性高阶转移只能在同职业，同部位，同等级的装备之间进行转移" )
		end
	else
		if srcBtn:GetClientUserData(2) ~= dstBtn:GetClientUserData(2) then
			return ShowMidTips( "属性转移只能在同品质，同名称的装备之间进行转移" )
		end
	end

	if useBtn:GetClientUserData(2) == 0 then
		return ShowMidTips( "请先放入转移石" )
	end
	
	local container	= mainplayer:getContainer();
	local srcItem = container:getItem( srcBtn:GetClientUserData(0), srcBtn:GetClientUserData(1) );
	local idx = CheckeItemImportantConfirmForSpecial( srcItem, parent );
	if idx > 0 then
		ImportantItemConfirmFrameTitle:SetText("装备属性转移确认");
		ShowImportantItemConfirmFrame( "技能属性转移", srcItem, idx, true );
		SetItemWarnFrame(parent);
		ImportantItemConfirmFrame:SetClientUserData( 0, 0 );
		ImportantItemConfirmFrame:SetClientUserData( 1, 0 );
		ImportantItemConfirmFrame:SetClientUserData( 2, 0 );
		return;
	end

	if os.clock() - lastTime >= 0.1 then
		GameArmProce:requestArmAttrMove( srcBtn:GetClientUserData(0),srcBtn:GetClientUserData(1),
									 dstBtn:GetClientUserData(0),dstBtn:GetClientUserData(1),
									 useBtn:GetClientUserData(0),useBtn:GetClientUserData(1),BASE_ATTR_MOVE_TYPE );
		lastTime = os.clock()
	end
end

function ArmAttrMoveStart( frameName )
	local srcBtn = getglobal( frameName.."SrcBtn" );
	local dstBtn = getglobal( frameName.."DstBtn" );
	local useBtn = getglobal( frameName.."UseBtn" );
	GameArmProce:requestArmAttrMove( srcBtn:GetClientUserData(0),srcBtn:GetClientUserData(1),
								 dstBtn:GetClientUserData(0),dstBtn:GetClientUserData(1),
								 useBtn:GetClientUserData(0),useBtn:GetClientUserData(1),BASE_ATTR_MOVE_TYPE );
end

function ArmAttrMoveSuccess( nFlag, nIndex , nType )
	local frame;
	if nType == BASE_ATTR_MOVE_TYPE then
		frame = nIndex > 0 and "ArmAdvAttrMoveFrame" or "ArmAttrMoveFrame";
	elseif nType == SKILL_ATTR_MOVE_TYPE then 
		frame = "ArmSkillAttrMoveFrame";
	else 
		ShowMidTips("该转移类型不存在！！");
		return;
	end
	local dstModel = getglobal( frame .. "DstBtnModelView" );
	local srcModel = getglobal( frame .. "SrcBtnModelView" );
	if nFlag > 0 then
		dstModel:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		dstModel:setPetScale( 0, 0.8, 0.8, 0.8 );
		dstModel:setEntityPosition(0, 0, 100, 0)
		if not dstModel:IsShown() then
			dstModel:Show();
		end
		srcModel:setPetModel(0, 0, "effect\\entity\\baoshixiaohao.ent")
		srcModel:setPetScale( 0, 0.8, 0.8, 0.8 );
		srcModel:setEntityPosition(0, 0, 100, 0)
		if not srcModel:IsShown() then
			srcModel:Show();
		end
	else 
		ShowMidTips("技能属性转移失败！！")
	end
end

----------------------------------------------高级属性转移----------------------------------------------

local szAdvDesc = [[
#c00e600☆相同等级,相同职业,相同部位的紫色和粉色品质装备,附加属性中的绿色属性可以相互转移,替换至目标装备
☆转移后,目标装备强制绑定,转移装备破碎消失#n]]

local function Save2ArmAdvAttrMoveFrame( item )
	local btn,list
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		btn = getglobal( "ArmAdvAttrMoveFrameSrcBtn" )
		list = btn:GetClientUserData(0)
		local use = ArmAdvAttrMoveFrameUseBtn:GetClientUserData(2)
		local val1 
		-- 装备未鉴定
		if item:isArmIdentity() then 
			return ShowMidTips("此装备还未鉴定，不能进行此操作");
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能进行属性转移" );
			return;
		end
		if item:getItemDef().Important < 3 then
			return ShowMidTips("请放入紫色品质以上装备");
		end
		if item:getItemDef().Important == 7 then
			return ShowMidTips("传奇装备无法进行高级属性转移");
		end
		if use > 0 then
			local useItemDef = getItemDef( use )
			for i=1,MAX_ITEM_RESULT do
				if useItemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
					val1 = useItemDef.Result[i-1].ResultVal1
				end	
			end
		end
		if list == 0 then
			if val1 ~= nil and val1 < item:getItemDef().WearPre.RoleLevelMin then
				return ShowMidTips( "装备等级与转移石不符" )
			end
			btn:SetClientUserData( 0, item:getListType() )
			btn:SetClientUserData( 1, item:getGridIdx() )
			btn:SetClientUserData( 2, item:getItemId() )
			LockItem( item:getListType(), item:getGridIdx() , "ArmAdvAttrMoveFrame" )			
			UpdateItem()
			return
		end
		btn = getglobal( "ArmAdvAttrMoveFrameDstBtn" )
		list = btn:GetClientUserData(0)
		if list == 0 then
			if val1 ~= nil and val1 < item:getItemDef().WearPre.RoleLevelMin then
				return ShowMidTips( "装备等级与转移石不符" )
			end
			btn:SetClientUserData( 0, item:getListType() )
			btn:SetClientUserData( 1, item:getGridIdx() )
			btn:SetClientUserData( 2, item:getItemId() )
			LockItem( item:getListType(), item:getGridIdx() , "ArmAdvAttrMoveFrame" )
			UpdateItem()
			return
		end
		ShowMidTips( "更换属性转移的装备，请先取回装备栏内的装备" )
	elseif item:getItemType() == ITEM_SLOTTYPE_ITEM then
		local itemdef = item:getItemDef()
		local val1
		local val2
		for i=1,MAX_ITEM_RESULT do
			if itemdef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
				val1 = itemdef.Result[i-1].ResultVal1
				val2 = itemdef.Result[i-1].ResultVal2
				break
			end
		end
		if val1 == nil or val2 == nil then
			return ShowMidTips( "请放入转移石" )
		end
		if val2 == 0 then
			return ShowMidTips( "该转移石只能用于属性普通转移" );
		end

		local arm = ArmAdvAttrMoveFrameSrcBtn:GetClientUserData(2)
		if arm == 0 then
			arm = ArmAdvAttrMoveFrameDstBtn:GetClientUserData(2)
		end
		if arm > 0 then
			local armDef = getItemDef(arm)
			if val1 < armDef.WearPre.RoleLevelMin then
				return ShowMidTips( "装备等级与转移石不符" )
			end
		end

		btn = getglobal( "ArmAdvAttrMoveFrameUseBtn" );
		if btn:GetClientUserData(2) > 0 then
			UnLockItem( btn:GetClientUserData(0), btn:GetClientUserData(1), btn:GetClientUserData(2) );--解锁
		end
		btn:SetClientUserData( 0, item:getListType() )
		btn:SetClientUserData( 1, item:getGridIdx() )
		btn:SetClientUserData( 2, item:getItemId() )
		LockItem( item:getListType(), item:getGridIdx() , "ArmAdvAttrMoveFrame" )
		UpdateItem()
		return
	end
end

function ArmAdvAttrMoveFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ArmAdvAttrMoveFrameRichText:SetText( szAdvDesc,160,155,130 )
	ItemFirstCallBack( "ArmAdvAttrMoveFrame" , Save2ArmAdvAttrMoveFrame );
end

function ArmAdvAttrMoveFrame_OnEvent()
	if this:IsShown() then
		if arg1 == "GE_CHANGE_MONEY" then
			CheckBtnState()			
		elseif arg1 == "GE_CONTAINER_UPDATE" then
			UpdateItem()
		end
	end
end

function ArmAdvAttrMoveFrame_OnShow()
	local btn = ArmAdvAttrMoveFrameSrcBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	btn = ArmAdvAttrMoveFrameDstBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	btn = ArmAdvAttrMoveFrameUseBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	ArmAdvAttrMoveFrameCostMoney:SetText( MOVE_COST_MONEY )
	UpdateItem()
end

function ArmAdvAttrMoveFrame_OnHide()
	ClearItem( this:GetName() )
end

----------------------------------------------技能属性转移-------------------------------------------------------
local nCurMoveType = 0;			-- 当前转移类型

local nIndexSrcItem = 1;      	-- 对应btn ClientID
local nIndexUseItem = 2;
local nIndexDstItem = 3;

local t_szArmSkillTips = {
	[1] = [[
#cff4e00技能属性#n替换说明：
☆替换装备必须有#cff4e00技能属性#n才能替换
☆替换装备上的技能属性将覆盖至被替换装备
☆普通替换只能在相同的装备之间进行替换
#cffff6e相同装备：同等级、同品质、同职业、同部位、同名称。#c00dcdc(杰出的 迅捷 修士指环 名称:修士指环)#n
			]],
	[2] = [[
#cff4e00技能属性#n替换说明：
☆替换装备必须有#cff4e00技能属性#n才能替换
☆替换装备上的技能属性将覆盖至被替换装备
#cffff6e☆高级替换可在同职业、同等级、同部位间的紫色和粉色装备替换#n
			]]
		}
local t_btnNameList = { "ArmSkillAttrMoveFrameSrcBtn","ArmSkillAttrMoveFrameUseBtn","ArmSkillAttrMoveFrameDstBtn"};
-- 更新 装备名字颜色
local function CheckItemBtnColorState()
	local money 	= getglobal( "ArmSkillAttrMoveFrameCostMoney" );
	local srcBtn 	= getglobal( "ArmSkillAttrMoveFrameSrcBtn" );
	local dstBtn 	= getglobal( "ArmSkillAttrMoveFrameDstBtn" );
	local srcName 	= getglobal( "ArmSkillAttrMoveFrameSrcName" );
	local dstName 	= getglobal( "ArmSkillAttrMoveFrameDstName" );

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getMoney() < SKILL_MOVE_COST_MONEY then
		money:SetTextColor( 255, 0, 0 )
	else
		money:SetTextColor( 255, 255, 190 )
	end

	local srcID = srcBtn:GetClientUserData( 2 )
	local dstID = dstBtn:GetClientUserData( 2 )
	srcName:SetTextColor( 160,160,90 )
	dstName:SetTextColor( 160,160,90 )
	if srcID == 0 or dstID == 0 then
		return;	
	end
	
	if nCurMoveType == ARM_SKILL_MOVE_UITYPE_LOW then
		if srcID ~= dstID then
			srcName:SetTextColor( 255,0,0 )
			dstName:SetTextColor( 255,0,0 )
		end
	elseif nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH then
		local srcItemDef = getItemDef( srcID );
		local dstItemDef = getItemDef( dstID );
		if srcItemDef.WearPre.Career ~= dstItemDef.WearPre.Career or srcItemDef.WearPos1 ~= dstItemDef.WearPos1 or
			srcItemDef.WearPre.RoleLevelMin ~= dstItemDef.WearPre.RoleLevelMin then
			srcName:SetTextColor( 255,0,0 )
			dstName:SetTextColor( 255,0,0 )
		end
	end
end
-- 自动放入 转移石
local function SkillAttrMoveAutoUseItem()
	local srcBtn = getglobal( "ArmSkillAttrMoveFrameSrcBtn" );
	local dstBtn = getglobal( "ArmSkillAttrMoveFrameDstBtn" );
	local useBtn = getglobal( "ArmSkillAttrMoveFrameUseBtn" );
	local srcItemID = srcBtn:GetClientUserData(2);
	local desItemID = dstBtn:GetClientUserData(2);
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	if useBtn:GetClientUserData(2) > 0 or ( srcItemID == 0 and desItemID == 0 ) then
		return
	end
	local armID = srcItemID > 0 and srcItemID or desItemID;

	if armID > 0 then
		local grid	= 0;
		local itemid = 0;
		local minlv = 200; 

		local ArmDef = getItemDef( armID );
		
		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) - 1 do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
			
			if item:getItemId() > 0 and item:getItemType() == ITEM_SLOTTYPE_ITEM then
				local val1,val2 = GetItemResultTypeValue( item:getItemId(), RESULT_ARM_ATTR_MOVE );

				if val1 > 0 and ArmDef.WearPre.RoleLevelMin <= val1 and val1 <= minlv
				 and( ( val2 > 0 and nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH ) or ( val2 < 1 and nCurMoveType == ARM_SKILL_MOVE_UITYPE_LOW ) )then
					minlv = val1;
					grid = i;
					itemid	= item:getItemId();
				end
			end
		end
		if itemid > 0 then
			LockItem( CONTAINER_TYPE_SUNDRIES, grid , frame );
			useBtn:SetClientUserData( 0, CONTAINER_TYPE_SUNDRIES );
			useBtn:SetClientUserData( 1, grid );
			useBtn:SetClientUserData( 2, itemid );
		end
	end
end

function GetItemResultTypeValue( nItemID , nResultType )
	local nValue1 = 0;
	local nValue2 = 0;
	local useItemDef = getItemDef( nItemID );
	if useItemDef ~= nil then 
		for i=1,MAX_ITEM_RESULT do
			if useItemDef.Result[i-1].ResultID == nResultType then
				nValue1 = useItemDef.Result[i-1].ResultVal1;
				nValue2 = useItemDef.Result[i-1].ResultVal2;
			end	
		end
	end
	return nValue1,nValue2;
end

local function Save2ArmArmSkillAttrMoveFrame( item )
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "技能属性转移" then 
		ShowMidTips("请先确定是否进行技能转移，再进行该操作");
		return;
	end
	local srcBtn = getglobal( "ArmSkillAttrMoveFrameSrcBtn" );
	local dstBtn = getglobal( "ArmSkillAttrMoveFrameDstBtn" );
	local useBtn = getglobal( "ArmSkillAttrMoveFrameUseBtn" );
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		local useID = useBtn:GetClientUserData(2);
		-- 装备未鉴定
		if item:isArmIdentity() then 
			ShowMidTips("此装备还未鉴定，不能进行此操作");
			return;
		end
		if GetItemTypeName( math.floor( item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能进行技能属性转移" );
			return;
		end
	
		if useID > 0 then 
			local nLvValue = GetItemResultTypeValue( useID,RESULT_ARM_ATTR_MOVE ) ;
			if srcBtn:GetClientUserData(2) == 0 then
				if nLvValue < item:getItemDef().WearPre.RoleLevelMin then
					ShowMidTips( "装备等级与转移石不符" )
					return ;
				end
				if item:getArmSkillMax() == 0 and dstBtn:GetClientUserData(2) > 0 then
					ShowMidTips( "此装备没有能替换的技能属性" );
					return ;	
				end 
			end
			if dstBtn:GetClientUserData(2) == 0 then 
				if nLvValue < item:getItemDef().WearPre.RoleLevelMin then
					ShowMidTips( "装备等级与转移石不符" )
					return ;
				end
			end
		end
		if nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH and item:getItemDef().Important < 3 then
			ShowMidTips( "请放入紫色品质以上装备" )
			return
		end
		if nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH and item:getItemDef().Important == 7 then
			ShowMidTips( "传奇装备无法进行高级技能属性转移" )
			return
		end
		if srcBtn:GetClientUserData(2) == 0 and item:getArmSkillMax() > 0 then			
			srcBtn:SetClientUserData( 0, item:getListType() )
			srcBtn:SetClientUserData( 1, item:getGridIdx() )
			srcBtn:SetClientUserData( 2, item:getItemId() )
			LockItem( item:getListType(), item:getGridIdx() , "ArmSkillAttrMoveFrame" )		
			SkillAttrMoveAutoUseItem();
			UpdateAllItemBtnInfo();	
			return;
		end
		if dstBtn:GetClientUserData(2) == 0 then
			dstBtn:SetClientUserData( 0, item:getListType() )
			dstBtn:SetClientUserData( 1, item:getGridIdx() )
			dstBtn:SetClientUserData( 2, item:getItemId() )
			LockItem( item:getListType(), item:getGridIdx() , "ArmSkillAttrMoveFrame" )
			SkillAttrMoveAutoUseItem();
			UpdateAllItemBtnInfo();
			return
		end
		ShowMidTips( "更换技能属性替换的装备，请先取回装备栏内的装备" )
	elseif item:getItemType() == ITEM_SLOTTYPE_ITEM then
		local itemdef = item:getItemDef()
		local val1,val2 = GetItemResultTypeValue( itemdef.ItemID,RESULT_ARM_ATTR_MOVE );	
		if val1 == 0 then
			ShowMidTips( "请放入转移石" );
			return;
		end
		if val2 > 0 and nCurMoveType == ARM_SKILL_MOVE_UITYPE_LOW then
			ShowMidTips( "该转移石只能用于高级技能属性转移" );
			return ;
		end
		if nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH and val2 < 1 then 
			ShowMidTips( "该转移石只能用于初级技能属性转移" );
			return ;
		end
		local nEquipId = srcBtn:GetClientUserData(2) > 0 and srcBtn:GetClientUserData(2) or dstBtn:GetClientUserData(2)

		if nEquipId > 0 then
			local armDef = getItemDef( nEquipId );
			if val1 < armDef.WearPre.RoleLevelMin then
				ShowMidTips( "装备等级与转移石不符" )
				return ;
			end
		end
		if useBtn:GetClientUserData(2) > 0 then
			UnLockItem( useBtn:GetClientUserData(0), useBtn:GetClientUserData(1), useBtn:GetClientUserData(2) );--解锁
		end
		useBtn:SetClientUserData( 0, item:getListType() )
		useBtn:SetClientUserData( 1, item:getGridIdx() )
		useBtn:SetClientUserData( 2, item:getItemId() )
		LockItem( item:getListType(), item:getGridIdx() , "ArmSkillAttrMoveFrame" );
		UpdateAllItemBtnInfo();
	end
end
function OpenArmSkillAttrMoveFrame( npcId,nSkillMoveType )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	if ArmSkillAttrMoveFrame:IsShown() then
		ArmSkillAttrMoveFrame:Hide();
	end
	
	nCurMoveType = nSkillMoveType;
	
	ArmSkillAttrMoveFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmSkillAttrMoveFrame:Show();
	util.testNpcDialogDistance( npcId, ArmSkillAttrMoveFrame );
end

function ArmSkillAttrMoveFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ItemFirstCallBack( "ArmSkillAttrMoveFrame" , Save2ArmArmSkillAttrMoveFrame );
end

function ArmSkillAttrMoveFrame_OnEvent()
	if this:IsShown() then
		if arg1 == "GE_CHANGE_MONEY" then
			CheckItemBtnColorState();		
		elseif arg1 == "GE_CONTAINER_UPDATE" then
			UpdateAllItemBtnInfo();
		end
	end
end

function ArmSkillAttrMoveFrame_OnShow()
	local tipRich = getglobal("ArmSkillAttrMoveFrameRichText");
	if nCurMoveType == ARM_SKILL_MOVE_UITYPE_LOW then 
		tipRich:SetText( t_szArmSkillTips[1],255,255,190 );
	elseif nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH then 
		tipRich:SetText( t_szArmSkillTips[2],255,255,190 ) ;
	else 
		ShowMidTips("当前技能属性转移类型不匹配！");
		return;
	end
	for _, name in ipairs( t_btnNameList ) do 
		local btn = getglobal( name );
		btn:SetClientUserData( 0, 0 )
		btn:SetClientUserData( 1, 0 )
		btn:SetClientUserData( 2, 0 )
	end
	ArmSkillAttrMoveFrameCostMoney:SetText( SKILL_MOVE_COST_MONEY );
	UpdateAllItemBtnInfo();
end

function UpdateAllItemBtnInfo()
	local mainplayer = ActorMgr:getMainPlayer();
	local container	 = mainplayer:getContainer();
	for _, name in ipairs( t_btnNameList ) do 
		local btn 	= getglobal( name );
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		local nItemId = btn:GetClientUserData(2)

		local IconTex 	= getglobal( name .. "IconTexture");
		local bindTex	= getglobal( name .. "BindTexture" );
		local uvTex		= getglobal( name .. "UVAnimationTex" );
		local numFont 	= getglobal( name .. "CountFont" );
		numFont:SetText("");
		local btnItem = container:getItem( nList, nGrid );
		if btnItem:getItemId() == 0 then 
			nItemId = 0;
			btn:SetClientUserData( 0,0 );
			btn:SetClientUserData( 1,0 );
			btn:SetClientUserData( 2,0 );
		end
		if nItemId == 0 then 
			if nList > 0 then
				UnLockItem( nList, nGrid, nItemId );--解锁
			end
			IconTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			uvTex:Hide();
			bindTex:Hide();
		else 
			local curItem = container:getItem( nList, nGrid );
			if curItem:getItemId() > 0 and curItem:getItemId() == nItemId then		
				local itemdef = curItem:getItemDef()
				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH
				end
				IconTex:SetTexture( IconPath );
				SetEquipImportantEffectUV( itemdef, uvTex:GetName() ,curItem );
				if curItem:isItemBind() then
					bindTex:Show()
				else
					bindTex:Hide()
				end
			end
		end
		local useFont = getglobal( "ArmSkillAttrMoveFrameUseName" );
		local srcFont = getglobal( "ArmSkillAttrMoveFrameSrcName" );
		local dstFont = getglobal( "ArmSkillAttrMoveFrameDstName" );
		
		local btnType = btn:GetClientID();
		if nItemId > 0 then 
			local curItem = container:getItem( nList, nGrid );
			if curItem:getItemId() > 0 and curItem:getItemId() == nItemId then		
				local itemdef = curItem:getItemDef()
				if btnType == nIndexSrcItem then 
					srcFont:SetText( itemdef.Name )
				elseif btnType == nIndexDstItem then 
					dstFont:SetText( itemdef.Name )
				elseif btnType == nIndexUseItem then 
					useFont:SetText( itemdef.Name )
					numFont:SetText( curItem:getNum() );
				end
			end
		else 
			if btnType == nIndexSrcItem then 
				srcFont:SetText( "替换装备" )
			elseif btnType == nIndexDstItem then 
				dstFont:SetText( "被替换装备" )
			elseif btnType == nIndexUseItem then 
				useFont:SetText( "放入转移石" )
			end
		end
	end
	CheckItemBtnColorState();
end

function ArmSkillAttrMoveFrame_OnHide()
	for _, name in ipairs( t_btnNameList ) do 
		local btn = getglobal( name );
		btn:SetClientUserData( 2,0 )
	end
	UpdateAllItemBtnInfo();
end
-- 转移 item 相关 
function ArmSkillAttrMoveFrameBtn_OnReceiveDrag()
	local btnType = this:GetClientID();
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "技能属性转移" then 
		ShowMidTips("请先确定是否进行技能转移，再进行该操作");
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if btnType == nIndexUseItem then 
		ArmSkillMoveUsedItemReceive();
	elseif btnType == nIndexSrcItem or btnType == nIndexDstItem then 
		ArmSkillMoveEquipItemReceive( btnType );
	end
end
function ArmSkillAttrMoveFrameBtn_OnBeginDrag()
	local btnType = this:GetClientID();
	if this:GetClientUserData(2) > 0 then
		if btnType == nIndexUseItem then 
			UIMSG:getUIGV().DragFrom = DRAG_ARM_SKILL_ATTR_MOVE_USE;
		elseif btnType == nIndexSrcItem then
			UIMSG:getUIGV().DragFrom = DRAG_ARM_SKILL_ATTR_MOVE_SRC;
		elseif btnType == nIndexDstItem then 
			UIMSG:getUIGV().DragFrom = DRAG_ARM_SKILL_ATTR_MOVE_DST;
		else 
			ShowMidTips("当前物品类型不匹配！！")
			return;
		end
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag( IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), this:GetClientUserData(2) );
	end
end

function ArmSkillAttrMoveFrameBtn_OnClick()
	if arg1 == "RightButton" then
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "技能属性转移" then 
			ShowMidTips("请先确定是否进行技能转移，再进行该操作");
			return;
		end
		local nList 	= this:GetClientUserData(0)
		local nGrid 	= this:GetClientUserData(1)
		local nItemId 	= this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
			this:SetClientUserData( 0, 0 )
			this:SetClientUserData( 1, 0 )
			this:SetClientUserData( 2, 0 )
			UpdateAllItemBtnInfo();
		end	
	end
end

function ArmSkillMoveEquipItemReceive( btnType )
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	
	if dragfrom == DRAG_ARM_SKILL_ATTR_MOVE_SRC then
		if btnType == nIndexSrcItem then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
	elseif dragfrom == DRAG_ARM_SKILL_ATTR_MOVE_DST then
		if btnType == nIndexDstItem then 
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
		end
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_ITEM then
			ShowMidTips( "请放入要转移的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能进行技能属性转移" );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		local itemDef = item:getItemDef();
		local flag = ( nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH ) and 1 or 0;	
		if flag > 0 and itemDef.Important < 3 then
			ShowMidTips( "请放入紫色品质以上装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if  flag > 0 and itemDef.Important == 7 then 
			ShowMidTips( "传奇装备无法进行高级技能属性转移" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		local nMax = item:getArmSkillMax();
		if btnType == nIndexSrcItem and nMax == 0  then 
			ShowMidTips( "此装备没有能替换的技能属性" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		local btn = getglobal( "ArmSkillAttrMoveFrameUseBtn" );
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local useItem = container:getItem( nList, nGrid )
			if useItem:getItemId() > 0 then
				local useItemDef = useItem:getItemDef()
				for i=1,MAX_ITEM_RESULT do
					if useItemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
						if itemDef.WearPre.RoleLevelMin > useItemDef.Result[i-1].ResultVal1 then
							ShowMidTips( "装备等级与转移石不符" );
							return ;
						end
						break
					end
				end
			end
		end

		nList = this:GetClientUserData(0);
		nGrid = this:GetClientUserData(1);
		local nItemId = this:GetClientUserData(2);
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx() , "ArmSkillAttrMoveFrame" );
		this:SetClientUserData( 0, srcList );
		this:SetClientUserData( 1, srcGrid );
		this:SetClientUserData( 2, itemDef.ItemID );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		SkillAttrMoveAutoUseItem();
		UpdateAllItemBtnInfo();
	end
end
function ArmSkillMoveUsedItemReceive()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	
	if dragfrom == DRAG_ARM_SKILL_ATTR_MOVE_USE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		if item:getItemType() == ITEM_SLOTTYPE_AMM then
			ShowMidTips( "请放入转移石" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end

		local itemDef = item:getItemDef()
		local val1
		local val2
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_ARM_ATTR_MOVE then
				val1 = itemDef.Result[i-1].ResultVal1
				val2 = itemDef.Result[i-1].ResultVal2
				break
			end
		end
		if val1 == nil or val2 == nil then
			ShowMidTips( "请放入转移石" );
			return ;
		end
		local flag = 0;
		if nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH then
			flag = 1;
		end
		if flag ~= val2 then
			ShowMidTips( string.format( "该转移石只能用于%s技能属性转移", val2 > 0 and "高级" or "初级" ) );
			return ;
		end

		local btn = getglobal( "ArmSkillAttrMoveFrameSrcBtn" );
		local nList = btn:GetClientUserData(0)
		local nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local srcItem = container:getItem( nList, nGrid )
			if srcItem:getItemId() > 0 and val1 < srcItem:getItemDef().WearPre.RoleLevelMin then
				ShowMidTips("装备等级与转移石不符");
				return ;
			end
		end

		btn = getglobal( "ArmSkillAttrMoveFrameDstBtn" );
		nList = btn:GetClientUserData(0)
		nGrid = btn:GetClientUserData(1)
		if nList > 0 then
			local dstItem = container:getItem( nList, nGrid )
			if dstItem:getItemId() > 0 and val1 < dstItem:getItemDef().WearPre.RoleLevelMin then
				ShowMidTips("装备等级与转移石不符");
				return ;
			end
		end

		nList = this:GetClientUserData(0)
		nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( srcList, srcGrid , "ArmSkillAttrMoveFrame" )
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateAllItemBtnInfo();
	end
end
local skillMoveLastTime = 0;
function ArmSkillAttrMoveFrameOKBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getMoney() < SKILL_MOVE_COST_MONEY then
		ShowBuyMoneyFrame();
		ShowMidTips( "银币不足，无法进行技能属性转移" )
		return ;
	end
	local srcBtn = getglobal( "ArmSkillAttrMoveFrameSrcBtn" );
	local dstBtn = getglobal( "ArmSkillAttrMoveFrameDstBtn" );
	local useBtn = getglobal( "ArmSkillAttrMoveFrameUseBtn" );

	if srcBtn:GetClientUserData(2) == 0 or dstBtn:GetClientUserData(2) == 0 then
		return ShowMidTips( "请先放入装备" )
	end

	if nCurMoveType == ARM_SKILL_MOVE_UITYPE_HIGH then
		local srcItemDef = getItemDef( srcBtn:GetClientUserData(2) );
		local dstItemDef = getItemDef( dstBtn:GetClientUserData(2) );
		if srcItemDef.Important < 3 or dstItemDef.Important < 3 then
			return ShowMidTips( "高级技能属性转移只能对紫色品质以上的装备进行转移" )
		end
		if srcItemDef.WearPre.Career ~= dstItemDef.WearPre.Career or srcItemDef.WearPos1 ~= dstItemDef.WearPos1 or
			srcItemDef.WearPre.RoleLevelMin ~= dstItemDef.WearPre.RoleLevelMin then
			return ShowMidTips( "高级技能属性转移只能在同职业，同部位，同等级的装备之间进行转移" )
		end
	else
		if srcBtn:GetClientUserData(2) ~= dstBtn:GetClientUserData(2) then
			return ShowMidTips( "初级技能属性转移只能在同品质，同名称的装备之间进行转移" )
		end
	end

	if useBtn:GetClientUserData(2) == 0 then
		return ShowMidTips( "请先放入转移石" )
	end
	
	local container	= mainplayer:getContainer();
	local srcItem = container:getItem( srcBtn:GetClientUserData(0), srcBtn:GetClientUserData(1) );
	local dstItem = container:getItem( dstBtn:GetClientUserData(0), dstBtn:GetClientUserData(1) );
	
	local idx = CheckeItemImportantConfirmForSpecial( srcItem, "ArmSkillAttrMoveFrame" );
	if idx > 0 then
		ImportantItemConfirmFrameTitle:SetText("装备技能转移确认");
		ShowImportantItemConfirmFrame( "技能属性转移", srcItem, idx, true );
		SetItemWarnFrame("ArmSkillAttrMoveFrame");
		ImportantItemConfirmFrame:SetClientUserData( 0, 0 );
		ImportantItemConfirmFrame:SetClientUserData( 1, 0 );
		ImportantItemConfirmFrame:SetClientUserData( 2, 0 );
		return;
	end
	if srcItem ~= nil and dstItem ~= nil and srcItem:getArmSkillMax() < dstItem:getArmSkillMax() then 
		local szText = "替换成功后，#cff0000技能属性数量会减少#n\n确定替换技能属性？";
		MessageBox( "技能属性转移",szText );
		MessageBoxFrame:SetClientString( "技能属性转移" );
		return;
	end
	if os.clock() - skillMoveLastTime >= 0.1 then
		GameArmProce:requestArmAttrMove( srcBtn:GetClientUserData(0),srcBtn:GetClientUserData(1),
									 dstBtn:GetClientUserData(0),dstBtn:GetClientUserData(1),
									 useBtn:GetClientUserData(0),useBtn:GetClientUserData(1),SKILL_ATTR_MOVE_TYPE );
		skillMoveLastTime = os.clock()
	end
end
function ArmSkillAttrMoveStart()
	local srcBtn = getglobal( "ArmSkillAttrMoveFrameSrcBtn" );
	local dstBtn = getglobal( "ArmSkillAttrMoveFrameDstBtn" );
	local useBtn = getglobal( "ArmSkillAttrMoveFrameUseBtn" );
	GameArmProce:requestArmAttrMove( srcBtn:GetClientUserData(0),srcBtn:GetClientUserData(1),
								 dstBtn:GetClientUserData(0),dstBtn:GetClientUserData(1),
								 useBtn:GetClientUserData(0),useBtn:GetClientUserData(1),SKILL_ATTR_MOVE_TYPE );
end