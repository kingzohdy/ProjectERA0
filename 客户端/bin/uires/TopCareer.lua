local t_TopCareerTexture = 
-- 根据职业来设置职业图片
{
	-- 战士双手  征服者
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 224, y = 455, width = 34, height = 34 } },
	-- 战士单手  守护者
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 292, y = 421, width = 34, height = 34 } },

	-- 祭司双手	驭雷师
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 258, y = 421, width = 34, height = 34 } },
	-- 祭司单手	圣佑者
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 224, y = 421, width = 34, height = 34 } },

	-- 枪手单手  猎杀者
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 326, y = 387, width = 34, height = 34 } },
	-- 枪手双手	毁灭者
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 292, y = 387, width = 34, height = 34 } },

	-- 法师双手  炼魂师
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 224, y = 387, width = 34, height = 34 } },
	-- 法师单手  聚能者
	{ path = "uires\\ZhuJieMian\\5.dds", UV = { x = 258, y = 387, width = 34, height = 34 } },
};
local t_RaceToButtonIdx = 
-- 根据职业来设置职业图片
{
	-- 战士							-- 战士单手  守护者				-- 战士双手  征服者
	[g_nCAREER_WARRIOR]			= 1,[g_nCAREER_WARRIOR_SINGEL]	= 2,[g_nCAREER_WARRIOR_DOUBLE]	= 1,

	-- 祭师							-- 祭司单手	圣佑者				-- 祭司双手	驭雷师
	[g_nCAREER_CHURCH]			= 3,[g_nCAREER_CHURCH_SINGEL]	= 4,[g_nCAREER_CHURCH_DOUBLE]	= 3,

	-- 枪手							-- 枪手单手  猎杀者				-- 枪手双手	毁灭者
	[g_nCAREER_GUNER]			= 5,[g_nCAREER_GUNER_SINGEL]	= 5,[g_nCAREER_GUNER_DOUBLE]	= 6,

	-- 法师							-- 法师单手  聚能者				-- 法师双手  炼魂师
	[g_nCAREER_MAGICOR]			= 7,[g_nCAREER_MAGICOR_SINGEL]	= 8,[g_nCAREER_MAGICOR_DOUBLE]	= 7,
};

local MAX_CHAMPION_NUM = 8
local t_LastChampions = {}			--用于记录上届获得冠军的玩家信息
local nRoleRotate = 0
local nRoleAngle = 0
function OpenTopCareerFrame( nStatus )
	--设置好上届冠军的名字
	for i = 1, MAX_CHAMPION_NUM do		
		t_LastChampions[i] = GameTopCareer:getCareerChampionName(i-1)
	end
	local time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*5+3600*19.5 )
	--如果当前时间超过，重新设置时间
	if getServerTime().sec > GameTopCareer:getActiveTime()+3600*24*5+3600*20.5 then
		time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*19+3600*20.5 )
	end
	local szTime = string.format( "%d-%d-%d 19:30 ", time.year, time.month, time.day, time.hour, time.min ) .. string.format( "(%s)", tWeekDayName[time.wday] )
	TopCareerFrameTime:SetText( "比赛开始时间："..szTime )
	--初始化模型旋转信息
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
		--设置冠军名称和所属公会
		local btnName = getglobal( btn:GetName().."Name" )
		local name = GameTopCareer:getCareerChampionName(i-1) == "" and "冠军虚位以待" or GameTopCareer:getCareerChampionName(i-1)
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
	MessageBox( "提示", "报名末世巅峰争霸赛需要收取#B"..MOSHI_DIANFENG_BAOMING_MONEY_TOLUA.."银币#n报名费；确定报名？" )
	MessageBoxFrame:SetClientString( "末世巅峰报名" )
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
---------------------------------------------巅峰规则面板---------------------------------------------------------
local TopCareerRules = [[#cffffbe规则介绍：
#cffffbe	1. 报名时间：#B%s至%s#n
#cffffbe	2. 参加比赛要求：#B等级≥65级#n
#cffffbe	3.比赛开始时间：#B%s#n
#cffffbe	4.赛制：3局2胜制；失败方直接淘汰
#cffffbe	5.比赛时不能使用怪物机械和各种传送卷轴
#B	6.比赛时间段内离线时间超过2分钟视为放弃]]

function TopCareerRuleFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
	local beginTime = os.date( "*t", GameTopCareer:getActiveTime() )
	--如果当前时间超过，重新设置时间
	if getServerTime().sec > GameTopCareer:getActiveTime()+3600*24*5+3600*20.5 then
		beginTime = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*14 )
	end
	local szBeginTime = string.format( "%d-%d-%d", beginTime.year, beginTime.month, beginTime.day )
	local time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*5+3600*19.5 )
	--如果当前时间超过，重新设置时间
	if getServerTime().sec > GameTopCareer:getActiveTime()+3600*24*5+3600*20.5 then
		time = os.date( "*t", GameTopCareer:getActiveTime()+3600*24*19+3600*19.5 )
	end
	local szTime = string.format( "%d-%d-%d %d:%d ", time.year, time.month, time.day, time.hour, time.min ) 
	TopCareerRuleFrameInfo:SetText( string.format( TopCareerRules, szBeginTime, szTime, szTime.. string.format( "(%s)", tWeekDayName[time.wday] ) ), 255, 255, 190 )
end
function TopCareerRuleFrame_OnClick()
	MessageBox( "提示", "报名末世巅峰争霸赛需要收取#B"..MOSHI_DIANFENG_BAOMING_MONEY_TOLUA.."银币#n报名费；确定报名？" )
	MessageBoxFrame:SetClientString( "末世巅峰报名" )
end
------------------------------------------竞猜职业选择面板-------------------------------------------------------
local TopCareerGuessInfos = [[#cffffbe	1.对本届巅峰之战的8个职业冠军进行竞猜
#cffffbe	2.猜中者最少可得到1w银币并且可以分享奖池
#cffffbe	3.竞猜消耗%d银币
#cffffbe	4.上届各职业组冠军：]]
local t_SelectWiners = {"选择支持的征服","选择支持的守护","选择支持的驭雷","选择支持的圣佑",
						"选择支持的猎杀","选择支持的毁灭","选择支持的炼魂","选择支持的聚能",}
local t_TopCareerTypeName = { "[征服]", "[守护]", "[驭雷]", "[圣佑]", "[猎杀]", "[毁灭]", "[炼魂]", "[聚能]", }
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
			lastChampionFont:SetText( "冠军虚位以待"..t_TopCareerTypeName[i] )
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
		t_SelectWiners = {"选择支持的征服","选择支持的守护","选择支持的驭雷","选择支持的圣佑",
						"选择支持的猎杀","选择支持的毁灭","选择支持的炼魂","选择支持的聚能",}
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
		ShowMidTips( "您已经竞猜过了" )
		return
	end
	for i = 1, MAX_CHAMPION_NUM do
		if not GameTopCareer:IsTopCareerSelected( i - 1 ) then
			ShowMidTips( "只有全部职业都选择了支持者，才能竞猜" )
			return
		end
	end
	MessageBox( "提示", "巅峰竞猜需要消耗"..MOSHI_DIANFENG_JINGCAI_MONEY_TOLUA.."银币，确认竞猜？\n#B（末世巅峰争霸赛比赛时间：本周六19：30）#n" )
	MessageBoxFrame:SetClientString("巅峰冠军竞猜");
end
function SetTopCareerGuessShow( allMoney, nflag )
	TopCareerGuessFrameMoneyAward:SetText( string.format( "#cffffbe奖池：#cfff669%d#n#cffffbe银币", allMoney ), 255, 255, 190 )
	if nflag == 2 or nflag == 4 then
		HasGuessed = false
		t_SelectWiners = {"选择支持的征服","选择支持的守护","选择支持的驭雷","选择支持的圣佑",
						"选择支持的猎杀","选择支持的毁灭","选择支持的炼魂","选择支持的聚能",}
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
--------------------------------------选择竞猜对象面板-----------------------------------------------
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
	TopCareerGuessInfoFrameTitle:SetText( t_TopCareerTypeName[groupIdx].."组竞猜" )
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
		ShowMidTips( "您已经竞猜过了" )
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
		ShowMidTips( "请选择职业组" )
		return
	elseif idx == 0 then
		ShowMidTips( "请选择支持的玩家" )
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
		ShowMidTips( "您搜索的角色还未报名末世巅峰争霸赛，您可选择其他角色！" )
		return
	end
	SetGuessInfos()
	btn:Checked()
end
function TopCareerGuessInfoFrameSearchBtn_OnEnter()
	SetGameTooltips( this:GetName(), "输入玩家姓名，可以快速查找玩家" )
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
--------------------------------------下方漂移报名提示关联面板----------------------------------------
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
	MessageBox( "提示", "报名末世巅峰争霸赛需要收取#B"..MOSHI_DIANFENG_BAOMING_MONEY_TOLUA.."银币#n报名费；确定报名？" )
	MessageBoxFrame:SetClientString( "末世巅峰报名" )
end
function TopCareerInviteJoinFrame_OnLoad()
	TopCareerInviteJoinFrameInfo:SetText( "末世巅峰正在接受报名和竞猜中,请需要参与的勇士前往#L末世接引使@@[303063,1000]#n处进行报名和竞猜！(截至时间：本周六19：30)", 255, 255, 190 )
end
function TopCareerInviteJoinFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
end
--------------------------------------下方漂移传送提示关联面板----------------------------------------
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
	TopCareerTransFrameInfo:SetText( "巅峰之战将在10分钟后开赛,请尽快进入准备场地等待,开赛后还未进入准备场地的视为自动放弃！", 255, 255, 190 )
end
function TopCareerTransFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 )
end
----------------------------------------------匹配成功面板----------------------------------------
local szInfo = "对手获取成功，%dS后将传送至比赛场地开始比赛，请做好比赛准备！"
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
---------------------------------------------末世巅峰胜负小面板------------------------------------
local t_TopCareerResultTex ={
	--未开始
	[0] = { path = "uires\\ui\\mask\\TouMingTongDao.tga", UV = { x = 0, y = 0, width = 1, height = 1 } },
	--成功	
	[1] = { path = "uires\\ZhuJieMian\\5.dds", UV = { x = 936, y = 27, width = 18, height = 18 } },
	--失败	
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
	TopCareerWinInfoFrameTime:SetText( "第"..CurFightIdx.."局:".. math.ceil(leaveTime) .."s" )
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
-----------------------------------------准备地图等待引导提示面板----------------------------------
local szReadyGuidInfo = [[#B          您已进入比赛准备地图，请耐心等待！
#B                （比赛将在19：30分准时展开）
#cffffbe	●3局2胜，失败者直接淘汰出局！
#cffffbe	●每局时长300S，超过时间双方都失败！
#cffffbe	●比赛期间不能使用机械和传送卷轴！
#cffffbe	●比赛期间离线视为放弃比赛！
#cffffbe	●比赛结束后请前往时间城#B[卡拉洛克]#n处领取奖励！]]
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
