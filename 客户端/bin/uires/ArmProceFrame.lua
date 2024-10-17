
function ArmProceFrame_OnLoad()
	local szText1 = "欢迎使用装备改造系统，请点击下方标签进行功能选择。强化、重铸、洗炼、附魔、镶嵌……让你从头到脚从里到外变得如神一样强悍，变成一夫当关万夫莫开的大英雄！"
	local szText2 = [[获取蓝色品质以上的装备：
1.#G怪物#n或#GBOSS#n掉落
2.#G战场#n荣誉购买
3.生产科技的#G铸造#n专业制造]]
	ArmProceFrameRich1:SetText( szText1, 160, 155, 130 )
	ArmProceFrameRich2:SetText( szText2, 255, 255, 190 )
end

function ArmProceFrameDisCheckAllBtn( szParent )
	local t_btn = { szParent .. "RecastBtn", szParent .. "StarBtn", szParent .. "WashBtn", szParent .. "BindBtn", szParent .. "MoveBtn" }
	for i=1,#t_btn do
		local btn = getglobal( t_btn[i] )
		btn:DisChecked()
	end
end

function ArmProceFrame_OnShow()
	ArmProceFrameDisCheckAllBtn( this:GetName() )
end

function ArmProceFrameTemplateCloseBtn_OnClick()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( "重铸面板关闭" );
		return;
	end
	if ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "强化", "本次强化结果未保存，确定关闭强化吗？" );
		MessageBoxFrame:SetClientString( "强化面板关闭" );
		return;
	end
	if ArmRecastSelectAttrFrame:IsShown() then
		MessageBox( "批量重铸", "本次批量重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( "批量重铸面板关闭" );
		return;
	end
	util.closeParent(this);
end

function ArmProceFrameRecastBtn_OnClick()
	if ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "强化切换重铸", "本次强化结果未保存，确定关闭强化吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmWashFrameSaveBtn:IsShown() then
		MessageBox( "洗炼切换重铸", "本次洗炼结果未保存，确定关闭洗炼吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if this:IsChecked() then
		return
	end
	local parent = getglobal( this:GetParent() )
	local npcid = parent:GetClientUserData( 0 )
	parent:Hide()
	ArmRecastFrame:Show()
	ArmRecastFrame:SetClientUserData( 0,npcid )
	ArmRecastFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmRecastFrame" )
	ArmRecastFrameRecastBtn:Checked()
	--util.testNpcDialogDistance( npcid, ArmRecastFrame );
end

function ArmProceFrameStarBtn_OnClick()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸切换强化", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmWashFrameSaveBtn:IsShown() then
		MessageBox( "洗炼切换强化", "本次洗炼结果未保存，确定关闭洗炼吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmRecastSelectAttrFrame:IsShown() then
		MessageBox( "批量重铸切换强化", "本次批量重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString(  "ArmAdvRecastFrameStarBtn"  );
		return;
	end
	if this:IsChecked() then
		return
	end
	local parent = getglobal( this:GetParent() )
	local npcid = parent:GetClientUserData( 0 )
	parent:Hide()
	ArmProceStarFrame:Show()
	ArmProceStarFrame:SetClientUserData( 0,npcid )
	ArmProceStarFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmProceStarFrame" )
	ArmProceStarFrameStarBtn:Checked()
	--util.testNpcDialogDistance( npcid, ArmProceStarFrame );
end

function ArmProceFrameWashBtn_OnClick()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸切换洗炼", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "强化切换洗炼", "本次强化结果未保存，确定关闭强化吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmRecastSelectAttrFrame:IsShown() then
		MessageBox( "批量重铸切换洗炼", "本次批量重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString(  "ArmAdvRecastFrameWashBtn"  );
		return;
	end
	if this:IsChecked() then
		return
	end
	local parent = getglobal( this:GetParent() )
	local npcid = parent:GetClientUserData( 0 )
	parent:Hide()
	ArmWashFrame:Show()
	ArmWashFrame:SetClientUserData( 0,npcid )
	ArmWashFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmWashFrame" )
	ArmWashFrameWashBtn:Checked()
	--util.testNpcDialogDistance( npcid, ArmWashFrame );
end

function ArmProceFrameBindBtn_OnClick()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸切换绑定", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "强化切换绑定", "本次强化结果未保存，确定关闭强化吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmWashFrameSaveBtn:IsShown() then
		MessageBox( "洗炼切换绑定", "本次洗炼结果未保存，确定关闭洗炼吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmRecastSelectAttrFrame:IsShown() then
		MessageBox( "批量重铸切换绑定", "本次批量重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString(  "ArmAdvRecastFrameBindBtn"  );
		return;
	end
	if this:IsChecked() then
		return
	end
	local parent = getglobal( this:GetParent() )
	local npcid = parent:GetClientUserData( 0 )
	parent:Hide()
	ArmProceBindFrame:Show()
	ArmProceBindFrame:SetClientUserData( 0,npcid )
	ArmProceBindFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmProceBindFrame" )
	ArmProceBindFrameBindBtn:Checked()
	--util.testNpcDialogDistance( npcid, ArmProceBindFrame );
end

function ArmProceFrameMoveBtn_OnClick()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸切换转星", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "强化切换转星", "本次强化结果未保存，确定关闭强化吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmWashFrameSaveBtn:IsShown() then
		MessageBox( "洗炼切换转星", "本次洗炼结果未保存，确定关闭洗炼吗？" );
		MessageBoxFrame:SetClientString( this:GetName() );
		return;
	end
	if ArmRecastSelectAttrFrame:IsShown() then
		MessageBox( "批量重铸切换转星", "本次批量重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString(  "ArmAdvRecastFrameMoveBtn"  );
		return;
	end
	if this:IsChecked() then
		return
	end
	local parent = getglobal( this:GetParent() )
	local npcid = parent:GetClientUserData( 0 )
	parent:Hide()
	ArmProceStarMoveFrame:Show()
	ArmProceStarMoveFrame:SetClientUserData( 0,npcid )
	ArmProceStarMoveFrame:SetPoint( "topleft", "UIClient", "topleft", parent:GetRealLeft(), parent:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmProceStarMoveFrame" )
	ArmProceStarMoveFrameMoveBtn:Checked()	
	--util.testNpcDialogDistance( npcid, ArmProceStarMoveFrame );
end

function ArmProceFrameBtn1_OnEnter()
	ArmProceTipsFrame:SetSize( 536, 416 )
	ArmProceTipsFrameTex:SetTexture( "uires\\TuPianLei\\ZhuangBeiShiLi\\ChongZhuShiLi 536x416.tga" )
	ArmProceTipsFrame:Show()
	ArmProceTipsFrame:SetPoint( "left",this:GetName(),"right",0,0 )
end
function ArmProceFrameBtn2_OnEnter()
	ArmProceTipsFrame:SetSize( 536, 416 )
	ArmProceTipsFrameTex:SetTexture( "uires\\TuPianLei\\ZhuangBeiShiLi\\QiangHuaShiLi 536x416.tga" )
	ArmProceTipsFrame:Show()
	ArmProceTipsFrame:SetPoint( "left",this:GetName(),"right",0,0 )
end
function ArmProceFrameBtn3_OnEnter()
	ArmProceTipsFrame:SetSize( 536, 416 )
	ArmProceTipsFrameTex:SetTexture( "uires\\TuPianLei\\ZhuangBeiShiLi\\XiLianShiLi 536x416.tga" )
	ArmProceTipsFrame:Show()
	ArmProceTipsFrame:SetPoint( "left",this:GetName(),"right",0,0 )
end
function ArmProceFrameBtn4_OnEnter()
	ArmProceTipsFrame:SetSize( 536, 578 )
	ArmProceTipsFrameTex:SetTexture( "uires\\TuPianLei\\ZhuangBeiShiLi\\FuMoShiLi 536x416.tga" )
	ArmProceTipsFrame:Show()
	ArmProceTipsFrame:SetPoint( "left",this:GetName(),"right",0,-30 )
end
function ArmProceFrameBtn5_OnEnter()
	ArmProceTipsFrame:SetSize( 536, 610 )
	ArmProceTipsFrameTex:SetTexture( "uires\\TuPianLei\\ZhuangBeiShiLi\\XiangQianShiLi 536x416.tga" )
	ArmProceTipsFrame:Show()
	ArmProceTipsFrame:SetPoint( "left",this:GetName(),"right",0,-40 )
end
function ArmProceFrameBtn6_OnEnter()
	SetGameTooltips( this:GetName(), "装备绑定：将装备与角色绑定，#G免于#n死亡#G掉落#n" );
end
function ArmProceFrameBtn7_OnEnter()
	SetGameTooltips( this:GetName(), "星级转移：将已强化装备的星级转移到其他装备上" );
end

function ArmProceFrameBtn_OnLeave()
	ArmProceTipsFrame:Hide();
end

function ArmProceFrameBtn1_OnClick()
	this = ArmProceFrameRecastBtn
	ArmProceFrameRecastBtn_OnClick()
end

function ArmProceFrameBtn2_OnClick()
	this = ArmProceFrameStarBtn
	ArmProceFrameStarBtn_OnClick()
end

function ArmProceFrameBtn3_OnClick()
	this = ArmProceFrameWashBtn
	ArmProceFrameWashBtn_OnClick()
end

function ArmProceFrameBtn6_OnClick()
	this = ArmProceFrameBindBtn
	ArmProceFrameBindBtn_OnClick()
end

function ArmProceFrameBtn7_OnClick()
	this = ArmProceFrameMoveBtn
	ArmProceFrameMoveBtn_OnClick()
end

function ArmProceTipsFrame_OnHide()
	ArmProceTipsFrameTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
end

------------------------------------------------------------------		装备强化		------------------------------------------------------------------------------
local MAX_ITEM_STAR = 12
local MAX_ARM_PROCE_STAR_DESC_LINE = 5;
local MAX_STAR_MUL_LIST = 10;
local nCurStarChose = 0;
local nCurStarMulIdx = 0;
local IsNeedGemExist = false;
local IsNeedGemBind = false;
local bChoseNextStar = false;
local bCanStarUp = true;
local bHasSaved = true;
local ITEM_STAT_UP_FIRST_PERFECT_TOLUA = 5200026;--必定完美道具
--人品值积累
--此处是从-9001~9001，所以初始化为-20000，
local ArmProceStarPerfectOldVal = -20000
--自动强化标记
local IsAutoArmStar = false

--可作为强化材料的物品Id列表
local t_CanAddArmStarItem = {}

function ArmProceFrameOnRightClick( szBtnName, desList, desGrid, nCount )
	if szBtnName == "ArmProceUnBindFrameArmItemBtn" and ArmProceUnBindFrameOKBtn:GetText() == "关 闭" then
		return;
	end
	if szBtnName == "ArmProceStarFrameArmItemBtn" and IsArmProceStarMessageBoxShown() then
		return;
	elseif not nCount and szBtnName == "ArmProceStarFrameNeedItemBtn" and ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "装备强化保存","当前强化星级属性尚未保存，点击确定保存");
		MessageBoxFrame:SetClientString("装备强化保存");
		return;
	end
	if szBtnName == "ArmProceUnBindFrameArmItemBtn" and MessageBoxFrame:IsShown() and MessageTitle:GetText() == "装备解绑" then
		return;
	end

	local btn	= getglobal( szBtnName );
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local container = MainPlayer:getContainer();
	if btn:GetClientUserData(0) < 0 or btn:GetClientUserData(1) < 0 then
		return
	end
	local item = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
	if item:getItemId() ~= 0 then
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );
	end
	btn:SetClientUserData(0,-1);
	btn:SetClientUserData(1,-1);
	btn:SetClientUserData(2,-1);
	local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex");
	UVAnimationTex:Hide();
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	if szBtnName == "ArmProceStarFrameArmItemBtn" then
		ClearArmProceStarFrameInfo();
		ArmProceStarFrameOKBtn:Disable();
		if desList ~= nil and desGrid ~= nil then
			if item ~= nil and item:getGridIdx() ~= desGrid then
				container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
			end
		end
	end
	if szBtnName == "ArmProceStarFrameNeedItemBtn" then
		IsNeedGemExist = false;
		IsNeedGemBind = false;
		ArmProceStarFrameNeedItemBtnCountFont:SetText("");
		ArmProceStarFrameNeedItemBtnBindTexture:Hide();
		ClearItemLock( szBtnName )
		ArmProceStarFrameOKBtn:Disable()
		SetArmProceStatNeedInfo()
	end
	if szBtnName == "ArmProceBindFrameArmItemBtn" then
		ArmProceBindFrameArmItemBtnBindTexture:Hide();
	end
end

function ArmProceStarFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent( "GE_ARMSTAR_SUCESS" );
	this:RegisterEvent( "GE_ARMSTAR_FAILED" );
	this:RegisterEvent( "GE_ARM_STAR_UP_RESULT" );

	ItemFirstCallBack( "ArmProceStarFrame" , Save2ArmSart );
	
	--先在列表里加入星辉宝钻
	table.insert( t_CanAddArmStarItem, ARM_STAR_ITEM_TOLUA )
	--下一星必定完美道具
	table.insert( t_CanAddArmStarItem, ITEM_STAT_UP_FIRST_PERFECT_TOLUA )
	--遍历装备星级表，将必定可以强化到完美星的物品加入列表
	for i = 1, ARM_STAR_LIMIT do
		local armStarDef = getArmStarDef( i )
		if armStarDef then
			if armStarDef.MustItemID ~= 0 and armStarDef.MustItemID ~= nil then
				table.insert( t_CanAddArmStarItem, armStarDef.MustItemID )
			end
		end
	end
end

function IsItemCanAddArmStar( nItemId )
	for i = 1, table.getn( t_CanAddArmStarItem ) do
		if nItemId == t_CanAddArmStarItem[i] then
			return true
		end
	end
	return false
end

function Save2ArmSart( item, nSrcList, nSrcGridIndex )
	if item:getItemType() == ITEM_SLOTTYPE_AMM then
		ReceiveArmProceStar( "ArmProceStarFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	elseif IsItemCanAddArmStar( item:getItemId() ) then
		ReceiveArmProceStar( "ArmProceStarFrameNeedItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	else
		ShowMidTips("请放入装备或者星辉宝钻");
	end
end

function ArmProceStarFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" and ArmProceStarFrame:IsShown() then
		UpdateArmProceStarFrame();
	end
	if arg1 == "GE_ARMSTAR_SUCESS" then
		UpdateArmProceStarFrame()
		ArmProceStarFrameArmItemBtnSuccessEffectTex:SetUVAnimation( 100,false );
		bCanStarUp = true;
	end
	if arg1 == "GE_ARMSTAR_FAILED" then
		UpdateArmProceStarFrame()
		ArmProceStarFrameArmItemBtnFailEffectTex:SetUVAnimation( 100,false );
		bCanStarUp = true;
	end
	if arg1 == "GE_ARM_STAR_UP_RESULT" then
		if bChoseNextStar then
			UpdateArmProceStarFrame();
			local itemId = ArmProceStarFrameNeedItemBtn:GetClientUserData(2);
			if itemId > 0 and itemId ~= ARM_STAR_ITEM_TOLUA then			
				ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
				ArmProceStarFrameOKBtn:Disable()
			end
		end
	end
end

function ClearArmProceStarFrameInfo()
	
	bChoseNextStar = false;
	bCanStarUp = true;
	bHasSaved = true;
	IsAutoArmStar = false;		--自动强化标记也重置下
	
	--把星级也全部隐藏
	for i=1,MAX_ITEM_STAR do
		local btn = getglobal("ArmProceStarFrameStar"..i);
		btn:Hide();
	end
	ArmProceStarFrameNeedItemFont:SetText("");
	ArmProceStarFrameNeedMoneyFont:SetText( "0" );
	ArmProceStarFrameArmStarFont:Hide();
	
	ArmProceStarFrameArmItemBtnBindTexture:Hide();
	ArmProceStarFrameArmItemBtnFailEffectTex:Hide();
	ArmProceStarFrameArmItemBtnSuccessEffectTex:Hide();
	ArmProceStarFrameSaveBkgTex:Hide();
	ArmProceStarFrameArrowTex:Hide();
	
	ArmProceStarFrameSaveBtn:Hide();
	ArmProceStarFrameCancleBtn:Hide();
	
	ArmProceStarFrameCurChooseTips:Hide();
	
	ArmProceStarFrameCurArmStarProceFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont:SetText("");
	ArmProceStarFrameNextArmStarProceFont:SetText("");
	ArmProceStarFrameNextArmStarAddFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont2:SetText("");
	ArmProceStarFrameNextArmStarAddFont2:SetText("");
	
	ArmProceStarFrameAddStarUp:Hide();
	ArmProceStarFrameAddStarEqual:Hide();
	ArmProceStarFrameAddStarDown:Hide();	
	
	ArmProceStarFramePerfectFont:Hide()
	ArmProceStarFramePerfect:Hide()
	ArmProceStarFramePerfectEffect:Hide();
	ArmProceStarFramePerfectModelEffect:Hide()
	ArmProceStarPerfectOldVal = -20000
end

function ArmProceStarFrame_OnShow()
	ArmProceStarFrame_OnHide();
	
	IsNeedGemExist = false;
	IsNeedGemBind = false;

	ArmProceStarFrameArmItemBtn:SetClientUserData( 0,-1 );
	ArmProceStarFrameArmItemBtn:SetClientUserData( 1,-1 );
	ArmProceStarFrameArmItemBtn:SetClientUserData( 2,-1 );
	ArmProceStarFrameArmItemBtnBackTexture:Show();
	ArmProceStarFrameArmItemBtnUVAnimationTex:Hide();
	ArmProceStarFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	
	ArmProceStarFrameNeedItemBtn:SetClientUserData( 0, -1 );
	ArmProceStarFrameNeedItemBtn:SetClientUserData( 1, -1 );
	ArmProceStarFrameNeedItemBtn:SetClientUserData( 2, -1 );
	ArmProceStarFrameOKBtn:Disable();
	nCurStarChose = 1;
	ArmProceStarFrameNeedItemBtnCountFont:SetText("");
	ClearArmProceStarFrameInfo();
	ClearItemLock( "ArmProceStarFrameNeedItemBtn" )
end

function ArmProceStarFrame_OnESC()
	if ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "强化", "本次强化结果未保存，确定关闭强化吗？" );
		MessageBoxFrame:SetClientString( "强化面板关闭" );
		return true;
	end
	return false
end

function ArmProceStarFrame_OnHide()
	if MessageBoxFrame:IsShown() then
		local szMsg = MessageBoxFrame:GetClientString();
		if szMsg =="装备强化" or szMsg =="装备强化保存" or szMsg =="完美强化提示" or szMsg =="装备强化绑定提示" or szMsg == "强化面板关闭"
			or MessageTitle:GetText() == "强化切换重铸" or MessageTitle:GetText() == "强化切换洗炼"
			or MessageTitle:GetText() == "强化切换绑定" or MessageTitle:GetText() == "强化切换转星"
			or MessageTitle:GetText() == "完美保存提示" then
			MessageBoxFrame:Hide();
		end
	end
	ArmProceStarFrameSaveBtn:Hide();
	ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn");
	
	ArmProceFrameOnRightClick("ArmProceStarFrameArmItemBtn");
	ArmProceStarFrameArmItemBtnFailEffectTex:StopUVAnim();
	ArmProceStarFrameArmItemBtnSuccessEffectTex:StopUVAnim();
	
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "装备强化" then
		MessageBoxFrame:Hide();
	end
	bCanStarUp = true;
	
	IsAutoArmStar = false
end

function ArmProceStarStartDrag( szBtnName )
	if IsArmProceStarMessageBoxShown() then
		return;
	end
	if szBtnName == "ArmProceStarFrameNeedItemBtn" and IsNeedGemExist then
		UIMSG:getUIGV().DragFrom = DRAG_ARMPROCE;
		local IconTex = getglobal("ArmProceStarFrameNeedItemBtnIconTexture");
		UIBeginDrag(IconTex:GetTexture(), 0, 0, 0 );
		return;
	elseif szBtnName == "ArmProceStarFrameNeedItemBtn" and ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "装备强化保存","当前强化星级属性尚未保存，点击确定保存");
		MessageBoxFrame:SetClientString("装备强化保存");
		return;
	end
	local btn		= getglobal( szBtnName );
	local nListType	= btn:GetClientUserData( 0 ) ;
	local nGrid		= btn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARMPROCE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),item:getNum() );
		return;
	end
end

function IsArmProceStarMessageBoxShown()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "装备强化" then
		return true;
	end
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "装备强化保存" then
		return true;
	end
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "完美强化提示" then
		return true;
	end
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "装备强化绑定提示" then
		return true;
	end
	return false;
end

function ReceiveArmProceStar( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if IsArmProceStarMessageBoxShown() then
		return;
	elseif szBtnName == "ArmProceStarFrameNeedItemBtn" and ArmProceStarFrameSaveBtn:IsShown() then
		MessageBox( "装备强化保存","当前强化星级属性尚未保存，点击确定保存");
		MessageBoxFrame:SetClientString("装备强化保存");
		return;
	end
	local btn = getglobal( szBtnName );
	-- 如果目标来自背包
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local srcList		= dragfrom;						-- 源容器类型
		local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
		local item		= container:getItem( srcList, srcGrid );
		if szBtnName == "ArmProceStarFrameArmItemBtn" then
			if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
				ShowMidTips("请放入装备");
				return;
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
				ShowMidTips( "时装不能放入" );
				return;
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
				ShowMidTips( "徽章装备不能放入" );
				return;
			end
		elseif szBtnName == "ArmProceStarFrameNeedItemBtn" then
			if not IsItemCanAddArmStar( item:getItemId() ) then
				ShowMidTips("请放入星辉宝钻");
				return;
			end
		end
		local itemdef		= item:getItemDef();
		if szBtnName == "ArmProceStarFrameArmItemBtn" then
			local nListType = btn:GetClientUserData( 0 );
			local nGridIdx	= btn:GetClientUserData( 1 );
		
			local srcitem = container:getItem( nListType,nGridIdx );			
			local nItemId	= srcitem:getItemId();
			if nItemId > 0 then
				UnLockItem( nListType, nGridIdx, nItemId );--解锁
			end			
			ClearArmProceStarFrameInfo();
		else
			ClearItemLock( "ArmProceStarFrameNeedItemBtn" )
			ArmProceStarFrameNeedItemBtnCountFont:SetText( "" );
		end
		btn:SetClientUserData( 0,srcList );
		btn:SetClientUserData( 1,srcGrid );
		btn:SetClientUserData( 2,item:getItemId() );
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		if szBtnName == "ArmProceStarFrameNeedItemBtn" then
			IsNeedGemExist = true;
			IsNeedGemBind = item:isItemBind();
			UnLockItem( item:getListType(), item:getGridIdx() );
			CheckNeedResetChoseStar( item:getItemId() );
		end
		if szBtnName == "ArmProceStarFrameArmItemBtn" then
			setCurChoseStar( item );
			ArmProceAutoAddStarItem();
		end
		UpdateArmProceStarFrame();
	else
		ShowMidTips("只有背包内装备才可进行此操作");
	end

	if dragfrom == DRAG_ARMPROCE then
		local desGrid = btn:GetClientID()-1;
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;	 
		return;
	end
end

function ArmProceAutoAddStarItem()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container		= mainplayer:getContainer();
	local needItemBtn	= getglobal("ArmProceStarFrameNeedItemBtn");
	if not IsNeedGemExist then
		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item		= container:getItem( CONTAINER_TYPE_SUNDRIES, i );
			local itemId	= item:getItemId();
			if itemId == ARM_STAR_ITEM_TOLUA and item:isItemBind() then
				IsNeedGemExist	= true;
				IsNeedGemBind	= true;
				ArmProceStarFrameNeedItemBtn:SetClientUserData( 0,CONTAINER_TYPE_SUNDRIES );
				ArmProceStarFrameNeedItemBtn:SetClientUserData( 1,i );
				ArmProceStarFrameNeedItemBtn:SetClientUserData( 2,ARM_STAR_ITEM_TOLUA );
				break;
			end
			if itemId == ARM_STAR_ITEM_TOLUA then
				IsNeedGemExist	= true;
				IsNeedGemBind	= false;
				ArmProceStarFrameNeedItemBtn:SetClientUserData( 0,CONTAINER_TYPE_SUNDRIES );
				ArmProceStarFrameNeedItemBtn:SetClientUserData( 1,i );
				ArmProceStarFrameNeedItemBtn:SetClientUserData( 2,ARM_STAR_ITEM_TOLUA );
			end
		end
	end
end

function SetArmProceStatNeedInfo()
	local MainPlayer = ActorMgr:getMainPlayer()
	local container = MainPlayer:getContainer()
	local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData( 0 )
	local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData( 1 )
	local needItem = container:getItem( nNeedListType, nNeedGridIdx )
	
	if nCurStarChose > 0 then
		local itemDef = getItemDef( ARM_STAR_ITEM_TOLUA );
		if itemDef == nil then return end
		local nNeedCount = GameArmProce:getStarNeedStuffNum( nCurStarChose );
		if nNeedCount >= 0 then
			ArmProceStarFrameNeedItemFont:SetText( itemDef.Name .. nNeedCount .."个" );
			local t_NameColor = GetItemNameColor( itemDef );
			ArmProceStarFrameNeedItemFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
		end
		
		if needItem ~= nil then
			local armStarDef = getArmStarDef( nCurStarChose )
			if IsItemCanAddArmStar( needItem:getItemId() ) and needItem:getItemId() ~= ARM_STAR_ITEM_TOLUA then
				local itemId = needItem:getItemId() ~= ITEM_STAT_UP_FIRST_PERFECT_TOLUA and armStarDef.MustItemID or ITEM_STAT_UP_FIRST_PERFECT_TOLUA;
				if itemId == 0 then return end
				local itemdef = getItemDef( itemId );
				if itemdef == nil then return end
				ArmProceStarFrameNeedItemFont:SetText( itemdef.Name .. "1个" );
				local t_NameColor = GetItemNameColor( itemdef );
				ArmProceStarFrameNeedItemFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
			end
		end
	else
		ArmProceStarFrameNeedItemFont:SetText( "" );
	end

	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );	
	local item = container:getItem( nListType, nGrid );
	if item ~= nil then
		local nNeedMoney = nCurStarChose*10
		if nNeedMoney >= 0 then
			ArmProceStarFrameNeedMoneyFont:SetText( nNeedMoney );
		end
	end
end

function getFirstChoseStar( item )
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer == nil then
		return item:getStart() + 1;
	end
	local container = MainPlayer:getContainer()
	local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData( 0 )
	local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData( 1 )
	local needItem = container:getItem( nNeedListType, nNeedGridIdx )
	if needItem:getItemId() ~= 0 and needItem:getItemId() == ITEM_STAT_UP_FIRST_PERFECT_TOLUA then
		for i=1, item:getStart() do
			local starValIdx = item:getArmAddStarValIdx( i-1 );
			if starValIdx < MAX_STAR_MUL_LIST - 1 then
				return i;
			end
		end
	end
	return item:getStart() + 1;
end

function CheckNeedResetChoseStar( itemId )
	if itemId == ITEM_STAT_UP_FIRST_PERFECT_TOLUA then
		local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
		local nGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
		local MainPlayer = ActorMgr:getMainPlayer()
		if MainPlayer == nil then return end;
		local container = MainPlayer:getContainer()
		local item = container:getItem( nListType, nGrid );
		if item:getItemId() ~= 0 then
			setCurChoseStar( item );
		end
	end
end

function setCurChoseStar( item )
	if not bHasSaved then return end
	for i = 1, ARM_STAR_LIMIT do
		local btn = getglobal( "ArmProceStarFrameStar"..i );
		btn:SetPoint( "topleft", "ArmProceStarFrame", "topleft", 97 + math.floor(180/item:getItemDef().StarLimit) * i, 154 )
		local btnTex = getglobal( "ArmProceStarFrameStar"..i.."StarTex" );
		btnTex:SetTexUV( 577, 64, 13, 12 );
	end
	if item:getStart() < item:getItemDef().StarLimit then
		nCurStarChose = getFirstChoseStar( item );
		local btn = getglobal( "ArmProceStarFrameStar"..nCurStarChose );
		btn:Enable();
		local btnTex = getglobal( btn:GetName().."StarTex" );
		btnTex:SetTexUV( 625, 622, 13, 12 );
		SetStarMulinfo();
		SetPerfectInfo();
		for i = 1, MAX_ITEM_STAR do
			local starChoseTex = getglobal("ArmProceStarFrameStar"..i.."ChoseStarTex");
			starChoseTex:Hide();
		end
		local bigStar = getglobal( "ArmProceStarFrameStar"..nCurStarChose.."ChoseStarTex" );
		bigStar:Show();
		local posX = 27 + math.floor(200/item:getItemDef().StarLimit) * nCurStarChose;
		ArmProceStarFrameCurChooseTips:SetPoint( "topleft","ArmProceStarFrame", "topleft", posX, 106 );
		ArmProceStarFrameCurChooseTips:Show();
		--材料
		SetArmProceStatNeedInfo()
		return;
	end
	for i = 1, item:getStart() do
		local starValIdx = item:getArmAddStarValIdx( i - 1 );
		if starValIdx >= 0 and starValIdx <= MAX_STAR_MUL_LIST then
			local starLv = item:getStart();
			if starLv > item:getItemDef().StarLimit then
				return;
			end
			local armStarDef = getArmStarDef( starLv );
			local mul = armStarDef.StarInfo[starValIdx].Mul;
			local minMul,maxMul = getArmDefMinAndMaxMul( armStarDef );
			if mul < maxMul then
				nCurStarChose = i;
				SetStarMulinfo();
				SetPerfectInfo();
				for i = 1, MAX_ITEM_STAR do
					local starChoseTex = getglobal("ArmProceStarFrameStar"..i.."ChoseStarTex");
					starChoseTex:Hide();
				end
				local bigStar = getglobal( "ArmProceStarFrameStar"..nCurStarChose.."ChoseStarTex" );
				bigStar:Show();
				local posX = 27 + math.floor(200/item:getItemDef().StarLimit) * nCurStarChose;
				ArmProceStarFrameCurChooseTips:SetPoint( "topleft","ArmProceStarFrame", "topleft", posX, 106 );
				ArmProceStarFrameCurChooseTips:Show();
				--材料
				SetArmProceStatNeedInfo()
				return;
			end				
		end
	end
	--如果9星全完美星
	for i = 1, MAX_ITEM_STAR do
		local starChoseTex = getglobal("ArmProceStarFrameStar"..i.."ChoseStarTex");
		starChoseTex:Hide();
	end
	nCurStarChose = 0;
	ArmProceStarFrameCurChooseTips:Hide();
	ArmProceStarFrameCurArmStarProceFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont:SetText("");
	ArmProceStarFrameNextArmStarAddFont:SetText("");
	ArmProceStarFrameNextArmStarProceFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont2:SetText("");
	ArmProceStarFrameNextArmStarAddFont2:SetText("");
end

function SetStarMulinfo()
	if nCurStarChose <= 0 then return end
	ArmProceStarFrameCurArmStarProceFont:SetTextColor( 255, 255, 255 );
	ArmProceStarFrameCurArmStarAddFont:SetTextColor( 255, 255, 255 );
	ArmProceStarFrameCurArmStarAddFont2:SetTextColor( 150, 40, 255 );
	local armListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local item = container:getItem( armListType, armGrid );
	
	ArmProceStarFrameCurArmStarProceFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont2:SetText("");
	
	if item == nil then return end
	
	if nCurStarChose > item:getStart() then return end
		
	local starValIdx = item:getArmAddStarValIdx( nCurStarChose - 1 );
	if starValIdx >= 0 and starValIdx <= MAX_STAR_MUL_LIST then
		local armStarDef = getArmStarDef( nCurStarChose );
		local mul = armStarDef.StarInfo[starValIdx].Mul;
		local minMul,maxMul = getArmDefMinAndMaxMul( armStarDef );
		if mul >= maxMul then
			ArmProceStarFrameCurArmStarProceFont:SetText("完美强化");
			ArmProceStarFrameCurArmStarProceFont:SetTextColor( 150, 40, 255 );
			ArmProceStarFrameCurArmStarAddFont:SetTextColor( 150, 40, 255 );			
		else
			ArmProceStarFrameCurArmStarProceFont:SetText("普通强化");
		end			
		ArmProceStarFrameCurArmStarAddFont:SetText(mul.."%");
		ArmProceStarFrameCurArmStarAddFont2:SetText("( "..minMul.."%-"..maxMul.."% )");
	end
end

function UpdateArmProceStarFrame()	
	ArmProceStarFrameOKBtn:Disable();
	ArmProceStarFrameNeedItemBtnBindTexture:Hide();
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	
	local nCount = 0;
	
	if IsNeedGemExist then
		-- 设置图标，
		ArmProceStarFrameNeedItemBtn:Show();
		local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData(0)
		local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData(1)
		local needItem = container:getItem( nNeedListType, nNeedGridIdx )
		if IsNeedGemBind then
			nCount = container:getBindItemNumNotCheckLock( CONTAINER_TYPE_SUNDRIES, ArmProceStarFrameNeedItemBtn:GetClientUserData(2) );
		else
			nCount = container:getUnBindItemNumNotCheckLock( CONTAINER_TYPE_SUNDRIES, ArmProceStarFrameNeedItemBtn:GetClientUserData(2) );
		end
		if nCount <= 0 then
			if ArmProceStarFrameNeedItemBtn:GetClientUserData(2) == ARM_STAR_ITEM_TOLUA then
				MessageBoxForItem("提示",getItemDef(ARM_STAR_ITEM_TOLUA).Name);
				MessageBoxForItemFrame:SetClientUserData(0,ARM_STAR_ITEM_TOLUA);
				--同时将自动强化停止
				IsAutoArmStar = false
			end
			ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
			ArmProceStarFrameOKBtn:Disable()
			return
		else
			if needItem:getItemId() == 0 then
				needItem = container:getItem( ArmProceStarFrameNeedItemBtn:GetClientUserData(2) )
			end
		end
		if needItem:getItemId() == 0 then
			--同时将自动强化停止
			IsAutoArmStar = false
			ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
			ArmProceStarFrameOKBtn:Disable()
			return
		end
		local gemItemDef = needItem:getItemDef();
		if gemItemDef == nil then
			--同时将自动强化停止
			IsAutoArmStar = false
			ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
			ArmProceStarFrameOKBtn:Disable()
			return
		end
		local IconPath = GetItemEquipIconPath()..gemItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		ArmProceStarFrameNeedItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( gemItemDef, "ArmProceStarFrameNeedItemBtnUVAnimationTex");
		--材料
		if needItem:getItemId() == ARM_STAR_ITEM_TOLUA then
			local itemDef = getItemDef( ARM_STAR_ITEM_TOLUA );
			if itemDef == nil then return end
			ClearItemLock( "ArmProceStarFrameNeedItemBtn" )
			if IsNeedGemBind then
				ArmProceStarFrameNeedItemBtnBindTexture:Show();
				nCount = container:getBindItemNum( CONTAINER_TYPE_SUNDRIES, ARM_STAR_ITEM_TOLUA );
			else
				ArmProceStarFrameNeedItemBtnBindTexture:Hide();
				nCount = container:getUnBindItemNum( CONTAINER_TYPE_SUNDRIES, ARM_STAR_ITEM_TOLUA );
			end
			for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
				local t = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
				if t:getItemId() == ARM_STAR_ITEM_TOLUA and t:isItemBind() == IsNeedGemBind then
					LockItem( t:getListType(), t:getGridIdx(), t:getItemId(), "ArmProceStarFrameNeedItemBtn" )
				end
			end
		else
			nCount = needItem:getNum()
			LockItem( needItem:getListType(), needItem:getGridIdx(), needItem:getItemId(), "ArmProceStarFrameNeedItemBtn" )
		end

		if nCount <= 0 then
			ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
		elseif nCount > 1 then
			ArmProceStarFrameNeedItemBtnCountFont:SetText( nCount );
		end
	else
		local btn = getglobal("ArmProceStarFrameNeedItemBtn");
		local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
		local btnTex = getglobal( btn:GetName().."IconTexture" );
		btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		ArmProceStarFrameNeedItemBtnCountFont:SetText("");
	end
	
	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	for i=1,MAX_ITEM_STAR do
		local btn = getglobal("ArmProceStarFrameStar"..i);
		btn:Hide();
	end	
	if nListType < 0 or nGrid < 0 then
		ArmProceStarFrameArmStarFont:Hide();
		return;
	end
	local item = container:getItem( nListType,nGrid );
	ArmProceStarFrameArmItemBtnBindTexture:Hide();
	
	local IsArmExist = false;
	ArmProceStarFramePerfectFont:Hide()
	ArmProceStarFramePerfect:Hide()
	if item:getItemId() <= 0 then
		ClearArmProceStarFrameInfo();
	else
		-- 设置图标，
		IsArmExist = true;
		ArmProceStarFrameArmStarFont:Show();
		local equipItemDef = item:getItemDef();
		local IconPath = GetItemEquipIconPath()..equipItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		for i=1, item:getItemDef().StarLimit do
			local btn = getglobal("ArmProceStarFrameStar"..i);
			btn:Show()
			btn:SetPoint( "topleft", "ArmProceStarFrame", "topleft", 80 + math.floor(200/item:getItemDef().StarLimit) * i, 154 )
		end		
		ArmProceStarFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( equipItemDef, "ArmProceStarFrameArmItemBtnUVAnimationTex",item);
		if item:isItemBind() then
			ArmProceStarFrameArmItemBtnBindTexture:Show();
		else
			ArmProceStarFrameArmItemBtnBindTexture:Hide();
		end		
				
		--星级
		local nStar = item:getStart();
		if nStar < 0 then
			return;
		end
		
		for i = 1,  nStar do
			local btn = getglobal("ArmProceStarFrameStar"..i);
			btn:Enable();
			local tex = getglobal( btn:GetName().."StarTex" );
			tex:SetTexUV( 516,33,13,12 );
		end
		if nStar <= item:getItemDef().StarLimit then
			for i = nStar + 1, item:getItemDef().StarLimit do
				local btn = getglobal("ArmProceStarFrameStar"..i);
				btn:Disable();
				if i == nStar + 1 and i < item:getItemDef().StarLimit + 1 then
					local StarTex = getglobal( "ArmProceStarFrameStar"..i.."StarTex" );
					StarTex:SetTexUV( 625, 622, 13, 12 );
					if bChoseNextStar then
						bChoseNextStar = false;
						nCurStarChose = i;
						for j = 1, MAX_ITEM_STAR do
							local starChoseTex = getglobal("ArmProceStarFrameStar"..j.."ChoseStarTex");
							starChoseTex:Hide();
						end
						local bigStar = getglobal( "ArmProceStarFrameStar"..i.."ChoseStarTex" );
						bigStar:Show();
						local posX = 27 + math.floor(200/item:getItemDef().StarLimit) * i;
						ArmProceStarFrameCurChooseTips:SetPoint( "topleft","ArmProceStarFrame", "topleft", posX, 106 );
						ArmProceStarFrameCurChooseTips:Show();
					end
					SetStarMulinfo();
					SetPerfectInfo();
					btn:Enable();
				end
			end

			if bChoseNextStar then
				local bFound = false;
				for i = 1, item:getStart() do
					local starValIdx = item:getArmAddStarValIdx( i - 1 );
					if not bFound and starValIdx < MAX_STAR_MUL_LIST - 1 then
						bChoseNextStar = false;
						nCurStarChose = i;
						for j = 1, MAX_ITEM_STAR do
							local starChoseTex = getglobal("ArmProceStarFrameStar"..j.."ChoseStarTex");
							starChoseTex:Hide();
						end
						local bigStar = getglobal( "ArmProceStarFrameStar"..i.."ChoseStarTex" );
						bigStar:Show();
						local posX = 27 + math.floor(200/item:getItemDef().StarLimit) * i;
						ArmProceStarFrameCurChooseTips:SetPoint( "topleft","ArmProceStarFrame", "topleft", posX, 106 );
						ArmProceStarFrameCurChooseTips:Show();
						SetStarMulinfo();
						SetPerfectInfo();
						bFound = true;
					end
				end
			end
		end

		for i = 1, item:getStart() do
			local btn = getglobal("ArmProceStarFrameStar"..i);
			local tex = getglobal( btn:GetName().."StarTex" );
			local starValIdx = item:getArmAddStarValIdx( i - 1 );
			if starValIdx >= 0 and starValIdx <= MAX_STAR_MUL_LIST then
				local starLv = item:getStart();
				if starLv <= item:getItemDef().StarLimit then
					local armStarDef = getArmStarDef( starLv );
					local mul = armStarDef.StarInfo[starValIdx].Mul;
					local minMul,maxMul = getArmDefMinAndMaxMul( armStarDef );
					if mul >= maxMul then
						tex:SetTexUV( 530, 33, 13, 12 );
					end	
				end			
			end
		end
		
		if nStar > item:getItemDef().StarLimit then
			ArmProceStarFrameNeedItemFont:SetText("");
			ArmProceStarFrameNeedMoneyFont:SetText( 0 );
			return;
		end

		--材料
		SetArmProceStatNeedInfo()
		-- 能否强化
		if IsArmExist and ArmProceStarFrameCurChooseTips:IsShown() and IsNeedGemExist and not IsAutoArmStar then
			ArmProceStarFrameOKBtn:Enable();
		else
			ArmProceStarFrameOKBtn:Disable();
		end		
		SetStarMulinfo();
		SetPerfectInfo();
	end
end

local PerfectInfoEdge = {
	{ 5, 20, 60, 90, 100, },
	{ 5, 20, 60, 90, 100, },
	{ 5, 20, 60, 90, 100, },
	{ 5, 20, 60, 90, 100, },
	{ 4, 18, 54, 81, 100, },
	{ 4, 18, 54, 81, 100, },
	{ 4, 18, 54, 81, 100, },
	{ 4, 18, 54, 81, 100, },
	{ 4, 18, 54, 81, 100, },
	{ 5, 20, 60, 90, 100, },
	{ 5, 20, 60, 90, 100, },
	{ 5, 20, 60, 90, 100, },
}
--人品值积累
function SetPerfectInfo()
	local armListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local armGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local item = container:getItem( armListType, armGrid );
	if item == nil then
		ArmProceStarFramePerfectFont:Hide()
		ArmProceStarFramePerfect:Hide()
		ArmProceStarFramePerfectEffect:Hide();
		ArmProceStarFramePerfectModelEffect:Hide()
		ArmProceStarPerfectOldVal = -20000
		return
	end
	local starLv = nCurStarChose;
	if starLv <= item:getItemDef().StarLimit and starLv > 0 then
		local armStarDef = getArmStarDef( starLv );
		local strengthenCount = item:getStrengthenCount();
		local perfectNum = armStarDef.PerfectNum;
		local val = strengthenCount/perfectNum * 100
		if ArmProceStarPerfectOldVal ~= strengthenCount and ArmProceStarPerfectOldVal ~= -20000 then
			ArmProceStarFramePerfectEffect:SetUVAnimation( 80, false )
			ArmProceStarFramePerfectEffect:Show();
			ArmProceStarFramePerfectModelEffect:setPetModel(0,0,"effect\\entity\\jiahao.ent");
			ArmProceStarFramePerfectModelEffect:setPetScale(0,1,1,1);
			ArmProceStarFramePerfectModelEffect:setEntityPosition(0,0,20,0);
			ArmProceStarFramePerfectModelEffect:Show()
		end
		--上次为完美星，本次不是完美星，隐藏特效
		if ArmProceStarFramePerfectInfo:GetText() == "已经爆了！" and val < 100 then
			ArmProceStarFramePerfectEffect:Hide()
		end
		ArmProceStarPerfectOldVal = strengthenCount
		--是否是完美星
		local starValIdx = item:getArmAddStarValIdx( starLv - 1 );
		if starValIdx >= 0 and starValIdx <= MAX_STAR_MUL_LIST then
			local mul = armStarDef.StarInfo[starValIdx].Mul;
			local minMul,maxMul = getArmDefMinAndMaxMul( armStarDef );
			if mul >= maxMul then
				ArmProceStarFramePerfectProgress:SetSize( 164, 13 )
				ArmProceStarFramePerfectInfo:SetText( "已是完美" )
				ArmProceStarFramePerfectFont:Hide()
				ArmProceStarFramePerfect:Hide()
				return
			end
		end
		if val <= PerfectInfoEdge[starLv][1] then
			ArmProceStarFramePerfectProgress:SetSize( 164*0.1, 13 )
			ArmProceStarFramePerfectModelEffect:SetPoint( "center", "ArmProceStarFramePerfectTex", "left",10+164*0.1, -63 )
			ArmProceStarFramePerfectInfo:SetText( "一般" )
		elseif val <= PerfectInfoEdge[starLv][2] then
			ArmProceStarFramePerfectProgress:SetSize( 164*0.3, 13 )
			ArmProceStarFramePerfectModelEffect:SetPoint( "center", "ArmProceStarFramePerfectTex", "left",10+164*0.3, -63 )
			ArmProceStarFramePerfectInfo:SetText( "较多" )
		elseif val <= PerfectInfoEdge[starLv][3] then
			ArmProceStarFramePerfectProgress:SetSize( 164*0.6, 13 )
			ArmProceStarFramePerfectModelEffect:SetPoint( "center", "ArmProceStarFramePerfectTex", "left",10+164*0.6, -63 )
			ArmProceStarFramePerfectInfo:SetText( "很多" )
		elseif val <= PerfectInfoEdge[starLv][4] then
			ArmProceStarFramePerfectProgress:SetSize( 164*0.8, 13 )
			ArmProceStarFramePerfectModelEffect:SetPoint( "center", "ArmProceStarFramePerfectTex", "left",10+164*0.8, -63 )
			ArmProceStarFramePerfectInfo:SetText( "非常多" )
		elseif val < PerfectInfoEdge[starLv][5] then
			ArmProceStarFramePerfectProgress:SetSize( 164*0.95, 13 )
			ArmProceStarFramePerfectModelEffect:SetPoint( "center", "ArmProceStarFramePerfectTex", "left",10+164*0.95, -63 )
			ArmProceStarFramePerfectInfo:SetText( "接近满值" )
		elseif val >= PerfectInfoEdge[starLv][5] then
			ArmProceStarFramePerfectProgress:SetSize( 164, 13 )
			ArmProceStarFramePerfectModelEffect:SetPoint( "center", "ArmProceStarFramePerfectTex", "left",174, -63 )
			ArmProceStarFramePerfectInfo:SetText( "已经爆了！" )
			ArmProceStarFramePerfectEffect:SetUVAnimation( 80, true )
			ArmProceStarFramePerfectEffect:Show();
		end
		ArmProceStarFramePerfectFont:Show()
		ArmProceStarFramePerfect:Show()
	end
end

function ArmProceStarFramePerfect_OnEnter()
	if ArmProceStarFramePerfectInfo:GetText() == "已经爆了！" then
		SetGameTooltips( this:GetName(), "人品值已积累满，当前星级必定出现完美强化。" )
	else
		local starLv = nCurStarChose;
		local t_starLvPercent = { 10,10,10,12,12,12,15,15,15,15,15,15 }
		local szInfo = "1.每次强化,基础属性增加在1%~"..t_starLvPercent[starLv].."%中随机，有一定几率达到最大的完美强化；\n2.每次强化未达到最大值时，根据当前星级高低积累祝福的人品值；\n3.人品值积累满时，下次强化必定为完美强化，并重新开始积累人品值。"
		SetGameTooltips( this:GetName(), szInfo )
	end
end

function ArmProceStarButton_onClick()
	--如果在自动强化过程中，则不可选取其它星级
	if IsAutoArmStar then return end
	if not this:IsShown() then return end
	if not bHasSaved then
		MessageBox( "装备强化保存","当前强化星级属性尚未保存，点击确定保存");
		MessageBoxFrame:SetClientString("装备强化保存");
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end;
	local container = MainPlayer:getContainer();
	local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData( 0 )
	local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData( 1 )
	local needItem = container:getItem( nNeedListType, nNeedGridIdx )
	if needItem:getItemId() == ITEM_STAT_UP_FIRST_PERFECT_TOLUA then
		return nCurStarChose ~= this:GetClientID() and ShowMidTips( "此道具只能用于第一颗还未完美强化的星级" ) or nil;
	end
	for i = 1, MAX_ITEM_STAR do
		local starChoseTex = getglobal("ArmProceStarFrameStar"..i.."ChoseStarTex");
		starChoseTex:Hide();
	end
	local bigStar = getglobal( this:GetName().."ChoseStarTex" );
	bigStar:Show();
	
	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	local item = container:getItem( nListType,nGrid );
	
	local posX = 27 + math.floor(200/item:getItemDef().StarLimit) * this:GetClientID();
	ArmProceStarFrameCurChooseTips:SetPoint( "topleft","ArmProceStarFrame", "topleft", posX, 106 );
	ArmProceStarFrameCurChooseTips:Show();
	nCurStarChose = this:GetClientID();
	--材料
	SetArmProceStatNeedInfo()
	SetStarMulinfo();
	SetPerfectInfo();
end

function ArmProceStarResult( succ, starIdx, mulIdx )
	bCanStarUp = true;
	bHasSaved = false;
	if  not succ then return end
	if starIdx == 0 then return end
	nCurStarMulIdx = mulIdx;
	if starIdx > ARM_STAR_LIMIT then return end
	local armStarDef = getArmStarDef( starIdx );
	local mul = armStarDef.StarInfo[mulIdx].Mul;
	local mulMin,mulMax = getArmDefMinAndMaxMul( armStarDef );
	ArmProceStarFrameNextArmStarProceFont:SetTextColor( 255, 255, 255 );
	ArmProceStarFrameNextArmStarAddFont:SetTextColor( 255, 255, 255 );
	ArmProceStarFrameNextArmStarAddFont2:SetTextColor( 150, 40, 255 );
	if mul >= mulMax then
		--ArmProceStarFrameNextArmStarProceFont:SetText("完美强化");
		--ArmProceStarFrameNextArmStarAddFont:SetTextColor( 150, 40, 255 );
		--ArmProceStarFrameNextArmStarProceFont:SetTextColor( 150, 40, 255 );
		bChoseNextStar = true;
		ArmProceStarFrameCancleBtn_onClick();
		ArmProceStarFrameSaveEffect:setPetModel( 0,0, "effect\\entity\\fumo01.ent" )
		ArmProceStarFrameSaveEffect:setPetScale( 0,0.3,0.3,0.3 )
		ArmProceStarFrameSaveEffect:setEntityPosition( 0,0,50,0 )
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		local container = mainplayer:getContainer();
		--强化	
		local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
		local nGridIdx	= ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
		local item = container:getItem( nListType,nGridIdx );
		local color = RGBColorToRichColor( GetItemNameColor( item:getItemDef() ) );
		MessageBoxOK( "完美保存提示", "恭喜您的" .. color .."【" .. item:getDisplayName() .. "】#n\n第#cffff00" .. starIdx .. "#n星达成完美！（已自动保存）" );
		MessageBoxFrame:SetClientString("完美保存提示");
		MessageBoxFrame:SetPoint( "topright", "UIClient", "topright", -236, 360 );
		--设置自动强化标记
		IsAutoArmStar = false
		return;
	else
		ArmProceStarFrameNextArmStarProceFont:SetText("普通强化");
	end
	ArmProceStarFrameNextArmStarAddFont:SetText(mul.."%");
	ArmProceStarFrameNextArmStarAddFont2:SetText("( "..mulMin.."%-"..mulMax.."% )");
	
	ArmProceStarFrameCancleBtn:Show();
	ArmProceStarFrameSaveBtn:Show();
	ArmProceStarFrameSaveBkgTex:Show();
	ArmProceStarFrameArrowTex:Show();
	
	local CurMul = 0
	--如果是第一次强化这颗星，则视原始属性为0
	if ArmProceStarFrameCurArmStarAddFont:GetText() == "" then
		CurMul = 0
	else
		local curMulEndPos = string.find( ArmProceStarFrameCurArmStarAddFont:GetText(), "%%", 1 );
		local curMulString = string.sub( ArmProceStarFrameCurArmStarAddFont:GetText(), 1, curMulEndPos - 1 );
		CurMul = tonumber( curMulString );
	end	
	if CurMul > mul then
		ArmProceStarFrameAddStarUp:Hide();
		ArmProceStarFrameAddStarEqual:Hide();
		ArmProceStarFrameAddStarDown:Show();
	elseif CurMul == mul then
		ArmProceStarFrameAddStarUp:Hide();
		ArmProceStarFrameAddStarEqual:Show();
		ArmProceStarFrameAddStarDown:Hide();
	elseif CurMul < mul then
		ArmProceStarFrameAddStarUp:Show();
		ArmProceStarFrameAddStarEqual:Hide();
		ArmProceStarFrameAddStarDown:Hide();
	end
end

function getArmDefMinAndMaxMul( armStarDef )
	local mul = armStarDef.StarInfo[0].Mul;
	local minMul = mul;
	for i = 1, MAX_STAR_MUL_LIST do
		if armStarDef.StarInfo[i-1].Mul < minMul then
			minMul = armStarDef.StarInfo[i-1].Mul;
		end
	end
	local maxMul = armStarDef.StarInfo[MAX_STAR_MUL_LIST-1].Mul;
	return minMul, maxMul;
end

function ArmProceStarFrameSaveBtn_onClick()
	if bHasSaved then return end
	if MessageBoxFrame:IsShown() and
	( MessageBoxFrame:GetClientString() == "装备强化保存" or MessageBoxFrame:GetClientString() == "完美强化提示" ) then
		MessageBoxFrame:Hide();
	end
	bHasSaved = true;
	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGridIdx	= ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	bChoseNextStar = true;
	GameArmProce:armAddSave( nListType, nGridIdx, nCurStarChose, nCurStarMulIdx );
	ArmProceStarFrameCurArmStarProceFont:SetTextColor( 255, 255, 255 );
	if ArmProceStarFrameNextArmStarProceFont:GetText() == "完美强化" then
		ArmProceStarFrameCurArmStarAddFont:SetTextColor( 150, 40, 255 );
		ArmProceStarFrameCurArmStarProceFont:SetTextColor( 150, 40, 255 );
	end
	ArmProceStarFrameCurArmStarProceFont:SetText( ArmProceStarFrameNextArmStarProceFont:GetText() );
	ArmProceStarFrameNextArmStarProceFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont:SetText( ArmProceStarFrameNextArmStarAddFont:GetText() );
	ArmProceStarFrameNextArmStarAddFont:SetText("");
	ArmProceStarFrameAddStarDown:Hide();
	ArmProceStarFrameAddStarEqual:Hide();
	ArmProceStarFrameAddStarUp:Hide();
	ArmProceStarFrameSaveBtn:Hide();
	ArmProceStarFrameCancleBtn:Hide();
	ArmProceStarFrameSaveBkgTex:Hide();
	ArmProceStarFrameArrowTex:Hide();
	ArmProceStarFrameCurArmStarAddFont2:SetText(ArmProceStarFrameNextArmStarAddFont:GetText());
	ArmProceStarFrameNextArmStarAddFont2:SetText("");
end

function ArmProceStarAutoSave()
	if bHasSaved then return end
	if MessageBoxFrame:IsShown() and
	( MessageBoxFrame:GetClientString() == "装备强化保存" or MessageBoxFrame:GetClientString() == "完美强化提示" ) then
		MessageBoxFrame:Hide();
	end
	bHasSaved = true;
	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGridIdx	= ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	GameArmProce:armAddSave( nListType, nGridIdx, nCurStarChose, nCurStarMulIdx );
	ArmProceStarFrameCurArmStarProceFont:SetTextColor( 255, 255, 255 );
	if ArmProceStarFrameNextArmStarProceFont:GetText() == "完美强化" then
		ArmProceStarFrameCurArmStarAddFont:SetTextColor( 150, 40, 255 );
		ArmProceStarFrameCurArmStarProceFont:SetTextColor( 150, 40, 255 );
	end
	ArmProceStarFrameCurArmStarProceFont:SetText( ArmProceStarFrameNextArmStarProceFont:GetText() );
	ArmProceStarFrameNextArmStarProceFont:SetText("");
	ArmProceStarFrameCurArmStarAddFont:SetText( ArmProceStarFrameNextArmStarAddFont:GetText() );
	ArmProceStarFrameNextArmStarAddFont:SetText("");
	ArmProceStarFrameAddStarDown:Hide();
	ArmProceStarFrameAddStarEqual:Hide();
	ArmProceStarFrameAddStarUp:Hide();
	ArmProceStarFrameSaveBtn:Hide();
	ArmProceStarFrameCancleBtn:Hide();
	ArmProceStarFrameSaveBkgTex:Hide();
	ArmProceStarFrameArrowTex:Hide();
	ArmProceStarFrameCurArmStarAddFont2:SetText(ArmProceStarFrameNextArmStarAddFont:GetText());
	ArmProceStarFrameNextArmStarAddFont2:SetText("");
end

function CanAddStar()
	local nList = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	if nList < 0 or nGrid < 0 then
		ShowMidTips("请放入装备");
		return false;
	end
	return true;
end

function ArmProceStarFrameCancleBtn_onClick()
	bHasSaved = true;
	ArmProceStarFrameNextArmStarProceFont:SetText("");
	ArmProceStarFrameNextArmStarAddFont:SetText("");
	ArmProceStarFrameAddStarUp:Hide();
	ArmProceStarFrameAddStarEqual:Hide();
	ArmProceStarFrameAddStarDown:Hide();
	ArmProceStarFrameCancleBtn:Hide();
	ArmProceStarFrameSaveBtn:Hide();
	ArmProceStarFrameSaveBkgTex:Hide();
	ArmProceStarFrameArrowTex:Hide();
	ArmProceStarFrameNextArmStarAddFont2:SetText("");
end

function RequestArmStarAdd()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	--强化	
	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGridIdx	= ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	local item = container:getItem( nListType,nGridIdx );
	local bind = 0;
	if IsNeedGemBind then
		bind = 1;
	end
	local must = 0
	local armStarDef = getArmStarDef( nCurStarChose )
	local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData( 0 );
	local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData( 1 );
	local needItem = container:getItem( nNeedListType, nNeedGridIdx )
	if needItem:getItemId() == ITEM_STAT_UP_FIRST_PERFECT_TOLUA then
		must = 2;
	elseif armStarDef.MustItemID == 0 then
		must = 0
	else
		if armStarDef.MustItemID == needItem:getItemId() then
			must = 1
		end
	end
	GameArmProce:armAddStar( nListType, nGridIdx, nCurStarChose, bind, must );
	bCanStarUp = false;
end

function ArmProceStarFrameOKBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end
	if IsArmProceStarMessageBoxShown() then
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end
	if not CanAddStar() then return end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local nListType = ArmProceStarFrameArmItemBtn:GetClientUserData( 0 );
	local nGridIdx	= ArmProceStarFrameArmItemBtn:GetClientUserData( 1 );
	local item = container:getItem( nListType,nGridIdx );
	if item == nil then return end

	if ArmProceStarFrameNextArmStarProceFont:GetText() == "完美强化" then
		--MessageBox( "完美强化提示", "#cfffebd当前获得属性为#c972df9完美强化#n#cfffebd，点击确定保存" );
		--MessageBoxFrame:SetClientString("完美强化提示");
	elseif ArmProceStarFrameCurArmStarProceFont:GetText() == "完美强化" then
		ShowMidTips("当前星级已是完美强化，无需再进行强化");
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end
	if item:isArmFengYin() then
		ShowMidTips("封印装备无法进行此操作");
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end
	
	if ArmProceStarFrameAddStarUp:IsShown() then
		ArmProceStarFrameSaveEffect:setPetModel( 0,0, "effect\\entity\\fumo01.ent" )
		ArmProceStarFrameSaveEffect:setPetScale( 0,0.3,0.3,0.3 )
		ArmProceStarFrameSaveEffect:setEntityPosition( 0,0,50,0 )
		ArmProceStarAutoSave()
		ShowMidTips( "当前获得属性比原属性高，已自动为您保存" )
	end

	--星辉宝钻
	ClearItemLock( "ArmProceStarFrameNeedItemBtn" )
	local nCount = 0
	local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData(0)
	local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData(1)
	local needItem = container:getItem( nNeedListType, nNeedGridIdx )
	--这里改下，如果取不到，可能是当前格位被消耗掉了，查找下背包里是否还有相同id的物品
	if IsNeedGemBind then
		nCount = container:getBindItemNumNotCheckLock( CONTAINER_TYPE_SUNDRIES, ArmProceStarFrameNeedItemBtn:GetClientUserData(2) );
	else
		nCount = container:getUnBindItemNumNotCheckLock( CONTAINER_TYPE_SUNDRIES, ArmProceStarFrameNeedItemBtn:GetClientUserData(2) );
	end
	if nCount <= 0 then
		if IsNeedGemExist and ArmProceStarFrameNeedItemBtn:GetClientUserData(2) == ARM_STAR_ITEM_TOLUA then
			MessageBoxForItem("提示",getItemDef(ARM_STAR_ITEM_TOLUA).Name);
			MessageBoxForItemFrame:SetClientUserData(0,ARM_STAR_ITEM_TOLUA);
			--同时将自动强化停止
			IsAutoArmStar = false
		end
		ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
		ArmProceStarFrameOKBtn:Disable()
		return
	else
		if needItem:getItemId() == 0 then
			needItem = container:getItem( ArmProceStarFrameNeedItemBtn:GetClientUserData(2) )
		end
	end
	if needItem:getItemId() == 0 then
		ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
		ArmProceStarFrameOKBtn:Disable()
		--同时将自动强化停止
		IsAutoArmStar = false
		return
	end
	if needItem:getItemId() == ARM_STAR_ITEM_TOLUA then
		local nNeedCount = GameArmProce:getStarNeedStuffNum( nCurStarChose );
		if IsNeedGemBind then
			nCount = container:getBindItemNum( CONTAINER_TYPE_SUNDRIES, ARM_STAR_ITEM_TOLUA );
		else
			nCount = container:getUnBindItemNum( CONTAINER_TYPE_SUNDRIES, ARM_STAR_ITEM_TOLUA );
		end
		if not IsNeedGemExist or nNeedCount > nCount then
			ArmProceFrameOnRightClick("ArmProceStarFrameNeedItemBtn", nil, nil, 0);
			ArmProceStarFrameOKBtn:Disable()
			MessageBoxForItem("提示",getItemDef(ARM_STAR_ITEM_TOLUA).Name);
			MessageBoxForItemFrame:SetClientUserData(0,ARM_STAR_ITEM_TOLUA);
			--同时将自动强化停止
			IsAutoArmStar = false
			return;
		end

		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local t = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
			if t:getItemId() == ARM_STAR_ITEM_TOLUA and t:isItemBind() == IsNeedGemBind then
				LockItem( t:getListType(), t:getGridIdx(), t:getItemId(), "ArmProceStarFrameNeedItemBtn" )
			end
		end
	elseif needItem:getItemId() ~= ITEM_STAT_UP_FIRST_PERFECT_TOLUA then
		local armStarDef = getArmStarDef( nCurStarChose )
		if armStarDef ~= nil then
			if armStarDef.MustItemID ~= needItem:getItemId() then
				ShowMidTips( "该型号星辉宝钻不适用于当前星级" )
				--同时将自动强化停止
				IsAutoArmStar = false
				return
			end
		else
			--同时将自动强化停止
			IsAutoArmStar = false
			return
		end
	end
	
	if not bCanStarUp then return end
	-- 费用
	local nMoney = getPlayerMoney( mainplayer );
	local nNeedMoney = nCurStarChose*10
	if nNeedMoney > nMoney then
		ShowMidTips("绑银不足，无法强化");
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end

	if checkNeedWarnCost( nNeedMoney ) then
		CostMoneyConfirmFrame:SetClientString("装备强化");
		CostMoneyConfirmFrame:SetClientUserData( 0, IsNeedGemBind and 1 or 0 );
		CostMoneyConfirmFrame:SetClientUserData( 1, item:isItemBind() and 1 or 0 );
		SetWarnFrame( "ArmProceStarFrame" );
		CostMoneyConfirmFrame:Show();
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end

	if IsNeedGemBind and not item:isItemBind() then
		MessageBox( "装备强化绑定提示", "继续强化则当前装备会被绑定，是否继续强化？");
		MessageBoxFrame:SetClientString("装备强化绑定提示");
		--同时将自动强化停止
		IsAutoArmStar = false
		return;
	end
	RequestArmStarAdd();
end

function ArmProceStarFrameNeedItemBtn_OnEnter()
	if not IsNeedGemExist then 
		SetGameTooltips( this:GetName(), "请放入星辉宝钻" );
		return;
	end
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local needGemItem = nil;
	local nNeedListType = ArmProceStarFrameNeedItemBtn:GetClientUserData( 0 )
	local nNeedGridIdx = ArmProceStarFrameNeedItemBtn:GetClientUserData( 1 )
	local needGemItem = container:getItem( nNeedListType, nNeedGridIdx )
	if needGemItem == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, needGemItem );
end

--新加的自动强化部分
function ArmProceStarFrameAutoBtn_OnClick()
	if not CanAddStar() then return end
	if not IsNeedGemExist then
		ShowMidTips( "请放入星辉宝钻" )
		return
	end
	IsAutoArmStar = true
	ArmProceStarFrameOKBtn_OnClick()
end
function ArmProceStarFrameAutoBtn_OnUpdate()
	if IsAutoArmStar then
		ArmProceStarFrameOKBtn:Disable()
		ArmProceStarFrameAutoBtn:Hide()
	else
		ArmProceStarFrameAutoBtn:Show()
	end
end
function ArmProceStarFrameStopAutoBtn_OnClick()
	IsAutoArmStar = false
	UpdateArmProceStarFrame()
end
function ArmProceStarFrameStopAutoBtn_OnUpdate()
	if IsAutoArmStar then
		ArmProceStarFrameOKBtn_OnClick()
		ArmProceStarFrameOKBtn:Disable()
		ArmProceStarFrameAutoBtn:Hide()
	else
		ArmProceStarFrameAutoBtn:Show()
	end
end
--------------		装备绑定	--------------------------
function ArmProceBindFrame_OnLoad()
	ArmProceBindFrameDescRich:resizeRect( 260,90 );
	ArmProceBindFrameDescRich:SetText( "☆\t装备绑定操作会将不绑定装备变为绑定的，绑定后的装备不可交易，也不会从你身上掉落。\n",170,160,120 );
	ArmProceBindFrameDescRich:AddText( "☆\t装备绑定操作会消耗一定的绑银。",170,160,120 );	

	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	this:RegisterEvent( "GE_ARMBIND_SUCESS" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" )
	ItemFirstCallBack( "ArmProceBindFrame" , Save2ArmBind );
end

function Save2ArmBind( item, nSrcList, nSrcGridIndex )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入装备");
		return;
	end
	ReceiveArmProceBind( "ArmProceBindFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
end

function ArmProceBindFrame_OnShow()
	ArmProceBindFrameArmItemBtn:SetClientUserData( 0,-1 );
	ArmProceBindFrameArmItemBtn:SetClientUserData( 1,-1 );
	ArmProceBindFrameArmItemBtnBackTexture:Show();
	ArmProceBindFrameArmItemBtnUVAnimationTex:Hide();
	ArmProceBindFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	ArmProceBindFrameCurrMoneyFont:SetText( getPlayerMoney( mainplayer ) );
	ArmProceBindFrameNeedMoneyFont:SetText( 0 );
	ArmProceBindFrameOKBtn:Disable();
	ArmProceBindFrameArmItemBtnEffectTex:Hide();
	ArmProceBindFrameArmItemBtnBindTexture:Hide();
end

function ArmProceBindFrame_OnHide()
	local nListType	= ArmProceBindFrameArmItemBtn:GetClientUserData( 0 ) ;
	local nGrid		= ArmProceBindFrameArmItemBtn:GetClientUserData( 1 );
	if nListType >= 0 and nGrid >= 0 then
		UnLockItem( nListType,nGrid,0 );
	end
	ArmProceBindFrameArmItemBtnEffectTex:StopUVAnim();
	ArmProceBindFrameArmItemBtnBindTexture:Hide();
end

function ArmProceBindFrame_OnEvent()
	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG"  or arg1 == "GE_CONTAINER_UPDATE" then
		if ArmProceBindFrame:IsShown() then
			UpdateArmProceBindFrame();
		end
	end
	if arg1 == "GE_ARMBIND_SUCESS" then
		ArmProceBindFrameArmItemBtnEffectTex:SetUVAnimation( 100,false );
	end
end

function ArmProceBindStartDrag( szBtnName )
	local btn		= getglobal( szBtnName );
	local nListType	= btn:GetClientUserData( 0 );
	local nGrid		= btn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARMBIND;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),item:getNum() );
		return;
	end
end

function ReceiveArmProceBind( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn = getglobal( szBtnName );
	-- 如果目标来自背包
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local srcList		= dragfrom;						-- 源容器类型
		local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
		local item			= container:getItem( srcList, srcGrid );
		if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
			ShowMidTips("请放入装备");
			return;
		end
		if item:isItemBind() then
			ShowMidTips("该物品已绑定，请放入未绑定物品");
			return;
		end
		local itemdef		= item:getItemDef();
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		
		local srcitem = container:getItem( nListType,nGridIdx );			
		local nItemId	= srcitem:getItemId();
		if nItemId > 0 then
			UnLockItem( nListType, nGridIdx, nItemId );--解锁
		end
		btn:SetClientUserData( 0,srcList );
		btn:SetClientUserData( 1,srcGrid );
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		UpdateArmProceBindFrame();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end

	if dragfrom == DRAG_ARMBIND then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;	 
		return;
	end
end

function UpdateArmProceBindFrame()
	local nListType = ArmProceBindFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceBindFrameArmItemBtn:GetClientUserData( 1 );
	ArmProceBindFrameArmItemBtnBindTexture:Hide();
	if nListType < 0 or nGrid < 0 then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		local itemDef = item:getItemDef();		
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmProceBindFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, "ArmProceBindFrameArmItemBtnUVAnimationTex",item);
	end
	if item:isItemBind() then
		ArmProceBindFrameArmItemBtnBindTexture:Show();
	else
		ArmProceBindFrameArmItemBtnBindTexture:Hide();
	end
	local nStar = item:getStart();
	if nStar < 0 then
		return;
	end
	local nMoney = getPlayerMoney( mainplayer );
	local nNeedMoney = GameArmProce:getBindNeedMoney( math.floor(item:getItemDef().WearPre.RoleLevelMin/10)+1 );
	if nNeedMoney >= 0 then	
		local equipItemDef = item:getItemDef();
		if equipItemDef.Important == 0 then
			nNeedMoney = nNeedMoney*80/100;
		elseif equipItemDef.Important == 1 then
			nNeedMoney = nNeedMoney*100/100;
		elseif equipItemDef.Important == 2 then
			nNeedMoney = nNeedMoney*150/100;
		else
			nNeedMoney = nNeedMoney*300/100;
		end	
		ArmProceBindFrameNeedMoneyFont:SetText( math.floor(nNeedMoney) );
	end
	ArmProceBindFrameCurrMoneyFont:SetText( nMoney );
	if nMoney >= nNeedMoney then
		ArmProceBindFrameOKBtn:Enable();
	else
		ArmProceBindFrameOKBtn:Disable();
	end
end

function ArmProceBindFrameOnBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local nListType = ArmProceBindFrameArmItemBtn:GetClientUserData( 0 );
	local nGridIdx	= ArmProceBindFrameArmItemBtn:GetClientUserData( 1 );

	local money = tonumber( ArmProceBindFrameNeedMoneyFont:GetText() );
	if money ~= nil then
		local mainplayer = ActorMgr:getMainPlayer();
		if getPlayerMoney( mainplayer ) < money then
			ShowMidTips( "绑银不足，无法绑定" );
			return;
		end

		if checkNeedWarnCost( money ) then
			CostMoneyConfirmFrame:SetClientString("装备绑定");
			CostMoneyConfirmFrame:SetClientUserData( 0, nListType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nGridIdx );
			SetWarnFrame( "ArmProceBindFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
	end
	GameArmProce:bindArm( nListType,nGridIdx );
end

------------------		装备解绑	--------------------------
function ArmProceUnBindFrame_OnLoad()
	ArmProceUnBindFrame:SetClientUserData( 0,0 );
	ArmProceUnBindFrame:SetClientUserData( 1,0 );
	ArmProceUnBindFrame:SetClientUserData( 2,0 );

	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	this:RegisterEvent( "GE_ARMUNBIND_SUCESS" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );

	ItemFirstCallBack( "ArmProceUnBindFrame" , Save2ArmUnBind );
end

function Save2ArmUnBind( item, nSrcList, nSrcGridIndex )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入装备");
		return;
	end
	if ArmProceUnBindFrameOKBtn:GetText() == "关 闭" then
		return
	end
	ReceiveArmProceUnBind( "ArmProceUnBindFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
end

local t_ItemLv = {
					[ARM_UNBIND_ITEM_LOW_TOLUA] = "60级以下",
					[ARM_UNBIND_ITEM_MID_TOLUA] = "90级以下",
					[ARM_UNBIND_ITEM_HIGH_TOLUA] = "所有",
				}
function ArmProceUnBindFrame_OnShow()
	local itemId = ArmProceUnBindFrame:GetClientUserData( 0 );
	if itemId == 0 then
		--说明是npc 打开
		ArmProceUnBindFrameDescRich:SetText( "☆\t装备解绑操作会将绑定装备变为不绑定的，解绑后的装备可以交易给其他人，同时也有可能因为PK而掉落这件装备。",170,160,120 );
		ArmProceUnBindFrameDescRich:AddText( "\n",170,160,120 );
		ArmProceUnBindFrameDescRich:AddText( "☆\t装备解绑操作会消耗一定的金币。",170,160,120 );

		ArmProceUnBindFrame:SetSize( 284,372 );
		ArmProceUnBindFrameNeedItemNameFont:SetText("");
		ArmProceUnBindFrameMoneyBtn1:Show();
		ArmProceUnBindFrameMoneyBtn2:Show();
		ArmProceUnBindFrameNeedMoneyBkg:Show();
		ArmProceUnBindFrameNeedMoneyTitle:Show();
		ArmProceUnBindFrameCurrMoneyBkg:Show();
		ArmProceUnBindFrameCurrMoneyTitle:Show();
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end;
		ArmProceUnBindFrameCurrMoneyFont:SetText( mainplayer:getGodCoin() );
		ArmProceUnBindFrameNeedMoneyFont:SetText( 0 );
		ArmProceUnBindFrameCurrMoneyFont:Show();
		ArmProceUnBindFrameNeedMoneyFont:Show();
	else
		--说明是右键道具打开
		ArmProceUnBindFrameDescRich:SetText( "☆\t装备解绑操作会将绑定装备变为不绑定的，解绑后的装备可以交易给其他人，同时也有可能因为PK而掉落这件装备。",170,160,120 );
		--ArmProceUnBindFrameDescRich:AddText( "☆\t装备解绑操作会消耗一定的金券。",170,160,120 );		
		local itemDef = getItemDef( itemId );
		if itemDef ~= nil then
			ArmProceUnBindFrameDescRich:AddText( "☆\t"..itemDef.Name .."能解绑".. t_ItemLv[itemId] .."的装备。",170,160,120 );
		end

		ArmProceUnBindFrame:SetSize( 284,310 );
		ArmProceUnBindFrameMoneyBtn1:Hide();
		ArmProceUnBindFrameMoneyBtn2:Hide();
		ArmProceUnBindFrameNeedMoneyBkg:Hide();
		ArmProceUnBindFrameNeedMoneyTitle:Hide();
		ArmProceUnBindFrameCurrMoneyBkg:Hide();
		ArmProceUnBindFrameCurrMoneyTitle:Hide();
		ArmProceUnBindFrameCurrMoneyFont:Hide();
		ArmProceUnBindFrameNeedMoneyFont:Hide();
	end
	ArmProceUnBindFrameArmItemBtn:SetClientUserData( 0,-1 );
	ArmProceUnBindFrameArmItemBtn:SetClientUserData( 1,-1 );

	ArmProceUnBindFrameArmItemBtnBackTexture:Show();
	ArmProceUnBindFrameArmItemBtnUVAnimationTex:Hide();
	ArmProceUnBindFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");

	ArmProceUnBindFrameOKBtn:SetText( "开始解绑" );
	ArmProceUnBindFrameOKBtn:Disable();
	ArmProceUnBindFrameArmItemBtnEffectTex:Hide();
end

function ArmProceUnBindFrame_OnEvent()
	if ( arg1 == "UI_ACTOR_ATTRIBUTE_CHG" or arg1== "GE_CONTAINER_UPDATE" ) and ArmProceUnBindFrame:IsShown() then
		UpdateArmProceUnBindFrame();		
	end
	if arg1 == "GE_ARMUNBIND_SUCESS" then
		ArmProceUnBindFrameArmItemBtnEffectTex:SetUVAnimation( 100,false );
		ArmProceUnBindFrameOKBtn:SetText( "关 闭" );
	end
end

function ArmProceUnBindFrame_OnHide()
	local nListType	= ArmProceUnBindFrameArmItemBtn:GetClientUserData( 0 ) ;
	local nGrid		= ArmProceUnBindFrameArmItemBtn:GetClientUserData( 1 );
	if nListType >= 0 and nGrid >= 0 then
		UnLockItem( nListType,nGrid,0 );
	end
	local itemId = this:GetClientUserData( 0 );
	if itemId > 0 then
		local nListType = ArmProceUnBindFrame:GetClientUserData( 1 );
		local nGridIdx = ArmProceUnBindFrame:GetClientUserData( 2 );
		UnLockItem( nListType,nGridIdx,itemId );
	end
	ArmProceUnBindFrame:SetClientUserData( 0,0 );
	ArmProceUnBindFrameArmItemBtnEffectTex:StopUVAnim();
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "装备解绑" then
		MessageBoxFrame:Hide();
	end
	ArmProceUnBindFrameArmItemBtnBindTexture:Hide();
end

function ArmProceUnBindStartDrag( szBtnName )
	if ArmProceUnBindFrameOKBtn:GetText() == "关 闭" then
		return;
	end
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "装备解绑" then
		return;
	end
	local btn		= getglobal( szBtnName );
	local nListType	= btn:GetClientUserData( 0 ) ;
	local nGrid		= btn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARMUNBIND;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),item:getNum() );
		return;
	end
end

local t_StuffItemLv = {
						[ARM_UNBIND_ITEM_LOW_TOLUA] = {0,60},
						[ARM_UNBIND_ITEM_MID_TOLUA] = {61,90},
						[ARM_UNBIND_ITEM_HIGH_TOLUA] = {91,100000},
						}
function ReceiveArmProceUnBind( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if ArmProceUnBindFrameOKBtn:GetText() == "关 闭" then
		return;
	end
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "装备解绑" then
		return;
	end
	local btn = getglobal( szBtnName );
	-- 如果目标来自背包
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local srcList		= dragfrom;						-- 源容器类型
		local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
		local item			= container:getItem( srcList, srcGrid );
		if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
			ShowMidTips("请放入装备");
			return;
		end
		if not item:isItemBind() then
			ShowMidTips("该物品目前未绑定，请放入绑定物品");
			return;
		end
		local itemDef = item:getItemDef();
		local itemId = ArmProceUnBindFrame:GetClientUserData( 0 );
		if itemId > 0 then
			if itemDef.WearPre.RoleLevelMin > t_StuffItemLv[itemId][2] then
				ShowMidTips("装备的等级不符");
				return;
			end
		end

		local itemdef		= item:getItemDef();
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		
		local srcitem = container:getItem( nListType,nGridIdx );			
		local nItemId	= srcitem:getItemId();
		if nItemId > 0 then
			UnLockItem( nListType, nGridIdx, nItemId );--解锁
		end
		btn:SetClientUserData( 0,srcList );
		btn:SetClientUserData( 1,srcGrid );
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		UpdateArmProceUnBindFrame();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end

	if dragfrom == DRAG_ARMUNBIND then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;	 
		return;
	end
end

function UpdateArmProceUnBindFrame()
	local nItemId = ArmProceUnBindFrame:GetClientUserData( 0 );
	local nListType = ArmProceUnBindFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceUnBindFrameArmItemBtn:GetClientUserData( 1 );
	if nListType < 0 or nGrid < 0 then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		local itemDef = item:getItemDef();
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmProceUnBindFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, "ArmProceUnBindFrameArmItemBtnUVAnimationTex",item);
		if item:isItemBind() then
			ArmProceUnBindFrameArmItemBtnBindTexture:Show();
		else
			ArmProceUnBindFrameArmItemBtnBindTexture:Hide();
		end
	end
	if nItemId > 0 then
		local stuffItemDef = getItemDef( nItemId );
		if stuffItemDef ~= nil then
			ArmProceUnBindFrameNeedItemNameFont:SetText( stuffItemDef.Name );
			local t_NameColor = GetItemNameColor( stuffItemDef );
			ArmProceUnBindFrameNeedItemNameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
		end
		ArmProceUnBindFrameOKBtn:Enable();
	else
		--钱
		local nBindGodCoin = mainplayer:getGodCoin();
		local nNeedGodCoin = GameArmProce:getUnBindNeedGold( math.floor(item:getItemDef().WearPre.RoleLevelMin/10)+1 );
		if nNeedGodCoin >= 0 then
		--[[
			local equipItemDef = item:getItemDef();
			if equipItemDef.Important == 0 then
				nNeedGodCoin = nNeedGodCoin*80/100;
			elseif equipItemDef.Important == 1 then
				nNeedGodCoin = nNeedGodCoin*100/100;
			elseif equipItemDef.Important == 2 then
				nNeedGodCoin = nNeedGodCoin*150/100;
			elseif equipItemDef.Important == 3 then
				nNeedGodCoin = nNeedGodCoin*300/100;
			end
			]]--
			ArmProceUnBindFrameNeedMoneyFont:SetText( nNeedGodCoin );
			ArmProceUnBindFrameCurrMoneyFont:SetText( nBindGodCoin );
		end
		if nBindGodCoin >= nNeedGodCoin then
			ArmProceUnBindFrameOKBtn:Enable();
		else
			ArmProceUnBindFrameOKBtn:Disable();
		end
	 end
end

function ArmProceUnBindFrameOkBtn_OnClick()
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "装备解绑" then
		return;
	end
	if this:GetText() == "关 闭" then
		ArmProceUnBindFrame:Hide();
	else
		local nItemId = ArmProceUnBindFrame:GetClientUserData( 0 );
		local nListType = ArmProceUnBindFrameArmItemBtn:GetClientUserData( 0 );
		local nGrid = ArmProceUnBindFrameArmItemBtn:GetClientUserData( 1 );
		--[[
		if nItemId == 0 then
			GameArmProce:unBindArm( nListType,nGrid,0,0,0 );
		else
			GameArmProce:unBindArm( nListType,nGrid,nItemId,ArmProceUnBindFrame:GetClientUserData(1),ArmProceUnBindFrame:GetClientUserData(2) );
		end
		]]--
		MessageBox("装备解绑","您确认解绑这个装备吗？");
		MessageBoxFrame:SetClientUserData( 0,nListType );
		MessageBoxFrame:SetClientUserData( 1,nGrid );
		MessageBoxFrame:SetClientUserData( 2,nItemId );
	end
end
------------------		品质转移		--------------------------
local IsStuffItemExist = false;
local IsStuffItemBind = false;
function ArmProceStarMoveFrameOnRightClick( szBtnName, desList, desGrid  )
	local btn	= getglobal( szBtnName );
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local container = MainPlayer:getContainer();
	if btn:GetClientUserData(0) < 0 or btn:GetClientUserData(1) < 0 then
		return
	end

	local item = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
	if item:getItemId() ~= 0 and szBtnName ~= "ArmProceStarMoveFrameStuffItemBtn" then
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );
	end
	btn:SetClientUserData(0,-1);
	btn:SetClientUserData(1,-1);
	local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex");
	UVAnimationTex:Hide();
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	if szBtnName == "ArmProceStarMoveFrameStuffItemBtn" then
		IsStuffItemExist = false;
		IsStuffItemBind = false;
		ClearItemLock( "ArmProceStarMoveFrameStuffItemBtn" )
	else
		if desList ~= nil and desGrid ~= nil then
			if item ~= nil and item:getGridIdx() ~= desGrid then
				container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
			end
		end
	end
	local bindTex = getglobal( btn:GetName().."BindTexture" );
	bindTex:Hide();
	ArmProceStarMoveFrameSrcStarFont:SetText("");
	ArmProceStarMoveFrameDestStarFont:SetText("");
	UpdateArmProceStarMoveFrame();
end

function ArnProceStarMoveFrameOnItemEnter( szBtnName )
	local btn = getglobal( szBtnName );
	if szBtnName == "ArmProceStarMoveFrameStuffItemBtn" then
		local MainPlayer = ActorMgr:getMainPlayer();
		local container = MainPlayer:getContainer();
		local stuffItem = nil;
		for i = 1, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item = container:getItem( CONTAINER_TYPE_SUNDRIES, i - 1 );
			if item:getItemId() == ARM_STAR_ITEM_TOLUA then
				if item:isItemBind() == IsStuffItemBind then
					stuffItem = item;
					break;
				end
			end
		end
		if stuffItem ~= nil then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, stuffItem );
		end
	else
		local nList = btn:GetClientUserData( 0 );
		local nGrid = btn:GetClientUserData( 1 );
		if nList >= 0 and nGrid >= 0 then
			local mainplayer = ActorMgr:getMainPlayer();
			local container = mainplayer:getContainer();
			local item = container:getItem( nList,nGrid );
			if item:getItemId() > 0 then
				ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
			end
		else
			if szBtnName == "ArmProceStarMoveFrameSrcItemBtn" then
				SetGameTooltips( szBtnName, "请放入要移除星的装备" );
			else
				SetGameTooltips( szBtnName, "请放入要得到星的装备" );
			end
		end
	end
end

function Save2ArmStarMove( item, nSrcList, nSrcGridIndex )
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入装备或者星辉宝钻");
		return;
	end
	local szBtnName = "";
	if item:getItemId() == ARM_STAR_ITEM_TOLUA then
		szBtnName = "ArmProceStarMoveFrameStuffItemBtn";
	elseif item:getItemType() == ITEM_SLOTTYPE_AMM then
		szBtnName = "ArmProceStarMoveFrameSrcItemBtn";
		if ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 0 ) > 0 then
			szBtnName = "ArmProceStarMoveFrameDestItemBtn";
		end
	end
	ReceiveArmProceStarMove( szBtnName, item:getListType(), item:getGridIdx(), 0, 0 );
end


function ArmProceStarMoveFrame_OnLoad()
	ArmProceStarMoveFrameDescRich:resizeRect( 240,90 );
	local szText = [[☆	转星装备不限制目标装备等级
☆	转星装备与目标装备必须为相同部件
☆	转星成功后目标装备将会绑定，转移           过程中不会损失星级与完美度]];
	ArmProceStarMoveFrameDescRich:SetText( szText ,170,160,120);

	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	this:RegisterEvent( "GE_ARMMOVESTAR_SUCESS" );

	ItemFirstCallBack( "ArmProceStarMoveFrame" , Save2ArmStarMove );
end

function ArmProceStarMoveFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" and ArmProceStarMoveFrame:IsShown() then
		UpdateArmProceStarMoveFrame();
	end
	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG" and ArmProceStarMoveFrame:IsShown() then
		UpdateArmProceStarMoveFrame();
	end
	if arg1 == "GE_ARMMOVESTAR_SUCESS" then
		ArmProceStarMoveFrameModelView:Show();
		ArmProceStarMoveFrameModelView:setPetModel(0,0,"effect\\entity\\pinzhizhuanyi.ent");
		ArmProceStarMoveFrameModelView:setEntityPosition(0,0,55,0);
	end
end

function ArmProceStarMoveFrame_OnShow()
	ArmProceStarMoveFrame_OnHide();
	ArmProceStarMoveFrameNeedItemFont:SetText( "" );
	ArmProceStarMoveFrameNeedMoneyFont:SetText( "0" );
	
	ArmProceStarMoveFrameSrcItemBtn:SetClientUserData( 0,-1 );
	ArmProceStarMoveFrameSrcItemBtn:SetClientUserData( 1,-1 );
	ArmProceStarMoveFrameSrcItemBtnBackTexture:Show();
	ArmProceStarMoveFrameSrcItemBtnUVAnimationTex:Hide();
	ArmProceStarMoveFrameSrcItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");

	IsStuffItemExist = false;
	IsStuffItemBind = false;
	ArmProceStarMoveFrameStuffItemBtn:SetClientUserData( 0,-1 );
	ArmProceStarMoveFrameStuffItemBtn:SetClientUserData( 1,-1 );
	ArmProceStarMoveFrameStuffItemBtnBackTexture:Show();
	ArmProceStarMoveFrameStuffItemBtnUVAnimationTex:Hide();
	ArmProceStarMoveFrameStuffItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");

	ArmProceStarMoveFrameDestItemBtn:SetClientUserData( 0,-1 );
	ArmProceStarMoveFrameDestItemBtn:SetClientUserData( 1,-1 );
	ArmProceStarMoveFrameDestItemBtnBackTexture:Show();
	ArmProceStarMoveFrameDestItemBtnUVAnimationTex:Hide();
	ArmProceStarMoveFrameDestItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");

	ArmProceStarMoveFrameSrcStarFont:SetText("");
	ArmProceStarMoveFrameDestStarFont:SetText("");
	ArmProceStarMoveFrameNeedMoneyFont:SetText(0);
	ArmProceStarMoveFrameOKBtn:Disable();
end

function ArmProceStarMoveFrame_OnHide()
	if MessageBoxFrame:IsShown() then
		local szMsg = MessageBoxFrame:GetClientString();
		if szMsg =="品质转移绑定提示" or szMsg =="品质转移星级覆盖提示" then
			MessageBoxFrame:Hide();
		end
	end
	local nList = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 0 );
	local nGrid = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 1 );
	if nList >= 0 and nGrid >= 0 then
		UnLockItem( nList, nGrid, 0 );
	end
	ArmProceStarMoveFrameSrcItemBtn:SetClientUserData(0,-1);
	ArmProceStarMoveFrameSrcItemBtn:SetClientUserData(1,-1);
	ArmProceStarMoveFrameSrcItemBtnUVAnimationTex:Hide();
	ArmProceStarMoveFrameSrcItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");

	nList = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 0 );
	nGrid = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 1 );
	if nList >= 0 and nGrid >= 0 then
		UnLockItem( nList, nGrid, 0 );
	end
	ArmProceStarMoveFrameDestItemBtn:SetClientUserData(0,-1);
	ArmProceStarMoveFrameDestItemBtn:SetClientUserData(1,-1);
	ArmProceStarMoveFrameDestItemBtnUVAnimationTex:Hide();
	ArmProceStarMoveFrameDestItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		
	ArmProceStarMoveFrameSrcItemBtnBindTexture:Hide();
	ArmProceStarMoveFrameDestItemBtnBindTexture:Hide();
	ArmProceStarMoveFrameStuffItemBtnBindTexture:Hide();
	ArmProceStarMoveFrameModelView:Hide();
	
	IsStuffItemExist = false;
	IsStuffItemBind = false;
	ArmProceStarMoveFrameStuffItemBtn:SetClientUserData( 0,-1 );
	ArmProceStarMoveFrameStuffItemBtn:SetClientUserData( 1,-1 );
	ArmProceStarMoveFrameStuffItemBtnBackTexture:Show();
	ArmProceStarMoveFrameStuffItemBtnUVAnimationTex:Hide();
	ArmProceStarMoveFrameStuffItemBtnCountFont:SetText( "" );
	ArmProceStarMoveFrameStuffItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ClearItemLock( "ArmProceStarMoveFrameStuffItemBtn" )
	
	ArmProceStarMoveFrameNeedItemFont:SetText( "" );
	ArmProceStarMoveFrameNeedMoneyFont:SetText( "0" );
end

function ArmProceStarMoveStartDrag( szBtnName )
	local btn		= getglobal( szBtnName );
	if szBtnName == "ArmProceStarMoveFrameStuffItemBtn" and IsStuffItemExist then
		UIMSG:getUIGV().DragFrom = DRAG_ARMSTARMOVE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), 0, 0, btn:GetClientID() );
		return;
	end
	local nListType	= btn:GetClientUserData( 0 ) ;
	local nGrid		= btn:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARMSTARMOVE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),btn:GetClientID() );
		return;
	end
end

function ReceiveArmProceStarMove( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn = getglobal( szBtnName );
	-- 如果目标来自背包
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "品质转移绑定提示" then return end;
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local srcList		= dragfrom;						-- 源容器类型
		local srcGrid		= dragdata1;					-- 从源容器哪个格子过来
		local item			= container:getItem( srcList, srcGrid );
		
		local SecondBtn = "";
		if szBtnName == "ArmProceStarMoveFrameSrcItemBtn" then
			if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
				ShowMidTips("请放入装备");
				return;
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
				ShowMidTips( "时装道具不能进行星级转移" );
				return;
			end
			if item:getStart() <= 0 then
				ShowMidTips("请放入有品质星级的装备");
				return;
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
				ShowMidTips( "徽章装备不能进行星级转移" );
				return;
			end
			szSecondBtn = "ArmProceStarMoveFrameDestItemBtn";
		elseif szBtnName == "ArmProceStarMoveFrameDestItemBtn" then
			if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
				ShowMidTips("请放入装备");
				return;
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
				ShowMidTips( "时装道具不能进行星级转移" );
				return;
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
				ShowMidTips( "徽章装备不能进行星级转移" );
				return;
			end
			szSecondBtn = "ArmProceStarMoveFrameSrcItemBtn";
		elseif szBtnName == "ArmProceStarMoveFrameStuffItemBtn" then
			if item:getItemId() ~= ARM_STAR_ITEM_TOLUA then
				ShowMidTips("请放入星辉宝钻");
				return;
			end
		end
		if szBtnName ~= "ArmProceStarMoveFrameStuffItemBtn" then
			local secondBtn = getglobal( szSecondBtn );
			local secondList = secondBtn:GetClientUserData( 0 );
			local secondGrid = secondBtn:GetClientUserData( 1 );
			if secondList >= 0 and secondGrid >= 0 then
				local secondItem = container:getItem( secondList,secondGrid );
				if secondItem:getItemId() > 0 then
					local srcItemDef = item:getItemDef();
					local secondItemDef = secondItem:getItemDef();
					--对装备位的判断
					if( srcItemDef.WearPos1 ~= secondItemDef.WearPos1 or
						srcItemDef.WearPos2 ~= secondItemDef.WearPos2 or
						srcItemDef.WearPos3 ~= secondItemDef.WearPos3 ) then
						ShowMidTips( "两件装备的类型不同" );
						return;
					end
					--[[
					--对装等级的判断
					if szBtnName == "ArmProceStarMoveFrameSrcItemBtn" then
						if ( math.abs(srcItemDef.WearPre.RoleLevelMin - secondItemDef.WearPre.RoleLevelMin) ) > 10 then
							if srcItemDef.WearPre.RoleLevelMin < secondItemDef.WearPre.RoleLevelMin then
								ShowMidTips( "转星装备等级过低，无法放入" );
								return;
							end
						end
					elseif szBtnName == "ArmProceStarMoveFrameDestItemBtn" then
						if ( math.abs(secondItemDef.WearPre.RoleLevelMin - srcItemDef.WearPre.RoleLevelMin) ) > 10 then
							if secondItemDef.WearPre.RoleLevelMin < srcItemDef.WearPre.RoleLevelMin then
								ShowMidTips( "目标装备等级过高，无法放入" );
								return;
							end
						end
					end
					--]]
				end
			end
		end

		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		
		local srcitem = container:getItem( nListType,nGridIdx );			
		local nItemId	= srcitem:getItemId();
		if nItemId > 0 then
			UnLockItem( nListType, nGridIdx, nItemId );--解锁
		end
		btn:SetClientUserData( 0,srcList );
		btn:SetClientUserData( 1,srcGrid );
		if szBtnName ~= "ArmProceStarMoveFrameStuffItemBtn" then
			LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		elseif szBtnName == "ArmProceStarMoveFrameStuffItemBtn" and item:getItemId() == ARM_STAR_ITEM_TOLUA then
			IsStuffItemExist = true;
			IsStuffItemBind = item:isItemBind();
		end
		UpdateArmProceStarMoveFrame();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		ShowMidTips("只有背包内装备才可进行此操作");
	end

	if dragfrom == DRAG_ARMSTARMOVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;	 
		return;
	end
end

function UpdateArmProceStarMoveFrame()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local nSrcList = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 0 );
	local nSrcGrid = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 1 );

	local nStar = 0;
	local nDestStar = 0;
	local nStuffNum = 0;
	local nCount = 0;
	local nNeedMoney = 0;
	local destItemDef,item = nil,nil;
	local srcExist = false;
	local desExist = false;
	ArmProceStarMoveFrameNeedItemFont:SetText( "" );
	ArmProceStarMoveFrameNeedMoneyFont:SetText( "0" );
	if nSrcList >= 0 and nSrcGrid >= 0 then
		srcExist = true;
		item = container:getItem( nSrcList,nSrcGrid );
		if item:getItemId() == 0 then
			UnLockItem( nSrcList, nSrcGrid, 0 );
			ArmProceStarMoveFrameSrcItemBtn:SetClientUserData(0,-1);
			ArmProceStarMoveFrameSrcItemBtn:SetClientUserData(1,-1);
			ArmProceStarMoveFrameSrcItemBtnUVAnimationTex:Hide();
			ArmProceStarMoveFrameSrcItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
			ArmProceStarMoveFrameSrcStarFont:SetText( "" );
			return;
		end
		local itemDef = item:getItemDef();
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmProceStarMoveFrameSrcItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, "ArmProceStarMoveFrameSrcItemBtnUVAnimationTex",item);	
		if item:isItemBind() then
			ArmProceStarMoveFrameSrcItemBtnBindTexture:Show();
		else
			ArmProceStarMoveFrameSrcItemBtnBindTexture:Hide();
		end

		nStar = item:getStart();
		if nStar > 0 then
			ArmProceStarMoveFrameSrcStarFont:SetText("+" .. nStar );
		else
			ArmProceStarMoveFrameSrcStarFont:SetText( "" );
		end
		--这里只显示背包里的宝石总数
		--[[
		nStuffNum = GameArmProce:getMoveStarNeedStuffNum( nStar );
		if nStuffNum > 0 then
			ArmProceStarMoveFrameNeedItemFont:SetText( "星辉宝钻"..nStuffNum.."个" );
		end
		]]--
	end
	if IsStuffItemExist then
		local stuffItemDef = getItemDef( ARM_STAR_ITEM_TOLUA );
		if stuffItemDef ~= nil then
			IconPath = GetItemEquipIconPath()..stuffItemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ArmProceStarMoveFrameStuffItemBtnIconTexture:SetTexture( IconPath );
			SetEquipImportantEffectUV( stuffItemDef, "ArmProceStarMoveFrameStuffItemBtnUVAnimationTex");
			if IsStuffItemBind then
				ArmProceStarMoveFrameStuffItemBtnBindTexture:Show();
			else
				ArmProceStarMoveFrameStuffItemBtnBindTexture:Hide();
			end
			--nCount = container:getItemCount( ARM_STAR_ITEM_TOLUA );
			ClearItemLock( "ArmProceStarMoveFrameStuffItemBtn" )
			if IsStuffItemBind then
				nCount = container:getBindItemNum( CONTAINER_TYPE_SUNDRIES, ARM_STAR_ITEM_TOLUA );
			else
				nCount = container:getUnBindItemNum( CONTAINER_TYPE_SUNDRIES, ARM_STAR_ITEM_TOLUA );
			end
			for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
				local t = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
				if t:getItemId() == ARM_STAR_ITEM_TOLUA and t:isItemBind() == IsNeedGemBind then
					LockItem( t:getListType(), t:getGridIdx(), t:getItemId(), "ArmProceStarMoveFrameStuffItemBtn" )
				end
			end
			if nCount <= 0 then
				ArmProceStarMoveFrameOnRightClick( "ArmProceStarMoveFrameStuffItemBtn" );
			elseif nCount > 1 then
				ArmProceStarMoveFrameStuffItemBtnCountFont:SetText( nCount );
			end
		end
	else
		ArmProceStarMoveFrameStuffItemBtnCountFont:SetText( "" );
	end
	local nDestList = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 0 );
	local nDestGrid = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 1 );
	if nDestList >= 0 and nDestGrid >= 0 then
		desExist = true;
		local destItem = container:getItem( nDestList,nDestGrid );
		if destItem:getItemId() > 0 then
			destItemDef = destItem:getItemDef();
			local IconPath = GetItemEquipIconPath()..destItemDef.IconID..".tga";
			if not IsInExistence( IconPath ) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			ArmProceStarMoveFrameDestItemBtnIconTexture:SetTexture( IconPath );
			SetEquipImportantEffectUV( destItemDef, "ArmProceStarMoveFrameDestItemBtnUVAnimationTex",destItem);
			if destItem:isItemBind() then
				ArmProceStarMoveFrameDestItemBtnBindTexture:Show();
			else
				ArmProceStarMoveFrameDestItemBtnBindTexture:Hide();
			end
			nDestStar = destItem:getStart();
			if nDestStar > 0 then
				ArmProceStarMoveFrameDestStarFont:SetText( "+" ..nDestStar );
			else
				ArmProceStarMoveFrameDestStarFont:SetText("");
			end
		else
			UnLockItem( nDestList, nDestGrid, 0 );
			ArmProceStarMoveFrameDestItemBtn:SetClientUserData(0,-1);
			ArmProceStarMoveFrameDestItemBtn:SetClientUserData(1,-1);
			ArmProceStarMoveFrameDestItemBtnUVAnimationTex:Hide();
			ArmProceStarMoveFrameDestItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");

		end
	end

	if destItemDef ~= nil and destItemDef.WearPre.RoleLevelMin >= 60 then
		local perfectStar = item ~= nil and item:getArmPefectStarNum() or 0
		local armStarDef = getArmStarDef( perfectStar );
		nNeedMoney = armStarDef and math.floor( ((destItemDef.WearPre.RoleLevelMin^2/100) * armStarDef.MoveMoneyVal)/10 ) * 10 or 0;
		ArmProceStarMoveFrameNeedMoneyFont:SetText( nNeedMoney );
	end

	
	--if srcExist and desExist then
		ArmProceStarMoveFrameOKBtn:Enable();
	--else
		--ArmProceStarMoveFrameOKBtn:Disable();
	--end
end

function CallArmProceStarMove()
	local nSrcList = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 0 );
	local nSrcGrid = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 1 );
	local nDestList = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 0 );
	local nDestGrid = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 1 );
	local bind = 0;
	if IsStuffItemBind then
		bind = 1;
	end
	GameArmProce:moveArmStar( nSrcList,nSrcGrid,nDestList,nDestGrid, bind );
end

function ArmProceStarMoveFrameOkBtn_OnClick()
	local nSrcList = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 0 );
	local nSrcGrid = ArmProceStarMoveFrameSrcItemBtn:GetClientUserData( 1 );
	local nDestList = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 0 );
	local nDestGrid = ArmProceStarMoveFrameDestItemBtn:GetClientUserData( 1 );
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	local srcItem = container:getItem( nSrcList, nSrcGrid );
	local desItem = container:getItem( nDestList, nDestGrid );
	if srcItem == nil or desItem == nil then return end
	--如果材料没有星级
	if srcItem:getStart() <= 0 then
		ShowMidTips("转移装备未强化，无法转移");
		return;
	end
	
	local destItemDef = desItem:getItemDef();
	local nStar		  = srcItem:getStart();


	local nNeedMoney = tonumber( ArmProceStarMoveFrameNeedMoneyFont:GetText() )
	if nNeedMoney > MainPlayer:getMoney() then
		ShowMidTips("银币不足，无法转移");
		ShowBuyMoneyFrame();
		return;
	end

	if srcItem:isArmFengYin() or desItem:isArmFengYin() then
		ShowMidTips("封印装备无法进行此操作");
		return;
	end
	
	local idx = 0;
	for i=3, desItem:getStart() do
		local starValIdx = desItem:getArmAddStarValIdx( i-1 );
		if starValIdx >= MAX_STAR_MUL_LIST - 1 then
			idx = 3;
			break;
		end
	end
	if idx ~= 0 then
		ImportantItemConfirmFrameTitle:SetText("装备转星确认");
		ShowImportantItemConfirmFrame( "转星", desItem, idx, false );
		ImportantItemConfirmFrame:SetClientUserData( 0, nDestList );
		ImportantItemConfirmFrame:SetClientUserData( 1, nDestGrid );
		SetItemWarnFrame("ArmProceStarMoveFrame");
		return;
	end

	--如果目标装备未绑定
	if not desItem:isItemBind() then
		MessageBox( "绑定提示", "转星会使目标装备绑定，确定继续转星？" );
		MessageBoxFrame:SetClientString("品质转移绑定提示");
		return;
	end

	--如果目标已有星级
	if desItem:getStart() > 0 then
		MessageBox( "品质转移星级覆盖提示","转星成功后,目标装备的强化星级将被覆盖，是否确定转星？" );
		MessageBoxFrame:SetClientString("品质转移星级覆盖提示");
		return;
	end
	CallArmProceStarMove();
end
------------------		宝石摘取		--------------------------
function GemProcePickFrame_OnLoad()
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent( "GE_GEM_PICK_SUCESS" );
end

function GemProcePickFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" and GemProcePickFrame:IsShown() then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local container = mainplayer:getContainer();
		local itemId = this:GetClientUserData( 0 );
		local nStuffID = ARM_GEM_PICK_ITEM_TOLUA;
		local nCount = container:getItemCount( nStuffID );
		GemProcePickFrameItemBtnCountFont:SetText( "1" .."/" .. nCount );
		GemProcePickFrameStrengBtn:SetClientUserData(0,nCount);
		if nCount > 0 then
			GemProcePickFrameItemBtnIconTexture:SetGray( false );
		else
			GemProcePickFrameItemBtnIconTexture:SetGray( true );
		end
	end
	if arg1 == "GE_GEM_PICK_SUCESS" then
		--刷新宝石界面
		UpdateBaoShiXiangQianFrame();
	end
end

function GemProcePickFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local nStuffID = ARM_GEM_PICK_ITEM_TOLUA;
	local stuffItemDef = getItemDef( nStuffID );
	if stuffItemDef ~= nil then
		GemProcePickFrameDesc:SetText( "摘取宝石会消耗一个" .. stuffItemDef.Name .. "，确定摘取吗？" )
		local btn = getglobal(this:GetName() .. "ItemBtn" );
		local boxTexture = getglobal( btn:GetName() .. "BoxTexture" );
		local iconTexture = getglobal( btn:GetName() .. "IconTexture" );
		local UVAnimationTex = getglobal( btn:GetName() .."UVAnimationTex" );		
		--boxTexture:SetTexUV( 749,652,44,44 );
		boxTexture:Show();
		local szPath = GetItemEquipIconPath()..stuffItemDef.IconID..".tga";
		if not IsInExistence( szPath ) then
			szPath = DEFAULT_ITEM_PATH;
		end
		iconTexture:SetTexture( szPath );
		GemProcePickFrameItemName:SetText( stuffItemDef.Name );
	end
	local nCount = container:getItemCount( nStuffID );
	GemProcePickFrameItemBtnCountFont:SetText( "1" .."/" .. nCount );
	GemProcePickFrameStrengBtn:SetClientUserData(0,nCount);
	if nCount > 0 then
		GemProcePickFrameItemBtnIconTexture:SetGray( false );
	else
		GemProcePickFrameItemBtnIconTexture:SetGray( true );
	end
end

function GemProcePickFrame_OnHide()
	this:SetClientUserData( 0,0 );
	this:SetClientUserData( 1,0 );
	this:SetClientUserData( 2,0 );
	this:SetClientUserData( 3,0 );
end

function GemProcePick_OnClick()
	if this:GetClientUserData(0) == 0 then
		MessageBoxForItem("提示",getItemDef(ARM_GEM_PICK_ITEM_TOLUA).Name);
		MessageBoxForItemFrame:SetClientUserData(0,ARM_GEM_PICK_ITEM_TOLUA);
		return;
	end
	local parent = getglobal( this:GetParent() );
	local itemId = parent:GetClientUserData( 0 );
	local nIdx = parent:GetClientUserData( 1 );
	local nGrid = parent:GetClientUserData( 2 );
	local nList = parent:GetClientUserData( 3 );
	local nStuffID = ARM_GEM_PICK_ITEM_TOLUA;
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nStuffID );
	if item:getItemId() > 0 then
		GameArmProce:gemPick( nIdx, nList, nGrid,nStuffID,item:getListType(),item:getGridIdx() );
		GemProcePickFrame:Hide();
	end
end
------------------------------镶嵌宝石属性重置--------------------------------
local t_result = { 
"物理攻击",	"法术攻击",	"魔法防御",	"物理防御",	"物理伤害减免",	"魔法伤害减免",
"最大HP上升",	"最大MP上升",	"HP回复",	"MP回复",	"命中率",		"破坏率",
"暴击率",	"暴击伤害",
			 }
local szRichText =	[[#ceaeabe重置说明：
#ca09b82●宝石重置会将已镶嵌的宝石属性重置
#ca09b82●宝石重置会花费金币或者金券
#ca09b82●点击“保存”会将已重置的属性保存下来
]];
--多彩宝石颜色值
local COLOR_FULL_VAL = 3	 
local ResetItemId = 0;
local ResetHoleIdx = 0;
local ResetEquipListType = 0;
local ResetEquipGridIndex = 0;
function setResetItemInfo( itemId, holeIdx, EquipListType, EquipGridIndex )
	ResetItemId = itemId;
	ResetHoleIdx = holeIdx;
	ResetEquipListType = EquipListType;
	ResetEquipGridIndex = EquipGridIndex;
end
function GemProceResetFrame_onLoad()
	local GoldCoinCheckBtn = getglobal("GemProceResetFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(true);
end
function GetGemResetEquipItem()
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local equipItem = nil;
	if ResetEquipListType == CONTAINER_TYPE_SUNDRIES then
		local container		=	MainPlayer:getContainer();
		equipItem	=	container:getItem( ResetEquipListType, ResetEquipGridIndex );
	elseif ResetEquipListType == CONTAINER_TYPE_WEAR then
		local equip = MainPlayer:getEquip();
		equipItem = equip:getItem( ResetEquipGridIndex );
	end
	return equipItem;
end
function SetResetShow()
	local equipItem 	=	GetGemResetEquipItem();
	if equipItem == nil then return end;
	ResetItemId			= 	equipItem:getItemId();
	if ResetItemId > 0 then
		local gemId		= equipItem:getHoleGemId( ResetHoleIdx - 1 );
		local gemNewDef = GameArmProce:getGemNewDef( gemId );
		local resetInfo = gemNewDef.ReSet;
		local MoneyFont = getglobal("GemProceResetFrameMoneyFont")
		local GoldCoinCheckBtn = getglobal("GemProceResetFrameChoseGoldCoin");
		if GoldCoinCheckBtn:GetCheckState() then
			MoneyFont:SetText( resetInfo.JinBi );
		else
			MoneyFont:SetText( resetInfo.JinQuan );
		end
		
		local AttrResultId = equipItem:getHoleGemAttrIdx( ResetHoleIdx -1 );
		local DesResultId = GetOneItemResultConfig( { ["resultID"] = gemNewDef.Attr[AttrResultId].AttrID } )
		-- 控制当前宝石等级（ 对应的充能等级可能不存在）
		local gemStar = equipItem:getHoleGemStar( ResetHoleIdx - 1 );
		local topStar = IsGemTopLevel( gemNewDef.Level,gemStar );
		if topStar < gemStar then 
			gemStar = topStar;
		end
		local nMin,nMax = getResultInfo( gemNewDef, AttrResultId, gemNewDef.Level, gemStar );
		local szInfo = DesResultId[2];
		local XiangQianInfoTypeFont = getglobal( "GemProceResetFrameXiangQianInfoTypeFont" );
		XiangQianInfoTypeFont:SetText( szInfo );
			
		local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[equipItem:getHoleGemAttrIdx( ResetHoleIdx - 1 )].AttrID, gemNewDef.Level ,gemStar );
		local ResultID = equipItem:getHoleGemResultIndex( ResetHoleIdx - 1 );
		local nGemMul = 1 + equipItem:getHoleGemMul( ResetHoleIdx - 1 ) / 100;
		local nGemValue = math.floor( gemAttrStarDef.Attr[ResultID].AttrID * nGemMul);
		if nGemValue < nMin then 
			nGemValue = nMin;
		elseif nGemValue > nMax then 
			nGemValue = nMax;
		end
		local XiangQianInfoResultFont = getglobal("GemProceResetFrameXiangQianInfoResultFont");
		XiangQianInfoResultFont:SetText( "+"..nGemValue );
		local XiangQianInfoNowFont = getglobal("GemProceResetFrameXiangQianInfoRateFont");
		XiangQianInfoNowFont:SetText("("..nMin .."~"..nMax..")");
	end
end
function GemProceResetFrame_onShow()
	GemProceResetFrame_onHide();
	this:SetPoint( "topleft", "UIClient", "topleft", BaoShiXiangQianFrame:GetRealLeft()+278, BaoShiXiangQianFrame:GetRealTop() );
	GemProceResetFrameResetInfo:SetText( szRichText, 255, 254, 189 );
	local GoldCoinCheckBtn = getglobal("GemProceResetFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(true);
	local GoldCouponsCheckBtn = getglobal("GemProceResetFrameChoseGoldCoupons");
	GoldCouponsCheckBtn:setCheckState(false);
	local MoneyTex = getglobal("GemProceResetFrameMoneyTex");
	MoneyTex:SetTexUV( 57, 494, 20, 18 );
	GemProceResetFrameSaveSuccessBtn:Hide();
	SetResetShow();
end
function GemProceResetFrame_onUpdate()
	SetResetShow();
end
function GemProceResetFrame_onHide()
	if MessageBoxFrame:IsShown() then
		if MessageBoxFrame:GetClientString() == "金券重置未绑定装备宝石" or MessageBoxFrame:GetClientString() == "宝石重置未保存满属性" 
			or MessageBoxFrame:GetClientString() == "宝石重置已有满属性" then
			MessageBoxFrame:Hide();
		end
	end
	GemProceResetFrameSaveSuccessBtn:Hide();
	GemProceResetFrameCurResetInfoBtn:Hide();
	GemProceResetFrameResetBtn:SetGray( false );
	GemProceResetFrameResetBtn:Enable();
	local MAX_XIANG_QIAN_KONG_NUM = 4;
	for i = 1, MAX_XIANG_QIAN_KONG_NUM do
		local resetBtn = getglobal( "BaoShiXiangQianFrame_BaoShiResetBtn"..i );
		resetBtn:DisChecked();
	end
end
function GemProceResetFrameChoseGoldCoin_onClick()
	local GoldCoinCheckBtn = getglobal("GemProceResetFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(true);
	local GoldCouponsCheckBtn = getglobal("GemProceResetFrameChoseGoldCoupons");
	GoldCouponsCheckBtn:setCheckState(false);
	local MoneyTex = getglobal("GemProceResetFrameMoneyTex");
	MoneyTex:SetTexUV( 57, 494, 20, 18 );
end
function GemProceResetFrameChoseGoldCoupons_onClick()
	local GoldCouponsCheckBtn = getglobal("GemProceResetFrameChoseGoldCoupons");
	GoldCouponsCheckBtn:setCheckState(true);
	local GoldCoinCheckBtn = getglobal("GemProceResetFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(false);
	local MoneyTex = getglobal("GemProceResetFrameMoneyTex");
	MoneyTex:SetTexUV( 99, 494, 17, 18 );
end
function GemProceResetFrameCurResetInfoBtnCancleBtn_onClick()
	GemProceResetFrameCurResetInfoBtn:Hide();
end
function GemProceResetFrameCurResetInfoBtnSaveBtn_onClick()
	GameArmProce:saveResetGem( ResetEquipListType, ResetEquipGridIndex );
end
function GemSaveResetResult( nFlag )
	GemProceResetFrameCurResetInfoBtn:Hide();
	if nFlag == 1 then
		GemProceResetFrameSaveSuccessBtn:Show();
	else
		GemProceResetFrameSaveSuccessBtn:Hide();
	end
end

function EnableGemProceResetFrame(b)
	if b then
		GemProceResetFrameResetBtn:Enable();
		GemProceResetFrameCurResetInfoBtnSaveBtn:Enable();
		GemProceResetFrameCurResetInfoBtnCancleBtn:Enable();
	else
		GemProceResetFrameResetBtn:Disable();
		GemProceResetFrameCurResetInfoBtnSaveBtn:Disable();
		GemProceResetFrameCurResetInfoBtnCancleBtn:Disable();
	end
end

function GemProceResetFrameResetBtn_onClick()
	if MessageBoxFrame:IsShown() then
		if MessageBoxFrame:GetClientString() == "金券重置未绑定装备宝石" or MessageBoxFrame:GetClientString() == "宝石重置未保存满属性" 
			or MessageBoxFrame:GetClientString() == "宝石重置已有满属性" then
			return
		end
	end
	local UseMoneyType = 0;
	local GoldCoinCheckBtn = getglobal("GemProceResetFrameChoseGoldCoin");
	if GoldCoinCheckBtn:GetCheckState() then
		UseMoneyType = 0;
	else
		UseMoneyType = 1;
	end
	if GemProceResetFrameCurResetInfoBtn:IsShown() and GemProceResetFrameCurResetInfoBtnOverValTex:IsShown() then
			MessageBox( "宝石重置","当前宝石已经重置为满值,点击确定保存" );
			MessageBoxFrame:SetClientString( "宝石重置未保存满属性" );
			MessageBoxFrame:SetClientUserData( 0,ResetHoleIdx );
			MessageBoxFrame:SetClientUserData( 1,UseMoneyType );
			MessageBoxFrame:SetClientUserData( 2,ResetEquipListType );
			MessageBoxFrame:SetClientUserData( 3,ResetEquipGridIndex );
		return;
	end
	--是否现有的属性以为最高属性
	local _,_,szCurAddattr = string.find( GemProceResetFrameXiangQianInfoResultFont:GetText(), "+(%d+)" )
	local _,_,szMin,szMax = string.find( GemProceResetFrameXiangQianInfoRateFont:GetText(), "(%d+)~(%d+)" )
	local nCurAddattr = tonumber( szCurAddattr )
	local nMin = tonumber( szMin )
	local nMax = tonumber( szMax )	
	if nCurAddattr ~= 0 and nMax ~= 0 and nCurAddattr >= nMax then
			MessageBox( "宝石重置","当前宝石属性达到满值,确认再次重置吗?" );
			MessageBoxFrame:SetClientString( "宝石重置已有满属性" );
			MessageBoxFrame:SetClientUserData( 0,ResetHoleIdx );
			MessageBoxFrame:SetClientUserData( 1,UseMoneyType );
			MessageBoxFrame:SetClientUserData( 2,ResetEquipListType );
			MessageBoxFrame:SetClientUserData( 3,ResetEquipGridIndex );
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	if container:getContainerLockState() ~= CONTAINER_LOCK_STATE_LOCK then
		GemProceResetFrameResetBtn:SetGray( true );
		GemProceResetFrameResetBtn:Disable();
	else
		ShowMidTips("背包上锁，无法重置");
		return;
	end

	if UseMoneyType == 0 then
		local item = GetGemResetEquipItem();
		if not item:isItemBind() then
			MessageBox( "绑定提示","使用金券重置会使该装备绑定，确定继续重置？" );
			MessageBoxFrame:SetClientString( "金券重置未绑定装备宝石" );
			MessageBoxFrame:SetClientUserData( 0,ResetHoleIdx );
			MessageBoxFrame:SetClientUserData( 1,UseMoneyType );
			MessageBoxFrame:SetClientUserData( 2,ResetEquipListType );
			MessageBoxFrame:SetClientUserData( 3,ResetEquipGridIndex );
			return;
		end
	else
		if mainplayer:getGodCoin() < tonumber( GemProceResetFrameMoneyFont:GetText() ) then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
			return;
		end
	end
	GameArmProce:resetGem( ResetHoleIdx, UseMoneyType, ResetEquipListType, ResetEquipGridIndex );
end
function GemResetResult( nFlag, nValIdx, nAttrIdx,nMul )
	GemProceResetFrameResetBtn:SetGray( false );
	GemProceResetFrameResetBtn:Enable();
	if nFlag == 1 then
		GemProceResetFrameSaveSuccessBtn:Hide();
		local gemId		= equipItem:getHoleGemId( ResetHoleIdx - 1 );
		local gemNewDef = GameArmProce:getGemNewDef( gemId );
		local equipItem = GetGemResetEquipItem();
		-- 控制宝石等级对应（充能等级可能不存在）
		local gemStar = equipItem:getHoleGemStar( ResetHoleIdx - 1 );
		local topStar = IsGemTopLevel( gemNewDef.Level,gemStar );
		if topStar < gemStar then 
			gemStar = topStar;
		end	
		
		local nMin,nMax = getResultInfo( gemNewDef, nAttrIdx, gemNewDef.Level, gemStar );
		local DesResultId = GetOneItemResultConfig( { ["resultID"] = gemNewDef.Attr[nAttrIdx].AttrID } )
		local szInfo = DesResultId[2]
		GemProceResetFrameCurResetInfoBtnTypeFont:SetText( szInfo );
		local nGemMul = 1 + nMul / 100;	
		
		local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[nAttrIdx].AttrID, gemNewDef.Level ,gemStar );
		local nGemValue = math.floor( gemAttrStarDef.Attr[nValIdx].AttrID * nGemMul);
		if nGemValue < nMin then 
			nGemValue = nMin;
		elseif nGemValue > nMax then 
			nGemValue = nMax;
		end
		GemProceResetFrameCurResetInfoBtnResultFont:SetText( "+"..nGemValue );
		GemProceResetFrameCurResetInfoBtnRangeFont:SetText("("..nMin .."~"..nMax..")");
		GemProceResetFrameCurResetInfoBtn:Show();
		GemProceResetFrameCurResetInfoBtnUpTex:Hide();
		GemProceResetFrameCurResetInfoBtnEqualTex:Hide();
		GemProceResetFrameCurResetInfoBtnDownTex:Hide();
		GemProceResetFrameCurResetInfoBtnOverValTex:Hide();
		--[[
		if gemNewDef.Color == COLOR_FULL_VAL then
			return;
		end
		--]]
		local szBeforVal = string.sub( GemProceResetFrameXiangQianInfoResultFont:GetText(), string.find( GemProceResetFrameXiangQianInfoResultFont:GetText(), "%d+" ) );
		local beforVal = tonumber( szBeforVal );
		if beforVal > nGemValue and gemNewDef.Color ~= COLOR_FULL_VAL then
			GemProceResetFrameCurResetInfoBtnUpTex:Show();
		elseif beforVal == nGemValue and gemNewDef.Color ~= COLOR_FULL_VAL then
			GemProceResetFrameCurResetInfoBtnEqualTex:Show();
		elseif beforVal < nGemValue and gemNewDef.Color ~= COLOR_FULL_VAL then
			GemProceResetFrameCurResetInfoBtnDownTex:Show();
		end
		if nGemValue == nMax then
			GemProceResetFrameCurResetInfoBtnOverValTex:Show();
		else
			GemProceResetFrameCurResetInfoBtnOverValTex:Hide();
		end

		if GemProceResetFrameXiangQianInfoTypeFont:GetText() == GemProceResetFrameCurResetInfoBtnTypeFont:GetText() then
			if beforVal < nGemValue then
				EnableGemProceResetFrame(false);
				MessageBox("宝石重置保存提示", "宝石属性有所提升，是否保存?");
				MessageBoxFrame:SetClientString("宝石重置保存提示");
			end
		end
	else
		GemProceResetFrameCurResetInfoBtn:Hide();
	end

	if EquipFrame:IsShown() then
		
		SetEquipInfo();
	end
end
function GemProceResetFrameCancleBtn_onClick()
	GemProceResetFrame:Hide();	
end
----------------------------------宝石充能------------------------------------
local CHARGE_GEM_MAX_NUM = 8;
local CHARGE_MAX_STAR_NUM = 9;
local t_MoneyChargeHide = {
	"GemProceChargeFrameGoldCoinEditBack",
	"GemProceChargeFrameGoldCoinTex",
	"GemProceChargeFrameCurGoldCoinNumFont",
	"GemProceChargeFrameGoldCouponsEditBack",
	"GemProceChargeFrameGoldCouponsTex",
	"GemProceChargeFrameCurGoldCouponsNumFont",
	"GemProceChargeFrameMoneyChargeInfoFont",
	"GemProceChargeFrameMoneyTypeFont",
	"GemProceChargeFrameNeedMoneyFont",
	"GemProceChargeFrameNeedMoneyNumFont",
	"GemProceChargeFrameMoneyTex",
	"GemProceChargeFrameCurGoldCoinFont",
	"GemProceChargeFrameCurGoldCouponsFont",
	"GemProceChargeFrameChoseGoldCoin",
	"GemProceChargeFrameChoseGoldCoinHeadBtn",
	"GemProceChargeFrameChoseGoldCoupons",
	"GemProceChargeFrameChoseGoldCouponsHeadBtn",
							
}
local t_BtnInfo = {
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
	{ itemNum = 0, itemId = 0, isBind = false, },
}
local needMoneyNum = 0;
local ChargeItemId = 0;
local GemOrVipCharge = 0;
local ChargeHoleIdx = 0;
local isInCharge = false;
local nChargeGemIndex = 1;
local nChargeSelectIndex = 1;
local ChargeGemNum = 0;
local nCurGemsTotalPower = 0;
local ChargeEquipListType = 0;
local ChargeEquipGridIndex = 0;
local ChargePowerVal = 0;
local CanPlayAnim = false;
local LevelUpStopCharge = false;
local CurStarNum = -1;
local USE_ID_POWER = 1;
local t_Color ={ 
					{ r="61",	g="109",	b="168", },
					{ r="66",	g="168",	b="61",	}, 
					{ r="202",	g="54",		b="33",},
					{ r="144",	g="142",	b="138",}, 
					};
local t_GemStarTexUV = {
	[0] = { x="915", y="237", w="11", h="12", },
	[1] = { x="927", y="237", w="11", h="12", },
	[2] = { x="939", y="237", w="11", h="12", },
	[3] = { x="915", y="250", w="11", h="12", },
	[4] = { x="927", y="250", w="11", h="12", },
	[5] = { x="939", y="250", w="11", h="12", },
}
function setCurChargeGemIndex()
	for i = 1, CHARGE_GEM_MAX_NUM do
		local curtexture	=	getglobal( "GemProceChargeFrameChargeGemBtn"..i.."IconTexture" );
		local curtextureName	=	curtexture:GetTexture();
		if  string.find( curtextureName, "TouMingTongDao.tga")  then	
			nChargeGemIndex	= i;
			break;
		end
	end
end
--背包中右键点击响应
function AddGemToGemProceCharge( item, nListType,nGridIndex )
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then return end
	if isInCharge then
		ShowMidTips( "充能中无法执行操作" );
		return;
	end
	local itemDef = item:getItemDef();
	
	--判断是否为宝石or碎片
	local IsGem = false;
	local IsGemSuipian = false;
	local GemSuipianPower = 0;
	local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID )
	if gemNewDef ~= nil then
		IsGem = true;
	end
	for i = 1, MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_GEM_CHIP then
			IsGemSuipian = true;
			GemSuipianPower = itemDef.Result[i-1].ResultVal1;
		end
	end
	if not IsGem and not IsGemSuipian then
		ShowMidTips("该物品不可充能");
		return;
	end
	
	
	local MoneyChargeBtn = getglobal("GemProceChargeFrameMoneyChargeBtn");
	if MoneyChargeBtn:IsChecked() then return end
	
	if ChargeGemNum >= 8 then
		ShowMidTips("充能背包已满");
		return;
	end
	if item:getItemId() ~= nil then
		--设置当前空位置索引
		setCurChargeGemIndex();
		if not IsContainerItemLock( nListType, nGridIndex ) then
			LockCurItem( item, "PlayerContainerFrame" );
		end
		local btn =	getglobal( "GemProceChargeFrameChargeGemBtn"..nChargeGemIndex );
		btn:SetClientUserData( 1, nListType);
		btn:SetClientUserData( 2, nGridIndex);
		t_BtnInfo[nChargeGemIndex]["itemNum"] = item:getNum();
		local itemNumFont = getglobal( "GemProceChargeFrameChargeGemBtn"..nChargeGemIndex.."CountFont" );
		if t_BtnInfo[nChargeGemIndex]["itemNum"] > 1 then
			itemNumFont:SetText( t_BtnInfo[nChargeGemIndex]["itemNum"] );
		end
		t_BtnInfo[nChargeGemIndex]["itemId"] = itemDef.ItemID;
		t_BtnInfo[nChargeGemIndex]["isBind"] = item:isItemBind();
		local texture	=	getglobal( btn:GetName().."IconTexture" );
		SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" )
		local texturePath	=	GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( texturePath ) then
			texturePath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( texturePath );
		texture:Show();
		ChargeGemNum = ChargeGemNum +1;
		if IsGem then
			local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level,item:getGemStart() );
			nCurGemsTotalPower = nCurGemsTotalPower + gemLevelStarDef.GetPower*t_BtnInfo[nChargeGemIndex]["itemNum"];
		else
			nCurGemsTotalPower = nCurGemsTotalPower + GemSuipianPower*t_BtnInfo[nChargeGemIndex]["itemNum"];
		end
	end
end
--宝石充能右键点击响应
function RemoveGemFromGemProceCharge( szBtnName )
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then return end
	if isInCharge then
		ShowMidTips( "充能中无法执行操作" );
		return;
	end
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then
		if IsContainerItemLock(  desList, desGrid ) then
			UnLockItem( desList, desGrid )
		end
		local IconTex = getglobal(btn:GetName().."IconTexture");		
		local texture		=	getglobal( btn:GetName().."IconTexture" )
		texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex" )
		UVAnimationTex:Hide()
		local itemNumFont = getglobal( btn:GetName().."CountFont" );
		itemNumFont:SetText("");	
		btn:SetClientUserData( 1, 0 )
		btn:SetClientUserData( 2, 0 )
				
		local itemDef = Item:getItemDef();
		local IsGemSuipian = false;
		local GemSuipianPower = 0;
		local nIdx = btn:GetClientID();
		for i = 1, MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_GEM_CHIP then
				IsGemSuipian = true;
				GemSuipianPower = itemDef.Result[i-1].ResultVal1;
			end
		end
		if not IsGemSuipian then
			local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
			local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level, Item:getGemStart() );
			nCurGemsTotalPower = nCurGemsTotalPower - gemLevelStarDef.GetPower*t_BtnInfo[nIdx]["itemNum"];
		else
			nCurGemsTotalPower = nCurGemsTotalPower - GemSuipianPower*t_BtnInfo[nIdx]["itemNum"];
		end
		
		t_BtnInfo[nIdx]["itemNum"] = 0;
		t_BtnInfo[nIdx]["itemId"] = 0;
		t_BtnInfo[nIdx]["isBind"] = false;	
		
		nChargeGemIndex = btn:GetClientID();
			
		ChargeGemNum = ChargeGemNum -1;
		
		ItemButton_OnLeave();
		--设置宝石充能索引
		setCurChargeGemIndex();
	end
end
--接收鼠标拖拽到宝石充能
function ReceiveGemFromContainer( szBtnName, dragFram, dragdata1, dragdata2, dragdata3 )
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then return end
	if isInCharge then
		ShowMidTips( "充能中无法执行操作" );
		return;
	end
	if not string.find( szBtnName, "Btn" ) then
		return;
	end	
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	if dragFram == DRAG_GEMCHARGE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	elseif dragFram == CONTAINER_TYPE_SUNDRIES then
		local srcList		=	dragFram;
		local srcGrid		=	dragdata1;	
		local item = container:getItem( srcList, srcGrid );
		local itemDef	=	item:getItemDef();
		local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID )
		
		local IsGem = false;
		local IsGemSuipian = false;
		local GemSuipianPower = 0;
		if gemNewDef ~= nil then
			IsGem = true;
		end
		for i = 1, MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == RESULT_GEM_CHIP then
				IsGemSuipian = true;
				GemSuipianPower = itemDef.Result[i-1].ResultVal1
			end
		end
		if not IsGem and not IsGemSuipian then
			ShowMidTips("该物品不可充能");
			return;
		end
		if ChargeGemNum >= 8 then
			ShowMidTips("充能背包已满")
			return;
		end
		nChargeGemIndex = btn:GetClientID();
		local BtnIconTexture	=	getglobal( btn:GetName().."IconTexture" );
		if not string.find( BtnIconTexture:GetTexture(), "TouMingTongDao.tga" ) then
			--设置当前空白索引
			setCurChargeGemIndex();
		end
		btn = getglobal( "GemProceChargeFrameChargeGemBtn"..nChargeGemIndex);
		local desBtn = getglobal( "PlayerContainerFrameItem"..(srcGrid+1) );
		LockCurItem( item, desBtn:GetParent() );
		--充能宝石总数统计
		btn:SetClientUserData( 1, srcList );
		btn:SetClientUserData( 2, srcGrid );
		t_BtnInfo[nChargeGemIndex]["itemNum"] = item:getNum();
		local itemNumFont = getglobal( btn:GetName().."CountFont" );
		if t_BtnInfo[nChargeGemIndex]["itemNum"] > 1 then
			itemNumFont:SetText( t_BtnInfo[nChargeGemIndex]["itemNum"] );
		end
		t_BtnInfo[nChargeGemIndex]["itemId"] = itemDef.ItemID;
		t_BtnInfo[nChargeGemIndex]["isBind"] = item:isItemBind();
		local texture	=	getglobal( btn:GetName().."IconTexture" );
		SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex" )
		local texturePath	=	GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( texturePath ) then
			texturePath = DEFAULT_ITEM_PATH;
		end
		texture:SetTexture( texturePath );
		texture:Show()	
		
		ChargeGemNum = ChargeGemNum +1;
		
		if IsGem then
			local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level, item:getGemStart() );
			nCurGemsTotalPower = nCurGemsTotalPower + gemLevelStarDef.GetPower*t_BtnInfo[nChargeGemIndex]["itemNum"];
		else
			nCurGemsTotalPower = nCurGemsTotalPower + GemSuipianPower*t_BtnInfo[nChargeGemIndex]["itemNum"];
		end
				
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	else
		ShowMidTips("该位置物品无法执行此操作");
		return;
	end
end
--宝石充能拖出
function ReceiveGemProceChargeDrag( szBtnName )
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then return end
	if isInCharge then
		ShowMidTips( "充能中无法执行操作" );
		return;
	end
	if not string.find( szBtnName, "Btn" ) then
		return;
	end	
	local btn			=	getglobal( szBtnName );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local desList		=	btn:GetClientUserData( 1 );
	local desGrid		=	btn:GetClientUserData( 2 );
	nChargeGemIndex	=	btn:GetClientID();
	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then
		UIMSG:getUIGV().DragFrom = DRAG_GEMCHARGE;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), desGrid, nChargeGemIndex, Item:getItemId());
	end
end
--背包接收 宝石充能移除
function RemoveGemAfterDrag( nIdx )
	local btn = getglobal( "GemProceChargeFrameChargeGemBtn" .. nIdx );
	local IconTex = getglobal(btn:GetName().."IconTexture");	
	local texture =	getglobal( btn:GetName().."IconTexture" )
	texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )	
	local UVAnimationTex = getglobal( "GemProceChargeFrameChargeGemBtn"..nIdx.."UVAnimationTex" )
	UVAnimationTex:Hide()
	local itemNumFont = getglobal( "GemProceChargeFrameChargeGemBtn"..nIdx.."CountFont" );
	itemNumFont:SetText( "" );
	local desList =	btn:GetClientUserData( 1 )
	local desGrid =	btn:GetClientUserData( 2 )
	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item = container:getItem( desList, desGrid );
	local itemDef = Item:getItemDef();
	local IsGemSuipian = false;
	local GemSuipianPower = 0;
	for i = 1, MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == RESULT_GEM_CHIP then
			IsGemSuipian = true;
			GemSuipianPower = itemDef.Result[i-1].ResultVal1;
		end
	end
	if not IsGemSuipian then
		local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
		local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level, Item:getGemStart() );
		nCurGemsTotalPower = nCurGemsTotalPower - gemLevelStarDef.GetPower*t_BtnInfo[nIdx]["itemNum"];
	else
		nCurGemsTotalPower = nCurGemsTotalPower - GemSuipianPower*t_BtnInfo[nIdx]["itemNum"];
	end
	
	UnLockItem( desList, desGrid );	
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
	t_BtnInfo[nIdx]["itemNum"] = 0;
	t_BtnInfo[nIdx]["itemId"] = 0;
	t_BtnInfo[nIdx]["isBind"] = false;
	
	ChargeGemNum = ChargeGemNum -1;
end
function GemChargeBtn_onClick()
	if ( arg1 == "RightButton" ) then
		RemoveGemFromGemProceCharge( this:GetName() );
	end
	if this:GetName() ~= "GemProceChargeFrameWaitForChargeGem" then
		nChargeSelectIndex = this:GetClientID();
	end
end
function GemChargeBtn_onEnter()
	if this:GetName() == "GemProceChargeFrameWaitForChargeGem" then
		local MainPlayer	=	ActorMgr:getMainPlayer();
		local Item;
		if ChargeEquipListType == CONTAINER_TYPE_SUNDRIES then
			local container		=	MainPlayer:getContainer();
			Item 				=	container:getItem( ChargeEquipListType, ChargeEquipGridIndex );
		elseif ChargeEquipListType == CONTAINER_TYPE_WEAR then
			local equip			=	MainPlayer:getEquip();
			Item				=	equip:getItem( ChargeEquipGridIndex );
		end
		local itemDef		=	getItemDef( ChargeHoleIdx == 0 and ChargeItemId or Item:getHoleGemId( ChargeHoleIdx -1 ) );	
		local nStar = ( ChargeHoleIdx == 0 and Item:getGemStart() or Item:getHoleGemStar( ChargeHoleIdx -1 ) );
		local gemNewDef		=	GameArmProce:getGemNewDef( itemDef.ItemID );
		local nCurLevelMaxStar = ( gemNewDef.Level - 2 ) * 9
		if nStar <= nCurLevelMaxStar and nStar >= 0 then
			local szGemData		=	( ChargeHoleIdx == 0 and Item:getGemStart()..","..Item:getGemPower() or
			Item:getHoleGemStar( ChargeHoleIdx -1 )..","..Item:getHoleGemPower( ChargeHoleIdx -1 ));
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef, nil, nil, nil, nil, szGemData );
		else
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef );
		end
		return;
	end
	local nListType = this:GetClientUserData( 1 );
	local nGridIndex = this:GetClientUserData( 2 );
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local container		=	MainPlayer:getContainer();
	local Item 			=	container:getItem( nListType, nGridIndex );
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, Item );
end
function GemChargeBtn_onLeave()
	NormalItemTipsFrame:Hide();
end
function RemoveSuccesChargeGem()
	for i = 1, CHARGE_GEM_MAX_NUM do
		if IsGemExist( i ) then
			local btn = getglobal( "GemProceChargeFrameChargeGemBtn"..i );
			local list = btn:GetClientUserData( 1 );
			local grid = btn:GetClientUserData( 2 );
			local MainPlayer	=	ActorMgr:getMainPlayer();
			local container		=	MainPlayer:getContainer();
			local Item 			=	container:getItem( list, grid );
			if Item == nil or Item:getItemId() <= 0 or Item:getNum() ~= t_BtnInfo[i]["itemNum"] then
				local IconTex = getglobal(btn:GetName().."IconTexture");	
				local texture =	getglobal( btn:GetName().."IconTexture" );
				texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )	
				local UVAnimationTex = getglobal( "GemProceChargeFrameChargeGemBtn"..i.."UVAnimationTex" )
				UVAnimationTex:Hide();
				local itemNumFont = getglobal( "GemProceChargeFrameChargeGemBtn"..i.."CountFont" );
				itemNumFont:SetText( "" );
								
				nCurGemsTotalPower = 0;
				UnLockItem( list, grid );
				btn:SetClientUserData( 1, 0 )
				btn:SetClientUserData( 2, 0 )
				
				t_BtnInfo[i]["itemNum"] = 0;
				t_BtnInfo[i]["itemId"] = 0;
				t_BtnInfo[i]["isBind"] = false;
	
				ChargeGemNum = ChargeGemNum -1;
			end
		end
	end
end
function GemChargeResult( nFlag )
	GemProceChargeFrameChargeActBtn:SetGray( false );
	GemProceChargeFrameChargeActBtn:Enable();
	if nFlag == 1 then
		RemoveSuccesChargeGem();
	end
	if EquipFrame:IsShown() then
		SetEquipInfo();
	end
end
--获取实际的充能需要消耗银币的能量值
function GetRealCostMoneyPower()
	local freeCostPower = 0
	for i=1, CHARGE_GEM_MAX_NUM do
		local itemID = t_BtnInfo[i]["itemId"]
		if itemID > 0 then
			local itemDef = getItemDef( itemID )
			for m = 1, MAX_ITEM_RESULT do
				--不需要消耗银币的充能道具，这里默认都是宝石碎片，好处理，如果是宝石的话会有星级这个概念，会复杂很多
				if itemDef.Result[m-1].ResultID == RESULT_GEM_ADD_POWER_FREE then
					for n=1, MAX_ITEM_RESULT do
						if itemDef.Result[n-1].ResultID == RESULT_GEM_CHIP then
							freeCostPower = freeCostPower + itemDef.Result[n-1].ResultVal1*t_BtnInfo[i]["itemNum"];
						end
					end
				end
			end
		end
	end
	return nCurGemsTotalPower - freeCostPower
end
function GemProceChargeFrame_onUpdata()
	GemProceChargeFrameCurPowerFont:SetText( "可充能量："..nCurGemsTotalPower );
	GemProceChargeFrameCurPowerCostMoneyFont:SetText( "消耗银币："..GetRealCostMoneyPower()*GameArmProce:getChargePerPowerUseMoney() );
	SetGemChargeInfo();
	for i = 1, CHARGE_GEM_MAX_NUM do
		local btnSelectTexture	= getglobal( "GemProceChargeFrameChargeGemBtn"..i.."BoxTexture" );
		local btnTexture		= getglobal( "GemProceChargeFrameChargeGemBtn"..i.."IconTexture" );
		if i ~= nChargeSelectIndex or string.find( btnTexture:GetTexture(), "TouMingTongDao.tga" ) then
			btnSelectTexture:Hide();
		else
			btnSelectTexture:Show();
		end
	end
end
function GemProceChargeFrame_onLoad()
	this:SetPoint( "topleft", "PlayerContainerFrame", "topleft", PlayerContainerFrame:GetRealLeft()+PlayerContainerFrame:GetRealWidth(), PlayerContainerFrame:GetRealTop());
	ItemFirstCallBack( "GemProceChargeFrame", AddGemToGemProceCharge );
	HideMoneyCharge();
	local GoldCoinCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(true);
end
function setChargeInfo( itemId, holeIdx, equipListType, equipGridIndex )
	ChargeItemId = itemId;
	ChargeHoleIdx = holeIdx;
	ChargeEquipListType = equipListType;
	ChargeEquipGridIndex = equipGridIndex;
	CanPlayAnim = false;
end
function GemProceChargeFrame_onHide()
	bOpenContainerFlag = true;
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then 
		MessageBoxFrame:Hide();
	end
	for i = 1, CHARGE_MAX_STAR_NUM do
		local starTex = getglobal("GemProceChargeFrameStarTex"..i);
		starTex:Hide();
	end
	for nIdx = 1, CHARGE_GEM_MAX_NUM do
		if IsGemExist( nIdx ) then
			local btn = getglobal( "GemProceChargeFrameChargeGemBtn" .. nIdx );
			local IconTex = getglobal(btn:GetName().."IconTexture");
			local boxTex = getglobal( btn:GetName().."BoxTexture" );
			boxTex:Hide();
			local texture =	getglobal( btn:GetName().."IconTexture" )
			texture:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )	
			local UVAnimationTex = getglobal( "GemProceChargeFrameChargeGemBtn"..nIdx.."UVAnimationTex" )
			UVAnimationTex:Hide()
			local itemNumFont = getglobal( btn:GetName().."CountFont" );
			itemNumFont:SetText( "" );
			local desList = btn:GetClientUserData( 1 );
			local desGrid = btn:GetClientUserData( 2 );
			UnLockItem( desList, desGrid );	
			btn:SetClientUserData( 1, 0 )
			btn:SetClientUserData( 2, 0 )
			t_BtnInfo[nIdx]["itemNum"] = 0;
			t_BtnInfo[nIdx]["itemId"] = 0;
			t_BtnInfo[nIdx]["isBind"] = false;
		end
	end
	if ChargeHoleIdx == 0 then
		UnLockItem( ChargeEquipListType, ChargeEquipGridIndex );
	end
	
	nCurGemsTotalPower = 0;
	ChargeGemNum = 0;
	GemProceChargeFrameCurPowerFont:SetText( "可充能量："..0 );
	GemProceChargeFrameCurPowerCostMoneyFont:SetText( "消耗银币："..0 );
end
function SetStarAndPower()
	local star = 0;
	local power = 0;
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local Item;
	if ChargeEquipListType == CONTAINER_TYPE_SUNDRIES then
		local container		=	MainPlayer:getContainer();
		Item				=	container:getItem( ChargeEquipListType, ChargeEquipGridIndex );
	elseif ChargeEquipListType == CONTAINER_TYPE_WEAR then
		local equip = MainPlayer:getEquip();
		Item		= equip:getItem( ChargeEquipGridIndex );
	end
	if ChargeHoleIdx == 0 then
		return  Item:getGemStart(), Item:getGemPower();
	else
		return Item:getHoleGemStar( ChargeHoleIdx - 1 ), Item:getHoleGemPower( ChargeHoleIdx - 1 );
	end
end
function SetGemNextLevelAndStart( nLv, nStar )
	local level = nLv;
	local star = nStar;	
	local topStar = IsGemTopLevel( level, star );
	if star >= topStar then
		return level, star;
	end
	star = star +1;
	return level,star;
end
function getResultInfo( gemNewDef, nResultID, level, star )
	local nMin				= 0;
	local nMax				= 0;
	local MAX_GEM_ATTR		= 10;
	local MAX_GEM_VAL		= 10;
	for i = 1, MAX_GEM_ATTR do
		if gemNewDef.Attr[i-1].AttrID == gemNewDef.Attr[nResultID].AttrID then
			local gemAttrStarDef = GameArmProce:getGemAttrStarDef( gemNewDef.Attr[i-1].AttrID, level, star );
			if gemAttrStarDef == nil then
				return nMin, nMax;
			end
			nMax = gemAttrStarDef.Attr[0].AttrID;
			nMin = gemAttrStarDef.Attr[0].AttrID;
			for j = 1,MAX_GEM_VAL do
				if gemAttrStarDef.Attr[j-1].AttrID > nMax then
					nMax = gemAttrStarDef.Attr[j-1].AttrID;
				end
				if gemAttrStarDef.Attr[j-1].AttrID < nMin then
					nMin = gemAttrStarDef.Attr[j-1].AttrID;
				end
			end
			break;
		end
	end
	return nMin, nMax;
end
function SetGemChargeInfo()	
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local equipItem;
	if ChargeEquipListType == CONTAINER_TYPE_SUNDRIES then
		local container		=	MainPlayer:getContainer();
		equipItem 			=	container:getItem( ChargeEquipListType, ChargeEquipGridIndex );
	elseif ChargeEquipListType == CONTAINER_TYPE_WEAR then
		local equip 		=	MainPlayer:getEquip();
		equipItem			= equip:getItem( ChargeEquipGridIndex );
	end
	
	local tempItemId	=	( ChargeHoleIdx == 0  and equipItem:getItemId()  or equipItem:getHoleGemId( ChargeHoleIdx - 1 ));
	if tempItemId then
		ChargeItemId = tempItemId;
	end
	
	local ItemDef		=	getItemDef( ChargeItemId );
	local gemNewDef		=	GameArmProce:getGemNewDef( ChargeItemId );
	
	local ItemTex = getglobal("GemProceChargeFrameWaitForChargeGemIconTexture");
	local texPath = GetItemEquipIconPath()..ItemDef.IconID..".tga";
	if not IsInExistence( texPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ItemTex:SetTexture( texPath );
	local UVAnimationTex = getglobal("GemProceChargeFrameWaitForChargeGemUVAnimationTex")
	SetEquipImportantEffectUV( ItemDef, UVAnimationTex:GetName());
	UVAnimationTex:Show();
	
	local GemNameFont = getglobal("GemProceChargeFrameGemNameFont");
	GemNameFont:SetText( ItemDef.Name );
	local color = GetItemNameColor(ItemDef);
	GemNameFont:SetTextColor( color[1], color[2], color[3] );

	local MainPlayer	=	ActorMgr:getMainPlayer();
	local CurGoldCoinNumFont = getglobal( "GemProceChargeFrameCurGoldCoinNumFont" );
	CurGoldCoinNumFont:SetText( MainPlayer:getBindGodCoin() );
	local CurGoldCouponsNumFont = getglobal( "GemProceChargeFrameCurGoldCouponsNumFont" );
	CurGoldCouponsNumFont:SetText( MainPlayer:getGodCoin() );
		
	if LevelUpStopCharge then
		return;
	end
	local gemStar,gemPower = SetStarAndPower();	
	local topStar = IsGemTopLevel( gemNewDef.Level, gemStar );
	if gemStar >= topStar then 
		gemStar = topStar;
	end	
	local starChange = false
	local lowerUV = t_GemStarTexUV[0]
	local starUV = t_GemStarTexUV[0]
	if ( gemStar ~= CurStarNum and gemStar%CHARGE_MAX_STAR_NUM ~= CurStarNum ) and CanPlayAnim then
		starChange = true;
	end
	if gemStar >= CHARGE_MAX_STAR_NUM then
		CurStarNum = ( gemStar%CHARGE_MAX_STAR_NUM == 0 ) and 9 or gemStar%CHARGE_MAX_STAR_NUM
		lowerUV = t_GemStarTexUV[math.floor( (gemStar-1) /CHARGE_MAX_STAR_NUM)]
		starUV = t_GemStarTexUV[math.floor( (gemStar-1) /CHARGE_MAX_STAR_NUM) + 1]
	else
		CurStarNum = gemStar
		lowerUV = t_GemStarTexUV[0]
		starUV = t_GemStarTexUV[1]
	end
		
	for i = 1, CHARGE_MAX_STAR_NUM do
		local starTex = getglobal("GemProceChargeFrameStarTex"..i);
		starTex:SetTexUV( lowerUV.x, lowerUV.y, lowerUV.w, lowerUV.h  )
		starTex:Show()
	end	
		
	for i = 1, CurStarNum do
		local starTex = getglobal("GemProceChargeFrameStarTex"..i);
		starTex:SetTexUV( starUV.x, starUV.y, starUV.w, starUV.h  )
	end
	local GemStartNumFont = getglobal("GemProceChargeFrameGemStartNumFont");
	GemStartNumFont:Show();
	
	local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level, gemStar );

	local powerUp = gemLevelStarDef.PowerUp ;
	local tempChargePowerVal = 0;
	if gemStar == topStar then
		local gemLevelStarDefBefor = GameArmProce:getGemLevelStarDef( gemNewDef.Level, topStar );
		gemPower = gemLevelStarDefBefor.PowerUp;
		powerUp = gemLevelStarDefBefor.PowerUp;
	else 
		tempChargePowerVal = ( gemPower / powerUp ) * 168 * GetScreenScaleY();
	end
	
	if tempChargePowerVal ~= ChargePowerVal  or starChange then
		local effect = getglobal("GemProceChargeFramePowerValBtnUVAnimationTex");
		effect:Show();
		if CanPlayAnim then
			effect:SetUVAnimation( 100, false );
			starChange = false;
		else
			effect:Hide();
		end
	end
	ChargePowerVal = tempChargePowerVal;
	local ValBartex = getglobal("GemProceChargeFramePowerValBtnTex");
	--ValBartex:SetWidth( ChagePowerVal );
	if powerUp <= 0 then		
		ValBartex:Hide();
	else	
		ValBartex:Show();
		ValBartex:SetSize( math.floor( ChargePowerVal ), 9 );
	end
	local valFont = getglobal("GemProceChargeFramePowerValBtnFont");
	valFont:SetText( gemPower.."/"..powerUp );
	
	if gemNewDef.Color == COLOR_FULL_VAL then
		local ResultID = 0;
		if ChargeHoleIdx ~= 0 then
			ResultID = equipItem:getHoleGemAttrIdx( ChargeHoleIdx - 1 );
		end
		local nMin,nMax = getResultInfo( gemNewDef, ResultID, gemNewDef.Level, gemStar  );
		GemProceChargeFrameCurValFont:SetText( "下一星 全属性".." "..nMin.."~"..nMax );
		GemProceChargeFrameCurValFont:Show();
		GemProceChargeFrameNextValFont:Hide();
		if gemStar == topStar then
			GemProceChargeFrameCurValFont:Hide();
		end
	else
		local ResultID = 0;
		if ChargeHoleIdx ~= 0 then
			ResultID = equipItem:getHoleGemAttrIdx( ChargeHoleIdx - 1 );
		end
		local nMin,nMax = getResultInfo( gemNewDef, ResultID, gemNewDef.Level, gemStar  );
		local szInfo;
		for i=1,table.getn(t_tagRESULT) do
			if t_tagRESULT[i][1] == gemNewDef.Attr[ResultID].AttrID then
				szInfo = t_tagRESULT[i][2];
			end
		end
		GemProceChargeFrameCurValFont:SetText( "当   前 "..szInfo.." "..nMin.."~"..nMax );
		GemProceChargeFrameCurValFont:Show();
		
		local nextLv,nextStar = SetGemNextLevelAndStart(  gemNewDef.Level, gemStar );
		nMin,nMax = getResultInfo( gemNewDef, ResultID, nextLv, nextStar  );
		for i=1,table.getn(t_tagRESULT) do
			if t_tagRESULT[i][1] == gemNewDef.Attr[ResultID].AttrID then
				szInfo = t_tagRESULT[i][2];
			end
		end
		GemProceChargeFrameNextValFont:SetText( "下一星 "..szInfo.." "..nMin.."~"..nMax );
		GemProceChargeFrameNextValFont:Show();	
		if gemStar == topStar then
			GemProceChargeFrameNextValFont:SetText("当前宝石星级已达极限");
		end
	end
		
	local useDef = GameArmProce:getUseDef( USE_ID_POWER );	
	local GoldCouponsCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoupons");
	local perMoney;
	if GoldCouponsCheckBtn:GetCheckState() then
		perMoney = useDef.UseJinquan;
	else
		perMoney = useDef.UseJinbi;
	end
	needMoneyNum = (gemLevelStarDef.PowerUp - gemPower) * perMoney ;
	local NeedMoneyNumFont = getglobal("GemProceChargeFrameNeedMoneyNumFont");
	NeedMoneyNumFont:SetText( needMoneyNum );
	local MoneyTex = getglobal("GemProceChargeFrameMoneyTex");
	MoneyTex:SetPoint( "left", NeedMoneyNumFont:GetName(), "left", NeedMoneyNumFont:GetTextExtentWidth( NeedMoneyNumFont:GetText() ) + 3 * GetScreenScaleY(), 0 );
	if gemStar == topStar then
		LevelUpStopCharge = true;
	end
end
function GemProceChargeFrame_onShow()
	PlayerContainerResolveFrame:Hide();
	if PlayerContainerSparEpurateFrame:IsShown() then 
		PlayerContainerSparEpurateFrame:Hide();
	end
	if not PlayerContainerFrame:IsShown() then 
		PlayerContainerFrame:Show();
	end
	PlayerContainerFrame:AddLevelRecursive(1);
	PlayerContainerFrame:SetPoint( "topleft", "UIClient", "topleft", PlayerContainerFrame:GetRealLeft(), PlayerContainerFrame:GetRealTop() );
	this:SetPoint( "topleft", "UIClient", "topleft", PlayerContainerFrame:GetRealLeft()-this:GetRealWidth(), PlayerContainerFrame:GetRealTop());
	needMoneyNum = 0;
	GemOrVipCharge = 0;
	GemProceChargeFrameGemChargeBtn:Checked();
	GemProceChargeFrameMoneyChargeBtn:DisChecked();
	--高级充能默认选择金券
	GemProceChargeFrameChoseGoldeCoinBtn_onClick();
	HideMoneyCharge();
	isInCharge = false;
	nChargeGemIndex = 1;
	ChargeGemNum = 0;
	nCurGemsTotalPower = 0;
	GemProceChargeFrameCurPowerFont:SetText( "可充能量："..nCurGemsTotalPower );
	GemProceChargeFrameCurPowerCostMoneyFont:SetText( "消耗银币："..GetRealCostMoneyPower()*GameArmProce:getChargePerPowerUseMoney() );
	ChargePowerVal = 0;
	LevelUpStopCharge = false;
	if ChargeItemId > 0 then
		local ItemDef		=	getItemDef( ChargeItemId );
		local gemNewDef		=	GameArmProce:getGemNewDef( ChargeItemId );
		if gemNewDef.Level < GEM_CHARGE_LEVEL_MIN  or gemNewDef.Level > GEM_CHARGE_LEVEL_MAX then
			if ChargeHoleIdx == 0 then
				UnLockItem( ChargeEquipListType, ChargeEquipGridIndex, ChargeItemId );
			end
			ShowMidTips("等级不符，不能充能");
			this:Hide();
			return;
		end
		local star, power = SetStarAndPower();
		CurStarNum = star;
		SetGemChargeInfo();
	end
end
function GemProceChargeFrameChoseGoldeCoinBtn_onClick()
	local GoldCoinCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(true);
	local GoldCouponsCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoupons");
	GoldCouponsCheckBtn:setCheckState(false);
	local MoneyTex = getglobal("GemProceChargeFrameMoneyTex");
	MoneyTex:SetTexUV( 57, 494, 20, 18 );
end
function GemProceChargeFrameChoseGoldeCouPonsBtn_onClick()
	local GoldCouponsCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoupons");
	GoldCouponsCheckBtn:setCheckState(true);
	local GoldCoinCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoin");
	GoldCoinCheckBtn:setCheckState(false);
	local MoneyTex = getglobal("GemProceChargeFrameMoneyTex");
	MoneyTex:SetTexUV( 99, 494, 17, 18 );
end
function HideMoneyCharge()
	for i = 1, table.getn( t_MoneyChargeHide ) do
		local layer = getglobal( t_MoneyChargeHide[i] );
		layer:Hide();
	end
	for i = 1, CHARGE_GEM_MAX_NUM do
		local btn = getglobal("GemProceChargeFrameChargeGemBtn"..i);
		btn:Show();
	end
	GemProceChargeFrameCurPowerFont:Show();
	GemProceChargeFrameCurPowerCostMoneyFont:Show();
	GemProceChargeFrameWarningTex:Show();
	GemProceChargeFrameWaring:Show();
end
function HideGemCharge()
	for i = 1, CHARGE_GEM_MAX_NUM do
		local btn = getglobal("GemProceChargeFrameChargeGemBtn"..i);
		btn:Hide();
	end
	GemProceChargeFrameCurPowerFont:Hide();
	GemProceChargeFrameCurPowerCostMoneyFont:Hide();
	GemProceChargeFrameWarningTex:Hide();
	GemProceChargeFrameWaring:Hide();
	
	for i = 1, table.getn( t_MoneyChargeHide ) do
		local layer = getglobal( t_MoneyChargeHide[i] );
		layer:Show();
	end
end
function GemProceChargeFrameGemChargeBtn_onClick()
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then return end
	this:Checked();
	local MoneyChargeBtn = getglobal("GemProceChargeFrameMoneyChargeBtn");
	MoneyChargeBtn:DisChecked();
	GemOrVipCharge = 0;
	HideMoneyCharge();
end
function GemProceChargeFrameMoneyChargeBtn_onClick()
	if MessageBoxFrame:IsShown() and ( MessageTitle:GetText() == "充能确认" 
		or MessageBoxFrame:GetClientString() == "金券充能未绑宝石"
		or MessageTitle:GetText() == "宝石充能绑定" ) then return end
	this:Checked();
	local GemChargeBtn = getglobal("GemProceChargeFrameGemChargeBtn");
	GemChargeBtn:DisChecked();
	GemOrVipCharge = 1;
	HideGemCharge();
end
function GemProceChargeFramePowerValBtn_OnEnter()
	SetGameTooltips( this:GetName(), "宝石能量" );
end
function IsGemExist( i )
	local BtnIconTexture = getglobal("GemProceChargeFrameChargeGemBtn"..i.."IconTexture")
	if not string.find( BtnIconTexture:GetTexture(), "TouMingTongDao.tga" ) then
		return true;
	end
	return false;
end
function GemProceChargeFrameChargeActBtn_onClick()
	if LevelUpStopCharge then
		ShowMidTips("已是最高级宝石不可充能");
		return;
	end
	if GemOrVipCharge == 0 and ChargeGemNum <= 0 then
		ShowMidTips("请填入可充能道具");
		return;
	end
	local gemStar,gemPower = SetStarAndPower();
	local MainPlayer	=	ActorMgr:getMainPlayer();
	local Item;
	if ChargeEquipListType == CONTAINER_TYPE_SUNDRIES then
		local container		=	MainPlayer:getContainer();
		Item				=	container:getItem( ChargeEquipListType, ChargeEquipGridIndex );
	elseif ChargeEquipListType == CONTAINER_TYPE_WEAR then
		local equip			=	MainPlayer:getEquip();
		Item				=	equip:getItem( ChargeEquipGridIndex );
	end
	local gemId = ChargeHoleIdx == 0 and Item:getItemId() or Item:getHoleGemId( ChargeHoleIdx - 1 );
	local gemNewDef = GameArmProce:getGemNewDef( gemId );
	local gemLevelStarDef = GameArmProce:getGemLevelStarDef( gemNewDef.Level, gemStar );
	--[[
	if IsPowerOutOf( gemNewDef.Level, gemStar, gemPower ) then
		MessageBox("充能确认","可充能量超过能量值上限，确认消耗所有宝石充能？");
		return;
	end
	--]]

	if GemOrVipCharge == 1 and not GemProceChargeFrameChoseGoldCoupons:GetCheckState() and not Item:isItemBind() then
		MessageBox("绑定提示","使用金券会使宝石（镶嵌宝石的装备）绑定，确定继续充能吗？");
		MessageBoxFrame:SetClientString( "金券充能未绑宝石" )
		return;
	end
	for i = 1, CHARGE_GEM_MAX_NUM do
		if IsGemExist( i ) and t_BtnInfo[i]["isBind"] then
			MessageBox( "宝石充能绑定", "充能材料中包含绑定物品,被充能宝石将会绑定,是否确认" );
			return;
		end
	end
	CallCharge();
end
local MAX_GEM_CHARGE_STAR = 45;
--是否已经到达充能顶级
function IsGemTopLevel( level, star )
	if level < 3 then return 0; end
	for i=1,MAX_GEM_CHARGE_STAR do 
		local gemLevelStarDef = GameArmProce:getGemLevelStarDef( level, i );
		if gemLevelStarDef.PowerUp == 0 then
			return i;	
		end
	end
end
--判断充能是否会溢出
function IsPowerOutOf( gemLevel, gemStar, gemPower )
	local TotalNeedPower = 0 - gemPower;
	local level = gemLevel;
	local star	= gemStar;
	local topStar = IsGemTopLevel( level, star );
	
	if star >= topStar then
		return true;
	else 
		for i=star,topStar do 
			local gemLevelStarDef = GameArmProce:getGemLevelStarDef( level, i );
			TotalNeedPower = TotalNeedPower + gemLevelStarDef.PowerUp;
		end
		if nCurGemsTotalPower > TotalNeedPower then
			return true;
		else
			return false;
		end
	end
end
--发送下一次充能请求
function CallCharge()
	local MainPlayer = ActorMgr:getMainPlayer();
	local money = nCurGemsTotalPower*GameArmProce:getChargePerPowerUseMoney();
	if MainPlayer == nil or MainPlayer:getMoney() < money then
		ShowBuyMoneyFrame();
		return;
	end

	local gemAddPower = GameArmProce:getChargeGemAddPowerInfo();
	gemAddPower.OpType = GemOrVipCharge;
	if GemOrVipCharge == 1 then
		local GoldCouponsCheckBtn = getglobal("GemProceChargeFrameChoseGoldCoupons");
		if GoldCouponsCheckBtn:GetCheckState() then
			if  MainPlayer:getGodCoin() < needMoneyNum then
				ShowMidTips("您的金币不足，无法充能");
				return;
			end
			gemAddPower.UseType = 1;
		else
			if MainPlayer:getBindGodCoin() < needMoneyNum then
				ShowMidTips("您的金券不足，无法充能");
				return;
			end
			gemAddPower.UseType = 0;
		end
	end
	gemAddPower.ArmHoleIdx = ChargeHoleIdx;
	gemAddPower.ArmData.ArmListType = ChargeEquipListType;
	gemAddPower.ArmData.ArmGridIdx = ChargeEquipGridIndex;
	gemAddPower.StuffNum = ChargeGemNum;
	local dataIdx = 0;
	for i = 1, CHARGE_GEM_MAX_NUM do
		if IsGemExist( i ) then
			local btn = getglobal("GemProceChargeFrameChargeGemBtn"..i)
			gemAddPower.StuffData[dataIdx].ItemNum = t_BtnInfo[i]["itemNum"];
			gemAddPower.StuffData[dataIdx].ListType = btn:GetClientUserData( 1 );
			gemAddPower.StuffData[dataIdx].GridIdx = btn:GetClientUserData( 2 );
			gemAddPower.StuffData[dataIdx].ItemID = t_BtnInfo[i]["itemId"];
			dataIdx = dataIdx + 1;
		end
	end
	gemAddPower.StuffNum = dataIdx;
	GameArmProce:chargeGem( gemAddPower );
	CanPlayAnim = true;
	GemProceChargeFrameChargeActBtn:SetGray( true );
	GemProceChargeFrameChargeActBtn:Disable();
end
function GemProceChargeFrameWarningBtn_onEnter()
	SetGameTooltips( this:GetName(), "1.在宝石框中放入可充能道具,可以给当前宝石充能,提升宝石星级\n2.宝石能量充满会增加一颗星,提升宝石属性\n3.可以充能的宝石,镶嵌在装备上也能继续充能" );
end

---------------------------------------------------背饰幻化----------------------------------------------
local szUnRealBackDesc = [[#cffffff☆ 目标背饰必须是可进阶背饰
#cffffff☆ 幻化不限制次数，幻化效果将持续至幻化
#cffffff  背饰有效期结束
#cffffff☆ 幻化只会消耗银币，背饰进阶后幻化效果
#cffffff  会消失
#cffffff☆ 幻化成功后，幻化背饰与目标背饰都会与
#cffffff  角色绑定]]
function OpenUnRealBack( npcId )
	ClearItemLock( "UnRealBackFrame" )
	UnRealBackFrameDescRich:SetText( szUnRealBackDesc, 255,255,255 )
	UnRealBackFrame:Show()
	util.testNpcDialogDistance( npcId, UnRealBackFrame )
end
function SaveToUnRealBack( item )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local itemDef = item:getItemDef()
	--是否来自背包
	if item:getListType() ~= CONTAINER_TYPE_SUNDRIES then
		ShowMidTips( "只有背包中的背饰才能放入" )
		return
	end
	if itemDef.WearPos1 ~= EQUIP_POS_FASHION_BACK then
		ShowMidTips( "非时装背饰，不能放入" )
		return
	end
		
	local btn = getglobal( "UnRealBackFrameSrcItemBtn" )
	if bitAnd( itemDef.CtrlFlag, ITEM_CAN_STEP_UP_BACK_TOLUA ) then
		btn = getglobal( "UnRealBackFrameDestItemBtn" )
	end
	--如果之前有物品的话，先取消掉
	if btn:GetClientUserData(1) >= 0 and btn:GetClientUserData(2) >= 0 then		
		local oldItem = container:getItem( btn:GetClientUserData(1),btn:GetClientUserData(2) );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() )
		end
	end
	btn:SetClientUserData( 0, item:getItemId() )
	btn:SetClientUserData( 1, item:getListType() )
	btn:SetClientUserData( 2, item:getGridIdx() )
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	LockCurItem( item, btn:GetParent() );
	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	btnTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex", item );
end
function UnRealBackOnRightClick( szBtnName )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local btn = getglobal( szBtnName )
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	--如果之前有物品的话，先取消掉
	if btn:GetClientUserData(1) >= 0 and btn:GetClientUserData(2) >= 0 then		
		local oldItem = container:getItem( btn:GetClientUserData(1),btn:GetClientUserData(2) );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() )
		end
	end
	btnTex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	SetEquipImportantEffectUV( nil, btn:GetName().."UVAnimationTex");
	btn:SetClientUserData( 0, 0 )
	btn:SetClientUserData( 1, 0 )
	btn:SetClientUserData( 2, 0 )
end
function UnRealBackFrameOnDrag( szBtnName )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local btn = getglobal( szBtnName );
	local item = container:getItem( btn:GetClientUserData(1), btn:GetClientUserData(2) )
	--如果之前有物品的话，则可以拖拽
	if item:getItemId() > 0 then
		local icon = getglobal( szBtnName.."IconTexture" );
		UIMSG:getUIGV().DragFrom = CONTAINER_TYPE_SUNDRIES;
		UIBeginDrag( icon:GetTexture(), btn:GetClientUserData( 2 ), 0, 0 );
		UnRealBackOnRightClick( szBtnName )
	else
		return
	end
end
--											srcframeType, srcGrid,
function UnRealBackFrameReceiveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local srcList = dragfrom;
	local srcGrid = dragdata1;
	local item = container:getItem( srcList, srcGrid )
	if item:getItemId() > 0 then
		local itemDef = item:getItemDef()
		if bitAnd( itemDef.CtrlFlag, ITEM_CAN_STEP_UP_BACK_TOLUA ) and szBtnName == "UnRealBackFrameSrcItemBtn" then
			ShowMidTips( "请放入不可升阶背饰" );
			return;
		elseif szBtnName == "UnRealBackFrameDestItemBtn" and not bitAnd( itemDef.CtrlFlag, ITEM_CAN_STEP_UP_BACK_TOLUA ) then
			ShowMidTips( "只能填入可升阶背饰" );
			return;
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		SaveToUnRealBack( item );
	end
end
function UnRealBackFrame_OnLoad()
	ItemFirstCallBack( "UnRealBackFrame", SaveToUnRealBack )
end
function UnRealBackFrame_OnHide()
	UnRealBackOnRightClick( "UnRealBackFrameSrcItemBtn" )
	UnRealBackOnRightClick( "UnRealBackFrameDestItemBtn" )
	ClearItemLock( "UnRealBackFrame" )
	PlayerContainerFrame:Hide()
end
function UnRealBackFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local srcBtn = getglobal( "UnRealBackFrameSrcItemBtn" )
	local desBtn = getglobal( "UnRealBackFrameDestItemBtn" )
	local srcItem = container:getItem( srcBtn:GetClientUserData(1), srcBtn:GetClientUserData(2) )
	local desItem = container:getItem( desBtn:GetClientUserData(1), desBtn:GetClientUserData(2) )
	local needMoney = 0
	if srcItem:getItemId() > 0 and desItem:getItemId() > 0 then
		needMoney = math.ceil( ( mainplayer:getLv()*mainplayer:getLv() )/100 )*100
	end	
	UnRealBackFrameNeedMoneyFont:SetText( needMoney )
end
function UnRealBackFrame_OnShow()
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show()
	end
	PlayerContainerFrame:SetPoint("center", "UIClient", "center", 200, 0)
	this:SetPoint( "topright", "PlayerContainerFrame", "topleft", -10, 0 );
end
function UnRealBackFrameButton_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local list = this:GetClientUserData( 1 )
	local grid = this:GetClientUserData( 2 )
	local item = container:getItem( list, grid )
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	end
end
function UnRealBackFrameOkBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	local srcBtn = getglobal( "UnRealBackFrameSrcItemBtn" )
	local desBtn = getglobal( "UnRealBackFrameDestItemBtn" )
	local srcItem = container:getItem( srcBtn:GetClientUserData(1), srcBtn:GetClientUserData(2) )
	local desItem = container:getItem( desBtn:GetClientUserData(1), desBtn:GetClientUserData(2) )
	if srcItem:getItemId() > 0 and desItem:getItemId() > 0 then
		GameArmProce:requestUnRealBack( srcBtn:GetClientUserData(1), srcBtn:GetClientUserData(2), desBtn:GetClientUserData(1), desBtn:GetClientUserData(2) )
		return
	elseif srcItem:getItemId() <= 0 then
		ShowMidTips( "请放入幻化背饰" )
		return
	elseif desItem:getItemId() <= 0 then
		ShowMidTips( "请放入目标背饰" )
		return
	end
end
function UnRealBackResult( nFlag )
	if nFlag == 1 then
		UnRealBackOnRightClick( "UnRealBackFrameSrcItemBtn" )
		UnRealBackFrameModelView:Show();
		UnRealBackFrameModelView:setPetModel(0,0,"effect\\entity\\pinzhizhuanyi.ent");
		UnRealBackFrameModelView:setEntityPosition(0,0,55,0);
	end
end
