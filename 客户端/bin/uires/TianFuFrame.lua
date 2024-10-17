local curTianFuType = 0
local MAX_STEP_NUM = 5
local MAX_INFO_NUM = 12

local FURY_TYPE_OPEN_LV = 70;
TILIAN_CAN_USE_LV = 100;
ADV_WASH_CAN_USE_LV = 150;

local tTianFuLvRange = {	{ "",		0,		1 },
							{ "一阶",	1,		5 },
							{ "二阶",	6,		10 },
							{ "三阶",	16,		20 },
							{ "四阶",	36,		30 },
							{ "五阶",	66,		40 },
					}

local tTianFuType = {
			[1] = "基础天赋",
			[2] = "狂暴天赋",
			[3] = "防御天赋",
			};

local tTianFuLockRange = {	
				{ "一阶",	0,	712,	0 },
				{ "一阶",	5,	712,	0 },
				{ "二阶",	15,	731,	0 },
				{ "三阶",	35,	712,	18 },
				{ "四阶",	65,	731,	18 },
				{ "五阶",	105,	712,	36 },
					};

--基础天赋
local tInfoBtnPos = {	-- 守护者
						[CLIENT_CAREER_WARRIOR_SINGLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [2] = 7, [5] = 8, [4] = 9, [11] = 10, [1] = 11, [12] = 12 },
						-- 征服者
						[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [5] = 8, [4] = 9, [11] = 10, [2] = 11, [12] = 12 },
						-- 聚能者
						[CLIENT_CAREER_MAGICOR_SINGLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [4] = 8, [5] = 9, [11] = 10, [3] = 11, [12] = 12 },
						-- 炼魂师
						[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [4] = 8, [5] = 9, [11] = 10, [3] = 11, [12] = 12 },
						-- 猎杀者
						[CLIENT_CAREER_GUNER_SINGLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [5] = 8, [4] = 9, [11] = 10, [2] = 11, [12] = 12 },
						-- 毁灭者
						[CLIENT_CAREER_GUNER_DOUBLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [5] = 8, [4] = 9, [11] = 10, [2] = 11, [12] = 12 },
						-- 圣佑者
						[CLIENT_CAREER_CHURCH_SINGLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [4] = 8, [5] = 9, [11] = 10, [3] = 11, [12] = 12 },
						-- 驭雷师
						[CLIENT_CAREER_CHURCH_DOUBLEHAND] = { [7] = 1, [6] = 2, [9] = 3, [10] = 4, [8] = 5, [11] = 6, [1] = 7, [4] = 8, [5] = 9, [11] = 10, [3] = 11, [12] = 12 },
					}

--狂暴天赋
local tFuryBtnPos = {	-- 守护者
						[CLIENT_CAREER_WARRIOR_SINGLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 征服者
						[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 聚能者
						[CLIENT_CAREER_MAGICOR_SINGLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 炼魂师
						[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 猎杀者
						[CLIENT_CAREER_GUNER_SINGLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 毁灭者
						[CLIENT_CAREER_GUNER_DOUBLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 圣佑者
						[CLIENT_CAREER_CHURCH_SINGLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
						-- 驭雷师
						[CLIENT_CAREER_CHURCH_DOUBLEHAND] = { [10] = 1, [11] = 2, [20] = 3, [21] = 4, [14] = 5, [15] = 6, [16] = 7, [17] = 8, [22] = 9, [23] = 10, [12] = 11, [13] = 12 },
					}

--防御天赋
local tDefBtnPos = {	-- 守护者
						[CLIENT_CAREER_WARRIOR_SINGLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 征服者
						[CLIENT_CAREER_WARRIOR_DOUBLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 聚能者
						[CLIENT_CAREER_MAGICOR_SINGLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 炼魂师
						[CLIENT_CAREER_MAGICOR_DOUBLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 猎杀者
						[CLIENT_CAREER_GUNER_SINGLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 毁灭者
						[CLIENT_CAREER_GUNER_DOUBLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 圣佑者
						[CLIENT_CAREER_CHURCH_SINGLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
						-- 驭雷师
						[CLIENT_CAREER_CHURCH_DOUBLEHAND] = { [18] = 1, [19] = 2, [26] = 3, [27] = 4, [22] = 5, [23] = 6, [24] = 7, [25] = 8, [28] = 9, [29] = 10, [20] = 11, [21] = 12 },
					}

local function GetTianFuLevel( level )
	for i=1,#tTianFuLvRange do
		if level >= tTianFuLvRange[i][2] and level < ( tTianFuLvRange[i][2] + tTianFuLvRange[i][3] ) then
			return tTianFuLvRange[i][1],(level - tTianFuLvRange[i][2] + 1), tTianFuLvRange[i][3]
		end
	end
end

local function GetTianFuLockLevel( level )
	local num = 0;
	for i=1,#tTianFuLockRange do
		if level < tTianFuLockRange[i][2] then
			break;
		end
		num = num + 1;
	end
	return num;
end

local geniusStepName = {	[0] = "",
							[1] = "一阶",
							[2] = "二阶",
							[3] = "三阶",
							[4] = "四阶",
							[5] = "五阶",
						}

local geniusStepColor = {	[0] = { r = 80, g = 80, b = 80 },
							[1] = { r=  254, g = 249, b = 234 },
							[2] = { r = 66, g = 168, b = 61 },
							[3] = { r = 61, g = 109, b = 168 },
							[4] = { r = 151, g = 45, b = 249 },
							[5] = { r = 255, g = 185, b = 255 },
						}

local bonusColor = {
			{ r = 158, g = 158, b = 158 },
			{ r = 214, g = 214, b = 214 },
			{ r = 255, g = 255, b = 255 },
			{ r = 255, g = 246, b = 11  },
			{ r = 255, g = 166, b = 11  },
			{ r = 255, g = 90,  b = 0   },
			{ r = 255, g = 0,   b = 0   },
			}

function GetGeniusBonusColor( diff, nType )
	local mdfDef = GeniusMgr:getGeniusMdfDef( nType );
	if mdfDef == nil then
		return {r=80,g=80,b=80}, 10000;
	end

	local idx = 0;
	for i=1, #bonusColor do
		if diff >= mdfDef.Mdf[i-1].LowerLimit and diff <= mdfDef.Mdf[i-1].UpperLimit then
			idx = i;
		end
	end

	if idx > 0 then
		return bonusColor[idx], mdfDef.Mdf[idx-1].Mul;
	else
		if math.abs( diff ) < 5 then
			return { r = 0, g = 220, b = 0 }, 10000;
		else
			idx = diff > 0 and #bonusColor or 1;
			return bonusColor[idx], mdfDef.Mdf[idx-1].Mul;
		end
	end
end

function getMaxTianFuLv()
	return tTianFuLvRange[#tTianFuLvRange][2] + tTianFuLvRange[#tTianFuLvRange][3] - 1;
end

local function UpdateTianFu()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end;
	local geniusCareerDef = GeniusMgr:getGeniusCareerDef( mainplayer:getRace() )

	local num = 1
	local minLevel = getMaxTianFuLv();
	if geniusCareerDef ~= nil then
		for i=1,MAX_GENIUS_SUBTYPE_TOLUA do
			local attrId = geniusCareerDef.Types[curTianFuType].AttrIDs[i-1]
			if attrId > 0 then
				local lv = GeniusMgr:getAttrLv(curTianFuType,attrId)
				local geniusAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv )
				local btn = getglobal( "TianFuFrameInfo" .. num )
				local icon = getglobal( btn:GetName() .. "Icon" )
				local IconPath = "uires\\TuPianLei\\TianFu\\"..geniusAttrDef.IconID..".tga";
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH
				end
				icon:SetTexture( IconPath )
				if lv > 0 then
					icon:SetGray( false )
				else
					icon:SetGray( true )
				end
				local uvTex = getglobal( btn:GetName() .. "UVTex" )
				if geniusAttrDef.Step > 1 then
					uvTex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei  ".. 10+geniusAttrDef.Step ..".tga" )
					uvTex:Show()
				else
					uvTex:Hide()
				end
				
				local bonusColor = lv > 0 and {r=255,g=255,b=255} or {r=80,g=80,b=80};
				if lv > 0 and curTianFuType == 0 then
					local fengLv = GetFengYinData();
					local maxLv = GameMgr:GetMaxLevel();
					local levelupDef = getLevelUP( fengLv > 0 and fengLv or maxLv );
					if levelupDef ~= nil then
						local baseLv = levelupDef.BaseGenius;
						local nType = GeniusMgr:getGeniusType(curTianFuType);
						bonusColor = GetGeniusBonusColor( lv - baseLv, nType );
					end
				end
				local t_fonts = {	[btn:GetName() .. "Name"] = { value = geniusAttrDef.Name .."·".. geniusStepName[geniusAttrDef.Step], color = GeniusMgr:getAttrLv(curTianFuType,attrId) > 0 and geniusStepColor[geniusAttrDef.Step] or {r=80,g=80,b=80} },
									[btn:GetName() .. "Lv" ] = { value = "等级：" ..( lv > 0 and geniusAttrDef.Lvl or "未激活" ), color = bonusColor },
								}
				util.SetFonts( t_fonts )
				
				local step = 0;
				if geniusAttrDef.AttrLvl ~= 0 then
					local _,nlv,max = GetTianFuLevel( geniusAttrDef.AttrLvl )
					step = math.floor(nlv / max * MAX_STEP_NUM);
					for i=1,step do
						local tex = getglobal( btn:GetName() .. "Lv" .. i )
						tex:Show()
						tex:SetSize( 26, 12 )
						tex:SetTexUV( 409, 488, 26, 12 )
					end

					if step ~= MAX_STEP_NUM then
						local scale = nlv * MAX_STEP_NUM / max - step;
						local tex = getglobal( btn:GetName() .. "Lv" .. step + 1 )
						tex:Show()
						tex:SetSize( 26*scale, 12 )
						tex:SetTexUV( 409, 488, 26*scale, 12 )
						step = step + 1;
					end
				end
				
				for i=step+1,MAX_STEP_NUM do
					local tex = getglobal( btn:GetName() .. "Lv" .. i )
					tex:Hide()
				end

				if lv > 0 then
					btn:SetNormalTextureTemplate( "LayerGrayGlassBkgTemplate" )
				else
					btn:SetNormalTextureTemplate( "LayerGrayGlassDisableTemplate" )
				end
				local upTex = getglobal( btn:GetName() .. "UpTex" )
				local upUVTex = getglobal( btn:GetName() .. "UpUVTex" )
				local nextAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv + 1 )
				if nextAttrDef ~= nil and ( GeniusMgr:getAllGeniusValue() >= geniusAttrDef.UpLimit.ElseAttrLvlLimit ) 
					and mainplayer:getLv() >= geniusAttrDef.UpLimit.RoleLvlLimit then
					upTex:Show()
					upUVTex:SetUVAnimation( 80, true )
				else
					upTex:Hide()
					upUVTex:Hide()
				end
				btn:SetClientUserData( 0, attrId )
				
				local idx = 0;
				if curTianFuType == 0 then
					idx = tInfoBtnPos[mainplayer:getRace()][attrId]
				elseif curTianFuType == 1 then
					idx = tFuryBtnPos[mainplayer:getRace()][attrId]
				else
					idx = tDefBtnPos[mainplayer:getRace()][attrId]
				end
				btn:SetPoint( "topleft","TianFuFrame","topleft", 17 + math.floor((idx-1)%3)*165, 64 + math.floor((idx-1)/3)*82 )
				btn:Show()
				num = num + 1
				minLevel = minLevel > lv and lv or minLevel;
			end
		end		
	end
	for i=num,MAX_INFO_NUM do
		local btn = getglobal( "TianFuFrameInfo" .. i )
		btn:Hide()
	end
	TianFuFrameVal:SetText( "基础天赋值：" .. GeniusMgr:getGeniusTypeValue(0) .. "    总天赋值：" .. GeniusMgr:getAllGeniusValue() )
	
	local nType = GeniusMgr:getGeniusType(curTianFuType);
	if GeniusMgr:getGeniusAddDef( nType, mainplayer:getRace() ) ~= nil then
		TianFuFrameLockTex:Show();
		TianFuFrameLockTips:Show();
		TianFuFrameLockTips:SetClientUserData( 0, minLevel );
		local lockLevel = GetTianFuLockLevel( minLevel );
		local szText = RGBColorToRichColor( { geniusStepColor[lockLevel-1].r, geniusStepColor[lockLevel-1].g, geniusStepColor[lockLevel-1].b } ) .. 
				GetCareerName( mainplayer:getRace() ) .. "·" .. tTianFuType[nType] .. "锁·" .. tTianFuLockRange[lockLevel][1] .."#n";
		TianFuFrameLockTex:SetTexUV( tTianFuLockRange[lockLevel][3], tTianFuLockRange[lockLevel][4], 18, 17 );
		if lockLevel > 1 then
			TianFuFrameLockTex:SetGray( false );
			TianFuFrameLockFont:SetText( szText .. "（已激活）", 0, 230, 0 );
		else
			TianFuFrameLockTex:SetGray( true );
			TianFuFrameLockFont:SetText( szText .. "（未激活）", 80, 80, 80 );
		end
	else
		TianFuFrameLockTex:Hide();
		TianFuFrameLockTips:Hide();
		TianFuFrameLockFont:Clear();
	end
end

function TianFuFrame_OnLoad()
	this:RegisterEvent( "GE_GENIUS_UPDATE" )
	this:RegisterEvent( "UI_ACTOR_LEVEL_CHG" );
end

function TianFuFrame_OnEvent()
	if this:IsShown() then
		if arg1 == "UI_ACTOR_LEVEL_CHG" then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer:getLv() == FURY_TYPE_OPEN_LV then
				TianFuFrameFuryTypeBtn:Enable();
				TianFuFrameDefenceTypeBtn:Enable();
				TianFuFrameFuryTypeBtnTips:Hide();
				TianFuFrameDefenceTypeBtnTips:Hide();
			end
		else
			UpdateTianFu();
		end
	end

	local btn = getglobal( "PlayerContainerResolveFrameSparEpurateBtn" );
	local tipBtn = getglobal( "PlayerContainerResolveFrameCannotEpurateBtn" );
	if GeniusMgr:getAllGeniusValue() < TILIAN_CAN_USE_LV then
		btn:Disable();
		tipBtn:Show();
	else
		btn:Enable();
		tipBtn:Hide();
	end

	if ArmWashFrame:IsShown() then
		btn = getglobal( "ArmWashFrameAdvBtn" );
		tipBtn = getglobal( "ArmWashFrameAdvTipBtn" );
		if GeniusMgr:getAllGeniusValue() < ADV_WASH_CAN_USE_LV then
			btn:Disable();
			tipBtn:Show();
		else
			btn:Enable();
			tipBtn:Hide();
		end
	end

	if GeniusMgr:getAllGeniusValue() >= TILIAN_CAN_USE_LV and not GameWizard:isDo( GAMEWIZARD_FIRST_SPAR_EQURATE ) then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		if t_centerXinShouBtnControl:getIndexByType( CENTER_XINSHOU_SPAR_EQURATE ) == 0 then
			t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOU_SPAR_EQURATE, frame = "PlayerContainerResolveFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = 0 } );
		end
	end
end

local function DisCheckAllTypeBtn()
	TianFuFrameBaseTypeBtn:DisChecked();
	TianFuFrameFuryTypeBtn:DisChecked();
	TianFuFrameDefenceTypeBtn:DisChecked();
	DisCheckAllBtn( "TianFuFrameInfo",MAX_INFO_NUM )
	if TianFuUpFrame:IsShown() then
		TianFuUpFrame:Hide();
	end
end

function TianFuFrame_OnShow()
	curTianFuType = 0;
	this:SetPoint( "center", "UIClient", "center", -180, 0 )
	UpdateTianFu()
	DisCheckAllBtn( "TianFuFrameInfo",MAX_INFO_NUM )

	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil or mainplayer:getLv() < FURY_TYPE_OPEN_LV then
		TianFuFrameFuryTypeBtn:Disable();
		TianFuFrameDefenceTypeBtn:Disable();
		TianFuFrameFuryTypeBtnTips:Show();
		TianFuFrameDefenceTypeBtnTips:Show();
	else
		TianFuFrameFuryTypeBtn:Enable();
		TianFuFrameDefenceTypeBtn:Enable();
		TianFuFrameFuryTypeBtnTips:Hide();
		TianFuFrameDefenceTypeBtnTips:Hide();
	end
	DisCheckAllTypeBtn();
	TianFuFrameBaseTypeBtn:Checked();
end

function TianFuFrameInfoBtn_OnClick()
	local attrId = this:GetClientUserData( 0 )
	TianFuUpFrame:SetClientUserData( 0, attrId )
	DisCheckAllBtn( "TianFuFrameInfo",MAX_INFO_NUM )
	this:Checked()
	if not TianFuUpFrame:IsShown() then
		TianFuUpFrame:Show()
	else
		UpdateTianFuUpFrame()
	end
end

function TianFuFrameInfoBtnTipsBtn_OnClick()
	local parent = getglobal( this:GetParent() )
	local attrId = parent:GetClientUserData( 0 )
	TianFuUpFrame:SetClientUserData( 0, attrId )
	DisCheckAllBtn( "TianFuFrameInfo",MAX_INFO_NUM )
	parent:Checked()
	if not TianFuUpFrame:IsShown() then
		TianFuUpFrame:Show()
	else
		UpdateTianFuUpFrame()
	end
end

function TianFuFrameInfoBtnTipsBtn_OnEnter()
	local parent = getglobal( this:GetParent() )
	parent:Highlight()
	local attrid = parent:GetClientUserData( 0 )
	TianFuTipsFrame:SetClientUserData( 0, attrid )
	TianFuTipsFrame:SetClientString( this:GetName() )
	TianFuTipsFrame:Show()
end

function TianFuFrameInfoBtnTipsBtn_OnLeave()
	local parent = getglobal( this:GetParent() )
	parent:DisHighlight()
	TianFuTipsFrame:Hide()
end

function TianFuFrameTypeBtn_OnClick()
	DisCheckAllTypeBtn();
	this:Checked();
	curTianFuType = this:GetClientID() - 1;
	UpdateTianFu();
end

function SetGeniusDesc( data, num )
	if data.ResultID ~= 0 then
		local tResultVal = { data.ResultVal1, data.ResultVal2, data.ResultVal3, data.ResultVal4 };
		for _,val in pairs( tResultVal ) do
			if val ~= 0 then
				local font = getglobal( "TianFuLockTipsFrameFont" .. num );
				font:SetText( getItemResultDesc( data.ResultID ).ResultDesc .. ( data.ResultID == 389 and "" or "：+" ) .. val .. "%" );--特殊处理编号389的属性
				font:SetTextColor( 255, 80, 0 );
				return 1;
			end
		end
	end
	return 0;
end

function UpdateTianFuLockTips( nType, minLevel, szBtnName, pActor )
	if pActor == nil then return end;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local nCareer = pActor:getRace();
	local lockLevel = GetTianFuLockLevel( minLevel );
	local t_fonts = {	["TianFuLockTipsFrameName"] = { value = GetCareerName( nCareer ) .. "·" .. tTianFuType[nType] .. "锁·" .. tTianFuLockRange[lockLevel][1], color = geniusStepColor[(lockLevel-1 == 0 and 1 or lockLevel-1)] },
				["TianFuLockTipsFrameState" ] = { value = lockLevel > 1 and "（已激活）" or "（未激活）", color = lockLevel > 1 and {r=0,g=230,b=0} or {r=80,g=80,b=80} },
				}
	util.SetFonts( t_fonts );
	local yOffset = 48;
	TianFuLockTipsFrameLineTex1:SetPoint( "topleft","TianFuLockTipsFrame","topleft", 2, yOffset );
	
	local geniusAddDef = GeniusMgr:getGeniusAddDef( nType, nCareer );
	local num = 1;
	if geniusAddDef ~= nil then
		local minLv = minLevel < geniusAddDef.Seg[0].From and geniusAddDef.Seg[0].From or minLevel;
		for i=1, MAX_GENIUS_SEG_TOLUA do
			if minLv >= geniusAddDef.Seg[i-1].From and minLv <= geniusAddDef.Seg[i-1].To then
				for j=1, MAX_GENIUS_RESULT_TOLUA do
					num = num + SetGeniusDesc( geniusAddDef.Seg[i-1].Add[j-1], num );
				end
				break;
			end
		end
	end

	for i = num, MAX_GENIUS_RESULT_TOLUA do
		local font = getglobal( "TianFuLockTipsFrameFont" .. i );
		font:SetText( "" );
	end


	yOffset = yOffset + 18 * ( num ) + 8;
	TianFuLockTipsFrameLineTex2:SetPoint( "topleft","TianFuLockTipsFrame","topleft", 2, yOffset );

	local lv = lockLevel + 1;
	if lockLevel + 1 > #tTianFuLvRange then
		yOffset = yOffset + 32;
		local t_fonts = { ["TianFuLockTipsFrameNextName"] = { value = "", color = {r=254,g=108,b=28} },
				  ["TianFuLockTipsFrameNextDesc"] = { value = "", color = {r=254,g=108,b=28} },
				  ["TianFuLockTipsFrameActivate"] = { value = "已达到最高阶天赋锁效果", color = {r=255,g=185,b=255} },
				}
		util.SetFonts( t_fonts );
	elseif lockLevel == 1 then
		yOffset = yOffset + 32;
		local t_fonts = { ["TianFuLockTipsFrameNextName"] = { value = "", color = {r=254,g=108,b=28} },
				  ["TianFuLockTipsFrameNextDesc"] = { value = "", color = {r=254,g=108,b=28} },
				  ["TianFuLockTipsFrameActivate"] = { value = "全部" .. tTianFuType[nType] .. "达到" .. tTianFuLvRange[lv][1] .. tTianFuLvRange[lv][3] .. "级后激活", color = {r=255,g=255,b=255} },
				}
		util.SetFonts( t_fonts );
	else
		yOffset = yOffset + 70;
		TianFuLockTipsFrameActivate:SetText( "全部" .. tTianFuType[nType] .. "达到" .. tTianFuLvRange[lv][1] .. tTianFuLvRange[lv][3] .. "级后激活" );
		local t_fonts = { ["TianFuLockTipsFrameNextName"] = { value = GetCareerName( nCareer ) .. "·" .. tTianFuType[nType] .. "锁·" .. tTianFuLockRange[lv][1], color = geniusStepColor[lv-1] },
				  ["TianFuLockTipsFrameNextDesc"] = { value = "下一阶天赋效果：", color = {r=120,g=120,b=60} },
				  ["TianFuLockTipsFrameActivate"] = { value = "全部" .. tTianFuType[nType] .. "达到" .. tTianFuLvRange[lv][1] .. tTianFuLvRange[lv][3] .. "级后激活", color = {r=255,g=255,b=255} },
				}
		util.SetFonts( t_fonts );
	end

	TianFuLockTipsFrameLineTex3:SetPoint( "topleft","TianFuLockTipsFrame","topleft", 2, yOffset );
	for i=1, #tTianFuType do
		local font = getglobal( "TianFuLockTipsFrameTypeCount" .. i );
		local szText = tTianFuType[i] .. "：" .. (pActor:GetID() ~= mainplayer:GetID() and GameWatchPlayer:getWatchPlayerGeniusTypePoint(i-1) or GeniusMgr:getGeniusTypeValue(i-1));
		font:SetText(szText);
	end

	yOffset = yOffset + 48;
	TianFuLockTipsFrame:SetHeight( yOffset );

	local nWidth    = TianFuLockTipsFrame:GetWidth();
	local nHeight   = TianFuLockTipsFrame:GetHeight();

	local curSelectBtn = getglobal( szBtnName );
	local nRealTop  = curSelectBtn:GetRealTop();
	local nRight    = curSelectBtn:GetRight();

	if ( nRealTop - nHeight ) > 0 then
		if ( nRight + nWidth < GetScreenWidth() ) then
			TianFuLockTipsFrame:SetPoint("BottomLeft",curSelectBtn:GetName(),"TopRight",0,-10);
		else
			TianFuLockTipsFrame:SetPoint("BottomRight",curSelectBtn:GetName(),"TopLeft",0,-10);
		end
	else
		if ( nRight + nWidth < GetScreenWidth() ) then
			TianFuLockTipsFrame:SetPoint("TopLeft",curSelectBtn:GetName(),"BottomRight",0,10);
		else
			TianFuLockTipsFrame:SetPoint("TopRight",curSelectBtn:GetName(),"BottomLeft",0,10);
		end
	end
end

function TianFuFrameLockTips_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	UpdateTianFuLockTips( GeniusMgr:getGeniusType(curTianFuType), this:GetClientUserData( 0 ), this:GetName(), mainplayer );
	TianFuLockTipsFrame:Show();
end

function TianFuFrameLockTips_OnLeave()
	TianFuLockTipsFrame:Hide();
end

function TianFuFrameTypeTipsBtn_OnClick()
	ShowMidTips( "角色达到" .. FURY_TYPE_OPEN_LV .. "级后自动激活" .. tTianFuType[this:GetClientID()] );
end

-----------------------------天赋升级----------------------------------------------
local tGeniusAttrName = {	[GENIUS_SUBTYPE_HP_TOLUA] = "生命上限增加",
				[GENIUS_SUBTYPE_PHYATK_TOLUA] = "物理攻击增加",
				[GENIUS_SUBTYPE_ICEATK_TOLUA] = "魔法攻击增加",
				[GENIUS_SUBTYPE_PHYDEF_TOLUA] = "物理防御增加",
				[GENIUS_SUBTYPE_ICEDEF_TOLUA] = "魔法防御增加",
				[GENIUS_SUBTYPE_MINGZHONG_LVL_TOLUA] = "命中等级增加",
				[GENIUS_SUBTYPE_SHANBI_LVL_TOLUA] = "闪避等级增加",
				[GENIUS_SUBTYPE_BAOJI_LVL_TOLUA] = "暴击等级增加",
				[GENIUS_SUBTYPE_DIKANG_LVL_TOLUA] = "暴击抵抗增加",
				[GENIUS_SUBTYPE_SH_ADD_DAM_TOLUA] = "对守护者造成的伤害增加",
				[GENIUS_SUBTYPE_ZF_ADD_DAM_TOLUA] = "对征服者造成的伤害增加",
				[GENIUS_SUBTYPE_SY_ADD_DAM_TOLUA] = "对圣佑者造成的伤害增加",
				[GENIUS_SUBTYPE_YL_ADD_DAM_TOLUA] = "对驭雷师造成的伤害增加",
				[GENIUS_SUBTYPE_JN_ADD_DAM_TOLUA] = "对聚能者造成的伤害增加",
				[GENIUS_SUBTYPE_LH_ADD_DAM_TOLUA] = "对炼魂师造成的伤害增加",
				[GENIUS_SUBTYPE_LS_ADD_DAM_TOLUA] = "对猎杀者造成的伤害增加",
				[GENIUS_SUBTYPE_HM_ADD_DAM_TOLUA] = "对毁灭者造成的伤害增加",
				[GENIUS_SUBTYPE_SH_DEC_DAM_TOLUA] = "守护者对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_ZF_DEC_DAM_TOLUA] = "征服者对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_SY_DEC_DAM_TOLUA] = "圣佑者对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_YL_DEC_DAM_TOLUA] = "驭雷师对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_JN_DEC_DAM_TOLUA] = "聚能者对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_LH_DEC_DAM_TOLUA] = "炼魂师对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_LS_DEC_DAM_TOLUA] = "猎杀者对自己造成的最终伤害减少",
				[GENIUS_SUBTYPE_HM_DEC_DAM_TOLUA] = "毁灭者对自己造成的最终伤害减少",
				}

local function getGeniusAttrDescText( geniusAttrDef )
	if geniusAttrDef.AttrID >= GENIUS_SUBTYPE_SH_ADD_DAM_TOLUA then
		return tGeniusAttrName[geniusAttrDef.AttrID] .. geniusAttrDef.AttrVar / 100 .. "%";
	else
		return tGeniusAttrName[geniusAttrDef.AttrID] .. geniusAttrDef.AttrVar;
	end
end

function UpdateTianFuUpFrame()
	local mainplayer = ActorMgr:getMainPlayer()
	local attrId = TianFuUpFrame:GetClientUserData( 0 )
	local lv = GeniusMgr:getAttrLv(curTianFuType,attrId)
	local geniusAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv )
	local nextDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv + 1 )
	if nextDef == nil then		
		ShowMidTips( geniusAttrDef.Name .. "已达到最高等级" )
		TianFuUpFrame:Hide()
		return
	end
	local lvcolor = ( mainplayer:getLv() < geniusAttrDef.UpLimit.RoleLvlLimit ) and {r=255,g=0,b=0} or {r=255,g=255,b=190}
	local moneyType = geniusAttrDef.UpLimit.UpUseMoneyType
	local needAttrColor = {r=255,g=255,b=190}
	if geniusAttrDef.UpLimit.ElseAttrLvlLimit > 0 then
		needAttrColor = GeniusMgr:getAllGeniusValue() < geniusAttrDef.UpLimit.ElseAttrLvlLimit and {r=255,g=0,b=0} or {r=255,g=255,b=190}
	end
	local bonusColor, Mul = {r=255,g=255,b=190}, 10000;
	local nextColor = {r=255,g=255,b=190}
	if curTianFuType == 0 then
		local fengLv = GetFengYinData();
		local maxLv = GameMgr:GetMaxLevel();
		local levelupDef = getLevelUP( fengLv > 0 and fengLv or maxLv );
		if levelupDef ~= nil then
			local baseLv = levelupDef.BaseGenius;
			local nType = GeniusMgr:getGeniusType(curTianFuType);
			bonusColor, Mul = GetGeniusBonusColor( lv - baseLv, nType );
			nextColor = GetGeniusBonusColor( nextDef.Lvl - baseLv, nType );
		end
	end
	local useExp = math.floor( geniusAttrDef.UpLimit.UpUseExp * Mul / 10000 );
	local useMoney = math.floor( geniusAttrDef.UpLimit.UpUseMoney * Mul / 10000 );
	local expcolor = ( mainplayer:getExp() < useExp ) and {r=255,g=0,b=0} or {r=255,g=255,b=190}
	local moneycolor =( getPlayerMoney( mainplayer, moneyType ) < useMoney ) and {r=255,g=0,b=0} or {r=255,g=255,b=190}

	local t_fonts = {	["TianFuUpFrameCaption"] = { value = geniusAttrDef.Name,  },
						["TianFuUpFrameLv"] = { value = "当前等级：" .. ( lv > 0 and (geniusStepName[geniusAttrDef.Step] .. geniusAttrDef.Lvl .. "级") or "未激活"), color = bonusColor },
						["TianFuUpFrameVal"] = { value = "天赋效果：" .. ( lv > 0 and getGeniusAttrDescText( geniusAttrDef ) or "无附加属性" ) },
						["TianFuUpFrameNextLv"] = { value = "下一等级：" .. (nextDef ~= nil and geniusStepName[nextDef.Step] .. nextDef.Lvl .. "级" or "" ), color = nextColor },
						["TianFuUpFrameNextVal"] = {value = "天赋效果：" .. (nextDef ~= nil and getGeniusAttrDescText( nextDef ) or "") },
						["TianFuUpFrameNeedAttr"] = {value = "天赋需求：" .. (geniusAttrDef.UpLimit.ElseAttrLvlLimit > 0 and ("天赋值达到" .. geniusAttrDef.UpLimit.ElseAttrLvlLimit .. "点") or "无" ), color= needAttrColor },
						["TianFuUpFrameNeedLv"] = { value = "角色等级：达到" .. geniusAttrDef.UpLimit.RoleLvlLimit .. "级", color = lvcolor  },
						["TianFuUpFrameNeedMoney"] = { value = "消耗" .. GetMoneyDesc(moneyType) .. "：" .. useMoney, color = moneycolor },
						["TianFuUpFrameNeedExp"] = { value = "消耗经验：" .. useExp, color = expcolor  },
						["TianFuUpFrameMoney"] = { value = "拥有货币：" .. getPlayerMoney( mainplayer, moneyType ) },
						["TianFuUpFrameExp"] = { value ="拥有经验：" .. mainplayer:getExp() },
					}
	util.SetFonts( t_fonts )

	if moneyType == ATTR_ID_MONEY then
		TianFuUpFrameSignTex:Hide();
		TianFuUpFrameNotice:Hide();
	else
		TianFuUpFrameSignTex:Show();
		TianFuUpFrameNotice:Show();
	end

	local szDesc = "为正常消耗";
	if Mul > 10000 then
		szDesc = "消耗增加";
	elseif Mul < 10000 then
		szDesc = "消耗减少"
	end
	TianFuUpFrameTooMuch:SetText( "当前天赋升级的经验和绑银" .. szDesc );
	
	--如果是防御天赋或者狂暴天赋，显示另一种升级方式
	if curTianFuType == 1 or curTianFuType == 2 then
		TianFuUpFrameTooMuch:SetText( "“双倍经验”天赋升级的经验消耗翻倍，不消耗银币" )
		TianFuUpFrameOkBtn:SetPoint( "topleft", "TianFuUpFrame", "topleft", 38, 345 )
		TianFuUpFrameDoubleExpBtn:Show()
		TianFuUpFrameDoubleExpBtn:SetPoint( "topleft", "TianFuUpFrame", "topleft", 198, 345 )
	else		
		TianFuUpFrameOkBtn:SetPoint( "topleft", "TianFuUpFrame", "topleft", 118, 345 )
		TianFuUpFrameDoubleExpBtn:Hide()
	end
end

function TianFuUpFrame_OnLoad()
	this:RegisterEvent( "GE_CHANGE_MONEY" )
	this:RegisterEvent( "UI_ACTOR_EXP_CHG" )
	this:RegisterEvent( "GE_GENIUS_UPDATE" )
	this:RegisterEvent( "UI_ACTOR_LEVEL_CHG" )
end

function TianFuUpFrame_OnEvent()
	if this:IsShown() then
		UpdateTianFuUpFrame()
	end
end

function TianFuUpFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 260, -26 )
	UpdateTianFuUpFrame();
end

function TianFuUpFrame_OnHide()
	DisCheckAllBtn( "TianFuFrameInfo",MAX_INFO_NUM )
end

function TianFuFrameOkBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	-- 对条件的判断
	local mainplayer = ActorMgr:getMainPlayer()
	local attrId = TianFuUpFrame:GetClientUserData( 0 )
	local lv = GeniusMgr:getAttrLv(curTianFuType,attrId)
	local geniusAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv )
	--[[
	local nextDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv + 1 )
	if nextDef == nil then
		-- 已经是最高等级
		ShowMidTips(  )
		return
	end
	--]]
	if mainplayer:isOutSide() then
		ShowMidTips( "主角处于跨服状态，无法升级" );
		return;
	end
	if mainplayer:isDead() then
		ShowMidTips( "主角处于死亡状态，无法升级。" );
		return;
	end
	if mainplayer:getLv() < geniusAttrDef.UpLimit.RoleLvlLimit then
		ShowMidTips( "角色等级不足，无法升级。" )
		return
	end

	local Mul = 10000;
	if curTianFuType == 0 then
		local maxLv = GetFengYinData();
		local levelupDef = getLevelUP( maxLv > 0 and maxLv or GameMgr:GetMaxLevel() );
		if levelupDef ~= nil then
			local baseLv = levelupDef.BaseGenius;
			local nType = GeniusMgr:getGeniusType(curTianFuType);
			_, Mul = GetGeniusBonusColor( lv - baseLv, nType );
		end
	end
	if mainplayer:getExp() < math.floor( geniusAttrDef.UpLimit.UpUseExp * Mul / 10000 ) then
		ShowMidTips( "经验不足，无法升级。" )
		return
	end
	if geniusAttrDef.UpLimit.ElseAttrLvlLimit > 0 and
		GeniusMgr:getAllGeniusValue() < geniusAttrDef.UpLimit.ElseAttrLvlLimit then
		ShowMidTips( "需要天赋值达到" ..geniusAttrDef.UpLimit.ElseAttrLvlLimit .. "点才可升级" )
		return
	end

	local useMoney = math.floor( geniusAttrDef.UpLimit.UpUseMoney * Mul / 10000 );
	if getPlayerMoney( mainplayer, moneyType ) < useMoney then
		ShowMidTips( GetMoneyDesc(geniusAttrDef.UpLimit.UpUseMoneyType) .. "不足，无法升级" )
		return
	end

	if geniusAttrDef.UpLimit.UpUseMoneyType == ATTR_ID_BULL and checkNeedWarnCost( useMoney ) then
		CostMoneyConfirmFrame:SetClientString("天赋面板");
		CostMoneyConfirmFrame:SetClientUserData( 0, curTianFuType );
		CostMoneyConfirmFrame:SetClientUserData( 1, attrId );
		CostMoneyConfirmFrame:SetClientUserData( 2, 0 );
		SetWarnFrame( "TianFuUpFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	GeniusMgr:LevelUp( GeniusMgr:getGeniusType( curTianFuType ), attrId, lv, 0 )
end
function TianFuFrameDoubleExpBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	-- 对条件的判断
	local mainplayer = ActorMgr:getMainPlayer()
	local attrId = TianFuUpFrame:GetClientUserData( 0 )
	local lv = GeniusMgr:getAttrLv(curTianFuType,attrId)
	local geniusAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv )
	if mainplayer:isOutSide() then
		ShowMidTips( "主角处于跨服状态，无法升级" );
		return;
	end
	if mainplayer:isDead() then
		ShowMidTips( "主角处于死亡状态，无法升级。" );
		return;
	end
	if mainplayer:getLv() < geniusAttrDef.UpLimit.RoleLvlLimit then
		ShowMidTips( "角色等级不足，无法升级。" )
		return
	end

	local Mul = 10000;
	if curTianFuType == 0 then
		local maxLv = GetFengYinData();
		local levelupDef = getLevelUP( maxLv > 0 and maxLv or GameMgr:GetMaxLevel() );
		if levelupDef ~= nil then
			local baseLv = levelupDef.BaseGenius;
			local nType = GeniusMgr:getGeniusType(curTianFuType);
			_, Mul = GetGeniusBonusColor( lv - baseLv, nType );
		end
	end
	if mainplayer:getExp() < math.floor( geniusAttrDef.UpLimit.UpUseExp * Mul / 10000 ) then
		ShowMidTips( "经验不足，无法升级。" )
		return
	end
	if geniusAttrDef.UpLimit.ElseAttrLvlLimit > 0 and
		GeniusMgr:getAllGeniusValue() < geniusAttrDef.UpLimit.ElseAttrLvlLimit then
		ShowMidTips( "需要天赋值达到" ..geniusAttrDef.UpLimit.ElseAttrLvlLimit .. "点才可升级" )
		return
	end

	GeniusMgr:LevelUp( GeniusMgr:getGeniusType( curTianFuType ), attrId, lv, 1 )
end

--------------------		tips	------------------------
local MAX_TIPS_FONT_NUM = 9

function TianFuTipsFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer()
	local attrId = this:GetClientUserData( 0 )
	local lv = GeniusMgr:getAttrLv(curTianFuType,attrId)
	local geniusAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv )
	local nextAttrDef = GeniusMgr:getGeniusAttrDef( mainplayer:getRace(), attrId, lv + 1)
	
	local IconPath = "uires\\TuPianLei\\TianFu\\"..geniusAttrDef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH
	end
	TianFuTipsFrameTexture:SetTexture( IconPath )
	TianFuTipsFrameName:SetText( geniusAttrDef.Name .."·".. geniusStepName[geniusAttrDef.Step] )
	TianFuTipsFrameName:SetTextColor( geniusStepColor[geniusAttrDef.Step]["r"],geniusStepColor[geniusAttrDef.Step]["g"],geniusStepColor[geniusAttrDef.Step]["b"] )
	TianFuTipsFrameLv:SetText( "等级：" .. (geniusAttrDef.Lvl > 0 and geniusAttrDef.Lvl or "未激活") )

	local yOffset = 10
	local nIdx = 1
	TianFuTipsFrameRich:SetText( geniusAttrDef.AttrDesc, 221,196,159 )
	yOffset = yOffset + TianFuTipsFrameRich:GetTotalHeight() + 10
	-- 等级
	local font = getglobal( "TianFuTipsFrameFont" .. nIdx )
	font:SetText( "当前等级：" .. ( lv > 0 and geniusStepName[geniusAttrDef.Step] .. geniusAttrDef.Lvl .. "级" or "未激活") )
	font:SetTextColor( 255, 255, 255 )
	font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",0, yOffset )
	yOffset = yOffset + 15
	nIdx = nIdx + 1
	-- 属性
	font = getglobal( "TianFuTipsFrameFont" .. nIdx )
	font:SetText( lv > 0 and getGeniusAttrDescText(geniusAttrDef) or "无附加属性" )
	font:SetTextColor( 255,255,0 )
	font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",10, yOffset )
	yOffset = yOffset + 15
	nIdx = nIdx + 1
	-- 分割线
	TianFuTipsFrameLineTex1:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",-17, yOffset )
	yOffset = yOffset + 10
	if nextAttrDef ~= nil then
		font = getglobal( "TianFuTipsFrameFont" .. nIdx )
		font:SetText( "下一等级：" ..  geniusStepName[nextAttrDef.Step] .. nextAttrDef.Lvl .. "级" )
		font:SetTextColor( 255, 255, 255 )
		font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",0, yOffset )
		yOffset = yOffset + 15
		nIdx = nIdx + 1
		-- 下级属性
		font = getglobal( "TianFuTipsFrameFont" .. nIdx )
		font:SetText( getGeniusAttrDescText(nextAttrDef) )
		font:SetTextColor( 255,255,0 )
		font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",10, yOffset )
		yOffset = yOffset + 15
		nIdx = nIdx + 1
		-- 升级条件
		font = getglobal( "TianFuTipsFrameFont" .. nIdx )
		font:SetText( "升级条件：" )
		font:SetTextColor( 255,255,255 )
		font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft", 0, yOffset )
		yOffset = yOffset + 15
		nIdx = nIdx + 1
		-- 其他天赋需求
		font = getglobal( "TianFuTipsFrameFont" .. nIdx )
		if geniusAttrDef.UpLimit.ElseAttrLvlLimit > 0 then
			font:SetText( "天赋需求：天赋值达到" .. geniusAttrDef.UpLimit.ElseAttrLvlLimit .. "点" )
			if GeniusMgr:getAllGeniusValue() < geniusAttrDef.UpLimit.ElseAttrLvlLimit then
				font:SetTextColor( 255,0,0 )
			else
				font:SetTextColor( 66,  168, 61 )
			end
		else
			font:SetText( "天赋需求：无" )
			font:SetTextColor( 66,  168, 61 )
		end
		font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft", 10, yOffset )
		yOffset = yOffset + 15
		nIdx = nIdx + 1
		-- 等级要求
		font = getglobal( "TianFuTipsFrameFont" .. nIdx )
		font:SetText( "角色等级：" .. geniusAttrDef.UpLimit.RoleLvlLimit )
		if mainplayer:getLv() < geniusAttrDef.UpLimit.RoleLvlLimit then
			font:SetTextColor( 255,0,0 )
		else
			font:SetTextColor( 66,  168, 61 )
		end
		
		font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft", 10, yOffset )
		yOffset = yOffset + 15
		nIdx = nIdx + 1		
	else
		-- 等级满
		font = getglobal( "TianFuTipsFrameFont" .. nIdx )
		font:SetText( "已升到最高等级" )
		font:SetTextColor( 255,60,0 )
		font:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft", 10, yOffset )
		yOffset = yOffset + 15
		nIdx = nIdx + 1
	end
	TianFuTipsFrameLineTex2:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",-17, yOffset )
	yOffset = yOffset + 10

	TianFuTipsFrameNextRich:SetText( "注：点击天赋图标打开升级面板，可进行天赋升级。", 221,196,159 )
	TianFuTipsFrameNextRich:SetPoint( "topleft","TianFuTipsFrameTexture","bottomleft",0, yOffset )
	yOffset = yOffset + TianFuTipsFrameNextRich:GetTotalHeight()

	for i=nIdx,MAX_TIPS_FONT_NUM do
		font = getglobal( "TianFuTipsFrameFont" .. i )
		font:SetText( "" )
	end

	TianFuTipsFrame:SetSize( 280, 65 + yOffset )

	local szBtnName = TianFuTipsFrame:GetClientString();
	if szBtnName ~= nil then
		local nWidth    = TianFuTipsFrame:GetWidth();
		local nHeight   = TianFuTipsFrame:GetHeight();

		local curSelectBtn = getglobal(szBtnName);
		local nRealTop  = curSelectBtn:GetRealTop();
		local nRight    = curSelectBtn:GetRight();

		if ( nRealTop - nHeight ) > 0 then
			if ( nRight + nWidth < GetScreenWidth() ) then
				TianFuTipsFrame:SetPoint("BottomLeft",szBtnName,"TopRight",0,-10);
			else
				TianFuTipsFrame:SetPoint("BottomRight",szBtnName,"TopLeft",0,-10);
			end
		else
			if ( nRight + nWidth < GetScreenWidth() ) then
				TianFuTipsFrame:SetPoint("TopLeft",szBtnName,"BottomRight",0,10);
			else
				TianFuTipsFrame:SetPoint("TopRight",szBtnName,"BottomLeft",0,10);
			end
		end
	end
end

function TianFuTipsFrame_OnHide()
end