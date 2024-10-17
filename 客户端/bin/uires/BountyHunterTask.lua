local szInfo = [[#cff9600次数说明：
	#cffffbe本周可领取赏金 50次
	放弃已接取的赏金也记为完成次数
	
#cff9600赏金令说明：
	#cffffbe赏金令是快速升级任务
	#c00ff00剿灭类任务最小数量为50个怪物
	#c00ff00收集类任务最小数量为1个物品
	#cffffbe消耗银币可刷新任务（包括数量、目标）
	消耗金币可刷新星级（星级越高奖励越高）
	
#cff9600奖励说明：
	#cffffbe赏金令等级越高奖励越好
	任务分为五个等级，星级越高奖励越好
	#c00ff00每周前50次完成5星任务可获得道具奖励]]
local szShowOtherStar = [[#cff6400#L查看其它星级奖励#n
]]
local szGotoNpc = [[#ca0a05a当条件达成时，请前往#c00ff00戈尔[时间城]#ca0a05a领取奖励
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
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "赏金星级" then
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
			-- 若主角与目标NPC不在同一张地图上
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
	BountyHunterTaskTaskName:SetText( "名称："..Quest:getQuestName( SJTaskID )  );
	BountyHunterTaskTaskDes:SetText( "目标："..Quest:getQuestReqDesc( SJTaskID ) );
	if Quest:getQuestType( SJTaskID ) ~= CLIENT_TASK_TYPE_SJ_BOSS then
		BountyHunterTaskTaskNum:SetText( "数量："..SJKillNum );
	else
		BountyHunterTaskTaskNum:SetText( "" );
	end
	BountyHunterTaskTaskTraceTip:SetText( Quest:getQuestRunTip( SJTaskID ),255,255,255 );
	local num = Quest:getGroupQuestWeekNum( TASK_SJ_GROUPID );
	BountyHunterTaskTodayFinishNum:SetText( "本周已完成次数：" .. (num > MAX_SJTASK_NUM and MAX_SJTASK_NUM or num) .. "/".. MAX_SJTASK_NUM )
	BountyHunterTaskTodayVipFinishNum:SetText( "翡翠徽章特权完成次数：" .. ( num - MAX_SJTASK_NUM > 0 and num - MAX_SJTASK_NUM or 0 ) .. "/" .. VipManager:getShangJinNum() );
	BountyHunterTaskCanGetNum:SetText( "本周可获得碎块数量：".. HasGetSJTaskItemNum .."/50" )
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
	--设置star
	for i = 1, SJTASK_MAX_STAR do
		local curTaskStar = getglobal( "BountyHunterTaskCurTaskStarTex"..i );
		local tsakStar = getglobal( "BountyHunterTaskTaskStarTex"..i );
		local starTemplate = "YellowSmallXingXingTemplate"
		if ( SJStar + 1) < i then
			starTemplate = "GraySmallXingXingTemplate"
		end
		--五星要变成紫色的
		if ( SJStar + 1 ) == 5 then
			starTemplate = "PinkSmallXingXingTemplate"
		end
		curTaskStar:ChangeTextureTemplate( starTemplate );
		tsakStar:ChangeTextureTemplate( starTemplate );
	end
	if sjID <= 0 then return end
	BountyHunterTaskTaskName:SetText( "名称："..Quest:getQuestName( SJTaskID )  );
	BountyHunterTaskTaskDes:SetText( "目标："..Quest:getQuestReqDesc( SJTaskID ) );
	if Quest:getQuestType( SJTaskID ) ~= CLIENT_TASK_TYPE_SJ_BOSS then
		BountyHunterTaskTaskNum:SetText( "数量："..SJKillNum );
	else
		BountyHunterTaskTaskNum:SetText( "" );
	end
	BountyHunterTaskTaskTraceTip:SetText( Quest:getQuestRunTip( SJTaskID ),255,255,255 );
	local num = Quest:getGroupQuestWeekNum( TASK_SJ_GROUPID );
	BountyHunterTaskTodayFinishNum:SetText( "本周已完成次数：" .. (num > MAX_SJTASK_NUM and MAX_SJTASK_NUM or num) .. "/" ..MAX_SJTASK_NUM )
	BountyHunterTaskTodayVipFinishNum:SetText( "翡翠徽章特权完成次数：" .. ( num - MAX_SJTASK_NUM > 0 and num - MAX_SJTASK_NUM or 0 ) .. "/" .. VipManager:getShangJinNum() );
	BountyHunterTaskCanGetNum:SetText( "本周可获得碎块数量：".. HasGetSJTaskItemNum .."/50" )
	local SJTaskDef = Quest:getSJTaskDef( SJID );	
	BountyHunterTaskTaskExp:SetText( "经验："..SJTaskDef.StarRate[SJStar].ExpMul );
	if SJTaskDef.StarRate[SJStar].MoneyMul == 0 then
		BountyHunterTaskTaskMoney:SetText( "" );
	else		
		local tagTaskMoney	= Quest:getQuestMoney(SJTaskID, 0);		--获取任务奖励金钱
		BountyHunterTaskTaskMoney:SetText( GetMoneyDesc(tagTaskMoney.Type).."："..SJTaskDef.StarRate[SJStar].MoneyMul );
	end
	--设置其它星级显示面板的信息
	local tagTaskMoney	= Quest:getQuestMoney(SJTaskID, 0);		--获取任务奖励金钱
	ShowOtherStarRewardMoneyFont:SetText( GetMoneyDesc(tagTaskMoney.Type));
	for i=1,SJTASK_MAX_STAR do
		local btn = getglobal( "ShowOtherStarRewardStarInfo"..i );
		local StarNum = getglobal( btn:GetName().."StarNum" );
		StarNum:SetText( i.."星：" );
		for j = 1, SJTASK_MAX_STAR do
			local StarTex = getglobal( btn:GetName().."StarTex"..j );
			local starTemplate = "YellowSmallXingXingTemplate"
			if i < j then
				starTemplate = "GraySmallXingXingTemplate"
			end
			--五星要变成紫色的
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
	--设置任务奖励		
	BountyHunterTaskAwardItemBtn:Hide();
	local awardNum = Quest:getQuestItemNum( SJTaskID, 0 )
	if Quest:getAwardType( SJTaskID, 0 ) == SJStar and HasGetSJTaskItemNum <= 50 then
		local nItemId = Quest:getQuestItem( SJTaskID, 0, 0 );     -- 获取任务奖励物品
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			BountyHunterTaskAwardItemBtn:Show();
			local itemdef				= getItemDef( nItemId );
			local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
			-- 如果没找到物品的图标则使用默认图标显示
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
	--设置button的信息
	BountyHunterTaskRefurbishStar:SetText( GetMoneyDesc(SJTaskDef.RefreshStarMoney.Type).."刷新星级" );
	BountyHunterTaskRefurbishTask:SetText( GetMoneyDesc(SJTaskDef.RefreshTaskMoney.Type).."刷新任务" );
	--获取五星物品奖励
	local nItemId = Quest:getQuestItem( SJTaskID, 0, 0 );
	local itemdef = getItemDef( nItemId );	
	local itemPinzhiColor = GetItemNameColor(itemdef)
	--ShowOtherStarRewardAwardItemRimind:SetText( "  5星任务额外奖励道具：#L"..RGBColorToRichColor(itemPinzhiColor)..itemdef.Name.."#n", 255,255,190 )
	--ShowOtherStarRewardAwardItemRimind:SetClientUserData( 0, nItemId )
	ShowOtherStarRewardAwardItemRimind:SetText( "#G  每周前50次完成5星任务可获得道具奖励#n", 255,255,190 )
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
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "赏金星级" then
		return;
	end
	if SJStar == 4 then
		MessageBox( "任务星级提示", "#cffffbe当前星级已为#c912df9最高星级5星#cffffbe，点击确定保留星级" );
		MessageBoxFrame:SetClientString( "赏金星级" );
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
	local szInfo = "#cffffbe花费#c00ffff"..SJTaskDef.RefreshStarMoney.Val..GetMoneyDesc(SJTaskDef.RefreshStarMoney.Type).."#cffffbe刷新\n每次刷新在5星级内随机抽取\n#c00ff00星级越高，奖励越高"
	SetGameTooltips( this:GetName().."OnTips", szInfo );
end

function BountyHunterTaskItemRefurbishStar_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "赏金星级" then
		return;
	end
	if SJStar == 4 then
		MessageBox( "任务星级提示", "#cffffbe当前星级已为#c912df9最高星级5星#cffffbe，点击确定保留星级" );
		MessageBoxFrame:SetClientString( "赏金星级" );
		return;
	end

	local SJTaskDef = Quest:getSJTaskDef( SJID );
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer()
	if container:getItemCount( SJ_ITEM_ID_TOLUA ) <= 0 then
		ShowMidTips( "【星粉】不足，刷新星级失败" )
		return
	end
	Quest:RefurbishBountyHunterStar( SJID, 1 );
	BountyHunterTaskUseItemRefurbishStar:Disable();
end

function BountyHunterTaskItemRefurbishStar_onEnter()
end

function BountyHunterTaskRefurbishTask_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "赏金星级" then
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
	local szInfo = "#cffffbe花费#c00ffff"..SJTaskDef.RefreshTaskMoney.Val..GetMoneyDesc(SJTaskDef.RefreshTaskMoney.Type).."#cffffbe刷新\n刷新会改变任务目标和数量"
	SetGameTooltips( this:GetName().."OnTips", szInfo );
end
function BountyHunterTaskAcceptTask_onClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "赏金星级" then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isDead() then
		ShowMidTips( "你已死亡" );
		return
	end
	
	local num = VipManager:isOwnVipFunc(STAR_RESULT_SHANGJIN_TOLUA) and MAX_SJTASK_NUM + VipManager:getShangJinNum() or MAX_SJTASK_NUM;
	if Quest:getGroupQuestWeekNum( TASK_SJ_GROUPID ) >= num then
		ShowMidTips( "本周可完成次数已满，接受任务失败" );
		return;
	end
	Quest:AcceptBountyHunterTask( BountyHunterTask:GetClientUserData( 2 ), SJID, SJTaskID );
end

function ShowOtherStarReward_onShow()
	ShowOtherStarReward:SetPoint( "center", "UIClient", "center", 0, 0 );
end
function BountyHunterCurTaskStarTip_onEnter()
	SetGameTooltips( this:GetName(), "星级越高，奖励越高\n可通过“刷新星级”按钮，改变当前任务的星级" );
end
function ShowOtherStarRewardAwardItemRimind_onClick()
	--local nItemID = ShowOtherStarRewardAwardItemRimind:GetClientUserData(0)
	--ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(nItemID) );
end
