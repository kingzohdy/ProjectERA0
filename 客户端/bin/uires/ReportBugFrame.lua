local HAVE_ADVISE	= 0;
local REPORT_BUG	= 1;
local JU_BAO		= 2;

local t_reportDesc = 
{
[HAVE_ADVISE]	= { desc = "    ����ϸ��д�����������������ǽ�����˺��������ļ�ֵͨ����Ϸ��������費ͬ�̶ȵĽ�����ͬʱ��л����������Ϸ��֧�֡�", 
					btn = "ReportBugFrame_HaveAdviseBtn", title = "���н���" , tips = "��������ѳɹ��ύ���ǳ���л�������Ԫ��֧�֣�ף����Ϸ��졣"
				},
[REPORT_BUG]	= { desc = "    �뾡����ϸ���������������⣬������д��صĽ�ɫ���֡��ص㡢��Ʒ���֣��Լ�ʱ��Σ���ϢԽ��ϸԽ�á�",
					btn = "ReportBugFrame_ReportBugBtn", title = "BUG�ύ" , tips = "���ķ�����BUG���ύ���ǳ���л�������Ԫ��֧�֣�ף����Ϸ��졣"
				},
[JU_BAO]		= { desc = "    �뾡����ϸ������Ҫ�ٱ��������������д��صĽ�ɫ���֡��ص㡢��Ʒ���֣��Լ�ʱ��Σ���ϢԽ��ϸԽ�á�",
					btn = "ReportBugFrame_JuBaoBtn", title = "��Ҫ�ٱ�" , tips = "��л�������ľٱ����Ѿ��ύ�������Ž��к˲飬�ǳ���л�������Ԫ��֧�֣�ף����Ϸ��졣"
				},
};
-- TODO
local t_ReportBugFrameControl = { Type = HAVE_ADVISE, TWO_COMMIT_MIN_INTERVAL = 10 * 60, isLock = false, changeToType = -1 };

t_ReportBugFrameControl["lock"] = 
function ( self )
	self["isLock"] = true;
end

t_ReportBugFrameControl["unlock"] = 
function ( self )
	self["isLock"] = false;
end

t_ReportBugFrameControl["setReportType"] = 
function ( self, data )
	self["Type"] = data["Type"];
	local t_riches = 
	{ ["ReportBugFrameDescRichText"] = 
		{ 
		value = t_reportDesc[data["Type"]]["desc"], 
		color = { r = 255, g = 254, b = 189 },
		}, 
	};

	util.SetRiches( t_riches );
	for _, data in pairs( t_reportDesc ) do
		local btn = getglobal( data["btn"] );
		btn:DisChecked();
	end

	local btn = getglobal( t_reportDesc[data["Type"]]["btn"] );
	btn:Checked();
	local edit = getglobal( "ReportBugFrameInpuMultiEditBox" );	
	edit:Clear();
	SetFocusFrame("ReportBugFrameInpuMultiEditBox");
end

function ReportBugFrame_OnShow()
	t_ReportBugFrameControl:setReportType( { Type = HAVE_ADVISE } );
end

function ReportBugConfirmFrame_OnLoad()
	local mul = getglobal("ReportBugFrameInpuMultiEditBox");
	mul:setMaxWordCount( 500 );
end

-- ���н���
function ReportBugFrame_HaveAdviseBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "���ȹر�ȷ�����" );
		return;
	end

	if this:IsChecked() then
		return;
	end

	local edit		= getglobal( "ReportBugFrameInpuMultiEditBox" );
	local szText	= edit:GetText();
	if szText ~= "" then
		t_ReportBugFrameControl["changeToType"] = HAVE_ADVISE;
		ReportBugConfirmFrame:Show();
		return;
	end

	t_ReportBugFrameControl:setReportType( { Type = HAVE_ADVISE } );
end

-- BUG�ύ
function ReportBugFrame_ReportBugBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "���ȹر�ȷ�����" );
		return;
	end

	if this:IsChecked() then
		return;
	end

	local edit		= getglobal( "ReportBugFrameInpuMultiEditBox" );
	local szText	= edit:GetText();
	if szText ~= "" then
		t_ReportBugFrameControl["changeToType"] = REPORT_BUG;
		ReportBugConfirmFrame:Show();
		return;
	end

	t_ReportBugFrameControl:setReportType( { Type = REPORT_BUG } );
end

-- ��Ҫ�ٱ�
function ReportBugFrame_JuBaoBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "���ȹر�ȷ�����" );
		return;
	end

	if this:IsChecked() then
		return;
	end

	local edit		= getglobal( "ReportBugFrameInpuMultiEditBox" );
	local szText	= edit:GetText();
	if szText ~= "" then
		t_ReportBugFrameControl["changeToType"] = JU_BAO;
		ReportBugConfirmFrame:Show();
		return;
	end

	t_ReportBugFrameControl:setReportType( { Type = JU_BAO } );
end

function CommitMailContent()
	local edit		= getglobal( "ReportBugFrameInpuMultiEditBox" );
	local szText	= edit:GetText();
	if szText == "" then
		ShowMidTips( "�������ݲ���Ϊ��" );
		return;
	end

	local nCurServeTime			= getServeSecondTime();
	local nLastCommitMailTime	= GameUserConfig:getLastCommitMailTime().m_uLastSendMailTime;
	if not GameWizard:isDo(GAMEWIZARD_FIRST_COMMIT_MAIL) then
		GameWizard:doStep(GAMEWIZARD_FIRST_COMMIT_MAIL);
	elseif nCurServeTime > nLastCommitMailTime and ( nCurServeTime - nLastCommitMailTime < t_ReportBugFrameControl["TWO_COMMIT_MIN_INTERVAL"] ) then
		ShowMidTips( "����Ƶ���ύ��������ټ��10����" );
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	szText = szText .."\n".."�û�����".. LoginMgr:getUserName() .."\t��ɫ����"..mainplayer:getName();
	local nType = t_ReportBugFrameControl["Type"];
	GameMgr:sendEMail( t_reportDesc[nType]["title"], szText,nType );
	GameUserConfig:setLastCommitMailTime( nCurServeTime );
	GameUserConfig:saveLastCommitMailTime();
	--show mid tips
	MessageBoxOK("��ʾ",t_reportDesc[nType]["tips"]);
end

function ReportBugFrame_CommitBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "���ȹر�ȷ�����" );
		return;
	end

	CommitMailContent();
end

function ReportBugConfirmFrame_OnShow()
	t_ReportBugFrameControl:lock();
end

function ReportBugConfirmFrame_OnHide()
	t_ReportBugFrameControl:setReportType( { Type = t_ReportBugFrameControl["changeToType"] } );
	t_ReportBugFrameControl:unlock();
end

function ReportBugConfirmFrame_ConfirmBtn_OnClick()
	CommitMailContent();
	ReportBugConfirmFrame:Hide();
end

function ReportBugFrameInpuMultiEditBox_OnChar()
	local edit = getglobal("ReportBugFrameInpuMultiEditBox");
	local nCount = edit:getTextCount();
	ReportBugFrame_InputEditNumLimitHeadBtnFont:SetText(nCount.."/500")
end