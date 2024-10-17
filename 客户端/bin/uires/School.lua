-------------------------------------------ѧ����ȡ/��ʦ����-��ʦ�ĵ�---------------------------------------
local t_SchoolGainedInfo = {
	[1] = {
		Gained = "1.��ʦ���Խ�ÿ�յ��ĵ��������ѧ��\n2.ѧ�������ȡ��ʦ�ĵú���ȡ��ɫ����\n3.��ʦд���ĵ���Ҫע�뵼ʦ�Ľ�ɫ����\n4.��ʦע�뾭���ѧ����ȡ�ɹ���ɻ�ô��е���",
		Remind = "��Ҫ��ɫ���飺#c00ff00%s#n\t\t���Ի�ȡ���е�����#c00ff00%s#n\n��ÿ�������Ի�ȡ#c00ff00500#n�㴫�е���\n\n�Ƿ�ȷ�ϳ����ɫ���飿",
	},
	[2] = {
		Gained = "1.��ĵ�ʦͨ����ʦ�ĵý��Լ��ľ������͸��㡣\n��ÿ��һ�Σ�\n2.ͨ����ʦ�ĵÿ�����ȡ��ʦΪ�㴢��ľ��顣",
		Remind = "��ʦΪ�㴢���#c00ff00%s#n�ξ��飬���Ѿ���ȡ��#c00ff00%s#n��\n��ǰ���Ի�ȡ�ľ��飺#c00ff00%s#n\n\n�Ƿ�ȷ����ȡ��ɫ���飿",
	},
}
function ShoolGainedFrame_OnLoad()
end

function SchoolGainedFrame_OnShow()
	ShoolGainedFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
end

function OpenTeacherGained( nType, nRoleHeight, nRoleLow, szName )
	if t_SchoolGainedInfo[nType] == nil then return	end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getLv() < 60 then return end
	local schoolXinDeDef = GameSchool:GetSchoolXinDeDef(mainplayer:getLv())
	ShoolGainedFrameGained:SetText( t_SchoolGainedInfo[nType].Gained, 160, 160, 90 )
	ShoolGainedFrameRemind:setCenterLine( true )
	local remind = string.format( t_SchoolGainedInfo[nType].Remind, schoolXinDeDef.AddExp, schoolXinDeDef.SchoolVal	 )
	ShoolGainedFrameRemind:SetText( remind, 255, 255, 190 )
	ShoolGainedFrame:Show()
	ShoolGainedFrame:SetClientString( szName )
	ShoolGainedFrame:SetClientUserData( 0, nRoleHeight )
	ShoolGainedFrame:SetClientUserData( 1, nRoleLow )
end
function OpenStudentGained( nType, nRoleHeight, nRoleLow, szName )
	if t_SchoolGainedInfo[nType] == nil then return	end
	ShoolGainedFrameGained:SetText( t_SchoolGainedInfo[nType].Gained, 160, 160, 90 )
	ShoolGainedFrameRemind:setCenterLine( true )
	local remind = string.format( t_SchoolGainedInfo[nType].Remind, GameSchool:GetSelfTeacherGainedCount(),	GameSchool:GetSelfAwardTeacherGainedCount(), GameSchool:GetSelfCanAwardTeacherGainedExp() )
	ShoolGainedFrameRemind:SetText( remind, 255, 255, 190 )
	ShoolGainedFrame:Show()
	ShoolGainedFrame:SetClientString( szName )
	ShoolGainedFrame:SetClientUserData( 0, nRoleHeight )
	ShoolGainedFrame:SetClientUserData( 1, nRoleLow )
end
function ShoolGainedFrameOkBtn_OnClick()
	local name = ShoolGainedFrame:GetClientString()
	local nHeight = ShoolGainedFrame:GetClientUserData( 0 )
	local nLow = ShoolGainedFrame:GetClientUserData( 1 )
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER_TOLUA then
		GameSchool:SchoolXinDe( nHeight, nLow, name )
	else
		GameSchool:AwardSchoolXinDe( nHeight, nLow )
	end
	util.closeParent( this )
end
--------------------------------------------ѧ���Ǽ����--------------------------------------------
local MAX_SHOW_REG_STUDENT_NUM = 10
function OpenRegistStudentsFrame( npcId )
	local npc = ActorMgr:findActorByName( "������", GAT_NPC );
	if npc == nil then return end
	util.testNpcDialogDistance( npc:GetID(), RegistStudentsFrame )
	RegistStudentsFrame:Show()
end
function RegistStudentsFrame_OnShow()
	RegistStudentsFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	RegistStudentsFrameInfosItem1:Checked()
	SetRegStudentsInfo()
	local mainplayer = ActorMgr:getMainPlayer()
	RegistStudentsFrameChose:Enable()
	if mainplayer:getLv() < 60 then
		RegistStudentsFrameChose:Disable()
	end
end
function RegistStudentsFrame_OnHide()
	RegistStudentsFrame_RightOptionFrame:Hide()
end
function SetRegStudentsInfo()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		btn:Hide()
	end
	--����scrollbtn���ֵ
	if GameSchool:GetRegStudentsNum() > MAX_SHOW_REG_STUDENT_NUM then
		RegistStudentsFrameInfosScrollBar:Show()
		RegistStudentsFrameInfosScrollBar:SetMaxValue( GameSchool:GetRegStudentsNum() - MAX_SHOW_REG_STUDENT_NUM )
	else
		RegistStudentsFrameInfosScrollBar:Hide()
		RegistStudentsFrameInfosScrollBar:SetMaxValue(0);
	end
	RegistStudentsFrameInfosScrollBar:SetValue(math.min(RegistStudentsFrameInfosScrollBar:GetMaxValue(), RegistStudentsFrameInfosScrollBar:GetValue()))
	local endIndex = math.min( GameSchool:GetRegStudentsNum(), MAX_SHOW_REG_STUDENT_NUM + RegistStudentsFrameInfosScrollBar:GetValue() )

	for i = 1 + RegistStudentsFrameInfosScrollBar:GetValue(), endIndex do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..( i - RegistStudentsFrameInfosScrollBar:GetValue() ) )
		btn:Show()
		local NameFont = getglobal( btn:GetName().."Name" )
		NameFont:SetText( GameSchool:GetRegStudentName( i - 1 ) )
		local LevelFont = getglobal( btn:GetName().."Level" )
		LevelFont:SetText( GameSchool:GetRegStudentLevel( i - 1 ) )
		local RaceFont = getglobal( btn:GetName().."Race" )
		local race = GameSchool:GetRegStudentRace( i - 1 )
		RaceFont:SetText( GetCareerName( race ) )
		local ClanFont = getglobal( btn:GetName().."Clan" )
		ClanFont:SetText( GameSchool:GetRegStudentClanName( i - 1 ) )
		local MsgFont = getglobal( btn:GetName().."Msg" )
		MsgFont:SetText( GameSchool:GetRegStudentMsg( i - 1 ) )
	end
end

function RegistStudentInfoTemplate_OnClick()
	RegistStudentsFrame_RightOptionFrame:Hide()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		btn:DisChecked()
	end
	this:Checked()
	if arg1 == "RightButton" then
		RegistStudentsFrame_RightOptionFrame:Show()
		RegistStudentsFrame_RightOptionFrame:SetPoint( "topleft", "UIClient", "topleft", GetCursorPosX(), GetCursorPosY() )
	end
end

function RegistStudentsFrameChose_OnClick()
	RegistStudentsFrame_RightOptionFrame:Hide()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		if btn:IsChecked() and btn:IsShown() then
			local index = btn:GetClientID() + RegistStudentsFrameInfosScrollBar:GetValue()
			local szName = GameSchool:GetRegStudentName( index - 1 )
			local mainplayer = ActorMgr:getMainPlayer() 
			if mainplayer:getName() == szName then
				ShowMidTips( "���ܺ��Լ��Ի�" )
				return
			end
			OpenSchoolChatFrame( szName )
			return
		end
	end
	ShowMidTips( "����ѡ��Ǽ�����ٽ����˽�" )
end

function RegistStudentsFrame_ScrollUpBtn()
	RegistStudentsFrame_RightOptionFrame:Hide()
	local slider = RegistStudentsFrameInfosScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
	SetRegStudentsInfo()
end
function RegistStudentsFrame_ScrollDownBtn()
	RegistStudentsFrame_RightOptionFrame:Hide()
	local slider	= RegistStudentsFrameInfosScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
	SetRegStudentsInfo()
end
function RegistStudentsFrame_ScrollBarValueChange()
	RegistStudentsFrame_RightOptionFrame:Hide()
	SetRegStudentsInfo()
end
---------------------------------------------�Ǽǰ��Ҽ��˵�----------------------------------------
---Ŀǰֻ���ĸ������ʱ��xml��ҲҪ���
local t_schooolRightOperation = {			
		[1] = { text = "˽��", func = function () 
						RegistStudentsFrame_PrivateBtn_OnClick(); 
					end },	
		[2] = { text = "���", func = function () 
						RegistStudentsFrame_TeamAddBtn_OnClick(); 
					end },	
		[3] = { text = "�۲����", func = function () 
						RegistStudentsFrame_WatchBtn_OnClick(); 
					end },	
		[4] = { text = "��������", func = function () 
						RegistStudentsFrame_CopyBtn_OnClick(); 
					end },
}
function RegistStudentsFrame_PrivateBtn_OnClick()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		if btn:IsChecked() and btn:IsShown() then
			local NameFont = getglobal( btn:GetName().."Name" )
			local szName = NameFont:GetText()
			local t_ChatInsertFrameControl = GetChatInsertFrameControl();
			t_ChatInsertFrameControl:setPrivatePlayerName( { ["name"] = szName } );
			SetChatType( CL_CHAT_PRIVATE );
			UpdateChatInsertFrameEnterFocus();
			SetFocusFrame("CSChatInsert");
		end
	end
end

function RegistStudentsFrame_TeamAddBtn_OnClick()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		if btn:IsChecked() and btn:IsShown() then
			local NameFont = getglobal( btn:GetName().."Name" )
			local szName = NameFont:GetText()
			TeamManager:Team_OP_Add( szName );
		end
	end
end

function RegistStudentsFrame_WatchBtn_OnClick()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		if btn:IsChecked() and btn:IsShown() then
			local NameFont = getglobal( btn:GetName().."Name" )
			local szName = NameFont:GetText()
			GameFriendManager:RequestWatchFriend( szName )
		end
	end
end
--��������
function RegistStudentsFrame_CopyBtn_OnClick()
	for i = 1, MAX_SHOW_REG_STUDENT_NUM do
		local btn = getglobal( "RegistStudentsFrameInfosItem"..i )
		if btn:IsChecked() and btn:IsShown() then
			local NameFont = getglobal( btn:GetName().."Name" )
			local szName = NameFont:GetText()
			CopyToMemory( szName );
		end
	end
end

function RegistStudentsFrame_RightBtn_OnClick()
	local index = this:GetClientID()
	t_schooolRightOperation[index].func();
	local frame = getglobal( this:GetParent() );
	frame:Hide();
end

function RegistStudentsFrame_RightOptionFrame_OnShow()
	for i = 1, table.getn( t_schooolRightOperation ) do
		local btnFont = getglobal( "RegistStudentsFrame_RightBtn"..i.."Font" )
		btnFont:SetText( t_schooolRightOperation[i].text )
	end
end
--------------------------------------------ѧ���������--------------------------------------------
function OpenStudentRegistMsg( npcId )
	StudentRegistMsg:Show()
	util.testNpcDialogDistance( npcId, StudentRegistMsg );
end
function StudentRegistMsgOkBtn_OnClick()
	local szMsg = StudentRegistMsgEdit:GetText()
	GameSchool:RegSchool( szMsg )
end
function StudentRegistMsgCancleBtn_OnClick()
	util.closeParent( this )
end

function StudentRegistMsgResult( nFlag )
	if nFlag == 0 then
		ShowMidTips( "���ѳɹ��Ǽ�" )
		StudentRegistMsg:Hide()		
	else
		ShowMidTips( "�Ǽ�ʧ��" )
	end
end
function StudentRegistMsg_OnShow()
	StudentRegistMsg:SetPoint( "center", "UIClient", "center", 0,0 )
	StudentRegistMsgEdit:SetText( "ϣ���ҵ�һ�����Һõĵ�ʦ" )
	SetFocusFrame( "StudentRegistMsgEdit" );
	StudentRegistMsgEdit:SelectAllText()
end
---------------------------------------------��ʦ��ͽ����-------------------------------------------
function OpenSchoolInviteFrame()
	local npc = ActorMgr:findActorByName( "������", GAT_NPC );
	if npc == nil then return end
	util.testNpcDialogDistance( npc:GetID(), SchoolInviteFrame )
	SchoolInviteFrame:Show()
end
function SchoolInviteFrameOkBtn_OnClick()
	local szName = SchoolInviteFrameEdit:GetText()
	if szName == "" then
		ShowMidTips( "������Ҫ��ѧ��������" )
		return 
	end
	GameSchool:InviteToSchool( szName )
	util.closeParent( this )
end
function SchoolInviteFrame_OnShow()
	SchoolInviteFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	SchoolInviteFrameEdit:SetText( "" )
	SchoolInviteFrameInfo:SetText( "\t��׼������ѧ��������ȷ��Ҫ�յ�ѧ����������",255,255,190 )	
end
---------------------------------------------��ҽ�����ͽ����---------------------------------------
function SchoolJoinFrame_OnShow()
	SchoolJoinFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
end
function OpenSchoolJoinFrame( szName )
	local name = szName
	SchoolJoinFrameInfo:setCenterLine( true )
	SchoolJoinFrameInfo:SetText( string.format( "%sϣ������Ϊ����ѧ��", name ), 255,255,190 )
	SchoolJoinFrame:SetClientString( szName )
	SchoolJoinFrame:Show()
end
function SchoolJoinFrameAcceptBtn_OnClick()
	GameSchool:AcceptToSchool( SchoolJoinFrame:GetClientString() )
	util.closeParent( this )	
end
function SchoolJoinFrameRefusBtn_OnClick()
	GameSchool:RejectToSchool( 0, SchoolJoinFrame:GetClientString() )
	util.closeParent( this )
end
