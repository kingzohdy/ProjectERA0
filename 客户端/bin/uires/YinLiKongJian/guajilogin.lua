nSelectRoleindex	=1;
oldstate		=1;
CanRoleEnter		= true;--防止服务器未返回结果时再次点击
RolelistIsGet		= false;
prelogin		= true;
DIR_NOT_WORK		= 0; 	--/* 维护状态 */
DIR_EXCELLENCE		= 1; 	--/* 很好 */
DIR_NORMAL		= 2; 	--/* 正常 */
DIR_BUSY		= 3; 	--/* 繁忙 */
DIR_FULL		= 4; 	--/* 爆满 */

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
 g_nLOGIN_ERR       = 16; 	 --/* account or role login fail because other reason */
function GuajiloginFrameOnLoad()
	this:RegisterEvent("GE_LOGIN_DIR");
	this:RegisterEvent("GE_KICK_OFF");
	this:RegisterEvent("GE_LOGIN_RESULT");
	this:RegisterEvent("GE_UPDATE_DIR");
	this:RegisterEvent("GE_DIR_ERR");
	this:RegisterEvent("GE_OFFLINE");
	BigWorldSelectNameBtn1:SetPoint( "topleft", "BigWorldSelectNameFrame", "topleft", 2, 0);
	WorldSelectNameBtn1:SetPoint( "topleft", "WorldSelectNameFrame", "topleft", 2, 0);
	for i=2,9 do
		local jbutton=getglobal("BigWorldSelectNameBtn"..i);
		jbutton:SetPoint( "topleft", "BigWorldSelectNameBtn"..i-1, "bottomleft", 0, 0);
		jbutton=getglobal("WorldSelectNameBtn"..i);
		jbutton:SetPoint( "topleft", "WorldSelectNameBtn"..i-1, "bottomleft", 0, 0);
		
	end
	if OffMgr:getlastusercheck() == 1 then
		LoginSaveUserNameChkBtn:setCheckState(true);
		LoginNameEdit:SetText(OffMgr:getlastname());
	else
		LoginSaveUserNameChkBtn:setCheckState(false);
	end
end

function GuajiloginFrameOnEvent()
	if arg1 == "GE_LOGIN_DIR"  then
		GetDirname();
	elseif arg1 == "GE_KICK_OFF"  and this:IsShown() then
		selectman:Hide();
		MainPlayer:setGameState(LOGIN_STATE);--选择角色的时候被T下来时状态重置]
	elseif arg1 == "GE_LOGIN_RESULT" then
		local nLoginResult = MudMgr:getLoginResult();
		if nLoginResult == g_nLOGIN_REP then
			 AddMessage( "该账号已在别处登录,请重试！ ",1);
		elseif nLoginResult == g_nLOGIN_SVRBUSY then
			 AddMessage( "服务器繁忙，请稍后登录！ ",1);
	        elseif nLoginResult == g_nLOGIN_BADCLT then
			 AddMessage( "客户端版本不匹配，请更新后登录！ ",1);
		elseif nLoginResult == g_nLOGIN_BAN then
			 AddMessage( "该账号被禁止登录 ",1);
		elseif nLoginResult == g_nLOGIN_FREQUENCY then
			 AddMessage( "该账号频繁登录！ ",1);
		elseif nLoginResult == g_nLOGIN_NOSERVICE then
			 AddMessage( "服务器未开放！ ",1);
		elseif nLoginResult == g_nLOGIN_AGAIN_SAMEIP then
			 AddMessage( "登录失败，请重试！ ",1);
		elseif nLoginResult == g_nLOGIN_QUEUE_FULL then
			 AddMessage( "服务器满员,请稍后再试！ ",1);
		 elseif nLoginResult == g_nLOGIN_ERR then
			 AddMessage( "登录失败！ ",1);
		 end
	elseif arg1 == "GE_UPDATE_DIR" then
		Set_WorldName();
	elseif arg1 == "GE_DIR_ERR" then
		 AddMessage( "服务器连接失败!",1);
	elseif arg1 == "GE_OFFLINE" then
		selectman:Hide();
		guajiloginSelectRoomFrame:Hide();
		if guajiFrame:IsShown() then
			guajiFrame:Hide();
			guajiloginFrame:Show();
			OffMgr:setsize(0);
			AllRichtextClear();
		else
			guajiloginFrame:Show();
			AllRichtextClear();
		end
		MainPlayer:setGameState(LOGIN_STATE);
		AddMessage("网络连接断开",1);
	end
end



function CFloginBtn_OnClick()
	--guajiFrame:Show();
	--OffMgr:setsize();
	if MudMgr:getSelectWorldState()  == -1 then
		AddMessage("请选择服务器！",1);
		return;
	elseif MudMgr:getSelectWorldState() == DIR_NOT_WORK then
		AddMessage("当前服务器处于维护状态！",1);
		return;
	end
	local username=LoginNameEdit:GetText();
	local userpossword=PassWordEdit:GetPassWord();
	local ip=MudMgr:getWorldIp();
	local port=MudMgr:getWorldPort();
	local worldid=MudMgr:getWorldId();
	local worldname=MudMgr:getSelectWorldName();
	if LoginSaveUserNameChkBtn:GetCheckState() then
		OffMgr:saveusername(username);
	else
		OffMgr:clearusername();
	end
	MudMgr:requestLoginGameServer(ip,port,username,userpossword,worldid,worldname);
	--guajiloginFrame:Hide();
	--guajiloginSelectRoomFrame:Show();
end

function GuajiloginFrameOnEnterpressed()
	CFloginBtn_OnClick();
end

function GuajiGetFirstRecommendWorld()
	local nAreaIndex = 0;
	local nWorldIndex = 0;
	for i = 1, MudMgr:getBigWorldNum() do
		local bFound = false;
		local bigWorldInfo = MudMgr:getBidWorldInfo( i - 1 );
		for j = 1, MudMgr:getWorldNum( i - 1 ) do
			local oneWorldInfo = bigWorldInfo:getIthWorld( j - 1 );	
			if oneWorldInfo.recommend then
				nWorldIndex = j - 1;
				bFound = true;
				break;
			end
		end

		if bFound then
			nAreaIndex = i - 1;
			break;
		end
	end
	return nAreaIndex, nWorldIndex;
end

function GetDirname()
	local nums=MudMgr:getSevernums();
	for i=1,nums do
		local button =  getglobal( "BigWorldSelectNameBtn" .. i );
		local font   = 	getglobal( "BigWorldSelectNameBtn" .. i .. "AreaName" );
		button:Enable();
		local str = MudMgr:getSeverName(i);
		font:SetText( str );
	end
	for i=nums+1,9 do
		local button =  getglobal( "BigWorldSelectNameBtn" .. i );
		local font   = 	getglobal( "BigWorldSelectNameBtn" .. i .. "AreaName" );
		button:Disable();
		font:SetText( "" );
	end
	--读取上一次的游戏世界选择记录
	if  prelogin  then
		local index = 0;
		local nAreaIndex, nWorldIndex = GuajiGetFirstRecommendWorld();
		if MudMgr:getLastSelectArea() == 0 then
			index = nAreaIndex + 1;
		else
			index = MudMgr:getLastSelectArea() + 1;
		end
		local button = getglobal( "BigWorldSelectNameBtn"..index);
		if button:IsEnable() then
			local font = getglobal( "BigWorldSelectNameBtn"..index.. "AreaName" );
			local str=   font:GetText()
			ServerFrame_SelectBtnFont:SetText(str);
			MudMgr:setBigworldindex(str);
			Set_WorldName();
			if MudMgr:getLastSelectWorld() == 0 then
				index = nWorldIndex + 1
			else
				index = MudMgr:getLastSelectWorld() + 1 ;
			end
			local font = getglobal( "WorldSelectNameBtn"..index .. "AreaName" );
			local str=   font:GetText()
			ServerWorldFrame_SelectBtnFont:SetText(str);
			MudMgr:setWorldindex(str);
			prelogin = false;
		end
	end
end

function ServerFrame_SelectDownBtn_OnClick()
	if BigWorldSelectNameFrame:IsShown() then
		BigWorldSelectNameFrame:Hide();
	else
		BigWorldSelectNameFrame:SetPoint( "topleft", "ServerFrame_SelectBtn", "bottomleft", 0, 12 );
		BigWorldSelectNameFrame:Show();
	end
end

function ServerFrame_SelectDownBtn_OnLostFoucs()
	BigWorldSelectNameFrame:Hide();
end

function ServerWorldFrame_SelectDownBtn_OnClick()
	if WorldSelectNameFrame:IsShown() then
		WorldSelectNameFrame:Hide();
	else
		WorldSelectNameFrame:SetPoint( "topleft", "ServerWorldFrame_SelectBtn", "bottomleft", 0, 12 );
		WorldSelectNameFrame:Show();
	end
end

function ServerWorldFrame_SelectDownBtn_OnLostFoucs()
	WorldSelectNameFrame:Hide();
end

function Set_WorldName()
	local index=MudMgr:getBigWorldindex();
	local nums=MudMgr:getWorldNum(index);
	if nums == -1 then 
	 return;
	 end
	for i=1,nums do
		local button =  getglobal( "WorldSelectNameBtn" .. i );
		local font   = 	getglobal( "WorldSelectNameBtn" .. i .. "AreaName" );
		button:Enable();
		local str = MudMgr:getWorldName(index,i);
		font:SetText( str );
		local nWorldState =  MudMgr:getWorldState(index,i);
		if nWorldState == DIR_NOT_WORK then
			font:SetTextColor( 180, 180, 180);
			button:SetClientString("维护");
		elseif nWorldState == DIR_EXCELLENCE then
			font:SetTextColor( 0, 255, 0);
			button:SetClientString("流畅");
		elseif nWorldState == DIR_NORMAL then
			font:SetTextColor( 255, 150, 0);
			button:SetClientString("一般");
		elseif nWorldState == DIR_BUSY then
			font:SetTextColor( 210, 65, 255);
			button:SetClientString("繁忙");
		else
			font:SetTextColor( 255, 0, 0);
			button:SetClientString("爆满");
		end
	end
	for i=nums+1,9 do
		local button =  getglobal( "WorldSelectNameBtn" .. i );
		local font   = 	getglobal( "WorldSelectNameBtn" .. i .. "AreaName" );
		button:Disable();
		font:SetText( "" );
	end
end

function WorldSelectBtn_OnClick()
	local id=this:GetClientID();
	if id <= 10 then
		local font = getglobal( this:GetName() .. "AreaName" );
		local str=   font:GetText()
		ServerFrame_SelectBtnFont:SetText(str);
		MudMgr:setBigworldindex(str);
		Set_WorldName();
		local str=  WorldSelectNameBtn1AreaName:GetText()
		ServerWorldFrame_SelectBtnFont:SetText(str);
		MudMgr:setWorldindex(str);
		BigWorldSelectNameFrame:Hide();
	else
		local font = getglobal( this:GetName() .. "AreaName" );
		local str=   font:GetText()
		ServerWorldFrame_SelectBtnFont:SetText(str);
		MudMgr:setWorldindex(str);
		WorldSelectNameFrame:Hide();
	end
end

function WorldSelectBtn_OnEnter()
	if this:GetClientString() ~= "" then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString() , 
								frame = this:GetParent(), button = "cursor" } );
	end
end

function WorldSelectBtn_OnLeave()
	if this:GetClientString() ~= "" then
		GameTooltipOnLeave();
	end
end
function GuajiloginFrameOnShow()
	MainPlayer:setGameState(LOGIN_STATE);
	if ResponseAddFriendFrame:IsShown() then
		ResponseAddFriendFrame:Hide();
	end
	if guajiMessagebox:IsShown() then
		guajiMessagebox:SetPoint("topleft","UIClient","topleft",25,200);
	end
	MudMgr:ReFreshServer();
	MudMgr:ResetRfreshTime();
end

function TuodongOnMouseDonw()
	if arg1 ~= "LeftButton" then
		return
	end	
	OffMgr:MoveWindowDown();	
end

function TuodongOnMouseUp()
	if arg1 ~= "LeftButton" then
		return
	end
	OffMgr:MoveWindowUp();
end

function SelectManFrameOnLoad()
	local	jbutton=getglobal( this:GetName() .. "jiaose"..1);
	jbutton:SetPoint( "topleft", this:GetName(), "topleft", 15, 50 );
	 jbutton:Disable();
	for  i=2,4 do
	   jbutton=getglobal( this:GetName() .. "jiaose"..i);
	   local str=this:GetName() .. "jiaose"..i-1;
	   jbutton:SetPoint( "topleft", str, "bottomleft", 0, 0 );
	    jbutton:Disable();
	end
	this:RegisterEvent("GE_ROLELIST_DIR");
end

function IdToname(i)
    if  i == 1 then 
     return "战士"
    elseif i == 2 then
     return "法师"
    elseif i==3 then
     return "枪手"
    elseif i == 4 then
    return "祭司"
    elseif i == 17 then
    return "守护者"
    elseif i == 33 then
    return "征服者"
    elseif i == 18 then
    return "聚能者"
    elseif i == 34 then
    return "炼魂师"
    elseif i == 19 then
    return "猎杀者"
    elseif i == 35 then
    return "毁灭者"
    elseif i == 20 then
    return "圣佑者"
    elseif i == 36 then
    return "驭雷师"
    end
    return "未知";
end

function rolelistclear()
	for i=1,4 do
	   local  str="selectmanjiaose"..i;
	   local  jbutton=getglobal(str);
	   jbutton:Disable();
	   jbutton=getglobal(str.."yuananniu");
	   jbutton:DisChecked();
	   jbutton:Hide();
	   local font=getglobal(str.."jiaoseFont");
	   font:SetText("");
	   font=getglobal(str.."zhiyeFont");
	   font:SetText("");
	   font=getglobal(str.."dengjiFont");
	   font:SetText("");
	end
end

function Updateroleinfo(j)
	local num = MainPlayer:getRoleListNums();
	if num > 4 then
		num = 4;
	end
	for i=1,num do
	   local  str="selectmanjiaose"..i;
	   local  jbutton=getglobal(str);
	   jbutton:Enable();
	   jbutton=getglobal(str.."yuananniu");
	   jbutton:Show();
	   local font=getglobal(str.."jiaoseFont");
	   local str1=MainPlayer:getRoleName(i+j);
	   font:SetText(str1);
	   font=getglobal(str.."zhiyeFont");
	   local nums1=MainPlayer:getRoleCareer(i+j);
	   str1=IdToname(nums1);
	   font:SetText( str1);
	   font=getglobal(str.."dengjiFont");
	   str1=MainPlayer:getRoleLevel(i+j);
	   font:SetText(""..str1);
	end 
end

function dealrolelist()
	rolelistclear();
	local num=MainPlayer:getRoleListNums();
	local du=0;
	if num > 4 then 
		num=4;
		selectmanSlider:Show();
		local nums=MainPlayer:getRoleListNums();
		selectmanSliderTexture:SetSize(14,57*4/nums);
		selectmanSlider:SetMinValue(0);
		selectmanSlider:SetMaxValue((nums - 4)*5);
		du=selectmanSlider:GetValue()/5;
	else
		selectmanSlider:Hide();
	end
	Updateroleinfo(du);
	selectman:Show();
end

function SelectManFrameOnEvent()
	if ( arg1 == "GE_ROLELIST_DIR") then
		dealrolelist();
		--为false即表示第一次登录，为true即表示角色的切换
		if not RolelistIsGet then
			RolelistIsGet = true;
			selectmanSlider:SetValue(0);
		else
			RolelistIsGet = false;
			MainPlayer:requestEnterGameMudWorld();
		end
	end
end

function SelectActorFrame_OnClick()
	local index=selectmanSlider:GetValue()/5;
	local  jbutton=getglobal(this:GetName().."yuananniu");
	if jbutton:IsChecked()  then
		if CanRoleEnter then
			if oldstate ~= LOGIN_STATE then
				if	not MainPlayer:IsSamePlayer(index + this:GetClientID()) then 
					MainPlayer:requestRoleLogout();
					AllRichtextClear();
				else
					selectman:Hide();
					return;
				end
			end
			CanRoleEnter = false;
			nSelectRoleindex = this:GetClientID();
			MainPlayer:setSelectRoleIndex(this:GetClientID() + index );
			if not RolelistIsGet then
				RolelistIsGet = true;
			else
				RolelistIsGet = false;
				OffMgr:SendUIEvent("GE_ENTERCHARROOM_LIST");
			end
		else
			AddMessage("正在等待服务器回复，请稍等！",0);
		end

	end
	SelectActorAllDischecked();
	jbutton:Checked();
end

function SelectActorFrame_OnEnter()
	local tex=getglobal(this:GetName().."HighLightTex");
	tex:Show();
end

function SelectActorFrame_OnLeave()
	local tex=getglobal(this:GetName().."HighLightTex");
	tex:Hide();
end

function SelectActorAllDischecked()
	for i=1,4 do
	local  jbutton=getglobal("selectmanjiaose"..i.."yuananniu");
	jbutton:DisChecked();
	end
end

function selectmanSurebtn_OnClick()
	local index=selectmanSlider:GetValue()/5;
	for i=1,4 do
		local  jbutton=getglobal("selectmanjiaose"..i.."yuananniu");
		if jbutton:IsChecked() then
			if CanRoleEnter then
				if oldstate ~= LOGIN_STATE then
					if	not MainPlayer:IsSamePlayer(index + i) then
						MainPlayer:requestRoleLogout();
						AllRichtextClear();
					else
						selectman:Hide();
						return;
					end
				end
				CanRoleEnter = false;
				nSelectRoleindex = i;
				MainPlayer:setSelectRoleIndex(i + index);
				if not RolelistIsGet then
					RolelistIsGet = true;
				else
					RolelistIsGet = false;
					OffMgr:SendUIEvent("GE_ENTERCHARROOM_LIST");
				end
				return;
			else
				AddMessage("正在等待服务器回复，请稍等！",0);
				return ;
			end
		end
	end	
	AddMessage("请选择要登录的角色!",0);
end

function selectman_Close()
	selectman:Hide();
	MainPlayer:setGameState(oldstate);
	if oldstate == LOGIN_STATE then
		MudMgr:requestLogout();
	end
end

function selectman_OnHide()
	CanRoleEnter = true;
	RolelistIsGet = false;
end
--用old保存之前的状态独立选人界面状态。。。
function SelectManFrameOnShow()
	oldstate=MainPlayer:getGameState();
	MainPlayer:setGameState(ROLELIST_STATE);
	--默认选择角色
	SelectActorAllDischecked();
	local index=selectmanSlider:GetValue()/5;
	nSelectRoleindex = MainPlayer:getSelectRoleIndex() - index;
	if nSelectRoleindex >= 1 and nSelectRoleindex <= 4 then
		local  jbutton=getglobal("selectmanjiaose"..nSelectRoleindex.."yuananniu");
		jbutton:Checked();
	end
end
function WebBtn_OnClick()
	OffMgr:OpenUrl("http://www.ljy0.com");
end

function LoginError(nRet)
		local t_ErrorInfo =
		{
			{ AUTH_ERR_SYS,								"网络连接错误"},
			{ AUTH_REASON_PASSWD_ERROR,					"账号或者密码错误"},
			{ AUTH_REASON_NO_ACCOUNT,					"账号或者密码错误"},
			{ AUTH_REASON_FREEZE,						"您的账号没有获得体验服资格"},
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
			{ AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR,		"防沉迷用户注册身份证已存在"},
			{ AUTH_IFM_USE_IDCARD_FORMAT_ERROR,			"错误的身份证格式"},
			{ AUTH_IFM_NEED_REGIST_CODE_ERROR,			"内测期间，需要激活码"},
			{ AUTH_IFM_REGIST_CODE_ERROR,				"激活码不正确，无法激活游戏"},
			{ AUTH_IFM_SYSTEM_BUSY,						"系统繁忙"}
		}

	for i = 1, table.getn( t_ErrorInfo ) do
		if nRet == t_ErrorInfo[i][1] then
			AddMessage(t_ErrorInfo[i][2],1);
			return;
		end
	end
end

function LoginNameEdit_OnEnterPressed()
	SetFocusFrame("PassWordEdit");
end

function LoginPasswordEdit_OnEnterPressed()
	SetFocusFrame("guajiloginFrame");
end

function LoginUpdateServerBtn_OnClick()
	MudMgr:ReFreshServer();
end

function SelectmanFrameUpBtn_OnClick()
	if selectmanSlider:IsShown() then
		local pos=selectmanSlider:GetValue() - 5 ;
		selectmanSlider:SetValue(pos);
	end
end

function SelectmanFrameDownBtn_OnClick()
	if selectmanSlider:IsShown() then
		local pos=selectmanSlider:GetValue() + 5 ;
		selectmanSlider:SetValue(pos);
	end
end


function SelectmanFrame_ScrollBarValueChange()
	local index=selectmanSlider:GetValue()/5;
	Updateroleinfo(index);
end