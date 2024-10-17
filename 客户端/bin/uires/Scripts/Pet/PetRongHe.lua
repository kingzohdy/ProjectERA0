-- 宠物融合申请
function CallScript_GetRejectFuseRes( nActorID )
	local actor				= ActorMgr:FindActor( nActorID );
	--ShowMidTips( actor:getName().."拒绝了你的融合请求" );
end

local t_ReqirePetRongHeFrameEvents = 
{	-- ["GE_PET_STATE_CHANGE"] = {},
	["GE_GET_PET_FUSE_INVITE"] = {},	
};

t_ReqirePetRongHeFrameEvents["GE_GET_PET_FUSE_INVITE"].func = 
function ()
	-- 服务器确保一次只会发一个申请给我，不会多于1个
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local inviteFuseData	= petPackage:getPetFuseInviteInfo();
	local inviterPetInfo	= inviteFuseData.Pet;
	local szPetName			= inviterPetInfo.Name;
	local petDef			= getPetDef( inviterPetInfo.PetID );
	if szPetName == "" then
		szPetName = petDef.Name;
	end
	
	local t_riches = 
	{ 
		[this:GetName().."_DescRich"] = { value = "#P"..inviteFuseData.RoleName.."的"..inviterPetInfo.Level..
													"级"..szPetName.."（"..GetPetGeniusValueDesc( inviterPetInfo )..
													"）", color =  { r = 255, g = 255, b = 190 } }, 
	};
	util.SetRiches( t_riches );
	
	local myFightPet	= GetFightPet( mainplayer );
	local myPetInfo		= petPackage:getPetInfo( myFightPet:getRoleWId() );
	local myPetDef		= getPetDef( myPetInfo.PetID );
	szPetName			= myPetInfo.Name;
	if szPetName == "" then
		szPetName = myPetDef.Name;
	end

	local t_fonts = { 
					[this:GetName().."BlackButtonFont"]		= { value = "想与您的"..szPetName.."进行融合，是否同意？", },
					};
	util.SetFonts( t_fonts );

	local t_ReqirePetRongHeFrameControl = GetReqirePetRongHeFrameControl();
	t_ReqirePetRongHeFrameControl:getInvite( { ["name"] = inviteFuseData.RoleName } );
	ReqirePetRongHeFrame:Show();
end

--[[
t_ReqirePetRongHeFrameEvents["GE_PET_STATE_CHANGE"].func = 
function()
	
	local frame = getglobal( "ReqirePetRongHeFrame" );
	if not frame:IsShown() then
		return;
	end

	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	
	local inviteFuseData	= petPackage:getPetFuseInviteInfo();
	petPackage:rejectFuse( inviteFuseData.MemID );
	frame:Hide();
	
end
--]]
-- [name] = { ["startTime"] = 0 }
local t_ReqirePetRongHeFrameControl = { ["inviteList"] = {  }, ["curRole"] = {} };
function GetReqirePetRongHeFrameControl()
	return t_ReqirePetRongHeFrameControl;
end

t_ReqirePetRongHeFrameControl["getInvite"] =
function ( self, data )
	local name		= data["name"];
	local nCurTime	= GameMgr:getTickTime();
	if self:isNoInvite() then
		self["curRole"] = { ["name"] = name, ["startTime"] = nCurTime };
	end
	-- TODO
	--self["inviteList"][name] = { ["startTime"] = nCurTime };
end

t_ReqirePetRongHeFrameControl["getCurProcessInvite"] =
function ( self )
	return self["curRole"];
end

t_ReqirePetRongHeFrameControl["isNoInvite"] =
function ( self )
	return table.getn( self["inviteList"] ) == 0;
end

t_ReqirePetRongHeFrameControl["clearCurInviteInfo"] =
function ( self )
	self["curRole"] = {};
	-- TODO: 检查邀请列表
end

-- 若同时有多个融合邀请怎么办
function ReqirePetRongHeFrame_OnLoad()
	for event, _ in pairs( t_ReqirePetRongHeFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_ReqirePetRongHeFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_ReqirePetRongHeFrameEvents, t_ReqirePetRongHeFrameEvents );
end

function ReqirePetRongHeFrame_OnEvent()
	t_ReqirePetRongHeFrameEvents[arg1].func();
end

function ReqirePetRongHeFrame_OnShow()
end

function ReqirePetRongHeFrame_OnHide()
	t_ReqirePetRongHeFrameControl:clearCurInviteInfo();
end

-- 同意融合邀请
function ReqirePetRongHeFrame_ConfirmBtn_OnClick()
	local frame = getglobal( "ReqirePetRongHeFrame" );
	frame:Hide();

	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local inviteFuseData	= petPackage:getPetFuseInviteInfo();
	local nActorID			= inviteFuseData.MemID;
	local actor				= ActorMgr:FindActor( nActorID );
	
	if actor == nil then
		ShowMidTips( "玩家不在视野内，邀请失效" );
		petPackage:rejectFuse( inviteFuseData.MemID );
		frame:Hide();
		return;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > CLIENT_MAX_RONG_HE_DIST ^ 2 then
		ShowMidTips( "玩家距离太远，邀请失效" );
		petPackage:rejectFuse( inviteFuseData.MemID );
		frame:Hide();
		return;
	end

	local myFightPetActor = GetFightPet( mainplayer );
	if myFightPetActor == nil then
		ShowMidTips( "宠物已被召回，邀请失效" );
		petPackage:rejectFuse( inviteFuseData.MemID );
		frame:Hide();
		return;
	end

	petPackage:acceptFuseInvite( inviteFuseData.MemID );
end

-- 拒绝融合邀请
function ReqirePetRongHeFrame_RefuseBtn_OnClick()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	
	local inviteFuseData	= petPackage:getPetFuseInviteInfo();
	petPackage:rejectFuse( inviteFuseData.MemID );
	local frame = getglobal( "ReqirePetRongHeFrame" );
	frame:Hide();
end

function ReqirePetRongHeFrame_OnUpdate()
	-- TODO: 是否要判断时间和距离
	local t_curProcessRoleInfo = t_ReqirePetRongHeFrameControl:getCurProcessInvite();
	local startTime = t_curProcessRoleInfo["startTime"];
	if ( GameMgr:getTickTime() - startTime >= MAX_INVITE_TIME ) then
		local mainplayer		= ActorMgr:getMainPlayer();
		local petPackage		= mainplayer:getPet();
		
		local inviteFuseData	= petPackage:getPetFuseInviteInfo();
		petPackage:rejectFuse( inviteFuseData.MemID );
		local frame = getglobal( this:GetParent() );
		this:Hide();
		return;
	end
end


-- 正在进行宠物融合
local t_PetRongHeProcessFrameControl = { rongHePlayerID = 0, ["startTime"] = 0, };

function GetPetRongHeProcessFrameControl()
	return t_PetRongHeProcessFrameControl;
end

t_PetRongHeProcessFrameControl["clearRongHeInfo"] =
function ( self )
	self["rongHePlayerID"]	= 0;
	self["startTime"]		= 0;
end

t_PetRongHeProcessFrameControl["getRongHePlayerMemID"] =
function ( self )
	return self["rongHePlayerID"];
end

t_PetRongHeProcessFrameControl["setRongHeInfo"] =
function ( self, data )
	self["rongHePlayerID"]	= data["ID"];
	self["startTime"]		= os.clock();
end

t_PetRongHeProcessFrameControl["updateRongHeTime"] =
function ( self )
	self["startTime"]		= os.clock();
end

t_PetRongHeProcessFrameControl["isAnyPetRongHe"] =
function ( self )
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local selfRongHePetWID	= petPackage:getSelfRongHePetWID();

	return not ( selfRongHePetWID.nHeight == 0 and selfRongHePetWID.nLow == 0 );
end

t_PetRongHeProcessFrameControl["isRongHePet"] =
function ( self, data )
	if not self:isAnyPetRongHe() then
		return false;
	end
	
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local selfRongHePetWID	= petPackage:getSelfRongHePetWID();

	return data["heightId"] == selfRongHePetWID.nHeight and data["lowId"] == selfRongHePetWID.nLow;
end

local t_PetRongHeProcessFrameEvents = 
{ 
	["GE_GET_OTHER_ACCEPT_INVITE_RES"] = {},	["GE_CHANGE_MONEY"] = {},
};

t_PetRongHeProcessFrameEvents["GE_GET_OTHER_ACCEPT_INVITE_RES"].func = 
function ()
	PetRongHeProcessFrame:Show();
end

t_PetRongHeProcessFrameEvents["GE_CHANGE_MONEY"].func = 
function ()
	if PetRongHeProcessFrame:IsShown() then
		UpdateMoneyAndSuccess();
	end
end

function PetRongHeProcessFrame_OnLoad()
	for event, _ in pairs( t_PetRongHeProcessFrameEvents ) do
		this:RegisterEvent( event );
	end

	t_PetRongHeProcessFrameEvents.__index = function ( tab, key )
									tab[key] = { func = function () end };
									return tab[key];
								end
	setmetatable( t_PetRongHeProcessFrameEvents, t_PetRongHeProcessFrameEvents );
end

function PetRongHeProcessFrame_OnEvent()
	t_PetRongHeProcessFrameEvents[arg1].func();
end

-- 刷新左边自己的宠物信息
function UpdatePetRongHeProcessFrameSelfPetInfo()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local selfRongHePetWID	= petPackage:getSelfRongHePetWID();
	local selfPetInfo		= petPackage:getPetInfo( selfRongHePetWID.nHeight, selfRongHePetWID.nLow );
	local szPetName			= selfPetInfo.Name;
	local petDef			= getPetDef( selfPetInfo.PetID );
	if szPetName == "" then
		szPetName = petDef.Name;
	end

	local t_fonts = {	["PetRongHeProcessFrameLeftPetBackTopHeadBtnFont"] = { value = mainplayer:getName().."的宠物" },

						["PetRongHeProcessFrameLeftPetNameBtnFont"] = { value = "名称："..szPetName },

						["PetRongHeProcessFrameLeftPetLevelBtnFont"] = { value = "等级："..selfPetInfo.Level },
						["PetRongHeProcessFrameLeftPetGeniusBtnFont"] = { value = "资质："..GetPetGeniusValueDesc( selfPetInfo ) },
						["PetRongHeProcessFrameLeftPetFightBtnFont"] = { value = "战斗力："..GetPetFightValue( selfPetInfo ) },
					};
	util.SetFonts( t_fonts );

	local t_textures =	{ 
							["PetRongHeProcessFrameLeftPetDaiShuBtnTex"]		= t_petDaiShu[petDef.Type][selfPetInfo.Hierarchy],
						};
	util.SetTextures( t_textures );

	local t_modelViews = 
	{ ["PetRongHeProcessFrameLeftPetModelBtnModelView"] = 
	{	path = GetModePathById( petDef.MouldID ), animId = 10100, 
		scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size 
		} 
	};

	util.SetModelView( t_modelViews );
end

-- 刷新右边对方的宠物信息
function UpdatePetRongHeProcessFrameAnotherPetInfo()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local rongHePlayerInfo	= petPackage:getRongHePlayerInfo();
	local petInfo			= rongHePlayerInfo.Pet;
	local szPetName			= petInfo.Name;
	local petDef			= getPetDef( petInfo.PetID );
	if szPetName == "" then
		szPetName = petDef.Name;
	end

	local t_fonts = {	["PetRongHeProcessFrameRightPetBackTopHeadBtnFont"] = { value = rongHePlayerInfo.RoleName.."的宠物" },

						["PetRongHeProcessFrameRightPetNameBtnFont"] = { value = "名称："..szPetName },

						["PetRongHeProcessFrameRightPetLevelBtnFont"] = { value = "等级："..petInfo.Level },
						["PetRongHeProcessFrameRightPetGeniusBtnFont"] = { value = "资质："..GetPetGeniusValueDesc( petInfo ) },
						["PetRongHeProcessFrameRightPetFightBtnFont"] = { value = "战斗力："..GetPetFightValue( petInfo ) },
					};
	util.SetFonts( t_fonts );

	local t_textures =	{ 
							["PetRongHeProcessFrameRightPetDaiShuBtnTex"]		= t_petDaiShu[petDef.Type][petInfo.Hierarchy],
						};
	util.SetTextures( t_textures );

	local t_modelViews = 
	{ ["PetRongHeProcessFrameRightPetModelBtnModelView"] = 
	{	path = GetModePathById( petDef.MouldID ), animId = 10100, 
		scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size 
		} 
	};

	util.SetModelView( t_modelViews );

end

local t_successProbability =
{
-- 优秀
[CLIENT_PET_GENIUS_B] = { value = 50 },
-- 杰出
[CLIENT_PET_GENIUS_A] = { value = 70 },
-- 完美
[CLIENT_PET_GENIUS_S] = { value = 90 },
};
-- 刷新金钱成功率
function UpdateMoneyAndSuccess()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local selfRongHePetWID	= petPackage:getSelfRongHePetWID();
	local selfPetInfo		= petPackage:getPetInfo( selfRongHePetWID.nHeight, selfRongHePetWID.nLow );
	
	local rongHePlayerInfo	= petPackage:getRongHePlayerInfo();
	local playerPetInfo		= rongHePlayerInfo.Pet;
	
	local successProbability= 0;
	if t_successProbability[selfPetInfo.PetGenius] == nil then
		successProbability = successProbability + 50;
	else
		successProbability = successProbability + t_successProbability[selfPetInfo.PetGenius]["value"];
	end

	if t_successProbability[playerPetInfo.PetGenius] == nil then
		successProbability = successProbability + 50;
	else
		successProbability = successProbability + t_successProbability[playerPetInfo.PetGenius]["value"];
	end

	successProbability = math.floor( successProbability / 2 );
	local petLevelUpDef = getLevelUP( selfPetInfo.Level );
	-- TODO: 问策划怎么算

	local nMoney = math.max( 3, petLevelUpDef.ContributeMoney / 2 );
	nMoney = math.floor( nMoney );
	nMoney = nMoney / 100 * 100;
	local t_fonts = {	["PetRongHeProcessFrameSuccessProbabilityValueBtnFont"] = { value = successProbability.."%" },

						["PetRongHeProcessFrameCostMoneyValueBtnFont"] = { value = nMoney },
					};
	util.SetFonts( t_fonts );

	if getPlayerMoney( mainplayer ) < nMoney then
		PetRongHeProcessFrameNotEnoughMoneyBtn:Show();
	else
		PetRongHeProcessFrameNotEnoughMoneyBtn:Hide();
	end
end

function ClearPetRongHeProcessFrame()
	local t_hideIcons = { "PetRongHeProcessFrameLeftPetBackBtnMaskTex", "PetRongHeProcessFrameRightPetBackBtnMaskTex" };
	util.HideIcons( t_hideIcons );

	local btn = getglobal( "PetRongHeProcessFrameRongHeBtn" );
	PetRongHeProcessFrameRongHeBtn:SetGray( false );
	PetRongHeProcessFrameRongHeBtn:Enable();
end

function CallScript_GetDealFuseRes( nActorID )
	local mainplayer		= ActorMgr:getMainPlayer();
	if nActorID == mainplayer:GetID() then
		PetRongHeProcessFrameLeftPetBackBtnMaskTex:Show();
		PetRongHeProcessFrameRongHeBtn:SetGray( true );
		PetRongHeProcessFrameRongHeBtn:Disable();
	else
		PetRongHeProcessFrameRightPetBackBtnMaskTex:Show();
	end
end

local t_PetRongHeProcessFrameMutexFrame = { "PetRongHeSuccessFrame" };
function PetRongHeProcessFrame_OnShow()
	for _, name in ipairs( t_PetRongHeProcessFrameMutexFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end

	ClearPetRongHeProcessFrame();
	-- 刷新左边自己的宠物信息
	UpdatePetRongHeProcessFrameSelfPetInfo();
	-- 刷新右边对方的宠物信息
	UpdatePetRongHeProcessFrameAnotherPetInfo();
	-- 刷新金钱成功率
	UpdateMoneyAndSuccess();

	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local rongHePlayerInfo	= petPackage:getRongHePlayerInfo();
	local actor				= ActorMgr:findActorByName( rongHePlayerInfo.RoleName );
	t_PetRongHeProcessFrameControl:setRongHeInfo( { ID = actor:GetID() } );
	
	local selfRongHePetWID	= petPackage:getSelfRongHePetWID();
	local selfPetInfo		= petPackage:getPetInfo( selfRongHePetWID.nHeight, selfRongHePetWID.nLow );
	local t_PetRongHeSuccessFrameControl = GetPetRongHeSuccessFrameControl();
	t_PetRongHeSuccessFrameControl:setSelfPetBeforeInfo( {	["attack"]		= GetAttackGeniusValue( selfPetInfo ), 
															["life"]		= selfPetInfo.HPGenius,
															["physicDef"]	= selfPetInfo.DefGenius,				
															["magicDef"]	= selfPetInfo.IceDefGenius,
															["hitRate"]		= selfPetInfo.HitRateGenius, 
															["flee"]		= selfPetInfo.FleeGenius 
														} 
														);
	local rongHePlayerInfo	= petPackage:getRongHePlayerInfo();
	local anotherPetInfo	= rongHePlayerInfo.Pet;
	t_PetRongHeSuccessFrameControl:setAnotherPetBeforeInfo( {	["attack"]		= GetAttackGeniusValue( anotherPetInfo ), 
																["life"]		= anotherPetInfo.HPGenius,
																["physicDef"]	= anotherPetInfo.DefGenius,				
																["magicDef"]	= anotherPetInfo.IceDefGenius,
																["hitRate"]		= anotherPetInfo.HitRateGenius, 
																["flee"]		= anotherPetInfo.FleeGenius 
															} 
															);
	SynchronizeMainPetFrame();

end

function LogRongHePetInfo( data )
	local szDesc	= data["text"];
	local petInfo	= data["petInfo"];
	logPrivateInfo( szDesc.."petName = "..petInfo.Name.."\nattack = "..GetAttackGeniusValue( petInfo ).."\nlife = "..petInfo.HPGenius..	
					"\nphysicDef = "..petInfo.DefGenius.."\nmagicDef = "..petInfo.IceDefGenius.."\nhitRate = "..petInfo.HitRateGenius..
					"\nflee = "..petInfo.FleeGenius.."\n" );
end

function SynchronizeMainPetFrame()
	if not PetFrame:IsShown() then
		return;
	end

	UpdateCallBackBtn();
end

function PetRongHeProcessFrame_OnHide()
	t_PetRongHeProcessFrameControl:clearRongHeInfo();

	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	petPackage:clearSelfRongHePetInfo();

	SynchronizeMainPetFrame();
end

local ACTOR_NOT_IN_VIEW_TO_RONG_HE					= 0;
local DIST_TOO_LONG_TO_RONG_HE						= 1;
local ACTOR_NOT_IN_VIEW_TO_ACCEPT_RONG_HE_INVITE	= 2;
local DIST_TOO_LONG_TO_ACCEPT_RONG_HE_INVITE		= 3;

-- 查看距离是否太远
function PetRongHeProcessFrame_OnUpdate()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local nActorID			= t_PetRongHeProcessFrameControl["rongHePlayerID"];
	local actor				= ActorMgr:FindActor( nActorID );

	if actor == nil then
		ShowMidTips( "进行融合的玩家不在视野内，取消融合" );
		petPackage:cancelFuse( nActorID );
		this:Hide();
		return;
	end

	local dist = util.CalActorsDistance( actor, mainplayer );
	if dist > CLIENT_MAX_RONG_HE_DIST ^ 2 then
		ShowMidTips( "进行融合的玩家距离太远，取消融合" );
		petPackage:cancelFuse( nActorID );
		this:Hide();
		return;
	end

	-- 再计算是否超时
	local nCurTime		= os.clock();
	local nStartTime	= t_PetRongHeProcessFrameControl["startTime"];
	if nCurTime - nStartTime >= CLIENT_MAX_RONG_HE_TIME then
		ShowMidTips( "融合时间太长，取消融合" );
		petPackage:cancelFuse( nActorID );
		this:Hide();
		return;
	end
end

function PetRongHeProcessFrame_SuccessProbabilityBtn_OnEnter()
	local szText = "融合成功率取决于两只宠物的资质，资质越高成功率越高\n优秀：50%\n杰出：70%\n完美：90%";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
					button = this:GetName(), vertical = "bottom" } );
end

function PetRongHeProcessFrame_HelpInfoBtn_OnEnter()
	local szText = "1.宠物等级必须达到50以上才能够融合\n2.只有一代宠物和变异宠物能够进行融合\n3.变异宠物只能与同种类的变异宠物融合\n4.每只一代宠物、变异宠物只能融合一次\n5.宠物的资质必须达到“优秀”才能融合";
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = this:GetParent(),
					button = this:GetName(), vertical = "bottom" } );
end

-- 同意融合
function PetRongHeProcessFrame_RongHeBtn_OnClick()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local nActorID			= t_PetRongHeProcessFrameControl["rongHePlayerID"];
	petPackage:agreeFuse( nActorID );
end

-- 取消融合
function PetRongHeProcessFrame_CancelBtn_OnClick()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local nActorID			= t_PetRongHeProcessFrameControl["rongHePlayerID"];
	petPackage:cancelFuse( nActorID );
	PetRongHeProcessFrame:Hide();
end

function CallScript_GetCancelFuseRes( nMemID )
	local frame = getglobal( "PetRongHeProcessFrame" );
	if frame:IsShown() then
		frame:Hide();
	end
end

local t_petRongHe = 
{
[CLIENT_PET_FUSE_FAIL]		= { path = [[effect\entity\cwhc02.ent]], modelView = "PetRongHeProcessFrameResultModelBtnModelView", 
								frame = "PetRongHeProcessFrame", desc = "融合失败" },
[CLIENT_PET_FUSE_SUCCESS]	= { path = [[effect\entity\cwhc01.ent]], modelView = "PetRongHeSuccessFrameResultModelBtnModelView",
								frame = "PetRongHeSuccessFrame", desc = "融合成功" },

};

function Private_GetFailFuseRes()
	t_PetRongHeProcessFrameControl:updateRongHeTime();
	local t_petRongHeFrameInfo = t_petRongHe[CLIENT_PET_FUSE_FAIL];
	ShowMidTips( t_petRongHeFrameInfo["desc"] );
	local frame = getglobal( t_petRongHeFrameInfo["frame"] );
	if not frame:IsShown() then
		return;
	end

	local modelView = getglobal( t_petRongHeFrameInfo["modelView"] );
	local path		= t_petRongHeFrameInfo["path"];
	modelView:setPetModel( 0, 0, path );
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();

	ClearPetRongHeProcessFrame();
end

function Private_GetSuccessFuseRes()
	PetRongHeSuccessFrame:Show();
	local t_petRongHeFrameInfo	= t_petRongHe[CLIENT_PET_FUSE_SUCCESS];
	ShowMidTips( t_petRongHeFrameInfo["desc"] );
	local modelView				= getglobal( t_petRongHeFrameInfo["modelView"] );
	local path					= t_petRongHeFrameInfo["path"];
	modelView:setPetModel( 0, 0, path );
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();
end

function CallScript_GetResultFuseRes( nFlag )
	if nFlag == CLIENT_PET_FUSE_SUCCESS then
		Private_GetSuccessFuseRes();
	elseif nFlag == CLIENT_PET_FUSE_FAIL then
		Private_GetFailFuseRes();
	end
end

function PetRongHeSuccessFrame_Test_OnShow()
	--[[
	local modelView = getglobal( "PetRongHeSuccessFrameResultModelBtnModelView" );
	local path		= t_petRongHe[CLIENT_PET_FUSE_FAIL]["path"];
	modelView:setPetModel( 0, 0, path );
	modelView:setEntityPosition(0,0,100,0);
	modelView:Show();
	--]]

	local petDef = getPetDef( 4 );
	local t_modelViews = 
	{ ["PetRongHeSuccessFrameRightPetModelBtnModelView"] = 
	{	path = GetModePathById( petDef.MouldID ), animId = 10100, 
		scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size 
		} 
	};
	util.SetModelView( t_modelViews );
end

-- 宠物天赋图标
function RongHePetGeniusButton_OnEnter()
end

local RONG_HE_ATTR_ADD = 0;
local RONG_HE_ATTR_DEC = 1;

local t_rongHeAttrChgColor = 
{
[RONG_HE_ATTR_ADD] = { r = 0,	g = 255, b = 0 },
[RONG_HE_ATTR_DEC] = { r = 255, g = 0, b = 0 },

};

-- 融合成功后
-- 刷新自己这边的宠物信息
function UpdatePetRongHeSuccessFrameSelfPetInfo()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local successRongHeInfo	= petPackage:getSuccessFuseInfo()
	local petInfo;
	if successRongHeInfo.MemID == mainplayer:GetID() then
		petInfo = successRongHeInfo.Pet;
	else
		petInfo = successRongHeInfo.OtherPet;
	end

	local petDef			= getPetDef( petInfo.PetID );
	local szFrameName		= "PetRongHeSuccessFrame";
	
	local t_PetRongHeSuccessFrameControl = GetPetRongHeSuccessFrameControl();
	local t_beforeRongHeSelfPetInfo	= t_PetRongHeSuccessFrameControl["beforeRongHe"]["self"];
	local nChgAttackGenius			= GetAttackGeniusValue( petInfo )	- t_beforeRongHeSelfPetInfo["attack"];
	local nChgLifeGenius			= petInfo.HPGenius					- t_beforeRongHeSelfPetInfo["life"];
	local nChgPhysicDefGenius		= petInfo.DefGenius					- t_beforeRongHeSelfPetInfo["physicDef"];
	local nChgMagicDefGenius		= petInfo.IceDefGenius				- t_beforeRongHeSelfPetInfo["magicDef"];
	local nChgHitRateGenius			= petInfo.HitRateGenius				- t_beforeRongHeSelfPetInfo["hitRate"];
	local nChgFleeGenius			= petInfo.FleeGenius				- t_beforeRongHeSelfPetInfo["flee"];
	local t_fonts = 
	{	
		-- 抬头拥有者信息
		[szFrameName.."LeftPetBackTopHeadBtnFont"]		= { value = mainplayer:getName().."的宠物" },
		-- 宠物资质
		[szFrameName.."LeftPetZiZhiValueBtnFont"]		= { value = GetPetGeniusValueDesc( petInfo ), 
															color = GetPetGeniusColor( petInfo ),  },
		-- 攻击成长
		[szFrameName.."LeftPetAttackValueBtnFont"]		= 
		{	value = ( nChgAttackGenius > 0 and "+" or ( nChgAttackGenius < 0 and "-" or "" ) )..nChgAttackGenius,
			color =  ( nChgAttackGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 生命成长值
		[szFrameName.."LeftPetLifeValueBtnFont"]		= 
		{	value = ( nChgLifeGenius > 0 and "+" or ( nChgLifeGenius < 0 and "-" or "" ) )..nChgLifeGenius,
			color =  ( nChgLifeGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 物防成长值
		[szFrameName.."LeftPetPhysicDefValueBtnFont"]	= 
		{	value = ( nChgPhysicDefGenius > 0 and "+" or ( nChgPhysicDefGenius < 0 and "-" or "" ) )..nChgPhysicDefGenius,
			color =  ( nChgPhysicDefGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 魔防成长值
		[szFrameName.."LeftPetMagicDefValueBtnFont"]	= 
		{	value = ( nChgMagicDefGenius > 0 and "+" or ( nChgMagicDefGenius < 0 and "-" or "" ) )..nChgMagicDefGenius,
			color =  ( nChgMagicDefGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 命中成长值
		[szFrameName.."LeftPetHitRateValueBtnFont"]	= 
		{	value = ( nChgHitRateGenius > 0 and "+" or ( nChgHitRateGenius < 0 and "-" or "" ) )..nChgHitRateGenius,
			color =  ( nChgHitRateGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 闪避成长值
		[szFrameName.."LeftPetFleeValueBtnFont"]	= 
		{	value = ( nChgFleeGenius > 0 and "+" or ( nChgFleeGenius < 0 and "-" or "" ) )..nChgFleeGenius,
			color =  ( nChgFleeGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },
	};
	util.SetFonts( t_fonts );

	local t_textures =	{ 
							[szFrameName.."LeftPetDaiShuBtnTex"]		= t_petDaiShu[petDef.Type][petInfo.Hierarchy],
						};
	util.SetTextures( t_textures );

	local t_modelViews = 
	{ [szFrameName.."LeftPetModelBtnModelView"] = 
	{	path = GetModePathById( petDef.MouldID ), animId = 10100, 
		scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size 
		} 
	};
	util.SetModelView( t_modelViews );

	local geniusSkillInfo = petInfo.NativeSkill;
	local btn			= getglobal( szFrameName.."LeftPetGeniusBtn" );
	if geniusSkillInfo.SkillID ~= 0 then
		local diathesisiDef = getDiathesisDef( geniusSkillInfo.SkillID, geniusSkillInfo.SkillLevel );
		local IconPath = getPetSkillPath()..diathesisiDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		local t_tgas = { [btn:GetName().."IconTex"] = { path = IconPath }, }
		util.SetTgas( t_tgas );

		btn:SetClientUserData( 0, geniusSkillInfo.SkillID );
		btn:SetClientUserData( 1, geniusSkillInfo.SkillLevel );
		btn:SetClientUserData( 2, geniusSkillInfo.GridType );
		btn:Enable();
	else
		local t_textures = { [btn:GetName().."IconTex"] = 
							{ path = "uires\\ZhuJieMian\\2.dds", UV = { x = 182, y = 513, width = 44, height = 44 }, }, };
		util.SetTextures( t_textures );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		btn:Disable();
	end
end

-- 刷新对方的宠物信息
function UpdatePetRongHeSuccessFrameAnotherPetInfo()
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local successRongHeInfo	= petPackage:getSuccessFuseInfo()
	local petInfo			= successRongHeInfo.OtherPet;
	local nActorID			= 0;
	if successRongHeInfo.MemID == mainplayer:GetID() then
		petInfo = successRongHeInfo.OtherPet;
		nActorID= successRongHeInfo.OtherMemID;	
	else
		petInfo = successRongHeInfo.Pet;
		nActorID= successRongHeInfo.MemID;
	end

	local petDef			= getPetDef( petInfo.PetID );
	local szFrameName		= "PetRongHeSuccessFrame";
	local actor				= ActorMgr:FindActor( nActorID );
	
	local t_PetRongHeSuccessFrameControl	= GetPetRongHeSuccessFrameControl();
	local t_beforeRongHeAnotherPetInfo		= t_PetRongHeSuccessFrameControl["beforeRongHe"]["another"];
	local nChgAttackGenius			= GetAttackGeniusValue( petInfo )	- t_beforeRongHeAnotherPetInfo["attack"];
	local nChgLifeGenius			= petInfo.HPGenius					- t_beforeRongHeAnotherPetInfo["life"];
	local nChgPhysicDefGenius		= petInfo.DefGenius					- t_beforeRongHeAnotherPetInfo["physicDef"];
	local nChgMagicDefGenius		= petInfo.IceDefGenius				- t_beforeRongHeAnotherPetInfo["magicDef"];
	local nChgHitRateGenius			= petInfo.HitRateGenius				- t_beforeRongHeAnotherPetInfo["hitRate"];
	local nChgFleeGenius			= petInfo.FleeGenius				- t_beforeRongHeAnotherPetInfo["flee"];
	
	local t_fonts = 
	{	
		-- 抬头拥有者信息
		[szFrameName.."RightPetBackTopHeadBtnFont"]		= { value = actor:getName().."的宠物" },
		-- 宠物资质
		[szFrameName.."RightPetZiZhiValueBtnFont"]		= { value = GetPetGeniusValueDesc( petInfo ), 
																		color = GetPetGeniusColor( petInfo ),  },
		-- 攻击成长
		[szFrameName.."RightPetAttackValueBtnFont"]		= 
		{	value = ( nChgAttackGenius > 0 and "+" or ( nChgAttackGenius < 0 and "-" or "" ) )..nChgAttackGenius,
			color =  ( nChgAttackGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 生命成长值
		[szFrameName.."RightPetLifeValueBtnFont"]		= 
		{	value = ( nChgLifeGenius > 0 and "+" or ( nChgLifeGenius < 0 and "-" or "" ) )..nChgLifeGenius,
			color =  ( nChgLifeGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 物防成长值
		[szFrameName.."RightPetPhysicDefValueBtnFont"]	= 
		{	value = ( nChgPhysicDefGenius > 0 and "+" or ( nChgPhysicDefGenius < 0 and "-" or "" ) )..nChgPhysicDefGenius,
			color =  ( nChgPhysicDefGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 魔防成长值
		[szFrameName.."RightPetMagicDefValueBtnFont"]	= 
		{	value = ( nChgMagicDefGenius > 0 and "+" or ( nChgMagicDefGenius < 0 and "-" or "" ) )..nChgMagicDefGenius,
			color =  ( nChgMagicDefGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 命中成长值
		[szFrameName.."RightPetHitRateValueBtnFont"]	= 
		{	value = ( nChgHitRateGenius > 0 and "+" or ( nChgHitRateGenius < 0 and "-" or "" ) )..nChgHitRateGenius,
			color =  ( nChgHitRateGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },

		-- 闪避成长值
		[szFrameName.."RightPetFleeValueBtnFont"]	= 
		{	value = ( nChgFleeGenius > 0 and "+" or ( nChgFleeGenius < 0 and "-" or "" ) )..nChgFleeGenius,
			color =  ( nChgFleeGenius < 0 and t_rongHeAttrChgColor[RONG_HE_ATTR_DEC] or t_rongHeAttrChgColor[RONG_HE_ATTR_ADD] ) },
	};
	util.SetFonts( t_fonts );

	local t_textures =	{ 
							[szFrameName.."RightPetDaiShuBtnTex"]		= t_petDaiShu[petDef.Type][petInfo.Hierarchy],
						};
	util.SetTextures( t_textures );

	local t_modelViews = 
	{ [szFrameName.."RightPetModelBtnModelView"] = 
	{	path = GetModePathById( petDef.MouldID ), animId = 10100, 
		scale = GetNpcDisplayInfo( petDef.MouldID ).size == 0 and 1 or GetNpcDisplayInfo( petDef.MouldID ).size 
		} 
	};
	util.SetModelView( t_modelViews );

	local geniusSkillInfo = petInfo.NativeSkill;
	local btn			= getglobal( szFrameName.."RightPetGeniusBtn" );
	if geniusSkillInfo.SkillID ~= 0 then
		local diathesisiDef = getDiathesisDef( geniusSkillInfo.SkillID, geniusSkillInfo.SkillLevel );
		local IconPath = getPetSkillPath()..diathesisiDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		local t_tgas = { [btn:GetName().."IconTex"] = { path = IconPath }, }
		util.SetTgas( t_tgas );

		btn:SetClientUserData( 0, geniusSkillInfo.SkillID );
		btn:SetClientUserData( 1, geniusSkillInfo.SkillLevel );
		btn:SetClientUserData( 2, PET_SKILL_SELF_BUFF );
		btn:Enable();
	else
		local t_textures = { [btn:GetName().."IconTex"] = 
							{ path = "uires\\ZhuJieMian\\2.dds", UV = { x = 182, y = 513, width = 44, height = 44 }, }, };
		util.SetTextures( t_textures );
		btn:SetClientUserData( 0, 0 );
		btn:SetClientUserData( 1, 0 );
		btn:SetClientUserData( 2, 0 );
		btn:Disable();
	end
end

local t_PetRongHeSuccessFrameControl = { ["beforeRongHe"] = {  ["self"]={}, ["another"]={} }, };

function GetPetRongHeSuccessFrameControl()
	return t_PetRongHeSuccessFrameControl;
end

t_PetRongHeSuccessFrameControl["setSelfPetBeforeInfo"] = 
function ( self, data )
	self["beforeRongHe"]["self"]["attack"]		= data["attack"];
	self["beforeRongHe"]["self"]["life"]		= data["life"];
	self["beforeRongHe"]["self"]["physicDef"]	= data["physicDef"];
	self["beforeRongHe"]["self"]["magicDef"]	= data["magicDef"];
	self["beforeRongHe"]["self"]["hitRate"]		= data["hitRate"];
	self["beforeRongHe"]["self"]["flee"]		= data["flee"];
end

t_PetRongHeSuccessFrameControl["setAnotherPetBeforeInfo"] = 
function ( self, data )
	self["beforeRongHe"]["another"]["attack"]		= data["attack"];
	self["beforeRongHe"]["another"]["life"]			= data["life"];
	self["beforeRongHe"]["another"]["physicDef"]	= data["physicDef"];
	self["beforeRongHe"]["another"]["magicDef"]		= data["magicDef"];
	self["beforeRongHe"]["another"]["hitRate"]		= data["hitRate"];
	self["beforeRongHe"]["another"]["flee"]			= data["flee"];
end

local t_PetRongHeSuccessFrameMutexFrame = { "PetRongHeProcessFrame" };
function PetRongHeSuccessFrame_OnShow()
	for _, name in ipairs( t_PetRongHeSuccessFrameMutexFrame ) do
		local frame = getglobal( name );
		if frame:IsShown() then
			frame:Hide();
		end
	end
	UpdatePetRongHeSuccessFrameSelfPetInfo();
	UpdatePetRongHeSuccessFrameAnotherPetInfo();
end