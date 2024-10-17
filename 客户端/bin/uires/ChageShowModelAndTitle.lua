local t_modeTipsDesc = 
{	[HIDE_OTHER_PLAYER_AND_PET]				= { ["desc"] = "�����س��Ｐ�Ƕ�����ҵ����Σ���F12��������", 
												["path"] = [[uires\ui\ModelAndTitle\HidePlayerAndPet.tga]] }, 

	[HIDE_OTHER_PLAYER_AND_PET_AND_STALL]	= { ["desc"] = "������̯λ�����Ｐ�Ƕ�����ҵ����Σ���F12��������", 
												["path"] = [[uires\ui\ModelAndTitle\HidePlayerAndPetAndStall.tga]] }, 
	
	[HIDE_MODEL_NONE]						= { ["desc"] = "����ʾ̯λ�����Ｐ�Ƕ�����ҵ����Σ���F12��������", 
												["path"] = "" }, 
												
};

local t_showOtherPlayerTitleTipsDesc = 
{	[true]				= { ["desc"] = "����ʾ������Ҽ����������֣���F11��������", 
												["path"] = [[uires\ui\ModelAndTitle\HidePlayerAndPet.tga]] }, 

	[false]	= { ["desc"] = "������������Ҽ����������֣���F11��������", 
												["path"] = [[uires\ui\ModelAndTitle\HidePlayerAndPetAndStall.tga]] }, 
												
};

local HIDE_TYPE_MODE	= 0;
local HIDE_TYPE_TITLE	= 1;

local MAX_MODEL_TITLE_BUTTONS = 2;

local t_ChageShowModelAndTitleFrameEvents = 
{ 
	["GE_CHANGE_SHOW_PLAYER_MODEL"] = {}, ["GE_CHANGE_SHOW_PLAYER_TITLE"] = {},	
};

t_ChageShowModelAndTitleFrameEvents["GE_CHANGE_SHOW_PLAYER_MODEL"].func = 
function ()
	UpdateChageShowModelAndTitleFrame();

	local nHideModelOption = ActorMgr:getShowOtherPlayerModelOption();
	AddGameMiddleTips2( t_modeTipsDesc[nHideModelOption]["desc"] );
end

t_ChageShowModelAndTitleFrameEvents["GE_CHANGE_SHOW_PLAYER_TITLE"].func = 
function ()
	UpdateChageShowModelAndTitleFrame();
	
	AddGameMiddleTips2( t_showOtherPlayerTitleTipsDesc[ActorMgr:isShowOtherPlayerTitle()]["desc"] );
end

function ChageShowModelAndTitleFrame_OnLoad()
	for event, _ in pairs( t_ChageShowModelAndTitleFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_ChageShowModelAndTitleFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ChageShowModelAndTitleFrameEvents, t_ChageShowModelAndTitleFrameEvents );
end

function ChageShowModelAndTitleFrame_OnEvent()
	t_ChageShowModelAndTitleFrameEvents[arg1].func();
end

function InitChageShowModelAndTitleFrame()
	ClearChageShowModelAndTitleFrame();
end

function ClearChageShowModelAndTitleFrame()
	for i = 1, MAX_MODEL_TITLE_BUTTONS do
		local btn = getglobal( "ChageShowModelAndTitleFrameButton"..i );
		btn:Hide();
	end
end

function UpdateChageShowModelAndTitleFrame()
	ClearChageShowModelAndTitleFrame();
	local nShowBtnIndex = 1;
	if not ActorMgr:isShowOtherPlayerTitle() then
		local btn		= getglobal( "ChageShowModelAndTitleFrameButton"..nShowBtnIndex );
		local t_tgas	= { [btn:GetName().."IconTexture"] = { path = [[uires\ui\ModelAndTitle\HideTitl.tga]], }, };
		util.SetTgas( t_tgas );
		btn:Show();
		btn:SetClientUserData( 0, HIDE_TYPE_TITLE );
		nShowBtnIndex = nShowBtnIndex + 1
	end
	
	local nHideModelOption = ActorMgr:getShowOtherPlayerModelOption();
	if nHideModelOption == HIDE_MODEL_NONE then
		return;
	end
		
	btn				= getglobal( "ChageShowModelAndTitleFrameButton"..nShowBtnIndex );
	local t_tgas	= { [btn:GetName().."IconTexture"] = { path = t_modeTipsDesc[nHideModelOption]["path"], }, };
	util.SetTgas( t_tgas );
	btn:Show();
	btn:SetClientUserData( 0, HIDE_TYPE_MODE );
	btn:SetClientUserData( 1, nHideModelOption );
end

function ChageShowModelAndTitleButton_OnEnter()
	local szText	= "";
	local nHideType	= this:GetClientUserData( 0 );
	if nHideType == HIDE_TYPE_TITLE then
		szText = t_showOtherPlayerTitleTipsDesc[ActorMgr:isShowOtherPlayerTitle()]["desc"];
	else
		local nHideModeLevel = this:GetClientUserData( 1 );
		szText = t_modeTipsDesc[nHideModeLevel]["desc"];
	end

	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetName(),
						button = this:GetName() } );
end