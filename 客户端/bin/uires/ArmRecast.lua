--- 装备 重铸 ---

local nLastRequestRecast = 0;

local MAX_BASE_ATTR_NUM = 2;

local function getItemResultValue( itemDef,resultID )
	for i=1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID == resultID then
			return itemDef.Result[i-1].ResultVal1,itemDef.Result[i-1].ResultVal2,itemDef.Result[i-1].ResultVal3,itemDef.Result[i-1].ResultVal4
		end
	end
	return 0,0,0,0
end

function ArmRecastFrame_OnShow()
	ClearArmRecastData();
	ArmRecastFrameArmInfo:Hide();
	ArmRecastFrameSaveSuccessBtn:Hide();
	UpdateArmRecastFrame();
end

function Save2ArmRecastFrame( item, nSrcList, nSrcGridIndex )
	if CheckItemResult( item:getItemDef(), RESULT_ARM_RECAST ) or CheckItemResult( item:getItemDef(), RESULT_ARM_RECAST_MUST ) then
		ArmRecastFrameItemButton_OnReciveDrag( "ArmRecastFrameNeedItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	else
		ArmRecastFrameItemButton_OnReciveDrag( "ArmRecastFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
	end
end

function ArmRecastFrame_OnLoad()
	ArmRecastFrameDescRich:resizeRect( 250,100 );
	local szText = [[☆  装备重铸可重置装备基础属性
☆  装备重铸需消耗装备等级对应的重铸石
]]
	ArmRecastFrameDescRich:SetText( szText,170,160,120);
	this:RegisterEvent( "GE_CONTAINER_UPDATE" );
	this:RegisterEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	this:RegisterEvent( "GE_ARM_RECAST" );

	ItemFirstCallBack( "ArmRecastFrame" , Save2ArmRecastFrame );
end

function ArmRecastFrame_OnEvent()
	if arg1 == "GE_CONTAINER_UPDATE" or arg1 == "UI_ACTOR_ATTRIBUTE_CHG" then
		if ArmRecastFrame:IsShown() then
			UpdateArmRecastFrame();
		end
		if ArmAdvRecastFrame:IsShown() then
			UpdateAdvArmRecastFrame();
		end
		if ArmRecastSelectAttrFrame:IsShown() then
			UpdateArmRecasetSelectAttrArmInfo();
		end
	elseif arg1 == "GE_ARM_RECAST" then		
		ArmAdvRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameSaveSuccessBtn:Hide();
		ArmRecastFrameSaveSuccessBtn:Hide();
		if ArmRecastFrame:IsShown() then
			UpdateSelectRecastResult();
		end
		if ArmAdvRecastFrame:IsShown() then
			UpdateAdvSelectRecastResult();
		end
	end
end

function getRecastNewAttrDesc( recastInfo,item,itemDef )
	-- 最大最小物攻，法攻特殊处理
	local szText = "";
	local newMaxPhsicAtk,newMinPhsicAtk,newMaxMagicAtk,newMinMagicAtk, newMaxAtk, newMinAtk = 0,0,0,0,0,0;
	local maxMaxPhsicAtk,maxMinPhsicAtk,maxMaxMagicAtk,maxMinMagicAtk, maxMaxAtk, maxMinAtk = 0,0,0,0,0,0;
	for i=1,recastInfo.BaseAttrNum do
		local resultID = recastInfo.BaseInfo[i-1].ResultID;
		local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = resultID } )			
		for j=1,MAX_ITEM_RESULT do
			if itemDef.Result[j-1].ResultID == resultID then
				local maxColor =  GetColor()[ COLOR_PURPLE ];
				if itemResultConfig ~= nil then
					local maxVal = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2;
					local newVal = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[i-1].Mul/100);
					local szVal = itemDef.Result[j-1].ResultID == RESULT_BASE_ATTR_HEAVY_HIT_VAL and string.format("%.2f%%",newVal/100) or newVal
					local szMaxVal = itemDef.Result[j-1].ResultID == RESULT_BASE_ATTR_HEAVY_HIT_VAL and string.format("%.2f%%",maxVal/100) or maxVal
					local valColor = GetColor()[ newVal == maxVal and COLOR_PURPLE or COLOR_WHITE ]
					szText = szText .. string.format( "#cffffff\b%s：#n#c%02x%02x%02x%s#n #c%02x%02x%02x(%s)#n\n", itemResultConfig[2],
						valColor[1], valColor[2], valColor[3], szVal, maxColor[1], maxColor[2], maxColor[3], szMaxVal );
				else						
					if resultID == RESULT_WEAPON_MIN_PHSIC_ATK then						
						maxMinPhsicAtk = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2
						newMinPhsicAtk = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[j-1].Mul/100)
						newMaxPhsicAtk = newMaxPhsicAtk > newMinPhsicAtk and newMaxPhsicAtk or newMinPhsicAtk
					elseif resultID == RESULT_WEAPON_MAX_PHSIC_ATK then
						maxMaxPhsicAtk = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2
						newMaxPhsicAtk = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[j-1].Mul/100)
						newMinPhsicAtk = newMinPhsicAtk < newMaxPhsicAtk and newMinPhsicAtk or newMaxPhsicAtk
					elseif resultID == RESULT_WEAPON_MIN_MAGIC_ATK then
						maxMinMagicAtk = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2
						newMinMagicAtk = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[j-1].Mul/100)
						newMaxMagicAtk = newMaxMagicAtk > newMinMagicAtk and newMaxMagicAtk or newMinMagicAtk
					elseif resultID == RESULT_WEAPON_MAX_MAGIC_ATK then
						maxMaxMagicAtk = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2
						newMaxMagicAtk = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[j-1].Mul/100)
						newMinMagicAtk = newMinMagicAtk < newMaxMagicAtk and newMinMagicAtk or newMaxMagicAtk
					elseif resultID == RESULT_WEAPON_MIN_ATK then
						maxMinAtk = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2
						newMinAtk = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[j-1].Mul/100)
						newMaxAtk = newMaxAtk > newMinAtk and newMaxAtk or newMinAtk
					elseif resultID == RESULT_WEAPON_MAX_ATK then
						maxMaxAtk = itemDef.Result[j-1].ResultVal1 + itemDef.Result[j-1].ResultVal2
						newMaxAtk = itemDef.Result[j-1].ResultVal1 + math.floor(itemDef.Result[j-1].ResultVal2*recastInfo.BaseInfo[j-1].Mul/100)
						newMinAtk = newMinAtk < newMaxAtk and newMinAtk or newMaxAtk
					end
					if newMaxPhsicAtk > 0 then
						local minValColor = GetColor()[ newMinPhsicAtk == maxMinPhsicAtk and COLOR_PURPLE or COLOR_WHITE ]
						local maxValColor = GetColor()[ newMaxPhsicAtk == maxMaxPhsicAtk and COLOR_PURPLE or COLOR_WHITE ]
						local signColor = GetColor()[ ( newMinPhsicAtk == maxMinPhsicAtk and newMaxPhsicAtk == maxMaxPhsicAtk ) and COLOR_PURPLE or COLOR_WHITE ]
						szText = string.format( "#cffffff\b物理攻击：#n#c%02x%02x%02x%d#n#c%02x%02x%02x-#n#c%02x%02x%02x%d#n#c%02x%02x%02x(%d-%d)#n\n",
						minValColor[1], minValColor[2], minValColor[3], newMinPhsicAtk, signColor[1], signColor[2], signColor[3],
						maxValColor[1], maxValColor[2], maxValColor[3], newMaxPhsicAtk, maxColor[1], maxColor[2], maxColor[3],
						maxMinPhsicAtk,maxMaxPhsicAtk )
					end
					if newMaxMagicAtk > 0 then
						local minValColor = GetColor()[ newMinMagicAtk == maxMinMagicAtk and COLOR_PURPLE or COLOR_WHITE ]
						local maxValColor = GetColor()[ newMaxMagicAtk == maxMaxMagicAtk and COLOR_PURPLE or COLOR_WHITE ]
						local signColor = GetColor()[ ( newMinMagicAtk == maxMinMagicAtk and newMaxMagicAtk == maxMaxMagicAtk ) and COLOR_PURPLE or COLOR_WHITE ]
						szText = string.format( "#cffffff\b魔法攻击：#n#c%02x%02x%02x%d#n#c%02x%02x%02x-#n#c%02x%02x%02x%d#n#c%02x%02x%02x(%d-%d)#n\n",
						minValColor[1], minValColor[2], minValColor[3], newMinMagicAtk, signColor[1], signColor[2], signColor[3],
						maxValColor[1], maxValColor[2], maxValColor[3], newMaxMagicAtk, maxColor[1], maxColor[2], maxColor[3],
						maxMinMagicAtk,maxMaxMagicAtk )
					end
					if newMaxAtk > 0 then
						local minValColor = GetColor()[ newMinAtk == maxMinAtk and COLOR_PURPLE or COLOR_WHITE ]
						local maxValColor = GetColor()[ newMaxAtk == maxMaxAtk and COLOR_PURPLE or COLOR_WHITE ]
						local signColor = GetColor()[ ( newMinAtk == maxMinAtk and newMaxAtk == maxMaxAtk ) and COLOR_PURPLE or COLOR_WHITE ]
						szText = string.format( "#cffffff\b双系攻击：#n#c%02x%02x%02x%d#n#c%02x%02x%02x-#n#c%02x%02x%02x%d#n#c%02x%02x%02x(%d-%d)#n\n",
						minValColor[1], minValColor[2], minValColor[3], newMinAtk, signColor[1], signColor[2], signColor[3],
						maxValColor[1], maxValColor[2], maxValColor[3], newMaxAtk, maxColor[1], maxColor[2], maxColor[3],
						maxMinAtk,maxMaxAtk )
					end
				end
			end
		end
	end
	return szText;
end

function UpdateSelectRecastResult()
	local uiMsg = UIMSG:getUIMsg();
	local nList = ArmRecastFrameArmItemBtn:GetClientUserData( 0 )
	local nGrid = ArmRecastFrameArmItemBtn:GetClientUserData( 1 )
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item = container:getItem( nList,nGrid )
	local itemDef = item:getItemDef();

	if item:getItemId() > 0 and isSameWid( item:getItemWId(),uiMsg.RecastInfo.WID ) then
		local recastInfo = uiMsg.RecastInfo.RecastItemsDetail[0]
		ArmRecastFrameArmInfo:Show();
		local szText = getRecastNewAttrDesc( recastInfo,item,itemDef )

		ArmRecastSelectAttrFrame:Hide();
		ArmRecastFrameArmInfoRich:SetText( "#cff9600本次重铸属性#n\n" .. szText, 255, 255, 255 )
		
		-- 处理属性增减标记
			local nMinPhsicAtkMul,nMaxPhsicAtkMul,nMinMagicAtkMul,nMaxMagicAtkMul,nMinAtkMul,nMaxAtkMul;
			local nIndex = 1;
			for i=1,recastInfo.BaseAttrNum do
				if nIndex > MAX_BASE_ATTR_NUM then
					break
				end
				local resultID = recastInfo.BaseInfo[i-1].ResultID;
				local val1,val2 = getItemResultValue( itemDef,resultID )
				local nChg = math.floor( recastInfo.BaseInfo[i-1].Mul * val2 / 100 ) - math.floor( item:getItemMulVal( resultID ) * val2 / 100 );
				if resultID == RESULT_WEAPON_MIN_PHSIC_ATK then
					nMinPhsicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MAX_PHSIC_ATK then
					nMaxPhsicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MIN_MAGIC_ATK then
					nMinMagicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MAX_MAGIC_ATK then
					nMaxMagicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MIN_ATK then
					nMinAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MAX_ATK then
					nMaxAtkMul = nChg
				else
					local tex = getglobal( "ArmRecastFrameArmInfoChangeSignTex" .. nIndex );
					if nChg == 0 then
						tex:SetTexUV( 728,741,13,4 )
						tex:SetSize( 13,4 );
					elseif nChg > 0 then
						tex:SetTexUV( 651,622,11,12 )
						tex:SetSize( 11,12 );
					else
						tex:SetTexUV( 639,622,11,12 )
						tex:SetSize( 11,12 );
					end					
					tex:Show();
					local nWidth = (12 + ArmRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
					tex:SetPoint( "topleft","ArmRecastFrameArmInfo","topleft", nWidth, 22 + (nIndex - 1) * 14 )
					nIndex = nIndex + 1;
				end
			end
			if nMinPhsicAtkMul ~= nil and nMaxPhsicAtkMul ~= nil and nIndex <= MAX_BASE_ATTR_NUM then
				local nChg = (nMinPhsicAtkMul + nMaxPhsicAtkMul)/2
				local tex = getglobal( "ArmRecastFrameArmInfoChangeSignTex" .. nIndex );
				if nChg == 0 then
					tex:SetTexUV( 728,741,13,4 )
					tex:SetSize( 13,4 );
				elseif nChg > 0 then
					tex:SetTexUV( 651,622,11,12 )
					tex:SetSize( 11,12 );
				else
					tex:SetTexUV( 639,622,11,12 )
					tex:SetSize( 11,12 );
				end
				tex:Show();
				local nWidth = (12 + ArmRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
				tex:SetPoint( "topleft","ArmRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
				nIndex = nIndex + 1;
			end
			if nMinMagicAtkMul ~= nil and nMaxMagicAtkMul ~= nil and nIndex <= MAX_BASE_ATTR_NUM then
				local nChg = (nMinMagicAtkMul + nMaxMagicAtkMul)/2
				local tex = getglobal( "ArmRecastFrameArmInfoChangeSignTex" .. nIndex );
				if nChg == 0 then
					tex:SetTexUV( 728,741,13,4 )
					tex:SetSize( 13,4 );
				elseif nChg > 0 then
					tex:SetTexUV( 651,622,11,12 )
					tex:SetSize( 11,12 );
				else
					tex:SetTexUV( 639,622,11,12 )
					tex:SetSize( 11,12 );
				end
				tex:Show();
				local nWidth = (12 + ArmRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
				tex:SetPoint( "topleft","ArmRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
				nIndex = nIndex + 1;
			end
			if nMinAtkMul ~= nil and nMaxAtkMul ~= nil and nIndex <= MAX_BASE_ATTR_NUM then
				local nChg = (nMinAtkMul + nMaxAtkMul)/2
				local tex = getglobal( "ArmRecastFrameArmInfoChangeSignTex" .. nIndex );
				if nChg == 0 then
					tex:SetTexUV( 728,741,13,4 )
					tex:SetSize( 13,4 );
				elseif nChg > 0 then
					tex:SetTexUV( 651,622,11,12 )
					tex:SetSize( 11,12 );
				else
					tex:SetTexUV( 639,622,11,12 )
					tex:SetSize( 11,12 );
				end
				tex:Show();
				local nWidth = (12 + ArmRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
				tex:SetPoint( "topleft","ArmRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
				nIndex = nIndex + 1;
			end
			for i=nIndex,MAX_BASE_ATTR_NUM do
				local tex = getglobal( "ArmRecastFrameArmInfoChangeSignTex" .. i );
				tex:Hide();
			end
	end	
end

function ArmRecastFrame_OnESC()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( "重铸面板关闭" );
		return true;
	end
	return false
end

function ArmRecastFrame_OnHide()
	local nListType	= ArmRecastFrameArmItemBtn:GetClientUserData( 0 ) ;
	local nGrid		= ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
	if nListType > 0 then
		UnLockItem( nListType,nGrid,0 );
	end
	ArmRecastFrameNeedItemBtn:SetClientUserData( 0,0 )
	ArmRecastFrameNeedItemBtn:SetClientUserData( 1,0 )
	ArmRecastFrameNeedItemBtn:SetClientUserData( 2,0 )
	ArmRecastFrameNeedItemBtn:SetClientUserData( 3,0 )
	ClearItemLock( "ArmRecastFrameNeedItemBtn" )
	ArmRecastFrameArmItemBtn:SetClientUserData( 0,0 )
	ArmRecastFrameArmItemBtn:SetClientUserData( 1,0 )
	ArmRecastFrameArmItemBtn:SetClientUserData( 2,0 )
	if dragfrom == DRAG_ARM_RESOLVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	if MessageBoxFrame:IsShown() and (MessageBoxFrame:GetClientString() == "绑定重铸" 
		or MessageTitle:GetText() == "重铸切换强化" or MessageTitle:GetText() == "重铸切换洗炼"
		or MessageTitle:GetText() == "重铸切换绑定" or MessageTitle:GetText() == "重铸切换转星"
		or MessageBoxFrame:GetClientString() == "重铸面板关闭"
		or MessageBoxFrame:GetClientString() == "切换高级重铸") then
		MessageBoxFrame:Hide()
	end	
	ArmRecastFrameArmInfo:Hide();
end

function ArmRecastFrameArmInfoSaveBtn_OnClick()
	local armListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
	GameArmProce:saveRecastArm( 0,armListType,armGridIndex )
	ArmRecastFrameArmInfo:Hide();
	ArmRecastFrameSaveSuccessBtn:Show();
end

function ArmRecastFrameArmInfoCancelBtn_OnClick()
	ArmRecastFrameArmInfo:Hide();
end

function ArmRecastFrameItemButton_OnRecive()
	ArmRecastFrameItemButton_OnReciveDrag( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
end

function ArmRecastFrameItemButton_OnReciveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if dragfrom == DRAG_ARM_RESOLVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateArmRecastFrame();
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
	if szBtnName == "ArmRecastFrameNeedItemBtn" then
		if not CheckItemResult( itemdef, RESULT_ARM_RECAST ) and not CheckItemResult( itemdef, RESULT_ARM_RECAST_MUST ) then
			ShowMidTips( "请放入装备或者重铸石" )
			return
		end

		local nArmListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
		local nArmGridIdx  = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
		local armItem = container:getItem( nArmListType,nArmGridIdx )
		if armItem:getItemId() > 0 then
			local nMaxRecastLv = 0;
			for i=1,MAX_ITEM_RESULT do
				if itemdef.Result[i-1].ResultID == RESULT_ARM_RECAST or itemdef.Result[i-1].ResultID == RESULT_ARM_RECAST_MUST then
					nMaxRecastLv = itemdef.Result[i-1].ResultVal1;
					break;
				end
			end
			-- 对等级判断
			if armItem:getItemDef().WearPre.RoleLevelMin > nMaxRecastLv then
				ShowMidTips( "装备等级与重铸石不符" )
				return;
			end
		end

		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		local srcitem = container:getItem( nListType,nGridIdx );
		local nItemId	= srcitem:getItemId();

		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		btn:SetClientUserData( 2,item:getItemId() )
		if item:isItemBind() then
			btn:SetClientUserData( 3,1 )
		else
			btn:SetClientUserData( 3,0 )
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		UpdateArmRecastFrame();
		ArmRecastFrameSaveSuccessBtn:Hide();
	else
		if dragfrom == CONTAINER_TYPE_SUNDRIES then			
			if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
				ShowMidTips( "请放入装备或者重铸石" );
				return;
			end		
			
			--[[
			if itemdef.Important == 0 then
				ShowMidTips( "白色品质不能重铸" );
				return;
			end
			]]--
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
				ShowMidTips( "时装不能重铸" )
				return
			end
			if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
				ShowMidTips( "徽章装备不能重铸" );
				return;
			end
			if item:isArmFengYin() then
				ShowMidTips("封印装备无法进行此操作");
				return;
			end
			
			local stuffItemId = ArmRecastFrameNeedItemBtn:GetClientUserData( 2 )
			if container:getItemCount( stuffItemId ) > 0 then
				local stuffItemDef = getItemDef( stuffItemId )
				local nMaxRecastLv = 0;
				for i=1,MAX_ITEM_RESULT do
					if stuffItemDef.Result[i-1].ResultID == RESULT_ARM_RECAST or stuffItemDef.Result[i-1].ResultID == RESULT_ARM_RECAST_MUST then
						nMaxRecastLv = stuffItemDef.Result[i-1].ResultVal1;
						break;
					end
				end
				-- 对等级判断
				if itemdef.WearPre.RoleLevelMin > nMaxRecastLv then
					ShowMidTips( "装备等级与重铸石不符" )
					return;
				end
			end			
			
			local nListType = btn:GetClientUserData( 0 );
			local nGridIdx	= btn:GetClientUserData( 1 );
			local srcitem = container:getItem( nListType,nGridIdx );			
			local nItemId	= srcitem:getItemId();
			if nItemId > 0 then
				UnLockItem( nListType, nGridIdx, nItemId );--解锁
			end
			LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
			btn:SetClientUserData( 0,srcList )
			btn:SetClientUserData( 1,srcGrid )
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			ArmRecastFrameArmInfo:Hide();
			ArmRecastFrameAutoAddNeedItem( itemdef );
			UpdateArmRecastFrame();
			ArmRecastFrameSaveSuccessBtn:Hide();
		end
	end
end

function ArmRecastFrameAutoAddNeedItem( ArmDef )
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local itemBtn	= getglobal("ArmRecastFrameNeedItemBtn");
	local armBtn	= getglobal("ArmRecastFrameArmItemBtn");
	local RecastItemGrid	= 0;
	local RecastItemBind	= 0;
	local RecastitemID		= 0;
	if itemBtn:GetClientUserData( 2 ) == 0 then
		local minNeedLv = 200; 
		local nRecastLv = 0;
		local nRecast	= false;
		for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
			local item		= container:getItem( CONTAINER_TYPE_SUNDRIES, i );
			local itemId	= item:getItemId();
			if itemId > 0 then
				local ItemDef = getItemDef( itemId );
				for i=1, MAX_ITEM_RESULT do
					if ItemDef.Result[i-1].ResultID == RESULT_ARM_RECAST then
						nRecastLv	= ItemDef.Result[i-1].ResultVal1;
						nRecast		= true;
						break;
					end
				end
				
				if nRecast and ArmDef.WearPre.RoleLevelMin < nRecastLv and nRecastLv <= minNeedLv and item:isItemBind() then
					minNeedLv		= nRecastLv;
					RecastItemGrid	= i;
					RecastitemID	= itemId;
					RecastItemBind	= 1;
				end
				if nRecast and ArmDef.WearPre.RoleLevelMin < nRecastLv and nRecastLv < minNeedLv then
					minNeedLv		= nRecastLv;
					RecastItemGrid	= i;
					RecastitemID	= itemId;
					RecastItemBind	= 0;
				end
			end
			nRecast	= false;
		end
		if minNeedLv == 200 then
			RecastItemGrid	= 0;
			RecastitemID	= 0;
		end
	end
	if RecastitemID > 0 then
		ArmRecastFrameNeedItemBtn:SetClientUserData( 0 ,CONTAINER_TYPE_SUNDRIES );
		ArmRecastFrameNeedItemBtn:SetClientUserData( 1 ,RecastItemGrid );
		ArmRecastFrameNeedItemBtn:SetClientUserData( 2 ,RecastitemID );
		ArmRecastFrameNeedItemBtn:SetClientUserData( 3 ,RecastItemBind );
	end
end

function ClearArmRecastData()
	-- 清理界面
	ArmRecastFrameNeedItemBtnBindTexture:Hide();
	ArmRecastFrameNeedItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmRecastFrameNeedItemBtnUVAnimationTex:Hide();
	ArmRecastFrameNeedItemBtnCountFont:SetText("");
	--ArmRecastFrameAddStuffTipFont:Show();
	
	ArmRecastFrameArmItemBtnBindTexture:Hide();
	ArmRecastFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmRecastFrameArmItemBtnUVAnimationTex:Hide();
	--ArmRecastFrameAddArmTipFont:Show();

	ArmRecastFrameNeedMoneyFont:SetText("")
	ArmRecastFrameArmOldInfoRich:SetText( "", 255, 255, 255 )
end

function UpdateArmRecastFrame()
	ArmRecastFrameOKBtn:Disable();
	ClearArmRecastData();
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local bEnable = true;
	local nStuffList = ArmRecastFrameNeedItemBtn:GetClientUserData( 0 );
	local nStuffGrid = ArmRecastFrameNeedItemBtn:GetClientUserData( 1 );
	local nStuffItemId = ArmRecastFrameNeedItemBtn:GetClientUserData( 2 );
	local stuffBind = ArmRecastFrameNeedItemBtn:GetClientUserData( 3 );
	ClearItemLock( "ArmRecastFrameNeedItemBtn" )
	local nNum = 0
	if stuffBind == 1 then
		nNum = container:getBindItemNum( CONTAINER_TYPE_SUNDRIES, nStuffItemId )		
	else
		nNum = container:getUnBindItemNum( CONTAINER_TYPE_SUNDRIES, nStuffItemId )
	end
	for i = 0, container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES ) do
		local t = container:getItem( CONTAINER_TYPE_SUNDRIES, i )
		local nBind = t:isItemBind() == true and 1 or 0
		if nStuffItemId ~= 0 and t:getItemId() == nStuffItemId and nBind == stuffBind then
			LockItem( t:getListType(), t:getGridIdx(), t:getItemId(), "ArmRecastFrameNeedItemBtn" )
		end
	end
	if nNum > 0 then
		local stuffItemDef = getItemDef( nStuffItemId )
		if nNum > 1 then
			ArmRecastFrameNeedItemBtnCountFont:SetText( nNum )
		else
			ArmRecastFrameNeedItemBtnCountFont:SetText( "" )
		end
		IconPath = GetItemEquipIconPath()..stuffItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmRecastFrameNeedItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( stuffItemDef, "ArmRecastFrameNeedItemBtnUVAnimationTex", stuffItem);
		
		if stuffBind == 1 then
			ArmRecastFrameNeedItemBtnBindTexture:Show()
		else
			ArmRecastFrameNeedItemBtnBindTexture:Hide();
		end
	else
		ArmRecastFrameNeedItemBtn:SetClientUserData( 2,0 )
		--bEnable = false;
	end
	
	local stuffItem = container:getItem( nStuffList,nStuffGrid );
	if stuffItem:getItemId() == 0 and nStuffList ~= 0 then
		ArmRecastFrameNeedItemBtn:SetClientUserData( 0,0 )
		ArmRecastFrameNeedItemBtn:SetClientUserData( 1,0 )
	end

	local nListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );

	local item = container:getItem( nListType,nGrid );
	ArmRecastFrameArmItemBtnBindTexture:Hide();	
	if item:getItemId() > 0 then
		-- 设置图标，
		local equipItemDef = item:getItemDef();
		local IconPath = GetItemEquipIconPath()..equipItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		ArmRecastFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( equipItemDef, "ArmRecastFrameArmItemBtnUVAnimationTex",item);
		if item:isItemBind() then
			ArmRecastFrameArmItemBtnBindTexture:Show();
		else
			ArmRecastFrameArmItemBtnBindTexture:Hide();
		end
		
		local nNeedMoney = (math.ceil(item:getItemDef().WearPre.RoleLevelMin/10))*5;
		ArmRecastFrameNeedMoneyFont:SetText( nNeedMoney );

		local tBaseAttr = GetItemNewBaseAttrDesc( equipItemDef,item );
		--local mulMin,mulMax = getArmDefMinAndMaxMul( item );
		local szText = "#cff9600原装备属性#n\n";
		for i=1,table.getn(tBaseAttr) do
			szText = szText .. "\b" .. tBaseAttr[i][1] .. "：" .. tBaseAttr[i][2] .. "\n";
			--szText = szText .. "\b" .. tBaseAttr[i][1] .. "：" .. tBaseAttr[i][2] .. " " .. mulMin .. " " .. mulMax .. "\n";
		end
		ArmRecastFrameArmOldInfoRich:SetText( szText, 255, 255, 255 )
	else
		bEnable = false;
		if ArmRecastFrameArmInfo:IsShown() then
			ArmRecastFrameArmInfo:Hide();
		end
	end
	if bEnable then
		ArmRecastFrameOKBtn:Enable();
	end
end

function IsArmFullAttr( itemdef, item )
	local t_AttrListFullInfo = {}
	local maxPhsicAtk,minPhsicAtk,maxMagicAtk,minMagicAtk,maxAtk,minAtk = 0,0,0,0,0,0;
	local maxMaxPhsicAtk,maxMinPhsicAtk,maxMaxMagicAtk,maxMinMagicAtk,maxMaxAtk,maxMinAtk = 0,0,0,0,0,0;
	for i=1,MAX_ITEM_RESULT do
		if IsEquipNewBaseAttr( itemdef.Result[i-1].ResultID ) then
			local maxColor =  GetColor()[ COLOR_PURPLE ];
			local itemResultConfig = GetOneItemResultConfig( { ["resultID"] = itemdef.Result[i-1].ResultID } )
			local maxVal = itemdef.Result[i-1].ResultVal1 + itemdef.Result[i-1].ResultVal2;
			if itemResultConfig ~= nil then
				local val = GetNewBaseAttrValue( itemdef.Result[i-1],item )				
				if itemResultConfig[1] == RESULT_BASE_ATTR_HEAVY_HIT_VAL then
					local bFullAttr = val/100 >= maxVal/100
					table.insert( t_AttrListFullInfo, bFullAttr )
				else
					local bFullAttr = val >= maxVal
					table.insert( t_AttrListFullInfo, bFullAttr )
				end				
			else
				if itemdef.Result[i-1].ResultID == RESULT_WEAPON_MIN_PHSIC_ATK then
					minPhsicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					maxPhsicAtk = maxPhsicAtk > minPhsicAtk and maxPhsicAtk or minPhsicAtk
					maxMinPhsicAtk = maxVal					
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MAX_PHSIC_ATK then
					maxPhsicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					minPhsicAtk = minPhsicAtk < maxPhsicAtk and minPhsicAtk or maxPhsicAtk
					maxMaxPhsicAtk = maxVal			
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MIN_MAGIC_ATK then
					minMagicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )							
					maxMagicAtk = maxMagicAtk > minMagicAtk and maxMagicAtk or minMagicAtk
					maxMinMagicAtk = maxVal
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MAX_MAGIC_ATK then
					maxMagicAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )
					minMagicAtk = minMagicAtk < maxMagicAtk and minMagicAtk or maxMagicAtk
					maxMaxMagicAtk = maxVal
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MIN_ATK then
					minAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )
					maxAtk = maxAtk > minAtk and maxAtk or minAtk
					maxMinAtk = maxVal
				elseif itemdef.Result[i-1].ResultID == RESULT_WEAPON_MAX_ATK then
					maxAtk = GetNewBaseAttrValue( itemdef.Result[i-1], item )
					minAtk = minAtk < maxAtk and minAtk or maxAtk
					maxMaxAtk = maxVal
				end
				if maxPhsicAtk > 0 then
					local bFullAttr = minPhsicAtk >= maxMinPhsicAtk and maxPhsicAtk >= maxMaxPhsicAtk
					table.insert( t_AttrListFullInfo, bFullAttr )
				end
				if maxMagicAtk > 0 then
					local bFullAttr = minMagicAtk >= maxMinMagicAtk and maxMagicAtk >= maxMaxMagicAtk
					table.insert( t_AttrListFullInfo, bFullAttr )
				end
				if maxAtk > 0 then
					local bFullAttr = minAtk > maxMinAtk and maxAtk > maxMaxAtk
					table.insert( t_AttrListFullInfo, bFullAttr )
				end
			end
		end
	end
	for i = 1, table.getn( t_AttrListFullInfo ) do
		if not t_AttrListFullInfo[i] then
			return false
		end
	end
	return true
end

function ArmRecastFrameOKBtn_OnClick()
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	if TradeFrame:IsShown() then
		ShowMidTips("交易中不可重铸");
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end;
	local container = mainplayer:getContainer();
	local stuffItem = ArmRecastFrameNeedItemBtn:GetClientUserData( 2 )
	local armListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );

	if stuffItem == 0 then
		local item = container:getItem( armListType, armGridIndex );
		MessageBoxForItem("提示","重铸石");
		MessageBoxForItemFrame:SetClientUserData(0,0);
		MessageBoxForItemFrame:SetClientUserData(1,item:getItemId());
		MessageBoxForItemFrame:SetClientString("重铸石不足");
		return;
	end
	
	-- 费用
	local nMoney = getPlayerMoney( mainplayer )
	if nMoney < tonumber( ArmRecastFrameNeedMoneyFont:GetText() ) then
		ShowMidTips("绑银不足，无法重铸")
		return
	end
	if checkNeedWarnCost( tonumber( ArmRecastFrameNeedMoneyFont:GetText() ) ) then
		CostMoneyConfirmFrame:SetClientString("装备重铸");
		SetWarnFrame( "ArmRecastFrame" );
		CostMoneyConfirmFrame:Show();
		return;
	end

	local stuffItem = ArmRecastFrameNeedItemBtn:GetClientUserData( 2 )
	local armListType = ArmRecastFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmRecastFrameArmItemBtn:GetClientUserData( 1 );
	local nBind = ArmRecastFrameNeedItemBtn:GetClientUserData( 3 )
	if os.clock() - nLastRequestRecast > 0.5 then
		local item = container:getItem( armListType, armGridIndex )
		if not item:isItemBind() and nBind == 1 then
			MessageBox("绑定提示","继续重铸则当前装备会被绑定，是否继续重铸？");
			MessageBoxFrame:SetClientString("绑定重铸");
			MessageBoxFrame:SetClientUserData( 0, stuffItem )
			MessageBoxFrame:SetClientUserData( 1, armListType )
			MessageBoxFrame:SetClientUserData( 2, armGridIndex )
			MessageBoxFrame:SetClientUserData( 3, nBind )
		else
			local must = 0 
			if CheckItemResult( getItemDef(stuffItem), RESULT_ARM_RECAST_MUST ) then
				must = 1
			end
			if IsArmFullAttr( item:getItemDef(), item ) then
				ShowMidTips( " 该装备的基础属性已满，不需要再进行重铸" )
				return
			end
			GameArmProce:normalRecastArm( stuffItem, armListType, armGridIndex, nBind, must );
		end		
		nLastRequestRecast = os.clock()
	end
end

function ArmRecastFrameItemButton_OnEnter()
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local nListType = this:GetClientUserData( 0 );
	local nGridIndex = this:GetClientUserData( 1 );
	local item = container:getItem( nListType,nGridIndex )
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
	else
		local itemId = this:GetClientUserData( 2 )
		local nBind = this:GetClientUserData( 3 )
		local stuffItem = nil
		for i=1,container:getContainerMaxNum(CONTAINER_TYPE_SUNDRIES) do
			item = container:getItem( CONTAINER_TYPE_SUNDRIES, (i-1) );
			if item:getItemId() == itemId then
				if nBind == 1 and item:isItemBind() then
					stuffItem = item;
					break;
				elseif nBind == 0 and not item:isItemBind() then
					stuffItem = item;
					break;
				end
			end
		end
		if stuffItem ~= nil then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, stuffItem );
		elseif itemId > 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( itemId ) )
		end
	end
	if item:getItemId() <= 0 then
		if this:GetParent() == "ArmRecastFrame" then
			if string.find( this:GetName(), "ArmItemBtn", 1, true ) then
				SetGameTooltips( this:GetName(), "请放入要重铸的装备" );
			elseif string.find( this:GetName(), "NeedItemBtn", 1, true  ) then
				SetGameTooltips( this:GetName(), "请放入对应装备等级的重铸石" );
			end
		elseif this:GetParent() == "ArmAdvRecastFrame" then
			SetGameTooltips( this:GetName(), "请放入要重铸的装备" );
		end
	end
end

function ArmRecastFrameItemButton_OnClick()
	if arg1 == "RightButton" then
		local nListType = this:GetClientUserData( 0 );
		local nGridIdx	= this:GetClientUserData( 1 );
		ClearItemLock( this:GetName() )
		this:SetClientUserData( 0,0 )
		this:SetClientUserData( 1,0 )
		this:SetClientUserData( 2,0 )
		this:SetClientUserData( 3,0 )
		UpdateArmRecastFrame();
		ArmRecastFrameSaveSuccessBtn:Hide()
	end
end

function ArmRecastFrameItemButton_Drag()	
	--处理开始拖动
	local nListType	= this:GetClientUserData( 0 );
	local nGrid		= this:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_RESOLVE;
		local IconTex = getglobal(this:GetName().."IconTexture");
		if this:GetName() == "ArmRecastFrameArmItemBtn" then
			UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),1 );
		elseif this:GetName() == "ArmRecastFrameNeedItemBtn" then
			UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),2 );
		end
	end
end

function ArmRecastFrameItemBtnClearBtn( szBtnName, desList, desGrid )
	local btn = getglobal( szBtnName );
	local nListType = btn:GetClientUserData( 0 );
	local nGridIdx	= btn:GetClientUserData( 1 );
	ClearItemLock( szBtnName )
	btn:SetClientUserData( 0,0 )
	btn:SetClientUserData( 1,0 )
	btn:SetClientUserData( 2,0 )
	btn:SetClientUserData( 3,0 )
	UpdateArmRecastFrame();
	ArmRecastFrameSaveSuccessBtn:Hide()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx )
	if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
		container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
	end
end

------------------------------------	高级重铸	--------------------------------------------

local nCurMoneyType = CONSUME_TYPE_BINDGODCOIN_TOLUA;
local nCurSelectIndex = 1;


function ArmAdvRecastFrame_OnShow()
	ClearArmAdvRecastFrame();
	ArmAdvRecastFrameArmInfo:Hide();
	ArmAdvRecastFrameSaveSuccessBtn:Hide();
	ArmAdvRecastFrameCheckGoldCoin_OnClick();
end

function ClearArmAdvRecastFrame()
	ArmAdvRecastFrameArmItemBtnBindTexture:Hide();
	ArmAdvRecastFrameArmItemBtnIconTexture:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	ArmAdvRecastFrameArmItemBtnUVAnimationTex:Hide();
	--ArmAdvRecastFrameAddArmTipFont:Show();
	ArmAdvRecastFrameNeedMoneyFont:SetText("")
	ArmAdvRecastFrameArmOldInfoRich:SetText( "",255,255,255 )
end

function ArmAdvRecastFrame_OnESC()
	if ArmRecastFrameArmInfo:IsShown() or ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "重铸", "本次重铸结果未保存，确定关闭重铸吗？" );
		MessageBoxFrame:SetClientString( "重铸面板关闭" );
		return true;
	end
	return false
end

function ArmAdvRecastFrame_OnHide()
	nListType = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 );
	nGrid = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 );
	if nListType > 0 then
		UnLockItem( nListType,nGrid,0 );
	end
	ArmAdvRecastFrameArmItemBtn:SetClientUserData( 0,0 )
	ArmAdvRecastFrameArmItemBtn:SetClientUserData( 1,0 )
	if ArmRecastSelectAttrFrame:IsShown() then
		local npcid = this:GetClientUserData( 0 );
		local npc = ActorMgr:FindActor(npcid)
		if not npc or npc:getResId() ~= NpcDialog:getLastNpcResId() or 
			util.CalActorsDistance(npc, ActorMgr:getMainPlayer()) > (NpcDialog:getMaxNpcTalkDist( npc ) / 100) ^ 2 then
			ArmRecastSelectAttrFrame:Hide();
		else
			ArmRecastSelectAttrFrameCloseBtn_OnClick();
		end		
	end
	if MessageBoxFrame:IsShown() then
		if MessageBoxFrame:GetClientString() == "替换重铸装备" or MessageBoxFrame:GetClientString() == "批量重铸"
			or MessageTitle:GetText() == "重铸切换强化" or MessageTitle:GetText() == "重铸切换洗炼"
			or MessageTitle:GetText() == "重铸切换绑定" or MessageTitle:GetText() == "重铸切换转星"
			or MessageBoxFrame:GetClientString() == "重铸面板关闭" or MessageBoxFrame:GetClientString() == "切换重铸"
			or MessageBoxFrame:GetClientString() == "批量重铸面板关闭"
			or MessageBoxFrame:GetClientString() == "金券重铸未绑定装备"
			or MessageTitle:GetText() == "单次重铸转批量重铸"
			or MessageTitle:GetText() == "批量重铸切换强化" 
			or MessageTitle:GetText() == "批量重铸切换洗炼"
			or MessageTitle:GetText() == "批量重铸切换绑定"
			or MessageTitle:GetText() == "批量重铸切换转星" then
			MessageBoxFrame:Hide();
		end
	end
	if dragfrom == DRAG_ARM_ADVRESOLVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
	ArmRecastSelectAttrFrame:Hide();
	ArmAdvRecastFrameArmInfo:Hide();
end

function ArmAdvRecastFrame_OnLoad()
	ArmAdvRecastFrameDescRich:resizeRect( 250,100 );
	local szText = [[☆  高级重铸可选择消耗金币或者金券
☆  批量重铸重置装备基础属性10次，可在结果中选择任意一个保存
]]
	ArmAdvRecastFrameDescRich:SetText( szText,170,160,120);
	ItemFirstCallBack( "ArmAdvRecastFrame" , Save2ArmAdvRecastFrame );
end

function ArmAdvRecastFrameItemButton_OnRecive()
	ArmAdvRecastFrameItemButton_OnReciveDrag( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
end

function Save2ArmAdvRecastFrame( item, nSrcList, nSrcGridIndex )
	ArmAdvRecastFrameItemButton_OnReciveDrag( "ArmAdvRecastFrameArmItemBtn", item:getListType(), item:getGridIdx(), 0, 0 );
end

function ArmAdvRecastFrameCheckGoldCoin_OnClick()
	ArmAdvRecastFrameCheckTicket:setCheckState( false )
	ArmAdvRecastFrameCheckGoldCoin:setCheckState( true )
	nCurMoneyType = CONSUME_TYPE_BINDGODCOIN_TOLUA;
	UpdateAdvArmRecastFrame();
end

function ArmAdvRecastFrameCheckTicket_OnClick()
	ArmAdvRecastFrameCheckTicket:setCheckState( true )
	ArmAdvRecastFrameCheckGoldCoin:setCheckState( false )
	nCurMoneyType = CONSUME_TYPE_GODCOIN_TOLUA;
	UpdateAdvArmRecastFrame();
end

function ArmAdvRecastFrameOKBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
		local nMoney = 0;
	local szMoneyName = ""
	if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
		nMoney = mainplayer:getGodCoin();
		szMoneyName = "金币"
	else
		nMoney = mainplayer:getBindGodCoin()
		szMoneyName = "金券"
	end
	if nMoney < tonumber( ArmAdvRecastFrameNeedMoneyFont:GetText() ) then
		if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
		else
			ShowMidTips( szMoneyName .. "不足" )
		end
		return
	end
	if TradeFrame:IsShown() then
		ShowMidTips("交易中不可重铸");
		return;
	end
	local armListType = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 );
	if os.clock() - nLastRequestRecast > 0.5 then
		nLastRequestRecast = os.clock();
		if ArmRecastSelectAttrFrame:IsShown() then
			MessageBox( "重铸","本次重铸结果未保存，确定关闭批量重铸界面吗？" );
			MessageBoxFrame:SetClientString( "重新单次重铸装备" )
			MessageBoxFrame:SetClientUserData( 0,nCurMoneyType )
			MessageBoxFrame:SetClientUserData( 1,armListType );
			MessageBoxFrame:SetClientUserData( 2,armGridIndex );
			return;
		end
		
		if nCurMoneyType == CONSUME_TYPE_BINDGODCOIN_TOLUA then
			local container = mainplayer:getContainer();
			local item = container:getItem( armListType,armGridIndex );
			if not item:isItemBind() then
				MessageBox( "绑定提示","使用金券重铸会使该装备绑定，确定继续重铸？" );
				MessageBoxFrame:SetClientString( "金券重铸未绑定装备" )
				MessageBoxFrame:SetClientUserData( 0,nCurMoneyType )
				MessageBoxFrame:SetClientUserData( 1,armListType );
				MessageBoxFrame:SetClientUserData( 2,armGridIndex );
				MessageBoxFrame:SetClientUserData( 3, 0 );
				return;
			end
		end

		GameArmProce:advanceRecastArm( nCurMoneyType,armListType,armGridIndex );		
	end
end

function ArmAdvRecastFrameBatchOKBtn_OnClick()
	if ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox( "单次重铸转批量重铸", "本次重铸结果未保存，确定切换批量重铸？" )
		return;
	end
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local nMoney = 0;
	local szMoneyName = ""
	if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
		nMoney = mainplayer:getGodCoin();
		szMoneyName = "金币"
	else
		nMoney = mainplayer:getBindGodCoin()
		szMoneyName = "金券"
	end
	if nMoney < 10*tonumber( ArmAdvRecastFrameNeedMoneyFont:GetText() ) then
		if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
			ExchangePointFrame:SetClientUserData(0,1);
			MarketManager:requestQueryTianyuan();
		else
			ShowMidTips( szMoneyName .. "不足" )
		end
		return
	end
	local armListType = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 );
	if os.clock() - nLastRequestRecast > 0.5 then
		if nCurMoneyType == CONSUME_TYPE_BINDGODCOIN_TOLUA then
			local container = mainplayer:getContainer();
			local item = container:getItem( armListType,armGridIndex );
			if not item:isItemBind() then
				MessageBox( "绑定提示","使用金券重铸会使该装备绑定，确定继续重铸？" );
				MessageBoxFrame:SetClientString( "金券重铸未绑定装备" )
				MessageBoxFrame:SetClientUserData( 0,nCurMoneyType )
				MessageBoxFrame:SetClientUserData( 1,armListType );
				MessageBoxFrame:SetClientUserData( 2,armGridIndex );
				MessageBoxFrame:SetClientUserData( 3, 1 );
				return true;
			end
		end

		GameArmProce:batchRecastArm( nCurMoneyType,armListType,armGridIndex );
		nLastRequestRecast = os.clock();
	end
end

function ArmAdvRecastFrameOKBtn_OnEnter()
	local szMoneyName = ""
	if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
		szMoneyName = "金币"
	else
		szMoneyName = "金券"
	end
	local szText = "消耗" .. tonumber(ArmAdvRecastFrameNeedMoneyFont:GetText()) .. szMoneyName .. "重铸1次"
	SetGameTooltips(this:GetName(),szText)
end

function ArmAdvRecastFrameBatchOKBtn_OnEnter()
	local szMoneyName = ""
	if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
		szMoneyName = "金币"
	else
		szMoneyName = "金券"
	end
	local szText = "消耗" .. 10*tonumber(ArmAdvRecastFrameNeedMoneyFont:GetText()) .. szMoneyName .. "重铸10次"
	SetGameTooltips(this:GetName(),szText)
end

function ArmAdvRecastFrameArmInfoSaveBtn_OnClick()
	local armListType = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 );
	local armGridIndex = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 );
	GameArmProce:saveRecastArm( 0,armListType,armGridIndex )
	ArmAdvRecastFrameArmInfo:Hide();
	ArmAdvRecastFrameSaveSuccessBtn:Show();
end

function ArmAdvRecastFrameArmInfoCancelBtn_OnClick()
	ArmAdvRecastFrameArmInfo:Hide();
end

function UpdateAdvSelectRecastResult()
	local uiMsg = UIMSG:getUIMsg();
	local nList = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 )
	local nGrid = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 )
	local mainplayer = ActorMgr:getMainPlayer();
	local container = mainplayer:getContainer();
	local item = container:getItem( nList,nGrid )
	local itemDef = item:getItemDef();

	if item:getItemId() > 0 and isSameWid( item:getItemWId(),uiMsg.RecastInfo.WID ) then
		if uiMsg.RecastInfo.RecaseNum > 1 then			
			ArmRecastSelectAttrFrame:SetClientUserData( 0,nList )
			ArmRecastSelectAttrFrame:SetClientUserData( 1,nGrid )
			if not ArmRecastSelectAttrFrame:IsShown() then
				ArmRecastSelectAttrFrame:Show();
			end
			UpdateArmRecasetSelectAttrInfo();		
		else
			local recastInfo = uiMsg.RecastInfo.RecastItemsDetail[0]
			ArmAdvRecastFrameArmInfo:Show();
			local szText = getRecastNewAttrDesc( recastInfo,item,itemDef )

			ArmRecastSelectAttrFrame:Hide();

			ArmAdvRecastFrameArmInfoRich:SetText( "#cff9600本次重铸属性#n\n" .. szText, 255, 255, 255 )

			-- 处理属性增减标记
			local nMinPhsicAtkMul,nMaxPhsicAtkMul,nMinMagicAtkMul,nMaxMagicAtkMul,nMinAtkMul,nMaxAtkMul;
			local nIndex = 1;
			for i=1,recastInfo.BaseAttrNum do
				if nIndex > MAX_BASE_ATTR_NUM then
					break
				end
				local resultID = recastInfo.BaseInfo[i-1].ResultID;
				local val1,val2 = getItemResultValue( itemDef,resultID )
				local nChg = math.floor( recastInfo.BaseInfo[i-1].Mul * val2 / 100 ) - math.floor( item:getItemMulVal( resultID ) * val2 / 100 );
				if resultID == RESULT_WEAPON_MIN_PHSIC_ATK then
					nMinPhsicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MAX_PHSIC_ATK then
					nMaxPhsicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MIN_MAGIC_ATK then
					nMinMagicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MAX_MAGIC_ATK then
					nMaxMagicAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MIN_ATK then
					nMinAtkMul = nChg
				elseif resultID == RESULT_WEAPON_MAX_ATK then
					nMaxAtkMul = nChg
				else
					local tex = getglobal( "ArmAdvRecastFrameArmInfoChangeSignTex" .. nIndex );
					if nChg == 0 then
						tex:SetTexUV( 728,741,13,4 )
						tex:SetSize( 13,4 );
					elseif nChg > 0 then
						tex:SetTexUV( 651,622,11,12 )
						tex:SetSize( 11,12 );
					else
						tex:SetTexUV( 639,622,11,12 )
						tex:SetSize( 11,12 );
					end					
					tex:Show();
					local nWidth = (12 + ArmAdvRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
					tex:SetPoint( "topleft","ArmAdvRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
					nIndex = nIndex + 1;
				end			
			end
			if nMinPhsicAtkMul ~= nil and nMaxPhsicAtkMul ~= nil and nIndex <= MAX_BASE_ATTR_NUM then
				local nChg = (nMinPhsicAtkMul + nMaxPhsicAtkMul)/2
				local tex = getglobal( "ArmAdvRecastFrameArmInfoChangeSignTex" .. nIndex );
				if nChg == 0 then
					tex:SetTexUV( 728,741,13,4 )
					tex:SetSize( 13,4 );
				elseif nChg > 0 then
					tex:SetTexUV( 651,622,11,12 )
					tex:SetSize( 11,12 );
				else
					tex:SetTexUV( 639,622,11,12 )
					tex:SetSize( 11,12 );
				end
				tex:Show();
				local nWidth = (12 + ArmAdvRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
				tex:SetPoint( "topleft","ArmAdvRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
				nIndex = nIndex + 1;
			end
			if nMinMagicAtkMul ~= nil and nMaxMagicAtkMul ~= nil and nIndex <= MAX_BASE_ATTR_NUM then
				local nChg = (nMinMagicAtkMul + nMaxMagicAtkMul)/2
				local tex = getglobal( "ArmAdvRecastFrameArmInfoChangeSignTex" .. nIndex );
				if nChg == 0 then
					tex:SetTexUV( 728,741,13,4 )
					tex:SetSize( 13,4 );
				elseif nChg > 0 then
					tex:SetTexUV( 651,622,11,12 )
					tex:SetSize( 11,12 );
				else
					tex:SetTexUV( 639,622,11,12 )
					tex:SetSize( 11,12 );
				end
				tex:Show();
				local nWidth = (12 + ArmAdvRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
				tex:SetPoint( "topleft","ArmAdvRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
				nIndex = nIndex + 1;
			end
			if nMinAtkMul ~= nil and nMaxAtkMul ~= nil and nIndex <= MAX_BASE_ATTR_NUM then
				local nChg = (nMinAtkMul + nMaxAtkMul)/2
				local tex = getglobal( "ArmAdvRecastFrameArmInfoChangeSignTex" .. nIndex );
				if nChg == 0 then
					tex:SetTexUV( 728,741,13,4 )
					tex:SetSize( 13,4 );
				elseif nChg > 0 then
					tex:SetTexUV( 651,622,11,12 )
					tex:SetSize( 11,12 );
				else
					tex:SetTexUV( 639,622,11,12 )
					tex:SetSize( 11,12 );
				end
				tex:Show();
				local nWidth = (12 + ArmAdvRecastFrameArmInfoRich:getLineWidth( nIndex ))/GetScreenScaleY()
				tex:SetPoint( "topleft","ArmAdvRecastFrameArmInfo","topleft", nWidth, 20 * nIndex )
				nIndex = nIndex + 1;
			end
			for i=nIndex,MAX_BASE_ATTR_NUM do
				local tex = getglobal( "ArmAdvRecastFrameArmInfoChangeSignTex" .. i );
				tex:Hide();
			end
		end
	end
end

function ArmAdvRecastFrameItemButton_OnReciveDrag( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "金券重铸未绑定装备" then return end;
	if dragfrom == DRAG_ARM_ADVRESOLVE then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	elseif dragfrom ~= CONTAINER_TYPE_SUNDRIES then
		ShowMidTips("只有背包内装备才可进行此操作");
		return;
	end	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	if dragfrom == CONTAINER_TYPE_SUNDRIES then
		local container = mainplayer:getContainer();
		local srcList		= dragfrom;		-- 源容器类型
		local srcGrid		= dragdata1;	-- 从源容器哪个格子过来
		local item			= container:getItem( srcList, srcGrid );
		local itemdef = item:getItemDef();
		local btn = getglobal( szBtnName )

		if item:getItemType() ~= ITEM_SLOTTYPE_AMM then
			ShowMidTips( "请放入装备" );
			return;
		end		
		
		--[[
		if itemdef.Important == 0 then
			ShowMidTips( "白色品质不能重铸" );
			return;
		end
		]]--

		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "时装" then
			ShowMidTips( "时装不能重铸" )
			return
		end
		if GetItemTypeName( math.floor(item:getItemId() / 10000 ) ) == "徽章" then
			ShowMidTips( "徽章装备不能重铸" );
			return;
		end
		if item:isArmFengYin() then
			ShowMidTips("封印装备无法进行此操作");
			return;
		end

		if ArmRecastSelectAttrFrame:IsShown() then
			MessageBox( "重铸","本次重铸结果未保存，确定关闭批量重铸界面吗？" );
			MessageBoxFrame:SetClientString( "替换重铸装备" )
			MessageBoxFrame:SetClientUserData( 0,srcList )
			MessageBoxFrame:SetClientUserData( 1,srcGrid );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		
		local nListType = btn:GetClientUserData( 0 );
		local nGridIdx	= btn:GetClientUserData( 1 );
		local srcitem = container:getItem( nListType,nGridIdx );			
		local nItemId	= srcitem:getItemId();
		if nItemId > 0 then
			UnLockItem( nListType, nGridIdx, nItemId );--解锁
		end
		LockItem( item:getListType(), item:getGridIdx(), item:getItemId(), szBtnName );
		btn:SetClientUserData( 0,srcList )
		btn:SetClientUserData( 1,srcGrid )
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;		
		ArmAdvRecastFrameArmInfo:Hide();
		ArmAdvRecastFrameSaveSuccessBtn:Hide();
		ArmRecastSelectAttrFrame:Hide();
		UpdateAdvArmRecastFrame();
	end
end

t_ArmAdvRecastCost = {
						[0] = LEVEL40_TOLUA,	-- 0-9
						[1] = LEVEL40_TOLUA,	-- 10-19
						[2] = LEVEL40_TOLUA,	-- 20-29
						[3] = LEVEL40_TOLUA,	-- 30-39
						[4] = LEVEL60_TOLUA,	-- 40-49
						[5] = LEVEL60_TOLUA,	-- 50-59
						[6] = LEVEL80_TOLUA,	-- 60-69
						[7] = LEVEL80_TOLUA,	-- 70-79
						[8] = LEVEL100_TOLUA,	-- 80-89
						[9] = LEVEL100_TOLUA,	-- 90-99
						[10] = LEVEL120_TOLUA,	-- 100-109
						[11] = LEVEL120_TOLUA,	-- 110-119
						[12] = LEVEL120_TOLUA,	-- 120
					}

function UpdateAdvArmRecastFrame()
	ArmAdvRecastFrameOKBtn:Disable();
	ArmAdvRecastFrameBatchOKBtn:Disable();

	ClearArmAdvRecastFrame();

	if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then			
		ArmAdvRecastFrameMoneyBtn:ChangeNormalTexture( 99, 494, 17, 18 );			
		ArmAdvRecastFrameMoneyBtn:SetClientUserData( 0,1 )
	else
		ArmAdvRecastFrameMoneyBtn:ChangeNormalTexture( 57,494,20,18 );			
		ArmAdvRecastFrameMoneyBtn:SetClientUserData( 0,2 )
	end
	
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();

	local nListType = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 );
	local nGrid = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 );
	--[[
	if nListType == 0 then
		ArmAdvRecastFrameAddArmTipFont:Show();
	else
		ArmAdvRecastFrameAddArmTipFont:Hide();
	end
	--]]
	local item = container:getItem( nListType,nGrid );
	ArmAdvRecastFrameArmItemBtnBindTexture:Hide();	
	if item:getItemId() > 0 then
		-- 设置图标，
		local equipItemDef = item:getItemDef();
		local IconPath = GetItemEquipIconPath()..equipItemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end	
		ArmAdvRecastFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( equipItemDef, "ArmAdvRecastFrameArmItemBtnUVAnimationTex",item);
		if item:isItemBind() then
			ArmAdvRecastFrameArmItemBtnBindTexture:Show();
		else
			ArmAdvRecastFrameArmItemBtnBindTexture:Hide();
		end
		local nNeedMoney = t_ArmAdvRecastCost[ math.floor((item:getItemDef().WearPre.RoleLevelMin)/10) ] ;
		ArmAdvRecastFrameNeedMoneyFont:SetText( nNeedMoney );
		ArmAdvRecastFrameOKBtn:Enable();
		ArmAdvRecastFrameBatchOKBtn:Enable();

		local tBaseAttr = GetItemNewBaseAttrDesc( equipItemDef,item );
		local szText = "#cff9600原装备属性#n\n";
		for i=1,table.getn(tBaseAttr) do
			szText = szText .. "\b" .. tBaseAttr[i][1] .. "：" .. tBaseAttr[i][2] .. "\n";
			
		end
		ArmAdvRecastFrameArmOldInfoRich:SetText( szText, 255, 255, 255 )
	else
		if ArmAdvRecastFrameArmInfo:IsShown() then
			ArmAdvRecastFrameArmInfo:Hide();
		end
	end
end

function ArmAdvRecastFrameItemButton_Drag()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "金券重铸未绑定装备" then return end;
	--处理开始拖动
	local nListType	= this:GetClientUserData( 0 );
	local nGrid		= this:GetClientUserData( 1 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType,nGrid );
	if item:getItemId() > 0 then
		UIMSG:getUIGV().DragFrom = DRAG_ARM_ADVRESOLVE;
		local IconTex = getglobal(this:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), item:getListType(), item:getGridIdx(),0 );
	end
end

function ArmAdvRecastFrameItemBtnClearBtn( desList, desGrid )
	local nListType = ArmAdvRecastFrameArmItemBtn:GetClientUserData( 0 );
	local nGridIdx	= ArmAdvRecastFrameArmItemBtn:GetClientUserData( 1 );
	UnLockItem( nListType, nGridIdx, nItemId );--解锁
	ArmAdvRecastFrameArmItemBtn:SetClientUserData( 0,0 )
	ArmAdvRecastFrameArmItemBtn:SetClientUserData( 1,0 )
	ArmAdvRecastFrameArmItemBtn:SetClientUserData( 2,0 )
	UpdateAdvArmRecastFrame();
	ArmAdvRecastFrameSaveSuccessBtn:Hide();
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local item = container:getItem( nListType, nGridIdx )
	if item:getItemId() > 0 and item:getGridIdx() ~= desGrid then
		container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
	end
end

function ArmAdvRecastFrameItemButton_OnClick()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "金券重铸未绑定装备" then return end;
	if arg1 == "RightButton" then
		local nListType = this:GetClientUserData( 0 );
		local nGridIdx	= this:GetClientUserData( 1 );
		UnLockItem( nListType, nGridIdx, nItemId );--解锁
		this:SetClientUserData( 0,0 )
		this:SetClientUserData( 1,0 )
		this:SetClientUserData( 2,0 )
		UpdateAdvArmRecastFrame();
		ArmAdvRecastFrameSaveSuccessBtn:Hide()
	end
end

local MAX_RECAST_SELECT_ATTR_NUM = 10

function ArmRecastSelectAttrFrame_OnLoad()
	for i=1,MAX_RECAST_SELECT_ATTR_NUM do
		local tex = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i .. "Bkg" )
		if math.mod( i,2 ) == 0 then
			tex:ChangeTextureTemplate( "WhiteCategoryBkg" );
		else
			tex:ChangeTextureTemplate( "BlackCategoryBkg" );
		end
	end
end

local tSelectAttrColor = {
	GetColor()[COLOR_GREEN],
	GetColor()[COLOR_GREEN],
	GetColor()[COLOR_BULE],
	GetColor()[COLOR_PURPLE],
}

local function GetArmRecastItemResultConfig( data )
	local oneConfig = GetOneItemResultConfig( data );
	if oneConfig ~= nil then return oneConfig end
	local tNewAttackAttr = { { RESULT_WEAPON_MIN_PHSIC_ATK,		"物理攻击下限"},
							{ RESULT_WEAPON_MAX_PHSIC_ATK,		"物理攻击上限"},
							{ RESULT_WEAPON_MIN_MAGIC_ATK,		"魔法攻击下限"},
							{ RESULT_WEAPON_MAX_MAGIC_ATK,		"魔法攻击上限"},
							{ RESULT_WEAPON_MIN_ATK,		"双系攻击下限"},
							{ RESULT_WEAPON_MAX_ATK,		"双系攻击上限"},
						   }
	for index, oneItemResultConfig in ipairs( tNewAttackAttr ) do
		if oneItemResultConfig[1] == data["resultID"] then
			return oneItemResultConfig
		end
	end
	return nil
end

function ClearArmRecastSelectAttrInfo()
	for i=1,MAX_RECAST_SELECT_ATTR_NUM do
		local btn = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i )
		local attrFont1 = getglobal("ArmRecastSelectAttrFrameAttrBtn" .. i .."AttrFont1" );
		local attrFont2 = getglobal("ArmRecastSelectAttrFrameAttrBtn" .. i .."AttrFont2" );
		local RecommandTex1 = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i .."Recommand1" );
		local RecommandTex2 = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i .."Recommand2" );
		attrFont1:SetText("")
		attrFont2:SetText("")
		RecommandTex1:Hide()
		RecommandTex2:Hide()
		btn:Hide();
	end
	ArmRecastSelectAttrFrameSaveFont:Hide();
	ArmRecastSelectAttrFrameSaveBtn:Enable();
end

function UpdateArmRecasetSelectAttrArmInfo()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local container = mainplayer:getContainer();
	local nList = ArmRecastSelectAttrFrame:GetClientUserData( 0 )
	local nGrid = ArmRecastSelectAttrFrame:GetClientUserData( 1 )
	local item = container:getItem( nList, nGrid )
	local itemdef = item:getItemDef()
	if item:getItemId() > 0 then
		local newBaseAttr = GetItemNewBaseAttrDesc( itemdef,item )
		ArmRecastSelectAttrFrameRich:SetText( "",255,255,255 )
		for i=1,table.getn( newBaseAttr ) do
			ArmRecastSelectAttrFrameRich:AddText( newBaseAttr[i][1] .. "：" .. newBaseAttr[i][2] .. "\n", 255,255,255 )
		end
		IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ArmRecastSelectAttrFrameArmItemBtnIconTexture:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemdef, "ArmRecastSelectAttrFrameArmItemBtnUVAnimationTex", stuffItem);
		ArmRecastSelectAttrFrameArmItemBtnBoxTexture:Show();
		if item:isItemBind() then
			ArmRecastSelectAttrFrameArmItemBtnBindTexture:Show();
		else
			ArmRecastSelectAttrFrameArmItemBtnBindTexture:Hide();
		end
		ArmRecastSelectAttrFrameArmItemBtn:SetClientUserData( 0,nList )
		ArmRecastSelectAttrFrameArmItemBtn:SetClientUserData( 1,nGrid )
	end
end

function UpdateArmRecasetSelectAttrInfo()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then 
		this:Hide();
		return
	end
	local container = mainplayer:getContainer();
	local nList = ArmRecastSelectAttrFrame:GetClientUserData( 0 )
	local nGrid = ArmRecastSelectAttrFrame:GetClientUserData( 1 )
	local item = container:getItem( nList, nGrid )
	local itemdef = item:getItemDef()
	ArmRecastSelectAttrDisCheckAll();
	if item:getItemId() > 0 then
		local uiMsg = UIMSG:getUIMsg();
		UpdateArmRecasetSelectAttrArmInfo();
		ClearArmRecastSelectAttrInfo();
		for i=1,uiMsg.RecastInfo.RecaseNum do
			local recastDetail = uiMsg.RecastInfo.RecastItemsDetail[i-1];
			local btn = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i )
			for j=1,recastDetail.BaseAttrNum do
				local val1,val2 = getItemResultValue( itemdef,recastDetail.BaseInfo[j-1].ResultID );
				local maxVal = val1 + val2;
				local nVal = val1 + math.floor( val2 * recastDetail.BaseInfo[j-1].Mul/100 );				
				local szVal = recastDetail.BaseInfo[j-1].ResultID == RESULT_BASE_ATTR_HEAVY_HIT_VAL and string.format("%.2f%%",nVal/100) or nVal
				local itemResultConfig = GetArmRecastItemResultConfig( { ["resultID"] = recastDetail.BaseInfo[j-1].ResultID } )				
				if itemResultConfig ~= nil then
					local szText = itemResultConfig[2] .. "：" .. szVal .. ( nVal >= maxVal and "（满）" or "" )
					local AttrFont = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i .."AttrFont" .. j );
					local RecommandTex = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i .."Recommand" .. j );
					local area = GameArmProce:getArmRecastArea( recastDetail.BaseInfo[j-1].ResultID,recastDetail.BaseInfo[j-1].Mul )
					if area > 1 then
						RecommandTex:Show();
					else
						RecommandTex:Hide();
					end
					AttrFont:SetText( szText );
					AttrFont:SetTextColor( tSelectAttrColor[area+1][1],tSelectAttrColor[area+1][2],tSelectAttrColor[area+1][3] )
				end
			end
			btn:Show();
		end
		-- 默认选中第一个
		ArmRecastSelectAttrFrameAttrBtn1Check:setCheckState( true )
		ArmRecastSelectAttrFrameAttrBtn1:Checked();
		nCurSelectIndex = 1;
	end
end

function ArmRecastSelectAttrFrame_OnShow()
	this:SetPoint( "center","UIClient","center",0,0 )
end

function ArmRecastSelectAttrFrame_OnHide()
	if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "重新单次重铸装备" then
		MessageBoxFrame:Hide();
	end
end

function ArmRecastSelectAttrFrameSaveBtn_OnClick()
	local armListType = ArmRecastSelectAttrFrame:GetClientUserData( 0 );
	local armGridIndex = ArmRecastSelectAttrFrame:GetClientUserData( 1 );
	GameArmProce:saveRecastArm( nCurSelectIndex-1,armListType,armGridIndex )
	ClearArmRecastSelectAttrInfo();
	ArmRecastSelectAttrFrameSaveFont:Show();
	ArmRecastSelectAttrFrameSaveBtn:Disable();
	ArmRecastSelectAttrFrame:Hide();
end

function ArmRecastSelectAttrFrameRecastBtn_OnClick()
	ArmAdvRecastFrameBatchOKBtn_OnClick();
end

function ArmRecastSelectAttrFrameSaveBtn_OnEnter()
	SetGameTooltips(this:GetName(), "选择一条重铸结果进行保存")
end

function ArmRecastSelectAttrFrameRecastBtn_OnEnter()
	local szMoneyName = ""
	if nCurMoneyType == CONSUME_TYPE_GODCOIN_TOLUA then
		szMoneyName = "金币"
	else
		szMoneyName = "金券"
	end
	local szText = "消耗" .. 10*tonumber(ArmAdvRecastFrameNeedMoneyFont:GetText()) .. szMoneyName .. "重铸10次"
	SetGameTooltips(this:GetName(),szText)
end

function ArmRecastSelectAttrDisCheckAll()
	for i=1,MAX_RECAST_SELECT_ATTR_NUM do
		local btn = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i )
		btn:DisChecked();
		local check = getglobal( "ArmRecastSelectAttrFrameAttrBtn" .. i .. "Check" )
		check:setCheckState( false );
	end
end

function ArmRecastSelectAttrFrameInfoBtn_OnClick()
	ArmRecastSelectAttrDisCheckAll();
	this:Checked();
	local check = getglobal( this:GetName() .. "Check" )
	check:setCheckState( true );
	nCurSelectIndex = this:GetClientID();
end

function ArmRecastSelectAttrFrameCloseBtn_OnClick()
	if ArmRecastSelectAttrFrameAttrBtn1:IsShown() then
		MessageBox( "重铸","本次重铸结果未保存，确定关闭批量重铸界面吗？" );
		MessageBoxFrame:SetClientString( "批量重铸" )
	else
		ArmRecastSelectAttrFrame:Hide();
	end
end

function ArmRecastFrameAdvRecastPanelBtn_OnClick()
	if ArmRecastFrameArmInfo:IsShown() then
		MessageBox("重铸","本次重铸结果未保存，确定关闭重铸吗？")
		MessageBoxFrame:SetClientString( "切换高级重铸" )
		return;
	end
	local npcid = ArmRecastFrame:GetClientUserData( 0 )
	ArmRecastFrame:Hide();
	ArmAdvRecastFrame:Show();
	ArmAdvRecastFrame:SetClientUserData( 0, npcid )
	ArmAdvRecastFrame:SetPoint( "topleft", "UIClient", "topleft", ArmRecastFrame:GetRealLeft(), ArmRecastFrame:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmAdvRecastFrame" )	
	ArmAdvRecastFrameRecastBtn:Checked()	
	--util.testNpcDialogDistance( npcid, ArmAdvRecastFrame );
end

function ArmAdvRecastFrameRecastPanelBtn_OnClick()
	if ArmAdvRecastFrameArmInfo:IsShown() then
		MessageBox("重铸","本次重铸结果未保存，确定关闭重铸吗？")
		MessageBoxFrame:SetClientString( "切换重铸" )
		return
	end
	if ArmRecastSelectAttrFrame:IsShown() then
		MessageBox( "批量重铸切换重铸", "本次批量重铸结果未保存，确定关闭批量重铸吗？" );
		MessageBoxFrame:SetClientString(  "切换重铸"  );
		return;
	end
	local npcid = ArmAdvRecastFrame:GetClientUserData( 0 )
	ArmAdvRecastFrame:Hide();
	ArmRecastFrame:Show();
	ArmRecastFrame:SetClientUserData( 0, npcid )
	ArmRecastFrame:SetPoint( "topleft", "UIClient", "topleft", ArmRecastFrame:GetRealLeft(), ArmRecastFrame:GetRealTop() )
	ArmProceFrameDisCheckAllBtn( "ArmRecastFrame" )	
	ArmRecastFrameRecastBtn:Checked()	
	--util.testNpcDialogDistance( npcid, ArmRecastFrame );
end
