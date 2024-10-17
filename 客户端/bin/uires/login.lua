--登录结果
 g_nLOGIN_SUCC      = 0; 	 --/* account or role login succ */
 g_nLOGIN_REP       = 1;	 --/* account or role login fail because repeat */
 g_nLOGIN_SVRBUSY   = 2; 	 --/* account or role login fail because server busy */
 g_nLOGIN_BADCLT    = 3;	 --/* account login fail because client protolcol version not compatible */
 g_nLOGIN_BAN       = 4; 	 --/* account or role login fail because system ban the account or role */
 g_nLOGIN_FREQUENCY = 5;	 --/* account login fail because frequency limit */
 g_nLOGIN_NOSERVICE = 6;	 --/* server not available */
 g_nLOGIN_AGAIN_SAMEIP = 7;	 --/* repeat login  */
 g_nLOGIN_QUEUE_FULL = 8;	 --/* queue full */
 g_nLOGIN_PRECREATEROLE = 9; --/* preCreateRole */
 g_nLOGIN_ERR       = 16; 	 --/* account or role login fail because other reason */

local nLastRequestTime = 0;
local bQuickStart = false;

function age_detect(szCert)
	local current_time = getServerTime().sec;
	local birth_time = string2time( string.len(szCert) > 15 and (string.sub(szCert, 7,14) .. "000000") or ("19" .. string.sub(szCert, 7, 12) .. "000000"));
	local inter_year = math.floor((current_time - birth_time)/31536000);
	if inter_year < 18 then
		return false;
	end
	return true;
end

function LoginFrame_OnLoginBtnClick()
	initWaitQueueControl();
	local name = LoginNameEdit:GetText();
	ClearFocus();
	if SmallKeyboardFrame:IsShown() then
		SmallKeyboardFrame:Hide();
	end
	if ( name == "" ) then
		Login_MessageBoxOK( { ["text"] = "账号不能为空", ["opDesc"] = "" } );
	elseif not LoginAcceptUserProtocolChkBtn:GetCheckState() then
		Login_MessageBoxOK( { ["text"] = "请先同意用户协议", ["opDesc"] = "" } );
	else
		if os.clock() - nLastRequestTime < 6 then
			--LoginSelectServerBtnFont:SetText("请勿频繁登录");
			return;
		end
		
		LoginSelectServerBtnFont:SetText("正在登录中……");

		nLastRequestTime = os.clock();
		name = string.lower( name );
		LoginNameEdit:SetText( name );
		bQuickStart = false;
		LoginMgr:requestLoginGameServer( SelectWorldInfo.bigWorldIndex - 1, SelectWorldInfo.worldIndex - 1, LoginNameEdit:GetText(), PassWordEdit:GetPassWord() );
		--MessageBoxOK("report",os.date("%x",getServerTime().sec));
		ClearOtherWorldRoleData()
		PassWordEdit:Clear();
	end
end

function LoginAcceptDescRich_OnClick()
	if ( arg3 ~= "LeftButton" ) then
		return;
	end

	if not ReadUserProtocolFrame:IsShown() then
		ReadUserProtocolFrame:Show();
	end
end

function LoginExitBtn_OnClick()
	LoginFrame:Hide();
	LoginBackFrame:Hide();
	GameMgr:QuitGame();
end

function LoginFrame_OnLoad()
	this:RegisterEvent("GE_UPDATE_ROLELIST");
	this:RegisterEvent("GE_UPDATE_BACKINIT");
	this:RegisterEvent("GE_UPDATE_BACKDIRLIST");
	this:RegisterEvent("GE_UPDATE_LEAVEGAME");
	this:RegisterEvent("GE_UPDATE_LOGINRESULT");
	this:RegisterEvent("GE_UPDATE_GETROLELIST");
	local bCheck = LoginMgr:getSaveUserCheck();
	if bCheck == 1 then
		LoginSaveUserNameChkBtn:setCheckState( true );
	elseif bCheck == 0 then
		LoginSaveUserNameChkBtn:setCheckState( false );
	end
	LoginQuickStartBtn:Disable()
end

function LoginFrame_OnEvent()
	local nLoginResult =  LoginMgr:getLoginResult();
	LoginSelectServerBtnFont:SetText("当前服务器："..SelectWorldInfo.world );

	nLastRequestTime = 0;
	if arg1 == "GE_UPDATE_LOGINRESULT" then
		if nLoginResult == g_nLOGIN_REP then
			Login_MessageBoxOK( { ["text"] = "该账号已在别处登录,请重试！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_SVRBUSY then
			Login_MessageBoxOK( { ["text"] = "服务器繁忙，请稍后登录！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_BADCLT then
			Login_MessageBoxOK( { ["text"] = "客户端版本不匹配，请更新后登录！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_BAN then
			Login_MessageBoxOK( { ["text"] = "该账号被禁止登录，请联系客服人员！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_FREQUENCY then
			Login_MessageBoxOK( { ["text"] = "该账号频繁登录！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_NOSERVICE then
			Login_MessageBoxOK( { ["text"] = "服务器未开放！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_AGAIN_SAMEIP then
			Login_MessageBoxOK( { ["text"] = "上次登录账号正在退出，请重试!(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_QUEUE_FULL then
			Login_MessageBoxOK( { ["text"] = "服务器满员,请稍后再试！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_ERR then
			Login_MessageBoxOK( { ["text"] = "无法连接游戏服务器！(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_PRECREATEROLE then
			Login_MessageBoxOK( { ["text"] = "预创建角色状态，正式开服请关注#Lwww.ljy0.com@@http://www.ljy0.com#n", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		end
		PasswdCardFrame:Hide()
	end

	if arg1 == "GE_UPDATE_ROLELIST" then
		if LoginFrame:IsShown() and not bQuickStart then
			if LoginSaveUserNameChkBtn:GetCheckState() then
				LoginMgr:saveLastUserName();
			else
				LoginMgr:clearLastUserName();
			end
		end
		PasswdCardFrame:Hide()
		LoginFrame:Hide();
		DirServerFrame:Hide();
		SetRegisterState( false );
	end

	if arg1 == "GE_UPDATE_GETROLELIST" then
		--SelectFrame:Hide();
		if not CharacterSelectFrame:IsShown() then
			CharacterSelectFrame:Show();
		end
	end

	if arg1 == "GE_UPDATE_LEAVEGAME" then
		LoginFrame:Hide();
		LoginBackFrame:Hide();
		GameMgr:QuitGame();
	end
	if arg1 == "GE_UPDATE_BACKDIRLIST" then
		LoginFrame:Show();
		LoginBackFrame:Show();
		CharacterSelectFrame:Hide();
	end
end

function LoginFrame_OnShow()
	if getGameState() == STATE_INWORLD then
		GameMgr:returnLogin();
		LoginMgr:breakNetConnect();
	end
	local t_hideIcons = { "PetUI", };
	util.HideIcons( t_hideIcons );

	if CSFashionFrame:IsShown() then
		CSFashionFrame:Hide();
	end
	local lastUserName = LoginMgr:getLastUserName();
	LoginNameEdit:SetText(lastUserName);
	PassWordEdit:SetText("");
	if lastUserName == "" then
		SetFocusFrame("LoginNameEdit");
	else
		SetFocusFrame("PassWordEdit");
	end

	LoginSelectServerBtnFont:SetText("当前服务器："..SelectWorldInfo.world );

	local t_riches = 
	{	
		["LoginAcceptDescRich"] = 
		{	value = "已阅读并同意#L《用户协议》#n",
			color = { r = 255, g = 255, b = 255 },  
		}, 
	};

	util.CompleteSetRiches( t_riches );
	LoginAcceptUserProtocolChkBtn:setCheckState( LoginMgr:getReadUserProtocol() );

	local t_hideIcons = {	"CharacterSelectFrameTopZheZhaoTex","CharacterSelectFrameBottomZheZhaoTex", "CharacterSelectFrameLeftZheZhaoChiLunTex",
							"CharacterSelectFrameRightZheZhaoChiLunTex", "SelectFrame" };
	util.HideIcons( t_hideIcons );
end

function LoginFrame_OnHide()
	 LoginNameEdit:enableIME( false ); 
	 PassWordEdit:enableIME( false );
	 SmallKeyboardFrame:Hide();
	 LoginFrame_UserNameDropDownBtn:Hide();

	 if DirServer_InputRegisterCodeFrame:IsShown() then
		 DirServer_InputRegisterCodeFrame:Hide();
	 end
	 if RegisterUserFrame:IsShown() then
		RegisterUserFrame:Hide();
	 end
	 if ReadUserProtocolFrame:IsShown() then
		ReadUserProtocolFrame:Hide();
	 end
end

--注册账号
function LoginRegisterAccountBtn_OnClick()
	if RegisterUserFrame:IsShown() then
		RegisterUserFrame:Hide();
	else
		RegisterUserFrame:Show();
		RegisterUserFrame:SetClientUserData( 0,0 )
	end	
end

--账号充值
function LoginFillAccountBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" )
end

--返回
function LoginReturnBtn_OnClick()
	if PasswdCardFrame:IsShown() then
		PasswdCardFrameCloseBtn_OnClick()
		return
	end
	if LoginFrame:IsShown() then
		LoginFrame:Hide();
		LoginBackFrame:Hide();
	--	DirServerFrame:Show();
		DirSimpleServerFrame:Show();
		LoginMgr:breakNetConnect();
		DirFrame_OnClickRefurbishBtn();
		WaitQueueFrame:Hide();
	end
end

function LoginSaveUserNameChkBtn_OnClick()

end

function LoginKeyBoardBtn_OnClick()
	bIsSmallKeyboardCheck = not bIsSmallKeyboardCheck;
	local frame = getglobal( "SmallKeyboardFrame" );
	if frame:IsShown() then
		frame:Hide();
	else
		frame:Show();
	end
end

function LoginFrame_OnLoginBtnTabPressed()
	SetFocusFrame("LoginNameEdit");

	if SmallKeyboardFrame:IsShown() then
		SmallKeyboardFrame:Hide();
	end
end

function PassWordEdit_OnFocusGained()
	local frame = getglobal( "SmallKeyboardFrame" );
	if not frame:IsShown() then
		frame:Show();
	end
end

function LoginNameEdit_OnTabPressed()
	SetFocusFrame("PassWordEdit");
end

function LoginNameEdit_OnClick()
	if SmallKeyboardFrame:IsShown() then
		SmallKeyboardFrame:Hide();
	end
end

function LoginErrorMes( nRet )
	local t_ErrorInfo =
		{
			{ AUTH_ERR_SYS,								"服务器已关闭"},
			{ AUTH_ERR_NET,								"连接游戏服务器失败"},
			{ AUTH_REASON_PASSWD_ERROR,					"账号或者密码错误"},
			{ AUTH_REASON_NO_ACCOUNT,					"您输入的账号不存在，请注册账号",		"账号不存在"},
			{ AUTH_REASON_FREEZE,						"账号已冻结，请联系客服"},
			{ AUTH_IFM_ACCOUNT_ERROR,					"激活时,用户名不合法"},
			{ AUTH_IFM_PASSWD_ERROR,					"激活时,密码不合法"},
			{ AUTH_IFM_CERTID_ERROR,					"输入的身份证不正确"},		
			{ AUTH_IFM_AAS_ERROR,						"用户属于防沉迷用户,已经激活过一个大区了"},
			{ AUTH_IFM_NOUSE_SERVICE,					"无法使用服务"},
			{ AUTH_IFM_ACCOUNT_EXSIT,					"账户已经存在"},
			{ AUTH_IFM_ACCOUNT_ACTIVATION,				"账户已经激活游戏"},
			{ AUTH_IFM_ACCOUNT_REGISTER_ERROR,			"用户注册失败"},
			{ AUTH_IFM_ACCOUNT_VALID_ERROR,				"验证密码失败"},
			{ AUTH_IFM_GET_ACCOUNT_INFO_ERR,			"获取账户详细信息失败"},
			{ AUTH_IFM_GAME_REGION_ERROR,				"游戏大区编号不正确"},
			{ AUTH_IFM_GAME_ACTIVE_ERROR,				"激活游戏大区失败"},
			{ AUTH_IFM_USE_IDCARD_ERROR,				"用户身份证已经存在"},
			{ AUTH_IFM_USE_IDCARD_REGIST_TIMES_ERROR,	"该用户注册身份证超过50次"},
			{ AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR,		"该用户注册身份证为防沉迷用户，并且已存在"},
			{ AUTH_IFM_USE_IDCARD_FORMAT_ERROR,			"错误的身份证格式"},
			{ AUTH_IFM_NEED_REGIST_CODE_ERROR,			"内测期间，需要激活码"},
			{ AUTH_IFM_REGIST_CODE_ERROR,				"激活码不正确，无法激活游戏"},
			{ AUTH_IFM_ACCOUNT_REGISTER_SUCCES,			"新手礼包的cdkey已经发送到账号邮箱中，请查收"},
			{ AUTH_IFM_SYSTEM_BUSY,						"服务器繁忙"},
			{ AUTH_REASON_FREEZE_TRYMAX,				"密码尝试次数过多，账号冻结10分钟"},
			{ AUTH_REASON_FREEZE_CHECKTOOL,				"账号行为异常，已被冻结一段时间"},
		}
	PasswdCardFrame:Hide()
	nLastRequestTime = 0;
	LoginSelectServerBtnFont:SetText("当前服务器："..SelectWorldInfo.world );
	for i = 1, table.getn( t_ErrorInfo ) do
		if nRet == t_ErrorInfo[i][1] then
			if nRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES then
				return
			end
			if t_ErrorInfo[i][3] then
				Login_MessageBox( { ["text"] = t_ErrorInfo[i][2] .. "(" .. nRet .. ")", ["opDesc"] = t_ErrorInfo[i][3] } );
			else
				Login_MessageBoxOK( { ["text"] = t_ErrorInfo[i][2] .. "(" .. nRet .. ")", ["opDesc"] = "" } );
			end
			ClearFocus();
			--[[
			if nRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES then
				GameMgr:SendHttpRequest("http://www.ljy0.comm/other/sendcdkey.php?username=" .. LoginMgr:getUserName() );
			end
			]]--
			return;
		end
	end
	Login_MessageBoxOK( { ["text"] = "登录失败" .. "(" .. nRet .. ")", ["opDesc"] = "" } );
	ClearFocus();
end

function LoginFrame_OnEnterPressed()
	if GetFocusFrameName() == "LoginNameEdit" then
		SetFocusFrame("PassWordEdit");
		return;
	end
	return LoginFrame_OnLoginBtnClick();
end

function LoginBackFrame_OnHide()
	--不显示的时候，释放占有的ui 资源
	--LoginBackFrameBackgroundTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
end

function LoginBackFrame_OnShow()
	--LoginBackFrameBackgroundTex:SetTexture("uires\\TuPianLei\\DengLuBeiJing.jpg");
	Private_UpdateBackTex();

	local t_chgAlphaTex = { "LoginBackFrameBackgroundTex" };
	for _, name in ipairs( t_chgAlphaTex ) do
		local tex = getglobal( name );
		tex:SetBlendAlpha( 1 );
	end
	
	local t_chgAlphaFont = { "LoginBackFrameBackgroundFont1", "LoginBackFrameBackgroundFont2" };
	for _, name in ipairs( t_chgAlphaFont ) do
		local font = getglobal( name );
		font:SetBlendAlpha( 1 );
	end
end

function Private_UpdateBackTex()
	local tex = getglobal("LoginBackFrameBackgroundTex");
	local nRealWidth  = tex:getRelWidth();
	local nRealHeight = tex:getRelHeight();

	-- 裁剪图片
	local nScale = nRealWidth / GetScreenWidth();
	local nScreenHeight =  GetScreenHeight() * nScale;
	if nScreenHeight > nRealHeight then
		nScale = nRealHeight / GetScreenHeight();
	end

	local nCutWidth		= math.abs( (nRealWidth - GetScreenWidth() * nScale)/2 );   
	local nCutHeight	= math.abs( (nRealHeight - GetScreenHeight() * nScale)/2 ); 
	tex:SetTexUV( nCutWidth, nCutHeight, (nRealWidth-2*nCutWidth), (nRealHeight-2*nCutHeight) );	

	local nBackScale = ( GetScreenWidth()/ GetScreenScaleY() ) / (nRealWidth-2*nCutWidth);
	LoginBackFrameBackground:SetSize( (nRealWidth-2*nCutWidth)*nBackScale, (nRealHeight-2*nCutHeight)*nBackScale );
end

function LoginBackFrame_OnUpdate()
	Private_UpdateBackTex();
end

---------------------------注册账号---------------------------------

function RegisterUserFrameOKBtn_OnClick()
	local szAccount = RegisterUserFrameAccountEdit:GetText();
	if szAccount == nil or szAccount == "" then
		Login_MessageBoxOK( { ["text"] = "请输入账号", ["opDesc"] = "" } );
		return;
	end

	szAccount = string.lower( szAccount )
	if string.len(szAccount) < 6 then
		Login_MessageBoxOK( { ["text"] = "账号长度不能少于6位", ["opDesc"] = "" } );
		return
	end
	if string.len(szAccount) > 30 then
		Login_MessageBoxOK( { ["text"] = "账号长度不能超过30位", ["opDesc"] = "" } );
		return;
	end
	if string.find(szAccount, "[^%w@_/./-]") then
		Login_MessageBoxOK( { ["text"] = "账号名称只能由英文或数字及邮箱常用字符组成", ["opDesc"] = "" } );
		return;
	end
	
	local szPwd = RegisterUserFramePwdEdit:GetPassWord();
	if szPwd == nil or szPwd == "" then
		Login_MessageBoxOK( { ["text"] = "请输入密码", ["opDesc"] = "" } );
		return
	end
	local szConfrimPwd = RegisterUserFrameConfirmPwdEdit:GetPassWord();
	if not (szPwd == szConfrimPwd) then
		Login_MessageBoxOK( { ["text"] = "两次输入的密码不匹配", ["opDesc"] = "" } );
		return;
	end
	if szPwd == szAccount then 
		Login_MessageBoxOK( { ["text"] = "密码不能与账号相同", ["opDesc"] = "" } );
		return;
	end
	if string.len(szPwd) < 6 or string.len(szPwd) > 16 then
		Login_MessageBoxOK( { ["text"] = "密码长度必须在6到16位之间", ["opDesc"] = "" } );
		return;
	end		

	LoginMgr:requestCreateUser(	SelectWorldInfo.bigWorldIndex - 1,
								SelectWorldInfo.worldIndex - 1,
								szAccount,
								szPwd,
								"",
								"",
								"",
								"" );
	
	ClearOtherWorldRoleData()
	RegisterUserFrame:Hide();
end

function RegisterUserFrame_OnShow()
	RegisterUserFrameDescRich:SetText("（带#cFF8700*#cA0A05A为必填项）",160,160,90);	
	RegisterUserFrameAccountEdit:Clear();
	RegisterUserFramePwdEdit:Clear();
	RegisterUserFrameConfirmPwdEdit:Clear();
	if LoginNameEdit:GetClientString() ~= "" then
		RegisterUserFrameAccountEdit:SetText( LoginNameEdit:GetClientString() );
	end
	SetFocusFrame("RegisterUserFrameAccountEdit");
end

function RegisterUserFrame_OnHide()
	ClearFocus();
end

local isStart = false;
function isNeedRegister()
	return isStart;
end

function SetRegisterState( state )
	isStart = state;
end

function ShowContinueRegisterFrame()
	if not ContinueRegisterUserFrame:IsShown() then
		ContinueRegisterUserFrame:Show();		
	end
end

function HideContinueRegisterFrame()
	ContinueRegisterUserFrame:Hide();
	isStart = false;
end

function ContinueRegisterUserFrame_OnShow()
	ContinueRegisterUserFrameDescRich:SetText("（带#cFF8700*#cA0A05A为必填项）",160,160,90);
	ContinueRegisterUserFrameCertEdit:Clear();
	ContinueRegisterUserFrameNameEdit:Clear();
	ContinueRegisterUserFrameMailEdit:Clear();
	isStart = true;
end

function ContinueRegisterUserFrame_OnHide()
	ClearFocus();
	setTimeTick();
end

function ContinueRegisterUserFrameOKBtn_OnClick()	
	local szCert = ContinueRegisterUserFrameCertEdit:GetText();	
	if szCert == nil or szCert == "" or not( string.len(szCert) == 15 or string.len(szCert) == 18 ) then
		Login_MessageBoxOK( { ["text"] = "请输入正确的身份证号", ["opDesc"] = "" } );
		return;
	end
	if not string.find( szCert,"x$" ) and not string.find( szCert,"X$" ) and not string.find( szCert,"%d$" ) then
		Login_MessageBoxOK( { ["text"] = "请输入正确的身份证号", ["opDesc"] = "" } );
		return;
	end
	if not string.find( szCert,"^%d+%a?$" ) then
		Login_MessageBoxOK( { ["text"] = "请输入正确的身份证号", ["opDesc"] = "" } );
		return;
	end
	local ss = string.sub( szCert,string.find( szCert,"^%d+%a?$" ) );
	if ss ~= szCert then
		Login_MessageBoxOK( { ["text"] = "请输入正确的身份证号", ["opDesc"] = "" } );
		return;
	end

	if not age_detect(szCert) then
		Login_MessageBoxOK( { ["text"] = "当前身份证信息未满18周岁", ["opDesc"] = "" } );
		return;
	end
	
	local szName = ContinueRegisterUserFrameNameEdit:GetText();
	if szName == nil or szName == "" then
		Login_MessageBoxOK( { ["text"] = "请输入真实姓名", ["opDesc"] = "" } );
		return;
	end

	local szEmail = ContinueRegisterUserFrameMailEdit:GetText();
	if szEmail == nil or szEmail == "" then
		Login_MessageBoxOK( { ["text"] = "请输入安全邮箱", ["opDesc"] = "" } );
		return;
	end
	szEmail = string.lower( szEmail );
	local _,pos = string.find(szEmail, "^%w[%w_/./-]*@[%w_/./-]*%w$")
	if pos ~= string.len( szEmail ) then
		Login_MessageBoxOK( { ["text"] = "请输入正确的邮箱地址", ["opDesc"] = "" } );
		return
	end

	LoginMgr:continueRegisterUserInfo( szCert, szName, szEmail );
	ContinueRegisterUserFrame:Hide();
end

--[[提示邮箱后缀下拉框
local tMailName = { "@qq.com", "@hotmail.com", "@gmail.com", "@yahoo.com", "@sina.com", "@163.com", "@126.com", "@vip.sina.com", "@sina.cn", "@sohu.com", "@live.com", "@live.cn" };
local MAIL_NAME_BTN_NUM = 12;
function RegisterUserFrameAccountEdit_OnUpdate()
	if not RegisterUserFrame:IsShown() or GetFocusFrameName() ~= "RegisterUserFrameAccountEdit" then
		return;
	end
	local szAccount = RegisterUserFrameAccountEdit:GetText();
	local nLen = string.len( szAccount );
	if nLen > 2 then
		RegisterUserFrame_AccountDropDownBtn:Show();		
	else
		RegisterUserFrame_AccountDropDownBtn:Hide();
	end
end

function RegisterUserFrame_AccountDropDownBtn_OnShow()
	RegisterUserFrame_AccountDropDownBtn:SetPoint( "topleft","RegisterUserFrameAccountEdit","bottomleft", -6, 4 );
	for i = 1, MAIL_NAME_BTN_NUM do
		local btn = getglobal( "RegisterUserFrame_AccountDropDownBtnName" .. i );
		local font = getglobal( btn:GetName() .. "Name");
		font:SetText("");
		btn:Hide();
	end
	local szAccount = RegisterUserFrameAccountEdit:GetText();
	local nNameIndex = string.find( szAccount, "@" );
	local nNum = 0;
	if nNameIndex ~= nil then
		local nLen = string.len( szAccount );
		local szText = string.sub( szAccount, nNameIndex, nLen );
		local szTmpText = string.sub( szAccount, 1, nNameIndex - 1);
		for i = 1, MAIL_NAME_BTN_NUM do			
			if string.find( tMailName[i], szText ) ~= nil then
				local btn = getglobal( "RegisterUserFrame_AccountDropDownBtnName" .. nNum + 1 );			
				local font = getglobal( btn:GetName() .. "Name");
				font:SetText( szTmpText .. tMailName[i] );
				btn:Show();
				nNum = nNum + 1;
			end
		end
		if nNum == 0 then
			local btn = getglobal( "RegisterUserFrame_AccountDropDownBtnName" .. nNum + 1 );
			local font = getglobal( btn:GetName() .. "Name");
			font:SetText( szAccount );
			btn:Show();
			nNum = nNum + 1;
		end
	else		
		for i = 1, MAIL_NAME_BTN_NUM do
			local btn = getglobal( "RegisterUserFrame_AccountDropDownBtnName" .. i );			
			local font = getglobal( btn:GetName() .. "Name");
			font:SetText( szAccount .. tMailName[i] );
			btn:Show();
			nNum = nNum + 1;
		end		
	end
	if nNum == 1 or nNum == 0 then
		RegisterUserFrame_AccountDropDownBtn:SetHeight( 23 );
	else
		RegisterUserFrame_AccountDropDownBtn:SetHeight( nNum * 20 + 3 );
	end
end

function RegisterUserFrameAccountBtn_OnClick()
	local font = getglobal( this:GetName() .. "Name" );
	RegisterUserFrameAccountEdit:Clear();
	RegisterUserFrameAccountEdit:SetText( font:GetText() );
	SetFocusFrame("RegisterUserFramePwdEdit");
	RegisterUserFrame_AccountDropDownBtn:Hide();
end

function RegisterUserFrameAccountEdit_OnTab()
	RegisterUserFrame_AccountDropDownBtn:Hide();
	SetFocusFrame("RegisterUserFramePwdEdit");
end
--]]

-- #P用户协议#n
local szUserProtocolText = 
[[#P软件最终用户使用许可协议#n

重要须知：

用户在使用本软件及相关各项服务之前，请仔细阅读本软件最终用户使用许可协议(下称本协议)中各项条款。许可人在此特别提醒用户认真阅读本协议的全部条款，特别是其中免除或者限制许可人责任的条款（该等条款通常含有“不负任何责任”、“无义务”等词汇）和其它限制用户权利的条款（该等条款通常含有“不得”等词汇），这些条款应在中国法律所允许的范围内最大程度地适用。

如用户不同意本协议的条款，请不要安装、使用本软件或者相关服务。用户一旦点击“接受”并安装本软件，或者注册、开始使用及/或继续使用本软件或相关服务，即视为用户已经接受本协议中全部条款。

用户有权在使用本软件及相关服务的期间，监督许可人是否按照许可人所公布的标准向用户提供服务，也可以随时向许可人提出与许可人的产品和服务有关意见和建议。 

1．定义和说明

本协议中，如无相反说明，以下词语具有如下含义：

1.1 本软件：包含本软件的客户端内容及程序，并可能包括相关网络服务器、网站（包括但不限于游戏官方网站）、电子媒体、印刷材料和“联机”或电子文档等。

1.2 用户：又称“最终用户”，指通过许可人提供的合法途径获得本软件使用许可而安装、使用本软件产品及服务的个人。 

1.3 许可人：指深圳市万游引力科技有限公司。

2．知识产权声明

2.1 许可人依法享有本软件的一切合法权属（包括但不限于所有权和知识产权）。本软件的全部著作权及一切其他知识产权，以及与本软件相关的所有信息内容，包括但不限于：文字表述及其组合、软件名称、电脑代码、主题、物品、角色、角色名、故事情节、对话、场景设置、概念、美术工艺、动画、音效、音乐、视觉效果、运行方式、精神权、相关文档、程序、聊天室脚本、印刷材料、电子文档等均受中国知识产权相关法律法规和有关国际条约的保护。

2.2 本软件所体现的“许可人”和本软件相关的产品和服务标识均属于许可人在中国及/或其他国家/地区的商标或者注册商标，用户未经许可人书面授权不得自行使用。

2.3 许可人在涉及本软件及相关各项服务中拥有专利权、专利申请权、商标权、著作权及其他知识产权。许可人并未因为本协议或者因为向用户提供本软件以及相关服务而授予用户任何与本软件相关的知识产权。本协议未明确授予用户的权利均由许可人保留。

3．隐私政策

尊重用户个人隐私是许可人的一项基本政策，所以，许可人不会公开、修改或透露用户的注册资料及保存在许可人各项服务中的非公开内容，除非许可人在诚信的基础上认为透露这些信息是必要的，包括但不限于以下情形：

3.1 遵守有关法律法规规定，包括在国家有关机关查询时，提供用户在许可人的网页上发布的信息内容及其发布时间、互联网地址或者域名。

3.2 保持维护许可人的知识产权和其他重要权利。

3.3 在紧急情况下竭力维护用户个人和社会大众的隐私安全。

3.4 根据许可人用户协议、许可人公布的规则、制度或者许可人认为必要的其他情况下。

4．责任限制 

4.1 用户承诺自行承担因使用本软件及相关服务所引起的任何风险。除根据特定目的和不违反规定的明确的适当的担保以外，许可人不作其他任何类型的担保（不论是明确的或隐含的）。许可人不担保：

4.1.1 本软件及许可人提供的相关服务符合用户的要求；

4.1.2 本软件及许可人提供的相关服务不受不可抗力、计算机病毒、黑客攻击、系统不稳定、用户所在位置、用户关机、电信部门原因及其他任何网络、技术、通信线路等外界或人为因素的影响；

4.1.3 安装、复制、访问网站、充值、运行本软件或以其它方式使用本软件及/或接受许可人提供的相关服务与任何其他软件不存在任何冲突；

4.1.4 通过许可人网站、游戏官方网站及其他相关网络上的链接和标签所指向的第三方的商业信誉及其提供服务的真实性、有效性和合法性。

5. 软件、服务与协议的变更、终止

5.1 为了保障许可人公司业务发展，许可人拥有变更本软件（包括但不限于升级、更新、增加、减少、演绎等）及相关服务以及本协议内容的权利，也拥有终止本软件/相关服务/本协议的权利；如有必要，许可人会以公告等形式公布于本软件服务网站重要页面上。许可人提请用户定期查阅了解有关变更、终止等信息。 

5.2 用户拥有自主权利，单方面随时决定终止使用本软件及相关服务并卸载本软件。如许可人对本软件、相关服务或本协议的内容作出任何变更，而用户不同意有关变更的内容，用户有权单方面立即停止使用本软件以及相关服务并卸载本软件。如用户在有关内容变更后，仍继续使用本软件和相关服务，即视为用户同意接受有关变更内容。

6．权利和义务

6.1 在用户同意接受本协议全部内容的前提下，许可人同意授予用户可撤销的、可变更的、非专有的、不可转让和不可再授权的许可权利。用户可在许可生效的时间内将本软件安装在个人使用的联网计算机上，并以指定的方式运行本软件的一份副本并享受许可人提供的服务。许可人基于本协议授予用户的许可是针对个人使用的许可。如用户有需要在个人使用的范围以外使用本软件及服务或者将本软件与服务用于任何商业用途，则用户应与许可人联系并获得许可人另行书面授权。任何未经许可人许可的安装、使用、访问、显示、运行等行为均视为违反本协议。

6.2 除非本协议另有规定，否则，未经许可人书面同意，用户不得实施下列行为（无论是营利的还是非营利的）：

6.2.1 复制、翻拷、传播和陈列本软件的程序、使用手册和其它图文音像资料的全部或部分内容。

6.2.2 公开展示和播放本软件的全部或部分内容。

6.2.3 出租、销售本软件或者利用本软件从事营利行为。

6.2.4 修改或遮盖本软件程序、图像、动画、包装和手册等内容上的产品名称、公司标志、版权信息等内容。

6.2.5 其它违反著作权法、计算机软件保护条例和相关法律法规的行为。

6.3 用户应通过合法的途径使用本软件和相关服务，不得作出以下侵害许可人或第三人利益、违反本协议或者许可人发布的其他规则或者国家有关法律法规规定的行为，包括但不限于：

6.3.1 修改、翻译、注释、整理、汇编、演绎本软件；

6.3.2 反向工程、反向编译或者反汇编本软件，或者采用其他技术手段对本软件进行分析、修改、攻击、衍生；

6.3.3 使用任何外挂程序或第三方修改程序（本协议所称“外挂程序”是指独立于本软件之外的，能够在本软件运行的同时影响本软件操作的所有程序，包括但不限于模拟键盘鼠标操作、改变操作环境、修改数据等一切类型。如国家有关法律、法规及政府主管部门的规章或规范性文件规定的外挂定义与本协议有冲突，则以法律、法规、部门规章或规范性文件规定的为准），对本软件进行还原工程、编译、译码或修改，包括但不限于修改本软件所使用的任何专有通讯协议、对动态随机存取内存（RAM）中资料进行修改或锁定；

6.3.4 使用异常的方法登录本软件、使用网络加速器等外挂软件或机器人程式等恶意破坏本软件服务设施、扰乱本软件正常服务秩序的行为；

6.3.5 通过异常或者非法方式使用本软件（包括但不限于利用本软件登录非本软件官方服务器），恶意破坏本软件，扰乱本软件正常的服务秩序或者实施其他不正当行为；

6.3.6 制作、传播或使用外挂、封包、加速软件及其它各种作弊程序，或组织、教唆他人使用此类软件程序，或销售此类软件程序而为个人或组织谋取经济利益；

6.3.7 使用任何方式或方法，试图攻击提供本软件服务的相关服务器、路由器、交换机以及其他设备，以达到非法获得或修改未经授权的数据资料、影响本软件的正常运行服务，以及其他危害性目的的任何行为；

6.3.8 利用本软件或者线上系统可能存在的技术缺陷或漏洞而以各种形式为自己及他人牟利（包括但不限于复制本软件中的虚拟物品等）或者从事其他不正当行为。

6.3.9 违反国家有关法律法规的规定，利用本软件制作、复制、发布和传播信息。

6.4 一旦许可人通过内部的监测程序发现或经其他用户举报而发现用户有可能正在从事上述行为，则许可人有权作出独立的判断并采取相应的措施，该措施包括但不限于限制用户账号的登录、限制用户在本软件运行过程中的活动、终止本软件授权、删除与复制有关的物品（包括复制出的虚拟物品和参与复制的虚拟物品）、删除用户的账号并要求用户赔偿因从事上述行为而给许可人造成的损失等。

6.5 许可人提供可使用本软件，并运用自己的网络系统通过国际互联网络为用户提供服务。同时，用户应自行提供以下的设备和信息：

6.5.1 自行配备上网的所需设备， 包括个人电脑、调制解调器或其他必备上网装置。

6.5.2 自行负担个人上网所支付的与此服务有关的电话费用、网络费用。

6.5.3 基于许可人提供服务的重要性，用户同意：
（1）提供详尽、准确的个人资料。
（2）不断更新注册资料，符合及时、详尽、准确的要求。
（3）牢记用户填写的注册资料、历史信息。

6.6 许可人在为用户提供相关客户服务的前提是用户能表明用户是账号的所有人，这可能需要用户提供相关信息（包括但不限于注册信息、历史密码等）。用户理解，如果用户不能提供准确完整的注册资料及相关历史信息、未及时更新相关注册资料或者相关的证据，将有可能导致许可人无法判断用户的身份，从而无法为提供密码找回等相关服务，而许可人对此不承担任何责任。如因用户提交的相关信息资料不真实、不准确、不完整、不合法从而导致许可人作出错误的判断的，许可人有权终止为用户提供服务并追究用户的法律责任。

6.7 许可人保留通过本软件和相关的服务向用户投放商业性广告的权利。

7．违约责任

7.1 用户同意保障和维护许可人及其他用户的利益，如因用户违反有关法律、法规或本协议项下的任何条款而给许可人造成损害，用户同意承担由此造成的损害赔偿责任，该等责任包括但不限于给许可人造成的任何直接或间接损失。

7.2 如用户违反本协议、许可人发布的其他规则或者国家法律法规规定，则许可人有权作出独立的判断，立即撤销相关许可，终止为用户提供服务并通过各种合法途径追究用户的法律责任。

8．法律适用和争议解决

8.1 本协议的订立、履行、解释及争议的解决均应适用中华人民共和国法律并排除其他一切冲突法的适用。

8.2 如双方就本协议内容或其执行发生任何争议，双方应尽量友好协商解决；协商不成时，任何一方均应向深圳市南山区人民法院提起诉讼。

9．其他规定

9.1 本协议构成双方关于本软件许可使用事项的完整协议。

9.2 如本协议中的任何条款因任何原因被判定为完全或部分无效或不具有执行力的，该无效或不具有执行力的条款将被最接近原条款意图的一项有效并可执行的条款所替代，并且本协议的其余条款仍应有效并且有执行力。

9.3 许可人不行使、未能及时行使或者未充分行使本协议或者按照法律规定所享有的权利，不应被视为放弃该权利，也不影响许可人在将来行使该权利。

9.4 在法律允许的最大范围内，许可人保留对本协议的最终解释权。 ]];

function ReadUserProtocolFrame_OnShow()
	local t_riches = 
	{ 
		["ReadUserProtocolFrameDescRich"] = { value = szUserProtocolText, color =  { r = 50, g = 30, b = 10 } }, 
	};
	util.SetRiches( t_riches );

	this:SetPoint( "center", "UIClient", "center", 0, 0 );

	local rich		= getglobal( "ReadUserProtocolFrameDescRich" );
	local slider	= getglobal( this:GetName() .. "_ScrollBar" );
	slider:SetValue( 0 );
	SetScrollBar( slider:GetName(), rich:GetAccurateViewLines(), rich:GetTextLines() );		
	util.UpdateRich( rich:GetName(), slider:GetValue() );
end

function ReadUserProtocolFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( this:GetParent().."_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function ReadUserProtocolFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( this:GetParent().."_ScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function ReadUserProtocolFrame_ScrollBar_OnValueChanged()
	local nValue	 = this:GetValue();
	local nMaxValue	 = this:GetMaxValue();

	if nValue <= nMaxValue then
		util.UpdateRich( "ReadUserProtocolFrameDescRich", nValue )
	end
end

function LoginAcceptUserProtocolChkBtn_OnClick()
	LoginMgr:setReadUserProtocol( this:GetCheckState() );
end

function LoginFrame_SelectUserNameBtn_OnClick()
	-- 显示下拉界面
	if not LoginFrame_UserNameDropDownBtn:IsShown() then
		LoginFrame_UserNameDropDownBtn:Show();
	else
		LoginFrame_UserNameDropDownBtn:Hide();
	end
	
end

function LoginFrameUserNameBtn_OnClick()
	local font = getglobal( this:GetName() .. "Name" );
	LoginNameEdit:Clear();
	LoginNameEdit:SetText( font:GetText() );
	SetFocusFrame("PassWordEdit");
	LoginFrame_UserNameDropDownBtn:Hide();
end

function LoginFrame_UserNameDropDownBtn_OnShow()
	-- 设置
	LoginFrame_UserNameDropDownBtn:SetPoint( "topleft","LoginNameEdit","bottomleft",-3,0 );
	local nIdx = 1;
	for i=1,MAX_SAVE_NAME_NUM do
		local szName = LoginMgr:getUserNameByIdx( i-1 );
		if szName ~= "" then
			local btn = getglobal( "LoginFrame_UserNameDropDownBtnName"..nIdx );
			local font = getglobal( btn:GetName() .. "Name");
			font:SetText( szName );
			font:SetPoint( "topleft","LoginFrame_UserNameDropDownBtnName"..nIdx,"topleft", -1,2 );
			btn:Show();
			nIdx = nIdx + 1;
		end
	end
	for i=nIdx,MAX_SAVE_NAME_NUM do
		local btn = getglobal( "LoginFrame_UserNameDropDownBtnName" .. i );		
		btn:Hide();
	end
	if nIdx == 1 then
		LoginFrame_UserNameDropDownBtn:Hide();
	else
		LoginFrame_UserNameDropDownBtn:SetSize( 245,24*(nIdx-1) );
	end
end

function LoginQuickStartBtn_OnClick()
	local t_hideIcons = {	"CharacterSelectFrameTopZheZhaoTex","CharacterSelectFrameBottomZheZhaoTex", "CharacterSelectFrameLeftZheZhaoChiLunTex",
							"CharacterSelectFrameRightZheZhaoChiLunTex", "SelectFrame" };
	util.HideIcons( t_hideIcons );
	bQuickStart = true;
	LoginMgr:requestEnterLoginWithoutUserName();
	local nLastSelectAreaIndex	= LoginMgr:getLastSelectArea();
	local nLastSelectWorldIndex = LoginMgr:getLastSelectWorld();
	local nAreaIndex, nWorldIndex = GetFirstRecommendWorld();

	if nLastSelectAreaIndex <= 0 or nLastSelectAreaIndex >= LoginMgr:getNumBigWorld() then
		nLastSelectAreaIndex = nAreaIndex;
	end

	local oneBigWorld = LoginMgr:getBigWorld( nLastSelectAreaIndex );
	if nLastSelectWorldIndex <= 0 or nLastSelectWorldIndex >= oneBigWorld:getNumWorld() then
		nLastSelectWorldIndex = nWorldIndex;
	end

	local world = oneBigWorld:getIthWorld( nLastSelectWorldIndex );
	SelectWorldInfo.bigworld = oneBigWorld:getName();
	SaveSelectWorldInfo( world, nLastSelectAreaIndex + 1, nLastSelectWorldIndex + 1 );
end

function OnQuickRegisterResult( result )
	if LoginMgr:isQuickStart() then
		QuickRegisterBtn:Show();
	else
		QuickRegisterBtn:Hide();
	end
	if result ~= 0 then
		local t_ErrorInfo =
		{
			{ AUTH_ERR_SYS,								"服务器已关闭"},
			{ AUTH_ERR_NET,								"连接游戏服务器失败"},
			{ AUTH_REASON_PASSWD_ERROR,					"账号或者密码错误"},
			{ AUTH_REASON_NO_ACCOUNT,					"账号或者密码错误"},
			{ AUTH_REASON_FREEZE,						"账号未激活该大区"},
			{ AUTH_IFM_ACCOUNT_ERROR,					"激活时,用户名不合法"},
			{ AUTH_IFM_PASSWD_ERROR,					"激活时,密码不合法"},
			{ AUTH_IFM_CERTID_ERROR,					"输入的身份证不正确"},		
			{ AUTH_IFM_AAS_ERROR,						"用户属于防沉迷用户,已经激活过一个大区了"},
			{ AUTH_IFM_NOUSE_SERVICE,					"无法使用服务"},
			{ AUTH_IFM_ACCOUNT_EXSIT,					"账户已经存在"},
			{ AUTH_IFM_ACCOUNT_ACTIVATION,				"账户已经激活游戏"},
			{ AUTH_IFM_ACCOUNT_REGISTER_ERROR,			"用户注册失败"},
			{ AUTH_IFM_ACCOUNT_VALID_ERROR,				"验证密码失败"},
			{ AUTH_IFM_GET_ACCOUNT_INFO_ERR,			"获取账户详细信息失败"},
			{ AUTH_IFM_GAME_REGION_ERROR,				"游戏大区编号不正确"},
			{ AUTH_IFM_GAME_ACTIVE_ERROR,				"激活游戏大区失败"},
			{ AUTH_IFM_USE_IDCARD_ERROR,				"用户身份证已经存在"},
			{ AUTH_IFM_USE_IDCARD_REGIST_TIMES_ERROR,	"该用户注册身份证超过50次"},
			{ AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR,		"该用户注册身份证为防沉迷用户，并且已存在"},
			{ AUTH_IFM_USE_IDCARD_FORMAT_ERROR,			"错误的身份证格式"},
			{ AUTH_IFM_NEED_REGIST_CODE_ERROR,			"内测期间，需要激活码"},
			{ AUTH_IFM_REGIST_CODE_ERROR,				"激活码不正确，无法激活游戏"},
			{ AUTH_IFM_SYSTEM_BUSY,						"服务器繁忙"},
			{ AUTH_REASON_FREEZE_TRYMAX,				"密码尝试次数过多，账号冻结6小时"},
			{ AUTH_REASON_FREEZE_CHECKTOOL,				"账号行为异常，已被冻结一段时间"},
		}
		for i = 1, table.getn( t_ErrorInfo ) do
			if result == t_ErrorInfo[i][1] then
				MessageBoxOK( "", t_ErrorInfo[i][2] );
			end
		end
	else
		if LoginSaveUserNameChkBtn:GetCheckState() then
			LoginMgr:saveLastUserName();
		else
			LoginMgr:clearLastUserName();
		end
		--[[
		if nRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES then
			GameMgr:SendHttpRequest("http://www.ljy0.com/other/sendcdkey.php?username=" .. LoginMgr:getUserName() );
			Login_MessageBoxOK("","新手礼包的cdkey已经发送到账号邮箱中，请查收");
		end
		]]--
	end
end

----------------------------------------手机绑定----------------------------------------------
local LastRequestTime = 0;
local LastCode = 0;
local t_resultDesc = { 
			[0] = { "认证成功", 1 },
			[1] = { "验证码错误", 0 },
			[2] = { "短信认证码过期", 0 },
			[3] = { "没有创建或删除了角色", 1 },
			[4] = { "增加礼包失败", 1 },
			[5] = { "礼包重复领取", 1 },
			[6] = { "未输入手机号", 0 },
			[7] = { "短信息发送失败", 0 },
			[8] = { "短信息密码获取太频繁", 0 },
			};

function PrebuildMobileCheckFrameRequestBtn_OnUpdate()
	if LastRequestTime > 0 and this:GetText() ~= "获取验证码" then
		local num = PRECREATE_GIFT_VERIFY_TIMEOUT_TOLUA - math.floor( os.clock() - LastRequestTime );
		if num > 0 then
			this:SetText( num .. "秒" );
		else
			this:SetText( "获取验证码" );
			this:Enable();
			LastRequestTime = 0;
		end
	end
end

function PrebuildMobileCheckFrameRequestBtn_OnClick()
	local phone = PrebuildMobileCheckFrameMobileShowBtnFont:GetText();
	if tonumber( phone ) == nil or string.len( phone ) < 8 then
		return Login_MessageBoxOK( { ["text"] = "请输入正确的手机号码", ["opDesc"] = "" } );
	end
	if LastRequestTime == 0 or os.clock() - LastRequestTime > 1 then
		LastRequestTime = os.clock();
		LoginMgr:requestGetSMS( phone );
	end
end

function PrebuildMobileCheckFrame_OnShow()
	PrebuildMobileCheckFrameRequestBtn:SetText( "获取验证码" );
	PrebuildMobileCheckFrameRequestBtn:Enable();
	LastRequestTime = 0;
	LastCode = 0;
	PrebuildMobileCheckFrameMobileEdit:SetText( "" );
	PrebuildMobileCheckFrameMobileEdit_OnFocusLost();
	PrebuildMobileCheckFrameCodeEdit:SetText( "" );
	PrebuildMobileCheckFrameCodeNum:SetText( "" );
end

function PrebuildMobileCheckFrame_OnLoad()
	this:RegisterEvent("GE_UPDATE_ROLELIST");
	this:RegisterEvent("GE_KICKOFF_INFO");
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN");
end

function PrebuildMobileCheckFrame_OnEvent()
	if not this:IsShown() then return end;
	if arg1 == "GE_KICKOFF_INFO" or arg1 == "GE_ENTER_PLAYERLOGIN" then
		this:Hide();
	elseif arg1 == "GE_UPDATE_ROLELIST" then
		if LoginMgr:getNumRole() == 0 then
			this:Hide();
		end
	end
end

function PrebuildMobileCheckFrameMobileShowBtn_OnClick()
	this:Hide();
	local szText = PrebuildMobileCheckFrameMobileShowBtnFont:GetText();
	szText = tonumber( szText ) ~= nil and szText or "";
	PrebuildMobileCheckFrameMobileEdit:SetText( szText );
	PrebuildMobileCheckFrameMobileEdit:Show();
	SetFocusFrame( "PrebuildMobileCheckFrameMobileEdit" );
	PrebuildMobileCheckFrameMobileEdit:SelectAllText();
end

function PrebuildMobileCheckFrameMobileEdit_OnFocusLost()
	local edit = getglobal( "PrebuildMobileCheckFrameMobileEdit" );
	edit:Hide();
	local szText = tonumber( edit:GetText() ) ~= nil and edit:GetText() or "点击填写手机号码";
	PrebuildMobileCheckFrameMobileShowBtnFont:SetText( szText );
	PrebuildMobileCheckFrameMobileShowBtn:Show();
end

function PrebuildMobileCheckFrameAcceptBtn_OnClick()
	if SelectRoleWarnFrame:IsShown() then return end;
	if LastCode == 0 then
		--短信验证屏蔽
		--return Login_MessageBoxOK( { ["text"] = "请先获取验证码", ["opDesc"] = "" } );
	end
	local phone = PrebuildMobileCheckFrameMobileShowBtnFont:GetText();
	if tonumber( phone ) == nil or string.len( phone ) < 8 then
		return Login_MessageBoxOK( { ["text"] = "请输入正确的手机号码", ["opDesc"] = "" } );
	end
	local code = PrebuildMobileCheckFrameCodeEdit:GetText();
	if tonumber( code ) == nil or string.len( code ) < 6 then
		--return Login_MessageBoxOK( { ["text"] = "请输入正确的验证码", ["opDesc"] = "" } );
	end
	--手机验证码取消，但是要先发送下获取验证码的消息，以通知服务器手机号
	if LastRequestTime == 0 or os.clock() - LastRequestTime > 1 then
		LastRequestTime = os.clock();
		LoginMgr:requestGetSMS( phone );
	end
	--LoginMgr:requsetVerifySMS( tonumber(code) );
	LoginMgr:requsetVerifySMS( 0 );
end

function UpdatePrebuildMobileCheckFrame( num )
	if num > 0 then
		LastCode = num;
		PrebuildMobileCheckFrameCodeEdit:SetText( "" );
		PrebuildMobileCheckFrameCodeNum:SetText( "验证码序号：" .. num );
		LastRequestTime = os.clock();
		PrebuildMobileCheckFrameRequestBtn:SetText( PRECREATE_GIFT_VERIFY_TIMEOUT_TOLUA .. "秒" );
		PrebuildMobileCheckFrameRequestBtn:Disable();
	else
		PrebuildMobileCheckFrame:Show();
	end
end

function UpdateMobileCheckResult( idx )
	if idx > #t_resultDesc then return end;
	Login_MessageBoxOK( { ["text"] = t_resultDesc[idx][1], ["opDesc"] = "" } );
	if t_resultDesc[idx][2] > 0 then
		PrebuildMobileCheckFrame:Hide();
	end
end