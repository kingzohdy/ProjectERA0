local pickTmpItemId	= 0;
local MailNEEDITEM	= 5050001;
local MsgR = 215;
local MsgG = 190;
local MsgB = 132;

local BufferTips = ""
local BufferTimer = 0
local BufferBegin = 0
local TeamSummonTimer = 0
local ClanSummonMap   = 0
local ClanSummonTimer = 0
local MAX_SAVE_MAP_NUM	= 5
local MAX_TRAD_ITEM_NUM = 8
function MessageBox(title, note)
	--清空上一次锁定	
	ClearItemLock( "MessageBoxFrame" );
	MessageBoxFrame:Show();
	MessageBoxFrame:SetClientString("");
	MessageText:clearHistory();
	MessageTitle:SetText(title);
	MessageText:SetText(note,MsgR,MsgG,MsgB);
	--设置按钮的显示状态和位置
	local okBtn = getglobal("MessageBoxAcceptButton");
	local cancelBtn = getglobal("MessageBoxCancelButton");
	okBtn:SetPoint("bottomleft","MessageBoxFrame","bottomleft",15,-5);
	okBtn:Show();
	cancelBtn:Show();
	MessageBoxHideBtn:Hide();
	MessageBoxHideBtn:setCheckState( false );
end

function MessageBoxForGame(title, note)
	MessageBoxForGameFrame:Show();
	MessageBoxForGameFrame:SetClientString("");
	MessageForGameText:clearHistory();
	MessageForGameTitle:SetText(title);
	MessageForGameText:SetText(note,MsgR,MsgG,MsgB);

	--设置按钮的显示状态和位置
	local okBtn = getglobal("MessageBoxForGameAcceptButton");
	local cancelBtn = getglobal("MessageBoxForGameCancelButton");
	okBtn:SetPoint("bottomleft","MessageBoxForGameFrame","bottomleft",5,-5);
	okBtn:Show();
	cancelBtn:Show();
end

-- 针对宠物吞噬的 messagebox
function MessageBoxForPet( title, note1,note2 )
	MessageBoxForPetFrame:Show();
	MessageBoxForPetFrame:SetClientString("");
	MessageBoxForPetText:clearHistory();
	MessageBoxForPetConfirmText:clearHistory();
	MessageBoxForPetTitle:SetText(title);
	MessageBoxForPetText:SetText(note1,255,255,190);
	MessageBoxForPetConfirmText:SetText(note2,255,255,190);
	--设置按钮的显示状态和位置
	local okBtn = getglobal("MessageBoxForPetAcceptButton");
	local cancelBtn = getglobal("MessageBoxForPetCancelButton");
--	okBtn:SetPoint("bottomleft","MessageBoxForPetFrame","bottomleft",5,-5);
	okBtn:Show();
	cancelBtn:Show();
	cancelBtn:SetText("取消");
	if title == "宠物吞噬" then 
		MessageBoxForPetFrame_TipsBtn:Show();
	else 
		MessageBoxForPetFrame_TipsBtn:Hide();
	end
end

function MessageBoxForPetOnLoad()
	MessageBoxForPetText:setCenterLine( true );
	MessageBoxForPetConfirmText:setCenterLine( true )
end
function MessageBoxForPetFrame_OnHide()
	ClearItemLock( "MessageBoxForPetFrame" );
end

function MessageBoxForPetFrame_TipsBtn_OnEnter()
	local szText = "#Y不离不弃#n关系说明：\n1.宠物所有基础属性提高10%\n2.宠物忠诚度上限提高20%\n3.宠物无法被再次交易";
	SetGameTooltips(this:GetName(),szText);
end

function MessageBoxForPetAcceptBtnOnClick()
	MessageBoxForPetFrame:Hide();
	if MessageBoxForPetFrame:GetClientString() == "宠物吞噬损失" then 
		PetAttrEatFrameStart();
	elseif MessageBoxForPetFrame:GetClientString() == "宠物转星损失" then
		PetStarMoveFrameStart();
	elseif MessageBoxForPetFrame:GetClientString() == "熔岩召唤确认" then
		RongYanSelectConfirmStart();
		return;
	elseif MessageBoxForPetFrame:GetClientString() == "熔岩骇心不足" then
		RongYanSelectItemLack();
	end
	MessageForGameText:Clear();
	MessageForGameText:clearHistory();
	MessageBoxForPetConfirmText:Clear();
	MessageBoxForPetConfirmText:clearHistory();
	MessageBoxForPetFrame:SetClientString("");
	MessageBoxForPetFrame:SetClientUserData( 0, 0 ); 
	MessageBoxForPetFrame:SetClientUserData( 1, 0 );	
	MessageBoxForPetFrame:SetClientUserData( 2, 0 ); 
	MessageBoxForPetFrame:SetClientUserData( 3, 0 ); 
end
function MessageBoxForPetCancelBtnOnClick()
	if this:GetText() == "快速购买" then 
		QuickBuyRongYanItem();
	else 
		MessageBoxForPetText:SetText("",MsgR,MsgG,MsgB);
		MessageBoxForPetFrame:Hide();	
	end
end

function MessageBoxFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

function MessageBoxFrame_OnHide()
	ClearItemLock( "MessageBoxFrame" );
end

function MessageBoxForGameFrame_onHide()
end

--只有一个确定按钮的messagebox
function MessageBoxOK(title,note)
	MessageBoxFrame:Show();
	MessageText:clearHistory();
	MessageTitle:SetText(title);
	MessageText:SetText(note,MsgR,MsgG,MsgB);
	--设置按钮的显示状态和位置
	local okBtn = getglobal("MessageBoxAcceptButton");
	local cancelBtn = getglobal("MessageBoxCancelButton");
	okBtn:SetPoint("center","MessageBoxFrame","center",0,45);
	okBtn:Show();
	cancelBtn:Hide();
	MessageBoxHideBtn:Hide();
end

--消息提示窗口确定
function MessageBoxAcceptBtnOnClick()
	MessageBoxFrame:Hide();
	--确定删除角色
	if(MessageBoxFrame:GetClientString() == "确定删除角色") then
		LoginMgr:requestDeleteSelectedRole();
	elseif(MessageBoxFrame:GetClientString() == "确定丢弃物品") then
		TropItem();
		playUISound( "sounds\\UI\\diuqi.wav"  );
	elseif(MessageBoxFrame:GetClientString() == "使用临时物品") then
		ActorMgr:pickTmpItem( pickTmpItemId );
	elseif (MessageBoxFrame:GetClientString() == "确认放弃任务") then
		Quest:dropQuest( MessageBoxFrame:GetClientUserData( 0 ) );
		nCurSelectQuestId	= 0;
		nCurSelectRecycleId	= 0;
		HideTaskLogFrameTaskLogInfo();
	elseif (MessageBoxFrame:GetClientString() == "护送任务失败") then
		Quest:dropQuest( MessageBoxFrame:GetClientUserData(0) );
	elseif (MessageBoxFrame:GetClientString() == "取消称号") then
		GameDesignation:sendSetDesignationReq( 0 );
	elseif (MessageBoxFrame:GetClientString() == "设置称号") then
		GameDesignation:sendSetDesignationReq( MessageBoxFrame:GetClientUserData(0) );
	elseif ( MessageBoxFrame:GetClientString() == "购买时装" ) then
		local nBuyID = MessageBoxFrame:GetClientUserData(1);
		local itemNum = MessageBoxFrame:GetClientUserData(2);
		local buyType = MessageBoxFrame:GetClientUserData(3);
		MarketManager:requestBuy(nBuyID,itemNum,0,"","",buyType);
	elseif ( MessageBoxFrame:GetClientString() == "覆盖附魔" ) or ( MessageBoxFrame:GetClientString() == "绑定附魔" ) then
		EnchantItem();
	elseif ( MessageBoxFrame:GetClientString() == "覆盖刻印" ) or ( MessageBoxFrame:GetClientString() == "绑定刻印" ) then
		KeyinItem();
	elseif MessageBoxFrame:GetClientString() == "符文镶嵌" then
		RuneInsert();
	elseif MessageBoxFrame:GetClientString() == "符文摘取" then
		RuneErase();
 	elseif ( MessageBoxFrame:GetClientString() == "隐藏界面" ) then
		GameMgr:changeShowUI();
		bFirstChange = false;
	elseif ( MessageBoxFrame:GetClientString() == "服务器提示" ) then
		local nBindType = MessageBoxFrame:GetClientUserData( 0 );
		local nID = MessageBoxFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_YES_TOLUA );
		nAcceptTime = 0;
	elseif ( MessageBoxFrame:GetClientString() == "领取福利" ) then
		GameWizard:sendGiftRequest( 0,MessageBoxFrame:GetClientUserData(0),CLIENT_GIFT_TYPE_ACCGIFT );
	elseif ( MessageBoxFrame:GetClientString() == "装备强化" ) then
		local nListType = MessageBoxFrame:GetClientUserData( 0 );
		local nGridIdx	= MessageBoxFrame:GetClientUserData( 1 );
		GameArmProce:armAddStar( nListType,nGridIdx );
	elseif( MessageBoxFrame:GetClientString() == "装备强化保存" ) then
		ArmProceStarFrameSaveBtn_onClick();
	elseif( MessageBoxFrame:GetClientString() == "完美强化提示" ) then
		ArmProceStarFrameSaveBtn_onClick();
	elseif( MessageBoxFrame:GetClientString() == "装备强化绑定提示" ) then
		RequestArmStarAdd();
	elseif( MessageBoxFrame:GetClientString() == "品质转移绑定提示" ) then
		CallArmProceStarMove();
	elseif( MessageBoxFrame:GetClientString() == "品质转移星级覆盖提示" ) then
		CallArmProceStarMove();
	elseif MessageBoxFrame:GetClientString() == "按键设置" then
		KeySettingFrameOnSave()
	elseif MessageBoxFrame:GetClientString() == "恢复键位设置" then
		KeySettingFrameOnResetAll()
	elseif MessageBoxFrame:GetClientString() == "丢弃机械" then
		local player = ActorMgr:getMainPlayer();
		if player == nil then return end
		local eqiup = player:getEquip();
		eqiup:looseTmpArm();
		local btn1 = getglobal("TempMagicShortcut1");
		local btn2 = getglobal("TempMagicShortcut2");
		btn1:SetClientUserData( 0, 0 );
		btn2:SetClientUserData( 0, 0 );
	elseif MessageBoxFrame:GetClientString() == "公会购买" then
		NpcShop:buyItem( MessageBoxFrame:GetClientUserData(0), 1 );
	elseif MessageBoxFrame:GetClientString() == "批量重铸" then
		ArmRecastSelectAttrFrame:Hide();
	elseif MessageBoxFrame:GetClientString() == "替换重铸装备" then
		local btn = getglobal("ArmAdvRecastFrameArmItemBtn")
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );	
		UnLockItem( nListType, nGridIdx, 0 );--解锁
		local srcList = MessageBoxFrame:GetClientUserData( 0 )
		local srcGrid = MessageBoxFrame:GetClientUserData( 1 )
		LockItem( srcList, srcGrid );
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		ArmAdvRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameSaveSuccessBtn:Hide();
		ArmRecastSelectAttrFrame:Hide();
		UpdateAdvArmRecastFrame();
	elseif MessageBoxFrame:GetClientString() == "重新单次重铸装备" then
		local nCurMoneyType = MessageBoxFrame:GetClientUserData( 0 )
		local armListType = MessageBoxFrame:GetClientUserData( 1 )
		local armGridIndex = MessageBoxFrame:GetClientUserData( 2 )
		ArmRecastSelectAttrFrame:Hide();
		GameArmProce:advanceRecastArm( nCurMoneyType,armListType,armGridIndex );	
	elseif MessageBoxFrame:GetClientString() == "绑定重铸" then
		local stuffItemId = MessageBoxFrame:GetClientUserData( 0 )
		local nList = MessageBoxFrame:GetClientUserData( 1 )
		local nGrid = MessageBoxFrame:GetClientUserData( 2 )
		local nBind = MessageBoxFrame:GetClientUserData( 3 )
		local must = 0
		if CheckItemResult( getItemDef(stuffItemId), RESULT_ARM_RECAST_MUST ) then
			must = 1
		end
		GameArmProce:normalRecastArm( stuffItemId, nList, nGrid, nBind, must )
	elseif MessageBoxFrame:GetClientString() == "金券重铸未绑定装备" then
		local nCurMoneyType = MessageBoxFrame:GetClientUserData( 0 )
		local armListType = MessageBoxFrame:GetClientUserData( 1 )
		local armGridIndex = MessageBoxFrame:GetClientUserData( 2 )
		if MessageBoxFrame:GetClientUserData( 3 ) > 0 then
			GameArmProce:batchRecastArm( nCurMoneyType,armListType,armGridIndex );
		else
			ArmRecastSelectAttrFrame:Hide();
			GameArmProce:advanceRecastArm( nCurMoneyType,armListType,armGridIndex );
		end
	elseif MessageBoxFrame:GetClientString() == "金券重置未绑定装备宝石" then
		local nIndex = MessageBoxFrame:GetClientUserData( 0 )
		local nType = MessageBoxFrame:GetClientUserData( 1 )
		local nList = MessageBoxFrame:GetClientUserData( 2 )
		local nGrid = MessageBoxFrame:GetClientUserData( 3 )
		GameArmProce:resetGem( nIndex, nType, nList, nGrid );
	elseif MessageBoxFrame:GetClientString() == "重铸面板关闭" or MessageBoxFrame:GetClientString() == "强化面板关闭"
			or MessageBoxFrame:GetClientString() == "批量重铸面板关闭" then
		ArmRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameArmInfo:Hide();
		ArmRecastFrame:Hide();
		ArmAdvRecastFrame:Hide();
		ArmProceStarFrame:Hide();		
	elseif MessageBoxFrame:GetClientString() == "切换高级重铸" then
		ArmRecastFrameArmInfo:Hide();
		ArmRecastFrameAdvRecastPanelBtn_OnClick();
	elseif MessageBoxFrame:GetClientString() == "宝石重置保存提示" then
		GemProceResetFrameCurResetInfoBtnSaveBtn_onClick();
		EnableGemProceResetFrame(true);
	elseif MessageBoxFrame:GetClientString() == "切换重铸" then
		ArmAdvRecastFrameArmInfo:Hide();
		ArmRecastSelectAttrFrame:Hide();
		ArmAdvRecastFrameRecastPanelBtn_OnClick();
	elseif MessageBoxFrame:GetClientString() == "绑定洗炼" then
		local stuffItemId = MessageBoxFrame:GetClientUserData( 0 )
		local nList = MessageBoxFrame:GetClientUserData( 1 )
		local nGrid = MessageBoxFrame:GetClientUserData( 2 )
		local nBind = MessageBoxFrame:GetClientUserData( 3 )
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local arm = container:getItem( nList, nGrid );
		if ArmWashFrame:IsShown() then
			removeWashItemLock( arm );
			local must = 0
			if CheckItemResult( getItemDef(stuffItemId), RESULT_ARM_WASH_STUFF_MUST ) then
				must = 1
			end
			GameArmProce:requestArmWash( stuffItemId, nList, nGrid, nBind, 0, 0, must );
		else
			GameArmProce:requestArmWash( stuffItemId, nList, nGrid, nBind, getWashItemLockValue(arm), getArmSafe(arm), 0 );
		end
	elseif MessageBoxFrame:GetClientString() ==  "锁定洗炼" then
		local stuffItem = ArmWashFrameStuffItemBtn:GetClientUserData( 2 )
		local nBind = ArmWashFrameStuffItemBtn:GetClientUserData( 3 );
		local armListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
		local armGridIndex = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local arm = container:getItem( armListType,armGridIndex );
		if not arm:isItemBind() and nBind == 1 then
			MessageBox("绑定提示","继续洗炼则当前装备会被绑定，是否继续洗炼？");
			MessageBoxFrame:SetClientString("绑定洗炼");
			MessageBoxFrame:SetClientUserData( 0, stuffItem )
			MessageBoxFrame:SetClientUserData( 1, armListType )
			MessageBoxFrame:SetClientUserData( 2, armGridIndex )
			MessageBoxFrame:SetClientUserData( 3, nBind )
		else
			removeWashItemLock( arm );
			local must = 0 
			if CheckItemResult( getItemDef(stuffItem), RESULT_ARM_WASH_STUFF_MUST ) then
				must = 1
			end
			GameArmProce:requestArmWash( stuffItem, armListType, armGridIndex, nBind, 0, 0, must );
		end
		return;
	elseif MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		if ArmWashFrame:IsShown() then
			ArmWashFrameComfirmBtnSaveBtn_OnClick();
		else
			ArmAdvWashFrameComfirmBtnSaveBtn_OnClick();
		end
	elseif MessageBoxFrame:GetClientString() == "保存洗炼" then
		if ArmWashFrame:IsShown() then
			ArmWashFrame:Hide();
			if MessageBoxFrame:GetClientUserData( 0 ) > 0 then
				ArmAdvWashFrame:Show();
			end
		else
			ArmAdvWashFrame:Hide();
		end
	elseif MessageBoxFrame:GetClientString() == "放弃洗炼保存" then
		if ArmWashFrame:IsShown() then
			local nListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
			local nGridIdx	= ArmWashFrameArmItemBtn:GetClientUserData( 1 );
			UnLockItem( nListType, nGridIdx );--解锁
			ArmWashFrameArmItemBtn:SetClientUserData( 0,0 )
			ArmWashFrameArmItemBtn:SetClientUserData( 1,0 )
			ArmWashFrameArmItemBtn:SetClientUserData( 2,0 )
			ArmWashFrameArmItemBtn:SetClientUserData( 3,0 )
			UpdateArmWashFrameItemInfo();
			ArmWashFrameUpdateNewAddAttr( );
		else
			local nListType = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
			local nGridIdx	= ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
			UnLockItem( nListType, nGridIdx );--解锁
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 0,0 )
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 1,0 )
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 2,0 )
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 3,0 )
			UpdateArmAdvWashFrameItemInfo();
			ArmAdvWashFrameUpdateNewAddAttr( );
		end
	elseif MessageBoxFrame:GetClientString() == "洗炼替换保存" then
		local btn = getglobal( "ArmWashFrameArmItemBtn" );
		if ArmAdvWashFrame:IsShown() then
			btn = getglobal( "ArmAdvWashFrameArmItemBtn" );
		end
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		local nList = btn:GetClientUserData( 0 );
		local nGrid = btn:GetClientUserData( 1 );
		local srcList = MessageBoxFrame:GetClientUserData( 0 )
		local srcGrid = MessageBoxFrame:GetClientUserData( 1 )
		local armItem = container:getItem( nList,nGrid );			
		local nItemId	= armItem:getItemId();
		if nItemId > 0 then
			UnLockItem( nList, nGrid, nItemId );--解锁
		end
		local item = container:getItem( srcList, srcGrid );
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		if ArmWashFrame:IsShown() then
			UpdateArmWashFrameItemInfo();
			ArmWashFrameUpdateNewAddAttr( );
		else
			UpdateArmAdvWashFrameItemInfo();
			ArmAdvWashFrameUpdateNewAddAttr( );
		end
	elseif MessageBoxFrame:GetClientString() == "洗炼未保存移动" then
		local btn = getglobal( "ArmWashFrameArmItemBtn" );
		if ArmAdvWashFrame:IsShown() then
			btn = getglobal( "ArmAdvWashFrameArmItemBtn" );
		end
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		local desList = MessageBoxFrame:GetClientUserData( 0 )
		local desGrid = MessageBoxFrame:GetClientUserData( 1 )
		MessageBoxFrame:SetClientUserData( 1, desGrid )
		UnLockItem( nListType, nGridIdx );--解锁
		btn:SetClientUserData( 0,0 )
		btn:SetClientUserData( 1,0 )
		btn:SetClientUserData( 2,0 )
		btn:SetClientUserData( 3,0 )
		if ArmAdvWashFrame:IsShown() then
			UpdateArmAdvWashFrameItemInfo();
			ArmAdvWashFrameUpdateNewAddAttr( );
		else
			UpdateArmWashFrameItemInfo();
			ArmWashFrameUpdateNewAddAttr( );
		end
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local item = container:getItem( nListType, nGridIdx )
		if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end
	elseif MessageBoxFrame:GetClientString() == "背包绑定提示" then
		local srcList = MessageBoxFrame:GetClientUserData( 0 );
		local srcGrid = MessageBoxFrame:GetClientUserData( 1 );
		local desList = MessageBoxFrame:GetClientUserData( 2 );
		local desGrid = MessageBoxFrame:GetClientUserData( 3 );
		ContainerUnbindAddToBind( srcList, srcGrid, desList, desGrid );
	elseif MessageBoxFrame:GetClientString() == "消耗活力值" then
		EnterDifficultyPworld();
	elseif MessageBoxFrame:GetClientString() == "商运押金" then
		local npcid = MessageBoxFrame:GetClientUserData( 0 );
		local questId = MessageBoxFrame:GetClientUserData( 1 );
		Quest:acceptQuest( npcid, questId );
	elseif MessageBoxFrame:GetClientString() == "开启符文槽" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local nNeedBull = MessageBoxFrame:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer()
		if getPlayerMoney( mainplayer ) < nNeedBull then
			ShowMidTips( "绑银不足，无法开启" );
			return;
		end
		if checkNeedWarnCost( nNeedBull ) then
			CostMoneyConfirmFrame:SetClientString("开启符文");
			CostMoneyConfirmFrame:SetClientUserData( 0, id );
			CostMoneyConfirmFrame:SetClientUserData( 1, nNeedBull );
			SetWarnFrame( "SkillFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
		RuneUnLock( id, nNeedBull );
	elseif MessageBoxFrame:GetClientString() == "退出竞赛界面" then
		IntelligenceRaceQuit();
	elseif MessageBoxFrame:GetClientString() == "银币出售确定" then
		MarketTradeSellSilverOrderSummit();
	elseif MessageBoxFrame:GetClientString() == "金币出售确定" then
		MarketTradeSellGoldOrderSummit();
	elseif MessageBoxFrame:GetClientString() == "银币转换绑银" then
		AgreeMoneyToBindMoney();
	elseif MessageBoxFrame:GetClientString() == "公会技能研发" then		
		local nId = MessageBoxFrame:GetClientUserData( 0 );
		local nLv = MessageBoxFrame:GetClientUserData( 1 );
		GameClanManager:Clan_Opt_SkillResearch( nId, nLv );
	elseif MessageBoxFrame:GetClientString() == "公会技能学习" then
		local nId = MessageBoxFrame:GetClientUserData( 0 );
		local nLv = MessageBoxFrame:GetClientUserData( 1 );
		GameClanManager:Clan_Opt_SkillLearn( nId, nLv );
	elseif MessageBoxFrame:GetClientString() == "副本传送" then
		local pworldId = MessageBoxFrame:GetClientUserData( 0 );
		AutoPworldManager:requestPworldPortal( pworldId );
	elseif MessageBoxFrame:GetClientString() == "公会商店贡献度消费" then
		local nCurIndex = MessageBoxFrame:GetClientUserData( 0 );
		local nNumber = MessageBoxFrame:GetClientUserData( 1 );
		NpcShop:buyItem( nCurIndex, nNumber );
	elseif MessageBoxFrame:GetClientString() == "公会仓库贡献度消费" then
		local num = MessageBoxFrame:GetClientUserData( 0 );
		result(ClanStashExchangeFrame, num);
		ClanStashExchangeFrame:Hide();
	elseif MessageBoxFrame:GetClientString() == "金券充能未绑宝石" then
		CallCharge();
	elseif MessageBoxFrame:GetClientString() == "充值礼包" then
		local idx = MessageBoxFrame:GetClientUserData( 0 );
		local propGiftInfo = GameWizard:getPropGiftInfo();
		GameWizard:requestPropsGift( propGiftInfo.PropsDef[idx-1].GroupID, propGiftInfo.PropsDef[idx-1].CashUp );
	elseif MessageBoxFrame:GetClientString() == "据点确认框" then
		local mapID = MessageBoxFrame:GetClientUserData(0);		
		GameClan_BaseWar:EnterStrongWar( mapID );
	elseif MessageBoxFrame:GetClientString() == "宠物转星损失" then
		PetStarMoveFrameStart();
	elseif MessageBoxFrame:GetClientString() == "宠物吞噬损失" then 
		PetAttrEatFrameStart();		
	elseif MessageBoxFrame:GetClientString() == "宝石重置未保存满属性" then
		local nList = MessageBoxFrame:GetClientUserData( 2 )
		local nGrid = MessageBoxFrame:GetClientUserData( 3 )
		GameArmProce:saveResetGem( nList, nGrid );
	elseif MessageBoxFrame:GetClientString() == "宝石重置已有满属性" then
		local nIndex = MessageBoxFrame:GetClientUserData( 0 )
		local nType = MessageBoxFrame:GetClientUserData( 1 )
		local nList = MessageBoxFrame:GetClientUserData( 2 )
		local nGrid = MessageBoxFrame:GetClientUserData( 3 )
		GameArmProce:resetGem( nIndex, nType, nList, nGrid );
	elseif MessageBoxFrame:GetClientString() == "辅助模式切换" then
		AutoAssistFunctionStart();
	elseif MessageBoxFrame:GetClientString() == "背饰进阶" then 
		BackItemLevelUp();
	elseif MessageBoxFrame:GetClientString() == "背饰改造" then 
		BackItemRemakeType();
	elseif MessageBoxFrame:GetClientString() == "抽卡面板关闭" then
		CardBox:requestCloseCardBox();
		SetCardBoxFrameShowMsg( MessageBoxHideBtn:GetCheckState() );
	elseif MessageBoxFrame:GetClientString() == "每日成长基金" then 
		local nShopID	= MessageBoxFrame:GetClientUserData(0);
		local itemData 	= MarketManager:getItemByID( nShopID );
		MarketManager:requestBuy(nShopID,1,0,"","",itemData.BuyType);
	elseif MessageBoxFrame:GetClientString() == "装备融合绑定提示" then
		ArmStepUpOpt();
	elseif MessageBoxFrame:GetClientString() == "放弃属性重置保存" then
		ArmBaseResetFrameOpt:Hide();
	elseif MessageBoxFrame:GetClientString() == "属性重置关闭提示" then
		ArmBaseResetFrame:Hide();
	elseif MessageBoxFrame:GetClientString() == "属性重置绑定提示" then
		ArmBaseResetOpt();
	elseif MessageBoxFrame:GetClientString() == "技能属性转移" then
		ArmSkillAttrMoveStart();
	elseif MessageBoxFrame:GetClientString() == "橙色装备制作提示" then
		ArmUpOrangeStart();
	elseif MessageBoxFrame:GetClientString() == "末世巅峰报名" then
		GameTopCareer:requestJoinTopCareer()
	elseif MessageBoxFrame:GetClientString() == "取消寄养" then
		ClanPetJiYangEnd()
	elseif MessageBoxFrame:GetClientString() == "巅峰冠军竞猜" then
		GameTopCareer:requestGuessTopCareer();
	elseif MessageBoxFrame:GetClientString() == "寄养宠物引诱" then
		ClanPetJiYangSeduce()
	elseif MessageBoxFrame:GetClientString() == "寄养宠物玩耍" then
		ClanPetJiYangWanShua()
	elseif MessageBoxFrame:GetClientString() == "造型更改确认" then
		local head, face, color = getFashion();
		LoginMgr:sendFashionChangeMessage(head, face, color);
	elseif MessageBoxFrame:GetClientString() == "造型更改取消" then
		CSFashionFrame:Hide();
		CSFashionResetButtonOnClick();
	elseif MessageBoxFrame:GetClientString() == "技能属性激活" then
		if MessageBoxFrame:GetClientUserData(2) == 1 then
			ShowMidTips( "激活失败，请收集齐材料后再进行激活" )
		elseif MessageBoxFrame:GetClientUserData(2) == 2 then
			ShowMidTips( "银币不足，技能属性激活失败" )
		else
			local srcList = MessageBoxFrame:GetClientUserData(0)
			local srcGrid = MessageBoxFrame:GetClientUserData(1)
			GameArmProce:requestArmSkillActive(srcGrid,srcList)
		end
	elseif MessageBoxFrame:GetClientString() == "技能属性锁定" then
		local srcList = MessageBoxFrame:GetClientUserData(0)
		local srcGrid = MessageBoxFrame:GetClientUserData(1)
		local idx = MessageBoxFrame:GetClientUserData(2)
		GameArmProce:requestArmSkillLock(idx,srcGrid,srcList)
	elseif MessageBoxFrame:GetClientString() == "摆放家具" then
		if GameHomelandManager:isFurnitureAlready() then
			GameHomelandManager:endFurnitureModeAlready()
		else
			GameHomelandManager:endFurnitureMode()
		end
	end

	if (MessageTitle:GetText() == "解散公会") then
		GameClanManager:clan_Opt_Disban();
	end

	if (MessageTitle:GetText() == "联盟邀请") then
		AnswerJoinUnion();
	end

	if (MessageTitle:GetText() == "删除邮件") then
		DeleteCheckMail();
	end

	if (MessageTitle:GetText() == "全部删除") then
		DeleteAllMail()
	end
	
	if (MessageTitle:GetText() == "充能确认") then
		CallCharge()
	end

	if (MessageTitle:GetText() == "发送邮件") then
		SendItemMail();
	end

	if (MessageTitle:GetText() == "发送快递包") then
		SendItemMail();
	end

	if (MessageTitle:GetText() == "绑定装备") then
		SendEquip( MessageBoxFrame:GetClientUserData( 0 ), MessageBoxFrame:GetClientUserData( 1 ),
						MessageBoxFrame:GetClientUserData( 2 ), MessageBoxFrame:GetClientUserData( 3 ));
	end

	if (MessageTitle:GetText() == "撤销订单" ) then
		TickListCancleOrderConfirm();
	end
	if (MessageTitle:GetText() == "伤害统计" ) then
		DamageCountFrameClearconfirm();
	end
	if (MessageTitle:GetText() == "退出游戏" ) then
		LoginMgr:startMud();
	end
	if ( MessageTitle:GetText() == "遗忘技能" ) then
		SecondarySkillConfirmDelSkill();
	end
	if ( MessageTitle:GetText() == "销毁物品" ) then
		local nIdx = MessageBoxFrame:GetClientUserData( 0 );
		local nType = MessageBoxFrame:GetClientUserData( 1 );
		GameClanManager:clan_Opt_MachineDestroy( nIdx,nType );
	end
	if ( MessageTitle:GetText() == "炼化加速" ) then
		local mainplayer = ActorMgr:getMainPlayer();
		local gameTrump	 = mainplayer:getTrump();
		local nIdx = MessageBoxFrame:GetClientUserData( 0 );
		gameTrump:requestDecEvolveTime( nIdx );
	end
	if ( MessageTitle:GetText() == "公会宣战" ) then
		local clanName = MessageBoxFrame:GetClientString();
		local high = MessageBoxFrame:GetClientUserData( 0 );
		local low = MessageBoxFrame:GetClientUserData( 1 );
		local clanId = MessageBoxFrame:GetClientUserData( 2 );
		GameClanManager:clan_Opt_ApplyJoinEnemy( high, low, clanId, clanName);
	end
	if ( MessageTitle:GetText() == "装备解绑" ) then
		local nListType = MessageBoxFrame:GetClientUserData( 0 );
		local nGrid = MessageBoxFrame:GetClientUserData( 1 );
		local nItemId = MessageBoxFrame:GetClientUserData( 2 );
		if nItemId == 0 then
			GameArmProce:unBindArm( nListType,nGrid,0,0,0 );
		else
			GameArmProce:unBindArm( nListType,nGrid,nItemId,ArmProceUnBindFrame:GetClientUserData(1),ArmProceUnBindFrame:GetClientUserData(2) );
		end
	end
	if MessageTitle:GetText() == "请离机械" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestMachineKickPlayer( id );
		end
	end
	if MessageTitle:GetText() == "邀请乘骑" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestApplyMachineInvite( id );
		end
	end
	if MessageTitle:GetText() == "重铸切换强化" or MessageTitle:GetText() == "洗炼切换强化"
		or MessageTitle:GetText() == "批量重铸切换强化"  then
		ArmRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameArmInfo:Hide();
		local btn = getglobal( MessageBoxFrame:GetClientString() )
		if btn:IsChecked() then
			return
		end
		local parent = getglobal( btn:GetParent() )
		local npcid = parent:GetClientUserData( 0 )
		parent:Hide()
		ArmProceStarFrame:Show()
		ArmProceStarFrame:SetClientUserData( 0,npcid )
		ArmProceStarFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
		ArmProceFrameDisCheckAllBtn( "ArmProceStarFrame" )
		ArmProceStarFrameStarBtn:Checked()
		--util.testNpcDialogDistance( npcid, ArmProceStarFrame );
	end
	if MessageTitle:GetText() == "强化切换重铸" or MessageTitle:GetText() == "洗炼切换重铸" then
		ArmProceStarFrameCancleBtn_onClick();
		local btn = getglobal( MessageBoxFrame:GetClientString() );
		if btn:IsChecked() then
			return
		end
		local parent = getglobal( btn:GetParent() )
		local npcid = parent:GetClientUserData( 0 )
		parent:Hide()
		ArmRecastFrame:Show()
		ArmRecastFrame:SetClientUserData( 0,npcid )
		ArmRecastFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
		ArmProceFrameDisCheckAllBtn( "ArmRecastFrame" )
		ArmRecastFrameRecastBtn:Checked()
		--util.testNpcDialogDistance( npcid, ArmRecastFrame );
	end
	if MessageTitle:GetText() == "重铸切换洗炼" or MessageTitle:GetText() == "强化切换洗炼" or MessageTitle:GetText() == "批量重铸切换洗炼" then
		ArmRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameArmInfo:Hide();
		local btn = getglobal( MessageBoxFrame:GetClientString() );
		if btn:IsChecked() then
			return
		end
		local parent = getglobal( btn:GetParent() )
		local npcid = parent:GetClientUserData( 0 )
		parent:Hide()
		ArmWashFrame:Show()
		ArmWashFrame:SetClientUserData( 0,npcid )
		ArmWashFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
		ArmProceFrameDisCheckAllBtn( "ArmWashFrame" )
		ArmWashFrameWashBtn:Checked()
		--util.testNpcDialogDistance( npcid, ArmWashFrame );
	end
	if MessageTitle:GetText() == "重铸切换绑定" or MessageTitle:GetText() == "强化切换绑定" or MessageTitle:GetText() == "洗炼切换绑定" 
		or MessageTitle:GetText() == "批量重铸切换绑定" then
		ArmRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameArmInfo:Hide();
		local btn = getglobal( MessageBoxFrame:GetClientString() )
		if btn:IsChecked() then
			return
		end
		local parent = getglobal( btn:GetParent() )
		local npcid = parent:GetClientUserData( 0 )
		parent:Hide()
		ArmProceBindFrame:Show()
		ArmProceBindFrame:SetClientUserData( 0,npcid )
		ArmProceBindFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
		ArmProceFrameDisCheckAllBtn( "ArmProceBindFrame" )
		ArmProceBindFrameBindBtn:Checked()
		--util.testNpcDialogDistance( npcid, ArmProceBindFrame );
	end
	if MessageTitle:GetText() == "重铸切换转星" or MessageTitle:GetText() == "强化切换转星" or MessageTitle:GetText() == "洗炼切换转星"
		or MessageTitle:GetText() == "批量重铸切换转星" then
		ArmRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameArmInfo:Hide();
		local btn = getglobal( MessageBoxFrame:GetClientString() );
		if btn:IsChecked() then
			return
		end
		local parent = getglobal( btn:GetParent() )
		local npcid = parent:GetClientUserData( 0 )
		parent:Hide()
		ArmProceStarMoveFrame:Show()
		ArmProceStarMoveFrame:SetClientUserData( 0,npcid )
		ArmProceStarMoveFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
		ArmProceFrameDisCheckAllBtn( "ArmProceStarMoveFrame" )
		ArmProceStarMoveFrameMoveBtn:Checked()	
		--util.testNpcDialogDistance( npcid, ArmProceStarMoveFrame );
	end
	if MessageTitle:GetText() == "单次重铸转批量重铸" then
		ArmAdvRecastFrameArmInfo:Hide();
		if ArmAdvRecastFrameBatchOKBtn_OnClick() then return end
	end
	if MessageTitle:GetText() == "公会旗帜保存" then
		local mapId = MessageBoxFrame:GetClientUserData( 0 );
		local npcID = MessageBoxFrame:GetClientUserData( 1 );
		local shapeIndex = MessageBoxFrame:GetClientUserData( 2 );
		local backIndex = MessageBoxFrame:GetClientUserData( 3 );
		local signIndex = MessageBoxFrame:GetClientUserData( 4 );
		GameClan_BaseWar:SetBanner( mapId, npcID, shapeIndex, backIndex, signIndex );
		BaseWarBannerFrame:Hide();
	end
	if MessageTitle:GetText() == "宝石合成绑定提示" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local list = MessageBoxFrame:GetClientUserData( 0 );
		local grid = MessageBoxFrame:GetClientUserData( 1 );
		local nType = MessageBoxFrame:GetClientUserData( 2 );
		local heChengFuItem = container:getItem( list, grid );
		GameArmProce:gemLevelUp( heChengFuItem,nType );
	end
	if MessageTitle:GetText() == "符文合成绑定提示" then
		HeChengBindOk();
	end
	if MessageTitle:GetText() == "宝石充能绑定" then
		CallCharge();
	end
	if MessageTitle:GetText() == "解除师徒关系" then
		local nHeight = MessageBoxFrame:GetClientUserData( 0 )
		local nLow = MessageBoxFrame:GetClientUserData( 1 )
		local szName = MessageBoxFrame:GetClientString()
		DelSchoolOk( nHeight, nLow, szName )
	end
	
	--登录界面
	if LoginFrame:IsShown() then
		SetFocusFrame("LoginNameEdit");
	end
	--创建角色界面
	if CSCreateFrame:IsShown() then
		SetFocusFrame("CSCreateName");
	end
		
	-- 临时加的退出界面
	if (MessageBoxFrame:GetClientString() == "离开游戏") then
		AgreeQuitGameButton_OnClick();
	end

	if (MessageBoxFrame:GetClientString() == "引力空间") then
		StartMudFrame:Show();
	end

	if (MessageTitle:GetText() == "撤销敌对" ) then
		GameClanManager:clan_Opt_Answer_Exit_Enemy( true );
	end
	if MessageTitle:GetText() == "幻化宠物" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		local nType = MessageBoxFrame:GetClientUserData( 0 );
		local ngrid = MessageBoxFrame:GetClientUserData( 1 );
		local targetIdx = MessageBoxFrame:GetClientUserData( 2 );
		container:send_UsedExpendableItem( nType, ngrid, targetIdx );
	end
	if MessageTitle:GetText() == "宠物资质重置" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		local nType = MessageBoxFrame:GetClientUserData( 0 );
		local ngrid = MessageBoxFrame:GetClientUserData( 1 );
		local targetIdx = MessageBoxFrame:GetClientUserData( 2 );
		container:send_UsedExpendableItem( nType, ngrid, targetIdx );
	end
	if MessageBoxFrame:GetClientString() == "解散队伍" then
		TeamManager:Team_OP_Destroy();
	end
	if MessageTitle:GetText() == "请离队伍" then
		local szName = MessageBoxFrame:GetClientString();
		TeamManager:Team_OP_Kick(szName);
	end

	if MessageBoxFrame:GetClientString() == "离开自动组队" then
		AutoTeamManager:cancelTranToPWorld();
	end
	if MessageTitle:GetText() == "邮件提示" then
		MailHeadFrame:Hide();
		MailSendItemFrame:Hide()
	end
	if MessageTitle:GetText() == "确认分配" then
		local money = MessageBoxFrame:GetClientUserData( 0 );
		local leftMoney = MessageBoxFrame:GetClientUserData( 1 );
		if leftMoney == 0 then
			if money ~= GetAssignCount( "" ) then
				ShowMidTips("公会成员信息有变化，请重新确认分配工资");
			else
				ClanBoon:requestClanBoonAssign();
			end
		end
	end
	if MessageBoxFrame:GetClientString() == "激活蓝色属性" then
		local num = MessageBoxFrame:GetClientUserData( 0 );
		local cost = MessageBoxFrame:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:getGodCoin() < cost then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
			return;
		end
		local armListType = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
		local armGridIndex = ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
		GameArmProce:requestArmWashExtent( armListType, armGridIndex, num );
	end
	if MessageTitle:GetText() == "图腾使用提示" then
		local MainPlayer = ActorMgr:getMainPlayer()
		local container = MainPlayer:getContainer()
		local listType = MessageBoxFrame:GetClientUserData(0)
		local gridIdx = MessageBoxFrame:GetClientUserData(1)
		container:usedItem( listType, gridIdx, 0, 0 );
	end
	if MessageBoxFrame:GetClientString() == "卖出全部货物" then
		local itemId = MessageBoxFrame:GetClientUserData( 0 );
		local price = MessageBoxFrame:GetClientUserData( 1 );
		local num = MessageBoxFrame:GetClientUserData( 2 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestPaoShangOpt( 2, itemId, price, num );
		end
	end
	MessageText:Clear();
	MessageText:clearHistory();
	MessageBoxFrame:SetClientString("");
	MessageBoxFrame:SetClientUserData( 0, 0 ); 
	MessageBoxFrame:SetClientUserData( 1, 0 );	
	MessageBoxFrame:SetClientUserData( 2, 0 ); 
	MessageBoxFrame:SetClientUserData( 3, 0 ); 
end

--消息提示窗口取消
function MessageBoxCancelBtnOnClick()
	MessageBoxFrame:Hide();	
	if ( MessageBoxFrame:GetClientString() == "服务器提示" ) then
		local nBindType = MessageBoxFrame:GetClientUserData( 0 );
		local nID = MessageBoxFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_NO_TOLUA );
	elseif( MessageBoxFrame:GetClientString() == "装备强化保存" ) then
		ArmProceStarFrameCancleBtn_onClick();
	elseif MessageBoxFrame:GetClientString() == "金券重置未绑定装备宝石" then
		GemProceResetFrameResetBtn:SetGray( false );
		GemProceResetFrameResetBtn:Enable();
	elseif MessageBoxFrame:GetClientString() == "沙滩椅使用" then
		BeachUnUseChairTmp()
	elseif MessageBoxFrame:GetClientString() == "宝石重置保存提示" then
		GemProceResetFrameCurResetInfoBtnCancleBtn_onClick();
		EnableGemProceResetFrame(true);
	end
	if (MessageTitle:GetText() == "撤销敌对" ) then
		GameClanManager:clan_Opt_Answer_Exit_Enemy( false );
	end
	if MessageTitle:GetText() == "邀请乘骑" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestRefuseMachineInvite( id );
		end
	end
	if MessageBoxFrame:GetClientString() == "确定保存洗炼" then
		if ArmWashFrame:IsShown() then
			ArmWashFrameComfirmBtnCancelBtn_OnClick();
		else
			ArmAdvWashFrameComfirmBtnCancelBtn_OnClick();
		end
	elseif MessageBoxFrame:GetClientString() == "公会仓库贡献度消费" then
		ClanStashExchangeFrame:Hide();
	end
	if (MessageTitle:GetText() == "撤销订单" ) then
		SetTradeListPageInfo( 3 );
		ShowGoldTradeListInfo();
	end	
	MessageText:SetText("",MsgR,MsgG,MsgB);
	--print( "MessageBoxCancelBtnOnClick--199" );
end

--消息提示窗口关闭
function MessageBoxCloseBtnOnClick()
	MessageBoxFrame:Hide();
	MessageText:SetText("",MsgR,MsgG,MsgB);
end

function MessageBoxOnEvent()
end

function MessageBoxOnLoad()
	MessageText:setCenterLine( true )
end

function MessageBoxForGameAcceptBtnOnClick()
	MessageBoxForGameFrame:Hide();
	if ( MessageBoxForGameFrame:GetClientString() == "服务器提示" ) then
		local nBindType = MessageBoxForGameFrame:GetClientUserData( 0 );
		local nID = MessageBoxForGameFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_YES_TOLUA );
		nAcceptTime = 0;
	end
	MessageForGameText:Clear();
	MessageForGameText:clearHistory();
	MessageBoxForGameFrame:SetClientString("");
	MessageBoxForGameFrame:SetClientUserData( 0, 0 ); 
	MessageBoxForGameFrame:SetClientUserData( 1, 0 );	
	MessageBoxForGameFrame:SetClientUserData( 2, 0 ); 
	MessageBoxForGameFrame:SetClientUserData( 3, 0 ); 
end
function MessageBoxForGameCancelBtnOnClick()
	MessageForGameText:resizeText(345,35);
	if ( MessageBoxForGameFrame:GetClientString() == "服务器提示" ) then
		local nBindType = MessageBoxForGameFrame:GetClientUserData( 0 );
		local nID = MessageBoxForGameFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_NO_TOLUA );
	end
	MessageForGameText:SetText("",MsgR,MsgG,MsgB);
	MessageBoxForGameFrame:Hide();	
end
function MessageBoxForGameFrame_OnUpdate()
	if MessageBoxForGameFrame:GetClientString() == "服务器提示" and BufferTimer ~= 0 then
		local timer = BufferTimer - math.ceil( os.clock() - BufferBegin );
		if timer == 0 then
			this:Hide();
			return;
		end
		MessageForGameText:SetText(BufferTips..SetBufferTimer( timer ),MsgR,MsgG,MsgB);
	end
	local npcid = this:GetClientUserData( 2 );
	if npcid > 0 then
		local npc = ActorMgr:FindActor(npcid)
		if npc == nil then
			this:Hide();
			return
		end
		if util.CalActorsDistance(npc, ActorMgr:getMainPlayer()) > (NpcDialog:getMaxNpcTalkDist( npc ) / 100) ^ 2 then
			this:Hide();
		end
	end
end

function MessageBoxForGameFrame_OnLoad()
	MessageForGameText:setCenterLine( true )
end

function PickTempItem( id )
	pickTmpItemId = id;
	if LoadingFrame:IsShown() then
		return;
	end
	MessageBox("提示", "是否使用临时物品？");		 
	MessageBoxFrame:SetClientString("使用临时物品");
end

-------------------------金钱输入框---------------------------

function showMoneyInput( max, min, fun, can )

end

--@Desc: 此为显示金钱数量输入框的时候响应
function InputMoneyFrame_OnShow()
	SetFocusFrame( "InputMoneyFrame_Edit" );
end

--@Desc: 此为点击金钱输入框上的确定按钮的时候响应
function InputMoneyFrame_AcceptBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end

	local szMoneyAmout	= InputMoneyFrame_Edit:GetText();
	local nMoneyAmout	= 0;
	if szMoneyAmout == "" then
		nMoneyAmout = 0;
	else
		nMoneyAmout = tonumber( szMoneyAmout );

		-- 若所输入金钱数超出目前玩家所携带的最大金钱数目,则将金钱数值为最大金钱数
		if nMoneyAmout > mainplayer:getMoney() then
			nMoneyAmout = mainplayer:getMoney();
		end
	end

	GameExchange:addExchangeMoney( nMoneyAmout );
	TradeSelfMoneyAmountEdit:SetText( nMoneyAmout );
	InputMoneyFrame:Hide();
	InputMoneyFrame_Edit:Clear();
end

--@Desc: 此为点击金钱输入框上的取消按钮的时候响应
function InputMoneyFrame_CancelBtn_OnClick()
	InputMoneyFrame:Hide();
	InputMoneyFrame_Edit:Clear();
end

function ShowServerMessageBoxForGame( nType,nID,szTips,timer )
	BufferTips = szTips;
	if timer ~= 0 then
		BufferTimer = timer;
		BufferBegin = os.clock();
		szTips = szTips..SetBufferTimer( timer )
	else
		BufferTimer = 0
		BufferBegin = 0
	end
	MessageBoxForGame("提示", szTips );
	MessageBoxForGameFrame:SetClientString("服务器提示");
	MessageBoxForGameFrame:SetClientUserData(0,nType);
	MessageBoxForGameFrame:SetClientUserData( 1,nID );
	if nType == BIND_TYPE_NPC_TOLUA then
		MessageBoxForGameFrame:SetClientUserData( 2,nID )
	else
		MessageBoxForGameFrame:SetClientUserData( 2,0 )
	end
end

function SetBufferTimer( timer )
	local nHour = math.floor( timer/3600 )
	local nMinute =  math.floor( ( timer - nHour*3600 ) / 60 )
	local nSecond = math.ceil( timer - nHour*3600 - nMinute*60 )
	if nHour == 0 and nMinute ==0 and nSecond ==0 then
		return "";
	end
	if nHour > 0 then
		return ",活动剩余时间："..nHour.."时"..nMinute.."分"..nSecond.."秒"
	end	
	if nMinute > 0 then
		return ",活动剩余时间："..nMinute.."分"..nSecond.."秒"
	end
	if nSecond > 0 then
		return ",活动剩余时间："..nSecond.."秒"
	end
end

function PKMessageBoxFrameAcceptBtnOnClick()
	if OnlineStoreFrame:IsShown() then
		return ShowMidTips( "查看店铺中，无法进行切磋" );
	end

	local roleName = PKMessageBoxFrame:GetClientString();
	GameActorPVP:requestSafePKRes( 0,roleName );
	nAcceptTime = 0;
	PKMessageBoxFrame:Hide();
end

function PKMessageBoxFrameCancelBtnOnClick()
	local roleName = PKMessageBoxFrame:GetClientString();
	GameActorPVP:requestSafePKRes( 1,roleName );
	nAcceptTime = 0;
	PKMessageBoxFrame:Hide();
end

function TeamKickVoteFrame_OnLoad()
	this:RegisterEvent( "GE_HIDE_VOTE_FRAME" );
	this:RegisterEvent( "GE_SHOW_VOTE_FRAME" );
	this:RegisterEvent( "GE_TEAM_LEAVETEAM" );
	this:RegisterEvent( "GE_TEAM_GET_TEAMINFO" );
end

function TeamKickVoteFrame_OnEvent()
	if arg1 == "GE_HIDE_VOTE_FRAME" or arg1 == "GE_TEAM_LEAVETEAM" then
		this:Hide();
		TeamVoteInfoFrame:Hide();
	elseif arg1 == "GE_SHOW_VOTE_FRAME" then
		if not this:IsShown() then
			this:Show();
		end		
	elseif arg1 == "GE_TEAM_GET_TEAMINFO" then
		local teamInfo = TeamManager:getTeamInfo();
		if teamInfo.KickVoteStartTime > 0 then
			local nAppectVote = 0;
			local nRefueseVote = 0;			
			for i=1,teamInfo.MemberNum do
				if teamInfo.TeamMember[i-1].VoteFlag == 1 then
					nAppectVote = nAppectVote + 1;
				elseif teamInfo.TeamMember[i-1].VoteFlag == 2 then
					nRefueseVote = nRefueseVote + 1;
				end				
			end
			local kickName = TeamManager:getVotePlayerName();
			if not CompareRoleName( kickName,ActorMgr:getMainPlayer():getName() ) then
				TeamVoteInfoFrame:Show();
				TeamVoteInfoFrameFont:SetText( "提示：请离" .. kickName .."投票：".. nRefueseVote .."人拒绝，" .. nAppectVote .. "人同意。" );
			end
		else
			TeamVoteInfoFrame:Hide();
		end
	end
end

function TeamKickVoteFrame_OnShow()
	TeamKickVoteFrameText:SetText( "#P是否同意将“" .. TeamManager:getVotePlayerName() .. "”请离队伍。",255,255,190 );	
end

function TeamKickVoteFrameAcceptBtnOnClick()
	TeamManager:requestVoteKick( true );
	util.closeParent(this);
end

function TeamKickVoteFrameCancelBtnOnClick()
	TeamManager:requestVoteKick( false );
	util.closeParent(this);
end

-------------------------------队友召唤--------------------------------
function TeamMemberSummon( overTime, mapID )
	TeamSummonTimer	=	GameMgr:getTickTime()/1000 + overTime;
	TeamSummonMap	=	mapID;

	if TeamMemberSummonFrame:IsShown() then
		TeamMemberSummonFrame:Hide();
	end

	local szText = "你的队长召唤你去他的身边";
	if TeamSummonMap >= MIN_COPY_ID then
		local pworldDef = getPworldDef( TeamSummonMap / 100 );
		if pworldDef.BattleID > 0 then
			local _,idx = string.find( pworldDef.Name,".（");
			if idx ~= nil then
				szText = szText .. string.format( "#cf57300（%s）", string.sub( pworldDef.Name, 1, idx-2 ) );
			end
		end
	end
	TeamMemberSummonFrameDescRich:SetText( szText, 255, 255, 0 );
	TeamMemberSummonFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	TeamMemberSummonFrame:Show();
end
function TeamMemberSummonFrameAcceptBtnOnClick()
	local mainPlayer = ActorMgr:getMainPlayer();
	if  mainPlayer == nil then
		return;
	end
	if mainPlayer:isInStall() then
		ShowMidTips("摆摊状态不可以切换地图！");
		return;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("战斗状态不可以切换地图！");
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "交易状态不可以切换地图" );
		return;
	end

	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("多人坐骑状态下无法传送！");
		return ;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("使用怪物机械时无法进行传送");
		return;
	end
	TeamManager:Team_OP_SummonRequest( true );
end
function TeamMemberSummonFrameCancelBtnOnClick()
	TeamManager:Team_OP_SummonRequest( false );
end

function TeamMemberSummonFrameClose()
	if TeamMemberSummonFrame:IsShown() then
		TeamMemberSummonFrame:Hide();
	end
end

function TeamMemberSummonFrame_OnLoad()
	this:setUpdateTime(1);
	TeamMemberSummonFrameDescRich:setCenterLine( true );
end

function TeamMemberSummonFrame_OnUpdate()
	local mainPlayer	= ActorMgr:getMainPlayer();
	local teamInfo		= TeamManager:getTeamInfo();
	local nCurtime 		= GameMgr:getTickTime()/1000;
	if  mainPlayer == nil then
		return;
	end
	--处于跨服状态关闭此面板，特殊处理跨服战场召唤
	if mainPlayer:isOutSide() then
		if TeamSummonMap < MIN_COPY_ID or getPworldDef( TeamSummonMap / 100 ).BattleID == 0 then
			TeamMemberSummonFrame:Hide();
		end
	end
	if nCurtime >= TeamSummonTimer 
		or mainPlayer:isCaptain() 
		or teamInfo.MemberNum == 0 
		or mainPlayer:isDead() then
		
		TeamMemberSummonFrame:Hide();
	end
end

function ShowTeamSummonUseMid( battleId )
	--根据需要添加其他战场提示
	if battleId == 2 then
		return ShowMidTips("此传送卷只可在绿洲战场使用");
	end
end

-------------------------------公会成员召唤--------------------------------
function ClanMemberSummon( overTime, position, name )
	ClanSummonTimer	=	GameMgr:getTickTime()/1000 + overTime;
	if ClanMemberSummonFrame:IsShown() then
		ClanMemberSummonFrame:Hide();
	end

	local szText = "#cf57300" .. name .. "#c0ad700" .. GetPosition( position ) .. "#n召唤你去他的身边";
	ClanMemberSummonFrameDescRich:SetText( szText, 255, 255, 0 );	
	ClanMemberSummonFrame:SetPoint( "center", "UIClient", "center", 0, 0 );
	ClanMemberSummonFrame:Show();
end

function ClanMemberSummonFrameAcceptBtnOnClick()
	local mainPlayer = ActorMgr:getMainPlayer();
	if  mainPlayer == nil then
		return;
	end
	if mainPlayer:isInStall() then
		ShowMidTips("摆摊状态不可以切换地图！");
		return;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("战斗状态不可以切换地图！");
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "交易状态不可以切换地图" );
		return;
	end

	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("多人坐骑状态下无法传送！");
		return ;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("使用怪物机械时无法进行传送");
		return;
	end
	GameClanManager:Clan_Opt_SummonRequest( true );
end

function ClanMemberSummonFrameCancelBtnOnClick()
	GameClanManager:Clan_Opt_SummonRequest( false );
end

function ClanMemberSummonFrameClose()
	if ClanMemberSummonFrame:IsShown() then
		ClanMemberSummonFrame:Hide();
	end
end

function ClanMemberSummonFrame_OnLoad()
	this:setUpdateTime(1);
	ClanMemberSummonFrameDescRich:setCenterLine( true );
end

function ClanMemberSummonFrame_OnUpdate()
	local mainPlayer	= ActorMgr:getMainPlayer();
	local nCurtime 		= GameMgr:getTickTime()/1000;
	if  mainPlayer == nil then
		return;
	end
	--处于跨服状态关闭此面板，特殊处理跨服战场召唤
	if mainPlayer:isOutSide() then
		ClanMemberSummonFrame:Hide();
	end
	if nCurtime >= ClanSummonTimer or mainPlayer:isDead() then
		ClanMemberSummonFrame:Hide();
	end
end

local	gridId	=	0;
local	subIndex	=	0;
function GetItemRecorderInfo( itemId ,gridIdx)
	if MapRecorderFrame:IsShown() or MapRecorderMessageFrame:IsShown() then
		MapRecorderFrame:Hide();
		MapRecorderMessageFrame:Hide();
	end	
	gridId	=	gridIdx;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local container	= MainPlayer:getContainer();
	local	item	= container:getItem( CONTAINER_TYPE_SUNDRIES,gridIdx );
	local itemDef	= getItemDef( itemId );
	local	mapNum	= item:getItemMapNum();
		subIndex	= mapNum;
	LockItem( CONTAINER_TYPE_SUNDRIES, gridIdx, itemId );
	local	MaxMapControl	=	MapShower:GetMaxMapContal();
	
	if mapNum	==	0 then
		MapRecorderMessageFrame:Show();
		local 	szText1	=	getglobal("MapRecorderMessageFrameDescFont1");
		local 	szText2	=	getglobal("MapRecorderMessageFrameDescFont2");
		szText1:SetText("空间记忆器尚未定位，请定位后使用");
		szText2:SetText( "当前剩余定位地点数量："..itemDef.Result[0].ResultVal1 );
	else         	
		MapRecorderFrame:Show();
		MapRecorderFrame:SetPoint( "center","UIClient","center",0,0 );
		local	text1	=	getglobal("MapRecorderFrameFont1");
		local	text2	=	getglobal("MapRecorderFrameFont2");
		local 	confirmBtn	=	getglobal("MapRecorderFrameConfirmButton");
		confirmBtn:Hide();
		for i=1 ,mapNum do
			local	btn	=	getglobal("MapRecorderFrameBtn"..i);
			local	btnFont	=	getglobal("MapRecorderFrameBtn"..i.."Text")
			local	szMapName	=	MaxMapControl:getMapName( item:getItemMapInfo(i-1).MapID );
			if string.find( szMapName , "天空岛") ~= nil then
				szMapName = "天空岛";
			elseif string.find( szMapName , "细语丘陵") ~= nil then
				szMapName = "细语丘陵";
			end
			local	mapX	=	math.floor(item:getItemMapInfo(i-1).X/100 + 1);
			local	mapY	=	math.floor(item:getItemMapInfo(i-1).Y/100 + 1);
			btn:Show();
			btn:DisChecked();
			btnFont:SetText(szMapName.." ( "..mapX..","..mapY.." )");
		end
		for i= mapNum + 1,MAX_SAVE_MAP_NUM do
			local	btn	=	getglobal("MapRecorderFrameBtn"..i);
			btn:Hide();
		end
		text1:SetText("当前剩余定位地点数量："..itemDef.Result[0].ResultVal1 - mapNum);
		text2:SetText("点击已定位地点可传送至该地点");
		confirmBtn:Hide();
	end
end
function MapRecorderMessageFrameLocationBtnOnClick()
	subIndex	=	0;
	local mainplayer= ActorMgr:getMainPlayer();
	local container	= mainplayer:getContainer();
	container:send_UsedExpendableItem( CONTAINER_TYPE_SUNDRIES, gridId, 0 ,subIndex ,1 );
	MapRecorderMessageFrame:Hide();
end
function MapRecorderMessageFrameCancelBtnOnClick()
	if MapRecorderMessageFrame:IsShown() then
		MapRecorderMessageFrame:Hide();
	end
	--判断交易中是否有该记忆器 Elmer
	local flag = true
	if TradeFrame:IsShown() then
		for i=1,MAX_TRAD_ITEM_NUM do
			local item = GameExchange:getExchangItem( 0, i-1 );
			if item:getGridIdx() == gridId then
				flag = false
			end
		end
	end
	if flag then
		UnlockMapRecorderItem();
	end
end

function MapRecorderFrameTrans_OnClick()
	for i = 1, MAX_SAVE_MAP_NUM do
		local btn = getglobal( "MapRecorderFrameBtn" .. i );
		btn:DisChecked();
	end
	this:Checked();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local 	rider		= mainplayer:getRider();
	local confirmBtn	= getglobal("MapRecorderFrameConfirmButton");
		subIndex		= this:GetClientID() - 1;
	if not confirmBtn:IsShown() then
		local	riderId		= rider:getRideItem().RideID;
		local	riderLv		= rider:getRideItem().nLevel;
		local	rideDef 	= getRideDef( riderId, riderLv );
		if mainplayer:isInRide() and  rideDef ~= nil and rideDef.RideOnNum >= 1 then
			ShowMidTips("多人坐骑状态下无法传送！");
			return ;
		end
		if mainplayer:isOnMonsterMachine() then
			ShowMidTips("使用怪物机械时无法进行传送");
			return;
		end
		container:send_UsedExpendableItem( CONTAINER_TYPE_SUNDRIES, gridId, 0 ,subIndex ,2 );
		MapRecorderFrame:Hide();
	end	
end
function MapRecorderFrameLocationBtnOnClick()
	
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local	item	=	container:getItem( CONTAINER_TYPE_SUNDRIES,gridId );
	local itemDef	= 	item:getItemDef();
	local confirmBtn=	getglobal("MapRecorderFrameConfirmButton");
	if	subIndex == itemDef.Result[0].ResultVal1 then
		--ShowMidTips("请选择需要覆盖的地点！");
		local	text1	=	getglobal("MapRecorderFrameFont1");
		local	text2	=	getglobal("MapRecorderFrameFont2");
		text1:SetText("定位数量已满，请选择需要覆盖的地点");
		text2:SetText("");
		confirmBtn:Show();
		return;
	end
	container:send_UsedExpendableItem( CONTAINER_TYPE_SUNDRIES, gridId, 0 ,subIndex ,1 );
	MapRecorderFrame:Hide();
end
function MapRecorderFrameConfirmBtnOnClick()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local	item		= container:getItem( CONTAINER_TYPE_SUNDRIES,gridId );
	local itemDef		= item:getItemDef();
	if subIndex == itemDef.Result[0].ResultVal1 then
		ShowMidTips("请选择需要覆盖的地点！");
		return;
	end
	container:send_UsedExpendableItem( CONTAINER_TYPE_SUNDRIES, gridId, 0 ,subIndex ,1 );
	MapRecorderFrame:Hide();
end
function MapRecorderFrameCancelBtnOnClick()
	if 	MapRecorderFrame:IsShown() then
		MapRecorderFrame:Hide();
	end
	--判断交易中是否有该记忆器 Elmer
	local flag = true
	if TradeFrame:IsShown() then
		for i=1,MAX_TRAD_ITEM_NUM do
			local item = GameExchange:getExchangItem( 0, i-1 );
			if item:getGridIdx() == gridId then
				flag = false ;
			end
		end
	end
	if flag then
		UnlockMapRecorderItem();
	end
end
function UnlockMapRecorderItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local	item	=	container:getItem( CONTAINER_TYPE_SUNDRIES,gridId );
	UnLockItem( CONTAINER_TYPE_SUNDRIES, gridId, item:getItemId());
end
function MapRecorderFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_CHG");
end
function MapRecorderFrame_OnEvent()
	if arg1 == "GE_CONTAINER_CHG" then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end
		local container = MainPlayer:getContainer();
		local	item	= container:getItem( CONTAINER_TYPE_SUNDRIES,gridId );
		if item:getItemId() == 0 then
			MapRecorderFrame:Hide();
			MapRecorderMessageFrame:Hide();
		end
	end
end
function MapRecorderFrame_OnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	if MainPlayer:isDead() then
		MapRecorderFrame:Hide();
		MapRecorderMessageFrame:Hide();
	end
end

function MessageBoxFrameForVip_OnHide()
	MessageBoxFrameForVip:SetClientUserData(0, 0);
	MessageBoxFrameForVip:SetClientUserData(1, 0);
	MessageBoxFrameForVip:SetClientUserData(2, 0);
end

function MessageBoxForVipAcceptBtnOnClick()
	if MessageBoxFrameForVipTitle:GetText() == "购买失败" then
		GameMgr:openWebSite( "http://pay.ljy0.com" );
	else
		if MessageBoxFrameForVipAcceptButton:GetText() == "提取金币" then
			MarketManager:requestQueryTianyuan();
			ExchangePointFrameNumEdit:SetText( 0 );
			SetFocusFrame("ExchangePointFrameNumEdit");
			ExchangePointFrame:SetClientUserData(0,0);
			ExchangePointFrame:Show();
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "快速充值" then
			GameMgr:openWebSite( "http://pay.ljy0.com" );
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "金币支付" then
			local monNum = MessageBoxFrameForVip:GetClientUserData(2);
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer ~= nil and mainplayer:getGodCoin() >= monNum then
				local BusyDefID = MessageBoxFrameForVip:GetClientUserData(0);
				local BusyExpItem = MessageBoxFrameForVip:GetClientUserData(1);
				MessageBoxFrameForVip:Hide();

				local busyDef	= BusyMgr:getBusyDef(BusyDefID);
				local container	= mainplayer:getContainer();
				if busyDef.AwardItem.ItemID > 0 and container:getUnusedGridNum( CONTAINER_TYPE_SUNDRIES ) == 0 then
					return ShowMidTips( "背包无法容纳更多物品，请整理背包" );
				end
				
				if CalculateLevelExpFull() then 
					return;
				end
				local lvFeng = GetFengYinData();
				if lvFeng> 0 and mainplayer:getLv() > lvFeng then
					return ShowMidTips( "当前等级大于封印等级，无法使用" );
				end

				BusyMgr:IsBusyTaskCommit( BusyDefID,BusyExpItem,0 );
				PromotionFrameBusyBtn_OnClick();
			else
				ExchangePointFrame:SetClientUserData(0,1);
				MarketManager:requestQueryTianyuan();
			end
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "购买银币" then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer ~= nil and mainplayer:isOutSide() then
				return ShowMidTips("跨服状态不允许该操作");
			end
			if not MarketTicketFrame:IsShown() then
				MarketTicketFrame:Show();
			end
			MarketTicketBuyBtn_OnClick();
		elseif MessageBoxFrameForVipAcceptButton:GetText() ==  "确定开启" then
			local MainPlayer = ActorMgr:getMainPlayer();
			if MainPlayer == nil then return end
			local container	= MainPlayer:getContainer();
			if MessageBoxFrameForVip:GetClientUserData(1) == 1 then
				local Eqiup = MainPlayer:getEquip();
				local nFashionNum = Eqiup:getSelectFashionNum();
				if MessageBoxFrameForVip:GetClientUserData(0) > nFashionNum + 1 then
					local itemId = 5251030;
					if container:getItemCount( itemId ) > 0 then
						container:usedItem( itemId );
					else
						SetMessageBoxForItem( itemId );
					end
				else
					ShowMidTips( "此栏位已经被开启" );
				end
			elseif MessageBoxFrameForVip:GetClientUserData(1) == 2 then
				local rider	= MainPlayer:getRider();
				local idx	= MessageBoxFrameForVip:GetClientUserData(0);
				local RideInfo	= rider:getRideGridInfo( idx - 1 );
				local btn	= getglobal( "RideFrameButton" .. idx );
				if RideInfo.RideID == 0 and btn:GetClientUserData(0) == 1 then
					local itemId = 5251402;
					if container:getItemCount( itemId ) > 0 then
						container:usedItem( itemId );
					else
						SetMessageBoxForItem( itemId );
					end
				else
					ShowMidTips( "此栏位已经被开启" );
				end
			elseif MessageBoxFrameForVip:GetClientUserData(1) == 3 then
				local idx = MessageBoxFrameForVip:GetClientUserData(0);
				local tex = getglobal( "StashFrameBagItem4BagBackTexture" );
				if not tex:IsShown() then
					return ShowMidTips( "当前没有可开启的栏位" );
				end
				local itemId = 5251023 + idx;
				if container:getItemCount( itemId ) > 0 then
					container:usedItem( itemId );
				else
					SetMessageBoxForItem( itemId );
				end
			end
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "购买扣带" then
			local shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, 5251001);
			if shopId > 0 then
				MarketBuyItemFrameBtn:SetClientUserData( 3, BUY_TYPE_TICKET );
				MarketManagerFrame_ShowBuyFrame(shopId);
				UpdateMarketBuyItemFrameMoneySymbol( BUY_TYPE_TICKET );
				FashionPreviewBuyFrame:Hide();
			end
		end
	end
	MessageBoxFrameForVip:Hide();
end

function MessageBoxForVipCancelBtnOnClick()
	MessageBoxFrameForVip:Hide();
end

function MessageBoxFrameForVip_OnLoad()
	MessageBoxFrameForVipText:setCenterLine( true );
end

function MessageBoxForVip(title, note, bCancel)
	MessageBoxFrameForVip:Show();
	MessageBoxFrameForVip:SetClientString("");
	MessageBoxFrameForVipText:clearHistory();
	MessageBoxFrameForVipTitle:SetText(title);
	MessageBoxFrameForVipText:SetText(note,MsgR,MsgG,MsgB);
	--设置按钮的显示状态和位置
	local okBtn = getglobal("MessageBoxFrameForVipAcceptButton");
	local cancelBtn = getglobal("MessageBoxFrameForVipCancelButton");
	if bCancel then
		okBtn:SetPoint("center","MessageBoxFrameForVip","center",0,45);
		cancelBtn:Hide();
	else
		okBtn:SetPoint("bottomleft","MessageBoxFrameForVip","bottomleft",15,-7);
		cancelBtn:Show();
	end
end

--道具购买提示框
function MessageBoxForItem(title, note)
	MarketBuyItemFrame:Hide();
	MessageBoxForItemFrame:Show();
	MessageBoxForItemFrame:SetClientString("");
	MessageBoxForItemText:clearHistory();
	MessageBoxForItemTitle:SetText(title);
	note = "#c0ad700" .. note .. "#n不足操作失败，是否需要购买";
	MessageBoxForItemText:SetText(note,MsgR,MsgG,MsgB);
	MessageBoxForItemFrame:SetClientUserData(0,0);
	MessageBoxForItemFrame:SetClientUserData(1,0);

	--[[设置按钮的显示状态和位置
	local okBtn = getglobal("MessageBoxForItemAcceptButton");
	local cancelBtn = getglobal("MessageBoxForItemCancelButton");
	okBtn:SetPoint("bottomleft","MessageBoxForItemFrame","bottomleft",5,-5);
	okBtn:Show();
	cancelBtn:Show();
	--]]
end

function MessageBoxForItemFrame_OnLoad()
	MessageBoxForItemText:setCenterLine( true );
end

function MessageBoxForItemAcceptBtnOnClick()
	MessageBoxForItemFrame:Hide();
	local itemId = MessageBoxForItemFrame:GetClientUserData(0);
	local shopId = 0;
	if itemId > 0 then
		shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, itemId);
	else
		if MessageBoxForItemFrame:GetClientString() == "重铸石不足" then
			local armId = MessageBoxForItemFrame:GetClientUserData(1);
			shopId = GetShopIDByArm( armId, "recast" );
		elseif MessageBoxForItemFrame:GetClientString() == "洗炼石不足" then
			local armId = MessageBoxForItemFrame:GetClientUserData(1);
			shopId = GetShopIDByArm( armId, "wash" );
		end
	end

	if shopId > 0 then
		MarketBuyItemFrameBtn:SetClientUserData( 3, BUY_TYPE_TICKET );
		MarketManagerFrame_ShowBuyFrame(shopId);
		UpdateMarketBuyItemFrameMoneySymbol( BUY_TYPE_TICKET );
		FashionPreviewBuyFrame:Hide();
	else
		if not MarketManagerFrame:IsShown() then
			MarketManagerFrame:Show();
		end
		MarketShowTargetPage( 1, 2, BUY_TYPE_TICKET );
	end
end

function MessageBoxForItemCancelBtnOnClick()
	MessageBoxForItemFrame:Hide();
end

function MessageBoxForItemFrame_OnHide()
end

function SetMessageBoxForItem( itemId )
	if itemId > 0 then
		MessageBoxForItem("提示",getItemDef(itemId).Name);
		MessageBoxForItemFrame:SetClientUserData(0,itemId);
	end
end