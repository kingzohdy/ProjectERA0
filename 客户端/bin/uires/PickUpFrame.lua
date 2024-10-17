local nMaxPickUpBtn  = 4;
local nMaxPageNumber = 5;	--当前一共有多少个页面
local nCurPageNumber = 1;	--当前页面
local nCurBootyId = 0;		--当前包裹Id
local nLastBootyId = 0;		--上一次包裹Id
local nBootyCount = 0;		--包裹里的物品数量
local nCurselectIndex = 0;


local t_PickUpButtonInfo =
{
	{ szName=nil,  nLevel=nil, nId=nil, Desc=nil },
	{ szName=nil,  nLevel=nil, nId=nil, Desc=nil },
	{ szName=nil,  nLevel=nil, nId=nil, Desc=nil },
	{ szName=nil,  nLevel=nil, nId=nil, Desc=nil },
	{ szName=nil,  nLevel=nil, nId=nil, Desc=nil },
	{ szName=nil,  nLevel=nil, nId=nil, Desc=nil }
};

--清除上一次显示的内容
function  ClearPickUpButton()
	for i=1, nMaxPickUpBtn do
		local btn = getglobal("PickUpButton"..i);
		if btn:IsShown() then
			btn:Hide();
		end

		local font = getglobal("PickUpButton"..i.."_ItemName_Font");
		font:SetText("");
	end

	if PickUpButtonSelect_Texture:IsShown() then
		PickUpButtonSelect_Texture:Hide();
	end
end

function  PickUpButton_OnEnter()
	--设置Tips
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	local btn = getglobal( "PickUpButton"..this:GetClientID());
	if btn:IsEnable() == 0 then	 
		return;
	end

	local back = getglobal( "PickUpButton".. this:GetClientID() .."Back");
	back:Show();  

	if this:GetClientUserData(0) <= 0 then	
		return;
	end		
	
	local item = container:getBootyItem( this:GetClientUserData(0), this:GetClientUserData(1) );
	if item == nil then				
		return;
	end
	ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEM, item );

	--选中时高亮
	PickUpButtonSelect_Texture:SetPoint("center",this:GetName(),"center",0,0);
	PickUpButtonSelect_Texture:Show();
end

function  PickUpButton_OnLeave()
	PickUpButtonSelect_Texture:Hide();
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
	local back = getglobal("PickUpButton"..this:GetClientID().."Back");
	back:Hide();
end

--双击战利品
function  PickUpButton_OnDoubleClick()
	--HideUIPanel("PickUpFrame");
end

--选中战利品
function PickUpButton_OnClick()
	NormalItemTipsFrame:Hide(); --当获取战利品时将TIPS隐藏
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();   

	nCurBootyId = container:getCurBootyId();
	nBootyCount = container:getBootyCount(nCurBootyId);

	local clientId = this:GetClientID();
	local btn = getglobal("PickUpButton"..( this:GetClientUserData(1)+1));
	local smallfont=getglobal("PickUpButton"..( this:GetClientUserData(1)+1).."CountSmallFont");
	local ItemName = getglobal( "PickUpButton"..( this:GetClientUserData(1)+1).."_ItemName_Font" );
	local Icon = getglobal("PickUpButton"..( this:GetClientUserData(1)+1).."_Item_Texture");

	local back = getglobal("PickUpButton"..( this:GetClientUserData(1)+1).."Back");
	if back == nil then
		return;
	end
	back:Hide();
	if btn == nil or smallfont == nil or Icon == nil then
		return;
	end
	smallfont:SetText( "" );
	ItemName:SetText("");
	Icon:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
	btn:Disable();

	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then
		return; 
	end

	local container = MainPlayer:getContainer();
	if container == nil then
		return; 
	end

	if container:getBootyCount(nCurBootyId) <= 1 then
		PickUpFrame:Hide();
	else
		PickUpFrame:Show();
	end

	container:send_SelectItem( nCurBootyId, (nCurPageNumber-1) * nMaxPickUpBtn + clientId - 1);

end

--全部拾取
function PickUpAll_OnClick()
	local nbtnNum = 1;
	local MainPlayer = ActorMgr:getMainPlayer();
	if MainPlayer == nil then return end
	local container = MainPlayer:getContainer();   

	for nbtnNum =1, nMaxPickUpBtn do
		local smallfont=getglobal("PickUpButton"..nbtnNum.."CountSmallFont");
		local btn = getglobal( "PickUpButton"..nbtnNum );
		if btn == nil then
			return;
		end
		smallfont:SetText( "" );
		btn:Disable();
	end
	local container = MainPlayer:getContainer();
	if container == nil then return end

	container:send_PickAllItem( nCurBootyId );

	nCurPageNumber = 1;
	PickUpFrame:Hide();
end

--上一页面
function  PickUpFramePrevious_OnClick()
	if nCurPageNumber > 1 then
		nCurPageNumber = nCurPageNumber - 1;
		PickUpFrame_OnShow();
	end
end

--下一页面
function  PickUpFrameNext_OnClick()
	if nCurPageNumber < nMaxPageNumber then
		nCurPageNumber = nCurPageNumber + 1;
		PickUpFrame_OnShow();
	end
end

--关闭
function  PickUpFrameClose_OnClick()
	PickUpFrame:Hide();
end

function  PickUpFrame_OnUpdate()
	CurPageNumber_Font:SetText(nCurPageNumber);
	MaxPageNumber_Font:SetText("/ "..nMaxPageNumber);
	--MaxAndCurPageNumber_Font:SetText(nCurPageNumber.." / "..nMaxPageNumber);
end

local t_PickUpFrameEvents = {		
							-- 拾取地上非临时物品
							["GE_BOOTY_GETITEM"]	= {},
							-- 
							["GE_DESTROY_ITEM"]	= {},
							};

t_PickUpFrameEvents["GE_BOOTY_GETITEM"].func = 
function ()
	PickUpFrame:Show();
end

t_PickUpFrameEvents["GE_DESTROY_ITEM"].func = 
function ()
	nCurPageNumber = 1;
end

function PickUpFrame_OnLoad()
	for event, _ in pairs( t_PickUpFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_PickUpFrameEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end
	setmetatable( t_PickUpFrameEvents, t_PickUpFrameEvents );

	for i=1, nMaxPickUpBtn do
		local btn = getglobal( "PickUpButton"..i );
		btn:SetClientUserData(0,0);
		btn:SetClientUserData(1,0);
		btn:Show();
	end
end

--在拾取物品的时候会收到 "GE_BOOTY_GETITEM" 消息
function  PickUpFrame_OnEvnet()
	t_PickUpFrameEvents[arg1].func();
end

function  PickUpFrame_OnShow()
	ClearPickUpButton();
	local MainPlayer = ActorMgr:getMainPlayer();
	local container = MainPlayer:getContainer();

	nCurBootyId = container:getCurBootyId();
	util.UIFrameToShutDown( "PickUpFrame", nCurBootyId, 10000 )
	--当前包裹Id为0时
	--if nCurBootyId == 0 and PickUpFrame:IsShown() and nLastBootyId ~= 0 and nCurBootyId ~= nLastBootyId then
	if nCurBootyId == 0 and PickUpFrame:IsShown() then
		container:closeBooty( nLastBootyId );
		PickUpFrame:Hide();
		nCurPageNumber = 1;
		return;
	end

	nLastBootyId = nCurBootyId;
	nBootyCount = container:getBootyCount(nCurBootyId);

	local nMod = nBootyCount % nMaxPickUpBtn;
	nMaxPageNumber = ( nBootyCount - nMod ) / nMaxPickUpBtn
	if nMod > 0 then
		nMaxPageNumber = nMaxPageNumber + 1;
	end

	if nCurPageNumber > nMaxPageNumber then
		nCurPageNumber = nMaxPageNumber;
	end

	--遍历包裹里的物品
	local nBtnIndex = 1;
	for nBtnIndex = 1, 4 do
		local back = getglobal("PickUpButton"..nBtnIndex.."Back");		
		if back == nil then
			return;
		end
		back:Hide();
		local Btn = getglobal("PickUpButton"..nBtnIndex );	 
		local font = getglobal("PickUpButton"..nBtnIndex.."_ItemName_Font");
		local smallfont=getglobal("PickUpButton"..nBtnIndex.."CountSmallFont");
		local itemUVTex = getglobal("PickUpButton"..nBtnIndex.."UVAnimationTex");
		Btn:Disable();	  
		font:SetText("");
		smallfont:SetText("");
		itemUVTex:Hide();
	end
	nBtnIndex = 1;
	for nGridIndex = ( nCurPageNumber - 1 ) * nMaxPickUpBtn, nBootyCount - 1 do
		local item = container:getBootyItem( nCurBootyId, nGridIndex );
		local btn = getglobal("PickUpButton"..nBtnIndex);
	
		if item ~= nil then
			local itemDef = item:getItemDef();
			if itemDef ~= nil and nBtnIndex <= nMaxPickUpBtn then
				local name = itemDef.Name;
				local Id = itemDef.IconID;
				local num = item:getNum();
				local desc = itemDef.ItemDesc;
				local nBuyPrice = itemDef.BuyPrice;
				local font = getglobal("PickUpButton"..nBtnIndex.."_ItemName_Font");
				local text = getglobal("PickUpButton"..nBtnIndex.."_Item_Texture");
				local smallfont = getglobal("PickUpButton"..nBtnIndex.."CountSmallFont");
				local itemUVTex = getglobal("PickUpButton"..nBtnIndex.."UVAnimationTex");
				SetEquipImportantEffectUV( itemDef, itemUVTex:GetName(),item);
				local IconPath = GetItemEquipIconPath()..Id..".tga";
				-- 如果没找到物品的图标则使用默认图标显示
				if not IsInExistence( IconPath ) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				text:SetTexture( IconPath );

				font:SetText( name );
				if smallfont == nil then
					return;
				end
				if num == 1 then
					smallfont:SetText( "" );
				else
					smallfont:SetText("x"..num);
				end
				btn:SetClientUserData(0,nCurBootyId);
				btn:SetClientUserData(1,nGridIndex);
				btn:Show();
				btn:Enable(); 
				nBtnIndex = nBtnIndex + 1;
			end
		end
	end

	for i = nBtnIndex, nMaxPickUpBtn do
		local btn = getglobal("PickUpButton"..i );
		local text = getglobal("PickUpButton"..i.."_Item_Texture");		
		local smallfont = getglobal("PickUpButton"..i.."CountSmallFont");
		text:SetTexture("uires\\ui\\mask\\TouMingTongDao.tga");
		smallfont:SetText( "" );
		btn:Show();
	end
end

function PickUpFrame_OnHide()
	NormalItemTipsFrame:Hide();
	ArmItemTipsFrame:Hide();
	SecondArmItemTipsFrame:Hide();
	local i = 0;
	for i = 1, 4 do
		local back = getglobal("PickUpButton"..i.."Back");
		if back == nil then
			return;
		end
		back:Hide();
	end
end
