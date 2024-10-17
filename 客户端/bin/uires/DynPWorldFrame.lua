local nHardSelect	= 0;
local nLvSelect		= 0;
local MAX_TIP_NUM	= 3;	
local MAX_FACE_NUM	= 3;

local t_DynWorldLevelTips =
	{	
		"副本等级取决于你队伍中最低等级玩家的等级",
		"副本等级取决于你队伍中玩家等级的平均值",
		"副本等级取决于你队伍中最高等级玩家的等级"
	}

local t_DynDiffTips =
	{
		"简单",
		"普通",
		"困难",
		"艰难",
		"地狱"
	}

function OnHardFaceSelect()
	for i = 1, MAX_FACE_NUM do
		local selectText = getglobal( "DyPWorldHardSelectFace"..i.."Select" );
		local normalText = getglobal( "DyPWorldHardSelectFace"..i.."Normal" );
		selectText:Hide();
		normalText:Show();
		if i == this:GetClientID() then		
			DynCurrentDiffcultyValue:SetText( t_DynDiffTips[i] );
			SelectHard( i );
			selectText:Show();
			normalText:Hide();
			nHardSelect = i - 1;
		end
	end
end

function SelectHard( nIndex )
	DynCurrentDiffcultyValue:SetText( t_DynDiffTips[nIndex] );
	for i = 1, 5 do
		local selectText	= getglobal( "DyPWorldHardSelect"..i.."Select" );
		local selectNormal	= getglobal( "DyPWorldHardSelect"..i.."Normal" );
		selectText:Hide();
		selectNormal:Show();
		if i == nIndex then
			SelectHardFace( nIndex );
			selectText:Show();
			selectNormal:Hide();
			nHardSelect = i - 1;
		end
	end
end

-- 计算副本等级，若选最低，则以队伍成员最低等级作为难度，若为平均则以平局队员等级为难度等级，若为最难则以最高等级队员为基准
function getDynLevel( nSelectLvIndex )
	local i			= 0;
	local nMemTotalLv	= 0;
	local nMemHighestLv	= 0;
	local nMemAverLv	= 0;
	local nMemLowestLv	= 10000;
	local TeamInfo		= TeamManager:getTeamInfo();
	if TeamInfo == nil then
		return;
	end
	local nMemberNum = TeamInfo.MemberNum;
	if nMemberNum == nil then
		return;
	end
	nMemLowestLv		= TeamInfo.TeamMember[0].Level;
	nMemHighestLv		= TeamInfo.TeamMember[0].Level;
	for i = 0, nMemberNum - 1 do
		nMemTotalLv = nMemTotalLv + TeamInfo.TeamMember[i].Level;
		if nMemHighestLv < TeamInfo.TeamMember[i].Level then
			nMemHighestLv = TeamInfo.TeamMember[i].Level;
		else
			if nMemLowestLv > TeamInfo.TeamMember[i].Level then
				nMemLowestLv = TeamInfo.TeamMember[i].Level;
			end
		end
	end
	nMemAverLv = math.ceil( nMemTotalLv / nMemberNum );
	if nSelectLvIndex == 1 then
		return nMemLowestLv;
	end

	if nSelectLvIndex == 2 then
		return nMemAverLv;
	end

	if nSelectLvIndex == 3 then
		return nMemHighestLv;
	end
end

function OnLevelSelect()
	for i = 1, 3 do
		local selectText = getglobal( "DyPWorldLvSelect"..i.."Select" );
		local normalText = getglobal( "DyPWorldLvSelect"..i.."Normal" );
		if selectText == nil or normalText == nil then
			return;
		end
		selectText:Hide();
		normalText:Hide();
		if i == this:GetClientID() then
			selectText:Show();
			if i == 1 then
				nLvSelect = 0;
				DynCurrentLevelValue:SetText( getDynLevel( i ).."级" );
			end
			if i == 2 then 
				nLvSelect = 2;
				DynCurrentLevelValue:SetText( getDynLevel( i ).."级" );
			end
			if i == 3 then
				nLvSelect = 1;
				DynCurrentLevelValue:SetText( getDynLevel( i ).."级" );
			end
		else
			normalText:Show();
		end
	end
end

function openPWorldSelect( PWorldId )
	DynPWorldFrame:Show();
end

function Close_DynPWorldFrame()
	DynPWorldFrame:Hide();
end

function Accept()
	GameDynPWorld:selectDynPWorld( nHardSelect, nLvSelect, 0 );
	DynPWorldFrame:Hide();
end

function DynPWorldFrame_OnShow()
	for i = 1, MAX_FACE_NUM do
		local selectText	= getglobal( "DyPWorldHardSelect"..i.."Select" );
		local selectFaceText	= getglobal( "DyPWorldHardSelectFace"..i.."Select" );
		local normalFaceText	= getglobal( "DyPWorldHardSelectFace"..i.."Normal" );
		selectText:Hide();
		selectFaceText:Hide();
		normalFaceText:Hide();
	end

	nLvSelect = 2;
	DynCurrentLevelValue:SetText( getDynLevel( 2 ).."级" );
	SelectHard( 1 );	
end

function SelectHardFace( nIndex )
	for i = 1, MAX_FACE_NUM do
		local selectText = getglobal( "DyPWorldHardSelectFace"..i.."Select" );
		local normalText = getglobal( "DyPWorldHardSelectFace"..i.."Normal" );
		selectText:Hide();
		normalText:Show();
		if i == nIndex then
			selectText:Show();
			normalText:Hide();
			nHardSelect = i - 1;
		end
	end
end

function GameLeveltipOnShow()
	local nSelect = this:GetClientUserData( 0 );
	if nSelect == nil or nSelect < 1 or nSelect > MAX_TIP_NUM then
		return;
	end
	GameLeveltipRichText:SetText( t_DynWorldLevelTips[nSelect], 161, 138, 82 );

	local szSelectBtnName	= this:GetClientString();
	local szSelectBtn	= getglobal( szSelectBtnName );
	GameLeveltipRichText:Show();
	DynLevelTips:SetPoint( "bottomleft", szSelectBtnName, "bottomleft", 42,  -42 );
end

function GameLeveltipOnHide()
	this:SetClientUserData( 0, 0 );
end

function LevelSelectBtn1_OnEnter()
	local szName = this:GetName();
	DynLevelTips:SetClientUserData( 0, 1 );
	DynLevelTips:SetClientString( szName );
	DynLevelTips:Show();
end

function LevelSelectBtn2_OnEnter()
	local szName = this:GetName();
	DynLevelTips:SetClientUserData( 0, 2 );
	DynLevelTips:SetClientString( szName );
	DynLevelTips:Show();
end

function LevelSelectBtn3_OnEnter()
	local szName = this:GetName();
	DynLevelTips:SetClientUserData( 0, 3 );
	DynLevelTips:SetClientString( szName );
	DynLevelTips:Show();
end