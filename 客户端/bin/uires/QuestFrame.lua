DISPAGE_QUESTRUN = 1;		--�Ѿ���ȡ������ҳ��
DISPAGE_CANACCEPTQUEST = 2;	--�ɽ�ȡ������ҳ��
local MAX_TRACE_PAGE = 3;
local nCurDisPage = DISPAGE_QUESTRUN;
local bShowNewQuestId = 0;
local nMaxTitleSign = 16;
local nMaxStarLevel = 12;
local nMaxAwardItemNum = 12;--�ر���������Ʒ��
local nCurSelectCanAcceptQuestId = 0;  --��ǰѡ��Ŀɽ�����Id
local nCurSelectCanAcceptRecycleId = 0;  --��ǰѡ��Ŀɽ�����Id

local nViewStartPointCanAccept	= 0;	 
local szTaskInfoDes = "\n#c431D09����˵��:#n\n";
local szTaskTitleDes = "#c431D09��������:#n\n";
local szNoFiniTraceTitleDesText = "\n#c431D09����Ҫ��:#n\n";
local t_QuestRunList = {};		--�Ѿ���ȡ������
local t_CanAcceptQuestList = {};--�ɽ�ȡ�������б�
local t_TaskDifficultList = {};
local bNeedTips = true;
local nLastTask = 0;
MAX_TRACE_NUM = 7;
CLAN_AWARD_ONE_TIME_SCALE = 0.5;
local t_RunListTitleState		= {}; -- �ѽ����� һ��Ŀ¼��չ��״̬
local t_CanAcceptListTitleState = {}; -- �ɽ����� һ��Ŀ¼��չ��״̬

local t_TaskTraceStateColor =
		{
			{247,243,240},
			{52,181,17},
			{184, 70, 70}
		};

function TaskClanResourceAwards(quest)
	local index = 0
	return function()
		local id = Quest:getQuestClanResourceId(quest, index)
		if id == -1 then
			return
		end
		local num = Quest:getQuestClanResourceNum(quest, index)
		index = index + 1
		return id, num
	end
end

function GetTaskTraceColor( data )
	local nState = data["state"] + 1;
	local t_oneStateColor = t_TaskTraceStateColor[nState];
	if t_oneStateColor == nil then
		return { 255, 255, 255 };
	end

	return t_oneStateColor;
end

local nChangeItemIndex = 1;

t_TaskDifficultList[CLIENT_TASK_DIFFICULT_NORM]   = "��";
t_TaskDifficultList[CLIENT_TASK_DIFFICULT_NOEASY] = "��ͨ";
t_TaskDifficultList[CLIENT_TASK_DIFFICULT_HARD]   = "����";

function InitTaskLogFrameData()
	nCurDisPage					= DISPAGE_QUESTRUN;
	bShowNewQuestId				= 0;
	nMaxTitleSign				= 16;
	nCurSelectCanAcceptQuestId	= 0;	--��ǰѡ��Ŀɽ�����Id
	nCurSelectCanAcceptRecycleId= 0;
	nViewStartPointCanAccept	= 0;
	t_QuestRunList				= {};	--�Ѿ���ȡ������
	t_CanAcceptQuestList		= {};	--�ɽ�ȡ�������б�
	t_TaskTraceInfoList			= {};
end

--������־��
function TaskLogFrame_OnLoad()
	local slier = getglobal("TaskLogFrame_ScrollBar");
	slier:SetMinValue(0);
	slier:SetValueStep(1);
	slier:SetMaxValue(1);
	this:RegisterEvent("GE_QUEST_UPDATECANACCEPT");
	for i=1,nMaxTitleSign do
		local tex = getglobal( "TaskBackGroundTex"..i )
		if math.mod( i,2 ) == 0 then
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" );
		end
	end
end

function TaskLogFrame_OnEvent()
	if ( arg1 == "GE_QUEST_UPDATECANACCEPT" ) and nCurDisPage == DISPAGE_CANACCEPTQUEST then
		SetCanAcceptQuestList();
		UpdateTaskInfoDis( nViewStartPointCanAccept, "" );
	end
end

function TaskLogDescFrame_OnLoad()
	for i = 1, nMaxAwardItemNum do
		local btn = getglobal( "TaskLogFrameItemBtn"..i.."AwardItem" );
		btn:SetClientUserData( 0, DRAG_AWARDITEM );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		btn:SetClientUserData( 3, 0 );
	end
end

function TaskLogFrame_OnHide()
	nCurSelectQuestId = 0;
	nCurSelectRecycleId = 0;
	nCurSelectCanAcceptQuestId = 0;
	nCurSelectCanAcceptRecycleId = 0;
	if TaskLogDescFrame:IsShown() then
		TaskLogDescFrame:Hide();
	end
end

function SetShowQuestLogState( QuestId )
	bShowNewQuestId = QuestId;
end

function SetTaskLogFrameGuide( nQuestId, nRecycleID )
	nCurDisPage		= DISPAGE_QUESTRUN;
	nCurSelectQuestId	= nQuestId;
	nCurSelectRecycleId	= nRecycleID;
end

function TaskLogFrame_OnShow()
	TaskLogFrame:SetPoint("topleft","UIClient","topleft",0,163);
	TaskLogDescFrame:SetPoint("topleft","UIClient","topleft",410,160);
	TaskLogFrame_ShareBtn:Disable();
	-- �ر�������ʾ��Ч
	GuideEffectHide();
	if TaskLogFrame_CanAcceptQuestChkGuide:IsShown() then
		TaskLogFrame_CanAcceptQuestChkGuide:Hide();
	end

	if table.getn(t_QuestRunList) == 0 then
		Quest:getQuest();
	end

	if Quest:getNumQusetRun() == 0 then
		nCurDisPage = DISPAGE_CANACCEPTQUEST;
	end

	UpdateTitleSignState();
	if nCurDisPage == DISPAGE_QUESTRUN then
		TaskLogFrame_QusetRunChk:Checked();
		TaskLogFrame_CanAcceptQuestChk:DisChecked();
		UpdateTaskInfoDis( nViewStartPointRun, "" );
		
		if nCurSelectQuestId == 0 and TaskLogDescFrame:IsShown() then
			TaskLogDescFrame:Hide();
		else
			SetTaskLogFrameTaskLogInfo(nCurSelectQuestId, nCurSelectRecycleId, true );
		end
	elseif nCurDisPage == DISPAGE_CANACCEPTQUEST then
		TaskLogFrame_CanAcceptQuestChk:Checked();
		TaskLogFrame_QusetRunChk:DisChecked();
		UpdateTaskInfoDis( nViewStartPointCanAccept, "" );
		
		if nCurSelectCanAcceptQuestId == 0 and TaskLogDescFrame:IsShown() then
			TaskLogDescFrame:Hide();
		else
			SetTaskLogFrameTaskLogInfo(nCurSelectCanAcceptQuestId, nCurSelectCanAcceptRecycleId, false );
		end
	end
end

function UpdateTitleSignState()
	local taskId, recycleId = 0, 0;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	local mapId = MaxMapControl:getMapID() ~= 0 and MaxMapControl:getMapID() or MapShower:getRealMapId();
	if mapId == 0 then return end;
	
	local mapName = getMapName( mapId );
	local defMapName = getMapName( 1000 );
	local nFound = -1;
	if #t_QuestRunList > 0 then
		for i=1, #t_QuestRunList do
			if string.find( mapName, t_QuestRunList[i][2], 1, true ) then
				t_QuestRunList[i][1] = true;
				if nFound > -1 then
					t_QuestRunList[nFound][1] = false;
				end
				nFound = i;
			--��ǰ��ͼ��������ѡ��ʱ���
			elseif nFound < 0 and string.find( defMapName, t_QuestRunList[i][2], 1, true ) then
				t_QuestRunList[i][1] = true;
				nFound = i;
			else
				t_QuestRunList[i][1] = false;
			end
		end
	end

	if nFound > -1 and nCurSelectQuestId == 0 then
		nCurSelectQuestId = t_QuestRunList[nFound][3][1][1];
		nCurSelectRecycleId = t_QuestRunList[nFound][3][1][5];
	end

	nFound = -1;
	if #t_CanAcceptQuestList > 0 then
		for i=1, #t_CanAcceptQuestList do
			if string.find( mapName, t_CanAcceptQuestList[i][2], 1, true ) then
				t_CanAcceptQuestList[i][1] = true;
				if nFound > -1 then
					t_CanAcceptQuestList[nFound][1] = false;
				end
				nFound = i;
			--��ǰ��ͼ��������ѡ��ʱ���
			elseif nFound < 0 and string.find( defMapName, t_CanAcceptQuestList[i][2], 1, true ) then
				t_CanAcceptQuestList[i][1] = true;
				nFound = i;
			else
				t_CanAcceptQuestList[i][1] = false;
			end
		end
	end

	if nFound > -1 and nCurSelectCanAcceptQuestId == 0 then
		nCurSelectCanAcceptQuestId = t_CanAcceptQuestList[nFound][3][1][1];
		nCurSelectCanAcceptRecycleId = t_CanAcceptQuestList[nFound][3][1][5];
	end
end

function ClearQuestRunList()
	for i = 1, table.getn(t_QuestRunList) do
		table.insert(t_RunListTitleState,{t_QuestRunList[i][1],t_QuestRunList[i][2]});
	end

	t_QuestRunList = {};
	HideTaskLogFrameTaskLogInfo();
end

--�����ѽ������б�
function SetQuestRunList()
	ClearQuestRunList();
	--������б�, �������ڳ���, ������ı�(��������)
	local nNum = Quest:getNumQusetRun();
	local bFound = false;
	for i = 1, nNum do
		local nQuestId = Quest:getQuestRunId(i-1);
		if nQuestId > 0 then -- and not Quest:IsTrustQuest( nQuestId ) and nQuestId < 10000 
			local szScene		= Quest:getQusetScene(nQuestId);	 --��ȡ�ɽ��������ڳ���
			local nTaskType		= Quest:getQuestType(nQuestId);  --��ȡ��������
			local szTaskName	= Quest:getQuestName(nQuestId); --��ȡ��������
			local nState		= Quest:getQuestRunState(nQuestId); --��ȡ����������
			local nRecycleID	= Quest:getQuestRunRecycleID(nQuestId);
			local szState = "";
			if nState == CLIENT_TASK_STAT_FINI then
				szState = "����ɡ�"
			end
			if nState == CLIENT_TASK_STAT_FAIL then
				szState = "��ʧ�ܡ�"
				if nTaskType == CLIENT_TASK_TYPE_SINGLE_CONVOY or nTaskType == CLIENT_TASK_TYPE_TEAM_CONVOY then
					MessageBox("������ʾ", "����<"..szTaskName..">��ʧ�ܣ��Ƿ�ɾ��������");
					MessageBoxFrame:SetClientString("��������ʧ��");
					MessageBoxFrame:SetClientUserData(0,nQuestId);
				end
			end
			local szText = "Lv." .. Quest:getQuestLvLimit(nQuestId) .. "��"..Quest:getQuestTypeName(nTaskType).."��"..szTaskName;
			local bDeploy = false;
			for i = 1, table.getn( t_RunListTitleState ) do
				if (string.find(szScene, t_RunListTitleState[i][2], 1, true)) then
					bDeploy = t_RunListTitleState[i][1];
				end
			end

			InserTaskInfo( t_QuestRunList, szScene, szText, szState, nTaskType, nQuestId, bDeploy, nRecycleID );
			if nQuestId == nCurSelectQuestId then
				bFound = true;
			end
		end
	end
	if not bFound then
		nCurSelectQuestId	= 0;
		nCurSelectRecycleId	= 0;
	end
	t_RunListTitleState = {};
end

function ClearCanAcceptQuestList()
	for i = 1, table.getn(t_CanAcceptQuestList) do
		table.insert(t_CanAcceptListTitleState,{t_CanAcceptQuestList[i][1],t_CanAcceptQuestList[i][2]});
	end

	t_CanAcceptQuestList = {};
	HideTaskLogFrameTaskLogInfo();
end

--���ÿɽ������б�
function SetCanAcceptQuestList()
	ClearCanAcceptQuestList();
	local nNum = Quest:getNumCanAcceptQuest();
	local ClanInfo = GameClanManager:getClanViewer();
	for i = 1, nNum do
		local nQuestId = Quest:getCanAcceptQuestId(i-1);
		if nQuestId > 0 and not Quest:IsTrustQuest( nQuestId ) 	and nQuestId < 10000 
		and Quest:canAcceptQuest( nQuestId ) ~= CLINT_CAN_NOT_ACCEPT_TASK_BY_LITTLE_LV 
		and Quest:getQuestType(nQuestId) ~= CLIENT_TASK_TYPE_DAILY_GOAL then
		-- ����Ŀ�� ����ʾ�ڿɽ�������
			local szScene		= Quest:getQusetScene(nQuestId);	 --��ȡ�ɽ��������ڳ���
			local nTaskType		= Quest:getQuestType(nQuestId);  --��ȡ��������
			local szTaskName	= Quest:getQuestName(nQuestId); --��ȡ��������
			local szText		= "Lv." .. Quest:getQuestLvLimit(nQuestId) .. "��"..Quest:getQuestTypeName(nTaskType).."��"..szTaskName;
			local bContinue		= true;
			if ( nTaskType == CLIENT_TASK_TYPE_CLAN and ClanInfo.m_clanInfo.m_uClanId == 0 ) then
				bContinue = false;
			end

			if not Quest:IsTrustQuest( nQuestId ) and bContinue then
				local bDeploy = false;
				for i = 1, table.getn( t_CanAcceptListTitleState ) do
					if (string.find(szScene, t_CanAcceptListTitleState[i][2], 1, true)) then
						bDeploy = t_CanAcceptListTitleState[i][1];
					end
				end
				
				local nRecycleID = Quest:getFirstCanAcceptRecycleTaskID( nQuestId );
				InserTaskInfo( t_CanAcceptQuestList, szScene, szText, "", nTaskType, nQuestId, bDeploy, nRecycleID );
			end
		end
	end
	t_CanAcceptListTitleState = {};
end

function IsInQuestTrace( nId )
	local nNum = Quest:getNumQuestTrace();
	for i = 1, nNum do
		local nQuestTraceId = Quest:getQuestTrace( i -1 );
		if  nId == nQuestTraceId then
			return true;
		end
	end
	return false
end

--ȫ��׷��
function TaskLogFrame_AllQuestChk_OnClick()
	local nState = this:GetCheckState();
	local nNum = Quest:getNumQusetRun();
	
	if nState then
		for i = 1, nNum do 
			local nQuestRunId = Quest:getQuestRunId( i - 1 ); 
			if not IsInQuestTrace( nQuestRunId ) and Quest:getQuestType( nQuestRunId ) ~= CLIENT_TASK_TYPE_DAILY_GOAL then
				Quest:addQuestTrace( nQuestRunId ); --�������׷��
			end
		end
	else
		for i = 1, nNum do 
			local nQuestRunId = Quest:getQuestRunId( i - 1 );
			if Quest:getQuestType( nQuestRunId ) ~= CLIENT_TASK_TYPE_DAILY_GOAL then
				Quest:delQuestTrace( nQuestRunId ); --ɾ������׷��
			end
		end
	end

	Quest:setQuestTraceAll(nState);
	UpdateTaskInfoDis( nViewStartPointRun, "" );

	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

function TaskLogFrame_AllQuestChk_OnUpdate()
	this:setCheckState(Quest:isQuestTraceAll())
end

--����׷��
function TaskLogFrameTaskTraceChk_OnClick()
	local font = getglobal(this:GetParent().."TaskLogRich")
	local nQuestId = font:GetClientUserData(0);
	if this:GetCheckState() then
		Quest:addQuestTrace( nQuestId );				--�������׷��
	else
		Quest:delQuestTrace( nQuestId );				--ɾ������׷��
	end
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

--����������־Richѡ��ʱ����
function SetTaskLogRichSelectHighLight( nIndex )
	for i = 1, nMaxTitleSign do
		local btn = getglobal("TaskLogFrameTaskLogInfo"..i);
		btn:DisChecked();
	end
	local btn = getglobal( "TaskLogFrameTaskLogInfo"..nIndex );
	btn:Checked();
end

function RefreshQuestDesc()
	TaskLogFrame_ShareBtn:Disable();
	for i = 1, nMaxTitleSign do
		local font		= getglobal("TaskLogFrameTaskLogInfo"..i.."TaskLogRich");
		local nQuestId		= font:GetClientUserData(0);
		local nRecycleID	= font:GetClientUserData(2);
		if nQuestId > 0 and nCurDisPage == DISPAGE_QUESTRUN and TaskLogFrame:IsShown() then
			SetTaskLogFrameTaskLogInfo(nQuestId, nRecycleID, false );
		end
	end
end

function TaskLogFrameTaskLogInfo_OnLoad()
	local font = getglobal(this:GetName().."TaskLogRich");
	font:SetClientUserData(0,0);
	this:SetOverlayRegion( 0,1,357,24 );
end

function TaskLogFrameTaskLogInfo_OnClick()
	TaskLogFrame_ShareBtn:Disable();
	local font			= getglobal(this:GetName().."TaskLogRich")
	local nQuestId		= font:GetClientUserData( 0 );
	local nRecycleID	= font:GetClientUserData( 2 );
	Quest:getMissionLink( nQuestId, nRecycleID ); -- ��ʾ�����ӵ������
	local btn = getglobal(this:GetName());
	local nClientId = btn:GetClientID();	
	SetTaskLogRichSelectHighLight( nClientId );

	if nQuestId == 0 then
		local nTaskInfoIndex = font:GetClientUserData(1);
		local t_TaskInfoList = GetTaskInfoList();
		if t_TaskInfoList[nTaskInfoIndex][1] then
			t_TaskInfoList[nTaskInfoIndex][1] = false;
		else
			t_TaskInfoList[nTaskInfoIndex][1] = true;
		end

		if nCurDisPage == DISPAGE_QUESTRUN then
			UpdateTaskInfoDis( nViewStartPointRun, "" );
		elseif nCurDisPage == DISPAGE_CANACCEPTQUEST then
			UpdateTaskInfoDis( nViewStartPointCanAccept,"" );
		end
		return ;
	end

	if nQuestId ~= 0 then
		SetTaskLogFrameTaskLogInfo( nQuestId, nRecycleID, true );
	end
	--��¼��ǰѡ�е�����ID
	if nCurDisPage == DISPAGE_QUESTRUN then
		nCurSelectQuestId	= nQuestId;
		nCurSelectRecycleId	= nRecycleID;
	else
		nCurSelectCanAcceptQuestId	= nQuestId;
		nCurSelectCanAcceptRecycleId	= nRecycleID;
	end
end

function ClearTable( t_Table )
	for i = 1, table.getn(t_Table) do
		table.remove( t_Table, 1 );
	end
end

function TaskLogDescFrame_OnShow()
	if bShowNewQuestId == 0 then
		if ( nCurDisPage == DISPAGE_QUESTRUN and nCurSelectQuestId == 0 ) then
			this:Hide();
			return;
		end

		if ( nCurDisPage == DISPAGE_CANACCEPTQUEST and nCurSelectCanAcceptQuestId == 0 ) then
			this:Hide();
			return;
		end
		
		if not TaskLogFrame:IsShown() then
			TaskLogDescFrame:SetPoint( "center", "$parent", "center", 0, 0 );
		end
	end
end

function HideTaskLogFrameTaskLogInfo()
	TaskLogFrameTaskAwardFont:Hide();
	TaskLogFrameTaskNameFont:Hide();
	TaskLogFrameTaskDifficultFont:Hide();
	TaskLogFrameTaskMoneyFont:Hide();
	TaskLogFrameTaskMoneyTex:Hide();
	TaskLogFrameTaskExpFont:Hide();
	TaskLogFrameTaskLogDesRich:Hide();

	TaskLogFrameTaskStarFont:Hide();
	ResetTaskLogFrameTaskLogInfo();

	for i = 1, nMaxStarLevel do
		local tex = getglobal( "TaskLogFrameTaskStarSignTex"..i );
		tex:Hide();
	end
	TaskLogDescFrame:Hide();
end

function ShowTaskLogFrameTaskLogInfo()
	TaskLogFrameTaskAwardFont:Show();
	TaskLogFrameTaskLogDesRich:Show();
	TaskLogFrameTaskNameFont:Show();
	TaskLogFrameTaskDifficultFont:Show();
	TaskLogFrameTaskMoneyFont:Show();
	TaskLogFrameTaskMoneyTex:Show();
	TaskLogFrameTaskExpFont:Show();
	TaskLogFrameTaskLogDesRich:Show();
	TaskLogFrameTaskStarFont:Show();
end

function ResetTaskLogFrameTaskLogInfo()
	for i = 1, nMaxAwardItemNum do
		local itemBtn = getglobal("TaskLogFrameItemBtn"..i);
		local itemName = getglobal("TaskLogFrameItemBtn"..i.."AwardItemFont");
		itemBtn:Hide();
		itemName:Hide();
		itemBtn:SetClientUserData( 1, 0 );
		itemBtn:SetClientUserData( 2, 0 );
		itemBtn:SetClientUserData( 3, 0 );
	end
end

function ResetCurCanAcceptQuestId()
	nCurSelectCanAcceptQuestId = 0;
	nCurSelectCanAcceptRecycleId = 0;
end

--�����ѽ����񼰿ɽ��������ϸ��Ϣ
function ClearTaskLogDescFrame()
	local t_fonts = { ["QuestScrollFrameTaskAwardBtnExtraAwardFont"]	= { value = "", }, };
	util.SetFonts( t_fonts );
end

function SetTaskLogFrameTaskLogInfo( nQuestId, nRecycleID, bChange )
	if nQuestId == 0 then return end
	
	if nCurDisPage == DISPAGE_QUESTRUN then
		if Quest:isShareQuest( nQuestId ) then
			TaskLogFrame_ShareBtn:Enable();
		else
			TaskLogFrame_ShareBtn:Disable();
		end
	end

	local MainPlayer = ActorMgr:getMainPlayer();
	local nRace = MainPlayer:getRace();

	if bShowNewQuestId == 0 then
		if nCurDisPage == DISPAGE_QUESTRUN  then
			if nCurSelectQuestId == 0 or bChange then
				nCurSelectQuestId	= nQuestId;
				nCurSelectRecycleId	= nRecycleID;
			else
				nQuestId	= nCurSelectQuestId;
				nRecycleID	= nCurSelectRecycleId;
			end
		else
			nCurSelectCanAcceptQuestId	= nQuestId;
			nCurSelectCanAcceptRecycleId	= nRecycleID;
		end
	end
	
	local scrollFrame = getglobal( "QuestScrollFrame" );
	scrollFrame:SetValueStep( 5 );
	scrollFrame:SetVerticalScroll( 0 );
	ClearTaskLogDescFrame();
	ResetTaskLogFrameTaskLogInfo();
	TaskLogFrameTaskLogDesRich:Clear();
	local szScene		= Quest:getQusetScene(nQuestId);					--��ȡ�ɽ��������ڳ���
	local szDialogDes	= ReplaceConsumeMoneyDesc(Quest:getQuestDialog(nQuestId),nQuestId, nRecycleID);--����Ի�
	local szDes			= ReplaceConsumeMoneyDesc(Quest:getQuestDes(nQuestId),nQuestId, nRecycleID); --��������
	local szRunReqDesc;														--����Ҫ������
	local nTaskType		= Quest:getQuestType(nQuestId);						--��ȡ��������
	local szTaskName	= Quest:getQuestName(nQuestId);						--��ȡ��������
	local nDifficult	= Quest:getQuestDifficult(nQuestId);				--��ȡ�����Ѷ�
	local nStar			= Quest:getQuestStar(nQuestId);						--��ȡ�����Ǽ�
	local tagTaskMoney	= Quest:getQuestMoney(nQuestId, nRecycleID);		--������Ǯ
	local nMoney		= tagTaskMoney.Val;
	local nType			= tagTaskMoney.Type;
	local nExp			= Quest:getQuestExp(nQuestId, nRecycleID);		--��������
	local nExpFontY		= 30;

	if bShowNewQuestId ~= 0 then
		szRunReqDesc = Quest:getQuestReqDesc( nQuestId );			--����Ҫ������
	elseif nCurDisPage == DISPAGE_QUESTRUN then
		szRunReqDesc = Quest:getQuestRunReqDesc( nQuestId );		-- �ѽ�����Ҫ������
	elseif nCurDisPage == DISPAGE_CANACCEPTQUEST then
		szRunReqDesc = Quest:getCanAcceptQuestReqDesc( nQuestId );	--�ɽ�����Ҫ������
	end

	TaskLogFrameTaskNameFont:SetText( szTaskName );
	TaskLogFrameTaskDifficultFont:SetText("�����Ѷȣ�"..t_TaskDifficultList[nDifficult]);
	TaskLogFrameTaskLevelFont:SetText("");

	local rich = getglobal("TaskLogFrameTaskLogDesRich");
	rich:SetPoint("topleft", "TaskLogDescFrame", "topleft", 30, 109);
	rich:Show();
	local szRecycleDesc = Quest:getRecycleDes(nQuestId, nRecycleID);
	rich:SetFontType( FONTSYTLE_NORMAL );
	rich:SetText(szTaskTitleDes..ReplaceQuestDesc( szDes ).."\n"..szRecycleDesc, 88, 73, 53);
	if nCurDisPage == DISPAGE_QUESTRUN and bShowNewQuestId == 0 then
		local szNoFiniTraceDes	= Quest:getQuestNoFiniTraceDes(nQuestId);	-- ��ȡ����δ�������׷����ʾ 
		if Quest:getQuestRunState( nQuestId ) == CLIENT_TASK_STAT_FINI then
			szNoFiniTraceDes	= Quest:getQuestFiniTraceDes(nQuestId);
		end
		
		szNoFiniTraceDes		= ReplaceTaskTraceTip( nQuestId, szNoFiniTraceDes ).szDes;	-- �滻��Ҫ�Ҽ�ʹ�õ�������ƷTips����
		szNoFiniTraceDes		= ReplaceAllDes( szNoFiniTraceDes, "#cFEBC42", "#c584935");	 -- �滻�������������ɫ
		rich:AddText("\n"..ReplaceQuestDesc( szNoFiniTraceDes ).."\n", 88, 73, 53);
	end

	if nCurDisPage == DISPAGE_QUESTRUN  then 
		rich:AddText(szTaskInfoDes..ReplaceQuestDesc( szDialogDes ), 88, 73, 53);
	else  
		rich:AddText(szNoFiniTraceTitleDesText, 88, 73, 53);
	end

	if szRunReqDesc ~= "" then
		rich:AddText(ReplaceQuestDesc( szRunReqDesc ), 88, 73, 53);
	end
	
	local nHeight = rich:GetTotalHeight();
	rich:SetHeight( nHeight / GetScreenScaleY() );
	
	QuestScrollFramePartionBtn:SetPoint("TOPLEFT", "TaskLogFrameTaskLogDesRich", "BOTTOMLEFT", 0, 10);
	QuestScrollFrameTaskAwardBtn:SetPoint("TOPLEFT", "TaskLogFrameTaskLogDesRich", "BOTTOMLEFT", 0, 20);

	if nCurDisPage == DISPAGE_QUESTRUN and bShowNewQuestId == 0 then
		local bTrace = false ;--�Ƿ�Ҫ׷��
		local nNum = Quest:getNumQuestTrace();
		for i = 1, nNum do
			local nQuestTraceId = Quest:getQuestTrace( i -1 );
			if  nQuestId == nQuestTraceId then
				bTrace = true;
				break;
			end
		end
	end
	TaskLogFrameTaskStarFont:SetText("");
	for i = 1, nStar do
		local tex = getglobal( "TaskLogFrameTaskStarSignTex"..i );
		tex:Show();
		TaskLogFrameTaskStarFont:SetText("�����Ǽ���");
	end

	for i = nStar+1, nMaxStarLevel do
		local tex = getglobal( "TaskLogFrameTaskStarSignTex"..i );
		tex:Hide();
	end

	ShowTaskLogFrameTaskLogInfo();
	SetAwardItemInfo( nQuestId, 1, nRecycleID );

	scrollFrame:ResetChildAnchor();
	SetScrollBar( "TaskLogDescFrame_ScrollBar", 0, scrollFrame:GetVerticalScrollRange() );
	TaskLogDescFrame_ScrollBar:SetValue(0);
	if scrollFrame:GetVerticalScrollRange() > 0 then
		TaskLogDescFrame_ScrollBackGroundTex:Show();
		TaskLogDescFrame_ScrollBackGroundTexTop:Show();
		TaskLogDescFrame_ScrollBackGroundTexBottom:Show();
	else
		TaskLogDescFrame_ScrollBackGroundTex:Hide();
		TaskLogDescFrame_ScrollBackGroundTexTop:Hide();
		TaskLogDescFrame_ScrollBackGroundTexBottom:Hide();
	end
	if not TaskLogDescFrame:IsShown() then
		TaskLogDescFrame:Show();
	end
end

function TaskLogDescFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "TaskLogDescFrame_ScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	local nLastValue = slider:GetLastValue();	
	local nStep		 = slider:GetValueStep();
	
	if nValue <= nMaxValue then
		local scrollFrame	= getglobal( "QuestScrollFrame" );
		scrollFrame:IncreaseVerticalScroll( nValue - nLastValue );
	end
end

function ReSetTaskAwardItemBtn()
	for i = 1, nMaxAwardItemNum do
		local btn = getglobal("TaskLogFrameItemBtn"..i);
		local itemfont = getglobal("TaskLogFrameItemBtn"..i.."AwardItemFont");
		btn:Hide();
		itemfont:Hide();
	end
end

function SetAwardItemInfo( nQuestId, nIndex, nRecycleID )
	local nItemNum		= Quest:getQuestItemNum( nQuestId, nRecycleID );	 --��ȡ��������Ʒ����
	local tagTaskMoney	= Quest:getQuestMoney(nQuestId, nRecycleID);	--������Ǯ
	local nMoney		= tagTaskMoney.Val;
	local nType			= tagTaskMoney.Type;
	local nExp			= Quest:getQuestExp(nQuestId, nRecycleID);		--��������
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end
	ReSetTaskAwardItemBtn();
	local nRace			= MainPlayer:getRace();
	local itemIndex		= 0;
	for i = nIndex, CLIENT_MAX_TASK_AWARD_ITEM do
		local nItemId = Quest:getQuestItem( nQuestId, nRecycleID, i-1 );     -- ��ȡ��������Ʒ
		if nItemId ~= 0 and getItemDef( nItemId ) ~= nil  then
			local itemdef				= getItemDef( nItemId );
			local itemFixedAttr			= Quest:getQuestFixedAttr( nQuestId, nRecycleID, i-1 );			-- ����������ؼ�������
			local itemBindingAndSlot	= Quest:getQuestBindingAndSlot( nQuestId, nRecycleID, i-1 );    -- �����������װ���󶨼�������
			local nItemCount			= Quest:getQuestItemCount( nQuestId, nRecycleID, i-1 );			-- ��ȡ��������Ʒ����
			--��ȡ��������Ʒ����Ҫְҵ
			if Quest:isQuestItemCareer(nQuestId, nRecycleID, i - 1, nRace) then
				itemIndex = itemIndex + 1;
				local Btn			= getglobal( "TaskLogFrameItemBtn"..itemIndex );
				local itemBtn		= getglobal( Btn:GetName().."AwardItem" );
				local itemTex		= getglobal( Btn:GetName().."AwardItemIconTexture" );
				local itemCountFont = getglobal( Btn:GetName().."AwardItemCountFont" );
				local itemName		= getglobal( Btn:GetName().."AwardItemFont" );
				local itemNameTex	= getglobal( Btn:GetName().."AwardItemTex" );
				local itemBackTex	= getglobal( Btn:GetName().."AwardItemBoxTexture" );	
				local itemUVTex		= getglobal( Btn:GetName().."AwardItemUVAnimationTex" );
				itemBackTex:SetTexUV(794, 562, 44, 44);
				itemCountFont:SetText("");
				itemBackTex:Show();
				itemNameTex:Show();
				itemName:Show();

				if itemIndex%2 == 0 then
					Btn:SetPoint("TOPLEFT","TaskLogFrameTaskLogDesRich","BOTTOMLEFT", 160,	45+(math.ceil(itemIndex/2)-1)*50);
					itemName:SetPoint("TOPLEFT","TaskLogFrameTaskLogDesRich","BOTTOMLEFT", 210, 50+(math.ceil(itemIndex/2)-1)*50);
				else
					Btn:SetPoint("TOPLEFT","TaskLogFrameTaskLogDesRich","BOTTOMLEFT", 0,	45+(math.ceil(itemIndex/2)-1)*50);
					itemName:SetPoint("TOPLEFT","TaskLogFrameTaskLogDesRich","BOTTOMLEFT", 50, 50+(math.ceil(itemIndex/2)-1)*50);
				end

				local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
				-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				itemTex:SetTexture( IconPath );
				itemTex:SetTexRelUV( 1, 1 );
				itemCountFont:SetPoint( "bottomleft", Btn:GetName(), "bottomleft", 0, 10 );
				
				SetEquipImportantEffectUV( itemdef, itemUVTex:GetName());
				itemUVTex:SetTexRelUV( 1, 1 );

				local szItemName = itemdef.Name;
				if nItemCount > 1 then
					szItemName = szItemName.."\n������"..nItemCount;
				end
				
				itemName:SetText( szItemName, 57, 48, 34 );
				Btn:Show();
				itemBtn:SetClientUserData( 1, nItemId );
				itemBtn:SetClientUserData( 2, itemFixedAttr );
				itemBtn:SetClientUserData( 3, itemBindingAndSlot );
			end
		end
	end
	
	local nExpFontY		= 23 +  math.ceil(itemIndex/2)*50;
	local nExpFontY		= SetPointMoneyAdnExp( nType, nMoney, nExp, nExpFontY, szAwardDesign, nQuestId, itemIndex, nRecycleID );
	SetTaskRecycleItem( nQuestId, nExpFontY + 10, "TaskLogFrame", "QuestScrollFrame", "TaskLogFrameTaskLogDesRich", nRecycleID );
end

function QuestFrameScrollFrame_OnVerticalScroll()
end

-- ��ScrollFrame��������Χ�仯��ʱ����Ӧ
function QuestFrameScrollFrameOnScrollRangeChanged()
	-- ����������Ϊ
	local scrollFrame	= getglobal( "QuestScrollFrame" );
	SetScrollBar( "TaskLogDescFrame_ScrollBar",  0, scrollFrame:GetVerticalScrollRange() );
	if scrollFrame:GetVerticalScrollRange() > 0 then
		TaskLogDescFrame_ScrollBackGroundTex:Show();
		TaskLogDescFrame_ScrollBackGroundTexTop:Show();
		TaskLogDescFrame_ScrollBackGroundTexBottom:Show();
	else
		TaskLogDescFrame_ScrollBackGroundTex:Hide();
		TaskLogDescFrame_ScrollBackGroundTexTop:Hide();
		TaskLogDescFrame_ScrollBackGroundTexBottom:Hide();
	end
end

function SetPointMoneyAdnExp( nType, nMoney, nExp, nExpFontY, szAwardDesign, nQuestId, nIndexItem, nRecycleID )
	local fScale	= 1;
	local nTaskType = Quest:getQuestType( nQuestId );
	if nTaskType == CLIENT_TASK_TYPE_CLAN and Quest:isRecycleTask( nQuestId ) then
		local nFiniRecycleIdx	= Quest:getReCycleQusetOneDayNum( nRecycleID );
		--fScale = fScale * CLAN_AWARD_ONE_TIME_SCALE ^ nFiniRecycleIdx;
		--print( "SetPointMoneyAdnExp -- nFiniRecycleIdx = "..nFiniRecycleIdx.."--681" );
	end

	if nExp > 0 then
		nExp = math.floor( nExp * fScale );
	end

	if nMoney > 0 then
		nMoney = math.floor( nMoney * fScale );
	end

	local szMonType = ""..MONEY_TYPE_MONEY_NAME.."��";
	local nOffset	= 0;
	local texPath = "uires\\ZhuJieMian\\1.dds"
	if nType == ATTR_ID_MONEY then
		TaskLogFrameTaskMoneyTex:SetTexture(texPath);
		TaskLogFrameTaskMoneyTex:SetTexUV(117,494,17,18);
		TaskLogFrameTaskMoneyTex:SetSize(17,18);
		QuestScrollFrameTaskMoneyTexBtn:Show();
		nOffset = nOffset-1;
		szMonType = MONEY_TYPE_MONEY_NAME.."��";
	elseif nType == ATTR_ID_BULL then
		TaskLogFrameTaskMoneyTex:SetTexture(texPath);
		TaskLogFrameTaskMoneyTex:SetTexUV(78,494,20,18);
		TaskLogFrameTaskMoneyTex:SetSize(20,18);
		QuestScrollFrameTaskMoneyTexBtn:Show();
		nOffset = nOffset;
		szMonType = MONEY_TYPE_BULL_NAME.."��";
	elseif nType == ATTR_ID_BINDGODCOIN then
		TaskLogFrameTaskMoneyTex:SetTexture(texPath);
		TaskLogFrameTaskMoneyTex:SetTexUV(57,494,20,18);
		TaskLogFrameTaskMoneyTex:SetSize(20,18);
		QuestScrollFrameTaskMoneyTexBtn:Show();
		nOffset = nOffset;
		szMonType = MONEY_TYPE_BIND_GODCOIN_NAME.."��";
	elseif nType == ATTR_ID_BATTLE_GLORY then
		TaskLogFrameTaskMoneyTex:SetTexture(texPath);
		TaskLogFrameTaskMoneyTex:SetTexUV(963,203,16,18);
		TaskLogFrameTaskMoneyTex:SetSize(16,18);
		QuestScrollFrameTaskMoneyTexBtn:Show();
		nOffset = nOffset;
		szMonType = "������";
	elseif nType == ATTR_ID_ZHENGYI then	
		SetTaskAwardTexture( "TaskLogFrameTaskMoneyTex",nType) ;
		szMonType = "����㣺";
		nOffset = nOffset;
	elseif nType == ATTR_ID_AOSHAN_BRAND then
		SetTaskAwardTexture( "TaskLogFrameTaskMoneyTex",nType) ;
		szMonType = "ս����£�";
		nOffset = nOffset;
	elseif nType == ATTR_ID_LIFT_BRAND then
		SetTaskAwardTexture( "TaskLogFrameTaskMoneyTex",nType) ;
		szMonType = "���潱�£�";
		nOffset = nOffset;
	elseif nType == ATTR_ID_SCHOOL_VAL then
		SetTaskAwardTexture( "TaskLogFrameTaskMoneyTex",nType) ;
		szMonType = "���е㣺";
		nOffset = nOffset;	
	end
	if tonumber( nMoney ) <= 0 and tonumber( nExp ) > 0  then
		TaskLogFrameTaskMoneyFont:SetText("");
		TaskLogFrameTaskExpFont:SetText("���飺"..nExp);
		QuestScrollFrameTaskExpBtn:SetPoint("topleft", "QuestScrollFrameTaskAwardBtn", "topleft", 0, nExpFontY);
		QuestScrollFrameTaskMoneyTexBtn:Hide();
	elseif tonumber( nMoney ) > 0 and tonumber( nExp ) <= 0 then
		TaskLogFrameTaskMoneyFont:SetText(szMonType..nMoney);
		TaskLogFrameTaskExpFont:SetText("");		
		local nMoneyFontWidth = TaskLogFrameTaskMoneyFont:GetTextExtentWidth( TaskLogFrameTaskMoneyFont:GetText() )/GetScreenScaleY();
		QuestScrollFrameTaskMoneyBtn:SetPoint("topleft", "QuestScrollFrameTaskAwardBtn", "topleft", 0, nExpFontY);
		QuestScrollFrameTaskMoneyTexBtn:SetPoint("topleft", "QuestScrollFrameTaskAwardBtn", "topleft", 0 + nMoneyFontWidth, nExpFontY+nOffset);
		QuestScrollFrameTaskMoneyTexBtn:Show();
	elseif  tonumber( nMoney ) > 0 and tonumber( nExp ) > 0 then
		TaskLogFrameTaskExpFont:SetText("���飺"..nExp);
		TaskLogFrameTaskMoneyFont:SetText(szMonType..nMoney);
		local nMoneyFontWidth = TaskLogFrameTaskMoneyFont:GetTextExtentWidth( TaskLogFrameTaskMoneyFont:GetText() )/GetScreenScaleY();
		QuestScrollFrameTaskMoneyTexBtn:SetPoint("topleft", "QuestScrollFrameTaskAwardBtn", "topleft", 167+ nMoneyFontWidth, nExpFontY+nOffset);
		QuestScrollFrameTaskExpBtn:SetPoint("topleft", "QuestScrollFrameTaskAwardBtn", "topleft", 0, nExpFontY);	
		QuestScrollFrameTaskMoneyBtn:SetPoint("topleft", "QuestScrollFrameTaskAwardBtn", "topleft", 162, nExpFontY);
		QuestScrollFrameTaskMoneyTexBtn:Show();
	else
		TaskLogFrameTaskExpFont:SetText("");
		TaskLogFrameTaskMoneyFont:SetText("");
		QuestScrollFrameTaskMoneyTexBtn:Hide();
		nExpFontY = nExpFontY - 20;
	end
	nExpFontY = nExpFontY + 20;
	local nPointY			= nExpFontY;
	local szAwardDesign		= Quest:getAwardDesignation(nQuestId, nRecycleID);				--��ȡ�������ƺ�
	local szAwardRepact		= Quest:getAwardRespactName(nQuestId, nRecycleID);				--��ȡ������������
	local nAwardRepact		= Quest:getAwardRespactValue(nQuestId,nRecycleID);				--��ȡ����������ֵ
	local szSecondarySkill	= Quest:getAwardSecondarySkillName(nQuestId);		--�����
	local nClanMoney		= math.floor( Quest:getQuestClanMoney( nQuestId, nRecycleID ) );--�����ʽ�
	local nClanAnDing		= math.floor( Quest:getQuestClanAnDing(nQuestId) * fScale );		--���ᰲ����
	local nClanContribute	= math.floor( Quest:getQuestClanContribute(nQuestId, nRecycleID) * fScale );	--���ṱ�׶�
	local nClanTechPoint	= math.floor( Quest:getQuestClanTechPoint(nQuestId) * fScale );	-- ����Ƽ���

	local szBtnRef			= "QuestScrollFrameTaskAwardBtn";
	local DesignationFont	= getglobal("TaskLogFrameDesignationFont");
	local DesignationBtn	= getglobal("QuestScrollFrameTaskDesignationBtn");
	local RespactFont		= getglobal("TaskLogFrameRespactFont");
	local RespactBtn		= getglobal("QuestScrollFrameTaskRespactBtn");
	local SeondarySkillFont = getglobal("TaskLogFrameSecondaryFont");
	local SecondarySkillBtn = getglobal("QuestScrollFrameTaskSecondarySkillBtn");
	local ClanMoneyFont		= getglobal("TaskLogFrameClanMoneyFont");
	local ClanMoneyBtn		= getglobal("QuestScrollFrameClanMoneyBtn");
	local ClanAnDingFont	= getglobal("TaskLogFrameClanAnDingFont");
	local ClanAnDingBtn		= getglobal("QuestScrollFrameClanAnDingBtn");
	local ClanContributeFont= getglobal("TaskLogFrameClanContributeFont");
	local ClanContributeBtn	= getglobal("QuestScrollFrameClanContributeBtn");
	local ClanTechPointFont= getglobal("TaskLogFrameClanTechPointFont");
	local ClanTechPointBtn	= getglobal("QuestScrollFrameClanTechPointBtn");

	if szAwardDesign ~= "" and szAwardRepact == "" then
		DesignationFont:SetText("�ƺţ�"..szAwardDesign);
		RespactFont:SetText("");
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);	
	elseif szAwardDesign == "" and szAwardRepact ~= "" then 
		RespactFont:SetText("������+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("");
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	elseif szAwardDesign ~= "" and szAwardRepact ~= "" then 
		RespactFont:SetText("������+"..nAwardRepact..szAwardRepact);
		DesignationFont:SetText("�ƺţ�"..szAwardDesign);
		RespactBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
		DesignationBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 162, nPointY);
	else
		RespactFont:SetText("");
		DesignationFont:SetText("");
		nExpFontY = nExpFontY - 20;
	end
	if szSecondarySkill ~= "" then
		nPointY = nPointY + 20;
		if Quest:getAwardSecondarySkillVal( nQuestId ) > 0 then
			SeondarySkillFont:SetText("����Ƽ������ȣ�".. math.ceil( Quest:getAwardSecondarySkillVal( nQuestId ) / 100 ) );
		else
			SeondarySkillFont:SetText("�Ƽ�������"..szSecondarySkill);
		end
		SecondarySkillBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", 0, nPointY);
	else
		SeondarySkillFont:SetText("");
	end
	
	local nPointX = 0
	if nClanMoney ~= 0 then
		nPointY = nPointY + 20;
		ClanMoneyFont:SetText("�����ʽ�"..nClanMoney);
		ClanMoneyBtn:SetPoint("bottomleft", szBtnRef, "bottomleft", nPointX, nPointY);
		nPointX = 162;
	else
		ClanMoneyFont:SetText("");
	end

	if nClanAnDing ~= 0 then
		if nPointX == 0 then
			nPointY = nPointY + 20;
		end
		ClanAnDingFont:SetText("���ᰲ���ȣ�"..nClanAnDing);
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
		ClanContributeFont:SetText("���ṱ�׶ȣ�"..nClanContribute);
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
		ClanTechPointFont:SetText("����Ƽ��㣺"..nClanTechPoint);
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
			local btn = getglobal("QuestScrollFrameTaskResource"..ResourceClass)
			if not btn then
				break
			end
			local text = getglobal("QuestScrollFrameTaskResource"..ResourceClass.."Text")
			if nPointX == 0 then
				nPointY = nPointY + 20
			end
			text:SetText(string.format("%s��%d", GameClanManager:getProductDefByID(id).Name, num))
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
		local text = getglobal("QuestScrollFrameTaskResource"..ResourceClass.."Text")
		if not text then
			break
		end
		text:SetText("")
	end

	nExpFontY = nPointY
	return nExpFontY;
end

--�ѽ�����
function TaskLogFrame_QusetRunChk_OnClick()
	nCurDisPage = DISPAGE_QUESTRUN;
	TaskLogFrame_QusetRunChk:Checked();
	TaskLogFrame_ShareBtn:Show();
	TaskLogFrame_CancelBtn:Show();
	TaskLogFrame_CanAcceptQuestChk:DisChecked();
	HideTaskLogFrameTaskLogInfo();

	SetQuestRunList();
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
	--RefreshQuestDesc();

	-- Ĭ��ѡ�е�һ������
	local font = getglobal( "TaskLogFrameTaskLogInfo2TaskLogRich" );
	if nCurSelectQuestId == 0 and font:GetClientUserData( 0 ) > 0 then
		nCurSelectQuestId	= font:GetClientUserData( 0 );
		nCurSelectRecycleId	= font:GetClientUserData( 2 );
	end

	SetTaskLogFrameTaskLogInfo( nCurSelectQuestId, nCurSelectRecycleId, true );

	-- ��ʾ�ѽ��������ϸ��Ϣ����
	TaskLogDescFrame:Show();
	UpdateTaskInfoDis( nViewStartPointRun, "" );
end

--�ɽ�����
function TaskLogFrame_CanAcceptQuestChk_OnClick()
	if TaskLogFrame_CanAcceptQuestChkGuide:IsShown() then
		TaskLogFrame_CanAcceptQuestChkGuide:Hide();
	end
	TaskLogFrame_CanAcceptQuestChk:Checked();
	TaskLogFrame_QusetRunChk:DisChecked();
	TaskLogFrame_ShareBtn:Hide();
	TaskLogFrame_CancelBtn:Hide();
	nCurDisPage = DISPAGE_CANACCEPTQUEST;
	HideTaskLogFrameTaskLogInfo();

	Quest:updateCanAcceptQuest();
	SetCanAcceptQuestList();
	SetTaskTraceInfoList();
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
	--RefreshQuestDesc();

	-- Ĭ��ѡ�е�һ������
	local font = getglobal( "TaskLogFrameTaskLogInfo2TaskLogRich" );
	if nCurSelectCanAcceptQuestId == 0 and font:GetClientUserData( 0 ) > 0 then
		nCurSelectCanAcceptQuestId	= font:GetClientUserData( 0 );
		nCurSelectCanAcceptRecycleId= font:GetClientUserData( 2 );
	end
	SetTaskLogFrameTaskLogInfo( nCurSelectCanAcceptQuestId, nCurSelectCanAcceptRecycleId, false );
	--[[local nNum = Quest:getNumCanAcceptQuest();
	for i = 1, nNum do
		local nQuestId = Quest:getCanAcceptQuestId(i-1);
		if nCurSelectCanAcceptQuestId == nQuestId then
			SetTaskLogFrameTaskLogInfo( nCurSelectCanAcceptQuestId, nCurSelectCanAcceptRecycleId, true );
		end
	end--]]

	-- ��ʾ�ɽ��������ϸ��Ϣ����
	TaskLogDescFrame:Show();
	UpdateTaskInfoDis( nViewStartPointCanAccept, "" );
end
--��������
function TaskLogFrame_ShareBtn_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if nCurSelectQuestId ~= 0 and Quest:isShareQuest( nCurSelectQuestId ) then
		local teamInfo		= TeamManager:getTeamInfo();--��ȡ������Ϣ
		-- �������ڶ�����
		if teamInfo.MemberNum > 0 then
			Quest:shareQuest( nCurSelectQuestId );
		end
	end
end

--��������
function TaskLogFrame_CancelBtn_OnClick()
	if nCurSelectQuestId == 0 then
		return;
	else
		MessageBox("��ʾ", "ȷ�Ϸ�������" .. Quest:getQuestName(nCurSelectQuestId) .. "��\n����Q������������־-�ɽ������ѯ��");
		MessageBoxFrame:SetClientString("ȷ�Ϸ�������");
		MessageBoxFrame:SetClientUserData( 0, nCurSelectQuestId );
	end
end


local t_BeforeDoTaskInfo =
{
	{ "�Ƿ�չ��", "��ʾ������", { { "����ID","��������" ,"����״̬" }, { "����ID","��������" ,"����״̬"} } },
};

--������б�, �������ڳ���, ������ı�(��������)
function InserTaskInfo( t_InfoList, szTitleName, szText, szState, nTaskType, nQuestId, bDeploy, nRecycleID )
	local bFound = false;
	for i = 1, table.getn(t_InfoList) do
		if (string.find( t_InfoList[i][2], szTitleName, 1, true)) and szTitleName ~= "" then
			local t_InfoChildList = t_InfoList[i][3];
			local t_TaskDesInfo = { nQuestId, szText, szState, nTaskType, nRecycleID }; --����������Ϣ
			table.insert( t_InfoChildList, t_TaskDesInfo );
			bFound = true;
			return;
		end
	end

	if not bFound then
		local t_RowInfo = { bDeploy, szTitleName, { {nQuestId, szText, szState, nTaskType, nRecycleID } } };
		table.insert( t_InfoList, t_RowInfo );
	end
end

--��ȡ�б��ܵĵ���ʾ����(����չ�����Ӽ��б���������)
function GetAllInfoListRow( t_InfoList )
	local nRow = 0;
	for i = 1, table.getn(t_InfoList) do
		nRow = nRow + 1;
		local t_ChildList = t_InfoList[i][3];
		if t_InfoList[i][1] then
			for j = 1, table.getn(t_ChildList) do
				nRow = nRow + 1;
			end
		end
	end
	return nRow;
end

--������־�������
function TaskLogFrame_ScrollUpBtn_OnClick()
	local slier = getglobal("TaskLogFrame_ScrollBar");
	local nValue = slier:GetValue() - slier:GetValueStep();
	if nValue >= 0 then
		slier:SetValue( nValue );
	end
end

--������־�������
function TaskLogFrame_ScrollDownBtn_OnClick()
	local slier = getglobal("TaskLogFrame_ScrollBar");
	local nMaxValue = slier:GetMaxValue();
	local nValue = slier:GetValue() + slier:GetValueStep();
	if nValue <= nMaxValue then
		slier:SetValue( nValue );
	end
end

--������־�϶����仯ֵ
function TaskLogFrame_ScrollBar_OnValueChanged()
	local slier = getglobal("TaskLogFrame_ScrollBar");
	local nValue = slier:GetValue();
	local nMaxValue = slier:GetMaxValue();
	if nValue <= nMaxValue then
		if nCurDisPage == DISPAGE_QUESTRUN then
			nViewStartPointRun = nValue;
			UpdateTaskInfoDis( nViewStartPointRun, "" );
		elseif nCurDisPage == DISPAGE_CANACCEPTQUEST then
			nViewStartPointCanAccept = nValue;
			UpdateTaskInfoDis( nViewStartPointCanAccept, "" );
		end
	end
end

--���������б�
function UpdateTaskInfoDis( nViewStartPoint, ListType )
	local t_InfoList = nil;
	if nCurDisPage == DISPAGE_QUESTRUN then
		t_InfoList = t_QuestRunList;
		TaskLogFrame_CancelBtn:Show();
		TaskLogFrame_ShareBtn:Show();
		TaskLogFrame_AllQuestChk:Show();
	elseif nCurDisPage == DISPAGE_CANACCEPTQUEST then
		t_InfoList = t_CanAcceptQuestList;
		TaskLogFrame_AllQuestChk:Hide();
	end

	local slier				= getglobal("TaskLogFrame_ScrollBar");
	local upbtn				= getglobal("TaskLogFrame_ScrollUpBtn");
	local downbtn			= getglobal("TaskLogFrame_ScrollDownBtn");
	local sliertex			= getglobal("TaskLogFrame_ScrollBackGroundTex");
	local nAllListDisRow	= GetAllInfoListRow( t_InfoList );
	if nAllListDisRow <= nMaxTitleSign then -- �����ǰȫ������������С�������ʾ���������õ�ǰ��valueֵΪ0
		slier:SetValue( 0 );		
		slier:SetMaxValue( 0 );		
		nViewStartPoint = 0;
	else
		local nMaxValue = nAllListDisRow-nMaxTitleSign; 
		slier:SetMaxValue( nMaxValue );	-- �����ǰ���ӵ������������������ʾ���������õ�ǰ��valueֵ
		--TaskLogFrame_ScrollBarThumbTexture:SetSize(15, slier:GetHeight()*(1-nMaxValue/(nMaxValue+16)));
	end

	if slier:GetMaxValue() > 0 then
		slier:Show();
		upbtn:Enable();
		downbtn:Enable();
	else
		slier:Hide();
		upbtn:Disable();
		downbtn:Disable();
	end
	ShowTaskInfo( t_InfoList, 16, nViewStartPoint, "TaskLogFrameTaskLogRich", "TaskLogFrameTitleSignChk", 50, 50, 5, 30  );
end

--�������ж����б�, �����ʾ������, ��ǰ�����������ʼ��,������ʾ�ı��Ŀؼ�����, ��־+-�Ŀؼ�����, ������ƫ��ֵX , ������ƫ��ֵY,�Ӽ���Ը�����ƫ��ֵX, title��־��ƫ��ֵX
function ShowTaskInfo( t_InfoList, nMaxShowRow, nViewStartPoint, szRichName,szChkName, nFirstPointX, nFirstPointY, nChildOffSetX, nTitleSignOffsetX  )
	HideAllRich(szRichName, nMaxShowRow);
	HideAllChk(szChkName, nMaxShowRow);
	for i = 1, nMaxTitleSign do
		local btn = getglobal("TaskLogFrameTaskLogInfo"..i);
		btn:DisChecked();
	end

	SetTaskInfoList(t_InfoList);
	local nRichClientID = 0;
	local nChkClientID = 0;
	local nCurRow = 0;
	local nCurRowPointX = nFirstPointX;
	local nCurRowPointY = nFirstPointY;
	local nRowHeight = 23;
	for i = 1, table.getn(t_InfoList) do
		nCurRow = nCurRow + 1;

		--��ʾ����
		if nCurRow > nViewStartPoint then
			nRichClientID = nRichClientID + 1;
			if nRichClientID > nMaxShowRow then return end --����������ʾ��Χʱreturn��

			nChkClientID = nChkClientID + 1;
			ShowTitleSign( szChkName, nChkClientID, nRichClientID, nTitleSignOffsetX,nCurRowPointY, i, t_InfoList[i][1]  );

			ShowParentInfo( szRichName, t_InfoList[i][2], nRichClientID, nCurRowPointX, nCurRowPointY );
			nCurRowPointY = nCurRowPointY + nRowHeight;
		end

		--��ʾ�Ӽ�
		if t_InfoList[i][1] == true then
			if nCurRow > nViewStartPoint then
				local chk = getglobal(szChkName..nChkClientID);
				chk:setCheckState(true);
			end
			--chk:SetPoint();
			local t_ChildInfoList = t_InfoList[i][3];
			for j = 1, table.getn(t_ChildInfoList) do
				nCurRow = nCurRow + 1;
				if nCurRow > nViewStartPoint then
					nRichClientID = nRichClientID + 1;
					if nRichClientID > nMaxShowRow then return end --����������ʾ��Χʱreturn��
					local nQuestId  = t_ChildInfoList[j][1];   --����ID
					local nTaskType = t_ChildInfoList[j][4];   --��������

					local btn = getglobal("TaskLogFrameTaskLogInfo"..nRichClientID);
					if btn ~= nil then
						if nQuestId == nCurSelectQuestId and nCurDisPage ==DISPAGE_QUESTRUN then
							SetTaskLogRichSelectHighLight(btn:GetClientID());
						elseif nQuestId == nCurSelectCanAcceptQuestId and nCurDisPage==DISPAGE_CANACCEPTQUEST  then
							SetTaskLogRichSelectHighLight(btn:GetClientID());
						end
					end
					local szTaskDes		= t_ChildInfoList[j][2];  --��������
					local szTaskState	= t_ChildInfoList[j][3];  --����״̬
					local nRecycleID	= t_ChildInfoList[j][5];
					ShowChildInfo( szRichName, szTaskDes, szTaskState, nTaskType, nRichClientID, nCurRowPointX,nCurRowPointY, 
									nChildOffSetX, nQuestId, nRecycleID );
					nCurRowPointY = nCurRowPointY + nRowHeight;
				end
			end
		end
	end
end

--���ø���λ��
--Rich����, ��ʾ�ı�, Rich��ClientID, Richλ��X, Richλ��Y
function ShowParentInfo( szRichName, szText, nRichClientID, nCurRowPointX,nCurRowPointY, bDeploy )
	szRichName = "TaskLogFrameTaskLogInfo";
	local btn = getglobal(szRichName..nRichClientID);
	btn:SetPoint("topleft", "TaskLogFrame", "topleft", nCurRowPointX-40,nCurRowPointY+10);
	btn:Show();
	local tasktracechk = getglobal( "TaskLogFrameTaskLogInfo" .. nRichClientID.. "TaskTraceChk" );
	tasktracechk:Hide();
	local font = getglobal(szRichName..nRichClientID.."TaskLogRich");
	font:SetPoint("topleft", "TaskLogFrame", "topleft", nCurRowPointX,nCurRowPointY+18);
	font:Show();
	font:SetText(szText);
	font:SetTextColor(255,254,189);
	font:SetClientUserData( 0, 0 );
end

--�����Ӽ�λ��
--Rich����, ��ʾ�ı�, Rich��ClientID, Richλ��X, Richλ��Y, ��Ը���ƫ����,����ID
function ShowChildInfo( szRichName, szText, szTaskState, nTaskType, nRichClientID, nCurRowPointX,nCurRowPointY , 
						nChildOffSetX, nQuestId, nRecycleID )
	szRichName = "TaskLogFrameTaskLogInfo";
	local btn = getglobal(szRichName..nRichClientID);
	btn:SetPoint("topleft", "TaskLogFrame", "topleft", nCurRowPointX-40,nCurRowPointY+10);
	btn:Show();

	local font = getglobal(szRichName..nRichClientID.."TaskLogRich");
	font:SetPoint("topleft", "TaskLogFrame", "topleft", nCurRowPointX+10,nCurRowPointY+18);
	font:Show();
	font:SetText( szText);
	font:SetClientUserData( 0, nQuestId );
	font:SetClientUserData( 2, nRecycleID );
	local bTrace = false ;--�Ƿ�Ҫ׷��
	local nNum = Quest:getNumQuestTrace();
	for i = 1, nNum do
		local nQuestTraceId = Quest:getQuestTrace( i -1 );
		if  nQuestId == nQuestTraceId then
			bTrace = true;
			break;
		end
	end

	local taskstatefont = getglobal(szRichName..nRichClientID.."FinishFont");
	taskstatefont:SetPoint("topleft", "TaskLogFrame", "topleft", nCurRowPointX+250,nCurRowPointY+18);
	taskstatefont:Show();
	taskstatefont:SetText( szTaskState);

	local tasktracechk = getglobal( "TaskLogFrameTaskLogInfo" .. nRichClientID.. "TaskTraceChk" );
	if nCurDisPage == DISPAGE_CANACCEPTQUEST then
		tasktracechk:Hide();
	else
		local tasktracechk = getglobal( "TaskLogFrameTaskLogInfo" .. nRichClientID.. "TaskTraceChk" );
		if bTrace then
			tasktracechk:setCheckState(true);
		else
			tasktracechk:setCheckState(false);
		end
		tasktracechk:Show();
	end

	font:SetTextColor(255,170,100);
	if nTaskType == CLIENT_TASK_TYPE_BRANCH then -- ֧��
		font:SetTextColor(155,255,110);
	elseif nTaskType == CLIENT_TASK_TYPE_ACT or nTaskType == CLIENT_TASK_TYPE_TRUST then -- �
		font:SetTextColor(180,150,255);
	end
end

--����title��־λ��
--Chk����, Chk��ClientID, Chkλ��X, Chkλ��Y
function ShowTitleSign(szChkName, nChkClientID, nRichClientID,nTitleSignOffsetX, nCurRowPointY, nTitleIndex, bDeploy )
	local chk = getglobal(szChkName..nChkClientID);
	chk:SetPoint("topleft", "TaskLogFrame", "topleft", nTitleSignOffsetX-8,nCurRowPointY+13);
	chk:setCheckState( bDeploy );
	chk:Show();
	chk:SetClientUserData( 0, nTitleIndex );
	local font = getglobal("TaskLogFrameTaskLogInfo"..nRichClientID.."TaskLogRich");
	font:SetClientUserData( 1, nTitleIndex );
end

function HideAllRich(szRichName, nMaxShowRow)
	for i = 1, nMaxShowRow do
		local rich = getglobal("TaskLogFrameTaskLogInfo"..i.."TaskLogRich");
		local taskstatefont = getglobal("TaskLogFrameTaskLogInfo"..i.."FinishFont");
		rich:SetClientUserData( 0, 0 );
		taskstatefont:SetText("");
		local btn = getglobal("TaskLogFrameTaskLogInfo"..i);
		btn:SetClientUserData( 0, 0 );
		btn:Hide();
	end
end

function HideAllChk(szChkName, nMaxShowRow)
	for i = 1, nMaxShowRow do
		local chk = getglobal(szChkName..i);
		chk:Hide();
	end
end

local t_CurTaskInfoList = {};
function SetTaskInfoList( t_List )
	t_CurTaskInfoList = t_List;
end
function GetTaskInfoList()
	return t_CurTaskInfoList;
end

--����׷�����
--���title���
function TaskLogFrameTitleSignChk_OnClick()
	local szName = this:GetName();
	if string.find(szName, "TaskLogFrameTitleSignChk", 1, true) then
		local nTaskInfoIndex =  this:GetClientUserData(0);
		local t_TaskInfoList = GetTaskInfoList();
		if t_TaskInfoList[nTaskInfoIndex][1] then
			t_TaskInfoList[nTaskInfoIndex][1] = false;
		else
			t_TaskInfoList[nTaskInfoIndex][1] = true;
		end

		if nCurDisPage == DISPAGE_QUESTRUN then
			UpdateTaskInfoDis( nViewStartPointRun, "" );
		elseif nCurDisPage == DISPAGE_CANACCEPTQUEST then
			UpdateTaskInfoDis( nViewStartPointCanAccept,"" );
		end
	end
end

--����׷�����
local nMaxTaskTraceRich = 14;
local nMaxTaskTraceChk = 7;
local nTaskTraceInfoStarPoint = 1;

--��������׷����ʾ�б�
function SetTaskTraceInfoList()
	ClearTable(t_TaskTraceInfoList);
	local nTraceNum = Quest:getNumQuestTrace();
	for i = 1, nTraceNum do
		local nTraceId		= Quest:getQuestTrace( i-1 );
		local nQuestType	= Quest:getQuestType(nTraceId);
		if Quest:IsTrustQuest( nTraceId ) then
			nQuestType = CLIENT_TASK_TYPE_TRUST;
		end

		local szTaskName	= Quest:getQuestName(nTraceId);
		local nState		= Quest:getQuestRunState(nTraceId);
		local szState		= "";
		if nState == CLIENT_TASK_STAT_FINI then
			szState = "����ɡ�"
		end

		if nState == CLIENT_TASK_STAT_FAIL then
			szState = "��ʧ�ܡ�"
		end
		local szText	= "��"..Quest:getQuestTypeName(nQuestType).."��"..szTaskName..szState;
		local szDes		= "";
		if nState == CLIENT_TASK_STAT_FINI then
			szDes = Quest:getQuestFiniTraceDes(nTraceId); 
		else
			szDes = Quest:getQuestNoFiniTraceDes(nTraceId);	  			
			szDes = ReplaceTaskTraceTip( nTraceId, szDes ).szDes;	-- �滻��Ҫ�Ҽ�ʹ�õ�������ƷTips����
		end
		--local t_TaskTraceInfo = { true, szText, szDes, nTraceId };

		local nRecycleID	= Quest:getQuestRunRecycleID(nTraceId);
		local t_TaskTraceInfo = {	needTrace = true, taskDesc = szText, progress = szDes, id = nTraceId, 
									["sceneName"] = Quest:getQusetScene( nTraceId ), ["type"] = Quest:getQuestType( nTraceId ),
									["minLevel"] = Quest:getQuestLevelMin( nTraceId ), ["index"] = table.getn( t_TaskTraceInfoList ),
									["recycleID"] = nRecycleID };
		-- TODO
		table.insert( t_TaskTraceInfoList, t_TaskTraceInfo );
	end
	SortTraceTask();
end
-- ��������ȵ�ͼ��������Type,����������Level,�������
function SortTraceTask()
	if table.getn( t_TaskTraceInfoList ) <= 2 then
		return;
	end

	table.sort( t_TaskTraceInfoList, 
	function ( lhs, rhs )
		if lhs["sceneName"] ~= rhs["sceneName"] then
			return lhs["sceneName"] < rhs["sceneName"];
		end

		if lhs["type"] ~= rhs["type"] then
			return lhs["type"] < rhs["type"];
		end

		if lhs["minLevel"] ~= rhs["minLevel"] then
			return lhs["minLevel"] < rhs["minLevel"];
		end

		return lhs["index"] < rhs["index"];
	end
	);
end

function TaskTraceFrame_OnLoad()
	this:RegisterEvent("GE_QUEST_UPDATECANACCEPT");
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
	for i = 1, nMaxTaskTraceRich do
		local rich = getglobal("TaskTraceFrameTaskTraceRich"..i);
		rich:SetAutoExtend( true );
	end
end

function TaskTraceFrame_OnShow()
	local animationTexture = getglobal( "TaskTraceFrameTaskTraceRich1GuideUVAnimationTex" );
	animationTexture:Hide();
	UpdateTaskTraceInfo(t_TaskTraceInfoList);
	TaskTraceFrame_QuestRunTipBtn:Hide();
	TaskTraceFrame_TitleFont:SetText("�������");
end

function TaskTraceFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		if not GameWizard:isDo(GAMEWIZARD_FIRST_SAWTASKLINK) then
			SetNeedShowTips( true, 0 );
		end
	end
end

function GuideCloseBtn_OnClick()
	local parent = getglobal( this:GetParent() );
	parent:Hide();
end

function TaskTraceFrame_MinimizeBtn_OnClick()
	TaskTraceFrame_MinimizeBtn:Hide();
	GameTooltip:Hide();
	TaskTraceFrame_RestoreBtn:Show();
	TaskTraceFrame_QuestRunTipBtn:Hide();
	
	for i = 1, MAX_TRACE_NUM do
		local TipBtn = getglobal( "TaskTraceFrameTipsBtn"..i );	 
		TipBtn:Hide();
	end	   

	for i = 1, MAX_TRACE_NUM*2 do
		local RichBtn = getglobal( "TaskTraceFrameTaskTraceRich"..i );	 
		RichBtn:Hide();
		local ItemBtn = getglobal( "TaskItemBtn"..i );
		ItemBtn:Hide();
	end
end

function TaskTraceFrame_RestoreBtn_OnClick()
	TaskTraceFrame_RestoreBtn:Hide();
	GameTooltip:Hide();
	TaskTraceFrame_MinimizeBtn:Show();
	
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

--ѡ��ҳ�水Ŧ
function TaskTraceFrame_PageBtn_OnClick()
	local szName = this:GetName();
	for i = 1, 3 do
		local btn = getglobal("TaskTraceFrame_Btn"..i);
		btn:DisChecked();
	end
	this:Checked();
	nTaskTraceInfoStarPoint = nMaxTaskTraceChk*(this:GetClientID()-1) + 1;
	if TaskTraceFrame_RestoreBtn:IsShown() then
		TaskTraceFrame_RestoreBtn_OnClick();
		return;
	end	
	UpdateTaskTraceInfo( t_TaskTraceInfoList );
end

function LastAutoFindFrame_OnLoad()
	this:RegisterEvent( "GE_BEGIN_AUTO_FIND" );
end

function LastAutoFindFrame_OnEvent()
	if arg1 == "GE_BEGIN_AUTO_FIND" then
		if not this:IsShown() then
			--this:Show();
		end
	end
end

-- �������һ���Զ�Ѱ·
function TaskTraceFrame_TourBtn_OnClick()
	ContinueLastAutoFindAfterRide();
end

function ContinueLastAutoFindAfterRide()
	local mainplayer = ActorMgr:getMainPlayer();
	-- �����Ǵ����Զ�Ѱ·״̬,�����κβ���
	if mainplayer:getAutoFind() then
		return;
	end
	local maxControl		= MapShower:GetMaxMapContal();
	local lastFindDestInfo	= maxControl:getLastFindDest();
	if lastFindDestInfo.m_nMapId ~= 0 and lastFindDestInfo.m_bNpcTran then
		maxControl:linkMoveto( lastFindDestInfo.m_nMapId, lastFindDestInfo.m_Dest.m_nNpcResId );
	elseif lastFindDestInfo.m_nMapId ~= 0 then
		maxControl:linkMoveto( lastFindDestInfo.m_nMapId, lastFindDestInfo.m_Dest.m_Pos.X, lastFindDestInfo.m_Dest.m_Pos.Y );
	end
end

function TaskTraceFrame_TourBtn_OnEnter()
	local MaxMapControl = MapShower:GetMaxMapContal();
	local lastFindDestInfo = MaxMapControl:getLastFindDest();
	local szText = "";
	if lastFindDestInfo.m_nMapId ~= 0 and lastFindDestInfo.m_bNpcTran then
		szText = "\n�ϴ����Ի�NPC��"..GetNpcName( lastFindDestInfo.m_Dest.m_nNpcResId ).."��λ��"
					..getMapName( lastFindDestInfo.m_nMapId ).."��ͼ";
	elseif lastFindDestInfo.m_nMapId ~= 0 then
		szText = "\n�ϴ��Զ�Ѱ·�ĵص�λ��"..getMapName( lastFindDestInfo.m_nMapId )
					.."��ͼ�����꣨"..math.floor( lastFindDestInfo.m_Dest.m_Pos.X / 100 + 0.5 ).."��"
					..math.floor( lastFindDestInfo.m_Dest.m_Pos.Y / 100 + 0.5 ).."��";
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "����Զ�Ѱ·"..szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TaskTraceFrame_TourBtn_OnLeave()
	GameTooltip:Hide();
end

function TaskTraceFrame_MinimizeBtn_OnEnter()
	local szText = "��������׷��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TaskTraceFrame_RestoreBtn_OnEnter()
	local szText = "��ʾ����׷��";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
						button = this:GetName() } );
end

function TaskTraceFrame_RestoreBtn_OnLeave()
	GameTooltip:Hide();
end

function ReSetTaskTraceInfo()
	for i = 1, nMaxTaskTraceChk do
		local btn = getglobal("TaskTraceFrameTipsBtn"..i);
		btn:Hide();
	end
	
	for i = 1, nMaxTaskTraceRich do
		local rich = getglobal("TaskTraceFrameTaskTraceRich"..i);
		rich:Hide();
		rich:SetClientUserData( 0, 0 );
	end
	
	for i= 1, MAX_TRACE_PAGE do
		local btn = getglobal("TaskTraceFrame_Btn"..i);
		btn:Disable();
	end
	
	for i = 1, nMaxTaskTraceChk do
		local itembtn		= getglobal( "TaskItemBtn"..i ); 
		itembtn:Hide();
		itembtn:SetClientUserData(0,0);
	end
	TaskTraceFrame_QuestRunTipBtn:Hide();
end

local nWidth = 200;
--local t_TaskTraceInfoList = { "�Ƿ�չ��", "��������(���������͡���������(����������)  )",  "����δ��ɸ�������(����������ɺ�׷������)", "����Id" };
function UpdateTaskTraceInfo( t_TaskTraceInfoList )	 
	ReSetTaskTraceInfo();
	BindTaskTraceInfo( t_TaskTraceInfoList );
end

function CanShowTaskTraceFrame()
	--GameExchange:test( "table.getn(t_TaskTraceInfoList) = "..table.getn(t_TaskTraceInfoList).."--CanShowTaskTraceFrame--1468" );
	if table.getn(t_TaskTraceInfoList) == 0 then
		return false;
	end
	
	return Quest:isQuestTraceAll();
end

--�Ƚ��ճ������ú�Ȼ�����ͺ���������
function SortTraceTaskList()
	if table.getn( t_TaskTraceInfoList ) <= 2 then
		return;
	end

	table.sort( t_TaskTraceInfoList, 
	function ( lhs, rhs )
		if lhs["type"] == CLIENT_TASK_TYPE_DAILY_GOAL and rhs["type"] ~= CLIENT_TASK_TYPE_DAILY_GOAL then
			return lhs["type"] < rhs["type"];
		end

		if lhs["type"] ~= rhs["type"] then
			return lhs["type"] < rhs["type"];
		end

		return lhs["index"] < rhs["index"];
	end
	);
end

function BindTaskTraceInfo( t_TaskTraceInfoList )
	local frame = getglobal("TaskTraceFrame");

	SortTraceTaskList();

	if table.getn(t_TaskTraceInfoList) == 0 then
		-- ����ǰ���Ը��ٵ�����������0��
		-- ��û��ѡ��ȫ�������Ҹ��������ʾ�ţ������ظ������
		frame:Hide();
		return;
	else
		if not frame:IsShown() and not CameraCoverFrame:IsShown() then
			frame:Show();
		end
	end

	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	
	local nMaxBtnIndex = 0;
	for i = 1, math.ceil( table.getn( t_TaskTraceInfoList ) / MAX_TRACE_NUM ) do
		if i > MAX_TRACE_PAGE then
			break;
		end
		local btn = getglobal("TaskTraceFrame_Btn"..i);
		btn:Enable();
		nMaxBtnIndex = i;
	end
	
	if nMaxBtnIndex > 0 then
		local maxCanClickPageBtn = getglobal("TaskTraceFrame_Btn"..nMaxBtnIndex);
		local nMaxTaskTraceInfoStarPoint = nMaxTaskTraceChk*(maxCanClickPageBtn:GetClientID()-1) + 1;
		if nTaskTraceInfoStarPoint > nMaxTaskTraceInfoStarPoint then
			nTaskTraceInfoStarPoint = nMaxTaskTraceInfoStarPoint;
			maxCanClickPageBtn:Checked();
		end
	end
	
	-- TODO: ������������׷�ٲ���ʾ����ĸ�Դ
	-- ���������б�׷�ٵ�����
	if TaskTraceFrame_RestoreBtn:IsShown() then
		return;
	end

	local nRichClientId		= 0;
	local nChkClientId		= 0;
	local nRowHeight		= 16;
	local nCurRowPointY		= 0;
	local nCurRowPointX		= 15;
	local nChildOffSetX		= 18;
	
	local fScreenScaleY = GetScreenScaleY();
	for i = 1, table.getn( t_TaskTraceInfoList ) do
		local nState = Quest:getQuestRunState(t_TaskTraceInfoList[i]["id"]);
		if nState ~=  CLIENT_TASK_STAT_FINI and Quest:isQuestTimeLimit( t_TaskTraceInfoList[i]["id"] ) then
			local nTaskEndTime	= Quest:getTaskEndTime( t_TaskTraceInfoList[i]["id"] );
			if nTaskEndTime <= 0 then
				nState =  CLIENT_TASK_STAT_FAIL;
			end
		end
		
		--������ʾ������׷������ʱreturn��
		--print( "i = "..i.."��nTaskTraceInfoStarPoint = "..nTaskTraceInfoStarPoint );
		if i >= nTaskTraceInfoStarPoint + nMaxTaskTraceChk then
			return
		end
		
		if i >= nTaskTraceInfoStarPoint then
			nRichClientId = nRichClientId + 1;
			local rich = getglobal( "TaskTraceFrameTaskTraceRich"..nRichClientId );
			rich:SetLinkTextColor(217,106,8);
			rich:resizeRect( 250 * fScreenScaleY, 16 * fScreenScaleY );
			rich:SetSizeNoRecal( 250, 16 );
			rich:SetFontType(FONTSYTLE_BORDER);
			rich:Show();
			local t_oneStateColor = GetTaskTraceColor( { ["state"] = nState } );
			if nState == CLIENT_TASK_STAT_FINI then
				rich:SetText(	t_TaskTraceInfoList[i]["taskDesc"], t_oneStateColor[1], t_oneStateColor[2],	t_oneStateColor[3] );
			else
				rich:SetText(	t_TaskTraceInfoList[i]["taskDesc"],	t_oneStateColor[1], t_oneStateColor[2],	t_oneStateColor[3] );
			end
			nCurRowPointY = nCurRowPointY + nRowHeight;
			rich:SetPoint( "topleft", "TaskTraceFrame", "topleft", nCurRowPointX+3, nCurRowPointY+10 );

			nChkClientId = nChkClientId + 1;
			local btn = getglobal("TaskTraceFrameTipsBtn"..nChkClientId);
			btn:SetPoint("topleft", "TaskTraceFrame", "topleft", 1, nCurRowPointY+10);
			btn:Show();
			btn:SetClientUserData( 0, t_TaskTraceInfoList[i]["id"] );	
			btn:SetClientUserData( 1, t_TaskTraceInfoList[i]["recycleID"] );
		end

		if t_TaskTraceInfoList[i]["needTrace"] and i >= nTaskTraceInfoStarPoint then
			nRichClientId = nRichClientId + 1;
			if nRichClientId > nMaxTaskTraceRich*2 then return end
			local rich = getglobal( "TaskTraceFrameTaskTraceRich"..nRichClientId);
			rich:resizeRect( 250 * fScreenScaleY, 16 * fScreenScaleY );
			rich:SetSizeNoRecal( 250, 16 );
			rich:SetLinkTextColor(217,106,8);
			rich:SetFontType(FONTSYTLE_BORDER);
			rich:SetPoint( "topleft", "TaskTraceFrame", "topleft", nCurRowPointX +nChildOffSetX-15, nCurRowPointY+nRowHeight+10 );
			rich:setAbsRect( rich:GetRealLeft(), rich:GetRealTop(), rich:GetRealLeft()+ rich:GetWidth(), rich:GetRealTop() + 80);
			local nQuestId			= t_TaskTraceInfoList[i]["id"];
			local szNoFiniTraceDes	= Quest:getQuestNoFiniTraceDes(nQuestId);--��ȡ����δ�������׷����ʾ
			local szDes				= t_TaskTraceInfoList[i]["progress"];  --׷������
			local t_oneStateColor = GetTaskTraceColor( { ["state"] = nState } );
			local nRecycleID		= t_TaskTraceInfoList[i]["recycleID"];
			--������Ϣ��ʾ
			local szRecycleDesc = Quest:getRecycleDes( nQuestId, nRecycleID )
			if string.find( szRecycleDesc, "��������" ) then
				local nPos = string.find( szRecycleDesc, "���ڽ���" )
				szRecycleDesc = "\t"..string.sub( szRecycleDesc, nPos, string.len(szRecycleDesc) )
			end
			if nState == CLIENT_TASK_STAT_FINI then
				rich:SetText( szRecycleDesc..ReplaceTaskTraceTip(nQuestId, szDes).szDes, t_oneStateColor[1],t_oneStateColor[2],
								t_oneStateColor[3]);
			else
				--print( "szNoFiniTraceDes = "..szNoFiniTraceDes.."--1519" );
				--writePrivateLog( "szNoFiniTraceDes = "..szNoFiniTraceDes );
				--print( "szNoFiniTraceDes = "..szNoFiniTraceDes );
				local szReplacedText = ReplaceTaskTraceTip(nQuestId, szNoFiniTraceDes).szDes;
				--writePrivateLog( "szReplacedText = "..szReplacedText );
				rich:SetText(	szRecycleDesc..szReplacedText, t_oneStateColor[1],
								t_oneStateColor[2],t_oneStateColor[3]	);
				
			end
			
			--[[
			local t_questStateDesc = 
			{
			[CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT]		= "����",
			[CLIENT_TASK_STAT_FINI]		= "���",
			[CLIENT_TASK_STAT_FAIL]		= "ʧ��",
			[CLIENT_RECYCLE_TASK_STAT_CAN_ACCEPT]	= "���ύ",
			};
			--]]
			if Quest:isQuestTimeLimit( nQuestId ) and nState ~= CLIENT_TASK_STAT_FINI then
				local nTaskEndTime	= Quest:getTaskEndTime( nQuestId );
				local nMin		= math.floor( nTaskEndTime / 60 );
				local nSecond	= math.floor( nTaskEndTime - nMin * 60 );
				local szMinText	= ( nMin >= 100 ) and string.format( "%03d", nMin ) or string.format( "%02d", nMin );
				--[[
				local nHour		= string.format( "%02d", nTaskEndTime / 3600 );
				local nMin		= os.date( "%M", nTaskEndTime );
				local nSecond	= os.date( "%S", nTaskEndTime );
				--]]
				local t_oneStateColor = GetTaskTraceColor( { ["state"] = nState } );
				if nTaskEndTime > 0 and nState == CLIENT_NORMAL_TASK_STAT_CAN_ACCEPT then
					--print( Quest:getQuestName( nQuestId ).." nTaskEndTime = "..nTaskEndTime.."nState = "..t_questStateDesc[nState].."--1454" );
					rich:AddText("����ʣ��ʱ�䣺#R"..szMinText.."��"..string.format( "%02d", nSecond ).."��", t_oneStateColor[1],
																								t_oneStateColor[2], t_oneStateColor[3] );
				elseif nTaskEndTime > 0 and nState == CLIENT_TASK_STAT_FAIL then
				elseif nTaskEndTime <= 0 then
					--print( Quest:getQuestName( nQuestId ).." nTaskEndTime = "..nTaskEndTime.."nState = "..t_questStateDesc[nState].."--1456" );
				end
			end
			
			if Quest:isTimeAccumQuest( nQuestId ) and nState ~= CLIENT_TASK_STAT_FINI and nState ~= CLIENT_TASK_STAT_FAIL then
				local timeQuestInfo = Quest:getTimeAccumQuest( nQuestId );
				--local nHour		= string.format( "%02d", nTaskEndTime / 3600 );
				local nMin;
				local nSecond;
				local nTaskTime	= timeQuestInfo.m_nMaxTime - timeQuestInfo.m_nPassTime;
				if timeQuestInfo.m_nTimeMode == 1 then	-- ����ʱ
					nTaskTime	= timeQuestInfo.m_nMaxTime - timeQuestInfo.m_nPassTime;
				elseif timeQuestInfo.m_nTimeMode == 0 then	-- ����ʱ
					nTaskTime	= timeQuestInfo.m_nPassTime;
				end
				nMin	= math.floor( nTaskTime / 60 );
				nSecond	= math.floor( nTaskTime - nMin * 60 );
				local szMinText	= ( nMin >= 100 ) and string.format( "%03d", nMin ) or string.format( "%02d", nMin );
				local t_oneStateColor = GetTaskTraceColor( { ["state"] = nState } );
				if timeQuestInfo.m_nTimeMode == 1 then	-- ����ʱ
					rich:AddText("����ʣ��ʱ�䣺#R"..szMinText.."��"..string.format( "%02d", nSecond ).."��", t_oneStateColor[1],
																						t_oneStateColor[2], t_oneStateColor[3] );
				elseif timeQuestInfo.m_nTimeMode == 0 then	-- ����ʱ
					rich:AddText("�Ѿ��ۻ�ʱ�䣺#R"..szMinText.."��"..string.format( "%02d", nSecond ).."��", t_oneStateColor[1],
																							t_oneStateColor[2], t_oneStateColor[3] );
				end
			end

			if nState == CLIENT_TASK_STAT_FAIL then
				rich:SetLinkTextColor(255,105,255)
				rich:AddText("������ʧ�ܣ�#L�����������@@��������" .. nQuestId .. "#n", t_oneStateColor[1], t_oneStateColor[2], t_oneStateColor[3] );
			end

			rich:SetSize(rich:GetWidth(),rich:GetTotalHeight()/GetScreenScaleY());
			nCurRowPointY = nCurRowPointY + rich:GetTotalHeight()/GetScreenScaleY();
			rich:Show();
			rich:SetClientUserData( 0, nQuestId );

			if Quest:getQuestType(nQuestId) == CLIENT_TASK_TYPE_RONGLU then
				local id = MagicSmelterManager:getRongluRecastEndID();
				if id ~= 0 then
					szNoFiniTraceDes = string.gsub( szNoFiniTraceDes, "#TaskItem(%d+)", "#TaskItem" .. id );
				end
			end

			local Container				= MainPlayer:getContainer();
			local nTaskItemId			= ReplaceTaskTraceTip(nQuestId, szNoFiniTraceDes).nItemId;
			local bBlink				= ReplaceTaskTraceTip(nQuestId, szNoFiniTraceDes).bBlink;
			
			if nTaskItemId ~= 0 and Container:getItemCount( tonumber(nTaskItemId) ) ~= 0
				and Quest:getQuestRunState(nQuestId) ~= CLIENT_TASK_STAT_FINI then

				local nCurLineWidth = rich:GetLineWidth( 1 ); 
				for i = 1, rich:GetTextLines() do
					local nLineWidth = rich:getLineWidth( i - 1 );
					if nCurLineWidth < nLineWidth then
						nCurLineWidth = nLineWidth;
					end
				end

				local titlerich = getglobal( "TaskTraceFrameTaskTraceRich"..(nRichClientId-1));
				if titlerich ~= nil and titlerich:GetLineWidth( 1 ) > nCurLineWidth then
					nCurLineWidth = titlerich:GetLineWidth( 1 ); 
				end

				local itembtn		= getglobal( "TaskItemBtn"..nChkClientId ); 
				local itembtnTex	= getglobal( "TaskItemBtn"..nChkClientId.."IconTexture" ); 
				itembtn:SetPoint( "topleft", "TaskTraceFrame", "topleft", ( nCurRowPointX + 10 + nCurLineWidth / GetScreenScaleY() ),nCurRowPointY-6 );
				itembtn:SetClientUserData( 2, ( nCurRowPointX + 10 + nCurLineWidth / GetScreenScaleY() ) )
				itembtn:SetClientUserData( 3, nCurRowPointY-6 )
				itembtn:SetClientString( bBlink )
				local ItemDef	= getItemDef(nTaskItemId);
				local IconPath	= GetItemEquipIconPath()..ItemDef.IconID..".tga";
				-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				itembtnTex:SetTexture( IconPath );
				itembtn:SetClientUserData( 0,nTaskItemId ); 	
				itembtn:SetClientUserData( 1,nQuestId ); 
				itembtn:Show();

				if Container:isItemInCD( nTaskItemId ) then
					local cd		= Container:getItemCD( nTaskItemId );
					local starttime = Container:getItemCDPassTime( nTaskItemId );
					itembtn:SetCooldownTimer( starttime, starttime + cd, true );
				else
					itembtn:SetCooldownTimer( 0, 0, true );
				end
			end
		end
	end
end

function TaskItem_OnHide()
	if NormalItemTipsFrame:GetClientString() =="����׷����Ʒ" and NormalItemTipsFrame:IsShown() then
		NormalItemTipsFrame:Hide();
	end
end

function TaskItem_OnShow()
	--[[
	local xinShouBtn = getglobal( this:GetName().."GuideBtn" );
	local rich = getglobal( this:GetName().."GuideBtnRich" );
	local nSelfDefineExtendWidth = 10;
	local szText ="��Ҫ��ҩ���Ⱦ��Ұ�";
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:Show();
	--]]
end

--�������׷��Rich
function TaskTraceFrameTaskTraceRich_OnClick()
	-- �ر� ����ָ�� ����
	local t_GuideFrame =
		{
			{"TaskTraceFrameTaskTraceRich1Guide","TaskTraceFrameTaskTraceRich2"}
		};
	for i = 1, table.getn( t_GuideFrame ) do
		local frame = getglobal( t_GuideFrame[i][1] );
		if frame:IsShown() and t_GuideFrame[i][2] == this:GetName() then
			SetNeedShowTips( false );
			frame:Hide();
		end
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end

	if arg1 == "LeftButton" then

	elseif ( arg1 == "RightButton" or arg3 == "RightButton" ) then

	elseif ( arg3 == "LeftButton" ) then
		local strLinkItemHref	= arg2;
		if strLinkItemHref == "�����" then
			PromotionFrame:Show();
			return
		elseif strLinkItemHref == "�ҵĻ���" then
			TradeShopFrameStorageBtn_OnClick();
			return;
		elseif string.find( strLinkItemHref, "��������" ) then
			local id = string.gsub( strLinkItemHref,"��������","" )
			id = tonumber(id)
			MessageBox("��ʾ", "ȷ�Ϸ�������" .. Quest:getQuestName(id) .. "��\n����Q������������־-�ɽ������ѯ��");
			MessageBoxFrame:SetClientString("ȷ�Ϸ�������");			
			MessageBoxFrame:SetClientUserData( 0, id );
			return 
		elseif string.find( strLinkItemHref,"url=",1,true ) then
			local szUrl = string.gsub( strLinkItemHref, "url=","" )
			if szUrl ~= nil or szUrl ~= "" then
				GameMgr:openWebSite( szUrl )
				return
			end
		elseif string.find( strLinkItemHref, "�򿪽��������" ) then
			local num = string.gsub( strLinkItemHref, "[^%d]", "" );
			local id, npcId = 0, 0;
			if string.sub( num, 1, 1 ) ~= "0" and string.len(num) > 7 then
				id = tonumber(string.sub( num, 7, string.len(num)));
				npcId = tonumber(string.sub( num, 1, 6 ));
			else
				id = tonumber(string.sub( num, 2, string.len(num)));
			end
			local recycleId = Quest:getQuestRunRecycleID( id );
			local szDes = Quest:getQuestFiniDialog( id );
			if TaskAcceptFrame:IsShown() then
				TaskAcceptFrame:Hide();
			elseif NpcTalkFrame:IsShown() then
				TaskAwardFrame:Hide();
			end
			local frame = getglobal("TaskAwardFrame");
			frame:SetClientUserData( 0, npcId );
			frame:SetClientUserData( 1, id );
			frame:SetClientUserData( 2, recycleId );
			frame:SetClientString( szDes );
			frame:Show();
		end
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

function GetLastShowTipsTask()
	return nLastTask;
end

function SetNeedShowTips( state, id )
	bNeedTips = state;
	if id then
		nLastTask = id;
	end
end

function UpdateTaskTraceFrameTaskTraceRichGuide()
	local animationTexture = getglobal( "TaskTraceFrameTaskTraceRich1GuideUVAnimationTex" );
	local t_Name = {};
	if Quest:isQuestRun( 1049 ) then
		table.insert( t_Name, { "������", 1.7, 13 } );
	elseif Quest:isQuestRun( 1050 ) then
		table.insert( t_Name, { "���ɶ�", 1.7, 13 } );
	elseif Quest:isQuestRun( 1051 ) then
		table.insert( t_Name, { "�±�ǩ����", 1.7, 21 } );
		table.insert( t_Name, { "���ɶ�", 1.7, 13 } );
	elseif Quest:isQuestRun( 1052 ) then
		table.insert( t_Name, { "����", 1.7, 13 } );
		table.insert( t_Name, { "�ȶ�", 1.7, 9 } );
	elseif Quest:isQuestRun( 1053 ) then
		table.insert( t_Name, { "�Ͷ�", 1.7, 9 } );
	elseif Quest:isQuestRun( 1054 ) then
		table.insert( t_Name, { "�ȶ�", 1.7, 9 } );
	elseif Quest:isQuestRun( 1055 ) then
		table.insert( t_Name, { "��ʯ", 1.7, 9 } );
		table.insert( t_Name, { "����", 1.7, 9 } );
	elseif Quest:isQuestRun( 1056 ) then
		table.insert( t_Name, { "����", 1.7, 9 } );
	elseif Quest:isQuestRun( 1057 ) then
		table.insert( t_Name, { "�ȶ�", 1.7, 9 } );
	elseif Quest:isQuestRun( 1058 ) then
		table.insert( t_Name, { "������", 1.7, 21 } );
		table.insert( t_Name, { "����", 1.7, 9 } );
	elseif Quest:isQuestRun( 1059 ) then
		table.insert( t_Name, { "��", 1.7, 9 } );
		table.insert( t_Name, { "����", 1.7, 9 } );
	--elseif Quest:isQuestRun( 1060 ) then
	--	table.insert( t_Name, { "����", 1.7, 9 } );
	elseif Quest:isQuestRun( 1062 ) then
		table.insert( t_Name, { "����", 1.7, 9 } );
	elseif Quest:isQuestRun( 1064 ) then
		table.insert( t_Name, { "����", 1.7, 9 } );
		table.insert( t_Name, { "����", 1.7, 9 } );
	elseif Quest:isQuestRun( 1065 ) then
		table.insert( t_Name, { "���ݻ�", 1.7, 13 } );
		table.insert( t_Name, { "������", 1.7, 13 } );
	elseif Quest:isQuestRun( 1066 ) then
	--	table.insert( t_Name, { "ħ����", 1.7, 13 } );
		table.insert( t_Name, { "������", 1.7, 13 } );
	elseif Quest:isQuestRun( 1068 ) then
		table.insert( t_Name, { "�����˹", 1.7, 17 } );
	elseif Quest:isQuestRun( 1069 ) then
		table.insert( t_Name, { "�Ĺ�����", 1.7, 17 } );
		table.insert( t_Name, { "������˹", 1.7, 17 } );
	elseif Quest:isQuestRun( 1070 ) then
		table.insert( t_Name, { "ħ��ʯ", 1.7, 13 } );
		table.insert( t_Name, { "�����˹", 1.7, 17 } );
	elseif Quest:isQuestRun( 1071 ) then
		table.insert( t_Name, { "����������", 1.7, 21 } );
		table.insert( t_Name, { "���޷�", 1.7, 13 } );
	elseif Quest:isQuestRun( 1072 ) then
		table.insert( t_Name, { "������˹", 1.7, 17 } );
	elseif Quest:isQuestRun( 1073 ) then
		table.insert( t_Name, { "��ɫ������", 1.7, 21 } );
		table.insert( t_Name, { "������˹", 1.7, 17 } );
	elseif Quest:isQuestRun( 1074 ) then
		table.insert( t_Name, { "������", 1.7, 13 } );
		table.insert( t_Name, { "��ɯ", 1.7, 9 } );
	--elseif Quest:isQuestRun( 1075 ) then
	--	table.insert( t_Name, { "���޷�", 1.7, 13 } );
	elseif Quest:isQuestRun( 1076 ) then
		table.insert( t_Name, { "ϣ��ϣ��", 1.7, 17 } );
	elseif Quest:isQuestRun( 1077 ) then
		table.insert( t_Name, { "������", 1.7, 13 } );
	elseif Quest:isQuestRun( 1078 ) then
		table.insert( t_Name, { "̫�ղղк�", 1.7, 21 } );
		table.insert( t_Name, { "������˹", 1.7, 17 } );
	elseif Quest:isQuestRun( 1079 ) then
		table.insert( t_Name, { "���޷�", 1.7, 13 } );
	elseif Quest:isQuestRun( 1080 ) then
		table.insert( t_Name, { "������", 1.7, 13 } );
		table.insert( t_Name, { "���޷�", 1.7, 13 } );
	elseif Quest:isQuestRun( 1081 ) then
		table.insert( t_Name, { "����", 1.7, 9 } );
	end

	local rich		= getglobal( "TaskTraceFrameTaskTraceRich2" );
	local linkTextRect	= nil;
	local scale, pos = 1.7, 9;
	for i=1, #t_Name do
		linkTextRect	= rich:getLinkTextRect( t_Name[i][1] );
		if linkTextRect:getWidth() > 0 then
			scale, pos = t_Name[i][2], t_Name[i][3];
			break;
		end
	end
	if linkTextRect and linkTextRect:getWidth() > 0 then
		animationTexture:SetSize( ( linkTextRect:getWidth() * scale ) / GetScreenScaleY(), ( linkTextRect:getHeight() * 1.8 ) / GetScreenScaleY() );
		animationTexture:SetPoint( "topleft", "UIClient", "topleft", ( linkTextRect.m_nLeft ) / GetScreenScaleY() - pos, 
									( linkTextRect.m_nTop ) / GetScreenScaleY() - 6 );
		animationTexture:SetUVAnimation( 50, true );
		animationTexture:Show();
		TaskTraceFrameTaskTraceRich1GuideRichFont:SetText( "����»�������\n�����Զ�Ѱ·",	86, 79, 54);
		TaskTraceFrameTaskTraceRich1Guide:Show();
	else
		animationTexture:Hide();
		TaskTraceFrameTaskTraceRich1Guide:Hide();
	end
end

function TaskTraceFrameTaskTraceRich1_OnUpdate()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SAWTASKLINK) and bNeedTips then
		UpdateTaskTraceFrameTaskTraceRichGuide();
	end
end

--�ر�����׷��Tips
function TaskTraceFrame_QuestRunTipCloseBtn_OnClick()
	TaskTraceFrame_QuestRunTipBtn:Hide();
end

--��ȡ�����ӵ�λ��( 1002, 2568, 2698 );
function GetLinkPos( szLinkText )
	local nLength	= string.len( szLinkText );
	local cStar		= string.sub( szLinkText, 1, 1 );
	local cEnd		= string.sub( szLinkText, nLength, nLength );

	local nMapId	= 0;
	local nMapPosX	= 0;
	local nMapPosY	= 0;
	if not ( cStar == '(' and cEnd == ")" ) then
		return nMapId, nMapPosX, nMapPosY;
	end
	
	-- mapID = ?, posX = ?, posY = ?
	local t_allPositiones = {};
	-- �������
	local nLeftBracketPos = 1;
	while nLeftBracketPos ~= nil do
		local nFirstPos		= string.find( szLinkText, ",", nLeftBracketPos,	nLength );
		local nSecondPos	= string.find( szLinkText, ",", nFirstPos	+ 1,	nLength );
		local nThirdPos		= string.find( szLinkText, ")", nSecondPos	+ 1,	nLength );

		local szMapId	= string.sub( szLinkText, nLeftBracketPos + 1,	nFirstPos	- 1 );
		local szMapX	= string.sub( szLinkText, nFirstPos  + 1,		nSecondPos	- 1 );
		local szMapY	= string.sub( szLinkText, nSecondPos + 1,		nThirdPos	- 1 );
		nMapId		= tonumber(szMapId);
		nMapPosX	= tonumber(szMapX);
		nMapPosY	= tonumber(szMapY);
		--print( "nMapId = "..nMapId.."�� nMapPosX = "..nMapPosX.."�� nMapPosY = "..nMapPosY );
		table.insert( t_allPositiones, { ["mapID"] = nMapId, ["posX"] = nMapPosX, ["posY"] = nMapPosY } );

		nLeftBracketPos = string.find( szLinkText, "(", nThirdPos	+ 1,	nLength );
	end

	local nIndex = 1;
	local nElemSize = table.getn( t_allPositiones );
	if nElemSize > 1 then
		math.randomseed( GameMgr:getTickTime() );
		nIndex = math.random( 1, nElemSize );
	end
	
	local selectPosInfo = t_allPositiones[nIndex];
	return selectPosInfo["mapID"], selectPosInfo["posX"], selectPosInfo["posY"];
end

function GetNpcPos( szLinkText )
	local nLength = string.len( szLinkText );
	local cStar = string.sub( szLinkText, 1, 1 );
	local cEnd  = string.sub( szLinkText, nLength, nLength );

	local bLinkNpc	= false;
	local nNpcId	= 0;
	local nTargetMapId = 0;

	if ( cStar ~= '[' and cEnd ~= "]" ) then
		return bLinkNpc, nNpcId,0;
	else
		local nFirstPos = string.find( szLinkText, ",", 1, nLength );

		if nFirstPos == nil then
			return bLinkNpc, nNpcId, 0;
		end

		nNpcId			= tonumber( string.sub( szLinkText, 2, nFirstPos-1 ) );
		nTargetMapId	= tonumber( string.sub( szLinkText, nFirstPos + 1, nLength-1 ) );
		bLinkNpc		= true;
		return bLinkNpc, nNpcId, nTargetMapId;
	end
end

-- �滻��Ҫ�Ҽ�ʹ�õ�������ƷTips����
function ReplaceTaskTraceTip( nQuestId, szTipDes )
	local t_TaskItemInfo = { nItemId = 0, szDes = szTipDes };
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return t_TaskItemInfo;
	end
	if Quest:getQuestMaxGenericProgress(nQuestId) > 0 then
		local progress = {}
		for s, id in enumsubstr(szTipDes, "^(.-)#TaskNum(%d+)#") do
			local _, _, str = string.find(s, "^#r(.*)")
			s = str or s
			id = tonumber(id) or 0
			progress[id] = s
		end
		for i = Quest:getQuestGenericProgress(nQuestId), 0, -1 do
			if progress[i] then
				szTipDes = progress[i]
				break
			end
		end
	end
	
	local item = {}
	local start = 1
	local bDo = false
		
	for s, id, flag, blink in enumsubstr(szTipDes, "^(.-)#TaskItem(%d+):(%d):(%d)#") do
		bDo = true
		local _, _, str = string.find(s, "^#r(.*)")
		s = str or s
		flag = flag ~= "0"
		id = tonumber(id) or 0
		table.insert(item, {id, flag, s, blink})
	end
	while true do
		local _, n = string.find(szTipDes, "^(.-)#TaskItem(%d+):(%d):(%d)#", start)
		if not n then
			break
		end
		start = n + 1
	end
	if not bDo then
		for s, id, flag in enumsubstr(szTipDes, "^(.-)#TaskItem(%d+):(%d)#") do
			local _, _, str = string.find(s, "^#r(.*)")
			s = str or s
			flag = flag ~= "0"
			blink = "1"
			id = tonumber(id) or 0
			table.insert(item, {id, flag, s, blink})
		end	
		while true do
			local _, n = string.find(szTipDes, "^(.-)#TaskItem(%d+):(%d)#", start)
			if not n then
				break
			end
			start = n + 1
		end
	end
	szTipDes = string.sub(szTipDes, start)
	local Container		= MainPlayer:getContainer()
	for i = 1, #item do
		local id = item[i][1]
		local flag = item[i][2]
		local str = item[i][3]
		local blink = item[i][4]
		if not flag or Container:getItemCount(id) ~= 0 then 
			szTipDes = str
			t_TaskItemInfo.nItemId = id
			t_TaskItemInfo.bBlink = blink
			break
		end
	end
	t_TaskItemInfo.szDes = szTipDes
	return t_TaskItemInfo
end

function TaskTraceFrameTipsBtn_OnClick()
	if TaskTraceFrame_QuestRunTipBtn:IsShown() then
		TaskTraceFrame_QuestRunTipBtn:Hide();
		return;
	end

	local nQuestId	= this:GetClientUserData( 0 );
	local nRecycleID= this:GetClientUserData( 1 );

	local szTaskTraceTip = Quest:getQuestRunTip( nQuestId );
	local szRecycleDesc = Quest:getRecycleDes(nQuestId, nRecycleID);
	--print( "szTaskTraceTip = "..szTaskTraceTip.."\n" );
	--print( "szRecycleDesc = "..szRecycleDesc.."\n" );
	local tiprich = getglobal("TaskTraceFrame_QuestRunTipRich");
	local nTipRowHight = 16;
	tiprich:SetText( szTaskTraceTip.."#r"..szRecycleDesc, 255, 255, 255 );
	
	util.ExtendFrameByRich( { frame = "TaskTraceFrame_QuestRunTipBtn", rich = "TaskTraceFrame_QuestRunTipRich", xOffset = 4, yOffset = 20,
								xAdjust = 5, yAdjust = 5 } );
	TaskTraceFrame_QuestRunTipBtn:SetPoint( "bottomright", this:GetName(), "bottomleft", -10, 0 );
	tiprich:SetDispPos( 0 );
	TaskTraceFrame_QuestRunTipBtn:Show();
end

function HideTaskLogDescFrame()
	TaskLogDescFrame:Hide();
end

function TaskItem_OnEnter()
	if this:GetClientUserData( 0 ) ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( this:GetClientUserData( 0 ) ) );
		NormalItemTipsFrame:SetClientString("����׷����Ʒ");
	end
end

function TaskItem_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	local btn = getglobal( this:GetName().."GuideBtn" );
	btn:Hide();
	local Container = MainPlayer:getContainer();
	if this:GetClientUserData( 0 ) ~= 0 then
		-- �Զ�ѡ�񸽽�Ŀ��
		local itemid = this:GetClientUserData( 0 )
		local itemdef = getItemDef( itemid )
		MainPlayer:SelectTarget( itemdef.UseTargetID, itemdef.UseTarget )
		Container:usedItem( itemid );
	end

	if NormalItemTipsFrame:IsShown() then
		NormalItemTipsFrame:Hide();
	end
end

function TaskItem_OnLoad()
	this:RegisterEvent("GE_ITEMUSED_PASSCD");
end

function TaskItem_OnEvent()
	if( arg1 == "GE_ITEMUSED_PASSCD" ) then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end
		if this:GetClientUserData(0) ~= 0 then
			this:SetCooldownTimer( 0, 0, true );
			local cd		= container:getItemCD( this:GetClientUserData(0) );
			local starttime = container:getItemCDPassTime( this:GetClientUserData(0) );
			this:SetCooldownTimer( starttime, starttime+cd, true );
			this:SetCooldownTextureRect( 3, 3, 28, 28 );
		end
	end
end

function ReplaceAllDes( szTipDes, szRepStr, szNewStr )
	while string.find( szTipDes, szRepStr, 1, true ) do
		local nBeginPos		= string.find(szTipDes,szRepStr);	
		local nLength		= string.len(szTipDes);	  
		local szPreText		= string.sub(szTipDes,1,nBeginPos-1);	 
		local szNexText		= string.sub(szTipDes,nBeginPos+8,nLength);
		szTipDes = szPreText..szNewStr..szNexText; 
	end
	return szTipDes;
end

function TaskItem_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local itemId  = this:GetClientUserData( 0 )
	local questId = this:GetClientUserData( 1 )
	local nBlink = tonumber( this:GetClientString() );
	local tex = getglobal( this:GetName() .. "UVAnim" )
	if nBlink == 0 then
		tex:StopAlphaAmin()
		tex:Hide()
		return 
	else
		if nBlink == 2 then
			-- �ж�Ŀ��
			local nTargetId = mainplayer:GetSelectTarget()
			local TargetObj	= ActorMgr:FindActor( nTargetId )
			if TargetObj == nil or not TargetObj:isInClan() or TargetObj:isInSameClan( mainplayer:getClanWID() ) then
				return
			end
		end
		if itemId > 0 then
			local t_questId = { [1064] = 1, [1066] = 1, [1071] = 1, [1072] = 1, [1074] = 1 }
			if t_questId[questId] ~= nil then
				if not tex:IsPlayAlphaAnim() then
					tex:StartAlphaAmin( 1 )
				end
			end

			local container = mainplayer:getContainer()
			local x = this:GetClientUserData( 2 )
			local y = this:GetClientUserData( 3 )
			if container:canUseTaskItem( itemId ) then
				if this:GetAnchorOffsetX() > x then
					this:SetPoint( "topleft", "TaskTraceFrame", "topleft",x,y )
				else
					this:SetPoint( "topleft", "TaskTraceFrame", "topleft",x+2,y+2 )
				end
				--����������Ʒ��һ�ν���Ŀ�귶Χ����
				QuestItemInCanUseArea( questId )
			else			
				this:SetPoint( "topleft", "TaskTraceFrame", "topleft",x,y )
				if tex:IsShown() then
					tex:StopAlphaAmin()
					tex:Hide()
				end
				--����������Ʒ����Ŀ�귶Χ����
				QuestItemNotInCanUseArea( questId )
			end
		end
	end
end

--����������Ʒ��һ�ν���Ŀ�귶Χ����
function QuestItemInCanUseArea( questId )
	if questId == 1066 then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_QUEST_IF_YOU_CAN ) and Quest:isQuestRun( 1066 ) then
			for i = 1, MAX_TRACE_NUM * 2 do
				local taskItemBtn = getglobal( "TaskItemBtn"..i );
				if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1066 then
					local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
					local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
					local nSelfDefineExtendWidth = 10;
					local itemDef = getItemDef( taskItemBtn:GetClientUserData( 0 ) );
					local szText ="������ʹ��";
					rich:SetClientString( szText );
					ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
					xinShouBtn:Show();
				end
			end
		end
	end

	if questId == 1072 then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_QUEST_SAVE_OTHERS ) and Quest:isQuestRun( 1072 ) then
			for i = 1, MAX_TRACE_NUM * 2 do
				local taskItemBtn = getglobal( "TaskItemBtn"..i );
				if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1072 then
					local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
					local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
					local nSelfDefineExtendWidth = 10;
					local itemDef = getItemDef( taskItemBtn:GetClientUserData( 0 ) );
					local szText ="������ʹ��";
					rich:SetClientString( szText );
					ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
					xinShouBtn:Show();
				end
			end
		end
	end
end
--����������Ʒ����Ŀ�귶Χ����
function QuestItemNotInCanUseArea( questId )
	if questId == 1066 then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_QUEST_IF_YOU_CAN ) and Quest:isQuestRun( 1066 ) then
			for i = 1, MAX_TRACE_NUM * 2 do
				local taskItemBtn = getglobal( "TaskItemBtn"..i );
				if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1066 then
					local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
					local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
					rich:SetClientString( "" );
					xinShouBtn:Hide();
				end
			end
		end
	end
	if questId == 1072 then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_QUEST_SAVE_OTHERS ) and Quest:isQuestRun( 1072 ) then
			for i = 1, MAX_TRACE_NUM * 2 do
				local taskItemBtn = getglobal( "TaskItemBtn"..i );
				if taskItemBtn:IsShown() and taskItemBtn:GetClientUserData( 1 ) == 1072 then
					local xinShouBtn = getglobal( taskItemBtn:GetName().."GuideBtn" );
					local rich = getglobal( taskItemBtn:GetName().."GuideBtnRich" );
					rich:SetClientString( "" );
					xinShouBtn:Hide();
				end
			end
		end
	end
end

-- ���ٽ�ȡ����
function doQuickAcceptQuest()
	if NpcTalkFrame:IsShown() then
		local t_NpcTalkFrameControl = GetNpcTalkFrameControl();
		local allOpts = t_NpcTalkFrameControl:getAllOpts();
		-- ��ɵ�����
		for i=1,#allOpts do
			if string.find( allOpts[i]["oneOptContent"],"(���)",1,true ) then
				NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
				return;
			end
		end
		-- �ɽӵ�����
		for i=1,#allOpts do
			if string.find( allOpts[i]["oneOptContent"],"(�ɽ�)",1,true ) then
				NpcDialog:selectOpt( allOpts[i]["optIndex"], ( allOpts[i]["optType"] == NPC_DIALOG_LAST_OPT ) );
				return;
			end
		end
	end
	if TaskAcceptFrame:IsShown() then
		TaskAcceptFrame_AcceptBtn_OnClick()
		return;
	end
	if TaskAwardFrame:IsShown() then
		TaskAwardFrame_GetBtn_OnClick()
		return
	end
	-- ����׷���� ��ÿ��Ŀ���
	for i=1,#t_TaskTraceInfoList do
		local oneTrace = t_TaskTraceInfoList[i]
		if oneTrace.needTrace and oneTrace.type ~= CLIENT_TASK_TYPE_DAILY_GOAL then
			if string.find( oneTrace.taskDesc,"����ɡ�" ) then
				local _,startPos = string.find( oneTrace.progress,"@@",1,true );
				local endPos = string.find( oneTrace.progress,"#n",startPos,true );
				if startPos ~= nil and endPos ~= nil then
					local strLinkItemHref = string.sub( oneTrace.progress, startPos + 1, endPos - 1 )
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
					return
				end
			end
		end
	end
	for i=1,#t_TaskTraceInfoList do
		local oneTrace = t_TaskTraceInfoList[i]
		if oneTrace.needTrace and oneTrace.type ~= CLIENT_TASK_TYPE_DAILY_GOAL then
			local _,startPos = string.find( oneTrace.progress,"@@",1,true );
			local endPos = string.find( oneTrace.progress,"#n",startPos,true );
			if startPos ~= nil and endPos ~= nil then
				local strLinkItemHref = string.sub( oneTrace.progress, startPos + 1, endPos - 1 )
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
				return
			end
		end
	end
end

function ShowRecycleTaskByItem( grid, list, taskId, recycleId )
	TaskAcceptFrame:SetClientUserData(0,0);				--NPCId
	TaskAcceptFrame:SetClientUserData(1,taskId);			--����Id
	TaskAcceptFrame:SetClientUserData(2,recycleId);			--���� ������ȡ����
	TaskAcceptFrame:SetClientUserData(3,grid);			--���� ������ȡ����
	TaskAcceptFrame:SetClientString("������ȡ����");
	local t_TaskAcceptFrameControl = GetTaskAcceptFrameControl();
	t_TaskAcceptFrameControl:setTalkInfo( { npcId = 0, taskId = taskId, isItemTask = true, taskType = ITEM_TASK_TYPE } );
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container		= mainplayer:getContainer();
	local item		= container:getItem( list, grid );
	if item:getItemId() ~= 0 then
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), "TaskAcceptFrame" );
	end
	NpcDialog:send_CloseTalk();
	TaskAcceptFrame:Show();
end