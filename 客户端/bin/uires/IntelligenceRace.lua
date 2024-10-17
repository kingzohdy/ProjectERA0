
local t_IntelligenceRaceInfo = {
					[1] = [[
#c461e0a����ƣ������󿼳�
#c461e0a�NPC��#L����@@[302129,1000]#n#c554623(ʱ���)
#c461e0a���ͼ������
#c461e0a����ȼ���10��]],
					[2] = [[
#c461e0a1.��������ÿ��13��30��19��00��ʽ��
#c461e0a  ���������Ա𣬲���ְҵ���ȼ�����10
#c461e0a  ����ȫ��������Ҷ����Բμӡ�

#c461e0a2.��õĽ����͵�ǰ�ȼ��Լ�����������
#c461e0a  �ء�����Խ�ߣ�����Խ���

#c461e0a3.���������ֵ�������������򲻻��
#c461e0a  �ý�����

#c461e0a4.�������������ڼ䣬ȷ���Լ�û�д���
#c461e0a  �ǰ�ȫ�����������ܲ���Ҫ����ʧ��

#c461e0a5.��ڼ䣬��������ߣ��ٴ����߿���
#c461e0a  ��ʱ���뾺����]],
					 			}
local raceStartTime	= 0;
local beginTimer 	= 29;
local readTimer	 	= 10;
local isReadFlag	= true;
local answerTimer	= 15;
local ChosenAnswer	= 0;
local trueAnswer 	= 0;
local StarNumber	= 3;
local RaceEndFlag	= false;
local RaceEndTime	= 0 ;
local SUBJECT_MAX_TOP 	= 20;
local MAX_OPTION_NUM	= 4;
function IntelligenceRaceJoinFrame_OnShow() 
	local LeftInfoRich	= getglobal("IntelligenceRaceJoinFrameLeftRichInfo");
	local RightInfoRich	= getglobal("IntelligenceRaceJoinFrameRightRichInfo");
	LeftInfoRich:SetLinkTextColor( 10, 100, 0);
	LeftInfoRich:SetText( t_IntelligenceRaceInfo[1],111,82,32 );
	RightInfoRich:SetText( t_IntelligenceRaceInfo[2],70,30,0 );
	local nCurtime 	= getServerTime().sec;
	local beginTime = raceStartTime - nCurtime;
	local stateText = getglobal("IntelligenceRaceJoinFrameStateFont");
	local endTimer	= getglobal("IntelligenceRaceJoinFrameEndTimeFont");
	if raceStartTime > nCurtime then
		stateText:SetText("������ʼ");
		endTimer:SetText("��������ʱ��"..beginTime.."��");
	else
		stateText:SetText("������");
		endTimer:SetText("��������С�");
	end
	IntelligenceRaceJoinFrame:SetPoint("center", "UIClient", "center", 0, 0 );
end

function IntelligenceRaceJoinFrame_OnUpdate()
	local stateText = getglobal("IntelligenceRaceJoinFrameStateFont");
	local endTimer	= getglobal("IntelligenceRaceJoinFrameEndTimeFont");
	local nCurtime 	= getServerTime().sec;
	local beginTime	= raceStartTime - nCurtime;
	if beginTime <= 0 then
		stateText:SetText("������");
		endTimer:SetText("��������С�");
	else
		stateText:SetText("������ʼ");
		endTimer:SetText("��������ʱ��"..beginTime.."��");
		
	end
end
function RaceInvitationShow( startTime, isRaceEnd)
	local mainPlayer = ActorMgr:getMainPlayer();
	if mainPlayer == nil then return end
	if mainPlayer:getLv() < 10 then 
		return;
	end
	if isRaceEnd == 1 then
		IntelligenceRaceJoinFrame:Hide();
		IntelligenceRaceEnd();
	else
		local stateText = getglobal("IntelligenceRaceJoinFrameStateFont");
		local endTimer	= getglobal("IntelligenceRaceJoinFrameEndTimeFont");
		raceStartTime	= startTime;
		
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		if t_centerXinShouBtnControl:getIndexByType( INTELLIGENCE_RACE_JOIN ) == 0 then
			t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = INTELLIGENCE_RACE_JOIN, frame = "IntelligenceRaceJoinFrame", 
													path ="uires\\ui\\Za\\ZhiLiJingSai.tga", index = 0 } );
		end
	end
end

function IntelligenceRaceJoinOkBtn_OnClick()
	IntelligenceRaceJoinFrame:Hide();
	IntelliRaceMgr:IntelligenceRaceJoinAgree();
end
--��ʾ��С����ť
function IntelligenceRaceJoinWaitBtn_OnClick()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( INTELLIGENCE_RACE_JOIN ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = INTELLIGENCE_RACE_JOIN, frame = "IntelligenceRaceJoinFrame", 
												path ="uires\\ui\\Za\\ZhiLiJingSai.tga", index = 0 } );
	end
	t_centerXinShouBtnControl:updateCenterBtns();
	IntelligenceRaceJoinFrame:Hide();
end
--������С����ť
function IntelligenceRaceEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( INTELLIGENCE_RACE_JOIN );
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
end
function IntelligenceRaceFrame_OnShow()
	IntelligenceRaceFrame:SetPoint( "center", "UIClient", "center", 0, 0);
	local szText		= "#cffffbe��У�������֮�ǣ�������У���#n\n#cffffbe��ѡ����ȷ�𰸣���ȷ��100%��";
	IntelligenceRaceFrameStarRich:SetText(szText,255,150,0);
	if StarNumber < 3 then
		for i = StarNumber, 2 do
			local starBtn = getglobal("IntelligenceRaceFrameStarButton"..i + 1) ;
			starBtn:Disable();
			starBtn:DisHighlight();
		end
	end
	if StarNumber == 0 then return end
	for i = 1, StarNumber do
		local starBtn = getglobal("IntelligenceRaceFrameStarButton"..i) ;
		starBtn:Enable();
	end	
end
function IntelligenceRaceFrame_OnHide()
	RaceEndFlag = false;
	RaceEndTime = 0;
	IntelligenceRaceFrame_QuestionBtnModelView:Hide();
	IntelligenceRaceFrame_EndBtnModelView:Hide();
end
function IntelligenceRaceFrame_OnUpdate()
	-- ��������� ��������200��
	local nCurtime 	= getServerTime().sec;
	if RaceEndTime ~= 0 then
		if nCurtime - RaceEndTime > 200 then
			IntelligenceRaceFrame:Hide();
		end
	end
	local startTime = beginTimer - nCurtime;
	local readTime	= readTimer - nCurtime;
	local replyTime	= answerTimer - nCurtime;
	local AnswerTimerTxt = getglobal("IntelligenceRaceFrameAnswerTimer");
	if IntelligenceRaceFrame_TimerBtn:IsShown() then
		if beginTimer <= 0 then return end
		local raceTimerTxt = getglobal("IntelligenceRaceFrame_TimerBtnEndTimer");	
		raceTimerTxt:SetText(startTime.."��");
	elseif IntelligenceRaceFrame_QuestionBtn:IsShown() then
		if readTime > 0 then
			AnswerTimerTxt:SetText("����ʱ�䵹��ʱ��"..readTime.."��");
		elseif replyTime > 0 then
			AnswerTimerTxt:SetText("����ʱ�䵹��ʱ��"..replyTime.."��");
			ChangeOptionTextColor();
		end
	elseif IntelligenceRaceFrame_EndBtn:IsShown() then
		AnswerTimerTxt:SetText("");
	end 
end

function SetIntelliRaceSelfResult( nTop, nScore, bEnd )
	local desFont;
	local szAdd = "";
	if bEnd then
		desFont = getglobal( "IntelligenceRaceFrame_EndBtnSelfTop" )
	else
		desFont = getglobal( "IntelligenceRaceFrameSelfRankFont" )
		szAdd = "�������У�"
	end
	if nTop <= 20 then
        desFont:SetText( szAdd..nTop);
	else
		if nScore >= 200 then
			if nTop < 200 then
   			     desFont:SetText( szAdd..nTop );
			elseif nTop <= 300 then
   			     desFont:SetText( szAdd.."200+" );
			elseif nTop <= 400 then
   			     desFont:SetText( szAdd.."500+" );
			else
   			     desFont:SetText( szAdd.."1000+" );
			end
		elseif nScore >= 100 then
			if nTop < 200 then
   			     desFont:SetText( szAdd.."200+" );
			elseif nTop < 500 then
   			     desFont:SetText( szAdd.."500+" );
			else
   			     desFont:SetText( szAdd.."1000+" );
			end
		elseif nScore >= 50 then
			if nTop < 500 then
   			     desFont:SetText( szAdd.."500+" );
			else
   			     desFont:SetText( szAdd.."1000+" );
			end
		elseif nScore >= 0 then
   			desFont:SetText( szAdd.."1000+" );
		end
	end
end

function UpdateRaceUIInfo()
    
    local FlushInfo     = IntelliRaceMgr:getSubJectFlushInfo();
    local subJectSelf   = FlushInfo.Self;
    local AwardTop      = FlushInfo.Award.Top;
    local AwardBase     = FlushInfo.Award.Base;
    local AwardAdd      = FlushInfo.Award.Add;
    local totalAward    = AwardTop + AwardBase + AwardAdd;
    local QuestId       = FlushInfo.Ploy.ID;
    local AlreadyNum    = FlushInfo.Ploy.Already;
        StarNumber      = subJectSelf.Star;
        ChosenAnswer    = subJectSelf.CurrIdx;
    local CommitInfo    = IntelliRaceMgr:getSubjectCommitInfo();
    CommitInfo.ID       = QuestId;
    CommitInfo.Idx      = AlreadyNum;
    CommitInfo.IsStar   = 0;
    
    if AwardTop > 0 or AwardBase > 0 or AwardAdd > 0  then
        IntelligenceRaceFrame_TimerBtn:Hide();
        IntelligenceRaceFrame_EndBtn:Show();
        IntelligenceRaceFrame_QuestionBtn:Hide();
        IntelligenceRaceFrame_EndBtnRankAward:SetText("��������������"..AwardTop.."����");
        IntelligenceRaceFrame_EndBtnExtraAward:SetText("�����������"..AwardAdd.."����");
        IntelligenceRaceFrame_EndBtnBaseAward:SetText("������������"..AwardBase.."����");
        IntelligenceRaceFrame_EndBtnTotalAwardNum:SetText(totalAward.."J");
        IntelligenceRaceFrameAnswerTimer:SetText("");
        ShowFinalAwardEntity();
        if IntelligenceRaceHintFrame:IsShown() then
            IntelligenceRaceHintFrame:Hide();
        end
        if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "�˳���������" then
            MessageBoxFrame:Hide();
        end
        local finalTop = subJectSelf.Top;
        local finalInteg = subJectSelf.Integ;
        SetIntelliRaceSelfResult( finalTop, finalInteg, true )
     
    --  IntelligenceRaceFrame_EndBtnSelfTop:SetText(subJectSelf.Top);
        RaceEndTime = getServerTime().sec;
        RaceEndFlag = true;
    else
        local stTime    = FlushInfo.Ploy.ReqEnd;
        local answerEnd = FlushInfo.Ploy.ResEnd;
        local readEnd   = FlushInfo.Ploy.LockEnd;
        local restQuest = FlushInfo.Ploy.Odd;
        UpdateQuestionInfo(QuestId,AlreadyNum);
        IsReadOrAnswer(readEnd,answerEnd,stTime);
        --�������ʱ��|����0��ʾҪ��ʾ��ʼ��������ʱ
        if stTime > 0 then
            IntelligenceRaceFrame_TimerBtn:Show();
            IntelligenceRaceFrame_EndBtn:Hide();
            IntelligenceRaceFrame_QuestionBtn:Hide();
        else
            IntelligenceRaceFrame_TimerBtn:Hide();
            IntelligenceRaceFrame_EndBtn:Hide();
            IntelligenceRaceFrame_QuestionBtn:Show();
        end
    end
    --���¸�����Ϣ
    local restQuest = FlushInfo.Ploy.Odd;
    UpdateSelfRaceInfo( subJectSelf ,restQuest );
    --�������а�
    local topRaceInfo = FlushInfo.Top;
    UpdateTopRaceInfo(topRaceInfo);
    if not IntelligenceRaceFrame:IsShown() then
        IntelligenceRaceFrame:Show();
    end
    local hintFrame = getglobal("IntelligenceRaceHintFrame");
    if not hintFrame:IsShown() and  not RaceEndFlag then
        hintFrame:Show();
        hintFrame:SetPoint( "center", "UIClient", "center", 0, -100 );
        IntelligenceRaceEnd()
    end
end
--���ö������ʱ�䵹��ʱ
function IsReadOrAnswer( readEnd,answerEnd,beginTime )
	local nCurtime 	= getServerTime().sec;
	local readTime	= readEnd - nCurtime;
	local replyTime	= answerEnd - nCurtime;
	local startTime	= beginTime - nCurtime;
	beginTimer 	= beginTime;
	readTimer	= readEnd;
	answerTimer	= answerEnd;
	local TimerFont	= getglobal("IntelligenceRaceFrameAnswerTimer");
	if beginTime > 0  and  beginTime > nCurtime then
		isReadFlag	= false;
		local raceTimerTxt = getglobal("IntelligenceRaceFrame_TimerBtnEndTimer");
		raceTimerTxt:SetText(startTime.."��")
		TimerFont:SetText("����ʹ���ʱ�䵹��ʱ");
		return;
	elseif readTimer > 0 then
		isReadFlag	= true;
		TimerFont:SetText("����ʱ�䵹��ʱ��"..readTime.."��");
		return;
	elseif answerEnd > nCurtime then
		isReadFlag	= false;
		TimerFont:SetText("����ʱ�䵹��ʱ��"..replyTime.."��");
		ChangeOptionTextColor();
		return;
	end
end
--����������Ϣ
function UpdateQuestionInfo( QuestId,AlreadyNum)
	local QuestInfo = getSubjectDef( QuestId );
	if QuestInfo == nil then return end
	trueAnswer	= QuestInfo.Succ;
	IntelligenceRaceFrame_QuestionBtnTitleFont:SetText(AlreadyNum.."."..QuestInfo.SubJect,255,255,190);
	IntelligenceRaceFrame_QuestionBtnTitleFont:SetClientString(AlreadyNum.."."..QuestInfo.SubJect);
	for i = 1 , MAX_OPTION_NUM do
		local optionText	= getglobal("IntelligenceRaceFrame_QuestionBtnOption"..i.."RichTxt");
		local optionChkBtn	= getglobal("IntelligenceRaceFrame_QuestionBtnChkBtn"..i);
		local parentBtn		= getglobal(optionText:GetParent());
		optionText:SetText(QuestInfo.SubJectKey[i-1].Str, 255, 255, 190);
		optionText:SetClientString( QuestInfo.SubJectKey[i-1].Str );
		optionChkBtn:setCheckState(false);
		optionChkBtn:Show();
		parentBtn:Show();
		if optionText:GetTextLines() == 1 then
			optionText:SetPoint("topleft", parentBtn:GetName(), "topleft",45,22);
		elseif optionText:GetTextLines() == 2 then
			optionText:SetPoint("topleft", parentBtn:GetName(), "topleft",45,14);
		elseif optionText:GetTextLines() >= 3 then
			optionText:SetPoint("topleft", parentBtn:GetName(), "topleft",45,6);
		else
			optionChkBtn:Hide();
			parentBtn:Hide();
		end
	end
end

function ChangeOptionTextColor()
	local titleTxt = IntelligenceRaceFrame_QuestionBtnTitleFont:GetClientString();
	IntelligenceRaceFrame_QuestionBtnTitleFont:SetText( titleTxt,255, 150, 0 )
	for i = 1 , MAX_OPTION_NUM do
		local optionText	= getglobal("IntelligenceRaceFrame_QuestionBtnOption"..i.."RichTxt");
		local szText		= optionText:GetClientString();
		optionText:SetText(szText, 255, 150, 0);
	end
	isReadFlag	= false;
	if ChosenAnswer == 5 then return end
	if ChosenAnswer ~= 0 then
		local correctBtn = getglobal("IntelligenceRaceFrame_QuestionBtnChkBtn"..ChosenAnswer);
		correctBtn:setCheckState(true);
		ShowIsTrueEntity();
		ChosenAnswer = 5;
	end
end
--���¸�����Ϣ
function UpdateSelfRaceInfo( subJectSelf ,restQuest )
	local	correctNum	= subJectSelf.Succ;
	local	selfTop		= subJectSelf.Top;
	local	selfPoint	= subJectSelf.Integ;

	IntelligenceRaceFrameSelfPointFont:SetText("���˵÷֣�"..selfPoint);
	IntelligenceRaceFrameCorrectAnsweredNum:SetText("���������"..correctNum);
	IntelligenceRaceFrameUnansweredNum:SetText("ʣ��������"..restQuest);
    
	SetIntelliRaceSelfResult( selfTop, selfPoint, false )
end
function UpdateTopRaceInfo(topRaceInfo)
	local topNum = topRaceInfo.Num;
	--��ʾ���а�
	for i=1 ,topNum do
		local rankNum		= topRaceInfo.Tops[i-1].Top;
		local rankPoint		= topRaceInfo.Tops[i-1].Integ;
		local playerName	= topRaceInfo.Tops[i-1].RoleName;
		local nNameLength 	= string.len( playerName );
		local topNumberFont = getglobal("IntelligenceRaceFrame_Top"..i.."TopNumberFont");
		local topNameFont 	= getglobal("IntelligenceRaceFrame_Top"..i.."TopNameFont");
		local topPointFont 	= getglobal("IntelligenceRaceFrame_Top"..i.."TopPointFont");
		topNumberFont:SetText(i);
		topPointFont:SetText( rankPoint );
		if nNameLength > 12 then
			if IsDBCSHead( playerName, 10) then 
				topNameFont:SetText( string.sub( playerName ,1 ,10 ).."..." );
			else
				topNameFont:SetText( string.sub( playerName ,1 ,11 ).."..." );
			end
		else
			topNameFont:SetText( playerName );
		end
	end
	if topNum < SUBJECT_MAX_TOP then
		for i = topNum + 1, SUBJECT_MAX_TOP do
			local topNumberFont = getglobal("IntelligenceRaceFrame_Top"..i.."TopNumberFont");
			local topNameFont 	= getglobal("IntelligenceRaceFrame_Top"..i.."TopNameFont");
			local topPointFont 	= getglobal("IntelligenceRaceFrame_Top"..i.."TopPointFont");
			topNumberFont:SetText("");
			topNameFont:SetText( "" );
			topPointFont:SetText( "" );
		end
	end
end
function IntelligenceRaceFrameQuestionOption_OnClick()
	if  isReadFlag then
		ShowMidTips("����׶��޷����д��⣡")
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	if ChosenAnswer ~= 0 then
		ShowMidTips("��ѡ��𰸣��޷�������ѡ��");
		this:setCheckState( not this:GetCheckState() );
		return;
	end
	ChosenAnswer = this:GetClientID();
	--��ʾ��ȷ������Ч
	local CommitInfo = IntelliRaceMgr:getSubjectCommitInfo();
	CommitInfo.KeyIdx = ChosenAnswer - 1;
	CommitInfo.IsStar = 0;
	IntelliRaceMgr:IntelliRaceAnswerCommit();
	ShowIsTrueEntity();
	ChosenAnswer = 5;
end
function IntelligenceRaceFrameOptionBtn_OnClick()
	local chkBtn = getglobal("IntelligenceRaceFrame_QuestionBtnChkBtn"..this:GetClientID() - 4);
	if isReadFlag then
		ShowMidTips("����׶��޷����д��⣡")
		return;
	end
	if ChosenAnswer ~= 0 then
		ShowMidTips("��ѡ��𰸣��޷�������ѡ��");
		return;
	end
	chkBtn:setCheckState(true);
	ChosenAnswer		= this:GetClientID() - 4;
	local CommitInfo	= IntelliRaceMgr:getSubjectCommitInfo();
	CommitInfo.KeyIdx	= ChosenAnswer - 1;
	CommitInfo.IsStar	= 0;
	IntelliRaceMgr:IntelliRaceAnswerCommit();
	ShowIsTrueEntity();
	ChosenAnswer = 5
end
function ShowIsTrueEntity()
	if ChosenAnswer == 5 then return end
	local modelview = getglobal("IntelligenceRaceFrame_QuestionBtnModelView");
	if ChosenAnswer == trueAnswer then
		modelview:setPetModel(0,0,"effect\\entity\\zhengque_16.ent");
		modelview:setEntityPosition(0,0,100,0);
	else
		modelview:setPetModel(0,0,"effect\\entity\\cuowu_16.ent");
		modelview:setEntityPosition(0,0,100,0);
	end
	modelview:Show();
end
function ShowFinalAwardEntity()
	local modelview = getglobal("IntelligenceRaceFrame_EndBtnModelView");
	modelview:setPetModel(0,0,"effect\\entity\\jiangli_16.ent");
	modelview:setEntityPosition(0,0,100,0);
	modelview:Show();
end
function InspireStarButtonOnClick()
	if RaceEndFlag then return end
	local CommitInfo = IntelliRaceMgr:getSubjectCommitInfo();
	if not isReadFlag then   
		ShowMidTips("�Ƕ���׶��޷�ʹ�����֮�ǣ�")
		return;
	end
	if CommitInfo.IsStar == 1 then
		ShowMidTips("�����Ѿ�ʹ�����֮�ǣ�")
		return;
	end

	this:Disable();
	GameTooltipOnLeave();
	local modelView = getglobal(this:GetName().."ModelView");
	modelView:setPetModel(0,0,"effect\\entity\\linggan_16.ent");
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();
	
	ChosenAnswer		= trueAnswer;
	CommitInfo.KeyIdx	= ChosenAnswer - 1;
	CommitInfo.IsStar	= 1;
	IntelliRaceMgr:IntelliRaceAnswerCommit();
end

function InspireStarButtonOnEnter()
	local szText = "ֻ���ڶ���ʱ����Ե��";
	SetGameTooltips(this:GetName(),szText)
end

function IntelligenceRaceFramesCloseBtn_OnClick()
	if RaceEndFlag then
		IntelligenceRaceFrame:Hide();
		IntelligenceRaceHintFrame:Hide();
		return;
	end
	MessageBox("��ʾ","��;�˳������������ý���,�Ƿ�ȷ���˳�");
	MessageBoxFrame:SetClientString( "�˳���������" );
end
function IntelligenceRaceQuit()
	IntelliRaceMgr:IntelliRaceAnswerQuit();
	IntelligenceRaceFrame:Hide();
	IntelligenceRaceHintFrame:Hide();
end
