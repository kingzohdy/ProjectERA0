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
	--�����һ������	
	ClearItemLock( "MessageBoxFrame" );
	MessageBoxFrame:Show();
	MessageBoxFrame:SetClientString("");
	MessageText:clearHistory();
	MessageTitle:SetText(title);
	MessageText:SetText(note,MsgR,MsgG,MsgB);
	--���ð�ť����ʾ״̬��λ��
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

	--���ð�ť����ʾ״̬��λ��
	local okBtn = getglobal("MessageBoxForGameAcceptButton");
	local cancelBtn = getglobal("MessageBoxForGameCancelButton");
	okBtn:SetPoint("bottomleft","MessageBoxForGameFrame","bottomleft",5,-5);
	okBtn:Show();
	cancelBtn:Show();
end

-- ��Գ������ɵ� messagebox
function MessageBoxForPet( title, note1,note2 )
	MessageBoxForPetFrame:Show();
	MessageBoxForPetFrame:SetClientString("");
	MessageBoxForPetText:clearHistory();
	MessageBoxForPetConfirmText:clearHistory();
	MessageBoxForPetTitle:SetText(title);
	MessageBoxForPetText:SetText(note1,255,255,190);
	MessageBoxForPetConfirmText:SetText(note2,255,255,190);
	--���ð�ť����ʾ״̬��λ��
	local okBtn = getglobal("MessageBoxForPetAcceptButton");
	local cancelBtn = getglobal("MessageBoxForPetCancelButton");
--	okBtn:SetPoint("bottomleft","MessageBoxForPetFrame","bottomleft",5,-5);
	okBtn:Show();
	cancelBtn:Show();
	cancelBtn:SetText("ȡ��");
	if title == "��������" then 
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
	local szText = "#Y���벻��#n��ϵ˵����\n1.�������л����������10%\n2.�����ҳ϶��������20%\n3.�����޷����ٴν���";
	SetGameTooltips(this:GetName(),szText);
end

function MessageBoxForPetAcceptBtnOnClick()
	MessageBoxForPetFrame:Hide();
	if MessageBoxForPetFrame:GetClientString() == "����������ʧ" then 
		PetAttrEatFrameStart();
	elseif MessageBoxForPetFrame:GetClientString() == "����ת����ʧ" then
		PetStarMoveFrameStart();
	elseif MessageBoxForPetFrame:GetClientString() == "�����ٻ�ȷ��" then
		RongYanSelectConfirmStart();
		return;
	elseif MessageBoxForPetFrame:GetClientString() == "���Һ��Ĳ���" then
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
	if this:GetText() == "���ٹ���" then 
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

--ֻ��һ��ȷ����ť��messagebox
function MessageBoxOK(title,note)
	MessageBoxFrame:Show();
	MessageText:clearHistory();
	MessageTitle:SetText(title);
	MessageText:SetText(note,MsgR,MsgG,MsgB);
	--���ð�ť����ʾ״̬��λ��
	local okBtn = getglobal("MessageBoxAcceptButton");
	local cancelBtn = getglobal("MessageBoxCancelButton");
	okBtn:SetPoint("center","MessageBoxFrame","center",0,45);
	okBtn:Show();
	cancelBtn:Hide();
	MessageBoxHideBtn:Hide();
end

--��Ϣ��ʾ����ȷ��
function MessageBoxAcceptBtnOnClick()
	MessageBoxFrame:Hide();
	--ȷ��ɾ����ɫ
	if(MessageBoxFrame:GetClientString() == "ȷ��ɾ����ɫ") then
		LoginMgr:requestDeleteSelectedRole();
	elseif(MessageBoxFrame:GetClientString() == "ȷ��������Ʒ") then
		TropItem();
		playUISound( "sounds\\UI\\diuqi.wav"  );
	elseif(MessageBoxFrame:GetClientString() == "ʹ����ʱ��Ʒ") then
		ActorMgr:pickTmpItem( pickTmpItemId );
	elseif (MessageBoxFrame:GetClientString() == "ȷ�Ϸ�������") then
		Quest:dropQuest( MessageBoxFrame:GetClientUserData( 0 ) );
		nCurSelectQuestId	= 0;
		nCurSelectRecycleId	= 0;
		HideTaskLogFrameTaskLogInfo();
	elseif (MessageBoxFrame:GetClientString() == "��������ʧ��") then
		Quest:dropQuest( MessageBoxFrame:GetClientUserData(0) );
	elseif (MessageBoxFrame:GetClientString() == "ȡ���ƺ�") then
		GameDesignation:sendSetDesignationReq( 0 );
	elseif (MessageBoxFrame:GetClientString() == "���óƺ�") then
		GameDesignation:sendSetDesignationReq( MessageBoxFrame:GetClientUserData(0) );
	elseif ( MessageBoxFrame:GetClientString() == "����ʱװ" ) then
		local nBuyID = MessageBoxFrame:GetClientUserData(1);
		local itemNum = MessageBoxFrame:GetClientUserData(2);
		local buyType = MessageBoxFrame:GetClientUserData(3);
		MarketManager:requestBuy(nBuyID,itemNum,0,"","",buyType);
	elseif ( MessageBoxFrame:GetClientString() == "���Ǹ�ħ" ) or ( MessageBoxFrame:GetClientString() == "�󶨸�ħ" ) then
		EnchantItem();
	elseif ( MessageBoxFrame:GetClientString() == "���ǿ�ӡ" ) or ( MessageBoxFrame:GetClientString() == "�󶨿�ӡ" ) then
		KeyinItem();
	elseif MessageBoxFrame:GetClientString() == "������Ƕ" then
		RuneInsert();
	elseif MessageBoxFrame:GetClientString() == "����ժȡ" then
		RuneErase();
 	elseif ( MessageBoxFrame:GetClientString() == "���ؽ���" ) then
		GameMgr:changeShowUI();
		bFirstChange = false;
	elseif ( MessageBoxFrame:GetClientString() == "��������ʾ" ) then
		local nBindType = MessageBoxFrame:GetClientUserData( 0 );
		local nID = MessageBoxFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_YES_TOLUA );
		nAcceptTime = 0;
	elseif ( MessageBoxFrame:GetClientString() == "��ȡ����" ) then
		GameWizard:sendGiftRequest( 0,MessageBoxFrame:GetClientUserData(0),CLIENT_GIFT_TYPE_ACCGIFT );
	elseif ( MessageBoxFrame:GetClientString() == "װ��ǿ��" ) then
		local nListType = MessageBoxFrame:GetClientUserData( 0 );
		local nGridIdx	= MessageBoxFrame:GetClientUserData( 1 );
		GameArmProce:armAddStar( nListType,nGridIdx );
	elseif( MessageBoxFrame:GetClientString() == "װ��ǿ������" ) then
		ArmProceStarFrameSaveBtn_onClick();
	elseif( MessageBoxFrame:GetClientString() == "����ǿ����ʾ" ) then
		ArmProceStarFrameSaveBtn_onClick();
	elseif( MessageBoxFrame:GetClientString() == "װ��ǿ������ʾ" ) then
		RequestArmStarAdd();
	elseif( MessageBoxFrame:GetClientString() == "Ʒ��ת�ư���ʾ" ) then
		CallArmProceStarMove();
	elseif( MessageBoxFrame:GetClientString() == "Ʒ��ת���Ǽ�������ʾ" ) then
		CallArmProceStarMove();
	elseif MessageBoxFrame:GetClientString() == "��������" then
		KeySettingFrameOnSave()
	elseif MessageBoxFrame:GetClientString() == "�ָ���λ����" then
		KeySettingFrameOnResetAll()
	elseif MessageBoxFrame:GetClientString() == "������е" then
		local player = ActorMgr:getMainPlayer();
		if player == nil then return end
		local eqiup = player:getEquip();
		eqiup:looseTmpArm();
		local btn1 = getglobal("TempMagicShortcut1");
		local btn2 = getglobal("TempMagicShortcut2");
		btn1:SetClientUserData( 0, 0 );
		btn2:SetClientUserData( 0, 0 );
	elseif MessageBoxFrame:GetClientString() == "���Ṻ��" then
		NpcShop:buyItem( MessageBoxFrame:GetClientUserData(0), 1 );
	elseif MessageBoxFrame:GetClientString() == "��������" then
		ArmRecastSelectAttrFrame:Hide();
	elseif MessageBoxFrame:GetClientString() == "�滻����װ��" then
		local btn = getglobal("ArmAdvRecastFrameArmItemBtn")
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );	
		UnLockItem( nListType, nGridIdx, 0 );--����
		local srcList = MessageBoxFrame:GetClientUserData( 0 )
		local srcGrid = MessageBoxFrame:GetClientUserData( 1 )
		LockItem( srcList, srcGrid );
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		ArmAdvRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameSaveSuccessBtn:Hide();
		ArmRecastSelectAttrFrame:Hide();
		UpdateAdvArmRecastFrame();
	elseif MessageBoxFrame:GetClientString() == "���µ�������װ��" then
		local nCurMoneyType = MessageBoxFrame:GetClientUserData( 0 )
		local armListType = MessageBoxFrame:GetClientUserData( 1 )
		local armGridIndex = MessageBoxFrame:GetClientUserData( 2 )
		ArmRecastSelectAttrFrame:Hide();
		GameArmProce:advanceRecastArm( nCurMoneyType,armListType,armGridIndex );	
	elseif MessageBoxFrame:GetClientString() == "������" then
		local stuffItemId = MessageBoxFrame:GetClientUserData( 0 )
		local nList = MessageBoxFrame:GetClientUserData( 1 )
		local nGrid = MessageBoxFrame:GetClientUserData( 2 )
		local nBind = MessageBoxFrame:GetClientUserData( 3 )
		local must = 0
		if CheckItemResult( getItemDef(stuffItemId), RESULT_ARM_RECAST_MUST ) then
			must = 1
		end
		GameArmProce:normalRecastArm( stuffItemId, nList, nGrid, nBind, must )
	elseif MessageBoxFrame:GetClientString() == "��ȯ����δ��װ��" then
		local nCurMoneyType = MessageBoxFrame:GetClientUserData( 0 )
		local armListType = MessageBoxFrame:GetClientUserData( 1 )
		local armGridIndex = MessageBoxFrame:GetClientUserData( 2 )
		if MessageBoxFrame:GetClientUserData( 3 ) > 0 then
			GameArmProce:batchRecastArm( nCurMoneyType,armListType,armGridIndex );
		else
			ArmRecastSelectAttrFrame:Hide();
			GameArmProce:advanceRecastArm( nCurMoneyType,armListType,armGridIndex );
		end
	elseif MessageBoxFrame:GetClientString() == "��ȯ����δ��װ����ʯ" then
		local nIndex = MessageBoxFrame:GetClientUserData( 0 )
		local nType = MessageBoxFrame:GetClientUserData( 1 )
		local nList = MessageBoxFrame:GetClientUserData( 2 )
		local nGrid = MessageBoxFrame:GetClientUserData( 3 )
		GameArmProce:resetGem( nIndex, nType, nList, nGrid );
	elseif MessageBoxFrame:GetClientString() == "�������ر�" or MessageBoxFrame:GetClientString() == "ǿ�����ر�"
			or MessageBoxFrame:GetClientString() == "�����������ر�" then
		ArmRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameArmInfo:Hide();
		ArmRecastFrame:Hide();
		ArmAdvRecastFrame:Hide();
		ArmProceStarFrame:Hide();		
	elseif MessageBoxFrame:GetClientString() == "�л��߼�����" then
		ArmRecastFrameArmInfo:Hide();
		ArmRecastFrameAdvRecastPanelBtn_OnClick();
	elseif MessageBoxFrame:GetClientString() == "��ʯ���ñ�����ʾ" then
		GemProceResetFrameCurResetInfoBtnSaveBtn_onClick();
		EnableGemProceResetFrame(true);
	elseif MessageBoxFrame:GetClientString() == "�л�����" then
		ArmAdvRecastFrameArmInfo:Hide();
		ArmRecastSelectAttrFrame:Hide();
		ArmAdvRecastFrameRecastPanelBtn_OnClick();
	elseif MessageBoxFrame:GetClientString() == "��ϴ��" then
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
	elseif MessageBoxFrame:GetClientString() ==  "����ϴ��" then
		local stuffItem = ArmWashFrameStuffItemBtn:GetClientUserData( 2 )
		local nBind = ArmWashFrameStuffItemBtn:GetClientUserData( 3 );
		local armListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
		local armGridIndex = ArmWashFrameArmItemBtn:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local arm = container:getItem( armListType,armGridIndex );
		if not arm:isItemBind() and nBind == 1 then
			MessageBox("����ʾ","����ϴ����ǰװ���ᱻ�󶨣��Ƿ����ϴ����");
			MessageBoxFrame:SetClientString("��ϴ��");
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
	elseif MessageBoxFrame:GetClientString() == "ȷ������ϴ��" then
		if ArmWashFrame:IsShown() then
			ArmWashFrameComfirmBtnSaveBtn_OnClick();
		else
			ArmAdvWashFrameComfirmBtnSaveBtn_OnClick();
		end
	elseif MessageBoxFrame:GetClientString() == "����ϴ��" then
		if ArmWashFrame:IsShown() then
			ArmWashFrame:Hide();
			if MessageBoxFrame:GetClientUserData( 0 ) > 0 then
				ArmAdvWashFrame:Show();
			end
		else
			ArmAdvWashFrame:Hide();
		end
	elseif MessageBoxFrame:GetClientString() == "����ϴ������" then
		if ArmWashFrame:IsShown() then
			local nListType = ArmWashFrameArmItemBtn:GetClientUserData( 0 );
			local nGridIdx	= ArmWashFrameArmItemBtn:GetClientUserData( 1 );
			UnLockItem( nListType, nGridIdx );--����
			ArmWashFrameArmItemBtn:SetClientUserData( 0,0 )
			ArmWashFrameArmItemBtn:SetClientUserData( 1,0 )
			ArmWashFrameArmItemBtn:SetClientUserData( 2,0 )
			ArmWashFrameArmItemBtn:SetClientUserData( 3,0 )
			UpdateArmWashFrameItemInfo();
			ArmWashFrameUpdateNewAddAttr( );
		else
			local nListType = ArmAdvWashFrameArmItemBtn:GetClientUserData( 0 );
			local nGridIdx	= ArmAdvWashFrameArmItemBtn:GetClientUserData( 1 );
			UnLockItem( nListType, nGridIdx );--����
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 0,0 )
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 1,0 )
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 2,0 )
			ArmAdvWashFrameArmItemBtn:SetClientUserData( 3,0 )
			UpdateArmAdvWashFrameItemInfo();
			ArmAdvWashFrameUpdateNewAddAttr( );
		end
	elseif MessageBoxFrame:GetClientString() == "ϴ���滻����" then
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
			UnLockItem( nList, nGrid, nItemId );--����
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
	elseif MessageBoxFrame:GetClientString() == "ϴ��δ�����ƶ�" then
		local btn = getglobal( "ArmWashFrameArmItemBtn" );
		if ArmAdvWashFrame:IsShown() then
			btn = getglobal( "ArmAdvWashFrameArmItemBtn" );
		end
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		local desList = MessageBoxFrame:GetClientUserData( 0 )
		local desGrid = MessageBoxFrame:GetClientUserData( 1 )
		MessageBoxFrame:SetClientUserData( 1, desGrid )
		UnLockItem( nListType, nGridIdx );--����
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
	elseif MessageBoxFrame:GetClientString() == "��������ʾ" then
		local srcList = MessageBoxFrame:GetClientUserData( 0 );
		local srcGrid = MessageBoxFrame:GetClientUserData( 1 );
		local desList = MessageBoxFrame:GetClientUserData( 2 );
		local desGrid = MessageBoxFrame:GetClientUserData( 3 );
		ContainerUnbindAddToBind( srcList, srcGrid, desList, desGrid );
	elseif MessageBoxFrame:GetClientString() == "���Ļ���ֵ" then
		EnterDifficultyPworld();
	elseif MessageBoxFrame:GetClientString() == "����Ѻ��" then
		local npcid = MessageBoxFrame:GetClientUserData( 0 );
		local questId = MessageBoxFrame:GetClientUserData( 1 );
		Quest:acceptQuest( npcid, questId );
	elseif MessageBoxFrame:GetClientString() == "�������Ĳ�" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local nNeedBull = MessageBoxFrame:GetClientUserData( 1 );
		local mainplayer = ActorMgr:getMainPlayer()
		if getPlayerMoney( mainplayer ) < nNeedBull then
			ShowMidTips( "�������㣬�޷�����" );
			return;
		end
		if checkNeedWarnCost( nNeedBull ) then
			CostMoneyConfirmFrame:SetClientString("��������");
			CostMoneyConfirmFrame:SetClientUserData( 0, id );
			CostMoneyConfirmFrame:SetClientUserData( 1, nNeedBull );
			SetWarnFrame( "SkillFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
		RuneUnLock( id, nNeedBull );
	elseif MessageBoxFrame:GetClientString() == "�˳���������" then
		IntelligenceRaceQuit();
	elseif MessageBoxFrame:GetClientString() == "���ҳ���ȷ��" then
		MarketTradeSellSilverOrderSummit();
	elseif MessageBoxFrame:GetClientString() == "��ҳ���ȷ��" then
		MarketTradeSellGoldOrderSummit();
	elseif MessageBoxFrame:GetClientString() == "����ת������" then
		AgreeMoneyToBindMoney();
	elseif MessageBoxFrame:GetClientString() == "���Ἴ���з�" then		
		local nId = MessageBoxFrame:GetClientUserData( 0 );
		local nLv = MessageBoxFrame:GetClientUserData( 1 );
		GameClanManager:Clan_Opt_SkillResearch( nId, nLv );
	elseif MessageBoxFrame:GetClientString() == "���Ἴ��ѧϰ" then
		local nId = MessageBoxFrame:GetClientUserData( 0 );
		local nLv = MessageBoxFrame:GetClientUserData( 1 );
		GameClanManager:Clan_Opt_SkillLearn( nId, nLv );
	elseif MessageBoxFrame:GetClientString() == "��������" then
		local pworldId = MessageBoxFrame:GetClientUserData( 0 );
		AutoPworldManager:requestPworldPortal( pworldId );
	elseif MessageBoxFrame:GetClientString() == "�����̵깱�׶�����" then
		local nCurIndex = MessageBoxFrame:GetClientUserData( 0 );
		local nNumber = MessageBoxFrame:GetClientUserData( 1 );
		NpcShop:buyItem( nCurIndex, nNumber );
	elseif MessageBoxFrame:GetClientString() == "����ֿ⹱�׶�����" then
		local num = MessageBoxFrame:GetClientUserData( 0 );
		result(ClanStashExchangeFrame, num);
		ClanStashExchangeFrame:Hide();
	elseif MessageBoxFrame:GetClientString() == "��ȯ����δ��ʯ" then
		CallCharge();
	elseif MessageBoxFrame:GetClientString() == "��ֵ���" then
		local idx = MessageBoxFrame:GetClientUserData( 0 );
		local propGiftInfo = GameWizard:getPropGiftInfo();
		GameWizard:requestPropsGift( propGiftInfo.PropsDef[idx-1].GroupID, propGiftInfo.PropsDef[idx-1].CashUp );
	elseif MessageBoxFrame:GetClientString() == "�ݵ�ȷ�Ͽ�" then
		local mapID = MessageBoxFrame:GetClientUserData(0);		
		GameClan_BaseWar:EnterStrongWar( mapID );
	elseif MessageBoxFrame:GetClientString() == "����ת����ʧ" then
		PetStarMoveFrameStart();
	elseif MessageBoxFrame:GetClientString() == "����������ʧ" then 
		PetAttrEatFrameStart();		
	elseif MessageBoxFrame:GetClientString() == "��ʯ����δ����������" then
		local nList = MessageBoxFrame:GetClientUserData( 2 )
		local nGrid = MessageBoxFrame:GetClientUserData( 3 )
		GameArmProce:saveResetGem( nList, nGrid );
	elseif MessageBoxFrame:GetClientString() == "��ʯ��������������" then
		local nIndex = MessageBoxFrame:GetClientUserData( 0 )
		local nType = MessageBoxFrame:GetClientUserData( 1 )
		local nList = MessageBoxFrame:GetClientUserData( 2 )
		local nGrid = MessageBoxFrame:GetClientUserData( 3 )
		GameArmProce:resetGem( nIndex, nType, nList, nGrid );
	elseif MessageBoxFrame:GetClientString() == "����ģʽ�л�" then
		AutoAssistFunctionStart();
	elseif MessageBoxFrame:GetClientString() == "���ν���" then 
		BackItemLevelUp();
	elseif MessageBoxFrame:GetClientString() == "���θ���" then 
		BackItemRemakeType();
	elseif MessageBoxFrame:GetClientString() == "�鿨���ر�" then
		CardBox:requestCloseCardBox();
		SetCardBoxFrameShowMsg( MessageBoxHideBtn:GetCheckState() );
	elseif MessageBoxFrame:GetClientString() == "ÿ�ճɳ�����" then 
		local nShopID	= MessageBoxFrame:GetClientUserData(0);
		local itemData 	= MarketManager:getItemByID( nShopID );
		MarketManager:requestBuy(nShopID,1,0,"","",itemData.BuyType);
	elseif MessageBoxFrame:GetClientString() == "װ���ںϰ���ʾ" then
		ArmStepUpOpt();
	elseif MessageBoxFrame:GetClientString() == "�����������ñ���" then
		ArmBaseResetFrameOpt:Hide();
	elseif MessageBoxFrame:GetClientString() == "�������ùر���ʾ" then
		ArmBaseResetFrame:Hide();
	elseif MessageBoxFrame:GetClientString() == "�������ð���ʾ" then
		ArmBaseResetOpt();
	elseif MessageBoxFrame:GetClientString() == "��������ת��" then
		ArmSkillAttrMoveStart();
	elseif MessageBoxFrame:GetClientString() == "��ɫװ��������ʾ" then
		ArmUpOrangeStart();
	elseif MessageBoxFrame:GetClientString() == "ĩ���۷屨��" then
		GameTopCareer:requestJoinTopCareer()
	elseif MessageBoxFrame:GetClientString() == "ȡ������" then
		ClanPetJiYangEnd()
	elseif MessageBoxFrame:GetClientString() == "�۷�ھ�����" then
		GameTopCareer:requestGuessTopCareer();
	elseif MessageBoxFrame:GetClientString() == "������������" then
		ClanPetJiYangSeduce()
	elseif MessageBoxFrame:GetClientString() == "����������ˣ" then
		ClanPetJiYangWanShua()
	elseif MessageBoxFrame:GetClientString() == "���͸���ȷ��" then
		local head, face, color = getFashion();
		LoginMgr:sendFashionChangeMessage(head, face, color);
	elseif MessageBoxFrame:GetClientString() == "���͸���ȡ��" then
		CSFashionFrame:Hide();
		CSFashionResetButtonOnClick();
	elseif MessageBoxFrame:GetClientString() == "�������Լ���" then
		if MessageBoxFrame:GetClientUserData(2) == 1 then
			ShowMidTips( "����ʧ�ܣ����ռ�����Ϻ��ٽ��м���" )
		elseif MessageBoxFrame:GetClientUserData(2) == 2 then
			ShowMidTips( "���Ҳ��㣬�������Լ���ʧ��" )
		else
			local srcList = MessageBoxFrame:GetClientUserData(0)
			local srcGrid = MessageBoxFrame:GetClientUserData(1)
			GameArmProce:requestArmSkillActive(srcGrid,srcList)
		end
	elseif MessageBoxFrame:GetClientString() == "������������" then
		local srcList = MessageBoxFrame:GetClientUserData(0)
		local srcGrid = MessageBoxFrame:GetClientUserData(1)
		local idx = MessageBoxFrame:GetClientUserData(2)
		GameArmProce:requestArmSkillLock(idx,srcGrid,srcList)
	elseif MessageBoxFrame:GetClientString() == "�ڷżҾ�" then
		if GameHomelandManager:isFurnitureAlready() then
			GameHomelandManager:endFurnitureModeAlready()
		else
			GameHomelandManager:endFurnitureMode()
		end
	end

	if (MessageTitle:GetText() == "��ɢ����") then
		GameClanManager:clan_Opt_Disban();
	end

	if (MessageTitle:GetText() == "��������") then
		AnswerJoinUnion();
	end

	if (MessageTitle:GetText() == "ɾ���ʼ�") then
		DeleteCheckMail();
	end

	if (MessageTitle:GetText() == "ȫ��ɾ��") then
		DeleteAllMail()
	end
	
	if (MessageTitle:GetText() == "����ȷ��") then
		CallCharge()
	end

	if (MessageTitle:GetText() == "�����ʼ�") then
		SendItemMail();
	end

	if (MessageTitle:GetText() == "���Ϳ�ݰ�") then
		SendItemMail();
	end

	if (MessageTitle:GetText() == "��װ��") then
		SendEquip( MessageBoxFrame:GetClientUserData( 0 ), MessageBoxFrame:GetClientUserData( 1 ),
						MessageBoxFrame:GetClientUserData( 2 ), MessageBoxFrame:GetClientUserData( 3 ));
	end

	if (MessageTitle:GetText() == "��������" ) then
		TickListCancleOrderConfirm();
	end
	if (MessageTitle:GetText() == "�˺�ͳ��" ) then
		DamageCountFrameClearconfirm();
	end
	if (MessageTitle:GetText() == "�˳���Ϸ" ) then
		LoginMgr:startMud();
	end
	if ( MessageTitle:GetText() == "��������" ) then
		SecondarySkillConfirmDelSkill();
	end
	if ( MessageTitle:GetText() == "������Ʒ" ) then
		local nIdx = MessageBoxFrame:GetClientUserData( 0 );
		local nType = MessageBoxFrame:GetClientUserData( 1 );
		GameClanManager:clan_Opt_MachineDestroy( nIdx,nType );
	end
	if ( MessageTitle:GetText() == "��������" ) then
		local mainplayer = ActorMgr:getMainPlayer();
		local gameTrump	 = mainplayer:getTrump();
		local nIdx = MessageBoxFrame:GetClientUserData( 0 );
		gameTrump:requestDecEvolveTime( nIdx );
	end
	if ( MessageTitle:GetText() == "������ս" ) then
		local clanName = MessageBoxFrame:GetClientString();
		local high = MessageBoxFrame:GetClientUserData( 0 );
		local low = MessageBoxFrame:GetClientUserData( 1 );
		local clanId = MessageBoxFrame:GetClientUserData( 2 );
		GameClanManager:clan_Opt_ApplyJoinEnemy( high, low, clanId, clanName);
	end
	if ( MessageTitle:GetText() == "װ�����" ) then
		local nListType = MessageBoxFrame:GetClientUserData( 0 );
		local nGrid = MessageBoxFrame:GetClientUserData( 1 );
		local nItemId = MessageBoxFrame:GetClientUserData( 2 );
		if nItemId == 0 then
			GameArmProce:unBindArm( nListType,nGrid,0,0,0 );
		else
			GameArmProce:unBindArm( nListType,nGrid,nItemId,ArmProceUnBindFrame:GetClientUserData(1),ArmProceUnBindFrame:GetClientUserData(2) );
		end
	end
	if MessageTitle:GetText() == "�����е" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestMachineKickPlayer( id );
		end
	end
	if MessageTitle:GetText() == "�������" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestApplyMachineInvite( id );
		end
	end
	if MessageTitle:GetText() == "�����л�ǿ��" or MessageTitle:GetText() == "ϴ���л�ǿ��"
		or MessageTitle:GetText() == "���������л�ǿ��"  then
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
	if MessageTitle:GetText() == "ǿ���л�����" or MessageTitle:GetText() == "ϴ���л�����" then
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
	if MessageTitle:GetText() == "�����л�ϴ��" or MessageTitle:GetText() == "ǿ���л�ϴ��" or MessageTitle:GetText() == "���������л�ϴ��" then
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
	if MessageTitle:GetText() == "�����л���" or MessageTitle:GetText() == "ǿ���л���" or MessageTitle:GetText() == "ϴ���л���" 
		or MessageTitle:GetText() == "���������л���" then
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
	if MessageTitle:GetText() == "�����л�ת��" or MessageTitle:GetText() == "ǿ���л�ת��" or MessageTitle:GetText() == "ϴ���л�ת��"
		or MessageTitle:GetText() == "���������л�ת��" then
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
	if MessageTitle:GetText() == "��������ת��������" then
		ArmAdvRecastFrameArmInfo:Hide();
		if ArmAdvRecastFrameBatchOKBtn_OnClick() then return end
	end
	if MessageTitle:GetText() == "�������ı���" then
		local mapId = MessageBoxFrame:GetClientUserData( 0 );
		local npcID = MessageBoxFrame:GetClientUserData( 1 );
		local shapeIndex = MessageBoxFrame:GetClientUserData( 2 );
		local backIndex = MessageBoxFrame:GetClientUserData( 3 );
		local signIndex = MessageBoxFrame:GetClientUserData( 4 );
		GameClan_BaseWar:SetBanner( mapId, npcID, shapeIndex, backIndex, signIndex );
		BaseWarBannerFrame:Hide();
	end
	if MessageTitle:GetText() == "��ʯ�ϳɰ���ʾ" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container = mainplayer:getContainer();
		local list = MessageBoxFrame:GetClientUserData( 0 );
		local grid = MessageBoxFrame:GetClientUserData( 1 );
		local nType = MessageBoxFrame:GetClientUserData( 2 );
		local heChengFuItem = container:getItem( list, grid );
		GameArmProce:gemLevelUp( heChengFuItem,nType );
	end
	if MessageTitle:GetText() == "���ĺϳɰ���ʾ" then
		HeChengBindOk();
	end
	if MessageTitle:GetText() == "��ʯ���ܰ�" then
		CallCharge();
	end
	if MessageTitle:GetText() == "���ʦͽ��ϵ" then
		local nHeight = MessageBoxFrame:GetClientUserData( 0 )
		local nLow = MessageBoxFrame:GetClientUserData( 1 )
		local szName = MessageBoxFrame:GetClientString()
		DelSchoolOk( nHeight, nLow, szName )
	end
	
	--��¼����
	if LoginFrame:IsShown() then
		SetFocusFrame("LoginNameEdit");
	end
	--������ɫ����
	if CSCreateFrame:IsShown() then
		SetFocusFrame("CSCreateName");
	end
		
	-- ��ʱ�ӵ��˳�����
	if (MessageBoxFrame:GetClientString() == "�뿪��Ϸ") then
		AgreeQuitGameButton_OnClick();
	end

	if (MessageBoxFrame:GetClientString() == "�����ռ�") then
		StartMudFrame:Show();
	end

	if (MessageTitle:GetText() == "�����ж�" ) then
		GameClanManager:clan_Opt_Answer_Exit_Enemy( true );
	end
	if MessageTitle:GetText() == "�û�����" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		local nType = MessageBoxFrame:GetClientUserData( 0 );
		local ngrid = MessageBoxFrame:GetClientUserData( 1 );
		local targetIdx = MessageBoxFrame:GetClientUserData( 2 );
		container:send_UsedExpendableItem( nType, ngrid, targetIdx );
	end
	if MessageTitle:GetText() == "������������" then
		local mainplayer = ActorMgr:getMainPlayer();
		local container	= mainplayer:getContainer();
		local nType = MessageBoxFrame:GetClientUserData( 0 );
		local ngrid = MessageBoxFrame:GetClientUserData( 1 );
		local targetIdx = MessageBoxFrame:GetClientUserData( 2 );
		container:send_UsedExpendableItem( nType, ngrid, targetIdx );
	end
	if MessageBoxFrame:GetClientString() == "��ɢ����" then
		TeamManager:Team_OP_Destroy();
	end
	if MessageTitle:GetText() == "�������" then
		local szName = MessageBoxFrame:GetClientString();
		TeamManager:Team_OP_Kick(szName);
	end

	if MessageBoxFrame:GetClientString() == "�뿪�Զ����" then
		AutoTeamManager:cancelTranToPWorld();
	end
	if MessageTitle:GetText() == "�ʼ���ʾ" then
		MailHeadFrame:Hide();
		MailSendItemFrame:Hide()
	end
	if MessageTitle:GetText() == "ȷ�Ϸ���" then
		local money = MessageBoxFrame:GetClientUserData( 0 );
		local leftMoney = MessageBoxFrame:GetClientUserData( 1 );
		if leftMoney == 0 then
			if money ~= GetAssignCount( "" ) then
				ShowMidTips("�����Ա��Ϣ�б仯��������ȷ�Ϸ��乤��");
			else
				ClanBoon:requestClanBoonAssign();
			end
		end
	end
	if MessageBoxFrame:GetClientString() == "������ɫ����" then
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
	if MessageTitle:GetText() == "ͼ��ʹ����ʾ" then
		local MainPlayer = ActorMgr:getMainPlayer()
		local container = MainPlayer:getContainer()
		local listType = MessageBoxFrame:GetClientUserData(0)
		local gridIdx = MessageBoxFrame:GetClientUserData(1)
		container:usedItem( listType, gridIdx, 0, 0 );
	end
	if MessageBoxFrame:GetClientString() == "����ȫ������" then
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

--��Ϣ��ʾ����ȡ��
function MessageBoxCancelBtnOnClick()
	MessageBoxFrame:Hide();	
	if ( MessageBoxFrame:GetClientString() == "��������ʾ" ) then
		local nBindType = MessageBoxFrame:GetClientUserData( 0 );
		local nID = MessageBoxFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_NO_TOLUA );
	elseif( MessageBoxFrame:GetClientString() == "װ��ǿ������" ) then
		ArmProceStarFrameCancleBtn_onClick();
	elseif MessageBoxFrame:GetClientString() == "��ȯ����δ��װ����ʯ" then
		GemProceResetFrameResetBtn:SetGray( false );
		GemProceResetFrameResetBtn:Enable();
	elseif MessageBoxFrame:GetClientString() == "ɳ̲��ʹ��" then
		BeachUnUseChairTmp()
	elseif MessageBoxFrame:GetClientString() == "��ʯ���ñ�����ʾ" then
		GemProceResetFrameCurResetInfoBtnCancleBtn_onClick();
		EnableGemProceResetFrame(true);
	end
	if (MessageTitle:GetText() == "�����ж�" ) then
		GameClanManager:clan_Opt_Answer_Exit_Enemy( false );
	end
	if MessageTitle:GetText() == "�������" then
		local id = MessageBoxFrame:GetClientUserData( 0 );
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer ~= nil then
			mainplayer:requestRefuseMachineInvite( id );
		end
	end
	if MessageBoxFrame:GetClientString() == "ȷ������ϴ��" then
		if ArmWashFrame:IsShown() then
			ArmWashFrameComfirmBtnCancelBtn_OnClick();
		else
			ArmAdvWashFrameComfirmBtnCancelBtn_OnClick();
		end
	elseif MessageBoxFrame:GetClientString() == "����ֿ⹱�׶�����" then
		ClanStashExchangeFrame:Hide();
	end
	if (MessageTitle:GetText() == "��������" ) then
		SetTradeListPageInfo( 3 );
		ShowGoldTradeListInfo();
	end	
	MessageText:SetText("",MsgR,MsgG,MsgB);
	--print( "MessageBoxCancelBtnOnClick--199" );
end

--��Ϣ��ʾ���ڹر�
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
	if ( MessageBoxForGameFrame:GetClientString() == "��������ʾ" ) then
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
	if ( MessageBoxForGameFrame:GetClientString() == "��������ʾ" ) then
		local nBindType = MessageBoxForGameFrame:GetClientUserData( 0 );
		local nID = MessageBoxForGameFrame:GetClientUserData( 1 );
		LoginMgr:replyPlayerMisc( nBindType,nID,CHOOSE_TYPE_NO_TOLUA );
	end
	MessageForGameText:SetText("",MsgR,MsgG,MsgB);
	MessageBoxForGameFrame:Hide();	
end
function MessageBoxForGameFrame_OnUpdate()
	if MessageBoxForGameFrame:GetClientString() == "��������ʾ" and BufferTimer ~= 0 then
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
	MessageBox("��ʾ", "�Ƿ�ʹ����ʱ��Ʒ��");		 
	MessageBoxFrame:SetClientString("ʹ����ʱ��Ʒ");
end

-------------------------��Ǯ�����---------------------------

function showMoneyInput( max, min, fun, can )

end

--@Desc: ��Ϊ��ʾ��Ǯ����������ʱ����Ӧ
function InputMoneyFrame_OnShow()
	SetFocusFrame( "InputMoneyFrame_Edit" );
end

--@Desc: ��Ϊ�����Ǯ������ϵ�ȷ����ť��ʱ����Ӧ
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

		-- ���������Ǯ������Ŀǰ�����Я��������Ǯ��Ŀ,�򽫽�Ǯ��ֵΪ����Ǯ��
		if nMoneyAmout > mainplayer:getMoney() then
			nMoneyAmout = mainplayer:getMoney();
		end
	end

	GameExchange:addExchangeMoney( nMoneyAmout );
	TradeSelfMoneyAmountEdit:SetText( nMoneyAmout );
	InputMoneyFrame:Hide();
	InputMoneyFrame_Edit:Clear();
end

--@Desc: ��Ϊ�����Ǯ������ϵ�ȡ����ť��ʱ����Ӧ
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
	MessageBoxForGame("��ʾ", szTips );
	MessageBoxForGameFrame:SetClientString("��������ʾ");
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
		return ",�ʣ��ʱ�䣺"..nHour.."ʱ"..nMinute.."��"..nSecond.."��"
	end	
	if nMinute > 0 then
		return ",�ʣ��ʱ�䣺"..nMinute.."��"..nSecond.."��"
	end
	if nSecond > 0 then
		return ",�ʣ��ʱ�䣺"..nSecond.."��"
	end
end

function PKMessageBoxFrameAcceptBtnOnClick()
	if OnlineStoreFrame:IsShown() then
		return ShowMidTips( "�鿴�����У��޷������д�" );
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
				TeamVoteInfoFrameFont:SetText( "��ʾ������" .. kickName .."ͶƱ��".. nRefueseVote .."�˾ܾ���" .. nAppectVote .. "��ͬ�⡣" );
			end
		else
			TeamVoteInfoFrame:Hide();
		end
	end
end

function TeamKickVoteFrame_OnShow()
	TeamKickVoteFrameText:SetText( "#P�Ƿ�ͬ�⽫��" .. TeamManager:getVotePlayerName() .. "��������顣",255,255,190 );	
end

function TeamKickVoteFrameAcceptBtnOnClick()
	TeamManager:requestVoteKick( true );
	util.closeParent(this);
end

function TeamKickVoteFrameCancelBtnOnClick()
	TeamManager:requestVoteKick( false );
	util.closeParent(this);
end

-------------------------------�����ٻ�--------------------------------
function TeamMemberSummon( overTime, mapID )
	TeamSummonTimer	=	GameMgr:getTickTime()/1000 + overTime;
	TeamSummonMap	=	mapID;

	if TeamMemberSummonFrame:IsShown() then
		TeamMemberSummonFrame:Hide();
	end

	local szText = "��Ķӳ��ٻ���ȥ�������";
	if TeamSummonMap >= MIN_COPY_ID then
		local pworldDef = getPworldDef( TeamSummonMap / 100 );
		if pworldDef.BattleID > 0 then
			local _,idx = string.find( pworldDef.Name,".��");
			if idx ~= nil then
				szText = szText .. string.format( "#cf57300��%s��", string.sub( pworldDef.Name, 1, idx-2 ) );
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
		ShowMidTips("��̯״̬�������л���ͼ��");
		return;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("ս��״̬�������л���ͼ��");
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "����״̬�������л���ͼ" );
		return;
	end

	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("��������״̬���޷����ͣ�");
		return ;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("ʹ�ù����еʱ�޷����д���");
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
	--���ڿ��״̬�رմ���壬���⴦����ս���ٻ�
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
	--������Ҫ�������ս����ʾ
	if battleId == 2 then
		return ShowMidTips("�˴��;�ֻ��������ս��ʹ��");
	end
end

-------------------------------�����Ա�ٻ�--------------------------------
function ClanMemberSummon( overTime, position, name )
	ClanSummonTimer	=	GameMgr:getTickTime()/1000 + overTime;
	if ClanMemberSummonFrame:IsShown() then
		ClanMemberSummonFrame:Hide();
	end

	local szText = "#cf57300" .. name .. "#c0ad700" .. GetPosition( position ) .. "#n�ٻ���ȥ�������";
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
		ShowMidTips("��̯״̬�������л���ͼ��");
		return;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("ս��״̬�������л���ͼ��");
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "����״̬�������л���ͼ" );
		return;
	end

	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("��������״̬���޷����ͣ�");
		return ;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("ʹ�ù����еʱ�޷����д���");
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
	--���ڿ��״̬�رմ���壬���⴦����ս���ٻ�
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
		szText1:SetText("�ռ��������δ��λ���붨λ��ʹ��");
		szText2:SetText( "��ǰʣ�ඨλ�ص�������"..itemDef.Result[0].ResultVal1 );
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
			if string.find( szMapName , "��յ�") ~= nil then
				szMapName = "��յ�";
			elseif string.find( szMapName , "ϸ������") ~= nil then
				szMapName = "ϸ������";
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
		text1:SetText("��ǰʣ�ඨλ�ص�������"..itemDef.Result[0].ResultVal1 - mapNum);
		text2:SetText("����Ѷ�λ�ص�ɴ������õص�");
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
	--�жϽ������Ƿ��иü����� Elmer
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
			ShowMidTips("��������״̬���޷����ͣ�");
			return ;
		end
		if mainplayer:isOnMonsterMachine() then
			ShowMidTips("ʹ�ù����еʱ�޷����д���");
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
		--ShowMidTips("��ѡ����Ҫ���ǵĵص㣡");
		local	text1	=	getglobal("MapRecorderFrameFont1");
		local	text2	=	getglobal("MapRecorderFrameFont2");
		text1:SetText("��λ������������ѡ����Ҫ���ǵĵص�");
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
		ShowMidTips("��ѡ����Ҫ���ǵĵص㣡");
		return;
	end
	container:send_UsedExpendableItem( CONTAINER_TYPE_SUNDRIES, gridId, 0 ,subIndex ,1 );
	MapRecorderFrame:Hide();
end
function MapRecorderFrameCancelBtnOnClick()
	if 	MapRecorderFrame:IsShown() then
		MapRecorderFrame:Hide();
	end
	--�жϽ������Ƿ��иü����� Elmer
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
	if MessageBoxFrameForVipTitle:GetText() == "����ʧ��" then
		GameMgr:openWebSite( "http://pay.ljy0.com" );
	else
		if MessageBoxFrameForVipAcceptButton:GetText() == "��ȡ���" then
			MarketManager:requestQueryTianyuan();
			ExchangePointFrameNumEdit:SetText( 0 );
			SetFocusFrame("ExchangePointFrameNumEdit");
			ExchangePointFrame:SetClientUserData(0,0);
			ExchangePointFrame:Show();
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "���ٳ�ֵ" then
			GameMgr:openWebSite( "http://pay.ljy0.com" );
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "���֧��" then
			local monNum = MessageBoxFrameForVip:GetClientUserData(2);
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer ~= nil and mainplayer:getGodCoin() >= monNum then
				local BusyDefID = MessageBoxFrameForVip:GetClientUserData(0);
				local BusyExpItem = MessageBoxFrameForVip:GetClientUserData(1);
				MessageBoxFrameForVip:Hide();

				local busyDef	= BusyMgr:getBusyDef(BusyDefID);
				local container	= mainplayer:getContainer();
				if busyDef.AwardItem.ItemID > 0 and container:getUnusedGridNum( CONTAINER_TYPE_SUNDRIES ) == 0 then
					return ShowMidTips( "�����޷����ɸ�����Ʒ����������" );
				end
				
				if CalculateLevelExpFull() then 
					return;
				end
				local lvFeng = GetFengYinData();
				if lvFeng> 0 and mainplayer:getLv() > lvFeng then
					return ShowMidTips( "��ǰ�ȼ����ڷ�ӡ�ȼ����޷�ʹ��" );
				end

				BusyMgr:IsBusyTaskCommit( BusyDefID,BusyExpItem,0 );
				PromotionFrameBusyBtn_OnClick();
			else
				ExchangePointFrame:SetClientUserData(0,1);
				MarketManager:requestQueryTianyuan();
			end
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "��������" then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer ~= nil and mainplayer:isOutSide() then
				return ShowMidTips("���״̬������ò���");
			end
			if not MarketTicketFrame:IsShown() then
				MarketTicketFrame:Show();
			end
			MarketTicketBuyBtn_OnClick();
		elseif MessageBoxFrameForVipAcceptButton:GetText() ==  "ȷ������" then
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
					ShowMidTips( "����λ�Ѿ�������" );
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
					ShowMidTips( "����λ�Ѿ�������" );
				end
			elseif MessageBoxFrameForVip:GetClientUserData(1) == 3 then
				local idx = MessageBoxFrameForVip:GetClientUserData(0);
				local tex = getglobal( "StashFrameBagItem4BagBackTexture" );
				if not tex:IsShown() then
					return ShowMidTips( "��ǰû�пɿ�������λ" );
				end
				local itemId = 5251023 + idx;
				if container:getItemCount( itemId ) > 0 then
					container:usedItem( itemId );
				else
					SetMessageBoxForItem( itemId );
				end
			end
		elseif MessageBoxFrameForVipAcceptButton:GetText() == "����۴�" then
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
	--���ð�ť����ʾ״̬��λ��
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

--���߹�����ʾ��
function MessageBoxForItem(title, note)
	MarketBuyItemFrame:Hide();
	MessageBoxForItemFrame:Show();
	MessageBoxForItemFrame:SetClientString("");
	MessageBoxForItemText:clearHistory();
	MessageBoxForItemTitle:SetText(title);
	note = "#c0ad700" .. note .. "#n�������ʧ�ܣ��Ƿ���Ҫ����";
	MessageBoxForItemText:SetText(note,MsgR,MsgG,MsgB);
	MessageBoxForItemFrame:SetClientUserData(0,0);
	MessageBoxForItemFrame:SetClientUserData(1,0);

	--[[���ð�ť����ʾ״̬��λ��
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
		if MessageBoxForItemFrame:GetClientString() == "����ʯ����" then
			local armId = MessageBoxForItemFrame:GetClientUserData(1);
			shopId = GetShopIDByArm( armId, "recast" );
		elseif MessageBoxForItemFrame:GetClientString() == "ϴ��ʯ����" then
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
		MessageBoxForItem("��ʾ",getItemDef(itemId).Name);
		MessageBoxForItemFrame:SetClientUserData(0,itemId);
	end
end