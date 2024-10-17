MAX_MAIN_GOOD_BUF_COUNT = 35; 
MAX_MAIN_DEBUF_COUNT	= 8;

STATUS_TYPE_GOOD	= 0; 	--/* 良性状态 */
STATUS_TYPE_BAD		= 1; 	--/* 不良状态 */
STATUS_TYPE_OTHER	= 2; 	--/* 其他状态，不会被顶替 */

MAX_GOOD_STATUS		= 16;
MAX_BAD_STATUS		= 8;
MAX_NOLIMIT_STATUS	= 12;

BUFF_TYPE_RIDE		= -1;
BUFF_TYPE_PET		= -2;
BUFF_TYPE_NORMAL	= -3;
BUFF_TYPE_FAIRY		= -4;
BUFF_TYPE_SAFE		= -5;
BUFF_TYPE_PET_GUAN_ZHI	= -6;
BUFF_TYPE_HAI_TAN	= -7;
BUFF_TYPE_BATTLE_LIMIT	= -8;
BUFF_TYPE_PET_HETI	= -9;
BUFF_TYPE_CLAN_SKILL	= -10;
BUFF_TYPE_PK		= -11;
BUFF_TYPE_SPAN_STRONG_OWNER		= -12;

local PK_BUFF_ID = 3183;

local BEACH_BUFF_ID = 3187;
local BeachBuffIn 	= 0;
local BeachBuffSec	= 0;
local BeachBufftimer = 0;
local beachBtn = 0;

local t_partyMemberBuffEvents =	{	["GE_MAIN_BUF_LIST_UPDATE"] = {}, ["GE_DESTROY_MAINPLAYER"] = {}, ["GE_MAIN_RIDE_CHG"] = {},
									["GE_UPDATE_MAIN_PET_FIGHT_MODE"] = {}, ["GE_MAIN_PET_FIGHT"] = {}, ["GE_MAIN_PET_CALLBACK"] = {},
									["GE_CLAN_ADVANCE_UPDATE"] = {}, ["GE_MAIN_PK_VAL_CHG"] = {}, ["GE_TEAM_GET_TEAMINFO"] = {},
									["GE_SPAN_STRONG_CHANGE_INFO"] = {},
							};

t_partyMemberBuffEvents["GE_MAIN_BUF_LIST_UPDATE"].func = 
function ()
	UpdateBuff();
end

t_partyMemberBuffEvents["GE_TEAM_GET_TEAMINFO"].func = 
function ()
	UpdateBuff();
end

t_partyMemberBuffEvents["GE_SPAN_STRONG_CHANGE_INFO"].func = 
function ()
	UpdateBuff();
end

t_partyMemberBuffEvents["GE_MAIN_PET_CALLBACK"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local bFound		= false;
	
	-- TODO: 若改成多只宠物可出征，这需要更改，这儿只能处理一只宠物出征的情况
	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		-- TODO: 是否要判断宠物已经死亡
		if pet ~= nil and pet:getPetFightMode() == PET_FIGHT_ACTIVE then
			local petDiathesis = pet:getPetDiathesis();
			for i = 1, petDiathesis.m_nNum do
				local nId = petDiathesis.m_diathesisData[i-1].m_nID;
				local nLv = petDiathesis.m_diathesisData[i-1].m_nLevel;
				local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
				if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
					bFound = true;
					break;
				end
			end
		end
	end
	
	t_partyMemberBuffEvents["GE_MAIN_BUF_LIST_UPDATE"].func();
end

function IsFairyHavePassiveBuff( nOwnerId, nCampFairyId, nCampFairyLv )
	local actor = ActorMgr:FindActor( nOwnerId );
	if actor == nil then
		return false;
	end

	local GameTrump		= actor:getTrump();
	local campFairyId	= nCampFairyId;
	local campFairyLv	= nCampFairyLv;
	if campFairyId == 0 then
		return false;
	end

	local TrumpDef	= GameTrump:getFairyDef( campFairyId, campFairyLv );
	for i = 1, FAIRY_MAX_SKILL do
		local trumpSkillInfo = TrumpDef.FairySkillList[i-1];
		if trumpSkillInfo.SkillID ~= 0 and trumpSkillInfo.SkillType == DIATHESIS_TYPE then
			local diathesisDef = SkillMgr:getDiathesisDef( trumpSkillInfo.SkillID, trumpSkillInfo.SkillLevel );
			if diathesisDef.IconPosition == 1 then
				return true;
			end
		end
	end

	return false;
end

function UpdateMainPlayerBuff( nOwnerId, nCampFairyId, nCampFairyLv )
	if not IsFairyHavePassiveBuff( nOwnerId, nCampFairyId, nCampFairyLv ) then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:GetID() == nOwnerId then	-- 更新主角
		UpdateBuff();
	end

	if mainplayer:GetSelectTarget() == nOwnerId then	-- 更新目标头像
		UpdateTargetBuffWhenTargetChange();
	end
end

function FairyCamp( nOwnerId, nCampFairyId, nCampFairyLv )
	UpdateMainPlayerBuff( nOwnerId, nCampFairyId, nCampFairyLv );
	local mainplayer = ActorMgr:getMainPlayer();
	local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if TargetObj == nil or TargetObj:getType() ~= GAT_PET then
		return;
	end

	if TargetObj:getOwnerId() == nOwnerId then
		UpdateTargetBuffWhenTargetChange();
	end
end

function FairyCallBack( nOwnerId, nCampFairyId, nCampFairyLv )
	UpdateMainPlayerBuff( nOwnerId, nCampFairyId, nCampFairyLv );
	local mainplayer = ActorMgr:getMainPlayer();
	local TargetObj  = ActorMgr:FindActor( mainplayer:GetSelectTarget() );
	if TargetObj == nil or TargetObj:getType() ~= GAT_PET then
		return;
	end

	if TargetObj:getOwnerId() == nOwnerId then
		UpdateTargetBuffWhenTargetChange();
	end
end

t_partyMemberBuffEvents["GE_MAIN_RIDE_CHG"].func = 
function ()
	t_partyMemberBuffEvents["GE_MAIN_BUF_LIST_UPDATE"].func();
end

t_partyMemberBuffEvents["GE_DESTROY_MAINPLAYER"].func = 
function ()
	ClearBuffButton();
end

t_partyMemberBuffEvents["GE_UPDATE_MAIN_PET_FIGHT_MODE"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	local petPackage	= mainplayer:getPet();
	local bFound		= false;

	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		-- TODO: 是否要判断宠物已经死亡
		if pet ~= nil then
			local petDiathesis = pet:getPetDiathesis();
			for i = 1, petDiathesis.m_nNum do
				local nId = petDiathesis.m_diathesisData[i-1].m_nID;
				local nLv = petDiathesis.m_diathesisData[i-1].m_nLevel;
				local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
				if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
					bFound = true;
					break;
				end
			end
			--天赋技能
			local nGeniusId = pet:getPetGeniusId();
			local nGeniusLv = pet:getPetGeniusLv();
			local geniusDiathesisDef = SkillMgr:getDiathesisDef( nGeniusId, nGeniusLv );
			if geniusDiathesisDef ~= nil and geniusDiathesisDef.PetDiathesisType == 2 then
				bFound = true;
				break;
			end
		end
	end

	if bFound then	
		UpdateBuff();
	end
end

t_partyMemberBuffEvents["GE_CLAN_ADVANCE_UPDATE"].func = 
function ()
	local mainplayer	= ActorMgr:getMainPlayer();
	if mainplayer:isInClan() and GameClanManager:getPlayerClanSkillNum() > 0 then
		UpdateBuff();
	end
end

t_partyMemberBuffEvents["GE_MAIN_PK_VAL_CHG"].func = 
function ()
	UpdateBuff();
end

-- 标识是否处于多人骑乘状态
function PartyMemberBuffTooltip_OnLoad()
	for event, _ in pairs( t_partyMemberBuffEvents ) do
		this:RegisterEvent( event );
	end

	t_partyMemberBuffEvents.__index = function ( tab, key )
								tab[key] = { func = function () end };
								return tab[key];
							end

	setmetatable( t_partyMemberBuffEvents, t_partyMemberBuffEvents );
end

function PartyMemberBuffTooltip_OnEvent()
	t_partyMemberBuffEvents[arg1].func();
end

function ClearBuffButton()
	-- 隐藏未使用的buf
	for i = 1, MAX_MAIN_GOOD_BUF_COUNT do
		DelBuff( "PartyMemberBuffTooltipBuff" .. i );	
	end

	for i = 1, MAX_MAIN_DEBUF_COUNT do
		DelBuff( "PartyMemberBuffTooltipDebuff" .. i );	
	end
	-- 清除海滩buff状态
	BeachBufftimer = 0;
	beachBtn = 0;
	BeachBuffIn 	= 0;
	BeachBuffSec	= 0;
end

local t_screenScale = { x = 1.0, y = 1.0 };
function PartyMemberBuffTooltip_OnUpdate()
	if t_screenScale["x"] ~= GetScreenScaleX() or t_screenScale["y"] ~= GetScreenScaleY() then
		t_screenScale["x"] = GetScreenScaleX();
		t_screenScale["y"] = GetScreenScaleY();
		--UpdateBuff();

		-- 在分辨率变化的时候重新调整buff的位置
		local mainplayer		= ActorMgr:getMainPlayer();
		if mainplayer == nil then return end
		local BufStatus			= mainplayer:getBufStatus();	
		local nRideBuffCount	= 0;
		local rider				= mainplayer:getRider();
		local curUsingRide		= rider:getRideItem();
		-- 若处于骑乘状态，则显示骑乘buff
		if rider:isOnHorse() and curUsingRide.RideID ~= 0 then
			nRideBuffCount = 1;
		end
		
		local nPetBuffCount	= 0;
		local petPackage	= mainplayer:getPet();
		for i = 1, PET_MAX_FIGHT_SHI do
			local pet = petPackage:getFightPet( i - 1 );
			if pet ~= nil then
				local petDiathesis = pet:getPetDiathesis();
				-- 若宠物不处于主动状态，则不能给主人加buff
				if petDiathesis.m_nFightMode == PET_FIGHT_ACTIVE and pet.Passive == PET_FIGHT_ACTIVE then
					for i = 1, petDiathesis.m_nNum do
						local nId = petDiathesis.m_diathesisData[i-1].m_nID;
						local nLv = petDiathesis.m_diathesisData[i-1].m_nLevel;
						local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
						if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
							nPetBuffCount = nPetBuffCount + 1;
						end
					end
				end
			end
		end
		
		
		local nGoodBufBtnCount = nPetBuffCount + BufStatus:getGoodStatusCount() + BufStatus:getNolimitStatusCount() + nRideBuffCount;
		--print( "PartyMemberBuffTooltip_OnUpdate--nGoodBufBtnCount = "..nGoodBufBtnCount.."--219" );
		AdJustBuffBtnsPos( math.min( MAX_MAIN_GOOD_BUF_COUNT, nGoodBufBtnCount ), BufStatus:getBadStatusCount() );
	end
end

function SetMainPlayerJiaFairyBuffer( data )
	local mainplayer		= ActorMgr:getMainPlayer();
	local GameTrump			= mainplayer:getTrump();
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local RoleTrump			= GameTrump:getCampRoleFairy();
	if RoleTrump == nil then
		return nGoodBufBtnCount
	end
	local campFairyId		= RoleTrump.FairyID;
	local campFairyLv		= RoleTrump.FairyLevel;
	if campFairyId == 0 then
		return nGoodBufBtnCount;
	end

	local TrumpDef	= GameTrump:getFairyDef( campFairyId, campFairyLv );
	for i = 1, FAIRY_MAX_SKILL do
		local trumpSkillInfo = TrumpDef.FairySkillList[i-1];
		if trumpSkillInfo.SkillID ~= 0 and trumpSkillInfo.SkillType == DIATHESIS_TYPE then
			local nId = trumpSkillInfo.SkillID;
			local nLv = trumpSkillInfo.SkillLevel;
			local diathesisDef = SkillMgr:getDiathesisDef( trumpSkillInfo.SkillID, trumpSkillInfo.SkillLevel );
			if diathesisDef.IconPosition == 1 then
				AddMainFairyBuf( "PartyMemberBuffTooltipBuff"..nGoodBufBtnCount, nId, nLv, mainplayer:GetID() );
				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end
		end
	end

	return nGoodBufBtnCount;
end

function SetMainPlayerJiaPetBuffer( data )
	-- 查看此人出征的宠物对他的影响
	local mainplayer		= ActorMgr:getMainPlayer();
	local petPackage		= mainplayer:getPet();
	local nGoodBufBtnCount	= data["goodBuffCount"];
	local szText = ""
	--宠物技能
	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		if pet ~= nil and not pet:isDead() then
			local petDiathesis = pet:getPetDiathesis();
			-- 若宠物不处于主动状态，则不能给主人加buff
			if petDiathesis.m_nFightMode ~= PET_FIGHT_ACTIVE and pet.Passive ~= PET_FIGHT_ACTIVE then
				return nGoodBufBtnCount;
			end
			for j=1, petDiathesis.m_nNum do
				local nId = petDiathesis.m_diathesisData[j-1].m_nID;
				local nLv = petDiathesis.m_diathesisData[j-1].m_nLevel;
				local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
				if diathesisDef ~= nil and diathesisDef.IconPosition == 1 then
					szText = szText .. string.gsub(diathesisDef.Name,"（被动技能）","：") .. diathesisDef.DiathesisDesc .. "#n\n";
				end
			end
		end
	end
	if szText ~= "" then
		szText = "#cff6e1c宠物技能：#n\n" ..szText
	end
	--宠物天赋技能
	for i = 1, PET_MAX_FIGHT_SHI do
		local pet = petPackage:getFightPet( i - 1 );
		if pet ~= nil then
			local petDiathesis = pet:getPetDiathesis();
			-- 若宠物不处于主动状态，则不能给主人加buff
			if petDiathesis.m_nFightMode ~= PET_FIGHT_ACTIVE and pet.Passive ~= PET_FIGHT_ACTIVE then	
				return nGoodBufBtnCount;
			end			
			local nId = pet:getPetGeniusId();
			local nLv = pet:getPetGeniusLv();
			local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
			if diathesisDef ~= nil and diathesisDef.PetDiathesisType == 2 then
				if szText ~= "" then
					szText = szText .. "\n#cff6e1c宠物天赋技能：#n\n"
				else
					szText = szText .. "#cff6e1c宠物天赋技能：#n\n"
				end
				szText = szText .. diathesisDef.Name ..":" .. diathesisDef.DiathesisDesc .. "#n\n";
			end
		end
	end
	if szText ~= "" then
		AddMainPetBuf( "PartyMemberBuffTooltipBuff"..nGoodBufBtnCount, szText );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end
	return nGoodBufBtnCount;
end

--是否和自己的徒弟组了队
function IsNeedShowTeamSchoolBuff()
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer then
		return false;
	end
	--自己为师傅，且组了队
	if GameSchool:GetSelfSchoolJobType() == ROLE_SCHOOL_TYPE_TEACHER_TOLUA and TeamManager:isInTeam( mainplayer:getName() ) then
		for i = 1, GameSchool:GetSelfStudentNum() do
			local studentInfo = GameSchool:GetSelfStudentByIndex( i - 1 )
			--有徒弟在队伍里边
			if TeamManager:isInTeam( studentInfo.RoleName ) then
				return true
			end
		end
	end
	return false
end
--和徒弟组队的师傅，为其加上特殊buff()
function AddMainSchoolBuff( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_HAI_TAN );
	buff:SetClientUserData( 3, nOwerId );	
	
	local statusDef	= getStatusDef( nId, nLv );

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetBuffIconPath( statusDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();
end
--幻境中-检查是否有高倍经验层buff,此为假buff，故而类型设置为海滩一类
function AddDreamLandBuff( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_HAI_TAN );
	buff:SetClientUserData( 3, nOwerId );	
	
	local statusDef	= getStatusDef( nId, nLv );

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetBuffIconPath( statusDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();
end

--是否占领有跨服据点
function IsSpanStrongOwner()
	if GameClan_SpanBaseWar:getSpanStrongOwnerExpVal() ~= 0 or GameClan_SpanBaseWar:getSpanStrongOwnerBindGold() ~= 0 
		or GameClan_SpanBaseWar:getSpanStrongOwnerBull() ~= 0 then
			return true;
	end
	return false
end
--占领了跨服据点，需要显示本服跨服奖励的假buff
function AddMainSpanStrongOwnerBuff( szBuffName )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, GameClan_SpanBaseWar:getSpanStrongOwnerExpVal() );
	buff:SetClientUserData( 1, GameClan_SpanBaseWar:getSpanStrongOwnerBindGold() );
	buff:SetClientUserData( 2, BUFF_TYPE_SPAN_STRONG_OWNER );
	buff:SetClientUserData( 3, GameClan_SpanBaseWar:getSpanStrongOwnerBull() );
		
	local szBuffDesc = "跨服[据点]奖励"
	if GameClan_SpanBaseWar:getSpanStrongOwnerExpVal() > 0 then
		szBuffDesc = szBuffDesc.. "\n\t●60级以上玩家打怪经验增加"..GameClan_SpanBaseWar:getSpanStrongOwnerExpVal().."%"
	end
	if GameClan_SpanBaseWar:getSpanStrongOwnerBindGold() > 0 then
		szBuffDesc = szBuffDesc.. "\n\t●每小时得10金券，每天最多"..GameClan_SpanBaseWar:getSpanStrongOwnerBindGold().."金券"
	end
	if GameClan_SpanBaseWar:getSpanStrongOwnerBull() > 0 then
		szBuffDesc = szBuffDesc.. "\n\t●每分钟额外获得100绑银，每天最多获得"..GameClan_SpanBaseWar:getSpanStrongOwnerBull().."绑银"
	end
	buff:SetClientString( szBuffDesc )

	local t_textures =	{
					[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
					[szBuffName .. "Icon"]		= { path = "uires\\TuPianLei\\JiNeng\\TianFu\\1109.tga" },
				};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();	
end


-- 调整buff图标位置的基本参考标准：第4个buf左部相对于目标头像右部的偏移值是22	二者剩余间隔占总间隔的比例是0.13836477987421
function UpdateBuff()
	-- 此为用于判断当前目标头像是否是主角,若是同步雷达和目标头像上的buff状态
	local mainplayer = ActorMgr:getMainPlayer();
	if not mainplayer then
		return;
	end

	local BufStatus	= mainplayer:getBufStatus();
	local nGoodBufBtnCount = 1;
	if nGoodBufBtnCount <= MAX_MAIN_GOOD_BUF_COUNT and mainplayer:isNeedShowSafeAreaBuf() and mainplayer:getPkTargetName()== "" then
		AddMainSafeBuf( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, 0, 0, 0 );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end
	if nGoodBufBtnCount <= MAX_MAIN_GOOD_BUF_COUNT and IsNeedShowTeamSchoolBuff() then
		AddMainSchoolBuff( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, 3209, 1, mainplayer:GetID() );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end

	for i = 1, MAX_GOOD_STATUS do
		local Buf		= BufStatus:getGoodStatus(i-1);
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\TuPianLei\\JiNeng\\Buff\\"..BufIconId..".tga";
		
		if nGoodBufBtnCount > MAX_MAIN_GOOD_BUF_COUNT then
			break;
		end

		if ( Buf:isShow() ) then
			assert( Buf:getBufLv() ~= 0, "获得Buf等级为0!!!" );
			AddBuff( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, GetBuffIconPath( BufIconId ), 0, Buf:getLife(), Buf:getBufId(), 
						Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ), 
						mainplayer:GetID() );

			nGoodBufBtnCount = nGoodBufBtnCount + 1;
		end
	end
	
	-- 若处于合体状态，则显示合体buff
	local petMgr = mainplayer:getPet();
	local petHetiIdx = petMgr:getPetHeti();
	
	if petHetiIdx ~= -1 then
		local petActor	= petMgr:getFightPet( petHetiIdx );
		local petInfo 	= petMgr:getPetInfo( petActor ) ;
		if petMgr:isExtraBattlePet(petInfo) then
			AddMainPetHetiBuf("PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, petHetiIdx, mainplayer:GetID(),true);
		else 
			AddMainPetHetiBuf("PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, petHetiIdx, mainplayer:GetID(),false);
		end
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end
	
	local rider	= mainplayer:getRider();
	local curUsingRide	= rider:getRideItem();
	-- 若处于骑乘状态，则显示骑乘buff
	if nGoodBufBtnCount <= MAX_MAIN_GOOD_BUF_COUNT and rider:isOnHorse() and curUsingRide.RideID ~= 0 then
		local rideDef	= getRideDef( curUsingRide.RideID, curUsingRide.nLevel );
		AddMainRideBuf( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, curUsingRide.RideID, curUsingRide.nLevel, mainplayer:GetID() );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end

	-- 若激活了工会技能，则显示公会技能buff
	if mainplayer:isInClan() and GameClanManager:getPlayerClanSkillNum() > 0 then
		local szText = GetMySkillList();
		if szText ~= "#cff6e1c公会技能：#n\n" then
			AddMainClanSkillBuf( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, szText );
			nGoodBufBtnCount = nGoodBufBtnCount + 1;
		end
	end

	-- 若PK值大于200则显示PK惩罚buff
	if GameActorPVP:getPkValue()/10 >= 200 then
		AddMainPKBuf( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end
	
	--幻境中-检查是否有高倍经验层buff
	local MaxMapControl = MapShower:GetMaxMapContal();
	if math.floor(MaxMapControl:getMapID()/100) == 115 and DreamLandMgr:GetBufferLayer() > 0 and DreamLandMgr:GetBufferLayer() <= 100 then
		AddDreamLandBuff( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, 1281, 1, mainplayer:GetID() )
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end

	-- 查看出征的宠物是否有给主人加良性Buff
	nGoodBufBtnCount = SetMainPlayerJiaPetBuffer( { goodBuffCount = nGoodBufBtnCount } );
	nGoodBufBtnCount = SetMainPlayerJiaFairyBuffer( { goodBuffCount = nGoodBufBtnCount } );
	--跨服据点的假buff
	if IsSpanStrongOwner() then
		AddMainSpanStrongOwnerBuff( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount )
		nGoodBufBtnCount = nGoodBufBtnCount + 1
	end
	
	local nBadBufBtnCount = 1;
	
	for i=1, MAX_NOLIMIT_STATUS do
		local Buf		= BufStatus:getNolimitStatus( i - 1 );
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\TuPianLei\\JiNeng\\Buff\\"..BufIconId..".tga";
		
		if nGoodBufBtnCount > MAX_MAIN_GOOD_BUF_COUNT then
			break;
		end

		if Buf:isShow() then
			assert( Buf:getBufLv() ~= nil, "获得Buf等级为0!!!" );
			if isStatusShowAsDebuff( Buf:getBufId(),Buf:getBufLv() ) then 
				AddBuff( "PartyMemberBuffTooltipDebuff" .. nBadBufBtnCount, GetBuffIconPath( BufIconId ), 0, Buf:getLife(), 
						Buf:getBufId(), Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ),
						mainplayer:GetID() );

				nBadBufBtnCount = nBadBufBtnCount + 1;
			else 
				AddBuff( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, GetBuffIconPath( Buf:getIconId() ),0, 
							Buf:getLife(), Buf:getBufId(), Buf:getBufLv(), 
							Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ), mainplayer:GetID() );

				nGoodBufBtnCount = nGoodBufBtnCount + 1;
			end
		end
	end

	for i=1, BufStatus:getJiaStatusCount() do
		local Buf		= BufStatus:getJiaStatus( i - 1 );
		
		if nGoodBufBtnCount > MAX_MAIN_GOOD_BUF_COUNT then
			break;
		end
		
		AddMainHaiTanBuf( "PartyMemberBuffTooltipBuff" .. nGoodBufBtnCount, Buf:getBufId(), Buf:getBufLv(), mainplayer:GetID() );
		nGoodBufBtnCount = nGoodBufBtnCount + 1;
	end

	if mainplayer:getBattleLimitLeftTime() > 0 then
		AddMainBattleLimitBuf( mainplayer,"PartyMemberBuffTooltipDebuff" .. nBadBufBtnCount )
		nBadBufBtnCount = nBadBufBtnCount + 1;
	end
	for i=1, MAX_BAD_STATUS do
		local Buf		= BufStatus:getBadStatus( i - 1 );
		local BufIconId	= Buf:getIconId();
		local BufText	= "uires\\TuPianLei\\JiNeng\\Buff\\"..BufIconId..".tga";
		
		if nBadBufBtnCount > MAX_MAIN_DEBUF_COUNT then
			break;
		end

		if Buf:isShow() then
			assert( Buf:getBufLv() ~= 0, "获得Buf等级为0!!!" );
			
			AddBuff( "PartyMemberBuffTooltipDebuff" .. nBadBufBtnCount, GetBuffIconPath( BufIconId ), 0, Buf:getLife(), 
						Buf:getBufId(), Buf:getBufLv(), Buf:getBufId() == CLIENT_DIE_REMIT_BUFFID and ( Buf:getOverlay() ) or ( Buf:getOverlay() + 1 ),
						mainplayer:GetID() );

			nBadBufBtnCount = nBadBufBtnCount + 1;
		end
	end
	
	-- 隐藏未使用的buf
	for i = nGoodBufBtnCount, MAX_MAIN_GOOD_BUF_COUNT do
		DelBuff( "PartyMemberBuffTooltipBuff" .. i );	
	end

	for i = nBadBufBtnCount, MAX_MAIN_DEBUF_COUNT do
		DelBuff( "PartyMemberBuffTooltipDebuff" .. i );	
	end
	
	--print( "UpdateBuff--nGoodBufBtnCount = "..( nGoodBufBtnCount - 1 ).."--388" );
	local curTime = getServerTime().sec;
	BeachBuffSec = BeachBuffSec - ( curTime - BeachBufftimer );
	SetOnBeachBuff( BeachBuffIn, BeachBuffSec );
	
	AdJustBuffBtnsPos( nGoodBufBtnCount - 1, nBadBufBtnCount - 1 );
end

function AdJustBuffBtnsPos( nGoodBuffNum, nBadBuffNum )
	-- 若屏幕宽度大于1024，向坐标多加buf图标
	-- 若屏幕宽度不超过1024，则按屏幕缩放比例来调整buf图标的位置
	-- 若屏幕高度不超过768的情况下，按屏幕缩放比例来缩放buf图标的高度，
	-- 若屏幕高度不少于768，则buf图标高度使用标准高度
	-- 第4个buf左部相对于目标头像右部的偏移值是22	二者剩余间隔占总间隔的比例是0.13836477987421
	--local STANDARD_SCALE	= 0.138;
	local STANDARD_SCALE	= 0.18;
	local STANDARD_X_SPACE	= -5;
	local STANDARD_Y_SPACE	= 15;
	local STANDARD_SIZE		= 32;
	local firstBtn			= getglobal( "PartyMemberBuffTooltipBuff1" );
	local fLargestDist		= math.abs( Target:GetRight() - firstBtn:GetRight() );
	local fSizeArrangeBuffs	= ( 1 - STANDARD_SCALE ) * fLargestDist;
	local fXOffSet			= ( GetScreenWidth() >= DEFAULT_UI_WIDTH ) and STANDARD_X_SPACE or 
									( STANDARD_X_SPACE * GetScreenScaleX() );
	local fYOffset			= ( GetScreenHeight() >= DEFAULT_UI_HEIGHT ) and STANDARD_Y_SPACE or 
									( STANDARD_Y_SPACE * GetScreenScaleY() );
	local firstYOffset		= ( GetScreenHeight() >= DEFAULT_UI_HEIGHT ) and STANDARD_Y_SPACE or 
									( STANDARD_Y_SPACE * GetScreenScaleY() );
	local nPerBtnValidWidth	= firstBtn:GetWidth() + math.abs( fXOffSet );
	local nBuffNumPerLine	= math.floor( fSizeArrangeBuffs / nPerBtnValidWidth );
	local nWidth			= ( GetScreenWidth() >= DEFAULT_UI_WIDTH ) and STANDARD_SIZE or 
									( STANDARD_SIZE * GetScreenScaleX() );
	local nHeight			= ( GetScreenHeight() >= DEFAULT_UI_HEIGHT ) and STANDARD_SIZE or 
									( STANDARD_SIZE * GetScreenScaleY() );
	
	-- 调整buff图标位置的基本参考标准：第4个buf左部相对于目标头像右部的偏移值是22	二者剩余间隔占总间隔的比例是0.13836477987421
	for i = 1, nGoodBuffNum do
		local btn = getglobal( "PartyMemberBuffTooltipBuff" .. i );
		
		if i > 1 then
			local relBtn;
			if math.fmod( i, nBuffNumPerLine ) == 1 then
				relBtn = getglobal( "PartyMemberBuffTooltipBuff" .. ( i - nBuffNumPerLine ) );
				btn:SetPoint( "TOP", relBtn:GetName(), "BOTTOM", 0, fYOffset );
			else
				relBtn = getglobal( "PartyMemberBuffTooltipBuff" .. ( i - 1 ) );
				btn:SetPoint( "right", relBtn:GetName(), "left", fXOffSet, 0 );
			end
		end	
		addChangedFrames( btn:GetName() );
	end
	
	firstBtn = getglobal( "PartyMemberBuffTooltipDebuff1" );
	-- 若普通Buff数量为0
	if nGoodBuffNum == 0 then
		firstBtn:SetPoint( "TOP", "PartyMemberBuffTooltipBuff1", "BOTTOM", 0, fYOffset );
	else
		local nBottomGoodBtnIndex = ( math.ceil( nGoodBuffNum / nBuffNumPerLine ) - 1 ) * nBuffNumPerLine + 1;
		firstBtn:SetPoint( "TOP", "PartyMemberBuffTooltipBuff"..nBottomGoodBtnIndex, "BOTTOM", 0, fYOffset );
	end

	addChangedFrames( firstBtn:GetName() );
	for i = 1, nBadBuffNum do
		local btn = getglobal( "PartyMemberBuffTooltipDebuff" .. i );
		if i > 1 then
			local relBtn;
			if math.fmod( i, nBuffNumPerLine ) == 1 then
				relBtn = getglobal( "PartyMemberBuffTooltipDebuff" .. ( i - nBuffNumPerLine ) );
				btn:SetPoint( "TOP", relBtn:GetName(), "BOTTOM", 0, fYOffset );
			else
				relBtn = getglobal( "PartyMemberBuffTooltipDebuff" .. ( i - 1 ) );
				btn:SetPoint( "right", relBtn:GetName(), "left", fXOffSet, 0 );
			end
		end	
		addChangedFrames( btn:GetName() );
	end
end

function AdJustTargetBuffBtnsPos( nGoodBuffNum, nBadBuffNum )
	-- 现在只需设置debuff图标的位置
	local firstBtn = getglobal( "TargetDebuff1" );
	if nGoodBuffNum == 0 then
		firstBtn:SetPoint( "TOPLEFT", "TargetBuff1", "TOPLEFT", 0, 0 );
	elseif nGoodBuffNum <= ( MAX_TARGET_GOOD_BUF_COUNT / 2 ) then
		firstBtn:SetPoint( "TOP", "TargetBuff1", "BOTTOM", 0, 3 );
	elseif nGoodBuffNum > ( MAX_TARGET_GOOD_BUF_COUNT / 2 ) then
		firstBtn:SetPoint( "TOP", "TargetBuff9", "BOTTOM", 0, 3 );
	end

	for i = 1, nBadBuffNum do
		addChangedFrames( "TargetDebuff" .. i );
	end
end

-- t_buffInfo.statusId, t_buffInfo.level, t_buffInfo.totalTime, t_buffInfo.startTime, t_buffInfo.buffButtonName， t_buffInfo.ownerId 

function ReplaceStatusDesc( statusDef, nOverlay )
	if statusDef.OverlayLimit == 0 then
		return statusDef.StatusDesc;
	end
	
	local szText = statusDef.StatusDesc;
	local firstPos1, endPos1 = string.find( szText, "#" );
	if firstPos1 == nil or endPos1 == nil then
		return statusDef.StatusDesc;
	end

	--print( "firstPos1 = "..firstPos1.."，endPos1 = "..endPos1 );
	local szText1 = string.sub( statusDef.StatusDesc, 1, firstPos1 - 1 );
	local firstPos2, endPos2 = string.find( szText, "#", endPos1 + 1 );
	if firstPos2 == nil or endPos2 == nil then
		return szText1;
	end

	local szText2 = string.sub( statusDef.StatusDesc, endPos2 + 1 );
	--print( "firstpos2 = "..firstPos2.."， endpos2 = "..endPos2 );
	local num = tonumber( string.sub( szText, endPos1 + 1, firstPos2 - 1 ) );
	
	if nOverlay > 0 then
		local statusOverlayDef = getStatusOverlayDef( statusDef.StatusID, statusDef.Level, nOverlay );
		num = math.ceil( num * ( 1 + statusOverlayDef.Mul / 100 ) );
	end
	
	--print( "num = "..num );
	return szText1..num..szText2;
end

local pkValueTips = {
	[2] = { 10, 10 },
	[3] = { 50, 30 },
	[4] = { 100, 50 },
	[5] = { 500, 90 },
}

function ShowJiaBuffTips( t_buffInfo )
	local nStatusId		= t_buffInfo.statusId;
	local nLv			= t_buffInfo.level;
	local statusDef		= getStatusDef( nStatusId, nLv );
	if statusDef == nil then
		return;
	end

	local szDesc		= statusDef.StatusDesc;
	local szBuffName	= statusDef.Name;
	if nStatusId == CLIENT_VIP_BAR_STATUS_ID and nLv == CLIENT_VIP_BAR_STATUS_LEVEL then
		local mainplayer = ActorMgr:getMainPlayer();
		local netBarInfo = mainplayer:getNetBarInfo();
		local t_ReplaceDesc = {};
		for i = 1,netBarInfo.DynValueNum do
			table.insert( t_ReplaceDesc,{ "_" .. i .. "_" , netBarInfo.DynValue[i-1] } )
		end	
		for i=1,#t_ReplaceDesc do
			local v = string.find( szDesc,t_ReplaceDesc[i][1] )
			if v ~= 0 then
				szDesc = string.gsub( szDesc,t_ReplaceDesc[i][1],t_ReplaceDesc[i][2] )
			end
		end
	elseif nStatusId == PK_BUFF_ID then
		local rank = math.floor( GameActorPVP:getPkValue() / 1000 );
		local t_ReplaceDesc = {};
		for i=1, 2 do
			table.insert( t_ReplaceDesc,{ "_" .. i .. "_" , pkValueTips[rank][i] } );
		end
		for i=1,#t_ReplaceDesc do
			local v = string.find( szDesc,t_ReplaceDesc[i][1] )
			if v ~= 0 then
				szDesc = string.gsub( szDesc,t_ReplaceDesc[i][1],t_ReplaceDesc[i][2] )
			end
		end
	end
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "#cff6e1c"..statusDef.Name.."#n\n"..szDesc, 
							frame = t_buffInfo.buffButtonName, button = t_buffInfo["button"] } );
end

function ShowBuffTips( t_buffInfo )
	local nStatusId		= t_buffInfo.statusId;
	local nLv			= t_buffInfo.level;
	local statusDef		= getStatusDef( nStatusId, nLv );
	local szDesc		= statusDef.StatusDesc;
	local szBuffName	= statusDef.Name;
	local fBuffTotal	= t_buffInfo.totalTime;
	local fBuffStart	= t_buffInfo.startTime;
	local nOverlay		= t_buffInfo["overlay"];
	local iremainTime	= math.ceil( fBuffTotal - fBuffStart );
	local mainplayer = ActorMgr:getMainPlayer();

	local strLeaveHour = "";
	if iremainTime >= 3600 then
		strLeaveHour = math.floor( iremainTime / 3600 ) .. "小时";
	end

	local strLeaveMinute = "";
	if iremainTime >= 60 then
		strLeaveMinute = math.floor( math.fmod( iremainTime, 3600 ) / 60 ) .. "分";
	end

	local strLeaveSecond = "";
	strLeaveSecond = math.fmod( iremainTime, 60 ) .. "秒";

	local szNameText	= "#cff6e1c"..statusDef.Name.."#n\n";
	
	local szBuffDesc	= "#cffffff"..ReplaceStatusDesc( statusDef, nOverlay ).."#n\n";
	local szLeaveTime	= (iremainTime < FOR_EVERY and canShowBuff( nStatusId, nLv )) and "#cffD800剩余时间：" .. strLeaveHour..strLeaveMinute..strLeaveSecond.."#n\n" or ""; 
	local szCancelHint	= ( ( isBuffCanCancel( nStatusId, nLv ) and mainplayer:GetID() == t_buffInfo.ownerId ) ) and 
								"#c817F6D右键点击可以取消效果#n\n" or "";
	local szText = szNameText..szBuffDesc..szLeaveTime..szCancelHint;
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = t_buffInfo.buffButtonName,
						button = t_buffInfo["button"] } );
end

function ShowRideBuffTips( t_buffInfo )	
	local SPEED_UNIT		= 1;
	local rideDef			= getRideDef( t_buffInfo.rideId, t_buffInfo.level );
	local mainplayer		= ActorMgr:getMainPlayer();
	local szNameText		= "#cff6e1c骑乘#n\n";
	local nIncrSpeedScale	= math.ceil( rideDef.RideSpd * 100 / CLIENT_BASE_MSPD );
	local nIntegralPart		= math.modf( nIncrSpeedScale / SPEED_UNIT );
	nIncrSpeedScale			= SPEED_UNIT * nIntegralPart;

	local szIncrSpeed		= ( rideDef.RideSpd == 0 ) and "" or "#cffffff移动速度提升"..nIncrSpeedScale.."%#n\n";
	local szRideDesc		= "#cffffff"..rideDef.RideDesc.."#n\n";
	local szRideLeaveTime	= "";
	local szCancelHint		= mainplayer:GetID() ~= t_buffInfo.ownerId and "" or "#c817F6D右键点击可以取消骑乘#n\n";
	local szText = szNameText..szIncrSpeed..szRideDesc..szRideLeaveTime..szCancelHint;
	ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = szText, frame = t_buffInfo.buffButtonName,
						button = t_buffInfo.buffButtonName } );

	SetGameTooltipRelFrame( { t_buffInfo["buffButtonName"], t_buffInfo["parent"] } );
end

local hetiAttrTypeTips = {
	[0] = "宠物拥有绝技%s，额外附加5%%的生命",
	[1] = "宠物拥有绝技%s，额外附加5%%的防御",
	[2] = "宠物拥有绝技%s，额外附加5%%的攻击",
}

function ShowPetHetiBuffTips(info)
	local actor = ActorMgr:FindActor(info.owner)
	if not actor then
		return
	end
	local petMgr = actor:getPet()
	local petActor = petMgr:getPetActor(info.index)
	local display = GetNpcDisplayInfo(petActor:getResId())
	if not display then
		return
	end
	local player = ActorMgr:getMainPlayer()
	local text
	local buffname = SkillMgr:getSpellStaticAtt(PET_HETI_SKILL_ID, 1).Name
	if player == actor then
		local t = {}
		local petInfo = petMgr:getPetInfo(petActor);
		if petMgr:isExtraBattlePet( petInfo ) then
			table.insert(t, "#cff6e1c战宠合体#n");
			table.insert(t, string.format("已与宠物%s合体", display.name))
			table.insert(t, string.format("#G战宠:%s%s#n", petInfo.Name," 提供 40% 属性加成" ))
			for i = 1, petMgr:getPetNum() do
				local battlePetInfo = petMgr:getPetInfo( i - 1 );
				if petMgr:isExtraBattlePet( battlePetInfo ) and 
						battlePetInfo.WID.nHeight ~= petInfo.WID.nHeight and 
						battlePetInfo.WID.nLow ~= petInfo.WID.nLow then 
					local nIndex = getSamePetNameIndex( i,petInfo );
					if nIndex == 0 then 	
						table.insert(t, string.format("#G战宠:%s%s#n",battlePetInfo.Name," 提供 25% 属性加成" ));
					else 
						table.insert(t, string.format("#G战宠:%s%d%s#n",battlePetInfo.Name,nIndex," 提供 25% 属性加成" ));
					end
				end
			end
		else 
			table.insert(t, string.format("#cff6e1c%s#n", buffname))
			table.insert(t, string.format("已与宠物%s合体", display.name))
			table.insert(t, "宠物自身属性的25%转化为人物属性")
			local petInfo = petMgr:getPetInfo(petActor)
			if petInfo then
				local petDefInfo = getPetDef(petInfo.PetID)
				if petDefInfo and petDefInfo.TejiID ~= 0 then
					local skills = petInfo.PetSkills.SkillGrid
					for i = 1, petInfo.PetSkills.SkillGridNum do
						if skills[i - 1].SkillID == petDefInfo.TejiID then
							local attrType = petDefInfo.PetAttrType
							local diathesisDef =  SkillMgr:getDiathesisDef(skills[i - 1].SkillID, skills[i - 1].SkillLevel)
							table.insert(t, string.format(hetiAttrTypeTips[petDefInfo.PetAttrType], diathesisDef.Name))
							break
						end
					end
				end
			end
		end
		table.insert(t, "宠物合体状态下持续消耗宠物忠诚度，忠诚度小于50%将结束合体状态")
		text = table.concat(t, "\n")
	else
		text = string.format("#cff6e1c%s#n\n已与宠物%s合体", buffname, display.name)
	end
	ShowGameTooltip({
		rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = text or "",
		frame = info.button, button = info.button,
	})
	SetGameTooltipRelFrame({info.button, info.parent})
end

function BuffButton_OnEnter()	
	-- 骑乘buf显示
	local nID		= this:GetClientUserData( 0 );
	local nLv		= this:GetClientUserData( 1 );
	local nOwnerId	= this:GetClientUserData( 3 );
	local nType		= this:GetClientUserData( 2 );
	local curTime 	= getServerTime().sec;
	if nType == BUFF_TYPE_RIDE then
		local mainplayer	= ActorMgr:getMainPlayer();
		local rider			= mainplayer:getRider( );
		local rideInfo		= rider:getRideInfoById( nID  );

		 ShowRideBuffTips( { rideId = nID, level = rideInfo.nLevel, buffButtonName = this:GetName(), ownerId = nOwnerId, parent = this:GetParent() } );
	elseif nType == BUFF_TYPE_FAIRY then
		-- TODO: 和策划对tips效果
		ShowPetDiathesisTips( { id = nID, level = nLv, ownerId = nOwnerId, button = this:GetName(), parent = this:GetParent() } );
	elseif nType == BUFF_TYPE_PET_HETI then
		ShowPetHetiBuffTips({index = nID, owner = nOwnerId, button = this:GetName(), parent = this:GetParent()})
	elseif nType == BUFF_TYPE_SAFE then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "#cff6e1c神源的庇护#n\n位于时间城处于养精蓄锐状态", 
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );
	elseif nType == BUFF_TYPE_BATTLE_LIMIT then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = "#cff6e1c蓄势待发#n\n您在4小时之内进入太多次战场，请稍事休息再迎接下一次的战斗，蓄势待发中您将暂时无法进入战场", 
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );
	elseif nType == BUFF_TYPE_HAI_TAN then		
		ShowJiaBuffTips( { statusId = nID, level = nLv, buffButtonName = this:GetName(),ownerId = this:GetClientUserData( 3 ), 
							button = this:GetName(),
						} );
	elseif nType == BUFF_TYPE_CLAN_SKILL then
		ShowGameTooltip( { rich = "GameTooltipRichText", tipsFrame = "GameTooltip", text = this:GetClientString(), 
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );
	elseif nType == BUFF_TYPE_PET then
		ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = this:GetClientString(), 
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );
	elseif nType == BUFF_TYPE_PK then
		ShowJiaBuffTips( { statusId = nID, level = nLv, buffButtonName = this:GetName(),ownerId = this:GetClientUserData( 3 ), 
							button = this:GetName(),
						} );
	elseif nType == BUFF_TYPE_SPAN_STRONG_OWNER then
		ShowGameTooltip( { rich = "WideGameTooltipRichText", tipsFrame = "WideGameTooltip", text = this:GetClientString(), 
							frame = this:GetName(), button = this:GetName() } );
		SetGameTooltipRelFrame( { this:GetName(), this:GetParent() } );

	elseif nID == BEACH_BUFF_ID then
		ShowBuffTips( { statusId = this:GetClientUserData( 0 ), level = this:GetClientUserData( 1 ),
						totalTime = this:GetClientUserData( 2 ), startTime = curTime - BeachBufftimer, buffButtonName = this:GetName(),
						ownerId = this:GetClientUserData( 3 ), button = this:GetName(), parent = this:GetParent(),
						1} );
	else 
		local mainplayer= ActorMgr:getMainPlayer();
		local BufStatus	= mainplayer:getBufStatus();
		local buf		= BufStatus:getBufStatus( nID, nLv );
		ShowBuffTips( { statusId = this:GetClientUserData( 0 ), level = this:GetClientUserData( 1 ),
						totalTime = this:GetClientUserData( 2 ), startTime = 0, buffButtonName = this:GetName(),
						ownerId = this:GetClientUserData( 3 ), button = this:GetName(), parent = this:GetParent(),
						overlay = buf:getOverlay() } );
	end
end

function BuffButton_OnLeave()
	GameTooltip:Hide();
	WideGameTooltip:Hide()
end

function CancelSpecialBuffer( t_buffInfo )
	local mainplayer	= ActorMgr:getMainPlayer();
	local rider			= mainplayer:getRider();
	rider:requestRideDown();
end

-- t_buffInfo.statusId，t_buffInfo.level
function CancelBuffer( t_buffInfo )
	local nStatusId		= t_buffInfo.statusId;
	local nLv			= t_buffInfo.level;

	if isBuffCanCancel( nStatusId, nLv ) then
		local mainplayer	= ActorMgr:getMainPlayer();
		local BufStatus		= mainplayer:getBufStatus();
		BufStatus:cancelBuffer( nStatusId, nLv );
	end
end

function BuffButton_OnClick()
	if arg1 == "LeftButton" then
		return;
	end
	
	local nType	= this:GetClientUserData( 2 );
	if nType == BUFF_TYPE_PET or nType == BUFF_TYPE_FAIRY or nType == BUFF_TYPE_SAFE or nType == BUFF_TYPE_PET_GUAN_ZHI or 
		nType == BUFF_TYPE_PET_HETI or nType == BUFF_TYPE_HAI_TAN  or nType == BUFF_TYPE_BATTLE_LIMIT or 
		nType == BUFF_TYPE_CLAN_SKILL or nType == BUFF_TYPE_PK or nType == BUFF_TYPE_SPAN_STRONG_OWNER then
		return;
	end

	local mainplayer = ActorMgr:getMainPlayer();
	local nPlayerID = this:GetClientUserData( 3 );
	-- 若所点buf不是主角的buf，则不能取消buf
	if nPlayerID ~= mainplayer:GetID() then
		return;
	end
	
	local nId = this:GetClientUserData( 0 );
	local nLv = this:GetClientUserData( 1 );
	if getRideDef( nId, nLv ) ~= nil then
		CancelSpecialBuffer( { rideId = nId, level = nLv } );
	else
		CancelBuffer( { statusId = nId, level = nLv } );
	end
end

-- 用于每一帧更新buff所显示的时间效果
function BuffButton_OnUpdate()	
	local nStatusId = this:GetClientUserData( 0 );
	local nLv		= this:GetClientUserData( 1 );
	local nType		= this:GetClientUserData( 2 );
	
	-- 若为骑乘图标，则不显示
	if nType == BUFF_TYPE_RIDE or nType == BUFF_TYPE_PET or nType == BUFF_TYPE_FAIRY or nType == BUFF_TYPE_SAFE or
		nType == BUFF_TYPE_PET_HETI or nType == BUFF_TYPE_PET_GUAN_ZHI or nType == BUFF_TYPE_HAI_TAN or 
		nType == BUFF_TYPE_CLAN_SKILL or nType == BUFF_TYPE_PK or nType == BUFF_TYPE_SPAN_STRONG_OWNER then
		return;
	end
	if nType == BUFF_TYPE_BATTLE_LIMIT then
		local nLeftTime = this:GetClientUserData( 3 );
		local nBeginTime = nLv;
		nLeftTime = nLeftTime - math.floor(( GameMgr:getTickTime() - nBeginTime )/1000);
		if nLeftTime <= 0 then
			UpdateBuff();
			return;
		end
		local rich = getglobal( this:GetName().."duration" );
		local timeUnit;
		if ( nLeftTime / 3600 ) >= 1 then
			timeUnit = math.floor( nLeftTime / 3600 ) .. "h";
		elseif ( nLeftTime / 60 ) >= 1 then
			timeUnit = math.floor( nLeftTime / 60 ) .. "m";
		else
			timeUnit = math.floor(nLeftTime) .. "s";
		end
		if not rich:IsShown() then
			rich:Show()
		end
		local MIN_BLINK_TIME = 15;
		if nLeftTime < MIN_BLINK_TIME then
			rich:SetText( "#b#P" .. timeUnit, 255, 255, 255 );
		else
			rich:SetText( "#P" .. timeUnit, 255, 255, 255 );
		end
		return;
	end
	--处理海滩buff的时间
	HandleBeachBuffTime( nType,this:GetName() );
	
	local statusDef	= getStatusDef( nStatusId, nLv );
	-- 若该buff不能显示时间，则不继续操作
	if not canShowBuff( nStatusId, nLv ) then
		return;
	end

	local MIN_BLINK_TIME	= 15;
	local rich = getglobal( this:GetName().."duration" );
	-- 若剩余的时间大于15秒就不闪烁
	local mainplayer= ActorMgr:getMainPlayer();
	local BufStatus	= mainplayer:getBufStatus();
	local buf		= BufStatus:getBufStatus( nStatusId, nLv );
	if not buf then
		return;
	end

	local duration =  math.ceil( buf:getLife() );
	this:SetClientUserData( 2, duration );
	if duration >= FOR_EVERY then
		return
	end
	local timeUnit;
	if ( duration / 3600 ) >= 1 then
		timeUnit = math.floor( duration / 3600 ) .. "h";
	elseif ( duration / 60 ) >= 1 then
		timeUnit = math.floor( duration / 60 ) .. "m";
	else
		timeUnit = duration .. "s";
	end
	if not rich:IsShown() then
		rich:Show()
	end
	
	if duration < MIN_BLINK_TIME then
		rich:SetText( "#b#P" .. timeUnit, 255, 255, 255 );
	else
		rich:SetText( "#P" .. timeUnit, 255, 255, 255 );
	end
end

BUFF_TYPE_GOOD	= 0;
BUFF_TYPE_BAD	= 1;
BUFF_TYPE_SKILL	= 2;
BUFF_TYPE_POSE	= 3;
BUFF_TYPE_OTHER	= 4;

t_buffBackPath =	
{
	[BUFF_TYPE_GOOD]	= { [0] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga",		[1] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga" }, 
	[BUFF_TYPE_BAD]		= { [0] = "uires\\ui\\buff  TuBiaoDiBan\\Green.tga",	[1] = "uires\\ui\\buff  TuBiaoDiBan\\Lan.tga" }, 
	[BUFF_TYPE_SKILL]	= { [0] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga",		[1] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga" }, 
	[BUFF_TYPE_POSE]	= { [0] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga",		[1] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga" }, 
	[BUFF_TYPE_OTHER]	= { [0] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga",		[1] = "uires\\ui\\buff  TuBiaoDiBan\\Bai.tga" }, 
};

local szBuffPath = "uires\\TuPianLei\\JiNeng\\Buff\\";
function AddMainRideBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_RIDE );
	buff:SetClientUserData( 3, nOwerId );
	-- TODO: 修改成支持显示精灵假buf图标
	local rideDef = getRideDef( nId, nLv );

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = rideDef.RideOnNum > 0 and GetRideBuffIcon( rideDef.IconID ) or
																	GetRideItemIcon( rideDef.IconID )
							},
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();
end

function AddMainPetHetiBuf(szBuffName, Idx, nOwerId, bBattlePet )
	local buff = getglobal(szBuffName)
	buff:SetClientUserData(0, Idx)
	buff:SetClientUserData(2, BUFF_TYPE_PET_HETI)
	buff:SetClientUserData(3, nOwerId)
	
	local tagSkillInfo	= SkillMgr:getSpellStaticAtt(PET_HETI_SKILL_ID, 1)
	local IconPath;
	if bBattlePet then 
		IconPath = GetSkillIconPath()..tagSkillInfo.IconID2..".tga"
	else 
		IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga"
	end
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	local t_textures = {
		[szBuffName .. "Ground"]	= {path = t_buffBackPath[BUFF_TYPE_GOOD][0]},
		[szBuffName .. "Icon"]		= {path = IconPath},
	};
	for name, data in pairs(t_textures) do 
		local tex = getglobal(name)
		tex:SetTexture(data.path)	
	end

	local t_fonts = {
		[szBuffName .. "Overlay"]	= {value = ""},
	}
	for name, data in pairs(t_fonts) do
		local font = getglobal(name)
		font:SetText(data.value)
	end
	
	local t_richs = {
		szBuffName .. "duration",
	};
	for _, name in ipairs(t_richs) do 
		local rich = getglobal(name)
		rich:Clear()
		rich:clearHistory()
		rich:Hide()
	end
	buff:Show()
end

function AddMainFairyBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_FAIRY );
	buff:SetClientUserData( 3, nOwerId );
	local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetGeniusPath( diathesisDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();
end

function AddMainPetBuf( szBuffName, szText  )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, 0 );
	buff:SetClientUserData( 1, 0 );
	buff:SetClientUserData( 2, BUFF_TYPE_PET );
	buff:SetClientUserData( 3, 0 );
	buff:SetClientString( szText );

	local szBuffPath = "uires\\TuPianLei\\JiNeng\\Buff\\3086.tga";

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = szBuffPath },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local font = getglobal( szBuffName .. "Overlay" );
	if font ~= nil then
		font:SetText( "" );
	end
	
	local rich = getglobal( szBuffName .. "duration" );
	if rich ~= nil then
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end

	buff:Show();
end

function AddMainClanSkillBuf( szBuffName, szText )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, 0 );
	buff:SetClientUserData( 1, 0 );
	buff:SetClientUserData( 2, BUFF_TYPE_CLAN_SKILL );
	buff:SetClientUserData( 3, 0 );
	buff:SetClientString( szText );

	local szBuffPath = "uires\\TuPianLei\\JiNeng\\Buff\\3080.tga";

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = szBuffPath },
						};

	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local font = getglobal( szBuffName .. "Overlay" );
	font:SetText( "" );

	local rich = getglobal( szBuffName .. "duration" );
	rich:Clear();	
	rich:clearHistory();
	rich:Hide();

	buff:Show();
end

function AddMainPKBuf( szBuffName )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, PK_BUFF_ID );
	buff:SetClientUserData( 1, 1 );
	buff:SetClientUserData( 2, BUFF_TYPE_PK );
	buff:SetClientUserData( 3, 0 );
	
	local statusDef		= getStatusDef( PK_BUFF_ID, 1 );
	
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[statusDef.Type][statusDef.SubType] },
							[szBuffName .. "Icon"]		= { path = GetBuffIconPath( statusDef.IconID ) },
						};

	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );
	end

	local font = getglobal( szBuffName .. "Overlay" );
	font:SetText( "" );

	local rich = getglobal( szBuffName .. "duration" );
	rich:Clear();
	rich:clearHistory();
	rich:Hide();

	buff:Show();
end

function AddCoolFairyBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_FAIRY );
	buff:SetClientUserData( 3, nOwerId );
	-- TODO: 修改成支持显示精灵假buf图标
	local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
	
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetPetSkillIconPath( diathesisDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end
	buff:SetCooldownTimer( 0, 0, false );
	buff:Show();
end

function AddCoolPetBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_FAIRY );
	buff:SetClientUserData( 3, nOwerId );
	-- TODO: 修改成支持显示精灵假buf图标
	local diathesisDef = SkillMgr:getDiathesisDef( nId, nLv );
	
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetPetSkillIconPath( diathesisDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end
	buff:SetCooldownTimer( 0, 0, false );
	buff:Show();
end

function AddCoolPetGuanZhiBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_PET_GUAN_ZHI );
	buff:SetClientUserData( 3, nOwerId );
	-- TODO: 修改成支持显示精灵假buf图标
	local statusDef		= getStatusDef( nId, nLv );
	
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[statusDef.Type][statusDef.SubType] },
							[szBuffName .. "Icon"]		= { path = GetBuffIconPath( statusDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end
	buff:SetCooldownTimer( 0, 0, false );
	buff:Show();
end

function AddCoolRideBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_RIDE );
	buff:SetClientUserData( 3, nOwerId );
	-- TODO: 修改成支持显示精灵假buf图标
	local rideDef = getRideDef( nId, nLv );
	
	local IconPath = GetItemEquipIconPath()..rideDef.IconID..".tga";
					-- 如果没找到物品的图标则使用默认图标显示
					if not IsInExistence( IconPath ) then
						IconPath = DEFAULT_ITEM_PATH;
					end
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = rideDef.RideOnNum > 0 and GetRideBuffIcon( rideDef.IconID ) or
																	GetRideItemIcon( rideDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end
	buff:SetCooldownTimer( 0, 0, false );
	buff:Show();
end

function AddCoolPetHetiBuf(szBuffName, Idx, nOwerId,bBattlePet)
	local buff = getglobal(szBuffName)
	buff:SetClientUserData(0, Idx)
	buff:SetClientUserData(2, BUFF_TYPE_PET_HETI)
	buff:SetClientUserData(3, nOwerId)
	local IconPath;
	local tagSkillInfo	= SkillMgr:getSpellStaticAtt(PET_HETI_SKILL_ID, 1)
	if bBattlePet then
		IconPath = GetSkillIconPath()..tagSkillInfo.IconID2..".tga"
	else 
		IconPath = GetSkillIconPath()..tagSkillInfo.IconID1..".tga"
	end
	-- 如果没找到物品的图标则使用默认图标显示
	if not IsInExistence(IconPath) then
		IconPath = DEFAULT_ITEM_PATH
	end
	local t_textures = {
		[szBuffName .. "Ground"]	= {path = t_buffBackPath[BUFF_TYPE_GOOD][0]},
		[szBuffName .. "Icon"]		= {path = IconPath},
	};
	for name, data in pairs(t_textures) do 
		local tex = getglobal(name)
		tex:SetTexture(data.path)	
	end
	buff:SetCooldownTimer(0, 0, false)
	buff:Show()
end

-- t_buffInfo = { name = ? texPath = ?, startTime = ?, lifeTime = ?, id = ?, lv ?, overlay = ?, owernerId = ?, index = ? }
-- 设置已经存在的相同buff
function SetExisBuff( data )
	local buff		= getglobal( data["btnName"] );
	local nOverlay	= data["overlay"];
	local nStatusId	= data["statusID"];
	local nLv		= data["lv"];

	buff:SetClientUserData( 2, data["life"] );
	local buffOverlay = getglobal( data["btnName"] .. "Overlay" ); 
	buffOverlay:SetText( ( nOverlay == 0 or nOverlay == 1 ) and "" or nOverlay );

	local MIN_BLINK_TIME = 15;
	local rich = getglobal( data["btnName"].."duration" );
	rich:clearHistory();

	-- 若buff不能显示持续时间
	if canShowBuff( nStatusId, nLv ) and data["life"] < FOR_EVERY then
		local timeUnit;

		if (  data["life"] / 3600 ) >= 1 then
			timeUnit = math.floor(  data["life"] / 3600 ) .. "h";
		elseif (  data["life"] / 60 ) >= 1 then
			timeUnit = math.floor(  data["life"] / 60 ) .. "m";
		else
			timeUnit = math.floor( data["life"] ) .. "s";
		end
		
		-- 若剩余的时间大于15秒就不闪烁
		if math.floor( data["life"] / MIN_BLINK_TIME ) < 1 then
			rich:SetText( "#b#P" .. timeUnit, 255, 255, 255 );
		else
			rich:SetText( "#P" .. timeUnit, 255, 255, 255 );
		end
		rich:Show();
	else
		rich:Hide();
	end
end

-- 设置新的buff
function SetNewBuff( data )
	local buff		= getglobal( data["btnName"] );
	local nStatusId	= data["statusID"];
	local nLv		= data["lv"];
	local nOverlay	= data["overlay"];

	buff:SetClientUserData( 0, data["statusID"] );
	buff:SetClientUserData( 1, data["lv"] );
	buff:SetClientUserData( 2, data["life"] );
	buff:SetClientUserData( 3, data["ownerId"] );

	local statusDef	= getStatusDef( nStatusId, nLv );
	local buffBack	= getglobal( data["btnName"] .. "Ground" );
	if buffBack:GetTexture() ~= t_buffBackPath[statusDef.Type][statusDef.SubType] then
		buffBack:SetTexture( t_buffBackPath[statusDef.Type][statusDef.SubType] );		
	end

	local buffIcon = getglobal( data["btnName"] .. "Icon" );
	if buffIcon:GetTexture() ~= data["buffTexture"] then
		buffIcon:SetTexture( data["buffTexture"] );		
	end

	local buffOverlay = getglobal( data["btnName"] .. "Overlay" ); 
	buffOverlay:SetText( ( nOverlay == 0 or nOverlay == 1 ) and "" or nOverlay );

	local MIN_BLINK_TIME = 15;
	
	local rich = getglobal( data["btnName"].."duration" );
	rich:clearHistory();

	-- 若buff不能显示持续时间
	if canShowBuff( nStatusId, nLv ) and data["life"] < FOR_EVERY then
		local timeUnit;
		
		if (  data["life"] / 3600 ) >= 1 then
			timeUnit = math.floor(  data["life"] / 3600 ) .. "h";
		elseif (  data["life"] / 60 ) >= 1 then
			timeUnit = math.floor(  data["life"] / 60 ) .. "m";
		else
			timeUnit = math.floor( data["life"] ) .. "s";
		end
		
		-- 若剩余的时间大于15秒就不闪烁
		if math.floor( data["life"] / MIN_BLINK_TIME ) < 1 then
			rich:SetText( "#b#P" .. timeUnit, 255, 255, 255 );
		else
			rich:SetText( "#P" .. timeUnit, 255, 255, 255 );
		end
		rich:Show();
	else
		rich:Hide();
	end
	
	buff:Show();
end

function AddBuff( szBuffName, buffTex, start, duration, nStatusId, nLv, nOverlay, nOwnerId, nIndex ) 
	duration = math.ceil( duration );
	local buff		= getglobal( szBuffName );
	local nCurBtnID = buff:GetClientUserData( 0 );
	local nCurBtnLV = buff:GetClientUserData( 1 );
	
	if nCurBtnID == nStatusId and nCurBtnLV == nLv then
		SetExisBuff( { btnName = szBuffName, statusID = nStatusId, lv = nLv, overlay = nOverlay, life = duration, ownerId = nOwnerId,
						buffTexture = buffTex } );
	else
		SetNewBuff(  { btnName = szBuffName, statusID = nStatusId, lv = nLv, overlay = nOverlay, life = duration, ownerId = nOwnerId,
						buffTexture = buffTex } );
	end
end

-- 将队伍头像的buff显示效果同目标头像的Buff显示效果同步
function SynchronizeTeamBuff( data )
	local statusDef = getStatusDef( data["statusId"], data["statusLv"] );
	local btn = getglobal( data["name"] );
	btn:SetClientUserData( 0, data["statusId"] );
	btn:SetClientUserData( 1, data["statusLv"] );
	btn:SetClientUserData( 3, data["ownerId"] );
	local buffBack = getglobal( btn:GetName() .. "Ground" );
	if buffBack:GetTexture() ~= t_buffBackPath[statusDef.Type][statusDef.SubType] then
		buffBack:SetTexture( t_buffBackPath[statusDef.Type][statusDef.SubType] );		
	end

	local buffIcon = getglobal( btn:GetName() .. "Icon" );
	buffIcon:SetTexture( "uires\\TuPianLei\\JiNeng\\Buff\\"..statusDef.IconID..".tga" );
	btn:SetCooldownTimer( data["startTime"], data["totalTime"], data["coolState"], true );
	btn:Show();
end

function AddCoolBuffer( szBuffName, buffTex, leaveTime, duration, nStatusId, nLv, nOverlay, nOwnerId, Buf )
	-- 若是buff实际上已经消失，处于同步则不作任何处理，让其自动消失
	if ( duration - leaveTime < 0 ) or ( duration < 0 ) then
		assert( "id = "..nStatusId.."，lv = "..nLv.."的buff时间小于0，(duration - leaveTime) = "..(duration - leaveTime).."，duration = "..duration );
	end
	if not Buf:isShow() or ( duration - leaveTime < 0 ) or ( duration < 0 ) then
		return false;
	end

	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nStatusId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, leaveTime );
	buff:SetClientUserData( 3, nOwnerId );
	local statusDef		= getStatusDef( nStatusId, nLv );
	local buffBack = getglobal( szBuffName .. "Ground" );
	if buffBack:GetTexture() ~= t_buffBackPath[statusDef.Type][statusDef.SubType] then
		buffBack:SetTexture( t_buffBackPath[statusDef.Type][statusDef.SubType] );		
	end

	local buffOverlay = getglobal( szBuffName .. "Overlay" ); 
	buffOverlay:SetText( ( nOverlay == 0 or nOverlay == 1 ) and "" or nOverlay );

	local buffIcon = getglobal( szBuffName .. "Icon" );
	if buffIcon:GetTexture() ~= buffTex then
		buffIcon:SetTexture( buffTex );		
	end
	
	assert( duration >= 0 , "时间为负,非法!!!" );
	if canShowBuff( nStatusId, nLv ) then
		buff:SetCooldownTimer( duration - leaveTime, duration, true, true );
	else
		buff:SetCooldownTimer( duration - leaveTime, duration, false );
	end

	buff:Show();
	return true;
end

function DelBuff( szBuffName )
	local buff = getglobal(szBuffName);
	if buff == nil then 
		return
	end
	buff:Hide();
	buff:SetClientUserData( 0, 0 );
	buff:SetClientUserData( 1, 0 );
	buff:SetClientUserData( 2, 0 );
	buff:SetClientUserData( 3, 0 );
end

function DelCollBuff( szBuffName )
	local buff = getglobal(szBuffName);
	buff:Hide();
	buff:SetClientUserData( 0, 0 );
	buff:SetClientUserData( 1, 0 );
	buff:SetClientUserData( 3, 0 );
	buff:SetCooldownTimer( 0, 0, false );
end

function AddCoolHaiTanBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_HAI_TAN );
	buff:SetClientUserData( 3, nOwerId );
	
	local statusDef	= getStatusDef( nId, nLv );
	 
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetBuffIconPath( statusDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end
	buff:SetCooldownTimer( 0, 0, false );
	buff:Show();
end

function AddMainHaiTanBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_HAI_TAN );
	buff:SetClientUserData( 3, nOwerId );	
	
	local statusDef	= getStatusDef( nId, nLv );

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = GetBuffIconPath( statusDef.IconID ) },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();
end

function AddMainSafeBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_SAFE );
	buff:SetClientUserData( 3, nOwerId );

	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = "uires\\TuPianLei\\JiNeng\\Buff\\ZhuCheng  BUFF.tga" },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end

	local t_fonts = { 
						[szBuffName .. "Overlay"]	= { value = "" },
					};
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data.value );		
	end
	
	local t_richs = { 
						szBuffName .. "duration",
					};
	for _, name in ipairs( t_richs ) do 
		local rich = getglobal( name );
		rich:Clear();	
		rich:clearHistory();
		rich:Hide();
	end
	buff:Show();
end

function AddCoolSafeBuf( szBuffName, nId, nLv, nOwerId )
	local buff = getglobal( szBuffName );
	buff:SetClientUserData( 0, nId );
	buff:SetClientUserData( 1, nLv );
	buff:SetClientUserData( 2, BUFF_TYPE_SAFE );
	buff:SetClientUserData( 3, nOwerId );
	
	local t_textures =	{
							[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
							[szBuffName .. "Icon"]		= { path = "uires\\TuPianLei\\JiNeng\\Buff\\10000.tga" },
						};
	for name, data in pairs( t_textures ) do 
		local tex = getglobal( name );
		tex:SetTexture( data.path );		
	end
	buff:SetCooldownTimer( 0, 0, false );
	buff:Show();
end

function AddMainBattleLimitBuf( mainplayer, szBuffName )
	if mainplayer == nil then return end
	local nLeftTime = mainplayer:getBattleLimitLeftTime();
	if nLeftTime > 0 then
		local buff = getglobal( szBuffName );
		buff:SetClientUserData( 0, 0 );
		buff:SetClientUserData( 1, GameMgr:getTickTime() );
		buff:SetClientUserData( 2, BUFF_TYPE_BATTLE_LIMIT );
		buff:SetClientUserData( 3, nLeftTime );

		local t_textures =	{
						[szBuffName .. "Ground"]	= { path = t_buffBackPath[BUFF_TYPE_GOOD][0] },
						[szBuffName .. "Icon"]		= { path = "uires\\TuPianLei\\JiNeng\\Buff\\10000.tga" },
					};
		for name, data in pairs( t_textures ) do 
			local tex = getglobal( name );
			tex:SetTexture( data.path );		
		end

		local t_fonts = { 
							[szBuffName .. "Overlay"]	= { value = "" },
						};
		for name, data in pairs( t_fonts ) do
			local font = getglobal( name );
			font:SetText( data.value );		
		end
		
		local rich = getglobal( szBuffName .. "duration" )
		-- 剩余时间
		local timeUnit;

		if ( nLeftTime / 3600 ) >= 1 then
			timeUnit = math.floor(  nLeftTime / 3600 ) .. "h";
		elseif (  nLeftTime / 60 ) >= 1 then
			timeUnit = math.floor( nLeftTime / 60 ) .. "m";
		else
			timeUnit = math.floor( nLeftTime ) .. "s";
		end
		local MIN_BLINK_TIME	= 15;
		-- 若剩余的时间大于15秒就不闪烁
		if math.floor( nLeftTime / MIN_BLINK_TIME ) < 1 then
			rich:SetText( "#b#P" .. timeUnit, 255, 255, 255 );
		else
			rich:SetText( "#P" .. timeUnit, 255, 255, 255 );
		end
		buff:Show();	
	end
end

function GetMySkillList()
	local t_MyList = {};
	local mySkill = GameClanManager:getPlayerClanSkillInfo();
	local clanSkill = GameClanManager:getClanSkillInfo();
	local szText ="#cff6e1c公会技能：#n\n";
	for i=1,mySkill.Num do
		local info = mySkill.ClanSkill[i-1];
		local valiLv = clanSkill.ClanSkill[info.ClanSkillIdx].ValidLevel;
		if valiLv ~= 0 then
			if info.Level > valiLv then
				table.insert( t_MyList, { id = info.SkillID, lv = valiLv } );
			else
				if info.Level ~= 0 then
					table.insert( t_MyList, { id = info.SkillID, lv = info.Level } );
				end
			end
		end
	end

	for i=1, #t_MyList do
		local skillDef = GameClanManager:getClanSkillDef( t_MyList[i].id, t_MyList[i].lv );
		if skillDef.AutoLearn == 0 then --如果不是作用于个人的技能则不显示
			szText = szText .. skillDef.Name .. skillDef.Level .. "级：" .. skillDef.SkillDesc .. "#n\n";
		end
	end

	return szText;
end

function SetOnBeachBuff( In, BuffSec )
	if BuffSec <= 0 then 
		return ;
	end

	local mainplayer	= ActorMgr:getMainPlayer();
	local StatusDef 	= getStatusDef( BEACH_BUFF_ID,1 );
	BeachBuffIn 	= In;
	BeachBuffSec	= BuffSec;
	if In == 1 then
		for i = 1, MAX_MAIN_GOOD_BUF_COUNT do
			local btn = getglobal("PartyMemberBuffTooltipBuff"..i);
			if btn:IsShown() then
				if btn:GetClientUserData(0) == BEACH_BUFF_ID then
					beachBtn = i;
					break;
				end
			elseif not btn:IsShown() then
		--		BeachBufftimer = BeachBufftimer > 0 and BeachBufftimer or getServerTime().sec;
				AddBuff( btn:GetName(), GetBuffIconPath( StatusDef.IconID ),0, 
						BuffSec, BEACH_BUFF_ID, 1, 
						1, mainplayer:GetID() );
				BeachBufftimer = getServerTime().sec;
				beachBtn = i;
				break;
			end
		end
	else
		DelBuff( "PartyMemberBuffTooltipBuff"..beachBtn);
		BeachBufftimer = 0;
		beachBtn = 0;
	end
end

function HandleBeachBuffTime( nTime, nName )
	if nTime < 0 then
		return;
	end

	local button	= getglobal(nName);
	if button:GetClientID() ~= beachBtn then
		return;
	end
	local MIN_BLINK_TIME	= 15;
	local rich = getglobal( nName.."duration" );

	local duration =  math.ceil( nTime + BeachBufftimer - getServerTime().sec );
	if duration >= FOR_EVERY then
		return
	end
	if duration <= 0 then 
		DelBuff( "PartyMemberBuffTooltipBuff"..beachBtn);
		BeachBufftimer = 0;
		beachBtn = 0;
		return;
	end
	local timeUnit;
	if ( duration / 3600 ) >= 1 then
		timeUnit = math.floor( duration / 3600 ) .. "h";
	elseif ( duration / 60 ) >= 1 then
		timeUnit = math.floor( duration / 60 ) .. "m";
	else
		timeUnit = duration .. "s";
	end
	
	if not rich:IsShown() then
		rich:Show()
	end
	-- 剩余的时间大于15秒就不闪烁
	if duration < MIN_BLINK_TIME then
		rich:SetText( "#b#P" .. timeUnit, 255, 255, 255 );
	else
		rich:SetText( "#P" .. timeUnit, 255, 255, 255 );
	end
end
-- 查询是否使用活力获得海滩低经验
function askIfBeginBeachLowExp()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local equip = mainplayer:getEquip();
	if equip:isOnChairTmp() then 
		local szText = "今日充足日光浴经验已经用完，继续使用沙滩椅会消耗活力值获取较少的经验，是否继续使用沙滩椅？";
		MessageBox( "提 示",szText );
		MessageBoxFrame:SetClientString( "沙滩椅使用" );
	end
end
-- 沙滩椅取消使用
function BeachUnUseChairTmp()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local equip = mainplayer:getEquip();
	if equip:isOnChairTmp() then 
		DropTempPropButton_OnClick()
	end
end

function getSamePetNameIndex( index,petInfo )
	local mainplayer 	= ActorMgr:getMainPlayer();
	if mainplayer == nil then return; end
	local petMgr	= mainplayer:getPet();
	local curPet	= petMgr:getPetInfo( index - 1 );
	local nCurIndex = 1;
	for i = 1 , petMgr:getPetNum() do 
		local PetInfo = petMgr:getPetInfo( i - 1 );
		if i ~= index and petMgr:isExtraBattlePet( PetInfo ) and curPet.Name == PetInfo.Name then 
			nCurIndex = nCurIndex + 1;
		end
	end
	local nIndex = 0;
	if nCurIndex > 1 then 
		for j = 1 , petMgr:getPetNum() do 
			local PetInfo = petMgr:getPetInfo( j - 1 );
			if j == index then
				nIndex = nIndex + 1; 
				break;
			end
			if curPet.Name == PetInfo.Name and petMgr:isExtraBattlePet( PetInfo ) then
				if PetInfo.WID.nHeight ~= petInfo.WID.nHeight and PetInfo.WID.nLow ~= petInfo.WID.nLow then 
					nIndex = nIndex + 1;
				end
			end
		end
	else 
		return 0;	
	end	
	return nIndex;
end 