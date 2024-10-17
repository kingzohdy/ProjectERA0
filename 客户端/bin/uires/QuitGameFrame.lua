local begin_timer	= 0;

function NLS_QuitGameScrollUpBtn_OnClick()
	local slider = getglobal("NLS_QuitGameScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function NLS_QuitGameScrollDownBtn_OnClick()
	local slider	= getglobal("NLS_QuitGameScrollBar");
	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function NLS_QuitGameScrollBar_OnValueChanged()

end

function NLS_QuitGameScrollBar_OnClick()

end

function AcquireTianYuanButton_OnClick()

end

function AcquireTianYuanButton_OnEnter()

end

function AcquireTianYuanButton_OnLeave()

end

function AgreeQuitGameButton_OnClick()
	QuitGameFrame:Hide();
	ViewLockFrame:Show();
	ReallyQuitGameFrame:Show();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil and not MainPlayer:getEquip():isTmpPatronid() then
		MainPlayer:breakPreAction();
	end
end

function AgreeQuitGameButton_OnEnter()

end

function AgreeQuitGameButton_OnLeave()

end

function DisagreeQuitGameButton_OnClick()
	QuitGameFrame:Hide();
end

function DisagreeQuitGameButton_OnEnter()

end

function DisagreeQuitGameButton_OnLeave()

end

function SmallWarningButtonOnClick()

end

function SmallWarningButtonOnEnter()

end

function SmallWarningButtonOnLeave()

end

function HelpInformationButtonOnClick()

end

function HelpInformationButtonOnEnter()

end

function HelpInformationButtonOnLeave()

end

function CloseWindowButtonOnClick()
	QuitGameFrame:Hide();
end

function CloseWindowButtonOnEnter()

end

function CloseWindowButtonOnLeave()

end

function QuitGameFrame_OnLoad()
	NLS_QuitGameBtnHeadFont:SetText("");--" "�˳���Ϸ" );
	TimeStatistics:SetText("");--" "�㱾�ε�¼��������" );
	HourStatistics:SetText("");--" "13" );
	MinuteStatistics:SetText("");--" "56" );
	TianYuanStatisticsValue:SetText("");--" "6530" );
	HourString:SetText("");--" "Сʱ" );
	MinuteString:SetText("");--" "����," );
	TianYuanStatistics:SetText("");--" "����ð���Ԫ" );
	TianYuanStatisticString:SetText("");--"��." );
	TaskInfoFont1:SetText("");-- "�����δ��ɵ��ճ�����£�" );
end

function NLS_QuitGameInfoShowBtn_OnLoad()
	NLS_ReallyQuitGameBtnHeadFont:SetText( "����㽫�˳���Ϸ" );
	NLS_ReallyQuitGameBtnHeadFontValue:SetText( "9" );
	this:RegisterEvent( "GE_BATTLE_TO_WORLD" );
end

function NLS_QuitGameInfoShowBtn_OnEvent()
	if arg1 == "GE_BATTLE_TO_WORLD" then
		-- ����˳�
		if ShowChangeRoleFrame:IsShown() then
			ShowChangeRoleFrame:Hide()
			ViewLockFrame:Hide()
		end
		if ReallyQuitGameFrame:IsShown() then
			ReallyQuitGameFrame:Hide()
			ViewLockFrame:Hide()
		end
	end
end

function ReallyQuitGameCancelButtonOnClick()
	ReallyQuitGameFrame:Hide();
	NLS_ReallyQuitGameBtnHeadFontValue:SetText( "9" );
end

function ReallyQuitGameCancelButtonOnEnter()

end

function ReallyQuitGameCancelButtonOnLeave()

end

function NLS_ChangeRoleInfoShowBtn_OnLoad()
	NLS_ChangeRoleBtnHeadFont:SetText( "����㽫���ؽ�ɫѡ�����" );
	NLS_ChangeRoleBtnHeadFontValue:SetText( "9" );
end

function ChangeRoleCancelButtonOnClick()
	ShowChangeRoleFrame:Hide();
	--print( "ChangeRoleCancelButtonOnClick--148" );
	ViewLockFrame:Hide();
	LoginMgr:requestCancelLogout();
	NLS_ChangeRoleBtnHeadFontValue:SetText( "9" );
end

function NLS_ChangeRoleInfoShowBtn_OnUpdate()
	if math.ceil( os.clock() - begin_timer) >= 1 then
		local nRemain = 10 - math.ceil( os.clock() - begin_timer);
		if nRemain < 0 then
			nRemain = 0;
		end
		NLS_ChangeRoleBtnHeadFontValue:SetText( nRemain );
	end
end

function NLS_ChangeRoleInfoShowBtn_OnShow()
	begin_timer = os.clock();
end

-- �˳���Ϸ����ʱ���
function NLS_QuitGameInfoShowBtn_OnUpdate()
	if math.ceil( os.clock() - begin_timer) >= 1 then
		NLS_ReallyQuitGameBtnHeadFontValue:SetText( 10 - math.ceil( os.clock() - begin_timer ) );
		if math.ceil( os.clock() - begin_timer ) >= 10 then
			ReallyQuitGameFrame:Hide();
			GameMgr:QuitGame();
		end
	end
end

function NLS_QuitGameInfoShowBtn_OnShow()
	begin_timer = os.clock();
end

function NLS_QuitGameInfoShowBtn_OnHide()
	local t_hideIcons = { "ViewLockFrame", };
	util.HideIcons( t_hideIcons );
end

function NLS_ChangeRoleInfoShowBtn_OnHide()

end

-- �����˳�
function QuickllyQuitGameButtonOnClick()
	GameMgr:QuitGame();
	ReallyQuitGameFrame:Hide();
end

function ViewLockFrame_OnLoad()
	local tex = getglobal( "ViewLockFrameLock" );
	tex:SetColor( 0, 0, 0 );
	tex:SetBlendAlpha( 0.6 );
end

function ViewLockFrame_OnUpdate()
	-- ����ָ����ʾTIPS����
	HideGuideEffectTip();
end

function ViewLockFrame_OnShow()
	if PetDialogFrame:IsShown() then
		PetDialogFrame:Hide();
	end
end

function StartMudFrame_OnLoad()
	StartMudFrameShowBtnHeadFont:SetText( "����㽫���������ռ�" );
	StartMudFrameShowBtnHeadFontValue:SetText( "9" );
end

function StartMudFrame_OnUpdate()
	if math.ceil( os.clock() - begin_timer) >= 1 then
		local nRemain = 10 - math.ceil( os.clock() - begin_timer);
		if nRemain <= 0 then
			StartMudFrame:Hide();
			ViewLockFrame:Hide();
			LoginMgr:startMud();			
		end
		StartMudFrameShowBtnHeadFontValue:SetText( nRemain );
	end
end

function StartMudFrame_OnShow()
	begin_timer = os.clock();
	SaveLastSelectPetInfo();
	-- ���ѡ�񷵻ؽ�ɫѡ�����ʱ��ֹͣ�ƶ�״̬��ͬʱ����������������� 
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil and not MainPlayer:getEquip():isTmpPatronid() then
		MainPlayer:breakPreAction();
		MainPlayer:breakBTypePre();
	end
	SkillMgr:onStop();
	PreLogOutAction();
	LoginMgr:requestStartMud();
	SystemSelectFrame:Hide();
	ViewLockFrame:Show();
	ViewLockFrame:Show();
	MessageBoxFrame:Hide();

end
