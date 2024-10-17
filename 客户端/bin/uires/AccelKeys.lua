local bAccelKey = false;

local t_winCloseHandlers =	{
								["NpcTalkFrame"]		= Close_NpcDialog,					
								["TaskAcceptFrame"]		= function () TaskAcceptFrame_LeaveBtn_OnClick(); end,
								["TaskNotFinishFrame"]	= Close_NpcDialog,					["TaskAwardFrame"]		= Close_NpcDialog, 
								["FriendChatFrame1"]	= CloseChatFrame,					["FriendChatFrame2"]	= CloseChatFrame,
								["SkillLoadingFrame"]	= Close_SkillLoadingDialog,	
								["FriendChatFrame3"]	= CloseChatFrame,					["FriendChatFrame4"]	= CloseChatFrame,
								["FriendChatFrame5"]	= CloseChatFrame,					["FriendChatFrame6"]	= CloseChatFrame,
								["RequestJoinClanFrame"]= RequestJoinClanFrame_RefuseBtn_OnClick, 
								["HundredLevelTownFinishDialog"]= HundredLevelTownFinishDialog_OnClose,
								["HundredLevelTownFailDialog"]= HundredLevelTownFailDialog_OnClose,

								["FriendAddCharacterFrame"] = function () FriendAddCharacterFrame_Cancel_OnClick(); end,
								["FriendMemFrame"] = function () FriendMemFrame_Cancel_OnClick(); end,
								["RequestTradeFrame"] = function () RequestTradeFrame_RefuseBtn_OnClick(); end,
								["TroopInviteFollowFrame"] = function () TroopInviteFollowFrame_RufuseOneButton_OnClick(); end,
								["TroopInviteFrame"] = function () RufuseOneButton_OnClick(); end,
								["InviteRideFrame"] = function () InviteRideFrame_RefuseBtn_OnClick(); end,
								["AutoTeamEnterPworldTimeDownFrame"] = function () AutoTeamEnterPworldTimeDownFrame_ConfirmButton_OnClick(); end,
								["PetRongHeProcessFrame"] = function () PetRongHeProcessFrame_CancelBtn_OnClick(); end,
								["ReqirePetRongHeFrame"] = function () ReqirePetRongHeFrame_RefuseBtn_OnClick(); end,
								["StallSaleFrame"] = function () StallSaleFrame_CloseBtn_OnClick(); return true; end,
								["PKMessageBoxFrame"] = function () PKMessageBoxFrameCancelBtnOnClick(); end,
								["ArmRecastSelectAttrFrame"] = function () ArmRecastSelectAttrFrameCloseBtn_OnClick(); return true end,
								["ArmWashFrame"] = function () ArmWashFrameCloseBtn_OnClick(); return true end,
								["TaskLogDescFrame"] = function () TaskLogFrame:Hide(); return true end,
								["FriendMutilSendFrame"] = function () FriendMutilSendFrame:Hide(); return true end,
								["ArmAdvWashFrame"] = function () ArmAdvWashFrameCloseBtn_OnClick(); return true end,
								["CardBoxFrame"] = function () CardBoxFrameCloseBtn_OnClick(); return true end,
								["CardBoxExtendFrame"] = function () CardBoxFrameCloseBtn_OnClick(); return true end,
								["ArmBaseResetFrame"] = function () ArmBaseResetFrameCloseBtn_OnClick(); return true end,
							};

t_winCloseHandlers["TradeFrame"] = 
function ()
	GameExchange:cancelExchange( INITIATIVE_REJECT );
end

t_winCloseHandlers["MailHeadFrame"] = 
function ()
	MailFramesClose("MailHeadFrame");
	return true;
end

t_winCloseHandlers["MailSendItemFrame"] = 
function ()
	MailFramesClose("MailSendItemFrame");
	return true;
end

t_winCloseHandlers["IntelligenceRaceJoinFrame"] = 
function ()
	IntelligenceRaceJoinWaitBtn_OnClick();
end

t_winCloseHandlers["IntelligenceRaceFrame"] = 
function ()
	IntelligenceRaceFramesCloseBtn_OnClick();
	return true;
end
--队友召唤卷轴的响应
t_winCloseHandlers["TeamMemberSummonFrame"] = 
function ()
	return true;
end

t_winCloseHandlers["ClanMemberSummonFrame"] = 
function ()
	return true;
end

t_winCloseHandlers["MailReadFrame"] = 
function ()
	MailReadFrame:Hide();
end

t_winCloseHandlers["webframe"] = 
function ()
	--webframe:Hide();
end

t_winCloseHandlers["CreatingAutoTeamFrame"] = 
function ()
	BeginPlayAutoTeamBtnEffect();
end

t_winCloseHandlers["SuccessCreatTaskAutoTeamFrame"] = 
function ()
	BeginPlayAutoTeamBtnEffect();
end

t_winCloseHandlers["SuccessCreatActiveAutoTeamFrame"] = 
function ()
	BeginPlayAutoTeamBtnEffect();
end

function t_winCloseHandlers.HundredLevelTownFinishFrame()
	if not GameWheelLotteryFrameClose:IsEnable() then
		return true
	end
	if GameWheelLottery:remain() > 0 then
		HundredLevelTownLotteryRemindDialog:Show()
		return true
	end
end

function t_winCloseHandlers.GameWheelLotteryFrame()
	return not GameWheelLotteryFrameClose:IsEnable()
end

function t_winCloseHandlers.HundredLevelTownFinishDialog()
	HundredLevelTownFinishDialog_OnClose()
end

function t_winCloseHandlers.HundredLevelTownFailDialog()
	HundredLevelTownFailDialog_OnClose()
end

function t_winCloseHandlers.MapLoadingMask()
	return true
end

function t_winCloseHandlers.PworldScoreFrame()
	PworldScoreFrame_OnEsc()
	return true
end

t_winCloseHandlers["VipWheelLotteryFrame"] = 
function ()
	return not VipWheelLotteryFrameClose:IsEnable()
end

t_winCloseHandlers["ArmProceStarFrame"] =
function()
	return ArmProceStarFrame_OnESC()
end

t_winCloseHandlers["CSFashionFrame"] =
function()
	return CSFashionFrame_OnEsc()
end

t_winCloseHandlers["ArmRecastFrame"] =
function()
	return ArmRecastFrame_OnESC()
end

t_winCloseHandlers["ArmAdvRecastFrame"] =
function()
	return ArmAdvRecastFrame_OnESC()
end

t_winCloseHandlers["RongYanGuessFrame"] =
function()
	return true;
end

t_winCloseHandlers["RongYanStartFrame"] =
function()
	RongYanStartFrame_MiniBtn_OnClick();
	return true;
end
t_winCloseHandlers["RongYanHighLvStartFrame"] =
function()
	RongYanHighLvStartFrame_MiniBtn_OnClick();
	return true;
end

local t_uncloseWins =	{
							"ReviveFrame", "PartyMemberBuffTooltip", "ArmDurSignFrame", "FadeSceneNameFrame", 
							"FollowHintFrame", "IntonateBar", "GameMiddleTipsFrame", "ChatShowBugleMsgFrame", "CameraCoverFrame",
							"PhotoFrame", "TeamRoleFrame" , "DamageCountFrame", "GameGetItemTipsFrame", "PetUI", "XPFrame", "TaskTraceFrame","SortInfoMsgFrame",	"EffectFrame",
							"UiEffectFrame",	"XinShouCenterGuideFrame", "SelectLeftHabitFrame","CountdownReviveFrame","ShortCutButtonsFrame","RollItemFrame",
							"TmpItemPickTipsFrame","QuestHelperFrame","ClanPworldIslandShowInfoFrame","ClanMachinePworldTimeShowFrame",
							"ClanStrongWarResFrame","OfflineExpBuffFrame", "CostMoneyConfirmFrame", "NiXiReviveFrame", "ChatAllBugleMsgFrame",
							"AutoFiniQuestFrame","AutoFiniQuestMiniFrame","HelperReviveFrame","SpartaWarInfoFrame","XuKongBalanceFrame","FlowerReportFrame",
							"ReturnGiftFrame","ClanWarResultFrame","TopCareerWinInfoFrame","ClanSpanStrongWarResFrame","ShiKongInfoFrame","ShiKongQueueTimeFrame",
						};

for _, name in ipairs( t_uncloseWins ) do
	pushUnCloseWin( name );
end 


-- 此为特别处理面板，但是其优先级不为dialog，故需特别处理
local t_specialFrames = {};
t_specialFrames["Target"] = 
function ()
	local mainplayer = ActorMgr:getMainPlayer();
	mainplayer:SetSelectTarget( 0 );
	Target:Hide();
end

t_specialFrames["LinkItemTipsFrame"] = 
function ()
	LinkItemTipsFrame:Hide();
	if ArmItemTipsFrame:IsShown() then
		ArmItemTipsFrame:Hide();
		SecondArmItemTipsFrame:Hide();
	end
end

t_specialFrames["TroopInviteFrame"] =
function ()
	RufuseOneButton_OnClick();
end

t_specialFrames["TaskAcceptFrame"] = 
function ()
	TaskAcceptFrame_LeaveBtn_OnClick();
end

-- 选人登录之类的界面
local t_LoginFrames = {};
t_LoginFrames["DirServerFrame"] = {["onEnter"] = DirServerFrame_OnEnterPress, 
								["onEsc"] = function () if DirMessageBoxFrame:IsShown() then
															DirMessageBoxFrame:Hide();
															return;
														end 
														DirServerFrame:Hide();
														DirSimpleServerFrame:Show();
														end };

t_LoginFrames["LoginFrame"] = {["onEnter"] = LoginFrame_OnEnterPressed, ["onEsc"] = LoginReturnBtn_OnClick,};
t_LoginFrames["SelectFrame"] = {["onEnter"] = SelectFrame_EnterGameButton_OnClick, ["onEsc"] = ReturnToLoginFrameByEsc,};
t_LoginFrames["CSSelectRaceDescFrame"] = {["onEnter"] = CSSelectNextStepButtonOnClick,["onEsc"] = CSSelectPerStepButtonOnClick,};
t_LoginFrames["CSSelectRoleFrame"] = {["onEnter"] = CSSelectNextStepButtonOnClick,["onEsc"] = CSSelectPerStepButtonOnClick,};
t_LoginFrames["CSCreateFrame"] = {["onEnter"] = CSGenderAcceptButtonOnClick,["onEsc"] = CSGenderCancelButtonOnClick,};
t_LoginFrames["DirSimpleServerFrame"] = {["onEnter"] = DirSimpleFrameSelectBtn_OnClick,
								["onEsc"] = function () if DirMessageBoxFrame:IsShown() then
															DirMessageBoxFrame:Hide();
															return;
														end 
														if not DirConfirmQuiGameFrame:IsShown() then
															DirConfirmQuiGameFrame:Show();
														end
													end };




local t_loginConfirmDialog = 
{
["ReadUserProtocolFrame"] = 
{ ["onEnter"] = 
function ()
	local frame = getglobal( "ReadUserProtocolFrame" );
	if frame:IsShown() then
		frame:Hide();
		return true;
	end

	return false;
end 
},

["DirMessageBoxFrame"] = 
{ ["onEnter"] = 
function ()
	local frame = getglobal( "DirMessageBoxFrame" );
	if frame:IsShown() then
		DirFrameOkBtn_OnClick();
		return true;
	end

	return false;
end 
},

};

function AccelKey_Enter()
	if arg1 ~= "keydown" then
		return;
	end
	local Frame
	-- 对 BlockMessageBox 的响应
	Frame = BlockMessageBox;
	if Frame ~= nil and Frame:IsShown() then
		bAccelKey = true;
		local oldThis = this;
		this = getglobal(Frame:GetName().."Enter");
		BlockMessageBox_OnEnter();
		this = oldThis;
		return;
	end
	-- 对 messageBox 的响应
	Frame = getglobal( "MessageBoxFrame" );
	if Frame ~= nil and Frame:IsShown() then
		bAccelKey = true;
		MessageBoxAcceptBtnOnClick();
		return;
	end
	
	Frame = getglobal( "SelectRoleWarnFrame" );
	if Frame:IsShown() then
		bAccelKey = true;
		SelectRoleWarnFrame_ConfirmBtn_OnClick();
		return;
	end
	Frame = getglobal("MarketBuyTicketFrame");
	if Frame:IsShown() then
		bAccelKey = true;
		MarketBuyTicketFrameConfirm_OnClick();
		return;
	end
	Frame = getglobal("MarketSaleTicketFrame");
	if Frame:IsShown() then
		bAccelKey = true;
		MarketSaleTicketFrameConfirm_OnClick();
		return;
	end
	for _, data in pairs( t_loginConfirmDialog ) do
		local onEnterControl = data["onEnter"];
		if onEnterControl ~= nil and onEnterControl() then
			return;
		end
	end

	if getGameState() ~= STATE_INWORLD then
		--对人物选择和登录界面特殊处理
		for name,func in pairs(t_LoginFrames) do
			local frame = getglobal( name );
			if frame ~= nil and frame:IsShown() then
				bAccelKey = true;
				if func.onEnter ~= nil then
					func.onEnter();
					return;
				end
			end
		end
		
		return;
	end
	if GameMgr:isInPlayCamerAnim() then
		return;
	end
	if ExchangeMoneyConfirmFrame:IsShown() then
		ExchangeMoneyConfirmFrameOKBtnOnClick();
		return;
	end
	
	Frame = getglobal( "ChatInsertFrame" );
	--[[
	if not Frame:IsShown() then
		return;
	end
	--]]

	bAccelKey = true;
	
	if (Frame:IsShown()) and GetFocusFrameParent() == "ChatInsertFrame" then
		ClearChatInsertFram();
		SetFocusFrame("UIClient");
	elseif (Frame:IsShown()) and GetFocusFrameParent() ~= "ChatInsertFrame"  then
		SetFocusFrame("CSChatInsert");
		DisCheckShortcutButton();
	
	
	elseif not Frame:IsShown() and ChatControlFrame:IsShown() then
		local nChatType = GetChatType();
		SetChatType( nChatType );
		Frame:Show();
		MainChatFrame:Show();
		SetFocusFrame("CSChatInsert");
		DisCheckShortcutButton();
	end
end

function IsAccelKey()
	return bAccelKey;
end

function SetAccelKey( bRet )
	bAccelKey = bRet;
end

function AccelKey_Esc()
	if GameMgr:isInPlayCamerAnim() then
		GameMgr:stopPlayCameraAnim();
		return;
	end
	if arg1 ~= "keydown" then
		return;
	end
	-- 对 messageBox 的响应
	local Frame = getglobal( "MessageBoxFrame" );
	if Frame ~= nil and Frame:IsShown() then
		MessageBoxCancelBtnOnClick();
		return;
	end

	Frame = getglobal( "SelectRoleWarnFrame" );
	if Frame:IsShown() then
		SelectRoleWarnFrame:Hide();
		return;
	end

	if LoadingFrame:IsShown() then 
		return
	end
	if getGameState() ~= STATE_INWORLD then
		--对人物选择和登录界面特殊处理
		for name,func in pairs(t_LoginFrames) do
			local frame = getglobal( name );
			if frame ~= nil and frame:IsShown() then
				if func.onEsc ~= nil then
					func.onEsc();
					return;
				end
			end
		end
		return;
	end

	if ShowChangeRoleFrame:IsShown() or StartMudFrame:IsShown() then
		return;
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer ~= nil then 
		local specialMove = mainplayer:getSpecialMove();
		if specialMove:checkCancelFirstAnimFly() then
			return;
		end
	end	
	 
	-- 对 GuideFrame 的响应	   
	local Frame = getglobal( "GuideFrame" );
	if Frame:IsShown() then
		GuideFrameCloseBtn_OnClick();
		return;
	end
	local szFrameName = GetCurrentDialogFrame();
	local dialogframe = getglobal( szFrameName );
	--附魔界面 特殊处理下
	if szFrameName == "EnchantFrame" then 
		coroutine.wrap(EnchantFrameCloseBtn_OnClick)();
		return;
	end
 	-- 优先处理优先级高的面板
	if dialogframe ~= nil then
		--Npc对话框关闭调用Close函数
		if t_winCloseHandlers[szFrameName] and t_winCloseHandlers[szFrameName](szFrameName) then
			return;
		end
		dialogframe:Hide();
		return;
	end

	for name, func in pairs( t_specialFrames ) do 
		local frame = getglobal( name );
		if frame:IsShown()  then
			func( name );
			return;
		end
	end
	if SkillMgr:getIntentSpell() then
		SkillMgr:clearIntentSpell();
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local machine = mainplayer:getMonsterMachine();
	if machine:cancelSkillSelectData() then
		return;
	end
	-- 当上面一切处理完后才会响应到这边
	if SelectLeftHabitFrame:IsShown() then
		return;
	end
	local specialMove = mainplayer:getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	SystemButtonOnClick();
end

function AccelKey_AltF4()
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then
		GameMgr:QuitGame();
	end
	if arg1 == "keydown" then
		if ViewLockFrame:IsShown() then
		else			
			WarnLeaveGame();
		end
	end
end

function AccelKey_CtrlF()
	if arg1 ~= "keydown" then
		return;
	end
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then 
		return 
	end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local TargetObj	 = ActorMgr:FindActor( mainplayer:GetSelectTarget( ) );	
	--存在目标时使用快捷键对目标进行普通跟随，没有目标时进行组队跟随
	if GameFollowManager:isWeiXing() then
		GameFollowManager:cancelWeiXing();
		PhotoFrameStopWeiXingButton:Hide();
		return;
	end
	if TeamManager:isInTeamFollow() then
		TeamManager:Team_OP_CancelTeamFollow();	
		return;	
	end
	if TargetObj == nil then
		--当前选中的目标为队员且不在视野中
		local bIsTeam, nName = getTargetInfo();
		if mainplayer:isInTeam() and bIsTeam then
			if mainplayer:isCaptain() then
				local teamInfo = TeamManager:getTeamInfo();
				for i = 1, teamInfo.MemberNum do
					local teamMember = teamInfo.TeamMember[i-1];
					if teamMember.MemberName == nName then
						ShowMidTips( "队长不能跟随队员" );
						return;
					end
				end
			end
			GameFollowManager:beginWeiXing( nName, 0, 0 );
			return;	
		end
		--当前没有目标
		if mainplayer:isCaptain() then
			--取消组队跟随
			if isAnyTeamMemberInFollow() then
				TeamManager:Team_OP_CancelTeamFollow();		
				return;
			end
		end
		if mainplayer:isInTeam() and canTeamFollow() then
			TeamManager:Team_OP_AppealTeamFollow();
			setTeamFollowBegin();
			TroopInviteTeamFollowFrame:Hide();
			return;
		end
		return;
	end
	if TargetObj:getType() == GAT_PLAYER then
		if mainplayer:isCaptain() then
			local teamInfo = TeamManager:getTeamInfo();
			for i = 1, teamInfo.MemberNum do
				local teamMember = teamInfo.TeamMember[i-1];
				if teamMember.MemberName == TargetObj:getName() then
					ShowMidTips( "队长不能跟随队员" );
					return;
				end
			end
		end
		GameFollowManager:beginWeiXing( TargetObj:getName(), 0, 0 );
		return;
	end	
	ShowMidTips( "无法跟随当前目标" );
	return;
end

function AccelKey_Up()
	if arg1 == "keydown" then
		if getGameState() ~= STATE_INWORLD and SelectFrame:IsShown() then
			local frame = getglobal( "MessageBoxFrame" );
			if frame:IsShown() and frame:GetClientString() == "确定删除角色" then
				return;
			end

			frame = getglobal( "SelectRoleWarnFrame" );
			if frame:IsShown() then
				local t_SelectRoleWarnFrameControl = GetSelectRoleWarnFrameControl()
				local szOpDesc = t_SelectRoleWarnFrameControl["opDesc"];
				if szOpDesc == "确定删除角色" then
					return;
				end
			end
			
			local nTotalRoleNum = LoginMgr:getNumRole();
			if nTotalRoleNum == 0 then
				return;
			end

			local nCurSelect = LoginMgr:getSelectRoleIndex() - 1;
			if nCurSelect < 0 then
				nCurSelect = nTotalRoleNum -1;
			end
			ClearBirthFrame();				-- 切换角色时隐藏出生地选择面板
			LoginMgr:selectRole( nCurSelect );
			SelectFrame_UpdateAvatar( { ["roleIndex"] = nCurSelect, } );
			Private_UpdateSelectRoleBtn();
			Private_UpdateDeleteAndEnterBtn();
		end
	end
end

function AccelKey_Down()
	if arg1 == "keydown" then
		if getGameState() ~= STATE_INWORLD and SelectFrame:IsShown() then
			local frame = getglobal( "MessageBoxFrame" );
			if frame:IsShown() and frame:GetClientString() == "确定删除角色" then
				return;
			end

			frame = getglobal( "SelectRoleWarnFrame" );
			if frame:IsShown() then
				local t_SelectRoleWarnFrameControl = GetSelectRoleWarnFrameControl()
				local szOpDesc = t_SelectRoleWarnFrameControl["opDesc"];
				if szOpDesc == "确定删除角色" then
					return;
				end
			end
			
			local nTotalRoleNum = LoginMgr:getNumRole();
			if nTotalRoleNum == 0 then
				return;
			end

			local nCurSelect = LoginMgr:getSelectRoleIndex() + 1;
			if nCurSelect > nTotalRoleNum -1 then
				nCurSelect = 0
			end
			ClearBirthFrame();				-- 切换角色时隐藏出生地选择面板
			LoginMgr:selectRole( nCurSelect );
			SelectFrame_UpdateAvatar( { ["roleIndex"] = nCurSelect, } );
			Private_UpdateSelectRoleBtn();
			Private_UpdateDeleteAndEnterBtn();
		end
	end
end

function AccelKey_Slash()
	local szFocusFrameName = GetFocusFrameName();
	if szFocusFrameName == "CSChatInsert" or szFocusFrameName == "ChatInsertFrame_PrivateNameEdit" then
		return;
	end
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	SetFocusFrame( "CSChatInsert" );
end

-- 快捷栏快捷键（F1-F10、1-10）按下响应
function AccelKey_OnPress( keyindex )
	local shorcutbtn = getglobal( "MagicShortcut"..keyindex );
	if shorcutbtn == nil then
		return;
	end
	
	local nTargetIndex	= 0;
	if shorcutbtn:GetClientUserData(0) == SHORTCUT_ITEM then
		local itemid = shorcutbtn:GetClientUserData(1);
		if itemid == 0 then
			return;
		end

		if IsItemLock( 0, 0, itemid ) then
			ShowMidTips("当前物品被锁定不可使用");
			return;
		end

		local itemDef = getItemDef( itemid );
		if itemDef == nil then
			return;
		end

		local MainPlayer	= ActorMgr:getMainPlayer();
		local container		= MainPlayer:getContainer();
		local used_Item = container:getItem( itemid );
		if not CanUseItem( itemDef, true, used_Item ) then
			return;
		end
		
		if container:getItemCount( itemid ) > 0 then
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_EXP_BUCHANG ) then
				OpenReturnGiftExp( used_Item )
				return
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_BUCHANG ) then
				OpenReturnArmSelect( used_Item, MainPlayer:getLv() )
				return
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_SELECT ) then
				for i = 1, MAX_ITEM_RESULT do
					local resultData = itemDef.Result[i-1];
					if resultData.ResultID == RESULT_ITEM_ARM_SELECT then
						OpenReturnArmSelect( used_Item, resultData.ResultVal1*10 )
					end
				end
				return
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_GEM_PAG ) then
				OpenGemSelectPag( used_Item )
				return
			end
			
			local t_GameCBTFrameControl = GetGameCBTFrameControl();
			local gameCBT				= MainPlayer:getCBTInfo();
			if gameCBT:isCangBaoTuItem( used_Item:getItemId() ) then
				if t_GameCBTFrameControl:canStartCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } ) then
					t_GameCBTFrameControl:startCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
				end
				return;
			end

			local t_changeCareerControl = GetChangeCareerControl();
			if t_changeCareerControl:isChangeCareerItem( { id = used_Item:getItemId() } ) then
				if t_changeCareerControl:canStartChangeCareer( { id = used_Item:getItemId() } ) then
					t_changeCareerControl:startChangeCareer( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
				end
				return;
			end
			-- 熔岩骇心使用
			if itemid == CLT_RONG_YAN_ITEM_ID or itemid == ZJ_RONGYAN_ITEM_ID_TOLUA then 
				UseRongYanSelectItem( itemid );
				return;
			end
			if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_TASK_RECYCLE ) then
				if MainPlayer:isDead() then
					ShowMidTips("你已经死亡");
				else
					Quest:RequestRecycleTaskByItem( used_Item:getListType(), used_Item:getGridIdx() );
				end
				return
			end
		end

		if itemDef.UseTarget == USE_TARGET_PET then
			nTargetIndex = getFightPetIndex();
		end
	end
	
	if not CanUseShort( keyindex - 1 ) then
		return;
	end
	
	if keyindex == 1 then
		local tempXinShouBtn = getglobal( "TempShortCutFrameAttackXinShouBtn" );
		if tempXinShouBtn:IsShown() then
			CheckTempShortCutFrameXinShou();
			tempXinShouBtn:Hide();
		end
	end
	
	-- 隐藏指引TIPS提示
	GuideTipsEffectHide();
	
	GameShortCut:usedShortCut( keyindex - 1, nTargetIndex );
	shorcutbtn:Checked();
end

-- 拾取附近物品
local function PickItem(press)
	if press then
		ItemPicker:pickAllItems()
	end
end

-- 打开好友面板
local function OpenFriendFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if press then
		FriendsButtonOnClick( "FriendsButton" );
	end
end

-- 交易
local function StartExchange(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		ChangeCursorState( "exchange" );
	end
end

-- 打开精灵面板
local function OpenXianFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		XianButtonOnClick();
	end
end

-- 打开活动助手
local function OpenActiveHelperFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if not ActivityShowBtn:IsShown() then
		return;
	end
	if press then
		ActivityShowBtn_nClick()
	end
end

-- 切换PK模式
local function ChangePkMode(press)
	if press then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer:isInPVPMap() then
			ShowMidTips( "跨服不允许切换战斗模式" )
		else
			GameActorPVP:changeNextPkMode();
		end
	end
end

--打开人物界面
local function OpenRoleFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then 
		return 
	end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		CharacterButtonOnClick();
	end
end

--打开公会界面
local function OpenClanFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if not press then
		return;
	end

	ChanButtonOnClick();
end

--打开技能界面
local function OpenSkillFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then
		return;
	end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		local t_Frame = { "DiathesisFrame", "SkillFrame"};
		local bShow = false;
		for _, name in ipairs( t_Frame ) do
			local Frame= getglobal( name );
			if (Frame:IsShown()) then
				Frame:Hide();
				bShow = true;
			end
		end

		if not bShow then
			GuideEffectHide( "SkillButton" );
			t_hideIcons = { "SkillButtonUVAnimationTex" };
			util.HideIcons( t_hideIcons )
			SkillFrame:Show();
		end
	end
end

--打开天赋界面
local function OpenDiathesisFrame(press)	
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if not TianFuShortcutButton:IsShown() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		TianFuButtonOnClick();
	end
end

-- 打开伤害统计
local function OpenDamageCountFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if not press then
		return;
	end

	if DamageCountFrame:IsShown() then
		DamageCountFrame:Hide();
	else
		DamageCountFrame:Show();
	end
	return;
end

-- 打开科技生产
local function OpenSecondarySkillFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if not press then
		return;
	end

	if SecondarySkillFrame:IsShown() then
		SecondarySkillFrame:Hide()
	else
		SecondarySkillFrame:Show();
	end

	--[[
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() < 30 then
		AddGameMiddleTips( GetMidTipsList()[MID_TRUST_NOTENOUGHLV] );
		return;
	end
	ShowMidTips("该功能暂时未开放");
	local Frame= getglobal( "TrustFrame" );
	if Frame:IsShown() then
		Frame:Hide();
	else
		Frame:Show();
	end
	]]--
end

--打开成长之路
local function OpenGrowUpFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if  not HelpShowBtn:IsShown() then
		return;
	end
	if press then
		ShorcutOpen();
	end
end

--打开能源界面
local function OpenSparFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if not SparButton:IsShown() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		SparButtonOnClick();
	end
end

--打开邮箱
local function OpenMailFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if not press then
		return;
	end

	local t_mailFrames = { "MailHeadFrame", "MailReadFrame", "MailSendItemFrame" };
	for _, name in ipairs( t_mailFrames ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
			return;
		end
	end

	local frame= getglobal( "MailHeadFrame" );
	frame:Show();
end

--打开背包
local function OpenBag(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		ContainerButtonOnClick();
		--[[
		local Frame= getglobal( "PlayerContainerFrame" );
		if (Frame:IsShown()) then
			Frame:Hide();
		else
			if not StashFrame:IsShown() then
				PlayerContainerFrame:SetPoint("center", "$parent", "center", 0, 0 );
			end
			Frame:Show();
		end
		--]]
	end
end

--打开任务日志
local function OpenTaskLogFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		TaskButtonOnClick();
		--[[
		local Frame= getglobal( "TaskLogFrame" );
		if (Frame:IsShown()) then
			Frame:Hide();
		else
			Frame:Show();
		end
		--]]
	end
end

-- 设置快捷发言
local function SetAccelSendMsgFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end

	if press then
		local Frame= getglobal( "ChatShortcutSendMsgFrame" );
		if (Frame:IsShown()) then
			Frame:Hide();
		else
			Frame:Show();
		end
	end
end

-- 打开世界地图
local function OpenWorldMap(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		local Frame= getglobal( "WorldMapFrame" );
		if (Frame:IsShown()) then
			Frame:Hide();
		else
			Frame:Show();
		end
	end
end

-- 打开骑乘面板
local function OpenRideFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if press then
		ClickThreeShortCut( "RideShortcutButton" );
	end
end

-- 打开商城
local function OpenShopFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() or LoadingFrame:IsShown() then
		return;
	end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end
	if press then
		local Frame = getglobal("MarketManagerFrame");
		if(Frame:IsShown()) then
			Frame:Hide();
		else
			Frame:Show();
		end
	end
end


--打开宠物界面
local function OpenPetFrame(press)
	if getGameState() ~= STATE_INWORLD  or GameMgr:isInPlayCamerAnim() then
		return;
	end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		PetButtonOnClick();
	end
end

--打开队伍面板
local function OpenTeamFrame(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		TeamButtonOnClick();
	end
end

--打开自动辅助
local function OpenAutoAssistFrame( press )
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if press then
		AutoAssistFrame_OnClick();
	end
end
-- 隐藏/显示界面
bFirstChange = true;
local function HideAllUI(press)
	if press then
		if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
		if ShowChangeRoleFrame:IsShown() then
			return;
		end
		local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
		if not specialMove:canShowUI() then
			return
		end
		if bFirstChange then
			MessageBox("提示","你确定要隐藏界面吗？再次按\\键可重新显示界面");
			MessageBoxFrame:SetClientString( "隐藏界面" );
		else
			GameMgr:changeShowUI();
		end
	end
end

-- 切换敌对目标
local function SelectNearEnemy(press)
	if press then
		GameShortCut:usedTab()
	end
end

local function SelectNearMonster(press)
	if press then
		GameShortCut:usedTab( 1 )
	end
end

local function SelectNearPlayer(press)
	if press then
		GameShortCut:usedTab( 2 )
	end
end

-- 屏蔽/显示所有玩家
local function HideAllPlayer(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	if press then
		GameShortCut:usedF12();
	end
end

-- 隐藏/显示玩家名字
local function HideAllPlayerName(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	if press then
		GameShortCut:usedF11();
	end
end

local function HideAllMonsterName(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	if press then
		GameShortCut:usedF9();
	end
end

local function HideAllMonsterHP(press)
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	if press then
		GameShortCut:usedF10();
	end
end

function AccelKey_GMTool()
	if arg1 == "keydown" and getGameState() == CLIENT_STATE_INWORLD then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil or not mainplayer:isHaveGMPerm() then return end
		if GMToolsFrame:IsShown() then
			GMToolsFrame:Hide();
		else
			GMToolsFrame:Show();
		end
	end
end

local function Jump(press)
	if press then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer then
			mainplayer:jump()
		end
	end
end

local function CareerSkillMove(press)
	if press then
		GameShortCut:useSpecialSkill()
	end
end

local function MoveUp(press, code)
	if press then
		KeyMoveOpt:StartMove(KEYMOVE_UP, code)
	end
end

local function MoveDown(press, code)
	if press then
		KeyMoveOpt:StartMove(KEYMOVE_DOWN, code)
	end
end

local function MoveLeft(press, code)
	if press then
		KeyMoveOpt:StartMove(KEYMOVE_LEFT, code)
	end
end

local function MoveRight(press, code)
	if press then
		KeyMoveOpt:StartMove(KEYMOVE_RIGHT, code)
	end
end

local function CancelTmpItem(press)
	if press then
		local player = ActorMgr:getMainPlayer()
		if not player then
			return
		end
		if player:isOnMonsterMachine() then
			local oldthis = this
			local monsterMachine = player:getMonsterMachine()
			local monsterMachineInfo = monsterMachine:getMonMachineDef()
			if monsterMachineInfo and monsterMachineInfo.CollectType ~= 0 then
				this = ClanCollectTmpFrameDownBtn
			else
				this = ClanTankTmpFrameDownBtn
			end
			coroutine.wrap(DropMonsterMachine)()
			this = oldthis
		else
			local equip = player:getEquip()
			if equip:getTmpArm() ~= 0 or equip:getTmpMachin() ~= 0 then
				DropTempPropButton_OnClick()
			end
		end
	end
end

local function QuickAcceptQuest( press, code )
	if press then
		doQuickAcceptQuest();
	end
end

do
	local shortcutKeys = {
		{1, 9, "主快捷栏%d", 49},
		{10, 10, "主快捷栏10", 48},
		{13, 22, "扩展快捷栏%d", 112},
		{33, 42, "右1快捷栏%d"},
		{23, 32, "右2快捷栏%d"},
	}
	for _, t in ipairs(shortcutKeys) do
		for i = 0, t[2] - t[1] do
			local index = i + t[1]
			RegisterFunctionName("快捷栏按键", string.format(t[3], i + 1), function(press, code)
				if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then
					return
				end
				if ShowChangeRoleFrame:IsShown() then
					return
				end
				if press then
					GameShortCut:setShortCutKeyPress(index, code % 256)
					AccelKey_OnPress(index)
				else
					local btn = getglobal("MagicShortcut"..index)
					btn:DisChecked()
					GameShortCut:setShortCutKeyPress(index, 0)
				end
			end, t[4] and (t[4] + i))
		end
	end
end

do
	local shortcutKeys = {
		{43, 45, "精灵技能%d", GetControlKeyCode(false, false, true) + 49},
	}
	for _, t in ipairs(shortcutKeys) do
		do break end
		for i = 0, t[2] - t[1] do
			local index = i + t[1]
			RegisterFunctionName("快捷栏按键", string.format(t[3], i + 1), function(press, code)
				local btn = getglobal("XPBufShortcut"..(i + 1))
				if press then
					local oldArg1 = arg1
					arg1 = "LeftButton"
					local oldThis = this
					this = btn
					GameShortCut:setShortCutKeyPress(index, code % 256)
					btn:Checked()
					ShortcutOnClick()
					this = oldThis
					arg1 = oldArg1
				else
					btn:DisChecked()
					GameShortCut:setShortCutKeyPress(index, 0)
				end
			end, t[4] and (t[4] + i))
		end
	end
end
--打开金币交易面板
local  function OpenMarketTicketFrame( press )
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if press then
		TradeBtn_OnClick();
	end
end

--打开vip界面
local function OpenVipFrame( press )
	if getGameState() ~= STATE_INWORLD or GameMgr:isInPlayCamerAnim() then return end
	if ShowChangeRoleFrame:IsShown() then
		return;
	end
	local specialMove = ActorMgr:getMainPlayer():getSpecialMove()
	if not specialMove:canShowUI() then
		return
	end

	if press then
		PhotoFrameVipBtn_OnClick();
	end
end

RegisterFunctionName("常规按键", "向上移动", MoveUp, 87)
RegisterFunctionName("常规按键", "向下移动", MoveDown, 83)
RegisterFunctionName("常规按键", "向左移动", MoveLeft, 65)
RegisterFunctionName("常规按键", "向右移动", MoveRight, 68)
RegisterFunctionName("常规按键", "跳跃", Jump)
RegisterFunctionName("常规按键", "特殊移动", CareerSkillMove, 32)
RegisterFunctionName("常规按键", "打开背包", OpenBag, 66)
RegisterFunctionName("常规按键", "打开宠物界面", OpenPetFrame, 88)
RegisterFunctionName("常规按键", "打开队伍面板", OpenTeamFrame, 84)
RegisterFunctionName("常规按键", "打开公会界面", OpenClanFrame, 71)
RegisterFunctionName("常规按键", "打开好友面板", OpenFriendFrame, 70)
RegisterFunctionName("常规按键", "打开活动助手", OpenActiveHelperFrame, 72)
RegisterFunctionName("常规按键", "打开技能界面", OpenSkillFrame, 75)
--RegisterFunctionName("常规按键", "打开精灵面板", OpenXianFrame, 80)
RegisterFunctionName("常规按键", "打开科技生产", OpenSecondarySkillFrame, 69)
RegisterFunctionName("常规按键", "打开骑乘面板", OpenRideFrame, 78)
RegisterFunctionName("常规按键", "打开人物界面", OpenRoleFrame, 67)
RegisterFunctionName("常规按键", "打开金币交易面板", OpenMarketTicketFrame, 80)
RegisterFunctionName("常规按键", "打开任务日志", OpenTaskLogFrame, 81)
RegisterFunctionName("常规按键", "打开伤害统计", OpenDamageCountFrame, 82)
RegisterFunctionName("常规按键", "打开商城", OpenShopFrame, 86)
RegisterFunctionName("常规按键", "打开世界地图", OpenWorldMap, 77)
RegisterFunctionName("常规按键", "打开天赋面板", OpenDiathesisFrame, 76)
RegisterFunctionName("常规按键", "打开邮箱", OpenMailFrame, 89)
RegisterFunctionName("常规按键", "交易", StartExchange, 85)
RegisterFunctionName("常规按键", "切换PK模式", ChangePkMode, GetControlKeyCode(true, false, false)  + 65)
RegisterFunctionName("常规按键", "切换敌对目标", SelectNearEnemy, 9)
RegisterFunctionName("常规按键", "切换敌对怪物", SelectNearMonster, 9)
RegisterFunctionName("常规按键", "切换附近玩家", SelectNearPlayer, 9)
RegisterFunctionName("常规按键", "设置快捷发言", SetAccelSendMsgFrame, 1105)
RegisterFunctionName("常规按键", "拾取附近物品", PickItem, 90)
RegisterFunctionName("常规按键", "隐藏/显示界面", HideAllUI, 220)
RegisterFunctionName("常规按键", "隐藏/显示玩家名字", HideAllPlayerName, 122)
RegisterFunctionName("常规按键", "屏蔽/显示所有玩家", HideAllPlayer, 123)
--RegisterFunctionName("常规按键", "取消临时道具", CancelTmpItem, 192)
RegisterFunctionName("常规按键", "快速任务", QuickAcceptQuest, 192)
RegisterFunctionName("常规按键", "打开成长之路", OpenGrowUpFrame, 73)
RegisterFunctionName("常规按键", "打开能源面板", OpenSparFrame, 74)
RegisterFunctionName("常规按键", "打开自动辅助", OpenAutoAssistFrame, GetControlKeyCode(true, false, false)  + 90)
RegisterFunctionName("常规按键", "打开VIP界面", OpenVipFrame, 79)