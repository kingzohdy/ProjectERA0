-------------------------------------------学生领取/导师存入-导师心得---------------------------------------
local t_SchoolGainedInfo = {
	[1] = {
		Gained = "1.导师可以将每日的心得体会赠给学生\n2.学生点击获取导师心得后会获取角色经验\n3.导师写下心得需要注入导师的角色经验\n4.导师注入经验后，学生领取成功后可获得传承点数",
		Remind = "需要角色经验：#c00ff00%s#n\t\t可以获取传承点数：#c00ff00%s#n\n你每天最多可以获取#c00ff00500#n点传承点数\n\n是否确认充入角色经验？",
	},
	[2] = {
		Gained = "1.你的导师通过导师心得将自己的经验赠送给你。\n（每日一次）\n2.通过导师心得可以领取导师为你储存的经验。",
		Remind = "导师为你储存过#c00ff00%s#n次经验，你已经领取过#c00ff00%s#n次\n当前可以获取的经验：#c00ff00%s#n\n\n是否确认领取角色经验？",
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
--------------------------------------------学生登记面板--------------------------------------------
local MAX_SHOW_REG_STUDENT_NUM = 10
function OpenRegistStudentsFrame( npcId )
	local npc = ActorMgr:findActorByName( "艾吉奥", GAT_NPC );
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
	--设置scrollbtn最大值
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
				ShowMidTips( "不能和自己对话" )
				return
			end
			OpenSchoolChatFrame( szName )
			return
		end
	end
	ShowMidTips( "请先选择登记玩家再进行了解" )
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
---------------------------------------------登记板右键菜单----------------------------------------
---目前只有四个，添加时在xml里也要添加
local t_schooolRightOperation = {			
		[1] = { text = "私聊", func = function () 
						RegistStudentsFrame_PrivateBtn_OnClick(); 
					end },	
		[2] = { text = "组队", func = function () 
						RegistStudentsFrame_TeamAddBtn_OnClick(); 
					end },	
		[3] = { text = "观察玩家", func = function () 
						RegistStudentsFrame_WatchBtn_OnClick(); 
					end },	
		[4] = { text = "复制名称", func = function () 
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
--复制名字
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
--------------------------------------------学生留言面板--------------------------------------------
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
		ShowMidTips( "您已成功登记" )
		StudentRegistMsg:Hide()		
	else
		ShowMidTips( "登记失败" )
	end
end
function StudentRegistMsg_OnShow()
	StudentRegistMsg:SetPoint( "center", "UIClient", "center", 0,0 )
	StudentRegistMsgEdit:SetText( "希望找到一个对我好的导师" )
	SetFocusFrame( "StudentRegistMsgEdit" );
	StudentRegistMsgEdit:SelectAllText()
end
---------------------------------------------导师收徒界面-------------------------------------------
function OpenSchoolInviteFrame()
	local npc = ActorMgr:findActorByName( "艾吉奥", GAT_NPC );
	if npc == nil then return end
	util.testNpcDialogDistance( npc:GetID(), SchoolInviteFrame )
	SchoolInviteFrame:Show()
end
function SchoolInviteFrameOkBtn_OnClick()
	local szName = SchoolInviteFrameEdit:GetText()
	if szName == "" then
		ShowMidTips( "请输入要收学生的名字" )
		return 
	end
	GameSchool:InviteToSchool( szName )
	util.closeParent( this )
end
function SchoolInviteFrame_OnShow()
	SchoolInviteFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
	SchoolInviteFrameEdit:SetText( "" )
	SchoolInviteFrameInfo:SetText( "\t你准备好收学生了吗？请确保要收的学生在你的身边",255,255,190 )	
end
---------------------------------------------玩家接受收徒邀请---------------------------------------
function SchoolJoinFrame_OnShow()
	SchoolJoinFrame:SetPoint( "center", "UIClient", "center", 0, 0 )
end
function OpenSchoolJoinFrame( szName )
	local name = szName
	SchoolJoinFrameInfo:setCenterLine( true )
	SchoolJoinFrameInfo:SetText( string.format( "%s希望收您为他的学生", name ), 255,255,190 )
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
