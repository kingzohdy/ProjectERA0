local MAX_FUNC_BTN_NUM = 9
local MAX_TYPE_BTN_NUM = 8


local MAX_STONE_FUNC_TOLUA = 16
local MAX_STONE_OPEN_CASE_TOLUA = 3
local nSelectID = 0

local function UpdateType()
	for i=1,MAX_TYPE_BTN_NUM do
		local def = VipManager:getStarStoneDefByIdx( i-1 )
		local btn = getglobal( "VipFrameTypeBtn" .. i )
		local icon = getglobal( btn:GetName() .. "Icon" )
		btn:SetText( def.Name )
		icon:SetTexture( "uires\\ui\\Za\\TeQuanHuiZhang\\" .. def.IconID .. ".tga" )
		if VipManager:isEnableVip( def.ID ) then
			icon:SetGray( false )
		else
			icon:SetGray( true )
		end
		btn:SetClientUserData( 0, def.ID )
	end
	if nSelectID == 0 then
		nSelectID = 1
		DisCheckAllBtn( "VipFrameTypeBtn", MAX_TYPE_BTN_NUM )
		VipFrameTypeBtn1:Checked()
	end
end

local function UpdateSelectData()
	local def = VipManager:getStarStoneDef( nSelectID )
	local endTime = VipManager:getVipEndTime( nSelectID ) - getServerTime().sec
	if endTime < 0 then
		VipFrameTime:SetText( "剩余时间：#cff00000天", 255, 255, 190 )
	else
		VipFrameTime:SetText( "剩余时间：#c00ff00" .. math.ceil( endTime/(24*60*60) ) .. "天", 255, 255, 190 )
	end
	local num = 1;
	local bEnable = VipManager:isEnableVip( nSelectID )
	for i=1,MAX_STONE_FUNC_TOLUA do
		if def.StarFuncList[i-1] == 0 or num > MAX_FUNC_BTN_NUM then
			break
		end
		
		local funcDef = VipManager:getStarFuncDef( def.StarFuncList[i-1] )
		local t_fonts = {	["VipFrameFuncBtn" .. i .. "Name"] = { value = "功能" .. i },
							["VipFrameFuncBtn" .. i .. "Desc"] = { value = funcDef.StarFuncDesc }
						}
		util.SetFonts( t_fonts )

		local btn = getglobal( "VipFrameFuncBtn" .. i .. "Btn" )
		if funcDef.OpenLink ~= "" then
			btn:SetText( funcDef.OpenLink )
			btn:Show()
			btn:SetClientUserData( 0, funcDef.StarResult.ResultID )
			btn:SetClientString( funcDef.StarFuncDesc );
			if bEnable then
				btn:Enable()
			else
				btn:Disable()
			end
		else
			btn:Hide()
		end
		btn = getglobal( "VipFrameFuncBtn" .. i )
		btn:Show()
		num = num + 1
	end
	for i=num,MAX_FUNC_BTN_NUM do
		local btn = getglobal( "VipFrameFuncBtn" .. i )
		btn:Hide()
	end
	HideAllFuncTipsBtn();
end

local t_BuyDay = { 7, 30, 180 }
local function UpdateBuyData()
	local def = VipManager:getStarStoneDef( nSelectID )
	for i=1,MAX_STONE_OPEN_CASE_TOLUA do
		local btn = getglobal( "VipFrameBuyBtn" .. i )
		local t_fonts = {	[btn:GetName() .. "Name"] = { value = "购买" .. t_BuyDay[MAX_STONE_OPEN_CASE_TOLUA-i+1] .. "天" },
							[btn:GetName() .. "Cost"] = { value = "金币：" .. def.OpenGodCoin[MAX_STONE_OPEN_CASE_TOLUA-i] },
							[btn:GetName() .. "Time"] = { value = "有效期：" .. t_BuyDay[MAX_STONE_OPEN_CASE_TOLUA-i+1] .. "天" },
						}
		local buyBtn = getglobal( btn:GetName() .. "BuyBtn" )
		buyBtn:SetClientUserData( 0, MAX_STONE_OPEN_CASE_TOLUA-i+1 )
		util.SetFonts( t_fonts )
	end
end

function VipFrame_OnLoad()
	this:RegisterEvent( "GE_VIP_INFO_UPDATE" );
	this:RegisterEvent( "GE_CONTAINER_CHG" );

	local count = 0;
	for i=1,MAX_TYPE_BTN_NUM do
		local def = VipManager:getStarStoneDefByIdx( i-1 )
		local btn = getglobal( "VipFrameTypeBtn" .. i )
		if def ~= nil then
			btn:Show();
			count = count + 1;
		end
	end
	MAX_TYPE_BTN_NUM = count;

	for i=1,MAX_FUNC_BTN_NUM do
		local tex1 = getglobal( "VipFrameFuncBtn" .. i .. "FuncBkg" )
		local tex2 = getglobal( "VipFrameFuncBtn" .. i .. "DescBkg" )
		local tex3 = getglobal( "VipFrameFuncBtn" .. i .. "BtnBkg" )
		if i%2 == 0 then
			tex1:ChangeTextureTemplate( "WhiteCategoryBkg" )
			tex2:ChangeTextureTemplate( "WhiteCategoryBkg" )
			tex3:ChangeTextureTemplate( "WhiteCategoryBkg" )
		else
			tex1:ChangeTextureTemplate( "BlackCategoryBkg" )
			tex2:ChangeTextureTemplate( "BlackCategoryBkg" )
			tex3:ChangeTextureTemplate( "BlackCategoryBkg" )
		end
	end
end

function VipFrame_OnEvent()
	if arg1 == "GE_VIP_INFO_UPDATE" then
		if VipFrame:IsShown() then
			UpdateType()
			UpdateSelectData()
			UpdateBuyData()
		end

		if VipManager:haveOpenVip() then
			PhotoFrameVipBtn:SetGrayEx( false )
			MarketManagerFrameVipBtn:SetGrayEx( false )
		else
			PhotoFrameVipBtn:SetGrayEx( true )
			MarketManagerFrameVipBtn:SetGrayEx( true )
		end

		if VipManager:isEnableVip( STAR_STONE_FIRE ) then--自动任务
			checkNeedResetAutoQuestNum();
			if not AutoFiniQuestFrame:IsShown() then
				AutoFiniQuestFrame:Show();
			end
		else
			AutoFiniQuestFrame:Hide();
			AutoFiniQuestConfigFrame:Hide();
			AutoRecoveryFrame:Hide();
			VipAutoAssistFrame:Hide();
		end

		if PlayerContainerFrame:IsShown() then
			ContainerFrame_VipShopBtn:SetGray( not VipManager:isOwnVipFunc( STAR_RESULT_SHOP_TOLUA ) );
			ContainerFrame_VipStashBtn:SetGray( not VipManager:isOwnVipFunc( STAR_RESULT_STASH_TOLUA ) );
			ContainerFrame_VipStoreBtn:SetGray( not VipManager:isOwnVipFunc( STAR_RESULT_STORE_TOLUA ) );
		end
	elseif arg1 == "GE_CONTAINER_CHG" then
		if AutoRecoveryFrame:IsShown() then
			AutoRecoveryFrameFoodCount();
		end
	end
end

function VipFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	UpdateType()
	UpdateSelectData()
	UpdateBuyData()
end

function VipFrameTypeBtn_OnClick()
	if IsMessageBoxBlock( VipFrame ) then
		return
	end
	DisCheckAllBtn( "VipFrameTypeBtn", MAX_TYPE_BTN_NUM )
	this:Checked()
	nSelectID = this:GetClientUserData( 0 )
	UpdateSelectData()
	UpdateBuyData()
end

function VipFrameChargeBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" )
end

function VipFrameFuncBtn_OnClick()
	local resultID = this:GetClientUserData( 0 )
	if resultID == STAR_RESULT_STASH_TOLUA then
		OpenStashFrame()
	elseif resultID ==  STAR_RESULT_SHOP_TOLUA then
		VipManager:openVipShop()
	elseif resultID == STAR_RESULT_TRANS_TOLUA then
		local btn = getglobal( this:GetParent() .. "Tips" );
		if btn:IsShown() then
			btn:Hide();
		else
			local szText = "使用急你所需航空功能，可直接\n免费飞行到地宫地图";
			local font = getglobal( btn:GetName() .. "Font" );
			font:SetText(szText);
			btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
			btn:SetSize( string.len(szText)*4 + 6, 38 );
			btn:Show();
		end
	elseif resultID == STAR_RESULT_PWORLD_CARD_TOLUA then
		local btn = getglobal( this:GetParent() .. "Tips" );
		if btn:IsShown() then
			btn:Hide();
		else
			local szText = "副本通关后拥有水晶徽章的角色\n可以翻开特权区的牌子";
			local font = getglobal( btn:GetName() .. "Font" );
			font:SetText(szText);
			btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
			btn:SetSize( string.len(szText)*4 + 6, 38 );
			btn:Show();
		end
	elseif resultID == STAR_RESULT_RONGLU_TOLUA then
		MapShower:GetMaxMapContal():linkMoveto( 1000, 307024 );
	elseif resultID == STAR_RESULT_DADI_TOLUA then
		MapShower:GetMaxMapContal():linkMoveto( 1000, 302061 );
	elseif resultID == STAR_RESULT_SHANGSAN_TOLUA then
		MapShower:GetMaxMapContal():linkMoveto( 1000, 302062 );
	elseif resultID == STAR_RESULT_SHATAN_EXP_TOLUA then
		local btn = getglobal( this:GetParent() .. "Tips" );
		if btn:IsShown() then
			btn:Hide();
		else
			local szText = "阳光海滩经验增加";
			local font = getglobal( btn:GetName() .. "Font" );
			font:SetText(szText);
			btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
			btn:SetSize( string.len(szText)*7 + 4, 23 );
			btn:Show();
		end
	elseif resultID == STAR_RESULT_PKVAL_TOLUA then
		local btn = getglobal( this:GetParent() .. "Tips" );
		if btn:IsShown() then
			btn:Hide();
		else
			local szText = "加快杀气值消减的速度";
			local font = getglobal( btn:GetName() .. "Font" );
			font:SetText(szText);
			btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
			btn:SetSize( string.len(szText)*7 + 2, 23 );
			btn:Show();
		end
	elseif resultID == STAR_RESULT_BUSY_TOLUA then
		local btn = getglobal( this:GetParent() .. "Tips" );
		if btn:IsShown() then
			btn:Hide();
		else
			local szText = "免费使用所有的\"我很忙\"";
			local font = getglobal( btn:GetName() .. "Font" );
			font:SetText(szText);
			btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
			btn:SetSize( string.len(szText)*7 + 2, 23 );
			btn:Show();
		end
	elseif resultID == STAR_RESULT_DEC_PKVAL_TOLUA then
		local btn = getglobal( this:GetParent() .. "Tips" );
		if btn:IsShown() then
			btn:Hide();
		else
			local szText = "任意地图都可以随时使用悔过书";
			local font = getglobal( btn:GetName() .. "Font" );
			font:SetText(szText);
			btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
			btn:SetSize( string.len(szText)*7 + 6, 23 );
			btn:Show();
		end
	elseif resultID == STAR_RESULT_FUZHU_TOLUA then
		if this:GetClientString() == "自动任务" then
			if not AutoFiniQuestConfigFrame:IsShown() then
				AutoFiniQuestConfigFrame:Show();
			else
				AutoFiniQuestConfigFrame:Hide();
			end
		elseif this:GetClientString() == "自动恢复" then
			if not AutoRecoveryFrame:IsShown() then
				AutoRecoveryFrame:Show();
			else
				AutoRecoveryFrame:Hide();
			end
		else
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil or mainplayer:getLv() < 11 then
				return ShowMidTips("当前等级无法使用自动辅助");
			end
			if not VipAutoAssistFrame:IsShown() then
				AutoAssistFrame:Show();
				AutoAssistFrame:SetPoint( "center","UIClient","center",0,0 );
				VipAutoAssistFrame:SetPointAlwaysRecalc("bottomleft", "AutoAssistFrame", "bottomright", 0, 0);
				VipAutoAssistFrame:Show();
			else
				AutoAssistFrame:Hide();
				VipAutoAssistFrame:Hide();
			end
		end
	elseif resultID == STAR_RESULT_STORE_TOLUA then
		OpenVipStore();
	else
		if string.find( this:GetClientString(), "流浪汉", 1, true ) then
			MapShower:GetMaxMapContal():linkMoveto( 1000, 304013 );
		elseif string.find( this:GetClientString(), "货车", 1, true ) then
			local btn = getglobal( this:GetParent() .. "Tips" );
			if btn:IsShown() then
				btn:Hide();
			else
				local szText = "商运时货车速度加快";
				local font = getglobal( btn:GetName() .. "Font" );
				font:SetText(szText);
				btn:SetPoint( "topleft", this:GetName(), "topright", 5, -10 );
				btn:SetSize( string.len(szText)*7 + 6, 23 );
				btn:Show();
			end
		elseif this:GetText() == "领取" then
			MapShower:GetMaxMapContal():linkMoveto( 1000, 302130 );
		end
	end
end

function VipFrameBuyBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "购买VIP" then
		return;
	end
	local def = VipManager:getStarStoneDef( nSelectID )
	local Type = this:GetClientUserData( 0 )
	if MessageBoxBlock("特权购买确认", string.format( "您要购买的#cffff00%d天的%s#n需要花费#cffff00%d#n个金币。请问是否购买？", t_BuyDay[Type], def.Name, def.OpenGodCoin[Type-1] ), VipFrame ) then
		VipManager:requestOpenVip( Type, nSelectID )
	end
end

function OnOpenStoneRes( nSuccess, nID )
	if nSuccess == 0 then
		local def = VipManager:getStarStoneDef( nID )
		MessageBoxForVip( "购买成功", "#cffff00" .. def.Name .. "#n已经购买成功。", true )
	else
		ExchangePointFrame:SetClientUserData(0,1);
		MarketManager:requestQueryTianyuan();
	end
end

function VipFrame_OnHide()
	HideAllFuncTipsBtn();
end

function HideAllFuncTipsBtn()
	for i=1,MAX_FUNC_BTN_NUM do
		local btn = getglobal( "VipFrameFuncBtn" .. i .."Tips" );
		btn:Hide()
	end
end

-------------------------------------------------vip自动回复--------------------------------------------------
local	STATUS_RESULT_HPADD = 25;
local	STATUS_RESULT_MPADD = 26;
local	PWORLD_NO_RESTORE_ITEM = 64;

function AutoRecoveryFrame_OnShow()
	if AutoAssistFrame:IsShown() then
		this:SetPoint( "topleft", "AutoAssistFrame", "topright", 0, 0 );
	else
		this:SetPoint( "center", "UIClient", "center", 0, 0 );
	end
	AutoRecoveryFrameFoodCount();
end

function AutoRecoveryFrameLimitHPRecoveryBarValueChange()
	local sliderValue	= this:GetValue();
   	local  hpFont		= getglobal("AutoRecoveryFrameLimitHPRecoveryBtnFont");
   	hpFont:SetText("生命值低于"..sliderValue.."%时使用食物");
	VipManager:getAutoRecoveryOpt().m_hpValue = sliderValue;
	VipManager:saveAutoRecoveryOpt();
end

function AutoRecoveryFrameLimitMPRecoveryBarValueChange()
	local sliderValue   = this:GetValue();
   	local  hpFont = getglobal("AutoRecoveryFrameLimitMPRecoveryBtnFont");
   	hpFont:SetText("魔法值低于"..sliderValue.."%时使用食物");
	VipManager:getAutoRecoveryOpt().m_mpValue = sliderValue;
	VipManager:saveAutoRecoveryOpt();
end

function AutoRecoveryLoadSevData()
	local	structData	=	VipManager:getAutoRecoveryOpt();
	local	hpSlider = getglobal("AutoRecoveryFrameLimitHPRecoveryBar");
	hpSlider:SetValue( structData.m_hpValue );
	local	mpSlider = getglobal("AutoRecoveryFrameLimitMPRecoveryBar");
	mpSlider:SetValue( structData.m_mpValue );
	local	mpBtn	= getglobal("AutoRecoveryFrameMPMedicineBtn");
	local	hpBtn	= getglobal("AutoRecoveryFrameHPMedicineBtn");
	if structData.m_hpItemID ~= 0 then
		hpBtn:SetClientUserData(0,structData.m_hpItemID);
		local	hpItemDef	= getItemDef(structData.m_hpItemID);
		local	hpIconTex 	= getglobal("AutoRecoveryFrameHPMedicineBtnIcon");
		local	szPath 		= GetItemEquipIconPath()..hpItemDef.IconID..".tga";
		hpIconTex:SetTexture(szPath);
	else
		hpBtn:SetClientUserData(0,0);
		local	szPath		= "uires\\ui\\mask\\TouMingTongDao.tga";
		local	hpIconTex 	= getglobal("AutoRecoveryFrameHPMedicineBtnIcon");
		hpIconTex:SetTexture(szPath);
	end
	if structData.m_mpItemID ~= 0 then
		mpBtn:SetClientUserData(0,structData.m_mpItemID);
		local	mpItemDef	= getItemDef(structData.m_mpItemID);
		local	mpIconTex 	= getglobal("AutoRecoveryFrameMPMedicineBtnIcon");
		local	szPath 		= GetItemEquipIconPath()..mpItemDef.IconID..".tga";
		mpIconTex:SetTexture(szPath);
	else
		mpBtn:SetClientUserData(0,0);
		local	szPath		= "uires\\ui\\mask\\TouMingTongDao.tga";
		local	mpIconTex 	= getglobal("AutoRecoveryFrameMPMedicineBtnIcon");
		mpIconTex:SetTexture(szPath);
	end
	AutoRecoveryFrameFoodCount();
end

local function checkUseLv( itemDef )
	local	mainplayer	= ActorMgr:getMainPlayer();
	if itemDef.WearPre.RoleLevelMin > mainplayer:getLv() then
		return false;
	elseif itemDef.WearPre.RoleLevelMax ~= 0 and itemDef.WearPre.RoleLevelMax < mainplayer:getLv() then
		return false;
	end
	return true;
end

function AutoRecoveryFrameReceiveDrag( szBtnName, itemDef )
	if not checkUseLv( itemDef ) then
		return ShowMidTips("当前等级无法使用该物品");
	end
	local btn = getglobal( szBtnName );
	if string.find( szBtnName, "HPMedicine", 1, true ) then
		if isFood( itemDef,STATUS_RESULT_HPADD ,RESULT_HP_ADD ) then
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			local tex	= getglobal(szBtnName.."Icon");
			tex:SetTexture( szPath );
			btn:SetClientUserData(0,itemDef.ItemID);
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			VipManager:getAutoRecoveryOpt().m_hpItemID = itemDef.ItemID;
			AutoRecoveryFrameFoodCount( szBtnName );
		else
			return ShowMidTips("请放入恢复生命值的物品");
		end
	else
		if isFood( itemDef,STATUS_RESULT_MPADD ,RESULT_MP_ADD ) then
			local szPath	= GetItemEquipIconPath()..itemDef.IconID..".tga";
			local tex	= getglobal(szBtnName.."Icon");
			tex:SetTexture( szPath );
			btn:SetClientUserData(0,itemDef.ItemID);
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			VipManager:getAutoRecoveryOpt().m_mpItemID = itemDef.ItemID;
			AutoRecoveryFrameFoodCount( szBtnName );
		else
			return ShowMidTips("请放入恢复魔法值的物品");
		end
	end
	VipManager:saveAutoRecoveryOpt();
end

function AutoRecoveryFrameFoodCount( szBtnName )
	local	mainPlayer	= ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end;
	local container		= mainPlayer:getContainer();

	if szBtnName ~= nil then
		local btn	= getglobal( szBtnName );
		local btnTex	= getglobal( szBtnName .. "Icon" );
		local itemId	= btn:GetClientUserData(0);
		if container:getItemCount( itemId ) ~= 0 then
			btnTex:SetGray( false );
		else
			btnTex:SetGray( true );
		end
	else
		local	hpBtn		= getglobal("AutoRecoveryFrameHPMedicineBtn");
		local	hpBtnTex	= getglobal("AutoRecoveryFrameHPMedicineBtnIcon");
		local	mpBtn		= getglobal("AutoRecoveryFrameMPMedicineBtn");
		local	mpBtnTex	= getglobal("AutoRecoveryFrameMPMedicineBtnIcon");
		local	itemHpId	= hpBtn:GetClientUserData(0);
		local	itemMpId	= mpBtn:GetClientUserData(0);

		local container		=	mainPlayer:getContainer();
		if itemHpId ~= 0 and container:getItemCount( itemHpId ) ~= 0 then
			hpBtnTex:SetGray( false );
		else
			hpBtnTex:SetGray( true );
		end
		if itemMpId ~= 0 and container:getItemCount( itemMpId ) ~= 0 then
			mpBtnTex:SetGray( false );
		else
			mpBtnTex:SetGray( true );
		end
	end
end

function VipHpCheckUpdate()
	local hpSlider		= getglobal("AutoRecoveryFrameLimitHPRecoveryBar");
	local hpBtn		= getglobal("AutoRecoveryFrameHPMedicineBtn");
	local itemId		= hpBtn:GetClientUserData(0);
	if itemId == 0 then return end;
	local sliderValue	= hpSlider:GetValue();
	local mainPlayer	= ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end

	local container = mainPlayer:getContainer();
	if container:getItemCount( itemId ) == 0 then
		return;
	end
	local hp	= mainPlayer:getHP();
	local maxHp	= mainPlayer:getMaxHP();
	if(hp < sliderValue * maxHp /100) then
		container:usedItem(itemId);
	end
end

function VipMpCheckUpdate()
	local mpSlider		= getglobal("AutoRecoveryFrameLimitMPRecoveryBar");
	local mpBtn		= getglobal("AutoRecoveryFrameMPMedicineBtn");
	local itemId		= mpBtn:GetClientUserData(0);
	if itemId == 0 then return end;
	local sliderValue	= mpSlider:GetValue();
	local mainPlayer	= ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end

	local container	= mainPlayer:getContainer();
	if container:getItemCount( itemId ) == 0 then
		return;
	end
	local mp	= mainPlayer:getMP();
	local maxMp	= mainPlayer:getMaxMP();
	if(mp < sliderValue * maxMp /100) then
		container:usedItem(itemId);
	end
end

function checkCanUseRecoveryItem()
	local nRealMapID = MapShower:getRealMapId();
	if nRealMapID >= MIN_COPY_ID then
		local pworldDef = getPworldDef( nRealMapID / 100 );
		if bitAnd( pworldDef.CtrlFlag, PWORLD_NO_RESTORE_ITEM ) then
			return false;
		end
	end
	return true;
end

function updateAutoRecovery()
	if AutoAssist:getAutoAssistBegin() then
		return;
	end
	if not checkCanUseRecoveryItem() then
		return;
	end

	VipHpCheckUpdate();
	VipMpCheckUpdate();
end