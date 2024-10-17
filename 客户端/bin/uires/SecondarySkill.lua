
local MAX_CATAGORY_NUM	= 12;	--最多显示的标签页
local MAX_SKILL_TYPE	= 3;	--最多的生活技能数量
local t_LabelNameList	= {};	--标签页数据
local MAX_SELECT_NAME	= 10;	--下拉列表的最大数量

local nCurSelSkillType	= 0;	--当前选中的技能类别

local nSecondaryLinkId	= -1;	--超链接ID

local nCurSelType	= 0;
local nCurSelSubType	= 0;

local nCurFliterBranch	= 0;	-- 分支过滤
local nCurFliterPart	= 0;	-- 部位过滤

local t_SubSkillName = {
						[CLIENT_SECONDRAY_SKILL_TYPE_MINING]		= "采矿",
						[CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM]		= "采药",
						[CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE]		= "分解",
						[CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING]	= "铸造",
						[CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY]		= "炼金",
						[CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT]		= "附魔",
						[CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING]	= "蒸汽科技",
						};

local t_BranchDropList = {};
local t_PartDropList = {};

local t_SkillNameColor = { [1]={254, 249, 234},[2] ={ 66,  168, 61},[3]={ 61,  109, 168},[4]={ 151, 45, 249 },[5]={240,120,10} };

--获取标签页的上级标签
function SecondarySkillGetCategoryParentNodeInfo( childNodeInfo )
	if childNodeInfo.nSubType == 0 then
		return;
	end
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType== 0 and t_LabelNameList[i].nType == childNodeInfo.nType then
			return t_LabelNameList[i];
		end
	end
end

function SecondarySkillCategoryHasChildNode( parentNode )
	if parentNode.nSubType > 0 then
		return 0;
	end
	for i=1,table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType > 0 and t_LabelNameList[i].nType  == parentNode.nType then
			return i;
		end
	end
	return 0
end

local t_BranchDropName = {
							[CLIENT_SECONDRAY_SKILL_TYPE_MINING]		= {"采集","熔炼"},
							[CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM]		= {"采集","研磨"},
							[CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE]		= {"分解","合成"},
							[CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING] = {"重置齿轮","头盔","胸甲","背饰","副手","面饰","肩甲","","徽章","护符","戒指","","","","鞋子","","项链","","圣杯","护符","其他"},
							[CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY]		= {"药物药水","药剂合剂","宠物药水","戒指镀金","项链镀金","护符镀金","鉴定仪","其他"},
							[CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT]		= {"主手","头盔","胸甲","","副手","面饰","肩甲","","","","","","","","鞋子","","","","","其他"},
							[CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING]	= {},
						}

local t_PartDropName = {
							[CLIENT_SECONDRAY_SKILL_TYPE_MINING]		= {},
							[CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM]		= {},
							[CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE]		= {},
							[CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING] = {"战士","法师","枪手","祭司","守护者","聚能者","猎杀者","圣佑者","征服者","炼魂师","毁灭者","驭雷师"},
							[CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY]		= {},
							[CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT]		= {"身体","头部","面部","肩部","脚部","武器","副手"},
							[CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING]	= {},
						}  

local t_LearnPosText = {
						[CLIENT_SECONDRAY_SKILL_TYPE_MINING]		= "你可以在时间城找到#L采矿大师@@[303010,1000]#n来提升你的专业称号",
						[CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM]		= "你可以在时间城找到#L采药大师@@[303011,1000]#n来提升你的专业称号",
						[CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE]		= "你可以在时间城找到#L分解大师@@[303012,1000]#n来提升你的专业称号",
						[CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING] = "你可以在时间城找到#L铸造大师@@[303013,1000]#n来提升你的专业称号",
						[CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY]		= "你可以在时间城找到#L炼金大师@@[303014,1000]#n来提升你的专业称号",
						[CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT]		= "你可以在时间城找到#L附魔大师@@[303015,1000]#n来提升你的专业称号",
						[CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING]	= "你可以在时间城找到#L蒸汽大师@@[303016,1000]#n来提升你的专业称号",
						}
local t_SkillLevelUpText = {
						[10]	= "可到%s大师处提升至熟练%s师",
						[20]	= "可到%s大师处提升至资深%s师",
						[30]	= "可到%s大师处提升至大师%s师",
						[40]	= "可到%s大师处提升至宗师%s师",
}

function SecondarySkillOpenLink( linkId )
	nSecondaryLinkId = linkId ;
	if SecondarySkillFrame:IsShown() then
		SecondarySkillFrameOnShow();
	else
		SecondarySkillFrame:Show();
	end
end

local function SecondarySkillSortNameByLeave( lhs, rhs )
	if lhs.LearnSkillLevel == rhs.LearnSkillLevel then
		return lhs.RecipeID > rhs.RecipeID;
	else
		return lhs.LearnSkillLevel > rhs.LearnSkillLevel;
	end	
end
--设置标签数据
function SecondarySkillSetLabelText()
	local t_TmpList = t_LabelNameList;
	t_LabelNameList = {};
	for i=1,table.getn( t_BranchDropName[nCurSelSkillType] ) do
		if t_BranchDropName[nCurSelSkillType][i] ~= "" then
			table.insert( t_LabelNameList,{name=t_BranchDropName[nCurSelSkillType][i],bCheck=false,nType=i,nSubType=0} );
		end
	end
	if  table.getn( t_TmpList ) < 1 or t_TmpList.SkillType ~= nCurSelSkillType then
		t_LabelNameList[1].bCheck = true;
		nCurSelSubType = 0;
	else
		for i=1,table.getn( t_TmpList ) do
			for j=1,table.getn( t_LabelNameList ) do
				if t_TmpList[i].nType == t_LabelNameList[j].nType and t_TmpList[i].nSubType == t_LabelNameList[j].nSubType then
					t_LabelNameList[j].bCheck = t_TmpList[i].bCheck;
					break;
				end
			end
		end
	end
	local t_skillId = {}
	for i=1,MAX_RECIPE_NUM do
		local recipeDef = SecondarySkillMgr:getRecipeDef( nCurSelSkillType,i )
		if recipeDef ~= nil then
			table.insert( t_skillId,recipeDef );
		end
	end
	table.sort( t_skillId, SecondarySkillSortNameByLeave )
	--添加这个生活技能中已经学会的配方
	if nSecondaryLinkId >= 0 then
		for i=1,MAX_RECIPE_NUM do
		local recipeData = t_skillId[i]				
			if recipeData ~= nil and SecondarySkillMgr:isLinkLearnedRecipe( nSecondaryLinkId, recipeData.RecipeID ) then				
				if (nCurFliterBranch == 0 or recipeData.RecipeType1 == nCurFliterBranch) 
					and (nCurFliterPart == 0 or recipeData.RecipeType2 == nCurFliterPart)
					and string.find(recipeData.NameDesc,SecondarySkillFrameSearchEdit:GetText(),1,true) then
					local nPos = 0;
					for idx=1,table.getn( t_LabelNameList ) do
						if t_LabelNameList[idx].nType == recipeData.RecipeType1 then
							if t_LabelNameList[idx].nSubType == 0 then
								t_LabelNameList[idx].nFliterPart = recipeData.RecipeType2;
							end
							nPos = idx;
						end
					end
					table.insert( t_LabelNameList,nPos + 1,{name=recipeData.NameDesc,bCheck=false,nType=recipeData.RecipeType1,nSubType=t_skillId[i].RecipeID + 1,nFliterPart=recipeData.RecipeType2 } )
				end
			end
		end
	else
		for i=1,MAX_RECIPE_NUM do
			local recipeData = t_skillId[i]	
			if recipeData ~= nil and SecondarySkillMgr:isLearnedRecipe( nCurSelSkillType, recipeData.RecipeID ) then
				if (nCurFliterBranch == 0 or recipeData.RecipeType1 == nCurFliterBranch) 
					and (nCurFliterPart == 0 or recipeData.RecipeType2 == nCurFliterPart)
					and string.find(recipeData.NameDesc,SecondarySkillFrameSearchEdit:GetText(),1,true) then
					local nPos = 0;
					for idx=1,table.getn( t_LabelNameList ) do
						if t_LabelNameList[idx].nType == recipeData.RecipeType1 then
							if t_LabelNameList[idx].nSubType == 0 then
								t_LabelNameList[idx].nFliterPart = recipeData.RecipeType2;
							end
							nPos = idx;
						end
					end
					table.insert( t_LabelNameList,nPos + 1,{name=recipeData.NameDesc,bCheck=false,nType=recipeData.RecipeType1,nSubType=t_skillId[i].RecipeID + 1,nFliterPart=recipeData.RecipeType2 } )						
				end
			end
		end
	end

	if SecondarySkillFrameSearchEdit:GetText() ~= "" then
		for i=1,table.getn( t_LabelNameList ) do
			if t_LabelNameList[i].nSubType ~= 0 then
				if string.find(t_LabelNameList[i].name,SecondarySkillFrameSearchEdit:GetText(),1,true) then
					local firstLevelNode = SecondarySkillGetCategoryParentNodeInfo( t_LabelNameList[i] );
					if firstLevelNode ~= nil then
						firstLevelNode.bCheck = true;
					end
				end
			end
		end
	end
	local nTotalCount = 0;
	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType == 0 then
			nTotalCount = nTotalCount + 1;
		else
			local firstLevelNode = SecondarySkillGetCategoryParentNodeInfo( t_LabelNameList[i] );
			if firstLevelNode ~= nil and firstLevelNode.bCheck then
				nTotalCount = nTotalCount + 1;
			end
		end
	end
	local slider = getglobal("SecondarySkillFrameScrollBar");
	SetScrollBar( "SecondarySkillFrameScrollBar", MAX_CATAGORY_NUM, nTotalCount );

	if slider:IsShown() then
		UpdateSecondarySkillFrameCategoryList( slider:GetValue() + 1 );
	else
		UpdateSecondarySkillFrameCategoryList( 1 );
	end
	t_LabelNameList.SkillType = nCurSelSkillType;
end

function SecondarySkillNoColl()
	for i=1,table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nSubType == 0 and t_LabelNameList[i].bCheck == true then
			return false;
		end
	end
	return true;
end


--点击标签页
function SecondarySkillFrame_CategoryBtn_OnClick()
	local slider = getglobal( "SecondarySkillFrameScrollBar" );
	nCurSelType = this:GetClientUserData( 0 );
	--nCurSelSubType = this:GetClientUserData( 1 ); 
	local nType = this:GetClientUserData( 0 )
	local nSubType = this:GetClientUserData( 1 )
	local nTotalCount = 0;
	local bFound = false;
	local nCurShowIndex = 0;

	for i = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[i].nType == nCurSelType and t_LabelNameList[i].nSubType == nSubType then
			t_LabelNameList[i].bCheck = not t_LabelNameList[i].bCheck;	
			nCurShowIndex = nCurShowIndex + 1;
			bFound = true;
		end
		if t_LabelNameList[i].nSubType == 0 then
			nTotalCount = nTotalCount + 1;
			if not bFound then
				nCurShowIndex = nCurShowIndex + 1;
			end
		else
			local firstLevelNode = SecondarySkillGetCategoryParentNodeInfo( t_LabelNameList[i] );
			if firstLevelNode ~= nil and firstLevelNode.bCheck then
				nTotalCount = nTotalCount + 1;
				if not bFound then
					nCurShowIndex = nCurShowIndex + 1;
				end
			end
		end
	end
	SetScrollBar( "SecondarySkillFrameScrollBar", MAX_CATAGORY_NUM, nTotalCount );

	if nSubType > 0 then
		nCurSelSubType = nSubType
	end

	if slider:IsShown() then
		UpdateSecondarySkillFrameCategoryList( slider:GetValue() + 1 );
	else
		UpdateSecondarySkillFrameCategoryList( 1 );
	end

	--说明是点击配方
	if nCurSelSubType > 0 or SecondarySkillNoColl() then
		SecondarySkillUpdateSkillDetail();
		if SecondarySkillMgr:isInSeondarySkillPre() then
			SecondarySkillFrameCountEditbox:SetText(1);
		end
	end
	SecondarySkillSaveSelData()
end

--背包中物品可制造的数量
function SecondarySkillGetCanMakeNum( nSkillType,nRecipeID )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then
		return 0;
	end
	local container = mainplayer:getContainer();
	local recipeData = SecondarySkillMgr:getRecipeDef( nSkillType,nRecipeID - 1 );
	if recipeData == nil then
		return 0;
	end
	local nMaxNum = -1;
	for i=1,MAX_RECIPE_STUFF do
		if recipeData.Stuff[i-1].ItemID > 0 and recipeData.Stuff[i-1].Num > 0 then
			local nNum = math.floor( container:getItemCount(recipeData.Stuff[i-1].ItemID)/recipeData.Stuff[i-1].Num );
			if nMaxNum < 0 or nMaxNum > nNum then
				nMaxNum = nNum;
			end
		end
	end
	--材料不足 返回-1
	if nMaxNum <= 0 then 
		return -1;
	end
	if recipeData.ConsumeVim > 0 then 
		local vimNum = math.floor( mainplayer:getVIM() / recipeData.ConsumeVim );
		if nMaxNum > vimNum then 
			nMaxNum = vimNum;
		end
		--精力值不足 返回-2
		if nMaxNum <= 0 then
			return -2;
		end
	end
	
	if recipeData.CostMoneyVal > 0 then
		local priceNum = -1;
		if recipeData.CostMoneyType == ATTR_ID_MONEY then 
			priceNum = math.floor( mainplayer:getMoney() / recipeData.CostMoneyVal );
		elseif recipeData.CostMoneyType == ATTR_ID_BULL then 
			priceNum = math.floor( getPlayerMoney( mainplayer ) / recipeData.CostMoneyVal );
		else 
			priceNum = nMaxNum;
		end
		if nMaxNum > priceNum then 
			nMaxNum = priceNum;
		end
		--银币绑银不足 返回-3
		if nMaxNum <= 0 then 
			return -3;
		end
	end
	return nMaxNum;
end

local t_CategoryTextColor = {
								{152,139,116},		--灰色
								{100,200,25},		--绿色
								{235,205,20},		--黄色
								{240,120,10},		--橙色
							}

--刷新标签页
function UpdateSecondarySkillFrameCategoryList( nStartContentIndex )
	DisCheckAllHighLightBtn( "SecondarySkillFrameCategoryBtn", MAX_CATAGORY_NUM );
	local nCurShowBtn = 1; -- 此为指向按钮的指针
	local canShowNum = 1;
	for nContentIndex = 1, table.getn( t_LabelNameList ) do
		-- 检查按钮指针是否越界
		if nCurShowBtn > MAX_CATAGORY_NUM then
			break;
		end		
				
		local btn			= getglobal( "SecondarySkillFrameCategoryBtn" .. nCurShowBtn .. "HighlightBtn" );
		local font			= getglobal( "SecondarySkillFrameCategoryBtn" .. nCurShowBtn .. "HighlightBtnType" );  
		local subTypeFont	= getglobal( "SecondarySkillFrameCategoryBtn" .. nCurShowBtn .. "HighlightBtnSubType" );
		local partionTex	= getglobal( "SecondarySkillFrameCategoryBtn" .. nCurShowBtn .. "HighlightBtnPartionTex" );

		-- 若为1级节点
		if t_LabelNameList[nContentIndex].nSubType == 0 and SecondarySkillCategoryHasChildNode( t_LabelNameList[nContentIndex] ) > 0 then
			font:SetText( t_LabelNameList[nContentIndex].name );
			if canShowNum < nStartContentIndex then
				canShowNum = canShowNum + 1;
			else
				subTypeFont:SetText("");
				btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nType );
				btn:SetClientUserData( 1, t_LabelNameList[nContentIndex].nSubType);
				btn:SetNormalTextureTemplate( "GongYongAnNiuA1NormalTemplate" );
				btn:SetHighlightTextureTemplate( "GongYongAnNiuA1HighTemplate" );
				btn:SetPushedTextureTemplate( "GongYongAnNiu1PushedTemplate" );
				btn:SetDisableTextureTemplate( "GongYongAnNiuA1DisabledTemplate" );
				btn:SetOverlayRegion( 0,0,153,26 );
				font:SetTextColor( 255, 254, 189 );
				btn:Show();
				partionTex:Hide();
				nCurShowBtn = nCurShowBtn + 1;
			end
		-- 若为2级节点
		elseif t_LabelNameList[nContentIndex].nSubType ~= 0 then
			--先查看其父节点是否为展开状态
			local firstLevelNode = SecondarySkillGetCategoryParentNodeInfo( t_LabelNameList[nContentIndex] );
			-- 若其父节点为展开状态, 则显示当前的2级节点内容,否则跳过当前2级节点
			if firstLevelNode ~= nil and firstLevelNode.bCheck then
				--拼接可以制造的数量
				if canShowNum < nStartContentIndex then
					canShowNum = canShowNum + 1;
				else
					local nNum = SecondarySkillGetCanMakeNum( nCurSelSkillType,t_LabelNameList[nContentIndex].nSubType );				
					if nNum > 0 then
						subTypeFont:SetText( t_LabelNameList[nContentIndex].name .. "[".. nNum .. "]"); 
					else
						subTypeFont:SetText( t_LabelNameList[nContentIndex].name ); 
					end
					font:SetText("");
					btn:SetClientUserData( 0, t_LabelNameList[nContentIndex].nType );
					btn:SetClientUserData( 1, t_LabelNameList[nContentIndex].nSubType);

					btn:SetNormalTextureTemplate( "TransTextureTemplate" );
					btn:SetHighlightTextureTemplate( "HighlightTextureTemplate" );
					btn:SetPushedTextureTemplate( "PushedTextureTemplate" );
					btn:SetDisableTextureTemplate( "TransTextureTemplate" );
					btn:SetOverlayRegion( 1,1,151,23 );
					
					--分解颜色特殊处理
					if nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE and t_LabelNameList[nContentIndex].nSubType == 1 then
						subTypeFont:SetTextColor( 255, 254, 189 );
					else
						local recipeDef = SecondarySkillMgr:getRecipeDef( nCurSelSkillType,t_LabelNameList[nContentIndex].nSubType - 1 );
						local nColorIdx = 1;
						if nSecondaryLinkId <0 then
							local skillInfo = SecondarySkillMgr:getSecondarySkillInfoByType( nCurSelSkillType );
							if skillInfo.SkillLevel - recipeDef.RecipelLevel >=10 then
								nColorIdx = 1;
							elseif skillInfo.SkillLevel - recipeDef.RecipelLevel >= 4 then
								nColorIdx = 2;
							elseif skillInfo.SkillLevel - recipeDef.RecipelLevel >= 0 then
								nColorIdx = 3
							else
								nColorIdx = 4;
							end
						else
							local skillLinkInfo = ChatManager:getSecondarySkillLink( nSecondaryLinkId );
							if skillLinkInfo.SkillType > 0 then
								if skillLinkInfo.SkillLevel - recipeDef.RecipelLevel >=10 then
									nColorIdx = 1;
								elseif skillLinkInfo.SkillLevel - recipeDef.RecipelLevel >=4 then
									nColorIdx = 2;
								elseif skillLinkInfo.SkillLevel - recipeDef.RecipelLevel >0 then
									nColorIdx = 3
								else
									nColorIdx = 4;
								end
							end
						end
						subTypeFont:SetTextColor( t_CategoryTextColor[nColorIdx][1],t_CategoryTextColor[nColorIdx][2],t_CategoryTextColor[nColorIdx][3] );
					end
					btn:Show();
					partionTex:Show();
					nCurShowBtn = nCurShowBtn + 1;
					-- 设置选中
					if nCurSelSubType == t_LabelNameList[nContentIndex].nSubType then
						btn:DisHighlight();
						btn:Checked();
					end
				end
			end
		end
	end
	-- 将没使用的按钮隐藏起来
	for i = nCurShowBtn, MAX_CATAGORY_NUM do
		local btn	= getglobal( "SecondarySkillFrameCategoryBtn" .. i .. "HighlightBtn" );
		btn:Hide();
		local font	= getglobal( "SecondarySkillFrameCategoryBtn" .. i .. "HighlightBtnType" );
		font:SetText( "" );
	end
end

function SecondarySkillHideAllDetail()
	HideProductBtn();
	HideResloveBtn();
	for i=1,MAX_RECIPE_GET_ITEM do
		local itemBtn = getglobal("SecondarySkillFrameRewardItemBtn"..i);
		itemBtn:Hide();
	end
	SecondarySkillHideProductItems();
	SecondarySkillFrameRewardDtailBkgTitleFont:Hide();
end

--刷新右边的详细信息
function SecondarySkillUpdateSkillDetail()
	if nCurSelSubType == 0 or SecondarySkillNoColl() then
		SecondarySkillHideAllDetail();
		return;
	end
	SecondarySkillFrameRewardDtailBkgTitleFont:Show();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local Eqiup = MainPlayer:getEquip();
	local RecipeData = nil;
	local nProduceIdx = 0;
	if nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE and nCurSelSubType <=100 then
	else
		RecipeData = SecondarySkillMgr:getRecipeDef( nCurSelSkillType,nCurSelSubType - 1 );
	end
	local nIndex = 1;
	if RecipeData ~= nil then		
		for i=1,MAXEQUIPPOS do
			local bagButton = getglobal( "EquipFrameEquip"..i);
			local item = Eqiup:getItem( bagButton:GetClientID() );
			if( item:getItemId() ~= 0 ) then
				local itemDef = item:getItemDef();
				for idx=1,MAX_ITEM_RESULT do
					if itemDef.Result[idx-1].ResultID == RESULT_SECONDARY_SKILL_PRODUCE_CHG and itemDef.Result[idx-1].ResultVal1 == nCurSelSkillType then
						nProduceIdx = itemDef.Result[i-1].ResultVal2;
					end
				end
			end
		end
		for i=1,MAX_RECIPE_GET_ITEM do				
			if RecipeData.ProduceList[nProduceIdx].GetItem[i-1].ItemID > 0 then
				local itemBtn = getglobal("SecondarySkillFrameRewardItemBtn"..i);
				local itemTex = getglobal("SecondarySkillFrameRewardItemBtn"..i.."IconTexture");
				local countFont = getglobal("SecondarySkillFrameRewardItemBtn"..i.."CountFont");
				local nameFont	= getglobal("SecondarySkillFrameRewardItemBtn"..i.."ItemNameFont");
				local rateFont	= getglobal("SecondarySkillFrameRewardItemBtn"..i.."ItemRateFont");
				local boxTex = getglobal("SecondarySkillFrameRewardItemBtn"..i.."BoxTexture");
				boxTex:Show();

				local itemDef = getItemDef( RecipeData.ProduceList[nProduceIdx].GetItem[i-1].ItemID );
				if itemDef ~= nil then
					local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					local mainplayer = ActorMgr:getMainPlayer();
					if mainplayer == nil then return end
					local container	 = mainplayer:getContainer();
					itemTex:SetTexture( IconPath );
					nameFont:SetText( itemDef.Name );
					local t_NameColor = GetItemNameColor(itemDef);
					nameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
					countFont:SetText( RecipeData.ProduceList[nProduceIdx].GetItem[i-1].Num );
					if RecipeData.ProduceList[nProduceIdx].GetItem[i-1].Rate >= 0 and RecipeData.ProduceList[nProduceIdx].GetItem[i-1].Rate <= 10000 then
						--rateFont:SetText("概率：".. string.format("%.02f",RecipeData.ProduceList[nProduceIdx].GetItem[i-1].Rate/100) .. "%");
						local rate = RecipeData.ProduceList[nProduceIdx].GetItem[i-1].Rate / 100;
						if rate < 1 then
							rateFont:SetText("概率：很低");
						elseif rate <= 10 then
							rateFont:SetText("概率：低");
						elseif rate <= 20 then
							rateFont:SetText("概率：较低");
						elseif rate <= 50 then
							rateFont:SetText("概率：高");
						elseif rate <= 80 then
							rateFont:SetText("概率：很高");
						else
							rateFont:SetText("概率：非常高");
						end
						rateFont:Show()
					else
						rateFont:Hide();
					end
				end
				itemBtn:SetClientUserData(0,RecipeData.ProduceList[nProduceIdx].GetItem[i-1].ItemID);
				itemBtn:Show();
				nIndex = nIndex + 1;
			end
		end
	end
	for i= nIndex,MAX_RECIPE_STUFF do
		local itemBtn = getglobal("SecondarySkillFrameRewardItemBtn"..i);
		itemBtn:Hide();
	end
	if nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_MINING or nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM then
	elseif nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE then
	else
		SecondarySkillRefreshProductType( nCurSelSubType - 1 );
	end
	-- 冷却时间
	if RecipeData ~= nil then
		local secondaryCoolInfo = SecondarySkillMgr:getSkillCoolInfo( RecipeData.SkillType, RecipeData.RecipeID );
		local serverTime = GameClanManager:getServerTime();
		if secondaryCoolInfo ~= nil and secondaryCoolInfo.CoolDownTime > 0 and secondaryCoolInfo.CoolDownTime > serverTime then			
			local dTime = secondaryCoolInfo.CoolDownTime - serverTime;
			local szText = "冷却时间："..os.date( "%H:%M",dTime );
			SecondarySkillFrameRewardDtailBkgCoolDownFont:SetText( szText );
			SecondarySkillFrameRewardDtailBkg:SetClientUserData( 0,secondaryCoolInfo.CoolDownTime );
		else
			SecondarySkillFrameRewardDtailBkgCoolDownFont:SetText("");
			SecondarySkillFrameRewardDtailBkg:SetClientUserData( 0,0 );
		end
	end
end

function HideProductBtn()
	SecondarySkillFrameMakeBtn:Hide();
	SecondarySkillFrameMakeAllBtn:Hide();
	SecondarySkillFrameCancelMakeBtn:Hide();
end

function UpdateProductBtn()
	if nSecondaryLinkId >=0 then
		HideProductBtn();
		SecondarySkillFrameLeftBtn:Hide();
		SecondarySkillFrameRightBtn:Hide();
		SecondarySkillFrameCountEditbox:Hide();
		SecondarySkillFrameCountBkg:Hide();
	else
		if SecondarySkillMgr:isInSeondarySkillPre() then
			SecondarySkillFrameMakeBtn:Hide();
			SecondarySkillFrameMakeAllBtn:Hide();
			SecondarySkillFrameCancelMakeBtn:Show();
		else
			SecondarySkillFrameMakeBtn:Show();
			SecondarySkillFrameMakeAllBtn:Show();
			SecondarySkillFrameCancelMakeBtn:Hide();
		end
	end
end

function SecondarySkillHideProductItems()
	--隐藏需要的材料
	for i=1,MAX_RECIPE_STUFF do
		local btn = getglobal("SecondarySkillFrameRequireItemBtn"..i);
		btn:Hide();
	end
	SecondarySkillFrameDetailBkgRequireVim:Hide();
	SecondarySkillFrameDetailBkgRequireMoney:Hide();
	SecondarySkillFrameDetailBkgPriceIconBtn:Hide();
	SecondarySkillFrameCountBkg:Hide();
	SecondarySkillFrameLeftBtn:Hide();
	SecondarySkillFrameRightBtn:Hide();
	SecondarySkillFrameCountEditbox:Hide();
	SecondarySkillFrameDetailBkgTitleFont:Hide();
end

function SecondarySkillHideRewardItems()
	for i=1,MAX_RECIPE_STUFF do
		local btn = getglobal( "SecondarySkillFrameRewardItemBtn"..i );
		btn:Hide();
	end
end

--刷新生产类
function SecondarySkillRefreshProductType( recipeID )
	for i=1,MAX_RECIPE_STUFF do
		local itemBtn = getglobal("SecondarySkillFrameRequireItemBtn"..i);
		itemBtn:Hide();
	end
	local RecipeData = SecondarySkillMgr:getRecipeDef( nCurSelSkillType,recipeID );
	if RecipeData ~= nil then
		for i=1,MAX_RECIPE_STUFF do
			if RecipeData.Stuff[i-1].ItemID > 0 then
				local itemBtn = getglobal("SecondarySkillFrameRequireItemBtn"..i);
				local itemTex = getglobal("SecondarySkillFrameRequireItemBtn"..i.."IconTexture");
				local countFont = getglobal("SecondarySkillFrameRequireItemBtn"..i.."CountFont");
				local nameFont	= getglobal("SecondarySkillFrameRequireItemBtn"..i.."ItemNameFont");
				local boxTex = getglobal("SecondarySkillFrameRequireItemBtn"..i.."BoxTexture");
				boxTex:Show();

				local itemDef = getItemDef( RecipeData.Stuff[i-1].ItemID );
				if itemDef ~= nil then
					local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
					local mainplayer = ActorMgr:getMainPlayer();
					if mainplayer == nil then return end
					local container	 = mainplayer:getContainer();
					itemTex:SetTexture( IconPath );
					nameFont:SetText(itemDef.Name);
					local t_NameColor = GetItemNameColor(itemDef);
					nameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
					local haveNum = container:getItemCount(RecipeData.Stuff[i-1].ItemID);
					if haveNum > 999 then
						haveNum = math.floor( haveNum/1000 ) .."K";
					end
					countFont:SetText( haveNum .."/"..RecipeData.Stuff[i-1].Num );
					itemBtn:SetClientUserData(0,RecipeData.Stuff[i-1].ItemID);
					itemBtn:Show();
					if container:getItemCount(RecipeData.Stuff[i-1].ItemID) >= RecipeData.Stuff[i-1].Num then
						itemTex:SetGray( false )
					else
						itemTex:SetGray( true )
					end
				end
			end
		end
		if RecipeData.ConsumeVim > 0 then
			SecondarySkillFrameDetailBkgRequireVim:SetText("消耗精力值：".. RecipeData.ConsumeVim .. "点");
			SecondarySkillFrameDetailBkgRequireVim:Show();
		else
			SecondarySkillFrameDetailBkgRequireVim:Hide();
		end

		local costMoney = getglobal("SecondarySkillFrameDetailBkgRequireMoney");
		local moneyTex	= getglobal("SecondarySkillFrameDetailBkgPriceIconBtnTex");
		local moneyBtn	= getglobal("SecondarySkillFrameDetailBkgPriceIconBtn");
		if RecipeData.CostMoneyVal > 0 then 
			if RecipeData.CostMoneyType == ATTR_ID_MONEY then 
				costMoney:SetText("消耗银币：".. RecipeData.CostMoneyVal);
				costMoney:Show();
			elseif RecipeData.CostMoneyType == ATTR_ID_BULL then 
				costMoney:SetText("消耗绑银：".. RecipeData.CostMoneyVal);
				costMoney:Show();
			else 
				costMoney:Hide();
			end
		else
			SecondarySkillFrameDetailBkgRequireMoney:Hide();
		end
		local fontWidth = costMoney:GetTextExtentWidth( costMoney:GetText() );
		local szTexPath = "uires\\ZhuJieMian\\1.dds";
		if RecipeData.CostMoneyType == ATTR_ID_MONEY then
			moneyTex:SetTexUV( 117,494,17,18 );
			moneyTex:SetSize(15,14);
			moneyBtn:SetClientUserData( 0, ATTR_ID_MONEY );
			moneyBtn:SetPoint("topleft",costMoney:GetName(),"topleft", fontWidth + 3 * GetScreenScaleY() ,0);
			moneyBtn:Show();
		elseif RecipeData.CostMoneyType == ATTR_ID_BULL then
			moneyTex:SetTexUV(78,494,20,18);
			moneyTex:SetSize(15,14);
			moneyBtn:SetClientUserData( 0, ATTR_ID_BULL );
			moneyBtn:SetPoint("topleft",costMoney:GetName(),"topleft", fontWidth + 3 * GetScreenScaleY() ,0);
			moneyBtn:Show();
		else 
			moneyBtn:Hide();
		end
		if not costMoney:IsShown() then 
			moneyBtn:Hide();
		end

		if nSecondaryLinkId < 0 then
			SecondarySkillFrameCountBkg:Show();
			SecondarySkillFrameLeftBtn:Show();
			SecondarySkillFrameRightBtn:Show();
			SecondarySkillFrameCountEditbox:Show();			
		end
		SecondarySkillFrameDetailBkgTitleFont:Show();
		local editbox = getglobal("SecondarySkillFrameCountEditbox");
		if editbox:IsShown() then
			local szNum = editbox:GetText();
			local nNum = tonumber( szNum );
			if nNum == nil then
				nNum = 1;
				editbox:SetText(nNum);
			end
		end
	end
end

function SecondarySkillSetCurSelectData()
	--刷新左边的标签页
	SecondarySkillSetLabelText();
	local skillVal,skillType,skillLevel = 0,0,0;
	if nSecondaryLinkId < 0 then
		local skillInfo = SecondarySkillMgr:getSecondarySkillInfoByType( nCurSelSkillType );
		if skillInfo == nil then return end
		skillType = skillInfo.SkillType;
		skillLevel = skillInfo.SkillLevel;
		skillVal = skillInfo.SkillVal;
	else
		local skillLinkInfo = ChatManager:getSecondarySkillLink( nSecondaryLinkId );
		if skillLinkInfo.SkillType == 0 then return end
		skillType = skillLinkInfo.SkillType;
		skillLevel = skillLinkInfo.SkillLevel;
		skillVal = skillLinkInfo.SkillVal;
	end

	local secondarySkillData = SecondarySkillMgr:getSecondarySkillData( skillType,skillLevel );
	if secondarySkillData ~= nil then
		local scale =  math.floor(skillVal/100)/math.floor(secondarySkillData.LevelUpVal/100);
		if scale > 1 then
			scale = 1;
		end
		if scale < 0 then
			scale = 0;
		end
		local IconPath = "uires\\TuPianLei\\ShengHuoJiNeng\\" .. secondarySkillData.SkillType ..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		SecondarySkillFrameDescBkgIcon:SetTexture( IconPath );
		SecondarySkillFrameDescBkgExpTex:SetTexUV( 0,480,234*scale,18 );
		SecondarySkillFrameDescBkgExpTex:SetSize(234*scale,18);
		if scale ==1 and t_SkillLevelUpText[secondarySkillData.Level] ~= nil then
			SecondarySkillFrameDescBkgExpFont:SetText( string.format( t_SkillLevelUpText[secondarySkillData.Level],t_SubSkillName[skillType],t_SubSkillName[skillType] ) );
		else
			SecondarySkillFrameDescBkgExpFont:SetText( math.floor(skillVal/100) .. "/" .. math.floor(secondarySkillData.LevelUpVal/100) );
		end
		SecondarySkillFrameDescBkgSkillTitle:SetText( secondarySkillData.NameDesc );
		local colorIdx = math.floor( (secondarySkillData.Level-1)/10 ) + 1;
		SecondarySkillFrameDescBkgSkillTitle:SetTextColor( t_SkillNameColor[colorIdx][1],t_SkillNameColor[colorIdx][2],t_SkillNameColor[colorIdx][3] );
		
		--SecondarySkillFrameDescBkgSkillLevel:SetText( string.sub(secondarySkillData.NameDesc,1,4) .. math.fmod(secondarySkillData.Level-1,10) + 1 .."级" );
		SecondarySkillFrameDescBkgSkillLevelLable:SetText( t_SubSkillName[skillType].."等级：" );
		SecondarySkillFrameDescBkgSkillLevel:SetText( secondarySkillData.Level );
		
		SecondarySkillFrameDescBkgSkillTypeName:SetText( secondarySkillData.NameDesc );
		SecondarySkillFrameSearchEdit:SetText("");

		SecondarySkillFrameRichText:SetText(t_LearnPosText[nCurSelSkillType],255,255,255);
	end
	if nCurSelSubType == 0 then
		for i=1,#t_LabelNameList do
			if t_LabelNameList[i].nSubType == 0 then
				local childIdx = SecondarySkillCategoryHasChildNode( t_LabelNameList[i] )
				if childIdx > 0 then
					t_LabelNameList[i].bCheck = true
					nCurSelSubType = t_LabelNameList[childIdx].nSubType
					break
				end
			end
		end
	end
	if nCurSelSubType > 0 then
		local slider = getglobal("SecondarySkillFrameScrollBar");
		if slider:IsShown() then
			UpdateSecondarySkillFrameCategoryList( slider:GetValue() + 1 );
		else
			UpdateSecondarySkillFrameCategoryList( 1 );
		end
		SecondarySkillUpdateSkillDetail();
	end
end

function SecondarySkillConfirmDelSkill()
	SecondarySkillMgr:requestDelSkill( nCurSelSkillType );
end

local t_SkillTypeUV = {
						[CLIENT_SECONDRAY_SKILL_TYPE_MINING] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\CaiKuang-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\CaiKuang-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\CaiKuang-3.tga",
									},
						[CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\CaiYao-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\CaiYao-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\CaiYao-3.tga",
									},
						[CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\FenJie-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\FenJie-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\FenJie-3.tga",
									},
						[CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\ZhuZao-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\ZhuZao-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\ZhuZao-3.tga",
									},
						[CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\LianJin-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\LianJin-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\LianJin-3.tga",
									},
						[CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\FoMo-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\FoMo-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\FoMo-3.tga",
									},
						[CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING] = 
									{
										"uires\\TuPianLei\\KeJiShengChan button\\ZhengQi-1.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\ZhengQi-2.tga",
										"uires\\TuPianLei\\KeJiShengChan button\\ZhengQi-3.tga",
									},
						}

function SecondarySkillUpdateSkillData()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local scale = mainplayer:getVIM()/mainplayer:getMaxVIM();
	if scale > 1 then
		scale = 1;
	end
	--显示当前的精力
	SecondarySkillFrameVimProValFont:SetText(mainplayer:getVIM() .. "/" .. mainplayer:getMaxVIM());
	SecondarySkillFrameVimProTex:SetSize( 210*scale,15 );

	if nSecondaryLinkId < 0 then
		SecondarySkillFrameSendLinkLabelBtn:Show();
		SecondarySkillFrameSendLinkBtn:Show();
		local index = 1;
		for i=1,SecondarySkillMgr:getSecondarySkillNum() do
			if i > MAX_SKILL_TYPE then return end
			local SecondarySkillInfo = SecondarySkillMgr:getSecondarySkillInfo(i - 1);
			if SecondarySkillInfo == nil then return end
			local btn = getglobal( "SecondarySkillFrameSkillType"..i .."Btn" );
			
			local btnUV = t_SkillTypeUV[SecondarySkillInfo.SkillType];

			btn:SetNormalTexture( btnUV[1],"blend" );
			btn:SetHightlightTexture( btnUV[2],"blend" );
			btn:SetPushedTexture( btnUV[3], "blend" );

			btn:DisHighlight();
			
			if nCurSelSkillType == SecondarySkillInfo.SkillType then
				btn:Checked();
			else
				btn:DisChecked();
			end
			btn:Show();
			btn:Enable();
			btn:SetClientUserData( 0,SecondarySkillInfo.SkillType );
			index = index + 1;
		end
		for i=index,MAX_SKILL_TYPE do
			local btn = getglobal( "SecondarySkillFrameSkillType"..i .."Btn" );	
			btn:SetNormalTexture( "uires\\TuPianLei\\KeJiShengChan button\\WeiXueHui-1.tga","blend" );
			btn:SetHightlightTexture( "uires\\TuPianLei\\KeJiShengChan button\\WeiXueHui-1.tga","blend" );
			btn:SetPushedTexture( "uires\\TuPianLei\\KeJiShengChan button\\WeiXueHui-1.tga", "blend" );
			btn:SetDisableTexture( "uires\\TuPianLei\\KeJiShengChan button\\WeiXueHui-1.tga", "blend" );
			btn:Disable();
			btn:Show();
		end
		if nCurSelSkillType == 0 or not SecondarySkillMgr:isLearnedSecondarySkill( nCurSelSkillType ) then
			local firstBtn = getglobal("SecondarySkillFrameSkillType1Btn");
			if firstBtn:IsEnable() then
				nCurSelSkillType = firstBtn:GetClientUserData(0);
				firstBtn:Checked();
			end
		end
	else
		for i=1,MAX_SKILL_TYPE do
			local btn = getglobal( "SecondarySkillFrameSkillType"..i .."Btn" );
			btn:Hide();
		end
		SecondarySkillFrameSendLinkLabelBtn:Hide();
		SecondarySkillFrameSendLinkBtn:Hide();
		local secondarySkillLink = ChatManager:getSecondarySkillLink( nSecondaryLinkId );
		nCurSelSkillType = secondarySkillLink.SkillType;
	end

	if nCurSelSkillType > 0 then
		SecondarySkillSetCurSelectData();
		SecondarySkillClearFilter();
	end
end

--刷新面板
function SecondarySkillReSetData()
	SecondarySkillHideProductItems();
	SecondarySkillHideRewardItems();

	SecondarySkillUpdateSkillData();
	SecondarySkillFrameCountEditbox:SetText(1);
	SecondarySkillFrameRewardDtailBkgCoolDownFont:SetText("");
end

function SecondarySkillFrameOnLoad()
	this:RegisterEvent("GE_SECONDARYSKILL_RECIPECHG");
	this:RegisterEvent("GE_SECONDARYSKILL_SKILL_CHG");
	this:RegisterEvent("GE_SECONDARYSKILL_RECIPE_SUCC");
	this:RegisterEvent("GE_SECONDARYSKILL_LEARNED_RECIPE");
	this:RegisterEvent("UI_ACTOR_ATTRIBUTE_CHG");
	this:RegisterEvent("GE_CONTAINER_UPDATE");
	this:RegisterEvent("GE_SECONDARYSKILL_RECIPE_BREAK");
end

function SecondarySkillFrameOnEvent()
	if arg1 == "GE_SECONDARYSKILL_RECIPECHG" then
		if SecondarySkillFrame:IsShown() then
			local msg = UIMSG:getUIMsg();
			if nCurSelSkillType == msg.SecondarySkill.skillType and nCurSelSubType - 1 == msg.SecondarySkill.skillId then
				SecondarySkillUpdateSkillDetail();
			end
		end
	end

	if arg1 == "GE_SECONDARYSKILL_LEARNED_RECIPE" then
		if SecondarySkillFrame:IsShown() then
			SecondarySkillReSetData();
		end
		if SecondarySkillLearnFrame:IsShown() then
			SecondarySkillSkillLearn();
		end
	end

	if arg1 == "GE_SECONDARYSKILL_SKILL_CHG" then
		if SecondarySkillFrame:IsShown() then
			if SecondarySkillMgr:getSecondarySkillNum() == 0 then
				SecondarySkillFrame:Hide();
			else
				SecondarySkillUpdateSkillData();
			end
		end
		if SecondarySkillLearnFrame:IsShown() then
			SecondarySkillRefreash();
		end
	end
	if arg1 == "GE_SECONDARYSKILL_SKILL_DEL" then
		if SecondarySkillFrame:IsShown() then
			if SecondarySkillMgr:getSecondarySkillNum() == 0 then
				nCurSelSubType = 0;
				SecondarySkillFrame:Hide();
			else
				nCurSelSubType = 0;
				SecondarySkillUpdateSkillData();				
			end
		end
	end
	if arg1 == "GE_SECONDARYSKILL_RECIPE_BREAK" then
		if SecondarySkillFrame:IsShown() then
			SecondarySkillUpdateSkillDetail();
		end
	end
	if arg1 == "GE_SECONDARYSKILL_RECIPE_SUCC" then
		if SecondarySkillFrame:IsShown() then
			local edit = getglobal("SecondarySkillFrameCountEditbox");
			local szNum = edit:GetText();
			local nNum = 0;
			if szNum ~= nil and szNum ~= "" then
				nNum = tonumber(szNum);
			end
			if nNum > 0 then
				nNum = nNum - 1;
			end
			if nNum > 0 then
				edit:SetText(nNum);
				local recipeData = SecondarySkillMgr:getRecipeDef( nCurSelSkillType, nCurSelSubType - 1 );
				if recipeData ~= nil and recipeData.CostMoneyType == ATTR_ID_BULL and checkNeedWarnCost( recipeData.CostMoneyVal ) then
					CostMoneyConfirmFrame:SetClientString("科技制造");
					CostMoneyConfirmFrame:SetClientUserData( 0, nCurSelSkillType );
					CostMoneyConfirmFrame:SetClientUserData( 1, nCurSelSubType - 1 );
					SetWarnFrame( "SecondarySkillFrame" );
					CostMoneyConfirmFrame:Show();
					return;
				end
				SecondarySkillMgr:requestUseSkillPre( nCurSelSkillType,nCurSelSubType - 1 );
			end			
			SecondarySkillUpdateSkillData();
		end
	end

	if arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if SecondarySkillFrame:IsShown() then
			--刷新精力
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end
			local scale = mainplayer:getVIM()/mainplayer:getMaxVIM();
			if scale > 1 then
				scale = 1;
			end
			SecondarySkillFrameVimProValFont:SetText(mainplayer:getVIM() .. "/" .. mainplayer:getMaxVIM());
			SecondarySkillFrameVimProTex:SetSize( 210*scale,15 );
		end
		if SecondarySkillLearnFrame:IsShown() then
			local mainplayer = ActorMgr:getMainPlayer();
			if mainplayer == nil then return end
			SecondarySkillLearnFrameStudyCostDescOwnMoney:SetText( getPlayerMoney( mainplayer ) );
		end
	end
	if arg1 == "GE_CONTAINER_UPDATE" then
		--刷新数量之类的数据
		if SecondarySkillFrame:IsShown() then
			local slider = getglobal("SecondarySkillFrameScrollBar");
			if slider:IsShown() then
				UpdateSecondarySkillFrameCategoryList( slider:GetValue() + 1 );
			else
				UpdateSecondarySkillFrameCategoryList( 1 );
			end
			SecondarySkillUpdateSkillDetail();			
		end
	end
end

function SecondarySkillFrameOnShow()
	if nSecondaryLinkId < 0 and SecondarySkillMgr:getSecondarySkillNum() == 0 then
		ShowMidTips("你当前没有学习科技生产，不能打开科技生产面板");
		SecondarySkillFrame:Hide();
		return;
	end
	nCurSelType = 1;
	SecondarySkillFrameDescBkgSkillTitle:SetText("");
	SecondarySkillFrameDescBkgSkillLevel:SetText("");
	SecondarySkillFrameDescBkgSkillTypeName:SetText("");
	SecondarySkillFrameDescBkgExpTex:SetSize(0,13);
	SecondarySkillFrameDescBkgExpFont:SetText("");
	SecondarySkillFrameDescBkgIcon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	SecondarySkillFrameSearchEdit:SetText("");
	for i=1,MAX_CATAGORY_NUM do
		local btn = getglobal("SecondarySkillFrameCategoryBtn"..i.."HighlightBtn");
		btn:Hide();
	end
	SecondarySkillClearFilter();
	SecondarySkillReSetData();
	SecondarySkillFrame:SetPoint("center","UIParent","center",0,-60);
end

function SecondarySkillFrameOnHide()
	SecondarySkillDropDownFrame:Hide();
	nSecondaryLinkId = -1;
end

--点击发送超链接
function SecondarySkillFrameSendLinkBtn_OnClick()
	SecondarySkillMgr:setSecondarySkillLink( nCurSelSkillType );
end
--搜索
function SecondarySkillFrameSearchBtn_OnClick()
	local slider = getglobal( "SecondarySkillFrameScrollBar" );
	slider:SetValue( 0 );
	SecondarySkillSetLabelText();
	UpdateSecondarySkillFrameCategoryList( 1 );
end

function SecondarySkillFrame_ScrollUpBtn_OnClick()
	local slider = getglobal( "SecondarySkillFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function SecondarySkillFrame_ScrollDownBtn_OnClick()
	local slider	= getglobal( "SecondarySkillFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end
	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();
	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function SecondarySkillFrame_ScrollBar_OnValueChanged()
	local slider	 = getglobal( "SecondarySkillFrameScrollBar" );
	local nValue	 = slider:GetValue();
	local nMaxValue	 = slider:GetMaxValue();
	if nValue <= nMaxValue then
		UpdateSecondarySkillFrameCategoryList( nValue + 1 );
	end
end

function startSecondarySkill(nType,id,nTime)
	local recipeDef = SecondarySkillMgr:getRecipeDef( nType,id );	
	if nType == CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE and id < 100 then
		
	else
		IntonateBar:SetClientUserData( 0, SECONDARYSKILL_OTHER_TYPE );
		IntonateBar:SetClientUserData( 1, nType );
		IntonateBar:SetClientUserData( 3, id );
		IntonateBar_text:SetText(recipeDef.NameDesc .. "中....");
	end

	IntonateBar:SetClientUserData( 2, nTime );
	IntonateBar:Show();

end

--点击全部制造
function SecondarySkillFrameMakeAllBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local nMaxNum = SecondarySkillGetCanMakeNum( nCurSelSkillType,nCurSelSubType  );
	if nMaxNum > 0 then
		if nMaxNum > 99 then
			nMaxNum = 99;
		end
		SecondarySkillFrameCountEditbox:SetText(nMaxNum);
		local recipeData = SecondarySkillMgr:getRecipeDef( nCurSelSkillType, nCurSelSubType - 1 );
		if recipeData ~= nil and recipeData.CostMoneyType == ATTR_ID_BULL and checkNeedWarnCost( recipeData.CostMoneyVal ) then
			CostMoneyConfirmFrame:SetClientString("科技制造");
			CostMoneyConfirmFrame:SetClientUserData( 0, nCurSelSkillType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nCurSelSubType - 1 );
			SetWarnFrame( "SecondarySkillFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
		SecondarySkillMgr:requestUseSkillPre( nCurSelSkillType,nCurSelSubType - 1 );
	elseif nMaxNum == -1 then
		ShowMidTips("材料不足");
	elseif nMaxNum == -2 then
		ShowMidTips("精力值不足");
	elseif nMaxNum == -3 then
		ShowMidTips("银币、绑银不足");
	end
end

--点击制造
function SecondarySkillFrameMakeBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local szNum = SecondarySkillFrameCountEditbox:GetText();
	local nNum = tonumber(szNum);
	if nNum == nil then
		ShowMidTips("请输入要制造的数量");
		return;
	end
	local nCanNum = SecondarySkillGetCanMakeNum( nCurSelSkillType,nCurSelSubType  );
	if nCanNum > 0 then	
		if nNum == 0 then
			nNum = 1;
		end
		if nNum > nCanNum then
			nNum = nCanNum;
		end
		SecondarySkillFrameCountEditbox:SetText(nNum);
		local recipeData = SecondarySkillMgr:getRecipeDef( nCurSelSkillType, nCurSelSubType - 1 );
		if recipeData ~= nil and recipeData.CostMoneyType == ATTR_ID_BULL and checkNeedWarnCost( recipeData.CostMoneyVal ) then
			CostMoneyConfirmFrame:SetClientString("科技制造");
			CostMoneyConfirmFrame:SetClientUserData( 0, nCurSelSkillType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nCurSelSubType - 1 );
			SetWarnFrame( "SecondarySkillFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
		SecondarySkillMgr:requestUseSkillPre( nCurSelSkillType,nCurSelSubType - 1 );
	elseif nCanNum == -1 then
		ShowMidTips("材料不足");
	elseif nCanNum == -2 then
		ShowMidTips("精力值不足");
	elseif nCanNum == -3 then
		ShowMidTips("银币、绑银不足");
	end
end

--停止制造
function SecondarySkillFrameCancelMakeBtn_OnClick()
	SecondarySkillMgr:requestBreakReciptPre();
	SecondarySkillFrameCountEditbox:SetText( 1 )
end

function SecondarySkillFrameBreakPre()
	SecondarySkillMgr:requestBreakReciptPre();
end

local nLastClickTime = 0;
--制造数量 - 1
function SecondarySkillFrameLeftBtn_OnClick()
	if os.clock() - nLastClickTime > 0.1 then
		local edit = getglobal("SecondarySkillFrameCountEditbox");
		local szNum = edit:GetText();
		local nNum = 0;
		if szNum ~= nil and szNum ~= "" then
			nNum = tonumber(szNum);
		end
		if nNum > 1 then
			nNum = nNum - 1;
		end
		edit:SetText(nNum);
		nLastClickTime = os.clock();
	end
end

function SecondarySkillFrameRightBtn_OnClick()
	if os.clock() - nLastClickTime > 0.1 then
		local edit = getglobal("SecondarySkillFrameCountEditbox");
		local szNum = edit:GetText();
		local nNum = 0;
		if szNum ~= nil and szNum ~= "" then
			nNum = tonumber(szNum);
		end
		nNum = nNum + 1;
		edit:SetText(nNum);
		nLastClickTime = os.clock();
	end
end

local t_SelSkill = {};
local t_SelSkillSub = {};  --存储已选择子按钮
function SecondarySkillSaveSelData()
	if nCurSelSkillType ~= 0 or nCurSelSkillType ~= nil then
		t_SelSkillSub[nCurSelSkillType] = nCurSelSubType ;
	end
	t_SelSkill[nCurSelSkillType] = {}
	for nContentIndex = 1, table.getn( t_LabelNameList ) do
		if t_LabelNameList[nContentIndex].nSubType == 0 and t_LabelNameList[nContentIndex].bCheck then
			table.insert( t_SelSkill[nCurSelSkillType], nContentIndex );			
		end
	end
end

function SecondarySkillFrameSkillTypeBtn_OnClick()
	if nCurSelSkillType ~= this:GetClientUserData(0) then 
		nCurSelSkillType = this:GetClientUserData(0);			
		SecondarySkillFrameCountEditbox:SetText( 1 );
		SecondarySkillClearFilter();
		SecondarySkillReSetData();
		nCurSelSubType = t_SelSkillSub[nCurSelSkillType] == nil and 0 or t_SelSkillSub[nCurSelSkillType]
		if t_SelSkill[nCurSelSkillType] ~= nil then
			for nIndex = 1, table.getn( t_SelSkill[nCurSelSkillType] ) do			
				local nContentIndex = t_SelSkill[nCurSelSkillType][nIndex];
				t_LabelNameList[nContentIndex].bCheck = true;
			end
		end
		SecondarySkillSetCurSelectData();		

		-- 打断当前正在制造的物品
		if SecondarySkillMgr:isInSeondarySkillPre() then
			SecondarySkillFrameCountEditbox:SetText(1);
		end
	end
end

function SecondarySkillClearFilter()
	nCurFliterBranch	= 0;
	nCurFliterPart		= 0;
	SecondarySkillFrameBranchBtnNameFont:SetText("所有分支");
	if nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING then
		SecondarySkillFramePartBtnNameFont:SetText("所有职业");
	else
		SecondarySkillFramePartBtnNameFont:SetText("所有部位");
	end	
end

local t_FliterList = {};
function SecondarySkillFrameBranchBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if SecondarySkillDropDownFrame:GetClientString() ~= "Branch" or not SecondarySkillDropDownFrame:IsShown() then
		t_FliterList = {};
		SecondarySkillDropDownFrame:SetClientString( "Branch" );
		table.insert(t_FliterList,{name="所有分支",value=0});
		for i=1,table.getn(t_BranchDropName[nCurSelSkillType]) do
			if nSecondaryLinkId >=0 then
				if SecondarySkillMgr:LinkHasRecipeType1( nSecondaryLinkId,i ) and t_BranchDropName[nCurSelSkillType][i]~= "" then
					table.insert( t_FliterList,{name=t_BranchDropName[nCurSelSkillType][i],value=i} );
				end
			else
				if SecondarySkillMgr:HasRecipeType1( nCurSelSkillType,i ) and t_BranchDropName[nCurSelSkillType][i]~= "" then
					table.insert( t_FliterList,{name=t_BranchDropName[nCurSelSkillType][i],value=i} );
				end
			end			
		end
		SecondarySkillDropDownFrame:SetPoint( "topleft","SecondarySkillFrameBranchBtn","bottomleft",0,2 );
		SecondarySkillDropDownFrame:Show();
	else
		SecondarySkillDropDownFrame:Hide();
	end
end

function SecondarySkillFramePartBtn_OnClick()
	if arg1 ~= "LeftButton" then
		return;
	end
	if SecondarySkillDropDownFrame:GetClientString() ~= "Part" or not SecondarySkillDropDownFrame:IsShown() then
		t_FliterList = {};
		SecondarySkillDropDownFrame:SetClientString( "Part" );
		if nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING then
			table.insert(t_FliterList,{name="所有职业",value=0});
		else
			table.insert(t_FliterList,{name="所有部位",value=0});
		end
		for i=1,table.getn(t_PartDropName[nCurSelSkillType]) do
			if SecondarySkillMgr:HasRecipeType2( nCurSelSkillType,i ) then
				table.insert( t_FliterList,{name=t_PartDropName[nCurSelSkillType][i],value=i} );
			end
		end

		SecondarySkillDropDownFrame:SetPoint( "topleft","SecondarySkillFramePartBtn","bottomleft",0,2 );
		SecondarySkillDropDownFrame:Show();
	else
		SecondarySkillDropDownFrame:Hide();
	end
end

function SecondarySkillDropDownFrameSelectBtn_OnClick()	
	local font = getglobal( this:GetName() .. "AreaName" );
	SecondarySkillDropDownFrame:Hide();
	local parent = getglobal( this:GetParent() );
	if parent:GetClientString() == "Branch" then
		SecondarySkillFrameBranchBtn:SetText(font:GetText());
		nCurFliterBranch	=  this:GetClientUserData(0) ;
	elseif parent:GetClientString() == "Part" then
		SecondarySkillFramePartBtn:SetText(font:GetText());
		nCurFliterPart	= this:GetClientUserData(0) ;
	end
	local slider = getglobal( "SecondarySkillFrameScrollBar" );
	slider:SetValue( 0 );
	SecondarySkillSetLabelText();
	UpdateSecondarySkillFrameCategoryList( 1 );
end

function UpdateDropDownFrameData( nStartIndex )
	for i=1, MAX_SELECT_NAME do
		local button =  getglobal( "SecondarySkillSelectNameBtn" .. i );
		local font	 = 	getglobal( "SecondarySkillSelectNameBtn" .. i .. "AreaName" );
		button:Disable();
		button:SetClientUserData( 0,0 );
		font:SetText( "" );
	end

	local nIconIndex = 1;
	for i=nStartIndex, table.getn( t_FliterList ) do
		if nIconIndex > MAX_SELECT_NAME then
			break;
		end
		local button =  getglobal( "SecondarySkillSelectNameBtn" .. nIconIndex );
		local font	 = 	getglobal( "SecondarySkillSelectNameBtn" .. nIconIndex .. "AreaName" );
		button:Enable();
		button:SetClientUserData( 0,t_FliterList[i].value );
		font:SetText( t_FliterList[i].name );
		nIconIndex = nIconIndex + 1;
	end
	--调整大小
	SecondarySkillDropDownFrame:SetSize(180,nIconIndex*19);
end

function SecondarySkillDropDownFrame_OnShow()
	UpdateDropDownFrameData( 1 );
end

function ClearSecondarySkillData()
	SecondarySkillMgr:clearSecondarySkillData();
	t_LabelNameList		= {};
	nCurSelSkillType	= 0;
	nCurSelType			= 0;
	nCurSelSubType		= 0;
	SecondarySkillClearFilter();
	nSecondaryLinkId	= -1;
	t_SelSkill			= {};
	t_SelSkillSub		= {};
end

function SecondarySkillNoNeedItem( skillType )
	if skillType == CLIENT_SECONDRAY_SKILL_TYPE_MINING then
		ShowMidTips("需要装备矿锄")
	elseif skillType == CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM then
		ShowMidTips("需要装备镰剪")
	end
end

function SecondarySkillCollectLevel( SkillType,skillID )
	local recipeDef = SecondarySkillMgr:getRecipeDef( SkillType,skillID );
	local SkillDef = SecondarySkillMgr:getSecondarySkillData( SkillType,recipeDef.LearnSkillLevel );
	--ShowMidTips( "需要"..SkillDef.NameDesc.. math.fmod(recipeDef.LearnSkillLevel-1,10)+1 .."级" );
	ShowMidTips( "需要" .. t_SubSkillName[SkillType] .. "等级" .. recipeDef.LearnSkillLevel );
end

function SecondarySkillFrameSendLinkBtn_OnEnter()
	--SetGameTooltips(this:GetName(),"发送链接");
	local szSkillName = t_SubSkillName[nCurSelSkillType];
	local szText = ( "发送"..szSkillName..[[技能到
聊天框，供其他
玩家查看
]])
	SetGameTooltips( this:GetName(), szText )
end

--[[
function SecondarySkillFrameDeleteSkillBtn_OnEnter()
	SetGameTooltips(this:GetName(),"遗忘技能");
end
--]]

function SecondarySkillFrameOnUpdate()
	if nCurSelSubType == 0 or SecondarySkillNoColl() then
		return;
	end
	--刷新输入框
	local editbox = getglobal("SecondarySkillFrameCountEditbox");
	if editbox:IsShown() then
		local szNum = editbox:GetText();
		local nNum = tonumber( szNum );
		if nNum == nil then
			--nNum = 1;
			--editbox:SetText(nNum);
		end
		if nNum < 1 then
			nNum = 1;
			editbox:SetText(nNum);
		end
		if nNum > 99 then
			nNum = 99;
			editbox:SetText(nNum);
		end
	end
	--刷新按钮
	if nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_MINING or nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM then
	elseif nCurSelSkillType == CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE then
	else
		UpdateProductBtn( );
	end
	if SecondarySkillFrameRewardDtailBkg:GetClientUserData( 0 ) > 0 then
		local serverTime = GameClanManager:getServerTime();
		local dTime = SecondarySkillFrameRewardDtailBkg:GetClientUserData( 0 ) - serverTime;
		if dTime > 0 then
			SecondarySkillFrameRewardDtailBkgCoolDownFont:SetText( "冷却时间："..os.date( "%H:%M",dTime ) );
		else
			SecondarySkillFrameRewardDtailBkgCoolDownFont:SetText("");
			SecondarySkillFrameRewardDtailBkg:SetClientUserData( 0,0 );
		end
	end
end

function SecondarySkillFrameVimPro_OnEnter()
	SetGameTooltips( this:GetName(), "制作物品需要消耗精力值，精力值通过时间会缓慢恢复，或通过饮用精力药水快速恢复" )
end

---------------------------------------------------------------
-- 学习生活技能

local MAX_VIEW_LINE = 10;
-- 用于表示没有选中任何类型的技能
local SKILLLEARN_TYPE_NONE = -1;
-- 用于指示当前被选中的技能
local t_CurSelectSkill = { skillId = 0, skillType = SKILLLEARN_TYPE_NONE, nLineNumOnUI = 0 };

--指示全部技能页面和可学习技能页面的标示符
local SKILLPAGE_TOTAL	 = 3;
local SKILLPAGE_CANSTUDY = 4;
local SKILLPAGE_NONSTUDY = 5;

local nCurSkillPage = SKILLPAGE_CANSTUDY;--用于保存当前显示的技能页面是哪个,默认显示全部技能页面

local nLearnCurSelSkillType = CLIENT_SECONDRAY_SKILL_TYPE_MINING;

--定义用于保存可学习技能的列表和全部技能的列表
local t_TotalSkillList = {};
local t_CanStudySkillList = {};
local t_NonStudySkillList = {};


function SecondarySkillOpenFrame( skillType,nActorId )
	nLearnCurSelSkillType = skillType;
	local skillInfo = SecondarySkillMgr:getSecondarySkillInfoByType( nLearnCurSelSkillType );
	--if skillInfo == nil then return	end

	SecondarySkillLearnFrameHeadFont:SetText( t_SubSkillName[nLearnCurSelSkillType] .. "大师" );
	if skillInfo ~= nil then
		SecondarySkillLearnFrameCurSkillFont:SetText( "当前" .. t_SubSkillName[nLearnCurSelSkillType] .. "等级：" .. skillInfo.SkillLevel );
	else
		SecondarySkillLearnFrameCurSkillFont:SetText( "当前" .. t_SubSkillName[nLearnCurSelSkillType] .. "等级：" .. "未学习" );
	end
	SecondarySkillLearnFrame:Show();
	util.testNpcDialogDistance( nActorId, SecondarySkillLearnFrame );
end

function SecondarySkillSortStudyList( lhs,rhs )	
	local RecipeDef1 = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,lhs );
	local RecipeDef2 = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,rhs );
	if RecipeDef1.LearnSkillLevel == RecipeDef2.LearnSkillLevel then
		return RecipeDef1.RecipeID < RecipeDef2.RecipeID;
	else
		return RecipeDef1.LearnSkillLevel < RecipeDef2.LearnSkillLevel;
	end	
end

function SecondarySkillUpdateTotalList()
	--清空原来的列表内容
	t_TotalSkillList = {};
	for i=1,MAX_RECIPE_NUM do
		local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,i-1 );
		if recipeData ~= nil and recipeData.LearnType == LEARN_TYPE_PANEL and not SecondarySkillMgr:isLearnedRecipe(nLearnCurSelSkillType,i-1) then
			table.insert( t_TotalSkillList, recipeData.RecipeID );
		end
	end
	table.sort( t_TotalSkillList, SecondarySkillSortStudyList );
end

function SecondarySkillUpdateCanStudyList()
	--清空原来的列表内容
	t_CanStudySkillList = {};
	for i=1,MAX_RECIPE_NUM do
		local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,i-1 );
		if recipeData ~= nil and recipeData.LearnType == LEARN_TYPE_PANEL and not SecondarySkillMgr:isLearnedRecipe(nLearnCurSelSkillType,i-1) then
			if SecondarySkillMgr:canLearnSkill(nLearnCurSelSkillType,recipeData.RecipeID) then
				table.insert( t_CanStudySkillList, recipeData.RecipeID );			
			end
		end
	end
	table.sort( t_CanStudySkillList, SecondarySkillSortStudyList );	
end

function SecondarySkillUpdateNonStudyList()
	--清空原来的列表内容
	t_NonStudySkillList = {};
	for i=1,MAX_RECIPE_NUM do
		local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,i-1 );
		if recipeData ~= nil and recipeData.LearnType == LEARN_TYPE_PANEL and not SecondarySkillMgr:isLearnedRecipe(nLearnCurSelSkillType,i-1) then
			if not SecondarySkillMgr:canLearnSkill(nLearnCurSelSkillType,recipeData.RecipeID) then
				table.insert( t_NonStudySkillList, recipeData.RecipeID );
			end
		end
	end
	table.sort( t_NonStudySkillList, SecondarySkillSortStudyList );
end

function SecondarySkillRefreash()
	local t_CurDisplayList = {};

	SecondarySkillUpdateTotalList();
	SecondarySkillUpdateCanStudyList();
	SecondarySkillUpdateNonStudyList();

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider = getglobal( "SecondarySkillLearnFrameScrollBar" );
	SecondarySetLearnSkillScrollBar( t_CurDisplayList );	
	slider:SetValue( 0 );
	
	if slider:IsShown() then
		SecondarySkillUpdateSkillPage( slider:GetValue() + 1 );
	else
		SecondarySkillUpdateSkillPage( 1 );
	end

	if table.getn( t_CurDisplayList ) > 0 then
		local btn = getglobal( "SecondarySkillLearnFrameLearnSkillInfo1" )
		t_CurSelectSkill.nLineNumOnUI	=  btn:GetClientID();
		t_CurSelectSkill.skillId		= btn:GetClientUserData( 0 );
		SecondarySkillLearnFrame_ShowSkillInfo( t_CurSelectSkill.skillId );		
		SecondaryFixHighLightTex( t_CurSelectSkill.nLineNumOnUI );
	end
end

function SecondarySkillSkillLearn()
	local t_CurDisplayList = {};

	SecondarySkillUpdateTotalList();
	SecondarySkillUpdateCanStudyList();
	SecondarySkillUpdateNonStudyList();

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local slider = getglobal( "SecondarySkillLearnFrameScrollBar" );
	SecondarySetLearnSkillScrollBar( t_CurDisplayList );	
	slider:SetValue( 0 );
	
	if slider:IsShown() then
		SecondarySkillUpdateSkillPage( slider:GetValue() + 1 );
	else
		SecondarySkillUpdateSkillPage( 1 );
	end
	
	if table.getn( t_CurDisplayList ) > 0 then
		local btn = getglobal( "SecondarySkillLearnFrameLearnSkillInfo1" )
		t_CurSelectSkill.nLineNumOnUI	=  btn:GetClientID();
		t_CurSelectSkill.skillId		= btn:GetClientUserData( 0 );
		SecondarySkillLearnFrame_ShowSkillInfo( t_CurSelectSkill.skillId );		
		SecondaryFixHighLightTex( t_CurSelectSkill.nLineNumOnUI );
	end	
end

function SecondarySkillLearnFrame_ShowSkillInfo( nSkillId )
	--图标
	local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,nSkillId  );
	if recipeData == nil then return end
	local IconPath = "uires\\TuPianLei\\WuPin\\"..recipeData.IconID..".tga";
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end	
	SecondarySkillLearnFrameIconBtnTex:SetTexture( IconPath );
	SecondarySkillLearnFrameIconBtn:Show();

	for i=1,MAX_RECIPE_STUFF do
		local itemBtn = getglobal("SecondarySkillLearnFrameRequireItemBtn"..i);
		itemBtn:Hide();
	end

	for i=1,MAX_RECIPE_STUFF do
		if recipeData.Stuff[i-1].ItemID > 0 then
			local itemBtn = getglobal("SecondarySkillLearnFrameRequireItemBtn"..i);
			local itemTex = getglobal("SecondarySkillLearnFrameRequireItemBtn"..i.."Tex");
			local countFont = getglobal("SecondarySkillLearnFrameRequireItemBtn"..i.."NeedFont");
			local nameFont	= getglobal("SecondarySkillLearnFrameRequireItemBtn"..i.."NameFont");
			local boxTex = getglobal("SecondarySkillLearnFrameRequireItemBtn"..i.."BoxTex");
			boxTex:Show();

			local itemDef = getItemDef( recipeData.Stuff[i-1].ItemID );
			if itemDef ~= nil then
				local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				itemTex:SetTexture( IconPath );
				nameFont:SetText(itemDef.Name);
				countFont:SetText( "(" .. recipeData.Stuff[i-1].Num .. ")" );
				itemBtn:SetClientUserData(0,recipeData.Stuff[i-1].ItemID);
				itemBtn:Show();
			end
		end
	end

	local SecondarySkillData = SecondarySkillMgr:getSecondarySkillData( recipeData.SkillType,recipeData.LearnSkillLevel );
	if SecondarySkillData ~= nil then
		SecondarySkillLearnFrameStudyCostDescNeedLevel:SetText( t_SubSkillName[recipeData.SkillType] .. "等级" .. recipeData.LearnSkillLevel );
	end

	local nProductId = recipeData.ProduceList[0].GetItem[0].ItemID;
	local itemDef = getItemDef( nProductId );
	local t_NameColor = GetItemNameColor( itemDef );	
	SecondarySkillLearnFrameStudyCostDescNameFont:SetTextColor( t_NameColor[1], t_NameColor[2], t_NameColor[3] );
	SecondarySkillLearnFrameStudyCostDescNameFont:SetText(recipeData.NameDesc);
	SecondarySkillLearnFrameStudyCostDesc:Show();
	SecondarySkillLearnFrameStudyCostDescCostMoneyFont:SetText( recipeData.LearnMoneyVal );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	SecondarySkillLearnFrameStudyCostDescOwnMoney:SetText( getPlayerMoney( mainplayer ) );

	SecondarySkillLearnFrame_StudyBtn:Show();
	SecondarySkillLearnFrameMoneyTexBtn:Show();
	SecondarySkillLearnFrameCostMoneyTexBtn:Show();
end

function SecondarySkillHideSkillInfo()
	for i=1,MAX_RECIPE_STUFF do
		local btn = getglobal("SecondarySkillLearnFrameRequireItemBtn"..i);
		btn:Hide();
	end
	SecondarySkillLearnFrameIconBtn:Hide();
	SecondarySkillLearnFrameStudyCostDesc:Hide();
	SecondarySkillLearnFrame_StudyBtn:Hide();
	SecondarySkillLearnFrameMoneyTexBtn:Hide();
	SecondarySkillLearnFrameCostMoneyTexBtn:Hide();

end

function SecondarySkillInfoBtn_OnClick()
	t_CurSelectSkill.nLineNumOnUI	=  this:GetClientID();
	t_CurSelectSkill.skillId		= this:GetClientUserData( 0 );
	SecondarySkillLearnFrame_ShowSkillInfo( t_CurSelectSkill.skillId );		
	SecondaryFixHighLightTex( t_CurSelectSkill.nLineNumOnUI );
end

function SecondarySkillLearnFrameIconBtn_OnEnter()
	local nSkillId = t_CurSelectSkill.skillId;
	if nSkillId > 0 then
		local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,nSkillId );
		if recipeData == nil then return end
		local nProductId = recipeData.ProduceList[0].GetItem[0].ItemID;
		local itemDef = getItemDef( nProductId );
		if itemDef ~= nil then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemDef )
		end
	end
end

function SecondarySkillLearnFrameIconBtn_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function SecondarySkillLearnFrameRequireItemBtn_OnEnter()
	local itemId = this:GetClientUserData(0);
	if itemId >= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,getItemDef(itemId) );
	end
end

function SecondarySkillLearnFrameRequireItemBtn_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function SecondaryFixHighLightTex( idx )
	for i=1, MAX_VIEW_LINE do
		local btn = getglobal("SecondarySkillLearnFrameLearnSkillInfo"..i );
		btn:DisChecked();
	end
	if idx <= MAX_VIEW_LINE then
		local btn = getglobal( "SecondarySkillLearnFrameLearnSkillInfo" .. idx )
		btn:Checked();
	end
end

function SecondarySetLearnSkillScrollBar( t_SkillList )
	local slider		= getglobal( "SecondarySkillLearnFrameScrollBar" );
	local nTableNum		= table.getn( t_SkillList );
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

	if slider:GetValue() > slider:GetMaxValue( ) then
		slider:SetValue( nMaxValue );
	end
end

function SecondarySkillLearnFrame_OnShow()
	SecondarySkillHideSkillInfo();
	t_CurSelectSkill.skillId = 0;
	nCurSkillPage = SKILLPAGE_TOTAL;
	t_TotalSkillList = {};
	t_CanStudySkillList = {};
	t_NonStudySkillList = {};

	SecondarySkillUpdateTotalList();
	SecondarySkillUpdateCanStudyList();
	SecondarySkillUpdateNonStudyList();
	local t_SkillList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_SkillList = t_TotalSkillList;
		SecondarySkillLearnFrame_OneLayerBtn_OneLayerName:SetText( "全部" );
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_SkillList = t_CanStudySkillList;
		SecondarySkillLearnFrame_OneLayerBtn_OneLayerName:SetText( "可学习" );
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_SkillList = t_NonStudySkillList;
		SecondarySkillLearnFrame_OneLayerBtn_OneLayerName:SetText( "不可学习" );
	end
	
	local slider = getglobal( "SecondarySkillLearnFrameScrollBar" );	
	slider:SetValue( 0 );
	SecondarySetLearnSkillScrollBar( t_SkillList );
	if slider:IsShown() then
		SecondarySkillUpdateSkillPage( slider:GetValue() + 1 );
	else
		SecondarySkillUpdateSkillPage( 1 );
	end
	if table.getn( t_SkillList ) > 0 then		
		local btn = getglobal( "SecondarySkillLearnFrameLearnSkillInfo1" )		
		t_CurSelectSkill.nLineNumOnUI	=  btn:GetClientID();
		t_CurSelectSkill.skillId		= btn:GetClientUserData( 0 );
		SecondarySkillLearnFrame_ShowSkillInfo( t_CurSelectSkill.skillId );		
		SecondaryFixHighLightTex( t_CurSelectSkill.nLineNumOnUI );
	end
	
	this:SetPoint("left","UIParent","left",50,0);
end

function SecondarySkillLearnFrame_OnHide()
	if SecondarySkillLearnDropDownFrame:IsShown() then
		SecondarySkillLearnDropDownFrame:Hide();
	end
	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillType	= SKILLLEARN_TYPE_NONE;
	SecondarySkillHideSkillInfo();
end

function SecondarySkillLearnFrame_OnLoad()
	for i = 1, MAX_VIEW_LINE do
		local tex = getglobal( "SecondarySkillLearnFrameSecondBackBtnSkillBkg" .. i );
		if math.mod( i,2 ) == 0 then
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" );
		end
	end
end

function SecondarySkillLearnFrameScrollUpBtn_OnClick()
	local slider = getglobal("SecondarySkillLearnFrameScrollBar");
	if not slider:IsShown() then
		return;
	end

	if SecondarySkillLearnDropDownFrame:IsShown() then
		SecondarySkillLearnDropDownFrame:Hide();
	end

	local nValue = slider:GetValue() - slider:GetValueStep();

	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function SecondarySkillLearnFram_ScrollDownBtn_OnClick()
	local slider = getglobal( "SecondarySkillLearnFrameScrollBar" );
	if not slider:IsShown() then
		return;
	end

	if SecondarySkillLearnDropDownFrame:IsShown() then
		SecondarySkillLearnDropDownFrame:Hide();
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function SecondarySkillLearnFrame_ScrollBar_OnValueChanged()
	if SecondarySkillLearnDropDownFrame:IsShown() then
		SecondarySkillLearnDropDownFrame:Hide();
	end

	local slider	= getglobal( "SecondarySkillLearnFrameScrollBar" );
	local nValue	= slider:GetValue();
	local nMaxValue = slider:GetMaxValue();

	if nValue <= nMaxValue then
		SecondarySkillUpdateSkillPage( slider:GetValue() + 1 );
	end

	local nLineNum    = t_CurSelectSkill.nLineNumOnUI;

	if 0 == nLineNum then return end--若没有任何子节点被选中

	local btn = getglobal( "SecondarySkillLearnFrameLearnSkillInfo" .. nLineNum );
	btn:DisChecked();

	local nSkillId	  = t_CurSelectSkill.skillId;

	for i = 1, MAX_VIEW_LINE do --遍历每一行找到
		local button = getglobal( "SecondarySkillLearnFrameLearnSkillInfo"..i );
		if nSkillId == button:GetClientUserData( 0 ) then
			t_CurSelectSkill.nLineNumOnUI = i;
			SecondaryFixHighLightTex( i );
			return;
		end
	end
end

function SecondarySkillLearnFrame_StudyBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local nSkillId = t_CurSelectSkill.skillId;
	if 0 == nSkillId then
		return;
	end
	if nSkillId > 0 then
		local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,nSkillId  );
		local money = recipeData.LearnMoneyVal;
		local mainplayer = ActorMgr:getMainPlayer();
		if getPlayerMoney( mainplayer ) < money then
			ShowMidTips( "绑银不足，无法学习" );
			return;
		end

		if checkNeedWarnCost( money ) then
			CostMoneyConfirmFrame:SetClientString("科技学习");
			CostMoneyConfirmFrame:SetClientUserData( 0, nLearnCurSelSkillType );
			CostMoneyConfirmFrame:SetClientUserData( 1, nSkillId );
			SetWarnFrame( "SecondarySkillLearnFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
		SecondarySkillMgr:requestLearnRecipt( nLearnCurSelSkillType,nSkillId );
	end
end

local function HideAlSkilllBtn()
	for i = 1, MAX_VIEW_LINE do
		local btn = getglobal("SecondarySkillLearnFrameLearnSkillInfo"..i);
		btn:Hide();
	end
end

function SecondarySkillUpdateSkillPage( nStartTableNum )
	HideAlSkilllBtn();
	local t_CurDisplayList = {};

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_CurDisplayList = t_TotalSkillList;
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_CurDisplayList = t_CanStudySkillList;
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_CurDisplayList = t_NonStudySkillList;
	end

	local nIndex = 1;
	for i = nStartTableNum, table.getn( t_CurDisplayList ) do
		if nIndex > MAX_VIEW_LINE then break end
		local recipeId = t_CurDisplayList[i];
		local recipeData = SecondarySkillMgr:getRecipeDef( nLearnCurSelSkillType,recipeId );
		if recipeData ~= nil then
			local btn = getglobal( "SecondarySkillLearnFrameLearnSkillInfo" .. nIndex );
			local font = getglobal( "SecondarySkillLearnFrameLearnSkillInfo"..nIndex.."SkillNameFont" );
			local levelFont	= getglobal( "SecondarySkillLearnFrameLearnSkillInfo"..nIndex.."SkillLevelFont" );
			
			font:SetText( recipeData.NameDesc );
			local SecondarySkillData = SecondarySkillMgr:getSecondarySkillData( recipeData.SkillType,recipeData.LearnSkillLevel );
			if SecondarySkillData ~= nil then
				--levelFont:SetText( "("..string.sub(SecondarySkillData.NameDesc,1,4) .. math.fmod(recipeData.LearnSkillLevel-1,10)+1 .." 级)" );
				levelFont:SetText( "需要：" .. t_SubSkillName[recipeData.SkillType] .. "等级" .. recipeData.LearnSkillLevel );
			end

			if SecondarySkillMgr:canLearnSkill( nLearnCurSelSkillType,recipeId ) 
				and SecondarySkillMgr:isLearnedSecondarySkill(nLearnCurSelSkillType,recipeData.LearnSkillLevel ) then
				-- 在全部技能页面中，若技能可以学习，则显示为绿色
				font:SetTextColor( 237, 235, 187 );
				levelFont:SetTextColor( 237, 235, 187 );
			else -- 若技能不能学习，显示为红色
				font:SetTextColor( 152, 60, 60 );
				levelFont:SetTextColor( 152, 60, 60 );
			end
			btn:Show();
			btn:SetClientUserData( 0, recipeId );
			nIndex = nIndex + 1;
		end
	end
end

function SecondarySkillLearnFrameDownArrowBtn_OnClick()
	if SecondarySkillLearnDropDownFrame:IsShown() then
		SecondarySkillLearnDropDownFrame:Hide();
	else
		SecondarySkillLearnDropDownFrame:Show();
		SecondarySkillLearnDropDownFrame:SetSize(98,68);
	end
end

function SecondarySkillLearnDropDownFrame_OnShow()
	SecondarySkillLearnSelectNameBtn1SplitName:SetText("全部");
	SecondarySkillLearnSelectNameBtn2SplitName:SetText("可学习");
	SecondarySkillLearnSelectNameBtn3SplitName:SetText("不可学习");
end

function SecondarySkillLearnDropDownFrameSelectBtn_OnClick()
	SecondarySkillLearnDropDownFrame:Hide();

	if this:GetClientID() == nCurSkillPage then	return end

	nCurSkillPage = this:GetClientID();

	if SKILLPAGE_TOTAL == nCurSkillPage then
		t_SkillList = t_TotalSkillList;
		SecondarySkillLearnFrame_OneLayerBtn_OneLayerName:SetText( "全部" );
	elseif SKILLPAGE_CANSTUDY == nCurSkillPage then
		t_SkillList = t_CanStudySkillList;
		SecondarySkillLearnFrame_OneLayerBtn_OneLayerName:SetText( "可学习" );
	elseif SKILLPAGE_NONSTUDY == nCurSkillPage then
		t_SkillList = t_NonStudySkillList;
		SecondarySkillLearnFrame_OneLayerBtn_OneLayerName:SetText( "不可学习" );
	end

	local slider = getglobal( "SecondarySkillLearnFrameScrollBar" );
	--slider:SetValue( 0 );
	SecondarySetLearnSkillScrollBar( t_SkillList );
	if slider:IsShown() then
		SecondarySkillUpdateSkillPage( slider:GetValue() + 1 ); -- 刚打开页面的时候默认从技能列表第一行开始显示
	else
		SecondarySkillUpdateSkillPage( 1 );
	end

	SecondarySkillHideSkillInfo();

	t_CurSelectSkill.skillId	= 0;
	t_CurSelectSkill.skillLv	= 0;
end

-------------------------------------------
--附魔相关

function EnchantFrameOnLoad()
	this:RegisterEvent("GE_ENCHANT_SUCCEED");
	ItemFirstCallBack("EnchantFrame", EnchantSetEnchantItem );
end

--[[
function EnchantApplyEnchantItem( item, nlisttype, nGridIndex )
	local itemDef = item:getItemDef();
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	if item:isExtend() then
		if itemDef.WearPre.RoleLevelMin > 0 and MainPlayer:getLv() < itemDef.WearPre.RoleLevelMin then
			ShowMidTips("等级不够");
			return;
		end
		if itemDef.WearPre.RoleLevelMax > 0 and MainPlayer:getLv() > itemDef.WearPre.RoleLevelMax then
			ShowMidTips("等级太高");
			return;
		end
		if EnchantFrame:IsShown() then
			EnchantFrame:Hide();
		end
		EnchantUseItem( item );
		return;
	else
		EnchantSetEnchantItem( item, nlisttype, nGridIndex )
	end
end
]]--

local t_ValueChangeTexture = { "EnchantFrameValueBtnValueUp","EnchantFrameValueBtnValueEqual","EnchantFrameValueBtnValueDown","EnchantFrameValueBtnFull"}

function EnchantSetEnchantItem( item, nlisttype, nGridIndex )
	local enchantBtn = getglobal("EnchantFrameComfirmBtn");
	local btn = getglobal( "EnchantFrameItemBtn" );

	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local container	= MainPlayer:getContainer();

	local enchantItem = nil;
	if enchantBtn:GetClientUserData( 0 ) >= 0 and enchantBtn:GetClientUserData( 1 ) >= 0 then
		enchantItem = container:getItem( enchantBtn:GetClientUserData(0),enchantBtn:GetClientUserData(1) );
	end
	--[[
	if enchantItem == nil or enchantItem:getItemId() == 0 then
		ShowMidTips("该卷轴已被使用过");
		return;
	end
	--]]
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入装备");
		return;
	end
	if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
		ShowMidTips( "时装不能放入" )
		return
	end
	
	local itemDef = item:getItemDef();
	local enchantItemDef = enchantItem:getItemDef();
--[[	TODO HERE
	if enchantItemDef.WearPre.RoleLevelMin > 0 and itemDef.WearPre.RoleLevelMin < enchantItemDef.WearPre.RoleLevelMin then
		ShowMidTips("等级不够");
		return;
	end
	if enchantItemDef.WearPre.RoleLevelMax > 0 and itemDef.WearPre.RoleLevelMax > enchantItemDef.WearPre.RoleLevelMax then
		ShowMidTips("等级太高");
		return;
	end
	]]--

	if enchantItemDef.WearPosNum >0 then
		local bFound = false;
		if enchantItemDef.WearPos1 == itemDef.WearPos1 then
			bFound = true;
		end
		if enchantItemDef.WearPos2 == itemDef.WearPos1 then
			bFound = true;
		end
		if enchantItemDef.WearPos3 == itemDef.WearPos1 then
			bFound = true;
		end
		if not bFound then
			ShowMidTips("不能对该装备使用");
			return;
		end
	end
	local valueBtn	= getglobal("EnchantFrameValueBtn");
	local saveBtn 	= getglobal("EnchantFrameValueBtnSaveBtn")
	local cancleBtn	= getglobal("EnchantFrameValueBtnCancleBtn");
	
	if valueBtn:IsShown() and saveBtn:IsShown() then 
		ShowMidTips("本次附魔属性还未保存，请先确定是否取消操作")
		return;
	end
	valueBtn:Show();
	saveBtn:Hide();
	cancleBtn:Hide();
	--如果之前有物品的话，先取消掉
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then		
		local oldItem = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() )
		end
	end
	btn:SetClientUserData(0,nlisttype);
	btn:SetClientUserData(1,nGridIndex);
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	LockCurItem( item, btn:GetParent() );
	local itemdef = item:getItemDef();
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	btnTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemdef, btn:GetName().."UVAnimationTex");

	local ExtendedFromDefID = item:getExtendedFromDefID();
	local extendedResult = item:getExtendedResult();
	local szText = "";
	local curFont 	= getglobal("EnchantFrameValueBtnCurNoteFont");
	local nextFont 	= getglobal("EnchantFrameValueBtnNextNoteFont")
	if ExtendedFromDefID > 0 and extendedResult ~= nil and extendedResult.ResultID ~= 0 then
		local itemDef = getItemDef( ExtendedFromDefID );
		local nValue = 0;
		if itemDef.ExtendedType == ITEM_EXTENDED_TYPE_RESULT then
			local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = extendedResult.ResultID } )
			if itemResultConfig ~= nil then
				local t_ResultValue = { extendedResult.ResultVal1,extendedResult.ResultVal2,extendedResult.ResultVal3,extendedResult.ResultVal4};
				for k = 1, table.getn(t_ResultValue) do
					if t_ResultValue[k] ~= 0 then
						nValue = t_ResultValue[k];
						break;
					end
				end
				szText = itemResultConfig[2].."：+" .. nValue;
			end
		end
		local descRich = getglobal("EnchantFrameDescRich");
		local btn = getglobal( "EnchantFrameComfirmBtn" );
		local rich = btn:GetClientString();
	--	descRich:SetText( rich .. "\n\n#cf0780a当前附魔属性：\n#c00ff00" .. szText, 255, 254, 189 );
	--	descRich:SetText( rich , 255, 254, 189 );
		--elmer
		saveBtn:SetClientUserData(0,nValue);	
		if szText == "" then 
			curFont:SetText("无");
		else 
			curFont:SetText(szText);
		end
		
	elseif ExtendedFromDefID == 0 then
		saveBtn:SetClientUserData(0,0);
		curFont:SetText("无");
	end	
	nextFont:SetText("");
	for i=1, #t_ValueChangeTexture do 
		local tex = getglobal( t_ValueChangeTexture[i] );
		if tex:IsShown() then
			tex:Hide();
		end		
	end
end
local enchantSuccFlag = true;
function EnchantFrameOnEvent()
	if arg1 == "GE_ENCHANT_SUCCEED" then
		enchantSuccFlag = true;
		local btn 		= getglobal("EnchantFrameComfirmBtn");
		if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 1 then
			local modelview = getglobal("EnchantFrameModelView");
			modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
			modelview:setEntityPosition(0,0,100,0);
	--		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
	--		btn:SetClientUserData(0,-1);
	--		btn:SetClientUserData(1,-1);
	--		EnchantFrameFinishBtn:Show();
			
			local MainPlayer = ActorMgr:getMainPlayer();
			if MainPlayer == nil then return end
			local container	= MainPlayer:getContainer();
			local usedItem = container:getItem( btn:GetClientUserData(0), btn:GetClientUserData(1) );
			if usedItem:getItemId() == 0 then 
				UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
			end
			
			local itemBtn = getglobal( "EnchantFrameItemBtn" );
			local item = container:getItem( itemBtn:GetClientUserData(0), itemBtn:GetClientUserData(1) );
			local ExtendedFromDefID = item:getExtendedFromDefID();
			local extendedResult = item:getExtendedResult();
			local szText = "";
			if ExtendedFromDefID > 0 and extendedResult ~= nil and extendedResult.ResultID ~= 0 then
				local itemDef = getItemDef( ExtendedFromDefID );
				if itemDef.ExtendedType == ITEM_EXTENDED_TYPE_RESULT then
					local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = extendedResult.ResultID } )
					if itemResultConfig ~= nil then
						local t_ResultValue = { extendedResult.ResultVal1,extendedResult.ResultVal2,extendedResult.ResultVal3,extendedResult.ResultVal4};
						local nValue = 0;
						for k = 1, table.getn(t_ResultValue) do
							if t_ResultValue[k] ~= 0 then
								nValue = t_ResultValue[k];
								break;
							end
						end
						szText = itemResultConfig[2].."：+" .. nValue;
					end
				end
				local descRich = getglobal("EnchantFrameDescRich");
				local rich = btn:GetClientString();
			--	descRich:SetText( rich .. "\n\n#cf0780a当前附魔属性：\n#c00ff00" .. szText, 255, 254, 189 );
			end	
		end
	end
end

function EnchantFrameCloseBtn_OnClick()
	local valueBtn	= getglobal("EnchantFrameValueBtn");
	local saveBtn 	= getglobal("EnchantFrameValueBtnSaveBtn")
	if valueBtn:IsShown() and saveBtn:IsShown() then 
		if MessageBoxBlock("附魔","本次附魔属性还未保存，确定取消附魔操作？",EnchantFrame) then 
			EnchantFrame:Hide();
		end
	else 
		EnchantFrame:Hide();
	end	
end

local nTmpValue;
local nTmpFromID = 0;
function UpdateEnchantFrameValue( nFromId )
	local saveBtn 	= getglobal("EnchantFrameValueBtnSaveBtn");
	local cancleBtn	= getglobal("EnchantFrameValueBtnCancleBtn");
	saveBtn:Show();
	cancleBtn:Show();
	local nextFont  = getglobal("EnchantFrameValueBtnNextNoteFont");
	
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container	= MainPlayer:getContainer();
	local itemBtn = getglobal( "EnchantFrameItemBtn" );
	
	local extendedResult = SecondarySkillMgr:getExtendResult();
	local szText = "";
	local nValue = 0;
	if nFromId > 0 and extendedResult ~= nil and extendedResult.ResultID ~= 0 then
		local itemDef = getItemDef( nFromId );
		if itemDef.ExtendedType == ITEM_EXTENDED_TYPE_RESULT then
			local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = extendedResult.ResultID } )
			if itemResultConfig ~= nil then
				local t_ResultValue = { extendedResult.ResultVal1,extendedResult.ResultVal2,extendedResult.ResultVal3,extendedResult.ResultVal4};
				for k = 1, table.getn(t_ResultValue) do
					if t_ResultValue[k] ~= 0 then
						nValue = t_ResultValue[k];
						break;
					end
				end
				szText = itemResultConfig[2].."：+" .. nValue;
			end
		end
		local nextFont = getglobal("EnchantFrameValueBtnNextNoteFont");
		nextFont:SetText(szText);	
	end
	coroutine.wrap(setValueUpDownTex)( nValue,nFromId );
	nTmpValue = nValue;
	nTmpFromID = nFromId;
end

function setValueUpDownTex( nValue,nFromId )	
	for i=1, #t_ValueChangeTexture do 
		local tex = getglobal( t_ValueChangeTexture[i] );
		if tex:IsShown() then
			tex:Hide();
		end		
	end
	local saveBtn 	= getglobal("EnchantFrameValueBtnSaveBtn");
	local oldValue	= saveBtn:GetClientUserData(0);
	local nMaxValue = getExtendFullValue( nFromId );
	if nValue == nMaxValue - 1 then 
		EnchantFrameValueBtnFull:Show();
		if MessageBoxBlock("附魔","本次附魔已将属性附魔至满值，请点击确定保存",EnchantFrame) then 
			EnchantFrameSaveBtn_onClick();
		end
	elseif nValue > oldValue then 
		EnchantFrameValueBtnValueUp:Show();
	elseif nValue < oldValue then 
		EnchantFrameValueBtnValueDown:Show();
	else 
		EnchantFrameValueBtnValueEqual:Show();
	end
end

function getExtendFullValue( nFromId )
	local nMax = 0;
	local itemDef = getItemDef( nFromId );
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID > 0 then
			local t_ResultValue = { itemDef.Result[i-1].ResultVal1,itemDef.Result[i-1].ResultVal2,itemDef.Result[i-1].ResultVal3,itemDef.Result[i-1].ResultVal4};
			for k = 1, table.getn(t_ResultValue) do
				if t_ResultValue[k] > nMax then
					nMax = t_ResultValue[k];
				end
			end
		end
	end
	return nMax;    
end
function EnchantFrameCancleBtn_onClick()
	local nextFont 	= getglobal("EnchantFrameValueBtnNextNoteFont");
	local saveBtn 	= getglobal("EnchantFrameValueBtnSaveBtn");
	local cancleBtn	= getglobal("EnchantFrameValueBtnCancleBtn");
	nextFont:SetText("");
	saveBtn:Hide();
	cancleBtn:Hide();
	for i=1, #t_ValueChangeTexture do 
		local tex = getglobal( t_ValueChangeTexture[i] );
		if tex:IsShown() then
			tex:Hide();
		end		
	end
end

function EnchantFrameSaveBtn_onClick() 
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container	= MainPlayer:getContainer();
	local btn 		= getglobal("EnchantFrameValueBtn");
	local curFont 	= getglobal("EnchantFrameValueBtnCurNoteFont");
	local nextFont  = getglobal("EnchantFrameValueBtnNextNoteFont");
	local saveBtn 	= getglobal("EnchantFrameValueBtnSaveBtn");
	local cancleBtn	= getglobal("EnchantFrameValueBtnCancleBtn");
	curFont:SetText( nextFont:GetText() );
	nextFont:SetText("");
	saveBtn:Hide();
	cancleBtn:Hide();
	for i=1, #t_ValueChangeTexture do 
		local tex = getglobal( t_ValueChangeTexture[i] );
		if tex:IsShown() then
			tex:Hide();
		end		
	end
	local itemBtn = getglobal("EnchantFrameItemBtn");
	if itemBtn:GetClientUserData(0) >= 0 and itemBtn:GetClientUserData(1) >= 0 then
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		local descList		= item:getListType();
		local descGrid		= item:getGridIdx();
		SecondarySkillMgr:saveExtendResult( nTmpFromID,descList, descGrid );
		saveBtn:SetClientUserData( 0,nTmpValue ) ; 
	end
end

function EnchantFrameOnShow()
	local itemBtn = getglobal("EnchantFrameItemBtn");
	local itembox = getglobal("EnchantFrameItemBtnBoxTexture");
	itemBtn:SetClientUserData(0,-1);
	itemBtn:SetClientUserData(1,-1);
	itembox:Show();
	EnchantFrameFinishBtn:Hide();
	EnchantFrameValueBtn:Hide();
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	this:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	enchantSuccFlag = true;
end

function EnchantFrameOnHide()
	local btn = getglobal("EnchantFrameComfirmBtn");
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local containerType = btn:GetClientUserData(0);
	local itemID		= btn:GetClientUserData(2);
	ClearItemLock( "EnchantFrameComfirmBtn" );
	if containerType >=0 and itemID >= 0 then
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		btn:SetClientUserData(2,-1);
		btn:SetClientString("");
	end
	local ItemBtn = getglobal("EnchantFrameItemBtn");
	if ItemBtn:GetClientUserData(0) >= 0 and ItemBtn:GetClientUserData(1) >= 0 then
		UnLockItem( ItemBtn:GetClientUserData(0),ItemBtn:GetClientUserData(1),0 );
		ItemBtn:SetClientUserData(0,-1);
		ItemBtn:SetClientUserData(1,-1);
		local tex = getglobal(ItemBtn:GetName().."IconTexture");
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local UVAnimationTex = getglobal( ItemBtn:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
	end
end

local bExtend;
local KeyInText = [[★ 元素伤害属性只能刻印在武器、肩甲上
★ 元素抗性属性只能刻印在帽子、衣服、面饰、鞋子上
★ 刻印上装备时，从该元素印记中随机选取一个值
★ 刻印后的元素属性能维持24小时，到期效果消失
]]
function EnchantUseItem( used_item )
	local mainplayer	= ActorMgr:getMainPlayer();
	local container		= mainplayer:getContainer();
	local containerType = used_item:getListType();
	local itemID		= used_item:getItemId();
	
	for i = 0, container:getContainerMaxNum( containerType ) - 1 do
		local bagItem = container:getItem(containerType, i)
		if bagItem:getItemId() == itemID then
			LockItem( containerType, i, itemID,"EnchantFrameComfirmBtn" );
		end
	end

	local btn = getglobal( "EnchantFrameComfirmBtn" );
	btn:SetClientUserData( 0,containerType );
	btn:SetClientUserData( 1,used_item:getGridIdx() );
	btn:SetClientUserData( 2,itemID );
	
	--设置名称 和描述
	local itemDef = used_item:getItemDef();
	local nameFont = getglobal("EnchantFrameNameFont");
	local descRich = getglobal("EnchantFrameDescRich");
	nameFont:SetText( itemDef.Name );	
	EnchantFrame:Show();
	if used_item:isExtend() then
		bExtend = true;
		EnchantFrameTitleFont:SetText("附魔");
		EnchantFrameComfirmBtn:SetText("附魔");
	else
		bExtend = false;
		EnchantFrameTitleFont:SetText("刻印");
		EnchantFrameComfirmBtn:SetText("刻印");
	end
	if bExtend then
		descRich:SetText( itemDef.ItemDesc,255,254,189 );
		btn:SetClientString( itemDef.ItemDesc );
	else
		descRich:SetText(KeyInText,255,254,189);
	end
end

function EnchantReceiveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	if dragfrom == DRAG_ENCHANT then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if dragfrom ~= CONTAINER_TYPE_SUNDRIES and dragfrom ~= CONTAINER_TYPE_STASH then
		return;
	end
	local valueBtn	= getglobal("EnchantFrameValueBtn");
	local saveBtn = getglobal("EnchantFrameValueBtnSaveBtn")
	if valueBtn:IsShown() and saveBtn:IsShown() then 
		ShowMidTips("本次附魔属性还未保存，请先确定是否取消操作")
		return; 
	end
	local container	= MainPlayer:getContainer();	
	local item = container:getItem( dragfrom, dragdata1 );
	EnchantSetEnchantItem( item,dragfrom,dragdata1 );

	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function EnchantOnRightClick( szBtnName )
	local btn	= getglobal( szBtnName );
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local valueBtn	= getglobal("EnchantFrameValueBtn");
	local saveBtn = getglobal("EnchantFrameValueBtnSaveBtn")
	if valueBtn:IsShown() and saveBtn:IsShown() then
		ShowMidTips("本次附魔属性还未保存，请先确定是否取消操作")
		return; 
	end
	local container = MainPlayer:getContainer();
	if btn:GetClientUserData(0) < 0 or btn:GetClientUserData(1) < 0 then
		return
	end
	local item = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
	if item:getItemId() ~= 0 then
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
		local btnTex = getglobal( this:GetName().."IconTexture" );
		btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local btn = getglobal( "EnchantFrameComfirmBtn" );
		local rich = btn:GetClientString();
		local descRich = getglobal("EnchantFrameDescRich");
		descRich:SetText( rich, 255, 254, 189 );
		local valueBtn = getglobal("EnchantFrameValueBtn")
		valueBtn:Hide();
	end
end

--确认附魔
local t_LastClickTime = 0;
function EnchantFrameComfirmBtn_OnClick()
	if IsMessageBoxBlock(EnchantFrame) then 
		return;
	end
	if os.clock() - t_LastClickTime > 2 then
		t_LastClickTime = os.clock();
		enchantSuccFlag = true;
	end
	if not enchantSuccFlag then 
		return;
	end
	local itemBtn = getglobal("EnchantFrameItemBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
--[[
	if this:GetClientUserData(0) < 0 or this:GetClientUserData(1) < 0 then
		ShowMidTips("该卷轴已被使用过");
		return;
	end
	--]]
	local enchantItem = container:getItem( this:GetClientUserData(0),this:GetClientUserData(1) );
	if enchantItem:getItemId() == 0 then
		UnLockItem( this:GetClientUserData(0),this:GetClientUserData(1),0);
		local nMaxNum = container:getContainerMaxNum( this:GetClientUserData(0) ) - 1;
		for i = 0, nMaxNum do
			local bagItem = container:getItem(this:GetClientUserData(0), i)
			if bagItem:getItemId() == this:GetClientUserData(2) then
				this:SetClientUserData( 1, i );
				break;				
			end
			if i == nMaxNum then 
				this:SetClientUserData( 1, -1 );
				ShowMidTips("附魔失败，附魔卷轴不足！");
				return;
			end	
		end	
	end
	local itemBtn = getglobal("EnchantFrameItemBtn");	
	if bExtend then
		if itemBtn:GetClientUserData(0) == -1 or itemBtn:GetClientUserData(1) == -1 then		
			ShowMidTips("请放入需要附魔的装备");
			return;
		end
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要附魔的装备");
			return;
		end
		if not item:isItemBind() and enchantItem:isItemBind() then
			MessageBox("提示","由于" .. enchantItem:getDisplayName().. "是绑定的，附魔之后您的装备也会变成绑定物品，确认要附魔吗？");
			MessageBoxFrame:SetClientString("绑定附魔");
			return;
		end
		--[[
		if item:getExtendedFromDefID() > 0 then
			MessageBox( "提示","是否替换当前附魔属性？" );
			MessageBoxFrame:SetClientString("覆盖附魔");
			return;
		end	
		--]]
		EnchantItem();
	else
		if itemBtn:GetClientUserData(0) == -1 or itemBtn:GetClientUserData(1) == -1 then		
			ShowMidTips("请放入需要刻印的装备");
			return;		
		end
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要刻印的装备");
			return;
		end		
		if not item:isItemBind() and enchantItem:isItemBind() then
			MessageBox("提示","由于" .. enchantItem:getDisplayName().. "是绑定的，刻印之后您的装备也会变成绑定物品，确认要刻印吗？");
			MessageBoxFrame:SetClientString("绑定刻印");
		end
		if item:getAddAttrInst().AddAttrID > 0 then
			MessageBox( "提示","是否替换当前刻印属性？" );
			MessageBoxFrame:SetClientString("覆盖刻印");
			return;
		end		
		KeyinItem();
	end
	enchantSuccFlag = false;
	t_LastClickTime = os.clock();
end

function KeyinItem()
	local btn = getglobal("EnchantFrameComfirmBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local itemBtn = getglobal("EnchantFrameItemBtn");
	if itemBtn:GetClientUserData(0) >= 0 and itemBtn:GetClientUserData(1) >= 0 then
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要刻印的装备");
			return;
		end
		local srcListType	= btn:GetClientUserData( 0 );
		local srcGrid 		= btn:GetClientUserData( 1 );
		local descList		= item:getListType();
		local descGrid		= item:getGridIdx();
		local gameTrump = mainplayer:getTrump();
		--TODO HERE 判断绑定
		gameTrump:requestKeyInItem( srcListType,srcGrid,descList,descGrid );		
	end

end

function EnchantItem()
	local btn = getglobal("EnchantFrameComfirmBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local itemBtn = getglobal("EnchantFrameItemBtn");
	if itemBtn:GetClientUserData(0) >= 0 and itemBtn:GetClientUserData(1) >= 0 then
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要附魔的装备");
			return;
		end
		local srcListType	= btn:GetClientUserData( 0 );
		local srcGrid 		= btn:GetClientUserData( 1 );
		local descList		= item:getListType();
		local descGrid		= item:getGridIdx();
		--TODO HERE 判断绑定
		SecondarySkillMgr:requestUseEnchant( srcListType,srcGrid,descList,descGrid );		
	end
end

-- 显示学习生活技能道具tips相关

function getSecondarySkillNeed( itemDef )
	local recipeType,recipeId = getSecondarySkillItemData( itemDef );
	if recipeType > 0 then
		local recipeDef = SecondarySkillMgr:getRecipeDef( recipeType,recipeId );
		if recipeDef ~= nil then
			local SecondarySkill = SecondarySkillMgr:getSecondarySkillData( recipeDef.SkillType,recipeDef.LearnSkillLevel );
			if SecondarySkill ~= nil then
				if SecondarySkillMgr:isLearnedRecipe( recipeType,recipeId ) then
					return "已经学会\b" .. recipeDef.NameDesc
				else
					--return "需要技能\b"..SecondarySkill.NameDesc.. math.fmod(recipeDef.LearnSkillLevel-1,10)+1 .." 级";
					return "需要技能\b" .. t_SubSkillName[recipeDef.SkillType] .. "等级" .. recipeDef.LearnSkillLevel;
				end
			end
		end
	end
	return "";
end

function getSecondarySkillColor( itemDef )
	local recipeType,recipeId = getSecondarySkillItemData( itemDef );
	if recipeType > 0 then
		local recipeDef = SecondarySkillMgr:getRecipeDef( recipeType,recipeId );
		if recipeDef ~= nil then
			if SecondarySkillMgr:canLearnSkill( recipeType,recipeId ) then
				return GetColor()[COLOR_WHITE];
			else
				return GetColor()[COLOR_RED];
			end
		end
	end
	return GetColor()[COLOR_WHITE];
end

function getSecondarySkillItemData( itemDef )
	if itemDef ~= nil then
		for i=1,MAX_ITEM_RESULT do
			if itemDef.Result[ i-1 ].ResultID == RESULT_SECONDARY_SKILL_RECIPE then
				return itemDef.Result[ i-1 ].ResultVal1,itemDef.Result[ i-1 ].ResultVal2;
			end
		end
	end
	return 0,0;
end

function getSecondarySkillRequireItemDesc( RecipeDef )
	local szText = "";
	for i=1,MAX_RECIPE_STUFF do
		if RecipeDef.Stuff[i-1].ItemID ~= 0 then
			local itemDef = getItemDef( RecipeDef.Stuff[i-1].ItemID );
			if itemDef ~= nil then
				szText = szText..itemDef.Name.."("..RecipeDef.Stuff[i-1].Num..") " ;
			end
		end
	end
	return szText;
end

function DetailBkgPriceIconBtn_OnEnter()
	local szText = "";
	local moneyBtn	= getglobal("SecondarySkillFrameDetailBkgPriceIconBtn");
	if moneyBtn:GetClientUserData( 0 ) == ATTR_ID_BULL then 
		szText = "制作配方时优先消耗绑银，绑银不足将消耗银币";
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
					button = this:GetName() } );
	end
end


--科技生产界面相关用户提示tips
--[[
function SecondarySkillFrameSkillTitleLabelBtn_OnEnter()
	local szText = 职称决定技能等
级的提升上限，
当等级无法提升
时需到相应技能
的训练师那里提
升职称
	SetGameTooltips( this:GetName(), szText )
end
--]]

function SecondarySkillFrameSkillTitleBtn_OnEnter()
	local szText = [[见习对应技能等级： 1-10
熟练对应技能等级：11-20
资深对应技能等级：21-30
大师对应技能等级：31-40
宗师对应技能等级：41-50
]]
	SetGameTooltips( this:GetName(), szText )
end

function SecondarySkillFrameSkillLevelLebelBtn_OnEnter()
	local szText = [[通过熟练度
提升等级
]]
	SetGameTooltips( this:GetName(), szText )
end

function SecondarySkillFrameSkillExpLabelBtn_OnEnter()
	local szText = [[通过制作物品
来增加熟练度
]]
	SetGameTooltips( this:GetName(), szText )
end

function SecondarySkillFrameSendLinkLabelBtn_OnEnter()
	--[[
	local szSkillName = t_SubSkillName[nCurSelSkillType];
	local szText = ( "发送"..szSkillName..技能到
聊天框，供其他
玩家查看
)
	SetGameTooltips( this:GetName(), szText )
	--]]
end
------------------ 鉴定器------------------------
local ExtistTime = 0;

function ArmIdentifySuccess( nSucc )
	local btn = getglobal("IdentifyFrameComfirmBtn");
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 1 then
		local modelview = getglobal("IdentifyFrameModelView");
		modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
		modelview:setEntityPosition(0,0,100,0);
		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		local Rich = getglobal("IdentifyFrameCheckRich");
		Rich:SetText( "已鉴定",232, 226, 72 );
		ExtistTime = getServerTime().sec;
	end
end
function IdentifyFrameOnLoad() 
	ItemFirstCallBack("IdentifyFrame", IdentifySetIdentifyItem );
	local Rich = getglobal("IdentifyFrameCheckRich");
	Rich:setCenterLine( true );
end

function IdentifyFrameOnShow()
	local itemBtn = getglobal("IdentifyFrameItemBtn");
	local itembox = getglobal("IdentifyFrameItemBtnBoxTexture");
	itemBtn:SetClientUserData(0,-1);
	itemBtn:SetClientUserData(1,-1);
	itembox:Show();
--	EnchantFrameFinishBtn:Hide();

	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	this:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	local descRich = getglobal("IdentifyFrameCheckRich");
	descRich:SetText( "", 232, 226, 72 );	
end

function IdentifyFrameOnHide() 
	local btn = getglobal("IdentifyFrameComfirmBtn");
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then
		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
	end
	local ItemBtn = getglobal("IdentifyFrameItemBtn");
	if ItemBtn:GetClientUserData(0) >= 0 and ItemBtn:GetClientUserData(1) >= 0 then
		UnLockItem( ItemBtn:GetClientUserData(0),ItemBtn:GetClientUserData(1),0 );
		ItemBtn:SetClientUserData(0,-1);
		ItemBtn:SetClientUserData(1,-1);
		local tex = getglobal(ItemBtn:GetName().."IconTexture");
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local UVAnimationTex = getglobal( ItemBtn:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
	end
	ExtistTime = 0;
end
local t_LastIdentityClickTime = 0 ;
function IdentifyFrameComfirmBtn_OnClick()
	if os.clock() - t_LastIdentityClickTime < 3 then
		return;
	end
	t_LastIdentityClickTime = os.clock();
	local itemBtn = getglobal("IdentifyFrameItemBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();

	if this:GetClientUserData(0) < 0 or this:GetClientUserData(1) < 0 then
		ShowMidTips("该鉴定仪已被使用过");
		return;
	end
	local identyItem = container:getItem( this:GetClientUserData(0),this:GetClientUserData(1) );
	if identyItem:getItemId() == 0 then
		return;
	end	

	if itemBtn:GetClientUserData(0) == -1 or itemBtn:GetClientUserData(1) == -1 then		
		ShowMidTips("请放入需要鉴定的装备");
		return;
	end
	local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
	if item:getItemId() == 0 then
		ShowMidTips("请放入需要鉴定的装备");
		return;
	end

	IdentifyItem();
end
function IdentifyItem()
	local btn = getglobal("IdentifyFrameComfirmBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local itemBtn = getglobal("IdentifyFrameItemBtn");
	if itemBtn:GetClientUserData(0) >= 0 and itemBtn:GetClientUserData(1) >= 0 then
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要鉴定的装备");
			return;
		end
		local descList		= btn:GetClientUserData( 0 );
		local descGrid		= btn:GetClientUserData( 1 );
		local srcListType	= item:getListType();
		local srcGrid		= item:getGridIdx();
		--TODO HERE
		GameArmProce:armIdentifyStart( srcListType,srcGrid,descList,descGrid );		
	end
end
-- 鉴定右键功能
function IdentifyOnRightClick( szBtnName )
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
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
		local btnTex = getglobal( this:GetName().."IconTexture" );
		btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
--		local btn = getglobal( "IdentifyFrameComfirmBtn" );
--		local rich = btn:GetClientString();
		local Rich = getglobal("IdentifyFrameCheckRich");
		Rich:SetText( "",232, 226, 72 );
	end
end

function IdentityUseItem( used_item )
	LockItem( used_item:getListType(),used_item:getGridIdx(),used_item:getItemId() );
	local btn = getglobal( "IdentifyFrameComfirmBtn" );
	btn:SetClientUserData( 0,used_item:getListType() );
	btn:SetClientUserData( 1,used_item:getGridIdx() );

	--设置名称 和描述
	local itemDef = used_item:getItemDef();
	local nameFont = getglobal("IdentifyFrameNameFont");
	local descRich = getglobal("IdentifyFrameDescRich");
	nameFont:SetText( itemDef.Name );	
	IdentifyFrame:Show();	
	descRich:SetText( itemDef.ItemDesc,255,254,189 );
--	btn:SetClientString( itemDef.ItemDesc );
end
function IdentityReceiveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	if dragfrom ~= CONTAINER_TYPE_SUNDRIES and dragfrom ~= CONTAINER_TYPE_STASH then
		return;
	end
	local container	= MainPlayer:getContainer();	
	local item = container:getItem( dragfrom, dragdata1 );
	IdentifySetIdentifyItem( item,dragfrom,dragdata1 );

	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function IdentifySetIdentifyItem( item, nlisttype, nGridIndex )
	local identityBtn = getglobal("IdentifyFrameComfirmBtn");
	local btn = getglobal( "IdentifyFrameItemBtn" );

	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local container	= MainPlayer:getContainer();

	local identifyItem = nil;
	if identityBtn:GetClientUserData( 0 ) >= 0 and identityBtn:GetClientUserData( 1 ) >= 0 then
		identifyItem = container:getItem( identityBtn:GetClientUserData(0),identityBtn:GetClientUserData(1) );
	end
	if identifyItem == nil or identifyItem:getItemId() == 0 then
		ShowMidTips("该鉴定仪已被使用过");
		return;
	end
	if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
		ShowMidTips("请放入装备");
		return;
	end
	
	if not item:isArmIdentity() then 
		ShowMidTips("该装备不需要进行鉴定操作")
		return;
	end
	if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
		ShowMidTips( "时装不能放入" )
		return
	end
	local itemDef = item:getItemDef();
--	local identifyItemDef = identifyItem:getItemDef();
	local identityLv		= identifyItem:getIdentityLevel();
	if  identityLv > 0 and identityLv < itemDef.WearPre.RoleLevelMin then
		ShowMidTips("装备等级与鉴定器不符");
		return;
	end

	--如果之前有物品的话，先取消掉
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then		
		local oldItem = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() )
		end
	end
	btn:SetClientUserData(0,nlisttype);
	btn:SetClientUserData(1,nGridIndex);	
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	LockCurItem( item, btn:GetParent() );
	local itemdef = item:getItemDef();
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	btnTex:SetTexture( IconPath );
	local Rich = getglobal("IdentifyFrameCheckRich");
	Rich:SetText( "未鉴定",232, 226, 72 );
end
function UpdateIdentifyFrame()
	if ExtistTime > 0 then 
		local curTime = getServerTime().sec ;
		if curTime - ExtistTime > 1 then 
			IdentifyFrame:Hide();
		end
	end
end
local ModelViewAngle = 0;
function UpdateBackFrameModelView( frameName , backId )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local Eqiup 	= MainPlayer:getEquip();
	local nRace 	= MainPlayer:getRace();
	local nGender 	= MainPlayer:getGender();
	local ModelViewer = getglobal( frameName.."BackBtnModelView");
	
	Eqiup:setAvatarAttrib( nRace, nGender );
	setAvatarAttrib( POS_ID_HAIR,MainPlayer:getHair() + 1 );
	setAvatarAttrib( POS_ID_FACE,MainPlayer:getFace());
	setAvatarAttrib( POS_ID_HEAD,MainPlayer:getHead());
	local nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_BODY );
	if not Eqiup:getItem(nWearPos):isValidEnd() then
		setAvatarAttrib( POS_ID_FASHION_BODY,Eqiup:getItem(nWearPos):getItemId() );
	end
	nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_HEAD );
	if not Eqiup:getItem(nWearPos):isValidEnd() then
		setAvatarAttrib( POS_ID_FASHION_HEAD,Eqiup:getItem(nWearPos):getItemId() );
	end
	nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_BACK );
	if backId > 0 then 
		setAvatarAttrib( POS_ID_FASHION_BACK,backId );
	else
		if not Eqiup:getItem(nWearPos):isValidEnd() and EquipFrame_DisBack:GetCheckState() then
			setAvatarAttrib( POS_ID_FASHION_BACK,Eqiup:getItem(nWearPos):getItemId() );
		end
	end
	nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_SHOES );
	if not Eqiup:getItem(nWearPos):isValidEnd() then
		setAvatarAttrib( POS_ID_FASHION_SHOES,Eqiup:getItem(nWearPos):getItemId() );
	end
	nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_HAND );
	if not Eqiup:getItem(nWearPos):isValidEnd() then
		setAvatarAttrib( POS_ID_FASHION_HAND,Eqiup:getItem(nWearPos):getItemId() );
	end
	nWearPos = GetFashionSuitPos( EQUIP_POS_FASHION_LEG );
	if not Eqiup:getItem(nWearPos):isValidEnd() then
		setAvatarAttrib( POS_ID_FASHION_LEG,Eqiup:getItem(nWearPos):getItemId() );
	end
	setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	g_ConfigFashionFair();
	for j=1, CLIENT_MAX_AVATAR_ATTRIB do
		if Eqiup:getAvatarId( j-1 ) ~= 0 then
			if j == 1 then
				ModelViewer:setAvatarAttrib(0, j-1, Eqiup:getAvatarId( j-1 ),Eqiup:getAvatarStarLv( j-1 ), true );
			else
				ModelViewer:setAvatarAttrib(0, j-1, Eqiup:getAvatarId( j-1 ),Eqiup:getAvatarStarLv( j-1 ) );
			end
		end
	end
	ModelViewer:setEntityAvatar( 0 );
	ModelViewer:setEntityRotate(0,ModelViewAngle);	
	
	local leftarmid		= getAvatarAttrib( POS_ID_LEFTHAND );
	local rightarmid	= getAvatarAttrib( POS_ID_RIGHTHAND );	
	
	local t_AvartarControl	= GetAvartarControl();
	local nAnimID = t_AvartarControl:getAvatarAnim( {	["race"] = GetInitRace( nRace ), ["leftHandItem"] = leftarmid, 
														["rightHandItem"] = rightarmid } );
	ModelViewer:playEntityAnim(0, nAnimID, nil );
	--男性小些
	if nGender == 1 then
		ModelViewer:setPetScale(0,0.7,0.7,0.7);--设置缩放
	else
		ModelViewer:setPetScale(0,0.8,0.8,0.8);--设置缩放
	end
	ModelViewer:setEntityPosition(0,0,20,0);
end
function BackItemBtnTemplate_OnClick()
	if( arg1 == "RightButton" ) then
		if UIIsInDragState() then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		--背饰进阶
		if this:GetParent() == "BackUpFrame" then 
			BackUpItemOnRightClick( this:GetName() );
			return;
		end
		--背饰改造
		if this:GetParent() == "BackRemakeFrame" then 
			BackRemakeItemOnRightClick( this:GetName() );
			return;
		end
	end
	if( arg1 == "LeftButton" ) then
		if this:GetClientUserData(2) <= 0 then 
			return;
		end
		--背饰进阶
		if this:GetParent() == "BackUpFrame" then 
			BackUpItemOnLeftClick( this:GetName() );
			return;
		end
		--背饰改造
		if this:GetParent() == "BackRemakeFrame" then 
			BackRemakeItemOnLeftClick( this:GetName() );
			return;
		end
	end
end

function BackItemBtnTemplate_OnEnter()
	if this:GetClientUserData(2) > 0 then 
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( this:GetClientUserData(2) ) );
	end
end
function BackItemBtnTemplate_OnReceiveDrag()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	local container = MainPlayer:getContainer();
	if container == nil then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return
	end
	-- 背饰进阶
	if this:GetParent() == "BackUpFrame" then 
		BackUpItemReceiveDrag( this:GetName(),UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end
	-- 背饰改造
	if this:GetParent() == "BackRemakeFrame" then 
		BackRemakeItemReceiveDrag( this:GetName(),UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end
end
----------------------------------背饰进阶---------------------------------------
local nLastModelviewMouseClickX = 0 ;
function BackFrameBackBtn_OnMouseDown()
	if arg1 == "LeftButton" then
		nLastModelviewMouseClickX = GetCursorPosX();
	end
end
function BackFrameBackBtn_OnMouseMove()
	if this:GetPushedState() then
		local nCurMouseClickX = GetCursorPosX();
		ModelViewAngle = ModelViewAngle - (nCurMouseClickX - nLastModelviewMouseClickX) ;
		BackUpFrameBackBtnModelView:setEntityRotate(0, ModelViewAngle);
		nLastModelviewMouseClickX = nCurMouseClickX;
	end
end
function BackUpFrameOnLoad()
	ItemFirstCallBack("BackUpFrame", BackUpFrameSetSuitItem );
end

function BackUpFrameOnShow()
	local itemBtn = getglobal("BackUpFrameItemBtn");
	itemBtn:SetClientUserData(0,-1);
	itemBtn:SetClientUserData(1,-1);
	itemBtn:SetClientUserData(2,0);
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	if BackRemakeFrame:IsShown() then 
		BackRemakeFrame:Hide();
	end
	SetPreItemBtnInfo();
	ModelViewAngle = 0;
	this:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	UpdateBackFrameModelView( this:GetName(),0 );
end

local BackExtistTime = 0;
function BackUpFrameOnHide()
	local btn = getglobal("BackUpFrameComfirmBtn");
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then
		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
	end
	local ItemBtn = getglobal("BackUpFrameItemBtn");
	if ItemBtn:GetClientUserData(0) >= 0 and ItemBtn:GetClientUserData(1) >= 0 then
		UnLockItem( ItemBtn:GetClientUserData(0),ItemBtn:GetClientUserData(1),0 );
		ItemBtn:SetClientUserData(0,-1);
		ItemBtn:SetClientUserData(1,-1);
		ItemBtn:SetClientUserData(2,0);
		local tex = getglobal(ItemBtn:GetName().."IconTexture");
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local UVAnimationTex = getglobal( ItemBtn:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
	end
	local modelview = getglobal("BackUpFrameModelView");
	modelview:Hide();
	BackExtistTime = 0;
	ModelViewAngle = 0;
end

function BackLevelUpSuccess( nSucc )
	local btn = getglobal("BackUpFrameComfirmBtn");
	if nSucc == 0 then 
		ShowMidTips("进阶失败！！！");
		return;
	end
	if nSucc == 1 and btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 1 then
		local modelview = getglobal("BackUpFrameModelView");
		modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
		modelview:setEntityPosition(0,0,100,0);
		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		BackExtistTime = getServerTime().sec;
	end
end
function UpdateBackUpFrame()
	if BackExtistTime > 0 then 
		local curTime = getServerTime().sec ;
		if curTime - BackExtistTime > 1 then 
			BackUpFrame:Hide();
		end
	end
end
-- 预览背饰相关
function SetPreItemBtnInfo()
	local btn 		= getglobal( "BackUpFrameItemBtn" );
	local afterBtn 	= getglobal( "BackUpFramePreItemBtn");
	local afterTex 	= getglobal( afterBtn:GetName().."IconTexture" );
	local uvTex		= getglobal( afterBtn:GetName().."UVAnimationTex" );
	local itemID	= btn:GetClientUserData(2);
	if btn:GetClientUserData(0) < 0 then 
		afterBtn:SetClientUserData(2,0);
		afterTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		uvTex:Hide();
	else 
		local backDef	= GameArmProce:getBackLevelDef( itemID );
		local afterDef 	= getItemDef( backDef.NextItemID );
		local texPath 	= GetItemEquipIconPath()..afterDef.IconID..".tga";
		if not IsInExistence( texPath ) then
			texPath = DEFAULT_ITEM_PATH;
		end
		afterBtn:SetClientUserData(2,backDef.NextItemID);
		afterTex:SetTexture( texPath );
		SetEquipImportantEffectUV( afterDef, afterBtn:GetName().."UVAnimationTex" );		
	end 
end
function BackLevelUpItemUse( used_Item )
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local itemDef = used_Item:getItemDef();
	if itemDef.WearPre.RoleLevelMin > 0 and MainPlayer:getLv() < itemDef.WearPre.RoleLevelMin then
		ShowMidTips("等级不够");
		return;
	end
	if BackUpFrame:IsShown() then
		BackUpFrame:Hide();
	end
	LockItem( used_Item:getListType(),used_Item:getGridIdx(),used_Item:getItemId() );
	local btn = getglobal( "BackUpFrameComfirmBtn" );
	btn:SetClientUserData( 0,used_Item:getListType() );
	btn:SetClientUserData( 1,used_Item:getGridIdx() );

	--设置名称 和描述
	
	local nameFont = getglobal("BackUpFrameNameFont");
	local descRich = getglobal("BackUpFrameDescRich");
	nameFont:SetText( itemDef.Name );	
	BackUpFrame:Show();	
	descRich:SetText( itemDef.ItemDesc,255,254,189 );
end

function BackUpItemOnRightClick( szBtnName )
	if szBtnName ~= "BackUpFrameItemBtn" then 
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
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		btn:SetClientUserData(2,0);
		local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
		local btnTex = getglobal( this:GetName().."IconTexture" );
		btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	end
	SetPreItemBtnInfo();
	UpdateBackFrameModelView( "BackUpFrame",btn:GetClientUserData(2) );
end
function BackUpItemOnLeftClick( itemBtnName )
	local btn 		= getglobal( itemBtnName );
	local frameName = btn:GetParent();
	UpdateBackFrameModelView( frameName,btn:GetClientUserData(2) );
end

function BackUpItemReceiveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if szBtnName ~= "BackUpFrameItemBtn" then 
		return;
	end
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	if dragfrom ~= CONTAINER_TYPE_SUNDRIES and dragfrom ~= CONTAINER_TYPE_STASH then
		return;
	end
	local container	= MainPlayer:getContainer();	
	local item = container:getItem( dragfrom, dragdata1 );
	BackUpFrameSetSuitItem( item,dragfrom,dragdata1 );

	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end
function BackUpFrameSetSuitItem( item, nlisttype, nGridIndex )
	local backUpBtn = getglobal("BackUpFrameComfirmBtn");
	local btn 		= getglobal( "BackUpFrameItemBtn" );

	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local container	= MainPlayer:getContainer();

	local backItem = nil;
	if backUpBtn:GetClientUserData( 0 ) >= 0 and backUpBtn:GetClientUserData( 1 ) >= 0 then
		backItem = container:getItem( backUpBtn:GetClientUserData(0),backUpBtn:GetClientUserData(1) );
	end
	if backItem == nil or backItem:getItemId() == 0 then
		ShowMidTips("该进阶卷轴已被使用过");
		return;
	end
	local levelDef 	= backItem:getItemDef();
	local itemDef 	= item:getItemDef();
	local backDef	= GameArmProce:getBackLevelDef( itemDef.ItemID);
	if backDef == nil then 
		return;
	end
	local itemLv = backDef.Level;
	if  itemDef.WearPos1 ~= EQUIP_POS_FASHION_BACK then
		ShowMidTips( "非时装背饰，不能放入" )
		return
	end
	local upLv = 0;
	for idx=1,MAX_ITEM_RESULT do
		if levelDef.Result[idx-1].ResultID == RESULT_BACK_LEVELUP then
			upLv = levelDef.Result[idx-1].ResultVal1;
			break;
		end
	end
	if upLv > 0 and itemLv ~= upLv then 
		ShowMidTips("请放入"..upLv.."阶背饰");
		return;
	end
	--如果之前有物品的话，先取消掉
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then		
		local oldItem = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() )
		end
	end
	-- 原背饰相关
	btn:SetClientUserData(0,nlisttype);
	btn:SetClientUserData(1,nGridIndex);
	btn:SetClientUserData(2,itemDef.ItemID);	
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	LockCurItem( item, btn:GetParent() );
	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	btnTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex", item );
	SetPreItemBtnInfo();
end

function BackUpFrameComfirmBtn_OnClick()
	local itemBtn = getglobal("BackUpFrameItemBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("背包锁已锁无法进阶，请先验证背包安全锁");
		return;
	end
	if this:GetClientUserData(0) < 0 or this:GetClientUserData(1) < 0 then
		ShowMidTips("该卷轴已被使用过");
		return;
	end
	local levelItem = container:getItem( this:GetClientUserData(0),this:GetClientUserData(1) );
	if levelItem:getItemId() == 0 then
		UnLockItem( this:GetClientUserData(0),this:GetClientUserData(1),0);
		return;
	end
	if itemBtn:GetClientUserData(0) == -1 or itemBtn:GetClientUserData(1) == -1 then		
		ShowMidTips("请放入需要进阶的背饰");
		return;
	end
	local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
	if item:getItemId() == 0 then
		ShowMidTips("请放入需要进阶的背饰");
		return;
	end

	if not item:isItemBind() and levelItem:isItemBind() then
		MessageBox("提示","由于" .. levelItem:getDisplayName().. "是绑定的，进阶之后您的背饰也会变成绑定物品，确认要进阶吗？");
		MessageBoxFrame:SetClientString("背饰进阶");
		return;
	end
	BackItemLevelUp();
end

function BackItemLevelUp()
	local btn = getglobal("BackUpFrameComfirmBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local itemBtn 	= getglobal("BackUpFrameItemBtn");
	local levelItem = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
	if itemBtn:GetClientUserData(0) >= 0 and itemBtn:GetClientUserData(1) >= 0 then
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要进阶的背饰");
			return;
		end
		local srcListType	= btn:GetClientUserData( 0 );
		local srcGrid 		= btn:GetClientUserData( 1 );
		local descList		= item:getListType();
		local descGrid		= item:getGridIdx();
		--TODO HERE 判断绑定
		GameArmProce:requestBackLevelUp( descList,descGrid,item:getItemWId(),srcListType,srcGrid,levelItem:getItemWId() );		
	end
end

----------------------------------背饰改造---------------------------------------
function BackRemakeFrameBackBtn_OnMouseDown()
	if arg1 == "LeftButton" then
		nLastModelviewMouseClickX = GetCursorPosX();
	end
end
function BackRemakeFrameBackBtn_OnMouseMove()
	if this:GetPushedState() then
		local nCurMouseClickX = GetCursorPosX();
		ModelViewAngle = ModelViewAngle - (nCurMouseClickX - nLastModelviewMouseClickX) ;
		BackRemakeFrameBackBtnModelView:setEntityRotate(0, ModelViewAngle);
		nLastModelviewMouseClickX = nCurMouseClickX;
	end
end
function SetRemakePreItemBtnInfo()
	local btn 		= getglobal( "BackRemakeFrameItemBtn" );
	local remakeBtn = getglobal( "BackRemakeFrameComfirmBtn");
	local afterBtn 	= getglobal( "BackRemakeFramePreItemBtn");
	local afterTex 	= getglobal( afterBtn:GetName().."IconTexture" );
	local uvTex		= getglobal( afterBtn:GetName().."UVAnimationTex" );
	local itemID	= btn:GetClientUserData(2);
	if btn:GetClientUserData(0) < 0 then 
		afterBtn:SetClientUserData(2,0);
		afterTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		uvTex:Hide();
	else 
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end
		local container		= MainPlayer:getContainer();	
		local remakeItem 	= container:getItem( remakeBtn:GetClientUserData(0),remakeBtn:GetClientUserData(1) );
		local remakeDef 	= remakeItem:getItemDef();	
		local upLv 		= 0;
		local upType 	= 0;
		for idx=1,MAX_ITEM_RESULT do
			if remakeDef.Result[idx-1].ResultID == RESULT_BACK_CHG_TYPE then
				upLv 	= remakeDef.Result[idx-1].ResultVal2;
				upType 	= remakeDef.Result[idx-1].ResultVal1;
				break;
			end
		end
		local backID	= GameArmProce:getBackLevelID( upType, upLv );
		local afterDef 	= getItemDef( backID );
		local texPath 	= GetItemEquipIconPath()..afterDef.IconID..".tga";
		if not IsInExistence( texPath ) then
			texPath = DEFAULT_ITEM_PATH;
		end
		afterBtn:SetClientUserData(2,backID);
		afterTex:SetTexture( texPath );
		SetEquipImportantEffectUV( afterDef, afterBtn:GetName().."UVAnimationTex" );		
	end 
end

function BackRemakeFrameOnLoad()
	ItemFirstCallBack("BackRemakeFrame", BackRemakeFrameSetSuitItem );
end

function BackRemakeFrameOnShow()
	local itemBtn = getglobal("BackRemakeFrameItemBtn");
	itemBtn:SetClientUserData(0,-1);
	itemBtn:SetClientUserData(1,-1);
	itemBtn:SetClientUserData(2,0);
	if not PlayerContainerFrame:IsShown() then
		PlayerContainerFrame:Show();
	end
	if BackUpFrame:IsShown() then 
		BackUpFrame:Hide();
	end
	SetRemakePreItemBtnInfo();
	ModelViewAngle = 0;
	this:SetPointAlwaysRecalc("topright", "PlayerContainerFrame", "topleft",0 ,0 );
	UpdateBackFrameModelView( this:GetName(),0 );
end
local RemakeExtistTime = 0;
function BackRemakeFrameOnHide()
	local btn = getglobal("BackRemakeFrameComfirmBtn");
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then
		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
	end
	local ItemBtn = getglobal("BackRemakeFrameItemBtn");
	if ItemBtn:GetClientUserData(0) >= 0 and ItemBtn:GetClientUserData(1) >= 0 then
		UnLockItem( ItemBtn:GetClientUserData(0),ItemBtn:GetClientUserData(1),0 );
		ItemBtn:SetClientUserData(0,-1);
		ItemBtn:SetClientUserData(1,-1);
		ItemBtn:SetClientUserData(2,0);
		local tex = getglobal(ItemBtn:GetName().."IconTexture");
		tex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		local UVAnimationTex = getglobal( ItemBtn:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
	end
	local modelview = getglobal("BackRemakeFrameModelView");
	modelview:Hide()
	RemakeExtistTime = 0
	ModelViewAngle = 0;
end

function BackChangeTypeSuccess( nSucc )
	local btn = getglobal("BackRemakeFrameComfirmBtn");
	if nSucc == 0 then 
		ShowMidTips("背饰改造失败！！！");
		return;
	end
	if nSucc == 1 and btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 1 then
		local modelview = getglobal("BackRemakeFrameModelView");
		modelview:setPetModel(0,0,"effect\\entity\\fumo01.ent");
		modelview:setEntityPosition(0,0,100,0);
		UnLockItem( btn:GetClientUserData(0),btn:GetClientUserData(1),0 );
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		RemakeExtistTime = getServerTime().sec;
	end
end
function UpdateBackRemakeFrame()
	if RemakeExtistTime > 0 then 
		local curTime = getServerTime().sec ;
		if curTime - RemakeExtistTime > 1 then 
			BackRemakeFrame:Hide();
		end
	end
end

function BackChangeItemUse( used_Item )
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local itemDef = used_Item:getItemDef();
	if itemDef.WearPre.RoleLevelMin > 0 and MainPlayer:getLv() < itemDef.WearPre.RoleLevelMin then
		ShowMidTips("等级不够");
		return;
	end
	if BackRemakeFrame:IsShown() then
		BackRemakeFrame:Hide();
	end
	LockItem( used_Item:getListType(),used_Item:getGridIdx(),used_Item:getItemId() );
	local btn = getglobal( "BackRemakeFrameComfirmBtn" );
	btn:SetClientUserData( 0,used_Item:getListType() );
	btn:SetClientUserData( 1,used_Item:getGridIdx() );

	--设置名称 和描述
	local nameFont = getglobal("BackRemakeFrameNameFont");
	local descRich = getglobal("BackRemakeFrameDescRich");
	nameFont:SetText( itemDef.Name );	
	BackRemakeFrame:Show();	
	descRich:SetText( itemDef.ItemDesc,255,254,189 );
end

function BackRemakeItemOnRightClick( szBtnName ) 
	if szBtnName ~= "BackRemakeFrameItemBtn" then 
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
		btn:SetClientUserData(0,-1);
		btn:SetClientUserData(1,-1);
		btn:SetClientUserData(2,0);
		local UVAnimationTex = getglobal( this:GetName().."UVAnimationTex");
		UVAnimationTex:Hide();
		local btnTex = getglobal( this:GetName().."IconTexture" );
		btnTex:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	end
	SetRemakePreItemBtnInfo();
	UpdateBackFrameModelView( "BackRemakeFrame",btn:GetClientUserData(2) );
end
function BackRemakeItemOnLeftClick( itemBtnName )
	local btn 		= getglobal( itemBtnName );
	local frameName = btn:GetParent();
	UpdateBackFrameModelView( frameName,btn:GetClientUserData(2) );
end

function BackRemakeItemReceiveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if szBtnName ~= "BackRemakeFrameItemBtn" then 
		return;
	end
	local MainPlayer	= ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	if dragfrom ~= CONTAINER_TYPE_SUNDRIES and dragfrom ~= CONTAINER_TYPE_STASH then
		return;
	end
	local container	= MainPlayer:getContainer();	
	local item = container:getItem( dragfrom, dragdata1 );
	BackRemakeFrameSetSuitItem( item,dragfrom,dragdata1 );
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
end

function BackRemakeFrameSetSuitItem( item, nlisttype, nGridIndex )
	local remakeBtn = getglobal( "BackRemakeFrameComfirmBtn");
	local btn 		= getglobal( "BackRemakeFrameItemBtn" );

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local container	= MainPlayer:getContainer();
	local remakeItem = nil;
	if remakeBtn:GetClientUserData( 0 ) >= 0 and remakeBtn:GetClientUserData( 1 ) >= 0 then
		remakeItem = container:getItem( remakeBtn:GetClientUserData(0),remakeBtn:GetClientUserData(1) );
	end
	if remakeItem == nil or remakeItem:getItemId() == 0 then
		ShowMidTips("该进阶卷轴已被使用过");
		return;
	end
	local remakeDef = remakeItem:getItemDef();
	local itemDef 	= item:getItemDef();
	local backDef	= GameArmProce:getBackLevelDef( itemDef.ItemID );
	if backDef == nil then 
		return;
	end
	local itemLv = backDef.Level;
	if  itemDef.WearPos1 ~= EQUIP_POS_FASHION_BACK then
		ShowMidTips( "非时装背饰，不能放入" )
		return
	end
	local upLv = 0;
	for idx=1,MAX_ITEM_RESULT do
		if remakeDef.Result[idx-1].ResultID == RESULT_BACK_CHG_TYPE then
			upLv = remakeDef.Result[idx-1].ResultVal2;
			break;
		end
	end
	if upLv > 0 and itemLv ~= upLv then 
		ShowMidTips("请放入"..upLv.."阶背饰");
		return;
	end
	--如果之前有物品的话，先取消掉
	if btn:GetClientUserData(0) >= 0 and btn:GetClientUserData(1) >= 0 then		
		local oldItem = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId() )
		end
	end
	btn:SetClientUserData(0,nlisttype);
	btn:SetClientUserData(1,nGridIndex);
	btn:SetClientUserData(2,itemDef.ItemID);	
	local btnTex = getglobal( btn:GetName().."IconTexture" );
	LockCurItem( item, btn:GetParent() );
	local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	btnTex:SetTexture( IconPath );
	SetEquipImportantEffectUV( itemDef, btn:GetName().."UVAnimationTex", item );
	SetRemakePreItemBtnInfo();
end

function BackRemakeFrameComfirmBtn_OnClick()
	local itemBtn 	= getglobal("BackRemakeFrameItemBtn");
	local preBtn	= getglobal("BackRemakeFramePreItemBtn")
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local lockState = container:getContainerLockState();
	if lockState == CONTAINER_LOCK_STATE_LOCK then
		ShowMidTips("背包锁已锁无法进阶，请先验证背包安全锁");
		return;
	end
	if this:GetClientUserData(0) < 0 or this:GetClientUserData(1) < 0 then
		ShowMidTips("该卷轴已被使用过");
		return;
	end
	local levelItem = container:getItem( this:GetClientUserData(0),this:GetClientUserData(1) );
	if levelItem:getItemId() == 0 then
		UnLockItem( this:GetClientUserData(0),this:GetClientUserData(1),0);
		return;
	end
	if itemBtn:GetClientUserData(0) == -1 or itemBtn:GetClientUserData(1) == -1 then		
		ShowMidTips("请放入需要改造的背饰");
		return;
	end
	local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
	if item:getItemId() == 0 then
		ShowMidTips("请放入需要改造的背饰");
		return;
	end
	if itemBtn:GetClientUserData(2) == preBtn:GetClientUserData(2) then 
		ShowMidTips("放入的背饰和改造结果一致,不需改造!");
		return;
	end
	if not item:isItemBind() and levelItem:isItemBind() then
		MessageBox("提示","由于" .. levelItem:getDisplayName().. "是绑定的，改造之后您的背饰也会变成绑定物品，确认要改造吗？");
		MessageBoxFrame:SetClientString("背饰改造");
		return;
	end
	BackItemRemakeType();
end

function BackItemRemakeType()
	local btn = getglobal("BackRemakeFrameComfirmBtn");
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local itemBtn 	= getglobal("BackRemakeFrameItemBtn");
	local levelItem = container:getItem( btn:GetClientUserData(0),btn:GetClientUserData(1) );
	if itemBtn:GetClientUserData(0) >= 0 and itemBtn:GetClientUserData(1) >= 0 then
		local item = container:getItem( itemBtn:GetClientUserData(0),itemBtn:GetClientUserData(1) );
		if item:getItemId() == 0 then
			ShowMidTips("请放入需要改造的背饰");
			return;
		end
		local srcListType	= btn:GetClientUserData( 0 );
		local srcGrid 		= btn:GetClientUserData( 1 );
		local descList		= item:getListType();
		local descGrid		= item:getGridIdx();
		--TODO HERE 判断绑定
		GameArmProce:requestBackChange( descList,descGrid,item:getItemWId(),srcListType,srcGrid,levelItem:getItemWId() );		
	end
end

function AddFashionBackTipsInfo( itemDef,t_EquipInfo)
	local bShow = true;
	for i = 1, MAX_ITEM_ADD_ATTR_TOLUA do
		local AddAttrID		= itemDef.AddAttr[i-1].ID;
		local AddAttrValue	= itemDef.AddAttr[i-1].Idx; 
		if AddAttrID <= 0 then 
			break;
		end
		local newAddAttrDef = getNewAddAttrDef( AddAttrID );
		local addattrRandDef = getAddAttrRandDef( math.floor(itemDef.WearPre.RoleLevelMin/10), itemDef.Important )
		if newAddAttrDef ~= nil and addattrRandDef ~= nil then
			local base = newAddAttrDef.Val1[ addattrRandDef.ValLevel - 1 ];
			local mul = getNewAddAttrMul( newAddAttrDef, itemDef.WearPos1 );
			if mul > 0 then	
				if bShow then 
					local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
					table.insert( t_EquipInfo, t_oneEquipElem );
					-- 附加属性
					t_oneEquipElem ={ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
					table.insert( t_EquipInfo, t_oneEquipElem );
					bShow = false;
				end
				AddAttrValue = base*AddAttrValue/100*mul/100 ;
				if itemDef.ItemLevel > 0 then
					local tAddattrVal = { AddAttrValue, newAddAttrDef.Val2, newAddAttrDef.Val3, newAddAttrDef.Val4 }
					local t_oneEquipElem
					
					t_oneEquipElem = {	FIGHT_AREA, "\b", getNewAttrDesc( AddAttrID, tAddattrVal ),
									GetColor()[COLOR_ITEMKEYWORLD], LINE_TYPE_TEXT, false };
					table.insert( t_EquipInfo, t_oneEquipElem );
				end
			end
		end
	end
	--特殊属性
	if itemDef.TeshuID > 0 then
		if bShow then 
			local t_oneEquipElem = { FIGHT_AREA,	"", " ",	GetColor()[COLOR_ITEMSTART], LINE_TYPE_DISTANCE, true};
			table.insert( t_EquipInfo, t_oneEquipElem );
			-- 附加属性
			t_oneEquipElem ={ FIGHT_AREA, "",	"附加属性：",	GetColor()[COLOR_OGRANGE], LINE_TYPE_TEXT, false };
			table.insert( t_EquipInfo, t_oneEquipElem );
			bShow = false;
		end
		local armTeshuDef = getArmTeshuDef( itemDef.TeshuID )
		if armTeshuDef ~= nil then
			local rich 	= getglobal( "NormalItemTipsFrameITItemRich1" );
			local szText	= divide2lines( rich, armTeshuDef.DefDesc );
			t_oneEquipElem = {	FIGHT_AREA, "", szText,
						GetColor()[COLOR_PURPLE], LINE_TYPE_RICH, false };
			table.insert( t_EquipInfo, t_oneEquipElem );
		end 
	end
end