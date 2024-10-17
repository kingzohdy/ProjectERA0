
function getSkillInfo(SkillDef,szText)
	local SkillInfo=
	{
				{"@Level@",                       SkillDef.Level},								--/*   等级*/
				{"@AttackActTime@",				  SkillDef.AttackActTime/10},
				{"@Distance@",                    SkillDef.Distance/100 },                      --/*   射程 */
				{"@TargetNum@",                   SkillDef.TargetNum},                       	--/*   范围攻击时同时伤害的对象个数 */
                
                {"@IceHurtMax@",                  SkillDef.IceHurtMax },						--/*   最大法术伤害*/	
                {"@IceHurtMin@",                  SkillDef.IceHurtMin},							--/*   最大法术伤害*/	 
                {"@PhsicHurtMax@",                SkillDef.PhsicHurtMax},						--/*   最大物理伤害*/	 
                {"@PhsicHurtMin@",                SkillDef.PhsicHurtMin},						--/*   最小物理伤害*/  

				{"@IceHurtMulMax@",               ( SkillDef.IceHurtMulMax + 100 ) },			--/*   最大法术伤害倍数*/	 
                {"@IceHurtMul@",				  ( SkillDef.IceHurtMul + 100 ) },				--/*   最小法术伤害倍数*/ 
				{"@PhsicHurtMulMax@",             ( SkillDef.PhsicHurtMulMax + 100 ) },			--/*   最大物理伤害倍数*/	 
                {"@PhsicHurtMul@",				  ( SkillDef.PhsicHurtMul + 100 ) },			--/*   最小物理伤害倍数*/  

				{"@MainRangeRangeVal1@",		  SkillDef.MainRange.RangeVal1/100},			--/*  技能主范围 半径或宽*/
				
				--策划需求，8*4，加的有点儿篇幅太长。。。
				{"@SkillResult1_SkillResultVal1@",		  SkillDef.SkillResult[0].SkillResultVal1},
				{"@SkillResult1_SkillResultVal2@",		  SkillDef.SkillResult[0].SkillResultVal2},
				{"@SkillResult1_SkillResultVal3@",		  SkillDef.SkillResult[0].SkillResultVal3},
				{"@SkillResult1_SkillResultVal4@",		  SkillDef.SkillResult[0].SkillResultVal4},
				
				{"@SkillResult2_SkillResultVal1@",		  SkillDef.SkillResult[1].SkillResultVal1},
				{"@SkillResult2_SkillResultVal2@",		  SkillDef.SkillResult[1].SkillResultVal2},
				{"@SkillResult2_SkillResultVal3@",		  SkillDef.SkillResult[1].SkillResultVal3},
				{"@SkillResult2_SkillResultVal4@",		  SkillDef.SkillResult[1].SkillResultVal4},
				
				{"@SkillResult3_SkillResultVal1@",		  SkillDef.SkillResult[2].SkillResultVal1},
				{"@SkillResult3_SkillResultVal2@",		  SkillDef.SkillResult[2].SkillResultVal2},
				{"@SkillResult3_SkillResultVal3@",		  SkillDef.SkillResult[2].SkillResultVal3},
				{"@SkillResult3_SkillResultVal4@",		  SkillDef.SkillResult[2].SkillResultVal4},
				
				{"@SkillResult4_SkillResultVal1@",		  SkillDef.SkillResult[3].SkillResultVal1},
				{"@SkillResult4_SkillResultVal2@",		  SkillDef.SkillResult[3].SkillResultVal2},
				{"@SkillResult4_SkillResultVal3@",		  SkillDef.SkillResult[3].SkillResultVal3},
				{"@SkillResult4_SkillResultVal4@",		  SkillDef.SkillResult[3].SkillResultVal4},
				
				{"@SkillResult5_SkillResultVal1@",		  SkillDef.SkillResult[4].SkillResultVal1},
				{"@SkillResult5_SkillResultVal2@",		  SkillDef.SkillResult[4].SkillResultVal2},
				{"@SkillResult5_SkillResultVal3@",		  SkillDef.SkillResult[4].SkillResultVal3},
				{"@SkillResult5_SkillResultVal4@",		  SkillDef.SkillResult[4].SkillResultVal4},
				
				{"@SkillResult6_SkillResultVal1@",		  SkillDef.SkillResult[5].SkillResultVal1},
				{"@SkillResult6_SkillResultVal2@",		  SkillDef.SkillResult[5].SkillResultVal2},
				{"@SkillResult6_SkillResultVal3@",		  SkillDef.SkillResult[5].SkillResultVal3},
				{"@SkillResult6_SkillResultVal4@",		  SkillDef.SkillResult[5].SkillResultVal4},
				
				{"@SkillResult7_SkillResultVal1@",		  SkillDef.SkillResult[6].SkillResultVal1},
				{"@SkillResult7_SkillResultVal2@",		  SkillDef.SkillResult[6].SkillResultVal2},
				{"@SkillResult7_SkillResultVal3@",		  SkillDef.SkillResult[6].SkillResultVal3},
				{"@SkillResult7_SkillResultVal4@",		  SkillDef.SkillResult[6].SkillResultVal4},
				
				{"@SkillResult8_SkillResultVal1@",		  SkillDef.SkillResult[7].SkillResultVal1},
				{"@SkillResult8_SkillResultVal2@",		  SkillDef.SkillResult[7].SkillResultVal2},
				{"@SkillResult8_SkillResultVal3@",		  SkillDef.SkillResult[7].SkillResultVal3},
				{"@SkillResult8_SkillResultVal4@",		  SkillDef.SkillResult[7].SkillResultVal4},
	};

	local nSize = table.getn(SkillInfo);
	for i = 1 , nSize do
		if string.find( szText, SkillInfo[i][1], 1, true ) then
			szText = string.gsub(szText,SkillInfo[i][1],SkillInfo[i][2]);
		end
	end
	return szText;
end

local t_SpellNextLevelColor = {221,196,159};
local MAX_SPELLTIPS_FONT_NUM	= 10;
local MAX_SPELLTIP_LINE_NUM		= 7;
local MAX_CLANTIP_LINE_NUM		= 5;
local SPELL_LINE_DISTANCE		= 10;
local MAX_ARM_SKILL_EFFECT_NUM  = 5;

local PREPARE_TYPE_PER = 1;
local TIPS_FONT_HEIGHT = 12;
local MAX_DIATHESISI_LV = 10;

-- 技能tips
local t_SpellTipsRelFrame = {};
function SetSpellTipsRelFrame( data )
	t_SpellTipsRelFrame = data;
end

function GetSpellTipsRelFrame( data )
	return t_SpellTipsRelFrame;
end

function ClearSpellTipsRelFrame()
	t_SpellTipsRelFrame = {};
end

local t_SpellTipsFrameControl = { id = 0, level = 0, };

function GetSpellTipsFrameControl()
	return t_SpellTipsFrameControl;
end

function SpellTipsFrame_OnLoad()
	this:SetClientUserData( 0, 0 );	 
	this:SetClientUserData( 1, 0 );		 
	this:SetClientUserData( 2, 0 );
	this:SetClientUserData( 3, 0 );
end

function ClearSpellTips()
	for i = 1, MAX_SPELLTIPS_FONT_NUM do
		local font = getglobal("SpellTipsfont"..i);
		font:SetText("");
		font:SetTextColor(255, 255, 255);
		font:Hide();
	end

	SpellTipsLearnMode:Hide();
	SpellTipsRune:Hide();

	for i = 1,MAX_SPELLTIP_LINE_NUM do
		local LineTex = getglobal("SpellTipsITItemLineTex"..i);
		LineTex:Hide();
	end
	
	local t_riches = { "SpellTipsRichText", "SpellNextLevelRichText" };	
	for _, name in ipairs( t_riches ) do
		local rich = getglobal( name );
		rich:Hide();
		rich:Clear();
		rich:clearHistory();
	end

	for i = 1, MAX_ARM_SKILL_EFFECT_NUM do
		local font = getglobal( "ArmSkillTipsFont" .. i );
		font:SetText("");
		font:SetTextColor( 0, 220, 0 );
	end

	SpellTipsNextLvBtn:Hide();
end

t_SkillType = { [SKILL_TYPE] = { desc = "SKILL_TYPE", }, [DIATHESIS_TYPE] = { desc = "DIATHESIS_TYPE", }, 
				[TRUMP_SKILL_TYPE] = { desc = "TRUMP_SKILL_TYPE", } 
			};

--用来监测技能是否附带某种效果			
function CheckSkillResult( SkillDef, nSkillResult )
	for i=1,8 do
		if SkillDef.SkillResult[i-1].SkillResultID == nSkillResult then
			return true
		end
	end
	return false
end

function SpellTipsFrame_OnShow( )
	ClearSpellTips();
	local id = SpellTipsFrame:GetClientUserData( 0 );
	if id <= 0 or id == nil then
		SpellTipsFrame:Hide();
		return;
	end
	local frame = getglobal( "SpellTipsFrame" );
	SpellTipsTexture:SetPoint( "topleft", "SpellTipsFrame", "topleft", 18, 12 );
	local lv = SpellTipsFrame:GetClientUserData( 1 );
	if SpellTipsFrame:GetClientUserData( 2 ) ~= DIATHESIS_TYPE then
		local nFontHeight	= 15;
		local nFrameHeight	= 0;
		local nIndex = 0;
		local nLineIndex = 1;
		local font = nil;

		local level  = SkillMgr:getSpellLv( id );

		if SpellTipsFrame:GetClientUserData( 3 ) == TRUMP_SKILL_TYPE or SpellTipsFrame:GetClientUserData( 3 ) == TMP_TANK_SKILL_TYPE
			or SpellTipsFrame:GetClientUserData( 3 ) == RUNE_SKILL_TYPE then
			level = lv;
		end
		--开始设置TIPS
		local SkillDef		= SkillMgr:getSpellStaticAtt(id, level);
		if SkillDef == nil then
			SpellTipsFrame:Hide();	
			return;
		end
		local mainplayer = ActorMgr:getMainPlayer();
		
		local bShowFitting	= false;
		local mpUse			= SkillDef.MPUse;
		local xpUse			= SkillDef.UseXp;
		local collectUse	= SkillDef.CollectSkillUse;
		local energyUse		= SkillDef.TmpEnergeUse;
		local fuelUse		= SkillDef.EnergyUse;
		local selfCool		= SkillDef.SelfCool;
		local distance		= SkillDef.Distance;
		local tagLearn		= SkillDef.Learn;
		local szText		= SkillDef.SkillDesc;

		local IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga";
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		SpellTipsTexture:SetTexture( IconPath );

		SpellTipsName:SetText(SkillDef.Name);

		--等级
		font = getglobal("SpellTipsCountFont");
		if level > 0 then
			font:SetText("lv"..level);
			font:Show();
		else
			font:Hide();
		end
		
		local race = mainplayer:getRace();
		if race % 0x10 == race then
			local career = SkillDef.Learn.Career;
			if career ~= race and career ~= 0 then
				nIndex = nIndex + 1;
				font = getglobal("SpellTipsfont"..nIndex);
				font:SetText("职业要求 : "..(t_raceList[career] or "∞"))
				font:SetTextColor(255, 80, 80);
				font:Show();
			end
		end
		
		--武器类型
		local Arms = {}
		for i = 0, SkillMgr.MAX_ARMTYPE - 1 do
			local arm = SkillMgr:getSpellWeaponType(id, level, i)
			if arm ~= 0 and arm ~= ARM_ANY then
				local s = GetItemTypeName(arm)
				if s then
					table.insert(Arms, s)
				end
			end
		end
		if #Arms ~= 0 then
			nIndex = nIndex + 1;
			font = getglobal("SpellTipsfont"..nIndex);
			font:SetText("武器类型 : "..table.concat(Arms, "/"))
			if not SkillMgr:castWeapon(id, level) then
				font:SetTextColor(255, 80, 80);
			end
			font:Show();
		end

		--技能消耗
		if mpUse > 0 or xpUse > 0 or collectUse > 0 or fuelUse > 0 then
			nIndex = nIndex + 1;
			font = getglobal("SpellTipsfont"..nIndex);
			if mpUse > 0 then
				font:SetText("技能消耗 : "..mpUse.." 魔法值");
				if not SkillMgr:castMP(id, level) then
					font:SetTextColor(255, 80, 80);
				end
			elseif  xpUse > 0 then
				font:SetText("技能消耗 : "..xpUse.." 魂能");
				if not SkillMgr:castXP(id, level) then
					font:SetTextColor(255, 80, 80);
				end
			elseif collectUse > 0 then
				font:SetText("技能消耗 : "..collectUse.." 战意");
				if not SkillMgr:castCollect(id, level) then
					font:SetTextColor(255, 80, 80);
				end
			elseif energyUse > 0 then
				font:SetText("技能消耗 : "..energyUse.." 能量");
				if not SkillMgr:castEnergy(id, level) then
					font:SetTextColor(255, 80, 80);
				end
			elseif fuelUse > 0 then
				font:SetText("技能消耗 : "..fuelUse.." 燃料");
				local monsterMachine = mainplayer:getMonsterMachine():getPlayerMonsterMachine();
				local fuel = monsterMachine:getEnergy();
				if fuel < fuelUse then
					font:SetTextColor(255, 80, 80);
				end
			end
			font:Show();
		end

		--施放距离
		if distance > 0 then
			nIndex = nIndex + 1;
			font = getglobal("SpellTipsfont"..nIndex);
			distance = distance / 100;
			if distance <= 3 then
				font:SetText("施放距离 : 近身");
			else
				if math.floor(distance) == distance then
					font:SetText("施放距离 : "..distance.."米");
				else
					font:SetText("施放距离 : "..string.format("%.1f", distance).."米");
				end
			end
			font:Show();
		end

		-- 吟唱时间
		local PrepareTime = SkillMgr:getSkillPrepareTime(id, level);
		if PrepareTime ~= 0 then
			nIndex = nIndex + 1;
			font = getglobal("SpellTipsfont"..nIndex);
			PrepareTime = PrepareTime / 10;
			font:SetText("吟唱时间 : "..PrepareTime.."秒");
			font:Show();
		else
			PrepareTime = SkillMgr:getSkillCollectTime(id, level) * SkillMgr:getSkillCollectNum(id, level);
			if PrepareTime ~= 0 then
				nIndex = nIndex + 1;
				font = getglobal("SpellTipsfont"..nIndex);
				PrepareTime = PrepareTime / 10;
				font:SetText("集气时间 : "..PrepareTime.."秒");
				font:Show();
			end
		end
		
		--冷却时间
		if selfCool > 0 then
			nIndex = nIndex + 1;
			font = getglobal("SpellTipsfont"..nIndex);
			if selfCool%10 ~= 0 then
				selfCool = string.format("%.1f",selfCool/10);
			else
				selfCool = selfCool/10;
			end
			font:SetText("冷却时间 : "..selfCool.."秒");
			font:Show();
		end

		-- 积累数上限  挂起。
		-- todo:add  CollectNumMax

		-- 积累方式 在线、/战斗、定时 挂起。
		local SpellTipsLineTex = getglobal("SpellTipsITItemLineTex"..nLineIndex);
		if nIndex > 2 then
			nFrameHeight = nIndex * (nFontHeight + 2) - 24;
		else
			nFrameHeight = nFontHeight;
		end
		SpellTipsLineTex:SetPoint( "bottomleft", "SpellTipsTexture", "bottomleft", -18, nFrameHeight );
		SpellTipsLineTex:Show();
		nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
		nLineIndex = nLineIndex + 1;
		-- 描述
		if szText ~= "" then
			--设置窗口位置
			szText = getSkillInfo( SkillDef, szText );
			SpellTipsRichText:resizeRichWidth(245 * GetScreenScaleY());
			SpellTipsRichText:resizeRichHeight(100);			
			SpellTipsRichText:SetText( szText,221,196,159 );
			nFrameHeight = nFrameHeight + SpellTipsRichText:GetTotalHeight()/GetScreenScaleY();
			SpellTipsRichText:SetPoint( "topleft", ("SpellTipsITItemLineTex"..(nLineIndex-1)), "bottomleft", 18, SPELL_LINE_DISTANCE );
			SpellTipsRichText:Show();
			nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
		end
		
		if not SkillMgr:isSkillLearned(id) then
			local s
			if GameSkillLearn:isSkillAutoLearn(id, 1) then
				--s = "自动领悟"
			elseif GameSkillLearn:isSkillNeedBookLearn(id, 1) then
				s = "通过技能书领悟"
			end
			if s then
				SpellTipsLearnMode:SetPoint( "topleft", "SpellTipsTexture", "bottomleft", 0, nFrameHeight - 10);
				SpellTipsLearnMode:Show();
				SpellTipsLearnMode:SetText(s);
				nFrameHeight = nFrameHeight + 15;
			end
		end

		--可升级
		local lvUpFont = getglobal("SpellTipsCanLevelUp");
		lvUpFont:Hide();
		
		local runeItemId = SkillMgr:getSpellRuneEnable(id) and SkillMgr:getSpellRuneItemId(id) or 0
		if runeItemId ~= 0 then
			local runeItemDef = getItemDef(runeItemId)
			local runeType = RuneMgr:RuneItemType(runeItemId)
			local SpellTipsLineTex = getglobal("SpellTipsITItemLineTex"..nLineIndex);
			SpellTipsLineTex:SetPoint( "bottomleft", "SpellTipsTexture", "bottomleft", -18, nFrameHeight  );
			SpellTipsLineTex:Show();

			nLineIndex = nLineIndex + 1;
			nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
			
			SpellTipsRune:SetPoint( "topleft", "SpellTipsTexture", "bottomleft", -18, nFrameHeight);
			SpellTipsRune:Show();
			SpellTipsRuneTitle:SetText(runeItemDef.Name);
			SpellTipsRuneText:SetText(SkillMgr:getSpkillRuneDesc(id, level, runeType));
			nFrameHeight = nFrameHeight + 36;
		end

		local armBonus, skillData = checkArmSkill( id );
		if armBonus then
			SpellTipsLineTex = getglobal( "SpellTipsITItemLineTex"..nLineIndex );
			SpellTipsLineTex:SetPoint( "topleft", "SpellTipsTexture", "bottomleft", -18, nFrameHeight  );
			SpellTipsLineTex:Show();

			nLineIndex = nLineIndex + 1;
			nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;

			local idx = 1;
			font = getglobal("ArmSkillTipsFont"..idx);
			font:SetText("装备加成");
			font:SetPoint( "topleft", SpellTipsLineTex:GetName(), "topleft", 18, SPELL_LINE_DISTANCE );
			nFrameHeight = nFrameHeight + 24;
			for i,val in pairs( skillData ) do
				idx = idx + 1;
				font = getglobal("ArmSkillTipsFont"..idx);
				font:SetPoint( "topleft", SpellTipsLineTex:GetName(), "topleft", 18, 15 * idx - 5 );
				nFrameHeight = nFrameHeight + 15;
				--治疗技能特殊处理下
				if CheckSkillResult( SkillDef, RESULT_MAGIC_HP_ADD ) then
					font:SetText( "使用时治疗效果：+" .. val .. "点" );
				elseif i == 0 then
					font:SetText( "使用时" .. getAttackType( SkillDef.MainAttr ) .. "攻击力：+" .. val .. "点" );
				elseif i == 1 then
					font:SetText( "使用时技能最终伤害：+" .. val .. "%" );
				end
			end
		end

		local NextSkillDef  = SkillMgr:getSpellStaticAtt(id, level + 1);
		--精灵技能不显示下一级
		if NextSkillDef ~= nil and SkillDef.NextLevelDesc ~= "" and  SpellTipsFrame:GetClientUserData( 3 )~= TRUMP_SKILL_TYPE and
			( SkillMgr:isSkillLearned(id) or SpellTipsFrame:GetClientUserData( 3 ) == RUNE_SKILL_TYPE ) then
			local SpellTipsLineTex = getglobal("SpellTipsITItemLineTex"..nLineIndex);
			SpellTipsLineTex:SetPoint( "bottomleft", "SpellTipsTexture", "bottomleft", -18, nFrameHeight  );
			SpellTipsLineTex:Show();
			nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
			
			local btn = getglobal( "SpellTipsNextLvBtn" );
			btn:SetPoint( "topleft", ("SpellTipsITItemLineTex"..nLineIndex), "topleft", 18, SPELL_LINE_DISTANCE );
			
			local bCanLvUP = false;

			local lv = NextSkillDef.Learn.LevelMin;
			local nextLvFont = getglobal( btn:GetName() .. "NextLvNumFont" );
			if lv > 0 then
				local mainplayer = ActorMgr:getMainPlayer();
				if mainplayer and mainplayer:getLv() < lv then
					nextLvFont:SetText( lv .. " 级" );
					nextLvFont:SetTextColor( 255, 80, 80 );
				else
					nextLvFont:SetText( lv .. " 级" );
					nextLvFont:SetTextColor( 160, 160, 90 );
					bCanLvUP = true;
					lvUpFont:Show();
				end
			end

			local nextDisFont = getglobal( btn:GetName() .. "NextDisFont" );
			local distance = NextSkillDef.Distance;
			nextDisFont:SetText("");
			if distance > 0 then
				distance = distance / 100;
				if distance <= 3 then
					--nextDisFont:SetText("施放距离 : 近身");
				else
					if math.floor(distance) == distance then
						nextDisFont:SetText("施放距离 : "..distance.." 米");
					else
						nextDisFont:SetText("施放距离 : "..string.format("%.1f", distance).." 米");
					end
				end
			end

			local nextCostFont = getglobal( btn:GetName() .. "NextCostFont" );
			local mpUse			= NextSkillDef.MPUse;
			local xpUse			= NextSkillDef.UseXp;
			local collectUse	= NextSkillDef.CollectSkillUse;
			local energyUse		= NextSkillDef.TmpEnergeUse;
			local fuelUse		= NextSkillDef.EnergyUse;
			if mpUse > 0 or xpUse > 0 or collectUse > 0 or fuelUse > 0 then
				if mpUse > 0 then
					nextCostFont:SetText("技能消耗 : "..mpUse.." 魔法值");
				elseif  xpUse > 0 then
					nextCostFont:SetText("技能消耗 : "..xpUse.." 魂能");
				elseif collectUse > 0 then
					nextCostFont:SetText("技能消耗 : "..collectUse.." 战意");
				elseif energyUse > 0 then
					nextCostFont:SetText("技能消耗 : "..energyUse.." 能量");
				elseif fuelUse > 0 then
					nextCostFont:SetText("技能消耗 : "..fuelUse.." 燃料");
				end
			end

			local nextCoolFont = getglobal( btn:GetName() .. "NextCoolFont" );
			local selfCool = NextSkillDef.SelfCool;
			if selfCool > 0 then
				if selfCool%10 ~= 0 then
					selfCool = string.format("%.1f",selfCool/10);
				else
					selfCool = selfCool/10;
				end
				nextCoolFont:SetText( "冷却时间 : "..selfCool.." 秒" );
			else
				nextCoolFont:SetText( "冷却时间 : 无" );
			end

			local rich = getglobal("SpellNextLevelRichText");
			if SkillDef.NextLevelDesc ~= "" then
				rich:AddText( SkillDef.NextLevelDesc, 160, 160, 90 );
			end
			rich:SetHeight( rich:GetTextLines() * nFontHeight + 2*nFontHeight );
			rich:SetPoint( "topleft", nextCostFont:GetName(), "bottomleft", 0, SPELL_LINE_DISTANCE);
			
			rich:Show();
			btn:Show();
			nFrameHeight = nFrameHeight + rich:GetTextLines() * nFontHeight + 70;

			if bCanLvUP then
				nLineIndex = nLineIndex + 1;
				SpellTipsLineTex = getglobal( "SpellTipsITItemLineTex"..nLineIndex );
				SpellTipsLineTex:SetPoint( "bottomleft", "SpellTipsTexture", "bottomleft", -18, nFrameHeight  );
				SpellTipsLineTex:Show();
				lvUpFont:SetText( "此技能可升级，请在技能训练师处升级" );
				lvUpFont:SetPoint("topleft", ("SpellTipsITItemLineTex"..nLineIndex), "topleft", 18, SPELL_LINE_DISTANCE);
				nFrameHeight = nFrameHeight + 32;
			end
		end

		SpellTipsFrame:SetHeight( nFrameHeight - nFontHeight + 78 );

		 --调整窗口位置
		local szBtnName = SpellTipsFrame:GetClientString();
		if szBtnName ~= nil then
			local nWidth    = SpellTipsFrame:GetWidth();
			local nHeight   = SpellTipsFrame:GetHeight();

			local curSelectBtn = getglobal(szBtnName);
			local nRealTop  = curSelectBtn:GetRealTop();
			local nRight    = curSelectBtn:GetRight();

			if ( nRealTop - nHeight ) > 0 then
				if ( nRight + nWidth < GetScreenWidth() ) then
					SpellTipsFrame:SetPoint("BottomLeft",szBtnName,"TopRight",0,-10);
				else
					SpellTipsFrame:SetPoint("BottomRight",szBtnName,"TopLeft",0,-10);
				end
			else
				if ( nRight + nWidth < GetScreenWidth() ) then
					SpellTipsFrame:SetPoint("TopLeft",szBtnName,"BottomRight",0,10);
				else
					SpellTipsFrame:SetPoint("TopRight",szBtnName,"BottomLeft",0,10);
				end
			end
		end
	else
		local nLineIndex= 1;
		local nRow = 0;
		local name = "";
		local szText = "";
		local nLv = 0;
		local nIcon	= 0;
		local IconPath = DEFAULT_ITEM_PATH;

		local DiathesisDef = SkillMgr:getDiathesisDef(id, lv );
		if lv == 0 then
			DiathesisDef = SkillMgr:getDiathesisDef(id, 1);
		end

		if DiathesisDef == nil then
			SpellTipsFrame:Hide();
			return;
		end
		name		= DiathesisDef.Name;
		nLineIndex	= 1;
		nRow		= 0;
		szText		= DiathesisDef.DiathesisDesc;
		nLv			= DiathesisDef.Level;
		IconPath = getDiathesisPath()..DiathesisDef.IconID..".tga";
		
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end

		SpellTipsTexture:SetTexture( IconPath );
		SpellTipsName:SetText(name);

		--等级
		font = getglobal("SpellTipsCountFont");
		font:SetText("");
		if lv > 0 then
			font:SetText("lv"..nLv);
			font:Show();
		end
		local nCurHeight	= 85;
		local INIT_HEIGHT	= 100;
		local nRichYOffset	= 20;
		if SpellTipsFrame:GetClientUserData( 3 ) == TRUMP_SKILL_TYPE then
			SpellTipsRichText:Hide();
		else
			-- TODO: 这儿要设置宽度
			-- SpellTipsFrame:GetWidth()
			local nXAdjust		= 5;
			
			local nWidth	= SpellTipsFrame:GetWidth() - 48 - 18 - 10 - nXAdjust;
			SpellTipsRichText:resizeRichWidth( nWidth );
			
			SpellTipsRichText:resizeRichHeight( INIT_HEIGHT );	
			
			SpellTipsRichText:SetPoint( "topleft", "SpellTipsTexture", "topright", 10,nRichYOffset );
			
			--print( "SpellTipsFrame_OnShow--szText = "..szText.."--411" );
			SpellTipsRichText:SetText( szText, 221,196,159 );			
			nRow = SpellTipsRichText:GetTotalHeight();
			SpellTipsRichText:Show();
			--nCurHeight = nCurHeight + SpellTipsRichText:GetTotalHeight();
		end
		
		SpellNextLevelRichText:Hide();
		
		local nRichRow = 0;
		local mainplayer	= ActorMgr:getMainPlayer();
		local diathesisMgr	= mainplayer:getDiathesisManager();
		local nextdiathesisiDef = getDiathesisDef( id, lv+1 );
		local szDes = "学习";
		if lv ~= 0 then
			szDes = "升级";
		end
		
		local SpellTipsLineTex	= getglobal("SpellTipsITItemLineTex1");
		local nRichTotalHeight	= SpellTipsRichText:GetTotalHeight();
		--print( "nRichTotalHeight = "..nRichTotalHeight );
		local nCurExactYOffset	= nRichYOffset + 12 + nRichTotalHeight + 5;
		local INIT_TEX_Y_OFFSET = 72;
		if nCurExactYOffset > INIT_TEX_Y_OFFSET then
			--print( "nCurExactYOffset - INIT_TEX_Y_OFFSET--"..( nCurExactYOffset - INIT_TEX_Y_OFFSET ) );
			SpellTipsTexture:SetPoint( "topleft", "SpellTipsFrame", "topleft", 18, 12 + ( nCurExactYOffset - INIT_TEX_Y_OFFSET ) / 2 );
		end

		SpellTipsITItemLineTex1:SetPoint( "topleft", "SpellTipsFrame", "topleft", 0, math.max( INIT_TEX_Y_OFFSET, nCurExactYOffset ) );
		SpellTipsLineTex:Show();
		SpellNextLevelRichText:Clear();
		--SpellNextLevelRichText:SetPoint( "Topleft", "SpellTipsTexture", "bottomleft", 0, 20);
		SpellNextLevelRichText:SetPoint( "Topleft", "SpellTipsITItemLineTex1", "Topleft", 18, 7 );
		local nXAdjust	= 5;
		local nWidth	= SpellTipsFrame:GetWidth() - 48 - 18 - 20 - nXAdjust;
		SpellNextLevelRichText:resizeRichWidth(nWidth);
		SpellNextLevelRichText:resizeRichHeight(100);

		if DiathesisFrame:IsShown() and isPointInFrame( "DiathesisFrame" ) then
			if nextdiathesisiDef ~= nil
			and (  nextdiathesisiDef.Learn.PreDiathesisLevel ~= 0 
				or nextdiathesisiDef.PreUsedGeniusPoint ~= 0 
				or nextdiathesisiDef.UseGeniusPoint ~= 0 
				or nextdiathesisiDef.Learn.LevelMin ~= 0  
				or nextdiathesisiDef.Learn.Career ~= 0 ) then		
					
				if nextdiathesisiDef.Learn.PreDiathesisLevel ~= 0 then
					SpellNextLevelRichText:AddText( "前置天赋等级:"..nextdiathesisiDef.Learn.PreDiathesisLevel, 255,216,0 );
				end
				if nextdiathesisiDef.PreUsedGeniusPoint ~= 0 then
					SpellNextLevelRichText:AddText( "前置消耗的天赋点:"..nextdiathesisiDef.PreUsedGeniusPoint, 255,216,0 );
				end
				if nextdiathesisiDef.UseGeniusPoint ~= 0 then
					SpellNextLevelRichText:AddText( szDes.."本天赋需天赋点:"..nextdiathesisiDef.UseGeniusPoint, 255,216,0 );
				end
				if nextdiathesisiDef.Learn.LevelMin ~= 0 then
					SpellNextLevelRichText:AddText( "学习该天赋所需等级:"..nextdiathesisiDef.Learn.LevelMin, 255,216,0 );
				end
				if nextdiathesisiDef.Learn.Career ~= 0 then
					SpellNextLevelRichText:AddText( "可学习该天赋的职业:"..GetCareerName( nextdiathesisiDef.Learn.Career ), 255,216,0 );
				end
				
				local nTotalGeniusPoint = 0;
				for i = 1, MAX_DIATHESISI_LV do
					local diathesisiDef = getDiathesisDef( id, i );
					if diathesisiDef == nil then
						break;
					end
					nTotalGeniusPoint = nTotalGeniusPoint + diathesisiDef.UseGeniusPoint;
				end
				SpellNextLevelRichText:AddText( "达到最高等级所需天赋点:"..nTotalGeniusPoint, 255,216,0 );
			end
			
			--print( "lv = "..lv );
			if lv ~= 0 then
				SpellNextLevelRichText:AddText( "当前效果:", 255,255,255 );
				SpellNextLevelRichText:AddText( DiathesisDef.DiathesisDesc, 255,255,255 );
			end

			if nextdiathesisiDef ~= nil then
				SpellNextLevelRichText:AddText( "下一等级效果:", 255,216,0 );
				SpellNextLevelRichText:AddText( nextdiathesisiDef.DiathesisDesc, 255,216,0 );
				SpellNextLevelRichText:AddText( "#r（双击即可"..szDes.."该天赋）", 212,184,149 );
			end
			if nextdiathesisiDef == nil then	
				SpellNextLevelRichText:AddText( "已达到最高天赋等级！", 212,184,149 );
			end
		else
			local szBtnName = SpellTipsFrame:GetClientString();
			if string.find( szBtnName,"TrumpLevelUpSkillBtn",1,true ) or
				string.find( szBtnName,"TrumpSkillBtn",1,true ) or
				string.find( szBtnName,"XPBufShortcut",1,true ) then
				SpellNextLevelRichText:AddText( szText, 221,196,159 );
			end
		end

		SpellNextLevelRichText:ScrollFirst();
		SpellNextLevelRichText:SetHeight(SpellNextLevelRichText:GetTotalHeight()/GetScreenScaleY());
		nCurHeight = nCurHeight + SpellNextLevelRichText:GetTotalHeight()/GetScreenScaleY();
		SpellNextLevelRichText:Show();

		SpellTipsFrame:SetHeight( nCurHeight );
		 --调整窗口位置
		local szBtnName = SpellTipsFrame:GetClientString();
		if szBtnName ~= nil then
			local nWidth    = SpellTipsFrame:GetWidth();
			local nHeight   = SpellTipsFrame:GetHeight();

			local curSelectBtn = getglobal(szBtnName);
			local nRealTop  = curSelectBtn:GetRealTop();
			local nRight    = curSelectBtn:GetRight();

			if ( nRealTop - nHeight ) > 0 then
				if ( nRight + nWidth < GetScreenWidth() ) then
					SpellTipsFrame:SetPoint("BottomLeft",szBtnName,"TopRight",0,-10);
				else
					SpellTipsFrame:SetPoint("BottomRight",szBtnName,"TopLeft",0,-10);
				end
			else
				if ( nRight + nWidth < GetScreenWidth() ) then
					SpellTipsFrame:SetPoint("TopLeft",szBtnName,"BottomRight",0,10);
				else
					SpellTipsFrame:SetPoint("TopRight",szBtnName,"BottomLeft",0,10);
				end
			end

		end
	end
end

function SpellTipsFrame_OnHide()
	this:SetClientString( "" );	   
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );	  
	this:SetClientUserData( 2, 0 );	 
	this:SetClientUserData( 3, 0 );
end

function SpellTipsFrame_OnUpdate()
	for _, name in ipairs( t_SpellTipsRelFrame ) do
		local frame = getglobal( name );
		if not frame:IsShown() then
			this:Hide();
			ClearSpellTipsRelFrame();
			return;
		end
	end
end

-- 技能tips
function OnSpellTipsOnHide()
	this:SetClientString( "" );
end

function OnSpellTooltipOnLoad()
	this:SetClientString( "" );
end

local t_GameTooltipRelFrame = {};
function SetGameTooltipRelFrame( data )
	t_GameTooltipRelFrame = data;
end

function GetGameTooltipRelFrame( data )
	return t_GameTooltipRelFrame;
end

function ClearGameTooltipRelFrame()
	t_GameTooltipRelFrame = {};
end

local t_GameTooltipRelBtn = {};
function SetGameTooltipRelBtn( data )
	t_GameTooltipRelBtn = data;
end

function ClearGameTooltipRelBtn()
	t_GameTooltipRelBtn = {};
end

function GameTooltipOnHide()
	ClearGameTooltipRelBtn();
	local moneyTexture	=	getglobal( "GameTooltipAddTextureMoneyTex" );
	if moneyTexture ~= nil then
		moneyTexture:Hide();
	end
	
end

function GameTooltipOnUpdate()
	for _, name in ipairs( t_GameTooltipRelBtn ) do
		local btn = getglobal( name );
		if not btn:IsEnable() then
			this:Hide();
			ClearGameTooltipRelBtn();
			return;
		end
	end

	for _, name in ipairs( t_GameTooltipRelFrame ) do
		local frame = getglobal( name );
		if not frame:IsShown() then
			this:Hide();
			ClearGameTooltipRelFrame();
			return;
		end
	end

	local frame = getglobal( this:GetClientString() );
	if frame == nil then
		return;
	end

	if not frame:IsShown() then
		this:Hide();
		this:SetClientString( "" );
	end
end

-- @param: szRelFrameName	tips所在的顶级面板名称
function ShowGameTooltip( data )
	if not data["frame"] then
		return;
	end

--	print(data["rich"])

	local rich		= getglobal( data["rich"] );
	local tipsFrame = getglobal( data["tipsFrame"] );
	rich:clearHistory();
	if data["tipsFrame"] ~= "WideGameTooltip" then
		rich:resizeRichWidth( 200 * GetScreenScaleY() );
	else
		rich:resizeRichWidth( 300 * GetScreenScaleY() );
	end
	if data["rich"] == "SortInfotipRichText" then
		rich:SetText( data["text"], 255, 255, 255 );
		rich:SetFontType( FONTSYTLE_NORMAL );
	else
		local color = 255 * 65536 + 255 * 256 + 190;
		if data["color"] ~= nil then
			color = data["color"][1] * 65536 + data["color"][2] * 256 + data["color"][3];
			rich:SetText( data["text"], data["color"][1], data["color"][2], data["color"][3] );
		else			
			rich:SetText( data["text"], 255, 255, 190 );
		end
		rich:SetClientUserData(0, color)
		rich:SetText( data["text"], (color / 65536) % 256, (color / 256) % 256, color % 256);
		rich:SetFontType( FONTSYTLE_BORDER );
	end
	rich:SetClientString( data["text"] );
	tipsFrame:SetClientString( data["frame"] );
	tipsFrame:Show();

	local t_relBtns = data["relBtn"];
	if t_relBtns ~= nil then
		SetGameTooltipRelBtn( t_relBtns );
		SetGameTooltipRelFrame( t_relBtns );
	end
	
	if data["button"] == "freedom" then
		util.AdjustTooltipsByCursorWithConflictTest( { tips = data["tipsFrame"], horizonal = data["horizonal"], vertical = data["vertical"], 
										yAdjust = data["yAdjust"], conflictFrame = data["conflictFrame"], 
										["verticalReferencePoint"]	= data["verticalReferencePoint"], 
										["horizonalReferencePoint"] = data["horizonalReferencePoint"], } );
	elseif data["button"] == "cursor" then
		util.AdjustTooltipsScreenPos( { tips = data["tipsFrame"], horizonal = data["horizonal"], vertical = data["vertical"], 
										yAdjust = data["yAdjust"] } );
	else
		util.AdjustBigtipsScreenPos( { tipsFrame = data["tipsFrame"], button = data["button"], horizonal = data["horizonal"], 
										vertical = data["vertical"] } );
	end
end

function GameTooltipOnShow()
	local rich = getglobal( "GameTooltipRichText" );
	local frame = getglobal( "GameTooltip" );

	local szText = GameTooltipRichText:GetClientString();
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	
	--print( "new"..math.ceil( nWidth + 4 * GetScreenScaleY() ).."**"..math.ceil( nHeight + 4 * GetScreenScaleY() ) )

	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ), math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	frame:resizeRect( math.ceil( nWidth + 6 * GetScreenScaleY() ), math.ceil( nHeight + 6 * GetScreenScaleY() ) );
	--rich:resizeRect( math.ceil( nWidth ), math.ceil( nHeight ) );
	--frame:resizeRect( math.ceil( nWidth ), math.ceil( nHeight ) );
	--print( "new"..math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ).."**"..math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) )
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	
end
function GameTooltipAddTextureOnShow()
--	print("GameTooltipAddTextureOnShow")
	local rich = getglobal( "GameTooltipAddTextureRichText" );
	local frame = getglobal( "GameTooltipAddTexture" );

	local szText = rich:GetClientString();
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end
	
	local textEndPosX	=	rich:getLineWidth( rich:GetTextLines() - 1 ) + 4;
	local textEndPosY	=	rich:GetTotalHeight() - 12;
	local moneyTexture	=	getglobal( "GameTooltipAddTextureMoneyTex" );
	moneyTexture:SetPoint( "topleft", "GameTooltipAddTexture", "topleft", textEndPosX, textEndPosY );
	local findFrom	= string.find( szText, "你在这里" )
	local findGray	= string.find( szText,"占领")
	local findVip	= string.find( szText, "特权" )
	if findFrom or findGray or findVip then
		moneyTexture:Hide();
	else
		moneyTexture:Show();
	end	
	

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;

	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ), math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	frame:resizeRect( math.ceil( nWidth + 6 * GetScreenScaleY() ), math.ceil( nHeight + 6 * GetScreenScaleY() ) );
	--rich:resizeRect( math.ceil( nWidth ), math.ceil( nHeight ) );
	--frame:resizeRect( math.ceil( nWidth ), math.ceil( nHeight ) );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	
end

function TempGameTooltip_OnShow()
	local rich = getglobal( "TempGameTooltipRichText" );
	local frame = getglobal( "TempGameTooltip" );

	local szText = GameTooltipRichText:GetClientString();
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;

	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ), math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	frame:resizeRect( math.ceil( nWidth + 20 * GetScreenScaleY() ), math.ceil( nHeight + 20 * GetScreenScaleY() ) );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
end

function WorldMapGameTooltip_OnShow()
	local rich = getglobal( "WorldMapGameTooltipRichText" );
	local frame = getglobal( "WorldMapGameTooltip" );

	local szText = rich:GetClientString();
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;

	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ) , math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	frame:resizeRect( math.ceil( nWidth + 6 * GetScreenScaleY() ), math.ceil( nHeight + 6 * GetScreenScaleY() ) );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	rich:SetSize( math.ceil( nWidth + 4 * GetScreenScaleY() ) / GetScreenScaleY(), math.ceil( nHeight + 4 * GetScreenScaleY() ) / GetScreenScaleY() );
	local color = rich:GetClientUserData(0);
	rich:SetText(szText, (color / 65536) % 256, (color / 256) % 256, color % 256);
	--print( "totalHeight = "..nHeight );
end
function WorldMapGameRegionTooltip_OnShow()
	local rich = getglobal( "WorldMapGameRegionTooltipRichText" );
	local frame = getglobal( "WorldMapGameRegionTooltip" );
	local szText = rich:GetClientString();
	rich:resizeRect( 250, 68 );
	frame:resizeRect( 250,68 );
	frame:SetSize( 250,  68 );
	rich:SetSize( 250, 68 );
	local color = rich:GetClientUserData(0);
	rich:SetText(szText, (color / 65536) % 256, (color / 256) % 256, color % 256);
end

function ClearClanSkillTips()
	for i = 1, MAX_SPELLTIPS_FONT_NUM do
		local font = getglobal("ClanSkillTipsfont"..i);
		font:SetText("");
		font:SetTextColor(255, 255, 255);
		font:Hide();
	end

	for i = 1,MAX_CLANTIP_LINE_NUM do
		local LineTex = getglobal("ClanSkillTipsITItemLineTex"..i);
		LineTex:Hide();
	end
	
	local t_riches = { "ClanSkillTipsRichText", "ClanSkillTipsResearchRichText", "ClanSkillTipsLearnRichText" };	
	for _, name in ipairs( t_riches ) do
		local rich = getglobal( name );
		rich:Hide();
		rich:Clear();
		rich:clearHistory();
	end
end

function ClanSkillTipsFrame_OnShow( )
	ClearClanSkillTips();

	local id = ClanSkillTipsFrame:GetClientUserData( 0 );
	if id <= 0 or id == nil then
		ClanSkillTipsFrame:Hide();
		return;
	end

	local frame = getglobal( "ClanSkillTipsFrame" );
	ClanSkillTipsTexture:SetPoint( "topleft", "ClanSkillTipsFrame", "topleft", 18, 12 );

	local lv = ClanSkillTipsFrame:GetClientUserData( 1 );
	local nFontHeight	= 15;
	local nFrameHeight	= 0;
	local nIndex = 0;
	local nLineIndex = 1;

	local SkillDef = GameClanManager:getClanSkillDef(id, lv);
	if SkillDef.SkillID == 0 then
		ClanSkillTipsFrame:Hide();
		return;
	end

	local needLevel = SkillDef.ResearchNeedClanLevel;
	local needMoney = SkillDef.ResearchNeedClanMoney;
	local skillDesc = SkillDef.SkillDesc;
	local szText;

	local IconPath = GetSkillIconPath()..SkillDef.IconID1..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ClanSkillTipsTexture:SetTexture( IconPath );

	ClanSkillTipsName:SetText(SkillDef.Name);

	local SpellTipsLineTex = getglobal("ClanSkillTipsITItemLineTex"..nLineIndex);
	if nIndex > 2 then
		nFrameHeight = nIndex * (nFontHeight + 2) - 24;
	else
		nFrameHeight = nFontHeight;
	end
	SpellTipsLineTex:SetPoint( "bottomleft", "ClanSkillTipsTexture", "bottomleft", -17, nFrameHeight );
	SpellTipsLineTex:Show();
	nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
	nLineIndex = nLineIndex + 1;

	local ClanViewer = GameClanManager:getClanViewer();
	local ClanInfo = ClanViewer.m_clanInfo;	
	local isRearch,_,valiLv = isInRearchList(id);
	local value = nil;
	local NextLvSkillDef = nil;
	if isRearch then
		valiLv = (valiLv == 0) and 1 or valiLv;
		szText = "#c5dfbf8公会研发\n已研发" .. valiLv .. "级:\n";
		SkillDef = GameClanManager:getClanSkillDef( id, valiLv );
		value = SkillDef.SkillDesc;

		NextLvSkillDef = GameClanManager:getClanSkillDef( id, valiLv + 1 );
		if NextLvSkillDef.SkillID ~= 0 then
			szText = szText .. value .. "#n\n\n研发下一等级：\n"..NextLvSkillDef.SkillDesc.."\n";

			if NextLvSkillDef.ResearchNeedClanLevel > ClanInfo.m_nClanLevel then
				value = "#cff0000需要公会等级：" .. NextLvSkillDef.ResearchNeedClanLevel .. "#n\n";
			else
				value = "需要公会等级："..NextLvSkillDef.ResearchNeedClanLevel .. "\n";
			end
			szText = szText .. value;

			if NextLvSkillDef.ResearchNeedClanMoney > ClanInfo.m_nMoney then
				value = "#cff0000需要公会资金：" .. NextLvSkillDef.ResearchNeedClanMoney .. "#n\n";
			else
				value = "需要公会资金："..NextLvSkillDef.ResearchNeedClanMoney .. "\n";
			end
			szText = szText .. value
		else
			szText = szText .. value .. "#n\n\n#c9bff6e当前已是最高等级#n\n";
		end
	else
		SkillDef = GameClanManager:getClanSkillDef( id, 0 );
		szText = "#c5dfbf8公会研发\n已研发0级："..SkillDef.SkillDesc.."#n\n\n研发下一等级：\n"..skillDesc.."\n";

		if needLevel > ClanInfo.m_nClanLevel then
			value = "#cff0000需要公会等级：" .. needLevel .. "#n\n";
		else
			value = "需要公会等级："..needLevel .. "\n";
		end
		szText = szText .. value;

		if needMoney > ClanInfo.m_nMoney then
			value = "#cff0000需要公会资金：" .. needMoney .. "#n\n";
		else
			value = "需要公会资金："..needMoney .. "\n";
		end
		szText = szText .. value;
	end

	if szText ~= "" then
		--设置窗口位置
		ClanSkillTipsResearchRichText:resizeRichWidth(183 * GetScreenScaleY());
		ClanSkillTipsResearchRichText:resizeRichHeight(100);			
		ClanSkillTipsResearchRichText:SetText( szText,221,196,159 );
		nFrameHeight = nFrameHeight + ClanSkillTipsResearchRichText:GetTotalHeight()/GetScreenScaleY();
		ClanSkillTipsResearchRichText:SetPoint( "topleft", ("ClanSkillTipsITItemLineTex"..(nLineIndex-1)), "bottomleft", 18, SPELL_LINE_DISTANCE );
		ClanSkillTipsResearchRichText:Show();
		nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
	end
	
	SpellTipsLineTex = getglobal("ClanSkillTipsITItemLineTex"..nLineIndex);
	SpellTipsLineTex:SetPoint( "bottomleft", "ClanSkillTipsTexture", "bottomleft", -17, nFrameHeight  );

	local mainplayer = ActorMgr:getMainPlayer();
	local clanMember = GameClanManager:getMemberInfo( mainplayer:getName());
	if SkillDef.AutoLearn == 2 then
		szText = ""
	else
		if isRearch then
			local isLearn, lv = isInLearnList( id );
			if isLearn then
				SkillDef = GameClanManager:getClanSkillDef( id, lv );
				szText = "#c9bff6e会员学习\n已学习" .. lv .. "级：\n" .. SkillDef.SkillDesc .."#n\n\n升级下一等级：";
				value = "#c9bff6e会员学习\n已学习" .. lv .. "级：\n" .. SkillDef.SkillDesc .."#n\n\n#c9bff6e当前已是最高等级#n\n";
				NextLvSkillDef = GameClanManager:getClanSkillDef( id, lv + 1 );
			else
				SkillDef = GameClanManager:getClanSkillDef( id, 0 );
				szText = "#c9bff6e会员学习\n已学习0级："..SkillDef.SkillDesc.."#n\n\n学习下一级：";
				NextLvSkillDef = GameClanManager:getClanSkillDef( id, 1 );
			end
		else
			SkillDef = GameClanManager:getClanSkillDef( id, 0 );
			szText = "#c9bff6e会员学习\n已学习0级："..SkillDef.SkillDesc.."#n\n\n学习下一级：";
			NextLvSkillDef = GameClanManager:getClanSkillDef( id, 1 );
		end

		if NextLvSkillDef.SkillID ~= 0 then
			if NextLvSkillDef.LearnNeedContri > clanMember.Contribute then
				value = "#cff0000消耗公会贡献度：" .. NextLvSkillDef.LearnNeedContri .. "#n\n";
			else
				value = "消耗公会贡献度："..NextLvSkillDef.LearnNeedContri .. "#n\n";
			end
			szText = szText .."\n"..NextLvSkillDef.SkillDesc.."#n\n".. value;
		else
			szText = value;
		end
	end

	if szText ~= "" then
		SpellTipsLineTex:Show();
		nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
		nLineIndex = nLineIndex + 1;
		--设置窗口位置
		ClanSkillTipsLearnRichText:resizeRichWidth(183 * GetScreenScaleY());
		ClanSkillTipsLearnRichText:resizeRichHeight(100);			
		ClanSkillTipsLearnRichText:SetText( szText,221,196,159 );
		nFrameHeight = nFrameHeight + ClanSkillTipsLearnRichText:GetTotalHeight()/GetScreenScaleY();
		ClanSkillTipsLearnRichText:SetPoint( "topleft", ("ClanSkillTipsITItemLineTex"..(nLineIndex-1)), "bottomleft", 18, SPELL_LINE_DISTANCE );
		ClanSkillTipsLearnRichText:Show();
		nFrameHeight = nFrameHeight + SPELL_LINE_DISTANCE;
	else
		SpellTipsLineTex:Hide();
	end

	ClanSkillTipsFrame:SetHeight( nFrameHeight - nFontHeight + 78 );

	 --调整窗口位置
	local szBtnName = ClanSkillTipsFrame:GetClientString();
	if szBtnName ~= nil then
		local nWidth    = ClanSkillTipsFrame:GetWidth();
		local nHeight   = ClanSkillTipsFrame:GetHeight();

		local curSelectBtn = getglobal(szBtnName);
		local nRealTop  = curSelectBtn:GetRealTop();
		local nRight    = curSelectBtn:GetRight();

		if ( nRealTop - nHeight ) > 0 then
			if ( nRight + nWidth < GetScreenWidth() ) then
				ClanSkillTipsFrame:SetPoint("BottomLeft",szBtnName,"TopRight",0,-10);
			else
				ClanSkillTipsFrame:SetPoint("BottomRight",szBtnName,"TopLeft",0,-10);
			end
		else
			if ( nRight + nWidth < GetScreenWidth() ) then
				ClanSkillTipsFrame:SetPoint("TopLeft",szBtnName,"BottomRight",0,10);
			else
				ClanSkillTipsFrame:SetPoint("TopRight",szBtnName,"BottomLeft",0,10);
			end
		end
	end
end

function ClanSkillTipsFrame_OnHide()
	this:SetClientString( "" );	   
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
end

function WideGameTooltip_OnShow()
	local rich = getglobal( "WideGameTooltipRichText" );
	local frame = getglobal( "WideGameTooltip" );
	local szText = rich:GetClientString();
	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end
	nWidth = nWidth + 6;
	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;

	rich:resizeRect( nWidth, nHeight );
	frame:resizeRect( nWidth, nHeight + 4 );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	local color = rich:GetClientUserData(0);
	rich:SetText(szText, (color / 65536) % 256, (color / 256) % 256, color % 256);
end

function checkArmSkill( id )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local equip = mainplayer:getEquip();
	local t_data = {};
	for i=1, MAXEQUIPPOS do
		local bagButton	= getglobal( "EquipFrameEquip"..i);
		local item	= equip:getItem( bagButton:GetClientID() );
		local num = item:getArmSkillNum();
		if num > 0 then
			local itemDef	= item:getItemDef();
			for i=1, num do
				local data = item:getArmSkillOne( i-1 );
				local attrDef = GameArmProce:getArmSkillAttrDef( data.ID );
				if attrDef ~= nil and attrDef.SkillID == id then
					local lvMul = attrDef.LvlMdf[math.floor( itemDef.WearPre.RoleLevelMin / 10 )] / 10000;
					local posMul =  getSkillPosMul( attrDef, itemDef.WearPos1 ) / 10000;
					local impMul = attrDef.ImpMdf[itemDef.Important] / 10000;
					local rndMul = attrDef.RandMdf[data.Idx] / 10000;
					local val = math.floor( attrDef.BaseVal * lvMul * posMul * impMul * rndMul );
					val = attrDef.EffectID > 0 and math.floor( val / 100 ) / 100 or val;
					t_data[attrDef.EffectID] = t_data[attrDef.EffectID] ~= nil and t_data[attrDef.EffectID] + val or val;
				end
			end
		end
	end

	if #t_data > 0 then
		return true, t_data;
	else
		for _,v in pairs( t_data ) do
			return true, t_data;
		end
		return false;
	end
end