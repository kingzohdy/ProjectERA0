local t_LabelNameList =	{};		--标签名表
local nCurSelIndex = 0;
local MAX_ITEM_GIFT_NUM = 8; 	--/* 每日奖励最大次数 */
local MAX_CLAN_GIFT_CATEGORY_NUM = 20;

local szTitleColor		= "#c431D09";
local szContentColor	= "#c6f6149";

function ClanGiftFrame_OnLoad()
	this:RegisterEvent("GE_GIFT_UPDATE");
	this:RegisterEvent("GE_CLAN_SVR_INFO");
	this:RegisterEvent("GE_CLAN_DISABLEFRAME");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
end

function ClanGiftFrame_OnEvent()
	if arg1 == "GE_GIFT_UPDATE" or arg1 == "GE_CLAN_SVR_INFO" or arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if ClanGiftFrame:IsShown() then
			BondingClanGiftData();
		end
	end
	if arg1 == "GE_CLAN_DISABLEFRAME" then
		if ClanGiftFrame:IsShown() then
			ClanGiftFrame:Hide();
		end
	end
end

function ClanGiftFrame_OnShow()
	this:SetPoint( "center","UIClient","center",0,0 )
	nCurSelIndex = 0;
	BondingClanGiftData();
end

function GetClanGiftCategoryParentNodeInfo( childNodeInfo )
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType== 0 and t_LabelNameList[i].nType == childNodeInfo.nType then
			return t_LabelNameList[i];
		end
	end
end

function CanGetClanAward( idx )
	local onedaygift = GameWizard:GetClanOneDayGift( idx );
	local mainplayer = ActorMgr:getMainPlayer()
	if onedaygift.GiftCond.ClanTouch > 0 then
		if mainplayer:getLv() < CLAN_GIFT_TOUCH_EXP_LEVEL_TOLUA then
			return CAN_NOT_GET_GIFT
		end
		if GameClanManager:GetClanYesterdayTouchNum() == 0 then
			return CAN_NOT_GET_GIFT
		end		
	end
	if GameWizard:IsAlreadGetClanDayGift( idx ) then
		return ALREADY_GET_GIFT;
	end
	local cityInfo = GameClanManager:getClanCity():getCityInfo();
	if cityInfo.m_nLevel < onedaygift.GiftCond.ClanCityLevel then 
		return CAN_NOT_GET_GIFT
	end
	local clanViewer = GameClanManager:getClanViewer();
	local nIndex = nil;
	for i = 1, clanViewer.m_clanInfo.m_nClanMemNum do
		local member = clanViewer.m_Members[i-1];
		if CompareRoleName( mainplayer:getName(), member.RoleMember.m_szRoleName ) then
			nIndex = i - 1;
			break;
		end
	end
	if nIndex == nil then
		return GAN_NOT_GET_GIFT
	end
	if clanViewer.m_Members[nIndex].TotalContribute < onedaygift.GiftCond.TotalContri then
		return CAN_NOT_GET_GIFT
	end

	if clanViewer.m_clanInfo.m_nClanLevel == 1 then
		local levelUpClanDef = getClanLevelDef(clanViewer.m_clanInfo.m_nClanLevel);
		if levelUpClanDef.WorkingCost.Memory > clanViewer.m_clanInfo.m_nMoney then
			return CAN_NOT_GET_GIFT;
		end
	end
	
	return CAN_GET_GIFT;
end

function BondingClanGiftData()
	t_LabelNameList = {};
	local nNum = GameWizard:GetClanOneDayGift();
	local nSubType = 1;	
	for i = 1, nNum do
		local onedaygift = GameWizard:GetClanOneDayGift( i - 1 );
		local nRet		 = -1;
		local clanLv = GameClanManager:getClanViewer().m_clanInfo.m_nClanLevel
		local bFound = false;
		if onedaygift.GiftCond.ClanLevel == 0 or onedaygift.GiftCond.ClanLevel == clanLv then
			for _, data in ipairs(t_LabelNameList) do
				if data.nType == onedaygift.TypeName then
					bFound = true;
				end
			end
			if not bFound then				
				table.insert(t_LabelNameList,{ name=onedaygift.TypeName,  bCheck = true, nType = onedaygift.TypeName, nSubType = 0, nDataIndex = -1*i, nAwardState = 0, 0 });
			end
			nRet = CanGetClanAward( i - 1 );
			table.insert(t_LabelNameList,{ name=onedaygift.Name, bCheck=true, nType=onedaygift.TypeName, nSubType=nSubType, nDataIndex = i, nAwardState = nRet, id = onedaygift.GiftID } );
			nSubType = nSubType + 1;
		end
	end

	UpdateClanGiftCategoryList( 1 )
	Quest:updateNpcQuestTitle();
end

--刷新标签页
function UpdateClanGiftCategoryList( nStartContentIndex )
	local nCurShowBtn			= 1; -- 此为指向按钮的指针
	local t_selHilightBtnInfo	= { haveSel = false, initCurSelIndex = nCurSelIndex };
	local nCurHilightBtnIndex	= 0;
	local nSubLabelNum = 1;
	local bFound = false;
	local oldonedaygift = nil;
	if nCurSelIndex ~= 0 then
		oldonedaygift = GameWizard:GetClanOneDayGift( nCurSelIndex - 1 );
	end
	for nContentIndex = nStartContentIndex, table.getn( t_LabelNameList ) do
		-- 检查按钮指针是否越界
		if nCurShowBtn > MAX_CLAN_GIFT_CATEGORY_NUM then
			break;
		end
		local btn				= getglobal( "ClanGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtn" );
		local font				= getglobal( "ClanGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnType" );  
		local subTypeFont		= getglobal( "ClanGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnSubType" );
		local awardStateFont	= getglobal( "ClanGiftFrame_CategoryBtn" .. nCurShowBtn .. "HighlightBtnAwardState" );
		
		btn:SetClientUserData( 0, 0 );
		btn:DisChecked();
		-- 若为1级节点
		if t_LabelNameList[nContentIndex].nSubType == 0 then
			font:SetText( t_LabelNameList[nContentIndex].name );
			subTypeFont:SetText("");
			awardStateFont:SetText( "" );
			btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nDataIndex);

			btn:SetNormalTextureTemplate( "GongYongAnNiuA1NormalTemplate" );
			btn:SetHighlightTextureTemplate( "GongYongAnNiuA1HighTemplate" );
			btn:SetPushedTextureTemplate( "GongYongAnNiuA1PushedTemplate" );
			btn:SetDisableTextureTemplate( "GongYongAnNiuA1DisabledTemplate" );
			font:SetTextColor( 255, 254, 189 );
			btn:Show();
			nCurShowBtn = nCurShowBtn + 1;
		-- 若为2级节点
		else
			-- 先查看其父节点是否为展开状态
			local firstLevelNode = GetClanGiftCategoryParentNodeInfo( t_LabelNameList[nContentIndex] );
			if nCurSelIndex == 0 and t_LabelNameList[nContentIndex].nAwardState == CAN_GET_GIFT then
				nCurSelIndex	= t_LabelNameList[nContentIndex].nDataIndex; 				
				DisCheckAllHighLightBtn( "ClanGiftFrame_CategoryBtn", MAX_CLAN_GIFT_CATEGORY_NUM );
				btn:DisHighlight();
				btn:Checked();
			elseif nCurSelIndex == t_LabelNameList[nContentIndex].nDataIndex then
				DisCheckAllHighLightBtn( "ClanGiftFrame_CategoryBtn", MAX_CLAN_GIFT_CATEGORY_NUM );
				btn:DisHighlight();
				btn:Checked();
			end

			-- 若其父节点为展开状态, 则显示当前的2级节点内容,否则跳过当前2级节点
			if firstLevelNode.bCheck then
				local t_AwardState = 
					{
						[-1]					= { text = "",			r = 52,  g = 139, b = 116	},
						[CAN_NOT_GET_GIFT]		= { text = "",			r = 52,  g = 139, b = 116	},
						[CAN_GET_GIFT]			= { text = "可领取",	r = 10,  g = 215, b = 0		},
						[ALREADY_GET_GIFT]		= { text = "已领取",	r = 255, g = 60,  b = 0		},
					};
				subTypeFont:SetText( t_LabelNameList[nContentIndex].name ); 
				local awarddata = t_AwardState[t_LabelNameList[nContentIndex].nAwardState];
				awardStateFont:SetText( awarddata.text );
				awardStateFont:SetTextColor(awarddata.r, awarddata.g, awarddata.b );
				font:SetText("");
				btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nDataIndex );
				
				if math.mod( nSubLabelNum,2 ) == 0 then
					btn:SetNormalTextureTemplate( "WhiteCategoryBkg" );
				else
					btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
				end
				btn:SetHighlightTextureTemplate( "HighlightTextureTemplate" );
				btn:SetPushedTextureTemplate( "PushedTextureTemplate" );
				btn:SetDisableTextureTemplate( "TransTextureTemplate" );
				btn:Show();
				nCurShowBtn = nCurShowBtn + 1;
				nSubLabelNum = nSubLabelNum + 1;
			end
			if t_LabelNameList[nContentIndex].nDataIndex == nCurSelIndex then				
				bFound = true
			end
			if not bFound and oldonedaygift ~= nil and oldonedaygift.GiftCond.ClanLevel > 0 then
				-- 说明是公会礼包
				local onedaygift = GameWizard:GetClanOneDayGift( t_LabelNameList[nContentIndex].nDataIndex - 1 );
				if onedaygift ~= nil and onedaygift.GiftCond.ClanLevel > 0 then
					nCurSelIndex = t_LabelNameList[nContentIndex].nDataIndex;
					bFound = true;
				end
			end
		end
	end
	
	if nCurSelIndex == 0 or not bFound then
		for i=1,nCurShowBtn do
			local btn = getglobal( "ClanGiftFrame_CategoryBtn" .. i .. "HighlightBtn" )
			if btn:GetClientUserData( 0 ) > 0 then
				nCurSelIndex	= btn:GetClientUserData( 0 );				
				DisCheckAllHighLightBtn( "ClanGiftFrame_CategoryBtn", MAX_CLAN_GIFT_CATEGORY_NUM );
				btn:DisHighlight();
				btn:Checked();
				break;
			end
		end
	end
	if nCurShowBtn == 1 then
		nCurSelIndex = 0;
	end

	-- 将没使用的按钮隐藏起来
	for i = nCurShowBtn, MAX_CLAN_GIFT_CATEGORY_NUM do
		local btn	= getglobal( "ClanGiftFrame_CategoryBtn" .. i .. "HighlightBtn" );
		btn:Hide();
		local font	= getglobal( "ClanGiftFrame_CategoryBtn" .. i .. "HighlightBtnType" );
		font:SetText( "" );
	end
	BondingClanGifDesc();
end

function BondingClanGiftItemInfo( itemdef, itemIndex, nItemCount )
	local Btn = getglobal("ClanGiftFrame_ItemBtn"..itemIndex);
	local itemBtn = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItem");
	local itemTex = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItemIconTexture");
	local itemCountFont = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItemCountFont");
	local itemName = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItemFont");
	local itemNameTex = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItemTex");
	local itemBackTex = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItemBoxTexture");	
	local itemUVTex = getglobal("ClanGiftFrame_ItemBtn"..itemIndex.."AwardItemUVAnimationTex");
	itemBackTex:SetTexUV(794, 562, 44, 44);
	itemCountFont:SetText("");
	itemBackTex:Show();
	itemNameTex:Show();
	itemName:Show();

	if itemIndex%2 == 0 then
		Btn:SetPoint("TOPLEFT","ClanGiftFrameBackBtn","TOPLEFT", 360,100 + (math.ceil(itemIndex/2)-1)*50);
	else
		Btn:SetPoint("TOPLEFT","ClanGiftFrameBackBtn","TOPLEFT", 200,100 + (math.ceil(itemIndex/2)-1)*50);
	end

	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	itemTex:SetTexture( IconPath );
	itemTex:SetTexRelUV( 1, 1 );
	itemCountFont:SetPoint( "bottomleft", Btn:GetName(), "bottomleft", 0, 10 );
	
	SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());
	itemUVTex:SetTexRelUV( 1, 1 );

	local szItemName = itemdef.Name;
	szItemName = szItemName.."\n数量："..nItemCount;
	
	itemName:SetText( szItemName );
	Btn:Show();
	itemBtn:SetClientUserData( 1, itemdef.ItemID );
end

function BondingClanGifDesc()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end

	for i = 1, MAX_ITEM_GIFT_NUM do
		local Btn = getglobal("ClanGiftFrame_ItemBtn"..i);
		local itemName = getglobal("ClanGiftFrame_ItemBtn"..i.."AwardItemFont");
		Btn:Hide();
		itemName:Hide();
	end

	ClanGiftFrame_GetAwardContentRich:SetText("",0,0,0);
	ClanGiftFrameGetAwardRequireBtnFont:SetText("");
	ClanGiftFrame_GetAwardBtn:Disable();
	ClanGiftFrame_GetAwardContentRich:Hide();
	ClanGiftFrameGuildActivityBtn1:Hide();
	ClanGiftFrameGuildActivityBtn2:Hide();
	ClanGiftFrameGetAwardContentBtn:Hide();
	ClanGiftFrameBackBtnSepTex:Hide();	

	local szGiftLimit	= "";
	local szGiftContent = "#n"..szTitleColor.."礼包内容：#n";

	if nCurSelIndex == 0 then
		return;
	end

	local onedaygift = GameWizard:GetClanOneDayGift( nCurSelIndex - 1 );
	if onedaygift == nil then
		return
	end

	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nDataIndex == nCurSelIndex and t_LabelNameList[i].nAwardState == CAN_GET_GIFT then
			ClanGiftFrame_GetAwardBtn:Enable();
		end
	end

	if onedaygift.GiftCond.ClanLevel > 0 then
		ClanGiftFrame_GetAwardContentRich:Show();
		local szText;
		if onedaygift.GiftCond.ClanCityLevel > 0 then
			szText = "（此礼包拥有公会城市后,并且个人贡献度达到" .. onedaygift.GiftCond.TotalContri .. "才可领取）";
		end
		-- 公会礼包
		ClanGiftFrame_GetAwardContentRich:SetText( szGiftContent, 0, 0, 0 )
		ClanGiftFrameGetAwardRequireBtnFont:SetText( szText );
		local itemIndex = 1;
		for i = 1, MAX_ITEM_GIFT_NUM do
			if onedaygift.GiftItems[0].GiftItems[i-1].ItemID ~= 0 then
				local itemDef = getItemDef( onedaygift.GiftItems[0].GiftItems[i-1].ItemID );
				local nItemCount = onedaygift.GiftItems[0].GiftItems[i-1].ItemNum;
				if itemDef ~= nil then
					BondingClanGiftItemInfo( itemDef, itemIndex, nItemCount );
					itemIndex = itemIndex + 1;
				end
			end
		end

		local clanViewer = GameClanManager:getClanViewer();
		local nIndex;
		for i = 1, clanViewer.m_clanInfo.m_nClanMemNum do
			local member = clanViewer.m_Members[i-1];
			if MainPlayer:getName() == member.RoleMember.m_szRoleName then
				nIndex = i - 1;
				break;
			end
		end
		if clanViewer.m_Members[nIndex].TotalContribute < onedaygift.GiftCond.TotalContri then
			ClanGiftFrame_GetAwardBtn:Disable();
		end
	else
		ClanGiftFrame_GetAwardBtn:Enable();
		ClanGiftFrameGetAwardContentBtn:Show();
		ClanGiftFrameGuildActivityBtn1:Show();
		ClanGiftFrameGuildActivityBtn2:Show();
		ClanGiftFrameBackBtnSepTex:Show();		
		local clanViewer = GameClanManager:getClanViewer()
		local levelup = getLevelUP( MainPlayer:getLv() )
		local baseExp = math.floor( levelup.ContributeExp/80 )
		if baseExp < 1 then baseExp = 1 end
		local clanLvExp = math.floor( levelup.ContributeExp/400 ) * clanViewer.m_clanInfo.m_nClanLevel;
		if clanLvExp < clanViewer.m_clanInfo.m_nClanLevel then clanLvExp = clanViewer.m_clanInfo.m_nClanLevel end
		local num = GameClanManager:GetClanYesterdayTouchNum();
		local clanTouchExp = math.floor( levelup.ContributeExp/120000 ) * num * 10;
		local clanContribute = clanViewer.m_clanInfo.m_nClanLevel * 10
		local clanAnDing = clanViewer.m_clanInfo.m_nBuildingValue;
		local clanAnDingExp = 0;
		if clanAnDing < 1 then
			clanAnDingExp = 0;
		elseif clanAnDing < 5001 then
			clanAnDingExp = 1;
		elseif clanAnDing < 7001 then
			clanAnDingExp = 5
		elseif clanAnDing < 9001 then
			clanAnDingExp = 10;
		else
			clanAnDingExp = 20;
		end

		local clanPworld = MainPlayer:getOldDayPworldScore();
		local clanPworldExp;
		if clanPworld > 0 then
			clanPworldExp = 30 * clanPworld;
		else
			clanPworldExp = 0;
		end
		ClanGiftFrameGetAwardContentBtnExpFont:SetText( baseExp + clanLvExp + clanTouchExp );
		ClanGiftFrameGetAwardContentBtnContriFont:SetText( clanContribute + clanAnDingExp + clanPworldExp );
		ClanGiftFrameGetAwardContentBtnBaseExpFont:SetText( baseExp );
		ClanGiftFrameGetAwardContentBtnLevelFont:SetText( "+" .. clanLvExp );
		ClanGiftFrameGetAwardContentBtnTouchFont:SetText( "+" .. clanTouchExp );
		ClanGiftFrameGetAwardContentBtnBaseContriFont:SetText( clanContribute );
		ClanGiftFrameGetAwardContentBtnStableFont:SetText( "+" .. clanAnDingExp );
		ClanGiftFrameGetAwardContentBtnPworldFont:SetText( "+" .. clanPworldExp );
	end	
end

function ClanPworldFrame_OnEnter()
	local szText = "点击打开“公会活动助手”\n——驱逐入侵";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
end

function ClanOpenTouchTipFrame_OnEnter()
	local szText = "点击打开“公会活动助手”\n---公会宝藏";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
								button = this:GetName(), horizonal = "right" } );
end

function ClanGiftFrame_CategoryBtn_OnClick()
	if this:GetClientUserData( 0 ) ~= 0 then
		nCurSelIndex = this:GetClientUserData( 0 ); 
	end

	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nDataIndex == this:GetClientUserData( 0 ) then
			t_LabelNameList[i].bCheck = not t_LabelNameList[i].bCheck;	
		end
	end
	UpdateClanGiftCategoryList( 1 );
end

function ClanGiftFrame_GetAwardBtn_OnClick()
	local onedaygift = GameWizard:GetClanOneDayGift( nCurSelIndex - 1 );
	nGiftId = onedaygift.GiftID;
	nType = CLIENT_GIFT_TYPE_ONEDAY;
	GameWizard:sendGiftRequest( 0, nGiftId, nType );
end

-----------------------------领取低等级经验补偿---------------------------------
RETURN_GIFT_MAX_DAY = 30
--是否有回归奖励礼包
function HasOfflineGift( nDay )
	if nDay == 0 then return false end
	--大于30天的读取30天的表
	if nDay > RETURN_GIFT_MAX_DAY then
		nDay = RETURN_GIFT_MAX_DAY
	end
	local buChangDef = GetBuChangDef( nDay )
	for i = 1, MAX_BUCHANG_ITEM_TOLUA do
		if buChangDef.GiftItem[i-1].ItemID > 0 and buChangDef.GiftItem[i-1].ItemNum > 0 then
			return true
		end
	end
	return false
end
function OpenOfflineExpBuffFrame( nExp, nDay )
	--跨服状态不允许领取奖励
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isOutSide() then return end
	if not HasOfflineGift( nDay ) then
		--如果没有师徒关系
		if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_NONE_TOLUA and mainplayer:getLv() < 60 then
			GuidToSchoolNpcFrame:Show()		
		else
			if nExp == 0 then return end
			local font = getglobal( "OfflineExpBuffFrameFont" );
			font:SetText( "累计经验：" .. nExp );
			OfflineExpBuffFrame:Show();
		end
	else
		OpenOfflineGiftFrame( nExp, nDay )
	end
end

function OfflineExpBuffFrameConfirmBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		mainplayer:requestOfflineExpBuff();
		util.closeParent(this);
	end
end
function OfflineExpBuffFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		if mainplayer:isOutSide() then
			this:Hide()
		end
	end
end
------------------------------找寻师徒npc面板------------------------
function GuidToSchoolNpcFrame_OnLoad()
	GuidToSchoolNpcFrameRich:SetText( "#cffffbe与玩家建立师生关系，您将可以在每日上线时领取海量的经验，具体相关事宜请询问时间城的#L艾吉奥@@[304123,1000]#n",255,255,190 )
end
function GotoSchoolNpc()
	local szLinkInfo = "[304123,1000]"
	local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( szLinkInfo );
	GuidToSchoolNpcFrame:Hide()
	if bLinkNpc == true then
		MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
		return;
	end
	local nMapId, nMapPosX, nMapPosY = GetLinkPos( szLinkInfo );
	if nMapId ~= 0 then
		MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
	end
end
