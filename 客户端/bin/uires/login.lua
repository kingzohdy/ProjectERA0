--��¼���
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
		Login_MessageBoxOK( { ["text"] = "�˺Ų���Ϊ��", ["opDesc"] = "" } );
	elseif not LoginAcceptUserProtocolChkBtn:GetCheckState() then
		Login_MessageBoxOK( { ["text"] = "����ͬ���û�Э��", ["opDesc"] = "" } );
	else
		if os.clock() - nLastRequestTime < 6 then
			--LoginSelectServerBtnFont:SetText("����Ƶ����¼");
			return;
		end
		
		LoginSelectServerBtnFont:SetText("���ڵ�¼�С���");

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
	LoginSelectServerBtnFont:SetText("��ǰ��������"..SelectWorldInfo.world );

	nLastRequestTime = 0;
	if arg1 == "GE_UPDATE_LOGINRESULT" then
		if nLoginResult == g_nLOGIN_REP then
			Login_MessageBoxOK( { ["text"] = "���˺����ڱ𴦵�¼,�����ԣ�(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_SVRBUSY then
			Login_MessageBoxOK( { ["text"] = "��������æ�����Ժ��¼��(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_BADCLT then
			Login_MessageBoxOK( { ["text"] = "�ͻ��˰汾��ƥ�䣬����º��¼��(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_BAN then
			Login_MessageBoxOK( { ["text"] = "���˺ű���ֹ��¼������ϵ�ͷ���Ա��(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_FREQUENCY then
			Login_MessageBoxOK( { ["text"] = "���˺�Ƶ����¼��(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_NOSERVICE then
			Login_MessageBoxOK( { ["text"] = "������δ���ţ�(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_AGAIN_SAMEIP then
			Login_MessageBoxOK( { ["text"] = "�ϴε�¼�˺������˳���������!(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_QUEUE_FULL then
			Login_MessageBoxOK( { ["text"] = "��������Ա,���Ժ����ԣ�(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_ERR then
			Login_MessageBoxOK( { ["text"] = "�޷�������Ϸ��������(" .. nLoginResult ..")", ["opDesc"] = "", ["lock"] = true } );
			ClearFocus();
		elseif nLoginResult == g_nLOGIN_PRECREATEROLE then
			Login_MessageBoxOK( { ["text"] = "Ԥ������ɫ״̬����ʽ�������ע#Lwww.ljy0.com@@http://www.ljy0.com#n", ["opDesc"] = "", ["lock"] = true } );
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

	LoginSelectServerBtnFont:SetText("��ǰ��������"..SelectWorldInfo.world );

	local t_riches = 
	{	
		["LoginAcceptDescRich"] = 
		{	value = "���Ķ���ͬ��#L���û�Э�顷#n",
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

--ע���˺�
function LoginRegisterAccountBtn_OnClick()
	if RegisterUserFrame:IsShown() then
		RegisterUserFrame:Hide();
	else
		RegisterUserFrame:Show();
		RegisterUserFrame:SetClientUserData( 0,0 )
	end	
end

--�˺ų�ֵ
function LoginFillAccountBtn_OnClick()
	GameMgr:openWebSite( "http://pay.ljy0.com" )
end

--����
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
			{ AUTH_ERR_SYS,								"�������ѹر�"},
			{ AUTH_ERR_NET,								"������Ϸ������ʧ��"},
			{ AUTH_REASON_PASSWD_ERROR,					"�˺Ż����������"},
			{ AUTH_REASON_NO_ACCOUNT,					"��������˺Ų����ڣ���ע���˺�",		"�˺Ų�����"},
			{ AUTH_REASON_FREEZE,						"�˺��Ѷ��ᣬ����ϵ�ͷ�"},
			{ AUTH_IFM_ACCOUNT_ERROR,					"����ʱ,�û������Ϸ�"},
			{ AUTH_IFM_PASSWD_ERROR,					"����ʱ,���벻�Ϸ�"},
			{ AUTH_IFM_CERTID_ERROR,					"��������֤����ȷ"},		
			{ AUTH_IFM_AAS_ERROR,						"�û����ڷ������û�,�Ѿ������һ��������"},
			{ AUTH_IFM_NOUSE_SERVICE,					"�޷�ʹ�÷���"},
			{ AUTH_IFM_ACCOUNT_EXSIT,					"�˻��Ѿ�����"},
			{ AUTH_IFM_ACCOUNT_ACTIVATION,				"�˻��Ѿ�������Ϸ"},
			{ AUTH_IFM_ACCOUNT_REGISTER_ERROR,			"�û�ע��ʧ��"},
			{ AUTH_IFM_ACCOUNT_VALID_ERROR,				"��֤����ʧ��"},
			{ AUTH_IFM_GET_ACCOUNT_INFO_ERR,			"��ȡ�˻���ϸ��Ϣʧ��"},
			{ AUTH_IFM_GAME_REGION_ERROR,				"��Ϸ������Ų���ȷ"},
			{ AUTH_IFM_GAME_ACTIVE_ERROR,				"������Ϸ����ʧ��"},
			{ AUTH_IFM_USE_IDCARD_ERROR,				"�û����֤�Ѿ�����"},
			{ AUTH_IFM_USE_IDCARD_REGIST_TIMES_ERROR,	"���û�ע�����֤����50��"},
			{ AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR,		"���û�ע�����֤Ϊ�������û��������Ѵ���"},
			{ AUTH_IFM_USE_IDCARD_FORMAT_ERROR,			"��������֤��ʽ"},
			{ AUTH_IFM_NEED_REGIST_CODE_ERROR,			"�ڲ��ڼ䣬��Ҫ������"},
			{ AUTH_IFM_REGIST_CODE_ERROR,				"�����벻��ȷ���޷�������Ϸ"},
			{ AUTH_IFM_ACCOUNT_REGISTER_SUCCES,			"���������cdkey�Ѿ����͵��˺������У������"},
			{ AUTH_IFM_SYSTEM_BUSY,						"��������æ"},
			{ AUTH_REASON_FREEZE_TRYMAX,				"���볢�Դ������࣬�˺Ŷ���10����"},
			{ AUTH_REASON_FREEZE_CHECKTOOL,				"�˺���Ϊ�쳣���ѱ�����һ��ʱ��"},
		}
	PasswdCardFrame:Hide()
	nLastRequestTime = 0;
	LoginSelectServerBtnFont:SetText("��ǰ��������"..SelectWorldInfo.world );
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
	Login_MessageBoxOK( { ["text"] = "��¼ʧ��" .. "(" .. nRet .. ")", ["opDesc"] = "" } );
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
	--����ʾ��ʱ���ͷ�ռ�е�ui ��Դ
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

	-- �ü�ͼƬ
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

---------------------------ע���˺�---------------------------------

function RegisterUserFrameOKBtn_OnClick()
	local szAccount = RegisterUserFrameAccountEdit:GetText();
	if szAccount == nil or szAccount == "" then
		Login_MessageBoxOK( { ["text"] = "�������˺�", ["opDesc"] = "" } );
		return;
	end

	szAccount = string.lower( szAccount )
	if string.len(szAccount) < 6 then
		Login_MessageBoxOK( { ["text"] = "�˺ų��Ȳ�������6λ", ["opDesc"] = "" } );
		return
	end
	if string.len(szAccount) > 30 then
		Login_MessageBoxOK( { ["text"] = "�˺ų��Ȳ��ܳ���30λ", ["opDesc"] = "" } );
		return;
	end
	if string.find(szAccount, "[^%w@_/./-]") then
		Login_MessageBoxOK( { ["text"] = "�˺�����ֻ����Ӣ�Ļ����ּ����䳣���ַ����", ["opDesc"] = "" } );
		return;
	end
	
	local szPwd = RegisterUserFramePwdEdit:GetPassWord();
	if szPwd == nil or szPwd == "" then
		Login_MessageBoxOK( { ["text"] = "����������", ["opDesc"] = "" } );
		return
	end
	local szConfrimPwd = RegisterUserFrameConfirmPwdEdit:GetPassWord();
	if not (szPwd == szConfrimPwd) then
		Login_MessageBoxOK( { ["text"] = "������������벻ƥ��", ["opDesc"] = "" } );
		return;
	end
	if szPwd == szAccount then 
		Login_MessageBoxOK( { ["text"] = "���벻�����˺���ͬ", ["opDesc"] = "" } );
		return;
	end
	if string.len(szPwd) < 6 or string.len(szPwd) > 16 then
		Login_MessageBoxOK( { ["text"] = "���볤�ȱ�����6��16λ֮��", ["opDesc"] = "" } );
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
	RegisterUserFrameDescRich:SetText("����#cFF8700*#cA0A05AΪ�����",160,160,90);	
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
	ContinueRegisterUserFrameDescRich:SetText("����#cFF8700*#cA0A05AΪ�����",160,160,90);
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
		Login_MessageBoxOK( { ["text"] = "��������ȷ�����֤��", ["opDesc"] = "" } );
		return;
	end
	if not string.find( szCert,"x$" ) and not string.find( szCert,"X$" ) and not string.find( szCert,"%d$" ) then
		Login_MessageBoxOK( { ["text"] = "��������ȷ�����֤��", ["opDesc"] = "" } );
		return;
	end
	if not string.find( szCert,"^%d+%a?$" ) then
		Login_MessageBoxOK( { ["text"] = "��������ȷ�����֤��", ["opDesc"] = "" } );
		return;
	end
	local ss = string.sub( szCert,string.find( szCert,"^%d+%a?$" ) );
	if ss ~= szCert then
		Login_MessageBoxOK( { ["text"] = "��������ȷ�����֤��", ["opDesc"] = "" } );
		return;
	end

	if not age_detect(szCert) then
		Login_MessageBoxOK( { ["text"] = "��ǰ���֤��Ϣδ��18����", ["opDesc"] = "" } );
		return;
	end
	
	local szName = ContinueRegisterUserFrameNameEdit:GetText();
	if szName == nil or szName == "" then
		Login_MessageBoxOK( { ["text"] = "��������ʵ����", ["opDesc"] = "" } );
		return;
	end

	local szEmail = ContinueRegisterUserFrameMailEdit:GetText();
	if szEmail == nil or szEmail == "" then
		Login_MessageBoxOK( { ["text"] = "�����밲ȫ����", ["opDesc"] = "" } );
		return;
	end
	szEmail = string.lower( szEmail );
	local _,pos = string.find(szEmail, "^%w[%w_/./-]*@[%w_/./-]*%w$")
	if pos ~= string.len( szEmail ) then
		Login_MessageBoxOK( { ["text"] = "��������ȷ�������ַ", ["opDesc"] = "" } );
		return
	end

	LoginMgr:continueRegisterUserInfo( szCert, szName, szEmail );
	ContinueRegisterUserFrame:Hide();
end

--[[��ʾ�����׺������
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

-- #P�û�Э��#n
local szUserProtocolText = 
[[#P��������û�ʹ�����Э��#n

��Ҫ��֪��

�û���ʹ�ñ��������ظ������֮ǰ������ϸ�Ķ�����������û�ʹ�����Э��(�³Ʊ�Э��)�и������������ڴ��ر������û������Ķ���Э���ȫ������ر��������������������������ε�����õ�����ͨ�����С������κ����Ρ����������񡱵ȴʻ㣩�����������û�Ȩ��������õ�����ͨ�����С����á��ȴʻ㣩����Щ����Ӧ���й�����������ķ�Χ�����̶ȵ����á�

���û���ͬ�ⱾЭ�������벻Ҫ��װ��ʹ�ñ����������ط����û�һ����������ܡ�����װ�����������ע�ᡢ��ʼʹ�ü�/�����ʹ�ñ��������ط��񣬼���Ϊ�û��Ѿ����ܱ�Э����ȫ�����

�û���Ȩ��ʹ�ñ��������ط�����ڼ䣬�ල������Ƿ���������������ı�׼���û��ṩ����Ҳ������ʱ����������������˵Ĳ�Ʒ�ͷ����й�����ͽ��顣 

1�������˵��

��Э���У������෴˵�������´���������º��壺

1.1 �����������������Ŀͻ������ݼ����򣬲����ܰ�������������������վ����������������Ϸ�ٷ���վ��������ý�塢ӡˢ���Ϻ͡�������������ĵ��ȡ�

1.2 �û����ֳơ������û�����ָͨ��������ṩ�ĺϷ�;����ñ����ʹ����ɶ���װ��ʹ�ñ������Ʒ������ĸ��ˡ� 

1.3 ����ˣ�ָ���������������Ƽ����޹�˾��

2��֪ʶ��Ȩ����

2.1 ������������б������һ�кϷ�Ȩ��������������������Ȩ��֪ʶ��Ȩ�����������ȫ������Ȩ��һ������֪ʶ��Ȩ���Լ��뱾�����ص�������Ϣ���ݣ������������ڣ����ֱ���������ϡ�������ơ����Դ��롢���⡢��Ʒ����ɫ����ɫ����������ڡ��Ի����������á�����������ա���������Ч�����֡��Ӿ�Ч�������з�ʽ������Ȩ������ĵ������������ҽű���ӡˢ���ϡ������ĵ��Ⱦ����й�֪ʶ��Ȩ��ط��ɷ�����йع�����Լ�ı�����

2.2 ����������ֵġ�����ˡ��ͱ������صĲ�Ʒ�ͷ����ʶ��������������й���/����������/�������̱����ע���̱꣬�û�δ�������������Ȩ��������ʹ�á�

2.3 ��������漰���������ظ��������ӵ��ר��Ȩ��ר������Ȩ���̱�Ȩ������Ȩ������֪ʶ��Ȩ������˲�δ��Ϊ��Э�������Ϊ���û��ṩ������Լ���ط���������û��κ��뱾�����ص�֪ʶ��Ȩ����Э��δ��ȷ�����û���Ȩ����������˱�����

3����˽����

�����û�������˽������˵�һ��������ߣ����ԣ�����˲��ṫ�����޸Ļ�͸¶�û���ע�����ϼ�����������˸�������еķǹ������ݣ�����������ڳ��ŵĻ�������Ϊ͸¶��Щ��Ϣ�Ǳ�Ҫ�ģ��������������������Σ�

3.1 �����йط��ɷ���涨�������ڹ����йػ��ز�ѯʱ���ṩ�û�������˵���ҳ�Ϸ�������Ϣ���ݼ��䷢��ʱ�䡢��������ַ����������

3.2 ����ά������˵�֪ʶ��Ȩ��������ҪȨ����

3.3 �ڽ�������½���ά���û����˺������ڵ���˽��ȫ��

3.4 ����������û�Э�顢����˹����Ĺ����ƶȻ����������Ϊ��Ҫ����������¡�

4���������� 

4.1 �û���ŵ���ге���ʹ�ñ��������ط�����������κη��ա��������ض�Ŀ�ĺͲ�Υ���涨����ȷ���ʵ��ĵ������⣬����˲��������κ����͵ĵ�������������ȷ�Ļ������ģ�������˲�������

4.1.1 �������������ṩ����ط�������û���Ҫ��

4.1.2 �������������ṩ����ط����ܲ��ɿ�����������������ڿ͹�����ϵͳ���ȶ����û�����λ�á��û��ػ������Ų���ԭ�������κ����硢������ͨ����·��������Ϊ���ص�Ӱ�죻

4.1.3 ��װ�����ơ�������վ����ֵ�����б��������������ʽʹ�ñ������/�����������ṩ����ط������κ���������������κγ�ͻ��

4.1.4 ͨ���������վ����Ϸ�ٷ���վ��������������ϵ����Ӻͱ�ǩ��ָ��ĵ���������ҵ���������ṩ�������ʵ�ԡ���Ч�ԺͺϷ��ԡ�

5. �����������Э��ı������ֹ

5.1 Ϊ�˱�������˹�˾ҵ��չ�������ӵ�б������������������������������¡����ӡ����١�����ȣ�����ط����Լ���Э�����ݵ�Ȩ����Ҳӵ����ֹ�����/��ط���/��Э���Ȩ�������б�Ҫ������˻��Թ������ʽ�����ڱ����������վ��Ҫҳ���ϡ�����������û����ڲ����˽��йر������ֹ����Ϣ�� 

5.2 �û�ӵ������Ȩ������������ʱ������ֹʹ�ñ��������ط���ж�ر������������˶Ա��������ط����Э������������κα�������û���ͬ���йر�������ݣ��û���Ȩ����������ֹͣʹ�ñ�����Լ���ط���ж�ر���������û����й����ݱ�����Լ���ʹ�ñ��������ط��񣬼���Ϊ�û�ͬ������йر�����ݡ�

6��Ȩ��������

6.1 ���û�ͬ����ܱ�Э��ȫ�����ݵ�ǰ���£������ͬ�������û��ɳ����ġ��ɱ���ġ���ר�еġ�����ת�úͲ�������Ȩ�����Ȩ�����û����������Ч��ʱ���ڽ��������װ�ڸ���ʹ�õ�����������ϣ�����ָ���ķ�ʽ���б������һ�ݸ���������������ṩ�ķ�������˻��ڱ�Э�������û����������Ը���ʹ�õ���ɡ����û�����Ҫ�ڸ���ʹ�õķ�Χ����ʹ�ñ������������߽����������������κ���ҵ��;�����û�Ӧ���������ϵ��������������������Ȩ���κ�δ���������ɵİ�װ��ʹ�á����ʡ���ʾ�����е���Ϊ����ΪΥ����Э�顣

6.2 ���Ǳ�Э�����й涨������δ�����������ͬ�⣬�û�����ʵʩ������Ϊ��������Ӫ���Ļ��Ƿ�Ӫ���ģ���

6.2.1 ���ơ������������ͳ��б�����ĳ���ʹ���ֲ������ͼ���������ϵ�ȫ���򲿷����ݡ�

6.2.2 ����չʾ�Ͳ��ű������ȫ���򲿷����ݡ�

6.2.3 ���⡢���۱�����������ñ��������Ӫ����Ϊ��

6.2.4 �޸Ļ��ڸǱ��������ͼ�񡢶�������װ���ֲ�������ϵĲ�Ʒ���ơ���˾��־����Ȩ��Ϣ�����ݡ�

6.2.5 ����Υ������Ȩ������������������������ط��ɷ������Ϊ��

6.3 �û�Ӧͨ���Ϸ���;��ʹ�ñ��������ط��񣬲������������ֺ�����˻���������桢Υ����Э���������˷���������������߹����йط��ɷ���涨����Ϊ�������������ڣ�

6.3.1 �޸ġ����롢ע�͡�������ࡢ���ﱾ�����

6.3.2 ���򹤳̡����������߷���౾��������߲������������ֶζԱ�������з������޸ġ�������������

6.3.3 ʹ���κ���ҳ����������޸ĳ��򣨱�Э�����ơ���ҳ�����ָ�����ڱ����֮��ģ��ܹ��ڱ�������е�ͬʱӰ�챾������������г��򣬰�����������ģ��������������ı�����������޸����ݵ�һ�����͡�������йط��ɡ����漰�������ܲ��ŵĹ��»�淶���ļ��涨����Ҷ����뱾Э���г�ͻ�����Է��ɡ����桢���Ź��»�淶���ļ��涨��Ϊ׼�����Ա�������л�ԭ���̡����롢������޸ģ��������������޸ı������ʹ�õ��κ�ר��ͨѶЭ�顢�Զ�̬�����ȡ�ڴ棨RAM�������Ͻ����޸Ļ�������

6.3.4 ʹ���쳣�ķ�����¼�������ʹ�������������������������˳�ʽ�ȶ����ƻ������������ʩ�����ұ�������������������Ϊ��

6.3.5 ͨ���쳣���߷Ƿ���ʽʹ�ñ���������������������ñ������¼�Ǳ�����ٷ����������������ƻ�����������ұ���������ķ����������ʵʩ������������Ϊ��

6.3.6 ������������ʹ����ҡ��������������������������׳��򣬻���֯����������ʹ�ô���������򣬻����۴�����������Ϊ���˻���֯ıȡ�������棻

6.3.7 ʹ���κη�ʽ�򷽷�����ͼ�����ṩ������������ط�������·�������������Լ������豸���Դﵽ�Ƿ���û��޸�δ����Ȩ���������ϡ�Ӱ�챾������������з����Լ�����Σ����Ŀ�ĵ��κ���Ϊ��

6.3.8 ���ñ������������ϵͳ���ܴ��ڵļ���ȱ�ݻ�©�����Ը�����ʽΪ�Լ�������Ĳ���������������ڸ��Ʊ�����е�������Ʒ�ȣ����ߴ���������������Ϊ��

6.3.9 Υ�������йط��ɷ���Ĺ涨�����ñ�������������ơ������ʹ�����Ϣ��

6.4 һ�������ͨ���ڲ��ļ������ֻ������û��ٱ��������û��п������ڴ���������Ϊ�����������Ȩ�����������жϲ���ȡ��Ӧ�Ĵ�ʩ���ô�ʩ�����������������û��˺ŵĵ�¼�������û��ڱ�������й����еĻ����ֹ�������Ȩ��ɾ���븴���йص���Ʒ���������Ƴ���������Ʒ�Ͳ��븴�Ƶ�������Ʒ����ɾ���û����˺Ų�Ҫ���û��⳥�����������Ϊ�����������ɵ���ʧ�ȡ�

6.5 ������ṩ��ʹ�ñ�������������Լ�������ϵͳͨ�����ʻ�������Ϊ�û��ṩ����ͬʱ���û�Ӧ�����ṩ���µ��豸����Ϣ��

6.5.1 �����䱸�����������豸�� �������˵��ԡ����ƽ�����������ر�����װ�á�

6.5.2 ���и�������������֧������˷����йصĵ绰���á�������á�

6.5.3 ����������ṩ�������Ҫ�ԣ��û�ͬ�⣺
��1���ṩ�꾡��׼ȷ�ĸ������ϡ�
��2�����ϸ���ע�����ϣ����ϼ�ʱ���꾡��׼ȷ��Ҫ��
��3���μ��û���д��ע�����ϡ���ʷ��Ϣ��

6.6 �������Ϊ�û��ṩ��ؿͻ������ǰ�����û��ܱ����û����˺ŵ������ˣ��������Ҫ�û��ṩ�����Ϣ��������������ע����Ϣ����ʷ����ȣ����û���⣬����û������ṩ׼ȷ������ע�����ϼ������ʷ��Ϣ��δ��ʱ�������ע�����ϻ�����ص�֤�ݣ����п��ܵ���������޷��ж��û�����ݣ��Ӷ��޷�Ϊ�ṩ�����һص���ط��񣬶�����˶Դ˲��е��κ����Ρ������û��ύ�������Ϣ���ϲ���ʵ����׼ȷ�������������Ϸ��Ӷ��������������������жϵģ��������Ȩ��ֹΪ�û��ṩ����׷���û��ķ������Ρ�

6.7 ����˱���ͨ�����������صķ������û�Ͷ����ҵ�Թ���Ȩ����

7��ΥԼ����

7.1 �û�ͬ�Ᵽ�Ϻ�ά������˼������û������棬�����û�Υ���йط��ɡ������Э�����µ��κ�����������������𺦣��û�ͬ��е��ɴ���ɵ����⳥���Σ��õ����ΰ����������ڸ��������ɵ��κ�ֱ�ӻ�����ʧ��

7.2 ���û�Υ����Э�顢����˷���������������߹��ҷ��ɷ���涨�����������Ȩ�����������жϣ��������������ɣ���ֹΪ�û��ṩ����ͨ�����ֺϷ�;��׷���û��ķ������Ρ�

8���������ú�������

8.1 ��Э��Ķ��������С����ͼ�����Ľ����Ӧ�����л����񹲺͹����ɲ��ų�����һ�г�ͻ�������á�

8.2 ��˫���ͱ�Э�����ݻ���ִ�з����κ����飬˫��Ӧ�����Ѻ�Э�̽����Э�̲���ʱ���κ�һ����Ӧ����������ɽ������Ժ�������ϡ�

9�������涨

9.1 ��Э�鹹��˫�����ڱ�������ʹ�����������Э�顣

9.2 �籾Э���е��κ��������κ�ԭ���ж�Ϊ��ȫ�򲿷���Ч�򲻾���ִ�����ģ�����Ч�򲻾���ִ�������������ӽ�ԭ������ͼ��һ����Ч����ִ�е���������������ұ�Э�������������Ӧ��Ч������ִ������

9.3 ����˲���ʹ��δ�ܼ�ʱ��ʹ����δ�����ʹ��Э����߰��շ��ɹ涨�����е�Ȩ������Ӧ����Ϊ������Ȩ����Ҳ��Ӱ��������ڽ�����ʹ��Ȩ����

9.4 �ڷ�����������Χ�ڣ�����˱����Ա�Э������ս���Ȩ�� ]];

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
	-- ��ʾ��������
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
	-- ����
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
			{ AUTH_ERR_SYS,								"�������ѹر�"},
			{ AUTH_ERR_NET,								"������Ϸ������ʧ��"},
			{ AUTH_REASON_PASSWD_ERROR,					"�˺Ż����������"},
			{ AUTH_REASON_NO_ACCOUNT,					"�˺Ż����������"},
			{ AUTH_REASON_FREEZE,						"�˺�δ����ô���"},
			{ AUTH_IFM_ACCOUNT_ERROR,					"����ʱ,�û������Ϸ�"},
			{ AUTH_IFM_PASSWD_ERROR,					"����ʱ,���벻�Ϸ�"},
			{ AUTH_IFM_CERTID_ERROR,					"��������֤����ȷ"},		
			{ AUTH_IFM_AAS_ERROR,						"�û����ڷ������û�,�Ѿ������һ��������"},
			{ AUTH_IFM_NOUSE_SERVICE,					"�޷�ʹ�÷���"},
			{ AUTH_IFM_ACCOUNT_EXSIT,					"�˻��Ѿ�����"},
			{ AUTH_IFM_ACCOUNT_ACTIVATION,				"�˻��Ѿ�������Ϸ"},
			{ AUTH_IFM_ACCOUNT_REGISTER_ERROR,			"�û�ע��ʧ��"},
			{ AUTH_IFM_ACCOUNT_VALID_ERROR,				"��֤����ʧ��"},
			{ AUTH_IFM_GET_ACCOUNT_INFO_ERR,			"��ȡ�˻���ϸ��Ϣʧ��"},
			{ AUTH_IFM_GAME_REGION_ERROR,				"��Ϸ������Ų���ȷ"},
			{ AUTH_IFM_GAME_ACTIVE_ERROR,				"������Ϸ����ʧ��"},
			{ AUTH_IFM_USE_IDCARD_ERROR,				"�û����֤�Ѿ�����"},
			{ AUTH_IFM_USE_IDCARD_REGIST_TIMES_ERROR,	"���û�ע�����֤����50��"},
			{ AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR,		"���û�ע�����֤Ϊ�������û��������Ѵ���"},
			{ AUTH_IFM_USE_IDCARD_FORMAT_ERROR,			"��������֤��ʽ"},
			{ AUTH_IFM_NEED_REGIST_CODE_ERROR,			"�ڲ��ڼ䣬��Ҫ������"},
			{ AUTH_IFM_REGIST_CODE_ERROR,				"�����벻��ȷ���޷�������Ϸ"},
			{ AUTH_IFM_SYSTEM_BUSY,						"��������æ"},
			{ AUTH_REASON_FREEZE_TRYMAX,				"���볢�Դ������࣬�˺Ŷ���6Сʱ"},
			{ AUTH_REASON_FREEZE_CHECKTOOL,				"�˺���Ϊ�쳣���ѱ�����һ��ʱ��"},
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
			Login_MessageBoxOK("","���������cdkey�Ѿ����͵��˺������У������");
		end
		]]--
	end
end

----------------------------------------�ֻ���----------------------------------------------
local LastRequestTime = 0;
local LastCode = 0;
local t_resultDesc = { 
			[0] = { "��֤�ɹ�", 1 },
			[1] = { "��֤�����", 0 },
			[2] = { "������֤�����", 0 },
			[3] = { "û�д�����ɾ���˽�ɫ", 1 },
			[4] = { "�������ʧ��", 1 },
			[5] = { "����ظ���ȡ", 1 },
			[6] = { "δ�����ֻ���", 0 },
			[7] = { "����Ϣ����ʧ��", 0 },
			[8] = { "����Ϣ�����ȡ̫Ƶ��", 0 },
			};

function PrebuildMobileCheckFrameRequestBtn_OnUpdate()
	if LastRequestTime > 0 and this:GetText() ~= "��ȡ��֤��" then
		local num = PRECREATE_GIFT_VERIFY_TIMEOUT_TOLUA - math.floor( os.clock() - LastRequestTime );
		if num > 0 then
			this:SetText( num .. "��" );
		else
			this:SetText( "��ȡ��֤��" );
			this:Enable();
			LastRequestTime = 0;
		end
	end
end

function PrebuildMobileCheckFrameRequestBtn_OnClick()
	local phone = PrebuildMobileCheckFrameMobileShowBtnFont:GetText();
	if tonumber( phone ) == nil or string.len( phone ) < 8 then
		return Login_MessageBoxOK( { ["text"] = "��������ȷ���ֻ�����", ["opDesc"] = "" } );
	end
	if LastRequestTime == 0 or os.clock() - LastRequestTime > 1 then
		LastRequestTime = os.clock();
		LoginMgr:requestGetSMS( phone );
	end
end

function PrebuildMobileCheckFrame_OnShow()
	PrebuildMobileCheckFrameRequestBtn:SetText( "��ȡ��֤��" );
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
	local szText = tonumber( edit:GetText() ) ~= nil and edit:GetText() or "�����д�ֻ�����";
	PrebuildMobileCheckFrameMobileShowBtnFont:SetText( szText );
	PrebuildMobileCheckFrameMobileShowBtn:Show();
end

function PrebuildMobileCheckFrameAcceptBtn_OnClick()
	if SelectRoleWarnFrame:IsShown() then return end;
	if LastCode == 0 then
		--������֤����
		--return Login_MessageBoxOK( { ["text"] = "���Ȼ�ȡ��֤��", ["opDesc"] = "" } );
	end
	local phone = PrebuildMobileCheckFrameMobileShowBtnFont:GetText();
	if tonumber( phone ) == nil or string.len( phone ) < 8 then
		return Login_MessageBoxOK( { ["text"] = "��������ȷ���ֻ�����", ["opDesc"] = "" } );
	end
	local code = PrebuildMobileCheckFrameCodeEdit:GetText();
	if tonumber( code ) == nil or string.len( code ) < 6 then
		--return Login_MessageBoxOK( { ["text"] = "��������ȷ����֤��", ["opDesc"] = "" } );
	end
	--�ֻ���֤��ȡ��������Ҫ�ȷ����»�ȡ��֤�����Ϣ����֪ͨ�������ֻ���
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
		PrebuildMobileCheckFrameCodeNum:SetText( "��֤����ţ�" .. num );
		LastRequestTime = os.clock();
		PrebuildMobileCheckFrameRequestBtn:SetText( PRECREATE_GIFT_VERIFY_TIMEOUT_TOLUA .. "��" );
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