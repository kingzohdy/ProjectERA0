local t_mainAttrConfig = 
{
-- /* 守护者 */	体质
[CLIENT_CAREER_WARRIOR_SINGLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn3", attrID = ATTR_ID_TILI, ["careerDesc"] = "守护者", },

-- /* 征服者 */	力量
[CLIENT_CAREER_WARRIOR_DOUBLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn1", attrID = ATTR_ID_LILIANG, ["careerDesc"] = "征服者", },

-- /* 聚能者 */	感知
[CLIENT_CAREER_MAGICOR_SINGLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn6", attrID = ATTR_ID_MINJIE, ["careerDesc"] = "聚能者", },

-- /* 炼魂师 */	智力
[CLIENT_CAREER_MAGICOR_DOUBLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn2", attrID = ATTR_ID_ZHILI, ["careerDesc"] = "炼魂师", },

-- /* 猎杀者 */	力量
[CLIENT_CAREER_GUNER_SINGLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn1", attrID = ATTR_ID_LILIANG, ["careerDesc"] = "猎杀者", },

-- /* 毁灭者 */	敏捷
[CLIENT_CAREER_GUNER_DOUBLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn5", attrID = ATTR_ID_FANGYU, ["careerDesc"] = "毁灭者", },

-- /* 圣佑者 */	精神
[CLIENT_CAREER_CHURCH_SINGLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn4", attrID = ATTR_ID_JINGSHEN, ["careerDesc"] = "圣佑者", },

-- /* 驭雷师 */	智力
[CLIENT_CAREER_CHURCH_DOUBLEHAND]	= { ["btn"] = "SparFrameRoleAttrBtn_BaseAttrBtn2", attrID = ATTR_ID_ZHILI, ["careerDesc"] = "驭雷师", },
};

local nLastShowSparNum = 0;

function InitSparFrameData()
	nLastShowSparNum = 0
end

function Save2SparFrame( item, nSrcList, nSrcGridIndex )
	local btn = nil
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
		local item = container:getItem( CONTAINER_TYPE_SPAR, i-1 )
		if item:getItemId() == 0 then
			btn = "SparFrameSparItemBtn" .. i
			break;
		end
	end
	if btn == nil and container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) > 0 then
		btn = "SparFrameSparItemBtn1"
	end
	if btn ~= nil then
		SparFrameProcessOnReciveDrag(  btn, item:getListType(), item:getGridIdx(), 0, 0 )
	else
		ShowMidTips( "没有空闲的晶石孔" )
	end
end

function SparFrameProcessOnReciveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if dragfrom == CONTAINER_TYPE_SPAR then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragfrom ~= CONTAINER_TYPE_SUNDRIES then
		--ShowMidTips("只有背包内装备才可进行此操作");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local srcList		= dragfrom;						-- 源容器类型
	local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
	local item			= container:getItem( srcList, srcGrid );
	local itemdef = item:getItemDef();
	local btn = getglobal( szBtnName )
	if not CheckItemResult( itemdef,RESULT_SPAR ) then
		ShowMidTips( "请放入晶石" )
		return
	end
	if mainplayer:isInFight() then
		ShowMidTips("战斗中无法进行该操作")
		return
	end
	local lockTex = getglobal( btn:GetName() .. "LockTexture" )
	if not lockTex:IsShown() then
		GameArmProce:requestSparInlay( srcList, srcGrid, CONTAINER_TYPE_SPAR, btn:GetClientID() - 1 );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

tSparModelTex = {
							[CLIENT_GENDER_MALE] = { "uires\\TuPianLei\\NanMo 110X200.tga", 110, 200, 86, 131 },
							[CLIENT_GENDER_FEMALE] = { "uires\\TuPianLei\\NvMo 86X200.tga", 86, 200, 98, 133 }
						}
local hideTips = false;
local tipsIndex = 1;

function UpdateSparFrameInfo()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local modelTex = tSparModelTex[mainplayer:getGender()];
	SparFramePlayerTex:SetTexture( modelTex[1] )
	SparFramePlayerTex:SetSize( modelTex[2], modelTex[3] )
	SparFramePlayerTex:SetPoint("topleft", "SparFrame", "topleft", modelTex[4], modelTex[5] )
	SparFrameNameFont:SetText( mainplayer:getName() )
	SparFrameLvFont:SetText( "等级："..mainplayer:getLv().."级" )
	SparFrameRaceFont:SetText( "职业："..GetCareerName( mainplayer:getRace() ) )
	local nIndex = 1
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
		local item = container:getItem( CONTAINER_TYPE_SPAR, i-1 )
		local btn = getglobal( "SparFrameSparItemBtn" .. i )
		local tex = getglobal( "SparFrameSparTex" .. i )
		local iconTexture = getglobal( btn:GetName() .. "IconTexture" )
		local lockTexture  = getglobal( btn:GetName() .. "LockTexture" )
		local bindTexture = getglobal( btn:GetName() .. "BindTexture" )
		local openFont = getglobal( btn:GetName() .. "OpenFont" )
		local UVAnimationTex = getglobal( btn:GetName() .. "UVAnimationTex" )
		if item:getItemId() == 0 then
			iconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
			bindTexture:Hide()
			lockTexture:Hide()
			tex:Hide();
		else
			-- 显示
			local itemDef = item:getItemDef()
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end	
			iconTexture:SetTexture( IconPath )
			if item:isItemBind() then
				bindTexture:Show()
			else
				bindTexture:Hide()
			end
			lockTexture:Hide()
			if IsContainerItemLock( CONTAINER_TYPE_SPAR, i-1 ) then
				iconTexture:SetGray( true )
			else
				iconTexture:SetGray( false )
			end
			tex:Show()
		end
		UVAnimationTex:Hide()
		openFont:SetText( "" )		
		nIndex = nIndex + 1
	end
	for i=nIndex,MAX_SPAR_PKG_TOLUA do
		local btn = getglobal( "SparFrameSparItemBtn" .. i )
		local tex = getglobal( "SparFrameSparTex" .. i )
		local iconTexture = getglobal( btn:GetName() .. "IconTexture" )
		local lockTexture  = getglobal( btn:GetName() .. "LockTexture" )
		local bindTexture = getglobal( btn:GetName() .. "BindTexture" )
		local openFont = getglobal( btn:GetName() .. "OpenFont" )
		local UVAnimationTex = getglobal( btn:GetName() .. "UVAnimationTex" )
		lockTexture:Show()
		local openLv = GameArmProce:getSparOpenLv( i-1 )
		openFont:SetText( openLv .. "级开启" )
		tex:Hide()
		iconTexture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		bindTexture:Hide()
		UVAnimationTex:Hide()
	end
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SPARFRAME_SHOW) then
		ShowXinShouTips();
	end
	UpdateSparFrameRoleAttrBtn();
end

function SparFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_UPDATE")
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG")
	this:RegisterEvent("GE_CONTAINER_ALL_UPDATE")
	local szText = [[●将晶石镶嵌至晶石孔中可为角色增加属性
●已镶嵌的晶石可#c0ad700shift+右键#cffffbe点击打开吸收界面
●拖拽已镶嵌晶石进背包可取下晶石
]]
	SparFrameDescRich:SetText( szText, 255, 255, 190 )

	ItemFirstCallBack( "SparFrame" , Save2SparFrame );
end

function SparFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" or arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if SparFrame:IsShown() then
			UpdateSparFrameInfo()
		end
	end
	if arg1 == "GE_CONTAINER_ALL_UPDATE" then
		local mainplayer = ActorMgr:getMainPlayer()
		local container = mainplayer:getContainer()
		nLastShowSparNum = container:getContainerMaxNum( CONTAINER_TYPE_SPAR )
	end
end

function SparFrame_OnShow()

	local t_frame = {"EquipFrame", "EquipDesignationFrame", "RespactFrame"};
	local bShow = false;
	for _, szFrame in ipairs( t_frame ) do
		local frame = getglobal( szFrame );
		if frame:IsShown() then
			bShow = true;
			frame:Hide();
			break;
		end
	end

	if not bShow then
		-- 重置窗口坐标	
		SparFrame:SetPoint("topleft", "$parent", "topleft", 32, 163 );
	end
	UpdateSparFrameInfo();
	SparFrameRoleAttrBtn:Show();	
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SPARFRAME_SHOW) then
		hideTips = false;
		ShowXinShouTips();
	end
	SparFrameCheckNewGrid();
end

function SparFrameCheckNewGrid()
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer()
	local sparNum = container:getContainerMaxNum( CONTAINER_TYPE_SPAR )
	if nLastShowSparNum < sparNum and container:getItem( CONTAINER_TYPE_SPAR, sparNum - 1 ):getItemId() == 0 then
		local UVAnimationTex = getglobal( "SparFrameSparItemBtn" .. sparNum .. "UVAnimationTex" )
		UVAnimationTex:SetUVAnimation( 50, true )
	end
	nLastShowSparNum = sparNum
end

function ShowXinShouTips()
	if hideTips then return end;
	local nIndex = nil;
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
		local item = container:getItem( CONTAINER_TYPE_SPAR, i-1 )
		if item:getItemId() ~= 0 then
			nIndex = i
			break;
		end
	end
	if not nIndex then 
		for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
			local xinShouBtn = getglobal( "SparFrameSparItemBtn" .. i .. "XinShouBtn" );
			xinShouBtn:Hide();
		end
		return;
	end
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
		local xinShouBtn = getglobal( "SparFrameSparItemBtn" .. i .. "XinShouBtn" );
		if xinShouBtn:IsShown() then
			local item = container:getItem( CONTAINER_TYPE_SPAR, i-1 )
			if nIndex <= i and item:getItemId() ~= 0 then
				nIndex = i;
			end;
		end
		xinShouBtn:Hide();
	end

	local szText = "你镶嵌了一颗晶石，可shift+右键打开吸收界面，晶石吸收经验会提升等级";
	local xinShouBtn = getglobal( "SparFrameSparItemBtn" .. nIndex .. "XinShouBtn" );
	local rich	 = getglobal( xinShouBtn:GetName() .."Rich" );
	
	local nSelfDefineExtendWidth = 14;
	rich:SetClientString( szText );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	tipsIndex = nIndex;
	xinShouBtn:Show();
end

function SparFrame_OnHide()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
		local xinShouBtn = getglobal( "SparFrameSparItemBtn" .. i .. "XinShouBtn" );
		xinShouBtn:Hide();
	end
end

function SparFrameItemBtn_OnClick()
	if arg1 == "RightButton" then
		local mainplayer = ActorMgr:getMainPlayer()
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local item = container:getItem( CONTAINER_TYPE_SPAR, this:GetClientID() - 1 );
		if item:getItemId() > 0 then
			if ( isKeyPressed(160) or isKeyPressed(161) ) then
				if SparSorbFrame:IsShown() then
					SparSorbFrame:Hide();
				end
				SparSorbFrameItemBtn:SetClientUserData( 0, item:getListType() )
				SparSorbFrameItemBtn:SetClientUserData( 1, item:getGridIdx() )
				LockItem( item:getListType(), item:getGridIdx() )
				UpdateSparFrameInfo();
				SparSorbFrame:Show();
			else
				if mainplayer:isInFight() then
					ShowMidTips("战斗中无法进行该操作")
					return
				end
				if IsContainerItemLock( CONTAINER_TYPE_SPAR, this:GetClientID() - 1 ) then
					return;
				end
				local idx = container:getNoneGridIndex( CONTAINER_TYPE_SUNDRIES )
				if idx >= 0 then
					GameArmProce:requestSparInlay( CONTAINER_TYPE_SUNDRIES, idx, CONTAINER_TYPE_SPAR, this:GetClientID() - 1 );
				end
			end
		end
	end
end

function SparFrameItemBtn_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local item = container:getItem( CONTAINER_TYPE_SPAR, this:GetClientID() - 1 )
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	else
		local lockTex = getglobal( this:GetName() .. "LockTexture" )
		if lockTex ~= nil and not lockTex:IsShown() then
			SetGameTooltips(this:GetName(),"晶石孔：可镶嵌晶石\n分解装备有几率得到晶石")
		end
	end
end

function SparFrameItemBtn_OnBeginDrag()
	if IsContainerItemLock( CONTAINER_TYPE_SPAR, this:GetClientID() - 1 ) then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( CONTAINER_TYPE_SPAR,this:GetClientID() - 1 );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = CONTAINER_TYPE_SPAR;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(), 0 );
	end
end

function SparFrameItemBtn_ReceiveDrag()
	if IsContainerItemLock( CONTAINER_TYPE_SPAR, this:GetClientID() - 1 ) then
		return;
	end
	SparFrameProcessOnReciveDrag( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() )
end

function HideSparXinShouBtn()
	local tipsBtn = getglobal( "SparFrameSparItemBtn" .. tipsIndex .. "XinShouBtn" );
	tipsBtn:Hide();
	hideTips = true;
end

function SparFrameRoleAttrBtn_OnShow()
	SparFrameRoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	SparFrameRoleAttrBtn_SpecialAttrLabelBtn:DisChecked();
	UpdateSparFrameRoleAttrBtn();
end

-- 高级属性标签
function SparFrameRoleAttrBtn_AdvanceAttrLabelBtn_OnClick()
	SparFrameRoleAttrBtn_AdvanceAttrLabelBtn:Checked();
	SparFrameRoleAttrBtn_SpecialAttrLabelBtn:DisChecked();

	UpdateSparFrameNormalAttackAttr();
	UpdateSparFrameNormalDefendAttr();

	SparFramePrivate_ShowEquipAdvanceAttr();

	SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont1:SetText("攻");
	SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont2:SetText("击");
	SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont3:SetText("属");
	SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont4:SetText("性");
	SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButtonFont5:SetText("");
	SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont1:SetText("防");
	SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont2:SetText("御");
	SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont3:SetText("属");
	SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont4:SetText("性");
	SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButtonFont5:SetText("");
end

-- 特殊属性标签
function SparFrameRoleAttrBtn_SpecialAttrLabelBtn_OnClick()
	SparFrameRoleAttrBtn_SpecialAttrLabelBtn:Checked();
	SparFrameRoleAttrBtn_AdvanceAttrLabelBtn:DisChecked();	
	SparFramePrivate_ShowEquipYuanSuAttr();
	UpdateSparFrameSpecialAttackAttr();
	UpdateSparFrameSpecialDefendAttr();
	UpdateSparFrameFuZhuAttr();
end

function UpdateSparFrameRoleAttrBtn()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return;
	end

	local nCurExp		= mainplayer:getExp();
	local nLevelNextExp = mainplayer:getLvExp( mainplayer:getLv() );
	
	-- 更新HP和MP条
	local MAX_HP_LENGTH = 152;
	--显示面板标题
	-- 设置HP
	local maxHp		= mainplayer:getMaxHP();
	local hp		= mainplayer:getHP();
	hp				= math.max( math.min( hp, maxHp ), 0 );
	local fHpScale	= math.max( math.min( hp / maxHp, 1 ), 0 );
	local hpBtn		= getglobal( "SparFrameRoleAttrBtnFirstBaseLifeAttrButton" );
	local hpTex		= getglobal(  hpBtn:GetName().."Tex" );
	hpTex:SetSize(  fHpScale * MAX_HP_LENGTH, 11 );
	
	-- 设置MP
	local maxMp		= mainplayer:getMaxMP();
	local mp		= mainplayer:getMP();
	mp				= math.max( math.min( mp, maxMp ), 0 );
	local fMpScale	= math.max( math.min( mp / maxMp, 1 ), 0 );
	local mpBtn		= getglobal( "SparFrameRoleAttrBtnFirstBaseMagicAttrButton" );
	local mpTex		= getglobal(  mpBtn:GetName().."Tex" );
	mpTex:SetSize( fMpScale * MAX_HP_LENGTH, 11 );

	local t_fonts = {	[hpBtn:GetName().."PercentFont"] = { value = hp.."/"..maxHp }, 
						[mpBtn:GetName().."PercentFont"] = { value = mp.."/"..maxMp },
					};
	util.SetFonts( t_fonts );

	UpdateSparFrameBaseAttr();
	-- 若选择的高级属性	IsEnable
	if SparFrameRoleAttrBtn_AdvanceAttrLabelBtn:IsChecked() then
		UpdateSparFrameNormalAttackAttr();
		UpdateSparFrameNormalDefendAttr();
		SparFramePrivate_ShowEquipAdvanceAttr();
	else
		UpdateSparFrameSpecialAttackAttr();
		UpdateSparFrameSpecialDefendAttr();
		UpdateSparFrameFuZhuAttr();
		SparFramePrivate_ShowEquipYuanSuAttr();
	end

	UpdateSparFrameSuitAttr();
end

function SparFramePrivate_ShowEquipYuanSuAttr()
	ClearSparFrameNormalAttackAttr();
	ClearSparFrameNormalDefendAttr();

	local t_showIcons = { "SparFrameRoleAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "SparFrameRoleAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"SparFrameRoleAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );

	local t_hideIcons = { "SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButton", "SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );
end

-- 更新特殊攻击属性
function ClearSparFrameYuanSuAttackAttr()
	for i = 1, MAX_YUAN_SU_ATTACK_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSparFrameSpecialAttackAttr()
	ClearSparFrameYuanSuAttackAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["元素攻击力"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "SparFrameRoleAttrBtn_YuanSuAttackAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新特殊防御属性
function ClearSparFrameYuanSuDefendAttr()
	for i = 1, MAX_YUAN_SU_DEFEND_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSparFrameSpecialDefendAttr()
	ClearSparFrameYuanSuDefendAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["元素防御力"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "SparFrameRoleAttrBtn_YuanSuDefendAttrBtn"..i );
		btn:SetClientString( oneElem["desc"] );
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

function ClearSparFrameFuZhuAttr()
	for i = 1, MAX_FU_ZHU_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_FuZhuAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSparFrameFuZhuAttr()
	ClearSparFrameFuZhuAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["辅助属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn = getglobal( "SparFrameRoleAttrBtn_FuZhuAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新套装属性
function ClearSparFrameSuitAttr()
	for i = 1, MAX_SUIT_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_SuitAttrBtn"..i );
		btn:Hide();
	end
	
	local t_hideIcons = { "SparFrameRoleAttrBtnSuitBackGroundButton", };
	util.HideIcons( t_hideIcons );
end

function UpdateSparFrameSuitAttr()
	ClearSparFrameSuitAttr();
	
	local t_SuitList	= {};
	local mainplayer	= ActorMgr:getMainPlayer();
	local Eqiup			= mainplayer:getEquip();

	local isHaveSuitFunc = 
	function ( itemDef )
		for _, data in ipairs( t_SuitList ) do
			if itemDef.SuitSetID == data["suitID"] then
				return true;
			end
		end

		return false;
	end

	for i = 1, MAXEQUIPPOS do
		local bagButton	= getglobal( "EquipFrameEquip"..i);
		local Item		= Eqiup:getItem( bagButton:GetClientID() );
		local itemDef	= Item:getItemDef();
		if itemDef ~= nil and itemDef.SuitSetID > 0 and not Item:isValidEnd() then
			if not isHaveSuitFunc( itemDef ) then
				local SuitSet		= getSuitSet( itemDef.SuitSetID );
				local t_SuitColor	= GetItemNameColor(itemDef);

				local t_oneSuitElem = 
				{	name	= SuitSet.Name,
					value	= Eqiup:getPlayerSuitEquipNum( itemDef.SuitSetID ) .. "/"..getSuitEquipNum( itemDef.SuitSetID ),
					color	= { r = t_SuitColor[1], g = t_SuitColor[2], b = t_SuitColor[3] },
					suitID	= itemDef.SuitSetID,
				};
				table.insert( t_SuitList, t_oneSuitElem );
			end
		end
	end
	
	-- 先设置装备按钮数据
	for i = 1, table.getn( t_SuitList ) do
		local oneSuitData	= t_SuitList[i];
		local btn			= getglobal( "SparFrameRoleAttrBtn_SuitAttrBtn"..i );
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneSuitData["name"],	color = oneSuitData["color"],  }, 
							[btn:GetName().."ValueFont"]	= {	value = oneSuitData["value"],	color = oneSuitData["color"], },
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local ONE_SUIT_BTN_HEIGHT			= 16;
	local EXTEND_FRAME_SIZE_LIMIT_NUM	= 2;
	local nSuitNum	= table.getn( t_SuitList );
	if nSuitNum > 0 then
		SparFrameRoleAttrBtnSuitBackGroundButton:Show();
		SparFrameRoleAttrBtnSuitBackGroundButton:SetHeight( math.max( EXTEND_FRAME_SIZE_LIMIT_NUM, table.getn( t_SuitList ) ) * ONE_SUIT_BTN_HEIGHT );
	end
	
	if nSuitNum > EXTEND_FRAME_SIZE_LIMIT_NUM then
		local size = 456 - 32 - MAX_BOTTOM_LEAVE_SPACE +  nSuitNum * 16;
		SparFrameRoleAttrBtn:SetHeight( size );
	else
		SparFrameRoleAttrBtn:SetHeight( 456 );
	end	
end

function SparFramePrivate_ShowEquipAdvanceAttr()
	ClearSparFrameYuanSuAttackAttr();
	ClearSparFrameYuanSuDefendAttr();
	ClearSparFrameFuZhuAttr();

	local t_hideIcons = { "SparFrameRoleAttrBtnSecondBig_YuanSuAttackAttrBackGroundButton", "SparFrameRoleAttrBtnSecondBig_YuanSuDefendAttrBackGroundButton",	
							"SparFrameRoleAttrBtnSecondBig_FuZhuAttrBackGroundButton" };
	util.HideIcons( t_hideIcons );

	local t_showIcons = { "SparFrameRoleAttrBtnSecondBig_FirstChildAttrBackGroundButton", "SparFrameRoleAttrBtnSecondBig_SecondChildAttrBackGroundButton" };
	util.ShowIcons( t_showIcons );
end


-- 更新普通防御属性
function ClearSparFrameNormalDefendAttr()
	for i = 1, MAX_NORMAL_DEFEND_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_DefendAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSparFrameNormalDefendAttr()
	ClearSparFrameNormalDefendAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["防御属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end

		local btn			= getglobal( "SparFrameRoleAttrBtn_DefendAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end
		local funcGetDesc	= oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新普通攻击属性
function ClearSparFrameNormalAttackAttr()
	for i = 1, MAX_NORMAL_ATTACK_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_AttackAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSparFrameNormalAttackAttr()
	ClearSparFrameNormalAttackAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["攻击属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];

	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];

		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local nCurMaxValue  = nil;
		local nBaseMaxValue = nil;

		if oneElem["maxAttrID"] ~= nil then
			nCurMaxValue = funcGetCur( oneElem["maxAttrID"],	mainplayer );
			nBaseMaxValue = funcGetBase( oneElem["maxAttrID"],	mainplayer );
			if nCurMaxValue < nBaseMaxValue then
				t_color	= { r = 255, g = 0, b = 0 };
			end
			nCurValue = nCurValue .. "-" .. nCurMaxValue
		end		

		local btn = getglobal( "SparFrameRoleAttrBtn_AttackAttrBtn"..i );

		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end
		
		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
end

-- 更新基础属性
function ClearSparFrameBaseAttr()
	for i = 1, MAX_BASE_ATTR_NUM do
		local btn = getglobal( "SparFrameRoleAttrBtn_BaseAttrBtn"..i );
		btn:Hide();
	end
end

function UpdateSparFrameBaseAttr()
	ClearSparFrameBaseAttr();

	local mainplayer	= ActorMgr:getMainPlayer();
	local initRace		= GetInitRace( mainplayer:getRace() );
	
	local t_attrList	= t_RoleAttrInfo["基础属性"][initRace];
	local maxAttrNum	= t_attrList["maxElemNum"];
	for i = 1, maxAttrNum do
		local oneElem		= t_attrList[i];
		local funcGetBase	= oneElem["getBaseAttrFunc"] ~= nil and oneElem["getBaseAttrFunc"]	or GetRoleBaseAttr;
		local funcGetCur	= oneElem["getCurAttrFunc"] ~= nil	and oneElem["getCurAttrFunc"]	or GetRoleAttr;

		local nCurValue		= funcGetCur( oneElem["attrID"],	mainplayer );
		local nBaseValue	= funcGetBase( oneElem["attrID"],	mainplayer );
		--print( "ATTR_ID = "..oneElem["attrID"].."， nCurValue = "..nCurValue.."， nBaseValue = "..nBaseValue );

		local t_color	= { r = 255, g = 254, b = 189 };
		if nCurValue > nBaseValue then
			t_color	= { r = 15, g = 255, b = 0 };
		elseif nCurValue < nBaseValue then
			t_color	= { r = 255, g = 0, b = 0 };
		end

		local btn = getglobal( "SparFrameRoleAttrBtn_BaseAttrBtn"..i );
		
		local funcGetTips = oneElem["getAttrTipsDescFunc"];
		if funcGetTips ~= nil then
			btn:SetClientString( funcGetTips( oneElem["attrID"], mainplayer ) );
		else
			btn:SetClientString( oneElem["desc"] );
		end

		local funcGetDesc = oneElem["getAttrDescFunc"];
		local t_fonts = {	[btn:GetName().."NameFont"]		= { value = oneElem["name"] }, 
							[btn:GetName().."ValueFont"]	= 
							{	value = funcGetDesc ~= nil and funcGetDesc( oneElem["attrID"], mainplayer ) or nCurValue,	
								color = t_color, 
							},
						};
		util.SetFonts( t_fonts );
		btn:Show();
	end
	
	local mainAttrFlagBtn		= getglobal( "SparFrameRoleAttrBtnMainAttrFlagButton" );
	local t_changeCareerControl = GetChangeCareerControl();
	local nRace					= mainplayer:getRace();
	if t_changeCareerControl:isChangeCareer( { ["race"] = nRace } ) then
		local szAttrBtn = t_mainAttrConfig[nRace]["btn"];
		mainAttrFlagBtn:SetPoint( "left", szAttrBtn, "left", 35, 0 );
		mainAttrFlagBtn:Show();
	else
		mainAttrFlagBtn:Hide();
	end
end

--------------------------------------------	晶石吸收	-----------------------------------
local MAX_SPAR_SORB_STUFF_NUM = 8

function SparSorbFrameClearInfo()
	SparSorbFrameNameFont:SetText( "" )

	SparSorbFrameItemBtnBindTexture:Hide();
	SparSorbFrameItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	SparSorbFrameItemBtnUVAnimationTex:Hide();
	SparSorbFrameItemBtnCountFont:SetText("");
	
	for i=1,MAX_SPAR_SORB_STUFF_NUM do
		local btn = getglobal( "SparSorbFrameStuffItemBtn" .. i )
		local tex = getglobal( btn:GetName() .. "IconTexture" )
		local bindTex = getglobal( btn:GetName() .. "BindTexture" )
		local uvanmin = getglobal( btn:GetName() .. "UVAnimationTex" )
		local countFont = getglobal( btn:GetName() .. "CountFont" )
		bindTex:Hide();
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		uvanmin:Hide();
		countFont:SetText("");
	end

	SparSorbFrameAddExpFont:SetText( "增加经验值：0" )
end

--获取实际需要消耗银币数值，这里要去掉不消耗银币的那部分
function GetRealExpCostMoney()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local costExp = 0;
	for i=1,MAX_SPAR_SORB_STUFF_NUM do
		local btn = getglobal( "SparSorbFrameStuffItemBtn" .. i )
		nList = btn:GetClientUserData( 0 )
		nGrid = btn:GetClientUserData( 1 )
		item = container:getItem( nList, nGrid )
		if item:getItemId() > 0 then
			local itemDef = item:getItemDef();
			--数量
			local num = item:getNum();
			local nAdd = 0
			if item:getItemType() == ITEM_SLOTTYPE_AMM then
				-- 装备
				local armFenJieDef = GameArmProce:getArmFenJieDef( (itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important );
				if armFenJieDef ~= nil then
					nAdd = armFenJieDef.SparExp
				end
			else
				-- 晶石
				local sparDef = GameArmProce:getSparDef( item:getItemId() )
				local sparItemAdd = 0
				if sparDef ~= nil then
					for i=1,item:getItemInstVal1()-1 do
						sparItemAdd = sparItemAdd + sparDef.UpExp[i-1]
					end
					nAdd = num * ( sparDef.SparExp + sparItemAdd + item:getItemInstVal3() )
				end
			end
			--如果是免费充能道具
			for m = 1, MAX_ITEM_RESULT do
				if itemDef.Result[m-1].ResultID == RESULT_SPAR_SORB_FREE then
					nAdd = 0
				end
			end
			costExp = costExp + nAdd
		end
	end
	return costExp
end

function SparSorbFrameUpdateInfo()
	SparSorbFrameClearInfo();

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local nList = SparSorbFrameItemBtn:GetClientUserData( 0 )
	local nGrid = SparSorbFrameItemBtn:GetClientUserData( 1 )
	local item = container:getItem( nList, nGrid )
	if item:getItemId() > 0 then
		local itemDef = item:getItemDef();
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		SparSorbFrameItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, "SparSorbFrameItemBtnUVAnimationTex",item );
		if item:isItemBind() then
			SparSorbFrameItemBtnBindTexture:Show();
		else
			SparSorbFrameItemBtnBindTexture:Hide();
		end
		SparSorbFrameNameFont:SetText( item:getDisplayName() )
		local t_NameColor = GetItemNameColor( itemDef );
		SparSorbFrameNameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );

		SparSorbFrameLvFont:SetText("");
		SparSorbFrameCurValFont:SetText("")
		SparSorbFrameNextValFont:SetText("")
		SparSorbFrameProgressBtnFont:SetText( "" )
		SparSorbFrameProgressBtnTex:SetSize( 0,0 )
		local sparLv = item:getItemInstVal1()
		local resultID = item:getItemFlag()
		local sparBaseVal = item:getItemInstVal2()
		local sparExp = item:getItemInstVal3()
		local sparDef = GameArmProce:getSparDef( item:getItemId() )
		if sparDef ~= nil and sparLv > 0 then
			SparSorbFrameLvFont:SetText( "等级：" .. sparLv .. "级（最高" .. sparDef.MaxLvl .. "级）" )
			local value = sparBaseVal * ( 2 + sparLv ) / 3 
			SparSorbFrameCurValFont:SetText("当  前：" .. getNewAttrDesc( resultID, { value } ) )
			SparSorbFrameProgressBtnFont:SetText( sparExp .. "/" .. sparDef.UpExp[sparLv-1] )
			if sparLv < sparDef.MaxLvl then
				value = sparBaseVal * ( 2 + sparLv + 1 ) / 3 
				SparSorbFrameNextValFont:SetText("下一级：" ..  getNewAttrDesc( resultID, { value } ) )				
			end
			SparSorbFrameProgressBtnTex:SetSize( 164*( sparExp/sparDef.UpExp[sparLv-1] > 1 and 1 or sparExp/sparDef.UpExp[sparLv-1] ), 9 )
			if sparLv == sparDef.MaxLvl and sparLv > 1 then
				SparSorbFrameProgressBtnFont:SetText( sparDef.UpExp[sparLv-2] .. "/" .. sparDef.UpExp[sparLv-2] )
				SparSorbFrameProgressBtnTex:SetSize( 164, 9 )
			end					
		end
	elseif nList > 0 then
		UnLockItem( nList, nGrid )
		SparSorbFrameItemBtn:SetClientUserData( 0, 0 )
		SparSorbFrameItemBtn:SetClientUserData( 1, 0 )
	end
	
	-- 材料
	local addExp = 0;
	for i=1,MAX_SPAR_SORB_STUFF_NUM do
		local btn = getglobal( "SparSorbFrameStuffItemBtn" .. i )
		nList = btn:GetClientUserData( 0 )
		nGrid = btn:GetClientUserData( 1 )
		item = container:getItem( nList, nGrid )
		if item:getItemId() > 0 then
			-- 显示
			local tex = getglobal( btn:GetName() .. "IconTexture" )
			local bindTex = getglobal( btn:GetName() .. "BindTexture" )
			local countFont = getglobal( btn:GetName() .. "CountFont" )
			local itemDef = item:getItemDef();
			local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end	
			tex:SetTexture( IconPath );
			SetEquipImportantEffectUV( itemDef, btn:GetName() .. "UVAnimationTex",item );
			if item:isItemBind() then
				bindTex:Show();
			else
				bindTex:Hide();
			end
			--数量
			local num = item:getNum();
			if num > 1 then
				countFont:SetText( num )
			end
			if item:getItemType() == ITEM_SLOTTYPE_AMM then
				-- 装备
				local armFenJieDef = GameArmProce:getArmFenJieDef( (itemDef.WearPre.RoleLevelMin / 10 ), itemDef.Important );
				if armFenJieDef ~= nil then
					addExp = addExp + armFenJieDef.SparExp
				end
			else
				-- 晶石
				local sparDef = GameArmProce:getSparDef( item:getItemId() )
				local sparItemAdd = 0
				if sparDef ~= nil then
					for i=1,item:getItemInstVal1()-1 do
						sparItemAdd = sparItemAdd + sparDef.UpExp[i-1]
					end
					addExp = addExp + num * ( sparDef.SparExp + sparItemAdd + item:getItemInstVal3() )
				end
			end
		elseif nList > 0 then
			UnLockItem( nList, nGrid )
			btn:SetClientUserData( 0, 0 )
			btn:SetClientUserData( 1, 0 )
		end
	end
	SparSorbFrameAddExpFont:SetText( "增加经验值：" .. addExp )
	SparSorbFrameTips2Font:SetText( "消耗银币：" .. GetRealExpCostMoney() )
	if addExp > 0 then
		SparSorbFrameOKBtn:Enable()
	else
		SparSorbFrameOKBtn:Disable()
	end
end

function Save2SparSorbFrame( item, nSrcList, nSrcGridIndex )
	local btn = nil
	for i=1,MAX_SPAR_SORB_STUFF_NUM do
		local tmp = getglobal( "SparSorbFrameStuffItemBtn" .. i )
		if tmp:GetClientUserData( 0 ) == 0 then
			btn = tmp
			break
		end
	end
	if btn ~= nil then
		return SparSorbFrameProcessOnReciveDrag( btn:GetName(), item:getListType(), item:getGridIdx(), 0, 0, false );
	end	
end

function SparSorbFrameProcessOnReciveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3, bDrag )
	if dragfrom == DRAG_SPAR_SORB then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	elseif dragfrom ~= CONTAINER_TYPE_SUNDRIES then
		ShowMidTips("只有背包内装备才可进行此操作");
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local srcList		= dragfrom;						-- 源容器类型
	local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
	local item			= container:getItem( srcList, srcGrid );
	local itemdef = item:getItemDef();
	local btn = getglobal( szBtnName )
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		if itemdef.Important == 0 then
			ShowMidTips( "白色品质装备不可进行此操作" );
			return;
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
			ShowMidTips( "时装不可进行此操作" )
			return
		end
		local nList = btn:GetClientUserData( 0 );
		local nGrid  = btn:GetClientUserData( 1 );
		if nList > 0 then
			UnLockItem( nList, nGrid );
		end
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		LockItem( srcList, srcGrid )
	else
		if CheckItemResult( itemdef, RESULT_SPAR ) or CheckItemResult( itemdef, RESULT_SPAR_SHOP ) then
			local nList = btn:GetClientUserData( 0 );
			local nGrid  = btn:GetClientUserData( 1 );
			if nList > 0 then
				UnLockItem( nList, nGrid );
			end
			btn:SetClientUserData( 0,srcList )
			btn:SetClientUserData( 1,srcGrid )
			LockItem( srcList, srcGrid )
		else
			ShowMidTips( "请放入晶石或绿色以上装备" )
			return;
		end
	end
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
	SparSorbFrameUpdateInfo()
end

function SparSorbFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	ItemFirstCallBack( "SparSorbFrame" , Save2SparSorbFrame );
end

function SparSorbFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" then
		SparSorbFrameUpdateInfo();
	end
end

function SparSorbFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	PlayerContainerFrame:SetPoint("topright", "$parent", "topright",-86, 163);  --elmer
	SparSorbFrame:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	if not GameWizard:isDo(GAMEWIZARD_FIRST_SPARFRAME_SHOW) then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
			local xinShouBtn = getglobal( "SparFrameSparItemBtn" .. i .. "XinShouBtn" );
			xinShouBtn:Hide();
		end
		GameWizard:doStep(GAMEWIZARD_FIRST_SPARFRAME_SHOW);
	end
	SparSorbFrameUpdateInfo();
	SparSorbFrameProgressBtnUVAnimationTex:Hide();	
end

function SparSorbFrame_OnHide()
	local nList = SparSorbFrameItemBtn:GetClientUserData( 0 )
	local nGrid = SparSorbFrameItemBtn:GetClientUserData( 1 )
	if nList > 0 then
		UnLockItem( nList, nGrid )
	end
	SparSorbFrameItemBtn:SetClientUserData( 0,0 )
	SparSorbFrameItemBtn:SetClientUserData( 1,0 )

	for i=1,MAX_SPAR_SORB_STUFF_NUM do
		local btn = getglobal( "SparSorbFrameStuffItemBtn" .. i )
		nList = btn:GetClientUserData( 0 )
		nGrid = btn:GetClientUserData( 1 )
		if nList > 0 then
			UnLockItem( nList, nGrid )
		end
		btn:SetClientUserData( 0,0 )
		btn:SetClientUserData( 1,0 )
	end
	if SparFrame:IsShown() then
		UpdateSparFrameInfo();
	end
end

function SparSorbFrameItemButton_OnClick()
	if this:GetName() == "SparSorbFrameItemBtn" then return	end
	if arg1 == "RightButton" then
		local nListType = this:GetClientUserData( 0 );
		local nGridIdx	= this:GetClientUserData( 1 );
		UnLockItem( nListType, nGridIdx );--解锁
		this:SetClientUserData( 0,0 )
		this:SetClientUserData( 1,0 )
		SparSorbFrameUpdateInfo();
	end
end

function SparSorbFrameItemButton_OnEnter()
	local nListType = this:GetClientUserData( 0 );
	local nGridIdx	= this:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx )
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item )
	end
end

function SparSorbFrameItemButton_BeginDrag()
	if this:GetName() == "SparSorbFrameItemBtn" then return	end
	local nListType	= this:GetClientUserData( 0 );
	local nGrid		= this:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_SPAR_SORB;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),this:GetClientID() );
	end
end

function SparSorbFrameItemButton_ReceiveDrag()
	if this:GetName() == "SparSorbFrameItemBtn" then return	end
	SparSorbFrameProcessOnReciveDrag( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3(), true );
end

function SparSorbFrameItemBtnClearBtn( clientId, desList, desGrid  )
	local btn = getglobal( "SparSorbFrameStuffItemBtn" .. clientId );
	local nListType = btn:GetClientUserData( 0 );
	local nGridIdx	= btn:GetClientUserData( 1 );
	UnLockItem( nListType, nGridIdx );--解锁
	btn:SetClientUserData( 0,0 )
	btn:SetClientUserData( 1,0 )
	btn:SetClientUserData( 2,0 )
	SparSorbFrameUpdateInfo();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx )
	if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
		container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
	end

end

function SparSorbFrameOKBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local sparSorbData = GameArmProce:getSparSorbData()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end

	local _,_,money = string.find( SparSorbFrameAddExpFont:GetText(), "(%d+)" );
	if getPlayerMoney( mainplayer ) < tonumber( money ) then
		ShowMidTips( "绑银不足，无法吸收" )
		return
	end
	if checkNeedWarnCost( tonumber( money ) ) then
		CostMoneyConfirmFrame:SetClientString("晶石吸收");
		CostMoneyConfirmFrame:SetClientUserData( 0, MAX_SPAR_SORB_STUFF_NUM );
		SetWarnFrame( "SparSorbFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	local container = mainplayer:getContainer()
	for i=1,MAX_SPAR_SORB_STUFF_NUM do
		local btn = getglobal( "SparSorbFrameStuffItemBtn" .. i )
		local nList = btn:GetClientUserData( 0 )
		local nGrid = btn:GetClientUserData( 1 )
		local item = container:getItem( nList, nGrid )
		if item:getItemId() > 0 then
			sparSorbData.StuffData[sparSorbData.StuffNum].ListType = item:getListType()
			sparSorbData.StuffData[sparSorbData.StuffNum].GridIdx = item:getGridIdx()
			sparSorbData.StuffData[sparSorbData.StuffNum].ItemNum = item:getNum()
			sparSorbData.StuffData[sparSorbData.StuffNum].ItemID = item:getItemId()
			sparSorbData.StuffNum = sparSorbData.StuffNum + 1
		end
	end
	
	local srcList = SparSorbFrameItemBtn:GetClientUserData( 0 )
	local srcGrid = SparSorbFrameItemBtn:GetClientUserData( 1 )
	sparSorbData.Src.ArmListType = srcList
	sparSorbData.Src.ArmGridIdx = srcGrid
	GameArmProce:requestSparSorb();
end

function ArmSparSorResult( result )
	if result == 1 then
		SparSorbFrameProgressBtnUVAnimationTex:SetUVAnimation( 100, false );
	else
		SparSorbFrameProgressBtnUVAnimationTex:Hide();
	end
end
