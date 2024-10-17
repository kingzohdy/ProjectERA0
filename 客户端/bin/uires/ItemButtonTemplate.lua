local PETARMTYPE		= 1;
local nCheckItemIndex	= 0;
local szCurOnEnterBtn	= "";

local t_itemControl = {};

function GetItemControl()
	return t_itemControl;
end

t_itemControl["isSatisfyLv"] =
function ( self, data )
	local itemDef		= data["itemDef"];
	local mainplayer	= ActorMgr:getMainPlayer();
	local nLv			= mainplayer:getLv();
	if itemDef.WearPre.RoleLevelMin > 0 and mainplayer:getLv() < itemDef.WearPre.RoleLevelMin then
		ShowMidTips("�ȼ�����");
		return false;
	end

	if itemDef.WearPre.RoleLevelMax > 0 and mainplayer:getLv() > itemDef.WearPre.RoleLevelMax then
		ShowMidTips("�ȼ�̫��");
		return false;
	end

	return true;
end

-- ʹ��ָ��װ��
function SendEquip( nItemId, EquipPos, nListType, nGridIdx )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end

	local container = MainPlayer:getContainer();
	if container == nil then
		return;
	end

	local item = container:getItem( nListType, nGridIdx );
	if item:getItemId() ~= 0 then
		container:usedItem( item:getListType(), item:getGridIdx(), EquipPos );
	end
end

-- ����������Ʒ��
t_LockItem = {};

-- ��Ҫתʹ��CDʱ��������
local t_ItemButtonTemplateParentName =
{
	"PlayerContainerFrame",
	"StashFrame",
};

-- ��Ʒ�ܷ�ʹ�ã���ȴʱ�䣩
function IsCanDo( szParentName )
	for i = 1, table.getn(t_ItemButtonTemplateParentName) do
		if string.find(szParentName, t_ItemButtonTemplateParentName[i], 1, true) then
			local parent = getglobal( szParentName )
			if parent ~= nil and parent:IsShown() then
				return true;
			end
		end
	end
	return false;
end

local t_ItemFirstCallBackFun = {};


--@Desc: �������ͬʱ��ʱ����������Ӧ�Ļص�����
--@Param1:��������
--@Param2:������Ӧ���һ�����
function ItemFirstCallBack( parentname, functionname )
	table.insert( t_ItemFirstCallBackFun, { parentname, functionname } );
end

-- ������Ʒ
function LockItem( nListType, nGridIdx, itemid, szParent )
	local bfound = false;

	if table.getn( t_LockItem ) == 0 then
		table.insert( t_LockItem, { nListType, nGridIdx, itemid, szParent } );
	else
		for i=1, table.getn( t_LockItem ) do
			if t_LockItem[i][1] == nListType and t_LockItem[i][2] == nGridIdx then
				bfound = true;
			end
		end
		if not bfound then
			table.insert( t_LockItem, { nListType, nGridIdx, itemid, szParent } );
		end
	end
	if PlayerContainerFrame:IsShown() and GetCurContainerType("PlayerContainerFrame") == nListType then
		local nIndex = nGridIdx + 1 - nSUNDRIESGridMax*( GetCurContainerPage() - 1 );
		if nIndex > 0 and nIndex <= nSUNDRIESGridMax then
			local btn = getglobal( "PlayerContainerFrameItem"..nIndex );
			local tnlockbtex = getglobal("PlayerContainerFrameItem"..nIndex.."IconTexture");
			tnlockbtex:SetGray(true);	
		end
	end
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer();
	container:onItemLockChange();
end

-- ������Ʒ
function UnLockItem( nListType, nGridIdx, itemid )
	local nIdx = 0;
	for i=1, table.getn( t_LockItem ) do
		if ( t_LockItem[i][1] == nListType and t_LockItem[i][2] == nGridIdx ) then
			nIdx = i;
			break;
		end
	end

	if nIdx ~= 0 then
		table.remove( t_LockItem, nIdx );
	end

	local nIndex = nGridIdx%nSUNDRIESGridMax + 1;
	if nIndex > 0 and nIndex < nSUNDRIESGridMax + 1 then
		local tnlockbtex = getglobal("PlayerContainerFrameItem"..nIndex.."IconTexture");
		tnlockbtex:SetGray(false);
	end
	local mainplayer = ActorMgr:getMainPlayer()
	local container = mainplayer:getContainer();
	container:onItemLockChange();
end

local SingleLockedItem

function SingleLockItem(item)
	if SingleLockedItem then
		UnLockItem(SingleLockedItem.list, SingleLockedItem.grid, SingleLockedItem.id)
	end
	SingleLockedItem = item
	if SingleLockedItem then
		LockItem(SingleLockedItem.list, SingleLockedItem.grid, SingleLockedItem.id)
	end
end

function ClearItemLock( szParent )
	if szParent == nil then
		t_LockItem = {};
		for i = 1, nSUNDRIESGridMax  do
			local tnlockbtex = getglobal("PlayerContainerFrameItem"..i.."IconTexture");
			tnlockbtex:SetGray(false);
		end
	end
	
	local index = 1;
	while true do
		if t_LockItem[index] == nil then
			break;
		end

		if t_LockItem[index][4] == szParent then
			local nIndex = (t_LockItem[index][2])%nSUNDRIESGridMax + 1;
			if nIndex > 0 and nIndex < nSUNDRIESGridMax + 1 then
				local tnlockbtex = getglobal("PlayerContainerFrameItem"..nIndex.."IconTexture");
				tnlockbtex:SetGray(false);
			end
			table.remove( t_LockItem, index );
			index = index - 1;
		end
		index = index + 1;
	end
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer == nil then return end
	local container = mainplayer:getContainer()
	container:onItemLockChange();
end

--��Ʒ�Ƿ�����
function IsItemLock( nListType, nGridIdx, itemid )
	for i=1, table.getn( t_LockItem ) do
		if nListType == 0 and nGridIdx == 0 and t_LockItem[i][3] == itemid then
			return true;
		elseif ( t_LockItem[i][1] == nListType and t_LockItem[i][2] == nGridIdx ) then
			return true;
		end
	end
	return false;
end

-- �жϱ����Ƿ�����Ʒ������
function IsContainerHasItemLock( nListType )
	for i=1, table.getn( t_LockItem ) do
		if t_LockItem[i][1] == nListType then
			return true;
		end
	end

	return false;
end

--������Ʒ�Ƿ�������ֻ����λ��
function IsContainerItemLock( nListType, nGridIdx )
	for i=1, table.getn( t_LockItem ) do
		if t_LockItem[i][1] == nListType and t_LockItem[i][2] == nGridIdx then
			return true;
		end
	end
	return false;
end

--@Desc:���ô��ڵ���Ʒ��ť����״̬
--@Param1:��Ʒ�� item
--@Param2:��Ʒ�� ������
--@Param3:��Ʒ�� ��λ
--@Param4:��Ʒ�� �ؼ���
function SetItemButtonCheckState( szParent, nGrid, szName )
	local btn = getglobal( szName );
	local bfound = false;

	local nIndex = 1;
	while true do
		if t_ItemButtonCheckIndex[nIndex] == nil then
			break;
		end
		if t_ItemButtonCheckIndex[nIndex][1] == szParent then
			if nGrid ~= t_ItemButtonCheckIndex[nIndex][2] then
				local nIndex =  (t_ItemButtonCheckIndex[nIndex][2])%nSUNDRIESGridMax + 1;
				if nIndex > 0 and nIndex < nSUNDRIESGridMax + 1 then
					local lastbtn = getglobal( szParent.."Item"..nIndex );
					lastbtn:DisChecked();
					btn:Checked();
				end
				table.remove( t_ItemButtonCheckIndex, nIndex );
				table.insert( t_ItemButtonCheckIndex, { szParent, nGrid } );
				nIndex = nIndex - 1;
			end
			bfound = true;
		end
		nIndex = nIndex + 1;
	end

	if table.getn(t_ItemButtonCheckIndex) == 0 then
		table.insert( t_ItemButtonCheckIndex, { szParent, nGrid } );
		btn:Checked();
	elseif not bfound then
		table.insert( t_ItemButtonCheckIndex, { szParent, nGrid } );
		btn:Checked();
	end
end

-- ����װ����Ч
function SetEquipImportantEffectUV( itemdef, frame,item )
	local ItemTex = getglobal( frame );

	if itemdef == nil then
		ItemTex:Hide();
		return;
	end
	if bitAnd( itemdef.CtrlFlag,ITEM_A_ToLUA ) or bitAnd( itemdef.CtrlFlag,ITEM_S_ToLUA ) then 
		HandlePurpleArmImportantEffectUV( itemdef, frame,item );
		return;
	end
	local nIndex = 11;
	if (item ~= nil and item:getStart() >= 3) or bitAnd( itemdef.CtrlFlag,ITEM_SHOW_LIGHT_TOLUA ) then
		ItemTex:SetUVAnimation(100, true);
		nIndex = 0;
	end
	if bitAnd( itemdef.CtrlFlag, ITEM_COLOR_GREEN_TOLUA ) then
		nIndex = nIndex + 1
	elseif bitAnd( itemdef.CtrlFlag, ITEM_COLOR_BLUE_TOLUA ) then
		nIndex = nIndex + 2
	elseif bitAnd( itemdef.CtrlFlag, ITEM_COLOR_PURPLE_TOLUA ) then
		nIndex = nIndex + 3
	elseif bitAnd( itemdef.CtrlFlag, ITEM_COLOR_PINK_TOLUA ) then
		nIndex = nIndex + 11
	elseif item ~= nil and CheckItemResult( itemdef, RESULT_GEM ) and ( item:getGemStart() > 0 or item:getGemPower() > 0 ) then 
		nIndex = 0;
		if itemdef.Important >= ITEM_IMPORTANT_EPIC then
				nIndex = 11;
		else
				nIndex = nIndex + itemdef.Important;
		end		
	else
		local suitDef = nil;
		if itemdef.SuitSetID > 0 then
			suitDef = getSuitSet( itemdef.SuitSetID );
		end
		if suitDef ~= nil and suitDef.SuitSetType > 0 then
			local num = suitDef.SuitSetType > 3 and 3 or 8;
			nIndex = nIndex + suitDef.SuitSetType + num;
		else
			if itemdef.Important == ITEM_IMPORTANT_NORMAL then
				nIndex = 0
			else
				nIndex = nIndex + itemdef.Important
			end			
		end
	end
	if nIndex == 0 then
		ItemTex:Hide()
		return
	end

	ItemTex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei  "..nIndex..".tga" );
	if nIndex <= 11 then
		ItemTex:SetUVAnimation(100, true);
	else
		ItemTex:StopUVAnim();
		ItemTex:Show();
	end
end

function HandlePurpleArmImportantEffectUV( itemdef, frame,item )
	local ItemTex = getglobal( frame );
	if itemdef == nil then
		ItemTex:Hide();
		return;
	end
	local nIndex = 11;
	if item == nil or item:getStart() < 3 then 
		if bitAnd( itemdef.CtrlFlag,ITEM_A_ToLUA ) then 
			nIndex = 35;
		elseif bitAnd( itemdef.CtrlFlag,ITEM_S_ToLUA )then 
			nIndex = 36;
		end
		ItemTex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei "..nIndex..".tga" );
		ItemTex:StopUVAnim();
		ItemTex:Show();
	else 
		if bitAnd( itemdef.CtrlFlag,ITEM_A_ToLUA ) then 
			nIndex = 37;
		elseif bitAnd( itemdef.CtrlFlag,ITEM_S_ToLUA ) then 
			nIndex = 38;
		end
		ItemTex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei "..nIndex..".tga" );
		ItemTex:SetUVAnimation(100, true);
	end
end
function ItemButton_OnUpdate()
	if this:GetParent() == "IdentifyFrame" then 
		UpdateIdentifyFrame();
	end
end

local t_cannotRightUse= 
{ 
--[USE_TARGET_PET] =	{}
resultID =	{	RESULT_PET_ADD_LOVEVAL,			-- ���ӳ����ҳ϶�
				RESULT_PET_WASH_GENIUS ,		-- ����ϴ�����
				RESULT_PET_ADD_ATTR ,			-- ��ǿ�������Ե���
				RESULT_PET_STRENGTHEN_STUFF ,	-- ����ǿ������
				RESULT_PET_STRENGTHEN_SAFE ,	-- ����ǿ������
				RESULT_PET_SKILL_BOOK ,			-- ����ѧϰ������
				RESULT_ADD_SKILL_GRID ,			-- �������Ӽ��ܸ��ӵĵ���
				RESULT_PET_WASH_SKILL_GRID ,	-- ����ϴ���ܸ��ӵ���
			} ,
subType = { ITEM_PETXUNFU_ID, },
cannotUseDesc = "�˵��߲���ͨ������Ҽ�ʹ��"
};

local t_rightReceiveFrames = {
							--[[["Pet_XiDianFrame"]				= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },]]
							["Pet_DevelopmentBuildUpFrame"]	= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							["Pet_SkillLearnFrame"]			= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							["PetStepUpFrame"]				= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							["PetImpUpFrame"]				= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							--[[
							["Pet_ZengQiangFrame"]			= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							
							
							["Pet_SkillLevelUpFrame"]		= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							["Pet_SkillGridExtendFrame"]	= { func = function ( item, nSrcList, nSrcGridIndex, szFrameName ) 
																			Save2Pet_OperationFrame( item, nSrcList, nSrcGridIndex, szFrameName ) 
																		end },
							--]]
							["RideLevelUpFrame"]			= { func = function ( item ) 
																			Save2RideLevelUpFrame( item ) 
																		end },
							["NpcCommitItemFrame"]			= { func = function ( item ) 
																			Save2NpcCommitItemScrollFrame( item ) 
																		end },
							["MultiCommitItemFrame"]		= { func = function(item)
																			AutoSetMultiCommitItemFrameItem(item)
																		end },
							["BaoShiHeChengFrame"]			= { func = function ( item ) 
																			Save2BaoShiHeChengFrame( item ) 
																		end },
							["BaoShiXiangQianFrame"]		= { func = function ( item ) 
																			Save2BaoBaoShiXiangQianFrame( item ) 
																		end },

							["EquipJieFengFrame"]			= { func = function ( item ) 
																			Save2_EquipJieFengFrame( item ) 
																		end },
							["SkillRuneCombineFrame"]		= { func = function ( item ) 
																			SetSkillRuneCombineItem(item)
																		end },
																	
							};
function ItemButton_OnLoad()
	this:RegisterEvent("GE_ITEMUSED_PASSCD");
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	this:SetClientUserData( 3, 0 );

	t_cannotRightUse.__index = function ( tab, key )
								--tab[key] = { resultID = {}, subType = {}, cannotUseDesc = "" };
								tab[key] = {};
								return tab[key];
							end
	setmetatable( t_cannotRightUse, t_cannotRightUse );

	for name, data in pairs( t_rightReceiveFrames ) do
		ItemFirstCallBack( name, data.func );
	end
end

local ContainerType = 
	{
		{ CONTAINER_TYPE_TASK,			"PlayerContainerFrame" },
		{ CONTAINER_TYPE_SUNDRIES,		"PlayerContainerFrame" },
		{ CONTAINER_TYPE_STASH,			"StashFrame" },
	}; 
 
function ItemButton_OnEvent()
	if( arg1 == "GE_ITEMUSED_PASSCD" ) then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then return end
		local container = MainPlayer:getContainer();
		if container == nil then return end
		if this:GetParent() == nil then return end;
		if string.find( this:GetName(),"BagItem",1,true ) then return end

		if IsCanDo( this:GetParent() ) then
			local nGridIndex = this:GetClientID()-1;
			if nGridIndex < 0 then
				writeErrorLog( "������������С��0��ָ��nGridIndex = "..nGridIndex.."����Ӧ�Ŀؼ�������"..this:GetName() );
			end
			for i = 1, table.getn( ContainerType ) do
				local nMaxNum = container:getContainerMaxNum( ContainerType[i][1] );
				if nGridIndex < nMaxNum then
					--[[
					if nGridIndex >= nMaxNum then
						return;
					end
					]]
					
					local item = container:getItem( ContainerType[i][1], nGridIndex );
					if this:GetParent() == ContainerType[i][2] 
					and item:getItemId() ~= 0 
					and item:getItemType() ~= ITEM_SLOTTYPE_NONE
					and GetCurContainerType(this:GetParent(), this:GetName()) == ContainerType[i][1] then
						local cd		= container:getItemCD( item:getItemId() );
						local starttime = container:getItemCDPassTime( item:getItemId() );
						this:SetCooldownTimer( starttime, starttime+cd, true );
						this:SetCooldownTextureRect( 3, 5, 38, 38 );
						SetItemCDDuration( item:getItemId(), cd );
					end
				end
			end
		end
	end
end

function ItemButton_OnDoubleClick()
	if this:GetParent() == "ContainerBuyFrame" then
		ContainerBuyFrameItemOnClick( this );
		ContainerBuyFrameBuyBtn_OnClick();
		return;
	end
	local nType		= this:GetClientUserData(0);
	if nType == DRAG_GETMAILITEM then
		GameMailManager:mail_GetGoods( MailReadFrame:GetClientUserData( 0 ), MailReadFrame:GetClientUserData( 1 ), this:GetClientID() - 1 );
	end
end

function ItemButton_OnClick()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end

	local container = MainPlayer:getContainer();
	if container == nil then
		return;
	end

	if( arg1 == "RightButton" ) then
		if UIIsInDragState() then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		-- ���������Ҽ��������
		if this:GetParent() == "PlayerContainerFrame" or this:GetParent() == "StashFrame" then
			ContainerRightBtnClick(this:GetName());
			return;
		end

		-- ������Ҽ��������
		if ( string.find( this:GetName(),"TradeFrame", 1, true ) ) then
			TradeRightBtnClick( this:GetName() );
			return;
		end

		-- ������Ҽ��������װ��
		if this:GetParent() == "EquipFrame" then
			WearformRigthBtnClick( this:GetName() );
			return;
		end

		-- ������Ҽ�������ʼ�
		if this:GetParent() == "MailSendItemFrame" then
			MailRigthBtnClick( this:GetName() );
			NormalItemTipsFrame:Hide();
			ArmItemTipsFrame:Hide();
			SecondArmItemTipsFrame:Hide();
			GameTooltip:Hide();
			return;
		end

		-- ������Ҽ�������ʼ�
		if this:GetParent() == "MailReadFrame" then
			GetMailItemRightBtnClick( this:GetName() );
			NormalItemTipsFrame:Hide();
			ArmItemTipsFrame:Hide();
			SecondArmItemTipsFrame:Hide();
			GameTooltip:Hide();
			return;
		end

		-- ������ڰ�̯��������Ҽ����
		if this:GetParent() == "StallSaleFrame" then
			StallSaleRightBtnClick( this:GetName() );
			return;
		end

		-- ������ڰ�̯�������Ҽ����
		if this:GetParent() == "StallBuyFrame" then
			StallBuyRightBtnClick( this:GetName() );
			return;
		end

		-- ������ڵ�������Ҽ����
		if this:GetParent() == "OnlineStoreFrame" then
			OnlineStoreItemRightBtnClick( this:GetName() );
			return;
		end

		-- �������������Ҽ������Ʒ
		if this:GetParent() == "ShopFrame" then
			ShopRightBtnClick( this:GetName() );
			return;
		end

		--������ڴ��ӹ�װ�����
		if this:GetParent() == "ArmProceStarFrame" or
			this:GetParent() == "ArmProceBindFrame" or
			this:GetParent() == "ArmProceUnBindFrame" then
			ArmProceFrameOnRightClick( this:GetName() );
			NormalItemTipsFrame:Hide();
			ArmItemTipsFrame:Hide();
			SecondArmItemTipsFrame:Hide();
			GameTooltip:Hide();
			return;
		end
		if this:GetParent() == "ArmProceStarMoveFrame" then			
			if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "Ʒ��ת�ư���ʾ" then return end;
			ArmProceStarMoveFrameOnRightClick( this:GetName() );
			NormalItemTipsFrame:Hide();
			ArmItemTipsFrame:Hide();
			SecondArmItemTipsFrame:Hide();
			GameTooltip:Hide();
			return;
		end
		--��ħ
		if this:GetParent() == "EnchantFrame" then
			EnchantOnRightClick( this:GetName() );
		end
		--��װ�Դ�����
		if string.find(this:GetName(),"FashionPreviewBtnWear",1,true) then
			FashionViewOnClick( this );
			return;
		end
		--����
		if this:GetParent() == "IdentifyFrame" then
			IdentifyOnRightClick( this:GetName() );
			return;
		end
		if this:GetParent() == "ArmStepUpFrame" then
			ArmStepUpFrameItemOnRightClick( this:GetName() );
			return;
		end
		if this:GetParent() == "ArmBaseResetFrame" then
			ArmBaseResetFrameItemOnRightClick( this:GetName() );
			return;
		end
		--���λû����
		if this:GetParent() == "UnRealBackFrame" then
			UnRealBackOnRightClick( this:GetName() );
			return;
		end
	end

	if( arg1 == "LeftButton" ) then
		-- �������
		if GetCurrentCursor() == "repair" and this:GetClientUserData( 3 ) == CONTAINER_TYPE_WEAR then
			RepairOneItem( CONTAINER_TYPE_WEAR, this:GetClientID() );
			return;
		end

		-- ������������������
		if this:GetParent() == "PlayerContainerFrame" or this:GetParent() == "StashFrame" then
			ContainerLeftBtnClick(this:GetName());
			return;
		end

		-- ����������������������װ��
		if this:GetParent() == "EquipFrame" then
			EquipLeftBtnClick(this:GetName());
			return;
		end

		-- ��������������������Ʒ
		if this:GetParent() == "ShopFrame" then
			ShopLeftBtnClick( this:GetName() );
			return;
		end

		-- ��������������ʼ�
		if this:GetParent() == "MailReadFrame" or this:GetParent() == "MailSendItemFrame" then
			GetMailItemLeftBtnClick( this:GetName() );
			return;
		end

		-- ������ڰ�̯�������������
		if this:GetParent() == "StallSaleFrame" then
			StallSaleLeftBtnClick( this:GetName() );
			return;
		end

		-- ������ڰ�̯������������
		if this:GetParent() == "StallBuyFrame" then
			StallBuyLeftBtnClick( this:GetName() );
			return;
		end

		-- ������ڵ������������
		if this:GetParent() == "OnlineStoreFrame" then
			OnlineStoreItemLeftBtnClick( this:GetName() );
			return;
		end
		
		if  this:GetParent() == "ArmProceGemLevelUpFrame" then
			if string.find( this:GetName(),"_InSureItemButton", 1, true ) then
				return;
			end
		end

		-- ��������̳�ҳ��
		if string.find(this:GetParent(),"MarketManagerFrame_SaleItem",1,true) then
			MarketManagerItemOnClick( this );
			return;
		end

		if this:GetParent() == "ContainerBuyFrame" then
			ContainerBuyFrameItemOnClick( this );
			return;
		end
	end
end

function ItemButton_OnEnter()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end

	local container = MainPlayer:getContainer();
	if container == nil then
		return;
	end

	-- �����������뱳��
	if this:GetParent() == "PlayerContainerFrame" or this:GetParent() == "StashFrame" or 
		this:GetParent() == "PlayerContainerFrameTrade" then
		ContainerOnEnter(this:GetName());
		return;
	end

	-- �����������뽻�����
	if  ( string.find( this:GetName(),"TradeFrame", 1, true ) ) then
		TradeOnEnter( this:GetName() );
		return;
	end

	-- �������������̵���Ʒ
	if this:GetParent() == "ShopFrame"
	or string.find( this:GetName(),"PromotionFrameRewardItem", 1, true )
	or string.find( this:GetName(),"GuideGiftFrame", 1, true ) then
		ShopOnEnter( this:GetName() );
		return;
	end
	-- �����������ص�
	if string.find( this:GetName(),"TaskNotFinishFrame", 1, true )
	or string.find( this:GetName(),"TaskAcceptFrame", 1, true )
	or string.find( this:GetName(),"TaskAwardFrame", 1, true )
	or string.find( this:GetName(),"TaskLogFrame", 1, true )
	or string.find( this:GetName(),"ClanGiftFrame", 1, true ) then
		TaskOnEnter( this:GetName() );
		return;
	end

	-- �����������װ��
	if this:GetParent() == "EquipFrame" then
		WearformOnEnter( this:GetName() );
		return;
	end

	-- ������ڹ۲�Է�װ��
	if this:GetParent() == "WatchPlayerFrame" then
		WatchPlayerOnEnter( this:GetName() );
		return;
	end

	-- ��������ʼ�
	if this:GetParent() == "MailReadFrame" or this:GetParent() == "MailSendItemFrame" then
		MailOnEnter( this:GetName() );
		return;
	end

	if this:GetParent() == "StallSaleFrame" then
		StallSaleOnEnter( this:GetName() );
		return;
	end
	-- ������ڰ�̯������
	if this:GetParent() == "StallBuyFrame"  then
		StallBuyOnEnter( this:GetName() );
		return;
	end

	if this:GetParent() == "OnlineStoreFrame"  then
		StoreGridItemOnEnter( this:GetName() );
		return;
	end

	if string.find( this:GetName(), "SetFittingFrameRetainItem", 1, true) then
		SetFittingRetainItemOnEnter( this:GetName() );
		return;
	end

	--������ڴ��ӹ�װ�����
	if this:GetParent() == "ArmProceStarFrame" or
		this:GetParent() == "ArmProceBindFrame" or 
		this:GetParent() == "ArmProceUnBindFrame" then
		local nListType = this:GetClientUserData( 0 );
		local nGrid = this:GetClientUserData( 1 );
		if nListType >= 0 and nGrid >= 0 then
			local item = container:getItem( nListType,nGrid );
			if item:getItemId() > 0 then
				ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
			end
		else
			if this:GetParent() == "ArmProceStarFrame" then
				if this:GetName() == "ArmProceStarFrameArmItemBtn" then
					SetGameTooltips( this:GetName(), "�����Ҫǿ����װ��" );
				end
			elseif this:GetParent() == "ArmProceBindFrame" then
				SetGameTooltips( this:GetName(), "�����Ҫ�󶨵�װ��" );
			end
		end
		return;
	end

	if this:GetParent() == "PurchaseHomelandFrame" then
		local nListType	= this:GetClientUserData( 0 );
		local nGrid	= this:GetClientUserData( 1 );
		if nListType >= 0 and nGrid >= 0 then
			local item = container:getItem( nListType,nGrid );
			if item:getItemId() > 0 then
				ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
			else
				SetGameTooltips( this:GetName(), "��������" );
			end
		else
			SetGameTooltips( this:GetName(), "��������" );
		end
		return;
	end
	
	if this:GetParent() == "ArmProceStarMoveFrame" then
		ArnProceStarMoveFrameOnItemEnter( this:GetName() );
		return;
	end
	if this:GetParent() == "GemProcePickFrame" then
		local parent = getglobal( this:GetParent() );
		local itemId = parent:GetClientUserData( 0 );
		local itemDef = getItemDef( ARM_GEM_PICK_ITEM_TOLUA );
		if itemDef ~= nil then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef );
		end
		return;
	end
	if this:GetParent() == "ArmStepUpFrame" then
		ArmStepUpFrameItemOnEnter( this:GetName() );
		return;
	end
	if this:GetParent() == "ArmBaseResetFrame" then
		ArmBaseResetFrameItemOnEnter( this:GetName() );
		return;
	end

	-- �����������뷨������������Ʒ
	if this:GetParent() == "TrumpFrame" then
		ShopOnEnter( this:GetName() );
		return;
	end
	--�̳���Ʒ
	if string.find(this:GetParent(),"MarketManagerFrame_SaleItem",1,true) or this:GetParent() == "MarketBuyItemFrame" or this:GetParent() == "MarketContributeFrame" then
		MarketManagerItemOnEnter( this )		
		return;
	end

	if string.find(this:GetName(),"SecondarySkillFrameRewardItemBtn",1,true) or string.find(this:GetName(),"SecondarySkillFrameRequireItemBtn",1,true) then
		local itemId = this:GetClientUserData(0);
		if itemId >= 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,getItemDef(itemId) );
		end
	end

	--��ħ����
	if this:GetParent() == "EnchantFrame" then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end
		local container	= MainPlayer:getContainer();
		if this:GetClientUserData(0) >=0 and this:GetClientUserData(1) >=0 then
			local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) );
			if item:getItemId() ==0 then
				return;
			end
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item);
		end
		return;
	end
	--��������
	if this:GetParent() == "IdentifyFrame" then
		local MainPlayer = ActorMgr:getMainPlayer();
		if MainPlayer == nil then
			return;
		end
		local container	= MainPlayer:getContainer();
		if this:GetClientUserData(0) >=0 and this:GetClientUserData(1) >=0 then
			local item = container:getItem( this:GetClientUserData(0), this:GetClientUserData(1) );
			if item:getItemId() ==0 then
				return;
			end
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item);
		end
		return;
	end
	--[[�����̳�
	if string.find(this:GetParent(),"ContainerMarketFrameItem",1,true) then
		local nItemID = this:GetClientUserData(1);
		if nItemID > 0 then 
			local itemDef = getItemDef(nItemID);
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef)
		end
		return;
	end--]]
	--����
	if this:GetParent() == "ClanProductFrame" then
		local itemID = this:GetClientUserData( 1 );
		if itemID > 0 then
			local itemDef = getItemDef( itemID );
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef);
		end
	end
	--roll
	if string.find(this:GetParent(),"RollItemFrame",1,true) then
		local parent = getglobal( this:GetParent() );
		local nBootId = parent:GetClientUserData( 2 );
		local item = GameRollMgr:getRollItem( nBootId );
		if item ~= nil then
			ShowItemTips(  LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );
		end
	end
	--��װ�Դ�����
	if string.find(this:GetName(),"FashionPreviewBtnWear",1,true) then
		MarketManagerItemOnEnter( this )
		return;
	end
	--�����̳ǽ���
	if this:GetParent() == "ContainerBuyFrame" then
		ContainerBuyFrameItemOnEnter( this );
		return;
	end
	--���̵��̽���
	if string.find( this:GetName(), "TradeShopFrame" ) then
		local itemID = this:GetClientUserData( 0 );
		if itemID > 0 then
			local itemDef = getItemDef( itemID );
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL,LUA_ITEMTIPS_ITEMDEF,itemDef);
		end
	end
end

function ItemButton_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
	GameTooltip:Hide();
	SpellTipsFrame:Hide();
	MarketItemTipsFrame:Hide();
end

--@Desc: ��Ϊ��Ʒ�ϳ���ʱ����Ӧ
function ItemButton_Drag()
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return
	end

	local container = MainPlayer:getContainer();
	if container == nil then
		return
	end

	-- �������
	if GetCurrentCursor() == "repair" and this:GetClientUserData( 3 ) == CONTAINER_TYPE_WEAR then
		RepairOneItem(CONTAINER_TYPE_WEAR,this:GetClientID());
		return;
	end

	-- �����������뱳��
	if this:GetParent() == "PlayerContainerFrame" or this:GetParent() == "StashFrame" then
		ContainerDrag( this:GetName() );
		return;
	end

	-- �����������뱳���ֽ�
	if this:GetParent() == "PlayerContainerResolveFrame" then
		ContainerResolveDrag( this:GetName() );
		return;
	end

	-- ���������뾧ʯ����
	if this:GetParent() == "PlayerContainerSparEpurateFrame" then
		ContainerSparEpurateDrag( this:GetName() );
		return;
	end
	
	if this:GetParent() == "GemProceChargeFrame" then
		ReceiveGemProceChargeDrag( this:GetName() );
		return;
	end

	-- �����������װ��
	if this:GetParent() == "EquipFrame" then
		WearformDrag( this:GetName() );
		return;
	end

	-- ��������ʼ�
	if this:GetParent() == "MailSendItemFrame" then
		MailDrag( this:GetName() );
		return;
	end

	-- ������ڰ�̯�������
	if this:GetParent() == "StallSaleFrame" then
		StallDrag( this:GetName() );
		return;
	end

	-- ������ڽ��������
	if string.find( this:GetName(), "TradeFrame", 1, true) then
		TradeDrag( this:GetName() );
		return;
	end
	
	if string.find( this:GetName(), "SetFittingFrameRetainItem", 1, true) then
		SetFittingRetainItemDrag( this:GetName() );
		return;
	end

	--������ڴ��ӹ�װ�����
	if this:GetParent() == "ArmProceStarFrame" then
		ArmProceStarStartDrag( this:GetName() );
		return;
	end
	
	if this:GetParent() == "ArmProceBindFrame" then
		ArmProceBindStartDrag( this:GetName() );
		return;
	end

	if this:GetParent() == "ArmProceUnBindFrame" then
		ArmProceUnBindStartDrag( this:GetName() );
		return;
	end

	if this:GetParent() == "ArmProceStarMoveFrame" then
		if MessageBoxFrame:IsShown() and MessageBoxFrame:GetClientString() == "Ʒ��ת�ư���ʾ" then return end;
		ArmProceStarMoveStartDrag( this:GetName() );
		return;
	end

	if this:GetParent() == "ArmStepUpFrame" then
		ArmStepUpFrameItemOnDrag( this:GetName() );
		return;
	end
	if this:GetParent() == "ArmBaseResetFrame" then
		ArmBaseResetFrameItemOnDrag( this:GetName() );
		return;
	end
	--���λû�
	if this:GetParent() == "UnRealBackFrame" then
		UnRealBackFrameOnDrag( this:GetName() );
		return;
	end
end

--@Desc: ��Ϊ��Ʒ���յ�ʱ����Ӧ
function ItemButton_ReceiveDrag()
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

	-- ����Ǳ���
	if this:GetParent() == "PlayerContainerFrame" or this:GetParent() == "StashFrame" then
		ReceiveContainer( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end
	
	--����Ƿֽⱳ��	
	if this:GetParent() == "PlayerContainerResolveFrame" then
		ReceiveContainerResolve( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end
	--����Ǿ�ʯ����
	if this:GetParent() == "PlayerContainerSparEpurateFrame" then
		ReceiveContainerSparEpurate( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end

	--�����ʯ����
	if this:GetParent() == "GemProceChargeFrame" then
		ReceiveGemFromContainer( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end

	-- ����ǽ������
	if ( string.find( this:GetName(),"TradeFrame", 1, true ) ) then
		ReceiveTrade( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	-- �����������װ��
	if this:GetParent() == "EquipFrame" then
		ReceiveEquipWear( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	-- ��������ʼ�
	if this:GetParent() == "MailSendItemFrame" then
		ReceiveMail( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	-- ������ڰ�̯
	if this:GetParent() == "StallSaleFrame" then
		ReceiveStall( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	-- ��������̵�
	if this:GetParent() == "ShopFrame" then
		ReceiveShop( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	-- ���Ŀ��������װ���ӹ�
	if this:GetParent() == "ArmProceStarFrame" then
		ReceiveArmProceStar( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	if this:GetParent() == "ArmProceBindFrame" then
		ReceiveArmProceBind( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return
	end

	if this:GetParent() == "ArmProceUnBindFrame" then
		ReceiveArmProceUnBind( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	if this:GetParent() == "ArmProceStarMoveFrame" then
		ReceiveArmProceStarMove( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end
	--��ħ
	if this:GetParent() == "EnchantFrame" then
		EnchantReceiveDrag( this:GetName(),UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end
	
	if this:GetParent() == "RideMoveLevelFrame" then
		RideMoveReceiveDrag( this:GetName(),UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end
	--����
	if this:GetParent() == "IdentifyFrame" then 
		IdentityReceiveDrag( this:GetName(),UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
	end

	if this:GetParent() == "ArmStepUpFrame" then
		ReceiveArmStepUp( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end

	if this:GetParent() == "ArmBaseResetFrame" then
		ReceiveArmBaseReset( this:GetName(), UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end
	--���λû�
	if this:GetParent() == "UnRealBackFrame" then
		UnRealBackFrameReceiveDrag( this:GetName(),UIMSG:getUIGV().DragFrom, UIGetDragUserData1(), UIGetDragUserData2(), UIGetDragUserData3() );
		return;
	end
end

function SetFittingRetainItemDrag( szBtnName )
	local btn = getglobal( szBtnName );
	if btn:GetClientUserData(0) ~= 0 then
		UIMSG:getUIGV().DragFrom = btn:GetClientUserData(2);
		local IconTex = getglobal(szBtnName.."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), btn:GetClientUserData(3), btn:GetClientUserData(1), btn:GetClientUserData(0));
	end
end

--------------------------------------------------------------ITEMTEMPLATE��ק�ͷ�ģ��-------------------------------------------------------------------------------------------
-- ���Ŀ�������Ǳ������ֿ�
function ReceiveContainer( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn = getglobal( szBtnName );
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local srcList		= dragfrom;								 -- Դ��������
	local desList		= GetCurContainerType( btn:GetParent(),btn:GetName() ); -- ��ȡ��ǰ�һ�����Ʒ�ı�������;
	local srcGrid		= dragdata1;							 -- ��Դ�����ĸ����ӹ���
	local desGrid		= btn:GetClientID()-1;					 -- Ŀ�����

	if srcList == CONTAINER_TYPE_TASK and desList == CONTAINER_TYPE_STASH then
		ShowMidTips("������Ʒ�����ɴ���ֿ�");
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if srcList == CONTAINER_TYPE_SUNDRIES and desList == CONTAINER_TYPE_STASH then
		local srcItem = container:getItem( srcList, srcGrid );
		if srcItem:getItemId() > 0 then
			local srcItemDef = srcItem:getItemDef()
			if CheckItemResult( srcItemDef, RESULT_LOST_TOTEM ) and srcItem:getLostTotemState() == 1 then
				ShowMidTips( "ʧ��ͼ�ڴ��ڼ���״̬�����ɴ���ֿ�" )
				return				
			end
		end
	end

	if string.find( szBtnName,"PlayerContainerFrameBagItem" ) or string.find( szBtnName,"StashFrameBagItem" ) then
		local BagBackTexture = getglobal( szBtnName.."BagBackTexture" );
		if BagBackTexture:IsShown() then
			return;
		end
	end
	
	if ( srcList == CONTAINER_TYPE_STUFF
		or srcList == CONTAINER_TYPE_TASK
		or srcList == CONTAINER_TYPE_SUNDRIES
		or srcList == CONTAINER_TYPE_STASH
		or srcList == CONTAINER_TYPE_EXTEND
		or srcList == CONTAINER_TYPE_EXTEND_STASH ) then
		--TODO HERE 
		

		if IsContainerItemLock( desList, desGrid )or IsContainerItemLock( srcList, srcGrid ) or ( ( srcList == desList ) and ( srcGrid == desGrid ) ) then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		if ( srcList == CONTAINER_TYPE_SUNDRIES and desList == CONTAINER_TYPE_SUNDRIES ) or
			( srcList == CONTAINER_TYPE_STASH and desList == CONTAINER_TYPE_STASH ) or
			( srcList == CONTAINER_TYPE_STASH and desList == CONTAINER_TYPE_SUNDRIES ) or
			( srcList == CONTAINER_TYPE_SUNDRIES and desList == CONTAINER_TYPE_STASH ) 
		 then
			local srcItem = container:getItem( srcList, srcGrid );
			local desItem = container:getItem( desList, desGrid );
			if srcItem:getItemId() > 0 and desItem:getItemId() > 0 then
				local srcItemDef = srcItem:getItemDef();
				local desItemDef = desItem:getItemDef();
				local bGemFit = compareItemGemValue( srcItem, desItem );   -- ��ʯ ��Ҫ��δ���ܱ�ʯ
				if bGemFit and srcItemDef.ItemID == desItemDef.ItemID and not srcItem:isItemBind() and desItem:isItemBind() then
					if  desItem:getNum() < desItem:getItemDef().MaxPile then
						UIEndDrag();
						UIMSG:getUIGV().DragFrom = DRAG_NONE;
						MessageBox( "��ʾ", "���Ӻ���߽��ᱻ�󶨣��Ƿ�ȷ�ϵ��ӣ�" );
						MessageBoxFrame:SetClientString( "��������ʾ" );
						MessageBoxFrame:SetClientUserData( 0, srcList );
						MessageBoxFrame:SetClientUserData( 1, srcGrid );
						MessageBoxFrame:SetClientUserData( 2, desList );
						MessageBoxFrame:SetClientUserData( 3, desGrid );
						return;
					end
				end
			end
		end
		local item = container:getItem( srcList, srcGrid );
		local desBtn = getglobal( "PlayerContainerFrameItem"..(srcGrid+1) );
		btn:SwapCooldownMembers(desBtn);
		container:send_MoveItem( srcList, desList, dragdata1, item:getNum(), desGrid );
		local t_getItemSourceControl = GetItemSourceControl();
		if srcList == CONTAINER_TYPE_SUNDRIES and srcList == desList and t_getItemSourceControl:getBlinkItemGrid() == ( dragdata1 + 1 ) then
			HideContainerEffectFrame();
			t_getItemSourceControl:setBlinkItemGrid( { grid = -1 } );
		end

		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList	== DRAG_CONTAINERRESOLVE then
		ContainerRemoveResolveItem( dragdata2 );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList	== DRAG_SPAR_EPURATE then
		ContainerRemoveSparEpurateItem( dragdata2 );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_GEMCHARGE then
		RemoveGemAfterDrag( dragdata2 );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	-- ��קĿ�������ʼ�
	if srcList == DRAG_MAIL then
		local senddata	= GameMailManager:getSendData();
		local item		= senddata.m_Items[dragdata1];
		if item:getItemId() == 0 then
			return;
		end

		if IsContainerItemLock( desList, desGrid ) and  item:getGridIdx() ~= desGrid then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		if item:getGridIdx() ~= desGrid then
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end

		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );	-- ����
		GameMailManager:delSendItem( dragdata1 );	-- �Żر������Ƴ��ʼ��Ĵ�����Ʒ�еĵ�ǰ��Ʒ
		MailSendItemUpdate();

		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	-- ��קĿ�����԰�̯
	if srcList == DRAG_STALL then
		local stallitem		= GameStallManager:getSelfStallItem();
		local item			= stallitem.m_ItemData[dragdata1];
		local stallsrcList	= item:getListType();
		local stallsrcGrid	= item:getGridIdx();
		if IsContainerItemLock( desList, desGrid ) and  stallsrcGrid ~= desGrid then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		if stallsrcGrid ~= desGrid then
			container:send_MoveItem( stallsrcList, desList, stallsrcGrid, item:getNum(), desGrid );	-- �ڱ����������ƶ���Ʒ
		end
		UnLockItem( stallsrcList, stallsrcGrid, item:getItemId() );								-- ����
		GameStallManager:delStallItem( dragdata1 );												-- �Ӱ�̯������Ʒ���Ƴ�

		UpdateStallSaleFrame();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	-- ��קĿ�����Խ�����
	if srcList == DRAG_TRADE then
		--�����Խ���������Ʒ��������1��, ������ִ���
		-- TODO: Ϊ�Ժ���Ԥ��
		--[[local tradeItem = GameExchange:getExchangItem( 0, srcGrid );
		if item:getNum() > 1 then
			SplitItem( {
						szFrameName = "PlayerContainerFrame", nMinNum = 0, nMaxNum = item:getNum(),
						srcGrid = srcGrid, szFromFrameName = "TradeFrame"
						} );
		else

		end
		--]]

		local item	= GameExchange:getExchangItem( 0, srcGrid );
		if IsContainerItemLock( desList, desGrid ) and item:getGridIdx() ~= desGrid then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		if item:getGridIdx() ~= desGrid then
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end
		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--����
		GameExchange:delExchangeItem( srcGrid );
		-- ���Լ�������������״̬��������������
		if GameExchange:isSrcLockExchange() then
			GameExchange:cancelLock();
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if srcList == DRAG_PETOPERATION then
		local nItemId = srcGrid;
		local item = container:getItem( nItemId );			
		if IsContainerItemLock( desList, desGrid ) and item:getGridIdx() ~= desGrid then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--����
		if item:getGridIdx() ~= desGrid then
			container:send_MoveItem( desList, desList, item:getGridIdx(), item:getNum(), desGrid );
		end

		-- ��ճ���������ĵ���
		ClearPet_OperationItem();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	-- ��קĿ�����Գ�������װ��
	if srcList == DRAG_PETARM then
		local PetArmId		= dragdata1;
		local nHightId		= dragdata2;
		local nLowId		= dragdata3;
					
		if IsContainerItemLock( desList, desGrid ) then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		local GamePet		= MainPlayer:getPet();
		if GamePet == nil then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		GamePet:petUnWearArm(PetArmId,desList,desGrid,nHightId,nLowId);
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	-- ��קĿ�������������װ��
	if srcList == DRAG_WEARFORM then
		local ItemId	= dragdata1;
		local wearPos	= dragdata2;
		local Eqiup		= MainPlayer:getEquip();
		if IsContainerItemLock( desList, desGrid ) then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		Eqiup:send_unEquip( ItemId, desList, desGrid, wearPos );

		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if srcList ==  DRAG_ARMPROCE then
		local item = container:getItem( dragdata1, dragdata2 );
		if item ~= nil and item:getItemType() == ITEM_SLOTTYPE_AMM then
			ArmProceFrameOnRightClick( "ArmProceStarFrameArmItemBtn", desList, desGrid  );
		else
			ArmProceFrameOnRightClick( "ArmProceStarFrameNeedItemBtn" );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_ARMBIND then
		ArmProceFrameOnRightClick( "ArmProceBindFrameArmItemBtn" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_ARMUNBIND then
		ArmProceFrameOnRightClick( "ArmProceUnBindFrameArmItemBtn" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_ARMSTARMOVE then
		if dragdata3 == 1 then
			ArmProceStarMoveFrameOnRightClick( "ArmProceStarMoveFrameSrcItemBtn", desList, desGrid  );
		elseif dragdata3 == 2 then
			ArmProceStarMoveFrameOnRightClick( "ArmProceStarMoveFrameDestItemBtn", desList, desGrid  );
		elseif dragdata3 == 3 then
			ArmProceStarMoveFrameOnRightClick( "ArmProceStarMoveFrameStuffItemBtn" );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_ARM_RESOLVE then
		if dragdata3 == 1 then
			ArmRecastFrameItemBtnClearBtn( "ArmRecastFrameArmItemBtn", desList, desGrid );
		elseif dragdata3 == 2 then
			ArmRecastFrameItemBtnClearBtn( "ArmRecastFrameNeedItemBtn", desList, desGrid );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_ARM_WASH then
		if dragdata3 == 1 then
			ArmWashFrameItemBtnClearBtn( "ArmWashFrameArmItemBtn", desList, desGrid );
		elseif dragdata3 == 2 then
			ArmWashFrameItemBtnClearBtn( "ArmWashFrameStuffItemBtn", desList, desGrid );
		elseif dragdata3 == 3 then
			ArmAdvWashFrameItemBtnClearBtn( "ArmAdvWashFrameArmItemBtn", desList, desGrid );
		elseif dragdata3 == 4 then
			ArmAdvWashFrameItemBtnClearBtn( "ArmAdvWashFrameStuffItemBtn", desList, desGrid );
		end
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_SPAR_SORB then
		SparSorbFrameItemBtnClearBtn( dragdata3, desList, desGrid );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == CONTAINER_TYPE_SPAR then
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		if MainPlayer:isInFight() then
			ShowMidTips("ս�����޷����иò���")
			return
		end
		GameArmProce:requestSparInlay( desList, desGrid, srcList, dragdata2 )
		return;
	end
	if srcList == DRAG_ARM_ADVRESOLVE then
		ArmAdvRecastFrameItemBtnClearBtn( desList, desGrid );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_PET_AUTO_ITEM then
		PetAutoRecoverFrame_ItemGridBtn_ClearBtn( dragdata3 );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
	if srcList == DRAG_SKILL_RUNE_COMBINE then
		RemoveSkillRuneCombineItem( dragdata1 );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if srcList == DRAG_ARM_ATTR_MOVE_SRC then
		ClearArmAttrMoveBtn( "SrcBtn", desList, desGrid )
	elseif srcList == DRAG_ARM_ATTR_MOVE_DST then
		ClearArmAttrMoveBtn( "DstBtn", desList, desGrid )
	elseif srcList == DRAG_ARM_ATTR_MOVE_USE then
		ClearArmAttrMoveBtn( "UseBtn", desList, desGrid )
	elseif srcList == DRAG_ARM_ATTR_RESET then
		ClearArmAttrResetBtn( dragdata1, dragdata2, dragdata3, desList, desGrid )
	end

	if srcList == DRAG_ARM_STEP_UP then
		local frame = "ArmStepUpFrame";
		if dragdata3 > 0 then
			frame = frame .. "ItemBtn" .. dragdata3;
		else
			frame = frame .. "ArmItemBtn";
		end
		ClearArmStepUpBtn( frame, desList, desGrid )
		return;
	elseif srcList == DRAG_ARM_BASE_RESET then
		local frame = "ArmBaseResetFrame";
		if dragdata3 > 0 then
			frame = frame .. "ItemBtn" .. dragdata3;
		else
			frame = frame .. "ArmItemBtn";
		end
		ClearArmBaseResetBtn( frame, desList, desGrid );
		return;
	elseif srcList == DRAG_ARM_SKILL_ACTIVE then
		ClearArmSkillActiveBtn( "ArmSkillActiveFrameArmItemBtn", desList, desGrid );
		return;
	end
end

function compareItemGemValue( srcItem, desItem )
	local srcLvValue = srcItem:getGemStart();
	local desLvValue = desItem:getGemStart();
	local srcValue	= srcItem:getGemPower();
	local desValue	= desItem:getGemPower();
	if srcLvValue > 0 or desLvValue > 0 or srcValue > 0 or desValue > 0 then  
		return false ;
	else 
		return true;
	end
end
--�ǰ󶨵�����Ʒ�ĵ���
function ContainerUnbindAddToBind( srcList, srcGrid, desList, desGrid )
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();
	local srcItem = container:getItem( srcList, srcGrid );
	local desItem = container:getItem( desList, desGrid );
	if srcItem:getItemId() ~= desItem:getItemId() then return end
	if srcItem:getItemId() == nil or desItem:getItemId() == nil then
		return;
	else
		container:send_MoveItem( srcList, desList, srcGrid, srcItem:getNum(), desGrid );
	end
end
--���Ŀ�������ǽ�����
function ReceiveTrade( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn = getglobal( szBtnName );
	-- ��Ϊ�������жԷ�����Ʒ��,���ܷ����Լ�����Ʒ
	if btn:GetClientUserData( 2 ) == 1 then
		local parentBtn = getglobal( btn:GetParent() );
		btn = getglobal( "TradeFrame_SelfItem"..parentBtn:GetClientID().."ItemIconBtn" );
	end

	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local srcList		= dragfrom;					-- Դ��������
	local srcGrid		= dragdata1;				-- ��Դ�����ĸ����ӹ���
	local desList		= btn:GetClientUserData(0);	-- Ŀ����������
	local desGrid		= btn:GetClientUserData(1);	-- Ŀ�����
	-- ���Ŀ�����Ա���
	if srcList == CONTAINER_TYPE_SUNDRIES
	or srcList == CONTAINER_TYPE_TASK
	or srcList == CONTAINER_TYPE_STUFF then
		local item		= container:getItem( srcList, srcGrid );
		local itemdef	= item:getItemDef();
		
		local nError = CanExChange( item );
		if nError ~= 0 then
			ShowMidTips( GetExChangeErrorMsg( nError, "exchange" ) );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		if GameExchange:isSrcLockExchange() then
			GameExchange:cancelLock();
		end

		-- ������ҷ����Ʒ��������1,������ִ���
		-- ����Ͻ�������Ʒ�����Ƿ����1,������1�򵯳���ִ���
		--[[if item:getNum() > 1 then
			SplitItem( {
						szFrameName = "TradeFrame", nMinNum = 0, nMaxNum = item:getNum(),
						desGrid = desGrid, srcList = srcList, srcGrid = srcGrid
						} );
		else
			GameExchange:addExchangeItem( desGrid, srcList, srcGrid, 1 );
		end--]]
		-- �ڶ�Ӧ������λ�Ѿ�����Ʒ������£���ȥ��ԭ������Ʒ
		local oldItem = GameExchange:getExchangItem( 0, desGrid );
		if oldItem:getItemId() ~= 0 then
			UnLockItem( oldItem:getListType(), oldItem:getGridIdx(), oldItem:getItemId(), "TradeFrame" );
			GameExchange:delExchangeItem( desGrid );
		end

		GameExchange:addExchangeItem( desGrid, srcList, srcGrid, item:getNum() );
		LockItem( srcList, srcGrid, item:getItemId(), "TradeFrame" ); -- �����Ʒ��ӳɹ�����ס����Ʒ
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

-- ���Ŀ��������װ�����
function ReceiveEquipWear( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn = getglobal( szBtnName );
	local srcList		= dragfrom;					-- Դ��������
	local srcGrid		= dragdata1;				-- ��Դ�����ĸ����ӹ���
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();

	if srcList == CONTAINER_TYPE_SUNDRIES then
		local item = container:getItem( srcList, srcGrid );
		if getItemType( item:getItemId() ) == ITEM_SLOTTYPE_AMM then
			local itemDef = item:getItemDef();
			if itemDef ~= nil then
				if itemDef.WearPos1 == btn:GetClientID() then
					local nWearPos = itemDef.WearPos1;
					if IsFashionItem( itemDef ) then 
						nWearPos = GetFashionSuitPos( itemDef.WearPos1 );
					end
					if itemDef ~= nil and not item:isItemBind() and itemDef.ArmBind == ITEM_WEAR_BIND then
						MessageBoxFrame:SetClientUserData( 0, itemDef.ItemID );
						MessageBoxFrame:SetClientUserData( 1, nWearPos );
						MessageBoxFrame:SetClientUserData( 2, srcList);
						MessageBoxFrame:SetClientUserData( 3, srcGrid);
						if GetArmWearLv( itemDef, item ) > MainPlayer:getLv() then
							ShowMidTips("��ǰ�ȼ�����װ��");
							UIEndDrag();
							UIMSG:getUIGV().DragFrom = DRAG_NONE;
							return;
						end
						if itemDef.WearPre.GeniusLimit > 0 and GeniusMgr:getGeniusTypeValue(0) < itemDef.WearPre.GeniusLimit then
							ShowMidTips("��ǰ�����츳ֵ���㣬����װ��");
							UIEndDrag();
							UIMSG:getUIGV().DragFrom = DRAG_NONE;
							return;
						end
						local race = MainPlayer:getRace()
						local must = itemDef.WearPre.Career
						if must ~= race and must ~= race % 0x10 and must ~= 0  then
							ShowMidTips("ְҵ����������װ��");
							UIEndDrag();
							UIMSG:getUIGV().DragFrom = DRAG_NONE;
							return;
						end
						MessageBox("��װ��","�Ƿ�װ��"..itemDef.Name.."?(װ�����)");
					else
						container:usedItem( srcList, srcGrid, nWearPos );
					end
				elseif itemDef.WearPos1 == 11 and btn:GetClientID() >= 12 and btn:GetClientID() <= 13 then
					if( item:isRing() ) then
						if not item:isItemBind() and itemDef.ArmBind == ITEM_WEAR_BIND then
							MessageBoxFrame:SetClientUserData( 0, itemDef.ItemID );
							MessageBoxFrame:SetClientUserData( 1, btn:GetClientID() );
							MessageBoxFrame:SetClientUserData( 2, srcList);
							MessageBoxFrame:SetClientUserData( 3, srcGrid);
							if GetArmWearLv( itemDef, item ) > MainPlayer:getLv() then
								ShowMidTips("��ǰ�ȼ�����װ��");
								UIEndDrag();
								UIMSG:getUIGV().DragFrom = DRAG_NONE;
								return;
							end
							if itemDef.WearPre.GeniusLimit > 0 and GeniusMgr:getGeniusTypeValue(0) < itemDef.WearPre.GeniusLimit then
								ShowMidTips("��ǰ�����츳ֵ���㣬����װ��");
								UIEndDrag();
								UIMSG:getUIGV().DragFrom = DRAG_NONE;
								return;
							end
							local race = MainPlayer:getRace()
							local must = itemDef.WearPre.Career
							if must ~= race and must ~= race % 0x10 and must ~= 0  then
								ShowMidTips("ְҵ����������װ��");
								UIEndDrag();
								UIMSG:getUIGV().DragFrom = DRAG_NONE;
								return;
							end
							
							MessageBox("��װ��","�Ƿ�װ��"..itemDef.Name.."?(װ�����)");
						else
							container:usedItem( srcList, srcGrid, btn:GetClientID() );
						end
					end
				elseif itemDef.WearPos1 == 10 and btn:GetClientID() >= 20 and btn:GetClientID() <= 21 then
					if( item:isAmuleft() ) then
						if not item:isItemBind() and itemDef.ArmBind == ITEM_WEAR_BIND then
							MessageBoxFrame:SetClientUserData( 0, itemDef.ItemID );
							MessageBoxFrame:SetClientUserData( 1, btn:GetClientID() );
							MessageBoxFrame:SetClientUserData( 2, srcList);
							MessageBoxFrame:SetClientUserData( 3, srcGrid);
							if GetArmWearLv( itemDef, item ) > MainPlayer:getLv() then
								ShowMidTips("��ǰ�ȼ�����װ��");
								UIEndDrag();
								UIMSG:getUIGV().DragFrom = DRAG_NONE;
								return;
							end
							if itemDef.WearPre.GeniusLimit > 0 and GeniusMgr:getGeniusTypeValue(0) < itemDef.WearPre.GeniusLimit then
								ShowMidTips("��ǰ�����츳ֵ���㣬����װ��");
								return;
							end
							local race = MainPlayer:getRace()
							local must = itemDef.WearPre.Career
							if must ~= race and must ~= race % 0x10 and must ~= 0  then
								ShowMidTips("ְҵ����������װ��");
								UIEndDrag();
								UIMSG:getUIGV().DragFrom = DRAG_NONE;
								return;
							end
							MessageBox("��װ��","�Ƿ�װ��"..itemDef.Name.."?(װ�����)");
						else
							container:usedItem( srcList, srcGrid, btn:GetClientID() );
						end
					end
				else
					ShowMidTips("���Ƕ�Ӧ��װ��λ��");
				end
			end
		end
	end
	UIEndDrag();
	UIMSG:getUIGV().DragFrom = DRAG_NONE;
	return;
end

-- ���Ŀ���������ʼ�
function ReceiveMail( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn		= getglobal( szBtnName );
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local srcList		= dragfrom;				-- Դ��������
	local srcGrid		= dragdata1;				-- ��Դ�����ĸ����ӹ���
	local desGrid		= btn:GetClientID()-1;			-- Ŀ�����
	local senddata		= GameMailManager:getSendData();
	local mailitem		= senddata.m_Items[desGrid];

	-- ���Ŀ�������ʼ�
	if dragfrom == DRAG_MAIL then
		--GameMailManager:addSendItem( senddata.m_Items[dragdata1], (desGrid), dragdata2 );
		--GameMailManager:delSendItem( dragdata1 );

		-- ��Ʒ����ȥ��ˢ�´�����Ʒ�б�
		MailSendItemUpdate();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	if dragfrom == DRAG_WEARFORM then
		ShowMidTips( "������Ʒ�����ʼ�" );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end

	-- ���Ŀ�����Ա���
	if srcList == CONTAINER_TYPE_SUNDRIES
	or srcList == CONTAINER_TYPE_TASK
	or srcList == CONTAINER_TYPE_STUFF then
		local lockState = container:getContainerLockState();
		if lockState == CONTAINER_LOCK_STATE_LOCK then
			ShowMidTips("������֤������ȫ��");
			return;
		end
		local item = container:getItem( srcList, srcGrid );
		local itemdef = item:getItemDef();
		-- NoDealֵΪ1��ʱ�򣬱�ʾ�����������֮�佻��
		local nError = CanExChange(item);
		if nError ~= 0 then
			ShowMidTips(GetExChangeErrorMsg(nError,"mail"));
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		-- ������ҷ����Ʒ��������1,������ִ���
		-- ����Ͻ�������Ʒ�����Ƿ����1,������1�򵯳���ִ���
		if item:getNum() > 1 then
			SplitItem( {
						szFrameName = "MailSendItemFrame", nMinNum = 0, nMaxNum = item:getNum(),
						desGrid = desGrid, srcList = srcList, srcGrid = srcGrid
						} );
		else

			if mailitem:getItemId() > 0 then
				UnLockItem( mailitem:getListType(), mailitem:getGridIdx(), mailitem:getItemId() );	-- ����
				GameMailManager:delSendItem( desGrid );	-- �Żر������Ƴ��ʼ��Ĵ�����Ʒ�еĵ�ǰ��Ʒ
			end
			GameMailManager:addSendItem( item , (desGrid), 1 );
			LockItem( srcList, srcGrid, item:getItemId(), "MailSendItemFrame" ); -- �����Ʒ��ӳɹ�����ס����Ʒ
		end
		
		-- ��Ʒ����ȥ��ˢ�´�����Ʒ�б�
		MailSendItemUpdate();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
end

-- ���Ŀ�������ǰ�̯
function ReceiveStall( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn		= getglobal( szBtnName );
	local srcList	= dragfrom;					-- Դ��������
	local srcGrid	= dragdata1;				-- ��Դ�����ĸ����ӹ���
	local desList	= btn:GetClientUserData(0);	-- Ŀ����������
	local desGrid	= btn:GetClientID()-1;		-- Ŀ�����
	local stalldata = GameStallManager:getSelfStallItem();
	local stallitem = stalldata.m_ItemData[desGrid];

	if srcList == DRAG_STALL then
		local mainplayer = ActorMgr:getMainPlayer();
		if mainplayer:isInStall() then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
		if desGrid == srcGrid or stallitem:getItemId() > 0 then
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		else
			local stallitem		= GameStallManager:getSelfStallItem();
			local item			= stallitem.m_ItemData[dragdata1];
			srcList				= item:getListType();
			srcGrid				= item:getGridIdx();
			GameStallManager:addStallItem( btn:GetClientUserData(1), srcList, srcGrid, item:getNum(), item:getStallSellPrice());
			GameStallManager:delStallItem( dragdata1 );
			RemoveCheckItem( "StallSaleFrame" );
			local StallData = GameStallManager:getSelfStallItem();
			UpdateStallSaleFrame();
			local IconTex = getglobal( btn:GetName().."IconTexture" );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end
	end

	-- ���Ŀ�����Ա���
	if srcList == CONTAINER_TYPE_SUNDRIES
	or srcList == CONTAINER_TYPE_TASK
	or srcList == CONTAINER_TYPE_STUFF then
		local MainPlayer	= ActorMgr:getMainPlayer();
		local container		= MainPlayer:getContainer();
		local item			= container:getItem( srcList, srcGrid );
		local itemdef		= item:getItemDef();

		local nError = CanExChange( item );
		if nError ~= 0 then
			ShowMidTips( GetExChangeErrorMsg( nError, "stall" ) );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		-- �Ӳֿ��ϳ��Ļ���Ŀ����λ�Ѿ�����Ʒ�Ĳ��÷�
		if dragfrom == CONTAINER_TYPE_STASH or stallitem:getItemId() > 0 then
			Sale2Stall( item, srcList, srcGrid );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		-- ���ڵİ�̯�е���Ʒ���÷�
		--[[
		if MainPlayer:isInStall() then
			-- TODO: ADD
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			--AddGameMiddleTips( GetMidTipsList()[MID_STALL_CANSTALLSTATE] );
			return;
		end
		--]]

		-- ��̯λ�Ѱ�����Ʒ�����ܽ���Ʒ�ŵ�̯λ��
		if GameStallManager:isStallItemListFull() then
			ShowMidTips( "̯λ�Ѱ�����Ʒ" );
			UIEndDrag();
			UIMSG:getUIGV().DragFrom = DRAG_NONE;
			return;
		end

		local t_StallSellerShangJiaItemFrameControl = GetStallSellerShangJiaItemFrameControl();
		if StallSellerShangJiaItemFrame:IsShown() then
			ReleaseStallSellerShangJiaItemFrame_LockItem();
		end
		t_StallSellerShangJiaItemFrameControl:setItemInfo( { ["srcList"] = srcList, ["srcGrid"] = srcGrid, ["destGrid"] = desGrid } );
		StallSellerShangJiaItemFrame:Show();
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
		return;
	end
end

-- ���Ŀ���������̵�
function ReceiveShop( szBtnName, dragfrom, dragdata1, dragdata2, dragdata3 )
	local btn		= getglobal( szBtnName );
	local srcList	= dragfrom;					-- Դ��������
	local srcGrid	= dragdata1;				-- ��Դ�����ĸ����ӹ���

	-- ���Ŀ�����Ա���
	if srcList == CONTAINER_TYPE_SUNDRIES
	or srcList == CONTAINER_TYPE_TASK
	or srcList == CONTAINER_TYPE_STUFF then
		local MainPlayer	= ActorMgr:getMainPlayer();
		local container		= MainPlayer:getContainer();
		local item			= container:getItem( srcList, srcGrid );

		Sale2Shop( item, item:getListType(), item:getGridIdx() );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

--------------------------------------------------------------ITEMTEMPLATE����Ҽ����ģ��-------------------------------------------------------------------------------------------
-- ���ǶԽ�����Ʒ�Ҽ�
function TradeRightBtnClick( szBtnName )
	local btn	= getglobal( szBtnName );
	-- ȡ�½�����Ʒ
	-- ���ǶԷ�����Ʒ����������
	if btn:GetClientUserData( 2 ) == 1 then
		return;
	end
	-- �����Լ�����Ʒ��,��û���κζ���
	local parentBtn = getglobal( btn:GetParent() );
	local item =  GameExchange:getExchangItem( 0, parentBtn:GetClientID() - 1 );
	if	item:getItemId() == 0 then
		return;
	end

	if GameExchange:isSrcLockExchange() then
		GameExchange:cancelLock();
	end

	UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--����
	GameExchange:delExchangeItem( btn:GetClientUserData( 1 ) );
end

-- �Ҽ���װ��
function WearformRigthBtnClick( szBtnName )
	local btn				= getglobal( szBtnName );
	local Mainplayer		= ActorMgr:getMainPlayer();
	local Eqiup				= Mainplayer:getEquip();
	local grid 				= GetFashionSuitPos( btn:GetClientID()  );	
	local EquipItem			= Eqiup:getItem( grid  );
	if EquipItem:getItemId() == 0 then
		return;
	end
	
	local itemDef = EquipItem:getItemDef();
	if ( isKeyPressed(160) or isKeyPressed(161) ) and itemDef.Important >= 1 and not IsFashionItem( itemDef ) then
		if GetItemTypeName( math.floor(EquipItem:getItemId() / 10000 ) ) == "ʱװ" then
			ShowMidTips( "ʱװ������Ƕ" );
			return;
		end
		if EquipItem:isArmFengYin() then
			ShowMidTips( "װ����ӡ���޷���Ƕ" );
			return;
		end
		if GetItemTypeName( math.floor(EquipItem:getItemId() / 10000 ) ) == "����" then
			ShowMidTips( "����װ��������Ƕ" );
			return;
		end
		local t_BaoShiXiangQianFrameControl = GetBaoShiXiangQianFrameControl();
		t_BaoShiXiangQianFrameControl:startBaoShiXiangQian( { ["grid"] = grid, list = CONTAINER_TYPE_WEAR, lockBtn = szBtnName } );
		return;
	end

	local xinShouBtn	= getglobal( "EquipFrameEffectFrame" );
	xinShouBtn:Hide();
	
	
	local container		= Mainplayer:getContainer();
	local nMaxNum		= container:getContainerMaxNum( CONTAINER_TYPE_SUNDRIES );
	if EquipItem:getItemId() ~= 0 then
		local nonegrid = container:getNoneGridIndex(CONTAINER_TYPE_SUNDRIES);
		Eqiup:send_unEquip( EquipItem:getItemId(), CONTAINER_TYPE_SUNDRIES, nonegrid, grid );
		UIEndDrag();
		UIMSG:getUIGV().DragFrom = DRAG_NONE;
	end
end

function CanUseItem( itemDef, bShowMsg, item )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return false;
	end
	
	local t_cannotUse= 
	{ 
			resultID	=	
							{	
								--RESULT_PET_ADD_LOVEVAL,			-- ���ӳ����ҳ϶�
								RESULT_PET_WASH_GENIUS ,		-- ����ϴ�����
								RESULT_PET_ADD_ATTR ,			-- ��ǿ�������Ե���
								RESULT_PET_STRENGTHEN_STUFF ,	-- ����ǿ������
								RESULT_PET_STRENGTHEN_SAFE ,	-- ����ǿ������
								--RESULT_PET_SKILL_BOOK ,			-- ����ѧϰ������
								RESULT_ADD_SKILL_GRID ,			-- �������Ӽ��ܸ��ӵĵ���
								RESULT_PET_WASH_SKILL_GRID ,	-- ����ϴ���ܸ��ӵ���
							} ,
			subType		= 
							{ 
								--ITEM_PETXUNFU_ID, 
							},

		cannotUseDesc	=	"�˵��߲���ͨ������Ҽ�ʹ��"
	};

	-- �ж������Ƿ��ܹ��Ҽ�ʹ��
	for _, resultID in ipairs( t_cannotUse.resultID ) do
		for i = 1, MAX_ITEM_RESULT do
			if itemDef.Result[i-1].ResultID == resultID then
				return false;
			end
		end
	end

	for _, subType in ipairs( t_cannotUse.subType ) do
		if math.floor(itemDef.ItemID / 1000) == subType then
			return false;
		end
	end
	
	-- �ж��Ƿ��Ǹ����ﲹѪ��ҩƷ�����ǣ��鿴�Ƿ��г����ڳ�������û������ʹ�ø�ҩƷ
	local nTargetIndex = -1;
	if itemDef.UseTarget == USE_TARGET_PET then
		local petPackage = MainPlayer:getPet();
		for i = 1, petPackage:getPetNum() do
			if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
				nTargetIndex = i - 1;
				break;
			end
		end
		if CheckItemResult( itemDef,RESULT_PET_QIANLI ) and nTargetIndex == -1 then 
			ShowMidTips( "����Ʒֻ�ܶԳ����ĳ���ʹ�ã������ٻ�һֻ�����ս��" );
			return false
		end
		if nTargetIndex == - 1 then
			if bShowMsg then
			--	ShowMidTips( "����Ʒֻ�ܶԳ����ĳ���ʹ�ã�����û���κγ����ĳ���" );
			--	ShowMidTips( "�޳��ﱻ�ٻ��������ٻ�һֻ����" );
			end
			return false;
		end
	end
	--[[
	if not CanSatisfyDesignation( itemDef )  then
		ShowMidTips("����δ�ﵽҪ��");
		return false;
	end
	--]]
	
	if itemDef.UseTarget == USE_TARGET_PET then
		local petPackage = MainPlayer:getPet();
		for i = 1, petPackage:getPetNum() do			
			if petPackage:isPetInState( i - 1, CLIENT_PET_STAT_FIGHT ) then
				local petInfo = petPackage:getPetInfo( i - 1 );
				if itemDef.WearPre.RoleLevelMin > petInfo.Level or 
				( itemDef.WearPre.RoleLevelMax ~= 0 and itemDef.WearPre.RoleLevelMax < petInfo.Level ) then
					if bShowMsg then
						ShowMidTips("��ǰ����ȼ�����ʹ�ø���Ʒ");
					end
					return false;
				end
			end
		end
	else
		if GetArmWearLv( itemDef, item ) > MainPlayer:getLv() or 
			( itemDef.WearPre.RoleLevelMax ~= 0 and itemDef.WearPre.RoleLevelMax < MainPlayer:getLv() ) then
			if getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM then
				ShowMidTips("��ǰ�ȼ�����װ��");
			else
				ShowMidTips("��ǰ�ȼ�����ʹ�ø���Ʒ");
			end
			return false;
		end

		if itemDef.WearPre.GeniusLimit > 0 and GeniusMgr:getGeniusTypeValue(0) < itemDef.WearPre.GeniusLimit then
			ShowMidTips("��ǰ�����츳ֵ���㣬����װ��");
			return false;
		end
	end
	local race = MainPlayer:getRace()
	local must = itemDef.WearPre.Career
	if must ~= race and must ~= race % 0x10 and must ~= 0  then
		if getItemType( itemDef.ItemID ) == ITEM_SLOTTYPE_AMM then
			ShowMidTips("ְҵ����������װ��");
		else
			ShowMidTips("ְҵ����������ʹ�ø���Ʒ");
		end
		return false;
	end

	local t_trackRoleControl = GetTrackRoleControl();
	if t_trackRoleControl:isZhuiZongItem( { id = itemDef.ItemID } ) then
		ShowMidTips("��ͨ����������׷�ٹ���ʹ��");
		return false;
	end
	
	local firstResult = itemDef.Result[0];
	if firstResult.ResultID == RESULT_PET_HATCH then
		local petDef = getPetDef( firstResult.ResultVal1 );
		if MainPlayer:getLv() < petDef.FightLevel then
			ShowMidTips("���ĵȼ��������ٻ��ó�����ս��");
			return false;
		end
	end

	return true;
end

-- ������ڱ����Ҽ����
local t_containerLockEvent = 
{
["BaoShiHeChengFrame"] = 
{
["func"] = t_rightReceiveFrames["BaoShiHeChengFrame"]["func"],
}
};

function GetHighestDrawLevelProcessFrame( data )
	local curFrameDrawLevel = 0;
	local btn = data["btn"];

	local name;
	for i = 1, table.getn( t_ItemFirstCallBackFun ) do
		local frame = getglobal( t_ItemFirstCallBackFun[i][1] );
		if frame:IsShown()
			and t_ItemFirstCallBackFun[i][2] ~= nil
			and frame:GetName() ~= btn:GetParent()
			and curFrameDrawLevel < frame:getFrameDrawLevel() then
				curFrameDrawLevel = frame:getFrameDrawLevel();
				name = frame:GetName();
		end
	end
	
	return name;
end

function ContainerRightBtnClick( szBtnName )
	local btn				= getglobal( szBtnName );
	local nContainerType	= GetCurContainerType( btn:GetParent(),szBtnName ); -- ��ȡ��ǰ�һ�����Ʒ�ı�������
	local MainPlayer		= ActorMgr:getMainPlayer();
	local container			= MainPlayer:getContainer();
	local Eqiup				= MainPlayer:getEquip();
	
	local used_Item	= container:getItem( nContainerType, btn:GetClientID()-1 );
	if used_Item:getItemId() == 0 then
		if string.find(szBtnName,"PlayerContainerFrameBagItem",1,true ) then
			local tex = getglobal( szBtnName .. "BagBackTexture" );
			if tex:IsShown() then
				MessageBoxForVip( "��ʾ", "ʹ�ñ����۴���һ�����ʿ���������λ��" );
				MessageBoxFrameForVipAcceptButton:SetText("����۴�");
				return;
			end
		elseif string.find(szBtnName,"StashFrameBagItem",1,true ) then
			for i=1, 4 do
				local tex = getglobal( "StashFrameBagItem" .. i .. "BagBackTexture" );
				if tex:IsShown() then
					MessageBoxForVip( "��ʾ", "ʹ�òֿ�Կ�׿��Կ����ֿ���λ��" );
					MessageBoxFrameForVipAcceptButton:SetText("ȷ������");
					MessageBoxFrameForVip:SetClientUserData( 0, i );
					MessageBoxFrameForVip:SetClientUserData( 1, 3 );
					return;
				end
			end
		end
		return;
	end

	-- TODO: ѡ����������Щ   �����ǰ�һ�����Ʒ����ѡ��״̬�򷵻�
	if IsContainerItemLock( nContainerType, btn:GetClientID()-1 ) then
		local szMaxDrawLevelProcessFrameName = GetHighestDrawLevelProcessFrame( { ["btn"] = btn } );
		if szMaxDrawLevelProcessFrameName == nil then
			return;
		end
		
		local t_frameLockControl = t_containerLockEvent[szMaxDrawLevelProcessFrameName];
		if t_frameLockControl == nil then
			return;
		end

		local lockProcessFunc = t_frameLockControl["func"];
		if lockProcessFunc == nil then
			return;
		end
		
		lockProcessFunc( used_Item );
		return;
	end

	local bProcessed = ContainerChecAddkBagItem( used_Item );
	if bProcessed then
		return;
	end

	if nContainerType == CONTAINER_TYPE_STASH then
		Save2Container( used_Item, nContainerType, btn:GetClientID()-1 );
		return;
	end
	
	local itemDef = used_Item:getItemDef();

	if itemDef == nil then return end
	local gemNewDef = GameArmProce:getGemNewDef( itemDef.ItemID );
	if ( isKeyPressed(160) or isKeyPressed(161) ) and gemNewDef ~= nil then
		GemProceChargeFrame:Hide();
		setChargeInfo( itemDef.ItemID, 0, used_Item:getListType(),used_Item:getGridIdx() );
		LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId() );
		GemProceChargeFrame:Show();
		return;
	end	

	if used_Item:isArmItem() and nContainerType == CONTAINER_TYPE_SUNDRIES then
		if ( isKeyPressed(160) or isKeyPressed(161) ) and itemDef.Important >= 1 and not IsFashionItem( itemDef ) then
			if GetItemTypeName( math.floor(used_Item:getItemId() / 10000 ) ) == "ʱװ" then
				ShowMidTips( "ʱװ������Ƕ" );
				return;
			end
			if GetItemTypeName( math.floor(used_Item:getItemId() / 10000 ) ) == "����" then
				ShowMidTips( "����װ��������Ƕ" );
				return;
			end
			if used_Item:isArmFengYin() then
				ShowMidTips( "װ����ӡ���޷���Ƕ" );
				return;
			end
			local t_BaoShiXiangQianFrameControl = GetBaoShiXiangQianFrameControl();
			t_BaoShiXiangQianFrameControl:startBaoShiXiangQian( { grid = used_Item:getGridIdx(), list = CONTAINER_TYPE_SUNDRIES } );
			return;
		end
	end

	if not ShopFrame:IsShown() and not TradeFrame:IsShown() and not StallSaleFrame:IsShown() and not StashFrame:IsShown() 
		and not OnlineStoreFrame:IsShown() then
		if ( used_Item:isExtend() or used_Item:isFairyKeyIn() ) then
			if itemDef.WearPre.RoleLevelMin > 0 and MainPlayer:getLv() < itemDef.WearPre.RoleLevelMin then
				ShowMidTips("�ȼ�����");
				return;
			end
			if itemDef.WearPre.RoleLevelMax > 0 and MainPlayer:getLv() > itemDef.WearPre.RoleLevelMax then
				ShowMidTips("�ȼ�̫��");
				return;
			end
			if EnchantFrame:IsShown() then
				EnchantFrame:Hide();
			end
			EnchantUseItem( used_Item );
			return;
		-- ������ ʹ��
		elseif used_Item:isIdentity() then 
			if itemDef.WearPre.RoleLevelMin > 0 and MainPlayer:getLv() < itemDef.WearPre.RoleLevelMin then
				ShowMidTips("�ȼ�����");
				return;
			end
			if IdentifyFrame:IsShown() then
				IdentifyFrame:Hide();
			end
			IdentityUseItem( used_Item );
			return;
		-- ���ν��׾���ʹ��
		elseif used_Item:isBackLevelUp() then 
			BackLevelUpItemUse( used_Item );
			return;
		-- ���θ���
		elseif used_Item:isBackChange() then 
			BackChangeItemUse( used_Item );
			return;
		end
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_SPAR ) and ( isKeyPressed(160) or isKeyPressed(161) ) then
		if SparSorbFrame:IsShown() then
			SparSorbFrame:Hide();
		end
		SparSorbFrameItemBtn:SetClientUserData( 0, used_Item:getListType() )
		SparSorbFrameItemBtn:SetClientUserData( 1, used_Item:getGridIdx() )
		LockItem( used_Item:getListType(), used_Item:getGridIdx() )
		SparSorbFrame:Show();
		return
	end

	-- ������Ӧ�ĺ�������
	local curFrameIndex		= 0;
	local curFrameDrawLevel = 0;
	for i = 1, table.getn( t_ItemFirstCallBackFun ) do
		local frame = getglobal( t_ItemFirstCallBackFun[i][1] );
		if frame:IsShown()
			and t_ItemFirstCallBackFun[i][2] ~= nil
			and frame:GetName() ~= btn:GetParent()
			and curFrameDrawLevel < frame:getFrameDrawLevel() then
				curFrameDrawLevel = frame:getFrameDrawLevel();
				curFrameIndex = i;
		end
	end
	if curFrameIndex > 0 then
		if not t_ItemFirstCallBackFun[curFrameIndex][2]( used_Item, nContainerType, btn:GetClientID()-1, t_ItemFirstCallBackFun[curFrameIndex][1] ) then
			return;
		end
	end
	-- ���Һ�����Ʒʹ��
	if used_Item:getItemId() == CLT_RONG_YAN_ITEM_ID or used_Item:getItemId() == ZJ_RONGYAN_ITEM_ID_TOLUA  then 
		UseRongYanSelectItem( used_Item:getItemId() );
		return;
	end
	if used_Item ~= nil and nContainerType == CONTAINER_TYPE_SUNDRIES then
		local itemDef = used_Item:getItemDef() 
		if CheckItemResult( itemDef, RESULT_LOST_TOTEM ) then
			--ʧ��ͼ�ڵ���
			local maxPower = used_Item:getItemInstVal1()
			local curPower = used_Item:getItemInstVal2()
			if curPower == maxPower and not container:isItemInCD( itemDef.ItemID ) then
				MessageBox("ͼ��ʹ����ʾ","ÿ�����ʹ��5�Σ�ȷ��ʹ�ú����ӽ�ɫ����");
				MessageBoxFrame:SetClientUserData(0,used_Item:getListType());
				MessageBoxFrame:SetClientUserData(1,used_Item:getGridIdx());				
				LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "MessageBoxFrame" )
				return
			end
		end
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_SPAR ) then
		for i=1,container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) do
			local item = container:getItem( CONTAINER_TYPE_SPAR, i-1 )
			if item:getItemId() == 0 then
				if GameArmProce:requestSparInlay( used_Item:getListType(), used_Item:getGridIdx(), CONTAINER_TYPE_SPAR, i-1 ) then
					SparFrame:Show()
				end
				return;
			end
		end
		--ShowMidTips( "û�п��еľ�ʯ��" )
		if container:getContainerMaxNum( CONTAINER_TYPE_SPAR ) > 0 then
			if GameArmProce:requestSparInlay( used_Item:getListType(), used_Item:getGridIdx(), CONTAINER_TYPE_SPAR, 0 ) then
				SparFrame:Show()
			end
		end
		return;
	end
	
	if used_Item:isValidEnd() then
		ShowMidTips(used_Item:getDisplayName() .. "����Ч���ѵ��ڣ��޷�ʹ��");
		return;
	end

	local t_changeCareerControl = GetChangeCareerControl();
	if t_changeCareerControl:isChangeCareerItem( { id = used_Item:getItemId() } ) then
		if t_changeCareerControl:canStartChangeCareer( { id = used_Item:getItemId() } ) then
			t_changeCareerControl:startChangeCareer( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
		end
		return;
	end
	
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_BISAI ) then
		t_changeCareerControl:startChangeCareer( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
	end

	if used_Item:getItemId() == ARM_UNBIND_ITEM_LOW_TOLUA or used_Item:getItemId() == ARM_UNBIND_ITEM_MID_TOLUA or used_Item:getItemId() == ARM_UNBIND_ITEM_HIGH_TOLUA then
		ArmProceUnBindFrame:SetClientUserData( 0,used_Item:getItemId() );
		ArmProceUnBindFrame:SetClientUserData( 1,used_Item:getListType() );
		ArmProceUnBindFrame:SetClientUserData( 2,used_Item:getGridIdx() );
		LockItem( used_Item:getListType(),used_Item:getGridIdx(), used_Item:getItemId(), "ArmProceUnBindFrame" ); 
		ArmProceUnBindFrame:SetPoint("topleft","UIClient","topleft",200+PlayerContainerFrame:GetWidth() ,170);
		ArmProceUnBindFrame:Show();
	end

	if used_Item:getItemId() == CLIENT_MAIL_PACKAGE_ID or used_Item:getItemId() == CLIENT_MAIL_BINDED_PACKAGE_ID then
		local check = getglobal("MailItemSendItemCheckBtn");
		if not check:GetCheckState() or not MailSendItemFrame:IsShown() then
			MailSendItemFrame:Show();
			MailSendItemFrame:SetClientUserData(0,nContainerType);
			MailSendItemFrame:SetClientUserData(1,(btn:GetClientID()-1));
			if MailHeadFrame:IsShown() then
				MailHeadFrame:Hide();
				MailSendItemFrame:SetPoint("topleft","$parent","topleft",MailHeadFrame:GetRealLeft(), MailHeadFrame:GetRealTop());
			end
			
			check:setCheckState( true );
			MailSendItemRefreshItemBox();
			--PlayerContainerFrame:SetPoint("center", "$parent", "center", -170, 0 );
			--MailSendItemFrame:SetPoint("center", "$parent", "center", 230, 0 );
			LockItem( nContainerType, (btn:GetClientID()-1), used_Item:getItemId(), "MailSendItemFrame" ); 
			return;
		end
	end

	if bProcessed == nil then
		ShowMidTips("�޷�װ������Ʒ");
	end
	
	if CheckItemResult( used_Item:getItemDef(), RESULT_PET_GENIUS_BOOK ) then
		print("CheckItemResult pet genius book is true");
		UsePetGiftBook( used_Item )
		return
	end
	
	-- �ж���Ʒ�ܷ�ʹ��
	if not CanUseItem( itemDef, true, used_Item ) then
		return;
	end
	local t_GameCBTFrameControl = GetGameCBTFrameControl();
	local gameCBT				= MainPlayer:getCBTInfo();
	if gameCBT:isCangBaoTuItem( used_Item:getItemId() ) then
		if t_GameCBTFrameControl:canStartCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } ) then
			t_GameCBTFrameControl:startCangBao( { listType = used_Item:getListType(), gridx = used_Item:getGridIdx() } );
		end
		return;
	end

	local t_BaoShiHeChengControl = GetBaoShiHeChengControl();
	if t_BaoShiHeChengControl:isHeChengFu( { id = used_Item:getItemId() } ) then
		if t_BaoShiHeChengControl:canStartHeChengGem( { id = used_Item:getItemId() } ) then
			t_BaoShiHeChengControl:startHeChengGem( { list = used_Item:getListType(), grid = used_Item:getGridIdx() } );
		end
		return;
	end
	
	if IsSkillRuneCombineItem(used_Item:getItemId()) then
		return OpenSkillRuneCombineFrame(used_Item)
	end
	
	if IsItemPetLearnSKillBook(used_Item:getItemId()) then
		return UseItemLearnPetSkill(used_Item)
	end
	
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_EXP_BUCHANG ) then
		OpenReturnGiftExp( used_Item )
		return
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_BUCHANG ) then
		OpenReturnArmSelect( used_Item, MainPlayer:getLv() )
		return
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_ARM_SELECT ) then
		for i = 1, MAX_ITEM_RESULT do
			local resultData = itemDef.Result[i-1];
			if resultData.ResultID == RESULT_ITEM_ARM_SELECT then
				OpenReturnArmSelect( used_Item, resultData.ResultVal1*10 )
			end
		end
		return
	end

	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_CHAT ) or CheckItemResult( used_Item:getItemDef(), RESULT_SPAN_CHAT ) then
		-- TODO
		local t_laBaItem = GetLaBaItem();
		if t_laBaItem["grid"] ~= -1 then
			local oldItem	= container:getItem( t_laBaItem["list"], t_laBaItem["grid"] );
			UnLockItem( t_laBaItem["list"], t_laBaItem["grid"], oldItem:getItemId() );
			SetLaBaItem( { grid = -1, list = -1 } );
		end

		SetLaBaItem( { grid = used_Item:getGridIdx(), list = used_Item:getListType() } );
		LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "ChatBugleFrame" );

		if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_CHAT ) then
			ChatBugleFrameHeadFont:SetText( "С����" );
		else
			ChatBugleFrameHeadFont:SetText( "�������" );
		end
		ChatBugleFrame:Show();
		return;
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_SHANG_JI_TASK ) then
		local oldItemListType = BountyHunterTask:GetClientUserData( 0 );
		local oldItemGridIdx = BountyHunterTask:GetClientUserData( 1 );
		local oldItemId = BountyHunterTask:GetClientUserData( 2 );
		local oldItem = container:getItem( oldItemListType, oldItemGridIdx );
		if oldItem:getItemId() ~= nil and oldItem:getItemId() == oldItemId then
			UnLockItem( oldItemListType, oldItemGridIdx, oldItemId );
		end
		BountyHunterTask:SetClientUserData( 0, used_Item:getListType() );
		BountyHunterTask:SetClientUserData( 1, used_Item:getGridIdx() );
		BountyHunterTask:SetClientUserData( 2, used_Item:getItemId() );
		Quest:UseBountyHunterItem( used_Item:getItemId() );
		return;
	end

	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_TASK_RECYCLE ) then
		if MainPlayer:isDead() then
			ShowMidTips("���Ѿ�����");
		else
			Quest:RequestRecycleTaskByItem( used_Item:getListType(), used_Item:getGridIdx() );
		end
		return
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_TILIAN ) then
		if GeniusMgr:getAllGeniusValue() < TILIAN_CAN_USE_LV then
			return ShowMidTips( string.format("��Ҫ���츳ֵ�ﵽ%d�����ʹ�þ�ʯ����", TILIAN_CAN_USE_LV) );
		else
			if MainPlayer:isDead() then
				return ShowMidTips("���Ѿ�����");
			end
			PlayerContainerSparEpurateFrame:Show();
		end
		return;
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_LEGEND_SCROLL ) then 
		OpenArmUpOrangeFrame( used_Item );
		return;
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_PET_GENIUS_UP ) then
		UsePetGeniusUpItem( used_Item )
		return
	end	
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_GEM_PAG ) then
		OpenGemSelectPag( used_Item )
		return
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_JIAJU ) then
		BeginFurnitureMode( used_Item )
		return
	end
	if CheckItemResult( used_Item:getItemDef(), RESULT_ITEM_HOME_PET ) then
		ChangeHomelandPet( used_Item )
		return
	end
	if used_Item:getItemId() >= 5063000 and used_Item:getItemId() <= 5063999 then
		if GetItemLink(used_Item:getItemId()) ~= nil then
			--webframe:SetPoint( "center", "$parent", "center", 0, 0 );
			--webframe:setLocalFile( GetItemLink(used_Item:getItemId()).link );
			--webframe:SetSize(498,409);
			--webframe:SetClientString("��ҳ����");
			--webframe:Show();
			return;
		end
	end
	
	if used_Item:getItemId() == 5056000 then
		if container:usedItem(5056000) then
			returnToChangeFashion();
			return;
		end
	end
	-- װ����ָ���������
	local nWearPos = 0;
	if( used_Item:isRing() ) then
		nWearPos = GetEquipRingPos();
	end

	if ( used_Item:isAmuleft() ) then
		nWearPos = GetEquipXianlianPos();
	end

	if IsFashionItem( itemDef ) then 
		nWearPos = GetFashionSuitPos( itemDef.WearPos1 );
	end
	
	if not used_Item:isItemBind() and itemDef.ArmBind == ITEM_WEAR_BIND then
		MessageBoxFrame:SetClientUserData( 0, itemDef.ItemID );
		MessageBoxFrame:SetClientUserData( 1, nWearPos );
		MessageBoxFrame:SetClientUserData( 2, nContainerType);
		MessageBoxFrame:SetClientUserData( 3, (btn:GetClientID()-1));
		MessageBox("��װ��","�Ƿ�װ��"..itemDef.Name.."?(װ�����)");
		return;
	end

	for i = 1,MAX_ITEM_RESULT do
		if	itemDef.Result[i-1].ResultID ==  RESULT_ITEM_TASK then
			local t_ResultValue =
			{
				itemDef.Result[i-1].ResultVal1,
				itemDef.Result[i-1].ResultVal2,
				itemDef.Result[i-1].ResultVal3,
				itemDef.Result[i-1].ResultVal4
			};

			local nTaskId = 0;
			for i = 1, table.getn(t_ResultValue) do
				if t_ResultValue[i] ~= 0 then
					nTaskId = t_ResultValue[i];
					break;
				end
			end

			if nTaskId ~= 0 then
				-- TODO
				local recycleId = Quest:isQuestRun( nTaskId ) and Quest:getQuestRunRecycleID( nTaskId ) 
					or Quest:getFirstCanAcceptRecycleTaskID( nTaskId );
				local finiNum = Quest:getQuestOneDayLimitNum( nTaskId, recycleId );
				if finiNum == 0 and Quest:isQuestFini( nTaskId ) then
					container:send_MoveItem( used_Item:getListType(), -1, used_Item:getGridIdx(), used_Item:getNum(), 0 );
					ShowMidTips( "����������ɣ��޷��ٴν�ȡ�����������" );		
					return;
				elseif finiNum > 0 and Quest:getQuestOneDayFiniNum( nTaskId, recycleId ) >= finiNum then
					return ShowMidTips( "����������ﵽÿ�����ޣ������������ȡ" );
				end

				if Quest:isQuestRun( nTaskId ) then
					ShowMidTips( "�Ѿ����ܸ������޷��ٴ���ȡ" );		
					return;
				end

				if Quest:canAcceptQuest( nTaskId ) ~= CLINT_CAN_ACCEPT_TASK then
					ShowMidTips( "�����񲻿ɽ���" );		
					return;
				end

				TaskAcceptFrame:SetClientUserData(0,0);					--NPCId
				TaskAcceptFrame:SetClientUserData(1,nTaskId);			--����Id
				TaskAcceptFrame:SetClientUserData(2,nContainerType);	--���� ������ȡ����
				TaskAcceptFrame:SetClientUserData(3,btn:GetClientID()-1);	--���� ������ȡ����
				TaskAcceptFrame:SetClientString("������ȡ����");
				local t_TaskAcceptFrameControl = GetTaskAcceptFrameControl();
				t_TaskAcceptFrameControl:setTalkInfo( { npcId = 0, taskId = nTaskId, isItemTask = true, taskType = ITEM_TASK_TYPE } );
				LockItem( used_Item:getListType(), used_Item:getGridIdx(), used_Item:getItemId(), "TaskAcceptFrame" );
				NpcDialog:send_CloseTalk();
				TaskAcceptFrame:Show();
			end
			return;
		end
	end	
	for i = 1,MAX_ITEM_RESULT do
		if itemDef.Result[i-1].ResultID ==  RESULT_FAIRY_BORN and MainPlayer:isDead() then			
			ShowMidTips("����״̬���޷���Ӿ���");
			return;	
		end
	end
	container:usedItem( nContainerType, btn:GetClientID()-1, nWearPos, itemDef.UseTarget == USE_TARGET_PET and getFightPetIndex() or 0 );
end

-- ������ڴ��ʼ���Ʒ�б����һ�
function MailRigthBtnClick( szBtnName )
	if MessageBoxFrame:IsShown() and MessageTitle:GetText() == "�����ʼ�" then
		return
	end

	local btn	= getglobal( szBtnName );
	local senddata	= GameMailManager:getSendData();
	local item		= senddata.m_Items[btn:GetClientID()-1];
	UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );	-- ����
	GameMailManager:delSendItem( btn:GetClientID()-1 );
	MailSendItemUpdate();
end

-- ��������ռ���Ʒ�б����һ�
function GetMailItemRightBtnClick( szBtnName )
	local btn	= getglobal( szBtnName );
	GameMailManager:mail_GetGoods( MailReadFrame:GetClientUserData( 0 ), MailReadFrame:GetClientUserData( 1 ), (btn:GetClientID()-1));
	ReSetMailContent();
	local t_CheckMailInfo = GetCheckMailInfo();
	local PageMail = GameMailManager:getOnePageMail( t_CheckMailInfo[1] );
	BondingMailContent( PageMail.m_MainData[t_CheckMailInfo[2]-1], COLOR_WHITE);
end

-- ���ǶԴ���̯��Ʒ����Ҽ�
function Private_XiaJiaInStall()
	local selfStallInfo = GameStallManager:getSelfStallItem();
	if t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_ITEM then
		local t_selectItemData	= t_StallSaleFrameControl["item"];
		local nSelectIndex		= t_selectItemData["selectIndex"];
		if nSelectIndex == 0 then
			return;
		end

		GameStallManager:delItemInStall( nSelectIndex - 1 );
	elseif t_StallSaleFrameControl["stallPageType"] == CLIENT_STALL_TYPE_PET then
		local t_selectPetData	= t_StallSaleFrameControl["pet"];
		local nSelectIndex		= t_selectPetData["selectIndex"];
		if nSelectIndex == 0 then
			return;
		end
		
		GameStallManager:delPetInStall( nSelectIndex - 1 );
	end
end

function StallSaleRightBtnClick( szBtnName )
	local btn		= getglobal( szBtnName );
	local nIndex	= btn:GetClientUserData(1);
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInStall() then
		GameStallManager:delItemInStall( nIndex );
		return;
	end

	
	local nType		= btn:GetClientUserData(0);
	local itemdata	= GameStallManager:getSelfStallItem();
	local item		= itemdata.m_ItemData[nIndex];
	if item:getItemId() == 0 then	
		return;
	end
	UnLockItem( item:getListType(), item:getGridIdx(), item:getItemId() );--����
	GameStallManager:delStallItem( nIndex );

	local StallData = GameStallManager:getSelfStallItem();
	UpdateStallSaleFrame();
	RemoveCheckItem( btn:GetParent() );
end

function OnlineStoreItemLeftBtnClick( szBtnName )
	local btn = getglobal( szBtnName );
	local nIdx = btn:GetClientUserData(0) - 1;
	if nIdx < 0 then return end;

	local item = StoreMgr:getStoreItem( nIdx );
	if item == nil then return end;
	SetSelectStoreItem( szBtnName );
	OnlineStoreFramePriceFont:SetText( StoreMgr:getStoreItemPrice( nIdx ) );

	StoreMgr:setItemLink( nIdx );
end

function OnlineStoreItemRightBtnClick( szBtnName )
	local btn = getglobal( szBtnName );
	local nIdx = btn:GetClientUserData(0) - 1;
	if nIdx < 0 then return end;
	
	local self, id = IsInSelfStore();
	local item = StoreMgr:getStoreItem( nIdx );
	if id == 0 or item == nil then return end;
	SetSelectStoreItem( szBtnName );
	if self then
		StoreMgr:requestStoreDownItem( nIdx, item:getItemWId() );
	else
		ShowOnlineStoreItemOptFrame( nIdx, "����ȷ��", StoreMgr:getStoreItemPrice( nIdx ) );
	end
end

--------------------------------------------------------------ITEMTEMPLATE�������ģ��-------------------------------------------------------------------------------------------
-- �����������뱳��
function ContainerOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local container			= MainPlayer:getContainer();

	local nContainerType	= GetCurContainerType( btn:GetParent(),szBtnName ); -- ��ȡ��ǰ�һ�����Ʒ�ı�������

	if nContainerType == 0 and  btn:GetClientID() < 0 then
		return;
	end	
	local item = container:getItem( nContainerType, btn:GetClientID()-1 );
	if item:getItemId() == 0 then
		if string.find( szBtnName,"PlayerContainerFrameBagItem" ) then
			local BagBackTexture = getglobal( szBtnName.."BagBackTexture" );
			local szIndex = "��"..btn:GetClientID().."�񱳰���λ��\n"
			if BagBackTexture:IsShown() then
				SetGameTooltips(btn:GetName(), szIndex.."��ʹ�ñ����۴���չ��λ" );
			else
				SetGameTooltips(btn:GetName(), szIndex.."����뱳��");
			end
		elseif string.find( szBtnName,"StashFrameBagItem" ) then
			local BagBackTexture = getglobal( szBtnName.."BagBackTexture" );
			local szIndex = "��"..btn:GetClientID().."��ֿ���λ��\n"
			if BagBackTexture:IsShown() then
				SetGameTooltips(btn:GetName(), szIndex.."��ʹ�òֿ�Կ����չ��λ");
			else
				SetGameTooltips(btn:GetName(), szIndex.."����뱳��");
			end
		elseif string.find( szBtnName, "PlayerContainerFrameTradeItem" ) then
			local itemId = btn:GetClientUserData(0);
			if itemId ~= 0 then
				ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemId) );
				return;
			end
		end
	end

	if item:getItemId() <= 0 then
		local t_TipFrames = { "NormalItemTipsFrame", "ArmItemTipsFrame", "SecondArmItemTipsFrame" };
		for _, szTips in ipairs( t_TipFrames ) do
			local tipsframe = getglobal(szTips);
			if tipsframe:IsShown() then
				tipsframe:Hide();
			end	
		end
		return;
	end
	
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, ContainerOnEnter, szBtnName);
end

-- �����������뽻�����
function TradeOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	local item		= nil;
	if btn:GetClientUserData( 0 ) == DRAG_TRADE then
		if btn:GetClientUserData( 2 ) == 0 then
			-- ����ʾ�ļ�����Ʒ����
			item = GameExchange:getExchangItem( 0, btn:GetClientUserData(1) );
		else
			-- ����ʾ���ǶԷ���Ʒ������
			item = GameExchange:getExchangItem( 1, btn:GetClientUserData(1) );
		end
	end

	if item:getItemId() ~= 0  then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, TradeOnEnter, szBtnName);
	end
end

-- �������������̵���Ʒ
function ShopOnEnter( szBtnName )
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local btn		= getglobal( szBtnName );
	local szGemData;
	if btn:GetClientUserData(1) > 0 then
		if getItemDef(btn:GetClientUserData(1)) ~= nil and btn:GetClientUserData(0) ~= DRAG_SAVEITEM then
			--print( "ShopOnEnter--szBtnName--2389" );
			local isShopItem;
			if string.find( szBtnName, "ShopItemFrame", 1, true) then
				isShopItem = true;
			end
			szGemData = "0,0";
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(btn:GetClientUserData(1)), ShopOnEnter, szBtnName, isShopItem, nil, szGemData );
		elseif btn:GetClientUserData(2) ~= 0 then
			local ShopList			= NpcShop:getShopList();
			local shopSaveItem		= ShopList.m_ShopSaveItems[btn:GetClientUserData(2)-1].m_SaveItem;
			if shopSaveItem:getItemType() ~= ITEM_SLOTTYPE_NONE then
				--print( "ShopOnEnter--szBtnName--2394" );
				szGemData = shopSaveItem:getGemStart()..","..shopSaveItem:getGemPower();
				ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, shopSaveItem, ShopOnEnter, szBtnName, nil, nil, szGemData );
			end
		end
	end
end

-- ��������������������Ʒ
function TaskOnEnter( szBtnName ) 
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return;
	end
	local btn		= getglobal( szBtnName );
	if btn:GetClientUserData(1) ~= 0 and getItemDef(btn:GetClientUserData(1)) ~= nil then
		NormalItemTipsFrame:SetClientUserData( 0, btn:GetClientUserData(2) );
		NormalItemTipsFrame:SetClientUserData( 1, btn:GetClientUserData(3) );
		ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(btn:GetClientUserData(1)), TaskOnEnter, szBtnName);
	end
end

-- �����������װ��
function WearformOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	local MainPlayer	= ActorMgr:getMainPlayer();
	local Eqiup		= MainPlayer:getEquip();
	if Eqiup == nil then
		return;
	end
	local nWearPos = GetFashionSuitPos( btn:GetClientID() );
	local item = Eqiup:getItem( nWearPos );
	if item == nil then
		return;
	elseif item:getItemId() == 0 and t_WearPosInfo[btn:GetClientID()] ~= nil then
		SetGameTooltips(btn:GetName(), t_WearPosInfo[btn:GetClientID()] );
		return;
	end
	
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, WearformOnEnter, szBtnName );
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
end

function WatchPlayerOnEnter( szBtnName )
	local btn				= getglobal( szBtnName );
	local nWearPos 			= GetWatchFashionSuitPos( btn:GetClientID() );
	local Item				= GameWatchPlayer:getItem( nWearPos );

	if Item:getItemId() == 0 then
		SetGameTooltips(btn:GetName(), t_WearPosInfo[btn:GetClientID()] );
		return;
	end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, Item, WatchPlayerOnEnter, szBtnName );
end

-- ��������ʼ�
function MailOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	if btn:GetParent() == "MailSendItemFrame" then
		local MailSendData	= GameMailManager:getSendData();
		local item		= MailSendData.m_Items[btn:GetClientID()-1];
		if item:getItemId() > 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, MailOnEnter, szBtnName );
		end
	end

	if btn:GetParent() == "MailReadFrame" then
		local PageMailData	= GameMailManager:getOnePageMail( MailReadFrame:GetClientUserData( 3 ) );
		local MailData		= PageMailData.m_MainData[MailReadFrame:GetClientUserData( 2 ) - 1];
		local item = MailData.m_MailDetail.m_Items[btn:GetClientID() - 1];
		if item:getItemId() > 0 then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, MailOnEnter, szBtnName );
		end
	end
end

-- ������ڰ�̯�������
function StallSaleOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	local StallItem = GameStallManager:getSelfStallItem();
	local item		= StallItem.m_ItemData[btn:GetClientUserData( 1 )];
	if item:getItemId() ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, StallSaleOnEnter, szBtnName, false, true );
	end
end

-- ������ڰ�̯������
function StallBuyOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	local StallItem = GameStallManager:getSeeStallItem();
	local item		= StallItem.m_ItemData[btn:GetClientUserData( 1 )];
	if item:getItemId() ~= 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, StallBuyOnEnter, szBtnName, false, true );
	end
end

--������������ѡ��Ʒ���
function SetFittingRetainItemOnEnter( szName )
	local MainPlayer		= ActorMgr:getMainPlayer();
	local container			= MainPlayer:getContainer();
	local btn				= getglobal(szName);
	local Item = container:getItem(btn:GetClientUserData(2), btn:GetClientUserData(3));
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, Item );
end

--������ڴ��ӹ�װ�����
function ProceEquipOnEnter( szBtnName )
	local item	= GameArmProce:getSrcItem();
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, ProceEquipOnEnter, szBtnName );
	end
end

--������ڱ�ʯ�ӹ��������
function StuffGemOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	if btn:GetClientUserData( 0 ) > 0 then
		if getItemDef( btn:GetClientUserData( 0 ) ) ~= nil then
			ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( btn:GetClientUserData( 0 ) ), StuffGemOnEnter, szBtnName );
		end
	end
end

--�������װ���ӹ��������
function StuffEquipOnEnter( szBtnName )
	local btn		= getglobal( szBtnName );
	local item	= GameArmProce:getStuffItem(btn:GetClientID() - 1);
	if item:getItemId() > 0 then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item, StuffEquipOnEnter, szBtnName );
		return;
	end

	if btn:GetParent() == "GemProcePickFrame"  then
		if btn:GetClientUserData( 1 ) > 0 then
			if getItemDef( btn:GetClientUserData( 1 ) ) ~= nil then
				ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef( btn:GetClientUserData( 1 ) ), StuffEquipOnEnter, szBtnName );
			end
		end
	end
end

--�̳�
function MarketOnEnter(szBtnName)
	local btn	= getglobal(szBtnName);
	if btn:GetClientUserData(1) > 0 then
		if getItemDef(btn:GetClientUserData(1)) ~= nil then
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(btn:GetClientUserData(1)), ShopOnEnter, szBtnName);
		end
	end
end
--------------------------------------------------------------ITEMTEMPLATE��קģ��-------------------------------------------------------------------------------------------
-- �ӱ����ϳ�
function ContainerDrag( szBtnName )
	local btn				= getglobal( szBtnName );
	local MainPlayer		= ActorMgr:getMainPlayer();
	local container			= MainPlayer:getContainer();
	local desList			= GetCurContainerType( btn:GetParent(),szBtnName ); -- ��ȡ��ǰ�һ�����Ʒ�ı�������
	local desGrid			= btn:GetClientID()-1;						-- Ŀ�����

	-- �Ƿ���ʹ��shitf������Ʒ��Ʒ������
	bClickLink = container:getItemLink( desList, desGrid );
	if bClickLink then
		return;
	end
	if IsContainerItemLock( desList, desGrid ) then
		return;
	end
	-- �������
	if GetCurrentCursor() == "repair" and desList == CONTAINER_TYPE_SUNDRIES then
		RepairOneItem( CONTAINER_TYPE_SUNDRIES, desGrid );
		return;
	end

	local Item = container:getItem( desList, desGrid );
	if Item:getItemId() ~= 0 then
		-- ������Ľ����е���Ʒ�����϶�
		if IsContainerItemLock(  desList, desGrid ) then
			return;
		end

		if CheckItemResult( Item:getItemDef(), RESULT_ITEM_ZHONGZHI ) then
			if not BeginPlantMode( szBtnName, Item ) then
				return
			end
		end

		UIMSG:getUIGV().DragFrom = desList;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		--print( "ContainerDrag--2630" );
		UIBeginDrag(IconTex:GetTexture(), desGrid, Item:getNum(), Item:getItemId());
	end

	-- ������ק״̬ ͬʱ �Ҳ�������δ��ʾ ����ʾ�Ҳ�������
	if UIIsInDragState() and not ExtendSkillLeftFrame:IsShown() then
		ShowExtendSkill();
	end
end

-- ������װ���ϳ�
function WearformDrag( szBtnName )
	local btn			= getglobal( szBtnName );
	local MainPlayer	= ActorMgr:getMainPlayer();
	local container		= MainPlayer:getContainer();
	local Eqiup			= MainPlayer:getEquip();
	local nWearPos 		= GetFashionSuitPos( btn:GetClientID()  );
	local Item			= Eqiup:getItem( nWearPos );
	if Item:getItemId() ~= 0 then
		UIMSG:getUIGV().DragFrom = DRAG_WEARFORM;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture() , Item:getItemId(), nWearPos , 0);
	end

	local xinShouBtn	= getglobal( "EquipFrameEffectFrame" );
	xinShouBtn:Hide();
end

-- ���ʼ�������Ʒ���ϳ�
function MailDrag( szBtnName )
	local btn		= getglobal( szBtnName );
	local senddata = GameMailManager:getSendData();
	local item = senddata.m_Items[btn:GetClientID()-1];
	if item:getItemId() ~= 0 then
		UIMSG:getUIGV().DragFrom = DRAG_MAIL;
		local IconTex = getglobal(btn:GetName().."IconTexture");
		UIBeginDrag(IconTex:GetTexture(), (btn:GetClientID()-1), item:getNum(), btn:GetClientUserData( 2 ));
	end
end

-- �Ӱ�̯������Ʒ���ϳ�
function StallDrag( szBtnName )
	local btn		= getglobal( szBtnName );
	-- ���ڵİ�̯�е���Ʒ�����϶�
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:isInStall() then
		return;
	end

	local stalldata = GameStallManager:getSelfStallItem();
	local stallitem	= stalldata.m_ItemData[btn:GetClientUserData(1)];
	if stallitem:getItemId() == 0  then
		return;
	end

	UIMSG:getUIGV().DragFrom = DRAG_STALL;
	local IconTex = getglobal(btn:GetName().."IconTexture");
	UIBeginDrag(IconTex:GetTexture(), btn:GetClientUserData( 1 ), btn:GetClientUserData( 2 ) , 0);
end

-- �ӽ������Ĵ�������Ʒ���ϳ�
function TradeDrag( szBtnName )
	local btn = getglobal( szBtnName );
	-- ���϶����ǶԷ�����Ʒ���ϵ���Ʒ,���ܽ����϶�����
	if btn:GetClientUserData( 2 ) == 1 then
		return;
	end
	-- ������Ʒ��û���κ���Ʒ,����Ӧ��ҷ����
	local item = GameExchange:getExchangItem( 0, btn:GetClientUserData( 1 ) );
	if	item:getItemId() == 0 then
		return;
	end

	UIMSG:getUIGV().DragFrom = DRAG_TRADE;
	local IconTex = getglobal( btn:GetName().."IconTexture" );
	UIBeginDrag( IconTex:GetTexture(), btn:GetClientUserData( 1 ), 0 , 0 );
end

--------------------------------------------------------------ITEMTEMPLATE������ģ��-------------------------------------------------------------------------------------------

-- ������ڱ���������
function ContainerLeftBtnClick( szBtnName )
	local btn				= getglobal( szBtnName );					-- ��ȡ��ǰ��Ʒ�ĸ�����
	local nContainerType	= GetCurContainerType( btn:GetParent(),szBtnName ); -- ��ȡ��ǰ�һ�����Ʒ�ı�������
	local mainplayer		= ActorMgr:getMainPlayer();
	local container			= mainplayer:getContainer();
	local item				= container:getItem( nContainerType, btn:GetClientID()-1 );

	if item:getItemId() == 0 then
		if string.find(szBtnName,"PlayerContainerFrameBagItem",1,true ) then
			local tex = getglobal( szBtnName .. "BagBackTexture" );
			if tex:IsShown() then
				MessageBoxForVip( "��ʾ", "ʹ�ñ����۴���һ�����ʿ���������λ��" );
				MessageBoxFrameForVipAcceptButton:SetText("����۴�");
				return;
			end
		elseif string.find(szBtnName,"StashFrameBagItem",1,true ) then
			for i=1, 4 do
				local tex = getglobal( "StashFrameBagItem" .. i .. "BagBackTexture" );
				if tex:IsShown() then
					MessageBoxForVip( "��ʾ", "ʹ�òֿ�Կ�׿��Կ����ֿ���λ��" );
					MessageBoxFrameForVipAcceptButton:SetText("ȷ������");
					MessageBoxFrameForVip:SetClientUserData( 0, i );
					MessageBoxFrameForVip:SetClientUserData( 1, 3 );
					return;
				end
			end
		end
		return;
	end

	-- �����ǰ�һ�����Ʒ����ѡ��״̬�򷵻�
	if IsContainerItemLock( nContainerType, btn:GetClientID()-1 ) then
		return;
	end

	-- �������
	if GetCurrentCursor() == "repair" and nContainerType == CONTAINER_TYPE_SUNDRIES then
		RepairOneItem( CONTAINER_TYPE_SUNDRIES, btn:GetClientID()-1 );
		return;
	end
	if string.find(szBtnName,"BagItem",1,true ) then
		PlayerContainerFrameSetPackage();
		StashContainerFrameSetPackage();
	else
		SetItemButtonCheckState( btn:GetParent(), btn:GetClientID()-1, btn:GetName() );
	end	
	
	if nContainerType < CONTAINER_TYPE_END then
		-- �Ƿ���ʹ��shitf������Ʒ��Ʒ������
		-- "FriendChatFrame1_InputEdit"	
		local friendChatFrame = GetCurDrawHighChatFrame();
		if OnlineStoreFrame:IsShown() then
			if item:getItemId() ~= 0 then
				local itemDef = item:getItemDef()
				OnlineStoreFrameSearchEdit:SetText( itemDef.Name )
				OnlineStoreFrame:AddLevelRecursive()
				SetFocusFrame( "OnlineStoreFrameSearchEdit" )
				return
			end
		elseif ( ChatInsertFrame:IsShown() or ChatBugleFrame:IsShown() or ( friendChatFrame ~= nil ) or FriendMutilSendFrame:IsShown() ) then
			bClickLink = container:getItemLink( nContainerType, btn:GetClientID()-1 );
			if bClickLink then
				return;
			end
		else
			if isKeyPressed(160) or isKeyPressed(161) then
				if nContainerType == CONTAINER_TYPE_STUFF or nContainerType == CONTAINER_TYPE_TASK or nContainerType == CONTAINER_TYPE_SUNDRIES then
					ContainerFrame_SplitBtn_OnClick();
				elseif nContainerType == CONTAINER_TYPE_STASH then
					StashFrame_SplitBtn_OnClick();
				end
			end
		end
	end
end

-- ����������������������װ��
function EquipLeftBtnClick( szBtnName )
	local btn			= getglobal( szBtnName );
	local MainPlayer	= ActorMgr:getMainPlayer();
	local Eqiup			= MainPlayer:getEquip();
	local nWearPos 		= GetFashionSuitPos( btn:GetClientID()  );
	local xinShouBtn	= getglobal( "EquipFrameEffectFrame" );
	xinShouBtn:Hide();

	local item = Eqiup:getItem( nWearPos );
	if item == nil then
		return;
	elseif item:getItemId() == 0 then
		SetGameTooltips(btn:GetName(), t_WearPosInfo[btn:GetClientID()] );
		return;
	end

	--��ʾ���ֵ���������
	if OnlineStoreFrame:IsShown() then
		if item:getItemId() ~= 0 then
			local itemDef = item:getItemDef()
			OnlineStoreFrameSearchEdit:SetText( itemDef.Name )
			OnlineStoreFrame:AddLevelRecursive()
			SetFocusFrame( "OnlineStoreFrameSearchEdit" )
			return
		end
	end
	-- �Ƿ���ʹ��shitf������Ʒ��Ʒ������
	bClickLink = Eqiup:getItemLink( btn:GetClientID() );
	if bClickLink then
		return;
	end
end

-- ��������̵���Ʒ�ع��е��
function ShopLeftBtnClick( szBtnName , item )
	if CostMoneyConfirmFrame:IsShown() then
		return;
	end
	local btn = getglobal( szBtnName );
	if btn:GetClientUserData(0) ~= DRAG_SAVEITEM or btn:GetClientUserData( 1 ) == 0 then
		return;
	end
	
	local nIndex = btn:GetClientID() - 1;
	local ShopList = NpcShop:getShopList();
	local shopSaveItem = ShopList.m_ShopSaveItems[nIndex].m_SaveItem;
	local itemDef = shopSaveItem:getItemDef();
	if itemDef ~= nil then
		local mainplayer = ActorMgr:getMainPlayer();
		local money = itemDef.SellPrice * shopSaveItem:getNum();
		if getPlayerMoney( mainplayer ) < money then
			ShowMidTips( "�������㣬�޷��ع�" );
			return;
		end

		if checkNeedWarnCost( money ) then
			CostMoneyConfirmFrame:SetClientString("�ع���Ʒ");
			CostMoneyConfirmFrame:SetClientUserData( 0, nIndex );
			SetWarnFrame( "ShopFrame" );
			CostMoneyConfirmFrame:Show();
			return;
		end
	end

	BuySaveItem( nIndex );
	btn:SetClientUserData( 1, 0 );
end

-- ��������ʼ���Ʒ��ȡ�е��
function GetMailItemLeftBtnClick( szBtnName )
	local btn	= getglobal( szBtnName );
	if btn:GetClientUserData(2) == 0 then
		return;
	end
	for i=1, MAXMAILITEM do
		local petButton = getglobal( "MailGetBoxSendItem"..i );
		petButton:DisChecked();
	end
	btn:Checked();
end

function TaskAwardFrameLeftBtnClick( szBtnName )
end

-- ������Ʒ �����Ӧ
function TaskAwardItem_OnClick()
	--�������NPC������ȡ���
	if this:GetParent() == "TaskAwardScrollFrame" then
		TaskAwardFrameLeftButtonOnClick( this:GetClientID() );
	end
end

function TaskAwardItem_SubBtn_OnClick()
	local parentBtn = getglobal( this:GetParent() );
	local nClientID = parentBtn:GetClientID()

	if parentBtn:GetParent() == "TaskAwardScrollFrame" then
		TaskAwardFrameLeftButtonOnClick( nClientID );
	end
end

function TaskAwardItem_OnEnter()
	local Tex = getglobal( this:GetName().."AwardItemTex" );
	Tex:ChangeTextureTemplate("ItemNameHighTemplate");
end

function TaskAwardItem_OnLeave()
	if not this:IsChecked() then
		local Tex = getglobal( this:GetName().."AwardItemTex" );
		Tex:ChangeTextureTemplate("ItemNameNormalTemplate");
	end
end

function TaskAwardItemFont_OnClick()
	--�������NPC������ȡ���
	if string.find( this:GetName(),"ButtonTex", 1, true ) then
		local parent = getglobal( this:GetParent() );
		if parent:GetParent() == "TaskAwardScrollFrame" then	
			TaskAwardFrameLeftButtonOnClick( parent:GetClientID() );
		end
	end
end

function TaskAwardItemFont_OnEnter()
	if string.find( this:GetName(),"ButtonTex", 1, true ) then
		local Tex = getglobal( this:GetParent().."AwardItemTex" );
		Tex:ChangeTextureTemplate("ItemNameHighTemplate");
	end
end

function TaskAwardItemFont_OnLeave()
	if string.find( this:GetName(),"ButtonTex", 1, true ) then
		local parent = getglobal( this:GetParent() );
		if not parent:IsChecked() then
			local Tex = getglobal( parent:GetName().."AwardItemTex" );
			Tex:ChangeTextureTemplate("ItemNameNormalTemplate");
		end
	end
end

function IsArmSafeItem( itemDef, nSafeType )
	for i = 1, MAX_ITEM_RESULT do
		if	itemDef.Result[i-1].ResultID ==  RESULT_NONE then
			return false;
		end

		if ( nSafeType == itemDef.Result[i-1].ResultID ) then
			return true;
		end
	end
end

local t_chgTexPath = { 
	["enter"] = {
		edgePath	= "uires\\ui\\tips\\WuPinMingZi\\WuPinMingChengTouMingHuang7.tga",
		bgPath		= "uires\\ui\\tips\\WuPinMingZi\\WuPinMingChengTouMingHuang 7 Zhong.tga",
		tileAttr	= true,	-- m_bTile
		edgeSize	= 7,	-- m_iEdgeSize
		tileSize	= 1,	-- m_iTileSize
		backgroundInset = { left = 7, right = 7, top = 7, bottom = 7 },	-- m_BackgroundInsets
	},

	["leave"] = {
		edgePath	= "uires\\ui\\tips\\WuPinMingZi\\WuPinMingChengTouMingHei7.tga",		-- m_EdgeTexName
		bgPath		= "uires\\ui\\tips\\WuPinMingZi\\WuPinMingChengTouMingHei 7 Zhong.tga",		-- m_hBorderTex
		tileAttr	= true,	-- m_bTile
		edgeSize	= 7,	-- m_iEdgeSize
		tileSize	= 1,	-- m_iTileSize
		backgroundInset = { left = 7, right = 7, top = 7, bottom = 7 },		-- m_BackgroundInsets
	},
	
	["disable"] = {
		edgePath	= "uires\\ui\\tips\\WuPinMingZi\\WuPinMingChengTouMingHong7.tga",
		bgPath		= "uires\\ui\\tips\\WuPinMingZi\\WuPinMingChengTouMingHong  7  Zhong.tga",
		tileAttr	= true,	-- m_bTile
		edgeSize	= 7,	-- m_iEdgeSize
		tileSize	= 1,	-- m_iTileSize
		backgroundInset = { left = 7, right = 7, top = 7, bottom = 7 },
	}
};

local t_uvTexPath = {
	enter = {
		"uires\\ZhuJieMian\\1.dds", 941, 738, 83, 18
	},
	
	leave = {
		"uires\\ZhuJieMian\\1.dds", 941, 757, 83, 18
	},
	
	disable = {
		"uires\\ZhuJieMian\\1.dds", 941, 776, 83, 18
	},
}

local t_DropPackageCursorControl = { lootByEnter = false, packageID = 0 };

function GetDropPackageCursorControl()
	return t_DropPackageCursorControl;
end

t_DropPackageCursorControl["init"] = 
function ( self )
	self["lootByEnter"] = false;
	self["packageID"]	= 0;
end

t_DropPackageCursorControl["clear"] = 
function ( self )
	self["lootByEnter"] = false;
	self["packageID"]	= 0;
end

t_DropPackageCursorControl["update"] = 
function ( self )
	if not self["lootByEnter"] then
		return;
	end

	local actor = ActorMgr:FindActor( self["packageID"] );
	if actor ~= nil then
		return;
	end
	
	if GetCurrentCursor() == "lootButton" then
		ChangeCursorState( "normal" );
		self["lootByEnter"] = false;
		self["packageID"]	= 0;
	end
end

t_DropPackageCursorControl["setootState"] = 
function ( self, data )
	self["lootByEnter"]		= true;
	self["packageID"]		= data["packageID"];
end

function ItemOutFocus( actorId )

	local actor		= ActorMgr:FindActor( actorId );
	if actor == nil then
		return;
	end

	local szBtnName = actor:getName()..actor:GetID().."��UI����";
	if actor:needAddButtonName() then
		local btn	= FindButton( szBtnName );
		if btn == nil then
			return;
		end
		
		local tex		= getglobal(btn:GetName().."Tex")
		local config	= t_uvTexPath.leave
		tex:SetTexture(config[1])
		tex:SetTexUV(config[2], config[3], config[4], config[5]);
	end

end

function ItemInFocus( actorId )
	local actor		= ActorMgr:FindActor( actorId );
	local szBtnName = actor:getName()..actor:GetID().."��UI����";
	if actor:needAddButtonName() then
		local btn	= FindButton( szBtnName );
		if btn == nil then
			return;
		end
		
		local tex		= getglobal(btn:GetName().."Tex")
		local config	= t_uvTexPath.enter
		tex:SetTexture(config[1])
		tex:SetTexUV(config[2], config[3], config[4], config[5])
	end
end

function enterPackageHeadButton( data )
	local nLastHighlightItemID	= GameMgr:getLastHighlightItemID();
	local btn					= getglobal( data["btnName"] );
	local nNewItemID			= btn:GetClientUserData( 0 );
	assert( nNewItemID ~= 0, "������IDΪ0��ʵ��" );
	if nNewItemID == nLastHighlightItemID then
		return;
	end

	if nLastHighlightItemID ~= 0 then
		ItemOutFocus( nLastHighlightItemID );
		GameMgr:HighlightNewItem( nNewItemID );
	end
	
	local actor		= ActorMgr:FindActor( nNewItemID );
	local szBtnName = actor:getName()..nNewItemID.."��UI����";

	ChangeCursorState( "lootButton" );
	
	t_DropPackageCursorControl:setootState( { packageID = nNewItemID } );
	local tex = getglobal(btn:GetName().."Tex")
	local config = t_uvTexPath.enter
	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5]);
end

function leavePackageHeadButton( data )
	ChangeCursorState( "normal" );
	t_DropPackageCursorControl:clear();
	local btn = getglobal( data["btnName"] );
	t_DropPackageCursorControl:setootState( { packageID = btn:GetClientUserData( 0 ) } );
	local tex = getglobal(btn:GetName().."Tex")
	local config = t_uvTexPath.leave
	tex:SetTexture( config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5])
end

function DropPackageHeadButton_OnEnter()
	--[[
	local t_config = t_chgTexPath["enter"];
	this:SetBackDropBorderTex( t_config["bgPath"] );
	this:SetBackDropEdgeTex( t_config["edgePath"] );
	local t_insetConfig = t_config["backgroundInset"];
	this:SetBackDropAttr( t_config["edgeSize"], t_insetConfig["left"], t_insetConfig["right"], t_insetConfig["top"], t_insetConfig["bottom"] );
	]]

	enterPackageHeadButton( { btnName = this:GetName() } );
end

function DropPackageHeadButton_OnLeave()
	leavePackageHeadButton( { btnName = this:GetName() } );

	--[[
	local t_config = t_chgTexPath["leave"];
	this:SetBackDropBorderTex( t_config["bgPath"] );
	this:SetBackDropEdgeTex( t_config["edgePath"] );
	local t_insetConfig = t_config["backgroundInset"];
	this:SetBackDropAttr( t_config["edgeSize"], t_insetConfig["left"], t_insetConfig["right"], t_insetConfig["top"], t_insetConfig["bottom"] );
	--]]
end

function EnableDropPackageHeadButton(this, enable)
	--[[
	local t_config
	if enable then
		t_config = t_chgTexPath["leave"];
	else
		t_config = t_chgTexPath["disable"];
	end
	this:SetBackDropBorderTex( t_config["bgPath"] );
	this:SetBackDropEdgeTex( t_config["edgePath"] );
	local t_insetConfig = t_config["backgroundInset"];
	this:SetBackDropAttr( t_config["edgeSize"], t_insetConfig["left"], t_insetConfig["right"], t_insetConfig["top"], t_insetConfig["bottom"] );
	]]
	local config
	if enable then
		config = t_uvTexPath.leave
	else
		config = t_uvTexPath.disable
	end
	local tex = getglobal(this:GetName().."Tex")
	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5])
end

function DropPackageHeadButton_OnClick()
	local nActorID	= this:GetClientUserData( 0 );
	if arg1 == "LeftButton"then
		ItemPicker:pickItem(nActorID);
	elseif arg1 == "RightButton"then
		ItemPicker:tryPickItem(nActorID);
	end
end

function CanUseBackClanCityItem()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil or not mainplayer:isInClan() then
		ShowMidTips("�㻹û�м���һ������");
	elseif GameClanManager:getClanCity():getCityInfo().m_nLevel == 0 then
		ShowMidTips("��Ĺ������ڻ�û�г���")
	else
		return true;
	end
	return false;
end
local lastStallId = 0;
-- ̯��ͷ����ť
function StallActorHeadButton_OnEnter()
	HighlightStallNameButton( this:GetName() );
end

function StallActorHeadButton_OnLeave()
	NormalStallNameButton( this:GetName() );
end

function HighlightStallNameButton( btnName )
	local nLastStallActorID	= GameMgr:getLastHighStallActorID();
	local btn				= getglobal( btnName );
	local nNewID			= btn:GetClientUserData( 0 );
	assert( nNewID ~= 0, "������IDΪ0��ʵ��" );
	if nNewID == nLastStallActorID then
		return;
	end
	if lastStallId == nNewID then 
		return;
	end
	if nLastStallActorID ~= 0 then
		StallOutFocus( nLastStallActorID );
		GameMgr:HighlightStallActor( nNewID );
	end
	
	local tex = getglobal(btnName.."Tex")
	local config = t_uvTexPath.enter
	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5]);
end

function NormalStallNameButton( btnName )
	local btn = getglobal( btnName );
	if btn == nil then
		return;
	end
	local nNewID = btn:GetClientUserData( 0 );
	if lastStallId == nNewID then 
		return;
	end
	local tex = getglobal(btnName.."Tex")
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	--[[
	local config = t_uvTexPath.leave
	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5]);
	--]]
end

function StallActorHeadButton_OnClick()
	local nActorID	= this:GetClientUserData( 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	if nActorID == mainplayer:GetID() then
		if not StallSaleFrame:IsShown() then
			StallSaleFrame:Show();
		end
		return;
	end

	local actor			= ActorMgr:FindActor( nActorID );
	local seestalldata	= GameStallManager:getSeeStallItem();
	if seestalldata.m_szRoleName == actor:getName() then
		return;
	end

	GameStallManager:stall_See( nActorID );
end

function StallOutFocus( actorId )
	local actor		= ActorMgr:FindActor( actorId );
	if actor == nil then
		return;
	end
	if lastStallId == actorId then 
		return;
	end
	local szBtnName = actor:getName()..actor:GetID().."��̯λ��ͷ��UI";
	local btn	= FindButton( szBtnName );
	if btn == nil then
		return;
	end
	
	local tex		= getglobal(btn:GetName().."Tex")
	--[[
	local config	= t_uvTexPath.leave
	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5]);
	--]]
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
end

function StallInFocus( actorId )
	local actor		= ActorMgr:FindActor( actorId );
	local szBtnName = actor:getName()..actor:GetID().."��̯λ��ͷ��UI";
	
	if lastStallId == actorId then 
		return;
	end
	local btn	= FindButton( szBtnName );
	if btn == nil then
		return;
	end
	
	local tex		= getglobal(btn:GetName().."Tex")
	local config	= t_uvTexPath.enter

	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5])
	
end

function ChangeSeenStallNameColor( actorId )
	local actor		= ActorMgr:FindActor( actorId );
	local szBtnName = actor:getName()..actor:GetID().."��̯λ��ͷ��UI";
	local btn	= FindButton( szBtnName );
	if btn == nil then
		return;
	end
	local font	= FindFontString( szBtnName.."Font" );
	font:SetTextColor( 255,131,85 );
	
	local tex		= getglobal(btn:GetName().."Tex")
	local config	= t_uvTexPath.enter
	tex:SetTexture(config[1])
	tex:SetTexUV(config[2], config[3], config[4], config[5]);
	if lastStallId ~= actor:GetID() then
		ClearStallNameBack();
	end
	lastStallId = actor:GetID();
end

function ClearStallNameBack()
	local actor		= ActorMgr:FindActor( lastStallId );
	if actor == nil then return ; end
	
	local szBtnName = actor:getName()..actor:GetID().."��̯λ��ͷ��UI";

	local btn	= FindButton( szBtnName );
	if btn == nil then
		return;
	end
	
	local tex		= getglobal(btn:GetName().."Tex")
	tex:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" );
	
	lastStallId = 0;
end