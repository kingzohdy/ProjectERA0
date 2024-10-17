local MAX_STUFF_NUM = 2
local nLastRequestRecast = 0
local IsOrangeArmRequest = false
local szDesc = [[#cffffbe升级说明：#n
● 可将卓越装备进阶为#cffb9ff史诗套装、英雄套装#n
● 进阶后，将全数保留装备的#cf57300强化星级#n
● 进阶后，将全数保留装备的#cf57300附加属性#n
● 进阶后，将全数保留装备的#cf57300附魔属性#n
● 进阶后，将全数保留装备的#cf57300镶嵌宝石#n]]

local szOrangeDesc = [[#G传奇武器档次提升：#n
●可将部分传奇武器升级提升档次#n
●档次提升后；将全数保留装备的#cf57300强化星级#n
●档次提升后；将全数保留装备的#cf57300镶嵌宝石#n
#G●档次提升后；将重新获取所有的附加属性#n]]
function setIsOrangeArmRequest( flag )
	IsOrangeArmRequest = flag
	if flag then
		ArmLevelUpFrameCaption:SetText( "装备升档" )
		ArmLevelUpFrameRichText:SetText( szOrangeDesc,160,155,130 )
		ArmLevelUpFrameTips4:SetText( "升档后装备" )
		ArmLevelUpFrameOKBtn:SetText( "开始升档" )
	else
		ArmLevelUpFrameCaption:SetText( "装备进阶" )
		ArmLevelUpFrameRichText:SetText( szDesc,160,155,130 )
		ArmLevelUpFrameTips4:SetText( "进阶后装备" )
		ArmLevelUpFrameOKBtn:SetText( "开始进阶" )
		
	end
end
local function ClearFrame()
	local nList = ArmLevelUpFrameArmBtn:GetClientUserData(0)
	local nGrid = ArmLevelUpFrameArmBtn:GetClientUserData(1)
	local nItemId = ArmLevelUpFrameArmBtn:GetClientUserData(2)
	if nList > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end
	ArmLevelUpFrameArmBtn:SetClientUserData( 0, 0 )
	ArmLevelUpFrameArmBtn:SetClientUserData( 1, 0 )
	ArmLevelUpFrameArmBtn:SetClientUserData( 2, 0 )
	ArmLevelUpFrameArmBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
	ArmLevelUpFrameArmBtnUVAnimationTex:Hide()
	ArmLevelUpFrameArmBtnCountFont:SetText("")
	ArmLevelUpFrameArmBtnBindTexture:Hide()
	ArmLevelUpFrameArmBtnBackTexture:Show()
	for i=1,MAX_STUFF_NUM do
		local btn = getglobal( "ArmLevelUpFrameStuff" .. i .. "Btn" )
		local icon = getglobal( btn:GetName() .. "IconTexture" )
		local uvTex = getglobal( btn:GetName() .. "UVAnimationTex" )
		local count = getglobal( btn:GetName() .. "CountFont" )
		local back = getglobal( btn:GetName() .. "BackTexture" )
		icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		uvTex:Hide()
		count:SetText("")
		back:Show()
		btn:SetClientUserData( 0, 0 )
	end
end

local function UpdateItem()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local item = container:getItem( ArmLevelUpFrameArmBtn:GetClientUserData(0), ArmLevelUpFrameArmBtn:GetClientUserData(1) )
	if item:getItemId() > 0 and item:getItemId() == ArmLevelUpFrameArmBtn:GetClientUserData(2) then
		local def = getArmLevelUPDef( item:getItemId() )
		if def == nil or def.NextType > 6 then
			return
		end
		local t_fonts = {	["ArmLevelUpFrameCostMoney"] = { value = def.MoneyVal },
							["ArmLevelUpFrameHaveMoney"] = { value = mainplayer:getMoney(), },
						}
		util.SetFonts( t_fonts )
		
		local itemdef = item:getItemDef()
		local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		ArmLevelUpFrameArmBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemdef, "ArmLevelUpFrameArmBtnUVAnimationTex",item )
		if item:isItemBind() then
			ArmLevelUpFrameArmBtnBindTexture:Show()
		else
			ArmLevelUpFrameArmBtnBindTexture:Hide()
		end
		ArmLevelUpFrameArmBtnBackTexture:Show()

		local num = 1
		for i=1,MAX_STUFF_NUM do
			if def.Stuff[i-1].ItemID > 0 then
				local btn = getglobal( "ArmLevelUpFrameStuff" .. num .. "Btn" )
				local icon = getglobal( btn:GetName() .. "IconTexture" )
				local count = getglobal( btn:GetName() .. "CountFont" )
				local back = getglobal( btn:GetName() .. "BackTexture" )
				local itemDef = getItemDef( def.Stuff[i-1].ItemID )
				local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH
				end
				icon:SetTexture( IconPath )
				SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimationTex" )
				back:Show()
				local haveNum = container:getItemCount( def.Stuff[i-1].ItemID )
				count:SetText( haveNum .. "/" .. def.Stuff[i-1].Num )
				if def.Stuff[i-1].Num > haveNum then
					count:SetTextColor( 255, 0, 0 )
				else
					count:SetTextColor( 204, 128, 51 )
				end
				btn:SetClientUserData( 0, def.Stuff[i-1].ItemID )
				num = num + 1
			end
		end
		for i=num,MAX_STUFF_NUM do			
			local btn = getglobal( "ArmLevelUpFrameStuff" .. i .. "Btn" )
			local icon = getglobal( btn:GetName() .. "IconTexture" )
			local count = getglobal( btn:GetName() .. "CountFont" )
			local back = getglobal( btn:GetName() .. "BackTexture" )
			local itemDef = getItemDef( def.Stuff[i-1].ItemID )
			btn:SetClientUserData( 0, 0 )
			icon:SetTexture( "uires\\ui\\Mask\\TouMingTongDao.tga" )
			back:Show()
			count:SetText( "" )
		end
		itemDef = getItemDef( def.NextItemID )
		if itemDef ~= nil then
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga"
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			ArmLevelUpFrameGetBtnIconTexture:SetTexture( IconPath )
			SetEquipImportantEffectUV( itemDef, "ArmLevelUpFrameGetBtnUVAnimationTex" )
			ArmLevelUpFrameGetBtnBindTexture:Show()
			ArmLevelUpFrameGetBtnBackTexture:Show()
			ArmLevelUpFrameGetBtn:SetClientUserData( 3, def.NextItemID )
		else
			ArmLevelUpFrameGetBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
			ArmLevelUpFrameGetBtnUVAnimationTex:Hide()
			ArmLevelUpFrameGetBtnBindTexture:Hide()
			ArmLevelUpFrameGetBtnBackTexture:Show()
			ArmLevelUpFrameGetBtn:SetClientUserData( 0, 0 )
			ArmLevelUpFrameGetBtn:SetClientUserData( 1, 0 )
			ArmLevelUpFrameGetBtn:SetClientUserData( 2, 0 )
			ArmLevelUpFrameGetBtn:SetClientUserData( 3, 0 )
		end
	else
		ClearFrame()
		local t_fonts = {	["ArmLevelUpFrameCostMoney"] = { value = 0 },
							["ArmLevelUpFrameHaveMoney"] = { value = mainplayer:getMoney(), },
						}
		util.SetFonts( t_fonts )
		local upItem = container:getItem( ArmLevelUpFrameGetBtn:GetClientUserData(0), ArmLevelUpFrameGetBtn:GetClientUserData(1) )
		if upItem:getItemId() > 0 and upItem:getItemId() == ArmLevelUpFrameGetBtn:GetClientUserData(2) then
			local itemdef = upItem:getItemDef()
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga"
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH
			end
			ArmLevelUpFrameGetBtnIconTexture:SetTexture( IconPath )
			SetEquipImportantEffectUV( itemdef, "ArmLevelUpFrameGetBtnUVAnimationTex", upItem )
			if upItem:isItemBind() then
				ArmLevelUpFrameGetBtnBindTexture:Show()
			else
				ArmLevelUpFrameGetBtnBindTexture:Hide()
			end
			ArmLevelUpFrameArmBtnBackTexture:Show()
		else
			ArmLevelUpFrameGetBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
			ArmLevelUpFrameGetBtnUVAnimationTex:Hide()
			ArmLevelUpFrameGetBtnBindTexture:Hide()
			ArmLevelUpFrameGetBtnBackTexture:Show()
			ArmLevelUpFrameGetBtn:SetClientUserData( 0, 0 )
			ArmLevelUpFrameGetBtn:SetClientUserData( 1, 0 )
			ArmLevelUpFrameGetBtn:SetClientUserData( 2, 0 )
			ArmLevelUpFrameGetBtn:SetClientUserData( 3, 0 )
		end
	end
end

function Save2ArmLevelUpFrame( item )
	local szKeyWorld = "进阶"
	if IsOrangeArmRequest then
		szKeyWorld = "升档"		
	end
	if item:getItemType() == ITEM_SLOTTYPE_ITEM then
		ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
		return
	end
	local def = getArmLevelUPDef( item:getItemId() )
	if def == nil or def.NextType > 6 then
		ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
		return
	end
	local itemDef = item:getItemDef()
	if IsOrangeArmRequest then
		if itemDef.Important ~= 7 then
			ShowMidTips("非橙色装备不可放入")
			return;
		end
		--[[
	else
		if itemDef.Important == 7 then
			ShowMidTips("橙色装备不可放入");
			return;
		end
		--]]
	end
	if item:isArmIdentity() then 
		ShowMidTips("此装备还未鉴定，不能进行此操作");
		return;
	end
	local btn = ArmLevelUpFrameArmBtn
	local nList = btn:GetClientUserData(0)
	local nGrid = btn:GetClientUserData(1)
	local nItemId = btn:GetClientUserData(2)
	if nList > 0 and nItemId > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end

	btn:SetClientUserData( 0, item:getListType() )
	btn:SetClientUserData( 1, item:getGridIdx() )
	btn:SetClientUserData( 2, item:getItemId() )
	LockItem( item:getListType(), item:getGridIdx() , "ArmLevelUpFrame" )
	btn = ArmLevelUpFrameGetBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	UpdateItem()
end

function OnArmLevelSuccess( item )
	if item ~= nil then
		btn = ArmLevelUpFrameGetBtn
		btn:SetClientUserData( 0, item:getListType() )
		btn:SetClientUserData( 1, item:getGridIdx() )
		btn:SetClientUserData( 2, item:getItemId() )
		btn:SetClientUserData( 3, 0 )
		UpdateItem()

		local modelview = getglobal( "ArmLevelUpFrameModelView" )
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
	end
end

function ArmLevelUpFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	ArmLevelUpFrameRichText:SetText( szDesc,160,155,130 )
	ItemFirstCallBack( "ArmLevelUpFrame" , Save2ArmLevelUpFrame );
end

function ArmLevelUpFrame_OnEvent()
	if this:IsShown() then
		if arg1 == "GE_CONTAINER_UPDATE" or arg1 == "GE_CHANGE_MONEY" then
			UpdateItem()
		end
	end
end

function ArmLevelUpFrame_OnShow()
	local btn = ArmLevelUpFrameGetBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	UpdateItem()
end

function ArmLevelUpFrame_OnHide()
	local btn = ArmLevelUpFrameArmBtn
	local nList = btn:GetClientUserData(0)
	local nGrid = btn:GetClientUserData(1)
	local nItemId = btn:GetClientUserData(2)
	if nList > 0 and nItemId > 0 then
		UnLockItem( nList, nGrid, nItemId );--解锁
	end
	btn = ArmLevelUpFrameGetBtn
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	ClearFrame()
end

function ArmLevelUpFrameTipsBtn_OnEnter()
	SetGameTooltips( this:GetName(), "装备升级后，装备将与您绑定\n每次装备升级需要小号一个与待升级装备等级相应的战勋\n每次装备升级需要消耗2个与待升级装备等级相应的紫色水晶" );
end

function ArmLevelUpFrameArmBtn_OnBeginDrag()
	if this:GetClientUserData(2) > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_LEVEL;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , this:GetClientUserData(0), this:GetClientUserData(1), this:GetClientUserData(2) );
	end
end

function ArmLevelUpFrameArmBtn_OnReceiveDrag()
	local dragfrom, dragdata1, dragdata2, dragdata3 = UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();

	if dragfrom == DRAG_ARM_LEVEL then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragfrom
		local srcGrid		=	dragdata1
		local item = container:getItem( srcList, srcGrid )
		local szKeyWorld = "进阶"
		if IsOrangeArmRequest then
			szKeyWorld = "升档"		
		end
		if item:getItemType() == ITEM_SLOTTYPE_ITEM then
			ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end
		if item:isArmIdentity() then 
			ShowMidTips("此装备还未鉴定，不能进行此操作");
			return;
		end
		local itemDef = item:getItemDef()
		-- 判断能否放进去
		local def = getArmLevelUPDef( itemDef.ItemID )
		if def == nil or def.NextType > 6 then
			ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return
		end

		local nList = this:GetClientUserData(0)
		local nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx() , "ArmLevelUpFrame" )
		this:SetClientUserData( 0, srcList )
		this:SetClientUserData( 1, srcGrid )
		this:SetClientUserData( 2, itemDef.ItemID )
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE

		local btn = ArmLevelUpFrameGetBtn
		btn:SetClientUserData( 0, 0 )
		btn:SetClientUserData( 1, 0 )
		btn:SetClientUserData( 2, 0 )
		UpdateItem()
	end
end

function ArmLevelUpFrameArmBtn_OnClick()
	if arg1 == "RightButton" then
		local nList = this:GetClientUserData(0)
		local nGrid = this:GetClientUserData(1)
		local nItemId = this:GetClientUserData(2)
		if nList > 0 and nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
			this:SetClientUserData( 0, 0 )
			this:SetClientUserData( 1, 0 )
			this:SetClientUserData( 2, 0)
			UpdateItem()
		end
	end
end

function ArmLevelUpFrameArmBtn_OnEnter()
	if this:GetClientUserData( 2 ) > 0 then
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) )
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item )
	end
end

function ArmLevelUpFrameStuffBtn_OnEnter()
	if this:GetClientUserData( 0 ) > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( this:GetClientUserData( 0 ) ) )
	end
end

function ArmLevelUpFrameGetBtn_OnEnter()
	if this:GetClientUserData(3) > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( this:GetClientUserData( 3 ) ) )
	else
		ArmLevelUpFrameArmBtn_OnEnter()
	end
end

function ArmLevelUpFrameOKBtn_OnClick()
	local nList = ArmLevelUpFrameArmBtn:GetClientUserData( 0 )
	local nGrid = ArmLevelUpFrameArmBtn:GetClientUserData( 1 )
	local szKeyWorld = "进阶"
	if IsOrangeArmRequest then
		szKeyWorld = "升档"		
	end
	if nList == 0 then
		ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
		return
	end
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local nonegrid 	= container:getNoneGridIndex(CONTAINER_TYPE_SUNDRIES );
	if nonegrid == -1 then
		ShowMidTips("背包已满，请清理背包后再进行"..szKeyWorld.."！");
		return;
	end
	local item = container:getItem( nList, nGrid )
	if item:getItemId() == 0 then
		ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
		return
	end
	local def = getArmLevelUPDef( item:getItemId() )
	if def == nil or def.NextType > 6 then
		ShowMidTips( "请放入可"..szKeyWorld.."的装备" )
		return
	end
	if getPlayerMoney( mainplayer ,def.MoneyType ) < def.MoneyVal then
		ShowMidTips( GetMoneyDesc( def.MoneyType ) .. "不足，无法"..szKeyWorld )
		if def.MoneyType == ATTR_ID_MONEY then
			ShowBuyMoneyFrame();
		end
		return
	end
	for i=1,MAX_STUFF_NUM do
		if def.Stuff[i-1].ItemID > 0 and container:getItemCount( def.Stuff[i-1].ItemID ) < def.Stuff[i-1].Num then
			ShowMidTips( getItemDef(def.Stuff[i-1].ItemID).Name .. "不足,无法"..szKeyWorld )
			return
		end
	end
	if os.clock() - nLastRequestRecast > 0.5 then
		nLastRequestRecast = os.clock()
		GameArmProce:armAddLevel( item:getListType(), item:getGridIdx() )
	end
end