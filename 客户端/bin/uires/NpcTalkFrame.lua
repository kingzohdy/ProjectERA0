--道具奖励类型：0 无物品奖励 1 选择一个 2 固定
local AWARD_TYPE_NONE	= 0;	--无物品奖励
local AWARD_TYPE_ONE	= 1;	--选择一个
local AWARD_TYPE_ALL	= 2;	--固定
local AWARD_TYPE_RAND	= 3;	--固定
local MAX_SELECTOPT		= 15;	--最大可视对话选项

MAX_AWARD_ITEM_NUM	= 12;
local nSelectAwardId = 0;

local share_task_clock = 0;
local share_task_duration = 20;

nViewStartPointRun	= 0;
t_TaskTraceColor	= {52,181,17};
t_TaskNofinishColor = {247,243,240};
t_TaskFaileColor = {184, 70, 70};
t_TaskTraceInfoList = {};
-- 

local t_NpcCommitItemFrameControl = { npcId = 0, list = -1, grid = -1 };

function GetNpcCommitItemFrameControl()
	return t_NpcCommitItemFrameControl;
end

t_NpcCommitItemFrameControl["update"] = 
function ( self )
	if self["npcId"] == 0 then
		return;
	end

	local frame = getglobal( "NpcCommitItemFrame" );
	
	local npc = ActorMgr:FindActor( self["npcId"] );
	if npc == nil then
		Close_NpcDialog( "NpcCommitItemFrame" );
		frame:Hide();
		return;
	end

	local nDist = util.CalActorsDistance( npc, ActorMgr:getMainPlayer() );
	if nDist > ( NpcDialog:getMaxNpcTalkDist( npc ) / 100 ) ^ 2 then
		Close_NpcDialog( "NpcCommitItemFrame" );
		frame:Hide();
	end
end

t_NpcCommitItemFrameControl["setItemInfo"] = 
function ( self, data )
	self["list"]	= data["list"];
	self["grid"]	= data["grid"];
end

t_NpcCommitItemFrameControl["clearItemInfo"] = 
function ( self )
	self["list"]	= -1;
	self["grid"]	= -1;
end

t_NpcCommitItemFrameControl["clearTalkInfo"] = 
function ( self )
	self["npcId"]	= 0;
	self["list"]	= -1;
	self["grid"]	= -1;
end

t_NpcCommitItemFrameControl["setTalkInfo"] = 
function ( self, data )
	self["npcId"] = data["npcId"];
end

t_NpcCommitItemFrameControl["getNpcID"] = 
function ( self )
	return self["npcId"];
end

local t_NpcCommitItemFrameEvents = 
{ ["GE_NPC_COMMIT"] = {}, ["GE_CLOSE_CLAN_DIALOG"] = {}, };

t_NpcCommitItemFrameEvents["GE_NPC_COMMIT"].func = 
function ()
	local Msg	= UIMSG:getUIMsg();
	local commitDialogInfo = Msg.NpcCommitDialog;
	if commitDialogInfo.CommitNum ~= 1 then
		return
	end
	local t_riches = { ["NpcCommitItemScrollFrameContentRich"] = 
	{ 
		value = ReplaceQuestDesc( commitDialogInfo.Answer ), 
		color = { r = 88, g = 73, b = 53 } }, 
	};
	util.SetRiches( t_riches );

	t_NpcCommitItemFrameControl:setTalkInfo( { npcId = commitDialogInfo.m_nNpcId } );
	local Frame= getglobal( "NpcCommitItemFrame" );
	if not Frame:IsShown() then
		Frame:Show();
	end
	PlayerContainerFrame:Show();
end

t_NpcCommitItemFrameEvents["GE_CLOSE_CLAN_DIALOG"].func = 
function ()
	NpcCommitItemFrame:Hide();
end

-- 提交物品对话框
function NpcCommitItemFrame_OnLoad()
	for event, _ in pairs( t_NpcCommitItemFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_NpcCommitItemFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_NpcCommitItemFrameEvents, t_NpcCommitItemFrameEvents );
end

function NpcCommitItemFrame_OnEvent()
	t_NpcCommitItemFrameEvents[arg1].func();
end

function NpcCommitItemFrame_OnShow()
	this:SetPoint("left", "$parent", "left", 0, 0 );
	UpdateNpcCommitItemFrame();
end

function UpdateNpcCommitItemFrame()
	local scrollFrame = getglobal( "NpcCommitItemScrollFrame" );
	scrollFrame:SetValueStep( 5 );
	scrollFrame:SetVerticalScroll( 0 );
	
	local rich		= getglobal( "NpcCommitItemScrollFrameContentRich" );
	local nHeight	= rich:GetTotalHeight();
	rich:SetHeight( nHeight / GetScreenScaleY() );
	local partionBtn = getglobal( "NpcCommitItemScrollFramePartionBtn" );
	partionBtn:SetPoint( "BOTTOMLEFT", rich:GetName(), "BOTTOMLEFT", 0, 20 );

	local itemBtn = getglobal( "NpcCommitItemScrollFrame_ItemBtn" );
	itemBtn:SetPoint( "center", partionBtn:GetName(), "center", 0, 35 );

	scrollFrame:ResetChildAnchor();
	SetScrollBar( "NpcCommitItemFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	NpcCommitItemFrame_ScrollBar:SetValue(0);
end

function NpcCommitItemFrame_OnUpdate()
	t_NpcCommitItemFrameControl:update();
end

function NpcCommitItemFrame_OnHide()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	-- 若已经有物品在上面
	if t_NpcCommitItemFrameControl["grid"] ~= -1 then
		local oldItem	= container:getItem( t_NpcCommitItemFrameControl["list"], t_NpcCommitItemFrameControl["grid"] );
		UnLockItem( t_NpcCommitItemFrameControl["list"], t_NpcCommitItemFrameControl["grid"], oldItem:getItemId() );
		local btn		= getglobal( "NpcCommitItemScrollFrame_ItemBtn" );
		btn:SetClientUserData( 0, 0 );
		local tex = getglobal( btn:GetName().."IconTexture" );
		tex:SetTexture( [[uires\ZhuJieMian\1.dds]] );
		tex:SetTexUV( 749, 472, 44, 44 );
	end

	t_NpcCommitItemFrameControl:clearTalkInfo();
end

-- 滚动条移动
function NpcCommitItemFrame_ScrollBar_OnValueChanged()
	local scrollFrame	= getglobal( "NpcCommitItemScrollFrame" );
	local nValue		= this:GetValue();
	local nMaxValue		= this:GetMaxValue();
	local nLastValue	= this:GetLastValue();	
	local nStep			= this:GetValueStep();
	
	if nValue <= nMaxValue then
		scrollFrame:IncreaseVerticalScroll( nValue - nLastValue );
	end
end

function NpcCommitItemFrame_OnVerticalScroll()
end

function NpcCommitItemFrameOnScrollRangeChanged()
	SetScrollBar( "NpcCommitItemFrame_ScrollBar", 0, this:GetVerticalScrollRange() );
end

function Save2NpcCommitItemScrollFrame( item )
	local itemdef		= item:getItemDef();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	-- 若已经有物品在上面
	if t_NpcCommitItemFrameControl["grid"] ~= -1 then
		local oldItem	= container:getItem( t_NpcCommitItemFrameControl["list"], t_NpcCommitItemFrameControl["grid"] );
		UnLockItem( t_NpcCommitItemFrameControl["list"], t_NpcCommitItemFrameControl["grid"], oldItem:getItemId() );
		t_NpcCommitItemFrameControl:clearItemInfo();
	end

	t_NpcCommitItemFrameControl:setItemInfo( { grid = item:getGridIdx(), list = item:getListType() } );
	LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "NpcCommitItemFrame" );

	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;

	local szPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	if not IsInExistence( szPath ) then
		szPath = DEFAULT_ITEM_PATH;
	end
	
	local btn = getglobal( "NpcCommitItemScrollFrame_ItemBtn" );
	local t_tgas = { [btn:GetName().."IconTexture"] = { path = szPath }, };
	util.SetTgas( t_tgas );
	btn:SetClientUserData( 0, item:getItemId() );
	btn:SetClientUserData( 1, item:getListType() );
	btn:SetClientUserData( 2, item:getGridIdx() );
end

-- 面板的物品按钮
function NpcCommitItemScrollFrame_ItemBtn_Drag()
end

function NpcCommitItemScrollFrame_ItemBtn_ReceiveDrag()
	local srcList = UIMSG:getUIGV().DragFrom;
	if srcList ~= CONTAINER_TYPE_SUNDRIES and srcList ~= CONTAINER_TYPE_STUFF then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	local srcGrid		= UIGetDragUserData1();
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item			= container:getItem( srcList, srcGrid );
	Save2NpcCommitItemScrollFrame( item );
end

-- 放公会任务物品
function NpcCommitItemScrollFrame_ItemBtnEnterFunc( szBtnName )
	local btn		= getglobal( szBtnName );
	local nItemID	= btn:GetClientUserData( 0 );
	local MainPlayer= ActorMgr:getMainPlayer();
	local container	= MainPlayer:getContainer();
	--如果是根据id来显示tips，永远都是显示背包找到的第一个相同id的物品来显示，有可能出错，修改下
	--local item		= container:getItem( nItemID );
	local item		= container:getItem( btn:GetClientUserData( 1 ), btn:GetClientUserData( 2 ) );
	if item == nil then
		item		= container:getItem( nItemID );
	end
	if item:getItemId() ~= 0 then
		--不是很明白使用PetPhotoItemBtnEnterFunc的用意，暂且注释，替换为下边代码
		--ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, PetPhotoItemBtnEnterFunc, szBtnName );
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, nil, szBtnName );
	end
end

function NpcCommitItemScrollFrame_ItemBtn_OnEnter()
	local nItemID = this:GetClientUserData( 0 );
	if nItemID == 0 then
		-- 考虑是否给个提示
		return;
	end

	NpcCommitItemScrollFrame_ItemBtnEnterFunc( this:GetName() );
end

function NpcCommitItemScrollFrame_ItemBtn_OnLeave()
	local t_hideIcons = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame", "SpellTipsFrame" };
	util.HideIcons( t_hideIcons );
end

function NpcCommitItemScrollFrame_ItemBtn_OnClick()
	local nItemID = this:GetClientUserData( 0 );
	if nItemID == 0 then
		return;
	end

	if arg1 ~= "RightButton" then
		return;
	end

	this:SetClientUserData( 0, 0 );
	UnLockItem( t_NpcCommitItemFrameControl["list"], t_NpcCommitItemFrameControl["grid"], nItemID );
	t_NpcCommitItemFrameControl:clearItemInfo();

	local t_textures = 
	{
		[this:GetName().."IconTexture"] = 
		{	
			path = [[uires\\ZhuJieMian\\1.dds]], UV = { x = 749, y = 472, width = 44, height = 44 },
		},
	};
	util.SetTextures( t_textures );
end

-- 确定提交按钮
function NpcCommitItemFrame_Confirm_OnClick()
	local btn		= getglobal( "NpcCommitItemScrollFrame_ItemBtn" );
	local nItemID	= btn:GetClientUserData( 0 );
	if nItemID == 0 then
		ShowMidTips( "请放入物品" );
		return;
	end
	
	NpcDialog:commitTaskItem( t_NpcCommitItemFrameControl["list"], t_NpcCommitItemFrameControl["grid"] );
end

-- 提交物品对话框

local t_NpcOpt = {
					["m_szSentence"]	= "",
					["m_nAllOptNum"]	= 0,
					["m_allOpts"]		= {},	-- m_allOpts = { text = , Type = }
					["m_lastOptNum"]	= 0,
				 };

function InitNpcTalkFrame()
end

function NpcTalkFrame_Test_OnShow()
	local btn				= getglobal( "NpcTalkFrameXinShouBtn" );
	local rich				= getglobal( btn:GetName().."Rich" );
	local animationTexture	= getglobal( btn:GetName().."UVAnimationTex" );
	local szText = "测试NPC对话面板的效果";
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), btn:GetName(), nSelfDefineExtendWidth );
	animationTexture:SetUVAnimation( 20, true );
	btn:Show();
end

local t_NpcTalkFrameEvents = 
{	
	-- 切换目标对象
	["GE_NPC_ANSWER"] = {},

	["GE_NPC_ANSWER_END"]	= {},

	-- 有选项框的NPC对话
	["GE_NPC_OPT"]	= {},

	["GE_NPC_LAST"]	= {},
	-- 未完成任务提示框
	["GE_QUEST_DIALOG_NOFINI"]	= {},
	-- 任务接取框
	["GE_QUEST_DIALOG_ACCEPT"]	= {},
	-- 领取任务奖励框
	["GE_QUEST_DIALOG_AWARD"]	= {},
	-- 任务接取提示框
	["GE_QUEST_DIALOG_ACCEPTTIP"] = {},
	-- 获取角色任务信息
	["GE_QUEST_GETQUESTINFO"]	= {},
	-- 任务变化
	["GE_QUEST_CHANGE"]	= {},
	
	-- 删除任务追踪
	["GE_QUEST_DELQUESTTRACE"]	= {},
	-- 添加任务追踪
	["GE_QUEST_ADDQUESTTRACE"]	= {},
	-- 删除已接取任务
	["GE_QUEST_DELQUESTRUN"]	= {},
	-- 背包有变化
	["GE_CONTAINER_UPDATE"]	= {},

	["GE_QUEST_TIMECHANGE"]	= {},

	["GE_QUEST_ITEM_CHANGE"] = {},
};

-- 切换目标对象
t_NpcTalkFrameEvents["GE_NPC_ANSWER"].func = 
function ()
	local Frame= getglobal( "NpcTalkFrame" );
	Frame:Show();
	local Msg = UIMSG:getUIMsg();
	NpcTalkContentRich:SetFontType( FONTSYTLE_NORMAL );
	NpcTalkContentRich:SetText( ReplaceQuestDesc( Msg.NpcDialog.m_szAnswer ), 88,73,53);
end

-- 结束对话
t_NpcTalkFrameEvents["GE_NPC_ANSWER_END"].func = 
function ()
	local Frame= getglobal( "NpcTalkFrame" );
	if Frame:IsShown() then
		Frame:Hide();
		NpcDialog:send_CloseTalk();
	end
end

-- 有选项框的NPC对话
t_NpcTalkFrameEvents["GE_NPC_OPT"].func = 
function ()
	-- 清空NPC对话选项数据表
	t_NpcOpt = {
				["m_szSentence"]	= "",
				["m_nAllOptNum"]	= 0,
				["m_allOpts"]		= {},
				["m_lastOptNum"]	= 0,
			   };
	local Msg					= UIMSG:getUIMsg();
	local npcOptData			= Msg.NpcOpt;
	local npc					= ActorMgr:FindActor( npcOptData.m_nNpcId );
	t_NpcOpt["m_npcResId"]		= npc:getResId();
	t_NpcOpt["m_szSentence"]	= npcOptData.m_szSentence;
	t_NpcOpt["m_nAllOptNum"]	= npcOptData.m_optTal;	
	t_NpcOpt["m_lastOptNum"]	= npcOptData.m_lastOptNum;

	for i = 1, npcOptData.m_optTal do	
		local oneOptData = Msg.NpcOpt.m_OneOpt[i-1];
		table.insert( t_NpcOpt.m_allOpts, { oneOptContent = oneOptData.Opt, optIndex = i, optType = oneOptData.m_nType } );
	end

	--t_NpcOpt.m_allOpts = SortNpcTalkOpt( t_NpcOpt.m_allOpts );

	UpdateNpcDialogInfo();
	local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
	t_NpcTalkFrameControl:setTalkInfo( { npcId = Msg.QuestTalk.m_nNpcId } );
	local Frame= getglobal( "NpcTalkFrame" );
	if not Frame:IsShown() then
		Frame:Show();
	end
end

-- 
t_NpcTalkFrameEvents["GE_NPC_LAST"].func = 
function ()
	local Msg = UIMSG:getUIMsg();
	NpcTalkText:SetFontType(FONTSYTLE_NORMAL);
	NpcTalkContentRich:SetText( ReplaceQuestDesc( Msg.NpcLastDialog.DlgLast ), 88,73,53 );
	local Frame= getglobal( "NpcTalkFrame" );
	Frame:Show();
end

-- 未完成任务提示框
t_NpcTalkFrameEvents["GE_QUEST_DIALOG_NOFINI"].func = 
function ()
	local talkFrame = getglobal( "NpcTalkFrame" );
	if talkFrame:IsShown() then
		talkFrame:Hide();
	end

	local frame		= getglobal("TaskNotFinishFrame");
	local Msg		= UIMSG:getUIMsg();
	local nNpcId	= Msg.QuestTalk.m_nNpcId;
	local nQuestId	= Msg.QuestTalk.m_nQuestId;
	local szDes		= Msg.QuestTalk.DlgLast;
	local nRecycleID= Msg.QuestTalk.m_nRecycleID;

	frame:SetClientUserData( 0, nNpcId );
	frame:SetClientUserData( 1, nQuestId );
	frame:SetClientUserData( 2, nRecycleID );
	frame:SetClientString(szDes);
	frame:Show();
end

-- 任务接取框
t_NpcTalkFrameEvents["GE_QUEST_DIALOG_ACCEPT"].func = 
function ()
	local talkFrame = getglobal( "NpcTalkFrame" );
	local t_TaskAcceptFrameControl = GetTaskAcceptFrameControl();
	if talkFrame:IsShown() then
		t_TaskAcceptFrameControl:clearTalkInfo();
		talkFrame:Hide();
	end

	local frame		= getglobal("TaskAcceptFrame");
	local Msg		= UIMSG:getUIMsg();
	local nNpcId	= Msg.QuestTalk.m_nNpcId;
	local nQuestId	= Msg.QuestTalk.m_nQuestId;
	local szDes		= Msg.QuestTalk.DlgLast;
	local nRecycleID= Msg.QuestTalk.m_nRecycleID;
	frame:SetClientUserData( 0, nNpcId );
	frame:SetClientUserData( 1, nQuestId );
	frame:SetClientUserData( 2, nRecycleID );
	frame:SetClientString( szDes );

	if ( string.find(szDes, "#next#", 1, true) ) then
		local nPos = ( string.find(szDes, "#next#") );
		local nLength = string.len( szDes );
		NpcDialogFrame:SetClientString( string.sub(szDes, 1, nPos-1 ) );
		frame:SetClientString( string.sub(szDes, nPos+6, nLength ) );
		NpcDialogFrame:Show();
		return;
	end
	
	t_TaskAcceptFrameControl:setTalkInfo( { npcId = nNpcId, taskId = Msg.QuestTalk.m_nQuestId,
											taskType = ( ( nNpcId == 0 ) and SHARE_TASK_TYPE or NORMAL_TASK_TYPE ) } );

	frame:Show();
end

-- 领取任务奖励框
t_NpcTalkFrameEvents["GE_QUEST_DIALOG_AWARD"].func = 
function ()
	local talkFrame = getglobal( "NpcTalkFrame" );
	if talkFrame:IsShown() then
		talkFrame:Hide();
	end

	local frame		= getglobal("TaskAwardFrame");
	local Msg		= UIMSG:getUIMsg();
	local nNpcId	= Msg.QuestTalk.m_nNpcId;
	local nQuestId	= Msg.QuestTalk.m_nQuestId;
	local szDes		= Msg.QuestTalk.DlgLast;
	local nRecycleID= Msg.QuestTalk.m_nRecycleID;

	frame:SetClientUserData( 0, nNpcId );
	frame:SetClientUserData( 1, nQuestId );
	frame:SetClientUserData( 2, nRecycleID );
	frame:SetClientString( szDes );
	frame:Show();
end

-- 任务接取提示框
t_NpcTalkFrameEvents["GE_QUEST_DIALOG_ACCEPTTIP"].func = 
function ()
end

-- 获取角色任务信息
t_NpcTalkFrameEvents["GE_QUEST_GETQUESTINFO"].func = 
function ()
	SetQuestRunList();
	SetCanAcceptQuestList();

	SetTaskTraceInfoList();
	if not GameMgr:isInPlayCamerAnim() then
		UpdateTaskTraceInfo( t_TaskTraceInfoList );
	end
end

-- 任务变化
t_NpcTalkFrameEvents["GE_QUEST_CHANGE"].func = 
function ()
	-- 刷新可新接任务
	Quest:updateCanAcceptQuest();
	SetQuestRunList();
	SetCanAcceptQuestList();
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
	RefreshQuestDesc();
	UpdateTaskInfoDis( nViewStartPointRun, "" );
	
	local curChgTaskInfo		= Quest:getChgTaskInfo();
	local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
	if NpcTalkFrame:IsShown() then
		local nNpcMemID = t_NpcTalkFrameControl:getNpcID();
		if nNpcMemID == 0 or not t_NpcTalkFrameControl:isTaskInTalk( { ["taskID"] = curChgTaskInfo.m_nTaskID } )then
			return;
		end

		--print( "nNpcMemID = "..nNpcMemID );
		local npc		= ActorMgr:FindActor( nNpcMemID );
		NpcDialog:send_CloseTalk();
		NpcDialog:talkByResId( npc:getResId() );
	end
end

-- 删除任务追踪
t_NpcTalkFrameEvents["GE_QUEST_DELQUESTTRACE"].func = 
function ()
	--删除任务追踪
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

-- 添加任务追踪
t_NpcTalkFrameEvents["GE_QUEST_ADDQUESTTRACE"].func = 
function ()
	--添加任务追踪
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

-- 删除已接取任务
t_NpcTalkFrameEvents["GE_QUEST_DELQUESTRUN"].func = 
function ()
	--删除已接取任务
	SetQuestRunList();
	SetCanAcceptQuestList();
	UpdateTaskInfoDis( nViewStartPointRun, "" );
end

-- 背包有变化
t_NpcTalkFrameEvents["GE_CONTAINER_UPDATE"].func = 
function ()
	--背包有变化
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

-- 任务时间变化导致任务属性变化
t_NpcTalkFrameEvents["GE_QUEST_TIMECHANGE"].func = 
function ()
	--任务时间变化
	SetTaskTraceInfoList();
	BindTaskTraceInfo( t_TaskTraceInfoList );	   
end

t_NpcTalkFrameEvents["GE_QUEST_ITEM_CHANGE"].func = 
function ()
	--任务物品变化
	SetTaskTraceInfoList();
	BindTaskTraceInfo( t_TaskTraceInfoList );	   
end

local t_NpcTalkFrameControl = { npcId = 0, ["taskOpts"]	= {}, };
function GetNpcTalkFrameControl()
	return t_NpcTalkFrameControl;
end

t_NpcTalkFrameControl["update"] = 
function ( self )
	if self["npcId"] == 0 then
		return;
	end

	local frame = getglobal( "NpcTalkFrame" );
	
	local npc = ActorMgr:FindActor( self["npcId"] );
	if npc == nil then
		Close_NpcDialog( "NpcTalkFrame" );
		frame:Hide();
		return;
	end

	local nDist = util.CalActorsDistance( npc, ActorMgr:getMainPlayer() );
	if nDist > ( NpcDialog:getMaxNpcTalkDist( npc ) / 100 ) ^ 2 then
		Close_NpcDialog( "NpcTalkFrame" );
		frame:Hide();
	end
end

t_NpcTalkFrameControl["isTalkOpt"] = 
function ( self, data )
	local oneOptDesc = data["text"];
	if ( string.find( oneOptDesc, "(可接)", 1, true ) ) then
		return true;
	end

	if ( string.find( oneOptDesc, "(不可接)", 1, true ) ) then
		return true;
	end
	
	if ( string.find( oneOptDesc, "(完成)", 1, true ) ) then
		return true;
	end

	if ( string.find( oneOptDesc, "(未完成)", 1, true ) ) then
		return true;
	end

	return false;
end

t_NpcTalkFrameControl["getTaskOptInfo"] = 
function ( self, data )
	local oneOptDesc = data["text"];
	local szTaskName	= "";
	for i = string.len( oneOptDesc ), 1, -1 do
		local oneChar = string.sub( oneOptDesc, i, i );
		if oneChar ==']' or oneChar == ")" or oneChar == " " then
			szTaskName = string.sub( oneOptDesc, i + 1 );
			break;
		end
	end

	if ( string.find( oneOptDesc, "(可接)", 1, true ) ) then
		return { state = CLIENT_TASK_STAT_CANACCEPT, taskName = szTaskName };
	end

	if ( string.find( oneOptDesc, "(不可接)", 1, true ) ) then
		return { state = CLIENT_TASK_STAT_CANNOTACCEPT, taskName = szTaskName };
	end
	
	if ( string.find( oneOptDesc, "(完成)", 1, true ) ) then
		return { state = CLIENT_TASK_STAT_FINI, taskName = szTaskName };
	end

	if ( string.find( oneOptDesc, "(未完成)", 1, true ) ) then
		return { state = CLIENT_TASK_STAT_RUN, taskName = szTaskName };
	end
end

t_NpcTalkFrameControl["addOneTalkInfo"] =
function ( self, data )
	table.insert( self["taskOpts"], { state = data["state"], taskName = data["taskName"], } );
end

t_NpcTalkFrameControl["setTalkInfo"] = 
function ( self, data )
	self["npcId"]		= data["npcId"];
	local Msg			= UIMSG:getUIMsg();
	local npcOptData	= Msg.NpcOpt;
	for i = 1, npcOptData.m_optTal do
		local oneOptData = npcOptData.m_OneOpt[i-1];
		if self:isTalkOpt( { ["text"] = oneOptData.Opt } ) then
			local oneTaskInfo = self:getTaskOptInfo( { ["text"] = oneOptData.Opt } );
			self:addOneTalkInfo( oneTaskInfo );
		end
	end
end

t_NpcTalkFrameControl["isTaskInTalk"] = 
function ( self, data )
	local nTaskID		= data["taskID"];
	local szTaskName	= Quest:getQuestName( nTaskID );
	for _, oneTaskData in ipairs( self["taskOpts"] ) do
		if szTaskName == oneTaskData["taskName"] then
			return true;
		end
	end

	return false;
end

t_NpcTalkFrameControl["getNpcID"] = 
function ( self )
	return self["npcId"];
end

t_NpcTalkFrameControl["init"] = 
function ( self )
	self:clearTalkInfo();
end

t_NpcTalkFrameControl["clearTalkInfo"] = 
function ( self )
	self["npcId"]		= 0;
	self["taskOpts"]	= {}
end

t_NpcTalkFrameControl["getAllOpts"] = 
function ( self )
	return t_NpcOpt.m_allOpts;
end

function NpcTalkFrame_OnLoad()
	for event, _ in pairs( t_NpcTalkFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_NpcTalkFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_NpcTalkFrameEvents, t_NpcTalkFrameEvents );
	
end

--任务已完成
function QusetFinish( title, taskID )
	if CanShowTaskMidMsg() then
		ShowMidTips( "任务["..title.."]已完成" );
	end

	if taskID == 1053 and not GameWizard:isDo( GAMEWIZARD_FIRST_OPENSHOP ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_OPENSHOP);
		GameWizard:doStep( GAMEWIZARD_FIRST_OPEN_BUY_CONFIRM);
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_COLLECT ) then
		local t_taskDelActorPop = 
		{
		-- 获得第一次采集任务
		-- taskId  
		[1055] = { 293003, },
		[1059] = { 293002, },
		[1070] = { 293000, },
		[1078] = { 293001, },
		};
	
		local t_chgActorShowEvent = t_taskDelActorPop[taskID];
		if t_chgActorShowEvent ~= nil then
			for _, resId in ipairs( t_chgActorShowEvent ) do
				GameWizard:delGroupActorsByResId( resId );
				if taskID == 1078 then
					GameWizard:doStep( GAMEWIZARD_FIRST_COLLECT );
				end
			end
		end
	end

	if taskID == 1081 and not GameWizard:isDo( GAMEWIZARD_FIRST_REPAIR ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_REPAIR );
	end
end

local t_TaskState =
	{
--		任务				对应的图标UV    偏移
		{"(不可接)",	739,594,9,21,	-10,-5},
		{"(可接)",		739,500,9,21,	-3,-8},	
		{"(未完成)",	213,204,14,21,  -3,-8},
		{"(完成)",		213,182,14,21,  -3,-8},
		{"(已接)",		739,594,9,21,	-3,-8},	--TODO HERE
		{"(买卖)",		727,719,21,21,  -3,-8},
		{"(修理)",		986,853,21,21,  -3,-8},
		{"(默认)",		171,301,22,20,  -3,-8},	 
		{"(环次)",		273,110,17,23,  -10,0},
		{"(离开)",		171,301,22,20,  -10,0},
		{"(福利)",		929,653,22,20,  -10,0},
		
	};

function GetTaskState()
	return t_TaskState;
end

function TaskNotFinishFrame_OnHide()
	AutoHideNomalTips("TaskNotFinishFrame");
	NpcDialog:send_CloseTalk();
end

function TaskAwardFrame_OnHide()
	local t_hideIcons = { "TaskAwardFrame_GetBtnXinShouBtn" };
	util.HideIcons( t_hideIcons );
	
	AutoHideNomalTips("TaskAwardFrame");
	NpcDialog:send_CloseTalk();
end

function SetNpcDialogBtn( data )
end

function GetTaskState( data )
	for i = 1, table.getn( t_TaskState ) do
		if ( string.find( data["text"], t_TaskState[i][1], 1, true ) ) then
			return t_TaskState[i][1];
		end
	end
	
	return "";
end

-- 设置NPC对话的具体内容
function ClearNpcDialogInfo()
	for i = 1, MAX_SELECTOPT do
		local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
		selectButton:Hide();
		selectButton:DisHighlight();
	end
	for i =1, 6 do
		local tex = getglobal( "NpcDialogScrollFramePartionBtnLinePartionTex" .. i )
		tex:Hide();
	end
end

--判断一个选项是否是任务
local t_TaskSelectText = {"(不可接)","(可接)","(未完成)","(完成)","(已接)"};
function IsQuestSelectText( szText )
	for i=1,table.getn( t_TaskSelectText ) do
		if string.find( szText,t_TaskSelectText[i],1,true ) then
			return true;
		end
	end
	return false;
end

function UpdateNpcDialogInfo()
	ClearNpcDialogInfo();
	--设置NPC对话面板的NPC名字、头像及对话内容
	local mainplayer	= ActorMgr:getMainPlayer();
	local TargetObj		= ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	-- TODO: 去掉这步，当距离NPC过远的时候不能打开NPC对话面板
	--if TargetObj == nil then
	--	return;
	--end
	
	local scrollFrame = getglobal( "NpcDialogScrollFrame" );
	scrollFrame:SetValueStep( 5 );
	scrollFrame:SetVerticalScroll( 0 );
	
	if TargetObj then
		local t_fonts = { ["NpcNameFont"] = { value = TargetObj:getName() }, };
		util.SetFonts( t_fonts );
	end
	
	local t_riches = { ["NpcTalkContentRich"] = { 
							value = ReplaceQuestDesc( t_NpcOpt.m_szSentence ), 
							color = { r = 88, g = 73, b = 53 } }
						};
	util.SetRiches( t_riches );

	local nHeight = NpcTalkContentRich:GetTotalHeight();
	NpcTalkContentRich:SetHeight( nHeight / GetScreenScaleY() );
	NpcTalkContentRich:SetFontType(FONTSYTLE_NORMAL);

	NpcDialogScrollFramePartionBtn:SetPoint("BOTTOMLEFT", "NpcTalkContentRich", "BOTTOMLEFT", 0, 20);

	local nIndex = 1;
	local nHeight = -25;
	local nPartionIdx = 1;
	--local t_TypeDesc = { [NPC_DIALOG_LAST_OPT] = "NPC_DIALOG_LAST_OPT", [NPC_DIALOG_NORMAL_OPT] = "NPC_DIALOG_NORMAL_OPT" };
	for i = 1, t_NpcOpt.m_nAllOptNum do
		local t_oneOpt = t_NpcOpt.m_allOpts[i];
		if nIndex > MAX_SELECTOPT or t_oneOpt["optType"] == NPC_DIALOG_LEAVE_OPT then
			break;
		end
		--print( "i = "..i.."， content = "..t_oneOpt["oneOptContent"].."npcOptType = "..t_TypeDesc[t_oneOpt["optType"]] );

		local selectButton		= getglobal( "NpcTalkSelectSelectItem"..nIndex );
		local selectOptFont		= getglobal( selectButton:GetName().."SelectFont" );
		local selectTexture		= getglobal( selectButton:GetName().."SelectTex" );
		selectButton:Show();

		if i ~= 1 and IsQuestSelectText(t_oneOpt["oneOptContent"]) ~= IsQuestSelectText(t_NpcOpt.m_allOpts[i-1]["oneOptContent"]) then			
			local tex = getglobal( "NpcDialogScrollFramePartionBtnLinePartionTex" .. nPartionIdx );
			if tex ~= nil then
				tex:Show();
				tex:SetPoint( "TOPLEFT", "NpcDialogScrollFramePartionBtn", "TOPLEFT", 0, nHeight + 52 );
			end
			nHeight = nHeight + 35 + 30;
			nPartionIdx = nPartionIdx + 1;
		else
			nHeight = nHeight + 35 ;			
		end
		selectButton:SetPoint( "TOPLEFT", "NpcDialogScrollFramePartionBtn", "TOPLEFT", 0, nHeight );
		
		selectTexture:SetTexUV( 171, 301, 22, 20);
		selectTexture:SetSize( 22,20 );	 
		selectButton:SetClientUserData( 2,-1);
		local szText = t_oneOpt["oneOptContent"];
		selectButton:SetClientString( szText );
		--print( "szText = "..szText );
		for j = 1, table.getn( t_TaskState ) do
			if ( string.find(szText,t_TaskState[j][1], 1, true) ) then
				local nBeginPos = string.find(szText,")");
				local nLength	= string.len( szText );
				szText = string.sub(szText, nBeginPos + 1, nLength );
				selectTexture:SetTexUV( t_TaskState[j][2], t_TaskState[j][3], t_TaskState[j][4], t_TaskState[j][5] );
				selectTexture:SetSize( t_TaskState[j][4], t_TaskState[j][5] );
				--print( "j = "..j );
				selectButton:SetClientUserData( 2,j);
				break;
			end
		end

		selectTexture:Show();
		-- 设置选项显示的文本
		selectOptFont:SetText( szText );
		selectButton:SetClientUserData( 0, t_oneOpt["optType"] );
		selectButton:SetClientUserData( 1, t_oneOpt["optIndex"] );	
		nIndex = nIndex + 1;
	end
	
	local nLastBeginIndex = nIndex;
	for i = nIndex, t_NpcOpt.m_nAllOptNum do
		local t_oneOpt = t_NpcOpt.m_allOpts[i];
		if nIndex > MAX_SELECTOPT then
			break;
		end

		local selectButton		= getglobal( "NpcTalkSelectSelectItem"..nIndex );
		local selectOptFont		= getglobal( selectButton:GetName().."SelectFont" );
		local selectTexture		= getglobal( selectButton:GetName().."SelectTex" );
		selectButton:Show();
		-- 若只有last选项，则没有必要多向下偏移一段距离
		if nLastBeginIndex == 1 then
			selectButton:SetPoint("TOPLEFT", "NpcDialogScrollFramePartionBtn", "TOPLEFT", 0, (nIndex-1)*35+ 10 );
		else
			selectButton:SetPoint("TOPLEFT", "NpcDialogScrollFramePartionBtn", "TOPLEFT", 0, (nIndex-1)*35+ 20 );
		end
		
		selectTexture:SetTexUV( 171, 301, 22, 20);
		selectTexture:SetSize(22,20);	 
		local szText = t_oneOpt["oneOptContent"];
		selectButton:SetClientString( szText );
		selectButton:SetClientUserData( 2,-1);
		
		for j = 1, table.getn( t_TaskState ) do
			if ( string.find(szText,t_TaskState[j][1], 1, true) ) then
				local nBeginPos = string.find(szText,")");
				local nLength	= string.len( szText );
				szText = string.sub(szText, nBeginPos + 1, nLength );
				selectTexture:SetTexUV( t_TaskState[j][2], t_TaskState[j][3], t_TaskState[j][4], t_TaskState[j][5]);
				selectTexture:SetSize(t_TaskState[j][4], t_TaskState[j][5]);
				selectButton:SetClientUserData( 2,j);
				break;
			end
		end

		selectTexture:Show();
		-- 设置选项显示的文本
		selectOptFont:SetText( szText );
		selectButton:SetClientUserData( 0, t_oneOpt["optType"] );
		selectButton:SetClientUserData( 1, t_oneOpt["optIndex"] );	 
		nIndex = nIndex + 1;
	end

	scrollFrame:ResetChildAnchor();
	SetScrollBar( "NpcDialogFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	NpcDialogFrame_ScrollBar:SetValue(0);

	CheckNpcTalkXinShouGuid( t_NpcOpt.m_npcResId );
end

function IsSelfOpt( szText )
	for _, data in ipairs( t_TaskState ) do
		local szDesc = data[1];
		if ( string.find( szText, szDesc, 1, true ) ) then
			return true;
		end
	end

	return false;
end

function SortNpcTalkOpt( t_allNpcOpt )
	local t_FiniOpt		= {};
	local t_OtherOpt	= {};
	local t_lastOpt		= {};
	local t_noKeyWordOpt= {};
	for i = 1, table.getn( t_allNpcOpt ) do
		local t_oneOpt = t_allNpcOpt[i];
		if ( string.find( t_oneOpt["oneOptContent"], "(完成)", 1, true ) ) then
			table.insert( t_FiniOpt, t_oneOpt );

		elseif ( string.find( t_oneOpt["oneOptContent"], "(离开)", 1, true ) ) then
			t_oneOpt["optType"] = NPC_DIALOG_LEAVE_OPT;
			table.insert( t_lastOpt, t_oneOpt );

		elseif not IsSelfOpt( t_oneOpt["oneOptContent"] ) then
			table.insert( t_noKeyWordOpt, t_oneOpt );
		
		else
			table.insert( t_OtherOpt, t_oneOpt );
		end
	end

	local t_notLastTable = MergeTables( t_FiniOpt, t_OtherOpt );
	t_notLastTable = MergeTables( t_notLastTable, t_noKeyWordOpt );
	return MergeTables( t_notLastTable, t_lastOpt );
end

function NpcTalkFrame_OnEvent()	
	t_NpcTalkFrameEvents[arg1].func();
end

--@Desc:创建公会
function ShowCreatClanFrame( nActorId )
	-- 判断是否能够创建工会 
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if mainplayer:getLv() < MIN_LVEL_CREATE_CLAN then
		ShowMidTips( "等级小于30级，无法创建公会" );
		return;
	end

	local clanLvDef = getClanLevelDef( 0 );
	local nNeedMoney = MIN_BUUL_CREATE_CLAN
	if clanLvDef ~= nil then nNeedMoney = clanLvDef.Required.Money end
	if getPlayerMoney( mainplayer ) < nNeedMoney then
		ShowMidTips( "拥有的"..MONEY_TYPE_BULL_NAME.."小于"..nNeedMoney.."，无法创建公会" );
		return;
	end

	local ClanViewer = GameClanManager:getClanViewer();
	if mainplayer:isInClan() then
		ShowMidTips( "你已经加入公会，无法创建公会" );
		return;
	end

	Clan_CreateInputFrame:Show();
	Clan_CreateInputFrame:SetClientString("创建公会");
	Clan_CreateInputFrame:SetClientUserData( 0, nNeedMoney );
	Clan_CreateInputFrameCostInfoFont:SetText( "创建公会需消耗" .. nNeedMoney .. MONEY_TYPE_BULL_NAME )
	Clan_CreateInputFrameNoticeInfo:SetText( "请输入公会名字！" );
	SetFocusFrame( "Clan_InputFrame_Input_SetNameEditBox" );
	util.testNpcDialogDistance( nActorId, Clan_CreateInputFrame );
end

--@Desc:解散公会
function DistoryClanFrame()
	return;
end

-- 公会解散面板
function DisBindClan( npcId )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getClanPosition() == CLAN_POS_PRESIDENT  then
		MessageBox( "解散公会", "确认要解散公会？" )
		util.testNpcDialogDistance( npcId, MessageBoxFrame );
	end
end

-- 公会宣告面板
function ManifestoClan( nActorId )
	local mainplayer = ActorMgr:getMainPlayer();
	Clan_EditFrame:Show();
	ClanEditFrameNoticeInfo:SetText( "请输入新的宣告！" );
	Clan_EditFrame:SetClientString("修改宣告");
	Clan_EditFrame:AddLevelRecursive(1);
	util.testNpcDialogDistance( nActorId, Clan_EditFrame );
end

-- 装备品质提升
function ArmProceArmStar( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmProceStarFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmProceStarFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmProceStarFrame );
end

function ArmProceArmBind( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmProceBindFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmProceBindFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmProceBindFrame );
end

function ArmProceArmUnbind( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmProceUnBindFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmProceUnBindFrame:Hide();
	ArmProceUnBindFrame:SetClientUserData( 0,0 );
	ArmProceUnBindFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmProceUnBindFrame );
end

-- 装备战斗力提升
function ArmProceArmFight()
end

-- 装备等级提升
function ArmProceArmLevel()
end

-- 装备鉴定
function ArmProceArmIdentifiability()
end

-- 宝石升级
function ArmProceArmGemLevelup( nNpcActorId )
end

-- 宝石镶嵌
function ArmProceArmGemInLay( nNpcActorId )
end

-- 宝石摘取
function ArmProceArmGemPickup( nNpcActorId )
end

--品质转移
function ArmProceMoveStar( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmProceStarMoveFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmProceStarMoveFrame:SetClientUserData( 0,0 );
	ArmProceStarMoveFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmProceStarMoveFrame );
end

-- 普通装备重铸
function NormalArmRecast( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmRecastFrame:Hide()
	ArmRecastFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmRecastFrame:SetClientUserData( 0,0 );
	ArmRecastFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmRecastFrame );
end

-- 高级装备重铸
function AdvanceArmRecast( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmAdvRecastFrame:Hide()
	ArmAdvRecastFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmAdvRecastFrame:SetClientUserData( 0,nNpcActorId );
	ArmAdvRecastFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmAdvRecastFrame );
end
-- 装备洗练
function OpenArmWashFrame( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmWashFrame:Hide()
	ArmWashFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmWashFrame:SetClientUserData( 0,nNpcActorId );
	ArmWashFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmWashFrame );
end
-- 装备加工指引
function OpenArmProceFrame( nNpcActorId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	PlayerContainerFrame:SetPoint( "topright", "$parent", "topright",-86, 163 );
	ArmProceFrame:Hide()
	ArmProceFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmProceFrame:SetClientUserData( 0,nNpcActorId );
	ArmProceFrame:Show();
	--util.testNpcDialogDistance( nNpcActorId, ArmProceFrame );
	ArmWashFrame:Hide()
	ArmAdvWashFrame:Hide()
	ArmAdvRecastFrame:Hide()
	ArmRecastFrame:Hide()
	ArmProceStarMoveFrame:Hide()
	ArmProceStarFrame:Hide()
	ArmProceBindFrame:Hide()
end

-- 装备升级
function OpenArmLevelUpFrame( npcId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmLevelUpFrame:Hide()
	ArmLevelUpFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmLevelUpFrame:Show();
	setIsOrangeArmRequest(false)
	util.testNpcDialogDistance( npcId, ArmLevelUpFrame );
end
-- 橙色装备升级
function OpenArmLevelUpOrangeFrame( npcId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	ArmLevelUpFrame:Hide()
	ArmLevelUpFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmLevelUpFrame:Show();
	setIsOrangeArmRequest(true)
	util.testNpcDialogDistance( npcId, ArmLevelUpFrame );
end

function OpenArmAttrMoveFrame( npcId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	ArmAdvAttrMoveFrame:Hide();
	ArmAttrMoveFrame:Hide();
	ArmAttrMoveFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmAttrMoveFrame:Show();
	util.testNpcDialogDistance( npcId, ArmAttrMoveFrame );
end

function OpenArmAdvAttrMoveFrame( npcId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	ArmAttrMoveFrame:Hide();
	ArmAdvAttrMoveFrame:Hide();
	ArmAdvAttrMoveFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmAdvAttrMoveFrame:Show();
	util.testNpcDialogDistance( npcId, ArmAdvAttrMoveFrame );
end

function OpenArmResetFrame( npcId )
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	ArmAttrResetFrame:Hide()
	ArmAttrResetFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	ArmAttrResetFrame:Show();
	util.testNpcDialogDistance( npcId, ArmAttrResetFrame );
end

-- 公会福利
function OpenClanGiftFrame( nNpcActorId )
	ClanGiftFrame:Show();
	util.testNpcDialogDistance( nNpcActorId, ClanGiftFrame );
end

function OpenPetStarMoveFrame( npcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end
	if not PetFrame:IsShown() then 
		PetFrame:Show();
	end
	PetStarMoveFrame:Hide()
	PetStarMoveFrame:SetPointAlwaysRecalc("topleft", "PetFrame", "topright",0 ,0 );
	PetStarMoveFrame:Show()
	util.testNpcDialogDistance( npcId, PetStarMoveFrame );
end

function OpenPetEatFrame( npcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end
	if not PetFrame:IsShown() then 
		PetFrame:Show();
	end
	PetAttrEatFrame:Hide()
	PetAttrEatFrame:SetPointAlwaysRecalc("topleft", "PetFrame", "topright",0 ,0 );
	PetAttrEatFrame:Show()
	util.testNpcDialogDistance( npcId, PetAttrEatFrame );
end

function OpenPetStepUpFrame( npcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end
	if not PetFrame:IsShown() then 
		PetFrame:Show();
	end
	PetStepUpFrame:Hide()
	PetStepUpFrame:SetPointAlwaysRecalc("topleft", "PetFrame", "topright",0 ,0 );
	PetStepUpFrame:Show()
	util.testNpcDialogDistance( npcId, PetStepUpFrame );
end
function OpenPetUnBindFrame( npcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end
	if not PetFrame:IsShown() then 
		PetFrame:Show();
	end
	PetUnBindFrame:Hide()
	PetUnBindFrame:SetPointAlwaysRecalc("topleft", "PetFrame", "topright",0 ,0 );
	PetUnBindFrame:Show()
	util.testNpcDialogDistance( npcId, PetUnBindFrame );
end

function OpenPetImpUpFrame( npcId )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() == 0 then
		ShowMidTips( "身上没有宠物，不能进行该操作" );
		return;
	end

	PetImpUpFrame:Hide()
	PetImpUpFrame:SetPointAlwaysRecalc("topleft", "PetFrame", "topright",0 ,0 );
	PetImpUpFrame:Show()
	util.testNpcDialogDistance( npcId, PetImpUpFrame );
end

function NpcTalkOpt_OnClick()
	local nOptType	= this:GetClientUserData( 0 );
	local nOptIndex	= this:GetClientUserData( 1 );

	NpcDialog:selectOpt( nOptIndex, ( nOptType == NPC_DIALOG_LAST_OPT ) );
end
function NpcTalkOpt_OnEnter()
	ISOnEnterSpecialSelect( this:GetClientUserData( 1 ), t_NpcOpt.m_npcResId );	
end
function NpcTalkOpt_OnLeave()
	ClearSpecialTips();
end
function ISOnEnterSpecialSelect( selectIndex, npcId )
	if npcId == 302126 then
		local t_oneOpt = t_NpcOpt.m_allOpts[selectIndex];
		local szText = t_oneOpt["oneOptContent"];
		if string.find( szText, "装备分解（晶石获得）示意图", 1 ) then
			ZhuangBeiFenJieShiLiFrame:Show();
		end
		if string.find( szText, "镶嵌晶石（增加属性）示意图", 1 ) then
			XiangQianJingShiShiLiFrame:Show();
		end
		if string.find( szText, "晶石吸收（晶石升级）示意图", 1 ) then
			JingShiXiShouShiLiFrame:Show();
		end
	end
end
function ClearSpecialTips()
	ZhuangBeiFenJieShiLiFrame:Hide();
	XiangQianJingShiShiLiFrame:Hide();
	JingShiXiShouShiLiFrame:Hide();
end

function Close_NpcDialog( szDialogName )
	NpcDialog:send_CloseTalk();
	local frame = getglobal( szDialogName );
	frame:Hide();
end

function CheckNpcTalkXinShouGuid( npcId )
	local xinShouBtn = getglobal( "NpcTalkFrameXinShouBtn" );
	xinShouBtn:Hide();
	
	local mainplayer = ActorMgr:getMainPlayer();
	local checkEffectFunc = function ( npcId )
								local t_firstShowBtnEffectNpcIDs = { 304001, 306011, 350042, 304028, 302004,
												     302016, 316000, 317002, 304002, 306002, 
												     302005, 303021, 306000, };--303042
								for _, limitId in ipairs( t_firstShowBtnEffectNpcIDs ) do
									if npcId == limitId then
										return true;
									end
								end
								return false;
							end

	if ( not GameWizard:isDo(GAMEWIZARD_FIRST_GETQUEST_FROMUI) or not GameWizard:isDo(GAMEWIZARD_FIRST_FINISHQUEST_FROMUI) ) and 
		checkEffectFunc( npcId ) then
		local selectButton	= getglobal( "NpcTalkSelectSelectItem1" );
		if not selectButton:IsShown() then
			return;
		end

		local szState = GetTaskState( { text = selectButton:GetClientString() } );
		if szState == "(可接)" then
			ShowNpcTalkFrameXinShouBtn( { text = "请点击领取任务", btn = "NpcTalkSelectSelectItem1" } );
		elseif szState == "(完成)" then
			ShowNpcTalkFrameXinShouBtn( { text = "请点击提交任务", btn = "NpcTalkSelectSelectItem1" } );
		elseif szState == "" then
			ShowNpcTalkFrameXinShouBtn( { text = "请点击进行任务", btn = "NpcTalkSelectSelectItem1" } );
		end
		
		return;
	end
	--[[
	if not GameWizard:isDo(GAMEWIZARD_FIRST_FLY_QUEST)  and Quest:isQuestRun( 1060 ) then
		for i=1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			if selectButton:IsShown() and GetTaskState( { text = selectButton:GetClientString() } ) == "" then
				ShowNpcTalkFrameXinShouBtn( { text = "请点击进行飞行", btn = selectButton:GetName() } );
				break;
			end
		end
		GameWizard:doStep( GAMEWIZARD_FIRST_FLY_QUEST );
	end
	--]]
	
	checkEffectFunc = 
	function ( npcId )
		local t_firstShowBtnEffectNpcIDs = { 302016 };
		for _, limitId in ipairs( t_firstShowBtnEffectNpcIDs ) do
			if npcId == limitId then
				return true;
			end
		end
		return false;
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_TRAN_TO_SKY_COPY ) and Quest:isQuestRun( 1062 ) and Quest:isQuestRun( 1063 ) and 
			checkEffectFunc( npcId ) then
		for i=1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local szConvertResult = GetTaskState( { text = selectButton:GetClientString() }  );
			if selectButton:IsShown() and szConvertResult == "" then
				ShowNpcTalkFrameXinShouBtn( { text = "请点击进入副本", btn = selectButton:GetName() } );
				GameWizard:hideNpcTalkByResId( 302016 );
				break;
			end
		end
	end
	
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_containerControl	= GetContainerControl();
	
	-- TODO: 这儿需要策划给个宠物技能学习的任务ID	and Quest:isQuestRun( 1062 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEARN_PET_SKILL_OPTION ) and npcId == 304006 and 
		mainplayer:getLv() >= 30 and t_containerControl:isHavePetSkillLearnItem() and petPackage:getPetNum() > 0 then
		for i = 1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local selectFont	= getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "宠物技能学习", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击让宠物学习技能", btn = selectButton:GetName() } );
			end
		end
	end

	-- TODO: 这儿需要策划给个强化宠物的任务ID	and Quest:isQuestRun( 1062 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STRENGTHEN_PET_OPTION ) and npcId == 304006 and 
		mainplayer:getLv() >= 30 and t_containerControl:isHavePetDevelopmentBuildUpItem() and petPackage:getPetNum() > 0  then
		for i = 1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local selectFont	= getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "宠物强化", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击进行宠物强化", btn = selectButton:GetName() } );
			end
		end
	end

	-- TODO: 这儿需要策划给个装备任务ID	and Quest:isQuestRun( 1062 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_LEVEL_UP_BAO_SHI_OPTION ) and npcId == 306010 
		and mainplayer:getLv() >= 30 and t_containerControl:isHaveArmUpItem() then
		for i = 1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local selectFont	= getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "装备品质提升", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击进行装备强化", btn = selectButton:GetName() } );
			end
		end
	end

	-- TODO: 这儿需要策划给个装备任务ID	and Quest:isQuestRun( 1062 )
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_XIANGQIAN_BAO_SHI_OPTION ) and npcId == 302015
		and mainplayer:getLv() >= 30 and t_containerControl:isHaveGemInLayItem() then
		for i = 1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local selectFont	= getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "宝石镶嵌", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击进行宝石镶嵌", btn = selectButton:GetName() } );
			end
		end
	end
	
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_ENTER_FUBEN_OPTION ) then
		for i = 1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local selectFont	= getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "进入试炼场", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击进入试炼场副本", btn = selectButton:GetName() } );
			end
		end
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION ) and npcId == 306000 then
		for i = 1, MAX_SELECTOPT do
			local selectButton	= getglobal( "NpcTalkSelectSelectItem"..i );
			local selectFont	= getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "学习技能", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击进行技能学习", btn = selectButton:GetName() } );
			end
		end
	end

	if not GameWizard:isDo( GAMEWIZARD_FIRST_REPAIR ) and npcId == 302003 then
		for i = 1, MAX_SELECTOPT do
			local selectButton = getglobal( "NpcTalkSelectSelectItem".. i );
			local selectFont = getglobal( selectButton:GetName().."SelectFont" );
			if selectButton:IsShown() and string.find( selectFont:GetText(), "打开商店", 1, true ) then
				ShowNpcTalkFrameXinShouBtn( { text = "点击打开商店，修理装备", btn = selectButton:GetName() } );
			end
		end
	end
end

function ShowNpcTalkFrameXinShouBtn( data )
	local npcTalkXinShouBtn = getglobal( "NpcTalkFrameXinShouBtn" );
	local rich = getglobal( npcTalkXinShouBtn:GetName().."Rich" );
	rich:SetClientString( data["text"] );
	local nSelfDefineExtendWidth = 10;
	ReSizeTooltip( rich:GetName(), npcTalkXinShouBtn:GetName(), nSelfDefineExtendWidth );
	npcTalkXinShouBtn:SetClientString( data["btn"] );
	local relBtn = getglobal( data["btn"] );
	local animationTexture = getglobal( npcTalkXinShouBtn:GetName().."UVAnimationTex" );
	if relBtn:GetClientID() == 1 then
		npcTalkXinShouBtn:SetPoint( "topleft", data["btn"], "topleft", -5, -12 );
		animationTexture:SetPoint( "topleft", "NpcTalkFrameXinShouBtn" , "topleft", -25, 0 );
	else
		npcTalkXinShouBtn:SetPoint( "topleft", data["btn"], "topleft", -30,	-12 );
		animationTexture:SetPoint( "topleft", "NpcTalkFrameXinShouBtn" , "topleft", 0, 0 );
	end
	
	animationTexture:SetUVAnimation( 40, true );
	animationTexture:Show();
	npcTalkXinShouBtn:Show();
end

function ReAdjustNpcTalkFrameXinShouBtnPos()
	local npcTalkXinShouBtn = getglobal( "NpcTalkFrameXinShouBtn" );
	if not npcTalkXinShouBtn:IsShown() then
		return;
	end
	
	local relBtn = getglobal( npcTalkXinShouBtn:GetClientString() );
	local animationTexture = getglobal( npcTalkXinShouBtn:GetName().."UVAnimationTex" );
	if relBtn:GetClientID() == 1 then
		npcTalkXinShouBtn:SetPointEx( "topleft", relBtn:GetName(), "topleft", -5, -12 );
		animationTexture:SetPointEx( "topleft", "NpcTalkFrameXinShouBtn" , "topleft", -25, 0 );
	else
		npcTalkXinShouBtn:SetPointEx( "topleft", relBtn:GetName(), "topleft", -30,	-12 );
		animationTexture:SetPointEx( "topleft", "NpcTalkFrameXinShouBtn" , "topleft", 0, 0 );
	end
end

function NpcTalkFrame_OnShow()
	-- 重置窗口坐标
	NpcTalkFrame:SetPoint("left", "$parent", "left", 0, 0 );

	if ShopFrame:IsShown() then
		CFCloseBtn_OnClick();
	end

	if MultiCommitItemFrame:IsShown() then
		MultiCommitItemFrame:Hide();
	end

	if NpcCommitItemFrame:IsShown() then
		NpcCommitItemFrame:Hide();
	end
end

function NpcTalkFrame_OnHide()
	t_NpcTalkFrameControl:clearTalkInfo();
end

function NpcTalkFrame_OnUpdate()
	t_NpcTalkFrameControl:update();
end

 NORMAL_TASK_TYPE	= 1;
 ITEM_TASK_TYPE		= 2;
 SHARE_TASK_TYPE	= 3;

local t_TaskAcceptFrameControl = { npcId = 0, taskId = 0, taskType = 0 };

function GetTaskAcceptFrameControl()
	return t_TaskAcceptFrameControl;
end

t_TaskAcceptFrameControl["update"] = 
function ( self )
	local frame = getglobal( "TaskAcceptFrame" );
	if self["npcId"] == 0 and self["taskId"] ~= 0 then
		if ( os.clock() - share_task_clock ) > share_task_duration then
			Quest:refuseQuest( self["taskId"] );
			frame:Hide();
		end
		return;
	end
	
	local npc = ActorMgr:FindActor( self["npcId"] );
	if npc == nil then
		--[[
		-- TODO: NPC对话流程不改
		if self["npcId"] ~= 0 then
			NpcDialog:send_CloseTalk();
		end
		--]]
		
		frame:Hide();
		return;
	end

	local nDist = util.CalActorsDistance( npc, ActorMgr:getMainPlayer() );
	if nDist > ( NpcDialog:getMaxNpcTalkDist( npc ) / 100 ) ^ 2 then
		-- TODO: NPC对话流程不改
		--NpcDialog:send_CloseTalk();
		frame:Hide();
	end
end

t_TaskAcceptFrameControl["setTalkInfo"] = 
function ( self, data )
	self["npcId"]		= data["npcId"];
	self["taskId"]		= data["taskId"];
	self["isItemTask"]	= data["isItemTask"];
	self["taskType"]	= data["taskType"];
end

t_TaskAcceptFrameControl["clearTalkInfo"] = 
function ( self )
	if self["taskType"] == ITEM_TASK_TYPE then
		ClearItemLock( "TaskAcceptFrame" );
	end

	self["npcId"]		= 0;
	self["taskId"]		= 0;
	self["isItemTask"]	= false;
	self["taskType"]	= 0;
end

t_TaskAcceptFrameControl["getTaskType"] = 
function ( self )
	return self["taskType"];
end

t_TaskAcceptFrameControl["getNpcID"] = 
function ( self )
	return self["npcId"];
end

t_TaskAcceptFrameControl["getTaskID"] = 
function ( self )
	return self["taskId"];
end

t_TaskAcceptFrameControl["queryIsItemTask"] = 
function ( self )
	return self["isItemTask"];
end

--接任务界面相关操作
function TaskAcceptFrame_OnLoad()
	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskAcceptFrameItemBtn"..i.."AwardItem");
		btn:SetClientUserData( 0, DRAG_AWARDITEM );
	end

	InitTaskAcceptFrame();
end

function TaskAcceptFrame_OnEvent()
end

function TaskAcceptFrame_OnHide()
	local t_hideIcons = { "TaskAcceptFrame_AcceptBtnXinShouBtn" };
	util.HideIcons( t_hideIcons );
	AutoHideNomalTips("TaskAcceptFrame");
	-- npcid == 0 为共享任务
	-- TODO: NPC对话流程不改 这2句要去掉
	if t_TaskAcceptFrameControl:getNpcID() ~= 0 then
		NpcDialog:send_CloseTalk();
	end
	
	
	--print( "npcId = "..t_TaskAcceptFrameControl:getNpcID().."，  questId = "..t_TaskAcceptFrameControl:getTaskID().."--1332" );
	--[[
	if t_TaskAcceptFrameControl:getNpcID() == 0 and t_TaskAcceptFrameControl:getTaskID() ~= 0 and 
		not t_TaskAcceptFrameControl:queryIsItemTask() then
		--print( "Quest:refuseQuest--1336" );
		Quest:refuseQuest( t_TaskAcceptFrameControl:getTaskID() );
	end
	--]]
	t_TaskAcceptFrameControl:clearTalkInfo();
end

function TaskAcceptFrame_OnUpdate()
	t_TaskAcceptFrameControl:update();
end

function ClearTaskAcceptFrame()
	local t_fonts = { ["TaskAcceptScrollFrameTaskAwardBtnExtraAwardFont"]	= { value = "", }, };
	util.SetFonts( t_fonts );
end

function TaskAcceptFrame_OnShow()
	ClearTaskAcceptFrame();
	-- npcid == 0 为共享任务
	local nNpcId = this:GetClientUserData( 0 );
	if nNpcId == 0 then
		share_task_clock = os.clock();
	end

	FirstClickAcceptTask( this:GetClientUserData( 1 ) );

	ResetCurCanAcceptQuestId();
	-- 重置窗口坐标
	TaskAcceptFrame:SetPoint("topleft", "$parent", "topleft",NpcTalkFrame:GetRealLeft(), NpcTalkFrame:GetRealTop());
	local scrollFrame = getglobal( "TaskAcceptScrollFrame" );
	scrollFrame:SetValueStep( 5 );
	scrollFrame:SetVerticalScroll( 0 );

	TaskAcceptFrame_AcceptBtn:Enable();
	ReSetTaskAcceptMember();
	local nNpcId = this:GetClientUserData(0);			--NPCId
	local nQuestId = this:GetClientUserData(1);			--任务Id
	local nRecycleID = 0;
	if this:GetClientString() ~= "道具领取任务" then
		nRecycleID = this:GetClientUserData( 2 );
	end

	local szDlgText = "";
	local szDes	= "";
	if t_TaskAcceptFrameControl:getTaskType() == ITEM_TASK_TYPE then
		szDes	= ReplaceConsumeMoneyDesc( Quest:getQuestDialog(nQuestId), nQuestId, nRecycleID ); --任务描述
	else
		szDes	= ReplaceConsumeMoneyDesc( Quest:getQuestDes(nQuestId), nQuestId, nRecycleID ); --任务描述
	end
	
	local nExp			= Quest:getQuestExp(nQuestId, nRecycleID);			--获取任务奖励经验
	local tagTaskMoney	= Quest:getQuestMoney(nQuestId, nRecycleID);		--获取任务奖励金钱
	local szTaskName	= Quest:getQuestName(nQuestId);			--获取任务名称
	local szName		= "";									--NPC名称
	
	if Quest:canAcceptQuest( nQuestId ) == 2 then
		TaskAcceptFrame_AcceptBtn:Disable();
		local lvmin = Quest:getQuestLvLimit(nQuestId);
		szTaskName = szTaskName.."(等级达到"..lvmin.."级后可接)";
	end

	if nNpcId ~= 0 then
		local npc = ActorMgr:FindActor(nNpcId);
		szName = npc:getName();
		--print( "TaskAcceptFrame_OnShow--this:GetClientString()"..this:GetClientString().."--1028" );
		szDlgText = ReplaceConsumeMoneyDesc( this:GetClientString(), nQuestId, nRecycleID ); --接取对话内容
		--print( "szDlgText = "..szDlgText.."--1110\n\n" );
	end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then 
		return;
	end
	
	TaskAcceptFrame_DlgTextRich:SetFontType(FONTSYTLE_NORMAL);	
	local rich = getglobal( "TaskAcceptFrame_DlgTextRich" );
	rich:resizeRect( 320 * GetScreenScaleY(), 400 * GetScreenScaleY() );
	local szQuestDesc = ReplaceQuestDesc( "#c982e02"..szTaskName.."#n\n"..szDes );
	--print( "szQuestDesc = "..szQuestDesc.."--1517" );
	TaskAcceptFrame_DlgTextRich:SetText( szQuestDesc, 88, 73, 53 );
	--print( ReplaceQuestDesc( "#c982e02"..szTaskName.."#n\n"..szDes ).."--1037" );
	local szAddDlgText = ReplaceQuestDesc( szDlgText );
	--print( "szAddDlgText = "..szAddDlgText.."--1521" );
	TaskAcceptFrame_DlgTextRich:AddText( szAddDlgText, 88, 73, 53 );
	--print( "szDlgText = "..szDlgText.."--1040\n\n" );
	--print( ReplaceQuestDesc( szDlgText ).."--1042" );

	local nHeight = TaskAcceptFrame_DlgTextRich:GetTotalHeight();
	TaskAcceptFrame_DlgTextRich:SetHeight( nHeight / GetScreenScaleY() );
	TaskAcceptFrame_DlgTextRich:resizeRichHeight( nHeight / GetScreenScaleY() );
	TaskAcceptFrame_DlgTextRich:ScrollFirst();

	TaskAcceptScrollFramePartionBtn:SetPoint("BOTTOMLEFT", "TaskAcceptFrame_DlgTextRich", "BOTTOMLEFT", 0, 20);
	TaskAcceptScrollFrameTaskAwardBtn:SetPoint( "TOPLEFT","TaskAcceptFrame_DlgTextRich","BOTTOMLEFT", 0, 30 );

	local nItemNum = Quest:getQuestItemNum(nQuestId, nRecycleID);
	local nIndexItem = 0;
	local nOffestX	= 40;
	TaskAcceptFrameItemGetDesc:SetText("");
	for i = 1, CLIENT_MAX_TASK_AWARD_ITEM do
		if nIndexItem > 6 then
			return;
		end
		local nItemId = Quest:getQuestItem( nQuestId, nRecycleID, i - 1 );
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			local nItemCount = Quest:getQuestItemCount( nQuestId, nRecycleID, i - 1 );
			--获取任务奖励物品所需要职业
			local MainPlayer = ActorMgr:getMainPlayer();
			if MainPlayer == nil then return end
			
			if Quest:isQuestItemCareer(nQuestId, nRecycleID, i - 1, MainPlayer:getRace()) then
				nIndexItem = nIndexItem +1;
				local parentbtn = getglobal("TaskAcceptFrameItemBtn"..nIndexItem);
				local btn = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItem");
				local tex = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItemIconTexture");
				local itemtex = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItemTex");
				local itemfont = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItemFont");	   
				local itembacktex = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItemBackTexture");	 
				local itemCountFont = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItemCountFont"); 
				local itemUVTex = getglobal("TaskAcceptFrameItemBtn"..nIndexItem.."AwardItemUVAnimationTex");
				itemCountFont:SetPoint( "bottomleft", parentbtn:GetName(), "bottomleft", 0, 10 );
				itemCountFont:SetText("");
				itembacktex:Show();
				parentbtn:Show();
				itemtex:Show();
				itemfont:Show();
				btn:Show();
				itemfont:SetSize( 95, 35 );
				if nIndexItem%2 == 0 then
					parentbtn:SetPoint("TOPLEFT","TaskAcceptScrollFramePartionBtn","BOTTOMLEFT", 160,	nOffestX + 15+(math.ceil(nIndexItem/2)-1)*50);
					itemfont:SetPoint("TOPLEFT","TaskAcceptScrollFramePartionBtn","BOTTOMLEFT", 210, nOffestX + 20+(math.ceil(nIndexItem/2)-1)*50);
				else
					parentbtn:SetPoint("TOPLEFT","TaskAcceptScrollFramePartionBtn","BOTTOMLEFT", 0,	nOffestX + 15+(math.ceil(nIndexItem/2)-1)*50);
					itemfont:SetPoint("TOPLEFT","TaskAcceptScrollFramePartionBtn","BOTTOMLEFT", 50, nOffestX + 20+(math.ceil(nIndexItem/2)-1)*50);
				end

				local itemdef = getItemDef( nItemId );
				if itemdef ~= nil then
					local szItemName = itemdef.Name;
					if nItemCount > 1 then
						szItemName = szItemName.."\n数量："..nItemCount;
					end
					
					itemfont:SetText( szItemName, 57, 48, 34 );
					itemCountFont:SetPoint( "bottomleft", parentbtn:GetName(), "bottomleft", 0, 10 );
					local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					tex:SetTexture( IconPath );
					tex:SetTexRelUV( 1, 1 );
					tex:Show();
				end

				SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());
				itemUVTex:SetTexRelUV( 1, 1 );
				
				btn:SetClientUserData( 1, nItemId );

				--获取道具奖励类型
				local nAwardType = Quest:getAwardType( nQuestId, nRecycleID );
				TaskAcceptScrollFrameTaskItemGetDescBtn:SetPoint( "TOPLEFT","TaskAcceptScrollFramePartionBtn","BOTTOMLEFT", 0, nOffestX - 10 );
				if nAwardType == AWARD_TYPE_ALL or nAwardType == 4 then
					TaskAcceptFrameItemGetDesc:SetText("你将得到：");
				elseif nAwardType == AWARD_TYPE_ONE then
					TaskAcceptFrameItemGetDesc:SetText("该任务奖励其中一件物品：");
				elseif nAwardType == AWARD_TYPE_RAND then
					TaskAcceptFrameItemGetDesc:SetText("系统将从以下奖励物品随机选择一个：");
				end
			end
		end
	end

	TaskAcceptFrameNpcNameFont:SetText(szName);

	local nPointY =nOffestX + 83+(math.ceil(nIndexItem/2)-1)*50;
	if nIndexItem == 0 then
		nPointY = nOffestX +8;
	end
	local nMoney = tagTaskMoney.Val;
	local nType = tagTaskMoney.Type;
	local nOffset	= 0;
	local szMonType = "";
	local texPath = "uires\\ZhuJieMian\\1.dds"
	if nType == ATTR_ID_MONEY then
		TaskAcceptFrameMoneySignTex:SetTexture(texPath);
		TaskAcceptFrameMoneySignTex:SetTexUV(117,494,17,18);
		TaskAcceptFrameMoneySignTex:SetSize(17,18);
		szMonType = ""..MONEY_TYPE_MONEY_NAME.."：";
		nOffset = nOffset;
	elseif nType == ATTR_ID_BULL then
		TaskAcceptFrameMoneySignTex:SetTexture(texPath);
		TaskAcceptFrameMoneySignTex:SetTexUV(78,494,20,18);
		TaskAcceptFrameMoneySignTex:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BULL_NAME.."：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BINDGODCOIN then
		TaskAcceptFrameMoneySignTex:SetTexture(texPath);
		TaskAcceptFrameMoneySignTex:SetTexUV(57,494,20,18);
		TaskAcceptFrameMoneySignTex:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BIND_GODCOIN_NAME.."：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BATTLE_GLORY then
		TaskAcceptFrameMoneySignTex:SetTexture(texPath);
		TaskAcceptFrameMoneySignTex:SetTexUV(963,203,16,18);
		TaskAcceptFrameMoneySignTex:SetSize(16,18);
		szMonType = "荣誉：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_ZHENGYI then
		SetTaskAwardTexture( "TaskAcceptFrameMoneySignTex",nType) ;
		szMonType = "正义点：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_AOSHAN_BRAND then
		SetTaskAwardTexture( "TaskAcceptFrameMoneySignTex",nType) ;
		szMonType = "战魂徽章：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_LIFT_BRAND then
		SetTaskAwardTexture( "TaskAcceptFrameMoneySignTex",nType) ;
		szMonType = "生存奖章：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_SCHOOL_VAL then
		SetTaskAwardTexture( "TaskAcceptFrameMoneySignTex",nType) ;
		szMonType = "传承点：";
		nOffset = nOffset + 1;
	end
	
	local fScale	= 1;
	local nTaskType = Quest:getQuestType( nQuestId );
	if nTaskType == CLIENT_TASK_TYPE_CLAN and Quest:isRecycleTask( nQuestId ) then
		local nFiniRecycleIdx	= Quest:getReCycleQusetOneDayNum( nRecycleID );
		--fScale = fScale * CLAN_AWARD_ONE_TIME_SCALE ^ nFiniRecycleIdx;
		--print( "TaskAcceptFrame_OnShow -- nFiniRecycleIdx = "..nFiniRecycleIdx.."--1552" );
	end
	
	if nExp > 0 then
		nExp = math.floor( nExp * fScale );
	end

	if nMoney > 0 then
		nMoney = math.floor( nMoney * fScale );
	end

	if tonumber( nMoney ) <= 0 and tonumber( nExp ) > 0  then
		TaskAcceptFrameMoneyFont:SetText("");
		TaskAcceptFrameExpFont:SetText("经验："..nExp);
		TaskAcceptScrollFrameTaskExpBtn:SetPoint("bottomleft", "TaskAcceptScrollFramePartionBtn", "bottomleft", 0, nPointY );
		TaskAcceptScrollFrameTaskMoneyTexBtn:Hide();
	elseif tonumber( nMoney ) > 0 and tonumber( nExp ) <= 0 then
		TaskAcceptFrameExpFont:SetText("");
		TaskAcceptFrameMoneyFont:SetText(szMonType..nMoney);	
		local nMoneyFontWidth = TaskAcceptFrameMoneyFont:GetTextExtentWidth( TaskAcceptFrameMoneyFont:GetText() ) /GetScreenScaleY();
		TaskAcceptScrollFrameTaskMoneyBtn:SetPoint("bottomleft", "TaskAcceptScrollFramePartionBtn", "bottomleft", 0, nPointY );	 
		TaskAcceptScrollFrameTaskMoneyTexBtn:SetPoint("bottomleft", "TaskAcceptScrollFramePartionBtn", "bottomleft", nMoneyFontWidth, nPointY+nOffset );
		TaskAcceptScrollFrameTaskMoneyTexBtn:Show();
	elseif  tonumber( nMoney ) > 0 and tonumber( nExp ) > 0 then
		TaskAcceptFrameExpFont:SetText("经验："..nExp);
		TaskAcceptFrameMoneyFont:SetText(szMonType..nMoney);
		local moneyparent = getglobal(TaskAcceptFrameMoneyFont:GetParent());
		local nMoneyFontWidth = TaskAcceptFrameMoneyFont:GetTextExtentWidth( TaskAcceptFrameMoneyFont:GetText() )/GetScreenScaleY();
		TaskAcceptScrollFrameTaskExpBtn:SetPoint("bottomleft", "TaskAcceptScrollFramePartionBtn", "bottomleft", 0, nPointY );
		TaskAcceptScrollFrameTaskMoneyBtn:SetPoint("bottomleft", "TaskAcceptScrollFramePartionBtn", "bottomleft", 162, nPointY );
		TaskAcceptScrollFrameTaskMoneyTexBtn:SetPoint("bottomleft", "TaskAcceptScrollFramePartionBtn", "bottomleft", 167+nMoneyFontWidth, nPointY+nOffset );
		TaskAcceptScrollFrameTaskMoneyTexBtn:Show();
	else
		TaskAcceptFrameExpFont:SetText("");
		TaskAcceptFrameMoneyFont:SetText("");
		TaskAcceptScrollFrameTaskMoneyTexBtn:Hide();
		nPointY = nPointY - 20;
	end
	nPointY = nPointY + 20;
	
	local szAwardDesign		= Quest:getAwardDesignation(nQuestId, nRecycleID);				--获取任务奖励称号
	local szAwardRepact		= Quest:getAwardRespactName(nQuestId, nRecycleID);				--获取任务奖励声望名
	local nAwardRepact		= Quest:getAwardRespactValue(nQuestId, nRecycleID);				--获取任务奖励声望值
	local szSecondarySkill	= Quest:getAwardSecondarySkillName(nQuestId);		--生活技能
	local nClanMoney		= math.floor( Quest:getQuestClanMoney(nQuestId, nRecycleID ) );		--公会资金
	local nClanAnDing		= math.floor( Quest:getQuestClanAnDing(nQuestId) * fScale );		--公会安定度
	local nClanContribute	= math.floor( Quest:getQuestClanContribute(nQuestId, nRecycleID) * fScale );	--个人贡献度
	local nClanTechPoint	= math.floor( Quest:getQuestClanTechPoint(nQuestId) * fScale );	-- 公会科技点

	local szBtnRef			= "TaskAcceptScrollFramePartionBtn";
	local DesignationFont	= getglobal("TaskAcceptFrameDesignationFont");
	local DesignationBtn	= getglobal("TaskAcceptScrollFrameTaskDesignationBtn");
	local RespactFont		= getglobal("TaskAcceptFrameRespactFont");
	local RespactBtn		= getglobal("TaskAcceptScrollFrameTaskRespactBtn");
	local SeondarySkillFont = getglobal("TaskAcceptFrameSecondaryFont");
	local SecondarySkillBtn = getglobal("TaskAcceptScrollFrameTaskSecondarySkillBtn");
	local ClanMoneyFont		= getglobal("TaskAcceptFrameClanMoneyFont");
	local ClanMoneyBtn		= getglobal("TaskAcceptScrollFrameTaskClanMoneyBtn");
	local ClanAnDingFont	= getglobal("TaskAcceptFrameClanAnDingFont");
	local ClanAnDingBtn		= getglobal("TaskAcceptScrollFrameTaskClanAnDingBtn");
	local ClanContributeFont= getglobal("TaskAcceptFrameClanContributeFont");
	local ClanContributeBtn	= getglobal("TaskAcceptScrollFrameTaskClanContributeBtn");
	local ClanTechPointFont= getglobal("TaskAcceptFrameClanTechPointFont");
	local ClanTechPointBtn	= getglobal("TaskAcceptScrollFrameTaskClanTechPointBtn");

	if szAwardDesign ~= "" and szAwardRepact == "" then
		DesignationFont:SetText("称号："..szAwardDesign);
		RespactFont:SetText("");
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	elseif szAwardDesign == "" and szAwardRepact ~= "" then 
		RespactFont:SetText("声望：+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("");
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	elseif szAwardDesign ~= "" and szAwardRepact ~= "" then 
		RespactFont:SetText("声望：+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("称号："..szAwardDesign);
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 162, nPointY);
	else
		RespactFont:SetText("");
		DesignationFont:SetText("");
	end
	if szSecondarySkill ~= "" then
		nPointY = nPointY + 20;
		if Quest:getAwardSecondarySkillVal( nQuestId ) > 0 then
			SeondarySkillFont:SetText("生活科技熟练度："..math.ceil( Quest:getAwardSecondarySkillVal( nQuestId ) / 100  ) );
		else
			SeondarySkillFont:SetText("科技生产："..szSecondarySkill);
		end
		SecondarySkillBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	else
		SeondarySkillFont:SetText("");
	end
	
	local nPointX = 0;
	if nClanMoney ~= 0 then
		nPointY = nPointY + 20;
		ClanMoneyFont:SetText("公会资金："..nClanMoney);
		ClanMoneyBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
		nPointX = 162;
	else
		ClanMoneyFont:SetText("");
	end

	if nClanAnDing ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanAnDingFont:SetText("公会安定度："..nClanAnDing);
		ClanAnDingBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanAnDingFont:SetText("");
	end

	if nClanContribute ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanContributeFont:SetText("个人贡献度："..nClanContribute);
		ClanContributeBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanContributeFont:SetText("");
	end

	if nClanTechPoint ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanTechPointFont:SetText("公会科技点："..nClanTechPoint);
		ClanTechPointBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanTechPointFont:SetText("");
	end
	
	local ResourceClass = 0
	for id, num in TaskClanResourceAwards(nQuestId) do
		num = math.floor(num * fScale)
		if id ~= 0 and num ~= 0 then
			ResourceClass = ResourceClass + 1
			local btn = getglobal("TaskAcceptScrollFrameTaskResource"..ResourceClass)
			if not btn then
				break
			end
			local text = getglobal("TaskAcceptScrollFrameTaskResource"..ResourceClass.."Text")
			if nPointX == 0 then
				nPointY = nPointY + 20
			end
			text:SetText(string.format("%s：%d", GameClanManager:getProductDefByID(id).Name, num))
			btn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY)
			if nPointX > 0 then
				nPointX = 0
			else
				nPointX = 162
			end
		end
	end
	while true do
		ResourceClass = ResourceClass + 1
		local text = getglobal("TaskAcceptScrollFrameTaskResource"..ResourceClass.."Text")
		if not text then
			break
		end
		text:SetText("")
	end

	SetTaskRecycleItem( nQuestId, nPointY, "TaskAcceptFrame", "TaskAcceptScrollFrame", "TaskAcceptFrame_DlgTextRich", nRecycleID );
	scrollFrame:ResetChildAnchor();
	SetScrollBar( "TaskAcceptFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	if scrollFrame:GetVerticalScrollRange() > 0 then
		TaskAcceptFrame_ScrollBackGroundTexCenter:Show();
		TaskAcceptFrame_ScrollBackGroundTexTop:Show();
		TaskAcceptFrame_ScrollBackGroundTexBottom:Show();
	else
		TaskAcceptFrame_ScrollBackGroundTexCenter:Hide();
		TaskAcceptFrame_ScrollBackGroundTexTop:Hide();
		TaskAcceptFrame_ScrollBackGroundTexBottom:Hide();
	end
	TaskAcceptFrame_ScrollBar:SetValue(0);
end

function ReSetTaskAcceptMember()
	TaskAcceptFrame_DlgTextRich:Clear();

	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskAcceptFrameItemBtn"..i);
		local rich = getglobal("TaskAcceptFrameItemBtn"..i.."AwardItemFont");	
		btn:Hide();
		rich:Hide();
	end
end

function InitTaskAcceptFrame()
	TaskAcceptFrame:SetClientUserData(0,0);	--NPCId
	TaskAcceptFrame:SetClientUserData(1,0);	--任务Id
	TaskAcceptFrame:SetClientUserData(2,0);	--类型 道具领取任务
	TaskAcceptFrame:SetClientUserData(3,0);	--类型 道具领取任务
	TaskAcceptFrame:SetClientString("");
end

function TaskAwardScrollFrame_OnVerticalScroll()
end

function NpcTalkFrame_OnVerticalScroll()
	ReAdjustNpcTalkFrameXinShouBtnPos();
end

-- 当ScrollFrame最大滚动范围变化的时候响应
function NpcTalkFrameOnScrollRangeChanged()
	local szSlider = "";
	if this:GetName() == "TaskAcceptScrollFrame" then
		szSlider = "TaskAcceptFrame_ScrollBar";
	elseif this:GetName() == "TaskNotFinishScrollFrame" then
		szSlider = "TaskNotFinishFrame_ScrollBar";
	elseif this:GetName() == "TaskAwardScrollFrame"  then
		szSlider = "TaskAwardFrame_ScrollBar";
	elseif this:GetName() == "NpcDialogScrollFrame" then
		szSlider = "NpcDialogFrame_ScrollBar";
	end
	-- 滚动条设置为
	local scrollFrame	= getglobal( this:GetName() );
	SetScrollBar( szSlider, 0, scrollFrame:GetVerticalScrollRange() );
	ReAdjustNpcTalkFrameXinShouBtnPos();
end

function SetTaskRecycleItem( nQuestId, nY, szParent, szScroll, szRef, nRecycleID )
	local tagTaskMoney	= Quest:getQuestMoney(nQuestId, nRecycleID, true);		
	local nRacycleIdx	= Quest:getTaskCycleNum( nQuestId, nRecycleID );
	local nTaskType		= Quest:getQuestType(nQuestId);  --获取任务类型 
	local szPartionBtn	= szScroll.."PartionBtnRecycle";
	local t_RecyCleUI = 
		{
			["PartionBtn"]	= { frame = getglobal( szPartionBtn ) };
			["GetDescBtn"]  = { frame = getglobal( szScroll.."TaskItemGetDescBtnRecycle" ) };
			["GetDescFont"] = { frame = getglobal( szParent.."ItemGetDescRecycle" ) };
			["SignTex"]		= { frame = getglobal( szParent.."MoneySignTexRecycle" ) };
			["MoneyFont"]	= { frame = getglobal( szParent.."MoneyFontRecycle" ) };
			["ExpFont"]     = { frame = getglobal( szParent.."ExpFontRecycle" ) };
			["DesiFont"]    = { frame = getglobal( szParent.."DesignationFontRecycle" ) };
			["RespactFont"] = { frame = getglobal( szParent.."RespactFontRecycle" ) };
			["MoneyTexBtn"]	= { frame = getglobal( szScroll.."TaskMoneyTexBtnRecycle" ) };
			["MoneyBtn"]    = { frame = getglobal( szScroll.."TaskMoneyBtnRecycle" ) };
			["ExpBtn"]		= { frame = getglobal( szScroll.."TaskExpBtnRecycle" ) };
			["DesiBtn"]		= { frame = getglobal( szScroll.."TaskDesignationBtnRecycle" ) };
			["RespactBtn"]	= { frame = getglobal( szScroll.."TaskRespactBtnRecycle" ) };
		};
	
	if (nRacycleIdx) == 0 then
		for _, ui in pairs( t_RecyCleUI ) do
			ui.frame:Hide();
		end
		return;
	end
	
	for _, ui in pairs( t_RecyCleUI ) do
		ui.frame:Show();
	end

--[[
	if nTaskType == CLIENT_TASK_TYPE_CLAN then
		local t_hideIcons = 
		{
			szPartionBtn, szScroll.."TaskItemGetDescBtnRecycle"
		};
		util.HideIcons( t_hideIcons );
	end
	]]--
	
	t_RecyCleUI["PartionBtn"].frame:SetPoint("BOTTOMLEFT", szRef, "BOTTOMLEFT", 0, 35+nY );
	t_RecyCleUI["PartionBtn"].frame:Show();
	t_RecyCleUI["GetDescBtn"].frame:SetPoint( "TOPLEFT",szPartionBtn,"BOTTOMLEFT", 0, 5 );

	local isShowRecycleDesc = Quest:isShowRecycleDesc( nQuestId, nRecycleID );
	if isShowRecycleDesc then
		--t_RecyCleUI["GetDescFont"].frame:SetText("完成"..nRacycleIdx.."环任务将额外获得以下奖励：");
		local nLeastTaskInRecycle	= Quest:getRecycleLeastTaskNum( nRecycleID );
		t_RecyCleUI["GetDescFont"].frame:SetText("完成1环任务（"..nLeastTaskInRecycle.."个任务）将额外获得以下奖励：");
	else
		t_RecyCleUI["PartionBtn"].frame:Hide();
		t_RecyCleUI["GetDescFont"].frame:SetText("");
	end
	
	local nExtraAwardRate	= Quest:getRecycleTaskAwardRate( nQuestId, nRecycleID );
	local szAwardRateDesc	= "";
	
	if nExtraAwardRate > 0 then
		local nCurRecycleIndex	= Quest:getQuestRecycleIndex( nQuestId, nRecycleID );
		szAwardRateDesc			= "（每日第"..( nCurRecycleIndex + 1 ).."环奖励："..nExtraAwardRate.."%）"
	end
	
	--print( "SetTaskRecycleItem--"..szScroll.."TaskAwardBtnExtraAwardFont--szAwardRateDesc = "..szAwardRateDesc );
	local t_fonts = { [szScroll.."TaskAwardBtnExtraAwardFont"]	= { value = szAwardRateDesc, }, };
	util.SetFonts( t_fonts );

	local szName		= "";									
	local szAwardDesign = "";	
	local nLastItemNum	= nIndexItem;
	local nItemNum		= Quest:getQuestItemNum( nQuestId, nRecycleID, true );
	local nExp			= Quest:getQuestExp(nQuestId, nRecycleID, true);			--获取任务奖励经验
	local nOffsetX		= 5;
	local RECYCLE_ITEM_INDEX = 6;
	local nIndexItem = RECYCLE_ITEM_INDEX;
	for i = 1, CLIENT_MAX_TASK_AWARD_ITEM do
		if nIndexItem > 12 then
			return;
		end

		local nItemId		= Quest:getQuestItem( nQuestId, nRecycleID, i - 1, true );
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			local nItemCount	= Quest:getQuestItemCount( nQuestId, nRecycleID, i - 1, true );
			--获取任务奖励物品所需要职业
			nIndexItem = nIndexItem +1;
			local parentbtn		= getglobal( szParent.."ItemBtn"..nIndexItem );
			local btn			= getglobal( parentbtn:GetName().."AwardItem" );
			local tex			= getglobal( parentbtn:GetName().."AwardItemIconTexture" );
			local itemtex		= getglobal( parentbtn:GetName().."AwardItemTex" );
			local itemfont		= getglobal( parentbtn:GetName().."AwardItemFont" );	   
			local itembacktex	= getglobal( parentbtn:GetName().."AwardItemBackTexture" );	 
			local itemCountFont = getglobal( parentbtn:GetName().."AwardItemCountFont" ); 
			local itemUVTex		= getglobal( parentbtn:GetName().."AwardItemUVAnimationTex" );
			itemCountFont:SetPoint( "bottomleft", parentbtn:GetName(), "bottomleft", 0, 10 );
			itemCountFont:SetText("");
			itembacktex:Show();
			parentbtn:Show();
			itemtex:Show();
			itemfont:Show();
			btn:Show();
			itemfont:SetSize( 95, 35 );
			if nIndexItem%2 == 0 then
				parentbtn:SetPoint("TOPLEFT",szPartionBtn,"BOTTOMLEFT", 160,	nOffsetX + 25 + (math.ceil((nIndexItem-RECYCLE_ITEM_INDEX)/2)-1)*50);
				itemfont:SetPoint("TOPLEFT",szPartionBtn,"BOTTOMLEFT", 210, nOffsetX + 30 + (math.ceil((nIndexItem-RECYCLE_ITEM_INDEX)/2)-1)*50);
			else
				parentbtn:SetPoint("TOPLEFT",szPartionBtn,"BOTTOMLEFT", 0,	nOffsetX + 25 + (math.ceil((nIndexItem-RECYCLE_ITEM_INDEX)/2)-1)*50);
				itemfont:SetPoint("TOPLEFT",szPartionBtn,"BOTTOMLEFT", 50, nOffsetX + 30 + (math.ceil((nIndexItem-RECYCLE_ITEM_INDEX)/2)-1)*50);
			end

			local itemdef = getItemDef( nItemId );
			if itemdef ~= nil then
				local szItemName = itemdef.Name;
				if nItemCount > 1 then
					szItemName = szItemName.."\n数量："..nItemCount;
				end
				
				itemfont:SetText( szItemName, 57, 48, 34 );
				itemCountFont:SetPoint( "bottomleft", parentbtn:GetName(), "bottomleft", 0, 10 );
				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				tex:SetTexture( IconPath );
				tex:SetTexRelUV( 1, 1 );
				tex:Show();
			end

			SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());
			itemUVTex:SetTexRelUV( 1, 1 );		
			btn:SetClientUserData( 1, nItemId );
		end
	end

	local nPointY = nOffsetX + 40 + (math.ceil((nIndexItem-RECYCLE_ITEM_INDEX)/2))*50;
	if (nIndexItem-RECYCLE_ITEM_INDEX) == 0 then
		nPointY =  nOffsetX + 40;
	end
	local nMoney = tagTaskMoney.Val;
	local nType = tagTaskMoney.Type;
	local nOffset	= 0;
	if nType == ATTR_ID_MONEY then
		t_RecyCleUI["SignTex"].frame:SetTexUV(117,494,17,18);
		t_RecyCleUI["SignTex"].frame:SetSize(17,18);
		t_RecyCleUI["SignTex"].frame:Show();
		nOffset = nOffset;
		szMonType = ""..MONEY_TYPE_MONEY_NAME.."：";
	elseif nType == ATTR_ID_BULL then
		t_RecyCleUI["SignTex"].frame:SetTexUV(78,494,20,18);
		t_RecyCleUI["SignTex"].frame:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BULL_NAME.."：";
		t_RecyCleUI["SignTex"].frame:Show();
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BINDGODCOIN then
		t_RecyCleUI["SignTex"].frame:SetTexUV(57,494,20,18);
		t_RecyCleUI["SignTex"].frame:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BIND_GODCOIN_NAME.."：";
		t_RecyCleUI["SignTex"].frame:Show();
		nOffset = nOffset + 1;
	end
	
	if tonumber( nMoney ) <= 0 and tonumber( nExp ) > 0  then
		t_RecyCleUI["MoneyFont"].frame:SetText("");
		t_RecyCleUI["ExpFont"].frame:SetText("经验："..nExp);
		t_RecyCleUI["ExpBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 0, nPointY );
		nPointY = nPointY + 20;
		t_RecyCleUI["MoneyTexBtn"].frame:Hide();
	elseif tonumber( nMoney ) > 0 and tonumber( nExp ) <= 0 then
		t_RecyCleUI["ExpFont"].frame:SetText("");
		t_RecyCleUI["MoneyFont"].frame:SetText(szMonType..nMoney);	
		local nMoneyFontWidth = t_RecyCleUI["MoneyFont"].frame:GetTextExtentWidth( t_RecyCleUI["MoneyFont"].frame:GetText() );
		t_RecyCleUI["MoneyBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 0, nPointY );	 
		t_RecyCleUI["MoneyTexBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", nMoneyFontWidth, nPointY+nOffset );
		nPointY = nPointY + 20;
		t_RecyCleUI["MoneyTexBtn"].frame:Show();
	elseif  tonumber( nMoney ) > 0 and tonumber( nExp ) > 0 then
		t_RecyCleUI["ExpFont"].frame:SetText("经验："..nExp);
		t_RecyCleUI["MoneyFont"].frame:SetText(szMonType..nMoney);
		local moneyparent = getglobal(t_RecyCleUI["MoneyFont"].frame:GetParent());
		local nMoneyFontWidth = t_RecyCleUI["MoneyFont"].frame:GetTextExtentWidth( t_RecyCleUI["MoneyFont"].frame:GetText() )/GetScreenScaleY();
		t_RecyCleUI["ExpBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 0, nPointY );
		t_RecyCleUI["MoneyBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 162, nPointY );
		t_RecyCleUI["MoneyTexBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 167+nMoneyFontWidth, nPointY+nOffset );
		nPointY = nPointY + 20;
		t_RecyCleUI["MoneyTexBtn"].frame:Show();
	else
		t_RecyCleUI["ExpFont"].frame:SetText("");
		t_RecyCleUI["MoneyFont"].frame:SetText("");
		t_RecyCleUI["MoneyTexBtn"].frame:Hide();
	end

	local szAwardDesign = Quest:getAwardDesignation(nQuestId,nRecycleID,true);	--获取任务奖励称号
	local szAwardRepact = Quest:getAwardRespactName(nQuestId,nRecycleID,true);	--获取任务奖励声望名
	local nAwardRepact  = Quest:getAwardRespactValue(nQuestId,nRecycleID,true);--获取任务奖励声望值
	
	if szAwardDesign ~= "" and szAwardRepact == "" then
		t_RecyCleUI["DesiFont"].frame:SetText("称号："..szAwardDesign);
		t_RecyCleUI["RespactFont"].frame:SetText("");
		t_RecyCleUI["DesiBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 0, nPointY);	
	elseif szAwardDesign == "" and szAwardRepact ~= "" then 
		t_RecyCleUI["RespactFont"].frame:SetText("声望：+"..nAwardRepact..szAwardRepact);
		t_RecyCleUI["DesiFont"].frame:SetText("");
		t_RecyCleUI["RespactBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 0, nPointY);
	elseif szAwardDesign ~= "" and szAwardRepact ~= "" then 
		t_RecyCleUI["RespactFont"].frame:SetText("声望：+"..nAwardRepact..szAwardRepact);
		t_RecyCleUI["DesiFont"].frame:SetText("称号："..szAwardDesign);
		t_RecyCleUI["RespactBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 0, nPointY);
		t_RecyCleUI["DesiBtn"].frame:SetPoint("bottomleft", szPartionBtn, "bottomleft", 162, nPointY);
	else
		t_RecyCleUI["RespactFont"].frame:SetText("");
		t_RecyCleUI["DesiFont"].frame:SetText("");
	end
end

--接受任务
function TaskAcceptFrame_AcceptBtn_OnClick()
	if TaskAcceptFrame:GetClientString() == "道具领取任务" then
		local nListType = TaskAcceptFrame:GetClientUserData(2);
		local nGridIdx	= TaskAcceptFrame:GetClientUserData(3);
		local MainPlayer	= ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end

		local container		= MainPlayer:getContainer();
		container:usedItem( nListType, nGridIdx );
		TaskAcceptFrame:Hide();
		TaskAcceptFrame:SetClientString("");
		return;
	elseif TaskAcceptFrame:GetClientString() == "道具领取环组" then
		local nTaskId = TaskAcceptFrame:GetClientUserData(1);
		local nListType = CONTAINER_TYPE_SUNDRIES;
		local nGridIdx	= TaskAcceptFrame:GetClientUserData(3);
		Quest:AcceptRecycleTaskByItem( nListType, nGridIdx, nTaskId );
		TaskAcceptFrame:Hide();
		TaskAcceptFrame:SetClientString("");
		return;
	end
	
	local nNpcId	= t_TaskAcceptFrameControl:getNpcID();			--NPCId
	local nQuestId	= t_TaskAcceptFrameControl:getTaskID();			--任务Id

	if Quest:getQuestType( nQuestId ) == CLIENT_TASK_TYPE_DART_CAR then		
		local ConsumeMoney = Quest:getConvoyQuestMoney( nQuestId );
		if ConsumeMoney ~= nil and ConsumeMoney.Val > 0 then
			TaskAcceptFrame:SetClientUserData( 1, 0 );
			TaskAcceptFrame:Hide();
			MessageBox("","商运需要支付押金" .. ConsumeMoney.Val .. GetMoneyDesc( ConsumeMoney.Type ) );
			MessageBoxFrame:SetClientString( "商运押金" )
		--	print( nNpcId,nNpcId )
			MessageBoxFrame:SetClientUserData( 0, nNpcId );
			MessageBoxFrame:SetClientUserData( 1, nQuestId );
			util.testNpcDialogDistance( nNpcId, MessageBoxFrame )
			return
		end
	end

	TaskAcceptFrame:SetClientUserData( 1, 0 );
	TaskAcceptFrame:Hide();
	Quest:acceptQuest( nNpcId, nQuestId );
	-- 若是通过last选项弹出的面板则不要发CLOSE消息
	-- TODO: NPC对话流程不改
	--[[
	if NpcDialog:getTalkState() == BEGIN_DIALOG then
		NpcDialog:send_CloseTalk();
	end
	--]]
	
	-- 若不是通过last选项弹出的面板则要发CLOSE消息

	-- 若是第一次接受任务，则要关闭接受闪烁特效
	local xinShouBtn = getglobal( "TaskAcceptFrame_AcceptBtnXinShouBtn" );
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == TaskAcceptFrame_AcceptBtn:GetName() then
		xinShouBtn:Hide();
	end
end

function TaskAcceptFrame_LeaveBtn_OnClick()
	if TaskAcceptFrame:GetClientString() == "道具领取任务" or TaskAcceptFrame:GetClientString() == "道具领取环组" then
		TaskAcceptFrame:Hide();
		TaskAcceptFrame:SetClientString("");
	else
		local nNpcID	= t_TaskAcceptFrameControl:getNpcID();
		local nQuestID	= t_TaskAcceptFrameControl:getTaskID();
		if nNpcID == 0 and nQuestID ~= 0 then
			Quest:refuseQuest( nQuestID );
			TaskAcceptFrame:Hide();
		else
			Close_NpcDialog("TaskAcceptFrame");
		end
	end
end

--未完成任务界面相关操作
function TaskNotFinishFrame_OnLoad()
	this:RegisterEvent("GE_NPC_ANSWER_END");
	this:RegisterEvent("UI_ACTOR_LEVEL_CHG");

	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskNotFinishFrameItemBtn"..i.."AwardItem");
		btn:SetClientUserData( 0, DRAG_AWARDITEM );
	end
end

function UpdateTaskNotFinishFrameMoneAndExp()
	local nQuestId		= TaskNotFinishFrame:GetClientUserData(1);	--任务Id
	local nRecycleID	= TaskNotFinishFrame:GetClientUserData(2);
	local tagTaskMoney	= Quest:getQuestMoney(nQuestId, nRecycleID);	--获取任务奖励金钱
	local nMoney		= tagTaskMoney.Val;
	local nType			= tagTaskMoney.Type;
	
	local szMonType = "";
	if nType == ATTR_ID_MONEY then
		szMonType = ""..MONEY_TYPE_MONEY_NAME.."：";
	elseif nType == ATTR_ID_BULL then
		szMonType = ""..MONEY_TYPE_BULL_NAME.."：";
	elseif nType == ATTR_ID_BINDGODCOIN then
		szMonType = ""..MONEY_TYPE_BIND_GODCOIN_NAME.."：";
	elseif nType == ATTR_ID_BATTLE_GLORY then
		szMonType = "荣誉："
	elseif nType == ATTR_ID_ZHENGYI then 
		szMonType = "正义点："
	elseif nType == ATTR_ID_AOSHAN_BRAND then 
		szMonType = "战魂徽章："
	elseif nType == ATTR_ID_LIFT_BRAND then 
		szMonType = "生存奖章："
	elseif nType == ATTR_ID_SCHOOL_VAL then 
		szMonType = "传承点："
	end
	
	local nExp = Quest:getQuestExp( nQuestId, nRecycleID );	-- 获取任务奖励经验
	local nTaskType = Quest:getQuestType( nQuestId );
	local t_fonts = {	["TaskNotFinishFrameExpFont"] = { value = ( nExp <= 0 )	and "" or ( "经验："..nExp )	},
						["TaskNotFinishFrameMoneyFont"]	= { value = ( nMoney <= 0 ) and "" or ( szMonType..nMoney ) },
						--["TaskNotFinishFrameExpFont"]	= { value = "金券：1000" },
						};
	util.SetFonts( t_fonts );
end

function TaskNotFinishFrame_OnEvent()
	if ( arg1 == "GE_NPC_ANSWER_END" ) and this:IsShown() then
		this:Hide();
		NpcDialog:send_CloseTalk();
	end

	if ( arg1 == "UI_ACTOR_LEVEL_CHG" ) then
		if TaskNotFinishFrame:IsShown() then
			UpdateTaskNotFinishFrameMoneAndExp();
		end
	end
end

function ReSetTaskNotFinishMember()
	TaskAcceptFrame_DlgTextRich:Clear();
	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskNotFinishFrameItemBtn"..i);
		local itemfont = getglobal("TaskNotFinishFrameItemBtn"..i.."AwardItemFont");
		btn:Hide();
		itemfont:Hide();
	end
end

function ClearTaskNotFinishFrame()
	local t_fonts = { ["TaskNotFinishScrollFrameTaskAwardBtnExtraAwardFont"]	= { value = "", }, };
	util.SetFonts( t_fonts );
end


function TaskNotFinishFrame_OnShow()
	ClearTaskNotFinishFrame();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	-- 重置窗口坐标
	TaskNotFinishFrame:SetPoint("topleft", "$parent", "topleft",NpcTalkFrame:GetRealLeft(), NpcTalkFrame:GetRealTop());
	local scrollFrame = getglobal( "TaskNotFinishScrollFrame" );
	scrollFrame:SetValueStep( 5 );
	scrollFrame:SetVerticalScroll( 0 );

	TaskNotFinishFrame_DlgTextRich:Clear();
	ReSetTaskNotFinishMember();
	local nNpcId		= this:GetClientUserData(0);			--NPCId
	local nQuestId		= this:GetClientUserData(1);			--任务Id
	local nRecycleID	= this:GetClientUserData(2);			--任务Id
	local szDlgText		= this:GetClientString();			--接取对话内容
	local szDes			= Quest:getQuestDes(nQuestId);			--任务描述
	szDes				= ReplaceConsumeMoneyDesc( szDes, nQuestId, nRecycleID );
	
	--设置当远离npc的时候面板关闭
	util.testNpcDialogDistance( nNpcId, TaskNotFinishFrame );
	
	local npc = ActorMgr:FindActor(nNpcId);
	local szName = npc:getName();						--NPC名称
	local nItemNum = Quest:getQuestItemNum( nQuestId, nRecycleID );	--获取任务奖励物品数量
	local nIndexItem = 0;
	TaskNotFinishFrameItemGetDesc:SetText("");
	local rich = getglobal("TaskNotFinishFrame_DlgTextRich");
	rich:SetFontType(FONTSYTLE_NORMAL);
	rich:SetText( ReplaceQuestDesc( szDlgText ),88,73,53);
	rich:AddText("\n#c6D4A1D任务要求:#n\n"..ReplaceQuestDesc( szDes ),88,73,53);
	
	local nHeight = rich:GetTotalHeight();
	rich:SetHeight( nHeight / GetScreenScaleY() );
	rich:resizeRichHeight( nHeight / GetScreenScaleY() );
	rich:ScrollFirst();

	TaskNotFinishScrollFramePartionBtn:SetPoint("BOTTOMLEFT", "TaskNotFinishFrame_DlgTextRich", "BOTTOMLEFT", 0, 20);
	TaskNotFinishScrollFrameTaskAwardBtn:SetPoint( "TOPLEFT","TaskNotFinishFrame_DlgTextRich","BOTTOMLEFT", 0, 30 );
	
	local nOffestX	= 40;
	for i = 1, CLIENT_MAX_TASK_AWARD_ITEM do
		if nIndexItem > 6 then
			return;
		end
		local nItemId = Quest:getQuestItem( nQuestId, nRecycleID, i - 1 );
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			local nItemCount = Quest:getQuestItemCount( nQuestId, nRecycleID, i - 1 );
			--获取任务奖励物品所需要职业
			if Quest:isQuestItemCareer(nQuestId, nRecycleID, i - 1, MainPlayer:getRace()) then
				nIndexItem = nIndexItem +1;
				local parentbtn = getglobal( "TaskNotFinishFrameItemBtn"..nIndexItem );
				local btn		= getglobal(  parentbtn:GetName().."AwardItem" );
				local tex		= getglobal( parentbtn:GetName().."AwardItemIconTexture" );

				local itemtex		= getglobal( parentbtn:GetName().."AwardItemTex" );
				local itemfont		= getglobal( parentbtn:GetName().."AwardItemFont" );
				local itembacktex	= getglobal( parentbtn:GetName().."AwardItemBackTexture" );		 
				local itemCountFont = getglobal( parentbtn:GetName().."AwardItemCountFont" );	 
				local itemUVTex		= getglobal( parentbtn:GetName().."AwardItemUVAnimationTex" );
				itemCountFont:SetPoint( "bottomleft", parentbtn:GetName(), "bottomleft", 0, 10 );
				itemCountFont:SetText("");
				itembacktex:Show();

				parentbtn:Show();
				itemtex:Show();
				itemfont:Show();
				tex:Show();
				btn:Show();
				btn:SetClientUserData( 1, nItemId );
				
				if nIndexItem%2 == 0 then
					parentbtn:SetPoint("TOPLEFT","TaskNotFinishScrollFramePartionBtn","BOTTOMLEFT", 160, nOffestX + 15+(math.ceil(nIndexItem/2)-1)*50);
					itemfont:SetPoint("TOPLEFT","TaskNotFinishScrollFramePartionBtn","BOTTOMLEFT", 210, nOffestX + 20+(math.ceil(nIndexItem/2)-1)*50);
				else
					parentbtn:SetPoint("TOPLEFT","TaskNotFinishScrollFramePartionBtn","BOTTOMLEFT", 0,	nOffestX + 15+(math.ceil(nIndexItem/2)-1)*50);
					itemfont:SetPoint("TOPLEFT","TaskNotFinishScrollFramePartionBtn","BOTTOMLEFT", 50, nOffestX + 20+(math.ceil(nIndexItem/2)-1)*50);
				end

				local itemdef = getItemDef( nItemId );
				SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());

				local szItemName = itemdef.Name;
				if nItemCount > 1 then
					szItemName = szItemName.."\n数量："..nItemCount;
				end					
				itemfont:SetText( szItemName, 57, 48, 34 );

				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				tex:SetTexture( IconPath )
				tex:SetTexRelUV( 1, 1 );
				itemUVTex:SetTexRelUV( 1, 1 );

				TaskNotFinishScrollFrameTaskItemGetDescBtn:SetPoint( "TOPLEFT","TaskNotFinishScrollFramePartionBtn","BOTTOMLEFT", 0, nOffestX - 10 );

				local nAwardType = Quest:getAwardType( nQuestId, nRecycleID );		--获取道具奖励类型
				if nAwardType == AWARD_TYPE_ALL or nAwardType == 4 then
					TaskNotFinishFrameItemGetDesc:SetText("你将得到：");
				elseif nAwardType == AWARD_TYPE_ONE then
					TaskNotFinishFrameItemGetDesc:SetText("该任务奖励其中一件物品：");
				elseif nAwardType == AWARD_TYPE_RAND then
					TaskNotFinishFrameItemGetDesc:SetText("系统将从以下奖励物品随机选择一件：");
				end
			end
		end
	end

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end

	TaskNotFinishFrameNpcNameFont:SetText(szName);

	local nPointY = nOffestX + 83+(math.ceil(nIndexItem/2)-1)*50;
	if nIndexItem == 0 then
		nPointY =  nOffestX +8;
	end

	local tagTaskMoney = Quest:getQuestMoney( nQuestId, nRecycleID );	--获取任务奖励金钱
	local nMoney		= tagTaskMoney.Val;
	local nType			= tagTaskMoney.Type;
	local nOffset		= 0;
	
	local szMonType = "";
	local texPath = "uires\\ZhuJieMian\\1.dds"
	if nType == ATTR_ID_MONEY then
		TaskNotFinishFrameMoneySignTex:SetTexture(texPath);
		TaskNotFinishFrameMoneySignTex:SetTexUV(117,494,17,18);
		TaskNotFinishFrameMoneySignTex:SetSize(17,18);
		nOffset = nOffset;
		szMonType = ""..MONEY_TYPE_MONEY_NAME.."：";
	elseif nType == ATTR_ID_BULL then
		TaskNotFinishFrameMoneySignTex:SetTexture(texPath);
		TaskNotFinishFrameMoneySignTex:SetTexUV(78,494,20,18);
		TaskNotFinishFrameMoneySignTex:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BULL_NAME.."：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BINDGODCOIN then
		TaskNotFinishFrameMoneySignTex:SetTexture(texPath);
		TaskNotFinishFrameMoneySignTex:SetTexUV(57,494,20,18);
		TaskNotFinishFrameMoneySignTex:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BIND_GODCOIN_NAME.."：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BATTLE_GLORY then
		TaskNotFinishFrameMoneySignTex:SetTexture(texPath);
		TaskNotFinishFrameMoneySignTex:SetTexUV(963,203,16,18);
		TaskNotFinishFrameMoneySignTex:SetSize(16,18);
		szMonType = "荣誉：";
		nOffset = nOffset + 1;	
	elseif nType == ATTR_ID_ZHENGYI then
		SetTaskAwardTexture( "TaskNotFinishFrameMoneySignTex",nType) ;
		szMonType = "正义点：";
		nOffset = nOffset + 1;	
	elseif nType == ATTR_ID_AOSHAN_BRAND then
		SetTaskAwardTexture( "TaskNotFinishFrameMoneySignTex",nType) ;
		szMonType = "战魂徽章：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_LIFT_BRAND then
		SetTaskAwardTexture( "TaskNotFinishFrameMoneySignTex",nType) ;
		szMonType = "生存奖章：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_SCHOOL_VAL then
		SetTaskAwardTexture( "TaskNotFinishFrameMoneySignTex",nType) ;
		szMonType = "传承点：";
		nOffset = nOffset + 1;
	end
	
	local nExp		= Quest:getQuestExp( nQuestId, nRecycleID );	-- 获取任务奖励经验
	local nTaskType = Quest:getQuestType( nQuestId );
	local fScale	= 1;
	if nTaskType == CLIENT_TASK_TYPE_CLAN and Quest:isRecycleTask( nQuestId ) then
		local nFiniRecycleIdx	= Quest:getReCycleQusetOneDayNum( nRecycleID );
		--fScale = fScale * CLAN_AWARD_ONE_TIME_SCALE ^ nFiniRecycleIdx;
		--print( "TaskNotFinishFrame_OnShow -- nFiniRecycleIdx = "..nFiniRecycleIdx.."--2197" );
	end

	if nExp > 0 then
		nExp = math.floor( nExp * fScale );
	end

	if nMoney > 0 then
		nMoney = math.floor( nMoney * fScale );
	end

	local t_fonts = {	["TaskNotFinishFrameExpFont"]	= { value = ( nExp <= 0 )	and "" or ( "经验："..nExp )	},
						["TaskNotFinishFrameMoneyFont"]	= { value = ( nMoney <= 0 ) and "" or ( szMonType..nMoney ) },
					};
	util.SetFonts( t_fonts );

	if tonumber( nMoney ) <= 0 and tonumber( nExp ) > 0  then
		TaskNotFinishScrollFrameTaskExpBtn:SetPoint("bottomleft", "TaskNotFinishScrollFramePartionBtn", "bottomleft", 0, nPointY );
		TaskNotFinishScrollFrameTaskMoneyTexBtn:Hide();
	elseif tonumber( nMoney ) > 0 and tonumber( nExp ) <= 0 then
		local nMoneyFontWidth = TaskNotFinishFrameMoneyFont:GetTextExtentWidth( TaskNotFinishFrameMoneyFont:GetText() )/GetScreenScaleY();
		TaskNotFinishScrollFrameTaskMoneyBtn:SetPoint("bottomleft", "TaskNotFinishScrollFramePartionBtn", "bottomleft", 0, nPointY );	 
		TaskNotFinishScrollFrameTaskMoneyTexBtn:SetPoint("bottomleft", "TaskNotFinishScrollFramePartionBtn", "bottomleft", nMoneyFontWidth, nPointY+nOffset );
		TaskNotFinishScrollFrameTaskMoneyTexBtn:Show();
	elseif  tonumber( nMoney ) > 0 and tonumber( nExp ) > 0 then
		local nMoneyFontWidth = TaskNotFinishFrameMoneyFont:GetTextExtentWidth( TaskNotFinishFrameMoneyFont:GetText() )/GetScreenScaleY();
		TaskNotFinishScrollFrameTaskExpBtn:SetPoint("bottomleft", "TaskNotFinishScrollFramePartionBtn", "bottomleft", 0, nPointY );
		TaskNotFinishScrollFrameTaskMoneyBtn:SetPoint("bottomleft", "TaskNotFinishScrollFramePartionBtn", "bottomleft", 162, nPointY );
		TaskNotFinishScrollFrameTaskMoneyTexBtn:SetPoint("bottomleft", "TaskNotFinishScrollFramePartionBtn", "bottomleft", 167+nMoneyFontWidth, nPointY+nOffset );
		TaskNotFinishScrollFrameTaskMoneyTexBtn:Show();
	else
		TaskNotFinishScrollFrameTaskMoneyTexBtn:Hide();
		nPointY = nPointY - 20;
	end
	nPointY = nPointY + 20;

	local szAwardDesign		= Quest:getAwardDesignation(nQuestId,nRecycleID);				--获取任务奖励称号
	local szAwardRepact		= Quest:getAwardRespactName(nQuestId,nRecycleID);				--获取任务奖励声望名
	local nAwardRepact		= Quest:getAwardRespactValue(nQuestId,nRecycleID);				--获取任务奖励声望值
	local szSecondarySkill	= Quest:getAwardSecondarySkillName(nQuestId);
	local nClanMoney		= math.floor( Quest:getQuestClanMoney( nQuestId, nRecycleID ) );		--公会资金
	local nClanAnDing		= math.floor( Quest:getQuestClanAnDing(nQuestId) * fScale );		--公会安定度
	local nClanContribute	= math.floor( Quest:getQuestClanContribute(nQuestId, nRecycleID) * fScale );	--个人贡献度
	local nClanTechPoint	= math.floor( Quest:getQuestClanTechPoint(nQuestId) * fScale );	-- 公会科技点

	local szBtnRef			= "TaskNotFinishScrollFramePartionBtn";
	local DesignationFont	= getglobal("TaskNotFinishFrameDesignationFont");
	local DesignationBtn	= getglobal("TaskNotFinishScrollFrameTaskDesignationBtn");
	local RespactFont		= getglobal("TaskNotFinishFrameRespactFont");
	local RespactBtn		= getglobal("TaskNotFinishScrollFrameTaskRespactBtn");
	local secondaryFont		= getglobal("TaskNotFinishFrameSecondaryFont");
	local secondaryBtn		= getglobal("TaskNotFinishScrollFrameTaskSecondarySkillBtn");
	local ClanMoneyFont		= getglobal("TaskNotFinishFrameClanMoneyFont");
	local ClanMoneyBtn		= getglobal("TaskNotFinishScrollFrameTaskClanMoneyBtn");
	local ClanAnDingFont	= getglobal("TaskNotFinishFrameClanAnDingFont");
	local ClanAnDingBtn		= getglobal("TaskNotFinishScrollFrameTaskClanAnDingBtn");
	local ClanContributeFont= getglobal("TaskNotFinishFrameClanContributeFont");
	local ClanContributeBtn	= getglobal("TaskNotFinishScrollFrameTaskClanContributeBtn");
	local ClanTechPointFont= getglobal("TaskNotFinishFrameClanTechPointFont");
	local ClanTechPointBtn	= getglobal("TaskNotFinishScrollFrameTaskClanTechPointBtn");

	if szAwardDesign ~= "" and szAwardRepact == "" then
		DesignationFont:SetText("称号："..szAwardDesign);
		RespactFont:SetText("");
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	elseif szAwardDesign == "" and szAwardRepact ~= "" then 
		RespactFont:SetText("声望：+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("");
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	elseif szAwardDesign ~= "" and szAwardRepact ~= "" then 
		RespactFont:SetText("声望：+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("称号："..szAwardDesign);
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 162, nPointY);
	else
		RespactFont:SetText("");
		DesignationFont:SetText("");
	end

	if szSecondarySkill ~= "" then
		nPointY = nPointY + 20;
		if Quest:getAwardSecondarySkillVal( nQuestId ) > 0 then
			secondaryFont:SetText("生活科技熟练度：".. math.ceil( Quest:getAwardSecondarySkillVal( nQuestId ) / 100 ) );
		else
			secondaryFont:SetText("科技生产："..szSecondarySkill);
		end
		secondaryBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	else
		secondaryFont:SetText("");
	end

	local nPointX = 0;
	if nClanMoney ~= 0 then
		nPointY = nPointY + 20;
		ClanMoneyFont:SetText("公会资金："..nClanMoney);
		ClanMoneyBtn:SetPoint( "bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		nPointX = 162;
	else
		ClanMoneyFont:SetText("");
	end

	if nClanAnDing ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanAnDingFont:SetText("公会安定度："..nClanAnDing);
		ClanAnDingBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanAnDingFont:SetText("");
	end

	if nClanContribute ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanContributeFont:SetText("个人贡献度："..nClanContribute);
		ClanContributeBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end			
	else
		ClanContributeFont:SetText("");
	end

	if nClanTechPoint ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanTechPointFont:SetText("公会科技点："..nClanTechPoint);
		ClanTechPointBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanTechPointFont:SetText("");
	end
	
	local ResourceClass = 0
	for id, num in TaskClanResourceAwards(nQuestId) do
		num = math.floor(num * fScale)
		if id ~= 0 and num ~= 0 then
			ResourceClass = ResourceClass + 1
			local btn = getglobal("TaskNotFinishScrollFrameTaskResource"..ResourceClass)
			if not btn then
				break
			end
			local text = getglobal("TaskNotFinishScrollFrameTaskResource"..ResourceClass.."Text")
			if nPointX == 0 then
				nPointY = nPointY + 20
			end
			text:SetText(string.format("%s：%d", GameClanManager:getProductDefByID(id).Name, num))
			btn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY)
			if nPointX > 0 then
				nPointX = 0
			else
				nPointX = 162
			end
		end
	end
	while true do
		ResourceClass = ResourceClass + 1
		local text = getglobal("TaskNotFinishScrollFrameTaskResource"..ResourceClass.."Text")
		if not text then
			break
		end
		text:SetText("")
	end

	SetTaskRecycleItem( nQuestId, nPointY, "TaskNotFinishFrame", "TaskNotFinishScrollFrame", "TaskNotFinishFrame_DlgTextRich", nRecycleID );
	scrollFrame:ResetChildAnchor();
	SetScrollBar( "TaskNotFinishFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	if scrollFrame:GetVerticalScrollRange() > 0 then
		TaskNotFinishFrame_ScrollBackGroundTexCenter:Show();
		TaskNotFinishFrame_ScrollBackGroundTexTop:Show();
		TaskNotFinishFrame_ScrollBackGroundTexBottom:Show();
	else
		TaskNotFinishFrame_ScrollBackGroundTexCenter:Hide();
		TaskNotFinishFrame_ScrollBackGroundTexTop:Hide();
		TaskNotFinishFrame_ScrollBackGroundTexBottom:Hide();
	end
	TaskNotFinishFrame_ScrollBar:SetValue(0);
end

--领取任务奖励框相关
function TaskAwardFrame_OnLoad()
	this:RegisterEvent("GE_NPC_ANSWER_END");
	local slier = getglobal("TaskAwardFrame_ScrollBar");
	slier:SetMinValue(0);
	slier:SetValueStep(1);
	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskAwardFrameItemBtn"..i.."AwardItem");
		btn:SetClientUserData( 0, DRAG_AWARDITEM );
	end
end

function TaskAwardFrame_OnEvent()
	if ( arg1 == "GE_NPC_ANSWER_END" ) then
		if this:IsShown() then
			this:Hide();
			NpcDialog:send_CloseTalk();
		end
	end
end

function ReSetTaskAwardMember()
	TaskAwardFrame_DlgTextRich:Clear();
	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskAwardFrameItemBtn"..i);
		local itemfont = getglobal("TaskAwardFrameItemBtn"..i.."AwardItemFont");
		btn:Hide();
		itemfont:Hide();
	end
end

function ClearTaskAwardFrame()
	local t_fonts = { ["TaskAwardScrollFrameTaskAwardBtnExtraAwardFont"]	= { value = "", }, };
	util.SetFonts( t_fonts );
end

function TaskAwardFrame_OnShow()   
	ClearTaskAwardFrame();
	local MainPlayer = ActorMgr:getMainPlayer();
	FirstClickFinishTask( this:GetClientUserData( 1 ) );
	-- 重置窗口坐标
	TaskAwardFrame:SetPoint("topleft", "$parent", "topleft",NpcTalkFrame:GetRealLeft(), NpcTalkFrame:GetRealTop());
	local scrollFrame = getglobal( "TaskAwardScrollFrame" );
	scrollFrame:SetValueStep( 5 );
	scrollFrame:SetVerticalScroll( 0 );
	
	ReSetTaskAwardMember();
	nSelectAwardId = 0;
	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskAwardFrameItemBtn"..i );
		local Tex = getglobal("TaskAwardFrameItemBtn"..i.."AwardItemTex" );
		btn:DisChecked();
		Tex:ChangeTextureTemplate("ItemNameNormalTemplate");
	end
	--print( "TaskAwardFrame_OnShow--开始--2264" );
	local nNpcId		= this:GetClientUserData(0);			--NPCId
	local nQuestId		= this:GetClientUserData(1);			--任务Id
	local nRecycleID	= this:GetClientUserData(2);
	local szDlgText		= this:GetClientString();				--接取对话内容
	local szDes		= Quest:getQuestDes(nQuestId);			--任务描述
	local szTaskName	= Quest:getQuestName(nQuestId);			--获取任务名称
	szDes = ReplaceConsumeMoneyDesc( szDes, nQuestId, nRecycleID );
	--获取任务未完成任务追踪提示
	local nExp	= Quest:getQuestExp(nQuestId, nRecycleID);			--获取任务奖励经验
	if nNpcId > 0 then
		local npc	= ActorMgr:FindActor(nNpcId);
		if npc == nil then return end
	end
	--local szName = npc:getName();					--NPC名称
	local rich = getglobal("TaskAwardFrame_DlgTextRich");
	rich:SetFontType(FONTSYTLE_NORMAL);
	rich:SetText(ReplaceQuestDesc( "#c982e02"..szTaskName.."#n\n"..szDlgText ),88,73,53);
	--rich:AddText("\n#c6D4A1D任务要求:#n\n"..ReplaceQuestDesc( szDes ),88,73,53);
	local nHeight = rich:GetTotalHeight();
	rich:SetHeight( nHeight / GetScreenScaleY() );

	TaskAwardScrollFramePartionBtn:SetPoint("BOTTOMLEFT", "TaskAwardFrame_DlgTextRich", "BOTTOMLEFT", 0, 20);
	TaskAwardScrollFrameTaskAwardBtn:SetPoint( "TOPLEFT","TaskAwardFrame_DlgTextRich","BOTTOMLEFT", 0, 30 );
	local nItemNum = Quest:getQuestItemNum( nQuestId, nRecycleID );
	local nIndexItem = 0;
	local nOffestX  = 40;
	TaskAwardFrameItemGetDesc:SetText("");
	for i = 1, CLIENT_MAX_TASK_AWARD_ITEM do
		if nIndexItem > 6 then
			return;
		end
		local nItemId = Quest:getQuestItem( nQuestId, nRecycleID, i-1 );
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			local nItemCount = Quest:getQuestItemCount( nQuestId, nRecycleID, i-1 );
			--获取任务奖励物品所需要职业
			if Quest:isQuestItemCareer(nQuestId, nRecycleID, i - 1, MainPlayer:getRace()) then
				nIndexItem = nIndexItem + 1;
				local parentbtn			= getglobal( "TaskAwardFrameItemBtn"..nIndexItem );
				local btn				= getglobal( parentbtn:GetName().."AwardItem" );
				local itemtex			= getglobal( parentbtn:GetName().."AwardItemTex" ); 
				local tex				= getglobal( parentbtn:GetName().."AwardItemIconTexture" );
				local itemfont			= getglobal( parentbtn:GetName().."AwardItemFont" );
				local itembacktex		= getglobal( parentbtn:GetName().."AwardItemBackTexture" );		 
				local itemCountFont		= getglobal( parentbtn:GetName().."AwardItemCountFont" );	 
				local itemUVTex			= getglobal( parentbtn:GetName().."AwardItemUVAnimationTex" );
				itemCountFont:SetPoint( "bottomleft", parentbtn:GetName(), "bottomleft", 0, 10 );
				itemCountFont:SetText("");
		
				itembacktex:Show();
				parentbtn:Show();
				itemtex:Show();		
				itemfont:Show();
				btn:Show();
				btn:SetClientUserData( 1, nItemId );
				
				if nIndexItem%2 == 0 then
					parentbtn:SetPoint("TOPLEFT","TaskAwardScrollFramePartionBtn","BOTTOMLEFT", 160,	nOffestX +15+(math.ceil(nIndexItem/2)-1)*50);
					itemfont:SetPoint("TOPLEFT","TaskAwardScrollFramePartionBtn","BOTTOMLEFT", 210, nOffestX +20+(math.ceil(nIndexItem/2)-1)*50);
				else
					parentbtn:SetPoint("TOPLEFT","TaskAwardScrollFramePartionBtn","BOTTOMLEFT", 10,	nOffestX +15+(math.ceil(nIndexItem/2)-1)*50);
					itemfont:SetPoint("TOPLEFT","TaskAwardScrollFramePartionBtn","BOTTOMLEFT", 60, nOffestX +20+(math.ceil(nIndexItem/2)-1)*50);
				end

				local itemdef = getItemDef( nItemId );				  			
				SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());

				local szItemName = itemdef.Name;
				if nItemCount > 1 then
					szItemName = szItemName.."\n数量："..nItemCount;
				end					
				itemfont:SetText( szItemName, 57, 48, 34 );

				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				tex:SetTexture( IconPath );
				tex:SetTexRelUV( 1, 1 );
				
				TaskAwardScrollFrameTaskItemGetDescBtn:SetPoint( "TOPLEFT","TaskAwardScrollFramePartionBtn","BOTTOMLEFT", 0, nOffestX - 10 );
				local nAwardType = Quest:getAwardType(nQuestId, nRecycleID);		--获取道具奖励类型
				if nAwardType == AWARD_TYPE_ALL or nAwardType == 4 then
					TaskAwardFrameItemGetDesc:SetText("你将得到：");
				elseif nAwardType == AWARD_TYPE_ONE then
					TaskAwardFrameItemGetDesc:SetText("请从以下奖励物品选择一个：");
				elseif nAwardType == AWARD_TYPE_RAND then
					TaskAwardFrameItemGetDesc:SetText("系统将从以下奖励物品随机选择一个：");
				end
			end
		end
	end

	
	local nPointY = nOffestX +83+(math.ceil(nIndexItem/2)-1)*50;
	if nIndexItem == 0 then
		nPointY = nOffestX +8;
	end

	local tagTaskMoney	= Quest:getQuestMoney( nQuestId, nRecycleID );		--获取任务奖励金钱
	local nMoney		= tagTaskMoney.Val;
	local nType			= tagTaskMoney.Type;
	local nOffset		= 0;

	local fScale	= 1;
	local nTaskType = Quest:getQuestType( nQuestId );
	if nTaskType == CLIENT_TASK_TYPE_CLAN and Quest:isRecycleTask( nQuestId ) then
		local nFiniRecycleIdx	= Quest:getReCycleQusetOneDayNum( nRecycleID );
		--fScale = fScale * CLAN_AWARD_ONE_TIME_SCALE ^ nFiniRecycleIdx;
		--print( "TaskAwardFrame_OnShow -- nFiniRecycleIdx = "..nFiniRecycleIdx.."--2501" );
	end

	if nExp > 0 then
		nExp = math.floor( nExp * fScale );
	end

	if nMoney > 0 then
		nMoney = math.floor( nMoney * fScale );
	end
	local texPath = "uires\\ZhuJieMian\\1.dds"
	if nType == ATTR_ID_MONEY then
		TaskAwardFrameMoneySignTex:SetTexture(texPath);
		TaskAwardFrameMoneySignTex:SetTexUV(117,494,17,18);
		TaskAwardFrameMoneySignTex:SetSize(17,18);
		szMonType = ""..MONEY_TYPE_MONEY_NAME.."：";
		nOffset = nOffset;
	elseif nType == ATTR_ID_BULL then
		TaskAwardFrameMoneySignTex:SetTexture(texPath);
		TaskAwardFrameMoneySignTex:SetTexUV(78,494,20,18);
		TaskAwardFrameMoneySignTex:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BULL_NAME.."：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BINDGODCOIN then
		TaskAwardFrameMoneySignTex:SetTexture(texPath);
		TaskAwardFrameMoneySignTex:SetTexUV(57,494,20,18);
		TaskAwardFrameMoneySignTex:SetSize(20,18);
		szMonType = ""..MONEY_TYPE_BIND_GODCOIN_NAME.."：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_BATTLE_GLORY then
		TaskAwardFrameMoneySignTex:SetTexture(texPath);
		TaskAwardFrameMoneySignTex:SetTexUV(963,203,16,18);
		TaskAwardFrameMoneySignTex:SetSize(16,18);
		szMonType = "荣誉：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_ZHENGYI then
		SetTaskAwardTexture( "TaskAwardFrameMoneySignTex",nType) ;
		szMonType = "正义点：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_AOSHAN_BRAND then
		SetTaskAwardTexture( "TaskAwardFrameMoneySignTex",nType) ;
		szMonType = "战魂徽章：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_LIFT_BRAND then
		SetTaskAwardTexture( "TaskAwardFrameMoneySignTex",nType) ;
		szMonType = "生存奖章：";
		nOffset = nOffset + 1;
	elseif nType == ATTR_ID_SCHOOL_VAL then
		SetTaskAwardTexture( "TaskAwardFrameMoneySignTex",nType) ;
		szMonType = "传承点：";
		nOffset = nOffset + 1;
	end

	if tonumber( nMoney ) <= 0 and tonumber( nExp ) > 0  then
		TaskAwardFrameMoneyFont:SetText("");
		TaskAwardFrameExpFont:SetText("经验："..nExp);
		TaskAwardScrollFrameTaskExpBtn:SetPoint("bottomleft", "TaskAwardScrollFramePartionBtn", "bottomleft", 0, nPointY );
		TaskAwardScrollFrameTaskMoneyTexBtn:Hide();
	elseif tonumber( nMoney ) > 0 and tonumber( nExp ) <= 0 then
		TaskAwardFrameExpFont:SetText("");
		TaskAwardFrameMoneyFont:SetText(szMonType..nMoney);		
		local nMoneyFontWidth = TaskAwardFrameMoneyFont:GetTextExtentWidth( TaskAwardFrameMoneyFont:GetText() )/GetScreenScaleY();	
		TaskAwardScrollFrameTaskMoneyBtn:SetPoint("bottomleft", "TaskAwardScrollFramePartionBtn", "bottomleft", 0, nPointY );	 
		TaskAwardScrollFrameTaskMoneyTexBtn:SetPoint("bottomleft", "TaskAwardScrollFramePartionBtn", "bottomleft", nMoneyFontWidth, nPointY+nOffset );
		TaskAwardScrollFrameTaskMoneyTexBtn:Show();
	elseif  tonumber( nMoney ) > 0 and tonumber( nExp ) > 0 then
		TaskAwardFrameExpFont:SetText("经验："..nExp);
		TaskAwardFrameMoneyFont:SetText(szMonType..nMoney);
		local nMoneyFontWidth = TaskAwardFrameMoneyFont:GetTextExtentWidth( TaskAwardFrameMoneyFont:GetText() )/GetScreenScaleY();
		TaskAwardScrollFrameTaskExpBtn:SetPoint("bottomleft", "TaskAwardScrollFramePartionBtn", "bottomleft", 0, nPointY );
		TaskAwardScrollFrameTaskMoneyBtn:SetPoint("bottomleft", "TaskAwardScrollFramePartionBtn", "bottomleft", 162, nPointY );
		TaskAwardScrollFrameTaskMoneyTexBtn:SetPoint("bottomleft", "TaskAwardScrollFramePartionBtn", "bottomleft", 167+nMoneyFontWidth, nPointY+nOffset );
		TaskAwardScrollFrameTaskMoneyTexBtn:Show();
	else
		TaskAwardFrameExpFont:SetText("");
		TaskAwardFrameMoneyFont:SetText("");
		TaskAwardScrollFrameTaskMoneyTexBtn:Hide();
		nPointY = nPointY - 20;
	end
	nPointY = nPointY + 20;

	local szAwardDesign		= Quest:getAwardDesignation(nQuestId,nRecycleID);				--获取任务奖励称号
	local szAwardRepact		= Quest:getAwardRespactName(nQuestId,nRecycleID);				--获取任务奖励声望名
	local nAwardRepact		= Quest:getAwardRespactValue(nQuestId,nRecycleID);				--获取任务奖励声望值
	local szSecondarySkill	= Quest:getAwardSecondarySkillName(nQuestId);
	local nClanMoney		= math.floor( Quest:getQuestClanMoney( nQuestId, nRecycleID ) );		--公会资金
	local nClanAnDing		= math.floor( Quest:getQuestClanAnDing(nQuestId) * fScale );		--公会安定度
	local nClanContribute	= math.floor( Quest:getQuestClanContribute(nQuestId, nRecycleID) * fScale );	--个人贡献度
	local nClanTechPoint	= math.floor( Quest:getQuestClanTechPoint(nQuestId) * fScale );	-- 公会科技点

	local szBtnRef			= "TaskAwardScrollFramePartionBtn";
	local DesignationFont	= getglobal("TaskAwardFrameDesignationFont");
	local DesignationBtn	= getglobal("TaskAwardScrollFrameTaskDesignationBtn");
	local RespactFont		= getglobal("TaskAwardFrameRespactFont");
	local RespactBtn		= getglobal("TaskAwardScrollFrameTaskRespactBtn");
	local SeondarySkillFont = getglobal("TaskAwardFrameSecondaryFont");
	local SecondarySkillBtn = getglobal("TaskAwardScrollFrameTaskSecondarySkillBtn");
	local ClanMoneyFont		= getglobal("TaskAwardFrameClanMoneyFont");
	local ClanMoneyBtn		= getglobal("TaskAwardScrollFrameTaskClanMoneyBtn");
	local ClanAnDingFont	= getglobal("TaskAwardFrameClanAnDingFont");
	local ClanAnDingBtn		= getglobal("TaskAwardScrollFrameTaskClanAnDingBtn");
	local ClanContributeFont= getglobal("TaskAwardFrameClanContributeFont");
	local ClanContributeBtn	= getglobal("TaskAwardScrollFrameTaskClanContributeBtn");
	local ClanTechPointFont= getglobal("TaskAwardFrameClanTechPointFont");
	local ClanTechPointBtn	= getglobal("TaskAwardScrollFrameTaskClanTechPointBtn");

	if szAwardDesign ~= "" and szAwardRepact == "" then
		DesignationFont:SetText("称号："..szAwardDesign);
		RespactFont:SetText("");
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);	
	elseif szAwardDesign == "" and szAwardRepact ~= "" then 
		RespactFont:SetText("声望：+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("");
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	elseif szAwardDesign ~= "" and szAwardRepact ~= "" then 
		RespactFont:SetText("声望：+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("称号："..szAwardDesign);
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 162, nPointY);
	else
		RespactFont:SetText("");
		DesignationFont:SetText("");
		nPointY = nPointY - 20;
	end

	if szSecondarySkill ~= "" then
		nPointY = nPointY + 20;
		if Quest:getAwardSecondarySkillVal( nQuestId ) > 0 then
			SeondarySkillFont:SetText("生活科技熟练度：".. math.ceil( Quest:getAwardSecondarySkillVal( nQuestId ) / 100 ) );
		else
			SeondarySkillFont:SetText("科技生产："..szSecondarySkill);
		end
		SecondarySkillBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	else
		SeondarySkillFont:SetText("");
	end
	
	local nPointX = 0;
	if nClanMoney ~= 0 then
		nPointY = nPointY + 20;
		ClanMoneyFont:SetText("公会资金："..nClanMoney);
		ClanMoneyBtn:SetPoint( "bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		nPointX = 162;
	else
		ClanMoneyFont:SetText("");
	end

	if nClanAnDing ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanAnDingFont:SetText("公会安定度："..nClanAnDing);
		ClanAnDingBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanAnDingFont:SetText("");
	end

	if nClanContribute ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanContributeFont:SetText("个人贡献度："..nClanContribute);
		ClanContributeBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanContributeFont:SetText("");
	end

	if nClanTechPoint ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanTechPointFont:SetText("公会科技点："..nClanTechPoint);
		ClanTechPointBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY );
		if nPointX > 0 then
			nPointX = 0;
		else
			nPointX = 162;
		end		
	else
		ClanTechPointFont:SetText("");
	end
	
	local ResourceClass = 0
	for id, num in TaskClanResourceAwards(nQuestId) do
		num = math.floor(num * fScale)
		if id ~= 0 and num ~= 0 then
			ResourceClass = ResourceClass + 1
			local btn = getglobal("TaskAwardScrollFrameTaskResource"..ResourceClass)
			if not btn then
				break
			end
			local text = getglobal("TaskAwardScrollFrameTaskResource"..ResourceClass.."Text")
			if nPointX == 0 then
				nPointY = nPointY + 20
			end
			text:SetText(string.format("%s：%d", GameClanManager:getProductDefByID(id).Name, num))
			btn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY)
			if nPointX > 0 then
				nPointX = 0
			else
				nPointX = 162
			end
		end
	end
	while true do
		ResourceClass = ResourceClass + 1
		local text = getglobal("TaskAwardScrollFrameTaskResource"..ResourceClass.."Text")
		if not text then
			break
		end
		text:SetText("")
	end

	nPointY = nPointY + 20;
	nSelectAwardId = 0;
	SetTaskRecycleItem( nQuestId, nPointY, "TaskAwardFrame", "TaskAwardScrollFrame", "TaskAwardFrame_DlgTextRich", nRecycleID );
	scrollFrame:ResetChildAnchor();
	SetScrollBar( "TaskAwardFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	if scrollFrame:GetVerticalScrollRange() > 0 then
		TaskAwardFrame_ScrollBackGroundTexCenter:Show();
		TaskAwardFrame_ScrollBackGroundTexTop:Show();
		TaskAwardFrame_ScrollBackGroundTexBottom:Show();
	else
		TaskAwardFrame_ScrollBackGroundTexCenter:Hide();
		TaskAwardFrame_ScrollBackGroundTexTop:Hide();
		TaskAwardFrame_ScrollBackGroundTexBottom:Hide();
	end
	TaskAwardFrame_ScrollBar:SetValue(0);
end

function ReplaceConsumeMoneyDesc( szTalk, nQuestId, nRecycleID )
	--print( "ReplaceConsumeMoneyDesc--szTalk = "..szTalk.."--1994\n\n" );
	local ConsumeMoney = Quest:getConvoyQuestMoney( nQuestId );
	local szConsumeDesc = "";
	if ConsumeMoney ~= nil then
		szConsumeDesc = ConsumeMoney.Val.."("..GetMoneyDesc( ConsumeMoney.Type )..")";
	end
	
	local szFiniCount = "";
	local szTotlaCount = "";
	local szLeftCount = "";
	if Quest:getQuestOneDayFiniNum(nQuestId,nRecycleID) ~= -1 then
		szFiniCount = Quest:getQuestOneDayFiniNum(nQuestId,nRecycleID);
	end

	if Quest:getQuestOneDayLimitNum(nQuestId, nRecycleID) ~= 0 then
		szTotlaCount = Quest:getQuestOneDayLimitNum(nQuestId, nRecycleID);
	end

	if szFiniCount~= "" 
		and szTotlaCount ~= "" 
		and tonumber( szTotlaCount ) >= tonumber( szFiniCount ) then
		szLeftCount = tonumber(szTotlaCount ) - tonumber( szFiniCount );
	end
	local t_RepStr = 
		{ 
			["#finicount#"]		= szFiniCount, 
			["#leftcount#"]		= szLeftCount, 
			["#totalcount#"]	= szTotlaCount, 
			["#consume#"]		= szConsumeDesc 
		};
	for szsrc, szdes in pairs(t_RepStr) do
		szTalk = ReplaceStr(szTalk, szsrc, szdes);
	end

	return szTalk;
end

function ReplaceStr( szTalk, szsrc, szdes )
	if ( string.find(szTalk, szsrc, 1, true) ) then
		local nPos			= ( string.find(szTalk, szsrc ) );
		local nLength		= string.len( szTalk );
		local nDesLength	= string.len( szsrc );
		local szText		= string.sub( szTalk, 1, nPos-1 ) .. szdes .. string.sub(szTalk, nPos+nDesLength, nLength );
		szTalk				= szText;
	end
	return szTalk;
end

--设置选中的领取奖励物品的id
function TaskAwardFrameLeftButtonOnClick( nCheckIndex )
	for i = 1, MAX_AWARD_ITEM_NUM do
		local btn = getglobal("TaskAwardFrameItemBtn"..i );
		local Tex = getglobal("TaskAwardFrameItemBtn"..i.."AwardItemTex" );
		btn:DisChecked();
		Tex:ChangeTextureTemplate("ItemNameNormalTemplate");
	end

	local btn		= getglobal( "TaskAwardFrameItemBtn"..nCheckIndex )
	local btnItem	= getglobal( btn:GetName().."AwardItem" );
	local CheckTex	= getglobal( btn:GetName().."AwardItemTex" );
	CheckTex:ChangeTextureTemplate("ItemNameHighTemplate");
	btn:Checked();

	nSelectAwardId = btnItem:GetClientUserData( 1 );
end

--领取奖励
function TaskAwardFrame_GetBtn_OnClick()
	local frame			= getglobal("TaskAwardFrame");
	local nNpcId		= frame:GetClientUserData(0);	--NPCId
	local nQuestId		= frame:GetClientUserData(1);	--任务Id
	local nRecycleID	= frame:GetClientUserData(2);
	local nAwardType	= Quest:getAwardType( nQuestId, nRecycleID );	--获取道具奖励类型

	local Mainplayer	= ActorMgr:getMainPlayer();
	local container		= Mainplayer:getContainer();
	-- 若是第一次接受任务，则要关闭接受闪烁特效
	local xinShouBtn = getglobal( "TaskAwardFrame_GetBtnXinShouBtn" );
	if xinShouBtn:IsShown() and xinShouBtn:GetClientString() == TaskAwardFrame_GetBtn:GetName() then
		xinShouBtn:Hide();
	end
	
	frame:Hide();
	if nAwardType == AWARD_TYPE_NONE then
		Quest:awardQuest( nNpcId, nQuestId );
	elseif nAwardType == AWARD_TYPE_ALL or nAwardType == AWARD_TYPE_RAND or nAwardType == 4 then
		--local nonegrid = container:getNoneGridIndex(CONTAINER_TYPE_SUNDRIES );
		--if nonegrid == -1 then
		--	ShowMidTips("背包已满，无法领取奖励物品");
		--else
			Quest:awardQuest( nNpcId, nQuestId );
		--end
	elseif nAwardType == AWARD_TYPE_ONE then
		if nSelectAwardId == 0 then
			MessageBox( "提示", "请选择要领取的物品" );
			NpcDialog:send_CloseTalk();
		else
			Quest:awardQuest( nNpcId, nQuestId, nSelectAwardId );
		end
	end
end

--任务日志的离开按钮
function TaskLogFrame_LeaveBtn_OnClick()
	local frame = getglobal("TaskLogFrame");
	frame:Hide();
end

function NpcTalkFrame_ScrollBar_OnValueChanged()
	local szName = this:GetName();
	local slier = getglobal("TaskAcceptFrame_ScrollBar");
	local scrollFrame = getglobal("TaskAcceptScrollFrame");
	if szName == "TaskAcceptFrame_ScrollBar" then
		slier		= getglobal("TaskAcceptFrame_ScrollBar");
		scrollFrame = getglobal("TaskAcceptScrollFrame");
	
	elseif szName == "TaskNotFinishFrame_ScrollBar" then
		slier		= getglobal("TaskNotFinishFrame_ScrollBar");
		scrollFrame = getglobal("TaskNotFinishScrollFrame");

	elseif szName == "TaskAwardFrame_ScrollBar"  then
		slier		= getglobal("TaskAwardFrame_ScrollBar");
		scrollFrame = getglobal("TaskAwardScrollFrame");
	
	elseif szName == "NpcDialogFrame_ScrollBar" then
		slier		= getglobal("NpcDialogFrame_ScrollBar");
		scrollFrame = getglobal("NpcDialogScrollFrame");
	end

	local nValue	 = slier:GetValue();
	local nMaxValue	 = slier:GetMaxValue();
	local nLastValue = slier:GetLastValue();	
	local nStep		 = slier:GetValueStep();
	
	if nValue <= nMaxValue then
		scrollFrame:IncreaseVerticalScroll( nValue - nLastValue );
	end
end

function NpcDialogFrame_NextBtn_OnClick()
	NpcDialogFrame:Hide();
	TaskAcceptFrame:Show();
end

function NpcDialogFrame_OnShow()
	NpcDialogFrame_DlgTextRich:clearHistory();
	NpcDialogFrame_DlgTextRich:SetText( ReplaceQuestDesc( NpcDialogFrame:GetClientString() ),88,73,53);
end

local t_npcDialogData = { text = "", relFrame = "", button = "" };
function ShowNpcDialogEffectFrame( data )
	local t_xinShouBtnMap = {	["TaskAwardFrame_GetBtn"]		= { name = "TaskAwardFrame_GetBtnXinShouBtn" }, 
								["TaskAcceptFrame_AcceptBtn"]	= { name = "TaskAcceptFrame_AcceptBtnXinShouBtn" },
							};
	t_npcDialogData = data;

	local xinShouBtn = getglobal( t_xinShouBtnMap[t_npcDialogData["button"]]["name"] );
	xinShouBtn:Show();
end


function NpcDialogEffectFrameCloseBtn_OnClick()
	local btn = getglobal( this:GetParent().."GuideBtn" );
	btn:Hide();
end

function NpcXinShouEffectBtn_OnShow()
	local btn				= getglobal( this:GetName().."GuideBtn");
	local rich				= getglobal( this:GetName().."Rich");
	local animationTexture	= getglobal( this:GetName().."UVAnimationTex");

	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( t_npcDialogData["text"] );
	ReSizeTooltip( rich:GetName(), btn:GetName(), nSelfDefineExtendWidth );
	rich:SetPointAlwaysRecalc( "center",btn:GetName(),"center",0,0 )

	animationTexture:SetUVAnimation( 30, true );
	this:SetClientString( t_npcDialogData["button"] );
	btn:Show();
end

function NpcXinShouEffectBtn_OnHide()
	this:SetClientString( "" );
end

-------------------------------------------------------------------------------------

local npcId
local lstItem = {}

local function ClearMultiCommitItemFrameScrollFrameItem(i)
	local btn = getglobal("MultiCommitItemFrameScrollFrameItem"..i)
	local tex = getglobal("MultiCommitItemFrameScrollFrameItem"..i.."Icon")
	local font = getglobal("MultiCommitItemFrameScrollFrameItem"..i.."Count")
	tex:SetTexture([[uires\ZhuJieMian\1.dds]])
	tex:SetTexUV(749, 472, 44, 44)
	font:SetText("")
	if lstItem[i] then
		UnLockItem(lstItem[i].list, lstItem[i].grid, lstItem[i].id)
		lstItem[i] = nil
	end
end

function MultiCommitItemFrame_OnLoad()
	this:RegisterEvent("GE_NPC_COMMIT")
	this:RegisterEvent("GE_CLOSE_CLAN_DIALOG")
	MultiCommitItemFrameScrollFrame:SetValueStep(5)
end

function MultiCommitItemFrame_OnEvent()
	if arg1 == "GE_NPC_COMMIT" then
		local Msg	= UIMSG:getUIMsg().NpcCommitDialog
		if Msg.CommitNum <= 1 or Msg.CommitNum > 10 then
			return
		end
		npcId = Msg.m_nNpcId
		MultiCommitItemFrameScrollFrame:SetVerticalScroll(0)
		MultiCommitItemFrameScrollFrameContent:SetText(ReplaceQuestDesc(Msg.Answer), 88, 73, 53)
		local btn
		for i = 1, Msg.CommitNum do
			btn = getglobal("MultiCommitItemFrameScrollFrameItem"..i)
			btn:Show()
		end
		for i = Msg.CommitNum + 1, 10 do
			btn = getglobal("MultiCommitItemFrameScrollFrameItem"..i)
			btn:Hide()
		end
		if not this:IsShown() then
			this:Show()
		end
	elseif arg1 == "GE_CLOSE_CLAN_DIALOG" then
		this:Hide()
	end
end

function MultiCommitItemFrame_OnShow()
	this:SetPoint("left", "$parent", "left", 0, 0)
	
	local rich		= MultiCommitItemFrameScrollFrameContent
	local nHeight	= rich:GetTotalHeight()
	rich:SetHeight(nHeight / GetScreenScaleY())
	local partion = MultiCommitItemFrameScrollFramePartionBtn
	partion:SetPoint("bottomleft", rich:GetName(), "bottomleft", 0, 20)
	MultiCommitItemFrameScrollFrameItem3:SetPoint("center", partion:GetName(), "center", 0, 35)
	MultiCommitItemFrameScrollFrameItem2:SetPoint("right", "MultiCommitItemFrameScrollFrameItem3", "left", -15, 0)
	MultiCommitItemFrameScrollFrameItem1:SetPoint("right", "MultiCommitItemFrameScrollFrameItem2", "left", -15, 0)
	MultiCommitItemFrameScrollFrameItem4:SetPoint("left", "MultiCommitItemFrameScrollFrameItem3", "right", 15, 0)
	MultiCommitItemFrameScrollFrameItem5:SetPoint("left", "MultiCommitItemFrameScrollFrameItem4", "right", 15, 0)
	for i = 6, 10 do
		local btn = getglobal("MultiCommitItemFrameScrollFrameItem"..i)
		btn:SetPoint("top", "MultiCommitItemFrameScrollFrameItem"..i - 5, "bottom", 0, 15)
	end
	MultiCommitItemFrameScrollFrame:ResetChildAnchor()
	SetScrollBar( "MultiCommitItemFrameScrollBar", 0, MultiCommitItemFrameScrollFrame:GetVerticalScrollRange())
	MultiCommitItemFrameScrollBar:SetValue(0)
	if not PlayerContainerFrame:IsShown() then
		ContainerButtonOnClick()
	end
end

function MultiCommitItemFrame_OnHide()
	for i = 1, 10 do
		ClearMultiCommitItemFrameScrollFrameItem(i)
	end
end

function MultiCommitItemFrame_OnUpdate()
	if not npcId then
		return
	end
	local npc = ActorMgr:FindActor(npcId)
	if not npc then
		return Close_NpcDialog("MultiCommitItemFrame")
	end

	local dist = util.CalActorsDistance(npc, ActorMgr:getMainPlayer())
	if dist > (NpcDialog:getMaxNpcTalkDist(npc) / 100) ^ 2 then
		return Close_NpcDialog("MultiCommitItemFrame")
	end
end

function MultiCommitItemFrameScrollBar_OnValueChanged()
	MultiCommitItemFrameScrollFrame:IncreaseVerticalScroll(this:GetValue() - this:GetLastValue())
end

function MultiCommitItemFrame_OnVerticalScroll()
	
end

function MultiCommitItemFrame_OnScrollRangeChanged()
	SetScrollBar("MultiCommitItemFrameScrollBar", 0, this:GetVerticalScrollRange())
end

function MultiCommitItemFrameItem_OnDrag()
	local idx = this:GetClientID()
	local t = lstItem[idx]
	if not t then
		return
	end
	local player = ActorMgr:getMainPlayer()
	if not player then
		return
	end
	local container = player:getContainer()
	local list, grid = t.list, t.grid
	local item = container:getItem(list, grid)
	if not item then
		return
	end
	local tex = getglobal(this:GetName().."Icon")
	UIMSG:getUIGV().DragFrom = list
	UIBeginDrag(tex:GetTexture(), grid, item:getNum(), item:getItemId())
	ClearMultiCommitItemFrameScrollFrameItem(idx)
end

function MultiCommitItemFrameItem_OnReceive()
	local list = UIMSG:getUIGV().DragFrom
	if list ~= CONTAINER_TYPE_SUNDRIES and list ~= CONTAINER_TYPE_STUFF then
		UIEndDrag()
		UIMSG:getUIGV().DragFrom = DRAG_NONE
		return
	end
	local grid			= UIGetDragUserData1()
	local mainplayer	= ActorMgr:getMainPlayer()
	local container		= mainplayer:getContainer()
	local item			= container:getItem(list, grid)
	UIEndDrag()
	UIMSG:getUIGV().DragFrom = DRAG_NONE
	SetMultiCommitItemFrameItem(this, item)
end

function MultiCommitItemFrameItem_OnEnter()
	local t = lstItem[this:GetClientID()]
	if not t then
		return
	end
	local player = ActorMgr:getMainPlayer()
	local container = player:getContainer()
	local item = container:getItem(t.list, t.grid)
	if item:getItemId() ~= 0 then
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, nil, this:GetName())
	end
end

function MultiCommitItemFrameItem_OnLeave()
	NormalItemTipsFrame:Hide()
	ArmItemTipsFrame:Hide()
	SecondArmItemTipsFrame:Hide()
	SpellTipsFrame:Hide()
end

function MultiCommitItemFrameItem_OnClick()
	if arg1 ~= "RightButton" then
		return
	end
	ClearMultiCommitItemFrameScrollFrameItem(this:GetClientID())
end

-- 确定提交按钮
function MultiCommitItemFrameItem_OnConfirm()
	local list = {}
	for _, t in pairs(lstItem) do
		table.insert(list, t.list)
		table.insert(list, t.grid)
	end
	if #list == 0 then
		ShowMidTips("请放入物品")
		return
	end
	NpcDialog:commitTaskItem(unpack(list))
end

function SetMultiCommitItemFrameItem(which, item)
	if not which or not item then
		return
	end
	local ItemDef = item:getItemDef()
	local name = which:GetName()
	local idx = which:GetClientID()
	
	local path = GetItemEquipIconPath()..ItemDef.IconID..".tga"
	if not IsInExistence(path) then
		path = DEFAULT_ITEM_PATH
	end
	local tex = getglobal(name.."Icon")
	tex:SetTexture(path)
	tex:SetTexRelUV(1, 1)
	
	local num = item:getNum()
	local font = getglobal(name.."Count")
	if num == 1 or num == 0 then
		font:SetText("")
	else
		font:SetText(num)
	end
	
	if lstItem[idx] then
		UnLockItem(lstItem[idx].list, lstItem[idx].grid, lstItem[idx].id)
	end
	local id, list, grid = item:getItemId(), item:getListType(), item:getGridIdx()
	lstItem[idx] = {id = id, list = list, grid = grid}
	LockItem(item:getListType(), item:getGridIdx(), item:getItemId(), "MultiCommitItemFrame")
end

function AutoSetMultiCommitItemFrameItem(item)
	for i = 1, 10 do
		if not lstItem[i] then
			local btn = getglobal("MultiCommitItemFrameScrollFrameItem"..i)
			if btn:IsShown() then
				return SetMultiCommitItemFrameItem(btn, item)
			end
		end
	end
	ShowMidTips("没有多余的空格来放置该物品")
end
