local szInfo = [[#cff9600����˵����
	#cffffbe���ܿ���ȡ�ͽ� 50��
	�����ѽ�ȡ���ͽ�Ҳ��Ϊ��ɴ���
	
#cff9600�ͽ���˵����
	#cffffbe�ͽ����ǿ�����������
	#c00ff00������������С����Ϊ50������
	#c00ff00�ռ���������С����Ϊ1����Ʒ
	#cffffbe�������ҿ�ˢ�����񣨰���������Ŀ�꣩
	���Ľ�ҿ�ˢ���Ǽ����Ǽ�Խ�߽���Խ�ߣ�
	
#cff9600����˵����
	#cffffbe�ͽ���ȼ�Խ�߽���Խ��
	�����Ϊ����ȼ����Ǽ�Խ�߽���Խ��
	#c00ff00ÿ��ǰ50�����5������ɻ�õ��߽���]]
local szShowOtherStar = [[#cff6400#L�鿴�����Ǽ�����#n
]]
local szGotoNpc = [[#ca0a05a���������ʱ����ǰ��#c00ff00���[ʱ���]#ca0a05a��ȡ����
]]
local SJKillNum = 0;
local SJID = 0;
local SJStar = 0;
local SJTaskID = 0;
local SJTASK_MAX_STAR = 5;
local MAX_SJTASK_NUM = 50;
local HasGetSJTaskItemNum = 0
function SetHasGetSJTaskItemNum( num )
	HasGetSJTaskItemNum = num
end
function BountyHunterTask_onShow()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local useItemListType = this:GetClientUserData( 0 );
	local useItemGridIdx = this:GetClientUserData( 1 );
	local useItemId = this:GetClientUserData( 2 );
	local usedItem = container:getItem( useItemListType, useItemGridIdx );
	if usedItem:getItemId() ~= nil and usedItem:getItemId() == useItemId then
	end
	BountyHunterTaskInfo:SetText( szInfo, 255, 255, 255 )
	BountyHunterTaskShowOtherStarInfo:SetText( szShowOtherStar, 255, 255, 255 )
	BountyHunterTaskGoToNpc:SetText( szGotoNpc, 255, 255, 255 )
end

function BountyHunterTask_onHide()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�ͽ��Ǽ�" then
		MessageBoxFrame:Hide();
	end
	ShowOtherStarReward:Hide();
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local oldItemListType = BountyHunterTask:GetClientUserData( 0 );
	local oldItemGridIdx = BountyHunterTask:GetClientUserData( 1 );
	local oldItemId = BountyHunterTask:GetClientUserData( 2 );
	local oldItem = container:getItem( oldItemListType, oldItemGridIdx );
	if oldItem:getItemId() ~= nil then
		UnLockItem( oldItemListType, oldItemGridIdx, oldItemId );
	end
end
function BountyHunterTaskShowOtherStarInfo_onClick()
	ShowOtherStarReward:Show();
end
function BountyHunterTaskGoToNpc_onClick()
	if arg3 == "LeftButton" then
		local strLinkItemHref	= arg2;
		local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( strLinkItemHref );

		if bLinkNpc == true then
			-- ��������Ŀ��NPC����ͬһ�ŵ�ͼ��
			MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
			return;
		end

		local nMapId, nMapPosX, nMapPosY = GetLinkPos( strLinkItemHref );
		if nMapId ~= 0 then
			MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
		end
	end
end
function BountyHunterTask_onUpdate()
	BountyHunterTaskTaskName:SetText( "���ƣ�"..Quest:getQuestName( SJTaskID )  );
	BountyHunterTaskTaskDes:SetText( "Ŀ�꣺"..Quest:getQuestReqDesc( SJTaskID ) );
	if Quest:getQuestType( SJTaskID ) ~= CLIENT_TASK_TYPE_SJ_BOSS then
		BountyHunterTaskTaskNum:SetText( "������"..SJKillNum );
	else
		BountyHunterTaskTaskNum:SetText( "" );
	end
	BountyHunterTaskTaskTraceTip:SetText( Quest:getQuestRunTip( SJTaskID ),255,255,255 );
	local num = Quest:getGroupQuestWeekNum( TASK_SJ_GROUPID );
	BountyHunterTaskTodayFinishNum:SetText( "��������ɴ�����" .. (num > MAX_SJTASK_NUM and MAX_SJTASK_NUM or num) .. "/".. MAX_SJTASK_NUM )
	BountyHunterTaskTodayVipFinishNum:SetText( "��������Ȩ��ɴ�����" .. ( num - MAX_SJTASK_NUM > 0 and num - MAX_SJTASK_NUM or 0 ) .. "/" .. VipManager:getShangJinNum() );
	BountyHunterTaskCanGetNum:SetText( "���ܿɻ�����������".. HasGetSJTaskItemNum .."/50" )
end
function ShowBountyHunter( num, sjID, star, taskID )
	if sjID == 0 or taskID == 0 then return end
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer()
	local oldItemListType = BountyHunterTask:GetClientUserData( 0 );
	local oldItemGridIdx = BountyHunterTask:GetClientUserData( 1 );
	local oldItemId = BountyHunterTask:GetClientUserData( 2 );
	local oldItem = container:getItem( oldItemListType, oldItemGridIdx );
	if oldItem:getItemId() ~= nil and oldItem:getItemId() == oldItemId then
		LockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() );
	end
	MAX_SJTASK_NUM = Quest:getQuestOneDayLimitNum( taskID, 0 )
	SetBountyHunterTaskInfo( num, sjID, star, taskID );
	BountyHunterTaskRefurbishStar:Enable();
	BountyHunterTaskUseItemRefurbishStar:Enable();
	BountyHunterTask:Show();
end
function SetBountyHunterTaskInfo( num, sjID, star, taskID )
	SJKillNum = num;
	SJID = sjID;
	SJStar = star;
	SJTaskID = taskID;
	--����star
	for i = 1, SJTASK_MAX_STAR do
		local curTaskStar = getglobal( "BountyHunterTaskCurTaskStarTex"..i );
		local tsakStar = getglobal( "BountyHunterTaskTaskStarTex"..i );
		local starTemplate = "YellowSmallXingXingTemplate"
		if ( SJStar + 1) < i then
			starTemplate = "GraySmallXingXingTemplate"
		end
		--����Ҫ�����ɫ��
		if ( SJStar + 1 ) == 5 then
			starTemplate = "PinkSmallXingXingTemplate"
		end
		curTaskStar:ChangeTextureTemplate( starTemplate );
		tsakStar:ChangeTextureTemplate( starTemplate );
	end
	if sjID <= 0 then return end
	BountyHunterTaskTaskName:SetText( "���ƣ�"..Quest:getQuestName( SJTaskID )  );
	BountyHunterTaskTaskDes:SetText( "Ŀ�꣺"..Quest:getQuestReqDesc( SJTaskID ) );
	if Quest:getQuestType( SJTaskID ) ~= CLIENT_TASK_TYPE_SJ_BOSS then
		BountyHunterTaskTaskNum:SetText( "������"..SJKillNum );
	else
		BountyHunterTaskTaskNum:SetText( "" );
	end
	BountyHunterTaskTaskTraceTip:SetText( Quest:getQuestRunTip( SJTaskID ),255,255,255 );
	local num = Quest:getGroupQuestWeekNum( TASK_SJ_GROUPID );
	BountyHunterTaskTodayFinishNum:SetText( "��������ɴ�����" .. (num > MAX_SJTASK_NUM and MAX_SJTASK_NUM or num) .. "/" ..MAX_SJTASK_NUM )
	BountyHunterTaskTodayVipFinishNum:SetText( "��������Ȩ��ɴ�����" .. ( num - MAX_SJTASK_NUM > 0 and num - MAX_SJTASK_NUM or 0 ) .. "/" .. VipManager:getShangJinNum() );
	BountyHunterTaskCanGetNum:SetText( "���ܿɻ�����������".. HasGetSJTaskItemNum .."/50" )
	local SJTaskDef = Quest:getSJTaskDef( SJID );	
	BountyHunterTaskTaskExp:SetText( "���飺"..SJTaskDef.StarRate[SJStar].ExpMul );
	if SJTaskDef.StarRate[SJStar].MoneyMul == 0 then
		BountyHunterTaskTaskMoney:SetText( "" );
	else		
		local tagTaskMoney	= Quest:getQuestMoney(SJTaskID, 0);		--��ȡ��������Ǯ
		BountyHunterTaskTaskMoney:SetText( GetMoneyDesc(tagTaskMoney.Type).."��"..SJTaskDef.StarRate[SJStar].MoneyMul );
	end
	--���������Ǽ���ʾ������Ϣ
	local tagTaskMoney	= Quest:getQuestMoney(SJTaskID, 0);		--��ȡ��������Ǯ
	ShowOtherStarRewardMoneyFont:SetText( GetMoneyDesc(tagTaskMoney.Type));
	for i=1,SJTASK_MAX_STAR do
		local btn = getglobal( "ShowOtherStarRewardStarInfo"..i );
		local StarNum = getglobal( btn:GetName().."StarNum" );
		StarNum:SetText( i.."�ǣ�" );
		for j = 1, SJTASK_MAX_STAR do
			local StarTex = getglobal( btn:GetName().."StarTex"..j );
			local starTemplate = "YellowSmallXingXingTemplate"
			if i < j then
				starTemplate = "GraySmallXingXingTemplate"
			end
			--����Ҫ�����ɫ��
			if i == 5 then
				starTemplate = "PinkSmallXingXingTemplate"
			end
			StarTex:ChangeTextureTemplate( starTemplate );
		end	
		local StarExp = getglobal( btn:GetName().."StarExp" );
		StarExp:SetText( SJTaskDef.StarRate[i-1].ExpMul )
		local StarMoney = getglobal( btn:GetName().."StarMoney" );
		StarMoney:SetText( SJTaskDef.StarRate[i-1].MoneyMul )
	end
	--����������		
	BountyHunterTaskAwardItemBtn:Hide();
	local awardNum = Quest:getQuestItemNum( SJTaskID, 0 )
	if Quest:getAwardType( SJTaskID, 0 ) == SJStar and HasGetSJTaskItemNum <= 50 then
		local nItemId = Quest:getQuestItem( SJTaskID, 0, 0 );     -- ��ȡ��������Ʒ
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			BountyHunterTaskAwardItemBtn:Show();
			local itemdef				= getItemDef( nItemId );
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			BountyHunterTaskAwardItemBtnIconTexture:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemdef, "BountyHunterTaskAwardItemBtnUVAnimationTex");
			local itemCount = Quest:getQuestItemCount( SJTaskID, 0, 0 );
			if itemCount > 1 then
				BountyHunterTaskAwardItemBtnCountFont:SetText( itemCount )
			else
				BountyHunterTaskAwardItemBtnCountFont:SetText( "" )	
			end
			BountyHunterTaskAwardItemBtn:SetClientUserData( 0, nItemId );
		end
	end
	--����button����Ϣ
	BountyHunterTaskRefurbishStar:SetText( GetMoneyDesc(SJTaskDef.RefreshStarMoney.Type).."ˢ���Ǽ�" );
	BountyHunterTaskRefurbishTask:SetText( GetMoneyDesc(SJTaskDef.RefreshTaskMoney.Type).."ˢ������" );
	--��ȡ������Ʒ����
	local nItemId = Quest:getQuestItem( SJTaskID, 0, 0 );
	local itemdef = getItemDef( nItemId );	
	local itemPinzhiColor = GetItemNameColor(itemdef)
	--ShowOtherStarRewardAwardItemRimind:SetText( "  5��������⽱�����ߣ�#L"..RGBColorToRichColor(itemPinzhiColor)..itemdef.Name.."#n", 255,255,190 )
	--ShowOtherStarRewardAwardItemRimind:SetClientUserData( 0, nItemId )
	ShowOtherStarRewardAwardItemRimind:SetText( "#G  ÿ��ǰ50�����5������ɻ�õ��߽���#n", 255,255,190 )
end

function BountyHunterTaskAwardItemBtn_OnEnter()
	local nItemID = this:GetClientUserData(0)
	local itemdef				= getItemDef( nItemID );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end

function HideBountyHunter()
	BountyHunterTask:Hide();
end

function BountyHunterTaskRefurbishStar_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�ͽ��Ǽ�" then
		return;
	end
	if SJStar == 4 then
		MessageBox( "�����Ǽ���ʾ", "#cffffbe��ǰ�Ǽ���Ϊ#c912df9����Ǽ�5��#cffffbe�����ȷ�������Ǽ�" );
		MessageBoxFrame:SetClientString( "�ͽ��Ǽ�" );
		return;
	end

	local SJTaskDef = Quest:getSJTaskDef( SJID );
	if SJTaskDef.RefreshStarMoney.Type == ATTR_ID_GODCOIN then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil or mainplayer:getGodCoin() < SJTaskDef.RefreshStarMoney.Val then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
			return;
		end
	end
	Quest:RefurbishBountyHunterStar( SJID, 0 );
	BountyHunterTaskRefurbishStar:Disable();
end

function BountyHunterTaskRefurbishStar_onEnter()
	if SJID <= 0 then return end
	local SJTaskDef = Quest:getSJTaskDef( SJID );	
	local szInfo = "#cffffbe����#c00ffff"..SJTaskDef.RefreshStarMoney.Val..GetMoneyDesc(SJTaskDef.RefreshStarMoney.Type).."#cffffbeˢ��\nÿ��ˢ����5�Ǽ��������ȡ\n#c00ff00�Ǽ�Խ�ߣ�����Խ��"
	SetGameTooltips( this:GetName().."OnTips", szInfo );
end

function BountyHunterTaskItemRefurbishStar_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�ͽ��Ǽ�" then
		return;
	end
	if SJStar == 4 then
		MessageBox( "�����Ǽ���ʾ", "#cffffbe��ǰ�Ǽ���Ϊ#c912df9����Ǽ�5��#cffffbe�����ȷ�������Ǽ�" );
		MessageBoxFrame:SetClientString( "�ͽ��Ǽ�" );
		return;
	end

	local SJTaskDef = Quest:getSJTaskDef( SJID );
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer()
	if container:getItemCount( SJ_ITEM_ID_TOLUA ) <= 0 then
		ShowMidTips( "���Ƿۡ����㣬ˢ���Ǽ�ʧ��" )
		return
	end
	Quest:RefurbishBountyHunterStar( SJID, 1 );
	BountyHunterTaskUseItemRefurbishStar:Disable();
end

function BountyHunterTaskItemRefurbishStar_onEnter()
end

function BountyHunterTaskRefurbishTask_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�ͽ��Ǽ�" then
		return;
	end
	local SJTaskDef = Quest:getSJTaskDef( SJID );
	if SJTaskDef.RefreshTaskMoney.Type == ATTR_ID_MONEY then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil or mainplayer:getMoney() < SJTaskDef.RefreshTaskMoney.Val then
			ShowBuyMoneyFrame();
			return;
		end
	end
	Quest:RefurbishBountyHunterTask( SJID );
end
function BountyHunterTaskRefurbishTask_onEnter()
	if SJID <= 0 then return end
	local SJTaskDef = Quest:getSJTaskDef( SJID );	
	local szInfo = "#cffffbe����#c00ffff"..SJTaskDef.RefreshTaskMoney.Val..GetMoneyDesc(SJTaskDef.RefreshTaskMoney.Type).."#cffffbeˢ��\nˢ�»�ı�����Ŀ�������"
	SetGameTooltips( this:GetName().."OnTips", szInfo );
end
function BountyHunterTaskAcceptTask_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�ͽ��Ǽ�" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then
		ShowMidTips( "��������" );
		return
	end
	
	local num = VipManager:isOwnVipFunc(STAR_RESULT_SHANGJIN_TOLUA) and MAX_SJTASK_NUM + VipManager:getShangJinNum() or MAX_SJTASK_NUM;
	if Quest:getGroupQuestWeekNum( TASK_SJ_GROUPID ) >= num then
		ShowMidTips( "���ܿ���ɴ�����������������ʧ��" );
		return;
	end
	Quest:AcceptBountyHunterTask( BountyHunterTask:GetClientUserData( 2 ), SJID, SJTaskID );
end

function ShowOtherStarReward_onShow()
	ShowOtherStarReward:SetPoint( "center", "UIClient", "center", 0, 0 );
end
function BountyHunterCurTaskStarTip_onEnter()
	SetGameTooltips( this:GetName(), "�Ǽ�Խ�ߣ�����Խ��\n��ͨ����ˢ���Ǽ�����ť���ı䵱ǰ������Ǽ�" );
end
function ShowOtherStarRewardAwardItemRimind_onClick()
	--local nItemID = ShowOtherStarRewardAwardItemRimind:GetClientUserData(0)
	--ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(nItemID) );
end
