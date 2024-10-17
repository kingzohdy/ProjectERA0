local MAX_BTN_NUM = 10
local raidus = 60
local speed = 0
local angle = 0
local startTime
local order
local imageIdx = 1

local function RandomPos()
	local t = {}
	for i=1,MAX_BTN_NUM do
		table.insert( t, i-1)
	end
	for i=1,MAX_BTN_NUM do
		local nIdx = math.random( 1, #t )
		local btn = getglobal( "PasswdCardFrameNumBtn" .. i-1 )
		btn:SetText( t[nIdx] )
		btn:SetClientID( t[nIdx] )
		table.remove( t, nIdx )
	end
end

local function Start()
	speed = 100
	startTime = os.clock()
	order = math.random(0,1) == 0 and -1 or 1
end

local function Stop()
	speed = 0
end

local function SetImage()
	local path = LoginMgr:getPasswdCardPath( imageIdx )
	PasswdCardFrameCodeTex:SetTexture( path )
end

local function MoveBtn()
	angle = angle + speed*(os.clock()-startTime)*order
	startTime = os.clock()
	for i=1,MAX_BTN_NUM do 
		local nCur = angle + (i-1)*36
		local x,y = raidus*math.cos( nCur*math.pi/180 ),raidus*math.sin( nCur*math.pi/180 )
		local btn = getglobal( "PasswdCardFrameNumBtn" .. i-1 )
		btn:SetPoint( "center","PasswdCardFrameLockTex","center",x,y )
	end
end

function PasswdCardFrame_OnLoad()
	this:RegisterEvent( "GE_NEED_PASSWD_CARD" )
	this:setUpdateTime( 0 )
	Start()
end

function PasswdCardFrame_OnEvent()
	if arg1 == "GE_NEED_PASSWD_CARD" then
		PasswdCardFrame:Show()
	end
end

function PasswdCardFrame_OnShow()
	imageIdx = 1
	angle = 0
	SetImage()
	for i=1,3 do
		local font = getglobal( "PasswdCardFrameNumFont" .. i )
		font:SetText( "" )
		LoginMgr:SetPasswdCardInfo( i - 1, "" )
	end
end

function PasswdCardFrame_OnUpdate()
	if speed > 0 then
		MoveBtn()
	end
end

function PasswdFrameNumBtn_OnClick()
	local font = getglobal( "PasswdCardFrameNumFont" .. imageIdx )
	if string.len( font:GetText() ) < 2 then
		font:SetText( font:GetText() .. this:GetClientID() )
	end
	LoginMgr:SetPasswdCardInfo( imageIdx - 1, font:GetText() ) 
	if string.len( font:GetText() ) == 2 then
		imageIdx = imageIdx + 1
		if imageIdx > 3 then 
			imageIdx = 3
			return
		end
		SetImage()
	end
	for i=1,MAX_BTN_NUM do
		local btn = getglobal( "PasswdCardFrameNumBtn" .. (i-1) )
	end
	RandomPos()
	this:DisHighlight()
	this:DisChecked()
	PasswdFrameNumBtn_OnLeave()

end

function PasswdFrameNumBtn_OnEnter()
	for i=1,MAX_BTN_NUM do
		local font = getglobal( "PasswdCardFrameNumBtn" .. (i-1) .. "Font" )
		font:Hide()
	end
	Stop()
end

function PasswdFrameNumBtn_OnLeave()
	for i=1,MAX_BTN_NUM do
		local font = getglobal( "PasswdCardFrameNumBtn" .. (i-1) .. "Font" )
		font:Show()
	end
	Start()
end

function PasswdFrameOKBtn_OnClick()
	LoginMgr:requestAuthPasswdCard()
end

function PasswdFrameClearBtn_OnClick()
	PasswdCardFrame_OnShow()
end

function PasswdCardFrameCloseBtn_OnClick()
	PasswdCardFrame:Hide()
	LoginMgr:breakNetConnect()
	LoginSelectServerBtnFont:SetText("当前服务器："..SelectWorldInfo.world )
end