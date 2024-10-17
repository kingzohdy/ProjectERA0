local MAX_VIEW_LINE = 16;
-- 在点击学习按钮以后,不能继续学习下一个技能的原因是没有对studyButton设置其对应的数据造成的
-- 用于表示没有选中任何类型的技能
local SKILLLEARN_TYPE_NONE = -1;
-- 用于指示当前被选中的技能
local t_CurSelectSkill = { skillId = 0, skillLv = 0, skillType = SKILLLEARN_TYPE_NONE, nLineNumOnUI = 0 };

--指示全部技能页面和可学习技能页面的标示符
local SKILLPAGE_TOTAL	 = 3;
local SKILLPAGE_CANSTUDY = 4;
local SKILLPAGE_NONSTUDY = 5;

local nCurSkillPage = SKILLPAGE_CANSTUDY;--用于保存当前显示的技能页面是哪个,默认显示全部技能页面

--local t_ParentNode = {  父节点名称, 技能类型, 指示父节点是否展开的状态标志, {子节点集合列表}};
--t_ParentNodeList = { [技能类型1] = {对应父节点ParentNode1}, [技能类型2] = {对应父节点ParentNode2}, ... }
local PARENT_NODE_LENGTH = 4;--此为父节点的长度

--local t_ChildNode  = { 子节点名称,技能ID, 技能要求的最低等级};
--t_ChildNodeList = { [技能类型1] = {技能列表ChildNode1}, [技能类型2] = {技能列表ChildNode2}, ... };
local CHILD_NODE_LENGTH = 3;--此为子节点的长度

--定义各个职业的技能分类
local t_WarriorSkillTypeList = { [SKILLLEARN_TYPE_PUBLIC] = "共有" };
local t_MagicorSkillTypeList = { [SKILLLEARN_TYPE_PUBLIC] = "共有" };
local t_GunerSkillTypeList	 = { [SKILLLEARN_TYPE_PUBLIC] = "共有" };
local t_ChurchSkillTypeList	 = { [SKILLLEARN_TYPE_PUBLIC] = "共有" };

local t_SelectSkillTypeList = {
						[g_nCAREER_NON] = {}, [g_nCAREER_WARRIOR] = t_WarriorSkillTypeList, [g_nCAREER_MAGICOR] = t_MagicorSkillTypeList,
						[g_nCAREER_GUNER] = t_GunerSkillTypeList, [g_nCAREER_CHURCH] = t_ChurchSkillTypeList
							  };

local nNpcId;

--@Desc:获得NPC的职业
--@return: NPC职业nRace
function GetNpcRace()
	local mainplayer = ActorMgr:getMainPlayer();
	return mainplayer:getRace();
end
--@Desc：选择NPC所对应的职业
--@return:返回NPC对应的技能类型列表
function GetNpcSkillTypeList()
	--获得当前游戏中的玩家
	local tagPlayer = ActorMgr:getMainPlayer();
	local nRace		= tagPlayer:getRace();
	return t_SelectSkillTypeList[nRace % 0x10];
end

--定义用于保存可学习技能的列表和全部技能的列表
--t_TotalSkillList = {[技能类型1] = {对应父节点ParentNode1}, [技能类型2] = {对应父节点ParentNode2}, ... }
local t_TotalSkillList = {};

--@Desc:用于在调用GameSkillLearn:updateSkillLearnList()后，与应用程序中的列表同步
--@后置条件:刷新列表后，要保证列表中原来的展开状态不变
--@return: Nothing
function UpdateTotalList()
	--遍历每个父节点，并同步脚本中的父节点与应用程序中的父节点
	local tagParentNodeInfo;
	local tagParentNode;--保存要插入的父节点
	local tagChildNode;--保存插入父节点的子节点
	local t_SkillTypeList = GetNpcSkillTypeList();
	--获得原来列表中各个父节点的展开状态
	local t_TempList = t_TotalSkillList;
	--清空原来的列表内容
	t_TotalSkillList = {};
	local bFound = false;

	for nSkillType, szTypeName in pairs( t_SkillTypeList ) do
		bFound = false;
		tagParentNodeInfo = GameSkillLearn:getAllSkill( nSkillType );
		--先填充父节点的展开状态，若为新的节点则默认为非展开状态
		for i = 1, table.getn( t_TempList ) do
			if nSkillType == t_TempList[i][2] then
				tagParentNode = { szTypeName, nSkillType, t_TempList[i][3], {} };
				bFound = true;--表示要建立的新的父节点在原来的列表中存在
				break;
			end
		end

		--若要建立的父节点是新的节点
		if not bFound then
			tagParentNode = { szTypeName, nSkillType, true, {} };
		end

		--往父节点中加入其对应的子节点
		local nSkillId		= 0;
		local nSkillLv		= 0;
		local tagSkillAttr	= {};
		local szChildName	= "";

		for i = 1, tagParentNodeInfo.m_nNum do
			nSkillId		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nSkillId;
			nSkillLv		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nLv;
			tagSkillAttr	= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			tagChildNode	= { tagSkillAttr.Name, nSkillId, nSkillLv };

			table.insert( tagParentNode[4], tagChildNode );
		end
		--将父节点插入技能列表
		table.insert( t_TotalSkillList, tagParentNode );
		SortTotalSkillList();
	end
end

function SortTotalSkillList()
	local mainplayer= ActorMgr:getMainPlayer();
	for _, raceSkillList in ipairs( t_TotalSkillList ) do
		local nInsertIndex = 1;
		local t_skillList = raceSkillList[4];
		for i = 1, table.getn( t_skillList ) do
			local skill		= t_skillList[i];
			local skillDef	= SkillMgr:getSpellStaticAtt( skill[2], skill[3] );
			if mainplayer:getLv() >= skillDef.Learn.LevelMin then
				local element = table.remove( t_skillList, i );
				table.insert( t_skillList, nInsertIndex, element );
				nInsertIndex = nInsertIndex + 1;
			end
		end
	end
end

local t_CanStudySkillList = {};

--@Desc:用于在调用GameSkillLearn:updateSkillLearnList()后，与应用程序中的列表同步
--@后置条件:刷新列表后，要保证列表中原来的展开状态不变
--@return: Nothing
function UpdateCanStudyList()
	--遍历每个父节点，并同步脚本中的父节点与应用程序中的父节点
	local tagParentNodeInfo;
	local tagParentNode;--保存要插入的父节点
	local tagChildNode;--保存插入父节点的子节点
	local t_SkillTypeList = GetNpcSkillTypeList();
	--获得原来列表中各个父节点的展开状态
	local t_TempList = t_CanStudySkillList;
	--清空原来的列表内容
	t_CanStudySkillList = {};
	local bFound = false;

	for nSkillType, szTypeName in pairs( t_SkillTypeList ) do
		bFound = false;
		tagParentNodeInfo = GameSkillLearn:getCanLearnSkill( nSkillType );
		--先填充父节点的展开状态，若为新的节点则默认为非展开状态
		for i = 1, table.getn( t_TempList ) do
			if nSkillType == t_TempList[i][2] then
				tagParentNode = { szTypeName, nSkillType, t_TempList[i][3], {} };
				bFound = true;--表示要建立的新的父节点在原来的列表中存在
				break;
			end
		end

		--若要建立的父节点是新的节点
		if not bFound then
			tagParentNode = { szTypeName, nSkillType, true, {} };
		end

		--往父节点中加入其对应的子节点
		local nSkillId		= 0;
		local nSkillLv		= 0;
		local tagSkillAttr	= {};
		local szChildName	= "";

		for i = 1, tagParentNodeInfo.m_nNum do
			nSkillId		= GameSkillLearn:getSkillInfo( nSkillType, i-1, false ).m_nSkillId;
			nSkillLv		= GameSkillLearn:getSkillInfo( nSkillType, i-1, false ).m_nLv;

			tagSkillAttr	= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			szChildName		= tagSkillAttr.Name;
			tagChildNode	= { szChildName, nSkillId, nSkillLv };

			table.insert( tagParentNode[4], tagChildNode );
		end
		--将父节点插入技能列表
		table.insert( t_CanStudySkillList, tagParentNode );
	end
end

local t_NonStudySkillList = {};
--@Desc:用于在调用GameSkillLearn:updateSkillLearnList()后，与应用程序中的列表同步
--@后置条件:刷新列表后，要保证列表中原来的展开状态不变
--@return: Nothing
function UpdateNonStudyList()
	--遍历每个父节点，并同步脚本中的父节点与应用程序中的父节点
	local tagParentNodeInfo;
	local tagParentNode;--保存要插入的父节点
	local t_SkillTypeList = GetNpcSkillTypeList();
	--获得原来列表中各个父节点的展开状态
	local t_TempList = t_NonStudySkillList;
	--清空原来的列表内容
	t_NonStudySkillList = {};
	local bFound = false;
	local mainplayer= ActorMgr:getMainPlayer();
	for nSkillType, szTypeName in pairs( t_SkillTypeList ) do
		bFound = false;
		tagParentNodeInfo = GameSkillLearn:getAllSkill( nSkillType );
		--先填充父节点的展开状态，若为新的节点则默认为非展开状态
		for i = 1, table.getn( t_TempList ) do
			if nSkillType == t_TempList[i][2] then
				tagParentNode = { szTypeName, nSkillType, t_TempList[i][3], {} };
				bFound = true;--表示要建立的新的父节点在原来的列表中存在
				break;
			end
		end

		--若要建立的父节点是新的节点
		if not bFound then
			tagParentNode = { szTypeName, nSkillType, true, {} };
		end

		--往父节点中加入其对应的子节点
		local nSkillId		= 0;
		local nSkillLv		= 0;
		local tagSkillAttr	= {};
		for i = 1, tagParentNodeInfo.m_nNum do
			nSkillId		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nSkillId;
			nSkillLv		= GameSkillLearn:getSkillInfo( nSkillType, i-1, true ).m_nLv;
			local	skillDef= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
			if mainplayer:getLv() < skillDef.Learn.LevelMin then
				table.insert( tagParentNode[4], { skillDef.Name, nSkillId, nSkillLv } );
			end
		end
		--将父节点插入技能列表
		table.insert( t_NonStudySkillList, tagParentNode );
	end
end

--@Desc:此为用于获得表的实际有效行数
--return:获得技能列表中可显示节点的最大行数
local function GetListRowNum( t_SkillList  )
	local nRowNum = 0;
	for i = 1, table.getn( t_SkillList ) do
		nRowNum = nRowNum + 1;
		--检测父节点是否可以展开
		if t_SkillList[i][3] then
			local t_ChildInfoList = t_SkillList[i][4];
			--遍历当前父节点的子列表
			for j = 1, table.getn( t_ChildInfoList ) do
				nRowNum = nRowNum + 1;
			end
		end
	end

	return nRowNum;
end

--用于保存模板中开始显示的节点ID
--local nStartDisplayPoint = 0;

--@Desc:用于注册技能学习面板的消息
--@return: Nothing
function NpcLearnSkillFrame_OnLoad()
	this:RegisterEvent("GE_SKILLLEARN_OPEN");
end

--@Desc:用于响应各种事件消息
--@return: Nothing
function NpcLearnSkillFrame_OnEvent()
	--MessageBox( "提示", "NpcLearnSkillFrame_OnEvent" );
	if arg1 == "GE_SKILLLEARN_OPEN" then
		local Frame= getglobal( "NpcLearnSkillFrame" );
		Frame:Show();
	end
end

--@Desc:用于在打开技能面板的时候获取NPC的ID
--@return: Nothing
function openSkillLearn( NpcId )
	nNpcId = NpcId;
	GameSkillLearn:updateSkillLearnList();
	if not NpcLearnSkillFrame:IsShown() then
		NpcLearnSkillFrame:Show();
		util.testNpcDialogDistance(NpcId, NpcLearnSkillFrame)
	end
end

--@Desc:用于向服务器发消息,表示当前学习成功了由nSkillId和nSkillLv标识的技能
--@Param: nSkillId  此为当前学习成功的技能ID
--@Param: nSkillLv  此为当前学习成功的技能Lv
function SkillLearn( nSkillId, nSkillLv )
	if NpcLearnSkillFrame:IsShown() and nSkillLv > 1 then
		if not GameWizard:isDo( GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_SEE_STUDY_SKILL_NPC );
		end

		if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION );
		end	

		if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN ) then
			GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN );
		end	
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--刷新技能学习面板		FixHighLightTex()
	GameSkillLearn:updateSkillLearnList();
	SkillLearnClientData( nSkillId, nSkillLv );
	AcceclKeySkillLearn( nSkillId );	
	if nSkillLv == 1 or SKILL_COMMON[nSkillId] then
		InitSkillData();
	end

	if not NpcLearnSkillFrame:IsShown() then
		return;
	end
	--现在要重新刷新"全部"和"可学习"技能列表中的内容
	UpdateTotalList();
	UpdateCanStudyList();
	UpdateNonStudyList();
	local slider = getglobal( "NLS_ScrollBar" );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
	else
		UpdateSkillPage( 1 );
	end
	ResetHighlightAndSkillInfo();
end

--@Desc:此为用来显示技能的详细信息
--@Param: nSkillId 此为要显示的技能的ID
--@Param: nSkillLv 此为要显示的技能的Lv
--@return: Nothing
function NlsFrame_ShowSkillInfo( nSkillId, nSkillLv )
	local skillDef		= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
	local mainplayer	= ActorMgr:getMainPlayer();
	local nRace			= GetNpcRace();
	local nLevel			= SkillMgr:isSkillLearned(nSkillId) and SkillMgr:getSpellLv(nSkillId) or 0;

	-- 特别处理,以后改用FontString
	local t_riches = { ["NlsTechnologyDescText"] =	{	value = "       " .. getSkillInfo( skillDef, skillDef.SkillDesc ),
														color = { r = 255, g = 255, b = 190 },
													},
					};
	for name, data in pairs( t_riches ) do
		local rich = getglobal( name );
		rich:SetText( data.value, data.color.r, data.color.g, data.color.b );
		rich:Show();
	end
	
	local diathesisDef;
	if skillDef.Learn.PreDiathesisID ~= 0 then
		diathesisDef =  getDiathesisDef( skillDef.Learn.PreDiathesisID, skillDef.Learn.PreDiathesisLevel );
	end

	local t_skillFonts =	{ 
								["NlsFrameTechnologyNameFont"] =
								{
									val = skillDef.Name
								},

								["NlsFrameTechnologyLevelFont"] =
								{
									val = "（等级  " .. nSkillLv .. "）"
								},
								
								["NlsFramePreTianFuValueFont"] = 
								{
									val = ( ( diathesisDef == nil ) and "无" or ( diathesisDef.Name.." "..diathesisDef.Level.."级" ) ),					color = { r = 255 , g = 255, b = 190 }	
								},

								["NlsFramePreTechNameValueFont"] = {
									val =  ( nSkillLv > 1 and ( skillDef.Name.."（等级" .. ( nSkillLv - 1 ) .. "）" ) or "无" ) , 
									color = {  r = 255 , g = 255, b = 190 }
								},
								
								-- 最低金钱数
								["NlsMinMoneyValueFont"] = 
								{
									val = skillDef.Learn.MoneyVal > 0 and ( skillDef.Learn.MoneyVal .. ""..MONEY_TYPE_BULL_NAME.."" ) or "无", color = { r = 255 , g = 255, b = 190 }	
								},
								
								-- 最低人物等级
								["NlsFrameMinLevelValueFont"] = 
								{
									val = skillDef.Learn.LevelMin > 0 and ( skillDef.Learn.LevelMin .. "级" ) or "无", color = { r = 255 , g = 255, b = 190 }	
								},
								
								-- 当前金钱数量
								["NlsCurrentMoneyValueFont"] = 
								{
									val = getPlayerMoney( mainplayer ), color = { r = 209 , g = 185, b = 102 }	
								},
								
								["NLS_SkillDesc_UseSkillPointFont"] = 
								{
									val = skillDef.MPUse > 0 and ( skillDef.MPUse.."点魔法值" ) or "无", color = { r = 255 , g = 255, b = 190 }	
								},
								
								["NLS_SkillDesc_LengQueFont"] = 
								{
									val = skillDef.SelfCool > 0 and ( skillDef.SelfCool / 10 .. "秒") or "无", color = { r = 255 , g = 255, b = 190 }	
								},
								
								["NLS_SkillDesc_ShiFangDistFont"] = 
								{
									val = skillDef.Distance > 0 and ( skillDef.Distance / 100 .. "米") or "无", color = { r = 255 , g = 255, b = 190 }	
								},
								
								["NLS_SkillDesc_YinChangTimeFont"] = 
								{
									val = skillDef.PrepareTime > 0 and ( skillDef.PrepareTime / 10 .. "秒" ) or "无", color = { r = 255 , g = 255, b = 190 }	
								},
							};
	for szFontName, data in pairs( t_skillFonts ) do
		local font = getglobal( szFontName );
		if data.val then
			font:SetText( data.val );
		end
		if data.color then
			font:SetTextColor( data.color.r, data.color.g, data.color.b );
		end
		font:Show();
	end
	
	-- TODO
	local nMoneyVal		= getPlayerMoney( mainplayer, skillDef.Learn.MoneyAttrID )
	local diathesisMgr	= mainplayer:getDiathesisManager();

	local t_failFonts = {
							["NlsFrameMinLevelValueFont"] = { color = ( ( mainplayer:getLv() < skillDef.Learn.LevelMin ) and 
																( { r = 250, g = 60, b = 60 } ) or 
																( { r = 255 , g = 255, b = 190 } ) ) },
							
							["NlsMinMoneyValueFont"] = { color = ( ( nMoneyVal < skillDef.Learn.MoneyVal ) and 
																	( { r = 250, g = 60, b = 60 } ) or 
																	( { r = 255 , g = 255, b = 190 } ) ) },
							
							["NlsFramePreTechNameValueFont"] = { color = ( nLevel < nSkillLv - 1 and 
																	{ r = 250, g = 60, b = 60 } or 
																	{ r = 255 , g = 255, b = 190 } ) },

							["NlsFramePreTianFuValueFont"] = 
							{
								color = ( diathesisDef ~= nil and not diathesisMgr:isLearnedGenius( diathesisDef.DiathesisID, 
											diathesisDef.Level ) ) and ( { r = 250, g = 60, b = 60 } ) or ( { r = 255 , g = 255, b = 190 } )
							},
						};

	
	for name, data in pairs( t_failFonts ) do
		local font = getglobal( name );
		font:SetTextColor( data.color.r, data.color.g, data.color.b );
	end

	local itemButton = getglobal("NpcLearnSkillItemBtn");

	--切换所显示技能图标
	local skillTex	= getglobal("NpcLearnSkillItemBtnIcon");--用于保存技能对应的图标

	local IconPath = GetSkillIconPath()..skillDef.IconID1..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	skillTex:SetTexture( IconPath );

	itemButton:Show();
	NLS_SkillInfoBtn:Show();
end

--@Desc:此为隐藏技能相关详细信息
--@return: Nothing
function HideSkillInfo()
	-- 特别处理,以后改用FontString
	NlsTechnologyDescText:Hide();
	NlsTechnologyDescText:Clear();

	-- 隐藏技能图标
	NpcLearnSkillItemBtn:Hide();
	NLS_SkillInfoBtn:Hide();
end

--@Desc:清除界面上原来的高亮显示
function ClearHighLightTex( nMaxViewLine )
	for i = 1, nMaxViewLine do
		local tex = getglobal("LearnSkillFrameLearnSkillInfo"..i.."LearnSkillRichSelectHighLightTex");
		tex:Hide();
	end
end

--@Desc:固定高亮条的位置
function FixHighLightTex( highLightTex, relativeWidgetName )
	ClearHighLightTex( MAX_VIEW_LINE );

	highLightTex:Show();
end

--@Desc:设置滚动条的位置
function SetLearnSkillScrollBar( t_SkillList )
	--确定技能学习面板显示的具体技能
	local slider		= getglobal( "NLS_ScrollBar" );
	local nTableNum		= GetListRowNum( t_SkillList );--此当前表中可显示的内容的行数
	local nNum			= nTableNum - MAX_VIEW_LINE;
	local nMaxValue		= slider:GetMaxValue( );

	if nNum > 0  then
		slider:SetMaxValue( nNum );
		slider:Show();
	else
		slider:SetMaxValue( 1 );
		slider:Hide();
	end

	slider:SetMinValue( 0 );
	slider:SetValueStep( 1 );

	-- 在学习了技能以后,要特别检测用的,以防止
	if slider:GetValue() > slider:GetMaxValue( ) then
		slider:SetValue( nMaxValue );
	end
end

local bRecond	= false;
local nLastTime = 0;
function NpcLearnSkillFrame_OnUpdate()
	if bRecond then
		nLastTime = os.clock();
		bRecond = false;
	end

	if os.clock() - nLastTime > 0.3 then
		bRecond = true;
	end
end

--t_TotalSkillList = {[技能类型1] = {对应父节点ParentNode1}, [技能类型2] = {对应父节点ParentNode2}, ... }
--local t_ChildNode  = { 子节点名称,技能ID, 技能要求的最低等级};
--local t_ParentNode = {  父节点名称, 技能类型, 指示父节点是否展开的状态标志, {子节点集合列表}};
--@Desc:在技能面板弹出来显示的时候响应
--@return: Nothing
function NpcLearnSkillFrame_OnShow()
	-- 新手指引特效显示
	local MainPlayer = ActorMgr:getMainPlayer();
	local nLevel = MainPlayer:getLv();
	local nRunQuestNum = Quest:getNumQusetRun();
	local bFound = false;
	for i = 1, nRunQuestNum do
		if Quest:getQuestRunId(i-1) == 1058 then
			bFound = true;
		end
	end
	if GameWizard:isDo( GAMEWIZARD_FIRST_LEARNSKILL) == false and  bFound then
		GameWizard:doStep( GAMEWIZARD_FIRST_LEARNSKILL);
		GuideEffectShow( "LearnSkillFrameLearnSkillInfo1", "", "LearnSkillFrameLearnSkillInfo1", "左键点击\n可查看技能信息", "uires\\ui\\mask\\TouMingTongDao.tga", 1, 1, 1, 1, 67, 67, 0, 0, -65, 24, false );
		GuideEffectShow( "NpcLearnSkillFrame_StudyBtn", "","NpcLearnSkillFrame_StudyBtn", "点击按钮\n学习新技能", "uires\\ZhuJieMian\\1.dds", 352, 609, 60, 60, 128, 48, -32, 12, 46, -35, false );
	else
		-- 关闭新手指引特效显示
		LearnSkillFrameLearnSkillInfo1EffectFrame:Hide();
	end

	HideSkillInfo();
	--刚显示界面的时候，清空面板中保存的值，防止按"学习"按钮出现宕机
	local frame	= getglobal( "NpcLearnSkillFrame" );
	t_CurSelectSkill.skillId = 0;
	t_CurSelectSkill.skillLv = 0;

	nCurSkillPage = SKILLPAGE_TOTAL;

	UpdateTotalList();
	UpdateCanStudyList();
	UpdateNonStudyList();
	local t_SkillList = {};

	--检测要显示的页面
	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_SkillList = t_TotalSkillList;
		NlsFrame_OneLayerName:SetText( "全部" );
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then -- SKILLPAGE_CANSTUDY
		t_SkillList = t_CanStudySkillList;
		NlsFrame_OneLayerName:SetText( "可学习" );
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_SkillList = t_NonStudySkillList;
		NlsFrame_OneLayerName:SetText( "不可学习" );
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_SkillList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
	else
		UpdateSkillPage( 1 );
	end
	
	local nCurTableNum = 0;
	for i = 1, table.getn( t_SkillList ) do
		nCurTableNum = nCurTableNum + 1;
		-- 遍历子级
		if t_SkillList[i][3] then
			local t_ChildInfoList = t_SkillList[i][4];
			if table.getn( t_ChildInfoList ) > 0 then
				break;
			end
		end
	end
	
	local rich	= getglobal( "LearnSkillFrameLearnSkillInfo2LearnSkillRich" );
	t_CurSelectSkill.nLineNumOnUI	= 2;
	t_CurSelectSkill.skillId		= rich:GetClientUserData( 0 );
	t_CurSelectSkill.skillLv		= rich:GetClientUserData( 1 );
	NlsFrame_ShowSkillInfo( t_CurSelectSkill.skillId, t_CurSelectSkill.skillLv );
	
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo2LearnSkillRichSelectHighLightTex" );

	--[[
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION ) then
		GameWizard:doStep( GAMEWIZARD_FIRST_BLINK_NPCTALK_STUDY_SKILL_OPTION );
	end	
	--]]
	if LearnSkillFrameLearnSkillInfo2LearnSkillRich:GetText() == "" then     --elmer
		local richTex		= getglobal( "LearnSkillFrameLearnSkillInfo1LearnSkillRich" );
		local highLightRich	= getglobal( "LearnSkillFrameLearnSkillInfo1LearnSkillRichSelectHighLightTex" );
		FixHighLightTex( highLightRich, richTex:GetName() );
		NLS_SkillInfoBtn:Hide();
		NpcLearnSkillFrame:SetSize( 391,493 );
	else
		FixHighLightTex( highLightTex, rich:GetName() );
		this:SetPoint( "center","UIClient","left",420,0 )
	end
end

function NpcLearnSkillFrame_OnHide()
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	NLS_SkillInfoBtn:Hide();
end

--@Desc:点击滚动条的向上箭头时响应
--@return: Nothing
function NLS_ScrollUpBtn_OnClick()
	local slider = getglobal("NLS_ScrollBar");
	if not slider:IsShown() then
		return;
	end

	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	local nValue = slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

--@Desc:点击滚动条的向下箭头时响应
--@return: Nothing
function NLS_ScrollDownBtn_OnClick()
	local slider = getglobal( "NLS_ScrollBar" );
	if not slider:IsShown() then
		return;
	end

	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

--@Desc:此为移动滚动条时，用于刷新文本
--@return: Nothing
function NLS_ScrollBar_OnValueChanged()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	local slider	= getglobal( "NLS_ScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		UpdateSkillPage( slider:GetValue() + 1 );
	end

	local studyButton = getglobal( "NpcLearnSkillFrame_StudyBtn" );
	local nLineNum    = t_CurSelectSkill.nLineNumOnUI;


	if 0 == nLineNum then return end--若没有任何子节点被选中

	local nSkillId	  = t_CurSelectSkill.skillId;
	local nSkillLv	  = t_CurSelectSkill.skillLv;
	local oldHighLightTex =  getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum .. "LearnSkillRichSelectHighLightTex");
	local button;

	oldHighLightTex:Hide();--隐藏原来的高亮条

	for i = 1, MAX_VIEW_LINE do --遍历每一行找到
		button = getglobal( "LearnSkillFrameLearnSkillInfo"..i );

		if nSkillId == button:GetClientUserData( 0 ) and nSkillLv == button:GetClientUserData( 1 ) then
			t_CurSelectSkill.nLineNumOnUI = i;
			local newHighLightTex  =  getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex");
			local szRelativeWidget = "LearnSkillFrameLearnSkillInfo"..i.."LearnSkillRich";
			FixHighLightTex( newHighLightTex, szRelativeWidget );
			return;
		end
	end
end

function Help_LearnSkillFrame()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end
end

function Recommend_LearnSkillFrame()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end
end

--@Desc:此为学习成功技能后重置高亮条和技能详细信息
function ResetHighlightAndSkillInfo()
	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--学习成功后，作一些更新动作

	--设置高亮条

	--获得当前技能表中的内容是否比原来指定行数nLineNum短
	local szRelativeName		= "";
	local szHighLightTexName	= "";
	local nCurTableNum			= 0;
	local nLineNum				= t_CurSelectSkill.nLineNumOnUI;

	for i = 1, table.getn( t_CurDisplayList ) do
		nCurTableNum = nCurTableNum + 1;

		--遍历子级
		if t_CurDisplayList[i][3] then
			local t_ChildInfoList = t_CurDisplayList[i][4];

			for j = 1, table.getn( t_ChildInfoList ) do
				nCurTableNum = nCurTableNum + 1;
			end
		end
	end

	if nCurTableNum < nLineNum then
		nLineNum = nCurTableNum;
	end

	t_CurSelectSkill.nLineNumOnUI = nLineNum;
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRichSelectHighLightTex" );
	richName = "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich";
	FixHighLightTex( highLightTex, richName );

	--设置学习完后能继续学下一个技能
	local nextButton = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum );

	if nextButton:GetClientUserData( 2 ) == PARENT_NODE_LENGTH then -- 若当前定位高亮的节点是父节点
		t_CurSelectSkill.skillId	= 0;
		t_CurSelectSkill.skillLv	= 0;
		t_CurSelectSkill.skillType	= nextButton:GetClientUserData( 0 );

		HideSkillInfo();
	else -- 若当前定位高亮的节点是儿子节点
		t_CurSelectSkill.skillId	= nextButton:GetClientUserData( 0 );
		t_CurSelectSkill.skillLv	= nextButton:GetClientUserData( 1 );
		NlsFrame_ShowSkillInfo( t_CurSelectSkill.skillId, t_CurSelectSkill.skillLv );
	end
end

--@Desc:学习技能
--@return: Nothing
function NpcLearnSkill_Study_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end

	local t_hideIcons = { "NLS_SkillInfoBtnXinShouBtn", "NpcLearnSkillFrame_StudyBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );

	-- 关闭新手指引特效显示
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	--首先检测技能是否能学习
	local frame			= getglobal( "NpcLearnSkillFrame" );
	local nSkillId		= t_CurSelectSkill.skillId;
	local nSkillLv		= t_CurSelectSkill.skillLv;
	local nSkillType	= t_CurSelectSkill.skillType;

	if 0 == nSkillId or 0 == nSkillLv then
		return;
	end

	local nSkillResult = GameSkillLearn:checkSkillLearn( nSkillId, nSkillLv );
	if SKILLLEARN_SUCCED == nSkillResult then
		local skillDef = SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
		if nil == SkillMgr:getSkillDyAttrib( nSkillId  ) then
			--可以学习技能
			if checkNeedWarnCost( skillDef.Learn.MoneyVal ) then
				CostMoneyConfirmFrame:SetClientString("技能学习");
				CostMoneyConfirmFrame:SetClientUserData( 0, nNpcId );
				CostMoneyConfirmFrame:SetClientUserData( 1, nSkillId );
				CostMoneyConfirmFrame:SetClientUserData( 2, nSkillLv );
				SetWarnFrame( "NpcLearnSkillFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
			GameSkillLearn:skillLearn( nNpcId, nSkillId, nSkillLv );
		else
			if checkNeedWarnCost( skillDef.Learn.MoneyVal ) then
				CostMoneyConfirmFrame:SetClientString("技能升级");
				CostMoneyConfirmFrame:SetClientUserData( 0, nNpcId );
				CostMoneyConfirmFrame:SetClientUserData( 1, nSkillId );
				CostMoneyConfirmFrame:SetClientUserData( 2, nSkillLv );
				SetWarnFrame( "NpcLearnSkillFrame" );
				CostMoneyConfirmFrame:Show();
				return;
			end
			GameSkillLearn:skillLvUp( nNpcId, nSkillId, nSkillLv );
		end
	--若不能学习,则给出造成不能学习的中央提示
	else
		if nSkillResult == SKILLLEARN_FAIL_CAREER then
			ShowMidTips( "职业不符合" );
		elseif nSkillResult == SKILLLEARN_FAIL_LVMIN then
			ShowMidTips( "等级不够" );
		elseif nSkillResult == SKILLLEARN_FAIL_NONEED then
			ShowMidTips( "该技能自动掌握，不需要学习" );
		elseif nSkillResult == SKILLLEARN_FAIL_SKILLPOINT then
			ShowMidTips( "技能点不够" );
		elseif nSkillResult == SKILLLEARN_FAIL_MONEY then
			ShowMidTips( "金钱不够" );
		elseif nSkillResult == SKILLLEARN_FAIL_QUEST then
			ShowMidTips( "任务不符合" );
		elseif nSkillResult == SKILLlEARN_FAIL_SKILLLV then
			ShowMidTips( "技能学习等级太高" );
		elseif nSkillResult == SKILLlEARN_FAIL_EXP then
			ShowMidTips( "经验不够" );
		end
	end
end

--@Desc:此为点击父节点的时候，响应相应的收缩动作
--@return: 返回点击父节点前，父节点的展开状态
local function ParentNode_OnClick()
	local nSkillType	= this:GetClientUserData( 0 );
	local frame			= getglobal( "NpcLearnSkillFrame" );
	local bPreUnwrap	= false;--获取点击前父节点的展开状态
	t_CurSelectSkill.skillType	= nSkillType;

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--找出对应的父节点，并改变其状态
	local slider = getglobal( "NLS_ScrollBar" );

	for i = 1, table.getn( t_CurDisplayList ) do
		if nSkillType == t_CurDisplayList[i][2] then
			bPreUnwrap = t_CurDisplayList[i][3];--获取当前父节点是展开还是收缩状态，以用于恢复以前点击的高亮条设置
			t_CurDisplayList[i][3] = not t_CurDisplayList[i][3];
			--更改了当前父节点的展开状态后，要刷新列表
			SetLearnSkillScrollBar( t_CurDisplayList );
			if slider:IsShown() then
				UpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
			else
				UpdateSkillPage( 1 );
			end
			return bPreUnwrap;
		end
	end
end

--@Desc:此为点击子节点的时候响应
--@return: Nothing
local function ChildNode_OnClick()
	local nSkillId	= this:GetClientUserData( 0 );
	local nSkillLv	= this:GetClientUserData( 1 );
	--在学习按钮中把保存当前所点子节点对应控件的位置

	local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
	t_CurSelectSkill.nLineNumOnUI	= this:GetClientID();
	t_CurSelectSkill.skillId		= nSkillId;
	t_CurSelectSkill.skillLv		= nSkillLv;

	local frame		= getglobal( "NpcLearnSkillFrame" );

	--点击子节点的时候，让显示框保存技能的ID和最低等级
	t_CurSelectSkill.skillId	= nSkillId;
	t_CurSelectSkill.skillLv	= nSkillLv;

	NlsFrame_ShowSkillInfo( nSkillId, nSkillLv );

	local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
	studyButton:Show();

	local skillTex		= getglobal("NpcLearnSkillItemBtnIcon");--用于保存技能对应的图标
	local itemButton	= getglobal("NpcLearnSkillItemBtn");
	local skill_attr	= SkillMgr:getSkillDyAttrib( nSkillId );

	if ( skill_attr ~= nill ) then
		local skillDef		= SkillMgr:getSpellStaticAtt( nSkillId, nSkillLv );
		local IconPath = GetSkillIconPath()..skillDef.IconID1..".tga";
		-- 如果没找到物品的图标则使用默认图标显示
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		skillTex:SetTexture( IconPath );
	end

	skillTex:Show();
	itemButton:Show();
end

--@Desc:此为鼠标点击到富文本框时候自动响应
--@return: Nothing
function NpcLearnSkillRich_OnClick()
	-- 关闭新手指引特效显示
	if LearnSkillFrameLearnSkillInfo1EffectFrame:IsShown() then
		LearnSkillFrameLearnSkillInfo1EffectFrame:Hide();
	end

	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	-- 当确认点击的按钮不是父控件且其空间不保存任何技能ID和LV
	if this:GetClientUserData( 2 ) ~= PARENT_NODE_LENGTH and this:GetClientUserData( 0 ) == 0 and
		this:GetClientUserData( 1 ) == 0 then
		return;
	end

	local nNodeLength	= this:GetClientUserData( 2 );
	local bParent		= false;	-- 检查被点击的是否是父节点
	local bPreUnwrap	= true;		-- 在点击前是否为展开状态,默认

	if PARENT_NODE_LENGTH == nNodeLength then
		bPreUnwrap  = ParentNode_OnClick();--获得父节点点击前的展开状态
		bParent		= true;
	else
		ChildNode_OnClick();
	end

	local nLineNum	= this:GetClientID();
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRichSelectHighLightTex" );

	FixHighLightTex( highLightTex, this:GetName() );

	--若点击的父节点，检查是否要恢复以前点击的子节点的高亮条
	if bParent then
		local frame			= getglobal( "NpcLearnSkillFrame" );
		local nSkillId		= t_CurSelectSkill.skillId;
		local nSkillLv		= t_CurSelectSkill.skillLv;

		--若父节点是由收缩状态变为展开状态
		if not bPreUnwrap then

			if 0 ~= nSkillId and 0 ~= nSkillLv then--若原来已经有子节点被点击过

				local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
				local nLineNum		= t_CurSelectSkill.nLineNumOnUI;
				local highLightTex  = getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum .. "LearnSkillRichSelectHighLightTex" );
				local richName		= "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich";

				highLightTex:Show();
				-- 同时隐藏父节点的选中高亮条
				local parentHighlightTex = getglobal( "LearnSkillFrameLearnSkillInfo" .. this:GetClientID() .. "LearnSkillRichSelectHighLightTex" );
				parentHighlightTex:Hide();
			end
		end
	end
end

--@Desc:点击上拉和下拉控件时候响应,切换控件的显示状态
--@return: Nothing
function LearnSkillFrameTitleSignChk_OnClick()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end

	if this:GetClientUserData( 2 ) ~= PARENT_NODE_LENGTH and this:GetClientUserData( 0 ) == 0 and
		this:GetClientUserData( 1 ) == 0 then
		return;
	end

	local nNodeLength	= this:GetClientUserData( 2 );
	local bParent		= false;	-- 检查被点击的是否是父节点
	local bPreUnwrap	= true;		-- 在点击前是否为展开状态,默认

	if PARENT_NODE_LENGTH == nNodeLength then
		bPreUnwrap  = ParentNode_OnClick();--获得父节点点击前的展开状态
		bParent		= true;
	else
		ChildNode_OnClick();
	end

	local parentButton	= getglobal( this:GetParent( ) );
	local nLineNum		= parentButton:GetClientID();
	local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRichSelectHighLightTex" );

	FixHighLightTex( highLightTex, this:GetParent() );

	--若点击的父节点，检查是否要恢复以前点击的子节点的高亮条
	if bParent then
		local frame			= getglobal( "NpcLearnSkillFrame" );
		local nSkillId		= t_CurSelectSkill.skillId;
		local nSkillLv		= t_CurSelectSkill.skillLv;

		--若父节点是由收缩状态变为展开状态
		if not bPreUnwrap then

			if 0 ~= nSkillId and 0 ~= nSkillLv then--若原来已经有子节点被点击过

				local studyButton	= getglobal( "NpcLearnSkillFrame_StudyBtn" );
				local nLineNum		= t_CurSelectSkill.nLineNumOnUI;
				local highLightTex  = getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum .. "LearnSkillRichSelectHighLightTex" );
				local richName		= "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich";

				highLightTex:Show();
				-- 同时隐藏父节点的选中高亮条  Elmer
				local parentHighlightTex = getglobal( "LearnSkillFrameLearnSkillInfo"..parentButton:GetClientID().."LearnSkillRichSelectHighLightTex");
				parentHighlightTex:Hide();
			end
		end
	end
end

--@Desc:此为鼠标进入到富文本框时候自动响应，不是鼠标点击
--@return: Nothing
function NpcLearnSkillRich_OnEnter()
	if this:GetClientUserData( 2 ) ~= PARENT_NODE_LENGTH and this:GetClientUserData( 0 ) == 0 and
		this:GetClientUserData( 1 ) == 0 then
		return;
	end

	--首先获得富文本框的名字
	local tex = getglobal( this:GetName() .. "HighLightTex" );
	tex:Show();
end

--@Desc:此为鼠标离开富文本框的时候自动响应
--@return: Nothing
function NpcLearnSkillRich_OnLeave()
	local tex = getglobal(  this:GetName() .. "HighLightTex" );
	tex:Hide();
end

--@Desc:设置根据nLineNum所指定的子控件中的内容并显示设置后的控件内容
--@Param: [in]nLineNum		指定子节点对应的控件所在的行数
--@Param: [in]tagChildNode	指定要在界面上显示的子节点
--@Param: [in]nCurRowPointX	指定了子节点对应的父节点相对于面板左上角顶点的X偏移
--@Param: [in]nCurRowPointY	指定了子节点对应的父节点相对于面板左上角定点的Y偏移
--@Param: [in]nChildOffSetX	指定了子节点相对于它对应父节点的X偏移
--@return: Nothing
function ShowChildWidgetNode( nLineNum, tagChildNode, nCurRowPointX, nCurRowPointY , nChildOffSetX )
	--根据显示的是全部技能页面还是可学习技能页面,来变化字体的颜色
	local button = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum );
	--button:SetPoint( "topleft", "NpcLearnSkillFrame", "topleft", nCurRowPointX + nChildOffSetX , nCurRowPointY );
	button:Show();
	button:SetClientUserData( 0, tagChildNode[2] ); -- 保存SkillId
	button:SetClientUserData( 1, tagChildNode[3] ); -- 保存SkillLv
	button:SetClientUserData( 2, table.getn( tagChildNode ) ); -- 控件的第3个位置用于保存显示的节点的长度

	local rich		= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich" );
	rich:SetClientID( nLineNum );
	rich:SetClientUserData( 0 , tagChildNode[2] );
	rich:SetClientUserData( 1 , tagChildNode[3] );
	rich:SetClientUserData( 2, table.getn( tagChildNode ) ); -- 控件的第3个位置用于保存显示的节点的长度
	rich:Show();
	rich:SetText( tagChildNode[1] );

	local levelFont	= getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."SkillLevelFont" );
	levelFont:SetText( "（等级  " .. tagChildNode[3] .. "）" );
	--检测当前显示的页面时 "全部" 技能页面还是 "可学习"技能页面
	--若为全部技能页面，则要调整显示的技能信息的颜色
	local mainplayer= ActorMgr:getMainPlayer();
	local skillDef = SkillMgr:getSpellStaticAtt( tagChildNode[2], tagChildNode[3] );
	if SKILLPAGE_TOTAL == nCurSkillPage then
		if mainplayer:getLv() >= skillDef.Learn.LevelMin then
			-- 在全部技能页面中，若技能可以学习，则显示为绿色
			rich:SetTextColor( 255, 255, 190 );
			levelFont:SetTextColor( 255, 255, 190 );
		else -- 若技能不能学习，显示为红色
			rich:SetTextColor( 255, 60, 60 );
			levelFont:SetTextColor( 255, 60, 60 );
		end
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then --SKILLPAGE_CANSTUDY
		rich:SetTextColor( 255, 255, 190 );
		levelFont:SetTextColor( 255, 255, 190 );
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		rich:SetTextColor( 255, 60, 60 );
		levelFont:SetTextColor( 255, 60, 60 );
	end
end

--@Desc:设置根据nLineNum所指定的父控件中的内容并显示设置后的控件内容
--@Param: [in]nLineNum			指定父节点对应的控件所在的行数
--@Param: [in]tagParentNode		指定要在界面上显示的父节点
--@Param: [in]nCurRowPointX		指定了父节点相对于面板左上角顶点的X偏移
--@Param: [in]nCurRowPointY		指定了父节点相对于面板左上角顶点的Y偏移
--@Param: [in]nTitleSignOffsetX	指定了父节点对应的+-控件相对于面板左上角顶点的X偏移
--@return: Nothing
function ShowParentWidgetNode( nLineNum, tagParentNode, nCurRowPointX, nCurRowPointY, nTitleSignOffsetX )
	local button = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum );
	button:Show();
	button:SetClientUserData( 0, tagParentNode[2] ); -- 保存父节点指示的nSkillType
	button:SetClientUserData( 1, 0 );
	button:SetClientUserData( 2, table.getn( tagParentNode ) ); -- 控件的第3个位置用于保存显示的节点的长度

	local checkButton = getglobal( "LearnSkillFrameLearnSkillInfo" .. nLineNum ..  "TitleSignChk" );
	checkButton:setCheckState( tagParentNode[3] );
	checkButton:SetClientUserData( 0, tagParentNode[2] ); -- 保存父节点指示的nSkillType
	checkButton:SetClientUserData( 1, 0 );
	checkButton:SetClientUserData( 2, table.getn( tagParentNode ) ); -- 控件的第3个位置用于保存显示的节点的长度
	checkButton:Show();

	local rich = getglobal( "LearnSkillFrameLearnSkillInfo"..nLineNum.."LearnSkillRich" );
	rich:SetClientID( nLineNum );
	rich:SetText( tagParentNode[1] );
	rich:SetTextColor( 210, 180, 130 );
	rich:Show();
	rich:SetClientUserData( 0, tagParentNode[2] ); -- 保存父节点中技能类型
	rich:SetClientUserData( 1, 0 );
	rich:SetClientUserData( 2, table.getn( tagParentNode ) ); -- 控件的第3个位置用于保存显示的节点的长度
end

--@Desc:用于隐藏所有富文本文字框
--@return: Nothing
local function HideAlSkilllRich()
	local rich;
	local button;
	local checkButton;

	for i = 1, MAX_VIEW_LINE do
		rich = getglobal("LearnSkillFrameLearnSkillInfo"..i.."LearnSkillRich");
		rich:SetClientUserData( 0, 0 );
		rich:SetClientUserData( 1, 0 );
		rich:SetClientUserData( 2, 0 );
		rich:SetText( "" );

		levelRich = getglobal("LearnSkillFrameLearnSkillInfo"..i.."SkillLevelFont");
		levelRich:SetText( "" );

		button = getglobal("LearnSkillFrameLearnSkillInfo"..i);
		button:SetClientUserData( 0, 0 );
		button:SetClientUserData( 1, 0 );
		button:SetClientUserData( 2, 0 );

		checkButton = getglobal( "LearnSkillFrameLearnSkillInfo" .. i ..  "TitleSignChk" );
		checkButton:SetClientUserData( 0, 0 );
		checkButton:SetClientUserData( 1, 0 );
		checkButton:SetClientUserData( 2, 0 );
	end
end

--@Desc:用于隐藏原先所有的+-控件
--@return: Nothing
local function HideAllChk()
	local checkButton;

	for i = 1, MAX_VIEW_LINE do
		local checkButton = getglobal( "LearnSkillFrameLearnSkillInfo" .. i ..  "TitleSignChk" );
		checkButton:Hide();
	end
end

--local t_ChildNode  = { 子节点名称,技能ID, 技能要求的最低等级};
--t_TotalSkillList = {[技能类型1] = {对应父节点ParentNode1}, [技能类型2] = {对应父节点ParentNode2}, ... }
--@Desc: 用于刷新面板中的内容
--@Param: [in]nStartTableNum	用于指定技能列表中起始显示内容在表中所处的位置
--前条件：调用者要确保传递给UpdateSkillPage的起始行数必须是有效的
function UpdateSkillPage( nStartTableNum )
	--刷新页面前，先清除原先的内容
	HideAlSkilllRich();
	HideAllChk();
	for i = 1, MAX_VIEW_LINE do
		local checkButton = getglobal("LearnSkillFrameLearnSkillInfo"..i.."BackgroundTex");
		if (i + nStartTableNum) % 2 == 0 then
			checkButton:Show();
		else
			checkButton:Hide();
		end
	end

	--根据当前显示的是 全部 技能页面还是 可学习 技能页面来刷新页面
	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	--先检测返回是否为子节点
	local nRichClientID		= 0;--用于记录当前显示的行数
	local nCurTableNum		= 0;--用于记录当前遍历到的表的行数
	local nCurRowPointX		= 50;
	local nCurRowPointY		= 65;
	local nChildOffSetX		= 5;
	local nRowHeight		= 12;
	local nTitleSignOffsetX = 30;

	for i = 1, table.getn( t_CurDisplayList ) do
		nCurTableNum = nCurTableNum + 1;
		-- 检测当前表中的行数是否大于起始行数
		-- 显示父级
		if nCurTableNum >= nStartTableNum then
			nRichClientID = nRichClientID + 1;

			--检测当前显示的内容是否超出可显示的区域
			if nRichClientID > MAX_VIEW_LINE then return end
			ShowParentWidgetNode( nRichClientID, t_CurDisplayList[i], nCurRowPointX, nCurRowPointY, nTitleSignOffsetX );
			nCurRowPointY = nCurRowPointY + nRowHeight;
		end

		--显示子级
		if t_CurDisplayList[i][3] then--若父级是展开状态
			local t_ChildInfoList = t_CurDisplayList[i][4];

			for j = 1, table.getn( t_ChildInfoList ) do
				nCurTableNum = nCurTableNum + 1;
				if nCurTableNum >= nStartTableNum then
					nRichClientID = nRichClientID + 1;

					if nRichClientID > MAX_VIEW_LINE then return end--检测当前要显示的内容是否超出可显示区域的最大范围
					ShowChildWidgetNode( nRichClientID, t_ChildInfoList[j], nCurRowPointX, nCurRowPointY, nChildOffSetX  );
					nCurRowPointY = nCurRowPointY + nRowHeight;
				end
			end
		end
	end
end

--@Desc:此为鼠标接近技能图标的时候响应
function NpcLearnSkillItemBtn_OnEnter()
	local skillId   = t_CurSelectSkill.skillId;
	local skillLv   = t_CurSelectSkill.skillLv;
	local szName    = this:GetName();

	if skillId <= 0 or skillId == nil or szName == nil or szName == "" then return end

	SpellTipsFrame:SetClientUserData(0, SkillMgr:getRuneSkillId(skillId));
	SpellTipsFrame:SetClientUserData(1, skillLv);
	SpellTipsFrame:SetClientUserData(3, TRUMP_SKILL_TYPE);
	SpellTipsFrame:SetClientString(szName);
	SpellTipsFrame:Show();
end

function NpcLearnSkillItemBtn_OnLeave()
	SpellTipsFrame:Hide();
end

function NpcLearnSkillItemBt_OnClick()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	end
end

--@Desc:此为点击 全部 和 可学习选项的下拉箭头时候响应
--@return: Nothing
function NlsFrame_DownArrowBtn_OnClick()
	if NlsFrame_SkillOptBtn:IsShown() then
		NlsFrame_SkillOptBtn:Hide();
	else
		NlsFrame_SkillOptBtn:Show();
	end
end

--@Desc:此为在点击 全部 和 可学习 第一行选项时候响应
--@return: Nothing
function NlsFrame_FirstLayerBtn_OnClick()
	NlsFrame_SkillOptBtn:Hide();
	local firstFont = getglobal( "NlsFrame_FirstLayerName" );
	local szFirstLineText = firstFont:GetText();
	local nPreSkillPage = nCurSkillPage;

	if "全部" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_TOTAL;
	elseif "可学习" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_CANSTUDY;
	elseif "不可学习" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_NONSTUDY;
	end

	if nPreSkillPage == nCurSkillPage then
		return;
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
	else
		UpdateSkillPage( 1 );
	end

	NlsFrame_OneLayerName:SetText( szFirstLineText );

	-- 隐藏所选中的所有高亮条
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	HideSkillInfo();

	--清空学习面板中的值防止按 学习 按钮出现非预期行为
	local frame	= getglobal( "NpcLearnSkillFrame" );

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

--@Desc:此为在点击 全部 和 可学习 第二行选项时候响应
--@return: Nothing
function NlsFrame_SecondLayerBtn_OnClick()
	NlsFrame_SkillOptBtn:Hide();
	local secondFont = getglobal( "NlsFrame_SecondLayerName" );
	local szFirstLineText = secondFont:GetText();
	local nPreSkillPage = nCurSkillPage;

	if "全部" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_TOTAL;
	elseif "可学习" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_CANSTUDY;
	elseif "不可学习" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_NONSTUDY;
	end

	if nPreSkillPage == nCurSkillPage then
		return;
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
	else
		UpdateSkillPage( 1 );
	end

	NlsFrame_OneLayerName:SetText( szFirstLineText );

	-- 隐藏所选中的所有高亮条
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	HideSkillInfo();

	--清空学习面板中的值，防止按 学习 按钮出现非预期行为
	local frame	= getglobal( "NpcLearnSkillFrame" );

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

function NlsFrame_FirstLayerBtn_OnEnter()
	--首先获得富文本框的名字
	local szName = this:GetName();
	LearnSkillFrameSkillHighLightTex:SetPoint( "center", szName, "center", 1, 0 );
	LearnSkillFrameSkillHighLightTex:Show();
end

function NlsFrame_FirstLayerBtn_OnLeave()
	LearnSkillFrameSkillHighLightTex:Hide();
end

function NlsFrame_SecondLayerBtn_OnEnter()
	--首先获得富文本框的名字
	local szName = this:GetName();
	LearnSkillFrameSkillHighLightTex:SetPoint( "center", szName, "center", 1, 0 );
	LearnSkillFrameSkillHighLightTex:Show();
end

function NlsFrame_SecondLayerBtn_OnLeave()
	LearnSkillFrameSkillHighLightTex:Hide();
end

--@Desc:此为在点击 全部 和 可学习 第二行选项时候响应
--@return: Nothing
function NlsFrame_ThirdLayerBtn_OnClick()
	NlsFrame_SkillOptBtn:Hide();
	local thirdFont = getglobal( "NlsFrame_ThirdLayerName" );
	local szFirstLineText = thirdFont:GetText();
	local nPreSkillPage = nCurSkillPage;

	if "全部" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_TOTAL;
	elseif "可学习" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_CANSTUDY;
	elseif "不可学习" == szFirstLineText then
		nCurSkillPage = SKILLPAGE_NONSTUDY;
	end

	if nPreSkillPage == nCurSkillPage then
		return;
	end

	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider		= getglobal( "NLS_ScrollBar" );
	local itemButton	= getglobal( "NpcLearnSkillItemBtn" );
	itemButton:Hide();
	slider:SetValue( 0 );
	SetLearnSkillScrollBar( t_CurDisplayList );
	if slider:IsShown() then
		UpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
	else
		UpdateSkillPage( 1 ); -- 若滚动条不显示则从第一行显示技能
	end

	NlsFrame_OneLayerName:SetText( szFirstLineText );

	-- 隐藏所选中的所有高亮条
	for i = 1, MAX_VIEW_LINE do
		local highLightTex	= getglobal( "LearnSkillFrameLearnSkillInfo" .. i .. "LearnSkillRichSelectHighLightTex" );
		highLightTex:Hide();
	end
	HideSkillInfo();

	--清空学习面板中的值，防止按 学习 按钮出现非预期行为
	local frame	= getglobal( "NpcLearnSkillFrame" );

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

function NlsFrame_ThirdLayerBtn_OnEnter()
	--首先获得富文本框的名字
	local szName = this:GetName();
	LearnSkillFrameSkillHighLightTex:SetPoint( "center", szName, "center", 1, 0 );
	LearnSkillFrameSkillHighLightTex:Show();
end

function NlsFrame_ThirdLayerBtn_OnLeave()
	LearnSkillFrameSkillHighLightTex:Hide();
end

function ShowNpcLearnSkillFrameXinShou()
	local xinShouBtn	= getglobal( "NLS_SkillInfoBtnXinShouBtn" );
	local tex			= getglobal( "NpcLearnSkillFrame_StudyBtnUVAnimationTex" );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	tex:SetUVAnimation( 50, true);
	local szText = "点击学习技能";
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:Show();
end

function NLS_SkillInfoBtn_OnShow()
	--print( "NLS_SkillInfoBtn_OnShow--1660" );
	if not GameWizard:isDo( GAMEWIZARD_FIRST_BLINK_NPCLEARNSKILL_STUDY_BTN ) then
		ShowNpcLearnSkillFrameXinShou();
		--print( "NLS_SkillInfoBtn_OnShow--1663" );
	end
	local TipTxt = getglobal("NlsFrameMoneyTipFont");
	TipTxt:SetText("提示：学习技能优先消耗绑银，绑银不足将消耗银币");
	NpcLearnSkillFrame:SetSize( 782,493 )
end

function NLS_SkillInfoBtn_OnHide()
	local t_hideIcons = { "NLS_SkillInfoBtnXinShouBtn", "NpcLearnSkillFrame_StudyBtnUVAnimationTex" };
	util.HideIcons( t_hideIcons );
	NpcLearnSkillFrame:SetSize( 391,493 )
end