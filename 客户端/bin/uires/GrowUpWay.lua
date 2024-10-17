----------------------------�ɳ�֮·--------------------------------
local GrowUpWayAttentionID = 0;
local GrowUpWayAttentionTaskID = 0;
local TASK_MAX_NUM = 5;
local ACHIEVEMENT_CATEGORY_NUM = 7;
--�ɳ�֮·Ŀ������ �� ���������״�� ��Ӧ��
local t_AchievementCategoryList = {
--Ŀ������	������1			������2			������3			������4			������5
	[1] = { [1] = false,	[2] = false,	[3] = false },
	[2] = { [5] = false,	[22] = false,	[9] = false,	[10] = false, },
	[3] = { [6] = false,	[23] = false,	[14] = false,	[15] = false, },
	[4] = { [17] = false,	[11] = false,	[21] = false,	[13] = false, },
	[5] = { [7] = false,	[20] = false,	[33] = false, },
	[6] = { [25] = false,	[26] = false,	[27] = false,	[8] = false,},
	[7] = { [4] = false,	[16] = false,	[19] = false,	[24] = false,	[34] = false, },
--	[8] = { [28] = false,	[29] = false,	[30] = false,	[31] = false,	[32] = false },
};
local t_categoryIds = {
	[1] = { 1,	2,	3, },
	[2] = { 5,	22,	9,	10, },
	[3] = { 6,	23,	14,	15, },
	[4] = { 17,	11,	21, 13, },
	[5] = { 7,	20,	33, },
	[6] = { 25,	26,	27,	8, },
	[7] = { 4,	16,	19, 24, 34, },
--	[8] = { 28,	29,	30,	31,	32 },

};
local t_levelTexUV = {
	[0] = { x = 0, y = 0, w = 1, h = 1 },
	[10] = { x = 0, y = 0, w = 50, h = 15 },
	[20] = { x = 50, y = 0, w = 50, h = 15 },
	[30] = { x = 100, y = 0, w = 50, h = 15 },
	[35] = { x = 150, y = 0, w = 50, h = 15 },
	[40] = { x = 0, y = 17, w = 50, h = 15 },
	[50] = { x = 50, y = 17, w = 50, h = 15 },
	[60] = { x = 100, y = 17, w = 50, h = 15 },
	[65] = { x = 150, y = 17, w = 50, h = 15 },
}
local t_levelCategoryList = {
	{ ["min"] = 10, ["max"] = 19 },
	{ ["min"] = 20, ["max"] = 29 },
	{ ["min"] = 30, ["max"] = 39 },
	--{ ["min"] = 30, ["max"] = 34 },
	--{ ["min"] = 35, ["max"] = 39 },
	{ ["min"] = 40, ["max"] = 49 },
	{ ["min"] = 50, ["max"] = 59 },
	{ ["min"] = 60, ["max"] = 64 },
	{ ["min"] = 65, ["max"] = 120 },
}
local szRichText = [[1.����ﵽ10����ͨ����ɯ��ָ��������ʱ���
2.�ڰݷù����ܴ�ʦ��ʳ�����˺���ͻ����������ӵ�����������
3��ͨ������ǰ���ʺ絺�������ҵ����������[�Ը����]������ͻ���һֻ�ɰ�������ǹ����
]]
function ShorcutOpen()
	if GrowUpWayFrame:IsShown() then
		GrowUpWayFrame:Hide();
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local level = mainplayer:getLv();
	for i = 1, table.getn( t_levelCategoryList ) do
		if level >= t_levelCategoryList[i]["min"] and level <= t_levelCategoryList[i]["max"] then
			GrowUpWayAttentionID = i;
			GrowUpWayFrame:Show();
			return;
		end
	end
	ShowMidTips( "��ȥ�����������ɣ�10�����ܴ򿪳ɳ�֮·Ӵ��" );
end
function SetTipsPositon()
	local setUnfinishedTips = false;
	GrowUpWayFrameTaskInfoFont:Hide();
	GrowUpWayFrameCurTipsBtn:Hide();
	--Ѱ��һ��û����ɵ���������ʾtips
	for index in ipairs( t_categoryIds[GrowUpWayAttentionID] ) do
		local ID = t_categoryIds[GrowUpWayAttentionID][index];
		local mainplayer = ActorMgr:getMainPlayer();	
		local AchievementDef = mainplayer:getAchievementDef( ID, GrowUpWayAttentionID );
		if t_AchievementCategoryList[GrowUpWayAttentionID][ID] == false then
			if not setUnfinishedTips then
				setUnfinishedTips = true;
				local szAttr = "";
				local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
				--[[
				for i = 1, table.getn( t_tagRESULT ) do
					if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
						szAttr = szAttr..t_tagRESULT[i][2].."+"..AchievementDef.Result.ResultVal1;
					end
				end
				--]]
				GrowUpWayFrameDescRich:SetText( AchievementDef.GuideDes, 0, 255, 0 );
				GrowUpWayFrameTaskInfoFont:Show();
				GrowUpWayFrameCurTipsBtn:Show();
				GrowUpWayFrameTaskInfoFont:SetText( szAttr );
				GrowUpWayFrameTaskInfoFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -38+65*index, 344 );
				GrowUpWayFrameCurTipsBtnDescFont:SetText( AchievementDef.Name );
				GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -50+65*index, 260 );
				GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", -7+65*index, 285  );
				if index == 5 then
					GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -100+65*index, 260 );
					GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", -12+65*index, 285  );
				end
				return;
			end
		end
	end
	--���ȫ�������Ĭ����ʾ��һ��
	local ID = t_categoryIds[GrowUpWayAttentionID][1];
	local mainplayer = ActorMgr:getMainPlayer();
	local AchievementDef = mainplayer:getAchievementDef( ID, GrowUpWayAttentionID );
	local szAttr = "";
	local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
	if DesResult ~= nil then
		szAttr = szAttr..DesResult[2].."+"..AchievementDef.Result.ResultVal1;
	end
	--[[
	for i = 1, table.getn( t_tagRESULT ) do
		if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
			szAttr = szAttr..t_tagRESULT[i][2].."+"..AchievementDef.Result.ResultVal1;
		end
	end
	--]]
	GrowUpWayFrameDescRich:SetText( AchievementDef.GuideDes, 0, 255, 0 );
	GrowUpWayFrameTaskInfoFont:Show();
	GrowUpWayFrameCurTipsBtn:Show();
	GrowUpWayFrameTaskInfoFont:SetText( szAttr );
	GrowUpWayFrameTaskInfoFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 27, 344 );
	GrowUpWayFrameCurTipsBtnDescFont:SetText( AchievementDef.Name );
	GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 15, 260 );
	GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", 52, 285  );
end
function GrowUpWayFrame_onShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	GrowUpWayFrame_onUpdate();
	if GrowUpWayAttentionTaskID ~= 0 then
		local mainplayer = ActorMgr:getMainPlayer();
		local AchievementDef = mainplayer:getAchievementDef(  GrowUpWayAttentionTaskID, GrowUpWayAttentionID  );
		local szAttr = "";
		local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
		if DesResult ~= nil then
			szAttr = szAttr..DesResult[2].."+"..AchievementDef.Result.ResultVal1;
		end
		--[[
		for i = 1, table.getn( t_tagRESULT ) do
			if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
				szAttr = szAttr..t_tagRESULT[i][2].."+"..AchievementDef.Result.ResultVal1;
			end
		end
		--]]
		local index = 0;
		for i = 1, table.getn( t_categoryIds[GrowUpWayAttentionID] ) do
			if t_categoryIds[GrowUpWayAttentionID][i] == GrowUpWayAttentionTaskID then
				index = i;
			end
		end
		if index == 0 then return end
		GrowUpWayFrameDescRich:SetText( AchievementDef.GuideDes, 0, 255, 0 );
		GrowUpWayFrameTaskInfoFont:Show();
		GrowUpWayFrameCurTipsBtn:Show();
		GrowUpWayFrameTaskInfoFont:SetText( szAttr );
		GrowUpWayFrameTaskInfoFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -38+65*index, 344 );
		GrowUpWayFrameCurTipsBtnDescFont:SetText( AchievementDef.Name );
		GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -50+65*index, 260 );
		GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", -7+65*index, 285  );
		if index == 5 then
			GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -100+65*index, 260 );
			GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", -12+65*index, 285  );
		end
	else
		SetTipsPositon();
	end
end
function GrowUpWayFrame_onHide()
	GrowUpWayAttentionTaskID = 0;
	GrowUpWayAttentionID = 0;
	GameTooltipOnLeave();
end
function isAchievementCategoryFinish( Id )
	for index in ipairs( t_categoryIds[Id] ) do
		local ID = t_categoryIds[Id][index];
			if not t_AchievementCategoryList[Id][ID] then
				return false;
			end
	end
	return true;
end
function GrowUpWayFrame_onUpdate()
	local mainplayer = ActorMgr:getMainPlayer();
	GrowUpWayFrameLevelBtn2:SetSize( 84, 105 );
	GrowUpWayFrameLevelBtn2NameFont:Hide();
	GrowUpWayFrameLevelBtn2NameBigFont:Show();
	GrowUpWayFrameLevelBtn2IsFineshFont:Hide();
	GrowUpWayFrameLevelBtn2IsFineshBigFont:Show();
	GrowUpWayFrameLevelBtn2:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 190, 40 );
	local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( GrowUpWayAttentionID );
	local UV = t_levelTexUV[AchievementCategoryDef.Level];
	GrowUpWayFrameLevelBtn2Level:SetTexUV( UV.x, UV.y, UV.w, UV.h );
	GrowUpWayFrameLevelBtn2NameFont:SetText( AchievementCategoryDef.Name );
	GrowUpWayFrameLevelBtn2NameBigFont:SetText( AchievementCategoryDef.Name );
	GrowUpWayFrameLevelBtn2IconTexture:SetTexture( "uires\\TuPianLei\\WuPin\\"..AchievementCategoryDef.Icon..".tga" );
	GrowUpWayFrameLevelBtn2IconTexture:SetGray(  AchievementCategoryDef.Level > mainplayer:getLv() );
	GrowUpWayFrameLevelBtn2IsFineshFont:SetText( (isAchievementCategoryFinish(GrowUpWayAttentionID) and "�����" or "") );
	GrowUpWayFrameLevelBtn2IsFineshBigFont:SetText( (isAchievementCategoryFinish(GrowUpWayAttentionID) and "�����" or "") );
	if GrowUpWayAttentionID == 1 then
		if GrowUpWayFrameLeftPageBtn:IsEnable() then
			GrowUpWayFrameLeftPageBtn:SetGrayEx( true );
			GrowUpWayFrameLeftPageBtn:Disable();
		end
		UV = t_levelTexUV[0];
		GrowUpWayFrameLevelBtn1Level:SetTexUV( UV.x, UV.y, UV.w, UV.h );
		GrowUpWayFrameLevelBtn1NameFont:SetText( "" );
		GrowUpWayFrameLevelBtn1IconTexture:SetTexture( "uires\\TuPianLei\\WuPin\\10028.tga" );
		GrowUpWayFrameLevelBtn1IconTexture:SetGray(  false );
		GrowUpWayFrameLevelBtn1IsFineshFont:SetText( "" );
	else
		if not GrowUpWayFrameLeftPageBtn:IsEnable() then
			GrowUpWayFrameLeftPageBtn:SetGrayEx( false );
			GrowUpWayFrameLeftPageBtn:DisHighlight();
			GrowUpWayFrameLeftPageBtn:ClearPushState();
			GrowUpWayFrameLeftPageBtn:Enable();
		end
		local AchievementCategoryDefBefor = mainplayer:getAchievementCategoryDef( (GrowUpWayAttentionID -2 + ACHIEVEMENT_CATEGORY_NUM ) % ACHIEVEMENT_CATEGORY_NUM + 1 );
		UV = t_levelTexUV[AchievementCategoryDefBefor.Level];
		GrowUpWayFrameLevelBtn1Level:SetTexUV( UV.x, UV.y, UV.w, UV.h );
		GrowUpWayFrameLevelBtn1NameFont:SetText( AchievementCategoryDefBefor.Name );
		GrowUpWayFrameLevelBtn1IconTexture:SetTexture( "uires\\TuPianLei\\WuPin\\"..AchievementCategoryDefBefor.Icon..".tga" );
		GrowUpWayFrameLevelBtn1IconTexture:SetGray(  AchievementCategoryDefBefor.Level > mainplayer:getLv() );
		GrowUpWayFrameLevelBtn1IsFineshFont:SetText( (isAchievementCategoryFinish((GrowUpWayAttentionID -2 + ACHIEVEMENT_CATEGORY_NUM ) % ACHIEVEMENT_CATEGORY_NUM + 1) and "�����" or "") );
	end
	if GrowUpWayAttentionID == ACHIEVEMENT_CATEGORY_NUM then
		GrowUpWayFrameRightPageBtn:SetGrayEx( true );
		GrowUpWayFrameRightPageBtn:Disable();
		UV = t_levelTexUV[0];
		GrowUpWayFrameLevelBtn3Level:SetTexUV( UV.x, UV.y, UV.w, UV.h );
		GrowUpWayFrameLevelBtn3NameFont:SetText( "" );
		GrowUpWayFrameLevelBtn3IconTexture:SetTexture( "uires\\TuPianLei\\WuPin\\10028.tga" );
		GrowUpWayFrameLevelBtn3IconTexture:SetGray(  false );
		GrowUpWayFrameLevelBtn3IsFineshFont:SetText( "" );
	else
		local AchievementCategoryDefAfter = mainplayer:getAchievementCategoryDef( GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1 );
		if AchievementCategoryDefAfter.Level <= mainplayer:getLv() and not GrowUpWayFrameRightPageBtn:IsEnable() then
			GrowUpWayFrameRightPageBtn:SetGrayEx( false );
			GrowUpWayFrameRightPageBtn:DisHighlight();
			GrowUpWayFrameRightPageBtn:ClearPushState();
			GrowUpWayFrameRightPageBtn:Enable();
		elseif  AchievementCategoryDefAfter.Level > mainplayer:getLv() and GrowUpWayFrameRightPageBtn:IsEnable() then
			GrowUpWayFrameRightPageBtn:SetGrayEx( true );
			GrowUpWayFrameRightPageBtn:Disable();
		end
		UV = t_levelTexUV[AchievementCategoryDefAfter.Level];
		GrowUpWayFrameLevelBtn3Level:SetTexUV( UV.x, UV.y, UV.w, UV.h );
		GrowUpWayFrameLevelBtn3NameFont:SetText( AchievementCategoryDefAfter.Name );
		GrowUpWayFrameLevelBtn3IconTexture:SetTexture( "uires\\TuPianLei\\WuPin\\"..AchievementCategoryDefAfter.Icon..".tga" );
		GrowUpWayFrameLevelBtn3IconTexture:SetGray(  AchievementCategoryDefAfter.Level > mainplayer:getLv() );
		GrowUpWayFrameLevelBtn3IsFineshFont:SetText( (isAchievementCategoryFinish(GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1) and "�����" or "") );
	end	
	--������Ӧ����button
	local setUnfinishedTips = false;
	local icons = {}
	for index in ipairs( t_categoryIds[GrowUpWayAttentionID] ) do
		icons[index] = true;
		local taskBtn = getglobal( "GrowUpWayFrameTaskBtn"..index );
		local taskIcon = getglobal( taskBtn:GetName().."IconTexture" );
		local ID = t_categoryIds[GrowUpWayAttentionID][index];	
		local AchievementDef = mainplayer:getAchievementDef( ID, GrowUpWayAttentionID );
		taskIcon:SetTexture( "uires\\TuPianLei\\TouXiang\\GuaiWuTouXiang\\"..AchievementDef.Icon..".tga" );
		taskBtn:SetSize( 51*AchievementDef.IconRate/100, 51*AchievementDef.IconRate/100 );
		if t_AchievementCategoryList[GrowUpWayAttentionID][ID] == true then
			taskIcon:SetGray( false );
		else
			taskIcon:SetGray( true );
		end
		taskBtn:Show();
	end
	for i = 1, TASK_MAX_NUM do
		if not icons[i] then
			local taskBtn = getglobal( "GrowUpWayFrameTaskBtn"..i );
			local taskIcon = getglobal( taskBtn:GetName().."IconTexture" );
			taskIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
			taskBtn:SetSize( 51, 51 );
			taskBtn:Hide();
		end
	end
	--���ü�ͷͼ����ʾ
	for i = 2, TASK_MAX_NUM do
		taskBtn = getglobal( "GrowUpWayFrameTaskBtn"..i );
		ArrowTex = getglobal( "GrowUpWayFrameArrowTex"..( i - 1 ) );
		if taskBtn:IsShown() then
			ArrowTex:Show();
		else
			ArrowTex:Hide();
		end
	end
	--ͳ�����Լӳ�
	local t_TaskInfo = {};
	for i = 1, MAX_CATEGORY_ACHIEVEMENT_TOLUA do
		local ID = AchievementCategoryDef.AchievementID[i-1];
		if ID > 0 and t_AchievementCategoryList[GrowUpWayAttentionID][ID] then
			local AchievementDef = mainplayer:getAchievementDef( AchievementCategoryDef.AchievementID[i-1], GrowUpWayAttentionID );
			for j = 1,table.getn( t_tagRESULT ) do
				if t_tagRESULT[j][1] == AchievementDef.Result.ResultID then
					for k = 1, table.getn( t_TaskInfo )	do
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						if t_TaskInfo[k]["Attr"] == DesResult[2] then
							t_TaskInfo[k]["AttrVal"] = t_TaskInfo[k]["AttrVal"] + AchievementDef.Result.ResultVal1;
							break;
						end
						if k == table.getn( t_TaskInfo ) then
							t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
							table.insert( t_TaskInfo, t_oneTaskInfo );
						end
					end	
					if table.getn( t_TaskInfo ) == 0 then
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
						table.insert( t_TaskInfo, t_oneTaskInfo );
					end	
				end
			end
		end
	end
	local szInfo = "";
	for i = 1, table.getn( t_TaskInfo ) do
		szInfo = szInfo..t_TaskInfo[i]["Attr"].."+"..t_TaskInfo[i]["AttrVal"];
	end
	if table.getn( t_TaskInfo ) == 0 then
		szInfo = szInfo.."(δ����)";
	end
	GrowUpWayFrameCurInfoFont:SetText( szInfo );
	local t_TotalTaskInfo = {};
	for i = 1, MAX_CATEGORY_ACHIEVEMENT_TOLUA do
		local ID = AchievementCategoryDef.AchievementID[i-1];
		if ID > 0 then
			local AchievementDef = mainplayer:getAchievementDef( AchievementCategoryDef.AchievementID[i-1], GrowUpWayAttentionID );
			for j = 1,table.getn( t_tagRESULT ) do
				if t_tagRESULT[j][1] == AchievementDef.Result.ResultID then
					for k = 1, table.getn( t_TotalTaskInfo )	do
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						if t_TotalTaskInfo[k]["Attr"] == DesResult[2] then
							t_TotalTaskInfo[k]["AttrVal"] = t_TotalTaskInfo[k]["AttrVal"] + AchievementDef.Result.ResultVal1;
							break;
						end
						if k == table.getn( t_TotalTaskInfo ) then
							t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
							table.insert( t_TotalTaskInfo, t_oneTaskInfo );
						end
					end	
					if table.getn( t_TotalTaskInfo ) == 0 then
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
						table.insert( t_TotalTaskInfo, t_oneTaskInfo );
					end	
				end
			end
		end
	end
	szInfo = "";
	for i = 1, table.getn( t_TotalTaskInfo ) do
		szInfo = szInfo ..t_TotalTaskInfo[i]["Attr"].."+"..t_TotalTaskInfo[i]["AttrVal"];
	end
	GrowUpWayFrameFinalInfoFont:SetText( szInfo );
	if isAchievementCategoryFinish( GrowUpWayAttentionID ) then
		GrowUpWayFrameCurFont:SetText( "��ȫ�����" );
		GrowUpWayFrameCurFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 350, 300 );
		GrowUpWayFrameCurInfoFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 363, 315 );
		GrowUpWayFrameCurInfoFont:SetTextColor( 151, 45, 249 );
		GrowUpWayFrameFinalInfoFont:Hide();
		GrowUpWayFrameFinalFont:Hide();
	else
		GrowUpWayFrameCurFont:SetText( "��ǰ�ӳɣ�" );
		GrowUpWayFrameCurFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 350, 265 );
		GrowUpWayFrameCurInfoFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 363, 280 );
		GrowUpWayFrameCurInfoFont:SetTextColor( 255, 125, 0 );
		GrowUpWayFrameFinalInfoFont:Show();
		GrowUpWayFrameFinalFont:Show();
	end
end
function GrowUpWayFrameLeftPageBtn_onClick()
	if GrowUpWayAttentionID == 1 then return end
	GrowUpWayFrameLevelBtn2:SetSize( 52, 70 );
	GrowUpWayFrameLevelBtn2NameFont:Show();
	GrowUpWayFrameLevelBtn2NameBigFont:Hide();
	GrowUpWayFrameLevelBtn2IsFineshFont:Show();
	GrowUpWayFrameLevelBtn2IsFineshBigFont:Hide();
	GrowUpWayFrameLevelBtn2:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 207, 63 );
	GrowUpWayAttentionID = (GrowUpWayAttentionID -2 + ACHIEVEMENT_CATEGORY_NUM ) % ACHIEVEMENT_CATEGORY_NUM + 1;
	GameTooltipOnLeave();
	SetTipsPositon();
end
function GrowUpWayFrameLeftPageBtn_onUpdate()
end
function GrowUpWayFrameRightPageBtn_onClick()
	if GrowUpWayAttentionID == ACHIEVEMENT_CATEGORY_NUM then return end
	local mainplayer = ActorMgr:getMainPlayer();
	local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1 );
	if AchievementCategoryDef.Level > mainplayer:getLv() then
		return;
	end
	GrowUpWayFrameLevelBtn2:SetSize( 52, 70 );
	GrowUpWayFrameLevelBtn2NameFont:Show();
	GrowUpWayFrameLevelBtn2NameBigFont:Hide();
	GrowUpWayFrameLevelBtn2IsFineshFont:Show();
	GrowUpWayFrameLevelBtn2IsFineshBigFont:Hide();
	GrowUpWayFrameLevelBtn2:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 207, 63 );
	GrowUpWayAttentionID = GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1;
	GameTooltipOnLeave();
	SetTipsPositon();
end
function GrowUpWayFrameRightPageBtn_onUpdate()
end
function GrowUpWayFrameDescRich_onClick()
	if arg3 == "LeftButton" then
		local strLinkItemHref	= arg2;
		local bLinkNpc, nNpcId, nTargetMapId = GetNpcPos( strLinkItemHref );

		if bLinkNpc == true then
			-- ��������Ŀ��NPC����ͬһ�ŵ�ͼ��
			MapShower:GetMaxMapContal():linkMoveto( nTargetMapId, nNpcId );
			return;
		end

		local nMapId, nMapPosX, nMapPosY = GetLinkPos( strLinkItemHref );
		if nMapId ~= 0 then
			MapShower:GetMaxMapContal():linkMoveto( nMapId, nMapPosX, nMapPosY );
		end
	end
end
function GrowUpWayLevelBtn_onClick()
	GameTooltipOnLeave();
	local index = this:GetClientID();
	if index == 1 then
		if GrowUpWayAttentionID == 1 then
			return;
		end
		GrowUpWayFrameLevelBtn2:SetSize( 52, 70 );
		GrowUpWayFrameLevelBtn2NameFont:Show();
		GrowUpWayFrameLevelBtn2NameBigFont:Hide();
		GrowUpWayFrameLevelBtn2IsFineshFont:Show();
		GrowUpWayFrameLevelBtn2IsFineshBigFont:Hide();
		GrowUpWayFrameLevelBtn2:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 207, 63 );
		GrowUpWayAttentionID = (GrowUpWayAttentionID -2 + ACHIEVEMENT_CATEGORY_NUM ) % ACHIEVEMENT_CATEGORY_NUM + 1;
		SetLevelBtnTips( index );
		SetTipsPositon();
	elseif index == 3 then
		local mainplayer = ActorMgr:getMainPlayer();
		local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1 );
		if AchievementCategoryDef.Level > mainplayer:getLv() or GrowUpWayAttentionID == ACHIEVEMENT_CATEGORY_NUM then
			return;
		end
		GrowUpWayFrameLevelBtn2:SetSize( 52, 70 );
		GrowUpWayFrameLevelBtn2NameFont:Show();
		GrowUpWayFrameLevelBtn2NameBigFont:Hide();
		GrowUpWayFrameLevelBtn2IsFineshFont:Show();
		GrowUpWayFrameLevelBtn2IsFineshBigFont:Hide();
		GrowUpWayFrameLevelBtn2:SetPoint( "topleft", "GrowUpWayFrame", "topleft", 207, 63 );
		GrowUpWayAttentionID = GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1;
		SetLevelBtnTips( index );
		SetTipsPositon();
	end	
end
function SetLevelBtnTips( index )
	local AchievementCategoryId = 0;
	if index == 2 then
		AchievementCategoryId = GrowUpWayAttentionID;
	elseif index == 1 then
		if GrowUpWayAttentionID == 1 then
			return;
		end
		AchievementCategoryId = (GrowUpWayAttentionID -2 + ACHIEVEMENT_CATEGORY_NUM ) % ACHIEVEMENT_CATEGORY_NUM + 1;
	elseif index == 3 then
		if GrowUpWayAttentionID == ACHIEVEMENT_CATEGORY_NUM then
			return;
		end
		AchievementCategoryId = GrowUpWayAttentionID % ACHIEVEMENT_CATEGORY_NUM + 1;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( AchievementCategoryId );
	local isLevelEnough = false;
	local isAchievementEnough = true;
	--ͳ���Ѵ�ɵ�����
	local t_finishedTask = {};
	for i = 1, MAX_CATEGORY_ACHIEVEMENT_TOLUA do
		local ID = AchievementCategoryDef.AchievementID[i-1];
		if ID > 0 and t_AchievementCategoryList[AchievementCategoryId][ID] then
			local AchievementDef = mainplayer:getAchievementDef( AchievementCategoryDef.AchievementID[i-1], AchievementCategoryId );
			for i = 1, table.getn( t_tagRESULT ) do
				if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
					local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
					local t_finshtask = { ["name"] = AchievementDef.Name, ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };
					table.insert( t_finishedTask, t_finshtask );
				end
			end
		elseif ID > 0 and not t_AchievementCategoryList[AchievementCategoryId][ID] then
			isAchievementEnough = false;
		end
	end
	--ͳ��������������
	local t_TotalTaskInfo = {};
	for i = 1, MAX_CATEGORY_ACHIEVEMENT_TOLUA do
		local ID = AchievementCategoryDef.AchievementID[i-1];
		if ID > 0 then
			local AchievementDef = mainplayer:getAchievementDef( AchievementCategoryDef.AchievementID[i-1], AchievementCategoryId );
			for j = 1,table.getn( t_tagRESULT ) do
				if t_tagRESULT[j][1] == AchievementDef.Result.ResultID then
					for k = 1, table.getn( t_TotalTaskInfo )	do
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						if t_TotalTaskInfo[k]["Attr"] == DesResult[2] then
							t_TotalTaskInfo[k]["AttrVal"] = t_TotalTaskInfo[k]["AttrVal"] + AchievementDef.Result.ResultVal1;
							break;
						end
						if k == table.getn( t_TotalTaskInfo ) then
							t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
							table.insert( t_TotalTaskInfo, t_oneTaskInfo );
						end
					end	
					if table.getn( t_TotalTaskInfo ) == 0 then
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
						table.insert( t_TotalTaskInfo, t_oneTaskInfo );
					end	
				end
			end
		end
	end
	local szInfo = "";
	for i = 1, table.getn( t_TotalTaskInfo ) do
		szInfo = szInfo .."#cf0a004"..t_TotalTaskInfo[i]["Attr"].."+"..t_TotalTaskInfo[i]["AttrVal"];
	end
	if mainplayer:getLv() >= AchievementCategoryDef.Level then
		isLevelEnough = true;
	end
	if not isLevelEnough then
		if table.getn( t_finishedTask ) > 0 then
			szInfo = szInfo.."\n\n#cff0000��ǰ�Ѽ����������ԣ�";
			for i = 1, table.getn( t_finishedTask ) do
				szInfo = szInfo.."\n		"..t_finishedTask[i]["name"].."	"..t_finishedTask[i]["Attr"].."+"..t_finishedTask[i]["AttrVal"];
			end
			szInfo = szInfo.."\n\n	#c008aff�������ĵȼ����㣬��ʱ�޷����ܸ����Լӳɣ���Ҫ���ĵȼ��ﵽ"..AchievementCategoryDef.Level.."��";
		else
			szInfo = szInfo.."\n\n	#cff0000����"..AchievementCategoryDef.Level.."��������"..AchievementCategoryDef.Name.."ҳ��ɳ�����,���ɻ�ø����Լӳ�".."\n\n	#c008aff����������Զ�����";
		end
	else
		szInfo = "ȫ������������������ԣ�\n	"..szInfo;
	end
	SetGameTooltips( "GrowUpWayFrameLevelBtn"..index.."IconTexture", szInfo );
end
function GrowUpWayLevelBtn_onEnter()
	local index = this:GetClientID();
	SetLevelBtnTips( index );
end
function GrowUpWayTaskBtn_OnClick()
	local index = this:GetClientID();
	local mainplayer = ActorMgr:getMainPlayer();
	local AchievementDef = mainplayer:getAchievementDef( t_categoryIds[GrowUpWayAttentionID][index], GrowUpWayAttentionID );
	GrowUpWayFrameDescRich:SetText( AchievementDef.GuideDes, 0, 255, 0 );
	local szAttr = "";
	for i = 1, table.getn( t_tagRESULT ) do
		if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
			local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
			szAttr = szAttr..DesResult[2].."+"..AchievementDef.Result.ResultVal1;
		end
	end
	GrowUpWayFrameTaskInfoFont:Show();
	GrowUpWayFrameCurTipsBtn:Show();
	GrowUpWayFrameTaskInfoFont:SetText( szAttr );
	GrowUpWayFrameCurTipsBtnDescFont:SetText( AchievementDef.Name );
	GrowUpWayFrameTaskInfoFont:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -38+65*index, 344 );
	GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -50+65*index, 260 );
	GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", -7+65*index, 285  );
	if index == 5 then
		GrowUpWayFrameCurTipsBtn:SetPoint( "topleft", "GrowUpWayFrame", "topleft", -100+65*index, 260 );
		GrowUpWayFrameCurTipsBtnTipsArrow:SetPoint(  "topleft", "GrowUpWayFrame", "topleft", -12+65*index, 285  );
	end
end
function GrowUpWayTaskBtn_OnEnter()
	local index = this:GetClientID();
	local mainplayer = ActorMgr:getMainPlayer();
	local ID = t_categoryIds[GrowUpWayAttentionID][index];
	local AchievementDef = mainplayer:getAchievementDef( ID, GrowUpWayAttentionID );
	local szInfo = "#cf0a004�����������ԣ�";
	for i = 1, table.getn( t_tagRESULT ) do
		if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
			local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
			szInfo = szInfo..DesResult[2].."+"..AchievementDef.Result.ResultVal1.."\n\n	#c00ff00"..AchievementDef.Cond.Des;
		end
	end
	if not t_AchievementCategoryList[GrowUpWayAttentionID][ID] then
		szInfo = szInfo.."\n\n	#c008aff����������Զ�����";
	else
		szInfo = szInfo.."\n\n	#c008aff�Ѽ���";
	end
	SetGameTooltips( this:GetName(), szInfo );
end

-------------------------�ɳ�֮·������ʾ-----------------------------
local SHOW_TIME = 5;
local nLastShow = 0;
local newAttentionID = 0;
local newTaskID = 0;
local t_AttentionBtnInfo = {
		{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
		{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
		{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
}
function SetGrowUp( lv )
	--[[
	if lv < 10 then return end
	local level = lv;
	for i = 1, table.getn( t_levelCategoryList ) do
		if level == t_levelCategoryList[i]["min"] then
			local AttentionID = i;
			if AttentionID ~= 0 then
				newAttentionID = AttentionID;
				newTaskID = 0;
				GrowUpWayAttentionFrame:Show();
			end
			return;
		end
	end
	--]]
end
function AddNewAchievementCategoryID( id )
	local mainplayer = ActorMgr:getMainPlayer();
	if id <= 0 or id > 34 then return end
	SetAchievementCategoryList( id );
	local AttentionID = 0;
	AttentionID = EnableWhichCategory( id );
	if AttentionID ~= 0 then
		if mainplayer:getLv() < 10 then
			ShowMidTips( "��ȥ�����������ɣ�10�����ܴ򿪳ɳ�֮·Ӵ��" );
			return;
		end
		newAttentionID = AttentionID;
		newTaskID = id;
		local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( newAttentionID );
		if GrowUpWayFrame:IsShown() and AchievementCategoryDef.Level <= mainplayer:getLv() then			
			SetGrowUpWayInfo( AttentionID, id );
			GrowUpWayFrame:Show();
		end
		GrowUpWayAttentionFrame:Show();
	end
end
function EnableWhichCategory( id )
	local CategoryId = 0;
	for i = 1, table.getn( t_AchievementCategoryList ) do
		if t_AchievementCategoryList[i][id] ~= nil then
			CategoryId = i;
		end
	end
	return CategoryId;
end
function SetAchievementCategoryList( id )
	for i = 1, table.getn( t_AchievementCategoryList ) do
		if t_AchievementCategoryList[i][id] ~= nil then
			t_AchievementCategoryList[i][id] = true;
		end
	end
end
function LoadAchievementCategoryID( id )
	SetAchievementCategoryList( id );
end
function ClearAchievementCategoryList()
	t_AchievementCategoryList = {
	[1] = { [1] = false,	[2] = false,	[3] = false },
	[2] = { [5] = false,	[22] = false,	[9] = false,	[10] = false, },
	[3] = { [6] = false,	[23] = false,	[14] = false,	[15] = false, },
	[4] = { [17] = false,	[11] = false,	[21] = false,	[13] = false, },
	[5] = { [7] = false,	[20] = false,	[33] = false, },
	[6] = { [25] = false,	[26] = false,	[27] = false,	[8] = false,},
	[7] = { [4] = false,	[16] = false,	[19] = false,	[24] = false,	[34] = false, },
--	[8] = { [28] = false,	[29] = false,	[30] = false,	[31] = false,	[32] = false },
};
	local t_AttentionBtnInfo = {
			{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
			{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
			{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
	}
	for i = 1, table.getn( t_AttentionBtnInfo ) do
		local btn = getglobal( "GrowUpWayAttentionFrameBtn"..i );
		btn:Hide();
	end
end
function GrowUpWayAttentionFrame_onShow()
	this:SetPoint( "center", "UIClient", "center", 0, 240 );
	if newAttentionID == 0 then return end
	table.remove( t_AttentionBtnInfo );
	t_btnInfo = { ["ID"] = newAttentionID, ["LastShowTime"] = 0, ["taskID"] = newTaskID, };
	newAttentionID = 0;
	newTaskID = 0;
	table.insert( t_AttentionBtnInfo, 1, t_btnInfo );
	for i = 1 , table.getn( t_AttentionBtnInfo ) do
		local btn = getglobal( "GrowUpWayAttentionFrameBtn"..i );
		if t_AttentionBtnInfo[i]["ID"] ~= 0 then
			btn:Show();	
		end
	end
end
function GrowUpWayAttentionFrame_onHide()
	--������е���Ϣ
	t_AttentionBtnInfo = {
			{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
			{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
			{ ["ID"] = 0, ["LastShowTime"] = 0, ["taskID"] = 0, },
	}
end
function GrowUpWayAttentionBtn_onShow()
	local index = this:GetClientID();
	t_AttentionBtnInfo[index]["LastShowTime"] = os.clock();
	local AttentionID = t_AttentionBtnInfo[index]["ID"];
	local mainplayer = ActorMgr:getMainPlayer();
	local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( AttentionID );
	local szAttentionRichText = "#c461900[�ɳ�֮·]���Լӳɣ�  ";
	if t_AttentionBtnInfo[index]["taskID"] > 0 then
		local AchievementDef = mainplayer:getAchievementDef( t_AttentionBtnInfo[index]["taskID"], AttentionID );
		for i = 1, table.getn( t_tagRESULT ) do
			if t_tagRESULT[i][1] == AchievementDef.Result.ResultID then
				local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
				szAttentionRichText = szAttentionRichText.."#cb41e00"..DesResult[2].."+"..AchievementDef.Result.ResultVal1;
			end
		end
		if AchievementCategoryDef.Level > mainplayer:getLv() then
			szAttentionRichText = szAttentionRichText.."#cff0000("..AchievementCategoryDef.Level.."�������ܴ˼ӳ�)";
		end
		szAttentionRichText = szAttentionRichText.."#c461900\n"..AchievementCategoryDef.Name..":"..AchievementDef.Name;
		local DescRich = getglobal( this:GetName().."DescRich" );
		DescRich:SetText( szAttentionRichText, 255, 255, 0 );
		return;
	end
	local t_TaskInfo = {};
	for i = 1, MAX_CATEGORY_ACHIEVEMENT_TOLUA do
		local ID = AchievementCategoryDef.AchievementID[i-1];
		if ID > 0 and t_AchievementCategoryList[AttentionID][ID] then
			local AchievementDef = mainplayer:getAchievementDef( ID, AttentionID );
			--��ǰ����������Ի�ȡ
			for j = 1,table.getn( t_tagRESULT ) do
				if t_tagRESULT[j][1] == AchievementDef.Result.ResultID then
					--������������
					local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
					for k = 1, table.getn( t_TaskInfo )	do
						--����� +valֵ
						if t_TaskInfo[k]["Attr"] == DesResult[2] then
							t_TaskInfo[k]["AttrVal"] = t_TaskInfo[k]["AttrVal"] + AchievementDef.Result.ResultVal1;
							break;
						end
						--���û�� insert
						if k == table.getn( t_TaskInfo ) then
							t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
							table.insert( t_TaskInfo, t_oneTaskInfo );
						end
					end
					--����б�Ϊ�� insert
					if table.getn( t_TaskInfo ) == 0 then
						local DesResult = GetOneItemResultConfig( { ["resultID"] = AchievementDef.Result.ResultID } )
						t_oneTaskInfo = { ["Attr"] = DesResult[2], ["AttrVal"] = AchievementDef.Result.ResultVal1 };							
						table.insert( t_TaskInfo, t_oneTaskInfo );
					end	
				end
			end
		end
	end
	for i = 1, table.getn( t_TaskInfo ) do
		szAttentionRichText = szAttentionRichText.."#cb41e00"..t_TaskInfo[i]["Attr"];
		szAttentionRichText = szAttentionRichText.."+"..t_TaskInfo[i]["AttrVal"];
	end
	szAttentionRichText = szAttentionRichText.."#c461900\n��ϲ��ﵽ"..AchievementCategoryDef.Level.."�����ɼ���["..AchievementCategoryDef.Name.."]�������";
	local DescRich = getglobal( this:GetName().."DescRich" );
	DescRich:SetText( szAttentionRichText, 255, 255, 0 );
end
function GrowUpWayAttentionBtn_onUpdate()
	local index = this:GetClientID();
	local nLastShow = t_AttentionBtnInfo[index]["LastShowTime"];
	--[[
	if ( os.clock() - nLastShow) >= SHOW_TIME then
		this:Hide();
	end
	--]]
end
function GrowUpWayAttentionBtn_onHide()
	local index = this:GetClientID();	
	t_AttentionBtnInfo[index]["LastShowTime"] = 0;
	t_AttentionBtnInfo[index]["taskID"] = 0;
	t_AttentionBtnInfo[index]["ID"] = 0;
end
function GrowUpWayAttentionBtn_onClick()
	local index = this:GetClientID();
	if t_AttentionBtnInfo[index]["ID"] > 0 then
		local id = t_AttentionBtnInfo[index]["ID"];
		local taskID = t_AttentionBtnInfo[index]["taskID"];
		--����ӵȼ��ж�
		local mainplayer = ActorMgr:getMainPlayer();
		local AchievementCategoryDef = mainplayer:getAchievementCategoryDef( id );
		if AchievementCategoryDef.Level > mainplayer:getLv() then
			local level = mainplayer:getLv();			
			for i = 1, table.getn( t_levelCategoryList ) do
				if level >= t_levelCategoryList[i]["min"] and level <= t_levelCategoryList[i]["max"] then
					id = i;
					taskID = 0;
				end
			end
		end
		SetGrowUpWayInfo( id, taskID );
		GrowUpWayFrame:Show();
	end
	local btn = getglobal( "GrowUpWayAttentionFrameBtn"..index );
	btn:Hide();
end
function SetGrowUpWayInfo( id, taskID )
	GrowUpWayAttentionID = id;
	GrowUpWayAttentionTaskID = taskID;
end
