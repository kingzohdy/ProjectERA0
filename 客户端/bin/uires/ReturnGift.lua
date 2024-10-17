-------------------------------------------------------------回归礼包--------------------------------------------------------------------
function OpenOfflineGiftFrame( nExp, nDay )
	SetReturnGiftInfo( nExp, nDay )
	ReturnGiftFrame:Show()
	ReturnGiftFrame:SetClientUserData( 0, nDay )
	ReturnGiftFrameItemBackTexture:Show()
end
function SetReturnGiftInfo( nExp, nDay )
	ReturnGiftFrameInfo1:SetText( "为了更好的让您体验游戏后期内容，系统将为您累积部分补偿经验，愿您升级轻松，快乐游戏！",255,255,190 )
	if nExp <= 0 then
		ReturnGiftFrameInfo2:SetText( "",0,255,190 )
	else
		ReturnGiftFrameInfo2:SetText( "累积经验："..nExp,0,255,0 )
	end
	ReturnGiftFrameInfo3:SetText( "由于您已经#G"..nDay.."#n天未登陆游戏，零纪元特别为您准备了一份回归礼包，希望您能感受到我们的诚意！",255,255,190 )
	--写死的用于显示礼包的物品5052180
	local itemDef = getItemDef( 5052180 )
	ReturnGiftFrameItem:SetClientUserData( 0, 5052180 )
	local tex = getglobal( "ReturnGiftFrameItemIconTexture" )
	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	tex:SetTexture( IconPath );
	ReturnGiftFrameItemName:SetText( itemDef.Name.."x1" )
end
function ReturnGiftFrameGetBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer()
	MainPlayer:requestOffLineGift()
end
function ReturnGiftFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		if mainplayer:isOutSide() then
			this:Hide()
		end
	end
end
function ReturnGiftGetResult( nFlag )
	if nFlag == 1 then
		ReturnGiftFrame:Hide()
		ShowMidTips( "已成功领取回归奖励礼包" )
	else
		ShowMidTips( "领取失败" )		
	end
end
function ReturnGiftFrameItem_OnEnter()
	--local id = this:GetClientUserData( 0 )
	--ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id) )
end
--------------------------------------------领取经验---------------------------------------------------------
function OpenReturnGiftExp( item )
	local nExp = item:getItemInstVal1()
	SetReturnExp( nExp )
	--清除上一次的物品锁定
	ClearItemLock("ReturnGiftExpFrame")
	ReturnGiftExpFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	ReturnGiftExpFrame:Show()
	ReturnGiftExpFrame:SetClientUserData( 0, item:getListType() )
	ReturnGiftExpFrame:SetClientUserData( 1, item:getGridIdx() )
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ReturnGiftExpFrame" )
end	
function SetReturnExp( nExp )
	ReturnGiftExpFrameInfo:SetText( "回归补偿经验值"..nExp,255,255,190 )
	ReturnGiftExpFrameCheck1Font:SetText( "使用".. math.floor(nExp/1000000)+1 .."金币    200%经验" )
end
function ReturnGiftExpFrame_OnLoad()
end
function ReturnGiftExpFrame_OnShow()
	ReturnGiftExpFrameCheck1:setCheckState( true )
	ReturnGiftExpFrameCheck2:setCheckState( false )
end
function ReturnGiftExpFrameCheck_OnClick()
	ReturnGiftExpFrameCheck1:setCheckState( false )
	ReturnGiftExpFrameCheck2:setCheckState( false )
	this:setCheckState( true )
end
function ReturnGiftExpFrameGetBtn_OnClick()
	local index = 0
	if ReturnGiftExpFrameCheck2:GetCheckState() then
		index = 1
	end
	local MainPlayer = ActorMgr:getMainPlayer()
	local container = MainPlayer:getContainer()
	if container ~= nil then
		local listType = ReturnGiftExpFrame:GetClientUserData( 0 )
		local gridIdx = ReturnGiftExpFrame:GetClientUserData( 1 )
		container:send_UsedExpendableItem( listType,gridIdx,0,index )
		ReturnGiftExpFrame:Hide()
	end
end
function ReturnGiftExpFrame_OnHide()
	ClearItemLock("ReturnGiftExpFrame")
end 
--------------------------------------------领取装备----------------------------------------------------------
local MAX_RETURN_ARM_SELECT_NUM = 6
function OpenReturnArmSelect( item, nLvl )
	local MainPlayer = ActorMgr:getMainPlayer()
	local nCareer = MainPlayer:getRace()
	local armBuChangDef = GetArmBuChangDef( math.floor(nLvl/10), nCareer )
	if armBuChangDef == nil then return end
	SetReturnArm(nLvl)
	--清除上一次的物品锁定
	ClearItemLock("ReturnArmSelectFrame")
	ReturnArmSelectFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	ReturnArmSelectFrame:Show()
	ReturnArmSelectFrame:SetClientUserData( 0, item:getListType() )
	ReturnArmSelectFrame:SetClientUserData( 1, item:getGridIdx() )
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "ReturnArmSelectFrame" )
end
function SetReturnArm(nLvl)
	local MainPlayer = ActorMgr:getMainPlayer()
	local nCareer = MainPlayer:getRace()
	local armBuChangDef = GetArmBuChangDef( math.floor(nLvl/10), nCareer )
	for i = 1, MAX_RETURN_ARM_SELECT_NUM do
		local btn = getglobal( "ReturnArmSelectFrameItem"..i )
		local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex" )
		local btnTex = getglobal( btn:GetName().."IconTexture" )
		local btnName = getglobal( btn:GetName().."Name" )
		if armBuChangDef.ItemID[i-1] <= 0 then
			btn:Disable();
			btn:SetClientUserData( 0, 0 )
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName()); -- 设置装备特效
			btnTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			btnName:SetText("")
		else
			btn:Enable();
			btn:SetClientUserData( 0, armBuChangDef.ItemID[i-1] )
			local itemdef = getItemDef( armBuChangDef.ItemID[i-1] )
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName()); -- 设置装备特效

			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			btnTex:SetTexture( IconPath );
			--部位
			btnName:SetText( GetWearPos(itemdef).."*1" )
		end
	end
end
function ReturnArmSelectFrame_OnLoad()
	for i = 1, MAX_RETURN_ARM_SELECT_NUM do
		local btn = getglobal( "ReturnArmSelectFrameItem"..i )
		btn:Show()
		local btnBack = getglobal( btn:GetName().."BackTexture" )
		btnBack:Show()
	end
end
function ReturnArmSelectFrame_OnShow()
	for i = 1, MAX_RETURN_ARM_SELECT_NUM do
		local btn = getglobal( "ReturnArmSelectFrameItem"..i )
		btn:DisChecked()
	end
	ReturnArmSelectFrameItem1:Checked()
end
function ReturnArmSelectFrameItem_OnClick()
	for i = 1, MAX_RETURN_ARM_SELECT_NUM do
		local btn = getglobal( "ReturnArmSelectFrameItem"..i )
		btn:DisChecked()
	end
	this:Checked()
end
function ReturnArmSelectFrameItem_OnEnter()
	local id = this:GetClientUserData( 0 )
	if id == 0 then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id) )
end
function ReturnArmSelectFrameGetBtn_OnClick()
	local index = 0
	for i = 1, MAX_RETURN_ARM_SELECT_NUM do
		local btn = getglobal( "ReturnArmSelectFrameItem"..i )
		if btn:IsChecked() and btn:IsEnable() then
			index = i
		end
	end
	if index == 0 then
		ShowMidTips( "请选择装备位后，再进行领取" )
		return
	end
	local MainPlayer = ActorMgr:getMainPlayer()
	local container = MainPlayer:getContainer()
	if container ~= nil then
		local listType = ReturnArmSelectFrame:GetClientUserData( 0 )
		local gridIdx = ReturnArmSelectFrame:GetClientUserData( 1 )
		container:send_UsedExpendableItem( listType,gridIdx,0,index )
		ReturnArmSelectFrame:Hide()
	end
end
function ReturnArmSelectFrame_OnHide()
	ClearItemLock("ReturnArmSelectFrame")
end
----------------------------------------宝石包道具------------------------------------
function OpenGemSelectPag( item )
	if not SetGemPagSelect( item ) then return end
	--清除上一次的物品锁定
	ClearItemLock("GemPackageSelectFrame")
	GemPackageSelectFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	GemPackageSelectFrame:Show()
	GemPackageSelectFrame:SetClientUserData( 0, item:getListType() )
	GemPackageSelectFrame:SetClientUserData( 1, item:getGridIdx() )
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "GemPackageSelectFrame" )
end
function SetGemPagSelect( item )
	local itemDef = getItemDef( item:getItemId() )
	if itemDef.ItemID == 0 then
		return false
	end
	local gemPagDef = nil
	for i = 1, MAX_ITEM_RESULT do
		local resultData = itemDef.Result[i-1];
		if resultData.ResultID == RESULT_ITEM_GEM_PAG then
			gemPagDef = getGemPagDef( resultData.ResultVal1 );
		end
	end
	if gemPagDef == nil then
		return false
	end
	for i = 1, MAX_GEM_PAG_GEM_NUM_TOLUA do
		local btn = getglobal( "GemPackageSelectFrameItem"..i )
		local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex" )
		local btnTex = getglobal( btn:GetName().."IconTexture" )
		local btnName = getglobal( btn:GetName().."Name" )
		local btnBind = getglobal( btn:GetName().."BindTexture" )
		if gemPagDef.GemInfo[i-1].ItemID == 0 then
			btn:Disable();
			btn:SetClientUserData( 0, 0 )
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName()); -- 设置装备特效
			btnTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			btnName:SetText("")
			btnBind:Hide()			
		else
			btn:Enable();
			btn:SetClientUserData( 0, gemPagDef.GemInfo[i-1].ItemID )
			local itemdef = getItemDef( gemPagDef.GemInfo[i-1].ItemID )
			SetEquipImportantEffectUV( itemdef, UVAnimationTex:GetName()); -- 设置装备特效

			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			btnTex:SetTexture( IconPath );
			--描述
			btnName:SetText( gemPagDef.GemInfo[i-1].Desc )
			if gemPagDef.GemInfo[i-1].Bind == 1 then
				btnBind:Show()
			end
		end
	end
	return true
end
function GemPackageSelectFrame_OnShow()
	for i = 1, MAX_GEM_PAG_GEM_NUM_TOLUA do
		local btn = getglobal( "GemPackageSelectFrameItem"..i )
		btn:DisChecked()
	end
	GemPackageSelectFrameItem1:Checked()
end
function GemPackageSelectFrame_OnLoad()
	for i = 1, MAX_GEM_PAG_GEM_NUM_TOLUA do
		local btn = getglobal( "GemPackageSelectFrameItem"..i )
		btn:Show()
		local btnBack = getglobal( btn:GetName().."BackTexture" )
		btnBack:Show()
	end
end
function GemPackageSelectFrame_OnHide()
	ClearItemLock("GemPackageSelectFrame")
	GemPackageSelectFrame:SetClientUserData( 0, 0 )
	GemPackageSelectFrame:SetClientUserData( 1, 0 )
end
function GemPackageSelectFrameItem_OnClick()
	for i = 1, MAX_GEM_PAG_GEM_NUM_TOLUA do
		local btn = getglobal( "GemPackageSelectFrameItem"..i )
		btn:DisChecked()
	end
	this:Checked()
end
function GemPackageSelectFrameItem_OnClickItem_OnEnter()
	local id = this:GetClientUserData( 0 )
	if id == 0 then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id) )
end
function GemPackageSelectFrameGetBtn_OnClick()
	local index = 0
	for i = 1, MAX_GEM_PAG_GEM_NUM_TOLUA do
		local btn = getglobal( "GemPackageSelectFrameItem"..i )
		if btn:IsChecked() and btn:IsEnable() then
			index = i
		end
	end
	if index == 0 then
		ShowMidTips( "请选择宝石后，再进行领取" )
		return
	end
	local MainPlayer = ActorMgr:getMainPlayer()
	local container = MainPlayer:getContainer()
	if container ~= nil then
		local listType = GemPackageSelectFrame:GetClientUserData( 0 )
		local gridIdx = GemPackageSelectFrame:GetClientUserData( 1 )
		container:send_UsedExpendableItem( listType,gridIdx,0,index )
		GemPackageSelectFrame:Hide()
	end
end
