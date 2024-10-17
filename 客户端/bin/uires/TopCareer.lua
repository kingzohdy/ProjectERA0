local t_TopCareerTexture = 
-- ����ְҵ������ְҵͼƬ
{
	-- սʿ˫��  ������
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 224, y = 455, width = 34, height = 34 } },
	-- սʿ����  �ػ���
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 292, y = 421, width = 34, height = 34 } },

	-- ��˾˫��	Ԧ��ʦ
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 258, y = 421, width = 34, height = 34 } },
	-- ��˾����	ʥ����
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 224, y = 421, width = 34, height = 34 } },

	-- ǹ�ֵ���  ��ɱ��
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 326, y = 387, width = 34, height = 34 } },
	-- ǹ��˫��	������
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 292, y = 387, width = 34, height = 34 } },

	-- ��ʦ˫��  ����ʦ
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 224, y = 387, width = 34, height = 34 } },
	-- ��ʦ����  ������
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 258, y = 387, width = 34, height = 34 } },
};
local t_RaceToButtonIdx = 
-- ����ְҵ������ְҵͼƬ
{
	-- սʿ							-- սʿ����  �ػ���				-- սʿ˫��  ������
	[g_nCAREER_WARRIOR]			= 1,[g_nCAREER_WARRIOR_SINGEL]	= 2,[g_nCAREER_WARRIOR_DOUBLE]	= 1,

	-- ��ʦ							-- ��˾����	ʥ����				-- ��˾˫��	Ԧ��ʦ
	[g_nCAREER_CHURCH]			= 3,[g_nCAREER_CHURCH_SINGEL]	= 4,[g_nCAREER_CHURCH_DOUBLE]	= 3,

	-- ǹ��							-- ǹ�ֵ���  ��ɱ��				-- ǹ��˫��	������
	[g_nCAREER_GUNER]			= 5,[g_nCAREER_GUNER_SINGEL]	= 5,[g_nCAREER_GUNER_DOUBLE]	= 6,

	-- ��ʦ							-- ��ʦ����  ������				-- ��ʦ˫��  ����ʦ
	[g_nCAREER_MAGICOR]			= 7,[g_nCAREER_MAGICOR_SINGEL]	= 8,[g_nCAREER_MAGICOR_DOUBLE]	= 7,
};

local MAX_CHAMPION_NUM = 8
local t_LastChampions = {}			--���ڼ�¼�Ͻ��ùھ��������Ϣ
local nRoleRotate = 0
local nRoleAngle = 0
function OpenTopCareerFrame( nStatus )
	--���ú��Ͻ�ھ�������
	for i = 1, MAX_CHAMPION_NUM do		
		t_LastChampions[i] = GameTopCareer:getCareerChampionName(i-1)
	end
	local time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*5+3600*19.5 )
	--�����ǰʱ�䳬������������ʱ��
	if getServerTime().sec > GameTopCareer:getActiveTime()+3600*24*5+3600*20.5 then
		time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*19+3600*20.5 )
	end
	local szTime = string.format( "%d-%d-%d 19:30 ", time.year, time.month, time.day, time.hour, time.min ) .. string.format( "(%s)", tWeekDayName[time.wday] )
	TopCareerFrameTime:SetText( "������ʼʱ�䣺"..szTime )
	--��ʼ��ģ����ת��Ϣ
	nRoleRotate = 0
	nRoleAngle = 0
	TopCareerFrame:Show();
end
function TopCareerFrame_OnLoad()
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerFrameChampion"..i )
		local btnCareerTex = getglobal( btn:GetName().."Career" )
		local uv = t_TopCareerTexture[i]["UV"]
		btnCareerTex:SetTexUV( uv.x, uv.y, uv.width, uv.height )		
	end
end
function TopCareerFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	local mainplayer = ActorMgr:getMainPlayer();
	local idx = mainplayer == nil and 1 or t_RaceToButtonIdx[mainplayer:getRace()]
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerFrameChampion"..i )
		btn:DisChecked()
	end
	local btn = getglobal( "TopCareerFrameChampion"..idx )
	btn:Checked()
	SetCurTopCareerRole()
end
function SetCurTopCareerRole()
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerFrameChampion"..i )
		if btn:IsChecked() then	
			local id = btn:GetClientID()
			TopCareerFrameRole_ModelView:setAvatarAttrib( 0, 0, 0, 0, true)
			for j=1, CLIENT_MAX_AVATAR_ATTRIB do
				if GameTopCareer:getCareerChampionAvatarId( id - 1, j-1 ) > 0 then
					if j == 1 then
						TopCareerFrameRole_ModelView:setAvatarAttrib(0, j-1, GameTopCareer:getCareerChampionAvatarId( id-1, j-1 ), GameTopCareer:getCareerChampionAvatarStarLv( id-1, j-1 ), true );
					else
						TopCareerFrameRole_ModelView:setAvatarAttrib(0, j-1, GameTopCareer:getCareerChampionAvatarId( id-1, j-1 ), GameTopCareer:getCareerChampionAvatarStarLv( id-1, j-1 ) );
					end
				end
			end
			TopCareerFrameRole_ModelView:setAvatarAttrib( 0, 0, GameTopCareer:getCareerChampionRace( id - 1 ), 0 )
			TopCareerFrameRole_ModelView:setAvatarAttrib( 0, 1, GameTopCareer:getCareerChampionGender( id - 1 ), 0 )
			TopCareerFrameRole_ModelView:setPetScale(0,0.1,0.1,0.1)
			TopCareerFrameRole_ModelView:setEntityAvatar( 0 );
			TopCareerFrameRole_ModelView:setEntityRotate( 0, nRoleAngle );	
			TopCareerFrameRole_ModelView:setEntityPosition(0,0,-10,90);
			local leftarmid		=  GameTopCareer:getCareerChampionAvatarId( id - 1, POS_ID_LEFTHAND );
			local rightarmid	=  GameTopCareer:getCareerChampionAvatarId( id - 1, POS_ID_RIGHTHAND );	
	
			local t_AvartarControl	= GetAvartarControl();
			local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GameTopCareer:getCareerChampionRace( id - 1 ), ["leftHandItem"] = leftarmid, 
														["rightHandItem"] = rightarmid } );
			TopCareerFrameRole_ModelView:playEntityAnim(0, nAnimID, nil );
		end
		--���ùھ����ƺ���������
		local btnName = getglobal( btn:GetName().."Name" )
		local name = GameTopCareer:getCareerChampionName(i-1) == "" and "�ھ���λ�Դ�" or GameTopCareer:getCareerChampionName(i-1)
		btnName:SetText( name )
		local btnClan = getglobal( btn:GetName().."Clan" )
		name = GameTopCareer:getCareerChampionClanName(i-1) == "" and "" or GameTopCareer:getCareerChampionClanName(i-1)
		btnClan:SetText( name )
	end
end
function TopCareerFrame_OnUpdate()
end
function ChampionButtonTemplate_OnClick()
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerFrameChampion"..i )
		btn:DisChecked()
	end
	nRoleAngle = 0
	this:Checked()
	SetCurTopCareerRole()
end
function TopCareerFrameRuleButton_OnClick()
	TopCareerRuleFrame:Show()
end
function TopCareerFrameGuessButton_OnClick()
	GameTopCareer:requestTopCareerIfGuessed()
end
function TopCareerFrameEnterButton_OnClick()
	MessageBox( "��ʾ", "����ĩ���۷���������Ҫ��ȡ#B"..MOSHI_DIANFENG_BAOMING_MONEY_TOLUA.."����#n�����ѣ�ȷ��������" )
	MessageBoxFrame:SetClientString( "ĩ���۷屨��" )
end
function TopCareerFrameRole_Model_OnMouseDown()
	if arg1 == "LeftButton" then
		nRoleRotate = GetCursorPosX()
	end
end
function TopCareerFrameRole_Model_OnMouseMove()
	if this:GetPushedState() then
		local nCurnRoleRotate = GetCursorPosX();
		nRoleAngle = nRoleAngle - (nCurnRoleRotate - nRoleRotate) ;
		TopCareerFrameRole_ModelView:setEntityRotate(0, nRoleAngle);
		nRoleRotate = nCurnRoleRotate;
	end
end
---------------------------------------------�۷�������---------------------------------------------------------
local TopCareerRules = [[#cffffbe������ܣ�
#cffffbe	1. ����ʱ�䣺#B%s��%s#n
#cffffbe	2. �μӱ���Ҫ��#B�ȼ���65��#n
#cffffbe	3.������ʼʱ�䣺#B%s#n
#cffffbe	4.���ƣ�3��2ʤ�ƣ�ʧ�ܷ�ֱ����̭
#cffffbe	5.����ʱ����ʹ�ù����е�͸��ִ��;���
#B	6.����ʱ���������ʱ�䳬��2������Ϊ����]]

function TopCareerRuleFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	local beginTime = os.date( "*t", GameTopCareer:getActiveTime() )
	--�����ǰʱ�䳬������������ʱ��
	if getServerTime().sec > GameTopCareer:getActiveTime()+3600*24*5+3600*20.5 then
		beginTime = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*14 )
	end
	local szBeginTime = string.format( "%d-%d-%d", beginTime.year, beginTime.month, beginTime.day )
	local time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*5+3600*19.5 )
	--�����ǰʱ�䳬������������ʱ��
	if getServerTime().sec > GameTopCareer:getActiveTime()+3600*24*5+3600*20.5 then
		time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*19+3600*19.5 )
	end
	local szTime = string.format( "%d-%d-%d %d:%d ", time.year, time.month, time.day, time.hour, time.min ) 
	TopCareerRuleFrameInfo:SetText( string.format( TopCareerRules, szBeginTime, szTime, szTime.. string.format( "(%s)", tWeekDayName[time.wday] ) ), 255, 255, 190 )
end
function TopCareerRuleFrame_OnClick()
	MessageBox( "��ʾ", "����ĩ���۷���������Ҫ��ȡ#B"..MOSHI_DIANFENG_BAOMING_MONEY_TOLUA.."����#n�����ѣ�ȷ��������" )
	MessageBoxFrame:SetClientString( "ĩ���۷屨��" )
end
------------------------------------------����ְҵѡ�����-------------------------------------------------------
local TopCareerGuessInfos = [[#cffffbe	1.�Ա����۷�֮ս��8��ְҵ�ھ����о���
#cffffbe	2.���������ٿɵõ�1w���Ҳ��ҿ��Է�����
#cffffbe	3.��������%d����
#cffffbe	4.�Ͻ��ְҵ��ھ���]]
local t_SelectWiners = {"ѡ��֧�ֵ�����","ѡ��֧�ֵ��ػ�","ѡ��֧�ֵ�Ԧ��","ѡ��֧�ֵ�ʥ��",
						"ѡ��֧�ֵ���ɱ","ѡ��֧�ֵĻ���","ѡ��֧�ֵ�����","ѡ��֧�ֵľ���",}
local t_TopCareerTypeName = { "[����]", "[�ػ�]", "[Ԧ��]", "[ʥ��]", "[��ɱ]", "[����]", "[����]", "[����]", }
local HasGuessed = false
function TopCareerGuessFrame_OnLoad()
	this:RegisterEvent("GE_UPDATE_ROLELIST")
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerGuessFrameCareer"..i )
		local btnCareerTex = getglobal( btn:GetName().."Career" )
		local uv = t_TopCareerTexture[i]["UV"]
		btnCareerTex:SetTexUV( uv.x, uv.y, uv.width, uv.height )		
	end
	TopCareerGuessFrameMoneyAward:setCenterLine( true )
end
function TopCareerGuessFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerGuessFrameCareer"..i )
		btn:DisChecked()
	end
	UpdateTopCareerGuessFrame()
end
function UpdateTopCareerGuessFrame()
	for i = 1, MAX_CHAMPION_NUM do
		local lastChampionFont = getglobal( "TopCareerGuessFrameLastChampion" ..i)
		if t_LastChampions[i] ~= "" then
			lastChampionFont:SetText( t_LastChampions[i]..t_TopCareerTypeName[i] )
		else
			lastChampionFont:SetText( "�ھ���λ�Դ�"..t_TopCareerTypeName[i] )
		end
	end
	local szGuessInfos = string.format( TopCareerGuessInfos, MOSHI_DIANFENG_JINGCAI_MONEY_TOLUA )
	TopCareerGuessFrameRemindInfo:SetText( szGuessInfos, 255, 255, 190 )
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerGuessFrameCareer"..i )
		local btnName = getglobal( btn:GetName().."Name" )
		btnName:SetText( t_SelectWiners[i] )
	end
end
function TopCareerGuessFrame_OnEvent()
	if arg1 == "GE_UPDATE_ROLELIST" then
		for i = 1, MAX_CHAMPION_NUM do
			GameTopCareer:SelectTopCareerWiner( i - 1, -1 )
		end
		t_SelectWiners = {"ѡ��֧�ֵ�����","ѡ��֧�ֵ��ػ�","ѡ��֧�ֵ�Ԧ��","ѡ��֧�ֵ�ʥ��",
						"ѡ��֧�ֵ���ɱ","ѡ��֧�ֵĻ���","ѡ��֧�ֵ�����","ѡ��֧�ֵľ���",}
		HasGuessed = false
	end
end
function TopCareerGuessFrame_OnHide()
	TopCareerGuessInfoFrame:Hide()
end
function CareerGuessButtonTemplate_OnClick()
	local id = this:GetClientID()	
	for i = 1, MAX_CHAMPION_NUM do
		local btn = getglobal( "TopCareerGuessFrameCareer"..i )
		btn:DisChecked()
	end
	this:Checked()
	GameTopCareer:requestTopCareerSupportInfo( id );
	TopCareerGuessInfoFrame:Hide()
end
function TopCareerGuessFrameGuessButton_OnClick()
	if HasGuessed then
		ShowMidTips( "���Ѿ����¹���" )
		return
	end
	for i = 1, MAX_CHAMPION_NUM do
		if not GameTopCareer:IsTopCareerSelected( i - 1 ) then
			ShowMidTips( "ֻ��ȫ��ְҵ��ѡ����֧���ߣ����ܾ���" )
			return
		end
	end
	MessageBox( "��ʾ", "�۷徺����Ҫ����"..MOSHI_DIANFENG_JINGCAI_MONEY_TOLUA.."���ң�ȷ�Ͼ��£�\n#B��ĩ���۷�����������ʱ�䣺������19��30��#n" )
	MessageBoxFrame:SetClientString("�۷�ھ�����");
end
function SetTopCareerGuessShow( allMoney, nflag )
	TopCareerGuessFrameMoneyAward:SetText( string.format( "#cffffbe���أ�#cfff669%d#n#cffffbe����", allMoney ), 255, 255, 190 )
	if nflag == 2 or nflag == 4 then
		HasGuessed = false
		t_SelectWiners = {"ѡ��֧�ֵ�����","ѡ��֧�ֵ��ػ�","ѡ��֧�ֵ�Ԧ��","ѡ��֧�ֵ�ʥ��",
						"ѡ��֧�ֵ���ɱ","ѡ��֧�ֵĻ���","ѡ��֧�ֵ�����","ѡ��֧�ֵľ���",}
	elseif nflag == 1 or nflag == 3 then
		HasGuessed = true
		for i = 1, MAX_CHAMPION_NUM do
			t_SelectWiners[i] = GameTopCareer:getSupportNameByIdx( i - 1 )
		end
	end
	TopCareerGuessFrame:SetPoint( "center", "UIClient", "center", -100, 0 )
	TopCareerGuessFrame:Show()
end
function SetSelctWiners()
	HasGuessed = true
	for i = 1, MAX_CHAMPION_NUM do
		t_SelectWiners[i] = GameTopCareer:getSupportNameByIdx( i - 1 )
	end
	UpdateTopCareerGuessFrame()
end
--------------------------------------ѡ�񾺲¶������-----------------------------------------------
local curGuessInfoPage = 1
local MAX_SHOW_GUESS_INFO_NUM = 18
local sortElem = "supportNum"
local sortOrder = true
local t_BtnSortElems = { "name", "clanName", "supportNum", }
local t_GuessGroupInfos = {}
function GetGuessInfos()
	t_GuessGroupInfos = {}
	for i = 1, GameTopCareer:getGuessGroupNum() do
		local oneInfo = {
			idx = i,
			name = GameTopCareer:getGuessGroupNameByIndex( i - 1 ),
			clanName = GameTopCareer:getGuessGroupClanNameByIndex( i - 1 ),
			supportNum = GameTopCareer:getGuessGroupSupportNumByIndex( i - 1 ),
		}
		table.insert( t_GuessGroupInfos, oneInfo )
	end
	SorGuessInfo( t_GuessGroupInfos, sortElem, sortOrder )
end
function SorGuessInfo( t_Infos, sortElem, sortOrder )
	table.sort( t_Infos,function(a,b)
		if sortOrder then
			return a[sortElem] > b[sortElem]
		else
			return a[sortElem] < b[sortElem]
		end
	end)
end
function OpenGueesInfoFrame( groupIdx )
	TopCareerGuessInfoFrame:SetClientUserData( 0, groupIdx )
	if t_TopCareerTypeName[groupIdx] == nil then return end	
	TopCareerGuessInfoFrameTitle:SetText( t_TopCareerTypeName[groupIdx].."�龺��" )
	TopCareerGuessInfoFrame:SetPoint( "topleft", "UIClient", "topleft", TopCareerGuessFrame:GetRealRight(), TopCareerGuessFrame:GetRealTop() )
	TopCareerGuessInfoFrame:Show()
end
function TopCareerGuessInfoButton_OnClick()
	for i = 1, MAX_SHOW_GUESS_INFO_NUM do
		local btn = getglobal( "TopCareerGuessInfoFrameInfoButton"..i )
		btn:DisChecked()
	end
	this:Checked()
	if arg1 == "RightButton" then
		local btnName = getglobal( this:GetName().."Name" )		
		RankMgr:requestWatchPlayer( btnName:GetText() )
	end
end
function TopCareerGuessInfoFrameSelectButton_OnClick()
	if HasGuessed then
		ShowMidTips( "���Ѿ����¹���" )
		return
	end
	local groupIdx = TopCareerGuessInfoFrame:GetClientUserData( 0 )
	local idx = 0
	for i = 1, MAX_SHOW_GUESS_INFO_NUM do
		local btn = getglobal( "TopCareerGuessInfoFrameInfoButton"..i )
		if btn:IsChecked() then
			local index = (curGuessInfoPage-1)*18+i
			idx = t_GuessGroupInfos[index]["idx"]
		end
	end
	if groupIdx == 0 then
		ShowMidTips( "��ѡ��ְҵ��" )
		return
	elseif idx == 0 then
		ShowMidTips( "��ѡ��֧�ֵ����" )
		return
	end
	GameTopCareer:SelectTopCareerWiner( groupIdx - 1, idx - 1 )
	t_SelectWiners[groupIdx] = GameTopCareer:getGuessGroupNameByIndex( idx - 1 )
	UpdateTopCareerGuessFrame()
	TopCareerGuessInfoFrame:Hide()
end
function GetGuessInfoColor( idx )
	for i = 1, MAX_CHAMPION_NUM do
		if t_SelectWiners[i] == t_GuessGroupInfos[idx]["name"] then
			return { 255, 115, 0 }
		end
	end
	return { 255, 255, 190 }
end
function SetGuessInfos()
	GetGuessInfos()
	for i = 1, MAX_SHOW_GUESS_INFO_NUM do
		local btn = getglobal( "TopCareerGuessInfoFrameInfoButton"..i )
		btn:Disable()
		btn:DisChecked()
		local btnName = getglobal( btn:GetName().."Name" )
		btnName:SetText( "" )
		local btnClan = getglobal( btn:GetName().."Clan" )
		btnClan:SetText( "" )
		local btnSupportNum = getglobal( btn:GetName().."SupportNum" )
		btnSupportNum:SetText( "" )
	end
	
	local nEnd  = GameTopCareer:getGuessGroupNum() > curGuessInfoPage*18 and curGuessInfoPage*18 or GameTopCareer:getGuessGroupNum()
	
	for i = 1, nEnd - (curGuessInfoPage-1)*18 do
		local idx = (curGuessInfoPage-1)*18 + i
		local btn = getglobal( "TopCareerGuessInfoFrameInfoButton"..i )
		local color = GetGuessInfoColor( idx )
		local btnName = getglobal( btn:GetName().."Name" )
		btnName:SetText( t_GuessGroupInfos[idx]["name"] )
		btnName:SetTextColor( color[1], color[2], color[3] )
		local btnClan = getglobal( btn:GetName().."Clan" )
		btnClan:SetText( t_GuessGroupInfos[idx]["clanName"] )
		btnClan:SetTextColor( color[1], color[2], color[3] )
		local btnSupportNum = getglobal( btn:GetName().."SupportNum" )
		btnSupportNum:SetText( t_GuessGroupInfos[idx]["supportNum"] )
		btnSupportNum:SetTextColor( color[1], color[2], color[3] )
		btn:Enable()
	end
	
	local szPageInfo = curGuessInfoPage .."/".. math.ceil( GameTopCareer:getGuessGroupNum()/18 )
	TopCareerGuessInfoFrameCurPageBtnPageFont:SetText( szPageInfo )
end
function TopCareerGuessInfoFrame_OnShow()
	curGuessInfoPage = 1
	SetGuessInfos()
end
function TopCareerGuessInfoFrameSearchBtn_OnClick()
	local szSearch = TopCareerGuessInfoFrameSearchEdit:GetText()
	if szSearch == "" then return end
	local btn = nil
	for i = 1, table.getn( t_GuessGroupInfos ) do
		if szSearch == t_GuessGroupInfos[i]["name"] then
			curGuessInfoPage = math.ceil( i/18 )
			btn = getglobal( "TopCareerGuessInfoFrameInfoButton"..( i - (curGuessInfoPage-1)*18 ) )
		end
	end
	if btn == nil then
		ShowMidTips( "�������Ľ�ɫ��δ����ĩ���۷�������������ѡ��������ɫ��" )
		return
	end
	SetGuessInfos()
	btn:Checked()
end
function TopCareerGuessInfoFrameSearchBtn_OnEnter()
	SetGameTooltips( this:GetName(), "����������������Կ��ٲ������" )
end
function TopCareerGuessInfoFrame_FirstPageBtn_OnClick()
	curGuessInfoPage = 1
	SetGuessInfos()
end
function TopCareerGuessInfoFrame_PrePageBtn_OnClick()
	curGuessInfoPage = curGuessInfoPage - 1 > 1 and curGuessInfoPage - 1 or 1 
	SetGuessInfos()
end
function TopCareerGuessInfoFrame_NextPageBtn_OnClick()
	curGuessInfoPage = curGuessInfoPage + 1 <= math.ceil( GameTopCareer:getGuessGroupNum()/18 ) and curGuessInfoPage + 1 or math.ceil( GameTopCareer:getGuessGroupNum()/18 )
	SetGuessInfos()
end
function TopCareerGuessInfoFrame_LastPageBtn_OnClick()
	curGuessInfoPage = math.ceil( GameTopCareer:getGuessGroupNum()/18 )
	SetGuessInfos()
end
function TopCareerGuessInfoFrameNameTitle_OnClick()
	if sortElem == "name" then
		sortOrder = not sortOrder
	else
		sortElem = "name"
		sortOrder = true
	end
	SetGuessInfos()
end
function TopCareerGuessInfoFrameClanTitle_OnClick()
	if sortElem == "clanName" then
		sortOrder = not sortOrder
	else
		sortElem = "clanName"
		sortOrder = true
	end
	SetGuessInfos()
end
function TopCareerGuessInfoFrameSupportNum_OnClick()
	if sortElem == "supportNum" then
		sortOrder = not sortOrder
	else
		sortElem = "supportNum"
		sortOrder = true
	end
	SetGuessInfos()
end
--------------------------------------�·�Ư�Ʊ�����ʾ�������----------------------------------------
function AddTopCareerInviteJoinTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( TOP_CAREER_INVITE_ACTIVITY ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = TOP_CAREER_INVITE_ACTIVITY, frame = "TopCareerInviteJoinFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\MoShiDianFeng.tga", index = 0 } );
	end
end
function RemoveTopCareerInviteJoinTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( TOP_CAREER_INVITE_ACTIVITY );	
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex(  nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	if TopCareerInviteJoinFrame:IsShown() then
		TopCareerInviteJoinFrame:Hide();
	end
end
function TopCareerInviteJoinResult( nflag )
	if nflag == 1 then
		TopCareerInviteJoinFrame:Hide();
		RemoveTopCareerInviteJoinTips()
	elseif nflag == 2 then
	elseif nflag == 3 then
		RemoveTopCareerInviteJoinTips()
	end
end
function TopCareerInviteJoinFrameEnterBtn_OnClick()
	MessageBox( "��ʾ", "����ĩ���۷���������Ҫ��ȡ#B"..MOSHI_DIANFENG_BAOMING_MONEY_TOLUA.."����#n�����ѣ�ȷ��������" )
	MessageBoxFrame:SetClientString( "ĩ���۷屨��" )
end
function TopCareerInviteJoinFrame_OnLoad()
	TopCareerInviteJoinFrameInfo:SetText( "ĩ���۷����ڽ��ܱ����;�����,����Ҫ�������ʿǰ��#Lĩ������ʹ@@[303063,1000]#n�����б����;��£�(����ʱ�䣺������19��30)", 255, 255, 190 )
end
function TopCareerInviteJoinFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
end
--------------------------------------�·�Ư�ƴ�����ʾ�������----------------------------------------
function AddTopCareerTransJoinTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( TOP_CAREER_TRANS_ACTIVITY ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = TOP_CAREER_TRANS_ACTIVITY, frame = "TopCareerTransFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\MoShiDianFeng.tga", index = 0 } );
	end
end
function RemoveTopCareerTransJoinTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( TOP_CAREER_TRANS_ACTIVITY );	
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex(  nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	if TopCareerTransFrame:IsShown() then
		TopCareerTransFrame:Hide();
	end
end
function TopCareerTransResult( nflag )
	if nflag == 1 then
		TopCareerTransFrame:Hide();
		RemoveTopCareerTransJoinTips()
		TopCareerReadyGuidFrame:Show()
		TopCareerFrame:Hide()
		TopCareerRuleFrame:Hide()
		TopCareerGuessFrame:Hide()
		TopCareerGuessInfoFrame:Hide()
	elseif nflag == 2 then
	else
		RemoveTopCareerTransJoinTips()
	end
end
function TopCareerTransFrameTransBtn_OnClick()
	GameTopCareer:requestTopCareerTrans()
end
function TopCareerTransFrame_OnLoad()
	TopCareerTransFrameInfo:SetText( "�۷�֮ս����10���Ӻ���,�뾡�����׼�����صȴ�,������δ����׼�����ص���Ϊ�Զ�������", 255, 255, 190 )
end
function TopCareerTransFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
end
----------------------------------------------ƥ��ɹ����----------------------------------------
local szInfo = "���ֻ�ȡ�ɹ���%dS�󽫴������������ؿ�ʼ�����������ñ���׼����"
function SetTopCareerSucMatchState( nflag )
	if nflag == 1 then
		TopCareerSucMatchFrame:Show()
		TopCareerSucMatchFrame:SetClientUserData( 0, os.clock() )
		TopCareerSucMatchFrameInfo:SetText( string.format( szInfo, 20 ), 255, 255, 190 )
	elseif nflag == 2 then
		TopCareerSucMatchFrame:Hide()
	end
end
function TopCareerSucMatchFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
end
function TopCareerSucMatchFrame_OnUpdate()
	local nPassTime = os.clock() - this:GetClientUserData( 0 )
	if 20 - nPassTime < 0 then
		TopCareerSucMatchFrame:Hide()
	end
	TopCareerSucMatchFrameInfo:SetText( string.format( szInfo, 20 - nPassTime ), 255, 255, 190 )
end
---------------------------------------------ĩ���۷�ʤ��С���------------------------------------
local t_TopCareerResultTex ={
	--δ��ʼ
	[0] = { path = "uires\\ui\\mask\\TouMingTongDao.tga", UV = { x = 0, y = 0, width = 1, height = 1 } },
	--�ɹ�	
	[1] = { path = "uires\\ZhuJieMian\\5.dds", UV = { x = 936, y = 27, width = 18, height = 18 } },
	--ʧ��	
	[2] = { path = "uires\\ZhuJieMian\\5.dds", UV = { x = 956, y = 27, width = 18, height = 18 } },
}
local nLeaveTime = 0
local CurFightIdx = 0
function SetTopCareerWinInfo( nResult1,nResult2,nResult3 )
	local t_result = { nResult1, nResult2, nResult3 }
	CurFightIdx = 0
	for i = 1, table.getn( t_result ) do
		local resultTex = getglobal( "TopCareerWinInfoFrameResult"..i )
		local index = t_result[i]
		local texInfo = t_TopCareerResultTex[index]
		resultTex:SetTexture( texInfo["path"] )
		resultTex:SetTexUV( texInfo["UV"].x, texInfo["UV"].y, texInfo["UV"].width, texInfo["UV"].height )
		if t_result[i] == 0 and CurFightIdx == 0 then
			CurFightIdx = i
		end
	end
end
function TopCareerWinInfoFrame_OnUpdate()
	local leaveTime = GetPworldLeaveTime() > 0 and GetPworldLeaveTime() or 0
	TopCareerWinInfoFrameTime:SetText( "��"..CurFightIdx.."��:".. math.ceil(leaveTime) .."s" )
end
function TopCareerWinInfoFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_MAP" )
end
function TopCareerWinInfoFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		if MapShower:getRealMapId() == DIANFENG_PWORLD_ID_TOLUA*100 then
			TopCareerWinInfoFrame:Show()
		else
			TopCareerWinInfoFrame:Hide()
		end
	end
end
-----------------------------------------׼����ͼ�ȴ�������ʾ���----------------------------------
local szReadyGuidInfo = [[#B          ���ѽ������׼����ͼ�������ĵȴ���
#B                ����������19��30��׼ʱչ����
#cffffbe	��3��2ʤ��ʧ����ֱ����̭���֣�
#cffffbe	��ÿ��ʱ��300S������ʱ��˫����ʧ�ܣ�
#cffffbe	������ڼ䲻��ʹ�û�е�ʹ��;��ᣡ
#cffffbe	������ڼ�������Ϊ����������
#cffffbe	�������������ǰ��ʱ���#B[�������]#n����ȡ������]]
local t_TopCareerReadyMaps = {
	SHZ_READY_MAP_ID_TOLUA, ZHZ_READY_MAP_ID_TOLUA, LSZ_READY_MAP_ID_TOLUA, HMZ_READY_MAP_ID_TOLUA,
	JNZ_READY_MAP_ID_TOLUA, LHS_READY_MAP_ID_TOLUA, SYZ_READY_MAP_ID_TOLUA, YLS_READY_MAP_ID_TOLUA,
}
function TopCareerReadyGuidFrame_OnLoad()
	this:RegisterEvent( "GE_ENTER_MAP" )
	TopCareerReadyGuidFrameInfo:SetText( szReadyGuidInfo, 255,255,190 )
end
function TopCareerReadyGuidFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		for i = 1, table.getn( t_TopCareerReadyMaps ) do
			if MapShower:getRealMapId() == t_TopCareerReadyMaps[i] then
				return
			end
		end
		TopCareerReadyGuidFrame:Hide()
	end
end
function TopCareerReadyGuidFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
end
