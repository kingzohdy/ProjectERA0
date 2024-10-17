-- ѡ�˽����������ƶ�����
local MOVE_FRAME_NOT_START_PERIOD	= -1;
local MOVE_FRAME_DOWN_PERIOD		= 0;
local MOVE_FRAME_UP_PERIOD			= 1;
local MOVE_FRAME_COLLISON_PERIOD	= 2;
local MOVE_FRAME_ZUNI_PERIOD		= 3;
local MOVE_FRAME_END_PERIOD			= 4;


local MOVE_FRAME_DECR_COLLISON_PERIOD	= 0;
local MOVE_FRAME_INCR_COLLISON_PERIOD	= 1;



local MOVE_FRAME_UP_DECR_ZUNI_PERIOD		= 0;
local MOVE_FRAME_UP_INCR_ZUNI_PERIOD		= 1;

local MOVE_FRAME_DOWN_INCR_ZUNI_PERIOD		= 2;
local MOVE_FRAME_DOWN_DECR_ZUNI_PERIOD		= 3;

-- Ĭ�ϳ�ʼ�����ٶ�
local DEFAULT_DOWN_SPEED			= 800;
-- ���¼��ٵ������˶�
local ADJUST_DOWN_INCR_ZU_NI_SPEED	= 0.3;
-- ���ϼ��ٵ������˶�
local ADJUST_UP_DECR_ZU_NI_SPEED	= 0.5;
-- �������Ĭ�������Ļ����Yƫ�ƣ�ȡ
local ADJUST_INIT_UP_POS			= 800;
local t_SelectFrameMoveFrameAnimControl = 
{	["isPlayMoveFrameAnim"] = false,	["startTime"] = 0, ["periodType"] = MOVE_FRAME_NOT_START_PERIOD, MAX_MOVE_FRAME_TIME = 5,
	["curSpeed"] = 0,					["collisionMode"] = MOVE_FRAME_DECR_COLLISON_PERIOD,	["zuNiLastMaxOffsetY"] = 0,
	["zuNiMode"] = MOVE_FRAME_UP_DECR_ZUNI_PERIOD, ["curAnimframe"] = "",

	["frameConfig"] = { ["backBtn"] = { ["farestTopHeight"]	= 768 - 92 - 541,	
										["frames"] = {	["SelectFrame"]				= "SelectFrameBackGround",
														["CSSelectRaceDescFrame"]	= "CSSelectRaceDescFrameBackGround",
														["CSCreateFrame"]			= "CSCreateFrameBackGround", 
													},

										["anchor"] = {	["SelectFrame"] = { ["myPoint"] = "topleft", ["relPoint"] = "topleft", ["xOffset"] = 46 }, 
														["CSSelectRaceDescFrame"] = { ["myPoint"] = "topright", ["relPoint"] = "topright", ["xOffset"] = -46 },
														["CSCreateFrame"] = { ["myPoint"] = "topleft", ["relPoint"] = "topleft", ["xOffset"] = 46 }
													},		

										["pingHengHeight"]	= 109,				["topZheZhaoBottom"]	= 77 
										},

						["lianTiao"]= { ["frames"] = {	["SelectFrame"] = { "SelectFrameLeftTieLianBtn", "SelectFrameRightTieLianBtn" },

														["CSSelectRaceDescFrame"] = {	"CSSelectRaceDescFrameLeftTieLianBtn", 
																						"CSSelectRaceDescFrameRightTieLianBtn" 
																					},

														["CSCreateFrame"] = { "CSCreateFrameLeftTieLianBtn", "CSCreateFrameRightTieLianBtn" },
													},	
										["standardSize"]	= 54,
										["standardAnchorY"] = 69 
										}, 
					},
	
	--									����������ٶ�								������ٶ�
	["DOWN_SPEED"] = DEFAULT_DOWN_SPEED, ["UP_SPEED"] = DEFAULT_DOWN_SPEED * 0.8,	["ZUNI_SPEED_ACCELEARTION"] = DEFAULT_DOWN_SPEED * 4,
};

function GetSelectFrameMoveFrameAnimControl()
	return t_SelectFrameMoveFrameAnimControl;
end

t_SelectFrameMoveFrameAnimControl["isPlayingAnim"] =
function ( self )
	return self["isPlayMoveFrameAnim"];
end

t_SelectFrameMoveFrameAnimControl["init"] =
function ( self, data )
	if data["curAnimframe"] ~= self["curAnimframe"] then
		return;
	end

	self["isPlayMoveFrameAnim"]	= false;
	self["startTime"]			= 0;
	self["periodType"]			= MOVE_FRAME_NOT_START_PERIOD;
	self["curSpeed"]			= 0;
	self["curAnimframe"]		= "";
end

t_SelectFrameMoveFrameAnimControl["stop"] =
function ( self )
	self["isPlayMoveFrameAnim"]	= false;
	self["startTime"]			= 0;
	self["periodType"]			= MOVE_FRAME_END_PERIOD;
	self["curSpeed"]			= 0;
	--self["curAnimframe"]		= "";
	--checkNeedTurnToCharacterPage();
end

t_SelectFrameMoveFrameAnimControl["begin"] =
function ( self, data )
	self["isPlayMoveFrameAnim"]	= true;
	self["startTime"]			= os.clock();
	self["periodType"]			= MOVE_FRAME_DOWN_PERIOD;
	self["curSpeed"]			= self["DOWN_SPEED"];
	self["curAnimframe"]		= data["curAnimframe"];
	self:Private_RollBackAllBtn();
end

t_SelectFrameMoveFrameAnimControl["update"] =
function ( self )
	if not self["isPlayMoveFrameAnim"] then
		return;
	end

	if self["periodType"] == MOVE_FRAME_DOWN_PERIOD then
		self:Private_UpdateDownPeriod();
	elseif self["periodType"] == MOVE_FRAME_COLLISON_PERIOD then
		self:Private_UpdateCollisionPeriod();
	elseif self["periodType"] == MOVE_FRAME_UP_PERIOD then
		self:Private_UpdateUpPeriod();
	elseif self["periodType"] == MOVE_FRAME_ZUNI_PERIOD then
		self:Private_UpdateZuNiPeriod();
	end
end

-- �����пؼ���Ϊ��ʼ״̬
t_SelectFrameMoveFrameAnimControl["Private_RollBackAllBtn"] =
function ( self )
	local szCurFrame	= self["curAnimframe"];
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );

	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	if backBtn == nil then
		print( "szCurFrame = "..szCurFrame.."--130" );
	end

	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], 
						-( 109 + 541 ) );
	local lianTiaoCofig = self["frameConfig"]["lianTiao"];
	local szCurFrame	= self["curAnimframe"];
	local t_hideIcons	= lianTiaoCofig["frames"][szCurFrame];
	util.HideIcons( t_hideIcons );
end

-- ��������ʱ��
t_SelectFrameMoveFrameAnimControl["Private_UpdateDownPeriod"] =
function ( self )
	self:Private_UpdateNameBack_OnDownPeriod();
	self:Private_UpdateLianTiao_OnDownPeriod();
end

-- ��ѯ�Ƿ��������ʱ��
t_SelectFrameMoveFrameAnimControl["Private_isEnd_OnDownPeriod"] =
function ( self, data )
	local nCurYOffset	= data["yOffset"];
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	return nCurYOffset >= backBtnCofig["farestTopHeight"];
end

-- �л�����ײģʽ
t_SelectFrameMoveFrameAnimControl["Private_EnterCollisionMode"] =
function ( self )
	self["periodType"]		= MOVE_FRAME_COLLISON_PERIOD;
	self["startTime"]		= os.clock();
	self["collisionMode"]	= MOVE_FRAME_DECR_COLLISON_PERIOD;
end

-- �л�������ģʽ
t_SelectFrameMoveFrameAnimControl["Private_EnterCollisionMode"] =
function ( self )
	self["periodType"]		= MOVE_FRAME_UP_PERIOD;
	self["startTime"]		= os.clock();
	self["curSpeed"]		= self["UP_SPEED"];
end

-- ����ʱ�ڸ������ֵװ�
t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnDownPeriod"] =
function ( self )
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local nCurTime		= os.clock();
	local nCurYOffset	= self["curSpeed"] * ( nCurTime - self["startTime"] ) - ADJUST_INIT_UP_POS;
	if nCurYOffset > backBtnCofig["farestTopHeight"] then
		nCurYOffset = backBtnCofig["farestTopHeight"];
	end
	
	local szCurFrame	= self["curAnimframe"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );
	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], nCurYOffset );

	if self:Private_isEnd_OnDownPeriod( { ["yOffset"] = nCurYOffset } ) then
		self:Private_EnterCollisionMode();
	end
end

-- ����ʱ�ڸ�������
t_SelectFrameMoveFrameAnimControl["Private_UpdateLianTiao_OnDownPeriod"] =
function ( self )
	local szCurFrame	= self["curAnimframe"];
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local btn			= getglobal( backBtnCofig["frames"][szCurFrame] );
	local nCurRealTop	= btn:GetAnchorOffsetY();
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local lianTiaoCofig = self["frameConfig"]["lianTiao"];
	
	--  ��������嶥����û�����ϲ����ֵײ�
	if nCurRealTop < backBtnCofig["topZheZhaoBottom"] then
		local t_hideIcons = lianTiaoCofig["frames"][szCurFrame];
		util.HideIcons( t_hideIcons );
		return;
	end

	-- ��������嶥�������ϲ����ֵײ�����δ�ﵽƽ��λ��
	-- ��������嶥������ƽ��λ��
	-- ����ƽ���λ�õ�ʱ�򣬸߶���standardSize��������topZheZhaoBottom��ʱ�򣬸߶���0
	local nStandardAnchorY	= backBtnCofig["pingHengHeight"] - lianTiaoCofig["standardAnchorY"];
	local nCurAnchorY		= nCurRealTop - lianTiaoCofig["standardAnchorY"];
	local nLianTiaoHeight	= lianTiaoCofig["standardSize"] + ( nCurAnchorY - nStandardAnchorY );

	--print( "szCurFrame = "..szCurFrame.."--Private_UpdateLianTiao_OnDownPeriod--201" );

	for _, name in ipairs( lianTiaoCofig["frames"][szCurFrame] ) do
		local lianTiaoBtn = getglobal( name );

		lianTiaoBtn:SetSize( 12, nLianTiaoHeight );
		lianTiaoBtn:SetNormalTextureSize( 12, nLianTiaoHeight );
		if not lianTiaoBtn:IsShown() then
			lianTiaoBtn:Show();
		end
	end
end

-- ��������ʱ��
t_SelectFrameMoveFrameAnimControl["Private_UpdateUpPeriod"] =
function ( self )
	self:Private_UpdateNameBack_OnUpPeriod();
	self:Private_UpdateLianTiao_OnUpPeriod();
end

-- ����ʱ�ڸ�������
t_SelectFrameMoveFrameAnimControl["Private_UpdateLianTiao_OnUpPeriod"] =
function ( self )
	self:Private_UpdateLianTiao_OnDownPeriod();
end

-- ����ʱ�ڸ������ֵװ�
t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnUpPeriod"] =
function ( self )
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local nCurTime		= os.clock();
	local nCurYOffset	= backBtnCofig["farestTopHeight"] - self["curSpeed"] * ( nCurTime - self["startTime"] );
	
	if nCurYOffset < backBtnCofig["pingHengHeight"] then
		nCurYOffset = backBtnCofig["pingHengHeight"];
	end

	local szCurFrame	= self["curAnimframe"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );
	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], nCurYOffset );

	if self:Private_isEnd_OnUpPeriod( { ["yOffset"] = nCurYOffset } ) then
		self:Private_EnterZuNiMode();
	end
end

-- �Ƿ�ﵽ��ƽ��λ��
t_SelectFrameMoveFrameAnimControl["Private_isEnd_OnUpPeriod"] =
function ( self, data )
	local nCurYOffset	= data["yOffset"];
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	return nCurYOffset <= backBtnCofig["pingHengHeight"];
end

-- �л�������ģʽ
t_SelectFrameMoveFrameAnimControl["Private_EnterZuNiMode"] =
function ( self )
	self["periodType"]		= MOVE_FRAME_ZUNI_PERIOD;
	self["startTime"]		= os.clock();
	self["zuNiMode"]		= MOVE_FRAME_UP_DECR_ZUNI_PERIOD;
	self["curSpeed"]		= self["curSpeed"] * ADJUST_UP_DECR_ZU_NI_SPEED;
end

-- ���������˶�ʱ��
t_SelectFrameMoveFrameAnimControl["Private_UpdateZuNiPeriod"] =
function ( self )
	self:Private_UpdateNameBack_OnZuNiPeriod();
	self:Private_UpdateLianTiao_OnZuNiPeriod();
end

-- �����˶�ʱ�ڸ�������
t_SelectFrameMoveFrameAnimControl["Private_UpdateLianTiao_OnZuNiPeriod"] =
function ( self )
	self:Private_UpdateLianTiao_OnDownPeriod();
end

-- �����˶�ʱ�ڸ������ֵװ�
t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnZuNiPeriod"] =
function ( self )
	if self["zuNiMode"] == MOVE_FRAME_UP_DECR_ZUNI_PERIOD then
		self:Private_UpdateNameBack_OnUpDecrZuNiPeriod();
	elseif self["zuNiMode"] == MOVE_FRAME_DOWN_INCR_ZUNI_PERIOD then
		self:Private_UpdateNameBack_OnDownIncrZuNiPeriod();
	elseif self["zuNiMode"] == MOVE_FRAME_DOWN_DECR_ZUNI_PERIOD then
		self:Private_UpdateNameBack_OnDownDecrZuNiPeriod();
	elseif self["zuNiMode"] == MOVE_FRAME_UP_INCR_ZUNI_PERIOD then
		self:Private_UpdateNameBack_OnUpIncrZuNiPeriod();
	end
end

t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnUpDecrZuNiPeriod"] =
function ( self )
	local nCurTime	= os.clock();
	local nPassTime	= nCurTime - self["startTime"];
	local nCurSpeed = self["curSpeed"] - nPassTime * self["ZUNI_SPEED_ACCELEARTION"];
	local nMaxTime	= self["curSpeed"] / self["ZUNI_SPEED_ACCELEARTION"];
	
	local nCalculateTime	= math.min( nPassTime, nMaxTime );
	local nCurYOffset		= self["curSpeed"] * nCalculateTime + 0.5 * self["ZUNI_SPEED_ACCELEARTION"] * nCalculateTime ^ 2;

	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local szCurFrame	= self["curAnimframe"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );
	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], 
						backBtnCofig["pingHengHeight"] - nCurYOffset );

	if nCurSpeed <= 0 then
		self:Private_ChangeZuNiMode( { ["mode"] = MOVE_FRAME_DOWN_INCR_ZUNI_PERIOD, ["speed"] = 0, ["zuNiLastMaxOffsetY"] = nCurYOffset } );
	end
end

t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnDownIncrZuNiPeriod"] =
function ( self )
	local nCurTime		= os.clock();
	local nPassTime		= nCurTime - self["startTime"];
	local nCurChgYOffset= self["curSpeed"] * nPassTime + 0.5 * self["ZUNI_SPEED_ACCELEARTION"] * nPassTime ^ 2;
	
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local nCurYOffset	= backBtnCofig["pingHengHeight"] - self["zuNiLastMaxOffsetY"] + nCurChgYOffset;
	if nCurYOffset > backBtnCofig["pingHengHeight"] then
		nCurYOffset = backBtnCofig["pingHengHeight"];
	end
	
	local szCurFrame	= self["curAnimframe"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );
	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], nCurYOffset );

	if nCurYOffset >=  backBtnCofig["pingHengHeight"] then
		local nMaxTime = ( 2 * self["zuNiLastMaxOffsetY"] / self["ZUNI_SPEED_ACCELEARTION"] ) ^ 0.5;
		self:Private_ChangeZuNiMode( {	["mode"] = MOVE_FRAME_DOWN_DECR_ZUNI_PERIOD, 
										["speed"] = nMaxTime * self["ZUNI_SPEED_ACCELEARTION"] * ADJUST_DOWN_INCR_ZU_NI_SPEED, 
										["zuNiLastMaxOffsetY"] = 0 } );
	end
end

t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnDownDecrZuNiPeriod"] =
function ( self )
	local nCurTime	= os.clock();
	local nPassTime	= nCurTime - self["startTime"];
	local nCurSpeed = self["curSpeed"] - nPassTime * self["ZUNI_SPEED_ACCELEARTION"];
	local nMaxTime	= self["curSpeed"] / self["ZUNI_SPEED_ACCELEARTION"];

	local nCalculateTime	= math.min( nPassTime, nMaxTime );
	local nCurYOffset		= self["curSpeed"] * nCalculateTime + 0.5 * self["ZUNI_SPEED_ACCELEARTION"] * nCalculateTime ^ 2;
	
	local szCurFrame	= self["curAnimframe"];
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );
	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], backBtnCofig["pingHengHeight"] + nCurYOffset );

	if nCurSpeed <= 0 then
		self:Private_ChangeZuNiMode( { ["mode"] = MOVE_FRAME_UP_INCR_ZUNI_PERIOD, ["speed"] = 0, ["zuNiLastMaxOffsetY"] = nCurYOffset } );
	end
end

t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnUpIncrZuNiPeriod"] =
function ( self )
	local nCurTime		= os.clock();
	local nPassTime		= nCurTime - self["startTime"];
	local nCurChgYOffset= self["curSpeed"] * nPassTime + 0.5 * self["ZUNI_SPEED_ACCELEARTION"] * nPassTime ^ 2;

	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local nCurYOffset	= backBtnCofig["pingHengHeight"] + self["zuNiLastMaxOffsetY"] - nCurChgYOffset;
	if nCurYOffset < backBtnCofig["pingHengHeight"] then
		nCurYOffset = backBtnCofig["pingHengHeight"];
	end
	
	local szCurFrame	= self["curAnimframe"];
	local backBtnCofig	= self["frameConfig"]["backBtn"];
	local backBtn		= getglobal( backBtnCofig["frames"][szCurFrame] );
	local t_anchorConfig= backBtnCofig["anchor"][szCurFrame];
	backBtn:SetPoint( t_anchorConfig["myPoint"], szCurFrame, t_anchorConfig["relPoint"], t_anchorConfig["xOffset"], nCurYOffset );
	if nCurYOffset <=  backBtnCofig["pingHengHeight"] then
		--print( "Private_UpdateNameBack_OnUpIncrZuNiPeriod--nCurYOffset = "..nCurYOffset.."--325" );
		self:stop();
	end
end

-- �л������µ������˶�
t_SelectFrameMoveFrameAnimControl["Private_ChangeZuNiMode"] =
function ( self, data )
	self["startTime"]			= os.clock();
	self["zuNiMode"]			= data["mode"];
	self["curSpeed"]			= data["speed"];
	self["zuNiLastMaxOffsetY"]	= data["zuNiLastMaxOffsetY"];
end

-- ������ײʱ��
t_SelectFrameMoveFrameAnimControl["Private_UpdateCollisionPeriod"] =
function ( self )
	self:Private_UpdateNameBack_OnCollisionPeriod();
	self:Private_UpdateLianTiao_OnCollisionPeriod();
end

-- ��ײʱ�ڸ�������
t_SelectFrameMoveFrameAnimControl["Private_UpdateLianTiao_OnCollisionPeriod"] =
function ( self )
	self:Private_UpdateLianTiao_OnDownPeriod();
end

-- ������ײʱ�ڸ����������
t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnDecrCollisionPeriod"] =
function ( self )
end

-- ������ײʱ�ڸ����������
t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnIncrCollisionPeriod"] =
function ( self )
end

-- ��ײʱ�ڸ������ֵװ�
t_SelectFrameMoveFrameAnimControl["Private_UpdateNameBack_OnCollisionPeriod"] =
function ( self )
	if self["collisionMode"] == MOVE_FRAME_DECR_COLLISON_PERIOD then
		self:Private_UpdateNameBack_OnDecrCollisionPeriod();
	elseif self["collisionMode"] == MOVE_FRAME_INCR_COLLISON_PERIOD then
		self:Private_UpdateNameBack_OnIncrCollisionPeriod();
	end
end