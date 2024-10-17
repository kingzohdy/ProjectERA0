
local OPERATION_NONE		= -1
local OPERATION_BUILD		= 0
local OPERATION_ENTER		= 1
local OPERATION_DESTROY		= 2
local OPERATION_SELL		= 3
local OPERATION_BUY		= 4

local PET_OP_NONE		= -1
local PET_OP_FEED		= 1
local PET_OP_CLEAN		= 2
local PET_OP_PLAY		= 3
local PET_OP_EXERCISE		= 4

local TITLE_DEED_ID		= 5070100
local TITLE_DEED_NAME		= "地契"

local MAX_HOMELAND_PAGE_NUM	= 10
local nCurrentPage		= 0
local selectedIndex		= 1
local selectedHome		= nil
local t_HomelandStatus		= { "关门", "开门", }
local t_Status			= { [ HOME_STAT_OPEN_CLIENT ] = "开门", [ HOME_STAT_CLOSE_CLIENT ] = "关门", [ HOME_STAT_SALE_CLIENT ] = "出售/开门", 
					[ "关门" ] = HOME_STAT_CLOSE_CLIENT, [ "开门" ] = HOME_STAT_OPEN_CLIENT, [ "出售/开门" ] = HOME_STAT_SALE_CLIENT, }

local modelAngle		= 0
local scrollIndex		= 1
local MAX_MESSAGEBORD_LIST_NUM	= 4
local t_Message			= { }
local t_Operation		= OPERATION_NONE
local t_petOp			= PET_OP_NONE
local t_petEffect = {
	10420,
	10421,
	10424,
	10423,
}
local t_petID = 0
local t_FontColor = {
			{ r = 255, g = 254 , b = 189 },
			{ r = 255, g = 132 , b = 153 },
			{ r = 255, g = 150 , b = 25  },
			{ r = 255, g = 0   , b = 0   },	}

local function plant( idx, gidx )
	if idx and gidx then
		GameHomelandManager:requestHomePlant( idx, gidx )
	end
end

local function destroy( id )
	if id then
		GameHomelandManager:requestHomeDestroy( id )
		t_Operation = OPERATION_DESTROY
	end
end

local function buy( id )
	if id then
		GameHomelandManager:requestHomeBuy( id )
		t_Operation = OPERATION_BUY
	end
end

local szHomelandTips	= { 
	[ "开垦土地" ] = { [ 1 ] = [[开垦当前土地需要菜地等级：%d级
银币：%d
是否确认开垦 ]], [ 2 ] = plant },

	[ "拆除家园" ]	= { [ 1 ] = [[是否确定拆除家园，拆除后，将不
再拥有此家园。]], [ 2 ] = destroy },
	
	[ "购买家园" ]	= { [ 1 ] = [[是否确认购买此家园，
消耗%d银币和一个地契。]], [ 2 ] = buy },
 }
 
local function getPworldName( id )
	if not id then
		return ""
	end

	local pworldDef = getPworldDef( id )
	return pworldDef ~= nil and pworldDef.Name or ""
end

local function getHomelandStatus( home )
	if home then
		return home.Info.Sale > 0 and t_Status[ HOME_STAT_SALE_CLIENT ] or t_Status[ home.Info.Stat ]
	end
	return ""
end

function playPetEffect( id )
	if not id then
		return
	end
	local modelview = getglobal( "PetHomelandFrame_ModelBtnView" )
	modelview:playEntityAnim( 0, id, nil )
	GameHomelandManager:playPetAction( id )
end

local function SetHomelandPage( pageInfo )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if not pageInfo or pageInfo.Page ~= nCurrentPage or pageInfo.Num > MAX_HOMELAND_PAGE_NUM then
		return
	end
	
	InquireHomelandFramePageFont:SetText( "当前家园分页：" .. nCurrentPage + 1 .. "/" .. ( pageInfo.CountPage == 0 and 1 or pageInfo.CountPage ) )
	InquireHomelandFrameBuiltNumFont:SetText( "已建造数量：" .. pageInfo.CountHome .. "/" .. HOME_CLT_MAX_TOLUA )

	if pageInfo.Num <= 0 then
		InquireHomelandFrame_InfoBtn:Hide()
	else
		InquireHomelandFrame_InfoBtn:Show()
	end

	if pageInfo.CountHome >= HOME_CLT_MAX_TOLUA then
		InquireHomelandFrame_BuildBtn:Disable()
	else
		InquireHomelandFrame_BuildBtn:Enable()
	end

	for i = 1, pageInfo.Num do
		local roleName	= pageInfo.Homes[ i - 1 ].RoleName
		local homeInfo	= pageInfo.Homes[ i - 1 ].Info
		local btn	= getglobal( "InquireHomelandFrame_ListInfoBtnList" .. i )
		local name	= getglobal( btn:GetName() .. "NameFont" )
		local status	= getglobal( btn:GetName() .. "StatusFont" )
		local nStat	= homeInfo.Sale > 0 and HOME_STAT_SALE_CLIENT or homeInfo.Stat

		local nColorIndex = 1;
		--根据和主角的好友关系 改变颜色
		local friendInfo = GameFriendManager:getFriendInfoByName( roleName );
		if friendInfo ~= nil then
			if friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_FRIEND then
				nColorIndex = 2;
			elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_SU_DI then
				nColorIndex = 4;
			elseif friendInfo.m_nRelationType == CLIENT_RELATION_TYPE_XIONG_SHOU then
				nColorIndex = 4;
			end
		end
		--主角颜色
		if roleName == mainplayer:getName() then
			nColorIndex = 3;
		end

		name:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b )
		name:SetText( homeInfo.Name )
		status:SetTextColor( t_FontColor[nColorIndex].r,t_FontColor[nColorIndex].g,t_FontColor[nColorIndex].b )
		status:SetText( t_Status[ nStat ] )
		btn:SetClientUserData( 0, nStat )
		btn:Enable()
		btn:DisChecked()
		btn:Show()
	end

	if pageInfo.Num < MAX_HOMELAND_PAGE_NUM then
		for i = pageInfo.Num + 1, MAX_HOMELAND_PAGE_NUM do
			local btn	= getglobal( "InquireHomelandFrame_ListInfoBtnList" .. i )
			local name	= getglobal( btn:GetName() .. "NameFont" )
			local status	= getglobal( btn:GetName() .. "StatusFont" )

			name:SetText( "" )
			status:SetText( "" )
			btn:DisChecked()
			btn:Hide()
		end
	end
	local btn = getglobal( "InquireHomelandFrame_ListInfoBtnList" .. selectedIndex )
	btn:Checked()
	SetHomelandOwnerInfo( selectedIndex )
end

local function AutoBindTitleDeed()
	local mainplayer	 = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container		= mainplayer:getContainer();
	local btn		= getglobal( "PurchaseHomelandFrameItemBtn" );
	local iconTexture	= getglobal( btn:GetName() .. "IconTexture" );

	for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local item		= container:getItem( CONTAINER_TYPE_SUNDRIES, i );
		local itemId		= item:getItemId();
		if itemId == TITLE_DEED_ID then
			if itemId > 0 then
				UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--解锁
			end
			LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), btn:GetName() );
			btn:SetClientUserData( 0, CONTAINER_TYPE_SUNDRIES );
			btn:SetClientUserData( 1, i );
			btn:SetClientUserData( 2, TITLE_DEED_ID );
			local itemdef	= item:getItemDef();
			iconTexture:SetTexture( GetItemEquipIconPath() .. itemdef.IconID .. ".tga" );
			break;
		end
	end
end

local function SetHomelandInfo_Purchase( fitment )
	if not fitment then
		return
	end
	PurchaseHomelandFrameFitmentMoney:SetText( fitment )
end

function PurchaseHomelandFrame_OnShow()
	SetHomelandInfo_Purchase( this:GetClientUserData( 1 ) )
	AutoBindTitleDeed();
end

function PurchaseHomelandFrame_OnHide()
	local mainplayer	 = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container		= mainplayer:getContainer();
	local btn			= getglobal("PurchaseHomelandFrameItemBtn");
	local nListType		= btn:GetClientUserData( 0 );
	local nGridIdx		= btn:GetClientUserData( 1 );
	local srcitem		= container:getItem( nListType, nGridIdx );
	if srcitem == nil then return end
	local nItemId		= srcitem:getItemId();
	if nItemId > 0 and nItemId == TITLE_DEED_ID then
		UnLockItem( nListType, nGridIdx, nItemId );--解锁
	end
	PurchaseHomelandFrameItemBtnIconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
end

function hasTitleDeed( btnName )
	local btn = getglobal( btnName )
	if not btn then
		return false
	end
	local mainplayer	 = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container		= mainplayer:getContainer();
	local nListType		= btn:GetClientUserData( 0 );
	local nGridIdx		= btn:GetClientUserData( 1 );
	local srcitem		= container:getItem( nListType, nGridIdx );
	if srcitem == nil then return false end
	local nItemId		= srcitem:getItemId();
	if nItemId > 0 and nItemId == TITLE_DEED_ID then
		return true
	end
	return false
end

function PurchaseHomelandFrame_ConfirmBtn_OnClick()
	if not hasTitleDeed( "PurchaseHomelandFrameItemBtn" ) then
		ShowMidTips( "请放入地契" )
		return
	end
	
	local key = PurchaseHomelandFrame:GetClientString()
	if key == "purchase" then
		local id 	= PurchaseHomelandFrame:GetClientUserData( 0 )
		local title	= getglobal( "SecondHomelandFrameTitleFont" )
		local rich	= getglobal( "SecondHomelandFrameContent" )
		
		title:SetText( "购买家园" )
		rich:setCenterLine( true )
		rich:SetText( string.format( szHomelandTips[ "购买家园" ][ 1 ], PurchaseHomelandFrame:GetClientUserData( 1 ) ),
										255, 255, 190 )
		PurchaseHomelandFrame:Hide()
		SecondHomelandFrame:SetClientUserData( 0, PurchaseHomelandFrame:GetClientUserData( 0 ) )
		SecondHomelandFrame:Show()
	elseif key == "build" then
		GameHomelandManager:requestHomeNew()
		PurchaseHomelandFrame:Hide()
		t_Operation = OPERATION_BUILD
	end
end

function PurchaseHomelandFrame_CancelBtn_OnClick()
	PurchaseHomelandFrame:Hide()
end

function PurchaseHomelandFrameItemBtn_OnClick()
	if arg1 == "RightButton" then
		local nListType = this:GetClientUserData( 0 );
		local nGridIdx	= this:GetClientUserData( 1 );
		ClearItemLock( this:GetName() )
		this:SetClientUserData( 0,0 )
		this:SetClientUserData( 1,0 )
		this:SetClientUserData( 2,0 )
		this:SetClientUserData( 3,0 )
	end
end

function PurchaseHomelandFrameItemBtn_OnReceiveDrag()
	PurchaseHomelandFrameItemBtn_OnReceiveDrag_implement( { name = this:GetName(), dragFrom = UIMSG:getUIGV().DragFrom, data1 = UIGetDragUserData1(), data2 = UIGetDragUserData2(), data3 = UIGetDragUserData3() } );
end

function isTitleDeed( itemdef )
	if itemdef then
		if itemdef.ItemID == TITLE_DEED_ID or string.find( itemdef.Name, TITLE_DEED_NAME ) then
			return true;
		end
	end
	return false;
end

function PurchaseHomelandFrameItemBtn_OnReceiveDrag_implement( rec )
	if rec.dragFrom ~= CONTAINER_TYPE_SUNDRIES then
		ShowMidTips("只有背包内物品才可进行此操作");
		return;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container		= mainplayer:getContainer();
	local srcList		= rec.dragFrom;						-- 源容器类型
	local srcGrid		= rec.data1;					-- 从源容器哪个格子过来
	local item			= container:getItem( srcList, srcGrid );
	local itemdef		= item:getItemDef();
	local btnName		= rec.name
	local btn			= getglobal( btnName )

	if not isTitleDeed( itemdef ) then
		ShowMidTips("请放入地契");
		return;
	end

	local nListType = btn:GetClientUserData( 0 );
	local nGridIdx	= btn:GetClientUserData( 1 );
	local dstitem	= container:getItem( nListType,nGridIdx );			
	local nItemId	= dstitem:getItemId();
	if nItemId > 0 then
		UnLockItem( nListType, nGridIdx, nItemId );--解锁
	end
	nItemId = item:getItemId()
	if nItemId > 0 then
		UnLockItem( srcList, srcGrid, nItemId );--解锁
	end
	LockItem( srcList, srcGrid, nItemId, btnName );
	btn:SetClientUserData( 0, srcList )
	btn:SetClientUserData( 1, srcGrid )
	btn:SetClientUserData( 2, nItemId )
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
	PurchaseHomelandFrameItemBtnIconTexture:SetTexture( GetItemEquipIconPath()..itemdef.IconID..".tga" );
end

function OpenInquireFrame( msgid )
	GameHomelandManager:requestHomePage( nCurrentPage )
end

function InquireHomelandFrame_OnLoad()
	this:RegisterEvent( "GE_HOMELAND_PAGE" )
	this:RegisterEvent( "GE_HOMELAND_SINGLE" )
	this:RegisterEvent( "GE_HOMELAND_NAMECHG" )
	this:RegisterEvent( "GE_HOMELAND_STATCHG" )
	this:RegisterEvent( "GE_HOMELAND_LIUYANCHG" )
	this:RegisterEvent( "GE_HOMELAND_SALECHG" )
	this:RegisterEvent( "GE_HOMELAND_BUY" )
	this:RegisterEvent( "GE_HOMELAND_DEL" )
	this:RegisterEvent( "GE_HOMELAND_PET" )
	this:RegisterEvent( "GE_HOMELAND_HOUSE" )
	this:RegisterEvent( "GE_HOMELAND_FARM" )
end

function InquireHomelandFrame_OnShow()
	InquireHomelandFrame_InfoBtnWelcome:SetText( "家园寄语：欢迎来访", 52, 182, 17 )
	selectedIndex = 1
end

function InquireHomelandFrame_OnEvent()
	if arg1 == "GE_HOMELAND_PAGE" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
		if not GameHomelandManager:isInHome() and not InquireHomelandFrame:IsShown() then
			InquireHomelandFrame:Show()
		end
	elseif arg1 == "GE_HOMELAND_NAMECHG" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
		ShowMidTips( "修改家园名称成功" )
	elseif arg1 == "GE_HOMELAND_SINGLE" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
		if t_Operation == OPERATION_BUILD then
			MessageBoxOK( "建造家园完成", "恭喜你获得此家园的所有权" )
			GameHomelandManager:requestHomePage( nCurrentPage )
			t_Operation = OPERATION_NONE
		else
			CloseHomelandFrames()
			GameHomelandManager:displayHomelandPet( 7868, 4819 )
		end
	elseif arg1 == "GE_HOMELAND_STATCHG" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
		ShowMidTips( "修改家园状态成功" )
	elseif arg1 == "GE_HOMELAND_LIUYANCHG" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
	elseif arg1 == "GE_HOMELAND_SALECHG" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
	elseif arg1 == "GE_HOMELAND_BUY" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		SetHomelandPage( pageInfo )
		GameHomelandManager:requestHomePage( nCurrentPage )
		ShowMidTips( "购买家园成功" )
	elseif arg1 == "GE_HOMELAND_DEL" then
		local pageInfo = GameHomelandManager:getHomeSvrPage()
		selectedIndex = selectedIndex > pageInfo.Num and pageInfo.Num or selectedIndex
		if selectedIndex < 1 then
			nCurrentPage = nCurrentPage - 1 >= 0 and nCurrentPage - 1 or nCurrentPage
			selectedIndex = 1
		end
		GameHomelandManager:requestHomePage( nCurrentPage )
		--SetHomelandPage( pageInfo )
		ShowMidTips( "拆除家园成功" )
	elseif arg1 == "GE_HOMELAND_PET" then
		UpdateHomelandPetInfo()
		if t_petEffect[ t_petOp ] then
			local id = t_petEffect[ t_petOp ]
			playPetEffect( id )
		end
		t_petOp = PET_OP_NONE
	elseif arg1 == "GE_HOMELAND_HOUSE" then
		--do something here
	elseif arg1 == "GE_HOMELAND_FARM" then
		--do something here
	end
end

function InquireHomelandFrame_OnHide()
	nCurrentPage = 0
end

local function SetHomeland_OwnerInfo( home )
	InquireHomelandFrame_InfoBtnSaleTex:Hide()
	InquireHomelandFrame_InfoBtnEditBkgTex:Hide()
	InquireHomelandFrame_InfoBtnPriceFont:Hide()
	InquireHomelandFrame_InfoBtnMoneyType:Hide()
	InquireHomelandFrame_InfoBtn_RecallBtn:Hide()

	InquireHomelandFrame_InfoBtn_SetNameBtn:Show()
	InquireHomelandFrame_InfoBtn_SetStatusBtn:Show()
	if home then
		InquireHomelandFrame_InfoBtnStatusFont:SetText( "家园状态：" .. getHomelandStatus( home ) )
	end
	InquireHomelandFrame_InfoBtn_EnterBtn:SetText( "进入")
	InquireHomelandFrame_InfoBtn_EnterBtn:Show()
end

local function SetHomeland_OtherInfo( home )
	InquireHomelandFrame_InfoBtnSaleTex:Hide()
	InquireHomelandFrame_InfoBtnEditBkgTex:Hide()
	InquireHomelandFrame_InfoBtnPriceFont:Hide()
	InquireHomelandFrame_InfoBtnMoneyType:Hide()
	InquireHomelandFrame_InfoBtn_RecallBtn:Hide()

	InquireHomelandFrame_InfoBtn_SetNameBtn:Hide()
	InquireHomelandFrame_InfoBtn_SetStatusBtn:Hide()
	if home then
		InquireHomelandFrame_InfoBtnStatusFont:SetText( "家园状态：" .. getHomelandStatus( home ) )
	end
	if home and home.Info.Stat == HOME_STAT_CLOSE_CLIENT then
		InquireHomelandFrame_InfoBtn_EnterBtn:Hide()
	else
		InquireHomelandFrame_InfoBtn_EnterBtn:SetText( "进入")
		InquireHomelandFrame_InfoBtn_EnterBtn:Show()
	end
end

local function SetHomeland_Buy( home )
	InquireHomelandFrame_InfoBtn_RecallBtn:Hide()
	InquireHomelandFrame_InfoBtn_SetNameBtn:Hide()
	InquireHomelandFrame_InfoBtn_SetStatusBtn:Hide()

	InquireHomelandFrame_InfoBtnSaleTex:Show()
	InquireHomelandFrame_InfoBtnEditBkgTex:Show()
	if home then
		InquireHomelandFrame_InfoBtnStatusFont:SetText( "家园状态：" .. getHomelandStatus( home ) )
	end
	InquireHomelandFrame_InfoBtnPriceFont:SetText( "出售金额：" .. ( home.Info ~= nil and home.Info.Sale or "0" ) )
	InquireHomelandFrame_InfoBtnPriceFont:Show()
	InquireHomelandFrame_InfoBtnMoneyType:Show()
	InquireHomelandFrame_InfoBtn_EnterBtn:SetText( "购买")
	InquireHomelandFrame_InfoBtn_EnterBtn:Show()
end

local function SetHomeland_Sale( home )
	InquireHomelandFrame_InfoBtnEditBkgTex:Hide()
	InquireHomelandFrame_InfoBtnPriceFont:Hide()
	InquireHomelandFrame_InfoBtnMoneyType:Hide()
	InquireHomelandFrame_InfoBtn_EnterBtn:Hide()
	InquireHomelandFrame_InfoBtn_SetNameBtn:Hide()
	InquireHomelandFrame_InfoBtn_SetStatusBtn:Hide()

	if home then
		InquireHomelandFrame_InfoBtnStatusFont:SetText( "家园状态：" .. getHomelandStatus( home ) )
	end

	InquireHomelandFrame_InfoBtnSaleTex:Show()
	InquireHomelandFrame_InfoBtn_RecallBtn:Show()
end

function IsSelfHome( homeInfo )
	local mainplayer = ActorMgr:getMainPlayer()
	if not homeInfo or not mainplayer then
		return false
	end

	return homeInfo.RoleName == mainplayer:getName()
end

local function EnableOwnerFunction()
	local destroy	= getglobal( "InquireHomelandFrame_DestroyBtn")
	local sale	= getglobal( "InquireHomelandFrame_SaleBtn" )
	local inquire	= getglobal( "InquireHomelandFrame_InquireBtn" )

	destroy:Enable()
	sale:Enable()
	inquire:Enable()
end

local function DisableOwnerFunction()
	local destroy	= getglobal( "InquireHomelandFrame_DestroyBtn")
	local sale	= getglobal( "InquireHomelandFrame_SaleBtn" )
	local inquire	= getglobal( "InquireHomelandFrame_InquireBtn" )

	destroy:Disable()
	sale:Disable()
	inquire:Disable()
end

function SetHomelandOwnerInfo( index )
	local pageInfo = GameHomelandManager:getHomeSvrPage()
	if not index or index < 1 or index > pageInfo.Num then
		return
	end

	local btn		= getglobal( "InquireHomelandFrame_ListInfoBtnList" .. index )
	local home		= pageInfo.Homes[ index - 1 ]
	local name		= getglobal( "InquireHomelandFrame_InfoBtnNameFont" )
	local owner		= getglobal( "InquireHomelandFrame_InfoBtnOwnerFont" )
	local room		= getglobal( "InquireHomelandFrame_InfoBtnRoomFont")
	local level		= getglobal( "InquireHomelandFrame_InfoBtnLevelFont")
	local farmLevel	= getglobal( "InquireHomelandFrame_InfoBtnFarmLevelFont" )
	local petLevel	= getglobal( "InquireHomelandFrame_InfoBtnPetLevelFont" )
	
	name:SetText( "家园名称：" .. home.Info.Name )
	owner:SetText( "园主名称：" .. home.RoleName )
	room:SetText(  "房间序号：" .. home.HomeID )
	level:SetText( "家园等级：" .. home.Lvl )
	farmLevel:SetText( "菜园等级：" .. home.Info.Siol.Lvl )
	petLevel:SetText( "小宠物等级：" .. home.Info.Pet.Lvl )

	local bSelfHome = IsSelfHome( home )
	local nStatus	= btn:GetClientUserData( 0 )

	selectedHome = home
	
	if not bSelfHome then
		if nStatus == HOME_STAT_SALE_CLIENT then
			SetHomeland_Buy( selectedHome )
		else
			SetHomeland_OtherInfo( selectedHome )
		end
		DisableOwnerFunction()
	else
		if nStatus == HOME_STAT_SALE_CLIENT then
			SetHomeland_Sale( selectedHome )
		else
			SetHomeland_OwnerInfo( selectedHome )
		end
		EnableOwnerFunction()
	end
	selectedIndex = index
end

function HomelandOwnerBtn_OnClick()
	if this:IsChecked() then
		return
	end

	for i = 1, HOME_PAGE_MAX_TOLUA do
		local btn = getglobal( "InquireHomelandFrame_ListInfoBtnList" .. i )
		btn:DisChecked()
	end

	this:Checked()
	SetHomelandOwnerInfo( this:GetClientID() )
end

function InquireHomelandFrame_FirstPageBtn_OnClick()
	nCurrentPage = 0
	GameHomelandManager:requestHomePage( nCurrentPage )
end

function InquireHomelandFrame_LastPageBtn_OnClick()
	local pageInfo 	= GameHomelandManager:getHomeSvrPage()
	nCurrentPage 	= pageInfo.CountPage - 1
	GameHomelandManager:requestHomePage( nCurrentPage )
end

function InquireHomelandFrame_PreviousPageBtn_OnClick()
	nCurrentPage = nCurrentPage - 1
	if nCurrentPage < 0 then
		nCurrentPage = 0
	end
	GameHomelandManager:requestHomePage( nCurrentPage )
end

function InquireHomelandFrame_NextPageBtn_OnClick()
	local pageInfo = GameHomelandManager:getHomeSvrPage()
	nCurrentPage = nCurrentPage + 1
	if nCurrentPage >= pageInfo.CountPage then
		nCurrentPage = pageInfo.CountPage - 1
	end
	GameHomelandManager:requestHomePage( nCurrentPage )
end

local function EnterHomeland( homeInfo )
	if homeInfo then
		GameHomelandManager:requestHomeEnter( homeInfo.HomeID )
		t_Operation = OPERATION_ENTER
	end
end

local function PurchaseHomeland( homeInfo )
	if homeInfo then
		PurchaseHomelandFrame_HeadFont:SetText( "购买他人家园" )
		PurchaseHomelandFrame:SetClientString( "purchase" )
		PurchaseHomelandFrame:SetClientUserData( 0, selectedHome.HomeID )
		PurchaseHomelandFrame:SetClientUserData( 1, selectedHome.Info.Sale )
		PurchaseHomelandFrameFitment:SetText( "购买家园需要消耗银币：" )
		PurchaseHomelandFrame_ConfirmBtn:SetText( "确认购买" )
		PurchaseHomelandFrame_CancelBtn:SetText( "取消购买" )
		PurchaseHomelandFrameItemBtn:SetClientUserData( 0, 0 )
		PurchaseHomelandFrameItemBtn:SetClientUserData( 1, 0 )
		PurchaseHomelandFrameItemBtn:SetClientUserData( 2, 0 )
		PurchaseHomelandFrameItemBtn:SetClientUserData( 3, 0 )
		InquireHomelandFrame:Hide()
		PurchaseHomelandFrame:Show()
	end
end

local function BuildHomeland()
	PurchaseHomelandFrame_HeadFont:SetText( "建造家园" )
	PurchaseHomelandFrame:SetClientString( "build" )
	PurchaseHomelandFrame:SetClientUserData( 1, 10000 )
	PurchaseHomelandFrameFitment:SetText( "装修家园需要消耗银币：" )
	PurchaseHomelandFrame_ConfirmBtn:SetText( "确认建造" )
	PurchaseHomelandFrame_CancelBtn:SetText( "取消建造" )
	PurchaseHomelandFrameItemBtn:SetClientUserData( 0, 0 )
	PurchaseHomelandFrameItemBtn:SetClientUserData( 1, 0 )
	PurchaseHomelandFrameItemBtn:SetClientUserData( 2, 0 )
	PurchaseHomelandFrameItemBtn:SetClientUserData( 3, 0 )
	PurchaseHomelandFrame:Show()
end

function InquireHomelandFrame_InfoBtn_EnterBtn_OnClick()
	local text	= this:GetText()
	if text == "进入" then
		EnterHomeland( selectedHome )
	elseif text == "购买" then
		PurchaseHomeland( selectedHome )
	end
	InquireHomelandFrame:Hide()
end

function InquireHomelandFrame_CloseCloseBtn_OnClick()
	InquireHomelandFrame:Hide()
end

local function SetHomelandInfo_Sell( home )
	if not home then
		return
	end

	local name	= getglobal( "SellHomelandFrameNameFont" )
	local level	= getglobal( "SellHomelandFrameLevelFont" )
	local status	= getglobal( "SellHomelandFrameStatusFont" )

	name:SetText( "家园名称：" .. home.Info.Name )
	level:SetText( "家园等级：" .. home.Lvl )
	status:SetText( "" ) --"家园状态：" .. getHomelandStatus( home ) )
end

function SellHomelandFrame_OnShow()
	SetFocusFrame( "SellHomelandFrame_EditBox" )
	SellHomelandFrame_EditBox:Clear()
	SellHomelandFrame_EditBox:SetText( "2000000" )
	SellHomelandFrame_EditBox:SelectAllText()
end

function SellHomelandFrame_OnHide()
	SetFocusFrame( "UIClient" )
end

function SellHomelandFrame_ConfirmBtn_OnClick()
	local text	= SellHomelandFrame_EditBox:GetText()
	if not text or text == "" then
		ShowMidTips( "请填写适当的价格" )
		SetFocusFrame( "SellHomelandFrame_EditBox" )
		SellHomelandFrame_EditBox:SelectAllText()
		return
	end

	local price	= tonumber( text )
	if price <= 0 then
		ShowMidTips( "填写的价格有误，请填写大于0的数值" )
		SetFocusFrame( "SellHomelandFrame_EditBox" )
		SellHomelandFrame_EditBox:SelectAllText()
		return
	end

	local id	= SellHomelandFrame:GetClientUserData( 0 )
	GameHomelandManager:requestHomeSale( id, price )
	SellHomelandFrame:Hide()
	ShowMidTips( "家园状态转为出售状态" )
end

function SellHomelandFrame_CancelBtn_OnClick()
	SellHomelandFrame:Hide()
end

function SetNameHomelandFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer then return end
	local mainplayerName = mainplayer:getName();
	SetFocusFrame( "SetNameHomelandFrame_EditBox" )
	SetNameHomelandFrame_EditBox:Clear()
	SetNameHomelandFrame_EditBox:SetText( selectedHome ~= nil and 
		( selectedHome.Info.Name ~= "" and selectedHome.Info.Name or mainplayerName ) or mainplayerName )
	SetNameHomelandFrame_EditBox:SelectAllText()
end

function SetNameHomelandFrame_OnHide()
	SetFocusFrame( "UIClient" )
end

function SetNameHomelandFrame_ConfirmBtn_OnClick()
	local text = SetNameHomelandFrame_EditBox:GetText()

	if text == "" or string.len( text ) < 3 then
		ShowMidTips( "家园名称至少需要三个字符以上" )
		SetFocusFrame( "SetNameHomelandFrame_EditBox" )
		SetNameHomelandFrame_EditBox:SelectAllText()
		return
	end

	GameHomelandManager:requestHomeSetName( SetNameHomelandFrame:GetClientUserData( 0 ), text )
	SetNameHomelandFrame:Hide()
end

function SetNameHomelandFrame_CancleBtn_OnClick()
	SetNameHomelandFrame:Hide()
end

local function setStatusBar( bar, tl )
	if not bar or not tl then
		return
	end

	local name = bar:GetName() .. "_Btn"
	for i = 1, #tl do
		local font = getglobal( name .. i .. "Font" )
		if not btn then
			break
		end
		font:SetText( tl[ i ] )
	end
end

function SetStatusHomelandFrame_StatusDownBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return
	end

	if HomelandSelectStatusFrame:IsShown() then
		HomelandSelectStatusFrame:Hide()
	else
		HomelandSelectStatusFrame:Show()
	end
end

function HomelandSelectStatusFrame_OnShow()
	local bar	= getglobal( "HomelandSelectStatusFrame" )
	local tl	= t_HomelandStatus
	local font	= getglobal( "SetStatusHomelandFrame_StatusSelectBtnFont" )
	local text	= font:GetText()

	setStatusBar( bar, tl )
end

function HomelandSelectStatusFrameBtn_OnClick()
	local font = getglobal( this:GetName() .. "Font" )
	local text = font:GetText()

	HomelandSelectStatusFrame:Hide()
	SetStatusHomelandFrame_StatusSelectBtnFont:SetText( text )
end

function SetStatusHomelandFrame_ConfirmBtn_OnClick()
	local text = SetStatusHomelandFrame_StatusSelectBtnFont:GetText()
	if text and text ~= "" and t_Status[ text ] then
		GameHomelandManager:requestHomeSetStat( SetStatusHomelandFrame:GetClientUserData( 0 ), t_Status[ text ] )
		SetStatusHomelandFrame:Hide()
	else
		ShowMidTips( "请选择正确的家园状态")
	end
end

function SetStatusHomelandFrame_CancleBtn_OnClick()
	SetStatusHomelandFrame:Hide()
end

function SetStatusHomelandFrame_OnShow()
	
end

function SetStatusHomelandFrame_OnHide()
	SetFocusFrame( "UIClient" )
end

function InquireHomelandFrame_DestroyBtn_OnClick()
	local title	= getglobal( "SecondHomelandFrameTitleFont" )
	local rich	= getglobal( "SecondHomelandFrameContent" )
	
	title:SetText( "拆除家园" )
	rich:setCenterLine( true )
	rich:SetText( szHomelandTips[ "拆除家园" ][ 1 ], 255, 255, 190 )
	SecondHomelandFrame:SetClientUserData( 0, selectedHome.HomeID )
	SecondHomelandFrame:Show()
end

function InquireHomelandFrame_SaleBtn_OnClick()
	SetHomelandInfo_Sell( selectedHome )
	SellHomelandFrame:SetClientUserData( 0, selectedHome.HomeID )
	SellHomelandFrame:Show()
end

function SecondHomelandFrame_OnShow()
	
end

function SecondHomelandFrame_OnHide()

end

function SecondHomelandFrame_ConfirmBtn_OnClick()
	local text	= SecondHomelandFrameTitleFont:GetText()
	local first	= SecondHomelandFrame:GetClientUserData( 0 )
	local second	= SecondHomelandFrame:GetClientUserData( 1 )
	if szHomelandTips[ text ] then
		szHomelandTips[ text ][ 2 ]( first, second )
	end
	SecondHomelandFrame:Hide()
end

function SecondHomelandFrame_CancleBtn_OnClick()
	SecondHomelandFrame:Hide()
end

local BAR_WIDTH		= 190
local BAR_HEIGHT	= 9

local function SetPetProperty( pet )
	if not pet then return end
	
	local level	= getglobal( "PetHomelandFrameLevelFont" )
	local energy	= getglobal( "PetHomelandFrame_PropertyBtnEnergyFont" )
	local clean	= getglobal( "PetHomelandFrame_PropertyBtnCleanFont" )
	local happy	= getglobal( "PetHomelandFrame_PropertyBtnHappyFont" )
	local health	= getglobal( "PetHomelandFrame_PropertyBtnHealthFont" )
	local bar	= getglobal( "PetHomelandFrameProgressBar" )
	local monLvlDef	= GameHomelandManager:getHomeMonLvlDef( pet.Lvl + 1 )
	if not monLvlDef or monLvlDef.Exp == 0 then
		bar:SetSize( BAR_WIDTH, BAR_HEIGHT )
	else
		local scale = pet.Exp / monLvlDef.Exp
		if scale >= 0.0 and scale <= 1.0 then
			bar:SetSize( scale * BAR_WIDTH, BAR_HEIGHT )
		end
	end

	level:SetText( "lv: " .. pet.Lvl .. "级" )
	energy:SetText( "饱食度：" .. pet.Baoshi .. "%" )
	clean:SetText( "清洁度：" .. pet.Qingjie  .. "%" )
	happy:SetText( "愉快度：" .. pet.Happy .. "%" )
	health:SetText( "健康度：" .. pet.Jiankang .. "%" )
end

function PetHomelandFrame_ModelBtn_TurnLeftBtn_OnMouseDown()
	local model = getglobal( "PetHomelandFrame_ModelBtnView" )
	modelAngle = modelAngle - 5
	model:setEntityRotate( 0, modelAngle )
end

function PetHomelandFrame_ModelBtn_TurnRightBtn_OnMouseDown()
	local model = getglobal( "PetHomelandFrame_ModelBtnView" )
	modelAngle = modelAngle + 5
	model:setEntityRotate( 0, modelAngle )
end

function UpdateHomelandPetInfo()
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer == nil then return end 
	local pet		= GameHomelandManager:getHomePet()
	local model		= getglobal( "PetHomelandFrame_ModelBtnView" )
	local monsterDef	= getMonsterDef( pet.ID )
	if pet == nil or monsterDef == nil then PetHomelandFrame:Hide() return end
	local path 		= GetModePathById( monsterDef.MonsterID )
	if path == nil then PetHomelandFrame:Hide() end
	local scale		= 1
	
	if t_petID ~= pet.ID then
		if monsterDef.PetID == 301 then
			scale = 0.15
		else
			scale = GetNpcDisplayInfo( monsterDef.MonsterID ).size == 0 and 1 or GetNpcDisplayInfo( monsterDef.MonsterID ).size
		end
		model:setPetModel( 0, 10100, path )
		model:setPetScale( 0, scale, scale, scale )
		local t_ModelPos = { [214] = -20,[1214] = -20 }
		if t_ModelPos[ monsterDef.MonsterID ] then
			model:setEntityPosition( 0, 0, t_ModelPos[ monsterDef.MonsterID ], 0 )
		else
			model:setEntityPosition( 0, 0, 40, 0 )
		end
		t_petID = pet.ID
	end
	
	PetHomelandFrame_HeadFont:SetText( "宠物" .. monsterDef.Name )
	SetPetProperty( pet )
end

function PetHomelandFrame_OnShow()
	modelAngle = 0
	UpdateHomelandPetInfo()
end

function PetHomelandFrame_OnHide()
	t_petID = 0
end

function PetHomelandFrame_PropertyBtn_FeedBtn_OnClick()
	t_petOp = PET_OP_FEED
	GameHomelandManager:requestHomePet( HOME_PET_OP_EAT_CLIENT )
end

function PetHomelandFrame_PropertyBtn_CleanBtn_OnClick()
	t_petOp = PET_OP_CLEAN
	GameHomelandManager:requestHomePet( HOME_PET_OP_CLAER_CLIENT )
end

function PetHomelandFrame_PropertyBtn_PlayBtn_OnClick()
	t_petOp = PET_OP_PLAY
	GameHomelandManager:requestHomePet( HOME_PET_OP_PLAY_CLIENT )
end

function PetHomelandFrame_PropertyBtn_ExerciseBtn_OnClick()
	t_petOp = PET_OP_EXERCISE
	GameHomelandManager:requestHomePet( HOME_PET_OP_DUANLIAN_CLIENT )
end

function PetHomelandFrame_HarvestBtn_OnClick()
	GameHomelandManager:requestHomePet( HOME_PET_OP_GET_CLIENT )
end

function SetSeeMessageBoard( home )
	
end

function SeeMessageBoardHomelandFrame_OnShow()
	InitMessageBoardFrame()
end

function SeeMessageBoardHomelandFrame_OnHide()

end

function SetMessageBoardScrollBar( len )
	if len == nil then return end
	local slider = getglobal("SeeMessageBoardHomelandFrame_ScrollBar");
	SetScrollBar( "SeeMessageBoardHomelandFrame_ScrollBar", MAX_MESSAGEBORD_LIST_NUM, len );
	slider:SetValue( 0 );
end

function InitMessageBoardFrame()
	SetMessageBoardScrollBar( #t_Message )
	UpdateMessageBoardList( 1 )
end

function ClearMessageBoard()
	for i = 1, MAX_MESSAGEBORD_LIST_NUM do
		local name	= "SeeMessageBoardHomelandFrameList" .. i
		local btn	= getglobal( name )
		local font	= getglobal( name .. "NameAndTimeFont" )
		local rich	= getglobal( name .. "Message" )

		font:SetText( "" )
		rich:Clear()
	end
end

function UpdateMessageBoardList( nIndex )
	local count	= 1

	ClearMessageBoard()

	for i = nIndex, #t_Message do
		if count > MAX_MESSAGEBORD_LIST_NUM then
			break
		end
		
		local one	= t_Message[ i ]
		local name	= "SeeMessageBoardHomelandFrameList" .. count
		local btn	= getglobal( name )
		local font	= getglobal( name .. "NameAndTimeFont" )
		local rich	= getglobal( name .. "Message" )
		
		font:SetText( one.name )
		rich:SetText( one.content, 255, 255, 255 )
		
		count = count + 1
	end
	
	for i = count, MAX_MESSAGEBORD_LIST_NUM do
		local name	= "SeeMessageBoardHomelandFrameList" .. count
		local btn	= getglobal( name )
		local font	= getglobal( name .. "NameAndTimeFont" )
		local rich	= getglobal( name .. "Message" )
		btn:Disable()
		font:SetText( "" )
		rich:Clear()
	end
end

function SeeMessageBoardHomelandFrame_ScrollUpBtn_OnClick()
	local slider	= getglobal( "SeeMessageBoardHomelandFrame_ScrollBar" )
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	local nMaxValue	= slider:GetMaxValue()
	if nValue <= nMaxValue then
		slider:SetValue( nValue )
	end
end

function SeeMessageBoardHomelandFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( "SeeMessageBoardHomelandFrame_ScrollBar" )
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() - slider:GetValueStep()
	if nValue >= 0 then
		slider:SetValue( nValue )
	end
end

function SeeMessageBoardHomelandFrame_ScrollBar_OnValueChanged()
	local slider 	= getglobal("SeeMessageBoardHomelandFrame_ScrollBar");
	local nValue 	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateMessageBoardList( nValue + 1 );
	end
end

function EditMessageBoardHomelandFrame_OnShow()
	SetFocusFrame( "EditMessageBoardHomelandFrameContent_MultiEditBox" )
end

function EditMessageBoardHomelandFrame_OnHide()
	SetFocusFrame( "UIClient" )
end

function EditMessageBoardHomelandFrame_SendBtn_OnClick()
	local text 	= EditMessageBoardHomelandFrameContent_MultiEditBox:GetText()
	local id	= EditMessageBoardHomelandFrame:GetClientUserData( 0 )
	
	GameHomelandManager:requestHomeMsgBoard( id, text )
	EditMessageBoardHomelandFrame:Hide()
	ShowMidTips( "留言发送成功" )
end

function InquireHomelandFrame_InfoBtn_SetNameBtn_OnClick()
	if not selectedHome then return end
	SetNameHomelandFrame:SetClientUserData( 0, selectedHome.HomeID )
	SetNameHomelandFrame:Show()
end

function InquireHomelandFrame_InfoBtn_SetStatusBtn_OnClick()
	if not selectedHome then return end
	SetStatusHomelandFrame:SetClientUserData( 0, selectedHome.HomeID )
	SetStatusHomelandFrame:Show()
end

function SetMessage( home )
	if not home then
		return
	end
	
	t_Message = { }
	local msg = home.Info.Liuyan
	local num = msg.Num > HOME_MAX_LIUYAN_TOLUA and HOME_MAX_LIUYAN_TOLUA or msg.Num
	for i = 1, num do
		local one = msg.One[ i - 1 ]
		t_Message[ #t_Message + 1 ] = { name = one.Name .. "\t" .. os.date( "( %Y-%m-%d %H:%M:%S )", one.Time ), 
							content = one.Text }
	end
end

function InquireHomelandFrame_InquireBtn_OnClick()
	SetMessage( selectedHome )
	SeeMessageBoardHomelandFrame:Show()
end

function InquireHomelandFrame_MessageBtn_OnClick()
	if selectedHome then
		EditMessageBoardHomelandFrameReceiverFont:SetText( string.format( "留言给：%s —— %s", selectedHome.RoleName, selectedHome.Info.Name ) )
		EditMessageBoardHomelandFrame:SetClientUserData( 0, selectedHome.HomeID )
		EditMessageBoardHomelandFrame:Show()
	end
end

function InquireHomelandFrame_InfoBtn_RecallBuyBtn_OnClick()
	GameHomelandManager:requestHomeSale( selectedHome.HomeID, 0 )
	ShowMidTips( "家园状态转为" .. t_Status[ selectedHome.Info.Stat ] .. "状态" )
end

function InquireHomelandFrame_BuildBtn_OnClick()
	BuildHomeland()
end

function InquireHomelandFrame_BuildBtn_OnEnter()
	local tip = "当前还有土地可用来建造家园"
	if not this:IsEnable() then
		tip = "当前已没有足够的土地用来建造家园了"
	end
	SetGameTooltips( this:GetName(), tip )
end

function InquireHomelandFrame_BuildBtn_OnLeave()
	GameTooltip:Hide()
end

function OpenHomelandPetFrame()
	if not GameHomelandManager:isInSelfHome() then
		ShowMidTips( "他人萌宠，无法查看信息" )
		return
	end
	if PetHomelandFrame:IsShown() then
		return
	end
	PetHomelandFrame:Show()
end

function CloseHomelandPetFrame()
	PetHomelandFrame:Hide()
end

function CloseHomelandFrames()
	local frames = { "PurchaseHomelandFrame",
		"SellHomelandFrame",
		"InquireHomelandFrame",
		"SetNameHomelandFrame",
		"SetStatusHomelandFrame",
		"SecondHomelandFrame",
		"PetHomelandFrame",
		"SeeMessageBoardHomelandFrame",
		"EditMessageBoardHomelandFrame",
	}
	for _, key in ipairs( frames ) do
		local frame = getglobal( key )
		frame:Hide()
	end
end

function ShowFurnitureFrame()
	if not FurnitureOpFrame:IsShown() then
		FurnitureOpFrame:Show()
	end
end

function HideFurnitureFrame()
	FurnitureOpFrame:Hide()
end

function ShowFurnitureClickTip()
	ShowMidTips( "同一时刻只能摆放一件家具" )
end

function BeginFurnitureMode( item )
	if not item then return end
	if not GameHomelandManager:isInHome() then
		ShowMidTips( "该道具只能在家园内使用" )
		return
	end
	if not GameHomelandManager:isInSelfHome() then
		ShowMidTips( "园主不允许外人对他的家园进行此项操作" )
		return
	end
	if GameHomelandManager:isFurnitureMode() then
		ShowMidTips( "同一时刻只能摆放一件家具" )
		return
	end
	if not GameHomelandManager:isInHouse() then
		ShowMidTips( "请在房间内摆放家具" )
		return
	end
	local itemId	= item:getItemId()
	if itemId > 0 then
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--解锁
	end
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), btn:GetName() );
	GameHomelandManager:beginFurnitureMode( item )
	FurnitureOpFrame:Show()
end

function PutHomelandFurniture()
	MessageBox( "摆放家具", "确认摆放家具于此" )
	MessageBoxFrame:SetClientString( "摆放家具" )
end

function ChangeHomelandPet( item )
	if item then
		GameHomelandManager:requestHomePet( HOME_PET_OP_REPLACE_CLIENT, item:getGridIdx() )
	end
end

function ShowForbiddenTip()
	ShowMidTips( "园主不允许外人对他的家园进行此项操作" )
end

function BeginPlantMode( btnName, item )
	if not btnName or not item then
		return false
	end
	if not GameHomelandManager:isInHome() then
		ShowMidTips( "该道具只能在家园内使用" )
		return
	end
	if not GameHomelandManager:isInSelfHome() then
		ShowMidTips( "园主不允许外人对他的家园进行此项操作" )
		return false
	end
	if GameHomelandManager:isInPlantMode() then
		ShowMidTips( "已有种子处于种植当中" )
		return false
	end
	local itemId	= item:getItemId()
	if itemId > 0 then
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--解锁
	end
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "PlayerContainerFrame" );
	GameHomelandManager:beginPlantMode( item )
	return true
end

function CancelPlantMode( type, grid, id )
	if not type or not grid or not id then
		return
	end
	if id > 0 then
		UnLockItem( type, grid, id )
	end
	UIEndDrag()
	UIMSG:getUIGV().DragFrom = DRAG_NONE
	GameMgr:cancelNextSelect()
end

function EndPlantMode()
	UIEndDrag()
	UIMSG:getUIGV().DragFrom = DRAG_NONE
	GameMgr:cancelNextSelect()
end

function ShowPlantUnreclaimed()
	ShowMidTips( "此田地尚未开垦，无法直接种植" )
end

function ShowPlantOverride()
	ShowMidTips( "此田地已有作物" )
end

function ShowOutsideOfHouseTip()
	ShowMidTips( "位于房间外，自动取消家具摆放" )
end

local isWalkHolding	= false
local isRotateHolding	= false
local delta		= 10.0

function FurnitureOpFrame_WalkBtn_OnEnter()
	SetGameTooltips( this:GetName(), "Hold住移动家具" )
end

function FurnitureOpFrame_WalkBtn_OnMouseDown()
	isWalkHolding = true
	GameHomelandManager:beginMoveMode()
end

function FurnitureOpFrame_WalkBtn_OnMouseUp()
	isWalkHolding = false
end

function FurnitureOpFrame_WalkBtn_OnLeave()
	GameTooltipOnLeave()
	if isWalkHolding then
		--GameHomelandManager:endMoveMode()
		isWalkHolding = false
	end
end

function FurnitureOpFrame_RotateBtn_OnEnter()
	SetGameTooltips( this:GetName(), "点击旋转家具" )
end

function FurnitureOpFrame_RotateBtn_OnMouseDown()
	GameHomelandManager:stepFurnitureAngle( delta )
	isRotateHolding = true
end

function FurnitureOpFrame_RotateBtn_OnMouseUp()
	isRotateHolding = false
end

function FurnitureOpFrame_RotateBtn_OnLeave()
	isRotateHolding = false
	GameTooltipOnLeave()
end

function FurnitureOpFrame_OnUpdate()
	if isRotateHolding then
		GameHomelandManager:stepFurnitureAngle( delta )
	end
end

function FurnitureOpFrame_CancelBtn_OnEnter()
	SetGameTooltips( this:GetName(), "点击撤消家具摆放" )
end

function FurnitureOpFrame_CancelBtn_OnClick()
	if GameHomelandManager:isFurnitureAlready() then
		GameHomelandManager:cancelFurnitureModeAlready()
	else
		GameHomelandManager:cancelFurnitureMode()
	end
end

function FurnitureOpFrame_CancelBtn_OnLeave()
	GameTooltipOnLeave()
end

function FurnitureOpFrame_OKBtn_OnEnter()
	SetGameTooltips( this:GetName(), "点击摆放家具" )
end

function FurnitureOpFrame_OKBtn_OnClick()
	PutHomelandFurniture()
end

function FurnitureOpFrame_OKBtn_OnLeave()
	GameTooltipOnLeave()
end