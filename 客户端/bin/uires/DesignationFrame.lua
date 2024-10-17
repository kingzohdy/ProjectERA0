local szCurCategory;
local nCurSelectIndex;
local nCurSelectBtn = 1;
local MAX_DESIGNATIONNUM_UI;
local MAX_CATEGORYBTN = 5;
local nTotalSliderValue = 0;
local t_unHideDesignList = {};
local bDelDesignation = false;

-- ��ʼ������
function InitEquipDesignation()
	szCurCategory			= "ȫ��";
	nCurSelectIndex			= 1;
	nCurSelectBtn			= 1;
	MAX_DESIGNATIONNUM_UI		= 7;
	bDelDesignation			= false;
end

function EquipDesignationFrame_Test_OnShow()
	local btn				= getglobal( "EquipDesignationFrameXinShouBtn");
	local animationTexture	= getglobal( "EquipDesignationFrameXinShouBtnUVAnimationTex");
	
	animationTexture:SetUVAnimation( 50, true );
	btn:Show();
end

function CheckDesignationGuide()
	if GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO ) and 
			not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CHENG_HAO_BTN_IN_CHENG_HAO_FRAME_GET_CHENG_HAO ) then

		local btn				= getglobal( "EquipDesignationFrameXinShouBtn");
		local animationTexture	= getglobal( "EquipDesignationFrameXinShouBtnUVAnimationTex");
		animationTexture:SetUVAnimation( 50, true );
		animationTexture:Show();
		btn:Show();
	end
end

--���·�����ʾ
function LoveShowCheck_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if this:GetCheckState() then
		mainplayer:setShowLove( true )
	else
		mainplayer:setShowLove( false )
	end
end
function LoveShowCheck_OnUpdate()
	SetLoveCheck()
end
function SetLoveCheck()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:isShowLove() then
		EquipDesignationFrame_LoveShowBtnCheck:setCheckState( true )
	else
		EquipDesignationFrame_LoveShowBtnCheck:setCheckState( false )
	end
end
function LoveShowBtn_OnEnter()
	SetGameTooltips( "EquipDesignationFrame_LoveShowBtnCheck", "��ʾ/�����Լ������»���³ƺ�" )
end

function EquipDesignationFrame_OnLoad()
	this:RegisterEvent( "GE_DESIGNATION_UPDATE" );
	this:RegisterEvent( "GE_ENTER_PLAYERLOGIN" );
	this:RegisterEvent( "GE_CUR_DESIGNATION" );
	InitEquipDesignation();
	local num = 0;
	for i=1,GameDesignation:GetDesignationMapSize() do
		if num + 1 > MAX_CATEGORYBTN then
			break;
		end		
		local szName = GameDesignation:GetDesignationCateName( i-1 );
		if szName ~= "" then
			local btn = getglobal( "EquipDesignationFrame_CategoryName" .. num + 1 );
			btn:SetText( szName );
			num = num + 1;
		end
	end
	for i=num + 1, MAX_CATEGORYBTN do
		local btn = getglobal( "EquipDesignationFrame_CategoryName" .. i );
		btn:Hide();
	end
end

function EquipDesignationFrame_OnEvent()
	if arg1 == "GE_ENTER_PLAYERLOGIN" then
		DisCheckedOtherDesignationBtn();
		EquipDesignationFrame_AllButton:Checked();
		InitEquipDesignation();
	elseif arg1 == "GE_DESIGNATION_UPDATE" then
		local slider = getglobal("EquipDesignationFrame_ScrollBar");
		local nValue = slider:GetValue();
		nValue = ( nValue == 0 and 1 or nValue );
		if bDelDesignation then
			nCurSelectIndex	= 1;
			nCurSelectBtn = 1;
			bDelDesignation = false;
			slider:SetValue( 0 );
			nValue = 1;
		end	
		BondingDesignationList( nValue );
		SetDesignationDesc();
	elseif arg1 == "GE_CUR_DESIGNATION" then
		-- ���õ�ǰ�ƺ�
		SetCurDesignation();
	end
end

function NeedRunXinShouDesignation()
	if EquipDesignationFrame:IsShown() and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CHENG_HAO_BTN_IN_CHENG_HAO_FRAME_GET_CHENG_HAO ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO );
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO );
		CheckDesignationGuide();
		return true;
	end

	if EquipFrame:IsShown() and not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_CHANRACTER_FRAME_LABLE_BTN_GET_CHENG_HAO ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO );
		ShowEquipGuide();
		return true;
	end
	
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO ) then
		GuideEffectShow( "", "","CharacterButton", "�����������棬���óƺ�\n#P��ݼ���C��", "uires\\ZhuJieMian\\1.dds", 
									352, 609, 60, 60, 50, 60, -9, 5, 11, -51, true );
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_MAIN_BTN_GET_CHENG_HAO );
		return true;
	end

	return false;
end

-- �ƺŴ��ڴ�ʱ��Ӧ
function EquipDesignationFrame_OnShow()
	-- ���óƺ��б�
	if nCurSelectBtn == 1 and nCurSelectIndex == 1 then
		EquipDesignationFrame_ScrollBar:SetValue( 0 );
		BondingDesignationList(1);
		SetDesignationDesc();
	end		
	--EquipDesignationFrame_TipsDescRich:SetText( "��ʾ:ʹ���µĳƺŽ��滻���гƺŵ�����Ч����", 254,108,28 );
	CheckDesignationGuide();
	--���÷���ͷ���Ƿ�ѡ
	SetLoveCheck();
end

-- ���õ�ǰ�ƺ�
function SetCurDesignation()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer ~= nil then
		local szCurDesignation =  MainPlayer:getDesignation();
		EquipDesignationFrame_CurDesignationBtn_CurFont:SetText( szCurDesignation );
	end	
end

-- �ƺŴ��ڹر�ʱ��Ӧ
function EquipDesignationFrame_OnHide()
	local btn	= getglobal( "EquipDesignationFrameXinShouBtn");
	btn:Hide();
end

-- �ƺŴ���֡ˢ��
function EquipDesignationFrame_OnUpdate()
	for i = 1, MAX_DESIGNATIONNUM_UI do
		local DesignationBtn		= getglobal( "EquipDesignationFrame_NameBtn" .. i ); 
		DesignationBtn:DisChecked();
	end
	
	local nIndex = nCurSelectBtn - EquipDesignationFrame_ScrollBar:GetValue();
	if nIndex > 0 and nIndex <= MAX_DESIGNATIONNUM_UI  then
		local CheckBtn = getglobal( "EquipDesignationFrame_NameBtn" .. nIndex );  
		CheckBtn:Checked();
	end
end

-- �ų����صĳƺ�
function GetUnHideDesignInfo()
	-- ��ȡ�ƺŵ�����
	local nDesignationNum = 0;
	if szCurCategory == "ȫ��" then
		nDesignationNum = GameDesignation:getDesignationNum();		
	else
		nDesignationNum = GameDesignation:getDesignationNum( szCurCategory );
	end

	t_unHideDesignList = {};
	nTotalSliderValue = nDesignationNum;
	for i=1, nDesignationNum do
		local DesignationDef = nil;
		if szCurCategory == "ȫ��" then	 
			DesignationDef = GameDesignation:getDesignation( i - 1 );
		else
			DesignationDef = GameDesignation:getDesignation( szCurCategory, ( i - 1 ) );   
		end
		if DesignationDef ~= nil then
			if DesignationDef.NotShow == 1 and not GameDesignation:isHaveDesignation( DesignationDef.ID ) then
				nTotalSliderValue = nTotalSliderValue - 1;
			else
				table.insert( t_unHideDesignList, DesignationDef );
			end
		end
	end
end

-- �󶨳ƺ��б�
function BondingDesignationList( nSliderValue )
	-- ��ȡ����ʾ�ĳƺ��б�
	GetUnHideDesignInfo();

	-- �����б������	
	if nTotalSliderValue > MAX_DESIGNATIONNUM_UI then
		EquipDesignationFrame_ScrollBar:SetMaxValue( nTotalSliderValue - MAX_DESIGNATIONNUM_UI );
		EquipDesignationFrame_ScrollBar:SetValueStep(1);
		EquipDesignationFrame_ScrollBar:Show();
		EquipDesignationFrame_ScrollUpBtn:Show();
		EquipDesignationFrame_ScrollDownBtn:Show();
	else												  		
		EquipDesignationFrame_ScrollBar:Hide();
		EquipDesignationFrame_ScrollUpBtn:Hide();
		EquipDesignationFrame_ScrollDownBtn:Hide();
	end 

	-- �ڽ�������ʾ�ƺ�
	local nListIndex	= 1;
	for i = nSliderValue, nTotalSliderValue do
		if nListIndex <= MAX_DESIGNATIONNUM_UI then
			local DesignationDef = nil;
			DesignationDef = t_unHideDesignList[i];
			-- �󶨳ƺ��б�
			local DesignationBtn		= getglobal( "EquipDesignationFrame_NameBtn" .. nListIndex ); 
			local DesignationNameFont	= getglobal( "EquipDesignationFrame_NameBtn" .. nListIndex .. "Name" );
			DesignationBtn:Show();
			DesignationNameFont:SetText( DesignationDef.Name );	
			DesignationNameFont:SetTextColor(133,133,130);
			if GameDesignation:isHaveDesignation( DesignationDef.ID ) then
				DesignationNameFont:SetTextColor(255,254,189);
			end
			nListIndex = nListIndex + 1; 
			DesignationBtn:SetClientUserData( 0, i );
		end
	end
	
	for i = nListIndex, MAX_DESIGNATIONNUM_UI do 
		local DesignationBtn		= getglobal( "EquipDesignationFrame_NameBtn" .. i ); 
		DesignationBtn:Hide();
	end
end

local t_LevelStar = {
			[0] = "��";
			[1] = "���";
			[2] = "����";
			[3] = "�����";
			[4] = "������";
			[5] = "�������";
			};

function GetLevelStar( nLevel )
	return t_LevelStar[nLevel];
end

function GetDesignationEndTime( DesignationDef )
	if DesignationDef == nil then
		return "��ʱ";
	end

	if not GameDesignation:isHaveDesignation( DesignationDef.ID ) then
		return "��ʱ";
	end

	local szEndTime = GameDesignation:getDesignationEndTime( DesignationDef.ID );
	if szEndTime ~= 0 then
		szEndTime = "��Ч����"..os.date("%Y", szEndTime).."-"..os.date("%m", szEndTime).."-"..os.date("%d", szEndTime).."  "
					..os.date("%H", szEndTime)..":"..os.date("%M", szEndTime);
	end
	return szEndTime == 0 and "��ʱ" or szEndTime;
end

-- ���óƺ�������Ϣ
function SetDesignationDesc()
	local DesignationLevelDesc = "�ƺ�Ʒ�ʣ�";
	local DesignationTypeDesc = "�ƺ����ͣ�";		
	local DesignationDef	 = nil;
	
	if nTotalSliderValue > 0 then			
		local btn = getglobal( "EquipDesignationFrame_NameBtn1" );

		DesignationDef = t_unHideDesignList[nCurSelectIndex];

		DesignationLevelDesc = DesignationLevelDesc .. GetLevelStar( DesignationDef.Level ) .. "#n\n";
		DesignationTypeDesc = DesignationTypeDesc .. ( DesignationDef.DesTime == 0 and "����" or GetDesignationEndTime( DesignationDef ) ) .. "#n\n\n";
		EquipDesignationFrame_DescRich:SetText( DesignationLevelDesc .. DesignationTypeDesc .. DesignationDef.DescText , 254, 108, 28 );
	else
		EquipDesignationFrame_DescRich:SetText( DesignationLevelDesc .. "#n\n" .. DesignationTypeDesc .. "#n\n", 254, 108, 28 );
	end
end

function SetIndexValue( idx )
	local num = table.getn( t_unHideDesignList );
	nCurSelectIndex = idx;
	local slider = getglobal( "EquipDesignationFrame_ScrollBar" );
	if num > MAX_DESIGNATIONNUM_UI then
		slider:SetMaxValue( num - MAX_DESIGNATIONNUM_UI );
		if num - idx < MAX_DESIGNATIONNUM_UI then	
			slider:SetValue( slider:GetMaxValue() );
			BondingDesignationList( slider:GetMaxValue() + 1 );			
		else
			slider:SetValue( idx - 1 );
			BondingDesignationList( idx );
		end
		nCurSelectBtn = idx;
	else	
		slider:SetValue( 1 );
		BondingDesignationList( 1 );
		nCurSelectBtn = idx + 1;
	end
end

function AddDesignation( nId )
	local DesignationDef = GameDesignation:getDesignationById( nId );
	if DesignationDef ~= nil then
		szCurCategory = DesignationDef.CategoryName;
		if szCurCategory ~= "" then
			for i=1, MAX_CATEGORYBTN do
				local btn = getglobal( "EquipDesignationFrame_CategoryName" .. i );
				if btn:GetText() == szCurCategory then
					DisCheckedOtherDesignationBtn();
					btn:Checked();
					break;
				end
			end
		else
			DisCheckedOtherDesignationBtn();
			AllDesignationButton:Checked();
		end
	end

	GetUnHideDesignInfo();
	local idx = 1;
	local num = table.getn( t_unHideDesignList );
	for i=1, num do
		local DesignInfo = t_unHideDesignList[i];
		if DesignInfo.Name == DesignationDef.Name then
			idx = i;
			break;
		end
	end
	
	SetIndexValue( idx );	
	SetDesignationDesc();

	-- ��һ�λ�óƺ�
	if NeedRunXinShouDesignation() then		
		return;
	end

	if not EquipDesignationFrame:IsShown() then
		local parentframe		= getglobal( "EquipFrame" );
		local desframe			= getglobal( "EquipDesignationFrame" );
		local deschenghaobtn	= getglobal( desframe:GetName().."_ChengHaoBtn" );
		local desfashionbtn		= getglobal( desframe:GetName().."_FashionBtn" );  
		local desshengwangbtn	= getglobal( desframe:GetName().."_ShengWangBtn" );
		local deseNengYuanBtn	= getglobal( desframe:GetName().."_NengYuanBtn" );
		local desequipbtn		= getglobal( desframe:GetName().."_EquipBtn" );	  
		
		parentframe:Hide();
		desframe:Show();
		deschenghaobtn:Checked();
		desfashionbtn:DisChecked();
		desshengwangbtn:DisChecked();
		deseNengYuanBtn:DisChecked();	
		desequipbtn:DisChecked(); 
		desframe:SetPoint("topleft", "UIClient", "topleft",parentframe:GetRealLeft(), parentframe:GetRealTop());		
	end	
end

function GetDesignationBufStateDesc( DesignationDef ) 
	local szDesc = "";
	if DesignationDef ~= nil then 
		if GameDesignation:isHaveDesignation( DesignationDef.ID ) then
			if GameDesignation:isFirstSetDesignation( DesignationDef.ID ) then
				szDesc = "#cFFFFFF�óƺŽ���δʹ�ã�"
			else
				szDesc = "#cFFFFFF�óƺŽ�����ʹ�ã�"
			end
		else
			szDesc = "#cFFFFFF����δӵ�иóƺţ�"
		end
	end
	return szDesc;
end
 
function DisCheckedOtherDesignationBtn()
	EquipDesignationFrame_AllButton:DisChecked();
	for i=1, MAX_CATEGORYBTN do
		local btn = getglobal( "EquipDesignationFrame_CategoryName" .. i );
		btn:DisChecked();
	end
end

-- ȫ���ƺŰ�ť����ʱ��Ӧ
function AllDesignationButton_OnClick()
	-- ���ñ�ǩ����
	DisCheckedOtherDesignationBtn();
	this:Checked();
	-- ���õ�ǰ�ƺ����
	szCurCategory	= "ȫ��"; 
	nCurSelectIndex	= 1;
	nCurSelectBtn = 1;
	-- �󶨳ƺ��б�
	EquipDesignationFrame_ScrollBar:SetValue( 0 );
	BondingDesignationList(1);	
	SetDesignationDesc();
end

-- ����ƺŰ�ť����ʱ��Ӧ
function JuQinButton_OnClick() 
	-- ���ñ�ǩ����
	DisCheckedOtherDesignationBtn();
	this:Checked();
	-- ���õ�ǰ�ƺ����
	szCurCategory	= this:GetText(); 
	nCurSelectIndex	= 1;
	nCurSelectBtn = 1;
	-- �󶨳ƺ��б�
	EquipDesignationFrame_ScrollBar:SetValue( 0 );
	BondingDesignationList(1); 
	SetDesignationDesc();
end

-- �����ƺŰ�ť����ʱ��Ӧ
function ShengWangeButton_OnClick()	
	-- ���ñ�ǩ����
	DisCheckedOtherDesignationBtn();
	this:Checked();
	-- ���õ�ǰ�ƺ����
	szCurCategory	= this:GetText();
	nCurSelectIndex	= 1;
	nCurSelectBtn = 1;
	-- �󶨳ƺ��б�
	EquipDesignationFrame_ScrollBar:SetValue( 0 );
	BondingDesignationList(1); 
	SetDesignationDesc();
end

-- �����ƺŰ�ť����ʱ��Ӧ
function JingJiButton_OnClick()	
	-- ���ñ�ǩ����
	DisCheckedOtherDesignationBtn();
	this:Checked();
	-- ���õ�ǰ�ƺ����
	szCurCategory	= this:GetText();
	nCurSelectIndex	= 1;
	nCurSelectBtn = 1;
	-- �󶨳ƺ��б�
	EquipDesignationFrame_ScrollBar:SetValue( 0 );
	BondingDesignationList(1); 
	SetDesignationDesc();
end

-- ��ƺŰ�ť����ʱ��Ӧ
function HuoDongButton_OnClick() 
	-- ���ñ�ǩ����
	DisCheckedOtherDesignationBtn();
	this:Checked();
	-- ���õ�ǰ�ƺ����
	szCurCategory	= this:GetText(); 
	nCurSelectIndex	= 1;
	nCurSelectBtn = 1;
	-- �󶨳ƺ��б�
	EquipDesignationFrame_ScrollBar:SetValue( 0 );
	BondingDesignationList(1);
	SetDesignationDesc();
end

-- ����ƺŰ�ť����ʱ��Ӧ
function TeShuButton_OnClick()
	-- ���ñ�ǩ����
	DisCheckedOtherDesignationBtn();
	this:Checked(); 
	-- ���õ�ǰ�ƺ����
	szCurCategory	= this:GetText(); 
	nCurSelectIndex	= 1;
	nCurSelectBtn = 1;
	-- �󶨳ƺ��б�
	EquipDesignationFrame_ScrollBar:SetValue( 0 );
	BondingDesignationList(1);
	SetDesignationDesc();
end

function DesignationNameBtn_OnClick() 
	local nValue = EquipDesignationFrame_ScrollBar:GetValue();
	nCurSelectIndex = (this:GetClientUserData( 0 ) == 0 and 1 or this:GetClientUserData( 0 ));
	nCurSelectBtn = this:GetClientID() + nValue;
	SetDesignationDesc();
end

function DesignationNameBtn_OnEnter()
	--[[
	local DesignationDef = nil;
	local NameFont = getglobal( this:GetName().."Name" );	 
	local nValue = EquipDesignationFrame_ScrollBar:GetValue();
	if szCurCategory == "ȫ��" then
		DesignationDef = GameDesignation:getDesignation( this:GetClientID() + nValue - 2 );
	elseif szCurDesignation ~= "" then
		DesignationDef = GameDesignation:getDesignation( szCurCategory, this:GetClientID() + nValue - 1 );  
	end
	
	if DesignationDef ~= nil then 
		local szDesc = "";
		if GameDesignation:isHaveDesignation( DesignationDef.ID ) then
			if GameDesignation:isFirstSetDesignation( DesignationDef.ID ) then
				szDesc = "�óƺŽ���δʹ�ã�"
			else
				szDesc = "�óƺŽ�����ʹ�ã�"
			end
		else
			szDesc = "����δӵ�иóƺţ�"
		end

		SetGameTooltips(this:GetName(), szDesc );
	end
	--]]
end
 
-- ʹ��ѡ�гƺ�
function SetDesignation_OnClick()
	local btn = getglobal( "EquipDesignationFrameXinShouBtn");
	if btn:IsShown() then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_CHENG_HAO_BTN_IN_CHENG_HAO_FRAME_GET_CHENG_HAO );
	end
	btn:Hide();
	local MainPlayer = ActorMgr:getMainPlayer(); 
	local nValue = EquipDesignationFrame_ScrollBar:GetValue();
	if MainPlayer ~= nil then
		local szOldDesignation = MainPlayer:getDesignation();
		local nOldDesignationID= MainPlayer:getDesignationID();
		local nDesignationId = nil;
		local DesignationDef = nil;
		DesignationDef = t_unHideDesignList[nCurSelectIndex];
		nDesignationId = DesignationDef.ID;

		if nOldDesignationID ~= 0 and nOldDesignationID == nDesignationId then
			AddGameMiddleTips2("�����øóƺţ�");
			return;
		end

		if not GameDesignation:isHaveDesignation( nDesignationId ) then
			AddGameMiddleTips2("����δӵ�иóƺţ�");
			return;
		end
		
		if MainPlayer:getVIM() < DesignationDef.VimCost then
			AddGameMiddleTips2("�������㣬�޷����øóƺţ�");
			return;
		end	
		--[[
		MessageBoxFrame:SetClientUserData(0,nDesignationId);
		if szOldDesignation == "" or szOldDesignation ~= DesignationDef.Name then
			MessageBox("��ʾ","�Ƿ񽫵�ǰ�ƺ�����Ϊ��".. DesignationDef.Name .."��\n\b\b\b\b\b  (�ò�������"..DesignationDef.VimCost.."�㾫��ֵ)	");
		else
			MessageBox("��ʾ","�Ƿ��ٴ�ʹ�øóƺţ�\n\b      (�ò�����ˢ������Ч��,����"..DesignationDef.VimCost.."�㾫��ֵ)");
		end
		MessageBoxFrame:SetClientString("���óƺ�");
		--]]
		GameDesignation:sendSetDesignationReq( nDesignationId );
	end
end

function CancleDesignation_OnClick()
	if GameDesignation:getCurDesignationId() ~= 0 then		
		MessageBox("��ʾ","�Ƿ�ȡ����ǰ�ƺţ�");
		MessageBoxFrame:SetClientString("ȡ���ƺ�");
	else
		AddGameMiddleTips2("��ǰû������ʹ�õĳƺ�");
	end
end

function EquipDesignationFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("EquipDesignationFrame_ScrollBar");
	local nValue = slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		BondingDesignationList( nValue + 1 );
	end	
end

function EquipDesignationFrame_ScrollDownBtn_OnClick()
	local slider = getglobal("EquipDesignationFrame_ScrollBar");
	local nValue = slider:GetValue() + slider:GetValueStep();
	local nMaxValue = slider:GetMaxValue();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function EquipDesignationFrame_ScrollUpBtn_OnClick()	
	local slider = getglobal("EquipDesignationFrame_ScrollBar");
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end	
end

function DesignationSetResult( nState, nId )
	if nState == 0 then
		AddGameMiddleTips2("�ƺ�����ʧ�ܣ�");
	elseif nId == 0 then
		bDelDesignation = true;
		AddGameMiddleTips2("��ȡ���ƺţ�");
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "ȡ���ƺ�" then
			MessageBoxFrame:Hide();
		end		
	else								   		
		AddGameMiddleTips2("�ƺ����óɹ���"); 
	end
end