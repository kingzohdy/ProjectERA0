local MAX_PET_LIST_NUM	= 5;
local StarMoveIndex		= 0;
local EatIndex			= 0;
local MAX_PET_STRENGTHEN_LEVELS = 12;
local t_AttrMoveTips 	= {
					[1] = [[
#cffffce转星说明：#n
1.#cff0000【目标宠物】#n获得#G【转星宠物】#n的强化星级和强化属性
2.#G【转星宠物】#n将失去强化星级和强化属性
3.#G【转星宠物】#n至少需要拥有#Y1颗#n强化星级]],
					[2] = [[
#cffffce吞噬说明：#n
1.#cff6400【主宠物】#n继承#cff0000【被吞噬宠物】#n部分经验和属性
2.#cff6400【主宠物】#n将晋升为#Y不离不弃#n关系，所有基础属性提高#G10%#n
3.#cff0000【被吞噬宠物】#n将被#cff6400【主宠物】#n吞噬消失
4.#cff0000【被吞噬宠物】#n需要宠物资质至少达到#G良好#n]],
					[3] = [[
#cffffce宠物进阶条件：#n
#G1、宠物等级≥60，资质达到优秀、杰出、完美#n
#G2、拥有战宠越多，进阶需要的进阶果实越多#n
#Y进阶后，宠物心情会变成不离不弃（不可交易）#n]],
					[4] = [[
#Y宠物进化说明：#n
#Y 进化后，宠物心情会变成不离不弃（不可交易）#n]],
					 	}
local t_StarMovePet = { srcPet = 0, desPet = 0, oldBtnId = 0, srcWidHight = 0, srcWidLow = 0, desWidHight = 0,desWidLow = 0 };
local t_EatPet		= { srcPet = 0, desPet = 0, oldBtnId = 0, srcWidHight = 0, srcWidLow = 0, desWidHight = 0,desWidLow = 0 };


function UpdatePetListInfo( frameName, Index )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	
	for i = 1, MAX_PET_LIST_NUM do 
		local btn = getglobal( frameName.."PetBtn"..i );
		btn:Hide();
	end
	-- petPackage:getPetNum()
	for i = 1, MAX_PET_LIST_NUM do
		local btn 		= getglobal( frameName.."PetBtn"..i );
		local petInfo 	= petPackage:getPetInfo( Index + i - 1 );
		if petInfo == nil then 
			break;
		end
		local petDef	= getPetDef( petInfo.PetID );
		-- 宠物头像
		local tex		= getglobal( btn:GetName().."Icon" );
		local texPath	= GetMonsterIconPath( petDef.IconID ) ;
		tex:SetTexture( texPath );
		-- 宠物名字 Name
		local font		= getglobal( btn:GetName().."Font");
		font:SetText( petInfo.Name );
		btn:Show();
		btn:DisChecked();
		btn:SetClientUserData( 0 , petInfo.PetID );
		btn:SetClientUserData( 1, Index + i );
		local rich = getglobal( btn:GetName().."Rich");
		rich:Hide();
	end
end
	 	
function AttrMovePetBtn_OnClick()
	local player = ActorMgr:getMainPlayer();
	local petMgr = player:getPet();
	if arg1 == "LeftButton" then
		local i = this:GetClientID() + StarMoveIndex;
		local info = petMgr:getPetInfo(i - 1)
		if info then
			if not PetFrame:IsShown() then 
				PetFrame:Show();
			end
			PetListButtonSelect(i - 1)
		end
	elseif arg1 == "RightButton" then
		if petMgr:isPetInState( this:GetClientUserData( 1 ) - 1, CLIENT_PET_STAT_FIGHT ) then 
			ShowMidTips("该宠物为召唤状态，无法进行该操作");
			return;
		end
		SetPetStarMoveInfo( this:GetClientUserData( 0 ) , this:GetClientUserData( 1 ) );
	end
end
function EatAttrMovePetBtn_OnClick()
	local player = ActorMgr:getMainPlayer();
	local petMgr = player:getPet();
	if arg1 == "LeftButton" then
		local i = this:GetClientID() + EatIndex;
		local info = petMgr:getPetInfo(i - 1)
		if info then
			if not PetFrame:IsShown() then 
				PetFrame:Show();
			end 
			PetListButtonSelect(i - 1)
		end
	elseif arg1 == "RightButton" then
		if petMgr:isPetInState( this:GetClientUserData( 1 ) - 1, CLIENT_PET_STAT_FIGHT ) then 
			ShowMidTips("该宠物为召唤状态，无法进行该操作");
			return;
		end
		SetPetAttrEatInfo( this:GetClientUserData( 0 ) , this:GetClientUserData( 1 ) );
	end
end

function PetListBtnOnEnter()
	local szText = "左键点击查看/右键点击选取";
	SetGameTooltips(this:GetName(),szText);
end
---------------------宠物 转星------------------------
local srcPetValue = 0;
local desPetValue = 0;

function PetStarMoveSucc( nSucc )
	if nSucc == 1 then 
		SetStarMoveFrameSrcPetInfo();
		ShowMidTips("您的宠物转星成功！");
		local mainplayer 	= ActorMgr:getMainPlayer();
		local petPackage 	= mainplayer:getPet();
		local PetInfo 		= petPackage:getPetInfo( t_StarMovePet["desPet"] - 1 );
		if PetInfo == nil then 
			return;
		end
		local nStrenValue = 0;
		for i = 1,MAX_PET_STRENGTHEN_LEVELS do 
			nStrenValue = nStrenValue + petPackage:getStrthenVal( PetInfo, i - 1 );
		end
		PetStarMoveFrameDesNameFont:SetText( "星级属性：#+"..nStrenValue.."%",160,160,90 );
		local modelview = getglobal("PetStarMoveFrameDesPetBtnModelView");
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		modelview:Show()
	end
end
function PetStarMoveFrame_OnLoad()
	this:RegisterEvent("GE_DEL_PET");
	this:RegisterEvent("GE_PET_ATTR_CHG");
	this:RegisterEvent("GE_GET_NEW_PET");
	local tipRich = getglobal("PetStarMoveFrameChooseRich");	
	tipRich:setCenterLine( true );
	PetStarMoveFrameDesNameFont:setCenterLine( true );
	PetStarMoveFrameSrcNameFont:setCenterLine( true );
end

function PetStarMoveFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	if(arg1 == "GE_DEL_PET") then
		UpdatePetListInfo( this:GetName(), StarMoveIndex );
		local srcIndex = GetMovePetIndex( t_StarMovePet["srcPet"], t_StarMovePet["srcWidHight"],t_StarMovePet["srcWidLow"]);
		local desIndex = GetMovePetIndex( t_StarMovePet["desPet"], t_StarMovePet["desWidHight"],t_StarMovePet["desWidLow"]);
		t_StarMovePet["srcPet"] = srcIndex;
		t_StarMovePet["desPet"] = desIndex;
		t_StarMovePet["oldBtnId"] = desIndex;
		UpdatePetStarMoveInfo( srcIndex,desIndex );
		UpdateStarMovePetChooseTips();
	end 
	if(arg1 == "GE_PET_ATTR_CHG") or (arg1 == "GE_GET_NEW_PET") then
		UpdatePetListInfo( this:GetName(), StarMoveIndex );
		UpdatePetStarMoveInfo( t_StarMovePet["srcPet"],t_StarMovePet["desPet"] );
		UpdateStarMovePetChooseTips();
	end
end
function  GetMovePetIndex( nPetIndex, nHightID, nLowID )
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if petPackage:getPetNum() <=  MAX_PET_LIST_NUM then 
		StarMoveIndex 	= 0;
		EatIndex		= 0;
	end
	if nPetIndex == 0 then 
		return 0;
	else 
		for i = 1, petPackage:getPetNum() do
			local petInfo = petPackage:getPetInfo( i - 1 );
			if petInfo.WID.nHeight == nHightID and petInfo.WID.nLow == nLowID then 
				return i;
			end
		end
	end
	return 0;
end

function UpdatePetStarMoveInfo( srcIndex, desIndex )
	local srcBtnID	= srcIndex - StarMoveIndex;
	local srcPetBtn = getglobal( "PetStarMoveFramePetBtn"..srcBtnID );
	
	if srcPetBtn ~= nil then 
		local srcFont	= getglobal( srcPetBtn:GetName().."Font" );
		local srcTex	= getglobal( srcPetBtn:GetName().."Icon" );	
		local srcRich 	= getglobal( srcPetBtn:GetName().."Rich");
		srcRich:setCenterLine( true );
		srcRich:SetText("转星宠物",0,255,0);
		srcRich:Show(); 
		PetStarMoveFrameSrcPetBtnIcon:SetTexture( srcTex:GetTexture() );
		srcPetBtn:Checked();
	else 
		PetStarMoveFrameSrcPetBtn:Hide();
	end
	local desBtnID	= desIndex - StarMoveIndex;
	local desPetBtn = getglobal( "PetStarMoveFramePetBtn"..desBtnID );
	if desPetBtn ~= nil then 
		local desFont	= getglobal( desPetBtn:GetName().."Font" );
		local desTex	= getglobal( desPetBtn:GetName().."Icon" );
		local desRich 	= getglobal( desPetBtn:GetName().."Rich");
		desRich:setCenterLine( true );
		desRich:SetText( "目标宠物",255,0,0 );
		desRich:Show(); 	 
		PetStarMoveFrameDesPetBtnIcon:SetTexture( desTex:GetTexture() );
		desPetBtn:Checked();
	else 
		PetStarMoveFrameDesPetBtn:Hide();
	end
end
local t_StarMoveCloseFrame = { "PetGrowResetFrame", "Pet_DevelopmentBuildUpFrame","PetAttrEatFrame","PetStepUpFrame"}
function PetStarMoveFrame_OnShow()
	PetStarMoveFrameScrollDownBtn:SetAngle( 180 );
	PetStarMoveFrameTipRich:SetText( t_AttrMoveTips[1],160,155,130 );
	StarMoveIndex = 0;
	t_StarMovePet["srcPet"] 		= 0;
	t_StarMovePet["desPet"] 		= 0;
	t_StarMovePet["oldBtnId"] 		= 0;
	UpdatePetListInfo( this:GetName(), StarMoveIndex );
	PetStarMoveFrameSrcPetBtn:Hide();
	PetStarMoveFrameDesPetBtn:Hide();
	PetStarMoveFrameSrcNameFont:SetText("",160,160,90);
	PetStarMoveFrameDesNameFont:SetText("",160,160,90);
	local modelview = getglobal("PetStarMoveFrameDesPetBtnModelView");
	modelview:Hide();
	PetStarMoveFrameCostBtnNumFont:SetText( "0" );
	for _, name in pairs( t_StarMoveCloseFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	for i=1,MAX_PET_LIST_NUM do 
		local rich = getglobal("PetStarMoveFramePetBtn"..i.."Rich")
		rich:setCenterLine( true );
		rich:SetText( "",255,100,0 );
	end
	UpdateStarMovePetChooseTips();
end

function SetPetStarMoveInfo( petID,petIndex )
	local btnID		= petIndex - StarMoveIndex;
	local button	= getglobal( "PetStarMoveFramePetBtn"..btnID );
	local petDef	= getPetDef( petID );
	if petDef == nil then 
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local PetInfo 		= petPackage:getPetInfo( petIndex - 1 );
	if PetInfo == nil then 
		return;
	end
	local nPerStarNum = 0;
	for i = 1,MAX_PET_STRENGTHEN_LEVELS do 
		local nValue = petPackage:getStrthenVal( PetInfo, i - 1 );
		if nValue == 10 then 
			nPerStarNum = nPerStarNum + 1;	
		end
	end
	if t_StarMovePet["srcPet"] == petIndex then 
		ClearStarMoveSrcPetInfo( button );
	elseif t_StarMovePet["desPet"] == petIndex then
		ClearStarMoveDesPetInfo( button );
	elseif t_StarMovePet["desPet"] == 0 then 		
		SetStarMoveDesPetInfo( petID, PetInfo,petIndex);
	elseif t_StarMovePet["srcPet"] == 0  and t_StarMovePet["desPet"] > 0 then
		if PetInfo.StrengthenLevel < 1 then 
			ShowMidTips("“转星宠物”至少需要拥有1颗强化星级");
			return;
		end  
		SetStarMoveSrcPetInfo( petID, PetInfo,petIndex,nPerStarNum );
	else 
		ShowMidTips("取消选中转星的宠物后，才能继续选择");
		return;
	end
	UpdateStarMovePetChooseTips();
end 
function ClearStarMoveSrcPetInfo( button )
	t_StarMovePet["srcPet"] = 0;
	PetStarMoveFrameSrcPetBtnIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	PetStarMoveFrameSrcPetBtnMaskTexture:Hide();
	PetStarMoveFrameSrcPetBtn:Hide();
	PetStarMoveFrameCostBtnNumFont:SetText( "0" );
	button:DisChecked();
	local rich = getglobal(button:GetName().."Rich");
	rich:SetText("",0,255,0 );
end 
function SetStarMoveSrcPetInfo( petID, petInfo,petIndex,nPerStarNum )
	local btnID		= petIndex - StarMoveIndex;
	local button 	= getglobal( "PetStarMoveFramePetBtn"..btnID );
	local petDef	= getPetDef( petID );
	if petDef == nil then 
		return;
	end
	local texPath	= GetMonsterIconPath( petDef.IconID ) ;
	local rich 		= getglobal( button:GetName().."Rich");

	rich:SetText( "转星宠物",0,255,0 );
	rich:Show();
	t_StarMovePet["srcPet"] 		= petIndex;
	t_StarMovePet["srcWidHight"] 	= petInfo.WID.nHeight;
	t_StarMovePet["srcWidLow"] 		= petInfo.WID.nLow;
	PetStarMoveFrameSrcPetBtnIcon:SetTexture( texPath );
	PetStarMoveFrameSrcPetBtn:Show();
	-- 完美星算钱
	if nPerStarNum > 0 then
		local strenDef = getPetStrengthenDef( nPerStarNum );
		PetStarMoveFrameCostBtnNumFont:SetText( strenDef.MoveMoneyVal );
	else 
		PetStarMoveFrameCostBtnNumFont:SetText( "0" );
	end
	button:Checked();
end
function SetStarMoveDesPetInfo( petID, petInfo,petIndex)
	local btnID		= petIndex - StarMoveIndex;
	local button 	= getglobal( "PetStarMoveFramePetBtn"..btnID );
	local petDef	= getPetDef( petID );
	if petDef == nil then 
		return;
	end
	local texPath	= GetMonsterIconPath( petDef.IconID ) ;
	local curRich	= getglobal( button:GetName().."Rich" );

	curRich:SetText( "目标宠物",255,0,0  );
	curRich:Show();
	t_StarMovePet["oldBtnId"] 		= petIndex ;
	t_StarMovePet["desPet"] 		= petIndex;
	t_StarMovePet["desWidHight"] 	= petInfo.WID.nHeight;
	t_StarMovePet["desWidLow"] 		= petInfo.WID.nLow;
	PetStarMoveFrameDesPetBtnIcon:SetTexture( texPath );
	PetStarMoveFrameDesPetBtn:Show();
	button:Checked();
end
function ClearStarMoveDesPetInfo( button )
	PetStarMoveFrameDesPetBtnIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	PetStarMoveFrameDesPetBtnMaskTexture:Hide();
	PetStarMoveFrameDesPetBtn:Hide();
	t_StarMovePet["oldBtnId"] 	= 0;
	t_StarMovePet["desPet"] 		= 0;
	button:DisChecked();
	local rich = getglobal(button:GetName().."Rich");
	rich:SetText( "",255,0,0 );
end
function UpdateStarMovePetChooseTips()
	local tipRich = getglobal("PetStarMoveFrameChooseRich");
	local szText;
	for i=1,MAX_PET_LIST_NUM do 
		local btn = getglobal( "PetStarMoveFramePetBtn"..i );
		btn:SetNormalTextureTemplate("LayerBlackBkgTemplate");
		local tex = getglobal( "PetStarMoveFramePetBtn"..i.."Icon") 
		btn:SetGray(false);
		tex:SetGray(false);
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	setPetStrenthenValue( t_StarMovePet["srcPet"]  , true );
	setPetStrenthenValue( t_StarMovePet["desPet"]  , false );
	if t_StarMovePet["desPet"] == 0 then 
		szText = "#G请从左方列表中选择需要获得星级的目标宠物\n（左键点击查看/右键点击选取）#n"
	elseif t_StarMovePet["srcPet"] == 0 then
		szText = "#G请从左方列表中选择需要转移星级的转星宠物\n（左键点击查看/右键点击选取）#n"
		for i=1,MAX_PET_LIST_NUM do 
			local btn 			= getglobal( "PetStarMoveFramePetBtn"..i );
			local petInfo 		= petPackage:getPetInfo( i - 1 + StarMoveIndex );
			if btn:IsShown() and not btn:IsChecked() and petInfo.StrengthenLevel == 0 then 
				btn:SetNormalTextureTemplate("LayerGrayGlassHighlightTemplate");
			elseif btn:IsShown() and petInfo.StrengthenLevel > 0 then
				btn:SetNormalTextureTemplate("LayerBlackBkgTemplate");
			end
		end
	else
		for i = 1, MAX_PET_LIST_NUM do
			local btn = getglobal( "PetStarMoveFramePetBtn"..i );
			local tex = getglobal( "PetStarMoveFramePetBtn"..i.."Icon") 
			if btn:IsShown() and not btn:IsChecked() then
				btn:SetGray(true);
				tex:SetGray(true)
			end	
		end
		local szPetText = "星级属性："
		if srcPetValue > desPetValue then 
			szPetText = szPetText.."#Y+"..srcPetValue.."%#n" 
			PetStarMoveFrameSrcNameFont:SetText( szPetText,160,160,90 );
		elseif  srcPetValue < desPetValue then
			szPetText = szPetText.."#Y+"..desPetValue.."%#n"
			PetStarMoveFrameDesNameFont:SetText( szPetText,160,160,90 );
		end	
		szText = "#G“目标宠物”与“转星宠物”准备就绪\n（点击转星按钮进行转星）#n"
	end
	tipRich:SetText(szText,18,65,6);	
end

function setPetStrenthenValue( petIndex , flag )
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local nStrenValue 	= 0;
	local szText 		= "";
	local petInfo 		= petPackage:getPetInfo( petIndex - 1 );
	if petIndex > 0 and petInfo then
		for i = 1,MAX_PET_STRENGTHEN_LEVELS do 
			local nValue = petPackage:getStrthenVal( petInfo, i - 1 );
			nStrenValue = nStrenValue + nValue;
		end
		szText = szText.."星级属性：";
		if nStrenValue > 0 then 
			szText = szText.."+"..nStrenValue.."%" 
		else 
			szText = szText..nStrenValue.."%";
		end
	end
	if flag then
		srcPetValue = nStrenValue;
		PetStarMoveFrameSrcNameFont:SetText( szText,160,160,90 );
	else 
		desPetValue = nStrenValue;
		PetStarMoveFrameDesNameFont:SetText( szText,160,160,90 );
	end
end
function PetStarMoveFrameStartBtn_OnClick()
	if 	t_StarMovePet["srcPet"] == 0 then 
		ShowMidTips("请选择转星宠物！")	
		return;
	end
	if  t_StarMovePet["desPet"] == 0 then
		ShowMidTips("请选择目标宠物！")	
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local money 		= tonumber( PetStarMoveFrameCostBtnNumFont:GetText() );
	if money > mainplayer:getMoney() then 
		ShowMidTips("您的银币不足，无法进行转星");
		ShowBuyMoneyFrame();
		return;
	end
	local srcPetInfo 	= petPackage:getPetInfo( t_StarMovePet["srcPet"] - 1 );
	local desPetInfo 	= petPackage:getPetInfo( t_StarMovePet["desPet"] - 1 );
	local srcValue		= 0;
	local desValue		= 0;
	for i = 1,MAX_PET_STRENGTHEN_LEVELS do 
		srcValue = srcValue + petPackage:getStrthenVal( srcPetInfo, i - 1 );
		desValue = desValue + petPackage:getStrthenVal( desPetInfo, i - 1 ); 
	end
	if srcValue < desValue then
		local szText = "#cff0000【目标宠物】#n的星级属性高于#c00ff00【转星宠物】#n\n成功转星后，#cff0000【目标宠物】#n的星级属性将被覆盖";
		
		local confirmText = "点击确定转移宠物星级";
		MessageBoxForPet("宠物转星",szText,confirmText)
		MessageBoxForPetFrame:SetClientString( "宠物转星损失" );
		return;
	end
	local szText = "成功转星后，#c00ff00【转星宠物】#n将失去强化星级和星级属性\n点击确定转移宠物星级";
	MessageBox( "宠物转星",szText);
	MessageBoxFrame:SetClientString( "宠物转星损失" );
end
function PetStarMoveFrameStart()
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local srcPetInfo 	= petPackage:getPetInfo( t_StarMovePet["srcPet"] - 1 );
	local desPetInfo 	= petPackage:getPetInfo( t_StarMovePet["desPet"] - 1 );
	petPackage:startMoveStar( srcPetInfo.WID.nHeight, srcPetInfo.WID.nLow, desPetInfo.WID.nHeight, desPetInfo.WID.nLow);
end
function UpdateMovePetListRich()
	local srcBtnID	= t_StarMovePet["srcPet"] - StarMoveIndex;
	local srcPetBtn = getglobal( "PetStarMoveFramePetBtn"..srcBtnID );
	
	if srcPetBtn ~= nil then 
		local srcRich 	= getglobal( srcPetBtn:GetName().."Rich");
		srcRich:setCenterLine( true );
		srcRich:SetText("转星宠物",0,255,0);
		srcRich:Show(); 
		srcPetBtn:Checked();
	end
	local desBtnID	= t_StarMovePet["desPet"] - StarMoveIndex;
	local desPetBtn = getglobal( "PetStarMoveFramePetBtn"..desBtnID );
	if desPetBtn ~= nil then 
		local desRich 	= getglobal( desPetBtn:GetName().."Rich");
		desRich:setCenterLine( true );
		desRich:SetText( "目标宠物",255,0,0 );
		desRich:Show(); 	 
		desPetBtn:Checked();
	end
end
function PetStarMoveFrameScrollDownBtn_OnClick()
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet()
	local petNum		= petPackage:getPetNum();
	if petNum <= MAX_PET_LIST_NUM then 
		return;
	end

	StarMoveIndex = ( StarMoveIndex + 1 > petNum - MAX_PET_LIST_NUM ) 
				and ( petNum - MAX_PET_LIST_NUM ) 
				or ( StarMoveIndex + 1 )  ;

	UpdatePetListInfo( this:GetParent(), StarMoveIndex );
	UpdateMovePetListRich();
	UpdateStarMovePetChooseTips();
end

function PetStarMoveFrameScrollUpBtn_OnClick()
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet()
	local petNum		= petPackage:getPetNum();
	if petNum <= MAX_PET_LIST_NUM then 
		return;
	end
 
	StarMoveIndex = ( StarMoveIndex - 1 > 0 ) 
			and ( StarMoveIndex - 1 ) 
			or 0  ;

	UpdatePetListInfo( this:GetParent(), StarMoveIndex );
	UpdateMovePetListRich();
	UpdateStarMovePetChooseTips();
end
function PetStarMoveFrameSrcPetCloseBtn_OnClick()
	SetStarMoveFrameSrcPetInfo();
end
function PetStarMoveFrameSrcPetBtn_OnClick()
	if t_StarMovePet["srcPet"] == 0 then 
		return;
	elseif arg1 == "RightButton" then
		SetStarMoveFrameSrcPetInfo();
	end 
end
function SetStarMoveFrameSrcPetInfo()
	local btnID		= t_StarMovePet["srcPet"] - StarMoveIndex;
	local btn 		= getglobal( "PetStarMoveFramePetBtn"..btnID );
	local rich		= getglobal( "PetStarMoveFramePetBtn"..btnID.."Rich");
	t_StarMovePet["srcPet"] = 0;
	PetStarMoveFrameSrcPetBtn:Hide();
	PetStarMoveFrameSrcNameFont:SetText("",160,160,90);
	PetStarMoveFrameCostBtnNumFont:SetText("0");
	UpdateStarMovePetChooseTips();
	if btn == nil then 
		return;
	end
	btn:DisChecked();
	rich:Hide();
end
function PetStarMoveFrameDesPetCloseBtn_OnClick()
	SetStarMoveFrameDesPetInfo();
end
function PetStarMoveFrameDesPetBtn_OnClick()
	if t_StarMovePet["desPet"] == 0 then 
		return;
	elseif arg1 == "RightButton" then
		SetStarMoveFrameDesPetInfo();
	end 
end
function SetStarMoveFrameDesPetInfo()
	local btnID		= t_StarMovePet["desPet"] - StarMoveIndex;
	local btn 		= getglobal( "PetStarMoveFramePetBtn"..btnID );
	local rich		= getglobal( "PetStarMoveFramePetBtn"..btnID.."Rich");
	t_StarMovePet["desPet"] = 0;
	PetStarMoveFrameDesNameFont:SetText("",160,160,90);
	PetStarMoveFrameDesPetBtn:Hide();
	UpdateStarMovePetChooseTips();
	if btn == nil then 
		return;
	end
	btn:DisChecked();
	rich:Hide();
end
---------------------宠物 吞噬------------------------
local eatRate 			= 0;
function PetEatSucc( nSucc )
	if nSucc == 1 then 
		PetAttrEatFrameSrcPetBtn:Hide();
		local mainplayer 	= ActorMgr:getMainPlayer();
		local petPackage 	= mainplayer:getPet();
		local desPetInfo 	= petPackage:getPetInfo( t_EatPet["desPet"] - 1 );
		if desPetInfo ~= nil then
			SetBackGeniusByGenius( "PetAttrEatFrameDesBackTexture" ,desPetInfo.PetGenius );
			SetBackGeniusByGenius( "PetAttrEatFramePreBackTexture" ,desPetInfo.PetGenius );
		end
		
		PetAttrEatFrameCostBtnNumFont:SetText("0");
		local btnID		= t_EatPet["srcPet"] - EatIndex;
		local btn 		= getglobal( "PetAttrEatFramePetBtn"..btnID );
		local rich		= getglobal( "PetAttrEatFramePetBtn"..btnID.."Rich");
		t_EatPet["srcPet"] = 0;
		if btn ~= nil then 
			btn:DisChecked();
			rich:Hide();
		end
		
		ShowMidTips("您的宠物吞噬成功！");
		local modelview = getglobal("PetAttrEatFrameDesPetBtnModelView");
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		modelview:Show()
		UpdateEatPetChooseTips();
	end
end

function PetAttrEatFrame_OnLoad()
	this:RegisterEvent("GE_DEL_PET");
	this:RegisterEvent("GE_PET_ATTR_CHG");
	this:RegisterEvent("GE_GET_NEW_PET");
	local tipRich = getglobal("PetAttrEatFrameChooseRich");
	tipRich:setCenterLine(true)
end

function PetAttrEatFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	if(arg1 == "GE_DEL_PET") then
		UpdatePetListInfo( this:GetName(), EatIndex );
		local srcIndex = GetMovePetIndex( t_EatPet["srcPet"], t_EatPet["srcWidHight"],t_EatPet["srcWidLow"]);
		local desIndex = GetMovePetIndex( t_EatPet["desPet"], t_EatPet["desWidHight"],t_EatPet["desWidLow"]);
		t_EatPet["srcPet"] 		= srcIndex;
		t_EatPet["desPet"] 		= desIndex;
		t_EatPet["oldBtnId"] 	= desIndex ;
		SetEatFrameSrcPetBackTex( );
		UpdatePetAttrEatInfo( srcIndex,desIndex );
		UpdateEatPetChooseTips();
	end 

	if(arg1 == "GE_PET_ATTR_CHG") or (arg1 == "GE_GET_NEW_PET") then
		UpdatePetListInfo( this:GetName(), EatIndex );
		SetEatFrameSrcPetBackTex();
		UpdatePetAttrEatInfo( t_EatPet["srcPet"],t_EatPet["desPet"] );
		UpdateEatPetChooseTips();	
	end
end

function UpdatePetAttrEatInfo( srcIndex, desIndex )
	local srcBtnID	= srcIndex - EatIndex;
	local srcPetBtn = getglobal( "PetAttrEatFramePetBtn"..srcBtnID );
	
	if srcPetBtn ~= nil then 
		local srcTex	= getglobal( srcPetBtn:GetName().."Icon" );	
		local srcRich 	= getglobal( srcPetBtn:GetName().."Rich");

		srcRich:SetText("被吞噬宠物",255,0,0);
		srcRich:Show(); 
		PetAttrEatFrameSrcPetBtnIcon:SetTexture( srcTex:GetTexture() );

		srcPetBtn:Checked();
	else 
		PetAttrEatFrameSrcPetBtn:Hide();
	end
	local desBtnID	= desIndex - EatIndex;
	local desPetBtn = getglobal( "PetAttrEatFramePetBtn"..desBtnID );

	if desPetBtn ~= nil then 
		local desTex	= getglobal( desPetBtn:GetName().."Icon" );
		local desRich 	= getglobal( desPetBtn:GetName().."Rich");
		desRich:SetText( "主宠物",255,100,0 );
		desRich:Show(); 	 
		PetAttrEatFrameDesPetBtnIcon:SetTexture( desTex:GetTexture() );
		PetAttrEatFramePreviewBtnIcon:SetTexture( desTex:GetTexture() );
		desPetBtn:Checked();
	else 
		PetAttrEatFrameDesPetBtn:Hide();
	end
end
local t_EatCloseFrame = { "PetGrowResetFrame", "Pet_DevelopmentBuildUpFrame","PetStarMoveFrame","PetStepUpFrame"}
function  PetAttrEatFrame_OnShow()
	PetAttrEatFrameScrollDownBtn:SetAngle( 180 );
	PetAttrEatFrameTipRich:SetText( t_AttrMoveTips[2],160,155,130 );
	EatIndex = 0;
	UpdatePetListInfo( this:GetName(), EatIndex );
	-- 吞噬表 归零
	t_EatPet["srcPet"] 		= 0;
	t_EatPet["desPet"] 		= 0;
	t_EatPet["oldBtnId"] 	= 0;
	PetAttrEatFrameSrcPetBtn:Hide();
	PetAttrEatFrameDesPetBtn:Hide();

	local modelview = getglobal("PetAttrEatFrameDesPetBtnModelView");
	modelview:Hide();
	PetAttrEatFrameCostBtnNumFont:SetText("0")
	for _, name in pairs( t_EatCloseFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	for i=1,MAX_PET_LIST_NUM do 
		local rich = getglobal("PetAttrEatFramePetBtn"..i.."Rich")
		rich:setCenterLine( true );
		rich:SetText( "",255,100,0 );
	end
	UpdateEatPetChooseTips();
end

function SetPetAttrEatInfo( petID,petIndex )	
	local btnID		= petIndex - EatIndex;
	local button 	= getglobal( "PetAttrEatFramePetBtn"..btnID );

	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	local petInfo 	= petPackage:getPetInfo( petIndex - 1 );
	if petInfo == nil then 
		return;
	end

	if t_EatPet["srcPet"] == petIndex then 
		ClearEatSrcPetInfo( button );
	elseif t_EatPet["desPet"] == petIndex then
		ClearEatDesPetInfo( button );
	elseif t_EatPet["desPet"] == 0 and petInfo.PetGenius == CLIENT_PET_GENIUS_S then 		
		ShowMidTips("该宠物已经达到完美资质，不能吞噬其他宠物增长资质值");
		return;
	elseif t_EatPet["desPet"] == 0 and petInfo.PetGenius < CLIENT_PET_GENIUS_S then 		
		SetEatDesPetInfo( petID, petInfo,petIndex );
	elseif t_EatPet["srcPet"] == 0  and t_EatPet["desPet"] > 0 then
		if petInfo.PetGenius < CLIENT_PET_GENIUS_C then 
			ShowMidTips("“被吞噬宠物”需要宠物资质至少达到良好");
			return;
		end  
		SetEatSrcPetInfo( petID, petInfo,petIndex );
	else
		ShowMidTips("取消选中吞噬的宠物后，才能继续选择");
		return;
	end
	UpdateEatPetChooseTips();
end  

function UpdateEatPetChooseTips()
	local tipRich = getglobal("PetAttrEatFrameChooseRich");
	local szText;
	for i=1,MAX_PET_LIST_NUM do 
		local btn = getglobal( "PetAttrEatFramePetBtn"..i );
		btn:SetNormalTextureTemplate("LayerBlackBkgTemplate");
		local tex = getglobal( "PetAttrEatFramePetBtn"..i.."Icon") 
		btn:SetGray(false);
		tex:SetGray(false);
	end
	if t_EatPet["desPet"] == 0 then 
		szText = "#G请从左方列表中选择需要成长的主宠物\n（左键点击查看/右键点击选取）#n"
		for i=1,MAX_PET_LIST_NUM do 
			local btn 			= getglobal( "PetAttrEatFramePetBtn"..i );
			local mainplayer 	= ActorMgr:getMainPlayer();
			local petPackage 	= mainplayer:getPet();
			local petInfo 		= petPackage:getPetInfo( i - 1 + EatIndex );
			if btn:IsShown() and not btn:IsChecked() and petInfo.PetGenius == CLIENT_PET_GENIUS_S then 
				btn:SetNormalTextureTemplate("LayerGrayGlassHighlightTemplate");
			elseif btn:IsShown() and petInfo.PetGenius < CLIENT_PET_GENIUS_S then
				btn:SetNormalTextureTemplate("LayerBlackBkgTemplate");
			end
		end		
	elseif t_EatPet["srcPet"] == 0 then
		szText = "#G请从左方列表中选择被吞噬宠物\n（左键点击查看/右键点击选取）#n"
		for i=1,MAX_PET_LIST_NUM do 
			local btn 			= getglobal( "PetAttrEatFramePetBtn"..i );
			local mainplayer 	= ActorMgr:getMainPlayer();
			local petPackage 	= mainplayer:getPet();
			local petInfo 		= petPackage:getPetInfo( i - 1 + EatIndex );
			if btn:IsShown() and not btn:IsChecked() and petInfo.PetGenius < CLIENT_PET_GENIUS_C then 
				btn:SetNormalTextureTemplate("LayerGrayGlassHighlightTemplate");
			elseif btn:IsShown() and petInfo.PetGenius > CLIENT_PET_GENIUS_D then
				btn:SetNormalTextureTemplate("LayerBlackBkgTemplate");
			end
		end
	else
		for i = 1, MAX_PET_LIST_NUM do
			local btn = getglobal( "PetAttrEatFramePetBtn"..i );
			local tex = getglobal( "PetAttrEatFramePetBtn"..i.."Icon") 
			if btn:IsShown() and not btn:IsChecked() then
				btn:SetGray(true);
				tex:SetGray(true)
			end	
		end
		szText = "#G“主宠物”与“被吞噬宠物”准备就绪\n（点击吞噬按钮进行吞噬）#n"	
	end
	
	tipRich:SetText(szText,18,65,6);
	SetEatFrameDesPetBackTex();
end
function ClearEatSrcPetInfo( button )
	t_EatPet["srcPet"] 		= 0;
	PetAttrEatFrameSrcPetBtn:Hide();
	PetAttrEatFrameCostBtnNumFont:SetText( "0" );
	button:DisChecked();
	local rich = getglobal(button:GetName().."Rich");
	rich:SetText("",255,0,0 );
end
function SetEatSrcPetInfo( petID, petInfo,petIndex )
	local btnID		= petIndex - EatIndex;
	local button 	= getglobal( "PetAttrEatFramePetBtn"..btnID );
	local petDef	= getPetDef( petID );
	if petDef == nil then 
		return;
	end
	local texPath	= GetMonsterIconPath( petDef.IconID ) ;
	local rich 		= getglobal( button:GetName().."Rich");

	rich:SetText( "被吞噬宠物",255,0,0 );
	rich:Show();
	t_EatPet["srcPet"] 		= petIndex;
	t_EatPet["srcWidHight"] = petInfo.WID.nHeight;
	t_EatPet["srcWidLow"] 	= petInfo.WID.nLow;
	PetAttrEatFrameSrcPetBtnIcon:SetTexture( texPath );
	PetAttrEatFrameSrcPetBtnMaskTexture:Show();
	PetAttrEatFrameSrcPetBtn:Show();

	SetEatFrameSrcPetBackTex();
	SetEatFrameDesPetBackTex();
	-- 吞噬消耗银币
	local geniusDef = getPetGeniusDef( petInfo.PetGenius + 1 );
	PetAttrEatFrameCostBtnNumFont:SetText( geniusDef.EatMoneyVal );
	button:Checked();
end
function ClearEatDesPetInfo( button )
--	PetAttrEatFrameDesPetBtnIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	PetAttrEatFramePreviewBtnIcon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
--	PetAttrEatFrameDesPetBtnMaskTexture:Hide();
	PetAttrEatFramePreviewBtnMaskTexture:Hide();
	PetAttrEatFrameDesPetBtn:Hide();
	t_EatPet["oldBtnId"] 	= 0;
	t_EatPet["desPet"] 		= 0;
	button:DisChecked();
	local rich = getglobal(button:GetName().."Rich");
	rich:SetText( "",255,100,0 );
end
function SetEatDesPetInfo( petID,petInfo,petIndex )
	local btnID		= petIndex - EatIndex;
	local button 	= getglobal( "PetAttrEatFramePetBtn"..btnID );
	local petDef	= getPetDef( petID );
	if petDef == nil then 
		return;
	end
	local texPath	= GetMonsterIconPath( petDef.IconID ) ;

	local curRich	= getglobal( button:GetName().."Rich" );

	curRich:SetText( "主宠物",255,100,0 );
	curRich:Show();
	t_EatPet["oldBtnId"] 	= petIndex ;
	t_EatPet["desPet"] 		= petIndex;
	t_EatPet["desWidHight"] = petInfo.WID.nHeight;
	t_EatPet["desWidLow"] 	= petInfo.WID.nLow;
	PetAttrEatFrameDesPetBtnIcon:SetTexture( texPath );
	PetAttrEatFramePreviewBtnIcon:SetTexture( texPath );
	PetAttrEatFramePreviewBtnMaskTexture:Show();
	PetAttrEatFrameDesPetBtn:Show();
	PetAttrEatFramePreviewBtn:Show();
	SetEatFrameDesPetBackTex();
	button:Checked();
end
-- 设置宠物头像-资质背景
function SetEatFrameSrcPetBackTex()
	if t_EatPet["srcPet"] == 0 then 
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local petInfo 		= petPackage:getPetInfo( t_EatPet["srcPet"] - 1 );
	local petBackTex	= getglobal( "PetAttrEatFrameSrcBackTexture" );
	if petInfo.PetGenius == CLIENT_PET_GENIUS_S then 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Cheng2.tga" );
		eatRate = 1;
	elseif petInfo.PetGenius == CLIENT_PET_GENIUS_A then 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Zi2.tga" );
		eatRate = 0.8;
	elseif petInfo.PetGenius == CLIENT_PET_GENIUS_B then 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Lan2.tga" );
		eatRate = 0.5;
	else 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Lv2.tga" );
		eatRate = 0.2;
	end
end
local ALL_MAX_PET_LEVEL = 120;
local t_AttrUpColor = { 
						{ upColor = "G"},
						{ upColor = "cffffce"}
					}
function SetEatFrameDesPetBackTex()
	if t_EatPet["desPet"] == 0 then 
		PetAttrEatFramePreviewBtnlevelRich:SetText("等级:\n\n资质:",255,255,190)
		PetAttrEatFramePreviewBtnGrowRich:SetText("攻击成长：\n生命成长：\n物防成长：\n魔防成长：\n命中成长：\n闪避成长：",255,255,190)
		return;
	end

	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local desPetInfo 	= petPackage:getPetInfo( t_EatPet["desPet"] - 1 );
	local desGenius		= desPetInfo.PetGenius;
	local desPetDef 	= getPetDef( desPetInfo.PetID );
	local desImp		= desPetDef.Important;
	--设置主宠物-资质背景
	SetBackGeniusByGenius( "PetAttrEatFrameDesBackTexture" ,desPetInfo.PetGenius );
	-- 主宠物资质为 完美时 不能吞噬
	if desGenius == CLIENT_PET_GENIUS_S then 
		PetAttrEatFramePreviewBtnlevelRich:SetText("等级:\n\n资质:",255,255,190)
		PetAttrEatFramePreviewBtnGrowRich:SetText("攻击成长：\n生命成长：\n物防成长：\n魔防成长：\n命中成长：\n闪避成长：",255,255,190)
		return;
	end
	-- 没有被吞噬宠物 则预览资质同主宠物
	if t_EatPet["srcPet"] == 0 then 
		SetBackGeniusByGenius( "PetAttrEatFramePreBackTexture" ,desPetInfo.PetGenius );
		PetAttrEatFramePreviewBtnlevelRich:SetText("等级:"..desPetInfo.Level.."\n\n资质:#c"..t_geniusDesc[desPetInfo.PetGenius]["hexiColor"]..t_geniusDesc[desPetInfo.PetGenius]["value"].."#n",255,255,190)
		PetAttrEatFramePreviewBtnGrowRich:SetText("攻击成长：\n生命成长：\n物防成长：\n魔防成长：\n命中成长：\n闪避成长：",255,255,190)
		return;
	end
	
	local srcPetInfo 	= petPackage:getPetInfo( t_EatPet["srcPet"] - 1 );
	local preGenius 	= desPetInfo.PetGenius;

	local petDef		= getPetDef( srcPetInfo.PetID );
	local srcPetGenius	= srcPetInfo.PetGenius;

	local srcImp	= petDef.Important;
	local szText = "";

	-- 被吞噬宠物资质值
	local srcImpUpAttrDef = getPetImpUpAttrDef( srcPetInfo.Imp,petDef.PetAttrType)
	local t_SrcPetGeniusValue = {
		{ curValue = srcPetInfo.PhsicAtkGenius, minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].PhsicAtkGenius, 	flag = "攻击成长"}, 
		{ curValue = srcPetInfo.HPGenius, 		minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].HPGenius, 		flag = "生命成长"},
		{ curValue = srcPetInfo.DefGenius,		minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].DefGenius,		flag = "物防成长"},
		{ curValue = srcPetInfo.IceDefGenius, 	minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].IceDefGenius,		flag = "魔防成长"},
		{ curValue = srcPetInfo.HitRateGenius, 	minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].HitRateGenius,	flag = "命中成长"},
		{ curValue = srcPetInfo.FleeGenius, 	minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].FleeGenius,		flag = "闪避成长"}	
						}		
	
	for i = preGenius ,CLIENT_MAX_PET_GENIUS - 1 do
		-- 当前主宠物资质值 
		if i >= CLIENT_PET_GENIUS_S then 
			break;
		end
		local nValueRange = petPackage:getPetBlinkMinRange( i, desPetInfo.PetID, desPetInfo.Imp );
		local t_DesPetGeniusCompareValue = {
				{ curValue = desPetInfo.PhsicAtkGenius, minValue = nValueRange.Min.Attack},
				{ curValue = desPetInfo.HPGenius, 		minValue = nValueRange.Min.HP},
				{ curValue = desPetInfo.DefGenius,		minValue = nValueRange.Min.Defend},
				{ curValue = desPetInfo.IceDefGenius, 	minValue = nValueRange.Min.Resist},
				{ curValue = desPetInfo.HitRateGenius, 	minValue = nValueRange.Min.Exact},
				{ curValue = desPetInfo.FleeGenius, 	minValue = nValueRange.Min.Dodge}
							}
		local geniusNum = 0;
		for j = 1, #t_SrcPetGeniusValue do 
			local nEatRate 	= getCaculatePetEatRate( desGenius,srcPetGenius,desImp,srcImp ,j);
			if t_DesPetGeniusCompareValue[j]["minValue"] - t_DesPetGeniusCompareValue[j]["curValue"] 
			< ( t_SrcPetGeniusValue[j]["curValue"] - t_SrcPetGeniusValue[j]["minValue"] ) * nEatRate then 
				geniusNum = geniusNum + 1;
			else 	
				break;	
			end
		end
		-- 六项资质值都满足条件
		if geniusNum == 6 then 
			preGenius = i + 1;
		else 
			break;
		end
	end
	SetBackGeniusByGenius( "PetAttrEatFramePreBackTexture",preGenius);
	local nExp 		= 0;
	local nLevel 	= desPetInfo.Level;
	if srcPetInfo.Level > 1 then
		for i =  1 , srcPetInfo.Level - 1 do 
			local petLevelDef = getPetLevelDef( i );
			nExp = nExp + petLevelDef.LevelUpExp;
		end
		nExp = ( srcPetInfo.Exp + nExp ) * 0.3 + desPetInfo.Exp;
	elseif srcPetInfo.Level == 1 then 
		nExp = srcPetInfo.Exp  * 0.3 + desPetInfo.Exp;
	end
	local nLevelValue = 0;
	for k = nLevel, ALL_MAX_PET_LEVEL do 	
		local petLevelDef = getPetLevelDef( k);
		nLevelValue = petLevelDef.LevelUpExp + nLevelValue;
		if nExp < nLevelValue or k > mainplayer:getLv() + 10 then 
			nLevel = k;
			break;
		end	
	end
	for j = 1, #t_SrcPetGeniusValue do 
		local nEatRate 	= getCaculatePetEatRate( desGenius,srcPetGenius,desImp,srcImp ,j );
		nUpValue 	= math.floor( ( t_SrcPetGeniusValue[j]["curValue"] - t_SrcPetGeniusValue[j]["minValue"] ) * nEatRate );
--		print("吞噬基础值相关",t_SrcPetGeniusValue[j]["curValue"],t_SrcPetGeniusValue[j]["minValue"] )
		nColorIndex = nUpValue > 0 and 1 or 2;
		if nUpValue > 0  then 
			szText = szText.."\n"..t_SrcPetGeniusValue[j]["flag"].."：#"..t_AttrUpColor[nColorIndex]["upColor"].."+"..nUpValue.."#n";
		else 
			szText = szText.."\n"..t_SrcPetGeniusValue[j]["flag"].."：#"..t_AttrUpColor[nColorIndex]["upColor"].."0#n";
		end
	end
	local levelText = "";
	if nLevel > desPetInfo.Level then 
		levelText = levelText.."等级:#G"..nLevel.."#n\n\n资质:#c"..t_geniusDesc[preGenius]["hexiColor"]..t_geniusDesc[preGenius]["value"].."#n"	
	else 
		levelText = levelText.."等级:"..nLevel.."\n\n资质:#c"..t_geniusDesc[preGenius]["hexiColor"]..t_geniusDesc[preGenius]["value"].."#n"	
	end
	PetAttrEatFramePreviewBtnlevelRich:SetText( levelText, 255,255,190)
	PetAttrEatFramePreviewBtnGrowRich:SetText(szText,255,255,190)
end

function getCaculatePetEatRate( desGenius,srcPetGenius,desImp,srcImp,nIndex ) 
	local nRate1 = 1;
	local nRate2 = 1;
	local nRate	= 1;
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local srcPetInfo = petPackage:getPetInfo( t_EatPet["srcPet"] - 1 );
	local desPetInfo = petPackage:getPetInfo( t_EatPet["desPet"] - 1 );
	local srcMonyDef = getPetMoneyDef(srcImp);
	local desMonyDef = getPetMoneyDef(desImp);
	local desPetDef  = getPetDef( desPetInfo.PetID );
	local srcPetDef	 = getPetDef( srcPetInfo.PetID );
	local desImpUpAttrDef = getPetImpUpAttrDef( desPetInfo.Imp,desPetDef.PetAttrType)
	-- 主宠物资质值
	local t_DesPetGeniusValue = { 
				{ minValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].PhsicAtkGenius, 		nValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].PhsicAtkGenius },
				{ minValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].HPGenius, 				nValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].HPGenius},
				{ minValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].DefGenius,				nValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].DefGenius},
				{ minValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].IceDefGenius, 			nValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].IceDefGenius},
				{ minValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].HitRateGenius, 		nValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].HitRateGenius},
				{ minValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].FleeGenius, 			nValue = desImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].FleeGenius}	
						}
	if desMonyDef.Count > 0 then
		nRate1 = ( t_DesPetGeniusValue[nIndex]["nValue"] * 1.36 - t_DesPetGeniusValue[nIndex]["minValue"] ) / desMonyDef.Count;
	end
--	print(" 目标宠物 相关",t_DesPetGeniusValue[nIndex]["nValue"],t_DesPetGeniusValue[nIndex]["minValue"],desMonyDef.Money[desGenius + 1],desMonyDef.Money[desGenius])
	local srcImpUpAttrDef = getPetImpUpAttrDef( srcPetInfo.Imp,srcPetDef.PetAttrType)
	local t_SrcPetGeniusValue = {
		{ minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].PhsicAtkGenius, 	nValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].PhsicAtkGenius},  
		{ minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].HPGenius, 			nValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].HPGenius},
		{ minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].DefGenius,			nValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].DefGenius},
		{ minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].IceDefGenius, 		nValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].IceDefGenius},
		{ minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].HitRateGenius, 	nValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].HitRateGenius},
		{ minValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_D].FleeGenius, 		nValue = srcImpUpAttrDef.Genius[CLIENT_PET_GENIUS_S].FleeGenius}	
				}
	nRate2 = srcMonyDef.Count / ( t_SrcPetGeniusValue[nIndex]["nValue"] * 1.36 - t_SrcPetGeniusValue[nIndex]["minValue"] );
--	print("被吞噬普通宠物相关",srcMonyDef.Count,t_SrcPetGeniusValue[nIndex]["nValue"],t_SrcPetGeniusValue[nIndex]["minValue"]);
	nRate = nRate1 * nRate2 * srcMonyDef.Money[srcPetGenius] / 10000;
	return nRate;
end

function SetBackGeniusByGenius( TextureName, Genius )
	local petBackTex	= getglobal( TextureName );
	if Genius == CLIENT_PET_GENIUS_S then 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Cheng.tga" );
	elseif Genius == CLIENT_PET_GENIUS_A then 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Zi.tga" );
	elseif Genius == CLIENT_PET_GENIUS_B then 
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Lan.tga" );
	elseif Genius == CLIENT_PET_GENIUS_C then
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Lv.tga" );
	else
		petBackTex:SetTexture( "uires\\TuPianLei\\ChongWuDiWen\\Bai.tga" );
	end
end

-- 吞噬开始
function PetAttrEatFrameStartBtn_OnClick()
	if 	t_EatPet["srcPet"] == 0 then 
		ShowMidTips("请选择被吞噬宠物！")	
		return;
	end
	if  t_EatPet["desPet"] == 0 then
		ShowMidTips("请选择主宠物！")	
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local money 		= tonumber( PetAttrEatFrameCostBtnNumFont:GetText() );
	if money > mainplayer:getMoney() then 
		ShowMidTips("您的金钱不足，无法进行吞噬");
		return;
	end
	local srcPetInfo 	= petPackage:getPetInfo( t_EatPet["srcPet"] - 1 );
	local desPetInfo 	= petPackage:getPetInfo( t_EatPet["desPet"] - 1 );
	local srcStarValue	= srcPetInfo.StrengthenLevel;
	if desPetInfo.PetGenius == CLIENT_PET_GENIUS_S then 
		ShowMidTips("您的宠物已经达到完美资质，不能吞噬其他宠物增长资质值");
		return;
	end
	if srcStarValue > 0 then
		local szString = "成功吞噬后，#cff6400【主宠物】#n晋升为#Y不离不弃#n关系\n#cff0000【被吞噬宠物】#n有强化星级，将会被吞噬消失";
		local okText = "点击确定吞噬宠物";
		MessageBoxForPet("宠物吞噬",szString,okText);
		MessageBoxForPetFrame:SetClientString( "宠物吞噬损失" );
		return;
	end
	local nDesGenius = desPetInfo.PetGenius;
	local nSrcGenius = srcPetInfo.PetGenius;
	local desPetDef	= getPetDef( desPetInfo.PetID );
	local srcPetDef = getPetDef( srcPetInfo.PetID );
	local szText = "成功吞噬后，#cff6400【主宠物】#n晋升为#Y不离不弃#n关系\n成功吞噬后，#cff0000【被吞噬宠物】#n将会被吞噬消失";
	
	local confirmText = "点击确定吞噬宠物";
	MessageBoxForPet("宠物吞噬",szText,confirmText)
	MessageBoxForPetFrame:SetClientString( "宠物吞噬损失" );
end 
function PetAttrEatFrameStart()
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet();
	local srcPetInfo 	= petPackage:getPetInfo( t_EatPet["srcPet"] - 1 );
	local desPetInfo 	= petPackage:getPetInfo( t_EatPet["desPet"] - 1 );
	petPackage:startEatPet( srcPetInfo.WID.nHeight, srcPetInfo.WID.nLow, desPetInfo.WID.nHeight, desPetInfo.WID.nLow);
end
function UpdateEatPetListRich()
	local srcBtnID	= t_EatPet["srcPet"] - EatIndex;
	local srcPetBtn = getglobal( "PetAttrEatFramePetBtn"..srcBtnID );
	
	if srcPetBtn ~= nil then 
		local srcRich 	= getglobal( srcPetBtn:GetName().."Rich");
		srcRich:setCenterLine( true );
		srcRich:SetText("被吞噬宠物",255,0,0);
		srcRich:Show(); 
		srcPetBtn:Checked();
	end
	local desBtnID	= t_EatPet["desPet"] - EatIndex;
	local desPetBtn = getglobal( "PetAttrEatFramePetBtn"..desBtnID );

	if desPetBtn ~= nil then 
		local desRich 	= getglobal( desPetBtn:GetName().."Rich");
		desRich:setCenterLine( true );
		desRich:SetText( "主宠物",255,100,0 );
		desRich:Show(); 	 
		desPetBtn:Checked();
	end
end 

function PetAttrEatFrameScrollDownBtn_OnClick()
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet()
	local petNum		= petPackage:getPetNum();
	if petNum <= MAX_PET_LIST_NUM then 
		return;
	end
	EatIndex = ( EatIndex + 1 > petNum - MAX_PET_LIST_NUM ) 
				and ( petNum - MAX_PET_LIST_NUM ) 
				or ( EatIndex + 1 )  ;

	UpdatePetListInfo( this:GetParent(), EatIndex );
	UpdateEatPetListRich();
	UpdateEatPetChooseTips();
end

function PetAttrEatFrameScrollUpBtn_OnClick()
	local mainplayer 	= ActorMgr:getMainPlayer();
	local petPackage 	= mainplayer:getPet()
	local petNum		= petPackage:getPetNum();
	if petNum <= MAX_PET_LIST_NUM then 
		return;
	end
	EatIndex = ( EatIndex - 1 > 0 ) 
			and ( EatIndex - 1 ) 
			or 0  ;
	UpdatePetListInfo( this:GetParent(), EatIndex );
	UpdateEatPetListRich();
	UpdateEatPetChooseTips();
end

function PetAttrEatFrameSrcPetBtn_OnClick()
	if t_EatPet["srcPet"] == 0 then 
		return;
	elseif arg1 == "RightButton" then
		SetPetAttrEatFrameSrcPetInfo();
	end 
end 

function SetPetAttrEatFrameSrcPetInfo()
	PetAttrEatFrameSrcPetBtn:Hide();
	local desPetTex = getglobal( "PetAttrEatFrameDesPetBtnIcon" );
	local previwTex	= getglobal( "PetAttrEatFramePreviewBtnIcon" );
	local texPath	= desPetTex:GetTexture();
	previwTex:SetTexture( texPath );
	PetAttrEatFrameCostBtnNumFont:SetText("0");
	local btnID		= t_EatPet["srcPet"] - EatIndex;
	local btn 		= getglobal( "PetAttrEatFramePetBtn"..btnID );
	local rich		= getglobal( "PetAttrEatFramePetBtn"..btnID.."Rich");
	t_EatPet["srcPet"] = 0;
	UpdateEatPetChooseTips();
	if btn == nil then 
		return;
	end
	btn:DisChecked();
	rich:Hide();	
	
end
function PetAttrEatFrameSrcPetCloseBtn_OnClick()
	SetPetAttrEatFrameSrcPetInfo();
end
function PetAttrEatFrameDesPetBtn_OnClick()
	if t_EatPet["desPet"] == 0 then 
		return;
	elseif arg1 == "RightButton" then
		SetPetAttrEatFrameDesPetInfo();
	end 
end
function SetPetAttrEatFrameDesPetInfo()
	local btnID		= t_EatPet["desPet"] - EatIndex;
	local btn 		= getglobal( "PetAttrEatFramePetBtn"..btnID );
	local rich		= getglobal( "PetAttrEatFramePetBtn"..btnID.."Rich");

	PetAttrEatFrameDesPetBtn:Hide();
	t_EatPet["desPet"] = 0;
	UpdateEatPetChooseTips();
	if btn == nil then 
		return;
	end
	btn:DisChecked();
	rich:Hide();
end
function PetAttrEatFrameDesPetCloseBtn_OnClick()
	SetPetAttrEatFrameDesPetInfo();
end
------------------------------------宠物进阶-------------------------------------
local t_StepUpCloseFrame = { "PetGrowResetFrame", "Pet_DevelopmentBuildUpFrame","PetAttrEatFrame","PetStarMoveFrame"}
function PetStepUpFrame_OnShow()
--[[
	for _, name in pairs( t_StepUpCloseFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	--]]
	local rich = getglobal( "PetStepUpFrame_TipsRichText");
	rich:SetText( t_AttrMoveTips[3], 160,155,130 );
	local t_selUseItem = { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
	SetOperationSelUseItem( t_selUseItem );
	
	DefaultShow( this:GetName() );
	local mainplayer			= ActorMgr:getMainPlayer();
	local petPackage			= mainplayer:getPet();
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	
	local petInfo				= petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	SetPetInfoShow( this:GetName() );
	local modelview = getglobal( "PetStepUpFrame_IconBtnModelView" );
	modelview:Hide();
	-- 若宠物面板没显示，则随着宠物操作面板一起显示
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local frame = getglobal( "PetFrame" );
	SetMainSelPetInfo(t_operationSelPetInfo);
	if not frame:IsShown() then
		frame:Show();
		frame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	end
	UpdatePetFrame();
	CheckPetStepUpItem();
	UpdatePetBattleFlagIcon( "PetStepUpFrame" );
end

function CheckPetStepUpItem()
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local item = container:getItem( PET_STEP_UP_USE_ID_TOLUA );
	local t_selUseItem = GetOperationSelUseItem();
	if t_selUseItem["id"] ~= 0 then
		ClearPet_OperationItem();
	end

	local szFrameName = "PetStepUpFrame";

	local nTotalItemNum	= container:getItemTotalNum( PET_STEP_UP_USE_ID_TOLUA );
	local btn			= getglobal( "PetStepUpFrame_IconBtn" );
	btn:SetClientUserData( 0, PET_STEP_UP_USE_ID_TOLUA );
	btn:SetClientUserData( 1, nTotalItemNum );
	if nTotalItemNum == 0 then
		return;
	end
	-- 设置图标
	local tex		= getglobal( "PetStepUpFrame_IconBtnIcon" );
	local itemdef	= item:getItemDef();
	local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
	tex:SetTexRelUV( 1, 1 );
	
	t_selUseItem = { selItem = item, id = item:getItemId(), listType = item:getListType(), gridx = item:getGridIdx() };
	SetOperationSelUseItem( t_selUseItem );
	
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local bagItem = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if bagItem:getItemId() == PET_STEP_UP_USE_ID_TOLUA then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_STEP_UP_USE_ID_TOLUA, szFrameName)
		end
	end
	UpdatePetStepUpItemNum();
end

function UpdatePetStepUpItemNum()
	local mainplayer 	= ActorMgr:getMainPlayer()
	local petMgr 		= mainplayer:getPet()
	local t_selUseItem = GetOperationSelUseItem();
	if t_selUseItem["id"] == 0 then
		PetStepUpFrame_IconBtn:SetClientUserData( 0, 0 );
		PetStepUpFrame_CostCaiLiaoBtnFont:SetText("");
		return;
	end

	local container		= mainplayer:getContainer();
	local nTotalItemNum = container:getItemTotalNum( PET_STEP_UP_USE_ID_TOLUA );
	local nItemNum		= ( petMgr:getBattlePetNum() + 1 ) * 10 ;
	--这边要用战宠数量计算
	PetStepUpFrame_CostCaiLiaoBtnFont:SetText( nTotalItemNum.."/"..nItemNum );
end

function UpdatePetBattleFlagIcon( parent )
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local slider		= getglobal( parent.."_ScrollBar" );
	local nStartIndex	= 0;
	if slider:IsShown() then
		nStartIndex = slider:GetValue();
	end
	for i = 1, MAX_PET_NUM do 
		local battleTex	= getglobal( parent.."_PetNameBtn"..i.."BattleIcon" );
		battleTex:Hide();
	end
	
	local nButtonIndex	= 1;
	-- 选定原来的被选定的宠物列表按钮
	for i = nStartIndex + 1, petPackage:getPetNum() do
		if nButtonIndex > MAX_PET_NUM then
			break;
		end
		local battleFlag	= getglobal( parent.."_PetNameBtn"..nButtonIndex.."BattleIcon" );
		local petInfo		= petPackage:getPetInfo( i - 1 );
		
		if petPackage:isExtraClanJiYangPet( petInfo ) then 
			battleFlag:SetTexture("uires\\ZhuJieMian\\3.dds")
			battleFlag:SetTexUV( 395, 792, 15, 15 )
			battleFlag:Show();
		elseif petPackage:isExtraBattlePet( petInfo ) then
			battleFlag:SetTexture("uires\\ZhuJieMian\\1.dds")
			battleFlag:SetTexUV( 994, 285, 15, 15 )
			battleFlag:Show();
		end	
		nButtonIndex = nButtonIndex + 1;
	end	
end

function PetStepUpFrame_OnLoad()
	this:RegisterEvent( "GE_DEL_PET" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent("GE_PET_ATTR_CHG");
	this:RegisterEvent("GE_GET_NEW_PET");
end

function UpdatePetStepUpList( petPackage )
	PetStepUpFrame_PetListBackGroundHeadFont:SetText( "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() );
	SetScrollBar( "PetStepUpFrame_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );	
end

function PetStepUpFrame_OnEvent() 
	if not this:IsShown() then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if(arg1 == "GE_DEL_PET") then
		if petPackage:getPetNum() == 0 then
			ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
			this:Hide();
			PetFrame:Hide();
			return;
		end
		UpdatePetStepUpList( petPackage );
		
		UpdateOperationPetList( "PetStepUpFrame" );
		UpdatePetBattleFlagIcon( "PetStepUpFrame" );
		SetPetInfoShow( "PetStepUpFrame"  );	
	end
	if(arg1 == "GE_CONTAINER_UPDATE") then
		local container		= mainplayer:getContainer();		
		local nTotalItemNum	= container:getItemTotalNum( PET_STEP_UP_USE_ID_TOLUA );
		if nTotalItemNum == 0 then 
			local t_selUseItem = { id = 0, selItem = nil, listType = 0, gridx = 0, num = 0 };
			local tex = getglobal( "PetStepUpFrame_IconBtnIcon" ); 
			tex:SetTexture( "uires\\ZhuJieMian\\1.dds" );
			tex:SetTexUV( 749, 472, 44, 44 );
			SetOperationSelUseItem( t_selUseItem );
		else 
			local jinJieItem = GetOperationSelUseItem();
			if jinJieItem["id"] ~= PET_STEP_UP_USE_ID_TOLUA and PetStepUpFrame:IsShown() then
				local item 		= container:getItem( PET_STEP_UP_USE_ID_TOLUA );
				local tex		= getglobal( "PetStepUpFrame_IconBtnIcon" );
				local itemdef	= item:getItemDef();
				local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
				tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
				tex:SetTexRelUV( 1, 1 );
				t_selUseItem = { selItem = item, id = PET_STEP_UP_USE_ID_TOLUA, listType = item:getListType(), gridx = item:getGridIdx() };
				SetOperationSelUseItem( t_selUseItem );
				for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
					local bagItem = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
					if bagItem:getItemId() == PET_STEP_UP_USE_ID_TOLUA then
						LockItem(CONTAINER_TYPE_SUNDRIES, i, PET_STEP_UP_USE_ID_TOLUA, "PetStepUpFrame")
					end
				end
			end
		end 
		UpdatePetStepUpItemNum();
	end
	if (arg1 == "GE_GET_NEW_PET") then
		UpdatePetStepUpList( petPackage );
		UpdateOperationPetList( "PetStepUpFrame" );
		UpdatePetBattleFlagIcon( "PetStepUpFrame" );
	end
	if (arg1 == "GE_PET_ATTR_CHG") then
		UpdatePetStepUpList( petPackage );
		UpdateOperationPetList( "PetStepUpFrame" );
		UpdatePetBattleFlagIcon( "PetStepUpFrame" );
	end
end
function PetStepUpSucc( nSucc )
	if nSucc == 1 then 	
		ShowMidTips("您的宠物进阶成功！");
		local modelview = getglobal("PetStepUpFrame_IconBtnModelView");
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		if not modelview:IsShown() then
			modelview:Show();
		end
		UpdatePetBattleFlagIcon( "PetStepUpFrame" );
	end
end
-- 开始进阶
function PetStepUpFrameRankUpBtn_OnClick()
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	if t_operationSelPetInfo["id"] == 0 then
		return;
	end
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( t_operationSelPetInfo["index"] );
	if petInfo == nil then 
		ShowMidTips("该宠物已不存在");
		return ;
	end
	if petInfo.Level < 60 then 
		ShowMidTips("宠物等级需达到60级");
		return ;
	end
	if petInfo.PetGenius < CLIENT_PET_GENIUS_B then 
		ShowMidTips("宠物资质需要达到优秀或优秀以上");
		return ;
	end
	local bBattlePet	= petPackage:isExtraBattlePet( petInfo );
	if bBattlePet then 
		ShowMidTips("该宠物已经是战宠");
		return ;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips("宠物正在寄养中，请前往公会城市取消寄养再进行该操作！");
		return ;
	end
	local useDef 		= GameArmProce:getUseDef( USE_ID_PET_STEP_UP_TOLUA );
	local nNeedMoney 	= ( useDef == nil ) and 0 or useDef.UseMoney;
	if mainplayer:getMoney() < nNeedMoney then 
		ShowMidTips("所需银币不足！");
		ShowBuyMoneyFrame();
		return ;
	end
	local nTotalItemNum = container:getItemTotalNum( PET_STEP_UP_USE_ID_TOLUA );
	local nItemNum		= ( petPackage:getBattlePetNum() + 1 ) * 10 ;
	if nTotalItemNum == 0 or nTotalItemNum < nItemNum then 
		ShowMidTips("进阶果实不足！");
		return ;
	end

	petPackage:petStartStepUp( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );	
end

-----------------------------------宠物成长进阶部分----------------------------------
local nCurPetIndex = -1;

function PetImpUpSucc( bSuc )
	if bSuc == 1 then 	
		ShowMidTips("您的宠物进化成功！");
		local modelview = getglobal("PetImpUpFrame_IconBtnModelView");
		modelview:setPetModel(0, 0, "effect\\entity\\fumo01.ent")
		modelview:setEntityPosition(0, 0, 100, 0)
		if not modelview:IsShown() then
			modelview:Show();
		end
		local mainplayer = ActorMgr:getMainPlayer();
		local petPackage = mainplayer:getPet();
		local petInfo = petPackage:getPetInfo( nCurPetIndex );
		SetPetImpUpFramePetInfo(petInfo);
	else 
		ShowMidTips("您的宠物进化失败！");
	end
end
function PetImpUpFrame_OnLoad()
	PetImpUpFrame_TipsRichText:SetText( t_AttrMoveTips[4],160,155,130 )
	this:RegisterEvent( "GE_DEL_PET" );
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent("GE_PET_ATTR_CHG");
	this:RegisterEvent("GE_GET_NEW_PET");
end
function PetImpUpFrame_OnEvent()
	if not this:IsShown() then
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	local petPackage = mainplayer:getPet();
	if(arg1 == "GE_DEL_PET") then
		if petPackage:getPetNum() == 0 then
			ShowMidTips( "身上没宠物，不能进行该操作，自动关闭面板" );
			this:Hide();
			PetFrame:Hide();
			return;
		end
		PetImpUpFrame_PetListBackGroundHeadFont:SetText( "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() );
		SetScrollBar( "PetImpUpFrame_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );
		
		UpdateOperationPetList( "PetImpUpFrame" );
		UpdatePetBattleFlagIcon( "PetImpUpFrame" );
		SetPetInfoShow( "PetImpUpFrame"  );	
	end
	if(arg1 == "GE_CONTAINER_UPDATE") then	
		local itemID = PetImpUpFrame_IconBtn:GetClientUserData(0)
		ChangePetImpUpItem(itemID)
	end
	if (arg1 == "GE_GET_NEW_PET") then
		PetImpUpFrame_PetListBackGroundHeadFont:SetText( "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() );
		SetScrollBar( "PetImpUpFrame_ScrollBar", MAX_PET_NUM, petPackage:getPetNum() );
		
		UpdateOperationPetList( "PetImpUpFrame" );
		UpdatePetBattleFlagIcon( "PetImpUpFrame" );
	end
	if (arg1 == "GE_PET_ATTR_CHG") then
		UpdateOperationPetList( "PetImpUpFrame" );
		UpdatePetBattleFlagIcon( "PetImpUpFrame" );
		local petInfo = petPackage:getPetInfo( nCurPetIndex );
		SetPetImpUpFramePetInfo(petInfo);
	end
end
function PetImpUpFrame_OnShow()
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();
	
	DefaultShow( this:GetName() );
	local t_operationSelPetInfo = GetOperationSelPetInfo();
	nCurPetIndex = t_operationSelPetInfo["Index"]
	
	local petInfo = petPackage:getPetInfo( t_operationSelPetInfo["heightId"], t_operationSelPetInfo["lowId"] );
	SetPetInfoShow( this:GetName() );
	local modelview = getglobal( "PetImpUpFrame_IconBtnModelView" );
	modelview:Hide();
	-- 若宠物面板没显示，则随着宠物操作面板一起显示
	this:SetPoint("topleft", "UIClient", "topleft", 428, 163)
	local frame = getglobal( "PetFrame" );
	SetMainSelPetInfo(t_operationSelPetInfo);
	if not frame:IsShown() then
		frame:Show();
		frame:SetPoint("topright", "UIClient", "topleft", 428, 163)
	end
	UpdatePetFrame();
	
	PetImpUpFrame_PetListBackGroundHeadFont:SetText( "宠物列表"..petPackage:getPetNum().."/".. petPackage:getPetMaxNum() );
	
	UpdateOperationPetList( "PetImpUpFrame" );
	UpdatePetBattleFlagIcon( "PetImpUpFrame" )
end

function PetImpUpFrame_OnHide()
	ClearItemLock("PetImpUpFrame");
end

function PetImpUpFrameRankUpBtn_OnClick()
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();
	local petInfo		= petPackage:getPetInfo( nCurPetIndex );
	if petInfo == nil then 
		ShowMidTips("该宠物已不存在");
		return ;
	end
	if petPackage:isExtraClanJiYangPet( petInfo ) then 
		ShowMidTips("宠物正在寄养中，请前往公会城市取消寄养再进行该操作！");
		return ;
	end
	local ImpUpDef	= getPetImpUpDef( petInfo.Imp );
	if ImpUpDef == nil then 
		ShowMidTips("宠物已成长到最高等级了！")
		return;
	end
	local nMoney = ImpUpDef.UpUseMoney;
	if mainplayer:getMoney() < nMoney then 
		ShowMidTips("所需银币不足！");
		ShowBuyMoneyFrame();
		return ;
	end
	local nTotalItemNum = container:getItemTotalNum( ImpUpDef.UpUseItem );
	if nTotalItemNum == 0 or nTotalItemNum < ImpUpDef.UpUseItemNum then 
		ShowMidTips("进化所需道具不足！");
		return ;
	end
	if petPackage:isPetInState( nCurPetIndex, CLIENT_PET_STAT_FIGHT ) then 
		ShowMidTips("您的宠物处于出征状态，无法进化！");
		return ;
	end
	petPackage:petStartImpUp( petInfo.WID.nHeight, petInfo.WID.nLow );	
end
function PetImpUpFrame_PetNameBtn_OnClick()
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local container		= mainplayer:getContainer();
	local petPackage	= mainplayer:getPet();
	
	local slider		= getglobal( "PetImpUpFrame_ScrollBar" );
	local nStartIndex	= 0;
	if slider:IsShown() then
		nStartIndex	= slider:GetValue();
	end
	
	nCurPetIndex = this:GetClientID() + nStartIndex - 1
	local petInfo = petPackage:getPetInfo( nCurPetIndex );
	if petInfo == nil then 
		return ;
	end
	local t_mainSelPetInfo = { heightId = petInfo.WID.nHeight, lowId = petInfo.WID.nLow, id = petInfo.PetID, index = nCurPetIndex };
	SetMainSelPetInfo( t_mainSelPetInfo );
	SetOperationSelPetInfo( t_mainSelPetInfo );
	ShowPetInfo( petInfo, "PetFrame", true, true );
	UpdatePetImpUpFrame( 2 )
	DisCheckAllBtn( "PetImpUpFrame_PetNameBtn", MAX_PET_NUM );
	this:Checked()
end

function UpdatePetImpUpFrame( nType ) -- nType （ 1为点击宠物界面 、2为点击本界面）
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local petPackage	= mainplayer:getPet();
	if nType == 1 then 
		local t_operationSelPetInfo = GetOperationSelPetInfo();
		if t_operationSelPetInfo["id"] == 0 then
			return;
		end		
		local petInfo = petPackage:getPetInfo( t_operationSelPetInfo["index"] );
		if petInfo == nil then 
			return ;
		end
		nCurPetIndex = t_operationSelPetInfo["index"]
		SetPetImpUpFrameCheck( "PetImpUpFrame" );
		SetPetImpUpFramePetInfo(petInfo)
	elseif nType == 2 then 
		if not PetFrame:IsShown() then 
			return;
		end
		local petInfo = petPackage:getPetInfo( nCurPetIndex );
		SetPetImpUpFramePetInfo(petInfo);
		UpdatePeFrame_PetList();
	end
end
-- 宠物列表check状态
function SetPetImpUpFrameCheck( frameName )
	DisCheckAllBtn( frameName.."_PetNameBtn", MAX_PET_NUM );
	local slider		= getglobal( frameName.."_ScrollBar" );
	local nStartIndex	= 0;
	if slider:IsShown() then
		nStartIndex	= slider:GetValue();
	end
	local id = nCurPetIndex + 1 - nStartIndex;
	local btn = getglobal( frameName.."_PetNameBtn1" )
	if id < 0 then 
		slider:SetValue( nCurPetIndex + 1 )
	elseif id > MAX_PET_NUM then 
		btn = getglobal( frameName.."_PetNameBtn"..MAX_PET_NUM )
		slider:SetValue( nCurPetIndex + 1 - MAX_PET_NUM );
	elseif id > 0 and id <= MAX_PET_NUM then
		btn = getglobal( frameName.."_PetNameBtn"..id )
	end
	btn:Checked();
end
function SetPetImpUpFramePetInfo( petinfo )
	if petinfo == nil then 
		ShowMidTips("该宠物不存在！")
		return 
	end
	PetImpUpFrame_PetNameFont:SetText( petinfo.Name )
	local ImpUpDef	= getPetImpUpDef( petinfo.Imp );
	if ImpUpDef ~= nil then 
		PetImpUpFrame_CostBullShowFont:SetText( ImpUpDef.UpUseMoney );	
	else
		PetImpUpFrame_CostBullShowFont:SetText( 0 );	
	end
	local petDef = getPetDef( petinfo.PetID )
	PetImpUpFrame_PhotoTex:SetTexture( GetMonsterIconPath(petDef.IconID) )
	PetImpUpFrame_LevelFont:SetText( "成长："..petinfo.Imp )
	local percentVal = ( ImpUpDef == nil ) and 0 or petinfo.ImpExp / ImpUpDef.ExpUp * 10000
	local ImpPercent = 0
	if percentVal < 1 then 
		ImpPercent = math.ceil( percentVal )	
	else 
		ImpPercent = math.floor( percentVal )
	end
	PetImpUpFrame_ValFont:SetText( "潜力："..( ImpPercent / 100 ).."%" )
	if ImpUpDef ~= nil then 
		SetPetImpUpItemInfo( ImpUpDef.UpUseItem ,ImpUpDef.UpUseItemNum );
	else 
		SetPetImpUpItemInfo( 0 ,0 );
	end
end
-- 设置升级道具物品
function SetPetImpUpItemInfo( ItemID,ItemNum )
	ClearItemLock("PetImpUpFrame");
	local tex = getglobal("PetImpUpFrame_IconBtnIcon")
	if itemID == 0 then 
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		PetImpUpFrame_CostCaiLiaoBtnFont:SetText("")
		PetImpUpFrame_CostNameBtnFont:SetText("进化果实")
		return;
	end
	
	local mainplayer	= ActorMgr:getMainPlayer()
	local container		= mainplayer:getContainer()		
	local nTotalItemNum	= container:getItemTotalNum( ItemID );
	PetImpUpFrame_IconBtn:SetClientUserData( 0,ItemID );
	PetImpUpFrame_IconBtn:SetClientUserData( 1,nTotalItemNum )
	PetImpUpFrame_CostCaiLiaoBtnFont:SetText( nTotalItemNum.."/"..ItemNum );
	if nTotalItemNum == 0 then 
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
		PetImpUpFrame_CostCaiLiaoBtnFont:SetText("")
		PetImpUpFrame_CostNameBtnFont:SetText("进化果实")
		return;
	end
	local itemdef = getItemDef( ItemID );
	if itemdef == nil then return end
	
	local IconPath	= GetItemEquipIconPath()..itemdef.IconID..".tga";
	tex:SetTexture( ( IsInExistence( IconPath ) and IconPath or DEFAULT_ITEM_PATH ) );
	tex:SetTexRelUV( 1, 1 );
	PetImpUpFrame_CostNameBtnFont:SetText(itemdef.Name )
	
	for i = 0, container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) - 1 do
		local bagItem = container:getItem(CONTAINER_TYPE_SUNDRIES, i)
		if bagItem:getItemId() == ItemID then
			LockItem(CONTAINER_TYPE_SUNDRIES, i, ItemID, "PetImpUpFrame")
		end
	end
end
function PetImpUpFrame_ItemButton_OnClick()
	if arg1 ~= "RightButton" then
		return;
	end
	local sText = PetImpUpFrame_CostCaiLiaoBtnFont:GetText() 
	if sText == "" then 
		return
	end
	ClearItemLock( "PetImpUpFrame" );
	PetImpUpFrame_IconBtnIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga")
	-- 用于tips显示
	PetImpUpFrame_IconBtn:SetClientUserData( 1,0 )
	PetImpUpFrame_CostCaiLiaoBtnFont:SetText("")
	PetImpUpFrame_CostNameBtnFont:SetText("进化果实")
end
--  背包右击升级道具
function ChangePetImpUpItem( itemID )
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local petPackage	= mainplayer:getPet();
	local petInfo = petPackage:getPetInfo( nCurPetIndex )
	local ImpUpDef	= getPetImpUpDef( petInfo.Imp );
	if ImpUpDef == nil then 
		ShowMidTips("当前宠物已成长到最高等级了！")
		return
	end
	SetPetImpUpItemInfo( ImpUpDef.UpUseItem ,ImpUpDef.UpUseItemNum )
end