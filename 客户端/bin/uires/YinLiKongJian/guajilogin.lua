nSelectRoleindex	=1;
oldstate		=1;
CanRoleEnter		= true;--��ֹ������δ���ؽ��ʱ�ٴε��
RolelistIsGet		= false;
prelogin		= true;
DIR_NOT_WORK		= 0; 	--/* ά��״̬ */
DIR_EXCELLENCE		= 1; 	--/* �ܺ� */
DIR_NORMAL		= 2; 	--/* ���� */
DIR_BUSY		= 3; 	--/* ��æ */
DIR_FULL		= 4; 	--/* ���� */

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
		MainPlayer:setGameState(LOGIN_STATE);--ѡ���ɫ��ʱ��T����ʱ״̬����]
	elseif arg1 == "GE_LOGIN_RESULT" then
		local nLoginResult = MudMgr:getLoginResult();
		if nLoginResult == g_nLOGIN_REP then
			 AddMessage( "���˺����ڱ𴦵�¼,�����ԣ� ",1);
		elseif nLoginResult == g_nLOGIN_SVRBUSY then
			 AddMessage( "��������æ�����Ժ��¼�� ",1);
	        elseif nLoginResult == g_nLOGIN_BADCLT then
			 AddMessage( "�ͻ��˰汾��ƥ�䣬����º��¼�� ",1);
		elseif nLoginResult == g_nLOGIN_BAN then
			 AddMessage( "���˺ű���ֹ��¼ ",1);
		elseif nLoginResult == g_nLOGIN_FREQUENCY then
			 AddMessage( "���˺�Ƶ����¼�� ",1);
		elseif nLoginResult == g_nLOGIN_NOSERVICE then
			 AddMessage( "������δ���ţ� ",1);
		elseif nLoginResult == g_nLOGIN_AGAIN_SAMEIP then
			 AddMessage( "��¼ʧ�ܣ������ԣ� ",1);
		elseif nLoginResult == g_nLOGIN_QUEUE_FULL then
			 AddMessage( "��������Ա,���Ժ����ԣ� ",1);
		 elseif nLoginResult == g_nLOGIN_ERR then
			 AddMessage( "��¼ʧ�ܣ� ",1);
		 end
	elseif arg1 == "GE_UPDATE_DIR" then
		Set_WorldName();
	elseif arg1 == "GE_DIR_ERR" then
		 AddMessage( "����������ʧ��!",1);
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
		AddMessage("�������ӶϿ�",1);
	end
end



function CFloginBtn_OnClick()
	--guajiFrame:Show();
	--OffMgr:setsize();
	if MudMgr:getSelectWorldState()  == -1 then
		AddMessage("��ѡ���������",1);
		return;
	elseif MudMgr:getSelectWorldState() == DIR_NOT_WORK then
		AddMessage("��ǰ����������ά��״̬��",1);
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
	--��ȡ��һ�ε���Ϸ����ѡ���¼
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
			button:SetClientString("ά��");
		elseif nWorldState == DIR_EXCELLENCE then
			font:SetTextColor( 0, 255, 0);
			button:SetClientString("����");
		elseif nWorldState == DIR_NORMAL then
			font:SetTextColor( 255, 150, 0);
			button:SetClientString("һ��");
		elseif nWorldState == DIR_BUSY then
			font:SetTextColor( 210, 65, 255);
			button:SetClientString("��æ");
		else
			font:SetTextColor( 255, 0, 0);
			button:SetClientString("����");
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
     return "սʿ"
    elseif i == 2 then
     return "��ʦ"
    elseif i==3 then
     return "ǹ��"
    elseif i == 4 then
    return "��˾"
    elseif i == 17 then
    return "�ػ���"
    elseif i == 33 then
    return "������"
    elseif i == 18 then
    return "������"
    elseif i == 34 then
    return "����ʦ"
    elseif i == 19 then
    return "��ɱ��"
    elseif i == 35 then
    return "������"
    elseif i == 20 then
    return "ʥ����"
    elseif i == 36 then
    return "Ԧ��ʦ"
    end
    return "δ֪";
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
		--Ϊfalse����ʾ��һ�ε�¼��Ϊtrue����ʾ��ɫ���л�
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
			AddMessage("���ڵȴ��������ظ������Եȣ�",0);
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
				AddMessage("���ڵȴ��������ظ������Եȣ�",0);
				return ;
			end
		end
	end	
	AddMessage("��ѡ��Ҫ��¼�Ľ�ɫ!",0);
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
--��old����֮ǰ��״̬����ѡ�˽���״̬������
function SelectManFrameOnShow()
	oldstate=MainPlayer:getGameState();
	MainPlayer:setGameState(ROLELIST_STATE);
	--Ĭ��ѡ���ɫ
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
			{ AUTH_ERR_SYS,								"�������Ӵ���"},
			{ AUTH_REASON_PASSWD_ERROR,					"�˺Ż����������"},
			{ AUTH_REASON_NO_ACCOUNT,					"�˺Ż����������"},
			{ AUTH_REASON_FREEZE,						"�����˺�û�л��������ʸ�"},
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
			{ AUTH_IFM_AASUSE_IDCARD_EXIST_ERROR,		"�������û�ע�����֤�Ѵ���"},
			{ AUTH_IFM_USE_IDCARD_FORMAT_ERROR,			"��������֤��ʽ"},
			{ AUTH_IFM_NEED_REGIST_CODE_ERROR,			"�ڲ��ڼ䣬��Ҫ������"},
			{ AUTH_IFM_REGIST_CODE_ERROR,				"�����벻��ȷ���޷�������Ϸ"},
			{ AUTH_IFM_SYSTEM_BUSY,						"ϵͳ��æ"}
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