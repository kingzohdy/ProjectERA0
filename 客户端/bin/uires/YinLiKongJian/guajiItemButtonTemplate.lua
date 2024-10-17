
-- 设置装置特效
function SetEquipImportantEffectUV( itemdef, frame,item )
	local ItemTex = getglobal( frame );

	if itemdef == nil then
		ItemTex:Hide();
		return;
	end

	local nIndex = 4;

	if (item ~= nil and item:getStart() >= 3) or itemdef.ItemStar >=3 then
		ItemTex:SetUVAnimation(100, true);
		nIndex = nIndex - 4;
	end	

	if itemdef.Important == ITEM_IMPORTANT_NORMAL then
		ItemTex:Hide();
		return;
	end
	if itemdef.Important == ITEM_IMPORTANT_EXCELLENCE then
		nIndex = nIndex + 1;
	end

	if itemdef.Important == ITEM_IMPORTANT_BEST then
		nIndex = nIndex + 2;
	end

	if itemdef.Important == ITEM_IMPORTANT_NONSUCH then
		nIndex = nIndex + 3;
	end
	ItemTex:SetTexture( "uires\\ui\\TeXiao\\ZhuangBei  "..nIndex..".tga" );
	if nIndex < 4 then
		ItemTex:SetUVAnimation(100, true);
	else
		ItemTex:StopUVAnim();
		ItemTex:Show();
	end
end

function ItemButton_OnLoad()
	this:SetClientUserData( 0, 0 );
	this:SetClientUserData( 1, 0 );
	this:SetClientUserData( 2, 0 );
	this:SetClientUserData( 3, 0 );
end

function ItemButton_OnEnter()
	-- 如果是在观察对方装备
	if this:GetParent() == "WatchPlayerFrame" then
		WatchPlayerOnEnter( this:GetName() );
		return;
	end
end

function ItemButton_OnLeave()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	GameTooltip:Hide();
	SpellTipsFrame:Hide();
end

function WatchPlayerOnEnter( szBtnName )
	local btn = getglobal( szBtnName );
	local Item = OfflineWatchPlayer:getItem( btn:GetClientID() );
	if Item:getItemId() == 0 then
			ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = t_WearPosInfo[btn:GetClientID()], frame = this:GetParent(),
						button = btn:GetName() } )
		return;
	end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, Item, WatchPlayerOnEnter, szBtnName );
end