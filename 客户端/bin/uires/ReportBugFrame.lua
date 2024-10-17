local HAVE_ADVISE	= 0;
local REPORT_BUG	= 1;
local JU_BAO		= 2;

local t_reportDesc = 
{
[HAVE_ADVISE]	= { desc = "    请详细填写您的问题和意见，我们将在审核后根据意见的价值通过游戏内邮箱给予不同程度的奖励，同时感谢您对我们游戏的支持。", 
					btn = "ReportBugFrame_HaveAdviseBtn", title = "我有建议" , tips = "您的意见已成功提交，非常感谢您对零纪元的支持，祝您游戏愉快。"
				},
[REPORT_BUG]	= { desc = "    请尽量详细描述您遇到的问题，完整填写相关的角色名字、地点、物品名字，以及时间段，信息越详细越好。",
					btn = "ReportBugFrame_ReportBugBtn", title = "BUG提交" , tips = "您的反馈的BUG已提交，非常感谢您对零纪元的支持，祝您游戏愉快。"
				},
[JU_BAO]		= { desc = "    请尽量详细描述您要举报的情况，完整填写相关的角色名字、地点、物品名字，以及时间段，信息越详细越好。",
					btn = "ReportBugFrame_JuBaoBtn", title = "我要举报" , tips = "感谢您的热心举报，已经提交技术部门进行核查，非常感谢您对零纪元的支持，祝您游戏愉快。"
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

-- 我有建议
function ReportBugFrame_HaveAdviseBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "请先关闭确认面板" );
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

-- BUG提交
function ReportBugFrame_ReportBugBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "请先关闭确认面板" );
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

-- 我要举报
function ReportBugFrame_JuBaoBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "请先关闭确认面板" );
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
		ShowMidTips( "发送内容不能为空" );
		return;
	end

	local nCurServeTime			= getServeSecondTime();
	local nLastCommitMailTime	= GameUserConfig:getLastCommitMailTime().m_uLastSendMailTime;
	if not GameWizard:isDo(GAMEWIZARD_FIRST_COMMIT_MAIL) then
		GameWizard:doStep(GAMEWIZARD_FIRST_COMMIT_MAIL);
	elseif nCurServeTime > nLastCommitMailTime and ( nCurServeTime - nLastCommitMailTime < t_ReportBugFrameControl["TWO_COMMIT_MIN_INTERVAL"] ) then
		ShowMidTips( "不能频繁提交意见，至少间隔10分钟" );
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	szText = szText .."\n".."用户名：".. LoginMgr:getUserName() .."\t角色名："..mainplayer:getName();
	local nType = t_ReportBugFrameControl["Type"];
	GameMgr:sendEMail( t_reportDesc[nType]["title"], szText,nType );
	GameUserConfig:setLastCommitMailTime( nCurServeTime );
	GameUserConfig:saveLastCommitMailTime();
	--show mid tips
	MessageBoxOK("提示",t_reportDesc[nType]["tips"]);
end

function ReportBugFrame_CommitBtn_OnClick()
	if t_ReportBugFrameControl["isLock"] then
		ShowMidTips( "请先关闭确认面板" );
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