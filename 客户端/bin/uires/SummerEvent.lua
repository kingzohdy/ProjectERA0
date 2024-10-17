
------------------------------------------������� ����------------------------------------------
local MAX_RANKINFOBTN_NUM 	= 10;
local onRange 				= false;
local isTakeAward 			= 0;

function HangHaiFrame_OnLoad()
	this:RegisterEvent("GE_HANG_HAI_RANGE_INFO");	
end

function HangHaiFrame_OnEvent()
	if arg1 == "GE_HANG_HAI_RANGE_INFO" then
		local uiMsg = UIMSG:getUIMsg();
		local HangHaiRangeInfo 	= uiMsg.HangHaiRangData;
		local rangeInfo 	= HangHaiRangeInfo.HangHaiRange;
		if rangeInfo == nil then
			return;
		end
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then
			return;
		end
		local npcID = NpcDialog:getTalkNpcMemID();
		if HangHaiRangeInfo.OpType == 0 then
			local num = 0;
			onRange = false;
			isTaskAward = 1;

			if not HangHaiFrame:IsShown() then
				HangHaiFrame:Show();
			--	util.testNpcDialogDistance( npcID, HangHaiFrame );
			end

			for i=1, MAX_RANKINFOBTN_NUM do
				if i > rangeInfo.Num then
					break;
				end
				local btn = getglobal( "HangHaiFrame_ListBackBtnRankInfo" .. i );
				local sortFont = getglobal( btn:GetName() .. "SortIndexFont" );
				sortFont:SetText( rangeInfo.Members[i-1].Range );
				local nameFont = getglobal( btn:GetName() .. "NameFont" );
				nameFont:SetText( rangeInfo.Members[i-1].PlayerName );
				local lvFont = getglobal( btn:GetName() .. "LvFont" );
				lvFont:SetText( rangeInfo.Members[i-1].Level );
				local careerFont = getglobal( btn:GetName() .. "CareerFont" );
				careerFont:SetText( t_allCareerDesc[rangeInfo.Members[i-1].Career].desc );
				local timeFont = getglobal( btn:GetName() .. "TimeFont" );
				local szTime = os.date("%M", rangeInfo.Members[i-1].UseTime)..":"..os.date("%S", rangeInfo.Members[i-1].UseTime);
				timeFont:SetText( szTime );
				if mainplayer:isSameRoleWId( rangeInfo.Members[i-1].RoleID ) then
					num = i;
					onRange = true;
					isTakeAward = rangeInfo.Members[i-1].TakeAward;
					sortFont:SetTextColor( 10, 215, 0 );
					nameFont:SetTextColor( 10, 215, 0 );
					lvFont:SetTextColor( 10, 215, 0 );
					careerFont:SetTextColor( 10, 215, 0 );
					timeFont:SetTextColor( 10, 215, 0 );
				end
			end			

			if onRange then
				HangHaiFramePlayerRankFont:SetText( "���ڱ��񵥵�" .. num .. "λ" );
				if isTakeAward == 0 then
					HangHaiFrameGetAwardBtn:Enable();
					HangHaiFrameGetAwardBtnFont:SetTextColor( 255, 255, 190 );
				end
			end
		elseif HangHaiRangeInfo.OpType == 1 then
			if not HangHaiFrame:IsShown() then
				return;
			end
			for i=1, MAX_RANKINFOBTN_NUM do
				if i > rangeInfo.Num then
					break;
				end
				if mainplayer:isSameRoleWId( rangeInfo.Members[i-1].RoleID ) then
					onRange = true;
					isTakeAward = rangeInfo.Members[i-1].TakeAward;
					if isTakeAward == 1 then
						HangHaiFrameGetAwardBtn:Disable();
						HangHaiFrameGetAwardBtnFont:SetTextColor( 120, 120, 120 );
					end
					break;
				end
			end
		end
	end
end

function HangHaiFrame_OnShow()
	HangHaiFrame:SetPoint( "center", "UIClient", "center", 0, 0 );

	for i=1, MAX_RANKINFOBTN_NUM do
		local btn = getglobal( "HangHaiFrame_ListBackBtnRankInfo" .. i );
		local sortFont = getglobal( btn:GetName() .. "SortIndexFont" );
		sortFont:SetText("");
		sortFont:SetTextColor( 255, 255, 255 );
		local nameFont = getglobal( btn:GetName() .. "NameFont" );
		nameFont:SetText("");
		nameFont:SetTextColor( 255, 255, 255 );
		local lvFont = getglobal( btn:GetName() .. "LvFont" );
		lvFont:SetText("");
		lvFont:SetTextColor( 255, 255, 255 );
		local careerFont = getglobal( btn:GetName() .. "CareerFont" );
		careerFont:SetText("");
		careerFont:SetTextColor( 255, 255, 255 );
		local timeFont = getglobal( btn:GetName() .. "TimeFont" );
		timeFont:SetText("");
		timeFont:SetTextColor( 255, 255, 255 );
		if i%2 == 0 then
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
		end
	end

	HangHaiFramePlayerRankFont:SetText( "�㵱ǰ��δ�ϰ�" );
	HangHaiFrameGetAwardBtn:Disable();
	HangHaiFrameGetAwardBtnFont:SetTextColor( 120, 120, 120 );	
end

function HangHaiFrameGetAwardBtn_OnClick()
	if not onRange or isTakeAward ~= 0 then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then
		mainplayer:requestHangHaiRangeAward();
	end
end

----------------------------------------------������ ����------------------------------------------
local t_HaiBangFeedNum = { 1,5,10,30 };
local t_HaiBangFeedItemInfo = { 
		{ nLevel = 50, nItemID = 5060046 },
		{ nLevel = 60, nItemID = 5060046 },
		{ nLevel = 70, nItemID = 5060046 },
		{ nLevel = 80, nItemID = 5060046 }
	}
local HaiBangEndTime 	= 0;         -- �����ջ����ʱ��
local HaiBangLifeTime	= 0;		 -- ��������ʱ��
local HaiBangVal		= 0;
local MAX_HAIBANG_VAL	= 10;
local MAX_HAIBANG_TIME	= 600;
local nHaiBangItem 		= 0 ;
local bFeedFlag			= true;
function SetYangHaiBangUIInfo( nEndTime, nLevel, nVal,tLifeTime )
	HaiBangEndTime 	= nEndTime;
	HaiBangLifeTime = tLifeTime;
	HaiBangVal		= nVal;
	local curTime	= getServerTime().sec;
	local nTime;
	local liveTime	= tLifeTime - curTime > 0 and tLifeTime - curTime or 0 ;
	local szText 	= SetEndTimeText( liveTime );
	if nLevel == 1 then 
		MAX_HAIBANG_VAL 	= 10;
		MAX_HAIBANG_TIME	= 600;
	elseif nLevel == 2 then 
		MAX_HAIBANG_VAL 	= 20;
		MAX_HAIBANG_TIME	= 1200;
	elseif nLevel == 3 then 
		MAX_HAIBANG_VAL 	= 30;
		MAX_HAIBANG_TIME	= 1800;
	else 
		return;
	end
	
	if curTime >= nEndTime then 
		nTime = MAX_HAIBANG_TIME;
		HaiBangEndTime = 0;
	else 
		nTime = MAX_HAIBANG_TIME - ( nEndTime - curTime );
	end
	if nVal < MAX_HAIBANG_VAL then 
		bFeedFlag = true;
	else 
		bFeedFlag = false;
	end
	local liveFont	= getglobal("YangHaiBangFrameLiveFont");
	local timeFont 	= getglobal("YangHaiBangFrameTimeFont");
	local levelFont	= getglobal("YangHaiBangFrameLevelFont");
	local valFont	= getglobal("YangHaiBangFrameDegreeFont");
	timeFont:SetText("�ջ�ʱ�䣺"..nTime.."/"..MAX_HAIBANG_TIME.."��");
	levelFont:SetText("�ɳ��ȼ���"..nLevel );
	valFont:SetText("ι���̶ȣ�"..nVal.."/"..MAX_HAIBANG_VAL);
	liveFont:SetText( "����ʱ�䣺"..szText );
	if not YangHaiBangFrame:IsShown() then
		YangHaiBangFrame:Show();
	end
end

function YangHaiBangFrame_OnLoad()
	this:setUpdateTime(1);
	this:RegisterEvent("GE_PLAYER_LEAVEUP")
	for i=1,#t_HaiBangFeedNum do 
		local btn 	= getglobal( "YangHaiBangFrameFeedNumBtnSelectBtn"..i );
		local font 	= getglobal( "YangHaiBangFrameFeedNumBtnSelectBtn"..i.."Font" );
		if btn == nil then 
			break;
		end
		btn:SetClientUserData( 0, t_HaiBangFeedNum[i] );
	end
end

function YangHaiBangFrame_OnEvent()
	if arg1 == "GE_PLAYER_LEAVEUP" then
		if not YangHaiBangFrame:IsShown() then 
			return;
		end
		SetYangHaiBangItemInfo();
	end
end

function YangHaiBangFrame_OnUpdate()
	local curTime	= getServerTime().sec;
	local nTime;
	local liveTime	= HaiBangLifeTime - curTime > 0 and HaiBangLifeTime - curTime or 0 ;
	local szText 	= SetEndTimeText( liveTime );
	if liveTime == 0 then 
		YangHaiBangFrame:Hide();
		return;
	end
	
	local liveFont	= getglobal("YangHaiBangFrameLiveFont");
	liveFont:SetText("����ʱ�䣺"..szText );
	
	if HaiBangEndTime <= 0 then
		return;
	end
	
	if curTime >= HaiBangEndTime then 
		nTime = MAX_HAIBANG_TIME;
		HaiBangEndTime = 0;
	--	ShowMidTips("�������⺣̲��С�����ѵ��ջ�ʱ��ඣ��Ͻ�ȥ�����ɣ�");
	else 
		nTime = MAX_HAIBANG_TIME - ( HaiBangEndTime - curTime );
	end
	
	local timeFont 	= getglobal("YangHaiBangFrameTimeFont");
	timeFont:SetText("�ջ�ʱ�䣺"..nTime.."/"..MAX_HAIBANG_TIME.."��");
end

function UpdateHaiBangEndTime()
	--[[
	if YangHaiBangFrame:IsShown() then 
		return;
	end
	local curTime = getServerTime().sec;
	if HaiBangEndTime > 0 and curTime >= HaiBangEndTime then 
		ShowMidTips("�������⺣̲��С�����ѵ��ջ�ʱ��ඣ��Ͻ�ȥ�����ɣ�");
		HaiBangEndTime = 0;
	end
	--]]
end

function YangHaiBangFrame_OnShow()
	SetYangHaiBangItemInfo();
	local font = getglobal("YangHaiBangFrameChosenNumBtnFont");
	font:SetText( "1��" );
	local feedBtn = getglobal("YangHaiBangFrameFeedBtn");
	feedBtn:SetClientUserData( 1,1);
end

function SetYangHaiBangItemInfo()
	local feedBtn = getglobal("YangHaiBangFrameFeedBtn");
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end
	
	for j=1,#t_HaiBangFeedItemInfo do 
		local nLv = mainPlayer:getLv() 
		if nLv >= t_HaiBangFeedItemInfo[j]["nLevel"] and nLv <= t_HaiBangFeedItemInfo[j]["nLevel"] + 9 then 
			nHaiBangItem = t_HaiBangFeedItemInfo[j]["nItemID"];
		end
	end
	local itemDef = getItemDef( nHaiBangItem );
	local szName = "";
	if itemDef == nil then 
		szName = "��֦��¶";
		feedBtn:SetClientUserData( 0, 5060046 );
	else 
		szName = itemDef.Name;
		feedBtn:SetClientUserData( 0, nHaiBangItem );
	end
	for i=1,#t_HaiBangFeedNum do 
		local font 	= getglobal( "YangHaiBangFrameFeedNumBtnSelectBtn"..i.."Font" );
		if font == nil then 
			break;
		end
		font:SetText( szName.."��"..t_HaiBangFeedNum[i] );
	end
end

function YangHaiBangFrameFeedBtn_OnClick()
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end
	
	local container	= mainPlayer:getContainer();
	if container == nil then return; end
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("�����������޷�ι����������֤������ȫ��");
		return;
	end
	if not bFeedFlag then 
		local curTime = getServerTime().sec;
		if curTime >= HaiBangEndTime then
			ShowMidTips("�Ͻ�ȡ��ɣ��õ�����֮��ǵ�ȥ���׶�˹��ȡ����Ӵ��");
		else 
			ShowMidTips("����ι���̶������������ջ�ʱ�䵽��֮��ȡ��");
		end
		return;
	end
	local nItemId 	= this:GetClientUserData(0);
	local nItemNum	= this:GetClientUserData(1);
	if container:getItemCount( nItemId ) < nItemNum then
		local itemDef = getItemDef( nItemId ) ;
		ShowMidTips("������"..itemDef.Name.."����������");	
	else 
		mainPlayer:requestYangHaiBangAdd( nItemNum );
	end
end

function YangHaiBangFrameTakeBtn_OnClick()
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end
	
	local curTime	= getServerTime().sec;
	local nTime;
	if curTime < HaiBangEndTime then 
		ShowMidTips("�������ջ�ʱ��δ���������ĵȺ�");
		return;
	end
	if HaiBangVal < MAX_HAIBANG_VAL then 
		ShowMidTips("������δ���죬����ι��");
		return;
	end
	mainPlayer:requestYangHaiBangAward();
	YangHaiBangFrame:Hide();
end

function YangHaiBangFrameScrollDownBtn_OnClick()
	if YangHaiBangFrameFeedNumBtn:IsShown() then
		YangHaiBangFrameFeedNumBtn:Hide();
	else
		YangHaiBangFrameFeedNumBtn:Show();
	end
end

function HaiBangFrameFeedNumSelectBtn_OnClick()
	local nNum = this:GetClientUserData(0);
	local font = getglobal("YangHaiBangFrameChosenNumBtnFont");
	font:SetText( nNum.."��" );
	YangHaiBangFrameFeedNumBtn:Hide();
	local feedBtn = getglobal("YangHaiBangFrameFeedBtn");
	feedBtn:SetClientUserData( 1,nNum );
end

---------------------------------------------���Ҿ��� ����---------------------------------------------------

local MAX_RONGYAN_BTN_NUM = 4;
local selectIndex 	= 0;
local selectMulty 	= 0;
local selectNum		= 0;
local RongYanItemUse = true;
local bMapType		= false;
local t_RongYanAllFrame = {"RongYanSelectFrame","RongYanStartFrame","RongYanGuessFrame","RongYanMiniFrame"}
function RongYanSelectFrame_OnLoad()
	for i=1, MAX_RONGYAN_BTN_NUM do
		local btn = getglobal("RongYanSelectFrameMonInfo"..i)
		if i%2 == 0 then
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
		end
	end
	local rich = getglobal("RongYanSelectFrameTipRich");
	rich:setCenterLine( true );
	local szText = "���ٻ������Ҿ��޺�\n���齫������,��Ա�����˳��������������¶�Ա"
	rich:SetText( szText,255,255,190 );
	this:RegisterEvent("GE_TEAM_LEAVETEAM");
	this:RegisterEvent("GE_UPDATE_ROLELIST");
	this:RegisterEvent("GE_KICKOFF_INFO");
end

function RongYanSelectFrame_OnEvent()
	if arg1 == "GE_TEAM_LEAVETEAM" then
		for i=1, #t_RongYanAllFrame do 
			local frame = getglobal(t_RongYanAllFrame[i]);
			if frame:IsShown() then 
				frame:Hide();
			end
		end
		RongYanItemUse = true;
	elseif arg1 == "GE_UPDATE_ROLELIST" or arg1 == "GE_KICKOFF_INFO" then 
		RongYanItemUse = true;
	end
end

function RongYanSelectFrame_OnShow()
	local RongYanDef = ExerciseManager:getRongYanJuShouDef( 1 );
	for i=1, MAX_RONGYAN_BTN_NUM do
		local btn = getglobal("RongYanSelectFrameMonInfo"..i)
		if btn == nil then 
			break;
		end
		local szText = "  "..RongYanDef.RongYan[i-1].ExpMul.."���������Ҿ���\t ��Ҫ���Һ��ģ�"..RongYanDef.RongYan[i-1].ItemNum.."��";
		btn:SetText(szText);
		btn:SetClientUserData(0,i);
		btn:SetClientUserData(1,RongYanDef.RongYan[i-1].ExpMul);
		btn:SetClientUserData(2,RongYanDef.RongYan[i-1].ItemNum);
		local summonBtn = getglobal(btn:GetName().."ChooseBtn");
		summonBtn:SetClientUserData(0,i);
		summonBtn:SetClientUserData(1,RongYanDef.RongYan[i-1].ExpMul);
		summonBtn:SetClientUserData(2,RongYanDef.RongYan[i-1].ItemNum);
	end	
end

function RongYanSelectFrame_OnHide()
	ClearItemLock( "RongYanSelectFrame" );
end
function SetRongYanItemUseMapType( bType ) 
	bMapType = bType;
end
function UseRongYanSelectItem( itemID )
	if not RongYanItemUse then 
		ShowMidTips("�������Ҿ��޻�У��޷�ʹ�ã�");
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local container		= mainplayer:getContainer()
	--�ռ����Ҿ��޲����ƶӳ�
	if not mainplayer:isCaptain() and itemID ~= ZJ_RONGYAN_ITEM_ID_TOLUA then
		ShowMidTips("ֻ�жӳ�����ʹ�ø���Ʒ��");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips( "����״̬�У��޷�ʹ�ø���Ʒ" );
		return;
	end 
	local itemDef = getItemDef( itemID ) ;
	if itemDef ~= nil and itemDef.WearPre.RoleLevelMin > 0 and mainplayer:getLv() < itemDef.WearPre.RoleLevelMin then
		ShowMidTips("�ȼ�����");
		return;
	end
	local MinMapContal	= MapShower:GetMinMapContal();
	local g_nReferMapId	= MinMapContal:getMapID();
	local g_nRealMapId	= MapShower:getRealMapId();
	local nCurMapID 	= g_nReferMapId > 0 and g_nReferMapId or  g_nRealMapId ;
	local mapDef 		= getMapDef( nCurMapID );
	
	if g_nRealMapId >= MIN_COPY_ID then
		ShowMidTips( "��ǰ�����У��޷�ʹ�ø���Ʒ" );
		return;
	end
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷�ʹ�ø���Ʒ" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	for i = 1, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
		local item = container:getItem(CONTAINER_TYPE_SUNDRIES, i-1)
		local id = item:getItemId();
		if id == itemID then
			if itemID == ZJ_RONGYAN_ITEM_ID_TOLUA then
				LockItem(CONTAINER_TYPE_SUNDRIES, i-1, id, "RongYanHighLvCallFrame");
			else
				LockItem(CONTAINER_TYPE_SUNDRIES, i-1, id, "RongYanSelectFrame");
			end
		end
	end
	if itemID == ZJ_RONGYAN_ITEM_ID_TOLUA then
		RongYanHighLvCallFrame:Show();
	else
		RongYanSelectFrame:Show();
	end
end

function RongYanSelectBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	if MessageBoxForPetFrame:IsShown() then 
		MessageBoxForPetFrame:Hide();
	end
	if not mainplayer:isCaptain() then
		ShowMidTips("ֻ�жӳ����ܽ���ѡ��");
		return;
	end
	local g_nRealMapId	= MapShower:getRealMapId();
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷������ٻ�" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips( "����״̬�У��޷�ʹ�ø���Ʒ" );
		return;
	end 
	selectIndex = this:GetClientUserData(0);
	selectMulty = this:GetClientUserData(1);
	selectNum	= this:GetClientUserData(2);
	
	local szText = "��ѡ���ٻ�#cff0000��"..selectMulty.."����#n��������Ҿ���\n��Ҫ#cff9600��"..selectNum.."����#n���Һ���";
	local confirmText = "�Ƿ�ȷ���ٻ�?";
	MessageBoxForPet("�ٻ�ȷ��",szText,confirmText);
	MessageBoxForPetFrame:SetClientString( "�����ٻ�ȷ��" );
end

function RongYanSelectConfirmStart()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local g_nRealMapId	= MapShower:getRealMapId();
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷������ٻ�" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips( "����״̬�У��޷�ʹ�ø���Ʒ" );
		return;
	end 
	local container	= mainplayer:getContainer();
	local nItemNum = container:getItemCount( CLT_RONG_YAN_ITEM_ID );
	if nItemNum < selectNum then 
		local szText = "��ѡ���ٻ�#cff0000��"..selectMulty.."����#n��������Ҿ���\n��Ҫ#cff9600��"..selectNum.."����#n���Һ���";
		local confirmText = "��ǰ�㱳�������Һ�����������";
		MessageBoxForPet("�ٻ�ȷ��",szText,confirmText);
		MessageBoxForPetFrame:SetClientString( "���Һ��Ĳ���" );
		local cancelBtn = getglobal("MessageBoxForPetCancelButton");
		cancelBtn:SetText("���ٹ���");
		return;
	end
	ExerciseManager:sendBeginMultiplityNum( selectIndex );
	RongYanSelectFrame:Hide();
end

function QuickBuyRongYanItem()
	local shopId = MarketManager:getShopIDByID(BUY_TYPE_TICKET, CLT_RONG_YAN_ITEM_ID);
	if shopId > 0 then
		MarketBuyItemFrameBtn:SetClientUserData( 3, BUY_TYPE_TICKET );
		MarketManagerFrame_ShowBuyFrame(shopId);
		UpdateMarketBuyItemFrameMoneySymbol( BUY_TYPE_TICKET );
		FashionPreviewBuyFrame:Hide();
	else
		ShowMidTips("�̳�������û���ϼܸ���Ʒ");
	end
end 

function RongYanSelectItemLack()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local g_nRealMapId	= MapShower:getRealMapId();
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷������ٻ�" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips("����״̬�޷������ٻ������ȸ���");
		return;
	end
	local container	= mainplayer:getContainer();
	local nItemNum = container:getItemCount( CLT_RONG_YAN_ITEM_ID );
	if nItemNum < selectNum then 
		ShowMidTips("���Һ�����������");
		return;
	end
	ExerciseManager:sendBeginMultiplityNum( selectIndex );
	RongYanSelectFrame:Hide();
end
--�߼����Ҿ���
function RongYanHighLvCallFrameCallBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	if MessageBoxForPetFrame:IsShown() then 
		MessageBoxForPetFrame:Hide();
	end
	local g_nRealMapId	= MapShower:getRealMapId();
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷������ٻ�" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips( "����״̬�У��޷�ʹ�ø���Ʒ" );
		return;
	end 
	ExerciseManager:CallHighLvRongYanMonster()
	RongYanHighLvCallFrame:Hide()
end
local szHighLvRongYanInfo = [[������ÿ������ٻ�3ֻ�ռ����Ҿ���
�ٻ�������Ҫ5����
���ɹ���ɱ�ٻ��������Ҿ��޺�
�ٻ��߻��3�����飬Эͬ��ɱ�߻��1������]]
function RongYanHighLvCallFrame_OnLoad()
	RongYanHighLvCallFrameTipRich:setCenterLine(true)
	RongYanHighLvCallFrameTipRich:SetText( szHighLvRongYanInfo, 255,255,190 )
end
function RongYanHighLvCallFrame_OnHide()
	ClearItemLock( "RongYanHighLvCallFrame" )
end
function CloseHighLvRongYanStartUI()
	RongYanHighLvStartFrame:SetClientUserData( 0, 0 )
	RongYanHighLvStartFrame:SetClientUserData( 1, 0 )
	RongYanHighLvStartFrame:Hide()
	RongYanMiniFrame:Hide();
	RongYanMiniFrame:SetClientString( "" )
end
function OpenHighLvRongYanStartUI( time, expMul, monsterLv, nextExpMul, curExp )
	if time > 0 then
		local szTime = "����"..math.floor( time/60 ).."��"..math.floor( time%60 ).."���ٻ����ռ����Ҿ���"
		RongYanHighLvStartFrameTipFont:SetText( szTime )
	else
		RongYanHighLvStartFrameTipFont:SetText( "�ѽ��������ռ����Ҿ��޵�����" )
	end
	RongYanHighLvStartFrameInfoBtn1:SetText("   ��ǰ���Ҿ��޾��齱��Ϊ��\t\t"..expMul.."��");
	RongYanHighLvStartFrameInfoBtn2:SetText("   ��ǰ���Ҿ��޾��齱�����У�\t"..curExp);
	RongYanHighLvStartFrameInfoBtn3:SetText("   ��ǰ���Ҿ��޵ȼ�Ϊ��\t\t\t\t"..monsterLv.."��");
	RongYanHighLvStartFrameInfoBtn4:SetText("   ��һ�����Ҿ��޾��齱��Ϊ��\t"..nextExpMul.."��");
	RongYanHighLvStartFrame:Show()
	RongYanHighLvStartFrame:SetClientUserData( 0, time )
	RongYanHighLvStartFrame:SetClientUserData( 1, getServerTime().sec )
end
function RongYanHighLvStartFrame_OnUpdate()
	local lastTime = getServerTime().sec - RongYanHighLvStartFrame:GetClientUserData( 1 )
	local time = RongYanHighLvStartFrame:GetClientUserData(0) - lastTime
	if time > 0 then
		local szTime = "����"..math.floor( time/60 ).."��"..math.floor( time%60 ).."���ٻ����ռ����Ҿ���"
		RongYanHighLvStartFrameTipFont:SetText( szTime )
	else
		RongYanHighLvStartFrameTipFont:SetText( "�ѽ��������ռ����Ҿ��޵�����" )
	end
end
function RongYanHighLvStartFrame_MiniBtn_OnClick()
	RongYanHighLvStartFrame:Hide();
	RongYanMiniFrame:Show();
	RongYanMiniFrame:SetClientString( "RongYanHighLvStartFrame" )
end

-- ��С����ť
function RongYanMiniFrame_OnClick()
	local frame = getglobal( RongYanMiniFrame:GetClientString() )
	frame:Show();
	RongYanMiniFrame:Hide();
end

function RongYanMiniFrame_OnEnter()
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "���������Ҿ�����Ϣ���", 
								frame = this:GetName(), button = this:GetName() } );
end
function RongYanMiniFrame_OnShow()
	local tex = getglobal( this:GetName() .. "UVAnimationTex" );
	tex:SetUVAnimation( 50, true );
	tex:Show();
end
function RongYanMiniFrame_OnHide()
	local tex = getglobal( this:GetName() .. "UVAnimationTex" );
	tex:Hide();
end

--���Ҿ��� ��ʼUI
local nCurRongYanLayers 	= 0;
local nCurRongYanIndex		= 0;
function RongYanStartFrame_OnShow()
	UpdateRongYanMonsterInfo();
	this:SetPoint( "right", "UIClient", "right", -50, -30 )
end

function RongYanStartFrame_OnLoad()
	for i=1, MAX_RONGYAN_BTN_NUM do
		local btn = getglobal("RongYanStartFrameInfoBtn"..i)
		if i%2 == 0 then
			btn:SetNormalTextureTemplate( "BlackCategoryBkg" );
		end
	end
	this:RegisterEvent("GE_TEAM_GET_TEAMINFO");
	this:RegisterEvent("GE_PLAYER_LEAVEUP");
end
function RongYanStartFrame_OnEvent()
	if not RongYanStartFrame:IsShown() then 
		return;
	end
	if arg1 == "GE_TEAM_GET_TEAMINFO" then
		local teamInfo = TeamManager:getTeamInfo();
		local aveLv	= 0;
		for i=1,teamInfo.MemberNum do 
			aveLv = aveLv + teamInfo.TeamMember[i-1].Level;
		end
		aveLv = math.floor( aveLv / teamInfo.MemberNum );
		RongYanStartFrameInfoBtn3:SetText("   ��ǰ����ƽ���ȼ���\t\t\t\t\t"..aveLv.."��");
	elseif arg1 == "GE_PLAYER_LEAVEUP"  then
		UpdateRongYanMonsterInfo();
	end
end
function OpenRongYanMonsterUI( nCurLayer, nIndex ) 
	RongYanItemUse = false;
	for i=1, #t_RongYanAllFrame do 
		local frame = getglobal(t_RongYanAllFrame[i]);
		if frame:IsShown() then 
			frame:Hide();
		end
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	nCurRongYanLayers = nCurLayer;
	nCurRongYanIndex  = nIndex; 
	RongYanStartFrame:Show();
end
function UpdateRongYanMonsterInfo()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local RongYanDef = ExerciseManager:getRongYanJuShouDef( nCurRongYanLayers );
	local NextRongYanDef = ExerciseManager:getRongYanJuShouDef( nCurRongYanLayers + 1 );
	
	RongYanStartFrameInfoBtn1:SetText("   ��ǰ���Ҿ��޾��齱��Ϊ��\t\t"..RongYanDef.RongYan[nCurRongYanIndex-1].ExpMul.."��");
	if NextRongYanDef == nil then
		RongYanStartFrameInfoBtn4:SetText("");
	else
		RongYanStartFrameInfoBtn4:SetText("   ��һ�����Ҿ��޾��齱��Ϊ��\t"..NextRongYanDef.RongYan[nCurRongYanIndex-1].ExpMul.."��");
	end
	local Exp = 0;
	local RongYanDynDef = ExerciseManager:getRongYanMonAttrDef( mainplayer:getLv() );
	local contriRate	= getLevelUP( mainplayer:getLv() ).ContributeExp / 10000;
	local baseExp 		= ExerciseManager:getRongYanMonsterBootyExp( RongYanDef.MonID );
	local nMulty		= RongYanDef.RongYan[nCurRongYanIndex-1].ExpMul;
--	print("���� ����ͳ��",baseExp,nMulty,mainplayer:getLv(),contriRate,RongYanDynDef.DynExpMul)
	local finalExp		= math.floor( baseExp * nMulty * contriRate * ( 1 + RongYanDynDef.DynExpMul / 100 ) );
	RongYanStartFrameInfoBtn2:SetText("   ��ǰ���Ҿ��޾��齱�����У�\t"..finalExp );
	
	local teamInfo = TeamManager:getTeamInfo();
	if teamInfo == nil then 
		return;
	end
	local aveLv	= 0;
	for i=1,teamInfo.MemberNum do 
		aveLv = aveLv + teamInfo.TeamMember[i-1].Level;
	end
	aveLv = math.floor( aveLv / teamInfo.MemberNum );
	RongYanStartFrameInfoBtn3:SetText("   ��ǰ����ƽ���ȼ���\t\t\t\t\t"..aveLv.."��");
end
function RongYanStartFrame_MiniBtn_OnClick()
	RongYanStartFrame:Hide();
	RongYanMiniFrame:Show();
	RongYanMiniFrame:SetClientString( "RongYanStartFrame" )
end

--------�������bossѡ�񲿷�
local guessTime = 0;
function OpenRongYanMonsterGuessUI( nCurLayer, nIndex )
	for i=1,#t_RongYanAllFrame do 
		local frame = getglobal(t_RongYanAllFrame[i]);
		if frame:IsShown() then
			frame:Hide();
		end
	end
	RongYanItemUse = false;
	
	if nCurLayer < 0 then     -- ��ʶ���޻����
		RongYanItemUse = true;
		return;
	end
	if nCurLayer == 0 then    -- ��ʶ��boss
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local RongYanDef 		= ExerciseManager:getRongYanJuShouDef( nCurLayer );
	local NextRongYanDef 	= ExerciseManager:getRongYanJuShouDef( nCurLayer + 1 );
	local iconID = 200033;
	if NextRongYanDef == nil then 
		RongYanGuessFrameTipFont2:SetText("��һ�����Ҿ��޵ľ��齱��Ϊ��"..RongYanDef.RongYan[nIndex-1].ExpMul.."��");
	else 
		RongYanGuessFrameTipFont2:SetText("��һ�����Ҿ��޵ľ��齱��Ϊ��"..NextRongYanDef.RongYan[nIndex-1].ExpMul.."��");
	end
	RongYanGuessFrameMonBtn1IconTexture:SetTexture( GetMonsterIconPath( iconID ) )
	RongYanGuessFrameMonBtn2IconTexture:SetTexture( GetMonsterIconPath( iconID ) )
	guessTime = getServerTime().sec;
	RongYanGuessFrame:Show();
end 

function RongYanGuessFrame_OnShow()

end 
function RongYanGuessFrame_OnLoad()
	this:setUpdateTime(1);
end
function RongYanGuessFrame_OnUpdate()
	local curTime = getServerTime().sec;
	local nTime = curTime - guessTime ;
	if nTime >= 180 then 
		ExerciseManager:sendBossChooseOption( false );
		RongYanGuessFrame:Hide();
	end
end
function RongYanGuessSelectBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local g_nRealMapId	= MapShower:getRealMapId();
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷�����ѡ��" );
		return;
	end
	if g_nRealMapId >= MIN_COPY_ID then
		ShowMidTips( "��ǰ�����У��޷�����ѡ��" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips("����״̬�޷������ٻ������ȸ���");
		return;
	end
	ExerciseManager:sendBossChooseOption( true );
	RongYanGuessFrame:Hide();
end

--�߼����Ҿ������ѡ��
local highLvGuessTime = 0
function OpenHighLvRongYanGuess( open, nextExpMul )
	if open == 0 then
		HighLvRongYanGuessFrame:Show();
		highLvGuessTime = getServerTime().sec
	else
		HighLvRongYanGuessFrame:Hide();
		highLvGuessTime = 0
	end
	HighLvRongYanGuessFrameTipFont2:SetText("��һ�����Ҿ��޵ľ��齱��Ϊ��"..nextExpMul.."��")
	local iconID = 200033;
	HighLvRongYanGuessFrameMonBtn1IconTexture:SetTexture( GetMonsterIconPath( iconID ) )
	HighLvRongYanGuessFrameMonBtn2IconTexture:SetTexture( GetMonsterIconPath( iconID ) )
end
function HighLvRongYanGuessSelectBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		return;
	end
	local g_nRealMapId	= MapShower:getRealMapId();
	if g_nRealMapId == 1000 then
		ShowMidTips( "ʱ������޷�����ѡ��" );
		return;
	end
	if g_nRealMapId >= MIN_COPY_ID then
		ShowMidTips( "��ǰ�����У��޷�����ѡ��" );
		return;
	end
	if bMapType	then
		ShowMidTips("������������ʹ�ø���Ʒ");
		return;
	end
	if mainplayer:isDead() then 
		ShowMidTips("����״̬�޷������ٻ������ȸ���");
		return;
	end
	ExerciseManager:sendHighLvBossChooseOption( true );
	HighLvRongYanGuessFrame:Hide();
end
function HighLvRongYanGuessFrame_OnUpdate()
	local curTime = getServerTime().sec;
	local nTime = curTime - highLvGuessTime ;
	if nTime >= 180 then 
		ExerciseManager:sendHighLvBossChooseOption( false );
		HighLvRongYanGuessFrame:Hide();
	end
end
